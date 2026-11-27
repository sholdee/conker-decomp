#include <ultra64.h>
#include "structs.h"

/* This unit's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
f32 gDustRegionsArea;
f32 gAmbientEaseRed;
f32 gAmbientEaseGreen;
f32 gAmbientEaseBlue;
f32 gAmbientEasePhase;
f32 gSavedAmbientRed;
f32 gSavedAmbientGreen;
f32 gSavedAmbientBlue;
f32 gSavedDirLightAmbientTintRed;
f32 gSavedDirLightAmbientTintGreen;
f32 gSavedDirLightAmbientTintBlue;
f32 gSavedDirLightAmount;
f32 gSavedDirLightAmbientAmount;
RegionNode *gDustRegions;

#include "effects.h"
#include <math.h>
#include "conker/actor.h"
#include "conker/beam.h"
#include "conker/blood.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/heap.h"
#include "conker/light.h"
#include "conker/particle.h"

/* A node of a list of spawn regions with their cached volume and area. */
RegionNode *light_newRegionNode(region)
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
void light_pushRegionNode(RegionNode **list, RegionNode *node) {
    node->next = *list;
    *list = node;
}

typedef void (*ZoneHandler)(Actor *obj, TriggerZone *zone);

/* TriggerZone handlers per zone->data1. Declared without a prototype: particle_spawnZoneGroundFxNearCamera reads the zone
 * through its own view. */
static void (*gHandlerZoneHandlers[12])() = {
    func_1515F2B8, light_runAmbientZoneEffect, func_1515FB70, particle_spawnZoneGroundFxNearCamera, func_151A6B68, func_1515FB94,
    light_killActorInZone, func_1515FC34, func_1515FBC4, effect_stageZoneHandler, NULL,          effect_setZoneFlag,
};

/* Runs zone handler zone->data1 for `obj`. */
void light_runHandlerZone(Actor *obj, TriggerZone *zone) {
    s32 idx = zone->data1;

    if (idx >= 0 && idx < 12) {
        if (gHandlerZoneHandlers[idx] != NULL) {
            gHandlerZoneHandlers[idx](obj, zone);
        }
    }
}

void func_1515F2B8(Actor *obj, TriggerZone *zone) {
    actor_hitScripted(obj, 0x6001D, obj->langle, -1);
}

/* Ambient effects per zone->data2 */
static ZoneHandler gAmbientZoneEffects[3] = { light_easeAmbientToGrey, light_easeAmbientToYellowGreen, light_easeAmbientToPulse };

/* TriggerZone handler: when the player (unique id 1) is in the zone, runs ambient effect zone->data2. */
void light_runAmbientZoneEffect(Actor *obj, TriggerZone *zone) {
    s32 idx;

    if (obj->unique_id == 1) {
        idx = zone->data2;
        if (idx >= 0 && idx < 3) {
            gAmbientZoneEffects[idx](obj, zone);
        }
    }
}

/* Ambient effect 0: eases the ambient colour halfway towards (97, 96, 98) each frame. */
void light_easeAmbientToGrey(Actor *obj, TriggerZone *zone) {
    u8 red;
    u8 green;
    u8 blue;

    red = gAmbientColour[0];
    green = gAmbientColour[1];
    blue = gAmbientColour[2];
    if (gAmbientEaseRed != 97.0f) {
        gAmbientEaseRed += (97.0f - gAmbientEaseRed) * 0.5f;
        red = (u32)gAmbientEaseRed;
    }
    if (gAmbientEaseGreen != 96.0f) {
        gAmbientEaseGreen += (96.0f - gAmbientEaseGreen) * 0.5f;
        green = (u32)gAmbientEaseGreen;
    }
    if (gAmbientEaseBlue != 98.0f) {
        gAmbientEaseBlue += (98.0f - gAmbientEaseBlue) * 0.5f;
        blue = (u32)gAmbientEaseBlue;
    }
    light_setAmbient(red, green, blue, 0);
}

/* Ambient effect 1: eases the ambient colour halfway towards (229, 253, 160) each frame. */
void light_easeAmbientToYellowGreen(Actor *obj, TriggerZone *zone) {
    u8 red;
    u8 green;
    u8 blue;

    red = gAmbientColour[0];
    green = gAmbientColour[1];
    blue = gAmbientColour[2];
    if (gAmbientEaseRed != 229.0f) {
        gAmbientEaseRed += (229.0f - gAmbientEaseRed) * 0.5f;
        red = (u32)gAmbientEaseRed;
    }
    if (gAmbientEaseGreen != 253.0f) {
        gAmbientEaseGreen += (253.0f - gAmbientEaseGreen) * 0.5f;
        green = (u32)gAmbientEaseGreen;
    }
    if (gAmbientEaseBlue != 160.0f) {
        gAmbientEaseBlue += (160.0f - gAmbientEaseBlue) * 0.5f;
        blue = (u32)gAmbientEaseBlue;
    }
    light_setAmbient(red, green, blue, 0);
}

/* Ambient effect 2: eases the ambient colour towards a colour pulsing with sin(gAmbientEasePhase). */
void light_easeAmbientToPulse(Actor *obj, TriggerZone *zone) {
    f32 s;
    f32 targetR;
    f32 targetG;
    f32 targetB;
    u8 color[3];

    color[0] = gAmbientColour[0];
    color[1] = gAmbientColour[1];
    color[2] = gAmbientColour[2];
    s = sinf(gAmbientEasePhase);
    targetR = (s * 29.5f) + 127.5f;
    targetG = (s * 71.0f) + 109.0f;
    targetB = (s * 26.0f) + 26.0f;
    gAmbientEasePhase += 0.07100000232f * gFrameDelta;
    gAmbientEasePhase = math_wrapRadians(gAmbientEasePhase);
    if (targetR != gAmbientEaseRed) {
        gAmbientEaseRed += (targetR - gAmbientEaseRed) * 0.5f;
        color[0] = (u32)gAmbientEaseRed;
    }
    if (targetG != gAmbientEaseGreen) {
        gAmbientEaseGreen += (targetG - gAmbientEaseGreen) * 0.5f;
        color[1] = (u32)gAmbientEaseGreen;
    }
    if (targetB != gAmbientEaseBlue) {
        gAmbientEaseBlue += (targetB - gAmbientEaseBlue) * 0.5f;
        color[2] = (u32)gAmbientEaseBlue;
    }
    light_setAmbient(color[0], color[1], color[2], 0);
}

/* TriggerZone handler: light_runAmbientZoneEffect for a handler table that is empty in this build. */
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
void light_killActorInZone(Actor *obj, TriggerZone *zone) {
    s32 pad; // matching: stack slot
    f32 limit;
    f32 scale;
    s32 value;

    if (obj->mv != NULL && obj->mv->death_State == 0) {
        value = zone->data2;
        if (value != 0) {
            scale = (u32)value * 0.009999999776f;
            value = math_distance2D(zone->x, zone->z, obj->xpos, obj->zpos);
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
