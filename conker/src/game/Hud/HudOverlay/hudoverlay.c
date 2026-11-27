#include <ultra64.h>
#include "structs.h"

/* .bss, linked at 0x800E0C30..0x800E0C90 in this order. */
static s16 gSinglePlayerHudAlpha; /* alpha of the single-player HUD (fades in) */
static Mtx gHudOrthoMtx; /* the HUD's orthographic projection */
static s16 D_800E0C78;
static u8 *gCodeEntryText; /* text entry buffer (0x1A8 bytes) */
static s8 gCodeEntryColumn;  /* text entry cursor column */
static s8 gCodeEntryRow;  /* text entry cursor row */
static s8 gCodeEntryKeyPress;
static s8 gCodeEntryLength;
static s8 gCodeEntryRepeatCount;
static u8 gHudImagePanelImage; /* not referenced */
static u8 gHudSlotAlphas[4]; /* alpha of each player slot */

#include <math.h>
#include "conker/actor.h"
#include "conker/anim.h"
#include "conker/boot.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/cutscene.h"
#include "conker/env.h"
#include "conker/game.h"
#include "conker/heap.h"
#include "conker/hud.h"
#include "conker/level.h"
#include "conker/math.h"
#include "conker/mp.h"
#include "conker/mp3.h"
#include "conker/os.h"
#include "conker/player.h"
#include "conker/rom.h"
#include "conker/save.h"
#include "conker/script.h"
#include "conker/sound.h"
#include "conker/sprite.h"
#include "conker/text.h"
#include "conker/texture.h"
#include "conker/view.h"

/* Per-mode (gFrontEndState) overlay drawers, called by hud_drawModeOverlay. */
Gfx *(*gHudModeOverlayDrawers[13])(Gfx *) = {
    hud_drawSinglePlayerHud, NULL, NULL, hud_drawMenuOverlay, NULL, NULL, hud_drawResults,
    NULL, NULL, NULL, NULL, func_151EBB50, NULL,
};

/* Per-mode (gFrontEndState) HUD drawers, called by hud_drawMain. */
Gfx *(*gHudModeDrawers[13])(Gfx *) = {
    NULL, func_151EC178, func_151EC1F0, NULL, func_151ED09C, NULL, NULL,
    NULL, hud_drawFadingMessage, NULL, NULL, NULL, NULL,
};

/* Render state for the HUD sprites: textured, white environment color. */
Gfx gHudSpriteDl[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetEnvColor(0xFF, 0xFF, 0xFF, 0xFF),
    gsDPSetCombine(0x12D225, 0xFFA7FFFF),
    gsDPSetOtherMode(0x002C3F, 0x504244),
    gsSPEndDisplayList(),
};

HudModel *D_80090058 = NULL;
HudModel *D_8009005C = NULL;
HudSprite gHudSprite = { (void *)0x7BC, 0x100, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };
/* texture_getAddress returns this instead of a texture address when it fails. */
#define TEXTURE_NOT_LOADED ((s32)0x80000000)

/* Draws the HUD overlay: shared HUD, the per-mode overlay, and a blinking warning message. */
Gfx *hud_drawMain(Gfx *gfx) {
    s32 message;

    gDebugStatusCode = 0x9000001;
    message = 0;
    gfx = hud_darkenScreen(gfx);
    if (gHudModeDrawers[gFrontEndState] != NULL) {
        gfx = gHudModeDrawers[gFrontEndState](gfx);
    }

    if (D_80000300 != 0) {
        if (gIsMultiplayer != 0 && gHumanCount >= 2) {
            if ((gConnectedPads & 0xF) == 0) {
                if (D_800E0BD3 == 1) {
                    message = 0x33;
                } else if (D_800E0BD3 == 2) {
                    message = 0x16;
                }
            }
        } else {
            if ((gConnectedPads & 1) == 0) {
                if (D_800E0BD3 == 1) {
                    message = 0x32;
                } else if (D_800E0BD3 == 2) {
                    message = 0x15;
                }
            }
        }
    }

    if (message != 0) {
        text_setColor(0xFF, 0xFF, 0xFF, 0xFF);
        text_printAt(0x94, 0xC8, 0x81, (char *)gTextStrings[message]);
    }

    gDebugStatusCode = 0;
    return gfx;
}


/* Draws the per-mode overlay; if it overran the display list buffer, discards what it wrote. */
Gfx *hud_drawModeOverlay(Gfx *gfx) {
    s32 overflow;
    Gfx *start;
    s32 mode;

    mode = gFrontEndState;
    start = gfx;
    gDebugStatusCode = 0x9000000;
    if (gHudModeOverlayDrawers[mode] != NULL) {
        gfx = gHudModeOverlayDrawers[mode](gfx);
    }
    if (gFrontEndState != 0) {
        D_80090058 = NULL;
        D_800E0C78 = 0;
    }
    gDebugStatusCode = 0;
    if (gfx - gDisplayListBuffers[gDrawBufferIndex] > gDisplayListLimit) {
        overflow = TRUE;
    } else {
        overflow = FALSE;
    }
    if (overflow) {
        return start;
    }
    return gfx;
}

/* Emits a scissored texture rectangle, scaled by the current screen scale factors. */
Gfx *hud_texRectScaled(Gfx *gfx, s32 xl, s32 yl, s32 xh, s32 yh, s32 tile, s32 s, s32 t, s32 dsdx, s32 dtdy) {
    f32 yScale;

    if (gHudScaleX != 1.0f) {
        xl = xl * gHudScaleX;
        yScale = gHudScaleY;
        xh = xh * gHudScaleX;
        yl = yl * yScale;
        yh = yh * yScale;
        dsdx = dsdx / gHudScaleX;
        dtdy = dtdy / gHudScaleY;
    }
    gSPScisTextureRectangle(gfx++, xl, yl, xh, yh, tile, s, t, dsdx, dtdy);
    return gfx;
}



/* RGBA colour of each team: red, blue, green, yellow. */
static const u8 gTeamColors[4][4] = {
    { 255, 0, 0, 255 }, { 0, 0, 255, 255 }, { 0, 255, 0, 255 }, { 255, 255, 0, 255 },
};
u8 gScoreDigitCols[4] = { 1, 8, 15, 21 };
u8 gScoreDigitRows[3] = { 0, 8, 16 };

