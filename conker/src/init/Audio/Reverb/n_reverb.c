#include <ultra64.h>

#include "n_libaudio.h"
#include "variables.h"
#include "n_abi.h"
#include "libultra/audio/n_synthInternals.h"

/* Conker's variant of the libultra reverb/chorus effect (n_reverb.c): the FX pull,
 * its parameter handler, and the delay-line load/save/filter/modulation helpers.
 *
 * This TU deliberately does NOT include "functions.h": functions.h and n_libaudio.h
 * give n_alInit/n_alClose incompatible prototypes, so they cannot both be included.
 *
 * D_800428C4[bus] is the bus's stereo-routing flag (0: one delay line, 1: a second
 * line for the other channel); D_800428C6[bus] inverts the feed coefficients and
 * picks the alternative output buffer when routing is on.
 */

/* The chorus resampler attached to a delay section; one state per bus. */
typedef struct {
    ALFilter        filter;   /* 0x00 */
    RESAMPLE_STATE *state[2]; /* 0x14 */
    f32             ratio;    /* 0x1C */
    s32             upitch;   /* 0x20 */
    f32             delta;    /* 0x24 */
    s32             first;    /* 0x28 */
} Resampler;

/* One delay section (ALDelay): taps `input`/`output` samples behind the line's write
 * head, feed-forward/feedback/output gains, and the chorus LFO (rsinc, rsval, rsgain)
 * that modulates the output tap through the resampler. */
typedef struct {
    u32        input;   /* 0x00 */
    u32        output;  /* 0x04 */
    s16        ffcoef;  /* 0x08 */
    s16        fbcoef;  /* 0x0A */
    s16        gain;    /* 0x0C */
    f32        rsinc;   /* 0x10 */
    f32        rsval;   /* 0x14 */
    s32        rsdelta; /* 0x18 */
    f32        rsgain;  /* 0x1C */
    LowPass   *lp;      /* 0x20 */
    Resampler *rs;      /* 0x24 */
} Delay;                /* 0x28 */

/* The effect (ALFx): `length` samples of circular delay line per bus, starting at
 * base[bus], with the write head at input[bus]. */
typedef struct {
    u32    length;         /* 0x00 */
    Delay *delay;          /* 0x04 */
    u8     section_count;  /* 0x08 */
    u8     pad9[0x17];
    s16   *base[2];        /* 0x20 */
    s16   *input[2];       /* 0x28 */
} Fx;

/* The aux bus as the ROM lays it out: stride 0x44 (n_alSynNew allocates it that way)
 * with the effect pointer at 0x1C.  n_synthInternals.h's N_ALAuxBus puts `fx` at 0x20,
 * so this file indexes n_syn->auxBus through this view instead. */
typedef struct {
    u8  pad0[0x1C];
    Fx *primaryFx;  /* 0x1C */
    u8  pad20[0x24];
} FxBus;            /* 0x44 */

Acmd *func_100210C0(s32 sampleOffset, Acmd *p, s32 bus, s32 *cmdCount);
Acmd *_n_loadOutputBuffer(Fx *fx, Delay *d, s32 bus, s32 buff, Acmd *p);
Acmd *_n_loadBuffer(Fx *fx, s32 bus, s16 *curr, s32 buff, s32 count, Acmd *p);
Acmd *_n_saveBuffer(Fx *fx, s32 bus, s16 *curr, s32 buff, Acmd *p);
Acmd *_n_filterBuffer(LowPass *lp, s32 bus, s32 buff, Acmd *p);
f32 _doModFunc(Delay *d, s32 count);

/* n_alFxPull: pull the sources feeding aux bus `bus`, write the mix into the delay
 * line(s), then run every delay section (feed-forward, feedback, low-pass, chorus) and
 * accumulate its output gain into the aux output buffer.  Address-taken: it is the
 * aux bus filter handler. */
