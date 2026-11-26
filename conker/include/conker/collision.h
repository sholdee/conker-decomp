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
 *   game/Collision/GroundQuery/groundquery.s .. game/Script/TaskExit/taskexit.s (hand-written asm)
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
s32 collision_resolveActorWalls(f32 x, f32 y, f32 z, Actor *obj, s32 arg4, s32 secondPass);
void collision_rotateTriVerts(s16 **vertices, Vec3f *out, f32 sinY, f32 cosY, f32 ox, f32 oy, f32 oz, s32 offset);
void func_15044658(void);
void collision_setProbeHeights(Actor *obj, f32 x, f32 y, f32 z);
TriggerVolume *collision_addTriggerVolume(s32 lifetime, s32 delay, s32 action, s32 depth, s32 width, s32 height, s32 angle,
                             s16 *position, s16 *size);
TriggerNode *collision_addTriggerNode(s32 size, s32 check, s32 lifetime, s32 delay, s32 action, s32 x, s32 y, s32 z);
void collision_updateTriggers(void);
s32 collision_checkPlayerInVolume(TriggerVolume *volume);
s32 collision_checkTrackedVolume(TriggerVolume *volume);
s32 collision_runVolumeAttack(TriggerVolume *volume);
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
extern f32 gLedgeZ; // ledge probe results (written by collision_findLedge, game/Collision/WallCollide/wallcollide.s)
extern f32 gLedgeAngle1;
extern f32 gLedgeAngle2;
extern u16 gActorTouchedObject[25]; // per movement_obj: 1 + the world object it touches, 0 if none
extern u8 D_800CBDD2;
extern u8 D_800CBDD3;
extern u8 D_800CBDD4;
extern f32 gLedgeHeight1; // ledge heights, -32768.0f if none
extern f32 gLedgeHeight2;
extern f32 gLedgeX;
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
void collision_allocActorLights(Actor *obj);
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
s32 collision_findSlotBySpawnIndex(s32 id);
s32 collision_initModelData(Actor *obj, s32 model);
void collision_releaseAllModelData(Actor *obj);
void collision_releaseModelData(Actor *obj, s32 model);
void collision_loadSpawnModel(SpawnRecord *spawn, s32 arg1, s32 arg2);
void collision_freeObjectModels(Actor *self);
s32 collision_getModelId(Actor *obj);
s32 collision_getLodModelId(Actor *obj, s32 *outIndex);
void collision_updateModelUnload(void);
void func_15084C30(Actor *obj);
s32 collision_getSpawnModelTypeIndex(s32 modelId);
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

/* game/Collision/GroundQuery/groundquery.s: the ground probe. collision_gatherGroundTris gathers the candidate hits under (x, z) into
 * gGroundCandidates and returns their count; collision_getTriHeightAt gives the height of a triangle at (x, z), from
 * its three vertex pointers or, when triVerts is NULL, from the vertices in tri. */
s32 collision_gatherGroundTris(s32 x, s32 z);
s32 collision_getTriHeightAt(f32 x, f32 z, Vec3s **triVerts, Vec3s *tri, f32 *y);

/* game/Collision/PropGround/propground.s */
s32 collision_gatherPropGroundTris(s32 x, s32 z, s32 count, u16 *props);

/* game/Collision/PropTris/proptris.s */
void collision_transformPropTris(s32 count, u16 *props, s32 arg2);
/* (s32 map); declared without a prototype: collision_selectMap (game/Collision/Floor/floor.c) passes its own argument on */
void collision_setCurMap();

/* game/Collision/ActorHitMesh/actorhitmesh.s */
void collision_updateActorHitMesh(Actor *actor);

/* game/Collision/ActorGround/actorground.s */
s32 collision_gatherActorGroundTris(s32 x, s32 z);

/* game/Gfx/CullDl/culldl.s */
Gfx *gfx_copyDlCulled(Gfx *dl, Gfx *gfx, MtxF viewMtx, s32 arg3, s32 arg4, s32 viewMask);

