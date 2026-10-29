#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 (*D_8008B078[])(void *);
extern s32 (*D_8008B07C[])(void *);
s32 func_1514401C(u8, void *, void *, u8);

struct Struct1515C158Inner {
    u8 pad0[0x8];
    struct Struct1515C158Inner *next;
    u8 padC[0x38];
    s32 unk44;
    s32 unk48;
};

extern u8 D_800DCE50[];

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 pad13;
    s16 unk14;
    u8 pad16[0x2];
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x18];
    s8 unk38;
    s8 unk39;
} Struct1515BAE0;

void func_1515BAE0(Struct1515BAE0 *arg0) {
    s32 temp;
    u8 failed;

    failed = 0;
    if (arg0->unk11 & 1) {
        arg0->unk14 -= D_800BE9E4;
        if (arg0->unk14 < 0) {
            failed = 1;
        }
    }
    if (failed == 0) {
        temp = arg0->unk38;
        if (temp != -1) {
            if (D_8008B078[temp](arg0) == 0) {
                failed = 1;
            }
        }
        if (arg0->unk1C != 0) {
            failed = func_1514401C(arg0->unk10, &arg0->unk1C, &arg0->unk18, arg0->unk12);
        }
    }
    if (failed) {
        temp = arg0->unk39;
        if (temp != -1) {
            if (D_8008B07C[temp](arg0) != 0) {
                func_1516972C((struct102 *) arg0);
            }
        } else {
            func_1516972C((struct102 *) arg0);
        }
    }
}

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
void func_151441A4(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, u8 arg8, u8 arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13);
void func_151442FC(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13);

typedef struct {
    u8 pad0[0xC];
} Struct1515BBF0_tex;
typedef struct {
    s32 unk0;
    s32 unk4;
} Struct1515BBF0_mode;
extern Struct1515BBF0_tex D_80090B60[];
extern Struct1515BBF0_mode D_800A4AC8[];
extern s32 D_800D2C9C;

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 unk11;
    u8 pad12[0x6];
    s32 unk18;
    u8 pad1C[0x4];
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 pad2D[0x3];
    s32 unk30;
    s32 unk34;
    u8 pad38[0x18];
    u8 unk50[1];
} Struct1515BBF0;

Gfx *func_1515BBF0(Gfx *gfx, Struct1515BBF0 *arg1, s32 arg2) {
    s32 pad;
    u8 update;
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;

    update = 1;
    func_151441A4(&primR, &primG, &primB, &primA, arg1->unk20, arg1->unk21, arg1->unk22, arg1->unk23,
                  arg1->unk24, arg1->unk25, arg1->unk26, arg1->unk27, arg1->unk28, arg1->unk29);
    func_151442FC(&envR, &envG, &envB, &envA, arg1->unk20, arg1->unk21, arg1->unk22, arg1->unk23,
                  arg1->unk24, arg1->unk25, arg1->unk26, arg1->unk27, arg1->unk28, arg1->unk2A);
    gfx = func_15142B7C(gfx, arg1->unk30, arg1->unk34);
    gfx = func_15142C10(gfx, envR, envG, envB, envA, &update);
    gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &update);
    gfx = func_15142E24(gfx, (s32)&D_80090B60[arg1->unk10], arg1->unk18, 0, 0, 0, arg1->unk10, 0, 0, &update, 3);
    gfx = func_1513F4E4(gfx, arg1->unk2C, &update);
    gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | ((arg1->unk11 & 2) ? G_CYC_2CYCLE : G_CYC_1CYCLE) | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[arg1->unk2B].unk0 | D_800A4AC8[arg1->unk2B].unk4, &update);
    gSPVertex(gfx++, arg1->unk50, 4, 0);
    gSP1Triangle(gfx++, 0, 1, 2, 0);
    gSP1Triangle(gfx++, 0, 2, 3, 0);
    return gfx;
}

void func_1515BE48(void) {
}

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

struct Struct1515BE50_src {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

struct Struct1515BE50_ret {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x8];
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    s32 unk44;
    s32 unk48;
};

s32 func_1515BE50(struct Struct1515BE50_src **arg0, s32 arg1, s32 arg2, s32 arg3) {
    struct Struct1515BE50_ret *temp_v0;
    struct Struct1515BE50_ret *sp2C;

    if (*arg0 == 0) {
        return 0;
    }
    temp_v0 = (struct Struct1515BE50_ret *)func_15167A68(0x32, arg3, arg1 + 0x50, 1, (u8)arg2, 1);
    if (temp_v0 == 0) {
        return 0;
    }
    sp2C = temp_v0;
    memcpy((s32)temp_v0 + 0x18, arg0, 8);
    sp2C->unk20 = (*arg0)->unk14;
    sp2C->unk24 = (*arg0)->unk18;
    sp2C->unk28 = (*arg0)->unk1C;
    sp2C->unk2C = (*arg0)->unk14;
    sp2C->unk30 = (*arg0)->unk18;
    sp2C->unk34 = (*arg0)->unk1C;
    sp2C->unk44 = 0;
    sp2C->unk48 = -1;
    sp2C->unk10 = 1;
    sp2C->unk14 = 0;
    sp2C->unk38 = 0.0f;
    sp2C->unk3C = 0.0f;
    sp2C->unk40 = 0.0f;
    return sp2C;
}

