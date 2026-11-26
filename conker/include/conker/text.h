#ifndef CONKER_TEXT_H
#define CONKER_TEXT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "text": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_42DC0.c
 *   game_439B0.c
 *   game_6E770.c
 *   game_6EA90.c
 *   game_70200.c
 *   game_AD9B0.c
 *   game_197C20.c
 *   game_197F20.c
 *   game_203E20.c
 */

/* game_42DC0.c */
void func_15015910(void);
void func_15015920(s16 font);
void func_15015A38(u8 *record, s32 glyph, s32 font);

/* game_439B0.c */
void func_15016500(void);
void func_15016588(void);
extern u8 D_800BE580[9]; // bit set over the 0x43 text ids

/* game_6E770.c */
Gfx *func_150412C0(Gfx *gfx);
Gfx *func_150413FC(Gfx *gfx, s32 x, s32 y, u8 *str);
s32 func_15041480(u8 ch);
Gfx *func_15041508(Gfx *gfx, s32 x, s32 y, s32 glyph);

/* game_6EA90.c */
void func_150415E0(s32 glyph, s32 *width, s32 *height, f32 *yOffset, s32 *noCombine, f32 *scale);
Gfx *func_150417AC(Gfx *gfx, f32 x, f32 y, u8 *str, u8 r, u8 g, u8 b, u8 a, f32 scaleX, f32 scaleY, s32 len);
void func_150428D4(u8 *str, s32 *width, s32 *height, s32 *depth);
s32 func_15042C40(u8 ch);
extern u8 D_800CBD4C;  // cleared by func_15015910
extern u16 D_800CBD4E; // animation clock (high bits pick icon frames)

/* game_70200.c */
void func_15042D50(void);
void func_15042D78(u8 flags);
void func_15042D94(s32 x, s32 y, u8 flags, char *format, ...);
void func_15042E3C(char *format, ...);
void func_15042ECC(u8 *format, s32 *argumentWords);
void func_150432BC(f32 scale);
void func_150432CC(Actor *object, s32 yOffset);
void func_150432FC(s16 x, s16 y);
void func_1504332C(u8 red, u8 green, u8 blue, u8 alpha);
Gfx *func_15043384(Gfx *gfx);
void func_15043A00(MsgRing *ring, u8 *buffer, s32 capacity);
s32 func_15043A20(u8 *buffer, s32 capacity, s32 offset, u8 *src, s32 size);
s32 func_15043AC8(u8 *buffer, s32 capacity, s32 offset, u8 *dst, s32 size);
s32 func_15043B70(u8 *buffer, s32 capacity, s32 offset, s32 size);
s32 func_15043BB8(MsgRing *ring, u8 *message, s32 size);
s32 func_15043CA4(MsgRing *ring, u8 *dst, s32 dstSize);

/* game_AD9B0.c */
/* (Actor *obj, TriggerZone *zone, s32 slot, s32 textId); declared without a prototype: game_CDE80.c passes 5 arguments */
void func_15080500();
void func_15080620(s32 objIndex, s32 slot, s32 textId, s32 arg3);
void func_150806A8(s32 objIndex);
void func_15080718(s32 n, s32 *byteIndex, s32 *bitMask);
s32 func_15080738(s32 textId);
void func_15080784(void);
void func_150807F4(TextBoxConfig *config, s32 page, s32 event);
void func_15080828(s32 arg0);
void func_15080BE8(void);
void func_15080C64(void);
s32 func_15080CF4(void);

/* game_197C20.c */
s32 func_1516A770(u8 *text);
TextBox *func_1516A7B0(u32 msgIndex, s32 objIndex, s32 arg2,
                       void (*callback)(TextBoxConfig *config, s32 page, s32 state),
                       TextBoxConfig *suppliedConfig, s32 owner);

/* game_197F20.c */
void func_1516AA70(TextBox *box);
Gfx *func_1516B6BC(Gfx *gfx, TextBox *box, s32 arg2);
u8 *func_1516C878(TextBoxConfig *config, s32 index, s16 source);
Gfx *func_1516C934(Gfx *gfx, TextBox *box, u8 r, u8 g, u8 b, u8 a);
void func_1516CC58(TextBox *box);
void func_1516D0CC(TextBox *box, f32 *x, f32 *y, f32 *angle);
void func_1516D2D8(void);
void func_1516D2E0(TextBox *box);
void func_1516D328(TextBox *box);
TextBox *func_1516D378(TextBox *box);
TextBox *func_1516D3C4(TextBox *box);

/* game_203E20.c */
void func_151D6970(Actor *character, void *obj);
void func_151D69B4(WorldObject *pickup, Actor *player);
void func_151D6BFC(Actor *owner, u8 arg1, s32 arg2);
s32 func_151D6E60(ModelEffect *fx);
s32 func_151D7000(TextObj *obj);
void func_151D70CC(TextObj *obj, u8 *payload, u8 type);
Gfx *func_151D710C(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed);

#endif
