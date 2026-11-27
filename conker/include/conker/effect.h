#ifndef CONKER_EFFECT_H
#define CONKER_EFFECT_H

#include <ultra64.h>
#include "structs.h"
#include "conker/blood.h"
#include "conker/particle.h"
#include "conker/trail.h"

/*
 * Subsystem "effect": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Effects/HomingReset/homingreset.c
 *   game/Effects/ObjectReset/objectreset.c
 *   game/Effects/EffectQueueInit/effectqueueinit.c
 *   game/Effects/AttachedBurst/attachedburst.c
 *   game/Effects/ActorImpact/actorimpact.c
 *   game/Effects/PerimeterEmitter/perimeteremitter.c
 *   game/Effects/BoneStreak/bonestreak.c
 *   game/Effects/OwnerSpark/ownerspark.c
 *   game_E2DA0.c
 *   game/Effects/BreakUp/breakup.c
 *   game/Effects/JointDecal/jointdecal.c
 *   game/Effects/ParticleBursts/particlebursts.c
 *   game/Effects/SegmentTrail/segmenttrail.c
 *   game/Effects/EmitterSpawner/emitterspawner.c
 *   game_EB600.c
 *   game/Effects/BoneGroundFx/bonegroundfx.c
 *   game/Effects/BoneRing/bonering.c
 *   game/Effects/PulseEffect/pulseeffect.c
 *   game/Effects/WatcherEffects/watchereffects.c
 *   game/Effects/ParticleCallbacks/particlecallbacks.c
 *   game/Effects/BounceRecords/bouncerecords.c
 *   game/Effects/RingEffect/ringeffect.c
 *   game/Effects/RotatingZone/rotatingzone.c
 *   game/Effects/ActorEmitterTick/actoremittertick.c
 *   game/Effects/HopSprite/hopsprite.c
 *   game/Effects/TextLines/textlines.c
 *   game/Effects/ZoneSounds/zonesounds.c
 *   game_FE850.c
 *   game_FE8C0.c
 *   game/Effects/RaySpinner/rayspinner.c
 *   game/Effects/HitSpray/hitspray.c
 *   game/Effects/TintedObject/tintedobject.c
 *   game/Effects/PuffTimer/pufftimer.c
 *   game/Effects/FallingStream/fallingstream.c
 *   game/Effects/TrapEffect/trapeffect.c
 *   game/Effects/LevelHook14/levelhook14.c
 *   game/Effects/ZonePairEmitter/zonepairemitter.c
 *   game/Effects/RainHook/rainhook.c
 *   game/Effects/Explosion/explosion.c
 *   game_10CD70.c
 *   game/Effects/SplashBurst/splashburst.c
 *   game/Effects/PropEmitter/propemitter.c
 *   game/Effects/HomingEffect/homingeffect.c
 *   game/Effects/GroundHits/groundhits.c
 *   game/Effects/MaterialImpact/materialimpact.c
 *   game/Effects/DistantBolts/distantbolts.c
 *   game/Effects/Oscillator/oscillator.c
 *   game/Effects/GroundFlash/groundflash.c
 *   game/Effects/BoneGroundHit/bonegroundhit.c
 *   game/Effects/SwayObject/swayobject.c
 *   game/Effects/SurfaceSound/surfacesound.c
 *   game/Effects/BeamZones/beamzones.c
 *   game/Effects/ProjectilePuffs/projectilepuffs.c
 *   game/Effects/StageEffect/stageeffect.c
 *   game_127060.c
 *   game/Effects/StarField/starfield.s
 *   game/Effects/ChildEffect/childeffect.c
 *   game/Effects/ChildEffectFree/childeffectfree.c
 *   game/Effects/AreaSpray/areaspray.c
 *   game/Effects/TreadScroll/treadscroll.c
 *   game/Effects/BonePointFx/bonepointfx.c
 *   game/Effects/OwnerEffect/ownereffect.c
 *   game/Effects/LevelHook08/levelhook08.c
 *   game/Effects/FlagLights/flaglights.c
 *   game/Effects/BoneBeamParticle/bonebeamparticle.c
 *   game/Effects/DebrisThrow/debristhrow.c
 *   game/Effects/ImpactSplash/impactsplash.c
 *   game/Effects/DebrisEffect/debriseffect.c
 *   game/Effects/DynModel/dynmodel.c
 *   game/Effects/ActorEffects/actoreffects.c
 *   game/Effects/ActorDamageFx/actordamagefx.c
 *   game/Effects/EffectObj/effectobj.c
 *   game/Effects/TimedEffect/timedeffect.c
 *   game/Effects/ActorAttachments/actorattachments.c
 *   game/Effects/AttachmentTables/attachmenttables.c (data only, linked after game/Effects/ActorAttachments/actorattachments)
 *   game/Effects/PlayerMarker/playermarker.c
 *   game/Effects/ModelEffect/modeleffect.c
 *   game/Effects/EffectActor/effectactor.c
 *   game/Effects/CharacterWatcher/characterwatcher.c
 *   game/Effects/VertexSwing/vertexswing.c
 *   game_193420.c
 *   game/Effects/QuadEffect/quadeffect.c
 *   game/Effects/ObjectList/objectlist.c
 *   game/Effects/ModelBurst/modelburst.c
 *   game/Effects/LevelUpdate/levelupdate.c
 *   game/Effects/Rain/rain.c
 *   game/Effects/FadeBurst/fadeburst.c
 *   game/Effects/ColorAnim/coloranim.c
 *   game/Effects/TumblingPiece/tumblingpiece.c
 *   game/Effects/EffectQueue/effectqueue.c
 *   game/Effects/AnchorLight/anchorlight.c
 *   game/Effects/BurnDamage/burndamage.c
 *   game/Effects/TriggerSwap/triggerswap.c
 *   game/Effects/SurfaceHandlers/surfacehandlers.c
 *   game/Effects/FallingObject/fallingobject.c
 *   game/Effects/ContactEffects/contacteffects.c
 *   game_1CC420.c
 *   game/Effects/ZoneEffect/zoneeffect.c
 *   game/Effects/DebrisBurst/debrisburst.c
 *   game/Effects/ActorTasks/actortasks.c
 *   game/Effects/ImpactSets/impactsets.c
 *   game/Effects/TaskGroundFx/taskgroundfx.c
 *   game/Effects/LandingBurst/landingburst.c
 *   game/Effects/SpawnEmitterHook/spawnemitterhook.c
 *   game/Effects/RibbonChild/ribbonchild.c
 *   game/Effects/FollowEffects/followeffects.c
 *   game/Effects/StatCounter/statcounter.c
 *   game/Effects/ActorSoundBurst/actorsoundburst.c
 *   game/Effects/BoneFollower/bonefollower.c
 */

/* Two texture-id tables of five animation frames each (gBounceRecordFrameTables in game/Effects/BounceRecords/bouncerecords.c, gTexAnimFrameTables in game/Effects/TintedObject/tintedobject.c). */
typedef struct TextureFrameTables {
    /* 0x0 */ s32 *frames[2];
} TextureFrameTables; /* size 0x8 */


/* Level effect spawners (game/Effects/EmitterSpawner/emitterspawner.c, game_EB600.c). */
/* The 8-byte payload effect_newEmitterSpawner copies to +0x28 of the object it creates. */
typedef struct {
    /* 0x00 */ Camera *owner;
    /* 0x04 */ s16 timer; // countdown to the next emitter spawn
    /* 0x06 */ s16 unk6;
} EB340Spawn;

/* The object effect_newEmitterSpawner creates; effect_updateEmitterSpawner is its per-frame update. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EB340Spawn spawn;
} EB340Obj;

/* Copied to +0x70 of the emitter object sprite_newScreenSprite returns. */
typedef struct {
    /* 0x00 */ Camera *owner;
    /* 0x04 */ u8   pad4[0x8];
    /* 0x0C */ u8   unkC;
    /* 0x0D */ u8   unkD;
    /* 0x0E */ u8   padE[0x2];
    /* 0x10 */ f32  unk10;
    /* 0x14 */ u8   pad14[0x44];
} EB340Payload;

typedef struct {
    /* 0x00 */ u8  pad0[0x14];
    /* 0x14 */ f32 y;      /* ScreenSprite info.y */
    /* 0x18 */ u8  pad18[0x68];
    /* 0x80 */ f32 ySpeed;
} EB340Rising;

/* Behaviour scratch state (WorldObject.touch) of func_150BE210. */
typedef struct {
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ f32 angle; // turns at spinY per frame until past 360
} TurnerWork;

#define TURNER_WORK(obj) ((TurnerWork *)&(obj)->touch)

/* Behaviour scratch state (WorldObject.touch) of effect_updatePathMover (game_EB600.c): the end points of its path. */
typedef struct {
    /* 0x7C */ s16 from[3];
    /* 0x82 */ s16 to[3];
} PathMoverWork;

#define PATH_MOVER_WORK(obj) ((PathMoverWork *)&(obj)->touch)

/* Effect kind 4 of game/Effects/QuadEffect/quadeffect.c (0xA8 bytes from effect_newObject): one textured quad, double-buffered by
 * gDrawBufferIndex. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Vtx quads[2][4];
    /* 0x90 */ s8 dirX;       // -cos(direction) * 127, mode 1 only
    /* 0x91 */ s8 dirZ;       //  sin(direction) * 127, mode 1 only
    /* 0x92 */ u8 life;       // frames left; fades out over the last 3/4 of lifeMax
    /* 0x93 */ u8 lifeMax;
    /* 0x94 */ u8 unk94;
    /* 0x95 */ u8 mode;       // 0: flat in XZ, 1: upright facing dirX/dirZ, 2: rotated by angleA/angleB
    /* 0x96 */ s16 growthRate;
    /* 0x98 */ s16 x;
    /* 0x9A */ s16 y;
    /* 0x9C */ s16 z;
    /* 0x9E */ s16 size;      // half the quad's edge length
    /* 0xA0 */ u8 angleA;
    /* 0xA1 */ u8 angleB;
    /* 0xA2 */ u8 padA2[0x6];
} QuadEffect;

/* game/Effects/HomingReset/homingreset.c */
void effect_clearHomingEffects(void);

/* game/Effects/ObjectReset/objectreset.c */
void effect_clearObjectLists(s32 mode);
void effect_runResetCallbacks(s32 mode);

/* game/Effects/EffectQueueInit/effectqueueinit.c */
void effect_initQueue(void);
void effect_allocQueue(void);
void func_15008BE0(void);

/* game/Effects/ObjectList/quadfxdraw.s */
Gfx *effect_drawQuadFx(Gfx *gfx, QuadFx *fx, s16 viewIndex); /* draw handler of object type 14 */

/* game/Effects/AttachedBurst/attachedburst.c */
typedef struct Particle150AED9C Particle150AED9C;
typedef struct AttachedEffect AttachedEffect;
void effect_levelHook02(Camera *arg0);
void effect_updateAttachedBurst(AttachedEffect *effect);
void effect_spawnAttachedBursts(Actor *actor);
void func_150AECCC(struct42 *arg0);
void effect_growBillboard(BillboardParticle *p);
s32 func_150AED9C(Particle150AED9C *particle);
s32 effect_fadeQuadSpriteByLife(QuadSprite *sprite);
void effect_attachedEffectMessage(AttachedEffect *effect, ActorMsg *msg, u8 type);

/* game/Effects/ActorImpact/actorimpact.c */
void effect_spawnActorImpactDebris(Actor *actor, Actor *other, u8 arg2, s32 arg3);

/* game/Effects/PerimeterEmitter/perimeteremitter.c */
void effect_pickPerimeterPoint(Actor *owner, Actor *player, f32 *forward, f32 *right, Vec3f *outPos, s16 *outAngle);
void effect_attachPerimeterEmitter(Actor *owner);
void effect_spawnPerimeterEmitter(Actor *owner, u8 arg1, s32 arg2);
void effect_perimeterEmitterMessage(DDB60Effect *effect, s32 arg1, u8 arg2);
void effect_updatePerimeterEmitter(DDB60Effect *effect);

/* game/Effects/BoneStreak/bonestreak.c */
void func_150B3AB0(Actor *obj, u8 arg1);
s32 func_150B3C0C(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 pitch, s32 yaw, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 owner);

/* game/Effects/OwnerSpark/ownerspark.c */
typedef struct OwnerSparkEffect OwnerSparkEffect;
OwnerSparkEffect *effect_spawnOwnerSpark(Actor *owner, s16 duration, u8 arg2, s32 arg3);
void effect_updateOwnerSpark(OwnerSparkEffect *effect);
void effect_releaseOwnerSparkAnchors(OwnerSparkEffect *effect);
void effect_destroyOwnerSpark(OwnerSparkEffect *effect);
void effect_freeOwnerSpark(OwnerSparkEffect *effect);
void effect_ownerSparkMessage(OwnerSparkEffect *effect, ActorMsg *event, u8 eventId);

