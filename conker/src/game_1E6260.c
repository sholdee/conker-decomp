#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_1E6260/func_151B8DB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E6260/func_151B9214.s")

void func_150A8050(f32 (*)[4], f32, f32, f32);

s32 func_151B9310(s32 *arg0, s32 *arg1) {
    f32 m[18];

    func_150A8050((f32 (*)[4])&m[2], 0.0f, 0.0f, *(f32 *)((u8 *)arg1 + 0x170));

    m[14] = *(f32 *)((u8 *)arg1 + 0x38);
    m[15] = *(f32 *)((u8 *)arg1 + 0x3C);
    m[16] = *(f32 *)((u8 *)arg1 + 0x40);

    m[2] = *(f32 *)((u8 *)arg1 + 0x18) * m[2];
    m[3] = *(f32 *)((u8 *)arg1 + 0x18) * m[3];
    m[4] = *(f32 *)((u8 *)arg1 + 0x18) * m[4];

    m[6] = *(f32 *)((u8 *)arg1 + 0x1C) * m[6];
    m[7] = *(f32 *)((u8 *)arg1 + 0x1C) * m[7];
    m[8] = *(f32 *)((u8 *)arg1 + 0x1C) * m[8];

    m[10] = *(f32 *)((u8 *)arg1 + 0x18) * m[10];
    m[11] = *(f32 *)((u8 *)arg1 + 0x18) * m[11];
    m[12] = *(f32 *)((u8 *)arg1 + 0x18) * m[12];

    guMtxF2L((f32 (*)[4])&m[2], (Mtx *)arg0);
    return 1;
}

void func_150A7960(void *, f32, f32, f32, f32 *, f32 *, f32 *);
extern f32 D_800AA568;

typedef struct Game1E67C0State {
    u8 *source;
    u8 *target;
    f32 rate;
} Game1E67C0State;

s32 func_151B9408(u8 *actor) {
    f32 extend;
    u8 *source;
    struct {
        f32 matrix[16];
        f32 x, y, z;
    } work;
    f32 reach;
    f32 rate;
    register f32 dx, dy, dz;
    Game1E67C0State *state;
    u8 *target;

    state = (Game1E67C0State *)(actor + 0x18);
    source = state->source;
    func_150A8050((f32 (*)[4])work.matrix, 0.0f, 0.0f, *(f32 *)(source + 0x170));
    work.matrix[12] = *(f32 *)(source + 0x38);
    work.matrix[13] = *(f32 *)(source + 0x3C);
    work.matrix[14] = *(f32 *)(source + 0x40);
    work.matrix[0] *= *(f32 *)(source + 0x18);
    work.matrix[1] *= *(f32 *)(source + 0x18);
    work.matrix[2] *= *(f32 *)(source + 0x18);
    work.matrix[4] *= *(f32 *)(source + 0x1C);
    work.matrix[5] *= *(f32 *)(source + 0x1C);
    work.matrix[6] *= *(f32 *)(source + 0x1C);
    work.matrix[8] *= *(f32 *)(source + 0x18);
    work.matrix[9] *= *(f32 *)(source + 0x18);
    work.matrix[10] *= *(f32 *)(source + 0x18);
    func_150A7960(work.matrix, 0.0f, -150.0f, 0.0f, &work.x, &work.y, &work.z);
    *(s16 *)(*(u8 **)(actor + 0x14) + 0xE) = (s16)(s32)work.x;
    *(s16 *)(*(u8 **)(actor + 0x14) + 0x10) = (s16)(s32)work.y;
    *(s16 *)(*(u8 **)(actor + 0x14) + 0x12) = (s16)(s32)work.z;
    target = state->target;
    if (target != 0) {
        extend = 500.0f;
        reach = *(f32 *)(source + 0x1C) * 65.0f;
        rate = state->rate * D_800AA568;
        dx = -(work.x - *(f32 *)(source + 0x38)) * rate;
        dy = -(work.y - *(f32 *)(source + 0x3C)) * rate;
        dz = -(work.z - *(f32 *)(source + 0x40)) * rate;
        *(f32 *)(target + 0x34) = (f32)(*(f32 *)(source + 0x38) + dx * reach);
        *(f32 *)(state->target + 0x38) = (f32)(*(f32 *)(source + 0x3C) + dy * reach);
        *(f32 *)(state->target + 0x3C) = (f32)(*(f32 *)(source + 0x40) + dz * reach);
        target = state->target;
        *(f32 *)(target + 0x40) = (f32)(*(f32 *)(target + 0x34) + dx * extend);
        target = state->target;
        *(f32 *)(target + 0x44) = (f32)(*(f32 *)(target + 0x38) + dy * extend);
        target = state->target;
        *(f32 *)(target + 0x48) = (f32)(*(f32 *)(target + 0x3C) + dz * extend);
    }
    return 1;
}

extern f32 D_800AA56C;
extern f32 D_800AA570;

void func_151B9660(s32 *arg0) {
    *(f32 *)((u8 *)arg0 + 0x188) = 5.0f;
    *(f32 *)((u8 *)arg0 + 0x18C) = D_800AA56C;
    *(f32 *)((u8 *)arg0 + 0x190) = D_800AA570;
}
