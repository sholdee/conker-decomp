#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/prop.h"
#include "conker/sound.h"
#include "conker/zone.h"

/* This file's .bss. */
u8 gRandomSoundTimer; /* frames until the next random zone sound (game_11EEB0.c runs the same countdown) */

void effect_playRandomZoneSounds(s32 arg0) {
    s32 var_s0;

    var_s0 = 0;
    prop_updateWaveChains();

    if (gRandomSoundTimer != 0) {
        if (gFrameTicks < gRandomSoundTimer) {
            gRandomSoundTimer -= gFrameTicks;
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
        gRandomSoundTimer = (math_random() & 0x7F) + 0x80;
        sound_playAt((math_random() % 3U) + 0x6C, 0x5DC0, 0, 0, 0,
                      zones[var_s0].x,
                      zones[var_s0].y,
                      zones[var_s0].z,
                      0x320, 0x5DC);
    }
}
