#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern void *D_8008CA4C[];

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

    obj = &movement_obj[objIndex];
    if ((obj->inwater == 0) && (-10000.0f == obj->unk118)) {
        y = obj->ypos + 5.0f;
        angle = obj->theta * 0.01745329238f;
        x = obj->xpos;
        z = obj->zpos;
        x -= 10.0f * sinf(angle);
        z -= 10.0f * cosf(angle);
        if (spread != 0) {
            rand = func_150ADA20_u8();
            x += rand % spread;
            // matching: `+ rand * 0` keeps rand live across the 2nd call (spilled to its stack slot)
            rand = func_150ADA20_u8() + rand * 0;
            z += rand % spread;
        }
        // matching: a use after the merge gives rand a stack home, stored after each call
        if (rand) {}
        if (growthRange != 0) {
            growth += func_150ADA20_u8() % (u32)growthRange;
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
        // matching: the original read the low byte of owner (lbu) and passed it on without a further u8 conversion
        func_15167D84(&template, 0, 0, -1, (s32)(u8)owner, list);
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
