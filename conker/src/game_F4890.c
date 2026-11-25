#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern void func_151616D0(u8, u8, s32);

typedef struct GameF4890LightState {
    /* 0x00 */ u8  pad0[0x13];
    /* 0x13 */ u8  state;
    /* 0x14 */ u8  pad14[0x24];
    /* 0x38 */ s32 remaining;  /* frames left in the flash sequence */
    /* 0x3C */ s32 brightness; /* 0..255 */
} GameF4890LightState;

/*
 * Per-frame update for a flashing light: flickers randomly for the last 60 frames, ramps
 * brightness over frames 60..300, pulses until 330 and eases toward full above that. While lit it
 * sets state 2, plays a random sound (0x44B..0x452) on first lighting and toggles
 * func_151616D0(0x11, 0x40/0x41) via obj->unk2E4; when dark it sets state 12.
 */
s32 func_150C73E0(GameF4890LightState *light, struct108 *obj) {
    f32 brightness;
    f32 fullBrightness;
    f32 progress;

    fullBrightness = 1.0f; // matching: a variable, so the 1.0f - 0.25f limit is computed at runtime
    brightness = light->brightness / 255.0f;

    if (light->remaining == 0) {
        light->state = 12;
        if (obj->unk2E4 != 0) {
            func_151616D0(0x11, 0x41, 0);
            obj->unk2E4 = 0;
        }
        return 0;
    }

    if (light->remaining < 60) {
        brightness = (u32)(func_150ADA20() & 1) * ((light->remaining / 60.0f) / 4.0f);
    } else if (light->remaining < 300) {
        progress = (light->remaining - 60) / 240.0f;
        brightness = (progress * 0.75f) + 0.25f;
    } else if (light->remaining < 330.0f) {
        // matching: (1.0f / 4.0f) here, not 0.25f, gives the original constant load order
        if ((fullBrightness - (1.0f / 4.0f)) < brightness) {
            brightness = 0.25f;
        } else {
            brightness = brightness + (1.0f / 4.0f);
        }
    } else {
        brightness += (fullBrightness - brightness) * 0.1f;
    }

    if (light->remaining <= D_800BE9E4) {
        light->remaining = 0;
    } else {
        light->remaining -= D_800BE9E4;
    }

    light->brightness = brightness * 255.0f;

    if (brightness != 0.0f) {
        if (light->state != 2) {
            func_10010F30((func_150ADA20() & 7) + 0x44B, 0x7D00, 0x40, 0, 0);
        }
        light->state = 2;
        if (obj->unk2E4 != 1) {
            func_151616D0(0x11, 0x40, 0);
            obj->unk2E4 = 1;
        }
    } else {
        light->state = 12;
        if (obj->unk2E4 != 0) {
            func_151616D0(0x11, 0x41, 0);
            obj->unk2E4 = 0;
        }
    }

    return 0;
}
