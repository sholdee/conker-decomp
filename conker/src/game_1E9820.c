#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Drip emitter callback (D_80089A70[39]): spawns one particle at (x, y, z), flung in a random
 * direction and pushed against the drip's movement (dx, dy, dz). The func_151A26EC arguments
 * after velocity are drag, gravity, size, timer, alpha, fade timings and an (r, g, b) of (203, 0, 0). */
void func_151BC370(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, EffectHeader *effect) {
    Vec3f position;
    Vec3f velocity;
    Vec3f offset;
    f32 scale;
    f32 fraction_b;
    u32 random_b;
    u32 random_a;
    f32 fraction_a;

    position.x = x;
    position.y = y;
    position.z = z;
    random_a = func_150ADA20();
    random_b = func_150ADA20();
    fraction_a = func_150ADA68();
    func_15143794(random_a & 0xFF, random_b % 65U - 32, fraction_a * 1680.0f * 0.01f, &velocity);
    scale = (func_150ADA68() * 202.0f + 208.0f) * 0.001f;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;
    velocity.x += -dx * D_800BE9A8 * scale;
    velocity.y += -dy * D_800BE9A8 * scale;
    velocity.z += -dz * D_800BE9A8 * scale;
    fraction_a = func_150ADA68();
    fraction_b = func_150ADA68();
    random_a = func_150ADA20();
    random_b = func_150ADA20();
    func_151A26EC(&position, &offset, &velocity, 1.0f, (fraction_a * 1049.0f + -1022.0f) * 0.001f,
                  fraction_b * 124.0f + 202.0f, (random_a & 0xF) + 20, random_b % 201U + 55, 15, 15, 0, -1,
                  203, 0, 0, effect->owner, effect->kind);
}
