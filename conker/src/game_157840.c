#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} struct1512ABF8;

extern struct1512ABF8 D_80089590[];
extern u8 D_800DC0C0[];
extern struct1512ABF8 D_800DC200[][4];
extern s16 D_800DC204[];
extern s16 D_800DC206[];
extern f32 D_800A3694;
extern f32 D_800A3698;

void func_15123A54(struct108 *arg0);
void func_1512AD54(struct108 *arg0);
void func_1512C150(struct108 *arg0);

typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ f32 unk18;
    /* 0x01C */ u8 pad1C[0xC];
    /* 0x028 */ f32 unk28;
    /* 0x02C */ u8 pad2C[0x22];
    /* 0x04E */ u8 unk4E;
    /* 0x04F */ u8 pad4F[0x32];
    /* 0x081 */ u8 unk81;
    /* 0x082 */ u8 pad82[0x2B];
    /* 0x0AD */ u8 unkAD;
    /* 0x0AE */ u8 padAE[0x54];
    /* 0x102 */ u8 unk102;
    /* 0x103 */ u8 pad103[0x11];
    /* 0x114 */ s16 unk114;
    /* 0x116 */ u8 pad116[0x2];
    /* 0x118 */ f32 unk118;
    /* 0x11C */ u8 pad11C[0x1B];
    /* 0x137 */ u8 unk137;
    /* 0x138 */ u8 pad138[0x44];
    /* 0x17C */ f32 unk17C;
    /* 0x180 */ f32 unk180;
} Obj1512A390Sub;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0x54];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ u8 pad88[0x1B4];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 pad23D[0x6B];
    /* 0x2A8 */ f32 unk2A8;
    /* 0x2AC */ u8 pad2AC[0x8];
    /* 0x2B4 */ f32 unk2B4;
    /* 0x2B8 */ u8 pad2B8[0x40];
    /* 0x2F8 */ f32 unk2F8;
    /* 0x2FC */ f32 unk2FC;
    /* 0x300 */ f32 unk300;
    /* 0x304 */ f32 unk304;
    /* 0x308 */ f32 unk308;
    /* 0x30C */ u8 pad30C[0x3C];
    /* 0x348 */ f32 unk348;
    /* 0x34C */ f32 unk34C;
    /* 0x350 */ u8 pad350[0x4];
    /* 0x354 */ f32 unk354;
    /* 0x358 */ f32 unk358;
    /* 0x35C */ f32 unk35C;
    /* 0x360 */ f32 unk360;
    /* 0x364 */ f32 unk364;
    /* 0x368 */ u8 pad368[0x68];
    /* 0x3D0 */ Obj1512A390Sub *unk3D0;
    /* 0x3D4 */ Obj1512A390Sub *unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x218];
    /* 0x5F0 */ s32 unk5F0;
    /* 0x5F4 */ u8 pad5F4[0x4C];
    /* 0x640 */ s32 unk640;
    /* 0x644 */ s32 unk644;
    /* 0x648 */ s32 unk648;
    /* 0x64C */ u8 pad64C[0x28];
    /* 0x674 */ f32 unk674;
    /* 0x678 */ u8 pad678[0x50];
    /* 0x6C8 */ s32 unk6C8;
    /* 0x6CC */ u8 pad6CC[0x64];
    /* 0x730 */ s16 unk730;
    /* 0x732 */ u8 pad732[0x82];
    /* 0x7B4 */ f32 unk7B4;
    /* 0x7B8 */ u8 pad7B8[0x3C];
    /* 0x7F4 */ u16 unk7F4;
    /* 0x7F6 */ u8 pad7F6[0x52];
    /* 0x848 */ u8 unk848;
    /* 0x849 */ u8 pad849[0x7B];
    /* 0x8C4 */ f32 unk8C4;
    /* 0x8C8 */ f32 unk8C8;
    /* 0x8CC */ f32 unk8CC;
    /* 0x8D0 */ s32 unk8D0;
    /* 0x8D4 */ u8 pad8D4[0x54];
    /* 0x928 */ f32 unk928;
    /* 0x92C */ u8 unk92C;
} Obj1512A390;

void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
void func_1507C3E0(Obj1512A390Sub *arg0, s16 *arg1, s16 *arg2, s16 *arg3);
s32 func_150A3FC4(f32, f32, s32, void *, void *);
void func_1510E8BC(void *a0, void *a1, void *a2, void *a3, void *arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, s32 arg10, s32 arg11, f32 arg12, f32 arg13, s32 arg14);
void func_151236D0(void *);
s32 func_15125490(void *);

