#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 *D_800DBF94;
extern struct131 *func_151149AC(u8);
extern s32 func_1505D024(struct127 *, s32, u16, s32);
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern void func_150E1AB0(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, u16, s32, s32, s16, s16, s32, u8, s16, s32, s32, u8, f32, f32, f32, f32, f32, f32);
extern void func_150E2DB4(struct127 *, u8, s16, s32, f32, f32, f32, f32, f32, f32, s16, s16, u16, u8);
extern void func_15179008(s32);

/* If object idA (else idB) has bit 0 of its D_800DBF94 flag word set, calls func_1505D024 on
 * D_800CC2D0[0] with 0x6E00 (else 0xEE00). Does nothing while D_800CC3D4[0] is set. */
void func_150AF200(s32 idA, s32 idB) {
    struct131 *obj;
    s32 index;

    if (D_800CC3D4[0] != 0) {
        return;
    }

    obj = func_151149AC(idA);
    index = obj - D_800DBEF4;
    if (D_800DBF94[index] & 1) {
        func_1505D024(&D_800CC2D0[0], 0x3F, 0x6E00, -1);
        return;
    }

    obj = func_151149AC(idB);
    index = obj - D_800DBEF4;
    if (D_800DBF94[index] & 1) {
        func_1505D024(&D_800CC2D0[0], 0x3F, 0xEE00, -1);
    }
}

extern void func_151CF898(void *, f32, f32);

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 pad4[2];
    s16 unk8;
} Range150AF2E0;

/* Calls func_151CF898(arg0, unk2 + unk8, unk2). */
void func_150AF2E0(void *arg0, Range150AF2E0 *range) {
    func_151CF898(arg0, range->unk2 + range->unk8, range->unk2);
}

/* Particle spawn template passed to func_15130280. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 scaleX;
    /* 0x2C */ f32 scaleY;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex unk3C;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 flags; /* 0x40: flip X, 0x80: flip Y */
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 pad67[9];
} ParticleSpawn; /* size 0x70 */

/* Per-particle data of the wobbling particles func_150AF328 spawns (updated by func_150AF6E4). */
typedef struct {
    /* 0x00 */ u8 phase[2];     /* x and z angles of the wobble ellipse */
    /* 0x02 */ u8 phaseStep[2]; /* added to phase per tick */
    /* 0x04 */ f32 amplitude[2];
    /* 0x0C */ f32 drag;        /* velocity scale per tick */
} WobbleData;

/* A particle created by func_15130280. */
typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ vertex vel;
    /* 0x64 */ u8 pad64[0xA8 - 0x64];
    /* 0xA8 */ WobbleData wobble;
} Particle;

/* Per-effect data of the func_150AF328 emitter. */
typedef struct {
    /* 0x00 */ vertex pos;
    /* 0x0C */ f32 spawnAccum;
} EmitterData;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ EmitterData data;
} Emitter;

extern Particle *func_15130280(ParticleSpawn *, u8, s32, s32, u8, s32);
extern void func_15131828(Particle *, f32 *amplitude, u8 *phase, u8 *phaseStep);
extern void func_15131958(vertex *vel, f32 drag);

/* Emitter update: accumulates 0.1352..0.305 particles per unit of D_800BE9A4 and spawns one
 * randomly sized particle at the emitter's position for each whole one. */
