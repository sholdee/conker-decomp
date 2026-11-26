#ifndef CONKER_TEXTURE_H
#define CONKER_TEXTURE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "texture": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_139410.c
 *   game_139FC0.c
 *   game_13AAE0.c
 *   game_13ABD0.c
 */

/* game_139410.c */
/* Scrolls the texture coordinates of one mesh's vertices along a circle. */
typedef struct UvWobble {
    /* 0x00 */ TexCoord *primary;   /* original UVs of the vertices in mesh 0 */
    /* 0x04 */ TexCoord *secondary; /* original UVs of the vertices in mesh 1 */
    /* 0x08 */ u8 primaryCount;
    /* 0x09 */ u8 secondaryCount;
    /* 0x0A */ u8 primaryIndex;     /* first vertex in D_800B0E10[0] */
    /* 0x0B */ u8 secondaryIndex;   /* first vertex in D_800B0E10[1] */
    /* 0x0C */ u8 disabled;
    /* 0x0D */ u8 pad0D;
    /* 0x0E */ u16 phase;
    /* 0x10 */ u16 rate;
    /* 0x12 */ u16 amplitude;
} UvWobble;

extern u8 D_800D9E64;

s32 func_1510BF60(Gfx *dl, s32 tex, Vtx **outVtx, s32 skip);
UvWobble *func_1510C4AC(s32 tex, s32 disabled, s32 rate, s32 amplitude);
void func_1510C8A8(void);

/* game_139FC0.c */
extern u16 D_800D9E70[4][3];
extern u8 D_800D9E88[4][3];
extern u8 D_800D9E98[4][3];
extern u8 D_800D9EA8[4][3];
extern u8 D_800D9EB4[3];
extern u8 D_800D9EB8[4][3];
extern const u8 D_800A2CD0[7][3];
extern const u8 D_800A2CE8[1][3];
extern const u8 D_800A2CEC[7][3];
extern const u8 D_800A2D04[7][3];
void func_1510CB10(s32 slot);
Gfx *func_1510CDB8(Gfx *gfx, s32 primAlpha, s32 envAlpha, s32 slot);
s32 func_1510CE60(Gfx *dl, s32 strict, u32 countUse, s32 lifetime, s16 **usedOut);
s32 func_1510D0EC(s32 index, s32 *sizeOut, s32 lifetime, s32 countUse); /* texture address of a texture id */
s32 func_1510D374(s32 index);
void func_1510D404(void);
void func_1510D608(s32 index, s32 lifetime);

/* game_13AAE0.c */
/* list: a count-prefixed array of s16 texture ids (built by func_1510CE60), freed after it is released. */
void func_1510D630(s16 *list);
void func_1510D694(s32 id);

/* game_13ABD0.c */
extern s32 D_800D9F58;
extern s32 D_800D9F5C;
extern u8 D_800D9F60;
extern u8 D_800D9F68[7762];
extern u8 D_800DBDBA;
extern s32 D_800DBDBC;
void func_1510D720(s32 index);
void func_1510D7AC(u32 index);
void func_1510D864(void);
void func_1510D874(void *object, s32 texture, s32 palette, s32 textureSegment, s32 paletteSegment);
Gfx *func_1510D8C0(Gfx *gfx, void *object);

#endif
