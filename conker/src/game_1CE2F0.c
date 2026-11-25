#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15143794(s32, s32, f32, f32 *);
void func_15152B38(void *, u8, s32);
s32 func_1513170C(s32 *, s32);
struct Obj151A2960;
extern void (*D_8008F8E0[])(struct Obj151A2960 *, s32);
extern f32 D_800A8D30;
extern f32 D_800A8D34;
extern f32 D_800A8D38;
void func_151A11CC(f32, f32, f32, f32, f32, s32, s32, s32,
                   s32, s32, f32, s32, s32, s32, s32, s32,
                   s32, f32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32);

void func_151A0E40(f32 x, f32 y, f32 z, u8 arg3, s32 arg4) {
    func_151A11CC(x, y, z, 163.0f, 2.019000053f, 160, 75, 1055, 1270,
                  1, 2.019000053f, 135, 68, 1055, 1270, 2,
                  1, 2.019000053f, 70, 66, 1055, 1270, 7, 65,
                  4, 1, arg3, arg4);
}

void func_151A0F28(f32 x, f32 y, f32 z, u8 arg3, s32 arg4) {
    func_151A11CC(x, y, z, 163.0f, 2.019000053f, 160, 75, 1055, 1270,
                  1, 2.019000053f, 135, 68, 1055, 1270, 2,
                  1, 2.019000053f, 70, 66, 1055, 1270, 7, 65,
                  4, 1, arg3, arg4);
}

extern f32 D_800A8D28;

/* Calls func_151A11CC with parameters scaled by scaleA/scaleB and a duration-derived count. */
void func_151A1010(f32 x, f32 y, f32 z, s16 duration, f32 scaleA, f32 scaleB, u8 arg6, s32 arg7) {
    s16 count;
    s32 offset;
    s16 a;
    s16 b;
    s16 c;
    s16 d;
    s32 a32;
    s32 b32;
    s32 c32;
    s32 d32;

    count = duration - 32;
    offset = (s16)(duration - 32);
    if (offset <= 0) {
        count = 1;
    }
    // matching: the dead `| (D_800A8D28 == 0 & 0)` read keeps uopt from merging each s32 into its s16 copy
    func_151A11CC(x, y, z, 250.0f * scaleA, 1.0f,
                  a = a32 = (s32)(70.0f * scaleB) | (D_800A8D28 == 0 & 0),
                  b = b32 = (s32)(30.0f * scaleB) | (D_800A8D28 == 0 & 0),
                  c = c32 = D_800A8D28 * scaleA,
                  d = d32 = (s32)(100.0f * scaleA) | (D_800A8D28 == 0 & 0), 18, 1.0f,
                  90.0f * scaleB, 20.0f * scaleB, c, d,
                  count, 16, 1.0f,
                  a, b, c, d,
                  15.0f * scaleA, 10.0f * scaleA, 18, offset + 16, arg6, arg7);
}

void func_151A11CC(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                  f32 arg4, s32 arg5, s32 arg6, s32 arg7,
                  s32 arg8, s32 arg9, f32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, s32 arg14, s32 arg15,
                  s32 arg16, f32 arg17, s32 arg18, s32 arg19,
                  s32 arg20, s32 arg21, s32 arg22, s32 arg23,
                  s32 arg24, s32 arg25, s32 arg26, s32 arg27) {
}

/* Sprite animation (as in game_1944C0.c): one texture, or a table of frameCount textures. */
typedef struct {
    /* 0x0 */ s32 *frames;
    /* 0x4 */ u8 frameCount;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u16 unk6;
    /* 0x8 */ u16 unk8;
    /* 0xA */ u8 unkA;
} SpriteAnim;

/* 0x38-byte particle template; func_15167D84 copies it to Particle.anim onwards. */
typedef struct {
    /* 0x00 */ SpriteAnim *anim;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s16 frame;
    /* 0x0E */ s16 frameStep;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 velX;
    /* 0x18 */ s16 velZ;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ u8 subX;
    /* 0x1D */ u8 subZ;
    /* 0x1E */ u8 subY;
    /* 0x1F */ s8 callback;
    /* 0x20 */ s16 velY;
    /* 0x22 */ s16 accelY;
    /* 0x24 */ s16 width;
    /* 0x26 */ s16 height;
    /* 0x28 */ s16 life;
    /* 0x2A */ s8 fadeStep;
    /* 0x2B */ u8 pad2B;
    /* 0x2C */ u8 primR;
    /* 0x2D */ u8 primG;
    /* 0x2E */ u8 primB;
    /* 0x2F */ u8 primA;
    /* 0x30 */ u8 envR;
    /* 0x31 */ u8 envG;
    /* 0x32 */ u8 envB;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ u16 flags;
    /* 0x36 */ u8 pad36[2];
} ParticleTemplate;

