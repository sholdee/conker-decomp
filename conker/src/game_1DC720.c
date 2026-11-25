#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

void func_1516962C(s32 type, Actor *actor, u8 eventType);
void *func_1513418C(BloodDripDesc *desc, s32 arg1, u8 arg2, s32 arg3);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);
void func_151AF388(f32 *position, f32 dx, f32 dy, f32 dz, s32 effectUnkC);

/* Attaches emitter 10 (func_151AF338) to bone 1 of `actor` for 320 frames. */
void *func_151AF270(Actor *actor, u8 arg1, s32 arg2) {
    void *result;
    BloodDripDesc desc;

    if (actor == NULL) {
        return NULL;
    }
    func_1516962C(40, actor, 22);
    desc.target[0] = 0;
    desc.target[1] = 0;
    desc.ownerId = actor->unique_id;
    desc.owner = actor;
    desc.bone = 1;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.radius = 9.800000191f;
    desc.rate = 0.1430000067f;
    desc.flags = 6; /* ON_BONE | LIFETIME (see game_161520.c) */
    desc.lifetime = 320;
    desc.emitter = 10;
    desc.callback = -1;
    desc.unk2D = 9;
    result = func_1513418C(&desc, 0, arg1, arg2);
    return result;
}

/* Emitter callback 10 (D_80089A70): spawns one particle at (x, y, z) pushed against the
 * emitter's movement (dx, dy, dz). */
void func_151AF338(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect) {
    f32 position[3];

    position[0] = x;
    position[1] = y;
    position[2] = z;
    func_151AF388(position, dx, dy, dz, effect->owner);
}

/* Spawns one randomised func_151D9014 particle at `position`, moving at -0.13 * (dx, dy, dz). */
void func_151AF388(f32 *position, f32 dx, f32 dy, f32 dz, s32 effectUnkC) {
    f32 factor = 0.13000001f;
    f32 velocity[3];
    u8 flag;

    velocity[0] = -dx * factor;
    velocity[1] = -dy * factor;
    velocity[2] = -dz * factor;
    flag = (func_150ADA20() & 1) ? 1 : 0;
    func_151D9014(position, velocity, 0,
                  func_150ADA68() * 0.126f + -0.702f,
                  (func_150ADA20() & 0xF) + 10,
                  func_150ADA20() % 45 + 137,
                  func_150ADA68() * 202.0f + 142.0f,
                  flag, 1.7f, 1.7f, 0, 0, 1, 0,
                  (u8)effectUnkC, 1);
}
