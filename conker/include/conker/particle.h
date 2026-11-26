#ifndef CONKER_PARTICLE_H
#define CONKER_PARTICLE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "particle": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_35E80.c
 *   game_39750.c
 *   game_DC6B0.c
 *   game_DF2D0.c
 *   game_DF930.c
 *   game_E5E90.c
 *   game_E6A10.c
 *   game_E7380.c
 *   game_E7970.c
 *   game_E7DE0.c
 *   game_EDE60.c
 *   game_EE710.c
 *   game_EF410.c
 *   game_EFEA0.c
 *   game_F0610.c
 *   game_F3270.c
 *   game_F8590.c
 *   game_F9430.c
 *   game_FA240.c
 *   game_FA360.c
 *   game_FB060.c
 *   game_FFBA0.c
 *   game_1028F0.c
 *   game_113480.c
 *   game_113D60.c
 *   game_116D40.c
 *   game_1199D0.c
 *   game_11FF10.c
 *   game_120950.c
 *   game_121A20.c
 *   game_128D70.c
 *   game_12DAB0.c
 *   game_135780.c
 *   game_1368C0.c
 *   game_15D730.c
 *   game_15F680.c
 *   game_169510.c
 *   game_176A00.c
 *   game_177480.c
 *   game_177B50.c
 *   game_1792B0.c
 *   game_1792D0.c
 *   game_179400.c
 *   game_1797A0.c
 *   game_17CAF0.c
 *   game_183640.c
 *   game_188440.c
 *   game_193E50.c
 *   game_197120.c
 *   game_19A8B0.c
 *   game_19F150.c
 *   game_1A1B40.c
 *   game_1A7260.c
 *   game_1A7490.c
 *   game_1B74A0.c
 *   game_1B81D0.c
 *   game_1B8B60.c
 *   game_1B9F30.c
 *   game_1BA1D0.c
 *   game_1BA670.c
 *   game_1BB950.c
 *   game_1BC650.c
 *   game_1BD940.c
 *   game_232150.c (data only, linked after the game_1BA670..game_1BDA00 unit)
 *   game_1BDC20.c
 *   game_1BF090.c
 *   game_1BFC70.c
 *   game_1BFDD0.c
 *   game_1C0B10.c
 *   game_1CDEC0.c
 *   game_1CE2F0.c
 *   game_1D0600.c
 *   game_1D2B10.c
 *   game_1D3AB0.c
 *   game_1D9A00.c
 *   game_1DBD40.c
 *   game_1DC720.c
 *   game_1DCA70.c
 *   game_1DEF60.c
 *   game_1E30A0.c
 *   game_1E34C0.c
 *   game_1E5FF0.c
 *   game_1E6B40.c
 *   game_1E9820.c
 *   game_1F1CD0.c
 *   game_1FA770.c
 *   game_1FBB80.c
 *   game_200930.c
 *   game_2062D0.c
 *   game_209B50.c
 *   game_20A290.c
 */

/* An RGB colour, one byte per channel (game_F8590.c, game_F9430.c). */
typedef struct Rgb8 {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
} Rgb8; /* size 0x3 */

/* game/done/game_35E80.c */
void func_150089D0(void);

/* game_39750.c */
/* An area effect of func_150139AC (game_40490.c): the particles spawned over a zone. */
typedef struct Struct150139ACEntry {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    u8 unk34;
    u8 pad35[3];
} Struct150139ACEntry;

extern const Struct150139ACEntry D_80095FA0[6];

void func_1500C2A0(void);
void func_1500C2F0(TriggerZone *region, f32 density);
void func_1500C8D0(TriggerZone *region, f32 density);
void func_1500CE4C(TriggerZone *region, f32 density);
void func_1500D3AC(TriggerZone *region, f32 density);
void func_1500D8C8(TriggerZone *region, f32 density);
void func_1500DE88(TriggerZone *region, f32 density);

/* game_DC6B0.c */
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 pad4[2];
    s16 unk8;
} Range150AF2E0;

/* Per-effect data of the func_150AF328 emitter. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 spawnAccum;
} SmokeEmitterData;

/* Timed effect of func_150AF328. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ SmokeEmitterData data;
} SmokeEmitter;

typedef struct GameDC6B0SmokeTimers {
    s16 puff;
    s16 spark;
} GameDC6B0SmokeTimers;

typedef struct GameDC6B0SmokeSource {
    TimedEffect base;
    GameDC6B0SmokeTimers timers;
} GameDC6B0SmokeSource;

/* A sine oscillator: value = base + amplitude * sin(phase), with phase advancing by rate. */
typedef struct {
    /* 0x0 */ f32 base;
    /* 0x4 */ f32 amplitude;
    /* 0x8 */ f32 phase;
    /* 0xC */ f32 rate; /* per unit of D_800BE9A4 */
} TimeOscillator;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 pad14[0x5C];
    /* 0x70 */ TimeOscillator osc;
} Obj150AFBF4;

/* A projectile created by func_1513264C. */
typedef struct {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ EffectObj *trail; /* ribbon effect (func_151B7328) following the projectile */
} GameDC6B0Projectile;

struct Obj150B060C {
    f32 unk0;
    f32 unk4;
    WorldObject *object;
    f32 x; /* the object's position (WorldObject 0x10..0x14) */
    f32 y;
    f32 z;
};

void func_150AF200(s32 idA, s32 idB);
void func_150AF2E0(void *arg0, Range150AF2E0 *range);
void func_150AF328(SmokeEmitter *emitter);
s32 func_150AF6E4(SpriteParticle *particle, s32 arg1);
void func_150AF738(s16 lifetime, u8 arg1, s32 arg2);
void func_150AF790(void *arg0, u8 *arg1);
void func_150AF7C4(GameDC6B0SmokeSource *source);
s32 func_150AFBF4(Obj150AFBF4 *obj);
void func_150AFC68(s32 object, s32 bone, s32 timer, s32 radiusXZ, u8 arg4, s32 arg5);
void func_150AFDB0(void);
void func_150AFE64(s32 altBone);
void func_150B003C(s32 arg0);
void func_150B0094(Vec3f *from, Vec3f *to, u8 arg2, s32 arg3);
void func_150B02C0(GameDC6B0Projectile *projectile);
void func_150B02F0(GameDC6B0Projectile *projectile);
void func_150B031C(GameDC6B0Projectile *projectile);
void func_150B0348(Actor *obj, u8 arg1, s32 arg2);
/* (u8 id, struct Obj150B060C *out); declared without a prototype: game_36680.c passes a third argument */
s32 func_150B060C();

/* game_DF2D0.c */
/* Data of the effect spawned by func_150B1EE0 and run by func_150B1F90. */
typedef struct {
    Vec3f pos;
    f32 accum; /* particles owed; one is spawned per whole unit */
} PointEmitterData;

/* Timed effect of func_150B1F90. */
typedef struct {
    TimedEffect base;
    PointEmitterData data;
} PointEmitter;

void func_150B1E20(Camera *camera);
void func_150B1EE0(EffectHeader *spawner, Vec3f *pos, s32 arg2, s32 arg3);
void func_150B1F90(PointEmitter *effect);
void func_150B2340(Camera *camera);

/* game_DF930.c */
s32 func_150B2480(s32 arg0, s32 arg1);
void func_150B2494(Actor *obj, s32 arg1, s32 arg2);
s32 func_150B2570(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 owner);

/* game_E5E90.c */
/* Wall-collision query of game_15F680.c (only its position is read here). */
typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ Vec3f pos;
} CollisionProbe;

/* A particle spawned by func_1513D594: the 0x110-byte particle, its 0x18-byte quad axes, then the
 * caller's motion block. */
typedef struct {
    /* 0x000 */ u8 pad0[0x128];
    /* 0x128 */ f32 motion[2];
} AxisParticle;

s32 func_150B89E0(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, void *surface);
s32 func_150B8F44(ModelParticle *particle, CollisionProbe *probe, f32 x, f32 y, f32 z);
extern Actor *D_80088720;
extern Vec3f D_80088724;
extern Actor *D_80088730;
extern s32 D_80088740;

/* game_E6A10.c */
/* A live particle of game_169510.c whose motion block is a fall speed (func_150B9560); the update callbacks
 * of D_80089CB8 / D_80089D60 get one. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ f32 fallSpeed;     /* first word of the per-behaviour motion block after the particle */
} FallGrowParticle;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x83];
    /* 0x90 */ s32 unk90;
    /* 0x94 */ u8 pad94[4];
    /* 0x98 */ s16 unk98;
    /* 0x9A */ u8 pad9A[2];
    /* 0x9C */ s16 unk9C;
    /* 0x9E */ u8 pad9E[2];
    /* 0xA0 */ s16 unkA0;
    /* 0xA2 */ s16 unkA2;
    /* 0xA4 */ u8 padA4[2];
    /* 0xA6 */ s16 unkA6;
} Obj150B9D8C;

s32 func_150B9560(FallGrowParticle *particle);
s32 func_150B95FC(FallGrowParticle *particle);
s32 func_150B961C(QuadSprite *sprite);
s32 func_150B963C(ModelParticle *particle);
s32 func_150B9D14(Mtx *mtx, ModelParticle *particle);
void func_150B9D8C(Obj150B9D8C *obj);

/* game_E7380.c */
/* Per-behaviour motion block stored right after a particle. */
typedef struct {
    /* 0x00 */ Vec3f vel;
    /* 0x0C */ f32 rotVelX;
    /* 0x10 */ f32 rotVelY;
    /* 0x14 */ f32 minY;           /* the particle expires below this height */
} FxParticleMotion;

/* A live particle of game_169510.c whose motion block is a velocity and spin (func_150BA37C); the update
 * callbacks of D_80089CB8 / D_80089D60 get one. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ FxParticleMotion motion;
} TumblingParticle;

void func_150B9ED0(Actor *actor, f32 prevX, f32 prevZ, u8 arg3, s32 arg4);
s32 func_150BA35C(QuadSprite *sprite);
s32 func_150BA37C(TumblingParticle *particle);
s32 func_150BA424(TumblingParticle *particle);

/* game_E7970.c */
/* Per-effect data: the object the effect is attached to and a spawn accumulator. */
typedef struct {
    /* 0x0 */ Actor *owner;
    /* 0x4 */ u8 ownerId; // owner's unique_id when the effect was created
    /* 0x8 */ f32 spawnAccum;
} EffectE7970Data; // size 0xC

/* Effect slot as returned by func_15149130 (a TimedEffect); the effect data lives at 0x28. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EffectE7970Data data;
} EffectE7970;

void func_150BA4C0(Actor *owner, u8 arg1, s32 arg2);
void func_150BA55C(EffectE7970 *effect);
void func_150BA8F0(EffectE7970 *effect, s32 arg1, u8 arg2);

/* game_E7DE0.c */
s32 func_150BA930(Vec3f *pos, Actor *obj, CollisionHit *hit, s32 arg3);
s32 func_150BAA00(s32 arg0, s32 arg1);
void func_150BAA14(Actor *obj, s32 arg1, s32 arg2);

/* game_EDE60.c */
/* Live particle spawned by an emitter (see func_15130280). */
typedef struct PoolParticle PoolParticle;

/* One entry of an emitter's slot pool; slots are chained by index, -1 ends a chain. */
typedef struct {
    /* 0x00 */ PoolParticle *particle;
    /* 0x04 */ s16 next;
    /* 0x06 */ s16 pad6;
} PoolEmitterSlot;

/* Describes an emitter; func_150C0AC0 copies it into the emitter. */
typedef struct {
    /* 0x00 */ Vec3f *source;     /* followed when flags & 2 */
    /* 0x04 */ f32 unk4;          /* func_1514F5CC stores 0.0f */
    /* 0x08 */ Actor *target;  /* followed otherwise */
    /* 0x0C */ u8 targetId;        /* target->unique_id; a mismatch means the object was reused */
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ f32 rate;           /* particles per frame step while the target moves */
    /* 0x14 */ s16 slotCount;
    /* 0x16 */ s16 timer;          /* counts down when flags & 1 */
    /* 0x18 */ u8 flags;
    /* 0x19 */ u8 pad19[3];
} PoolEmitterDesc; /* size 0x1C */

/* Emitter object that keeps spawning particles behind a moving object. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ PoolEmitterDesc desc;
    /* 0x34 */ f32 lastX;
    /* 0x38 */ f32 lastZ;
    /* 0x3C */ f32 spawnAccum;
    /* 0x40 */ PoolEmitterSlot *slots; /* desc.slotCount slots, stored right after the emitter */
    /* 0x44 */ s16 activeHead;     /* chain of slots holding a live particle */
    /* 0x46 */ s16 freeHead;       /* chain of unused slots */
} PoolEmitter; /* size 0x48 */

struct PoolParticle {
    /* 0x00 */ DescParticleHead head;  /* head.desc.pos: the anchor the emitter moves it to */
    /* 0xA8 */ PoolEmitter *owner;
};

void func_150C09B0(PoolEmitter *emitter, s32 arg1, u8 arg2);
void func_150C09F0(PoolEmitter *emitter);
void func_150C0A1C(PoolEmitter *emitter);
void func_150C0A48(PoolEmitter *emitter);
PoolEmitter *func_150C0AC0(PoolEmitterDesc *desc, s32 arg1, s32 arg2);
void func_150C0C38(PoolEmitter *emitter);
void func_150C1198(PoolParticle *particle);

/* game_EE710.c */
void func_150C1260(Actor *actor, u8 foot);

/* game_EF410.c */
void func_150C1F60(u8 arg0);
void func_150C2290(u8 arg0);
void func_150C2424(u8 arg0);
s32 func_150C251C(ChannelParticle *particle);
s32 func_150C2558(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
s32 func_150C2700(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
void func_150C2804(f32 *x, f32 *y, f32 *z, s16 delay, s32 arg4, s32 arg5);
void func_150C2898(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, Struct150C29F0 *emitter);

/* game_EFEA0.c */
void func_150C29F0(Struct150C29F0 *emitter);
void func_150C2C00(Struct150C29F0 *emitter);
s32 func_150C2FCC(ChannelParticle *particle);
s32 func_150C308C(ChannelParticle *particle);

/* game_F0610.c */
Gfx *func_150C3160(Gfx *gdl, Actor *obj);
void func_150C3230(Actor *obj);
void func_150C3574(Actor *obj);
void func_150C3994(Actor *obj);
s32 func_150C3D48(s32 value);
void func_150C3D5C(Actor *obj);

/* game_F3270.c */
/* The objects an emitter keeps alive; each clears its own slot when it is destroyed. */
// matching: volatile makes IDO reload the slot after the NULL test (func_150C5DC0, func_150C5F40)
typedef struct {
    /* 0x0 */ FlickerSprite *volatile particle;
    /* 0x4 */ WatcherEmitter *volatile linked;
} GameF3270Children;

/* A character watcher with GameF3270Children as extra data. */
typedef struct GameF3270Emitter {
    /* 0x00 */ CharacterWatcher watcher;
    /* 0x58 */ GameF3270Children children;
} GameF3270Emitter;

void func_150C5DC0(GameF3270Emitter *emitter);
s8 func_150C5E0C(FlickerSprite *particle);
s32 func_150C5EFC(FlickerSprite *particle);
void func_150C5F40(GameF3270Emitter *emitter);
WatcherEmitter *func_150C5F94(Actor *actor, GameF3270Emitter *emitter);
void func_150C60D8(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, WatcherEmitter *source);
s32 func_150C63EC(WatcherEmitter *obj);
void func_150C6410(WatcherEmitter *obj);
void func_150C6438(WatcherEmitter *obj);
FlickerSprite *func_150C6460(GameF3270Emitter *emitter);

/* game_F8590.c */
/* Growth applied by func_150CBA30, stored in the dust particle at 0x128. */
typedef struct {
    /* 0x00 */ s16 growTime; // frames left to grow
    /* 0x04 */ f32 growRate; // size gained per frame
} DustGrowth;

/* The dust puff particle func_150CB1F4 spawns (func_1513D668, a DefParticleHead view); updated by func_150CBA30. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0xC];
    /* 0x01C */ s16 timer;
    /* 0x01E */ u8 pad1E[0xE];
    /* 0x02C */ f32 size0;
    /* 0x030 */ f32 size1;
    /* 0x034 */ u8 pad34[0x24];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0xCB];
    /* 0x128 */ DustGrowth growth;
} DustPuff;

/* Particle moved by func_150CBE88. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x10];
    /* 0x20 */ f32 unk20[3]; // integrated from unk50
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ f32 pos[3];
    /* 0x44 */ f32 vel[3];
    /* 0x50 */ f32 unk50[3];
    /* 0x5C */ f32 gravity;  // added to vel[1]
} BallisticParticle;

/* Particle handled by func_150CBF5C. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0xC];
    /* 0x1C */ s16 timer;
    /* 0x1E */ u8 pad1E[0x3A];
    /* 0x58 */ s32 flags;
} FadeoutParticle;

