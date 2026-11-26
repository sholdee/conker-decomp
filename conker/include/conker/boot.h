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
 *   init_5AB0.s
 *   init_5BE0.s
 *   init_5C2C.s
 *   init_6240.s
 *   init_709C.s
 *   init_7DA0.s
 *   game/entrypoint.c
 */

/* init_1050.c */
void func_10001050(void);
void func_100010F8(void *arg);
void func_10001194(void *arg);
extern RomFile D_8002AAD0[2];
extern u8 D_8002AAE0;
extern u8 D_8002AAE4;
extern u16 *D_8002AAE8[2]; // the two framebuffers

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

/* init_5AB0.s: TLB paging of the game code (virtual 0x15000000) */
void func_10005B04(s32 pageCount);

/* init_5BE0.s */
void func_10005BE0(void);

/* init_5C2C.s */
void func_100061F8(s32 firstEntry, s32 lastEntry);

/* init_6240.s: decompresses src into dst and returns the size written.
 * (void *src, void *dst, u32 work); declared without a prototype: callers pass the destination as a
 * pointer or as an address in an s32 */
s32 func_10006240();

/* init_709C.s */
void func_1000709C(void);

/* init_7DA0.s: a syscall (debug trap) */
void func_10007DA0(void);

/* game/entrypoint.c */
void func_15007830(void);

#endif
