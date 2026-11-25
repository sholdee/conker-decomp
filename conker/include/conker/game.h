#ifndef CONKER_GAME_H
#define CONKER_GAME_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "game": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_34ED0.c
 *   game_34F20.c
 *   game_3B920.c
 *   game_45B80.c
 *   game_49BE0.c
 *   game/done/game_49D10.c
 *   game/done/game_49D20.c
 */

/* game/done/game_34ED0.c */
void func_15007A20(void);

/* game_34F20.c */
void func_15007A70(s16 arg0, s16 arg1, s16 arg2);
void func_15007B3C(void);
void func_150081E4(void);
void func_15008230(void);
void func_15008248(s32 arg0);
void func_150082CC(void);

/* game_3B920.c */
void func_1500E470(s32 levelId);

/* game_45B80.c */
void func_150186D0(void);
s32 func_1501878C(void);
void func_15018DFC(void);
Gfx *func_15018E88(Gfx *gfx, s16 player);
Gfx *func_15018F08(Gfx *gfx, s16 player);
void func_15018F80(s16 player);
void func_1501905C(void);
s32 func_15019130(void);
void func_15019414(void);
Gfx *func_15019464(Gfx *gfx, s16 player);
Gfx *func_150195A0(Gfx *gfx, s16 player);
Gfx *func_150198FC(Gfx *gfx);
Gfx *func_15019BB8(Gfx *gfx, s32 player);
Gfx *func_15019CC8(Gfx *gfx, s32 player);
Gfx *func_15019E60(Gfx *gfx);
void func_15019F20(void);
extern f32 D_800BE5E8[10]; // the last 10 frame deltas
extern u32 D_800BE610; // the leftover tick remainder
extern u8 D_800BE614;
extern u8 D_800BE615;
extern u8 D_800BE616;
extern u8 D_800BE617;

/* game_49BE0.c */
void func_1501C730(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

/* game/done/game_49D10.c */
void func_1501C860(void);

/* game/done/game_49D20.c */
void func_1501C870(void);

#endif
