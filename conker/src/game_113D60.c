#include <ultra64.h>
/* include/functions.h declares func_15169260's first parameter as s32; func_150E8930 passes
 * a struct address that IDO must build directly in a0, which only a pointer parameter
 * produces.  The header declaration is renamed out of the way and a file-local prototype
 * is used instead (same approach as game_10CD70.c). */
#define func_15169260 func_15169260_s32_decl_in_functions_h
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_15169260
#undef func_150ADA20
u32 func_150ADA20(void);
void func_15169260(void *, s32, s32, s32);
#include "variables.h"

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
} Struct150E81A8Vec;

typedef struct {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ Struct150E81A8Vec unk10;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ s8 unk58;
} Struct150E81A8;

typedef struct {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 unk5;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 unk7;
} Struct150E81A8Small;

extern Struct150E81A8Vec D_800A1290[];
extern f32 *D_80088A3C;
extern f32 *D_80088A40;
extern f32 D_800A130C;
extern f32 D_800A1354;
extern f32 D_800A1358;
extern f32 D_800A135C;
extern f32 D_800A1360;
extern f32 D_800A1364;
extern f32 D_800A1310;
extern f32 D_800A1314;
extern f32 D_800A1318;
extern f32 D_800A131C;
extern f32 D_800A1320;
extern void func_151D3FF4(Struct150E81A8Vec *arg0, u8 arg1, s32 arg2);
extern void func_1514FCE8(Struct150E81A8 *arg0, u8 arg1, s32 arg2);
extern void *func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 func_151337C0(f32 *arg0);


#pragma GLOBAL_ASM("asm/nonmatchings/game_113D60/func_150E68B0.s")

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 ampX;
    /* 0x0C */ f32 ampY;
    /* 0x10 */ f32 phaseX;
    /* 0x14 */ f32 phaseY;
    /* 0x18 */ f32 speedX;
    /* 0x1C */ f32 speedY;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 count;
    /* 0x2C */ u8 unk2C[0xC];
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 unk3E;
    /* 0x40 */ u8 unk40;
} Struct150E6B84Sub;

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ u8 sub[0x24];
    /* 0x4C */ f32 rate;
    /* 0x50 */ f32 accum;
} Struct150E6B84;

extern f32 sinf(f32);
extern void func_151436B4(f32, f32, f32, f32 *);
extern s32 func_150E5FD0(void *, f32 *, f32, f32, f32, u8, s32, u8, u8, s16, s8);
extern f32 func_15144528(f32, f32, f32);

void func_150E6B84(Struct150E6B84 *arg0) {
    Struct150E6B84Sub *sub;
    f32 inv;
    f32 y;
    f32 step;
    f32 pos[9];
    f32 phaseX;
    f32 phaseY;
    f32 vx;
    f32 vy;
    f32 s1;
    f32 s2;
    u32 rnd;
    u32 rnd2;

    sub = (Struct150E6B84Sub *)arg0->sub;
    arg0->accum += arg0->rate * D_800BE9A4;
    if (arg0->accum > 1.0f) {
        inv = 1.0f / sub->count;
        y = sub->unk20 + D_800BE9A4;
        step = y * inv;
        phaseX = sub->phaseX;
        phaseY = sub->phaseY;
        vx = sub->speedX * D_800BE9A4;
        vy = sub->speedY * D_800BE9A4;
        do {
            s1 = sinf(phaseX);
            s2 = sinf(phaseY);
            func_151436B4(sub->x + s1 * sub->ampX, sub->y + s2 * sub->ampY, 5.0f, pos);
            s1 = func_150ADA68();
            s2 = func_150ADA68();
            rnd = func_150ADA20();
            rnd2 = func_150ADA20();
            func_150E5FD0(sub->unk2C, pos, sub->unk38, (s1 * 300.0f + 100.0f) * 0.1000000015f,
                          (s2 * 700.0f + 300.0f) * 0.1000000015f, arg0->unkC, arg0->unk1, sub->unk40,
                          rnd % 86 + 0xAA, rnd2 % (u32)(sub->unk3E + 1) + sub->unk3C, -1);
            phaseX += vx * inv;
            phaseY += vy * inv;
            y -= step;
            sub->count -= 1.0f;
        } while (sub->count > 1.0f);
        sub->phaseX = func_15144528(phaseX, 6.283185482f, 0.0f);
        sub->phaseY = func_15144528(phaseY, 6.283185482f, 0.0f);
        sub->unk20 = y;
    }
}

void func_150E6E34(f32 *out) {
    f32 *p;
    f32 t;

    if (func_150ADA68() < D_800A130C) {
        p = D_80088A3C;
    } else {
        p = D_80088A40;
    }
    t = func_150ADA68();
    out[0] = p[0] + (p[3] - p[0]) * t;
    out[1] = p[1] + (p[4] - p[1]) * t;
    out[2] = p[2] + (p[5] - p[2]) * t;
}