/* game_E2DA0.c */
typedef struct ObjBurstEffect ObjBurstEffect;
typedef struct BoltRingEffect BoltRingEffect;
typedef struct Arg150B66DC Arg150B66DC;
GameCommand *effect_emitFacingCommand(GameCommand *cmd, s32 objIndex);
void effect_updateObjBurst(ObjBurstEffect *effect);
void effect_spawnObjBurstAt(Vec3f *pos, u8 owner, s32 kind);
void effect_spawnLargeObjBurstAt(Vec3f *pos, u8 owner, s32 kind);
void effect_spawnObjBurstParticle(Vec3f *pos, u8 owner, s32 kind);
void effect_spawnObjBurstLight(Vec3f *pos, u8 owner, s32 kind);
void effect_getBone5Point(Actor *obj, Vec3f *out);
void effect_updateBoltRing(BoltRingEffect *effect);
void effect_boltRingMessage(EffectHeader *effect, s32 arg1, u8 msg);
void effect_spawnScatterTrailVariant(u8 variant);
s32 func_150B66DC(Arg150B66DC *ctx);
void effect_flashViewLightBlue(u8 owner, s32 kind);

/* game/Effects/BreakUp/breakup.c */
void effect_spawnBreakUp(Actor *obj, u8 arg1, s32 arg2);
void effect_spawnBreakUpPieces(Vec3f *center, f32 *height, f32 *radius, f32 *yScale, u8 arg4, s32 arg5);
void effect_spawnBreakUpParticle(Vec3f *center, f32 *height, f32 *radius, u8 arg3, s32 arg4);
s32 func_150B879C(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);

/* game/Effects/JointDecal/jointdecal.c */
/* A QuadSprite run by effect_startDecalFade / effect_updateFadeQuadSprite, with its fade as extra data. */
typedef struct FadeQuadSprite {
    /* 0x00 */ QuadSprite quad;
    /* 0xB0 */ s16 fadeStart;      /* lifetime below which quad.def.unk10 fades */
    /* 0xB2 */ s16 fadeStep;       /* unk10 is capped at lifetime * fadeStep */
} FadeQuadSprite;

