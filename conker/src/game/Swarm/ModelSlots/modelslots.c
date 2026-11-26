#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c); conker/swarm.h describes it. */
EffectModelSlot gEffectModelSlots[11];
u8 gSwarmGroupsInUse[4];
u8 gSwarmGroupSoundTimers[4];
s8 gEffectModelSlotStates[11];
Gfx *gEffectModelSecondDls[11];

#include "functions.h"
#include "variables.h"

/*
 * Effect model slots.
 *
 * gEffectModelSlots[11] holds one loaded model per slot (EffectModelSlot).
 * gEffectModelSlotStates[slot] is the slot's use state: 1 = used this frame, then counts down each frame;
 * -2 = unused, freed by swarm_updateModelSlots.  gEffectModelSecondDls[slot] is the model's second display list.
 * gSwarmGroupsInUse[4] marks the swarm groups handed out by swarm_spawnEffect.
 */

/* Model resource returned by the loader rom_loadPathTable. */
typedef struct ModelResource {
    /* 0x00 */ Vtx *unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ Vtx *unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ Gfx *unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ Gfx *unk18;
} ModelResource;

/* The model id of each slot (swarm_callModelSecondDl looks a model up here). */
static const u8 gEffectModelSlotIds[11] = { 10, 10, 21, 22, 23, 24, 20, 19, 68, 69, 19 };
/* The wing vertex indices that effect_swingVertices moves, per slot: left half, then right half. */
static const u8 D_800A72DC[2] = { 13, 16 };
static const u8 D_800A72E0[2] = { 1, 3 };
static const u8 D_800A72E4[4] = { 9, 10, 13, 16 };
/* The number of wing vertex indices per slot. */
static const u8 gSlotWingVertexCounts[7] = { 2, 2, 2, 2, 2, 2, 4 };

/* Per-slot motion parameters: this file wobbles the wings of slots 0..6 between minAngle and maxAngle. */
SwarmFlight gSwarmFlights[11] = {
    { 10, 8, 60, 60, 40.0f, 40.0f, 10.0f, 4.0f, 2.0f, { { 0.0f, 0.0f, 0.0f, 17.0f, 45.0f, -45.0f, 35.0f, 0.0f } } },
    { 12, 8, 60, 60, 40.0f, 40.0f, 10.0f, 4.0f, 2.0f, { { 0.0f, 0.0f, 0.0f, 15.0f, 45.0f, -45.0f, 35.0f, 0.0f } } },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, { { 0.0f, 0.0f, 0.0f, 10.0f, 75.0f, 10.0f, 49.0f, 0.0f } } },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, { { 0.0f, 0.0f, 0.0f, 9.0f, 75.0f, 10.0f, 49.0f, 0.0f } } },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, { { 0.0f, 0.0f, 0.0f, 8.0f, 75.0f, 10.0f, 49.0f, 0.0f } } },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, { { 0.0f, 0.0f, 0.0f, 10.0f, 75.0f, 10.0f, 49.0f, 0.0f } } },
    { 4, 1, 20, 20, 40.0f, 40.0f, 10.0f, 4.0f, 2.0f, { { 0.0f, 0.0f, 0.0f, 10.0f, 20.0f, -20.0f, 471.0f, 0.0f } } },
    { 10, 4, 60, 60, 40.0f, 40.0f, 10.0f, 4.0f, 2.0f, { { 0.0f, 35.0f, 17.5f, 12.5f, 45.0f, -45.0f, 29.0f, 72.0f } } },
    { 2, 1, 120, 120, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, { { 10.0f, 40.0f, 0.0f, 0.0f, 21.0f, 5.0f, 25.0f, 0.0f } } },
    { 2, 1, 120, 120, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, { { 10.0f, 40.0f, 0.0f, 0.0f, 21.0f, 5.0f, 25.0f, 0.0f } } },
    { 6, 2, 60, 60, 40.0f, 40.0f, 10.0f, 4.0f, 2.0f, { { 0.0f, 35.0f, 17.5f, 12.5f, 45.0f, -45.0f, 29.0f, 72.0f } } },
};

