#include <ultra64.h>
#define func_15048A40 func_15048A40_void_unused
#define func_10010F88 func_10010F88_void_decl_in_functions_h
#include "functions.h"
#undef func_15048A40
#undef func_10010F88
s32 func_10010F88(s32 arg0, u16 arg1, s16 arg2, u8 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 arg9);
#include "variables.h"
extern f32 func_15048A40(u8);
extern void func_15188010(s32, f32 *);
extern s32 func_151149AC(u32);
extern f32 D_800A2F8C;
extern void *allocate_memory(s32, s32, s32, s32);
extern f32 D_800A3210;
extern f32 D_800A3214;
extern f32 D_800A2FF4;
extern f32 D_800A2FF8;
extern f32 D_800A2FFC;
extern f32 D_800A2FB0;
extern f32 D_800A2FB4;

struct Struct1511F31CArg {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x28];
    u16 unk3E;
    u8 pad40[0x14];
    u16 unk54;
};

extern void func_1511F31C(struct Struct1511F31CArg *arg0);

struct Struct1511490C {
    char pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    char pad16[2];
    f32 unk18;
    char pad1C[0x10];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
};

extern void func_1511490C(f32 arg0[4][4], struct Struct1511490C *arg1);
extern void func_150A7960(f32 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6);

struct Obj151151FC {
    f32 unk0;
    u8 pad4[0x38];
    s32 unk3C;
    u8 pad40[0x20];
    f32 unk60;
};

struct Obj151150BC {
    u8 pad0[0x8];
    f32 unk8;
    u8 padC[0x30];
    s32 unk3C;
    u8 pad40[0x28];
    f32 unk68;
};

struct Obj1511515C {
    u8 pad0[0x4];
    f32 unk4;
    u8 pad8[0x34];
    s32 unk3C;
    u8 pad40[0x24];
    f32 unk64;
};

struct Obj151152A8 {
    char pad_0x0[0x12];
    s16 field_0x12;
    char pad_0x14[0x28];
    s32 field_0x3C;
    char pad_0x40[0xF];
    u8 field_0x4F;
    char pad_0x50[0x23];
    u8 field_0x73;
    char pad_0x74[0x8];
    s32 field_0x7C;
};

void func_151150B0(s32 arg0) {
}

void func_151150BC(struct Obj151150BC *arg0) {
    arg0->unk68 = (f32)((arg0->unk3C >> 16) * D_800BE9E4) * 0.00390625f;
    arg0->unk8 = arg0->unk8 + arg0->unk68;
    if (arg0->unk8 < 0.0f) {
        arg0->unk8 = arg0->unk8 + 360.0f;
        return;
    }
    if (arg0->unk8 >= 360.0f) {
        arg0->unk8 = arg0->unk8 - 360.0f;
    }
}

void func_1511515C(struct Obj1511515C *arg0) {
    arg0->unk64 = (f32)((arg0->unk3C >> 16) * D_800BE9E4) * 0.00390625f;
    arg0->unk4 = arg0->unk4 + arg0->unk64;
    if (arg0->unk4 < 0.0f) {
        arg0->unk4 = arg0->unk4 + 360.0f;
        return;
    }
    if (arg0->unk4 >= 360.0f) {
        arg0->unk4 = arg0->unk4 - 360.0f;
    }
}

void func_151151FC(struct Obj151151FC *arg0) {
    arg0->unk60 = (f32)((arg0->unk3C >> 16) * D_800BE9E4) * 0.00390625f;
    arg0->unk0 = arg0->unk0 + arg0->unk60;
    if (arg0->unk0 < 0.0f) {
        arg0->unk0 = arg0->unk0 + 360.0f;
        return;
    }
    if (arg0->unk0 >= 360.0f) {
        arg0->unk0 = arg0->unk0 - 360.0f;
    }
}

void func_1511529C(s32 arg0) {
}

void func_151152A8(struct Obj151152A8 *arg0) {
    s32 target;
    s32 current;
    s32 step;
    s32 *targetPtr;

    targetPtr = &arg0->field_0x7C;
    target = *targetPtr;
    if (target == 0) {
        target = arg0->field_0x12;
        *targetPtr = target;
    }
    current = arg0->field_0x12;
    if ((arg0->field_0x4F & 0xFF) & 4) {
        step = arg0->field_0x3C;
        if ((target - current) < (s16)step) {
            arg0->field_0x12 = current - ((s16)(step >> 16) * D_800BE9E4);
            return;
        }
    } else {
        arg0->field_0x12 = (s32)((f32)current + ((f32)(target - current) * D_800A2F8C * (f32)D_800BE9E4));
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_15115368.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_151155C0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511575C.s")

extern f32 D_800A2FA0;
extern f32 D_800A2FA4;

struct S_15115EDC { u8 pad[0x7C]; f32 unk7C; f32 unk80; };
struct A_15115EDC { u8 pad[0x84]; u16 unk84; };

struct B_15115E0C {
    u8 pad0[0x4];
    f32 unk4;
    u8 pad8[0x8];
    s16 unk10;
    u8 pad12[0x2];
    s16 unk14;
    u8 pad16[0x39];
    u8 unk4F;
    u8 pad50[0x2C];
    f32 unk7C;
    f32 unk80;
};

struct C_15115E0C {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
};

struct Obj151163C0 {
    f32 unk0;
    u8 pad4[0xE];
    s16 unk12;
    u8 pad14[0x4];
    f32 unk18;
    u8 pad1C[0x20];
    s32 unk3C;
    u8 pad40[0x1C];
    s16 unk5C;
    u8 pad5E[0x2];
    f32 unk60;
    u8 pad64[0x18];
    s32 unk7C;
    s32 unk80;
};

void func_15115E0C(struct B_15115E0C *arg0, struct C_15115E0C *arg1) {
    f32 s;
    f32 c;
    f32 dx;
    f32 dy;

    s = func_150AD780(arg0->unk4 * D_800A2FA0);
    c = func_150AD78C(arg0->unk4 * D_800A2FA4);
    dx = arg1->unk14 - (f32)arg0->unk10;
    dy = arg1->unk1C - (f32)arg0->unk14;
    if ((arg0->unk4F & 4) == 4) {
        arg0->unk7C = arg0->unk7C + (c * dx + s * dy);
        arg0->unk80 = arg0->unk80 - (c * dy + s * dx);
    }
}

void func_15115EDC(struct S_15115EDC *arg0, struct A_15115EDC *arg1) {
    f32 oldX;
    f32 oldY;
    oldX = arg0->unk7C;
    oldY = arg0->unk80;
    func_15115E0C((struct B_15115E0C *)arg0, (struct C_15115E0C *)arg1);
    if (arg1->unk84 == 0x4B) {
        oldX = (arg0->unk7C - oldX) * 4.0f;
        oldY = (arg0->unk80 - oldY) * 4.0f;
        arg0->unk7C = arg0->unk7C + oldX;
        arg0->unk80 = arg0->unk80 + oldY;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_15115F68.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_15116058.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_15116110.s")

struct Obj1511617C {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[0x26];
    s32 vel;
    u8 pad40[0xF];
    u8 flags;
    u8 pad50[0xC];
    s16 dy;
    u8 pad5E[0x1E];
    f32 ground;
};

extern u16 D_800CC354;
extern f32 D_800A2FA8;
extern f32 D_800A2FAC;
extern f32 func_1510F648(f32, f32, f32);

void func_1511617C(struct Obj1511617C *arg0) {
    f32 delta;
    s32 target;
    s16 oldY;

    oldY = arg0->y;
    arg0->ground = func_1510F648(arg0->x, arg0->y, arg0->z);
    if (fabsf(arg0->ground - arg0->y) > 200.0f) {
        arg0->vel = 0;
    }
    if (!(arg0->flags & 4)) {
        delta = -arg0->vel;
        arg0->vel += delta * D_800A2FA8;
    } else {
        target = -20;
        if (D_800CC354 == 0x4B) {
            target = -40;
        }
        delta = target - arg0->vel;
        arg0->vel += delta * D_800A2FAC;
    }
    arg0->y = arg0->ground + arg0->vel;
    arg0->dy = arg0->y - oldY;

}

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_151162D4.s")

void func_151163C0(struct Obj151163C0 *arg0) {
    volatile f32 sp2C;
    u32 sp28;
    u32 sp24;
    volatile f32 sp20;
    s16 sp1E;
    f32 sp18;
    f32 temp_f2;
    u32 temp_t2;
    s32 temp_v0;

    temp_v0 = arg0->unk3C;
    temp_t2 = (temp_v0 >> 8) & 0xFF;
    sp28 = (temp_v0 >> 0x10) & 0xFF;
    sp2C = (f32)(temp_v0 & 0xFF);
    sp24 = (temp_v0 >> 0x18) & 0xFF;
    sp20 = (f32)temp_t2 * 1.40625f;
    sp1E = arg0->unk12;
    sp18 = arg0->unk0;
    arg0->unk18 = cosf((f32)arg0->unk7C * D_800A2FB0) * sp2C;
    temp_f2 = cosf((f32)arg0->unk80 * D_800A2FB4) * sp20;
    arg0->unk0 = temp_f2;
    arg0->unk7C = arg0->unk7C + (sp28 * D_800BE9E4);
    arg0->unk80 = arg0->unk80 + (sp24 * D_800BE9E4);
    arg0->unk5C = arg0->unk12 - sp1E;
    arg0->unk60 = temp_f2 - sp18;
}

struct Obj1511650C {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x26];
    s32 unk3C;
    u8 pad40[0xF];
    u8 unk4F;
    u8 pad50[0x14];
    f32 unk64;
    u8 pad68[0xC];
    u16 unk74;
    u8 pad76[0x6];
    s32 unk7C;
    s32 unk80;
};

struct Obj15116888;
s32 func_15116888(s32 arg0, s32 arg1, s32 arg2, struct Obj15116888 *arg3);

void func_1511650C(struct Obj1511650C *arg0, s32 arg1, s32 arg2, f32 arg3) {
    s32 var_a0;
    s32 var_v0;
    s32 temp_v1;
    s32 temp_a1;
    f32 temp_f14;
    f32 var_f12;
    u32 temp_v0;
    u16 temp_a0;

    if (!(arg0->unk4F & 4)) {
        var_a0 = 0;
    } else {
        var_a0 = func_15116888(arg0->unk10, arg0->unk14, (s32)&arg0->unk7C, (struct Obj15116888 *)D_800CC2D0) >> 3;
    }
    if ((arg1 & 1) && (var_a0 < 0)) {
        var_a0 = 0;
    }
    var_v0 = *(s16 *)&arg0->unk3C;
    var_v0 += var_a0;
    if (var_a0 != 0) {
        arg0->unk80 = 0;
    }
    temp_v1 = arg0->unk80;
    temp_a1 = var_v0 * -0.0032;
    if (temp_v1 <= 0) {
        var_v0 += temp_a1;
        if (var_v0 > 0) {
            var_v0 -= 1;
            if (var_v0 <= 0) {
                if (var_a0 == 0) {
                    if (temp_v1 == 0) {
                        arg0->unk80 = 20;
                    } else {
                        arg0->unk80 = 0;
                        var_v0 = 0;
                    }
                } else {
                    var_v0 = 0;
                }
            }
        } else if (var_v0 < 0) {
            var_v0 += 1;
            if (var_v0 >= 0) {
                if (var_a0 == 0) {
                    if (temp_v1 == 0) {
                        arg0->unk80 = 20;
                    } else {
                        arg0->unk80 = 0;
                        var_v0 = 0;
                    }
                } else {
                    var_v0 = 0;
                }
            }
        }
    } else if (temp_v1 > 0) {
        if (var_v0 > 0) {
            var_v0 += 1;
            arg0->unk80 = temp_v1 - D_800BE9E4;
            if (arg0->unk80 <= 0) {
                arg0->unk80 = -1;
            }
        } else {
            var_v0 -= 1;
            arg0->unk80 = temp_v1 - D_800BE9E4;
            if (arg0->unk80 <= 0) {
                arg0->unk80 = -1;
            }
        }
    }
    var_v0 <<= 16;
    arg0->unk3C = (arg0->unk3C & 0xFFFF) | var_v0;
    func_1511515C((struct Obj1511515C *)arg0);
    if (arg2 != 0) {
        f32 one = 1.0f;
        temp_f14 = arg0->unk64;
        var_f12 = fabsf(temp_f14) * 0.1000000015f;
        if (var_f12 > 1.0f) {
            var_f12 = 1.0f;
        }
        temp_v0 = (12767.0f * var_f12) + 20000.0f;
        temp_a0 = arg0->unk74;
        var_f12 = (var_f12 * arg3) - 500.0f;
        if (temp_a0 == 0) {
            if (0.009999999776f < temp_f14) {
                arg0->unk74 = func_10010F88(arg2, temp_v0, var_f12, 0, -1, arg0->unk10, arg0->unk12, arg0->unk14, 1000, 6000);
            }
        } else {
            if (0.009999999776f < temp_f14) {
                func_1000F91C(temp_a0, temp_v0, var_f12, 0, -1, arg0->unk10, arg0->unk12, arg0->unk14, 200, 2000);
                return;
            }
            func_100111C8(temp_a0);
            arg0->unk74 = 0;
        }
    }
}

struct Obj15116888 {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
    u8 pad20[0x1C];
    f32 unk3C;
    u8 pad40[0x36];
    u16 unk76;
};

struct Vec15116888 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[0x4];
};

s32 func_150490A8(struct17 *arg0);

s32 func_15116888(s32 arg0, s32 arg1, s32 arg2, struct Obj15116888 *arg3) {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    f32 r;
    struct Vec15116888 sp1C;

    a = (s32)arg3->unk14;
    b = (s32)arg3->unk1C;
    sp1C.unk0 = (f32)(a - arg0);
    sp1C.unk8 = (f32)(b - arg1);

    c = func_150490A8((struct17 *)&sp1C);
    d = (s32)arg3->unk76 >> 8;
    d = 0x40 - d;
    d = d - c;
    r = func_15048A40((u8)d);
    r = arg3->unk3C * r;
    return (s32)r;
}

void func_15116924(s32 arg0) {
}

void func_15116930(u8 *arg0, u8 *arg1) {
    s32 temp;
    if (arg0[0x4F] & 0x4) {
        if (!(arg0[0x73] & 0x3) && !(arg0[0x73] & 0x4)) {
            if (((u8 **)arg1)[0x31C / 4][0x57] == 1) {
                arg0[0x73] &= 0xFFFC;
                arg0[0x73] |= 0x2;
            }
        }
    }
}

struct Obj15116BAC {
    u8 pad0[0x10];
    s16 unk10[3];
    u8 pad16[0x26];
    s32 unk3C;
    u8 pad40[0x1A];
    s16 unk5A[3];
    u8 pad60[0x14];
    u16 unk74;
    u8 pad76[0x6];
    s32 unk7C[3];
};

struct SoundEntry15116BAC {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
};

extern struct SoundEntry15116BAC D_80089260[];
extern struct SoundEntry15116BAC D_80089264[];
extern struct SoundEntry15116BAC D_80089268[];
extern void func_15114D24(struct Obj15116BAC *, s32, s32, s32, s32, s32);
extern void func_151169B4(struct Obj15116BAC *arg0);
extern void func_15116BAC(struct Obj15116BAC *arg0);

void func_15116984(u8 *arg0) {
    if (arg0[0x73] & 0x2) {
        func_151169B4((struct Obj15116BAC *)arg0);
    }
}

void func_151169B4(struct Obj15116BAC *arg0) {
    s32 step;
    s32 sound;
    s16 *rot;
    s16 *prev;
    s32 *tgt;
    s32 i;

    sound = arg0->unk3C >> 10;
    if ((arg0->unk7C[0] != arg0->unk10[0]) || (arg0->unk7C[1] != arg0->unk10[1]) || (arg0->unk7C[2] != arg0->unk10[2])) {
        if ((sound != 0) && (D_80089268[sound].unk0 != 0)) {
            func_15116BAC(arg0);
            return;
        }
        step = arg0->unk3C & ~0xFC00;
        if ((sound != 0) && (arg0->unk74 == 0) && (D_80089260[sound].unk0 != 0)) {
            func_15114D24(arg0, D_80089260[sound].unk0, 0x5DC0, 0x7D0, 0xFA0, 0);
        }
        step = (step * D_800BE9E4) >> 1;
        rot = arg0->unk10;
        prev = arg0->unk5A;
        tgt = arg0->unk7C;
        for (i = 0; i < 3; i++) {
            prev[i] = rot[i];
            if (rot[i] != tgt[i]) {
                if (tgt[i] < rot[i]) {
                    rot[i] = rot[i] - step;
                    if (rot[i] < tgt[i]) {
                        rot[i] = tgt[i];
                    }
                } else {
                    rot[i] = rot[i] + step;
                    if (tgt[i] < rot[i]) {
                        rot[i] = tgt[i];
                    }
                }
            }
            prev[i] = rot[i] - prev[i];
        }
    } else {
        arg0->unk5A[2] = 0;
        arg0->unk5A[1] = arg0->unk5A[2];
        arg0->unk5A[0] = arg0->unk5A[2];
        if (arg0->unk74 != 0) {
            if ((sound != 0) && (D_80089264[sound].unk0 != 0)) {
                func_15114D24(arg0, D_80089264[sound].unk0, 0x5DC0, 0x7D0, 0xFA0, 4);
            }
            arg0->unk74 = 0;
        }
    }
}

void func_15116BAC(struct Obj15116BAC *arg0) {
    s32 rate;
    s32 step;
    s32 sound;
    s32 started;
    s16 *rot;
    s16 *prev;
    s32 *tgt;
    s32 diff;
    s32 i;
    s32 maxDiff;

    sound = arg0->unk3C >> 10;
    started = 0;
    maxDiff = 0;
    rate = arg0->unk3C & ~0xFC00;
    if ((sound != 0) && (arg0->unk74 == 0) && (D_80089260[sound].unk0 != 0)) {
        func_15114D24(arg0, D_80089260[sound].unk0, 0x5DC0, 0x7D0, 0xFA0, 0);
        started = 1;
    }
    step = (rate * D_800BE9E4) >> 1;
    rot = arg0->unk10;
    prev = arg0->unk5A;
    tgt = arg0->unk7C;
    for (i = 0; i < 3; i++) {
        prev[i] = rot[i];
        if (rot[i] != tgt[i]) {
            if (tgt[i] < rot[i]) {
                rot[i] = rot[i] - step;
                diff = rot[i] - tgt[i];
                if (diff < 0) {
                    rot[i] = tgt[i];
                }
            } else {
                rot[i] = rot[i] + step;
                diff = tgt[i] - rot[i];
                if (diff < 0) {
                    rot[i] = tgt[i];
                }
            }
            if (maxDiff < diff) {
                maxDiff = diff;
            }
        }
        prev[i] = rot[i] - prev[i];
    }
    if ((sound != 0) && (D_80089260[sound].unk8 != 0)) {
        maxDiff -= (D_80089260[sound].unkA * rate) >> 1;
        if (maxDiff <= 0) {
            if ((maxDiff > -step) || started) {
                func_15114D24(arg0, D_80089260[sound].unk8, 0x5DC0, 0x7D0, 0xFA0, 0);
            }
        }
    }
}

struct Obj15116D7C {
    f32 unk0[3];
    u8 padC[0x30];
    s32 unk3C;
    u8 pad40[0x20];
    f32 unk60[3];
    u8 pad6C[0x10];
    f32 unk7C[3];
};

void func_15116D7C(struct Obj15116D7C *arg0) {
    f32 step;
    f32 dt;
    f32 *rot;
    f32 *cur;
    f32 *tgt;
    s32 i;

    step = arg0->unk3C * 0.00390625f;
    dt = D_800BE9E4;
    rot = arg0->unk0;
    cur = arg0->unk60;
    tgt = arg0->unk7C;
    for (i = 0; i < 3; i++) {
        if (cur[i] != tgt[i]) {
            if (tgt[i] < cur[i]) {
                cur[i] -= step;
                if (cur[i] < tgt[i]) {
                    cur[i] = tgt[i];
                }
            } else {
                cur[i] += step;
                if (tgt[i] < cur[i]) {
                    cur[i] = tgt[i];
                }
            }
        }
        rot[i] += cur[i] * dt;
        if (rot[i] < 0.0f) {
            rot[i] += 360.0f;
        } else if (rot[i] >= 360.0f) {
            rot[i] -= 360.0f;
        }
    }
}

typedef struct {
    u8 frames;
    u8 count;
} Anim15116EA4;

typedef struct {
    s32 start;
    s32 end;
} Rom15116EA4;

extern Anim15116EA4 D_80089320[];
extern Rom15116EA4 D_8002AAD0[];

struct Obj15116EA4 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0xA];
    Vtx *unk20[2];
    u8 pad28[0x14];
    s32 unk3C;
    u8 pad40[0x3C];
    s32 unk7C;
    s16 *unk80;
    s32 unk84;
};

