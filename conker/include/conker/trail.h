#ifndef CONKER_TRAIL_H
#define CONKER_TRAIL_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "trail": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_DC360.c
 *   game_E1280.c
 *   game_EEB10.c
 *   game_100810.c
 *   game_105FC0.c
 *   game_175250.c
 *   game_1897A0.c
 *   game_1B5CC0.c
 *   game_1B6DB0.c
 *   game_1B8F40.c
 *   game_1CA420.c
 *   game_1CC440.c
 *   game_1D0840.c
 *   game_1E37D0.c
 *   game_1FA770.c
 *   game_1FEA80.c
 *   game_204660.c
 */

/* One point of a trail's ring buffer. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 width;
    /* 0x1C */ u16 alpha;
    /* 0x1E */ s16 life;
    /* 0x20 */ u8 texS;
    /* 0x21 */ u8 pad21[3];
} ScatterTrailPoint; // size = 0x24

/* Per-trail settings and emitter state, stored behind trail->state; the first 0x40 bytes are the
 * TrailStateParams given to func_150B3F5C. */
typedef struct {
    /* 0x00 */ Vec3f baseVel;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ u8 phaseJitter;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 speedBase;
    /* 0x18 */ f32 speedSwing;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ f32 emitRate;
    /* 0x24 */ u8 shade;
    /* 0x25 */ u8 pad25;
    /* 0x26 */ s16 alpha;
    /* 0x28 */ f32 widthGrowth;
    /* 0x2C */ f32 width;
    /* 0x30 */ u8 texStep;
    /* 0x31 */ u8 texStepJitter;
    /* 0x32 */ u8 fadeTailCount;
    /* 0x33 */ u8 fadeTailStep;
    /* 0x34 */ u8 fadeHeadCount;
    /* 0x35 */ u8 fadeHeadStep;
    /* 0x36 */ s16 life;
    /* 0x38 */ u8 burstPreset;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ f32 burstRate;
    /* 0x40 */ f32 speed;
    /* 0x44 */ f32 emitAccum;
    /* 0x48 */ u8 texS;
    /* 0x49 */ u8 pad49[3];
    /* 0x4C */ f32 speedTarget;
    /* 0x50 */ u8 phase;
    /* 0x51 */ u8 pad51[3];
    /* 0x54 */ f32 burstAccum;
} ScatterTrailState; // size = 0x58

/* A trail that throws out ScatterPreset particle bursts (D_8009FBF0) from its emitter as it goes. */
typedef struct ScatterTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (func_151D5D60) */
    /* 0x94 */ ScatterTrailPoint *points;
    /* 0x98 */ ScatterTrailState *state;
} ScatterTrail;

/* Written by func_15147A80's caller into a glow at 0x170 (GlowParams.anim's place), read by func_150D4CC4. */
typedef struct {
    /* 0x0 */ s16 timer;
    /* 0x4 */ f32 targetScale;
} TrailGlowData;

/* A glow returned by func_151407D0. */
typedef struct TrailGlow {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ GlowShape shape;          /* shape.scale eases toward tail.targetScale (func_150D4CC4) */
    /* 0x16C */ u8 pad16C[4];
    /* 0x170 */ TrailGlowData tail;
} TrailGlow;

/* Per-trail state, at CharacterTrail.state; also the sound callbacks' user data. */
typedef struct {
    Actor *owner;
    u8 ownerId;         // owner->unique_id, to notice the owner slot being reused
    u8 mode;            // 0, 1 or 2 (from owner speed or stick tilt); selects sounds and sizes
    u8 pad6[2];
    ModelAttachment *attachment; // func_1503195C(owner, 0x79 / 0x7A): the trail runs between two of its points
    TrailGlow *effects[2];
    f32 width;          // trail width at the head, eased toward targetWidth each frame
    f32 targetWidth;
} CharacterTrailState; // size 0x1C

typedef struct {
    Vec3f pos;
    f32 len;            // distance from the previous point
    f32 width;
} CharacterTrailPoint;

/* The actor returned by func_15147A80. */
typedef struct CharacterTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ Vtx *vtxBufs[4];
    /* 0x94 */ CharacterTrailPoint *points;
    /* 0x98 */ CharacterTrailState *state;
} CharacterTrail;

/* Sound slot passed to func_150D5124 by the sound system. */
typedef struct TrailSound {
    s16 sound;
    s16 x;
    s16 y;
    s16 z;
    u8 pad8[4];
    s32 volume;
    u8 pad10[8];
    CharacterTrailState *state;  // user data given to func_1000FA64
    s32 soundId;
    s16 pitch;
    u8 pad22[2];
    u16 voiceHandle; /* SoundRecord.sound_id: the handle func_100111C8 stops */
} TrailSound;

/* One point of a trail's ring buffer (0x24 bytes). */
typedef struct {
    Vec3f pos;
    Vec3f vel;
    f32 len;        /* distance to the newer neighbour */
    u8 alpha;
    u8 pad1D[3];
    f32 texture;    /* texture s coordinate */
} PissTrailPoint;

/* Trail state block (0x50 bytes), copied into trail->data at spawn. */
typedef struct {
    Actor *owner;
    u8 ownerId;
    u8 pad5[3];
    Vec3f unk8;
    Vec3f lastPos;
    Vec3f lastVel;
    f32 time;
    u16 sound;
    u8 pad32[2];
    f32 pending;        /* fractional points still to emit */
    f32 texScroll;
    f32 speedScale;
    f32 surfaceAccum;   /* hit effect accumulators, see func_150DA67C */
    f32 impactAccum;
    f32 sparkAccum;
    u8 flags;
    s8 timer;
    u8 pad4E[2];
} PissTrailData;

