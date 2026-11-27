#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/*
 * Model attachment hook 8 (gAttachHooks[8]): per-frame update for a flashing light. Flickers
 * randomly for the last 60 frames, ramps brightness over frames 60..300, pulses until 330 and
 * eases toward full above that. While lit it hides display list 1, plays a random sound
 * (0x44B..0x452) on first lighting and toggles light_sendEvent(0x11, 0x40/0x41), keeping the
 * owner's taskint[0] at 1 while that is in effect; when dark it hides display lists 2 and 3.
 * light->hookState: frames left in the flash sequence; light->hookState2: brightness 0..255.
 */
s32 attach_hookFlashingLight(ModelAttachment *light, Actor *owner) {
    f32 brightness;
    f32 fullBrightness;
    f32 progress;

    fullBrightness = 1.0f; // matching: a variable, so the 1.0f - 0.25f limit is computed at runtime
    brightness = light->hookState2 / 255.0f;

    if (light->hookState == 0) {
        light->hiddenDLists = 12;
        if (owner->taskint[0] != 0) {
            light_sendEvent(0x11, 0x41, 0);
            owner->taskint[0] = 0;
        }
        return 0;
    }

    if (light->hookState < 60) {
        brightness = (math_random() & 1) * ((light->hookState / 60.0f) / 4.0f);
    } else if (light->hookState < 300) {
        progress = (light->hookState - 60) / 240.0f;
        brightness = (progress * 0.75f) + 0.25f;
    } else if (light->hookState < 330.0f) {
        // matching: (1.0f / 4.0f) here, not 0.25f, gives the original constant load order
        if ((fullBrightness - (1.0f / 4.0f)) < brightness) {
            brightness = 0.25f;
        } else {
            brightness = brightness + (1.0f / 4.0f);
        }
    } else {
        brightness += (fullBrightness - brightness) * 0.1f;
    }

    if (light->hookState <= gFrameTicks) {
        light->hookState = 0;
    } else {
        light->hookState -= gFrameTicks;
    }

    light->hookState2 = brightness * 255.0f;

    if (brightness != 0.0f) {
        if (light->hiddenDLists != 2) {
            sound_play((math_random() & 7) + 0x44B, 0x7D00, 0x40, 0, 0);
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
