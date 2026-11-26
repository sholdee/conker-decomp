#ifndef CONKER_COLLISION_H
#define CONKER_COLLISION_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "collision": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Collision/MapBackground/mapbackground.c
 *   game/Collision/QuadTree/quadtree.c
 *   game/Collision/CylinderSets/cylindersets.c
 *   game/Collision/Probe/probe.c
 *   game/Collision/HitSphere/hitsphere.c
 *   game/Collision/RayCast/raycast.c
 *   game/Collision/Spawn/spawn.c
 *   game/Collision/PartSets/partsets.c (data only, linked after game/Collision/Spawn/spawn)
 *   game_D0F20.s .. game_DB730.s (hand-written asm)
 *   game/Collision/WobbleQuad/wobblequad.c
 *   game/Collision/ObjectRay/objectray.c
 *   game/Collision/GroundHeight/groundheight.c
 *   game/Collision/Floor/floor.c
 */

/* Types that these files define; the declarations below use them. */
typedef struct QuadNode QuadNode;
typedef struct TriggerVolume TriggerVolume;
struct Foo150AE280;
struct ShadowVtx;
struct Foo150AE36C;

/* A TriggerNode with a box volume (collision_addTriggerVolume). */
struct TriggerVolume {
    /* 0x00 */ TriggerNode node;
    /* 0x10 */ s16 depth;
    /* 0x12 */ s16 width;
    /* 0x14 */ s16 height;
    /* 0x16 */ u8 angle;
    /* 0x17 */ u8 pad17;
    /* 0x18 */ s16 *position;
    /* 0x1C */ s16 *size;
};

// One 0x10-byte part of a part set (gPartSets): a child object or an attachment.
typedef struct PartDef {
    /* 0x00 */ u8 model;    // model id of the child object or the attachment
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 type;     // 0: child object (collision_createChildObject), otherwise attach_add
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 flags;    // the attachment's flags
    /* 0x08 */ u16 data[3]; // the attachment's ownerStates
    /* 0x0E */ u8 parentSet; // the attachment's parentSet
    /* 0x0F */ u8 parentMtx; // the attachment's parentMtx
} PartDef;

typedef struct PartSet {
    /* 0x0 */ const PartDef *parts;
    /* 0x4 */ u8 count;
} PartSet;

/* game/Collision/MapBackground/mapbackground.c */
void collision_clearUvWobbles(void);
void collision_initMapBackground(s32 arg0);

/* game/Collision/QuadTree/quadtree.c */
void collision_addDlTris(Gfx *dl);
void collision_initTriArena(void);
void collision_allocQuadTree(void);
void func_15001B08(void);
QuadNode *collision_newQuadNode(void);
void collision_quadPutByte(u8 value);
void collision_quadPutHalf(u16 value);
void collision_buildQuadTree(s32 map, s16 *nodeCount, s16 *leafCount);
void collision_buildLeafQuadTree(s32 map);
QuadNode *collision_buildQuadNode(s32 x, s32 z, s32 halfX, s32 halfZ, s32 arg4, u16 *triList);
void collision_writeLeafTris(s32 minX, s32 maxX, s32 minZ, s32 maxZ, u16 *triList);
int collision_collectBoxTris(s32 minX, s32 maxX, s32 minZ, s32 maxZ, s32 *minY, s32 *maxY, u16 *triList, u16 **subList);
void collision_threadQuadTree(QuadNode *node, QuadNode *parentNext);
void func_150025FC(void);
void collision_beginMapTris(s32 map);
void collision_endMapTris(s32 map);
void collision_addDlTriCount(Gfx *dl);
void collision_allocMapTriTables(void);
s32 collision_countDlTris(Gfx *dl);
s32 collision_getMaxMapTriCount(void);
s32 collision_buildVertexRuns(s32 map);
void collision_setFloorReach(s32 map);
s32 collision_xorScramble(s32 value);
void collision_loadMapSettings(s32 map);
void func_1500310C(void);
extern u8 *gTriArenaCursor; /* triangle-data arena cursor; game_34ED0 clears the game .bss from here */

/* game/Collision/CylinderSets/cylindersets.c */
void collision_loadCylinderSets(s32 arg0);

