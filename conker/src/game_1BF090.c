#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "libc/string.h"

/* ParticleDesc.flags (as in game_15D730.c) */
#define PARTICLE_LIFETIME  0x1
#define PARTICLE_GRAVITY   0x2
#define PARTICLE_MOVE      0x4
#define PARTICLE_FLIP_S    0x40
#define PARTICLE_FLIP_T    0x80
#define PARTICLE_FADE      0x200
#define PARTICLE_HANDLERS  0x4000
#define PARTICLE_DRAW_FUNC 0x8000
#define PARTICLE_LIT       0x10000

/* Spawn parameters of a func_15130374 particle (same layout as ParticleDesc in game_15D730.c). */
typedef struct ParticleDesc {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode;
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex offset;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ s32 flags;
    /* 0x5C */ void *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;
    /* 0x63 */ s8 deathFunc;
    /* 0x64 */ s8 drawFunc;
    /* 0x65 */ u8 handlerSet;
    /* 0x66 */ u8 pad66[0xA];
} ParticleDesc; /* size 0x70 */

/* Floor data copied out of a func_15046F84 probe result (from its +0x4). */
typedef struct FloorData {
    u8 bytes[16];
    u16 last;
} FloorData; /* 0x12 */

/* Floor probe result filled by func_15046F84 (cf. Struct150E0870Floor in game_10DD20.c). */
typedef struct FloorProbe {
    /* 0x00 */ f32 y;
    /* 0x04 */ FloorData data;
    /* 0x16 */ u8 pad16[0xE];
} FloorProbe; /* 0x24 */

/* Per-particle data of a dust particle (func_1519203C, updated by func_1519257C). */
typedef struct DustData {
    /* 0x00 */ f32 baseSize;
    /* 0x04 */ u8 phase[2];
    /* 0x06 */ s8 phaseStep[2];
    /* 0x08 */ f32 amplitude[2];
    /* 0x10 */ f32 floorY;
    /* 0x14 */ FloorData floor;
    /* 0x26 */ u8 kind;
} DustData; /* 0x28 */

/* A particle object (same layout as Particle in game_15D730.c). */
typedef struct DustParticle {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ ParticleDesc desc;
    /* 0x80 */ u8 pad80[0x28];
    /* 0xA8 */ DustData dust;
} DustParticle;

/* Common head of an effect object (as EffectHead in game_1228D0.c); data follows at +0x28. */
typedef struct EffectHead {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 state; /* set to -1 to free the effect */
    /* 0x10 */ u8 pad10[0x28 - 0x10];
} EffectHead;

/* Data of the dust emitter effect (func_15191BE0, run by func_15191D54). */
typedef struct DustEmitterData {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ struct17 offset;    /* emitter position in the bone's space */
    /* 0x14 */ s32 matrixOffset;   /* byte offset of the bone's 0x40-byte matrix in owner->unk1D4 */
    /* 0x18 */ f32 accum;          /* particles owed; one is spawned per whole unit */
    /* 0x1C */ f32 rateBase;
    /* 0x20 */ f32 rateRandom;
    /* 0x24 */ FloorProbe floor;
    /* 0x48 */ f32 gravityBase;
    /* 0x4C */ f32 gravityRandom;
    /* 0x50 */ s16 lifetimeBase;
    /* 0x52 */ s16 lifetimeRandom;
    /* 0x54 */ u8 alphaBase;
    /* 0x55 */ u8 alphaRandom;
    /* 0x56 */ u8 pad56[2];
    /* 0x58 */ f32 sizeBase;
    /* 0x5C */ f32 sizeRandom;
    /* 0x60 */ f32 unk60;          /* probability; > 0 also makes the particles probe the floor */
    /* 0x64 */ u8 kind;
    /* 0x65 */ u8 pad65[3];
} DustEmitterData; /* 0x68 */

typedef struct DustEmitter {
    EffectHead head;
    DustEmitterData data;
} DustEmitter;

struct Struct15191BE0Arg0 {
    u8 pad0[0x3B];
    u8 unk3B;
};

struct Struct15191BE0Local {
    struct Struct15191BE0Arg0 *unk0;
    u8 unk4;
    u8 pad5[3];
    Header2 unk8;
    s32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 unk24[9];
    f32 unk48;
    f32 unk4C;
    s16 unk50;
    s16 unk52;
    u8 unk54;
    u8 unk55;
    u8 pad56[2];
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    u8 unk64;
    u8 pad65[3];
};

struct Struct15191BE0Stack {
    s32 pad0;
    struct Struct15191BE0Local local;
};

void func_1504715C(s32, s32);

