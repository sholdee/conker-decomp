#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/swarm.h describes it. */
EffectModelSlot D_800DDE80[11];
u8 D_800DDF5C[4];
u8 D_800DDF60[4];
s8 D_800DDF68[11];
Gfx *D_800DDF78[11];

#include "functions.h"
#include "variables.h"

/*
 * Effect model slots.
 *
 * D_800DDE80[11] holds one loaded model per slot (EffectModelSlot).
 * D_800DDF68[slot] is the slot's use state: 1 = used this frame, then counts down each frame;
 * -2 = unused, freed by func_15183D28.  D_800DDF78[slot] is the model's second display list.
 * D_800DDF5C[4] marks the swarm groups handed out by func_15183290.
 */

/* Model resource returned by the loader func_1502B6BC. */
typedef struct ModelResource {
    /* 0x00 */ Vtx *unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vtx *unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ Gfx *unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ Gfx *unk18;
} ModelResource;

/*
 * Per-slot motion parameters, indexed like D_800DDE80. This file wobbles the wings of slots 0..6 (and the
 * second vertex set of slots 2..4 with D_8008D318) between minAngle and maxAngle through func_15165C80.
 * game_1A20A0 reads the swarm slots through its own view (SwarmFlight), game_1A7260 the wave slots 8 and 9
 * (WaveState): they give other meanings to the words from 0x18 on.
 */
typedef struct Game1B0740Wave {
    /* 0x00 */ u8 maxSpeed;
    /* 0x01 */ u8 minSpeed;
    /* 0x02 */ u8 wanderTime;
    /* 0x03 */ u8 turnTime;
    /* 0x04 */ f32 maxRoll;
    /* 0x08 */ f32 maxPitch;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 maxYawStep;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 angle;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 velocity;
    /* 0x28 */ f32 maxAngle;
    /* 0x2C */ f32 minAngle;
    /* 0x30 */ f32 radius;
    /* 0x34 */ f32 unk34;
} Game1B0740Wave; /* size 0x38 */

/* The model id of each slot (func_15183BA4 looks a model up here). */
static const u8 D_800A72D0[11] = { 10, 10, 21, 22, 23, 24, 20, 19, 68, 69, 19 };
/* The wing vertex indices that func_15165C80 moves, per slot: left half, then right half. */
static const u8 D_800A72DC[2] = { 13, 16 };
static const u8 D_800A72E0[2] = { 1, 3 };
static const u8 D_800A72E4[4] = { 9, 10, 13, 16 };
/* The number of wing vertex indices per slot. */
static const u8 D_800A72E8[7] = { 2, 2, 2, 2, 2, 2, 4 };

/* No header extern: game_1A20A0 and game_1A7260 declare their own views of it. */
Game1B0740Wave D_8008D0B0[11] = {
    { 10, 8, 60, 60, 40.0f, 40.0f, 10.0f, 4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 17.0f, 45.0f, -45.0f, 35.0f, 0.0f },
    { 12, 8, 60, 60, 40.0f, 40.0f, 10.0f, 4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 15.0f, 45.0f, -45.0f, 35.0f, 0.0f },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, 0.0f, 0.0f, 0.0f, 10.0f, 75.0f, 10.0f, 49.0f, 0.0f },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, 0.0f, 0.0f, 0.0f, 9.0f, 75.0f, 10.0f, 49.0f, 0.0f },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, 0.0f, 0.0f, 0.0f, 8.0f, 75.0f, 10.0f, 49.0f, 0.0f },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, 0.0f, 0.0f, 0.0f, 10.0f, 75.0f, 10.0f, 49.0f, 0.0f },
    { 4, 1, 20, 20, 40.0f, 40.0f, 10.0f, 4.0f, 2.0f, 0.0f, 0.0f, 0.0f, 10.0f, 20.0f, -20.0f, 471.0f, 0.0f },
    { 10, 4, 60, 60, 40.0f, 40.0f, 10.0f, 4.0f, 2.0f, 0.0f, 35.0f, 17.5f, 12.5f, 45.0f, -45.0f, 29.0f, 72.0f },
    { 2, 1, 120, 120, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, 10.0f, 40.0f, 0.0f, 0.0f, 21.0f, 5.0f, 25.0f, 0.0f },
    { 2, 1, 120, 120, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, 10.0f, 40.0f, 0.0f, 0.0f, 21.0f, 5.0f, 25.0f, 0.0f },
    { 6, 2, 60, 60, 40.0f, 40.0f, 10.0f, 4.0f, 2.0f, 0.0f, 35.0f, 17.5f, 12.5f, 45.0f, -45.0f, 29.0f, 72.0f },
};

