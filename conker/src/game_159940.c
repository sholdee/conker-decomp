#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void guMtxIdentF(float mf[4][4]);
extern void guMtxF2L(float mf[4][4], Mtx *m);
extern void guMtxCatL(Mtx *m, Mtx *n, Mtx *res);
f32 func_150AD780(f32);
f32 func_150AD78C(f32);
extern u8 D_800BE9C0;
extern s32 D_800BE9E4;
extern struct10 *D_800DC2A0;
extern f32 D_800A36C0;
extern f32 D_800A36C4;
extern f32 D_800A36C8;
extern f32 D_800A36CC;
extern f32 D_800A36D0;
extern f32 D_800A36D4;
extern f32 D_800A36D8;
void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);

typedef struct Func1512D070Object {
    u8 pad0[0x2C];
    s32 unk2C;
    u8 pad30[0x23C - 0x30];
    u8 unk23C;
    u8 unk23D;
    u8 pad23E[0x298 - 0x23E];
    s16 unk298;
    u8 pad29A[0x5F0 - 0x29A];
    s32 unk5F0;
    u8 pad5F4[0x7B0 - 0x5F4];
    f32 unk7B0;
    f32 unk7B4;
    u8 pad7B8[0x7D8 - 0x7B8];
    f32 unk7D8;
    f32 unk7DC;
} Func1512D070Object;


typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_1512C490;

typedef struct {
    u8 pad0[0xC4];
    f32 unkC4;
    u8 padC8[0x102 - 0xC8];
    u8 unk102;
} Obj3D0_1512C490;

typedef struct {
    u8 pad0[0x120];
    u8 unk120;
    u8 pad121[0x130 - 0x121];
    Vec3_1512C490 unk130;
    Vec3_1512C490 unk13C;
    Vec3_1512C490 unk148;
    u8 pad154[0x197 - 0x154];
    u8 unk197;
} Obj3D4_1512C490;

typedef struct {
    u8 pad0[0x2C];
    s32 unk2C;
    u8 pad30[0x18C - 0x30];
    f32 unk18C;
    f32 unk190;
    f32 unk194;
    f32 unk198;
    u8 pad19C[0x23C - 0x19C];
    u8 unk23C;
    u8 unk23D;
    u8 unk23E;
    u8 pad23F[0x264 - 0x23F];
    f32 unk264;
    u8 pad268[0x298 - 0x268];
    s16 unk298;
    u8 pad29A[0x2A4 - 0x29A];
    Vec3_1512C490 unk2A4;
    u8 pad2B0[0x2BC - 0x2B0];
    Vec3_1512C490 unk2BC;
    u8 pad2C8[0x2E0 - 0x2C8];
    Vec3_1512C490 unk2E0;
    Vec3_1512C490 unk2EC;
    Vec3_1512C490 unk2F8;
    u8 pad304[0x388 - 0x304];
    f32 unk388;
    f32 unk38C;
    u8 pad390[0x398 - 0x390];
    f32 unk398;
    u8 pad39C[0x3A0 - 0x39C];
    f32 unk3A0;
    u8 pad3A4[0x3A8 - 0x3A4];
    f32 unk3A8;
    u8 pad3AC[0x3D0 - 0x3AC];
    Obj3D0_1512C490 *unk3D0;
    Obj3D4_1512C490 *unk3D4;
    u8 pad3D8[0x5D0 - 0x3D8];
    f32 unk5D0;
    u8 pad5D4[0x5E8 - 0x5D4];
    f32 unk5E8;
    f32 unk5EC;
    s32 unk5F0;
    u8 pad5F4[0x74C - 0x5F4];
    f32 unk74C;
    u8 pad750[0x754 - 0x750];
    f32 unk754;
    u8 pad758[0x7B4 - 0x758];
    f32 unk7B4;
    u8 pad7B8[0x7D0 - 0x7B8];
    f32 unk7D0;
    f32 unk7D4;
    u8 pad7D8[0x84D - 0x7D8];
    u8 unk84D;
    u8 unk84E;
    u8 pad84F;
    s32 unk850;
    f32 unk854;
    f32 unk858;
    f32 unk85C;
    f32 unk860;
    f32 unk864;
    f32 unk868;
} Obj1512C490;