struct260 *func_15191BE0(struct Struct15191BE0Arg0 *arg0, Header2 *arg1, u8 arg2, f32 arg3, f32 arg4, s16 arg5, f32 arg6, f32 arg7, s16 arg8, s16 arg9, u8 argA, u8 argB, f32 argC, f32 argD, f32 argE, u8 argF, u8 arg10, s32 arg11) {
    struct260 *temp_v0;
    struct Struct15191BE0Stack sp3C;
    s32 temp_a0;
    u8 temp_arg4;

    if (arg0 == 0) {
        return 0;
    }

    sp3C.local.unk0 = arg0;
    sp3C.local.unk4 = arg0->unk3B;
    sp3C.local.unk8 = *arg1;
    sp3C.local.unk14 = arg2 << 6;
    sp3C.local.unk1C = arg3;
    sp3C.local.unk18 = 0.0f;
    sp3C.local.unk20 = arg4;
    func_1504715C((s32) sp3C.local.unk24, (s32) arg0);

    sp3C.local.unk48 = arg6;
    sp3C.local.unk4C = arg7;
    sp3C.local.unk50 = arg8;
    sp3C.local.unk52 = arg9;
    sp3C.local.unk54 = argA;
    sp3C.local.unk55 = argB;
    sp3C.local.unk58 = argC;
    sp3C.local.unk5C = argD;
    sp3C.local.unk60 = argE;
    sp3C.local.unk64 = argF;

    if (arg5 == -1) {
        temp_a0 = 0x12C;
    } else {
        temp_a0 = arg5;
    }

    if (arg5 == -1) {
        temp_arg4 = 0;
    } else {
        temp_arg4 = 1;
    }

    temp_v0 = func_15149130(temp_a0, -1, 0x1D, -1, temp_arg4, 0x1D, (struct37 *)0x68, arg10, arg11);
    if (temp_v0 != 0) {
        memcpy((s32) temp_v0 + 0x28, &sp3C.local, sizeof(sp3C.local));
    }
    return temp_v0;
}

struct Struct15191D54Owner {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x198];
    s32 unk1D4;
};

struct Struct15191D54Local {
    struct Struct15191D54Owner *unk0;
    u8 unk4;
    u8 pad5[3];
    Header2 unk8;
    s32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s32 unk28[8];
    f32 unk48;
    f32 unk4C;
    s16 unk50;
    s16 unk52;
    u8 unk54;
    u8 unk55;
    u8 pad56[2];
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    u8 unk64;
    u8 pad65[3];
};

struct Struct15191D54Arg0 {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 unkE;
    u8 pad10[0x18];
    struct Struct15191D54Local unk28;
};

void func_15143134(void *, f32 *, s32);
s32 func_1514672C(f32 *);
s32 func_15046F84(f32 *, s32, f32, f32 *);
void *func_1519203C(f32 *, f32 *, f32, s32, s32, s32, f32, s32, f32, s32 *, s32, s32, s32);
extern f32 D_800A80E8;
extern f32 D_800A80EC;
extern f32 D_800A80F0;

void func_15191D54(struct Struct15191D54Arg0 *arg0) {
    struct Struct15191D54Owner *owner;
    struct Struct15191D54Local *local;
    f32 spB4[3];
    f32 scale;
    f32 spA4[3];
    f32 rand;
    s32 flag;
    u32 r1;
    u32 r2;
    f32 dist;

    owner = arg0->unk28.unk0;
    local = &arg0->unk28;
    if ((owner->unk0 == 0) || (owner->unk3B != local->unk4)) {
        arg0->unkE = -1;
        return;
    }
    local->unk18 += (local->unk1C + (func_150ADA68() * local->unk20)) * D_800BE9A4;
    if (local->unk18 > 1.0f) {
        if (owner->unk1D4 != 0) {
            func_15143134(&local->unk8, spB4, owner->unk1D4 + local->unk14);
            if (local->unk60 > 0.0f) {
                if (func_1514672C(spB4) == 0) {
                    scale = D_800A80E8;
                } else {
                    dist = D_800A80EC;
                    if (func_15046F84(spB4, 0, dist, &local->unk24) != 0) {
                        scale = local->unk24;
                    } else {
                        scale = dist;
                    }
                }
            } else {
                scale = D_800A80F0;
            }
            do {
                flag = (func_150ADA68() < local->unk60) ? 1 : 0;
                spA4[0] = 0.0f;
                spA4[1] = 0.0f;
                spA4[2] = 0.0f;
                rand = func_150ADA68();
                r1 = func_150ADA20();
                r2 = func_150ADA20();
                func_1519203C(spB4, spA4, ((f32)rand * local->unk4C) + local->unk48,
                              (s16)((r1 % (u32)(local->unk52 + 1)) + local->unk50),
                              (r2 % (u32)(local->unk55 + 1)) + local->unk54, 0xFF,
                              (func_150ADA68() * local->unk5C) + local->unk58, flag, scale,
                              local->unk28, local->unk64, arg0->unkC, arg0->unk1);
                local->unk18 -= 1.0f;
            } while (local->unk18 > 1.0f);
        } else {
            while (local->unk18 > 1.0f) {
                local->unk18 -= 1.0f;
            }
        }
    }
}

