#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8proto
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

extern u8 D_800A3FE6[];
extern u8 D_800A4058[];
extern u8 D_800A4068[];
extern f32 D_800A45B0;
extern f32 D_800A482C;
extern f32 D_800A4830;
extern f32 D_800A4834;
extern f32 D_800A4838;
extern f32 D_800A483C;
extern f32 D_800A4840;
extern void func_15153F18(void *, void *, s32, u8, s32);
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_15143134(void *, s32, s32);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);
extern s32 func_15146078(struct17 *, struct17 *, struct17 *);
extern void func_151BC5A4(struct260 *, s32, u8);

typedef struct {
    s32 unk0[4];
} Struct15136698Table;

extern Struct15136698Table D_80089BAC;

// requires jump table
s32 func_15134070(void *arg0) {
    switch (*((u8 *)arg0 + 4)) {
    case 135:
    case 176:
        return 0xE;
    case 177:
        return 0xF;
    case 180:
        return 0x10;
    case 112:
    case 178:
        return 0x11;
    case 0xAB:
        return 0x12;
    case 16:
    case 56:
    case 145:
        return 0xD;
    case 152:
        return 0xC;
    case 52:
        return 0xB;
    case 71:
        return 0xA;
    case 17:
    case 20:
    case 59:
        return 1;
    case 90:
    case 95:
    case 116:
    case 117:
    case 122:
        return 2;
    case 88:
    case 91:
    case 128:
        return 3;
    case 22:
    case 136:
    case 144:
        return 4;
    case 156:
        return 5;
    case 157:
        return 6;
    case 150:
        return 7;
    case 160:
        return 8;
    case 159:
        return 9;
    case 0x9A:
        return 0x13;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return 0;
    default:
        return 0x63;
    }
}

s32 func_1513416C(struct102 *arg0) {
    s16 temp_v0 = arg0->unk1C;
    if (temp_v0 < 32) {
        arg0->unk28 = temp_v0 * 8;
    }
    return 1;
}

typedef struct {
    u8 pad0[0x10];
    u8 pad10[0x8];
    u8 unk18;
    u8 pad19[0x3];
    struct127 *unk1C;
    u8 unk20;
    u8 pad21[0x3];
    u8 unk24[0xC];
    f32 unk30;
    u8 pad34[0x6];
    u8 unk3A;
    u8 pad3B[0x5];
    u8 unk40[0xC];
    f32 unk4C;
    f32 unk50;
} Struct1513418C;