/* The wobble of the second vertex set of slots 2..4. */
static Game1B0740Wave D_8008D318[3] = {
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.5f, 75.0f, 40.0f, 49.0f, 0.0f },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.400000006f, 75.0f, 40.0f, 49.0f, 0.0f },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.3000000119f, 75.0f, 40.0f, 49.0f, 0.0f },
};

/* The wing vertex indices of each wobbling slot. */
static const u8 *D_8008D3C0[7] = {
    D_800A72DC, D_800A72DC, D_800A72E0, D_800A72E0, D_800A72E0, D_800A72E0, D_800A72E4,
};

/*
 * Spawns effect `type` at (x, y, z) for `player`, loading the models it needs.
 * Returns the swarm group used, or -1 if no group is free or the effect can't start.
 */
s32 func_15183290(s32 x, s32 y, s32 z, s32 count, s32 type, s32 radius, s32 player, s32 maxRadius,
                  s32 heightRange, s32 minRadius, s32 spawnFlags) {
    s32 group;
    Actor *target;

    for (group = 0; group < 4; group++) {
        if (D_800DDF5C[group] == 0) {
            break;
        }
    }
    if (group == 4) {
        return -1;
    }
    maxRadius *= 16;
    heightRange *= 16;
    minRadius *= 16;
    target = camera_global_Cameras[player].CharPtr;
    switch (type) {
        case 0:
            func_15177F90(count, NULL, 0xFE, x, y, z, 0, 0x7FFF, 0xFF, 1, 0);
            break;
        case 1:
            func_15177F90(count, &movement_obj[0].xpos, 0xFE, x, y, z, 0, 0x7FFF, 0xFF, 1, 0);
            break;
        case 2:
            func_151838B0(0);
            func_15175390(count, group, x, y, z, 63, player, 30, 0, 0x30003, 10, 0, 0, 1, 2, 0.8000000119f, 0.5f,
                          spawnFlags);
            break;
        case 3:
            if (func_15184118(target)) {
                func_15174BF0(count, group, x, y, z, player, 0.8000000119f, 0.5f, spawnFlags);
                break;
            }
            return -1;
        case 8:
            return -1;
        case 4:
            func_151839F0(7, 10);
            func_15174FA4(count, group, x, y, z, maxRadius * 4, player, 0, 0.8000000119f, 0.5f);
            break;
        case 10:
            func_15183ACC(7);
            func_15174FA4(count, group, x, y, z, maxRadius * 4, player, 1, 0.8000000119f, 0.5f);
            break;
        case 11:
            func_15183ACC(2);
            func_15183ACC(3);
            func_15183ACC(4);
            func_15183ACC(5);
            func_15175390(count, group, x, y, z, maxRadius, player, heightRange, minRadius, 0x4D, 21, 3, 2, -1, 0,
                          0.8000000119f, 0.5f, spawnFlags);
            break;
        case 5:
            func_15183ACC(2);
            func_15183ACC(3);
            func_15183ACC(4);
            func_15183ACC(5);
            func_15175390(count, group, x, y, z, maxRadius, player, heightRange, minRadius, 0x4D, 21, 3, 2, -1, 2,
                          0.8000000119f, 0.5f, spawnFlags);
            break;
        case 6:
            func_15183974(8);
            func_15175390(count, group, x, y, z, maxRadius, player, heightRange, minRadius, 0x7D, 68, 2, 8, -1, 2,
                          0.8000000119f, 0.5f, spawnFlags);
            break;
        case 7:
            func_15183ACC(6);
            func_15175390(count, group, x, y, z, maxRadius, player, heightRange, minRadius, 0x4D, 20, 0, 6, 0, 2,
                          0.03999999911f, 0.02500000037f, spawnFlags);
            break;
        case 9:
            func_1517AA20(target->speed, target->xpos, target->ypos, target->zpos, count);
            break;
    }
    if (type != 9) {
        D_800DDF5C[group] = 1;
    }
    return group;
}

