#include <ultra64.h>
#include <n_libaudio.h>
#include <os_internal.h>
#include <ultraerror.h>

/* Oscillator state of the sequence players' tremolo / vibrato (init_128D0.c: func_10012E04 init,
 * func_10012F94 update, func_100131D8 stop), the libultra demo's oscData; free ones are linked from gFreeOscStates. */
typedef struct OscState {
    /* 0x00 */ struct OscState *next;
    /* 0x04 */ u8  type;      // 1 = tremolo sine, 128 = vibrato sine
    /* 0x05 */ u8  pad5[0x1D];
    /* 0x22 */ s16 maxCount;  // period in updates
    /* 0x24 */ s16 curCount;
    /* 0x26 */ u8  pad26[2];
    /* 0x28 */ union {
        struct {
            u8 halfdepth;
            u8 baseVol;
        } tsin;
        struct {
            f32 depth;    // cents ratio (func_10012D80)
        } vsin;
    } data;
} OscState; // size 0x2C

/* Storage in the init .bss that no file defines. */
extern OscState *gFreeOscStates; /* free oscillator states */
extern s32  D_80042804;

void alSeqFileNew(ALSeqFile *arg0, u8 *base) {
    s32 offset = base;
    s32 i;

    for (i = 0; i < arg0->seqCount; i++)
    {
        arg0->seqArray[i].offset += offset;
    }
}

// non-vanila alBnkfNew
void func_10012934(ALBankFile *file, u8 *table, s32 arg2) {
    s32 offset = file;
    s32 woffset = table;
    s32 sp1C = arg2;

    s32 i;

    ALFailIf(file->revision != AL_BANK_VERSION, ERR_ALBNKFNEW);

    for (i = 0; i < file->bankCount; i++) {
        file->bankArray[i] = (ALBank *)((u8 *)file->bankArray[i] + offset) ; // (s32)sp24; // ??
        if (file->bankArray[i] != 0) {
            func_10012A28(file->bankArray[i], offset, woffset, sp1C);
        }
    }
}

/* Conker's _bnkfPatchBank: relocates the percussion and instrument 0 by the bank file offset, the other
 * instruments by the wave table offset. */
void func_10012A28(ALBank *bank, s32 offset, s32 table, s32 arg3) {
    s32 i;
    s32 tmp;

    if (bank->flags != 0) {
        return;
    }

    bank->flags = 1;

    if (bank->percussion != NULL) {
        bank->percussion = (ALInstrument *)((u8 *)bank->percussion + offset);
        func_10012B84(bank->percussion, arg3);
    }

    for (i = 0; i < bank->instCount; i ++) {
        if (bank->instArray[i] != NULL) {
            if (i == 0) {
                bank->instArray[i] = (ALInstrument *)((u8 *)bank->instArray[i] + offset);
                func_10012BD0(bank->instArray[i], arg3);
            } else {
                tmp = (arg3 & -8) << 5;
                bank->instArray[i] = (ALInstrument *)((u8 *)bank->instArray[i] + tmp);
            }
        }
    }
}

/* Relocates the first sound of a percussion instrument. */
void func_10012B84(ALInstrument *inst, s32 arg1) {
    if (inst->flags) {
        return;
    }
    inst->flags = 1;
    arg1 = (0xFFFFFFF8 & arg1) << 5;
    inst->soundArray[0] = (ALSound *)((u8 *)inst->soundArray[0] + arg1);
}

/* Relocates every sound of an instrument. */
void func_10012BD0(ALInstrument *inst, s32 arg1) {
    s32 i;

    if (inst->flags != 0) {
        return;
    }
    inst->flags = 1;

    arg1 = (arg1 & -8) << 5;

    for (i = 0; i < inst->soundCount; i++)
    {
        inst->soundArray[i] = (ALSound *)((u8 *)inst->soundArray[i] + arg1);
    }
}

/* _bnkfPatchSound: relocates a sound's envelope, key map and wave table, then the wave table itself. */
void func_10012C5C(ALSound *sound, s32 offset, s32 table) {
    if (sound->flags) {
        return;
    }
    sound->flags = 1;
    sound->envelope = (ALEnvelope *)((u8 *)sound->envelope + offset);
    sound->keyMap = (ALKeyMap *)((u8 *)sound->keyMap + offset);
    sound->wavetable = (ALWaveTable *)((u8 *)sound->wavetable + offset);
    func_10012CFC((WaveTable *)sound->wavetable, offset, table);
}

