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
    /* 0x08 */ vertex offset;      /* emitter position in the bone's space */
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

void func_1504715C(FloorProbe *probe, struct127 *obj);

/* Spawns a dust emitter effect (func_15191D54) on `owner`'s bone; it emits dust particles at `offset`
 * in the bone's space at a randomised rate. Returns the effect, or NULL without an owner. */
struct260 *func_15191BE0(struct127 *owner, vertex *offset, u8 bone, f32 rateBase, f32 rateRandom,
                         s16 duration, f32 gravityBase, f32 gravityRandom, s16 lifetimeBase,
                         s16 lifetimeRandom, u8 alphaBase, u8 alphaRandom, f32 sizeBase, f32 sizeRandom,
                         f32 unk60, u8 kind, u8 arg16, s32 arg17) {
    struct260 *effect;
    DustEmitterData data;

    if (owner == NULL) {
        return NULL;
    }

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.offset = *offset;
    data.matrixOffset = bone << 6;
    data.rateBase = rateBase;
    data.accum = 0.0f;
    data.rateRandom = rateRandom;
    func_1504715C(&data.floor, owner);

    data.gravityBase = gravityBase;
    data.gravityRandom = gravityRandom;
    data.lifetimeBase = lifetimeBase;
    data.lifetimeRandom = lifetimeRandom;
    data.alphaBase = alphaBase;
    data.alphaRandom = alphaRandom;
    data.sizeBase = sizeBase;
    data.sizeRandom = sizeRandom;
    data.unk60 = unk60;
    data.kind = kind;

    // the header's struct37 * parameter is really the size of the effect's data
    effect = func_15149130((duration == -1) ? 300 : duration, -1, 0x1D, -1, (duration == -1) ? 0 : 1, 0x1D,
                           (struct37 *)sizeof(data), arg16, arg17);
    if (effect != NULL) {
        memcpy(&((DustEmitter *)effect)->data, &data, sizeof(data));
    }
    return effect;
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
DustParticle *func_1519203C(vertex *position, vertex *velocity, f32 gravity, s32 lifetime, u8 alpha,
                            u8 brightness, f32 size, s32 unused, f32 floorY, FloorData *floor, u8 kind,
                            u8 arg11, s32 arg12);
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
                func_1519203C((vertex *)spB4, (vertex *)spA4, ((f32)rand * local->unk4C) + local->unk48,
                              (s16)((r1 % (u32)(local->unk52 + 1)) + local->unk50),
                              (r2 % (u32)(local->unk55 + 1)) + local->unk54, 0xFF,
                              (func_150ADA68() * local->unk5C) + local->unk58, flag, scale,
                              (FloorData *)local->unk28, local->unk64, arg0->unkC, arg0->unk1);
                local->unk18 -= 1.0f;
            } while (local->unk18 > 1.0f);
        } else {
            while (local->unk18 > 1.0f) {
                local->unk18 -= 1.0f;
            }
        }
    }
}

DustParticle *func_15130374(ParticleDesc *desc, u8 kind, s32 dataSize, u8 arg3, s32 arg4);
extern u8 (*D_8008FCD0[])(void);

/* Spawns a dust particle at `position` that pulses in size and splashes when it falls below floorY
 * (update func 0xF, func_1519257C). */
DustParticle *func_1519203C(vertex *position, vertex *velocity, f32 gravity, s32 lifetime, u8 alpha,
                            u8 brightness, f32 size, s32 unused, f32 floorY, FloorData *floor, u8 kind,
                            u8 arg11, s32 arg12) {
    DustParticle *particle;
    ParticleDesc desc;
    DustData dust;
    s32 flipT;
    s32 flipS;

    desc.renderMode = 0x4401;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r = 0;
    desc.g = 0;
    desc.b = 0;
    desc.a = 0xFF;
    desc.r2 = 0;
    desc.g2 = 0;
    desc.b2 = 0;
    desc.alpha = alpha;
    desc.brightness = brightness;
    desc.pos = *position;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.vel = *velocity;
    desc.gravity = gravity;
    desc.scaleStart = 1;
    desc.scaleRate = 1.0f;
    if (func_150ADA20() & 1) {
        flipS = PARTICLE_FLIP_S;
    } else {
        flipS = 0;
    }
    if (func_150ADA20() & 1) {
        flipT = PARTICLE_FLIP_T;
    } else {
        flipT = 0;
    }
    desc.flags = flipT | (PARTICLE_LIFETIME | PARTICLE_GRAVITY | PARTICLE_MOVE | PARTICLE_FADE |
                          PARTICLE_HANDLERS | PARTICLE_DRAW_FUNC) | flipS | PARTICLE_LIT;
    desc.timer = (s16)lifetime + 18;
    desc.fadeStart = 18;
    desc.fadeRate = 14;
    desc.primColorMode = 6;
    desc.envColorMode = 8;
    desc.updateFunc = 0xF;
    desc.deathFunc = -1;
    desc.drawFunc = -1;
    desc.handlerSet = 3;
    dust.phase[0] = func_150ADA20() & 0xFF;
    dust.phase[1] = func_150ADA20() & 0xFF;
    desc.texture = D_8008FCD0[kind]();
    desc.height = size;
    desc.width = size;
    dust.baseSize = size;
    dust.phaseStep[0] = func_150ADA20() % 6U + 5;
    dust.phaseStep[1] = func_150ADA20() % 6U + 5;
    dust.amplitude[0] = (func_150ADA68() * 0.25f + 0.2f) * dust.baseSize;
    dust.amplitude[1] = (func_150ADA68() * 0.25f + 0.2f) * dust.baseSize;
    dust.floorY = floorY;
    dust.floor = *floor;
    dust.kind = kind;
    particle = func_15130374(&desc, 1, sizeof(DustData), arg11, arg12);
    if (particle != NULL) {
        memcpy(&particle->dust, &dust, sizeof(DustData));
    }
    return particle;
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
