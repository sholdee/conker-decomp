#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c). */
// Sprite placement computed by func_15095B08 for func_15095D34.
f32 D_800D2C70; // x scale
f32 D_800D2C74; // y scale
f32 D_800D2C78; // left
f32 D_800D2C7C; // top
f32 D_800D2C80; // depth
f32 D_800D2C84; // width
f32 D_800D2C88; // height
Texture D_800D2C90; // the shared texture of func_15095060
s32 D_800D2C9C;
u8 *D_800D2CA0;
MtxF D_800D2CA8[4];
u8 D_800D2DA8;
u8 D_800D2DA9;
u8 D_800D2DAA;
u8 D_800D2DAB;

#include "functions.h"
#include "variables.h"

/* Per G_IM_SIZ (4b, 8b, 16b, 32b): as libultra's G_IM_SIZ_*_INCR, _SHIFT, line-bits factor and _LOAD_BLOCK. */
const u8 D_8009DEB0[4] = { 3, 1, 0, 0 };
const u8 D_8009DEB4[4] = { 2, 1, 0, 0 };
const u8 D_8009DEB8[4] = { 1, 1, 2, 2 };
const u8 D_8009DEBC[4] = { G_IM_SIZ_16b, G_IM_SIZ_16b, G_IM_SIZ_16b, G_IM_SIZ_32b };

/* Sprite render state (func_15094F40). */
Gfx D_800873D0[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, TEXEL0, PRIMITIVE, 0, TEXEL0, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, TEXEL0, 0),
    gsDPSetBlendColor(0, 0, 0, 0x20),
    gsSPClearGeometryMode(G_FOG | G_LOD),
    gsDPSetOtherMode(0x002C0F, 0x0055307C),
    gsSPEndDisplayList(),
};

Gfx D_80087408[] = {
    gsSPEndDisplayList(),
};

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define VIEWS ((RenderView *)gViews)


/* Combines view `view`'s current projection and view matrices into D_800D2CA8[view]. */
void func_15094EA0(s32 view) {
    MtxF projection;
    MtxF viewMtx;

    guMtxL2F(projection, &D_800DC2A0[D_800BE9C0][view]);
    guMtxL2F(viewMtx, &VIEWS[view].projectionMtx[D_800BE9C0]);
    func_150A7A48(projection, viewMtx, D_800D2CA8[view]);
}


/* Emits display list D_800873D0 and forgets the last loaded texture address. */
Gfx *func_15094F40(Gfx *gfx) {
    gSPDisplayList(gfx++, D_800873D0);
    D_800D2CA0 = NULL;
    return gfx;
}

/* Loads frame `frame >> 8` of `desc` as tile `tile` at tmem `tmem`, with the tile origin at (256, 256). */
Gfx *func_15094F70(Gfx *gfx, TextureDesc *desc, s32 frame, SpriteParams *sprite, s32 tmem, s32 tile, s32 sfrac, s32 cm, s32 assetArg) {
    func_15095060(desc, frame, sprite);
    return func_150950D4(gfx, &D_800D2C90, tmem, tile, 0, sfrac, cm, 0x100, 0x100, assetArg);
}

/* As func_15094F70, with the tile origin (s, t) given. */
Gfx *func_15094FE8(Gfx *gfx, TextureDesc *desc, s32 frame, SpriteParams *sprite, s32 tmem, s32 tile, s32 sfrac, s32 cm, s32 s, s32 t, s32 assetArg) {
    func_15095060(desc, frame, sprite);
    return func_150950D4(gfx, &D_800D2C90, tmem, tile, 0, sfrac, cm, s, t, assetArg);
}

/* Fills the shared texture D_800D2C90 from `desc`, and points `sprite` (if any) at it. */
void func_15095060(TextureDesc *desc, s32 frame, SpriteParams *sprite) {
    u32 addr;

    if (sprite != NULL) {
        sprite->tex = &D_800D2C90;
    }

    addr = desc->addr;
    if (addr < 0x10000000U) {
        D_800D2C90.addr = addr;
    } else {
        D_800D2C90.addr = ((u32 *)addr)[frame >> 8];
    }

    D_800D2C90.width = desc->width;
    D_800D2C90.height = desc->height;
    D_800D2C90.fmt = desc->fmt;
    D_800D2C90.siz = desc->siz;
    D_800D2C90.frames = desc->frameCount;
}