void func_1512A390(Obj1512A390 *arg0) {
    f32 sp7C;
    s32 sp78;
    s16 sp76;
    s16 sp74;
    s16 sp72;
    f32 sp6C;
    f32 pad68;
    f32 pad64;

    sp7C = arg0->unk3D0->unk28;
    if (arg0->unk3D4 != NULL) {
        sp78 = arg0->unk3D4->unk4E;
        sp6C = arg0->unk3D4->unk114;
    } else {
        sp78 = 0;
        func_1507C3E0(arg0->unk3D0, &sp76, &sp74, &sp72);
        sp6C = sp76;
    }
    if (!arg0->unk84 & 0x40) {
        arg0->unk35C = arg0->unk354 = arg0->unk3D0->unk180;
        return;
    }
    if ((arg0->unk23C != 0) || (arg0->unk84 & 0x400000) || (arg0->unk5F0 & 0x40) ||
        (func_150A3FC4(arg0->unk2F8, arg0->unk300, arg0->unk644, &arg0->unk648, &arg0->unk35C) == 0)) {
        func_1510E8BC(&arg0->unk644, &arg0->unk648, &arg0->unk35C, &arg0->unk360, &arg0->unk640, 0,
                      arg0->unk2F8, arg0->unk35C, arg0->unk300, arg0->unk308, 0, 0, -10000.0f, arg0->unk2FC, 1);
    }
    if (sp7C) {}
    if (D_800BE9F0 == 0x28) {
        arg0->unk364 = -10000.0f;
        arg0->unk360 = -10000.0f;
    }
    if (-10000.0f != arg0->unk364) {
        arg0->unk360 = arg0->unk364;
    }
    func_151236D0(arg0);
    if (!(arg0->unk84 & 0x10) && (arg0->unk3D0->unk102 == 0) && (D_800C3671 == 0) && (arg0->unk92C == 0) &&
        (arg0->unk3D0->unk17C < arg0->unk35C)) {
        func_15128774((struct108 *)arg0, (struct127 *)arg0->unk3D0);
    }
    if (-10000.0f == arg0->unk35C) {
        if (D_800BE616 != 0) {
            arg0->unk35C = arg0->unk3D0->unk180;
        } else {
            arg0->unk35C = 10000.0f;
            arg0->unk84 |= 0x800000;
            func_1510E8BC(&arg0->unk644, &arg0->unk648, &arg0->unk35C, &arg0->unk360, &arg0->unk640, 0,
                          arg0->unk2F8, arg0->unk2FC, arg0->unk300, arg0->unk308, 0, 0, -10000.0f, arg0->unk2FC, 1);
        }
    } else {
        arg0->unk84 &= ~0x800000;
    }
    if (-10000.0f != arg0->unk364) {
        arg0->unk360 = arg0->unk364;
    }
    if (arg0->unk3D0->unk137 != 0) {
        arg0->unk35C = arg0->unk354 = arg0->unk3D0->unk18;
    }
    if ((D_800C3671 != 0) || (arg0->unk7F4 != 0)) {
        arg0->unk35C = arg0->unk354 = arg0->unk3D0->unk180;
        return;
    }
    if (((arg0->unk2C & 0x100) || (arg0->unk2C != 0x80)) && (arg0->unk35C < arg0->unk360) &&
        (arg0->unk360 < arg0->unk2FC)) {
        arg0->unk35C = arg0->unk360;
        arg0->unk354 = arg0->unk360;
    } else {
        arg0->unk354 = arg0->unk35C;
    }
    if (arg0->unk92C != 0) {
        if (((-10000.0f == arg0->unk354) && (arg0->unk354 < arg0->unk3D0->unk180)) ||
            (arg0->unk928 < fabsf(arg0->unk354 - arg0->unk3D0->unk180))) {
            arg0->unk354 = arg0->unk3D0->unk180;
        }
    } else if (D_800C3671 == 0) {
        if (!(arg0->unk84 & 0x20000000)) {
            if (arg0->unk84 & 0x10000000) {
                if (arg0->unk354 < arg0->unk3D0->unk180) {
                    arg0->unk35C = arg0->unk354 = arg0->unk3D0->unk180;
                }
            }
            if ((arg0->unk2C != 0x80) || ((arg0->unk5F0 & 0x80) && (arg0->unk3D0->unk81 != 0)) ||
                (arg0->unk84 & 0x800)) {
                if (arg0->unk354 < arg0->unk2A8) {
                    arg0->unk354 = arg0->unk2A8;
                }
            }
        }
    }
    if (!(arg0->unk84 & 0x10000000) && (arg0->unk2C != 0x80)) {
        if ((arg0->unk5F0 & 1) && (sp78 != 0)) {
            arg0->unk35C = arg0->unk3D0->unk118;
        }
        if (arg0->unk2C == 0x80) {
            if (arg0->unk2FC < arg0->unk354 + 30.0f) {
                arg0->unk2FC = arg0->unk354 + 30.0f;
            }
            func_1512523C((struct108 *)arg0);
            return;
        }
        if ((arg0->unk6C8 != 0) && (arg0->unk730 != 0)) {
            arg0->unk348 = arg0->unk730;
        } else {
            arg0->unk348 = arg0->unk34C;
        }
    } else {
        arg0->unk354 = arg0->unk35C;
    }
    arg0->unk354 = arg0->unk35C + (arg0->unk354 - arg0->unk35C);
    if (!(arg0->unk84 & 0x4000) || (func_15125490(arg0) == 0)) {
        if ((arg0->unk3D0->unkAD == 1) && (arg0->unk2C != 0x80) && (-10000.0f != arg0->unk360)) {
            arg0->unk354 = arg0->unk360 + 50.0f;
        }
    }
    if ((arg0->unk5F0 & 0x40) && (arg0->unk3D0->unk137 == 0)) {
        if (arg0->unk2A8 - arg0->unk2B4 < -5.0f) {
            arg0->unk8D0 = 2;
            arg0->unk8C4 += 10.0f;
        } else if (arg0->unk2A8 - arg0->unk2B4 > 5.0f) {
            arg0->unk8D0 = 1;
            arg0->unk8C4 -= 10.0f;
        } else {
            arg0->unk8D0 = 0;
            arg0->unk8C4 = 0.0f;
        }
        arg0->unk8C4 = (arg0->unk8C4 < -300.0f) ? -300.0f : ((arg0->unk8C4 > 300.0f) ? 300.0f : arg0->unk8C4);
        func_150495B0(&arg0->unk8CC, arg0->unk8C4, &arg0->unk8C8, 1.0f, 1.5f, arg0->unk7B4);
        arg0->unk354 = arg0->unk2A8 + arg0->unk8CC + (sp6C * 0.8500000238f);
        arg0->unk354 = (arg0->unk354 < arg0->unk3D0->unk180 + arg0->unk348) ? arg0->unk3D0->unk180 + arg0->unk348 : arg0->unk354;
        arg0->unk354 = (arg0->unk354 < arg0->unk35C + 50.0f) ? arg0->unk35C + 50.0f : arg0->unk354;
        arg0->unk354 = (arg0->unk354 < arg0->unk360 + 50.0f) ? arg0->unk360 + 50.0f : arg0->unk354;
        arg0->unk674 = 0.0f;
        arg0->unk84 |= 0x10000000;
        arg0->unk848 = 1;
    lbl1: ;
    } else if (arg0->unk848 != 0) {
        arg0->unk674 = 1.0f;
        arg0->unk84 &= ~0x10000000;
        arg0->unk8D0 = 0;
        arg0->unk848 = 0;
    }
    if ((arg0->unk2C != 0x100) && (arg0->unk3D0->unk137 == 0)) {
        if (arg0->unk2FC < arg0->unk35C + 40.0f) {
            arg0->unk2FC = arg0->unk35C + 40.0f;
        }
    }
}

