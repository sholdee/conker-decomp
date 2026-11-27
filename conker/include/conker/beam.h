#ifndef CONKER_BEAM_H
#define CONKER_BEAM_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "beam": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Beam/Strip/strip.c
 *   game/Beam/BeamQuad/beamquad.c
 *   game/Beam/Tracer/tracer.c
 *   game/Beam/AimedBeam/aimedbeam.c
 *   game_11EEB0.c
 *   game/Beam/ModelBeam/modelbeam.c
 *   game/Beam/CastBeam/castbeam.c
 *   game/Beam/BoltRain/boltrain.c
 *   game/Beam/BoltChain/boltchain.c
 *   game/Beam/BeamMotion/beammotion.c
 *   game/Beam/Lightning/lightning.c
 *   game/Beam/AreaEmitter/areaemitter.c
 *   game/Beam/WavyBeam/wavybeam.c
 */

/* Lightning bolts (game/Beam/Lightning/lightning.c, game/Beam/AreaEmitter/areaemitter.c). */
typedef struct {
    u8 r, g, b, a;
} BoltColor;

/* Spawn parameters of a lightning bolt; beam_newLightning copies them into Bolt.def. */
typedef struct {
    /* 0x00 */ s16 lifetime;         /* frames, counted down while BOLT_TIMED */
    /* 0x02 */ s16 numPoints;
    /* 0x04 */ f32 jitterDelay;      /* re-jitter after jitterDelay + rand * jitterDelayRange */
    /* 0x08 */ f32 jitterDelayRange;
    /* 0x0C */ u8 flags;             /* BOLT_TIMED | BOLT_ACTIVE */
    /* 0x10 */ f32 jitter;           /* largest sideways displacement of a midpoint */
    /* 0x14 */ BoltColor color;
    /* 0x18 */ f32 width;
    /* 0x1C */ s8 update1;           /* index into gLightningUpdateHooks, -1 for none */
    /* 0x1D */ s8 update2;           /* index into gLightningUpdate2Hooks, -1 for none */
    /* 0x20 */ Vec3f start;
    /* 0x2C */ Vec3f end;
    /* 0x38 */ Vec3f up;
    /* 0x44 */ f32 length;
    /* 0x48 */ f32 invLength;
    /* 0x4C */ u8 type;              /* index into the gLightningDestroyHooks/gLightningFreeHooks/gLightningMsgHandlers handlers */
} BoltDef; /* size 0x50 */

#define BOLT_TIMED  1
#define BOLT_ACTIVE 2

typedef struct {
    Vec3f local; /* x: sideways, y: along up, z: fraction of the way from start to end */
    Vec3f world;
} BoltPoint;

/* User data of a bolt that runs from an actor bone to a point near the actor (beam_spawnBoneLightning). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;           /* owner->unique_id at spawn; the bolt stops when it changes */
    /* 0x08 */ Vec3f boneOffset;
    /* 0x14 */ u8 bone;
    /* 0x18 */ Vec3f targetOffset;   /* end point, relative to the owner's position */
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 linkFlags;         /* 1: the end point has been ray-cast */
} BoltLink; /* size 0x28 */

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ BoltDef def;
    /* 0x60 */ void *link;           /* user data, stored after the points: a BoltLink, or a SparkLink
                                      * for a beam's sparks (game/Beam/AimedBeam/aimedbeam.c) */
    /* 0x64 */ BoltPoint *points;
    /* 0x68 */ f32 jitterTimer;
    /* 0x6C */ u8 vtxBuf[0x10];
    /* 0x7C */ s32 vtxBufSize;
    /* 0x80 */ BoltPoint pointData[1];
} Bolt;

/* Spawn parameters; beam_newStrip copies them into the object at +0x18. */
typedef struct StripDesc {
    /* 0x00 */ Vec3f p0;
    /* 0x0C */ Vec3f p1;
    /* 0x18 */ s8 updateIdx;
    /* 0x19 */ u8 pad19[3];
    /* 0x1C */ f32 texScale;
    /* 0x20 */ u8 kind;
    /* 0x21 */ u8 pad21[3];
} StripDesc; // size 0x24

/* Character the strip end follows; stored right after the object header. */
typedef struct StripTarget {
    /* 0x00 */ Actor *obj;
    /* 0x04 */ u8 uniqueId;     // obj->unique_id when attached
    /* 0x05 */ u8 bone;         // matrix index into obj->ob_matrix_ptr
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ Vec3f offset;    // bone-local position
} StripTarget;

/* A camera-facing textured strip drawn from p0 to p1 (object type 0x46). */
typedef struct StripObj {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ Vec3f p0;
    /* 0x24 */ Vec3f p1;
    /* 0x30 */ s8 updateIdx;    // index into gStripUpdateHandlers, -1 for none
    /* 0x31 */ u8 pad31[3];
    /* 0x34 */ f32 texScale;    // v texcoord = length * texScale * 64
    /* 0x38 */ u8 kind;         // index into gStripDestroyHandlers / gStripFreeHandlers / gStripMsgHandlers
    /* 0x39 */ u8 pad39[7];
    /* 0x40 */ Vtx quad[2][4];  // double-buffered by gDrawBufferIndex
    /* 0xC0 */ f32 length;      // |p0 - p1|
    /* 0xC4 */ u8 padC4[4];
    /* 0xC8 */ StripTarget target;
} StripObj;

/* Self-propelled beam quad (beam_spawnBoneQuad / beam_updateQuad). */
typedef struct {
    Vec3f vel;
    f32 size;
    f32 sizeRate;
    f32 scaleX;     /* pulses between 0.8 and 1.2 */
    f32 scaleY;
    f32 scaleXRate;
    f32 scaleYRate;
    f32 damping;
    u8  fadeRate;
} BeamMotion;

