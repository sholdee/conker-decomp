#include <ultra64.h>

#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/init_8F90/func_10008F90.s")
// NON-MATCHING: so much to do
// void func_10008F90(struct15 *arg0, OSPri arg1, struct52 *arg2) {
//     // ? sp160;
//     s32 sp58;
//     // ? *temp_t4_2;
//     f32 temp_f0;
//     s32 temp_f8;
//     s32 temp_t4;
//     s32 temp_t7;
//     s32 temp_v0;
//     // u32 temp_a1;
//     // u32 temp_s0_2;
//     // void *temp_a0;
//     // void *temp_s0;
//     // void *temp_s0_3;
//     // void *temp_s1;
//     // void *temp_t0;
//     // void *temp_v0_2;
//     // void *temp_v0_3;
//     // void *temp_v0_4;
//     // void *temp_v1;
//     u32 phi_v1;
//     void *phi_t0;
//     s32 *phi_t4;
//     // void *phi_s0;
//     // void *phi_s1;
//     // void *phi_v0;
//     // s32 phi_v1_2;
//     // u32 phi_a1;
//     // u32 phi_s0_2;
//     // void *phi_s0_3;
//
//     func_10012588(&D_8003E370);
//     arg0->unk10 = func_10009980;
//     temp_v0 = osAiSetFrequency(arg2->unk0);
//     arg0->unk2C = temp_v0;
//     arg0->unk14 = func_10009FFC;
//     arg0->unk18 = func_10009B2C;
//     arg0->unk1C = func_10009B90;
//     arg0->unk20 = func_10009B4C;
//     temp_f0 = ((f32) (u32) arg2->unk4 * (f32) temp_v0) / 30.0f;
//     temp_f8 = (s32) temp_f0;
//     D_80040F8C = temp_f8;
//     phi_v1 = temp_f8;
//     if ((f32) (u32) temp_f8 < temp_f0) {
//         temp_t4 = temp_f8 + 1;
//         D_80040F8C = temp_t4;
//         phi_v1 = (u32) temp_t4;
//     }
//     temp_t7 = ((phi_v1 / 184) * 184) + 184;
//     D_80040F8C = temp_t7;
//     D_80040F88 = temp_t7 - 184;
//     D_80040F90 = temp_t7 + 84;
//     D_80040F84 = (u8)0;
//     phi_t0 = D_8002AE54;
//     phi_t4 = &sp58;
// // loop_3:
// //     temp_t0 = phi_t0 + 0xC;
// //     temp_t4_2 = phi_t4 + 0xC;
// //     temp_t4_2->unk-C = (s32) *phi_t0;
// //     temp_t4_2->unk-8 = (s32) temp_t0->unk-8;
// //     temp_t4_2->unk-4 = (s32) temp_t0->unk-4;
// //     phi_t0 = temp_t0;
// //     phi_t4 = temp_t4_2;
// //     if (temp_t0 != 0x8002B064) {
// //         goto loop_3;
// //     }
// //     arg0->unk34 = &sp58;
// //     arg0->unk38 = &sp160;
// //     n_alInit(0x8003E640, arg0, (void *)0x80040F8C, &sp58);
// //     *(void *)0x8003E380 = 0x8003E640;
// //     D_800406B8.unk4 = 0;
// //     D_800406B8.unk0 = 0;
// //     phi_s0 = &D_800406B8;
// //     phi_s1 = (void *)0x800406CC;
// // loop_5:
// //     temp_v0_2 = phi_s0->unk0;
// //     phi_s0->unk18 = phi_s0;
// //     phi_s0->unk14 = temp_v0_2;
// //     if (temp_v0_2 != 0) {
// //         temp_v0_2->unk4 = phi_s1;
// //     }
// //     phi_s0->unk0 = phi_s1;
// //     temp_s1 = phi_s1 + 0x14;
// //     temp_s0 = phi_s0 + 0x14;
// //     temp_s0->unk-4 = alHeapDBAlloc(0, 0, arg0->unk28, 1, 2048);
// //     phi_s0 = temp_s0;
// //     phi_s1 = temp_s1;
// //     if ((u32) temp_s1 < (u32) &D_80040AC8) {
// //         goto loop_5;
// //     }
// //     temp_s0->unk10 = alHeapDBAlloc(0, 0, arg0->unk28, 1, 2048);
// //     bzero(&D_80040AC8, 1200);
// //     D_80040AC8.unk4 = 0;
// //     D_80040AC8.unk0 = 0;
// //     phi_v0 = &D_80040AC8;
// //     phi_v1_2 = 0x80040AE0;
// //     phi_a1 = 0U;
// // loop_9:
// //     temp_a0 = phi_v0->unk0;
// //     phi_v0->unk1C = phi_v0;
// //     phi_v0->unk18 = temp_a0;
// //     if (temp_a0 != 0) {
// //         temp_a0->unk4 = phi_v1_2;
// //     }
// //     phi_v0->unk0 = phi_v1_2;
// //     temp_a1 = phi_a1 + 1;
// //     temp_v0_3 = phi_v0 + 0x18;
// //     temp_v0_3->unk-8 = 0;
// //     phi_v0 = temp_v0_3;
// //     phi_v1_2 = phi_v1_2 + 0x18;
// //     phi_a1 = temp_a1;
// //     if (temp_a1 < 0x31U) {
// //         goto loop_9;
// //     }
// //     temp_v0_3->unk10 = 0;
// //     phi_s0_2 = 0x8003E388U;
// // loop_13:
// //     temp_s0_2 = phi_s0_2 + 4;
// //     temp_s0_2->unk-4 = alHeapDBAlloc(0, 0, arg0->unk28, 1, arg2->unk8 * 8);
// //     phi_s0_2 = temp_s0_2;
// //     if (temp_s0_2 < 0x8003E390U) {
// //         goto loop_13;
// //     }
// //     *(void *)0x80040F94 = (s32) arg2->unk8;
// //     phi_s0_3 = (void *)0x8003E388;
// // loop_15:
// //     temp_v0_4 = alHeapDBAlloc(0, 0, arg0->unk28, 1, 0x90);
// //     phi_s0_3->unk8 = temp_v0_4;
// //     temp_v0_4->unk70 = (u16)2;
// //     temp_v1 = phi_s0_3->unk8;
// //     temp_v1->unk74 = temp_v1;
// //     temp_s0_3 = phi_s0_3 + 4;
// //     phi_s0_3->unk8->unk0 = alHeapDBAlloc(0, 0, arg0->unk28, 1, *(void *)0x80040F90 * 4);
// //     phi_s0_3 = temp_s0_3;
// //     if (temp_s0_3 != 0x8003E394) {
// //         goto loop_15;
// //     }
//
//     osCreateMesgQueue(&D_8003E608, &D_8003E620, 8);
//     osCreateMesgQueue(&D_8003E5D0, &D_8003E5E8, 8);
//     osCreateMesgQueue(&D_80041298, &D_800412B0, 32);
//     osCreateMesgQueue(&D_800416F0, &D_80041708, 40);
//     osCreateThread(&D_8003E3A0, 4, (void *) func_10009400, 0, &D_800406A0, arg1);
//     D_8002AE40 = (u8)1;
//     osStartThread(&D_8003E3A0);
// }