void func_1512ABF8(void) {
    f32 temp_f2;
    f32 temp_f0;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_t1;
    s32 temp_t2;
    u8 i;
    u8 j;
    register struct1512ABF8 *src;
    struct1512ABF8 *temp_v0;

    temp_f2 = 0.75f;
    temp_f0 = 0.5f;
    src = D_80089590;
    temp_t2 = 0x1E;
    temp_t1 = 0x1B;

    for (temp_a1 = D_80082FA0, temp_a2 = D_800BE9F0, i = 0; i < 4; i++) {
        j = 0;
        do {
            temp_v0 = &D_800DC200[i][j];
            *temp_v0 = src[j];

            if ((temp_a1 > 0) && (temp_a1 < 4)) {
                temp_v0->unk0 = temp_v0->unk0 * temp_f0;
                temp_v0->unk2 = temp_v0->unk2 * temp_f0;
            }

            if ((temp_t1 == temp_a2) || (temp_t2 == temp_a2)) {
                temp_v0->unk0 = temp_v0->unk0 * temp_f2;
                temp_v0->unk2 = temp_v0->unk2 * temp_f2;
            }
            j++;
        } while (j < 4);
    }

    bzero(D_800DC0C0, 0x130);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_157840/func_1512AD54.s")

void func_1512B100(struct108 *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->unk2C;
    if ((temp_v0 & ~0x100) && (D_800BEAC0 == 0) && (D_800C3671 == 0)) {
        if (temp_v0 != 0x40) {
            temp_v0 = arg0->unk84;
            if ((temp_v0 & 8) && !(temp_v0 & 0x200) && (arg0->unk23C == 0)) {
                temp_v0 = arg0->unk5F0;
                if (!(temp_v0 & 0x80) && !(temp_v0 & 0x40)) {
                    func_1512AD54(arg0);
                    return;
                }
            }
        }
        *(s16 *)((u8 *)arg0 + 0x5F8) = 1;
        arg0->unk5FC = 2;
        arg0->unk5FE = 0x3C;
    }
}

typedef struct {
    s32 shift;
    s32 base;
} struct80089630;

typedef struct {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x180 - 0x34];
} struct800BE628;

