#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawn parameters of a 2D sprite effect, copied into the effect at offset 0x10. */
typedef struct {
    /* 0x10 */ void *texture;      // drawn with func_1509629C
    /* 0x14 */ void *tile1Texture; // loaded into tile 1 when flags & 1
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s16 lifetime;       // frames left; 0 = lives forever
    /* 0x26 */ s16 frame;          // animation frame, 8.8 fixed point
    /* 0x28 */ s16 frameSpeed;     // added to frame every tick
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ f32 x;
    /* 0x30 */ f32 y;
    /* 0x34 */ s16 sOffset;
    /* 0x36 */ s16 extraWidth;
    /* 0x38 */ s16 scaleX;         // 4096 = 1.0
    /* 0x3A */ s16 scaleY;
    /* 0x3C */ u16 width;
    /* 0x3E */ u16 height;
    /* 0x40 */ s8 behavior;        // index into D_8008CA20, 0 = none
    /* 0x41 */ u8 frameCount;
    /* 0x42 */ u8 red;
    /* 0x43 */ u8 green;
    /* 0x44 */ u8 blue;
    /* 0x45 */ u8 alpha;
    /* 0x46 */ u8 renderMode;      // index into D_800A4AC8
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 cycleType;       // 2 = two-cycle
    /* 0x49 */ u8 flags;
} Game196DB0SpriteParams;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ Game196DB0SpriteParams params;
} Game196DB0Sprite; // size 0x4C

void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void (*D_8008CA20[])(Game196DB0Sprite *);

/* Allocates a sprite effect and initialises it from `params`. */
Game196DB0Sprite *func_15169900(Game196DB0SpriteParams *params, s32 arg1) {
    Game196DB0Sprite *sprite;

    sprite = func_15167A68(0x5E, 0, sizeof(Game196DB0Sprite), 0, arg1, 1);
    if (sprite != NULL) {
        bcopy(params, &sprite->params, sizeof(sprite->params));
    }
    return sprite;
}

Game196DB0Sprite *func_15169968(Game196DB0SpriteParams *params) {
    return func_15169900(params, 0xFF);
}

/* Per-tick update: runs the sprite's behaviour, advances its animation and counts down its lifetime. */
void func_15169988(Game196DB0Sprite *sprite) {
    s32 behavior;
    s32 frame;
    s32 lifetime;
    s32 frameEnd;

    behavior = sprite->params.behavior;
    if (behavior != 0) {
        D_8008CA20[behavior](sprite);
    }

    frame = sprite->params.frame;
    frame += sprite->params.frameSpeed * D_800BE9E4;
    frameEnd = sprite->params.frameCount << 8;
    if (frame >= frameEnd) {
        frame -= frameEnd;
    } else if (frame < 0) {
        frame += frameEnd;
    }

    lifetime = sprite->params.lifetime;
    sprite->params.frame = frame;
    if (lifetime != 0) {
        lifetime -= D_800BE9E4;
        if (lifetime <= 0) {
            func_1516972C(&sprite->header);
            return;
        }
        sprite->params.lifetime = lifetime;
    }
}

typedef struct {
    s32 cycle1;
    s32 cycle2;
} Game196DB0RenderMode;
extern Game196DB0RenderMode D_800A4AC8[];

Gfx *func_15094F70(Gfx *, void *, s32, void *, s32, s32, s32, s32, s32);
Gfx *func_1509629C(Gfx *, void *, f32, f32, f32, f32, s32, s32, s32, s32, s32, s32);
Gfx *func_15142FBC(Gfx *gfx, s32 otherModeH, s32 otherModeL, u8 *update);
Gfx *func_1513F4E4(Gfx *gfx, u8 arg1, u8 *update);

/* Draws the sprite as a scaled textured rectangle. */
Gfx *func_15169A48(Gfx *gfx, Game196DB0Sprite *sprite, s32 arg2) {
    s32 pad; // matching: unused stack slots keep the original frame size
    s32 drawFlags;
    u8 update;
    s32 sfrac;
    s32 cycleType;
    s32 pad2;

    if (sprite->params.scaleX == 0 || sprite->params.scaleY == 0) {
        return gfx;
    }
    update = TRUE;
    gfx = func_1513F4E4(gfx, sprite->params.unk47, &update);
    gDPSetPrimColor(gfx++, 1, 0, sprite->params.red, sprite->params.green, sprite->params.blue,
                    sprite->params.alpha);
    if (sprite->params.flags & 1) {
        sfrac = (sprite->params.flags & 2) ? (sprite->params.sOffset * 4) >> 5 : 0;
        gfx = func_15094F70(gfx, sprite->params.tile1Texture, 0, NULL, 0x100, 1, sfrac, 2, 3);
    }
    drawFlags = (sprite->params.flags & 0x10) ? 1 : 0;
    if (sprite->params.flags & 4) {
        drawFlags |= 2;
    }
    if (sprite->params.flags & 8) {
        drawFlags |= 4;
    }
    if (sprite->params.flags & 0x20) {
        gDPSetPrimDepth(gfx++, 0x795A, 0);
    }
    cycleType = (sprite->params.cycleType == 2) ? G_CYC_2CYCLE : G_CYC_1CYCLE;
    gfx = func_15142FBC(gfx,
                        cycleType | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                        D_800A4AC8[sprite->params.renderMode].cycle1 | D_800A4AC8[sprite->params.renderMode].cycle2 |
                            G_ZS_PRIM,
                        &update);
    return func_1509629C(gfx, sprite->params.texture, sprite->params.scaleX, sprite->params.scaleY, sprite->params.x,
                         sprite->params.y, sprite->params.width, sprite->params.height, drawFlags,
                         sprite->params.sOffset, sprite->params.extraWidth, sprite->params.frame >> 8);
}