/* game/Gfx/CullLevelDl/cullleveldl.s */
Gfx *gfx_copyLevelDlCulled(Gfx *dl, Gfx *gfx, MtxF viewMtx, s32 arg3, s32 arg4, s32 viewMask);

/* game/Shadow/ShadowFaces/shadowfaces.s */
struct ShadowVtx *shadow_projectLevelFaces(u32 *tris, struct ShadowVtx *out, f32 *mtx, struct ShadowVtx *end);
struct ShadowVtx *shadow_projectPropFaces(u16 *props, struct ShadowVtx *out, f32 *mtx, struct ShadowVtx *end);
extern f32 gShadowProjScaleX;
extern f32 gShadowProjScaleY;
extern f32 D_800D3688; /* shadow near plane (light-space z) */
extern f32 D_800D368C;
extern MeshTri *gShadowFaceTris;
extern u32 *gShadowFaceBitsEnd;

/* game/View/QuadInView/quadinview.s */
s32 view_getQuadViewDepth(MtxF mtx, f32 x, f32 y, f32 z, f32 width, f32 height, f32 arg6);

/* game/View/CylinderInView/cylinderinview.s: view_testCylinderInView tests a cylinder against a view's frustum and returns its distance */
s32 view_testCylinderInView(RenderView *view, MtxF viewMtx, f32 x, f32 y, f32 z, f32 radius, f32 height, f32 farDist);
void collision_findPropsBelow(s16 x, s16 z, s32 *groups, s32 y);

/* game/Collision/PropQuery/propquery.s */
s32 collision_findPropsAt(s16 x, s16 z, s32 *groups, u16 mask);

/* game/Collision/PropBoxQuery/propboxquery.s */
s32 collision_findPropsInBox(s16 x, s16 z, s32 *groups, u16 group, s32 ix, s32 iz, s32 minY, s32 maxY);
void func_150A6760(s32 index);

/* game/Prop/WaveVertices/wavevertices.s: wave deformers of vertex lists */
void prop_waveVerticesWeighted(Vtx *dst, u16 count, WaveParams *params, s8 *phase, f32 *weight, Vtx *src);
void prop_waveVertices(Vtx *dst, u16 count, WaveParams *params, s8 *phase, Vtx *src);
void prop_waveVerticesMode1(Vtx *dst, u16 count, WaveParams *params, u8 *unk, s8 *phase, Vtx *src);
void prop_waveVerticesMode1Weighted(Vtx *dst, u16 count, WaveParams *params, u8 *unk, s8 *phase, f32 *weight, Vtx *src);
void prop_waveVerticesMode2Weighted(Vtx *dst, u16 count, WaveParams *params, u8 *angle, s8 *phase, f32 *weight, Vtx *src);