/* Particle object (layout as in game_1944C0.c); the spawner's extra data follows it at 0x50. */
typedef struct {
    /* 0x00 */ u8 link[0x10];
    /* 0x10 */ SpriteAnim *anim;
    /* 0x14 */ s32 unk14;
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
    /* 0x2F */ s8 callback;
    /* 0x30 */ s16 velY;
    /* 0x32 */ s16 accelY;
    /* 0x34 */ s16 width;
    /* 0x36 */ s16 height;
    /* 0x38 */ s16 life;
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
} Particle;

#define PARTICLE_ENV_COLOR   0x1
#define PARTICLE_FLIP_S      0x4
#define PARTICLE_FLIP_T      0x8
#define PARTICLE_CUSTOM_MODE 0x100

/* Emitter settings at 0x28 of the emitter objects below. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 radius;
    /* 0x10 */ f32 spawnAccum;  /* += spawnRate * frame time; one particle per whole unit */
    /* 0x14 */ f32 spawnRate;
    /* 0x18 */ s16 frameStep;
    /* 0x1A */ s16 frameStepRange;
    /* 0x1C */ s16 size;
    /* 0x1E */ s16 sizeRange;
    /* 0x20 */ s16 growRate;
    /* 0x22 */ s16 growRateRange;
} EmitterParams;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ EmitterParams params;
} Emitter;

/*
 * Extra data of the particles spawned by func_151A11E4, read by its callback func_151A175C.
 * The *Start/*End fields are values of Particle.life, which counts down.
 */
typedef struct {
    /* 0x00 */ s16 fadeInStep;
    /* 0x02 */ s16 fadeInStart;
    /* 0x04 */ s16 fadeInEnd;
    /* 0x06 */ s16 growStep;
    /* 0x08 */ s16 growStart;
    /* 0x0A */ s16 growEnd;
    /* 0x0C */ s16 moveEnd;
    /* 0x0E */ s16 velX;    /* 8.8 fixed point: integer parts here, fractions in velSub* */
    /* 0x10 */ s16 velY;
    /* 0x12 */ s16 velZ;
    /* 0x14 */ s16 velSubX;
    /* 0x16 */ s16 velSubY;
    /* 0x18 */ s16 velSubZ;
    /* 0x1A */ s16 fadeOutStart;
    /* 0x1C */ s16 fadeOutStep;
    /* 0x1E */ s16 shrinkStart;
    /* 0x20 */ s16 shrinkStep;
} DriftParticleData;

extern SpriteAnim D_80090B60[];
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);

/* Spawns particles at random points in a sphere around the emitter; each drifts back to the centre. */
void func_151A11E4(Emitter *emitter) {
    EmitterParams *params;
    ParticleTemplate packet;
    DriftParticleData data;
    u8 angle1;
    u8 angle2;
    f32 cos1;
    f32 sin1;
    f32 cos2;
    f32 sin2;
    f32 dist;
    f32 horiz;
    f32 tz;
    f32 ty;
    f32 tx;
    s16 size;
    s16 life;
    f32 inv;
    f32 vx;
    Particle *particle;

    emitter->params.spawnAccum += emitter->params.spawnRate * D_800BE9A4;
    params = &emitter->params;
    if (emitter->params.spawnAccum > 1.0f) {
        packet.primR = 255;
        packet.primG = 255;
        packet.primB = 255;
        packet.envR = 0;
        packet.envG = 0;
        packet.envB = 0;
        packet.callback = 0x10;
        packet.flags = 0x120;
        packet.unk4 = 0xC0001;
        packet.frame = 0;
        packet.anim = &D_80090B60[39];
        packet.velX = 0;
        packet.velY = 0;
        packet.velZ = 0;
        packet.accelY = 0;
        packet.fadeStep = 0;
        packet.primA = 0;
        packet.height = 2000;
        packet.width = 2000;
        do {
            angle1 = func_150ADA20() & 255;
            angle2 = func_150ADA20() & 255;
            cos1 = func_151423D8(angle1 - 64);
            sin1 = func_151423D8(angle1);
            cos2 = func_151423D8(angle2 - 64);
            sin2 = func_151423D8(angle2);
            dist = func_150ADA68() * params->radius;
            horiz = dist * sin2;
            tx = params->x + horiz * cos1;
            ty = params->y - dist * cos2;
            tz = params->z + horiz * sin1;
            if (func_150ADA20() & 1) {
                packet.flags |= PARTICLE_FLIP_S;
            }
            if (func_150ADA20() & 1) {
                packet.flags |= PARTICLE_FLIP_T;
            }
            packet.frameStep = func_150ADA20() % (u32)(params->frameStepRange + 1) + params->frameStep;
            size = func_150ADA20() % (u32)(params->sizeRange + 1) + params->size;
            packet.life = D_80090B60[39].frameCount * (256 / packet.frameStep);
            life = packet.life;
            data.growStart = data.fadeInStart = life;
            data.fadeInEnd = life - (life >> 2);
            data.growEnd = life >> 1;
            data.fadeInStep = 255 / (data.fadeInStart - data.fadeInEnd);
            data.growStep = size / (data.growStart - data.growEnd);
            data.fadeOutStart = life >> 2;
            data.fadeOutStep = 255 / data.fadeOutStart;
            data.shrinkStart = life >> 1;
            data.shrinkStep = size / data.shrinkStart;
            packet.x = params->x;
            packet.y = params->y;
            packet.z = params->z;
            packet.subX = (s32)(params->x * 256.0f) & 0xFF;
            packet.subY = (s32)(params->y * 256.0f) & 0xFF;
            packet.subZ = (s32)(params->z * 256.0f) & 0xFF;
            data.moveEnd = life >> 1;
            inv = 1.0f / data.moveEnd;
            vx = (tx - params->x) * inv;
            // matching: vy and vz reuse horiz and dist (separate locals change the frame)
            // matching: an empty block boundary here makes IDO keep vx in its own register (mov.s)
            do { } while (0);
            horiz = (ty - params->y) * inv;
            dist = (tz - params->z) * inv;
            data.velX = (s32)vx;
            data.velY = (s32)horiz;
            data.velZ = (s32)dist;
            data.velSubX = (u8)(s32)(vx * 256.0f);
            data.velSubY = (u8)(s32)(horiz * 256.0f);
            data.velSubZ = (u8)(s32)(dist * 256.0f);
            particle = (Particle *)func_15167D84(&packet, 0, sizeof(data), -1, emitter->unkC, emitter->unk1);
            if (particle != NULL) {
                memcpy(particle + 1, &data, sizeof(data));
            }
            params->spawnAccum -= 1.0f;
            // matching: a scoped local after the decrement re-materializes 1.0f at the loop test
            { s32 q; }
        } while (params->spawnAccum > 1.0f);
    }
}