/* Loads frame `frame` of `tex` (and its palette) into tmem, as tile `tile` placed at (s, t). */
Gfx *func_150950D4(Gfx *gfx, Texture *tex, s32 tmem, s32 tile, s32 frame, s32 sfrac, s32 cm, s32 s, s32 t, s32 assetArg) {
    s32 width;
    s32 size;
    s32 masks;
    s32 maskt;
    s32 i;
    s32 fmt;

    if (tex->frames == 0) {
        frame = 0;
    }
    size = D_8009DEB0[tex->siz] + tex->width * tex->height;
    if (tex->fmt == 5) {
        size += (tex->width * tex->height) / 4;
    }
    size >>= D_8009DEB4[tex->siz];
    D_800D2CA0 = (u8 *)tex->addr;
    if ((u32)D_800D2CA0 < 0x10000000) {
        D_800D2CA0 = (u8 *)func_1510D0EC((s32)D_800D2CA0, 0, assetArg, 0);
    }
    if (D_800D2CA0 != (u8 *)0x80000000) { // the loader's "no asset" value
        D_800D2CA0 += (size + size) * frame;
    }
    if (tex->siz == G_IM_SIZ_4b) {
        width = tex->width >> 1;
    } else {
        width = tex->width;
    }
    masks = 1;
    for (i = 2; i < tex->width; i <<= 1) {
        masks++;
    }
    maskt = 1;
    for (i = 2; i < tex->height; i <<= 1) {
        maskt++;
    }
    if (tex->fmt != 5) {
        fmt = tex->fmt;
    } else {
        fmt = G_IM_FMT_RGBA;
    }
    gDPPipeSync(gfx++);
    gDPSetTextureImage(gfx++, fmt, D_8009DEBC[tex->siz], 1, D_800D2CA0);
    gDPSetTile(gfx++, fmt, D_8009DEBC[tex->siz], 0, tmem, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, size - 1, 0);
    gDPSetTile(gfx++, fmt, tex->siz, (D_8009DEB8[tex->siz] * width + 7) >> 3, tmem, tile, 0, cm, maskt, 0, cm, masks, 0);
    gDPSetTileSize(gfx++, tile, (s << 2) + sfrac, t << 2, ((tex->width + s - 1) << 2) + sfrac, (tex->height + t - 1) << 2);
    if (tex->fmt == 5) {
        gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_4b, (D_8009DEB8[G_IM_SIZ_4b] * (width >> 1) + 7) >> 3, tmem + (width * tex->height) / 4, tile + 1, 0, cm, maskt, 0, cm, masks, 0);
        gDPSetTileSize(gfx++, tile + 1, (s << 2) + sfrac, t, ((tex->width + s - 1) << 2) + sfrac, (tex->height + t - 1) << 2);
    }
    if (tex->fmt == G_IM_FMT_CI) {
        s32 bits;
        s32 colors;

        if (tex->siz == G_IM_SIZ_4b) {
            bits = 4;
            colors = 15;
        } else {
            bits = 8;
            colors = 255;
        }
        // the palette follows the texels
        gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, D_800D2CA0 + (tex->width * tex->height * bits) / 8);
        gDPLoadSync(gfx++);
        gDPLoadTLUTCmd(gfx++, G_TX_LOADTILE - 1, colors);
        D_800D2C9C = G_TT_RGBA16;
    } else {
        D_800D2C9C = G_TT_NONE;
    }
    return gfx;
}

/* Projects (x, y, z) through view gCurrentView. Writes screen x/y to *outX/*outY and the clip z/w;
 * returns 0 when w is outside (gDefaultNear, maxW). */
s32 func_1509563C(f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ, f32 *outW, f32 maxW) {
    f32 w;
    f32 invW;
    f32 screenX;
    f32 screenY;
    RenderView *view;

    func_150A7A00(D_800D2CA8[gCurrentView], x, y, z, outX, outY, outZ, outW);
    w = *outW;
    if ((maxW <= w) || (w <= gDefaultNear)) {
        return FALSE;
    }
    invW = 1.0f / w;
    view = &VIEWS[gCurrentView];
    screenX = *outX;
    screenY = *outY;
    screenX = (view->halfWidth + 5.0f) * screenX * invW;
    screenY = (view->halfHeight + 5.0f) * screenY * invW;
    screenX += view->centerX;
    screenY = view->centerY - screenY;
    *outX = screenX;
    *outY = screenY;
    return TRUE;
}

