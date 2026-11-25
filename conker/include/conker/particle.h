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
void func_1500C2A0(void);
void func_1500C2F0(s32 region, f32 density);
void func_1500C8D0(s32 region, f32 density);
void func_1500CE4C(s32 region, f32 density);
void func_1500D3AC(s32 region, f32 density);
void func_1500D8C8(s32 region, f32 density);
void func_1500DE88(s32 region, f32 density);

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
    u8 pad0[0x170];
    EffectHeader *trail; /* ribbon effect (func_151B7328) following the projectile */
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
s32 func_150B060C(u8 id, struct Obj150B060C *out);

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

s32 func_150B89E0(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, void *surface);
s32 func_150B8F44(ModelParticle *particle, CollisionProbe *probe, f32 x, f32 y, f32 z);

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
    /* 0x04 */ s32 unk4;
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

s32 func_150CB0E0(f32 *pos, Actor *actor, CollisionHit *ground, u8 side);
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
    f32 matrix[16];
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
    f32 matrix[16];
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
void func_150D278C(s16 *record, f32 *matrix, u8 unkC, s32 unk1);
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
void func_150D65F0(f32 *pos, s32 *arg1, u8 arg2, s32 arg3);
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
    /* 0x14 */ s32 unk14[5];
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
    /* 0x08 */ u8 unk8[0x14];
} Game113D60DebrisExtra;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x028 */ u8 pad28[0x10];
    /* 0x038 */ Vec3f position;
    /* 0x044 */ u8 pad44[0x12C];
    /* 0x170 */ Game113D60DebrisExtra extra;
} Game113D60Debris;

void func_150E68B0(s32 actor, s32 arg1, s32 arg2, u8 sourceIndex);
void func_150E6B84(Game113D60Effect *effect);
void func_150E6E34(Vec3f *out);
void func_150E6ED8(Vec3f *out);
void func_150E6F18(Vec3f *out);
void func_150E6FAC(Vec3f *out, Actor *actor);
void func_150E70CC(Vec3f *out, Actor *actor);
void func_150E70EC(Vec3f *start, Vec3f *end, Vec3f *delta, Game113D60Emission *emission);
void func_150E71E4(Vec3f *start, Vec3f *end, Vec3f *delta, Game113D60Emission *emission);
void func_150E7290(u8 arg0, u8 owner, s32 kind);
void func_150E75A0(Vec2f *position, f32 size, s16 unk12, u8 flags, u8 unk1E, u8 unk23, s16 unk16, s16 unk18, s32 *ids,
                   s32 idCount, u8 owner, s32 kind);
void func_150E76D0(f32 size, s16 unk12, u8 flags, u8 unk1E, u8 unk23, s16 unk16, s16 unk18, s32 *ids, s32 idCount,
                   u8 owner, s32 kind);
TimedEffect *func_150E7994(s16 count, f32 rate, u8 owner, s32 kind);
void func_150E7C9C(Game113D60BurstEffect *effect);
void func_150E7FEC(f32 size, u8 arg1, s32 arg2, Vec3f *position, s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, u8 owner, s32 kind);
void func_150E81A8(s32 index, s32 owner, s32 kind);
void func_150E83AC(Vec3f *position, s16 duration, u8 owner, s32 kind);
void func_150E8470(Game113D60EmitterEffect *effect);
s32 func_150E8824(Game113D60Particle *particle, s32 arg1);
void func_150E8854(void);
void func_150E88C0(Game113D60TimerEffect *effect);
void func_150E8930(void);
void func_150E8A80(void);
void func_150E8B1C(Game113D60RegionSpawnerEffect *effect);
void func_150E8D5C(Game113D60PointSpawnerEffect *effect);
void func_150E90DC(void);
void func_150E9178(Game113D60RegionSpawnerEffect *effect);
void func_150E93DC(Game113D60PointSpawnerEffect *effect);
s32 func_150E971C(Game113D60Debris *debris);