void func_100093CC(void) {
    if (D_8002AE40 != 0) {
        osStopThread(&D_8003E3A0);
    }
}

// audio thread
#pragma GLOBAL_ASM("asm/nonmatchings/init_8F90/func_10009400.s")
// NON-MATCHING: a long way to go
// void func_10009400(s32 arg0) {
//     OSMesg *msg;
//     s32 sp54;
//     OSPfs *sp4C;
//     s16 temp_v0;
//     u32 phi_s0;
//     u32 phi_s0_2;
//     s32 phi_s5;
//     s32 phi_s4;
//     s32 phi_s1;
//
//     msg = NULL;
//     sp54 = 0;
//     phi_s0 = 0;
//     phi_s5 = 0;
//     phi_s4 = 1;
//     phi_s1 = 0;
//     func_100051C8(&sp4C, &D_8003E5D0);
//     do {
//         osRecvMesg(&D_8003E5D0, &msg, 1);
//         if (D_8002AC5C != 0) {
//             ((struct53*)msg)->unk0 = (u16)4;
//         }
//         temp_v0 = ((struct53*)msg)->unk0;
//         if (temp_v0 != 1) {
//             if (temp_v0 == (u16)4 || temp_v0 == 16) {
//                     phi_s1 = 1;
//             }
//         } else {
//             phi_s0_2 = phi_s0;
//             if (phi_s0 >= 2U) {
//                 phi_s0_2 = 0U;
//             }
//             if (phi_s0_2 == 0 && (func_100095A0(D_8003E390[D_8002AE44 % 3U], phi_s5) != 0)) {
//                 if (phi_s4 == 0) {
//                     osRecvMesg(&D_8003E608, &msg, 1);
//                     phi_s5 = ((struct53*)msg)->unk4;
//                 }
//                 phi_s4 = 0;
//             }
//             phi_s0 = phi_s0_2 + 1;
//         }
//     }
//     while (phi_s1 == 0);
//
//     func_10018E0C(&D_8003E640);
//     while (1) {
//         osRecvMesg(&D_8003E5D0, &msg, 1);
//     }
// }