/* Trail object created by func_150D8B88 (game_90840.c calls it when the actor starts pissing, mv->pssd_is_pissing). */
typedef struct PissTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (func_151D5D60) */
    /* 0x94 */ PissTrailPoint *points;
    /* 0x98 */ PissTrailData *data;
} PissTrail;

/* Message broadcast to trails by func_15147D64. */
typedef struct TrailMessage {
    Actor *owner;
    union {
        u8 ownerId;             /* types 0, 0x44 */
        Actor *other;       /* type 0x2D */
    } x4;
    union {
        f32 speedScale;         /* type 0x44 */
        struct {
            u8 ownerId;
            u8 otherId;
        } ids;                  /* type 0x2D */
    } x8;
} TrailMessage;


/* One point of the stream ring buffer. */
typedef struct StreamPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 velocityY;
    /* 0x10 */ f32 width;
    /* 0x14 */ s16 alpha;
    /* 0x16 */ u8 texCoord;
    /* 0x17 */ u8 pad17;
} StreamPoint; // size 0x18

/* Per-stream tuning, copied into the actor's state by func_150DBD70. */
typedef struct StreamState {
    /* 0x00 */ f32 swayX;
    /* 0x04 */ u8 angleX;
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 angleXRange;
    /* 0x08 */ f32 swayZ;
    /* 0x0C */ u8 angleZ;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 angleZRange;
    /* 0x10 */ f32 x;
    /* 0x14 */ f32 z;
    /* 0x18 */ f32 gravity;
    /* 0x1C */ f32 rate;
    /* 0x20 */ f32 count;
    /* 0x24 */ f32 floor; /* points below this height splash */
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 alpha;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ f32 widthBase;
    /* 0x30 */ f32 widthRange;
    /* 0x34 */ u8 widthAngle;
    /* 0x35 */ u8 pad35;
    /* 0x36 */ s16 widthAngleRange;
    /* 0x38 */ f32 velocityY;
    /* 0x3C */ u8 texCoordStep;
    /* 0x3D */ u8 pad3D;
    /* 0x3E */ s16 texCoordStepRange;
    /* 0x40 */ u8 texCoord;
    /* 0x41 */ u8 trailTexture; /* D_80090B60 index */
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 pad43;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 pad61[3];
    /* 0x64 */ f32 spawnRate;
    /* 0x68 */ f32 spawnCount;
    /* 0x6C */ f32 radius;
    /* 0x70 */ s16 pitchBase;
    /* 0x72 */ s16 pitchRange;
    /* 0x74 */ f32 speedBase;
    /* 0x78 */ f32 speedRange;
    /* 0x7C */ s16 lifetime;
    /* 0x7E */ s16 accelBase;
    /* 0x80 */ f32 accelRange;
    /* 0x84 */ s16 fadeBase;
    /* 0x86 */ s16 fadeRange;
    /* 0x88 */ s16 sizeBase;
    /* 0x8A */ u8 pad8A[2];
    /* 0x8C */ f32 sizeRange;
    /* 0x90 */ TextureDesc *texture;
    /* 0x94 */ s32 frameRate;
    /* 0x98 */ s8 callback;
    /* 0x99 */ u8 pad99[3];
    /* 0x9C */ f32 splashRate;
    /* 0xA0 */ f32 splashCount;
    /* 0xA4 */ f32 splashRadius;
    /* 0xA8 */ s16 splashPitchBase;
    /* 0xAA */ s16 splashPitchRange;
    /* 0xAC */ f32 splashSpeedBase;
    /* 0xB0 */ f32 splashSpeedRange;
    /* 0xB4 */ f32 splashScaleBase;
    /* 0xB8 */ f32 splashScaleRange;
    /* 0xBC */ f32 splashSizeBase;
    /* 0xC0 */ f32 splashSizeRange;
    /* 0xC4 */ s16 splashLifetime;
    /* 0xC6 */ s16 splashCountBase;
    /* 0xC8 */ s16 splashCountRange;
    /* 0xCA */ u8 unkCA;
    /* 0xCB */ u8 padCB;
    /* 0xCC */ f32 dropRate;
    /* 0xD0 */ f32 dropCount;
    /* 0xD4 */ f32 dropRadius;
    /* 0xD8 */ s16 dropPitchBase;
    /* 0xDA */ s16 dropPitchRange;
    /* 0xDC */ f32 dropSpeedBase;
    /* 0xE0 */ f32 dropSpeedRange;
    /* 0xE4 */ s16 dropLifetime;
    /* 0xE6 */ s16 dropAccelBase;
    /* 0xE8 */ s16 dropAccelRange;
    /* 0xEA */ s16 dropFadeBase;
    /* 0xEC */ s16 dropFadeRange;
    /* 0xEE */ s16 dropSizeBase;
    /* 0xF0 */ f32 dropSizeRange;
    /* 0xF4 */ TextureDesc *dropTexture;
    /* 0xF8 */ s32 dropFrameRate;
    /* 0xFC */ s8 dropCallback;
    /* 0xFD */ u8 padFD[3];
    /* 0x100 */ f32 ringY;
    /* 0x104 */ f32 ringRate;
    /* 0x108 */ f32 ringCount;
    /* 0x10C */ f32 ringScaleBase;
    /* 0x110 */ f32 ringScaleRange;
    /* 0x114 */ u8 soundTimer;
    /* 0x115 */ u8 pad115;
    /* 0x116 */ s16 sound; /* 0: silent */
    /* 0x118 */ s16 timer;
    /* 0x11A */ u8 pad11A[2];
} StreamState; // size 0x11C

/* A falling stream (game_109220.c): the object func_15147A80 returns. */
struct StreamActor {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (func_151D5D60) */
    /* 0x94 */ StreamPoint *records;
    /* 0x98 */ StreamState *state;
};

/* One point of a trail. */
typedef struct FallingTrailPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 velY;
} FallingTrailPoint; /* size 0x10 */

