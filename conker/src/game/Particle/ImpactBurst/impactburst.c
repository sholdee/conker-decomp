#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/particle.h"


/* Spawns a burst at pos scaled by scale: one particle_newOwnedSpriteParticle particle moving along vel,
 * 5-12 particle_newModelParticle pieces flung in random directions, and a light_createWithEffect flash. */
void particle_spawnImpactBurst(Vec3f *pos, Vec3f *vel, f32 scale, u8 owner, s32 list) {
    s32 count;
    ParticleDesc6C spawn;
    s32 yaw;
    ParticleSpawn piece;
    LightEffectHeader flash;
    Vec3i origin;
    s32 pitch;

    spawn.fadeStart = spawn.scaleStart = spawn.timer = (math_random() % 31U) + 20;
    spawn.fadeRate = 255 / spawn.timer;
    spawn.alpha = 0xFF;
    spawn.width = spawn.height = ((math_randomFloat() * 608.0f) + 808.0f) * scale;
    spawn.scaleRate = ((math_randomFloat() * 18.0f) + 1016.0f) * 0.001f;
    spawn.texture = 22;
    spawn.renderMode = 0x2001;
    spawn.setGeometryMode = 0x200005;
    spawn.clearGeometryMode = 0;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.a = 0xFF;
    spawn.brightness = 0xFF;
    spawn.flags = 0x800E05;
    spawn.primColorMode = 3;
    spawn.envColorMode = 3;
    spawn.updateFunc = -1;
    spawn.deathFunc = -1;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    if (math_random() & 1) {
        spawn.flags |= 0x40;
    }
    if (math_random() & 1) {
        spawn.flags |= 0x80;
    }
    spawn.r2 = 0xFF;
    spawn.g2 = 0x8E;
    spawn.b2 = 0x13;
    spawn.r = 0x2F;
    spawn.g = 0;
    spawn.b = 0;
    spawn.pos = *pos;
    spawn.vel = *vel;
    spawn.gravity = 0.0f;
    particle_newOwnedSpriteParticle(&spawn, 0, 0, owner, list);

    count = (math_random() & 7) + 5;
    piece.flags = 0x21E8;
    piece.model = 27;
    piece.radius = 0.0f;
    piece.rot.x = 0.0f;
    piece.rot.y = 0.0f;
    piece.rot.z = 0.0f;
    piece.rotVel.y = 0.0f;
    piece.variant = 0;
    piece.zone = 0;
    piece.updateFn = 1;
    piece.update2Fn = 0;
    piece.floorHitFn = 0;
    piece.floorType3Fn = 0;
    piece.wallHitFn = 0;
    piece.destroyFn = 0;
    piece.squashFn = 0;
    piece.matrixFn = 0;
    piece.lightMode = 2;
    piece.owner = NULL;
    piece.ownerId = 0;
    piece.fadeStart = 20;
    piece.fadeRate = 12;
    piece.scale.x = 1.0f;
    piece.scale.y = 1.0f;
    piece.scale.z = 1.0f;
    piece.elasticity = 0.2f;
    while (count > 0) {
        piece.pos = *pos;
        piece.lifetime = (math_random() % 41U) + 29;
        piece.alpha = (math_random() % 76U) + 180;
        yaw = math_random();
        pitch = math_random();
        math_dirFromBinAngles(yaw & 0xFF, -(pitch % 65U), ((math_randomFloat() * 2031.0f) + 1641.0f) * 0.01f, &piece.vel);
        piece.sizeXZ = piece.sizeY = ((math_randomFloat() * 3047.0f) + 1016.0f) * 0.0001f * scale;
        // matching: `+ -206.0f` (add.s of a negative constant); `- 206.0f` emits sub.s
        piece.gravity = ((math_randomFloat() * 99.0f) + -206.0f) * 0.01f;
        piece.rotVel.x = (math_randomFloat() * 101.0f) + -49.0f;
        piece.rotVel.z = (math_randomFloat() * 101.0f) + -49.0f;
        particle_newModelParticle(&piece, 3, 0xFF, 0, owner, list);
        count--;
    }

    flash.flags = 3;
    flash.update = -1;
    flash.lifetime = (math_random() % 13U) + 8;
    flash.lightType = 0;
    origin.x = pos->x;
    origin.y = pos->y;
    origin.z = pos->z;
    light_createWithEffect(&flash, &origin, (math_random() % 31U) + 70, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, owner, list);
}


/* Emitter update (handler table entry): accumulates a random spawn rate and, for each whole unit,
 * fires a particle_spawnImpactBurst burst at the spawner's source with a random direction and scale. */
void particle_updateRandomBurstEmitter(RandomBurstEmitter *emitter) {
    BurstSpawner *spawner;
    Vec3f pos;
    Vec3f dir;
    f32 scale;
    s32 yaw;
    s32 pitch;

    spawner = &emitter->spawner;
    spawner->spawnAccum += (spawner->rateBase + math_randomFloat() * spawner->rateRange) * gFrameDelta;
    while (spawner->spawnAccum > 1.0f) {
        scale = math_randomFloat() * spawner->scaleRange + spawner->scaleBase;
        math_randomPointInZone(spawner->source, &pos);
        yaw = math_random();
        pitch = math_random();
        math_dirFromBinAngles(yaw & 0xFF, 0x40 - (pitch % 129U), math_randomFloat() * 600.0f * 0.01f, &dir);
        particle_spawnImpactBurst(&pos, &dir, scale, emitter->base.header.owner, emitter->base.header.kind);
        spawner->spawnAccum -= 1.0f;
    }
}
