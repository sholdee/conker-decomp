#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 sinf(f32);

void func_150D278C(s32, void*, u8, s32);

struct Func150D317CSub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x8];
    f32 unk28;
    f32 unk2C;
};

struct Func150D317CObj {
    u8 pad0[0x5C];
    u8 unk5C;
    u8 pad5D[0xCB];
    f32 unk128;
    f32 unk12C;
    f32 unk130;
    f32 unk134;
};

struct S150D26F0 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 unk10;
};

void func_150D26F0(u8 *arg0) {
    struct S150D26F0 *v1 = (struct S150D26F0 *)(arg0 + 0x28);

    if (arg0[0x78] & 1) {
        v1->unkC = v1->unkC - D_800BE9E4;
        if (v1->unkC < 0) {
            func_150D278C(v1->unk0, &v1->unk10, arg0[0xC], arg0[1]);
            v1->unkC = func_150ADA20() % (u32)(v1->unk8 + 1) + v1->unk4;
        }
    }
}

typedef struct {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 unk1C[0x40];
    u8 unk5C[3];
} S150D278C;

void func_150D278C(s32 arg0, void *arg1, u8 arg2, s32 arg3) {
    S150D278C sp48;
    struct260 *temp_v0;

    sp48.unk0 = arg0;
    sp48.unk4 = func_150ADA68() * 2.0f * 3.141592741f;
    sp48.unk8 = (func_150ADA68() * 0.400000006f) + 0.3000000119f;
    sp48.unkC = 1.0f / sp48.unk8;
    sp48.unk10 = (func_150ADA68() * 0.1300000101f) + 0.3000000119f;
    if (func_150ADA20() & 1) {
        sp48.unk10 = -sp48.unk10;
    }
    sp48.unk14 = 0.0f;
    sp48.unk18 = 0.0f;
    memcpy(sp48.unk1C, arg1, 0x40);
    {
        const u8 colors[3][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0x00, 0x00 }, { 0x00, 0xFF, 0x00 } };
        u8 i = func_150ADA20() % 3U;
        sp48.unk5C[0] = colors[i][0];
        sp48.unk5C[1] = colors[i][1];
        sp48.unk5C[2] = colors[i][2];
    }
    temp_v0 = func_15149130((func_150ADA20() % 101U) + 100, -1, 0x32, -1, 1, 0, (struct37 *)0x60, arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp48, 0x60);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_FFBA0/func_150D2924.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_FFBA0/func_150D2D6C.s")

s32 func_150D317C(struct Func150D317CObj *arg0) {
    struct Func150D317CSub *sub;
    f32 *step;

    step = &D_800BE9A4;
    arg0->unk128 += arg0->unk130 * *step;
    arg0->unk12C += arg0->unk134 * *step;
    sub = (struct Func150D317CSub *)((u8 *)arg0 + 0x110);
    sub->unk18 = func_15144B68(arg0->unk128);
    sub->unk1C = func_15144B68(sub->unk1C);
    sub->unk0 = (sinf(sub->unk18) * sub->unk10) + sub->unk8;
    sub->unk4 = (sinf(sub->unk1C) * sub->unk14) + sub->unkC;
    sub->unk28 -= D_800BE9A4;
    arg0->unk5C = sub->unk2C * sub->unk28;
    if (sub->unk28 > 0.0f) {
        return 1;
    }
    return 0;
}

void func_150D278C(s32, void*, u8, s32);

struct S150D32FC {
    s32 unk0;
    u8 pad4[0xC];
    u8 unk10;
    u8 pad11[0x40];
    u8 unk51;
};

void func_150D32FC(u8 *arg0, u8 *arg1, u8 arg2) {
    struct S150D32FC *v0 = (struct S150D32FC *)(arg0 + 0x28);

    if (arg2 == 0x34 && arg1[0] == ((u8*)v0)[0x51]) {
        func_150D278C(v0->unk0, &v0->unk10, arg0[0xC], arg0[1]);
    }
}
