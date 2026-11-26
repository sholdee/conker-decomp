#include "n_synthInternals.h"
#include <ultra64.h>
#include "structs.h"

/* Round a sample count up to a whole number of FIXED_SAMPLE frames. */
#define ROUND_UP_FRAME(n) ((((n) + FIXED_SAMPLE - 1) / FIXED_SAMPLE) * FIXED_SAMPLE)

/* Equal-power pan/fx-mix curve (libultra eqpower[128]). */
extern s16 D_8002BC10[];

/* Output-mode flags set by init_17870.c: C0 = stereo/headphone, C1 = mono, C2 = headphone. */
extern u8 D_800428C0;
extern u8 D_800428C1;
extern u8 D_800428C2;

Acmd *func_10020ABC(N_PVoice *filter, s16 *outp, s16 *inp, s32 sampleOffset, Acmd *p);
s16  _getVol(s16 ivol, s32 samples, s16 ratem, u16 ratel);
s32  func_10021C40(N_PVoice *f, s32 paramID, void *param);

/* n_alEnvmixerPull: drain the voice's parameter list, emitting one RSP sub-frame per
   parameter delta, then emit the remainder of the 184-sample frame. */
Acmd *func_10020000(N_PVoice *f, s32 sampleOffset, Acmd *p) {
    Acmd     *ptr = p;
    N_PVoice *e = f;
    s16       outp;
    u32       lastDelta;
    u32       delta = sampleOffset;
    s32       deltaSamples;
    s16       inp = 0;
    s32       vol;
    ALParam  *thisCmd;
    s32       count = FIXED_SAMPLE;

    outp = 0;

    while (e->em_ctrlList != NULL) {
        lastDelta = delta;
        delta = e->em_ctrlList->delta;
        deltaSamples = ROUND_UP_FRAME(delta - lastDelta);
        if (deltaSamples == 0) {
            delta = lastDelta;
        }
        if (deltaSamples > count) {
            break;
        }

        switch (e->em_ctrlList->type) {
            case AL_FILTER_START_VOICE_ALT: {
                ALStartParamAlt *startParam = (ALStartParamAlt *)e->em_ctrlList;
                s32              startVol;

                if (startParam->unity) {
                    e->rs_upitch = 1;
                }
                func_10021C40(e, AL_FILTER_SET_WAVETABLE, startParam->wave);
                e->em_motion = AL_PLAYING;
                e->em_first = 1;
                e->em_delta = 0;
                e->em_segEnd = ROUND_UP_FRAME(startParam->samples);
                startVol = (startParam->volume + startParam->volume) / 2; // matching: -g keeps the temp store
                e->em_volume = startVol;
                e->em_pan = startParam->pan;
                e->em_dryamt = D_8002BC10[startParam->fxMix & 0x7F] & 0xFFFC;
                if (D_800428C0 != 0) {
                    e->em_dryamt |= startParam->fxMix >> 7;
                }
                e->em_wetamt = D_8002BC10[127 - (startParam->fxMix & 0x7F)] & 0xFFFC;
                if (D_800428C2 != 0) {
                    e->em_pan = (e->em_pan >> 1) + 32;
                } else if (D_800428C1 != 0) {
                    e->em_pan = AL_PAN_CENTER;
                }
                if (startParam->samples != 0) {
                    e->em_cvolL = 1;
                    e->em_cvolR = 1;
                } else {
                    e->em_cvolL = (D_8002BC10[e->em_pan] * e->em_volume) >> 15;
                    e->em_cvolR = (D_8002BC10[127 - e->em_pan] * e->em_volume) >> 15;
                }
                e->rs_ratio = startParam->pitch;
                e->unkA2 = startParam->unk15;
                e->unkA0 = startParam->unk18;
                e->unkC8 = 1;
                e->unk99 = startParam->unk14;
                break;
            }

            case AL_FILTER_SET_VOLUME:
            case AL_FILTER_SET_PAN:
            case AL_FILTER_SET_FXAMT:
                ptr = func_10020ABC(e, &outp, &inp, deltaSamples, ptr);
                if (e->em_delta >= e->em_segEnd) {
                    e->em_ltgt = (D_8002BC10[e->em_pan] * e->em_volume) >> 15;
                    e->em_rtgt = (D_8002BC10[127 - e->em_pan] * e->em_volume) >> 15;
                    e->em_delta = e->em_segEnd;
                    e->em_cvolL = e->em_ltgt;
                    e->em_cvolR = e->em_rtgt;
                } else {
                    e->em_cvolL = _getVol(e->em_cvolL, e->em_delta, e->em_lratm, e->em_lratl);
                    e->em_cvolR = _getVol(e->em_cvolR, e->em_delta, e->em_rratm, e->em_rratl);
                }
                if (e->em_cvolL == 0) {
                    e->em_cvolL = 1;
                }
                if (e->em_cvolR == 0) {
                    e->em_cvolR = 1;
                }
                if (e->em_ctrlList->type == AL_FILTER_SET_PAN) {
                    if (D_800428C2 != 0) {
                        e->em_pan = ((s16)e->em_ctrlList->data.i >> 1) + 32;
                    } else if (D_800428C1 != 0) {
                        e->em_pan = AL_PAN_CENTER;
                    } else {
                        e->em_pan = e->em_ctrlList->data.i;
                    }
                }
                if (e->em_ctrlList->type == AL_FILTER_SET_VOLUME) {
                    e->em_delta = 0;
                    vol = e->em_ctrlList->data.i;
                    vol = (vol + vol) / 2; // matching: -g keeps this as its own store to vol
                    e->em_volume = vol;
                    e->em_segEnd = ROUND_UP_FRAME(e->em_ctrlList->moredata.i);
                }
                if (e->em_ctrlList->type == AL_FILTER_SET_FXAMT) {
                    if (((e->em_dryamt & 1) ^ (e->em_wetamt & 1) ^
                         ((e->em_ctrlList->data.i + 1) >> 7)) != 0) {
                        if (D_800428C0 != 0) {
                            if (e->em_pan > AL_PAN_CENTER) {
                                e->em_dryamt ^= 1;
                                if (e->em_pan < 111) {
                                    e->em_dryamt |= 2;
                                }
                            } else {
                                e->em_wetamt ^= 1;
                                if (e->em_pan > 16) {
                                    e->em_wetamt |= 2;
                                }
                            }
                        }
                    }
                    e->em_dryamt =
                        (D_8002BC10[e->em_ctrlList->data.i & 0x7F] & 0xFFFC) | (e->em_dryamt & 3);
                    e->em_wetamt =
                        (D_8002BC10[127 - (e->em_ctrlList->data.i & 0x7F)] & 0xFFFC) | (e->em_wetamt & 3);
                }
                e->em_first = 1;
                break;

            case AL_FILTER_START_VOICE: {
                ALStartParam *startParam = (ALStartParam *)e->em_ctrlList;

                if (startParam->unity) {
                    e->rs_upitch = 1;
                }
                func_10021C40(e, AL_FILTER_SET_WAVETABLE, startParam->wave);
                e->em_motion = AL_PLAYING;
                break;
            }

            case AL_FILTER_STOP_VOICE:
                ptr = func_10020ABC(e, &outp, &inp, deltaSamples, ptr);
                n_alEnvmixerParam(e, AL_FILTER_RESET, 0);
                break;

            case AL_FILTER_FREE_VOICE: {
                N_ALFreeParam *freeParam = (N_ALFreeParam *)e->em_ctrlList;

                freeParam->pvoice->offset = 0;
                _n_freePVoice(freeParam->pvoice);
                break;
            }

            case AL_FILTER_SET_PITCH:
                ptr = func_10020ABC(e, &outp, &inp, deltaSamples, ptr);
                e->rs_ratio = e->em_ctrlList->data.f;
                break;

            case AL_FILTER_SET_UNITY_PITCH:
                ptr = func_10020ABC(e, &outp, &inp, deltaSamples, ptr);
                e->rs_upitch = 1;
                break;

            case AL_FILTER_SET_WAVETABLE:
                ptr = func_10020ABC(e, &outp, &inp, deltaSamples, ptr);
                func_10021C40(e, AL_FILTER_SET_WAVETABLE, (void *)e->em_ctrlList->data.i);
                break;

            default:
                ptr = func_10020ABC(e, &outp, &inp, deltaSamples, ptr);
                n_alEnvmixerParam(e, e->em_ctrlList->type, (void *)e->em_ctrlList->data.i);
                break;
        }

        inp += deltaSamples * 2;
        count -= deltaSamples;
        thisCmd = e->em_ctrlList;
        e->em_ctrlList = e->em_ctrlList->next;
        if (e->em_ctrlList == NULL) {
            e->em_ctrlTail = NULL;
        }
        _n_freeParam(thisCmd);
    }

    ptr = func_10020ABC(e, &outp, &inp, count, ptr);
    if (e->em_delta > e->em_segEnd) {
        e->em_delta = e->em_segEnd;
    }
    return ptr;
}