#pragma GLOBAL_ASM("asm/nonmatchings/init_8F90/func_100095A0.s")
// NON-MATCHING: so far away
// s32 func_100095A0(struct50 *arg0, struct51 *arg1) {
//     s32 sp3C;
//     s32 sp34;
//     u32 sp30;
//     s32 temp_a2;
//     s32 temp_v1;
//     u32 temp_t7;
//     u8 temp_v0;
//     s32 phi_v1;
//     s32 phi_a2;
//
//     sp3C = osVirtualToPhysical(arg0->unk0);
//     func_100099BC();
//     func_1000A03C();
//     temp_a2 = sp3C;
//     temp_t7 = (u32) AI_A4500004 >> 2;
//     phi_v1 = (s32) temp_t7;
//     if (arg1 != 0) {
//         sp3C = temp_a2;
//         sp30 = temp_t7;
//         func_10002DB0(arg1->unk4, arg1->unk8 * 4, temp_a2);
//         phi_v1 =  sp30;
//     }
//     if ((phi_v1 >= 0xF9) && (D_80040F84 == 0)) {
//         arg0->unk8 = D_80040F88; // *
//         D_80040F84 = 2U;
//     } else {
//         arg0->unk8 = D_80040F8C; // *
//         temp_v0 = D_80040F84;
//         if (temp_v0 != 0) {
//             D_80040F84 = (u8) (temp_v0 - 1);
//         }
//     }
//     if (((temp_a2 + (arg0->unk8 * 4)) & 0x1FFF) == 0) {
//         arg0->unk4 = (s32) (arg0->unk0 + 16);
//         phi_a2 = temp_a2 + 16;
//     } else {
//         arg0->unk4 = (s32) arg0->unk0;
//         phi_a2 = temp_a2;
//     }
//     // temp_v1 = alAudioFrame(D_8003E388[D_8002AE4C], &sp34, phi_a2, arg0->unk8);
//     if (sp34 == 0) {
//         return 0;
//     }
//     arg0->unk10 = 0;
//     arg0->unk68 = 0x8003E608;
//     arg0->unk6C = arg0->unk70;
//     arg0->unk1C = 2;
//     arg0->unk20 = 0;
//     arg0->unk58 = (s32) D_8003E388[D_8002AE4C];
//     arg0->unk5C = (s32) (((s32) D_8003E388[temp_v1 - D_8002AE4C] >> 3) * 8);
//     arg0->unk28 = 2;
//     arg0->unk30 = 0x100290D0;
//     arg0->unk34 = (s32) (D_100291A0 - D_100290D0);
//     arg0->unk2C = 0;
//     arg0->unk38 = 0x100291A0;
//     arg0->unk40 = 0x8002C960;
//     arg0->unk44 = 2048;
//     arg0->unk60 = 0;
//     arg0->unk64 = 1024;
//     // D_8002AE4C, 0x8003E388, 2
//     osWritebackDCacheAll();
//     osSendMesg(&D_8003B200, &arg0->unk10, 1);
//     D_8002AE4C = D_8002AE4C ^ 1;
//     return 1;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/init_8F90/func_100097CC.s")
// NON-MATCHING: another horrible function
// s32 func_100097CC(u32 arg0, s32 arg1, s32 arg2) {
//     s32 sp2C;
//     s32 sp28;
//     s32 temp_a3;
//     s32 temp_t0;
//     s32 temp_t1;
//     s32 temp_t9;
//     u32 temp_v0;
//     u32 temp_v0_6;
//     struct19 *temp_a2;
//     struct19 *temp_v0_2;
//     struct19 *temp_v0_3;
//     struct19 *temp_v0_4;
//     struct19 *temp_v0_5;
//     struct19 *temp_v1;
//     struct19 *temp_v1_2;
//     struct19 *temp_v1_3;
//     struct19 *phi_v1;
//     struct19 *phi_a2;
//
//     temp_v1 = D_80040F78.unk4;
//     phi_v1 = temp_v1;
//     phi_a2 = NULL;
//     phi_a2 = NULL;
//     if (temp_v1 != 0) {
// loop_1:
//         temp_v0 = phi_v1->unk8;
//         if (arg0 >= temp_v0) {
//             temp_a2 = phi_v1;
//             if ((temp_v0 + 0x800) >= (arg0 + arg1)) {
//                 phi_v1->unkC = (s32) D_8002AE44;
//                 return osVirtualToPhysical((phi_v1->unk10 + arg0) - temp_v0); //, temp_a2, arg0);
//             }
//             temp_v1_2 = phi_v1->unk0;
//             phi_v1 = temp_v1_2;
//             phi_a2 = temp_a2;
//             // phi_a2 = temp_a2;
//             if (temp_v1_2 != 0) {
//                 goto loop_1;
//             }
//         }
//     }
//     temp_v1_3 = D_80040F78.unk8;
//     if ((temp_v1_3 == 0) || ((u32) D_8002AE48 >= 0x20U)) {
//         return 0;
//     }
//     D_80040F78.unk8 = (struct19 *) temp_v1_3->unk0;
//     temp_v0_2 = temp_v1_3->unk0;
//     if (temp_v0_2 != 0) {
//         temp_v0_2->unk4 = (struct19 *) temp_v1_3->unk4;
//     }
//     temp_v0_3 = temp_v1_3->unk4;
//     if (temp_v0_3 != 0) {
//         temp_v0_3 = (struct19 *) temp_v1_3->unk0; // ()
//     }
//     if (phi_a2 != 0) {
//         temp_v1_3->unk4 = phi_a2;
//         temp_v1_3->unk0 = (struct19 *) phi_a2; // *
//         temp_v0_4 = phi_a2; // *
//         if (temp_v0_4 != 0) {
//             temp_v0_4->unk4 = temp_v1_3;
//         }
//         phi_a2 = temp_v1_3; // *
//     } else {
//         temp_v0_5 = D_80040F78.unk4;
//         if (temp_v0_5 != 0) {
//             D_80040F78.unk4 = temp_v1_3;
//             temp_v1_3->unk0 = temp_v0_5;
//             temp_v1_3->unk4 = NULL;
//             temp_v0_5->unk4 = temp_v1_3;
//         } else {
//             D_80040F78.unk4 = temp_v1_3;
//             temp_v1_3->unk0 = NULL;
//             temp_v1_3->unk4 = NULL;
//         }
//     }
//     temp_t0 = arg0 & 1;
//     temp_t1 = temp_v1_3->unk10;
//     sp28 = temp_t0;
//     temp_a3 = arg0 - temp_t0;
//     temp_v1_3->unk8 = temp_a3;
//     temp_v1_3->unkC = (s32) D_8002AE44;
//     temp_v0_6 = D_8002AE48;
//     // temp_t9 = temp_v0_6; // * 0x18;
//     D_8002AE48 = (u32) (temp_v0_6 + 1); // next free slot in D_80040F98?
//     sp2C = temp_t1;
//     osPiStartDma(&D_80040F98[temp_v0_6], 1, 0, temp_a3, temp_t1, 0x800, &D_80041298);
//     return osVirtualToPhysical(sp2C) + sp28;
// }