/* Draws `sprite` at its world position, depth-tested at its projected depth. */
Gfx *func_15095760(Gfx *gfx, SpriteParams *sprite) {
    f32 screenX;
    f32 screenY;
    f32 clipZ;
    f32 clipW;

    if (!func_1509563C(sprite->x, sprite->y, sprite->z, &screenX, &screenY, &clipZ, &clipW, 4000.0f)) {
        return gfx;
    }
    gDPPipeSync(gfx++);
    gDPSetPrimDepth(gfx++,
        (s32)(((clipZ / clipW) * VIEWS[gCurrentView].viewports[D_800BE9C0].vp.vscale[2]
            + VIEWS[gCurrentView].viewports[D_800BE9C0].vp.vtrans[2]) * 32.0f), 0);
    return func_15095A90(gfx, sprite, screenX, screenY, clipW, TRUE, 0, 0, 0);
}

/* Places `sprite` at its world position; if it is on screen, emits its depth and sets *visible. */
Gfx *func_150958B0(Gfx *gfx, SpriteParams *sprite, s32 *visible) {
    f32 screenX;
    f32 screenY;
    f32 clipZ;
    f32 clipW;

    if (!func_1509563C(sprite->x, sprite->y, sprite->z, &screenX, &screenY, &clipZ, &clipW, 4000.0f)) {
        *visible = FALSE;
        return gfx;
    }
    func_15095B08(sprite, screenX, screenY, clipW, TRUE, visible);
    if (*visible) {
        gDPPipeSync(gfx++);
        gDPSetPrimDepth(gfx++, clipZ / clipW * 16352.0f + 16352.0f, 0);
    }
    return gfx;
}

/* Draws `sprite` centred on screen position (x, y). */
Gfx *func_15095A48(Gfx *gfx, SpriteParams *sprite, f32 x, f32 y) {
    return func_15095A90(gfx, sprite, x, y, 4096.0f, FALSE, 0, 0, 0);
}

/* Places `sprite` centred on screen position (x, y) at depth z and draws it if it is on screen. */
Gfx *func_15095A90(Gfx *gfx, SpriteParams *sprite, f32 x, f32 y, f32 z, s32 projected, s32 sOffset, s32 tOffset, s32 extraWidth) {
    s32 visible;

    func_15095B08(sprite, x, y, z, projected, &visible);
    if (visible) {
        gfx = func_15095D34(gfx, sprite, sOffset, tOffset, extraWidth);
    }
    return gfx;
}

/* Computes `sprite`'s screen rectangle centred on (x, y) at depth z, and whether it is on screen. */
void func_15095B08(SpriteParams *sprite, f32 x, f32 y, f32 z, s32 projected, s32 *visible) {
    RenderView *view;
    Texture *tex;
    f32 scaleX;
    f32 scaleY;
    f32 invZ;
    f32 width;
    f32 height;

    *visible = FALSE;
    if (sprite->scaleX == 0 || sprite->scaleY == 0) {
        return;
    }
    scaleX = sprite->scaleX;
    scaleY = sprite->scaleY;
    scaleX *= D_800380A0;
    scaleY *= D_800380A4;
    if (projected) { // matching: the view address is computed in each branch
        view = &VIEWS[gCurrentView];
        scaleX *= view->unk1C;
        scaleY *= view->unk20;
    } else {
        view = &VIEWS[gCurrentView];
        scaleX *= view->scaleX;
        scaleY *= view->scaleY;
    }
    tex = sprite->tex;
    invZ = 1.0f / z;
    width = (u32)tex->width * scaleX * invZ;
    height = (u32)tex->height * scaleY * invZ;
    x -= width * 0.5f;
    y -= height * 0.5f;
    if (view->left <= x + width && x < view->right && view->top <= y + height && y < view->bottom) {
        *visible = TRUE;
    }
    D_800D2C70 = scaleX;
    D_800D2C74 = scaleY;
    D_800D2C78 = x;
    D_800D2C7C = y;
    D_800D2C80 = z;
    D_800D2C84 = width;
    D_800D2C88 = height;
}

