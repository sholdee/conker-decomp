#ifndef CONKER_ATTACH_H
#define CONKER_ATTACH_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "attach": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_5D2C0.c
 *   game_F4890.c
 *   game_F5BE0.c
 *   game_105760.c
 *   game_105D90.c
 *   game_1B9DB0.c
 */

/* Types that these files define; the declarations below use them. */
struct SoundRecord;
struct S150D88AC;

/* game_5D2C0.c */
extern ModelAttachment *gAttachments; // head of the model attachment list
s32 attach_loadModel(s32 model, Gfx ***dlists, u8 *numDLists, u8 **data, s32 *arg4, s32 *arg5, s32 *arg6);
s32 attach_initModel(ModelAttachment *att, Actor *owner);
void attach_free(ModelAttachment *att, s32 immediate);
void attach_removeSet(Actor *owner, s32 setId);
s32 attach_removeFromSet(Actor *owner, s32 setId, s32 index);
s32 attach_removeAll(Actor *owner);
void attach_updateAll(void);
ModelAttachment *attach_add(Actor *owner, s32 model, s32 bone, s32 flags, s32 unk15, s32 unk16, s32 lifetime,
                               s32 setId, s32 handler, const u16 *ownerStates, s32 unk17, s32 parentSet, s32 parentMtx);
void attach_setOffset(ModelAttachment *att, f32 x, f32 y, f32 z, f32 rotX, f32 rotY, f32 rotZ);
Gfx *attach_drawView(Gfx *gdl, s32 view, s32 pass);
Gfx *attach_drawOwner(Gfx *gdl, Actor *owner, s32 view, s32 arg3);
s32 attach_getMatrices(ModelAttachment *att, Actor *owner, Mtx **mtx, Mtx **boneMtx);
Gfx *attach_draw(Gfx *gdl, ModelAttachment *att, Actor *owner, s32 view, s32 arg4);
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
s32 func_15033328(ModelAttachment *att, Actor *owner);
s32 func_150333A8(ModelAttachment *att, Actor *owner);
s32 func_15033440(ModelAttachment *att, Actor *owner);
s32 attach_scrollTile(ModelAttachment *att, Actor *owner);
ModelAttachment *attach_attachInPlace(Actor *obj, Actor *target, s32 setId, s32 bone, s32 arg4, s32 noTranslation);
s32 func_1503378C(ModelAttachment *att, Actor *owner);
s32 attach_hookToggleTexture(ModelAttachment *att, Actor *owner);
s32 attach_hookStateEffect(ModelAttachment *att, Actor *owner);
s32 func_150339C8(ModelAttachment *att, Actor *owner);
s32 attach_hookDelayedSound(ModelAttachment *att, Actor *owner);
/* A sound record callback (SoundRecordCallback, conker/sound.h). */
s32 attach_soundCallback(struct SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_15033E00(ModelAttachment *att, Actor *owner);
s32 attach_getAll(Actor *owner, ModelAttachment **out);
ModelAttachment *attach_getFirst(Actor *owner);
void attach_swapOwners(s32 a, s32 b);
s32 func_15033F0C(ModelAttachment *att, Actor *owner);
s32 func_15033F70(ModelAttachment *att, Actor *owner);

/* game_F4890.c */
s32 attach_hookFlashingLight(ModelAttachment *light, Actor *owner);

/* game_F5BE0.c */
s32 attach_hookSpawnProjectile(ModelAttachment *att, Actor *actor);

/* game_105760.c */
void func_150D82B0(s32 arg0);
s32 attach_hookScrollTexH(ModelAttachment *att, Actor *actor);
s32 attach_hookScrollTexV(ModelAttachment *att, Actor *actor);
GameCommand *func_150D8590(GameCommand *cmd, s32 arg1);
void attach_spawnFootImpact(Actor *actor);
s32 func_150D88AC(struct S150D88AC *arg0);

/* game_105D90.c */
s32 attach_getBonePoint(Vec3f *out, Actor *actor, u8 point);
s32 func_150D8A20(s32 arg0, s32 arg1);
void attach_spawnBonePointEffects(Actor *actor, s32 point, s32 arg2);

/* game_1B9DB0.c */
extern u8 gAttachModelRefCounts[0x1E4];                 /* per-model load reference counts */
extern s16 *gAttachModelTexIds[0x1E4];               /* per model: written by texture_resolveDl */
extern AttachModelHeader *gAttachModelHeaders[0x1E4]; /* per model: loaded attachment model header */
Gfx *attach_loadModelDl(s32 arg0);
void attach_releaseModel(s32 arg0);

#endif