s32 func_150CB0E0(Vec3f *pos, Actor *actor, CollisionHit *ground, u8 side);
s32 func_150CB1E0(s32 arg0, s32 arg1);
void func_150CB1F4(Actor *actor, s32 side, s32 arg2);
s32 func_150CB7CC(QuadSprite *sprite);
s32 func_150CB800(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);
s32 func_150CBA30(DustPuff *dust);
s32 func_150CBABC(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
s32 func_150CBCE0(SparkParticle *particle, s32 arg1);
s32 func_150CBE88(BallisticParticle *particle);
void func_150CBF5C(FadeoutParticle *particle);
extern struct Queue *D_800D98E0;

/* game_F9430.c */
/* Per-particle data copied to 0x128 of a particle by func_150CBF80. */
typedef struct {
    /* 0x0 */ s16 growDelay; /* age after which the particle starts growing */
    /* 0x4 */ f32 growRate;
} GrowParticleExtra;

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 age;
    /* 0x01E */ u8 pad1E[0x2C - 0x1E];
    /* 0x02C */ f32 size0;
    /* 0x030 */ f32 size1;
    /* 0x034 */ u8 pad34[0x58 - 0x34];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0x128 - 0x5D];
    /* 0x128 */ GrowParticleExtra extra;
} GrowParticle;

typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ f32 decay; /* set by func_150CC6B8 */
} DecayParticle;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 rotation[3];
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ f32 position[3];
    /* 0x44 */ f32 velocity[3];
    /* 0x50 */ f32 rotationVel[3];
    /* 0x5C */ f32 gravity;
} TumbleParticle;

void func_150CBF80(Actor *owner, u8 index, s32 arg2, u8 arg3);
s32 func_150CC638(GrowParticle *p);
s32 func_150CC6B8(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);
s32 func_150CC8D4(DecayParticle *p, s32 arg1);
s32 func_150CCA7C(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);
s32 func_150CCCB4(TumbleParticle *p);

/* game_FA240.c */
void func_150CCD90(f32 z, f32 *r, f32 *g, f32 *b);

/* game_FA360.c */
/* Per-effect motion state of a drifting quad (lives at DriftQuad + 0x110). */
typedef struct {
    /* 0x00 */ Vec3f vel;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ f32 drag;
    /* 0x14 */ Vec3f spin; /* added to DriftQuad.dir every frame */
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ f32 swayLength;
    /* 0x28 */ f32 swayLengthRate;
    /* 0x2C */ f32 swayYaw;
    /* 0x30 */ f32 phase;
    /* 0x34 */ f32 phaseSpeed;
    /* 0x38 */ f32 phaseScale;
    /* 0x3C */ f32 minVy;
} QuadDriftMotion; /* size 0x40 */

/* Effect instance created by func_1513D524 from an ParticleDef. */
typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ f32 halfWidth;
    /* 0x030 */ f32 halfHeight;
    /* 0x034 */ Vec3f pos;
    /* 0x040 */ Vec3f dir;   /* rotation angles for func_150A8050 */
    /* 0x04C */ Vec3f sway;  /* offset added to pos while falling */
    /* 0x058 */ u8 pad58[0x68];
    /* 0x0C0 */ Vtx_t template[4];
    /* 0x100 */ Vtx_t *buffers[4]; /* each holds two copies of the 4-vertex quad */
    /* 0x110 */ QuadDriftMotion drift;
} DriftQuad;

typedef struct {
    f32 pending; /* bursts owed; one is spawned per whole unit */
    f32 rate;    /* 0..1, set by func_150CDB6C */
} BurstTimer;

/* Timed effect driven by func_150CD7F8 (a D_8008A4E8 tick callback); D_80088870 points at one. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BurstTimer timer;
} DriftBurstEmitter;

extern DriftBurstEmitter *D_80088870;

DriftQuad *func_150CCEB0(DriftSpawn *spawn, u8 arg1, u8 arg2);
s32 func_150CD17C(DriftQuad *effect);
Vtx_t *func_150CD59C(DriftQuad *effect, s16 bufferIndex);
void func_150CD7F8(DriftBurstEmitter *emitter);
void func_150CDB6C(s32 intensity);

/* game_FB060.c */
void func_150CDBB0(Actor *actor, u8 owner, s32 arg2);
s32 func_150CDCF4(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);
s32 func_150CDF10(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);

/* game_FFBA0.c */
/* Object data (at +0x28) of a trigger that spawns spark emitters. */
typedef struct EmitterTrigger {
    s16 *record;
    s32 minDelay;
    s32 delayRange;
    s32 countdown;
    MtxF matrix;
    u8 params[2]; // [0] bit 0: spawn periodically; [1]: id matched by event 0x34
                  // matching: one array; two u8 fields swap the compare's registers in func_150D32FC
} EmitterTrigger;

/* Object data (at +0x28) of a spark emitter: created by func_150D278C, updated by func_150D2924. */
typedef struct RingSparkEmitter {
    s16 *record;
    f32 angle;
    f32 rate;
    f32 interval;
    f32 angleStep;
    f32 spawnAccum;
    f32 elapsed;
    MtxF matrix;
    u8 color[3];
} RingSparkEmitter; // size 0x60

/* Timed effect (func_15149130) of this file, with an emitter trigger or a spark emitter (func_150D278C) as data. */
typedef struct SparkEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ union {
        EmitterTrigger trigger;
        RingSparkEmitter emitter;
    } data;
} SparkEffect;

/* Motion data (at +0x110) of a spark particle spawned by func_150D2924. */
typedef struct SparkMotion {
    f32 posX;
    f32 posY;
    f32 baseX;
    f32 baseY;
    f32 amplitudeX;
    f32 amplitudeY;
    f32 phaseX;
    f32 phaseY;
    f32 phaseSpeedX;
    f32 phaseSpeedY;
    f32 life;
    f32 fadeScale;
} SparkMotion;

typedef struct RingSparkParticle {
    u8 pad0[0x5C];
    u8 unk5C; // fades out with the remaining life
    u8 pad5D[0xB3];
    SparkMotion motion;
} RingSparkParticle;

typedef struct GameFFBA0Beam {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ f32 texture[2];
} GameFFBA0Beam;

void func_150D26F0(SparkEffect *obj);
void func_150D278C(s16 *record, MtxF matrix, u8 unkC, s32 unk1);
void func_150D2924(SparkEffect *obj);
Vtx *func_150D2D6C(GameFFBA0Beam *beam, s16 index);
s32 func_150D317C(RingSparkParticle *particle);
void func_150D32FC(SparkEffect *obj, u8 *msg, u8 event);

/* game_1028F0.c */
/* Effect (a TimedEffect with per-effect data at 0x28) that follows an object. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 spawnAccum;
} FollowEffectData;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ FollowEffectData data;
} FollowEffect;

/* Per-particle data of the drifting particles that follow an object (func_150D6CC4). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 drag; /* passed to func_15131958 with the velocity */
} FollowDriftData;

/* Per-particle data of the growing, fading particle that follows an object (func_150D6F0C). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 time;
    /* 0x0C */ f32 lifetime;
    /* 0x10 */ f32 growth;    /* size = sqrt(time) * growth */
    /* 0x14 */ f32 fadeStart; /* unk2B = fadeStart - fadeRate * time^2 */
    /* 0x18 */ f32 fadeRate;
} GrowParticleData;

/* Live particle, as created by func_15130280 from a ParticleDesc. */
typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ union {
        FollowDriftData drift;
        GrowParticleData grow;
    } data;
} FollowParticle;

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ u8  pad08[0xC];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s8  unk50;
} Struct150D64E8;

void func_150D5440(Actor *obj, u8 arg1, s32 arg2);
void func_150D54C8(FollowEffect *effect);
void func_150D596C(FollowEffect *effect, ActorMsg *msg, u8 type);
void func_150D5A2C(TimedEffect *effect);
void func_150D5A4C(TimedEffect *effect);
void func_150D5A6C(Actor *obj, u8 arg1, s32 arg2);
void func_150D6388(Vec3f *pos, u8 arg1, s32 arg2);
void func_150D6434(Vec3f *pos, u8 arg1, s32 arg2);
void func_150D64E8(Struct150D64E8 *info, f32 *outValue);
void func_150D65F0(CollisionTri *surface, Vec3f *pos, u8 arg2, s32 arg3);
void func_150D66A4(u8 arg0, u8 arg1, s32 arg2);
void func_150D6730(Actor *obj, u8 arg1, s32 arg2);
void func_150D6C98(Actor *obj, Vec3f *out);
s32 func_150D6CC4(FollowParticle *particle, s32 arg1);
void func_150D6E60(FollowParticle *particle, ActorMsg *msg, u8 type);
s32 func_150D6F0C(FollowParticle *particle, s32 arg1);
void func_150D7068(FollowParticle *particle, ActorMsg *msg, u8 type);

/* game_113480.c */
/* Hit record copied to Streak.impact (+0x110) when a streak is spawned. */
typedef struct {
    /* 0x00 */ u8 flags; /* bit 0: the streak's path hits a surface */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Vec3f point;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ CollisionTri tri; /* the triangle hit */
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ f32 side; /* side of the surface the streak starts on (+1 / -1) */
    /* 0x2C */ s8 callback; /* index into D_80088A10, -1 for none */
    /* 0x2D */ u8 pad2D[3];
} StreakImpact; /* size 0x30 */

/* The particle object spawned by func_150E5FD0 (kind 0x41). */
typedef struct {
    /* 0x00 */ DefParticleHead head; /* head.def.height: length, head.def.rot: direction, head.def.scale.y: speed, head.def.unk44: alpha */
    /* 0x110 */ StreakImpact impact;
} Streak;

void *func_150E5FD0(Vec3f *start, Vec3f *dir, f32 arg2, f32 arg3, f32 arg4, u8 arg5, s32 arg6, u8 checkHit, u8 arg8,
                    s16 life, s8 callback);
s8 func_150E6230(Streak *streak);
void *func_150E63A0(Streak *streak, s16 viewIdx);
s32 func_150E679C(Streak *streak);
s32 func_150E67D0(Streak *streak);

/* game_113D60.c */
typedef struct Game113D60Emission {
    /* 0x00 */ ParticleStream stream;
    /* 0x38 */ f32 scale;
    /* 0x3C */ s16 duration, durationRange;
    /* 0x40 */ u8 color;
} Game113D60Emission; // size 0x44

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Game113D60Emission emission;
} Game113D60Effect;

typedef struct {
    /* 0x00 */ f32 rate;
    /* 0x04 */ f32 accum;
    /* 0x08 */ s16 count;
    /* 0x0A */ s16 next;
} Game113D60BurstHead;

typedef struct {
    /* 0x00 */ Game113D60BurstHead head;
    /* 0x0C */ s32 pad0C;
    /* 0x10 */ f32 points[2]; // `head.count` interleaved x, y pairs
} Game113D60Burst;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Game113D60Burst burst;
} Game113D60BurstEffect;

/* Extra data of the particles func_150E8470 spawns, stored at 0xA8 of each particle: effects.h WobbleData
 * without the drag (the particles are spawned with only these 0xC bytes). */
typedef struct {
    /* 0x0 */ u8 phase[2];
    /* 0x2 */ u8 phaseStep[2];
    /* 0x4 */ f32 amplitude[2];
} Game113D60ParticleExtra;

typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ Game113D60ParticleExtra extra;
} Game113D60Particle;

typedef struct {
    /* 0x0 */ Vec3f position;
    /* 0xC */ f32 accum;
} Game113D60Emitter;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Game113D60Emitter emitter;
} Game113D60EmitterEffect;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ f32 timer;
} Game113D60TimerEffect;

/* Spawn rate data shared by the region spawners: `rate + random * rateRange` per frame. */
typedef struct {
    /* 0x0 */ f32 rate;
    /* 0x4 */ f32 rateRange;
    /* 0x8 */ f32 accum;
} Game113D60Spawner;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Game113D60Spawner spawner;
} Game113D60RegionSpawnerEffect;

/* A spawner at a fixed point: `position`, then the spawn rate as in Game113D60Spawner. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 rate;
    /* 0x10 */ f32 rateRange;
    /* 0x14 */ f32 accum;
} Game113D60PointSpawner;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Game113D60PointSpawner spawner;
} Game113D60PointSpawnerEffect;

/* Extra data of the particles func_150E93DC spawns, stored at 0x170 of each particle. */
typedef struct {
    /* 0x00 */ f32 floorY;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ CollisionTri floorTri; /* never filled: the splash spawns with it as its surface */
    /* 0x1A */ u8 pad1A[2];
} Game113D60DebrisExtra;

typedef struct {
    /* 0x000 */ ModelParticle base;
    /* 0x170 */ Game113D60DebrisExtra extra;
} Game113D60Debris;

void func_150E68B0(Actor *actor, s32 arg1, TriggerZone *zone, u8 sourceIndex);
void func_150E6B84(Game113D60Effect *effect);
void func_150E6E34(Vec3f *out);
void func_150E6ED8(Vec3f *out);
void func_150E6F18(Vec3f *out);
void func_150E6FAC(Vec3f *out, Actor *actor);
void func_150E70CC(Vec3f *out, Actor *actor);
void func_150E70EC(Vec3f *start, Vec3f *end, Vec3f *delta, Game113D60Emission *emission);
void func_150E71E4(Vec3f *start, Vec3f *end, Vec3f *delta, Game113D60Emission *emission);
/* (u8 arg0, u8 owner, s32 kind); declared without a prototype: game_CDE80.c passes an s32 arg0 without narrowing it */
void func_150E7290();
void func_150E75A0(Vec2f *position, f32 size, s16 unk12, u8 flags, u8 unk1E, u8 unk23, s16 unk16, s16 unk18, s32 *ids,
                   s32 idCount, u8 owner, s32 kind);
void func_150E76D0(f32 size, s16 unk12, u8 flags, u8 unk1E, u8 unk23, s16 unk16, s16 unk18, s32 *ids, s32 idCount,
                   u8 owner, s32 kind);
TimedEffect *func_150E7994(s16 count, f32 rate, u8 owner, s32 kind);
void func_150E7C9C(Game113D60BurstEffect *effect);
/* (u8 index, u8 owner, s32 kind); declared without a prototype: the K&R definition changes with one */
void func_150E81A8();
void func_150E83AC(const Vec3f *position, s16 duration, u8 owner, s32 kind);
void func_150E8470(Game113D60EmitterEffect *effect);
s32 func_150E8824(Game113D60Particle *particle, s32 arg1);
void func_150E8854(void);
void func_150E88C0(Game113D60TimerEffect *effect);
/* (void); declared without a prototype: func_150E88C0 passes the effect */
void func_150E8930();
void func_150E8A80(void);
void func_150E8B1C(Game113D60RegionSpawnerEffect *effect);
void func_150E8D5C(Game113D60PointSpawnerEffect *effect);
void func_150E90DC(void);
void func_150E9178(Game113D60RegionSpawnerEffect *effect);
void func_150E93DC(Game113D60PointSpawnerEffect *effect);
s32 func_150E971C(Game113D60Debris *debris);
extern TriggerZone *D_800D9A20[2]; /* end points of source 1; func_15014040 registers them (by unk18) */

/* game_116D40.c */
/* Spawner state at +0x28 of a burst emitter. */
typedef struct {
    /* 0x00 */ TriggerZone *source; // where func_1514470C places each burst
    /* 0x04 */ f32 rateBase;    // bursts per unit of D_800BE9A4
    /* 0x08 */ f32 rateRange;
    /* 0x0C */ f32 scaleBase;   // size scale passed to func_150E9890
    /* 0x10 */ f32 scaleRange;
    /* 0x14 */ f32 spawnAccum;
} BurstSpawner;

/* Timed effect driven by func_150E9E34 (a D_8008A4E8 tick callback). */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BurstSpawner spawner;
} RandomBurstEmitter;

void func_150E9890(Vec3f *pos, Vec3f *vel, f32 scale, u8 arg3, s32 arg4);
void func_150E9E34(RandomBurstEmitter *emitter);

/* game_1199D0.c */
/* Data of each effect the emitter spawns. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 w;
} EmittedPoint;

/* Data of the particle source effect (func_150EC6B0). */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 accumulator;
} ParticleSourceData;

/* Data of the fog flash effect (func_150ECB8C). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 r;
    /* 0x06 */ u8 g;
    /* 0x07 */ u8 b;
    /* 0x08 */ u8 a;
    /* 0x09 */ u8 rate;
} FogFlashData;

/* An effect (a TimedEffect with per-effect data at 0x28), as created by func_15149130. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ union {
        ShapeEmitter emitter;
        EmittedPoint point;
        ParticleSourceData source;
        FogFlashData fog;
    } data;
} EmitterEffect;

void func_150EC520(EmitterEffect *effect);
void func_150EC6B0(EmitterEffect *effect);
void func_150ECA68(Actor *obj, u8 r, u8 g, u8 b, u8 a, u8 rate, s16 duration, u8 owner, s32 kind);
void func_150ECB4C(EmitterEffect *effect, s32 arg1, u8 arg2);
void func_150ECB8C(EmitterEffect *effect);
void func_150ECC00(Actor *obj, u8 owner, s32 kind);
void func_150ECC70(Camera *obj);

/* game_11FF10.c */
/* State the emitter object keeps at +0x28: func_150F2C8C fills it, func_150F2D14 updates it each frame. */
typedef struct {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetId;   // target->unique_id when attached; a mismatch means the slot was reused
    /* 0x05 */ u8 prevUnk222; // target->unk222 seen last frame
    /* 0x08 */ f32 timer;     // one particle is emitted per 1.0 accumulated
    /* 0x0C */ s8 count;      // particles left in the current burst
} BoneBurstState;

