#include <ultra64.h>

/*
 * Audio manager: synthesis driver setup, the audio thread, the sample DMA cache
 * (after the SDK's audiomgr.c) and Conker's sound bank loader.
 */

#define QUIT_MSG 10

#define NUM_DMA_BUFFERS   52
#define DMA_BUFFER_LENGTH 0x800
#define MAX_SAMPLE_DMAS   32 /* sample DMAs per frame */

#define NUM_BANK_RECORDS 50
#define MAX_BANK_DMAS    40 /* bank loads in flight */

#define AUDIO_STACK_SIZE 0x2000

/* Output frames are a whole number of 184-sample blocks. */
#define FRAME_SAMPLE_ALIGN 184
#define EXTRA_SAMPLES      84

/* libaudio's alLink() and alUnlink(), inlined for any node type with next/prev links. */
#define LIST_LINK(type, element, after) { type *ln = (element); type *to = (after); ln->next = to->next; ln->prev = to; if (to->next != NULL) { to->next->prev = ln; } to->next = ln; }
#define LIST_UNLINK(type, element) { type *ln = (element); if (ln->next != NULL) { ln->next->prev = ln->prev; } if (ln->prev != NULL) { ln->prev->next = ln->next; } }

typedef struct {
    Acmd *ACMDList[2];
    struct AudioInfo_s *audioInfo[3];
} AudioMgr;

struct AudioMgrConfig {
    u32 frequency;
    u32 framesPerField;
    u32 maxACMDSize;
};

typedef struct {
    s32 values[2][66];
} FxParams;

/* A cached 0x800-byte window of sample data in ROM. */
typedef struct DMABuffer {
    struct DMABuffer *next;
    struct DMABuffer *prev;
    u32 startAddr;
    u32 lastFrame;
    u8 *ptr;
} DMABuffer;

typedef struct {
    u8 initialized;
    DMABuffer *firstUsed; /* sorted by startAddr */
    DMABuffer *firstFree;
} DMAState;

/*
 * A bank loaded from ROM on demand. The owner slot holds either a ROM reference
 * (bit 0 set: ROM offset in the upper bits, size in the low byte) or, once a load
 * has started, a pointer to its record.
 */
struct BankRecord {
    struct BankRecord *next;
    struct BankRecord *prev;
    u32 romRef;     /* the owner slot's original contents */
    u32 *ownerSlot;
    void *buffer;
    s8 refCount;
    u8 state;       /* 0: loading, 1: loaded, 2: claimed */
    u8 mode;        /* 1: an instrument whose sound pointers are relocated after loading */
};

typedef struct {
    u8 initialized;
    struct BankRecord *loadedList;
    struct BankRecord *freeList;
    struct BankRecord *pendingList; /* DMA in flight */
    struct BankRecord *releaseList; /* unreferenced, buffer still allocated */
} BankState;

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

/*
 * This file's .bss, 0x8003E380-0x800417B0. It is defined before functions.h and variables.h: IDO places a
 * variable that an extern declared earlier (gReleaseRawBanksPending, of conker/audio.h) after the others. The types
 * above are defined before it on purpose: IDO rotates the .bss order by the number of types and externs
 * declared before the definitions, and with fewer of them D_8003E380 is no longer first.
 */
static void *D_8003E380;          /* the synthesis driver's globals */
s32 gReleaseRawBanksPending;
static AudioMgr gAudioMgr;
static OSThread gAudioThread;       /* audio thread */
static u8 D_8003E550[0x80];       /* not referenced */
static OSMesgQueue gAudioMsgQueue;    /* retrace and pre-NMI messages from the scheduler */
static OSMesg D_8003E5E8[8];
static OSMesgQueue gAudioDoneQueue;    /* audio task done messages */
static OSMesg D_8003E620[8];
static u8 D_8003E640[0x60];       /* N_ALGlobals of the synthesis driver (n_alInit) */
static u64 gAudioThreadStack[AUDIO_STACK_SIZE / sizeof(u64)]; /* audio thread stack */

/*
 * The bank loader's state: five separate variables that the code mostly uses as one BankState.
 * matching: the ROM needs both forms. Most accesses go through one base register at D_800406A0 with
 * offsets 0x4 to 0x10, which IDO emits only for one object (BANK_STATE). Three stores in audio_retireBank
 * and audio_fetchBank have their own lui, which IDO emits only for a separate symbol (D_800406A4,
 * D_800406B0). Separate variables read by name, or a defined BankState, each change those functions.
 */
