#ifndef CONKER_TRAIL_H
#define CONKER_TRAIL_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "trail": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_data_800D9860.c (data only, owner unknown; linked after game/Script/ScriptInterp/scriptinterp)
 *   game/Effects/Trail/TrailSpray/trailspray.c
 *   game/Effects/Trail/ScatterTrail/scattertrail.c
 *   game/Effects/Trail/TrailRing/trailring.c
 *   game/Effects/Trail/CharacterTrail/charactertrail.c
 *   game/Effects/Trail/PissTrail/pisstrail.c
 *   game/Effects/Trail/MotionTrail/motiontrail.c
 *   game/Effects/Trail/FallingTrail/fallingtrail.c
 *   game/Effects/Trail/Afterimage/afterimage.c
 *   game/Effects/Trail/SprayEmitter/sprayemitter.c
 *   game/Effects/Trail/TrailEmitter/trailemitter.c
 *   game/Effects/Trail/MatrixTrail/matrixtrail.c
 *   game/Effects/Trail/SurfaceTrail/surfacetrail.c
 *   game/Effects/Trail/BoneTrail/bonetrail.c
 *   game/Effects/Trail/RibbonTrail/ribbontrail.c
 *   game/Effects/Trail/LevelStream/levelstream.c
 *   game/Effects/Trail/TrackerTrail/trackertrail.c
 *   game/Effects/Trail/AnchoredTrail/anchoredtrail.c
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
 * TrailStateParams given to trail_spawnScatterTrail. */
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

/* A trail that throws out ScatterPreset particle bursts (gScatterPresets) from its emitter as it goes. */
typedef struct ScatterTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (particle_getViewBuffer) */
    /* 0x94 */ ScatterTrailPoint *points;
    /* 0x98 */ ScatterTrailState *state;
} ScatterTrail;

/* Written by effect_newEffectObj's caller into a glow at 0x170 (GlowParams.anim's place), read by trail_updateTrailGlow. */
typedef struct {
    /* 0x0 */ s16 timer;
    /* 0x4 */ f32 targetScale;
} TrailGlowData;

/* A glow returned by flare_spawnGlow. */
typedef struct TrailGlow {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ GlowShape shape;          /* shape.scale eases toward tail.targetScale (trail_updateTrailGlow) */
    /* 0x16C */ u8 pad16C[4];
    /* 0x170 */ TrailGlowData tail;
} TrailGlow;

/* Per-trail state, at CharacterTrail.state; also the sound callbacks' user data. */
typedef struct {
    Actor *owner;
    u8 ownerId;         // owner->unique_id, to notice the owner slot being reused
    u8 mode;            // 0, 1 or 2 (from owner speed or stick tilt); selects sounds and sizes
    u8 pad6[2];
    ModelAttachment *attachment; // attach_find(owner, 0x79 / 0x7A): the trail runs between two of its points
    TrailGlow *effects[2];
    f32 width;          // trail width at the head, eased toward targetWidth each frame
    f32 targetWidth;
} CharacterTrailState; // size 0x1C

typedef struct {
    Vec3f pos;
    f32 len;            // distance from the previous point
    f32 width;
} CharacterTrailPoint;

/* The actor returned by effect_newEffectObj. */
typedef struct CharacterTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ Vtx *vtxBufs[4];
    /* 0x94 */ CharacterTrailPoint *points;
    /* 0x98 */ CharacterTrailState *state;
} CharacterTrail;

/* Sound slot passed to trail_characterTrailSoundCallback by the sound system. */
typedef struct TrailSound {
    s16 sound;
    s16 x;
    s16 y;
    s16 z;
    u8 pad8[4];
    s32 volume;
    u8 pad10[8];
    CharacterTrailState *state;  // user data given to sound_addDynamicSound
    s32 soundId;
    s16 pitch;
    u8 pad22[2];
    u16 voiceHandle; /* SoundRecord.sound_id: the handle sound_stop stops */
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
    f32 surfaceAccum;   /* hit effect accumulators, see trail_spawnPissImpact */
    f32 impactAccum;
    f32 sparkAccum;
    u8 flags;
    s8 timer;
    u8 pad4E[2];
} PissTrailData;