/* The emitter object spawned by func_150F2C8C. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoneBurstState state;
} BoneBurstEmitter;

/* State each emitted particle keeps after its DefParticleHead (copied in by func_150F2D14, updated by func_150F3214). */
typedef struct {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetId;
    /* 0x08 */ f32 age;
    /* 0x0C */ f32 lifetime;
    /* 0x10 */ f32 growth;    // width = height = growth * sqrtf(age)
    /* 0x14 */ f32 fadeFrom;  // def.unk44 = fadeFrom - fadeRate * age * age
    /* 0x18 */ f32 fadeRate;  // fadeFrom / (lifetime * lifetime): def.unk44 reaches 0 at lifetime
    /* 0x1C */ Vec3f axisU; // quad axes for func_15140410
    /* 0x28 */ Vec3f axisV;
} ParticleState;

/* A particle spawned by func_1513D2F0. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ ParticleState state;
} BoneBurstParticle;

void func_150F2A60(s32 arg0);
void func_150F2C8C(Actor *target);
void func_150F2D14(BoneBurstEmitter *emitter);
void func_150F3194(BoneBurstEmitter *emitter, s32 arg1, u8 arg2);
void func_150F31D4(BoneBurstParticle *particle, s32 arg1, u8 arg2);
s32 func_150F3214(BoneBurstParticle *particle);
void func_150F337C(BoneBurstParticle *particle, s16 viewport);
void func_150F33B0(WorldObject *obj);
void func_150F33F8(s32 player);
extern ZoneNode *D_800D9A40; // zone at (0x1500, 0x700); func_150F33F8 sets its unk0 bits per player

/* game_120950.c */
f32 func_150F34A0(Actor *obj, f32 pitch);
void func_150F34F4(Actor *actor);
void func_150F43F0(Camera *actor);

/* game_121A20.c */
extern u8 D_80088B10[16]; /* the bone of the second gun's muzzle point (game_200930's D_8008FC94) */
/* Per-object state of the effect spawned by func_150F4570: it circles a centre that moves
 * from one point towards another (updated by func_150F48D0). */
typedef struct {
    /* 0x00 */ f32 timeLeft;
    /* 0x04 */ Vec3f center;
    /* 0x10 */ f32 phase; /* radians */
    /* 0x14 */ f32 phaseRate;
    /* 0x18 */ f32 cosDir;
    /* 0x1C */ f32 sinDir;
    /* 0x20 */ f32 radius;
    /* 0x24 */ u8 flags; /* SPIRAL_* */
    /* 0x25 */ u8 pad25[3];
} SpiralState;

/* An object spawned by func_1513264C with a SpiralState. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x10];
    /* 0x020 */ Vec3f rot; /* degrees */
    /* 0x02C */ u8 pad2C[0xC];
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ Vec3f spin;
    /* 0x05C */ u8 pad5C[0x14];
    /* 0x070 */ u8 alpha;
    /* 0x071 */ u8 unk71;
    /* 0x072 */ u8 pad72[0x16];
    /* 0x088 */ u32 sound; /* sound id in the low half, playing handle in the high half */
    /* 0x08C */ u8 pad8C[0xE4];
    /* 0x170 */ SpiralState spiral;
} SpiralParticle;

/* Per-object state of the emitter started by func_150F4D5C (updated by func_150F4DEC). */
typedef struct {
    /* 0x00 */ Camera *camera;
    /* 0x04 */ f32 pending; /* spirals owed; one is spawned per whole unit */
    /* 0x08 */ s8 condition; /* index into D_80088B38, or -1 for none */
    /* 0x09 */ u8 variants; /* index into D_80088B20 */
    /* 0x0A */ u8 padA[2];
} CameraEmitterData;

/* The emitter effect object created by func_15149130. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ CameraEmitterData data;
} CameraEmitter;

extern VariantList D_80088B20[2];

void func_150F4570(Vec3f *from, Vec3f *to, u8 id, f32 speed, f32 invSpeed, f32 scale, u8 fadeNear, s32 sound, u8 owner,
                   s32 kind);
s32 func_150F48D0(SpiralParticle *obj);
s32 func_150F4A38(SpiralParticle *obj);
void func_150F4CFC(SpiralParticle *obj, s32 arg1, u8 event);
void func_150F4D5C(Camera *camera, s8 condition, u8 variants, u8 owner, s32 kind);
void func_150F4DEC(CameraEmitter *emitter);
extern const s16 D_800A19F0[3];
extern const f32 D_800A19F8[3];
extern const s16 D_800A1A04[16];
extern const f32 D_800A1A24[16];

/* game_128D70.c */
/* State block that func_150FC438 copies into its spawned object at offset 0x28. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerUniqueId;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 spawnTimer;
    /* 0x0C */ u8 side;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ CamWobble *wobble; /* the other actor's camera wobble (func_15164780), or NULL */
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s8 unk16; // owner camera's unk23D, or -1
    /* 0x17 */ u8 pad17;
    /* 0x18 */ Actor *other;
    /* 0x1C */ u8 otherUniqueId;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 pad1E[2];
} TwoSidedEmitterState; // size 0x20

/* The object spawned by func_150FC438. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ TwoSidedEmitterState state;
} TwoSidedEmitter;

typedef struct {
    /* 0x000 */ u8 pad0[0x1A0];
    /* 0x1A0 */ Actor *owner;
    /* 0x1A4 */ u8 ownerUniqueId;
} Particle_150FC368;

void func_150FB8C0(Actor *obj, u8 side, f32 timeOffset, u8 skipTarget, u8 arg4, s32 arg5);
void func_150FC368(Particle_150FC368 *particle);
void func_150FC438(Actor *owner, Actor *other, u8 arg2, u8 arg3);
void func_150FC614(TwoSidedEmitter *obj);
void func_150FC818(TwoSidedEmitter *obj, ActorMsg *msg, u8 msgType);

/* game_12DAB0.c */
/* State of the effect, stored after its main particle. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;          /* owner->unique_id */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ Vec3f unk8;
    /* 0x14 */ Vec3f unk14;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ s8 ownerSlot;        /* owner's index in movement_obj */
    /* 0x22 */ u8 unk22;            /* owner->camera->CNumber, 0xFF without a camera */
    /* 0x23 */ u8 unk23;
    /* 0x24 */ u8 *particleA;       /* func_15130374 particle */
    /* 0x28 */ u8 *subParticle;     /* func_1513D4B8 particle */
    /* 0x2C */ LightEffect *flash;    /* func_151602C0 */
    /* 0x30 */ u8 *glow;            /* func_151407D0 glow */
    /* 0x34 */ u8 *particleB;       /* func_15130374 particle */
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ f32 unk5C;
    /* 0x60 */ f32 unk60;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ f32 unk68;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 unk70;
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ f32 unk80;
    /* 0x84 */ f32 unk84;
    /* 0x88 */ f32 unk88;
    /* 0x8C */ f32 unk8C;
    /* 0x90 */ s16 sound;           /* func_10010F30 handle */
    /* 0x92 */ u8 pad92[2];
} EffectParticleState; /* size 0x94 */

/* The main particle: a func_1513D4B8 particle (0x110 bytes) followed by its EffectParticleState. */
typedef struct EffectParticle {
    DefParticleHead head;
    EffectParticleState state;
} EffectParticle;

extern const Vec3f D_800A21A0[7];
extern const Vec3f D_800A21F4[7];
extern const u8 D_800A2248[7];
EffectParticle *func_15100600(Actor *owner, const Vec3f *vec1, const Vec3f *vec2, u8 arg3, s16 lifetime, u8 r, u8 g, u8 b, f32 arg8,
                              f32 arg9, f32 argA, f32 argB, u8 argC, u8 argD, s32 argE);

/* game_135780.c */
/*
 * Per-particle data of the particles func_151082D0 spawns (update function 0x18, func_15108658).
 * The animation frame steps 0 -> 1 -> 2 at the start of the particle's life and back at the end.
 */
typedef struct {
    /* 0x00 */ f32 life;       /* remaining; the particle dies below 0 */
    /* 0x04 */ f32 lifetime;   /* initial life */
    /* 0x08 */ f32 frame1Time; /* time spent below frame 2 at either end */
    /* 0x0C */ f32 frame0Time; /* time spent at frame 0 at either end */
    /* 0x10 */ f32 flashTimer; /* counts down from 11 while flashing, 0 otherwise */
} FlashParticleData;

/* Particle created by func_15130280 (only the fields used here, see game_15D730.c). */
typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ FlashParticleData data;
} AreaFlashParticle;

/* Points the emitter spawns particles around: a centre, a radius and a height range. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
} EmitterArea;

/* Per-effect data of the func_151082D0 emitter. */
typedef struct {
    /* 0x00 */ f32 spawnAccum;
    /* 0x04 */ EmitterArea *area;
    /* 0x08 */ u8 flags;
} AreaEmitterData;

/* Timed effect of func_151082D0. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ AreaEmitterData data;
} AreaEmitter;

void func_151082D0(AreaEmitter *emitter);
s32 func_15108658(AreaFlashParticle *particle, s32 arg1);
void func_151087FC(AreaEmitter *emitter, s32 arg1, u8 event);

/* game_1368C0.c */
/* Data of effect 0x1A (func_15109410 / func_151094FC). */
typedef struct {
    /* 0x00 */ Actor *object;
    /* 0x04 */ u8 objectId; /* object->unique_id when attached; a mismatch means the slot was reused */
    /* 0x08 */ f32 spawnAccum;
    /* 0x0C */ f32 spawnRate;
    /* 0x10 */ s8 tickFunc; /* index into D_80088C60, -1: none */
    /* 0x11 */ s8 emitFunc; /* index into D_80088C64, -1: none */
} ObjectEmitterData; /* size 0x14 */

/* Data of effect 0x1B (func_15109ED4 / func_15109FB8). */
typedef struct {
    /* 0x00 */ void *unk0; /* passed to func_1511490C to build the emitter's matrix */
    /* 0x04 */ f32 spawnAccum;
    /* 0x08 */ f32 spawnRate;
    /* 0x0C */ s8 tickFunc; /* index into D_80088C60, -1: none */
    /* 0x0D */ s8 emitFunc; /* index into D_80088C64, -1: none */
} MatrixEmitterData; /* size 0x10 */

/* Data of effect 0x1C (func_1510A344 / func_1510A40C). */
typedef struct {
    /* 0x00 */ Actor *object;
    /* 0x04 */ u8 objectId;
    /* 0x08 */ f32 spawnAccum;
    /* 0x0C */ f32 spawnRate;
    /* 0x10 */ f32 spawnRateRandom; /* added to spawnRate scaled by a random [0, 1) */
} PairEmitterData; /* size 0x14 */

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ union {
        ActorRef tracked;
        ObjectEmitterData object;
        MatrixEmitterData matrix;
        PairEmitterData pair;
    } data;
} Effect1368C0;

TimedEffect *func_15109410(Actor *object, s16 duration, s8 tickFunc, s8 emitFunc, f32 spawnRate, s32 extraSize,
                           u8 owner, s32 kind);
void func_151094FC(Effect1368C0 *effect);
void func_15109848(Effect1368C0 *effect, Vec3f *point, Vec3f *start, Vec3f *end, Vec3f *tip, Vec3f *dir);
void func_15109C20(Effect1368C0 *effect, Vec3f *point, Vec3f *start, Vec3f *end, Vec3f *tip, Vec3f *dir);
TimedEffect *func_15109ED4(void *arg0, s16 duration, s8 tickFunc, s8 emitFunc, f32 spawnRate, s32 extraSize, u8 owner,
                           s32 kind);
void func_15109FB8(Effect1368C0 *effect);
TimedEffect *func_1510A344(Actor *object, s16 duration, u8 owner, s32 kind);
void func_1510A40C(Effect1368C0 *effect);
void func_1510A870(Effect1368C0 *effect, ActorSwapMsg *msg, u8 msgType);
void func_1510A8CC(Effect1368C0 *effect, ActorSwapMsg *msg, u8 msgType);

/* game_15D730.c */
extern Gfx D_80089700[]; /* render setup (object type table D_8008B4A8) */
extern Gfx D_80089730[]; /* render teardown (object type table D_8008B4A8) */
/* Each returns a SpriteParticle followed by dataSize bytes of its kind's data (the kind's own type). */
void *func_15130280(ParticleDesc *desc, u8 kind, CollisionHit *ground, s32 dataSize, u8 arg4, s32 arg5);
void *func_15130374(ParticleDesc *desc, u8 kind, s32 dataSize, u8 arg3, s32 arg4);
void *func_151303BC(ParticleDesc *desc, u8 kind, s32 dataSize);
void func_151303EC(SpriteParticle *p);
Gfx *func_15130A9C(Gfx *gfx, SpriteParticle *p, s16 viewIdx);
void func_1513137C(s16 *r, s16 *g, s16 *b, s16 *a, SpriteParticle *p);
void func_15131514(s16 *r, s16 *g, s16 *b, s16 *a, SpriteParticle *p);
void func_1513164C(s16 *primR, s16 *primG, s16 *primB, s16 *primA, s16 *envR, s16 *envG, s16 *envB, s16 *envA,
                   SpriteParticle *p);
s32 func_151316AC(SpriteParticle *p, Vec3f *prevPos);
s32 func_151316DC(SpriteParticle *p, Vec3f *prevPos);
s32 func_1513170C(SpriteParticle *p, Vec3f *prevPos);
void func_1513173C(DescParticleHead *p);
void func_1513175C(DescParticleHead *p);
void func_1513177C(SpriteParticle *p);
s32 func_151317C8(SpriteParticle *p);
s32 func_15131814(s32 arg0, s32 arg1);
void func_15131828(DescParticleHead *head, f32 *amplitude, u8 *phase, u8 *phaseStep);
void func_151318E8(Vec3f *vel, f32 drag);
void func_15131918(Vec3f *vel, f32 drag);
/* void func_15131958(Vec3f *vel, f32 drag) is declared in the files that call it: game_105FC0.c and
   game_200930.c pass a third argument. */
s32 func_151319A4(SpriteParticle *p, Vec3f *prevPos);
s32 func_151319C4(SpriteParticle *p, Vec3f *prevPos, ParticleBounce *bounce);
s32 func_15131AFC(SpriteParticle *p, Vec3f *prevPos);
s32 func_15131B3C(SpriteParticle *p, Vec3f *prevPos);
s32 func_15131B7C(SpriteParticle *p, Vec3f *prevPos, f32 *hit, ParticleBounce *bounce);
void func_15131C2C(SpriteParticle *p, s32 arg1, u8 arg2);
void func_15131C84(u8 *phase, s8 *phaseStep, f32 base, f32 *amplitude, f32 *outX, f32 *outY);
void func_15131D4C(s32 arg0, u8 arg1);
s32 func_15131D9C(SpriteParticle *p, Vec3f *prevPos);
s32 func_15131DEC(SpriteParticle *p, Vec3f *prevPos);
SpriteParticle *func_15131EE4(Vec3f *pos, f32 size, u8 arg2, s32 arg3);

/* game_15F680.c */
extern ParticleNode *D_800DC460;
extern ParticleNode *D_800DC464;
extern s16 D_800DC468[233];
extern s32 D_800DC63C;
void func_151321D0(ModelParticle *particle);
void func_151323AC(ModelParticle *particle);
void func_151323F8(ModelParticle *particle);
void func_15132444(ModelParticle *particle);
void func_15132570(ModelParticle *particle);
void func_1513259C(ModelParticle *particle);
void func_151325C8(ModelParticle *particle);
/* Both return a ModelParticle followed by extraSize bytes of its kind's data (the kind's own type). */
void *func_1513264C(ParticleSpawn *spawn, s32 arg1, s32 arg2, CollisionHit *floor, s32 extraSize, u8 arg5,
                    s32 arg6);
