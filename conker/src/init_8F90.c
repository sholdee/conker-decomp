#include <ultra64.h>

#define func_10009BE4 func_10009BE4_hdr
#define struct50 struct50_hdr
#define D_8003E390 D_8003E390_hdr
#define D_80040F88 D_80040F88_hdr
#define D_80040F8C D_80040F8C_hdr
#define D_80040F90 D_80040F90_hdr
#include "functions.h"
#undef func_10009BE4
#define D_8002AE54 D_8002AE54_orig
#define D_8003E388 D_8003E388_orig
#define D_800406B8 D_800406B8_orig
#define D_80040AC8 D_80040AC8_orig
#include "variables.h"
#undef D_8002AE54
#undef D_8003E388
#undef D_800406B8
#undef D_80040AC8
#undef struct50
#undef D_8003E390
#undef D_80040F88
#undef D_80040F8C
#undef D_80040F90
extern u32 D_80040F88;
extern u32 D_80040F8C;
extern u32 D_80040F90;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u32 unk1C;
    s32 unk20;
    s32 unk24;
    u32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s16 unk70;
    s16 unk72;
    void *unk74;
    s32 pad78[6];
} struct50;
extern struct50 *D_8003E390[];

extern u8 D_8002C960[];
Acmd *n_alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 maxSamples);
s32 func_100095A0(struct50 *arg0, struct51 *arg1);
void func_100099BC(void);
void func_1000A03C(void);


typedef struct {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    void *dmaNew;
    void *unk14;
    void *unk18;
    void *unk1C;
    void *unk20;
    void *waveRom;
    ALHeap *heap;
    s32 outputRate;
    u8 fxType[4];
    void *unk34;
    void *unk38;
} SynDriverConfig;

typedef struct {
    u32 frequency;
    u32 numFields;
    u32 maxACMDSize;
} AudioSpec;

typedef struct {
    s32 unk0[66];
    s32 unk108[66];
} FxParams;

typedef struct DMAState_s {
    ALLink link;
    s32 unk8;
    s32 unkC;
    void *unk10;
} DMAState;

typedef struct {
    Acmd *ACMDList[2];
    struct50 *audio_info[3];
} AudioMgrPtrs;

extern FxParams D_8002AE54;
extern void *D_8003E380;
extern AudioMgrPtrs D_8003E388;
extern DMAState D_800406B8[52];
extern struct54 D_80040AC8[50];
extern u32 D_80040F94;

void func_10009BE4(struct54 *arg0);


void func_10008F90(SynDriverConfig *arg0, OSPri arg1, AudioSpec *arg2) {
    u32 i;
    f32 fsize;

    func_10012588((struct14 *)&D_8003E370);
    arg0->dmaNew = func_10009980;
    arg0->outputRate = osAiSetFrequency(arg2->frequency);
    arg0->unk14 = func_10009FFC;
    arg0->unk18 = func_10009B2C;
    arg0->unk1C = func_10009B90;
    arg0->unk20 = func_10009B4C;

    fsize = (f32)arg2->numFields * arg0->outputRate / 30.0f;
    D_80040F8C = (s32)fsize;
    if (D_80040F8C < fsize) {
        D_80040F8C++;
    }
    D_80040F8C = ((D_80040F8C / 0xB8) * 0xB8) + 0xB8;
    D_80040F88 = D_80040F8C - 0xB8;
    D_80040F90 = D_80040F8C + 0x54;
    D_80040F84 = 0;

    {
        FxParams sp58;
        sp58 = D_8002AE54;
        arg0->unk34 = sp58.unk0;
        arg0->unk38 = sp58.unk108;
        n_alInit(&D_8003E640, arg0);
    }
    D_8003E380 = &D_8003E640;

    D_800406B8[0].link.prev = NULL;
    D_800406B8[0].link.next = NULL;
    for (i = 0; i < 51; i++) {
        {
            ALLink *dl1 = &D_800406B8[i + 1].link;
            ALLink *dl2 = &D_800406B8[i].link;
            dl1->next = dl2->next;
            dl1->prev = dl2;
            if (dl2->next != NULL) {
                dl2->next->prev = dl1;
            }
            dl2->next = dl1;
        }
        D_800406B8[i].unk10 = alHeapDBAlloc(0, 0, arg0->heap, 1, 0x800);
    }
    D_800406B8[i].unk10 = alHeapDBAlloc(0, 0, arg0->heap, 1, 0x800);

    bzero(D_80040AC8, 0x4B0);
    D_80040AC8[0].unk4 = NULL;
    D_80040AC8[0].unk0 = NULL;
    for (i = 0; i < 49; i++) {
        {
            struct54 *ol1 = &D_80040AC8[i + 1];
            struct54 *ol2 = &D_80040AC8[i];
            ol1->unk0 = ol2->unk0;
            ol1->unk4 = ol2;
            if (ol2->unk0 != NULL) {
                ol2->unk0->unk4 = ol1;
            }
            ol2->unk0 = ol1;
        }
        D_80040AC8[i].unk10 = 0;
    }
    D_80040AC8[i].unk10 = 0;

    for (i = 0; i < 2; i++) {
        D_8003E388.ACMDList[i] = alHeapDBAlloc(0, 0, arg0->heap, 1, arg2->maxACMDSize * sizeof(Acmd));
    }
    D_80040F94 = arg2->maxACMDSize;

    for (i = 0; i < 3; i++) {
        D_8003E388.audio_info[i] = alHeapDBAlloc(0, 0, arg0->heap, 1, sizeof(struct50));
        D_8003E388.audio_info[i]->unk70 = 2;
        D_8003E388.audio_info[i]->unk74 = D_8003E388.audio_info[i];
        D_8003E388.audio_info[i]->unk0 = (s32)alHeapDBAlloc(0, 0, arg0->heap, 1, 4 * D_80040F90);
    }

    osCreateMesgQueue((OSMesgQueue *)&D_8003E608, (OSMesg *)&D_8003E620, 8);
    osCreateMesgQueue((OSMesgQueue *)&D_8003E5D0, (OSMesg *)&D_8003E5E8, 8);
    osCreateMesgQueue((OSMesgQueue *)&D_80041298, (OSMesg *)&D_800412B0, 0x20);
    osCreateMesgQueue((OSMesgQueue *)&D_800416F0, (OSMesg *)&D_80041708, 0x28);
    osCreateThread(&D_8003E3A0, 4, (void (*)(void *))func_10009400, NULL, &D_800406A0, arg1);
    D_8002AE40 = 1;
    osStartThread(&D_8003E3A0);
}

