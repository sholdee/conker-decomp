#include <ultra64.h>
#include "effects.h"
#include <math.h>
#include "conker/game.h"
#include "conker/particle.h"


/* Spawns a pulsing particle of the given size at pos (fading out over 0x100 >> fadeShift frames at the end
 * of its lifetime). */
PulseParticle *particle_spawnPulse(Vec3f *pos, Vec3f *vel, f32 size, Vec3f *rot, Vec3f *rotVel, f32 gravity,
                        s16 lifetime, u8 alpha, s32 unused, f32 elasticity, u8 fadeShift, u8 arg11, u8 arg12,
                        s32 arg13) {
    PulseParticle *particle;
    ParticleSpawn spawn;
    Pulse pulse;
    s32 fadeTime;

    pulse.baseSize = size;
    pulse.phase = 0.0f;
    spawn.radius = 8.100000381f * size;
    spawn.elasticity = elasticity;
    spawn.sizeY = size;
    spawn.sizeXZ = size;
    spawn.rot = *rot;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos = *pos;
    spawn.gravity = gravity;
    spawn.flags = 0x3900;
    fadeTime = 0x100 >> fadeShift;
    if (lifetime == -1) {
        spawn.lifetime = 300;
    } else {
        spawn.lifetime = lifetime + fadeTime;
        spawn.flags = 0x3980;
    }
    if (vel != NULL) {
        spawn.vel = *vel;
        spawn.flags |= 0x20;
    } else {
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
    }
    if (rotVel != NULL) {
        spawn.rotVel = *rotVel;
        spawn.flags |= 0x40;
    } else {
        spawn.rotVel.x = 0.0f;
        spawn.rotVel.y = 0.0f;
        spawn.rotVel.z = 0.0f;
    }
    if (arg11) {
        spawn.flags |= 1;
    }
    spawn.model = 3;
    spawn.variant = 0;
    spawn.unk5C = 0;
    spawn.alpha = alpha;
    spawn.updateFn = 6;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 6;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 2;
    spawn.unk6A = 0;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = fadeTime;
    spawn.fadeRate = 255 / fadeTime;
    particle = particle_newModelParticle(&spawn, 3, 255, sizeof(Pulse), arg12, arg13);
    if (particle != NULL) {
        memcpy(&particle->pulse, &pulse, sizeof(Pulse));
    }
    return particle;
}

/* Update of the pulsing particle: advances the phase (wrapping at 2 pi) and wobbles the size by 6.1% of the
 * base size. */
void particle_updatePulseWobble(PulseParticle *particle) {
    Pulse *pulse = &particle->pulse;

    pulse->phase += 0.5560000539f * gFrameDelta;
    while (pulse->phase > 6.283185482f) {
        pulse->phase -= 6.283185482f;
    }
    {
        f32 wave = sinf(pulse->phase);
        f32 size = pulse->baseSize + wave * (0.06100000441f * pulse->baseSize);

        particle->head.sizeY = size;
        particle->head.sizeXZ = size;
    }
    particle_moveBallisticFloor(&particle->head);
}
