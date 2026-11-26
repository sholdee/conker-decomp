#ifndef CONKER_BEAM_H
#define CONKER_BEAM_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "beam": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_F2820.c
 *   game_108320.c
 *   game_10EF60.c
 *   game_11C2B0.c
 *   game_11EEB0.c
 *   game_1228D0.c
 *   game_124260.c
 *   game_12E540.c
 *   game_131F30.c
 *   game_133190.c
 *   game_1BBFC0.c
 *   game_1D4E00.c
 *   game_1D5FD0.c
 *   game_1E26F0.c
 */

/* Lightning bolts (game_1D4E00.c, game_1D5FD0.c). */
typedef struct {
    u8 r, g, b, a;
} BoltColor;

/* Spawn parameters of a lightning bolt; func_151A7950 copies them into Bolt.def. */
typedef struct {
    /* 0x00 */ s16 lifetime;         /* frames, counted down while BOLT_TIMED */
    /* 0x02 */ s16 numPoints;
    /* 0x04 */ f32 jitterDelay;      /* re-jitter after jitterDelay + rand * jitterDelayRange */
    /* 0x08 */ f32 jitterDelayRange;
    /* 0x0C */ u8 flags;             /* BOLT_TIMED | BOLT_ACTIVE */
    /* 0x10 */ f32 jitter;           /* largest sideways displacement of a midpoint */
    /* 0x14 */ BoltColor color;
    /* 0x18 */ f32 width;
    /* 0x1C */ s8 update1;           /* index into D_8008F940, -1 for none */
    /* 0x1D */ s8 update2;           /* index into D_8008F948, -1 for none */
    /* 0x20 */ Vec3f start;
    /* 0x2C */ Vec3f end;
    /* 0x38 */ Vec3f up;
    /* 0x44 */ f32 length;
    /* 0x48 */ f32 invLength;
    /* 0x4C */ u8 type;              /* index into the D_8008F94C/D_8008F958/D_8008F964 handlers */
} BoltDef; /* size 0x50 */

#define BOLT_TIMED  1
#define BOLT_ACTIVE 2

typedef struct {
    Vec3f local; /* x: sideways, y: along up, z: fraction of the way from start to end */
    Vec3f world;
} BoltPoint;

/* User data of a bolt that runs from an actor bone to a point near the actor (func_151A8624). */
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
                                      * for a beam's sparks (game_11C2B0.c) */
    /* 0x64 */ BoltPoint *points;
    /* 0x68 */ f32 jitterTimer;
    /* 0x6C */ u8 vtxBuf[0x10];
    /* 0x7C */ s32 vtxBufSize;
    /* 0x80 */ BoltPoint pointData[1];
} Bolt;

/* Spawn parameters; func_150C5370 copies them into the object at +0x18. */
typedef struct StripDesc {
    /* 0x00 */ Vec3f p0;
    /* 0x0C */ Vec3f p1;
    /* 0x18 */ s8 updateIdx;
    /* 0x19 */ u8 pad19[3];
    /* 0x1C */ f32 texScale;
    /* 0x20 */ u8 unk20;
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
    /* 0x30 */ s8 updateIdx;    // index into D_80088760, -1 for none
    /* 0x31 */ u8 pad31[3];
    /* 0x34 */ f32 texScale;    // v texcoord = length * texScale * 64
    /* 0x38 */ u8 unk38;        // index into D_800887B8 / D_800887C0 / D_800887C8
    /* 0x39 */ u8 pad39[7];
    /* 0x40 */ Vtx quad[2][4];  // double-buffered by D_800BE9C0
    /* 0xC0 */ f32 length;      // |p0 - p1|
    /* 0xC4 */ u8 padC4[4];
    /* 0xC8 */ StripTarget target;
} StripObj;

/* Self-propelled beam quad (func_150DAE70 / func_150DB114). */
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