void *func_15132A4C(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 extraSize, u8 arg4, s32 arg5);
void func_15132A88(ModelParticle *particle);
Gfx *func_15132B80(Gfx *gfx, ModelParticle *arg1, s16 arg2);
s32 func_15132DDC(ModelParticle *particle);
s32 func_151332DC(ModelParticle *particle);
s32 func_15133510(Mtx *mtx, ModelParticle *particle);
void func_15133588(ModelParticle *particle, void *data, u8 event);
s32 func_151336A8(s32 model, ParticleNode *node, ModelParticle *particle);
s32 func_15133760(Mtx *mtx, ModelParticle *particle);
s32 func_151337C0(ModelParticle *particle);
s32 func_15133894(ModelParticle *particle);
s32 func_151339D4(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
s32 func_15133A50(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
s32 func_15133A94(Mtx *mtx, ModelParticle *particle);
s32 func_15133B98(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);
s32 func_15133C58(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);
s32 func_15133D20(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);
void func_15133DE8(ModelParticle *particle, ActorRef *data, u8 event);
void func_15133E3C(s32 arg0, u8 arg1);
Gfx *func_15133E84(Gfx *gfx, ModelParticle *particle, s16 arg2);
Gfx *func_15133EB8(Gfx *gfx, ModelParticle *particle, s16 arg2);
Gfx *func_15133EEC(Gfx *gfx, u16 index, u8 segment, s32 arg3);
Gfx *func_15133FD8(Gfx *gfx, ModelParticle *particle, s16 arg2);

/* game_169510.c */
extern Gfx D_80089EB8[]; /* render setup (object type table D_8008B4A8) */
extern Gfx D_80089ED8[]; /* render teardown (object type table D_8008B4A8) */
/* Source of the four vertex colours given to func_151400D0. */
typedef struct {
    s16 r, g, b, a;
    u16 flag;
} VtxColor;

void func_1513C060(QuadSprite *sprite);
QuadSprite *func_1513C350(void *def, const void *colors, u8 updateFn, u8 expireFn, u8 flip, s32 model, s32 arg6, Vec3f *pos,
                          s32 extraSize, u8 arg9, s32 argA);
QuadSprite *func_1513C4EC(void *def, const void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width, f32 length,
                          u8 angle, u8 flip, s32 model, s32 arg6, s32 extraSize, u8 argE, s32 argF);
QuadSprite *func_1513C5B0(void *def, const void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width, f32 length,
                          u8 angle, u8 flip, s32 extraSize, u8 argC, s32 argD);
QuadSprite *func_1513C650(void *def, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                          f32 height, u8 angle, u8 flip, s32 model, s32 arg6, s32 extraSize, u8 argE, s32 argF);
QuadSprite *func_1513C73C(void *def, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                          f32 height, u8 angle, u8 flip, s32 extraSize, u8 argC, s32 argD);
QuadSprite *func_1513C804(void *def, s32 arg1, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z,
                          f32 width, f32 height, u8 angle, u8 flip, s32 extraSize, u8 argD, s32 argE);
void func_1513C8D4(QuadSprite *sprite);
void func_1513C900(QuadSprite *sprite);
void func_1513C92C(QuadSprite *sprite);
void func_1513C9B0(QuadSprite *sprite);
void func_1513C9FC(QuadSprite *sprite);
void func_1513CA48(DefParticleHead *particle);
void func_1513CA6C(DefParticleHead *particle);
void func_1513CAA0(DefParticleHead *particle);
void func_1513CAD4(DefParticleHead *arg0);
void func_1513CB58(DefParticleHead *particle);
void func_1513CBA4(DefParticleHead *particle);
Gfx *func_1513CBF0(Gfx *gfx, QuadSprite *sprite, s16 viewport);
void func_1513CF9C(s32 arg0, s32 arg1, u8 arg2);
void func_1513CFD0(s32 arg0, s32 arg1, u8 arg2);
void func_1513CFE4(DefParticleHead *particle);
void *func_1513D2F0(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 model,
                    s32 arg8, s32 motionSize, u8 argA, s32 argB);
void *func_1513D4B8(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip,
                    s32 motionSize, u8 arg8, s32 arg9);
void *func_1513D524(void *def, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 motionSize, u8 arg7,
                    s32 arg8);
void *func_1513D594(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width,
                    f32 height, Vec3s **triPtrs, CollisionTri *surface, s32 model, s32 argC, u8 argD, s32 motionSize,
                    u8 argF, s32 arg10);
void *func_1513D668(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width,
                    f32 height, Vec3s **triPtrs, CollisionTri *surface, u8 argB, s32 motionSize, u8 argD, s32 argE);
s32 func_1513D6FC(ParticleAxes *axes, s16 angle, f32 width, f32 height, Vec3s **triPtrs, CollisionTri *surface,
                   u8 onSlope);
Gfx *func_1513DB00(Gfx *gfx, DefParticleHead *particle, s16 viewport);
void func_1513DF9C(DefParticleHead *particle, s32 arg1, u8 arg2);
void func_1513E070(DefParticleHead *particle, s32 arg1, u8 arg2);
void func_1513E084(DefParticleHead *particle, ActorMsg *msg, u8 eventId);
void func_1513E134(void);
void func_1513E13C(QuadSprite *sprite, f32 x, f32 y, f32 z, f32 width, f32 length, u8 angle);
void func_1513E2A4(void);
s32 func_1513E2AC(QuadSprite *sprite, Vec3s **triPtrs, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                  f32 height, u8 angle);
s32 func_1513E6E8(DefParticleHead *particle);
Vtx *func_1513E83C(DefParticleHead *particle, s16 viewport);
Vtx *func_1513EAD8(DefParticleHead *particle, s32 reversed, s32 viewport);
Vtx *func_1513EDB4(void *particle, s16 viewport);
Vtx *func_1513EDE4(void *particle, s16 viewport);
Vtx *func_1513EE14(DefParticleHead *particle, s16 viewport);
Vtx *func_1513F114(DefParticleHead *particle, s16 viewport);
void func_1513F4B0(DefParticleHead *head, s16 viewport);
Gfx *func_1513F4E4(Gfx *gfx, u8 combiner, u8 *needSync);
void func_1513F680(DefParticleHead *particle, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn);
void func_1513F6C0(QuadSprite *sprite, u8 updateFn, u8 expireFn);
s32 func_1513F6E8(DefParticleHead *particle);
s32 func_1513F728(DefParticleHead *particle);
void func_1513FA2C(DefParticleHead *particle, s16 viewport);
void func_1513FA70(DefParticleHead *particle, s16 viewport);
Vtx *func_1513FAB4(DefParticleHead *particle, u8 reversed, BeamScale *scale, s16 viewport);
void func_1513FFF4(Vtx *vtx, u8 texture, u8 flip);
void func_151400D0(Vtx *vtx, const VtxColor *colors);
Vtx *func_15140190(DefParticleHead *particle, s16 viewport);
void func_151403A8(s32 arg0, u8 arg1);
void func_151403DC(s32 arg0, u8 arg1);
Vtx *func_15140410(DefParticleHead *particle, Vec3f *axisU, Vec3f *axisV, s16 viewport);
s32 func_151406AC(s32 arg0, s32 list, s32 group, s16 phase);
extern const VtxColor D_800A4AA0[4];
extern const s32 D_800A5168[4];

/* game_176A00.c */
ChannelParticle *func_15149550(ParticleEmitterDesc *emitter, u8 arg1, u8 arg2, u8 kind, u8 arg4, s32 arg5);
s32 func_15149838(ChannelParticle *particle);
s32 func_15149A94(ChannelParticle *particle);
s32 func_15149BF4(ChannelParticle *particle);
s32 func_15149C58(ChannelParticle *particle);
Vtx *func_15149D18(ChannelParticle *particle, s16 index);
s32 func_15149EC4(ChannelParticle *particle);

/* game_177480.c */
/* Per-effect behaviour parameters, at 0x110 in FlickerParticle. */
typedef struct {
    /* 0x00 */ f32 widthMax;
    /* 0x04 */ f32 widthMin;
    /* 0x08 */ f32 widthTarget;
    /* 0x0C */ f32 heightMid;
    /* 0x10 */ f32 heightMin;
    /* 0x14 */ f32 heightMax;
    /* 0x18 */ f32 heightTarget;
    /* 0x1C */ s16 widthTimer;
    /* 0x1E */ s16 heightTimer;
    /* 0x20 */ Vec3f vel;
    /* 0x2C */ f32 gravity;
    /* 0x30 */ f32 damping;
    /* 0x34 */ f32 shrinkRate;
    /* 0x38 */ f32 unk24Max;
    /* 0x3C */ f32 unk24Min;
    /* 0x40 */ f32 unk24Target;
    /* 0x44 */ s16 unk24Timer;
    /* 0x46 */ s16 unk156;
    /* 0x48 */ s16 unk158;
} FlickerParams;

/* Effect instance shared by the update callbacks in this file. */
typedef struct {
    /* 0x00 */ DefParticleHead head; /* head.def.rot: the previous position, head.def.scale.z: trail factor,
                                      * head.unk74: flags */
    /* 0x110 */ FlickerParams params;
} FlickerParticle;

Vtx *func_15149FD0(FlickerParticle *effect, s16 index);
s32 func_1514A19C(FlickerParticle *effect);
s32 func_1514A380(FlickerParticle *effect);
s32 func_1514A498(FlickerParticle *effect);
s32 func_1514A4EC(FlickerParticle *effect);
s32 func_1514A594(FlickerParticle *effect);

/* game_177B50.c */
typedef struct Beam1514A6A0 {
    u8 unknown0[0x2C];
    f32 width;
    f32 progress;
    Vec3f start;
    Vec3f end;
    f32 radius;
    u8 unknown50[0x70];
    u8 vertexTemplate[0x40];
    u8 *buffers[1];
} Beam1514A6A0;

/* Common particle instance layout used by the update callbacks below. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ u8 pad110[0x40];
    /* 0x150 */ f32 unk150;
} Particle177B50;

typedef struct {
    f32 unk0;
    f32 unk4;
} Struct1514B8E4Size;

void *func_1514A6A0(Beam1514A6A0 *beam, s16 cam);
void func_1514AB5C(f32 x, f32 y, f32 z, f32 radius, s16 count, f32 sizeBase, f32 sizeRange, f32 heightBase,
                   f32 heightRange, s16 lifeBase, s16 lifeRange, u8 arg11);
void func_1514AD9C(Vec3f *pos, u8 arg1, s32 arg2);
s32 func_1514AF74(Particle177B50 *p);
void func_1514B034(Vec3f *pos, u8 arg1, s32 arg2);
void func_1514B364(Vec3f *position, CollisionHit *ground, u8 arg2, s32 arg3);
s32 func_1514B844(Particle177B50 *p);
s32 func_1514B87C(Particle177B50 *p);
s32 func_1514B8B0(Particle177B50 *p);
s32 func_1514BC08(JitterParticle *p, ParticleJitter *w);

/* game_1792B0.c */
void func_1514BE00(JitterParticle *p);

/* game_1792D0.c */
void func_1514BE20(void *arg);

/* game_179400.c */
void func_1514BF50(JitterParticle *p);
void func_1514BF7C(JitterParticle *p);
void func_1514BF9C(JitterParticle *p);
s32 func_1514C258(JitterParticle *p);
s32 func_1514C288(JitterParticle *p);
s32 func_1514C2B8(JitterParticle *p);

/* game_1797A0.c */
void func_1514C2F0(f32 x, f32 y, f32 z, f32 radius, u8 startAngle, s8 angleStep, s16 count, u8 kind, s32 arg8, f32 arg9,
                   void *data, u8 arg11);
void func_1514C470(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 count, u8 kind, s32 arg8, f32 arg9, void *data,
                   u8 arg11);
void func_1514C678(f32 x, f32 y, f32 z, f32 radius, s16 angleA, s16 angleB, s16 count, u8 kind, s32 arg8, f32 arg9,
                   void *data, u8 arg11);
void func_1514C858(f32 x, f32 y, f32 z, f32 radius, s16 yaw, s16 angleA, s16 angleB, s16 count, u8 kind, s32 arg9,
                   f32 arg10, void *data, u8 arg12);

/* game_17CAF0.c */
/* Copied into the object spawned by func_151541B8 at +0x170; advanced each frame by func_1515452C. */
typedef struct {
    f32 elapsed;
    f32 duration;
    f32 growth; /* size = growth * sqrt(elapsed) */
    f32 alpha;  /* alpha - fade * elapsed^2 */
    f32 fade;
    u8 growOneAxis; /* grow only the first size field */
} Extra1515452C;

/* Copied into the object spawned by func_15154884 at +0x170; advanced each frame by func_15154A88. */
typedef struct {
    f32 remaining;
    f32 size;
    f32 alpha;
    f32 phase; /* size and alpha are scaled by sinf(phase) */
    f32 phaseRate;
} Extra15154A88;

/* The fields of a func_15132A4C object that the update functions below use. */
typedef struct {
    u8 pad0[0x18];
    f32 unk18;
    f32 unk1C;
    Vec3f position;
    u8 pad2C[0x24];
    Vec3f velocity;
    u8 pad5C[0x14];
    u8 alpha;
    u8 pad71[0xFF];
    union {
        Extra1515452C grow;
        Extra15154A88 pulse;
    } extra;
} Object15132A4C;

void func_1514F640(TriangleSource *source, ConeBasis *cone);
s32 func_1514F6E8(ConeBasis *cone);
void func_1514F808(ConeBasis *cone, f32 speed, Vec3f *out);
void func_1514F8F8(JetSpray *config, Vec3f *direction, Vec3f *axis0, Vec3f *axis1, f32 spread, u8 arg5, s32 arg6);
void func_1514FB98(DirectedJetSpray *emitter, u8 arg1, s32 arg2);
void func_1514FBFC(SurfaceJetSpray *emitter, u8 arg1, s32 arg2);
void func_1514FCE8(JetSpraySpec *config, u8 arg1, s32 arg2);
void func_1514FEFC(TriangleSource *source, DropSpray *config, CollisionHit *ground, u8 arg3, s32 arg4);
void func_1514FF44(ConeBasis *cone, DropSpray *config, CollisionHit *ground, u8 arg3, s32 arg4);
void func_15150178(AngleRange *angles, DropSpray *config, CollisionHit *ground, u8 arg3, s32 arg4);
void func_15150400(DebrisScatter *config, AngleRange *angles, u8 arg2, s32 arg3);
void func_1515080C(DebrisSpray *config, const s32 *variantIds, const f32 *variantScales, s32 variantCount,
                   void *extraData, s32 extraSize, u8 arg6, u8 arg7, s8 arg8, CollisionHit *floor, f32 radius, u8 argB,
                   u8 argC, u8 argD, s32 argE);
void func_15150D1C(DecalSpray *config, u8 arg1, s32 arg2);
void func_15150F90(PieceSpray *config, u8 arg1, u8 arg2, s32 arg3);
void func_151511FC(SurfaceBurst *config, u8 arg1, s32 arg2);
void func_15151670(SurfacePuffSpray *config, u8 arg1, s32 arg2);
void func_15151A38(PuffSpray *config, u8 arg1, s32 arg2);
void func_15151D6C(SurfaceDebrisBurst *config, s32 *variantIds, f32 *variantScales, u32 variantCount, u8 arg4,
                   s32 arg5);
void func_15152190(DebrisBurst *config, const s32 *variantIds, const f32 *variantScales, u32 variantCount, f32 radius, u8 arg5,
                   u8 arg6, s32 arg7);
void func_15152520(ModelBurst *config, u8 arg1, s32 arg2);
void func_15152874(DriftBurst *config, u8 arg1, s32 arg2);
void func_15152ABC(Color8 *out);
/* (TrailBurst *config, u8 arg1, s32 arg2); declared without a prototype: game_11C2B0.c passes a fourth argument */
void func_15152B38();
void func_15152F70(SimplePuffSpray *config, u8 arg1);
void func_15153298(BillboardBurst *config, u8 arg1, s32 arg2);
void func_15153634(SpriteBurst *config, u8 arg1, u8 arg2, s32 arg3);
void func_151539B4(StreakBurst *config, u8 arg1);
void func_15153C84(TriangleSource *source, SplashSpray *config, CollisionHit *ground, u8 arg3, s32 arg4);
void func_15153CCC(ConeBasis *cone, SplashSpray *config, CollisionHit *ground, u8 arg3, s32 arg4);
void func_15153F18(AngleRange *angles, SplashSpray *config, CollisionHit *ground, u8 arg3, s32 arg4);
void *func_151541B8(const Vec3f *position, f32 duration, f32 growth, f32 alpha, f32 arg4, u8 arg5, s32 arg6);
s32 func_1515452C(Object15132A4C *object);
void func_15154684(ParticleBurst *config, u8 arg1, s32 arg2);
void *func_15154884(Vec3f *position, f32 duration, f32 size, f32 alpha, u8 arg4, s32 arg5);
s32 func_15154A88(Object15132A4C *object);

/* game_183640.c */
extern BurstPoint D_800DCA30[3][10]; /* random burst points around each pipe (func_150124A0 fills them) */
extern Gfx D_8008AD40[]; /* render setup (object type table D_8008B4A8) */
/*
 * A streak particle: a camera-facing triangle from its tail point to a
 * width-wide edge at its head. Head and tail move with separate velocities.
 */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Vec3f headPos;
    /* 0x1C */ Vec3f headVel;
    /* 0x28 */ Vec3f tailPos;
    /* 0x34 */ Vec3f tailVel;
    /* 0x40 */ u8 flags;            // 1: lifetime counts down, 2: cull front, 4: cull back, 8: fades out
    /* 0x42 */ s16 lifetime;
    /* 0x44 */ u16 renderMode;      // low byte: D_800A4AC8 render mode, high byte: func_1513F4E4 combiner
    /* 0x48 */ f32 width;
    /* 0x4C */ u8 alpha;
    /* 0x4E */ s16 fadeStart;       // fade begins when lifetime drops below this
    /* 0x50 */ s16 fadeRate;        // alpha = lifetime * fadeRate while fading
    /* 0x52 */ u8 pad52[0x6];
    /* 0x58 */ Vtx vtxTemplate[3];  // tail, head + side, head - side
    /* 0x88 */ Vtx *vtxBuffers[4];  // per-view vertex buffers (two frames each)
} StreakParticle;

