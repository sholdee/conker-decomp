#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .bss. */
u8 gRandomSoundTimer; /* frames until the next random zone sound (game_11EEB0.c runs the same countdown) */

void effect_playRandomZoneSounds(s32 arg0) {
    s32 var_s0;

    var_s0 = 0;
    prop_updateWaveChains();

    if (gRandomSoundTimer != 0) {
        if (D_800BE9E4 < gRandomSoundTimer) {
            gRandomSoundTimer -= D_800BE9E4;
        } else {
            gRandomSoundTimer = 0;
        }
        return;
    }

    if (zone_isActorOutsideId(0, 0x15) == 0) {
        var_s0 = 0x15;
    } else if (zone_isActorOutsideId(0, 0x1A) == 0) {
        var_s0 = 0x1A;
    } else if (zone_isActorOutsideId(0, 0x1B) == 0) {
        var_s0 = 0x1B;
    } else if (zone_isActorOutsideId(0, 0x1C) == 0) {
        var_s0 = 0x1C;
    } else if (zone_isActorOutsideId(0, 0x1D) == 0) {
        var_s0 = 0x1D;
    } else if (zone_isActorOutsideId(0, 0x20) == 0) {
        var_s0 = 0x20;
    }

    if (var_s0 != 0) {
        gRandomSoundTimer = (func_150ADA20() & 0x7F) + 0x80;
        func_10010F88((func_150ADA20() % 3U) + 0x6C, 0x5DC0, 0, 0, 0,
                      zones[var_s0].x,
                      zones[var_s0].y,
                      zones[var_s0].z,
                      0x320, 0x5DC);
    }
}
