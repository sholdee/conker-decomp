#include <ultra64.h>
#include "conker/colourfb.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/screen.h"
#include "conker/sight.h"
#include "libc/string.h"

/* Object type 0x34: screen effects from ../Effects/ColourFrameBuffer/colourframebuffer.c */
#define OBJ_TYPE_SCREEN_EFFECT 0x34

/* Update and draw handlers per screen effect kind (EffectDesc.updateIndex, drawIndex). Declared without
 * a prototype: each handler takes the effect as its own type. */
static s32 (*gScreenEffectUpdateProcs[])() = { colourfb_readTunables, screen_updateMosaic };
static void (*gScreenEffectDrawProcs[])() = { screen_drawMosaic, sight_drawLens, particle_drawGreyscaleScreen };
/* Message handlers per kind (EffectDesc.messageIndex), as colourfb_dispatchMessage calls them; the handlers take
 * the effect and the message as their own types. */
static void (*gScreenEffectMessageProcs[])(ScreenEffect *, s32, u8) = { NULL, sight_handleLensMessage, screen_handleMosaicMessage };

/* Draws every screen effect in both object lists through its draw handler. */
void colourfb_drawScreenEffects(s32 arg0) {
    u8 list;
    u8 visible;

    for (list = 0; list < 2; list++) {
        ScreenEffect *effect;

        effect = (ScreenEffect *)gObjectLists[list][OBJ_TYPE_SCREEN_EFFECT];
        gObjectWalkDepth++;
        for (; effect != NULL; effect = gObjectWalkNext[gObjectWalkDepth]) {
            gObjectWalkNext[gObjectWalkDepth] = effect->next;
            if (effect->desc.flags & 4) {
                effect->desc.flags &= ~4;
                continue;
            }
            if (effect->desc.drawIndex == -1) {
                continue;
            }
            visible = TRUE;
            if ((effect->desc.flags & 2) && (gCutsceneStates[0] == 1)) {
                visible = FALSE;
            }
            if ((effect->desc.flags & 8) && ((screen_isIrisOpen(0) == 0) || (screen_getViewFadeAlpha(0) != 0))) {
                visible = FALSE;
            }
            if (visible) {
                gScreenEffectDrawProcs[effect->desc.drawIndex](effect, arg0);
            }
        }
        gObjectWalkDepth--;
    }
}

/* Creates a screen effect from desc with dataSize bytes of effect data after the header. */
ScreenEffect *colourfb_createScreenEffect(EffectDesc *desc, s32 dataSize, u8 owner, s32 list) {
    ScreenEffect *effect;

    effect = effect_newObject(OBJ_TYPE_SCREEN_EFFECT, list, dataSize + sizeof(ScreenEffect), 1, owner, 1);
    if (effect == NULL) {
        return NULL;
    }
    memcpy(&effect->desc, desc, sizeof(EffectDesc));
    return effect;
}

/* Update: counts down the lifetime and runs the update handler; destroys the effect when either ends it. */
void colourfb_updateScreenEffect(ScreenEffect *effect) {
    s32 updateIndex;
    u8 finished;

    finished = FALSE;
    if (effect->desc.flags & 1) {
        effect->desc.lifetime -= gFrameTicks;
        if (effect->desc.lifetime < 0) {
            finished = TRUE;
        }
    }
    if (!finished) {
        updateIndex = effect->desc.updateIndex;
        if (updateIndex != -1) {
            if (gScreenEffectUpdateProcs[updateIndex](effect) == 0) {
                finished = TRUE;
            }
        }
    }
    if (finished) {
        effect_destroyObject(effect);
    }
}

/* Message handler: forwards the message to the effect's own handler, if it has one. */
void colourfb_dispatchMessage(ScreenEffect *effect, s32 data, u8 event) {
    void (*handler)(ScreenEffect *, s32, u8);

    handler = gScreenEffectMessageProcs[effect->desc.messageIndex];
    if (handler != NULL) {
        handler(effect, data, event);
    }
}

/* Update handler: reads the effect's position, size and pixelation from the debug tunables. */
s32 colourfb_readTunables(ColourFrameBufferEffect *effect) {
    ColourFrameBufferData *data;
    s32 tunable;

    data = &effect->data;
    tunable = 0;
    data->pos[0] = math_debugTunable(tunable++, "pos s", -2000, 2000, 0,
                                "../Effects/ColourFrameBuffer/colourframebuffer.c", 452) * 0.001000000047f;
    data->pos[1] = math_debugTunable(tunable++, "pos t", -2000, 2000, 0,
                                "../Effects/ColourFrameBuffer/colourframebuffer.c", 457) * 0.001000000047f;
    data->size[0] = math_debugTunable(tunable++, "size s", 0, 2000, 500,
                                 "../Effects/ColourFrameBuffer/colourframebuffer.c", 462) * 0.001000000047f;
    data->size[1] = math_debugTunable(tunable++, "size t", 0, 2000, 500,
                                 "../Effects/ColourFrameBuffer/colourframebuffer.c", 467) * 0.001000000047f;
    data->pixelation = math_debugTunable(tunable, "pixelation", 0, 0x10000, 0x10000,
                                     "../Effects/ColourFrameBuffer/colourframebuffer.c", 473);
    return 1;
}

/* Sends message `event` with `data` to every screen effect. */
void colourfb_broadcastMessage(data, event)
    s32 data;
    u8 event;
{
    const s32 kinds[1] = { OBJ_TYPE_SCREEN_EFFECT };

    effect_sendEventToTypes(kinds, 1, data, event);
}
