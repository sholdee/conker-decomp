#ifndef CONKER_ATTACH_H
#define CONKER_ATTACH_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "attach": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Attach/Attachment/attachment.c
 *   game/Attach/FlashingLight/flashinglight.c
 *   game/Attach/AttachProjectile/attachprojectile.c
 *   game/Attach/TextureScroll/texturescroll.c
 *   game/Attach/BonePoint/bonepoint.c
 *   game/Attach/AttachModel/attachmodel.c
 */

/* Types that these files define; the declarations below use them. */
struct SoundRecord;

/* game/Attach/Attachment/attachment.c */
extern ModelAttachment *gAttachments; // head of the model attachment list
s32 attach_loadModel(s32 model, Gfx ***dlists, u8 *numDLists, u8 **data, s32 *arg4, s32 *arg5, s32 *numParts);
s32 attach_initModel(ModelAttachment *att, Actor *owner);
void attach_free(ModelAttachment *att, s32 immediate);
void attach_removeSet(Actor *owner, s32 setId);
s32 attach_removeFromSet(Actor *owner, s32 setId, s32 index);
s32 attach_removeAll(Actor *owner);
void attach_updateAll(void);
ModelAttachment *attach_add(Actor *owner, s32 model, s32 bone, s32 flags, s32 unk15, s32 unk16, s32 lifetime,
                               s32 setId, s32 handler, const u16 *ownerStates, s32 animBank, s32 parentSet, s32 parentMtx);
void attach_setOffset(ModelAttachment *att, f32 x, f32 y, f32 z, f32 rotX, f32 rotY, f32 rotZ);
Gfx *attach_drawView(Gfx *gdl, s32 view, s32 pass);
Gfx *attach_drawOwner(Gfx *gdl, Actor *owner, s32 view, s32 reflections);
s32 attach_getMatrices(ModelAttachment *att, Actor *owner, Mtx **mtx, Mtx **boneMtx);
Gfx *attach_draw(Gfx *gdl, ModelAttachment *att, Actor *owner, s32 view, s32 reflections);
void attach_setAlpha(Actor *owner, s32 setId, s32 alpha, s32 index);
ModelAttachment *attach_find(Actor *owner, s32 setId, s32 index);
ModelAttachment *attach_findInSet(s32 setId, Actor *owner);
void attach_applyOwnerEffects(ModelAttachment *att, Actor *owner);
void attach_undoOwnerEffects(ModelAttachment *att);
s32 attach_hookCycleTexture(ModelAttachment *att, Actor *owner);
s32 attach_hookFrameTile(ModelAttachment *att, Actor *owner);
s32 attach_hookHeldProp(ModelAttachment *att, Actor *owner);
s32 attach_hookFollowAnim(ModelAttachment *att, Actor *owner);
s32 attach_hookPlayOnce(ModelAttachment *att, Actor *owner);
s32 attach_hookRemoveWhenDry(ModelAttachment *att, Actor *owner);
s32 attach_hookRemoveInWater(ModelAttachment *att, Actor *owner);
s32 func_15033440(ModelAttachment *att, Actor *owner);
s32 attach_scrollTile(ModelAttachment *att, Actor *owner);
ModelAttachment *attach_attachInPlace(Actor *obj, Actor *target, s32 setId, s32 bone, s32 arg4, s32 noTranslation);
s32 func_1503378C(ModelAttachment *att, Actor *owner);
s32 attach_hookToggleTexture(ModelAttachment *att, Actor *owner);
s32 attach_hookStateEffect(ModelAttachment *att, Actor *owner);
s32 attach_hookScrollTileSound(ModelAttachment *att, Actor *owner);
s32 attach_hookDelayedSound(ModelAttachment *att, Actor *owner);
/* A sound record callback (SoundRecordCallback, conker/sound.h). */
s32 attach_soundCallback(struct SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 attach_hookRemoveWhenHidden(ModelAttachment *att, Actor *owner);
s32 attach_getAll(Actor *owner, ModelAttachment **out);
ModelAttachment *attach_getFirst(Actor *owner);
void attach_swapOwners(s32 a, s32 b);
s32 func_15033F0C(ModelAttachment *att, Actor *owner);
s32 func_15033F70(ModelAttachment *att, Actor *owner);

/* game/Attach/FlashingLight/flashinglight.c */
s32 attach_hookFlashingLight(ModelAttachment *light, Actor *owner);

/* game/Attach/AttachProjectile/attachprojectile.c */
s32 attach_hookSpawnProjectile(ModelAttachment *att, Actor *actor);

/* game/Attach/TextureScroll/texturescroll.c */
void attach_emptyLevelUpdateHook1(s32 unused);
s32 attach_hookScrollTexH(ModelAttachment *att, Actor *actor);
s32 attach_hookScrollTexV(ModelAttachment *att, Actor *actor);
GameCommand *func_150D8590(GameCommand *cmd, s32 index);
void attach_spawnFootImpact(Actor *actor);
s32 attach_disableLightWithoutExpression(LightEffect *effect);

/* game/Attach/BonePoint/bonepoint.c */
s32 attach_getBonePoint(Vec3f *out, Actor *actor, u8 point);
s32 attach_selectBonePointEffects(s32 kind, s32 actor);
void attach_spawnBonePointEffects(Actor *actor, s32 point, s32 unused);

/* game/Attach/AttachModel/attachmodel.c */
extern u8 gAttachModelRefCounts[0x1E4];                 /* per-model load reference counts */
extern s16 *gAttachModelTexIds[0x1E4];               /* per model: written by texture_resolveDl */
extern AttachModelHeader *gAttachModelHeaders[0x1E4]; /* per model: loaded attachment model header */
Gfx *attach_loadModelDl(s32 model);
void attach_releaseModel(s32 model);

#endif