void func_150AF328(Emitter *emitter) {
    EmitterData *data;

    data = &emitter->data;
    data->spawnAccum += (1352.0f + func_150ADA68() * 1698.0f) * 0.0001f * D_800BE9A4;
    if (data->spawnAccum > 1.0f) {
        ParticleSpawn spawn;
        WobbleData wobble;
        Particle *particle;

        wobble.drag = 0.9704700112f;
        spawn.unk1D = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.unk1E = 70;
        spawn.unk20 = 3;
        spawn.flags = 0x80DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        spawn.unk62 = 0x16;
        wobble.phase[0] = 0;
        wobble.phase[1] = 0;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 70;
        spawn.unk14 = 0xC7;
        spawn.unk15 = 0x78;
        spawn.unk16 = 8;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0x30;
        spawn.unk19 = 0xE;
        spawn.unk1A = 0;
        spawn.unk1C = 0xFF;
        spawn.unk24 = 1.021028996f;
        spawn.pos = data->pos;
        spawn.unk3C.x = 0.0f;
        spawn.unk3C.y = 0.0f;
        spawn.unk3C.z = 0.0f;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        do {
            wobble.phaseStep[0] = func_150ADA20() % 5U + 4;
            wobble.phaseStep[1] = func_150ADA20() % 5U + 4;
            wobble.amplitude[0] = func_150ADA68() * 11.0f;
            wobble.amplitude[1] = func_150ADA68() * 11.0f;
            spawn.unk54 = func_150ADA68() * 0.172f + 0.09800000489f;
            spawn.flags &= ~0xC0;
            spawn.flags |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.unk1B = func_150ADA20() % 101U + 100;
            spawn.unkA = func_150ADA20() % 51U + 80;
            spawn.scaleX = spawn.scaleY = func_150ADA68() * 71.0f + 80.0f;
            particle = func_15130280(&spawn, 0, 0, 0x10, emitter->unkC, emitter->unk1);
            if (particle != NULL) {
                memcpy(&particle->wobble, &wobble, sizeof(wobble));
            }
            data->spawnAccum -= 1.0f;
        } while (data->spawnAccum > 1.0f);
    }
}

/* Update for func_150AF328's particles: wobbles the offset and applies drag to the velocity. */
s32 func_150AF6E4(Particle *particle, s32 arg1) {
    WobbleData *wobble;

    wobble = &particle->wobble;
    func_15131828(particle, wobble->amplitude, wobble->phase, wobble->phaseStep);
    func_15131958(&particle->vel, wobble->drag);
    return 1;
}

/* Effect descriptor that func_1515FF74 copies into the new effect node at 0xE. */
typedef struct {
    /* 0x0 */ u8 flags;        /* 1: the effect ends when lifetime runs out */
    /* 0x1 */ s8 updateIndex;  /* D_8008B0D0 handler, -1 for none */
    /* 0x2 */ s8 drawIndex;    /* D_8008B0D8 handler */
    /* 0x3 */ u8 pad3;
    /* 0x4 */ s16 lifetime;
    /* 0x6 */ u8 messageIndex; /* D_8008B0E4 handler */
} EffectDesc;

extern void func_1515FF74(EffectDesc *desc, s32 dataSize, u8, s32);

/* Starts a data-less effect (draw handler 2, no update) that ends after `lifetime` frames. */
void func_150AF738(s16 lifetime, u8 arg1, s32 arg2) {
    EffectDesc desc;

    desc.flags = 1;
    desc.updateIndex = -1;
    desc.drawIndex = 2;
    desc.lifetime = lifetime;
    desc.messageIndex = 0;
    func_1515FF74(&desc, 0, arg1, arg2);
}

extern void func_150B1DB0(void *, void *);

void func_150AF790(void *arg0, u8 *arg1) {
    func_150B1DB0(arg1, arg1 + 0x1ECC0);
}

typedef struct GameDC6B0SmokeTimers {
    s16 puff;
    s16 spark;
} GameDC6B0SmokeTimers;

typedef struct GameDC6B0SmokeSource {
    u8 pad0;
    u8 owner;
    u8 pad2[0xA];
    u8 group;
    u8 padD;
    s16 maxLifetime;
    u8 pad10[0x18];
    GameDC6B0SmokeTimers timers;
} GameDC6B0SmokeSource;

typedef struct GameDC6B0SmokePacket {
    f32 x, y;
    f32 scaleX, scaleY;
    u8 selector;
    u8 pad11;
    s16 lifetime, flags, field16, field18;
    u8 color0, color1, color2, color3, color4;
    u8 color5, color6, color7, color8, color9;
    s32 field24, field28, field2C, field30, field34, field38, field3C;
    u8 field40, field41;
    u8 pad42[2];
    u8 field44;
    u8 pad45[0x13];
} GameDC6B0SmokePacket;

typedef struct GameDC6B0SmokeMotion {
    f32 x, y;
    f32 phase; /* random in [0, 2pi) */
    f32 unkC;
} GameDC6B0SmokeMotion;

