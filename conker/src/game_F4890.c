#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/*
 * Model attachment hook 8 (D_800841C0[8]): per-frame update for a flashing light. Flickers
 * randomly for the last 60 frames, ramps brightness over frames 60..300, pulses until 330 and
 * eases toward full above that. While lit it hides display list 1, plays a random sound
 * (0x44B..0x452) on first lighting and toggles light_sendEvent(0x11, 0x40/0x41), keeping the
 * owner's taskint[0] at 1 while that is in effect; when dark it hides display lists 2 and 3.
 * light->unk38: frames left in the flash sequence; light->unk3C: brightness 0..255.
 */
s32 func_150C73E0(ModelAttachment *light, Actor *owner) {
    f32 brightness;
    f32 fullBrightness;
    f32 progress;

    fullBrightness = 1.0f; // matching: a variable, so the 1.0f - 0.25f limit is computed at runtime
    brightness = light->unk3C / 255.0f;

    if (light->unk38 == 0) {
        light->hiddenDLists = 12;
        if (owner->taskint[0] != 0) {
            light_sendEvent(0x11, 0x41, 0);
            owner->taskint[0] = 0;
        }
        return 0;
    }

    if (light->unk38 < 60) {
        brightness = (func_150ADA20() & 1) * ((light->unk38 / 60.0f) / 4.0f);
    } else if (light->unk38 < 300) {
        progress = (light->unk38 - 60) / 240.0f;
        brightness = (progress * 0.75f) + 0.25f;
    } else if (light->unk38 < 330.0f) {
        // matching: (1.0f / 4.0f) here, not 0.25f, gives the original constant load order
        if ((fullBrightness - (1.0f / 4.0f)) < brightness) {
            brightness = 0.25f;
        } else {
            brightness = brightness + (1.0f / 4.0f);
        }
    } else {
        brightness += (fullBrightness - brightness) * 0.1f;
    }

    if (light->unk38 <= D_800BE9E4) {
        light->unk38 = 0;
    } else {
        light->unk38 -= D_800BE9E4;
    }

    light->unk3C = brightness * 255.0f;

    if (brightness != 0.0f) {
        if (light->hiddenDLists != 2) {
            func_10010F30((func_150ADA20() & 7) + 0x44B, 0x7D00, 0x40, 0, 0);
        }
        light->hiddenDLists = 2;
        if (owner->taskint[0] != 1) {
            light_sendEvent(0x11, 0x40, 0);
            owner->taskint[0] = 1;
        }
    } else {
        light->hiddenDLists = 12;
        if (owner->taskint[0] != 0) {
            light_sendEvent(0x11, 0x41, 0);
            owner->taskint[0] = 0;
        }
    }

    return 0;
}