/* Quad that follows externally owned values (beam_spawnLinkedQuad / beam_updateLinkedQuad / beam_drawLinkedQuad). */
typedef struct {
    f32 *pos;
    f32 *angle;     /* degrees */
    f32 *halfWidth;
    f32 *halfHeight;
    f32 *alpha;
} LinkedQuad;

/* Quad particle created by particle_newQuadParticleWhite (a DefParticleHead view) with this file's motion after it. */
typedef struct {
    u8  pad0[0x2C];
    f32 halfWidth;
    f32 halfHeight;
    Vec3f pos;
    Vec3f dir;
    u8  pad4C[0x10];
    u8  alpha;
    u8  pad5D[0x63];
    Vtx_t template[4];
    Vtx_t *buffers[4];  /* each holds two copies of the 4-vertex quad */
    union {
        BeamMotion beam;
        LinkedQuad linked;
    } u;
} BeamQuad;

/* A tracer/beam effect: a textured quad stretched from its position along its direction. */
typedef struct {
    EffectHeader header;
    Vtx_t quads[2][4]; // double-buffered by gDrawBufferIndex
    s16 lifetime;
    u8 pad92[2];
    f32 x, y, z;
    f32 pitch, yaw;
    f32 speed;    // distance per frame; 0 means random
    f32 stopDist; // nonzero: stop on reaching the target
    f32 target_x, target_y, target_z;
    f32 halfWidth;
    f32 length;
    f32 dx, dy, dz; // unit direction
    s8 mode;        // nonzero: re-fired from a bone of the owner
    u8 padD1;
    s16 onTime;
    s16 offTime;
    s16 timer;      // drawn while positive
    s16 texture;    // index into gEffectAnimTextures
    u8 ownerId;     // owner's unique_id when attached
    u8 drawn;
    Actor *owner;
    s16 ownerTag;   // compared with owner->unk84.uh; -1 for none
    u8 bone;
    u8 attachDelay;
    f32 startOffset[3]; // in bone space
    f32 endOffset[3];
    u8 impactEffect;
    u8 fireCounter;
    u8 unkFE;
    u8 padFF;
} Game10EF60Effect;

/* Message 0x43 payload: an actor reference plus the actor's effect slot. */
typedef struct {
    /* 0x0 */ Actor *actor;
    /* 0x4 */ u8 uniqueId;
    /* 0x5 */ u8 slot;
} ActorSlotMsg;

/* Payload of the messages a beam handles (0 and 0x43: ActorSlotMsg, 0x2D: an actor swap). */
typedef union {
    ActorSlotMsg ref;
    ActorSwapMsg swap;
} BeamMsg;

/* State a beam object carries at +0x110. The beam runs from a bone of `actor` to `target`. */
typedef struct {
    /* 0x00 */ Actor *actor;       /* the shooter */
    /* 0x04 */ u8 actorId;             /* actor->unique_id at spawn */
    /* 0x08 */ Actor *target;
    /* 0x0C */ u8 targetId;
    /* 0x0D */ u8 slot;                /* 0 or 1: picks the source bone and offset */
    /* 0x10 */ f32 widthPhase;
    /* 0x14 */ f32 alphaPhase;
    /* 0x18 */ u8 flags;               /* bit 0: the beam is aimed this frame */
    /* 0x1C */ Bolt *sparks[2];         /* bolts carrying a SparkLink */
    /* 0x24 */ struct BeamEmitter *emitter;
    /* 0x28 */ struct Glow *glow;
    /* 0x2C */ LightEffect *flash;         /* muzzle flash; its payload (data.object) is the beam */
    /* 0x30 */ Vec3f direction;
    /* 0x3C */ f32 distance;
    /* 0x40 */ f32 inverse;
} AimedBeamState; /* 0x44 */

typedef struct AimedBeam {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ AimedBeamState link;
} AimedBeam;

/* Back-reference a beam's spark carries (at spark->link). */
typedef struct {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId;
    /* 0x08 */ AimedBeam *beam;
    /* 0x0C */ u8 index;               /* which beam->link.sparks[] entry */
} SparkLink; /* 0x10 */

/* Back-reference a beam's emitter carries at +0x28. */
typedef struct {
    /* 0x00 */ AimedBeam *beam;
    /* 0x04 */ f32 accum;              /* particles owed, spawned one per 1.0 */
} EmitterLink; /* 0x8 */

/* A beam's emitter (created by effect_newTimedEffect), carrying its EmitterLink at +0x28. */
typedef struct BeamEmitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ EmitterLink link;
} BeamEmitter;


/* Back-reference a beam's glow carries at +0x170. */
typedef struct {
    Actor *actor;
    u8 actorId;
    u8 slot;
    AimedBeam *beam;
} GlowLink;

typedef struct Glow {
    /* 0x000 */ DefParticleHead head;   /* head.def.flags bits 1-2: drawn */
    /* 0x110 */ GlowShape shape;
    /* 0x16C */ u8 pad16C[4];
    /* 0x170 */ GlowLink link;
} Glow;

/* State an actor-following particle (created by particle_newSpriteParticleWithFloor) carries at +0xA8. */
typedef struct {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId;
    /* 0x08 */ f32 phase[3];
} ActorParticleLink;

/* State a trail particle (see beam_updateTrailEmitter) carries at +0xA8. */
typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2;
    s8 unk3;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} TrailParticleLink;

typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ union {
        ActorParticleLink actor;
        TrailParticleLink trail;
    } link;
} AttachedSpriteParticle;