/* The wobble of the second vertex set of slots 2..4. */
static SwarmFlight gSwarmAltWobbles[3] = {
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, { { 0.0f, 0.0f, 0.0f, 0.5f, 75.0f, 40.0f, 49.0f, 0.0f } } },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, { { 0.0f, 0.0f, 0.0f, 0.400000006f, 75.0f, 40.0f, 49.0f, 0.0f } } },
    { 2, 1, 10, 10, 40.0f, 40.0f, 5.0f, 8.0f, 4.0f, { { 0.0f, 0.0f, 0.0f, 0.3000000119f, 75.0f, 40.0f, 49.0f, 0.0f } } },
};

/* The wing vertex indices of each wobbling slot. */
static const u8 *gSlotWingVertices[7] = {
    D_800A72DC, D_800A72DC, D_800A72E0, D_800A72E0, D_800A72E0, D_800A72E0, D_800A72E4,
};

/*
 * Spawns effect `type` at (x, y, z) for `player`, loading the models it needs.
 * Returns the swarm group used, or -1 if no group is free or the effect can't start.
 */
s32 swarm_spawnEffect(s32 x, s32 y, s32 z, s32 count, s32 type, s32 radius, s32 player, s32 maxRadius,
                  s32 heightRange, s32 minRadius, s32 spawnFlags) {
    s32 group;
    Actor *target;

    for (group = 0; group < 4; group++) {
        if (gSwarmGroupsInUse[group] == 0) {
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
            swarm_createSwarm(count, NULL, 0xFE, x, y, z, 0, 0x7FFF, 0xFF, 1, 0);
            break;
        case 1:
            swarm_createSwarm(count, &movement_obj[0].xpos, 0xFE, x, y, z, 0, 0x7FFF, 0xFF, 1, 0);
            break;
        case 2:
            swarm_loadModelWithAltVerts(0);
            swarm_spawnRing(count, group, x, y, z, 63, player, 30, 0, 0x30003, 10, 0, 0, 1, 2, 0.8000000119f, 0.5f,
                          spawnFlags);
            break;
        case 3:
            if (func_15184118(target)) {
                swarm_spawnAwayFromPlayer(count, group, x, y, z, player, 0.8000000119f, 0.5f, spawnFlags);
                break;
            }
            return -1;
        case 8:
            return -1;
        case 4:
            swarm_loadModelWithExtra(7, 10);
            swarm_spawnScattered(count, group, x, y, z, maxRadius * 4, player, 0, 0.8000000119f, 0.5f);
            break;
        case 10:
            swarm_loadModel(7);
            swarm_spawnScattered(count, group, x, y, z, maxRadius * 4, player, 1, 0.8000000119f, 0.5f);
            break;
        case 11:
            swarm_loadModel(2);
            swarm_loadModel(3);
            swarm_loadModel(4);
            swarm_loadModel(5);
            swarm_spawnRing(count, group, x, y, z, maxRadius, player, heightRange, minRadius, 0x4D, 21, 3, 2, -1, 0,
                          0.8000000119f, 0.5f, spawnFlags);
            break;
        case 5:
            swarm_loadModel(2);
            swarm_loadModel(3);
            swarm_loadModel(4);
            swarm_loadModel(5);
            swarm_spawnRing(count, group, x, y, z, maxRadius, player, heightRange, minRadius, 0x4D, 21, 3, 2, -1, 2,
                          0.8000000119f, 0.5f, spawnFlags);
            break;
        case 6:
            swarm_loadModelPair(8);
            swarm_spawnRing(count, group, x, y, z, maxRadius, player, heightRange, minRadius, 0x7D, 68, 2, 8, -1, 2,
                          0.8000000119f, 0.5f, spawnFlags);
            break;
        case 7:
            swarm_loadModel(6);
            swarm_spawnRing(count, group, x, y, z, maxRadius, player, heightRange, minRadius, 0x4D, 20, 0, 6, 0, 2,
                          0.03999999911f, 0.02500000037f, spawnFlags);
            break;
        case 9:
            particle_spawnGroundChunks(target->speed, target->xpos, target->ypos, target->zpos, count);
            break;
    }
    if (type != 9) {
        gSwarmGroupsInUse[group] = 1;
    }
    return group;
}

/* Loads model `slot` and, into slot + 1, its alternate vertex set (sharing slot's display list). */
void swarm_loadModelWithAltVerts(s32 slot) {
    ModelResource *res;

    if (gEffectModelSlots[slot].resource == NULL) {
        swarm_loadModel(slot);
    }
    if (gEffectModelSlots[slot + 1].resource == NULL) {
        res = rom_loadPathTable(0, 0, 0, 2, 9, slot + 0xAE);
        if (res != NULL) {
            gEffectModelSlots[slot + 1].resource = res;
            gEffectModelSlots[slot + 1].unk4 = res->unk0;
            gEffectModelSlots[slot + 1].unk8 = res->unk8;
            gEffectModelSlots[slot + 1].unk10 = NULL;
            gEffectModelSlots[slot + 1].displayList = gEffectModelSlots[slot].displayList;
            gEffectModelSlotStates[slot + 1] = 1;
        }
    }
}

/* Loads models `slot` and slot + 1, giving slot + 1 slot's display list. */
void swarm_loadModelPair(s32 slot) {
    if (gEffectModelSlots[slot].resource == NULL) {
        swarm_loadModel(slot);
    }
    if (gEffectModelSlots[slot + 1].resource == NULL) {
        swarm_loadModel(slot + 1);
        gEffectModelSlots[slot + 1].displayList = gEffectModelSlots[slot].displayList;
    }
}

/* Loads model `slot` and, once it is loaded, the vertex-only model `extraSlot`. */
void swarm_loadModelWithExtra(s32 slot, s32 extraSlot) {
    ModelResource *res;

    if (gEffectModelSlots[slot].resource == NULL) {
        swarm_loadModel(slot);
    }
    if (gEffectModelSlots[slot].resource != NULL && gEffectModelSlots[extraSlot].resource == NULL) {
        res = rom_loadPathTable(0, 0, 0, 2, 9, extraSlot + 0xAD);
        if (res != NULL) {
            gEffectModelSlots[extraSlot].resource = res;
            gEffectModelSlots[extraSlot].unk4 = res->unk0;
            gEffectModelSlots[extraSlot].unk8 = res->unk8;
            gEffectModelSlots[extraSlot].displayList = res->unk18;
            gEffectModelSlots[extraSlot].unk10 = NULL;
            gEffectModelSlotStates[extraSlot] = 1;
        }
    }
}

/* Loads model `slot` (asset slot + 0xAD) if it isn't loaded yet. */
void swarm_loadModel(s32 slot) {
    ModelResource *res;

    if (gEffectModelSlots[slot].resource == NULL) {
        res = rom_loadPathTable(0, 0, 0, 2, 9, slot + 0xAD);
        if (res != NULL) {
            gEffectModelSlots[slot].resource = res;
            gEffectModelSlots[slot].unk4 = res->unk0;
            gEffectModelSlots[slot].unk8 = res->unk8;
            gEffectModelSlots[slot].displayList = res->unk18;
            gEffectModelSecondDls[slot] = res->unk10;
            texture_resolveDl(gEffectModelSecondDls[slot], 0, 1, 0x3E, &gEffectModelSlots[slot].unk10);
            gEffectModelSlotStates[slot] = 1;
        }
    }
}

/* Appends a call to the second display list of the slot whose model id is `modelId`, if it is loaded. */
Gfx *swarm_callModelSecondDl(Gfx *gdl, s32 modelId, s32 arg2, s32 arg3) {
    s32 slot;
    s32 i;

    slot = -1;
    for (i = 0; i < 11; i++) {
        if (modelId == gEffectModelSlotIds[i]) {
            slot = i;
            break;
        }
    }

    if (slot != -1 && gEffectModelSecondDls[slot] != NULL) {
        gSPDisplayList(gdl++, gEffectModelSecondDls[slot]);
    }
    return gdl;
}

/* Frees model `slot`. Slots 8 and 9 are a pair: slot 8's model is kept until slot 9 is unused too, and freeing
 * slot 9 after slot 8 also frees slot 8's model. */
void swarm_freeModel(s32 slot) {
    if (gEffectModelSlots[slot].unk10 != NULL) {
        texture_releaseList(gEffectModelSlots[slot].unk10);
        gEffectModelSlots[slot].unk10 = NULL;
    }
    if (slot == 8) {
        if (gEffectModelSlotStates[9] != -2) {
            return;
        }
    } else if (slot == 9 && gEffectModelSlotStates[8] == -2 && gEffectModelSlots[8].resource != NULL) {
        // matching: as below, the int-to-pointer cast keeps the argument in v0 before the move to a0
        heap_free((void *)(s32)gEffectModelSlots[8].resource);
        gEffectModelSlots[8].resource = NULL;
        gEffectModelSecondDls[8] = NULL;
    }
    if (gEffectModelSlots[slot].resource != NULL) {
        // matching: IDO reloads the argument into a0 directly unless it goes through an int-to-pointer cast
        heap_free((void *)(s32)gEffectModelSlots[slot].resource);
        gEffectModelSlots[slot].resource = NULL;
        gEffectModelSecondDls[slot] = NULL;
    }
}

/* Per-frame update: ages the slots, animates the used ones and frees the ones no longer used. */
void swarm_updateModelSlots(void) {
    s32 i;
    s32 value;
    SwarmFlight *wave;

    for (i = 0; i < 11; i++) {
        if (gEffectModelSlotStates[i] >= -1 && (gEffectModelSlots[i].resource == NULL || gEffectModelSecondDls[i] == NULL)) {
            gEffectModelSlotStates[i] = -2;
        }
    }
    for (i = 0; i < 4; i++) {
        if (gSwarmGroupSoundTimers[i] > 0) {
            value = gSwarmGroupSoundTimers[i] - D_800BE9E4;
            if (value < 0) {
                value = 0;
            }
            gSwarmGroupSoundTimers[i] = value;
        }
    }
    for (i = 0; i < 7; i++) {
        if (gEffectModelSlotStates[i] == 1) {
            wave = &gSwarmFlights[i];
            effect_swingVertices(&wave->u.wobble.angle, gEffectModelSlots[i].unk8, wave->u.wobble.radius, &wave->u.wobble.velocity, wave->u.wobble.maxAngle,
                          wave->u.wobble.minAngle, gSlotWingVertices[i], gSlotWingVertexCounts[i]);
        }
    }
    for (i = 2; i < 5; i++) {
        if (gEffectModelSlotStates[i] == 1) {
            wave = &gSwarmAltWobbles[i - 2];
            effect_swingVertices(&wave->u.wobble.angle, gEffectModelSlots[i].unk4, wave->u.wobble.radius, &wave->u.wobble.velocity, wave->u.wobble.maxAngle,
                          wave->u.wobble.minAngle, gSlotWingVertices[i], gSlotWingVertexCounts[i]);
        }
    }
    if (gEffectModelSlotStates[7] == 1) {
        swarm_flapWings(7);
    }
    for (i = 0; i < 2; i++) {
        if (gEffectModelSlotStates[i + 8] == 1) {
            func_15179DB0(i + 8);
        }
    }
    if (gEffectModelSlotStates[7] == 1) {
        gEffectModelSlotStates[10] = 1;
    }
    for (i = 0; i < 11; i++) {
        if (gEffectModelSlotStates[i] == -2 && gEffectModelSlots[i].resource != NULL) {
            swarm_freeModel(i);
        }
    }
    for (i = 0; i < 11; i++) {
        if (gEffectModelSlotStates[i] > 0) {
            gEffectModelSlotStates[i] = 0;
        } else if (gEffectModelSlotStates[i] >= -1) {
            gEffectModelSlotStates[i]--;
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