/* Draws the multiplayer score HUD: team scores, target and team icons, and arrows. */
Gfx *hud_drawTeamScores(Gfx *gfx, s32 unused, s32 alpha) {
    s32 zero; // matching: indexing D_800E0BCC through a zero variable keeps &D_800E0BCC in a register
    s32 x;
    s32 y;
    s32 i;
    s32 image;
    s32 score;
    s32 place;
    s32 spacing;
    s32 digit;
    s32 digitX;
    s32 team;
    s32 width;
    s32 kind;
    ActorCtl *sub;
    Actor *obj;
    u8 nearTarget;
    u8 flags;
    s8 started;
    s8 done;
    s32 rowOffset;
    s32 t;
    f32 px;
    f32 py;
    f32 pz;
    s32 s;

    flags = 0;
    gMatchFlags &= ~0x10;
    gSPDisplayList(gfx++, gHudSpriteDl);
    gDPSetCombine(gfx++, 0x12FE25, 0xFFFFF3F9);
    if (gScreenLayout == 0) {
        y = 0x320;
    } else {
        y = 0x198;
    }
    spacing = 0x124;
    if (D_800E0BB0 > 0) { spacing = 0x124 / D_800E0BB0; } // matching: one line (-g3 line numbers steer scheduling)
    zero = 0;
    D_800E0BCC = 0;
    nearTarget = 0;
    team = -1;
    for (i = 0; i < gPlayerCount; i++) {
        obj = &movement_obj[i];
        sub = obj->mv;
        kind = sub->context_b & 0x7F;
        if ((obj->lifting != 0) || (sub->unk128 & 0x20)) {
            team = obj->team;
            (&D_800E0BCC)[zero] = D_800E0BCC | (1 << i);
        }
        if ((team != -1) && (gMatchHudFlags & 0x8000)) {
            if (team == 0) {
                flags |= 2;
            } else {
                flags |= 1;
            }
            team = -1;
        }
        if ((obj->team == 1) && (obj->taskint[0] != 0) && (gMatchHudFlags & 0x4000)) {
            flags |= 4;
        }
        if (kind == 0x25) {
            mp_getNodePos(mp_findNodeById(0x49, i), &px, &py, &pz);
            px -= obj->xpos;
            pz -= obj->zpos;
            pz = px * px + pz * pz;
            if (pz < 6250000.0f) {
                gMatchFlags |= 0x10;
                nearTarget = 1;
            }
            {
                s32 bit = 1 << i;
                (&D_800E0BCC)[zero] = D_800E0BCC | bit;
            }
        }
    }
    if ((gTotalTicks & 0x1F) < 0xB) {
        D_800E0BCC = 0;
        nearTarget = 0;
        team = -1;
        flags = 0;
    }
    image = 0;
    if (gMatchHudFlags & 9) {
        image = texture_getAddress((s32)&D_D14, 0, 3, 0);
        if (image != TEXTURE_NOT_LOADED) {
            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
            gDPLoadSync(gfx++);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
            gDPPipeSync(gfx++);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
            gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
            gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
        } else {
            image = 0;
        }
    }
    if ((gMatchHudFlags & 8) && (image != 0)) {
        x = spacing >> 1;
        for (i = 0; i < D_800E0BB0; i++) {
            score = player_getStat(i, 6);
            started = 0;
            done = 0;
            gDPPipeSync(gfx++);
            {
                u8 *color = (u8 *)gTeamColors + (i << 2); // matching: shift form, not gTeamColors[i]
                gDPSetEnvColor(gfx++, color[0], color[1], color[2], alpha);
            }
            gfx = hud_texRectScaled(gfx, (x + 3) << 2, y, (x + 10) << 2, y + 0x1C, 0, 0x1C0, 0x200, 0x400, 0x400);
            digitX = (x + 10) << 2;
            place = 10000;
            do {
                digit = score / place;
                if ((digit != 0) || started || (place == 1)) {
                    started = 1;
                    s = gScoreDigitCols[digit & 3] << 5;
                    t = gScoreDigitRows[digit >> 2] << 5;
                    gfx = hud_texRectScaled(gfx, digitX, y, digitX + 0x14, y + 0x1C, 0, s, t, 0x400, 0x400);
                    digitX += 0x18;
                }
                score -= digit * place;
                if (place == 1) {
                    done = 1;
                }
                place /= 10;
            } while (!done);
            x += spacing;
        }
    }
    if (gMatchHudFlags & 1) {
        gfx = hud_drawScorePips(gfx, y, team, 0, 1);
    }
    if (nearTarget || (team != -1)) {
        if (gMatchHudFlags & 0x4000) {
            i = (s32)(&D_D16 + 1); // matching: the loop counter doubles as the texture id
        } else {
            i = (s32)&D_D16;
        }
        image = texture_getAddress(i, 0, 3, 0);
        if (image != TEXTURE_NOT_LOADED) {
            gDPSetTextureImage(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, image);
            gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 6, 0, G_TX_CLAMP, 6, 0);
            gDPLoadSync(gfx++);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
            gDPPipeSync(gfx++);
            gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_4b, 4, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, 0, G_TX_CLAMP, 6, 0);
            gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0xFC, 0xFC);
            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, image + 0x800);
            gDPLoadSync(gfx++);
            gDPLoadTLUTCmd(gfx++, 6, 15);
            gDPSetOtherMode(gfx++, 0x00AC3F, 0x504244);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
            if (nearTarget) {
                gfx = hud_texRectScaled(gfx, 0x238, 0xC, 0x268, 0x48, 0, 0x640, 0x660, 0x400, 0x400);
            }
            if (team != -1) {
                if (gMatchHudFlags & 0x4000) {
                    gfx = hud_texRectScaled(gfx, 0x220, 0xC, 0x250, 0x48, 0, 0x600, 0x540, 0x400, 0x400);
                } else {
                    rowOffset = 0;
                    if (gMatchHudFlags & 1) {
                        t = 0x1A;
                    } else {
                        t = 0x26;
                        rowOffset = 0xB;
                    }
                    gfx = hud_texRectScaled(gfx, 0x238, (rowOffset + 3) << 2, 0x268, (rowOffset + 0xE) << 2, 0, 0x640, t << 5, 0x400, 0x400);
                }
            }
        }
    }
    width = 0x20;
    if (flags) {
        if (flags & 4) {
            i = (s32)(&D_D10 + 1);
            width = 0x10;
        } else {
            i = (s32)&D_D10;
        }
        image = texture_getAddress(i, 0, 3, 0);
        if (image != TEXTURE_NOT_LOADED) {
            gDPPipeSync(gfx++);
            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
            gDPLoadSync(gfx++);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, (width * 32) - 1, 0);
            gDPPipeSync(gfx++);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, ((width * 2) + 7) >> 3, 0, G_TX_RENDERTILE, 0,
                       G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
            gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, (width - 1) << 2, 0x7C);
            gDPSetOtherMode(gfx++, 0x000C3F, 0x504244);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, 0xFF);
            if (flags & 1) {
                x = y - 0x10U; // matching: unsigned subtract steers IDO register allocation
                gfx = hud_texRectScaled(gfx, 0xA0, x, 0xE0, x + 0x40, 0, 0, 0x200, 0x400, 0x400);
            }
            if (flags & 2) {
                s32 yy = y - 0x10U;
                gfx = hud_texRectScaled(gfx, 0x3B0, yy, 0x3F0, yy + 0x40, 0, 0x200, 0x200, 0x400, 0x400);
            }
            if (flags & 4) {
                gfx = hud_texRectScaled(gfx, 0x250, 0xC, 0x280, 0x48, 0, 0, 0x200, 0x400, 0x400);
            }
        }
    }
    if (gMatchHudFlags & 0x6340) {
        gfx = hud_drawTeamTotals(gfx, y, 1);
    }
    return gfx;
}



/* Draws each team's row of score pips; teams with no active player get a separate icon. */
Gfx *hud_drawScorePips(Gfx *gfx, s32 y, s32 extraPipTeam, s8 loadTexture, u8 refresh) {
    s8 inactiveTeams;
    s32 image;
    s32 i;
    s32 j;
    s32 filled;
    s32 pipX;
    s32 x;
    s32 spacing;

    inactiveTeams = 0;
    if (loadTexture != 0) {
        image = texture_getAddress((s32)&D_D14, 0, 3, 0);
        if (image == TEXTURE_NOT_LOADED) {
            return gfx;
        }
        gDPPipeSync(gfx++);
        gDPSetCombine(gfx++, 0x12FE25, 0xFFFFF3F9);
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
    }
    spacing = 0x124;
    if (D_800E0BB0 > 0) {
        spacing = 0x124 / D_800E0BB0;
    }
    if (refresh) {
        inactiveTeams = 0xF;
        for (i = 0; i < gPlayerCount; i++) {
            if (player_getStat(i, 3)) {
                inactiveTeams &= ~(1 << gRosterTeams[i]);
            }
        }
    }
    x = spacing >> 1;
    for (i = 0; i < D_800E0BB0; i++) {
        if (refresh) {
            filled = player_getStat(i, 6);
            D_800E0AA0[i] = filled;
        } else {
            filled = D_800E0AA0[i];
        }
        if (!(inactiveTeams & (1 << i))) {
            if (i == extraPipTeam) {
                filled++;
            }
            gDPPipeSync(gfx++);
            {
                u8 *color = (u8 *)gTeamColors + (i << 2); // matching: shift form, not gTeamColors[i]
                gDPSetEnvColor(gfx++, color[0], color[1], color[2], 0xFF);
            }
            pipX = x - (D_80087268 * 4);
            for (j = 0; j < D_80087268; j++) {
                if (j == filled) {
                    gDPPipeSync(gfx++);
                    gDPSetEnvColor(gfx++, 0x40, 0x40, 0x40, 0x40);
                }
                gfx = hud_texRectScaled(gfx, pipX << 2, y, (pipX + 7) << 2, y + 0x1C, 0, 0x1C0, 0x200, 0x400, 0x400);
                pipX += 8;
            }
        }
        x += spacing;
    }
    if (inactiveTeams) {
        image = texture_getAddress((s32)&D_D16 + 1, 0, 3, 0);
        if (image == TEXTURE_NOT_LOADED) {
            return gfx;
        }
        gDPPipeSync(gfx++);
        gDPSetTextureImage(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, image);
        gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 6, 0, G_TX_CLAMP, 6, 0);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_4b, 4, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 6, 0, G_TX_CLAMP, 6, 0);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0xFC, 0xFC);
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, image + 0x800);
        gDPLoadSync(gfx++);
        gDPLoadTLUTCmd(gfx++, 6, 15);
        gDPSetOtherMode(gfx++, 0x00AC3F, 0x504244);
        x = spacing >> 1;
        for (i = 0; i < D_800E0BB0; i++) {
            if (inactiveTeams & (1 << i)) {
                gDPPipeSync(gfx++);
                {
                    const u8 *color = gTeamColors[i];
                    gDPSetEnvColor(gfx++, color[0], color[1], color[2], 0xFF);
                }
                gfx = hud_texRectScaled(gfx, (x - 8) << 2, y - 0x18, (x + 8) << 2, y + 0x28, 0, 0x600, 0x280, 0x400, 0x400);
            }
            x += spacing;
        }
    }
    gDPPipeSync(gfx++);
    return gfx;
}

/* Draws the two team icons with each team's total score beside them. */
Gfx *hud_drawTeamTotals(Gfx *gfx, s32 rowY, s32 refresh) {
    s32 tex;
    s32 iconT;
    s32 image;
    s32 width;
    s32 amount;
    s32 team0Total;
    s32 team1Total;
    s32 y;
    s32 leftIconS;
    s32 rightIconT;
    s32 i;

    y = rowY - 0x10;
    if ((gMatchHudFlags & 0x4000) || (gMenuState->selection == 8)) {
        tex = (s32)((gMenuState->selection == 8) ? &D_D10 + 2 : &D_D10 + 1);
        width = 0x10;
        leftIconS = 0;
        rightIconT = 0x200;
    } else {
        tex = (s32)&D_D10;
        width = 0x20;
        leftIconS = 0x200;
        rightIconT = 0;
    }

    image = texture_getAddress(tex, 0, 3, 0);
    if (image == TEXTURE_NOT_LOADED) {
        return gfx;
    }
    gDPPipeSync(gfx++);
    gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
    gDPLoadSync(gfx++);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, (width * 32) - 1, 0);
    gDPPipeSync(gfx++);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, ((width * 2) + 7) >> 3, 0, G_TX_RENDERTILE, 0,
               G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, (width - 1) << 2, 0x7C);
    gDPSetOtherMode(gfx++, 0x000C3F, 0x504244);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, 0xFF);

    iconT = 0;
    team0Total = 0;
    team1Total = 0;
    if (refresh != 0) {
        if (gMatchHudFlags & 0x6040) {
            team0Total = player_getStat(0, 6);
            team1Total = player_getStat(1, 6);
        } else if (gMatchHudFlags & 0x100) {
            for (i = 0; i < gPlayerCount; i++) {
                amount = player_getStat(i, 3);
                if (amount < 0) {
                    amount = 0;
                }
                if (movement_obj[i].team == 0) {
                    team0Total += amount;
                } else {
                    team1Total += amount;
                }
            }
        } else {
            iconT = 0x200;
            for (i = 0; i < gPlayerCount; i++) {
                amount = gMenuState->score[i];
                if (amount < 0) {
                    amount = 0;
                }
                if (gRosterTeams[i] == 0) {
                    team0Total += amount;
                } else {
                    team1Total += amount;
                }
            }
        }
        D_800E0AA0[0] = team0Total;
        D_800E0AA0[1] = team1Total;
    } else {
        team0Total = D_800E0AA0[0];
        team1Total = D_800E0AA0[1];
    }

    gfx = hud_texRectScaled(gfx, 0x108, y, 0x148, y + 0x40, 0, leftIconS, iconT, 0x400, 0x400);
    gfx = hud_texRectScaled(gfx, 0x318, y, 0x358, y + 0x40, 0, 0, iconT + rightIconT, 0x400, 0x400);
    text_setColor(0xC0, 0xC0, 0xC0, 0xFF);
    text_printAt(0x4F, (y >> 2) + 1, 0x80, " %d", team1Total);
    text_printAt(0xD3, (y >> 2) + 1, 0x80, " %d", team0Total);
    return gfx;
}

