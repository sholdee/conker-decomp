#ifndef CONKER_PATH_H
#define CONKER_PATH_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "path": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_B3020.c
 */

/* game_B3020.c */
void func_15085B70(s32 id);
s32 func_15085BE8(void);
u8 func_15085DA8(f32 y);
s32 func_15085DF8(f32 x, f32 y, f32 z, s8 type, s8 layer);
s32 func_15086098(f32 x, f32 y, f32 z, s8 nGoals, s8 nAvoid, u8 *goals, u8 *avoid);
s32 func_15086364(s32 start, s8 nGoals, s8 nAvoid, u8 *goals, u8 *avoid);

#endif