struct Struct1512B5FC;
extern struct80089630 D_80089630[];
s32 func_1512B630(struct108 *arg0, struct Struct1512B5FC *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void func_1512B1B8(struct108 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp74[0x281];
    s32 start;
    s32 i;
    s32 lim;
    s16 n;
    u16 code;
    struct80089630 *tbl;
    struct1512ABF8 *p;
    s16 y;

    p = &D_800DC200[arg0->unk23D][arg0->unk1B4];
    n = p->unk0;
    y = p->unk2;
    *(f32 *)((u8 *)arg0 + 0x60C) += (n - *(f32 *)((u8 *)arg0 + 0x60C)) * 0.05000000075f;
    n = (s16)*(f32 *)((u8 *)arg0 + 0x60C) + 0.5f;
    *(f32 *)((u8 *)arg0 + 0x610) += (y - *(f32 *)((u8 *)arg0 + 0x610)) * 0.05000000075f;
    start = n / 4;
    lim = (n * 3) / 4;

    for (i = start; i < n; i++) {
        if ((*(struct800BE628 **)&D_800BE628)[arg0->unk23D].unk2C < (f32)(arg1 - i)) {
            {
            s16 pad;
            code = ((u16 *)arg0->unk8BC)[arg1 - i];
            }
            tbl = &D_80089630[code >> 13];
            sp74[n - i] = arg3 - ((u32)(tbl->base + (((code >> 2) & 0x7FF) << tbl->shift)) >> 3);
            if (func_1512B630(arg0, (struct Struct1512B5FC *)(D_800DC0C0 + arg0->unk23D * 0x4C), sp74[n - i], i, lim, 0)) {
                i = n;
            }
        }
    }
    for (i = start; i < n; i++) {
        if ((f32)(arg1 + i) < (*(struct800BE628 **)&D_800BE628)[arg0->unk23D].unk30) {
            {
            s16 pad;
            code = ((u16 *)arg0->unk8BC)[arg1 + i];
            }
            tbl = &D_80089630[code >> 13];
            (sp74 + i)[n] = arg3 - ((u32)(tbl->base + (((code >> 2) & 0x7FF) << tbl->shift)) >> 3);
            if (func_1512B630(arg0, (struct Struct1512B5FC *)(D_800DC0C0 + arg0->unk23D * 0x4C + 0xC), (sp74 + i)[n], i, lim, 0)) {
                i = n;
            }
        }
    }
}

/* struct108 view: the header types unk5FC as u16 but this function reads it with lh. */
typedef struct {
    u8 pad0[4];
    f32 unk4;
    u8 pad8[0x178];
} Rec1512B53C;

typedef struct {
    u8 pad0[0x23D];
    u8 unk23D;
    u8 pad23E[0x5F8 - 0x23E];
    s16 unk5F8;
    u8 pad5FA[2];
    s16 unk5FC;
    u8 pad5FE[0x60C - 0x5FE];
    f32 unk60C;
    u8 pad610[0x8BC - 0x610];
    u16 *unk8BC;
} Obj1512B53C;


s32 func_1512B53C(Obj1512B53C *arg0) {
    if (arg0->unk5FC != 0) {
        s32 i;

        arg0->unk5F8 = 1;
        for (i = 0; i < ((Rec1512B53C *)D_800BE628)[arg0->unk23D].unk4; i++) {
            arg0->unk8BC[i] = 0xFFFC;
        }
        arg0->unk60C = 0.0f;
        arg0->unk5FC--;
        return 1;
    }
    return 0;
}


struct Struct1512B5FC {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4;
    s32 unk8;
};

void func_1512B5FC(struct Struct1512B5FC *arg0, s32 arg1, s32 arg2) {
    arg0->unk2 = arg0->unk1;
    if (arg0->unk4 < arg1) {
        arg0->unk1 = 0;
    } else {
        arg0->unk1 = 1;
    }
    arg0->unk8 = arg2;
    arg0->unk4 = arg1;
}

s32 func_1512B630(struct108 *arg0, struct Struct1512B5FC *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 temp;

    if (arg5 == 0) {
        temp = *(s16 *)((u8 *)D_800DC204 + (arg0->unk23D * 0x20) + (arg0->unk1B4 * 8));
    } else {
        temp = *(s16 *)((u8 *)D_800DC206 + (arg0->unk23D * 0x20) + (arg0->unk1B4 * 8));
    }

    if (arg0->unk84 & 0x8000) {
        temp = (s16)(temp << 1);
    }

    if ((temp < arg2) && (arg3 < arg4)) {
        arg1->unk0 = 2;
        func_1512B5FC(arg1, arg3, arg2);
        return 1;
    }

    if ((temp < arg2) && (arg4 < arg3)) {
        arg1->unk0 = 1;
        func_1512B5FC(arg1, arg3, arg2);
        return 1;
    }

    arg1->unk1 = 0;
    arg1->unk0 = 0;
    arg1->unk8 = 0xFFFC;
    return 0;
}

typedef struct {
    u8 unk0;
    u8 pad1[3];
    s32 unk4;
    s32 unk8;
} Ent1512B730;

typedef struct {
    Ent1512B730 e[4];
    u8 unk30;
    u8 pad31[0x4C - 0x31];
} Rec1512B730;

typedef struct {
    u8 pad0[0xAD];
    u8 unkAD;
    u8 padAE[0x120 - 0xAE];
    u8 unk120;
} Sub1512B730;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0x1B4 - 0x30];
    /* 0x1B4 */ s16 unk1B4;
    /* 0x1B6 */ u8 pad1B6[0x23C - 0x1B6];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 unk23D;
    /* 0x23E */ u8 pad23E[0x26C - 0x23E];
    /* 0x26C */ f32 unk26C;
    /* 0x270 */ u8 pad270[0x348 - 0x270];
    /* 0x348 */ f32 unk348;
    /* 0x34C */ u8 pad34C[0x374 - 0x34C];
    /* 0x374 */ f32 unk374;
    /* 0x378 */ u8 pad378[0x3D0 - 0x378];
    /* 0x3D0 */ Sub1512B730 *unk3D0;
    /* 0x3D4 */ Sub1512B730 *unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x3F0 - 0x3D8];
    /* 0x3F0 */ s16 unk3F0;
    /* 0x3F2 */ u8 pad3F2[2];
    /* 0x3F4 */ f32 unk3F4;
    /* 0x3F8 */ f32 unk3F8;
    /* 0x3FC */ u8 pad3FC[0x5FE - 0x3FC];
    /* 0x5FE */ s16 unk5FE;
} Obj1512B730;