extern s32 func_1514470C(struct134 *, s32);

void func_150E6ED8(s32 arg0) {
    func_1514470C((&D_800D9A20)[func_150ADA20() & 1], arg0);
}

extern f32 *D_80088A44[];

void func_150E6F18(f32 *out) {
    f32 *p;
    f32 t;
    p = D_80088A44[(u32)func_150ADA20() % 6];
    t = func_150ADA68();
    out[0] = p[0] + (p[3] - p[0]) * t;
    out[1] = p[1] + (p[4] - p[1]) * t;
    out[2] = p[2] + (p[5] - p[2]) * t;
}

typedef struct {
    u8 pad0[0x38];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
} Bone150E6FAC;

typedef struct {
    u8 pad0[0x10];
    Bone150E6FAC *unk10;
} Node150E6FAC;

typedef struct {
    u8 pad0[0x14];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0x2D4];
    void *unk2F4;
} Obj150E6FAC;

s32 func_1514ECE0(void *, s16, void *);

void func_150E6FAC(f32 *out, Obj150E6FAC *obj) {
    Node150E6FAC *node;
    f32 c;
    f32 radius;
    s16 angle;
    f32 s;

    if (func_1514ECE0(obj->unk2F4, 0x16, &node) != 0) {
        Bone150E6FAC *bone;

        radius = (func_150ADA68() * 100.0f) + 80.0f;
        angle = func_150ADA20() & 0xFF;
        s = func_151423D8(angle - 0x40);
        c = func_151423D8(angle);
        bone = node->unk10;
        out[0] = obj->x + (bone->unk38 * -80.0f) + (s * radius);
        out[1] = obj->y + (bone->unk3C * -80.0f) + 100.0f;
        out[2] = obj->z + (bone->unk40 * -80.0f) + (c * radius);
    } else {
        out[0] = obj->x;
        out[1] = obj->y;
        out[2] = obj->z;
    }
}

void func_150E70CC(f32 *arg0, f32 *arg1) {
    arg0[0] = arg1[5];
    arg0[1] = arg1[6];
    arg0[2] = arg1[7];
}

void func_150E70EC(s32 arg0, s32 arg1, f32 *arg2, f32 *out) {
    f32 temp_f2;
    f32 temp_f16;

    out[0] = func_150484A0(arg2[0], arg2[2]);
    out[2] = func_150ADA68() * D_800A1310;
    out[4] = func_150ADA68() * D_800A1314;
    out[6] = func_150ADA68() * 0.5f;
    temp_f2 = arg2[0];
    temp_f16 = arg2[2];
    out[1] = func_150484A0(sqrtf((temp_f2 * temp_f2) + (temp_f16 * temp_f16)), arg2[1]) - D_800A1318;
    out[3] = func_150ADA68() * D_800A131C;
    out[5] = func_150ADA68() * D_800A1320;
    out[7] = func_150ADA68() * 0.5f;
}

extern f32 D_800A1324;
extern f32 D_800A1328;
extern f32 D_800A132C;
extern f32 D_800A1330;
extern f32 D_800A1334;
extern f32 D_800A1338;
extern f32 D_800A133C;

void func_150E71E4(s32 arg0, s32 arg1, f32 *arg2, f32 *out) {
    out[0] = func_150484A0(arg2[0], arg2[2]);
    out[2] = D_800A1324;
    out[4] = func_150ADA68() * D_800A1328;
    out[6] = func_150ADA68() * D_800A132C;
    out[1] = D_800A1330;
    out[3] = D_800A1334;
    out[5] = func_150ADA68() * D_800A1338;
    out[7] = func_150ADA68() * D_800A133C;
}

typedef struct {
    /* 0x0 */ f32 unk0;
    /* 0x4 */ f32 unk4;
} Struct150E75A0Pos;

typedef struct {
    /* 0x0 */ s32 unk0[2];
} Struct150E7290Tbl;

extern Struct150E7290Tbl D_80088A5C;
void func_150E75A0(Struct150E75A0Pos *arg0, f32 arg1, s16 arg2, u8 arg3, u8 arg4, u8 arg5, s16 arg6, s16 arg7, s32 arg8,
                   s32 arg9, u8 argA, s32 argB);
void func_150E76D0(f32 arg0, s16 arg1, u8 arg2, u8 arg3, u8 arg4, s16 arg5, s16 arg6, s32 arg7, s32 arg8, u8 arg9, s32 arg10);
extern void func_15164F0C(u8, u8, void *, u8, s32);