/* Quad that follows externally owned values (func_150DB518 / func_150DB630 / func_150DB714). */
typedef struct {
    f32 *pos;
    f32 *angle;     /* degrees */
    f32 *halfWidth;
    f32 *halfHeight;
    f32 *alpha;
} LinkedQuad;

/* Quad particle created by func_1513D524 (a DefParticleHead view) with this file's motion after it. */
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
    Vtx_t quads[2][4]; // double-buffered by D_800BE9C0
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
    s16 texture;    // index into D_8008CA4C
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

/* A beam's emitter (created by func_15149130), carrying its EmitterLink at +0x28. */
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

/* State an actor-following particle (created by func_15130280) carries at +0xA8. */
typedef struct {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId;
    /* 0x08 */ f32 phase[3];
} ActorParticleLink;

/* State a trail particle (see func_150F1170) carries at +0xA8. */
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

/* State an actor's trail emitter (created by func_15149130) carries at +0x28. */
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

/* Data of the spark effect spawned by func_150F5C08 and run by func_150F5C98 (sparks along a
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

typedef struct {
    /* 0x00 */ u8 pad0[0x48];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ ModelBeamEffect *effect; /* the beam effect that drives this beam */
    /* 0x54 */ u8 pad54[0x58 - 0x54];
    /* 0x58 */ u8 flags; /* bit 0 = shown */
} BeamPart;

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
    LightEffect *flash; /* created by func_151602C0 */
} ModelBeamData;

struct ModelBeamEffect {
    TimedEffect base;
    ModelBeamData data;
};

/* A sprite spawned by func_15130280 that marks one end of the beam. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x30];
    /* 0x40 */ Vec3f pos;
    /* 0x4C */ u8 pad4C[0x74 - 0x4C];
    /* 0x74 */ s8 mode; /* 3 or -1 (see func_150F706C) */
} BeamEnd;

/* Per-object state of a beam (0x78 bytes, copied in by func_150F6DE4). */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; /* owner->unique_id */
    /* 0x08 */ BeamEnd *ends[2];
    /* 0x10 */ RayHit ray;
    /* 0x74 */ u8 flags;
} BeamState;

/* A beam object allocated by func_15149130. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BeamState beam;
} BeamObj;

typedef struct RayBeam RayBeam;

/* Aim state of the beam's owner (owner->aim). */
typedef struct {
    u8 pad0[0xB4];
    f32 targetDist;
    Vec3f targetPos;
    u8 padC4[0x24];
    Vec3f targetDir;
    CollisionTri targetHit;
    u8 pad106[3];
    u8 targetType; // 0 = no target
    u8 pad10A[0x8D];
    u8 unk197;
} BeamOwnerAim;

/* The actor the beam is attached to. */
typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x38];
    u8 unk74;
    u8 pad75[0x15F];
    MtxF *boneMtx;
    u8 pad1D8[0x144];
    BeamOwnerAim *aim;
} BeamOwner;

/* How a child object refers back to its beam. */
typedef struct {
    RayBeam *beam;
    u8 flags; // 1 = visible
} RayBeamLink;