/* _bnkfPatchWaveTable: relocates the wave data by `table` and the loop and ADPCM book by `offset`. */
void func_10012CFC(WaveTable *wave, s32 offset, s32 table) {
    wave->flags = 1;
    wave->base += table;
    if (wave->type == AL_ADPCM_WAVE) {
        wave->book = (ALADPCMBook *)((u8 *)wave->book + offset);
        if (wave->loop != NULL) {
            wave->loop = (u8 *)wave->loop + offset;
        }
        wave->unk14 = 0;
    } else if ((wave->type == AL_RAW16_WAVE) && (wave->loop != NULL)) {
        wave->loop = (u8 *)wave->loop + offset;
    }
}

f32 func_10012D80(u8 arg0) {
    f32 sp4 = 1.0309929847717285f;
    f32 sp0 = 1.0f;

    while (arg0)
    {
        if (arg0 & 1) {
            sp0 *= sp4;
        }
        sp4 *= sp4;
        arg0 = (u32)arg0 >> 1;
    }
    return sp0;
}

s32 func_10012E04(s32 *arg0, f32 *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
    OscState *sp2C;
    s32 ret;

    ret = 0;
    if (arg5 == 0) {
        return 0;
    }
    if (gFreeOscStates != 0) {
        sp2C = gFreeOscStates;
        gFreeOscStates = gFreeOscStates->next;
        sp2C->type = arg2;
        *arg0 = sp2C;
        ret = arg5 << 0xE;
        switch (arg2) {
            case 1:
                sp2C->curCount = 0;
                sp2C->maxCount = 0x103 - arg3;
                sp2C->data.tsin.halfdepth = arg4 >> 1;
                sp2C->data.tsin.baseVol = 0x7F - sp2C->data.tsin.halfdepth;
                *arg1 = sp2C->data.tsin.baseVol;
                break;
            case 128:
                sp2C->data.vsin.depth = func_10012D80(arg4);
                sp2C->curCount = 0;
                sp2C->maxCount = 0x103 - arg3;
                *arg1 = 1.0f;
                break;
            default:
                break;
        }
    }
    return ret;
}

s32 func_10012F94(OscState *arg0, f32 *arg1) {
    f32 sp2C;
    OscState *sp28;
    s32 ret;

    sp28 = arg0;
    ret = 16000;

    switch (sp28->type)
    {
        case 1: // guess this means union is u8s
            sp28->curCount = (u16)sp28->curCount + 1;
            if ((u16)sp28->curCount >= (u16)sp28->maxCount) {
                sp28->curCount = 0;
            }
            sp2C = (f32)(u16)sp28->curCount / (f32) (u16)sp28->maxCount;
            sp2C = sinf(sp2C * 6.2831854820251465f); // 2 * PI
            sp2C = sp28->data.tsin.halfdepth * sp2C;
            *arg1 = sp28->data.tsin.baseVol + sp2C;
            break;

        case 128: // guess this means float?
            sp28->curCount = (u16)sp28->curCount + 1;
            if ((u16)sp28->curCount >= (u16)sp28->maxCount) {
                sp28->curCount = 0;
            }
            sp2C = (f32)(u16) sp28->curCount / (f32)(u16) sp28->maxCount;
            sp2C = sinf(sp2C * 6.2831854820251465f) * sp28->data.vsin.depth;
            *arg1 = alCents2Ratio(sp2C);
            break;
        default:
            break;
    }

    return ret;
}

void func_100131D8(s32 *arg0) {
    *arg0 = (s32*)gFreeOscStates;
    gFreeOscStates = arg0;
}

/* initOsc setup: allocates `arg1` oscillator states into the free list and installs the oscillator callbacks. */
void func_100131FC(ALSeqpConfig *config, s32 arg1) {
    s32 *sp24;
    s32 i;

    D_80042804 = alHeapDBAlloc(0, 0, config->heap, arg1, sizeof(OscState));
    gFreeOscStates = (s32) D_80042804;
    gFreeOscStates = (s32) D_80042804; // ???
    sp24 = D_80042804;

    for (i = 0; i < (arg1 - 1); i++) {
        *sp24 = (i * 0x2C) + D_80042804 + 0x2C; // is this an array?
        sp24 = *sp24;
    }
    *sp24 = 0;
    config->initOsc = func_10012E04;
    config->updateOsc = func_10012F94;
    config->stopOsc = func_100131D8;
}
