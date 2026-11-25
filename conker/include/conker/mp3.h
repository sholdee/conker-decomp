#ifndef CONKER_MP3_H
#define CONKER_MP3_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "mp3": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init_12560.c
 *   game_43620.c
 *   game_221290.c
 */

/* Types that these files define; the declarations below use them. */
typedef struct Mp3Decoder Mp3Decoder;
typedef s32 (*Mp3ReadFn)(s32 handle, void *dst, s32 size, s32 seekPos);

/* init_12560.c */
void func_10012560(s32 unused);
void func_10012588(s32 heap);
void func_100125CC(s32 levelId);
void func_1001263C(s32 id, s32 volume, s32 pan);
s32 func_100126E8(u8 *dst, s32 dstSize);
s32 func_10012718(u16 id, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance);
s32 func_100127D0(void);

/* game_43620.c */
void func_15016170(ALHeap *arg0);

/* game_221290.c */
s32 func_151F3DE0(void);
s32 func_151F42E8(Mp3Decoder *dec, s32 gr, s32 ch);
s32 func_151F4F38(Mp3Decoder *dec, s32 gr, s32 ch);
s32 func_151F578C(Mp3Decoder *dec, s32 gr, s32 ch);
s32 func_151F63C4(Mp3Decoder *dec, s32 gr, s32 ch);
s32 func_151F6970(Mp3Decoder *dec, s32 gr);
s32 func_151F6B28(Mp3Decoder *dec, s32 gr, s32 ch);
s32 func_151F6FD0(Mp3Decoder *dec);
s32 func_151F78B4(Mp3Decoder *dec);
s32 func_151F7F60(Mp3Decoder *dec);
s32 func_151F8088(Mp3Decoder *dec, s32 seekPos);
Mp3Decoder *func_151F85C4(s32 handle, Mp3ReadFn read, s32 arg2);
s32 func_151F86B0(Mp3Decoder *decoder, u8 **outBlock, s32 *outChannels);

#endif