extern struct Queue *gGroundDecalQueue;
void effect_spawnJointStepEffects(Actor *obj, u8 side, u8 arg2);
s32 effect_spawnJointSpark(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
s32 effect_dampJointSparkXZ(SparkParticle *particle, s32 arg1);
void effect_startDecalFade(FadeQuadSprite *sprite);
s32 effect_updateFadeQuadSprite(FadeQuadSprite *sprite);
s32 effect_spawnJointPiece(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, s32 arg13, u8 arg14);

/* game/Effects/ParticleBursts/particlebursts.c */
struct Obj150BCFB8;
s32 func_150BB760(Actor *actor);
s32 func_150BBB5C(Actor *actor);
s32 func_150BC488(Actor *actor);
s32 func_150BCBBC(Actor *actor);
s32 effect_bounceOnGround(struct Obj150BCFB8 *obj, s32 arg1, s32 arg2, s32 arg3, f32 floorY);
void func_150BD070(u8 arg0, s32 arg1);

/* game/Effects/SegmentTrail/segmenttrail.c */
struct Obj150BDB3C;
void func_150BD740(Actor *actor, u8 arg1, s32 arg2);
s32 func_150BD954(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10,
                  s32 arg11, s32 arg12, f32 *dir, u8 arg14);
s32 effect_fadeSegmentTrailByLife(struct Obj150BDB3C *obj);
extern const Vec3f D_8009FF40[4];
extern const Vec3f D_8009FF70[4];
extern const Vec3f D_8009FFA0[4];

/* game/Effects/EmitterSpawner/emitterspawner.c */
void effect_newEmitterSpawner(Camera *owner, u8 arg1, s32 arg2);
void effect_updateEmitterSpawner(EB340Obj *obj);

/* game_EB600.c */
void effect_emitterSpawnerMessage(EB340Obj *obj, void **msg, u8 event);
s32 effect_updateRisingScreenSprite(EB340Rising *arg0);
void func_150BE210(WorldObject *obj);
void effect_updatePathMover(WorldObject *mover);
GameCommand *func_150BE438(GameCommand *cmd, s32 id);
void func_150BE494(Actor *obj, u8 arg1, s32 arg2);
extern const s32 D_800A0010[10];
extern const f32 D_800A0038[10];
extern const s32 D_800A0060[1];
extern const f32 D_800A0064[1];
extern const f32 D_800A0068[1];

/* game/Effects/BoneGroundFx/bonegroundfx.c */
s32 effect_getQuadrupedFootPos(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side);
s32 effect_selectQuadrupedFootEffect(s32 arg0, s32 arg1);
void effect_spawnQuadrupedFootprint(s32 actor, s32 side, s32 strength);
s32 effect_getQuadrupedFootPosAlt(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side);
s32 effect_selectQuadrupedFootEffectAlt(s32 arg0, s32 arg1);
void effect_spawnQuadrupedFootprintAlt(s32 actor, s32 side, s32 strength);
void effect_levelHook27(Camera *arg0);

/* game/Effects/BoneRing/bonering.c */
s32 effect_getStompFootPos(Vec3f *pos, Actor *actor, u8 side);
s32 effect_selectStompEffect(s32 arg0, s32 arg1);
void effect_spawnStompRing(Actor *actor, s32 side, s32 arg2);
s32 effect_emitStompPuff(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, f32 fromX, f32 fromY, f32 fromZ, s32 arg8, s32 arg9,
                  s32 argA, s32 argB, f32 size);

/* game/Effects/PulseEffect/pulseeffect.c */
typedef struct PulseEffect PulseEffect;
void effect_spawnPulseEffect(Actor *owner, s16 duration, u8 arg2, s32 arg3);
void effect_updatePulseEffect(PulseEffect *effect);
void effect_freePulseParticle(PulseEffect *effect);
void effect_destroyPulseEffect(PulseEffect *effect);
void effect_freePulseEffect(PulseEffect *effect);
void effect_pulseEffectMessage(PulseEffect *effect, s32 arg1, u8 event);
extern const Vec3f gPulseBonePathOffsets[3];
extern const u8 gPulseBonePathBones[4];
extern const Vec3f gPulseAttachPathOffsets[4];
extern const u8 gPulseAttachPathMatrices[4];

/* game/Effects/WatcherEffects/watchereffects.c */
typedef struct Watcher Watcher;
struct CeilingProbe;
void effect_keepWatcherFloorSprite(Watcher *watcher);
s32 effect_updateFlickerSprite(FlickerSprite *sprite);
s32 effect_expireFlickerSprite(FlickerSprite *sprite);
void effect_keepWatcherEmitter(Watcher *watcher);
WatcherEmitter *effect_attachRisingEmitter(Actor *character, Watcher *watcher);
void effect_spawnRisingParticle(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, WatcherEmitter *emitter);
s32 effect_isWatcherEmitterAlive(WatcherEmitter *emitter);
void effect_destroyWatcherEmitter(WatcherEmitter *emitter);
void effect_freeWatcherEmitter(WatcherEmitter *emitter);
FlickerSprite *effect_createFloorFlickerSprite(Watcher *watcher);
extern const struct CeilingProbe gWatcherCeilingProbe;

/* game/Effects/ParticleCallbacks/particlecallbacks.c */
typedef struct Obj150C7670 Obj150C7670;
s32 func_150C7670(Obj150C7670 *arg0);
s32 effect_setParticleIdentityMtx(void *arg0, s32 arg1);

/* game/Effects/BounceRecords/bouncerecords.c */
void effect_updateBounceRecords(void);
void effect_levelUpdateBounceRecords(s32 arg0);
void effect_randomizeBounceRecordSpeeds(s32 scale);
Gfx *effect_bindBounceRecordFrames(Gfx *gdl);

/* game/Effects/RingEffect/ringeffect.c */
typedef struct RadialEffect RadialEffect;
typedef struct ParticleObject ParticleObject;
typedef struct BurstParticle BurstParticle;
RadialEffect *effect_newRingEffect(RadialEffectDef *def, s32 extraSize, u8 arg2, s32 arg3);
void effect_updateRingEffect(RadialEffect *effect);
Gfx *effect_drawRingEffect(Gfx *gdl, RadialEffect *effect, s32 arg2);
void effect_ringEmitDarkParticle(RadialEffect *effect, Vec3f *pos, u8 heading, f32 cosine, f32 sine);
void effect_ringHitActors(RadialEffect *effect, f32 oldRadius);
void effect_ringEmitBurst(RadialEffect *effect, Vec3f *pos, u8 heading, f32 cosine, f32 sine);
s32 effect_moveBurstParticle(BurstParticle *particle);
void effect_spawnImpactRing(Actor *actor);
void effect_emitGroundCircle(Vec3f *pos);
extern Gfx gRingRenderSetupDl[];
extern Gfx gRingRenderResetDl[];
s32 effect_capQuadSpriteAlphaByLife(QuadSprite *arg0);
s32 effect_spawnDampedParticle(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 heading, s32 arg9,
                  s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14);
s32 effect_dampParticleVelocityXZ(ParticleObject *obj, s32 arg1);
s32 effect_spawnTumblingBurst(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 heading, s32 arg9,
                  s32 arg10, s32 arg11, s32 arg12, f32 *arg13, u8 arg14);
s32 effect_moveTumblingBurst(BurstParticle *particle);

/* game/Effects/RotatingZone/rotatingzone.c */
void effect_levelUpdateRotatingZone(s32 arg0);
f32 func_150CF040(s32 arg0, s32 arg1);
void effect_levelHook16(s32 *arg0);

/* game/Effects/ActorEmitterTick/actoremittertick.c */
typedef struct ActorEmitter ActorEmitter;
void effect_updateActorEmitter(ActorEmitter *emitter);
void effect_actorEmitterMessage(ActorEmitter *emitter, ActorMsg *msg, u8 type);

/* game/Effects/HopSprite/hopsprite.c */
typedef struct HopSprite HopSprite;
extern s32 gTile0ScrollS; /* texture scroll offsets (also advanced by game/Level/LevelHook30/levelhook30.c) */
extern s32 gTile0ScrollT;
extern s32 gTile1ScrollS;
extern s32 gTile1ScrollT;
void func_150CF530(s32 arg0);
void effect_scrollTileTextures(s32 arg0);
Gfx *effect_setScrolledTileSizes(Gfx *gfx);
void effect_spawnHopSprite(s16 unk12, u8 unkC, s32 unk1);
s32 effect_updateHopSprite(HopSprite *hopper);
void effect_hopSpriteMessage(HopSprite *hopper, f32 *targetX, u8 type);
void effect_sendHopMessage(f32 targetX);
s32 effect_updateBounceSprite(HopSprite *particle);

/* game/Effects/TextLines/textlines.c */
u8 *effect_findLineEnd(u8 *str);
u8 *effect_findStringEnd(u8 *str);
s32 effect_getLineLength(u8 *str, u8 **lineEnd);
s32 effect_getLongestLineLength(u8 *str);
void effect_copyTextLine(TextObj *obj);
void effect_nextTextLine(TextObj *obj);
TextObj *effect_newTextBox(u8 id, u8 *str, s16 duration, s32 styleSize, s8 updateFn, u8 msgFn, u8 arg6, s32 arg7);
s32 effect_drawTextBox(s32 arg0, TextObj *obj, s32 arg2);
void effect_textBoxMessage(TextObj *obj, u8 *payload, u8 type);
void effect_newFadeTextBox(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4);
s32 effect_updateFadeTextBox(TextObj *obj);
void effect_newPromptTextBox(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4);
s32 effect_updatePromptTextBox(TextObj *obj);
void effect_newSlowFadeTextBox(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4);
s32 effect_updateSlowFadeTextBox(TextObj *obj);

/* game/Effects/ZoneSounds/zonesounds.c */
void effect_playRandomZoneSounds(s32 arg0);
extern u8 gRandomSoundTimer;

/* game_FE850.c */
void effect_spinDownActor(Actor *arg0);

/* game_FE8C0.c */
/* A timed effect that pulses the ambient colour (effect_updateAmbientPulse, and light_updateAmbientRedDrift of game/Effects/AmbientPulse/ambientpulse.c). */
typedef struct {
    TimedEffect base;
    /* 0x28 */ f32 red;    /* the ambient red level, eased toward target */
    /* 0x2C */ f32 target;
    /* 0x30 */ f32 timer;  /* frames until target is re-rolled */
} AmbientPulse;
void func_150D1410(Actor *arg0);
void func_150D146C(Actor *arg0);
void effect_updateAmbientPulse(AmbientPulse *arg0);

/* game/Effects/RaySpinner/rayspinner.c */
typedef struct FF0E0Object FF0E0Object;
/* Payload of message 0x32 (passed as the s32 argument). */
typedef struct {
    Vec3f angularVelocity;
    f32 intensity;
} FF0E0SpinMsg;
extern const u8 gRaySpinnerChildColors[13][3];
extern const FF0E0SpinMsg gRaySpinnerSpinPresets[3];
void effect_updateRaySpinner(FF0E0Object *obj);
void effect_raySpinnerMessage(FF0E0Object *obj, s32 arg, u8 msg);
void effect_destroyRaySpinnerChildren(FF0E0Object *obj);
void effect_destroyRaySpinner(FF0E0Object *obj);
void effect_freeRaySpinner(FF0E0Object *obj);

/* game/Effects/HitSpray/hitspray.c */
void effect_spawnRayHitDebris(Vec3f *pos, u8 fxOwner, s32 fxList);
void effect_spawnRaySurfaceImpact(CollisionTri *surface, Vec3f *pos, Vec3f *dir, u8 fxOwner, s32 fxList);

/* game/Effects/TintedObject/tintedobject.c */
typedef struct TintedObject TintedObject;
void effect_handleTintMessageRedFrom1(TintedObject *obj, u8 *state, u8 msg);
void effect_handleTintMessageRedFrom2(TintedObject *obj, u8 *state, u8 msg);
void effect_handleTintMessageGreenOnly(TintedObject *obj, u8 *state, u8 msg);
void effect_advanceTexAnimClock(s32 arg0);
Gfx *effect_bindAnimatedTextures(Gfx *gdl);
void func_150D7790(Actor *obj, s32 action);
void func_150D7928(Actor *obj);

/* game/Effects/PuffTimer/pufftimer.c */
typedef struct PuffTimer PuffTimer;
void effect_startPuffJets(Actor *owner, s32 type, u8 sceneTag);
void effect_updatePuffTimer(PuffTimer *timer);

/* game/Effects/FallingStream/fallingstream.c */
typedef struct StreamActor StreamActor;
s32 effect_createFallingStream(u8 kind);
s32 effect_updateFallingStream(StreamActor *actor);
s32 effect_updateFallingStreamTrail(StreamActor *actor);
Gfx *effect_drawFallingStream(StreamActor *actor, Gfx *gfx, s16 view);

/* game/Effects/TrapEffect/trapeffect.c */
typedef struct TrapEffect TrapEffect;
typedef struct EffectSpawnSource EffectSpawnSource;
void effect_onTrapTriggered(WorldObject *trap);
s32 effect_startTrapStream(TrapEffect *obj);
s32 effect_spawnTrailSpriteLarge(EffectSpawnSource *source, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface);
s32 effect_spawnTrailSpriteSmall(EffectSpawnSource *source, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface);
s32 effect_fadeTrailSpriteByLife(QuadSprite *sprite);
void effect_shrinkBillboard(BillboardParticle *particle);

/* game/Effects/LevelHook14/levelhook14.c */
void effect_emptyLevelHook14(s32 arg0);

/* game/Effects/ZonePairEmitter/zonepairemitter.c */
typedef struct ZonePairEffect ZonePairEffect;
typedef struct TravelParticle TravelParticle;
typedef struct TexturePair TexturePair;
void func_150DE320(s32 arg0);
void effect_onBulletImpactZones(s32 arg0, s32 arg1, s32 arg2);
void func_150DE458(WorldObject *prop);
void func_150DE6D8(WorldObject *prop);
void effect_updateZonePairEmitter(ZonePairEffect *emitter);
s32 effect_moveTravelParticle(TravelParticle *particle);
Vtx *effect_buildTravelParticleQuad(TravelParticle *particle, s16 arg1);
void effect_sendFirstLightAndGlowEvents(s32 arg0);
void effect_sendLightAndGlowEvent(u8 index, u8 arg1);
extern const TexturePair gTravelParticleTextures;

/* game/Effects/RainHook/rainhook.c */
void effect_levelUpdateRain(s32 arg0);

/* game/Effects/Explosion/explosion.c */
typedef struct PointEffect PointEffect;
void effect_spawnExplosion(Vec3f *pos, u8 owner, s32 kind);
void effect_dropPointEffect(PointEffect *effect);
void effect_spawnScatteredParticles(Vec3f *pos, s32 arg1, s32 arg2, u8 owner, s32 kind);

/* game_10CD70.c */
typedef struct SwingObject SwingObject;
typedef struct BoltEmitter BoltEmitter;
extern u8 gWarpModelSetupDone;
extern u8 gMaterialGlowLevels[3];
s32 effect_isMaterialZoneActive(s32 index);
void effect_levelUpdateMaterialScroll(s32 arg0);
Gfx *effect_bindMaterialTextures(Gfx *gdl);
void effect_updateSwingObject(SwingObject *obj);
void func_150DFDA4(Actor *actor);
s32 func_150DFDD0(ModelAttachment *att, Actor *owner);
void effect_updateBoltEmitter(BoltEmitter *emitter);
void effect_boltEmitterMessage(BoltEmitter *emitter, s32 arg1, u8 arg2);
void effect_setupWarpModelOnce(void);
void effect_attachBloodDrip(Actor *actor, u8 arg1, s32 arg2);
void effect_spawnDripDrop(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);
void effect_fadeByCameraDistance(WorldObject *obj);
void effect_spawnSlotBoneBeam(Actor *actor, u8 slot, u8 arg2, s32 arg3);

/* game/Effects/SplashBurst/splashburst.c */
void func_150E0870(Actor *obj, u8 arg1, s32 arg2);
void func_150E0BE0(Actor *obj);

/* game/Effects/PropEmitter/propemitter.c */
typedef struct PropEmitterEffect PropEmitterEffect;
void effect_levelHook12(Camera *camera);
TimedEffect *effect_spawnPropEmitter(u8 index, u8 owner, s32 kind);
void effect_updatePropEmitter(PropEmitterEffect *effect);

/* game/Effects/HomingEffect/homingeffect.c */
typedef struct Obj4174 Obj4174;
typedef struct Obj4010 Obj4010;
typedef struct HomingEffect HomingEffect;
HomingEffect *effect_newHomingEffect(s32 x, s32 y, s32 z, s32 targetX, s32 targetY, s32 targetZ, s32 arg6, f32 speed,
                            Actor *targetObj, f32 originX, f32 originY, f32 originZ, f32 *follow, s16 lifetime);
s32 effect_startHomingToPoint(s32 x, s32 y, s32 z, s32 targetX, s32 targetY, s32 targetZ, s32 arg6, s32 frames);
s32 effect_startHomingOnActor(s32 x, s32 y, s32 z, Actor *targetObj, s32 arg4, f32 speed);
void effect_startFollowingHoming(f32 *origin, s32 *to, f32 *follow, s16 lifetime);
s32 effect_homingSoundCallback(s32 arg0, s32 arg1, void **arg2, s32 arg3);
s32 effect_startHomingWithSound(s32 originX, s32 originY, s32 originZ, s32 x, s32 y, s32 z, Actor *targetObj, s32 arg7, f32 speed);
void effect_destroyHomingEffect(HomingEffect *effect);
void effect_stopHomingEffect(s32 handle);
void effect_getHomingEffectPos(s32 handle, s32 *x, s32 *y, s32 *z);
void effect_updateHomingEffect(HomingEffect *arg0);
void func_150E4010(Obj4010 *obj);
void func_150E411C(struct Game1EF500Trail *trail);
s32 func_150E4174(Obj4174 *obj);
extern HomingEffect *gHomingEffects[8]; /* live homing effects by handle - 1 */

/* game/Effects/GroundHits/groundhits.c */
void effect_pickGroundHitCentre(void);
void effect_scatterGroundHits(s32 count);
void effect_scatterGroundHitsAhead(s32 amount);

/* game/Effects/MaterialImpact/materialimpact.c */
typedef struct Struct150E5810 Struct150E5810;
typedef struct Struct150E4CBC Struct150E4CBC;
void effect_spawnMaterialImpact(f32 x, f32 y, f32 z, s32 size, s32 material, s16 **tri, s32 slot);
void effect_spawnGroundImpactsUnder(WorldObject *obj);
void func_150E4CBC(Struct150E4CBC *fx);
void effect_sprayDebris(f32 x, f32 y, f32 z, s32 kind, s32 size, s16 **tri, s32 slot);
void effect_launchDebrisChunk(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, s32 kind, s32 size);
s32 effect_bounceOffTriangle(Struct150E5810 *obj, s32 arg1, s32 arg2, s32 arg3, f32 groundY, s16 (*verts)[3]);

/* game/Effects/DistantBolts/distantbolts.c */
void effect_updateDistantTracers(void);

/* game/Effects/Oscillator/oscillator.c */
typedef struct struct_EA11C_arg5 struct_EA11C_arg5;
typedef struct OscillatorObject OscillatorObject;
OscillatorObject *func_150E9FE0(Actor *owner, s16 arg1, u8 arg2, s32 arg3);
s32 effect_resetOscillator(OscillatorObject *obj);
void effect_spawnSegmentParticle(Vec3f *from, Vec3f *to, s32 arg2, s32 arg3, f32 t, struct_EA11C_arg5 *arg5);
f32 effect_stepOscillator(OscillatorObject *obj);
void *func_150EA500(Actor *owner, s16 lifetime, u8 arg2, s32 arg3);
void effect_spawnDriftParticle(f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz);
void effect_levelUpdateDistantTracers(s32 arg0);
void func_150EA904(Vec3f *pos, s32 index);
void func_150EA944(WorldObject *entry);

/* game/Effects/GroundFlash/groundflash.c */
typedef struct struct_EAB10 struct_EAB10;
void effect_spawnGroundFlashes(struct_EAB10 *actor);
s32 effect_updateGroundFlash(LightEffect *obj);

/* game/Effects/BoneGroundHit/bonegroundhit.c */
s32 effect_getAltFootPos(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side);
s32 effect_selectAltFootEffect(s32 kind, Actor *actor);
void effect_spawnFootprintAtAltFoot(s32 actor, s32 side, s32 strength);

/* game/Effects/SwayObject/swayobject.c */
typedef struct SwayObject SwayObject;
s32 effect_getLegBonePoint(Vec3f *out, Actor *owner, u8 slot);
s32 effect_selectLegImpactEffect(s32 arg0, s32 arg1);
void effect_spawnLegImpactBurst(Actor *owner, s32 slot, s32 arg2);
s32 effect_updateSway(SwayObject *obj);
s32 effect_updateModelSway(DynModel *model);
s32 effect_buildSwayMatrix(DynModel *model, s32 arg1);
s32 effect_placeSwayLight(LightEffect *effect);
void effect_levelHook26(Camera *actor);
void func_150F2994(Camera *actor, s32 flag);

/* game/Effects/SurfaceSound/surfacesound.c */
void effect_setZoneFlag(Actor *obj, TriggerZone *zone);
s32 effect_consumeZoneFlag(void);
void effect_updateSurfaceSound(s32 arg0);
extern u8 gZoneFlag; // set by effect_setZoneFlag, read and cleared by effect_consumeZoneFlag

/* game/Effects/BeamZones/beamzones.c */
void effect_turnBeamsOff(void);

/* game/Effects/ProjectilePuffs/projectilepuffs.c */
typedef struct TrailPuff TrailPuff;
typedef struct ProjectileParticle ProjectileParticle;
void effect_spawnProjectile(Vec3f *pos, Vec3f *aim, Vec3f *altOrigin, Vec3f *target, u8 targetIsDir, RayHit *knownHit, f32 speed, f32 arg7,
                   f32 arg8, Actor *owner, u8 arg10, u8 arg11, u8 arg12, u8 arg13, s32 arg14, s8 arg15, s32 sound,
                   u8 arg17, u8 arg18, s32 arg19);
s8 effect_updateProjectile(ProjectileParticle *obj);
s32 effect_updateTrailPuff(TrailPuff *obj);
void effect_buildTrailPuffQuad(TrailPuff *obj, s16 arg1);
void effect_spawnPuffFlash(Vec3f *pos, u8 arg1, s32 arg2);
void effect_spawnPuffFollower(f32 *target, s16 arg1, u8 arg2, s32 arg3);
s32 effect_updatePuffFollower(ProjectileParticle *obj);

/* game/Effects/StageEffect/stageeffect.c */
typedef struct StageObj StageObj;
typedef struct StageMsg StageMsg;
void effect_hitReactLightBurst(s32 arg0, Actor *obj, s32 arg2);
void effect_stageZoneHandler(Actor *obj, TriggerZone *zone);
void effect_stageMessage(StageObj *obj, StageMsg *msg, u8 type);
void effect_advanceStage(StageObj *obj);
void effect_jumpStagesToEnd(u8 index);
void effect_releaseStage(TimedEffect *arg0);
void effect_destroyStage(TimedEffect *arg0);
void effect_freeStage(TimedEffect *arg0);
void effect_spawnCutsceneBoneBeam(Actor *obj, u8 arg1, s32 arg2);
void effect_spawnGunMuzzleEffects(Actor *obj, u8 arg1, s32 arg2);
extern const u8 D_800A1C00[8][8];

/* game_127060.c */
typedef struct Obj150FADC8 Obj150FADC8;
typedef struct LineEmitter LineEmitter;
typedef struct FadeInSprite FadeInSprite;
typedef struct FadeInEffect FadeInEffect;
void effect_spawnDelayedBurst(Actor *actor, u8 owner, s32 kind);
void effect_updateDelayedBurst(BurstEffect *effect);
void effect_handleLightSwitchEvent(TimedEffect *effect, s32 arg1, u8 event);
void effect_spawnBonePointImpact(Actor *actor, u8 point, u8 owner, s32 kind);
void effect_startLineEmitter(u8 owner, s32 kind);
extern u8 gLineEmitterStarted; // set once effect_startLineEmitter's emitter runs; game/Game/LevelInit/levelinit clears it
void effect_updateLineEmitter(LineEmitter *effect);
void func_150FACE4(EffectHeader *obj, s32 arg1, u8 event);
void effect_turnLightsOn(void);
void effect_turnLightsOff(void);
void func_150FADC8(Obj150FADC8 *obj, s32 arg1, u8 event);
void effect_spawnScreenCash(s16 duration, u8 owner, s32 kind);
s32 effect_placeScreenCashModel(ModelEffect *fx);
Gfx *effect_preDrawScreenCashModel(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed);
void effect_fadeInAlpha(u8 *alpha, s16 timer, s16 duration, s16 fadeLength, s16 fadeRate);
s32 effect_updateFadeInEffect(FadeInEffect *effect);
s32 effect_updateFadeInSprite(FadeInSprite *sprite);
void func_150FB324(Actor *actor, u8 arg1, s32 arg2);

/* game/Effects/StarField/starfield.s */
Gfx *effect_drawStarField(Gfx *gfx, MtxF viewMtx);

/* game/Effects/ChildEffect/childeffect.c */
typedef struct Effect_129DE0 Effect_129DE0;
/* An effect that owns another object (child), destroyed with it (see effect_destroyChildEffect). */
struct Effect_129DE0 {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ u8 pad28[0x10];
    /* 0x38 */ EffectHeader *child;
};
s32 effect_filterExcludedTargets(Actor *obj, s32 arg1, s32 arg2);
void effect_destroyChildEffect(Effect_129DE0 *effect);
void effect_destroyWithChild(Effect_129DE0 *effect);

/* game/Effects/ChildEffectFree/childeffectfree.c */
void effect_freeWithChild(Effect_129DE0 *obj);

/* game/Effects/AreaSpray/areaspray.c */
typedef struct Actor150FD514 Actor150FD514;
extern TriggerZone *gSprayAreas[3];
void effect_eruptSprayAreas(void);
void effect_sprayFromArea(u8 index);
f32 effect_getSprayAreaWobbleScale(void *arg0);
void effect_levelUpdateSprayCooldowns(s32 arg0);
void effect_spawnActorBeam(Actor *obj, u8 arg1, s32 arg2);
s32 effect_updateActorBeam(Actor150FD514 *particle);
void effect_actorBeamMessage(Actor150FD514 *actor, ActorMsg *msg, u8 eventId);
Vtx *effect_buildActorBeamQuad(Actor150FD514 *actor, s16 viewport);
void effect_releaseActorBeam(Actor150FD514 *actor);
void effect_destroyActorBeam(Actor150FD514 *actor);
void effect_freeActorBeam(Actor150FD514 *actor);

/* game/Effects/TreadScroll/treadscroll.c */
void effect_levelUpdateCutsceneTreads(s32 arg0);

/* game/Effects/BonePointFx/bonepointfx.c */
extern u8 D_80088B90;
extern const Vec3f D_800A1FB0;
extern const Vec3f D_800A1FBC;
struct Game1EF500Trail *func_150FE320(Actor *obj, u8 arg1, s32 arg2);
void func_150FE49C(Actor *obj, u8 arg1, s32 arg2, Vec3f *outPosition, Vec3f *outOrigin);
void func_150FE604(Actor *obj, Vec3f *position, u8 arg2, s32 arg3);
void effect_getBonePointPos(Actor *obj, s16 arg1, Vec3f *out);
void func_150FE7D4(Actor *obj);

/* game/Effects/OwnerEffect/ownereffect.c */
void effect_sendOwnerGone(Actor *owner);
void effect_spawnOwnerEffect(Actor *owner);
void effect_ownerEffectMessage(OwnerEffect *effect, ActorRef *msg, u8 msgId);
void effect_updateOwnerEffect(OwnerEffect *effect);

/* game/Effects/LevelHook08/levelhook08.c */
void effect_emptyLevelHook8(s32 arg0);

/* game/Effects/FlagLights/flaglights.c */
void effect_levelUpdateFlagLights(s32 arg0);
Gfx *effect_bindFlagLightTextures(Gfx *gdl);

/* game/Effects/BoneBeamParticle/bonebeamparticle.c */
typedef struct BoneBeamParticle BoneBeamParticle;
QuadSprite *effect_spawnSurfaceSprite(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6,
                          s32 surfaceFlags, u8 arg8, s32 arg9);
BoneBeamParticle *effect_spawnBoneBeamParticle(Actor *target, u8 bone, const Vec3f *start, const Vec3f *end, ParticleSize *size,
                                s16 lifetime, u8 alpha, f32 flashSize, Vec3f *flashPos, u8 viewMask, s32 extraSize, s16 node,
                                u8 arg12, s32 arg13);
void effect_boneBeamMessage(BoneBeamParticle *particle, s32 arg1, u8 arg2);
s32 effect_updateBoneBeamParticle(BoneBeamParticle *particle);
Vtx *effect_buildBoneBeamQuad(BoneBeamParticle *particle, s16 viewport);
void effect_spawnFlashParticle(s16 lifetime, u8 alpha, f32 size, Vec3f *pos, u8 views, u8 arg5, s32 arg6);

/* game/Effects/DebrisThrow/debristhrow.c */
void effect_levelUpdateSoundCooldowns(s32 arg0);
void effect_throwDebrisBurst(Vec3f *origin, u8 arg1, s32 arg2);
struct SoundRecord;
/* A sound record callback (SoundRecordCallback, conker/sound.h) that never stops its record. */
s32 effect_nullSoundCallback(struct SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);

/* game/Effects/ImpactSplash/impactsplash.c */
void effect_spawnImpactBlast(Actor *owner, Actor *obj, Vec3f *pos, u8 variant, s32 arg4, u8 arg5, s32 arg6);

/* game/Effects/DebrisEffect/debriseffect.c */
void func_15108120(Actor *obj, u8 arg1, s32 arg2);

/* game/Effects/DynModel/dynmodel.c */
extern Gfx gDynModelRenderSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gDynModelRenderResetDl[]; /* render teardown (object type table gObjectTypes) */
DynModel *effect_newDynModel(DynModelDesc *desc, u8 arg1, s32 userDataSize, u8 arg3, s32 arg4);
void effect_updateDynModel(DynModel *obj);
Gfx *effect_drawDynModel(Gfx *gfx, DynModel *obj, s16 viewport);
s32 effect_setDynModelIdentityMtx(DynModel *obj, s16 viewport);
void effect_defaultDestroyDynModel(DynModel *obj);
void effect_defaultFreeDynModel(DynModel *obj);
void effect_destroyDynModel(DynModel *obj);
void effect_freeDynModel(DynModel *obj);
void effect_dynModelMessage(DynModel *obj, void *msg, u8 msgType);
s32 effect_skipDynModelDraw(DynModel *obj, s16 viewport);
DynModel *effect_spawnWarpModel(void);
s32 effect_warpModelTexCoords(DynModel *obj, s16 viewport);
s32 effect_updateWarpModel(DynModel *obj);

/* game/Effects/ActorEffects/actoreffects.c */
typedef struct Msg151419D0 Msg151419D0;
typedef struct LinkedObject151419D0 LinkedObject151419D0;
typedef struct Args151420F8 Args151420F8;
void effect_detachTimedHandler(LinkedObject151419D0 *obj);
void effect_destroyTimedHandler(void *arg0);
void effect_freeTimedHandler(void *arg0);
void effect_timedHandlerMessage(LinkedObject151419D0 *obj, Msg151419D0 *msg, u8 msgType);
void effect_runActorEffects(Actor *actor, s32 arg1);
s32 effect_getActorEffectSelector(Actor *actor);
s32 effect_getSurfaceEffectVariant(u32 type);
void effect_tryAttachActorEffect(void *actor, s32 selector, s32 handler);
void effect_attachActorEffect(Actor *actor, s32 handler);
void effect_spawnFootprint(u8 kind, CollisionHit *src, f32 scale, u8 arg3, Vec3f *pos, u8 arg5);
s32 func_151420F8(Actor *actor);
void effect_spawnSplashSpray(u8 kind, Vec3f *pos, s32 arg2, f32 scaleA, f32 scaleB);
extern void (*gProbeVisibleCallbacks[])(void *probe, s32 r, s32 g, s32 b); /* probe visible callbacks of game/Flare/VisProbe/visprobe.c */
extern ActorEffectDef gActorEffects[20];
extern const Args151420F8 D_800A5200;

/* game/Math/MathUtil/mathutil.c */
s32 math_debugTunableRange(s32 id, const char *name, s32 min, s32 max, const char *file, s32 line);
s32 math_debugTunable(s32 id, const char *name, s32 min, s32 max, s32 value, const char *file, s32 line);
s32 func_151422F8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
/* (void *bones, s32 bone, Vec3f *out); declared without a prototype: game/Effects/ActorTasks/actortasks.c also passes the actor */
void math_getBoneTranslation();
f32 math_cosBinAngleMasked(u8 angle);
Actor *math_findActorById(u8 uniqueId, Actor *self);
void math_mtxRotScaleAxes(Mtx *out, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 sx, f32 sy, f32 sz, f32 x,
                   f32 y, f32 z);
void math_mtxLookAtScale(Mtx *out, f32 scaleXZ, f32 scaleY, f32 sx, f32 sy, f32 sz, f32 x, f32 y, f32 z, f32 atX, f32 atY,
                   f32 atZ);
void math_mtxRotScaleXZ(Mtx *out, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z);
void math_mtxFRotScaleXZ(MtxF mf, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z);
void math_pickPaletteColour(u8 palette, u8 *r, u8 *g, u8 *b);
s32 func_15142A5C(Actor *actor);
f32 math_lagrangeWeight0(f32 t);
f32 math_lagrangeWeight1(f32 t);
f32 math_lagrangeWeight2(f32 t);
f32 math_lagrangeWeight3(f32 t);
Gfx *math_gfxUpdateGeometryMode(Gfx *gfx, s32 setBits, s32 clearBits);
Gfx *math_gfxSetEnvColor(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *math_gfxSetPrimColor(Gfx *gfx, s32 minLevel, s32 lodFrac, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *math_gfxLoadTextureFrame(Gfx *gfx, TextureDesc *desc, s32 frame, s32 cm, s32 s, s32 t, s32 texture, u8 mode,
                   SpriteParams *sprite, u8 *needSync, s32 assetArg);
Gfx *math_gfxSetOtherMode(Gfx *gfx, s32 modeH, s32 modeL, u8 *needSync);
s16 func_15143044(u8 arg0, s32 arg1);
s32 math_getTextureFrameId(TextureDesc *desc, s32 texture, s32 frame, u8 mode);
void math_transformOffset(const Vec3f *offset, Vec3f *out, void *mtx);
void math_randomZoneFootprintPoint(TriggerZone *region, f32 *x, f32 *z, f32 *top, f32 *bottom);
void math_dirFromAngles(f32 yaw, f32 pitch, f32 length, Vec3f *out);
void math_dirFromAngleXZ(f32 angle, f32 length, f32 *x, f32 *z);
void math_dirFromBinAngles(s16 yaw, s16 pitch, f32 length, Vec3f *out); /* (yaw, pitch, length) -> vector */
void func_15143834(s16 yaw, s16 pitch, f32 length, Vec3f *out);
void math_dirFromBinAngleXZ(s16 angle, f32 length, f32 *x, f32 *z);
TriggerZone *math_findMatchingZone(s32 start, s32 end, u16 fields, TriggerZone *key);
void math_clampRange(s32 *start, s32 *end, s32 lo, s32 hi);
s32 math_clampInt(s32 *value, s32 lo, s32 hi);
s32 math_getActorFacingBin(Actor *actor);
s32 math_getActorRelFacingBin(Actor *actor);
f32 math_vec3Length(const Vec3f *v);
u8 math_shakeIfPlayerInZone(s32 arg0, s32 arg1);
s32 math_advanceFrameSeq(u8 id, u32 *speed, s32 *pos, u8 flags);
void math_pickPrimColour(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, s32 a0, u8 r1, u8 g1, u8 b1, u8 a1,
                   u8 weight, u8 mode);
void math_pickEnvColour(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, u8 a1,
                   u8 weight, u8 mode);
s32 math_wrapInt(s32 value, s32 hi, s32 lo);
f32 math_wrapFloat(f32 value, f32 hi, f32 lo);
f32 math_getZoneArea(TriggerZone *region);
f32 math_getZoneVolume(TriggerZone *region);
void math_randomPointInZone(TriggerZone *region, Vec3f *out);
f32 math_vec3Dot(Vec3f *a, Vec3f *b); /* dot product */
/* (s32 index); declared without a prototype: game/Weapon/Bullet/bullet.c passes a second argument */
f32 math_getCameraYaw();
Vec3f *math_getCameraPos(s32 index);
f32 math_wrapRadians(f32 angle);
f32 math_wrapDegrees(f32 angle); /* wraps an angle into [0, 360] degrees */
s32 math_wrapByte(s16 value);
f32 math_angleDistance(f32 a, f32 b);
/* (f32 *pos, f32 *screen, f32 *outA, f32 *outDepth, f32 *outInvDepth, u8 viewport); declared without a prototype:
 * flare_buildGlowQuad passes its s16 view without narrowing it */
s32 math_projectToScreen();
s32 math_getTriEdges(CollisionTri *tri, Vec3f *binormal, Vec3f *edgeOut, Vec3f *normal); /* triangle edges + normal */
void math_vec3Cross(Vec3f *a, Vec3f *b, Vec3f *out);
s32 math_vec3Normalize(Vec3f *v, Vec3f *out, f32 *length, f32 *invLength); /* normalize */
s32 math_raySphereWithin(Vec3f *origin, Vec3f *dir, Vec3f *center, f32 radius, f32 maxDist, Vec3f *hitNear, Vec3f *hitFar,
                  f32 *tNear, f32 *tFar);
s32 math_raySphere(Vec3f *origin, Vec3f *dir, Vec3f *center, f32 radius, Vec3f *hitNear, Vec3f *hitFar, f32 *tNear,
                  f32 *tFar);
s32 math_isNearCamera(u8 index, f32 radius, Vec3f *point);
void math_closestPointOnSegment(Vec3f *start, Vec3f *dir, Vec3f *point, Vec3f *out, f32 *t);
s32 math_closestPointOnLine(Vec3f *start, Vec3f *dir, Vec3f *point, Vec3f *out, f32 *t);
void math_getActorAim(Actor *actor, Vec3f *target, Vec3f *dir, Vec3f *offsetDir, f32 pitchOffset);
void math_getHeadingElevation(Vec3f *v, f32 *heading, f32 *elevation);
f32 math_circleHalfChord(f32 arg0, f32 scale, f32 arg2);
void func_15145A50(Actor *actor);
s32 math_rayActorEllipsoid(Vec3f *start, Vec3f *dir, Actor *actor, Vec3f *hitNear, Vec3f *hitFar, f32 *radius, f32 *arg6,
                  Vec3f *center);
u8 math_isObjectHidden(s32 index);
void math_transformObjectPointRefs(WorldObject *placement, const f32 **in, f32 **out, s32 count);
void math_transformObjectPoints(WorldObject *placement, Vec3f *in, Vec3f *out, s32 count);
void math_transformOffsets(const Vec3f **offsets, Vec3f **results, void *mtx, s32 count);
s32 math_buildPerpendicularBasis(Vec3f *n, Vec3f *u, Vec3f *v); /* two axes perpendicular to a direction */
void *math_emitPointLights(void *gfx, ModelLink *link, u8 mode, Actor *actor, u8 uniqueId, s16 slot, Vec3f *pos, u8 flags,
                    s32 arg8);
u8 math_isZoneHiddenFromViews(ZoneNode *zone);
void math_broadcastActorSwap(Actor *a, Actor *b);
s32 math_transformAttachmentOffsets(Actor *actor, ModelAttachment *point, s32 index, const Vec3f **offsets, Vec3f **results,
                  s32 count);
s32 math_isInWorldBounds(Vec3f *pos);
void math_randomDrift(f32 *timer, f32 maxTime, f32 *target, f32 lo, f32 mid, f32 hi, f32 rate, f32 *value);
extern const Vec3f gZeroVec3f; /* zero vector */
extern const f32 gUnitCircleTable[102];

/* game/Effects/ActorDamageFx/actordamagefx.c */
s32 effect_getFootPos(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side);
s32 effect_selectPlayerFootEffect(s32 kind, Actor *actor);
void effect_spawnFootPieceSpray(s32 actor, s32 side, s32 strength);
void func_15146BB8(s32 actor, s32 side, s32 strength);
void func_15146BD8(s32 actor, s32 side, s32 strength);
void effect_spawnFootprintAndSplash(s32 actor, s32 side, s32 strength, u8 alt);
void effect_spawnFootprintAtFoot(s32 actor, s32 side, s32 strength);
void effect_spawnFootSplash(s32 actor, s32 side, s32 strength);
void effect_spawnFootCircleBurst(s32 actor, s32 side, s32 strength);
s32 func_151470FC(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
void func_15147318(s32 actor, s32 side, s32 strength);
void func_15147478(s32 actor, s32 side, s32 strength);
void func_151475D8(s32 actor, s32 side, s32 strength);

/* game/Effects/EffectObj/effectobj.c */
extern Gfx gEffectObjRenderSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gEffectObjRenderResetDl[]; /* render teardown (object type table gObjectTypes) */
void effect_updateEffectObj(EffectObj *obj);
void effect_releaseEffectObjBuffers(EffectObj *obj);
/* effect_defaultDestroyEffectObj and effect_defaultFreeEffectObj take any view of a effect_newEffectObj object. */
void effect_defaultDestroyEffectObj(void *obj);
void effect_defaultFreeEffectObj(void *obj);
void effect_freeEffectObjLights(EffectObj *obj);
void effect_destroyEffectObj(EffectObj *obj);
void effect_freeEffectObj(EffectObj *obj);
/* Returns the new object; each caller keeps it in its own view (TrackerTrail, MotionTrail, ...). */
void *effect_newEffectObj(EffectTemplate *header, s32 dataSize, s32 itemSize, s32 updateKind, s32 updateKind2,
                    s32 drawKind, s32 arg6, s32 arg7, CollisionHit *hit, u8 arg9, s32 argA);
void *effect_drawEffectObj(void *gfx, EffectObj *obj, s16 arg2);
void effect_handleEffectObjMessage(EffectObj *obj, s32 arg1, u8 arg2);
void effect_sendEventToEffectObjs(s32 arg0, u8 arg1);
extern const s32 gEffectObjTypes[2]; /* the two effect object types */

/* game/Effects/TimedEffect/timedeffect.c */
extern Gfx gTimedEffectRenderSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gTimedEffectRenderResetDl[]; /* render teardown (object type table gObjectTypes) */
/* Both return a TimedEffect followed by dataSize bytes of its kind's data (the kind's own type). */
void *effect_newTimedEffect(s16 timer, s8 expireCallback, s8 tickCallback, s8 drawCallback, u8 flags, u8 callbackSet,
                    s32 dataSize, u8 owner, s32 list);
void *effect_newTimedEffectNoDraw(s16 timer, s8 expireCallback, s8 tickCallback, u8 flags, u8 callbackSet, s32 dataSize,
                    u8 owner, s32 list);
void effect_updateTimedEffect(TimedEffect *object);
void effect_releaseTimedEffectBuffers(TimedEffect *object);
void effect_defaultDestroyTimedEffect(TimedEffect *object);
void effect_defaultFreeTimedEffect(TimedEffect *object);
void effect_destroyTimedEffect(TimedEffect *object);
void effect_freeTimedEffect(TimedEffect *object);
void effect_handleTimedEffectMessage(TimedEffect *object, s32 arg1, u8 arg2);
Gfx *effect_drawTimedEffect(Gfx *gfx, TimedEffect *object, s16 view);
void effect_sendEventToTimedEffects(s32 arg0, u8 arg1);
void effect_handleTimedEffectOwnerEvent(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj);
extern const s32 gTimedEffectTypes[2]; /* the two timed effect object types */

/* game/Effects/ActorAttachments/actorattachments.c */
typedef struct ToggleTarget ToggleTarget;
s32 effect_findAttachSpawnSlots(Actor *obj, s32 *uniqueSlot, s32 *globalSlot, s32 *idSlot);
s32 effect_spawnAttachments(Actor *obj);
s32 effect_freeAttachments(Actor *owner);
s32 effect_sendAttachmentMessage(Actor *owner, s16 type, s32 msg, s32 arg);
s32 effect_isAttachTypeInTree(s16 root, s16 type);
void func_1514D564(Actor *obj);
void func_1514D64C(Actor *obj);
void func_1514D96C(Actor *obj);
void func_1514D978(Actor *obj);
void effect_attachWatcherAndRopeRider(Actor *obj);
void func_1514DA38(Actor *obj);
void func_1514DAA4(Actor *obj);
void effect_attachCharacterWatcher(Actor *obj);
void func_1514DB58(Actor *obj);
void effect_spawnRopeTieInDinoArena(Actor *obj);
void func_1514DB98(Actor *obj);
void effect_attachBone2Light(Actor *obj);
void func_1514DC38(Actor *obj);
void func_1514DC5C(Actor *obj);
void func_1514DC84(Actor *obj);
void func_1514DC98(Actor *obj);
void func_1514DCAC(Actor *obj);
void func_1514DCF4(Actor *obj);
void func_1514DD2C(Actor *obj);
void func_1514DD4C(Actor *obj);
void func_1514DD6C(Actor *obj);
void func_1514DD8C(Actor *obj);
void func_1514DDAC(Actor *obj);
void func_1514DDCC(Actor *obj);
void func_1514DDEC(Actor *obj);
void func_1514DE0C(Actor *obj);
void func_1514DE2C(Actor *obj);
void func_1514DE38(Actor *obj);
void func_1514DE44(Actor *obj);
void func_1514DE50(Actor *obj);
void func_1514DE88(Actor *obj);
void func_1514DE94(Actor *obj);
void effect_linkToObjectId9(Actor *obj);
void func_1514E00C(Actor *obj);
void func_1514E194(Actor *obj);
void func_1514E31C(Actor *obj);
void effect_attachCharacterEffect0(Actor *obj);
void effect_attachCharacterEffectMinus1(Actor *obj);
void effect_attachCharacterEffect1(Actor *obj);
void effect_attachCharacterEffect2(Actor *obj);
void effect_attachCharacterEffect3(Actor *obj);
void func_1514E824(Actor *obj);
void effect_releaseAttachedObject(EffectHeader *handle);
void effect_releaseAttachedEmitter(void *handle);
void effect_releaseAttachedPointLight(void *handle);
s32 effect_handleToggleMessage(ToggleTarget *target, s32 msg, s32 arg);
s32 effect_handleGlowToggleMessage(s32 handle, s32 msg, s32 arg);
s32 effect_forwardGlowToggleMessage(s32 handle, s32 msg, s32 arg);
s32 effect_forwardToggleMessage(ToggleTarget *target, s32 msg, s32 arg);
s32 effect_handleEmitterTableMessage(ActorEmitterProcess *target, s32 msg, s32 arg);
s32 effect_handleRopeRiderMessage(ToggleTarget *target, s32 msg, s32 arg);
s32 effect_acceptAnyMessage(s32 handle, s32 msg, s32 arg);
Attachment *effect_newAttachment(void *handle, s16 type, s32 extraSize);
/* Actor attachment lists (Actor.attachments): `handle` is the attached effect or object. */
Attachment *effect_attach(void *handle, Actor *owner, s16 type);
s32 effect_findAttachmentByType(Attachment *list, s16 type, Attachment **out);
s32 effect_findAttachmentByHandle(Attachment *list, void *handle, Attachment **out);
void *effect_removeAttachment(Attachment *node, Actor *owner);
void effect_detachHandle(void *handle, Actor *owner);
void effect_attachActorTracker(Actor *obj);
void func_1514EECC(Actor *obj);
void func_1514F110(Actor *obj);
s32 effect_handleLightMessage(LightEffect *target, s32 msg, s32 arg);
void effect_attachTankEffects(Actor *obj);
void effect_attachBone5LightBeam(Actor *obj);
void func_1514F3CC(Actor *obj);
void effect_attachFrontendFireImpEffect(Actor *obj);
void effect_attachTrailToBone1(Actor *obj);
void effect_attachTrailToBone2(Actor *obj);
void effect_attachTrailToBone3(Actor *obj);
void effect_attachTrailToBone4(Actor *obj);
void effect_attachPulseEffect(Actor *obj);
void func_1514F590(Actor *obj);
void effect_spawnActorPoolEmitter(Actor *obj);

/* game/Effects/AttachmentTables/attachmenttables.c (data only, linked after game/Effects/ActorAttachments/actorattachments) */
extern const u8 gAttachSpawnSlotsByLevel[];
extern const u8 gAttachSpawnSlotsById[];
extern const u8 *gAttachSpawnSlotLists[];
extern const AttachmentType *gAttachmentTypes[];

/* game/Effects/PlayerMarker/playermarker.c */
extern Gfx gPlayerMarkerRenderTeardownDl[]; /* render teardown (object type table gObjectTypes) */
/*
 * Type 0x50 object: a marker over player movement_obj[playerId], made of a white point light and a sprite
 * (attachment model 166, gAttachModelHeaders[166]) that bob 160 units above the player.
 */
typedef struct PlayerMarker {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 playerId;
    /* 0x11 */ u8 state;
    /* 0x12 */ u8 bobPhase;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ PointLight *light;
    /* 0x18 */ Mtx mtx[2];
    /* 0x98 */ f32 heightOffset;
} PlayerMarker;

PlayerMarker *effect_newPlayerMarker(s32 playerId, s32 owner);
void effect_showPlayerMarker(s32 playerId, s32 duration, f32 heightOffset);
void effect_updatePlayerMarker(PlayerMarker *marker);
Gfx *effect_drawPlayerMarker(Gfx *gfx, PlayerMarker *marker, s16 view);
void effect_freePlayerMarker(PlayerMarker *arg0);
void effect_rearmPlayerMarker(void);
void effect_keepPlayerMarkerShown(void);
PlayerMarker *effect_findPlayerMarker(s32 playerId);
void effect_playerMarkerMessage(PlayerMarker *marker, Actor **actors, u8 eventType);

/* game/Effects/ModelEffect/modeleffect.c */
extern Mtx gViewOrthoMtxs[]; /* per-view orthographic projection (view_buildOrthoMatrices builds them) */
extern const s32 gModelEffectTypes[3]; /* the model effect types */
extern Gfx gModelEffectRenderSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gModelEffectRenderTeardownDl[]; /* render teardown (object type table gObjectTypes) */
extern s32 (*gModelEffectUpdateFuncs[])();
extern s32 (*gModelEffectUpdate2Funcs[])(ModelEffect *);
extern s32 (*gModelEffectUpdate2FuncsTail[])();
ModelEffect *effect_newModelEffect(EffectParams *params, s32 arg1, f32 arg2, s32 attachArg, s32 arg4, s32 extraSize, u8 owner,
                           s32 list);
void effect_freeModelEffectLights(ModelEffect *fx);
void effect_destroyModelEffect(ModelEffect *fx);
void effect_freeModelEffect(ModelEffect *fx);
void effect_updateModelEffect(ModelEffect *fx);
Gfx *effect_drawModelEffect(Gfx *gfx, ModelEffect *fx, s16 view);
s32 effect_resetModelEffectRoot(ModelEffect *fx);
ModelEffect *effect_newMovingModelEffect(EffectParams *params, EffectMotion *motion, s32 arg2, f32 arg3, s32 attachArg, s32 arg5,
                           s32 extraSize, u8 owner, s32 list);
s32 effect_setModelEffectRootRotScale(ModelEffect *fx);
s32 effect_moveModelEffect(ModelEffect *fx);
void effect_modelEffectMessage(ModelEffect *fx, s32 event, u8 eventType);
s32 effect_setModelEffectRootFromExtra(ModelEffect *fx);
void effect_setModelEffectRootXform(ModelEffect *fx, f32 *xform);
Gfx *effect_preDrawModelEffectOrtho(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed);
Gfx *effect_postDrawModelEffectProjection(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync);
void effect_sendEventToModelEffects(s32 event, u8 eventType);

/* game/Effects/EffectActor/effectactor.c */
extern Gfx gEffectActorRenderSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gEffectActorRenderTeardownDl[]; /* render teardown (object type table gObjectTypes) */
EffectActor *effect_newEffectActor(EffectActorTemplate *template, s32 attachment, s32 arg2, u8 large, s32 extraSize, u8 arg5,
                           s32 arg6);
EffectActor *effect_newUnattachedEffectActor(EffectActorTemplate *template, u8 large, s32 extraSize, u8 arg3, s32 arg4);
void effect_updateEffectActor(EffectActor *actor);
Gfx *effect_drawEffectActor(Gfx *gfx, EffectActor *actor, s16 arg2);
s32 effect_buildEffectActorRotScaleMtx(Mtx *mtx, EffectActor *actor);
s32 effect_moveEffectActor(EffectActor *actor);
s32 effect_buildEffectActorScaleMtx(Mtx *mtx, EffectActor *actor);
void effect_freeEffectActorLights(EffectActor *actor);
void effect_destroyEffectActor(EffectActor *actor);
void effect_freeEffectActor(EffectActor *actor);
s32 effect_fadeEffectActor(EffectActor *actor);
void effect_effectActorMessage(EffectActor *actor, ActorMsg *msg, u8 msgId);

/* game/Effects/CharacterWatcher/characterwatcher.c */
typedef struct StateRule StateRule;
typedef struct SpawnerTemplate SpawnerTemplate;
typedef struct SpawnerSource SpawnerSource;
typedef struct Spawner Spawner;
typedef struct RiseEmitterObject RiseEmitterObject;
typedef struct ParticleUpdateArg ParticleUpdateArg;
typedef struct EffectMotionParticle EffectMotionParticle;
typedef struct DiskEmitterObject DiskEmitterObject;
extern Gfx *gEffectActorDisplayLists[]; /* effect actor display lists (EffectActorTemplate.displayList) */
CharacterWatcher *effect_newCharacterWatcher(Actor *chr, u8 enableActions, s32 extraSize);
void effect_destroyCharacterWatcher(CharacterWatcher *watcher);
void effect_freeCharacterWatcher(CharacterWatcher *watcher);
void effect_updateCharacterWatcher(CharacterWatcher *watcher);
void effect_characterWatcherMessage(void *self, ActorMsg *msg, u8 msgId);
s32 effect_watcherIsOutOfWater(Actor *chr, u8 mode);
s32 effect_watcherIsDeepWater(Actor *chr, u8 mode);
s32 effect_watcherIsNearWaterSurface(Actor *chr, u8 mode);
s32 effect_getWatcherMoveState(Actor *chr, Vec3f *lastPosition, u8 prevState);
s32 func_151592B8(Actor *chr, u8 mode);
s32 effect_pickWatcherMode(Actor *chr, u8 *mode);
u8 effect_matchWatcherState(Actor *chr, s16 state0, s16 state1, s16 state2, s16 moveState, s16 surfaceState);
u8 effect_updateRiseParticleToCeiling(EffectMotionParticle *particle, ParticleUpdateArg *arg1);
s32 effect_updateRiseParticle(EffectMotionParticle *particle, ParticleUpdateArg *arg1);
EffectMotionParticle *effect_spawnRiseParticle(Vec3f *pos, Vec3f *velocity, s32 arg2, s32 arg3);
void effect_emitRiseParticle(f32 x, f32 y, f32 z, s32 arg3, s32 arg4, s32 arg5, EffectHeader *obj);
void effect_updateDiskEmitter(DiskEmitterObject *obj);
s32 effect_updateOscillateParticle(EffectMotionParticle *particle, ParticleUpdateArg *arg1);
EffectMotionParticle *effect_spawnDriftMotionParticle(Vec3f *pos, Vec3f *velocity, f32 arg2, f32 damping, CollisionHit *ground, f32 size,
                                    s16 lifetime, u8 alpha, u8 arg8, u8 arg9, s32 argA);
s32 effect_updateDriftParticle(EffectMotionParticle *particle, ParticleUpdateArg *arg1);
u8 effect_updateLandingDriftParticle(EffectMotionParticle *particle, ParticleUpdateArg *arg1);
s32 effect_getWatcherFloorHit(Actor *chr, CollisionHit **out);
void effect_spawnRiseZoneEmitter(SpawnerSource *volatile src, s32 arg1);
void effect_spawnRiseEmitter(Spawner *spawner, Vec3f *pos, f32 ceiling, s32 arg3);
void effect_updateRiseEmitter(RiseEmitterObject *obj);

/* game/Effects/VertexSwing/vertexswing.c */
void effect_swingVertices(f32 *angle, Vtx *verts, f32 radius, f32 *velocity, f32 maxAngle, f32 minAngle, const u8 *vtxIndices,
                   u8 count);

/* game_193420.c */
void effect_emptyLevelUpdateHook(void);

/* game/Effects/QuadEffect/quadeffect.c */
extern Gfx gQuadEffectRenderSetupDl[]; /* render setup (object type table gObjectTypes) */
void effect_spawnQuadEffect(s32 direction, s32 x, s32 y, s32 z, s32 growthRate, s32 life, s32 arg6, u8 arg7, s32 arg8);
void effect_spawnRotatedQuadEffect(s32 angleA, s32 angleB, s32 x, s32 y, s32 z, s32 growthRate, s32 life, s32 arg7, u8 arg8, s32 arg9);
void effect_updateQuadEffect(QuadEffect *effect);
Gfx *effect_drawQuadEffect(Gfx *gdl, QuadEffect *effect, s32 arg2);
Gfx *effect_beginQuadEffectDraw(Gfx *gdl, s32 arg1, s32 arg2, s32 arg3);

/* game/Effects/ObjectList/objectlist.c */
extern EffectHeader *gObjectLists[2][OBJ_TYPE_COUNT]; /* object lists: [list][type] heads a doubly linked list */
extern s8 gObjectWalkDepth; /* depth of the object-list walks in progress */
extern void *gObjectWalkNext[6]; // per object-list walk depth (gObjectWalkDepth): the next node to visit
extern s32 gCachedTextureId; // texture last loaded by particle_loadDebrisTexture
extern s32 gCachedTexture;
extern s32 gCachedTextureFrame;
extern s16 gCachedRenderModeKey;
extern s16 gCachedCombineKey;
extern s16 gCachedPrimR;
extern s16 gCachedPrimG;
extern s16 gCachedPrimB;
extern s16 gCachedPrimA;
extern s16 gCachedEnvR;
extern s16 gCachedEnvG;
extern s16 gCachedEnvB;
extern s16 gCachedEnvA;
extern f32 gMainCameraAngleSinCos[2];
extern f32 gCameraAngleSines[4];
extern f32 gCameraAngleCosines[4];
extern s32 gCachedGeomSetBits;
extern s32 gCachedGeomClearBits;
extern s16 gCachedPrimMinLevel;
extern s16 gCachedPrimLodFrac;
extern s32 gCachedTextureCm;
extern s32 gCachedTextureS;
extern s32 gCachedTextureT;
extern SpriteParams *gCachedTextureSprite; // compared by math_gfxLoadTextureFrame, but never set to its sprite
extern s32 gCachedOtherModeH;
extern s32 gCachedOtherModeL;
extern s32 gDrawHookTextureFrame;
extern s32 gDrawHookTextureCm;
extern TextureDesc *gDrawHookTexture; // texture of the current draw hook (effect_beginQuadEffectDraw, particle_beginPuffQuadDraw)
extern SpriteParams D_800DD230;
typedef struct SpriteFx SpriteFx;
typedef struct ModelFxData ModelFxData;
/*
 * Data of a type 0x10 model effect (ModelFx in game/Effects/ObjectList/objectlist.c); effect_spawnModelFx copies a template of it into
 * the object. The callback-2 update, particle_updateBounceDebris, reads it as Debris in game/Particle/BounceDebris/bouncedebris.c.
 */
struct ModelFxData {
    /* 0x00 */ f32 position[3];
    /* 0x0C */ f32 rotation[3];
    /* 0x18 */ f32 velocity[3];
    /* 0x24 */ f32 spin[3];
    /* 0x30 */ f32 scale[3];
    /* 0x3C */ s32 life;
    /* 0x40 */ Gfx *displayList;
    /* 0x44 */ u32 pad44;
    /* 0x48 */ f32 friction;
    /* 0x4C */ f32 bounce;
    /* 0x50 */ f32 spinDamp;
    /* 0x54 */ u8 callback;     /* index into gObjectCallbacks */
    /* 0x55 */ u8 primA;
    /* 0x56 */ u8 tileUlt;
    /* 0x57 */ u8 phase;
    /* 0x58 */ u8 flags;
    /* 0x59 */ u8 floorOffset;
    /* 0x5A */ u8 modelId;      /* the attach_loadModelDl resource displayList came from */
    /* 0x5B */ u8 kind;
    /* 0x5C */ u8 renderMode;
    /* 0x5D */ u8 combineMode;
    /* 0x5E */ u8 pad5E[2];
};
typedef struct ModelFx ModelFx;
typedef struct DLCommand DLCommand;
extern void (*gObjectCallbacks[])(); /* object callbacks */
extern TextureDesc *gEffectAnimTextures[]; /* texture table: pointers to texture entries (indexed by the effects' animIndex / texture) */
void effect_runTypeGlobalHooks(void);
void effect_runFrameHooks(void);
void effect_updateObjects(void);
void effect_preUpdateObjects(void);
void effect_preDrawObjects(void);
Gfx *effect_drawObjectGroup(Gfx *gfx, s32 group, s16 arg2, s32 list);
void *effect_newObject(); /* (s32 type, s32 list, s32 size, s32 arg3, u8 owner, u8 arg5); K&R definition, declared without a prototype */
/* The 0x18-byte template effect_spawnSpriteFx copies into a new SpriteFx from its anim field on. */
typedef struct SpriteFxTemplate {
    /* 0x00 */ TextureDesc *anim;
    /* 0x04 */ s16 frame;
    /* 0x06 */ s16 frameStep;
    /* 0x08 */ s16 x;
    /* 0x0A */ s16 y;
    /* 0x0C */ s16 z;
    /* 0x0E */ s16 scaleX;
    /* 0x10 */ s16 scaleY;
    /* 0x12 */ s8 fadeStep;
    /* 0x13 */ u8 pad13;        /* alpha, set by effect_spawnSpriteFx */
    /* 0x14 */ s8 callback;     /* index into gObjectCallbacks */
    /* 0x15 */ u8 unk15;
} SpriteFxTemplate;
void effect_spawnSpriteFx(SpriteFxTemplate *src, u8 owner, s32 list);
void effect_updateSpriteFx(SpriteFx *fx);
Gfx *effect_drawSpriteFx(Gfx *gdl, SpriteFx *fx, u8 arg2);
BillboardParticle *effect_spawnBillboard(BillboardTemplate *src, s32 altType, s32 extra, s8 onDeath, u8 owner, s32 list);
void effect_updateBillboard(BillboardParticle *p);
Gfx *effect_drawBillboard(Gfx *gfx, BillboardParticle *p, s16 viewIndex);
void effect_initQuadFxTemplate(QuadFxData *q, u8 r, u8 g, u8 b, u8 a);
QuadFx *effect_spawnQuadFx(); /* (QuadFxData *src, u8 owner, s32 list); K&R definition, declared without a prototype */
void effect_updateQuadFx(QuadFx *fx);
void effect_updatePendingFree(EffectHeader *obj);
void effect_linkObject(EffectHeader *obj, s32 type);
void effect_unlinkObject(EffectHeader *obj);
void effect_setObjectType(EffectHeader *obj, s32 type);
void effect_billboardDelayFade(void *arg0);
void effect_updateModelFx(ModelFx *fx);
void effect_spawnModelFx(ModelFxData *src, u8 owner, s32 list);
Gfx *effect_drawModelFx(Gfx *gfx, ModelFx *fx, s32 arg2);
void effect_relocateAddress(s32 *addr, s32 offset);
void effect_relocateDisplayList(Gfx *dl, s32 offset);
void effect_rebaseDisplayList(DLCommand *dl, s32 base);
void effect_getDrawGroupTypes(s32 group, s32 *first, s32 *last);
/* Event broadcast API. The event payload is a pointer to a message whose type depends on eventType, but
 * the original API passed it as an integer: callers that pass one address to several calls only match with
 * int parameters. So callers write (s32)&msg.
 */
void effect_broadcastEvent(s32 event, u8 eventType);
void effect_sendEventToTypeRange(s32 first, s32 last, s32 event, u8 eventType);
void effect_sendEventToTypes(const s32 *types, s32 count, s32 event, u8 eventType);
void effect_sendEventToType(s32 type, s32 event, u8 eventType);
void effect_broadcastActorEvent(Actor *actor, u8 eventType);
void effect_sendActorEventToType(s32 type, Actor *actor, u8 eventType);
Gfx *effect_beginBillboardDraw(Gfx *gfx, s32 type, s32 list, s16 view);
void effect_handleOwnerEvent(EffectHeader *obj, u8 *event, u8 eventType);
void effect_fixWalkCursors(EffectHeader *obj);
/* These take any object of the object lists. Each object type has its own struct. The list code uses
 * only the bytes 0x0-0xC of an object (type, list, prev, next, owner), which EffectHeader describes
 * together with flags and state at 0xD-0xF. A struct of only those bytes cannot be a member (its size
 * rounds up to 0x10), and many types (LightEffect, RumbleObj, TextBox, StyleParticle, ...) keep their
 * own fields at 0xD-0xF. */
void effect_destroyObject(void *obj);
void effect_freeObject(void *obj);
void effect_defaultDestroy(void *obj);
void effect_defaultFree(void *obj);
void effect_handleOwnerActorEvent(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj);

/* game/Effects/ModelBurst/modelburst.c */
typedef struct ModelBurstPiece ModelBurstPiece;
void effect_spawnModelBurst(WorldObject *actor, s32 modelId, s32 kind, s32 mode, u8 owner, s32 list);
void effect_spawnLevelModelBurst(WorldObject *actor, u8 owner, s32 list);
void effect_throwBurstPieces(WorldObject *actor, u8 owner, s32 list);
void func_151711C4(Actor *obj);
void effect_scatterModels(s32 count, s32 modelId, f32 x, f32 y, f32 z, f32 speed, f32 minScale, f32 maxScale, f32 radius,
                   s32 kind, s32 life, f32 bounce, u8 owner, s32 list);
void func_15171600(WorldObject *actor, u8 owner, s32 list);
void effect_spawnRectangleParticles(WorldObject *actor, f32 halfX, f32 halfZ, f32 offsetX, f32 offsetY, f32 offsetZ, s32 type, s32 style,
                   s32 countA, s32 countB, f32 scale, u8 owner, s32 list);
void effect_spawnDustRing(f32 x, f32 y, f32 z, f32 radius, s32 baseSize, f32 sizeScale, s32 color, s32 count, u8 owner,
                   s32 list);
void func_15171BF4(Actor *obj, u8 owner);
extern const f32 gModelBurstOffsets[6][3];
extern const ModelBurstPiece gBurstPieces[6];

/* game/Effects/LevelUpdate/levelupdate.c */
extern u8 D_8008CD00;
extern u8 D_800DD404;
extern u8 gLevelTexFrame; /* animation frame of the level textures, advanced by effect_advanceLevelTexFrame */
extern u8 gLevelTexFrameTime; /* frame time accumulated towards the next gLevelTexFrame step */
void effect_advanceLevelTexFrame(s32 arg0, s32 arg1);
void effect_updateLevel(s32 arg0, s32 arg1);
Gfx *effect_drawLevelHook1(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *effect_drawLevelHook2(Gfx *gfx, s32 arg1, s32 arg2);

/* game/Effects/Rain/rain.c */
extern s16 gRainStateTimer; /* rain state timer */
void effect_resetRain(void);
void effect_requestRainState(s32 state);
void effect_updateRain(s32 player);
void effect_centreRainVolume(void);
void effect_spawnRainDrop(s32 scatterY, s32 dropFlags);
void effect_updateRainDrop(QuadFx *drop);
void effect_markRainDropForRemoval(void);
void effect_spawnRainSplashes(s32 count);
void effect_updateRainSplash(QuadFx *splash);

/* game/Effects/FadeBurst/fadeburst.c */
extern Gfx gFadeBurstRenderSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gFadeBurstRenderTeardownDl[]; /* render teardown (object type table gObjectTypes) */
typedef struct FadeBurstEffect FadeBurstEffect;
void effect_spawnFadeBurst(f32 x, f32 y, f32 z, s32 speed, s32 duration, s32 durationJitter, f32 scaleYZ, f32 scaleX);
void effect_updateFadeBurst(FadeBurstEffect *burst);
Gfx *effect_drawFadeBurst(Gfx *gdl, FadeBurstEffect *burst, s32 arg2);
void effect_buildEulerRotation(MtxF mtx, f32 rx, f32 ry, f32 rz);

/* game/Effects/ColorAnim/coloranim.c */
s32 effect_newColorAnim(s32 type, f32 rate, u8 fromR, u8 fromG, u8 fromB, u8 toR, u8 toG, u8 toB);
void effect_clearColorAnims(void);
void effect_getColorAnimColor(s32 index, s32 *rgb);
void effect_getColorAnimBlend(s32 index, f32 *blend);
void effect_setColorAnimBlend(s32 index, f32 blend);
void effect_updateColorAnims(void);

/* game/Effects/TumblingPiece/tumblingpiece.c */
EffectActor *effect_spawnTumblingPiece(Vec3f *position, Vec3f *angle, f32 scale, Vec3f *velocity, Vec3f *angularVelocity,
                           f32 gravity, f32 damping, u8 flags, s16 lifetime, u8 altDisplayList, s32 extraSize, u8 arg11,
                           s32 arg12);
s32 effect_updateTumblingPiece(EffectActor *actor);
s32 effect_buildTumblingPieceMtx(Mtx *mtx, EffectActor *actor);

/* game/Effects/EffectQueue/effectqueue.c */
typedef struct QueueNode QueueNode;
typedef struct Queue Queue;
void effect_updateQueue(Queue *arg);
Queue *effect_newQueue(s32 capacity, s16 lifetime, u8 flags);
QueueNode *effect_newQueueNode(void *value, u8 releaseFunc);
s32 effect_pushQueue(Queue *queue, void *value, u8 releaseFunc);

/* game/Effects/AnchorLight/anchorlight.c */
typedef struct BoundEffect BoundEffect;
void effect_handleAnchorLightEvent(BoundEffect *effect, ActorRef *msg, u8 type);
void effect_unlinkAnchorLight(LightEffect *obj);
void effect_destroyAnchorLight(LightEffect *obj);

/* game/Effects/BurnDamage/burndamage.c */
typedef struct PeriodicTrigger {
    /* 0x0 */ s32 interval;
    /* 0x4 */ s32 elapsed;
    /* 0x8 */ Actor *target;
} PeriodicTrigger;

/* The burning effect of effect_setOnFire (game/Effects/ContactEffects/contacteffects.c): an AnchorEmitter (particle_startAnchorEmitter) whose 12 bytes of extra
 * data are a PeriodicTrigger. Update effect_updateBurnDamage, expire effect_clearOnFire, message effect_periodicTriggerMessage (game/Effects/TriggerSwap/triggerswap.c). */
typedef struct PeriodicObject {
    /* 0x00 */ AnchorEmitter emitter;   /* emitter.group.actor: the burning actor */
    /* 0x8C */ u8 pad8C[4];
    /* 0x90 */ PeriodicTrigger trigger;
} PeriodicObject;
void effect_anchorEmitterMessage(AnchorEmitter *obj, union ActorMsg *msg, u8 event);
s32 effect_updateBurnDamage(PeriodicObject *obj);
void effect_clearOnFire(PeriodicObject *obj);

/* game/Effects/TriggerSwap/triggerswap.c */
typedef struct ObjectPair ObjectPair;
void effect_periodicTriggerMessage(PeriodicObject *self, ObjectPair *pair, u8 msg);
void func_1519072C(Actor *self);

/* game/Effects/SurfaceHandlers/surfacehandlers.c */
void effect_runSurfaceHandler(Actor *obj, s32 arg1);
s32 effect_getSurfaceHandlerGroup(Actor *obj, s32 *group);

/* game/Effects/FallingObject/fallingobject.c */
typedef struct FallingObject FallingObject;
void effect_dropFallingObject(ObjectEmitter *volatile emitter);
void effect_updateFallingObject(FallingObject *obj);

/* game/Effects/ContactEffects/contacteffects.c */
void effect_spawnHitSplash(Actor *obj, u8 arg1, u8 arg2, s32 arg3);
void effect_hitReactSplash(Actor *self, Actor *other, u8 *arg2);
/* (Actor *self, Actor *other, u8 *arg2); declared without a prototype: game/Event/TimedEvent/timedevent.c passes two arguments */
void effect_hitReactSmallSplash();
void effect_hitReactLavaBoulder(Actor *self, Actor *other, u8 *arg2);
void effect_hitReactFireImp(Actor *self, Actor *other, u8 *arg2);
void func_15194408(Actor *self, Actor *other);
void func_15194588(Actor *self, Actor *other, u8 *arg2);
void func_151945CC(Actor *self, Actor *other, u8 *arg2);
void func_15194794(Actor *self, Actor *other, u8 *arg2);
void func_15194810(Actor *self, Actor *other, u8 *arg2);
void effect_hitReactNone(Actor *self, Actor *other, u8 *arg2);
void effect_spawnHitScreenFade(s32 arg0, Actor *self, Actor *other);
void func_15194A68(Actor *self, Actor *other, u8 *arg2);
void func_15194AB4(Actor *obj);
void func_15194B1C(Actor *obj);
void func_15194B94(Actor *self, Actor *other, u8 *arg2);
void effect_hitReactKill(Actor *self, Actor *other, u8 *arg2);
void effect_hitReactSetOnFire(Actor *self, Actor *other, u8 *arg2);
void effect_hitReactSetOnFireDamaging(Actor *self, Actor *other, u8 *arg2);
void effect_hitReactLava(Actor *self, Actor *other, u8 *arg2);
void effect_hitReactImpactDebris(Actor *self, Actor *other, u8 *arg2);
void effect_hitReactFlashAndKill(Actor *self, Actor *other, u8 *arg2);
void func_15194F48(Actor *self, Actor *other, u8 *arg2);
void effect_setOnFire(); /* (Actor *self, Actor *other, u8 arg2); K&R definition, declared without a prototype */
void effect_killAndSplitActor(Actor *obj);
void effect_hitReactActorImpact(Actor *self, Actor *other, u8 *arg2);
void effect_hitReactJointImpact(Actor *self, Actor *other, u8 *arg2);
void func_15195698(Actor *self, Actor *other, u8 *arg2);
void effect_hitReactRumble(Actor *self, Actor *other, u8 *arg2);

/* game_1CC420.c */
void effect_emptyActorEventHook(Actor *arg0, s32 arg1, s32 arg2);

/* game/Effects/ZoneEffect/zoneeffect.c */
struct ZoneEmitter; /* include/conker/particle.h */
void effect_destroyZoneEmitter(struct ZoneEmitter *obj);
void effect_freeZoneEmitter(struct ZoneEmitter *obj);
void func_151A6B68(Actor *target, TriggerZone *zone);
ZoneEffect *effect_findZoneEffect(TriggerZone *zone);

/* game/Effects/DebrisBurst/debrisburst.c */
void func_151A6C90(Actor *obj, u8 arg1, s32 arg2);

/* game/Effects/ActorTasks/actortasks.c */
struct SurfaceTrailOwner;
typedef struct Task151AA30C Task151AA30C;
typedef struct PulseFx PulseFx;
typedef struct Obj151AB3A4 Obj151AB3A4;
typedef struct Obj151AA6D8 Obj151AA6D8;
typedef struct LoopSoundFx LoopSoundFx;
typedef struct EmitterTask EmitterTask;
typedef struct ActorTask ActorTask;
void func_151A99D0(ActorTask *task);
void func_151A9AA4(ActorTask *task);
void func_151A9BA0(ActorTask *task);
void func_151A9CA0(ActorTask *task);
void func_151A9DC0(ActorTask *task);
void func_151A9EC0(ActorTask *task);
void func_151A9FC8(ActorTask *task);
void func_151AA09C(ActorTask *task);
void effect_watcherNoAction(s32 arg0);
void effect_watcherStartSurfaceTrails1(struct SurfaceTrailOwner *owner);
void effect_watcherReleaseSurfaceTrails1(void);
void effect_watcherReleaseSurfaceTrails2(void);
void effect_watcherStartSurfaceTrails2(struct SurfaceTrailOwner *owner);
void func_151AA264(Actor *actor, CollisionHit *hit);
void effect_emitFallingDriftParticle(f32 x, f32 y, f32 z, f32 arg3, f32 scale, f32 arg5, Task151AA30C *task);
PulseFx *effect_spawnTaskPulse(Actor *actor, ActorTask *task);
s32 effect_restartTaskPulse(PulseFx *fx);
void effect_emitStripRiseParticle(Vec3f *from, Vec3f *to, s32 arg2, s32 arg3, f32 t, Obj151AA6D8 *obj);
f32 effect_stepTaskPulse(PulseFx *fx);
void effect_refreshTaskEffects(ActorTask *task);
void effect_destroyTaskPulse(PulseFx *fx);
void effect_freeTaskPulse(PulseFx *fx);
s32 effect_isTaskPulseAlive(PulseFx *fx);
void effect_spawnBoneGroundMark(Actor *actor, u8 index);
s32 effect_fadeTrailByLife(MotionTrail *trail);
void effect_keepTaskFlicker(ActorTask *task);
s32 effect_updateTaskFlicker(FlickerSprite *fx);
s32 effect_detachTaskFlicker(FlickerSprite *fx);
void func_151AB1C4(ActorTask *task);
LoopSoundFx *effect_spawnTaskLoopSound(Actor *actor, ActorTask *task);
void effect_emitDriftRiseParticle(f32 x, f32 y, f32 z, f32 dx, f32 arg4, f32 dz, Obj151AB3A4 *obj);
s32 effect_updateLoopSound(LoopSoundFx *fx);
void effect_destroyLoopSound(LoopSoundFx *fx);
void effect_freeLoopSound(LoopSoundFx *fx);
void func_151AB828(ActorTask *task);
void effect_startPlayerDripEmitter(ActorTask *task);
void func_151AB920(Actor *actor, s32 arg1);
void effect_spawnSparkEmitterTask(Actor *actor);
void effect_updateSparkEmitterTask(EmitterTask *task);
void effect_emitterTaskMessage(EmitterTask *task, ActorMsg *msg, u8 msgId);
void func_151ABE00(Actor *actor);

/* game/Effects/ImpactSets/impactsets.c */
/* Per-impact-type effect selection: indices into the tables of game/Effects/ImpactSets/impactsets.c, -1 for none. */
typedef struct FxImpactSet {
    /* 0x00 */ s8 first;      /* gImpactFirstEffects index */
    /* 0x01 */ s8 firstStyle; /* gImpactSprays index (read by effect_spawnImpactSprayParticle) */
    /* 0x02 */ s8 second;     /* gImpactSecondEffects index */
    /* 0x03 */ s8 emitter;    /* gImpactEmitterConfigs index (read by particle_spawnYawSteam, game/Particle/EmitterQuads/emitterquads.c) */
    /* 0x04 */ s8 impact;     /* gImpactGroundEffects index */
} FxImpactSet; // size 0x5

/* Randomized particle parameters of an impact emitter (particle_spawnYawSteam): each value is base + random * range. */
typedef struct EmitterConfig {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 pitch;
    /* 0x06 */ u16 pitchRange;
    /* 0x08 */ f32 speed;
    /* 0x0C */ f32 speedRange;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u16 unk10Range;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk14Range;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk18Range;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk20Range;
    /* 0x28 */ f32 unk28Chance;
} EmitterConfig; // size 0x2C

extern const FxImpactSet gImpactSets[];
extern const EmitterConfig gImpactEmitterConfigs[];
typedef struct FxSpawnPayload FxSpawnPayload;
typedef struct FxEmitter FxEmitter;
void effect_spawnImpact(Vec3f *pos, CollisionHit *hit, s32 type, u8 arg3, s32 arg4);
s32 effect_spawnImpactSprayParticle(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw, s32 arg9, s32 argA,
                  s32 type, s32 argC, FxSpawnPayload *payload, u8 argE);
s32 effect_fadeImpactTrailByLife(FxEmitter *emitter);
s32 effect_spawnImpactTrailBurst(FxEmitter *emitter, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface);

/* game/Effects/TaskGroundFx/taskgroundfx.c */
void func_151AE6B0(CharacterWatcher *task);

/* game/Effects/LandingBurst/landingburst.c */
typedef struct Game1DD500Task Game1DD500Task;
typedef struct Game1DD500Particle Game1DD500Particle;
s32 func_151B0050(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 pitch, s16 yaw, s32 argA,
                  s32 argB, s32 argC, s32 argD, u8 argE);
void effect_spawnJointImpact(Actor *actor, Actor *other);
void effect_spawnLandingBurst(Actor *actor, u8 arg1, s32 arg2);
Game1DD500Task *effect_startJointEmitterTask(Actor *actor, Actor *other, s16 lifetime, u8 arg3, s32 arg4);
void effect_updateJointEmitterTask(Game1DD500Task *task);
s32 effect_moveReleasedParticle(Game1DD500Particle *particle);
s32 effect_fadeLandingParticleByLife(Game1DD500Particle *particle);
Vtx *effect_buildJointParticleQuad(Game1DD500Particle *particle, s16 view);
s32 effect_swingParticleSize(Game1DD500Particle *arg0);
void effect_freeJointEmitterParticles(Game1DD500Task *task);
void effect_jointEmitterTaskMessage(Game1DD500Task *task, void *msg, u8 type);
void effect_destroyJointEmitterTask(Game1DD500Task *task);
void effect_freeJointEmitterTask(Game1DD500Task *task);

/* game/Effects/SpawnEmitterHook/spawnemitterhook.c */
/* (Actor *spawner, Actor *spawned); declared without a prototype: game/Event/TimedEvent/timedevent.c also passes an angle */
void func_151B5BA0();

/* game/Effects/RibbonChild/ribbonchild.c */
typedef struct Effect1E5820 Effect1E5820;
void effect_destroyRibbonParticle(Effect1E5820 *effect);
void effect_destroyRibbon(Effect1E5820 *effect);
void effect_freeRibbon(Effect1E5820 *effect);

/* game/Effects/FollowEffects/followeffects.c */
typedef struct SoundChannel SoundChannel;
typedef struct OwnedParticle OwnedParticle;
void *effect_spawnOwnerModel(Actor *owner, u8 arg1, s32 arg2);
s32 effect_projectOwnerModelTexCoords(DynModel *effect, s16 viewport);
void effect_ownerModelMessage(DynModel *effect, ActorMsg *msg, u8 msgType);
void effect_spawnTankExplosion(Actor *obj, u8 arg1, s32 arg2);
void effect_startTankSound(Actor *obj);
/* sound update callback */
s32 effect_updateTankSound(SoundChannel *chan, s32 arg1, u32 *volume, s32 arg3, s32 *speedOut, s32 arg5, u16 *soundId);
void effect_spawnShotImpact(Actor *obj, Vec3f *from, Vec3f *to, u8 arg3, s32 arg4);
void effect_spawnMidpointImpact(Actor *obj, f32 *a, f32 *b, u8 arg3, s32 arg4);
void effect_spawnImpactLight(Vec3f *pos, u8 arg1, s32 arg2);
void func_151BBFBC(CollisionTri *surf, Vec3f *pos, s32 arg2, s32 arg3);
void effect_attachTankHeadlightBeam(Actor *obj);
void func_151BC104(Actor *obj, u8 arg1, s32 arg2);
void effect_emitBloodDropSpray(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect);
s32 effect_isBloodTrailOwnerActive(OwnedEffect40 *effect);
void effect_bloodTrailOwnerMessage(OwnedEffect40 *effect, ActorMsg *msg, u8 msgId);
s32 effect_ageRibbonTrail(RibbonTrail *trail);
s32 effect_followRibbonTrail(RibbonTrail *trail);
Gfx *effect_drawRibbonTrail(RibbonTrail *trail, Gfx *gfx, s16 viewport);
void effect_ribbonTrailMessage(RibbonTrail *trail, ActorMsg *msg, u8 msgId);
s32 effect_isLightTrackOwnerValid(OwnedEffect28 *effect);
void effect_spawnBloodStrips(Actor *actor, s32 arg1, s32 arg2);
s32 effect_resetBloodStripAge(AgedBloodStrip *strip);
void effect_emitBloodStripParticle(Vec3f *pointA, Vec3f *pointB, Vec3f *velA, Vec3f *velB, f32 time, BloodStrip *strip);
f32 effect_getBloodStripRate(AgedBloodStrip *strip);
void effect_updateBloodStripParticle(SpriteParticle *particle, Vec3f *prevPos);
void effect_sendBurstOwnerGone(Actor *actor);
void effect_spawnBoneBurst(Actor *actor, u8 arg1, s32 arg2);
void effect_updateBoneBurst(BurstEffect *effect);
void effect_boneBurstMessage(void *effect, s32 msg, u8 msgId);
void effect_freeBurstParts(BurstEffect *effect);
void effect_destroyBoneBurst(BurstEffect *effect);
void effect_freeBoneBurst(BurstEffect *effect);
s32 effect_updateBurstSpark(BurstSpark *spark);
Vtx *effect_buildBurstSparkVtx(BurstSpark *spark, s16 viewport);
void effect_ownedParticleMessage(OwnedParticle *particle, ActorMsg *msg, u8 msgId);
void effect_burstSparkMessage(BurstSpark *spark, s32 msg, u8 msgId);
void effect_unlinkBurstSpark(BurstSpark *spark);
void effect_destroyBurstSpark(BurstSpark *spark);
void effect_freeBurstSpark(BurstSpark *spark);
s32 effect_updateBurstGlow(BurstGlow *glow);
void effect_burstGlowMessage(BurstGlow *glow, s32 msg, u8 msgId);
void effect_unlinkBurstGlow(BurstGlow *glow);
void effect_destroyBurstGlow(BurstGlow *glow);
void effect_freeBurstGlow(BurstGlow *glow);

/* game/Effects/StatCounter/statcounter.c */
/* The two-digit counter of effect_showStatCounter (game/Hud/CounterFree/counterfree.c frees it). The block copied to its panel's +0x120. */
typedef struct {
    /* 0x00 */ Vec3f unk0;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
} PanelExtra;

/* The object sprite_newScreenSprite returns: one digit of the counter. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ f32 x;
    /* 0x14 */ f32 y;
} CounterDigit;

/* Effect data of the two-digit counter that slides onto the screen (the 0x18 bytes at effect+0x28). */
typedef struct {
    /* 0x00 */ ModelEffect *panel;      /* the background panel (effect_newModelEffect), with a PanelExtra */
    /* 0x04 */ CounterDigit *digits[2]; /* digits[1] is NULL for a one-digit count */
    /* 0x0C */ f32 timer;               /* counts down from 110 */
    /* 0x10 */ f32 plateau;             /* the slide curve's value while fully shown */
    /* 0x14 */ u8  flags;               /* bit 0: the slide has started */
} CounterDisplay;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ CounterDisplay display;
} CounterEffect;

struct S151BEB20;
typedef struct S151BECB8 S151BECB8;
void *func_151BE850(Vec3f *pos, f32 angle, u8 count, u8 arg3, u8 variant, u8 owner, s32 list);
s32 func_151BEB20(struct S151BEB20 *arg0);
Gfx *effect_preDrawModelEffectNone(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed);
s32 func_151BECB8(S151BECB8 *arg0);
s32 func_151BEE94(Actor *arg0);
void *func_151BEEE0(f32 rotY, Vec3f *pos, s32 arg2, u8 arg3, u8 variant, u8 arg5, ZoneNode *zone, u8 arg7, s32 arg8);
void func_151BF0C8(Vec3f *pos);
void effect_showStatCounter(u8 owner, s32 kind);
void effect_updateStatCounter(CounterEffect *effect);
void effect_freeStatCounterParts(CounterEffect *effect);
void effect_destroyStatCounter(CounterEffect *effect);

/* game/Effects/ActorSoundBurst/actorsoundburst.c */
void effect_spawnDeathDebrisBurst(Actor *actor, u8 arg1, s32 arg2);

/* game/Effects/BoneFollower/bonefollower.c */
typedef struct BoneFollower BoneFollower;
extern u8 gAnchorRefCounts[20];                 /* reference count per shared anchor resource */
extern u16 gAnchorCounts[20];                /* anchor count of each loaded set */
extern struct BoneAnchor *gAnchorSets[20]; /* the loaded anchor set of each resource */
void effect_acquireAnchorSet(s32 index);
void effect_releaseAnchorSet(s32 index);
BoneFollower *effect_spawnBoneFollower(void *params, Vec3f *position, s32 extraSize, u8 arg3, s32 arg4);
void effect_updateBoneFollower(BoneFollower *follower);
void effect_releaseBoneFollower(BoneFollower *follower);
void effect_destroyBoneFollower(BoneFollower *arg0);
void effect_freeBoneFollower(BoneFollower *arg0);
void effect_boneFollowerMessage(BoneFollower *follower, ActorMsg *msg, u8 type);
void *effect_newFollowerGroup(void *params, s32 extraSize, u8 arg2, s32 arg3); /* an ActorFollowerGroup and extraSize bytes */
void effect_updateFollowerGroup(ActorFollowerGroup *group);
void effect_destroyGroupFollowers(ActorFollowerGroup *group);
void effect_destroyFollowerGroup(ActorFollowerGroup *group);
void effect_freeFollowerGroup(ActorFollowerGroup *group);
void effect_followerGroupMessage(ActorFollowerGroup *group, ActorMsg *msg, u8 type);
void effect_linkBoneFollower(BoneFollower *follower);
void effect_unlinkBoneFollower(BoneFollower *follower);
void effect_clearGroupFollowers(ActorFollowerGroup *group);
void effect_handleFollowerGroupActorGone(ActorFollowerGroup *group, ActorMsg *msg);
void effect_sendEventToFollowerGroups(s32 arg0, u8 arg1);

#endif