/* Loads model `slot` and, into slot + 1, its alternate vertex set (sharing slot's display list). */
void func_151838B0(s32 slot) {
    ModelResource *res;

    if (D_800DDE80[slot].resource == NULL) {
        func_15183ACC(slot);
    }
    if (D_800DDE80[slot + 1].resource == NULL) {
        res = func_1502B6BC(0, 0, 0, 2, 9, slot + 0xAE);
        if (res != NULL) {
            D_800DDE80[slot + 1].resource = res;
            D_800DDE80[slot + 1].unk4 = res->unk0;
            D_800DDE80[slot + 1].unk8 = res->unk8;
            D_800DDE80[slot + 1].unk10 = NULL;
            D_800DDE80[slot + 1].displayList = D_800DDE80[slot].displayList;
            D_800DDF68[slot + 1] = 1;
        }
    }
}

/* Loads models `slot` and slot + 1, giving slot + 1 slot's display list. */
void func_15183974(s32 slot) {
    if (D_800DDE80[slot].resource == NULL) {
        func_15183ACC(slot);
    }
    if (D_800DDE80[slot + 1].resource == NULL) {
        func_15183ACC(slot + 1);
        D_800DDE80[slot + 1].displayList = D_800DDE80[slot].displayList;
    }
}

/* Loads model `slot` and, once it is loaded, the vertex-only model `extraSlot`. */
void func_151839F0(s32 slot, s32 extraSlot) {
    ModelResource *res;

    if (D_800DDE80[slot].resource == NULL) {
        func_15183ACC(slot);
    }
    if (D_800DDE80[slot].resource != NULL && D_800DDE80[extraSlot].resource == NULL) {
        res = func_1502B6BC(0, 0, 0, 2, 9, extraSlot + 0xAD);
        if (res != NULL) {
            D_800DDE80[extraSlot].resource = res;
            D_800DDE80[extraSlot].unk4 = res->unk0;
            D_800DDE80[extraSlot].unk8 = res->unk8;
            D_800DDE80[extraSlot].displayList = res->unk18;
            D_800DDE80[extraSlot].unk10 = NULL;
            D_800DDF68[extraSlot] = 1;
        }
    }
}

/* Loads model `slot` (asset slot + 0xAD) if it isn't loaded yet. */
void func_15183ACC(s32 slot) {
    ModelResource *res;

    if (D_800DDE80[slot].resource == NULL) {
        res = func_1502B6BC(0, 0, 0, 2, 9, slot + 0xAD);
        if (res != NULL) {
            D_800DDE80[slot].resource = res;
            D_800DDE80[slot].unk4 = res->unk0;
            D_800DDE80[slot].unk8 = res->unk8;
            D_800DDE80[slot].displayList = res->unk18;
            D_800DDF78[slot] = res->unk10;
            func_1510CE60(D_800DDF78[slot], 0, 1, 0x3E, &D_800DDE80[slot].unk10);
            D_800DDF68[slot] = 1;
        }
    }
}

