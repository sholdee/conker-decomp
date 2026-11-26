#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-effect data (at 0x28) of the spark effect that follows an object. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 sparkAccum; /* fractional count of beam_spawnBoundsBolt particles still to spawn */
    /* 0x0C */ f32 unkCAccum;  /* fractional count of func_15107B78 particles still to spawn */
} OwnerSparkData; /* size 0x10 */

/* A TimedEffect effect, as created by effect_newTimedEffect, carrying OwnerSparkData at 0x28. */
struct OwnerSparkEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ OwnerSparkData data;
};

/* Spawns a spark effect attached to `owner`; a duration of -1 means 300 frames, not looping. */
OwnerSparkEffect *effect_spawnOwnerSpark(Actor *owner, s16 duration, u8 arg2, s32 arg3) {
    OwnerSparkEffect *effect;
    OwnerSparkData data;

    if (owner == NULL) {
        return NULL;
    }

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.sparkAccum = 0.0f;
    data.unkCAccum = 0.0f;

    // matching: `duration != -1` compiles to sltu/xori; IDO only emits this branchy form for ?:
    effect = effect_newTimedEffect((duration == -1) ? 300 : duration, -1, 0x24, -1,
                                               (duration == -1) ? 0 : 1, 0x23, 0x10, arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
        effect_acquireAnchorSet(6);
    }
    return effect;
}

/* Per-frame update of a spark effect: ends it once the owner is gone, else spawns sparks around it. */
void effect_updateOwnerSpark(OwnerSparkEffect *effect) {
    OwnerSparkData *data;
    Actor *owner;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->type == 0) || (owner->unique_id != data->ownerId) || (owner->character_type == 0xFF)) {
        effect->base.header.state = -1;
        return;
    }
    actor_flashFog(owner, 0xFF, 0xFF, 0xFF, 30, 0, 10);
    data->sparkAccum += (0.2560000122f + (math_randomFloat() * 0.2960000038f)) * gFrameDelta;
    while (data->sparkAccum > 1.0f) {
        u8 colour[4];

        colour[0] = 0xBA;
        colour[1] = 0xD2;
        colour[2] = 0xFF;
        colour[3] = (math_random() % 126U) + 130;
        beam_spawnBoundsBolt(owner, math_random() & 0xFF, (math_random() % 101U) - 63,
                      (math_random() & 0xF) + 15, 4, 129.0f, (math_randomFloat() * 75.0f) + 35.0f, 0, 1,
                      colour, effect->base.header.owner, effect->base.header.kind);
        data->sparkAccum -= 1.0f;
    }
    data->unkCAccum += (0.2090000063f + (math_randomFloat() * 0.1840000153f)) * gFrameDelta;
    while (data->unkCAccum > 1.0f) {
        func_15107B78(owner, math_random() & 0xFF, (math_random() % 101U) - 63, effect->base.header.owner,
                      effect->base.header.kind);
        data->unkCAccum -= 1.0f;
    }
}

void effect_releaseOwnerSparkAnchors(OwnerSparkEffect *effect) {
    effect_releaseAnchorSet(6);
}

void effect_destroyOwnerSpark(OwnerSparkEffect *effect) {
    effect_releaseOwnerSparkAnchors(effect);
    effect_defaultDestroyTimedEffect(&effect->base);
}

void effect_freeOwnerSpark(OwnerSparkEffect *effect) {
    effect_releaseOwnerSparkAnchors(effect);
    effect_defaultFreeTimedEffect(&effect->base);
}

/* Event handler of a spark effect: event 0 destroys it when it concerns its owner, event 0x2D swaps
 * its owner. */
void effect_ownerSparkMessage(OwnerSparkEffect *effect, ActorMsg *event, u8 eventId) {
    OwnerSparkData *data = &effect->data;
    Actor *obj;

    if (eventId == 0) {
        obj = event->ref.actor; // matching: loading the pointer first puts it in v1 ahead of data->owner
        if ((obj == data->owner) || (event->ref.uniqueId == data->ownerId)) {
            effect_destroyObject(&effect->base.header);
        }
    } else if (eventId == 0x2D) {
        if (data->owner == event->swap.actorA) {
            data->owner = event->swap.actorB;
            data->ownerId = event->swap.idB;
        } else if (data->owner == event->swap.actorB) {
            data->owner = event->swap.actorA;
            data->ownerId = event->swap.idA;
        }
    }
}
