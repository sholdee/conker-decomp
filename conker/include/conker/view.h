#ifndef CONKER_VIEW_H
#define CONKER_VIEW_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "view": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_3F820.c
 *   game_3F950.c
 *   game_433F0.c
 *   game_476D0.c
 *   game_138520.c
 *   game_138B40.c
 */

/* game_3F820.c */
void func_15012370(void);
void func_15012470(void);

/* game_3F950.c */
void func_150124A0(void);

/* game_433F0.c */
void func_15015F40(s16 count, s32 map);
void func_15015FBC(s32 width, s32 height);

/* game_476D0.c */
void func_1501A220(s32 numPlayers, s32 arg1);
void func_1501A39C(void);
Gfx *func_1501A490(Gfx *gfx, s16 viewIndex, s32 arg2, s32 arg3, s32 maxBottom, s32 mode);
Gfx *func_1501A680(Gfx *gfx);
Gfx *func_1501A6CC(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry);
void func_1501A764(s16 view, f32 x, f32 y, f32 z, f32 *screenX, f32 *screenY, f32 *depth);
void func_1501A8C0(s32 view, s32 layout, s32 zRange, s16 arg3);
s32 func_1501AE94(s32 viewIndex);
void func_1501AF44(f32 *left, f32 *top, f32 *right, f32 *bottom);
void func_1501B0A0(s32 view, s32 layout);
void func_1501B22C(s32 viewIndex);
extern s32 D_80082FA0; // the screen layout: the number of players
extern s32 D_80082FA4; // the view being drawn
extern s32 D_800BE620; // the screen width
extern s32 D_800BE624; // the screen height
extern RenderView *D_800BE628; // the views
extern Vp *D_800BE62C; // one viewport per view
extern u8 D_800BE630[5]; // the player of each view
extern u8 D_800BE635;
extern s32 D_800BE638[6]; // the view width of each screen layout
extern s32 D_800BE650[6]; // the view height of each screen layout
extern s16 D_800BE668[4][5][2]; // the centre (x, y) of each view, by screen layout
extern f32 D_800BE6B8; // the width scale of the two-player layout
extern Gfx D_80082FC0[24][2]; // render mode tables for segment 8
extern Gfx D_80083140[24][2];
extern Gfx D_800832C0[24][2];
extern Gfx D_80083440[24][2];
extern Gfx D_800835C0[24][2];
extern Gfx D_80083740[24][2];
extern Gfx D_800838C0[24][2];
extern Gfx D_80083A40[24][2];
extern Gfx D_80083BC0[24][2];
extern Gfx D_80083D40[24][2];
extern Gfx D_80083EC0[24][2];

/* game_138520.c */
void func_1510B070(s32 arg0);
void func_1510B128(s32 viewIndex, f32 fovXDelta, f32 fovYDelta, f32 scale, f32 near);
void func_1510B32C(s32 viewIndex, f32 fovXDelta, f32 fovYDelta, f32 scale);
void func_1510B3B0(void);
void func_1510B458(s32 viewIndex, f32 near, f32 far);
void func_1510B51C(void);
void func_1510B5F8(s32 viewIndex, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);

/* game_138B40.c */
void func_1510B690(void);
Gfx *func_1510B7B4(Gfx *gfx, s32 view);
void func_1510B958(s32 view);
Gfx *func_1510B9D0(Gfx *gfx, s16 view);

#endif
