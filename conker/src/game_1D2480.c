#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Spawns a tint effect with extraSize bytes after it; returns NULL if the allocation fails. */
struct Obj151A4FD0 *func_151A4FD0(s32 type, s32 timer, s32 red, s32 green, s32 blue, s32 viewport, u8 unk19,
                                  s32 extraSize) {
    struct Obj151A4FD0 *effect = func_15167A68(0x5A, 0, extraSize + sizeof(struct Obj151A4FD0), 0, 0xFF, 1);

    if (effect == NULL) {
        return NULL;
    }
    effect->type = type;
    effect->timer = timer;
    effect->red = red;
    effect->green = green;
    effect->blue = blue;
    effect->unk19 = unk19;
    effect->rising = FALSE;
    effect->viewport = viewport;
    return effect;
}

/* Per-frame update: counts the timer down (freeing the effect when it runs out) and pulses red between 0x90 and 0xFF. */
void func_151A5070(struct Obj151A4FD0 *effect) {
    s32 timer;
    s32 step;
    s32 red;

    timer = effect->timer;
    if (timer != 0) {
        timer -= D_800BE9E4;
        if (timer <= 0) {
            func_1516972C(&effect->header);
            return;
        }
        effect->timer = timer;
    }

    if (effect->unk19 == 0) {
        func_10011FA0((s32 *)2);
    }

    red = effect->red;
    step = D_800BE9E4 * 2;
    if (!effect->rising) {
        red -= step;
        if (red < 0x90) {
            red = 0x90;
            effect->rising = TRUE;
        }
    } else {
        red += step;
        if (red >= 0x100) {
            red = 0xFF;
            effect->rising = FALSE;
        }
    }
    effect->red = red;
}

extern void (*D_8008F900[])(void *, struct Obj151A4FD0 *, s16);

/* Calls the effect's type handler from D_8008F900. */
void func_151A5130(void *arg0, struct Obj151A4FD0 *effect, s16 arg2) {
    D_8008F900[effect->type](arg0, effect, arg2);
}

/*
 * Redraws the effect's viewport from the current framebuffer, tinted by the effect's colour.
 * The copy goes through TMEM in strips: 8 rows of up to 256 columns, then 54 rows of any columns past 256.
 */
Gfx *func_151A5170(Gfx *gfx, struct Obj151A4FD0 *effect, s32 arg2) {
    s32 left;
    s32 pass;
    s32 y;
    s32 columns;
    s32 rows;
    s32 viewWidth;
    s32 viewHeight;
    u8 state;

    gDPPipeSync(gfx++);
    gDPSetCombineMode(gfx++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(gfx++, 1, 0, effect->red, effect->green, effect->blue, 255);
    func_1501A490(gfx, arg2, 0, 0, 0, 3);
    state = 0;
    gfx = func_15142FBC(gfx, 0xC00, 0x0F0A4004, &state);

    viewWidth = D_800BE628[effect->viewport].width;
    viewHeight = D_800BE628[effect->viewport].height;

    for (pass = 0; pass < 2; pass++) {
        if (pass == 0) {
            left = D_800BE628[effect->viewport].left;
            rows = 8;
            columns = (viewWidth > 256) ? 256 : viewWidth;
        } else {
            if (viewWidth <= 256) {
                break;
            }
            left = D_800BE628[effect->viewport].left + 256.0f;
            columns = viewWidth - 256;
            rows = 54;
        }

        y = D_800BE628[effect->viewport].top;

        while (y < D_800BE628[effect->viewport].top + viewHeight) {
            if (D_800BE628[effect->viewport].top + viewHeight < y + rows) {
                rows = D_800BE628[effect->viewport].top + viewHeight - y;
            }

            gDPPipeSync(gfx++);
            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_800BE620, D_8002AAE8[D_800BE9C0]);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, ((columns * G_IM_SIZ_16b_LINE_BYTES) + 7) >> 3, 0,
                       G_TX_LOADTILE, 0, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPLoadTile(gfx++, G_TX_LOADTILE, left << G_TEXTURE_IMAGE_FRAC, y << G_TEXTURE_IMAGE_FRAC,
                        (left + columns - 1) << G_TEXTURE_IMAGE_FRAC, (y + rows - 1) << G_TEXTURE_IMAGE_FRAC);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, ((columns * G_IM_SIZ_16b_LINE_BYTES) + 7) >> 3, 0,
                       G_TX_RENDERTILE, 0, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, columns << G_TEXTURE_IMAGE_FRAC, rows << G_TEXTURE_IMAGE_FRAC);
            gSPTextureRectangle(gfx++, left << G_TEXTURE_IMAGE_FRAC, y << G_TEXTURE_IMAGE_FRAC,
                                (left + columns) << G_TEXTURE_IMAGE_FRAC, (y + rows) << G_TEXTURE_IMAGE_FRAC,
                                G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

            y += rows;
        }
    }

    gDPPipeSync(gfx++);
    return func_15142FBC(gfx, 0x2C00, 0x0F0A4004, &state);
}

extern void (*D_8008F904[])(struct Obj151A4FD0 *, void *, u8);

/* Calls the effect's optional D_8008F904 handler, if it has one. */
void func_151A55D4(struct Obj151A4FD0 *effect, void *arg1, u8 arg2) {
    void (*handler)(struct Obj151A4FD0 *, void *, u8) = D_8008F904[effect->unk19];

    if (handler != NULL) {
        handler(effect, arg1, arg2);
    }
}


extern IdList1 D_800A8D70;

/* Forwards a message to func_15169260 with a local copy of the D_800A8D70 descriptor. */
void func_151A561C(s32 arg0, u8 arg1) {
    IdList1 descriptor;

    descriptor = D_800A8D70;
    func_15169260(&descriptor, 1, arg0, arg1);
}
