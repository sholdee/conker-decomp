#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Bobbing update callbacks for four particle/effect systems. Each one sets the object's Y position to
 * center + sin(phase) * amplitude and advances the phase; the sine parameters live in the velocity and
 * gravity slots of the object's motion data, which these bobbing objects reuse.
 */

/* Head of a pooled physics particle (full layout in game_15F680.c), with the bob in its vel/gravity slots. */
typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ vertex rot;
    /* 0x2C */ vertex scale;
    /* 0x38 */ vertex pos;
    /* 0x44 */ f32 bobPhase;     // vel.x
    /* 0x48 */ f32 bobCenter;    // vel.y
    /* 0x4C */ f32 bobSpeed;     // vel.z
    /* 0x50 */ vertex rotVel;
    /* 0x5C */ f32 bobAmplitude; // gravity
} BobParticle;

/* Particle update (D_800898B0): bobs the particle up and down while it spins about X and Z. */
s32 func_151B8B40(BobParticle *particle) {
    particle->pos.y = sinf(particle->bobPhase) * particle->bobAmplitude + particle->bobCenter;
    particle->bobPhase += particle->bobSpeed * D_800BE9A4;
    particle->bobPhase = func_15144B68(particle->bobPhase);
    particle->rot.x += particle->rotVel.x * D_800BE9A4;
    particle->rot.z += particle->rotVel.z * D_800BE9A4;
    return 1;
}

/* Sprite particle (Particle in game_2062D0.c), with the bob in its desc.vel/desc.gravity slots. */
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ struct17 pos;     // desc.pos
    /* 0x4C */ struct17 offset;  // desc.offset
    /* 0x58 */ f32 bobPhase;     // desc.vel.unk0
    /* 0x5C */ f32 bobCenter;    // desc.vel.unk4
    /* 0x60 */ f32 bobSpeed;     // desc.vel.unk8
    /* 0x64 */ f32 bobAmplitude; // desc.gravity
} BobSprite;

s32 func_151D9450(BobSprite *particle, struct17 *prevPos);

/* Sprite particle update (D_80089760): bobs the particle, then wobbles its size (func_151D9450). */
s32 func_151B8BE0(BobSprite *particle, struct17 *prevPos) {
    particle->pos.unk4 = sinf(particle->bobPhase) * particle->bobAmplitude + particle->bobCenter;
    particle->bobPhase += particle->bobSpeed * D_800BE9A4;
    particle->bobPhase = func_15144B68(particle->bobPhase);
    return func_151D9450(particle, prevPos);
}

/* Motion extension of a model effect (EffectMotion in game_1844C0.c), with the bob in its vel/gravity slots. */
typedef struct {
    /* 0x00 */ vertex rot;
    /* 0x0C */ f32 scale;
    /* 0x10 */ f32 phase;        // vel.x
    /* 0x14 */ f32 center;       // vel.y
    /* 0x18 */ f32 speed;        // vel.z
    /* 0x1C */ vertex rotVel;
    /* 0x28 */ f32 amplitude;    // gravity
} EffectBob;

/* Model effect (Effect in game_1844C0.c; only the fields used here). */
typedef struct {
    /* 0x000 */ u8 pad0[0x54];
    /* 0x054 */ vertex pos;      // params.pos
    /* 0x060 */ u8 pad60[0x120 - 0x60];
    /* 0x120 */ EffectBob bob;   // extra
} BobEffect;

/* Effect update (D_8008AD90): bobs the effect up and down while it spins about X and Z. */
s32 func_151B8C54(BobEffect *effect) {
    EffectBob *bob = &effect->bob;

    effect->pos.y = sinf(bob->phase) * bob->amplitude + bob->center;
    bob->phase += bob->speed * D_800BE9A4;
    bob->phase = func_15144B68(bob->phase);
    bob->rot.x += bob->rotVel.x * D_800BE9A4;
    bob->rot.z += bob->rotVel.z * D_800BE9A4;
    return 1;
}

/* Bob state that follows a game_169510.c particle (another member of its ParticleMotion union). */
typedef struct {
    /* 0x00 */ f32 center;
    /* 0x04 */ f32 amplitude;
    /* 0x08 */ f32 phase;
    /* 0x0C */ f32 speed;
    /* 0x10 */ vertex rotVel;
} QuadBob;

/* Quad particle of game_169510.c (only the fields used here), followed by its motion state. */
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ vertex pos;      // def.pos
    /* 0x040 */ vertex rot;      // def.rot
    /* 0x04C */ u8 pad4C[0x110 - 0x4C];
    /* 0x110 */ QuadBob bob;
} BobQuad;

/* Particle update (D_80089CB8): bobs the particle up and down while it spins about X and Z. */
s32 func_151B8CFC(BobQuad *particle) {
    QuadBob *bob = &particle->bob;

    particle->pos.y = sinf(bob->phase) * bob->amplitude + bob->center;
    bob->phase += bob->speed * D_800BE9A4;
    bob->phase = func_15144B68(bob->phase);
    particle->rot.x += bob->rotVel.x * D_800BE9A4;
    particle->rot.z += bob->rotVel.z * D_800BE9A4;
    return 1;
}
