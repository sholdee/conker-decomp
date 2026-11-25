#ifndef CONKER_SPRITE_H
#define CONKER_SPRITE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "sprite": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_C1D70.c
 *   game_C2350.c
 *   game_E4070.c
 *   game_137DE0.c
 *   game_182140.c
 *   game_188F90.c
 *   game_196DB0.c
 *   game_1A89B0.c
 */

/* Defined in their C files. A typedef here would reorder the .bss of game_6EA90.c and game_E4070.c. */
struct GroupPoint;
struct PointGroup;
struct ScreenSprite;
struct TexturedQuad;
struct ActorTracker;
struct ActorTrackerDesc;
struct Game1A89B0Position;

/* game_C1D70.c */
void func_150948C0(struct PointGroup *group, f32 (*out)[3]);
void func_1509499C(struct PointGroup *group, struct GroupPoint **slots);
void func_15094AB8(Vtx *verts, s16 (*texcoords)[2], s32 count, f32 angle, s32 width, s32 height);
void func_15094E98(void);

/* game_C2350.c */
void func_15094EA0(s32 view);
Gfx *func_15094F40(Gfx *gfx);
Gfx *func_15094F70(Gfx *gfx, TextureDesc *desc, s32 frame, SpriteParams *sprite, s32 tmem, s32 tile, s32 sfrac, s32 cm, s32 assetArg);
Gfx *func_15094FE8(Gfx *gfx, TextureDesc *desc, s32 frame, SpriteParams *sprite, s32 tmem, s32 tile, s32 sfrac, s32 cm, s32 s, s32 t, s32 assetArg);
void func_15095060(TextureDesc *desc, s32 frame, SpriteParams *sprite);
Gfx *func_150950D4(Gfx *gfx, Texture *tex, s32 tmem, s32 tile, s32 frame, s32 sfrac, s32 cm, s32 s, s32 t, s32 assetArg);
s32 func_1509563C(f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ, f32 *outW, f32 maxW);
Gfx *func_15095760(Gfx *gfx, SpriteParams *sprite);
Gfx *func_150958B0(Gfx *gfx, SpriteParams *sprite, s32 *visible);
Gfx *func_15095A48(Gfx *gfx, SpriteParams *sprite, f32 x, f32 y);
Gfx *func_15095A90(Gfx *gfx, SpriteParams *sprite, f32 x, f32 y, f32 z, s32 projected, s32 sOffset, s32 tOffset, s32 extraWidth);
void func_15095B08(SpriteParams *sprite, f32 x, f32 y, f32 z, s32 projected, s32 *visible);
Gfx *func_15095D0C(Gfx *gfx, SpriteParams *sprite);
Gfx *func_15095D34(Gfx *gfx, SpriteParams *sprite, s32 sOffset, s32 tOffset, s32 extraWidth);
Gfx *func_1509629C(Gfx *gfx, TextureDesc *desc, f32 scaleX, f32 scaleY, f32 x, f32 y, s32 width, s32 height, s32 flags, s32 sOffset, s32 extraWidth, s32 frame);
Gfx *func_15096934(Gfx *gfx);

/* game_E4070.c */
void func_150B6BC0(s32 unused);
void func_150B6C90(void);
void func_150B6D34(void);
void func_150B6D78(void);
void func_150B6DFC(Sprite2D *sprite);
void func_150B6E3C(Sprite2D *sprite);
void func_150B709C(Sprite2D *sprite);
void func_150B71A8(Sprite2D *sprite);
void func_150B7220(s32 arg0, s32 arg1, s32 duration, s32 arg3);
void func_150B73F0(Sprite2D *sprite);
void func_150B7484(void);
void func_150B7560(void);
void func_150B765C(void);
void func_150B768C(void);
void func_150B76BC(s32 x, s32 slot);
void func_150B77A8(void);
void func_150B791C(void);

/* game_137DE0.c */
void func_1510A930(s32 fadeIn, s32 hold, s32 fadeOut);

/* game_182140.c */
void func_15154C90(struct ScreenSprite *sprite);
Gfx *func_15154D80(Gfx *gfx, struct ScreenSprite *sprite, s16 viewport);
void *func_1515548C(ScreenSpriteInfo *info, u8 flags, s32 *kinds, s32 kindCount, s32 arg4, u8 arg5, s32 arg6);
void func_15155564(struct ScreenSprite *sprite, s32 arg1, u8 arg2);
s32 func_151555AC(f32 *center, f32 *halfSize, s32 *kinds, s32 kindCount);
void func_1515572C(s32 arg0, u8 arg1);

/* game_188F90.c */
void func_1515BAE0(struct TexturedQuad *quad);
Gfx *func_1515BBF0(Gfx *gfx, struct TexturedQuad *quad, s32 view);
void func_1515BE48(void);
struct ActorTracker *func_1515BE50(struct ActorTrackerDesc *desc, s32 extraSize, u8 owner, s32 list);
void func_1515BF50(struct ActorTracker *tracker);
void func_1515BF7C(struct ActorTracker *tracker);
void func_1515BFA8(struct ActorTracker *tracker);
void func_1515C0B8(struct ActorTracker *tracker, s32 event, u8 eventType);
s32 func_1515C0F8(Actor *actor, Vec3f **motion);
void func_1515C158(void);
void func_1515C1A0(Actor *actor, Vec3f *center, f32 *radius, f32 *arg3);
void func_1515C244(Actor *actor, Vec3f *center, f32 *radius, f32 *arg3);

/* game_196DB0.c */
Sprite2D *func_15169900(Sprite2DParams *params, s32 arg1);
Sprite2D *func_15169968(Sprite2DParams *params);
void func_15169988(Sprite2D *sprite);
Gfx *func_15169A48(Gfx *gfx, Sprite2D *sprite, s32 arg2);

/* game_1A89B0.c */
s32 func_1517B500(struct Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha);
s32 func_1517B6E8(struct Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha);
s32 func_1517B7A8(struct Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha);
s32 func_1517B7F8(struct Game1A89B0Position *pos, Vtx *vtx, s32 *phase, u8 *alpha);
void func_1517B89C(s32 first, s32 last);
void func_1517BBAC(s32 x, s32 y, s32 z, s32 radius, f32 angle, s32 fadeX, s32 fadeY, s32 fadeZ);
void func_1517CFC4(void);
Gfx *func_1517D074(Gfx *gfx, s16 x, s16 y, s16 z, f32 angle, s32 halfWidth, s32 height, u8 texWidth, u8 texHeight, s32 texture);
void func_1517D578(s16 x, s16 y, s16 z, f32 angle, s32 halfWidth, s32 height, u8 frame);
void func_1517D5FC(s16 x, s16 y, s16 z, s32 player, s32 halfWidth, s32 height);
Gfx *func_1517D690(Gfx *gfx, s32 texture);
void func_1517D7B0(Gfx **gfx, s32 pass);
void func_1517DE5C(void);
void func_1517E05C(s32 x, s32 y, s32 z);

#endif
