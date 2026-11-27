#ifndef CONKER_PARTICLE_H
#define CONKER_PARTICLE_H

#include <ultra64.h>
#include "structs.h"
#include "conker/beam.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/sprite.h"

/*
 * Subsystem "particle": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Particle/StyleTimerReset/styletimerreset.c
 *   game/Particle/AreaParticles/areaparticles.c
 *   game/Particle/SmokeEmitter/smokeemitter.c
 *   game/Particle/PointEmitter/pointemitter.c
 *   game/Particle/YawStreak/yawstreak.c
 *   game/Particle/ShatterPieces/shatterpieces.c
 *   game/Particle/MoveHitBurst/movehitburst.c
 *   game/Particle/OwnerEmitter/owneremitter.c
 *   game/Particle/ObjGroundEffects/objgroundeffects.c
 *   game/Particle/PoolEmitter/poolemitter.c
 *   game/Particle/FootDust/footdust.c
 *   game/Particle/PuffSprays/puffsprays.c
 *   game/Particle/DiscEmitter/discemitter.c
 *   game/Particle/SprayObjects/sprayobjects.c
 *   game/Particle/WatcherEmitter/watcheremitter.c
 *   game/Particle/SideDust/sidedust.c
 *   game/Particle/BoneGroundBurst/bonegroundburst.c
 *   game/Particle/DepthTint/depthtint.c
 *   game/Particle/DriftQuad/driftquad.c
 *   game/Particle/YawLaunch/yawlaunch.c
 *   game/Particle/SparkEmitter/sparkemitter.c
 *   game/Particle/FollowEffect/followeffect.c
 *   game/Particle/ImpactStreak/impactstreak.c
 *   game/Particle/EmissionSource/emissionsource.c
 *   game/Particle/ImpactBurst/impactburst.c
 *   game/Particle/FogFlash/fogflash.c
 *   game/Particle/BoneBurst/boneburst.c
 *   game/Player/FlyControl/flycontrol.c
 *   game/Particle/Spiral/spiral.c
 *   game/Particle/TwoSidedEmitter/twosidedemitter.c
 *   game/Particle/RayBeam/raybeam.c
 *   game/Particle/AreaFlash/areaflash.c
 *   game/Particle/SegmentEmitter/segmentemitter.c
 *   game/Particle/SpriteParticle/spriteparticle.c
 *   game/Particle/ModelParticle/modelparticle.c
 *   game/Particle/ParticleModelIds/particlemodelids.c
 *   game/Particle/QuadParticle/quadparticle.c
 *   game/Particle/ChannelParticle/channelparticle.c
 *   game/Particle/FlickerParticle/flickerparticle.c
 *   game/Particle/GroundCluster/groundcluster.c
 *   game/Particle/JitterUpdate/jitterupdate.c
 *   game/Particle/BendTarget/bendtarget.c
 *   game/Particle/JitterBallistic/jitterballistic.c
 *   game/Particle/SpawnShapes/spawnshapes.c
 *   game/Particle/BurstConfigs/burstconfigs.c
 *   game/Particle/StreakParticle/streakparticle.c
 *   game/Particle/SlidingSprite/slidingsprite.c
 *   game/Particle/PuffQuad/puffquad.c
 *   game/Particle/BounceDebris/bouncedebris.c
 *   game/Particle/StyleParticle/styleparticle.c
 *   game/Particle/FloorSprite/floorsprite.c
 *   game/Particle/WakePuff/wakepuff.c
 *   game/Swarm/WaveBob/wavebob.c
 *   game/Particle/SpinDebris/spindebris.c
 *   game/Particle/BoneGlow/boneglow.c
 *   game/Particle/Droplets/droplets.c
 *   game/Particle/BillboardBurst/billboardburst.c
 *   game/Particle/GrowingBillboard/growingbillboard.c
 *   game/Particle/PlacementRing/placementring.c
 *   game/Particle/ActorEmitter/actoremitter.c
 *   game/Particle/ActorEmitterHandlers/actoremitterhandlers.c
 *   game/Particle/AnchorEmitter/anchoremitter.c
 *   game/Particle/AnchorEmitterDestroy/anchoremitterdestroy.c
 *   game/Particle/ActorEmitterData/actoremitterdata.c (data only, linked after the game/Particle/ActorEmitter/actoremitter..game/Effects/BurnDamage/burndamage unit)
 *   game/Particle/Drips/drips.c
 *   game/Particle/DustEmitter/dustemitter.c
 *   game/Particle/BillboardRenew/billboardrenew.c
 *   game/Particle/SideEmitter/sideemitter.c
 *   game/Particle/SegmentBurst/segmentburst.c
 *   game/Particle/FastSpark/fastspark.c
 *   game/Particle/SphereEmitter/sphereemitter.c
 *   game/Particle/RegionEmitter/regionemitter.c
 *   game/Particle/ZoneEmitter/zoneemitter.c
 *   game/Particle/EmitterQuads/emitterquads.c
 *   game/Particle/DripData/dripdata.c (data only, linked after game/Rope/SwingRope/swingrope)
 *   game/Particle/SideSpray/sidespray.c
 *   game/Particle/OwnerSideEmitter/ownersideemitter.c
 *   game/Particle/SoundPulse/soundpulse.c
 *   game/Particle/Pulse/pulse.c
 *   game/Particle/Bob/bob.c
 *   game/Particle/FallingParticle/fallingparticle.c
 *   game/Particle/WobbleStream/wobblestream.c
 *   game/Particle/HitImpact/hitimpact.c
 *   game/Particle/Steam/steam.c
 *   game/Particle/ColorEmitter/coloremitter.c
 *   game/Particle/SmallDebris/smalldebris.c
 */

/* An RGB colour, one byte per channel (game/Particle/SideDust/sidedust.c, game/Particle/BoneGroundBurst/bonegroundburst.c). */
typedef struct Rgb8 {
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
} Rgb8; /* size 0x3 */

/* game/Particle/StyleTimerReset/styletimerreset.c */
void particle_resetStyleEmitterTimers(void);

/* game/Particle/AreaParticles/areaparticles.c */
/* An area effect of level_spawnZoneAreaEffect (game/Level/ZoneSpawn/zonespawn.c): the particles spawned over a zone. */
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

extern const Struct150139ACEntry gAreaEffectDefs[6];

void particle_resetModelList(void);
void particle_spawnAreaParticles(TriggerZone *region, f32 density);
void particle_spawnAreaSprites(TriggerZone *region, f32 density);
void particle_spawnAreaQuads(TriggerZone *region, f32 density);
void func_1500D3AC(TriggerZone *region, f32 density);
void func_1500D8C8(TriggerZone *region, f32 density);
void particle_spawnAreaLargeSprites(TriggerZone *region, f32 density);

/* game/Particle/SmokeEmitter/smokeemitter.c */
/* Per-effect data of the particle_updateSmokeEmitter emitter. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 spawnAccum;
} SmokeEmitterData;

/* Timed effect of particle_updateSmokeEmitter. */
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
    /* 0xC */ f32 rate; /* per unit of gFrameDelta */
} TimeOscillator;

/* A sprite_newScreenSprite sprite that carries a TimeOscillator at 0x70 (the puffs of particle_updateSmokeSource). */
typedef struct {
    /* 0x00 */ ScreenSprite sprite;
    /* 0x70 */ TimeOscillator osc;
} Obj150AFBF4;

/* A projectile created by particle_newModelParticleWithFloor. */
typedef struct {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ EffectObj *trail; /* ribbon effect (trail_spawnRibbon) following the projectile */
} GameDC6B0Projectile;

void particle_hitPlayerOnObjectContact(s32 idA, s32 idB);
void particle_spawnZoneGroundFxNearCamera(void *obj, TriggerZone *range);
void particle_updateSmokeEmitter(SmokeEmitter *emitter);
s32 particle_wobbleSmoke(SpriteParticle *particle, Vec3f *prevPos);
void particle_startGreyscaleScreen(s16 lifetime, u8 owner, s32 list);
void particle_drawGreyscaleScreen(void *effect, u8 *framebuffer);
void particle_updateSmokeSource(GameDC6B0SmokeSource *source);
s32 particle_updateSineOscillator(Obj150AFBF4 *obj);
void particle_spawnBoneSpriteEmitter(s32 object, s32 bone, s32 timer, s32 radiusXZ, u8 owner, s32 list);
void particle_freeCharSpriteEmitters(void);
void particle_fireActorBoneTracer(s32 altBone);
void func_150B003C(s32 unused);
void particle_fireTrailProjectile(Vec3f *from, Vec3f *to, u8 owner, s32 list);
void particle_freeProjectileTrail(GameDC6B0Projectile *projectile);
void particle_destroyTrailProjectile(GameDC6B0Projectile *projectile);
void particle_freeTrailProjectile(GameDC6B0Projectile *projectile);
void particle_spawnObjDebrisBurst(Actor *obj, u8 owner, s32 list);
/* (u8 id, LightSwing *out); declared without a prototype: game/Lighting/LevelLights/levellights.c passes a third argument */
s32 particle_initLightSwingOnObject();

/* game/Particle/PointEmitter/pointemitter.c */
/* Data of the effect spawned by particle_spawnPointEmitter and run by particle_updatePointEmitter. */
typedef struct {
    Vec3f pos;
    f32 accum; /* particles owed; one is spawned per whole unit */
} PointEmitterData;

/* Timed effect of particle_updatePointEmitter. */
typedef struct {
    TimedEffect base;
    PointEmitterData data;
} PointEmitter;

void particle_levelHook29(Camera *camera);
void particle_spawnPointEmitter(EffectHeader *spawner, Vec3f *pos, f32 top, u8 noGround);
void particle_updatePointEmitter(PointEmitter *effect);
void particle_levelHook21(Camera *camera);

/* game/Particle/YawStreak/yawstreak.c */
s32 particle_selectObjGroundBurst(s32 kind, Actor *actor);
void particle_spawnObjGroundBurst(Actor *obj, s32 arg1, s32 arg2);
s32 particle_spawnYawStreak(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s32 yaw, s32 baseAngle, f32 radius,
                  s32 arg11, s32 arg12, void *data, u8 owner);

/* game/Particle/ShatterPieces/shatterpieces.c */
/* Wall-collision query of game/Particle/ModelParticle/modelparticle.c (only its position is read here). */
typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ Vec3f pos;
} CollisionProbe;

/* A particle spawned by particle_spawnAxisQuadParticle: the 0x110-byte particle, its 0x18-byte quad axes, then the
 * caller's motion block. */
typedef struct {
    /* 0x000 */ u8 pad0[0x128];
    /* 0x128 */ f32 motion[2];
} AxisParticle;

/* A live particle of game/Particle/QuadParticle/quadparticle.c whose motion block is a fall speed (particle_fallAndGrow); the update callbacks
 * of gQuadParticleUpdate1Funcs / gQuadParticleUpdate2Funcs get one. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ f32 fallSpeed;     /* first word of the per-behaviour motion block after the particle */
} FallGrowParticle;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 list; /* EffectHeader byte 1: the object list */
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner; /* EffectHeader.owner */
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

s32 particle_floorShatter(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, void *surface);
s32 particle_wallShatter(ModelParticle *particle, CollisionProbe *probe, f32 x, f32 y, f32 z);
s32 particle_fallAndGrow(FallGrowParticle *particle);
s32 particle_fadeFallGrowParticle(FallGrowParticle *particle);
s32 particle_fadeQuadSpriteByLife(QuadSprite *sprite);
s32 particle_updateKickable(ModelParticle *particle);
s32 particle_buildLookAtMatrix(Mtx *mtx, ModelParticle *particle);
void particle_spawnFlatMark(Obj150B9D8C *obj);
extern Actor *gKickableLookAtActor;
extern Vec3f gKickableLookAtPos;
extern Actor *gKickablePusher;

/* game/Particle/MoveHitBurst/movehitburst.c */
/* Per-behaviour motion block stored right after a particle. */
typedef struct {
    /* 0x00 */ Vec3f vel;
    /* 0x0C */ f32 rotVelX;
    /* 0x10 */ f32 rotVelY;
    /* 0x14 */ f32 minY;           /* the particle expires below this height */
} FxParticleMotion;

/* A live particle of game/Particle/QuadParticle/quadparticle.c whose motion block is a velocity and spin (particle_moveTumbling); the update
 * callbacks of gQuadParticleUpdate1Funcs / gQuadParticleUpdate2Funcs get one. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ FxParticleMotion motion;
} TumblingParticle;

void particle_spawnMoveHitBurst(Actor *actor, f32 prevX, f32 prevZ, u8 owner, s32 list);
s32 particle_fadeMoveHitSprite(QuadSprite *sprite);
s32 particle_moveTumbling(TumblingParticle *particle);
s32 particle_fadeTumbling(TumblingParticle *particle);

/* game/Particle/OwnerEmitter/owneremitter.c */
/* Per-effect data: the object the effect is attached to and a spawn accumulator. */
typedef struct {
    /* 0x0 */ Actor *owner;
    /* 0x4 */ u8 ownerId; // owner's unique_id when the effect was created
    /* 0x8 */ f32 spawnAccum;
} EffectE7970Data; // size 0xC

/* Effect slot as returned by effect_newTimedEffect (a TimedEffect); the effect data lives at 0x28. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EffectE7970Data data;
} EffectE7970;

void particle_attachOwnerEmitter(Actor *owner, u8 fxOwner, s32 fxList);
void particle_updateOwnerEmitter(EffectE7970 *effect);
void particle_onOwnerEmitterEvent(EffectE7970 *effect, s32 event, u8 eventType);

/* game/Particle/ObjGroundEffects/objgroundeffects.c */
s32 particle_getObjGroundPos(Vec3f *pos, Actor *obj, CollisionHit *hit, s32 arg3);
s32 particle_selectObjEffects(s32 kind, Actor *actor);
void particle_spawnObjEffects(Actor *obj, s32 arg1, s32 arg2);

/* game/Particle/PoolEmitter/poolemitter.c */
/* Live particle spawned by an emitter (see particle_newSpriteParticleWithFloor). */
typedef struct PoolParticle PoolParticle;

/* One entry of an emitter's slot pool; slots are chained by index, -1 ends a chain. */
typedef struct {
    /* 0x00 */ PoolParticle *particle;
    /* 0x04 */ s16 next;
    /* 0x06 */ s16 pad6;
} PoolEmitterSlot;

