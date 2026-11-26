#ifndef CONKER_COLOURFB_H
#define CONKER_COLOURFB_H

#include <ultra64.h>
#include "structs.h"
#include "effects.h"

/*
 * Subsystem "colourfb": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_18D250.c
 */

/* Object node of type 0x34; the effect's own data (dataSize bytes) follows at 0x18. */
typedef struct ScreenEffect {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ struct ScreenEffect *next;
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ EffectDesc desc;
    /* 0x16 */ u8 pad16[2];
} ScreenEffect;

/* Effect data filled in by func_151600D8 from the debug tunables. */
typedef struct {
    /* 0x00 */ f32 pos[2];  /* s, t */
    /* 0x08 */ f32 size[2]; /* s, t */
    /* 0x10 */ s32 pixelation;
} ColourFrameBufferData;

typedef struct {
    /* 0x00 */ ScreenEffect header;
    /* 0x18 */ ColourFrameBufferData data;
} ColourFrameBufferEffect;

/* game_18D250.c */
void func_1515FDA0(s32 arg0);
ScreenEffect *func_1515FF74(EffectDesc *desc, s32 dataSize, u8 owner, s32 list);
void func_1515FFEC(ScreenEffect *effect);
void func_15160090(ScreenEffect *effect, s32 data, u8 event);
s32 func_151600D8(ColourFrameBufferEffect *effect);
/* (s32 data, u8 event); declared without a prototype: game_1F7050.c passes a third argument */
void func_15160274();

#endif
