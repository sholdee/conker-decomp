#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* BloodDripDesc.flags bits (see BloodDrip in game_161520.c). */
#define EFFECT_ON_BONE  0x02
#define EFFECT_LIFETIME 0x04

void func_1516962C(s32 type, Actor *actor, u8 eventType);
void *func_1513418C(BloodDripDesc *desc, s32 arg1, u8 arg2, s32 arg3);

/* Attaches emitter 6 to bone 1 of `actor`; a nonzero `lifetime` ends it after that many frames. */
void *func_151AE7B0(Actor *actor, f32 rate, s16 lifetime, u8 arg3, s32 arg4) {
    void *effect; // matching: the named result keeps IDO's 80-byte frame (direct return: 72)
    BloodDripDesc desc;

    if (actor == NULL) {
        return NULL;
    }

    func_1516962C(0x28, actor, 0x11);
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 1;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
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
