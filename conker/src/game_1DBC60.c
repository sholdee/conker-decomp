#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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
    desc.flags = BLOOD_DRIP_ON_BONE;
    if (lifetime != 0) {
        desc.lifetime = lifetime;
        desc.flags = BLOOD_DRIP_ON_BONE | BLOOD_DRIP_LIFETIME;
    } else {
        desc.lifetime = 300;
    }
    desc.emitter = 6;
    desc.callback = -1; /* none */
    desc.unk2D = 5;
    effect = func_1513418C(&desc, 0, arg3, arg4);
    return effect;
}
