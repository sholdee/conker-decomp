#include <ultra64.h>
#include "functions.h"
#include "variables.h"


Gfx *screen_drawImage(Gfx *gfx, void *img, s32 arg2, s32 arg3) {
    u32 w;
    u32 x;
    u32 h;
    u32 y;
    u32 dy;
    u32 dx;
    u32 x2;

    dy = dy;
    w = gScreenWidth;
    h = gScreenHeight;

    gDPPipeSync(gfx++);
    gDPSetCombineMode(gfx++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetOtherMode(gfx++, 0xCFF, 0xF0A4000);
    gSPLoadGeometryMode(gfx++, 0);
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    dy = 16;
    for (y = 0; y < h; y += dy) {
        x = 0;
        dx = 128;
        if (y + dy > h) {
            dy = h - y;
        }
        for (; x < w; x += dx) {
            if (x + dx > w) {
                dx = w - x;
            }
            gDPLoadTextureTile(gfx++, img, G_IM_FMT_RGBA, G_IM_SIZ_16b, w, h, x, y, x + dx - 1, y + dy - 1, 0,
                               0, 0, 0, 0, 0, 0);
            gSPTextureRectangle(gfx++, x << 2, y << 2, (x + dx) << 2, (y + dy) << 2, G_TX_RENDERTILE, x << 5, y << 5,
                                1 << 10, 1 << 10);
            gDPPipeSync(gfx++);
        }
    }
    gDPSetOtherMode(gfx++, 0x80C3F, 0xF0A4000);
    return gfx;
}

void screen_blurImage(u16 *arg0) {
    s32 y;
    s32 x;
    u16 *p;
    u16 *row;
    u16 prev;
    u16 cur;
    u16 next;
    u16 r;
    u16 g;
    u16 b;
    s32 w = gScreenWidth;
    s32 h = gScreenHeight;

    row = arg0;
    for (y = 0; y < h; y++) {
        p = row + 1;
        cur = row[1];
        next = row[2];
        for (x = 1; x < w - 1; x++) {
            prev = cur;
            cur = next;
            next = p[2];
            r = (cur >> 11) & 0x1F;
            r += (prev >> 12) & 0xF;
            r += (next >> 12) & 0xF;
            g = (cur >> 6) & 0x1F;
            g += (prev >> 7) & 0xF;
            g += (next >> 7) & 0xF;
            b = (cur >> 1) & 0x1F;
            b += (prev >> 2) & 0xF;
            b += (next >> 2) & 0xF;
            prev = ((r & 0x3E) << 10) | ((g & 0x3E) << 5) | (b & 0x3E) | 1;
            p[1] = prev;
            p++;
        }
        row += w;
    }
}

Gfx *screen_drawImageAlpha(Gfx *gfx, u8 *img, s32 arg2, s32 alpha) {
    u32 w;
    u32 y;
    u32 h;
    u8 *p;
    u32 stride;

    w = gScreenWidth;
    h = gScreenHeight;
    p = img;
    stride = w * 8;
    gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, alpha);
    gDPSetCombineMode(gfx++, G_CC_MODULATEI_PRIM, G_CC_MODULATEI_PRIM);
    gDPSetOtherMode(gfx++, 0xCFF, 0x504340);
    gSPLoadGeometryMode(gfx++, 0);
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    for (y = 0; y < h; y += 4) {
        gDPLoadTextureBlock(gfx++, OS_K0_TO_PHYSICAL(p), G_IM_FMT_RGBA, G_IM_SIZ_16b, w, 4, 0, 0, 0, 0, 0, 0, 0);
        gSPTextureRectangle(gfx++, 0, y << 2, (w - arg2) << 2, (y + 4) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10,
                            1 << 10);
        gDPPipeSync(gfx++);
        p += stride;
    }
    return gfx;
}

void screen_setFrameEcho(s32 arg0, s32 arg1) {
    if (level == 6 && D_80038080 == 0) {
        return;
    }
    if (arg1 == 0) {
        arg0 = 0;
    }
    D_800BE574 = arg0;
    if (arg0 != 0) {
        D_800BE575 = arg1;
    } else {
        D_800BE575 = 0;
    }
    if (arg0 == 0) {
        if (D_800BE570 != NULL) {
            func_100043B4((s32 *)(s32)D_800BE570, 3); // matching: the int round trip keeps the load in v0
            D_800BE570 = NULL;
        }
    }
}

Gfx *screen_drawFrameEcho(Gfx *gfx) {
    s32 alpha;
    u16 *img;

    if ((D_800BE574 == 0 && level != 0x32 && level != 0x33) || gPaused != 0) {
        return gfx;
    }
    alpha = D_800BE574;
    if (D_800BE574 != 0) {
        gDPPipeSync(gfx++);
        img = D_800BE570;
        if (img != NULL) {
            gfx = screen_drawImageAlpha(gfx, (u8 *)img, 0, alpha);
        } else {
            D_800BE570 = allocate_memory(gScreenWidth * gScreenHeight * 2, 1, 3, 1);
            if (D_800BE570 == NULL) {
                return gfx;
            }
        }
        gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gScreenWidth, D_800BE570);
        gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, gScreenWidth, gScreenHeight);
        gfx = screen_drawImage(gfx, D_8002AAE8[D_800BE9C0], 0, 4);
        gDPSetOtherMode(gfx++, 0x82C3F, 0x552230);
        gSPSetGeometryMode(gfx++, 0x220405);
        gfx = view_setColorImage(gfx);
    }
    return gfx;
}