void func_15116EA4(struct Obj15116EA4 *arg0) {
    s32 size;
    f32 frac;
    u32 frameSize;
    s32 n;
    s32 i;
    s16 *cur;
    s16 *next;
    s32 idx;
    s32 pos;
    s32 frames;
    s32 count;
    s32 speed;
    s32 frame;

    idx = arg0->unk3C & 0xFFFF;
    frames = D_80089320[idx].frames;
    pos = arg0->unk84;
    count = D_80089320[idx].count;
    size = D_8002AAD0[frames].end - D_8002AAD0[frames].start;
    speed = arg0->unk3C >> 16;
    if (arg0->unk80 == NULL) {
        arg0->unk80 = allocate_memory(size, 1, 2, 0);
        func_10004514(D_8002AAD0[frames].start, arg0->unk80, size, 1);
    }
    frac = count * (pos * 1.525902189e-05f);
    frame = frac;
    frac = frac - frame;
    if (count == frame) {
        frac = 1.0f;
        frame--;
    }
    frameSize = (size / count) & ~1;
    n = frameSize / 6;
    cur = (s16 *)((u8 *)arg0->unk80 + frame * frameSize);
    if (frame + 1 != (u32)count) {
        next = (s16 *)((u8 *)arg0->unk80 + (frame + 1) * frameSize);
    } else {
        next = cur;
    }
    for (i = 0; i < n; i++) {
        arg0->unk20[D_800BE9C0][i].v.ob[0] = cur[i * 3 + 0] + (next[i * 3 + 0] - (f32)cur[i * 3 + 0]) * frac + arg0->unk10;
        arg0->unk20[D_800BE9C0][i].v.ob[2] = cur[i * 3 + 2] + (next[i * 3 + 2] - (f32)cur[i * 3 + 2]) * frac + arg0->unk12;
        arg0->unk20[D_800BE9C0][i].v.ob[1] = cur[i * 3 + 1] + (next[i * 3 + 1] - (f32)cur[i * 3 + 1]) * frac + arg0->unk14;
    }
    if (arg0->unk7C < pos) {
        pos -= speed;
        if (pos < arg0->unk7C) {
            pos = arg0->unk7C;
        }
    } else if (pos < arg0->unk7C) {
        pos += speed;
        if (arg0->unk7C < pos) {
            pos = arg0->unk7C;
        }
    }
    arg0->unk84 = pos;
}

typedef struct Motion151172D8 {
    u8 pad0[0x3C];
    s32 packedTarget;
    u8 pad40[0x3C];
    f32 damping;
    f32 acceleration;
    f32 velocity;
} Motion151172D8;

extern f32 D_800A2FD8;

f32 func_151172D8(Motion151172D8 *arg0, f32 angle) {
    f32 velocity;
    f32 delta;
    f32 target;
    f32 magnitude;
    f32 damping;
    f32 acceleration;

    damping = arg0->damping;
    acceleration = arg0->acceleration;
    velocity = arg0->velocity;
    target = (f32)((arg0->packedTarget << 22) >> 22);
    if (angle != target || velocity != 0.0f) {
        if (velocity == 0.0f && (arg0->packedTarget >> 10) != 0) {
            if (D_80089260[arg0->packedTarget >> 10].unk0 != 0) {
                func_15114D24((struct Obj15116BAC *)arg0, D_80089260[arg0->packedTarget >> 10].unk0, 0x5DC0, 0xC8, 0x9C4, 0);
            }
        }
        angle += velocity * (f32)D_800BE9E4;
        delta = func_15048A70(angle, target);
        magnitude = fabsf(delta);
        if (delta > 0.0f) {
            velocity += magnitude * acceleration;
        } else {
            velocity -= magnitude * acceleration;
        }
        velocity *= damping;
        if (fabsf(delta) < D_800A2FD8 && fabsf(velocity) < D_800A2FD8) {
            angle = target;
            velocity = 0.0f;
        } else if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        arg0->velocity = velocity;
    }
    return angle;
}

struct Obj151174A0 {
    char pad0[0x8];
    f32 unk8;
};

void func_151174A0(struct Obj151174A0 *arg0) {
    arg0->unk8 = func_151172D8((Motion151172D8 *)arg0, arg0->unk8);
}

struct Obj151174C8 {
    char pad0[0x4];
    f32 unk4;
};

void func_151174C8(struct Obj151174C8 *arg0) {
    arg0->unk4 = func_151172D8((Motion151172D8 *)arg0, arg0->unk4);
}

struct Obj151174F0 {
    f32 unk0;
};

void func_151174F0(struct Obj151174F0 *arg0) {
    arg0->unk0 = func_151172D8((Motion151172D8 *)arg0, arg0->unk0);
}

f32 func_15117518(struct Obj151174F0 *arg0, f32 current) {
    f32 velocity;
    s32 sound_index;
    f32 target;
    f32 after;
    f32 before;
    f32 rebound;
    f32 acceleration;

    rebound = *(f32 *)((u8 *)arg0 + 0x7C);
    acceleration = *(f32 *)((u8 *)arg0 + 0x80) * (f32)D_800BE9E4;
    velocity = *(f32 *)((u8 *)arg0 + 0x84);
    target = (f32)((*(s32 *)((u8 *)arg0 + 0x3C) << 22) >> 22);
    if (current != target || velocity != 0.0f) {
        sound_index = *(s32 *)((u8 *)arg0 + 0x3C) >> 10;
        if (velocity == 0.0f && sound_index != 0 && D_80089260[sound_index].unk0 != 0) {
            func_15114D24((struct Obj15116BAC *)arg0, D_80089260[sound_index].unk0, 24000, 200, 2500, 0);
        }
        before = func_15048A70(current, target);
        current += velocity * (f32)D_800BE9E4;
        after = func_15048A70(current, target);
        if ((after <= 0.0f && before > 0.0f) || (after >= 0.0f && before < 0.0f)) {
            current = target;
            velocity = -rebound * velocity;
            after = 0;
        } else if (after > 0.0f) {
            velocity += acceleration;
        } else {
            velocity -= acceleration;
        }
        if (fabsf(after) < 2.0f && fabsf(velocity) < 1.0f) {
            current = target;
            velocity = 0.0f;
        } else if (current < 0.0f) {
            current += 360.0f;
        } else if (current >= 360.0f) {
            current -= 360.0f;
        }
        *(f32 *)((u8 *)arg0 + 0x84) = velocity;
    }
    return current;
}