Acmd *func_1001E530(s32 sampleOffset, Acmd *p, s32 bus) {
    Acmd  *ptr = p;
    Fx    *fx = ((FxBus *)n_syn->auxBus)[bus].primaryFx;
    s16    i;
    s16    buff1;
    s16    buff2;
    s16    input;
    s16    output;
    s16   *inPtr;
    s16   *outPtr;
    s16   *prevOutPtr = NULL;
    Delay *d;
    s32    cmdCount = 0;
    u32    j;

    ptr = func_100210C0(sampleOffset, p, bus, &cmdCount);
    input = N_AL_AUX_L_OUT;
    output = N_AL_AUX_R_OUT;
    buff1 = N_AL_TEMP_0;
    buff2 = N_AL_TEMP_1;
    if (D_800428C4[bus] == 0) {
        aMix(ptr++, 0, 0xC000, N_AL_AUX_L_OUT, input);
        aMix(ptr++, 0, 0x4000, N_AL_AUX_R_OUT, input);
    }
    /* write the mixed input into the delay line at its write head */
    ptr = _n_saveBuffer(fx, 0, fx->input[0], input, ptr);
    if (D_800428C4[bus] != 0) {
        ptr = _n_saveBuffer(fx, 1, fx->input[1], N_AL_AUX_R_OUT, ptr);
    }
    for (j = 0; j <= D_800428C4[bus]; j++) {
        aClearBuffer(ptr++, output, SAMPLES * 2);
        for (i = 0; i < fx->section_count; i++) {
            d = &fx->delay[i];
            inPtr = &fx->input[j][-d->input];
            outPtr = &fx->input[j][-d->output];
            if (D_800428C6[bus] && D_800428C4[bus]) {
                d->ffcoef = -d->ffcoef;
                d->fbcoef = -d->fbcoef;
            }
            if (inPtr == prevOutPtr) {
                s16 tmp = buff2;
                buff2 = buff1;
                buff1 = tmp;
            } else {
                ptr = _n_loadBuffer(fx, j, inPtr, buff1, SAMPLES, ptr);
            }
            ptr = _n_loadOutputBuffer(fx, d, j, buff2, ptr);
            if (d->ffcoef != 0) {
                aMix(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
                if (d->rs == NULL && d->lp == NULL) {
                    ptr = _n_saveBuffer(fx, j, outPtr, buff2, ptr);
                }
            }
            if (d->fbcoef != 0) {
                aMix(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
                ptr = _n_saveBuffer(fx, j, inPtr, buff1, ptr);
            }
            if (d->lp != NULL) {
                ptr = _n_filterBuffer(d->lp, j, buff2, ptr);
            }
            if (d->rs == NULL) {
                ptr = _n_saveBuffer(fx, j, outPtr, buff2, ptr);
            }
            if (d->gain != 0) {
                if (D_800428C4[bus] != 0) {
                    aMix(ptr++, 0, (u16)d->gain, buff2, output);
                } else {
                    u32 gain = d->gain * 1.4142f;

                    if (gain >= 0x8000) {
                        gain = 0x7FFF;
                    }
                    aMix(ptr++, 0, (u16)gain, buff2, output);
                }
            }
            prevOutPtr = &fx->input[j][d->output];
        }
        if (D_800428C4[bus] != 0 && j == 0) {
            ptr = _n_loadBuffer(fx, 1, fx->input[1], input, SAMPLES, ptr);
            if (D_800428C6[bus]) {
                aMix(ptr++, 0, 0x5A82, output, N_AL_MAIN_R_OUT);
            } else {
                aMix(ptr++, 0, 0x5A82, output, N_AL_MAIN_L_OUT);
            }
        } else {
            // matching: IDO -g emits the extra branch at this merge only with an else
        }
        aDMEMMove(ptr++, output, N_AL_AUX_L_OUT, SAMPLES * 2);
        /* advance the write head, modulo the line length */
        fx->input[j] += SAMPLES;
        if (fx->input[j] > &fx->base[j][fx->length]) {
            fx->input[j] -= fx->length;
        }
    }
    return ptr;
}

/* n_alFxParamHdl, called by n_alSynSetFXParam.  `paramID` packs the parameter in its
 * low 3 bits and the delay section in the rest.  Tap positions are converted from
 * milliseconds to 8-sample-aligned offsets and clamped to the line, and the chorus
 * depth is re-derived for the new tap spacing.
 *
 * `val` is declared `int`, not `s32` (`long` here): the conversion this forces in
 * `val * n_syn->outputRate` puts the operands of the multu in the original order. */
s32 n_alFxParamHdl(void *filter, s32 paramID, void *param) {
    Fx *f = filter;
    s32 sel = paramID & 7;
    s32 s = paramID >> 3;
    int val = *(s32 *)param;
    f32 depth;

    if (s >= f->section_count) {
        return 0;
    }
    switch (sel) {
    case 0: /* input tap, ms */
        f->delay[s].input = val * n_syn->outputRate / 1000 & ~7;
        break;
    case 1: /* output tap, ms */
        f->delay[s].output = val * n_syn->outputRate / 1000 & ~7;
        break;
    case 2:
        f->delay[s].fbcoef = val;
        break;
    case 3:
        f->delay[s].ffcoef = val;
        break;
    case 4:
        f->delay[s].gain = val;
        break;
    case 5: /* chorus rate */
        f->delay[s].rsinc = val / 1000.0f * 2.0f / n_syn->outputRate;
        break;
    case 6: /* chorus depth */
        depth = val;
        break;
    case 7: /* low-pass cutoff */
        if (f->delay[s].lp != NULL) {
            f->delay[s].lp->fc = val;
            init_lpfilter(f->delay[s].lp);
        }
        break;
    }
    if (f->delay[s].input >= f->length - 16) {
        f->delay[s].input = f->length - 16;
    }
    if (f->delay[s].input >= f->length - 8) {
        f->delay[s].input = f->length - 8;
    }
    if (f->delay[s].input >= f->delay[s].output) {
        f->delay[s].output = f->delay[s].input + 8;
    }
    /* 173123.40 = 120000 / ln(2): hundredths of a cent to ln(ratio) */
    if (f->delay[s].rs != NULL) {
        if (sel != 6) {
            if (f->delay[s].output - f->delay[s].input != 0) {
                depth = f->delay[s].rsgain / (f->delay[s].output - f->delay[s].input) *
                        173123.4062f;
            } else {
                depth = 0.0f;
            }
        }
        f->delay[s].rsgain = (f->delay[s].output - f->delay[s].input) * (depth / 173123.4062f);
    }
    return 0;
}

/* _n_loadOutputBuffer: load a section's output tap into DMEM `buff`.  With a chorus
 * resampler, the tap is modulated by the LFO and resampled: read the (8-byte aligned)
 * source run into DMEM N_AL_TEMP_2 and A_RESAMPLE it into `buff`. */
Acmd *_n_loadOutputBuffer(Fx *fx, Delay *d, s32 bus, s32 buff, Acmd *p) {
    Acmd *ptr = p;
    s32   ratio;
    s32   count;
    s32   rbuff = N_AL_TEMP_2;
    s16  *outPtr;
    f32   fincount;
    f32   fratio;
    f32   delta;
    s32   ramalign = 0;
    s32   length;
    s32   incount = SAMPLES;
    s16   seg;

    if (d->rs != NULL) {
        length = d->output - d->input;
        delta = _doModFunc(d, incount);
        delta = delta / length;
        delta = (s32)(delta * 32768.0f);
        delta = delta / 32768.0f;
        fratio = 1.0f - delta;
        fincount = d->rs->delta + fratio * incount;
        count = (s32)fincount;
        d->rs->delta = fincount - count;
        outPtr = &fx->input[bus][-(d->output - d->rsdelta)];
        ramalign = ((s32)outPtr & 7) >> 1;
        ptr = _n_loadBuffer(fx, bus, outPtr - ramalign, rbuff, count + ramalign, ptr);
        ratio = fratio * 32768.0f;
        seg = buff >> 8;
        n_aResample(ptr++, osVirtualToPhysical(d->rs->state[bus]), d->rs->first, ratio,
                    rbuff + ramalign * 2, seg);
        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
        outPtr = &fx->input[bus][-d->output];
        ptr = _n_loadBuffer(fx, bus, outPtr, buff, SAMPLES, ptr);
    }
    return ptr;
}

/* _n_loadBuffer: A_LOADBUFF `count` samples of bus `bus`'s delay line, starting at
 * `curr`, into DMEM `buff`, splitting the DMA where it wraps past the end. */
Acmd *_n_loadBuffer(Fx *fx, s32 bus, s16 *curr, s32 buff, s32 count, Acmd *p) {
    Acmd *ptr = p;
    s32   afterEnd;
    s32   beforeEnd;
    s16  *end;
    s16  *lineEnd;

    lineEnd = &fx->base[bus][fx->length];
    if (curr < fx->base[bus]) {
        curr += fx->length;
    }
    end = curr + count;
    if (end > lineEnd) {
        afterEnd = end - lineEnd;
        beforeEnd = lineEnd - curr;
        n_aLoadBuffer(ptr++, beforeEnd * 2, buff, osVirtualToPhysical(curr));
        n_aLoadBuffer(ptr++, afterEnd * 2, buff + beforeEnd * 2,
                      osVirtualToPhysical(fx->base[bus]));
    } else {
        n_aLoadBuffer(ptr++, count * 2, buff, osVirtualToPhysical(curr));
    }
    return ptr;
}

/* _n_saveBuffer: the A_SAVEBUFF mirror of _n_loadBuffer, always one frame long. */
Acmd *_n_saveBuffer(Fx *fx, s32 bus, s16 *curr, s32 buff, Acmd *p) {
    Acmd *ptr = p;
    s32   afterEnd;
    s32   beforeEnd;
    s16  *end;
    s16  *lineEnd;

    lineEnd = &fx->base[bus][fx->length];
    if (curr < fx->base[bus]) {
        curr += fx->length;
    }
    end = curr + SAMPLES;
    if (end > lineEnd) {
        afterEnd = end - lineEnd;
        beforeEnd = lineEnd - curr;
        n_aSaveBuffer(ptr++, beforeEnd * 2, buff, osVirtualToPhysical(curr));
        n_aSaveBuffer(ptr++, afterEnd * 2, buff + beforeEnd * 2,
                      osVirtualToPhysical(fx->base[bus]));
    } else {
        n_aSaveBuffer(ptr++, SAMPLES * 2, buff, osVirtualToPhysical(curr));
    }
    return ptr;
}

/* _n_filterBuffer: load the low-pass coefficients and run A_POLEF over DMEM `buff`
 * (only its bits 8..15 reach the command). */
Acmd *_n_filterBuffer(LowPass *lp, s32 bus, s32 buff, Acmd *p) {
    Acmd *ptr = p;
    s16   seg = buff >> 8;

    n_aLoadADPCM(ptr++, 32, osVirtualToPhysical(lp->fcvec.fccoef));
    n_aPoleFilter(ptr++, lp->first, lp->fgain, seg, osVirtualToPhysical(lp->fstate[bus]));
    lp->first = 0;
    return ptr;
}

/* _doModFunc: advance the chorus LFO by `count` samples and return how many samples
 * the output tap should move back: a triangle wave from -1 to 1 scaled by rsgain. */
f32 _doModFunc(Delay *d, s32 count) {
    f32 val;

    /* bipolar sawtooth from -2 to 2 */
    d->rsval += d->rsinc * count;
    d->rsval = (d->rsval > 2.0f) ? d->rsval - 4.0f : d->rsval;

    /* fold to a triangle from 0 to 2, then centre it on 0 */
    val = d->rsval;
    val = val < 0.0f ? -val : val;
    val = val - 1.0f;

    return val * d->rsgain;
}