void func_150E7290(u8 arg0, u8 arg1, s32 arg2) {
    Struct150E7290Tbl sp60;
    Struct150E75A0Pos sp58;
    Struct150E81A8Small sp50;
    s32 sp4C;
    u32 sp48;
    u32 sp44;
    u32 sp40;
    f32 sp3C;

    if (func_150ADA68() < 0.00419999985f) {
        sp60 = D_80088A5C;
        if (func_150ADA68() < 0.7500000596f) {
            sp58.unk0 = (func_150ADA68() * 300.0f) + -150.0f;
            sp58.unk4 = (func_150ADA68() * 200.0f) + -100.0f;
            sp3C = func_150ADA68();
            sp40 = func_150ADA20();
            sp44 = (func_150ADA20() & 1) ? 2 : 0;
            sp48 = (func_150ADA20() & 1) ? 4 : 0;
            func_150E75A0(&sp58, ((sp3C * 150.0f) + 200.0f) * 0.1000000015f, (sp40 % 201) + 500, (u8)(sp48 | sp44) | 9,
                          (func_150ADA20() % 26) + 100, 0xFF, 0x40, 3, (s32)&sp60, 2, arg1, arg2);
        } else {
            sp3C = func_150ADA68();
            sp48 = func_150ADA20();
            func_150E76D0(((sp3C * 100.0f) + 150.0f) * 0.1000000015f, (sp48 % 201) + 500, 9,
                          (func_150ADA20() % 26) + 100, 0xFF, 0x40, 3, (s32)&sp60, 2, arg1, arg2);
        }
        func_10010F30(0x360, 0x7FFF, 0, 0, 0);
        func_15164F0C(1, arg0, NULL, arg1, arg2);
        sp50.unk0 = 1;
        sp50.unk2 = (func_150ADA20() % 26) + 25;
        sp50.unk5 = 1;
        sp50.unk4 = (func_150ADA20() % 6) + 3;
        sp50.unk6 = -1;
        func_151D8868(&sp50, 0, 0xFF, 0);
    }
}


typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
} Struct150E76D0;

typedef struct {
    /* 0x0 */ s32 unk0[3];
} Struct150E76D0Tbl;

extern Struct150E76D0Tbl D_80088A68;
extern Struct150E76D0Tbl D_80088A74;
extern void *func_1515548C(Struct150E76D0 *, s32, s32, s32, s32, u8, s32);

typedef struct {
    /* 0x0 */ u8 unk0[1];
} Struct150E75A0Tbl;

extern Struct150E75A0Tbl D_80088A64;

void func_150E75A0(Struct150E75A0Pos *arg0, f32 arg1, s16 arg2, u8 arg3, u8 arg4, u8 arg5, s16 arg6, s16 arg7, s32 arg8,
                   s32 arg9, u8 argA, s32 argB) {
    Struct150E76D0 sp2C;
    Struct150E75A0Tbl sp28;

    sp28 = D_80088A64;
    *(Struct150E75A0Pos *)&sp2C.unk00 = *arg0;
    sp2C.unk08 = arg1;
    sp2C.unk0C = arg1;
    sp2C.unk10 = sp28.unk0[0];
    sp2C.unk12 = arg2;
    sp2C.unk14 = arg3 | 0x40;
    sp2C.unk16 = arg6;
    sp2C.unk18 = arg7;
    sp2C.unk1A = 4;
    sp2C.unk1B = 0xFF;
    sp2C.unk1C = 0xE6;
    sp2C.unk1D = 0xBE;
    sp2C.unk1E = arg4;
    sp2C.unk1F = 0xFF;
    sp2C.unk20 = 0xFF;
    sp2C.unk21 = 0xFF;
    sp2C.unk22 = 0xFF;
    sp2C.unk23 = arg5;
    sp2C.unk24 = 1;
    sp2C.unk28 = 0;
    sp2C.unk2C = 0;
    sp2C.unk40 = 0;
    sp2C.unk41 = 0xA;
    sp2C.unk30 = 7;
    sp2C.unk34 = 0x3C;
    sp2C.unk38 = 0x80;
    sp2C.unk3C = 0x20;
    func_1515548C(&sp2C, 0, arg8, arg9, 0, argA, argB);
}