extern f32 func_15117518(struct Obj151174F0 *, f32);

void func_15117770(struct Obj151174F0 *arg0) {
    arg0->unk0 = func_15117518(arg0, arg0->unk0);
}

struct Obj15117798 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
};

struct Obj15117DA4 {
    f32 unk0;
    f32 unk4;
    u8 pad8[0x8];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x28];
    u16 unk3E;
    u8 pad40[0x14];
    u16 unk54;
};

void func_15117798(struct Obj15117798 *arg0) {
    arg0->unk8 = func_15117518((struct Obj151174F0 *)arg0, arg0->unk8);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_151177C0.s")

s32 func_150AD9A0(s32, s32, s32);
extern f32 D_800A2FDC;
extern f32 D_800A2FE0;
extern f32 D_800A2FE4;
extern f32 D_800A2FE8;
extern f32 D_800A2FEC;
extern f32 D_800A2FF0;
void func_151179BC(void *arg0) {
    s16 x;
    s16 y;
    s16 z;
    s16 speed;
    s32 cur;
    s32 dist;
    f32 fdist;
    f32 m;
    s32 state;
    s32 delay;
    f32 cx;
    f32 sx;
    f32 cy;
    f32 sy;
    f32 cz;
    f32 sz;

    state = *(u8 *)((u8 *)arg0 + 0x73) & 3;
    speed = (*(s32 *)((u8 *)arg0 + 0x3C) >> 16) & 0xFFFF;
    delay = *(s32 *)((u8 *)arg0 + 0x3C) & 0xFFFF;
    if (*(s32 *)((u8 *)arg0 + 0x80) == 0) {
        *(s32 *)((u8 *)arg0 + 0x7C) = (*(s16 *)((u8 *)arg0 + 0x12) << 16) | (*(s16 *)((u8 *)arg0 + 0x10) & 0xFFFF);
        *(s32 *)((u8 *)arg0 + 0x80) = *(s16 *)((u8 *)arg0 + 0x14) | 0x80000000;
    }
    x = *(s32 *)((u8 *)arg0 + 0x7C) & 0xFFFF;
    y = *(s32 *)((u8 *)arg0 + 0x7C) >> 16;
    z = *(s32 *)((u8 *)arg0 + 0x80) & 0xFFFF;
    if (state == 0) {
        *(s16 *)((u8 *)arg0 + 0x10) = x;
        *(s16 *)((u8 *)arg0 + 0x12) = y;
        *(s16 *)((u8 *)arg0 + 0x14) = z;
        return;
    }
    cur = func_150AD9A0(x - *(s16 *)((u8 *)arg0 + 0x10), y - *(s16 *)((u8 *)arg0 + 0x12), z - *(s16 *)((u8 *)arg0 + 0x14));
    if (state == 3) {
        if (speed < 0) {
            dist = -speed;
        } else {
            dist = speed;
        }
        if ((delay != 0) && !(*(u8 *)((u8 *)arg0 + 0x4F) & 4) && !(*(u8 *)((u8 *)arg0 + 0x73) & 4)) {
            if (D_800BE9E4 < *(s32 *)((u8 *)arg0 + 0x84)) {
                *(s32 *)((u8 *)arg0 + 0x84) -= D_800BE9E4;
            } else {
                state = 1;
            }
        } else {
            *(s32 *)((u8 *)arg0 + 0x84) = delay;
        }
    } else if (state == 2) {
        dist = cur + 6;
        if (dist >= ((speed < 0) ? -speed : speed)) {
            state = 3;
            if (speed < 0) {
                dist = -speed;
            } else {
                dist = speed;
            }
            *(s32 *)((u8 *)arg0 + 0x84) = delay;
        }
    } else if (state == 1) {
        if (cur < 3) {
            dist = 0;
            state = 0;
        } else {
            dist = cur - 3;
        }
    }
    if (cur != dist) {
        cx = cosf(*(f32 *)((u8 *)arg0 + 0) * D_800A2FDC);
        sx = sinf(*(f32 *)((u8 *)arg0 + 0) * D_800A2FE0);
        cy = cosf(*(f32 *)((u8 *)arg0 + 4) * D_800A2FE4);
        sy = sinf(*(f32 *)((u8 *)arg0 + 4) * D_800A2FE8);
        cz = cosf(*(f32 *)((u8 *)arg0 + 8) * D_800A2FEC);
        sz = sinf(*(f32 *)((u8 *)arg0 + 8) * D_800A2FF0);
        if (speed < 0) {
            dist = -dist;
        }
        fdist = dist;
        m = cx * sy;
        *(s16 *)((u8 *)arg0 + 0x10) = x + fdist * ((m * cz) + (sx * sz));
        *(s16 *)((u8 *)arg0 + 0x12) = y + fdist * ((m * sz) - (sx * cz));
        *(s16 *)((u8 *)arg0 + 0x14) = z + fdist * cx * cy;
    }
    *(u8 *)((u8 *)arg0 + 0x73) &= ~3;
    *(u8 *)((u8 *)arg0 + 0x73) |= state;
}

void func_15117D3C(struct Obj151152A8 *arg0, struct127 *arg1) {
    s32 temp = arg0->field_0x3C & 0xFFFF;
    if (arg1->interaction_state == 1) {
        if (arg0->field_0x4F & 0x4) {
            if (!(arg0->field_0x73 & 0x3) && !(arg0->field_0x73 & 0x4)) {
                if (temp == 0 || arg1->unk31C->unk57 == 1) {
                    arg0->field_0x73 &= 0xFFFC;
                    arg0->field_0x73 |= 0x2;
                }
            }
        }
    }
}

void func_15117DA4(struct Obj15117DA4 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4) {
    f32 angle;
    f32 s;
    f32 c;
    f32 temp_z;
    f32 temp_x;

    if (arg0->unk54 == 0x8006) {
        if (D_800BE9F0 == 0x35) {
            arg0 = (struct Obj15117DA4 *)func_151149AC(0xFD);
            func_1511F31C((struct Struct1511F31CArg *)arg0);
            arg2 = D_800A2FF4;
            arg1 = 0.0f;
            arg3 = 0.0f;
        }
    }

    angle = arg0->unk0 * D_800A2FF8;
    s = sinf(angle);
    c = cosf(angle);
    arg4[0] = arg1;
    arg4[1] = (arg2 * c) - (arg3 * s);
    arg4[2] = (arg2 * s) + (arg3 * c);

    angle = arg0->unk4 * D_800A2FFC;
    s = sinf(angle);
    c = cosf(angle);
    temp_x = arg4[0];
    temp_z = arg4[2];
    arg4[0] = (temp_z * s) + (temp_x * c);
    arg4[2] = (temp_z * c) + (-temp_x * s);

    arg4[0] = *(volatile f32 *)&arg4[0] + arg0->unk10;
    arg4[1] = *(volatile f32 *)&arg4[1] + arg0->unk12;
    arg4[2] = *(volatile f32 *)&arg4[2] + arg0->unk14;
}

struct F7Cw {
    u8 p0[0x17];
    u8 unk17;
    u8 p18[0xF];
    u8 unk27;
    u8 p28[0x43];
    u8 unk6B;
    s16 unk6C;
    s16 unk6E;
    s16 unk70;
};

struct F7Tg {
    s32 p0;
    u8 p4[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 p20[0x45];
    u8 unk65;
    u8 p66[0x80];
    s16 unkE6;
    u8 pE8[0x1C];
    u8 unk104;
    u8 p105[0x216];
    struct F7Cw *w;
    u8 p320[0xC];
};

struct F7O {
    f32 unk0;
    u8 p4[0x28];
    f32 ox;
    f32 oy;
    f32 oz;
    u8 p38[0x17];
    u8 unk4F;
    u8 p50[0x23];
    u8 unk73;
    u16 unk74;
    u8 p76[6];
    s32 unk7C;
    f32 unk80;
    f32 unk84;
};

extern void func_15060A30(s32, void *);

void func_15117F3C(struct F7O *o) {
    f32 limit;
    s32 d1;
    f32 f84;
    f32 f80;
    struct F7Tg *p;
    struct F7Cw *w;
    s32 i;
    f32 dx;
    f32 dy;
    s32 flag8;
    s32 sound;
    f32 thr;
    s32 d2;
    struct F7Tg *target;
    s32 d3;
    f32 x;
    f32 y;
    f32 z;
    f32 pos[3];
    s32 d4;
    s32 mode;
    s32 t1;
    s32 flag4;
    s32 timer;
    f32 dz;
    f32 dist;

    limit = 40.0;
    thr = 40000.0f;
    sound = 0x4B8;
    mode = o->unk73 & 3;
    flag4 = o->unk73 & 4;
    flag8 = o->unk73 & 8;
    switch (D_800BE9F0) {
    case 1:
    case 41:
        x = -13.0f;
        y = 76.0f;
        z = 354.0f;
        thr = 10000.0f;
        break;
    case 11:
        x = 70.0f;
        y = 120.0f;
        z = 329.0f;
        sound = 0x4BA;
        flag8 = 8;
        break;
    case 23:
        x = 0.0f;
        y = 125.0f;
        z = 322.0f;
        break;
    case 40:
        limit = 55.0f;
        x = 0.0f;
        y = 99.0f;
        z = 168.0f;
        flag8 = 8;
        break;
    case 53:
        limit = 20.0f;
        x = 0.0f;
        y = -328.0f;
        z = 549.0f;
        break;
    case 47:
        limit = 90.0f;
        thr = 5625.0f;
        x = 0.0f;
        y = 61.0f;
        z = 104.0f;
        flag8 = 8;
        break;
    case 60:
        limit = 30;
        thr = 10000.0f;
        x = 22.0f;
        y = 13.0f;
        z = 419.0f;
        flag8 = 8;
        break;
    case 65:
        limit = 45.0f;
        if (flag4 == 4) {
            thr = -1.0f;
        }
        x = 0.0f;
        y = 25.0f;
        z = 336.0f;
        flag8 = 8;
        break;
    case 64:
        limit = 45.0f;
        x = 0.0f;
        y = 47.0f;
        z = 72.0f;
        flag8 = 8;
        thr = 10000.0f;
        break;
    default:
        return;
    }
    x *= o->ox;
    y *= o->oy;
    z *= o->oz;
    thr *= o->oz * o->oz;
    if (o->unk7C == 0) {
        o->unk7C = 2;
        if (mode == 3) {
            o->unk0 = limit;
        }
    }
    timer = (o->unk7C >> 8) & 0xFF;
    f80 = o->unk80;
    f84 = o->unk84;
    t1 = o->unk7C & 1;
    if (t1) {
    }
    func_15117DA4((struct Obj15117DA4 *)o, x, y, z, pos);
    target = 0;
    if (mode != 3 && mode != 1) {
        for (i = 0; i < D_8008FD8C; i++) {
            p = &((struct F7Tg *)D_800CC2D0)[i];
            if (p->unk65) {
                continue;
            }
            if (p->unk104) {
                continue;
            }
            w = p->w;
            if (w->unk17) {
                continue;
            }
            if (w->unk27) {
                continue;
            }
            if (w->unk6B >= 4) {
                continue;
            }
            if (p->y + 40.0f < pos[1]) {
                dy = (p->y + (f32)p->unkE6) - pos[1];
                dz = p->z - pos[2];
                dx = p->x - pos[0];
                dist = dz * dz + (dx * dx + dy * dy);
                if (dist) {
                }
                if (dist < thr) {
                    target = p;
                    break;
                }
            }
        }
    }
    if (target == 0 && mode == 2) {
        for (i = 0; i < D_8008FD8C; i++) {
            p = &((struct F7Tg *)D_800CC2D0)[i];
            if (p->w->unk6B != 0) {
                target = p;
                break;
            }
        }
    }
    if (target != 0 || mode == 2) {
        if (target != 0 && t1 == 0) {
            func_15060A30(0x16B, target);
        }
        if (flag4 == 0) {
            mode = 2;
            if (t1 == 0) {
                timer = 0x32;
                t1++;
                f80 = 0.0f;
            }
            if (target != 0) {
                if (timer != 0) {
                    target->w->unk6B = 1;
                } else {
                    target->w->unk6B = 2;
                }
            }
            if (timer != 0) {
                if (D_800BE9E4 < timer) {
                    timer -= D_800BE9E4;
                } else {
                    timer = 0;
                }
            } else {
                f84 += f80;
                if (30.0f < f84) {
                    f84 = 30.0f;
                }
                f80 += 0.02999999933f;
                if (0.200000003f < f80) {
                    if (o->unk74 == 0) {
                        func_15114D24((struct Obj15116BAC *)o, sound, 0x7D00, 0x5DC, 0xDAC, 0);
                    }
                }
                o->unk0 += f84;
                if (limit <= o->unk0) {
                    o->unk0 = limit;
                    f84 = 0.0f;
                    f80 = 0.0f;
                    mode = 3;
                    t1 = 0;
                    func_15114D24((struct Obj15116BAC *)o, 0x14, 0x7D00, 0x5DC, 0xDAC, 4);
                    if (target != 0) {
                        target->w->unk6B = 0;
                    }
                }
            }
            if (target != 0) {
                func_15117DA4((struct Obj15117DA4 *)o, x, y, z, pos);
                target->w->unk6C = (s16)pos[0];
                target->w->unk6E = (s16)pos[1];
                target->w->unk70 = (s16)pos[2];
            }
        } else {
            if (target != 0 && (w = target->w) != 0) {
                if (t1 == 0 && w->unk6B == 0) {
                    timer = 0x32;
                    t1++;
                }
                if (timer != 0) {
                    w->unk6B = 3;
                } else if (w->unk6B == 3) {
                    w->unk6B = 4;
                    t1 = 0;
                }
                if (timer != 0) {
                    if (D_800BE9E4 < timer) {
                        timer -= D_800BE9E4;
                    } else {
                        timer = 0;
                    }
                }
            }
        }
        if (target != 0) {
            if (target->w != 0) {
                func_15117DA4((struct Obj15117DA4 *)o, x, y, z, pos);
                target->w->unk6C = (s16)pos[0];
                target->w->unk6E = (s16)pos[1];
                target->w->unk70 = (s16)pos[2];
            }
        }
    } else if (flag8 == 0 || mode == 1) {
        if (0.0f < o->unk0) {
            if (t1 == 0) {
                f80 = 0.1000000015f;
                t1++;
            }
            mode = 1;
            f84 += 0.1000000015f;
            if (30.0f < f84) {
                f84 = 30.0f;
            }
            f80 += 0.05000000075f;
            o->unk0 = o->unk0 - f84;
            if (o->unk0 <= 0.0f) {
                o->unk0 = 0.0f;
                t1 = 0;
                mode = 0;
                f80 = 0.0f;
                f84 = 0.0f;
            }
        }
        if (target != 0) {
            target->w->unk6B = 0;
        }
    }
    o->unk73 &= 0xFFFC;
    o->unk73 |= mode;
    o->unk7C &= 0xFE;
    o->unk7C |= timer << 8;
    o->unk7C |= t1;
    o->unk80 = f80;
    o->unk84 = f84;
    o->unk4F &= 0xFF9F;
    if (mode == 2 || (mode == 0 && timer != 0)) {
        o->unk4F |= 0x20;
    } else {
        o->unk4F |= 0;
    }
}

s32 func_151189AC(void *arg0, s32 arg1);

s32 func_1511896C(void *arg0) {
    return func_151189AC(arg0, 0);
}

s32 func_1511898C(void *arg0) {
    return func_151189AC(arg0, 3);
}

extern void func_15173C60(s32, s32);
extern void func_15173C90(s32, s32, s32);
extern void func_1518804C(s32, f32);
extern void func_1511F990(void *, s32);
extern s32 func_15022B08(s32, s32);
extern struct SoundEntry15116BAC D_80089262[];
extern f32 D_800A3138;
extern f32 D_800A313C;
extern f32 D_800A3140;
extern f32 D_800A3144;

s32 func_151189AC(void *arg0, s32 arg1) {
    f32 speed;
    f32 target;
    f32 angle;
    f32 remaining;
    f32 damping;
    f32 accel;
    s32 mode;
    s32 index;
    s32 openAngle;
    s32 row;
    s32 sound;
    s32 slow;
    f32 scale;

    sound = 0;
    slow = 0;
    func_1511F990(arg0, arg1);
    row = (*(s32 *)(((u8 *)arg0) + 0x3C) & 0xFC00) >> 10;
    openAngle = (*(s32 *)(((u8 *)arg0) + 0x3C) << 22) >> 22;
    mode = ((u8 *)arg0)[0x73] & 3;
    target = openAngle;
    angle = *(f32 *)(((u8 *)arg0) + 4);
    speed = *(f32 *)(((u8 *)arg0) + 0x84);
    if (mode == 0 || mode == 1) {
        target = 0.0f;
        if (mode == 1 && (((u8 *)arg0)[0x73] & 4)) {
            slow = 1;
        }
    }
    if (mode == 0 || mode == 3) {
        speed = 0.0f;
        angle = target;
    }
    if (angle != target || speed != 0.0f) {
        if (*(f32 *)(((u8 *)arg0) + 0x7C) == 0.0f) {
            if (slow) {
                *(f32 *)(((u8 *)arg0) + 0x7C) = D_800A3138;
            } else {
                *(f32 *)(((u8 *)arg0) + 0x7C) = D_800A313C;
            }
        }
        damping = *(f32 *)(((u8 *)arg0) + 0x7C);
        if (*(f32 *)(((u8 *)arg0) + 0x80) == 0.0f) {
            if (slow) {
                *(f32 *)(((u8 *)arg0) + 0x80) = D_800A3140;
            } else {
                *(f32 *)(((u8 *)arg0) + 0x80) = D_800A3144;
            }
        }
        accel = *(f32 *)(((u8 *)arg0) + 0x80);
        if (speed == 0.0f) {
            if (target == 0.0f) {
                sound = D_80089262[row].unk0;
            } else {
                sound = D_80089260[row].unk0;
            }
        }
        angle += speed;
        remaining = func_15048A70(angle, target);
        if (fabsf(remaining) < fabsf(speed)) {
            speed = -speed * damping;
            if (fabsf(speed) < 1.0f) {
                speed = 0.0f;
                angle = target;
                if (target == 0.0f) {
                    mode = 0;
                    sound = D_80089260[row].unk6;
                } else {
                    mode = 3;
                    sound = D_80089260[row].unk4;
                }
            } else {
                accel *= 0.5f;
                *(f32 *)(((u8 *)arg0) + 0x80) = accel;
            }
        } else if (remaining > 0.0f) {
            speed += accel;
            if (speed > 5.0f) {
                speed = 5.0f;
            }
        } else {
            speed -= accel;
            if (speed < -5.0f) {
                speed = -5.0f;
            }
        }
        if ((remaining < 0 ? -remaining : remaining) > 30.0f && remaining * speed < 0.0f) {
            speed = 0.0f;
        }
        if (angle < -180.0f) {
            angle += 360.0f;
        } else if (angle >= 180.0f) {
            angle -= 360.0f;
        }
        ((u8 *)arg0)[0x73] &= ~3;
        ((u8 *)arg0)[0x73] |= mode;
        if (sound != 0) {
            func_15114D24((struct Obj15116BAC *)arg0, sound, 0x5DC0, 0x9C4, 0x1194, 0);
        }
    }
    *(f32 *)(((u8 *)arg0) + 0x84) = speed;
    if (D_800BE9B4 != 0 || angle != *(f32 *)(((u8 *)arg0) + 4)) {
        if (D_800C35EA == 1 && func_15022B08(((s32)arg0 - (s32)D_800DBEF4) / 160, 0) != 0) {
            angle = *(f32 *)(((u8 *)arg0) + 4);
        } else {
            *(f32 *)(((u8 *)arg0) + 4) = angle;
        }
        index = (*(s32 *)(((u8 *)arg0) + 0x3C) >> 24) & 0xFF;
        if (D_800BE9F0 == 2) {
            func_1000E75C(255.0f - fabsf(((angle - (f32)openAngle) * 255.0f) / (f32)openAngle));
            return;
        }
        if (index != 0) {
            scale = 1.0f - fabsf((fabsf(angle) - (openAngle < 0 ? -openAngle : openAngle)) / (f32)openAngle);
            func_1518804C(index - 1, scale);
            func_15173C60(scale * 255.0f, index - 1);
            if (D_800BE9F0 == 1 && index - 1 == 0) {
                func_15173C90(scale * 255.0f, 0, 0xF8);
            }
        }
    }
}

extern void func_1511F990(void *, s32);
extern void func_15173C60(s32, s32);
extern void func_1518804C(s32, f32);
extern void func_151669A0(s32, s32, s32, f32, u8, s32);
extern f32 D_800A3148;
extern f32 D_800A314C;
extern f32 D_800A3150;
extern f32 D_800A3154;
extern f32 D_800A3158;
void func_15118F24(u8 *arg0) {
    f32 speed;
    f32 target;
    f32 angle;
    f32 remaining;
    f32 damping;
    f32 accel;
    s32 mode;
    s32 index;
    s32 openAngle;
    f32 scale;

    func_1511F990(arg0, 1);
    mode = arg0[0x73] & 3;
    openAngle = *(s16 *)(arg0 + 0x3E);
    target = openAngle;
    angle = *(f32 *)(arg0 + 8);
    speed = *(f32 *)(arg0 + 0x84);
    if (mode == 0 || mode == 1) {
        target = 0.0f;
    }
    if (mode == 0 || mode == 3) {
        speed = 0.0f;
        angle = target;
    }
    if (angle != target || speed != 0.0f) {
        if (*(f32 *)(arg0 + 0x7C) == 0.0f) {
            *(f32 *)(arg0 + 0x7C) = D_800A3148;
        }
        damping = *(f32 *)(arg0 + 0x7C);
        if (*(f32 *)(arg0 + 0x80) == 0.0f) {
            *(f32 *)(arg0 + 0x80) = D_800A314C;
        }
        if (angle > 90.0f) {
            accel = *(f32 *)(arg0 + 0x80) * (180.0f - angle);
        } else {
            accel = *(f32 *)(arg0 + 0x80) * angle;
        }
        if (speed == 0.0f) {
            if (target == 0.0f) {
                accel += D_800A3150;
            } else {
                func_10010F88(0x4BA, 0x5DC0, 0, 0, 0, *(s16 *)(arg0 + 0x10), *(s16 *)(arg0 + 0x12),
                              *(s16 *)(arg0 + 0x14), 0xC8, 0x9C4);
                accel += D_800A3154;
            }
        }
        angle += speed;
        remaining = func_15048A70(angle, target);
        if (fabsf(remaining) < fabsf(speed)) {
            speed = -speed * damping;
            if (fabsf(speed) < *(f32 *)(arg0 + 0x80) * D_800A3158) {
                speed = 0.0f;
                angle = target;
                if (target == 0.0f) {
                    mode = 0;
                } else {
                    mode = 3;
                    func_10010F88(0x4BB, 0x5DC0, 0, 0, 0, *(s16 *)(arg0 + 0x10),
                                  *(s16 *)(arg0 + 0x12), *(s16 *)(arg0 + 0x14), 0xC8, 0x9C4);
                }
            } else {
                *(f32 *)(arg0 + 0x80) *= 0.5f;
            }
            func_151669A0(*(s16 *)(arg0 + 0x10), *(s16 *)(arg0 + 0x12), *(s16 *)(arg0 + 0x14), 0.15f,
                          0xFF, 0);
        } else if (remaining > 0.0f) {
            speed += accel;
            if (speed > 10.0f) {
                speed = 10.0f;
            }
        } else {
            speed -= accel;
            if (speed < -10.0f) {
                speed = -10.0f;
            }
        }
        if (angle < 0.0f) {
            angle += 360.0f;
        } else if (angle >= 360.0f) {
            angle -= 360.0f;
        }
        arg0[0x73] &= ~3;
        arg0[0x73] |= mode;
    }
    *(f32 *)(arg0 + 0x84) = speed;
    if (D_800BE9B4 != 0 || angle != *(f32 *)(arg0 + 8)) {
        *(f32 *)(arg0 + 8) = angle;
        index = (*(s32 *)(arg0 + 0x3C) >> 24) & 0xFF;
        if (index != 0 && openAngle != 0) {
            scale = 1.0f - fabsf((angle - (f32)openAngle) / (f32)openAngle);
            func_1518804C(index - 1, scale);
            func_15173C60(scale * 255.0f, index - 1);
        }
    }
}

struct Obj151193AC_0 {
    char pad0[0x73];
    u8 unk73;
};

struct Obj151193AC_1 {
    s32 unk0;
    char pad4[0x61];
    u8 unk65;
};

void func_151193AC(struct Obj151193AC_0 *arg0, struct Obj151193AC_1 *arg1) {
    if (arg1->unk0 == 1) {
        if (arg1->unk65 == 0) {
            if (!(arg0->unk73 & 0x3) && !(arg0->unk73 & 0x4)) {
                arg0->unk73 &= 0xFFFC;
                arg0->unk73 |= 0x2;
            }
        }
    }
}

struct Obj151193F4 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x28];
    s32 unk3C;
    u8 pad40[0x33];
    u8 unk73;
    u8 pad74[0x8];
    s32 unk7C;
    u32 unk80;
    s32 unk84;
};

