#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    s32 unk0;
    s32 unk4;
} CombineMode;

/* The two sprite lists func_151555AC searches. */
typedef struct {
    s32 id[2];
} SpriteListIds;

extern CombineMode D_800A4AC8[];

/* The render setup and reset display lists of the sprite object type (the object type table
   D_8008B4D4 points to them). */
Gfx D_8008AC70[] = {
    gsDPPipeSync(),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPMatrix(&D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx D_8008AC98[] = {
    gsDPPipeSync(),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetDepthSource(G_ZS_PIXEL),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

/* The update callback of each sprite kind (ScreenSprite.unk68 & 0xF; nonzero: keep the sprite) and the
   message handler of each kind (ScreenSprite.info.kind). The callbacks take their own sprite types, so
   their table is declared without a prototype. The handler table keeps the prototype of its call
   (func_15155564 narrows the u8 message id for it), and its entries take their own sprite and payload
   types (warning 709). */
s32 (*D_8008ACC8[15])() = {
    NULL,          func_15134CEC, func_1515B994, func_15135BF8, func_151CB510,
    sight_updateSidePanel, func_150AFBF4, func_151CBBE0, func_151CBC60, sight_updateCrossBar,
    func_150BE1C4, sight_updateScopeStrip, func_150CF800, func_150CFC60, func_150FB2E0,
};
void (*D_8008AD04[11])(ScreenSprite *, s32, u8) = {
    NULL, func_1515B62C, NULL, NULL, NULL, NULL, NULL, sight_handleSpriteMessage, func_150CFBEC, NULL, NULL,
};

/* The two sprite lists func_151555AC searches. */
static const SpriteListIds D_800A6030 = { 93, 87 };

/* The same two lists, for func_1515572C. */
static const IdList2 D_800A6038 = { 93, 87 };

/* Per-frame update of a sprite: runs its timer, its update callback and its fade-out; frees it when done. */
void func_15154C90(ScreenSprite *sprite) {
    s32 index;
    s8 finished;

    finished = 0;
    if (sprite->info.flags & 1) {
        sprite->info.timer -= D_800BE9E4;
        if (sprite->info.timer < 0) {
            finished = 1;
        }
    }
    index = sprite->unk68 & 0xF;
    if (index != 0 && !finished) {
        if (D_8008ACC8[index](sprite) == 0) {
            finished = 1;
        }
    }
    if (sprite->info.flags & 8) {
        if (sprite->info.timer < sprite->info.fadeStart) {
            // matching: the alpha reuses index; a separate local changes register allocation
            index = (s16)(sprite->info.timer * sprite->info.fadeRate);
            if (index < sprite->info.color1.alpha) {
                sprite->info.color1.alpha = index;
            }
        }
    }
    if (finished) {
        func_1516972C(&sprite->header);
    }
}

/*
 * Draws a sprite as a scissored texture rectangle in the given viewport, unless its flags hide it there.
 */
Gfx *func_15154D80(Gfx *gfx, ScreenSprite *sprite, s16 viewport) {
    s32 s;
    u8 update;
    u16 texWidth;
    s32 t;
    s32 dsdx;
    s32 dtdy;
    u32 pad; // matching: unused stack slot
    f32 x;
    f32 y;
    f32 halfWidth;
    f32 width;
    f32 height;
    f32 xl;
    f32 yl;
    f32 halfHeight;
    f32 texWidthF;
    f32 texHeightF;
    f32 xh;
    f32 yh;
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;
    RenderView *vp;

    update = 1;
    // matching: integer address arithmetic; indexing a RenderView array orders the addu differently
    vp = &gViews[gCurrentView];
    if ((sprite->info.flags & 0x10) && (viewport != sprite->info.viewport)) {
        return gfx;
    }
    if ((sprite->info.flags & 0x40) && (D_800C35EA[0] == 1)) {
        return gfx;
    }
    /* flag bits 11 and up: per viewport, skip unless screen_isIrisOpen and screen_getViewFadeAlpha allow it */
    if ((sprite->info.flags & (1 << (viewport + 11))) &&
        ((screen_isIrisOpen(viewport) == 0) || (screen_getViewFadeAlpha(viewport) > 200))) {
        return gfx;
    }
    if (sprite->info.texture != 0xFF) {
        // matching: the pointer goes through an integer (IDO keeps the converted value in a temporary)
        gfx = func_15142E24(gfx, (TextureDesc *)(s32)&D_80090B60[sprite->info.texture], 0, (sprite->info.flags & 0x200) ? 0 : 2,
                            0, 0, sprite->info.texture, 0, 0, &update, (sprite->info.flags & 0x400) ? 0x3E : 3);
    }
    gfx = func_15142B7C(gfx, sprite->info.unk28, sprite->info.unk2C);
    func_151441A4(&primR, &primG, &primB, &primA,
                  sprite->info.color2.red, sprite->info.color2.green, sprite->info.color2.blue, sprite->info.color2.alpha,
                  sprite->info.color1.red, sprite->info.color1.green, sprite->info.color1.blue, sprite->info.color1.alpha,
                  sprite->info.unk23, sprite->info.unk40);
    func_151442FC(&envR, &envG, &envB, &envA,
                  sprite->info.color2.red, sprite->info.color2.green, sprite->info.color2.blue, sprite->info.color2.alpha,
                  sprite->info.color1.red, sprite->info.color1.green, sprite->info.color1.blue, sprite->info.color1.alpha,
                  sprite->info.unk23, sprite->info.unk41);
    gfx = func_15142C10(gfx, envR, envG, envB, envA, &update);
    gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &update);
    // matching: the draw reads only the low byte of unk34 (lbu at 0x37)
    gfx = func_1513F4E4(gfx, ((u8 *)&sprite->info.unk34)[3], &update);
    gfx = func_15142FBC(gfx, sprite->info.otherModeH | D_800D2C9C | G_TF_BILERP | G_TC_FILT |
                            sprite->info.unk38 | sprite->info.unk3C,
                        D_800A4AC8[sprite->info.combineMode].unk0 | D_800A4AC8[sprite->info.combineMode].unk4, &update);
    x = vp->centerX + sprite->info.x * vp->scaleX;
    y = vp->centerY + sprite->info.y * vp->scaleY;
    while (0) {}  // matching: block boundary; without it IDO keeps vp in a different register
    halfWidth = sprite->info.halfWidth * vp->scaleX;
    halfHeight = sprite->info.halfHeight * vp->scaleY;
    xl = (x - halfWidth) * 4.0f;
    yl = (y - halfHeight) * 4.0f;
    xh = (x + halfWidth) * 4.0f;
    yh = (y + halfHeight) * 4.0f;
    texWidth = D_80090B60[sprite->info.texture].width;
    texWidthF = D_80090B60[sprite->info.texture].width;
    while (0) {}  // matching: block boundary steers the scheduling of the texture size loads
    texHeightF = D_80090B60[sprite->info.texture].height;
    width = (xh - xl) + 1.0f;
    height = (yh - yl) + 1.0f;
    if (sprite->info.flags & 0x80) {
        width *= sprite->info.scaleX;
        height *= sprite->info.scaleY;
    }
    if (width != 0.0f) {
        dsdx = (4096.0f * texWidthF) / width;
    } else {
        dsdx = 0;
    }
    if (height != 0.0f) {
        dtdy = (4096.0f * texHeightF) / height;
    } else {
        dtdy = 0;
    }
    s = 0;
    t = 0;
    if (sprite->info.flags & 2) { /* mirror horizontally */
        s = (texWidth - 1) << 5;
        dsdx = -dsdx & 0xFFFF;
    }
    if (sprite->info.flags & 4) { /* mirror vertically */
        t = (D_80090B60[sprite->info.texture].height - 1) << 5;
        dtdy = -dtdy & 0xFFFF;
    }
    if (sprite->info.flags & 0x100) {
        s += sprite->info.offsetS;
        t += sprite->info.offsetT;
    }
    gSPScisTextureRectangle(gfx++, (s32)xl, (s32)yl, (s32)xh, (s32)yh, G_TX_RENDERTILE, s, t, dsdx, dtdy);
    return gfx;
}

/*
 * Creates a sprite from info, flagged with flags. With kinds given, it is not created (NULL) when its
 * rectangle overlaps a live sprite of one of those kinds. Callers keep their own view of the sprite.
 */
void *func_1515548C(ScreenSpriteInfo *info, u8 flags, s32 *kinds, s32 kindCount, s32 arg4, u8 arg5, s32 arg6) {
    ScreenSprite *sprite;
    f32 halfSize[2];

    if ((kinds != NULL) && (kindCount > 0)) {
        halfSize[0] = info->halfWidth;
        halfSize[1] = info->halfHeight;
        if (func_151555AC(&info->x, halfSize, kinds, kindCount) != 0) {
            return NULL;
        }
    }
    sprite = func_15167A68((info->flags & 0x20) ? 0x57 : 0x5D, arg6, arg4 + 0x70, 1, arg5, 1);
    if (sprite == NULL) {
        return NULL;
    }
    memcpy(&sprite->info, info, sizeof(ScreenSpriteInfo));
    sprite->unk68 = 0;
    sprite->unk68 |= flags;
    return sprite;
}

/* Calls the handler for the sprite's kind, if it has one. */
void func_15155564(ScreenSprite *sprite, s32 arg1, u8 arg2) {
    if (D_8008AD04[sprite->info.kind] != NULL) {
        D_8008AD04[sprite->info.kind](sprite, arg1, arg2);
    }
}

/*
 * Returns 1 when a live sprite of one of the given kinds overlaps the rectangle center +- halfSize
 * (its own half size added), searching both sprite lists D_800A6030 names in both tables.
 */
s32 func_151555AC(f32 *center, f32 *halfSize, s32 *kinds, s32 kindCount) {
    s32 table;
    SpriteListIds lists;
    u8 i;
    ScreenSprite *sprite;
    ScreenSprite *next;
    s32 found;
    s32 k;
    f32 w;
    f32 h;

    i = 0;
    for (table = 0; table < 2; table++) {
        lists = D_800A6030;
        i &= 0; // matching: `i = 0` here (or a for loop) changes the loop's branch shape
        do {
            sprite = (ScreenSprite *)D_800DCE50[table][lists.id[i]];
            while (sprite != NULL) {
                next = (ScreenSprite *)sprite->header.next;
                k = kindCount;
                found = 0;
                while (k > 0 && !found) {
                    if (sprite->info.kind == kinds[k - 1]) {
                        found = 1;
                    } else {
                        k--;
                    }
                }
                if (found) {
                    // matching: indexing from halfWidth (as an array) keeps the original load order
                    w = halfSize[0] + (&sprite->info.halfWidth)[0];
                    h = halfSize[1] + (&sprite->info.halfWidth)[1];
                    if (center[0] - w <= sprite->info.x && sprite->info.x <= center[0] + w &&
                        center[1] - h <= sprite->info.y && sprite->info.y <= center[1] + h) {
                        return 1;
                    }
                }
                sprite = next;
            }
lbl: // matching: unused label; it splits the block before i++
            i++;
        } while (i < 2);
        i = 0; // matching: redundant reset fills the outer loop's delay slot
    }
    // matching: no `return 0`; i lives in v0, which is 0 once both tables are searched
}

void func_1515572C(s32 arg0, u8 arg1) {
    IdList2 pair;

    pair = D_800A6038;
    func_15169260(pair.ids, 2, arg0, arg1);
}