/* beam->data.startFx: placed at the beam's start, sized by the beam's width. */
typedef struct {
    /* 0x00 */ DescParticleHead head; /* a sprite particle */
    /* 0xA8 */ f32 baseSizeX;
    /* 0xAC */ f32 baseSizeY;
    /* 0xB0 */ u8 padB0[0x20];
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

/* RayBeam.data */
typedef struct {
    BeamOwner *owner;
    u8 unk4; // must match owner->unk3B
    u8 pad5[3];
    Vec3f localStart;  // in bone space
    Vec3f localTarget; // in bone space
    u8 bone;
    u8 pad21;
    u8 unk22;
    u8 flags; // 1 = drawn, 2 = aimed by the owner
    BeamStartFx *startFx;
    BeamHitFx *hitFx;
    LightEffect *endPoint; // light at the beam's end; its payload (data.object) is the RayBeam
    BeamLine *line;
    BeamEndFx *endFx;
    f32 width;
    u8 pad3C[8];
    f32 length;
    f32 scrollBase;  // texture scroll speed = scrollBase + scrollRange * sin(scrollPhase)
    f32 scrollRange;
    f32 scrollPhase;
    u8 pad54[4];
    f32 texOffset;
    u8 pad5C[0x34];
    s16 unk90;
} RayBeamData;

/* A beam that runs from a bone of its owner to the aim target or the first collision on the way
 * (placed by func_15101378, drawn by func_151019C4). */
struct RayBeam {
    /* 0x000 */ DefParticleHead head; /* head.def.pos: the start, head.def.rot: the end point */
    /* 0x110 */ RayBeamData data;
};

/* Rain of lightning bolts falling around a point onto the ground (func_15104C44, func_15104FF8). */
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
 * A bolt object. It is followed in memory by desc.segments BoltSamples (func_151061E0),
 * desc.segments + 2 jittered joint points (func_151061EC) and the per-kind user data.
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
    /* 0x40 */ u8 kind;            /* index into D_80088C38 */
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
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ f32 blend;
} Game1BBFC0Motion;

