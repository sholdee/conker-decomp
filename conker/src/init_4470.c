#include <ultra64.h>

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
            D_8003A570 += 1;
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
} TransferWord;

void func_1000480C(u32 source, void *destination, u32 size) {
    u32 count;
    TransferWord data;
    u32 status;
    u32 offset;
    u32 readAddress;
    u8 *writeAddress;

    *(volatile u8 *)&D_8003A572 = 1;
    size = (size + 1) & ~1U;
    while (*(volatile u8 *)&D_8003A573 != 0) {
    }
    while ((status = *(volatile u32 *)0xA4600010) & 3) {
    }
    source |= D_80000308;
    if (source & 2) {
        size -= 2;
        readAddress = source;
        data.word = *(volatile u32 *)((readAddress - 2) | 0xA0000000);
        count = size - 2;
        offset = 0;
        *(u16 *)((void **)&destination)[0] = data.half[1];
        if (count != 0) {
            readAddress = source + 2;
            writeAddress = ((void **)&destination)[0];
            do {
                data.word = *(volatile u32 *)((readAddress + offset) | 0xA0000000);
                offset += 4;
                *(s16 *)(writeAddress + 2) = data.word >> 16;
                writeAddress += 4;
                *(u16 *)writeAddress = data.word & 0xFFFF;
            } while (offset < count);
        }
        if (size & 2) {
            data.word = *(volatile u32 *)((source + offset + 2) | 0xA0000000);
            *(u16 *)((u8 *)((void **)&destination)[0] + offset + 2) = data.half[0];
        }
    } else {
        count = size - 2;
        offset = 0;
        if (count != 0) {
            readAddress = source;
            writeAddress = ((void **)&destination)[0];
            do {
                *(u32 *)writeAddress = *(volatile u32 *)(readAddress | 0xA0000000);
                offset += 4;
                readAddress += 4;
                writeAddress += 4;
            } while (offset < count);
        }
        if (size & 2) {
            data.word = *(volatile u32 *)((offset + source) | 0xA0000000);
            *(u16 *)((u8 *)((void **)&destination)[0] + offset) = data.half[0];
        }
    }
    *(volatile u8 *)&D_8003A572 = 0;
    if (*(volatile u8 *)&D_8003A575 != 0) {
        osStartThread((OSThread *)&D_80035910);
    }
}
// void func_1000480C(s32 devAddr, void *dramAddr, u32 size) {
//     s32 sp38;
//     s32 *temp_t0;
//     s32 *temp_t0_2;
//     s32 temp_a1;
//     s32 temp_s0;
//     s32 temp_s1;
//     s32 temp_t4;
//     s32 temp_t7;
//     u32 temp_a0;
//     u32 temp_a0_2;
//     u32 temp_a2;
//     u32 temp_a2_2;
//     void *temp_v0;
//     void *temp_v0_2;
//     s32 phi_a1;
//     void *phi_v0;
//     u32 phi_a0;
//     u32 phi_a0_2;
//     s32 phi_v1;
//     void *phi_v0_2;
//     u32 phi_a0_3;
//     u32 phi_a0_4;
//     void *phi_a1_2;
//     u32 phi_a2;
//
//     D_8003A572 = 1;
//     size = (size + 1) & ~1;
//     do {} while (D_8003A573 != 0);
//
//     temp_a2 = size - 2;
//
//     while ((IO_READ(PI_STATUS_REG) & (PI_STATUS_DMA_BUSY|PI_STATUS_IO_BUSY)) != 0) {};
//
//     temp_t0 = &sp38;
//     devAddr = devAddr | D_80000308;
//     if ((devAddr & 2) != 0) {
//         temp_s0 = size - 2;
//         sp38 = *((s32*)((devAddr - 2) | 0xA0000000));
// //         temp_a2_2 = temp_s0 - 2;
// //         *dramAddr = (u16) temp_t0->unk2;
// //         phi_a0_2 = 0U;
// //         phi_a1_2 = dramAddr;
// //         // phi_return = (void *) *(void *)0xA4600010;
// //         if (temp_a2_2 != 0) {
// //             phi_a1 = devAddr + 2;
// //             phi_v0 = dramAddr;
// //             phi_a0 = 0U;
// // loop_7:
// //             temp_t4 = *(phi_a1 | 0xA0000000);
// //             temp_a0 = phi_a0 + 4;
// //             sp38 = temp_t4;
// //             phi_v0->unk2 = (s16) (temp_t4 >> 0x10);
// //             temp_a1 = phi_a1 + 4;
// //             temp_v0 = phi_v0 + 4;
// //             *temp_v0 = (s16) sp38;
// //             phi_a1 = temp_a1;
// //             phi_v0 = temp_v0;
// //             phi_a0 = temp_a0;
// //             phi_a0_2 = temp_a0;
// //             phi_a1_2 = (void *) temp_a1;
// //             if (temp_a0 < temp_a2_2) {
// //                 goto loop_7;
// //             }
// //         }
// //         phi_a2 = temp_a2_2;
// //         if ((temp_s0 & 2) != 0) {
// //             sp38 = *((devAddr + phi_a0_2 + 2) | 0xA0000000);
// //             (dramAddr + phi_a0_2)->unk2 = (u16) temp_t0->unk0;
// //             phi_a2 = temp_a2_2;
// //         }
//     } else {
// //         phi_a0_4 = 0U;
// //         if (temp_a2 != 0) {
// //             phi_v1 = devAddr;
// //             phi_v0_2 = dramAddr;
// //             phi_a0_3 = 0U;
// // loop_12:
// //             temp_a0_2 = phi_a0_3 + 4;
// //             temp_v0_2 = phi_v0_2 + 4;
// //             temp_v0_2->unk-4 = (s32) *(phi_v1 | 0xA0000000);
// //             phi_v1 = phi_v1 + 4;
// //             phi_v0_2 = temp_v0_2;
// //             phi_a0_3 = temp_a0_2;
// //             phi_a0_4 = temp_a0_2;
// //             if (temp_a0_2 < temp_a2) {
// //                 goto loop_12;
// //             }
// //         }
// //         phi_a1_2 = dramAddr;
// //         phi_a2 = temp_a2;
// //         if ((size & 2) != 0) {
// //             temp_t0_2 = &sp38;
// //             sp38 = *((phi_a0_4 + devAddr) | 0xA0000000);
// //             *(dramAddr + phi_a0_4) = (u16) temp_t0_2->unk0;
// //             phi_a1_2 = dramAddr;
// //             phi_a2 = temp_a2;
// //         }
//     }
//     D_8003A572 = (u8)0;
//     if (D_8003A575 != 0) {
//         osStartThread(&D_80035910); //, phi_a1_2, phi_a2, 0xA0000000);
//     }
// }
