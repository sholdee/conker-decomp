#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Effect description passed to func_1518CA80 (same layout as ParticleEffect in game_1B81D0.c). */
typedef struct {
    /* 0x00 */ f32 x;            /* position as floats (mode 1) */
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ s16 intX;         /* position as integer + 1/256ths (other modes) */
    /* 0x0E */ s16 intY;
    /* 0x10 */ s16 intZ;
    /* 0x12 */ u8 subX;
    /* 0x13 */ u8 subZ;
    /* 0x14 */ u8 subY;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ s16 width;
    /* 0x18 */ s16 height;
    /* 0x1A */ s16 widthStep;    /* added to the particle's width every frame */
    /* 0x1C */ s16 heightStep;   /* added to the particle's height every frame */
    /* 0x1E */ s16 velX;
    /* 0x20 */ s16 velZ;
    /* 0x22 */ s16 velY;
    /* 0x24 */ s16 accelY;
    /* 0x26 */ s16 life;
    /* 0x28 */ u8 primColor[3];
    /* 0x2B */ u8 envColor[3];
    /* 0x2E */ u8 intensity;     /* both colours are scaled by intensity / 256 */
    /* 0x2F */ u8 alpha;
    /* 0x30 */ s8 fadeStep;
    /* 0x31 */ u8 onExpire;      /* low nibble: index into D_8008D5D0, run when life reaches 0 */
} ParticleEffect;

/*
 * Spawn template of a billboard particle: func_15167D84 copies these 0x38 bytes to Particle.anim
 * onwards (layout as ParticleTemplate in game_1E6B40.c).
 */
typedef struct {
    /* 0x00 */ void *anim;
    /* 0x04 */ s32 sizeSteps;    /* this file: high half width step, low half height step */
    /* 0x08 */ s32 kind;
    /* 0x0C */ s16 frame;
    /* 0x0E */ s16 frameStep;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 velX;
    /* 0x18 */ s16 velZ;
    /* 0x1A */ s16 size;
    /* 0x1C */ u8 subX;
    /* 0x1D */ u8 subZ;
    /* 0x1E */ u8 subY;
    /* 0x1F */ s8 callback;      /* index into D_8008C9C8, -1 = none */
    /* 0x20 */ s16 velY;
    /* 0x22 */ s16 accelY;
    /* 0x24 */ s16 width;
    /* 0x26 */ s16 height;
    /* 0x28 */ s16 life;
    /* 0x2A */ s8 fadeStep;
    /* 0x2B */ u8 onExpire;      /* this file: see ParticleEffect.onExpire */
    /* 0x2C */ u8 primR;
    /* 0x2D */ u8 primG;
    /* 0x2E */ u8 primB;
    /* 0x2F */ u8 primA;
    /* 0x30 */ u8 envR;
    /* 0x31 */ u8 envG;
    /* 0x32 */ u8 envB;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ u16 flags;
    /* 0x36 */ u8 pad36[2];
} ParticleTemplate; /* size 0x38 */

extern void *D_8008CA4C[];
s32 func_15167D84(void *, s32, s32, s32, s32, s32);

/*
 * Spawns a growing billboard particle (callback 9, func_1518CCA8) from `effect`. With mode 1 the
 * position comes from the float x/y/z, otherwise from the integer + fraction fields.
 */
void func_1518CA80(ParticleEffect *effect, u8 mode)
{
    ParticleTemplate template;
    u8 intensity;

    intensity = effect->intensity;
    template.primR = (effect->primColor[0] * intensity) >> 8;
    template.primG = (effect->primColor[1] * intensity) >> 8;
    template.primB = (effect->primColor[2] * intensity) >> 8;
    template.envR = (effect->envColor[0] * intensity) >> 8;
    template.envG = (effect->envColor[1] * intensity) >> 8;
    template.envB = (effect->envColor[2] * intensity) >> 8;
    template.life = effect->life;
    template.primA = effect->alpha;
    template.fadeStep = effect->fadeStep;
    template.frame = 0;
    template.anim = D_8008CA4C[0];
    template.frameStep = 0x100;

    if (mode == 1) {
        template.x = effect->x;
        template.y = effect->y;
        template.z = effect->z;
        template.subX = (s32)(effect->x * 256.0f) & 0xFF;
        template.subY = (s32)(effect->y * 256.0f) & 0xFF;
        template.subZ = (s32)(effect->z * 256.0f) & 0xFF;
    } else {
        template.x = effect->intX;
        template.y = effect->intY;
        template.z = effect->intZ;
        template.subX = effect->subX;
        template.subY = effect->subY;
        template.subZ = effect->subZ;
    }

    template.velX = effect->velX;
    template.velZ = effect->velZ;
    template.velY = effect->velY;
    template.accelY = effect->accelY;
    template.callback = 9;
    template.flags = 1;
    template.width = effect->width;
    template.height = effect->height;
    template.sizeSteps = effect->widthStep << 16;
    template.sizeSteps += effect->heightStep;
    template.onExpire = effect->onExpire;
    template.size = 0;
    template.kind = 0;
    func_15167D84(&template, 0, 0, -1, 0xFF, 1);
}

/* The parts of a spawned particle that func_1518CCA8 uses (layout as Particle in game_1944C0.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 sizeSteps;    /* copied from ParticleTemplate.sizeSteps */
    /* 0x18 */ u8 pad18[0x1C];
    /* 0x34 */ s16 width;
    /* 0x36 */ s16 height;
    /* 0x38 */ s16 life;
    /* 0x3A */ s8 fadeStep;
    /* 0x3B */ u8 onExpire;
} GrowingParticle;

extern void (*D_8008D5D0[])(void);

/* Particle callback 9 (D_8008C9C8[9]): grows the particle each frame and runs its expiry hook at life 0. */
void func_1518CCA8(GrowingParticle *particle)
{
    s32 steps;
    s32 hook;

    steps = particle->sizeSteps;
    particle->width += (steps & 0xFFFF0000) >> 16;
    particle->height += steps & 0xFFFFu;
    if (particle->life == 0) {
        hook = particle->onExpire & 0xF;
        if (hook != 0) {
            D_8008D5D0[hook]();
        }
    }
}
