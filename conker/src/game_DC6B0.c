#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 *D_800DBF94;
extern struct131 *func_151149AC(u8);
extern s32 func_1505D024(struct127 *, s32, u16, s32);
extern void func_150A7960(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);
extern void func_150E1AB0(s32, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 speed, f32 stopDist,
                          f32 halfWidth, f32 length, u16 lifetime, s32 texture, s32 mode, s16 onTime, s16 offTime,
                          struct127 *owner, u8 ownerId, s16 bone, s32 ownerTag, s32 impactEffect, u8 unkFE,
                          f32 startX, f32 startY, f32 startZ, f32 endX, f32 endY, f32 endZ);
extern void func_150E2DB4(struct127 *owner, u8 ownerId, s16 bone, s32 ownerTag, f32 startX, f32 startY,
                          f32 startZ, f32 endX, f32 endY, f32 endZ, s16 onTime, s16 offTime, u16 lifetime, u8);
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

/* A sine oscillator: value = base + amplitude * sin(phase), with phase advancing by rate. */
typedef struct {
    /* 0x0 */ f32 base;
    /* 0x4 */ f32 amplitude;
    /* 0x8 */ f32 phase;
    /* 0xC */ f32 rate; /* per unit of D_800BE9A4 */
} Oscillator;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 pad14[0x5C];
    /* 0x70 */ Oscillator osc;
} Obj150AFBF4;

