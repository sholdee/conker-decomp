#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define FONT_CHAR_COUNT 80
#define FONT_GLYPH_WIDTH 8
#define FONT_GLYPH_HEIGHT 12
#define FONT_GLYPHS_PER_ROW 8

extern u8 D_80084930[]; // 64x128 I4 font texture
extern u8 D_800848D0[]; // font charset, FONT_CHAR_COUNT entries

/** Sets up the render state and loads the 64x128 I4 font texture into TMEM. */
Gfx *func_150412C0(Gfx *gfx) {
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPPipeSync(gfx++);
    gDPSetCombineMode(gfx++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetOtherMode(gfx++, 0x002C0F, 0x0055204C);
    gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 64, D_80084930);
    gDPLoadTextureBlock_4b(gfx++, D_80084930, G_IM_FMT_I, 64, 128, 0, G_TX_NOMIRROR | G_TX_WRAP,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    return gfx;
}

/** Draws the NUL-terminated string str at (x, y), one font glyph per character. */
Gfx *func_150413FC(Gfx *gfx, s32 x, s32 y, u8 *str) {
    s32 i;

    i = 0;
    while (str[i] != '\0') {
        gfx = func_15041508(gfx, x + i * FONT_GLYPH_WIDTH, y, func_15041480(str[i]));
        i++;
    }
    return gfx;
}

/** Returns the glyph index of ch in the font charset, or FONT_CHAR_COUNT if absent. */
s32 func_15041480(u8 ch) {
    s32 i;

    for (i = 0; i < FONT_CHAR_COUNT; i++) {
        if (ch == D_800848D0[i]) {
            return i;
        }
    }
    return i;
}

/** Draws one font glyph as a textured rectangle at (x, y). */
Gfx *func_15041508(Gfx *gfx, s32 x, s32 y, s32 glyph) {
    s32 column = glyph % FONT_GLYPHS_PER_ROW;

    gSPTextureRectangle(gfx++, x << 2, y << 2, (x + FONT_GLYPH_WIDTH) << 2, (y + FONT_GLYPH_HEIGHT) << 2,
                        G_TX_RENDERTILE, (column * FONT_GLYPH_WIDTH) << 5,
                        ((glyph - column) / FONT_GLYPHS_PER_ROW * FONT_GLYPH_HEIGHT) << 5, 1 << 10, 1 << 10);
    return gfx;
}
