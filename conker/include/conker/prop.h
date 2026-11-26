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

/* A piece spawned by particle_spawnDebrisScatter, as the handlers below see it. */
typedef struct {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0xB];
    /* 0x018 */ f32 unk18;
    /* 0x01C */ u8 pad1C[0x1C];
    /* 0x038 */ Vec3f position;
    /* 0x044 */ u8 pad44[0x1C];
    /* 0x060 */ s32 unk60;
    /* 0x064 */ u8 pad64[0x2];
    /* 0x066 */ u16 unk66;
    /* 0x068 */ u8 pad68[0xA8];
    /* 0x110 */ u8 unk110[0x60];
    /* 0x170 */ f32 burstChance; /* prop_scatterPieces's extraData */
} Piece;

/* An entry whose level ramps toward maxLevel while `rising` is set and back to 0 otherwise. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[0x7];
    /* 0x1F */ u8 level;
    /* 0x20 */ u8 pad20[0x4];
    /* 0x24 */ u8 rising;
    /* 0x25 */ u8 pad25;
    /* 0x26 */ u8 maxLevel;
    /* 0x27 */ u8 riseRate;
    /* 0x28 */ u8 pad28[0x4];
    /* 0x2C */ u8 age;
    /* 0x2D */ s8 unk14Rate;
    /* 0x2E */ s8 unk16Rate;
    /* 0x2F */ u8 fallRate;
} Struct150BF0F4;

typedef struct {
    /* 0x00 */ u8  pad0[0x60];
    /* 0x60 */ s32 flags;
} Struct150C5310;

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

typedef struct Game1483E0UvVertex {
    s16 x, y, z;
    u8 unknown06[2];
    s16 s, t;
    u32 unknown0C;
} Game1483E0UvVertex;

typedef struct Game1483E0UvPair {
    s16 s, t;
} Game1483E0UvPair;

typedef struct Game1483E0State {
    u8 unknown00[0x10];
    s16 x, y, z;
    u16 count;
    u8 unknown18[8];
    Game1483E0UvVertex *buffers[2];
    Game1483E0UvVertex *source;
    u8 unknown2C[0x13];
    u8 reference_id;
    u8 unknown40[0x3C];
    Game1483E0UvPair *coords;
    Actor *reference;
} Game1483E0State;

typedef struct Game1483E0WaveState {
    Game1483E0UvPair *coords;
    s32 field4;
    s32 field8;
} Game1483E0WaveState;

typedef struct Game1483E0WaveMesh {
    u8 pad0[0x16];
    u16 count;
    u8 pad18[8];
    Game1483E0UvVertex *buffers[2];
    Game1483E0UvVertex *source;
    u8 pad2C[0x10];
    s32 flags; /* bits 0-11: S amplitude, 12-23: T amplitude, 24-31: speed */
    u8 pad40[0x3C];
    Game1483E0WaveState *wave;
    s32 phase;
} Game1483E0WaveMesh;

/* game/Prop/WaveChainStart/wavechainstart.c */
void prop_clearWaveChains(void);
void prop_startWaveChain(s32 arg0);

/* game/Prop/Scenery/scenery.c */
void func_150BEF70(s32 arg0);
void prop_startMirroredEmitters(s32 arg0);
s32 func_150BF0F4(Struct150BF0F4 *entry);
void func_150BF21C(WorldObject *obj);
void func_150BF760(WorldObject *obj);
void prop_updateSinkingPlatform(WorldObject *platform);
void func_150BFFE0(Actor *obj);
s32 func_150C01DC(Piece *piece, s32 arg1, s32 arg2, s32 arg3, f32 y);
void prop_scatterPieces(Vec3f *origin, void *arg1, s32 id, u8 arg3, u8 arg4, s32 arg5);
s32 prop_onPieceFloorHit(Piece *piece, f32 x, f32 y, f32 z, f32 floorHeight, CollisionTri *surface);

