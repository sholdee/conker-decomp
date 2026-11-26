#ifndef CONKER_ROPE_H
#define CONKER_ROPE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "rope": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Rope/SwingRope/swingrope.c
 *   game/Rope/RopeTie/ropetie.c
 *   game/Effects/HoltenRope/holtenrope.c
 */

/* Template copied over HoltenRopeEffect 0x10-0x47 when a rope is created (rope_createEffect). */
typedef struct HoltenRopeDesc {
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 timer;
    /* 0x04 */ Vec3f start;
    /* 0x10 */ Vec3f end;
    /* 0x1C */ s8 startCallback;
    /* 0x1D */ s8 endCallback;
    /* 0x1E */ u8 drawStyle;
    /* 0x1F */ u8 pad1F;
    /* 0x20 */ f32 halfWidth;
    /* 0x24 */ s8 updateCallback;
    /* 0x25 */ u8 pad25[3];
    /* 0x28 */ f32 length;
    /* 0x2C */ f32 accelScale;
    /* 0x30 */ f32 lagScale;
    /* 0x34 */ u8 type;
    /* 0x35 */ u8 pad35[3];
} HoltenRopeDesc; /* size 0x38 */

/* One rope node (0x18 bytes). */
typedef struct {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ f32 vel[3];
} RopeNode;

typedef struct Rope {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 id;                  // start of the 0x24-byte spawn data
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 x;
    /* 0x18 */ f32 topY;
    /* 0x1C */ f32 z;
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ f32 bottomY;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ u8 pad30[4];
    /* 0x34 */ f32 unk34;              // 1.0f / unk2C
    /* 0x38 */ RopeNode *nodes;        // nodes[0] is the bottom, nodes[nodeCount - 1] the anchor
    /* 0x3C */ f32 segmentLength;
    /* 0x40 */ u16 textureId;
    /* 0x42 */ u8 nodeCount;
    /* 0x43 */ u8 pad43;
    /* 0x44 */ Actor *rider;       // object hanging on the rope, or NULL
    /* 0x48 */ Vtx *vertices;          // [2 frame buffers][20 nodes][4]
    /* 0x4C */ f32 releaseForce;       // impulse applied to releaseNode, decays each frame
    /* 0x50 */ u8 releaseAngle;
    /* 0x51 */ u8 releaseNode;
    /* 0x52 */ s16 regrabTimer;        // the rope can't be grabbed until this reaches 0
    /* 0x54 */ u16 soundHandle;
} Rope;

/* The 0x30-byte link object that moves an owner along its rope (rope_createRider). */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 pad19[3];
    /* 0x1C */ Actor *owner;
    /* 0x20 */ u8 ownerId;             // owner->unique_id
    /* 0x21 */ u8 pad21[3];
    /* 0x24 */ s32 ownerIndex;         // owner - movement_obj
    /* 0x28 */ f32 radius;             // horizontal offset of the owner from the rope
} RopeRider;

/* A point fixed relative to one of an object's bones. */
typedef struct {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetId; // target->unique_id when attached
    /* 0x05 */ u8 bone;     // index into the Mtx array at target->ob_matrix_ptr
    /* 0x08 */ Vec3f offset;
} RopeAnchor;

typedef struct TiedRope TiedRope;

typedef struct RopeLine RopeLine;

/* Data at 0x28 of the effect that ties obj0 to obj1 with ropes. */
typedef struct {
    /* 0x00 */ Actor *obj0;
    /* 0x04 */ u8 obj0Id;   // obj0->unique_id when attached
    /* 0x08 */ Actor *obj1;
    /* 0x0C */ u8 obj1Id;
    /* 0x0D */ u8 state;    // last rope_getTieState result, 0 before the first update
    /* 0x10 */ TiedRope *ropes[3];
    /* 0x1C */ RopeLine *line;
} RopeTieData;

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ RopeTieData tie;
} RopeTie;

/* Data at 0x150 of each rope a RopeTie owns. */
typedef struct {
    /* 0x00 */ RopeAnchor start;
    /* 0x14 */ RopeAnchor end;
    /* 0x28 */ RopeTie *owner;
    /* 0x2C */ u8 slot; // index in owner->tie.ropes
} RopeTieLink;

/* Data at 0x28 of the straight line effect a RopeTie owns. */
typedef struct {
    /* 0x00 */ RopeAnchor anchor; // anchor.offset is the start point
    /* 0x14 */ Vec3f endOffset;
    /* 0x20 */ f32 halfWidth;
    /* 0x24 */ RopeTie *owner;
} RopeLineData;

struct RopeLine {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ RopeLineData line;
};

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
} HoltenRopePoint;

/* Rope effect: a 10-point chain hung between two endpoints and drawn as a textured strip. */
typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x010 */ u8 flags;           // ROPE_FLAG_*
    /* 0x011 */ u8 pad11;
    /* 0x012 */ s16 timer;
    /* 0x014 */ Vec3f start; // written by startCallback
    /* 0x020 */ Vec3f end;   // written by endCallback
    /* 0x02C */ s8 startCallback;   // index into gRopeEndpointCallbacks, -1 for none
    /* 0x02D */ s8 endCallback;     // index into gRopeEndpointCallbacks, -1 for none
    /* 0x02E */ u8 drawStyle;       // index into gRopeDrawStyles
    /* 0x02F */ u8 pad2F;
    /* 0x030 */ f32 halfWidth;
    /* 0x034 */ s8 updateCallback;  // index into gRopeUpdateCallbacks, -1 for none
    /* 0x035 */ u8 pad35[3];
    /* 0x038 */ f32 length;
    /* 0x03C */ f32 accelScale;
    /* 0x040 */ f32 lagScale;
    /* 0x044 */ u8 type;            // index into gRopeTypeMsgHandlers and gRopeTypeCleanups
    /* 0x045 */ u8 pad45[3];
    /* 0x048 */ HoltenRopePoint points[10];
    /* 0x138 */ f32 axesSq;         // a^2 + b^2 of a half ellipse with arc length `length`: 2 * length^2 / pi^2
    /* 0x13C */ s32 texStep;        // texture t step per segment
    /* 0x140 */ u8 vtxBuffer[0x10];
    /* 0x150 */                     // type-specific data follows
} HoltenRopeEffect;