void func_150E76D0(f32 arg0, s16 arg1, u8 arg2, u8 arg3, u8 arg4, s16 arg5, s16 arg6, s32 arg7, s32 arg8, u8 arg9, s32 arg10) {
    Struct150E76D0 sp54;
    u8 sp53;

    sp53 = func_150ADA20() & 3;

    sp54.unk12 = arg1;
    sp54.unk14 = arg2 & 0xFFF9;
    sp54.unk16 = arg5;
    sp54.unk18 = arg6;
    sp54.unk1A = 5;
    sp54.unk1B = 0xFF;
    sp54.unk1C = 0xE6;
    sp54.unk1D = 0xBE;
    sp54.unk1E = arg3;
    sp54.unk1F = 0xFF;
    sp54.unk20 = 0xFF;
    sp54.unk21 = 0xFF;
    sp54.unk22 = 0xFF;
    sp54.unk23 = arg4;
    sp54.unk24 = 1;
    sp54.unk28 = 0;
    sp54.unk2C = 0;
    sp54.unk40 = 0;
    sp54.unk41 = 0xA;
    sp54.unk30 = 7;
    sp54.unk34 = 0x3C;
    sp54.unk38 = 0x80;
    sp54.unk3C = 0x20;
    sp54.unk0C = arg0;
    sp54.unk08 = arg0;

    switch (sp53) {
    case 0:
    case 1:
        {
            Struct150E76D0Tbl sp44;

            sp44 = D_80088A68;
            sp54.unk10 = sp44.unk0[(u32)func_150ADA20() % 3];
            sp54.unk04 = (func_150ADA68() * 160.0f) + -80.0f;
            switch (sp53) {
            case 0:
                sp54.unk14 |= 2;
                sp54.unk00 = 145.0f - arg0;
                break;
            case 1:
                sp54.unk00 = arg0 - 145.0f;
                break;
            }
        }
        break;
    case 2:
    case 3:
        {
            Struct150E76D0Tbl sp38;

            sp38 = D_80088A74;
            sp54.unk10 = sp38.unk0[(u32)func_150ADA20() % 3];
            sp54.unk00 = (func_150ADA68() * 260.0f) + -130.0f;
            switch (sp53) {
            case 2:
                sp54.unk14 |= 4;
                sp54.unk04 = 110.0f - arg0;
                break;
            case 3:
                sp54.unk04 = arg0 - 110.0f;
                break;
            }
        }
        break;
    }

    func_1515548C(&sp54, 0, arg7, arg8, 0, arg9, arg10);
}

typedef struct {
    /* 0x0 */ f32 unk0;
    /* 0x4 */ f32 unk4;
    /* 0x8 */ s16 unk8;
    /* 0xA */ s16 unkA;
} Struct150E7994;

extern f32 func_15142A80(f32);
extern f32 func_15142AC0(f32);
extern f32 func_15142B04(f32);
extern f32 func_15142B44(f32);

struct260 *func_150E7994(s16 arg0, f32 arg1, u8 arg2, s32 arg3) {
    struct260 *obj;
    Struct150E7994 spB8;
    Struct150E81A8Small spB0;
    f32 sp90[8];
    f32 *out;
    f32 t;
    f32 step;
    s16 i;

    if (arg0 < 2) {
        return NULL;
    }

    func_1512D748(&D_800DBFF0[D_800BE9E8], 0, 1);

    spB0.unk0 = 1;
    spB0.unk2 = (func_150ADA20() % 0xBU) + 0x1E;
    spB0.unk4 = 8;
    spB0.unk6 = -1;
    spB0.unk5 = 1;
    func_151D8868(&spB0, 0, 0xFF, 0);

    spB8.unk0 = arg1;
    spB8.unk4 = 0.0f;
    spB8.unk8 = arg0;
    spB8.unkA = 0;

    obj = func_151491F4(0x12C, -1, 0x10, 1, 0xC, (arg0 * 8) + 0x10, arg2, arg3);
    if (obj != NULL) {
        memcpy((void *)((s32)obj + 0x28), &spB8, 0xC);

        sp90[0] = -146.0f;
        sp90[1] = (func_150ADA68() * 160.0f) - 80.0f;
        sp90[2] = -50.0f;
        sp90[3] = (func_150ADA68() * 160.0f) - 80.0f;
        sp90[4] = 50.0f;
        sp90[5] = (func_150ADA68() * 160.0f) - 80.0f;
        sp90[6] = 146.0f;
        sp90[7] = (func_150ADA68() * 160.0f) - 80.0f;

        t = -1.0f;
        out = (f32 *)((s32)obj + 0x38);
        step = 3.0f / (f32)(arg0 - 1);

        for (i = 0; i < arg0; i++) {
            out[i * 2] = (func_15142A80(t) * sp90[0]) + (func_15142AC0(t) * sp90[2]) + (func_15142B04(t) * sp90[4]) + (func_15142B44(t) * sp90[6]);
            out[(i * 2) + 1] = (func_15142A80(t) * sp90[1]) + (func_15142AC0(t) * sp90[3]) + (func_15142B04(t) * sp90[5]) + (func_15142B44(t) * sp90[7]);
            t += step;
        }
    }
    return obj;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_113D60/func_150E7C9C.s")

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
} Struct150E7FEC;