typedef struct {
    u8 pad0[0x84];
    f32 unk84;
    u8 pad88[0x180 - 0x88];
} Struct180_1512C490;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} Entry1512C490;

typedef struct {
    u8 pad0[0x54];
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    u8 pad60[0x68 - 0x60];
} Entry800DC2C0;

extern s32 D_800DC280[];
extern Entry1512C490 D_800DCDE0[];
extern Entry800DC2C0 D_800DC2C0[];
typedef struct {
    f32 unk0;
    u8 pad4[0x64];
} Entry800DC320;
extern Entry800DC320 D_800DC320[];
extern LookAt D_800D9B90[];
void func_15047B80(Mtx *arg0, LookAt *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10);
void func_150A7DF0(Mtx *arg0, f32 arg1, f32 arg2, f32 arg3);
void func_150A7D00(Mtx *arg0, f32 arg1, f32 arg2, f32 arg3);
void func_1512D070(Func1512D070Object *arg0);
void func_1512D368(s32 arg0);
struct Game159940Object;
void func_1512D2F8(struct Game159940Object *arg0);

#define CUR_MTX(i) (&((Mtx *) ((s32 *) (&D_800DC2A0))[D_800BE9C0])[i])

void func_1512C490(Obj1512C490 *arg0) {
    Mtx sp158;
    Mtx sp118;
    s32 pad114;
    f32 sp110;
    f32 sp10C;
    f32 sp108;
    f32 sp104;
    s32 pad100;
    f32 spFC;
    f32 spF8;
    Vec3_1512C490 spEC;
    Vec3_1512C490 spE0;
    Vec3_1512C490 spD4;
    Vec3_1512C490 spC8;
    f32 spC4;
    Vec3_1512C490 spB8;
    f32 var_f0;
    f32 var_f2;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    f32 sp64[4][4];
    f32 sp60;

    spC4 = ((Struct180_1512C490 *)D_800BE628)[arg0->unk23D].unk84;
    spC4 = (1.0f - spC4) * 0.5f + spC4;
    if (arg0->unk3D4->unk120 != 0) {
        arg0->unk198 = 0.0f;
        arg0->unk190 = 0.0f;
        arg0->unk74C = 0.0f;
        arg0->unk754 = 0.0f;
    }
    if (arg0->unk23C != 0) {
        arg0->unk7D0 = 0.0f;
        arg0->unk7D4 = 0.0f;
        arg0->unk194 = arg0->unk198;
        arg0->unk18C = arg0->unk190;
    } else if ((arg0->unk2C == 0x100) || (arg0->unk23E != 0)) {
        func_150495B0(&arg0->unk194, arg0->unk198, &arg0->unk7D0, 4.0f, 6.0f, arg0->unk7B4);
        func_150495B0(&arg0->unk18C, arg0->unk190, &arg0->unk7D4, 4.0f, 6.0f, arg0->unk7B4);
    } else {
        func_150495B0(&arg0->unk194, arg0->unk198, &arg0->unk7D0, 0.75f, 1.10000002f, arg0->unk7B4);
        func_150495B0(&arg0->unk18C, arg0->unk190, &arg0->unk7D4, 0.75f, 1.10000002f, arg0->unk7B4);
    }
    arg0->unk2E0 = arg0->unk2BC;
    arg0->unk2EC = arg0->unk2F8;
    if ((arg0->unk2C & 0x40000) || (arg0->unk2C & 0x20)) {
        spEC = arg0->unk2F8;
        spE0 = arg0->unk2A4;
    } else if (arg0->unk3D4->unk197 != 0) {
        spEC = arg0->unk2F8;
        spE0 = arg0->unk2BC;
        func_1512523C((struct108 *)arg0);
        func_15125594((struct108 *)arg0);
    } else {
        sp110 = arg0->unk194;
        sp10C = arg0->unk18C;
        sp108 = arg0->unk74C;
        sp104 = arg0->unk754;
        spF8 = sinf(arg0->unk3A0 - 1.57079637f) * sp110;
        spFC = cosf(arg0->unk3A0 - 1.57079637f) * sp110;
        spEC.x = arg0->unk2F8.x + sp108 + spF8;
        spEC.y = arg0->unk2F8.y + sp10C;
        spEC.z = arg0->unk2F8.z + sp104 + spFC;
        spE0.x = arg0->unk2BC.x + sp108 + spF8;
        spE0.y = arg0->unk2BC.y + sp10C;
        spE0.z = arg0->unk2BC.z + sp104 + spFC;
        spD4 = arg0->unk2F8;
        spC8 = arg0->unk2BC;
        arg0->unk2F8 = spEC;
        arg0->unk2BC = spE0;
        func_1512523C((struct108 *)arg0);
        func_15125594((struct108 *)arg0);
    }
    func_150491EC((struct17 *)&spEC, (struct17 *)&spE0, (struct17 *)&spB8);
    arg0->unk3D4->unk148 = spE0;
    arg0->unk3D4->unk13C = spEC;
    arg0->unk3D4->unk130 = spB8;
    {
    f32 var_f12;

    func_15047B80(CUR_MTX(arg0->unk23D), &D_800D9B90[D_800BE9C0], spEC.x, spEC.y, spEC.z, spE0.x, spE0.y, spE0.z, 0.0f, 1.0f, 0.0f);
    if (arg0->unk2C & 0x80000) {
        func_1512D070((Func1512D070Object *)arg0);
        func_1512D368((s32)arg0);
        return;
    }
    if (arg0->unk2C == 0x40000) {
        func_150A7DF0(&sp158, 0.0f, 0.0f, arg0->unk5EC);
        guMtxCatL(CUR_MTX(arg0->unk23D), &sp158, CUR_MTX(arg0->unk23D));
        func_1512D070((Func1512D070Object *)arg0);
        return;
    }
    if (arg0->unk3D4->unk197 != 0) {
        if (arg0->unk84D == 2) {
            Entry1512C490 *tbl;

            tbl = (Entry1512C490 *)D_800DC280[arg0->unk850];
            arg0->unk854 += (arg0->unk84E + tbl)->unkC;
            arg0->unk858 += (arg0->unk84E + tbl)->unk10;
            arg0->unk85C += (arg0->unk84E + tbl)->unk14;
            arg0->unk860 += (arg0->unk84E + tbl)->unk0;
            arg0->unk864 += (arg0->unk84E + tbl)->unk4;
            arg0->unk868 += (arg0->unk84E + tbl)->unk8;
        } else {
            arg0->unk854 -= arg0->unk854 * 0.0799999982f;
            arg0->unk858 -= arg0->unk858 * 0.0799999982f;
            arg0->unk85C -= arg0->unk85C * 0.0799999982f;
            arg0->unk860 -= arg0->unk860 * 0.0799999982f;
            arg0->unk864 -= arg0->unk864 * 0.0799999982f;
            arg0->unk868 -= arg0->unk868 * 0.0799999982f;
        }
        func_1512D2F8((struct Game159940Object *)arg0);
    } else {
        arg0->unk854 -= arg0->unk854 * 0.0799999982f;
        arg0->unk858 -= arg0->unk858 * 0.0799999982f;
        arg0->unk85C -= arg0->unk85C * 0.0799999982f;
        arg0->unk860 -= arg0->unk860 * 0.0799999982f;
        arg0->unk864 -= arg0->unk864 * 0.0799999982f;
        arg0->unk868 -= arg0->unk868 * 0.0799999982f;
    }
    arg0->unk5EC += arg0->unk854 * spC4;
    func_150A7DF0(&sp158, -arg0->unk858 * spC4, arg0->unk85C * spC4, arg0->unk854 * spC4);
    guMtxCatL(CUR_MTX(arg0->unk23D), &sp158, CUR_MTX(arg0->unk23D));
    if (arg0->unk3D4 != NULL) {
        sp60 = func_15048FC8((struct17 *)&arg0->unk3D4->unk130) - 180.0f;
        func_150A8050(sp64,
                      -func_150484A0(arg0->unk3D4->unk130.y, sqrtf(arg0->unk3D4->unk130.x * arg0->unk3D4->unk130.x + arg0->unk3D4->unk130.z * arg0->unk3D4->unk130.z)) * 57.2957764f - arg0->unk858 * spC4,
                      sp60 - arg0->unk85C * spC4, 0.0f);
        guMtxXFMF(sp64, 0.0f, 0.0f, 1.0f, &spA4, &spA8, &spAC);
        arg0->unk3D4->unk130.x = spA4;
        arg0->unk3D4->unk130.y = spA8;
        arg0->unk3D4->unk130.z = spAC;
    }
    if (arg0->unk3D4->unk197 != 0) {
        func_150A7DF0(&sp158, D_800DCDE0[arg0->unk23D].unkC, D_800DCDE0[arg0->unk23D].unk10, D_800DCDE0[arg0->unk23D].unk14);
        func_150A7D00(&sp118, D_800DCDE0[arg0->unk23D].unk0, D_800DCDE0[arg0->unk23D].unk4, D_800DCDE0[arg0->unk23D].unk8);
        guMtxCatL(&sp158, &sp118, &sp158);
        guMtxCatL(CUR_MTX(arg0->unk23D), &sp158, CUR_MTX(arg0->unk23D));
        return;
    }
    if (((arg0->unk2C & 0x80) || (arg0->unk3D0->unk102 != 0)) && (arg0->unk298 != 0)) {
        var_f12 = arg0->unk3D0->unkC4 * 0.25f;
    } else if ((arg0->unk5F0 & 0x80) && (arg0->unk2C != 0x40)) {
        var_f12 = arg0->unk3D0->unkC4 * 0.5f;
    } else {
        var_f12 = 0.0f;
    }
    if (!(arg0->unk2C & 0x40000)) {
        if ((arg0->unk5F0 & 0x80) || (arg0->unk3D0->unk102 != 0)) {
            var_f0 = 2.0f;
            var_f2 = 4.0f;
        } else {
            var_f0 = 4.0f;
            var_f2 = 8.0f;
        }
        func_150495B0(&arg0->unk5EC, var_f12, &arg0->unk264, var_f0, var_f2, arg0->unk7B4);
    }
    func_1512D368((s32)arg0);
    if (arg0->unk2C != 0x8000) {
        if (arg0->unk2C == 1) {
            arg0->unk388 -= arg0->unk5D0;
            arg0->unk398 = arg0->unk388 * 0.0174532924f;
        }
        func_150A7DF0(&sp158, D_800DCDE0[arg0->unk23D].unkC + (arg0->unk5D0 + arg0->unk3A8 + arg0->unk38C),
                      D_800DCDE0[arg0->unk23D].unk10 + arg0->unk5E8,
                      (arg0->unk5EC + D_800DC320[arg0->unk23D].unk0 * 0.5f) + D_800DCDE0[arg0->unk23D].unk14);
        if (arg0->unk2C == 0x40000) {
            func_150A7DF0(&sp158, 0.0f, 0.0f, arg0->unk5EC);
        }
    }
    guMtxCatL(CUR_MTX(arg0->unk23D), &sp158, CUR_MTX(arg0->unk23D));
    func_1512D070((Func1512D070Object *)arg0);
    if (!(arg0->unk2C & 0x40000)) {
        func_150A7D00(&sp118, D_800DC2C0[arg0->unk23D].unk54 + D_800DCDE0[arg0->unk23D].unk0, D_800DC2C0[arg0->unk23D].unk58 + D_800DCDE0[arg0->unk23D].unk4, D_800DC2C0[arg0->unk23D].unk5C + D_800DCDE0[arg0->unk23D].unk8);
        guMtxCatL(CUR_MTX(arg0->unk23D), &sp118, CUR_MTX(arg0->unk23D));
    }
    }
}

