#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

void func_15143794(s32, s32, f32, f32 *);
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

/* Empty in this build; func_151A0E40, func_151A0F28 and func_151A1010 call it with full parameter lists. */
void func_151A11CC(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                  f32 arg4, s32 arg5, s32 arg6, s32 arg7,
                  s32 arg8, s32 arg9, f32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, s32 arg14, s32 arg15,
                  s32 arg16, f32 arg17, s32 arg18, s32 arg19,
                  s32 arg20, s32 arg21, s32 arg22, s32 arg23,
                  s32 arg24, s32 arg25, s32 arg26, s32 arg27) {
}

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
Particle *func_15167D84(BillboardTemplate *, s32, s32, s8, u8, s32);

/* Spawns particles at random points in a sphere around the emitter; each drifts back to the centre. */
void func_151A11E4(Emitter *emitter) {
    EmitterParams *params;
    BillboardTemplate packet;
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
                packet.flags |= BILLBOARD_FLIP_S;
            }
            if (func_150ADA20() & 1) {
                packet.flags |= BILLBOARD_FLIP_T;
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
            particle = func_15167D84(&packet, 0, sizeof(data), -1, emitter->unkC, emitter->unk1);
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
} EffectData151A18DC;

/* Creates a type 3 func_151491F4 effect from the spawner's settings. */
void func_151A18DC(EffectSpawner *spawner) {
    s32 pad; // matching: unused stack slot
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
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
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
    BillboardTemplate packet;
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
                packet.flags |= BILLBOARD_FLIP_S;
            }
            if (func_150ADA20() & 1) {
                packet.flags |= BILLBOARD_FLIP_T;
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
            particle = func_15167D84(&packet, 0, sizeof(data), -1, emitter->unkC, emitter->unk1);
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
        particle->primA = (data->fadeInStart - life) * data->fadeInStep;
    }
    if (data->growEnd < life) {
        particle->width = particle->height = (data->growStart - life) * data->growStep;
    }
    if (life < data->fadeOutStart) {
        particle->primA = life * data->fadeOutStep;
    }
    if (life < data->shrinkStart) {
        particle->width = particle->height = life * data->shrinkStep;
    }
}

/* Data copied to 0x28 of the effect created by func_151A1EE8. */
typedef struct {
    /* 0x00 */ Words3 unk0;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
} EffectData151A1EE8;

/* Creates a type 4 func_151491F4 effect from the spawner's settings. */
void func_151A1EE8(EffectSpawner *spawner) {
    s32 pad; // matching: unused stack slot
    EffectData151A1EE8 data;
    struct260 *effect;

    data.unk0 = spawner->unk28;
    data.unkC = spawner->unk34;
    data.unk10 = 0.0f;
    data.unk14 = spawner->unk38;
    data.unk18 = spawner->unk3E;
    data.unk1A = spawner->unk40;
    data.unk1C = spawner->unk42;
    data.unk1E = spawner->unk44;
    data.unk20 = spawner->unk46;
    data.unk22 = spawner->unk48;

    effect = func_151491F4(spawner->unk3C, -1, 4, 1, 0, 0x24, spawner->unkC, spawner->unk1);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x28, &data, sizeof(data));
    }
}

/*
 * Extra data of the particles spawned by func_151A1FB4, read by its callbacks func_151A24A8 and
 * func_151A25E0. The *Start/*End fields are values of Particle.life, which counts down.
 */
typedef struct {
    /* 0x00 */ s16 fadeInStep;
    /* 0x02 */ s16 fadeInStart;
    /* 0x04 */ s16 fadeInEnd;
    /* 0x06 */ s16 growStep;
    /* 0x08 */ s16 growStart;
    /* 0x0A */ s16 growEnd;
    /* 0x0C */ s16 tintStart;    /* from here the env colour follows life (callback 0x13) */
    /* 0x0E */ s16 tintStep;
    /* 0x10 */ s16 fadeOutStart;
    /* 0x12 */ s16 fadeOutStep;
    /* 0x14 */ s16 swellStart;   /* from here the size grows by swellRate per frame */
    /* 0x16 */ s16 swellRate;
    /* 0x18 */ s16 accelStart;   /* once life passes it, accelY = accel */
    /* 0x1A */ s16 accel;
} SmokeParticleData;