void func_150E7FEC(f32 arg0, u8 arg1, s32 arg2, f32 *arg3, s16 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, u8 argA, s32 argB) {
    s32 pad;
    Struct150E7FEC sp5C;
    s32 sp58;
    s32 sp54;

    sp5C.unk22 = 0x14;
    sp5C.unk24 = 0xC;
    sp5C.unk06 = 0x4F;
    sp5C.unk07 = 0;
    sp5C.unk00 = ((arg4 == -1) ? 0 : 1) | (arg5 ? 0x400 : 0) | 0x100 | 0x200 | 0x1000 | 0x8000 | 0x40000;
    sp5C.unk04 = (arg4 == -1) ? 0x12C : arg4 + 0x14;
    sp5C.unk08 = 0;
    sp5C.unk0C = 0;
    sp5C.unk10 = arg1;
    sp5C.unk11 = 0xFF;
    sp5C.unk12 = arg7;
    sp5C.unk13 = arg8;
    sp5C.unk14 = arg9;
    sp5C.unk15 = 0xFF;
    sp5C.unk18 = ((arg6 != 0) ? 2 : 1) + 0x480000;
    sp5C.unk16 = 0;
    sp5C.unk17 = 6;
    if (arg5 != 0) {
        sp58 = 3;
        sp54 = 0xFF;
    } else {
        sp58 = 0;
        sp54 = 0;
    }
    func_1513C650((s32)&sp5C, 0, 0, arg2, arg3[0], arg3[1], arg3[2], arg0, arg0, func_150ADA20() & 0xFF, (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2), sp58, sp54, 0, argA, argB);
}

void func_150E81A8(u8 arg0, s32 arg1, s32 arg2) {
    Struct150E81A8Vec sp84;
    Struct150E81A8 sp28;
    Struct150E81A8Small sp20;

    if (arg0 == 4 || arg0 == 5 || arg0 == 6 || arg0 == 7) {
        sp84 = D_800A1290[arg0];
        *(f32 *)&sp84.unk4 += 200.0f;
        func_151D3FF4(&sp84, ((u8 *)&arg1)[3], arg2);

        sp28.unk00 = 0;
        sp28.unk02 = 0xFF;
        sp28.unk04 = -0x40;
        sp28.unk06 = 0x4D;
        sp28.unk08 = 0xA;
        sp28.unk0C = 5;
        sp28.unk10 = sp84;
        sp28.unk1C = 252.0f;
        sp28.unk20 = 117.0f;
        sp28.unk24 = 308.0f;
        sp28.unk28 = 256.0f;
        sp28.unk2C = D_800A1354;
        sp28.unk30 = D_800A1358;
        sp28.unk34 = 4;
        sp28.unk38 = 7;
        sp28.unk3C = 27.0f;
        sp28.unk40 = D_800A135C;
        sp28.unk44 = D_800A1360;
        sp28.unk48 = D_800A1364;
        sp28.unk4C = 25;
        sp28.unk4E = 15;
        sp28.unk50 = 100;
        sp28.unk52 = 100;
        sp28.unk54 = 12;
        sp28.unk56 = 20;
        sp28.unk58 = 0;

        func_1514FCE8(&sp28, ((u8 *)&arg1)[3], arg2);

        sp20.unk0 = 1;
        sp20.unk2 = (func_150ADA20() % 0xBU) + 0x1E;
        sp20.unk4 = 8;
        sp20.unk6 = -1;
        sp20.unk5 = 1;
        func_151D8868(&sp20, 0, 0xFF, 0);
    }
}

void func_150E83AC(struct127 *arg0, s16 arg1, u8 arg2, s32 arg3) {
    struct {
        Struct150E81A8Vec unk0;
        f32 unkC;
    } sp40;
    struct260 *temp_v0;

    sp40.unk0 = *(Struct150E81A8Vec *)arg0;
    sp40.unkC = 0.0f;

    temp_v0 = func_15149130((arg1 == -1) ? 0x12C : arg1, -1, 0x28, -1, (arg1 == -1) ? 0 : 1, 0, (struct37 *)0x10, arg2, arg3);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp40, 0x10);
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[9];
} Spawn150E8470;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
    f32 unk8;
} Extra150E8470;