typedef struct Vec1519203C {
    s32 x;
    s32 y;
    s32 z;
} Vec1519203C;

typedef struct Game1BF090Spawn {
    s32 header;
    s32 owner;
    s16 type, lifetime;
    s32 fieldC, field10;
    u8 color[4];
    u8 field18, field19, field1A, mode, group, kind;
    s16 field1E, field20, count;
    f32 scale, scaleX, scaleY;
    Vec1519203C position;
    f32 rotation[3];
    Vec1519203C direction;
    f32 speed;
    s32 flags;
    u8 pad5C[4];
    u8 channel0, channel1, channel2;
    s8 channel3, channel4;
    u8 channel5;
    u8 pad66[0xA];
} Game1BF090Spawn;

typedef struct Game1BF090Packed {
    u8 bytes[16];
    u16 last;
} Game1BF090Packed;

typedef struct Game1BF090Fragment {
    f32 scale;
    u8 random[4];
    f32 scaleX, scaleY, height;
    Game1BF090Packed body;
    u8 kind;
} Game1BF090Fragment;

void *func_15130374(s32, u8, s32, u8, s32);
extern s32 (*D_8008FCD0[])(void);
extern f32 D_800A80F4, D_800A80F8;

void *func_1519203C(f32 *position, f32 *direction,
                    f32 speed, s32 lifetime, s32 mode, s32 group,
                    f32 scale, s32 unused, f32 height, s32 *body,
                    s32 kind, s32 priority, s32 owner) {
    void *effect;
    Game1BF090Spawn spawn;
    Game1BF090Fragment fragment;
    s32 second;
    s32 first;

    spawn.type = 0x4401;
    spawn.header = 0x200005;
    spawn.owner = 0;
    spawn.fieldC = 0;
    spawn.field10 = 0;
    spawn.color[0] = 0;
    spawn.color[1] = 0;
    spawn.color[2] = 0;
    spawn.color[3] = 0xFF;
    spawn.field18 = 0;
    spawn.field19 = 0;
    spawn.field1A = 0;
    spawn.mode = ((u8 *)&mode)[3];
    spawn.group = ((u8 *)&group)[3];
    spawn.position = *(Vec1519203C *)position;
    spawn.rotation[0] = 0.0f;
    spawn.rotation[1] = 0.0f;
    spawn.rotation[2] = 0.0f;
    spawn.direction = *(Vec1519203C *)direction;
    spawn.speed = speed;
    spawn.count = 1;
    spawn.scale = 1.0f;
    if (func_150ADA20() & 1) {
        first = 0x40;
    } else {
        first = 0;
    }
    if (func_150ADA20() & 1) {
        second = 0x80;
    } else {
        second = 0;
    }
    spawn.flags = second | 0xC207 | first | 0x10000;
    spawn.lifetime = (s16)lifetime + 0x12;
    spawn.field1E = 0x12;
    spawn.field20 = 0xE;
    spawn.channel0 = 6;
    spawn.channel1 = 8;
    spawn.channel2 = 0xF;
    spawn.channel3 = -1;
    spawn.channel4 = -1;
    spawn.channel5 = 3;
    fragment.random[0] = func_150ADA20() & 0xFF;
    fragment.random[1] = func_150ADA20() & 0xFF;
    spawn.kind = D_8008FCD0[((u8 *)&kind)[3]]();
    spawn.scaleY = scale;
    spawn.scaleX = scale;
    fragment.scale = scale;
    fragment.random[2] = func_150ADA20() % 6U + 5;
    fragment.random[3] = func_150ADA20() % 6U + 5;
    fragment.scaleX = (func_150ADA68() * 0.25f + D_800A80F4) * fragment.scale;
    fragment.scaleY = (func_150ADA68() * 0.25f + D_800A80F8) * fragment.scale;
    fragment.height = height;
    fragment.body = *(Game1BF090Packed *)body;
    fragment.kind = ((u8 *)&kind)[3];
    effect = func_15130374((s32)&spawn, 1, 0x28, ((u8 *)&priority)[3], owner);
    if (effect != 0) {
        memcpy((u8 *)effect + 0xA8, &fragment, 0x28U);
    }
    return effect;
}