/* State an actor's trail emitter (created by effect_newTimedEffect) carries at +0x28. */
typedef struct {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId;
    /* 0x08 */ f32 accum;              /* particles owed, spawned one per 1.0 */
    /* 0x0C */ Vec3f lastPos;          /* actor position on the previous frame */
} ActorTrailLink; /* 0x18 */

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ ActorTrailLink link;
} ActorTrailEmitter;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x8];
    /* 0x18 */ ActorRef ref;
} ActorBound;

/* Data of the spark effect spawned by beam_spawnBoneSparks and run by beam_updateBoneSparks (sparks along a
 * segment of the owner's bone 5). */
typedef struct {
    Actor *owner;
    u8 ownerId; /* owner->unique_id at spawn; a mismatch means the owner died */
    u8 pad5[3];
    f32 accum;  /* sparks owed; one is spawned per whole unit */
} SparkData;

typedef struct {
    TimedEffect base;
    SparkData data;
} BoneSparkEffect;

typedef struct ModelBeamEffect ModelBeamEffect;

/* The beam particle's data (at +0x110), set by level_spawnModelBeam (game/Level/LevelSetup13/levelsetup13.c). The
 * particle runs effect_updateBurstSpark and effect_buildBurstSparkVtx, so the fields up to beamLengthTarget are
 * BurstSparkData's. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;         /* owner->unique_id at spawn */
    /* 0x05 */ u8 pad05[3];
    /* 0x08 */ f32 widthBase;
    /* 0x0C */ f32 heightBase;
    /* 0x10 */ f32 widthRange;
    /* 0x14 */ f32 heightRange;
    /* 0x18 */ f32 widthTarget;
    /* 0x1C */ f32 heightTarget;
    /* 0x20 */ f32 heightRange2;
    /* 0x24 */ f32 widthTimer;
    /* 0x28 */ f32 heightTimer;
    /* 0x2C */ f32 alphaBase;
    /* 0x30 */ f32 alphaRange;
    /* 0x34 */ f32 alphaTarget;
    /* 0x38 */ f32 alphaTimer;
    /* 0x3C */ f32 alpha;
    /* 0x40 */ f32 beamWidth;
    /* 0x44 */ f32 beamLength;
    /* 0x48 */ f32 beamWidthTarget;  /* beamWidth and beamLength ease towards these */
    /* 0x4C */ f32 beamLengthTarget;
    /* 0x50 */ ModelBeamEffect *effect; /* the beam effect that drives this beam */
    /* 0x54 */ f32 scale;            /* the owner's average scale */
    /* 0x58 */ u8 flags; /* bit 0 = shown */
} BeamPart; /* size 0x5C */

/* Something drawn between two points (a beam or streak). */
typedef struct {
    DefParticleHead head; /* head.def.pos / rot: the two end points */
    BeamPart part;
} ModelBeam;

/* ModelBeamEffect.data: the owner, and the beam and flash it drives. */
typedef struct {
    Actor *owner;
    u8 ownerId;
    u8 pad5[3];
    ModelBeam *beam;
    LightEffect *flash; /* created by light_createWithEffect */
} ModelBeamData;

struct ModelBeamEffect {
    TimedEffect base;
    ModelBeamData data;
};

/* A sprite spawned by particle_newSpriteParticleWithFloor that marks one end of the beam. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x30];
    /* 0x40 */ Vec3f pos;
    /* 0x4C */ u8 pad4C[0x74 - 0x4C];
    /* 0x74 */ s8 mode; /* 3 or -1 (see beam_updateCastBeam) */
} BeamEnd;

/* Per-object state of a beam (0x78 bytes, copied in by beam_spawnCastBeam). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id */
    /* 0x08 */ BeamEnd *ends[2];
    /* 0x10 */ RayHit ray;
    /* 0x74 */ u8 flags;
} BeamState;

/* A beam object allocated by effect_newTimedEffect. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BeamState beam;
} BeamObj;

typedef struct RayBeam RayBeam;

/* How a child object refers back to its beam. */
typedef struct {
    RayBeam *beam;
    u8 flags; // 1 = visible
} RayBeamLink;

/* A 2D sine wobble: offset = base + amplitude * sin(phase), with phase advancing at speed (particle_updateWobbleOffset). */
typedef struct {
    /* 0x00 */ f32 offset[2];
    /* 0x08 */ f32 base[2];
    /* 0x10 */ f32 amplitude[2];
    /* 0x18 */ f32 phase[2];
    /* 0x20 */ f32 speed[2];
} Wobble151025F4; /* size 0x28 */

/* beam->data.startFx: placed at the beam's start, sized by the beam's width times its wobble offset. */
typedef struct {
    /* 0x00 */ DescParticleHead head; /* a sprite particle */
    /* 0xA8 */ Wobble151025F4 wobble;
    /* 0xD0 */ RayBeamLink link;
} BeamStartFx;

typedef struct {
    u8 flags; // 1 = visible
    u8 pad1;
    CollisionTri hit;
    RayBeam *beam;
} BeamHitFxData;

/* beam->data.hitFx: placed just short of the hit point (head.def.pos) on type 1 hits. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ BeamHitFxData data;
} BeamHitFx;

/* beam->data.line: spans the beam from head.def.pos to head.def.rot (its end point). */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ u8 pad110[0x60];
    /* 0x170 */ RayBeamLink link;
} BeamLine;

/* beam->data.endFx: placed at the beam's end on type 2 and 3 hits. */
typedef struct {
    /* 0x00 */ DescParticleHead head; /* a sprite particle */
    /* 0xA8 */ RayBeamLink link;
} BeamEndFx;

