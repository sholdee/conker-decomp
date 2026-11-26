#ifndef CONKER_GFX_H
#define CONKER_GFX_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "gfx": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Gfx/DisplayListAlloc/displaylistalloc.c
 *   game/Gfx/Turret/turret.c
 *   game/Gfx/WorldObject/worldobject.c
 */

/* Turret settings (WorldObject.moveData.data); angles in degrees. */
typedef struct Turret15112520Cfg {
    /* 0x00 */ f32 speedNear;
    /* 0x04 */ f32 speedFar;
    /* 0x08 */ f32 maxStep;
    /* 0x0C */ f32 yawMax;
    /* 0x10 */ f32 yawMin;
    /* 0x14 */ f32 yawMargin;
    /* 0x18 */ f32 pitchMax;
    /* 0x1C */ f32 pitchMin;
    /* 0x20 */ f32 pitchMargin;
    /* 0x24 */ f32 baseYaw;
    /* 0x28 */ f32 basePitch;
    /* 0x2C */ f32 range;
    /* 0x30 */ u16 sfx;
    /* 0x32 */ u16 sfxCooldown; // frames before the motor sound can start again
} Turret15112520Cfg;

/* A sound started by gfx_playObjectSound that follows its object. */
typedef struct ObjSound {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x10];
    /* 0x18 */ WorldObject *obj;
    /* 0x1C */ s32 positioned;
} ObjSound;

/* game/Gfx/DisplayListAlloc/displaylistalloc.c */
void gfx_allocDisplayLists(void);

/* game/Gfx/Turret/turret.c */
void gfx_aimTurret(WorldObject *turret);
extern Turret15112520Cfg gTurretDefaultCfg;

/* game/Gfx/WorldObject/worldobject.c */
void gfx_buildDrawLists(s32 player);
void gfx_flagObjectUpdates(void);
void gfx_updateObjectMatrices(void);
Gfx *gfx_drawObjectList(Gfx *gfx, s32 translucent, s16 view);
Gfx *gfx_drawObject(Gfx *gfx, WorldObject *obj, Mtx *mtx, s32 view, s32 distance, s32 pass);
void gfx_updateObjectShadows(void);
void gfx_runObjectUpdates(s32 arg0);
s32 gfx_isObjectTouchedBy(WorldObject *obj, s32 actor);
s32 gfx_getTouchingActor(WorldObject *obj);
void gfx_dispatchObjectContacts(void);
void gfx_carryPointWithObject(s32 objIndex, f32 *x, f32 *y, f32 *z);
void gfx_carryActorOnObject(Actor *actor, s32 objIndex);
void gfx_getObjectRotationMtx(MtxF mtx, WorldObject *obj);
void gfx_getObjectModelMtx(MtxF mtx, WorldObject *obj);
WorldObject *gfx_findObjectById(u8 id);
void gfx_freeHiddenObjectBlocks(void);
void gfx_removeObject(u32 index);
s32 gfx_objectSoundCallback(ObjSound *sound, s32 arg1, s32 *follow, s32 arg3);
void gfx_playObjectSound(WorldObject *obj, s32 sfx, s32 arg2, s16 arg3, s16 arg4, s32 flags);
void gfx_startObjectSound(WorldObject *obj, s32 arg1, s32 arg2);
void func_15114F44(s32 arg0, s32 arg1, s32 arg2);
extern DrawEntry **gViewDrawListPtrs[4]; // per-view draw lists
extern Mtx **gObjectMtxBuffers[2]; // per frame buffer: one Mtx per object
extern DrawEntry *gViewDrawLists[4];
extern Mtx *gObjectMtxsA;
extern Mtx *gObjectMtxsB;
extern u16 gViewDrawCounts[4]; // per view: entries in gViewDrawListPtrs
extern s32 gWorldObjectCount; // number of db_Object_array entries
extern WorldObject *db_Object_array; // original name (Oct 2000 debugger); gWorldObjectCount entries
extern void **gObjectBlocks; // per object: handle released with heap_free
extern u8 *gObjectBlockDelays; // per object: frames to keep the handle once hidden
extern u8 gUngroupedObjectCount;
extern MtxF D_800DBF08[2]; // per frame buffer
extern u8 D_800DBF88;
extern Gfx *D_800DBF8C; // display list of object D_800DBF88 (func_15004574)
extern Vtx *D_800DBF90; // its source vertices
extern s32 *gWorldObjectContacts; // per object: bit N set when actor movement_obj[N] touches it
extern s32 gGroupedObjectCount; // grouped objects, kept at the end of db_Object_array

#endif