/* Draws `sprite` at the placement from func_15095B08, without offsets. */
Gfx *func_15095D0C(Gfx *gfx, SpriteParams *sprite) {
    return func_15095D34(gfx, sprite, 0, 0, 0);
}


/* Draws `sprite` at the placement func_15095B08 computed, then advances its animation. */
Gfx *func_15095D34(Gfx *gfx, SpriteParams *sprite, s32 sOffset, s32 tOffset, s32 extraWidth) {
    f32 x;
    f32 y;
    f32 z;
    f32 width;
    f32 height;
    s32 scaleX;
    s32 scaleY;
    s32 iy;
    s32 dsdx;
    s32 dtdy;
    s32 s;
    s32 t;
    s32 tfrac;
    s32 flip;
    s32 step;
    s32 iz;
    s32 unused; // matching: see the dead division below

    scaleX = D_800D2C70;
    scaleY = D_800D2C74;
    x = D_800D2C78;
    y = D_800D2C7C;
    z = D_800D2C80;
    if (tOffset != 0) {
        D_800D2C84 -= (D_800D2C70 + D_800D2C70) / D_800D2C80;
        D_800D2C88 -= (D_800D2C74 + D_800D2C74) / D_800D2C80;
        x += D_800D2C70 / D_800D2C80;
        y += D_800D2C74 / D_800D2C80;
    }
    width = D_800D2C84;
    height = D_800D2C88;
    iy = y;
    iz = z;
    if (width < 2.0f) {
        width = 2.0f;
    }
    dsdx = (iz << 10) / scaleX;
    dtdy = (iz << 10) / scaleY;
    unused = ((s32)z << 10) / scaleX; // matching: dead early division sets uopt's spill-slot colouring
    if (D_800D2DAB == 0) {
        gDPSetPrimColor(gfx++, 1, 0, D_800D2DA8, D_800D2DA9, D_800D2DAA, sprite->alpha);
    }
    tfrac = ((32.0f - (y - (s32)y) * 32) * z) / scaleY;
    s = (s32)(((32.0f - (x - (s32)x) * 32) * z) / scaleX) + sOffset + 0x2000;
    t = tfrac + tOffset + 0x2000;
    if (sprite->flip != 0) {
        flip = sprite->flip;
        if (flip & 1) {
            s = (sprite->tex->width + 0xFF) << 5;
            dsdx = -dsdx & 0xFFFF;
        }
        if (flip & 2) {
            t = ((sprite->tex->height + 0xFF) << 5) - tfrac;
            dtdy = -dtdy & 0xFFFF;
        }
    }
    gSPScisTextureRectangle(gfx++, (s32)x << 2, (s32)y << 2, ((s32)(extraWidth + width - 1.0f) + (s32)x) << 2,
                            ((s32)(height - 1.0f) + iy) << 2, G_TX_RENDERTILE, s, t, dsdx, dtdy);
    if (sprite->tex->frames != 0) {
        step = sprite->animSpeed * D_800BE9A0;
        if (sprite->tex->frames & 0x80) {
            // ping-pong: bit 7 of animTimer is the direction
            if (sprite->animTimer & 0x80) {
                sprite->animTimer -= step;
                if (!(sprite->animTimer & 0x80)) {
                    sprite->animTimer = (sprite->animTimer + step) & 0x7F;
                }
            } else {
                sprite->animTimer += step;
                if (sprite->animTimer >= (sprite->tex->frames & 0x7F) * 8) {
                    sprite->animTimer = (sprite->animTimer - step) | 0x80;
                }
            }
        } else {
            sprite->animTimer += step;
            if (sprite->animTimer >= sprite->tex->frames * 8) {
                sprite->animTimer -= sprite->tex->frames * 8;
            }
        }
    }
    return gfx;
}

/* Draws `desc` as a width x height image at (x, y) scaled by (scaleX, scaleY) / 4096, split into
 * tiles of the texture's size (one asset per tile, frame `frame` starting at frame * tile count).
 * flags: bit 0 = tiles carry a 1-texel border, bit 1 = flip s, bit 2 = flip t. */