/* RayBeam.data, filled by particle_spawnRayBeam; particle_flickerRayBeam eases the width, texture velocity and alpha. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; // must match owner->unique_id
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ Vec3f localStart;  // in bone space
    /* 0x14 */ Vec3f localTarget; // in bone space
    /* 0x20 */ u8 bone;
    /* 0x21 */ s8 ownerSlot;   // owner's index in movement_obj
    /* 0x22 */ u8 ownerCamera; // owner->CameraPtr->CNumber, 0xFF without a camera; the beam is hidden in that view
    /* 0x23 */ u8 flags; // 1 = drawn, 2 = aimed by the owner
    /* 0x24 */ BeamStartFx *startFx;
    /* 0x28 */ BeamHitFx *hitFx;
    /* 0x2C */ LightEffect *endPoint; // light at the beam's end; its payload (data.object) is the RayBeam
    /* 0x30 */ BeamLine *line;
    /* 0x34 */ BeamEndFx *endFx;
    /* 0x38 */ f32 width;
    /* 0x3C */ f32 widthTarget;
    /* 0x40 */ f32 widthBlend;
    /* 0x44 */ f32 length;
    /* 0x48 */ f32 scrollBase;  // texture scroll speed = scrollBase + scrollRange * sin(scrollPhase)
    /* 0x4C */ f32 scrollRange;
    /* 0x50 */ f32 scrollPhase;
    /* 0x54 */ f32 scrollPhaseRate;
    /* 0x58 */ f32 texOffset;
    /* 0x5C */ f32 texVelocity; // added to texOffset per unit of gFrameDelta
    /* 0x60 */ f32 texVelocityTarget;
    /* 0x64 */ f32 texVelocityRange;
    /* 0x68 */ f32 texVelocityTimer;
    /* 0x6C */ f32 texVelocityTimerRange;
    /* 0x70 */ f32 texVelocityBlend;
    /* 0x74 */ f32 alpha;
    /* 0x78 */ f32 alphaTarget;
    /* 0x7C */ f32 alphaBase;
    /* 0x80 */ f32 alphaRange;
    /* 0x84 */ f32 alphaTimer;
    /* 0x88 */ f32 alphaTimerRange;
    /* 0x8C */ f32 alphaBlend;
    /* 0x90 */ s16 sound; // sound_play handle
    /* 0x92 */ u8 pad92[2];
} RayBeamData; /* size 0x94 */

/* A beam that runs from a bone of its owner to the aim target or the first collision on the way
 * (placed by particle_updateRayBeam, drawn by particle_buildRayBeamQuad). */
struct RayBeam {
    /* 0x000 */ DefParticleHead head; /* head.def.pos: the start, head.def.rot: the end point */
    /* 0x110 */ RayBeamData data;
};

/* Rain of lightning bolts falling around a point onto the ground (beam_updateBoltRain, beam_boltRainMessage). */
typedef struct BoltRainState {
    /* 0x00 */ TriggerZone *region;
    /* 0x04 */ f32 pending;      /* bolts owed, accumulated per frame */
    /* 0x08 */ s16 timer;        /* frames left; negative when off */
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ Vec3f pos;
    /* 0x18 */ f32 radius;
    /* 0x1C */ f32 height;       /* bolts start this far above pos */
    /* 0x20 */ CollisionHit ground; /* where the next bolt lands */
    /* 0x44 */ ZoneNode *zone;
} BoltRainState;

typedef struct BoltRainObj {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoltRainState state;
} BoltRainObj;

/* The surface hit an impact effect object was spawned for. */
typedef struct ImpactHit {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ Vec3f position;
    /* 0x3C */ u8 pad3C[0x24];
    /* 0x60 */ Vec3f velocity;
    /* 0x6C */ CollisionTri triangle;
    /* 0x7E */ u8 pad7E[0xA];
    /* 0x88 */ s32 surfaceFlags;   /* bits 0-4: surface type */
} ImpactHit;

typedef struct ImpactObj {
    /* 0x00 */ TimedEffect base;
    /* 0x028 */ u8 pad28[0xE8];
    /* 0x110 */ ImpactHit hit;
} ImpactObj;

/* One point of the Bezier sampled at parameter t, with a frame for the sideways jitter. */
typedef struct BoltSample {
    /* 0x00 */ f32 t;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f tangent;
    /* 0x1C */ Vec3f normal;
    /* 0x28 */ Vec3f binormal;
} BoltSample; /* size 0x34 */

typedef struct BoltFlare {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x30];
    /* 0x40 */ Vec3f pos;
} BoltFlare;

/*
 * A bolt object. It is followed in memory by desc.segments BoltSamples (beam_getBoltSamples),
 * desc.segments + 2 jittered joint points (beam_getBoltJoints) and the per-kind user data.
 */
typedef struct BoltObj {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ BoltDesc desc;
    /* 0x60 */ void *userData;
    /* 0x64 */ f32 step;           /* Bezier parameter step, 1 / (segments + 1) */
    /* 0x68 */ s16 rejitterTimer;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ BoltFlare *flares[2];
    /* 0x74 */ u8 vtxBuf[0x10];
    /* 0x84 */ s32 vtxBufSize;
} BoltObj; /* size 0x88 */

/* A chain of bolts along a randomly spaced line of points, one bolt (link) at a time. */
typedef struct BoltChain {
    /* 0x00 */ s32 count;          /* number of points */
    /* 0x04 */ s32 index;          /* point where the next link starts */
    /* 0x08 */ BoltObj *link;      /* the live link, NULL once it is gone */
    /* 0x0C */ Vec3f *points;
    /* 0x10 */ BoltChainConfig config;
    /* 0x40 */ u8 kind;            /* index into gBoltChainMsgHandlers */
    /* 0x41 */ u8 pad41[3];
} BoltChain; /* size 0x44 */

typedef struct BoltChainObj {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoltChain chain;
    /* 0x6C */ u8 pad6C[4];
    /* 0x70 */ Vec3f points[1];    /* chain.count points */
} BoltChainObj;

