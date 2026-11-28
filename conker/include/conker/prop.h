#ifndef CONKER_PROP_H
#define CONKER_PROP_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "prop": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Prop/WaveChainStart/wavechainstart.c
 *   game/Prop/Scenery/scenery.c
 *   game/Prop/Pendulum/pendulum.c
 *   game/Prop/LinkedSpinners/linkedspinners.c
 *   game/Prop/LinkedToggle/linkedtoggle.c
 *   game/Prop/MoverSwing/moverswing.c
 *   game/Prop/Clock/clock.c
 *   game/Prop/ShakeSink/shakesink.c
 *   game/Prop/Seesaw/seesaw.c
 *   game/Prop/ScrollBob/scrollbob.c
 *   game/Prop/PushBob/pushbob.c
 *   game/Prop/PropMover/propmover.c
 *   game/Prop/SwingSpring/swingspring.c
 *   game/Prop/SwingBounce/swingbounce.c
 *   game/Prop/DoorsLifts/doorslifts.c
 *   game/Prop/Pickup/pickup.c
 *   game/Prop/TextureAnim/textureanim.c
 *   game/Prop/RippleMesh/ripplemesh.c
 *   game/Prop/PropUpdates/propupdates.c
 *   game/Prop/PropMotions/propmotions.c
 *   game/Prop/PropLinks/proplinks.c
 *   game/Prop/DrawHooks/drawhooks.c
 *   game/Prop/WaveDeformer/wavedeformer.c
 *   game/Prop/WaveChain/wavechain.c
 */

/* Behaviour scratch state (WorldObject.touch) of the scenery movers in game/Prop/MoverSwing/moverswing.c, game/Prop/PropMover/propmover.c and
 * game/Prop/SwingBounce/swingbounce.c (each file reaches it through its WORK(obj) macro). */
typedef union MoverWork {
    /* 0x7C */ s32 targetPos[3];  /* position to move to */
    /* 0x7C */ f32 targetSpin[3]; /* rotation to turn to */
    struct {
        /* 0x7C */ s32 target; /* animation position to reach, 0..0xFFFF */
        /* 0x80 */ u8 *frames; /* vertex frames loaded from ROM */
        /* 0x84 */ s32 pos;
    } anim;
    struct {
        /* 0x7C */ f32 damping;
        /* 0x80 */ f32 acceleration;
        /* 0x84 */ f32 velocity;
    } swing;
    struct {
        /* 0x7C */ f32 rebound; /* fraction of the velocity kept (reversed) when the target is crossed */
        /* 0x80 */ f32 acceleration; /* per tick */
        /* 0x84 */ f32 velocity;
    } bounce;
} MoverWork;

/*
 * Vtx_t with named coordinates. matching: Vtx_t's ob[] subscripts make IDO schedule the coordinate
 * loads in a different order.
 */
typedef struct PropVtx {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 cn[4];
} PropVtx;

/* State of prop_wobbleTexture, allocated on its first update: the saved texture coordinates and the pair it passes
 * to prop_animateTexture. */
typedef struct WobbleTextureState {
    TexCoord *coords;
    s32 texIndices;
    s32 animState;
} WobbleTextureState;

/* game/Prop/WaveChainStart/wavechainstart.c */
void prop_clearWaveChains(void);
void prop_startWaveChain(s32 presetId);

/* game/Prop/Scenery/scenery.c */
void prop_emptyLevelUpdateHook2(s32 unused);
void prop_startMirroredEmitters(s32 duration);
struct StyleParticle; /* conker/particle.h */
s32 prop_updateStyleFadeGrow(struct StyleParticle *entry);
void prop_moveToHeightAndBob(WorldObject *obj);
void func_150BF760(WorldObject *obj);
void prop_updateSinkingPlatform(WorldObject *platform);
void prop_spawnDeathBurst(Actor *obj);
struct Piece; /* game/Prop/Scenery/scenery.c */
struct ModelParticle; /* effects.h */
s32 prop_burstPieceOnFloor(struct ModelParticle *particle, f32 x, f32 y, f32 z, f32 floorHeight);
void prop_scatterPieces(Vec3f *origin, void *floor, s32 id, u8 lit, u8 owner, s32 list);
s32 prop_onPieceFloorHit(struct Piece *piece, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);

/* game/Prop/Pendulum/pendulum.c */
extern EffectHeader *gLevelSprayEffects[4]; /* level spray effects (game/Level/LevelSetupDogFish/levelsetupdogfish.c), destroyed by prop_destroyLevelSprays */
void prop_swingPendulum(WorldObject *pendulum);
void prop_rampTextureScroll(WorldObject *obj);
void prop_destroyLevelSprays(void);
s32 func_150C5280(void);
s32 func_150C52CC(void *fx);
s32 func_150C5310(struct ModelParticle *obj);
/* Texture asset ids 0x2C1 and 0x1C0 of prop_rampTextureScroll: link-time symbols whose address is the id (undefined_syms.us.txt). */
extern u8 D_2C1;
extern u8 D_1C0;