/* A trail object made by func_15147A80: a ring buffer of points falling under gravity. */
typedef struct FallingTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (func_151D5D60) */
    /* 0x94 */ FallingTrailPoint *points;
    /* 0x98 */ TrailBurstParams *data;
} FallingTrail;

/* Data block at 0x28 of the effect func_1515D088 spawns. */
typedef struct {
    /* 0x0 */ TriggerZone *owner;
    /* 0x4 */ f32 unk4;
    /* 0x8 */ s8 kind;
} LevelEffectData; /* size 0xC */

typedef struct LevelEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ LevelEffectData data;
} LevelEffect;

/* Emitter origin: a position and the radius particles spawn at around it. */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ s16 radius;
} EmitterOrigin;

/* Emitter state at 0x28 of its owner. */
typedef struct {
    /* 0x0 */ EmitterOrigin *origin;
    /* 0x4 */ f32 accum;  /* particles owed; one is emitted per whole unit */
    /* 0x8 */ u8 preset;  /* index into D_800A64A0 */
} TrailBurstState;

/* Timed effect of func_1515D130. */
typedef struct TrailBurstEmitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ TrailBurstState emitter;
} TrailBurstEmitter;

/* Params of a trail spawned by a SprayEmitter: the TrailParams, then the floor height. */
typedef struct {
    /* 0x00 */ TrailParams base;
    /* 0x48 */ f32 floorY;
} SprayTrailParams;

/* A MotionTrail (func_15147DA0) whose params carry SprayTrailParams. */
typedef struct SprayTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ MotionTrailPoint *points;
    /* 0x98 */ SprayTrailParams *params;
} SprayTrail;

typedef struct SprayEmitter {
    /* 0x00 */ EffectHeader link;
    /* 0x10 */ SprayDesc desc;
    /* 0x60 */ s16 idleTimer; /* frames until the next check for a nearby player */
    /* 0x62 */ u8 pad62[2];
    /* 0x64 */ f32 pending; /* particles owed, accumulated by rate */
    /* 0x68 */ f32 floorY;  /* lowest point of the spawn segment */
    /* 0x6C */ ZoneNode *zone;  /* from func_1510FD20 at the segment's midpoint */
    /* 0x70 */ u8 flags;
    /* 0x71 */ u8 pad71[7];
} SprayEmitter; /* size 0x78 */

/* Emitter settings, copied into the object from a template by func_1518BCD0. */
typedef struct TrailEmitterParams {
    /* 0x00 */ f32 speedMin;
    /* 0x04 */ f32 speedRange;
    /* 0x08 */ f32 halfWidthMin;
    /* 0x0C */ f32 halfWidthRange;
    /* 0x10 */ s16 pointRange; /* each trail gets 3 + rand % (pointRange + 1) points */
    /* 0x12 */ s16 angleRange;
    /* 0x14 */ Actor *owner;
    /* 0x18 */ u8 ownerId;     /* owner->unique_id when attached */
    /* 0x19 */ u8 texture;
    /* 0x1A */ u8 flags;
} TrailEmitterParams; /* size 0x1C */

/* Type 0x1F object: emits trails from two points of its owner actor. */
typedef struct TrailEmitter {
    /* 0x00 */ EffectHeader link;
    /* 0x10 */ TrailEmitterParams params;
    /* 0x2C */ s32 timers[2]; /* time until the next trail from each point */
    /* 0x34 */ f32 prevX[2];  /* each point's position on the previous update */
    /* 0x3C */ f32 prevZ[2];
} TrailEmitter; /* size 0x44 */

/* A trail's own data (0x60 bytes), at MatrixTrail::data. */
typedef struct MatrixTrailData {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;      /* owner's unique_id when the trail started */
    /* 0x05 */ u8 matrixIndex;  /* which of the owner's matrices the trail follows */
    /* 0x06 */ u8 flags;        /* bit 0: stopped, bit 1: fade in the head, bit 2: fade out the tail */
    /* 0x07 */ u8 texS;         /* texture S of the next point */
    /* 0x08 */ Vec3f offset;    /* attachment point, in the matrix's space */
    /* 0x14 */ Vec3f lastPos;   /* where the last point was emitted */
    /* 0x20 */ f32 time;        /* age of the last emitted point */
    /* 0x24 */ f32 gravity;
    /* 0x28 */ f32 spawnRate;
    /* 0x2C */ f32 spawnCount;  /* points owed, accumulated from spawnRate */
    /* 0x30 */ f32 drag;
    /* 0x34 */ f32 widthGrowth;
    /* 0x38 */ f32 width;
    /* 0x3C */ f32 follow;      /* how far the emitter moves towards the attachment point per frame */
    /* 0x40 */ u8 brightness;
    /* 0x41 */ u8 pad41;
    /* 0x42 */ s16 life;
    /* 0x44 */ s16 delay;
    /* 0x46 */ s16 lifeDecay;
    /* 0x48 */ s16 texStep;
    /* 0x4A */ s16 texStepRange;
    /* 0x4C */ u8 specIndex;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ Vec3f ownerPos;  /* owner position the points were last moved with */
    /* 0x5C */ u8 headFadeLen;
    /* 0x5D */ u8 headFadeStep;
    /* 0x5E */ u8 tailFadeLen;
    /* 0x5F */ u8 tailFadeStep;
} MatrixTrailData;

/* One point of a trail (0x24 bytes). */
typedef struct MatrixTrailPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 width;
    /* 0x1C */ s16 life;   /* counts down by lifeDecay once delay ran out; dead below 0 */
    /* 0x1E */ s16 delay;
    /* 0x20 */ u8 alpha;
    /* 0x21 */ u8 texS;
    /* 0x22 */ u8 pad22[2];
} MatrixTrailPoint;

