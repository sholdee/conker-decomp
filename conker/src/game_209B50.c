#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Direction spread of a burst: func_15143794 takes a random angle from each range (see game_2062D0.c). */
typedef struct {
    s16 angle1;
    s16 angle1Range;
    s16 angle2;
    s16 angle2Range;
} BurstAngles;

/* Parameters for func_15150178, which throws count + rand(countRange) func_151D9014 particles
 * (same layout as DropBurstSpec in game_2062D0.c). */
typedef struct {
    /* 0x00 */ BurstAngles angles;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ s16 count;
    /* 0x16 */ s16 countRange;
    /* 0x18 */ f32 speed;
    /* 0x1C */ f32 speedRange;
    /* 0x20 */ s16 lifetime;
    /* 0x22 */ s16 lifetimeRange;
    /* 0x24 */ f32 gravity;
    /* 0x28 */ f32 gravityRange;
    /* 0x2C */ u8 unk2C; /* particle arg5 */
    /* 0x2D */ u8 unk2CRange;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ f32 size;
    /* 0x34 */ f32 sizeRange;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ u8 kind;
    /* 0x3A */ u8 pad3A[2];
    /* 0x3C */ f32 unk3CChance; /* probability of particle arg7 */
    /* 0x40 */ s8 wobble;
    /* 0x41 */ u8 pad41[3];
    /* 0x44 */ f32 bounceChance;
} DropBurstSpec; /* 0x48 */

void func_15103254(s16 lifetime, u8 alpha, f32 size, Vec3f *pos, u8 views, u8 arg5, s32 arg6);
void func_15150178(DropBurstSpec *spec, Vec3f *pos, s32 arg2, u8 owner, s32 kind);
void func_151D3F14(Vec3f *pos, u8 owner, s32 kind);
void func_15182670(u8 r, u8 g, u8 b, u8 alpha, s32 duration, s32 view, u8 owner, s32 kind);

/* Spawns a flash particle, a burst of 12-18 drops, the func_151D3F14 effect and a white screen
 * flash at `pos`. */
void func_151DC6A0(Vec3f *pos, u8 owner, s32 kind) {
    func_15103254(func_150ADA20() % 5U + 12, func_150ADA20() % 56U + 200, func_150ADA68() * 600.0f + 1700.0f,
                  pos, 0xFF, owner, kind);
    {
        DropBurstSpec drops;

        drops.pos = *pos;
        drops.count = 12;
        drops.countRange = 7;
        drops.angles.angle1 = 0;
        drops.angles.angle1Range = 0xFF;
        drops.angles.angle2 = -64;
        drops.angles.angle2Range = 60;
        drops.lifetime = 36;
        drops.lifetimeRange = 60;
        drops.unk2C = 200;
        drops.unk2CRange = 55;
        drops.unk38 = 0;
        drops.kind = 12;
        drops.wobble = 1;
        drops.speed = 8.0f;
        drops.speedRange = 10.0f;
        drops.gravity = -1.097f;
        drops.gravityRange = 0.37f;
        drops.size = 280.0f;
        drops.sizeRange = 390.0f;
        drops.unk3CChance = 0.6f;
        drops.bounceChance = 1.0f;
        func_15150178(&drops, &drops.pos, 0, owner, kind);
    }
    func_151D3F14(pos, owner, kind);
    func_15182670(0xFF, 0xFF, 0xFF, func_150ADA20() % 51U + 150, func_150ADA20() % 5U + 8, 0, owner, kind);
}


/* Data of the effect spawned by func_151DC8BC and updated by func_151DC97C. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 accum;        /* particles still to emit; func_151DC97C emits one per whole unit */
    /* 0x10 */ u8 randomColors;  /* nonzero: each particle takes one of three random colour pairs */
    /* 0x11 */ u8 pad11[3];
} EmitterData; /* 0x14 */

/* Effect created by func_15149130 (only the fields used here). */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;          /* passed to func_15130280 as its 6th argument */
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;          /* passed to func_15130280 as its 5th argument */
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ EmitterData data;
} Emitter;


/* Spawns an emitter effect at `pos` that lasts `duration` frames and is updated by func_151DC97C
 * (D_8008A4E8[0x5B]), preceded by a func_151DC6A0 burst when `burst` is set. */
void func_151DC8BC(Vec3f *pos, s32 duration, u8 burst, u8 randomColors, u8 owner, s32 kind) {
    EmitterData data;
    Emitter *emitter;

    if (burst) {
        func_151DC6A0(pos, owner, kind);
    }

    data.pos = *pos;
    data.accum = 0.0f;
    data.randomColors = randomColors;

    emitter = func_15149130(duration, -1, 0x5B, -1, 1, 0, sizeof(data), owner, kind);
    if (emitter != NULL) {
        memcpy(&emitter->data, &data, sizeof(data));
    }
}

