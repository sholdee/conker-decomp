#ifndef CONKER_HUD_H
#define CONKER_HUD_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "hud": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_BC510.c
 *   game_1ED080.c
 *   game_215960.c
 */

/* A 3D model drawn on the HUD: its display lists, patched copies, and a transform per frame buffer. */
typedef struct {
    /* 0x00 */ Gfx **srcDisplayLists;
    /* 0x04 */ Gfx *displayLists[4];
    /* 0x14 */ u8 displayListCount;
    /* 0x15 */ u8 blinkTimer;
    /* 0x16 */ u8 unk16[6];
    /* 0x1C */ u8 *segment1Base; /* the model's vertex data: segment 1 base */
    /* 0x20 */ s32 unk20;
    /* 0x24 */ AnimInstance *anim; /* created by func_1503F62C */
    /* 0x28 */ MtxF matrices[2];
} HudModel;

typedef struct {
    /* 0x00 */ s32 v[7];
} PlaceValueTable;

typedef struct {
    Gfx *dl;
    s32 *data;
    s16 *usedTextures; /* the distinct textures dl uses (texture_resolveDl's usedOut) */
} Struct151EDF4C;

/* game_BC510.c */
void func_1508F060(void);
void func_1508F0A4(void);
void func_1508F0D4(void);
void func_1508F7BC(void);
void func_1508F9C4(void);
void func_1508F9F4(void);
Gfx *func_1508FD38(Gfx *gfx);
Gfx *func_150900F0(Gfx *gfx, s32 fixed);
Gfx *func_15090630(Gfx *gfx);
void func_1509093C(void);
Gfx *func_150911F4(Gfx *gfx);
Gfx *func_15091534(Gfx *gfx, s32 sheet, u8 *loaded);
void func_150916B4(s32 x, s32 y, s32 frames, s32 style);
Gfx *func_150918EC(Gfx *gfx);
void func_15093818(s32 enable);
void func_15093878(void);
Gfx *func_150938BC(Gfx *gfx);
Gfx *func_15093B58(Gfx *gfx, s32 step, s16 *scaled, s32 renderMode, u8 large);
extern const s16 D_8009DCB4[6]; /* the block stack's start [0..2] and target [3..5] position */
extern Mtx D_800D23D0;          /* the HUD's projection matrix */
extern f32 D_800D2410[6];       /* per block: its scale while it grows in */
extern f32 D_800D2428[3];       /* the block stack's position */
extern f32 D_800D2438[3];       /* the stack's wobble */
extern s32 D_800D2444;          /* the stack's display timer */
extern s32 D_800D2450;          /* the race time in frames */
extern u16 D_800D2454;          /* perspNorm for D_800D23D0 */
extern u8 D_800D2456;           /* blocks on the stack */
extern u8 D_800D2457;           /* blocks it moves to */
extern u8 D_800D2458;           /* the stack is shown */
extern BlockDebris *D_800D245C; /* the debris list */
extern Bubble D_800D2460[6];
extern s32 D_800D24C0;
extern WaterStage D_800D24C8;
extern Gfx *D_800D2588;         /* display list built by game_BC510 */

/* game_1ED080.c */
void func_151BFBD0(CounterEffect *effect);
void func_151BFBFC(EffectHeader *obj, s32 arg1, u8 msg);

/* game_215960.c */
Gfx *func_151E84B0(Gfx *gfx);
Gfx *func_151E8620(Gfx *gfx);
Gfx *func_151E86E4(Gfx *gfx, s32 xl, s32 yl, s32 xh, s32 yh, s32 tile, s32 s, s32 t, s32 dsdx, s32 dtdy);
Gfx *func_151E89A0(Gfx *gfx, s32 unused, s32 alpha);
Gfx *func_151E966C(Gfx *gfx, s32 y, s32 extraPipTeam, s8 loadTexture, u8 refresh);
Gfx *func_151E9D18(Gfx *gfx, s32 rowY, s32 refresh);
Gfx *func_151EA15C(Gfx *gfx, s32 topY, s32 alpha, s32 live);
Gfx *func_151EADFC(Gfx *gfx, s32 x, s32 y, s32 value);
Gfx *func_151EB06C(Gfx *gfx);
Gfx *func_151EB930(Gfx *gfx);
Gfx *func_151EB96C(Gfx *gfx);
Gfx *func_151EBB50(Gfx *gfx);
Gfx *func_151EC178(Gfx *gfx);
Gfx *func_151EC1F0(Gfx *gfx);
Gfx *func_151EC3E8(Gfx *gfx);
Gfx *func_151EC648(Gfx *gfx);
Gfx *func_151ED09C(Gfx *gfx);
Gfx *func_151ED1E0(Gfx *gfx);
Gfx *func_151ED29C(Gfx *gfx, HudSprite *sprite, s32 *blockSizeOut);
Gfx *func_151ED430(Gfx *gfx, HudSprite *sprite, s32 x, s32 y, s32 columns, s32 rows, f32 scale, s32 frame);
HudModel *func_151ED90C(s32 loadArg0, s32 loadArg1, s32 animArg0, f32 animArg1);
void func_151EDB58(HudModel *model);
Gfx *func_151EDBDC(Gfx *gfx, HudModel *model, f32 x, f32 y, f32 scale);
void func_151EDF4C(void);
Gfx *func_151EE184(Gfx *gfx);
Gfx *func_151EEBE8(Gfx *gfx, s32 index);
void func_151EEFF0(void);
extern Gfx *(*D_8008FFC0[13])(Gfx *);
extern Gfx *(*D_8008FFF4[13])(Gfx *);
extern Gfx D_80090028[];
extern HudModel *D_80090058;
extern HudModel *D_8009005C;
extern HudSprite D_80090060;
extern u8 D_8009006C[4];
extern u8 D_80090070[3];
extern s32 D_80090074[10];
extern PlaceValueTable D_8009009C;
extern Gfx D_80090110[];
extern Gfx D_80090128[];
extern Struct151EDF4C *D_80090138;
extern HudSprite D_8009013C;

#endif