/* Describes an emitter; particle_newPoolEmitter copies it into the emitter. */
typedef struct {
    /* 0x00 */ Vec3f *source;     /* followed when flags & 2 */
    /* 0x04 */ f32 unk4;          /* effect_spawnActorPoolEmitter stores 0.0f */
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

void particle_onPoolEmitterEvent(PoolEmitter *emitter, s32 event, u8 eventType);
void particle_destroyPoolEmitter(PoolEmitter *emitter);
void particle_freePoolEmitter(PoolEmitter *emitter);
void particle_freePoolParticles(PoolEmitter *emitter);
PoolEmitter *particle_newPoolEmitter(PoolEmitterDesc *desc, s32 owner, s32 list);
void particle_updatePoolEmitter(PoolEmitter *emitter);
void particle_releasePoolSlot(PoolParticle *particle);

/* game/Particle/FootDust/footdust.c */
void particle_spawnFootDust(Actor *actor, u8 foot);

/* game/Particle/PuffSprays/puffsprays.c */
void func_150C1F60(u8 owner);
void func_150C2290(u8 owner);
void func_150C2424(u8 owner);
s32 particle_fadeMotionTrailByLife(MotionTrail *trail);
s32 func_150C2558(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s32 angle, s32 baseAngle, f32 radius,
                  s32 argB, s32 argC, void *data, u8 owner);
s32 func_150C2700(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s32 angle, s32 baseAngle, f32 radius,
                  s32 argB, s32 argC, void *data, u8 owner);
void particle_startKind6ShortTrail(f32 *x, f32 *y, f32 *z, s16 delay, s32 owner, s32 list);
void particle_emitKind6TrailSpray(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, Struct150C29F0 *emitter);

/* game/Particle/DiscEmitter/discemitter.c */
void particle_emitDiscParticle(Struct150C29F0 *emitter);
void particle_updateOriginRingEmitter(Struct150C29F0 *emitter);
s32 particle_shrinkBallisticChannelParticle(ChannelParticle *particle);
s32 particle_expireRingParticleOffBearing(ChannelParticle *particle);

/* game/Particle/SprayObjects/sprayobjects.c */
Gfx *func_150C3160(Gfx *gdl, Actor *obj);
void func_150C3230(Actor *obj);
void func_150C3574(Actor *obj);
void func_150C3994(Actor *obj);
s32 particle_decodeTamperAddress(s32 value);
void func_150C3D5C(Actor *obj);

/* game/Particle/WatcherEmitter/watcheremitter.c */
/* The objects an emitter keeps alive; each clears its own slot when it is destroyed. */
// matching: volatile makes IDO reload the slot after the NULL test (particle_keepFlickerSprite, particle_keepWatcherEmitter)
typedef struct {
    /* 0x0 */ FlickerSprite *volatile particle;
    /* 0x4 */ WatcherEmitter *volatile linked;
} GameF3270Children;

/* A character watcher with GameF3270Children as extra data. */
typedef struct GameF3270Emitter {
    /* 0x00 */ CharacterWatcher watcher;
    /* 0x58 */ GameF3270Children children;
} GameF3270Emitter;

void particle_keepFlickerSprite(GameF3270Emitter *emitter);
s8 particle_updateFlickerSprite(FlickerSprite *particle);
s32 particle_detachFlickerSprite(FlickerSprite *particle);
void particle_keepWatcherEmitter(GameF3270Emitter *emitter);
WatcherEmitter *particle_spawnWatcherEmitter(Actor *actor, GameF3270Emitter *emitter);
void particle_spawnWatcherParticle(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, WatcherEmitter *source);
s32 particle_updateWatcherLink(WatcherEmitter *obj);
void particle_destroyWatcherLink(WatcherEmitter *obj);
void particle_freeWatcherLink(WatcherEmitter *obj);
FlickerSprite *particle_spawnFlickerSprite(GameF3270Emitter *emitter);

/* game/Particle/SideDust/sidedust.c */
/* Growth applied by particle_updateDustPuff, stored in the dust particle at 0x128. */
typedef struct {
    /* 0x00 */ s16 growTime; // frames left to grow
    /* 0x04 */ f32 growRate; // size gained per frame
} DustGrowth;

/* The dust puff particle particle_spawnSideDust spawns (particle_spawnAxisQuadParticleNoModel, a DefParticleHead view); updated by particle_updateDustPuff. */
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

/* Particle handled by particle_startDustFadeOut. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0xC];
    /* 0x1C */ s16 timer;
    /* 0x1E */ u8 pad1E[0x3A];
    /* 0x58 */ s32 flags;
} FadeoutParticle;

s32 particle_getDustSidePos(Vec3f *pos, Actor *actor, CollisionHit *ground, u8 side);
s32 particle_selectSideDust(s32 kind, Actor *actor);
void particle_spawnSideDust(Actor *actor, s32 side, s32 arg2);
s32 particle_fadeSideDustSprite(QuadSprite *sprite);
s32 particle_spawnYawModelParticle(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s16 angle, s32 baseAngle, f32 radius,
                  s32 arg11, s32 arg12, void *data, u8 owner);
s32 particle_updateDustPuff(DustPuff *dust);
s32 particle_spawnYawSpark(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s16 angle, s32 baseAngle, f32 radius,
                  s32 argB, s32 argC, void *data, u8 owner);
s32 particle_dampSparkXZ(SparkParticle *particle, Vec3f *prevPos);
s32 particle_moveBallisticConstAccel(ModelParticle *particle);
void particle_startDustFadeOut(FadeoutParticle *particle);
extern struct Queue *gEffectQueue;

/* game/Particle/BoneGroundBurst/bonegroundburst.c */
/* Per-particle data copied to 0x128 of a particle by particle_spawnBoneGroundBurst. */
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
    /* 0xA8 */ f32 decay; /* set by particle_spawnYawDampedSprite */
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

void particle_spawnBoneGroundBurst(Actor *owner, u8 index, s32 fxList, u8 fxOwner);
s32 particle_fadeInGrow(GrowParticle *p);
s32 particle_spawnYawDampedSprite(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s16 yaw, s32 baseAngle, f32 radius,
                  s32 arg11, s32 arg12, void *data, u8 owner);
s32 particle_dampYawSpriteXZ(DecayParticle *p, Vec3f *prevPos);
s32 particle_spawnYawTumbler(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s16 yaw, s32 baseAngle, f32 radius,
                  s32 arg11, s32 arg12, void *data, u8 owner);
s32 particle_moveTumbler(TumbleParticle *p);

/* game/Particle/DepthTint/depthtint.c */
void particle_getDepthTint(f32 z, f32 *r, f32 *g, f32 *b);

/* game/Particle/DriftQuad/driftquad.c */
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

/* Effect instance created by particle_newQuadParticleWhite from an ParticleDef. */
typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ f32 halfWidth;
    /* 0x030 */ f32 halfHeight;
    /* 0x034 */ Vec3f pos;
    /* 0x040 */ Vec3f dir;   /* rotation angles for math_mtxRotateRPYF */
    /* 0x04C */ Vec3f sway;  /* offset added to pos while falling */
    /* 0x058 */ u8 pad58[0x68];
    /* 0x0C0 */ Vtx_t template[4];
    /* 0x100 */ Vtx_t *buffers[4]; /* each holds two copies of the 4-vertex quad */
    /* 0x110 */ QuadDriftMotion drift;
} DriftQuad;

typedef struct {
    f32 pending; /* bursts owed; one is spawned per whole unit */
    f32 rate;    /* 0..1, set by particle_setDriftBurstRate */
} BurstTimer;

/* Timed effect driven by particle_updateDriftBurstEmitter (a gTimedEffectTickFuncs tick callback); gDriftBurstEmitter points at one. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BurstTimer timer;
} DriftBurstEmitter;

extern DriftBurstEmitter *gDriftBurstEmitter;

DriftQuad *particle_spawnDriftQuad(DriftSpawn *spawn, u8 owner, u8 list);
s32 particle_updateDriftQuad(DriftQuad *effect);
Vtx_t *particle_buildDriftQuad(DriftQuad *effect, s16 bufferIndex);
void particle_updateDriftBurstEmitter(DriftBurstEmitter *emitter);
void particle_setDriftBurstRate(s32 intensity);

/* game/Particle/YawLaunch/yawlaunch.c */
void particle_spawnGroundEffects(Actor *actor, u8 owner, s32 list);
s32 particle_spawnYawColorSprite(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s16 yaw, s32 baseAngle, f32 radius,
                  s32 arg11, s32 arg12, void *data, u8 owner);
s32 particle_spawnYawSpinModel(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s16 yaw, s32 baseAngle, f32 radius,
                  s32 arg11, s32 arg12, void *data, u8 owner);

/* game/Particle/SparkEmitter/sparkemitter.c */
/* Object data (at +0x28) of a trigger that spawns spark emitters. */
typedef struct EmitterTrigger {
    s16 *record;
    s32 minDelay;
    s32 delayRange;
    s32 countdown;
    MtxF matrix;
    u8 params[2]; // [0] bit 0: spawn periodically; [1]: id matched by event 0x34
                  // matching: one array; two u8 fields swap the compare's registers in particle_onSparkTrigger
} EmitterTrigger;

/* Object data (at +0x28) of a spark emitter: created by particle_spawnSparkEmitter, updated by particle_updateSparkEmitter. */
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

/* Timed effect (effect_newTimedEffect) of this file, with an emitter trigger or a spark emitter (particle_spawnSparkEmitter) as data. */
typedef struct SparkEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ union {
        EmitterTrigger trigger;
        RingSparkEmitter emitter;
    } data;
} SparkEffect;

/* Motion data (at +0x110) of a spark particle spawned by particle_updateSparkEmitter. */
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
    /* 0x000 */ DefParticleHead head; /* head.def.alpha fades out with the remaining life */
    /* 0x110 */ SparkMotion motion;
} RingSparkParticle;

typedef struct GameFFBA0Beam {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ f32 texture[2];
} GameFFBA0Beam;

void particle_updateSparkSpawner(SparkEffect *obj);
void particle_spawnSparkEmitter(s16 *record, MtxF matrix, u8 owner, s32 list);
void particle_updateSparkEmitter(SparkEffect *obj);
Vtx *particle_buildSparkBeamQuad(GameFFBA0Beam *beam, s16 index);
s32 particle_swaySpark(RingSparkParticle *particle);
void particle_onSparkTrigger(SparkEffect *obj, u8 *msg, u8 event);

/* game/Particle/FollowEffect/followeffect.c */
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

/* Per-particle data of the drifting particles that follow an object (particle_updateFollowDrift). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 drag; /* passed to particle_applyDrag with the velocity */
} FollowDriftData;

/* Per-particle data of the growing, fading particle that follows an object (particle_updateFollowGrow). */
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

/* Live particle, as created by particle_newSpriteParticleWithFloor from a ParticleDesc. */
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

void particle_attachFollowEffect(Actor *obj, u8 owner, s32 list);
void particle_updateFollowEffect(FollowEffect *effect);
void particle_onFollowEffectMsg(FollowEffect *effect, ActorMsg *msg, u8 type);
void particle_destroyFollowEffect(TimedEffect *effect);
void particle_freeFollowEffect(TimedEffect *effect);
void particle_spawnBreakup(Actor *obj, u8 owner, s32 list);
void particle_spawnRandomGrowFade(Vec3f *pos, u8 owner, s32 list);
void particle_spawnRandomSwell(Vec3f *pos, u8 owner, s32 list);
void func_150D64E8(Struct150D64E8 *info, f32 *outValue);
void particle_spawnRandomSurfaceMark(CollisionTri *surface, Vec3f *pos, u8 owner, s32 list);
void particle_flashScreenWhite(u8 view, u8 owner, s32 list);
void particle_spawnFollowDrift(Actor *obj, u8 owner, s32 list);
void particle_getPointAboveObj(Actor *obj, Vec3f *out);
s32 particle_updateFollowDrift(FollowParticle *particle, Vec3f *prevPos);
void particle_onFollowDriftMsg(FollowParticle *particle, ActorMsg *msg, u8 type);
s32 particle_updateFollowGrow(FollowParticle *particle, Vec3f *prevPos);
void particle_onFollowGrowMsg(FollowParticle *particle, ActorMsg *msg, u8 type);

/* game/Particle/ImpactStreak/impactstreak.c */
/* Hit record copied to Streak.impact (+0x110) when a streak is spawned. */
typedef struct {
    /* 0x00 */ u8 flags; /* bit 0: the streak's path hits a surface */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Vec3f point;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ CollisionTri tri; /* the triangle hit */
    /* 0x26 */ u8 pad26[2];
    /* 0x28 */ f32 side; /* side of the surface the streak starts on (+1 / -1) */
    /* 0x2C */ s8 callback; /* index into gStreakImpactFuncs, -1 for none */
    /* 0x2D */ u8 pad2D[3];
} StreakImpact; /* size 0x30 */

/* The particle object spawned by particle_spawnImpactStreak (kind 0x41). */
typedef struct {
    /* 0x00 */ DefParticleHead head; /* head.def.height: length, head.def.rot: direction, head.def.scale.y: speed, head.def.unk44: alpha */
    /* 0x110 */ StreakImpact impact;
} Streak;

void *particle_spawnImpactStreak(Vec3f *start, Vec3f *dir, f32 scale, f32 width, f32 height, u8 owner, s32 list, u8 checkHit, u8 alpha,
                    s16 life, s8 callback);
s8 particle_moveImpactStreak(Streak *streak);
void *particle_buildImpactStreakQuad(Streak *streak, s16 viewIdx);
s32 particle_fadeImpactStreak(Streak *streak);
s32 particle_spawnStreakImpact(Streak *streak);

/* game/Particle/EmissionSource/emissionsource.c */
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

/* Extra data of the particles particle_updateRandomEmitter spawns, stored at 0xA8 of each particle: effects.h WobbleData
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

/* Extra data of the particles particle_updateFallingDebrisSpawner spawns, stored at 0x170 of each particle. */
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

void particle_startSourceEmission(Actor *actor, s32 playerIndex, TriggerZone *zone, u8 sourceIndex);
void particle_updateSourceEmission(Game113D60Effect *effect);
void particle_randomPointOnTwoSegments(Vec3f *out);
void particle_randomPointInSourceZones(Vec3f *out);
void particle_randomPointOnSixSegments(Vec3f *out);
void particle_randomPointAroundActorNode(Vec3f *out, Actor *actor);
void particle_getEmissionActorPos(Vec3f *out, Actor *actor);
void particle_aimEmissionAlongDelta(Vec3f *start, Vec3f *end, Vec3f *delta, Game113D60Emission *emission);
void particle_aimEmissionTilted(Vec3f *start, Vec3f *end, Vec3f *delta, Game113D60Emission *emission);
/* (u8 playerIndex, u8 owner, s32 list); declared without a prototype: game/Zone/ZoneState/zonestate.c passes an s32 playerIndex without narrowing it */
void func_150E7290();
void particle_spawnScreenSprite(Vec2f *position, f32 size, s16 timer, u8 flags, u8 alpha, u8 colorLevel, s16 fadeStart, s16 fadeRate, s32 *ids,
                   s32 idCount, u8 owner, s32 list);
void particle_spawnScreenEdgeSprite(f32 size, s16 timer, u8 flags, u8 alpha, u8 colorLevel, s16 fadeStart, s16 fadeRate, s32 *ids, s32 idCount,
                   u8 owner, s32 list);
TimedEffect *particle_startScreenBurst(s16 count, f32 rate, u8 owner, s32 list);
void particle_updateScreenBurst(Game113D60BurstEffect *effect);
/* (u8 index, u8 owner, s32 list); declared without a prototype: the K&R definition changes with one */
void particle_spawnFlashAndJetSprayAtPoint();
void particle_startRandomEmitter(const Vec3f *position, s16 duration, u8 owner, s32 list);
void particle_updateRandomEmitter(Game113D60EmitterEffect *effect);
s32 particle_wobbleRandomEmitterParticle(Game113D60Particle *particle, Vec3f *prevPos);
void particle_startShakeTimer(void);
void particle_updateShakeTimer(Game113D60TimerEffect *effect);
/* (void); declared without a prototype: particle_updateShakeTimer passes the effect */
void particle_fireShake();
void particle_startDustRegionSpawner(void);
void particle_updateDustRegionSpawner(Game113D60RegionSpawnerEffect *effect);
void particle_updateFallingDustSpawner(Game113D60PointSpawnerEffect *effect);
void particle_startDebrisRegionSpawner(void);
void particle_updateDebrisRegionSpawner(Game113D60RegionSpawnerEffect *effect);
void particle_updateFallingDebrisSpawner(Game113D60PointSpawnerEffect *effect);
s32 particle_updateFallingDebris(Game113D60Debris *debris);
extern TriggerZone *gEmissionSourceZones[2]; /* end points of source 1; level_registerEmissionSourceZone registers them (by unk18) */

/* game/Particle/ImpactBurst/impactburst.c */
/* Spawner state at +0x28 of a burst emitter. */
typedef struct {
    /* 0x00 */ TriggerZone *source; // where math_randomPointInZone places each burst
    /* 0x04 */ f32 rateBase;    // bursts per unit of gFrameDelta
    /* 0x08 */ f32 rateRange;
    /* 0x0C */ f32 scaleBase;   // size scale passed to particle_spawnImpactBurst
    /* 0x10 */ f32 scaleRange;
    /* 0x14 */ f32 spawnAccum;
} BurstSpawner;

/* Timed effect driven by particle_updateRandomBurstEmitter (a gTimedEffectTickFuncs tick callback). */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BurstSpawner spawner;
} RandomBurstEmitter;

void particle_spawnImpactBurst(Vec3f *pos, Vec3f *vel, f32 scale, u8 owner, s32 list);
void particle_updateRandomBurstEmitter(RandomBurstEmitter *emitter);

/* game/Particle/FogFlash/fogflash.c */
/* Data of each effect the emitter spawns. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 w;
} EmittedPoint;

/* Data of the particle source effect (particle_updateSourceEmitter). */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 accumulator;
} ParticleSourceData;

