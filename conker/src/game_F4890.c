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

s32 func_150C73E0(GameF4890LightState *light, struct108 *arg1) {
    f32 brightness;
    f32 fullBrightness;
    f32 t;

    fullBrightness = 1.0f;
    brightness = light->brightness / 255.0f;

    if (light->remaining == 0) {
        light->state = 12;
        if (arg1->unk2E4 != 0) {
            func_151616D0(0x11, 0x41, 0);
            arg1->unk2E4 = 0;
        }
        return 0;
    }

    if (light->remaining < 60) {
        brightness = (u32)(func_150ADA20() & 1) * ((light->remaining / 60.0f) / 4.0f);
    } else if (light->remaining < 300) {
        t = (light->remaining - 60) / 240.0f;
        brightness = (t * 0.75f) + 0.25f;
    } else if (light->remaining < 330.0f) {
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
        if (arg1->unk2E4 != 1) {
            func_151616D0(0x11, 0x40, 0);
            arg1->unk2E4 = 1;
        }
    } else {
        light->state = 12;
        if (arg1->unk2E4 != 0) {
            func_151616D0(0x11, 0x41, 0);
            arg1->unk2E4 = 0;
        }
    }

    return 0;
}