/* game/Math/TransformPoint/transformpoint.s: transforms the point (x, y, z) by mtx */
void math_transformPoint(MtxF mtx, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/* game/Math/TransformPointW/transformpointw.s: the same with the w of the result */
void math_transformPointW(MtxF mtx, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ, f32 *outW);

/* game/Math/MtxMulF/mtxmulf.s: out = a * b */
void math_mtxMulF(MtxF a, MtxF b, MtxF out);

/* game/Math/MtxIdentity/mtxidentity.s: an identity Mtx */
void math_mtxIdentity(Mtx *m);

/* game/Math/TranslateMtx/translatemtx.s: a translation Mtx */
void math_mtxTranslate(Mtx *m, f32 x, f32 y, f32 z);

/* game/Math/MtxTranslateF/mtxtranslatef.s */
void math_mtxTranslateF(MtxF mtx, f32 x, f32 y, f32 z);

/* game/Math/MtxRotateRPY/mtxrotaterpy.s: a rotation Mtx, angles in degrees */
void math_mtxRotateRPY(Mtx *m, f32 rx, f32 ry, f32 rz);

/* game/Math/MtxRotateRPYF/mtxrotaterpyf.s */
void math_mtxRotateRPYF(MtxF mtx, f32 rx, f32 ry, f32 rz);

/* game/Anim/BoneMtxState/bonemtxstate.s */
extern s32 D_80088554;
extern s32 D_80088558;
extern s32 gAnimModAngle;

/* game/Anim/BoneMatrices/bonematrices.s */
/* cursor: the address of a matrix cursor (an Mtx * or a byte pointer), moved past the numParts float
   matrices that it writes */
void anim_buildBoneMatrices(void *cursor, MtxF transform, AnimInstance *anim, s32 arg3, s32 numParts, s16 *arg5, u8 *remap, s32 scale);

/* game/Math/MtxFToL/mtxftol.s: converts count float matrices (MtxF) in place into Mtx */
void math_mtxFToLInPlace(void *mtx, s32 count);

/* game/Math/MtxRotScaleF/mtxrotscalef.s */
void math_mtxRotScaleF(MtxF mtx, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);

/* game/Math/TriBoxOverlap/triboxoverlap.s */
s32 math_triBoxOverlap(Tri3f tri);

/* game/Collision/WallCollide/wallcollide.s */
s32 collision_resolveMapWalls(f32 x, f32 y, f32 z, Actor *actor, s32 arg4);
void collision_findLedge(f32 x, f32 y, f32 z, Actor *actor, s32 arg4);

/* game/Collision/RayWorld/rayworld.s: casts the ray from (x, y, z) along (dx, dy, dz), up to maxDist when it is not 0. Each
 * output pointer can be NULL. */
s32 collision_castRayWorld(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, void *outTriVerts, CollisionTri *outTri, f32 *hitX,
                  f32 *hitY, f32 *hitZ, f32 *outDist, s32 *outObject, s32 *outSurface, f32 maxDist);

/* game/Debug/FatalTrap/fataltrap.s */
void debug_fatalTrap(void);

/* game/Math/SinCos/sincos.s */
f32 math_cos(f32 x);
f32 math_sin(f32 x);

/* game/Math/Cross3/cross3.s: out = a x b */
void math_cross3(f32 *a, f32 *b, f32 *out);

/* game/Math/DotLength/dotlength.s */
f32 math_dot3(f32 *a, f32 *b);
f32 math_length3(f32 *v);

/* game/Math/Distance2D/distance2d.s */
s32 math_distance2D(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* game/Math/Rand64/rand64.s */
s32 math_distance3D(s32 dx, s32 dy, s32 dz);
u32 math_random(void);
/* The same random function at the same address, under a u8 return type. 14 files were compiled with
 * that view: they use the result as a byte without masking it. */
u8 math_randomU8(void);
f32 math_randomFloat(void);
void math_seedRandom(s32 seed);

/* game/Script/ScriptInterp/scriptinterp.s */
void script_runTask(IdRecord *node, s32 mode);

/* game/Script/TaskExit/taskexit.s */
void script_exitTask(void);

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
extern s32 gMapTriCounts[4];
extern s16 gQuadNodeBitmapStride;
extern s16 gMap0QuadNodeCount;
extern s16 gMap0QuadLeafCount;
extern s16 gViewCameraZones[4]; // per-view camera zone
extern s32 gMapTriCollected;
extern MeshTri *gCurMapTris;
extern struct XZRange *gCurMapTriXZBounds;
extern struct YRange *gCurMapTriYBounds;
extern ZoneNode *gCurMapQuadTree; // zone tree root
extern s32 gCurMapTriCount;
extern s32 gCurMap;
extern u32 *gMeshTriMaterials; // per world-mesh triangle: material / surface flags
extern u8  gMeshBuilt;
extern u8  D_800DBE63;
extern f32 gFloorQueryReach;
extern f32 gFloorQueryX;
extern f32 gFloorQueryY;
extern f32 gFloorQueryZ;
extern f32 gRayProbeBottom;
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