extern void func_1514EDF0(struct225 *, s32);

void func_1515BF50(struct225 *arg0) {
    func_1514EDF0(arg0, (s32)arg0->unk18);
    func_15169804((struct102 *)arg0);
}

void func_1515BF7C(struct225 *arg0) {
    func_1514EDF0(arg0, (s32)arg0->unk18);
    func_15169824((struct102 *)arg0);
}

struct Struct1515BFA8Src {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
};

struct Vec1515BFA8 {
    f32 x;
    f32 y;
    f32 z;
};

struct Struct1515BFA8 {
    u8 pad0[0x18];
    struct Struct1515BFA8Src *unk18;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    struct Vec1515BFA8 unk20;
    struct Vec1515BFA8 unk2C;
    struct Vec1515BFA8 unk38;
};

void func_1515BFA8(struct Struct1515BFA8 *arg0) {
    s32 remove;
    struct Struct1515BFA8Src *src;

    remove = 0;
    if (arg0->unk1D & 1) {
        arg0->unk1E -= D_800BE9E4;
        if (arg0->unk1E < 0) {
            remove = 1;
        }
    }
    src = arg0->unk18;
    if ((src->unk0 == 0) || (src->unk3B != arg0->unk1C)) {
        remove = 1;
    }
    if (remove == 0) {
        arg0->unk2C = arg0->unk20;
        src = arg0->unk18;
        arg0->unk20.x = src->unk14;
        arg0->unk20.y = src->unk18;
        arg0->unk20.z = src->unk1C;
        arg0->unk38.x = (arg0->unk2C.x - arg0->unk20.x) * D_800BE9A8;
        arg0->unk38.y = (arg0->unk2C.y - arg0->unk20.y) * D_800BE9A8;
        arg0->unk38.z = (arg0->unk2C.z - arg0->unk20.z) * D_800BE9A8;
    }
    if (remove != 0) {
        func_1516972C((struct102 *)arg0);
    }
}

void func_1515C0B8(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x18, arg0 + 0x1C, arg0);
}

s32 func_1514ECE0(void *, s16, void *);

s32 func_1515C0F8(struct102 *arg0, s32 *arg1) {
    s32 *temp;
    s32 v;

    if (arg0 == 0) {
        return 0;
    }
    if (func_1514ECE0(*(void **)((u8 *)arg0 + 0x2F4), 0x16, &temp) != 0) {
        v = *(s32 *)((u8 *)temp + 0x10);
        *arg1 = v + 0x38;
        return 1;
    }
    return 0;
}

void func_1515C158(void) {
    s32 i;
    struct Struct1515C158Inner *node;

    i = 0;
    do {
        node = *(struct Struct1515C158Inner **)&D_800DCE50[i + 0xC8];
        i += 0x1A0;
        if (node != 0) {
            do {
                node->unk44 = 0;
                node->unk48 = -1;
                node = node->next;
            } while (node != 0);
        }
    } while ((u8 *)&D_800DD190 != &D_800DCE50[i]);
}

struct Struct1515C1A0 {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0xF];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0xB2];
    s16 unkD2;
    s16 unkD4;
    s16 unkD6;
};

void func_1515C1A0(struct Struct1515C1A0 *arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    u8 temp_v0;
    f32 temp_f0;

    temp_v0 = arg0->unk4;
    if ((temp_v0 < 0xBB) && (temp_v0 != 0xFF)) {
        *arg2 = arg0->unkD2;
        *arg3 = arg0->unkD4;
        arg1[0] = arg0->unk14;
        arg1[1] = arg0->unk18 + arg0->unkD6;
        arg1[2] = arg0->unk1C;
    } else {
        temp_f0 = 1.0f;
        *arg2 = temp_f0;
        *arg3 = temp_f0;
        arg1[0] = arg0->unk14;
        arg1[1] = arg0->unk18;
        arg1[2] = arg0->unk1C;
    }
}

struct Struct1515C244 {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0xF];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0xC4];
    s16 unkE4;
    s16 unkE6;
    s16 unkE8;
};

void func_1515C244(struct Struct1515C244 *arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    u8 temp_v0;
    f32 temp_f0;

    temp_v0 = arg0->unk4;
    if ((temp_v0 < 0xBB) && (temp_v0 != 0xFF)) {
        *arg2 = arg0->unkE4;
        *arg3 = arg0->unkE6;
        arg1[0] = arg0->unk14;
        arg1[1] = arg0->unk18 + arg0->unkE8;
        arg1[2] = arg0->unk1C;
    } else {
        temp_f0 = 1.0f;
        *arg2 = temp_f0;
        *arg3 = temp_f0;
        arg1[0] = arg0->unk14;
        arg1[1] = arg0->unk18;
        arg1[2] = arg0->unk1C;
    }
}
