#ifndef CONKER_GFX_H
#define CONKER_GFX_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "gfx": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_43330.c
 *   game_13F9D0.c
 *   game_13FF30.c
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

/* A sound started by func_15114D24 that follows its object. */
typedef struct ObjSound {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u8 pad8[0x10];
    /* 0x18 */ WorldObject *obj;
    /* 0x1C */ s32 positioned;
} ObjSound;

/* game_43330.c */
void func_15015E80(void);

/* game_13F9D0.c */
void func_15112520(WorldObject *turret);
extern Turret15112520Cfg D_80089200;

/* game_13FF30.c */
void func_15112A80(s32 player);
void func_15113180(void);
void func_15113218(void);
Gfx *func_151135C4(Gfx *gfx, s32 translucent, s16 view);
Gfx *func_151137D4(Gfx *gfx, WorldObject *obj, Mtx *mtx, s32 view, s32 distance, s32 pass);
void func_15113C88(void);
void func_15113E54(s32 arg0);
s32 func_15114050(WorldObject *obj, s32 actor);
s32 func_151140C4(WorldObject *obj);
void func_15114188(void);
void func_15114348(s32 objIndex, f32 *x, f32 *y, f32 *z);
void func_1511473C(Actor *actor, s32 objIndex);
void func_151148A8(MtxF mtx, WorldObject *obj);
void func_1511490C(MtxF mtx, WorldObject *obj);
WorldObject *func_151149AC(u8 id);
void func_15114A1C(void);
void func_15114B94(u32 index);
s32 func_15114CC4(ObjSound *sound, s32 arg1, s32 *follow, s32 arg3);
void func_15114D24(WorldObject *obj, s32 sfx, s32 arg2, s16 arg3, s16 arg4, s32 flags);
void func_15114F04(WorldObject *obj, s32 arg1, s32 arg2);
void func_15114F44(s32 arg0, s32 arg1, s32 arg2);
extern DrawEntry **D_80089240[4]; // per-view draw lists
extern Mtx **D_80089250[2]; // per frame buffer: one Mtx per object
extern DrawEntry *D_800DBED0[4];
extern Mtx *D_800DBEE0;
extern Mtx *D_800DBEE4;
extern u16 D_800DBEE8[4]; // per view: entries in D_80089240
extern s32 gWorldObjectCount; // number of db_Object_array entries
extern WorldObject *db_Object_array; // original name (Oct 2000 debugger); gWorldObjectCount entries
extern void **D_800DBEF8; // per object: handle released with func_10004074
extern u8 *D_800DBEFC; // per object: frames to keep the handle once hidden
extern u8 D_800DBF00;
extern MtxF D_800DBF08[2]; // per frame buffer
extern u8 D_800DBF88;
extern Gfx *D_800DBF8C; // display list of object D_800DBF88 (func_15004574)
extern Vtx *D_800DBF90; // its source vertices
extern s32 *gWorldObjectContacts; // per object: bit N set when actor movement_obj[N] touches it
extern s32 D_800DBF98; // grouped objects, kept at the end of db_Object_array

#endif
