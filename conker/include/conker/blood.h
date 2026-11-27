#ifndef CONKER_BLOOD_H
#define CONKER_BLOOD_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "blood": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Effects/BloodStrips/bloodstrips.c
 *   game/Effects/Blood/blood.c
 *   game/Effects/BloodDebris/blooddebris.c
 *   game/Effects/BloodDrip/blooddrip.c
 *   game/Effects/BloodSpray/bloodspray.c
 *   game/Effects/BloodEmitters/bloodemitters.c
 */


typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
    u8 pad20[0xE];
    u8 unk2E;
    u8 pad2F[0x41];
    f32 unk70;
    f32 unk74;
} Struct15134CEC;

/* Motion state of a blood splat particle, kept at BloodSplat::motion. */
typedef struct {
    s16 timer;              /* frames of growth before the splat starts to fall */
    f32 growth;
    Vec2f velocity;
    f32 gravity;
    f32 spread;             /* growth per unit of horizontal speed while falling */
} Blood356D4Motion;

/* A 2D blood splat particle spawned by blood_spawnSplat. */
typedef struct {
    u8 pad0[0x10];
    Vec2f position;
    f32 width;
    f32 height;
    u8 pad20[0x50];
    Blood356D4Motion motion;
} BloodSplat;

/* The particle blood_spawnGrowingParticle spawns; its desc is copied to +0x18 and `extra` to growth. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ u8 pad110[0x18];
    /* 0x128 */ f32 growth;
} GrowingBloodParticle;

/* A bouncing chunk thrown by particle_newModelParticleWithFloor / particle_newModelParticle (from a ParticleSpawn desc at +0x10). */
typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    f32 radius;
    f32 bounce;             /* velocity factor kept on each bounce */
    f32 width;
    f32 height;
    u8 pad20[0xC];
    f32 unk2C;
    f32 type;               /* 0, 1 or 2 (no decal), from the blood profile */
    f32 unk34;
    Vec3f position;
    Vec3f velocity;
    Vec3f spin;
    f32 unk5C;
    s32 flags;
    u8 pad64[0x10C];
    s32 unk170;             /* counted down by message 0x45; first byte set by the spawner */
} BloodChunk;

/* A BloodTrail of a debris piece (blood_spawnDebris), with the size of that piece as its extra data. */
typedef struct DebrisBloodTrail {
    /* 0x00 */ BloodTrail trail;
    /* 0x40 */ f32 size;
} DebrisBloodTrail;

/* Per-particle data of the debris pieces, at 0x170 of the particle. */
typedef struct {
    /* 0x0 */ f32 stopSpeed;   /* landing slower than this (vel.y above it) stops the piece */
    /* 0x4 */ DebrisBloodTrail *trail;
    /* 0x8 */ u8 landPuffs;    /* spawn particle_scatterChannelParticles particles on landing */
    /* 0x9 */ u8 landPuffFlag;
    /* 0xA */ s16 landPuffLifeBase;
    /* 0xC */ s16 landPuffLifeRange;
} DebrisData;

/* A debris piece: a ModelParticle with DebrisData. */
typedef struct {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ DebrisData debris;
} BloodDebrisParticle;

/* An effect owned by an actor; effect_bloodTrailOwnerMessage handles its messages, blood_handleTrailMessage forwards them while unk28 is 1. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 pad29[0x17];
    /* 0x40 */ ActorRef owner;
} OwnedEffect40;

/* One point of a ribbon trail. */
typedef struct {
    Vec3f pos;
    s16 lifetime;
    u8 alpha;
    u8 padF;
    f32 texU; /* texture coordinate along the ribbon */
} RibbonTrailPoint;

/* Trail state that follows the owner actor. */
typedef struct {
    ActorRef owner;
    Vec3f lastPos;      /* 0x08: position of the newest point */
    f32 pending;           /* 0x14: points owed, emitted once it exceeds 1 */
    f32 unk18;
    f32 texU;              /* 0x1C: texture coordinate reached by the owner */
    f32 lastTexU;          /* 0x20: texture coordinate of the newest point */
} RibbonTrailData;

/* Ribbon trail effect: a ring buffer of points from head (oldest) to tail (next write). */
typedef struct {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 buffers[0x10];
    /* 0x94 */ RibbonTrailPoint *points;
    /* 0x98 */ RibbonTrailData *data;
} RibbonTrail;