static u8 D_800406A0;          /* the lists are set up */
static struct BankRecord *D_800406A4; /* loaded */
static struct BankRecord *D_800406A8; /* free */
static struct BankRecord *D_800406AC; /* DMA in flight */
static struct BankRecord *D_800406B0; /* unreferenced, buffer still allocated */
#define BANK_STATE (*(BankState *)&D_800406A0)

static DMABuffer gAudioDmaBuffers[NUM_DMA_BUFFERS];
static struct BankRecord gBankRecords[NUM_BANK_RECORDS];
static DMAState gAudioDmaState;
static u8 D_80040F84;               /* frames left before the next short frame */
static u32 D_80040F88;              /* short audio frame, in samples */
static u32 D_80040F8C;              /* audio frame, in samples */
static u32 D_80040F90;              /* longest audio frame, in samples */
static u32 D_80040F94;              /* command list length */
static OSIoMesg gSampleDmaMsgs[MAX_SAMPLE_DMAS];
static OSMesgQueue gSampleDmaQueue;      /* sample DMA completions */
static OSMesg D_800412B0[MAX_SAMPLE_DMAS];
static OSIoMesg gBankDmaMsgs[MAX_BANK_DMAS];
static OSMesgQueue gBankDmaQueue;      /* bank DMA completions */
static OSMesg D_80041708[MAX_BANK_DMAS];

#include "functions.h"
#include "variables.h"


typedef union {
    struct {
        s16 type;
    } gen;
    struct {
        s16 type;
        AudioInfo *info;
    } done;
    OSScMsg app;
} AudioMsg;

/* One audio frame: its output buffer, its RSP task and the task's completion message. */
struct AudioInfo_s {
    u8 *data;
    u8 *alignedData; /* data, or data + 0x10 when the frame would end on an 8KB boundary */
    s16 frameSamples;
    OSScTask task;
    AudioMsg msg;
};

struct SynDriverConfig {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    ALDMANew dma;
    BankFetchProc (*bankFetchNew)(void);
    void (*bankDecRef)(BankRecord *record);
    void (*bankAddRef)(BankRecord *record);
    void (*bankRelease)(BankRecord *record);
    void *waveRom;
    ALHeap *heap;
    s32 outputRate;
    u8 fxType[4];
    s32 *params[2];
};

extern u8 D_8002C960[];

/* This file's .data. */
static u8 gAudioThreadStarted = FALSE; /* the audio thread was started */
static u32 gAudioFrameCount = 0;    /* audio frame count */
static u32 gSampleDmaCount = 0;    /* sample DMAs issued this frame */
static s32 D_8002AE4C = 0;    /* the command list being built */
static u32 gBankDmaCount = 0;    /* bank DMAs in flight */
/* Effect parameters of the two FX busses: the section count, the delay length, then
   8 values per section (input, output, fbcoef, ffcoef, gain, chorus rate, chorus depth, lowpass). */
static FxParams gAudioFxParams = { {
        { 8, 6712,
          0, 192, 9830, -9830, 0, 0, 0, 0,
          192, 392, 9830, -9830, 11140, 0, 0, 9472,
          880, 2816, 16384, -16384, 4587, 0, 0, 12288,
          1056, 2112, 8192, -8192, 0, 0, 0, 0,
          3520, 6160, 16384, -16384, 4587, 0, 0, 13568,
          3696, 5280, 8192, -8192, 0, 0, 0, 0,
          5280, 5944, 8192, -8192, 0, 0, 0, 0,
          0, 6512, 13000, -13000, 0, 380, 10, 17664 },
        { 1, 1760,
          0, 1536, 13108, 0, 29493, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0 },
} };

Acmd *n_alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 maxSamples);

