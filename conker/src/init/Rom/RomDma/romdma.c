#include <ultra64.h>

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order it first
 * sees each name. */
static OSMesg gPiCmdBuffer[200];   // the PI manager's command queue buffer
static OSMesg gMessage0[300];
static OSMesgQueue gPiCmdQueue;   // the PI manager's command queue
static OSMesgQueue gMessageQueue0; // completions of the non-blocking reads
OSIoMesg gRomIoMesg;
static OSMesg gMessages[3];
OSMesgQueue gMessageQueue[3];    // completions of the blocking reads, one per game thread
static OSIoMesg gRomReadRing[300]; // the ring of non-blocking reads
static u8 gRomReadNext;            // next slot of gRomReadRing
static u8 gRomReadsInFlight;            // non-blocking reads in flight
vu8 gRomCopyActive; // PI interlock flags shared with the PI manager thread (init/Os/DevMgr/devmgr)
vu8 gPiDmaReadBusy;
u8 D_8003A574; // not referenced
vu8 gPiThreadStopped;

#include "conker/os.h"
#include "conker/rom.h"


/* Starts the PI manager and creates the DMA message queues. */
void rom_init(void) {
    s32 i;

    osCreatePiManager(OS_PRIORITY_PIMGR, &gPiCmdQueue, &gPiCmdBuffer, 200);

    for (i = 0; i < 3; i++) {
        osCreateMesgQueue(&gMessageQueue[i], &gMessages[i], 1);
    }

    osCreateMesgQueue(&gMessageQueue0, &gMessage0, 300);
    gRomReadNext = 0;
    gRomReadsInFlight = 0;
}

/*
 * Reads size bytes of cartridge ROM at devAddr into dramAddr. A blocking read waits on the calling
 * thread's queue; a non-blocking one takes the next slot of the 300-entry gRomReadRing ring
 * (gRomReadNext = next slot, gRomReadsInFlight = reads in flight) and is collected by rom_waitReads.
 * Both counters are u8, so the 299/300 limits below can never actually be reached.
 */
void rom_read(s32 devAddr, void *dramAddr, u32 size, s32 blocking) {
    OSMesgQueue *queue;
    OSIoMesg localIoMsg;
    OSIoMesg *ioMsg;
    s32 threadIndex;

    threadIndex = __osRunningThread->id - 3;
    if (size < 200 && threadIndex == 0) {
        rom_copyUncached(devAddr, dramAddr, size);
        return;
    }
    if (threadIndex >= 4 || threadIndex < 0) {
        threadIndex = 0;
    }
    if (!blocking) {
        if (gRomReadsInFlight == 300) {
            return;
        }
        ioMsg = &gRomReadRing[gRomReadNext];
        queue = &gMessageQueue0;
        if (gRomReadNext == 299) {
            gRomReadNext = 0;
        } else {
            gRomReadNext += 1; // matching: ++ changes how size is spilled
        }
        gRomReadsInFlight += 1;
    } else {
        ioMsg = &localIoMsg;
        queue = &gMessageQueue[threadIndex];
    }
    osInvalDCache(dramAddr, size);
    osPiStartDma(ioMsg, OS_MESG_PRI_NORMAL, OS_READ, devAddr, dramAddr, size, queue);

    if (blocking) {
        osRecvMesg(queue, NULL, OS_MESG_BLOCK);
    }
}

/* Waits for every non-blocking read started by rom_read. */
void rom_waitReads(void) {
    s32 i;

    for (i = 0; i < gRomReadsInFlight; i++) {
        osRecvMesg(&gMessageQueue0, NULL, OS_MESG_BLOCK);
    }

    gRomReadsInFlight = 0;
}

/* Blocking ROM read of any size, split into DMAs of at most 0x14000 bytes on the thread's queue. */
void rom_readLarge(s32 devAddr, void *dramAddr, u32 size) {
    OSIoMesg ioMsg;
    OSMesg recvMsg;
    s32 threadIndex;
    u32 sent;
    u32 chunkSize;

    sent = 0;
    threadIndex = __osRunningThread->id - 3;
    if (threadIndex >= 4 || threadIndex < 0) {
        threadIndex = 0;
    }

    osInvalDCache(dramAddr, size);
    while (sent < size) {
        chunkSize = MIN(size - sent, 0x14000);
        osPiStartDma(&ioMsg, OS_MESG_PRI_NORMAL, OS_READ, devAddr, dramAddr, chunkSize, &gMessageQueue[threadIndex]);
        osRecvMesg(&gMessageQueue[threadIndex], &recvMsg, OS_MESG_BLOCK);
        sent += chunkSize;
        devAddr += chunkSize;
        dramAddr = (u8 *)dramAddr + chunkSize;
    }
}

typedef union {
    s32 word;
    u16 half[2];
} RomWord;

/*
 * Copies size bytes (rounded up to even) from cartridge ROM offset romAddr to dest with uncached
 * CPU reads instead of a DMA, once the PI is idle. A ROM address that is not word aligned has its
 * first halfword taken from the low half of the preceding word.
 */
void rom_copyUncached(u32 romAddr, void *dest, u32 size) {
    u32 count;
    RomWord data;
    u32 status;
    u32 offset;
    u32 readAddr;
    u8 *writeAddr;

    gRomCopyActive = 1;
    size = (size + 1) & ~1;
    while (gPiDmaReadBusy) {
    }
    // matching: the status local is needed (IDO -g3 frame size)
    while ((status = IO_READ(PI_STATUS_REG)) & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY)) {
    }
    romAddr |= D_80000308; // osRomBase
    if (romAddr & 2) {
        size -= 2;
        readAddr = romAddr; // matching: reading through romAddr directly changes regalloc
        data.word = IO_READ(readAddr - 2);
        count = size - 2;
        offset = 0;
        *(u16 *)dest = data.half[1];
        if (count != 0) {
            readAddr = romAddr + 2;
            writeAddr = *(u8 **)&dest; // matching: forces dest to be reloaded from its stack home
            do {
                data.word = IO_READ(readAddr + offset);
                offset += 4;
                *(u16 *)(writeAddr + 2) = data.word >> 16;
                writeAddr += 4;
                *(u16 *)writeAddr = data.word & 0xFFFF; // matching: mask makes IDO reload data.word
            } while (offset < count);
        }
        if (size & 2) {
            data.word = IO_READ(romAddr + offset + 2);
            *(u16 *)((u8 *)dest + offset + 2) = data.half[0];
        }
    } else {
        count = size - 2;
        offset = 0;
        // matching: guarded do-while with cursors; for-loop forms change regalloc
        if (count != 0) {
            readAddr = romAddr;
            writeAddr = dest;
            do {
                *(u32 *)writeAddr = IO_READ(readAddr);
                offset += 4;
                readAddr += 4;
                writeAddr += 4;
            } while (offset < count);
        }
        if (size & 2) {
            data.word = IO_READ(offset + romAddr);
            *(u16 *)((u8 *)dest + offset) = data.half[0];
        }
    }
    gRomCopyActive = 0;
    if (gPiThreadStopped) {
        osStartThread(&D_80035910);
    }
}