#define REC (&((Rec1512B730 *)D_800DC0C0)[arg0->unk23D])
s32 func_1512B730(Obj1512B730 *arg0, f32 *arg1, s32 arg2) {
    s32 ret;
    u8 a;
    u8 b;
    u8 c;
    u8 d;
    f32 f0;
    f32 f2;
    f32 f12;

    ret = 0;
    a = REC->e[0].unk0;
    b = REC->e[1].unk0;
    c = REC->e[2].unk0;
    d = REC->e[3].unk0;
    f0 = REC->e[0].unk4 + 1.0f;
    f2 = REC->e[1].unk4 + 1.0f;
    if (a == 2 && b != 2) {
        REC->unk30 = 0;
    } else if (a != 2 && b == 2) {
        REC->unk30 = 1;
    }
    if (c == 2 && d != 2) {
        ((Rec1512B730 *)D_800DC0C0)[arg0->unk23D].unk30 = 2;
    } else if (c != 2 && d == 2) {
        ((Rec1512B730 *)D_800DC0C0)[arg0->unk23D].unk30 = 3;
    }
    if (a == 2 || b == 2) {
        f12 = D_800DC200[arg0->unk23D][arg0->unk1B4].unk0 + 1;
        if (a == 2 && b == 2) {
            if (arg0->unk3D0->unkAD != 1 && arg0->unk3D4->unk120 == 0) {
                arg0->unk5FE -= D_800BE9E4;
                if (arg0->unk5FE < 0) {
                    arg0->unk5FE = 0;
                }
            }
            if (arg0->unk2C != 0x80) {
                arg0->unk3F0 = 30;
            }
                    if (REC->e[1].unk4 < REC->e[0].unk4) {
                *arg1 = f12 - f0;
            } else if (REC->e[0].unk4 < REC->e[1].unk4) {
                *arg1 = f2 - f12;
            } else if (REC->unk30 == 0) {
                *arg1 = f12 - f0;
            } else {
                *arg1 = f2 - f12;
            }
        } else {
            arg0->unk3F0 = 0;
            arg0->unk5FE = 60;
                    if (REC->e[0].unk0 == 2) {
                if (arg0->unk23C != 0) {
                    *arg1 = f12 - (REC->e[0].unk4 + 1.0f);
                } else {
                    *arg1 = f12 - (REC->e[0].unk4 + 1.0f);
                }
            } else {
                if (arg0->unk23C != 0) {
                    *arg1 = (REC->e[1].unk4 + 1.0f) - f12;
                } else {
                    *arg1 = (REC->e[1].unk4 + 1.0f) - f12;
                }
            }
        }
        {
        s32 pad;
        ret = 1;
        arg0->unk26C = 0.004999999888f;
        }
    } else {
        ((Rec1512B730 *)D_800DC0C0)[arg0->unk23D].e[0].unk0 = 4;
        ((Rec1512B730 *)D_800DC0C0)[arg0->unk23D].e[1].unk0 = 4;
        *arg1 -= *arg1 * 0.1000000015f;
        arg0->unk5FE = 60;
    }
    if (arg0->unk3F0 != 0) {
        arg0->unk3F0 -= D_800BE9E4;
        if (arg0->unk3F0 < 0) {
            arg0->unk3F0 = 0;
        }
        if (arg0->unk3F4 == -1.0f) {
            arg0->unk3F4 = arg0->unk374;
            arg0->unk3F8 = arg0->unk348;
            arg0->unk374 = 150.0f;
            arg0->unk348 = 275.0f;
        }
    } else if (arg0->unk3F4 != -1.0f) {
        arg0->unk374 = arg0->unk3F4;
        arg0->unk348 = arg0->unk3F8;
        arg0->unk3F4 = -1.0f;
        arg0->unk3F8 = -1.0f;
    }
    return ret;
}

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ f32 unk14;
    /* 0x018 */ u8 pad18[0x4];
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ u8 pad20[0x75];
    /* 0x095 */ u8 unk95;
    /* 0x096 */ u8 pad96[0x6C];
    /* 0x102 */ u8 unk102;
    /* 0x103 */ u8 pad103[0xB0];
    /* 0x1B3 */ u8 unk1B3;
} Obj1512BB10Sub;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0x54];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ u8 pad88[0x104];
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ u8 pad190[0xAC];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 pad23D;
    /* 0x23E */ u8 unk23E;
    /* 0x23F */ u8 pad23F[0x2D];
    /* 0x26C */ f32 unk26C;
    /* 0x270 */ u8 pad270[0x40];
    /* 0x2B0 */ f32 unk2B0;
    /* 0x2B4 */ u8 pad2B4[0x4];
    /* 0x2B8 */ f32 unk2B8;
    /* 0x2BC */ u8 pad2BC[0x3C];
    /* 0x2F8 */ f32 unk2F8;
    /* 0x2FC */ f32 unk2FC;
    /* 0x300 */ f32 unk300;
    /* 0x304 */ f32 unk304;
    /* 0x308 */ f32 unk308;
    /* 0x30C */ f32 unk30C;
    /* 0x310 */ u8 pad310[0x44];
    /* 0x354 */ f32 unk354;
    /* 0x358 */ u8 pad358[0x1C];
    /* 0x374 */ f32 unk374;
    /* 0x378 */ u8 pad378[0x4];
    /* 0x37C */ f32 unk37C;
    /* 0x380 */ u8 pad380[0x50];
    /* 0x3D0 */ Obj1512BB10Sub *unk3D0;
    /* 0x3D4 */ Obj1512BB10Sub *unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x10];
    /* 0x3E8 */ u8 unk3E8;
    /* 0x3E9 */ u8 pad3E9[0x207];
    /* 0x5F0 */ s32 unk5F0;
    /* 0x5F4 */ u8 pad5F4[0x50];
    /* 0x644 */ s32 unk644;
    /* 0x648 */ u8 pad648[0xF4];
    /* 0x73C */ s16 unk73C;
    /* 0x73E */ u8 pad73E[0x76];
    /* 0x7B4 */ f32 unk7B4;
    /* 0x7B8 */ u8 pad7B8[0x194];
    /* 0x94C */ f32 unk94C;
    /* 0x950 */ f32 unk950;
    /* 0x954 */ f32 unk954;
    /* 0x958 */ f32 unk958;
    /* 0x95C */ f32 unk95C;
    /* 0x960 */ f32 unk960;
} Obj1512BB10;

typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ u8 pad4[0x10];
    /* 0x014 */ f32 unk14;
    /* 0x018 */ f32 unk18;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ f32 unk20;
    /* 0x024 */ u8 pad24[0x4];
    /* 0x028 */ f32 unk28;
    /* 0x02C */ u8 pad2C[0x14];
    /* 0x040 */ f32 unk40;
    /* 0x044 */ u8 pad44[0x13C];
    /* 0x180 */ f32 unk180;
    /* 0x184 */ u8 pad184[0x4];
    /* 0x188 */ s32 unk188;
    /* 0x18C */ u8 pad18C[0x18C];
    /* 0x318 */ void *unk318;
    /* 0x31C */ u8 pad31C[0x10];
} Obj1512BB10Probe;

extern u8 D_80089120[];
extern u8 D_800CBDD2;
extern u8 D_800CBDD3;
extern u8 D_800CBDD4;
s32 func_15044380(f32 arg0, f32 arg1, f32 arg2, void *arg3, s32 arg4, s32 arg5);

void func_1512BB10(Obj1512BB10 *arg0) {
    s32 sp3A4;
    s32 sp3A0;
    Obj1512BB10Probe sp74;
    s32 ret;
    s32 sp6C;
    s32 sp68;
    s32 sp64;
    f32 sp60;
    Obj1512BB10Sub *sub;

    sp3A0 = 0;
    if (!(arg0->unk84 & 0x40000) && (sub = arg0->unk3D4, sub->unk95 == 0) && (arg0->unk2C != 0x40) &&
        !(arg0->unk84 & 0x80) && !(arg0->unk84 & 0x200)) {
        sp6C = D_80089120[2];
        sp68 = D_80089120[1];
        if (arg0->unk5F0 & 0x800) {
            arg0->unk94C = 20.0f;
            arg0->unk950 = 40.0f;
        } else if (arg0->unk5F0 & 0x10) {
            arg0->unk94C = 20.0f;
            arg0->unk950 = 72.0f;
        } else if (arg0->unk374 > 580.0f) {
            arg0->unk94C = 70.0f;
            arg0->unk950 = 140.0f;
        } else if ((D_800BE9F0 == 0x31) && (arg0->unk23E == 3)) {
            sp3A0 = 1;
            arg0->unk94C = 20.0f;
            arg0->unk950 = 50.0f;
        } else {
            if (((arg0->unk73C != 0) && (arg0->unk73C != 1) && (arg0->unk73C != 3)) ||
                ((arg0->unk3D0->unk102 != 0) &&
                 ((arg0->unk3D0->unk14 != arg0->unk2B0) || (arg0->unk3D0->unk1C != arg0->unk2B8))) ||
                (sub->unk1B3 != 0)) {
                arg0->unk94C = 20.0f;
                arg0->unk950 = 20.0f;
            } else if (arg0->unk3D0->unk102 != 0) {
                sp3A0 = 1;
                arg0->unk94C = 25.0f;
                arg0->unk950 = 30.0f;
            } else {
                arg0->unk94C = 30.0f;
                arg0->unk950 = 60.0f;
            }
        }
        if (arg0->unk23C != 0) {
            arg0->unk954 = 0.0f;
            arg0->unk95C = arg0->unk94C;
            arg0->unk958 = 0.0f;
            arg0->unk960 = arg0->unk950;
        } else {
            func_150495B0(&arg0->unk95C, arg0->unk94C, &arg0->unk954, 4.0f, 6.0f, arg0->unk7B4);
            func_150495B0(&arg0->unk960, arg0->unk950, &arg0->unk958, 4.0f, 6.0f, arg0->unk7B4);
        }
        sp74.unk0 = 0x2D;
        sp74.unk318 = arg0;
        sp74.unk14 = arg0->unk2F8;
        if (arg0->unk5F0 & 0x800) {
            sp74.unk18 = arg0->unk2FC + arg0->unk18C;
        } else {
            sp74.unk18 = arg0->unk2FC;
        }
        sp74.unk1C = arg0->unk300;
        sp74.unk188 = arg0->unk644;
        sp74.unk28 = sp74.unk18 - arg0->unk354;
        sp74.unk180 = arg0->unk354;
        sp74.unk20 = 0.0f;
        sp74.unk40 = arg0->unk37C;
        if ((arg0->unk84 & 0x80000000) || (D_800BE9F0 == 0x37)) {
            D_80089120[2] = 0;
        }
        if (arg0->unk84 & 0x10000) {
            D_80089120[1] = 0;
        }
        D_800CBDD2 = 1;
        sp3A4 = D_800CBDD3;
        if (arg0->unk3D0->unk102 != 0) {
            D_800CBDD3 = 1;
        } else {
            D_800CBDD3 = 0;
        }
        sp64 = D_800CBDD4;
        if ((arg0->unk23C != 0) || ((arg0->unk2C == 0x100) && (sp3A0 == 0))) {
            D_800CBDD4 = 1;
        } else {
            D_800CBDD4 = 0;
        }
        ret = func_15044380(arg0->unk304, arg0->unk308, arg0->unk30C, &sp74, 0, 0);
        D_800CBDD4 = sp64;
        D_800CBDD2 = 0;
        D_800CBDD3 = sp3A4;
        D_80089120[2] = sp6C;
        D_80089120[1] = sp68;
        if (ret != 0) {
            arg0->unk3E8 = 0;
            arg0->unk26C = 0.004999999888f;
        } else {
            arg0->unk3E8 = 1;
            arg0->unk26C = arg0->unk26C + (1.0f - arg0->unk26C) * 0.1000000015f;
        }
        arg0->unk2F8 = sp74.unk14;
        if (arg0->unk2C != 0x100) {
            if (arg0->unk5F0 & 0x800) {
                arg0->unk2FC = sp74.unk18 - arg0->unk18C;
            } else {
                arg0->unk2FC = sp74.unk18;
            }
        }
        arg0->unk300 = sp74.unk1C;
        if ((D_800BE9F0 != 0x1B) && (ret != 0) && (arg0->unk2C != 0x100)) {
            func_1510E8BC(NULL, NULL, &sp60, NULL, NULL, 0, arg0->unk2F8, arg0->unk2FC, arg0->unk300,
                          arg0->unk308, 0, 0, -10000.0f, arg0->unk2FC, 1);
            if (arg0->unk2FC < sp60 + 40.0f) {
                s32 pad;
                arg0->unk2FC = sp60 + 40.0f;
            }
        }
    } else {
        arg0->unk3E8 = 1;
    }
    func_151236D0(arg0);
}