typedef struct Game1BBFC0Motion {
    /* 0x00 */ u8 unknown0[0x30];
    /* 0x30 */ Vec3f velocity;
    /* 0x3C */ f32 gravity;
    /* 0x40 */ f32 drag;
    /* 0x44 */ f32 shrinkRate;
    /* 0x48 */ s16 fadeStart;
    /* 0x4A */ s16 fadeRate;
    /* 0x4C */ f32 blend;
} Game1BBFC0Motion;

typedef struct Game1BBFC0Beam {
    /* 0x000 */ u8 unknown0[0x1C];
    /* 0x01C */ s16 lifetime;
    /* 0x01E */ u8 unknown1E[0xE];
    /* 0x02C */ f32 width;
    /* 0x030 */ f32 progress;
    /* 0x034 */ Vec3f start;
    /* 0x040 */ Vec3f end;
    /* 0x04C */ u8 unknown4C[0x10];
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 unknown5D[0x63];
    /* 0x0C0 */ Vtx vertexTemplate[4]; /* copied into a camera buffer when it is fresh */
    /* 0x100 */ Vtx *buffers[1]; /* one per camera, indexed by camera id */
    /* 0x104 */ u8 unknown104[0xC];
    /* 0x110 */ Game1BBFC0Motion motion;
} Game1BBFC0Beam;

typedef struct {
    s16 base;
    s16 range;
} RangeS16; /* base + rand % (range + 1) */

/* User data of a type 2 emitter: the particles spawn hook 1 (beam_launchJitterParticle) launches. */
typedef struct {
    /* 0x00 */ f32 vec0Base;         /* the two floats passed to particle_spawnJitterParticle: base + rand * range */
    /* 0x04 */ f32 vec1Base;
    /* 0x08 */ f32 vec0Range;
    /* 0x0C */ f32 vec1Range;
    /* 0x10 */ RangeS16 life;        /* the particle's life (particle_spawnJitterParticle) */
    /* 0x14 */ RangeS16 relaunchLifetime; /* goes into the particle's extra data (JitterRelaunch.lifetime) */
    /* 0x18 */ RangeS16 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ f32 unk20Base;        /* goes into the particle's extra data */
    /* 0x24 */ f32 unk20Range;
    /* 0x28 */ u8 flags;             /* 1: particle kind 0x71 (else 0x13 or 0x14); 2: clear the 2 flag */
} ParticleSettings; /* size 0x2C */

/* Effect data of the wavy beam drawn between two actors' bones (beam_drawWavyBeam). */
typedef struct {
    /* 0x000 */ Vtx vertices[2][44];  // double-buffered, indexed by gDrawBufferIndex
    /* 0x580 */ Actor *first;
    /* 0x584 */ u8 firstId;             // first->unique_id, to notice the slot being reused
    /* 0x585 */ u8 pad585[3];
    /* 0x588 */ Actor *second;
    /* 0x58C */ u8 secondId;
    /* 0x58D */ u8 pad58D[3];
    /* 0x590 */ f32 width;
    /* 0x594 */ f32 fadeStart;          // beam length where the wave starts
    /* 0x598 */ f32 fadeEnd;            // beam length where the wave is at fadeMax
    /* 0x59C */ f32 fadeScale;          // 1 / (fadeEnd - fadeStart)
    /* 0x5A0 */ f32 fadeMax;            // wave amplitude, as a fraction of width
    /* 0x5A4 */ u8 unk5A4[4];
} WavyBeamData; // size 0x5A8

/* The wavy beam effect (beam_spawnWavyBeam). */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ WavyBeamData data;
} WavyBeamEffect;

typedef struct BoltArcObj BoltArcObj;
typedef struct SparkObj SparkObj;

/* Lightning arcing between the four edges of a quad (beam_updateBoltArc, beam_boltArcMessage), set up by
 * level_spawnBoltArcZone (game/Level/ZoneSpawn/zonespawn.c) on the zone's rotated rectangle. */
typedef struct BoltArcState {
    /* 0x00 */ TriggerZone *region;
    /* 0x04 */ s16 timer;          /* frames left; negative when off */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ f32 pending;        /* bolts owed, accumulated per frame */
    /* 0x0C */ Vec3f edgeStart[4]; /* the rectangle's corners */
    /* 0x3C */ Vec3f edgeDelta[4]; /* edge i runs from edgeStart[i] to edgeStart[i] + edgeDelta[i] */
    /* 0x6C */ ZoneNode *zone;     /* collision_findZoneLeaf at the zone's x/z */
    /* 0x70 */ struct Actor *actor; /* NULL at the start; the player of the last message 0x38; read by func_1515FBC4 */
} BoltArcState; // size 0x74

struct BoltArcObj {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoltArcState state;
};

/* Sparks thrown up from random points of a spawn region (beam_updateSparkRegion, beam_sparkRegionMessage, beam_updateSparkZone). */
typedef struct SparkState {
    /* 0x00 */ TriggerZone *region;
    /* 0x04 */ f32 rate;
    /* 0x08 */ ZoneNode *zone;
    /* 0x0C */ u8 flags;           /* bit 0: on (set by message 0x38, cleared by 0x39) */
} SparkState;

struct SparkObj {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ SparkState state;
};