void func_100093CC(void) {
    if (D_8002AE40 != 0) {
        osStopThread(&D_8003E3A0);
    }
}

// audio thread
void func_10009400(s32 arg0) {
    u32 phi_s1;
    struct51 *phi_s5;
    struct53 *msg;
    u32 phi_s0;
    struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
    } sp4C;
    s32 phi_s4;

    phi_s1 = 0;
    msg = NULL;
    phi_s5 = NULL;
    phi_s0 = 0;
    phi_s4 = 1;
    sp4C.unk8 = 0;
    func_100051C8((OSPfs *)&sp4C, (OSMesgQueue *)&D_8003E5D0);
    while (phi_s1 == 0) {
        osRecvMesg((OSMesgQueue *)&D_8003E5D0, (OSMesg *)&msg, 1);
        if (D_8002AC5C != 0) {
            msg->unk0 = 4;
        }
        switch (msg->unk0) {
            case 1:
                if (phi_s0 >= 2U) {
                    phi_s0 = 0;
                }
                if ((phi_s0 == 0) && (func_100095A0(D_8003E390[D_8002AE44 % 3U], phi_s5) != 0)) {
                    if (phi_s4 == 0) {
                        osRecvMesg((OSMesgQueue *)&D_8003E608, (OSMesg *)&msg, 1);
                        phi_s5 = (struct51 *)msg->unk4;
                    }
                    phi_s4 = 0;
                }
                phi_s0++;
                break;
            case 4:
                phi_s1 = 1U;
                break;
            case 10:
                phi_s1 = 1U;
                break;
        }
    }
    n_alClose(&D_8003E640);
    do {
        osRecvMesg((OSMesgQueue *)&D_8003E5D0, (OSMesg *)&msg, 1);
    } while (1);
}

typedef struct {
    u32 type;
    u32 flags;
    void *boot;
    u32 bootSize;
    void *microcode;
    u32 microcodeSize;
    void *microcodeData;
    u32 microcodeDataSize;
    void *stack;
    u32 stackSize;
    void *output;
    void *outputSize;
    Acmd *commands;
    u32 commandSize;
    void *yieldData;
    u32 yieldSize;
} AudioRspTask95A0;

typedef struct {
    void *next;
    u8 pad4[8];
    u32 flags;
    void *framebuffer;
    u8 pad14[4];
    AudioRspTask95A0 task;
    OSMesgQueue *completionQueue;
    void *completionMessage;
} AudioSchedulerTask95A0;

typedef struct {
    u8 *buffer;
    u8 *adjustedBuffer;
    s16 samples;
    u8 padA[6];
    AudioSchedulerTask95A0 scheduler;
    s16 completion;
} AudioTaskRecord95A0;