/* A streak burst plus one particle at the same point (func_151568F8). */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 countBase;
    /* 0x0E */ s16 countRange;
    /* 0x10 */ s16 yawBase;
    /* 0x12 */ s16 pitchBase;
    /* 0x14 */ s16 yawRange;
    /* 0x16 */ s16 pitchRange;
    /* 0x18 */ f32 headSpeedBase;
    /* 0x1C */ f32 headSpeedRange;
    /* 0x20 */ f32 tailSpeedBase;
    /* 0x24 */ f32 tailSpeedRange;
    /* 0x28 */ Color8 headColor;
    /* 0x2C */ Color8 tailColor;
    /* 0x30 */ u8 flags;
    /* 0x31 */ u8 pad31;
    /* 0x32 */ s16 lifetimeBase;
    /* 0x34 */ s16 lifetimeRange;
    /* 0x36 */ u16 renderMode;
    /* 0x38 */ f32 widthBase;
    /* 0x3C */ f32 widthRange;
    /* 0x40 */ u8 alphaBase;
    /* 0x41 */ u8 alphaRange;
    /* 0x42 */ s16 fadeStart;
    /* 0x44 */ s16 fadeRate;
    /* 0x46 */ u8 kind;
    /* 0x47 */ u8 particleTexture;
    /* 0x48 */ u16 particleRenderMode;
    /* 0x4A */ u8 pad4A[2];
    /* 0x4C */ s32 particleUnk0;
    /* 0x50 */ s32 particleUnk4;
    /* 0x54 */ s16 particleTimer;
    /* 0x56 */ u8 pad56[2];
    /* 0x58 */ s32 particleFrame;
    /* 0x5C */ s32 particleFrameRate;
    /* 0x60 */ Color8 particleColor;
    /* 0x64 */ Color8 particleColor2;
    /* 0x68 */ u8 particleBrightness;
    /* 0x69 */ u8 pad69[3];
    /* 0x6C */ ParticleSize particleSize;
    /* 0x74 */ s16 particleFadeStart;
    /* 0x76 */ s16 particleFadeRate;
    /* 0x78 */ s16 particleScaleStart;
    /* 0x7A */ u8 pad7A[2];
    /* 0x7C */ f32 particleScaleRate;
    /* 0x80 */ s32 particleFlags;
    /* 0x84 */ u8 particlePrimColorMode;
    /* 0x85 */ u8 particleEnvColorMode;
    /* 0x86 */ s8 particleUpdateFunc;
    /* 0x87 */ s8 particleDeathFunc;
    /* 0x88 */ u8 particleKind;
    /* 0x89 */ u8 unk89;
    /* 0x8A */ u8 unk8A;
    /* 0x8B */ u8 unk8B;
    /* 0x8C */ u8 unk8C;
} StreakBurstEffectDesc;

/* A countdown restarted at base + rand() % (range + 1). */
typedef struct {
    /* 0x00 */ s16 base;
    /* 0x02 */ s16 range;
    /* 0x04 */ s16 value;
} RandomTimer;

/* An effect that sets off bursts at random points of D_800DCA30 at random intervals. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ RandomTimer timer;
} RandomBurstEffect;

StreakParticle *func_15156190(StreakParticleDesc *desc, u8 kind, s32 extraSize, u8 owner, s32 arg4);
void func_15156388(StreakParticleDesc *desc, u8 kind, s32 extraSize);
void func_151563B8(StreakParticle *arg0);
Gfx *func_151564F8(Gfx *gfx, StreakParticle *particle, s16 viewIndex);
void func_151568F8(StreakBurstEffectDesc *desc, u8 owner);
void func_15156B54(RandomBurstEffect *effect);
void func_15156D24(f32 *pos, u8 owner);
void func_15156F94(StreakParticle *particle);
void func_15156FB8(StreakParticle *particle);
void func_15156FE4(StreakParticle *particle);

/* game_188440.c */
/* Emitter state stored in an effect object at +0x28. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 rate;        // particles per unit of D_800BE9A4
    /* 0x10 */ f32 accumulator; // pending particles; one is spawned per whole unit
    /* 0x14 */ f32 unk14;
} RiseEmitterState;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ RiseEmitterState emitter;
} EmitterObject;

/* Per-sprite data func_1515B674 appends to its sprites (at ScreenSprite +0x70). */
typedef struct {
    /* 0x00 */ s16 player;
    /* 0x04 */ f32 acceleration;
    /* 0x08 */ f32 velocity;
    /* 0x0C */ f32 baseHalfHeight;
    /* 0x10 */ f32 stretch;
    /* 0x14 */ f32 unk14;
} SlidingSpriteData;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ ScreenSpriteInfo info;
    /* 0x68 */ u8 pad68[8];
    /* 0x70 */ SlidingSpriteData data;
} SlidingSprite;

void func_1515AF90(s16 player);
void func_1515B21C(EmitterObject *obj);
void func_1515B5F4(s16 player);
void func_1515B62C(SlidingSprite *sprite, s16 *payload, u8 type);
void func_1515B674(s16 player);
s32 func_1515B994(SlidingSprite *sprite);
void func_1515BA10(s16 player);
void func_1515BA1C(s16 player);
void func_1515BA48(s16 player);
void func_1515BA54(s16 player);
void func_1515BA80(s16 player);
void func_1515BAAC(s16 player);

/* game_193E50.c */
extern Gfx D_8008B440[]; /* object type 0xD render setup (object type table D_8008B4A8) */
/* Object type 0xD: a puff of three quads, each on its own matrix with a different yaw. */
typedef struct {
    EffectHeader header;
    Mtx mtx[3];
    u8 timer; // counts down from 10; particles spawn at 5, freed at 0
    u8 padD1;
    s16 x;
    s16 y;
    s16 z;
    f32 scale;
} PuffQuadEffect;

void func_151669A0(s32 x, s32 y, s32 z, f32 scale, u8 arg4, s32 arg5);
void func_15166B50(void *self);
Gfx *func_15166D68(Gfx *gfx, PuffQuadEffect *obj, s32 arg2);
Gfx *func_15166F6C(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);
Gfx *func_15166FD8(Gfx *gfx, s32 arg1, s32 arg2);

/* game_197120.c */
/* Partial view of a type 0x10 ModelFx object (game_1944C0.c): EffectHeader, matrices, then its data. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x80];
    /* 0x90 */ f32 x;
    /* 0x94 */ f32 y;
    /* 0x98 */ f32 z;
    /* 0x9C */ f32 rotX;
    /* 0xA0 */ f32 rotY;
    /* 0xA4 */ f32 rotZ;
    /* 0xA8 */ f32 velX;
    /* 0xAC */ f32 velY;
    /* 0xB0 */ f32 velZ;
    /* 0xB4 */ f32 spinX;
    /* 0xB8 */ f32 spinY;
    /* 0xBC */ f32 spinZ;
    /* 0xC0 */ u8 padC0[4];
    /* 0xC4 */ f32 scale;
    /* 0xC8 */ u8 padC8[4];
    /* 0xCC */ s32 timer;
    /* 0xD0 */ u8 padD0[4];
    /* 0xD4 */ f32 baseScale;
    /* 0xD8 */ f32 friction;
    /* 0xDC */ f32 bounce;
    /* 0xE0 */ f32 spinDamp;
    /* 0xE4 */ u8 padE4;
    /* 0xE5 */ u8 alpha;
    /* 0xE6 */ u8 padE6;
    /* 0xE7 */ u8 phase;
    /* 0xE8 */ u8 flags;
    /* 0xE9 */ u8 floorOffset;
    /* 0xEA */ u8 sound;
    /* 0xEB */ u8 kind;
    /* 0xEC */ u8 state;
} Debris;

/* Partial view of a type 5 billboard Particle (game_1944C0.c) as its falling-drop callback uses it. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 list;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[7];
    /* 0x14 */ s32 floorY;      /* floor height last found under the drop */
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ s16 x;
    /* 0x22 */ s16 y;
    /* 0x24 */ s16 z;
    /* 0x26 */ s16 velX;
    /* 0x28 */ s16 velZ;
    /* 0x2A */ s16 unk2A;       /* added to y for the floor test */
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ s16 life;
    /* 0x3A */ u8 pad3A;
    /* 0x3B */ u8 unk3B;
} DropParticle;

/* Partial view of a type 3 SpriteFx (game_1944C0.c); unk25 is SpriteFxTemplate.unk15. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 list;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[7];
    /* 0x14 */ s16 frame;       /* 8.8 fixed point */
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 z;
    /* 0x1E */ u8 pad1E[7];
    /* 0x25 */ u8 unk25;
} SplashFx;

void func_15169C70(Debris *debris);
void func_1516A3F4(DropParticle *drop);
void func_1516A538(SplashFx *fx);
struct StyleParticle; /* game_19A8B0.c */
s32 func_1516A648(struct StyleParticle *particle);

/* game_19A8B0.c */
extern Gfx D_8008CB70[]; /* render setup (object type table D_8008B4A8) */
extern Gfx D_8008CB90[]; /* render teardown (object type table D_8008B4A8) */
extern u8 D_800DD2A0[4]; /* per style object: intensity, raised to 0xFF over time (func_1516D400) */
/* A 0x34-byte sprite particle, spawned by func_1516D4E8 and updated every frame by the
 * handler that D_8008CBA0 holds for its kind. The bytes from 0x26 on depend on the kind. */
typedef struct StyleParticle {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 x;
    /* 0x10 */ s16 y;
    /* 0x12 */ s16 z;
    /* 0x14 */ s16 scaleX;
    /* 0x16 */ s16 scaleY;
    /* 0x18 */ s16 velocityY;
    /* 0x1A */ u8 texture; /* index into D_8008CA4C */
    /* 0x1B */ u8 frame;
    /* 0x1C */ u8 r;
    /* 0x1D */ u8 g;
    /* 0x1E */ u8 b;
    /* 0x1F */ u8 alpha;
    /* 0x20 */ u8 envR;
    /* 0x21 */ u8 envG;
    /* 0x22 */ u8 envB;
    /* 0x23 */ u8 kind; /* index into D_8008CBA0 */
    /* 0x24 */ u8 lifetime; /* fades in while nonzero, out once it runs down */
    /* 0x25 */ u8 yFraction;
    union {
        /* kinds that drift sideways with func_1516F864 */
        struct {
            /* 0x26 */ s8 velocityXHigh;
            /* 0x27 */ u8 velocityXLow;
            /* 0x28 */ s8 velocityZHigh;
            /* 0x29 */ u8 velocityZLow;
            /* 0x2A */ u8 xFraction;
            /* 0x2B */ u8 zFraction;
            /* 0x2C */ u8 unk2C;
            /* 0x2D */ u8 unk2D;
            /* 0x2E */ u8 unk2E;
            /* 0x2F */ u8 unk2F;
        } drift;
        /* func_1516FBCC: blends its colour from rgb0 to rgb1 while it grows */
        struct {
            /* 0x26 */ u8 r0;
            /* 0x27 */ u8 g0;
            /* 0x28 */ u8 b0;
            /* 0x29 */ u8 r1;
            /* 0x2A */ u8 g1;
            /* 0x2B */ u8 b1;
            /* 0x2C */ u8 blend;
            /* 0x2D */ s8 growX;
            /* 0x2E */ s8 growY;
            /* 0x2F */ u8 fadeShift;
        } tint;
        /* func_1516ECAC: scaled to size * alpha / 128 */
        struct {
            /* 0x26 */ u8 fadeRate;
            /* 0x27 */ u8 unk27[5];
            /* 0x2C */ u8 sizeHigh;
            /* 0x2D */ u8 sizeLow;
        } pulse;
        /* func_1516A648 (game_197120.c): drifts like the drift kinds and bounces off the floor */
        struct {
            /* 0x26 */ u8 drift[6];
            /* 0x2C */ u8 floorHi; /* floor height + 10, high byte */
            /* 0x2D */ u8 floorLo;
        } bounce;
        /* func_1516E8CC: colour from an 8-step ramp in D_800A6E0C */
        struct {
            /* 0x26 */ u8 start;
        } ramp;
    } u;
    /* 0x30 */ TriggerVolume *link; /* the volume that follows the particle (linkArg), or NULL */
} StyleParticle;

/* A 0x4C-byte particle emitter, spawned by func_1516D99C and run by func_1516DB90. Most of
 * its bytes are the template that func_1516D4E8 gets for each particle it spawns. */
typedef struct SpriteEmitter {
    /* 0x00 */ u8 unk0; /* 0x11, or 0x66 for an emitter whose particles are 0x67s */
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 x; /* also a pointer's high half (flags 0x30) or a D_800A6E30 row */
    /* 0x10 */ s16 y; /* also a pointer's low half (flags 0x30) */
    /* 0x12 */ s16 z;
    /* 0x14 */ s16 scaleX;
    /* 0x16 */ s16 scaleY;
    /* 0x18 */ s16 scaleXRandom;
    /* 0x1A */ s16 scaleYRandom;
    /* 0x1C */ s16 velocityY;
    /* 0x1E */ u16 velocityYRandom;
    /* 0x20 */ u16 timer; /* frames left; 0xFFFF runs forever */
    /* 0x22 */ u16 flags;
    /* 0x24 */ u8 texture;
    /* 0x25 */ u8 frame;
    /* 0x26 */ u8 r;
    /* 0x27 */ u8 g;
    /* 0x28 */ u8 b;
    /* 0x29 */ u8 alpha;
    /* 0x2A */ u8 envR;
    /* 0x2B */ u8 envG;
    /* 0x2C */ u8 envB;
    /* 0x2D */ u8 kind;
    /* 0x2E */ u8 velocityXHigh;
    /* 0x2F */ u8 velocityXLow;
    /* 0x30 */ u8 velocityZHigh;
    /* 0x31 */ u8 velocityZLow;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 velocityXHighRandom;
    /* 0x39 */ u8 velocityXLowRandom;
    /* 0x3A */ u8 velocityZHighRandom;
    /* 0x3B */ u8 velocityZLowRandom;
    /* 0x3C */ u8 initialTimer;
    /* 0x3D */ u8 radiusXZ;
    /* 0x3E */ u8 radiusY;
    /* 0x3F */ u8 object; /* movement_obj index whose bone it follows; 0xFF for none */
    /* 0x40 */ u8 particleLifetime;
    /* 0x41 */ u8 interval;
    /* 0x42 */ u8 countdown;
    /* 0x43 */ u8 chance; /* out of 256 per particle; 0 always spawns */
    /* 0x44 */ u8 burst; /* particles per spawn */
    /* 0x45 */ u8 speed;
    /* 0x46 */ u8 startOffset;
    /* 0x47 */ u8 bone;
    /* 0x48 */ u8 particleLink;
    /* 0x49 */ u8 pad49[3];
} SpriteEmitter;

void func_1516D400(void);
/* K&R definition: the int-argument prototype that callers use. */
void func_1516D4E8(s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
void func_1516D678(StyleParticle *particle);
Gfx *func_1516D738(Gfx *gfx, StyleParticle *particle, s32 arg2);
/* K&R definition: the int-argument prototype that callers use */
void func_1516D99C(s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, u8, s32, s32, s32, s32, s32, u8, s32);
void func_1516DB90(SpriteEmitter *arg0);
s32 func_1516E8CC(StyleParticle *particle);
s32 func_1516ECAC(StyleParticle *particle);
void func_1516ED68(s32 object, s32 duration, u8 arg2, s32 arg3);
void func_1516EED4(s32 object, s32 threshold, u8 arg2, s32 arg3);
s32 func_1516F024(StyleParticle *particle);
void func_1516F1C0(s32 object, s32 duration, u8 arg2, s32 arg3);
/* (u8 object, s32 duration, s32 particleLifetime, s32 speed, u8 arg4, s32 arg5); declared without a prototype:
   game_50D80.c passes the object index and arg4 unnarrowed */
void func_1516F2F8();
void func_1516F548(s32 x, s32 y, s32 z, s32 arg3, s32 velocityXHigh, s32 velocityY, s32 velocityZHigh, s32 scale,
                   s32 scaleRandom, s32 radius, s32 style0, s32 style1, s32 arg12, s32 arg13, s32 arg14, u8 arg15,
                   s32 arg16);
void func_1516F864(StyleParticle *particle);
void func_1516F8EC(StyleParticle *particle, s32 factor);
void func_1516F91C(StyleParticle *particle, s32 factor);
void func_1516F94C(StyleParticle *particle, s32 factor);
void func_1516F984(StyleParticle *particle, s32 factor);
s32 func_1516F9C4(StyleParticle *particle);
void func_1516FA88(s16 x, s16 y, s16 z, s32 isAlt, u8 arg4, s32 arg5);
s32 func_1516FBCC(StyleParticle *particle);
s32 func_1516FD50(StyleParticle *particle);
void func_1516FE1C(s32 object, s32 duration, u8 arg2, s32 arg3);
void func_15170034(s32 flags, f32 *outX, f32 *outY, f32 *outZ);
/* void func_151700D8(f32 (*verts)[3], f32 x, f32 z, u8 style, f32 scale, s32 type, s32 countA, s32 countB, u8 arg8,
   s32 arg9) is declared in the files that call it: game_19E040.c passes an s32 style. */
/* (s16 x, s16 y, s16 z, s32 angle, s32 isParticle, u8 arg5, s32 arg6); declared without a prototype: game_1A89B0.c
 * passes the coordinates without narrowing them */
void func_15170500();
s32 func_151707E0(StyleParticle *particle);
void func_151709B4(s32 x, s32 y, s32 z, s32 count, f32 speed, u8 arg5, s32 arg6);

/* game_19F150.c */
extern Gfx D_8008CC30[]; /* render setup (object type table D_8008B4A8) */
extern Gfx D_8008CC58[]; /* render teardown (object type table D_8008B4A8) */
/* A textured sprite particle: one quad, drawn by func_151720C4. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Vtx vtx[4];
    /* 0x50 */ s16 lifetime;  // drawn while >= 2; 0 starts the -3 -> -1 removal countdown
    /* 0x52 */ s16 animFrame; // 8.8 fixed point frame index
    /* 0x54 */ s16 animSpeed;
    /* 0x56 */ u16 color;     // RGBA5551
    /* 0x58 */ u8 animIndex;  // index into D_8008CA4C
    /* 0x59 */ u8 flags;      // 1: cloud blend, 2: flat quad, 4: play animation once, 8: texture-only combine
} Particle19F150;

