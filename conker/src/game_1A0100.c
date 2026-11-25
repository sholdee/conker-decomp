#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Per-player pending warps: a warp zone fades the player's view out, and once the fade has run its course
 * the player is respawned at the zone's spawn point (or the zone's level is requested, when it is not the
 * current level D_800BE9F0).
 */

extern s8 D_800DD2B0[]; // per player: level of the pending warp, -1 when none

/* variables.h declares D_800DD2C0 as a single u8; it is really a u8[16] of per-player spawn points.
 * Entry 0 is also read and written on its own as player 0's spawn point. */
#define D_800DD2C0_ARRAY (*(u8 (*)[16])&D_800DD2C0)

extern u8 D_800BE9B4;

void func_1517EE40(s32 r, s32 g, s32 b, s32 duration, s8 toColor, s32 view);
s32 func_1517F40C(s32 view);
void func_1501C730(s32, s32, s32, s32, s32);
void func_15085430(struct127 *obj, s32 spawnIndex, s32 unused);

/* Clears every pending warp and spawn point, then sets player 0's spawn point to `spawn0`. */
void func_15172C50(s32 spawn0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        D_800DD2B0[i] = -1;
        D_800DD2C0_ARRAY[i] = 0;
    }
    D_800DD2C0_ARRAY[0] = spawn0;
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
void func_15172D28(struct127 *actor, s32 spawnIndex) {
    func_15085430(actor, spawnIndex, 1);
    actor->unk2F8 &= ~0x10;
    if (D_800BE9B4 == 0 && actor->unk31C != NULL) {
        actor->unk31C->pad56 = 3;
    }
}

/* Per-frame update of the player's pending warp: cancels it when the player has no health, and once the
 * fade-out is complete either requests the warp's level or, in the current level, respawns the player. */
void func_15172D80(s32 player) {
    if (D_800DD2B0[player] == -1) {
        return;
    }
    if (D_800CC2D0[player].health == 0) {
        D_800DD2B0[player] = -1;
        func_1517EE40(0, 0, 0, 50, 0, player);
    }
    if (func_1517F40C(player)) {
        if (D_800BE9F0 != D_800DD2B0[player]) {
            func_1501C730(1, D_800DD2B0[player], 0, 0, 0);
        } else {
            func_15172CA8(player);
            func_15172D28(&D_800CC2D0[player], D_800DD2C0_ARRAY[player]);
        }
    }
}

/*
 * Warp zone touched by `actor`, leading to spawn point `spawnIndex` of `level`. A player (an actor with a
 * camera) with no warp pending starts a 10-frame fade-out and records the warp; any other actor is
 * respawned at once when `level` is the current one.
 */
void func_15172E7C(struct127 *actor, s32 level, s32 spawnIndex) {
    s32 player = actor->unk127;

    // struct126 has no field at 0x23 (only padding there in structs.h)
    if (actor->health != 0 && (actor->unk31C == NULL || *(s8 *)((u8 *)actor->unk31C + 0x23) <= 0)) {
        if (actor->camera != NULL) {
            if (D_800DD2B0[player] == -1) {
                func_1517EE40(0, 0, 0, 10, 1, player);
                actor->unk2F8 |= 0x10;
                D_800DD2B0[player] = level;
                D_800DD2C0_ARRAY[player] = spawnIndex;
            }
        } else if (level == D_800BE9F0) {
            func_15172D28(actor, spawnIndex);
        }
    }
}
