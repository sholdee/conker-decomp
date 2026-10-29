#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct Struct15191BE0Arg0 {
    u8 pad0[0x3B];
    u8 unk3B;
};

struct Struct15191BE0Local {
    struct Struct15191BE0Arg0 *unk0;
    u8 unk4;
    u8 pad5[3];
    Header2 unk8;
    s32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 unk24[9];
    f32 unk48;
    f32 unk4C;
    s16 unk50;
    s16 unk52;
    u8 unk54;
    u8 unk55;
    u8 pad56[2];
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    u8 unk64;
    u8 pad65[3];
};

struct Struct15191BE0Stack {
    s32 pad0;
    struct Struct15191BE0Local local;
};

typedef struct AngleVelocityState {
    u8 pad_0x00[0x38];
    f32 field_0x38;
    f32 field_0x3C;
    u8 pad_0x40[0x68];
    s32 field_0xA8;
    u8 field_0xAC[2];
    s8 field_0xAE[2];
    f32 field_0xB0;
} AngleVelocityState;

void func_1504715C(s32, s32);

struct260 *func_15191BE0(struct Struct15191BE0Arg0 *arg0, Header2 *arg1, u8 arg2, f32 arg3, f32 arg4, s16 arg5, f32 arg6, f32 arg7, s16 arg8, s16 arg9, u8 argA, u8 argB, f32 argC, f32 argD, f32 argE, u8 argF, u8 arg10, s32 arg11) {
    struct260 *temp_v0;
    struct Struct15191BE0Stack sp3C;
    s32 temp_a0;
    u8 temp_arg4;

    if (arg0 == 0) {
        return 0;
    }

    sp3C.local.unk0 = arg0;
    sp3C.local.unk4 = arg0->unk3B;
    sp3C.local.unk8 = *arg1;
    sp3C.local.unk14 = arg2 << 6;
    sp3C.local.unk1C = arg3;
    sp3C.local.unk18 = 0.0f;
    sp3C.local.unk20 = arg4;
    func_1504715C((s32) sp3C.local.unk24, (s32) arg0);

    sp3C.local.unk48 = arg6;
    sp3C.local.unk4C = arg7;
    sp3C.local.unk50 = arg8;
    sp3C.local.unk52 = arg9;
    sp3C.local.unk54 = argA;
    sp3C.local.unk55 = argB;
    sp3C.local.unk58 = argC;
    sp3C.local.unk5C = argD;
    sp3C.local.unk60 = argE;
    sp3C.local.unk64 = argF;

    if (arg5 == -1) {
        temp_a0 = 0x12C;
    } else {
        temp_a0 = arg5;
    }

    if (arg5 == -1) {
        temp_arg4 = 0;
    } else {
        temp_arg4 = 1;
    }

    temp_v0 = func_15149130(temp_a0, -1, 0x1D, -1, temp_arg4, 0x1D, (struct37 *)0x68, arg10, arg11);
    if (temp_v0 != 0) {
        memcpy((s32) temp_v0 + 0x28, &sp3C.local, sizeof(sp3C.local));
    }
    return temp_v0;
}

struct Struct15191D54Owner {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x198];
    s32 unk1D4;
};

struct Struct15191D54Local {
    struct Struct15191D54Owner *unk0;
    u8 unk4;
    u8 pad5[3];
    Header2 unk8;
    s32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s32 unk28[8];
    f32 unk48;
    f32 unk4C;
    s16 unk50;
    s16 unk52;
    u8 unk54;
    u8 unk55;
    u8 pad56[2];
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    u8 unk64;
    u8 pad65[3];
};

struct Struct15191D54Arg0 {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 unkE;
    u8 pad10[0x18];
    struct Struct15191D54Local unk28;
};

void func_15143134(void *, f32 *, s32);
s32 func_1514672C(f32 *);
s32 func_15046F84(f32 *, s32, f32, f32 *);
void func_1519203C(f32 *, f32 *, f32, s16, s32, s32, f32, s32, f32, s32 *, s32, s32, s32);
extern f32 D_800A80E8;
extern f32 D_800A80EC;
extern f32 D_800A80F0;