Particle19F150 *func_15171CA0(s16 lifetime, u16 color, u8 animIndex, s32 flags, s32 animSpeed, s32 arg5, u8 arg6,
                              s32 arg7);
void func_15171D4C(f32 x, f32 y, f32 z, s16 lifetime, u16 color, u8 animIndex, f32 angle, s32 dist, s32 size, s32 flags,
                   s32 animSpeed, s32 arg11, u8 arg12, s32 arg13);
void func_15171F04(f32 x, f32 y, f32 z, void *tri, s16 lifetime, u16 color, u8 animIndex, s32 size, s32 flags,
                   s32 animSpeed, s32 arg10, u8 arg11, s32 arg12);
void func_15171FC0(Particle19F150 *particle);
Gfx *func_151720C4(Gfx *gfx, Particle19F150 *particle, s32 arg2);
s32 func_151725FC(Vtx *vtx, void *tri, f32 x, f32 y, f32 z, f32 angle, s32 size, u8 width);
s32 func_15172B20(Vtx *vtx, f32 x, f32 y, f32 z, f32 angle, s32 dist, s32 size, u8 width);

/* game_1A1B40.c */
/* The parts of a spawned particle that func_15174920 uses (layout as Particle in game_1944C0.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 growth;          /* copied from BillboardTemplate.unk4 */
    /* 0x18 */ s32 fadeRate;        /* copied from BillboardTemplate.unk8 */
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ s16 width;
    /* 0x36 */ s16 height;
    /* 0x38 */ s16 life;
    /* 0x3A */ u8 pad3A[5];
    /* 0x3F */ u8 primA;
} PuffParticle;

void func_15174690(s32 objIndex, s32 spread, s32 growth, s32 growthRange, s32 size, s32 fadeRate, s32 velY, u8 owner,
                   s32 list);
void func_15174920(PuffParticle *particle);

/* game_1A7260.c */
void func_15179DB0(u8 slot);

/* game_1A7490.c */
extern Gfx D_8008CE20[]; /* render setup of object types 8 and 9 (object type table D_8008B4A8) */
extern Gfx D_8008CE40[]; /* render setup of object types 12 and 89 (object type table D_8008B4A8) */
/*
 * Small spinning, fading debris particles. They are allocated with func_15167A68 (0xB8 bytes) and driven by
 * the object-type handler table in .rodata (D_8008B648 etc.): an update callback (func_1517A1EC or
 * func_1517A84C), the draw callback func_1517A3A0 and the begin-draw hook func_1517A958.
 */
typedef struct SpinDebrisParticle {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Mtx mtx[2];   // model matrix, double-buffered by D_800BE9C0
    /* 0x90 */ s16 x;
    /* 0x92 */ s16 y;
    /* 0x94 */ s16 z;
    /* 0x96 */ s16 rotX;
    /* 0x98 */ s16 rotY;
    /* 0x9A */ s16 rotZ;
    /* 0x9C */ s8 spinX;
    /* 0x9D */ s8 spinY;
    /* 0x9E */ s8 spinZ;
    /* 0x9F */ u8 texture;   // passed to func_1517A9A8
    /* 0xA0 */ s16 velX;
    /* 0xA2 */ s16 velY;
    /* 0xA4 */ s16 velZ;
    /* 0xA6 */ u16 timer;
    /* 0xA8 */ f32 scale;
    /* 0xAC */ u16 unkAC;    // 5 selects falling motion; also the timer value below which drag applies
    /* 0xAE */ u16 dragRange; // the horizontal speed drops by up to this much per frame
    /* 0xB0 */ s16 groundY;
    /* 0xB2 */ u8 viewFlags; // bit n: drawn in view n; bit n+4: left view n afterwards
    /* 0xB3 */ u8 alpha;
} SpinDebrisParticle; // size 0xB8

void func_15179FE0(u8 texture, s16 x, s16 y, s16 z, s8 velY, u16 timer, u16 unkAC, u8 speedRange, u8 spinRange,
                   u8 dragRange, s32 type);
void func_1517A1EC(SpinDebrisParticle *particle);
Gfx *func_1517A394(Gfx *gfx);
Gfx *func_1517A3A0(Gfx *gfx, SpinDebrisParticle *particle, s16 view);
void func_1517A644(f32 size, u8 texture, s16 x, s16 y, s16 z);
void func_1517A84C(SpinDebrisParticle *particle);
Gfx *func_1517A958(Gfx *gfx, s32 type, s32 arg2, s32 arg3);
Gfx *func_1517A9A8(Gfx *gfx, s32 texture);
void func_1517AA20(f32 size, s32 x, s32 y, s32 z, s32 count);
Gfx *func_1517AB7C(Gfx *gfx, s32 arg1, s16 player);

/* game_1B74A0.c */
/* The object func_151407D0 spawns for func_15189FF0. head.def.pos / rot are the world-space attach
 * points, updated by func_1518A094. */
typedef struct BoneGlow {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ GlowShape params;
    /* 0x16C */ u8 pad16C[4];
    /* 0x170 */ EmitterAttach attach;
} BoneGlow;

BoneGlow *func_15189FF0(ParticleDef *spawn, GlowShape *params, EmitterAttach *attach, u8 arg3, u8 arg4, u8 arg5,
                        u8 arg6);
s32 func_1518A094(BoneGlow *emitter);
s32 func_1518A214(BoneGlow *emitter, u8 *follow);
s32 func_1518A2E8(BoneGlow *emitter, u8 *follow);
s32 func_1518A324(BoneGlow *emitter, u8 *follow);
void func_1518A360(BoneGlow *emitter, ActorSwapMsg *msg, u8 msgId);

/* game_1B81D0.c */
s32 func_1518AD20(FloorParticle *particle);
s32 func_1518B1AC(FloorParticle *particle);
s32 func_1518B1D8(FloorParticle *particle);
void func_1518B264(f32 *pos, f32 radius, f32 size, s32 count, u8 arg4);
void func_1518B2A8(f32 *pos, f32 radius, f32 size, s32 count, u16 model, s32 arg5, u8 arg6);

/* game_1B8B60.c */
void func_1518B6B0(f32 arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4);

/* game_1B9F30.c */
/* The parts of a spawned particle that func_1518CCA8 uses (layout as Particle in game_1944C0.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 sizeSteps;    /* copied from BillboardTemplate.unk4 */
    /* 0x18 */ u8 pad18[0x1C];
    /* 0x34 */ s16 width;
    /* 0x36 */ s16 height;
    /* 0x38 */ s16 life;
    /* 0x3A */ s8 fadeStep;
    /* 0x3B */ u8 onExpire;
} GrowingParticle;

void func_1518CA80(BillboardSpawn *effect, u8 mode);
void func_1518CCA8(GrowingParticle *particle);

/* game_1BA1D0.c */
void func_1518CD20(WorldObject *placement, u8 arg1, s32 arg2);

/* game_1BA670.c */
/* Particle template, referenced by an EmitterInfo. */
typedef struct ParticleTemplate {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s16 lifeMin;   // lifetime given to a particle when its slot releases it
    /* 0x36 */ s16 lifeRange;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 unk3A;
    /* 0x3B */ u8 unk3B;
    /* 0x3C */ u8 unk3C;
    /* 0x3D */ u8 unk3D;
    /* 0x3E */ u8 unk3E;
    /* 0x3F */ u8 spawnCb; // index into D_8008D620
} ParticleTemplate; /* 0x40 */

/* An emission point attached to one of the owner's bones. */
typedef struct {
    /* 0x0 */ Vec3f pos;
    /* 0xC */ s16 bone;
} EmitterPoint; /* 0x10 */

typedef struct {
    /* 0x00 */ ParticleTemplate *tmpl;
    /* 0x04 */ s32 numSlots;
    /* 0x08 */ EmitterPoint *points; // one per slot (attached mode)
    /* 0x0C */ f32 rate;             // particles added per frame
    /* 0x10 */ s32 maxCount;
    /* 0x14 */ s16 lifeMin;          // frames a slot holds its particle
    /* 0x16 */ s16 lifeRange;
    /* 0x18 */ u8 spread;            // 1: sphere, 2: horizontal disc (world mode)
    /* 0x19 */ u8 mode;              // 2: emit around one point instead of per-bone points
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ f32 speed;
    /* 0x20 */ f32 speedY;
    /* 0x24 */ f32 yOffset;
    /* 0x28 */ f32 x;
    /* 0x2C */ f32 y;
    /* 0x30 */ f32 z;
    /* 0x34 */ u8 bone;
} EmitterInfo;

/* IDs (Actor.unk84) that select one row of an EmitterTable. */
typedef struct {
    /* 0x0 */ s16 count;
    /* 0x4 */ u8 *ids;
} EmitterIdList;

/*
 * infos holds (n + 2) entries per owner variant (Actor.lod): one per id list,
 * then the attached-mode default, then the unattached one.
 */
typedef struct {
    /* 0x0 */ EmitterInfo **infos;
    /* 0x4 */ s16 n;
    /* 0x8 */ EmitterIdList *idLists;
} EmitterTable;

ActorEmitterProcess *func_1518D1C0(Actor *owner, s16 kind, s16 lifetime, u8 killExisting, u8 arg4, u8 arg5, void *arg6);
void func_1518D6E0(ActorEmitterProcess *emitter);
s32 func_1518E298(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
void func_1518E308(ActorEmitterProcess *emitter);
void func_1518E37C(ActorEmitterProcess *emitter, u8 table);
ActorEmitterProcess *func_1518E3C4(ActorEmitterParams *params, u8 arg1);

/* game_1BB950.c */
void func_1518E4A0(ActorEmitterProcess *emitter);
void func_1518E4CC(ActorEmitterProcess *emitter);
void func_1518E4F8(ActorEmitterProcess *emitter);
void func_1518E524(u8 *texture, u8 *flip, u8 *r, u8 *g, u8 *b, u8 *alpha, u16 *renderModes);
void func_1518E5D8(u8 *texture, u8 *flip, u8 *r, u8 *g, u8 *b, u8 *alpha, u16 *renderModes);
s32 func_1518E66C(ActorEmitterProcess *emitter);
s32 func_1518E6D4(ActorEmitterProcess *emitter);
Vtx *func_1518E73C(ActorEmitterParticle *p, s16 vp);

/* game_1BC650.c */
/* Per-type state of a sprite effect, at sprite + 0x110. */
typedef struct {
    /* 0x00 */ f32 hi0;
    /* 0x04 */ f32 lo0;
    /* 0x08 */ f32 target0;
    /* 0x0C */ f32 hi1;
    /* 0x10 */ f32 lo1;
    /* 0x14 */ f32 peak1;
    /* 0x18 */ f32 target1;
    /* 0x1C */ s16 timer0;
    /* 0x1E */ s16 timer1;
    /* 0x20 */ f32 hi2;
    /* 0x24 */ f32 lo2;
    /* 0x28 */ f32 target2;
    /* 0x2C */ s16 timer2;
} SpriteWobble;

typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ SpriteWobble wobble;
} EffectSprite;

/* Particle created by func_151D2BA4 from an EmitterParticleDesc. */
typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ JitterParticle *sprite; /* made by func_1514B8E4 */
    /* 0x30 */ AnchorEmitter *parent;
    /* 0x34 */ Vec3f pos;
} EmitterParticle;

typedef struct {
    s32 unk0;
    u8 unk4;
} ContactKey;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ ContactKey key;
} ContactObj;

s32 func_1518F1A0(EffectSprite *sprite);
s32 func_1518F384(EffectSprite *sprite);
void func_1518F45C(s32 arg0, u8 arg1);
void func_1518F49C(ContactObj *self, ContactKey *other, u8 arg2);
AnchorEmitter *func_1518F51C(Actor *owner, u8 index, s16 duration, s8 updateFunc, s8 unk59Func, s8 childKind, u8 unk5A,
                             s32 arg7, u8 arg8, s32 arg9);
AnchorEmitter *func_1518F5D0(Actor *owner, const AnchorEmitterTemplate *tmpl, s16 duration, s8 updateFunc, s8 unk59Func,
                             s8 childKind, u8 unk5A, s32 arg7, u8 arg8, s32 arg9);
s32 func_1518F7C4(AnchorEmitter *emitter);
void func_1518F858(AnchorEmitter *emitter);
void func_1518F89C(AnchorEmitter *emitter);
void func_1518F8E0(AnchorEmitter *emitter);
s32 func_1518FC04(EmitterParticle *particle, Vec3f *target);
s32 func_1518FC44(EmitterParticle *particle, Vec3f *target);
void func_1518FC84(EmitterParticle *particle, Vec3f *dest);
s32 func_1518FDC4(Actor *obj, u8 *kind, u8 variant);
u8 *func_1519003C(Actor *obj, s32 *found);
LightEffect *func_1519021C(AnchorEmitter *emitter, Actor *owner, u8 kind, s16 duration, u8 arg4, s32 arg5);

/* game_1BD940.c */
void func_15190490(LightEffect *obj);
void func_151904BC(AnchorEmitter *emitter);
void func_15190518(Actor *actor);

/* game_232150.c (data only, linked after the game_1BA670..game_1BDA00 unit) */
/* The emitter tables of game_1BA670.c (its D_8008D5E0). */
extern EmitterTable D_8008F780, D_8008F78C, D_8008F798, D_8008F7A4, D_8008F7B0, D_8008F7BC, D_8008F7C8, D_8008F7D4;
extern EmitterTable D_8008F7E0, D_8008F7EC, D_8008F7F8, D_8008F804, D_8008F810, D_8008F81C, D_8008F828, D_8008F834;

/* game_1BDC20.c */
#define DRIP_SLOT_COUNT 60

typedef struct {
    f32 width, height;
} Size2f;

/* Motion state of a drop that hangs on an actor's bone, then falls and splashes. */
typedef struct {
    /* 0x00 */ Vec3f vel;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ f32 drag;
    /* 0x14 */ s16 fadeStart;       /* lifetime below which the alpha fades */
    /* 0x16 */ s16 fadeRate;        /* alpha per frame of lifetime left */
    /* 0x18 */ f32 trail;           /* how far def.rot (the tail) lags behind def.pos */
    /* 0x1C */ f32 stretch;         /* tilt per sqrt of sideways speed (func_1519108C) */
    /* 0x20 */ u8 dropKind;         /* D_800A9D40 index */
    /* 0x21 */ u8 phase[2];         /* width / height wobble angles */
    /* 0x23 */ s8 phaseSpeed[2];
    /* 0x28 */ Size2f baseSize;
    /* 0x30 */ Size2f wobble;
    /* 0x38 */ CollisionHit ground;
    /* 0x5C */ u8 kind;             /* emitter kind, passed to the splash spawners */
    /* 0x5D */ u8 pad5D[3];
} DripMotion; /* size 0x60 */

typedef struct DripParticle {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ DripMotion motion;
} DripParticle;

typedef struct DripSlot {
    /* 0x0 */ DripParticle *drop;
    /* 0x4 */ s32 timer;            /* ticks until the drop falls */
    /* 0x8 */ struct DripSlot *next;
} DripSlot;

/* A func_151491F4 task that keeps the link of an emitter that could not be created yet. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ActorLink link;
} DripRetryTask;

/* Object type 0x20: hangs drops on the bones of an actor. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ ActorLink link;
    /* 0x1C */ f32 spawnAccum;
    /* 0x20 */ s32 dropCount;
    /* 0x24 */ DripSlot *slots;
    /* 0x28 */ void *extra;         /* caller-sized data after the slots */
    /* 0x2C */ u8 kind;             /* D_800AB310 / D_800AB404 index */
    /* 0x30 */ DripSlot slotStorage[DRIP_SLOT_COUNT];
} DripEmitter; /* size 0x300 */

DripEmitter *func_15190770(ActorLink *link, s32 extraSize, u8 owner, s32 list);
void func_15190840(DripEmitter *emitter);
void func_1519086C(DripEmitter *emitter);
void func_15190898(DripEmitter *emitter);
s32 func_15190F9C(DripParticle *drop);
Vtx *func_1519108C(DripParticle *drop, s16 vp);
void func_15191400(DripEmitter *emitter);
s8 func_15191520(DripParticle *drop);
s32 func_1519187C(DripParticle *drop);
void func_151918BC(DripRetryTask *task);
void func_15191980(DripEmitter *emitter, ActorRef *ref, u8 msg);
void func_15191A84(DripRetryTask *task, ActorRef *ref, u8 msg);
/* (ActorRef *ref, u8 msg); declared without a prototype: game_AC030.c passes the ref as an integer */
void func_15191B8C();

/* game_1BF090.c */
/* Per-particle data of a dust particle (func_1519203C, updated by func_1519257C). */
typedef struct DustData {
    /* 0x00 */ f32 baseSize;
    /* 0x04 */ u8 phase[2];
    /* 0x06 */ s8 phaseStep[2];
    /* 0x08 */ f32 amplitude[2];
    /* 0x10 */ f32 floorY;
    /* 0x14 */ CollisionTri floor;  /* the floor triangle under the spawn point, for the splash */
    /* 0x26 */ u8 kind;
} DustData; /* 0x28 */

/* A sprite particle (func_15130374) carrying DustData. */
typedef struct DustParticle {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ DustData dust;
} DustParticle; /* size 0xD0 */

