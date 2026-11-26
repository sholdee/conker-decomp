#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Bobbing update callbacks for four particle/effect systems. Each one sets the object's Y position to
 * center + sin(phase) * amplitude and advances the phase; the sine parameters live in the velocity and
 * gravity slots of the object's motion data, which these bobbing objects reuse.
 */

/* Particle update (D_800898B0): bobs the particle up and down while it spins about X and Z. */
s32 func_151B8B40(BobParticle *particle) {
    particle->pos.y = sinf(particle->bobPhase) * particle->bobAmplitude + particle->bobCenter;
    particle->bobPhase += particle->bobSpeed * D_800BE9A4;
    particle->bobPhase = func_15144B68(particle->bobPhase);
    particle->rot.x += particle->rotVel.x * D_800BE9A4;
    particle->rot.z += particle->rotVel.z * D_800BE9A4;
    return 1;
}

/* Sprite particle update (D_80089760): bobs the particle, then wobbles its size (func_151D9450).
 * The bob uses desc.vel (x: phase, y: center, z: speed) and desc.gravity (amplitude). */
s32 func_151B8BE0(SteamParticle *particle, Vec3f *prevPos) {
    particle->head.desc.pos.y = sinf(particle->head.desc.vel.x) * particle->head.desc.gravity + particle->head.desc.vel.y;
    particle->head.desc.vel.x += particle->head.desc.vel.z * D_800BE9A4;
    particle->head.desc.vel.x = func_15144B68(particle->head.desc.vel.x);
    return func_151D9450(particle, prevPos);
}

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

/* Particle update (D_80089CB8): bobs the particle up and down while it spins about X and Z. */
s32 func_151B8CFC(BobQuad *particle) {
    QuadBob *bob = &particle->bob;

    particle->head.def.pos.y = sinf(bob->phase) * bob->amplitude + bob->center;
    bob->phase += bob->speed * D_800BE9A4;
    bob->phase = func_15144B68(bob->phase);
    particle->head.def.rot.x += bob->rotVel.x * D_800BE9A4;
    particle->head.def.rot.z += bob->rotVel.z * D_800BE9A4;
    return 1;
}
