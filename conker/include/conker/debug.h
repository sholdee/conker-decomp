#ifndef CONKER_DEBUG_H
#define CONKER_DEBUG_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "debug": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_6CEA0.c
 *   game_6D800.c
 *   game_6E240.c
 */

/* game_6CEA0.c */
void debug_getStickRepeat(s16 controller, s16 *dirX, s16 *dirY);
void debug_resetTaskMenu(void);
s32 debug_drawTaskMenu(Gfx **gfxp, u16 backButtons, u16 selectButtons);
extern s16 D_80084480;

/* game_6D800.c */
void debug_checkDl(void *dl, u32 end);
void debug_walkCheckDl(void *dl, u32 end, Gfx *parent);
s32 debug_isMisalignedDl(s32 addr);
void debug_cmdDefault(Gfx *gfx);
s32 debug_isBadVtxCmd(Gfx *gfx);
void debug_cmdVtx(void);
void debug_cmdModifyVtx(Gfx *gfx);
void debug_cmdCullDl(Gfx *gfx);
void debug_cmdBranchZ(Gfx *gfx);
void debug_cmdLine3D(Gfx *gfx);
void debug_cmdRdpHalf1(Gfx *gfx);
void debug_cmdRdpHalf2(Gfx *gfx);
void debug_cmdSetKeyGB(Gfx *gfx);
void debug_cmdSetKeyR(Gfx *gfx);
void debug_cmdQuad(Gfx *gfx);
void debug_cmdTri2(Gfx *gfx);
void debug_cmdTri4(Gfx *gfx);
s32 debug_isBadTri1Cmd(Gfx *tri);
void debug_cmdTri1(Gfx *gfx);
void debug_cmdLoadTlut(Gfx *gfx);
void debug_cmdLoadTile(Gfx *gfx);
void debug_cmdRdpTileSync(Gfx *gfx);
void debug_cmdRdpFullSync(Gfx *gfx);
void debug_cmdRdpPipeSync(Gfx *gfx);
void debug_cmdRdpLoadSync(Gfx *gfx);
void debug_cmdSetCombine(Gfx *gfx);
s32 debug_isBadDlCmd(Gfx *gfx);
void debug_cmdDl(Gfx *gfx);
void debug_cmdSetOtherModeH(Gfx *gfx);
void debug_cmdSetOtherModeL(Gfx *gfx);
void debug_cmdSetConvert(Gfx *gfx);
void debug_cmdSetScissor(Gfx *gfx);
void debug_cmdNoop(Gfx *gfx);
void debug_cmdSpNoop(Gfx *gfx);
void debug_cmdSetCImg(Gfx *gfx);
void debug_cmdSetZImg(Gfx *gfx);
void debug_cmdSetTImg(Gfx *gfx);
void debug_cmdSetEnvColor(Gfx *gfx);
void debug_cmdSetPrimColor(Gfx *gfx);
void debug_cmdSetBlendColor(Gfx *gfx);
void debug_cmdSetFogColor(Gfx *gfx);
void debug_cmdSpecial3(Gfx *gfx);
void debug_cmdSpecial2(Gfx *gfx);
void debug_cmdSpecial1(Gfx *gfx);
void debug_cmdDmaIo(Gfx *gfx);
void debug_cmdSetFillColor(Gfx *gfx);
void debug_cmdFillRect(Gfx *gfx);
void debug_cmdTexRect(Gfx *gfx);
void debug_cmdTexRectFlip(Gfx *gfx);
void debug_cmdLoadBlock(Gfx *gfx);
void debug_cmdSetTile(Gfx *gfx);
void debug_cmdSetTileSize(Gfx *gfx);
void debug_cmdEndDl(Gfx *gfx);
void debug_cmdPopMtx(Gfx *gfx);
void debug_cmdMtx(Gfx *gfx);
void debug_cmdGeometryMode(Gfx *gfx);
void func_15040A4C(void);
void debug_cmdLoadUcode(Gfx *gfx);
void debug_cmdMoveMem(Gfx *gfx);
void debug_cmdMoveWord(Gfx *gfx);
void debug_dumpDlRange(Gfx *start, Gfx *from, Gfx *to);
void debug_dumpDlAround(Gfx *gfx);
void debug_cmdSetPrimDepth(Gfx *gfx);
void debug_cmdTexture(Gfx *gfx);
void debug_cmdRdpSetOtherMode(Gfx *gfx);

/* game_6E240.c */
void func_15040D90(s32 arg0);
void debug_drawFilledMarker(s32 arg0);
void debug_drawHollowMarker(s32 arg0);
void debug_busyWait(void);

#endif