/* Callback 0x10 (func_151A11E4's particles): fade in and grow, drift, then fade out and shrink. */
void func_151A175C(Particle *particle) {
    DriftParticleData *data = (DriftParticleData *)(particle + 1);

    if (particle->life > data->fadeInEnd) {
        particle->primA = (data->fadeInStart - particle->life) * data->fadeInStep;
    }
    if (particle->life > data->growEnd) {
        particle->width = particle->height = (data->growStart - particle->life) * data->growStep;
    }
    if (particle->life > data->moveEnd) {
        s32 px, py, pz, vx, vy, vz;

        px = particle->x;
        px = (px << 8) | particle->subX;
        vx = data->velX;
        vx = (vx << 8) | data->velSubX;
        px += vx * D_800BE9E4;
        particle->x = px >> 8;
        particle->subX = px & 0xFF;
        py = particle->y;
        py = (py << 8) | particle->subY;
        vy = data->velY;
        vy = (vy << 8) | data->velSubY;
        py += vy * D_800BE9E4;
        particle->y = py >> 8;
        particle->subY = py & 0xFF;
        pz = particle->z;
        pz = (pz << 8) | particle->subZ;
        vz = data->velZ;
        vz = (vz << 8) | data->velSubZ;
        pz += vz * D_800BE9E4;
        particle->z = pz >> 8;
        particle->subZ = pz & 0xFF;
    }
    if (particle->life < data->fadeOutStart) {
        particle->primA = particle->life * data->fadeOutStep;
    }
    if (particle->life < data->shrinkStart) {
        particle->width = particle->height = particle->life * data->shrinkStep;
    }
}

/* Object that spawns a func_151491F4 effect, passing on its settings from 0x28. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Words3;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ Words3 unk28;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ s16 unk40;
    /* 0x42 */ s16 unk42;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
} EffectSpawner;

/* Data copied to 0x28 of the effect created by func_151A18DC. */
typedef struct {
    /* 0x00 */ Words3 unk0;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s32 pad20;
} EffectData151A18DC;

/* Creates a type 3 func_151491F4 effect from the spawner's settings. */
void func_151A18DC(EffectSpawner *spawner) {
    EffectData151A18DC data;
    struct260 *effect;

    data.unk0 = spawner->unk28;
    data.unkC = spawner->unk34;
    data.unk10 = 0.0f;
    data.unk14 = spawner->unk38;
    data.unk18 = spawner->unk3E;
    data.unk1A = spawner->unk40;
    data.unk1C = spawner->unk42;
    data.unk1E = spawner->unk44;

    effect = func_151491F4(spawner->unk3C, -1, 3, 1, 0, 0x20, spawner->unkC, spawner->unk1);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, 0x20);
    }
}

/* Extra data of the particles spawned by func_151A1998, read by its callback func_151A1E34. */
typedef struct {
    /* 0x00 */ s16 fadeInStep;
    /* 0x02 */ s16 fadeInStart;
    /* 0x04 */ s16 fadeInEnd;
    /* 0x06 */ s16 growStep;
    /* 0x08 */ s16 growStart;
    /* 0x0A */ s16 growEnd;
    /* 0x0C */ s16 fadeOutStart;
    /* 0x0E */ s16 fadeOutStep;
    /* 0x10 */ s16 shrinkStart;
    /* 0x12 */ s16 shrinkStep;
} PuffParticleData;