/* game/Collision/Probe/probe.c */
void func_15044370(void);
s32 func_15044380(f32 x, f32 y, f32 z, Actor *obj, s32 arg4, s32 secondPass);
void collision_rotateTriVerts(s16 **vertices, Vec3f *out, f32 sinY, f32 cosY, f32 ox, f32 oy, f32 oz, s32 offset);
void func_15044658(void);
void collision_setProbeHeights(Actor *obj, f32 x, f32 y, f32 z);
TriggerVolume *collision_addTriggerVolume(s32 lifetime, s32 delay, s32 action, s32 depth, s32 width, s32 height, s32 angle,
                             s16 *position, s16 *size);
TriggerNode *collision_addTriggerNode(s32 size, s32 check, s32 lifetime, s32 delay, s32 action, s32 x, s32 y, s32 z);
void collision_updateTriggers(void);
s32 collision_checkPlayerInVolume(TriggerVolume *volume);
s32 collision_checkTrackedVolume(TriggerVolume *volume);
s32 func_15044D40(TriggerVolume *volume);
void func_15044DA0(void);
void func_15044DE8(void);
void func_15044E40(void);
void func_15044E88(void);
s32 collision_probeCeilingSet0(Vec3f *pos, f32 top, CollisionHit *hit);
s32 collision_probeFloorSet0(Vec3f *pos, f32 bottom, CollisionHit *hit);
s32 collision_probeCeilingSet0Cached(Vec3f *pos, f32 top, CollisionHit *hit);
s32 collision_probeCeilingSet3(Vec3f *pos, f32 top, CollisionHit *hit);
s32 collision_probeFloorSet3(Vec3f *pos, f32 bottom, CollisionHit *hit);
void collision_selectSurfaceSets(Vec3f *pos, u16 mask, s32 *setA, s32 *setB);
s32 collision_probeFloorSurfB(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 collision_probeFloorSurfBCached(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 collision_probeCeilingInSurfA(Vec3f *pos, f32 top, s32 *set, CollisionHit *hit);
s32 collision_probeFloorInSurfA(Vec3f *pos, f32 bottom, s32 *set, CollisionHit *hit);
s32 collision_probeCeilingInSurfB(Vec3f *pos, f32 top, s32 *set, CollisionHit *hit);
s32 collision_probeFloorInSurfB(Vec3f *pos, f32 bottom, s32 *set, CollisionHit *hit);
s32 collision_probeCeilingSurfAB(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit);
s32 collision_probeFloorSurfAB(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 collision_probeCeilingSurfABSet0(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit);
s32 collision_probeFloorSurfABSet0(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 collision_probeCeilingSurfABSet0Cached(Vec3f *pos, u16 mask, f32 top, CollisionHit *hit);
s32 collision_probeFloorSurfABSet0Cached(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 collision_probeFloorWithSet3(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 collision_probeFloorWithSet3Cached(Vec3f *pos, u16 mask, f32 bottom, CollisionHit *hit);
s32 collision_retestCachedFloor(Vec3f *pos, f32 bottom, CollisionHit *hit);
s32 collision_retestCachedCeiling(Vec3f *pos, f32 top, CollisionHit *hit);
void collision_initHitFromObject(CollisionHit *hit, Actor *obj);
void collision_initHitFromRay(CollisionHit *hit, RayHit *src);
/* Rare's copies of the SDK's guLookAtF, guLookAt, guLookAtReflectF and guLookAtReflect (gu.h). Unlike the SDK's,
 * they guard against zero-length vectors instead of dividing by zero. */
void guLookAtF(MtxF mf, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp);
void guLookAt(Mtx *m, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp);
void guLookAtReflectF(MtxF mf, LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp,
                   f32 zUp);
void guLookAtReflect(Mtx *m, LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp,
                   f32 zUp);
extern f32 D_800CBD90; // ledge probe results (written by func_150AC3E4, game_D86A0.s)
extern f32 D_800CBD94;
extern f32 D_800CBD98;
extern u16 gActorTouchedObject[25]; // per movement_obj: 1 + the world object it touches, 0 if none
extern u8 D_800CBDD2;
extern u8 D_800CBDD3;
extern u8 D_800CBDD4;
extern f32 D_800CBDF4; // ledge heights, -32768.0f if none
extern f32 D_800CBDF8;
extern f32 D_800CBDFC;
extern TriggerNode *gTriggerNodes; // timed trigger nodes (collision_addTriggerNode)

/* game/Collision/HitSphere/hitsphere.c */
s32 collision_testPointHitSpheres(Actor *self, Actor *obj, s32 type, f32 dist, f32 radius, s32 arg5);
s32 collision_getHitTableIndex(Actor *obj);
s32 collision_rayHitSpheres(Actor *obj, Vec3f *rayOrigin, Vec3f *rayDir, Vec3f *outNear, Vec3f *outFar, Vec3f *outCentre,
                  f32 *outDist, s32 *outIndex, s32 *outBone, s32 *outPart, s16 *outOffset);
s32 collision_findHitSphereTarget(Actor *self, f32 dist, f32 radius, Actor **outTarget, s32 arg4, s32 model);

/* game/Collision/RayCast/raycast.c */
void collision_castRay(Actor *self, f32 originX, f32 originY, f32 originZ, f32 dirX, f32 dirY, f32 dirZ, RayHit *out,
                   f32 maxDist, s32 skipWorld, s32 skipObjs, s32 skipModels, s8 filter, void *filterArg, s32 ignoreMask);
s32 collision_rayCylinder(Vec3f *origin, Vec3f *dir, Actor *obj, Vec3f *outNear, Vec3f *outFar, f32 *outNearDist,
                  f32 *outFarDist);
void func_15081E0C(Actor *obj, u16 part, u8 skip);
s32 collision_applyRayHit(Actor *self, RayHit *hit, s32 kind);

/* game/Collision/Spawn/spawn.c */
s32 collision_trySpawnRecord(s32 spawnIdx, s32 arg1);
void collision_trySpawnRecords(s32 arg0, s32 arg1, s32 firstRange);
void func_150829D8(Actor *obj);
s32 collision_spawnObject(SpawnRecord *spawn, s32 spawnIdx, s32 arg2, s32 arg3, s32 slot);
void func_150832AC(Actor *obj);
void func_15083384(Actor *obj, s32 arg1);
void *collision_createPartSet(Actor *parent, s32 setId, f32 arg2, s32 arg3);
void collision_removePartSet(Actor *parent, s32 setId);
void collision_setObjectModel(s32 objIdx, s32 modelId, s32 arg2);
s32 collision_allocAnimState(Actor *obj, s32 arg1, s32 arg2, f32 arg3);
void collision_applyModelDefaults(Actor *obj, s32 modelId, SpawnRecord *spawn);
Actor *collision_createChildObject(Actor *parent, s32 arg1, s32 modelId, s32 arg3, s32 state, s32 arg5, s32 inheritScale, s32 arg7,
                     f32 arg8);
s32 collision_getFreeUniqueId(void);
s32 collision_findSpawnById(u8 id);
Actor *collision_findObjectById(u8 id);
s32 collision_findObjectSlotById(u8 id);
s32 func_15084000(s32 id);
s32 collision_initModelData(Actor *obj, s32 model);
void collision_releaseAllModelData(Actor *obj);
void collision_releaseModelData(Actor *obj, s32 model);
void collision_loadSpawnModel(SpawnRecord *spawn, s32 arg1, s32 arg2);
void collision_freeObjectModels(Actor *self);
s32 collision_getModelId(Actor *obj);
s32 collision_getLodModelId(Actor *obj, s32 *outIndex);
void collision_updateModelUnload(void);
void func_15084C30(Actor *obj);
s32 func_15084CB0(s32 modelId);
extern u8 *gModelData[187]; // per model id: model data (just past its header)
extern ModelDef *gModelDefs[187];
extern u8 D_800D1F80[187];
extern u8 gModelUnloadTimers[187]; // per model id counter
extern SpawnRecord *gSpawnRecords;
extern u8 gSpawnRecordCount;
extern u8 gSpawnFirstCount;
extern PathPoint **gSpawnPaths;
extern u8 *gSpawnPathCounts;
extern u8 *gSpawnInUse;
extern u16 *gSpawnDelays;

/* game/Collision/PartSets/partsets.c */
/* The object filters: s32 (Actor *obj, Actor *self, arg); declared without a prototype, as their
   second and third parameters differ. */
extern s32 (*gObjectFilters[7])();
extern const u8 *gSubModelLists[6]; // per ModelDef.subModelList: the sub-model list
extern PartSet gPartSets[175]; // per ModelDef.partSet (1-based)
extern const u8 gModel0ExpressionActions[5];  // the expression actions of model 0

/* game_D0F20.s: the ground probe. func_150A3A70 gathers the candidate hits under (x, z) into
 * D_800D3300 and returns their count; func_150A3FC4 gives the height of a triangle at (x, z), from
 * its three vertex pointers or, when triVerts is NULL, from the vertices in tri. */
s32 func_150A3A70(s32 x, s32 z);
s32 func_150A3FC4(f32 x, f32 z, Vec3s **triVerts, Vec3s *tri, f32 *y);

/* game_D1890.s */
s32 func_150A43E0(s32 x, s32 z, s32 count, u16 *props);

/* game_D19A0.s */
void func_150A44F0(s32 count, u16 *props, s32 arg2);
/* (s32 map); declared without a prototype: collision_selectMap (game/Collision/Floor/floor.c) passes its own argument on */
void func_150A49F4();

/* game_D1FB4.s */
void func_150A4B04(Actor *actor);

/* game_D2450.s */
s32 func_150A4FA0(s32 x, s32 z);

/* game_D2570.s */
Gfx *func_150A50C0(Gfx *dl, Gfx *gfx, MtxF viewMtx, s32 arg3, s32 arg4, s32 viewMask);

/* game_D260C.s */
Gfx *func_150A5378(Gfx *dl, Gfx *gfx, MtxF viewMtx, s32 arg3, s32 arg4, s32 viewMask);

/* game_D3040.s */
struct ShadowVtx *func_150A5B90(u32 *tris, struct ShadowVtx *out, f32 *mtx, struct ShadowVtx *end);
struct ShadowVtx *func_150A5E44(u16 *props, struct ShadowVtx *out, f32 *mtx, struct ShadowVtx *end);

/* game_D36C0.s */
s32 func_150A6210(MtxF mtx, f32 x, f32 y, f32 z, f32 width, f32 height, f32 arg6);

/* game_D3810.s: func_150A6360 tests a cylinder against a view's frustum and returns its distance */
s32 func_150A6360(RenderView *view, MtxF viewMtx, f32 x, f32 y, f32 z, f32 radius, f32 height, f32 farDist);
void func_150A64C8(s16 x, s16 z, s32 *groups, s32 y);

/* game_D39B0.s */
s32 func_150A6500(s16 x, s16 z, s32 *groups, u16 mask);

/* game_D3A18.s */
s32 func_150A6568(s16 x, s16 z, s32 *groups, u16 group, s32 ix, s32 iz, s32 minY, s32 maxY);
void func_150A6760(s32 index);

/* game_D4450.s: wave deformers of vertex lists */
void func_150A6FA0(Vtx *dst, u16 count, WaveParams *params, s8 *phase, f32 *weight, Vtx *src);
void func_150A70C0(Vtx *dst, u16 count, WaveParams *params, s8 *phase, Vtx *src);
void func_150A71C8(Vtx *dst, u16 count, WaveParams *params, u8 *unk, s8 *phase, Vtx *src);
void func_150A7360(Vtx *dst, u16 count, WaveParams *params, u8 *unk, s8 *phase, f32 *weight, Vtx *src);
void func_150A751C(Vtx *dst, u16 count, WaveParams *params, u8 *angle, s8 *phase, f32 *weight, Vtx *src);

/* game_D4E10.s: transforms the point (x, y, z) by mtx */
void func_150A7960(MtxF mtx, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/* game_D4EB0.s: the same with the w of the result */
void func_150A7A00(MtxF mtx, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ, f32 *outW);

/* game_D4EF8.s: out = a * b */
void func_150A7A48(MtxF a, MtxF b, MtxF out);

/* game_D5030.s: an identity Mtx */
void func_150A7B80(Mtx *m);

/* game_D51B0.s: a translation Mtx */
void func_150A7D00(Mtx *m, f32 x, f32 y, f32 z);

/* game_D5250.s */
void func_150A7DA0(MtxF mtx, f32 x, f32 y, f32 z);

/* game_D52A0.s: a rotation Mtx, angles in degrees */
void func_150A7DF0(Mtx *m, f32 rx, f32 ry, f32 rz);

/* game_D5500.s */
void func_150A8050(MtxF mtx, f32 rx, f32 ry, f32 rz);

/* game_D5680.s */
/* cursor: the address of a matrix cursor (an Mtx * or a byte pointer), moved past the numParts float
   matrices that it writes */
void func_150A81D0(void *cursor, MtxF transform, AnimInstance *anim, s32 arg3, s32 numParts, s16 *arg5, u8 *remap, s32 scale);

/* game_D6E34.s: converts count float matrices (MtxF) in place into Mtx */
void func_150A9984(void *mtx, s32 count);

/* game_D6FBC.s */
void func_150A9B0C(MtxF mtx, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);

/* game_D8248.s */
s32 func_150AAD98(Tri3f tri);

/* game_D86A0.s */
s32 func_150AB1F0(f32 x, f32 y, f32 z, Actor *actor, s32 arg4);
void func_150AC3E4(f32 x, f32 y, f32 z, Actor *actor, s32 arg4);

/* game_D9E70.s: casts the ray from (x, y, z) along (dx, dy, dz), up to maxDist when it is not 0. Each
 * output pointer can be NULL. */
s32 func_150AC9C0(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, void *outTriVerts, CollisionTri *outTri, f32 *hitX,
                  f32 *hitY, f32 *hitZ, f32 *outDist, s32 *outObject, s32 *outSurface, f32 maxDist);

/* game_DAC20.s */
void func_150AD770(void);

/* game_DAC30.s */
f32 func_150AD780(f32 x);
f32 func_150AD78C(f32 x);

/* game_DAD60.s: out = a x b */
void func_150AD8B0(f32 *a, f32 *b, f32 *out);

/* game_DADB0.s */
f32 func_150AD900(f32 *a, f32 *b);
f32 func_150AD930(f32 *v);

/* game_DAE10.s */
s32 func_150AD960(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* game_DAE50.s */
u32 func_150ADA20(void);
/* The same random function at the same address, under a u8 return type. 14 files were compiled with
 * that view: they use the result as a byte without masking it. */
u8 func_150ADA20_u8(void);
f32 func_150ADA68(void);
void func_150ADACC(s32 seed);

/* game_DAFA0.s */
void func_150ADAF0(IdRecord *node, s32 mode);

/* game_DB730.s */
void func_150AE280(void);

/* game/Collision/WobbleQuad/wobblequad.c */
s32 func_150AE2A0(struct Foo150AE280 *arg0);
s32 func_150AE35C(s32 arg0);
s16 *func_150AE36C(struct Foo150AE36C *arg0, s16 arg1);

/* game/Collision/ObjectRay/objectray.c */
s32 collision_castRayAtObjects(Actor *source, f32 fromX, f32 fromY, f32 fromZ, f32 toX, f32 toY, f32 toZ, s32 skipIndex, s32 effect);

/* game/Collision/GroundHeight/groundheight.c */
void func_151045E0(void *arg0, s32 arg1, f32 arg2);
s32 func_151045F4(s32 arg0, s32 arg1);
void func_15104608(f32 x, f32 y, f32 z, s32 arg3);
s32 func_15104620(s32 arg0, s32 arg1);
s32 collision_getFloorNearY(f32 x, f32 z, f32 y, f32 *out);

/* game/Collision/Floor/floor.c */
extern void *gMapTriTables[4];
extern void *gMapTriXZBounds[4];
extern void *gMapTriYBounds[4];
extern struct QuadNode *gMapQuadTrees[4];
extern u8 gFloorProbeTypeEnabled[4];
s32 collision_isHitFacingUp(s32 index);
void collision_findFloorInRange(void **outTri, Vec3s *outVerts, f32 *outFloorY, f32 *outPropFloorY, u32 *outFlags, s16 *outType,
                   f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj, f32 minY, f32 maxY);
void collision_findFloor(void **outTri, Vec3s *outVerts, f32 *outFloorY, f32 *outPropFloorY, u32 *outFlags, s16 *outType,
                   f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj);
void collision_findFloorEx(void **outTri, Vec3s *outVerts, f32 *outFloorY, f32 *outPropFloorY, u32 *outFlags, s16 *outType,
                   f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj, f32 unused12, f32 unused13, s32 noFallback);
void collision_findFloorCore(void **outTri, Vec3s *outVerts, s16 (*outPropVerts)[3], f32 *outFloorY, f32 *outPropFloorY,
                   u32 *outFlags, s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *object, f32 minY,
                   f32 maxY, s32 noFallback);
f32 collision_getPropSurfaceHeight(f32 x, f32 y, f32 z);
s32 collision_findZoneLeavesNear(s32 x, s32 z, s32 radius, ZoneNode **outLeaves);
void collision_selectMap();
void collision_getTriSlope(void *tri, void *verts, f32 *outDirX, f32 *outDirZ, f32 *unused, f32 *outSlopeX, f32 *outSlopeZ);
s32 collision_getSurfaceType(s32 flags);
s32 collision_findMeshFloorNear(s32 x, s32 y, s32 z, s32 *outTri);
void collision_updateCameraZone(s32 index);
ZoneNode *collision_findZoneLeaf(s32 x, s32 z);
s32 collision_getZoneLeafIndex(ZoneNode *zone);

#endif