/* game_116D40.c */
/* Spawner state at +0x28 of a burst emitter. */
typedef struct {
    /* 0x00 */ s32 source;      // where func_1514470C places each burst
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

/* game_120950.c */
f32 func_150F34A0(Actor *obj, f32 pitch);
void func_150F34F4(Actor *actor);
void func_150F43F0(Camera *actor);

/* game_121A20.c */
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
    /* 0x10 */ TimedEffect *unk10;
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
typedef struct {
    DefParticleHead head;
    EffectParticleState state;
} EffectParticle;

EffectParticle *func_15100600(Actor *owner, Vec3f *vec1, Vec3f *vec2, u8 arg3, s16 lifetime, u8 r, u8 g, u8 b, f32 arg8,
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
TimedEffect *func_1510A344(Actor *object, s32 duration, u8 owner, s32 kind);
void func_1510A40C(Effect1368C0 *effect);
void func_1510A870(Effect1368C0 *effect, ActorSwapMsg *msg, u8 msgType);
void func_1510A8CC(Effect1368C0 *effect, ActorSwapMsg *msg, u8 msgType);

/* game_15D730.c */
SpriteParticle *func_15130280(ParticleDesc *desc, u8 kind, CollisionHit *ground, s32 dataSize, u8 arg4, s32 arg5);
SpriteParticle *func_15130374(ParticleDesc *desc, u8 kind, s32 dataSize, u8 arg3, s32 arg4);
SpriteParticle *func_151303BC(ParticleDesc *desc, u8 kind, s32 dataSize);
void func_151303EC(SpriteParticle *p);
Gfx *func_15130A9C(Gfx *gfx, SpriteParticle *p, s16 viewIdx);
void func_1513137C(s16 *r, s16 *g, s16 *b, s16 *a, SpriteParticle *p);
void func_15131514(s16 *r, s16 *g, s16 *b, s16 *a, SpriteParticle *p);
void func_1513164C(s16 *primR, s16 *primG, s16 *primB, s16 *primA, s16 *envR, s16 *envG, s16 *envB, s16 *envA,
                   SpriteParticle *p);
s32 func_151316AC(SpriteParticle *p, Vec3f *prevPos);
s32 func_151316DC(SpriteParticle *p, Vec3f *prevPos);
s32 func_1513170C(SpriteParticle *p, Vec3f *prevPos);
void func_1513173C(SpriteParticle *p);
void func_1513175C(SpriteParticle *p);
void func_1513177C(SpriteParticle *p);
s32 func_151317C8(SpriteParticle *p);
s32 func_15131814(s32 arg0, s32 arg1);
void func_15131828(SpriteParticle *p, f32 *amplitude, u8 *phase, u8 *phaseStep);
void func_151318E8(Vec3f *vel, f32 drag);
void func_15131918(Vec3f *vel, f32 drag);
void func_15131958(Vec3f *vel, f32 drag);
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
void func_151321D0(ModelParticle *particle);
void func_151323AC(ModelParticle *particle);
void func_151323F8(ModelParticle *particle);
void func_15132444(ModelParticle *particle);
void func_15132570(ModelParticle *particle);
void func_1513259C(ModelParticle *particle);
void func_151325C8(ModelParticle *particle);
ModelParticle *func_1513264C(ParticleSpawn *spawn, s32 arg1, s32 arg2, CollisionHit *floor, s32 extraSize, u8 arg5,
                             s32 arg6);
ModelParticle *func_15132A4C(ParticleSpawn *spawn, s32 arg1, s32 arg2, s32 extraSize, u8 arg4, s32 arg5);
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
/* Source of the four vertex colours given to func_151400D0. */
typedef struct {
    s16 r, g, b, a;
    u16 flag;
} VtxColor;

void func_1513C060(QuadSprite *sprite);
QuadSprite *func_1513C350(void *def, void *colors, u8 updateFn, u8 expireFn, u8 flip, s32 model, s32 arg6, Vec3f *pos,
                          s32 extraSize, u8 arg9, s32 argA);
QuadSprite *func_1513C4EC(void *def, void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width, f32 length,
                          u8 angle, u8 flip, s32 model, s32 arg6, s32 extraSize, u8 argE, s32 argF);
QuadSprite *func_1513C5B0(void *def, void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width, f32 length,
                          u8 angle, u8 flip, s32 extraSize, u8 argC, s32 argD);
QuadSprite *func_1513C650(void *def, u8 updateFn, u8 expireFn, s32 arg3, f32 x, f32 y, f32 z, f32 width, f32 height,
                          u8 angle, u8 flip, s32 model, s32 arg6, s32 extraSize, u8 argE, s32 argF);
QuadSprite *func_1513C73C(void *def, u8 updateFn, u8 expireFn, s32 arg3, f32 x, f32 y, f32 z, f32 width, f32 height,
                          u8 angle, u8 flip, s32 extraSize, u8 argC, s32 argD);
QuadSprite *func_1513C804(void *def, s32 arg1, u8 updateFn, u8 expireFn, s32 arg4, f32 x, f32 y, f32 z, f32 width,
                          f32 height, u8 angle, u8 flip, s32 extraSize, u8 argD, s32 argE);
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
void func_1513CFD0(s32 arg0, s32 arg1, s32 arg2);
void func_1513CFE4(DefParticleHead *particle);
void *func_1513D2F0(void *def, void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 model,
                    s32 arg8, s32 motionSize, u8 argA, s32 argB);
void *func_1513D4B8(void *def, void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip,
                    s32 motionSize, u8 arg8, s32 arg9);
void *func_1513D524(void *def, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 motionSize, u8 arg7,
                    s32 arg8);
void *func_1513D594(void *def, void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width,
                    f32 height, s32 arg9, s32 argA, s32 model, s32 argC, u8 argD, s32 motionSize, u8 argF, s32 arg10);
void *func_1513D668(void *def, void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width,
                    f32 height, s32 arg9, s32 argA, u8 argB, s32 motionSize, u8 argD, s32 argE);
s32 func_1513D6FC(ParticleAxes *axes, s16 angle, f32 width, f32 height, s32 arg4, s32 arg5, u8 onSlope);
Gfx *func_1513DB00(Gfx *gfx, DefParticleHead *particle, s16 viewport);
void func_1513DF9C(DefParticleHead *particle, s32 arg1, u8 arg2);
void func_1513E070(s32 arg0, s32 arg1, s32 arg2);
void func_1513E084(DefParticleHead *particle, ActorMsg *msg, u8 eventId);
void func_1513E134(void);
void func_1513E13C(QuadSprite *sprite, f32 x, f32 y, f32 z, f32 width, f32 length, u8 angle);
void func_1513E2A4(void);
s32 func_1513E2AC(s32 sprite, s32 arg1, s32 surface, f32 x, f32 y, f32 z, f32 width, f32 height, u8 angle);
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
void func_151400D0(Vtx *vtx, VtxColor *colors);
Vtx *func_15140190(DefParticleHead *particle, s16 viewport);
void func_151403A8(s32 arg0, u8 arg1);
void func_151403DC(s32 arg0, u8 arg1);
Vtx *func_15140410(DefParticleHead *particle, Vec3f *axisU, Vec3f *axisV, s16 viewport);
s32 func_151406AC(s32 arg0, s32 list, s32 group, s16 phase);

/* game_176A00.c */
ChannelParticle *func_15149550(ParticleEmitterDesc *emitter, u8 arg1, u8 arg2, u8 kind, u8 arg4, s32 arg5);
s32 func_15149838(ChannelParticle *particle);
s32 func_15149A94(ChannelParticle *particle);
s32 func_15149BF4(ChannelParticle *particle);
s32 func_15149C58(ChannelParticle *particle);
Vtx *func_15149D18(ChannelParticle *particle, s16 index);
s32 func_15149EC4(ChannelParticle *particle);

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
                   s32 arg10, u8 arg11);
void func_1514C470(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 count, u8 kind, s32 arg8, f32 arg9, s32 arg10,
                   u8 arg11);
void func_1514C678(f32 x, f32 y, f32 z, f32 radius, s16 angleA, s16 angleB, s16 count, u8 kind, s32 arg8, f32 arg9,
                   s32 arg10, u8 arg11);
void func_1514C858(f32 x, f32 y, f32 z, f32 radius, s16 yaw, s16 angleA, s16 angleB, s16 count, u8 kind, s32 arg9,
                   f32 arg10, s32 arg11, u8 arg12);

/* game_1A7260.c */
void func_15179DB0(u8 slot);

/* game_1B81D0.c */
s32 func_1518AD20(FloorParticle *particle);
s32 func_1518B1AC(FloorParticle *particle);
s32 func_1518B1D8(FloorParticle *particle);
void func_1518B264(Vec3f *pos, f32 radius, f32 size, s32 count, u8 arg4);
void func_1518B2A8(Vec3f *pos, f32 radius, f32 size, s32 count, u16 model, s32 arg5, u8 arg6);

/* game_1B8B60.c */
void func_1518B6B0(f32 arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4);

/* game_1BA1D0.c */
void func_1518CD20(WorldObject *placement, u8 arg1, s32 arg2);

/* game_1BB950.c */
void func_1518E4A0(ActorEmitterProcess *emitter);
void func_1518E4CC(ActorEmitterProcess *emitter);
void func_1518E4F8(ActorEmitterProcess *emitter);
void func_1518E524(u8 *texture, u8 *flip, u8 *r, u8 *g, u8 *b, u8 *alpha, s16 *renderModes);
void func_1518E5D8(u8 *texture, u8 *flip, u8 *r, u8 *g, u8 *b, u8 *alpha, s16 *renderModes);
s32 func_1518E66C(ActorEmitterProcess *emitter);
s32 func_1518E6D4(ActorEmitterProcess *emitter);
Vtx *func_1518E73C(ActorEmitterParticle *p, s16 vp);

/* game_1BFC70.c */
void func_151927C0(BillboardParticle *p);

/* game_1D0600.c */
void func_151A3150(FadeEventParticle *particle);

/* game_1D2B10.c */
void func_151A5660(u8 *effect);
void func_151A5CAC(TimedEffect *arg0);
void func_151A5CEC(TimedEffect *arg0);
void func_151A5D2C(u16 soundHandle);
QuadSprite *func_151A5D58(f32 size, u8 arg1, s32 arg2, f32 *pos, s16 duration, u8 arg5, u8 arg6, u8 arg7, s32 arg8);

/* game_1DBD40.c */
void func_151AE890(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, u8 *arg6);
void func_151AE984(f32 *pos, f32 dirX, f32 dirY, f32 dirZ, s32 flag, u8 arg5);
void func_151AEAB4(f32 *pos, s32 flag, u8 arg2);

/* game_1DC720.c */
void *func_151AF270(Actor *actor, u8 arg1, s32 arg2);
void func_151AF338(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);
void func_151AF388(f32 *position, f32 dx, f32 dy, f32 dz, s32 effectUnkC);

/* game_1E34C0.c */
PulseParticle *func_151B6010(Vec3f *pos, Vec3f *vel, f32 size, Vec3f *rot, Vec3f *rotVel, f32 gravity, s16 lifetime,
                             u8 alpha, s32 unused, f32 elasticity, u8 fadeShift, u8 arg11, u8 arg12, s32 arg13);
void func_151B6254(PulseParticle *particle);

/* game_1E9820.c */
void func_151BC370(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);

/* game_20A290.c */
void func_151DCDE0(void *arg0, u8 arg1, s32 arg2);

#endif