void func_15191D54(struct Struct15191D54Arg0 *arg0) {
    struct Struct15191D54Owner *owner;
    struct Struct15191D54Local *local;
    f32 spB4[3];
    f32 scale;
    f32 spA4[3];
    f32 rand;
    s32 flag;
    u32 r1;
    u32 r2;
    f32 dist;

    owner = arg0->unk28.unk0;
    local = &arg0->unk28;
    if ((owner->unk0 == 0) || (owner->unk3B != local->unk4)) {
        arg0->unkE = -1;
        return;
    }
    local->unk18 += (local->unk1C + (func_150ADA68() * local->unk20)) * D_800BE9A4;
    if (local->unk18 > 1.0f) {
        if (owner->unk1D4 != 0) {
            func_15143134(&local->unk8, spB4, owner->unk1D4 + local->unk14);
            if (local->unk60 > 0.0f) {
                if (func_1514672C(spB4) == 0) {
                    scale = D_800A80E8;
                } else {
                    dist = D_800A80EC;
                    if (func_15046F84(spB4, 0, dist, &local->unk24) != 0) {
                        scale = local->unk24;
                    } else {
                        scale = dist;
                    }
                }
            } else {
                scale = D_800A80F0;
            }
            do {
                flag = (func_150ADA68() < local->unk60) ? 1 : 0;
                spA4[0] = 0.0f;
                spA4[1] = 0.0f;
                spA4[2] = 0.0f;
                rand = func_150ADA68();
                r1 = func_150ADA20();
                r2 = func_150ADA20();
                func_1519203C(spB4, spA4, ((f32)rand * local->unk4C) + local->unk48,
                              (s16)((r1 % (u32)(local->unk52 + 1)) + local->unk50),
                              (r2 % (u32)(local->unk55 + 1)) + local->unk54, 0xFF,
                              (func_150ADA68() * local->unk5C) + local->unk58, flag, scale,
                              local->unk28, local->unk64, arg0->unkC, arg0->unk1);
                local->unk18 -= 1.0f;
            } while (local->unk18 > 1.0f);
        } else {
            while (local->unk18 > 1.0f) {
                local->unk18 -= 1.0f;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1BF090/func_1519203C.s")

extern void func_15131C84(s32, s32, s32, s32, s32, s32);

u8 func_15192308(AngleVelocityState *arg0, s32 arg1) {
    func_15131C84((s32) arg0->field_0xAC, (s32) arg0->field_0xAE, arg0->field_0xA8, (s32) &arg0->field_0xB0, (s32) &arg0->field_0x38, (s32) &arg0->field_0x3C);
    return 1;
}

typedef struct Game1BF090FloatVector {
    f32 values[3];
} Game1BF090FloatVector;

typedef struct Game1BF090Emitter {
    f32 scale;
    u8 pad4[0xC];
    f32 height;
    u8 data[0x12];
    u8 kind;
} Game1BF090Emitter;

typedef struct Game1BF090Effect {
    u8 pad0;
    u8 group;
    u8 pad2[0xA];
    u8 mode;
    u8 padD[0x1E];
    u8 index;
    u8 pad2C[0x14];
    f32 position[3];
    u8 pad4C[0x5C];
    Game1BF090Emitter emitter;
} Game1BF090Effect;

void func_151436B4(f32, f32, f32, f32 *);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32,
                  s32, s32, s32, s32, s32, s32, s32, s32);

u8 func_15192358(s32 arg0, s32 arg1) {
    u8 result;
    f32 factor;
    Game1BF090FloatVector position;
    Game1BF090FloatVector direction;
    f32 size_random;
    f32 speed_random;
    f32 angle_random;
    register u32 count_random;
    u32 duration_random;
    Game1BF090Emitter *emitter;

    result = 1;
    emitter = &((Game1BF090Effect *)arg0)->emitter;
    if (((Game1BF090Effect *)arg0)->position[1] < ((Game1BF090Effect *)arg0)->emitter.height) {
        result = 0;
        position.values[0] = ((Game1BF090Effect *)arg0)->position[0];
        position.values[1] = emitter->height + 2.0f;
        position.values[2] = ((Game1BF090Effect *)arg0)->position[2];
        func_151D9B8C(emitter->kind, emitter->scale * 0.08308f,
                      ((Game1BF090Effect *)arg0)->index, (s32)emitter->data, (struct17 *)&position,
                      100, 0, 1, 0, ((Game1BF090Effect *)arg0)->mode,
                      ((Game1BF090Effect *)arg0)->group);
        angle_random = func_150ADA68();
        speed_random = func_150ADA68();
        size_random = func_150ADA68();
        factor = 0.001f;
        func_151436B4(angle_random * 6.2831855f,
                      (speed_random * 1000.0f + -1207.0f) * factor,
                      (size_random * 3557.0f + 5336.0f) * factor,
                      (f32 *)&direction);
        size_random = func_150ADA68();
        duration_random = func_150ADA20();
        count_random = func_150ADA20();
        factor = 1.0f;
        func_151DA6F8((f32 *)&position, (f32 *)&direction, size_random * 0.20600002f + 0.20600002f,
                      (s16)(duration_random % 21U + 35), ((Game1BF090Effect *)arg0)->index,
                      emitter->scale * 0.02495f, count_random % 7U + 5, 0,
                      factor, factor, 1, emitter->kind, 0, 0x10, 0xF, 0,
                      ((Game1BF090Effect *)arg0)->mode, ((Game1BF090Effect *)arg0)->group);
    }
    return result;
}

u8 func_15192308(AngleVelocityState *arg0, s32 arg1);
u8 func_15192358(s32 arg0, s32 arg1);

u8 func_1519257C(s32 arg0, s32 arg1) {
    u8 ret;
    ret = func_15192308((AngleVelocityState *) arg0, arg1);
    if (ret != 0) {
        ret = func_15192358(arg0, arg1);
    }
    return ret;
}

extern Header2 D_800A8030[];
extern u8 D_800A8060[];
extern f32 D_800A8064[];
extern f32 D_800A8074[];
extern f32 D_800A8084[];
extern f32 D_800A8094[];
extern s16 D_800A80A4[];
extern s16 D_800A80AC[];
extern f32 D_800A80B4[];
extern f32 D_800A80C4[];
extern f32 D_800A80D4[];
extern u8 D_800A80E4[];

s32 func_151925C4(u8 *arg0, s16 arg1, u8 arg2, s32 arg3) {
    s32 kind;
    s32 result;
    u8 mode;

    if (arg0 == 0) {
        return 0;
    }
    kind = -1;
    result = 0;
    switch (arg0[4]) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        kind = 0;
        break;
    case 0x10:
        kind = 1;
        break;
    case 0x58:
        kind = 2;
        break;
    case 0x79:
        kind = 3;
        break;
    }
    if (kind != -1) {
        result = (s32)func_15191BE0((struct Struct15191BE0Arg0 *)arg0, &D_800A8030[kind],
                               D_800A8060[kind], D_800A8064[kind],
                               D_800A8074[kind], arg1,
                               D_800A8084[kind], D_800A8094[kind],
                               D_800A80A4[kind], D_800A80AC[kind],
                               0x64, 0x9B, D_800A80B4[kind],
                               D_800A80C4[kind], D_800A80D4[kind],
                               D_800A80E4[kind], arg2, arg3);
    }
    return result;
}

void func_1519277C(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}
