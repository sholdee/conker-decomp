#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/player.h describes it. */
s8 D_800DD2B0[16];
u8 D_800DD2C0[16];

#include "functions.h"
#include "variables.h"

/*
 * Per-player pending warps: a warp zone fades the player's view out, and once the fade has run its course
 * the player is respawned at the zone's spawn point (or the zone's level is requested, when it is not the
 * current level level).
 */

/* Clears every pending warp and spawn point, then sets player 0's spawn point to `spawn0`. */
void func_15172C50(s32 spawn0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        D_800DD2B0[i] = -1;
        D_800DD2C0[i] = 0;
    }
    D_800DD2C0[0] = spawn0;
}

/* Cancels the player's pending warp, if any, and fades their view back in over 50 frames. */
void func_15172CA8(s32 player) {
    if (D_800DD2B0[player] != -1) {
        D_800DD2B0[player] = -1;
        func_1517EE40(0, 0, 0, 0, 1, player);
        func_1517EE40(0, 0, 0, 50, 0, player);
    }
}

/* Respawns `actor` at spawn point `spawnIndex` and clears its warping flag (0x10). */
void func_15172D28(Actor *actor, s32 spawnIndex) {
    func_15085430(actor, spawnIndex, 1);
    actor->unk2F8 &= ~0x10;
    if (D_800BE9B4 == 0 && actor->mv != NULL) {
        actor->mv->unk56 = 3;
    }
}

/* Per-frame update of the player's pending warp: cancels it when the player has no health, and once the
 * fade-out is complete either requests the warp's level or, in the current level, respawns the player. */
void func_15172D80(s32 player) {
    if (D_800DD2B0[player] == -1) {
        return;
    }
    if (movement_obj[player].energy == 0) {
        D_800DD2B0[player] = -1;
        func_1517EE40(0, 0, 0, 50, 0, player);
    }
    if (func_1517F40C(player)) {
        if (level != D_800DD2B0[player]) {
            func_1501C730(1, D_800DD2B0[player], 0, 0, 0);
        } else {
            func_15172CA8(player);
            func_15172D28(&movement_obj[player], D_800DD2C0[player]);
        }
    }
}

/*
 * Warp zone touched by `actor`, leading to spawn point `spawnIndex` of `levelId`. A player (an actor with a
 * camera) with no warp pending starts a 10-frame fade-out and records the warp; any other actor is
 * respawned at once when `levelId` is the current one.
 */
void func_15172E7C(Actor *actor, s32 levelId, s32 spawnIndex) {
    s32 player = actor->playernumber;

    if (actor->energy != 0 && (actor->mv == NULL || actor->mv->door_walkf_time <= 0)) {
        if (actor->CameraPtr != NULL) {
            if (D_800DD2B0[player] == -1) {
                func_1517EE40(0, 0, 0, 10, 1, player);
                actor->unk2F8 |= 0x10;
                D_800DD2B0[player] = levelId;
                D_800DD2C0[player] = spawnIndex;
            }
        } else if (levelId == level) {
            func_15172D28(actor, spawnIndex);
        }
    }
}