/* A trail object made by func_15147A80, following a matrix of its owner. */
typedef struct MatrixTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuffers[0x10];
    /* 0x94 */ MatrixTrailPoint *points;
    /* 0x98 */ MatrixTrailData *data;
} MatrixTrail;

/* Data of a trail task (func_151491F4), at TrailTask::data. */
typedef struct TrailTaskData {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x08 */ s32 unk8;  /* spec index for func_1519E464, a frame timer for func_1519E754 */
} TrailTaskData;

typedef struct TrailTask {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ TrailTaskData data;
} TrailTask;

/* Event payloads the handlers below get (see func_15147D64). */
typedef union TrailEvent {
    ActorRef object;    /* events 0 and 7: trails of this owner stop */
    u8 specIndex;       /* event 6: trails of this kind stop (func_1519CF70) */
    ActorSwapMsg swap;  /* event 0x2D: owners actorA and actorB trade places */
} TrailEvent;

typedef struct TrailSide {
    /* 0x0 */ struct SurfaceTrail *trail;
    /* 0x4 */ struct TrailSpawner *spawner;
} TrailSide;

typedef struct TrailSet {
    /* 0x0 */ TrailSide sides[2]; /* [0] = side 6, [1] = side 7 */
    /* 0x10 */ f32 sideOffset;    /* distance of each trail from the actor's centre line */
} TrailSet;

/* A character watcher (func_15158BD0) whose extra data is the character's two surface trails. */
typedef struct SurfaceTrailOwner {
    /* 0x00 */ CharacterWatcher watcher; /* watcher.hit: the floor the trails are laid on */
    /* 0x58 */ TrailSet trails;
} SurfaceTrailOwner;

/* Per-trail parameters, copied into the trail object when it is created (0x50 bytes). */
typedef struct SurfaceTrailConfig {
    /* 0x00 */ SurfaceTrailOwner *owner;
    /* 0x04 */ u8 flags;           /* 1: detached (ends below 2 points), 2: fade in at the tail, 4: fade out at the head */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 pointsPerUnit;   /* new points emitted per unit of distance moved */
    /* 0x0C */ f32 pendingPoints;
    /* 0x10 */ Vec3f emitPos;   /* where the last point was emitted */
    /* 0x1C */ f32 emitTime;
    /* 0x20 */ f32 actorX;
    /* 0x24 */ f32 actorZ;
    /* 0x28 */ s16 texS;            /* texture s of the next point */
    /* 0x2A */ u8 color[3];
    /* 0x2D */ u8 pad2D;
    /* 0x2E */ s16 pointLife;
    /* 0x30 */ s16 pointAlpha;
    /* 0x32 */ s16 pointFadeDelay;
    /* 0x34 */ s16 alphaDecay;      /* alpha lost per frame once the fade delay has run out */
    /* 0x36 */ u8 pad36[2];
    /* 0x38 */ f32 startWidth;
    /* 0x3C */ f32 widthGrowth;
    /* 0x40 */ u8 tailFadeCount;
    /* 0x41 */ u8 tailFadeStep;
    /* 0x42 */ u8 headFadeCount;
    /* 0x43 */ u8 headFadeStep;
    /* 0x44 */ u8 texSStep;
    /* 0x45 */ u8 texSStepRandom;
    /* 0x46 */ s16 texT;
    /* 0x48 */ s16 texTSpeed;
    /* 0x4A */ s16 texTSpan;
    /* 0x4C */ f32 driftSpeed;
} SurfaceTrailConfig;

typedef struct SurfaceTrailPoint {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 z;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 width;
    /* 0x10 */ s16 alpha;
    /* 0x12 */ s16 fadeDelay;
    /* 0x14 */ s16 fade; /* alpha scale out of 256 for the fades at either end */
    /* 0x16 */ s16 texS;
    /* 0x18 */ s16 life;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ TrailVec2 vel; /* drift velocity */
} SurfaceTrailPoint; /* size 0x24 */

typedef struct SurfaceTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ CollisionHit collision; /* the last ground probe */
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (func_151D5D60) */
    /* 0x94 */ SurfaceTrailPoint *points;
    /* 0x98 */ SurfaceTrailConfig *config;
} SurfaceTrail;

typedef struct SurfaceTrailLink {
    /* 0x0 */ SurfaceTrailOwner *owner;
    /* 0x4 */ u8 side;
} SurfaceTrailLink;

typedef struct TrailSpawner {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ SurfaceTrailLink link;
} TrailSpawner;

/*
 * Creation parameters of a trail (a ribbon of points that follows a point
 * on one of an actor's bones). The same 0x30 bytes start BoneTrailState.
 */
typedef struct BoneTrailParams {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;          /* owner->unique_id when created */
    /* 0x05 */ u8 bone;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ Vec3f offset;        /* attachment point in bone space */
    /* 0x14 */ f32 pointsPerUnit;   /* points emitted per unit travelled */
    /* 0x18 */ s16 intensity;       /* initial BoneTrailPoint.intensity */
    /* 0x1A */ s16 holdTime;        /* initial BoneTrailPoint.holdTime */
    /* 0x1C */ s16 fadeRate;        /* intensity lost per tick after holdTime */
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ f32 width;           /* width of a new point */
    /* 0x24 */ f32 widthGrowth;     /* width gained per second */
    /* 0x28 */ u8 texStep;          /* texture coordinate advance per point... */
    /* 0x29 */ u8 texStepRandom;    /* ...plus a random 0..texStepRandom */
    /* 0x2A */ s8 texScroll;        /* texture offset scroll per tick */
    /* 0x2B */ u8 tailFadeCount;
    /* 0x2C */ u8 tailFadeStep;
    /* 0x2D */ u8 headFadeCount;
    /* 0x2E */ u8 headFadeStep;
    /* 0x2F */ u8 pad2F;
} BoneTrailParams;

