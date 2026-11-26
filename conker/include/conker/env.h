#ifndef CONKER_ENV_H
#define CONKER_ENV_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "env": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_3FC60.c
 *   game_FECC0.c
 *   game_13D350.c
 *   game_1A0790.c
 *   game_1A0E60.c
 */

/* A queued sky tile (game_13D350.c): its loaded texture and its format (the index into the per-format load tables). */
typedef struct SkyTile {
    s32 texture;
    s32 format;
} SkyTile;

/* game_3FC60.c */
void func_150127B0(void);
void func_15012C84(Vtx *arg0, u32 arg1);
void func_15012ED8(Gfx *dl);
void func_15012F90(void);
extern u8 (*D_800BE510[4])[3]; // [0]: base RGB of every level vertex, then the props'; [i]: level model i's first vertex
extern u8 (***D_800BE520)[3]; // [set][group][i]: base RGB of each color-group vertex
extern u16 *D_800BE524;       // first D_800BE510[0] entry of each prop

/* game_FECC0.c */
Gfx *func_150D1810(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, f32 tScroll, s32 arg5, f32 roll);

/* game_13D350.c */
extern void *D_800DBE80;
extern Vtx *D_800DBE88[4][2];
extern u8 D_800DBEA8[3];
extern u8 D_800DBEAB;
extern u8 D_800DBEAC;
extern Mtx *D_800DBEB0;
extern s32 *D_800891BC[];
Gfx *func_1510FEA0(Gfx *gfx, s32 arg1);
void func_151102CC(MtxF mtx, f32 pitch, f32 yaw, f32 roll);
void func_15110360(s32 viewIndex, MtxF mtx, f32 pitch, f32 yaw, f32 roll);
Gfx *func_151103C8(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);
Gfx *func_15110544(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry, u8 r, u8 g, u8 b);
Gfx *func_15110600(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);
Gfx *func_151106A8(Gfx *gfx);
s32 func_151108C4(f32 x, f32 y, f32 z, f32 pitch, f32 yaw);
Gfx *func_15110CFC(Gfx *gfx, Vtx **vtxBufPtr, s32 firstTex, Vtx *quad, s32 arg4, u32 width, u32 height, u32 tileW,
                   u32 tileH, f32 x, f32 y, f32 z, u8 axis, MtxF mtx, s32 limit, s32 *vtxCountPtr, s32 *vtxIdxPtr,
                   SkyTile *queue, s32 *lastFormatPtr, u8 *sizes, u8 *shifts, s16 *palOffsets, u8 *tlutCounts,
                   u8 *tiles, s32 texSet);
void func_1511172C(s32 sky);
void func_15111858(void);
Gfx *func_15111AF4(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent);

/* game_1A0790.c */
extern s8 D_800DD2E0;     /* 1 when the level has its own fog preset */
extern u8 D_800DD2E4[4];  /* the active fog: colour */
extern u16 D_800DD2E8;
extern u16 D_800DD2EA;
extern u16 D_800DD2EC;
extern u16 D_800DD2EE;
extern s16 D_800DD2F0;    /* blend of the last func_151733E4 call */
extern s16 D_800DD2F2;    /* near plane */
extern s16 D_800DD2F4;    /* far plane */
void func_151732E0(s32 arg0);
Gfx *func_151733D8(Gfx *gfx, s32 arg1);
void func_151733E4(s32 from, s32 to, s32 arg2, s32 blend, s32 dest);
void func_151738C4(s32 arg0);
s8 func_15173994(s32 arg0);

/* game_1A0E60.c */
void func_151739B0(s32 colorSet, s32 isProp, s32 blend, s32 group, s32 propIndex);
void func_15173C60(s32 blend, s32 group);
void func_15173C90(s32 blend, s32 group, s32 propId);

#endif
