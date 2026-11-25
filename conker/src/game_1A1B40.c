#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/*
 * Spawn template of a billboard particle: func_15167D84 copies these 0x38 bytes to Particle.anim
 * onwards (layout as ParticleTemplate in game_1E6B40.c).
 */
typedef struct {
    /* 0x00 */ void *anim;
    /* 0x04 */ s32 growth;          /* this file: width/height added per frame (SmokeParticle.growth) */
    /* 0x08 */ s32 fadeRate;        /* this file: alpha removed per frame (SmokeParticle.fadeRate) */
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
    /* 0x1F */ s8 callback;         /* index into D_8008C9C8, -1 = none */
    /* 0x20 */ s16 velY;
    /* 0x22 */ s16 accelY;
    /* 0x24 */ s16 width;
    /* 0x26 */ s16 height;
    /* 0x28 */ s16 life;
    /* 0x2A */ s8 fadeStep;
    /* 0x2B */ u8 onExpire;
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

/* The parts of a spawned particle that func_15174920 uses (layout as Particle in game_1944C0.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 growth;          /* copied from ParticleTemplate.growth */
    /* 0x18 */ s32 fadeRate;        /* copied from ParticleTemplate.fadeRate */
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ s16 width;
    /* 0x36 */ s16 height;
    /* 0x38 */ s16 life;
    /* 0x3A */ u8 pad3A[5];
    /* 0x3F */ u8 primA;
} SmokeParticle;

extern void *D_8008CA4C[];
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);

/*
 * Spawns a smoke puff (particle callback 5) 10 units behind object `objIndex`, unless it is in water.
 * `spread` randomises x/z by up to that much; `growth` (+ up to `growthRange` at random) and
 * `fadeRate` drive func_15174920.
 */
void func_15174690(s32 objIndex, s32 spread, s32 growth, s32 growthRange, s32 size, s32 fadeRate, s32 velY,
                   s32 owner, s32 list) {
    f32 angle;
    f32 x;
    f32 y;
    f32 z;
    s32 rand;
    struct127 *obj;
    ParticleTemplate template;

    obj = &D_800CC2D0[objIndex];
    if ((obj->in_water == 0) && (-10000.0f == obj->unk118)) {
        y = obj->y_position + 5.0f;
        angle = obj->unk40 * 0.01745329238f;
        x = obj->x_position;
        z = obj->z_position;
        x -= 10.0f * sinf(angle);
        z -= 10.0f * cosf(angle);
        if (spread != 0) {
            rand = func_150ADA20();
            x += (f32)(rand % spread);
            rand = func_150ADA20() + rand * 0;
            z += (f32)(rand % spread);
        }
        if (rand) {}
        if (growthRange != 0) {
            growth += func_150ADA20() % (u32)growthRange;
        }
        template.anim = D_8008CA4C[0];
        template.growth = growth;
        template.fadeRate = fadeRate;
        template.frame = 0;
        template.frameStep = 0x100;
        template.x = (s32)x;
        template.y = (s32)y;
        template.z = (s32)z;
        template.velX = 0;
        template.velZ = 0;
        template.size = 0;
        template.callback = 5;
        template.velY = velY;
        template.accelY = 0;
        template.width = size;
        template.height = size;
        template.life = 0x200;
        template.fadeStep = 0;
        template.onExpire = 0;
        template.primR = 0xFF;
        template.primG = 0xFF;
        template.primB = 0xFF;
        template.primA = 0xFF;
        template.flags = 0;
        func_15167D84(&template, 0, 0, -1, (u8)owner, list);
    }
}

/* Particle callback 5 (D_8008C9C8[5]): fades the puff out while it grows (height 8/7 as fast as width). */
void func_15174920(SmokeParticle *particle) {
    s32 alpha = particle->primA;

    if (alpha > 200) {
        alpha = 200;
    }
    alpha -= D_800BE9E4 * particle->fadeRate;
    if (alpha < 0) {
        particle->life = 0;
        return;
    }
    particle->primA = alpha;
    particle->width += particle->growth;
    particle->height += (particle->growth * 8) / 7;
}