/* Data of the fog flash effect (particle_updateFogFlash). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 r;
    /* 0x06 */ u8 g;
    /* 0x07 */ u8 b;
    /* 0x08 */ u8 a;
    /* 0x09 */ u8 rate;
} FogFlashData;

/* An effect (a TimedEffect with per-effect data at 0x28), as created by effect_newTimedEffect. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ union {
        ShapeEmitter emitter;
        EmittedPoint point;
        ParticleSourceData source;
        FogFlashData fog;
    } data;
} EmitterEffect;

void particle_updateShapeEmitter(EmitterEffect *effect);
void particle_updateSourceEmitter(EmitterEffect *effect);
void particle_attachFogFlash(Actor *obj, u8 r, u8 g, u8 b, u8 a, u8 rate, s16 duration, u8 owner, s32 list);
void particle_onFogFlashEvent(EmitterEffect *effect, s32 event, u8 eventType);
void particle_updateFogFlash(EmitterEffect *effect);
void particle_spawnFloorImpactGreenFlash(Actor *obj, u8 owner, s32 list);
void particle_levelHook17(Camera *obj);

/* game/Particle/BoneBurst/boneburst.c */
/* State the emitter object keeps at +0x28: particle_spawnBoneBurstEmitter fills it, particle_updateBoneBurstEmitter updates it each frame. */
typedef struct {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetId;   // target->unique_id when attached; a mismatch means the slot was reused
    /* 0x05 */ u8 prevTargetIndex; // target->targetIndex seen last frame
    /* 0x08 */ f32 timer;     // one particle is emitted per 1.0 accumulated
    /* 0x0C */ s8 count;      // particles left in the current burst
} BoneBurstState;

/* The emitter object spawned by particle_spawnBoneBurstEmitter. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoneBurstState state;
} BoneBurstEmitter;

/* State each emitted particle keeps after its DefParticleHead (copied in by particle_updateBoneBurstEmitter, updated by particle_updateBoneBurstParticle). */
typedef struct {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetId;
    /* 0x08 */ f32 age;
    /* 0x0C */ f32 lifetime;
    /* 0x10 */ f32 growth;    // width = height = growth * sqrtf(age)
    /* 0x14 */ f32 fadeFrom;  // def.unk44 = fadeFrom - fadeRate * age * age
    /* 0x18 */ f32 fadeRate;  // fadeFrom / (lifetime * lifetime): def.unk44 reaches 0 at lifetime
    /* 0x1C */ Vec3f axisU; // quad axes for particle_buildQuadFromAxes
    /* 0x28 */ Vec3f axisV;
} ParticleState;

/* A particle spawned by particle_newQuadParticle. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ ParticleState state;
} BoneBurstParticle;

void func_150F2A60(s32 unused);
void particle_spawnBoneBurstEmitter(Actor *target);
void particle_updateBoneBurstEmitter(BoneBurstEmitter *emitter);
void particle_onBoneBurstEmitterEvent(BoneBurstEmitter *emitter, s32 event, u8 eventType);
void particle_onBoneBurstParticleEvent(BoneBurstParticle *particle, s32 event, u8 eventType);
s32 particle_updateBoneBurstParticle(BoneBurstParticle *particle);
void particle_buildBoneBurstQuad(BoneBurstParticle *particle, s16 viewport);
void particle_cullObjectByCameraZ(WorldObject *obj);
void func_150F33F8(s32 player);
extern ZoneNode *D_800D9A40; // zone at (0x1500, 0x700); func_150F33F8 sets its unk0 bits per player

/* game/Player/FlyControl/flycontrol.c */
f32 player_getFlyingAnimSpeed(Actor *obj, f32 pitch);
void player_updateFlying(Actor *actor);
void player_levelHook24(Camera *actor);

/* game/Particle/Spiral/spiral.c */
/* Per-object state of the effect spawned by particle_spawnSpiral: it circles a centre that moves
 * from one point towards another (updated by particle_moveSpiral). */
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

/* An object spawned by particle_newModelParticleWithFloor with a SpiralState. */
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
    /* 0x071 */ u8 updateFn;
    /* 0x072 */ u8 pad72[0x16];
    /* 0x088 */ u32 sound; /* sound id in the low half, playing handle in the high half */
    /* 0x08C */ u8 pad8C[0xE4];
    /* 0x170 */ SpiralState spiral;
} SpiralParticle;

/* Per-object state of the emitter started by particle_startCameraSpiralEmitter (updated by particle_updateCameraSpiralEmitter). */
typedef struct {
    /* 0x00 */ Camera *camera;
    /* 0x04 */ f32 pending; /* spirals owed; one is spawned per whole unit */
    /* 0x08 */ s8 condition; /* index into gCameraEmitterConditions, or -1 for none */
    /* 0x09 */ u8 variants; /* index into gSpiralVariantLists */
    /* 0x0A */ u8 padA[2];
} CameraEmitterData;

/* The emitter effect object created by effect_newTimedEffect. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ CameraEmitterData data;
} CameraEmitter;

extern VariantList gSpiralVariantLists[2];

void particle_spawnSpiral(Vec3f *from, Vec3f *to, u8 id, f32 speed, f32 invSpeed, f32 scale, u8 fadeNear, s32 sound, u8 owner,
                   s32 list);
s32 particle_moveSpiral(SpiralParticle *obj);
s32 particle_updateSpiralFadeSound(SpiralParticle *obj);
void particle_onSpiralEvent(SpiralParticle *obj, s32 event, u8 eventType);
void particle_startCameraSpiralEmitter(Camera *camera, s8 condition, u8 variants, u8 owner, s32 list);
void particle_updateCameraSpiralEmitter(CameraEmitter *emitter);
extern const s16 gSpiralVariant0Ids[3];
extern const f32 gSpiralVariant0Scales[3];
extern const s16 gSpiralVariant1Ids[16];
extern const f32 gSpiralVariant1Scales[16];

/* game/Particle/TwoSidedEmitter/twosidedemitter.c */
/* State block that particle_spawnTwoSidedEmitter copies into its spawned object at offset 0x28. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerUniqueId;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 spawnTimer;
    /* 0x0C */ u8 side;
    /* 0x0D */ u8 skipTarget;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ CamWobble *wobble; /* the other actor's camera wobble (camera_createWobble), or NULL */
    /* 0x14 */ s16 rumbleTimer;
    /* 0x16 */ s8 otherCamera; // the other actor's camera number (CNumber), or -1
    /* 0x17 */ u8 pad17;
    /* 0x18 */ Actor *other;
    /* 0x1C */ u8 otherUniqueId;
    /* 0x1D */ u8 ownerAnimSeq;
    /* 0x1E */ u8 pad1E[2];
} TwoSidedEmitterState; // size 0x20

/* The object spawned by particle_spawnTwoSidedEmitter. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ TwoSidedEmitterState state;
} TwoSidedEmitter;

void particle_spawnSideEffect(Actor *obj, u8 side, f32 timeOffset, u8 skipTarget, u8 owner, s32 list);
void particle_spawnHitEffectsHideOwnerSparks(struct Game1EF500Trail *trail); /* an expire handler of D_8008FBD0 */
void particle_spawnTwoSidedEmitter(Actor *owner, Actor *other, u8 arg2, u8 arg3);
void particle_updateTwoSidedEmitter(TwoSidedEmitter *obj);
void particle_onTwoSidedEmitterMsg(TwoSidedEmitter *obj, ActorMsg *msg, u8 msgType);

/* game/Particle/RayBeam/raybeam.c */
typedef struct Struct15101F50 Struct15101F50;
typedef struct Owner151025F4 Owner151025F4;
RayBeam *particle_spawnRayBeam(Actor *owner, const Vec3f *vec1, const Vec3f *vec2, u8 bone, s16 lifetime, u8 r, u8 g, u8 b, f32 arg8,
                              f32 scrollBase, f32 scrollRange, f32 argB, u8 aimed, u8 fxOwner, s32 fxList);
void particle_destroyRayBeam(RayBeam *beam);
void particle_freeRayBeam(RayBeam *beam);
void particle_destroyBeamHitFx(BeamHitFx *fx);
void particle_freeBeamHitFx(BeamHitFx *fx);
void particle_destroyBeamEndPoint(LightEffect *endPoint);
void particle_freeBeamEndPoint(LightEffect *endPoint);
void particle_destroyBeamStartFx(BeamStartFx *fx);
void particle_freeBeamStartFx(BeamStartFx *fx);
void particle_destroyBeamLine(BeamLine *line);
void particle_freeBeamLine(BeamLine *line);
void particle_destroyBeamEndFx(BeamEndFx *fx);
void particle_freeBeamEndFx(BeamEndFx *fx);
s8 particle_updateRayBeam(RayBeam *beam);
Vtx *particle_buildRayBeamQuad(RayBeam *beam, s16 viewport);
Vtx *particle_buildCentredAxisQuad(Struct15101F50 *particle, s16 viewport);
s32 particle_flickerRayBeam(RayBeam *beam);
s32 particle_updateWobbleOffset(Owner151025F4 *obj, Vec3f *prevPos);
RayBeam *particle_startObjRayBeam(Actor *obj, s16 lifetime, u8 kind, u8 aimed, u8 owner, s32 list);
void particle_stopObjEffect(Actor *actor);
s32 func_1510281C(BeamStartFx *sprite, s16 view);
s32 func_15102884(BeamEndFx *fx, s32 view);
s32 func_151028AC(BeamLine *line, s16 view);

/* game/Particle/AreaFlash/areaflash.c */
/*
 * Per-particle data of the particles particle_updateAreaEmitter spawns (update function 0x18, particle_updateAreaFlash).
 * The animation frame steps 0 -> 1 -> 2 at the start of the particle's life and back at the end.
 */
typedef struct {
    /* 0x00 */ f32 life;       /* remaining; the particle dies below 0 */
    /* 0x04 */ f32 lifetime;   /* initial life */
    /* 0x08 */ f32 frame1Time; /* time spent below frame 2 at either end */
    /* 0x0C */ f32 frame0Time; /* time spent at frame 0 at either end */
    /* 0x10 */ f32 flashTimer; /* counts down from 11 while flashing, 0 otherwise */
} FlashParticleData;

/* Particle created by particle_newSpriteParticleWithFloor (only the fields used here, see game/Particle/SpriteParticle/spriteparticle.c). */
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

/* Per-effect data of the particle_updateAreaEmitter emitter. */
typedef struct {
    /* 0x00 */ f32 spawnAccum;
    /* 0x04 */ EmitterArea *area;
    /* 0x08 */ u8 flags;
} AreaEmitterData;

/* Timed effect of particle_updateAreaEmitter. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ AreaEmitterData data;
} AreaEmitter;

void particle_updateAreaEmitter(AreaEmitter *emitter);
s32 particle_updateAreaFlash(AreaFlashParticle *particle, Vec3f *prevPos);
void particle_onAreaEmitterEvent(AreaEmitter *emitter, s32 event, u8 eventType);

/* game/Particle/SegmentEmitter/segmentemitter.c */
/* Data of effect 0x1A (particle_startSegmentEmitter / particle_updateSegmentEmitter). */
typedef struct {
    /* 0x00 */ Actor *object;
    /* 0x04 */ u8 objectId; /* object->unique_id when attached; a mismatch means the slot was reused */
    /* 0x08 */ f32 spawnAccum;
    /* 0x0C */ f32 spawnRate;
    /* 0x10 */ s8 tickFunc; /* index into gSegmentEmitterTickFuncs, -1: none */
    /* 0x11 */ s8 emitFunc; /* index into gSegmentEmitFuncs, -1: none */
} ObjectEmitterData; /* size 0x14 */

/* Data of effect 0x1B (particle_startMatrixSegmentEmitter / particle_updateMatrixSegmentEmitter). */
typedef struct {
    /* 0x00 */ WorldObject *object; /* gfx_getObjectModelMtx builds the emitter's matrix from it */
    /* 0x04 */ f32 spawnAccum;
    /* 0x08 */ f32 spawnRate;
    /* 0x0C */ s8 tickFunc; /* index into gSegmentEmitterTickFuncs, -1: none */
    /* 0x0D */ s8 emitFunc; /* index into gSegmentEmitFuncs, -1: none */
} MatrixEmitterData; /* size 0x10 */

/* Data of effect 0x1C (particle_startPointPairEmitter / particle_updatePointPairEmitter). */
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

TimedEffect *particle_startSegmentEmitter(Actor *object, s16 duration, s8 tickFunc, s8 emitFunc, f32 spawnRate, s32 extraSize,
                           u8 owner, s32 list);
void particle_updateSegmentEmitter(Effect1368C0 *effect);
void particle_emitAlongDir(Effect1368C0 *effect, Vec3f *point, Vec3f *start, Vec3f *end, Vec3f *tip, Vec3f *dir);
void particle_emitDebris(Effect1368C0 *effect, Vec3f *point, Vec3f *start, Vec3f *end, Vec3f *tip, Vec3f *dir);
TimedEffect *particle_startMatrixSegmentEmitter(void *object, s16 duration, s8 tickFunc, s8 emitFunc, f32 spawnRate, s32 extraSize, u8 owner,
                           s32 list);
void particle_updateMatrixSegmentEmitter(Effect1368C0 *effect);
TimedEffect *particle_startPointPairEmitter(Actor *object, s16 duration, u8 owner, s32 list);
void particle_updatePointPairEmitter(Effect1368C0 *effect);
void particle_onSegmentEmitterSwap(Effect1368C0 *effect, ActorSwapMsg *msg, u8 msgType);
void particle_onPointPairEmitterSwap(Effect1368C0 *effect, ActorSwapMsg *msg, u8 msgType);

/* game/Particle/SpriteParticle/spriteparticle.c */
extern Gfx gSpriteParticleSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gSpriteParticleResetDl[]; /* render teardown (object type table gObjectTypes) */
/* Each returns a SpriteParticle followed by dataSize bytes of its kind's data (the kind's own type). */
void *particle_newSpriteParticleWithFloor(ParticleDesc *desc, u8 kind, CollisionHit *ground, s32 dataSize, u8 owner, s32 list);
void *particle_newOwnedSpriteParticle(ParticleDesc *desc, u8 kind, s32 dataSize, u8 owner, s32 list);
void *particle_newSpriteParticle(ParticleDesc *desc, u8 kind, s32 dataSize);
void particle_updateSpriteParticle(SpriteParticle *p);
Gfx *particle_drawSpriteParticle(Gfx *gfx, SpriteParticle *p, s16 viewIdx);
void particle_getSpritePrimColor(s16 *r, s16 *g, s16 *b, s16 *a, SpriteParticle *p);
void particle_getSpriteEnvColor(s16 *r, s16 *g, s16 *b, s16 *a, SpriteParticle *p);
void particle_getSpriteColors(s16 *primR, s16 *primG, s16 *primB, s16 *primA, s16 *envR, s16 *envG, s16 *envB, s16 *envA,
                   SpriteParticle *p);
s32 particle_updateDragY(SpriteParticle *p, Vec3f *prevPos);
s32 particle_updateDragXZ(SpriteParticle *p, Vec3f *prevPos);
s32 particle_updateDrag(SpriteParticle *p, Vec3f *prevPos);
void particle_destroySpriteDefault(DescParticleHead *p);
void particle_freeSpriteDefault(DescParticleHead *p);
void particle_destroySpriteParticle(SpriteParticle *p);
s32 particle_freeSpriteParticle(SpriteParticle *p);
s32 particle_hideSpriteDraw(SpriteParticle *particle, s32 view);
void particle_wobbleSpriteOffset(DescParticleHead *head, f32 *amplitude, u8 *phase, u8 *phaseStep);
void particle_applyDragY(Vec3f *vel, f32 drag);
void particle_applyDragXZ(Vec3f *vel, f32 drag);
void particle_applyDrag(Vec3f *vel, f32 drag);
s32 particle_updateFloorBounce(SpriteParticle *p, Vec3f *prevPos);
s32 particle_checkSpriteFloor(SpriteParticle *p, Vec3f *prevPos, ParticleBounce *bounce);
s32 particle_updateDragFloorBounce(SpriteParticle *p, Vec3f *prevPos);
s32 particle_updateDragXZFloorBounce(SpriteParticle *p, Vec3f *prevPos);
s32 particle_bounceOffFloor(SpriteParticle *p, Vec3f *prevPos, f32 *hit, ParticleBounce *bounce);
void particle_onSpriteEvent(SpriteParticle *p, s32 event, u8 eventType);
void particle_wobblePair(u8 *phase, s8 *phaseStep, f32 base, f32 *amplitude, f32 *outX, f32 *outY);
void particle_broadcastToSprites(s32 event, u8 eventType);
s32 particle_updatePulse(SpriteParticle *p, Vec3f *prevPos);
s32 particle_updateFlash(SpriteParticle *p, Vec3f *prevPos);
SpriteParticle *particle_spawnFlash(Vec3f *pos, f32 size, u8 owner, s32 list);