/* Sets up the synthesis driver, the DMA and bank caches and starts the audio thread. */
void audio_createAudioMgr(SynDriverConfig *config, OSPri priority, AudioMgrConfig *amConfig) {
    u32 i;
    f32 fsize;

    mp3_initStream(&gAudioHeap);
    config->dma = audio_dmaNew;
    config->outputRate = osAiSetFrequency(amConfig->frequency);
    config->bankFetchNew = audio_bankFetchNew;
    config->bankDecRef = audio_bankDecRef;
    config->bankAddRef = audio_bankAddRef;
    config->bankRelease = audio_bankRelease;

    fsize = (f32)amConfig->framesPerField * config->outputRate / 30.0f;
    D_80040F8C = (s32)fsize;
    if (D_80040F8C < fsize) {
        D_80040F8C++;
    }
    D_80040F8C = ((D_80040F8C / FRAME_SAMPLE_ALIGN) * FRAME_SAMPLE_ALIGN) + FRAME_SAMPLE_ALIGN;
    D_80040F88 = D_80040F8C - FRAME_SAMPLE_ALIGN;
    D_80040F90 = D_80040F8C + EXTRA_SAMPLES;
    D_80040F84 = 0;

    {
        FxParams params = gAudioFxParams;

        config->params[0] = params.values[0];
        config->params[1] = params.values[1];
        n_alInit(&D_8003E640, config);
    }
    D_8003E380 = &D_8003E640;

    gAudioDmaBuffers[0].prev = NULL;
    gAudioDmaBuffers[0].next = NULL;
    for (i = 0; i < NUM_DMA_BUFFERS - 1; i++) {
        LIST_LINK(DMABuffer, &gAudioDmaBuffers[i + 1], &gAudioDmaBuffers[i]);
        gAudioDmaBuffers[i].ptr = alHeapAlloc(config->heap, 1, DMA_BUFFER_LENGTH);
    }
    gAudioDmaBuffers[i].ptr = alHeapAlloc(config->heap, 1, DMA_BUFFER_LENGTH);

    bzero(gBankRecords, sizeof(gBankRecords));
    gBankRecords[0].prev = NULL;
    gBankRecords[0].next = NULL;
    for (i = 0; i < NUM_BANK_RECORDS - 1; i++) {
        LIST_LINK(BankRecord, &gBankRecords[i + 1], &gBankRecords[i]);
        gBankRecords[i].buffer = NULL;
    }
    gBankRecords[i].buffer = NULL;

    for (i = 0; i < 2; i++) {
        gAudioMgr.ACMDList[i] = alHeapAlloc(config->heap, 1, amConfig->maxACMDSize * sizeof(Acmd));
    }
    D_80040F94 = amConfig->maxACMDSize;

    for (i = 0; i < 3; i++) {
        gAudioMgr.audioInfo[i] = alHeapAlloc(config->heap, 1, sizeof(AudioInfo));
        gAudioMgr.audioInfo[i]->msg.done.type = OS_SC_DONE_MSG;
        gAudioMgr.audioInfo[i]->msg.done.info = gAudioMgr.audioInfo[i];
        gAudioMgr.audioInfo[i]->data = alHeapAlloc(config->heap, 1, 4 * D_80040F90);
    }

    osCreateMesgQueue(&gAudioDoneQueue, D_8003E620, 8);
    osCreateMesgQueue(&gAudioMsgQueue, D_8003E5E8, 8);
    osCreateMesgQueue(&gSampleDmaQueue, D_800412B0, MAX_SAMPLE_DMAS);
    osCreateMesgQueue(&gBankDmaQueue, D_80041708, MAX_BANK_DMAS);
    osCreateThread(&gAudioThread, 4, audio_threadMain, NULL, gAudioThreadStack + AUDIO_STACK_SIZE / sizeof(u64), priority);
    gAudioThreadStarted = TRUE;
    osStartThread(&gAudioThread);
}

void audio_stopThread(void) {
    if (gAudioThreadStarted) {
        osStopThread(&gAudioThread);
    }
}