/* Trail object created by trail_spawnPissTrail (game/Actor/ActorActions/actoractions.c calls it when the actor starts pissing, mv->pssd_is_pissing). */
typedef struct PissTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (particle_getViewBuffer) */
    /* 0x94 */ PissTrailPoint *points;
    /* 0x98 */ PissTrailData *data;
} PissTrail;

/* Message broadcast to trails by effect_sendEventToEffectObjs. */
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

/* Per-stream tuning, copied into the actor's state by effect_createFallingStream. */
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
    /* 0x28 */ u8 colorLevel;   /* grey level of the env colour */
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
    /* 0x41 */ u8 trailTexture; /* gEffectTextures index */
    /* 0x42 */ u8 renderModes;  /* low nibble: gEffectRenderModes index, high nibble: combiner */
    /* 0x43 */ u8 pad43;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 texAxis;      /* 2: the texture runs along T, else along S */
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

/* A falling stream (game/Effects/FallingStream/fallingstream.c): the object effect_newEffectObj returns. */
struct StreamActor {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (particle_getViewBuffer) */
    /* 0x94 */ StreamPoint *records;
    /* 0x98 */ StreamState *state;
};

/* One point of a trail. */
typedef struct FallingTrailPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 velY;
} FallingTrailPoint; /* size 0x10 */

/* A trail object made by effect_newEffectObj: a ring buffer of points falling under gravity. */
typedef struct FallingTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (particle_getViewBuffer) */
    /* 0x94 */ FallingTrailPoint *points;
    /* 0x98 */ TrailBurstParams *data;
} FallingTrail;

/* Data block at 0x28 of the effect trail_spawnLevelEffect spawns. */
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
    /* 0x8 */ u8 preset;  /* index into gTrailBurstPresets */
} TrailBurstState;

/* Timed effect of trail_updateBurstEmitter. */
typedef struct TrailBurstEmitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ TrailBurstState emitter;
} TrailBurstEmitter;

/* Params of a trail spawned by a SprayEmitter: the TrailParams, then the floor height. */
typedef struct {
    /* 0x00 */ TrailParams base;
    /* 0x48 */ f32 floorY;
} SprayTrailParams;

/* A MotionTrail (trail_createMotionTrail) whose params carry SprayTrailParams. */
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
    /* 0x6C */ ZoneNode *zone;  /* from collision_findZoneLeaf at the segment's midpoint */
    /* 0x70 */ u8 flags;
    /* 0x71 */ u8 pad71[7];
} SprayEmitter; /* size 0x78 */

/* Emitter settings, copied into the object from a template by trail_spawnEmitter. */
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

/* A trail object made by effect_newEffectObj, following a matrix of its owner. */
typedef struct MatrixTrail {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuffers[0x10];
    /* 0x94 */ MatrixTrailPoint *points;
    /* 0x98 */ MatrixTrailData *data;
} MatrixTrail;

/* Data of a trail task (effect_newTimedEffectNoDraw), at TrailTask::data. */
typedef struct TrailTaskData {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;
    /* 0x08 */ s32 unk8;  /* spec index for trail_updateStartTask, a frame timer for trail_updateTrailPairTask */
} TrailTaskData;

typedef struct TrailTask {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ TrailTaskData data;
} TrailTask;

