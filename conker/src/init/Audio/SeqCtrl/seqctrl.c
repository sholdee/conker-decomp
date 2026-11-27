#include <n_libaudio.h>
#include "n_seqp.h"

extern f32 fabsf(f32);
#pragma intrinsic (fabsf)


void audio_ctrlVolume(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 vol) {
    N_ALVoiceState *vs;
    s16 tmp;
    seqp->chanState[chan].vol = vol;
    for(vs = seqp->vAllocHead; vs != NULL; vs = vs->next) {
        if ((vs->channel == chan) && (vs->envPhase != 3)) {
            tmp = __n_vsVol(vs, (N_ALSeqPlayer *)seqp);
            n_alSynSetVol(&vs->voice, tmp, __n_vsDelta(vs, seqp->curTime));
        }
    }
}

void audio_ctrlPan(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 pan) {
    N_ALVoiceState *vs;
    u8 tmp;
    seqp->chanState[chan].pan = pan;
    for (vs = seqp->vAllocHead; vs != NULL; vs = vs->next) {
        if ((vs->channel == chan)) {
            tmp = __n_vsPan(vs, (N_ALSeqPlayer *)seqp);
            n_alSynSetPan(&vs->voice, tmp);
        }
    }
}

void audio_ctrlPriority(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 priority) {
    seqp->chanState[chan].priority = priority;
}

void audio_ctrlSendMesg(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    if (seqp->msgQueue != 0) {
        osSendMesg(seqp->msgQueue, (arg3 & 7) | 0x10 | ((seqp->node.samplesLeft << 5) & -0x100), 0);
    }
}

void audio_ctrlBank(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    seqp->chanState[chan].unk8 = arg3;
}

void audio_applyChannelLowPass(N_ALCSPlayer *seqp, u8 chan) {
    N_ALVoiceState *vs;
    s16 pan;
    s8 keyoff;
    f32 pb;

    keyoff = (s8)seqp->chanState[chan].unk15 - 0x40; // matching: this file reads the byte signed
    pb = seqp->chanState[chan].pitchBend;

    for (vs = seqp->vAllocHead; vs != NULL; vs = vs->next) {
        if (vs->channel == chan) {
            pan = seqp->chanState[chan].unk14;
            func_1001CB40(&vs->voice, pan);
            if (pan != 0) {
                func_1001CA90(&vs->voice,
                    func_1001CEA4((vs->key - vs->sound->keyMap->keyBase) + keyoff) * 440.0f * pb);
            }
        }
    }
}

void audio_ctrlLowPassGain(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    seqp->chanState[chan].unk14 = arg3;
    audio_applyChannelLowPass(seqp, chan);
}

void audio_ctrlLowPassKey(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    seqp->chanState[chan].unk15 = arg3;
    audio_applyChannelLowPass(seqp, chan);
}

void func_10019F98(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    N_ALVoiceState *vs;
    seqp->chanState[chan].unk16 = arg3;
    for (vs = seqp->vAllocHead; vs != NULL; vs = vs->next) {
        if (vs->channel == chan) {
            func_1001E350(&vs->voice, arg3);
        }
    }
}

void audio_ctrlSustain(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    N_ALVoiceState *vs;

    seqp->chanState[chan].sustain = arg3;

    for (vs = seqp->vAllocHead; vs != NULL; vs = vs->next) {
        if (vs->channel == chan && vs->phase != 3) {
            if ((u32)arg3 >= 0x40) {
                if (vs->phase == 0) {
                    vs->phase = 2;
                }
            } else if (vs->phase == 2) {
                vs->phase = 0;
            } else if (vs->phase == 4) {
                vs->phase = 3;
                if (seqp->chanState[chan].useEnv != 0) {
                    __n_seqpReleaseVoice((N_ALSeqPlayer *)seqp, &vs->voice,
                        (seqp->chanState[chan].releaseTime < 0x3E80)
                            ? 0x3E80
                            : seqp->chanState[chan].releaseTime);
                } else {
                    __n_seqpReleaseVoice((N_ALSeqPlayer *)seqp, &vs->voice,
                        (vs->sound->envelope->releaseTime < 0x3E80)
                            ? 0x3E80
                            : vs->sound->envelope->releaseTime);
                }
            }
        }
    }
}

void func_1001A224(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    N_ALVoiceState *vs;

    seqp->chanState[chan].fxmix = (seqp->chanState[chan].fxmix & 0x7F) | (arg3 << 7);
    for (vs = seqp->vAllocHead; vs != NULL; vs = vs->next) {
        if ((vs->channel == chan) && (vs->envPhase != 3)) {
            n_alSynSetFXMix(&vs->voice, seqp->chanState[chan].fxmix);
        }
    }
}

void audio_ctrlFxMix(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    arg3 = arg3 & 0x7F;
    seqp->chanState[chan].fxmix = (seqp->chanState[chan].fxmix & 0x80) | arg3;
    func_1001A224(seqp, event, chan, seqp->chanState[chan].fxmix >> 7);
}

void audio_ctrlFxBus(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    if ((u32)arg3 < n_syn->maxAuxBusses) {
        seqp->chanState[chan].unkB = arg3;
    }
}

void audio_ctrlMp3Bank(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    seqp->mpegBank = arg3;
}

/* Plays sound mpegBank * 100 + arg3. */
void audio_ctrlMp3Start(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    mp3_startTrack(seqp->mpegBank * 100 + arg3, 0x7FFF, 0x40);
}