/* game/Prop/Pendulum/pendulum.c */
extern EffectHeader *gLevelSprayEffects[4]; /* level spray effects (game/Level/LevelSetup08/levelsetup08.c), destroyed by prop_destroyLevelSprays */
void prop_swingPendulum(WorldObject *pendulum);
void func_150C4E9C(WorldObject *obj);
void prop_destroyLevelSprays(void);
s32 func_150C5280(void);
s32 func_150C52CC(void *fx);
s32 func_150C5310(Struct150C5310 *obj);
/* Texture asset ids 0x2C1 and 0x1C0 of func_150C4E9C: link-time symbols whose address is the id (undefined_syms.us.txt). */
extern u8 D_2C1;
extern u8 D_1C0;

/* game/Prop/LinkedSpinners/linkedspinners.c */
void func_150C7870(WorldObject *obj);
void func_150C78E0(WorldObject *obj);
void func_150C7930(WorldObject *obj);
void func_150C7968(WorldObject *obj);
void func_150C79BC(s32 view);
void func_150C7C90(WorldObject *obj);
void func_150C7D7C(WorldObject *obj);

/* game/Prop/LinkedToggle/linkedtoggle.c */
void func_150CF0A0(WorldObject *arg0);

/* game/Prop/MoverSwing/moverswing.c */
s32 prop_shotHitsCylinders(Vec3f *start, Vec3f *delta, Vec3f *hit);
s32 prop_segmentHitsCylinders(Vec3f *start, Vec3f *end, Vec3f *hit);
void func_150EB614(WorldObject *obj);
void func_150EB8C4(void);
void prop_spawnCylinderBurst(TriggerZone *gate, u8 owner, s32 kind);

/* game/Prop/Clock/clock.c */
void func_1510AA20(s32 arg0);
void prop_updateClock(WorldObject *minuteHand);
void func_1510ADD8(WorldObject *obj);
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
extern const SoundTriple D_800A2F84;
void func_151150B0(WorldObject *obj);
void prop_spinZ(WorldObject *obj);
void prop_spinY(WorldObject *obj);
void prop_spinX(WorldObject *obj);
void func_1511529C(s32 arg0);
void prop_sinkWhileTriggered(WorldObject *obj);
void prop_shakeAndSink(WorldObject *obj);

/* game/Prop/Seesaw/seesaw.c */
void prop_moveToHeight(WorldObject *obj);
void prop_moveByOffset(WorldObject *obj);
void prop_addSeesawWeight(WorldObject *obj, Actor *player);
void func_15115EDC(WorldObject *obj, Actor *player);
void prop_tiltSeesaw(WorldObject *obj);
void prop_scrollTexture(WorldObject *obj);

/* game/Prop/ScrollBob/scrollbob.c */
void prop_registerTextureScroll(WorldObject *obj);
void prop_snapToGround(WorldObject *obj);
void prop_bob(WorldObject *obj);

/* game/Prop/PushBob/pushbob.c */
void prop_bobAndSway(WorldObject *obj);
void prop_updatePush(WorldObject *obj, s32 arg1, s32 soundId, f32 pitchScale);
s32 prop_getApproachSpeed(s32 x, s32 z, s32 *unused, Actor *other);
void func_15116924(s32 arg0);

/* game/Prop/PropMover/propmover.c */
void func_15116930(WorldObject *obj, Actor *actor);
void prop_updateMover(WorldObject *obj);
void prop_stepToTarget(WorldObject *obj);
void func_15116BAC(WorldObject *obj);
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
void func_151177C0(WorldObject *prop);
void prop_slideOut(WorldObject *prop);
void prop_triggerOnPlayer(WorldObject *prop, Actor *obj);
void prop_localToWorld(WorldObject *prop, f32 x, f32 y, f32 z, f32 *out);
void func_15117F3C(WorldObject *prop);
void func_1511896C(WorldObject *prop);
void func_1511898C(WorldObject *prop);
void prop_swingDoor(WorldObject *prop, s32 arg1);
void prop_swingFlap(WorldObject *prop);
void prop_triggerOnFreePlayer(WorldObject *prop, Actor *obj);
void func_151193F4(WorldObject *prop);
void prop_floodFillVertexGroup(WorldObject *prop, PropVtx *seed, s32 minDistSq, s32 group);
void prop_initSwayGroups(WorldObject *prop);
void prop_swayVertexGroups(WorldObject *prop);

