#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-effect data (at 0x28) of the spark effect that follows an object. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 sparkAccum; /* fractional count of func_15107700 particles still to spawn */
    /* 0x0C */ f32 unkCAccum;  /* fractional count of func_15107B78 particles still to spawn */
} OwnerSparkData; /* size 0x10 */

/* A TimedEffect effect, as created by func_15149130, carrying OwnerSparkData at 0x28. */
struct OwnerSparkEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ OwnerSparkData data;
};

/* Spawns a spark effect attached to `owner`; a duration of -1 means 300 frames, not looping. */
OwnerSparkEffect *func_150B53D0(Actor *owner, s16 duration, u8 arg2, s32 arg3) {
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
    effect = func_15149130((duration == -1) ? 300 : duration, -1, 0x24, -1,
                                               (duration == -1) ? 0 : 1, 0x23, 0x10, arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
        func_151D2AB0(6);
    }
    return effect;
}

/* Per-frame update of a spark effect: ends it once the owner is gone, else spawns sparks around it. */
void func_150B54A8(OwnerSparkEffect *effect) {
    OwnerSparkData *data;
    Actor *owner;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->type == 0) || (owner->unique_id != data->ownerId) || (owner->character_type == 0xFF)) {
        effect->base.header.state = -1;
        return;
    }
    func_1502EA98(owner, 0xFF, 0xFF, 0xFF, 30, 0, 10);
    data->sparkAccum += (0.2560000122f + (func_150ADA68() * 0.2960000038f)) * D_800BE9A4;
    while (data->sparkAccum > 1.0f) {
        u8 colour[4];

        colour[0] = 0xBA;
        colour[1] = 0xD2;
        colour[2] = 0xFF;
        colour[3] = (func_150ADA20() % 126U) + 130;
        func_15107700(owner, func_150ADA20() & 0xFF, (func_150ADA20() % 101U) - 63,
                      (func_150ADA20() & 0xF) + 15, 4, 129.0f, (func_150ADA68() * 75.0f) + 35.0f, 0, 1,
                      colour, effect->base.header.owner, effect->base.header.kind);
        data->sparkAccum -= 1.0f;
    }
    data->unkCAccum += (0.2090000063f + (func_150ADA68() * 0.1840000153f)) * D_800BE9A4;
    while (data->unkCAccum > 1.0f) {
        func_15107B78(owner, func_150ADA20() & 0xFF, (func_150ADA20() % 101U) - 63, effect->base.header.owner,
                      effect->base.header.kind);
        data->unkCAccum -= 1.0f;
    }
}

void func_150B57C4(OwnerSparkEffect *effect) {
    func_151D2B4C(6);
}

void func_150B57E8(OwnerSparkEffect *effect) {
    func_150B57C4(effect);
    func_1514933C(&effect->base);
}

void func_150B5814(OwnerSparkEffect *effect) {
    func_150B57C4(effect);
    func_15149368(&effect->base);
}

/* Event handler of a spark effect: event 0 destroys it when it concerns its owner, event 0x2D swaps
 * its owner. */
void func_150B5840(OwnerSparkEffect *effect, ActorMsg *event, u8 eventId) {
    OwnerSparkData *data = &effect->data;
    Actor *obj;

    if (eventId == 0) {
        obj = event->ref.actor; // matching: loading the pointer first puts it in v1 ahead of data->owner
        if ((obj == data->owner) || (event->ref.uniqueId == data->ownerId)) {
            func_1516972C(&effect->base.header);
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
