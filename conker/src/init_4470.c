#include <ultra64.h>

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order it first
 * sees each name. */
static OSMesg D_800380E0[200];   // the PI manager's command queue buffer
static OSMesg gMessage0[300];
static OSMesgQueue D_800388B0;   // the PI manager's command queue
static OSMesgQueue gMessageQueue0; // completions of the non-blocking reads
OSIoMesg D_800388E0;
static OSMesg gMessages[3];
OSMesgQueue gMessageQueue[3];    // completions of the blocking reads, one per game thread
static OSIoMesg D_80038950[300]; // the ring of non-blocking reads
static u8 D_8003A570;            // next slot of D_80038950
static u8 D_8003A571;            // non-blocking reads in flight
vu8 D_8003A572; // PI interlock flags shared with the PI manager thread (init_2E50)
vu8 D_8003A573;
u8 D_8003A574; // not referenced
vu8 D_8003A575;

#include "functions.h"
#include "variables.h"


/* Starts the PI manager and creates the DMA message queues. */
void func_10004470(void) {
    s32 i;

    osCreatePiManager(OS_PRIORITY_PIMGR, &D_800388B0, &D_800380E0, 200);

    for (i = 0; i < 3; i++) {
        osCreateMesgQueue(&gMessageQueue[i], &gMessages[i], 1);
    }

    osCreateMesgQueue(&gMessageQueue0, &gMessage0, 300);
    D_8003A570 = 0;
    D_8003A571 = 0;
}

/*
 * Reads size bytes of cartridge ROM at devAddr into dramAddr. A blocking read waits on the calling
 * thread's queue; a non-blocking one takes the next slot of the 300-entry D_80038950 ring
 * (D_8003A570 = next slot, D_8003A571 = reads in flight) and is collected by func_10004674.
 * Both counters are u8, so the 299/300 limits below can never actually be reached.
 */
void func_10004514(s32 devAddr, void *dramAddr, u32 size, s32 blocking) {
    OSMesgQueue *queue;
    OSIoMesg localIoMsg;
    OSIoMesg *ioMsg;
    s32 threadIndex;

    threadIndex = __osRunningThread->id - 3;
    if (size < 200 && threadIndex == 0) {
        func_1000480C(devAddr, dramAddr, size);
        return;
    }
    if (threadIndex >= 4 || threadIndex < 0) {
        threadIndex = 0;
    }
    if (!blocking) {
        if (D_8003A571 == 300) {
            return;
        }
        ioMsg = &D_80038950[D_8003A570];
        queue = &gMessageQueue0;
        if (D_8003A570 == 299) {
            D_8003A570 = 0;
        } else {
            D_8003A570 += 1; // matching: ++ changes how size is spilled
        }
        D_8003A571 += 1;
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

/* Waits for every non-blocking read started by func_10004514. */
void func_10004674(void) {
    s32 i;

    for (i = 0; i < D_8003A571; i++) {
        osRecvMesg(&gMessageQueue0, NULL, OS_MESG_BLOCK);
    }

    D_8003A571 = 0;
}

/* Blocking ROM read of any size, split into DMAs of at most 0x14000 bytes on the thread's queue. */
void func_100046E4(s32 devAddr, void *dramAddr, u32 size) {
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
void func_1000480C(u32 romAddr, void *dest, u32 size) {
    u32 count;
    RomWord data;
    u32 status;
    u32 offset;
    u32 readAddr;
    u8 *writeAddr;

    D_8003A572 = 1;
    size = (size + 1) & ~1;
    while (D_8003A573) {
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
    D_8003A572 = 0;
    if (D_8003A575) {
        osStartThread(&D_80035910);
    }
}
