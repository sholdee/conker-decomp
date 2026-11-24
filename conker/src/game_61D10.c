#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s16 D_800C3EF0;

typedef struct {
    s16 a;
    s16 b;
} Pair15034860;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x1FE - 5];
    u8 unk1FE;
    u8 unk1FF;
    u8 unk200;
    u8 unk201;
    s8 unk202;
    s8 unk203;
    s16 unk204;
    u8 unk206;
    u8 unk207;
    s8 unk208;
    s8 unk209;
} Obj15034860;

Pair15034860 *func_15034860(Pair15034860 *arg0, Obj15034860 *arg1, s32 arg2, s32 arg3) {
    s32 flag;
    s32 padA;
    s32 reroll;
    f32 scale;
    s32 lo;
    s32 hi;
    s32 t1;
    s32 padB;
    s32 t0;
    s32 sign;
    s32 val;
    s32 neg;
    s16 pos;
    s8 vel;
    s16 sum;

    if (arg1->unk4 == 0x70) {
        flag = 1;
    } else {
        flag = 0;
    }
    if (arg1->unk4 == 0x7C) {
        t1 = 4;
        hi = 0;
    } else {
        t1 = 0;
        hi = 2;
    }
    switch (arg1->unk4) {
    case 0x69:
    case 0x7C:
    case 0x87:
        t0 = 0;
        break;
    default:
        t0 = 10;
        break;
    }
    if (arg2 != 0xFF) {
        if (arg1->unk201 == 0) {
            scale = 8191.875f;
        } else {
            scale = arg1->unk201 * 182.04444885f;
        }
        val = (s16)(s32)(arg1->unk1FE * scale * 0.003921568859f);
        if (!flag) {
            arg0->a = arg2 * 6 + t1;
            arg0->b = val;
            arg0++;
        }
        D_800C3EF0 = val;
        val = (s16)(s32)((f32)((arg1->unk202 * arg1->unk1FE) / 255) * 65535.0f * 0.003921568859f);
        if (!flag) {
            arg0->a = arg2 * 6 + hi;
            arg0->b = val;
            arg0++;
        }
        if (arg1->unk4 == 0x42) {
            lo = 0x50;
            hi = 0x78;
        } else {
            hi = 0x3C;
            lo = 0x28;
        }
        flag = arg1->unk1FF;
        if (flag == 0) {
            arg1->unk203 = -hi;
        } else if (flag == 2) {
            arg1->unk203 = hi;
            arg1->unk202 = 0;
        } else if (arg1->unk203 == 0 || arg1->unk1FE == 0 || arg1->unk1FE == 0xFF) {
            arg1->unk203 = func_150ADA20() % (u32)(hi - lo) + lo;
            if (arg1->unk1FE == 0xFF) {
                arg1->unk203 = -arg1->unk203;
            } else if (arg1->unk1FE == 0 && t0 != 0) {
                arg1->unk202 = func_150ADA20() % (u32)(t0 * 2) - t0;
            }
        }
        sum = arg1->unk1FE;
        sum += arg1->unk203;
        sum = (sum < 0) ? 0 : ((sum >= 0x100) ? 0xFF : sum);
        arg1->unk1FE = sum;
        if (arg1->unk1FF == 3) {
            if (arg1->unk200 >= D_800BE9E4) {
                arg1->unk200 -= D_800BE9E4;
            } else {
                arg1->unk200 = 0;
                arg1->unk1FF = 0;
            }
        }
    }
    if (arg3 != 0xFF) {
        sign = 1;
        if (arg1->unk208 == 0) {
            scale = 8191.875f;
        } else {
            scale = arg1->unk208 * 182.04444885f;
        }
        if (scale < 0.0f) {
            sign = -1;
        } else {
            sign = 1;
        }
        scale = fabsf(scale);
        sum = (s32)(arg1->unk204 * scale * 0.003921568859f);
        arg0->a = arg3 * 6 + t1;
        reroll = 0;
        neg = 0;
        arg0->b = -sum;
        arg0++;
        sum = arg1->unk206;
        if (sum == 0) {
            if (sign > 0) {
                arg1->unk209 = -0x3C;
            } else {
                arg1->unk209 = 0x3C;
            }
        } else if (sum == 2) {
            if (sign > 0) {
                arg1->unk209 = 0x3C;
            } else {
                arg1->unk209 = -0x3C;
            }
        } else if ((vel = arg1->unk209) == 0) {
            reroll = 1;
        } else {
            pos = arg1->unk204;
            if (sign > 0) {
                if (pos < 0 && vel < 0) {
                    neg = 1;
                } else if (pos == 0 && vel < 0) {
                    reroll = 1;
                } else if (pos * 1 == 0xFF) {
                    neg = 1;
                    reroll = 1;
                }
            } else {
                if (pos > 0 && vel > 0) {
                    neg = 1;
                } else if (pos == 0 && vel > 0) {
                    reroll = 1;
                } else if (pos == -0xFF) {
                    neg = 1;
                    reroll = 1;
                }
            }
        }
        if (reroll) {
            arg1->unk209 = func_150ADA20() % 20U + 0x28;
            arg1->unk209 = arg1->unk209 * sign;
        }
        if (neg) {
            arg1->unk209 = -arg1->unk209;
        }
        arg1->unk204 += arg1->unk209;
        if (sign > 0) {
            if (arg1->unk204 < 0 && arg1->unk209 < 0) {
                arg1->unk204 = 0;
            } else if (arg1->unk204 >= 0x100) {
                arg1->unk204 = 0xFF;
            }
        } else {
            if (arg1->unk204 > 0 && arg1->unk209 > 0) {
                arg1->unk204 = 0;
            } else if (arg1->unk204 < -0xFF) {
                arg1->unk204 = -0xFF;
            }
        }
        if (arg1->unk206 == 3) {
            if (arg1->unk207 >= D_800BE9E4) {
                arg1->unk207 -= D_800BE9E4;
            } else {
                arg1->unk207 = 0;
                arg1->unk206 = 0;
            }
        }
    }
    return arg0;
}

