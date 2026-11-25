#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* A four-oscillator wobble driven by an attack/decay/sustain/release envelope, one per player view. */
typedef struct {
    /* 0x00 */ f32 frequency0;     /* oscillator frequencies, in cycles per 30 frames */
    /* 0x04 */ f32 frequency1;     // matching: an f32[4] array here changes FP regalloc in func_1512D980
    /* 0x08 */ f32 frequency2;
    /* 0x0C */ f32 frequency3;
    /* 0x10 */ f32 attackTime;
    /* 0x14 */ f32 decayTime;
    /* 0x18 */ f32 sustainTime;
    /* 0x1C */ f32 releaseTime;
    /* 0x20 */ f32 peak;
    /* 0x24 */ f32 sustainLevel;
    /* 0x28 */ f32 elapsed;       /* -1.0f when idle */
    /* 0x2C */ f32 amplitude;
    /* 0x30 */ f32 attackEnd;
    /* 0x34 */ f32 decayEnd;
    /* 0x38 */ f32 sustainEnd;
    /* 0x3C */ f32 releaseEnd;
    /* 0x40 */ f32 attackRate;
    /* 0x44 */ f32 decayRate;
    /* 0x48 */ f32 sustainRate;
    /* 0x4C */ f32 releaseRate;
    /* 0x50 */ s32 stage;         /* 0 = loaded, 1..4 = attack..release, 5 = finished */
    /* 0x54 */ f32 output[4];
    /* 0x64 */ s32 mode;          /* nonzero: stage advances as the envelope runs */
} ViewEnvelope;

extern ViewEnvelope D_800DC2C0[];

/* Stops the view's envelope and zeroes its outputs. */
void func_1512D6F0(struct108 *view) {
    ViewEnvelope *env = &D_800DC2C0[view->unk23D];

    env->stage = 5;
    env->output[0] = 0.0f;
    env->output[1] = 0.0f;
    env->output[2] = 0.0f;
    env->output[3] = 0.0f;
    env->amplitude = 0.0f;
    env->elapsed = -1.0f;
}

extern u8 D_800895D0[][10];

/* Loads envelope preset presetIndex into the view's envelope and restarts it from the attack stage. */
/* Loads envelope preset presetIndex into the view's envelope and restarts it from the attack stage. */
void func_1512D748(struct108 *view, s32 presetIndex, s32 mode) {
    ViewEnvelope *env = &D_800DC2C0[view->unk23D];
    u8 *preset;
    volatile f32 sustainLevel;
    f32 attackTime;
    f32 decayTime;
    f32 peak;
    f32 releaseTime;
    f32 decayEnd;
    f32 sustainEnd;

    if (view->unk3D4->unk120 == 0) {
        preset = D_800895D0[presetIndex];
        env->frequency0 = (u32)preset[0];
        env->frequency1 = (u32)preset[1];
        env->frequency2 = (u32)preset[2];
        env->frequency3 = (u32)preset[3];
        env->attackTime = (u32)preset[4];
        env->decayTime = (u32)preset[5];
        env->sustainTime = (u32)preset[6];
        env->releaseTime = (u32)preset[7];
        env->peak = (u32)preset[8];
        env->sustainLevel = (u32)preset[9];
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
void func_1512D980(struct108 *view) {
    ViewEnvelope *env = &D_800DC2C0[view->unk23D];
    f32 sign;

    if (func_150ADA20() & 1) {
        sign = 1.0f;
    } else {
        sign = -1.0f;
    }
    if (env->elapsed != -1.0f && env->stage != 5 && !(view->unk84 & 0x80000)) {
        env->output[0] = sinf(env->elapsed * 0.03333333507f * env->frequency0 * 360.0f * 0.01745329238f) * (sign * env->amplitude);
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
            env->elapsed += (u32)D_800BE9A0;
        } else {
            env->stage = 5;
        }
    }
}
