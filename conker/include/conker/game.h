#ifndef CONKER_GAME_H
#define CONKER_GAME_H

#include <ultra64.h>
#include <PR/sched.h>
#include "structs.h"

/*
 * Subsystem "game": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_34ED0.c
 *   game_34F20.c
 *   game_3B920.c
 *   game_45B80.c
 *   game_49BE0.c
 *   game/done/game_49D10.c
 *   game/done/game_49D20.c
 */

/* game/done/game_34ED0.c */
void func_15007A20(void);

/* game_34F20.c */
void func_15007A70(s16 arg0, s16 arg1, s16 arg2);
void func_15007B3C(void);
void func_150081E4(void);
void func_15008230(void);
void func_15008248(s32 arg0);
void func_150082CC(void);

/* game_3B920.c */
void func_1500E470(s32 levelId);

/* game_45B80.c */
void func_150186D0(void);
s32 func_1501878C(void);
void func_15018DFC(void);
Gfx *func_15018E88(Gfx *gfx, s16 player);
Gfx *func_15018F08(Gfx *gfx, s16 player);
void func_15018F80(s16 player);
void func_1501905C(void);
s32 func_15019130(void);
void func_15019414(void);
Gfx *func_15019464(Gfx *gfx, s16 player);
Gfx *func_150195A0(Gfx *gfx, s16 player);
Gfx *func_150198FC(Gfx *gfx);
Gfx *func_15019BB8(Gfx *gfx, s32 player);
Gfx *func_15019CC8(Gfx *gfx, s32 player);
Gfx *func_15019E60(Gfx *gfx);
void func_15019F20(void);
extern u8 D_800BE5E0; // the ticks of this frame
extern f32 D_800BE5E8[10]; // the last 10 frame deltas
extern u32 D_800BE610; // the leftover tick remainder
extern u8 D_800BE614;
extern u8 D_800BE615;
extern u8 D_800BE616;
extern u8 D_800BE617;
extern s8 D_800BE618;
extern u8 D_800BE619;
extern u8 D_800BE61A;

/* game_49BE0.c */
void func_1501C730(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern u8 D_800BE9A0; // frame steps of this frame (half the ticks, see func_15019F20)
extern f32 D_800BE9A4; // frame delta: half the ticks of this frame
extern f32 D_800BE9A8; // 1 / D_800BE9A4 (0 when that is 0)
extern s32 D_800BE9AC; // ticks since the start
extern s32 D_800BE9B0; // D_800BE9AC % 30
extern u8 D_800BE9B4;
extern f32 D_800BE9B8; // the average of the last 10 frame deltas
extern f32 D_800BE9BC; // 1 / D_800BE9B8
extern u8 D_800BE9C0; // which of the two matrix buffers is being built this frame
extern u16 *D_800BE9C4; // z-buffer (D_800BE620 x D_800BE624)
extern Gfx *D_800BE9C8[2]; // display-list buffer per framebuffer (D_800BE9C0)
extern Gfx *D_800BE9D0; // display-list write head
extern Gfx *D_800BE9D8[2]; // per framebuffer: end of the prologue built by func_1501A39C
extern OSMesg D_800BE9E0;
extern s32 D_800BE9E4; // frames elapsed this update
extern s32 D_800BE9E8; // number of players minus one
extern u8 D_800BE9EC;
extern u8 D_800BE9ED;
extern s32 level; // the current map
extern s32 D_800BE9F4; // current scene / game mode id
extern s32 D_800BE9F8; // the previous map
extern s32 D_800BEA00;
extern s32 D_800BEA04;
extern s32 D_800BEA08; // the ticks of this frame, before the D_800BEA0C divider
extern u8 D_800BEA0C; // frame rate divider (0: none; 1: time stands still)
extern OSMesgQueue D_800BEA10; // graphics task done messages
extern OSMesg D_800BEA28[16]; // D_800BEA10's buffer
extern OSScMsg D_800BEA68[2]; // "task done" messages, one per frame buffer
extern s16 D_800BEAA8; // graphics tasks sent
extern u8 D_800BEAAA;
extern u8 D_800BEAAB;
extern u8 D_800BEAC0;
extern u8 D_800BEAC1;
extern u8 D_800BEAC2;
extern u8 D_800BEAC3;

/* game/done/game_49D10.c */
void func_1501C860(void);

/* game/done/game_49D20.c */
void func_1501C870(void);

#endif