/* Per-trail state, stored at BoneTrailEffect.state. */
typedef struct {
    /* 0x00 */ BoneTrailParams params;
    /* 0x30 */ u8 flags;
    /* 0x31 */ u8 pad31[3];
    /* 0x34 */ Vec3f lastPos;       /* head position when points were last emitted */
    /* 0x40 */ f32 pending;         /* fractional points not yet emitted */
    /* 0x44 */ u8 texCoord;
    /* 0x45 */ u8 pad45[3];
    /* 0x48 */ f32 age;
    /* 0x4C */ u8 texOffset;
    /* 0x4D */ u8 pad4D[3];
} BoneTrailState;

/* One point of a trail. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 width;
    /* 0x10 */ s16 intensity;       /* alpha scale; the point dies when it drops below 0 */
    /* 0x12 */ s16 holdTime;        /* ticks before intensity starts to fall */
    /* 0x14 */ u8 alpha;
    /* 0x15 */ u8 texCoord;
    /* 0x16 */ u8 pad16[2];
} BoneTrailPoint;

/* Effect object created by func_15147A80 for a trail. points[] is a ring buffer. */
typedef struct BoneTrailEffect {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (func_151D5D60) */
    /* 0x94 */ BoneTrailPoint *points;
    /* 0x98 */ BoneTrailState *state;
} BoneTrailEffect;

/* Emits particles from a point on one of the owner's bones (task data of func_151A499C). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;          /* owner->unique_id when created */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 rate;            /* emissions per second */
    /* 0x0C */ f32 pending;         /* emissions accumulated, not yet spawned */
    /* 0x10 */ u8 bone;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ Vec3f offset;        /* emission point in bone space */
} BoneEmitter;

typedef struct BoneEmitterTask {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoneEmitter emitter;
} BoneEmitterTask;

/* A func_151491F4 task that retries creating a trail until its owner can carry it. */
typedef struct TrailRetryTask {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoneTrailParams trail;
} TrailRetryTask;

/* An actor a trail follows and its unique_id: argument of a ribbon effect (selector 0) and of message 0. */
typedef struct TrailTargetRef {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetUnk3B;
} TrailTargetRef;

/* The object func_151CEA20 integrates: a 1-D position under constant acceleration, and a fade. */
typedef struct TrailScalarMotion {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 position;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ f32 acceleration;
    /* 0x44 */ f32 velocity;
    /* 0x48 */ u8 pad48[4];
    /* 0x4C */ f32 fadeRate;
    /* 0x50 */ f32 fade;
} TrailScalarMotion;

/* One point of a RibbonActor's ring buffer. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ u8 pad0C[8];
    /* 0x14 */ f32 width;
    /* 0x18 */ TrailVec2 offset; /* half-width vector across the ribbon */
    /* 0x20 */ f32 dirX;
    /* 0x24 */ f32 dirZ;
} RibbonActorPoint;

typedef struct {
    Actor *owner;
} RibbonInfo;

/* Trail actor (func_15147A80, see game_105FC0.c) drawn as a ribbon through its ring buffer of points. */
typedef struct RibbonActor {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuffer[0x10];
    /* 0x94 */ RibbonActorPoint *points;
    /* 0x98 */ RibbonInfo *info;
} RibbonActor;

/* Data of the camera-facing strip effect spawned by func_151D0058 and drawn by func_151D014C. */
typedef struct {
    /* 0x0 */ Actor *owner;
    /* 0x4 */ u8 ownerId;   /* owner->unique_id at spawn; a mismatch means the owner died */
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u16 ownerUnk84; /* owner->unk84 at spawn; the effect ends when it changes */
    /* 0x8 */ f32 scale;
    /* 0xC */ u8 alpha;
    /* 0xD */ u8 variant;   /* which pair of model points the strip joins */
    /* 0xE */ u8 padE[2];
} StripEffectData;

typedef struct StripEffect {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ StripEffectData data;
} StripEffect;

typedef struct OwnedEffect {
    TimedEffect base;
    ActorRef ref;
} OwnedEffect;

/* Partial view of the particle func_15130280 creates. */
typedef struct CarrierParticle {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ struct WaveTrail *effect; /* wave trail that travels with the particle (func_151CEAAC) */
} CarrierParticle;

/* Emitter data at 0x28: where the trail is anchored this frame. */
typedef struct {
    /* 0x00 */ struct AnchoredTrail *trail; /* attached trail, or NULL */
    /* 0x04 */ u8 anchorType;       /* index into D_8008FCA0 (update) and D_8008FCA4 (message handler) */
    /* 0x05 */ u8 flags;            /* bit 0: pos is valid this frame */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 maxStep; /* moving further than this in one frame detaches the trail */
} TrailAnchor;

/* Emitter data at 0x40 for anchor type 0: a point on an actor's bone. */
typedef struct TrailBoneLink {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId;  /* actor->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 bone;     /* index into D_800AB280 (offset) and D_800AB2D4 (bone matrix) */
    /* 0x06 */ s8 condition; /* index into D_8008FCA8, or -1 for none */
} TrailBoneLink;

typedef struct AnchoredTrailEmitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ TrailAnchor anchor;
    /* 0x40 */ TrailBoneLink link;
} AnchoredTrailEmitter;

/* A trail point. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 velY;
    /* 0x10 */ f32 length; /* distance to the next newer point */
    /* 0x14 */ u8 alpha;
    /* 0x18 */ f32 texU;
} AnchoredTrailPoint;

/* Trail data at 0x98. */
typedef struct {
    /* 0x00 */ AnchoredTrailEmitter *emitter; /* NULL once detached */
    /* 0x04 */ Vec3f lastPos;        /* where the last point was emitted */
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 spawnAccum;
    /* 0x18 */ f32 texPhase;
} AnchoredTrailData;