/* Spawns particles at random points in a sphere around the emitter; they stay where they are. */
void func_151A1998(Emitter *emitter) {
    EmitterParams *params;
    ParticleTemplate packet;
    PuffParticleData data;
    f32 cos1;
    f32 sin1;
    f32 cos2;
    f32 sin2;
    f32 dist;
    f32 x;
    f32 y;
    f32 z;
    u8 angle1;
    u8 angle2;
    s16 size;
    s16 life;
    Particle *particle;

    emitter->params.spawnAccum += emitter->params.spawnRate * D_800BE9A4;
    if (emitter->params.spawnAccum > 1.0f) {
        params = &emitter->params;
        packet.primR = 255;
        packet.primG = 255;
        packet.primB = 255;
        packet.envR = 0;
        packet.envG = 0;
        packet.envB = 0;
        packet.callback = 0x11;
        packet.flags = 0x120;
        packet.unk4 = 0xC0001;
        packet.frame = 0;
        packet.anim = &D_80090B60[39];
        packet.velX = 0;
        packet.velY = 0;
        packet.velZ = 0;
        packet.accelY = 0;
        packet.fadeStep = 0;
        packet.primA = 0;
        packet.width = packet.height = 2000;
        do {
            angle1 = func_150ADA20() & 255;
            angle2 = func_150ADA20() & 255;
            cos1 = func_151423D8(angle1 - 64);
            sin1 = func_151423D8(angle1);
            cos2 = func_151423D8(angle2 - 64);
            sin2 = func_151423D8(angle2);
            dist = func_150ADA68() * params->radius;
            x = params->x + dist * sin2 * cos1;
            y = params->y - dist * cos2;
            z = params->z + dist * sin2 * sin1;
            if (func_150ADA20() & 1) {
                packet.flags |= PARTICLE_FLIP_S;
            }
            if (func_150ADA20() & 1) {
                packet.flags |= PARTICLE_FLIP_T;
            }
            packet.frameStep = func_150ADA20() % (u32)(params->frameStepRange + 1) + params->frameStep;
            size = func_150ADA20() % (u32)(params->sizeRange + 1) + params->size;
            life = packet.life = D_80090B60[39].frameCount * (256 / packet.frameStep);
            data.fadeInStart = life;
            data.fadeInEnd = life - (life >> 2);
            data.fadeInStep = 255 / (data.fadeInStart - data.fadeInEnd);
            data.growStart = life;
            data.growEnd = life >> 1;
            data.growStep = size / (data.growStart - data.growEnd);
            data.fadeOutStart = life >> 2;
            data.fadeOutStep = 255 / data.fadeOutStart;
            data.shrinkStart = life >> 1;
            data.shrinkStep = size / data.shrinkStart;
            packet.x = x;
            packet.y = y;
            packet.z = z;
            packet.subX = (s32)(x * 256.0f) & 0xFF;
            packet.subY = (s32)(y * 256.0f) & 0xFF;
            packet.subZ = (s32)(z * 256.0f) & 0xFF;
            particle = (Particle *)func_15167D84(&packet, 0, sizeof(data), -1, emitter->unkC, emitter->unk1);
            if (particle != NULL) {
                memcpy(particle + 1, &data, sizeof(data));
            }
            params->spawnAccum -= 1.0f;
        } while (params->spawnAccum > 1.0f);
    }
}

/* Callback 0x11 (func_151A1998's particles): fade in and grow, then fade out and shrink. */
void func_151A1E34(Particle *particle) {
    s16 life;
    PuffParticleData *data;

    data = (PuffParticleData *)(particle + 1);
    life = particle->life;
    if (data->fadeInEnd < life) {
        particle->primA = (u32)(data->fadeInStart - life) * (u32)data->fadeInStep;
    }
    if (data->growEnd < life) {
        particle->width = particle->height = (u32)(data->growStart - life) * (u32)data->growStep;
    }
    if (life < data->fadeOutStart) {
        particle->primA = (u32)life * (u32)data->fadeOutStep;
    }
    if (life < data->shrinkStart) {
        particle->width = particle->height = (u32)life * (u32)data->shrinkStep;
    }
}

/* Data copied to 0x28 of the effect created by func_151A1EE8. */
typedef struct {
    /* 0x00 */ Words3 unk0;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ union {
        struct {
            s16 unk18;
            s16 unk1A;
            s16 unk1C;
            s16 unk1E;
        } s;
        f64 align; // matching: 8-byte alignment places this block at sp+0x38
    } u18;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
} EffectData151A1EE8;