/* Audio thread: builds and submits an audio frame on every second retrace. */
void audio_threadMain(void *arg) {
    u32 done;
    AudioInfo *lastInfo;
    AudioMsg *msg;
    u32 fieldCount;
    SchedulerClient client;
    s32 firstFrame;

    done = FALSE;
    msg = NULL;
    lastInfo = NULL;
    fieldCount = 0;
    firstFrame = TRUE;
    client.flags = 0;
    sched_addClient(&client, &gAudioMsgQueue);
    while (!done) {
        osRecvMesg(&gAudioMsgQueue, (OSMesg *)&msg, OS_MESG_BLOCK);
        if (gResetPending != 0) {
            msg->gen.type = OS_SC_PRE_NMI_MSG;
        }
        switch (msg->gen.type) {
            case OS_SC_RETRACE_MSG:
                if (fieldCount >= 2) {
                    fieldCount = 0;
                }
                if (fieldCount == 0 && audio_handleFrame(gAudioMgr.audioInfo[gAudioFrameCount % 3], lastInfo) != 0) {
                    if (!firstFrame) {
                        /* wait for the previous frame's task */
                        osRecvMesg(&gAudioDoneQueue, (OSMesg *)&msg, OS_MESG_BLOCK);
                        lastInfo = msg->done.info;
                    }
                    firstFrame = FALSE;
                }
                fieldCount++;
                break;
            case OS_SC_PRE_NMI_MSG:
                done = TRUE;
                break;
            case QUIT_MSG:
                done = TRUE;
                break;
        }
    }
    n_alClose(&D_8003E640);
    for (;;) {
        osRecvMesg(&gAudioMsgQueue, (OSMesg *)&msg, OS_MESG_BLOCK);
    }
}

/*
 * Queues the previous frame's samples to the AI, then builds this frame's command
 * list and sends its task to the scheduler. Returns 0 when there was nothing to do.
 */
s32 audio_handleFrame(AudioInfo *info, AudioInfo *lastInfo) {
    s16 *outBuffer;
    Acmd *cmdp;
    s32 cmdLen;
    s32 samplesLeft;
    OSScTask *t;
    u32 end;

    outBuffer = (s16 *)osVirtualToPhysical(info->data);
    audio_clearDma();
    audio_updateBanks();
    samplesLeft = IO_READ(AI_LEN_REG) >> 2;
    if (lastInfo != NULL) {
        osAiSetNextBuffer(lastInfo->alignedData, lastInfo->frameSamples << 2);
    }
    if (samplesLeft >= 249 && D_80040F84 == 0) {
        /* the AI is running ahead: make a short frame */
        info->frameSamples = D_80040F88;
        D_80040F84 = 2;
    } else {
        info->frameSamples = D_80040F8C;
        if (D_80040F84 != 0) {
            D_80040F84--;
        }
    }
    end = (u32)outBuffer + (info->frameSamples << 2);
    if ((end & 0x1FFF) == 0) {
        outBuffer += 8;
        info->alignedData = info->data + 0x10;
    } else {
        info->alignedData = info->data;
    }
    cmdp = n_alAudioFrame(gAudioMgr.ACMDList[D_8002AE4C], &cmdLen, outBuffer, info->frameSamples);
    if (cmdLen == 0) {
        return 0;
    }
    t = &info->task;
    t->next = NULL;
    t->msgQ = &gAudioDoneQueue;
    t->msg = &info->msg;
    t->flags = OS_SC_NEEDS_RSP;
    t->framebuffer = NULL;
    t->list.t.data_ptr = (u64 *)gAudioMgr.ACMDList[D_8002AE4C];
    t->list.t.data_size = (cmdp - gAudioMgr.ACMDList[D_8002AE4C]) * sizeof(Acmd);
    t->list.t.type = M_AUDTASK;
    t->list.t.ucode_boot = (u64 *)D_100290D0;
    t->list.t.ucode_boot_size = (s32)D_100291A0 - (s32)D_100290D0;
    t->list.t.flags = 0;
    t->list.t.ucode = (u64 *)(D_100290D0 + 0xD0); /* aspMain text follows rspboot */
    t->list.t.ucode_data = (u64 *)D_8002C960;
    t->list.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    t->list.t.yield_data_ptr = NULL;
    t->list.t.yield_data_size = OS_YIELD_AUDIO_SIZE;
    osWritebackDCacheAll();
    osSendMesg(&gSchedAudioTaskQueue, t, OS_MESG_BLOCK);
    D_8002AE4C ^= 1;
    return 1;
}

