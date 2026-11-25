#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Debris emitter settings read by func_151A6F00. Angles are 0..255 binary angles. */
typedef struct {
    /* 0x00 */ s16 count;
    /* 0x04 */ vertex pos;
    /* 0x10 */ s16 lifetime;
    /* 0x14 */ f32 radius;     /* spawn points lie within this distance of pos */
    /* 0x18 */ s16 yawBase;
    /* 0x1A */ s16 yawRange;
    /* 0x1C */ s16 pitchBase;
    /* 0x1E */ s16 pitchRange;
    /* 0x20 */ f32 speedBase;
    /* 0x24 */ f32 speedRange;
    /* 0x28 */ f32 velYBase;   /* mode 0 only */
    /* 0x2C */ f32 velYRange;
    /* 0x30 */ vertex dir;     /* modes 2 and 3 */
    /* 0x3C */ f32 sizeBase;
    /* 0x40 */ f32 sizeRange;
    /* 0x44 */ f32 gravityBase;
    /* 0x48 */ f32 gravityRange;
    /* 0x4C */ f32 spinBase;
    /* 0x50 */ f32 spinRange;
    /* 0x54 */ u8 landPuffs;   /* see DebrisData */
    /* 0x55 */ u8 landPuffFlag;
    /* 0x56 */ s16 landPuffLifeBase;
    /* 0x58 */ s16 landPuffLifeRange;
} DebrisEmitter;

/* Spawn template for func_15132A4C: the layout of Particle 0x10..0x8B (see game_15F680.c). */
typedef struct {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ vertex rot;
    /* 0x1C */ vertex scale;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 variant;
    /* 0x5C */ s16 *unk5C;
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ s8 drawFn;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[6];
} ParticleSpawn; // size 0x7C

/* Spawn parameters of a BloodTrail effect (see game_161520.c). */
typedef struct {
    /* 0x00 */ f32 *x;
    /* 0x04 */ f32 *y;
    /* 0x08 */ f32 *z;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 rate;
    /* 0x14 */ s16 lifetime;
    /* 0x16 */ u8 flags;
    /* 0x17 */ u8 emitter;     /* D_80089AFC index */
} BloodTrailDesc; // matching: the callee copies 0x1C bytes (two more fields), but this frame holds only 0x18

/* A BloodTrail effect; the effect header's bytes 1 and 0xC are passed on to the drops it emits. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x40 - 0xD];
    /* 0x40 */ f32 size;       /* extra data: the size of the debris piece it follows */
} BloodTrail;

/* Per-particle data of the debris pieces, at 0x170 of the particle. */
typedef struct {
    /* 0x0 */ f32 stopSpeed;   /* landing slower than this (vel.y above it) stops the piece */
    /* 0x4 */ BloodTrail *trail;
    /* 0x8 */ u8 landPuffs;    /* spawn func_1514AB5C particles on landing */
    /* 0x9 */ u8 landPuffFlag;
    /* 0xA */ s16 landPuffLifeBase;
    /* 0xC */ s16 landPuffLifeRange;
} DebrisData;

/* A live func_15132A4C particle (layout as Particle in game_15F680.c). */
typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ f32 radius;
    /* 0x014 */ f32 elasticity;
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ vertex rot;
    /* 0x02C */ vertex scale;
    /* 0x038 */ vertex pos;
    /* 0x044 */ vertex vel;
    /* 0x050 */ vertex rotVel;
    /* 0x05C */ f32 gravity;
    /* 0x060 */ u32 flags;
    /* 0x064 */ s16 lifetime;
    /* 0x066 */ u8 pad66[0x170 - 0x66];
    /* 0x170 */ DebrisData debris;
} Particle;

extern Particle *func_15132A4C(ParticleSpawn *, s32, s32, s32, u8, s32);
extern BloodTrail *func_15134908(BloodTrailDesc *, s32, u8, s32);

/*
 * Spawns emitter->count debris particles at random points within emitter->radius of emitter->pos,
 * each followed by a BloodTrail. mode picks the launch velocity: 0 = horizontal speed along the yaw
 * plus a separate vertical speed, 1 = speed along the spawn direction, 2 = dir, 3 = dir * speed.
 */