/* Event payloads the handlers below get (see effect_sendEventToEffectObjs). */
typedef union TrailEvent {
    ActorRef object;    /* events 0 and 7: trails of this owner stop */
    u8 specIndex;       /* event 6: trails of this kind stop (trail_stopMatrixTrailsOfKind) */
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

/* A character watcher (effect_newCharacterWatcher) whose extra data is the character's two surface trails. */
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
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (particle_getViewBuffer) */
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

/* Effect object created by effect_newEffectObj for a trail. points[] is a ring buffer. */
typedef struct BoneTrailEffect {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (particle_getViewBuffer) */
    /* 0x94 */ BoneTrailPoint *points;
    /* 0x98 */ BoneTrailState *state;
} BoneTrailEffect;

/* Emits particles from a point on one of the owner's bones (task data of trail_startBoneEmitter). */
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

/* A effect_newTimedEffectNoDraw task that retries creating a trail until its owner can carry it. */
typedef struct TrailRetryTask {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoneTrailParams trail;
} TrailRetryTask;

/* An actor a trail follows and its unique_id: argument of a ribbon effect (selector 0) and of message 0. */
typedef struct TrailTargetRef {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetUnk3B;
} TrailTargetRef;

/* The object trail_integrateScalarMotion integrates: a 1-D position under constant acceleration, and a fade. */
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

/* Trail actor (effect_newEffectObj, see game/Effects/Trail/PissTrail/pisstrail.c) drawn as a ribbon through its ring buffer of points. */
typedef struct RibbonActor {
    /* 0x00 */ EffectObjHead head;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 vtxBuffer[0x10];
    /* 0x94 */ RibbonActorPoint *points;
    /* 0x98 */ RibbonInfo *info;
} RibbonActor;

/* Data of the camera-facing strip effect spawned by trail_spawnStripEffect and drawn by trail_drawStripEffect. */
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

/* Partial view of the particle particle_newSpriteParticleWithFloor creates. */
typedef struct CarrierParticle {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ struct WaveTrail *effect; /* wave trail that travels with the particle (trail_spawnWaveTrail) */
} CarrierParticle;

/* Emitter data at 0x28: where the trail is anchored this frame. */
typedef struct {
    /* 0x00 */ struct AnchoredTrail *trail; /* attached trail, or NULL */
    /* 0x04 */ u8 anchorType;       /* index into gAnchorUpdateProcs (update) and gAnchorMessageProcs (message handler) */
    /* 0x05 */ u8 flags;            /* bit 0: pos is valid this frame */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 maxStep; /* moving further than this in one frame detaches the trail */
} TrailAnchor;

/* Emitter data at 0x40 for anchor type 0: a point on an actor's bone. */
typedef struct TrailBoneLink {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId;  /* actor->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 bone;     /* index into gTrailBoneAnchors (offset) and gTrailBoneMatrices (bone matrix) */
    /* 0x06 */ s8 condition; /* index into gAnchorConditionProcs, or -1 for none */
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
    /* 0x84 */ u8 vtxBuf[0x10]; /* per-view vertex buffers (particle_getViewBuffer) */
    /* 0x94 */ AnchoredTrailPoint *points;
    /* 0x98 */ AnchoredTrailData *data;
} AnchoredTrail;

/* game/Effects/Trail/TrailSpray/trailspray.c */
void trail_sprayFromRing(Actor *obj, u8 owner);
s32 trail_fadeMotionTrail(MotionTrail *trail);

/* game/Effects/Trail/ScatterTrail/scattertrail.c */
void func_150B3DD0(void);
void trail_destroyScatterTrail(ScatterTrail *trail);
void trail_freeScatterTrail(ScatterTrail *trail);
ScatterTrail *trail_spawnScatterTrail(TrailStateParams *params, Vec3f *pos, s16 capacity);
s32 trail_ageScatterTrail(ScatterTrail *trail);
s32 trail_emitScatterTrail(ScatterTrail *trail);
Gfx *trail_drawScatterTrail(ScatterTrail *trail, Gfx *gfx, s16 camIdx);
void trail_stopScatterTrail(ScatterTrail *trail);
void trail_burstScatterTrail(ScatterTrail *trail);
void trail_onScatterTrailMessage(ScatterTrail *trail, s32 event, u8 msg);

/* game/Effects/Trail/TrailRing/trailring.c */
void trail_spawnKind2Ring(f32 x, f32 y, f32 z, u8 owner);
s32 trail_spawnKind2Trail(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ, s32 angle, s32 baseAngle, f32 radius, s32 arg11, f32 arg12, void *data, u8 owner);
s32 trail_fadeKind2Trail(MotionTrail *trail);

/* game/Effects/Trail/CharacterTrail/charactertrail.c */
void trail_spawnCharacterTrail(Actor *owner, u8 fxOwner, s32 fxList);
s32 trail_updateCharacterTrail(CharacterTrail *actor);
s32 trail_trimCharacterTrail(CharacterTrail *actor);
Gfx *trail_drawCharacterTrail(CharacterTrail *actor, Gfx *gfx, s16 view);
void trail_onCharacterTrailMessage(CharacterTrail *actor, ActorMsg *msg, u8 id);
s32 trail_getCharacterTrailEnds(Vec3f *start, Vec3f *end, Actor *owner, ModelAttachment *attachment);
void trail_destroyCharacterTrail(CharacterTrail *actor);
void trail_freeCharacterTrail(CharacterTrail *actor);
void trail_stopCharacterTrailFx(CharacterTrail *actor);
s32 trail_updateTrailGlow(TrailGlow *particle);
void trail_throwSparks(Vec3f *pos, const Vec3f *vel, f32 angle, u8 owner, s32 list);
s32 trail_characterTrailSoundCallback(TrailSound *snd, s32 *active, u32 *volumeOut, s32 *pan, s32 *pitchOut, s32 *mix, u16 *playing);

/* game/Effects/Trail/PissTrail/pisstrail.c */
void trail_getPissAnchor(Actor *actor, Vec3f *out);
void trail_getPissLaunchDir(Actor *actor, Vec3f *out);
void trail_spawnPissTrail(Actor *actor);
void trail_stepBallistic(Vec3f *pos, Vec3f *vel, f32 dt);
void trail_endPissTrail(PissTrail *trail);
s32 trail_movePissPoints(PissTrail *trail);
s32 trail_emitPissPoints(PissTrail *trail);
s32 trail_tracePissTrail(PissTrail *trail);
Gfx *trail_drawPissTrail(PissTrail *trail, Gfx *gfx, s16 view);
void trail_stopPissSound(PissTrail *trail);
void trail_destroyPissTrail(PissTrail *trail);
void trail_freePissTrail(PissTrail *trail);
void trail_onPissTrailMessage(PissTrail *trail, TrailMessage *msg, u8 type);
void trail_setPissSpeedScale(Actor *actor, f32 speedScale);
s32 trail_updatePissSpark(SpriteParticle *particle, Vec3f *prevPos);
void trail_spawnPissImpact(PissTrail *trail, RayHit *hit, f32 *dir);

/* game/Effects/Trail/MotionTrail/motiontrail.c */
/* Returns the new MotionTrail; callers whose params extend TrailParams (extraSize) keep it in their own view. */
void *trail_createMotionTrail(EffectTemplate *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 updateFn, s32 finishFn, s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 maxLights, s32 lightMask, TrailRender *render, CollisionHit *hit, u8 owner, s32 list);
s8 trail_updateMotionTrail(MotionTrail *trail);
Gfx *trail_drawMotionTrail(MotionTrail *trail, Gfx *gfx, s16 view);
s32 trail_moveAndAppendPoint(MotionTrail *trail);
s32 trail_movePoints(MotionTrail *trail);
s32 trail_movePointsGroundTest(MotionTrail *trail);
s32 trail_dropNewestPoint(MotionTrail *trail);
s32 trail_dropPointsOnGroundHit(MotionTrail *trail, f32 x, f32 y, f32 z);
void trail_launchMotionTrail(u8 texture, f32 x, f32 y, f32 z, s32 arg4, s32 yaw, s32 pitch, f32 speed, s16 life, f32 halfWidth, f32 gravity, u8 owner);
s32 trail_fadeIn(MotionTrail *trail);
void trail_destroyEffectObj(EffectObj *obj);

/* game/Effects/Trail/FallingTrail/fallingtrail.c */
FallingTrail *trail_spawnFallingTrail(EffectTemplate *header, s32 extraSize, TrailBurstParams *data, CollisionHit *hit, u8 owner, s32 list);
s32 trail_moveFallingTrail(FallingTrail *trail);
s32 trail_fadeFallingTrail(FallingTrail *trail);
Gfx *trail_drawFallingTrail(FallingTrail *trail, Gfx *gfx, s16 view);
s32 trail_growFallingTrail(FallingTrail *trail, TrailBurstParams *data);
s32 trail_shrinkFallingTrail(FallingTrail *trail, TrailBurstParams *data);
LevelEffect *trail_spawnLevelEffect(TriggerZone *obj);
void trail_updateBurstEmitter(TrailBurstEmitter *owner);

/* game/Effects/Trail/Afterimage/afterimage.c */
extern MtxS16 *gBoneTrailMtxs;       /* bone matrices drawn by actor_drawModel mode 4 */
extern u8 gAfterimageAlpha;            /* alpha scale for actor_drawModel mode 4 */
extern BoneTrail *gBoneTrailLists[2]; /* [0]: Actor owners, [1]: world object owners */
void trail_startActorAfterimage(Actor *object, s32 duration, s32 maxDist);
void trail_startObjectAfterimage(WorldObject *owner, s32 duration, s32 maxDist);
void trail_appendBoneTrail(BoneTrail *trail, BoneTrail **list);
void trail_fadeActorAfterimage(Actor *object);
void trail_removeActorAfterimages(Actor *object);
void trail_updateBoneTrails(s32 listIndex);
Gfx *trail_drawBoneTrails(Gfx *gfx, s32 listIndex, s16 player);
void trail_shiftActorHistory(BoneTrail *trail);
void trail_shiftObjectHistory(BoneTrail *arg);
void trail_limitHistorySpacing(s32 listIndex);
void trail_freeActorAfterimage(BoneTrail *trail);
void trail_freeObjectAfterimage(BoneTrail *trail);
Gfx *trail_drawActorAfterimage(Gfx *gfx, BoneTrail *trail, s16 player);
Gfx *trail_drawObjectAfterimage(Gfx *gfx, BoneTrail *trail, s16 player);
void func_151898C0(Actor *object, s32 value);

/* game/Effects/Trail/SprayEmitter/sprayemitter.c */
void trail_createSprayEmitter(SprayDesc *desc, u8 floorSplash);
void trail_updateSprayEmitter(SprayEmitter *emitter);
s32 trail_landSprayTrail(SprayTrail *trail);
s32 trail_ignoreGroundHit(MotionTrail *trail, f32 x, f32 y, f32 z);

/* game/Effects/Trail/TrailEmitter/trailemitter.c */
TrailEmitter *trail_getActorEmitter(Actor *actor, u8 owner, s32 list);
void trail_destroyAllEmitters(Actor *actor);
TrailEmitter *trail_spawnEmitter(TrailEmitterParams *params, u8 owner, s32 list);
void trail_updateEmitter(TrailEmitter *emitter);
s32 trail_updateEmittedTrail(MotionTrail *trail);
s32 trail_fadeEmittedTrail(MotionTrail *trail);
s32 trail_spawnHitPuff(MotionTrail *trail, f32 x, f32 y, f32 z, f32 hitY);
s32 trail_spawnHitPuffAndEffect(MotionTrail *trail, f32 x, f32 y, f32 z, f32 hitY, CollisionTri *surface);
void trail_onEmitterMessage(TrailEmitter *emitter, s32 data, u8 type);

/* game/Effects/Trail/MatrixTrail/matrixtrail.c */
extern TrailTask *gPendingTrailPairTask;
void trail_stopMatrixTrailsOfKind(u8 specIndex);
void trail_stopMatrixTrail(MatrixTrail *trail);
void trail_destroyMatrixTrail(MatrixTrail *trail);
void trail_freeMatrixTrail(MatrixTrail *trail);
MatrixTrail *trail_startMatrixTrail(Actor *owner, s32 specIndex, s16 lifetime, u8 timed, u8 fxOwner, s32 fxList);
s32 trail_ageMatrixTrail(MatrixTrail *trail);
s32 trail_followMatrixTrail(MatrixTrail *trail);
Gfx *trail_drawMatrixTrail(MatrixTrail *trail, Gfx *gfx, s16 view);
void trail_onMatrixTrailEvent(MatrixTrail *trail, TrailEvent *event, u8 eventId);
s32 trail_getAttachPoint(MatrixTrailData *data, Vec3f *out, f32 x, f32 y, f32 z, f32 t);
void trail_restartTrailPair(Actor *owner, s16 lifetime, u8 fxOwner, s32 fxList);
void trail_updateStartTask(TrailTask *task);
void trail_onTaskEvent(TrailTask *task, TrailEvent *event, u8 eventId);
void trail_destroyOnEndEvent(EffectHeader *task, TrailEvent *event, u8 eventId);
void trail_stopTrailPair(s32 unused);
void trail_cancelTrailPair(void);
void trail_queueTrailPair(Actor *owner);
void trail_updateTrailPairTask(TrailTask *task);
void trail_onTrailPairTaskEvent(TrailTask *task, TrailEvent *event, u8 eventId);
void trail_destroyTask(TrailTask *task);
void trail_freeTask(TrailTask *task);
void trail_destroyTrailPairTask(TimedEffect *task);
void trail_freeTrailPairTask(TimedEffect *task);

/* game/Effects/Trail/SurfaceTrail/surfacetrail.c */
void trail_destroySurfaceTrail(SurfaceTrail *trail);
void trail_freeSurfaceTrail(SurfaceTrail *trail);
void trail_startSurfaceTrails(SurfaceTrailOwner *owner);
/* (SurfaceTrailOwner *owner); declared without a prototype: effect_watcherReleaseSurfaceTrails1 and effect_watcherReleaseSurfaceTrails2 pass no argument
   (the owner is still in a0) */
void trail_releaseSurfaceTrails();
void trail_detachSurfaceTrail(SurfaceTrail *trail);
s32 trail_ageSurfaceTrail(SurfaceTrail *trail);
s32 trail_followSurfaceTrail(SurfaceTrail *trail);
Gfx *trail_drawSurfaceTrail(SurfaceTrail *trail, Gfx *gfx, s16 view);
void trail_updateSurfaceSpawner(TrailSpawner *spawner);
void trail_unlinkSurfaceSpawner(TrailSpawner *spawner);
void trail_onSurfaceTrailEvent(SurfaceTrail *trail, ActorRef *event, u8 type);
void trail_onSurfaceSpawnerEvent(TrailSpawner *spawner, ActorRef *event, u8 type);

/* game/Effects/Trail/BoneTrail/bonetrail.c */
void trail_spawnActorBoneTrail(Actor *owner, u8 fxOwner);
BoneTrailEffect *trail_createBoneTrailEffect(BoneTrailParams *params, u8 owner);
s32 trail_ageBoneTrail(BoneTrailEffect *effect);
s32 trail_followBoneTrail(BoneTrailEffect *effect);
Gfx *trail_drawBoneTrail(BoneTrailEffect *effect, Gfx *gfx, s16 view);
void trail_startBloodDrip(Actor *owner, u8 fxOwner);
void trail_emitBurst(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, EffectHeader *parent);
s32 trail_updateBurstParticle(SpriteParticle *particle, Vec3f *prevPos);
s32 trail_updateBurstParticleTint(SpriteParticle *particle, Vec3f *prevPos);
void trail_startBoneEmitter(Actor *owner, u8 fxOwner);
void trail_updateBoneEmitter(BoneEmitterTask *task);
void trail_onBoneTrailMessage(BoneTrailEffect *effect, ActorMsg *msg, u8 type);
void trail_onBoneEmitterMessage(BoneEmitterTask *task, ActorMsg *msg, u8 type);
s32 trail_getBoneAttachPoint(BoneTrailParams *params, Vec3f *out);
void trail_detachBoneTrail(BoneTrailEffect *effect);
void trail_updateRetryTask(TrailRetryTask *task);
void trail_onRetryTaskMessage(TrailRetryTask *task, ActorMsg *msg, u8 type);

/* game/Effects/Trail/RibbonTrail/ribbontrail.c */
/* Particle a ribbon effect drags along (see trail_moveRibbon). */
typedef struct LinkedParticle {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ Vec3f pos;
} LinkedParticle;

/* State block of a ribbon effect (trail_spawnRibbon). A copy of its arguments follows at +0x10. */
typedef struct RibbonState {
    /* 0x00 */ LinkedParticle *linked;
    /* 0x04 */ void *args;
    /* 0x08 */ u8 selector; /* index into gRibbonSourceProcs and gRibbonMessageProcs */
    /* 0x09 */ u8 pad9[3];
} RibbonState;

void trail_spawnFollowTrail(Actor *target, u8 owner, s32 list);
s32 trail_ageFollowTrail(EffectObj *obj);
s32 trail_followTarget(EffectObj *obj);
Gfx *trail_drawFollowTrail(EffectObj *obj, Gfx *gfx, s16 view);
void trail_onFollowTrailMessage(EffectObj *obj, void *msg, u8 msgType);
void trail_spawnParticleAtTarget(Actor *target, u8 owner, s32 list);
EffectObj *trail_spawnRibbon(void *args, u8 selector, s32 argsSize, u8 owner, s32 list);
s32 trail_getRibbonActorPos(EffectObj *obj, Vec3f *pos);
s32 trail_getRibbonPointBehind(EffectObj *obj, Vec3f *pos);
s32 trail_moveRibbon(EffectObj *obj);
s32 trail_shapeRibbon(EffectObj *obj);
Gfx *trail_drawRibbon(EffectObj *obj, Gfx *gfx, s16 view);
void trail_onRibbonMessage(EffectObj *obj, void *msg, u8 msgType);
void trail_onActorRibbonMessage(EffectObj *obj, void *msg, u8 msgType);

/* game/Effects/Trail/LevelStream/levelstream.c */
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
    f32 spawnTc; /* tc where the last particle was spawned */
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

StreamTask *trail_createStreamTask(TriggerZone *source, u8 owner, s32 list);
void trail_startStream(s32 id);
void trail_stopStream(s32 id);
void trail_onStreamTaskEvent(StreamTask *task, s32 *id, u8 event);
/* Unprototyped: trail_createStreamEmitter is a K&R definition (callers pass its u8 arguments unnarrowed). */
LevelStream *trail_createStreamEmitter();
s32 trail_moveStreamParticles(LevelStream *emitter);
s32 trail_spawnStreamParticles(LevelStream *emitter);
s32 trail_fadeStreamEnds(LevelStream *emitter);
Gfx *trail_drawStream(LevelStream *emitter, Gfx *gfx, s16 view);
void trail_stopStreamEmitter(LevelStream *emitter);
void trail_destroyStreamEmitter(LevelStream *emitter);
void trail_freeStreamEmitter(LevelStream *emitter);
void trail_testStreamContact(Actor *actor, TriggerZone *source);
LevelStream *trail_findStream(s32 id);

/* Data of the random-point emitter (a TimedEffect's data). */
typedef struct Game1FBB80Effect {
    /* 0x00 */ TriggerZone *shape; /* the zone whose area the packets start in (math_randomZoneFootprintPoint) */
    /* 0x04 */ f32 rate;
    /* 0x08 */ f32 accumulator;
    /* 0x0C */ ZoneNode *zone;     /* the emitter's zone: it emits only while a view can see it */
    /* 0x10 */ CollisionHit floor; /* the last floor probe (collision_probeCeilingSet0Cached) under a packet */
} Game1FBB80Effect;

typedef struct Game1FBB80Object {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Game1FBB80Effect effect;
} Game1FBB80Object;

void trail_updateRandomPointEmitter(Game1FBB80Object *object);
s32 trail_integrateScalarMotion(TrailScalarMotion *motion);
WaveTrail *trail_spawnWaveTrail(Actor *owner, DescParticleHead *anchor, u8 smooth, u8 fxOwner, s32 fxList);
s32 trail_getWaveTrailAnchor(Vec3f *out, Actor *owner, DescParticleHead *anchor);
s32 trail_followWaveTrail(WaveTrail *trail);
s32 trail_shapeWaveTrail(WaveTrail *actor);
Gfx *trail_drawWaveRibbon(RibbonActor *ribbon, Gfx *gfx, s16 cameraIdx);
void trail_onFollowerMessage(TrackerTrail *actor, s32 event, u8 eventType);
void trail_spawnNearCamera(Actor *obj, f32 probeY, f32 bottom);
void trail_breakApart(Actor *obj, u8 owner, s32 list);
void trail_notifyFollowersGone(Actor *obj);
void trail_spawnStripEffect(Actor *obj, u8 variant, u8 owner, s32 list);
void trail_updateStripEffect(StripEffect *effect);
Gfx *trail_drawStripEffect(Gfx *gfx, StripEffect *effect, s16 cameraIdx);
void trail_onOwnedEffectMessage(OwnedEffect *effect, void *msg, u8 type);
void trail_emitLineBursts(Actor *obj, u8 owner, s32 list);
void trail_freeCarriedEffect(CarrierParticle *particle);
void trail_destroyCarrierParticle(CarrierParticle *particle);
void trail_freeCarrierParticle(CarrierParticle *particle);
void trail_spawnTrackerEffect(Actor *obj, u8 pointFn, u8 owner, s32 list);
s32 trail_getPointAboveFloor(Actor *obj, Vec3f *out);
s32 trail_getModelPoint0(Actor *obj, Vec3f *out);
s32 trail_getModelPoint1(Actor *obj, Vec3f *out);
s32 trail_getModelPoint(Actor *obj, Vec3f *out, u8 which);
void trail_updateTrackerEffect(TrackerEffect *effect);
void trail_onTrackerMessage(TrackerEffect *effect, s32 event, u8 eventType);
void trail_releaseTracker(TrackerEffect *effect);
void trail_destroyTracker(TrackerEffect *effect);
void trail_freeTracker(TrackerEffect *effect);
void trail_dropTrackerTrail(TrackerEffect *effect);
void trail_spawnTrackerTrail(TrackerEffect *effect);

/* game_data_800D9860.c (data only, owner unknown; linked after game/Script/ScriptInterp/scriptinterp) */
extern CollisionHit D_800D9860; /* the floor under the last spawn point of levelstream.c; a zone trigger of
                                 * game/Level/ZoneSpawn/zonespawn.c clears its flags */

/* game/Effects/Trail/TrackerTrail/trackertrail.c */
s32 trail_sampleTrackerTrail(TrackerTrail *actor);
s32 trail_shapeTrackerTrail(TrackerTrail *actor);
Gfx *trail_drawTrackerTrail(TrackerTrail *actor, Gfx *gfx, s16 view);
void trail_detachTrackerTrail(TrackerTrail *actor);
void trail_destroyTrackerTrail(TrackerTrail *actor);
void trail_freeTrackerTrail(TrackerTrail *actor);

/* game/Effects/Trail/AnchoredTrail/anchoredtrail.c */
TimedEffect *trail_createAnchoredEmitter(s16 timer, u8 flags, u8 anchorType, f32 maxStep, s32 extraSize, u8 owner, s32 list);
void trail_updateAnchoredEmitter(AnchoredTrailEmitter *emitter);
void trail_onAnchoredEmitterMessage(AnchoredTrailEmitter *emitter, TrailBoneLink *msg, u8 msgType);
void trail_releaseAnchoredEmitter(AnchoredTrailEmitter *emitter);
void trail_destroyAnchoredEmitter(AnchoredTrailEmitter *emitter);
void trail_freeAnchoredEmitter(AnchoredTrailEmitter *emitter);
void trail_broadcastActorGone(Actor *actor);
void trail_attachToBone(Actor *actor, u8 bone, s8 condition, u8 owner, s32 list);
void trail_onBoneAnchorMessage(AnchoredTrailEmitter *emitter, TrailBoneLink *msg, u8 msgType);
s32 trail_updateBoneAnchor(AnchoredTrailEmitter *emitter);
s32 func_151D7724(AnchoredTrailEmitter *emitter);
s32 func_151D7770(AnchoredTrailEmitter *emitter);
s32 trail_checkAnchorInWater(AnchoredTrailEmitter *emitter);
void trail_detachAnchoredTrail(AnchoredTrailEmitter *emitter);
void trail_spawnAnchoredTrail(AnchoredTrailEmitter *emitter);
s32 trail_fallAnchoredTrail(AnchoredTrail *trail);
s32 trail_followAnchoredTrail(AnchoredTrail *trail);
s32 trail_shapeAnchoredTrail(AnchoredTrail *trail);
Gfx *trail_drawAnchoredTrail(AnchoredTrail *trail, Gfx *gfx, s16 view);
void trail_fallPoint(Vec3f *pos, f32 *velY, f32 dt);
void trail_detachFromEmitter(AnchoredTrail *trail);
void trail_destroyAnchoredTrail(AnchoredTrail *trail);
void trail_freeAnchoredTrail(AnchoredTrail *trail);

#endif