/* Advances the oscillator at 0x70 (func_15144B68 wraps the phase) and writes its value to unk10. */
s32 func_150AFBF4(Obj150AFBF4 *obj) {
    Oscillator *osc = &obj->osc;

    osc->phase = osc->phase + osc->rate * D_800BE9A4;
    osc->phase = func_15144B68(osc->phase);
    obj->unk10 = sinf(osc->phase) * osc->amplitude + osc->base;
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

typedef struct Node150AFDB0 {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ struct Node150AFDB0 *next;
    /* 0x0C */ u8 padC[0x33];
    /* 0x3F */ u8 unk3F;
} Node150AFDB0;

extern Node150AFDB0 *D_800DCE94;

/* Releases (func_1516972C) every node of the D_800DCE94 list whose unk3F is D_800C3E78. Each
 * node's successor is saved on the D_800DD198 iteration stack first, so releasing it is safe. */
void func_150AFDB0(void) {
    Node150AFDB0 *node;

    D_800DD190++;
    for (node = D_800DCE94; node != NULL; node = ((Node150AFDB0 **)D_800DD198)[D_800DD190]) {
        ((Node150AFDB0 **)D_800DD198)[D_800DD190] = node->next;
        if (node->unk3F == D_800C3E78) {
            func_1516972C((struct102 *)node);
        }
    }
    D_800DD190--;
}

/* Fires a tracer along the -z axis of bone 3 (bone 2 if `altBone`) of D_800D154C, from 20 to
 * 150 units out, plus a tracer (func_150E2DB4) attached to the same bone. */
void func_150AFE64(s32 altBone) {
    f32 startX;
    f32 startY;
    f32 startZ;
    f32 endX;
    f32 endY;
    f32 endZ;
    s32 bone;
    f32 (*mtx)[4];

    if (D_800D154C->unk1D4 != NULL) {
        if (altBone == 0) {
            bone = 3;
        } else {
            bone = 2;
        }

        mtx = ((f32 (*)[4][4])D_800D154C->unk1D4)[bone];
        startX = 0.0f;
        startY = 0.0f;
        startZ = -20.0f;
        func_150A7960(mtx, startX, startY, startZ, &startX, &startY, &startZ);
        endX = 0.0f;
        endY = 0.0f;
        endZ = -150.0f;
        func_150A7960(mtx, endX, endY, endZ, &endX, &endY, &endZ);
        /* speed 40, stopDist 0, halfWidth 2, length 120, lifetime 60, texture 35 */
        func_150E1AB0(0, startX, startY, startZ, endX, endY, endZ, 40.0f, 0.0f, 2.0f, 120.0f, 60, 35,
                      0, 0, 0, NULL, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        /* from (0, 0, -39) to (0, 0, -150) in bone space; on 3 frames, off 255, lifetime 4 */
        func_150E2DB4(D_800D154C, D_800D154C->unique_id, bone, -1, 0.0f, 0.0f, -39.0f, 0.0f, 0.0f, -150.0f,
                      3, 0xFF, 4, 0);
    }
}

/* Runs func_150AF200 on objects 0xE2/0xE1 (after func_15179008(0)) when D_800BE9F0 is 6, else on 0xDF/0xDE. */
void func_150B003C(s32 arg0) {
    if (D_800BE9F0 == 6) {
        func_15179008(0);
        func_150AF200(0xE2, 0xE1);
        return;
    }
    func_150AF200(0xDF, 0xDE);
}

/* Projectile spawn template passed to func_1513264C. */
typedef struct GameDC6B0Packet {
    f32 field0, field4, field8, fieldC;
    f32 angle0, angle1, angle2; /* elevation, heading (degrees), roll */
    vertex scale;
    vertex position;
    vertex velocity;
    vertex field40;
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

/* A projectile created by func_1513264C. */
typedef struct {
    u8 pad0[0x170];
    struct102 *trail; /* ribbon effect (func_151B7328) following the projectile */
} GameDC6B0Projectile;

/* Arguments the projectile's ribbon trail keeps (func_151B7328 selector 1). */
typedef struct GameDC6B0TrailArgs {
    GameDC6B0Projectile *source;
    s32 unk4;
} GameDC6B0TrailArgs;

GameDC6B0Projectile *func_1513264C(GameDC6B0Packet *, s32, s32, s32, s32, u8, s32);
void func_15145974(vertex *v, f32 *heading, f32 *elevation);
struct102 *func_151B7328(void *args, s32 selector, s32 argsSize, s32, s32);
s32 func_15145128(vertex *v, vertex *out, f32 *length, f32 *invLength);

/* Fires a projectile from `from` towards `to` at speed 80, with a ribbon trail. Does nothing if
 * the two points coincide. */
void func_150B0094(vertex *from, vertex *to, u8 arg2, s32 arg3) {
    GameDC6B0Packet packet;
    vertex dir;
    f32 length;
    f32 invLength;
    GameDC6B0Projectile *projectile;
    GameDC6B0TrailArgs trailArgs;
    struct102 **trail;

    trailArgs.unk4 = 0;
    dir.x = to->x - from->x;
    dir.y = to->y - from->y;
    dir.z = to->z - from->z;
    if (func_15145128(&dir, &dir, &length, &invLength) != 0) {
        packet.field4 = packet.field0 = 1.0f;
        packet.field8 = packet.fieldC = 0.7f;
        packet.position = *from;
        packet.velocity.x = dir.x * 80.0f;
        packet.velocity.y = dir.y * 80.0f;
        packet.velocity.z = dir.z * 80.0f;
        func_15145974(&packet.velocity, &packet.angle1, &packet.angle0);
        packet.scale.z = packet.scale.y = packet.scale.x = 1.0f;
        packet.angle2 = 0.0f;
        packet.field40 = *(vertex *)&D_800A5480; /* D_800A5480 holds a zero vertex; variables.h declares it as f32 */
        packet.field50 = 0x19A0;
        packet.field54 = 300;
        packet.field56 = 210;
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
        projectile = func_1513264C(&packet, 3, 0xFF, 0, 4, arg2, arg3);
        if (projectile != NULL) {
            trail = &projectile->trail;
            memcpy(trail, &trailArgs.unk4, sizeof(trailArgs.unk4));
            trailArgs.source = projectile;
            *trail = func_151B7328(&trailArgs, 1, sizeof(trailArgs), arg2, arg3);
        }
    }
}

/* Releases the projectile's ribbon trail, if any. */
void func_150B02C0(GameDC6B0Projectile *projectile) {
    struct102 **trail = &projectile->trail;

    if (*trail != NULL) {
        func_1516972C(*trail);
    }
}

extern void func_15132570(void *);

/* Releases the trail, then func_15132570. */
void func_150B02F0(GameDC6B0Projectile *projectile) {
    func_150B02C0(projectile);
    func_15132570(projectile);
}

extern void func_1513259C(void *);

/* Releases the trail, then func_1513259C. */
void func_150B031C(GameDC6B0Projectile *projectile) {
    func_150B02C0(projectile);
    func_1513259C(projectile);
}

/* Particle burst description passed to func_15153634. */
typedef struct {
    /* 0x00 */ s16 count;
    /* 0x02 */ s16 countRange;
    /* 0x04 */ u8 kind;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ s32 flags;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s16 timer;
    /* 0x12 */ s16 timerRange;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 unk21;
    /* 0x22 */ u8 unk22;
    /* 0x23 */ u8 unk23;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 unk25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ vertex position;
    /* 0x44 */ s16 yaw;
    /* 0x46 */ s16 pitch;
    /* 0x48 */ s16 yawRange;
    /* 0x4A */ s16 pitchRange;
    /* 0x4C */ f32 speed;
    /* 0x50 */ f32 speedRange;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s8 unk60;
    /* 0x61 */ s8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 pad65[3];
    /* 0x68 */ f32 unk68;
} ParticleBurst;

/* Debris description passed to func_15152190: a random number of pieces thrown in random
 * directions from origin. */
typedef struct {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ vertex origin;
    /* 0x14 */ s16 yaw;
    /* 0x16 */ s16 yawRange;
    /* 0x18 */ s16 pitch;
    /* 0x1A */ s16 pitchRange;
    /* 0x1C */ f32 speed;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 scale;
    /* 0x34 */ f32 scaleRange;
    /* 0x38 */ f32 spin;
} DebrisBurst;

extern void func_15153634(ParticleBurst *, s32, u8, s32);
extern void func_15152190(DebrisBurst *, const s32 *variantIds, const f32 *variantScales, s32 variantCount,
                          f32 radius, s32, u8, s32);

extern const s32 D_8009F790[5];
extern const f32 D_8009F7A4[5];

/* Spawns a particle burst and a burst of debris (5 variants) 30 units above obj. */
void func_150B0348(struct127 *obj, u8 arg1, s32 arg2) {
    vertex pos;

    pos.x = obj->x_position;
    pos.y = obj->y_position + 30.0f;
    pos.z = obj->z_position;
    {
        ParticleBurst burst;

        burst.count = 10;
        burst.countRange = 7;
        burst.kind = 0x6C;
        burst.unk6 = 0x5103;
        burst.flags = 0x200005;
        burst.unkC = 0;
        burst.timer = 30;
        burst.timerRange = 15;
        burst.unk14 = 0;
        burst.unk18 = 0;
        burst.unk1F = 0xFF;
        burst.unk1C = 0x7F;
        burst.unk1D = 0x4B;
        burst.unk1E = 6;
        burst.unk20 = 0x80;
        burst.unk21 = 0x53;
        burst.unk22 = 0;
        burst.unk23 = 100;
        burst.unk24 = 100;
        burst.unk25 = 0xFF;
        burst.unk26 = 32;
        burst.unk28 = 7;
        burst.unk2A = 32;
        burst.unk2C = 1.025728941f;
        burst.unk30 = 351.0f;
        burst.unk34 = 200.0f;
        burst.position = pos;
        burst.yaw = 0;
        burst.pitch = -25;
        burst.yawRange = 0xFF;
        burst.pitchRange = 21;
        burst.speed = 5.0f;
        burst.speedRange = 25.0f;
        burst.unk54 = 0.008f;
        burst.unk58 = 0.266f;
        burst.unk5C = 0x40E07;
        burst.unk60 = 16;
        burst.unk61 = -1;
        burst.unk62 = 8;
        burst.unk63 = 6;
        burst.unk64 = 1;
        burst.unk68 = 0.938608f;
        func_15153634(&burst, 0xFF, arg1, arg2);
    }
    {
        DebrisBurst debris;

        debris.count = 36;
        debris.countRange = 10;
        debris.origin = pos;
        debris.yaw = 0;
        debris.yawRange = 0xFF;
        debris.pitch = -40;
        debris.pitchRange = 20;
        debris.speed = 11.0f;
        debris.speedRange = 8.0f;
        debris.unk24 = -1.09f;
        debris.unk28 = 0.3710000217f;
        debris.unk2C = 34;
        debris.unk2E = 15;
        debris.scale = 0.302000016f;
        debris.scaleRange = 0.3180000186f;
        debris.spin = 9.687f;
        func_15152190(&debris, D_8009F790, D_8009F7A4, 5, 65.0f, 0, arg1, arg2);
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