/* game/Beam/Strip/strip.c */
extern Gfx gStripRenderSetupDl[]; /* strip render setup (object type table D_8008B4D4) */
extern Gfx gStripRenderResetDl[]; /* strip render reset */
StripObj *beam_newStrip(StripDesc *desc, s32 payloadSize);
void beam_destroyStripDefault(StripObj *strip);
void beam_freeStripDefault(StripObj *strip);
void beam_destroyStrip(StripObj *strip);
void beam_freeStrip(StripObj *strip);
void beam_destroyStripDetach(StripObj *strip);
void beam_freeStripDetach(StripObj *strip);
void beam_updateStrip(StripObj *strip);
Gfx *beam_drawStrip(Gfx *gfx, StripObj *strip, s16 view);
s32 beam_stripFollowTarget(StripObj *strip);
s32 beam_stripFollowTargetBone(StripObj *strip);
void beam_sendStripMessage1(Actor *obj);
void beam_sendStripMessage2(Actor *obj);
void beam_stripMessage(StripObj *strip, ActorMsg *data, u8 msg);
void beam_stripTargetMessage(StripObj *strip, ActorMsg *data, u8 msg);

/* game/Beam/BeamQuad/beamquad.c */
void beam_spawnBoneQuad(Actor *owner, s32 type, u8 arg2, s32 arg3);
s32 beam_updateQuad(BeamQuad *effect);
Vtx_t *beam_drawQuad(BeamQuad *effect, s16 bufferIndex);
void beam_spawnLinkedQuad(f32 *pos, f32 *angle, f32 *halfWidth, f32 *alpha, f32 *halfHeight, s16 arg5, u8 arg6, s32 arg7);
s32 beam_updateLinkedQuad(BeamQuad *effect);
Vtx_t *beam_drawLinkedQuad(BeamQuad *effect, s16 bufferIndex);

/* game/Beam/Tracer/tracer.c */
void beam_spawnTracer(s32 arg0, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 speed, f32 stopDist, f32 halfWidth,
                   f32 length, s16 lifetime, u16 texture, s8 mode, s16 onTime, s16 offTime, Actor *owner, u8 ownerId,
                   u8 bone, s32 ownerTag, u8 impactEffect, u8 unkFE, f32 startX, f32 startY, f32 startZ, f32 endX,
                   f32 endY, f32 endZ);
void beam_updateTracer(Game10EF60Effect *effect);
Gfx *beam_drawTracer(Gfx *gfx, Game10EF60Effect *effect, s16 view);
s32 func_150E2DA4(s32 arg0, s32 arg1);
void beam_spawnBoneTracer(Actor *owner, u8 ownerId, s16 bone, s32 ownerTag, f32 startX, f32 startY, f32 startZ, f32 endX,
                   f32 endY, f32 endZ, s16 onTime, s16 offTime, u16 lifetime, u8 unkFE);
void beam_spawnBoneTracerEx(Actor *owner, s32 ownerId, s32 bone, s32 ownerTag, f32 startX, f32 startY, f32 startZ, f32 endX,
                   f32 endY, f32 endZ, s32 onTime, s32 offTime, s32 lifetime, f32 halfWidth, f32 length, s32 unkFE,
                   f32 speed);
s32 func_150E2F90(s32 a0, s32 a1, s16 a2);
void beam_tracerSwapMessage(Game10EF60Effect *effect, ActorSwapMsg *swap, u8 msg);
extern Gfx gTracerRenderSetupDl[];         /* tracer render setup display list */
extern AimImpact gAimImpacts[4];  /* aim-impact slots (effect_spawnMaterialImpact / effect_sprayDebris index them; 0xFF = none) */

/* game/Beam/AimedBeam/aimedbeam.c */
/* (Actor *actor, u8 slot); declared without a prototype: game/Event/TimedEvent/timedevent.c passes a third argument */
void beam_fireSlot();
/* (Actor *actor, u8 slot); declared without a prototype: game/Event/TimedEvent/timedevent.c passes a third argument */
void beam_releaseSlot();
AimedBeam *beam_spawnAimed(Actor *actor, u8 slot, u8 arg2, s32 arg3);
s32 beam_updateAimed(AimedBeam *beam);
s32 func_150EF784(Actor *actor, s32 arg1, s32 arg2);
void beam_destroyAimed(AimedBeam *beam);
void beam_freeAimed(AimedBeam *beam);
void beam_aimedMessage(AimedBeam *beam, BeamMsg *msg, u8 type);
s32 beam_updateSpark(Bolt *spark);
void beam_updateEmitter(BeamEmitter *emitter);
Glow *beam_spawnGlow(Actor *actor, u8 slot, u8 r, u8 g, u8 b, AimedBeam *beam, u8 arg6, s32 arg7);
s32 beam_updateGlow(Glow *glow);
LightEffect *beam_spawnFlash(u8 r, u8 g, u8 b, u8 a, AimedBeam *beam, u8 arg5, s32 arg6);
void beam_destroySpark(Bolt *spark);
void beam_freeSpark(Bolt *spark);
void beam_unlinkSpark(Bolt *spark);
void beam_destroyEmitter(BeamEmitter *emitter);
void beam_freeEmitter(BeamEmitter *emitter);
void beam_unlinkEmitter(BeamEmitter *emitter);
void beam_destroyGlow(Glow *glow);
void beam_freeGlow(Glow *glow);
void beam_unlinkGlow(Glow *glow);
void beam_destroyFlash(LightEffect *flash);
void beam_freeFlash(LightEffect *flash);
void beam_unlinkFlash(LightEffect *flash);
struct Game1EF500Trail *beam_fireSlotProjectile(); /* (Actor *actor, u8 slot, u8 arg2, s32 arg3); K&R definition, declared without a prototype */
void beam_sparkMessage(Bolt *spark, s32 data, u8 type);
void beam_glowMessage(Glow *glow, ActorMsg *msg, u8 type);
void beam_attachActorLights(Actor *actor);
void beam_spawnMuzzleParticles(Vec3f *pos);
void beam_spawnFollowParticle(Actor *actor);
s32 beam_updateFollowParticle(AttachedSpriteParticle *particle, s32 arg1);
void beam_followParticleMessage(AttachedSpriteParticle *particle, s32 data, u8 type);
void beam_attachTrailEmitter(Actor *actor);
void beam_updateTrailEmitter(ActorTrailEmitter *emitter);
void beam_trailEmitterMessage(ActorTrailEmitter *emitter, s32 data, u8 type);
void beam_actorBoundMessage(ActorBound *self, ActorMsg *msg, u8 type);
void beam_spawnMuzzleBurst(Actor *actor, u8 slot, u8 variant, u8 arg3, s32 arg4); /* K&R definition: the callers' view */

