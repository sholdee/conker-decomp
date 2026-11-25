#include <ultra64.h>
#include "functions.h"
#include "variables.h"


// Screen-flash effect spawned between two objects (see func_15104170 callers in game_1C1150.c).
typedef struct {
    u8         pad0[0x10];
    struct127 *owner;  // 0x10
    struct127 *source; // 0x14
    u16        timer;  // 0x18
    u8         state;  // 0x1A
    u8         alpha;  // 0x1B
    u8         mode;   // 0x1C: 0 = trigger on owner's animation frame, 1 = trigger after timer
} FlashEffect;

void *func_15167A68(s32, s32, s32, s32, s32, s32);

/* Spawns a flash effect for owner/source; mode picks how it triggers. */
void func_15104170(s32 mode, struct127 *owner, struct127 *source) {
    FlashEffect *effect;

    effect = func_15167A68(100, 0, 0x20, 0, 0xFF, 1);
    if (effect != NULL) {
        effect->timer = 15;
        effect->state = 0;
        effect->alpha = 0;
        effect->owner = owner;
        effect->source = source;
        effect->mode = mode;
    }
}

extern u8 D_800BEA0C;
void func_1000E2F4(s32 arg0);
void func_151254F4(struct108 *camera, s32 arg1);
void func_151D66F0(s32 arg0, s32 arg1);

/* Flash effect update: wait for trigger, fade in, hold, fade out and free. */
void func_151041E4(FlashEffect *effect) {
    s32 triggered;
    s32 mode;
    struct197 *anim;
    f32 frame;

    switch (effect->state) {
    case 0:
        anim = effect->owner->unk2D0;
        frame = anim->unk8;
        mode = effect->mode;
        triggered = FALSE;
        if (mode == 0) {
            if (frame >= 19.0f) {
                triggered = TRUE;
            }
        } else if (mode == 1) {
            if (D_800BE9E4 < effect->timer) {
                effect->timer -= D_800BE9E4;
            } else {
                triggered = TRUE;
            }
        }
        if (triggered) {
            struct127 *source;
            struct127 *owner;

            source = effect->source;
            owner = effect->owner;
            effect->state = 1;
            D_800BEA0C = 1;
            func_100176C4();
            func_10010F30(0x5B2, 0x7FFF, 0, 0, 0);
            func_10010F30(0x5B2, 0x7FFF, 127, -100, 0);
            func_10010F30(0x5B2, 0x7FFF, 0, 0, 0);
            func_10010F30(0x5B2, 0x7FFF, 127, -100, 0);
            func_151D66F0(210, 2);
            func_151254F4(owner->camera, source->unk13F);
            owner->camera->unk674 = 0.0f;
        }
        break;
    case 1: {
        s32 alpha;

        alpha = effect->alpha;
        alpha += D_800BEA08 * 10;
        if (alpha >= 255) {
            effect->alpha = 255;
            effect->state = 2;
            effect->timer = 180;
        } else {
            effect->alpha = alpha;
        }
    }
        break;
    case 2: {
        struct127 *owner;

        if (D_800BEA08 < effect->timer) {
            effect->timer -= D_800BEA08;
        } else {
            owner = effect->owner;
            D_800BEA0C = 0;
            func_1000E2F4(0);
            func_151254F4(owner->camera, owner->unk13F);
            owner->camera->unk674 = 0.0f;
            effect->state = 3;
        }
    }
        break;
    case 3: {
        s32 alpha;

        alpha = effect->alpha;
        alpha -= D_800BEA08 * 10;
        if (alpha <= 0) {
            func_1516972C((struct102 *)effect);
            func_151D66F0(0, 0);
        } else {
            func_151D66F0((alpha * 210) >> 8, 2);
            effect->alpha = alpha;
        }
    }
        break;
    }
}

s32 func_1517F08C(s32, s32, s32, s32, s32, s32);

s32 func_1510448C(s32 arg0, s32 arg1, s32 arg2) {
    s32 *p1 = &arg1;
    s32 *p2 = &arg2;
    s32 v0;
    if ((s16)*p2 != 0 || (v0 = *(u8 *)(*p1 + 0x1B)) == 0) {
        return arg0;
    }
    return func_1517F08C(arg0, ((v0 * 0x3F) >> 8), 0, 0, 0, *((s16 *)&arg2 + 1));
}

extern struct126 *D_800CC5EC;

u8 func_151044F4(void) {
    if (D_800CC5EC != 0) {
        return D_800CC5EC->matrix_physics;
    }
    return 0;
}
