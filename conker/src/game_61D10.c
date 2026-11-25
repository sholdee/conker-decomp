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

/* Element of the array that struct127::unk1D4 points to here (0x40 bytes). */
typedef struct {
    u8 pad0[0x34];
    f32 unk34;
    u8 pad38[0x8];
} Entry15034EB4;

/* Subtracts the last value stored in D_800C3EF0, scaled by the actor's xz_scale, from unk34 of one or two entries. */
void func_15034EB4(struct127 *actor, s32 first, s32 second) {
    f32 amount;
    Entry15034EB4 *entry;
    Entry15034EB4 *entry2;
    Entry15034EB4 *entries;

    if (D_800C3EF0 != 0) {
        entries = (Entry15034EB4 *)actor->unk1D4;
        amount = ((f32)D_800C3EF0 * 0.0007629627362f) * actor->xz_scale;
        entry = &entries[first];
        entry->unk34 -= amount;
        if (second != -1) {
            entry2 = &entries[second];
            entry2->unk34 -= amount;
        }
    }
}