/* game/Particle/ModelParticle/modelparticle.c */
extern ParticleNode *gParticleModelsHead;
extern ParticleNode *gParticleModelsTail;
extern s16 gParticleModelUseCounts[233];
extern s32 gModelParticleCount;
void particle_updateModelParticle(ModelParticle *particle);
void particle_destroyModelParticle(ModelParticle *particle);
void particle_freeModelParticle(ModelParticle *particle);
void particle_releaseModel(ModelParticle *particle);
void particle_destroyModelParticleDefault(ModelParticle *particle);
void particle_freeModelParticleDefault(ModelParticle *particle);
void particle_freeModelParticleLights(ModelParticle *particle);
/* Both return a ModelParticle followed by extraSize bytes of its kind's data (the kind's own type). */
void *particle_newModelParticleWithFloor(ParticleSpawn *spawn, s32 maxLights, s32 lightMask, CollisionHit *floor, s32 extraSize, u8 owner,
                    s32 list);
void *particle_newModelParticle(ParticleSpawn *spawn, s32 maxLights, s32 lightMask, s32 extraSize, u8 owner, s32 list);
void particle_buildModelParticleMatrix(ModelParticle *particle);
Gfx *particle_drawModelParticle(Gfx *gfx, ModelParticle *obj, s16 view);
s32 particle_movePhysics(ModelParticle *particle);
s32 particle_updateSquash(ModelParticle *particle);
s32 particle_buildScaledMatrix(Mtx *mtx, ModelParticle *particle);
void particle_onModelParticleEvent(ModelParticle *particle, void *data, u8 event);
s32 particle_loadModel(s32 model, ParticleNode *node, ModelParticle *particle);
s32 particle_buildUniformMatrix(Mtx *mtx, ModelParticle *particle);
s32 particle_moveBallistic(ModelParticle *particle);
s32 particle_moveBallisticFloor(ModelParticle *particle);
s32 particle_floorBounce(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
s32 particle_floorStop(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
s32 particle_buildOwnerMatrix(Mtx *mtx, ModelParticle *particle);
s32 particle_floorBounceSettle(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);
s32 func_15133C58(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);
s32 func_15133D20(ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);
void particle_onOwnerGone(ModelParticle *particle, ActorRef *data, u8 event);
void particle_broadcastToModelParticles(s32 event, u8 eventType);
Gfx *particle_bindDataTexture(Gfx *gfx, ModelParticle *particle, s16 view);
Gfx *particle_bindFirstListTexture(Gfx *gfx, ModelParticle *particle, s16 view);
Gfx *particle_bindSegmentTexture(Gfx *gfx, u16 index, u8 segment, s32 lifetime);
Gfx *particle_bindListTextures(Gfx *gfx, ModelParticle *particle, s16 view);

/* game/Particle/ParticleModelIds/particlemodelids.c (a data-only unit) */
extern const s32 gParticleModelIds[233]; /* the model ids by particle model (bank 9) */

/* game/Particle/QuadParticle/quadparticle.c */
extern Gfx gQuadParticleSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gQuadParticleEndDl[]; /* render teardown (object type table gObjectTypes) */
/* Source of the four vertex colours given to particle_setQuadColors. */
typedef struct {
    s16 r, g, b, a;
    u16 flag;
} VtxColor;

void particle_updateQuadSprite(QuadSprite *sprite);
QuadSprite *particle_newQuadSprite(void *def, const void *colors, u8 updateFn, u8 expireFn, u8 flip, s32 maxLights, s32 lightMask, Vec3f *pos,
                          s32 extraSize, u8 owner, s32 list);
QuadSprite *particle_spawnFlatQuadSprite(void *def, const void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width, f32 length,
                          u8 angle, u8 flip, s32 maxLights, s32 lightMask, s32 extraSize, u8 owner, s32 list);
QuadSprite *particle_spawnFlatQuadSpriteNoModel(void *def, const void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width, f32 length,
                          u8 angle, u8 flip, s32 extraSize, u8 owner, s32 list);
QuadSprite *particle_spawnSurfaceQuadSprite(void *def, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                          f32 height, u8 angle, u8 flip, s32 maxLights, s32 lightMask, s32 extraSize, u8 owner, s32 list);
QuadSprite *particle_spawnSurfaceQuadSpriteNoModel(void *def, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                          f32 height, u8 angle, u8 flip, s32 extraSize, u8 owner, s32 list);
QuadSprite *func_1513C804(void *def, s32 arg1, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z,
                          f32 width, f32 height, u8 angle, u8 flip, s32 extraSize, u8 owner, s32 list);
void particle_destroyQuadSpriteDefault(QuadSprite *sprite);
void particle_freeQuadSpriteDefault(QuadSprite *sprite);
void particle_freeQuadSpriteLights(QuadSprite *sprite);
void particle_destroyQuadSprite(QuadSprite *sprite);
void particle_freeQuadSprite(QuadSprite *sprite);
void particle_freeQuadVtxBuffers(DefParticleHead *particle);
void particle_destroyQuadParticleDefault(DefParticleHead *particle);
void particle_freeQuadParticleDefault(DefParticleHead *particle);
void particle_freeQuadParticleLights(DefParticleHead *particle);
void particle_destroyQuadParticle(DefParticleHead *particle);
void particle_freeQuadParticle(DefParticleHead *particle);
Gfx *particle_drawQuadSprite(Gfx *gfx, QuadSprite *sprite, s16 viewport);
void particle_onQuadSpriteEvent(s32 obj, s32 event, u8 eventType);
void particle_ignoreQuadSpriteEvent(s32 obj, s32 event, u8 eventType);
void particle_updateQuadParticle(DefParticleHead *particle);
void *particle_newQuadParticle(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 maxLights,
                    s32 lightMask, s32 motionSize, u8 owner, s32 list);
void *particle_newQuadParticleNoModel(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip,
                    s32 motionSize, u8 owner, s32 list);
void *particle_newQuadParticleWhite(void *def, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 motionSize, u8 owner,
                    s32 list);
void *particle_spawnAxisQuadParticle(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width,
                    f32 height, Vec3s **triPtrs, CollisionTri *surface, s32 maxLights, s32 lightMask, u8 onSlope, s32 motionSize,
                    u8 owner, s32 list);
void *particle_spawnAxisQuadParticleNoModel(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width,
                    f32 height, Vec3s **triPtrs, CollisionTri *surface, u8 onSlope, s32 motionSize, u8 owner, s32 list);
s32 particle_computeQuadAxes(ParticleAxes *axes, s16 angle, f32 width, f32 height, Vec3s **triPtrs, CollisionTri *surface,
                   u8 onSlope);
Gfx *particle_drawQuadParticle(Gfx *gfx, DefParticleHead *particle, s16 viewport);
void particle_onQuadParticleEvent(DefParticleHead *particle, s32 event, u8 eventType);
void particle_ignoreQuadParticleEvent(DefParticleHead *particle, s32 event, u8 eventType);
void particle_onQuadParticleOwnerMsg(DefParticleHead *particle, ActorMsg *msg, u8 eventId);
void func_1513E134(void);
void particle_layQuadSpriteFlat(QuadSprite *sprite, f32 x, f32 y, f32 z, f32 width, f32 length, u8 angle);
void func_1513E2A4(void);
s32 particle_layQuadSpriteOnSurface(QuadSprite *sprite, Vec3s **triPtrs, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                  f32 height, u8 angle);
s32 particle_updateDriftMotion(DefParticleHead *particle);
Vtx *particle_buildRotatedQuad(DefParticleHead *particle, s16 viewport);
Vtx *particle_buildUprightQuad(); /* (DefParticleHead *particle, u8 reversed, s16 viewport); K&R definition, declared without a prototype */
Vtx *particle_buildUprightQuadFwd(void *particle, s16 viewport);
Vtx *particle_buildUprightQuadRev(void *particle, s16 viewport);
Vtx *particle_buildRotatedRectQuadUnrolled(DefParticleHead *particle, s16 viewport);
Vtx *particle_buildBendingQuad(DefParticleHead *particle, s16 viewport);
void particle_buildAxisQuad(DefParticleHead *head, s16 viewport);
Gfx *particle_setCombineMode(Gfx *gfx, u8 combiner, u8 *needSync);
void particle_setQuadParticleFuncs(DefParticleHead *particle, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn);
void particle_setQuadSpriteFuncs(QuadSprite *sprite, u8 updateFn, u8 expireFn);
s32 particle_updateGrowMotion(DefParticleHead *particle);
s32 particle_updateBallisticMotion(DefParticleHead *particle);
void particle_buildBeamQuadFwd(DefParticleHead *particle, s16 viewport);
void particle_buildBeamQuadRev(DefParticleHead *particle, s16 viewport);
Vtx *particle_buildBeamQuad(DefParticleHead *particle, u8 reversed, BeamScale *scale, s16 viewport);
void particle_setQuadTexCoords(Vtx *vtx, u8 texture, u8 flip);
void particle_setQuadColors(Vtx *vtx, const VtxColor *colors);
Vtx *particle_buildRotatedRectQuad(DefParticleHead *particle, s16 viewport);
void particle_broadcastToQuadParticles(s32 event, u8 eventType);
void particle_broadcastToQuadSprites(s32 event, u8 eventType);
Vtx *particle_buildQuadFromAxes(DefParticleHead *particle, Vec3f *axisU, Vec3f *axisV, s16 viewport);
s32 particle_sortDrawList(s32 gfx, s32 type, s32 list, s16 view);
extern const VtxColor gWhiteVtxColors[4];
extern const RenderModePair gEffectRenderModes[28]; /* indexed by the effects' render-mode bytes */
extern const s32 gQuadParticleTypes[4];

/* game/Particle/ChannelParticle/channelparticle.c */
ChannelParticle *particle_spawnChannelParticle(ParticleEmitterDesc *emitter, u8 vertexFn, u8 findZone, u8 kind, u8 owner, s32 list);
s32 particle_flickerChannelParticle(ChannelParticle *particle);
s32 particle_growChannelParticle(ChannelParticle *particle);
s32 particle_shrinkChannelParticle(ChannelParticle *particle);
s32 particle_shrinkRisingChannelParticle(ChannelParticle *particle);
Vtx *particle_buildChannelQuad(ChannelParticle *particle, s16 index);
s32 particle_spawnBillboardOnExpire(ChannelParticle *particle);

/* game/Particle/FlickerParticle/flickerparticle.c */
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
    /* 0x38 */ f32 frameSpeedMax;
    /* 0x3C */ f32 frameSpeedMin;
    /* 0x40 */ f32 frameSpeedTarget;
    /* 0x44 */ s16 frameSpeedTimer;
    /* 0x46 */ s16 fadeStart; /* while def.lifetime is below it, alpha = lifetime * fadeRate */
    /* 0x48 */ s16 fadeRate;
} FlickerParams;

/* Effect instance shared by the update callbacks in this file. */
typedef struct {
    /* 0x00 */ DefParticleHead head; /* head.def.rot: the previous position, head.def.scale.z: trail factor,
                                      * head.unk74: flags */
    /* 0x110 */ FlickerParams params;
} FlickerParticle;

Vtx *particle_buildFlickerQuad(FlickerParticle *effect, s16 index);
s32 particle_flickerSize(FlickerParticle *effect);
s32 particle_growFlickerParticle(FlickerParticle *effect);
s32 particle_shrinkFlickerParticle(FlickerParticle *effect);
s32 particle_moveFlickerParticle(FlickerParticle *effect);
s32 particle_moveFlickerTrail(FlickerParticle *effect);

/* game/Particle/GroundCluster/groundcluster.c */
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
    /* 0x150 */ f32 shrinkFraction; /* width and height lose this fraction every call (particle_shrinkBallisticQuad) */
} Particle177B50;

void *particle_buildCameraFacingBeamQuad(Beam1514A6A0 *beam, s16 cam);
void particle_scatterChannelParticles(f32 x, f32 y, f32 z, f32 radius, s16 count, f32 sizeBase, f32 sizeRange, f32 heightBase,
                   f32 heightRange, s16 lifeBase, s16 lifeRange, u8 expireFn);
void particle_spawnRandomChannelParticle(Vec3f *pos, u8 owner, s32 list);
s32 particle_shrinkBallisticQuad(Particle177B50 *p);
void particle_scatterChannelParticlesInDisc(Vec3f *pos, u8 owner, s32 list);
void particle_spawnGroundCluster(Vec3f *position, CollisionHit *ground, u8 owner, s32 list);
s32 particle_fadeTrailByLife(MotionTrail *trail);
s32 particle_growQuadAtRate(Particle177B50 *p);
s32 particle_capQuadAlphaByLife(Particle177B50 *p);
s32 particle_updateJitter(JitterParticle *p, ParticleJitter *w);

/* game/Particle/JitterUpdate/jitterupdate.c */
void particle_updateJitterParticle(JitterParticle *p);

/* game/Particle/BendTarget/bendtarget.c */
void particle_easeBendTarget(void *arg);

/* game/Particle/JitterBallistic/jitterballistic.c */
void particle_resetBendTarget(JitterParticle *p);
void particle_updateRelaunchedJitter(JitterParticle *p);
void particle_moveJitterBallistic(JitterParticle *p);
s32 particle_updateJitterBallisticEased(JitterParticle *p);
s32 particle_updateJitterBallistic(JitterParticle *p);
s32 particle_relaunchJitterParticle(JitterParticle *p);

/* game/Particle/SpawnShapes/spawnshapes.c */
/*
 * Per-kind particle spawn callback, indexed by `kind` in gParticleKindCallbacks. `shape` is 0 line,
 * 1 even circle, 2 random circle, 3 random vertical arc; returning 0 stops the emitter.
 */
typedef s32 (*EmitterCallback)(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX,
                               f32 originY, f32 originZ, s32 angle, s32 baseAngle, f32 radius,
                               s32 arg11, f32 arg12, void *data, u8 arg14);
/* The callbacks by kind: the .rodata at 0x22F4C0 (no file yet). */
extern EmitterCallback gParticleKindCallbacks[];
void particle_emitOnCircle(f32 x, f32 y, f32 z, f32 radius, u8 startAngle, s8 angleStep, s16 count, u8 kind, s32 arg8, f32 arg9,
                   void *data, u8 owner);
void particle_emitOnLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 count, u8 kind, s32 arg8, f32 arg9, void *data,
                   u8 owner);
void particle_emitOnRandomCircle(f32 x, f32 y, f32 z, f32 radius, s16 angleA, s16 angleB, s16 count, u8 kind, s32 arg8, f32 arg9,
                   void *data, u8 owner);
void particle_emitOnRandomArc(f32 x, f32 y, f32 z, f32 radius, s16 yaw, s16 angleA, s16 angleB, s16 count, u8 kind, s32 arg9,
                   f32 arg10, void *data, u8 owner);

/* game/Particle/BurstConfigs/burstconfigs.c */
/* Copied into the object spawned by particle_spawnGrowFade at +0x170; advanced each frame by particle_updateGrowFade. */
typedef struct {
    f32 elapsed;
    f32 duration;
    f32 growth; /* size = growth * sqrt(elapsed) */
    f32 alpha;  /* alpha - fade * elapsed^2 */
    f32 fade;
    u8 growOneAxis; /* grow only head.sizeXZ */
} Extra1515452C;

