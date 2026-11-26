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
s32 func_1502FE10(s32 model, Gfx ***dlists, u8 *numDLists, u8 **data, s32 *arg4, s32 *arg5, s32 *arg6);
s32 func_1502FFD8(ModelAttachment *att, Actor *owner);
void func_15030158(ModelAttachment *att, s32 immediate);
void func_150302F0(Actor *owner, s32 setId);
s32 func_15030310(Actor *owner, s32 setId, s32 index);
s32 func_150303E4(Actor *owner);
void func_15030468(void);
ModelAttachment *func_15030AF4(Actor *owner, s32 model, s32 bone, s32 flags, s32 unk15, s32 unk16, s32 lifetime,
                               s32 setId, s32 handler, u16 *ownerStates, s32 unk17, s32 parentSet, s32 parentMtx);
void func_15030D54(ModelAttachment *att, f32 x, f32 y, f32 z, f32 rotX, f32 rotY, f32 rotZ);
Gfx *func_15030E08(Gfx *gdl, s32 view, s32 pass);
Gfx *func_15030F94(Gfx *gdl, Actor *owner, s32 view, s32 arg3);
s32 func_15031070(ModelAttachment *att, Actor *owner, Mtx **mtx, Mtx **boneMtx);
Gfx *func_150311C4(Gfx *gdl, ModelAttachment *att, Actor *owner, s32 view, s32 arg4);
void func_1503192C(Actor *owner, s32 setId, s32 alpha, s32 index);
ModelAttachment *func_1503195C(Actor *owner, s32 setId, s32 index);
ModelAttachment *func_150319CC(s32 setId, Actor *owner);
void func_15031A50(ModelAttachment *att, Actor *owner);
void func_15031C14(ModelAttachment *att);
s32 func_15031E2C(ModelAttachment *att, Actor *owner);
s32 func_15031E7C(ModelAttachment *att, Actor *owner);
s32 func_15031FC8(ModelAttachment *att, Actor *owner);
s32 func_150331B8(ModelAttachment *att, Actor *owner);
s32 func_1503327C(ModelAttachment *att, Actor *owner);
s32 func_15033328(ModelAttachment *att, Actor *owner);
s32 func_150333A8(ModelAttachment *att, Actor *owner);
s32 func_15033440(ModelAttachment *att, Actor *owner);
s32 func_150334B8(ModelAttachment *att, Actor *owner);
ModelAttachment *func_150335C8(Actor *obj, Actor *target, s32 setId, s32 bone, s32 arg4, s32 noTranslation);
s32 func_1503378C(ModelAttachment *att, Actor *owner);
s32 func_150337E4(ModelAttachment *att, Actor *owner);
s32 func_15033838(ModelAttachment *att, Actor *owner);
s32 func_150339C8(ModelAttachment *att, Actor *owner);
s32 func_15033AD8(ModelAttachment *att, Actor *owner);
/* A sound record callback (SoundRecordCallback, conker/sound.h). */
s32 func_15033BDC(struct SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_15033E00(ModelAttachment *att, Actor *owner);
s32 func_15033E28(Actor *owner, ModelAttachment **out);
ModelAttachment *func_15033E84(Actor *owner);
void func_15033EC4(s32 a, s32 b);
s32 func_15033F0C(ModelAttachment *att, Actor *owner);
s32 func_15033F70(ModelAttachment *att, Actor *owner);

/* game_F4890.c */
s32 func_150C73E0(ModelAttachment *light, Actor *owner);

/* game_F5BE0.c */
s32 func_150C8730(ModelAttachment *att, Actor *actor);

/* game_105760.c */
void func_150D82B0(s32 arg0);
s32 func_150D82BC(ModelAttachment *att, Actor *actor);
s32 func_150D83D8(ModelAttachment *att, Actor *actor);
GameCommand *func_150D8590(GameCommand *cmd, s32 arg1);
void func_150D85AC(Actor *actor);
s32 func_150D88AC(struct S150D88AC *arg0);

/* game_105D90.c */
s32 func_150D88E0(Vec3f *out, Actor *actor, u8 point);
s32 func_150D8A20(s32 arg0, s32 arg1);
void func_150D8A34(Actor *actor, s32 point, s32 arg2);

/* game_1B9DB0.c */
Gfx *func_1518C900(s32 arg0);
void func_1518CA04(s32 arg0);

#endif