typedef struct AnchoredTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (func_151D5D60) */
    /* 0x94 */ AnchoredTrailPoint *points;
    /* 0x98 */ AnchoredTrailData *data;
} AnchoredTrail;

/* game_DC360.c */
void func_150AEEB0(Actor *obj, u8 owner);
s32 func_150AF1C0(MotionTrail *trail);

/* game_E1280.c */
void func_150B3DD0(void);
void func_150B3E74(ScatterTrail *trail);
void func_150B3EE8(ScatterTrail *trail);
ScatterTrail *func_150B3F5C(TrailStateParams *params, Vec3f *pos, s16 capacity);
s32 func_150B40E8(ScatterTrail *trail);
s32 func_150B4294(ScatterTrail *trail);
Gfx *func_150B4710(ScatterTrail *trail, Gfx *gfx, s16 camIdx);
void func_150B5060(ScatterTrail *trail);
void func_150B5088(ScatterTrail *trail);
void func_150B538C(ScatterTrail *trail, s32 arg1, u8 msg);

/* game_EEB10.c */
void func_150C1660(f32 x, f32 y, f32 z, u8 arg3);
s32 func_150C16C0(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s32 angle, s32 baseAngle, f32 radius, s32 arg11, f32 arg12, s32 arg13, u8 arg14);
s32 func_150C1978(MotionTrail *trail);

/* game_100810.c */
void func_150D3360(Actor *owner, u8 arg1, s32 arg2);
s32 func_150D3A68(CharacterTrail *actor);
s32 func_150D3FD4(CharacterTrail *actor);
Gfx *func_150D4300(CharacterTrail *actor, Gfx *gfx, s16 view);
void func_150D49C0(CharacterTrail *actor, ActorMsg *msg, u8 id);
s32 func_150D4AE0(Vec3f *start, Vec3f *end, Actor *owner, ModelAttachment *attachment);
void func_150D4BD4(CharacterTrail *actor);
void func_150D4C00(CharacterTrail *actor);
void func_150D4C2C(CharacterTrail *actor);
s32 func_150D4CC4(TrailGlow *particle);
void func_150D4D58(Vec3f *pos, const Vec3f *vel, f32 angle, u8 arg3, s32 arg4);
s32 func_150D5124(TrailSound *snd, s32 arg1, u32 *arg2, s32 arg3, s32 *pitchOut, s32 arg5, u16 *playing);

/* game_105FC0.c */
void func_150D8B10(Actor *actor, Vec3f *out);
void func_150D8B3C(Actor *actor, Vec3f *out);
void func_150D8B88(Actor *actor);
void func_150D8D84(Vec3f *pos, Vec3f *vel, f32 dt);
void func_150D8E1C(PissTrail *trail);
s32 func_150D8E4C(PissTrail *trail);
s32 func_150D8FAC(PissTrail *trail);
s32 func_150D942C(PissTrail *trail);
Gfx *func_150D9C7C(PissTrail *trail, Gfx *gfx, s16 view);
void func_150DA484(PissTrail *trail);
void func_150DA4B4(PissTrail *trail);
void func_150DA4E0(PissTrail *trail);
void func_150DA50C(PissTrail *trail, TrailMessage *msg, u8 type);
void func_150DA5EC(Actor *actor, f32 speedScale);
s32 func_150DA628(SpriteParticle *particle, s32 arg1);
void func_150DA67C(PissTrail *trail, RayHit *hit, f32 *dir);

/* game_175250.c */
/* Returns the new MotionTrail; callers whose params extend TrailParams (extraSize) keep it in their own view. */
void *func_15147DA0(EffectTemplate *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 arg4, s32 finishFn, s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 arg9, s32 arg10, TrailRender *render, CollisionHit *hit, u8 arg13, s32 arg14);
s8 func_15147EB8(MotionTrail *trail);
Gfx *func_1514803C(MotionTrail *trail, Gfx *gfx, s16 arg2);
s32 func_151488C4(MotionTrail *trail);
s32 func_15148AF4(MotionTrail *trail);
s32 func_15148BA4(MotionTrail *trail);
s32 func_15148DE0(MotionTrail *trail);
s32 func_15148EF8(MotionTrail *trail, s32 arg1, s32 arg2, s32 arg3);
void func_15148F1C(u8 texture, f32 x, f32 y, f32 z, s32 arg4, s32 yaw, s32 pitch, f32 speed, s16 arg8, f32 halfWidth, f32 gravity, u8 arg11);
s32 func_151490C8(MotionTrail *trail);
void func_15149104(EffectObj *obj);

/* game_1897A0.c */
FallingTrail *func_1515C2F0(EffectTemplate *header, s32 extraSize, TrailBurstParams *data, CollisionHit *hit, u8 arg4, s32 arg5);
s32 func_1515C388(FallingTrail *trail);
s32 func_1515C534(FallingTrail *trail);
Gfx *func_1515C6F4(FallingTrail *trail, Gfx *gfx, s16 view);
s32 func_1515CF9C(FallingTrail *trail, TrailBurstParams *data);
s32 func_1515D030(FallingTrail *trail, TrailBurstParams *data);
LevelEffect *func_1515D088(TriggerZone *obj);
void func_1515D130(TrailBurstEmitter *owner);