/* ALDMAproc: returns the physical address of ROM sample data, DMAing it into a cache buffer if needed. */
s32 audio_dmaSample(s32 addr, s32 len, void *state) {
    u8 *foundBuffer;
    s32 delta;
    DMABuffer *dmaPtr;
    DMABuffer *lastDmaPtr;
    s32 buffEnd;

    dmaPtr = gAudioDmaState.firstUsed;
    lastDmaPtr = NULL;
    while (dmaPtr != NULL) {
        buffEnd = dmaPtr->startAddr + DMA_BUFFER_LENGTH;
        if (dmaPtr->startAddr > addr) {
            break; /* the list is sorted, so no later buffer holds it */
        }
        if (buffEnd >= addr + len) {
            dmaPtr->lastFrame = gAudioFrameCount;
            return osVirtualToPhysical(dmaPtr->ptr + addr - dmaPtr->startAddr);
        }
        lastDmaPtr = dmaPtr;
        dmaPtr = dmaPtr->next;
    }

    dmaPtr = gAudioDmaState.firstFree;
    if (dmaPtr == NULL || gSampleDmaCount >= MAX_SAMPLE_DMAS) {
        return 0;
    }
    gAudioDmaState.firstFree = dmaPtr->next;
    LIST_UNLINK(DMABuffer, dmaPtr);
    if (lastDmaPtr != NULL) {
        LIST_LINK(DMABuffer, dmaPtr, lastDmaPtr);
    } else if (gAudioDmaState.firstUsed != NULL) {
        lastDmaPtr = gAudioDmaState.firstUsed;
        gAudioDmaState.firstUsed = dmaPtr;
        dmaPtr->next = lastDmaPtr;
        dmaPtr->prev = NULL;
        lastDmaPtr->prev = dmaPtr;
    } else {
        gAudioDmaState.firstUsed = dmaPtr;
        dmaPtr->next = NULL;
        dmaPtr->prev = NULL;
    }
    foundBuffer = dmaPtr->ptr;
    delta = addr & 1;
    addr -= delta;
    dmaPtr->startAddr = addr;
    dmaPtr->lastFrame = gAudioFrameCount;
    osPiStartDma(&gSampleDmaMsgs[gSampleDmaCount++], OS_MESG_PRI_HIGH, OS_READ, addr, foundBuffer, DMA_BUFFER_LENGTH,
                 &gSampleDmaQueue);
    return osVirtualToPhysical(foundBuffer) + delta;
}

/* ALDMANew */
ALDMAproc audio_dmaNew(void *state) {
    if (!gAudioDmaState.initialized) {
        gAudioDmaState.firstUsed = NULL;
        gAudioDmaState.firstFree = gAudioDmaBuffers;
        gAudioDmaState.initialized = TRUE;
    }
    *(void **)state = NULL;
    return audio_dmaSample;
}

/* Collects this frame's sample DMAs and frees cache buffers unused for two frames. */
void audio_clearDma(void) {
    u32 i;
    OSMesg iomsg;
    DMABuffer *dmaPtr;
    DMABuffer *nextPtr;

    iomsg = NULL;
    for (i = 0; i < gSampleDmaCount; i++) {
        if (osRecvMesg(&gSampleDmaQueue, &iomsg, OS_MESG_NOBLOCK) == -1) {
            osRecvMesg(&gSampleDmaQueue, &iomsg, OS_MESG_BLOCK);
        }
    }
    dmaPtr = gAudioDmaState.firstUsed;
    while (dmaPtr != NULL) {
        nextPtr = dmaPtr->next;
        if (dmaPtr->lastFrame + 1 < gAudioFrameCount) {
            if (gAudioDmaState.firstUsed == dmaPtr) {
                gAudioDmaState.firstUsed = nextPtr;
            }
            LIST_UNLINK(DMABuffer, dmaPtr);
            if (gAudioDmaState.firstFree != NULL) {
                LIST_LINK(DMABuffer, dmaPtr, gAudioDmaState.firstFree);
            } else {
                gAudioDmaState.firstFree = dmaPtr;
                dmaPtr->next = NULL;
                dmaPtr->prev = NULL;
            }
        }
        dmaPtr = nextPtr;
    }
    gSampleDmaCount = 0;
    gAudioFrameCount++;
}

/* Drops a reference to a loaded bank. */
void audio_bankDecRef(BankRecord *record) {
    if (((u32)record & 1) == 0) {
        record->refCount--;
    }
}

/* Drops a reference to a loaded bank and releases it when none are left. */
void audio_bankRelease(BankRecord *record) {
    if (((u32)record & 1) == 0) {
        record->refCount--;
        if (record->refCount == 0) {
            audio_retireBank(record);
        }
    }
}

