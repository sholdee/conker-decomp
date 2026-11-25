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

/* game_FA240.c */
void func_150CCD90(f32 z, f32 *r, f32 *g, f32 *b);

/* game_FB060.c */
void func_150CDBB0(Actor *actor, u8 owner, s32 arg2);
s32 func_150CDCF4(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);
s32 func_150CDF10(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);

/* game_120950.c */
f32 func_150F34A0(Actor *obj, f32 pitch);
void func_150F34F4(Actor *actor);
void func_150F43F0(Camera *actor);

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