typedef struct {
    TimedEffect base;
    ActorRef owner;
} OwnedEffect28;

/* BloodStrip spawned by effect_spawnBloodStrips, with two bytes of extra data. */
typedef struct {
    BloodStrip strip;
    u8 pad7C[4];
    s16 sampleAge; /* 0x80: frames since the last sample */
} AgedBloodStrip;


/* game/Effects/BloodStrips/bloodstrips.c */
void blood_attachMirroredStrips(Actor *owner, s16 duration, u8 arg2, s32 arg3);
void blood_emitMirroredStripParticle(f32 *from, f32 *to, s32 arg2, s32 arg3, s32 arg4, EffectHeader *task);

/* game/Effects/Blood/blood.c */
s32 blood_getProfile(Actor *actor);
s32 blood_fadeQuadSpriteByLife(DefParticleHead *particle);
void *blood_spawnDrip(BloodDripDesc *desc, s32 arg1, u8 arg2, s32 arg3);
void blood_updateDrip(BloodDrip *drip);
s32 blood_clearDripStatic(s32 arg0, BloodDrip *drip, s32 arg2);
void blood_destroyDripDefault(EffectHeader *effect);
void blood_freeDripDefault(EffectHeader *effect);
void blood_destroyDrip(BloodDrip *drip);
void blood_freeDrip(BloodDrip *drip);
void blood_handleDripMessage(void *arg0, ActorMsg *msg, u8 message);
void blood_emitNoDripDrop(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *emitter);
void *blood_spawnTrail(BloodTrailDesc *desc, s32 extraSize, u8 arg2, s32 arg3); /* a BloodTrail, then extraSize bytes */
void blood_updateTrail(BloodTrail *trail);
void blood_handleTrailMessage(OwnedEffect40 *effect, ActorMsg *msg, u8 msgId);
void blood_emitNoTrailDrop(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *emitter);
s32 func_15134CEC(Struct15134CEC *particle);
void *blood_spawnStrip(BloodStripDesc *desc, s32 extraSize); /* a BloodStrip, then extraSize bytes */
void blood_updateStrip(BloodStrip *arg0);
void blood_destroyStripDefault(EffectHeader *effect);
void blood_freeStripDefault(EffectHeader *effect);
void blood_destroyStrip(BloodStrip *arg0);
void blood_freeStrip(BloodStrip *arg0);
void blood_transformPoints(void *matrix, Vec3f *inA, Vec3f *inB, Vec3f *outA, Vec3f *outB);
void blood_handleStripMessage(BloodStrip *strip, ActorMsg *msg, u8 message);
void blood_handleStripMessageMode2(BloodStrip *strip, ActorMsg *msg, u8 message);
void blood_handleStripMessageMode1(BloodStrip *strip, ActorMsg *msg, u8 message);
s32 blood_sampleFullPower(BloodStrip *strip);
f32 blood_getTunedPower(BloodStrip *strip);
void *blood_spawnSplat(Vec2f *position, f32 size, f32 gravity, f32 spread, u8 alpha, u8 endAlpha, s16 delay, s16 lifetime,
                    s16 duration, s16 period, u8 arg10, s32 arg11);
s32 blood_updateSplat(BloodSplat *splat);
void blood_spray(Vec3f *from, Vec3f *to, f32 radius, u8 arg3, s32 arg4);
void blood_scatterDecals(Actor *actor, f32 radius, u8 arg2, s32 arg3);
GrowingBloodParticle *blood_spawnGrowingParticle(f32 size, f32 arg1, u8 alpha, u8 arg3, s16 lifetime, CollisionTri *surface,
                                    Vec3f *arg6, u8 arg7, u8 arg8, u8 arg9, s32 arg10);
s32 blood_updateGrowingParticle(GrowingBloodParticle *particle);
void func_15136918(f32 size, u8 arg1, u8 arg2, s16 lifetime, s16 arg4, CollisionTri *surface, f32 *position, u8 arg7,
                   s32 arg8);