/* Data of the dust emitter effect (func_15191BE0, run by func_15191D54). */
typedef struct DustEmitterData {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ Vec3f offset;      /* emitter position in the bone's space */
    /* 0x14 */ s32 matrixOffset;   /* byte offset of the bone's 0x40-byte matrix in owner->ob_matrix_ptr */
    /* 0x18 */ f32 accum;          /* particles owed; one is spawned per whole unit */
    /* 0x1C */ f32 rateBase;
    /* 0x20 */ f32 rateRandom;
    /* 0x24 */ CollisionHit floor;
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
    TimedEffect base;
    DustEmitterData data;
} DustEmitter;

DustEmitter *func_15191BE0(Actor *owner, const Vec3f *offset, u8 bone, f32 rateBase, f32 rateRandom, s16 duration,
                           f32 gravityBase, f32 gravityRandom, s16 lifetimeBase, s16 lifetimeRandom, u8 alphaBase,
                           u8 alphaRandom, f32 sizeBase, f32 sizeRandom, f32 unk60, u8 kind, u8 arg16, s32 arg17);
void func_15191D54(DustEmitter *effect);
DustParticle *func_1519203C(Vec3f *position, Vec3f *velocity, f32 gravity, s16 lifetime, u8 alpha, u8 brightness,
                            f32 size, s32 unused, f32 floorY, CollisionTri *floor, u8 kind, u8 arg11, s32 arg12);
u8 func_15192308(DustParticle *particle, Vec3f *prevPos);
u8 func_15192358(DustParticle *particle, Vec3f *prevPos);
u8 func_1519257C(DustParticle *particle, Vec3f *prevPos);
DustEmitter *func_151925C4(Actor *obj, s16 duration, u8 arg2, s32 arg3);
void func_1519277C(DustEmitter *effect, s32 arg1, u8 arg2);

/* game_1BFC70.c */
void func_151927C0(BillboardParticle *p);

/* game_1BFDD0.c */
/* Data of the effects of this file that follow an object: an ObjectEmitterData (game_1368C0.c) without the
 * rate and the callbacks, which are fixed in code. */
typedef struct {
    /* 0x00 */ Actor *object;
    /* 0x04 */ u8 objectId; /* object->unique_id when attached */
    /* 0x08 */ f32 spawnAccum;
} FixedEmitterData; /* size 0xC */

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ FixedEmitterData data;
} Effect1BFDD0;

void func_15192920(Actor *object);
void func_151929A4(Effect1BFDD0 *effect);
Effect1BFDD0 *func_15192D48(Actor *object, s16 type, u8 arg2, s32 arg3);
void func_15192DF0(Effect1BFDD0 *effect);
void func_15193234(Effect1BFDD0 *effect, void *msg, u8 msgType);
void func_151932E0(Effect1BFDD0 *effect, void *msg, u8 msgType);

/* game_1C0B10.c */
/* Particle emitter that func_151938E4 initialises and func_151938FC spawns from. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0xF];
    /* 0x1C */ Actor *owner;
    /* 0x20 */ u8 pad20[0x54];
    /* 0x74 */ f32 scale; /* scales (to - from) into the spawn velocity */
} SegmentEmitter;

void func_15193660(Actor *actor, u8 arg1, s32 arg2);
void func_151937F4(Actor *owner, s32 arg1, s32 arg2);
s32 func_151938E4(SegmentEmitter *emitter);
void func_151938FC(Vec3f *from, Vec3f *to, s32 arg2, s32 arg3, f32 t, SegmentEmitter *emitter);

/* game_1CDEC0.c */
/* Data of the spark emitter effect, stored at 0x28 in the effect object from func_151491F4. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ u8 variant;
    /* 0x10 */ f32 accum; /* particles owed; one is emitted per whole unit */
} PointSparkEmitter; // size 0x14

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ PointSparkEmitter emitter;
} SparkEmitterEffect;

void func_151A0A10(Actor *actor, s16 duration, u8 owner, s32 arg3);
void func_151A0AF8(SparkEmitterEffect *effect);
s32 func_151A0C0C(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s32 angle,
                  s32 baseAngle, f32 radius, s32 variant, f32 arg12, s32 arg13, u8 owner);

/* game_1CE2F0.c */
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

/* Timed effect of the sphere emitters below (func_151A11E4, func_151A1998, func_151A1FB4). */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EmitterParams params;
} SphereEmitter;

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

void func_151A0E40(f32 x, f32 y, f32 z, u8 arg3, s32 arg4);
void func_151A0F28(f32 x, f32 y, f32 z, u8 arg3, s32 arg4);
void func_151A1010(f32 x, f32 y, f32 z, s16 duration, f32 scaleA, f32 scaleB, u8 arg6, s32 arg7);
void func_151A11CC(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9,
                   f32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, f32 arg17, s32 arg18,
                   s32 arg19, s32 arg20, s32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26, s32 arg27);
void func_151A11E4(SphereEmitter *emitter);
void func_151A175C(BillboardParticle *particle);
void func_151A18DC(EffectSpawner *spawner);
void func_151A1998(SphereEmitter *emitter);
void func_151A1E34(BillboardParticle *particle);
void func_151A1EE8(EffectSpawner *spawner);
void func_151A1FB4(SphereEmitter *emitter);
void func_151A24A8(BillboardParticle *particle);
void func_151A25E0(BillboardParticle *particle);
s32 func_151A26EC(Vec3f *pos, Vec3f *offset, Vec3f *vel, f32 drag, f32 gravity, f32 size, s16 timer, u8 alpha,
                  s16 fadeStart, s16 fadeOutStart, s16 eventTime, s8 eventFunc, u8 r, u8 g, u8 b, u8 argF, s32 arg10);
s32 func_151A2960(SpriteParticle *particle, Vec3f *prevPos);
void func_151A2A14(Vec3f *pos, s16 yaw, s16 pitch, f32 speed, f32 sizeBase, f32 sizeSlope, s32 count, f32 spread,
                   f32 drag, f32 gravityBase, f32 gravityRange, s16 delayBase, s16 delayRange, s16 alphaBase,
                   s16 alphaRange, s16 fadeStart, s16 fadeOutStart, s8 eventFunc, u8 arg18, s32 arg19);
void func_151A2AD4(Vec3f *pos, f32 *dir, f32 sizeBase, f32 sizeSlope, s32 count, f32 spread, f32 drag, f32 gravityBase,
                   f32 gravityRange, s16 delayBase, s16 delayRange, s16 alphaBase, s16 alphaRange, s16 fadeStart,
                   s16 fadeOutStart, s8 eventFunc, u8 arg16, s32 arg17);
void func_151A2B84(f32 *dir, s32 yaw, s32 pitch, f32 speed, f32 t, Vec3f *out);
void func_151A2BD0(f32 *dir, s32 yaw, s32 pitch, f32 speed, f32 t, Vec3f *out);
void func_151A2C24(Vec3f *pos, f32 *dir, s16 yaw, s16 pitch, f32 speed, f32 sizeBase, f32 sizeSlope, s32 count,
                   f32 spread, f32 drag, f32 gravityBase, f32 gravityRange, s16 delayBase, s16 delayRange,
                   s16 alphaBase, s16 alphaRange, s16 fadeStart, s16 fadeOutStart, s8 eventFunc, u8 arg13, s32 arg14,
                   u8 useAngles);
void func_151A2F0C(SpriteParticle *particle);

/* game_1D0600.c */
void func_151A3150(SpriteParticle *particle);

/* game_1D2B10.c */
void func_151A5660(u8 *effect);
void func_151A5CAC(TimedEffect *arg0);
void func_151A5CEC(TimedEffect *arg0);
void func_151A5D2C(u16 soundHandle);
QuadSprite *func_151A5D58(f32 size, u8 arg1, CollisionTri *surface, Vec3f *pos, s16 duration, u8 arg5, u8 arg6,
                          u8 arg7, s32 arg8);

/* game_1D3AB0.c */
/* State at 0x110 of the particle emitter over a TriggerZone (func_151A6600, func_151A6AB8). */
typedef struct ZoneEmitterState {
    /* 0x00 */ TriggerZone *region;
    /* 0x04 */ f32 angle;
    /* 0x08 */ f32 angleSpeed;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ EffectHeader *unk10; /* released by func_151A6AB8 */
    /* 0x14 */ u8 phase;
    /* 0x15 */ u8 phaseSpeed;
    /* 0x16 */ u8 alpha;      /* oscillates in [129, 255] with phase */
    /* 0x17 */ u8 hasZone;    /* nonzero: register a zone around the object each frame */
    /* 0x18 */ f32 pending;   /* particles owed, accumulated per frame */
} ZoneEmitterState;

typedef struct ZoneEmitter {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ ZoneEmitterState state;
} ZoneEmitter;

s32 func_151A6600(ZoneEmitter *obj);
void func_151A6AB8(ZoneEmitter *obj);

/* game_1D9A00.c */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
} EffectSample; // size 0x14

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ u8 pad4[0x17];
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ u8 unk20;
} EffectPayload;

/* A func_15147A80 effect object (EffectObj) whose items are EffectSamples and whose user data is an
 * EffectPayload. */
typedef struct {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ EffectSample *samples;
    /* 0x98 */ EffectPayload *payload;
} SampleTrail;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8;
} EmitterOwner;

/* A camera-facing quad particle drawn from double-buffered vertices. */
typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 unk1C;
    /* 0x01E */ u8 pad1E[0xE];
    /* 0x02C */ f32 width;
    /* 0x030 */ f32 height;
    /* 0x034 */ f32 x;
    /* 0x038 */ f32 y;
    /* 0x03C */ f32 z;
    /* 0x040 */ u8 pad40[0xC];
    /* 0x04C */ f32 unk4C;
    /* 0x050 */ f32 unk50;
    /* 0x054 */ u8 pad54[8];
    /* 0x05C */ u8 unk5C;
    /* 0x05D */ u8 pad5D[0x63];
    /* 0x0C0 */ Vtx template[4];
    /* 0x100 */ Vtx *buffers[1];
} QuadParticle;

s32 func_151AC550(SampleTrail *state, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface);
s32 func_151AC61C(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s32 yaw,
                  s32 baseYaw, f32 radius, s32 kind, f32 arg12, EmitterOwner *owner, u8 arg14);
Vtx *func_151AC810(QuadParticle *particle, s16 view);
s32 func_151AC9EC(QuadParticle *particle);
s32 func_151ACA20(QuadParticle *particle);

/* game_1DBD40.c */
void func_151AE890(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, u8 *arg6);
void func_151AE984(Vec3f *pos, f32 dirX, f32 dirY, f32 dirZ, s32 flag, u8 arg5);
void func_151AEAB4(Vec3f *pos, u8 flag, u8 arg2);

/* game_1DC720.c */
void *func_151AF270(Actor *actor, u8 arg1, s32 arg2);
void func_151AF338(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);
void func_151AF388(f32 *position, f32 dx, f32 dy, f32 dz, s32 effectUnkC);

/* game_1DCA70.c */
/* Growth applied by func_151AFC08, stored in the spray particle at 0x128. */
typedef struct {
    /* 0x00 */ s16 growEndTimer; // grows while desc.life is above this
    /* 0x04 */ f32 growRate;     // size gained per frame
} SprayGrowth;

/* The spray particle func_151AF6D4 spawns; updated by func_151AFC08. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ u8 pad110[0x18];
    /* 0x128 */ SprayGrowth growth;
} SprayParticle;

s32 func_151AF5C0(Vec3f *pos, Actor *actor, CollisionHit *ground, u8 side);
s32 func_151AF6C0(s32 arg0, s32 arg1);
void func_151AF6D4(Actor *actor, s32 side, s32 arg2);
s32 func_151AFBD4(QuadSprite *sprite);
s32 func_151AFC08(SprayParticle *spray);
s32 func_151AFC88(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);
s32 func_151AFEA4(SparkParticle *spark, s32 arg1);

/* game_1DEF60.c */
/* Per-task data kept at offset 0x28 of the task created by func_151B1AB0. */
typedef struct {
    Actor *owner;
    u8 ownerUnk3B;
    f32 timer;
} Data151B1AB0;

typedef struct {
    TimedEffect base;
    Data151B1AB0 data;
} Task151B1AB0;

void func_151B1AB0(Actor *owner);
void func_151B1B34(Task151B1AB0 *task);
void func_151B1FAC(Task151B1AB0 *task, ActorMsg *msg, u8 kind);

/* game_1E30A0.c */
/* Object whose sound handle func_151B5FCC stops. */
typedef struct {
    /* 0x00 */ u8 pad0[0x88];
    /* 0x88 */ s32 sound;
} SoundOwner;

PulseParticle *func_151B5BF0(Vec3f *pos, Vec3f *vel, f32 size, Vec3f *rot, Vec3f *rotVel, f32 gravity, s16 lifetime,
                             u8 alpha, s32 unused, f32 elasticity, u8 fadeShift, u8 arg11, u8 arg12, s32 arg13);
void func_151B5E8C(void);
void func_151B5E94(PulseParticle *particle);
void func_151B5FCC(SoundOwner *obj);

/* game_1E34C0.c */
PulseParticle *func_151B6010(Vec3f *pos, Vec3f *vel, f32 size, Vec3f *rot, Vec3f *rotVel, f32 gravity, s16 lifetime,
                             u8 alpha, s32 unused, f32 elasticity, u8 fadeShift, u8 arg11, u8 arg12, s32 arg13);
void func_151B6254(PulseParticle *particle);

/* Sprite particle types of game_2062D0.c, here before their first use in a prototype. */
/* Per-particle data of a func_151D9014 particle (SteamParticle.data). */
typedef struct {
    f32 size;
    u8 phaseX;
    u8 phaseY;
    s8 stepX;
    s8 stepY;
    f32 amplitudeX;
    f32 amplitudeY;
    f32 burstScale;
    f32 altBurstScale;
    u8 kind;
    u8 flags;
    u8 pad1A[2];
} SteamWobble; /* 0x1C */

/* Per-particle data of a func_151DB5D0 particle (SteamParticle.data). */
typedef struct {
    u8 flags;
    f32 drag;
    u8 angleX;
    u8 angleY;
    u8 angleZ;
    u8 stepX; /* signed steps, but the u8 angle sums wrap the same */
    u8 stepY;
    u8 stepZ;
    Vec3f amplitude;
} ParticleSpin; /* 0x1C */

/* A live sprite particle as returned by func_15130280. */
typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ union {
        SteamWobble wobble;
        ParticleSpin spin;
    } data;
} SteamParticle;

/* game_1E5FF0.c */
/* Head of a pooled physics particle (full layout in game_15F680.c), with the bob in its vel/gravity slots. */
typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Vec3f rot;
    /* 0x2C */ Vec3f scale;
    /* 0x38 */ Vec3f pos;
    /* 0x44 */ f32 bobPhase;     // vel.x
    /* 0x48 */ f32 bobCenter;    // vel.y
    /* 0x4C */ f32 bobSpeed;     // vel.z
    /* 0x50 */ Vec3f rotVel;
    /* 0x5C */ f32 bobAmplitude; // gravity
} BobParticle;

/* Motion extension of a model effect (EffectMotion in game_1844C0.c), with the bob in its vel/gravity slots. */
typedef struct {
    /* 0x00 */ Vec3f rot;
    /* 0x0C */ f32 scale;
    /* 0x10 */ f32 phase;        // vel.x
    /* 0x14 */ f32 center;       // vel.y
    /* 0x18 */ f32 speed;        // vel.z
    /* 0x1C */ Vec3f rotVel;
    /* 0x28 */ f32 amplitude;    // gravity
} EffectBob;

/* Model effect (Effect in game_1844C0.c; only the fields used here). */
typedef struct {
    /* 0x000 */ u8 pad0[0x54];
    /* 0x054 */ Vec3f pos;      // params.pos
    /* 0x060 */ u8 pad60[0x120 - 0x60];
    /* 0x120 */ EffectBob bob;   // extra
} BobEffect;

/* Bob state that follows a game_169510.c particle (another member of its DefParticleMotion union). */
typedef struct {
    /* 0x00 */ f32 center;
    /* 0x04 */ f32 amplitude;
    /* 0x08 */ f32 phase;
    /* 0x0C */ f32 speed;
    /* 0x10 */ Vec3f rotVel;
} QuadBob;

/* Quad particle of game_169510.c (only the fields used here), followed by its motion state. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ QuadBob bob;
} BobQuad;

s32 func_151B8B40(BobParticle *particle);
s32 func_151B8BE0(SteamParticle *particle, Vec3f *prevPos);
s32 func_151B8C54(BobEffect *effect);
s32 func_151B8CFC(BobQuad *particle);

/* game_1E6B40.c */
/* A particle spawned by func_151B9690 (layout as Particle in game_1944C0.c). */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 list;
    /* 0x02 */ u8 pad2[0xA];        /* object link */
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ TextureDesc *anim;
    /* 0x14 */ s32 floorAndMinSize; /* low half floor height, high half minimum size */
    /* 0x18 */ s32 kind;
    /* 0x1C */ s16 frame;
    /* 0x1E */ s16 frameStep;
    /* 0x20 */ s16 x;
    /* 0x22 */ s16 y;
    /* 0x24 */ s16 z;
    /* 0x26 */ s16 velX;
    /* 0x28 */ s16 velZ;
    /* 0x2A */ s16 size;
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
    /* 0x3B */ u8 phase;
    /* 0x3C */ u8 primR;
    /* 0x3D */ u8 primG;
    /* 0x3E */ u8 primB;
    /* 0x3F */ u8 primA;
} FallingParticle;