/* Queue a parameter update or apply an immediate envelope-mixer command. */
s32 n_alEnvmixerParam(N_PVoice *filter, s32 paramID, void *param) {
    N_PVoice *e = filter;

    switch (paramID) {
        case AL_FILTER_ADD_UPDATE:
            if (e->em_ctrlTail != NULL) {
                e->em_ctrlTail->next = (ALParam *)param;
            } else {
                e->em_ctrlList = (ALParam *)param;
            }
            e->em_ctrlTail = (ALParam *)param;
            break;

        case AL_FILTER_RESET:
            e->em_first = 1;
            e->em_motion = AL_STOPPED;
            e->em_volume = 1;
            e->em_segEnd = 0;
            e->rs_delta = 0.0f;
            e->rs_first = 1;
            e->rs_upitch = 0;
            n_alLoadParam(e, AL_FILTER_RESET, param);
            break;

        case AL_FILTER_START:
            e->em_motion = AL_PLAYING;
            break;

        default:
            n_alLoadParam(e, paramID, param);
            break;
    }
    return 0;
}

Acmd *func_10022040(N_PVoice *voice, s16 *outp, s32 arg2, Acmd *p);
s16 _getRate(f32 vol, f32 tgt, s32 count, u16 *ratel);

/* _pullSubFrame.  `inp` is never read here -- the caller advances it -- but it really is
   a pointer: n_alEnvmixerPull passes &inp, not a sample count. */
