#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct Game1F1CD0Emission {
    /* 0x00 */ f32 x, y;
    /* 0x08 */ f32 amplitudeX, amplitudeY;
    /* 0x10 */ f32 phaseX, phaseY;
    /* 0x18 */ f32 speedX, speedY;
    /* 0x20 */ f32 time, rate;
    /* 0x28 */ f32 remaining;
    /* 0x2C */ Vec3f position;
    /* 0x38 */ f32 scaleX, scaleY, parameter;
} Game1F1CD0Emission;

typedef struct Game1F1CD0Actor {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ Game1F1CD0Emission emission;
} Game1F1CD0Actor;

extern f32 sinf(f32);
extern void func_151436B4(f32, f32, f32, Vec3f *);
extern void *func_151C229C(Vec3f *, Vec3f *, s32, Vec3f *, u8, s32, f32, f32, f32, f32, f32, u8, u8 *, u8, u8,
                           u8, u8, u8, u8, s8, s32, f32, u8, s8, s32, u8, s32);

/* Emits this frame's particles along a sine-wobbled direction and advances the wobble phases. */
void func_151C4820(Game1F1CD0Actor *actor) {
    Game1F1CD0Emission *emission;
    f32 inverse;
    f32 time;
    f32 timeStep;
    f32 speedX;
    f32 speedY;
    f32 phaseX;
    f32 phaseY;
    f32 phaseStepX;
    f32 phaseStepY;
    Vec3f direction;

    emission = &actor->emission;
    emission->remaining += emission->rate * D_800BE9A4;
    if (emission->remaining > 1.0f) {
        inverse = 1.0f / emission->remaining;
        time = emission->time + D_800BE9A4;
        timeStep = time * inverse;
        speedX = emission->speedX * D_800BE9A4;
        speedY = emission->speedY * D_800BE9A4;
        phaseX = emission->phaseX;
        phaseY = emission->phaseY;
        phaseStepX = speedX * inverse;
        phaseStepY = speedY * inverse;
        do {
            f32 sineX;
            f32 sineY;
            f32 x;
            f32 y;
            f32 randomX;
            f32 randomY;

            sineX = sinf(phaseX);
            sineY = sinf(phaseY);
            x = emission->x + sineX * emission->amplitudeX;
            y = emission->y + sineY * emission->amplitudeY;
            func_151436B4(x, y, 5.0f, &direction);
            randomX = func_150ADA68();
            randomY = func_150ADA68();
            func_151C229C(&emission->position, &direction, 0, NULL, 0, 0, emission->scaleX, emission->scaleY,
                          randomX * 40.0f + 55.0f, randomY * 300.0f + 250.0f, emission->parameter,
                          func_150ADA20() % 56U + 200, NULL, 0, 0, 0, 0, 0, 0, -1, 0, time, 255, -1, 0, actor->owner,
                          actor->kind);
            phaseX += phaseStepX;
            phaseY += phaseStepY;
            time -= timeStep;
            emission->remaining -= 1.0f;
        } while (emission->remaining > 1.0f);
        emission->phaseX = func_15144B68(phaseX);
        emission->phaseY = func_15144B68(phaseY);
        emission->time = time;
    }
}