void *func_1513418C(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    Struct1513418C *temp_v0;
    struct127 *temp_v0_2;
    s32 temp_v1;
    s32 temp_a2;
    u8 temp_a0;
    Struct1513418C *sp24;

    temp_v0 = func_15167A68(0x28, arg3, arg1 + 0x58, 1, (u8)arg2, 1);
    if (temp_v0 == NULL) {
        return NULL;
    }
    sp24 = temp_v0;
    memcpy(&temp_v0->pad10, arg0, 0x30);
    temp_a0 = temp_v0->unk3A;
    if (temp_a0 & 2) {
        temp_v0_2 = temp_v0->unk1C;
        if ((temp_v0_2->interaction_state == 0) || (temp_v0_2->unique_id != temp_v0->unk18)) {
            func_1516972C((struct102 *)temp_v0);
            return NULL;
        }
        temp_v1 = (s32)temp_v0_2->unk1D4;
        if ((temp_v1 != 0) && ((temp_v0_2->unk74 & 0xF) != 0xF)) {
            sp24 = temp_v0;
            temp_a2 = temp_v1;
            temp_a2 += temp_v0->unk20 << 6;
            func_15143134(temp_v0->unk24, (s32)temp_v0->unk40, temp_a2);
        } else {
            temp_v0->unk3A = temp_a0 | 8;
        }
    } else {
        temp_v0->unk3A = temp_a0 | 0x18;
    }
    temp_v0->unk4C = 1.0f / (temp_v0->unk30 + temp_v0->unk30);
    temp_v0->unk50 = 0.0f;
    return temp_v0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151342BC.s")

s32 func_151346D0(s32 arg0, struct102 *arg1, s32 arg2) {
    arg1->unk3A &= 0xFFEF;
    return arg0;
}

void func_151346EC(struct102 *arg0) {
    func_15169804(arg0);
}

void func_1513470C(struct102 *arg0) {
    func_15169824(arg0);
}

void func_1513472C(struct102 *arg0) {
    s32 idx = arg0->unk3D;
    if (idx < 0) {
        idx = 0;
    }
    if (idx >= 10) {
        idx = 0;
    }
    D_80089AAC[idx]();
}

void func_1513477C(struct102 *arg0) {
  s32 idx = arg0->unk3D;
  if (idx < 0) {
      idx = 0;
  }
  if (idx >= 10) {
      idx = 0;
  }
    D_80089AD4[idx]();
}

typedef struct {
    u8  pad0[0x18];
    u8  unk18;
    u8  pad19[0x3];
    s32 unk1C;
    u8  pad20[0x1D];
    u8  unk3D;
} SubA_151347CC;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_151347CC;

void func_151347CC(void *arg0, s32 arg1, u8 arg2) {
    SubA_151347CC *a = (SubA_151347CC *)arg0;
    ArgB_151347CC *b = (ArgB_151347CC *)arg1;
    s32 t;

    if ((arg2 == 0) || (arg2 == 0x3)) {
        if ((b->unk0 == a->unk1C) || (b->u4.b4 == a->unk18)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x11) {
        if (a->unk3D == 0x5) {
            if ((b->unk0 == a->unk1C) || (b->u4.b4 == a->unk18)) {
                func_1516972C((struct102 *)arg0);
            }
        }
    } else if (arg2 == 0x16) {
        if (arg1 == a->unk1C) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        t = b->unk0;
        if (a->unk1C == t) {
            a->unk1C = b->u4.w4;
            a->unk18 = b->unk9;
        } else {
            if (a->unk1C == b->u4.w4) {
                a->unk1C = t;
                a->unk18 = b->unk8;
            }
        dummy_label_151347CC: ;
        }
    }
}

void func_151348F0(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
}

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

struct Struct15134908 {
    u8 pad0[0x10];
    f32 *unk10;
    f32 *unk14;
    f32 *unk18;
    f32 unk1C;
    u8 pad20[0xC];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
};

void *func_15134908(void *arg0, s32 arg1, u8 arg2, s32 arg3) {
    struct Struct15134908 *temp_v0;
    struct Struct15134908 *sp24;

    temp_v0 = (struct Struct15134908 *)func_15167A68(0x2A, arg3, arg1 + 0x40, 1, arg2, 1);
    if (temp_v0 == 0) {
        return NULL;
    }
    *(u8 *)((s32)arg0 + 0x16) |= 2;
    sp24 = temp_v0;
    memcpy((s32)temp_v0 + 0x10, arg0, 0x1C);
    sp24->unk2C = *sp24->unk10;
    sp24->unk30 = *sp24->unk14;
    sp24->unk34 = *sp24->unk18;
    sp24->unk38 = (f32)(1.0f / (sp24->unk1C + sp24->unk1C));
    sp24->unk3C = 0.0f;
    return sp24;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151349D0.s")

void func_15134C98(struct260 *arg0, s32 arg1, u8 arg2) {
    if (((struct102 *)arg0)->unk28 == 1) {
        func_151BC5A4(arg0, arg1, arg2);
    }
}

void func_15134CD4(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
}

typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    u8 pad18[0x4];
    f32 unk1C;
    u8 pad20[0xE];
    u8 unk2E;
    u8 pad2F[0x41];
    f32 unk70;
    f32 unk74;
} Struct15134CEC;

s32 func_15134CEC(Struct15134CEC *arg0) {
    s32 temp_v1;

    arg0->unk70 += 0.125f * D_800BE9A4;
    temp_v1 = arg0->unk2E;
    arg0->unk74 += D_800A45B0 * D_800BE9A4;
    arg0->unk14 += arg0->unk70 * D_800BE9A4;
    arg0->unk1C += arg0->unk74 * D_800BE9A4;
    if (arg0->unk14 > 130.0f) {
        return 0;
    }
    temp_v1 -= D_800BE9E4 * 2;
    if (temp_v1 < 0) {
        return 0;
    }
    arg0->unk2E = temp_v1;
    return 1;
}

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

void *func_15134DAC(void *arg0, s32 arg1) {
    void *temp_v1;

    temp_v1 = func_15167A68(0x29, 0, arg1 + 0x80, 1, 0xFF, 1);
    if (temp_v1 == 0) {
        return NULL;
    }
    memcpy((s32)temp_v1 + 0x18, arg0, 0x3C);
    *(s16 *)((s32)temp_v1 + 0x54) = -(*(s16 *)((s32)arg0 + 0x28));
    *(s32 *)((s32)temp_v1 + 0x10) = 1;
    *(s32 *)((s32)temp_v1 + 0x14) = 0;
    *(f32 *)((s32)temp_v1 + 0x70) = 0.0f;
    *(f32 *)((s32)temp_v1 + 0x74) = 0.0f;
    *(f32 *)((s32)temp_v1 + 0x78) = 0.0f;
    return temp_v1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15134E48.s")

void func_151352EC(struct102 *arg0) {
    func_15169804(arg0);
}

void func_1513530C(struct102 *arg0) {
    func_15169824(arg0);
}

extern void (*D_80089B70[])(void *);

typedef struct {
    u8 pad0[0x44];
    u16 unk44;
    u8 pad46[0xA];
    u8 unk50;
} struct1513532C;

void func_1513532C(struct1513532C *arg0) {
    struct1513532C *p = arg0;
    s32 idx;

    idx = p->unk50;
    if (idx < 0) {
        idx = 0;
    } else if (idx >= 6) {
        idx = 0;
    }
    if (p->unk44 != 0) {
        func_100111C8(p->unk44);
        p->unk44 = 0;
    }
    D_80089B70[idx](p);
}

extern void (*D_80089B88[])(void *);

void func_151353A8(struct1513532C *arg0) {
    struct1513532C *p = arg0;
    s32 idx;

    idx = p->unk50;
    if (idx < 0) {
        idx = 0;
    } else if (idx >= 6) {
        idx = 0;
    }
    if (p->unk44 != 0) {
        func_100111C8(p->unk44);
        p->unk44 = 0;
    }
    D_80089B88[idx](p);
}

extern s32 func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);

void func_15135424(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp20[2];
    s32 sp18[2];

    sp20[0] = arg1;
    sp20[1] = arg2;
    sp18[0] = arg3;
    sp18[1] = arg4;
    func_15145EA4(sp20, sp18, arg0, 2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15135480.s")

struct arg1_1513555C {
    s32 unk0;
    u8 unk4;
};

void func_1513555C(struct102 *arg0, struct arg1_1513555C *arg1, u8 arg2) {
    s32 a = arg2;
    if (a == 0 || a == 0x12) {
        if (arg1->unk0 == *(s32 *)&arg0->unk1C || arg0->unk18 == arg1->unk4) {
            func_1516972C(arg0);
        }
    }
}

void func_151355B8(struct102 *arg0, struct arg1_1513555C *arg1, u8 arg2) {
    switch (arg2) {
    case 0:
        if (arg1->unk0 == *(s32 *)&arg0->unk1C || arg0->unk18 == arg1->unk4) {
            func_1516972C(arg0);
        }
        break;
    case 3:
        if (arg1->unk0 == *(s32 *)&arg0->unk1C || arg0->unk18 == arg1->unk4) {
            s32 *p = (s32 *)((u8 *)arg0 + 0x10);
            *p &= ~1;
            *p = *p;
        }
        break;
    }
}

s32 func_15135658(void *arg0) {
    *(f32 *)((u8 *)arg0 + 0x74) = 1.0f;
    return 1;
}

f32 func_15135670(s32 arg0) {
    // "power", "../Effects/Blood/blood.c"
    return func_151422DC(0, &D_800A3FB4, 0, 2000, 1000, &D_800A3FBC, 2938) * D_800A45B4;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151356D4.s")

typedef struct {
    f32 x;
    f32 y;
} Vec2_15135BF8;

typedef struct {
    s16 timer;
    f32 unk4;
    Vec2_15135BF8 vel;
    f32 unk10;
    f32 unk14;
} Sub_15135BF8;

typedef struct {
    u8 pad0[0x10];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x50];
    s16 unk70;
} Obj_15135BF8;

s32 func_15135BF8(Obj_15135BF8 *arg0) {
    Sub_15135BF8 *sub;
    s32 ret;

    sub = (Sub_15135BF8 *)&arg0->unk70;
    ret = 1;
    if (arg0->unk70 > 0) {
        sub->timer -= D_800BE9E4;
        arg0->unk10 += sub->vel.x * D_800BE9A4;
        arg0->unk14 += sub->vel.y * D_800BE9A4;
        arg0->unk18 += sub->unk4 * D_800BE9A4;
        arg0->unk1C += sub->unk4 * D_800BE9A4;
    } else {
        Vec2_15135BF8 tmp;

        tmp = sub->vel;
        arg0->unk14 += sub->vel.y * D_800BE9A4 + sub->unk10 * D_800BE9A4 * D_800BE9A4 * 0.5f;
        sub->vel.y += sub->unk10 * D_800BE9A4;
        tmp.x += sub->vel.x;
        tmp.y += sub->vel.y;
        tmp.x *= 0.5f;
        tmp.y *= 0.5f;
        {
            f32 f = fabsf(tmp.x) * sub->unk14;

            arg0->unk18 += f * D_800BE9A4;
            arg0->unk1C += f * D_800BE9A4;
        }
    }
    if (arg0->unk10 > 200.0f || arg0->unk10 < -200.0f || arg0->unk14 > 200.0f || arg0->unk14 < -200.0f) {
        ret = 0;
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15135DD0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15136404.s")

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Struct15136698Vec;

typedef struct {
    u8 unk0;
    u8 pad1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    Struct15136698Vec unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 pad46[0x12];
} Struct15136698Local;

s32 func_15136698(f32 arg0, f32 arg1, u8 arg2, u8 arg3, s16 arg4, s32 arg5, Struct15136698Vec *arg6, u8 arg7, u8 arg8, u8 arg9, s32 arg10) {
    s32 ret;
    Struct15136698Local sp7C;
    f32 sp78;
    Struct15136698Table sp68;
    s32 sp64;
    s32 sp60;
    s32 temp_v0;
    s32 sp54;
    s32 sp50;

    sp68 = D_80089BAC;
    sp78 = arg1;
    sp7C.unk0 = sp68.unk0[func_150ADA20() & 3];
    if (arg7 != 0) {
        temp_v0 = 2;
    } else {
        temp_v0 = 1;
    }
    sp7C.unk2 = temp_v0 + 0x300;
    sp7C.unk8 = 0;
    sp7C.unkC = 0;
    sp7C.unk10 = 0;
    sp7C.unk11 = 0;
    sp7C.unk12 = 0;
    sp7C.unk13 = 0xFF;
    sp7C.unk18 = arg0;
    sp7C.unk14 = arg0;
    sp7C.unk1C = *arg6;
    sp7C.unk40 = 0;
    sp7C.unk28 = 0.0f;
    sp7C.unk2C = 0.0f;
    sp7C.unk30 = 0.0f;
    sp7C.unk34 = 1.0f;
    sp7C.unk38 = 1.0f;
    sp7C.unk3C = 1.0f;
    if (arg4 == -1) {
        sp7C.unk4 = 0x12C;
    } else {
        sp7C.unk4 = arg4 + 0x20;
        sp7C.unk40 = 1;
    }
    sp7C.unk44 = arg2;
    sp7C.unk45 = arg3;
    if (arg8 != 0) {
        sp64 = 3;
        sp60 = 0xFF;
    } else {
        sp64 = 0;
        sp60 = 0;
    }

    sp50 = func_150ADA20();
    sp54 = func_150ADA20();
    temp_v0 = func_150ADA20();
    ret = func_1513D594((s32)&sp7C, 0, 0, 0x1A, 0, (sp54 & 1) + (sp50 & 1), temp_v0 & 0xFF, 500.0f, 500.0f, 0, arg5, sp64, sp60, 0, 4, arg9, arg10);
    if (ret != 0) {
        memcpy(ret + 0x128, &sp78, sizeof(sp78));
    }
    return ret;
}

extern f32 D_800BE9A4;

s32 func_151368A8(struct102 *arg0) {
    s32 v0 = *(s16 *)((u8 *)arg0 + 0x1C);
    f32 *v1 = (f32 *)((u8 *)arg0 + 0x128);

    if (v0 < 0x20) {
        s32 sv = v0 << 3;
        if (sv < *(u8 *)((u8 *)arg0 + 0x5C)) {
            *(u8 *)((u8 *)arg0 + 0x5C) = sv;
        }
    }
    *(f32 *)((u8 *)arg0 + 0x2C) += *v1 * D_800BE9A4;
    *(f32 *)((u8 *)arg0 + 0x30) += *v1 * D_800BE9A4;
    return 1;
}

struct LocalDef15136918 {
    s32 unk0;
    s16 unk4;
    s8  unk6;
    s8  unk7;
    s32 unk8;
    s32 unkC;
    u8  unk10;
    u8  unk11;
    u8  unk12;
    u8  unk13;
    u8  unk14;
    u8  unk15;
    s8  unk16;
    s8  unk17;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
};

void func_15136918(f32 arg0, u8 arg1, u8 arg2, s16 arg3, s16 arg4, s32 arg5, f32 *arg6, u8 arg7, s32 arg8) {
    struct LocalDef15136918 tmp;
    f64 spPad;

    spPad = arg0;
    tmp.unk6 = 0x55;
    tmp.unk0 = 0x300;
    tmp.unk8 = 0;
    tmp.unkC = 0;
    tmp.unk10 = arg1;
    tmp.unk11 = arg2;
    tmp.unk12 = 0;
    tmp.unk13 = 0;
    tmp.unk14 = 0;
    tmp.unk15 = 0;
    tmp.unk18 = 0x280001;
    tmp.unk16 = 1;
    tmp.unk17 = 1;
    if (arg3 == -1) {
        tmp.unk4 = 0x12C;
    } else {
        tmp.unk0 = 0x301;
        tmp.unk4 = arg3 + 0x20;
    }
    func_1513C73C((s32)&tmp, 0xD, 0, arg5, arg6[0], arg6[1], arg6[2], arg0, arg0, arg4, 0, 0, arg7, arg8);
}

s32 func_15136A1C(struct102 *arg0) {
    s16 temp_v0 = arg0->unk1C;
    s32 temp_v1;

    if (temp_v0 < 0x20) {
        temp_v1 = temp_v0 << 3;
        if (temp_v1 < arg0->unk28) {
            arg0->unk28 = temp_v1;
        }
    }
    return 1;
}

extern f32 D_800A461C;
extern f32 D_800A4620;
extern void *func_15134908(void *, s32, u8, s32);

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ s8 unk19;
} Struct15136A50;

void func_15136A50(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s32 arg4, s32 arg5) {
    Struct15136A50 sp1C;

    sp1C.unk0C = D_800A461C;
    sp1C.unk00 = arg0;
    sp1C.unk04 = arg1;
    sp1C.unk08 = arg2;
    sp1C.unk10 = D_800A4620;
    sp1C.unk14 = arg3;
    sp1C.unk16 = 5;
    sp1C.unk17 = 5;
    sp1C.unk18 = 2;
    sp1C.unk19 = -1;
    func_15134908(&sp1C, 0, arg4, arg5);
}

extern f32 D_800A4624;
extern f32 D_800A4628;
extern f32 D_800A462C;
void func_151D9014(void *a0, void *a1, s32 a2, f32 a3, s32 sp10, s32 sp14,
                     f32 sp18, s32 sp1C, f32 sp20, f32 sp24, s32 sp28, s32 sp2C,
                     s32 sp30, s32 sp34, s32 sp38, s32 sp3C);

void func_15136AE4(f32 a0, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, u8 *a6) {
    f32 sp6C[3];
    f32 sp60[3];
    f32 r;
    struct { f32 c; s32 a; u32 b; s32 pad; } sp4C;

    sp6C[0] = a0;
    sp6C[1] = a1;
    sp6C[2] = a2;
    r = (func_150ADA68() * 112.0f + 247.0f) * D_800A4624;
    sp60[0] = -a3 * r;
    sp60[1] = -a4 * r;
    sp60[2] = -a5 * r;
    sp4C.c = func_150ADA68();
    sp4C.a = func_150ADA20();
    sp4C.b = func_150ADA20();
    func_151D9014(sp6C, sp60, 0, sp4C.c * D_800A4628 + D_800A462C,
                    (sp4C.a & 0xF) + 25, (sp4C.b % 101) + 155,
                    func_150ADA68() * 119.0f + 129.0f, 0, 1.0f, 1.0f, 1, 0, 1, 0,
                    a6[0xC], a6[1]);
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_36C3C;

typedef struct Blood36C3CActor {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x147];
    f32 unk14C;
    f32 unk150;
} Blood36C3CActor;

typedef struct {
    f32 values[4];
} Blood1380B4Vector;

typedef struct Blood36C3CHit {
    f32 height;
    u8 geometry[0x20];
} Blood36C3CHit;

typedef struct Blood36C3CSpray {
    s32 countBase;
    s32 countRange;
    Vec3f_36C3C origin;
    s16 angleBase;
    s16 angleRange;
    s16 pitchBase;
    s16 pitchRange;
    f32 maxDistance;
    f32 sizeBase;
    f32 sizeRange;
    s16 lifeBase;
    s16 lifeRange;
    u8 kind;
} Blood36C3CSpray;

s32 func_15134070();
void func_1504715C();
void func_1515C244(void *, void *, f32 *, f32 *);
void func_1513A24C(Vec3f_36C3C *, s32 *, f32 *, s32, f32, u8, u8, u8 *, void *,
                   u8, s32);
void func_15150D1C(Blood36C3CSpray *, u8, s32);
void func_151A9834(Vec3f_36C3C *, f32, f32, void *, s32, u8, s8 *, u8, s32);
void func_151C61A0(s32, s32, u8, s32);
void func_1513A48C(void *, u8, s32);
extern s32 *D_80089A20[];
extern f32 D_800A3C30;
extern s32 D_800A3F14[];
extern s32 D_800A3F64[];
extern u8 D_800A3FD8[];
extern Blood1380B4Vector D_800A3FF8;
extern Blood1380B4Vector D_800A40C8;
extern Blood1380B4Vector D_800A40F8;
extern f32 D_800A4630;

#define BLOOD_TBL ((Blood1380B4Vector *)D_800A3FD8)

void func_15136C3C(Blood36C3CActor *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4,
                   u8 arg5, u8 arg6, s32 arg7) {
    Vec3f_36C3C position;
    f32 unused0;
    f32 unused1;
    f32 halfHeight;
    Blood36C3CHit hit;
    s32 kind;
    Blood36C3CSpray spray;
    s8 heavy;

    if (arg0 == NULL) {
        return;
    }
    halfHeight = (arg0->unk14C + arg0->unk150) * 0.5f;
    func_1515C244(arg0, &position, &unused0, &unused1);
    kind = func_15134070(arg0);
    if (kind == 0x63) {
        return;
    }
    func_1504715C(&hit, arg0);
    if (arg3 != 0) {
        func_1513A24C(&position, D_80089A20[kind], &D_800A3C30,
                      ((arg5 != 0) || ((kind == 4) && (arg0->unk4 != 0x16)))
                          ? D_800A3F14[kind] - 2
                          : D_800A3F14[kind],
                      halfHeight, kind, D_800A3F64[kind], (u8 *)arg0, &hit, arg6,
                      arg7);
    }
    if (((u8 *)&BLOOD_TBL[kind])[0xE] == 2) {
        return;
    }
    if (arg1 != 0) {
        spray.countBase = 0xC;
        spray.countRange = 3;
        spray.origin = position;
        spray.angleBase = 0;
        spray.angleRange = 0xFF;
        spray.pitchBase = -0x40;
        spray.pitchRange = 0x45;
        spray.lifeBase = 0x96;
        spray.lifeRange = 0x64;
        spray.maxDistance = D_800A4630;
        spray.sizeBase = 25.0f;
        spray.sizeRange = 10.0f;
        if ((&D_800A3FF8 == &BLOOD_TBL[kind]) || (&D_800A40C8 == &BLOOD_TBL[kind]) ||
            (&D_800A40F8 == &BLOOD_TBL[kind])) {
            spray.kind = 1;
        } else {
            spray.kind = 0;
        }
        func_15150D1C(&spray, arg6, arg7);
    }
    if (arg2 != 0) {
        if ((&D_800A3FF8 == &BLOOD_TBL[kind]) || (&D_800A40C8 == &BLOOD_TBL[kind]) ||
            (&D_800A40F8 == &BLOOD_TBL[kind])) {
            heavy = 1;
        } else {
            heavy = 0;
        }
        func_151A9834(&position, position.y - 300.0f, 300.0f, &hit,
                      (func_150ADA20() % 6U) + 0x12, 0, &heavy, arg6, arg7);
    }
    if (arg4 != 0) {
        if ((&D_800A3FF8 == &BLOOD_TBL[kind]) || (&D_800A40C8 == &BLOOD_TBL[kind])) {
            func_151C61A0((s32)&position, (s32)&hit, arg6, arg7);
        } else {
            func_1513A48C(&position, arg6, arg7);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15136F50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15137610.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513783C.s")

s32 func_15137C64(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3,
                  struct17 *arg4, struct17 *arg5, struct17 *arg6, struct17 *arg7) {
    f32 sp2C;
    f32 sp28;

    if ((arg5 != NULL) && (arg6 != NULL)) {
        *arg0 = *arg5;
        *arg1 = *arg6;
    } else if (arg5 != NULL) {
        *arg0 = *arg5;
        *arg1 = *arg5;
    } else if (arg6 != NULL) {
        *arg0 = *arg6;
        *arg1 = *arg6;
    } else {
        return 0;
    }

    if (arg7 == NULL) {
        arg2->unk0 = arg1->unk0 - arg0->unk0;
        arg2->unk4 = arg1->unk4 - arg0->unk4;
        arg2->unk8 = arg1->unk8 - arg0->unk8;
        if (func_15145128(arg2, arg2, &sp2C, &sp28) == 0) {
            return 0;
        }
    } else {
        *arg2 = *arg7;
    }

    if (func_15146078(arg2, arg3, arg4) == 0) {
        return 2;
    }
    return 1;
}

extern f32 D_800A4828;

s32 func_15137E10(s32 *arg0) {
    f32 temp = func_150ADA68();
    *(f32 *)((u8 *)arg0 + 0x74) = (temp * 50.0f + 580.0f) * D_800A4828;
    return 1;
}

typedef struct { f32 x, y, z; } Vec15137;
typedef struct { u8 pad0[0x74]; f32 unk74; } Struct15137F30Scale;

extern void func_15137F30(Vec15137 *a0, Vec15137 *a1, Vec15137 *a2, Vec15137 *a3,
                          f32 sp10, Struct15137F30Scale *sp14, Vec15137 *out6C,
                          Vec15137 *out60, Vec15137 *out54, f32 *out50,
                          s16 *out4E, u8 *out4D, f32 *out48);
void func_15137E60(Vec15137 *arg0, Vec15137 *arg1, Vec15137 *arg2,
                   Vec15137 *arg3, f32 arg88, s32 *arg8C) {
    Vec15137 sp6C;
    Vec15137 sp60;
    Vec15137 sp54;
    f32 sp50;
    s16 sp4E;
    u8 sp4D;
    f32 sp48;

    func_15137F30(arg0, arg1, arg2, arg3, arg88, (Struct15137F30Scale *)arg8C,
                  &sp6C, &sp60, &sp54, &sp50, &sp4E, &sp4D, &sp48);
    func_151D9014(&sp6C, &sp54, 0, sp50, sp4E, sp4D, sp48, 0, 1.0f, 1.0f,
                  1, 0, 1, 0, *((u8 *)arg8C + 0xC), *((u8 *)arg8C + 1));
}

void func_15137F30(Vec15137 *a0, Vec15137 *a1, Vec15137 *a2, Vec15137 *a3,
                   f32 sp10, Struct15137F30Scale *sp14, Vec15137 *out6C, Vec15137 *out60,
                   Vec15137 *out54, f32 *out50, s16 *out4E, u8 *out4D,
                   f32 *out48) {
    out6C->x = a0->x + a2->x * sp10;
    out6C->y = a0->y + a2->y * sp10;
    out6C->z = a0->z + a2->z * sp10;
    out60->x = a1->x + a3->x * sp10;
    out60->y = a1->y + a3->y * sp10;
    out60->z = a1->z + a3->z * sp10;
    out54->x = (out60->x - out6C->x) * sp14->unk74;
    out54->y = (out60->y - out6C->y) * sp14->unk74;
    out54->z = (out60->z - out6C->z) * sp14->unk74;
    *out50 = (func_150ADA68() * 217.0f + -456.0f) * D_800A482C;
    *out4E = func_150ADA20() % 31U + 30;
    *out4D = func_150ADA20() % 156U + 100;
    *out48 = func_150ADA68() * 35.0f + 40.0f;
}

extern u8 D_800A3FD8[];
void func_15143134(void *, s32, s32);

s32 func_151380B4(s32 *arg0, s32 arg1, s32 arg2) {
    s32 temp_v0 = *(s32 *)((u8 *)arg0 + 0x1D4);
    if (temp_v0 == 0) {
        return 0;
    }
    if ((*(u8 *)((u8 *)arg0 + 0x74) & 0xF) == 0xF) {
        return 0;
    }
    func_15143134(&D_800A3FD8[arg1 << 4], arg2, temp_v0 + 0x300);
    return 1;
}

typedef struct {
    u8 unk00;
    u8 pad01[3];
    struct127 *unk04;
    u8 unk08;
    u8 pad09[3];
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 pad25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    u8 unk2E;
    u8 unk2F;
    s8 unk30;
    u8 pad31[3];
    f32 unk34;
    u8 unk38;
    s8 unk39;
    u8 pad3A[2];
} Local15138120;

void func_15138120(struct127 *arg0, s32 arg1, u8 arg2) {
    Local15138120 tmp;

    if (D_800A3FD8[(arg1 << 4) + 0xE] != 2) {
        tmp.unk00 = arg0->unique_id;
        tmp.unk04 = arg0;
        if (arg2) {
            tmp.unk08 = 0xC;
        } else {
            tmp.unk08 = 1;
        }
        tmp.unk24 = 2;
        tmp.unk26 = 0x28;
        tmp.unk28 = 0x10;
        tmp.unk0C = 0.0f;
        tmp.unk10 = 0.0f;
        tmp.unk14 = 0.0f;
        tmp.unk18 = 0.0f;
        tmp.unk20 = 0.0f;
        tmp.unk1C = 20.0f;
        if ((D_800A4058 == &D_800A3FD8[arg1 << 4]) || (D_800A4068 == &D_800A3FD8[arg1 << 4])) {
            tmp.unk2A = (arg0->unk94 & 0xE) ? 0x78 : 0xF0;
        } else {
            tmp.unk2A = 0x258;
        }
        tmp.unk2E = 5;
        if (D_800A3FD8[(arg1 << 4) + 0xE] == 0) {
            tmp.unk2F = 5;
        } else {
            tmp.unk2F = 6;
        }
        tmp.unk30 = -1;
        tmp.unk38 = 0;
        tmp.unk39 = -1;
        tmp.unk34 = 1.0f;
        tmp.unk2C = func_1000FA64(0x4FE, (s16)(s32)arg0->x_position,
                                  (s16)(s32)arg0->y_position,
                                  (s16)(s32)arg0->z_position, 0x5DC0,
                                  0x258, 0x12C, (s32)func_1000EBC4,
                                  (void *)0x78, 0, 0, 0);
        func_15134DAC(&tmp, 0);
    }
}

typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
} Vec151382E0;

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    Vec151382E0 unk08;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    s8 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s32 unk48;
} LocalDef151382E0;

void func_151382E0(Vec151382E0 *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4) {
    LocalDef151382E0 tmp;
    s32 temp_v0;

    temp_v0 = D_800A3FE6[arg1 << 4];
    if (temp_v0 != 2) {
        tmp.unk08 = *arg0;
        tmp.unk14 = D_800A4830;
        tmp.unk18 = D_800A4834;
        tmp.unk1C = D_800A4838;
        tmp.unk20 = D_800A483C;
        tmp.unk24 = 4.0f;
        tmp.unk28 = 9.0f;
        tmp.unk2C = 0x12;
        tmp.unk2E = 7;
        tmp.unk02 = 0xFF;
        tmp.unk00 = 0;
        tmp.unk04 = -0x3F;
        tmp.unk06 = 0x4E;
        tmp.unk30 = 3;
        tmp.unk32 = 3;
        tmp.unk34 = 0x14;
        tmp.unk36 = 0x1E;
        tmp.unk38 = 0x9B;
        tmp.unk3A = 0x64;
        tmp.unk44 = 0x10;
        tmp.unk46 = 0xF;
        tmp.unk48 = 0;
        tmp.unk40 = D_800A4840;
        if (temp_v0 == 1) {
            tmp.unk3C = 1;
        } else {
            tmp.unk3C = 0;
        }
        func_15153F18(&tmp, &tmp.unk08, arg2, arg3, arg4);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15138424.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151389A8.s")

void func_15138BC0(struct102 *arg0, u8 arg1, s32 arg2) {
    u8 sp50[8];
    u8 sp28[0x27];
    s32 temp_s1;

    temp_s1 = func_15134070(arg0);
    if (temp_s1 != 0x63) {
        sp28[0x1F] = func_151380B4(arg0, temp_s1, sp50 - 8);
        func_15138120(arg0, temp_s1, 1);
        if (sp28[0x1F] != 0) {
            func_1504715C(sp28 - 8, arg0);
            func_151382E0(sp50 - 8, temp_s1, sp28 - 8, arg1, arg2);
            func_15138424(arg0, sp50 - 8, temp_s1, sp28 - 8, arg1, arg2);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15138C80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15138E98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15139578.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15139768.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15139D74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513A24C.s")

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_A48C;

typedef struct {
    s32 field_0;
    s32 field_4;
    Vec3i_A48C position;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    s16 field_1A;
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    s16 field_2C;
    s16 field_2E;
    f32 field_30;
    f32 field_34;
    f32 field_38;
} BloodA48CConfig;

void func_15152190(void *, void *, void *, s32, f32, s32, s32, s32);
extern u8 D_800A4260[];
extern u8 D_800A4264[];

void func_1513A48C(void *arg0, u8 arg1, s32 arg2) {
    BloodA48CConfig config;

    config.field_0 = 8;
    config.field_4 = 4;
    config.position = *(Vec3i_A48C *)arg0;
    config.field_14 = 0;
    config.field_16 = 0xFF;
    config.field_18 = -0x37;
    config.field_1A = 0x20;
    config.field_2C = 0x28;
    config.field_2E = 0x14;
    config.field_30 = 0.07f;
    config.field_34 = 0.07f;
    config.field_1C = 10.0f;
    config.field_20 = 9.0f;
    config.field_24 = -1.0040001f;
    config.field_28 = 0.254f;
    config.field_38 = 23.446001f;
    func_15152190(&config, D_800A4260, D_800A4264, 1, 0.0f, 1,
                  (s32)arg1, arg2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513A594.s")

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3i_1513A5E0;

typedef struct {
    s32 field_0;
    s32 field_4;
    Vec3i_1513A5E0 position;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    s16 field_1A;
    f32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    s16 field_2C;
    s16 field_2E;
    f32 field_30;
    f32 field_34;
    f32 field_38;
} BloodA5E0Config;

void func_15152190(void *, void *, void *, s32, f32, s32, s32, s32);
extern u8 D_800A4268[];
extern u8 D_800A4270[];

void func_1513A5E0(s32 arg0, s32 arg1, s32 arg2) {
    BloodA5E0Config config;

    config.field_0 = 7;
    config.field_4 = 7;
    config.position = *(Vec3i_1513A5E0 *)arg0;
    config.field_14 = 0;
    config.field_16 = 0xFF;
    config.field_18 = -0x32;
    config.field_1A = 0x1B;
    config.field_1C = 4.0f;
    config.field_20 = 4.0f;
    config.field_2C = 0x19;
    config.field_2E = 0x28;
    config.field_30 = 0.0201f;
    config.field_34 = 0.0201f;
    config.field_24 = -0.43f;
    config.field_28 = 0.272f;
    config.field_38 = 20.187f;
    func_15152190(&config, D_800A4268, D_800A4270, 2, 0.0f, 1,
                  (u8)arg1, arg2);
}

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    struct17 unk10;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct17 unk28;
    struct17 unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
    s8 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    s8 unk68;
    u8 unk69;
    s8 unk6A;
    void *unk6C;
    u8 unk70;
    s16 unk72;
    s16 unk74;
    s32 unk78;
} SpawnA6E0;

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x74 - 0x3C];
    u8 unk74;
    u8 pad75[0x14C - 0x75];
    f32 unk14C;
    f32 unk150;
    u8 pad154[0x1D4 - 0x154];
    s32 unk1D4;
} ObjA6E0;

extern u8 D_800A4278[];
extern s32 D_800A4280[];
extern s32 D_800A4290[];
extern struct17 D_800A42A0[];
extern void func_1503F404(s32, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *);
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
extern s32 func_1513264C(void *, s32, s32, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, f32 *);
f32 func_150ADA68(void);

void func_1513A6E0(ObjA6E0 *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4) {
    u8 idx;
    u8 i;
    u8 mask;
    struct17 v;

    idx = func_150ADA20() % 6U;
    {
    SpawnA6E0 spawn;
    u8 extra[4];
    s32 pad1;
    extra[0] = 0xC;
    spawn.unk00 = 1.0f;
    spawn.unk04 = 1.0f;
    spawn.unk08 = spawn.unk0C = (arg0->unk14C + arg0->unk150) * 0.5f;
    spawn.unk10.unk0 = 0.0f;
    spawn.unk10.unk4 = 0.0f;
    spawn.unk10.unk8 = 0.0f;
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.unk28.unk0 = 0.0f;
    spawn.unk28.unk4 = 0.0f;
    spawn.unk28.unk8 = 0.0f;
    spawn.unk34.unk0 = 0.0f;
    spawn.unk34.unk4 = 0.0f;
    spawn.unk34.unk8 = 0.0f;
    spawn.unk40 = 0.0f;
    spawn.unk44 = 0.0f;
    spawn.unk48 = 0.0f;
    spawn.unk4C = 0.0f;
    spawn.unk50 = 0x101900;
    spawn.unk54 = 0x64;
    spawn.unk58 = 5;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 0;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 5;
    spawn.unk6A = 1;
    spawn.unk6C = arg0;
    spawn.unk70 = arg0->unk3B;
    spawn.unk72 = 0xC;
    spawn.unk74 = 0x15;
    for (i = 0, mask = 1; i < 4; i++, mask <<= 1) {
        if (D_800A4278[idx] & mask) {
            s32 ret;
            spawn.unk56 = D_800A4280[i];
            ret = func_15132A4C(&spawn, 3, 0xFF, 1, arg3, arg4);
            if (ret != 0) {
                memcpy((void *)(ret + 0x170), extra, 1);
            }
        }
    }
    }
    if (arg0->unk1D4 != 0 && (arg0->unk74 & 0xF) != 0xF) {
        struct17 a;
        struct17 b;
        SpawnA6E0 spawn2;
        u8 mask2;
        u32 r1;
        u32 r2;
        func_1513A5E0(arg1, arg3 & 0xFF, arg4);
        {
        s32 q;
        func_1503F404(arg0->unk1D4 + 0x300, &a.unk0, &a.unk4, &a.unk8, &v.unk0, &v.unk4, &v.unk8, &b.unk0, &b.unk4, &b.unk8);
        }
        spawn2.unk00 = 1.0f;
        spawn2.unk04 = 0.6490000486f;
        spawn2.unk08 = spawn2.unk0C = (arg0->unk14C + arg0->unk150) * 0.5f;
        spawn2.unk10 = v;
        spawn2.unk1C = 1.0f;
        spawn2.unk20 = 0.0f;
        spawn2.unk24 = 1.0f;
        spawn2.unk44 = 0.0f;
        spawn2.unk58 = 0;
        spawn2.unk5C = 0;
        spawn2.unk60 = 0xFF;
        spawn2.unk62 = 0;
        spawn2.unk64 = 0;
        spawn2.unk65 = 0;
        spawn2.unk66 = 0;
        spawn2.unk67 = 0;
        spawn2.unk6A = 1;
        spawn2.unk6C = arg0;
        spawn2.unk70 = arg0->unk3B;
        spawn2.unk72 = 0xC;
        spawn2.unk74 = 0x15;
        spawn2.unk50 = 0x1039E9;
        spawn2.unk61 = 0xD;
        spawn2.unk68 = 2;
        spawn2.unk63 = 9;
        for (i = 0, mask2 = 1; i < 4; i++, mask2 <<= 1) {
            if (!(D_800A4278[idx] & mask2)) {
                spawn2.unk56 = D_800A4290[i];
                func_15143134(&D_800A42A0[i], (s32)&spawn2.unk28, arg0->unk1D4 + 0x300);
                r1 = func_150ADA20();
                r2 = func_150ADA20();
                func_15143794(r1 & 0xFF, r2 % 35U - 84, func_150ADA68() * 6.119999886f + 3.0f, &spawn2.unk34.unk0);
                spawn2.unk40 = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.unk48 = func_150ADA68() * 30.7460022f + -15.3730011f;
                spawn2.unk4C = func_150ADA68() * 0.2080000043f + -0.7080000043f;
                spawn2.unk54 = func_150ADA20() % 101U + 60;
                func_1513264C(&spawn2, 3, 0xFF, arg2, 0, arg3, arg4);
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513ABB8.s")

void func_1513B0B8(void *arg0, s32 arg1, u8 arg2) {
    s32 *temp_v0 = (s32 *)((s32)arg0 + 0x170);

    if (arg2 == 0x45) {
        if (--(*temp_v0) < 0) {
            *(s32 *)((s32)arg0 + 0x60) |= 0x80;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513B0F8.s")