/* game_11EEB0.c */
void func_150F1A00(Actor *actor);
GameCommand *func_150F1B48(GameCommand *cmd, s32 index);
void func_150F1CB0(Actor *actor);
void beam_spawnActorExplosion(Actor *actor, u8 arg1, s32 arg2);
void beam_levelUpdateRandomSounds(s32 index);
void beam_spawnImpactBurstNearBone(Actor *actor, s32 arg1, s32 arg2);

/* game/Beam/ModelBeam/modelbeam.c */
void beam_showFadeSprite(s32 fadeIn, s32 hold, s32 fadeOut, s32 style);
void beam_updateGrowSprite(Sprite2D *sprite);
void beam_updateSpriteFade(Sprite2D *sprite);
void beam_levelUpdateRain(s32 arg0);
s32 beam_hookFrameTexture(ModelAttachment *att, Actor *actor);
s32 beam_hookSlideTexture(ModelAttachment *att, Actor *actor);
void beam_spawnBoneSparks(Actor *owner, s16 arg1, u8 arg2, s32 arg3);
void beam_updateBoneSparks(BoneSparkEffect *effect);
void beam_boneSparksMessage(BoneSparkEffect *effect, s32 arg1, u8 arg2);
void beam_updateModelBeam(ModelBeamEffect *effect);
void beam_releaseModelBeamParts(ModelBeamEffect *effect);
void beam_destroyModelBeamEffect(ModelBeamEffect *effect);
void beam_freeModelBeamEffect(ModelBeamEffect *effect);
void beam_modelBeamMessage(ModelBeamEffect *effect, s32 arg1, u8 arg2);
void beam_unlinkModelBeam(ModelBeam *beam);
void beam_destroyModelBeam(ModelBeam *beam);
void beam_freeModelBeam(ModelBeam *beam);
void func_150F6478(DefParticleHead *obj);
void func_150F6484(DefParticleHead *obj);
void func_150F64B0(DefParticleHead *obj);
void beam_updateFlashBurst(FlashBurstEffect *effect);
void beam_flashBurstMessage(FlashBurstEffect *effect, s32 arg1, u8 arg2);
void beam_spawnBoneBloodStrip(Actor *owner, s16 arg1, u8 arg2, s32 arg3);
void beam_emitBloodParticle(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip);

/* game/Beam/CastBeam/castbeam.c */
void beam_sendOwnerGone(Actor *owner);
BeamObj *beam_spawnCastBeam(Actor *owner);
void beam_updateCastBeam(BeamObj *obj);
void beam_castBeamMessage(BeamObj *obj, ActorRef *msg, u8 msgId);
void beam_teardownCastBeam(BeamObj *obj);
void beam_destroyCastBeam(BeamObj *obj);
void beam_freeCastBeam(BeamObj *obj);
extern Vec3f gLastBeamDir; // the last beam direction (beam_updateCastBeam); game/Camera/SegmentProgress/segmentprogress aims a camera by it

/* game/Beam/BoltRain/boltrain.c */
void beam_onPropArrival(WorldObject *prop);
void beam_updateBoltRain(BoltRainObj *obj);
void beam_boltRainMessage(BoltRainObj *obj, s32 arg1, u8 msg);
void beam_updateBoltArc(BoltArcObj *obj);
void beam_destroyOnMessage4B(EffectHeader *obj, s32 arg1, u8 msg);
void beam_boltArcMessage(BoltArcObj *obj, struct Actor **arg1, u8 msg);
void beam_spawnImpactEffects(ImpactObj *obj);
void beam_sparkRegionMessage(SparkObj *obj, s32 arg1, u8 msg);
void beam_updateSparkRegion(SparkObj *obj);
void beam_updateSparkZone(SparkObj *obj);
BoltArcObj *beam_findZoneRectEffect(TriggerZone *zone);

/* game/Beam/BoltChain/boltchain.c */
extern Gfx gBoltRenderSetupDl[]; /* the bolt effect type's render setup and teardown (D_8008C1A8) */
extern Gfx gBoltRenderTeardownDl[];
BoltObj *beam_newBolt(BoltDesc *desc, s32 extraSize, u8 arg2, s32 arg3);
void beam_updateBolt(BoltObj *bolt);
BoltSample *beam_getBoltSamples(BoltObj *bolt);
Vec3f *beam_getBoltJoints(BoltObj *bolt);
void beam_sampleBolt(BoltObj *bolt);
f32 beam_bezierB0(f32 t);
f32 beam_bezierB1(f32 t);
f32 beam_bezierB2(f32 t);
f32 beam_bezierB3(f32 t);
f32 beam_bezierB0Deriv(f32 t);
f32 beam_bezierB1Deriv(f32 t);
f32 beam_bezierB2Deriv(f32 t);
f32 beam_bezierB3Deriv(f32 t);
void beam_jitterBolt(BoltObj *bolt);
Gfx *beam_drawBolt(Gfx *gdl, BoltObj *bolt, s16 view);
void beam_releaseBolt(BoltObj *bolt);
void beam_destroyBolt(BoltObj *bolt);
void beam_freeBolt(BoltObj *bolt);
void beam_boltMessage(BoltObj *bolt, s32 arg1, u8 arg2);
BoltChainObj *beam_newBoltChain(Vec3f *start, Vec3f *end, s32 depth, BoltChainConfig *config, f32 spread, u8 kind,
                            u8 arg6, s32 arg7);