/* Takes a reference to a loaded bank. */
void audio_bankAddRef(BankRecord *record) {
    if (((u32)record & 1) == 0) {
        if (record->state == 1) {
            if (record->mode == 1) {
                record->refCount++;
            }
            record->state = 2;
            return;
        }
        record->refCount++;
    }
}

/* Restores the bank's owner slot and moves its record to the release list. */
void audio_retireBank(BankRecord *record) {
    if ((u32)record & 1) {
        D_8003C8E0 = 0x0F000004;
        debug_fatalTrap();
    } else {
        u32 *ownerSlot = record->ownerSlot;

        *ownerSlot = record->romRef;
        if (record == BANK_STATE.loadedList) {
            D_800406A4 = record->next;
        }
        LIST_UNLINK(BankRecord, record);
        if (BANK_STATE.releaseList != NULL) {
            LIST_LINK(BankRecord, record, BANK_STATE.releaseList);
        } else {
            D_800406B0 = record;
            record->next = NULL;
            record->prev = NULL;
        }
    }
}

/*
 * Bank fetch: returns the loaded bank for an owner slot, or NULL while it is not
 * loaded yet, starting its DMA (and pointing the slot at its record) on first use.
 */
void *audio_fetchBank(u32 *slot, s32 mode) {
    BankRecord *record;
    BankRecord *reuse;
    s32 size;
    s32 alignedSize;
    BankRecord *unused; // matching: IDO -g3 gives every local a stack slot; this one shifts the rest
    u32 value;
    u32 *ownerSlot;

    reuse = NULL;
    value = *slot;
    if (value & 1) {
        if (mode == 0) {
            size = (value & 0xFF) + 1;
        } else {
            size = (value & 0xFF & ~1) << 6;
        }
        record = BANK_STATE.freeList;
        if (record == NULL) {
            /* no free record: evict an unreferenced, claimed raw bank */
            record = BANK_STATE.loadedList;
            while (record != NULL) {
                if (record->refCount == 0 && record->mode == 0 && record->state == 2) {
                    reuse = record;
                }
                record = record->next;
            }
            record = reuse;
            if (reuse != NULL) {
                ownerSlot = reuse->ownerSlot;
                *ownerSlot = reuse->romRef;
                heap_setTag(reuse->buffer, 4);
                reuse->buffer = NULL;
                reuse->ownerSlot = NULL;
                if (reuse == BANK_STATE.loadedList) {
                    D_800406A4 = reuse->next;
                }
                LIST_UNLINK(BankRecord, reuse);
            }
        } else {
            BANK_STATE.freeList = record->next;
            LIST_UNLINK(BankRecord, record);
        }
        if (record != NULL) {
            record->next = NULL;
            record->prev = NULL;
            if (BANK_STATE.pendingList != NULL) {
                LIST_LINK(BankRecord, record, BANK_STATE.pendingList);
            } else {
                BANK_STATE.pendingList = record;
                record->next = NULL;
                record->prev = NULL;
            }
            record->romRef = value;
            record->refCount = 0;
            record->mode = mode;
            record->ownerSlot = slot;
            record->state = 0;
            if (gBankDmaCount < MAX_BANK_DMAS) {
                alignedSize = (size + 0xF) & ~0xF;
                record->buffer = allocate_memory(alignedSize, 0xFF, 2, 0);
                bzero(record->buffer, alignedSize);
            } else {
                record->buffer = NULL;
            }
            if (record->buffer != NULL) {
                osWritebackDCache(record->buffer, alignedSize);
                osInvalDCache(record->buffer, alignedSize);
                osPiStartDma(&gBankDmaMsgs[gBankDmaCount++], OS_MESG_PRI_HIGH, OS_READ, (value >> 5) & ~7,
                             record->buffer, alignedSize, &gBankDmaQueue);
                *slot = (u32)record;
            }
        }
        return NULL;
    }
    *slot = value;
    record = (BankRecord *)value;
    if (record->state != 0) {
        if (mode == 1) {
            if (record->state == 1) {
                record->state = 2;
            } else {
                record->refCount++;
            }
        }
        return record->buffer;
    }
    return NULL;
}

/* Returns the bank fetch callback, initialising the bank records on first use. */
BankFetchProc audio_bankFetchNew(void) {
    if (!BANK_STATE.initialized) {
        BANK_STATE.loadedList = NULL;
        BANK_STATE.freeList = gBankRecords;
        BANK_STATE.pendingList = NULL;
        BANK_STATE.releaseList = NULL;
        BANK_STATE.initialized = TRUE;
    }
    return audio_fetchBank;
}