/* A sprite created by func_1515548C; the puffs carry their motion at 0x70. */
typedef struct {
    u8 pad0[0x70];
    GameDC6B0SmokeMotion motion;
} GameDC6B0SmokeSprite;

u8 func_151D8E6C(void);
GameDC6B0SmokeSprite *func_1515548C(GameDC6B0SmokePacket *, u8, s32, s32, s32, u8, s32);

/* Counts down the source's two timers and, when each expires, spawns a moving puff sprite or a
 * static spark sprite (lifetime capped at the source's maxLifetime) and restarts it randomly. */
void func_150AF7C4(GameDC6B0SmokeSource *source) {
    GameDC6B0SmokeSprite *sprite;
    struct {
        GameDC6B0SmokeMotion motion;
        GameDC6B0SmokePacket packet;
    } puff;
    GameDC6B0SmokeTimers *timers;
    GameDC6B0SmokePacket spark;

    timers = &source->timers;
    timers->puff -= D_800BE9E4;
    if (timers->puff < 0) {
        puff.motion.x = func_150ADA68() * 280.0f + -140.0f;
        puff.motion.y = func_150ADA68() * 50.0f;
        puff.motion.phase = 2.0f * (func_150ADA68() * 3.141592741f);
        puff.motion.unkC = func_150ADA68() * 0.2680000067f;
        puff.packet.x = 0.0f;
        puff.packet.y = 0.0f;
        func_150ADA68();
        puff.packet.selector = 0xFF;
        puff.packet.scaleX = 1.0f;
        puff.packet.scaleY = 115.0f;
        puff.packet.lifetime = func_150ADA20() % 71U + 20;
        if (source->maxLifetime < puff.packet.lifetime) {
            puff.packet.lifetime = source->maxLifetime;
        }
        puff.packet.flags = 0x21;
        puff.packet.field16 = 1;
        puff.packet.field18 = 0xFF;
        puff.packet.color0 = 0;
        puff.packet.color1 = 0xFF;
        puff.packet.color2 = 0xFF;
        puff.packet.color3 = 0xFF;
        puff.packet.color4 = func_150ADA20() % 71U + 40;
        puff.packet.color5 = 0xFF;
        puff.packet.color6 = 0xFF;
        puff.packet.color7 = 0xFF;
        puff.packet.color8 = 0xFF;
        puff.packet.color9 = 0xFF;
        puff.packet.field24 = 0;
        puff.packet.field28 = 0x200004;
        puff.packet.field2C = 0x1F0601;
        puff.packet.field30 = 0x19;
        puff.packet.field34 = 0x54;
        puff.packet.field38 = 0x80;
        puff.packet.field3C = 0x20;
        puff.packet.field40 = 0;
        puff.packet.field41 = 0xA;
        puff.packet.field44 = 0;
        sprite = func_1515548C(&puff.packet, 6, 0, 0, 0x10, source->group, source->owner);
        if (sprite != NULL) {
            memcpy(&sprite->motion, &puff.motion, sizeof(puff.motion));
        }
        timers->puff = func_150ADA20() % 41U + 45;
    }
    timers->spark -= D_800BE9E4;
    if (timers->spark < 0) {
        spark.x = func_150ADA68() * 280.0f + -140.0f;
        spark.y = func_150ADA68() * 200.0f + -100.0f;
        spark.scaleX = spark.scaleY = 2.0f * func_150ADA68() + 1.0f;
        spark.selector = func_151D8E6C();
        spark.lifetime = func_150ADA20() % 7U + 5;
        if (source->maxLifetime < spark.lifetime) {
            spark.lifetime = source->maxLifetime;
        }
        spark.flags = 1 | (func_150ADA20() ? 2 : 0) | (func_150ADA20() ? 4 : 0) | 0x20;
        spark.field16 = 1;
        spark.field18 = 0xFF;
        spark.color0 = 0;
        spark.color1 = 0xFF;
        spark.color2 = 0xFF;
        spark.color3 = 0xFF;
        spark.color4 = func_150ADA20() % 121U + 80;
        spark.color5 = 0xFF;
        spark.color6 = 0xFF;
        spark.color7 = 0xFF;
        spark.color8 = 0xFF;
        spark.color9 = 0xFF;
        spark.field24 = 0;
        spark.field28 = 0x200004;
        spark.field2C = 0x1F0601;
        spark.field30 = 0x19;
        spark.field34 = 0x55;
        spark.field38 = 0x80;
        spark.field3C = 0x20;
        spark.field40 = 0;
        spark.field41 = 7;
        spark.field44 = 0;
        func_1515548C(&spark, 0, 0, 0, 0, source->group, source->owner);
        timers->spark = func_150ADA20() % 11U + 3;
    }
}