void func_151193F4(struct Obj151193F4 *arg0) {
    s32 speed;
    s32 target;
    s32 diff;
    s32 scale;
    s32 base;
    u32 mul;

    if (arg0->unk84 == 0) {
        arg0->unk84 = arg0->unk12 | 0x80000000;
    }
    speed = arg0->unk7C;
    if (speed != 0) {
        mul = arg0->unk80;
        base = (s16)(arg0->unk84 & 0xFFFF);
        scale = (u16)(arg0->unk3C >> 16);
        target = (s32)(scale * mul) / 256 + base;
        arg0->unk73 &= ~3;
        if (target != arg0->unk12) {
            diff = arg0->unk12 - target;
            if (diff < 0) {
                if (-diff < speed) {
                    arg0->unk12 = target;
                } else {
                    arg0->unk12 = arg0->unk12 + speed;
                }
            } else {
                if (diff < speed) {
                    arg0->unk12 = target;
                } else {
                    arg0->unk12 = arg0->unk12 - speed;
                }
            }
            /* state bits = 0 (moving): the original keeps this RMW, golden has lbu/sb here */
            arg0->unk73 |= 0;
        } else {
            arg0->unk73 |= 3;
        }
    }
}


struct Obj151194D4Node {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
    char pad8[8];
};