/* ParticleDesc.flags (see game_15D730.c) */
#define PARTICLE_FLIP_S 0x40
#define PARTICLE_FLIP_T 0x80

/* Per-particle data of particles with update function 0x16 (ParticleDesc.updateFunc). */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
} ParticleUpdate16Data;

/* Particle created by func_15130280 (only the fields used here). */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ ParticleUpdate16Data data;
} Particle;

extern Particle *func_15130280(ParticleDesc *desc, u8, s32, s32 dataSize, u8, s32);

/* Update function of the func_151DC8BC emitter: emits particles at its position at a random rate
 * of (0.136 to 0.257) * D_800BE9A4 per frame, in a fixed colour or one of three random colour pairs. */
void func_151DC97C(Emitter *emitter) {
    EmitterData *data;

    data = &emitter->data;
    data->accum += (0.136f + func_150ADA68() * 0.12100001f) * D_800BE9A4;
    if (data->accum > 1.0f) {
        ParticleDesc desc;
        ParticleUpdate16Data extra;
        Particle *particle;
        u8 colorIndex;

        extra.unkC = 0.97047f;
        desc.texture = 0x6C;
        desc.renderMode = 0x5103;
        desc.unk0 = 0x200005;
        extra.unk0 = 0;
        extra.unk1 = 0;
        desc.unk4 = 0;
        desc.frame = 0;
        desc.frameRate = 0;
        desc.fadeStart = 40;
        desc.fadeRate = 6;
        desc.flags = 0x80DE07;
        desc.primColorMode = 8;
        desc.envColorMode = 6;
        desc.updateFunc = 0x16;
        desc.deathFunc = -1;
        desc.drawFunc = -1;
        desc.handlerSet = 0;
        desc.scaleStart = 120;
        desc.brightness = 0xFF;
        desc.scaleRate = 1.006579f;
        desc.pos = data->pos;
        desc.offset.x = 0.0f;
        desc.offset.y = 0.0f;
        desc.offset.z = 0.0f;
        desc.vel.x = 0.0f;
        desc.vel.y = 0.0f;
        desc.vel.z = 0.0f;
        if (data->randomColors == 0) {
            desc.r = 0xEF;
            desc.g = 0xE0;
            desc.b = 0xCD;
            desc.a = 0xFF;
            desc.r2 = 0x30;
            desc.g2 = 0x30;
            desc.b2 = 0x30;
        }
        do {
            extra.unk2 = func_150ADA20() % 5U + 4;
            extra.unk3 = func_150ADA20() % 5U + 4;
            extra.unk4 = func_150ADA68() * 50.0f;
            extra.unk8 = func_150ADA68() * 50.0f;
            desc.gravity = func_150ADA68() * 0.15100001f + 0.19100001f;
            desc.flags &= ~(PARTICLE_FLIP_S | PARTICLE_FLIP_T);
            desc.flags |= ((func_150ADA20() & 1) ? PARTICLE_FLIP_S : 0) | ((func_150ADA20() & 1) ? PARTICLE_FLIP_T : 0);
            desc.alpha = func_150ADA20() % 56U + 200;
            desc.timer = func_150ADA20() % 36U + 110;
            desc.width = desc.height = func_150ADA68() * 370.0f + 403.0f;
            if (data->randomColors != 0) {
                const u8 colors[3][3] = { { 0xEF, 0xE0, 0xCD }, { 0x9E, 0x9C, 0xA0 }, { 0x9E, 0x9C, 0xA0 } };
                const u8 colors2[3][3] = { { 0x30, 0x30, 0x30 }, { 0x9A, 0x69, 0xD2 }, { 0xD5, 0x83, 0x91 } };
                colorIndex = func_150ADA20() % 3U;
                desc.r = colors[colorIndex][0];
                desc.g = colors[colorIndex][1];
                desc.b = colors[colorIndex][2];
                desc.a = 0xFF;
                desc.r2 = colors2[colorIndex][0];
                desc.g2 = colors2[colorIndex][1];
                desc.b2 = colors2[colorIndex][2];
            }
            particle = func_15130280(&desc, 0, 0, sizeof(extra), emitter->unkC, emitter->unk1);
            if (particle != NULL) {
                memcpy(&particle->data, &extra, sizeof(extra));
            }
            data->accum -= 1.0f;
        } while (data->accum > 1.0f);
    }
}