/* game_1B5CC0.c */
extern MtxS16 *D_800DF7C0;       /* bone matrices drawn by func_1502C974 mode 4 */
extern u8 D_800DF7C4;            /* alpha scale for func_1502C974 mode 4 */
extern BoneTrail *D_800DF7C8[2]; /* [0]: Actor owners, [1]: world object owners */
void func_15188810(Actor *object, s32 duration, s32 maxDist);
void func_1518894C(WorldObject *owner, s32 duration, s32 maxDist);
void func_15188A58(BoneTrail *trail, BoneTrail **list);
void func_15188A9C(Actor *object);
void func_15188AD0(Actor *object);
void func_15188B74(s32 listIndex);
Gfx *func_15188D00(Gfx *gfx, s32 listIndex, s16 player);
void func_15188E48(BoneTrail *trail);
void func_15188F84(BoneTrail *arg);
void func_15189118(s32 listIndex);
void func_151895A4(BoneTrail *trail);
void func_151895CC(BoneTrail *trail);
Gfx *func_151895F4(Gfx *gfx, BoneTrail *trail, s16 player);
Gfx *func_151897A4(Gfx *gfx, BoneTrail *trail, s16 player);
void func_151898C0(Actor *object, s32 value);

/* game_1B6DB0.c */
void func_15189900(SprayDesc *desc, u8 arg1);
void func_15189A00(SprayEmitter *emitter);
s32 func_15189EBC(SprayTrail *trail);
s32 func_15189FD0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* game_1B8F40.c */
TrailEmitter *func_1518BA90(Actor *actor, u8 owner, s32 list);
void func_1518BBF4(Actor *actor);
TrailEmitter *func_1518BCD0(TrailEmitterParams *params, u8 owner, s32 list);
void func_1518BD60(TrailEmitter *emitter);
s32 func_1518C0B8(MotionTrail *trail);
s32 func_1518C540(MotionTrail *trail);
s32 func_1518C57C(MotionTrail *trail, s32 arg1, s32 arg2, s32 arg3, f32 hitY);
s32 func_1518C69C(MotionTrail *trail, f32 x, f32 y, f32 z, f32 hitY, CollisionTri *surface);
void func_1518C850(TrailEmitter *emitter, s32 data, u8 type);

/* game_1CA420.c */
extern TrailTask *D_800E0920;
void func_1519CF70(u8 specIndex);
void func_1519CFA0(MatrixTrail *trail);
void func_1519CFD0(MatrixTrail *trail);
void func_1519D000(MatrixTrail *trail);
MatrixTrail *func_1519D030(Actor *owner, s32 specIndex, s16 lifetime, u8 arg3, u8 arg4, s32 arg5);
s32 func_1519D240(MatrixTrail *trail);
s32 func_1519D454(MatrixTrail *trail);
Gfx *func_1519D9F4(MatrixTrail *trail, Gfx *gfx, s16 view);
void func_1519E1F4(MatrixTrail *trail, TrailEvent *event, u8 eventId);
s32 func_1519E304(MatrixTrailData *data, Vec3f *out, f32 x, f32 y, f32 z, f32 t);
void func_1519E3BC(Actor *owner, s16 lifetime, u8 arg2, s32 arg3);
void func_1519E464(TrailTask *task);
void func_1519E570(TrailTask *task, TrailEvent *event, u8 eventId);
void func_1519E61C(EffectHeader *task, TrailEvent *event, u8 eventId);
void func_1519E65C(s32 unused);
void func_1519E688(void);
void func_1519E6BC(Actor *owner);
void func_1519E754(TrailTask *task);
void func_1519E818(TrailTask *task, TrailEvent *event, u8 eventId);
void func_1519E8CC(TrailTask *task);
void func_1519E8F8(TrailTask *task);
void func_1519E924(TimedEffect *task);
void func_1519E948(TimedEffect *task);

/* game_1CC440.c */
void func_1519F108(SurfaceTrail *trail);
void func_1519F168(SurfaceTrail *trail);
void func_1519F3B8(SurfaceTrailOwner *owner);
/* (SurfaceTrailOwner *owner); declared without a prototype: func_151AA1D0 and func_151AA1F0 pass no argument
   (the owner is still in a0) */
void func_1519F400();
void func_1519F48C(SurfaceTrail *trail);
s32 func_1519F4F0(SurfaceTrail *trail);
s32 func_1519F7F0(SurfaceTrail *trail);
Gfx *func_1519FE6C(SurfaceTrail *trail, Gfx *gfx, s16 arg2);
void func_151A084C(TrailSpawner *spawner);
void func_151A0928(TrailSpawner *spawner);
void func_151A0950(SurfaceTrail *trail, ActorRef *event, u8 type);
void func_151A09B4(TrailSpawner *spawner, ActorRef *event, u8 type);

/* game_1D0840.c */
void func_151A3390(Actor *owner, u8 arg1);
BoneTrailEffect *func_151A3504(BoneTrailParams *params, u8 arg1);
s32 func_151A361C(BoneTrailEffect *effect);
s32 func_151A37C0(BoneTrailEffect *effect);
Gfx *func_151A3BE4(BoneTrailEffect *effect, Gfx *gfx, s16 view);
void func_151A4590(Actor *owner, u8 arg1);
void func_151A4638(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, EffectHeader *parent);
s32 func_151A483C(SpriteParticle *particle, u8 arg1);
s32 func_151A4900(SpriteParticle *particle, u8 arg1);
void func_151A499C(Actor *owner, u8 arg1);
void func_151A4A38(BoneEmitterTask *task);
void func_151A4CE0(BoneTrailEffect *effect, ActorMsg *msg, u8 type);
void func_151A4D88(BoneEmitterTask *task, ActorMsg *msg, u8 type);
s32 func_151A4E34(BoneTrailParams *params, Vec3f *out);
void func_151A4E9C(BoneTrailEffect *effect);
void func_151A4ECC(TrailRetryTask *task);
void func_151A4F7C(TrailRetryTask *task, ActorMsg *msg, u8 type);

