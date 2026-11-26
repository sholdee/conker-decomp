#ifndef CONKER_MODEL_H
#define CONKER_MODEL_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "model": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_32200.c
 *   game/done/game_322B0.c
 *   game/done/game_323B0.c
 *   game_48B10.c
 *   game_623D0.c
 *   game_63A20.c
 *   game_6A3D0.c
 *   game_6B320.c
 */

/* One queued reflection: a model's mirrored matrices plus how to draw them. */
typedef struct ReflectionEntry {
    /* 0x00 */ u32 mtx;        // segment address of the mirrored matrices
    /* 0x04 */ u8 alpha;
    /* 0x05 */ u8 color[3];
    /* 0x08 */ u8 objIndex;    // index into movement_obj
    /* 0x09 */ u8 ownerId;     // unique_id of that object
    /* 0x0A */ u8 setId;       // attachment set id (attachment reflections only)
    /* 0x0B */ u8 isAttachment;
} ReflectionEntry;

/* An 8-byte entry of a 0-terminated offset list. */
typedef struct OffsetEntry {
    /* 0x0 */ s32 offset;
    /* 0x4 */ s32 unk4;
} OffsetEntry;

/* game_32200.c */
void func_15004D50(void);

/* game_322B0.c */
void func_15004E00(void);
void func_15004E80(void);

/* game_323B0.c */
void func_15004F00(void);
void func_15004F10(void);

/* game_48B10.c */
void func_1501B660(Actor *arg0, f32 *arg1, s32 arg2);

/* game_623D0.c */
void func_15034F20(void);
void func_15034F30(Actor *obj, ReflectionEntry *entry, s32 mode, MtxF *srcMtx, MtxF *dstMtx, s32 count, f32 planeX, f32 planeY, f32 planeZ, f32 unused, f32 angle, f32 objX, f32 objY, f32 objZ, f32 fadeStart, f32 fadeEnd, f32 maxAlpha, s32 srcIsFixed, s32 dstToFixed);
ReflectionEntry *func_150356C8(void);
s32 func_15035714(s32 mode, Actor *obj, f32 (*mtx)[4], f32 planeY);
void func_15035808(s32 mode, s32 objIndex, f32 planeX, f32 planeY, f32 planeZ, f32 unused, f32 angle, f32 fadeStart, f32 fadeEnd, f32 maxAlpha);
Gfx *func_15035D6C(Gfx *gfx, ModelAttachment *att, s32 red, s32 green, s32 blue);
Gfx *func_15035FE8(Gfx *gfx, s32 player);
void func_15036148(void);

/* game_63A20.c */
void func_15036570(s32 index, MtxF pinMtx, MtxF followMtx);
Gfx *func_150368C4(Gfx *gfx, s32 index, s32 view);

/* game_6A3D0.c */
s32 func_1503CF20(s32 id, s32 arg1, SpawnRecord *def, s32 loadTextures, s32 flags);
void func_1503D368(s8 *dl, s32 base);
void func_1503D438(s32 *offsetSlot, s32 base);
void func_1503D45C(OffsetEntry *entry, s32 base);
void func_1503D484(ActorAnim *anim, s32 id);
void func_1503D510(s32 id);
s32 func_1503D5F0(s32 id);
s32 func_1503D660(s32 id, s32 arg1);
s32 func_1503D774(s32 id, s32 arg1);
s32 func_1503D804(s32 id);
void func_1503D984(s32 id);
s32 func_1503DA3C(s32 id, s32 index);
s32 func_1503DA9C(Actor *obj, s32 id, s32 model, s32 loadTextures);
s32 func_1503DC3C(s32 id);
void func_1503DD1C(s32 id);

/* game_6B320.c */
void func_1503DE70(Actor *obj, s32 kind, s32 variant);
void func_1503DF0C(s32 objIndex, s32 kind, s32 maskLo, s32 maskHi);
void func_1503DF48(s32 objIndex);
s32 func_1503E1F4(s32 partIndex, s32 objIndex);
void func_1503E260(s32 objIndex);
void func_1503E3C4(s32 objIndex, s32 partIndex, s32 boneOffset, ObjPart *part, s32 noParent);
void func_1503E5F8(MtxF mtx, f32 *x, f32 *y, f32 *z, f32 *rotX, f32 *rotY, f32 *rotZ, f32 *scaleX, f32 *scaleY, f32 *scaleZ);
void func_1503E82C(s32 objIndex);
void func_1503EA54(s32 objIndex);
void func_1503EB78(ObjPart *part, f32 spreadXZ, f32 spreadY, s32 strong);
void func_1503ECA0(s32 objIndex);
void func_1503EEB8(void);
void func_1503EEC0(s32 objIndex);
s32 func_1503EF4C(s32 kind, s32 variant, s32 objIndex);
void func_1503EFC4(s32 objIndex);
void func_1503F078(ObjPart *part, s32 objIndex);
void func_1503F0AC(ObjPart *part, s32 objIndex);
void func_1503F0D8(ObjPart *part, s32 objIndex);
void func_1503F108(s32 objIndex);
void func_1503F16C(s32 objIndex);
void func_1503F2B0(s32 objIndex);
void func_1503F404(Mtx *mtx, f32 *x, f32 *y, f32 *z, f32 *rotX, f32 *rotY, f32 *rotZ, f32 *scaleX, f32 *scaleY, f32 *scaleZ);

#endif