struct Obj151194D4Cmd {
    s8 unk0;
    char pad1[3];
    u32 unk4;
};

struct Obj151194D4 {
    char pad0[0x16];
    u16 unk16;
    char pad18[4];
    struct Obj151194D4Cmd *unk1C;
    char pad20[8];
    struct Obj151194D4Node *unk28;
    char pad2C[0x28];
    u16 unk54;
    char pad56[0x2E];
    s32 unk84;
};

void func_151194D4(struct Obj151194D4 *arg0, struct Obj151194D4Node *arg1, s32 arg2, s32 arg3) {
    struct Obj151194D4Cmd *cmd;
    struct Obj151194D4Node *node;
    s32 base;
    s32 found;
    u32 i;
    u32 word;
    u32 bits;

    cmd = arg0->unk1C;
    while (cmd->unk0 != -0x21) {
        switch (cmd->unk0) {
            case 1:
                base = (cmd->unk4 & 0xFFFFFF) >> 4;
                break;
            case 5:
                word = cmd->unk4;
                found = 0;
                bits = word;
                for (i = 0; i < 3; i++) {
                    node = &arg0->unk28[(bits & 0xFF) / 10 + base];
                    if ((arg1->unk0 == node->unk0) && (arg1->unk2 == node->unk2) && (arg1->unk4 == node->unk4)) {
                        found = 1;
                        node->unk6 = arg3;
                    } else {
                        bits >>= 8;
                    }
                }
                if (found) {
                    bits = word;
                    for (i = 0; i < 3; i++) {
                        node = &arg0->unk28[(bits & 0xFF) / 10 + base];
                        if (node->unk6 == 0) {
                            if (arg2 < ((node->unk4 * node->unk4) + (node->unk0 * node->unk0))) {
                                node->unk6 = arg3;
                                func_151194D4(arg0, node, arg2, arg3);
                            }
                        }
                        bits >>= 8;
                    }
                }
                break;
        }
        cmd++;
    }
}

extern f32 func_150489B0(u8);
extern f32 D_800A315C;

void func_151196D4(struct Obj151194D4 *arg0) {
    struct Obj151194D4Node *nodes;
    struct Obj151194D4Node *node;
    f32 sn;
    f32 cs;
    f32 scale;
    s32 range;
    s32 count;
    s32 i;
    s32 sumX;
    s32 sumZ;
    s32 angle;
    s32 temp;
    u8 back;

    node = arg0->unk28;
    range = 0x64;
    if ((arg0->unk54 == 0x21) || (arg0->unk54 == 0x22)) {
        range = 0x190;
    }
    arg0->unk84 = 1;
    count = 0;
    for (i = 0; i < arg0->unk16; i++) {
        if (node->unk6 == 0) {
            temp = node->unk0;
            temp = (node->unk4 * node->unk4) + (temp * temp);
            if (range < temp) {
                count++;
                node->unk6 = count;
                func_151194D4(arg0, node, range, count);
            }
        }
        node++;
    }

    if (count != 0) {
        scale = D_800A315C;
        do {
            nodes = arg0->unk28;
            node = nodes;
            sumX = 0;
            sumZ = 0;
            for (i = 0; i < arg0->unk16; i++) {
                if (count == node->unk6) {
                    sumX += node->unk0;
                    sumZ += node->unk4;
                }
                node++;
            }
            if ((sumX != 0) || (sumZ != 0)) {
                node = nodes;
                angle = (u8)(s32)(func_150484A0((f32)sumX, (f32)sumZ) * scale);
                if (count < angle) {
                    back = -angle;
                    cs = func_15048A40(back);
                    sn = func_150489B0(back);
                    for (i = 0; i < arg0->unk16; i++) {
                        if (count == node->unk6) {
                            temp = node->unk0;
                            node->unk6 = (((count - 1) & 3) << 8) | angle;
                            node->unk0 = (node->unk4 * cs) + (temp * sn);
                            node->unk4 = (node->unk4 * sn) - (temp * cs);
                        }
                        node++;
                    }
                }
            }
            count--;
        } while (count != 0);
    }
}

typedef struct {
    s16 x, y, z;
    u16 group;
    u8 pad8[8];
} Game144C70Vertex;

typedef struct {
    u8 pad0[4];
    f32 unk4;
    u8 pad8[8];
    s16 x, y, z;
    u16 count;
    u8 pad18[8];
    Game144C70Vertex *outputs[2];
    Game144C70Vertex *vertices;
    f32 scaleX, scaleY, scaleZ;
    u8 pad38[0x1C];
    u16 kind;
    u8 pad56[0x26];
    s32 timer;
    f32 phase;
    s32 active;
} Game144C70VertexState;

extern s32 func_151EF610(void);
struct127 *func_1505EFD0(s32);
extern f32 D_800A3160;
extern f32 D_800A3164;
extern f32 D_800A3168;
extern f32 D_800A316C;