/* Creates a type 4 func_151491F4 effect from the spawner's settings. */
void func_151A1EE8(EffectSpawner *spawner) {
    EffectData151A1EE8 data;
    struct260 *effect;

    data.unk0 = spawner->unk28;
    data.unkC = spawner->unk34;
    data.unk10 = 0.0f;
    data.unk14 = spawner->unk38;
    data.u18.s.unk18 = spawner->unk3E;
    data.u18.s.unk1A = spawner->unk40;
    data.u18.s.unk1C = spawner->unk42;
    data.u18.s.unk1E = spawner->unk44;
    data.unk20 = spawner->unk46;
    data.unk22 = spawner->unk48;

    effect = func_151491F4(spawner->unk3C, -1, 4, 1, 0, 0x24, spawner->unkC, spawner->unk1);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, 0x24);
    }
}

struct Sub151A1FB4 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
};

struct Obj151A1FB4 {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    struct Sub151A1FB4 unk28;
};

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Fade151A1FB4;

typedef struct {
    void *unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s8 unk1C;
    s8 unk1D;
    s8 unk1E;
    u8 unk1F;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    u16 unk34;
    s16 unk36;
} Packet151A1FB4;


void func_151A1FB4(struct Obj151A1FB4 *arg0) {
    struct Sub151A1FB4 *sub;
    Packet151A1FB4 pkt;
    Fade151A1FB4 fade;
    u8 a;
    u8 b;
    f32 sinA;
    f32 cosA;
    f32 sinB;
    f32 cosB;
    f32 x;
    f32 y;
    f32 z;
    f32 r;
    s16 n;
    s32 ret;

    arg0->unk28.unk10 += arg0->unk28.unk14 * D_800BE9A4;
    sub = &arg0->unk28;
    if (arg0->unk28.unk10 > 1.0f) {
        pkt.unk2C = 5;
        pkt.unk2D = 0;
        pkt.unk2E = 0;
        pkt.unk30 = 0;
        pkt.unk31 = 0;
        pkt.unk32 = 0;
        pkt.unk1F = 0x12;
        pkt.unk34 = 0x120;
        pkt.unk04 = 0xC0001;
        pkt.unk0C = 0;
        pkt.unk00 = &D_80090B60[39];
        pkt.unk16 = 0;
        pkt.unk20 = 0;
        pkt.unk18 = 0;
        pkt.unk22 = 0;
        pkt.unk2A = 0;
        pkt.unk2F = 0;
        pkt.unk26 = 0x7D0;
        pkt.unk24 = 0x7D0;
        do {
            a = func_150ADA20() & 0xFF;
            b = func_150ADA20() & 0xFF;
            cosA = func_151423D8(a - 0x40);
            sinA = func_151423D8(a);
            cosB = func_151423D8(b - 0x40);
            sinB = func_151423D8(b);
            r = func_150ADA68() * sub->unkC;
            x = sub->unk0 + r * sinB * cosA;
            y = sub->unk4 - r * cosB;
            z = sub->unk8 + r * sinB * sinA;
            if (func_150ADA20() & 1) {
                pkt.unk34 |= 4;
            }
            if (func_150ADA20() & 1) {
                pkt.unk34 |= 8;
            }
            pkt.unk0E = (func_150ADA20() % (u32)(sub->unk1A + 1)) + sub->unk18;
            n = (func_150ADA20() % (u32)(sub->unk1E + 1)) + sub->unk1C;
            pkt.unk28 = D_80090B60[39].frameCount * (0x100 / pkt.unk0E);
            fade.unk2 = pkt.unk28;
            fade.unk8 = pkt.unk28;
            fade.unk4 = pkt.unk28 - (pkt.unk28 >> 2);
            fade.unk0 = 0xFF / (pkt.unk28 - fade.unk4);
            fade.unkA = pkt.unk28 >> 1;
            fade.unk6 = n / (pkt.unk28 - (s16)(pkt.unk28 >> 1));
            fade.unkC = pkt.unk28 >> 1;
            fade.unkE = 0xFF / (pkt.unk28 >> 1);
            fade.unk10 = pkt.unk28 >> 2;
            fade.unk12 = 0xFF / (pkt.unk28 >> 2);
            fade.unk14 = pkt.unk28 >> 1;
            fade.unk16 = (func_150ADA20() % (u32)(sub->unk22 + 1)) + sub->unk20;
            fade.unk18 = pkt.unk28 >> 1;
            fade.unk1A = (func_150ADA20() % 81U) + 0x50;
            pkt.unk10 = x;
            pkt.unk12 = y;
            pkt.unk14 = z;
            pkt.unk1C = ((s32)(x * 256.0f)) & 0xFF;
            pkt.unk1E = ((s32)(y * 256.0f)) & 0xFF;
            pkt.unk1D = ((s32)(z * 256.0f)) & 0xFF;
            ret = func_15167D84(&pkt, 0, 0x1C, -1, arg0->unkC, arg0->unk1);
            if (ret != 0) {
                memcpy((u8 *)ret + 0x50, &fade, 0x1C);
            }
            sub->unk10 -= 1.0f;
        } while (sub->unk10 > 1.0f);
    }
}

typedef struct Fade151A24A8 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Fade151A24A8;