void func_151A6F00(DebrisEmitter *emitter, u8 mode, u8 arg2, s32 arg3) {
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
    Particle *particle;

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
        /* func_151423D8 is a cosine, so cos(a - 64) is sin(a) */
        sinYaw = func_151423D8((u8)(yaw - 64));
        cosYaw = func_151423D8(yaw);
        sinPitch = func_151423D8((u8)(pitch - 64));
        cosPitch = func_151423D8(pitch);
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
        particle = func_15132A4C(&spawn, 0, 0, sizeof(DebrisData), arg2, arg3);
        if (particle != NULL) {
            BloodTrailDesc trailDesc;
            DebrisData *data;
            BloodTrail *trail;
            data = &particle->debris;
            memcpy(data, &debris, sizeof(DebrisData));
            trailDesc.x = &particle->pos.x;
            trailDesc.y = &particle->pos.y;
            trailDesc.z = &particle->pos.z;
            trailDesc.radius = 15.0f * spawn.sizeY;
            trailDesc.lifetime = 300;
            trailDesc.flags = 0;
            trailDesc.emitter = 1;
            trailDesc.rate = 0.3580000103f;
            data->trail = NULL;
            trail = func_15134908(&trailDesc, sizeof(f32), arg2, arg3);
            data->trail = trail;
            if (trail != NULL) {
                memcpy(&trail->size, &size, sizeof(f32));
            }
        }
    }
}

/* Particle update2Fn of the debris pieces: drops the blood trail once the piece starts fading out. */
s32 func_151A73EC(Particle *particle) {
    DebrisData *data = &particle->debris;

    if (particle->lifetime < 32) {
        if (data->trail != NULL) {
            func_1516972C((struct102 *)data->trail);
            data->trail = NULL;
        }
    }
    return 1;
}

void func_1514AB5C(f32 x, f32 y, f32 z, f32 radius, s16 count, f32 sizeBase, f32 sizeRange,
                   f32 heightBase, f32 heightRange, s16 lifeBase, s16 lifeRange, u8 arg11);

/*
 * Particle floorHitFn of the debris pieces. A slow landing stops the piece (and starts its fade-out),
 * optionally with a puff of func_1514AB5C particles; a fast one bounces it.
 */
s32 func_151A743C(Particle *particle, f32 x, f32 y, f32 z, f32 floorY) {
    DebrisData *data;
    u8 puffFlag;

    data = &particle->debris;
    particle->pos.y = particle->radius + floorY;
    if (data->trail != NULL) {
        func_1516972C((struct102 *)data->trail);
        data->trail = NULL;
    }
    if (data->stopSpeed < particle->vel.y) {
        particle->flags &= ~7;
        particle->flags &= ~8;
        particle->flags &= ~0x40;
        particle->flags &= ~0x20;
        particle->vel.x = 0.0f;
        particle->vel.y = 0.0f;
        particle->vel.z = 0.0f;
        if (particle->lifetime > 32) {
            particle->lifetime = 32;
        }
        if (data->landPuffs) {
            puffFlag = FALSE;
            if (data->landPuffFlag) {
                puffFlag = TRUE;
            }
            func_1514AB5C(x, floorY, z, particle->sizeXZ * 35.0f, func_150ADA20() % 3 + 2,
                          particle->sizeXZ, particle->sizeXZ * 0.5f,
                          particle->sizeXZ, particle->sizeXZ * 0.5f,
                          data->landPuffLifeBase, data->landPuffLifeRange, puffFlag);
        }
    } else {
        particle->vel.x *= particle->elasticity;
        particle->vel.y = -particle->vel.y * particle->elasticity;
        particle->vel.z *= particle->elasticity;
        particle->rotVel.x *= particle->elasticity;
        particle->rotVel.y *= particle->elasticity;
        particle->rotVel.z *= particle->elasticity;
    }
    return 1;
}

s32 func_151A26EC(f32 *pos, f32 *offset, f32 *vel, f32 drag, f32 gravity, f32 size, s16 timer, u8 alpha,
                  s16 fadeStart, s16 fadeOutStart, s16 eventTime, s8 eventFunc, u8 r, u8 g, u8 b,
                  u8 argF, s32 arg10);

extern f32 D_800A8DC8; // matching: 0.001f; as a literal IDO CSEs it with the second 0.001f

/*
 * BloodTrail emitter 1 (the debris trails of func_151A6F00): emits one blood drop at (x, y, z),
 * flung back against the trail's motion (dx, dy, dz) and sized by the debris piece.
 */
void func_151A7610(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, BloodTrail *trail) {
    f32 pos[3];
    f32 vel[3];
    f32 offset[3];
    f32 speed;
    f32 size;
    struct {
        f32 gravity;
        f32 size;
        u32 timer;
        u32 pad[2]; // matching: the draws sit 8 bytes lower in the frame
    } rand;
    f32 dt;

    size = trail->size;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    // matching: an early (empty) use of dt makes IDO give it $f12 and speed $f2
    do { } while (0);
    if (dt) {
    }
    speed = func_150ADA68();
    dt = D_800BE9A8;
    speed = speed * 166.0f + 160.0f;
    offset[0] = 0.0f;
    offset[1] = 0.0f;
    offset[2] = 0.0f;
    speed *= D_800A8DC8;
    vel[0] = -dx * dt * speed;
    vel[1] = -dy * dt * speed;
    vel[2] = -dz * dt * speed;
    rand.gravity = func_150ADA68();
    rand.size = func_150ADA68();
    rand.timer = func_150ADA20();
    // matching: `+ -600.0f` (add.s); `- 600.0f` would emit sub.s
    func_151A26EC(pos, offset, vel, 1.0f, (rand.gravity * 591.0f + -600.0f) * 0.001000000047f,
                  (rand.size * 500.0f + 400.0f) * size, rand.timer % 21 + 15, func_150ADA20() % 101 + 100,
                  15, 20, 0, -1, 86, 39, 0, trail->unkC, trail->unk1);
}