Acmd *func_10020ABC(N_PVoice *filter, s16 *outp, s16 *inp, s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    N_PVoice *e = filter;

    if (e->dc_table != NULL) {
        ((WaveTable *)e->dc_table)->inUse = 1;
    }

    if ((e->em_motion != AL_PLAYING) || (sampleOffset == 0)) {
        return ptr;
    }

    ptr = func_10022040(e, outp, sampleOffset, p);

    if (e->em_first != 0) {
        e->em_first = 0;

        e->em_ltgt = (D_8002BC10[e->em_pan] * e->em_volume) >> 15;
        e->em_lratm = _getRate(e->em_cvolL, e->em_ltgt, e->em_segEnd, &e->em_lratl);
        e->em_rtgt = (D_8002BC10[127 - e->em_pan] * e->em_volume) >> 15;
        e->em_rratm = _getRate(e->em_cvolR, e->em_rtgt, e->em_segEnd, &e->em_rratl);

        n_aSetVolume(ptr++, A_VOL | A_LEFT, e->em_cvolL, e->em_dryamt, e->em_wetamt);
        n_aSetVolume(ptr++, A_VOL | A_RIGHT, e->em_rtgt, e->em_rratm, e->em_rratl);
        n_aSetVolume(ptr++, A_RATE, e->em_ltgt, e->em_lratm, e->em_lratl);
        n_aEnvMixer(ptr++, A_INIT, e->em_cvolR, osVirtualToPhysical(e->em_state));
    } else {
        n_aEnvMixer(ptr++, A_CONTINUE, 0, osVirtualToPhysical(e->em_state));
    }

    *outp += FIXED_SAMPLE << 1;
    e->em_delta += FIXED_SAMPLE;

    if ((e->em_dryamt & 2) || (e->em_wetamt & 2)) {
        e->em_dryamt &= ~2;
        e->em_wetamt &= ~2;
        e->em_first = 1;
    }

    return ptr;
}

/* Compute the per-8-sample volume ramp (integer part returned, fraction in *ratel)
   that takes `vol` to `tgt` over `count` samples. */
s16 _getRate(f32 vol, f32 tgt, s32 count, u16 *ratel) {
    s16 rateInt;
    s16 carry;
    f32 invCount;
    f32 rate;
    f32 frac;

    if (count == 0) {
        if (vol <= tgt) {
            *ratel = 0xFFFF;
            return 0x7FFF;
        } else {
            *ratel = 0;
            return -0x8000;
        }
    }
    invCount = 1.0f / count;

    if (tgt < 1.0f) {
        tgt = 1.0f;
    }
    if (vol <= 0.0f) {
        vol = 1.0f;
    }
    rate = (tgt - vol) * invCount * 8.0f;
    rateInt = rate;
    frac = rate - rateInt;
    rateInt -= 1;
    frac += 1.0f;
    carry = frac;
    rateInt += carry;
    frac -= carry;
    *ratel = 65535.0f * frac;
    return rateInt;
}

/* Advance a volume ramp by `samples` samples at rate ratem.ratel per 8 samples. */
s16 _getVol(s16 ivol, s32 samples, s16 ratem, u16 ratel) {
    s32 step;

    samples >>= 3;
    if (samples == 0) {
        return ivol;
    }

    step = ratel * samples;
    step >>= 16;
    step += ratem * samples;
    ivol += step;
    return ivol;
}
