#ifndef CONKER_BOOT_H
#define CONKER_BOOT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "boot": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Boot/BootMain/bootmain.c
 *   init/Boot/ArgStub/argstub.c
 *   init_38E0.c
 *   init/Boot/VoidStub/voidstub.c
 *   init/Boot/ScreenInit/screeninit.c
 *   init/Boot/Paging/pagesetup.s
 *   init/Boot/Paging/pagebitmap.s
 *   init/Boot/Paging/pagefault.s
 *   init/Boot/Inflate/decompress.s
 *   init/Boot/Inflate/inflatetables.s
 *   init/Boot/DebugTrap/debugtrap.s
 *   init/Boot/FaultDebugger/faultdebugger.s
 *   game/Boot/GameMain/gamemain.c
 */

/* init/Boot/BootMain/bootmain.c */
void boot_entry(void);
void boot_idleThread(void *arg);
void boot_mainThread(void *arg);
extern RomFile D_8002AAD0[2];
extern u8 D_8002AAE0;
extern u8 gPadInitDone;
extern u16 *gFramebuffers[2]; // the two framebuffers
extern u64 gIdleThreadStack[0x400 / sizeof(u64)];  // idle thread stack
extern u64 gMainThreadStack[0x3C00 / sizeof(u64)]; // main thread stack, with gMainThreadStackTop
extern OSThread gMainThread;                  // main thread
extern s32 *gChunk0Offsets;                      // chunk0 offsets

/* init/Boot/ArgStub/argstub.c */
void func_100038C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

/* init_38E0.c */
s32 func_100038E0(void);
s32 func_1000390C(void);

/* init/Boot/VoidStub/voidstub.c */
void func_100039B0(void);

/* init/Boot/ScreenInit/screeninit.c */
void boot_initScreen(void);
void boot_clearFramebuffers(s32 red, s32 green, s32 blue);
extern f32 gScreenScaleX; // screen width / 292
extern f32 gScreenScaleY; // screen height / 216

/* init/Boot/Paging/pagesetup.s: TLB paging of the game code (virtual 0x15000000) */
void boot_initPaging(s32 pageCount);
extern s32 gPageFreeBitmap;
extern s32 gPagePool;

/* init/Boot/Paging/pagebitmap.s */
void boot_resetPageBitmap(void);

/* init/Boot/Paging/pagefault.s */
void boot_invalidateTlbEntries(s32 firstEntry, s32 lastEntry);

/* init/Boot/Inflate/decompress.s: decompresses src into dst and returns the size written.
 * (void *src, void *dst, u32 work); declared without a prototype: callers pass the destination as a
 * pointer or as an address in an s32 */
s32 boot_decompress();

/* init/Boot/Inflate/inflatetables.s */
void boot_initInflateFixedTables(void);

/* init/Boot/DebugTrap/debugtrap.s: a syscall (debug trap) */
void boot_debugTrap(void);

/* init/Boot/FaultDebugger/faultdebugger.s */
extern s32 gDebugStatusCode; // status code that the game posts for the debugger (0 when clear)

/* game/Boot/GameMain/gamemain.c */
void boot_runGame(void);

#endif