void func_1512D070(Func1512D070Object *arg0) {
    s32 mtx[16];
    f32 mf[4][4];

    {
    f32 var_f2;

    if ((arg0->unk2C & 0x80000) != 0) {
        var_f2 = D_800A36C0;
    } else if (arg0->unk298 != 0) {
        var_f2 = D_800A36C4;
    } else {
        var_f2 = D_800A36C8;
    }

    if (((arg0->unk2C & 0x80000) != 0) || (arg0->unk5F0 & 1) || ((arg0->unk2C != 0x40000) && (arg0->unk5F0 & 8))) {
        arg0->unk7B0 += D_800A36CC * ((f32) D_800BE9E4);
        if (D_800A36D0 < arg0->unk7B0) {
            arg0->unk7B0 -= D_800A36D4;
        }
    } else {
        var_f2 = 0.0f;
    }

    if ((arg0->unk23C != 0) || (arg0->unk5F0 & 4)) {
        arg0->unk7DC = var_f2;
    } else {
        func_150495B0(&arg0->unk7DC, var_f2, &arg0->unk7D8, 0.200000003f, D_800A36D8, arg0->unk7B4);
    }

    guMtxIdentF(mf);
    mf[0][0] = ((func_150AD78C(arg0->unk7B0) * arg0->unk7DC) * 2.0f) + (arg0->unk7DC + arg0->unk7DC) + 1.0f;
    mf[1][1] = ((func_150AD780(arg0->unk7B0) * arg0->unk7DC) + arg0->unk7DC) + 1.0f;
    guMtxF2L(mf, (Mtx *)mtx);
    guMtxCatL(&((Mtx *) ((s32 *) (&D_800DC2A0))[D_800BE9C0])[arg0->unk23D],
              (Mtx *)mtx,
              &((Mtx *) ((s32 *) (&D_800DC2A0))[D_800BE9C0])[arg0->unk23D]);
    }
}