/* Billboard particle created by func_15167D84 (layout as Particle in game_1944C0.c). */
typedef struct {
    /* 0x00 */ u8 link[0x10];
    /* 0x10 */ void *anim;
    /* 0x14 */ s32 unk14;       /* PARTICLE_CUSTOM_MODE: bits 0-7 render mode, bits 16-31 combine mode */
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s16 frame;
    /* 0x1E */ s16 frameStep;
    /* 0x20 */ s16 x;
    /* 0x22 */ s16 y;
    /* 0x24 */ s16 z;
    /* 0x26 */ s16 velX;
    /* 0x28 */ s16 velZ;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ u8 subX;
    /* 0x2D */ u8 subZ;
    /* 0x2E */ u8 subY;
    /* 0x2F */ s8 callback;     /* index into D_8008C9C8, -1 = none */
    /* 0x30 */ s16 velY;
    /* 0x32 */ s16 accelY;
    /* 0x34 */ s16 width;
    /* 0x36 */ s16 height;
    /* 0x38 */ s16 life;        /* counts down */
    /* 0x3A */ s8 fadeStep;
    /* 0x3B */ u8 pad3B;
    /* 0x3C */ u8 primR;
    /* 0x3D */ u8 primG;
    /* 0x3E */ u8 primB;
    /* 0x3F */ u8 primA;
    /* 0x40 */ u8 envR;
    /* 0x41 */ u8 envG;
    /* 0x42 */ u8 envB;
    /* 0x43 */ u8 pad43;
    /* 0x44 */ u16 flags;
    /* 0x46 */ u8 pad46[2];
    /* 0x48 */ s8 onDeath;
    /* 0x49 */ u8 pad49[7];
} SpriteParticle; // the spawner's extra data follows at 0x50

#define PARTICLE_ENV_COLOR   0x1
#define PARTICLE_CUSTOM_MODE 0x100
#define PARTICLE_2CYCLE      0x200

/* Extra data of the particles run by callbacks 0x14 and 0x15. The *Start fields are values of life. */
typedef struct {
    /* 0x0 */ s16 tintStart;    /* from here the env colour follows life (callback 0x15) */
    /* 0x2 */ s16 tintStep;
    /* 0x4 */ s16 fadeOutStart;
    /* 0x6 */ s16 fadeOutStep;
    /* 0x8 */ s16 swellStart;   /* from here the size grows by swellRate per frame */
    /* 0xA */ s16 swellRate;
} SwellParticleData;

/* Callback 0x14: fade out and swell; switches to callback 0x15 to tint. */
void func_151A77C0(SpriteParticle *particle) {
    SwellParticleData *data = (SwellParticleData *)(particle + 1);

    if (particle->life < data->fadeOutStart) {
        particle->primA = particle->life * data->fadeOutStep;
    }
    if (particle->life < data->swellStart) {
        s16 growth = data->swellRate * D_800BE9E4;

        particle->width += growth;
        particle->height += growth;
    }
    if (particle->life < data->tintStart) {
        particle->callback = 0x15;
        particle->flags |= PARTICLE_ENV_COLOR | PARTICLE_2CYCLE;
        particle->envR = particle->envG = particle->envB = particle->life * data->tintStep;
        particle->unk14 = 0xC000F;
    }
}

/* Callback 0x15: func_151A77C0 after the switch, with the env colour following life. */
void func_151A787C(SpriteParticle *particle) {
    SwellParticleData *data = (SwellParticleData *)(particle + 1);

    if (particle->life < data->fadeOutStart) {
        particle->primA = particle->life * data->fadeOutStep;
    }
    if (particle->life < data->swellStart) {
        s16 growth = data->swellRate * D_800BE9E4;

        particle->width += growth;
        particle->height += growth;
    }
    particle->envR = particle->envG = particle->envB = particle->life * data->tintStep;
}

/* Particle destroyFn of the debris pieces: removes the blood trail. */
void func_151A7908(Particle *particle) {
    DebrisData *data = &particle->debris;

    if (data->trail != NULL) {
        func_1516972C((struct102 *)data->trail);
        data->trail = NULL;
    }
}
