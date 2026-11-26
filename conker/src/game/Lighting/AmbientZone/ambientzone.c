#include <ultra64.h>
#include "structs.h"

/* This unit's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
f32 D_800DCD90;
f32 D_800DCD94;
f32 D_800DCD98;
f32 D_800DCD9C;
f32 D_800DCDA0;
f32 D_800DCDA4;
f32 D_800DCDA8;
f32 D_800DCDAC;
f32 D_800DCDB0;
f32 D_800DCDB4;
f32 D_800DCDB8;
f32 D_800DCDBC;
f32 D_800DCDC0;
RegionNode *D_800DCDC4;

#include "functions.h"
#include "variables.h"

/* A node of a list of spawn regions with their cached volume and area. */
RegionNode *func_1515F1B0(region)
    TriggerZone *region;
{
    RegionNode *node;

    node = heap_alloc(sizeof(RegionNode), 1, 2, 0, 1);
    if (node == NULL) {
        return NULL;
    }
    node->region = region;
    if (region != NULL) {
        node->volume = math_getZoneVolume(region);
    } else {
        node->volume = 0.0f;
    }
    if (region != NULL) {
        node->area = math_getZoneArea(region);
    } else {
        node->area = 0.0f;
    }
    node->next = NULL;
    return node;
}

/* Pushes `node` onto the front of `list`. */
void func_1515F25C(RegionNode **list, RegionNode *node) {
    node->next = *list;
    *list = node;
}

typedef void (*ZoneHandler)(Actor *obj, TriggerZone *zone);

/* TriggerZone handlers per zone->data1. Declared without a prototype: func_150AF2E0 reads the zone
 * through its own view. */
static void (*D_8008B090[12])() = {
    func_1515F2B8, func_1515F2E8, func_1515FB70, func_150AF2E0, func_151A6B68, func_1515FB94,
    func_1515FC60, func_1515FC34, func_1515FBC4, func_150F884C, NULL,          effect_setZoneFlag,
};

/* Runs zone handler zone->data1 for `obj`. */
void func_1515F270(Actor *obj, TriggerZone *zone) {
    s32 idx = zone->data1;

    if (idx >= 0 && idx < 12) {
        if (D_8008B090[idx] != NULL) {
            D_8008B090[idx](obj, zone);
        }
    }
}

void func_1515F2B8(Actor *obj, TriggerZone *zone) {
    actor_hitScripted(obj, 0x6001D, obj->langle, -1);
}

/* Ambient effects per zone->data2 */
static ZoneHandler gAmbientZoneEffects[3] = { func_1515F338, func_1515F5C4, func_1515F850 };

/* TriggerZone handler: when the player (unique id 1) is in the zone, runs ambient effect zone->data2. */
void func_1515F2E8(Actor *obj, TriggerZone *zone) {
    s32 idx;

    if (obj->unique_id == 1) {
        idx = zone->data2;
        if (idx >= 0 && idx < 3) {
            gAmbientZoneEffects[idx](obj, zone);
        }
    }
}

/* Ambient effect 0: eases the ambient colour halfway towards (97, 96, 98) each frame. */
void func_1515F338(Actor *obj, TriggerZone *zone) {
    u8 red;
    u8 green;
    u8 blue;

    red = gAmbientColour[0];
    green = gAmbientColour[1];
    blue = gAmbientColour[2];
    if (D_800DCD94 != 97.0f) {
        D_800DCD94 += (97.0f - D_800DCD94) * 0.5f;
        red = (u32)D_800DCD94;
    }
    if (D_800DCD98 != 96.0f) {
        D_800DCD98 += (96.0f - D_800DCD98) * 0.5f;
        green = (u32)D_800DCD98;
    }
    if (D_800DCD9C != 98.0f) {
        D_800DCD9C += (98.0f - D_800DCD9C) * 0.5f;
        blue = (u32)D_800DCD9C;
    }
    light_setAmbient(red, green, blue, 0);
}

