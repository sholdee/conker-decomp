#ifndef CONKER_ROPE_H
#define CONKER_ROPE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "rope": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_1D9F10.c
 *   game_1DF510.c
 *   game_1E0560.c
 */

/* Template copied over HoltenRopeEffect 0x10-0x47 when a rope is created (func_151B30B0). */
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

/* The 0x30-byte link object that moves an owner along its rope (func_151ACA60). */
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
    /* 0x0D */ u8 state;    // last func_151B22F4 result, 0 before the first update
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
    /* 0x02C */ s8 startCallback;   // index into D_8008FAF0, -1 for none
    /* 0x02D */ s8 endCallback;     // index into D_8008FAF0, -1 for none
    /* 0x02E */ u8 drawStyle;       // index into D_8008FB10
    /* 0x02F */ u8 pad2F;
    /* 0x030 */ f32 halfWidth;
    /* 0x034 */ s8 updateCallback;  // index into D_8008FAF8, -1 for none
    /* 0x035 */ u8 pad35[3];
    /* 0x038 */ f32 length;
    /* 0x03C */ f32 accelScale;
    /* 0x040 */ f32 lagScale;
    /* 0x044 */ u8 type;            // index into D_8008FB68 and D_8008FB70
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

/* A rope a RopeTie owns: tied between two actors' bones (func_151B47D8 places its ends). */
struct TiedRope {
    /* 0x000 */ HoltenRopeEffect effect;
    /* 0x150 */ RopeTieLink link;
};

/* A rope whose type data at 0x150 points to the object it takes its color from. */
typedef struct {
    HoltenRopeEffect effect;
    Actor *colorSource;
} HoltenRopeTinted;

/* game_1D9F10.c */
RopeRider *func_151ACA60(Actor *obj, f32 radius, s32 extraSize);
s32 func_151ACB38(Actor *obj, u8 *out);
s32 func_151ACB60(RopeRider *rider);
void func_151ACB94(RopeRider *rider, s32 arg1, u8 arg2);
Rope *func_151ACBD4(void *spawn, s32 extraSize);
void func_151AD174(Rope *rope);
Gfx *func_151AD92C(Gfx *dl, Rope *rope, s32 arg2);
void func_151AE06C(Actor *obj, TriggerZone *zone);
void func_151AE0E4(Actor *obj, u8 ropeId);
void func_151AE264(Actor *obj);
void func_151AE2BC(f32 *progress, Rope *rope, f32 x, f32 y, f32 z);
s32 func_151AE3A8(RopeRider *rider);
Rope *func_151AE590(u8 id);
void func_151AE640(Rope *rope, Actor **objs, u8 msg);

/* game_1DF510.c */
void func_151B2060(Actor *obj);
void func_151B2100(RopeTie *self);
void func_151B220C(RopeTie *self);
void func_151B222C(RopeTie *self);
void func_151B229C(RopeTie *self);
void func_151B22C8(RopeTie *self);
s32 func_151B22F4(RopeTie *self);
void func_151B2348(RopeTie *self);
void func_151B2690(RopeTie *self);
void func_151B2950(TiedRope *self);
Gfx *func_151B2974(Gfx *gfx, RopeLine *self, s16 view);
void func_151B2EC4(RopeLine *self, s32 arg1, u8 event);
void func_151B2F04(RopeTie *self, ActorSwapMsg *swap, u8 event);
s32 func_151B2FA0(TiedRope *self, Vec3f *pos, u8 isStart);
void func_151B2FD0(RopeLine *self);
void func_151B2FE8(RopeLine *self);
void func_151B3014(RopeLine *self);
void func_151B3040(TiedRope *self, s32 arg1, u8 event);

/* game_1E0560.c */
extern Gfx D_8008FB18[]; /* render setup (object type table gObjectTypes) */
extern Gfx D_8008FB40[]; /* render teardown (object type table gObjectTypes) */
HoltenRopeEffect *func_151B30B0(HoltenRopeDesc *desc, f32 texScale, s32 extraSize, u8 arg3, s32 arg4);
void func_151B3184(HoltenRopeEffect *effect);
Gfx *func_151B32C8(Gfx *gfx, HoltenRopeEffect *effect, s16 view);
void func_151B3A34(HoltenRopeEffect *effect, s32 arg1, u8 arg2);
s32 func_151B3A7C(HoltenRopeEffect *effect);
s32 func_151B3CF0(HoltenRopeEffect *effect);
s32 func_151B3F28(HoltenRopeAttached *rope, Vec3f *pos, u8 isStart);
s32 func_151B3FDC(HoltenRopeEffect *effect);
s32 func_151B42A4(HoltenRopeEffect *effect);
s32 func_151B47D8(HoltenRopeEffect *effect, RopeTieLink *anchors, Vec3f *pos, u8 isStart);
s32 func_151B48DC(HoltenRopeEffect *effect);
s32 func_151B498C(HoltenRopeEffect *effect, s32 *mode0, s32 *mode1, s32 *primR, s32 *primG, s32 *primB, s32 *primA, s32 *envR, s32 *envG, s32 *envB, s32 *envA, s32 *cycleType, u8 *renderMode, u8 *unk13);
s32 func_151B4A14(HoltenRopeTinted *rope, s32 *mode0, s32 *mode1, s32 *primR, s32 *primG, s32 *primB, s32 *primA, s32 *envR, s32 *envG, s32 *envB, s32 *envA, s32 *cycleType, u8 *renderMode, u8 *unk13);
s32 func_151B4B78(HoltenRopeEffect *effect);
void func_151B4C1C(HoltenRopeEffect *effect);
void func_151B4C6C(HoltenRopeEffect *effect);
void func_151B4C98(HoltenRopeEffect *effect);

#endif