typedef struct {
    struct17 pos;
    f32 accum;
} Sub150E8470;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Sub150E8470 unk28;
} Obj150E8470;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_150E8470(Obj150E8470 *arg0) {
    Sub150E8470 *sub;

    sub = &arg0->unk28;
    sub->accum += (0.09999999404f + func_150ADA68() * 0.08789999783f) * D_800BE9A4;
    if (sub->accum > 1.0f) {
        Spawn150E8470 spawn;
        Extra150E8470 extra;
        void *ret;

        spawn.unk1D = 0x6C;
        spawn.unk8 = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.unk1E = 0x73;
        spawn.unk20 = 2;
        spawn.unk58 = 0x80DE07;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        spawn.unk62 = 0x19;
        spawn.unk63 = -1;
        extra.unk0 = 0;
        extra.unk1 = 0;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk22 = 0x73;
        spawn.unk14 = 0x24;
        spawn.unk15 = 0x22;
        spawn.unk16 = 0x11;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0x7B;
        spawn.unk19 = 0x93;
        spawn.unk1A = 0xAA;
        spawn.unk1C = 0xFF;
        spawn.unk24 = 1.022418022f;
        spawn.unk30 = sub->pos;
        spawn.unk3C.unk0 = 0.0f;
        spawn.unk3C.unk4 = 0.0f;
        spawn.unk3C.unk8 = 0.0f;
        spawn.unk48.unk0 = 0.0f;
        spawn.unk48.unk4 = 0.0f;
        spawn.unk48.unk8 = 0.0f;
        do {
            extra.unk2 = func_150ADA20() % 5U + 4;
            extra.unk3 = func_150ADA20() % 5U + 4;
            extra.unk4 = func_150ADA68() * 13.0f;
            extra.unk8 = func_150ADA68() * 13.0f;
            spawn.unk54 = (func_150ADA68() * 40.0f + 68.0f) * 0.001000000047f;
            spawn.unk58 &= ~0xC0;
            spawn.unk58 |= ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0);
            spawn.unk1B = func_150ADA20() % 156U + 100;
            spawn.unkA = func_150ADA20() % 34U + 0x8D;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 71.0f + 80.0f;
            ret = func_15130280(&spawn, 1, 0, 0xC, arg0->unkC, arg0->unk1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &extra, sizeof(extra));
            }
            sub->accum -= 1.0f;
        } while (sub->accum > 1.0f);
    }
}

extern s32 func_15131828(s32, s32, s32, s32);

s32 func_150E8824(s32 arg0, s32 arg1) {
    func_15131828(arg0, arg0 + 0xAC, arg0 + 0xA8, arg0 + 0xAA);
    return 1;
}

void func_150E8854(void) {
    struct260 *temp_v0;
    f32 sp30;

    sp30 = 10.0f;

    temp_v0 = func_15149130(0x12C, -1, 0x35, -1, 0, 0, (struct37 *)0x4, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp30, 4);
    }
}

extern f32 D_800A1378;
extern void func_150E8930();

void func_150E88C0(void *arg0) {
    *(f32 *)((u8 *)arg0 + 0x28) -= D_800BE9A4;
    if (*(f32 *)((u8 *)arg0 + 0x28) < 0.0f) {
        *(f32 *)((u8 *)arg0 + 0x28) = (func_150ADA68() * D_800A1378) + 201.0f;
        func_150E8930(arg0);
    }
}

typedef struct Game115D70Pair {
    s32 first;
    s32 second;
} Game115D70Pair;

typedef struct Game115D70Spawn {
    s8 active;
    u8 pad1;
    s16 duration;
    s8 size;
    s8 enabled;
    s8 sentinel;
    u8 pad7;
} Game115D70Spawn;

extern Game115D70Pair D_80088A80;
extern u8 D_800BE9EB;
extern void func_15164F0C(u8, u8, void *, u8, s32);
void func_150E8A80(void);
void func_150E90DC(void);

void func_150E8930(void) {
    Game115D70Pair sp30;
    Game115D70Spawn sp28;
    s32 active;

    sp30 = D_80088A80;
    func_15169260(&sp30, 2, 0, 0x1B);
    func_15164F0C(0, D_800BE9EB, 0, 0xFF, 1);
    sp28.active = 1;
    sp28.duration = (func_150ADA20() % 21U) + 0x14;
    sp28.enabled = 1;
    sp28.size = (func_150ADA20() % 6U) + 3;
    sp28.sentinel = -1;
    func_151D8868(&sp28, 0, 0xFF, 1);
    active = D_800DCDC4;
    if (active != 0) {
        func_150E8A80();
        active = D_800DCDC4;
    }
    if (active != 0) {
        func_150E90DC();
    }
    func_10010F30(0x4C8, 0x7FFF, 0x40,
                   (s16)(0x200 - (func_150ADA20() & 0x400)), 0);
    func_10010F30(0x4CD, 0x5DC0, 0x40,
                   (s16)(0x200 - (func_150ADA20() & 0x400)), 0);
}

extern f32 D_800A137C;
extern f32 D_800A1380;

void func_150E8A80(void) {
    struct260 *temp_v0;
    struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
    } sp38;

    sp38.unk0 = D_800A137C;
    sp38.unk4 = D_800A1380;
    sp38.unk8 = 0.0f;

    temp_v0 = func_15149130((func_150ADA20() % 0x29U) + 0x1E, -1, 0x33, -1, 1, 0, (struct37 *)0xC, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp38, 0xC);
    }
}

extern f32 *func_15144B34(s32);

typedef struct Node150E8B1C {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    struct Node150E8B1C *unkC;
} Node150E8B1C;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} Struct150E8B1C;

