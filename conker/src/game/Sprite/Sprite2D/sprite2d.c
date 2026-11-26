#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Allocates a sprite effect and initialises it from `params`. */
Sprite2D *sprite_newSprite2D(Sprite2DParams *params, s32 arg1) {
    Sprite2D *sprite;

    sprite = effect_newObject(0x5E, 0, sizeof(Sprite2D), 0, arg1, 1);
    if (sprite != NULL) {
        bcopy(params, &sprite->params, sizeof(sprite->params));
    }
    return sprite;
}

Sprite2D *sprite_newSprite2DDefault(Sprite2DParams *params) {
    return sprite_newSprite2D(params, 0xFF);
}

/* Per-tick update: runs the sprite's behaviour, advances its animation and counts down its lifetime. */
void sprite_updateSprite2D(Sprite2D *sprite) {
    s32 behavior;
    s32 frame;
    s32 lifetime;
    s32 frameEnd;

    behavior = sprite->params.behavior;
    if (behavior != 0) {
        gObjectCallbacks[behavior](sprite);
    }

    frame = sprite->params.frame;
    frame += sprite->params.frameSpeed * D_800BE9E4;
    frameEnd = sprite->params.frameCount << 8;
    if (frame >= frameEnd) {
        frame -= frameEnd;
    } else if (frame < 0) {
        frame += frameEnd;
    }

    lifetime = sprite->params.timer;
    sprite->params.frame = frame;
    if (lifetime != 0) {
        lifetime -= D_800BE9E4;
        if (lifetime <= 0) {
            effect_destroyObject(&sprite->header);
            return;
        }
        sprite->params.timer = lifetime;
    }
}

typedef struct {
    s32 cycle1;
    s32 cycle2;
} Game196DB0RenderMode;
extern Game196DB0RenderMode gEffectRenderModes[];

/* Draws the sprite as a scaled textured rectangle. */
Gfx *sprite_drawSprite2D(Gfx *gfx, Sprite2D *sprite, s32 arg2) {
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
    gfx = particle_setCombineMode(gfx, sprite->params.unk37, &update);
    gDPSetPrimColor(gfx++, 1, 0, sprite->params.red, sprite->params.green, sprite->params.blue,
                    sprite->params.alpha);
    if (sprite->params.flags & 1) {
        sfrac = (sprite->params.flags & 2) ? (sprite->params.sOffset * 4) >> 5 : 0;
        gfx = sprite_loadDescFrame(gfx, sprite->params.tile1Texture, 0, NULL, 0x100, 1, sfrac, 2, 3);
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
                        cycleType | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                        gEffectRenderModes[sprite->params.renderMode].cycle1 | gEffectRenderModes[sprite->params.renderMode].cycle2 |
                            G_ZS_PRIM,
                        &update);
    return sprite_drawImage(gfx, sprite->params.texture, sprite->params.scaleX, sprite->params.scaleY, sprite->params.x,
                         sprite->params.y, sprite->params.width, sprite->params.height, drawFlags,
                         sprite->params.sOffset, sprite->params.extraWidth, sprite->params.frame >> 8);
}