void func_15119938(Game144C70VertexState *arg0) {
    f32 dist;
    Game144C70Vertex *vertex;
    Game144C70Vertex *out;
    s32 offset;
    s32 i;
    f32 *pair;
    struct127 *player;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 x;
    f32 y;
    f32 z;
    f32 sine;
    f32 rot[4][2];
    f32 rotCos;
    f32 rotSin;
    f32 cosine;
    f32 angle;

    vertex = arg0->vertices;
    out = arg0->outputs[D_800BE9C0];
    posX = arg0->x;
    posY = arg0->y;
    posZ = arg0->z;
    scaleX = arg0->scaleX;
    scaleY = arg0->scaleY;
    scaleZ = arg0->scaleZ;
    offset = arg0->unk4 * D_800A3160;
    if (arg0->active == 0) {
        func_151196D4((struct Obj151194D4 *)arg0);
    }
    if (arg0->timer >= 0x400) {
        arg0->phase += ((f32)(arg0->timer - 0x400) - arg0->phase) * D_800A3164 * D_800BE9A4;
        if (arg0->timer != 0x420) {
            arg0->timer--;
            if (arg0->timer == 0x400) {
                arg0->timer = 0;
                arg0->phase = 0.0f;
            }
        }
    } else {
        arg0->phase += (0.01953125f + 0.00390625f * (func_151EF610() % 6)) * D_800BE9A4;
        arg0->timer -= func_151EF610() % 3;
        if (arg0->timer < 0x14) {
            arg0->timer = 0xEC;
        }
    }
    for (i = 0; i < 4; i++) {
        if (arg0->timer >= 0x400) {
            rot[i][0] = func_15048A40(arg0->phase);
            rot[i][1] = func_150489B0(arg0->phase);
        } else {
            angle = arg0->phase * D_800A3168 - D_800A316C * i;
            if (angle <= 0.0f) {
                angle = 5.0f;
            } else {
                sine = func_15048A40(arg0->timer >> 1);
                angle = sinf(angle) * (2.0f * sine) + 5.0f;
            }
            rot[i][0] = func_15048A40(angle);
            rot[i][1] = func_150489B0(angle);
        }
    }
    if (arg0->timer != 0x420) {
        player = func_1505EFD0(0);
        if (player != NULL) {
            x = player->x_position - arg0->x;
            y = player->y_position - arg0->y;
            z = player->z_position - arg0->z;
            dist = x * x + y * y + z * z;
            if (dist < 3072.0f) {
                if (player->unk31C->unk57 == 1) {
                    arg0->timer = 0x420;
                } else {
                    arg0->timer = 0x41F;
                }
            }
        }
    }
    for (i = 0; i < arg0->count; i++) {
        if (vertex->group != 0) {
            sine = func_15048A40(vertex->group + offset);
            cosine = func_150489B0(vertex->group + offset);
            pair = rot[vertex->group >> 8];
            rotCos = pair[0];
            rotSin = pair[1];
            z = vertex->y * rotCos + vertex->z * rotSin;
            y = vertex->y * rotSin - vertex->z * rotCos;
            if (y < 1.0f) {
                z -= y;
                y = 1.0f;
            }
            x = vertex->x * cosine + z * sine;
            z = z * cosine - vertex->x * sine;
        } else {
            x = vertex->x;
            y = vertex->y;
            z = vertex->z;
        }
        out->x = x * scaleX + posX;
        out->y = y * scaleY + posY;
        out->z = z * scaleZ + posZ;
        out++;
        vertex++;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_15119FC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511A410.s")

typedef struct {
    s32 *unk0;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 unk7;
    u8 unk8;
    u8 pad9[3];
} Struct1511A494Entry;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[0x34];
    u16 unk54;
} Struct1511A494Arg;

extern Struct1511A494Entry D_80089324[];
extern Struct1511A494Entry D_80089444[];
extern s32 func_1511A410(s32, s32 *);
extern s32 func_1510D0EC(s32, s32 *, s32, s32);
extern void func_1510D874(void *, s32, s32, s32, s32);

void func_1511A494(Struct1511A494Arg *arg0, s32 *arg1, s32 *arg2) {
    s32 pad64;
    s32 sp60;
    s32 i;
    Struct1511A494Entry *found;
    s32 id;
    s32 frame;
    s32 dir;
    s32 sp48;
    s32 type;
    s32 width;
    s32 offset;
    s32 x;

    found = NULL;
    for (i = 0; i < 24; i++) {
        id = D_80089324[i].unk5;
        if (D_80089324[i].unk6 != -1) {
            if (D_800BE9F0 != D_80089324[i].unk6) {
                continue;
            }
            if (id != 0xFF) {
                id |= 0x8000;
            }
        }
        if (id == 0xFF || id == arg0->unk54) {
            found = &D_80089324[i];
            break;
        }
    }
    if (found != NULL) {
        if (*arg1 == 0) {
            x = func_1511A410(arg0->unk1C, &sp48);
            *arg1 = (sp48 << 16) | x;
        } else {
            sp48 = (u16)(*arg1 >> 16);
        }
        dir = (*arg2 >> 16) & 0xFFFF;
        dir = (s16)dir;
        frame = *arg2 & 0xFFFF;
        if (dir == 0) {
            dir = 1;
        }
        frame += dir;
        if (found->unk8 != 0) {
            if (frame < 0) {
                dir = 1;
                frame = found->unk7;
            } else if (frame >= found->unk4 * found->unk7) {
                dir = -1;
                frame = (found->unk4 - 1) * found->unk7;
            }
        } else {
            if (frame >= found->unk4 * found->unk7) {
                frame -= found->unk4 * found->unk7;
                dir = 1;
            }
        }
        x = found->unk0[frame / found->unk7];
        offset = 0;
        if (D_800BE9F0 == 6 || D_800BE9F0 == 0x3B) {
            type = 3;
        } else {
            type = 0x3E;
        }
        width = func_1510D0EC(x, &sp60, type, 0);
        if (sp48 != 0) {
            offset = sp60 + width - 0x20;
        }
        func_1510D874(arg0, width, offset, 4, 5);
        *arg2 = (dir << 16) | frame;
    }
}

extern void func_15116110(void *);

void func_1511A6FC(s32 *arg0) {
    if (arg0[0xF] != 0) {
        func_15116110(arg0);
    }
    func_1511A494(arg0, (u8 *)arg0 + 0x80, (u8 *)arg0 + 0x84);
}

extern u8 D_800C35EA;
extern struct131 *D_800DBEF4;
extern s32 func_15022B08(s32, s32);
extern void func_151162D4(void *);

void func_1511A738(struct131 *arg0) {
    if (D_800C35EA == 1) {
        if (func_15022B08(arg0 - D_800DBEF4, 0) != 0) {
            *(s32 *)((u8 *)arg0 + 0x7C) = 0;
            *(f32 *)((u8 *)arg0 + 0x18) = 0.0f;
        }
    }
    func_151162D4(arg0);
    func_1511A494(arg0, (u8 *)arg0 + 0x80, (u8 *)arg0 + 0x84);
}

struct S_1511A7C0 {
    u8 pad0[0x4];
    f32 *unk4;
    u8 pad8[0x8];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
};
struct A_1511A7C0 {
    u8 pad0[0x16];
    u16 unk16;
    u8 pad18[0x68];
    struct S_1511A7C0 *unk80;
};
void func_1511A7C0(struct A_1511A7C0 *arg0) {
    struct S_1511A7C0 *v0;
    s32 i;
    v0 = arg0->unk80;
    v0->unk1E = 1;
    v0->unk10 = 0.0f;
    v0->unk1C = 0;
    v0->unk1D = 7;
    v0->unk14 = 260.0f;
    v0->unk18 = 100.0f;
    for (i = 0; i < arg0->unk16; i++) {
        v0->unk4[i] = 0.0f;
    }
}

typedef struct {
    u8 *bytes;
    f32 *values;
    f32 *rates;
    f32 *scales;
    f32 field_10;
    f32 field_14;
    f32 field_18;
    u8 field_1C;
    u8 field_1D;
    u8 field_1E;
    s32 field_20;
    s32 field_24;
} Game147C70Output;

typedef struct {
    u8 pad0[0x16];
    u16 value_count;
    u8 pad18[8];
    Vtx_t *lists[2];
    Vtx_t *vertices;
    u8 pad2C[0x10];
    s32 packed3C;
    u8 pad40[0x33];
    u8 flags73;
    u8 pad74[0xC];
    Game147C70Output *output;
    s32 packed84;
} Game147C70State;

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_1511A838(Game147C70State *arg0) {
    s32 i;
    s32 j;
    f32 matrix[16];
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    f32 inv;
    f32 factor;
    f32 offsetX;
    f32 offsetZ;
    s32 old;
    u8 shift[4];
    u8 *bytes;
    f32 *values;
    f32 *rates;
    f32 *scales;
    Vtx_t *vertices;
    Game147C70Output *output;
    s32 offsetY;
    f32 radius;
    f32 dist;
    u8 angle;
    s32 lane;
    Vtx_t *list;
    f32 dx;
    f32 dz;

    vertices = arg0->vertices;
    scale = arg0->packed3C & 0xFF;
    if (arg0->output == NULL) {
        arg0->output = allocate_memory(0x28, 1, 0, 0);
        output = arg0->output;
        output->bytes = bytes = allocate_memory(arg0->value_count, 1, 0, 0);
        output->values = allocate_memory(arg0->value_count * 4, 1, 0, 0);
        values = output->values;
        output->rates = rates = allocate_memory(arg0->value_count * 4, 1, 0, 0);
        output->scales = scales = allocate_memory(arg0->value_count * 4, 1, 0, 0);
        radius = (arg0->packed3C >> 16) & 0xFFFF;
        if (radius == 0.0f) {
            inv = 1.0f;
        } else {
            inv = 1.0f / radius;
        }
        for (i = 0; i < arg0->value_count; i++) {
            bytes[i] = (u32)func_150ADA20() % 255;
            dx = vertices[i].ob[0];
            dz = vertices[i].ob[2];
            dx *= dx;
            dz *= dz;
            dist = sqrtf(dx + dz);
            scales[i] = (radius - dist) * inv;
            rates[i] = (radius - dist * 1.2f) * inv;
            if (rates[i] < 0.0f) {
                rates[i] = 0.0f;
            }
        }
        for (i = 0; i < arg0->value_count; i++) {
            bytes[i] = func_150ADA20();
            for (j = 0; j < arg0->value_count; j++) {
                if (vertices[i].ob[0] == vertices[j].ob[0] && vertices[i].ob[1] == vertices[j].ob[1] && vertices[i].ob[2] == vertices[j].ob[2]) {
                    bytes[j] = bytes[i];
                }
            }
        }
        output->field_10 = 0.0f;
        output->field_1E = 0;
        for (i = 0; i < arg0->value_count; i++) {
            values[i] = 0.0f;
        }
        output->field_20 = 0;
        output->field_24 = 0;
    }
    bytes = arg0->output->bytes;
    values = arg0->output->values;
    rates = arg0->output->rates;
    output = arg0->output;
    scales = arg0->output->scales;
    if ((arg0->flags73 & 3) == 3) {
        func_1511A7C0((void *)arg0);
        arg0->flags73 &= ~3;
    }
    if (output->field_1E != 0) {
        old = output->field_1C;
        output->field_1C += output->field_1D * D_800BE9E4;
        if (output->field_14 < output->field_18) {
            factor = 0.5f;
        } else {
            factor = 0.8f;
        }
        if (output->field_1C < old || (output->field_1C >= 0x81 && old < 0x81)) {
            output->field_14 *= factor;
        }
        output->field_10 = func_15048A40(output->field_1C);
        output->field_10 = output->field_14 * output->field_10;
        if (output->field_14 <= 3.0f) {
            output->field_10 = 0.0f;
            output->field_1E = 0;
        }
        for (i = 0; i < arg0->value_count; i++) {
            f32 delta = output->field_10 - values[i];

            values[i] += delta * rates[i];
        }
    }
    func_1511490C((void *)matrix, (void *)arg0);
    list = arg0->lists[D_800BE9C0];
    shift[0] = arg0->packed84 & 0xFF;
    shift[1] = (arg0->packed84 >> 8) & 0xFF;
    shift[2] = (arg0->packed84 >> 16) & 0xFF;
    shift[3] = (arg0->packed84 >> 24) & 0xFF;
    offsetY = 0;
    for (i = 0; i < arg0->value_count; i++, list++, vertices++) {
        if (!(list->flag & 0x4000)) {
            lane = bytes[i] & 3;
            angle = bytes[i] + shift[lane];
            if (bytes[i] & 0x80) {
                angle = -angle;
            }
            offsetX = func_15048A40(angle) * scale;
            offsetZ = func_150489B0(angle) * scale;
            if (output->field_1E != 0) {
                offsetY = values[i] * scales[i];
            }
            func_150A7960(matrix, vertices->ob[0] + offsetX, vertices->ob[1] + offsetY, vertices->ob[2] + offsetZ, &x, &y, &z);
        } else {
            func_150A7960(matrix, vertices->ob[0], vertices->ob[1], vertices->ob[2], &x, &y, &z);
        }
        list->ob[0] = x;
        list->ob[1] = y;
        list->ob[2] = z;
    }
    shift[0] += 1;
    shift[1] += 2;
    shift[2] += 3;
    shift[3] += 4;
    arg0->packed84 = shift[0] | (shift[1] << 8) | (shift[2] << 16) | (shift[3] << 24);
    func_1511A494((void *)arg0, &output->field_20, &output->field_24);
}

struct Obj1511AF30 {
    f32 pos;
    u8 pad4[0x38];
    s32 level;
    u8 pad40[0x3C];
    f32 base;
    f32 wobble;
    f32 wobbleTarget;
};

extern f32 D_800A3188;
extern f32 D_800A318C;
extern f32 D_800A3190;
extern f32 D_800A3194;

void func_1511AF30(struct Obj1511AF30 *arg0) {
    f32 level;
    f32 target;
    f32 diff;
    s32 r;

    level = arg0->level;
    target = 75.0f * level * D_800A3188;
    if (target != arg0->base) {
        diff = target - arg0->base;
        if (fabsf(diff) < 1.0f) {
            arg0->base = target;
        } else {
            arg0->base += (diff < 0.0f) ? -1 : 1;
        }
        arg0->wobble = 0.0f;
    } else {
        r = (u32)func_150ADA20() % 1000;
        if (r < 500) {
            arg0->wobbleTarget = r * 5.0f * level * D_800A318C * D_800A3190;
        }
        diff = arg0->wobbleTarget - arg0->wobble;
        arg0->wobble += diff * D_800A3194;
    }
    arg0->pos = arg0->base + arg0->wobble;

}

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511B07C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511B51C.s")

struct Vtx1511B7D4 {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[0xA];
};

struct Obj1511B7D4 {
    u8 pad0[0x8];
    f32 unk8;
    u8 padC[0x4];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0xA];
    struct Vtx1511B7D4 *unk20[2];
    struct Vtx1511B7D4 *unk28;
    u8 pad2C[0x10];
    s32 unk3C;
    u8 pad40[0x3C];
    s32 unk7C;
    f32 unk80;
    f32 unk84;
    u8 pad88[0x18];
};

extern u8 D_800A2F70;
extern u8 D_800A2F71[];

void func_1511B7D4(struct Obj1511B7D4 *arg0) {
    u8 *table;
    s32 count;
    s32 dx;
    s32 dy;
    f32 angle;
    f32 s;
    f32 c;
    s32 x;
    s32 y;
    s32 z;
    s32 tmp;
    struct Obj1511B7D4 *objs[2];

    count = 0;
    if (arg0->unk3C == 0) {
        table = D_800A2F71;
        count = D_800A2F70;
        objs[0] = arg0 - 1;
        objs[1] = arg0 - 2;
    }
    if (arg0->unk80 != objs[0]->unk8 || arg0->unk84 != objs[1]->unk8) {
        arg0->unk80 = objs[0]->unk8;
        arg0->unk84 = objs[1]->unk8;
        arg0->unk7C = 2;
    }
    if (arg0->unk7C != 0) {
        arg0->unk7C--;
        while (count--) {
            dx = objs[table[1]]->unk10 - arg0->unk10;
            angle = -objs[table[1]]->unk8 * 0.01745329238f;
            dy = objs[table[1]]->unk12 - arg0->unk12;
            s = sinf(angle);
            c = cosf(angle);
            y = arg0->unk28[table[0]].y;
            x = arg0->unk28[table[0]].x;
            z = arg0->unk28[table[0]].z;
            y -= dy;
            x -= dx;
            tmp = (s32)(y * c - x * s);
            x = (s32)(y * s + x * c);
            y = tmp;
            x += dx;
            y += dy;
            x += arg0->unk10;
            y += arg0->unk12;
            z += arg0->unk14;
            arg0->unk20[D_800BE9C0][table[0]].x = x;
            arg0->unk20[D_800BE9C0][table[0]].y = y;
            arg0->unk20[D_800BE9C0][table[0]].z = z;
            table += 2;
        }
    }
}


struct Obj1511BA24 {
    u8 pad0[0x3C];
    s32 unk3C;
    u8 pad40[0x4A];
    u8 unk8A;
};

void func_1511BA24(struct Obj1511BA24 *arg0) {
    f32 sp1C;

    sp1C = 0.0f;
    func_15188010(arg0->unk3C, &sp1C);
    arg0->unk8A = sp1C * 255.0f;
    if (arg0->unk8A == 0) {
        arg0->unk8A = 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511BB04.s")

struct Obj1511BDF4 {
    u8  pad0[0x3F];
    u8  unk3F;
    u8  pad40[0x40];
    struct127 *unk80;
};

extern void func_1511BB04(struct Obj1511BDF4 *, f32, f32, f32, f32);

void func_1511BDF4(struct Obj1511BDF4 *arg0) {
    struct127 *temp;
    struct127 *sp;

    temp = arg0->unk80;
    if (temp != 0) {
        sp = temp;
    } else {
        sp = func_15083E90(arg0->unk3F);
    }
    if (sp != 0) {
        func_1511BB04(arg0, sp->x_position, sp->z_position, 1.0f, 1.0f);
    }
}

struct Obj1511BE5C {
    u8  pad0[0x4];
    f32 unk4;
    u8  pad8[0x8];
    s16 unk10;
    s16 unk12;
    s16 unk14;
};

extern f32 D_800A31E0;

void func_1511BE5C(struct Obj1511BE5C *arg0) {
    arg0->unk4 = func_150484A0((f32)arg0->unk10 - D_800DBFF0->unk2F8, (f32)arg0->unk14 - D_800DBFF0->unk300) * D_800A31E0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511BEBC.s")

void func_1511C540(void) {
}

struct Obj1511C548 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[0x74];
    f32 unk80;
    f32 unk84;
};

extern f32 D_800A31EC;
extern f32 D_800A31F0;

void func_1511C548(struct Obj1511C548 *arg0) {
    f32 z;
    f32 x;

    x = sinf(arg0->unk80);
    z = sinf(arg0->unk84);
    arg0->unk0 = x * 16.0f;
    arg0->unk8 = z * 16.0f;
    arg0->unk80 += D_800A31EC * D_800BE9A4;
    arg0->unk84 += D_800A31F0 * D_800BE9A4;
    arg0->unk80 = func_15144B68(arg0->unk80);
    {
        extern f32 func_15144B68(f32);

        arg0->unk84 = func_15144B68(arg0->unk84);
    }
    if (arg0->unk8 == 0.0f && arg0->unk84 == 0.0f) {
        arg0->unk0 = 7.5f;
        arg0->unk8 = 8.0f;
    }
}


struct Obj1511C638 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x26];
    s32 unk3C;
    u8 pad40[0x2E];
    u8 unk6E;
    u8 pad6F[4];
    u8 unk73;
    u8 pad74[8];
    s32 unk7C;
    s32 unk80;
    u8 pad84[6];
    u8 unk8A;
    u8 pad8B[0x15];
};

extern void func_15170F4C(struct Obj1511C638 *, u8, s32);
extern void func_150B2740(u8, u8);
extern s32 func_1000EC24();

