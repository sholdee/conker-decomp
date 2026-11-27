/* n_synthInternals.h declares n_alFxNew over ALFx; this file defines it over its own N_ALFx. */
#define n_alFxNew n_alFxNew_hdr_decl
#include "n_synthInternals.h"
#undef n_alFxNew

void func_1001CBF0(f32 arg0, f32 arg1, f32 arg2, f32 arg3[3], f32 arg4[3]) {
    f32 sp24;
    f32 sp20;
    f32 sp1C;

    if (arg1 >= (arg0 - 200.0f)) {
        arg1 = arg0 - 200.0f;
    }

    sp24 = math_atan2(arg1 * 3.1415927f, arg0);
    sp20 = sp24 * sp24;
    sp1C = (sp24 * 1.4142137f) / arg2;
    arg3[0] = sp20 / (1.0f + sp20 + sp1C);
    arg3[1] = arg3[0] * 2.0f;
    arg3[2] = arg3[0];
    arg4[1] = ((sp20 - 1.0f) * 2.0f) / (1.0f + sp20 + sp1C);
    arg4[2] = ((1.0f + sp20) - sp1C) / (1.0f + sp20 + sp1C);
}

void init_lpfilter(LowPass *arg0) {
    s32 i;
    s32 sp10;
    s16 spE;
    f32 temp_f8;
    f32 sp4;
    f32 sp0;

    sp10 = arg0->fc * 16384.0f;
    spE = sp10 >> 15;
    arg0->fgain = 16384.0f - spE;
    arg0->first = 0;

    for (i = 0; i < 8; i++)
    {
        arg0->fcvec.fccoef[i] = 0;
    }
    arg0->fcvec.fccoef[i++] = spE;

    sp0 = 16384.0f;
    sp4 = temp_f8 = spE / sp0;

    for (;i < 16; i++)
    {
        sp4 *= temp_f8;
        arg0->fcvec.fccoef[i] = sp4 * sp0;
    }
}

f32 func_1001CEA4(s32 arg0) {
    f32 sp4;
    f32 sp0 = 1.0f;

    if (arg0 >= 0) {
        sp4 = 1.0594631f; /* 2^(1/12): one semitone up */
    } else {
        sp4 = 0.9438743f; /* one semitone down */
        arg0 = -arg0;
    }

    while (arg0 != 0) {
        if ((arg0 & 1) != 0) {
            sp0 = sp0 * sp4;
        }
        sp4 *= sp4;
        arg0 = arg0 >> 1;
    };

    return sp0;
}

void func_1001CF38(LowPass *arg0, f32 arg1) {
    s32 i;
    f32 sp30[3];
    f32 sp24[3];

    if (arg0->fgain == 0) {
        return;
    }
    if (arg0->fgain < 10) {
        arg0->fgain = 10;
    }
    func_1001CBF0(arg1,  arg0->fc + 10.0f, arg0->fgain / 10.0f, sp30, sp24);

    for(i = 3; i < 8; i++) {
        arg0->fcvec.fccoef[i] = 0;
    }
    arg0->fcvec.fccoef[0] = sp30[0] * (26768.0f - (arg0->fgain * 128.0f));
    arg0->fcvec.fccoef[1] = sp30[1] * (26768.0f - (arg0->fgain * 128.0f));
    arg0->fcvec.fccoef[2] = 0;

    arg0->fcvec.fccoef[8] = sp24[1] * -16384.0f;
    arg0->fcvec.fccoef[9] = sp24[2] * -16384.0f;

    for (i = 10; i < 16; i++) {
        arg0->fcvec.fccoef[i] = 0;
    }
}

/* Conker's n_audio reverb layout differs from synthInternals.h: each delay stage keeps two
 * resampler / low-pass states, and the effect keeps two delay lines. */
typedef struct {
    u8 pad0[0x14];
    RESAMPLE_STATE *state;     /* 0x14 */
    RESAMPLE_STATE *state2;    /* 0x18 */
    u8 pad1C[0x8];
    f32 delta;                 /* 0x24 */
    s32 first;                 /* 0x28 */
    u8 pad2C[0xC];
} N_ALResampler; /* 0x38 */

typedef struct {
    u32 input;
    u32 output;
    s16 ffcoef;
    s16 fbcoef;
    s16 gain;
    f32 rsinc;
    f32 rsval;
    s32 rsdelta;
    f32 rsgain;
    LowPass *lp;
    N_ALResampler *rs;
} N_ALDelay; /* 0x28 */