extern s32 D_800DC280[];
extern s32 D_800DC290[];
s32 func_1502B5C8(s32 *, s32, s32, s32);

typedef struct Game159940Object {
    char pad_0[0x84D];
    u8 field_0x84D;
    u8 field_0x84E;
    char pad_0x84F[0x1];
    s32 field_0x850;
} Game159940Object;

void func_1512D238(void) {
    u32 sp3C;
    s32 i;

    for (i = 0; i < 4; i++) {
        D_800DC280[i] = func_1502B5C8((s32 *)&sp3C, 2, 0x1B, i);
        D_800DC290[i] = sp3C / 0x18;
    }
}

typedef struct {
    u8 pad0[0x84D];
    /* 0x84D */ u8 unk84D;
    u8 pad84E[0x2];
    /* 0x850 */ s32 unk850;
} S1512D2E4;

void func_1512D2E4(S1512D2E4 *arg0, s32 arg1) {
    arg0->unk850 = arg1;
    arg0->unk84D = 0x1;
}

void func_1512D2F8(Game159940Object *arg0) {
    u8 temp;

    switch (arg0->field_0x84D) {
    case 1:
        arg0->field_0x84E = 0;
        arg0->field_0x84D = 2;
        break;
    case 2:
        temp = arg0->field_0x84E + D_800BE9E4;
        arg0->field_0x84E = temp;
        if ((s32)(temp & 0xFF) >= D_800DC290[arg0->field_0x850]) {
            arg0->field_0x84D = 0;
        }
        break;
    }
}

void func_1512D368(s32 arg0) {
    arg0 = arg0;
}