void func_1511C638(struct Obj1511C638 *arg0) {
    s32 state;
    struct Obj1511C638 *sub;
    struct Obj1511C638 *other;

    sub = &arg0[1];
    state = arg0->unk73 & 3;
    if (D_800BE9F0 == 4) {
        other = &arg0[2];
    } else {
        other = NULL;
    }
    if (state == 1) {
        state = 0;
        arg0->unk8A = 0;
        sub->unk6E = 0;
        if (other != NULL) {
            other->unk6E = 0;
        }
        if (arg0->unk3C != 0xFF) {
            func_15173C60(0, arg0->unk3C);
        }
    }
    if (D_800BE9B4 != 0) {
        if (state == 3) {
            arg0->unk8A = 0xFF;
            sub->unk6E = 1;
            if (other != NULL) {
                other->unk6E = 1;
            }
        } else {
            arg0->unk8A = 0;
            sub->unk6E = 0;
            if (other != NULL) {
                other->unk6E = 0;
            }
        }
    }
    if (state == 2) {
        if (arg0->unk7C < 0) {
            arg0->unk7C += D_800BE9E4;
        } else {
            if (arg0->unk80 == 0) {
                switch (sub->unk3C & 0xFF) {
                case 1:
                    func_15170F4C(sub, 0xFF, 0);
                    func_10010F30(0x2BE, 0x7FFF, 0x40, 0, 0);
                    break;
                case 0: {
                    u16 sp5A = func_150ADA20() % 3U;
                    u16 sp58;
                    const u16 sounds[3] = { 0x2B6, 0x2BD, 0x2BE };

                    sp58 = ((func_150ADA20() & 1) + sp5A + 1) % 3U;
                    sp5A = sounds[sp5A];
                    sp58 = sounds[sp58];
                    func_10010F30(0x2B7, 0x7FFF, 0x40, 0, 0);
                    func_10010F30(sp5A, 0x7FFF, 10, 0, 0);
                    func_10010F30(sp58, 0x7FFF, 0x36, 0, 0);
                    sp5A = func_150ADA20() % 5U;
                    sp58 = ((func_150ADA20() & 3) + sp5A + 1) % 5U;
                    func_1000FA64(sp5A + 0x2B1, arg0->unk10, arg0->unk12, arg0->unk14, 0x4000, 0x2711, 0x2710, (s32)func_1000EC24, (void *)0x1001E, 0, 0x200, 0);
                    func_1000FA64(sp58 + 0x2B1, arg0->unk10, arg0->unk12, arg0->unk14, 0x4000, 0x2711, 0x2710, (s32)func_1000EC24, (void *)0x1001E, 0, 0, 0);
                    sp5A = func_150ADA20() % 5U;
                    sp58 = ((func_150ADA20() & 3) + sp5A + 1) % 5U;
                    func_1000FA64(sp5A + 0x2B1, arg0->unk10, arg0->unk12, arg0->unk14, 0x4000, 0x2711, 0x2710, (s32)func_1000EC24, (void *)0x3C, 0, 0x300, 0);
                    func_1000FA64(sp58 + 0x2B1, arg0->unk10, arg0->unk12, arg0->unk14, 0x4000, 0x2711, 0x2710, (s32)func_1000EC24, (void *)0x3C, 0, 0x100, 0);
                }
                    break;
                case 2:
                    break;
                case 3:
                    func_150B2740(sub->unk3C >> 8, 0xFF);
                    break;
                }
            }
            arg0->unk8A = 0xFF;
            sub->unk6E = 1;
            if (other != NULL) {
                other->unk6E = 1;
            }
            arg0->unk80 += D_800BE9E4 * 30;
            if (arg0->unk80 >= 0xFF) {
                arg0->unk80 = 0xFF;
                state = 3;
            }
            if (arg0->unk3C != 0xFF) {
                func_15173C60(arg0->unk80, arg0->unk3C);
            }
            if (state == 3) {
                arg0->unk7C = 0;
                arg0->unk80 = 0;
            }
        }
    }
    arg0->unk73 &= ~3;
    arg0->unk73 |= state;
}

extern f32 D_800A31F4;

void func_1511CB2C(s32 arg0, f32 *arg1) {
    *arg1 = D_800A31F4;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511CB44.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511D394.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511D7BC.s")

struct Obj1511D9E4 {
    u8 pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u16 unk16;
    u8 pad18[0x8];
    Vtx *unk20[2];
    Vtx *unk28;
    u8 pad2C[0x10];
    s32 unk3C;
    u8 pad40[0x3C];
    s16 (*unk7C)[2];
    f32 unk80;
};

extern void func_15094AB8(Vtx *arg0, s16 (*arg1)[2], s32 arg2, f32 arg3, s32 arg4, s32 arg5);

void func_1511D9E4(struct Obj1511D9E4 *arg0) {
    s16 (*uv)[2];
    s32 hi;
    s32 mid;
    s32 i;
    f32 speed;

    if (arg0->unk7C == NULL) {
        uv = arg0->unk7C = allocate_memory(arg0->unk16 * 4, 1, 0, 0);
        for (i = 0; i < arg0->unk16; i++) {
            uv[i][0] = arg0->unk28[i].v.tc[0] - 0x2000;
            uv[i][1] = arg0->unk28[i].v.tc[1] - 0x2000;
        }
        for (i = 0; i < arg0->unk16; i++) {
            arg0->unk20[0][i].v.ob[0] = arg0->unk20[1][i].v.ob[0] = arg0->unk28[i].v.ob[0] + arg0->unk10;
            arg0->unk20[0][i].v.ob[1] = arg0->unk20[1][i].v.ob[1] = arg0->unk28[i].v.ob[1] + arg0->unk12;
            arg0->unk20[0][i].v.ob[2] = arg0->unk20[1][i].v.ob[2] = arg0->unk28[i].v.ob[2] + arg0->unk14;
        }
    } else {
        uv = arg0->unk7C;
    }
    hi = arg0->unk3C >> 24;
    mid = (arg0->unk3C >> 16) & 0xFF;
    speed = (f32)(s16)(arg0->unk3C & 0xFFFF) * 0.01;
    arg0->unk80 += speed * D_800BE9E4;
    func_15094AB8(arg0->unk20[D_800BE9C0], uv, arg0->unk16, arg0->unk80, hi, mid);
}

struct Obj1511DBC4Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
};

struct Obj1511DBC4 {
    u8 pad0[0x8];
    f32 unk8;
    u8 padC[0x67];
    u8 unk73;
    u8 pad74[0x8];
    struct Obj1511DBC4Sub *unk7C;
};

void func_1511DBC4(struct Obj1511DBC4 *arg0) {
    s32 state;
    struct Obj1511DBC4Sub *temp;
    f32 ftemp;

    if (arg0->unk7C == NULL) {
        temp = allocate_memory(0x10, 1, 0, 0);
        arg0->unk7C = temp;
        temp->unk0 = 0.0f;
        temp->unk4 = 0.0f;
        temp->unk8 = 0.0f;
        temp->unkC = D_800A3210;
    } else {
        temp = arg0->unk7C;
    }

    state = arg0->unk73 & 3;
    if (state == 0) {
        arg0->unk8 = 0.0f;
    } else if (state == 3) {
        arg0->unk8 = 90.0f;
        ftemp = cosf(temp->unk0 * D_800A3214);
        ftemp = ftemp * 0.5f;
        arg0->unk8 = arg0->unk8 + ftemp;
        temp->unk0 = temp->unk0 + (3.0f * (f32)D_800BE9E4);
    } else if (state == 2) {
        if (arg0->unk8 == 0.0f) {
            temp->unk4 = 0.0f;
            temp->unk8 = 0.0f;
        }
        temp->unk8 = temp->unk8 + temp->unkC;
        temp->unk4 = temp->unk4 + temp->unk8;
        arg0->unk8 = arg0->unk8 + temp->unk4;
        if (arg0->unk8 >= 90.0f) {
            state = 3;
            arg0->unk8 = 90.0f;
        }
    } else {
        if ((f32)D_800BE9E4 < arg0->unk8) {
            arg0->unk8 = arg0->unk8 - (f32)D_800BE9E4;
        } else {
            arg0->unk8 = 0.0f;
            state = 0;
        }
    }

    arg0->unk73 &= 0xFFFC;
    arg0->unk73 |= state;
}

void func_10010558(u16, void *, s32, s32, s32, s32);
void func_1511DD98(u8 *arg0, u8 *arg1) {
    s32 sound;
    s32 volume;
    s32 type_3B;
    s32 type_80;

    volume = 0x6590;
    type_3B = arg1[4] == 0x3B;
    type_80 = arg1[4] == 0x80;
    sound = 0;
    switch (*(u16 *)(arg0 + 0x54)) {
    case 0x39:
    case 0x5C:
    case 0x5D:
    case 0x5E:
        if (type_3B) {
            func_10010630(0x542, (struct127 *)arg1, 0x6590, 100, 1000);
        } else if (type_80) {
            func_10010630(0x567, (struct127 *)arg1, 0x6590, 100, 1000);
        }
        sound = 0x2B0;
        break;
    case 0x3A:
        if (type_3B) {
            sound = (func_150ADA20() & 1) + 0x53E;
        } else if (type_80) {
            sound = 0x567;
        } else {
            sound = 0x60A;
        }
        break;
    case 0x3B:
    case 0x51:
        sound = 0x181;
        break;
    case 0x3C:
    case 0x6A:
        sound = 0xB1;
        break;
    case 0x3D:
    case 0x6B:
        sound = 0x442;
        break;
    case 0x3E:
        sound = 0x444;
        break;
    case 0x46:
        sound = 0x2F0;
        break;
    case 0x47:
        sound = 0x4A3;
        break;
    case 0x50:
    case 0x55:
        sound = (func_150ADA20() & 1) + 0xB;
        break;
    case 0x52:
    case 0x58:
        sound = 0x510;
        break;
    case 0x53:
        sound = 0x2AF;
        if (type_80) {
            func_10010558(0x567, (struct127 *)arg1, 0x7D00, 100, 1000, 30);
        }
        break;
    case 0x56:
        if (type_80) {
            func_10010630(0x567, (struct127 *)arg1, 0x6590, 100, 1000);
        }
        sound = 0x5AE;
        break;
    case 0x57:
    case 0x6D:
    case 0x6E:
        sound = 0x51;
        volume = 0x2328;
        break;
    case 0x64:
    case 0x65:
        sound = 0x629;
        volume = 0x7FFF;
        break;
    }
    if (sound != 0) {
        func_10010630(sound, (struct127 *)arg1, volume, 100, 1000);
    }
}

struct Obj1511DF6CSub {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    f32 unk8;
    f32 unkC;
};

struct Obj1511DF6C {
    f32 unk0;
    f32 unk4;
    u8 pad8[0x8];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 pad16[0x2];
    f32 unk18;
    u8 pad1C[0x10];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    u8 pad38[0x1C];
    u16 unk54;
    u8 pad56[0x18];
    u8 unk6E;
    u8 pad6F[0xD];
    struct Obj1511DF6CSub *unk7C;
    s32 unk80;
    s32 unk84;
};

extern void *func_1509B570(s32);
extern void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
extern void func_151D6970(struct127 *, struct Obj1511DF6C *);

void func_1511DF6C(struct Obj1511DF6C *arg0) {
    f32 dist;
    s32 hp;
    f32 radius;
    f32 scale;
    s16 height;
    s16 sp64;
    s16 sp62;
    struct Obj1511DF6CSub *sub;
    s32 skip;
    s32 anim;
    s32 i;
    struct127 *obj;
    u8 *res;
    f32 y;

    if (arg0->unk7C == NULL) {
        arg0->unk7C = allocate_memory(0x10, 1, 0, 0);
        sub = arg0->unk7C;
        bzero(sub, 0x10);
        sub->unkC = arg0->unk2C;
        arg0->unk34 = 0.0f;
        arg0->unk30 = 0.0f;
        arg0->unk2C = 0.0f;
    } else {
        sub = arg0->unk7C;
    }
    skip = 0;
    anim = -1;
    switch (arg0->unk54) {
    case 0x59:
    case 0x5A:
        break;
    case 0x54:
        anim = 0x17;
        if (D_800BE9F0 == 0x2D) {
            res = func_1509B570(0xBA);
            if (res != NULL && (hp = *(s32 *)(res + 0x1FE4)) <= 0) {
                skip = 1;
            }
        }
    default:
        arg0->unk4 += (f32)D_800BE9E4 * 1.519999981f;
        dist = cosf(sub->unk0 * 9.587380191e-05f);
        arg0->unk0 = 8.0f * dist;
        dist = cosf(sub->unk2 * 9.587380191e-05f);
        arg0->unk18 = 12.0f * dist;
        sub->unk0 += (f32)D_800BE9E4 * 507.0f;
        sub->unk2 += (f32)D_800BE9E4 * 419.0f;
        break;
    }
    if (sub->unk4 < 3 && skip == 0) {
        for (i = 0; i < D_8008FD8C; i++) {
            obj = &D_800CC2D0[i];
            if (obj->interaction_state == 0 || obj->health <= 0 || obj->unk127 == 0xFF) {
                continue;
            }
            if (obj->unk300 == 0) {
                continue;
            }
            if ((arg0->unk84 == 1 && *(u8 *)&obj->pad128 != 0) || (arg0->unk84 == 2 && *(u8 *)&obj->pad128 == 0) ||
                (arg0->unk84 == 3 && (*(u8 *)&obj->pad128 & 1))) {
                continue;
            }
            if (anim != -1 && func_1503195C(obj, anim, 0) != NULL) {
                continue;
            }
            dist = ((f32)arg0->unk10 - obj->x_position) * ((f32)arg0->unk10 - obj->x_position) + ((f32)arg0->unk14 - obj->z_position) * ((f32)arg0->unk14 - obj->z_position);
            res = (u8 *)obj->unk31C;
            radius = 100.0f;
            scale = (f32)(*(s8 *)(res + 0x1AF) & 0xFF) * 0.015625f;
            if (res[0x84] != 0) {
                scale *= 1.399999976f;
            }
            radius *= scale;
            if (dist < radius * radius) {
                func_1507C3E0(obj, &height, &sp64, &sp62);
                y = (f32)arg0->unk12;
                if (y < *(f32 *)&obj->pad17C && obj->y_position < y + radius &&
                    y - radius < obj->y_position + height) {
                    arg0->unk80 = obj->unique_id;
                    sub->unk4 = 3;
                    func_1511DD98((u8 *)arg0, (u8 *)obj);
                    func_151D6970(obj, arg0);
                    break;
                }
            }
        }
    }
    if (sub->unk4 == 0) {
        arg0->unk34 += 0.2599999905f * sub->unkC;
        if (sub->unkC <= arg0->unk34) {
            arg0->unk34 = sub->unkC;
            sub->unk6 = 0;
            sub->unk4++;
            sub->unk8 = 0.3000000119f;
        }
        arg0->unk30 = arg0->unk34;
        arg0->unk2C = arg0->unk34;
    } else if (sub->unk4 == 1) {
        sub->unk8 -= 0.01999999955f * sub->unkC;
        if (sub->unk8 <= 0.0f) {
            sub->unk4++;
            sub->unk8 = 0.0f;
        }
        arg0->unk34 = cosf(sub->unk6 * 9.587380191e-05f) * sub->unk8 + sub->unkC;
        arg0->unk2C = arg0->unk30 = arg0->unk34;
        sub->unk6 += D_800BE9E4 * 0x1613;
    } else if (sub->unk4 == 2) {
    } else if (sub->unk4 == 3) {
        arg0->unk34 -= 0.2599999905f * sub->unkC;
        if (arg0->unk34 <= 0.0f) {
            sub->unk4 = 0;
            arg0->unk6E = 1;
            arg0->unk34 = 0.0f;
        }
        arg0->unk30 = arg0->unk34;
        arg0->unk2C = arg0->unk34;
    }
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511E780.s")

struct Obj1511EC50 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x28];
    s32 unk3C;
    u8 pad40[0x2E];
    s8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    f32 unk84;
};