extern f32 sinf(f32);

struct Sub150AFBF4 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
};

struct Obj150AFBF4 {
    char pad0[0x10];
    f32 unk10;
    char pad14[0x5C];
    struct Sub150AFBF4 unk70;
};

s32 func_150AFBF4(struct Obj150AFBF4 *arg0) {
    struct Sub150AFBF4 *p = &arg0->unk70;
    p->unk8 = p->unk8 + p->unkC * D_800BE9A4;
    p->unk8 = func_15144B68(p->unk8);
    arg0->unk10 = sinf(p->unk8) * p->unk4 + p->unk0;
    return 1;
}

/* Defined (K&R) in game_19A8B0.c; this is the int-argument prototype callers use. */
extern void func_1516D99C(s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
    s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
    s32, s32, s32, s32, s32, u8, s32, s32, s32, s32, s32, u8, s32);

/* Spawns a func_1516D99C particle emitter (texture 0xD, kind 4) on `object`'s `bone`. */
void func_150AFC68(s32 object, s32 bone, s32 timer, s32 radiusXZ, u8 arg4, s32 arg5) {
    func_1516D99C(1, 0, 0, 0xD,                 /* x, y, z, texture */
        0, 0, 0, 0, 0, 0, 0, 0,                 /* frame, rgba, env rgb */
        4, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0,        /* kind, velocity x/z high/low, unk32..34 */
        0xFF, 0, 1, 0, 0, 0, 0, 0xAA,           /* unk35..37, velocity randoms, scaleX */
        0xAA, 0xAA, 0xAA, timer, radiusXZ, 0,   /* scaleY, scale randoms, timer, radii */
        object, 0xF0, 0x50, 0x50, 1, 4, 0, 1,   /* object, lifetime, velocityY (+random), flags, interval, chance, burst */
        0, 0, 0, bone, 0, arg4, arg5);          /* speed, startOffset, isAlt, bone, particleLink */
}

extern void *D_800DCE94;

struct Node150AFDB0 {
    char pad0[0x8];
    struct Node150AFDB0 *unk8;
    char pad0C[0x33];
    u8 unk3F;
};

void func_150AFDB0(void) {
    struct Node150AFDB0 *node;

    D_800DD190++;
    node = D_800DCE94;
    while (node != 0) {
        ((struct Node150AFDB0 **)D_800DD198)[(s8)D_800DD190] = node->unk8;
        if (node->unk3F == D_800C3E78) {
            func_1516972C((struct102 *)node);
        }
        node = ((struct Node150AFDB0 **)D_800DD198)[(s8)D_800DD190];
    }
    D_800DD190--;
}

void func_150AFE64(s32 arg0) {
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    s32 idx;
    f32 *mtx;
    f32 zero;

    if (D_800D154C->unk1D4 != 0) {
        if (arg0 == 0) {
            idx = 3;
        } else {
            idx = 2;
        }

        zero = 0.0f;
        mtx = (f32 *)((u8 *)D_800D154C->unk1D4 + (idx << 6));
        spA4 = zero;
        spA0 = zero;
        sp9C = -20.0f;
        func_150A7960(mtx, spA4, spA0, sp9C, &spA4, &spA0, &sp9C);
        sp98 = zero;
        sp94 = zero;
        sp90 = -150.0f;
        func_150A7960(mtx, sp98, sp94, sp90, &sp98, &sp94, &sp90);
        func_150E1AB0(0, spA4, spA0, sp9C, sp98, sp94, sp90, 40.0f, zero, 2.0f, 120.0f, 0x3C, 0x23, 0, 0, 0, 0, 0, 0, 0, 0, 0, zero, zero, zero, zero, zero, zero);
        func_150E2DB4(D_800D154C, D_800D154C->unique_id, (s16)idx, -1, zero, zero, -39.0f, zero, zero, -150.0f, 3, 0xFF, 4, 0);
    }
}