/* game/Prop/LinkedSpinners/linkedspinners.c */
void func_150C7870(WorldObject *obj);
void prop_spinOppositeObject3(WorldObject *obj);
void prop_spinWithObject3(WorldObject *obj);
void prop_scrollTextureByObject3(WorldObject *obj);
void prop_levelUpdateMusicAndDebris(s32 view);
void func_150C7C90(WorldObject *obj);
void func_150C7D7C(WorldObject *obj);

/* game/Prop/LinkedToggle/linkedtoggle.c */
void func_150CF0A0(WorldObject *obj);

/* game/Prop/MoverSwing/moverswing.c */
s32 prop_shotHitsCylinders(Vec3f *start, Vec3f *delta, Vec3f *hit);
s32 prop_segmentHitsCylinders(Vec3f *start, Vec3f *end, Vec3f *hit);
void prop_springTiltByPlayerDistance(WorldObject *obj);
void prop_spawnFixedExplosion(void);
void prop_spawnCylinderBurst(TriggerZone *gate, u8 owner, s32 kind);

/* game/Prop/Clock/clock.c */
void prop_levelUpdateRainOnly(s32 unused);
void prop_updateClock(WorldObject *minuteHand);
void prop_swingClockPendulum(WorldObject *obj);
s32 prop_isOutsideFrustum(MtxF mtx, f32 x, f32 y, f32 z, f32 nearZ, f32 farZ, f32 slopeX, f32 slopeY, f32 *marginX,
                  f32 *marginY);

/* game/Prop/ShakeSink/shakesink.c */
/* (vertex, pivot) index pairs: vertices that follow the rotation of a preceding object (prop_rotateAboutPivots). */
typedef struct VertexPivots {
    /* 0x0 */ u8 count;
    /* 0x1 */ u8 pairs[8][2];
} VertexPivots;
typedef struct SoundTriple {
    /* 0x0 */ u16 ids[3];
} SoundTriple;
extern const VertexPivots gPivotVertexPairs;
extern const SoundTriple gReplaceNextSounds;
void prop_emptyBehavior22(WorldObject *obj);
void prop_spinZ(WorldObject *obj);
void prop_spinY(WorldObject *obj);
void prop_spinX(WorldObject *obj);
void prop_emptyBehavior55(WorldObject *obj);
void prop_sinkWhileTriggered(WorldObject *obj);
void prop_shakeAndSink(WorldObject *obj);

/* game/Prop/Seesaw/seesaw.c */
void prop_moveToHeight(WorldObject *obj);
void prop_moveByOffset(WorldObject *obj);
void prop_addSeesawWeight(WorldObject *obj, Actor *player);
void prop_addSeesawWeightBoosted(WorldObject *obj, Actor *player);
void prop_tiltSeesaw(WorldObject *obj);
void prop_scrollTexture(WorldObject *obj);

/* game/Prop/ScrollBob/scrollbob.c */
void prop_registerTextureScroll(WorldObject *obj);
void prop_snapToGround(WorldObject *obj);
void prop_bob(WorldObject *obj);

/* game/Prop/PushBob/pushbob.c */
void prop_bobAndSway(WorldObject *obj);
void prop_updatePush(WorldObject *obj, s32 flags, s32 soundId, f32 pitchScale);
s32 prop_getApproachSpeed(s32 x, s32 z, s32 *unused, Actor *other);
void prop_emptyBehavior71(WorldObject *obj);

/* game/Prop/PropMover/propmover.c */
void prop_triggerMover(WorldObject *obj, Actor *actor);
void prop_updateMover(WorldObject *obj);
void prop_stepToTarget(WorldObject *obj);
void prop_stepToTargetWithSounds(WorldObject *obj);
void prop_spinToTarget(WorldObject *obj);
void prop_animateVertices(WorldObject *obj);
f32 prop_springAngle(WorldObject *obj, f32 angle);
extern PropSounds gPropSounds[16]; // prop sound rows (doors, movers, sliders)

/* game/Prop/SwingSpring/swingspring.c */
void prop_swingZ(WorldObject *obj);
void prop_swingY(WorldObject *obj);

/* game/Prop/SwingBounce/swingbounce.c */
void prop_swingX(WorldObject *obj);
f32 prop_bounceAngle(WorldObject *obj, f32 angle);
void prop_bounceX(WorldObject *obj);
void prop_bounceZ(WorldObject *obj);