/* Copied into the object spawned by particle_spawnSwell at +0x170; advanced each frame by particle_updateSwell. */
typedef struct {
    f32 remaining;
    f32 size;
    f32 alpha;
    f32 phase; /* size and alpha are scaled by sinf(phase) */
    f32 phaseRate;
} Extra15154A88;

/* A particle_newModelParticle object of particle_spawnGrowFade or particle_spawnSwell, with its extra data. */
typedef struct {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ union {
        Extra1515452C grow;
        Extra15154A88 pulse;
    } extra;
} Object15132A4C;

void particle_initConeFromTri(TriangleSource *source, ConeBasis *cone);
s32 particle_completeConeBasis(ConeBasis *cone);
void particle_getRandomConeDir(ConeBasis *cone, f32 speed, Vec3f *out);
void particle_spawnJetSpray(JetSpray *config, Vec3f *direction, Vec3f *axis0, Vec3f *axis1, f32 spread, u8 owner, s32 list);
void particle_spawnDirectedJetSpray(DirectedJetSpray *emitter, u8 owner, s32 list);
void particle_spawnSurfaceJetSpray(SurfaceJetSpray *emitter, u8 owner, s32 list);
void particle_spawnJetSprayBursts(JetSpraySpec *config, u8 owner, s32 list);
void particle_spawnDropSprayOnTri(TriangleSource *source, DropSpray *config, CollisionHit *ground, u8 owner, s32 list);
void particle_spawnDropSpray(ConeBasis *cone, DropSpray *config, CollisionHit *ground, u8 owner, s32 list);
void particle_spawnDropSprayAtAngles(AngleRange *angles, DropSpray *config, CollisionHit *ground, u8 owner, s32 list);
void particle_spawnDebrisScatter(DebrisScatter *config, AngleRange *angles, u8 owner, s32 list);
void particle_spawnDebrisSpray(DebrisSpray *config, const s32 *variantIds, const f32 *variantScales, s32 variantCount,
                   void *extraData, s32 extraSize, u8 variant, u8 hasLifetime, s8 drawFn, CollisionHit *floor, f32 radius, u8 argB,
                   u8 argC, u8 owner, s32 list);
void particle_spawnDecalSpray(DecalSpray *config, u8 owner, s32 list);
void particle_spawnPieceSpray(PieceSpray *config, u8 altDisplayList, u8 owner, s32 list);
void particle_spawnSurfaceBurst(SurfaceBurst *config, u8 owner, s32 list);
void particle_spawnSurfacePuffSpray(SurfacePuffSpray *config, u8 owner, s32 list);
void particle_spawnPuffSpray(PuffSpray *config, u8 owner, s32 list);
void particle_spawnSurfaceDebrisBurst(SurfaceDebrisBurst *config, const s32 *variantIds, const f32 *variantScales, u32 variantCount, u8 owner,
                   s32 list);
void particle_spawnDebrisBurst(DebrisBurst *config, const s32 *variantIds, const f32 *variantScales, u32 variantCount, f32 radius, u8 arg5,
                   u8 owner, s32 list);
void particle_spawnModelBurst(ModelBurst *config, u8 owner, s32 list);
void particle_spawnDriftBurst(DriftBurst *config, u8 owner, s32 list);
void particle_getRandomPaletteColor(Color8 *out);
/* (TrailBurst *config, u8 owner, s32 list); declared without a prototype: game/Beam/AimedBeam/aimedbeam.c passes a fourth argument */
void particle_spawnTrailBurst();
void particle_spawnSimplePuffSpray(SimplePuffSpray *config, u8 owner);
void particle_spawnBillboardBurst(BillboardBurst *config, u8 owner, s32 list);
void particle_spawnSpriteBurst(SpriteBurst *config, u8 views, u8 owner, s32 list);
void particle_spawnStreakBurst(StreakBurst *config, u8 owner);
void particle_spawnSplashSprayOnTri(TriangleSource *source, SplashSpray *config, CollisionHit *ground, u8 owner, s32 list);
void particle_spawnSplashSpray(ConeBasis *cone, SplashSpray *config, CollisionHit *ground, u8 owner, s32 list);
void particle_spawnSplashSprayAtAngles(AngleRange *angles, SplashSpray *config, CollisionHit *ground, u8 owner, s32 list);
void *particle_spawnGrowFade(const Vec3f *position, f32 duration, f32 growth, f32 alpha, f32 sizeY, u8 owner, s32 list);
s32 particle_updateGrowFade(Object15132A4C *object);
void particle_spawnParticleBurst(ParticleBurst *config, u8 owner, s32 list);
void *particle_spawnSwell(Vec3f *position, f32 duration, f32 size, f32 alpha, u8 owner, s32 list);
s32 particle_updateSwell(Object15132A4C *object);

/* game/Particle/StreakParticle/streakparticle.c */
extern BurstPoint gPipeBurstPoints[3][10]; /* random burst points around each pipe (level_initPipeBurstPoints fills them) */
extern Gfx gStreakParticleSetupDl[]; /* render setup (object type table gObjectTypes) */
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
    /* 0x44 */ u16 renderMode;      // low byte: gEffectRenderModes render mode, high byte: particle_setCombineMode combiner
    /* 0x48 */ f32 width;
    /* 0x4C */ u8 alpha;
    /* 0x4E */ s16 fadeStart;       // fade begins when lifetime drops below this
    /* 0x50 */ s16 fadeRate;        // alpha = lifetime * fadeRate while fading
    /* 0x52 */ u8 pad52[0x6];
    /* 0x58 */ Vtx vtxTemplate[3];  // tail, head + side, head - side
    /* 0x88 */ Vtx *vtxBuffers[4];  // per-view vertex buffers (two frames each)
} StreakParticle;

/* A streak burst plus one particle at the same point (particle_spawnStreakBurstEffect). */
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
    /* 0x4C */ s32 particleSetGeometryMode;
    /* 0x50 */ s32 particleClearGeometryMode;
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

/* An effect that sets off bursts at random points of gPipeBurstPoints at random intervals. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ RandomTimer timer;
} RandomBurstEffect;

StreakParticle *particle_newOwnedStreakParticle(StreakParticleDesc *desc, u8 kind, s32 extraSize, u8 owner, s32 list);
void particle_newStreakParticle(StreakParticleDesc *desc, u8 kind, s32 extraSize);
void particle_updateStreakParticle(StreakParticle *obj);
Gfx *particle_drawStreakParticle(Gfx *gfx, StreakParticle *particle, s16 viewIndex);
void particle_spawnStreakBurstEffect(); /* (StreakBurstEffectDesc *desc, u8 owner); K&R definition, declared without a prototype */
void particle_updateRandomBurstEffect(RandomBurstEffect *effect);
void particle_spawnPipeBurst(f32 *pos, u8 owner);
void particle_freeStreakVtxBuffers(StreakParticle *particle);
void particle_destroyStreakParticle(StreakParticle *particle);
void particle_freeStreakParticle(StreakParticle *particle);

/* game/Particle/SlidingSprite/slidingsprite.c */
/* Emitter state stored in an effect object at +0x28. */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 rate;        // particles per unit of gFrameDelta
    /* 0x10 */ f32 accumulator; // pending particles; one is spawned per whole unit
    /* 0x14 */ f32 ceiling; // highest floor above the camera eye (GroundParticle.upperFloorY); copied to RiseMotion.ceiling
} RiseEmitterState;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ RiseEmitterState emitter;
} EmitterObject;

/* Per-sprite data particle_spawnSlidingSprites appends to its sprites (at ScreenSprite +0x70). */
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

void particle_spawnUnderwaterRiseEmitter(s16 player);
void particle_updateUnderwaterRiseEmitter(EmitterObject *obj);
void particle_removePlayerSlidingSprites(s16 player);
void particle_handleSlidingSpriteMessage(SlidingSprite *sprite, s16 *payload, u8 type);
void particle_spawnSlidingSprites(s16 player);
s32 particle_updateSlidingSprite(SlidingSprite *sprite);
void particle_onCameraStaysAboveWater(s16 player);
void particle_onCameraStaysUnderwater(s16 player);
void particle_onCameraSnapsAboveWater(s16 player);
void particle_onCameraLeavesWater(s16 player);
void particle_onCameraSnapsUnderwater(s16 player);
void particle_onCameraEntersWater(s16 player);

/* game/Particle/PuffQuad/puffquad.c */
extern Gfx gPuffQuadSetupDl[]; /* object type 0xD render setup (object type table gObjectTypes) */
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

void particle_spawnPuffQuad(s32 x, s32 y, s32 z, f32 scale, u8 owner, s32 list);
void particle_updatePuffQuad(void *self);
Gfx *particle_drawPuffQuad(Gfx *gfx, PuffQuadEffect *obj, s32 view);
Gfx *particle_beginPuffQuadDraw(Gfx *gfx, s32 type, s32 list, s32 view);
Gfx *particle_endPuffQuadDraw(Gfx *gfx, s32 type, s32 view);

/* game/Particle/BounceDebris/bouncedebris.c */
/* Partial view of a type 0x10 ModelFx object (game/Effects/ObjectList/objectlist.c): EffectHeader, matrices, then its data. */
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

/* Partial view of a type 5 billboard Particle (game/Effects/ObjectList/objectlist.c) as its falling-drop callback uses it. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s32 floorY;      /* floor height last found under the drop */
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ s16 x;
    /* 0x22 */ s16 y;
    /* 0x24 */ s16 z;
    /* 0x26 */ s16 velX;
    /* 0x28 */ s16 velZ;
    /* 0x2A */ s16 floorOffset; /* added to y for the floor test */
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ s16 life;
    /* 0x3A */ u8 pad3A;
    /* 0x3B */ u8 size;
} DropParticle;

/* Partial view of a type 3 SpriteFx (game/Effects/ObjectList/objectlist.c); unk25 is SpriteFxTemplate.unk15. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ s16 frame;       /* 8.8 fixed point */
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s16 x;
    /* 0x1A */ s16 y;
    /* 0x1C */ s16 z;
    /* 0x1E */ u8 pad1E[7];
    /* 0x25 */ u8 size;
} SplashFx;

void particle_updateBounceDebris(Debris *debris);
void particle_updateDropToSplash(DropParticle *drop);
void particle_updateSplashFx(SplashFx *fx);
struct StyleParticle; /* game/Particle/StyleParticle/styleparticle.c */
s32 particle_updateStyleBounce(struct StyleParticle *particle);

/* game/Particle/StyleParticle/styleparticle.c */
extern Gfx gStyleParticleSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gStyleParticleEndDl[]; /* render teardown (object type table gObjectTypes) */
extern u8 gStyleEmitterTimers[4]; /* per style object: intensity, raised to 0xFF over time (particle_tickStyleEmitterTimers) */
/* A 0x34-byte sprite particle, spawned by particle_newStyleParticle and updated every frame by the
 * handler that gStyleUpdateFuncs holds for its kind. The bytes from 0x26 on depend on the kind. */
typedef struct StyleParticle {
    /* 0x00 */ u8 type;  /* EffectHeader.type: 0x12, or 0x67 */
    /* 0x01 */ u8 list;  /* EffectHeader byte 1: the object list (effect_newObject) */
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner; /* EffectHeader.owner */
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 x;
    /* 0x10 */ s16 y;
    /* 0x12 */ s16 z;
    /* 0x14 */ s16 scaleX;
    /* 0x16 */ s16 scaleY;
    /* 0x18 */ s16 velocityY;
    /* 0x1A */ u8 texture; /* index into gEffectAnimTextures */
    /* 0x1B */ u8 frame;
    /* 0x1C */ u8 r;
    /* 0x1D */ u8 g;
    /* 0x1E */ u8 b;
    /* 0x1F */ u8 alpha;
    /* 0x20 */ u8 envR;
    /* 0x21 */ u8 envG;
    /* 0x22 */ u8 envB;
    /* 0x23 */ u8 kind; /* index into gStyleUpdateFuncs */
    /* 0x24 */ u8 lifetime; /* fades in while nonzero, out once it runs down */
    /* 0x25 */ u8 yFraction;
    union {
        /* kinds that drift sideways with particle_moveStyleXZ */
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
        /* particle_updateStyleTint: blends its colour from rgb0 to rgb1 while it grows */
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
        /* particle_updateStylePulse: scaled to size * alpha / 128 */
        struct {
            /* 0x26 */ u8 fadeRate;
            /* 0x27 */ u8 unk27[5];
            /* 0x2C */ u8 sizeHigh;
            /* 0x2D */ u8 sizeLow;
        } pulse;
        /* particle_updateStyleBounce (game/Particle/BounceDebris/bouncedebris.c): drifts like the drift kinds and bounces off the floor */
        struct {
            /* 0x26 */ u8 drift[6];
            /* 0x2C */ u8 floorHi; /* floor height + 10, high byte */
            /* 0x2D */ u8 floorLo;
        } bounce;
        /* particle_updateStyleColorRamp: colour from an 8-step ramp in gStyleRampColors */
        struct {
            /* 0x26 */ u8 start;
        } ramp;
    } u;
    /* 0x30 */ TriggerVolume *link; /* the volume that follows the particle (linkArg), or NULL */
} StyleParticle;

/* A 0x4C-byte particle emitter, spawned by particle_newSpriteEmitter and run by particle_updateSpriteEmitter. Most of
 * its bytes are the template that particle_newStyleParticle gets for each particle it spawns. */