/* Spawns particles at random points in a sphere around the emitter; they grow, tint and accelerate. */
void func_151A1FB4(Emitter *emitter) {
    EmitterParams *params;
    BillboardTemplate packet;
    SmokeParticleData data;
    u8 angle1;
    u8 angle2;
    f32 sin1;
    f32 cos1;
    f32 sin2;
    f32 cos2;
    f32 x;
    f32 y;
    f32 z;
    f32 dist;
    s16 size;
    Particle *particle;

    emitter->params.spawnAccum += emitter->params.spawnRate * D_800BE9A4;
    params = &emitter->params;
    if (emitter->params.spawnAccum > 1.0f) {
        packet.primR = 5;
        packet.primG = 0;
        packet.primB = 0;
        packet.envR = 0;
        packet.envG = 0;
        packet.envB = 0;
        packet.callback = 0x12;
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
            x = params->x + dist * sin2 * cos1;
            y = params->y - dist * cos2;
            z = params->z + dist * sin2 * sin1;
            if (func_150ADA20() & 1) {
                packet.flags |= BILLBOARD_FLIP_S;
            }
            if (func_150ADA20() & 1) {
                packet.flags |= BILLBOARD_FLIP_T;
            }
            packet.frameStep = func_150ADA20() % (u32)(params->frameStepRange + 1) + params->frameStep;
            size = func_150ADA20() % (u32)(params->sizeRange + 1) + params->size;
            packet.life = D_80090B60[39].frameCount * (256 / packet.frameStep);
            data.fadeInStart = packet.life;
            data.growStart = packet.life;
            data.fadeInEnd = packet.life - (packet.life >> 2);
            data.fadeInStep = 255 / (packet.life - data.fadeInEnd);
            data.growEnd = packet.life >> 1;
            // matching: the target recomputes life >> 1 here instead of reading growEnd
            data.growStep = size / (packet.life - (s16)(packet.life >> 1));
            data.tintStart = packet.life >> 1;
            data.tintStep = 255 / (packet.life >> 1);
            data.fadeOutStart = packet.life >> 2;
            data.fadeOutStep = 255 / (packet.life >> 2);
            data.swellStart = packet.life >> 1;
            data.swellRate = func_150ADA20() % (u32)(params->growRateRange + 1) + params->growRate;
            data.accelStart = packet.life >> 1;
            data.accel = func_150ADA20() % 81U + 80;
            packet.x = x;
            packet.y = y;
            packet.z = z;
            packet.subX = (s32)(x * 256.0f) & 0xFF;
            packet.subY = (s32)(y * 256.0f) & 0xFF;
            packet.subZ = (s32)(z * 256.0f) & 0xFF;
            particle = func_15167D84(&packet, 0, sizeof(data), -1, emitter->unkC, emitter->unk1);
            if (particle != NULL) {
                memcpy(particle + 1, &data, sizeof(data));
            }
            params->spawnAccum -= 1.0f;
        } while (params->spawnAccum > 1.0f);
    }
}

/* Callback 0x12 (func_151A1FB4's particles): fade in, grow, fade out, swell; switches to 0x13 to tint. */
void func_151A24A8(Particle *particle) {
    SmokeParticleData *data = (SmokeParticleData *)(particle + 1);

    if (particle->life > data->fadeInEnd) {
        particle->primA = (data->fadeInStart - particle->life) * data->fadeInStep;
    }
    if (particle->life > data->growEnd) {
        particle->width = particle->height = (data->growStart - particle->life) * data->growStep;
    }
    if (particle->life < data->fadeOutStart) {
        particle->primA = particle->life * data->fadeOutStep;
    }
    if (particle->life < data->swellStart) {
        s16 growth = data->swellRate * D_800BE9E4;

        particle->width += growth;
        particle->height += growth;
    }
    if (particle->life < data->tintStart) {
        particle->callback = 0x13;
        particle->flags |= BILLBOARD_ENV_COLOR | BILLBOARD_CUSTOM_MODE;
        particle->envR = particle->envG = particle->envB = particle->life * data->tintStep;
        particle->unk14 = 0x520003;
    }
    if (particle->life < data->accelStart) {
        particle->accelY = data->accel;
        data->accelStart = -9999;
    }
}

/* Callback 0x13: func_151A24A8 after the switch, with the env colour following life. */
void func_151A25E0(Particle *particle) {
    SmokeParticleData *data = (SmokeParticleData *)(particle + 1);

    if (particle->life > data->fadeInEnd) {
        particle->primA = (data->fadeInStart - particle->life) * data->fadeInStep;
    }
    if (particle->life > data->growEnd) {
        particle->width = particle->height = (data->growStart - particle->life) * data->growStep;
    }
    if (particle->life < data->fadeOutStart) {
        particle->primA = particle->life * data->fadeOutStep;
    }
    if (particle->life < data->swellStart) {
        s16 growth = data->swellRate * D_800BE9E4;

        particle->width += growth;
        particle->height += growth;
    }
    if (particle->life < data->accelStart) {
        particle->accelY = data->accel;
        data->accelStart = -9999;
    }
    particle->envR = particle->envG = particle->envB = particle->life * data->tintStep;
}

