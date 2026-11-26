#ifndef CONKER_SAVE_H
#define CONKER_SAVE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "save": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_33990.c
 *   game_20A3A0.c
 *   game_20AE20.c
 */

/* Per-team x jitter for stacked names (game_20AE20.c); a struct so the initial copy is one word. */
typedef struct Offs4 {
    s8 v[4];
} Offs4;

/* game_33990.c */
void func_150064E0(void);
void func_15006590(s32 eepromSlot);
void func_15006BEC(s32 saveSlot);
void func_1500707C(s32 saveSlot);
void func_15007168(void);
void func_1500727C(void);
void func_15007360(void);
void func_15007440(void);
void func_15007558(void);
void func_15007644(void);
void func_1500764C(void);
void func_15007668(void);
void func_15007684(void);
void func_150076A0(); /* defined with no parameters, but game_20AE20.c passes one */
void func_150076BC(s32 eepromSlot);
void func_15007718(s32 saveSlot);
void func_15007750(s32 saveSlot);
void func_15007778(void);
extern s16 D_800BE3DC;
extern u8 D_800BE3DE;
extern s8 D_800BE3DF;
extern u8 D_800BE3E0;
extern u32 D_800BE3E4; // saved clock (seconds)

/* game_20A3A0.c */
s32 func_151DCEF0(OSMesgQueue *mq, u8 block, u8 *buffer, s32 size); /* timed EEPROM write request */
s32 func_151DCFD8(s32 arg0);

/* game_20AE20.c */
void func_151DD970(void);
void func_151DD9E4(void);
s32 func_151DDB94(s32 value);
void func_151DDBA0(void);
void func_151DDC20(void);
void func_151DE6CC(void);
void func_151DE6D4(void);
void func_151DE7D4(void);
void func_151DE81C(void);
void func_151DE85C(void);
void func_151DE8E8(void);
void func_151DE8F0(void);
void func_151DF1BC(void);
void func_151DF574(void);
void func_151DFF38(void);
void func_151E0424(void);
void func_151E09DC(void);
void func_151E0B70(void);
void func_151E1214(void);
void func_151E1744(void);
void func_151E2284(void);
void func_151E22BC(void);
void func_151E2404(void);
s32 func_151E24F0(s32 *out);
void func_151E2834(void);
void func_151E30C4(void);
void func_151E327C(void);
void func_151E3344(s32 optionMask, s32 carousel, s32 showArrows, u8 alpha, s8 fade);
void func_151E4264(void);
void func_151E4314(void);
void func_151E43DC(void);
void func_151E4BD8(void);
void func_151E4DC4(void);
void func_151E4DD8(void);
void func_151E4E00(void);
void func_151E4E64(void);
void func_151E4EE8(void);
void func_151E5034(void);
void func_151E50C8(void);
void func_151E51EC(void);
void func_151E530C(void);
void func_151E53E8(s32 flag);
void func_151E557C(void);
void func_151E55A8(void);
void func_151E562C(void);
s8 func_151E564C(void);
s32 func_151E565C(void);
s32 func_151E5F64(s32 player);
s8 func_151E5FAC(void);
void func_151E5FF4(Actor *source, Actor *victim, s32 cause);
void func_151E6964(s32 bank);
void func_151E6BFC(void);
void func_151E6C1C(s32 index);
void func_151E7DC0(void);
void func_151E7E9C(void);
void func_151E7EF8(void);
void func_151E7F60(s32 slotIdx, s32 charIdx);
void func_151E81EC(void);
void func_151E8214(void);
void func_151E82B8(void);
void func_151E83E8(void);
extern s8 D_8008FD70;
extern u8 D_8008FD74;
extern s32 D_8008FD78;
extern s8 D_8008FD7C;
extern u8 D_8008FD80;
extern s8 D_8008FD84[4];
extern s8 D_8008FD88;
extern s8 D_8008FD8C;
extern s8 D_8008FD90;
extern s8 D_8008FD94;
extern s8 D_8008FD98;
extern s8 D_8008FD9C;
extern s8 D_8008FDA0; /* extra (non-controller) slots in the player list */
extern u8 D_8008FDA4;
extern s8 D_8008FDA8;
extern s16 D_8008FDAC;
extern s8 D_8008FDB0; /* highlighted row of the func_151DF574 list */
extern u8 D_8008FDB4;
extern u8 D_8008FDB8;
extern u16 D_8008FDBC;
extern u16 D_8008FDC0;
extern s8 D_8008FDC4;
extern s8 D_8008FDC8;
extern s16 D_8008FDCC; /* func_151DDC20 reads its low byte */
extern f32 D_8008FDD0;
extern MenuState *D_8008FDD4;
extern s32 D_8008FDD8;
extern s8 D_8008FDDC;
extern s8 D_8008FDE0;
extern s8 D_8008FDE4;
extern f32 D_8008FDE8;
extern void (*D_8008FDEC[12])(void);
extern f32 D_8008FE1C;
extern f32 D_8008FE20;
extern s32 D_8008FE24;
extern s8 D_8008FE28;
extern s8 D_8008FE2C;
extern s8 D_8008FE30;
extern u8 D_8008FE34;
extern f32 D_8008FE38;
extern f32 D_8008FE3C;
extern s8 D_8008FE40; /* number of players in D_8008FE44 */
extern s8 D_8008FE44[16]; /* controller of each player; -2: extra slot, -1: unused */
extern s8 D_8008FE54[23]; /* settings defaults */
extern s8 D_8008FE6C[23]; /* settings minimums */
extern s8 D_8008FE84[23]; /* settings maximums */
extern s8 D_8008FE9C[8];
extern u32 D_8008FEA4[8];
extern s8 D_8008FEC4[32];
extern s8 D_8008FEE4[8]; /* per team preset: row of D_800AB74C */
extern s8 D_8008FEEC;
extern s8 D_8008FEF0;
extern s8 D_8008FEF8;
extern s8 D_8008FF00;
extern u8 D_8008FF04;
extern u8 D_8008FF08[28];
extern Offs4 D_8008FF34;
extern s8 D_8008FF38[4]; /* indexed from 1 through D_8008FF37 */
extern u8 D_8008FF40[6];
extern f32 D_8008FF48[6];
extern f32 D_8008FF60[6];
extern s8 D_8008FF78[6];
extern s8 D_8008FF80[6];
extern u8 D_8008FF88[8];
extern f32 D_8008FF90[8];
extern u8 D_8008FFB0[2][6];
extern Actor *D_800E0BA0[4]; /* characters spawned by func_151E7F60 */
extern s8 D_800E0BB0;
extern s8 D_800E0BB1;
extern u8 D_800E0BB8[16];
extern s8 D_800E0BC8[4];
extern u16 D_800E0BCC;
extern s8 D_800E0BCE[2];
extern s8 D_800E0BD0;
extern u8 D_800E0BD1;
extern s8 D_800E0BD2;
extern s8 D_800E0BD3;
extern s32 D_800E0BD4;
extern u8 **D_800E0BD8; // text strings of the current language ([8 + n]: player names)
extern u8 D_800E0BDC;
extern s8 D_800E0BE0[0x1C];

#endif
