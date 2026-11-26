#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Spawns emitter->count debris particles at random points within emitter->radius of emitter->pos,
 * each followed by a BloodTrail. mode picks the launch velocity: 0 = horizontal speed along the yaw
 * plus a separate vertical speed, 1 = speed along the spawn direction, 2 = dir, 3 = dir * speed.
 */
void blood_spawnDebris(DebrisEmitter *emitter, u8 mode, u8 arg2, s32 arg3) {
    s16 count;
    ParticleSpawn spawn;
    s32 pad; // matching: unused stack slot
    DebrisData debris;
    f32 size;
    u8 yaw;
    u8 pitch;
    f32 sinYaw;
    f32 cosYaw;
    f32 sinPitch;
    f32 cosPitch;
    f32 dist;
    BloodDebrisParticle *particle;

    count = emitter->count;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.flags = 0x21E9;
    spawn.variant = 0;
    spawn.lifetime = emitter->lifetime;
    spawn.model = 0x1B;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.rotVel.y = 0.0f;
    spawn.elasticity = 0.200000003f;
    debris.stopSpeed = -18.0f;
    debris.landPuffs = emitter->landPuffs;
    debris.landPuffFlag = emitter->landPuffFlag;
    debris.landPuffLifeBase = emitter->landPuffLifeBase;
    debris.landPuffLifeRange = emitter->landPuffLifeRange;
    spawn.unk5C = NULL;
    spawn.alpha = 0xFF;
    spawn.updateFn = 1;
    spawn.update2Fn = 5;
    spawn.floorHitFn = 4;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 1;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.unk6A = 0;
    spawn.owner = NULL;
    spawn.ownerId = 0;
    spawn.fadeStart = 32;
    spawn.fadeRate = 7;
    while (count--) {
        yaw = func_150ADA20() % (emitter->yawRange + 1) + emitter->yawBase;
        pitch = func_150ADA20() % (emitter->pitchRange + 1) + emitter->pitchBase;
        /* math_cosBinAngleMasked is a cosine, so cos(a - 64) is sin(a) */
        sinYaw = math_cosBinAngleMasked(yaw - 64);
        cosYaw = math_cosBinAngleMasked(yaw);
        sinPitch = math_cosBinAngleMasked(pitch - 64);
        cosPitch = math_cosBinAngleMasked(pitch);
        dist = func_150ADA68() * emitter->radius;
        spawn.pos.x = emitter->pos.x + dist * cosPitch * sinYaw;
        spawn.pos.y = emitter->pos.y - dist * sinPitch;
        spawn.pos.z = emitter->pos.z + dist * cosPitch * cosYaw;
        switch (mode) {
        case 0: {
            f32 speed;
            f32 velY;
            speed = func_150ADA68() * emitter->speedRange + emitter->speedBase;
            velY = func_150ADA68() * emitter->velYRange + emitter->velYBase;
            spawn.vel.x = speed * sinYaw;
            spawn.vel.y = velY;
            spawn.vel.z = speed * cosYaw;
        }
            break;
        case 1: {
            f32 speed;
            speed = func_150ADA68() * emitter->speedRange + emitter->speedBase;
            spawn.vel.x = speed * cosPitch * sinYaw;
            spawn.vel.y = -speed * sinPitch;
            spawn.vel.z = speed * cosPitch * cosYaw;
        }
            break;
        case 2:
            spawn.vel = emitter->dir;
            break;
        case 3: {
            f32 speed;
            speed = func_150ADA68() * emitter->speedRange + emitter->speedBase;
            spawn.vel.x = emitter->dir.x * speed;
            spawn.vel.y = emitter->dir.y * speed;
            spawn.vel.z = emitter->dir.z * speed;
            break;
        }
        }
        size = spawn.sizeXZ = spawn.sizeY = func_150ADA68() * emitter->sizeRange + emitter->sizeBase;
        spawn.radius = size * 30.0f;
        spawn.gravity = func_150ADA68() * emitter->gravityRange + emitter->gravityBase;
        spawn.rotVel.x = func_150ADA68() * emitter->spinRange + emitter->spinBase;
        spawn.rotVel.z = func_150ADA68() * emitter->spinRange + emitter->spinBase;
        particle = particle_newModelParticle(&spawn, 0, 0, sizeof(DebrisData), arg2, arg3);
        if (particle != NULL) {
            BloodTrailDesc18 trailDesc;
            DebrisData *data;
            DebrisBloodTrail *trail;
            data = &particle->debris;
            memcpy(data, &debris, sizeof(DebrisData));
            trailDesc.x = &particle->head.pos.x;
            trailDesc.y = &particle->head.pos.y;
            trailDesc.z = &particle->head.pos.z;
            trailDesc.radius = 15.0f * spawn.sizeY;
            trailDesc.lifetime = 300;
            trailDesc.flags = 0;
            trailDesc.emitter = 1;
            trailDesc.rate = 0.3580000103f;
            data->trail = NULL;
            trail = blood_spawnTrail(&trailDesc, sizeof(f32), arg2, arg3);
            data->trail = trail;
            if (trail != NULL) {
                memcpy(&trail->size, &size, sizeof(f32));
            }
        }
    }
}

/* Particle update2Fn of the debris pieces: drops the blood trail once the piece starts fading out. */
s32 blood_dropDebrisTrail(BloodDebrisParticle *particle) {
    DebrisData *data = &particle->debris;

    if (particle->head.lifetime < 32) {
        if (data->trail != NULL) {
            effect_destroyObject(&data->trail->trail.header);
            data->trail = NULL;
        }
    }
    return 1;
}