/* Fade-out and one-shot event of func_151A26EC's particles, run by func_151A2960. */
typedef struct {
    /* 0x0 */ s16 fadeOutStart;  /* desc.timer value */
    /* 0x2 */ s16 fadeOutStep;
    /* 0x4 */ s16 eventTime;     /* desc.timer value, -1 for none */
    /* 0x6 */ s8 eventFunc;      /* D_8008F8E0 index, -1 for none */
} FadeEvent;

/* The particle func_15130374 returns (layout as Particle in game_15D730.c). */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ ParticleDesc desc;
    /* 0x80 */ u8 unk80[0x28];
    /* 0xA8 */ f32 drag;         /* read by func_1513170C */
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ FadeEvent fade;
} DescParticle;

/* func_151A26EC's per-particle data, before it is copied into the particle. */
typedef struct {
    /* 0x0 */ f32 drag;
    /* 0x4 */ FadeEvent fade;
} ParticleData151A26EC;

s32 func_1513170C(DescParticle *, Vec3f *);
extern void (*D_8008F8E0[])(DescParticle *, s32);

extern DescParticle *func_15130374(ParticleDesc *, u8, s32, u8, s32);

/* Spawns one func_15130374 particle (texture 39, update func 0x11) at pos/offset with velocity vel. */
s32 func_151A26EC(Vec3f *pos, Vec3f *offset, Vec3f *vel, f32 drag, f32 gravity, f32 size,
                  s16 timer, u8 alpha, s16 fadeStart, s16 fadeOutStart, s16 eventTime, s8 eventFunc,
                  u8 r, u8 g, u8 b, u8 argF, s32 arg10) {
    ParticleData151A26EC data;
    DescParticle *particle;
    ParticleDesc desc;

    if (fadeStart <= 0) {
        fadeStart = 1;
    }
    if (fadeOutStart <= 0) {
        fadeOutStart = 1;
    }
    data.drag = drag;
    data.fade.fadeOutStart = fadeOutStart;
    data.fade.fadeOutStep = 255 / fadeOutStart;
    desc.fadeStart = fadeStart;
    desc.fadeRate = 255 / fadeStart;
    data.fade.eventTime = eventTime;
    data.fade.eventFunc = eventFunc;
    desc.texture = 39;
    desc.renderMode = 0x5203;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.timer = timer;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r = r;
    desc.g = g;
    desc.b = b;
    desc.a = 255;
    desc.r2 = 255;
    desc.g2 = 255;
    desc.b2 = 255;
    desc.alpha = alpha;
    desc.brightness = 255;
    desc.height = size;
    desc.width = size;
    desc.pos = *pos;
    desc.offset = *offset;
    desc.vel = *vel;
    desc.scaleStart = 1;
    desc.gravity = gravity;
    desc.scaleRate = 1.0f;
    // matching: this grouping of the constant flags sets IDO's or-chain order
    desc.flags = (func_150ADA20() & 1 ? 0x40 : 0) | (1 | 2 | 4 | (func_150ADA20() & 1 ? 0x80 : 0)) | 0x200 |
                 0x1000 | 0x4000 | 0x8000 | 0x800000;
    desc.primColorMode = 7;
    desc.envColorMode = 2;
    desc.updateFunc = 0x11;
    desc.deathFunc = -1;
    desc.drawFunc = -1;
    desc.handlerSet = 2;
    particle = func_15130374(&desc, 1, 0x10, argF, arg10);
    if (particle != NULL) {
        // matching: the target copies 8 bytes from data.drag, overlapping the second copy
        memcpy(&particle->drag, &data.drag, 8);
        memcpy(&particle->fade, &data.fade, sizeof(data.fade));
    }
    return 0;
}

/* Update func 0x11 (func_151A26EC's particles): dim out, run the one-shot event, then apply drag. */
s32 func_151A2960(DescParticle *particle, Vec3f *prevPos) {
    FadeEvent *fade;

    fade = &particle->fade;
    if (particle->desc.timer < fade->fadeOutStart) {
        particle->desc.brightness = particle->desc.timer * fade->fadeOutStep;
    }
    if (D_800BE616 == 0 && fade->eventTime != -1 && fade->eventFunc != -1 &&
        fade->eventTime >= particle->desc.timer) {
        D_8008F8E0[fade->eventFunc](particle, fade->eventTime);
        fade->eventTime = -1;
    }
    return func_1513170C(particle, prevPos);
}

