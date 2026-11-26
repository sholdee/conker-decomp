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
 *   init_7DAC.s
 *   game/entrypoint.c
 */

/* init_1050.c */
void boot_entry(void);
void boot_idleThread(void *arg);
void boot_mainThread(void *arg);
extern RomFile D_8002AAD0[2];
extern u8 D_8002AAE0;
extern u8 D_8002AAE4;
extern u16 *gFramebuffers[2]; // the two framebuffers
extern u64 gIdleThreadStack[0x400 / sizeof(u64)];  // idle thread stack
extern u64 gMainThreadStack[0x3C00 / sizeof(u64)]; // main thread stack, with gMainThreadStackTop
extern OSThread gMainThread;                  // main thread
extern s32 *gChunk0Offsets;                      // chunk0 offsets

/* init_38C0.c */
void func_100038C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* init_38E0.c */
s32 func_100038E0(void);
s32 func_1000390C(void);

/* init_39B0.c */
void func_100039B0(void);

/* init_39C0.c */
void boot_initScreen(void);
void boot_clearFramebuffers(s32 red, s32 green, s32 blue);
extern f32 gScreenScaleX; // screen width / 292
extern f32 gScreenScaleY; // screen height / 216

/* init_5AB0.s: TLB paging of the game code (virtual 0x15000000) */
void func_10005B04(s32 pageCount);
extern s32 D_8003BE70;
extern s32 D_8003BE74;

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

/* init_7DAC.s */
extern s32 D_8003C8E0; // status code that the game posts for the debugger (0 when clear)

/* game/entrypoint.c */
void boot_runGame(void);

#endif