s32 func_10009980(s32 *arg0) {
    if (D_80040F78.unk0 == 0) {
        D_80040F78.unk4 = (u8) 0;
        D_80040F78.unk8 = &D_800406B8;
        D_80040F78.unk0 = 1;
    }
    *arg0 = 0;
    return func_100097CC;
}

#pragma GLOBAL_ASM("asm/nonmatchings/init_8F90/func_100099BC.s")

void func_10009B2C(struct54 *arg0) {
    if (((s32)arg0 & 1) == 0) {
        arg0->unk14--;
    }
}

void func_10009B4C(struct54 *arg0) { // struct147 unk14 is wrong type
    if (((s32)arg0 & 1) == 0) {
        arg0->unk14--;
        if (arg0->unk14 == 0) {
            func_10009BE4(arg0);
        }
    }
}

void func_10009B90(struct54 *arg0) {
    if (((s32)arg0 & 1) == 0) {
        if (1 == arg0->unk15) {
            if (1 == arg0->unk16) {
                arg0->unk14 += 1;
            }
            arg0->unk15 = 2;
            return;
        }
        arg0->unk14 += 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/init_8F90/func_10009BE4.s")
// void func_10009BE4(struct00 *arg0) {
//     struct00 *temp_a1;
//     struct00 *temp_v0;
//     struct00 *temp_v1;
//
//     if (((s32)arg0 & 1) != 0) {
//         D_8003C8E0 = 0x0F000004;
//         func_150AD770(); // 0x80040000
//         return;
//     }
//     arg0->unkC = (s32) arg0->unk8;
//     if ((s32)arg0 == D_800406A0.unk4) {
//         D_800406A4 = (struct54 *) arg0->unk0;
//     }
//     temp_v0 = arg0->unk0;
//     if (temp_v0 != 0) {
//         temp_v0->unk4 = (struct54 *) arg0->unk4;
//     }
//     temp_v0 = arg0->unk4;
//     if (temp_v0 != 0) {
//         temp_v0 = (struct54 *) arg0->unk0;
//     }
//     temp_v1 = D_800406A0.unk10;
//     if (temp_v1 != 0) {
//         arg0->unk4 = temp_v1;
//         arg0->unk0 = (struct54 *) &temp_v1;
//         temp_a1 = &temp_v1;
//         if (temp_a1 != 0) {
//             temp_a1->unk4 = arg0;
//         }
//         temp_v1 = arg0;
//         return;
//     }
//     D_800406B0 = arg0;
//     arg0->unk0 = NULL;
//     arg0->unk4 = NULL;
//     // return temp_v0;
// }

typedef struct AudioBufferState {
    struct AudioBufferState *next;
    struct AudioBufferState *prev;
    s32 savedValue;
    s32 *ownerSlot;
    void *buffer;
    s8 count;
    u8 state;
    u8 field16;
} AudioBufferState;

typedef struct {
    u8 initialized;
    u8 pad1[3];
    AudioBufferState *active;
    void *base;
    AudioBufferState *pending;
    AudioBufferState *freeAnchor;
} AudioBankManager;

#define D_800406A0_MGR (*(AudioBankManager *)&D_800406A0)

extern void *allocate_memory(s32, s32, s32, s32);
extern u32 D_8002AE50;
extern OSIoMesg D_80041330[];

s32 func_10009CBC(void *arg0, s32 mode) {
    AudioBufferState *record;
    AudioBufferState *reuse;
    s32 size;
    s32 alignedSize;
    AudioBufferState *anchor;
    u32 value;
    s32 *ownerSlot;

    reuse = 0;
    value = *(u32 *)arg0;
    if (value & 1) {
        if (mode == 0) {
            size = (value & 0xFF) + 1;
        } else {
            size = (value & 0xFF & ~1) << 6;
        }
        record = D_800406A0_MGR.base;
        if (record == 0) {
            record = D_800406A0_MGR.active;
            while (record != 0) {
                if (record->count == 0 && record->field16 == 0 && record->state == 2) {
                    reuse = record;
                }
                record = record->next;
            }
            record = reuse;
            if (reuse != 0) {
                ownerSlot = reuse->ownerSlot;
                *ownerSlot = reuse->savedValue;
                func_100043B4(reuse->buffer, 4);
                reuse->buffer = 0;
                reuse->ownerSlot = 0;
                if (reuse == D_800406A0_MGR.active) {
                    D_800406A4 = (struct54 *)reuse->next;
                }
                {
                    AudioBufferState *element = reuse;

                    if (element->next != 0) {
                        element->next->prev = element->prev;
                    }
                    if (element->prev != 0) {
                        element->prev->next = element->next;
                    }
                }
            }
        } else {
            D_800406A0_MGR.base = record->next;
            {
                AudioBufferState *element = record;

                if (element->next != 0) {
                    element->next->prev = element->prev;
                }
                if (element->prev != 0) {
                    element->prev->next = element->next;
                }
            }
        }
        if (record != 0) {
            record->next = 0;
            record->prev = 0;
            if (D_800406A0_MGR.pending != 0) {
                AudioBufferState *linkNode = record;
                AudioBufferState *linkAfter = D_800406A0_MGR.pending;

                linkNode->next = linkAfter->next;
                linkNode->prev = linkAfter;
                if (linkAfter->next != 0) {
                    linkAfter->next->prev = linkNode;
                }
                linkAfter->next = linkNode;
            } else {
                D_800406A0_MGR.pending = record;
                record->next = 0;
                record->prev = 0;
            }
            record->savedValue = value;
            record->count = 0;
            record->field16 = mode;
            record->ownerSlot = arg0;
            record->state = 0;
            if (D_8002AE50 < 0x28U) {
                alignedSize = (size + 0xF) & ~0xF;
                record->buffer = (void *)allocate_memory(alignedSize, 0xFF, 2, 0);
                bzero(record->buffer, alignedSize);
            } else {
                record->buffer = 0;
            }
            if (record->buffer != 0) {
                osWritebackDCache(record->buffer, alignedSize);
                osInvalDCache(record->buffer, alignedSize);
                osPiStartDma(&D_80041330[D_8002AE50++], 1, 0,
                             (value >> 5) & ~7U, record->buffer, alignedSize,
                             (OSMesgQueue *)&D_800416F0);
                *(u32 *)arg0 = (u32)record;
            }
        }
        return 0;
    }
    *(u32 *)arg0 = value;
    record = (AudioBufferState *)value;
    if (record->state != 0) {
        if (mode == 1) {
            if (record->state == 1) {
                record->state = 2;
            } else {
                record->count++;
            }
        }
        return (s32)record->buffer;
    }
    return 0;
}

s32 func_10009FFC(void) {
    if (D_800406A0.unk0 == 0) {
        D_800406A0.unk4 = NULL;
        D_800406A0.unk8 = &D_80040AC8;
        D_800406A0.unkC = 0;
        D_800406A0.unk10 = NULL;
        D_800406A0.unk0 = (u8)1U;
    }
    return func_10009CBC;
}

typedef struct {
    u16 type;
    u8 priority;
    u8 status;
    OSMesgQueue *returnQueue;
    void *dramAddress;
    u32 deviceAddress;
    u32 size;
    void *piHandle;
} TransferIoMessage;

typedef struct {
    u8 pad0[0xA];
    u8 fieldA;
} AudioWaveState;

typedef struct {
    u8 pad0[8];
    AudioWaveState *wave;
} AudioBankSound;

typedef struct {
    u8 pad0[0xE];
    s16 soundCount;
    AudioBankSound *sounds[1];
} AudioInstrument;

void func_10004074(s32);
void func_1000A348(void);
extern s32 D_8003E384;

void func_1000A03C(void) {
    u32 i;
    u32 sound;
    s32 found;
    s32 received;
    void *message;
    AudioBufferState *record;
    AudioBufferState *next;
    AudioInstrument *instrument;
    AudioWaveState *wave;
    s32 busy;

    received = 0;
    message = 0;
    for (i = 0; i < D_8002AE50; i++) {
        if (osRecvMesg((OSMesgQueue *)&D_800416F0, &message, 0) != -1) {
            record = D_800406A0_MGR.pending;
            received++;
            found = 0;
            while (record != 0 && found == 0) {
                if (record->buffer == ((TransferIoMessage *)message)->dramAddress) {
                    if (record == D_800406A0_MGR.pending) {
                        D_800406A0_MGR.pending = record->next;
                    }
                    {
                        AudioBufferState *element = record;

                        if (element->next != 0) {
                            element->next->prev = element->prev;
                        }
                        if (element->prev != 0) {
                            element->prev->next = element->next;
                        }
                    }
                    record->next = 0;
                    record->prev = 0;
                    if (D_800406A0_MGR.active != 0) {
                        AudioBufferState *linkNode = record;
                        AudioBufferState *linkAfter = D_800406A0_MGR.active;

                        linkNode->next = linkAfter->next;
                        linkNode->prev = linkAfter;
                        if (linkAfter->next != 0) {
                            linkAfter->next->prev = linkNode;
                        }
                        linkAfter->next = linkNode;
                    } else {
                        D_800406A0_MGR.active = record;
                        record->next = 0;
                        record->prev = 0;
                    }
                    if (record->field16 == 1) {
                        instrument = record->buffer;
                        for (sound = 0; sound < (u32)instrument->soundCount; sound++) {
                            instrument->sounds[sound] = (AudioBankSound *)
                                ((u32)instrument->sounds[sound] + (u32)record->buffer);
                        }
                    }
                    record->state = 1;
                    record->count++;
                    found = 1;
                } else {
                    record = record->next;
                }
            }
        }
    }
    D_8002AE50 -= received;
    record = D_800406A0_MGR.freeAnchor;
    if (record != 0) {
        do {
            busy = 0;
            next = record->next;
            if (record->field16 == 1) {
                AudioInstrument *cleanupInstrument = record->buffer;
                for (sound = 0; sound < (u32)cleanupInstrument->soundCount; sound++) {
                    wave = cleanupInstrument->sounds[sound]->wave;
                    if (wave->fieldA != 0) {
                        wave->fieldA = 0;
                        busy = 1;
                    }
                }
            }
            if (busy == 0) {
                record->count = 0;
                record->state = 0;
                func_10004074((s32)record->buffer);
                record->ownerSlot = 0;
                if (record == D_800406A0_MGR.freeAnchor) {
                    D_800406A0_MGR.freeAnchor = next;
                }
                {
                    AudioBufferState *element = record;

                    if (element->next != 0) {
                        element->next->prev = element->prev;
                    }
                    if (element->prev != 0) {
                        element->prev->next = element->next;
                    }
                }
                if (D_800406A0_MGR.base != 0) {
                    AudioBufferState *linkNode = record;
                    AudioBufferState *linkAfter = D_800406A0_MGR.base;

                    linkNode->next = linkAfter->next;
                    linkNode->prev = linkAfter;
                    if (linkAfter->next != 0) {
                        linkAfter->next->prev = record;
                    }
                    linkAfter->next = record;
                    record = next;
                    continue;
                } else {
                    D_800406A0_MGR.base = record;
                    record->next = 0;
                    record->prev = 0;
                }
            }
            record = next;
        } while (next != 0);
    }
    if (D_8003E384 != 0) {
        func_1000A348();
        D_8003E384 = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/init_8F90/func_1000A348.s")
