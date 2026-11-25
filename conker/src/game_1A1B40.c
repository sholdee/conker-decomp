#include <ultra64.h>
// matching: func_150ADA20 returns a u32 random number, but this file was built against a u8-returning
// prototype (callers use the low byte without masking it), so it keeps that view.
#define func_150ADA20 func_150ADA20_functions_h
#include "functions.h"
#undef func_150ADA20
u8 func_150ADA20(void);
#include "variables.h"
#include "effects.h"


/* The parts of a spawned particle that func_15174920 uses (layout as Particle in game_1944C0.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 growth;          /* copied from BillboardTemplate.unk4 */
    /* 0x18 */ s32 fadeRate;        /* copied from BillboardTemplate.unk8 */
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ s16 width;
    /* 0x36 */ s16 height;
    /* 0x38 */ s16 life;
    /* 0x3A */ u8 pad3A[5];
    /* 0x3F */ u8 primA;
} PuffParticle;

extern void *D_8008CA4C[];
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);

/*
 * Spawns a white, growing and fading puff (particle callback 5) 10 units behind object `objIndex`, unless it is in water.
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
    Actor *obj;
    BillboardTemplate template;

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
            x += rand % spread;
            // matching: `+ rand * 0` keeps rand live across the 2nd call (spilled to its stack slot)
            rand = func_150ADA20() + rand * 0;
            z += rand % spread;
        }
        // matching: a use after the merge gives rand a stack home, stored after each call
        if (rand) {}
        if (growthRange != 0) {
            growth += func_150ADA20() % (u32)growthRange;
        }
        template.anim = D_8008CA4C[0];
        template.unk4 = growth;
        template.unk8 = fadeRate;
        template.frame = 0;
        template.frameStep = 0x100;
        template.x = x;
        template.y = y;
        template.z = z;
        template.velX = 0;
        template.velZ = 0;
        template.unk1A = 0;
        template.callback = 5;
        template.velY = velY;
        template.accelY = 0;
        template.width = size;
        template.height = size;
        template.life = 0x200;
        template.fadeStep = 0;
        template.unk2B = 0;
        template.primR = 0xFF;
        template.primG = 0xFF;
        template.primB = 0xFF;
        template.primA = 0xFF;
        template.flags = 0;
        func_15167D84(&template, 0, 0, -1, (u8)owner, list);
    }
}

/* Particle callback 5 (D_8008C9C8[5]): fades the puff out while it grows (height 8/7 as fast as width). */
void func_15174920(PuffParticle *particle) {
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