/* Draws the per-player results table (name, team score, percentage and stat columns). */
Gfx *hud_drawResultsTable(Gfx *gfx, s32 topY, s32 alpha, s32 live) {
    s32 image;
    s32 y;
    s32 i;
    s32 team;
    s32 icon;
    s32 xbase;
    s32 x;
    s32 iconY;
    s32 nameBase;
    s32 iconBottom;
    s32 percent;
    s32 total;
    s32 nameIndex;
    s8 showTeam0;
    s8 texturesLoaded;
    s8 refreshScores;

    if ((gMenuState->selection == 0xB) || (gMenuState->selection == 0xC)) {
        if (live != 0) {
            return gfx;
        }
        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
        gDPSetCombine(gfx++, 0x12D225, 0xFFA7FFFF);
        image = texture_getAddress((s32)&D_843, 0, 3, 0);
        if (image == TEXTURE_NOT_LOADED) {
            return gfx;
        }
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
        y = 0x48;
        for (i = 0; i < 4; i++) {
            if (gRoster[i] >= 0) {
                icon = gRoster[i] & 3;
                gfx = hud_texRectScaled(gfx, 0x120, (y - 2) << 2, 0x160, (y + 0xE) << 2, 0,
                                    (icon & 1) << 9, (icon >> 1) << 9, 0x400, 0x400);
                y += 0x12;
            }
        }
        return gfx;
    }

    y = topY;
    if (gMatchHudFlags & 1) {
        xbase = 0x1A;
    } else {
        xbase = 0x34;
    }
    if (gFrontEndState != 0) {
        if (gFreeForAll != 0) {
            y = topY - 0x28;
        }
    }
    text_setColor(0xFF, 0xFF, 0xFF, alpha);
    if (D_800E0BD3 == 2) {
        nameBase = 9;
        refreshScores = 1;
    } else {
        nameBase = 0x5E;
        refreshScores = 0;
    }
    if (gMenuState->selection == 8) {
        showTeam0 = 0;
    } else {
        showTeam0 = 1;
    }
    gDPPipeSync(gfx++);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
    gDPSetCombine(gfx++, 0x12D225, 0xFFA7FFFF);
    texturesLoaded = 0;
    for (i = 0; i < 4; i++) {
        if (gRoster[i] < 0) {
            continue;
        }
        if (gDefaultCharacters[i] == 0xA) {
            continue;
        }
        team = gRosterTeams[i];
        if (team != 0 || (team == 0 && showTeam0)) { // matching: redundant team == 0 test is in the original
            if (texturesLoaded == 0) {
                image = texture_getAddress((s32)&D_887, 0, 3, 0);
                if (image == TEXTURE_NOT_LOADED) {
                    return gfx;
                }
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPLoadSync(gfx++);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPPipeSync(gfx++);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
                gDPSetOtherMode(gfx++, 0x000C3F, 0x504244);
                x = (xbase - 0x28) << 2;
                if (gMatchHudFlags & 1) {
                    iconY = (y - 0x11) << 2;
                    gfx = hud_texRectScaled(gfx, x + 0x1D4, iconY, x + 0x214, iconY + 0x40, 0, 0x200, 0, 0x400, 0x400);
                    x += 0x78;
                }
                iconY = (y - 0x11) << 2;
                x += 0x1D4;
                if (gMenuState->selection == 1) {
                    x -= 0x78;
                }
                gfx = hud_texRectScaled(gfx, x + 0x164, iconY, x + 0x1A4, iconY + 0x3C, 0, 0, 0x200, 0x400, 0x400);
                if (gMenuState->selection == 1) {
                    x += 0x78;
                }
                gDPPipeSync(gfx++);
                image = texture_getAddress((s32)&D_888, 0, 3, 0);
                if (image == TEXTURE_NOT_LOADED) {
                    return gfx;
                }
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPLoadSync(gfx++);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPPipeSync(gfx++);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
                iconBottom = iconY + 0x40;
                gfx = hud_texRectScaled(gfx, x, iconY, x + 0x40, iconBottom, 0, 0x200, 0, 0x400, 0x400);
                x += 0x74;
                gfx = hud_texRectScaled(gfx, x, iconY, x + 0x40, iconBottom, 0, 0, 0, 0x400, 0x400);
                if (gMenuState->selection != 1) {
                    x += 0x4C;
                    gfx = hud_texRectScaled(gfx, x, iconY, x + 0x80, iconBottom, 0, 0, 0x200, 0x400, 0x400);
                }
                gDPPipeSync(gfx++);
                image = texture_getAddress((s32)&D_843, 0, 3, 0);
                if (image == TEXTURE_NOT_LOADED) {
                    return gfx;
                }
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPLoadSync(gfx++);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPPipeSync(gfx++);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
                gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
                texturesLoaded = 1;
            }
            text_setColor(gTeamColors[team][0], gTeamColors[team][1], gTeamColors[team][2], alpha);
            percent = 0;
            icon = gRoster[i] & 3;
            gfx = hud_texRectScaled(gfx, (xbase + 8) << 2, (y - 2) << 2, (xbase + 0x18) << 2, (y + 0xE) << 2, 0,
                                (icon & 1) << 9, (icon >> 1) << 9, 0x400, 0x400);
            if (live != 0) {
                total = movement_obj[i].mv->shotsfired;
            } else {
                total = gMenuState->stats[i].shotsFired;
            }
            if (total > 0) {
                percent = (gMenuState->stats[i].hits * 100) / total;
            }
            if (percent >= 10000) {
                percent = 9999;
            }
            if (gFreeForAll != 0) {
                team = i;
            }
            x = xbase;
            nameIndex = (s8)(gPlayerRanks[team] - 1);
            if (nameIndex < 0) {
                nameIndex = 0;
            }
            text_printAt(x + 0x3C, y, 0x81, "%s", gTextStrings[nameIndex + nameBase]);
            if (gMatchHudFlags & 1) {
                text_printAt(xbase + 0x54, y, 0x81, "%d", D_800E0AA0[team]);
                x = xbase + 0x1E;
            }
            x += 0x58;
            text_printAt(x, y, 0x81, "%d%%", percent);
            x += 0x1A;
            text_printAt(x, y, 0x81, "%d", gMenuState->score[i]);
            x += 0x1E;
            if (gMenuState->selection != 1) {
                if ((gDefaultCharacters[i] == 9) || ((i == 0) && (D_8008FD7C == 9))) {
                    text_printAt(x, y, 0x81, "X");
                } else {
                    text_printAt(x, y, 0x81, "%d", gMenuState->stats[i].unk4);
                }
                x += 0x1E;
            }
            text_printAt(x, y, 0x81, "%d", D_800E0AD0[i]);
            y += 0xF;
        }
    }
    if (gMatchHudFlags & 0x6340) {
        gSPDisplayList(gfx++, gHudSpriteDl);
        gDPSetCombine(gfx++, 0x12FE25, 0xFFFFF3F9);
        gfx = hud_drawTeamTotals(gfx, 0x320, refreshScores);
    }
    if (live != 0) {
        if (gMatchHudFlags & 1) {
            gfx = hud_drawScorePips(gfx, 0x320, -1, 1, refreshScores);
        }
    }
    return gfx;
}

/* Texture ids of the digits 0-9. */
s32 gHudDigitTextures[10] = {
    0x8B2, 0x890, 0x8AB, 0x8A8, 0x882, 0x881, 0x8A2, 0x8A1, 0x880, 0x88F,
};
PlaceValueTable gPlaceValues = { { 1, 10, 100, 1000, 10000, 100000, 1000000 } };

/* Draws a number of up to seven digits, one 32x32 digit texture per place, without leading zeros. */
Gfx *hud_drawNumber(Gfx *gfx, s32 x, s32 y, s32 value) {
    s32 i;
    s32 digit;
    s32 started;
    s32 image;
    PlaceValueTable places;

    places = gPlaceValues;

    x <<= 2; y <<= 2;
    if (value >= 10000000) {
        value = 9999999;
    }
    started = 0;
    if (value < 0) {
        value = 0;
    }

    for (i = 6; i >= 0; i--) {
        digit = value / places.v[i];
        value = value % places.v[i];
        if ((digit > 0) || started || (i == 0)) {
            image = texture_getAddress(gHudDigitTextures[digit], 0, 3, 0);
            started = 1;
            if (image != TEXTURE_NOT_LOADED) {
                gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPLoadSync(gfx++);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
                gDPPipeSync(gfx++);
                gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
                gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
                gfx = hud_texRectScaled(gfx, x, y, x + 0x80, y + 0x80, 0, 0, 0, 0x400, 0x400);
            }
            x += 0x60;
        }
    }

    return gfx;
}