/* The actor a rope end is attached to. */
/* Endpoint data at 0x150 for ropes hung from an actor. */
typedef struct {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetId; // target->unique_id when the rope was attached
} HoltenRopeAnchor;

typedef struct {
    HoltenRopeEffect effect;
    HoltenRopeAnchor anchor;
} HoltenRopeAttached;

/* A rope a RopeTie owns: tied between two actors' bones (rope_placeTiedEnd places its ends). */
struct TiedRope {
    /* 0x000 */ HoltenRopeEffect effect;
    /* 0x150 */ RopeTieLink link;
};

/* A rope whose type data at 0x150 points to the object it takes its color from. */
typedef struct {
    HoltenRopeEffect effect;
    Actor *colorSource;
} HoltenRopeTinted;

/* game/Rope/SwingRope/swingrope.c */
RopeRider *rope_createRider(Actor *obj, f32 radius, s32 extraSize);
s32 rope_canRide(Actor *obj, u8 *out);
s32 rope_updateRider(RopeRider *rider);
void rope_riderMessage(RopeRider *rider, s32 arg1, u8 arg2);
Rope *rope_create(void *spawn, s32 extraSize);
void rope_update(Rope *rope);
Gfx *rope_draw(Gfx *dl, Rope *rope, s32 arg2);
void rope_touchZone(Actor *obj, TriggerZone *zone);
void rope_grab(Actor *obj, u8 ropeId);
void rope_release(Actor *obj);
void rope_getHeightProgress(f32 *progress, Rope *rope, f32 x, f32 y, f32 z);
s32 rope_placeRider(RopeRider *rider);
Rope *rope_findById(u8 id);
void rope_message(Rope *rope, Actor **objs, u8 msg);

/* game/Rope/RopeTie/ropetie.c */
void rope_spawnTie(Actor *obj);
void rope_updateTie(RopeTie *self);
void rope_cleanupTie(RopeTie *self);
void rope_killTieParts(RopeTie *self);
void rope_destroyTie(RopeTie *self);
void rope_freeTie(RopeTie *self);
s32 rope_getTieState(RopeTie *self);
void rope_tieBetweenActors(RopeTie *self);
void rope_tieFromActor(RopeTie *self);
void rope_unlinkTiedRope(TiedRope *self);
Gfx *rope_drawLine(Gfx *gfx, RopeLine *self, s16 view);
void rope_lineMessage(RopeLine *self, s32 arg1, u8 event);
void rope_tieMessage(RopeTie *self, ActorSwapMsg *swap, u8 event);
s32 rope_tiedRopeEndpoint(TiedRope *self, Vec3f *pos, u8 isStart);
void rope_unlinkLine(RopeLine *self);
void rope_destroyLine(RopeLine *self);
void rope_freeLine(RopeLine *self);
void rope_tiedRopeMessage(TiedRope *self, s32 arg1, u8 event);

/* game/Effects/HoltenRope/holtenrope.c */
extern Gfx gRopeRenderSetupDl[]; /* render setup (object type table gObjectTypes) */
extern Gfx gRopeRenderResetDl[]; /* render teardown (object type table gObjectTypes) */
HoltenRopeEffect *rope_createEffect(HoltenRopeDesc *desc, f32 texScale, s32 extraSize, u8 arg3, s32 arg4);
void rope_updateEffect(HoltenRopeEffect *effect);
Gfx *rope_drawEffect(Gfx *gfx, HoltenRopeEffect *effect, s16 view);
void rope_effectMessage(HoltenRopeEffect *effect, s32 arg1, u8 arg2);
s32 rope_layStraight(HoltenRopeEffect *effect);
s32 rope_layHalfCircle(HoltenRopeEffect *effect);
s32 rope_actorEndpoint(HoltenRopeAttached *rope, Vec3f *pos, u8 isStart);
s32 rope_layHalfEllipse(HoltenRopeEffect *effect);
s32 rope_swingHalfEllipse(HoltenRopeEffect *effect);
s32 rope_placeTiedEnd(HoltenRopeEffect *effect, RopeTieLink *anchors, Vec3f *pos, u8 isStart);
s32 rope_spaceAlongX(HoltenRopeEffect *effect);
s32 rope_drawStyleWhite(HoltenRopeEffect *effect, s32 *mode0, s32 *mode1, s32 *primR, s32 *primG, s32 *primB, s32 *primA, s32 *envR, s32 *envG, s32 *envB, s32 *envA, s32 *cycleType, u8 *renderMode, u8 *unk13);
s32 rope_drawStyleTinted(HoltenRopeTinted *rope, s32 *mode0, s32 *mode1, s32 *primR, s32 *primG, s32 *primB, s32 *primA, s32 *envR, s32 *envG, s32 *envB, s32 *envA, s32 *cycleType, u8 *renderMode, u8 *unk13);
s32 rope_spaceFixedX(HoltenRopeEffect *effect);
void rope_cleanupEffect(HoltenRopeEffect *effect);
void rope_destroyEffect(HoltenRopeEffect *effect);
void rope_freeEffect(HoltenRopeEffect *effect);

#endif
