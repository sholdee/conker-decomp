#ifndef CONKER_DEBUG_H
#define CONKER_DEBUG_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "debug": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_6CEA0.c
 *   game_6D800.c
 *   game_6E240.c
 */

/* game_6CEA0.c */
void func_1503F9F0(s16 controller, s16 *dirX, s16 *dirY);
void func_1503FB08(void);
s32 func_1503FB40(Gfx **gfxp, u16 backButtons, u16 selectButtons);
extern s16 D_80084480;

/* game_6D800.c */
void func_15040350(void *dl, u32 end);
void func_150403C8(void *dl, u32 end, s32 parent);
s32 func_1504072C(s32 addr);
void func_15040748(Gfx *gfx);
s32 func_15040754(Gfx *gfx);
void func_150407A0(void);
void func_150407A8(Gfx *gfx);
void func_150407B4(Gfx *gfx);
void func_150407C0(Gfx *gfx);
void func_150407CC(Gfx *gfx);
void func_150407D8(Gfx *gfx);
void func_150407E4(Gfx *gfx);
void func_150407F0(Gfx *gfx);
void func_150407FC(Gfx *gfx);
void func_15040808(Gfx *gfx);
void func_15040814(Gfx *gfx);
void func_15040820(Gfx *gfx);
s32 func_1504082C(u32 *w0);
void func_1504086C(Gfx *gfx);
void func_15040878(Gfx *gfx);
void func_15040884(Gfx *gfx);
void func_15040890(Gfx *gfx);
void func_1504089C(Gfx *gfx);
void func_150408A8(Gfx *gfx);
void func_150408B4(Gfx *gfx);
void func_150408C0(Gfx *gfx);
s32 func_150408CC(Gfx *gfx);
void func_150408F0(Gfx *gfx);
void func_150408FC(Gfx *gfx);
void func_15040908(Gfx *gfx);
void func_15040914(Gfx *gfx);
void func_15040920(Gfx *gfx);
void func_1504092C(Gfx *gfx);
void func_15040938(Gfx *gfx);
void func_15040944(Gfx *gfx);
void func_15040950(Gfx *gfx);
void func_1504095C(Gfx *gfx);
void func_15040968(Gfx *gfx);
void func_15040974(Gfx *gfx);
void func_15040980(Gfx *gfx);
void func_1504098C(Gfx *gfx);
void func_15040998(Gfx *gfx);
void func_150409A4(Gfx *gfx);
void func_150409B0(Gfx *gfx);
void func_150409BC(Gfx *gfx);
void func_150409C8(Gfx *gfx);
void func_150409D4(Gfx *gfx);
void func_150409E0(Gfx *gfx);
void func_150409EC(Gfx *gfx);
void func_150409F8(Gfx *gfx);
void func_15040A04(Gfx *gfx);
void func_15040A10(Gfx *gfx);
void func_15040A1C(Gfx *gfx);
void func_15040A28(Gfx *gfx);
void func_15040A34(Gfx *gfx);
void func_15040A40(Gfx *gfx);
void func_15040A4C(void);
void func_15040A54(Gfx *gfx);
void func_15040A60(Gfx *gfx);
void func_15040A6C(Gfx *gfx);
void func_15040A78(Gfx *start, Gfx *from, Gfx *to);
void func_15040CC8(Gfx *gfx);
void func_15040D60(Gfx *gfx);
void func_15040D6C(Gfx *gfx);
void func_15040D78(Gfx *gfx);

/* game_6E240.c */
void func_15040D90(s32 arg0);
void func_15040D9C(s32 arg0);
void func_15040FCC(s32 arg0);
void func_1504129C(void);

#endif