void func_151A2C24(Vec3f *pos, f32 *dir, s16 yaw, s16 pitch, f32 speed, f32 sizeBase, f32 sizeSlope,
                   s32 count, f32 spread, f32 drag, f32 gravityBase, f32 gravityRange, s16 delayBase,
                   s16 delayRange, s16 alphaBase, s16 alphaRange, s16 fadeStart, s16 fadeOutStart,
                   s8 eventFunc, u8 arg13, s32 arg14, u8 useAngles);

/* Bursts `count` particles from pos moving at yaw/pitch with a speed that falls off over the burst. */
void func_151A2A14(Vec3f *pos, s16 yaw, s16 pitch, f32 speed, f32 sizeBase, f32 sizeSlope, s32 count,
                   f32 spread, f32 drag, f32 gravityBase, f32 gravityRange, s16 delayBase, s16 delayRange,
                   s16 alphaBase, s16 alphaRange, s16 fadeStart, s16 fadeOutStart, s8 eventFunc, u8 arg18,
                   s32 arg19) {
    func_151A2C24(pos, NULL, yaw, pitch, speed, sizeBase, sizeSlope, count, spread, drag, gravityBase,
                  gravityRange, delayBase, delayRange, alphaBase, alphaRange, fadeStart, fadeOutStart, eventFunc,
                  arg18, arg19, 1);
}

/* Bursts `count` particles from pos moving along dir, scaled down over the burst. */
void func_151A2AD4(Vec3f *pos, f32 *dir, f32 sizeBase, f32 sizeSlope, s32 count, f32 spread, f32 drag,
                   f32 gravityBase, f32 gravityRange, s16 delayBase, s16 delayRange, s16 alphaBase,
                   s16 alphaRange, s16 fadeStart, s16 fadeOutStart, s8 eventFunc, u8 arg16, s32 arg17) {
    func_151A2C24(pos, dir, 0, 0, 0.0f, sizeBase, sizeSlope, count, spread, drag, gravityBase, gravityRange,
                  delayBase, delayRange, alphaBase, alphaRange, fadeStart, fadeOutStart, eventFunc, arg16, arg17,
                  0);
}

/* Velocity of a burst particle at fraction t of the burst. */
typedef void (*BurstVelocityFunc)(f32 *dir, s32 yaw, s32 pitch, f32 speed, f32 t, f32 *out);

/* BurstVelocityFunc: dir scaled by 1 - t. */
void func_151A2B84(f32 *dir, s32 yaw, s32 pitch, f32 speed, f32 t, f32 *out) {
    f32 scale = 1.0f - t;

    out[0] = dir[0] * scale;
    out[1] = dir[1] * scale;
    out[2] = dir[2] * scale;
}

/* BurstVelocityFunc: the yaw/pitch direction at speed * (1 - t). */
void func_151A2BD0(f32 *dir, s32 yaw, s32 pitch, f32 speed, f32 t, f32 *out) {
    func_15143794((s16)yaw, (s16)pitch, (1.0f - t) * speed, out);
}

/* Particle colours picked at random by func_151A2C24 (a local initializer's rodata). */
typedef struct {
    u8 rgb[4][3];
} BurstColors;
extern BurstColors D_8008F8E8;

/* Spawns `count` func_151A26EC particles around pos; size and speed change linearly over the burst. */
void func_151A2C24(Vec3f *pos, f32 *dir, s16 yaw, s16 pitch, f32 speed, f32 sizeBase, f32 sizeSlope,
                   s32 count, f32 spread, f32 drag, f32 gravityBase, f32 gravityRange, s16 delayBase,
                   s16 delayRange, s16 alphaBase, s16 alphaRange, s16 fadeStart, s16 fadeOutStart,
                   s8 eventFunc, u8 arg13, s32 arg14, u8 useAngles) {
    s16 minFade;
    s16 delay;
    f32 t;
    f32 step;
    BurstVelocityFunc velocityFunc;
    f32 offset[3];
    f32 vel[3];
    s32 color;
    BurstColors colors;

    if (!useAngles) {
        velocityFunc = func_151A2B84;
    } else {
        velocityFunc = func_151A2BD0;
    }
    if (fadeStart < fadeOutStart) {
        minFade = fadeStart;
    } else {
        minFade = fadeOutStart;
    }
    t = 0.0f;
    step = 1.0f / (f32)count;
    while (count != 0) {
        f32 rand;
        s32 a;  // matching: a and b are reused for the alpha roll and the event time
        s32 b;

        delay = func_150ADA20() % (u32)(delayRange + 1) + delayBase;
        a = func_150ADA20();
        b = func_150ADA20();
        func_15143794((s16)(a & 0xFF), (s16)(b & 0xFF), func_150ADA68() * spread, offset);
        velocityFunc(dir, yaw, pitch, speed, t, vel);
        colors = D_8008F8E8;
        color = func_150ADA20() & 3;
        rand = func_150ADA68();
        a = func_150ADA20();
        if (func_150ADA68() < 0.0802000016f) {
            b = 25;
        } else {
            b = -1;
        }
        func_151A26EC(pos, (Vec3f *)offset, (Vec3f *)vel, drag, rand * gravityRange + gravityBase,
                      sizeSlope * t + sizeBase, delay + minFade, a % (u32)(alphaRange + 1) + alphaBase,
                      fadeStart, fadeOutStart, b, eventFunc, colors.rgb[color][0], colors.rgb[color][1],
                      colors.rgb[color][2], arg13, arg14);
        t += step;
        count--;
    }
}