typedef struct Obj151A24A8 {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x2F - 0x18];
    u8 unk2F;
    u8 pad30[0x2];
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[0x5];
    u8 unk3F;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 pad43;
    u16 unk44;
    u8 pad46[0xA];
    Fade151A24A8 unk50;
} Obj151A24A8;

void func_151A24A8(Obj151A24A8 *arg0) {
    Fade151A24A8 *s = &arg0->unk50;

    if (arg0->unk38 > s->unk4) {
        arg0->unk3F = (s->unk2 - arg0->unk38) * s->unk0;
    }
    if (arg0->unk38 > s->unkA) {
        arg0->unk34 = arg0->unk36 = (s->unk8 - arg0->unk38) * s->unk6;
    }
    if (arg0->unk38 < s->unk10) {
        arg0->unk3F = arg0->unk38 * s->unk12;
    }
    if (arg0->unk38 < s->unk14) {
        s16 d = s->unk16 * D_800BE9E4;

        arg0->unk34 += d;
        arg0->unk36 += d;
    }
    if (arg0->unk38 < s->unkC) {
        arg0->unk2F = 0x13;
        arg0->unk44 |= 0x101;
        arg0->unk40 = arg0->unk41 = arg0->unk42 = arg0->unk38 * s->unkE;
        arg0->unk14 = 0x520003;
    }
    if (arg0->unk38 < s->unk18) {
        arg0->unk32 = s->unk1A;
        s->unk18 = -9999;
    }
}

typedef struct Fade151A25E0 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
} Fade151A25E0;

typedef struct Obj151A25E0 {
    u8 pad0[0x32];
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    u8 pad3A[0x5];
    u8 unk3F;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 pad43[0xD];
    Fade151A25E0 unk50;
} Obj151A25E0;

void func_151A25E0(Obj151A25E0 *arg0) {
    Fade151A25E0 *s = &arg0->unk50;

    if (arg0->unk38 > s->unk4) {
        arg0->unk3F = (s->unk2 - arg0->unk38) * s->unk0;
    }
    if (arg0->unk38 > s->unkA) {
        arg0->unk34 = arg0->unk36 = (s->unk8 - arg0->unk38) * s->unk6;
    }
    if (arg0->unk38 < s->unk10) {
        arg0->unk3F = arg0->unk38 * s->unk12;
    }
    if (arg0->unk38 < s->unk14) {
        s16 d = s->unk16 * D_800BE9E4;

        arg0->unk34 += d;
        arg0->unk36 += d;
    }
    if (arg0->unk38 < s->unk18) {
        arg0->unk32 = s->unk1A;
        s->unk18 = -9999;
    }
    arg0->unk40 = arg0->unk41 = arg0->unk42 = arg0->unk38 * s->unkE;
}

typedef struct {
    f32 x, y, z;
} Vec151A26EC;

typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    Vec151A26EC unk30;
    Vec151A26EC unk3C;
    Vec151A26EC unk48;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 pad66[0xA];
} Fx151A26EC;

typedef struct {
    f32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s8 unkA;
} Extra151A26EC;

s32 func_151A26EC(Vec151A26EC *arg0, Vec151A26EC *arg1, Vec151A26EC *arg2, f32 arg3, f32 arg4, f32 arg5,
                  s16 arg6, u8 arg7, s16 arg8, s16 arg9, s16 argA, s8 argB, u8 argC, u8 argD, u8 argE,
                  u8 argF, s32 arg10) {
    Extra151A26EC sp9C;
    struct260 *ret;
    Fx151A26EC sp28;

    if (arg8 <= 0) {
        arg8 = 1;
    }
    if (arg9 <= 0) {
        arg9 = 1;
    }
    sp9C.unk0 = arg3;
    sp9C.unk4 = arg9;
    sp9C.unk6 = 0xFF / arg9;
    sp28.unk1E = arg8;
    sp28.unk20 = 0xFF / arg8;
    sp9C.unk8 = argA;
    sp9C.unkA = argB;
    sp28.unk1D = 0x27;
    sp28.unk08 = 0x5203;
    sp28.unk00 = 0x200005;
    sp28.unk04 = 0;
    sp28.unk0A = arg6;
    sp28.unk0C = 0;
    sp28.unk10 = 0;
    sp28.unk14 = argC;
    sp28.unk15 = argD;
    sp28.unk16 = argE;
    sp28.unk17 = 0xFF;
    sp28.unk18 = 0xFF;
    sp28.unk19 = 0xFF;
    sp28.unk1A = 0xFF;
    sp28.unk1B = arg7;
    sp28.unk1C = 0xFF;
    sp28.unk2C = arg5;
    sp28.unk28 = arg5;
    sp28.unk30 = *arg0;
    sp28.unk3C = *arg1;
    sp28.unk48 = *arg2;
    sp28.unk22 = 1;
    sp28.unk54 = arg4;
    sp28.unk24 = 1.0f;
    sp28.unk58 = (func_150ADA20() & 1 ? 0x40 : 0) | (1 | 2 | 4 | (func_150ADA20() & 1 ? 0x80 : 0)) | 0x200 | 0x1000 | 0x4000 | 0x8000 | 0x800000;
    sp28.unk60 = 7;
    sp28.unk61 = 2;
    sp28.unk62 = 0x11;
    sp28.unk63 = -1;
    sp28.unk64 = -1;
    sp28.unk65 = 2;
    ret = func_15130374(&sp28, 1, 0x10, argF, arg10);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0xA8, &sp9C.unk0, 8);
        memcpy((u8 *)ret + 0xB0, &sp9C.unk4, 8);
    }
    return 0;
}