/* game_1E37D0.c */
void func_151B6320(Actor *target, u8 arg1, s32 arg2);
s32 func_151B6420(EffectObj *obj);
s32 func_151B65D4(EffectObj *obj);
Gfx *func_151B6928(EffectObj *obj, Gfx *gfx, s16 view);
void func_151B70B4(EffectObj *obj, void *msg, u8 msgType);
void func_151B7144(Actor *target, u8 arg1, s32 arg2);
EffectObj *func_151B7328(void *args, u8 selector, s32 argsSize, u8 arg3, s32 arg4);
s32 func_151B7678(EffectObj *obj, Vec3f *pos);
s32 func_151B76CC(EffectObj *obj, Vec3f *pos);
s32 func_151B77F4(EffectObj *obj);
s32 func_151B7998(EffectObj *obj);
Gfx *func_151B7C38(EffectObj *obj, Gfx *gfx, s16 view);
void func_151B82CC(EffectObj *obj, void *msg, u8 msgType);
void func_151B8318(EffectObj *obj, void *msg, u8 msgType);

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
void func_151CE51C(Actor *actor, TriggerZone *source);
LevelStream *func_151CE634(s32 id);

/* Data of the random-point emitter (a TimedEffect's data). */
typedef struct Game1FBB80Effect {
    /* 0x00 */ TriggerZone *shape; /* the zone whose area the packets start in (func_151432BC) */
    /* 0x04 */ f32 rate;
    /* 0x08 */ f32 accumulator;
    /* 0x0C */ ZoneNode *zone;     /* the emitter's zone: it emits only while a view can see it */
    /* 0x10 */ CollisionHit floor; /* the last floor probe (func_1504530C) under a packet */
} Game1FBB80Effect;

typedef struct Game1FBB80Object {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Game1FBB80Effect effect;
} Game1FBB80Object;

void func_151CE6D0(Game1FBB80Object *object);
s32 func_151CEA20(TrailScalarMotion *motion);
WaveTrail *func_151CEAAC(Actor *owner, DescParticleHead *anchor, u8 arg2, u8 arg3, s32 arg4);
s32 func_151CEC10(Vec3f *out, Actor *owner, DescParticleHead *anchor);
s32 func_151CEC54(WaveTrail *trail);
s32 func_151CF120(WaveTrail *actor);
Gfx *func_151CF380(RibbonActor *ribbon, Gfx *gfx, s16 cameraIdx);
void func_151CF844(TrackerTrail *actor, s32 arg1, u8 arg2);
void func_151CF898(Actor *obj, f32 probeY, f32 arg2);
void func_151CFA4C(Actor *obj, u8 arg1, s32 arg2);
void func_151D0024(Actor *obj);
void func_151D0058(Actor *obj, u8 variant, u8 arg2, s32 arg3);
void func_151D0128(StripEffect *effect);
Gfx *func_151D014C(Gfx *gfx, StripEffect *effect, s16 cameraIdx);
void func_151D08F0(OwnedEffect *effect, void *msg, u8 type);
void func_151D09A8(Actor *obj, u8 arg1, s32 arg2);
void func_151D0ED8(CarrierParticle *particle);
void func_151D0F08(CarrierParticle *particle);
void func_151D0F34(CarrierParticle *particle);
void func_151D0F60(Actor *obj, u8 pointFn, u8 arg2, s32 arg3);
s32 func_151D1074(Actor *obj, Vec3f *out);
s32 func_151D10A4(Actor *obj, Vec3f *out);
s32 func_151D10C4(Actor *obj, Vec3f *out);
s32 func_151D10E4(Actor *obj, Vec3f *out, u8 which);
void func_151D1138(TrackerEffect *effect);
void func_151D1328(TrackerEffect *effect, s32 arg1, u8 arg2);
void func_151D1368(TrackerEffect *effect);
void func_151D1388(TrackerEffect *effect);
void func_151D13B4(TrackerEffect *effect);
void func_151D13E0(TrackerEffect *effect);
void func_151D1448(TrackerEffect *effect);

/* game_1FEA80.c */
s32 func_151D15D0(TrackerTrail *actor);
s32 func_151D197C(TrackerTrail *actor);
Gfx *func_151D1C98(TrackerTrail *actor, Gfx *gfx, s16 arg2);
void func_151D223C(TrackerTrail *actor);
void func_151D2258(TrackerTrail *actor);
void func_151D2284(TrackerTrail *actor);

/* game_204660.c */
TimedEffect *func_151D71B0(s16 arg0, u8 arg1, u8 anchorType, f32 maxStep, s32 extraSize, u8 arg5, s32 arg6);
void func_151D7264(AnchoredTrailEmitter *emitter);
void func_151D73A8(AnchoredTrailEmitter *emitter, TrailBoneLink *msg, u8 msgType);
void func_151D7404(AnchoredTrailEmitter *emitter);
void func_151D7424(AnchoredTrailEmitter *emitter);
void func_151D7450(AnchoredTrailEmitter *emitter);
void func_151D747C(Actor *actor);
void func_151D74B0(Actor *actor, u8 bone, s8 condition, u8 arg3, s32 arg4);
void func_151D7538(AnchoredTrailEmitter *emitter, TrailBoneLink *msg, u8 msgType);
s32 func_151D75C4(AnchoredTrailEmitter *emitter);
s32 func_151D7724(AnchoredTrailEmitter *emitter);
s32 func_151D7770(AnchoredTrailEmitter *emitter);
s32 func_151D779C(AnchoredTrailEmitter *emitter);
void func_151D77C8(AnchoredTrailEmitter *emitter);
void func_151D7830(AnchoredTrailEmitter *emitter);
s32 func_151D792C(AnchoredTrail *trail);
s32 func_151D7A38(AnchoredTrail *trail);
s32 func_151D7CD0(AnchoredTrail *trail);
Gfx *func_151D80C4(AnchoredTrail *trail, Gfx *gfx, s16 view);
void func_151D8718(Vec3f *pos, f32 *velY, f32 dt);
void func_151D8764(AnchoredTrail *trail);
void func_151D8780(AnchoredTrail *trail);
void func_151D87AC(AnchoredTrail *trail);

#endif