void func_150E8B1C(Struct150E8B1C *arg0) {
    f32 *pos;
    f32 *p;
    Node150E8B1C *node;
    f32 r;
    struct {
        f32 unk00;
        f32 unk04;
        f32 unk08;
        f32 unk0C;
        f32 unk10;
        f32 unk14;
    } sp;
    f32 *v;
    f32 dx;
    f32 dy;
    f32 dz;

    v = (f32 *)&sp;
    pos = func_15144B34(D_800BE9E8);
    p = &arg0->unk28;
    p[2] += (p[0] + (func_150ADA68() * p[1])) * D_800BE9A4 * D_800DCD90;
    if (p[2] > 1.0f) {
        do {
            r = func_150ADA68() * D_800DCD90;
            node = (Node150E8B1C *)D_800DCDC4;
            while (node->unk8 < r) {
                r -= node->unk8;
                node = node->unkC;
            }
            func_1514470C((struct134 *)node->unk0, (s32)v);
            dx = sp.unk00 - pos[0];
            dy = sp.unk04 - pos[1];
            dz = sp.unk08 - pos[2];
            if (dx * dx + dy * dy + dz * dz < 3240000.0f) {
                struct260 *ret;

                sp.unk0C = 0.3171199858f;
                sp.unk10 = 1.046509981f;
                sp.unk14 = 0.0f;
                ret = func_15149130((func_150ADA20() % 13U) + 5, -1, 0x34, -1, 1, 0, (struct37 *)0x18, arg0->unkC, arg0->unk1);
                if (ret != NULL) {
                    memcpy((void *)((s32)ret + 0x28), v, 0x18);
                }
            }
            p[2] -= 1.0f;
        } while (p[2] > 1.0f);
    }
}

struct260 *func_15130374(void *, u8, s32, u8, s32);
void func_151436B4(f32, f32, f32, f32 *);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
} Struct150E8D5C;

void func_150E8D5C(Struct150E8D5C *arg0) {
    f32 *p;
    struct260 *ret;
    s32 flag;
    s32 flag2;
    struct {
        s32 unk00;
        s32 unk04;
        s16 unk08;
        s16 unk0A;
        s32 unk0C;
        s32 unk10;
        u8 unk14;
        u8 unk15;
        u8 unk16;
        u8 unk17;
        u8 unk18;
        u8 unk19;
        u8 unk1A;
        u8 unk1B;
        u8 unk1C;
        u8 unk1D;
        s16 unk1E;
        s16 unk20;
        s16 unk22;
        f32 unk24;
        f32 unk28;
        f32 unk2C;
        f32 unk30;
        f32 unk34;
        f32 unk38;
        f32 unk3C;
        f32 unk40;
        f32 unk44;
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
        s32 unk58;
        u8 pad5C[4];
        u8 unk60;
        u8 unk61;
        u8 unk62;
        s8 unk63;
    } sp94;
    s32 pad90;
    f32 sp8C;
    f32 zero;

    p = &arg0->unk28;
    p[5] += (p[3] + (func_150ADA68() * p[4])) * D_800BE9A4;
    if (p[5] > 1.0f) {
        zero = 0.0f;
        {
            f32 f;

            sp94.unk1D = 0x16;
            sp94.unk08 = 0xE01;
            sp94.unk00 = 0x200005;
            sp94.unk04 = 0;
            sp94.unk0C = 0;
            sp94.unk10 = 0;
            sp94.unk58 = 0x801E05;
            sp94.unk60 = 3;
            sp94.unk61 = 3;
            sp94.unk62 = 0x1A;
            sp94.unk63 = -1;
            sp94.unk3C = zero;
            sp94.unk40 = zero;
            sp94.unk44 = zero;
            sp94.unk54 = zero;
            sp94.unk17 = 0xFF;
            sp94.unk1C = 0xFF;
            sp94.unk1E = 0x1E;
            sp94.unk20 = 8;
            sp94.unk48 = zero;
            sp94.unk50 = zero;
            do {
                sp8C = 0.9700999856f;
                sp94.unk22 = sp94.unk0A = (func_150ADA20() & 0xF) + 0x28;
                sp94.unk1B = (func_150ADA20() % 119U) + 100;
                f = (func_150ADA68() * 59.0f) + 80.0f;
                sp94.unk2C = sp94.unk28 = f;
                sp94.unk24 = ((func_150ADA68() * 3727.0f) + 99979.0f) * 9.999999747e-06f;
                sp94.unk58 &= ~0xC0;
                if (func_150ADA20() & 1) {
                    flag = 0x80;
                } else {
                    flag = 0;
                }
                if (func_150ADA20() & 1) {
                    flag2 = 0x40;
                } else {
                    flag2 = 0;
                }
                sp94.unk58 |= (u32)(flag2 | flag);
                sp94.unk18 = 0x44;
                sp94.unk19 = 0x3C;
                sp94.unk1A = 0x27;
                sp94.unk14 = 0xF;
                sp94.unk15 = 0x11;
                sp94.unk16 = 5;
                func_151436B4(func_150ADA68() * 6.283185482f, func_150ADA68() * 6.283185482f, func_150ADA68() * 20.0f, &sp94.unk30);
                sp94.unk30 += p[0];
                sp94.unk34 += p[1];
                sp94.unk38 += p[2];
                sp94.unk4C = ((func_150ADA68() * 471.0f) + -1242.0f) * 0.009999999776f;
                ret = func_15130374(&sp94, 0, 4, arg0->unkC, arg0->unk1);
                if (ret != 0) {
                    memcpy((u8 *)ret + 0xA8, &sp8C, 4);
                }
                p[5] -= 1.0f;
            } while (p[5] > 1.0f);
        }
    }
}