/* Ambient effect 1: eases the ambient colour halfway towards (229, 253, 160) each frame. */
void func_1515F5C4(Actor *obj, TriggerZone *zone) {
    u8 red;
    u8 green;
    u8 blue;

    red = gAmbientColour[0];
    green = gAmbientColour[1];
    blue = gAmbientColour[2];
    if (D_800DCD94 != 229.0f) {
        D_800DCD94 += (229.0f - D_800DCD94) * 0.5f;
        red = (u32)D_800DCD94;
    }
    if (D_800DCD98 != 253.0f) {
        D_800DCD98 += (253.0f - D_800DCD98) * 0.5f;
        green = (u32)D_800DCD98;
    }
    if (D_800DCD9C != 160.0f) {
        D_800DCD9C += (160.0f - D_800DCD9C) * 0.5f;
        blue = (u32)D_800DCD9C;
    }
    light_setAmbient(red, green, blue, 0);
}

/* Ambient effect 2: eases the ambient colour towards a colour pulsing with sin(D_800DCDA0). */
void func_1515F850(Actor *obj, TriggerZone *zone) {
    f32 s;
    f32 targetR;
    f32 targetG;
    f32 targetB;
    u8 color[3];

    color[0] = gAmbientColour[0];
    color[1] = gAmbientColour[1];
    color[2] = gAmbientColour[2];
    s = sinf(D_800DCDA0);
    targetR = (s * 29.5f) + 127.5f;
    targetG = (s * 71.0f) + 109.0f;
    targetB = (s * 26.0f) + 26.0f;
    D_800DCDA0 += 0.07100000232f * gFrameDelta;
    D_800DCDA0 = math_wrapRadians(D_800DCDA0);
    if (targetR != D_800DCD94) {
        D_800DCD94 += (targetR - D_800DCD94) * 0.5f;
        color[0] = (u32)D_800DCD94;
    }
    if (targetG != D_800DCD98) {
        D_800DCD98 += (targetG - D_800DCD98) * 0.5f;
        color[1] = (u32)D_800DCD98;
    }
    if (targetB != D_800DCD9C) {
        D_800DCD9C += (targetB - D_800DCD9C) * 0.5f;
        color[2] = (u32)D_800DCD9C;
    }
    light_setAmbient(color[0], color[1], color[2], 0);
}

/* TriggerZone handler: func_1515F2E8 for a handler table that is empty in this build. */
void func_1515FB70(Actor *obj, TriggerZone *zone) {
    s32 idx;

    if (obj->unique_id == 1) {
        idx = zone->data2;
        if (idx >= 0 && idx < 0) {
        }
    }
}

void func_1515FB94(Actor *obj, TriggerZone *zone) {
    actor_hitScripted(obj, 0x6002D, obj->langle, -1);
}

/* TriggerZone handler: like func_1515FB94, also passing the index of the object linked to this zone (or -1). */
void func_1515FBC4(Actor *obj, TriggerZone *zone) {
    ZoneRectEffect *link;
    Actor *target;
    s32 targetIndex;

    target = NULL;
    link = beam_findZoneRectEffect(zone);
    if (link != NULL) {
        target = link->data.actor;
    }
    if (target != NULL) {
        targetIndex = target - movement_obj;
    } else {
        targetIndex = -1;
    }
    actor_hitScripted(obj, 0x6002E, obj->langle, targetIndex);
}

void func_1515FC34(Actor *obj, TriggerZone *zone) {
    actor_hitScripted(obj, 0x33, 0xC000, -1);
}

/*
 * TriggerZone handler: unless the zone has a range (data2, in hundredths of its radius) that `obj`
 * is outside of, fires event 0x2F on `obj`.
 */
void func_1515FC60(Actor *obj, TriggerZone *zone) {
    s32 pad; // matching: stack slot
    f32 limit;
    f32 scale;
    s32 value;

    if (obj->mv != NULL && obj->mv->death_State == 0) {
        value = zone->data2;
        if (value != 0) {
            scale = (u32)value * 0.009999999776f;
            value = func_150AD960(zone->x, zone->z, obj->xpos, obj->zpos);
            limit = (zone->xSize * scale) - obj->bodyRadius;
            if (limit > value) {
                return;
            }
        }
        actor_hitScripted(obj, 0x2F, 0, -1);
        blood_spawnHitEffects(obj, 1, 1, 1, 1, 0, 0xFF, 1);
        func_15145A50(obj);
        actor_kill(obj, 6);
    }
}