typedef struct SpriteEmitter {
    /* 0x00 */ u8 type; /* EffectHeader.type: 0x11, or 0x66 for an emitter whose particles are 0x67s */
    /* 0x01 */ u8 list; /* EffectHeader byte 1: the object list (effect_newObject) */
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner; /* EffectHeader.owner */
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 x; /* also a pointer's high half (flags 0x30) or a gSpriteEmitterSegments row */
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

void particle_tickStyleEmitterTimers(void);
/* K&R definition: the int-argument prototype that callers use. */
void particle_newStyleParticle(s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
void particle_updateStyleParticle(StyleParticle *particle);
Gfx *particle_drawStyleParticle(Gfx *gfx, StyleParticle *particle, s32 view);
/* K&R definition: the int-argument prototype that callers use */
void particle_newSpriteEmitter(s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                   s32, u8, s32, s32, s32, s32, s32, u8, s32);
void particle_updateSpriteEmitter(SpriteEmitter *obj);
/* void particle_spawnEmitterFollowingPos(f32 *position, s32 duration, u8 texture, u8 owner, s32 list) is declared in its only caller,
   game/Cutscene/SeqEvents/seqevents.c, which was compiled with an s8 texture. */
s32 particle_updateStyleColorRamp(StyleParticle *particle);
s32 particle_updateStylePulse(StyleParticle *particle);
void func_1516ED68(s32 object, s32 duration, u8 owner, s32 list);
void particle_spawnThrottledEmitter(s32 object, s32 threshold, u8 owner, s32 list);
s32 particle_updateStyleDrift(StyleParticle *particle);
void func_1516F1C0(s32 object, s32 duration, u8 owner, s32 list);
/* (u8 object, s32 duration, s32 particleLifetime, s32 speed, u8 owner, s32 list); declared without a prototype:
   game/Cutscene/SeqEvents/seqevents.c passes the object index and owner unnarrowed */
void func_1516F2F8();
void func_1516F548(s32 x, s32 y, s32 z, s32 duration, s32 velocityXHigh, s32 velocityY, s32 velocityZHigh, s32 scale,
                   s32 scaleRandom, s32 radius, s32 style0, s32 style1, s32 arg12, s32 arg13, s32 arg14, u8 owner,
                   s32 list);
void particle_moveStyleXZ(StyleParticle *particle);
void particle_scaleStyleVelX(StyleParticle *particle, s32 factor);
void particle_scaleStyleVelZ(StyleParticle *particle, s32 factor);
void particle_scaleStyleVelXZ(StyleParticle *particle, s32 factor);
void particle_scaleStyleVel(StyleParticle *particle, s32 factor);
s32 particle_updateStyleDamped(StyleParticle *particle);
void func_1516FA88(s16 x, s16 y, s16 z, s32 isAlt, u8 owner, s32 list);
s32 particle_updateStyleTint(StyleParticle *particle);
s32 particle_updateStyleAccelY(StyleParticle *particle);
void func_1516FE1C(s32 object, s32 duration, u8 owner, s32 list);
void particle_getRandomDirection(s32 flags, f32 *outX, f32 *outY, f32 *outZ);
/* void particle_spawnQuadEdgeStyleParticles(f32 (*verts)[3], f32 x, f32 z, u8 style, f32 scale, s32 type, s32 countA, s32 countB, u8 owner,
   s32 list) is declared in the files that call it: game/Effects/ModelBurst/modelburst.c passes an s32 style. */
/* (s16 x, s16 y, s16 z, s32 angle, s32 isParticle, u8 owner, s32 list); declared without a prototype: game/Sprite/LevelSprites/levelsprites.c
 * passes the coordinates without narrowing them */
void func_15170500();
s32 particle_updateStyleSlide(StyleParticle *particle);
void particle_spawnStyleBurst(s32 x, s32 y, s32 z, s32 count, f32 speed, u8 owner, s32 list);

/* game/Particle/FloorSprite/floorsprite.c */
extern Gfx gFloorSpriteSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gFloorSpriteEndDl[]; /* render teardown (object type table gObjectTypes) */
/* A textured sprite particle: one quad, drawn by particle_drawFloorSprite. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Vtx vtx[4];
    /* 0x50 */ s16 lifetime;  // drawn while >= 2; 0 starts the -3 -> -1 removal countdown
    /* 0x52 */ s16 animFrame; // 8.8 fixed point frame index
    /* 0x54 */ s16 animSpeed;
    /* 0x56 */ u16 color;     // RGBA5551
    /* 0x58 */ u8 animIndex;  // index into gEffectAnimTextures
    /* 0x59 */ u8 flags;      // 1: cloud blend, 2: flat quad, 4: play animation once, 8: texture-only combine
} Particle19F150;

Particle19F150 *particle_newFloorSprite(s16 lifetime, u16 color, u8 animIndex, s32 flags, s32 animSpeed, s32 arg5, u8 owner,
                              s32 list);
void particle_spawnFloorSprite(f32 x, f32 y, f32 z, s16 lifetime, u16 color, u8 animIndex, f32 angle, s32 dist, s32 size, s32 flags,
                   s32 animSpeed, s32 arg11, u8 owner, s32 list);
void particle_spawnTriSprite(f32 x, f32 y, f32 z, void *tri, s16 lifetime, u16 color, u8 animIndex, s32 size, s32 flags,
                   s32 animSpeed, s32 arg10, u8 owner, s32 list);
void particle_updateFloorSprite(Particle19F150 *particle);
Gfx *particle_drawFloorSprite(Gfx *gfx, Particle19F150 *particle, s32 view);
s32 particle_layQuadOnTri(Vtx *vtx, void *tri, f32 x, f32 y, f32 z, f32 angle, s32 size, u8 width);
s32 particle_layQuadOnFloor(Vtx *vtx, f32 x, f32 y, f32 z, f32 angle, s32 dist, s32 size, u8 width);

/* game/Particle/WakePuff/wakepuff.c */
/* The parts of a spawned particle that particle_updateWakePuff uses (layout as Particle in game/Effects/ObjectList/objectlist.c). */
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

void particle_spawnWakePuff(s32 objIndex, s32 spread, s32 growth, s32 growthRange, s32 size, s32 fadeRate, s32 velY, u8 owner,
                   s32 list);
void particle_updateWakePuff(PuffParticle *particle);

/* game/Swarm/WaveBob/wavebob.c */
void swarm_bobWaveVertices(u8 slot);

/* game/Particle/SpinDebris/spindebris.c */
extern Gfx gSpinDebrisZbSetupDl[]; /* render setup of object types 8 and 9 (object type table gObjectTypes) */
extern Gfx gSpinDebrisNoZSetupDl[]; /* render setup of object types 12 and 89 (object type table gObjectTypes) */
/*
 * Small spinning, fading debris particles. They are allocated with effect_newObject (0xB8 bytes) and driven by
 * the object-type handler table in .rodata (D_8008B648 etc.): an update callback (particle_updateSpinDebris or
 * particle_updateGroundChunk), the draw callback particle_drawSpinDebris and the begin-draw hook particle_beginSpinDebrisDraw.
 */
typedef struct SpinDebrisParticle {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Mtx mtx[2];   // model matrix, double-buffered by gDrawBufferIndex
    /* 0x90 */ s16 x;
    /* 0x92 */ s16 y;
    /* 0x94 */ s16 z;
    /* 0x96 */ s16 rotX;
    /* 0x98 */ s16 rotY;
    /* 0x9A */ s16 rotZ;
    /* 0x9C */ s8 spinX;
    /* 0x9D */ s8 spinY;
    /* 0x9E */ s8 spinZ;
    /* 0x9F */ u8 texture;   // passed to particle_loadDebrisTexture
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

void particle_spawnSpinDebris(u8 texture, s16 x, s16 y, s16 z, s8 velY, u16 timer, u16 unkAC, u8 speedRange, u8 spinRange,
                   u8 dragRange, s32 type);
void particle_updateSpinDebris(SpinDebrisParticle *particle);
Gfx *particle_emptySpinDebrisDrawHook(Gfx *gfx);
Gfx *particle_drawSpinDebris(Gfx *gfx, SpinDebrisParticle *particle, s16 view);
void particle_spawnGroundChunk(f32 size, u8 texture, s16 x, s16 y, s16 z);
void particle_updateGroundChunk(SpinDebrisParticle *particle);
Gfx *particle_beginSpinDebrisDraw(Gfx *gfx, s32 type, s32 list, s32 view);
Gfx *particle_loadDebrisTexture(Gfx *gfx, s32 texture);
void particle_spawnGroundChunks(f32 size, s32 x, s32 y, s32 z, s32 count);
Gfx *particle_endSpinDebrisDraw(Gfx *gfx, s32 type, s16 player);

/* game/Particle/BoneGlow/boneglow.c */
/* The object flare_spawnGlow spawns for particle_spawnBoneGlow. head.def.pos / rot are the world-space attach
 * points, updated by particle_followBoneGlow. */
typedef struct BoneGlow {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ GlowShape params;
    /* 0x16C */ u8 pad16C[4];
    /* 0x170 */ EmitterAttach attach;
} BoneGlow;

BoneGlow *particle_spawnBoneGlow(ParticleDef *spawn, GlowShape *params, EmitterAttach *attach, u8 updateFn1, u8 updateFn2, u8 expireFn,
                        u8 flip);
s32 particle_followBoneGlow(BoneGlow *emitter);
s32 particle_setBoneGlowBySwapTexture(BoneGlow *emitter, u8 *follow);
s32 particle_hideBoneGlowWhileEyeClosed(BoneGlow *emitter, u8 *follow);
s32 particle_hideBoneGlowWithoutExpression(BoneGlow *emitter, u8 *follow);
void particle_onBoneGlowSwap(BoneGlow *emitter, ActorSwapMsg *msg, u8 msgId);

/* game/Particle/Droplets/droplets.c */
s32 particle_splashOnFloor(FloorParticle *particle);
s32 particle_dieBelowFloor(FloorParticle *particle);
s32 particle_fadeInAboveFloor(FloorParticle *particle);
void particle_spawnDroplets(f32 *pos, f32 radius, f32 size, s32 count, u8 owner);
void particle_spawnModelDroplets(f32 *pos, f32 radius, f32 size, s32 count, u16 model, s32 unused, u8 owner);

/* game/Particle/BillboardBurst/billboardburst.c */
void particle_spawnModelBurstWithBillboard(f32 x, f32 y, f32 z, u8 owner, s32 list);

/* game/Particle/GrowingBillboard/growingbillboard.c */
/* The parts of a spawned particle that particle_updateGrowingBillboard uses (layout as Particle in game/Effects/ObjectList/objectlist.c). */
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

void particle_spawnGrowingBillboard(BillboardSpawn *effect, u8 mode);
void particle_updateGrowingBillboard(GrowingParticle *particle);

/* game/Particle/PlacementRing/placementring.c */
void particle_spawnPlacementRing(WorldObject *placement, u8 owner, s32 list);

/* The front-end .rodata of the unit that begins with game/Particle/ActorEmitter/actoremitter.c (0x24BF20), defined in that
 * file until the unit's files are merged (see the comment there). */
extern const char gAllOverFireToSmokeName[]; /* particle_chainAllOverFireToSmoke's string */
extern const char gTailFireToSmokeName[]; /* particle_chainTailFireToSmoke's string */
extern const IdList1 gActorEmitterTypes; /* { 0x21 }: the template of particle_broadcastToActorEmitters's local */
/* The anchor emitter templates, one array per kind (only variant 0 exists; gAnchorEmitterTemplates). */
extern const AnchorEmitterTemplate D_800A74D8[], D_800A7528[], D_800A7578[], D_800A75C8[], D_800A7618[], D_800A7668[], D_800A76B8[];
extern const AnchorEmitterTemplate D_800A7708[], D_800A7758[], D_800A77A8[], D_800A77F8[], D_800A7848[], D_800A7898[], D_800A78E8[];
extern const AnchorEmitterTemplate D_800A7938[], D_800A7988[], D_800A79D8[], D_800A7A28[], D_800A7A78[];
/* The per-object-id emitter data blocks of particle_getObjEmitterData (all empty in this build). */
extern const u8 D_800A7AC8[4], D_800A7ACC[4], D_800A7AD0[4], D_800A7AD4[4], D_800A7AD8[4], D_800A7ADC[4], D_800A7AE0[4];
extern const u8 D_800A7AE4[4], D_800A7AE8[4], D_800A7AEC[4], D_800A7AF0[4], D_800A7AF4[4], D_800A7AF8[4], D_800A7AFC[4];
extern const u8 D_800A7B00[4], D_800A7B04[4], D_800A7B08[4], D_800A7B0C[4], D_800A7B10[4];

/* game/Particle/ActorEmitter/actoremitter.c */
/* Particle template, referenced by an EmitterInfo. */
typedef struct ParticleTemplate {
    /* 0x00 */ f32 widthHi;
    /* 0x04 */ f32 widthLo;
    /* 0x08 */ f32 heightHi;
    /* 0x0C */ f32 heightLo;
    /* 0x10 */ f32 heightPeak;
    /* 0x14 */ f32 frameSpeedHi;
    /* 0x18 */ f32 frameSpeedLo;
    /* 0x1C */ f32 gravity;
    /* 0x20 */ f32 drag;
    /* 0x24 */ f32 shrinkRate;
    /* 0x28 */ s16 fadeStart;
    /* 0x2A */ s16 fadeRate;
    /* 0x2C */ f32 blend;
    /* 0x30 */ f32 stretch;
    /* 0x34 */ s16 lifeMin;   // lifetime given to a particle when its slot releases it
    /* 0x36 */ s16 lifeRange;
    /* 0x38 */ u8 updateFn1;
    /* 0x39 */ u8 updateFn2;
    /* 0x3A */ u8 vertexFn;
    /* 0x3B */ u8 releaseUpdateFn1;
    /* 0x3C */ u8 releaseUpdateFn2;
    /* 0x3D */ u8 releaseExpireFn;
    /* 0x3E */ u8 releaseVertexFn;
    /* 0x3F */ u8 spawnCb; // index into gActorEmitterSpawnFuncs
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

ActorEmitterProcess *particle_startActorEmitter(Actor *owner, s16 kind, s16 lifetime, u8 killExisting, u8 fxOwner, u8 fxList,
                                                const char *callerName);
void particle_updateActorEmitter(ActorEmitterProcess *emitter);
s32 particle_followEmitterSound(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
void particle_freeActorEmitterParticles(ActorEmitterProcess *emitter);
void particle_setActorEmitterTable(ActorEmitterProcess *emitter, u8 table);
ActorEmitterProcess *particle_newActorEmitter(ActorEmitterParams *params, u8 owner);

/* game/Particle/ActorEmitterHandlers/actoremitterhandlers.c */
void particle_detachActorEmitter(ActorEmitterProcess *emitter);
void particle_destroyActorEmitter(ActorEmitterProcess *emitter);
void particle_freeActorEmitter(ActorEmitterProcess *emitter);
void particle_pickBlackTexture(u8 *texture, u8 *flip, u8 *r, u8 *g, u8 *b, u8 *alpha, u16 *renderModes);
void particle_pickPaletteTexture(u8 *texture, u8 *flip, u8 *r, u8 *g, u8 *b, u8 *alpha, u16 *renderModes);
s32 particle_chainAllOverFireToSmoke(ActorEmitterProcess *emitter);
s32 particle_chainTailFireToSmoke(ActorEmitterProcess *emitter);
Vtx *particle_buildActorEmitterQuad(ActorEmitterParticle *p, s16 vp);

/* game/Particle/AnchorEmitter/anchoremitter.c */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ SpriteWobble wobble;
} EffectSprite;

/* Particle created by effect_spawnBoneFollower from an EmitterParticleDesc. */
typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ JitterParticle *sprite; /* made by particle_spawnJitterParticle */
    /* 0x30 */ AnchorEmitter *parent;
    /* 0x34 */ Vec3f pos;
} EmitterParticle;

s32 particle_wobbleSpriteSize(EffectSprite *sprite);
s32 particle_easeSpriteSizeOnce(EffectSprite *sprite);
void particle_broadcastToActorEmitters(s32 event, u8 eventType);
void particle_onActorEmitterMsg(ActorEmitterProcess *self, ActorRef *other, u8 eventType);
/* returns the AnchorEmitter, followed by arg7 bytes of the caller's data (PeriodicObject) */
void *particle_startAnchorEmitter(Actor *owner, u8 index, s16 duration, s8 updateFunc, s8 expireFunc, s8 childKind, u8 messageFunc, s32 extraSize,
                    u8 fxOwner, s32 fxList);
AnchorEmitter *particle_newAnchorEmitter(Actor *owner, const AnchorEmitterTemplate *tmpl, s16 duration, s8 updateFunc, s8 expireFunc,
                             s8 childKind, u8 messageFunc, s32 extraSize, u8 fxOwner, s32 fxList);
s32 particle_updateAnchorEmitter(AnchorEmitter *emitter);
void particle_runAnchorEmitterExpire(AnchorEmitter *emitter);
void particle_emitAnchorBurst(AnchorEmitter *emitter);
void particle_emitAtRandomAnchor(AnchorEmitter *emitter);
s32 particle_followAnchorEased(EmitterParticle *particle, Vec3f *target);
s32 particle_followAnchor(EmitterParticle *particle, Vec3f *target);
void particle_launchAnchorParticle(EmitterParticle *particle, Vec3f *dest);
s32 particle_getEmitterKindForObj(Actor *obj, u8 *kind, u8 variant);
const u8 *particle_getObjEmitterData(Actor *obj, s32 *found);
LightEffect *particle_spawnAnchorEmitterSprite(AnchorEmitter *emitter, Actor *owner, u8 kind, s16 duration, u8 fxOwner, s32 fxList);

/* game/Particle/AnchorEmitterDestroy/anchoremitterdestroy.c */
void particle_freeAnchorEmitterSprite(LightEffect *obj);
void particle_destroyAnchorEmitter(AnchorEmitter *emitter);
void particle_sendActorGoneToEmitters(Actor *actor);

/* game/Particle/ActorEmitterData/actoremitterdata.c (data only, linked after the game/Particle/ActorEmitter/actoremitter..game/Effects/BurnDamage/burndamage unit) */
/* The emitter tables of game/Particle/ActorEmitter/actoremitter.c (its gActorEmitterTables). */
extern EmitterTable D_8008F780, gAllOverFireEmitterTable, gTailFireEmitterTable, D_8008F7A4, D_8008F7B0, D_8008F7BC, D_8008F7C8, D_8008F7D4;
extern EmitterTable gAllOverSmokeEmitterTable, gTailSmokeEmitterTable, gLavaBoulderImpactEmitterTable, D_8008F804, gFireImpImpactEmitterTable, D_8008F81C, D_8008F828, D_8008F834;

/* game/Particle/Drips/drips.c */
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
    /* 0x1C */ f32 stretch;         /* tilt per sqrt of sideways speed (particle_buildDripQuad) */
    /* 0x20 */ u8 dropKind;         /* gDropKinds index */
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

/* A effect_newTimedEffectNoDraw task that keeps the link of an emitter that could not be created yet. */
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
    /* 0x2C */ u8 kind;             /* gKindSpriteTextures / D_800AB404 index */
    /* 0x30 */ DripSlot slotStorage[DRIP_SLOT_COUNT];
} DripEmitter; /* size 0x300 */

/* Lifetime and wobble of a kind of falling drop (gDropKinds, game/Particle/DripData/dripdata.c). */
typedef struct {
    /* 0x00 */ s16 lifeMin;
    /* 0x02 */ s16 lifeRange;
    /* 0x04 */ Size2f baseSize;
    /* 0x0C */ Size2f wobble;
} DropKind; /* size 0x14 */

/* A point on an actor's bone where a drop hangs. */
typedef struct {
    /* 0x0 */ Vec3f offset;
    /* 0xC */ u8 bone;
} BoneMount; /* size 0x10 */

/* An emitter kind (gDripEmitterKinds, game/Particle/DripData/dripdata.c; ActorLink.kind). */
typedef struct {
    /* 0x00 */ u8 dropKind;         /* gDropKinds index */
    /* 0x04 */ s32 numMounts;
    /* 0x08 */ const BoneMount *mounts;
    /* 0x0C */ f32 rate;            /* new drops per unit of gFrameDelta */
    /* 0x10 */ s32 maxDrops;
    /* 0x14 */ s16 holdMin;         /* gFrameTicks ticks a drop hangs before it falls */
    /* 0x16 */ s16 holdRange;
} EmitterKind; /* size 0x18 */

DripEmitter *particle_newDripEmitter(ActorLink *link, s32 extraSize, u8 owner, s32 list);
void particle_destroyDripEmitter(DripEmitter *emitter);
void particle_freeDripEmitter(DripEmitter *emitter);
void particle_updateDripEmitter(DripEmitter *emitter);
s32 particle_wobbleDrip(DripParticle *drop);
Vtx *particle_buildDripQuad(DripParticle *drop, s16 vp);
void particle_releaseDrips(DripEmitter *emitter);
s8 particle_fallDrip(DripParticle *drop);
s32 particle_fadeDrip(DripParticle *drop);
void particle_retryDripEmitter(DripRetryTask *task);
void particle_onDripEmitterMsg(DripEmitter *emitter, ActorRef *ref, u8 msg);
void particle_onDripRetryMsg(DripRetryTask *task, ActorRef *ref, u8 msg);
/* (ActorRef *ref, u8 msg); declared without a prototype: game/Anim/IdleAnim/idleanim.c passes the ref as an integer */
void particle_sendDripMsg();

/* game/Particle/DustEmitter/dustemitter.c */
/* Per-particle data of a dust particle (particle_spawnDustParticle, updated by particle_updateDustParticle). */
typedef struct DustData {
    /* 0x00 */ f32 baseSize;
    /* 0x04 */ u8 phase[2];
    /* 0x06 */ s8 phaseStep[2];
    /* 0x08 */ f32 amplitude[2];
    /* 0x10 */ f32 floorY;
    /* 0x14 */ CollisionTri floor;  /* the floor triangle under the spawn point, for the splash */
    /* 0x26 */ u8 kind;
} DustData; /* 0x28 */

/* A sprite particle (particle_newOwnedSpriteParticle) carrying DustData. */
typedef struct DustParticle {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ DustData dust;
} DustParticle; /* size 0xD0 */

/* Data of the dust emitter effect (particle_spawnDustEmitter, run by particle_updateDustEmitter). */
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

DustEmitter *particle_spawnDustEmitter(Actor *owner, const Vec3f *offset, u8 bone, f32 rateBase, f32 rateRandom, s16 duration,
                           f32 gravityBase, f32 gravityRandom, s16 lifetimeBase, s16 lifetimeRandom, u8 alphaBase,
                           u8 alphaRandom, f32 sizeBase, f32 sizeRandom, f32 unk60, u8 kind, u8 fxOwner, s32 fxList);
void particle_updateDustEmitter(DustEmitter *effect);
DustParticle *particle_spawnDustParticle(Vec3f *position, Vec3f *velocity, f32 gravity, s16 lifetime, u8 alpha, u8 brightness,
                            f32 size, s32 unused, f32 floorY, CollisionTri *floor, u8 kind, u8 owner, s32 list);
u8 particle_pulseDust(DustParticle *particle, Vec3f *prevPos);
u8 particle_landDust(DustParticle *particle, Vec3f *prevPos);
u8 particle_updateDustParticle(DustParticle *particle, Vec3f *prevPos);
DustEmitter *particle_startObjDustEmitter(Actor *obj, s16 duration, u8 owner, s32 list);
void particle_onDustEmitterEvent(DustEmitter *effect, s32 event, u8 eventType);

/* game/Particle/BillboardRenew/billboardrenew.c */
void particle_renewBillboard(BillboardParticle *p);

/* game/Particle/SideEmitter/sideemitter.c */
/* Data of the effects of this file that follow an object: an ObjectEmitterData (game/Particle/SegmentEmitter/segmentemitter.c) without the
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

void particle_attachSideEmitter(Actor *object);
void particle_updateSideEmitter(Effect1BFDD0 *effect);
Effect1BFDD0 *particle_attachBoneSideEmitter(Actor *object, s16 type, u8 owner, s32 list);
void particle_updateBoneSideEmitter(Effect1BFDD0 *effect);
void particle_onSideEmitterMsg(Effect1BFDD0 *effect, void *msg, u8 msgType);
void particle_onBoneSideEmitterMsg(Effect1BFDD0 *effect, void *msg, u8 msgType);

/* game/Particle/SegmentBurst/segmentburst.c */
/* Particle emitter that particle_sampleSegmentPower initialises and particle_emitOnSegment spawns from. */

void particle_spawnBoneModelBurst(Actor *actor, u8 owner, s32 list);
void particle_attachBloodStrip(Actor *owner, s32 fxOwner, s32 fxList);
s32 particle_sampleSegmentPower(BloodStrip *emitter);
void particle_emitOnSegment(Vec3f *from, Vec3f *to, s32 velocityA, s32 velocityB, f32 t, BloodStrip *emitter);

/* game/Particle/FastSpark/fastspark.c */
/* Data of the spark emitter effect, stored at 0x28 in the effect object from effect_newTimedEffectNoDraw. */
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

void particle_startSparkEmitter(Actor *actor, s16 duration, u8 owner, s32 list);
void particle_updateSparkEmitterEffect(SparkEmitterEffect *effect);
s32 particle_spawnFastSpark(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s32 angle,
                  s32 baseAngle, f32 radius, s32 variant, f32 arg12, void *data, u8 owner);

/* game/Particle/SphereEmitter/sphereemitter.c */
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

/* Timed effect of the sphere emitters below (particle_updateGatherEmitter, particle_updateHoverEmitter, particle_updateSwellEmitter). */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EmitterParams params;
} SphereEmitter;

