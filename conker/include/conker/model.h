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
    /* 0x00 */ void *mtx;      // the mirrored matrices, drawn through segment 3
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
void model_resetParts(void);
void model_allocBonePools(void);

/* game_323B0.c */
void func_15004F00(void);
void func_15004F10(void);

/* game_48B10.c */
void func_1501B660(Actor *arg0, f32 *arg1, s32 arg2);

/* game_623D0.c */
void model_clearReflections(void);
void model_mirrorMatrices(Actor *obj, ReflectionEntry *entry, s32 mode, MtxF *srcMtx, MtxF *dstMtx, s32 count, f32 planeX, f32 planeY, f32 planeZ, f32 unused, f32 angle, f32 objX, f32 objY, f32 objZ, f32 fadeStart, f32 fadeEnd, f32 maxAlpha, s32 srcIsFixed, s32 dstToFixed);
ReflectionEntry *model_allocReflection(void);
s32 model_isReflectionCulled(s32 mode, Actor *obj, f32 (*mtx)[4], f32 planeY);
void model_queueReflection(s32 mode, s32 objIndex, f32 planeX, f32 planeY, f32 planeZ, f32 unused, f32 angle, f32 fadeStart, f32 fadeEnd, f32 maxAlpha);
Gfx *model_drawAttachmentReflections(Gfx *gfx, ModelAttachment *att, s32 red, s32 green, s32 blue);
Gfx *model_drawReflections(Gfx *gfx, s32 player);
void func_15036148(void);

/* game_63A20.c */
extern Vtx D_80084300[5];  // the quad vertex template (actor 0's buffer)
extern Vtx *D_800C3FC0[]; // per actor: a 5-vertex quad buffer; [0] is D_80084300
void func_15036570(s32 index, MtxF pinMtx, MtxF followMtx);
Gfx *func_150368C4(Gfx *gfx, s32 index, s32 view);

/* game_6A3D0.c */
extern s32 *D_800C4020[187];           // per model: first vertex of each part
extern u16 D_800C4310[187];            // per model: part count (indexed by VtxMorphGroup.kind)
extern Gfx **gModelDlParts[187];          // per model: display-list parts
extern u16 gModelDlPartCounts[187];            // per model: display-list part count
extern Gfx **D_800C48F0[187];          // per model: display list table (ModelHeader dlists2)
extern void *gModelBones[187];          // per model: bone table (16-byte entries)
extern u16 gModelBoneCounts[187];            // per model: bone count
extern ObjTexture *gModelTextures[187];    // per model: texture table
extern u16 gModelTextureCounts[187];            // per model: texture count
extern u16 gModelVertexCounts[187];            // per model: vertex count
extern u16 D_800C5918[187];            // per model: triangle weight, see func_1503D984
extern u16 gModelAnimCounts[187];            // per model: animation count
extern VtxRun *gModelVtxRuns[187];        // per model: vertex runs (gModelVtxRunCounts[id] of them)
extern u16 gModelVtxRunCounts[187];            // per model: vertex run count
extern VtxTri *gModelTris[187];        // per model: triangles
extern TexCoord *gModelSavedTexCoords[187];      // per model: saved texture coordinates (model_saveTexCoords)
s32 model_load(s32 id, s32 arg1, SpawnRecord *def, s32 loadTextures, s32 flags);
void model_relocateDisplayList(s8 *dl, s32 base);
void model_relocateOffset(s32 *offsetSlot, s32 base);
void model_relocateOffsetList(OffsetEntry *entry, s32 base);
void model_relocateAnimTable(ActorAnim *anim, s32 id);
void model_shareAnimTable(s32 id);
s32 func_1503D5F0(s32 id);
s32 model_loadAnimTable(s32 id, s32 arg1);
s32 func_1503D774(s32 id, s32 arg1);
s32 model_saveTexCoords(s32 id);
void func_1503D984(s32 id);
s32 func_1503DA3C(s32 id, s32 index);
s32 model_loadForActor(Actor *obj, s32 id, s32 model, s32 loadTextures);
s32 model_loadTextures(s32 id);
void model_freeTextures(s32 id);

/* game_6B320.c */
extern ObjPartsState gObjPartsStates[25]; // per object: its parts effect state
void model_breakIntoParts(Actor *obj, s32 kind, s32 variant);
void func_1503DF0C(s32 objIndex, s32 kind, s32 maskLo, s32 maskHi);
void model_updateParts(s32 objIndex);
s32 model_isPartSelected(s32 partIndex, s32 objIndex);
void model_stopParts(s32 objIndex);
void model_initPart(s32 objIndex, s32 partIndex, s32 boneOffset, ObjPart *part, s32 noParent);
void model_decomposeMatrix(MtxF mtx, f32 *x, f32 *y, f32 *z, f32 *rotX, f32 *rotY, f32 *rotZ, f32 *scaleX, f32 *scaleY, f32 *scaleZ);
void model_rebuildBonesFromParts(s32 objIndex);
void model_moveFreeParts(s32 objIndex);
void model_randomizePart(ObjPart *part, f32 spreadXZ, f32 spreadY, s32 strong);
void model_bounceParts(s32 objIndex);
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
void model_decomposeBoneMatrix(void *mtx, f32 *x, f32 *y, f32 *z, f32 *rotX, f32 *rotY, f32 *rotZ, f32 *scaleX, f32 *scaleY, f32 *scaleZ);

#endif