void func_150B003C(s32 arg0) {
    if (D_800BE9F0 == 6) {
        func_15179008(0);
        func_150AF200(0xE2, 0xE1);
        return;
    }
    func_150AF200(0xDF, 0xDE);
}

typedef struct GameDC6B0Packet {
    f32 field0, field4, field8, fieldC;
    f32 angle0, angle1, angle2;
    struct17 scale;
    struct17 position;
    struct17 velocity;
    struct17 field40;
    f32 field4C;
    s32 field50;
    s16 field54, field56;
    u8 field58;
    u8 pad59[3];
    s32 field5C;
    u8 field60, field61, field62, field63;
    u8 field64, field65, field66, field67;
    u8 field68, pad69, field6A, pad6B;
    s32 field6C;
    u8 field70, pad71;
    s16 field72, field74;
    u8 pad76[2];
    s32 field78;
} GameDC6B0Packet;

typedef struct GameDC6B0Link {
    void *object;
    s32 zero;
} GameDC6B0Link;

void *func_1513264C(void *, s32, s32, s32, s32, u8, s32);
void func_15145974(void *, f32 *, f32 *);
void *func_151B7328(void *, s32, s32, s32, s32);
s32 func_15145128(f32 *, f32 *, f32 *, f32 *);


void func_150B0094(struct17 *arg0, struct17 *arg1, u8 arg2, s32 arg3) {
    GameDC6B0Packet packet;
    struct17 delta;
    f32 length;
    f32 inverse_length;
    u8 *created;
    GameDC6B0Link link;
    void **slot;

    link.zero = 0;
    delta.unk0 = arg1->unk0 - arg0->unk0;
    delta.unk4 = arg1->unk4 - arg0->unk4;
    delta.unk8 = arg1->unk8 - arg0->unk8;
    if (func_15145128(&delta.unk0, &delta.unk0, &length, &inverse_length) != 0) {
        packet.field4 = packet.field0 = 1.0f;
        packet.field8 = packet.fieldC = 0.7f;
        packet.position = *arg0;
        packet.velocity.unk0 = delta.unk0 * 80.0f;
        packet.velocity.unk4 = delta.unk4 * 80.0f;
        packet.velocity.unk8 = delta.unk8 * 80.0f;
        func_15145974(&packet.velocity, &packet.angle1, &packet.angle0);
        packet.scale.unk8 = packet.scale.unk4 = packet.scale.unk0 = 1.0f;
        packet.angle2 = 0.0f;
        packet.field40 = *(struct17 *)&D_800A5480;
        packet.field50 = 0x19A0;
        packet.field54 = 0x12C;
        packet.field56 = 0xD2;
        packet.field58 = 7;
        packet.field5C = 0;
        packet.field60 = 0xFF;
        packet.field61 = 8;
        packet.field62 = 0;
        packet.field63 = 0;
        packet.field64 = 0;
        packet.field65 = 0;
        packet.field66 = 0;
        packet.field67 = 0;
        packet.field68 = 2;
        packet.field6A = 0;
        packet.field6C = 0;
        packet.field70 = 0;
        packet.field72 = 1;
        packet.field74 = 0xFF;
        packet.field78 = 0;
        packet.field4C = 0.0f;
        created = func_1513264C(&packet, 3, 0xFF, 0, 4, arg2, arg3);
        if (created != 0) {
            slot = (void **)(created + 0x170);
            memcpy(slot, &link.zero, 4);
            link.object = created;
            *slot = func_151B7328(&link, 1, 8, arg2, arg3);
        }
    }
}

void func_150B02C0(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0x170) != 0) {
        func_1516972C(*(struct102 **)((u8 *)arg0 + 0x170));
    }
}

extern void func_150B02C0(void *);
extern void func_15132570(void *);