void beam_placeChainPoints(BoltChainObj *obj, s16 lo, s16 hi, f32 spread);
void beam_updateBoltChain(BoltChainObj *obj);
void beam_unlinkChainBolt(BoltObj *bolt);
void beam_killChainLink(BoltChainObj *obj);
void beam_destroyBoltChain(BoltChainObj *obj);
void beam_freeBoltChain(BoltChainObj *obj);
void beam_boltChainMessage(BoltChainObj *obj, s32 arg1, u8 arg2);
BoltObj *beam_spawnBoundsBolt(Actor *actor, s16 yaw, s16 pitch, s16 life, s32 segments, f32 jitter, f32 width,
                       s16 rejitterBase, s16 rejitterRange, u8 *color, u8 argA, s32 argB);
void beam_boundsBoltMessage(BoltObj *bolt, s32 arg1, u8 arg2);
s32 beam_followBoundsBolt(BoltObj *bolt, u8 *moved);
void beam_getBoundsPoint(Actor *actor, s32 yaw, s32 pitch, Vec3f *out, Vec3f *centerOut);
void beam_getStraightCtrlPoints(Vec3f *start, Vec3f *end, Vec3f *ctrl1, Vec3f *ctrl2);
void beam_spawnBoundsImpact(Actor *actor, s16 yaw, s16 pitch, u8 arg3, s32 arg4);
BoltObj *beam_spawnBoneBolt(Actor *actor, u8 bone, const Vec3f *localPos, s16 yaw, s16 pitch, s16 life, s32 segments,
                       f32 jitter, f32 width, s16 rejitterBase, s16 rejitterRange, u8 *color, u8 argC, s32 argD);
s32 beam_followBoneBolt(BoltObj *bolt, u8 *moved);
void beam_transformBoneLocal(Actor *actor, u8 bone, const Vec3f *localPos, Vec3f *out);
void beam_boneBoltMessage(BoltObj *bolt, s32 arg1, u8 arg2);

/* game/Beam/BeamMotion/beammotion.c */
Vtx *beam_fillMotionVerts(Game1BBFC0Beam *beam, s16 cam);
s32 beam_updateMotionPull(Game1BBFC0Beam *beam);
s32 beam_updateMotion(Game1BBFC0Beam *beam);
s32 beam_shrinkMotion(Game1BBFC0Beam *beam);
s32 beam_shrinkMotionProgress(Game1BBFC0Beam *beam);

/* game/Beam/Lightning/lightning.c */
extern Gfx gLightningRenderSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gLightningRenderTeardownDl[]; /* render teardown (object type table gObjectTypes) */
Bolt *beam_newLightning(BoltDef *def, s32 extraSize, u8 arg2, s32 arg3);
void beam_updateLightning(Bolt *bolt);
Gfx *beam_drawLightning(Gfx *gdl, Bolt *bolt, s16 view);
void beam_displaceLightning(Bolt *bolt, s16 first, s16 last, f32 amp, s16 depth);
void beam_freeLightningVtx(Bolt *bolt);
void beam_destroyLightning(Bolt *bolt);
void beam_freeLightning(Bolt *bolt);
Bolt *beam_spawnBoneLightning(Actor *owner, Vec3f *boneOffset, u8 bone, s16 lifetime, f32 jitterDelay, f32 jitterDelayRange,
                    BoltColor *color, f32 width, Vec3f *targetOffset, u8 arg9, u8 argA, s8 update2, s32 extraSize,
                    u8 argD, s32 argE);
s32 beam_followBoneLightning(Bolt *bolt);
void beam_lightningMessage(Bolt *bolt, s32 msg, u8 msgType);
void beam_boneLightningMessage(Bolt *bolt, s32 arg, u8 msgType);

/* game/Beam/AreaEmitter/areaemitter.c */
Emitter *beam_newAreaEmitter(EmitterDef *def, s16 lifetime, s32 extraSize, u8 arg3, s32 arg4);
void beam_updateAreaEmitter(Emitter *emitter);
void beam_pickAnchorPoint(Emitter *emitter, Vec3f *pos, f32 *top, f32 *bottom);
void beam_pickRadiusPoint(Emitter *emitter, Vec3f *pos, f32 *top, f32 *bottom);
void beam_areaEmitterMessage(Emitter *emitter, u8 *msg, u8 msgType);
s32 beam_runZoneHandler(TriggerZone *obj);
void beam_spawnZoneEmitter(TriggerZone *obj, s32 kind);
void beam_spawnSpriteParticle(Emitter *emitter, Vec3f *pos, f32 top, u8 noGround);
void beam_switchEmitterMessage(Emitter *emitter, u8 *msg, u8 msgType);
void beam_spawnEmitterPreset(u8 options, u8 preset, TriggerZone *obj, Vec3f *pos, f32 bottom, f32 radius, s16 lifetime,
                   u8 arg7, s32 arg8);
void beam_launchJitterParticle(Emitter *emitter, Vec3f *pos, f32 top, u8 noGround);
void beam_spawnAtGroundPoints(Vec3f *center, f32 minY, f32 radius, CollisionHit *hit, s32 count, u8 spawnFunc, void *arg6,
                   u8 arg7, s32 arg8);

/* game/Beam/WavyBeam/wavybeam.c */
WavyBeamEffect *beam_spawnWavyBeam(s16 duration, u8 arg1, s32 arg2);
Gfx *beam_drawWavyBeam(Gfx *gdl, WavyBeamEffect *effect, s16 viewIdx);
void beam_wavyBeamMessage(WavyBeamEffect *effect, void *msg, u8 type);

#endif
