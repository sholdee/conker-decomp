#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-type particle state at particle + 0x110, ballistic layout (func_1514BC08 reuses the area for its wobble state). */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ struct17 velocity;
    /* 0x44 */ f32 gravity;
    /* 0x48 */ f32 drag;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ s16 unk54;
} ParticleMotion;

/* Particle instance layout shared by the update callbacks in this file. */
typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 life;
    /* 0x01E */ u8 pad1E[0x16];
    /* 0x034 */ struct17 pos;
    /* 0x040 */ struct17 unk40;
    /* 0x04C */ u8 pad4C[0xC];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 pad5C[0x15];
    /* 0x071 */ u8 unk71;
    /* 0x072 */ u8 unk72;
    /* 0x073 */ u8 pad73[0x9D];
    /* 0x110 */ ParticleMotion motion;
} Particle;

s32 func_1514BC08(void *arg0, void *arg1);
void func_1514BE20(void *);

/* Sets unk40 to the point 100 units above the particle's position. */
void func_1514BF50(Particle *p) {
    p->unk40.unk0 = p->pos.unk0;
    p->unk40.unk4 = p->pos.unk4 + 100.0f;
    p->unk40.unk8 = p->pos.unk8;
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
    struct17 oldVel;
    s32 ticks;

    motion = &p->motion;
    oldVel = motion->velocity;
    for (ticks = D_800BE9E4; ticks != 0; ticks--) {
        motion->velocity.unk0 *= motion->drag;
        motion->velocity.unk4 *= motion->drag;
        motion->velocity.unk8 *= motion->drag;
    }
    {
        f32 accelX;
        f32 accelY;
        f32 accelZ;

        motion->velocity.unk4 += motion->gravity * D_800BE9A4;
        accelX = (motion->velocity.unk0 - oldVel.unk0) * D_800BE9A8;
        accelY = (motion->velocity.unk4 - oldVel.unk4) * D_800BE9A8;
        accelZ = (motion->velocity.unk8 - oldVel.unk8) * D_800BE9A8;
        p->pos.unk0 += (oldVel.unk0 + 0.5f * accelX * D_800BE9A4) * D_800BE9A4;
        p->pos.unk4 += (oldVel.unk4 + 0.5f * accelY * D_800BE9A4) * D_800BE9A4;
        p->pos.unk8 += (oldVel.unk8 + 0.5f * accelZ * D_800BE9A4) * D_800BE9A4;
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
    p->unk72 = 0;
    p->unk71 = 36;
    p->life = p->motion.unk54;
    p->flags |= 0x08000000;
    p->motion.gravity = p->motion.unk50;
    return 1;
}
