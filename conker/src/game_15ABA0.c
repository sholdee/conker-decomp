#include <ultra64.h>
#include "structs.h"

/* This file's .bss. */
ViewEnvelope D_800DC2C0[4]; // per-view camera wobble envelope

#include "functions.h"
#include "variables.h"


/* Stops the view's envelope and zeroes its outputs. */
void func_1512D6F0(Camera *view) {
    ViewEnvelope *env = &D_800DC2C0[view->CNumber];

    env->stage = 5;
    env->output[0] = 0.0f;
    env->output[1] = 0.0f;
    env->output[2] = 0.0f;
    env->output[3] = 0.0f;
    env->amplitude = 0.0f;
    env->elapsed = -1.0f;
}

/* Envelope presets: four oscillator frequencies, attack, decay, sustain and release times, peak, sustain level. */
static u8 D_800895D0[9][10] = {
    { 5, 5, 5, 5, 30, 20, 30, 30, 7, 3 },
    { 3, 4, 0, 0, 13, 5, 7, 14, 3, 5 },
    { 9, 4, 7, 0, 13, 5, 7, 14, 3, 5 },
    { 9, 4, 7, 0, 13, 5, 7, 14, 3, 5 },
    { 0, 12, 13, 3, 1, 1, 1, 1, 4, 1 },
    { 3, 4, 0, 0, 13, 5, 7, 14, 3, 5 },
    { 9, 4, 7, 0, 13, 5, 7, 14, 3, 5 },
    { 9, 4, 7, 0, 13, 5, 7, 14, 3, 5 },
    { 0, 12, 13, 3, 1, 1, 1, 1, 4, 1 },
};

/* Loads envelope preset presetIndex into the view's envelope and restarts it from the attack stage. */
/* Loads envelope preset presetIndex into the view's envelope and restarts it from the attack stage. */
void func_1512D748(Camera *view, s32 presetIndex, s32 mode) {
    ViewEnvelope *env = &D_800DC2C0[view->CNumber];
    u8 *preset;
    f32 sustainLevel;
    f32 attackTime;
    f32 decayTime;
    f32 peak;
    f32 releaseTime;
    f32 decayEnd;
    f32 sustainEnd;

    if (view->mv->death_State == 0) {
        preset = D_800895D0[presetIndex];
        env->frequency0 = preset[0];
        env->frequency1 = preset[1];
        env->frequency2 = preset[2];
        env->frequency3 = preset[3];
        env->attackTime = preset[4];
        env->decayTime = preset[5];
        env->sustainTime = preset[6];
        env->releaseTime = preset[7];
        env->peak = preset[8];
        env->sustainLevel = preset[9];
        attackTime = env->attackTime;
        decayTime = env->decayTime;
        decayEnd = decayTime + attackTime;
        sustainEnd = env->sustainTime + decayEnd;
        releaseTime = env->releaseTime;
        peak = env->peak;
        env->attackEnd = attackTime;
        env->decayEnd = decayEnd;
        env->sustainEnd = sustainEnd;
        env->releaseEnd = releaseTime + sustainEnd;
        env->elapsed = 0.0f;
        env->amplitude = 0.0f;
        env->attackRate = peak / attackTime;
        sustainLevel = env->sustainLevel;
        env->decayRate = (peak - sustainLevel) / decayTime;
        env->sustainRate = 0.0f;
        env->releaseRate = sustainLevel / releaseTime;
        env->stage = 0;
        env->mode = mode;
    }
}

/* Advances the view's envelope by one frame and updates its four oscillator outputs. */
void func_1512D980(Camera *view) {
    ViewEnvelope *env = &D_800DC2C0[view->CNumber];
    f32 sign;

    sign = (func_150ADA20() & 1) ? 1.0f : -1.0f;
    if (env->elapsed != -1.0f && env->stage != 5 && !(view->AvailableModes & 0x80000)) {
        env->output[0] = sinf(env->elapsed * 0.03333333507f * env->frequency0 * 360.0f * 0.01745329238f) * (sign * env->amplitude);
        // matching: the integer `1 *` (not 1.0f, not omitted) is what IDO compiled
        env->output[1] = sinf(env->elapsed * 0.03333333507f * env->frequency1 * 360.0f * 0.01745329238f) * (1 * env->amplitude);
        env->output[2] = sinf(env->elapsed * 0.03333333507f * env->frequency2 * 360.0f * 0.01745329238f) * (-1.0f * env->amplitude);
        env->output[3] = sinf(env->elapsed * 0.03333333507f * env->frequency3 * 360.0f * 0.01745329238f) * (-1.0f * env->amplitude);
        if (env->elapsed <= env->releaseEnd && env->amplitude >= 0.0f) {
            if (env->elapsed <= env->attackEnd) {
                if (env->stage == 0 && env->mode != 0) {
                    env->stage = 1;
                }
                env->amplitude += env->attackRate * D_800BE9A4;
            } else if (env->elapsed <= env->decayEnd) {
                if (env->stage == 1 && env->mode != 0) {
                    env->stage = 2;
                }
                env->amplitude -= env->decayRate * D_800BE9A4;
            } else if (env->elapsed <= env->sustainEnd) {
                if (env->stage == 2 && env->mode != 0) {
                    env->stage = 3;
                }
            } else if (env->elapsed < env->releaseEnd) {
                if (env->stage == 3 && env->mode != 0) {
                    env->stage = 4;
                }
                env->amplitude -= env->releaseRate * D_800BE9A4;
            } else {
                func_1512D6F0(view);
            }
            env->elapsed += D_800BE9A0;
        } else {
            env->stage = 5;
        }
    }
}