void func_15131C84(u8 *phase, s8 *phaseStep, f32 base, f32 *amplitude, f32 *outX, f32 *outY);

/* Particle update: pulses the dust particle's width and height around its base size. */
u8 func_15192308(DustParticle *particle, struct17 *prevPos) {
    func_15131C84(particle->dust.phase, particle->dust.phaseStep, particle->dust.baseSize,
                  particle->dust.amplitude, &particle->desc.width, &particle->desc.height);
    return TRUE;
}

void func_151436B4(f32, f32, f32, f32 *);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32,
                  s32, s32, s32, s32, s32, s32, s32, s32);

/* Particle update: once the dust particle falls below its floor, splash there and stop. */
u8 func_15192358(DustParticle *particle, struct17 *prevPos) {
    u8 alive;
    DustData *dust;
    vertex position;
    vertex velocity;
    f32 sizeRandom;
    f32 speedRandom;
    f32 angleRandom;
    u32 countRandom;
    u32 durationRandom;

    alive = TRUE;
    dust = &particle->dust;
    if (particle->desc.pos.y < dust->floorY) {
        alive = FALSE;
        position.x = particle->desc.pos.x;
        position.y = dust->floorY + 2.0f;
        position.z = particle->desc.pos.z;
        func_151D9B8C(dust->kind, dust->baseSize * 0.08308f, particle->desc.alpha, (s32)&dust->floor,
                      (struct17 *)&position, 100, 0, 1, 0, particle->unkC, particle->unk1);
        angleRandom = func_150ADA68();
        speedRandom = func_150ADA68();
        sizeRandom = func_150ADA68();
        func_151436B4(angleRandom * 6.2831855f,
                      (speedRandom * 1000.0f + -1207.0f) * 0.001f,
                      (sizeRandom * 3557.0f + 5336.0f) * 0.001f,
                      &velocity.x);
        sizeRandom = func_150ADA68();
        durationRandom = func_150ADA20();
        countRandom = func_150ADA20();
        func_151DA6F8(&position.x, &velocity.x, sizeRandom * 0.20600002f + 0.20600002f,
                      durationRandom % 21U + 35, particle->desc.alpha,
                      dust->baseSize * 0.02495f, countRandom % 7U + 5, 0,
                      1.0f, 1.0f, 1, dust->kind, 0, 0x10, 0xF, 0,
                      particle->unkC, particle->unk1);
    }
    return alive;
}

/* Dust particle update func (D_80089760): pulse, then land on the floor. */
u8 func_1519257C(DustParticle *particle, struct17 *prevPos) {
    u8 alive;

    alive = func_15192308(particle, prevPos);
    if (alive) {
        alive = func_15192358(particle, prevPos);
    }
    return alive;
}

extern Header2 D_800A8030[];
extern u8 D_800A8060[];
extern f32 D_800A8064[];
extern f32 D_800A8074[];
extern f32 D_800A8084[];
extern f32 D_800A8094[];
extern s16 D_800A80A4[];
extern s16 D_800A80AC[];
extern f32 D_800A80B4[];
extern f32 D_800A80C4[];
extern f32 D_800A80D4[];
extern u8 D_800A80E4[];

s32 func_151925C4(u8 *arg0, s16 arg1, u8 arg2, s32 arg3) {
    s32 kind;
    s32 result;
    u8 mode;

    if (arg0 == 0) {
        return 0;
    }
    kind = -1;
    result = 0;
    switch (arg0[4]) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        kind = 0;
        break;
    case 0x10:
        kind = 1;
        break;
    case 0x58:
        kind = 2;
        break;
    case 0x79:
        kind = 3;
        break;
    }
    if (kind != -1) {
        result = (s32)func_15191BE0((struct Struct15191BE0Arg0 *)arg0, &D_800A8030[kind],
                               D_800A8060[kind], D_800A8064[kind],
                               D_800A8074[kind], arg1,
                               D_800A8084[kind], D_800A8094[kind],
                               D_800A80A4[kind], D_800A80AC[kind],
                               0x64, 0x9B, D_800A80B4[kind],
                               D_800A80C4[kind], D_800A80D4[kind],
                               D_800A80E4[kind], arg2, arg3);
    }
    return result;
}

/* Effect callback for the dust emitter: hands func_15169850 the owner reference. */
void func_1519277C(DustEmitter *effect, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, (s32)&effect->data.owner, (s32)&effect->data.ownerId, (s32)effect);
}
