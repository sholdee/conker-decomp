#ifndef CONKER_HUD_H
#define CONKER_HUD_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "hud": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Hud/PlayerOverlay/playeroverlay.c
 *   game/Hud/CounterFree/counterfree.c
 *   game/Hud/HudOverlay/hudoverlay.c
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
    /* 0x24 */ AnimInstance *anim; /* created by anim_createInstance */
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

/* game/Hud/PlayerOverlay/playeroverlay.c */
void hud_resetBubbles(void);
void hud_updateOverlay(void);
void hud_updateBlockStack(void);
void hud_removeBlock(void);
void hud_addBlock(void);
void hud_updateBubbles(void);
Gfx *hud_drawOverlay(Gfx *gfx);
Gfx *hud_drawBlocks(Gfx *gfx, s32 fixed);
Gfx *hud_drawBubbles(Gfx *gfx);
void hud_updateWaterSurface(void);
Gfx *hud_drawWaterSurface(Gfx *gfx);
Gfx *hud_loadIconSheet(Gfx *gfx, s32 sheet, u8 *loaded);
void hud_printRaceTime(s32 x, s32 y, s32 frames, s32 style);
Gfx *hud_drawMultiplayerHud(Gfx *gfx);
void hud_setClockEnabled(s32 enable);
void hud_loadClock(void);
Gfx *hud_drawClock(Gfx *gfx);
Gfx *hud_drawRadar(Gfx *gfx, s32 step, s16 *scaled, s32 renderMode, u8 large);
extern const s16 gHudBlockStackPath[6]; /* the block stack's start [0..2] and target [3..5] position */
extern Mtx gHudProjection;          /* the HUD's projection matrix */
extern f32 gHudBlockScales[6];       /* per block: its scale while it grows in */
extern f32 gHudBlockStackPos[3];       /* the block stack's position */
extern f32 gHudBlockStackWobble[3];       /* the stack's wobble */
extern s32 gHudBlockStackTimer;          /* the stack's display timer */
extern s32 gHudClockTime;          /* the race time in frames */
extern u16 gHudPerspNorm;          /* perspNorm for gHudProjection */
extern u8 gHudBlockCount;           /* blocks on the stack */
extern u8 gHudBlockTarget;           /* blocks it moves to */
extern u8 gHudClockShown;           /* the stack is shown */
extern BlockDebris *gHudBlockDebris; /* the debris list */
extern Bubble gHudBubbles[6];
extern s32 gHudBubbleCooldown;
extern WaterStage gHudWaterStage;
extern Gfx *gHudBlockDl;         /* display list built by game/Hud/PlayerOverlay/playeroverlay */

/* game/Hud/CounterFree/counterfree.c */
void hud_freeCounter(CounterEffect *effect);
void hud_counterMessage(EffectHeader *obj, s32 arg1, u8 msg);

/* game/Hud/HudOverlay/hudoverlay.c */
Gfx *hud_drawMain(Gfx *gfx);
Gfx *hud_drawModeOverlay(Gfx *gfx);
Gfx *hud_texRectScaled(Gfx *gfx, s32 xl, s32 yl, s32 xh, s32 yh, s32 tile, s32 s, s32 t, s32 dsdx, s32 dtdy);
Gfx *hud_drawTeamScores(Gfx *gfx, s32 unused, s32 alpha);
Gfx *hud_drawScorePips(Gfx *gfx, s32 y, s32 extraPipTeam, s8 loadTexture, u8 refresh);
Gfx *hud_drawTeamTotals(Gfx *gfx, s32 rowY, s32 refresh);
Gfx *hud_drawResultsTable(Gfx *gfx, s32 topY, s32 alpha, s32 live);
Gfx *hud_drawNumber(Gfx *gfx, s32 x, s32 y, s32 value);
Gfx *hud_drawSinglePlayerHud(Gfx *gfx);
Gfx *hud_drawResults(Gfx *gfx);
Gfx *hud_drawFadingMessage(Gfx *gfx);
Gfx *func_151EBB50(Gfx *gfx);
Gfx *func_151EC178(Gfx *gfx);
Gfx *func_151EC1F0(Gfx *gfx);
Gfx *func_151EC3E8(Gfx *gfx);
Gfx *func_151EC648(Gfx *gfx);
Gfx *func_151ED09C(Gfx *gfx);
Gfx *hud_darkenScreen(Gfx *gfx);
Gfx *hud_setupSpriteTiles(Gfx *gfx, HudSprite *sprite, s32 *blockSizeOut);
Gfx *hud_drawSprite(Gfx *gfx, HudSprite *sprite, s32 x, s32 y, s32 columns, s32 rows, f32 scale, s32 frame);
HudModel *hud_loadModel(s32 loadArg0, s32 loadArg1, s32 animArg0, f32 animArg1);
void hud_freeModel(HudModel *model);
Gfx *hud_drawModel(Gfx *gfx, HudModel *model, f32 x, f32 y, f32 scale);
void hud_initCodeEntry(void);
Gfx *hud_updateCodeEntry(Gfx *gfx);
Gfx *hud_drawImagePanel(Gfx *gfx, s32 index);
void hud_clearCodeFlags(void);
extern Gfx *(*gHudModeOverlayDrawers[13])(Gfx *);
extern Gfx *(*gHudModeDrawers[13])(Gfx *);
extern Gfx gHudSpriteDl[];
extern HudModel *D_80090058;
extern HudModel *D_8009005C;
extern HudSprite gHudSprite;
extern u8 gScoreDigitCols[4];
extern u8 gScoreDigitRows[3];
extern s32 gHudDigitTextures[10];
extern PlaceValueTable gPlaceValues;
extern Gfx D_80090110[];
extern Gfx D_80090128[];
extern Struct151EDF4C *D_80090138;
extern HudSprite D_8009013C;

/* Texture asset ids of the HUD. Each is a link-time symbol whose address is the id (undefined_syms_auto.txt);
 * the code passes the address, and &D_D16 + n is the id 0xD16 + n. */
extern u8 D_507;
extern u8 D_508;
extern u8 D_7D3;
extern u8 D_7D4;
extern u8 D_7F1;
extern u8 D_7F2;
extern u8 D_7F6;
extern u8 D_7FB;
extern u8 D_7FC;
extern u8 D_80E;
extern u8 D_82F;
extern u8 D_830;
extern u8 D_843;
extern u8 D_85D;
extern u8 D_87B;
extern u8 D_887;
extern u8 D_888;
extern u8 D_88C;
extern u8 D_891;
extern u8 D_89C;
extern u8 D_CF1;
extern u8 D_CF2;
extern u8 D_D10;
extern u8 D_D13;
extern u8 D_D14;
extern u8 D_D16;
extern u8 D_11AD;
extern u8 D_1647;
extern u8 D_1648;
extern u8 D_1653;
extern u8 D_1654;

#endif
