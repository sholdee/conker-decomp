#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_1513470C(struct102 *arg0);

typedef struct {
    /* 0x00 */ s32 field_0x00;
    /* 0x04 */ u8 field_0x04;
    /* 0x05 */ u8 pad_0x05[0x3];
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ s16 field_0x14;
    /* 0x16 */ s16 field_0x16;
    /* 0x18 */ s16 field_0x18;
    /* 0x1A */ s16 field_0x1A;
    /* 0x1C */ f32 field_0x1C;
} RandomLerpState;

typedef struct {
    /* 0x00 */ char pad_0x00[0x24];
    /* 0x24 */ s32 field_0x24;
    /* 0x28 */ char pad_0x28[0x88];
    /* 0xB0 */ RandomLerpState field_0xB0;
} RandomLerpObject;

typedef struct {
    /* 0x00 */ char pad_0x00[0x6C];
    /* 0x6C */ RandomLerpObject *volatile field_0x6C;
} RandomLerpOwner;

RandomLerpObject *func_150C6D90(RandomLerpOwner *arg0);

void func_150C66F0(RandomLerpOwner *arg0) {
    RandomLerpState *temp;

    if (arg0->field_0x6C != 0) {
        temp = &arg0->field_0x6C->field_0xB0;
        temp->field_0x04 = 1;
        done:
            ;
    } else {
        arg0->field_0x6C = func_150C6D90(arg0);
    }
}

s32 func_150C673C(RandomLerpObject *arg0) {
    s8 ret;
    RandomLerpState *sub;

    ret = 1;
    sub = &arg0->field_0xB0;
    if (sub->field_0x04 == 0) {
        ret = 0;
    }
    sub->field_0x04 = 0;
    sub->field_0x14 -= D_800BE9E4;
    if (sub->field_0x14 < 0) {
        sub->field_0x14 = (func_150ADA20() % (u32)(sub->field_0x18 + 1)) + sub->field_0x16;
        sub->field_0x10 = func_150ADA68() * sub->field_0x0C + sub->field_0x08;
    }
    arg0->field_0x24 += (s32)((sub->field_0x10 - (f32)arg0->field_0x24) * sub->field_0x1C);
    return ret;
}

extern s32 func_1513F6C0(void *arg0, s32 arg1, s32 arg2);

s32 func_150C682C(u8 *arg0) {
    s32 *temp = (s32 *)((*(u8 **)((u8 *)arg0 + 0xB0)) + 0x58);
    temp[5] = 0;
    *(s32 *)((u8 *)arg0 + 0x18) |= 2;
    *(u8 **)((u8 *)arg0 + 0xB0) = 0;
    func_1513F6C0(arg0, 0, 0);
    return 0;
}

s32 func_150C68C4(s32 arg0);