/* Draws the single-player HUD (counters and models) after preloading its textures; fades it in. */
Gfx *hud_drawSinglePlayerHud(Gfx *gfx) {
    s32 show;
    s32 value;
    s32 x;
    s32 hasValue;
    s32 i;
    s32 start;
    static s8 D_800900B8 = 0;

    show = (gTaskDoneBits[0x19] & 4) || (gCurrentSaveSlot < 0);
    if (gIsMultiplayer && !gPaused && !gPauseRequested) {
        if (!save_isFreeForAll() || gMenuState->selection == 1) {
            gfx = hud_drawTeamScores(gfx, 0, 0xFF);
        }
    } else {
        D_800E0C78 = 0;
    }
    if (!gPaused || D_8008FEF0) {
        if (D_80090058 != NULL) {
            hud_freeModel(D_80090058);
            hud_freeModel(D_8009005C);
            D_80090058 = NULL;
        }
        D_8008FDCC = 0;
        D_800900B8 = 0;
        return gfx;
    }
    if (gLevelHeader->skyMode == 4 && D_800900B8 == 0) {
        if (level == LEVEL_W2_HARBOUR) {
            s32 id;
            id = gSkyTextureSets[gLevelHeader->skyTextureSet][2];
            for (i = 0; i < 0x48; i++, id++) {
                if (id != (s32)&D_1647 && id != (s32)&D_1648 && id != (s32)(&D_1648 + 1) &&
                    id != (s32)&D_1653 && id != (s32)&D_1654 && id != (s32)(&D_1654 + 1)) {
                    texture_releaseId(id);
                }
            }
            start = 0x4E;
        } else {
            start = 0;
        }
        for (i = start; i < 0x168; i++) {
            texture_releaseId(gSkyTextureSets[gLevelHeader->skyTextureSet][2] + i);
        }
    }
    if (D_800900B8 < 5) {
        D_800900B8++;
        return gfx;
    }
    D_8008FDCC += 0x20;
    if (D_8008FDCC >= 0x100) {
        D_8008FDCC = 0xFF;
    }
    gSPDisplayList(gfx++, gHudSpriteDl);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_8008FDCC);
    gHudSprite.texId = &D_891;
    {
        s32 remaining;
        s32 pad; // matching: stack slot
        texture_getAddress((s32)&D_7D3, &remaining, 3, 0);
        texture_getAddress((s32)&D_7D4, &remaining, 3, 0);
        texture_getAddress((s32)&D_7FC, &remaining, 3, 0);
        texture_getAddress((s32)(&D_7FC + 1), &remaining, 3, 0);
        texture_getAddress((s32)&D_7F1, &remaining, 3, 0);
        texture_getAddress((s32)&D_7F2, &remaining, 3, 0);
        texture_getAddress((s32)&D_82F, &remaining, 3, 0);
        texture_getAddress((s32)&D_830, &remaining, 3, 0);
        if (gIsMultiplayer) {
            texture_getAddress((s32)&D_80E, &remaining, 3, 0);
            texture_getAddress((s32)(&D_80E + 1), &remaining, 3, 0);
        }
        if (!gIsMultiplayer) {
            f32 halfWidth;
            value = player_getStat(0, 6);
            if (value == 0x7D00) {
                value = 1000000;
            }
            if (script_query(0, 0x5082, 0x1A) && !script_query(0, 0x5083, 0x1A)) {
                Struct1509B570 *entry = script_getTaskBss(0x83);
                if (entry != NULL) {
                    if (entry->unk64 == 0) {
                        value = 0;
                    } else if (entry->unk64 < 3) {
                        value = (s32)(value * (u32)entry->unk64) / 3;
                    }
                } else {
                    value = 0;
                }
            }
            if (gCutsceneStates[0] == 1 && gCutsceneCashOffset != -1) {
                if (gCutsceneCashOffset == 0) {
                    value = 0;
                } else {
                    value = (gCutsceneCashOffset == 0x7D00) ? 1000000 : value - gCutsceneCashOffset;
                }
            }
            if (value > 0) {
                hasValue = 1;
            } else {
                hasValue = 0;
            }
            gfx = hud_drawSprite(gfx, &gHudSprite, 0x94, 0x3C, 3, 1, 1.0f, 0);
            if (D_80090058 == NULL) {
                D_80090058 = hud_loadModel(0xA4, 0x17, 0, 1.0f);
                D_8009005C = hud_loadModel(0xA2, 8, 0, 1.0f);
            }
            gSPSetGeometryMode(gfx++, 0x220404);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, 0xFF);
            gfx = hud_drawBlocks(gfx, 1);
            halfWidth = gViews[0].width * 0.5f;
            func_151EF954(&gHudOrthoMtx, -halfWidth, halfWidth, -(gViews[0].height * 0.5f), gViews[0].height * 0.5f, 1.0f, 20000.0f, 1.0f);
            gSinglePlayerHudAlpha = D_8008FDCC;
            if (D_80090058 != NULL) {
                i = 10;
                if (hasValue) {
                    x = -0x10;
                    while (i < value) {
                        x -= 0x10;
                        i *= 10;
                    }
                    gfx = hud_drawModel(gfx, D_80090058, x + -10.0f, -83.0f, 0.2699999809f);
                }
            }
            if (D_8009005C != NULL && show) {
                gfx = hud_drawModel(gfx, D_8009005C, 70.0f, 80.0f, 0.3000000119f);
            }
            gSPDisplayList(gfx++, gHudSpriteDl);
            gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
            if (show) {
                s32 count;
                count = player_getStat(0, 3);
                if (count != 0) {
                    count--;
                }
                if (count >= 100) {
                    count = 99;
                }
                gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_8008FDCC);
                gfx = hud_drawNumber(gfx, 0xEC, 0x1E, count);
            }
            if (hasValue) {
                s32 pad4; // matching: stack slot
                gDPPipeSync(gfx++);
                gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, D_8008FDCC);
                gHudSprite.texId = &D_7F6;
                gfx = hud_drawNumber(hud_drawSprite(gfx, &gHudSprite, x + 0xA5, 0xAA, 1, 1, 1.0f, 0), x + 0xAD, 0x9B, value);
            }
        } else {
            gfx = hud_drawResultsTable(hud_drawSprite(gfx, &gHudSprite, 0x94, 0x12, 3, 1, 1.0f, 0), 0x85, D_8008FDCC, 1);
        }
    }
    return gfx;
}


/* Draws the results table while it is faded in. */
Gfx *hud_drawResults(Gfx *gfx) {
    if (D_8008FDCC != 0) {
        gfx = hud_drawResultsTable(gfx, 0x6A, D_8008FDCC, 0);
    }
    return gfx;
}


/* Draws the current message, a run of text lines ended by a "*" line, fading in and out. */
Gfx *hud_drawFadingMessage(Gfx *gfx) {
    s32 pad; // matching: stack slot
    s16 alpha;
    s16 i;
    s32 count;
    u8 alpha8;
    s32 spacing;
    s32 x;
    s32 firstX;
    u8 *line;
    if (gCreditsLine < 0) {
        return gfx;
    }
    if (gFrontEndTimer < 0x168) {
        alpha = gFrontEndTimer << 2;
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
    } else {
        alpha = 0x1A9 - gFrontEndTimer;
        alpha <<= 2;
        if (alpha < 0) {
            alpha = 0;
        }
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
    }
    i = gCreditsLine;
    count = 0;
    alpha8 = alpha;
    while (gTextStrings[i][0] != '*') {
        i++;
        count++;
    }
    if (count >= 0xE) {
        spacing = 0xB;
    } else {
        spacing = 0x11;
    }
    text_setColor(0xFF, 0, 0, alpha8);
    x = (0xD0 - count * spacing) >> 1;
    i = gCreditsLine;
    line = gTextStrings[i];
    firstX = x;
    while (*line != '*') {
        text_printAt(0x94, x, 1, (char *)line);
        text_setColor(0xFF, 0xFF, 0xFF, alpha8);
        if (x == firstX) {
            x += 0xC;
        }
        i++;
        line = gTextStrings[i];
        x += spacing;
    }
    return gfx;
}

