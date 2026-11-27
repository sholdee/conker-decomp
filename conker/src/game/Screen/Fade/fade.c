#include <ultra64.h>
#include "functions.h"
#include "variables.h"



/* Spawns a fade effect for owner/source; mode picks how it triggers. */
void screen_spawnFade(s32 mode, Actor *owner, Actor *source) {
    FadeEffect *effect;

    effect = effect_newObject(100, 0, 0x20, 0, 0xFF, 1);
    if (effect != NULL) {
        effect->timer = 15;
        effect->state = 0;
        effect->alpha = 0;
        effect->owner = owner;
        effect->source = source;
        effect->mode = mode;
    }
}

/* Fade effect update: wait for the trigger, fade in, hold for 180 frames, fade out and free itself. */
void screen_updateFade(FadeEffect *effect) {
    s32 triggered;
    s32 mode;
    AnimInstance *anim;
    f32 frame;

    switch (effect->state) {
    case 0:
        anim = effect->owner->unk2D0;
        frame = anim->frame[0];
        mode = effect->mode;
        triggered = FALSE;
        if (mode == 0) {
            if (frame >= 19.0f) {
                triggered = TRUE;
            }
        } else if (mode == 1) {
            if (gFrameTicks < effect->timer) {
                effect->timer -= gFrameTicks;
            } else {
                triggered = TRUE;
            }
        }
        if (triggered) {
            Actor *source;
            Actor *owner;

            source = effect->source;
            owner = effect->owner;
            effect->state = 1;
            gFrameRateDivider = 1;
            func_100176C4();
            sound_play(0x5B2, 0x7FFF, 0, 0, 0);
            sound_play(0x5B2, 0x7FFF, 127, -100, 0);
            sound_play(0x5B2, 0x7FFF, 0, 0, 0);
            sound_play(0x5B2, 0x7FFF, 127, -100, 0);
            screen_setFrameEcho(210, 2);
            camera_attachToObject(owner->CameraPtr, source->listpos);
            owner->CameraPtr->CharTrackingSpeedTarget = 0.0f;
        }
        break;
    case 1: {
        s32 alpha;

        alpha = effect->alpha;
        alpha += gRawFrameTicks * 10;
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
        Actor *owner;

        if (gRawFrameTicks < effect->timer) {
            effect->timer -= gRawFrameTicks;
        } else {
            owner = effect->owner;
            gFrameRateDivider = 0;
            music_pauseAll(0);
            camera_attachToObject(owner->CameraPtr, owner->listpos);
            owner->CameraPtr->CharTrackingSpeedTarget = 0.0f;
            effect->state = 3;
        }
    }
        break;
    case 3: {
        s32 alpha;

        alpha = effect->alpha;
        alpha -= gRawFrameTicks * 10;
        if (alpha <= 0) {
            effect_destroyObject(&effect->header);
            screen_setFrameEcho(0, 0);
        } else {
            screen_setFrameEcho((alpha * 210) >> 8, 2);
            effect->alpha = alpha;
        }
    }
        break;
    }
}

/* Fade effect draw hook: dims view 0 to black in proportion to the effect's alpha. */
Gfx *screen_drawFade(Gfx *gfx, FadeEffect *effect, s16 view) {
    FadeEffect *fade = effect; // matching: the param copy makes IDO home effect to its stack slot

    if (view != 0 || fade->alpha == 0) {
        return gfx;
    }
    return screen_fillView(gfx, (fade->alpha * 63) >> 8, 0, 0, 0, view);
}

/* Returns player 0's mv->matrix_physics, or 0 when it has no mv. */
u8 screen_getPlayer0MatrixPhysics(void) {
    if (movement_obj[0].mv != NULL) {
        return movement_obj[0].mv->matrix_physics;
    }
    return 0;
}