void func_150C6870(u8 *arg0) {
    s32 temp = *(s32 *)((u8 *)arg0 + 0x18);
    if (*(s32 *)((u8 *)arg0 + 0x70) != 0) {
        u8 *p = *(u8 **)((u8 *)arg0 + 0x70) + 0x58;
        p[4] = 1;
    } else {
        *(s32 *)((u8 *)arg0 + 0x70) = func_150C68C4(temp);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_F3BA0/func_150C68C4.s")

typedef struct GameF3BA0Burst {
    s32 flags, zero4;
    s16 texture, lifetime;
    s32 zeroC, zero10;
    u8 zero14, zero15, zero16, zero17, zero18, zero19, zero1A;
    u8 alpha, maximum, kind;
    s16 duration, period, zero22;
    f32 zero24, width, height;
    f32 x, y, z;
    f32 zero3C, zero40, zero44;
    f32 vx, vy, vz, rotation;
    s32 mode, unused5C;
    s8 field60, field61, field62, field63;
    u8 pad64[0xC];
} GameF3BA0Burst;

typedef struct GameF3BA0BurstMotion {
    f32 size;
    u8 mode0, mode1, count0, count1;
    f32 amount0, amount1, speed, damping;
    u8 mode2, mode3, count2, count3;
    f32 amount2, amount3, sourceScale;
} GameF3BA0BurstMotion;

void *func_151303BC(s32, u8, s32);
void *memcpy(void *, const void *, u32);
extern f32 D_800A045C, D_800A0460, D_800A0464, D_800A0468;
extern f32 D_800A046C, D_800A0470, D_800A0474, D_800A0478;

void func_150C6A08(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                  f32 arg4, f32 arg5, void *arg6) {
    GameF3BA0Burst burst;
    void *spawned;
    GameF3BA0BurstMotion motion;
    burst.kind = 0x2F;
    burst.texture = 0xC01;
    burst.flags = 0x200005;
    burst.zero4 = 0;
    burst.zero1A = 0;
    burst.zero19 = 0;
    burst.zero18 = 0;
    burst.zero17 = 0;
    burst.zero16 = 0;
    burst.zero15 = 0;
    burst.zero14 = 0;
    burst.zero10 = 0;
    burst.zeroC = 0;
    burst.maximum = 0xFF;
    burst.zero22 = 0;
    burst.zero24 = 0.0f;
    burst.mode = 0x3207;
    burst.field60 = 5;
    burst.field61 = 5;
    burst.field62 = 4;
    burst.field63 = -1;
    burst.x = arg0;
    burst.y = arg1;
    burst.zero3C = 0.0f;
    burst.zero40 = 0.0f;
    burst.zero44 = 0.0f;
    burst.duration = 30;
    burst.period = 8;
    burst.z = arg2;
    motion.sourceScale = *(f32 *)((u8 *)arg6 + 0x60);
    motion.speed = 4.0f;
    motion.damping = 0.25f;
    motion.mode0 = 0;
    motion.mode1 = 0;
    motion.mode2 = 0;
    motion.mode3 = 0;
    burst.vx = -arg3 * 0.2f;
    burst.vy = 0.0f;
    burst.vz = -arg5 * 0.2f;
    if (func_150ADA20() & 1) burst.mode |= 0x40;
    if (func_150ADA20() & 1) burst.mode |= 0x80;
    burst.lifetime = func_150ADA20() % 51U + 106;
    burst.alpha = func_150ADA20() % 101U + 100;
    motion.size = burst.width = burst.height = func_150ADA68() * 62.0f + 41.0f;
    motion.count0 = func_150ADA20() % 5U + 4;
    motion.count1 = func_150ADA20() % 5U + 4;
    motion.amount0 = (func_150ADA68() * 0.25f + D_800A0460) * motion.size;
    motion.amount1 = (func_150ADA68() * 0.25f + D_800A0464) * motion.size;
    motion.count2 = func_150ADA20() % 5U + 4;
    motion.count3 = func_150ADA20() % 5U + 4;
    motion.amount2 = (func_150ADA68() * D_800A0468 + D_800A046C) * motion.size;
    motion.amount3 = (func_150ADA68() * D_800A0470 + D_800A0474) * motion.size;
    burst.rotation = (func_150ADA68() * 60.0f + 21.0f) * D_800A0478;
    spawned = func_151303BC((s32)&burst, 2, 0x28);
    if (spawned != NULL) memcpy((u8 *)spawned + 0xA8, &motion, 0x28);
}

typedef struct {
    char pad[0x5C];
    u8 unk5C;
} Func150C6D1CArg;

s32 func_150C6D1C(Func150C6D1CArg* arg) {
    s32 ret = 1;
    if (arg->unk5C == 0) {
        ret = 0;
    }
    arg->unk5C = 0;
    return ret;
}

typedef struct {
    char pad[0x18];
    s32 unk18;
} Func150C6D40Inner;

typedef struct {
    char pad[0x58];
    Func150C6D40Inner inner58;
} Func150C6D40Sub;

typedef struct {
    char pad[0x58];
    Func150C6D40Sub* unk58;
} Func150C6D40Arg;

void func_150C6D40(Func150C6D40Arg* arg0) {
    Func150C6D40Inner* inner = &arg0->unk58->inner58;
    inner->unk18 = 0;
    func_151346EC((struct102*)arg0);
}

void func_150C6D68(Func150C6D40Arg* arg0) {
    Func150C6D40Inner* inner = &arg0->unk58->inner58;
    inner->unk18 = 0;
    func_1513470C((struct102*)arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_F3BA0/func_150C6D90.s")