/* Draws a timed full-screen sequence over black: a growing title, a page of text lines, then an image. */
Gfx *func_151EBB50(Gfx *gfx) {
    s32 alpha;
    f32 scale;

    alpha = 0xFF - ((gFrontEndTimer - 0xF0) << 3);
    if (alpha < 0) {
        alpha = 0;
    }
    if (alpha >= 0x100) {
        alpha = 0xFF;
    }

    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++, 0x082C3F, 0x504340);
    gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFEFB7D);
    gDPSetEnvColor(gfx++, 0, 0, 0, 0xFF);
    gfx = view_fillRect(gfx, 0, 0, gScreenWidth, gScreenHeight);
    if (D_80000300 == 0) {
        return gfx;
    }
    gSPDisplayList(gfx++, gHudSpriteDl);
    if (gFrontEndTimer < 0xF0) {
        alpha = 0xFF - ((gFrontEndTimer - 0xB4) << 3);
        if (alpha < 0) {
            alpha = 0;
        }
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
        scale = gFrontEndTimer / 180.0f + 0.75f;
        if (scale > 1.5f) {
            scale = 1.5f;
        }
        text_setScale(scale);
        text_setColor(0xFF, 0, 0, alpha);
        text_printAt(0x94, 0x69, 1, (char *)gTextStrings[0x8E]);
        text_setScale(1.0f);
    } else if (gFrontEndTimer < 0x21C) {
        if (gFrontEndTimer < 0x12C) {
            alpha = (gFrontEndTimer - 0xF0) << 3;
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        } else {
            alpha = 0xFF - ((gFrontEndTimer - 0x1E0) << 3);
            if (alpha < 0) {
                alpha = 0;
            }
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        }
        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha >> 2);
        gfx = hud_drawSprite(gfx, &D_800917F8, 0x92, 0x67, 5, 6, 1.0f, 0);
        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
        gfx = hud_drawSprite(gfx, &D_800917EC, 0x92, 0x1C, 2, 1, 1.0f, 0);
        text_setColor(0xFF, 0xFF, 0xFF, alpha);
        text_printAt(0x94, 0x9D, 1, (char *)gTextStrings[0x81]);
        text_printAt(0x94, 0xAA, 1, (char *)gTextStrings[0x82]);
        text_printAt(0x94, 0xBC, 1, (char *)gTextStrings[0x83]);
        text_setScale(0.8999999762f);
        text_printAt(0x94, 0x2D, 1, (char *)gTextStrings[0x84]);
        text_printAt(0x94, 0x37, 1, (char *)gTextStrings[0x85]);
        text_printAt(0x94, 0x41, 1, (char *)gTextStrings[0x86]);
        text_printAt(0x94, 0x4B, 1, (char *)gTextStrings[0x87]);
        text_printAt(0x94, 0x55, 1, (char *)gTextStrings[0x88]);
        text_printAt(0x94, 0x5F, 1, (char *)gTextStrings[0x89]);
        text_printAt(0x94, 0x69, 1, (char *)gTextStrings[0x8A]);
        text_printAt(0x94, 0x73, 1, (char *)gTextStrings[0x8B]);
        text_printAt(0x94, 0x7D, 1, (char *)gTextStrings[0x8C]);
        text_printAt(0x94, 0x87, 1, (char *)gTextStrings[0x8D]);
        text_setScale(1.0f);
        gDPPipeSync(gfx++);
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
        gHudSprite.texId = &D_89C;
        gHudSprite.width = 0x10;
        gHudSprite.height = 0x10;
        gfx = hud_drawSprite(gfx, &gHudSprite, 0xC7, 0xB0, 1, 1, 1.0f, 0);
        gHudSprite.width = 0x20;
        gHudSprite.height = 0x20;
    } else {
        if (gFrontEndTimer < 0x258) {
            alpha = (gFrontEndTimer - 0x21C) << 3;
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        } else {
            alpha = 0xFF - ((gFrontEndTimer - 0x30C) << 3);
            if (alpha < 0) {
                alpha = 0;
            }
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
        }
        gDPSetEnvColor(gfx++, 0xFF, 0, 0, alpha);
        gHudSprite.fmt = 4;
        gHudSprite.siz = 1;
        gHudSprite.texId = &D_85D;
        gHudSprite.width = 0x40;
        gHudSprite.height = 0x40;
        gfx = hud_drawSprite(gfx, &gHudSprite, 0x94, 0x69, 3, 1, 1.0f, 0);
        gHudSprite.width = 0x20;
        gHudSprite.height = 0x20;
        gHudSprite.fmt = 0;
        gHudSprite.siz = 3;
    }
    return gfx;
}


/* Fades in a text line once the timer passes 0x5DC. */
Gfx *func_151EC178(Gfx *gfx) {
    s32 alpha;
    if (gFrontEndTimer >= 0x5DD) {
        alpha = (gFrontEndTimer - 0x5DC) << 3;
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
        text_setColor(0xFF, 0xFF, 0xFF, alpha);
        text_printAt(0xDC, 0x130, 1, (char *)gTextStrings[0x74]);
    }
    return gfx;
}

/* Draws a fading image and two overlaid sprites (the second tinted by gMenuPulse); updates the fade level gFrontEndFade. */
Gfx *func_151EC1F0(Gfx *gfx) {
    s32 alpha;
    s32 temp;

    if (level == LEVEL_WFRONTEND_TABLE) {
        return gfx;
    }

    gSPDisplayList(gfx++, gHudSpriteDl);
    temp = gFrontEndTimer; // matching: temp is reused for 0x1AC below
    alpha = temp;
    if (alpha >= 0x12D) {
        temp = 0x1AC;
        alpha = temp - alpha;
        alpha <<= 1;
        if (alpha < 0) {
            alpha = 0;
        }
    } else {
        alpha = temp << 3;
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
    }

    if (alpha != 0) {
        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
        gfx = hud_drawSprite(gfx, &D_800917F8, 0x92, 0x63, 5, 6, 1.0f, 0);
    }

    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, 0xFF);
    gfx = hud_drawSprite(gfx, &D_80091804, 0x92, 0xCB, 5, 2, 1.0f, 0);
    gDPSetCombine(gfx++, 0xFFD3FF, 0xFFA6FF7F);
    gDPSetEnvColor(gfx++, 0x20, 0xFF, 0x20, gMenuPulse);
    gfx = hud_drawSprite(gfx, &D_80091810, 0x92, 0xCB, 5, 2, 1.0f, 0);
    gfx = sprite_endDraw(gfx);

    alpha = 0x1EA - gDemoInputIndex;
    if (alpha < 0) {
        alpha = 0;
    } else {
        alpha <<= 4;
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
    }
    alpha = 0xFF - alpha;
    gFrontEndFade = alpha;
    return gfx;
}


/* Draws the fading image picked by gMenuState->displayedStop (or the func_151EC648 / hud_updateCodeEntry screens). */
Gfx *hud_drawMenuOverlay(Gfx *gfx) {
    static void *D_800900BC[6] = { (void *)0x88C, (void *)0x7F3, (void *)0x7CA, (void *)0x7E1, (void *)0x7DE, (void *)0x7DB };
    static u8 D_800900D4 = 0;
    s32 alpha;

    if (gCutsceneStates[0] == 1) {
        D_800900D4 = 0;
        return gfx;
    }
    alpha = D_800900D4 + (gFrameTicks * 4);
    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    D_800900D4 = alpha;
    gSPDisplayList(gfx++, gHudSpriteDl);
    if (gMenuState->menu == 1) {
        return func_151EC648(gfx);
    }
    if (gMenuState->menu == 0) {
        if (gMenuState->displayedStop == 1 && D_8008FEF8 != 0) {
            gfx = hud_updateCodeEntry(gfx);
        } else {
            alpha = (gMenuState->stepProgress - 0.5f) * 512.0f;
            if (alpha < 0) {
                alpha = -alpha;
            }
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
            if (D_800900D4 < alpha) {
                alpha = D_800900D4;
            }
            if (gMenuState->displayedStop == 1) {
                alpha = (gMenuPageFade * alpha) >> 8;
                if (alpha >= 0xFE) {
                    alpha = 0xFF;
                }
            }
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
            gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
            gHudSprite.texId = D_800900BC[gMenuState->displayedStop];
            gfx = hud_drawSprite(gfx, &gHudSprite, 0x94, 0x1E, 3, 1, 1.0f, 0);
        }
        if (gMenuState->pathProgress >= 0.5f) {
            if (gMenuState->displayedStop >= 3) {
                if (gSaveSlots[gMenuState->displayedStop - 3].sequence != -1) {
                    gfx = hud_drawImagePanel(gfx, gSaveSlots[gMenuState->displayedStop - 3].checkpointLevel);
                }
            }
        }
    }
    return gfx;
}