struct Sub151A2960 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s8 unk6;
};

struct Obj151A2960 {
    u8 pad0[0x1A];
    s16 unk1A;
    u8 pad1C[0x2C - 0x1C];
    u8 unk2C;
    u8 pad2D[0xB0 - 0x2D];
    struct Sub151A2960 unkB0;
};

void func_151A2960(struct Obj151A2960 *arg0, s32 arg1) {
    struct Sub151A2960 *temp_v0;

    temp_v0 = &arg0->unkB0;
    if (arg0->unk1A < temp_v0->unk0) {
        arg0->unk2C = arg0->unk1A * temp_v0->unk2;
    }
    if (D_800BE616 == 0) {
        if (-1 != temp_v0->unk4) {
            if ((-1 != temp_v0->unk6) && (temp_v0->unk4 >= arg0->unk1A)) {
                D_8008F8E0[temp_v0->unk6](arg0, temp_v0->unk4);
                temp_v0->unk4 = -1;
            }
        }
    }
    func_1513170C((s32 *)arg0, arg1);
}

void func_151A2C24(s32, s32, s16, s16, f32, f32, f32, s32, f32, f32, f32, f32, s16, s16, s16, s16, s16, s16, s8, u8, s32, s32);

void func_151A2A14(s32 a0, s16 a1, s16 a2, f32 a3, f32 p4, f32 p5, s32 p6, f32 p7, f32 p8, f32 p9, f32 p10, s16 p11, s16 p12, s16 p13, s16 p14, s16 p15, s16 p16, s8 p17, u8 p18, s32 p19) {
    func_151A2C24(a0, 0, a1, a2, a3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, 1);
}

void func_151A2AD4(s32 a0, s32 a1, f32 a2, f32 a3, s32 p4, f32 p5, f32 p6, f32 p7, f32 p8, s16 p9, s16 p10, s16 p11, s16 p12, s16 p13, s16 p14, s8 p15, u8 p16, s32 p17) {
    func_151A2C24(a0, a1, 0, 0, 0.0f, a2, a3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, 0);
}

void func_151A2B84(f32 *in, s32 a1, s32 a2, s32 a3, f32 t, f32 *out) {
    f32 s = 1.0f - t;
    out[0] = in[0] * s;
    out[1] = in[1] * s;
    out[2] = in[2] * s;
}

void func_151A2BD0(f32 *in, s32 a1, s32 a2, f32 a3, f32 t, f32 *out) {
    func_15143794((s16) a1, (s16) a2, (1.0f - t) * a3, out);
}

typedef struct {
    u8 c[4][3];
} Col151A2C24;
extern Col151A2C24 D_8008F8E8;

void func_151A2C24(s32 arg0, s32 arg1, s16 arg2, s16 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7, f32 arg8, f32 arg9, f32 argA, f32 argB, s16 argC, s16 argD, s16 argE, s16 argF, s16 arg10, s16 arg11, s8 arg12, u8 arg13, s32 arg14, s32 arg15) {
    s16 minLife;
    s16 delay;
    f32 t;
    f32 step;
    void (*fn)(s32, s32, s32, f32, f32, f32 *);
    f32 pos[3];
    f32 vel[3];
    s32 c;
    Col151A2C24 colors;

    if ((u8) arg15 == 0) {
        fn = (void (*)(s32, s32, s32, f32, f32, f32 *)) func_151A2B84;
    } else {
        fn = (void (*)(s32, s32, s32, f32, f32, f32 *)) func_151A2BD0;
    }
    if (arg10 < arg11) {
        minLife = arg10;
    } else {
        minLife = arg11;
    }
    t = 0.0f;
    step = 1.0f / (f32) arg7;
    while (arg7 != 0) {
        f32 r;
        s32 a;
        s32 b;

        delay = (func_150ADA20() % (u32) (argD + 1)) + argC;
        a = func_150ADA20();
        b = func_150ADA20();
        func_15143794((s16) (a & 0xFF), (s16) (b & 0xFF), func_150ADA68() * arg8, pos);
        fn(arg1, arg2, arg3, arg4, t, vel);
        colors = D_8008F8E8;
        c = func_150ADA20() & 3;
        r = func_150ADA68();
        a = func_150ADA20();
        if (func_150ADA68() < 0.0802000016f) {
            b = 0x19;
        } else {
            b = -1;
        }
        func_151A26EC((Vec151A26EC *) arg0, (Vec151A26EC *) pos, (Vec151A26EC *) vel, arg9, (r * argB) + argA, (arg6 * t) + arg5, delay + minLife, (a % (u32) (argF + 1)) + argE, arg10, arg11, b, arg12, colors.c[c][0], colors.c[c][1], colors.c[c][2], arg13, arg14);
        t += step;
        arg7--;
    }
}

