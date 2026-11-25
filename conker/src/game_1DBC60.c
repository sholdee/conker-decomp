#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Effect descriptor passed to func_1513418C (same layout as game_10CD70.c's BloodDripDesc). */
typedef struct {
    /* 0x00 */ s32 target[2];
    /* 0x08 */ u8 ownerId;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ Actor *owner;
    /* 0x10 */ u8 bone;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ struct17 offset;
    /* 0x20 */ f32 radius;
    /* 0x24 */ f32 rate;
    /* 0x28 */ s16 lifetime;
    /* 0x2A */ u8 flags;
    /* 0x2B */ u8 emitter;
    /* 0x2C */ s8 callback;
    /* 0x2D */ u8 unk2D;
} EffectDesc;

/* EffectDesc.flags bits (see BloodDrip in game_161520.c). */
#define EFFECT_ON_BONE  0x02
#define EFFECT_LIFETIME 0x04

void func_1516962C(s32 type, Actor *actor, u8 eventType);
void *func_1513418C(EffectDesc *desc, s32 arg1, u8 arg2, s32 arg3);

/* Attaches emitter 6 to bone 1 of `actor`; a nonzero `lifetime` ends it after that many frames. */
void *func_151AE7B0(Actor *actor, f32 rate, s16 lifetime, u8 arg3, s32 arg4) {
    void *effect; // matching: the named result keeps IDO's 80-byte frame (direct return: 72)
    EffectDesc desc;

    if (actor == NULL) {
        return NULL;
    }

    func_1516962C(0x28, actor, 0x11);
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 1;
    desc.offset.unk0 = 0.0f;
    desc.offset.unk4 = 0.0f;
    desc.offset.unk8 = 0.0f;
    desc.radius = 15.40000057f;
    desc.rate = rate;
    desc.flags = EFFECT_ON_BONE;
    if (lifetime != 0) {
        desc.lifetime = lifetime;
        desc.flags = EFFECT_ON_BONE | EFFECT_LIFETIME;
    } else {
        desc.lifetime = 300;
    }
    desc.emitter = 6;
    desc.callback = -1; /* none */
    desc.unk2D = 5;
    effect = func_1513418C(&desc, 0, arg3, arg4);
    return effect;
}