/* Object that spawns a effect_newTimedEffectNoDraw effect, passing on its settings from 0x28. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Words3;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Words3 pos;
    /* 0x34 */ f32 radius;
    /* 0x38 */ f32 spawnRate;
    /* 0x3C */ s16 lifetime;
    /* 0x3E */ s16 frameStep;
    /* 0x40 */ s16 frameStepRange;
    /* 0x42 */ s16 size;
    /* 0x44 */ s16 sizeRange;
    /* 0x46 */ s16 growRate;
    /* 0x48 */ s16 growRateRange;
} EffectSpawner;

void func_151A0E40(f32 x, f32 y, f32 z, u8 arg3, s32 arg4);
void func_151A0F28(f32 x, f32 y, f32 z, u8 arg3, s32 arg4);
void func_151A1010(f32 x, f32 y, f32 z, s16 duration, f32 scaleA, f32 scaleB, u8 arg6, s32 arg7);
void func_151A11CC(f32 x, f32 y, f32 z, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9,
                   f32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14, s32 arg15, s32 arg16, f32 arg17, s32 arg18,
                   s32 arg19, s32 arg20, s32 arg21, s32 arg22, s32 arg23, s32 arg24, s32 arg25, s32 arg26, s32 arg27);
void particle_updateGatherEmitter(SphereEmitter *emitter);
void particle_updateGatherParticle(BillboardParticle *particle);
void particle_startHoverEmitter(EffectSpawner *spawner);
void particle_updateHoverEmitter(SphereEmitter *emitter);
void particle_updateHoverParticle(BillboardParticle *particle);
void particle_startSwellEmitter(EffectSpawner *spawner);
void particle_updateSwellEmitter(SphereEmitter *emitter);
void particle_updateSwellParticle(BillboardParticle *particle);
void particle_updateSwellTint(BillboardParticle *particle);
s32 particle_spawnDragFadeSprite(Vec3f *pos, Vec3f *offset, Vec3f *vel, f32 drag, f32 gravity, f32 size, s16 timer, u8 alpha,
                  s16 fadeStart, s16 fadeOutStart, s16 eventTime, s8 eventFunc, u8 r, u8 g, u8 b, u8 owner, s32 list);
s32 particle_updateDragFade(SpriteParticle *particle, Vec3f *prevPos);
void particle_burstAtYawPitch(Vec3f *pos, s16 yaw, s16 pitch, f32 speed, f32 sizeBase, f32 sizeSlope, s32 count, f32 spread,
                   f32 drag, f32 gravityBase, f32 gravityRange, s16 delayBase, s16 delayRange, s16 alphaBase,
                   s16 alphaRange, s16 fadeStart, s16 fadeOutStart, s8 eventFunc, u8 owner, s32 list);
void particle_burstAlongDir(Vec3f *pos, f32 *dir, f32 sizeBase, f32 sizeSlope, s32 count, f32 spread, f32 drag, f32 gravityBase,
                   f32 gravityRange, s16 delayBase, s16 delayRange, s16 alphaBase, s16 alphaRange, s16 fadeStart,
                   s16 fadeOutStart, s8 eventFunc, u8 owner, s32 list);
void particle_getBurstVelAlongDir(f32 *dir, s32 yaw, s32 pitch, f32 speed, f32 t, Vec3f *out);
void particle_getBurstVelAtYawPitch(f32 *dir, s32 yaw, s32 pitch, f32 speed, f32 t, Vec3f *out);
void particle_spawnDragFadeBurst(Vec3f *pos, f32 *dir, s16 yaw, s16 pitch, f32 speed, f32 sizeBase, f32 sizeSlope, s32 count,
                   f32 spread, f32 drag, f32 gravityBase, f32 gravityRange, s16 delayBase, s16 delayRange,
                   s16 alphaBase, s16 alphaRange, s16 fadeStart, s16 fadeOutStart, s8 eventFunc, u8 owner, s32 list,
                   u8 useAngles);
void particle_spawnTrailBurstAtSprite(SpriteParticle *particle);
void particle_spawnHeavyTrailBurstAtSprite(SpriteParticle *particle);

/* game/Particle/RegionEmitter/regionemitter.c */
typedef struct Game1D3420Listener Game1D3420Listener;
typedef struct Game1D2B10QuadVertex Game1D2B10QuadVertex;
typedef struct Game1D2B10QuadOwner Game1D2B10QuadOwner;
void particle_updateRegionEmitter(u8 *effect);
void particle_destroyRegionEmitter(TimedEffect *effect);
void particle_freeRegionEmitter(TimedEffect *effect);
void particle_stopEffectSound(u16 soundHandle);
QuadSprite *particle_spawnSurfaceMark(f32 size, u8 alpha, CollisionTri *surface, Vec3f *pos, s16 duration, u8 lit, u8 arg6,
                          u8 owner, s32 list);
void particle_handleRegionEmitterMessage(Game1D3420Listener *listener, void *data, u8 type);
void particle_fireRegionEmitter(TriggerZone *emitter, ZoneNode *zone, u8 playSound, u8 arg3, u8 owner, s32 list);
Game1D2B10QuadVertex *particle_buildYawedVerticalQuad(Game1D2B10QuadOwner *owner, s16 index);

/* game/Particle/ZoneEmitter/zoneemitter.c */
/* State at 0x110 of the particle emitter over a TriggerZone (particle_updateZoneEmitter, particle_shutZoneEmitter). */
typedef struct ZoneEmitterState {
    /* 0x00 */ TriggerZone *region;
    /* 0x04 */ f32 texScroll;      /* texture s of the quad's first edge (particle_buildYawedVerticalQuad) */
    /* 0x08 */ f32 texScrollSpeed;
    /* 0x0C */ f32 texSpan;        /* texture s span */
    /* 0x10 */ LightEffect *light; /* light_createJitterAt's light; released by particle_shutZoneEmitter */
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

s32 particle_updateZoneEmitter(ZoneEmitter *obj);
void particle_shutZoneEmitter(ZoneEmitter *obj);

/* game/Particle/EmitterQuads/emitterquads.c */
s32 particle_spawnSampleKindMark(MotionTrail *state, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface);
typedef struct FxSpawnPayload FxSpawnPayload; /* defined in conker/effect.h */
s32 particle_spawnYawSteam(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s32 yaw,
                  s32 baseYaw, f32 radius, s32 kind, f32 arg12, FxSpawnPayload *payload, u8 fxOwner);
Vtx *particle_buildQuadParticleBillboard(DefParticleHead *particle, s16 view);
s32 particle_growQuadParticle(DefParticleHead *particle);
s32 particle_fadeQuadParticleByLife(DefParticleHead *particle);

/* game/Particle/DripData/dripdata.c (data only, linked after game/Rope/SwingRope/swingrope) */
/* The drip data of game/Particle/Drips/drips.c. */
extern const DropKind gDropKinds[];
extern EmitterKind gDripEmitterKinds[];

/* game/Particle/SideSpray/sidespray.c */
/* Growth applied by particle_updateSpray, stored in the spray particle at 0x128. */
typedef struct {
    /* 0x00 */ s16 growEndTimer; // grows while desc.life is above this
    /* 0x04 */ f32 growRate;     // size gained per frame
} SprayGrowth;

/* The spray particle particle_spawnSideSpray spawns; updated by particle_updateSpray. */
typedef struct {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ u8 pad110[0x18];
    /* 0x128 */ SprayGrowth growth;
} SprayParticle;

s32 particle_getSpraySidePos(Vec3f *pos, Actor *actor, CollisionHit *ground, u8 side);
s32 particle_selectSideSpray(s32 kind, Actor *actor);
void particle_spawnSideSpray(Actor *actor, s32 side, s32 arg2);
s32 particle_fadeSideSpraySprite(QuadSprite *sprite);
s32 particle_updateSpray(SprayParticle *spray);
s32 particle_spawnYawSpraySpark(s32 shape, s32 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s16 yaw, s32 baseAngle, f32 radius,
                  s32 arg11, s32 arg12, void *data, u8 owner);
s32 particle_dampSprayXZ(SparkParticle *spark, Vec3f *prevPos);

/* game/Particle/OwnerSideEmitter/ownersideemitter.c */
/* Per-task data kept at offset 0x28 of the task created by particle_startOwnerSideTask. */
typedef struct {
    /* 0x0 */ Actor *owner;
    /* 0x4 */ u8 ownerId;
    /* 0x8 */ f32 timer;
} Data151B1AB0;

typedef struct {
    TimedEffect base;
    Data151B1AB0 data;
} Task151B1AB0;

void particle_startOwnerSideTask(Actor *owner);
void particle_updateOwnerSideTask(Task151B1AB0 *task);
void particle_onOwnerSideTaskMsg(Task151B1AB0 *task, ActorMsg *msg, u8 kind);

/* game/Particle/SoundPulse/soundpulse.c */
/* Object whose sound handle particle_stopPulseSound stops. */
typedef struct {
    /* 0x00 */ u8 pad0[0x88];
    /* 0x88 */ s32 sound;
} SoundOwner;

PulseParticle *particle_spawnSoundPulse(Vec3f *pos, Vec3f *vel, f32 size, Vec3f *rot, Vec3f *rotVel, f32 gravity, s16 lifetime,
                             u8 alpha, s32 unused, f32 elasticity, u8 fadeShift, u8 bounces, u8 owner, s32 list);
void func_151B5E8C(void);
void particle_updateSoundPulse(PulseParticle *particle);
void particle_stopPulseSound(SoundOwner *obj);

/* game/Particle/Pulse/pulse.c */
PulseParticle *particle_spawnPulse(Vec3f *pos, Vec3f *vel, f32 size, Vec3f *rot, Vec3f *rotVel, f32 gravity, s16 lifetime,
                             u8 alpha, s32 unused, f32 elasticity, u8 fadeShift, u8 bounces, u8 owner, s32 list);
void particle_updatePulseWobble(PulseParticle *particle);

/* Sprite particle types of game/Particle/Steam/steam.c, here before their first use in a prototype. */
/* Per-particle data of a particle_spawnSteamParticle particle (SteamParticle.data). */
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

/* Per-particle data of a particle_spawnKindSprite particle (SteamParticle.data). */
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

/* A live sprite particle as returned by particle_newSpriteParticleWithFloor. */
typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ union {
        SteamWobble wobble;
        ParticleSpin spin;
    } data;
} SteamParticle;