/*
 * Moves banks whose DMA has finished to the loaded list (relocating instrument
 * sound pointers), then frees the buffers of released banks.
 */
void audio_updateBanks(void) {
    u32 i;
    u32 sound;
    s32 found;
    s32 received;
    OSIoMesg *message;
    BankRecord *record;
    BankRecord *next;
    AudioInstrument *instrument;
    AudioWaveState *wave;
    s32 busy;

    received = 0;
    message = NULL;
    for (i = 0; i < gBankDmaCount; i++) {
        if (osRecvMesg(&gBankDmaQueue, (OSMesg *)&message, OS_MESG_NOBLOCK) != -1) {
            record = BANK_STATE.pendingList;
            received++;
            found = FALSE;
            while (record != NULL && !found) {
                if (record->buffer == message->dramAddr) {
                    if (record == BANK_STATE.pendingList) {
                        BANK_STATE.pendingList = record->next;
                    }
                    LIST_UNLINK(BankRecord, record);
                    record->next = NULL;
                    record->prev = NULL;
                    if (BANK_STATE.loadedList != NULL) {
                        LIST_LINK(BankRecord, record, BANK_STATE.loadedList);
                    } else {
                        BANK_STATE.loadedList = record;
                        record->next = NULL;
                        record->prev = NULL;
                    }
                    if (record->mode == 1) {
                        instrument = record->buffer;
                        for (sound = 0; sound < instrument->soundCount; sound++) {
                            instrument->sounds[sound] =
                                (AudioBankSound *)((u32)instrument->sounds[sound] + (u32)record->buffer);
                        }
                    }
                    record->state = 1;
                    record->refCount++;
                    found = TRUE;
                } else {
                    record = record->next;
                }
            }
        }
    }
    gBankDmaCount -= received;

    record = BANK_STATE.releaseList;
    if (record != NULL) {
        do {
            busy = FALSE;
            next = record->next;
            if (record->mode == 1) {
                AudioInstrument *instrument = record->buffer;

                for (sound = 0; sound < instrument->soundCount; sound++) {
                    wave = instrument->sounds[sound]->wave;
                    if (wave->fieldA != 0) {
                        wave->fieldA = 0;
                        busy = TRUE;
                    }
                }
            }
            if (!busy) {
                record->refCount = 0;
                record->state = 0;
                heap_free(record->buffer);
                record->ownerSlot = NULL;
                if (record == BANK_STATE.releaseList) {
                    BANK_STATE.releaseList = next;
                }
                LIST_UNLINK(BankRecord, record);
                if (BANK_STATE.freeList != NULL) {
                    BankRecord *ln = record;
                    BankRecord *to = BANK_STATE.freeList;

                    ln->next = to->next;
                    ln->prev = to;
                    if (to->next != NULL) {
                        to->next->prev = record;
                    }
                    to->next = record;
                    record = next;
                    continue; // matching: IDO gives the other branch its own copy of the loop test
                } else {
                    BANK_STATE.freeList = record;
                    record->next = NULL;
                    record->prev = NULL;
                }
            }
            record = next;
        } while (next != NULL);
    }
    if (gReleaseRawBanksPending != 0) {
        audio_releaseRawBanks();
        gReleaseRawBanksPending = 0;
    }
}

/* Moves every unreferenced raw bank from the loaded list to the release list. */
void audio_releaseRawBanks(void) {
    BankRecord *record;
    BankRecord *next;
    u32 *ownerSlot;

    record = BANK_STATE.loadedList;
    if (record != NULL) {
        do {
            next = record->next;
            if (record->refCount == 0 && record->mode == 0) {
                ownerSlot = record->ownerSlot;
                *ownerSlot = record->romRef;
                record->ownerSlot = NULL;
                if (record == BANK_STATE.loadedList) {
                    BANK_STATE.loadedList = next;
                }
                LIST_UNLINK(BankRecord, record);
                if (BANK_STATE.releaseList != NULL) {
                    LIST_LINK(BankRecord, record, BANK_STATE.releaseList);
                } else {
                    BANK_STATE.releaseList = record;
                    record->next = NULL;
                    record->prev = NULL;
                }
            }
            record = next;
        } while (next != NULL);
    }
}
