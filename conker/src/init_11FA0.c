#include <ultra64.h>

/* This file's .bss. */
static s32 gMixRequest;      /* one-shot selector request */
static s32 gMixOverride;      /* overriding selector */
static s32 gMixFallback;      /* fallback selector */
static s32 D_8004277C;      /* mix state */
static f32 gMixSentFreqs[2];   /* per channel: frequency last sent */
static f32 gMixSentGains[2];   /* per channel: gain last sent */
static f32 gMixSentVolume;      /* volume last sent */
static f32 D_80042794;
static f32 gMixWobblePhase;      /* phase of the frequency wobble */

#include "functions.h"
#include "variables.h"

/* Requests an audio-mix selector for the next update (cleared after each audio_updateMix). */
void audio_requestMixSelector(s32 selector) {
    gMixRequest = selector;
}

/* Sets the overriding selector; leaving selector 3 re-raises D_80041FD9. */
void audio_setMixOverride(s32 selector) {
    s32 previous = gMixOverride;

    if (previous == 3) {
        D_80041FD9 = 1;
    }
    gMixOverride = selector;
}

/* Sets the fallback selector, used when no overriding selector is set. */
void audio_setMixFallback(s32 selector) {
    gMixFallback = selector;
}

/* Clears all selectors and the mix state. */
void audio_clearMixSelectors(void) {
    gMixRequest = gMixOverride = gMixFallback = D_8004277C = 0;
}

typedef struct {
    f32 values[2];
} MainSelectorPair;

/* This file's .data: the gains and frequencies before any selector applies. */
static MainSelectorPair D_8002BA10 = { { 0.0f, 0.0f } };
static MainSelectorPair D_8002BA18 = { { 11025.0f, 11025.0f } };

/*
 * Per-frame audio mix update. Picks the active selector (override > fallback > one-shot request,
 * forced to 1 by camera_global_Cameras->SpecialFlags bit 0 unless D_800B0DF0 byte 0x2A bit 0), eases the volume, gain and frequency
 * targets for it, and sends any values that changed to the audio channels.
 */
void audio_updateMix(void) {
    s32 i;
    f32 volume = 0.0f;
    s32 selector;
    MainSelectorPair gains = D_8002BA10;
    MainSelectorPair frequencies = D_8002BA18;
    f32 scale;

    selector = gMixRequest;
    if ((camera_global_Cameras->SpecialFlags & 1) && !(D_800B0DF0->flags & 1)) {
        selector = 1;
    }
    if (gMixOverride != 0) {
        selector = gMixOverride;
    } else if (gMixFallback != 0) {
        selector = gMixFallback;
    }
    if (selector == 6) {
        selector = 0;
    }
    switch (selector) {
        case 1:
            volume = (13.0f - gMixSentVolume) + gMixSentVolume;
            D_8004277C = 0;
            gains.values[0] = 127.0f;
            frequencies.values[0] = gMixSentFreqs[0] + ((519.0f + sinf(gMixWobblePhase * 0.052359879f) * 200.0f) - gMixSentFreqs[0]);
            break;
        case 2:
            D_8004277C = 0;
            volume = 70.0f;
            gains.values[0] = 127.0f;
            frequencies.values[0] = sinf(gMixWobblePhase * 0.34906584f) * 200.0f + 450.0f;
            break;
        case 3:
            volume = (52.0f - gMixSentVolume) * 0.08f + gMixSentVolume;
            D_8004277C = 0;
            gains.values[0] = gMixSentGains[0] + (127.0f - gMixSentGains[0]) * 0.08f;
            frequencies.values[0] = gMixSentFreqs[0] + ((436.0f + sinf(gMixWobblePhase * 0.122173049f) * 282.0f) - gMixSentFreqs[0]) * 0.08f;
            D_80041FD9 = 0;
            break;
        case 4:
            D_8004277C = 4;
            gains.values[1] = 127.0f;
            if (D_800BE9B4 != 0) {
                frequencies.values[1] = 400.0f;
            } else {
                frequencies.values[1] = gMixSentFreqs[1] + (400.0f - gMixSentFreqs[1]);
            }
            D_80041FD9 = 0;
            break;
        case 5:
            gains.values[1] = 127.0f;
            frequencies.values[1] = 520.0f;
            D_80041FD9 = 0;
            break;
        default:
            if (D_8004277C == 4) {
                frequencies.values[1] = gMixSentFreqs[1] + (11000.0f - gMixSentFreqs[1]) * 0.05f;
                if (frequencies.values[1] < 9000.0f) {
                    D_8004277C = 4;
                } else {
                    D_8004277C = 0;
                    D_80041FD9 = 1;
                }
            }
            break;
    }
    gMixWobblePhase += gFrameDelta;
    if (volume != gMixSentVolume || D_80042794 != 1.0f) {
        scale = volume / 127.0f;
        for (i = 0; i < 3; i++) {
            func_10008BC0(i, scale, 1.0f);
        }
        gMixSentVolume = volume;
        D_80042794 = 1.0f;
    }
    for (i = 0; i < 2; i++) {
        if (frequencies.values[i] != gMixSentFreqs[i]) {
            func_10008B60(i, i, 9, 0, frequencies.values[i]);
            gMixSentFreqs[i] = frequencies.values[i];
        }
        if (gains.values[i] != gMixSentGains[i]) {
            func_10008B60(i, i, 8, 0, gains.values[i]);
            gMixSentGains[i] = gains.values[i];
        }
    }
    gMixRequest = 0;
}
