#include <ultra64.h>
#include "effects.h"
#include <math.h>
#include "conker/collision.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/save.h"
#include "conker/sound.h"


/* Spawns a pulsing particle of the given size at pos (fading out over 0x100 >> fadeShift frames at the end
 * of its lifetime). Also checksums collision_loadMapSettings's code as an anti-tamper check. */
PulseParticle *particle_spawnSoundPulse(Vec3f *pos, Vec3f *vel, f32 size, Vec3f *rot, Vec3f *rotVel, f32 gravity,
                        s16 lifetime, u8 alpha, s32 unused, f32 elasticity, u8 fadeShift, u8 arg11, u8 arg12,
                        s32 arg13) {
    PulseParticle *particle;
    ParticleSpawn spawn;
    Pulse pulse;
    s32 fadeTime;

    pulse.baseSize = size;
    pulse.phase = 0.0f;
    spawn.radius = 24.70000076f * size;
    spawn.elasticity = elasticity;
    spawn.sizeY = size;
    spawn.sizeXZ = size;
    spawn.rot = *rot;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos = *pos;
    spawn.gravity = gravity;
    spawn.flags = 0xB908;
    fadeTime = 0x100 >> fadeShift;
    if (lifetime == -1) {
        spawn.lifetime = 300;
    } else {
        spawn.lifetime = lifetime + fadeTime;
        spawn.flags = 0xB988;
    }
    if (vel != NULL) {
        spawn.vel = *vel;
        spawn.flags |= 0x20;
    } else {
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
    }
    {
        u32 *end;
        u32 *p;
        s32 sum;

        sum = 0;
        p = (u32 *)collision_loadMapSettings;
        end = (u32 *)func_1500310C;
        for (; p < end; p++) {
            sum += *p;
            sum <<= 1;
        }
        if (sum != 0x80D2D760) {
            gCurrentSaveSlot = -1;
        }
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
    spawn.model = 2;
    spawn.variant = 0;
    spawn.zone = 0;
    spawn.alpha = alpha;
    spawn.updateFn = 5;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 6;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 2;
    spawn.squashFn = 0;
    spawn.matrixFn = 2;
    spawn.lightMode = 0;
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

void func_151B5E8C(void) {
}

/* Update of the pulsing particle: advances the phase, playing a sound and restarting each time it passes
 * 29.9; for the first 4 pi the size wobbles by 5% of the base size. */
void particle_updateSoundPulse(PulseParticle *particle) {
    f32 size;
    Pulse *pulse;

    pulse = &particle->pulse;
    pulse->phase += 1.002000093f * gFrameDelta;
    if (pulse->phase > 29.89999962f) {
        pulse->phase = 0.0f;
        sound_playAt(0x502, 0x5DC0, 0, 0, -1, particle->head.pos.x, particle->head.pos.y, particle->head.pos.z, 100, 500);
    }
    if (pulse->phase < 12.56637096f) {
        f32 wave = sinf(pulse->phase);

        size = pulse->baseSize + wave * (0.05000000075f * pulse->baseSize);
        particle->head.sizeY = size;
        particle->head.sizeXZ = size;
    } else {
        size = pulse->baseSize;
        particle->head.sizeY = size;
        particle->head.sizeXZ = size;
    }
    particle_moveBallisticFloor(&particle->head);
}

/* Stops the object's sound, if one is playing. */
void particle_stopPulseSound(SoundOwner *obj) {
    if (obj->sound != 0) {
        sound_stop(obj->sound);
        obj->sound = 0;
    }
}
