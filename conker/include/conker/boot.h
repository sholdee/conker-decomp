#ifndef CONKER_BOOT_H
#define CONKER_BOOT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "boot": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_1050.c
 *   init_38C0.c
 *   init_38E0.c
 *   init_39B0.c
 *   init_39C0.c
 *   game/entrypoint.c
 */

/* init_1050.c */
void func_10001050(void);
void func_100010F8(s32 arg);
void func_10001194(s32 arg);

/* init_38C0.c */
void func_100038C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* init_38E0.c */
s32 func_100038E0(void);
s32 func_1000390C(void);

/* init_39B0.c */
void func_100039B0(void);

/* init_39C0.c */
void func_100039C0(void);
void func_10003ACC(s32 red, s32 green, s32 blue);

/* game/entrypoint.c */
void func_15007830(void);

#endif