s32 blood_capQuadSpriteAlphaByLife(DefParticleHead *particle);
void blood_startShortTrail(f32 *x, f32 *y, f32 *z, s16 lifetime, s32 arg4, s32 arg5);
void blood_emitTrailSpray(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, EffectHeader *emitter);
void blood_spawnHitEffects(Actor *actor, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7);
void blood_spawnImpact(Vec3f *from, Vec3f *to, Vec3f *direction, u8 arg3, u8 arg4, s32 arg5);
void blood_spawnActorImpact(Actor *actor, Vec3f *from, Vec3f *to, Vec3f *direction, u8 arg4, s32 arg5);
void blood_spawnDeathEffects(Actor *actor, u8 arg1, s32 arg2);
s32 blood_buildImpactFrame(Vec3f *start, Vec3f *end, Vec3f *dir, Vec3f *side, Vec3f *up, Vec3f *from, Vec3f *to,
                  Vec3f *direction);
s32 blood_sampleRandomPower(BloodStrip *strip);
void blood_emitStripParticle(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip);
void blood_getStripParticleParams(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip,
                   Vec3f *start, Vec3f *end, Vec3f *velocity, f32 *arg3, s16 *arg4, u8 *arg5, f32 *arg6);
s32 blood_getBloodPoint(Actor *actor, s32 kind, Vec3f *out);
void blood_startBleeding(Actor *actor, s32 kind, u8 arg2);
void blood_spawnBurst(Vec3f *position, s32 kind, void *arg2, u8 arg3, s32 arg4);
void blood_throwChunks(Actor *actor, Vec3f *position, s32 kind, void *arg3, u8 arg4, s32 arg5);
s32 blood_onChunkLand(BloodChunk *chunk, f32 x, f32 y, f32 z, f32 groundY, CollisionTri *surface);
void blood_spawnBoneHit(Actor *actor, u8 arg1, s32 arg2);
void blood_spawnFatalHit(Actor *actor, u8 arg1, s32 arg2);
void func_15138E98(Actor *actor, Vec3f *origin, u8 extraSpray, CollisionHit *hit, u8 arg4, s32 arg5);
s32 blood_onGorePartLand(BloodChunk *chunk, f32 x, f32 y, f32 z, f32 groundY, CollisionTri *surface);
void blood_spawnTedizGore(Actor *actor, Vec3f *origin, f32 scale, CollisionHit *hit, u8 arg4, s32 arg5);
void blood_spawnDefaultGore(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4);
void blood_spawnProfileBurst(Vec3f *position, const s32 *table, const f32 *arg2, s32 count, f32 scale, u8 kind, u8 arg6, Actor *actor,
                   CollisionHit *hit, u8 arg9, s32 argA);
