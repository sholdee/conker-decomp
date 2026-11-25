#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Per-type particle state at particle + 0x110, ballistic layout (func_1514BC08 reuses the area for its wobble state). */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ Vec3f velocity;
    /* 0x44 */ f32 gravity;
    /* 0x48 */ f32 drag;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ s16 unk54;
} ParticleMotion;

/* Particle instance layout shared by the update callbacks in this file. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ ParticleMotion motion;
} Particle;

s32 func_1514BC08(void *arg0, void *arg1);
void func_1514BE20(void *);

/* Sets unk40 to the point 100 units above the particle's position. */
void func_1514BF50(Particle *p) {
    p->head.def.rot.x = p->head.def.pos.x;
    p->head.def.rot.y = p->head.def.pos.y + 100.0f;
    p->head.def.rot.z = p->head.def.pos.z;
}

/* Eases the particle toward random wobble targets kept in its state at + 0x110. */
void func_1514BF7C(Particle *p) {
    func_1514BC08(p, &p->motion);
}

/*
 * Advances the particle one frame: applies drag once per elapsed tick and gravity to the
 * velocity, then moves the position by the average of the old and new velocity.
 */
void func_1514BF9C(Particle *p) {
    ParticleMotion *motion;
    Vec3f oldVel;
    s32 ticks;

    motion = &p->motion;
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

        motion->velocity.y += motion->gravity * D_800BE9A4;
        accelX = (motion->velocity.x - oldVel.x) * D_800BE9A8;
        accelY = (motion->velocity.y - oldVel.y) * D_800BE9A8;
        accelZ = (motion->velocity.z - oldVel.z) * D_800BE9A8;
        p->head.def.pos.x += (oldVel.x + 0.5f * accelX * D_800BE9A4) * D_800BE9A4;
        p->head.def.pos.y += (oldVel.y + 0.5f * accelY * D_800BE9A4) * D_800BE9A4;
        p->head.def.pos.z += (oldVel.z + 0.5f * accelZ * D_800BE9A4) * D_800BE9A4;
    }
}

/* Update callback: ballistic motion, then func_1514BE20. */
s32 func_1514C258(Particle *p) {
    func_1514BF9C(p);
    func_1514BE20(p);
    return 1;
}

/* Update callback: ballistic motion, then re-aims unk40 above the new position. */
s32 func_1514C288(Particle *p) {
    func_1514BF9C(p);
    func_1514BF50(p);
    return 1;
}

/* Resets the particle: life from motion.unk54, gravity from motion.unk50, sets flag 0x08000000. */
s32 func_1514C2B8(Particle *p) {
    p->head.expireFn = 0;
    p->head.updateFn2 = 36;
    p->head.def.lifetime = p->motion.unk54;
    p->head.def.flags |= 0x08000000;
    p->motion.gravity = p->motion.unk50;
    return 1;
}
