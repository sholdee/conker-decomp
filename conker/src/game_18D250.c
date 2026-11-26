#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Object type 0x34: screen effects from ../Effects/ColourFrameBuffer/colourframebuffer.c */
#define OBJ_TYPE_SCREEN_EFFECT 0x34

extern s32 (*D_8008B0D0[])(ScreenEffect *);
extern void (*D_8008B0D8[])(ScreenEffect *, s32);
extern void (*D_8008B0E4[])(ScreenEffect *, s32, u8);

/* Draws every screen effect in both object lists through its draw handler. */
void func_1515FDA0(s32 arg0) {
    u8 list;
    ScreenEffect *effect;
    u8 visible;

    for (list = 0; list < 2; list++) {
        effect = (ScreenEffect *)D_800DCE50[list][OBJ_TYPE_SCREEN_EFFECT];
        D_800DD190++;
        for (; effect != NULL; effect = D_800DD198[D_800DD190]) {
            D_800DD198[D_800DD190] = effect->next;
            if (effect->desc.flags & 4) {
                effect->desc.flags &= ~4;
                continue;
            }
            if (effect->desc.drawIndex == -1) {
                continue;
            }
            visible = TRUE;
            if ((effect->desc.flags & 2) && (D_800C35EA[0] == 1)) {
                visible = FALSE;
            }
            if ((effect->desc.flags & 8) && ((func_15181CC8(0) == 0) || (func_1517EF00(0) != 0))) {
                visible = FALSE;
            }
            if (visible) {
                D_8008B0D8[effect->desc.drawIndex](effect, arg0);
            }
        }
        {
            // matching: only a block-scope redeclaration (not a plain block or -= 1) schedules the decrement's lui first
            extern s8 D_800DD190;

            D_800DD190--;
        }
    }
}

/* Creates a screen effect from desc with dataSize bytes of effect data after the header. */
ScreenEffect *func_1515FF74(EffectDesc *desc, s32 dataSize, u8 owner, s32 list) {
    ScreenEffect *effect;

    effect = func_15167A68(OBJ_TYPE_SCREEN_EFFECT, list, dataSize + sizeof(ScreenEffect), 1, owner, 1);
    if (effect == NULL) {
        return NULL;
    }
    memcpy(&effect->desc, desc, sizeof(EffectDesc));
    return effect;
}

/* Update: counts down the lifetime and runs the update handler; destroys the effect when either ends it. */
void func_1515FFEC(ScreenEffect *effect) {
    s32 updateIndex;
    u8 finished;

    finished = FALSE;
    if (effect->desc.flags & 1) {
        effect->desc.lifetime -= D_800BE9E4;
        if (effect->desc.lifetime < 0) {
            finished = TRUE;
        }
    }
    if (!finished) {
        updateIndex = effect->desc.updateIndex;
        if (updateIndex != -1) {
            if (D_8008B0D0[updateIndex](effect) == 0) {
                finished = TRUE;
            }
        }
    }
    if (finished) {
        func_1516972C((EffectHeader *)effect);
    }
}

/* Message handler: forwards the message to the effect's own handler, if it has one. */
void func_15160090(ScreenEffect *effect, s32 data, u8 event) {
    void (*handler)(ScreenEffect *, s32, u8);

    handler = D_8008B0E4[effect->desc.messageIndex];
    if (handler != NULL) {
        handler(effect, data, event);
    }
}

/* Update handler: reads the effect's position, size and pixelation from the debug tunables. */
s32 func_151600D8(ColourFrameBufferEffect *effect) {
    ColourFrameBufferData *data;
    s32 tunable;

    data = &effect->data;
    tunable = 0;
    data->pos[0] = (f32)func_151422DC(tunable++, "pos s", -2000, 2000, 0,
                                      "../Effects/ColourFrameBuffer/colourframebuffer.c", 452) * 0.001000000047f;
    data->pos[1] = (f32)func_151422DC(tunable++, "pos t", -2000, 2000, 0,
                                      "../Effects/ColourFrameBuffer/colourframebuffer.c", 457) * 0.001000000047f;
    data->size[0] = (f32)func_151422DC(tunable++, "size s", 0, 2000, 500,
                                       "../Effects/ColourFrameBuffer/colourframebuffer.c", 462) * 0.001000000047f;
    data->size[1] = (f32)func_151422DC(tunable++, "size t", 0, 2000, 500,
                                       "../Effects/ColourFrameBuffer/colourframebuffer.c", 467) * 0.001000000047f;
    data->pixelation = func_151422DC(tunable, "pixelation", 0, 0x10000, 0x10000,
                                     "../Effects/ColourFrameBuffer/colourframebuffer.c", 473);
    return 1;
}

/* Sends message `event` with `data` to every screen effect. */
void func_15160274(data, event)
    s32 data;
    u8 event;
{
    const s32 kinds[1] = { OBJ_TYPE_SCREEN_EFFECT };

    func_15169260(kinds, 1, data, event);
}
