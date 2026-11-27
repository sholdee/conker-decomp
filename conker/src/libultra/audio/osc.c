#include <ultra64.h>
#include <n_libaudio.h>

/* Oscillator state of the sequence players' tremolo / vibrato (initOsc, updateOsc, stopOsc),
 * the libultra demo's oscData; free ones are linked from gFreeOscStates. */
typedef struct OscState {
    /* 0x00 */ struct OscState *next;
    /* 0x04 */ u8  type;      // 1 = tremolo sine, 128 = vibrato sine
    /* 0x05 */ u8  pad5[0x1D];
    /* 0x22 */ u16 maxCount;  // period in updates
    /* 0x24 */ u16 curCount;
    /* 0x26 */ u8  pad26[2];
    /* 0x28 */ union {
        struct {
            u8 halfdepth;
            u8 baseVol;
        } tsin;
        struct {
            f32 depth;    // cents ratio (_depth2Cents)
        } vsin;
    } data;
} OscState; // size 0x2C

/* This file's .bss. */
static OscState *gFreeOscStates; /* free oscillator states */
static OscState *D_80042804;     /* the oscillator states (func_100131FC) */

f32 _depth2Cents(u8 arg0) {
    f32 sp4 = 1.0309929847717285f;
    f32 sp0 = 1.0f;

    while (arg0)
    {
        if (arg0 & 1) {
            sp0 *= sp4;
        }
        sp4 *= sp4;
        arg0 >>= 1;
    }
    return sp0;
}

s32 initOsc(void **arg0, f32 *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
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
                sp2C->data.vsin.depth = _depth2Cents(arg4);
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

s32 updateOsc(OscState *arg0, f32 *arg1) {
    f32 sp2C;
    OscState *sp28;
    s32 ret;

    sp28 = arg0;
    ret = 16000;

    switch (sp28->type)
    {
        case 1: // guess this means union is u8s
            sp28->curCount = sp28->curCount + 1;
            if (sp28->curCount >= sp28->maxCount) {
                sp28->curCount = 0;
            }
            sp2C = (f32)sp28->curCount / (f32)sp28->maxCount;
            sp2C = sinf(sp2C * 6.2831854820251465f); // 2 * PI
            sp2C = sp28->data.tsin.halfdepth * sp2C;
            *arg1 = sp28->data.tsin.baseVol + sp2C;
            break;

        case 128: // guess this means float?
            sp28->curCount = sp28->curCount + 1;
            if (sp28->curCount >= sp28->maxCount) {
                sp28->curCount = 0;
            }
            sp2C = (f32)sp28->curCount / (f32)sp28->maxCount;
            sp2C = sinf(sp2C * 6.2831854820251465f) * sp28->data.vsin.depth;
            *arg1 = alCents2Ratio(sp2C);
            break;
        default:
            break;
    }

    return ret;
}

void stopOsc(OscState *arg0) {
    arg0->next = gFreeOscStates;
    gFreeOscStates = arg0;
}

/* initOsc setup: allocates `arg1` oscillator states into the free list and installs the oscillator callbacks. */
void func_100131FC(ALSeqpConfig *config, s32 arg1) {
    OscState *sp24;
    s32 i;

    D_80042804 = alHeapDBAlloc(0, 0, config->heap, arg1, sizeof(OscState));
    gFreeOscStates = D_80042804;
    gFreeOscStates = D_80042804; // matching: the ROM stores it twice
    sp24 = D_80042804;

    for (i = 0; i < (arg1 - 1); i++) {
        sp24->next = &D_80042804[i + 1];
        sp24 = sp24->next;
    }
    sp24->next = NULL;
    config->initOsc = initOsc;
    config->updateOsc = updateOsc;
    config->stopOsc = stopOsc;
}