/* Draws the multi-page screen selected by gMenuState: page images, a rotating list, or per-slot panels. */
Gfx *func_151EC648(Gfx *gfx) {
    static u8 *D_800900D8[8] = {
        (u8 *)0x831, (u8 *)0x884, (u8 *)0x8A4, (u8 *)0x80B, (u8 *)0x7BC, (u8 *)0x7B9, (u8 *)0x822, (u8 *)0x808,
    };
    static s8 D_800900F8 = 0;
    static u8 *D_800900FC[5] = { (u8 *)0x7C0, (u8 *)0x7C6, (u8 *)0x7CF, (u8 *)0x825, (u8 *)0x81C };
    s32 fadeAlpha;
    s32 alpha;
    s32 pad; // matching: stack slot
    s32 y;
    MenuState *state;
    f32 scale;
    f32 progress;
    u8 **image;
    s8 slotCount;
    s8 page;
    s32 i;
    s32 j;
    s32 prev;
    s32 cur;
    s32 mid;
    s32 v;
    s32 listAlpha;
    f32 angle;
    s32 diff;
    f32 sc;

    fadeAlpha = (gMenuState->stepProgress - 0.5f) * 512.0f;
    if (fadeAlpha < 0) {
        fadeAlpha = -fadeAlpha;
    }
    if (fadeAlpha >= 0x100) {
        fadeAlpha = 0xFF;
    }
    gDPPipeSync(gfx++);
    state = gMenuState;
    progress = state->pathProgress;
    if (0.0f == state->pathProgress && 0.0f == state->pathSpeed) {
        page = state->displayedStop;
        D_800900F8 = page;
    } else {
        page = state->displayedStop;
        if (page == 0 && state->pathSpeed < 0.0f) {
            D_800900F8 = 4;
        }
    }
    if (page == 0) {
        y = 0x6A;
        alpha = 0xFF - (s32)(progress * 300.0f);
    } else {
        alpha = fadeAlpha;
        if (D_800900F8 == 4) {
            sc = 68.0f;
        } else {
            sc = 76.0f;
        }
        y = 0x6A - (s32)(progress * sc);
    }
    if (state->prevPage != 3) {
        if (alpha > 0 && *(image = &D_800900D8[D_800900F8]) != NULL) {
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
            diff = 2;
            if (D_800900F8 == 4) {
                scale = 0.8000000119f;
                v = 2;
            } else {
                scale = 1.0f;
                v = 1;
                if (D_800900F8 != 0) {
                    diff = 3;
                }
            }
            gHudSprite.texId = *image;
            gfx = hud_drawSprite(gfx, &gHudSprite, 0x94, y, diff, v, scale, 0);
            state = gMenuState;
            progress = state->pathProgress;
        }
        alpha = 0xFF - (s32)(progress * 300.0f);
        progress = 2.0f;
        if (alpha > 0) {
            gDPPipeSync(gfx++);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
            gHudSprite.texId = &D_88C;
            gfx = hud_drawSprite(gfx, &gHudSprite, 0x94, 0x1E, 3, 1, 1.0f, 0);
            state = gMenuState;
        }
    }
    switch (state->prevPage) {
    case 0:
        gHudSlotAlphas[0] = 0;
        gHudSlotAlphas[1] = 0xFF;
        gHudSlotAlphas[2] = 0xFF;
        gHudSlotAlphas[3] = 0xFF;
        if (state->pathProgress >= 0.5f && state->displayedStop == 4) {
            listAlpha = (gMenuBrightness * gMenuPageFade) >> 8;
            if (listAlpha >= 0xFE) {
                listAlpha = 0xFF;
            }
            angle = 0.6283185482f;
            angle += 0.6283185482f + 0.6283185482f * D_8008FDE8;
            prev = 0x7E - (s32)(cosf(angle) * 52.0f);
            j = gMenuState->cursor - 4;
            if (j < 0) {
                j += 5;
            }
            for (i = 1; i != 8; i++) {
                angle -= 0.6283185482f;
                cur = 0x7E - (s32)(cosf(angle) * 52.0f);
                if (prev < cur) {
                    diff = cur - prev;
                    gDPPipeSync(gfx++);
                    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, listAlpha);
                    mid = (diff >> 1) + prev;
                    sc = diff / 32.0f;
                    gHudSprite.texId = D_800900FC[j];
                    gfx = hud_drawSprite(gfx, &gHudSprite, 0x53, mid, 2, 1, sc, -1);
                    if (D_8008FDE8 == 0.0f && i == 5 && gPauseCursor == 0) {
                        v = (gTotalTicks * 2) & 0x7F;
                        if (v >= 0x40) {
                            v = 0x7F - v;
                        }
                        v = (s32)((u32)listAlpha * (v + v + v + 0x3F)) >> 8;
                        if (v >= 0x100) {
                            v = 0xFF;
                        }
                        gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, v);
                        gHudSprite.texId = D_800900FC[j] + 2;
                        gfx = hud_drawSprite(gfx, &gHudSprite, 0x53, mid, 2, 1, sc, -1);
                    }
                }
                j++;
                if (j >= 5) {
                    j -= 5;
                }
                prev = cur;
            }
        }
        break;
    case 4: {
        s32 x;
        s32 dir;
        s32 count;
        s32 pulse;
        s32 slot;
        s32 valueIndex;
        s32 values[3];
        Actor **slotObj;

        sc = (gTotalTicks & 0x3F) * 0.0981746912f;
        sc = sinf(sc + 0.7853975296f);
        pulse = (gMenuPageFade * (s32)((sc + 1.0f) * 127.0f)) >> 8;
        if (gMenuState->displayedStop == 7) {
            x = 0x70;
            slotCount = 2;
        } else {
            x = 0x28;
            slotCount = 4;
        }
        count = save_addPartnerPlayers(values);
        if (count != 0) {
            valueIndex = 0;
        }
        for (slot = 0; slot < slotCount; slot++) {
            mid = -1;
            dir = 1;
            for (v = 0; v < gRosterSize; v++) {
                if (slot == gRoster[v]) {
                    mid = v;
                    v = gRosterSize;
                    dir = -1;
                }
            }
            if (slot != 0 && gMenuCharacters[0] != 0) {
                slotObj = &gMenuCharacters[slot];
                if (gHudSlotAlphas[slot] != 0xFF) {
                    if (*slotObj == 0 && mid != -1) {
                        save_spawnMenuCharacter(slot, gPlayerCharacters[mid]);
                    }
                } else if (*slotObj != 0) {
                    actor_remove(*slotObj, 1);
                    *slotObj = 0;
                }
            }
            alpha = gHudSlotAlphas[slot];
            alpha += dir * (gFrameTicks << 3);
            if (alpha >= 0x100) {
                alpha = 0xFF;
            } else if (alpha < 0) {
                alpha = 0;
            }
            gHudSlotAlphas[slot] = alpha;
            gDPPipeSync(gfx++);
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, gMenuPageFade);
            gHudSprite.texId = &D_89C + 2;
            y = 0x8C - (s32)(alpha * 0.1428571492f);
            gfx = hud_drawSprite(gfx, &gHudSprite, x, y, 1, 1, 1.0f, 0);
            if (alpha > 0) {
                gDPPipeSync(gfx++);
                gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, pulse);
                gfx = hud_drawSprite(gfx, &gHudSprite, x, y, 1, 1, 1.0f, 1);
            }
            if (alpha > 0) {
                gDPPipeSync(gfx++);
                alpha = (gMenuPageFade * alpha) >> 8;
                if (alpha >= 0xFE) {
                    alpha = 0xFF;
                }
                gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
                gHudSprite.texId = &D_87B;
                gfx = hud_drawSprite(gfx, &gHudSprite, x, 0x82, 1, 1, 1.0f, 0);
            }
            text_setColor(0xFF, 0xFF, 0xFF, gMenuPageFade);
            text_printAt(x, 0x41, 0x80, "#", slot + 1);
            if (count != 0 && mid >= 0 && gRoster[mid] >= 0) {
                count--;
                text_printAt(x, 0xA8, 0, "#", 0x38);
                text_printAt(x, 0xA2, 0x81, "%d", values[valueIndex] + 1);
                valueIndex++;
            }
            x += 0x48;
        }
    } // matching: the break must follow the block, not end it
        break;
    case 3:
        gfx = hud_drawResultsTable(gfx, 0x6A, 0xFF, 0);
        break;
    }
    return gfx;
}


/* Draws a fading-in image with a tinted overlay (strength gMenuPulse). */
Gfx *func_151ED09C(Gfx *gfx) {
    s32 alpha;

    gSPDisplayList(gfx++, gHudSpriteDl);
    alpha = gFrontEndTimer << 2;
    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, alpha);
    gfx = hud_drawSprite(gfx, &D_8009181C, 0x92, 0x6C, 8, 3, 1.0f, 0);
    gDPPipeSync(gfx++);
    gDPSetCombine(gfx++, 0xFFD3FF, 0xFFA6FF7F);
    gDPSetEnvColor(gfx++, 0xFF, 0x80, 0x20, (gMenuPulse * (alpha + 1)) >> 8);
    gfx = hud_drawSprite(gfx, &D_80091828, 0x92, 0x6C, 8, 3, 1.0f, 0);
    return sprite_endDraw(gfx);
}

/* Darkens the whole screen with a black rectangle of alpha gFrontEndFade. */
Gfx *hud_darkenScreen(Gfx *gfx) {
    if (gFrontEndFade == 0) {
        return gfx;
    }
    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++, 0x082C3F, 0x504340);
    gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFEFB7D);
    gDPSetEnvColor(gfx++, 0, 0, 0, gFrontEndFade);
    return view_fillRect(gfx, 0, 0, gScreenWidth, gScreenHeight);
}



/* Sets up the load and render tiles for a sprite; stores its LoadBlock texel count in *blockSizeOut. */
Gfx *hud_setupSpriteTiles(Gfx *gfx, HudSprite *sprite, s32 *blockSizeOut) {
    s32 masks;
    s32 maskt;
    s32 i;
    s32 fmt;
    s32 blockSize;

    blockSize = gImSizIncr[sprite->siz] + sprite->width * sprite->height;
    blockSize >>= gImSizShift[sprite->siz];
    *blockSizeOut = blockSize;
    masks = 1;
    for (i = 2; i < sprite->width; i <<= 1) {
        masks++;
    }
    maskt = 1;
    for (i = 2; i < sprite->height; i <<= 1) {
        maskt++;
    }
    fmt = sprite->fmt;
    gDPPipeSync(gfx++);
    gDPSetTile(gfx++, fmt, gImSizLoadBlock[sprite->siz], 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTile(gfx++, fmt, sprite->siz, (D_8009DEB8[sprite->siz] * sprite->width + 7) >> 3, 0, 0, 0, G_TX_CLAMP, maskt, 0, G_TX_CLAMP, masks, 0);
    gDPSetTileSize(gfx++, 0, 0, 0, (sprite->width - 1) << 2, (sprite->height - 1) << 2);
    return gfx;
}

/*
 * Draws a sprite as a columns x rows grid of tiles centred on (x, y), streaming its texture in 4KB
 * loads. A negative frame keeps the horizontal scale at 1.
 */
Gfx *hud_drawSprite(Gfx *gfx, HudSprite *sprite, s32 x, s32 y, s32 columns, s32 rows, f32 scale, s32 frame) {
    s32 column;
    s32 row;
    s32 width;
    s32 height;
    s32 top;
    s32 blockSize;
    s32 texId;
    s32 dsdx;
    s32 dtdy;
    s32 image;
    s32 midTexture;
    s32 remaining;
    f32 scaleX;

    midTexture = 0;
    gfx = hud_setupSpriteTiles(gfx, sprite, &blockSize);
    if (frame < 0) {
        scaleX = 1.0f;
        frame = 0;
    } else {
        scaleX = scale;
    }
    texId = (s32)sprite->texId + frame;
    width = (u32)sprite->width * scaleX * gHudScaleX;
    height = (u32)sprite->height * scale * gHudScaleY;
    dtdy = 1024.0f / (scale * gHudScaleY);
    dsdx = 1024.0f / (scaleX * gHudScaleX);
    x *= gHudScaleX;
    y *= gHudScaleY;
    x -= (width * columns) >> 1;
    top = y - ((height * rows) >> 1);
    for (column = 0; column < columns; column++) {
        y = top;
        for (row = 0; row < rows; row++) {
            if (!midTexture) {
                image = texture_getAddress(texId, &remaining, 3, 0);
            }
            if (image != TEXTURE_NOT_LOADED) {
                gDPPipeSync(gfx++);
                gDPSetTextureImage(gfx++, sprite->fmt, gImSizLoadBlock[sprite->siz], 1, image);
                gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, blockSize - 1, 0);
                gSPScisTextureRectangle(gfx++, x << 2, y << 2, (x + width) << 2, (y + height) << 2, 0, 0, 0, dsdx, dtdy);
            }
            y += height;
            if (remaining > 0x1000) {
                midTexture = 1;
                if (image != 0) {
                    image += (blockSize << sprite->siz) >> 1;
                }
                remaining -= (blockSize << sprite->siz) >> 1;
            } else {
                midTexture = 0;
                texId++;
            }
        }
        x += width;
    }
    gDPPipeSync(gfx++);
    return gfx;
}


