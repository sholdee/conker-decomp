#include <ultra64.h>
#include <math.h>
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"

/*
 * Bobbing update callbacks for four particle/effect systems. Each one sets the object's Y position to
 * center + sin(phase) * amplitude and advances the phase; the sine parameters live in the velocity and
 * gravity slots of the object's motion data, which these bobbing objects reuse.
 */

/* Particle update (gModelParticleUpdateFuncs): bobs the particle up and down while it spins about X and Z. */
s32 particle_bobModelParticle(BobParticle *particle) {
    particle->pos.y = sinf(particle->bobPhase) * particle->bobAmplitude + particle->bobCenter;
    particle->bobPhase += particle->bobSpeed * gFrameDelta;
    particle->bobPhase = math_wrapRadians(particle->bobPhase);
    particle->rot.x += particle->rotVel.x * gFrameDelta;
    particle->rot.z += particle->rotVel.z * gFrameDelta;
    return 1;
}

/* Sprite particle update (gSpriteUpdateFuncs): bobs the particle, then wobbles its size (particle_wobbleSteamSize).
 * The bob uses desc.vel (x: phase, y: center, z: speed) and desc.gravity (amplitude). */
s32 particle_bobSteamParticle(SteamParticle *particle, Vec3f *prevPos) {
    particle->head.desc.pos.y = sinf(particle->head.desc.vel.x) * particle->head.desc.gravity + particle->head.desc.vel.y;
    particle->head.desc.vel.x += particle->head.desc.vel.z * gFrameDelta;
    particle->head.desc.vel.x = math_wrapRadians(particle->head.desc.vel.x);
    return particle_wobbleSteamSize(particle, prevPos);
}

/* Effect update (gModelEffectUpdateFuncs): bobs the effect up and down while it spins about X and Z. */
s32 particle_bobEffect(BobEffect *effect) {
    EffectBob *bob = &effect->bob;

    effect->pos.y = sinf(bob->phase) * bob->amplitude + bob->center;
    bob->phase += bob->speed * gFrameDelta;
    bob->phase = math_wrapRadians(bob->phase);
    bob->rot.x += bob->rotVel.x * gFrameDelta;
    bob->rot.z += bob->rotVel.z * gFrameDelta;
    return 1;
}

/* Particle update (gQuadParticleUpdate1Funcs): bobs the particle up and down while it spins about X and Z. */
s32 particle_bobQuad(BobQuad *particle) {
    QuadBob *bob = &particle->bob;

    particle->head.def.pos.y = sinf(bob->phase) * bob->amplitude + bob->center;
    bob->phase += bob->speed * gFrameDelta;
    bob->phase = math_wrapRadians(bob->phase);
    particle->head.def.rot.x += bob->rotVel.x * gFrameDelta;
    particle->head.def.rot.z += bob->rotVel.z * gFrameDelta;
    return 1;
}
