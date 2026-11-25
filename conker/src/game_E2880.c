#include <ultra64.h>
/* include/functions.h declares func_150ADA20 (a random number) as returning u8, but
 * func_150B54A8 feeds its result straight into `divu` without zero-extending it, so it
 * really returns a word. The shared header is left alone; the right prototype is local. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

extern void func_151D2AB0(s32);

/* Per-effect data (at 0x28) of the spark effect that follows an object. */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 sparkAccum; /* fractional count of func_15107700 particles still to spawn */
    /* 0x0C */ f32 unkCAccum;  /* fractional count of func_15107B78 particles still to spawn */
} SparkEffectData; /* size 0x10 */

/* A struct260 effect, as created by func_15149130, carrying SparkEffectData at 0x28. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 state; /* -1 ends the effect */
    /* 0x10 */ u8 pad10[0x28 - 0x10];
    /* 0x28 */ SparkEffectData data;
} SparkEffect;

/* Spawns a spark effect attached to `owner`; a duration of -1 means 300 frames, not looping. */
SparkEffect *func_150B53D0(struct127 *owner, s16 duration, u8 arg2, s32 arg3) {
    SparkEffect *effect;
    SparkEffectData data;

    if (owner == NULL) {
        return NULL;
    }

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.sparkAccum = 0.0f;
    data.unkCAccum = 0.0f;

    // matching: `duration != -1` compiles to sltu/xori; IDO only emits this branchy form for ?:
    effect = (SparkEffect *)func_15149130((duration == -1) ? 300 : duration, -1, 0x24, -1, (duration == -1) ? 0 : 1, 0x23,
                                          (struct37 *)0x10, arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
        func_151D2AB0(6);
    }
    return effect;
}

extern void func_1502EA98(struct127 *, u8, u8, u8, u8, s32, u8);
extern void func_15107700(struct127 *, s16, s16, s16, s32, f32, f32, s16, s16, u8 *, u8, s32);
extern void func_15107B78(struct127 *, s32, s32, s32, s32);

/* Per-frame update of a spark effect: ends it once the owner is gone, else spawns sparks around it. */
void func_150B54A8(SparkEffect *effect) {
    SparkEffectData *data;
    struct127 *owner;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->interaction_state == 0) || (owner->unique_id != data->ownerId) || (owner->id == 0xFF)) {
        effect->state = -1;
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
        func_15107700(owner, (s16)(func_150ADA20() & 0xFF), (s16)((func_150ADA20() % 101U) - 63),
                      (s16)((func_150ADA20() & 0xF) + 15), 4, 129.0f, (func_150ADA68() * 75.0f) + 35.0f, 0, 1,
                      colour, effect->unkC, effect->unk1);
        data->sparkAccum -= 1.0f;
    }
    data->unkCAccum += (0.2090000063f + (func_150ADA68() * 0.1840000153f)) * D_800BE9A4;
    while (data->unkCAccum > 1.0f) {
        func_15107B78(owner, (s16)(func_150ADA20() & 0xFF), (s16)((func_150ADA20() % 101U) - 63), effect->unkC,
                      effect->unk1);
        data->unkCAccum -= 1.0f;
    }
}

extern void func_151D2B4C(s32);

void func_150B57C4(SparkEffect *effect) {
    func_151D2B4C(6);
}

void func_150B57E8(SparkEffect *effect) {
    func_150B57C4(effect);
    func_1514933C((struct260 *)effect);
}

void func_150B5814(SparkEffect *effect) {
    func_150B57C4(effect);
    func_15149368((struct260 *)effect);
}

/* Event data passed to func_150B5840; its layout depends on the event. */
typedef union {
    /* event 0 */
    struct {
        /* 0x00 */ struct127 *obj;
        /* 0x04 */ u8 id;
    } obj;
    /* event 0x2D: two objects swap places; effects attached to either move to the other */
    struct {
        /* 0x00 */ struct127 *objA;
        /* 0x04 */ struct127 *objB;
        /* 0x08 */ u8 idA;
        /* 0x09 */ u8 idB;
    } swap;
} SparkEvent;

/* Event handler of a spark effect: event 0 destroys it when it concerns its owner, event 0x2D swaps
 * its owner. */
void func_150B5840(SparkEffect *effect, SparkEvent *event, u8 eventId) {
    SparkEffectData *data = &effect->data;
    struct127 *obj;

    if (eventId == 0) {
        obj = event->obj.obj; // matching: loading the pointer first puts it in v1 ahead of data->owner
        if ((obj == data->owner) || (event->obj.id == data->ownerId)) {
            func_1516972C((struct102 *)effect);
        }
    } else if (eventId == 0x2D) {
        if (data->owner == event->swap.objA) {
            data->owner = event->swap.objB;
            data->ownerId = event->swap.idB;
        } else if (data->owner == event->swap.objB) {
            data->owner = event->swap.objA;
            data->ownerId = event->swap.idA;
        }
    }
}