/*
 * Loads a HUD model and makes patched copies of its display lists (SetOtherMode rewritten,
 * SetCombine turned into a no-op). Returns NULL on failure.
 */
HudModel *hud_loadModel(s32 loadArg0, s32 loadArg1, s32 animArg0, f32 animArg1) {
    HudModel *model;
    f32 (*mtx1)[4];
    f32 (*mtx2)[4];
    Gfx *dst;
    Gfx *src;
    s32 cmd;
    s32 size;
    s32 i;

    model = allocate_memory(sizeof(HudModel), 1, 0, 1);
    if (model == NULL) {
        return NULL;
    }
    model->blinkTimer = 1;
    if (anim_createInstance(loadArg0, loadArg1, &model->srcDisplayLists, &model->displayListCount, &model->segment1Base, &model->unk20, &model->anim) != 0) {
        heap_free(model);
        return NULL;
    }

    mtx1 = model->matrices[0];
    guMtxIdentF(mtx1);
    mtx2 = model->matrices[1];
    guMtxIdentF(mtx2);
    model->anim->matrices[0] = mtx1;
    model->anim->matrices[1] = mtx2;
    anim_play(model->anim, 1, animArg0, animArg1, 0.0f, 0);

    for (i = 0; i < model->displayListCount; i++) {
        src = model->srcDisplayLists[i];
        size = 0;
        do {
            cmd = (src->words.w0 >> 24) & 0xFF;
            src++;
            size += 8;
        } while (cmd != 0xDF);

        dst = allocate_memory(size, 1, 1, 1);
        model->displayLists[i] = dst;
        if (dst == NULL) {
            for (size = 0; size < i; size++) {
                heap_free(model->displayLists[size]);
            }
            heap_free(model);
            return NULL;
        }
    }

    for (i = 0; i < model->displayListCount; i++) {
        dst = model->displayLists[i];
        src = model->srcDisplayLists[i];
        do {
            dst->words.w0 = src->words.w0;
            dst->words.w1 = src->words.w1;
            src++;
            cmd = (dst->words.w0 >> 24) & 0xFF;
            if (cmd == 0xEF) {
                dst->words.w0 &= 0xFFFEFFFF;
                dst->words.w1 = 0x5041C8;
            }
            if (cmd == 0xFC) {
                dst->words.w0 = 0;
            }
            dst++;
        } while (cmd != 0xDF);
    }

    return model;
}


/* Frees a HUD model and its display list copies. */
void hud_freeModel(HudModel *model) {
    s32 i;

    if (model != NULL) {
        anim_freeInstance(model->anim);
        heap_setTag(model, 4);
        for (i = 0; i < model->displayListCount; i++) {
            heap_setTag(model->displayLists[i], 4);
        }
    }
}