typedef struct Game1BBFC0Beam {
    /* 0x000 */ u8 unknown0[0x1C];
    /* 0x01C */ s16 unk1C;
    /* 0x01E */ u8 unknown1E[0xE];
    /* 0x02C */ f32 width;
    /* 0x030 */ f32 progress;
    /* 0x034 */ Vec3f start;
    /* 0x040 */ Vec3f end;
    /* 0x04C */ u8 unknown4C[0x10];
    /* 0x05C */ u8 unk5C;
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

/* User data of a type 2 emitter: the particles spawn hook 1 (func_151A9634) launches. */
typedef struct {
    /* 0x00 */ f32 vec0Base;         /* the two floats passed to func_1514B8E4: base + rand * range */
    /* 0x04 */ f32 vec1Base;
    /* 0x08 */ f32 vec0Range;
    /* 0x0C */ f32 vec1Range;
    /* 0x10 */ RangeS16 unk10;
    /* 0x14 */ RangeS16 unk14;       /* goes into the particle's extra data */
    /* 0x18 */ RangeS16 unk18;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ f32 unk20Base;        /* goes into the particle's extra data */
    /* 0x24 */ f32 unk20Range;
    /* 0x28 */ u8 flags;             /* 1: particle kind 0x71 (else 0x13 or 0x14); 2: clear the 2 flag */
} ParticleSettings; /* size 0x2C */

/* Effect data of the wavy beam drawn between two actors' bones (func_151B5328). */
typedef struct {
    /* 0x000 */ Vtx vertices[2][44];  // double-buffered, indexed by D_800BE9C0
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

/* The wavy beam effect (func_151B5240). */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ WavyBeamData data;
} WavyBeamEffect;

typedef struct BoltArcObj BoltArcObj;
typedef struct SparkObj SparkObj;

/* Lightning arcing between the four edges of a quad (func_151050B0, func_15105548). */
typedef struct BoltArcState {
    /* 0x00 */ TriggerZone *region;
    /* 0x04 */ s16 timer;          /* frames left; negative when off */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ f32 pending;        /* bolts owed, accumulated per frame */
    /* 0x0C */ Vec3f edgeStart[4];
    /* 0x3C */ Vec3f edgeDelta[4]; /* edge i runs from edgeStart[i] to edgeStart[i] + edgeDelta[i] */
    /* 0x6C */ ZoneNode *zone;
    /* 0x70 */ s32 unk70;
} BoltArcState;

struct BoltArcObj {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ BoltArcState state;
};

/* Sparks thrown up from random points of a spawn region (func_151058B4, func_15105848, func_15105BC8). */
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

/* game_F2820.c */
StripObj *func_150C5370(StripDesc *desc, s32 payloadSize);
void func_150C5430(StripObj *strip);
void func_150C5450(StripObj *strip);
void func_150C5470(StripObj *strip);
void func_150C54C0(StripObj *strip);
void func_150C5510(StripObj *strip);
void func_150C553C(StripObj *strip);
void func_150C5568(StripObj *strip);
Gfx *func_150C56A4(Gfx *gfx, StripObj *strip, s16 view);
s32 func_150C5B88(StripObj *strip);
s32 func_150C5BD4(StripObj *strip);
void func_150C5C74(Actor *obj);
void func_150C5C9C(Actor *obj);
void func_150C5CC4(StripObj *strip, ActorMsg *data, u8 msg);
void func_150C5D0C(StripObj *strip, ActorMsg *data, u8 msg);

/* game_108320.c */
void func_150DAE70(Actor *owner, s32 type, u8 arg2, s32 arg3);
s32 func_150DB114(BeamQuad *effect);
Vtx_t *func_150DB2D8(BeamQuad *effect, s16 bufferIndex);
void func_150DB518(f32 *pos, f32 *angle, f32 *halfWidth, f32 *alpha, f32 *halfHeight, s16 arg5, u8 arg6, s32 arg7);
s32 func_150DB630(BeamQuad *effect);
Vtx_t *func_150DB714(BeamQuad *effect, s16 bufferIndex);

/* game_10EF60.c */
void func_150E1AB0(s32 arg0, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 speed, f32 stopDist, f32 halfWidth,
                   f32 length, s16 lifetime, u16 texture, s8 mode, s16 onTime, s16 offTime, Actor *owner, u8 ownerId,
                   u8 bone, s32 ownerTag, u8 impactEffect, u8 unkFE, f32 startX, f32 startY, f32 startZ, f32 endX,
                   f32 endY, f32 endZ);
void func_150E1D14(Game10EF60Effect *effect);
Gfx *func_150E28DC(Gfx *gfx, Game10EF60Effect *effect, s16 view);
s32 func_150E2DA4(s32 arg0, s32 arg1);
void func_150E2DB4(Actor *owner, u8 ownerId, s16 bone, s32 ownerTag, f32 startX, f32 startY, f32 startZ, f32 endX,
                   f32 endY, f32 endZ, s16 onTime, s16 offTime, u16 lifetime, u8 unkFE);
void func_150E2EA4(Actor *owner, s32 ownerId, s32 bone, s32 ownerTag, f32 startX, f32 startY, f32 startZ, f32 endX,
                   f32 endY, f32 endZ, s32 onTime, s32 offTime, s32 lifetime, f32 halfWidth, f32 length, s32 unkFE,
                   f32 speed);
s32 func_150E2F90(s32 a0, s32 a1, s16 a2);
void func_150E2FC0(Game10EF60Effect *effect, ActorSwapMsg *swap, u8 msg);

/* game_11C2B0.c */
/* (Actor *actor, u8 slot); declared without a prototype: game_981E0.c passes a third argument */
void func_150EEE00();
/* (Actor *actor, u8 slot); declared without a prototype: game_981E0.c passes a third argument */
void func_150EEF40();
AimedBeam *func_150EEF80(Actor *actor, u8 slot, u8 arg2, s32 arg3);
s32 func_150EF38C(AimedBeam *beam);
s32 func_150EF784(Actor *actor, s32 arg1, s32 arg2);
void func_150EF7B0(AimedBeam *beam);
void func_150EF860(AimedBeam *beam);
void func_150EF910(AimedBeam *beam, BeamMsg *msg, u8 type);
s32 func_150EFA4C(Bolt *spark);
void func_150EFB80(BeamEmitter *emitter);
Glow *func_150EFEC8(Actor *actor, u8 slot, u8 r, u8 g, u8 b, AimedBeam *beam, u8 arg6, s32 arg7);
s32 func_150F00EC(Glow *glow);
LightEffect *func_150F0198(u8 r, u8 g, u8 b, u8 a, AimedBeam *beam, u8 arg5, s32 arg6);
void func_150F0260(Bolt *spark);
void func_150F0280(Bolt *spark);
void func_150F02A0(Bolt *spark);
void func_150F02C0(BeamEmitter *emitter);
void func_150F02EC(BeamEmitter *emitter);
void func_150F0318(BeamEmitter *emitter);
void func_150F0328(Glow *glow);
void func_150F0354(Glow *glow);
void func_150F0380(Glow *glow);
void func_150F0390(LightEffect *flash);
void func_150F03BC(LightEffect *flash);
void func_150F03E8(LightEffect *flash);
struct Game1EF500Trail *func_150F03F8(Actor *actor, s32 slot, s32 arg2, s32 arg3); /* K&R definition (u8 slot, arg2): no caller declares it */
void func_150F07E4(Bolt *spark, s32 data, u8 type);
void func_150F088C(Glow *glow, ActorMsg *msg, u8 type);
void func_150F0938(Actor *actor);
void func_150F0A24(Vec3f *pos);
void func_150F0BEC(Actor *actor);
s32 func_150F0E48(AttachedSpriteParticle *particle, s32 arg1);
void func_150F1020(AttachedSpriteParticle *particle, s32 data, u8 type);
void func_150F10D4(Actor *actor);
void func_150F1170(ActorTrailEmitter *emitter);
void func_150F15F8(ActorTrailEmitter *emitter, s32 data, u8 type);
void func_150F1684(ActorBound *self, ActorMsg *msg, u8 type);
void func_150F16DC(Actor *actor, u8 slot, u8 variant, u8 arg3, s32 arg4); /* K&R definition: the callers' view */

/* game_11EEB0.c */
void func_150F1A00(Actor *actor);
GameCommand *func_150F1B48(GameCommand *cmd, s32 index);
void func_150F1CB0(Actor *actor);
void func_150F1D10(Actor *actor, u8 arg1, s32 arg2);
void func_150F20F0(s32 index);
void func_150F2230(Actor *actor, s32 arg1, s32 arg2);

/* game_1228D0.c */
void func_150F5420(s32 fadeIn, s32 hold, s32 fadeOut, s32 style);
void func_150F5590(Sprite2D *sprite);
void func_150F55C8(Sprite2D *sprite);
void func_150F568C(s32 arg0);
s32 func_150F56B0(ModelAttachment *att, Actor *actor);
s32 func_150F5A54(ModelAttachment *att, Actor *actor);
void func_150F5C08(Actor *owner, s16 arg1, u8 arg2, s32 arg3);
void func_150F5C98(BoneSparkEffect *effect);
void func_150F6138(BoneSparkEffect *effect, s32 arg1, u8 arg2);
void func_150F6178(ModelBeamEffect *effect);
void func_150F631C(ModelBeamEffect *effect);
void func_150F6368(ModelBeamEffect *effect);
void func_150F6394(ModelBeamEffect *effect);
void func_150F63C0(ModelBeamEffect *effect, s32 arg1, u8 arg2);
void func_150F6400(ModelBeam *beam);
void func_150F6420(ModelBeam *beam);
void func_150F644C(ModelBeam *beam);
void func_150F6478(DefParticleHead *obj);
void func_150F6484(DefParticleHead *obj);
void func_150F64B0(DefParticleHead *obj);
void func_150F64DC(FlashBurstEffect *effect);
void func_150F6850(FlashBurstEffect *effect, s32 arg1, u8 arg2);
void func_150F6890(Actor *owner, s16 arg1, u8 arg2, s32 arg3);
void func_150F695C(Vec3f *pointA, Vec3f *pointB, Vec3f *velocityA, Vec3f *velocityB, f32 time, BloodStrip *strip);

/* game_124260.c */
void func_150F6DB0(Actor *owner);
BeamObj *func_150F6DE4(Actor *owner);
void func_150F706C(BeamObj *obj);
void func_150F7310(BeamObj *obj, ActorRef *msg, u8 msgId);
void func_150F739C(BeamObj *obj);
void func_150F740C(BeamObj *obj);
void func_150F7438(BeamObj *obj);

/* game_12E540.c */
void func_15101090(RayBeam *beam);
void func_15101148(RayBeam *beam);
void func_151011E8(BeamHitFx *fx);
void func_15101210(BeamHitFx *fx);
void func_15101238(LightEffect *endPoint);
void func_15101260(LightEffect *endPoint);
void func_15101288(BeamStartFx *fx);
void func_151012B0(BeamStartFx *fx);
void func_151012D8(BeamLine *line);
void func_15101300(BeamLine *line);
void func_15101328(BeamEndFx *fx);
void func_15101350(BeamEndFx *fx);
s8 func_15101378(RayBeam *beam);
Vtx *func_151019C4(RayBeam *beam, s16 viewport);

/* game_131F30.c */
void func_15104A80(WorldObject *prop);
void func_15104C44(BoltRainObj *obj);
void func_15104FF8(BoltRainObj *obj, s32 arg1, u8 msg);
void func_151050B0(BoltArcObj *obj);
void func_1510550C(EffectHeader *obj, s32 arg1, u8 msg);
void func_15105548(BoltArcObj *obj, s32 *arg1, u8 msg);
void func_1510558C(ImpactObj *obj);
void func_15105848(SparkObj *obj, s32 arg1, u8 msg);
void func_151058B4(SparkObj *obj);
void func_15105BC8(SparkObj *obj);
ZoneRectEffect *func_15105C24(TriggerZone *zone);

/* game_133190.c */
BoltObj *func_15105CE0(BoltDesc *desc, s32 extraSize, u8 arg2, s32 arg3);
void func_1510608C(BoltObj *bolt);
BoltSample *func_151061E0(BoltObj *bolt);
Vec3f *func_151061EC(BoltObj *bolt);
void func_15106214(BoltObj *bolt);
f32 func_151064B4(f32 t);
f32 func_151064DC(f32 t);
f32 func_15106510(f32 t);
f32 func_15106540(f32 t);
f32 func_15106558(f32 t);
f32 func_15106584(f32 t);
f32 func_151065BC(f32 t);
f32 func_151065EC(f32 t);
void func_15106610(BoltObj *bolt);
Gfx *func_151067B8(Gfx *gdl, BoltObj *bolt, s16 view);
void func_15106E78(BoltObj *bolt);
void func_15106EF8(BoltObj *bolt);
void func_15106F24(BoltObj *bolt);
void func_15106F50(BoltObj *bolt, s32 arg1, u8 arg2);
BoltChainObj *func_15106F98(Vec3f *start, Vec3f *end, s32 depth, BoltChainConfig *config, f32 spread, u8 kind,
                            u8 arg6, s32 arg7);
void func_151070F8(BoltChainObj *obj, s16 lo, s16 hi, f32 spread);
void func_151072BC(BoltChainObj *obj);
void func_15107604(BoltObj *bolt);
void func_1510761C(BoltChainObj *obj);
void func_1510764C(BoltChainObj *obj);
void func_15107678(BoltChainObj *obj);
void func_151076A4(BoltChainObj *obj, s32 arg1, u8 arg2);
BoltObj *func_15107700(Actor *actor, s16 yaw, s16 pitch, s16 life, s32 segments, f32 jitter, f32 width,
                       s16 rejitterBase, s16 rejitterRange, u8 *color, u8 argA, s32 argB);
void func_151078E4(BoltObj *bolt, s32 arg1, u8 arg2);
s32 func_15107924(BoltObj *bolt, u8 *moved);
void func_15107A20(Actor *actor, s32 yaw, s32 pitch, Vec3f *out, Vec3f *centerOut);
void func_15107AE0(Vec3f *start, Vec3f *end, Vec3f *ctrl1, Vec3f *ctrl2);
void func_15107B78(Actor *actor, s16 yaw, s16 pitch, u8 arg3, s32 arg4);
BoltObj *func_15107C1C(Actor *actor, u8 bone, Vec3f *localPos, s16 yaw, s16 pitch, s16 life, s32 segments,
                       f32 jitter, f32 width, s16 rejitterBase, s16 rejitterRange, u8 *color, u8 argC, s32 argD);
s32 func_15107E48(BoltObj *bolt, u8 *moved);
void func_15107F54(Actor *actor, u8 bone, Vec3f *localPos, Vec3f *out);
void func_15107F98(BoltObj *bolt, s32 arg1, u8 arg2);

/* game_1BBFC0.c */
Vtx *func_1518EB10(Game1BBFC0Beam *beam, s16 cam);
s32 func_1518EF54(Game1BBFC0Beam *beam);
s32 func_1518F058(Game1BBFC0Beam *beam);
s32 func_1518F108(Game1BBFC0Beam *beam);
s32 func_1518F15C(Game1BBFC0Beam *beam);

/* game_1D4E00.c */
Bolt *func_151A7950(BoltDef *def, s32 extraSize, u8 arg2, s32 arg3);
void func_151A7A90(Bolt *bolt);
Gfx *func_151A7D6C(Gfx *gdl, Bolt *bolt, s16 view);
void func_151A8340(Bolt *bolt, s16 first, s16 last, f32 amp, s16 depth);
void func_151A8560(Bolt *bolt);
void func_151A8584(Bolt *bolt);
void func_151A85D4(Bolt *bolt);
Bolt *func_151A8624(Actor *owner, Vec3f *boneOffset, u8 bone, s16 lifetime, f32 jitterDelay, f32 jitterDelayRange,
                    BoltColor *color, f32 width, Vec3f *targetOffset, u8 arg9, u8 argA, s8 update2, s32 extraSize,
                    u8 argD, s32 argE);
s32 func_151A87F8(Bolt *bolt);
void func_151A8A20(Bolt *bolt, void *msg, u8 msgType);
void func_151A8A78(Bolt *bolt, void *arg, u8 msgType);

/* game_1D5FD0.c */
Emitter *func_151A8B20(EmitterDef *def, s16 lifetime, s32 extraSize, u8 arg3, s32 arg4);
void func_151A8CEC(Emitter *emitter);
void func_151A8F1C(Emitter *emitter, Vec3f *pos, f32 *top, f32 *bottom);
void func_151A8F6C(Emitter *emitter, Vec3f *pos, f32 *top, f32 *bottom);
void func_151A9024(Emitter *emitter, u8 *msg, u8 msgType);
s32 func_151A9060(TriggerZone *obj);
void func_151A90C0(TriggerZone *obj, s32 kind);
void func_151A91AC(Emitter *emitter, Vec3f *pos, f32 top, u8 noGround);
void func_151A931C(Emitter *emitter, u8 *msg, u8 msgType);
void func_151A9390(u8 options, u8 preset, TriggerZone *obj, Vec3f *pos, f32 bottom, f32 radius, s16 lifetime,
                   u8 arg7, s32 arg8);
void func_151A9634(Emitter *emitter, Vec3f *pos, f32 top, u8 noGround);
void func_151A9834(Vec3f *center, f32 minY, f32 radius, CollisionHit *hit, s32 count, u8 spawnFunc, void *arg6,
                   u8 arg7, s32 arg8);
extern const ParticleSettings D_800A8DF0;
extern const ParticleSettings D_800A8E1C;
extern const ParticleSettings D_800A8E48;
extern const ParticleSettings D_800A8E74;
extern const ParticleSettings D_800A8EA0;
extern const ParticleSettings D_800A8ECC;
extern const ParticleSettings D_800A8EF8;
extern const ParticleSettings D_800A8F24;

/* game_1E26F0.c */
WavyBeamEffect *func_151B5240(s16 duration, u8 arg1, s32 arg2);
Gfx *func_151B5328(Gfx *gdl, WavyBeamEffect *effect, s16 viewIdx);
void func_151B5A9C(WavyBeamEffect *effect, void *msg, u8 type);

#endif