/* Type 7 object (0x2C bytes): emits func_151B9690 particles every frame until its timer runs out. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ f32 *followPos; /* when set, emit from this position instead of x/y/z */
    /* 0x14 */ u8 animIndex;
    /* 0x15 */ u8 kind;
    /* 0x16 */ s16 x;
    /* 0x18 */ s16 y;
    /* 0x1A */ s16 z;
    /* 0x1C */ s16 velY;
    /* 0x1E */ s16 velYRange;
    /* 0x20 */ s16 speed;
    /* 0x22 */ s16 speedRange;
    /* 0x24 */ s16 minSize;
    /* 0x26 */ s16 size;
    /* 0x28 */ s16 count;      /* particles emitted per frame besides the 4 fast ones */
    /* 0x2A */ s16 timer;
} FallingEmitter;

void func_151B9690(s32 animIndex, s32 kind, s16 x, s16 y, s16 z, s16 unused, f32 angle, s16 angleSpread, s16 velY,
                   s16 velYRange, s16 speed, s16 speedRange, s16 radius, s16 minSize, s16 size, u8 owner, s32 list);
void func_151B9964(FallingParticle *particle);
void func_151B9BF0(s32 animIndex, s32 kind, s16 x, s16 y, s16 z, s16 velY, s16 velYRange, s16 speed, s16 speedRange,
                   s16 minSize, s16 size, s16 count, s16 timer, f32 *followPos, u8 owner, s32 list);
void func_151B9CB0(FallingEmitter *emitter);

/* game_1E9820.c */
void func_151BC370(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);

/* game_1F1CD0.c */
typedef struct Game1F1CD0Actor {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ProjectileStream emission;
} Game1F1CD0Actor;

void func_151C4820(Game1F1CD0Actor *actor);

/* game_1FA770.c */
/* One particle of a stream (0x1C bytes); the emitter keeps them in a ring. */
typedef struct {
    Vec3f pos;
    f32 velocity;
    f32 width;
    s16 alpha;
    f32 tc; /* texture t coordinate */
} StreamParticle;

/* Per-stream state, the 0x50-byte data block of a stream emitter. */
typedef struct {
    f32 swayAmpX;
    f32 swayAmpZ;
    f32 swayPhaseX;
    f32 swayPhaseZ;
    f32 swayRateX;
    f32 swayRateZ;
    Vec2fXZ spawnPos; /* where the last particle was spawned */
    f32 unk20;
    f32 baseWidth;
    f32 widthAmp;
    f32 widthPhase;
    f32 widthRate;
    f32 width;
    f32 unk38;
    f32 tc;
    f32 spawnCount; /* particles owed; one is spawned per whole unit */
    f32 floorY;
    s32 id;
    u8 textureIndex;
    u8 pad4D[3];
} StreamData;

typedef struct LevelStream {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 buffers[0x10];
    /* 0x94 */ StreamParticle *particles;
    /* 0x98 */ StreamData *data;
} LevelStream;

/* The spawner task's state: owns at most one emitter for its source. */
typedef struct {
    TriggerZone *source;
    s32 id;
    LevelStream *emitter;
    u8 textureIndex;
    u8 padD[3];
} StreamSpawner;

typedef struct {
    TimedEffect base;
    StreamSpawner spawner;
} StreamTask;

StreamTask *func_151CD2C0(TriggerZone *source, u8 arg1, s32 arg2);
void func_151CD35C(s32 id);
void func_151CD394(s32 id);
void func_151CD3CC(StreamTask *task, s32 *id, u8 event);
/* Unprototyped: func_151CD4C0 is a K&R definition (callers pass its u8 arguments unnarrowed). */
LevelStream *func_151CD4C0();
s32 func_151CD674(LevelStream *emitter);
s32 func_151CD7BC(LevelStream *emitter);
s32 func_151CDB94(LevelStream *emitter);
Gfx *func_151CDE20(LevelStream *emitter, Gfx *gfx, s16 view);
void func_151CE47C(LevelStream *emitter);
void func_151CE49C(LevelStream *emitter);
void func_151CE4DC(LevelStream *emitter);
void func_151CE51C(void *actor, TriggerZone *source);
LevelStream *func_151CE634(s32 id);

/* game_1FBB80.c */
typedef struct Game1FBB80Owner {
    s16 field00;
    s16 mask;
} Game1FBB80Owner;

typedef struct Game1FBB80Effect {
    void *shape;
    f32 rate;
    f32 accumulator;
    Game1FBB80Owner *owner;
    f32 height;
} Game1FBB80Effect;

typedef struct Game1FBB80Object {
    u8 pad00, flags01;
    u8 pad02[0xA];
    u8 flags0C;
    u8 pad0D[0x1B];
    Game1FBB80Effect effect;
} Game1FBB80Object;

void func_151CE6D0(Game1FBB80Object *object);

/* game_200930.c */
extern const Vec3f *D_8008FC8C[]; /* muzzle point of each gun (game_12CCF0 reads gun 0) */
extern u8 *D_8008FC94[];          /* the bone that point is on */
/* Bobbing motion stored in a debris particle's user data (object + 0x170). */
typedef struct {
    /* 0x0 */ s16 timer;
    /* 0x4 */ f32 phase;
    /* 0x8 */ f32 phaseSpeed;
    /* 0xC */ f32 amplitude;
} BobMotion;

/* A debris particle as seen by its update callback. */
typedef struct {
    /* 0x000 */ u8 pad0[0x20];
    /* 0x020 */ Vec3f unk20;
    /* 0x02C */ u8 pad2C[0x50 - 0x2C];
    /* 0x050 */ Vec3f unk50;
    /* 0x05C */ u8 pad5C[0x170 - 0x5C];
    /* 0x170 */ BobMotion wobble;
} WobbleParticle;

/* Effect user data that follows a point on an object, computed by D_8008FC70[posFunc]. */
typedef struct {
    /* 0x00 */ Actor *obj;
    /* 0x04 */ u8 uniqueId;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 posFunc;
    /* 0x0C */ f32 spawnTimer;
    /* 0x10 */ u8 flags; /* bit 0: pos is valid */
    /* 0x14 */ Vec3f pos;
} ObjEmitter;

/* An effect created by func_151D469C; its user data is an ObjEmitter. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ObjEmitter emitter;
} ObjEmitterEffect;



void func_151D3480(Vec3f *pos, Vec3f *dir, CollisionTri *surface, u8 kind, u8 arg4, s32 arg5);
s32 func_151D3D50(WobbleParticle *particle);
void func_151D3E04(Actor *obj, Vec3f *out, const Vec3f *localPos, u8 bone, f32 yOffset);
void func_151D3E6C(Actor *obj, Vec3f *a, Vec3f *b, s32 arg3);
void func_151D3F14(Vec3f *pos, u8 arg1, s32 arg2);
void func_151D3FF4(const Vec3f *pos, u8 arg1, s32 arg2);
void func_151D40D4(Vec3f *pos, Vec3f *dir, Actor *obj, Actor *target, CollisionTri *surface, s32 arg5, s32 arg6,
                   s32 arg7);
void func_151D42E8(Vec3f *pos, Vec3f *dir, Actor *obj, CollisionTri *surface, s32 arg4);
void func_151D4408(Vec3f *pos, Vec3f *dir, void *mtx, Actor *owner, f32 scale, u8 arg5, s32 arg6);
void func_151D4668(Actor *obj);
void func_151D469C(Actor *obj, s32 posFunc, s32 arg2, s32 arg3, s32 arg4);
void func_151D4794(ObjEmitterEffect *effect);
void func_151D4C38(ObjEmitterEffect *effect, ActorRef *handle, u8 msg);
s32 func_151D4D04(SpriteParticle *puff, s32 arg1);
void func_151D4D58(Actor *obj);
s32 func_151D4DAC(Actor *victim, Actor *attacker, Vec3f *from, Vec3f *to, RayHit *hit, s32 flags, Vec3f *dir, u8 arg7,
                  s32 arg8);
void func_151D5148(Actor *obj);
void func_151D5174(Actor *obj, Vec3f *arg1, Vec3f *arg2, Vec3f *arg3, Vec3f *arg4, Vec3f *pointBuf, Vec3f *dirBuf,
                   Vec3f **outPoint, Vec3f **outDir, Vec3f *arg9);
void func_151D5334(Vec3f *pos, f32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5, s32 arg6);
void func_151D5404(Vec3f *pos, f32 arg1, f32 arg2, f32 arg3, s32 base, s32 range, s32 arg6, s32 arg7);
void func_151D5514(const Vec3f *pos, u8 arg1, s32 arg2);
void func_151D5648(Vec3f *pos, u8 arg1, s32 arg2);
void func_151D5714(Actor *obj, const Vec3f *from, const Vec3f *to, u8 bone, f32 scale, u8 arg5, s32 arg6);
void func_151D57F8(Actor *obj, u8 slot);
void func_151D5A18(Actor *obj, Vec3f *out, Vec3f *forward, Vec3f *axis1, Vec3f *axis2, const Vec3f *localPos, u8 bone);
s32 func_151D5B6C(Vec3f *p0, Vec3f *p1, Actor *self, s8 filter, void *filterArg);
void func_151D5D60(void *slots, s16 index, s32 size, void *out, u8 *allocated);
void func_151D5E30(void *slots);

/* game_2062D0.c */
extern u8 D_800E0A10;              /* the effect kind of the current context (func_151D8E20) */
extern u8 (*D_8008FD04[13])(void); /* a second picker table by effect kind */
/* Data of a func_151DA08C effect: wobbles its scale and keeps spawning func_151DB5D0 particles. */
typedef struct {
    u8 phaseX;
    u8 phaseY;
    s8 stepX;
    s8 stepY;
    f32 amplitudeX;
    f32 amplitudeY;
    f32 spawnCredit;
    f32 size;
    f32 shrink; /* size multiplier per frame (timed effects) */
    u8 kind;
    u8 pad19[3];
} SteamData; /* 0x1C */

typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ u8 pad110[0x18];
    /* 0x128 */ SteamData steam;
} SteamEffect;

/* Data of a func_15149130 effect that drops a func_151D9014 particle whenever its timer runs out. */
typedef struct {
    s16 timer;
    f32 size;
    Vec3f pos;
    u8 unk14;
    u8 unk15;
    u8 kind;
} DripData; /* 0x18 */

typedef struct {
    TimedEffect base;
    DripData drip;
} DripEffect;

/* Data a splash carries for the bursts it makes on impact (func_151DA938, func_151DAA88). */
typedef struct {
    f32 burstScale;
    f32 altBurstScale;
    u8 kind;
    u8 pad9[3];
} SplashBurst; /* 0xC */

/* The block a live splash points to at +0x98: its TrailMotion copy, state, and SplashBurst. */
typedef struct {
    TrailMotion motion;
    u8 unk20;
    u8 pad21[0x27];
    SplashBurst burst;
} SplashState;

typedef struct {
    Vec3f pos;
    f32 unkC;
    f32 unk10;
} SplashNode; /* 0x14 */

typedef struct {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ SplashNode *nodes;
    /* 0x98 */ SplashState *state;
} Splash;

/* Data of a func_151DAB58 effect: squashes and stretches it on one phase. */
typedef struct {
    u8 phase;
    s8 step;
    u8 pad2[2];
    f32 amplitudeX;
    f32 amplitudeY;
} SquashData; /* 0xC */

typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ SquashData squash;
} SquashEffect;

s32 func_151D8E20(void);
u8 func_151D8E6C(void);
u8 func_151D8EB0(void);
u8 func_151D8EBC(void);
u8 func_151D8EC8(void);
u8 func_151D8EFC(void);
u8 func_151D8F30(void);
u8 func_151D8F7C(void);
u8 func_151D8FB0(void);
u8 func_151D8FBC(void);
u8 func_151D8FC8(void);
u8 func_151D8FD4(void);
u8 func_151D8FE0(void);
s32 func_151D93F4(SteamParticle *particle, Vec3f *arg1);
s32 func_151D9450(SteamParticle *particle, Vec3f *arg1);
u8 func_151D9534(SteamParticle *particle, Vec3f *arg1);
u8 func_151D97A8(void);
u8 func_151D9820(void);
u8 func_151D9878(void);
u8 func_151D98D0(void);
u8 func_151D9918(void);
u8 func_151D9960(void);
u8 func_151D99C8(void);
u8 func_151D9A20(void);
u8 func_151D9A68(void);
u8 func_151D9AC0(void);
u8 func_151D9ADC(void);
u8 func_151D9B34(void);
void func_151D9B8C(u8 kind, f32 size, u8 arg2, void *tri, Vec3f *pos, s16 lifetime, u8 arg6, u8 arg7, u8 arg8, u8 arg9,
                   s32 argA);
void func_151D9EB0(DripEffect *effect);
void func_151D9FC0(u8 kind, f32 size, u8 arg2, CollisionTri *surface, Vec3f *pos, u8 arg5, s32 arg6);
SteamEffect *func_151DA08C(u8 kind, f32 size, f32 shrink, u8 arg3, s16 lifetime, CollisionTri *surface, Vec3f *pos,
                          s32 arg7, s32 arg8);
s32 func_151DA368(SteamEffect *effect);
s32 func_151DA6A8(SteamEffect *effect);
Splash *func_151DA6F8(Vec3f *pos, Vec3f *vel, f32 size, s16 lifetime, u8 arg4, f32 scale, s32 arg6, u8 arg7,
                      f32 burstScale, f32 altBurstScale, u8 argA, u8 kind, CollisionHit *ground, s16 argD, s16 argE,
                      s32 material, u8 arg10, s32 arg11);
s32 func_151DA938(Splash *splash, f32 x, f32 y, f32 z, f32 floorY, CollisionTri *surface);
s32 func_151DAA88(Splash *splash, f32 x, f32 y, f32 z, f32 floorY, CollisionTri *surface);
void func_151DAB58(u8 kind, f32 size, u8 arg2, Vec3f *pos, volatile u8 arg4, u8 arg5, s32 arg6);
s32 func_151DADA0(SquashEffect *effect);
Vtx *func_151DAE28(DefParticleHead *effect, s16 view);
void func_151DB004(ParticleDesc *desc);
void func_151DB068(ParticleDesc *desc);
void func_151DB0CC(ParticleDesc *desc);
void func_151DB15C(ParticleDesc *desc);
void func_151DB1EC(ParticleDesc *desc);
void func_151DB27C(ParticleDesc *desc);
void func_151DB2A8(ParticleDesc *desc);
void func_151DB2CC(ParticleDesc *desc);
void func_151DB330(ParticleDesc *desc);
void func_151DB3D8(ParticleDesc *desc);
void func_151DB43C(ParticleDesc *desc);
void func_151DB4CC(ParticleDesc *desc);
s32 func_151DB97C(SteamParticle *particle, s32 arg1);
void func_151DBAA8(Vec3f *pos, s32 count, u8 kind, u8 arg3, s32 arg4);
void func_151DBBD4(Vec3f *pos, CollisionHit *hit, u8 *kind, u8 arg3, s32 arg4);
void func_151DBCBC(u8 kind, f32 size, s16 arg2, CollisionTri *surface, void *pos, u8 arg5, s32 arg6);
void func_151DBE80(u8 kind, f32 size, f32 arg2, s16 lifetime, Vec3f *pos, CollisionTri *tri, u8 arg6, u8 arg7,
                   u8 arg8, s32 arg9);
void func_151DC034(Vec3f *pos, f32 size, u8 arg2, s16 lifetime, u8 kind, u8 arg5, s32 arg6);
/* (Vec3f *pos, CollisionHit *ground, u8 kind, u8 arg3, s32 arg4); declared without a prototype: game_1C1150.c
   passes the kind unnarrowed */
void func_151DC260();
/* (Vec3f *pos, CollisionHit *ground, u8 kind, u8 arg3, s32 arg4); declared without a prototype: game_1C1150.c
   passes the kind unnarrowed */
void func_151DC484();
extern const u8 D_800AB310[13]; // texture id per effect kind
extern const u8 D_800AB3F4[13]; // particle texture id per effect kind
extern const u8 D_800AB404[13]; // flag per effect kind
extern const Color24 D_800AB414[13]; // effect colours (RGB per colour index / effect kind)

/* game_209B50.c */
/* Data of the effect spawned by func_151DC8BC and updated by func_151DC97C. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 accum;        /* particles still to emit; func_151DC97C emits one per whole unit */
    /* 0x10 */ u8 randomColors;  /* nonzero: each particle takes one of three random colour pairs */
    /* 0x11 */ u8 pad11[3];
} ColorEmitterData; /* 0x14 */

/* Effect created by func_15149130 (only the fields used here). */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ColorEmitterData data;
} ColorEmitter;

void func_151DC6A0(Vec3f *pos, u8 owner, s32 kind);
/* (Vec3f *pos, s32 duration, u8 burst, u8 randomColors, u8 owner, s32 kind); declared without a prototype:
 * game_50D80.c passes an s32 owner without narrowing it */
void func_151DC8BC();
void func_151DC97C(ColorEmitter *emitter);

/* game_20A290.c */
void func_151DCDE0(void *arg0, u8 arg1, s32 arg2);

#endif