#define info ((AudioTaskRecord95A0 *)arg0)
#define lastInfo ((AudioTaskRecord95A0 *)arg1)
s32 func_100095A0(struct50 *arg0, struct51 *arg1) {
    s16 *audioPtr;
    Acmd *cmdp;
    s32 cmdLen;
    s32 samplesLeft;
    AudioSchedulerTask95A0 *t;
    u32 end;

    audioPtr = (s16 *)osVirtualToPhysical(info->buffer);
    func_100099BC();
    func_1000A03C();
    samplesLeft = *(volatile u32 *)0xA4500004 >> 2;
    if (lastInfo != NULL) {
        osAiSetNextBuffer(lastInfo->adjustedBuffer, lastInfo->samples << 2);
    }
    if (samplesLeft >= 0xF9 && D_80040F84 == 0) {
        info->samples = D_80040F88;
        D_80040F84 = 2;
    } else {
        info->samples = D_80040F8C;
        if (D_80040F84 != 0) {
            D_80040F84--;
        }
    }
    end = (u32)audioPtr + (info->samples << 2);
    if ((end & 0x1FFF) == 0) {
        audioPtr += 8;
        info->adjustedBuffer = info->buffer + 0x10;
    } else {
        info->adjustedBuffer = info->buffer;
    }
    cmdp = n_alAudioFrame(D_8003E388.ACMDList[D_8002AE4C], &cmdLen, audioPtr, info->samples);
    if (cmdLen == 0) {
        return 0;
    }
    t = &info->scheduler;
    t->next = NULL;
    t->completionQueue = (OSMesgQueue *)&D_8003E608;
    t->completionMessage = &info->completion;
    t->flags = 2;
    t->framebuffer = NULL;
    t->task.commands = D_8003E388.ACMDList[D_8002AE4C];
    t->task.commandSize = (cmdp - D_8003E388.ACMDList[D_8002AE4C]) * sizeof(Acmd);
    t->task.type = 2;
    t->task.boot = (u8 *)&D_100290D0;
    t->task.bootSize = (s32)&D_100291A0 - (s32)&D_100290D0;
    t->task.flags = 0;
    t->task.microcode = (u8 *)&D_100290D0 + 0xD0;
    t->task.microcodeData = D_8002C960;
    t->task.microcodeDataSize = 0x800;
    t->task.yieldData = NULL;
    t->task.yieldSize = 0x400;
    osWritebackDCacheAll();
    osSendMesg(&D_8003B200, t, 1);
    D_8002AE4C ^= 1;
    return 1;
}
#undef info
#undef lastInfo

s32 func_100097CC(u32 arg0, s32 arg1, s32 arg2) {
    void *sp2C;
    s32 sp28;
    struct54 *v1;
    struct54 *a2;
    struct54 *v0;
    s32 temp_a0;

    v1 = (struct54 *)D_80040F78.unk4;
    a2 = NULL;
    while (v1 != NULL) {
        temp_a0 = v1->unk8 + 0x800;
        if (arg0 < v1->unk8) {
            break;
        }
        if (temp_a0 >= ((s32)arg0 + arg1)) {
            v1->unkC = D_8002AE44;
            return osVirtualToPhysical((void *)((v1->unk10 + arg0) - v1->unk8));
        }
        a2 = v1;
        v1 = v1->unk0;
    }
    v1 = (struct54 *)D_80040F78.unk8;
    if ((v1 == NULL) || ((u32)D_8002AE48 >= 0x20U)) {
        return 0;
    }
    D_80040F78.unk8 = (s32)v1->unk0;
    if (v1->unk0 != NULL) {
        v1->unk0->unk4 = v1->unk4;
    }
    if (v1->unk4 != NULL) {
        v1->unk4->unk0 = v1->unk0;
    }
    if (a2 != NULL) {
        {
            struct54 *dl1 = v1;
            struct54 *dl2 = a2;
            dl1->unk0 = dl2->unk0;
            dl1->unk4 = dl2;
            if (dl2->unk0 != NULL) {
                dl2->unk0->unk4 = dl1;
            }
            dl2->unk0 = dl1;
        }
    } else {
        v0 = (struct54 *)D_80040F78.unk4;
        if (v0 != NULL) {
            D_80040F78.unk4 = (s32)v1;
            v1->unk0 = v0;
            v1->unk4 = NULL;
            v0->unk4 = v1;
        } else {
            D_80040F78.unk4 = (s32)v1;
            v1->unk0 = NULL;
            v1->unk4 = NULL;
        }
    }
    sp2C = (void *)v1->unk10;
    sp28 = arg0 & 1;
    arg0 -= sp28;
    v1->unk8 = arg0;
    v1->unkC = D_8002AE44;
    osPiStartDma((OSIoMesg *)&D_80040F98[D_8002AE48++], 1, 0, arg0, sp2C, 0x800, (OSMesgQueue *)&D_80041298);
    return osVirtualToPhysical(sp2C) + sp28;
}