extern s16 D_800C3EF0;
extern f32 D_80097D60;

struct Entry15034EB4 {
    u8 pad[0x34];
    f32 unk34;
    u8 pad2[0x8];
};

struct Obj15034EB4 {
    u8 pad[0x14C];
    f32 unk14C;
    u8 pad2[0x84];
    struct Entry15034EB4 *unk1D4;
};

void func_15034EB4(struct Obj15034EB4 *obj, s32 idx, s32 idx2) {
    f32 amt;
    struct Entry15034EB4 *e;
    struct Entry15034EB4 *e2;
    struct Entry15034EB4 *base;

    if (D_800C3EF0 != 0) {
        base = obj->unk1D4;
        amt = ((f32)D_800C3EF0 * D_80097D60) * obj->unk14C;
        e = &base[idx];
        e->unk34 -= amt;
        if (idx2 != -1) {
            e2 = &base[idx2];
            e2->unk34 -= amt;
        }
    }
}

extern u8 D_800C3F00;
void func_15034F20(void);

void func_150A7A48(void *, void *, void *);
void guMtxF2L2(void *, void *);
void func_15034728(void *);
s32 func_150AD960(s32, s32, s32, s32);
void func_15034F30(u8 *arg0, u8 *arg1, s32 arg2, f32 (*arg3)[4][4], f32 (*arg4)[4][4], s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, f32 arg11, f32 arg12, f32 arg13, f32 arg14, f32 arg15, f32 arg16, s32 arg17, s32 arg18);

struct Entry150356C8 {
    u32 unk0;
    u8 unk4;
    u8 pad5[4];
    u8 unk9;
    u8 unkA;
    u8 unkB;
};

extern struct Entry150356C8 D_800C3F08[];
struct Entry150356C8 *func_150356C8(void);


s32 func_150A6360(void *, void *, f32, f32, f32, f32, f32, f32);
extern f32 D_80097D70;
extern u8 D_800D9C10;
s32 func_15035714(s32 arg0, u8 *arg1, u8 *arg2, f32 arg3);

extern u8 D_80082FC0[];
extern u8 D_80083140[];

struct Obj15035D6C {
    u8 unk0;
    u8 pad1[2];
    u8 unk3;
    u8 pad4[2];
    u8 unk6;
    u8 pad7[0xC];
    u8 unk13;
    u8 unk14;
    u8 pad15[0xF];
    Gfx **unk24;
};
Gfx *func_15035D6C(Gfx *gfx, struct Obj15035D6C *obj, s32 red, s32 green, s32 blue);

extern Gfx *func_1502CCFC(Gfx *, s32, s32, u32, s32, s32 *, s32, s32);
Gfx *func_15035FE8(Gfx *gfx, s32 arg1);

void func_1507C3E0(struct127 *arg0, s16 *arg1, s16 *arg2, s16 *arg3);
void func_15035808(s32, s32, f32, f32, f32, f32, f32, f32, f32, f32);
void func_15036148(void);