/* game/Prop/DoorsLifts/doorslifts.c */
void prop_lowerAndRaise(WorldObject *prop);
void prop_slideOut(WorldObject *prop);
void prop_triggerOnPlayer(WorldObject *prop, Actor *obj);
void prop_localToWorld(WorldObject *prop, f32 x, f32 y, f32 z, f32 *out);
void prop_grabAndSwingCharacter(WorldObject *prop);
void prop_swingDoorLink0(WorldObject *prop);
void prop_swingDoorLink3(WorldObject *prop);
void prop_swingDoor(WorldObject *prop, s32 mode);
void prop_swingFlap(WorldObject *prop);
void prop_triggerOnFreePlayer(WorldObject *prop, Actor *obj);
void prop_followHeight(WorldObject *prop);
void prop_floodFillVertexGroup(WorldObject *prop, PropVtx *seed, s32 minDistSq, s32 group);
void prop_initSwayGroups(WorldObject *prop);
void prop_swayVertexGroups(WorldObject *prop);

/* game/Prop/Pickup/pickup.c */
void prop_updatePickup(WorldObject *obj);

/* game/Prop/TextureAnim/textureanim.c */
s32 prop_findSetTimg(Gfx *dl, s32 *second);
void prop_animateTexture(WorldObject *obj, s32 *texIndices, s32 *animState);
void prop_scrollAndAnimateTexture(WorldObject *obj);
void prop_bobAndAnimateTexture(WorldObject *obj);

/* game/Prop/RippleMesh/ripplemesh.c */
void prop_startRipple(WorldObject *mesh);
void prop_updateRippleMesh(WorldObject *mesh);

/* game/Prop/PropUpdates/propupdates.c */
void prop_wobbleTilt(WorldObject *obj);
void prop_slideWaypoints(WorldObject *obj);
void prop_swingAndFadeColours(WorldObject *obj);
void prop_rotateAboutPivots(WorldObject *obj);
void prop_setAlphaFromColorAnim(WorldObject *obj);
void prop_projectTexture(WorldObject *obj, f32 refX, f32 refZ, f32 scaleS, f32 scaleT);
void prop_projectTextureUnderRef(WorldObject *obj);
void prop_faceCamera(WorldObject *obj);
void prop_grabAndLowerCharacter(WorldObject *obj);
void prop_updateNone(void);
void prop_sway(WorldObject *obj);
void prop_replaceNextWhenTriggered(WorldObject *obj);
void prop_getWaveOriginZ(WorldObject *obj, f32 *originZ);
void prop_animateWaveMesh(WorldObject *obj);
void prop_scrollWithOwnerWave(WorldObject *obj);
void prop_wobbleTexture(WorldObject *mesh);
void prop_rotateTexture(WorldObject *obj);
void prop_swingOpen90(WorldObject *obj);
void prop_playImpactSound(WorldObject *obj, Actor *character);
void prop_pulseUntilTouched(WorldObject *obj);
void prop_swingHinge(WorldObject *prop);
void prop_raiseWhileTriggered(WorldObject *prop);
void prop_slideZUntilTriggered(WorldObject *prop);

/* game/Prop/PropMotions/propmotions.c */
void prop_carryCharacterInArc(WorldObject *prop);
void prop_pinToLinkedObject(WorldObject *prop);
void prop_fadeOutAfterDelay(WorldObject *prop);
void prop_partFourPieces(WorldObject *prop);
void prop_start(WorldObject *prop, Actor *actor);
void prop_bobThenSink(WorldObject *prop);
void prop_copyLinkedPos(WorldObject *prop);

/* game/Prop/PropLinks/proplinks.c */
void prop_clearLinks(void);
void prop_registerLink(WorldObject *prop, s32 mode);

/* game/Prop/DrawHooks/drawhooks.c */
void prop_emptyFrameHook(s32 bufferIndex);
Gfx *prop_emptyDrawEffectAngled(Gfx *gfx, f32 x, f32 y, f32 z, f32 rot, s32 view);
Gfx *prop_emptyDrawEffect(Gfx *gfx, f32 x, f32 y, f32 z, s32 view);

/* game/Prop/WaveDeformer/wavedeformer.c */
extern WaveDeformer *gWaveDeformers[2]; /* the wave deformers, gNumWaveDeformers of them in use */
extern u8 gNumWaveDeformers;
void prop_createWaveDeformer(u8 flags, u8 objectId, s16 x, s16 y, s16 z, f32 heading, u16 radius, f32 amplitude, s8 speed,
                   s8 phaseStep, u8 r, u8 rAmp, u8 g, u8 gAmp, u8 b, u8 bAmp);
void prop_updateWaveDeformers(void);

/* game/Prop/WaveChain/wavechain.c */
extern u8 gNumWaveChains;                /* active wave chains */
extern u8 gWaveChainPresetIds[4];             /* per chain: its gWaveChainPresets preset */
extern WaveSample *gWaveChainSamples[8];    /* samples of each active wave chain (40 each) */
extern WavePreset gWaveChainPresets[4];     /* static parameters of each wave spring chain */
void prop_updateWaveChains(void);
void prop_bendWaveChainMesh(WorldObject *prop);
f32 prop_getWaveChainSample(s32 dist, s32 chain);
s32 prop_getWaveChainPush(Actor *actor, s32 sample, s32 chain);

#endif
