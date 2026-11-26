#ifndef CONKER_OS_H
#define CONKER_OS_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "os": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_2E50.c
 *   game_20A5F0.c
 *   game_20A850.c
 *   game_20A910.c
 *   game_20A990.c
 *   game_21C4F0.c
 *   game_21C540.c
 *   game_21CAF0.c
 *   game_21D5F0.c
 */

/* init_2E50.c (src/libultra/io/piint.h declares it too, for pimgr.c) */
void func_10002E50(void *arg); /* __osDevMgrMain with Rare's PI-thread interlock */

/* game_20A5F0.c */
s32 func_151DD140(OSMesgQueue *mq, u8 address, u8 *buffer); /* osEepromRead */
void func_151DD304(u8 address);                              /* __osPackEepReadData */

/* game_20A850.c */
/* s32 func_151DD3A0(OSMesgQueue *arg0, u8 arg1, u8 *arg2, s32 arg3) (osEepromLongRead) is declared in the file that
   calls it: game/Save/SaveData/savedata.c was compiled with a void return. */

/* game_20A910.c */
s32 func_151DD460(OSMesgQueue *mq); /* osEepromProbe */

/* game_20A990.c */
s32 func_151DD4E0(OSMesgQueue *mq, u8 address, u8 *buffer);  /* osEepromWrite: one block */
void func_151DD65C(u8 address, u8 *buffer);                  /* __osPackEepWriteData */
s32 func_151DD710(OSMesgQueue *mq, OSContStatus *data);      /* __osEepStatus */
void os_playDemoInput(void);                                    /* demo playback */
extern s32 gDemoInputIndex; /* demo playback: the next input record */

/* game_21C4F0.c */
s32 func_151EF040(u32 devAddr, u32 *data); /* osPiReadIo */
f32 func_151EF080(f32 arg0);

/* game_21C540.c */
s32 func_151EF090(OSMesgQueue *mq, u8 *bitpattern, OSContStatus *data); /* osContInit */
void func_151EF288(u8 *pattern, OSContStatus *data);                    /* __osContGetInitData */
void func_151EF358(u8 cmd);                                             /* __osPackRequestData */

/* game_21CAF0.c */
void func_151EF640(u32 func);

/* game_21D5F0.c: guMtxXFMF and guMtxCatF, declared by PR/gu.h */

#endif
