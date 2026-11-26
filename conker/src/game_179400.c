#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Sets unk40 to the point 100 units above the particle's position. */
void func_1514BF50(JitterParticle *p) {
    p->head.def.rot.x = p->head.def.pos.x;
    p->head.def.rot.y = p->head.def.pos.y + 100.0f;
    p->head.def.rot.z = p->head.def.pos.z;
}

/* Eases the particle toward random wobble targets kept in its state at + 0x110. */
void func_1514BF7C(JitterParticle *p) {
    func_1514BC08(p, &p->jitter);
}

/*
 * Advances the particle one frame: applies drag once per elapsed tick and gravity to the
 * velocity, then moves the position by the average of the old and new velocity.
 */
void func_1514BF9C(JitterParticle *p) {
    ParticleJitter *motion;
    Vec3f oldVel;
    s32 ticks;

    motion = &p->jitter;
    oldVel = motion->velocity;
    for (ticks = D_800BE9E4; ticks != 0; ticks--) {
        motion->velocity.x *= motion->drag;
        motion->velocity.y *= motion->drag;
        motion->velocity.z *= motion->drag;
    }
    {
        f32 accelX;
        f32 accelY;
        f32 accelZ;

        motion->velocity.y += motion->gravity * gFrameDelta;
        accelX = (motion->velocity.x - oldVel.x) * gFrameDeltaInv;
        accelY = (motion->velocity.y - oldVel.y) * gFrameDeltaInv;
        accelZ = (motion->velocity.z - oldVel.z) * gFrameDeltaInv;
        p->head.def.pos.x += (oldVel.x + 0.5f * accelX * gFrameDelta) * gFrameDelta;
        p->head.def.pos.y += (oldVel.y + 0.5f * accelY * gFrameDelta) * gFrameDelta;
        p->head.def.pos.z += (oldVel.z + 0.5f * accelZ * gFrameDelta) * gFrameDelta;
    }
}

/* Update callback: ballistic motion, then func_1514BE20. */
s32 func_1514C258(JitterParticle *p) {
    func_1514BF9C(p);
    func_1514BE20(p);
    return 1;
}

/* Update callback: ballistic motion, then re-aims unk40 above the new position. */
s32 func_1514C288(JitterParticle *p) {
    func_1514BF9C(p);
    func_1514BF50(p);
    return 1;
}

/* Resets the particle: life from relaunch.lifetime, gravity from relaunch.gravity, sets flag 0x08000000. */
s32 func_1514C2B8(JitterParticle *p) {
    p->head.expireFn = 0;
    p->head.updateFn2 = 36;
    p->head.def.lifetime = p->relaunch.lifetime;
    p->head.def.flags |= 0x08000000;
    p->jitter.gravity = p->relaunch.gravity;
    return 1;
}