/* game/Particle/Bob/bob.c */
/* Head of a pooled physics particle (full layout in game/Particle/ModelParticle/modelparticle.c), with the bob in its vel/gravity slots. */
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

/* Motion extension of a model effect (EffectMotion in game/Effects/ModelEffect/modeleffect.c), with the bob in its vel/gravity slots. */
typedef struct {
    /* 0x00 */ Vec3f rot;
    /* 0x0C */ f32 scale;
    /* 0x10 */ f32 phase;        // vel.x
    /* 0x14 */ f32 center;       // vel.y
    /* 0x18 */ f32 speed;        // vel.z
    /* 0x1C */ Vec3f rotVel;
    /* 0x28 */ f32 amplitude;    // gravity
} EffectBob;

/* Model effect (Effect in game/Effects/ModelEffect/modeleffect.c; only the fields used here). */
typedef struct {
    /* 0x000 */ u8 pad0[0x54];
    /* 0x054 */ Vec3f pos;      // params.pos
    /* 0x060 */ u8 pad60[0x120 - 0x60];
    /* 0x120 */ EffectBob bob;   // extra
} BobEffect;

/* Bob state that follows a game/Particle/QuadParticle/quadparticle.c particle (another member of its DefParticleMotion union). */
typedef struct {
    /* 0x00 */ f32 center;
    /* 0x04 */ f32 amplitude;
    /* 0x08 */ f32 phase;
    /* 0x0C */ f32 speed;
    /* 0x10 */ Vec3f rotVel;
} QuadBob;

/* Quad particle of game/Particle/QuadParticle/quadparticle.c (only the fields used here), followed by its motion state. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ QuadBob bob;
} BobQuad;

s32 particle_bobModelParticle(BobParticle *particle);
s32 particle_bobSteamParticle(SteamParticle *particle, Vec3f *prevPos);
s32 particle_bobEffect(BobEffect *effect);
s32 particle_bobQuad(BobQuad *particle);

/* game/Particle/FallingParticle/fallingparticle.c */
/* A particle spawned by particle_spawnFallingParticle (layout as Particle in game/Effects/ObjectList/objectlist.c). */
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

/* Type 7 object (0x2C bytes): emits particle_spawnFallingParticle particles every frame until its timer runs out. */
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

void particle_spawnFallingParticle(s32 animIndex, s32 kind, s16 x, s16 y, s16 z, s16 unused, f32 angle, s16 angleSpread, s16 velY,
                   s16 velYRange, s16 speed, s16 speedRange, s16 radius, s16 minSize, s16 size, u8 owner, s32 list);
void particle_updateFallingParticle(FallingParticle *particle);
void particle_newFallingEmitter(s32 animIndex, s32 kind, s16 x, s16 y, s16 z, s16 velY, s16 velYRange, s16 speed, s16 speedRange,
                   s16 minSize, s16 size, s16 count, s16 timer, f32 *followPos, u8 owner, s32 list);
void particle_updateFallingEmitter(FallingEmitter *emitter);

/* game/Particle/WobbleStream/wobblestream.c */
typedef struct Game1F1CD0Actor {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ProjectileStream emission;
} Game1F1CD0Actor;

void particle_updateWobbleStream(Game1F1CD0Actor *actor);

/* game/Particle/HitImpact/hitimpact.c */
extern const Vec3f *gGunMuzzlePoints[]; /* muzzle point of each gun (game/Weapon/GunShot/gunshot reads gun 0) */
extern u8 *gGunMuzzleBones[];          /* the bone that point is on */
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
    /* 0x020 */ Vec3f rot;
    /* 0x02C */ u8 pad2C[0x50 - 0x2C];
    /* 0x050 */ Vec3f restRot;
    /* 0x05C */ u8 pad5C[0x170 - 0x5C];
    /* 0x170 */ BobMotion wobble;
} WobbleParticle;

/* Effect user data that follows a point on an object, computed by gEmitterPointFuncs[posFunc]. */
typedef struct {
    /* 0x00 */ Actor *obj;
    /* 0x04 */ u8 uniqueId;
    /* 0x06 */ s16 posFuncArg;
    /* 0x08 */ u8 posFunc;
    /* 0x0C */ f32 spawnTimer;
    /* 0x10 */ u8 flags; /* bit 0: pos is valid */
    /* 0x14 */ Vec3f pos;
} ObjEmitter;

/* An effect created by particle_attachPointEmitter; its user data is an ObjEmitter. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ObjEmitter emitter;
} ObjEmitterEffect;



void particle_spawnHitImpact(Vec3f *pos, Vec3f *dir, CollisionTri *surface, u8 kind, u8 owner, s32 list);
s32 particle_settleWobble(WobbleParticle *particle);
void particle_getBonePoint(Actor *obj, Vec3f *out, const Vec3f *localPos, u8 bone, f32 yOffset);
void particle_runAttackAtMidpoint(Actor *obj, Vec3f *a, Vec3f *b, s32 attackId);
void particle_spawnShortLightFlash(Vec3f *pos, u8 owner, s32 list);
void particle_spawnLightFlash(const Vec3f *pos, u8 owner, s32 list);
void particle_runAttackPairAt(Vec3f *pos, Vec3f *dir, Actor *obj, Actor *target, CollisionTri *surface, s32 firstAttackId, s32 secondAttackId,
                   s32 losSkipSlot);
void particle_runAttackAlongDir(Vec3f *pos, Vec3f *dir, Actor *obj, CollisionTri *surface, s32 attackId);
void particle_spawnBoneDirParticle(Vec3f *pos, Vec3f *dir, void *mtx, Actor *owner, f32 scale, u8 fxOwner, s32 fxList);
void particle_detachObjEmitters(Actor *obj);
void particle_attachPointEmitter(); /* (Actor *obj, u8 posFunc, s16 lifetime, u8 owner, s32 list); K&R definition, declared without a prototype */
void particle_updatePointSmoke(ObjEmitterEffect *effect);
void particle_onPointEmitterMsg(ObjEmitterEffect *effect, ActorRef *handle, u8 msg);
s32 particle_updatePointPuff(SpriteParticle *puff, Vec3f *prevPos);
void particle_attachTwinEmitters(Actor *obj);
s32 particle_applyShotHit(Actor *victim, Actor *attacker, Vec3f *from, Vec3f *to, RayHit *hit, s32 flags, Vec3f *dir, u8 owner,
                  s32 list);
void particle_countShot(Actor *obj);
void particle_intersectAimPlane(Actor *obj, Vec3f *position, Vec3f *direction, Vec3f *axis0, Vec3f *axis1, Vec3f *pointBuf, Vec3f *dirBuf,
                   Vec3f **outPoint, Vec3f **outDir, Vec3f *endpoint);
void particle_startPlayerCameraWobbles(Vec3f *pos, f32 radius, f32 fadeRange, f32 invFadeRange, u8 presetIndex, u8 owner, s32 list);
void particle_startPlayerRumbles(Vec3f *pos, f32 radius, f32 fadeRange, f32 invFadeRange, s32 base, s32 range, s32 owner, s32 list);
void particle_spawnBlastSpray(const Vec3f *pos, u8 owner, s32 list);
void particle_spawnBlast(Vec3f *pos, u8 owner, s32 list);
void particle_spawnBoneSegmentParticle(Actor *obj, const Vec3f *from, const Vec3f *to, u8 bone, f32 scale, u8 owner, s32 list);
void particle_castGunRay(Actor *obj, u8 slot);
void particle_getGunAxes(Actor *obj, Vec3f *out, Vec3f *forward, Vec3f *axis1, Vec3f *axis2, const Vec3f *localPos, u8 bone);
s32 particle_raySphereObjectMask(Vec3f *p0, Vec3f *p1, Actor *self, s8 filter, void *filterArg);
void particle_getViewBuffer(void *slots, s16 index, s32 size, void *out, u8 *allocated);
void particle_freeViewBuffers(void *slots);

/* game/Particle/Steam/steam.c */
extern u8 gLevelEffectKind;              /* the effect kind of the current context (particle_getContextEffectKind) */
extern u8 (*gKindParticleTexturePickers[13])(void); /* texture id pickers by effect kind */
extern u8 (*D_8008FD04[13])(void); /* a second picker table by effect kind */
/* Data of a particle_startSteamEffect effect: wobbles its scale and keeps spawning particle_spawnKindSprite particles. */
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

/* Data of a effect_newTimedEffect effect that drops a particle_spawnSteamParticle particle whenever its timer runs out. */
typedef struct {
    s16 timer;
    f32 size;
    Vec3f pos;
    u8 alpha;
    u8 lit;
    u8 kind;
} DripData; /* 0x18 */

typedef struct {
    TimedEffect base;
    DripData drip;
} DripEffect;

/* Data a splash carries for the bursts it makes on impact (particle_splashBurst, particle_splashSpray). */
typedef struct {
    f32 burstScale;
    f32 altBurstScale;
    u8 kind;
    u8 pad9[3];
} SplashBurst; /* 0xC */

/* The block a live splash points to at +0x98: its TrailParams, then its SplashBurst. */
typedef struct {
    /* 0x00 */ TrailParams params;
    /* 0x48 */ SplashBurst burst;
} SplashState;

typedef struct {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 buffers[0x10];
    /* 0x94 */ MotionTrailPoint *nodes;
    /* 0x98 */ SplashState *state;
} Splash;

/* Data of a particle_spawnSquash effect: squashes and stretches it on one phase. */
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

s32 particle_getContextEffectKind(void);
u8 particle_pickKind0Texture(void);
u8 particle_pickKind1Texture(void);
u8 particle_pickKind2Texture(void);
u8 particle_pickKind3Texture(void);
u8 particle_pickKind4Texture(void);
u8 particle_pickKind5Texture(void);
u8 particle_pickKind6Texture(void);
u8 particle_pickKind7Texture(void);
u8 particle_pickKind9Texture(void);
u8 particle_pickKind10Texture(void);
u8 particle_pickKind11Texture(void);
u8 particle_pickKind12Texture(void);
s32 particle_updateSteamParticle(SteamParticle *particle, Vec3f *prevPos);
s32 particle_wobbleSteamSize(SteamParticle *particle, Vec3f *prevPos);
u8 particle_steamFloorHit(SteamParticle *particle, Vec3f *prevPos);
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
void particle_spawnKindEmitter(u8 kind, f32 size, u8 alpha, void *tri, Vec3f *pos, s16 lifetime, u8 arg6, u8 lit, u8 withDrip, u8 owner,
                   s32 list);
void particle_updateDripEffect(DripEffect *effect);
void particle_spawnKindSplash(u8 kind, f32 size, u8 alpha, CollisionTri *surface, Vec3f *pos, u8 owner, s32 list);
SteamEffect *particle_startSteamEffect(u8 kind, f32 size, f32 shrink, u8 alpha, s16 lifetime, CollisionTri *surface, Vec3f *pos,
                          s32 owner, s32 list);
s32 particle_updateSteamEffect(SteamEffect *effect);
s32 particle_shrinkSteamEffect(SteamEffect *effect);
Splash *particle_spawnSplash(Vec3f *pos, Vec3f *vel, f32 size, s16 lifetime, u8 alpha, f32 scale, s32 itemCount, u8 hitsFloor,
                      f32 burstScale, f32 altBurstScale, u8 lit, u8 kind, CollisionHit *ground, s16 fadeStart, s16 fadeRate,
                      s32 material, u8 owner, s32 list);
s32 particle_splashBurst(Splash *splash, f32 x, f32 y, f32 z, f32 floorY, CollisionTri *surface);
s32 particle_splashSpray(Splash *splash, f32 x, f32 y, f32 z, f32 floorY, CollisionTri *surface);
void particle_spawnSquash(u8 kind, f32 size, u8 alpha, Vec3f *pos, volatile u8 lit, u8 owner, s32 list);
s32 particle_updateSquashEffect(SquashEffect *effect);
Vtx *particle_buildSquashQuad(DefParticleHead *effect, s16 view);
void particle_setKind0Colors(ParticleDesc *desc);
void particle_setKind1Colors(ParticleDesc *desc);
void particle_setKind2Colors(ParticleDesc *desc);
void particle_setKind3Colors(ParticleDesc *desc);
void particle_setKind4Colors(ParticleDesc *desc);
void particle_setKind5Colors(ParticleDesc *desc);
void particle_setKind6Colors(ParticleDesc *desc);
void particle_setKind7Colors(ParticleDesc *desc);
void particle_setKind8Colors(ParticleDesc *desc);
void particle_setKind9Colors(ParticleDesc *desc);
void particle_setKind10Colors(ParticleDesc *desc);
void particle_setKind12Colors(ParticleDesc *desc);
s32 particle_updateKindSprite(SteamParticle *particle, Vec3f *prevPos);
void particle_throwSplashes(Vec3f *pos, s32 count, u8 kind, u8 owner, s32 list);
void particle_spawnKindEmitterAbove(Vec3f *pos, CollisionHit *hit, u8 *kind, u8 owner, s32 list);
void particle_spawnKindMark(u8 kind, f32 size, s16 alpha, CollisionTri *surface, void *pos, u8 owner, s32 list);
void particle_startKindEffect(u8 kind, f32 size, f32 scaleX, s16 lifetime, Vec3f *pos, CollisionTri *tri, u8 alpha, u8 arg7,
                   u8 owner, s32 list);
void particle_spawnKindPlainSprite(Vec3f *pos, f32 size, u8 alpha, s16 lifetime, u8 kind, u8 owner, s32 list);
/* (Vec3f *pos, CollisionHit *ground, u8 kind, u8 owner, s32 list); declared without a prototype: game/Effects/ContactEffects/contacteffects.c
   passes the kind unnarrowed */
void particle_spawnSplashAndDrops();
/* (Vec3f *pos, CollisionHit *ground, u8 kind, u8 owner, s32 list); declared without a prototype: game/Effects/ContactEffects/contacteffects.c
   passes the kind unnarrowed */
void particle_spawnSmallSplashAndDrops();
extern const u8 gKindSpriteTextures[13]; // texture id per effect kind
extern const u8 gKindSplashTextures[13]; // particle texture id per effect kind
extern const u8 D_800AB404[13]; // flag per effect kind
extern const Color24 gKindColors[13]; // effect colours (RGB per colour index / effect kind)

/* game/Particle/ColorEmitter/coloremitter.c */
/* Data of the effect spawned by particle_startColorEmitter and updated by particle_updateColorEmitter. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 accum;        /* particles still to emit; particle_updateColorEmitter emits one per whole unit */
    /* 0x10 */ u8 randomColors;  /* nonzero: each particle takes one of three random colour pairs */
    /* 0x11 */ u8 pad11[3];
} ColorEmitterData; /* 0x14 */

/* Effect created by effect_newTimedEffect (only the fields used here). */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ColorEmitterData data;
} ColorEmitter;

void particle_spawnFlashBurst(Vec3f *pos, u8 owner, s32 list);
/* (Vec3f *pos, s32 duration, u8 burst, u8 randomColors, u8 owner, s32 list); declared without a prototype:
 * game/Cutscene/SeqEvents/seqevents.c passes an s32 owner without narrowing it */
void particle_startColorEmitter();
void particle_updateColorEmitter(ColorEmitter *emitter);

/* game/Particle/SmallDebris/smalldebris.c */
void particle_spawnSmallDebrisBurst(Vec3f *pos, u8 owner, s32 list);

#endif