void func_1512C068(struct108 *arg0) {
    f32 temp_f14;
    f32 temp_f2;
    f32 temp_f12;
    f32 temp_f0;

    if (arg0->unk390 < 180.0f) {
        temp_f14 = 2.0f;
    } else {
        temp_f14 = -2.0f;
    }

    if ((arg0->unk2C != 0x80) && (arg0->unk2C != 0x40)) {
        temp_f2 = arg0->unk2F8 - arg0->unk2BC;
        temp_f12 = arg0->unk300 - arg0->unk2C4;
        temp_f0 = sqrtf((temp_f2 * temp_f2) + (temp_f12 * temp_f12));
        *(f32 *)arg0->pad370 = temp_f0;

        if (temp_f0 < 42.0f) {
            arg0->unk384 = arg0->unk37C - (45.0f * temp_f14);
            arg0->unk5CC = 0.0f;
            func_15123A54(arg0);
            arg0->unk240 |= 2;
        }
    }

    func_1512C150(arg0);
}

void func_1512C150(struct108 *arg0) {
    f32 temp_f0;

    arg0->unk5CC += D_800A3694;
    temp_f0 = arg0->unk5CC;
    if (temp_f0 > 1.0f) {
        arg0->unk5CC = 1.0f;
    } else {
        arg0->unk5CC = temp_f0;
    }

    if (arg0->unk5CC == 1.0f) {
        arg0->unk384 = D_800A3698;
        arg0->unk240 &= ~2;
    } else {
        arg0->unk384 += func_15048A70(arg0->unk384, arg0->unk37C) * arg0->unk5CC;
    }
}