/* Appends a call to the second display list of the slot whose model id is `modelId`, if it is loaded. */
Gfx *func_15183BA4(Gfx *gdl, s32 modelId, s32 arg2, s32 arg3) {
    s32 slot;
    s32 i;

    slot = -1;
    for (i = 0; i < 11; i++) {
        if (modelId == D_800A72D0[i]) {
            slot = i;
            break;
        }
    }

    if (slot != -1 && D_800DDF78[slot] != NULL) {
        gSPDisplayList(gdl++, D_800DDF78[slot]);
    }
    return gdl;
}

/* Frees model `slot`. Slots 8 and 9 are a pair: slot 8's model is kept until slot 9 is unused too, and freeing
 * slot 9 after slot 8 also frees slot 8's model. */
void func_15183C28(s32 slot) {
    if (D_800DDE80[slot].unk10 != NULL) {
        func_1510D630(D_800DDE80[slot].unk10);
        D_800DDE80[slot].unk10 = NULL;
    }
    if (slot == 8) {
        if (D_800DDF68[9] != -2) {
            return;
        }
    } else if (slot == 9 && D_800DDF68[8] == -2 && D_800DDE80[8].resource != NULL) {
        // matching: as below, the int-to-pointer cast keeps the argument in v0 before the move to a0
        func_10004074((void *)(s32)D_800DDE80[8].resource);
        D_800DDE80[8].resource = NULL;
        D_800DDF78[8] = NULL;
    }
    if (D_800DDE80[slot].resource != NULL) {
        // matching: IDO reloads the argument into a0 directly unless it goes through an int-to-pointer cast
        func_10004074((void *)(s32)D_800DDE80[slot].resource);
        D_800DDE80[slot].resource = NULL;
        D_800DDF78[slot] = NULL;
    }
}

/* Per-frame update: ages the slots, animates the used ones and frees the ones no longer used. */
void func_15183D28(void) {
    s32 i;
    s32 value;
    Game1B0740Wave *wave;

    for (i = 0; i < 11; i++) {
        if (D_800DDF68[i] >= -1 && (D_800DDE80[i].resource == NULL || D_800DDF78[i] == NULL)) {
            D_800DDF68[i] = -2;
        }
    }
    for (i = 0; i < 4; i++) {
        if (D_800DDF60[i] > 0) {
            value = D_800DDF60[i] - D_800BE9E4;
            if (value < 0) {
                value = 0;
            }
            D_800DDF60[i] = value;
        }
    }
    for (i = 0; i < 7; i++) {
        if (D_800DDF68[i] == 1) {
            wave = &D_8008D0B0[i];
            func_15165C80(&wave->angle, D_800DDE80[i].unk8, wave->radius, &wave->velocity, wave->maxAngle,
                          wave->minAngle, D_8008D3C0[i], D_800A72E8[i]);
        }
    }
    for (i = 2; i < 5; i++) {
        if (D_800DDF68[i] == 1) {
            wave = &D_8008D318[i - 2];
            func_15165C80(&wave->angle, D_800DDE80[i].unk4, wave->radius, &wave->velocity, wave->maxAngle,
                          wave->minAngle, D_8008D3C0[i], D_800A72E8[i]);
        }
    }
    if (D_800DDF68[7] == 1) {
        func_1517725C(7);
    }
    for (i = 0; i < 2; i++) {
        if (D_800DDF68[i + 8] == 1) {
            func_15179DB0(i + 8);
        }
    }
    if (D_800DDF68[7] == 1) {
        D_800DDF68[10] = 1;
    }
    for (i = 0; i < 11; i++) {
        if (D_800DDF68[i] == -2 && D_800DDE80[i].resource != NULL) {
            func_15183C28(i);
        }
    }
    for (i = 0; i < 11; i++) {
        if (D_800DDF68[i] > 0) {
            D_800DDF68[i] = 0;
        } else if (D_800DDF68[i] >= -1) {
            D_800DDF68[i]--;
        }
    }
}

/* Returns whether `obj`'s mv data has unk57 set. */
s32 func_15184118(Actor *obj) {
    if (obj->mv != NULL && obj->mv->unk57 != 0) {
        return 1;
    }
    return 0;
}