Gfx *func_1509629C(Gfx *gfx, TextureDesc *desc, f32 scaleX, f32 scaleY, f32 x, f32 y, s32 width, s32 height,
                   s32 flags, s32 sOffset, s32 extraWidth, s32 frame) {
    s32 tileW;
    s32 tileH;
    s32 fullTileW;
    s32 lastTileH;
    s32 fullTileH;
    s32 i;
    s32 j;
    s32 s;
    s32 t;
    s32 iy;
    f32 bottom;
    f32 tileHf;
    f32 top;
    s32 dsdx;
    s32 dtdy;
    s32 cols;
    s32 rows;
    f32 left;
    s32 dx;
    s32 sBase;
    s32 tBase;
    s32 ulx;
    s32 unused;
    f32 right;
    s32 pad[2]; // matching: frame size
    s32 dy;
    TextureDesc tile;

    tileW = desc->width;
    tileH = desc->height;
    if (flags & 1) {
        tileW -= 2;
        tileH -= 2;
    }
    fullTileW = tileW;
    fullTileH = tileH;
    if (flags & 2) {
        dsdx = (s32)(-4194304.0f / scaleX) & 0xFFFF;
        sBase = (desc->width + 0xFF) << 5;
    } else {
        dsdx = 4194304.0f / scaleX;
        sBase = 0x2000;
    }
    if (flags & 4) {
        dtdy = (s32)(-4194304.0f / scaleY) & 0xFFFF;
        tBase = (desc->height + 0xFF) << 5;
    } else {
        dtdy = 4194304.0f / scaleY;
        tBase = 0x2000;
    }
    cols = (width + tileW - 1) / tileW;
    rows = (height + tileH - 1) / tileH;
    tile.addr = desc->addr;
    x -= (width * scaleX) / 8192;
    tile.addr += frame * cols * rows;
    y -= (height * scaleY) / 8192;
    tile.width = desc->width;
    tile.fmt = desc->fmt;
    tile.siz = desc->siz;
    tile.frameCount = 1;
    lastTileH = height - (rows - 1) * tileH;
    left = x;
    for (i = 0; i < cols; i++) {
        unused = i + 1; // matching: dead store orders uopt's spill slots
        // the top row of each column takes the leftover height; the rows below it are full tiles
        tileH = lastTileH;
        tileHf = (tileH * scaleY) / 4096;
        tile.height = tileH;
        top = y;
        if (i == cols - 1) {
            tileW = width - (cols - 1) * fullTileW;
            tile.width = tileW;
            if (flags & 1) {
                tile.width += 2;
            }
        }
        s = sBase;
        right = left;
        right += (tileW * scaleX) / 4096;
        if (flags & 1) {
            s += (s32)(32 - (left - (s32)left) * 32.0f * (4096 / scaleX));
        }
        s += sOffset;
        if (flags & 1) {
            tile.height += 2;
        }
    lbl_unused:; // matching: extra basic block flips two register colourings
        for (j = 0; j < rows; j++) {
            bottom = top + tileHf;
            t = tBase;
            if (flags & 1) {
                t += (s32)(32 - (top - (s32)top) * 32.0f * (4096 / scaleY));
            }
            iy = top;
            gfx = func_15094F70(gfx, &tile, 0, NULL, 0, 0, 0, G_TX_CLAMP, 3);
            tile.addr++;
            dx = (s32)right - (s32)left;
            dy = (s32)bottom - iy;
            ulx = (s32)left * 4;
            gSPScisTextureRectangle(gfx++, ulx, iy * 4, ((s32)left + dx + extraWidth) * 4,
                                    (iy + dy) * 4, G_TX_RENDERTILE, s, t, dsdx, dtdy);
            if (j == 0) {
                tile.height = desc->height;
                tileH = fullTileH;
                tileHf = (tileH * scaleY) / 4096;
            }
            top = bottom;
        }
        left = right;
    }
    return gfx;
}


/* Emits display list D_80087408 and clears D_800D2DAB (back to the sprite prim colour). */
Gfx *func_15096934(Gfx *gfx) {
    gSPDisplayList(gfx++, D_80087408);
    D_800D2DAB = 0;
    return gfx;
}