/* game/Prop/Pickup/pickup.c */
void prop_updatePickup(WorldObject *obj);

/* game/Prop/TextureAnim/textureanim.c */
s32 prop_findSetTimg(Gfx *dl, s32 *second);
void prop_animateTexture(WorldObject *obj, s32 *texIndices, s32 *animState);
void func_1511A6FC(WorldObject *obj);
void func_1511A738(WorldObject *obj);

/* game/Prop/RippleMesh/ripplemesh.c */
void prop_startRipple(WorldObject *mesh);
void prop_updateRippleMesh(WorldObject *mesh);

/* game/Prop/PropUpdates/propupdates.c */
void func_1511AF30(WorldObject *obj);
void prop_slideWaypoints(WorldObject *obj);
void func_1511B51C(WorldObject *obj);
void prop_rotateAboutPivots(WorldObject *obj);
void func_1511BA24(WorldObject *obj);
void prop_projectTexture(Game1483E0State *obj, f32 refX, f32 refZ, f32 scaleS, f32 scaleT);
void prop_projectTextureUnderRef(Game1483E0State *obj);
void prop_faceCamera(WorldObject *obj);
void func_1511BEBC(WorldObject *obj);
void prop_updateNone(void);
void prop_sway(WorldObject *obj);
void func_1511C638(WorldObject *obj);
void prop_getWaveOriginZ(WorldObject *obj, f32 *originZ);
void prop_animateWaveMesh(WorldObject *obj);
void func_1511D394(WorldObject *obj);
void prop_wobbleTexture(Game1483E0WaveMesh *mesh);
void prop_rotateTexture(WorldObject *obj);
void func_1511DBC4(WorldObject *obj);
void prop_playImpactSound(WorldObject *obj, Actor *character);
void func_1511DF6C(WorldObject *obj);
void prop_swingHinge(WorldObject *prop);
void prop_raiseWhileTriggered(WorldObject *prop);
void func_1511ED84(WorldObject *prop);

/* game/Prop/PropMotions/propmotions.c */
void func_1511EF40(WorldObject *prop);
void func_1511F31C(WorldObject *prop);
void prop_fadeOutAfterDelay(WorldObject *prop);
void func_1511F4D0(WorldObject *prop);
void prop_start(WorldObject *prop, s32 arg1);
void prop_bobThenSink(WorldObject *prop);
void prop_copyLinkedPos(WorldObject *prop);

/* game/Prop/PropLinks/proplinks.c */
void prop_clearLinks(void);
void prop_registerLink(WorldObject *prop, s32 mode);

/* game/Prop/DrawHooks/drawhooks.c */
void func_1511FC20(s32 arg0);
Gfx *func_1511FC2C(Gfx *gfx, f32 x, f32 y, f32 z, f32 rot, s32 view);
Gfx *func_1511FC44(Gfx *gfx, f32 x, f32 y, f32 z, s32 view);

/* game/Prop/WaveDeformer/wavedeformer.c */
extern WaveDeformer *gWaveDeformers[2]; /* the wave deformers, gNumWaveDeformers of them in use */
extern u8 gNumWaveDeformers;
void prop_createWaveDeformer(u8 flags, u8 objectId, s16 x, s16 y, s16 z, f32 heading, u16 radius, f32 amplitude, s8 speed,
                   s8 phaseStep, u8 argA, u8 argB, u8 argC, u8 argD, u8 argE, u8 argF);
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