Gfx gHudModelTexOnDl[] = {
    gsDPPipeSync(),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx gHudModelTexOffDl[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};


/* Draws a HUD model at (x, y) with the given scale; the D_80090058 model also blinks. */
Gfx *hud_drawModel(Gfx *gfx, HudModel *model, f32 x, f32 y, f32 scale) {
    s32 tex;
    Gfx *start;
    s32 image;
    s32 i;

    start = gfx;
    if (gHudScaleX != 1.0f) {
        x *= gHudScaleX;
        y *= gHudScaleY;
        scale *= gHudScaleX;
    }

    guMtxIdentF(model->matrices[gDrawBufferIndex]);
    model->matrices[gDrawBufferIndex][3][0] = x;
    model->matrices[gDrawBufferIndex][3][1] = y;
    model->matrices[gDrawBufferIndex][3][2] = -100.0f;
    model->matrices[gDrawBufferIndex][0][0] = scale;
    model->matrices[gDrawBufferIndex][1][1] = scale;
    model->matrices[gDrawBufferIndex][2][2] = scale;
    anim_updateInstance(model->anim);

    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &gHudOrthoMtx, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx++, gHudModelTexOnDl);
    gDPSetCombine(gfx++, 0x12FE25, 0xFFFFFBFD);
    gSPLoadGeometryMode(gfx++, 0);
    gSPSetGeometryMode(gfx++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gDPSetPrimColor(gfx++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, gSinglePlayerHudAlpha);
    gDPSetFogColor(gfx++, 0, 0, 0, 0xFF);
    gSPSegment(gfx++, 3, model->anim->partMtx[gDrawBufferIndex]);
    gSPSegment(gfx++, 1, model->segment1Base);

    if (model == D_80090058) {
        model->blinkTimer--;
        if (model->blinkTimer == 0) {
            model->blinkTimer = (math_random() & 0x7F) + 0xF;
        }
        if (model->blinkTimer < 7) {
            tex = (s32)&D_507;
        } else {
            tex = (s32)&D_508;
        }
        image = texture_getAddress(tex, 0, 3, 0);
        if (image == TEXTURE_NOT_LOADED) {
            return start;
        }
        gSPSegment(gfx++, 6, image);
        gSPSegment(gfx++, 7, image);
    }

    for (i = 0; i < model->displayListCount; i++) {
        gSPDisplayList(gfx++, model->displayLists[i]);
    }
    gSPDisplayList(gfx++, gHudModelTexOffDl);
    return gfx;
}

Struct151EDF4C *gCodeEntryKeys = NULL;
HudSprite gHudImagePanelSprite = { NULL, 0x100, 64, 22, G_IM_FMT_RGBA, G_IM_SIZ_16b };

/*
 * Loads the 30 display lists 0x1C5..0x1E2 and patches them: vertex addresses relocated with the
 * first two vertices' colours set to opaque black, SetCombine and SetOtherMode rewritten.
 */
void hud_initCodeEntry(void) {
    s32 i;
    s32 j;
    s32 pad1; // matching: stack slots
    s32 pad2;
    s32 *data;
    Gfx *dl;
    s32 cmd;
    Vtx *vtx;

    gCodeEntryKeys = NULL;
    gCodeEntryText = allocate_memory(0x1A8, 1, 0, 0);
    if (gCodeEntryText != NULL) {
        gCodeEntryKeys = (Struct151EDF4C *)(gCodeEntryText + 0x40);
        for (i = 0x1C5, j = 0; i < 0x1E3; i++, j++) {
            data = rom_loadPathTable(0, 1, 0, 2, 9, i);
            gCodeEntryKeys[j].data = data;
            gCodeEntryKeys[j].dl = (Gfx *)*data;
            texture_resolveDl(gCodeEntryKeys[j].dl, 0, 1, 0x3E, &gCodeEntryKeys[j].usedTextures);
            dl = gCodeEntryKeys[j].dl;
            do {
                cmd = (dl->words.w0 >> 24) & 0xFF;
                if (cmd == 1) {
                    dl->words.w1 += (s32)data;
                    vtx = (Vtx *)(((volatile Gfx *)dl)->words.w1 | 0x80000000); // matching: volatile forces the reload of w1 after the store
                    vtx->v.cn[0] = 0;
                    vtx->v.cn[1] = 0;
                    vtx->v.cn[2] = 0;
                    vtx->v.cn[3] = 0xFF;
                    vtx++;
                    vtx->v.cn[0] = 0;
                    vtx->v.cn[1] = 0;
                    vtx->v.cn[2] = 0;
                    vtx->v.cn[3] = 0xFF;
                }
                if (cmd == 0xFC) {
                    dl->words.w0 = 0xFC127E05;
                    dl->words.w1 = 0xFFFFF3F8;
                }
                if (cmd == 0xEF) {
                    dl->words.w0 |= 0x100000;
                    dl->words.w1 = 0x0F0A4000;
                }
                dl++;
            } while (cmd != 0xDF);
        }
        gCodeEntryColumn = 0;
        gCodeEntryRow = 0;
        gCodeEntryKeyPress = 0;
        gCodeEntryLength = 0;
        gCodeEntryText[0] = 0;
        gCodeEntryText[0x20] = 0;
        gCodeEntryRepeatCount = 0;
    }
}

/*
 * Text entry on a 3D letter grid (cursor gCodeEntryRow row, gCodeEntryColumn column, text in gCodeEntryText):
 * moves the cursor, types letters, and on enter matches the text against the code strings
 * 0x90..0xA2 (sets a D_800E9D00 flag) and the word strings 0xA3..0xC7, then plays a response.
 */
Gfx *hud_updateCodeEntry(Gfx *gfx) {
    s32 sfx;
    s32 cursorMoved;
    s32 rowLength;
    s32 i;
    s32 selectedKey;
    Actor *obj;
    Mtx *mtx;
    s32 j;
    u32 matchMask;
    u16 perspNorm;
    s32 temp;
    s32 cheatType;
    u8 typed;
    s32 width;
    s32 height;
    s32 unused; /* third output of text_measureString */
    u8 *str;
    u8 *word;

    sfx = 0;
    if (gCodeEntryKeys == NULL) {
        return gfx;
    }
    cursorMoved = 0;
    if (gMenuState->input & 1) {
        cursorMoved = 1;
        gCodeEntryRow--;
    }
    if (gMenuState->input & 2) {
        gCodeEntryRow++;
        if (gCodeEntryRow == 4 && gCodeEntryColumn >= 5) {
            gCodeEntryColumn = 4;
        }
        cursorMoved = 1;
    }
    if (gCodeEntryRow < 0) {
        gCodeEntryRow = 5;
    }
    if (gCodeEntryRow >= 6) {
        gCodeEntryRow = 0;
    }
    if (gMenuState->input & 8) {
        gCodeEntryColumn++;
        cursorMoved = 1;
    }
    if (gMenuState->input & 4) {
        gCodeEntryColumn--;
        cursorMoved = 1;
    }
    if (gCodeEntryRow == 4) {
        rowLength = 5;
    } else if (gCodeEntryRow == 5) {
        rowLength = 1;
    } else {
        rowLength = 6;
    }
    if (gCodeEntryColumn < 0) {
        gCodeEntryColumn = rowLength - 1;
    }
    if (gCodeEntryColumn >= rowLength) {
        gCodeEntryColumn = 0;
    }
    if (cursorMoved) {
        sound_play(0x62D, 0x4650, 0x40, 0, 0);
    }
    selectedKey = gCodeEntryRow * 6 + gCodeEntryColumn;
    if (gCodeEntryRow == 5) {
        selectedKey--;
    }
    mtx = allocate_memory(0x80, 4, 2, 0);
    if (gPads[0].button & A_BUTTON) {
        gCodeEntryKeyPress += 8;
    } else {
        gCodeEntryKeyPress -= 8;
    }
    if (gCodeEntryKeyPress < 0) {
        gCodeEntryKeyPress = 0;
    }
    if (gCodeEntryKeyPress >= 13) {
        gCodeEntryKeyPress = 12;
    }
    if (gMenuState->input & 0x10) {
        i = selectedKey + 'A';

        sound_play(0x500, 0x7D00, 0x40, 0, 0);
        if (gCodeEntryRow == 5) {
            i = ' ';
        } else if (gCodeEntryRow == 4) {
            if (gCodeEntryColumn == 0) {
                if (gCodeEntryLength > 0) {
                    gCodeEntryLength--;
                }
                i = 0;
                gCodeEntryText[gCodeEntryLength] = 0;
            } else if (gCodeEntryColumn == 1) {
                i = '.';
            } else if (gCodeEntryColumn == 4) {
                matchMask = 0xFFC3FFFE;
                for (i = 0x90; i < 0xA3; i++) {
                    str = gTextStrings[i];
                    temp = i - 0x90;
                    if (temp < gCodeEntryLength) {
                        typed = gCodeEntryText[temp];
                    } else {
                        typed = ' ';
                    }
                    for (j = 0; j < 32; j++) {
                        if (typed != str[j]) {
                            matchMask &= ~(1 << j);
                        }
                    }
                }
                j = -1;
                for (i = 0; i < 33; i++) {
                    if (matchMask & 1) {
                        j = i;
                    }
                    matchMask >>= 1;
                }
                if (j >= 0) {
                    temp = 1 << j;
                    D_800E9D00 |= temp;
                    if (temp == 0x400) {
                        D_800E9D00 &= ~0x800;
                    } else if (temp == 0x800) {
                        D_800E9D00 &= ~0x400;
                    }
                    cheatType = 0x10;
                    sfx = (math_random() & 1) + 0xC3;
                lbl1:; // matching: as lbl2 below
                } else {
                    cheatType = 0x11;
                    for (i = 0xA3; i < 0xC8 && sfx == 0; i++) {
                        str = gCodeEntryText;
                        while (str != NULL && (word = gTextStrings[i]) != NULL) {
                            j = 0;
                            while (word[j] == *str && *str != 0 && word[j] != 0) {
                                str++;
                                j++;
                            }
                            if (word[j] == 0 && (*str == ' ' || *str == 0)) {
                                str = NULL;
                                sfx = (math_random() & 1) + 0xC1;
                            } else if (*str == ' ') {
                                str++;
                            } else {
                                while (*str && *str != ' ') {
                                    str++;
                                }
                                if (*str == 0) {
                                    str = NULL;
                                }
                            }
                        }
                    }
                    if (sfx == 0) {
                        sfx = (math_random() & 1) + 0xC5;
                        i = 0;
                        while (gCodeEntryText[i + 0x20] && gCodeEntryText[i] && gCodeEntryText[i + 0x20] == gCodeEntryText[i]) {
                            i++;
                        }
                        if (gCodeEntryText[i + 0x20] == 0 && gCodeEntryText[i] == 0) {
                            gCodeEntryRepeatCount++;
                            if (gCodeEntryRepeatCount == 1) {
                                sfx = 0xC7;
                            }
                        } else {
                            gCodeEntryRepeatCount = 0;
                        }
                    } else {
                        gCodeEntryRepeatCount = 0;
                    lbl2:; // matching: an empty label here ends a basic block for IDO's scheduler
                    }
                }
                for (i = 0; gCodeEntryText[i] != 0; i++) {
                    gCodeEntryText[i + 0x20] = gCodeEntryText[i];
                }
                gCodeEntryText[i + 0x20] = 0;
                i = 0;
                if (sfx != 0) {
                    mp3_startTrack(sfx, 0x7D00, 0x40);
                }
                obj = collision_findObjectById(9);
                if (obj != NULL) {
                    obj->intelwalk = cheatType;
                    gCurrentActor = obj;
                    dc_glbl_CharNum = gCurrentActor - movement_obj;
                    obj->scriptCmd = script_getScript(0, obj->intelwalk);
                    obj->scriptTimer = 0;
                }
                gCodeEntryLength = 0;
                gCodeEntryText[gCodeEntryLength] = 0;
            } else {
                i -= 2;
            }
        }
        if (i > 0) {
            gCodeEntryText[gCodeEntryLength] = i;
            if (gCodeEntryLength < 19) {
                gCodeEntryLength++;
            }
            gCodeEntryText[gCodeEntryLength] = 0;
        }
    }
    math_mtxRotTrans(mtx, -30.0f, -10.0f, 0.0f, -115.0f, 10.0f, -530.0f);
    math_mtxTranslate(mtx + 1, 0.0f, 0.0f, -(f32)gCodeEntryKeyPress);
    guPerspective(&gHudOrthoMtx, &perspNorm, 50.0f, 60.6f, 53.0f, 20000.0f, 1.0f);
    gSPPerspNormalize(gfx++, perspNorm);
    gSPMatrix(gfx++, &gHudOrthoMtx, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPLoadGeometryMode(gfx++, 0);
    gSPSetGeometryMode(gfx++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gSPMatrix(gfx++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    for (i = 0; i < 30; i++) {
        gDPPipeSync(gfx++);
        if (i == selectedKey) {
            gDPSetEnvColor(gfx++, 0xFF, 0x00, 0xFF, gMenuPageFade);
            gSPMatrix(gfx++, mtx + 1, G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
        } else {
            gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, gMenuPageFade);
        }
        gSPDisplayList(gfx++, gCodeEntryKeys[i].dl);
        if (i == selectedKey) {
            gSPMatrix(gfx++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        }
    }
    text_setColor(0xFF, 0xB1, 0x1E, gMenuPageFade);
    text_measureString(gCodeEntryText, &width, &height, &unused);
    i = 0x4B - (width >> 1);
    if (i < 10) {
        i = 10;
    }
    text_printAt(i, 0xB4, 0x80, (char *)gCodeEntryText);
    if ((gTotalTicks & 0x1F) >= 10) {
        width = i + width + 2;
        text_setColor(0xFF, 0xFF, 0xFF, 0xFF);
        text_printAt(width, 0xB4, 0x80, "-");
    }
    return gfx;
}

/* Draws a framed panel showing image `index` of table 0x1D (the texture id is cached in gHudImagePanelSprite). */
Gfx *hud_drawImagePanel(Gfx *gfx, s32 index) {
    s32 image;

    gDPPipeSync(gfx++);
    gDPSetEnvColor(gfx++, 0xFF, 0xFF, 0xFF, gMenuBrightness);
    image = texture_getAddress((s32)&D_7FB, 0, 3, 0);
    if (image != TEXTURE_NOT_LOADED) {
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, image);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_MIRROR, 5, 0, G_TX_CLAMP, 6, 0);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x5FF, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 12, 0, G_TX_RENDERTILE, 0, G_TX_MIRROR, 5, 0, G_TX_CLAMP, 6, 0);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0xBC, 0x7C);
        gDPSetOtherMode(gfx++, 0x002C3F, 0x504244);
        gfx = hud_texRectScaled(gfx, 0x190, 0x100, 0x250, 0x200, 0, 0, 0, 0x400, 0x400);
        gfx = hud_texRectScaled(gfx, 0x250, 0x100, 0x310, 0x200, 0, 0x5E0, 0, -0x400, 0x400);
    }

    gDPPipeSync(gfx++);
    image = texture_getAddress((s32)&D_887, 0, 3, 0);
    if (image != TEXTURE_NOT_LOADED) {
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, image);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPLoadSync(gfx++);
        gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x3FF, 0);
        gDPPipeSync(gfx++);
        gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, 0, G_TX_CLAMP, 5, 0);
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 0x7C, 0x7C);
        gfx = hud_texRectScaled(gfx, 0x250, 0xBC, 0x290, 0xFC, 0, 0, 0, 0x400, 0x400);
        gfx = hud_texRectScaled(gfx, 0x180, 0xBC, 0x1D0, 0xFC, 0, 0x200, 0x200, 0x400, 0x400);
    }

    if (gHudImagePanelImage != index + 1) {
        u16 *table;
        u32 size;

        table = NULL;
        size = rom_loadPathTo(&table, 1, 0x1D);
        if ((u32)index < size >> 1) {
            gHudImagePanelSprite.texId = (void *)table[index];
        } else {
            gHudImagePanelSprite.texId = NULL;
        }
        heap_free(table);
        gHudImagePanelImage = index + 1;
    }

    if (gHudImagePanelSprite.texId != NULL) {
        gDPPipeSync(gfx++);
        gfx = hud_drawSprite(gfx, &gHudImagePanelSprite, 0x94, 0x60, 1, 2, 1.0f, 0);
    }
    return gfx;
}

/* Clears the code flags set by hud_updateCodeEntry. */
void hud_clearCodeFlags(void) {
    D_800E9D00 = 0;
}

/* 0x40 of inter-TU padding at 0x151EF000..0x151EF040.  Not code: it is the
   tail of this TU's conker.us.yaml range (0x215960..0x21C4F0) and must be
   stated explicitly, or everything downstream slides. */
GLOBAL_ASM(
glabel pad_151EF000
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
)

