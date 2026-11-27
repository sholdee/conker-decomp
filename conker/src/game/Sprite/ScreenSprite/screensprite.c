#include <ultra64.h>
#include "conker/blood.h"
#include "conker/camera.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/screen.h"
#include "conker/sight.h"
#include "conker/sprite.h"
#include "conker/texture.h"
#include "conker/view.h"

/* The two sprite lists sprite_overlapsScreenSprite searches. */
typedef struct {
    s32 id[2];
} SpriteListIds;

/* The render setup and reset display lists of the sprite object type (the object type table
   D_8008B4D4 points to them). */
Gfx gScreenSpriteSetupDl[] = {
    gsDPPipeSync(),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPMatrix(&gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx gScreenSpriteTeardownDl[] = {
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
   (sprite_screenSpriteMessage narrows the u8 message id for it), and its entries take their own sprite and payload
   types (warning 709). */
s32 (*gScreenSpriteUpdates[15])() = {
    NULL,          func_15134CEC, particle_updateSlidingSprite, blood_updateSplat, sight_updateZTriggerFade,
    sight_updateSidePanel, particle_updateSineOscillator, sight_updateCircleDot, sight_updateRingGauge, sight_updateCrossBar,
    effect_updateRisingScreenSprite, sight_updateScopeStrip, effect_updateHopSprite, effect_updateBounceSprite, effect_updateFadeInSprite,
};
void (*gScreenSpriteMsgHandlers[11])(ScreenSprite *, s32, u8) = {
    NULL, particle_handleSlidingSpriteMessage, NULL, NULL, NULL, NULL, NULL, sight_handleSpriteMessage, effect_hopSpriteMessage, NULL, NULL,
};

/* The two sprite lists sprite_overlapsScreenSprite searches. */
static const SpriteListIds gScreenSpriteLists = { 93, 87 };

/* The same two lists, for sprite_sendEventToScreenSprites. */
static const IdList2 gScreenSpriteEventTypes = { 93, 87 };

/* Per-frame update of a sprite: runs its timer, its update callback and its fade-out; frees it when done. */
void sprite_updateScreenSprite(ScreenSprite *sprite) {
    s32 index;
    s8 finished;

    finished = 0;
    if (sprite->info.flags & 1) {
        sprite->info.timer -= gFrameTicks;
        if (sprite->info.timer < 0) {
            finished = 1;
        }
    }
    index = sprite->unk68 & 0xF;
    if (index != 0 && !finished) {
        if (gScreenSpriteUpdates[index](sprite) == 0) {
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
        effect_destroyObject(&sprite->header);
    }
}

/*
 * Draws a sprite as a scissored texture rectangle in the given viewport, unless its flags hide it there.
 */
Gfx *sprite_drawScreenSprite(Gfx *gfx, ScreenSprite *sprite, s16 viewport) {
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
    if ((sprite->info.flags & 0x40) && (gCutsceneStates[0] == 1)) {
        return gfx;
    }
    /* flag bits 11 and up: per viewport, skip unless screen_isIrisOpen and screen_getViewFadeAlpha allow it */
    if ((sprite->info.flags & (1 << (viewport + 11))) &&
        ((screen_isIrisOpen(viewport) == 0) || (screen_getViewFadeAlpha(viewport) > 200))) {
        return gfx;
    }
    if (sprite->info.texture != 0xFF) {
        // matching: the pointer goes through an integer (IDO keeps the converted value in a temporary)
        gfx = math_gfxLoadTextureFrame(gfx, (TextureDesc *)(s32)&gEffectTextures[sprite->info.texture], 0, (sprite->info.flags & 0x200) ? 0 : 2,
                            0, 0, sprite->info.texture, 0, 0, &update, (sprite->info.flags & 0x400) ? 0x3E : 3);
    }
    gfx = math_gfxUpdateGeometryMode(gfx, sprite->info.render.setGeometryMode, sprite->info.render.clearGeometryMode);
    math_pickPrimColour(&primR, &primG, &primB, &primA,
                  sprite->info.color2.red, sprite->info.color2.green, sprite->info.color2.blue, sprite->info.color2.alpha,
                  sprite->info.color1.red, sprite->info.color1.green, sprite->info.color1.blue, sprite->info.color1.alpha,
                  sprite->info.colorLevel, sprite->info.render.primMode);
    math_pickEnvColour(&envR, &envG, &envB, &envA,
                  sprite->info.color2.red, sprite->info.color2.green, sprite->info.color2.blue, sprite->info.color2.alpha,
                  sprite->info.color1.red, sprite->info.color1.green, sprite->info.color1.blue, sprite->info.color1.alpha,
                  sprite->info.colorLevel, sprite->info.render.envMode);
    gfx = math_gfxSetEnvColor(gfx, envR, envG, envB, envA, &update);
    gfx = math_gfxSetPrimColor(gfx, 0, 0, primR, primG, primB, primA, &update);
    // matching: the draw reads only the low byte of render.combiner (lbu at 0x37)
    gfx = particle_setCombineMode(gfx, ((u8 *)&sprite->info.render.combiner)[3], &update);
    gfx = math_gfxSetOtherMode(gfx, sprite->info.render.otherModeH | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT |
                            sprite->info.render.colorDither | sprite->info.render.alphaDither,
                        gEffectRenderModes[sprite->info.render.renderMode].cycle1 | gEffectRenderModes[sprite->info.render.renderMode].cycle2, &update);
    x = vp->centerX + sprite->info.x * vp->scaleX;
    y = vp->centerY + sprite->info.y * vp->scaleY;
    while (0) {}  // matching: block boundary; without it IDO keeps vp in a different register
    halfWidth = sprite->info.halfWidth * vp->scaleX;
    halfHeight = sprite->info.halfHeight * vp->scaleY;
    xl = (x - halfWidth) * 4.0f;
    yl = (y - halfHeight) * 4.0f;
    xh = (x + halfWidth) * 4.0f;
    yh = (y + halfHeight) * 4.0f;
    texWidth = gEffectTextures[sprite->info.texture].width;
    texWidthF = gEffectTextures[sprite->info.texture].width;
    while (0) {}  // matching: block boundary steers the scheduling of the texture size loads
    texHeightF = gEffectTextures[sprite->info.texture].height;
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
        t = (gEffectTextures[sprite->info.texture].height - 1) << 5;
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
void *sprite_newScreenSprite(ScreenSpriteInfo *info, u8 flags, s32 *kinds, s32 kindCount, s32 extraSize, u8 owner, s32 list) {
    ScreenSprite *sprite;
    f32 halfSize[2];

    if ((kinds != NULL) && (kindCount > 0)) {
        halfSize[0] = info->halfWidth;
        halfSize[1] = info->halfHeight;
        if (sprite_overlapsScreenSprite(&info->x, halfSize, kinds, kindCount) != 0) {
            return NULL;
        }
    }
    sprite = effect_newObject((info->flags & 0x20) ? 0x57 : 0x5D, list, extraSize + 0x70, 1, owner, 1);
    if (sprite == NULL) {
        return NULL;
    }
    memcpy(&sprite->info, info, sizeof(ScreenSpriteInfo));
    sprite->unk68 = 0;
    sprite->unk68 |= flags;
    return sprite;
}

/* Calls the handler for the sprite's kind, if it has one. */
void sprite_screenSpriteMessage(ScreenSprite *sprite, s32 event, u8 eventType) {
    if (gScreenSpriteMsgHandlers[sprite->info.kind] != NULL) {
        gScreenSpriteMsgHandlers[sprite->info.kind](sprite, event, eventType);
    }
}

/*
 * Returns 1 when a live sprite of one of the given kinds overlaps the rectangle center +- halfSize
 * (its own half size added), searching both sprite lists gScreenSpriteLists names in both tables.
 */
s32 sprite_overlapsScreenSprite(f32 *center, f32 *halfSize, s32 *kinds, s32 kindCount) {
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
        lists = gScreenSpriteLists;
        i &= 0; // matching: `i = 0` here (or a for loop) changes the loop's branch shape
        do {
            sprite = (ScreenSprite *)gObjectLists[table][lists.id[i]];
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

void sprite_sendEventToScreenSprites(s32 event, u8 eventType) {
    IdList2 pair;

    pair = gScreenSpriteEventTypes;
    effect_sendEventToTypes(pair.ids, 2, event, eventType);
}
