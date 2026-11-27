#include <ultra64.h>
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/screen.h"
#include "conker/texture.h"

/* Spawns an overlay for the given id. */
ScreenOverlay *screen_spawnOverlay(s16 id) {
    ScreenOverlay *overlay;

    overlay = effect_newObject(0x3C, 1, sizeof(ScreenOverlay), 0, 0xFF, 1);
    overlay->alpha = 0;
    overlay->scroll = 0;
    overlay->header.state = 0;
    overlay->speed = 1;
    overlay->id = id;
    return overlay;
}

/* Makes every overlay with the given id run backwards, fading it out. */
void screen_fadeOutOverlays(s16 id) {
    ScreenOverlay *overlay;

    for (overlay = (ScreenOverlay *)gObjectLists[1][0x3C]; overlay != NULL; overlay = (ScreenOverlay *)overlay->header.next) {
        if (id == overlay->id) {
            overlay->speed = -2;
        }
    }
}

/* Per-frame update: advances the timer, derives the fade alpha and scrolls the texture. */
void screen_updateOverlay(ScreenOverlay *overlay) {
    s32 speed;

    // matching: assigning speed inside the expression schedules its lb before the timer's lh
    overlay->header.state += gFrameTicks * (speed = overlay->speed);

    if (speed > 0 && overlay->header.state >= 237) {
        overlay->alpha = (296 - overlay->header.state) << 2;
        if (overlay->alpha < 0) {
            overlay->alpha = 0;
        }
    } else {
        overlay->alpha = overlay->header.state * 2;
    }

    if (overlay->alpha >= 128) {
        overlay->alpha = 128;
    }

    overlay->scroll++;
    if (overlay->scroll >= 256) {
        overlay->scroll -= 256;
    }

    if (overlay->header.state > 300 || overlay->header.state < 0) {
        effect_destroyObject(&overlay->header);
    }
}

/*
 * Draws the overlay if it belongs to the given id: the 64x64 IA8 texture D_A48 tiled over the whole
 * 320x240 screen twice, scrolling horizontally: green at full alpha, then red at half alpha and 2x scale.
 */
Gfx *screen_drawOverlay(Gfx *gfx, ScreenOverlay *overlay, s16 id) {
    s32 texture;

    if (id != overlay->id || overlay->alpha == 0) {
        return gfx;
    }

    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPPipeSync(gfx++);
    gDPSetCombineLERP(gfx++, TEXEL0, 0, ENVIRONMENT, 0, ENVIRONMENT, 0, TEXEL0, 0,
                      TEXEL0, 0, ENVIRONMENT, 0, ENVIRONMENT, 0, TEXEL0, 0);
    gDPSetOtherMode(gfx++, 0x002C3F, G_RM_XLU_SURF | G_RM_XLU_SURF2 | G_ZS_PRIM);

    texture = texture_getAddress((s32)D_A48, NULL, 3, 0);

    gDPLoadTextureBlockS(gfx++, texture, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 64, 0, G_TX_WRAP | G_TX_NOMIRROR,
                         G_TX_WRAP | G_TX_NOMIRROR, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetEnvColor(gfx++, 0, 255, 0, overlay->alpha);
    gSPTextureRectangle(gfx++, 0, 0, 320 << 2, 240 << 2, G_TX_RENDERTILE, overlay->scroll << 3, 32 << 5, 1 << 10,
                        1 << 10);
    gDPPipeSync(gfx++);
    gDPSetEnvColor(gfx++, 255, 0, 0, overlay->alpha >> 1);
    gSPTextureRectangle(gfx++, 0, 0, 320 << 2, 240 << 2, G_TX_RENDERTILE, overlay->scroll << 3, 0, 1 << 9,
                        1 << 9);

    return gfx;
}