void audio_applyChannelVolume(N_ALCSPlayer *seqp, u8 chan) {
    N_ALVoiceState *vs;
    s16 tmp;
    for (vs = seqp->vAllocHead; vs != NULL; vs = vs->next) {
        if ((vs->channel == chan) && (vs->envPhase != 3)) {
            tmp = __n_vsVol(vs, (N_ALSeqPlayer *)seqp);
            n_alSynSetVol(&vs->voice, tmp, __n_vsDelta(vs, seqp->curTime));
        }
    }
}
void audio_ctrlFadeTo(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    f32 delta;

    if (seqp->chanState[chan].unkF == 0) {
        seqp->chanState[chan].unkF = 0x88;
    }
    if (seqp->chanState[chan].unkE != arg3) {
        delta = arg3 - seqp->chanState[chan].unkD;
        seqp->chanState[chan].unk10 = delta / (seqp->chanState[chan].unkF & 0x7F);
        seqp->chanState[chan].unk10 = fabsf(seqp->chanState[chan].unk10);
        if (seqp->chanState[chan].unkE == seqp->chanState[chan].unkD) {
            seqp->chanState[chan].unkE = arg3;
        } else {
            seqp->chanState[chan].unkE = arg3;
            return;
        }
        goto call_block;
    }
    return;
call_block:
    event->msg.midi.byte1 = 0xFE;
    audio_ctrlFadeStep(seqp, event, chan, arg3);
}

void audio_ctrlFadeStep(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    u8 sp2F;
    u8 sp2E;
    s32 sp28;
    s32 sp24;
    f32 sp20;
    f32 sp1C;

    sp2F = seqp->chanState[chan].unkD;
    sp2E = seqp->chanState[chan].unkE;
    sp20 = seqp->chanState[chan].unk10;
    sp28 = sp2E - sp2F;
    if (sp28 > 0) {
        if (seqp->chanState[chan].unkF & 0x80) {
            sp20 = sp20 * 2.0f;
        }
        sp24 = sp20;
        if (sp24 == 0) {
            sp24 = 1;
            sp1C = 1.0f / sp20;
        } else {
            sp1C = sp24 / sp20;
        }
        if (sp28 > sp24) {
            sp28 = sp24;
        }
    } else {
        sp24 = sp20;
        if (sp24 == 0) {
            sp24 = 1;
            sp1C = sp20;
        } else {
            sp1C = sp24 / sp20;
        }
        sp24 = -sp24;
        if (sp28 < sp24) {
            sp28 = sp24;
        }
    }
    sp2F = sp28 + sp2F;
    seqp->chanState[chan].unkD = sp2F;
    if (sp2F != sp2E) {
        n_alEvtqPostEvent(&seqp->evtq, event, seqp->uspt * 100 * sp1C, 2);
    }
    if (sp2F != 0) {
        seqp->chanMask |= (1 << chan);
    } else {
        seqp->chanMask &= ~(1 << chan);
    }
    audio_applyChannelVolume(seqp, chan);
}

void audio_ctrlFadeTime(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    seqp->chanState[chan].unkF = arg3;
}

void audio_ctrlFadeSet(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 arg3) {
    seqp->chanState[chan].unkD = arg3;
    seqp->chanState[chan].unkE = arg3;
    if (arg3 == 0) {
        seqp->chanMask &= (1 << chan) ^ 0xFFFF; // disable
    } else {
        seqp->chanMask |= 1 << chan;
    }
    audio_applyChannelVolume(seqp, chan);
}

/* The controller-change handlers (see n_seqp.h). */
CSPCtrlHandler gCspCtrlHandlers[0x5D + 4] = {
    /* 0x00 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    /* 0x07 */ audio_ctrlVolume, /* volume */
    /* 0x08 */ NULL, NULL,
    /* 0x0A */ audio_ctrlPan, /* pan */
    /* 0x0B */ NULL, NULL, NULL, NULL, NULL,
    /* 0x10 */ audio_ctrlPriority, /* priority */
    /* 0x11 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    /* 0x19 */ NULL,
    /* 0x1A */ audio_ctrlMp3Start,
    /* 0x1B */ audio_ctrlMp3Bank,
    /* 0x1C */ NULL, NULL,
    /* 0x1E */ audio_ctrlSendMesg,
    /* 0x1F */ NULL,
    /* 0x20 */ audio_ctrlBank,
    /* 0x21 */ audio_ctrlLowPassGain,
    /* 0x22 */ audio_ctrlLowPassKey,
    /* 0x23 */ func_10019F98,
    /* 0x24 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    /* 0x2C */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    /* 0x34 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    /* 0x3C */ NULL, NULL, NULL, NULL,
    /* 0x40 */ audio_ctrlSustain, /* sustain */
    /* 0x41 */ func_1001A224,
    /* 0x42 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    /* 0x4A */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    /* 0x52 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    /* 0x5A */ NULL,
    /* 0x5B */ audio_ctrlFxMix, /* fx mix */
    /* 0x5C */ audio_ctrlFxBus,
    /* 0x5D */ audio_ctrlFadeTo, /* controller 0xFF */
    /* 0x5E */ audio_ctrlFadeStep, /* controller 0xFE */
    /* 0x5F */ audio_ctrlFadeTime, /* controller 0xFD */
    /* 0x60 */ audio_ctrlFadeSet  /* controller 0xFC */
};