void blood_spawnSmallSplash(Vec3f *position, u8 arg1, s32 arg2);
void blood_spawnSplashOnlyGore(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4);
void blood_spawnSplash(Vec3f *position, u8 arg1, s32 arg2);
void blood_spawnConkerGore(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4);
void blood_spawnZombieGore(Actor *actor, Vec3f *origin, u8 variant, CollisionHit *hit, u8 arg4, s32 arg5);
void blood_handleChunkMessage(BloodChunk *chunk, s32 arg1, u8 message);
void blood_spawnGreggGore(Actor *actor, Vec3f *origin, CollisionHit *hit, u8 arg3, s32 arg4);
extern const f32 gBloodBurstScales[15];
extern const s32 *gBloodModelLists[20];
extern const s32 gConkerBloodModels[10];
extern const s32 gWeaselBloodModels[8];
extern const s32 gTedizBloodModels[8];
extern const s32 D_800A3CD4[8];
extern const s32 gUgaBugaBloodModels[8];
extern const s32 D_800A3D14[8];
extern const s32 D_800A3D34[8];
extern const s32 gBlackConkerBloodModels[10];
extern const s32 gZombieDressBloodModels[8];
extern const s32 gZombieSuitBloodModels[8];
extern const s32 gRoboSpiderBloodModels[12];
extern const s32 gTntImpBloodModels[8];
extern const s32 D_800A3E0C[8];
extern const s32 gRockBloodModels[7];
extern const s32 D_800A3E48[10];
extern const s32 gBroadTedizBloodModels[8];
extern const s32 D_800A3E90[9];
extern const s32 D_800A3EB4[8];
extern const s32 D_800A3ED4[7];
extern const s32 D_800A3EF0[9];
extern const s32 gBloodModelCounts[20];
extern const s32 gBloodBurstFloorHitFns[20];
extern const BloodProfile gBloodProfiles[2];
extern const BloodProfile gTedizBloodProfiles[2];
extern const BloodProfile D_800A4018[1];
extern const BloodProfile D_800A4028[1];
extern const BloodProfile D_800A4038[2];
extern const BloodProfile gZombieDressBloodProfiles[1];
extern const BloodProfile gZombieSuitBloodProfiles[6];
extern const BloodProfile gBroadTedizBloodProfiles[3];
extern const BloodProfile D_800A40F8[2];
extern const u8 D_800A4160[6];
extern const s32 D_800A4168[4];
extern const s32 D_800A4178[4];
extern const Vec3f D_800A4188[4];
extern const Vec3f D_800A41B8;
extern const u8 gTedizGoreMasks[4];
extern const s32 gTedizGoreAttachedModels[4];
extern const s32 gTedizGoreThrownModels[4];
extern const Vec3f gTedizGoreThrowOffsets[4];
extern const u8 gDefaultGoreMasks[3];
extern const s32 gDefaultGoreAttachedModels[3];
extern const s32 gDefaultGoreThrownModels[3];
extern const Vec3f gDefaultGoreThrowOffsets[3];
extern const s32 gBloodSmallSplashIds[1];
extern const f32 gBloodSmallSplashScales[1];
extern const s32 gBloodSplashIds[2];
extern const f32 gBloodSplashScales[2];
extern const u8 gConkerGoreMasks[6];
extern const s32 gConkerGoreAttachedModels[4];
extern const s32 gConkerGoreThrownModels[4];
extern const Vec3f gConkerGoreThrowOffsets[4];
extern const u8 gZombieGoreMasks[3];
extern const s32 gZombieGoreAttachedModels[2][2];
extern const s32 gZombieGoreThrownModels[2][2];
extern const Vec3f gZombieGoreThrowOffsets[2][2];
extern const u8 gGreggGoreMasks[3];
extern const s32 gGreggGoreAttachedModels[2];
extern const s32 gGreggGoreThrownModels[2];
extern const Vec3f gGreggGoreThrowOffsets[2];

/* game/Effects/BloodDebris/blooddebris.c */
void blood_spawnDebris(DebrisEmitter *emitter, u8 mode, u8 arg2, s32 arg3);
s32 blood_dropDebrisTrail(BloodDebrisParticle *particle);
s32 blood_onDebrisFloorHit(BloodDebrisParticle *particle, f32 x, f32 y, f32 z, f32 floorY);
void blood_emitDebrisDrop(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, DebrisBloodTrail *trail);
void blood_billboardFadeGrowThenTint(BillboardParticle *particle);
void blood_billboardFadeGrowTint(BillboardParticle *particle);
void blood_destroyDebris(BloodDebrisParticle *particle);

/* game/Effects/BloodDrip/blooddrip.c */
void *func_151AE7B0(Actor *actor, f32 rate, s16 lifetime, u8 arg3, s32 arg4);
void func_151AE890(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, u8 *arg6);
void func_151AE984(Vec3f *pos, f32 dirX, f32 dirY, f32 dirZ, s32 flag, u8 arg5);
void func_151AEAB4(Vec3f *pos, u8 flag, u8 arg2);
void blood_spawnGroundBurst(Actor *obj, u8 arg1, s32 arg2);
void *func_151AF270(Actor *actor, u8 arg1, s32 arg2);
void func_151AF338(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);
void func_151AF388(Vec3f *position, f32 dx, f32 dy, f32 dz, u8 owner);

/* game/Effects/BloodSpray/bloodspray.c */
void blood_sprayFromBone(Actor *, u8, u8, s32);

/* game/Effects/BloodEmitters/bloodemitters.c */
BloodDrip *blood_attachBleedDrip(Actor *actor, u8 arg1, s32 arg2);
void blood_emitBleedDrop(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, BloodDrip *emitter);
void blood_spawnBleedDrop(Vec3f *position, f32 dirX, f32 dirY, f32 dirZ, u8 arg4, u8 arg5);
BloodDrip *func_151B4FE0(Actor *actor, u8 arg1, s32 arg2);
void func_151B50A4(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, BloodDrip *emitter);
void func_151B50F4(Vec3f *position, f32 dirX, f32 dirY, f32 dirZ, u8 arg4);

#endif