void func_150B02F0(void *arg0) {
    func_150B02C0(arg0);
    func_15132570(arg0);
}

extern void func_1513259C(void *);

void func_150B031C(void *arg0) {
    func_150B02C0(arg0);
    func_1513259C(arg0);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    struct17 unk38;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    s8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 unk68;
} Arg15153634;

typedef struct {
    s32 unk00;
    s32 unk04;
    struct17 unk08;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 unk30;
    f32 unk34;
    f32 unk38;
} Arg15152190;

extern void func_15153634(Arg15153634 *, s32, u8, s32);
extern void func_15152190(Arg15152190 *, const s32 *, const f32 *, s32, f32, s32, u8, s32);

extern const s32 D_8009F790[5];
extern const f32 D_8009F7A4[5];

void func_150B0348(u8 *arg0, u8 arg1, s32 arg2) {
    struct17 pos;

    pos.unk0 = *(f32 *)(arg0 + 0x14);
    pos.unk4 = *(f32 *)(arg0 + 0x18) + 30.0f;
    pos.unk8 = *(f32 *)(arg0 + 0x1C);
    {
    Arg15153634 a;

    a.unk0 = 0xA;
    a.unk2 = 7;
    a.unk4 = 0x6C;
    a.unk6 = 0x5103;
    a.unk8 = 0x200005;
    a.unkC = 0;
    a.unk10 = 0x1E;
    a.unk12 = 0xF;
    a.unk14 = 0;
    a.unk18 = 0;
    a.unk1F = 0xFF;
    a.unk1C = 0x7F;
    a.unk1D = 0x4B;
    a.unk1E = 6;
    a.unk20 = 0x80;
    a.unk21 = 0x53;
    a.unk22 = 0;
    a.unk23 = 0x64;
    a.unk24 = 0x64;
    a.unk25 = 0xFF;
    a.unk26 = 0x20;
    a.unk28 = 7;
    a.unk2A = 0x20;
    a.unk2C = 1.025728941f;
    a.unk30 = 351.0f;
    a.unk34 = 200.0f;
    a.unk38 = pos;
    a.unk44 = 0;
    a.unk46 = -0x19;
    a.unk48 = 0xFF;
    a.unk4A = 0x15;
    a.unk4C = 5.0f;
    a.unk50 = 25.0f;
    a.unk54 = 0.008f;
    a.unk58 = 0.266f;
    a.unk5C = 0x40E07;
    a.unk60 = 0x10;
    a.unk61 = -1;
    a.unk62 = 8;
    a.unk63 = 6;
    a.unk64 = 1;
    a.unk68 = 0.938608f;
    func_15153634(&a, 0xFF, arg1, arg2);
    }
    {
    Arg15152190 b;

    b.unk00 = 0x24;
    b.unk04 = 0xA;
    b.unk08 = pos;
    b.unk14 = 0;
    b.unk16 = 0xFF;
    b.unk18 = -0x28;
    b.unk1A = 0x14;
    b.unk1C = 11.0f;
    b.unk20 = 8.0f;
    b.unk24 = -1.09f;
    b.unk28 = 0.3710000217f;
    b.unk2C = 0x22;
    b.unk2E = 0xF;
    b.unk30 = 0.302000016f;
    b.unk34 = 0.3180000186f;
    b.unk38 = 9.687f;
    func_15152190(&b, D_8009F790, D_8009F7A4, 5, 65.0f, 0, arg1, arg2);
    }
}


struct Obj150B060C {
    f32 unk0;
    f32 unk4;
    struct131 *object;
    f32 x; /* the object's position (struct131 0x10..0x14) */
    f32 y;
    f32 z;
};

/* Looks up object `id` into out; on success also sets out's two constants and copies the
 * object's position. Returns 0 if there is no such object. */
s32 func_150B060C(u8 id, struct Obj150B060C *out) {
    out->object = func_151149AC(id);
    if (out->object == NULL) {
        return 0;
    }
    out->unk0 = -150.0f;
    out->unk4 = 4.5f;
    out->x = out->object->unk10;
    out->y = out->object->unk12;
    out->z = out->object->unk14;
    return 1;
}