/* Settings of a func_15152B38 burst (layout as struct Local15152B38Arg in game_17CAF0.c). */
typedef struct {
    /* 0x00 */ s32 countBase;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ Vec3f position;
    /* 0x14 */ f32 sizeBase;
    /* 0x18 */ f32 sizeRange;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 speedBase;
    /* 0x28 */ f32 speedRange;
    /* 0x2C */ s16 yawBase;
    /* 0x2E */ s16 yawRange;
    /* 0x30 */ s16 pitchBase;
    /* 0x32 */ s16 pitchRange;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s16 lifeBase;
    /* 0x3E */ s16 lifeRange;
    /* 0x40 */ u16 kind;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ Color8 colorBase;
    /* 0x49 */ Color8 colorRange;
    /* 0x4D */ Color8 color2Base;
    /* 0x51 */ Color8 color2Range;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ s8 unk70;
    /* 0x71 */ s8 unk71;
    /* 0x72 */ s8 unk72;
    /* 0x73 */ s8 unk73;
} BurstConfig;

void func_15152B38(BurstConfig *, u8, s32);

/* D_8008F8E0[0]: a func_15152B38 burst at the particle's position, sized by the particle. */
void func_151A2F0C(DescParticle *particle) {
    BurstConfig config;

    config.countBase = 8;
    config.countRange = 6;
    config.position = particle->desc.pos;
    if (particle->desc.flags & 0x1000) {
        config.position.x += particle->desc.offset.x;
        config.position.y += particle->desc.offset.y;
        config.position.z += particle->desc.offset.z;
    }
    config.sizeBase = (particle->desc.width + particle->desc.height) * 0.5f * 0.01999999955f;
    config.sizeRange = (particle->desc.width + particle->desc.height) * 0.5f * 0.03529999778f;
    config.unk1C = -0.2680000067f;
    config.unk20 = 0.0f;
    config.speedBase = 8.0f;
    config.speedRange = 10.0f;
    config.yawBase = 0;
    config.yawRange = 255;
    config.pitchBase = -64;
    config.pitchRange = 86;
    config.unk34 = 3;
    config.unk38 = 1;
    config.lifeBase = 17;
    config.lifeRange = 18;
    config.kind = 1;
    config.unk42 = 4;
    config.unk43 = 2;
    config.unk44 = 3;
    config.colorBase.red = 255;
    config.colorBase.green = 200;
    config.colorBase.blue = 200;
    config.colorBase.alpha = 255;
    config.colorRange.red = 0;
    config.colorRange.green = 55;
    config.colorRange.blue = 55;
    config.colorRange.alpha = 0;
    config.color2Base.red = 255;
    config.color2Base.green = 255;
    config.color2Base.blue = 255;
    config.color2Base.alpha = 255;
    config.color2Range.red = 0;
    config.color2Range.green = 0;
    config.color2Range.blue = 0;
    config.color2Range.alpha = 0;
    config.unk55 = 255;
    config.unk56 = 0;
    config.unk57 = 1;
    config.unk58 = 0x24;
    config.unk5C = 0x200005;
    config.unk60 = 0x60600;
    config.unk64 = 7;
    config.unk66 = 0x24;
    config.unk68 = 1;
    config.unk6A = 0;
    config.unk6C = 1.0f;
    config.unk70 = -1;
    config.unk71 = 0;
    config.unk72 = -1;
    config.unk73 = -1;

    func_15152B38(&config, particle->unkC, particle->unk1);
}