typedef struct {
    u32 length;                /* 0x00 */
    N_ALDelay *delay;          /* 0x04 */
    u8 section_count;          /* 0x08 */
    u8 pad9[0x17];
    s16 *base;                 /* 0x20 */
    s16 *base2;                /* 0x24 */
    s16 *input;                /* 0x28 */
    s16 *output;               /* 0x2C */
} N_ALFx; /* 0x30 */

/* The default effect parameters (libultra's NULL_PARAMS): no delay sections. */
static s32 D_8002BBE0[10] = {
    0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

#define RANGE 2.0f

void n_alFxNew(N_ALFx **fx_ar, ALSynConfig *c, s16 bus, ALHeap *hp) {
    u16 i, j, k;
    s32 *param = 0;
    N_ALDelay *d;
    N_ALFx *r;

    r = alHeapAlloc(hp, 1, sizeof(N_ALFx));
    *fx_ar = r;

    switch (c->fxType[bus]) {
        case AL_FX_CUSTOM:
            param = c->params[bus];
            break;
        default:
            param = D_8002BBE0;
            break;
    }

    j = 0;

    r->section_count = param[j++];
    r->length = param[j++];

    r->delay = alHeapAlloc(hp, r->section_count, sizeof(N_ALDelay));
    r->base = alHeapAlloc(hp, r->length, sizeof(s16));
    r->input = r->base;
    r->base2 = alHeapAlloc(hp, r->length, sizeof(s16));
    r->output = r->base2;

    for (k = 0; k < r->length; k++) {
        r->base2[k] = 0;
        r->base[k] = r->base2[k];
    }

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = param[j++];
        d->output = param[j++];
        d->fbcoef = param[j++];
        d->ffcoef = param[j++];
        d->gain = param[j++];

        if (param[j]) {
            d->rsinc = ((((f32)param[j++]) / 1000) * RANGE) / c->outputRate;
            d->rsgain = (((f32)param[j++]) / 173123.40625f) * (d->output - d->input);
            d->rsval = 1.0;
            d->rsdelta = 0.0;
            d->rs = alHeapAlloc(hp, 1, sizeof(N_ALResampler));
            d->rs->state = alHeapAlloc(hp, 1, sizeof(RESAMPLE_STATE));
            d->rs->state2 = alHeapAlloc(hp, 1, sizeof(RESAMPLE_STATE));
            d->rs->delta = 0.0;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }

        if (param[j]) {
            d->lp = alHeapAlloc(hp, 1, sizeof(LowPass));
            d->lp->fstate[0] = alHeapAlloc(hp, 1, sizeof(POLEF_STATE));
            d->lp->fstate[1] = alHeapAlloc(hp, 1, sizeof(POLEF_STATE));
            d->lp->fc = param[j++];
            init_lpfilter(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}


void alN_PVoiceNew(N_PVoice *mv, ALDMANew dmaNew, ALHeap *hp) {
    mv->dc_state = alHeapDBAlloc(0, 0, hp, 1, sizeof(ADPCM_STATE));
    mv->dc_lstate = alHeapDBAlloc(0, 0, hp, 1, sizeof(ADPCM_STATE));
    mv->dc_dma = dmaNew(&mv->dc_dmaState);
    mv->dc_lastsam = 0;
    mv->dc_first = 1;
    mv->dc_memin = 0;

    mv->rs_state = alHeapDBAlloc(0, 0, hp, 1, sizeof(RESAMPLE_STATE));
    mv->rs_delta  = 0.0;
    mv->rs_first  = 1;
    mv->rs_ratio = 1.0;
    mv->rs_upitch = 0;

    mv->em_state = alHeapAlloc(hp, 1, sizeof(ENVMIX_STATE));
    mv->em_first = 1;
    mv->em_motion = AL_STOPPED;
    mv->em_volume = 1;
    mv->em_ltgt = 1;
    mv->em_rtgt = 1;
    mv->em_cvolL = 1;
    mv->em_cvolR = 1;
    mv->em_dryamt = 0;
    mv->em_wetamt = 0;
    mv->em_lratm = 1;
    mv->em_lratl = 0;
    mv->em_lratm = 1;
    mv->em_lratl = 0;
    mv->em_delta = 0;
    mv->em_segEnd = 0;
    mv->em_pan = 0;
    mv->em_ctrlList = 0;
    mv->em_ctrlTail = 0;
    // non-vanilla
    mv->unk99 = 0;
    mv->unkA2 = 0;
    mv->unkA0 = 0;
    mv->unkCC = alHeapDBAlloc(0, 0, hp, 1, 8);
    mv->unkC8 = 0;
}