void func_151A2F0C(void *arg0) {
    struct Vec151A2F0C {
        f32 a;
        f32 b;
        f32 c;
    };
    struct Obj151A2F0C {
        u8 pad0[0x1];
        u8 unk1;
        u8 pad2[0xC - 0x2];
        u8 unkC;
        u8 padD[0x38 - 0xD];
        f32 unk38;
        f32 unk3C;
        struct Vec151A2F0C unk40;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
        u8 pad58[0x68 - 0x58];
        s32 unk68;
    };
    struct Arg151A2F0C {
        s32 unk00;
        s32 unk04;
        struct Vec151A2F0C unk08;
        f32 unk14;
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        f32 unk24;
        f32 unk28;
        s16 unk2C;
        s16 unk2E;
        s16 unk30;
        s16 unk32;
        s32 unk34;
        s32 unk38;
        s16 unk3C;
        s16 unk3E;
        s16 unk40;
        u8 unk42;
        u8 unk43;
        u8 unk44;
        u8 unk45;
        u8 unk46;
        u8 unk47;
        u8 unk48;
        u8 unk49;
        u8 unk4A;
        u8 unk4B;
        u8 unk4C;
        u8 unk4D;
        u8 unk4E;
        u8 unk4F;
        u8 unk50;
        u8 unk51;
        u8 unk52;
        u8 unk53;
        u8 unk54;
        u8 unk55;
        u8 unk56;
        u8 unk57;
        u8 unk58;
        u8 pad59[3];
        s32 unk5C;
        s32 unk60;
        s16 unk64;
        s16 unk66;
        s16 unk68;
        u8 unk6A;
        u8 pad6B;
        f32 unk6C;
        s8 unk70;
        s8 unk71;
        s8 unk72;
        s8 unk73;
    };
    struct Arg151A2F0C sp1C;
    struct Obj151A2F0C *obj;

    obj = arg0;
    sp1C.unk00 = 8;
    sp1C.unk04 = 6;
    sp1C.unk08 = obj->unk40;
    if (obj->unk68 & 0x1000) {
        sp1C.unk08.a += obj->unk4C;
        sp1C.unk08.b += obj->unk50;
        sp1C.unk08.c += obj->unk54;
    }
    sp1C.unk14 = ((obj->unk38 + obj->unk3C) * 0.5f) * D_800A8D30;
    sp1C.unk18 = ((obj->unk38 + obj->unk3C) * 0.5f) * D_800A8D34;
    sp1C.unk1C = D_800A8D38;
    sp1C.unk20 = 0.0f;
    sp1C.unk24 = 8.0f;
    sp1C.unk28 = 10.0f;
    sp1C.unk2C = 0;
    sp1C.unk2E = 0xFF;
    sp1C.unk30 = -0x40;
    sp1C.unk32 = 0x56;
    sp1C.unk34 = 3;
    sp1C.unk38 = 1;
    sp1C.unk3C = 0x11;
    sp1C.unk3E = 0x12;
    sp1C.unk40 = 1;
    sp1C.unk42 = 4;
    sp1C.unk43 = 2;
    sp1C.unk44 = 3;
    sp1C.unk45 = 0xFF;
    sp1C.unk46 = 0xC8;
    sp1C.unk47 = 0xC8;
    sp1C.unk48 = 0xFF;
    sp1C.unk49 = 0;
    sp1C.unk4A = 0x37;
    sp1C.unk4B = 0x37;
    sp1C.unk4C = 0;
    sp1C.unk4D = 0xFF;
    sp1C.unk4E = 0xFF;
    sp1C.unk4F = 0xFF;
    sp1C.unk50 = 0xFF;
    sp1C.unk51 = 0;
    sp1C.unk52 = 0;
    sp1C.unk53 = 0;
    sp1C.unk54 = 0;
    sp1C.unk55 = 0xFF;
    sp1C.unk56 = 0;
    sp1C.unk57 = 1;
    sp1C.unk58 = 0x24;
    sp1C.unk5C = 0x200005;
    sp1C.unk60 = 0x60600;
    sp1C.unk64 = 7;
    sp1C.unk66 = 0x24;
    sp1C.unk68 = 1;
    sp1C.unk6A = 0;
    sp1C.unk6C = 1.0f;
    sp1C.unk70 = -1;
    sp1C.unk71 = 0;
    sp1C.unk72 = -1;
    sp1C.unk73 = -1;

    func_15152B38(&sp1C, obj->unkC, obj->unk1);
}