extern f32 D_800A13B0;
extern f32 D_800A13B4;

void func_150E90DC(void) {
    struct260 *temp_v0;
    struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
    } sp38;

    sp38.unk0 = D_800A13B0;
    sp38.unk4 = D_800A13B4;
    sp38.unk8 = 0.0f;

    temp_v0 = func_15149130((func_150ADA20() % 0x1AU) + 0x5, -1, 0x36, -1, 1, 0, (struct37 *)0xC, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp38, 0xC);
    }
}

extern f32 *func_15144B34(s32);

typedef struct Node150E9178 {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    struct Node150E9178 *unkC;
} Node150E9178;

void func_150E9178(Struct150E8D5C *arg0) {
    f32 *pos;
    f32 *p;
    Node150E9178 *node;
    f32 r;
    struct {
        f32 unk00;
        f32 unk04;
        f32 unk08;
        f32 unk0C;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        u8 pad1C[0x14];
        s32 unk30;
        u8 unk34;
        u8 unk35;
        s32 unk38;
    } sp;
    f32 *v;
    f32 dx;
    f32 dy;
    f32 dz;

    v = (f32 *)&sp;
    pos = func_15144B34(D_800BE9E8);
    p = &arg0->unk28;
    p[2] += (p[0] + (func_150ADA68() * p[1])) * D_800BE9A4 * D_800DCD90;
    if (p[2] > 1.0f) {
        do {
            r = func_150ADA68() * D_800DCD90;
            node = (Node150E9178 *)D_800DCDC4;
            while (node->unk8 < r) {
                r -= node->unk8;
                node = node->unkC;
            }
            func_1514470C((struct134 *)node->unk0, (s32)v);
            dx = sp.unk00 - pos[0];
            dy = sp.unk04 - pos[1];
            dz = sp.unk08 - pos[2];
            if (dx * dx + dy * dy + dz * dz < 1960000.0f) {
                struct260 *ret;

                sp.unk0C = 0.1932400018f;
                sp.unk10 = 0.3140099943f;
                sp.unk14 = 0.0f;
                sp.unk18 = 99999.89844f;
                sp.unk30 = 0;
                sp.unk34 = 0;
                sp.unk35 = 0;
                sp.unk38 = 0;
                ret = func_15149130((func_150ADA20() % 13U) + 5, -1, 0x37, -1, 1, 0, (struct37 *)0x3C, arg0->unkC, arg0->unk1);
                if (ret != NULL) {
                    memcpy((void *)((s32)ret + 0x28), v, 0x3C);
                }
            }
            p[2] -= 1.0f;
        } while (p[2] > 1.0f);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_113D60/func_150E93DC.s")

s32 func_150E971C(f32 *arg0) {
    f32 *temp_v1;
    struct157 tmp[1];

    func_151337C0(arg0);
    temp_v1 = (f32 *)((u8 *)arg0 + 0x170);
    if (arg0[15] < temp_v1[0]) {
        if ((((s32 *)temp_v1)[1] & 0x1F) == 0xA) {
            f32 temp_f2;

            temp_f2 = (func_150ADA68() * 20.0f) + 20.0f;

            tmp->unk6 = 0xB;
            tmp->unk7 = 0;
            tmp->unk0 = 0x9701;
            tmp->unk4 = 0x64;
            tmp->unk8 = 0;
            tmp->unkC = 0;
            *(u8 *)&tmp->unk10 = 0xFF;
            *(u8 *)&tmp->unk11 = 0xFF;
            *(u8 *)&tmp->unk12 = 0xFF;
            *(u8 *)&tmp->unk13 = 0xFF;
            *(u8 *)&tmp->unk14 = 0xFF;
            *(u8 *)&tmp->unk15 = 0xFF;
            tmp->unk18 = 0x3B0003;
            tmp->unk16 = 0;
            tmp->unk17 = 7;
            tmp->unk22 = 0x14;
            tmp->unk24 = 0xC;

            func_1513C73C((s32)tmp, 0, 0, (s32)(temp_v1 + 2), arg0[14], temp_v1[0] + 5.0f, arg0[16], temp_f2, temp_f2, func_150ADA20() & 0xFF, 0, 0, *(u8 *)((u8 *)arg0 + 0xC), *(u8 *)((u8 *)arg0 + 1));
        }
        return 0;
    }
    return 1;
}
