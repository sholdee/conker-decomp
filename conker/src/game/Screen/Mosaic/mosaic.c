#include <ultra64.h>
#include "conker/colourfb.h"
#include "conker/effect.h"
#include "conker/screen.h"
#include "conker/view.h"


/* Starts a mosaic effect over owner for lifetime frames. */
MosaicEffect *screen_spawnMosaic(Actor *owner, s16 lifetime, u8 arg2, s32 arg3) {
    EffectDesc desc;
    MosaicData data;
    MosaicEffect *effect;

    if (owner == NULL) {
        return NULL;
    }
    data.halfWidth = 30000.0f;
    desc.flags = 1;
    desc.updateIndex = 1;
    desc.drawIndex = 0;
    desc.lifetime = lifetime;
    desc.messageIndex = 2;
    data.step = 10000;
    data.flags = 0;
    data.halfHeight = 30000.0f;
    data.owner = owner;
    data.ownerId = owner->unique_id;
    effect = (MosaicEffect *)colourfb_createScreenEffect(&desc, sizeof(data), arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    // matching: the original falls off the end here without returning effect
}

/* Offset that screen_updateMosaic transforms by the owner's bone matrix to centre the mosaic rectangle. */
static const Vec3f gMosaicCentreOffset = { 0.0f, -49.0f, -2.0f };

/*
 * Update handler: projects the owner's bone 52 to the screen and sets the mosaic rectangle,
 * clamped to the 292x216 view. Returns 0 to end the effect once the owner is gone.
 */
s32 screen_updateMosaic(MosaicEffect *effect) {
    MosaicData *mosaic = &effect->data;
    Vec3f pos;
    f32 projectedZ;
    f32 projectedW;
    f32 scale;

    if (mosaic->owner->type == 0) {
        return 0;
    }
    if (mosaic->owner->unique_id != mosaic->ownerId) {
        return 0;
    }
    if (mosaic->owner->ob_matrix_ptr == NULL) {
        mosaic->flags &= ~1;
    } else {
        math_transformOffset(&gMosaicCentreOffset, &pos, &mosaic->owner->ob_matrix_ptr[52]);
        if (math_projectToScreen(&pos, mosaic->center, &projectedZ, &projectedW, &scale, gCurrentView)) {
            f32 dx;
            f32 dy;

            dx = mosaic->halfWidth * scale;
            dy = mosaic->halfHeight * scale;
            mosaic->left = mosaic->center[0] - dx;
            mosaic->top = mosaic->center[1] - dy;
            mosaic->right = mosaic->center[0] + dx;
            mosaic->bottom = mosaic->center[1] + dy;
            if (mosaic->left > 291.0f || mosaic->right < 0.0f || mosaic->top > 215.0f || mosaic->bottom < 0.0f) {
                mosaic->flags &= ~1;
            } else {
                if (mosaic->left < 0.0f) {
                    mosaic->left = 0;
                }
                if (mosaic->top < 0.0f) {
                    mosaic->top = 0;
                }
                if (mosaic->right > 291.0f) {
                    mosaic->right = 291;
                }
                if (mosaic->bottom > 215.0f) {
                    mosaic->bottom = 215;
                }
                mosaic->flags |= 1;
            lbl_ret: // matching: an unused label here keeps IDO from merging this return with the one below
                return 1;
            }
        } else {
            mosaic->flags &= ~1;
        }
    }
    return 1;
}

/*
 * Draw handler: redraws the mosaic rectangle of the framebuffer in place. Each pixel takes the
 * colour of its block's source pixel; rows whose source row equals the row below are copied
 * whole. Works bottom-up and right-to-left so sources are read before they are overwritten.
 */
void screen_drawMosaic(MosaicEffect *effect, u16 *fb) {
    MosaicData *mosaic = &effect->data;
    s16 *srcColumn;
    s16 *srcRow;
    u32 acc;
    u32 i;
    s32 first;
    s32 src;
    s32 y;
    s32 x;

    if ((mosaic->flags & 1) == 0) {
        return;
    }

    srcColumn = mosaic->srcColumn;
    srcRow = mosaic->srcRow;

    acc = 0;
    first = mosaic->left;
    src = first;
    for (i = first; i <= mosaic->right; i++) {
        srcColumn[i] = src;
        acc += mosaic->step;
        if (acc >= 0x10000) {
            src = i;
            acc -= 0x10000;
        }
    }

    acc = 0;
    first = mosaic->top;
    src = first;
    for (i = first; i <= mosaic->bottom; i++) {
        srcRow[i] = src;
        acc += mosaic->step;
        if (acc >= 0x10000) {
            src = i;
            acc -= 0x10000;
        }
    }

    for (y = mosaic->bottom; y >= mosaic->top; y--) {
        if (y != mosaic->bottom && srcRow[y + 1] == srcRow[y]) {
            // matching: byte offsets keep fb as the first addu operand (&fb[i] swaps them)
            memcpy((u8 *)fb + (y * gScreenWidth + mosaic->left) * 2,
                   (u8 *)fb + ((y + 1) * gScreenWidth + mosaic->left) * 2,
                   (mosaic->right - mosaic->left + 1) * sizeof(u16));
        } else {
            for (x = mosaic->right; x >= mosaic->left; x--) {
                fb[y * gScreenWidth + x] = fb[srcRow[y] * gScreenWidth + srcColumn[x]];
            }
        }
    }
}

/*
 * Message handler: message 0x2D moves the effect between the two objects of the pair,
 * message 0 ends it when its owner goes away.
 */
void screen_handleMosaicMessage(MosaicEffect *effect, ActorMsg *msg, u8 type) {
    MosaicData *mosaic = &effect->data;
    Actor *object;

    if (type == 0x2D) {
        if (mosaic->owner == msg->swap.actorA) {
            mosaic->owner = msg->swap.actorB;
            mosaic->ownerId = msg->swap.idB;
        } else {
            if (mosaic->owner == msg->swap.actorB) {
                mosaic->owner = msg->swap.actorA;
                mosaic->ownerId = msg->swap.idA;
            }
        // matching: an empty labelled statement makes this branch jump straight to the epilogue
        end_swap:
            ;
        }
    } else if (type == 0) {
        object = msg->ref.actor;
        if (object == mosaic->owner || msg->ref.uniqueId == mosaic->ownerId) {
            effect_destroyObject(&effect->header);
        }
    }
}