void func_1512C200(s32 arg0) {
}

typedef struct {
    u8 pad0[0x2A4];
    struct17 unk2A4;
    u8 pad2B0[0x48];
    struct17 unk2F8;
    u8 pad304[0x78];
    f32 unk37C;
    u8 pad380[0x5B0];
    f32 unk930;
    f32 unk934;
    f32 unk938;
    struct17 unk93C;
    u8 unk948;
} struct1512C20C;

extern f32 D_800895B0;
#define SQ(x) ((x) * (x))
void func_1508EF80(struct17 *arg0, struct17 *arg1, f32 arg2, struct17 *arg3);

void func_1512C20C(struct1512C20C *arg0) {
    f32 sp6C;
    struct17 sp60;
    struct17 sp54;
    struct17 sp48;

    if (arg0->unk948) {
        f32 temp_f0;
        temp_f0 = arg0->unk2F8.unk0 - arg0->unk93C.unk0;
        sp6C = arg0->unk2F8.unk8 - arg0->unk93C.unk8;
        sp60 = arg0->unk2F8;
        if (sqrtf(SQ(temp_f0) + SQ(sp6C)) < arg0->unk938) {
            struct17 sp38;
            f32 sp34;
            f32 sp30;
            sp38 = arg0->unk2A4;
            sp38.unk4 = 0.0f;
            sp60.unk4 = 0.0f;
            arg0->unk93C.unk4 = 0.0f;
            func_150491EC(&arg0->unk93C, &sp60, &sp54);
            sp34 = func_15048FC8(&sp54) - 180.0f;
            func_15048758(&sp34);
            func_150491EC(&sp38, &sp60, &sp48);
            func_15049148(&sp54, arg0->unk938, &sp54);
            sp60.unk0 = arg0->unk93C.unk0 + sp54.unk0;
            sp60.unk4 = 0.0f;
            sp60.unk8 = arg0->unk93C.unk8 + sp54.unk8;
            sp6C = sqrtf(SQ(sp38.unk0 - sp60.unk0) + SQ(sp38.unk8 - sp60.unk8));
            func_15048F90(&sp60, &sp38, &sp54);
            sp30 = func_15048FC8(&sp54);
            func_15048758(&sp30);
            arg0->unk930 = sp30;
            temp_f0 = func_15048A70(sp34, sp30);
            if (fabsf(temp_f0) < 35.0f) {
                if (temp_f0 <= 0.0f) {
                    temp_f0 = 5.0f;
                } else {
                    temp_f0 = -5.0f;
                }
                func_1508EF80(&sp60, &arg0->unk93C, temp_f0, &sp60);
            }
            D_800895B0 = sp6C;
            sp60.unk4 = arg0->unk2F8.unk4;
            arg0->unk2F8 = sp60;
        } else {
            D_800895B0 = 0.0f;
            arg0->unk930 = arg0->unk37C;
        }
    } else {
        D_800895B0 = 0.0f;
        arg0->unk930 = 0.0f;
    }
    arg0->unk948 = 0;
}

s32 func_1512C47C(s32 arg0) {
    return 0x1;
}