struct Obj1511ED84 {
    u8 pad0[0x14];
    s16 unk14;
    u8 pad16[0x5C];
    u8 unk72;
    u8 pad73[0x9];
    s32 unk7C;
    s32 unk80;
    f32 unk84;
};

void func_1511EC50(struct Obj1511EC50 *arg0) {
    f32 step;
    f32 limit;

    if (arg0->unk7C == 0) {
        arg0->unk84 = (f32)arg0->unk12;
    }

    step = (f32)D_800BE9E4 * 4.0f;
    if (arg0->unk3C != 0) {
        step = step * 0.25f;
    }

    if (arg0->unk80 != 0) {
        arg0->unk12 = (s32)((f32)arg0->unk12 + (step * 5.0f));
        limit = arg0->unk84 + 500.0f;
        if (limit < (f32)arg0->unk12) {
            arg0->unk12 = (s32)limit;
            arg0->unk6E = 1;
        }
    } else {
        limit = arg0->unk84;
        arg0->unk12 = (s32)((f32)arg0->unk12 - step);
        if ((f32)arg0->unk12 < limit) {
            arg0->unk12 = (s32)limit;
        }
    }
    arg0->unk7C = 1;
}

void func_1511ED84(struct Obj1511ED84 *arg0) {
    f32 step;
    f32 limit;
    s32 direction;

    if (arg0->unk7C == 0) {
        arg0->unk84 = (f32)arg0->unk14;
    }

    step = (f32)D_800BE9E4 * 4.0f;
    direction = 1 - (arg0->unk72 & 1);

    if (arg0->unk80 != 0) {
        if (direction & 1) {
            arg0->unk14 = (s32)((f32)arg0->unk14 + step);
            limit = arg0->unk84;
            if (limit < (f32)arg0->unk14) {
                arg0->unk14 = (s32)limit;
            }
        } else {
            limit = arg0->unk84;
            arg0->unk14 = (s32)((f32)arg0->unk14 - step);
            if ((f32)arg0->unk14 < limit) {
                arg0->unk14 = (s32)limit;
            }
        }
    } else {
        if (direction & 1) {
            arg0->unk14 = (s32)((f32)arg0->unk14 - step);
            limit = arg0->unk84 - 350.0f;
            if ((f32)arg0->unk14 < limit) {
                arg0->unk14 = (s32)limit;
            }
        } else {
            arg0->unk14 = (s32)((f32)arg0->unk14 + step);
            limit = arg0->unk84 + 350.0f;
            if (limit < (f32)arg0->unk14) {
                arg0->unk14 = (s32)limit;
            }
        }
    }
    arg0->unk7C = 1;
}

typedef struct {
    f32 x, y, z, radius, angle, dx, dz;
    s16 timer;
    u8 pad1E[6];
} Game11EF40Motion;

typedef struct {
    f32 x, y, z;
} Vec3f_1511EF40;

extern s32 func_15086CBC(s32, f32 *, f32 *, f32 *);
extern s32 func_15086D48(s32);
extern f32 D_800A3378;

void func_1511EF40(u8 *arg0) {
    u8 *player;
    f32 x, y, z, angle;
    Game11EF40Motion *motion;
    u8 *actor;
    f32 dx;

    if (*(Game11EF40Motion **)(arg0 + 0x7C) == NULL) {
        motion = *(Game11EF40Motion **)(arg0 + 0x7C) = (Game11EF40Motion *)allocate_memory(0x24, 1, 0, 0);
        bzero(motion, 0x24);
    } else {
        motion = *(Game11EF40Motion **)(arg0 + 0x7C);
    }
    if (motion->timer > 0) {
        motion->timer -= D_800BE9E4;
        if (motion->timer <= 0) {
            actor = (u8 *)D_800DBFF0 + *(s32 *)(arg0 + 0x84) * 0x9A0;
            func_151239CC((struct108 *)actor, 0xA);
        }
    }
    if (*(s32 *)(arg0 + 0x80) == 1) {
        player = (u8 *)D_800CC2D0 + *(s32 *)(arg0 + 0x84) * 0x32C;
        *(s16 *)(arg0 + 0x10) = (s32)*(f32 *)(player + 0x14);
        *(s16 *)(arg0 + 0x12) = (s16)*(f32 *)(player + 0x18) + 100;
        *(s16 *)(arg0 + 0x14) = (s32)*(f32 *)(player + 0x1C);
        func_15086CBC(func_15086D48(0x28), &x, &y, &z);
        motion->dx = (x - *(f32 *)(player + 0x14)) * 0.5f;
        dx = motion->dx;
        motion->dz = (z - *(f32 *)(player + 0x1C)) * 0.5f;
        angle = motion->dz;
        motion->x = *(f32 *)(player + 0x14) + dx;
        motion->y = *(f32 *)(player + 0x18) + 100.0f;
        motion->z = *(f32 *)(player + 0x1C) + angle;
        motion->radius = sqrtf(dx * dx + angle * angle);
        motion->angle = -90.0f;
        *(s32 *)(arg0 + 0x80) = 2;
        return;
    }
    if (*(s32 *)(arg0 + 0x80) == 2) {
        if (D_80082FA0 >= *(s32 *)(arg0 + 0x84)) {
            actor = (u8 *)D_800DBFF0 + *(s32 *)(arg0 + 0x84) * 0x9A0;
            if (*(s32 *)(actor + 0x2C) != 0x400000) {
                func_15123934((struct108 *)actor, 0x400000, 0, *(s32 *)(actor + 0x134), 0xA);
            }
            *(f32 *)(actor + 0x2BC) = *(s16 *)(arg0 + 0x10);
            *(f32 *)(actor + 0x2C0) = *(s16 *)(arg0 + 0x12);
            *(f32 *)(actor + 0x2C4) = *(s16 *)(arg0 + 0x14);
            *(Vec3f_1511EF40 *)(actor + 0x2A4) = *(Vec3f_1511EF40 *)(actor + 0x2BC);
            *(f32 *)(actor + 0x2F8) = 0.0f;
            *(f32 *)(actor + 0x300) = 0.0f;
            *(f32 *)(actor + 0x2FC) = 300.0f;
            motion->timer = 60;
        }
        angle = (motion->angle * D_800A3378) / 180.0f;
        *(s16 *)(arg0 + 0x12) = (s32)(cosf(angle) * motion->radius + motion->y);
        dx = sinf(angle);
        *(s16 *)(arg0 + 0x10) = (s32)(motion->x + motion->dx * dx);
        *(s16 *)(arg0 + 0x14) = (s32)(motion->z + motion->dz * dx);
        motion->angle += (f32)D_800BE9E4;
        if (motion->angle > 90.0f) {
            motion->angle = 90.0f;
            *(s32 *)(arg0 + 0x80) = 0;
            *(s8 *)(arg0 + 0x6E) = 1;
            if (D_80082FA0 >= *(s32 *)(arg0 + 0x84)) func_151239CC((struct108 *)actor, 0xA);
        }
    }
}

void func_1511F31C(struct Struct1511F31CArg *arg0) {
    f32 sp3C[21];
    struct Struct1511490C *temp_v0;
    u32 temp_id;

    temp_id = arg0->unk3E;
    if (arg0->unk54 == 0x8005) {
        if (D_800BE9F0 == 0x35) {
            temp_v0 = (struct Struct1511490C *)func_151149AC(temp_id & 0xFF);
            if (temp_v0 != NULL) {
                func_1511490C((f32 (*)[4])&sp3C[3], temp_v0);
                func_150A7960(&sp3C[3], -3.0f, 223.0f, 549.0f, &sp3C[2], &sp3C[1], &sp3C[0]);
                arg0->unk10 = (s16)sp3C[2];
                arg0->unk12 = (s16)sp3C[1];
                arg0->unk14 = (s16)sp3C[0];
            }
        }
    }
}

struct Obj1511F3E8 {
    u8 pad0[0x3C];
    s32 unk3C;
    u8 pad40[0x2E];
    u8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    u8 pad88[2];
    u8 unk8A;
};

void func_1511F3E8(struct Obj1511F3E8 *arg0) {
    s32 ticks;

    if (arg0->unk7C == 0) {
        ticks = arg0->unk3C;
        arg0->unk80 = ((ticks >> 16) & 0xFFFF) * 60;
        arg0->unk7C = 1;
    }
    ticks = arg0->unk3C;
    ticks = (ticks & 0xFFFF) * 60;
    if (arg0->unk80 != 0) {
        if (D_800BE9E4 < arg0->unk80) {
            arg0->unk80 -= D_800BE9E4;
        } else {
            arg0->unk80 = 0;
            arg0->unk84 = ticks;
        }
    } else if (D_800BE9E4 < arg0->unk84) {
        arg0->unk84 -= D_800BE9E4;
        arg0->unk8A = (arg0->unk84 * 255) / ticks;
    } else {
        arg0->unk6E = 1;
    }
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_142560/func_1511F4D0.s")

void func_1511F768(s32 *arg0, s32 arg1) {
    if (arg0[0x20] == 0) {
        arg0[0x20] = 1;
    }
}

struct Obj1511F788 {
    u8 pad0[0x12];
    s16 unk12;
    u8 pad14[0x4];
    f32 unk18;
    u8 pad1C[0x20];
    s32 unk3C;
    u8 pad40[0x1C];
    s16 unk5C;
    u8 pad5E[0x10];
    s8 unk6E;
    u8 pad6F[0xD];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
};

union Temp1511F788 {
    struct {
        f32 pad;
        s32 w;
    } s;
    f64 d;
};

void func_1511F788(struct Obj1511F788 *arg0) {
    s32 dummy;
    union Temp1511F788 temp;
    s16 initial;
    s32 state;

    initial = (s16)(s32)((f32)arg0->unk12 + arg0->unk18);
    temp.s.w = arg0->unk3C;
    temp.s.w = (temp.s.w * 0x3C) / 0x3C;
    if (D_800BE9B4 != 0) {
        arg0->unk7C = func_150ADA20() & 0xFFFF;
    }

    arg0->unk18 = cosf((f32)arg0->unk7C * 0.0054931640625f) * 25.0f;
    state = arg0->unk80;
    arg0->unk7C = arg0->unk7C + (D_800BE9E4 * 10);

    if (state == 1) {
        arg0->unk84 = arg0->unk84 + D_800BE9E4;
        if (temp.s.w < arg0->unk84) {
            arg0->unk80 = 2;
        }
    } else if (state == 2) {
        arg0->unk12 = arg0->unk12 - D_800BE9E4;
        if (arg0->unk12 < -10000) {
            ((u8 *)arg0)[0x6E] = 1;
        }
    }

    arg0->unk5C = (s32)(((f32)arg0->unk12 + arg0->unk18) - (f32)initial);
}

extern s32 func_151149AC(u32);

void func_1511F92C(s16 *arg0) {
    s16 *temp_v0;

    temp_v0 = (s16 *)func_151149AC(*(u8 *)((u8 *)arg0 + 0x3F));
    if (temp_v0 != NULL) {
        arg0[8] = temp_v0[8];
        arg0[9] = temp_v0[9];
        arg0[0xA] = temp_v0[0xA];
    }
}
