#include <ultra64.h>
/* functions.h declares func_15169260(s32, ...); passing a struct pointer through an s32 cast changes codegen. */
#define func_15169260 func_15169260_s32_decl
#include "functions.h"
#undef func_15169260
#include "variables.h"

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} SpriteColor;

/* The 0x58-byte description a screen sprite is created from (copied to ScreenSprite.info). */
typedef struct {
    /* 0x00 */ f32 x;           /* centre, in viewport units */
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 halfSize[2];
    /* 0x10 */ u8 texture;    /* index into D_80090B60, 0xFF = none */
    /* 0x11 */ u8 pad11;
    /* 0x12 */ s16 timer;     /* counts down by D_800BE9E4 when (flags & 1) */
    /* 0x14 */ u16 flags;
    /* 0x16 */ s16 fadeStart; /* (flags & 8): below this timer, alpha = timer * fadeRate */
    /* 0x18 */ s16 fadeRate;
    /* 0x1A */ u8 kind;       /* index into D_8008AD04 */
    /* 0x1B */ SpriteColor color1;
    /* 0x1F */ SpriteColor color2;
    /* 0x23 */ u8 unk23;
    /* 0x24 */ s32 otherModeH;  /* ORed with unk38, unk3C into the othermode-H word */
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 combineMode; /* index into D_800A4AC8 */
    /* 0x34 */ u8 pad34[3];
    /* 0x37 */ u8 unk37;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s8 unk40;
    /* 0x41 */ s8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ u8 viewport;   /* the only viewport drawn in when (flags & 0x10) */
    /* 0x45 */ u8 pad45[3];
    /* 0x48 */ f32 scaleX;    /* (flags & 0x80) */
    /* 0x4C */ f32 scaleY;
    /* 0x50 */ f32 offsetS;   /* (flags & 0x100) */
    /* 0x54 */ f32 offsetT;
} ScreenSpriteInfo;

typedef struct ScreenSprite {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ struct ScreenSprite *next;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ ScreenSpriteInfo info;
    /* 0x68 */ u8 unk68;      /* low nibble: index into D_8008ACC8 */
} ScreenSprite;

/* Per-viewport record (0x180 bytes) of the table D_800BE628 points to. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 scaleX;
    /* 0x18 */ f32 scaleY;
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ f32 centerX;
    /* 0x38 */ f32 centerY;
    /* 0x3C */ u8 pad3C[0x144];
} Viewport;

typedef struct {
    u8 pad0[0x6];
    u16 width;
    u16 height;
    u8 padA[0x2];
} SpriteTexture;

typedef struct {
    s32 unk0;
    s32 unk4;
} CombineMode;

typedef struct {
    s32 unk0;
    s32 unk4;
} Pair1515572C;

/* The two sprite lists func_151555AC searches. */
typedef struct {
    s32 id[2];
} SpriteListIds;

extern s32 (*D_8008ACC8[])(ScreenSprite *);
extern void (*D_8008AD04[])(ScreenSprite *, s32, u8);
extern SpriteTexture D_80090B60[];
extern CombineMode D_800A4AC8[];
extern SpriteListIds D_800A6030;
extern Pair1515572C D_800A6038;
extern s32 D_800D2C9C;
extern ScreenSprite *D_800DCE50[2][0x68];

extern void func_15169260(Pair1515572C *, s32, s32, u8);
extern s32 func_15181CC8(s32);
extern s32 func_1517EF00(s32);
extern ScreenSprite *func_15167A68(s32, s32, s32, s32, u8, s32);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
void func_151441A4(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, u8 arg8, u8 arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13);
void func_151442FC(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13);
s32 func_151555AC(f32 *center, f32 *halfSize, s32 *kinds, s32 kindCount);

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
        func_1516972C((struct102 *)sprite);
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
    Viewport *vp;

    update = 1;
    // matching: integer address arithmetic; indexing a Viewport array orders the addu differently
    vp = (Viewport *)&D_800BE628[D_80082FA4];
    if ((sprite->info.flags & 0x10) && (viewport != sprite->info.viewport)) {
        return gfx;
    }
    if ((sprite->info.flags & 0x40) && (D_800C35EA == 1)) {
        return gfx;
    }
    /* flag bits 11 and up: per viewport, skip unless func_15181CC8 and func_1517EF00 allow it */
    if ((sprite->info.flags & (1 << (viewport + 11))) &&
        ((func_15181CC8(viewport) == 0) || (func_1517EF00(viewport) > 200))) {
        return gfx;
    }
    if (sprite->info.texture != 0xFF) {
        gfx = func_15142E24(gfx, (s32)&D_80090B60[sprite->info.texture], 0, (sprite->info.flags & 0x200) ? 0 : 2,
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
    gfx = func_1513F4E4(gfx, sprite->info.unk37, &update);
    gfx = func_15142FBC(gfx, sprite->info.otherModeH | D_800D2C9C | G_TF_BILERP | G_TC_FILT |
                            sprite->info.unk38 | sprite->info.unk3C,
                        D_800A4AC8[sprite->info.combineMode].unk0 | D_800A4AC8[sprite->info.combineMode].unk4, &update);
    x = vp->centerX + sprite->info.x * vp->scaleX;
    y = vp->centerY + sprite->info.y * vp->scaleY;
    while (0) {}  // matching: block boundary; without it IDO keeps vp in a different register
    halfWidth = sprite->info.halfSize[0] * vp->scaleX;
    halfHeight = sprite->info.halfSize[1] * vp->scaleY;
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
 * rectangle overlaps a live sprite of one of those kinds.
 */
ScreenSprite *func_1515548C(ScreenSpriteInfo *info, u8 flags, s32 *kinds, s32 kindCount, s32 arg4, u8 arg5, s32 arg6) {
    ScreenSprite *sprite;
    f32 halfSize[2];

    if ((kinds != NULL) && (kindCount > 0)) {
        halfSize[0] = info->halfSize[0];
        halfSize[1] = info->halfSize[1];
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
            sprite = D_800DCE50[table][lists.id[i]];
            while (sprite != NULL) {
                next = sprite->next;
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
                    w = halfSize[0] + sprite->info.halfSize[0];
                    h = halfSize[1] + sprite->info.halfSize[1];
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
    Pair1515572C pair;

    pair = D_800A6038;
    func_15169260(&pair, 2, arg0, arg1);
}