s32 func_10009980(s32 *arg0) {
    if (D_80040F78.unk0 == 0) {
        D_80040F78.unk4 = (u8) 0;
        D_80040F78.unk8 = &D_800406B8;
        D_80040F78.unk0 = 1;
    }
    *arg0 = 0;
    return func_100097CC;
}

void func_100099BC(void) {
    u32 i;
    OSMesg iomsg;
    struct54 *dmaPtr;
    struct54 *nextPtr;

    iomsg = NULL;
    for (i = 0; i < (u32)D_8002AE48; i++) {
        if (osRecvMesg((OSMesgQueue *)&D_80041298, &iomsg, 0) == -1) {
            osRecvMesg((OSMesgQueue *)&D_80041298, &iomsg, 1);
        }
    }
    dmaPtr = (struct54 *)D_80040F78.unk4;
    while (dmaPtr != NULL) {
        nextPtr = dmaPtr->unk0;
        if ((u32)(dmaPtr->unkC + 1) < D_8002AE44) {
            if ((struct54 *)D_80040F78.unk4 == dmaPtr) {
                D_80040F78.unk4 = (s32)nextPtr;
            }
            {
                struct54 *ln = dmaPtr;

                if (ln->unk0 != NULL) {
                    ln->unk0->unk4 = ln->unk4;
                }
                if (ln->unk4 != NULL) {
                    ln->unk4->unk0 = ln->unk0;
                }
            }
            if ((struct54 *)D_80040F78.unk8 != NULL) {
                struct54 *ln = dmaPtr;
                struct54 *after = (struct54 *)D_80040F78.unk8;

                ln->unk0 = after->unk0;
                ln->unk4 = after;
                if (after->unk0 != NULL) {
                    after->unk0->unk4 = ln;
                }
                after->unk0 = ln;
            } else {
                D_80040F78.unk8 = (s32)dmaPtr;
                dmaPtr->unk0 = NULL;
                dmaPtr->unk4 = NULL;
            }
        }
        dmaPtr = nextPtr;
    }
    D_8002AE48 = 0;
    D_8002AE44++;
}

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

void func_10009BE4(struct54 *arg0) {
    if ((s32)arg0 & 1) {
        D_8003C8E0 = 0x0F000004;
        func_150AD770();
    } else {
        struct54 *p;

        p = (struct54 *)arg0->unkC;
        p->unk0 = (struct54 *)arg0->unk8;
        if (arg0 == (struct54 *)D_800406A0.unk4) {
            D_800406A4 = arg0->unk0;
        }
        p = arg0->unk0;
        if (p != NULL) {
            p->unk4 = arg0->unk4;
        }
        p = arg0->unk4;
        if (p != NULL) {
            p->unk0 = arg0->unk0;
        }
        if (D_800406A0.unk10 != NULL) {
            struct54 *q = D_800406A0.unk10;

            arg0->unk0 = q->unk0;
            arg0->unk4 = q;
            if (q->unk0 != NULL) {
                q->unk0->unk4 = arg0;
            }
            q->unk0 = arg0;
        } else {
            D_800406B0 = arg0;
            arg0->unk0 = NULL;
            arg0->unk4 = NULL;
        }
    }
}

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

void func_1000A348(void) {
    AudioBufferState *record;
    AudioBufferState *next;
    AudioBufferState *anchor;
    s32 *ownerSlot;

    record = D_800406A0_MGR.active;
    if (record != 0) {
        do {
            next = record->next;
            if ((record->count == 0) && (record->field16 == 0)) {
                ownerSlot = record->ownerSlot;
                *ownerSlot = record->savedValue;
                record->ownerSlot = 0;
                if (record == D_800406A0_MGR.active) {
                    D_800406A0_MGR.active = next;
                }
                if (record->next != 0) {
                    record->next->prev = record->prev;
                }
                if (record->prev != 0) {
                    record->prev->next = record->next;
                }
                anchor = D_800406A0_MGR.freeAnchor;
                if (anchor != 0) {
                    {
                        AudioBufferState *linkNode = record;
                        AudioBufferState *linkAfter;

                        linkNode->next = anchor->next;
                        linkNode->prev = anchor;
                        linkAfter = anchor;
                        anchor = linkAfter->next;
                        if (linkAfter->next != 0) {
                            linkAfter->next->prev = linkNode;
                        }
                        linkAfter->next = linkNode;
                    }
                } else {
                    D_800406A0_MGR.freeAnchor = record;
                    record->next = 0;
                    record->prev = 0;
                }
            }
            record = next;
        } while (next != 0);
    }
}