/*
 * Particle floorHitFn of the debris pieces. A slow landing stops the piece (and starts its fade-out),
 * optionally with a puff of func_1514AB5C particles; a fast one bounces it.
 */
s32 blood_onDebrisFloorHit(BloodDebrisParticle *particle, f32 x, f32 y, f32 z, f32 floorY) {
    DebrisData *data;
    u8 puffFlag;

    data = &particle->debris;
    particle->head.pos.y = particle->head.radius + floorY;
    if (data->trail != NULL) {
        effect_destroyObject(&data->trail->trail.header);
        data->trail = NULL;
    }
    if (data->stopSpeed < particle->head.vel.y) {
        particle->head.flags &= ~7;
        particle->head.flags &= ~8;
        particle->head.flags &= ~0x40;
        particle->head.flags &= ~0x20;
        particle->head.vel.x = 0.0f;
        particle->head.vel.y = 0.0f;
        particle->head.vel.z = 0.0f;
        if (particle->head.lifetime > 32) {
            particle->head.lifetime = 32;
        }
        if (data->landPuffs) {
            puffFlag = FALSE;
            if (data->landPuffFlag) {
                puffFlag = TRUE;
            }
            func_1514AB5C(x, floorY, z, particle->head.sizeXZ * 35.0f, func_150ADA20() % 3 + 2,
                          particle->head.sizeXZ, particle->head.sizeXZ * 0.5f,
                          particle->head.sizeXZ, particle->head.sizeXZ * 0.5f,
                          data->landPuffLifeBase, data->landPuffLifeRange, puffFlag);
        }
    } else {
        particle->head.vel.x *= particle->head.elasticity;
        particle->head.vel.y = -particle->head.vel.y * particle->head.elasticity;
        particle->head.vel.z *= particle->head.elasticity;
        particle->head.rotVel.x *= particle->head.elasticity;
        particle->head.rotVel.y *= particle->head.elasticity;
        particle->head.rotVel.z *= particle->head.elasticity;
    }
    return 1;
}



/*
 * BloodTrail emitter 1 (the debris trails of blood_spawnDebris): emits one blood drop at (x, y, z),
 * flung back against the trail's motion (dx, dy, dz) and sized by the debris piece.
 */
void blood_emitDebrisDrop(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, DebrisBloodTrail *trail) {
    Vec3f pos;
    Vec3f vel;
    Vec3f offset;
    f32 speed;
    f32 size;
    f32 dt;
    s32 pad; // matching: unused stack slot above rand
    struct {
        f32 gravity;
        f32 size;
        u32 timer;
    } rand;

    size = trail->size;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    // matching: an early (empty) use of dt makes IDO give it $f12 and speed $f2
    do { } while (0);
    if (dt) {
    }
    speed = func_150ADA68();
    dt = gFrameDeltaInv;
    speed = speed * 166.0f + 160.0f;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;
    speed *= 0.001000000047f;
    vel.x = -dx * dt * speed;
    vel.y = -dy * dt * speed;
    vel.z = -dz * dt * speed;
    rand.gravity = func_150ADA68();
    rand.size = func_150ADA68();
    rand.timer = func_150ADA20();
    // matching: `+ -600.0f` (add.s); `- 600.0f` would emit sub.s
    particle_spawnDragFadeSprite(&pos, &offset, &vel, 1.0f, (rand.gravity * 591.0f + -600.0f) * 0.001000000047f,
                  (rand.size * 500.0f + 400.0f) * size, rand.timer % 21 + 15, func_150ADA20() % 101 + 100,
                  15, 20, 0, -1, 86, 39, 0, trail->trail.header.owner, trail->trail.header.kind);
}

/* Callback 0x14: fade out and swell; switches to callback 0x15 to tint. */
void func_151A77C0(BillboardParticle *particle) {
    AgeFadeParams *data = (AgeFadeParams *)(particle + 1);

    if (particle->tmpl.life < data->fadeStart) {
        particle->tmpl.primA = particle->tmpl.life * data->fadeStep;
    }
    if (particle->tmpl.life < data->growStart) {
        s16 growth = data->growRate * D_800BE9E4;

        particle->tmpl.width += growth;
        particle->tmpl.height += growth;
    }
    if (particle->tmpl.life < data->tintStart) {
        particle->tmpl.callback = 0x15;
        particle->tmpl.flags |= BILLBOARD_ENV_COLOR | BILLBOARD_2CYCLE;
        particle->tmpl.envR = particle->tmpl.envG = particle->tmpl.envB = particle->tmpl.life * data->tintStep;
        particle->tmpl.unk4 = 0xC000F;
    }
}

/* Callback 0x15: func_151A77C0 after the switch, with the env colour following life. */
void func_151A787C(BillboardParticle *particle) {
    AgeFadeParams *data = (AgeFadeParams *)(particle + 1);

    if (particle->tmpl.life < data->fadeStart) {
        particle->tmpl.primA = particle->tmpl.life * data->fadeStep;
    }
    if (particle->tmpl.life < data->growStart) {
        s16 growth = data->growRate * D_800BE9E4;

        particle->tmpl.width += growth;
        particle->tmpl.height += growth;
    }
    particle->tmpl.envR = particle->tmpl.envG = particle->tmpl.envB = particle->tmpl.life * data->tintStep;
}

/* Particle destroyFn of the debris pieces: removes the blood trail. */
void blood_destroyDebris(BloodDebrisParticle *particle) {
    DebrisData *data = &particle->debris;

    if (data->trail != NULL) {
        effect_destroyObject(&data->trail->trail.header);
        data->trail = NULL;
    }
}
