#ifndef CONKER_OS_H
#define CONKER_OS_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "os": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Os/DevMgr/devmgr.c
 *   game/Os/EepromRead/eepromread.c
 *   game/Os/EepromLongRead/eepromlongread.c
 *   game/Os/EepromProbe/eepromprobe.c
 *   game/Os/EepromWrite/eepromwrite.c
 *   game/Os/PiReadIo/pireadio.c
 *   game/Os/ContInit/continit.c
 *   game/Os/ViSetSpecial/visetspecial.c
 *   game/Os/MtxCatF/mtxcatf.c
 */

/* init/Os/DevMgr/devmgr.c (src/libultra/io/piint.h declares it too, for pimgr.c) */
void __osDevMgrMain(void *arg); /* __osDevMgrMain with Rare's PI-thread interlock */

/* game/Os/EepromRead/eepromread.c */
s32 osEepromRead(OSMesgQueue *mq, u8 address, u8 *buffer); /* osEepromRead */
void __osPackEepReadData(u8 address);                              /* __osPackEepReadData */

/* game/Os/EepromLongRead/eepromlongread.c */
/* s32 func_151DD3A0(OSMesgQueue *mq, u8 address, u8 *buffer, int length), Rare's copy of osEepromLongRead, is declared in
   the file that calls it: game/Save/SaveData/savedata.c was compiled with a void return (save_readFile's register
   allocation needs it). It keeps its address name: a void osEepromLongRead would clash with PR/os_eeprom.h. */

/* game/Os/EepromProbe/eepromprobe.c */
s32 osEepromProbe(OSMesgQueue *mq); /* osEepromProbe */

/* game/Os/EepromWrite/eepromwrite.c */
s32 osEepromWrite(OSMesgQueue *mq, u8 address, u8 *buffer);  /* osEepromWrite: one block */
void __osPackEepWriteData(u8 address, u8 *buffer);                  /* __osPackEepWriteData */
s32 __osEepStatus(OSMesgQueue *mq, OSContStatus *data);      /* __osEepStatus */
void os_playDemoInput(void);                                    /* demo playback */
extern s32 gDemoInputIndex; /* demo playback: the next input record */

/* game/Os/PiReadIo/pireadio.c */
s32 osPiReadIo(u32 devAddr, u32 *data); /* osPiReadIo */
f32 func_151EF080(f32 arg0);

/* game/Os/ContInit/continit.c */
/* The game's copy of the SDK's controller.c (osContInit, __osContGetInitData, __osPackRequestData); the init copy
 * (src/libultra/io/controller.c) has the SDK names, so these take the 2 suffix, like osPfsIsPlug2. */
s32 osContInit2(OSMesgQueue *mq, u8 *bitpattern, OSContStatus *data);
void __osContGetInitData2(u8 *pattern, OSContStatus *data);
void __osPackRequestData2(u8 cmd);

/* game/Os/ViSetSpecial/visetspecial.c */
void osViSetSpecialFeatures(u32 func);

/* game/Os/MtxCatF/mtxcatf.c: guMtxXFMF and guMtxCatF, declared by PR/gu.h */

/* libultra/os/thread.c */
extern OSThread *__osRunningThread;

/* libultra/os/initialize.c */
extern u32  __osShutdown;

/* libultra/io/pimgr.c */
extern OSThread D_80035910; // piThread of libultra/io/pimgr.c (which defines it); stopped by the PI manager in init/Os/DevMgr/devmgr

/* libultra/io/controller.c */
extern OSMesg __osEepromTimerMsg;

/* low memory: the boot globals that IPL3 and the OS write at 0x80000300 */
extern s32  D_80000300;
extern s32  D_80000308;
extern s32  D_8000030C; // osResetType; libultra's declaration is in src/libultra/os/osint.h
extern u32  D_80000310;
extern s32  D_8000031C[16]; // osAppNMIBuffer (64 bytes); libultra's declaration is in src/libultra/os/osint.h

#endif
