#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/*
 * Spawns a growing billboard particle (callback 9, func_1518CCA8) from `effect`. With mode 1 the
 * position comes from the float x/y/z, otherwise from the integer + fraction fields.
 */
void func_1518CA80(BillboardSpawn *effect, u8 mode)
{
    BillboardTemplate template;
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
        template.x = effect->pos.x;
        template.y = effect->pos.y;
        template.z = effect->pos.z;
        template.subX = (s32)(effect->pos.x * 256.0f) & 0xFF;
        template.subY = (s32)(effect->pos.y * 256.0f) & 0xFF;
        template.subZ = (s32)(effect->pos.z * 256.0f) & 0xFF;
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
    template.unk4 = effect->widthStep << 16;
    template.unk4 += effect->heightStep;
    template.unk2B = effect->onExpire;
    template.unk1A = 0;
    template.unk8 = 0;
    func_15167D84(&template, 0, 0, -1, 0xFF, 1);
}

/*
 * This file's .data: the billboard expiry hooks, by the low nibble of onExpire. Hook 1 is func_151927C0
 * (game_1BFC70.c), which takes the particle as its own BillboardParticle view, so the table has no prototype.
 */
static void (*D_8008D5D0[])() = { NULL, func_151927C0 };

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
            D_8008D5D0[hook](particle);
        }
    }
}
