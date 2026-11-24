#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

extern f32 D_800A1EB0;
extern f32 D_800A1EB4;
extern f32 D_800A1EB8;
extern f32 D_800A1EBC;
extern f32 D_800A1EC0;
extern f32 D_800A1EC4;
extern f32 D_800A1EC8;
extern f32 D_800A1ECC;
extern f32 D_800A1ED0;
extern struct260 *func_15164780(void *, s32, u8, s32);

typedef struct {
    u8 pad0[0x23D];
    u8 unk23D;
} Sub318_150FC438;

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x2DC];
    Sub318_150FC438 *unk318;
} Arg1_150FC438;

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
} Arg0_150FC438;

typedef struct {
    s32 unk0;
    u8  unk4;
    u8  pad5[3];
    f32 unk8;
    u8  unkC;
    u8  unkD;
    u8  padE[2];
    s32 unk10;
    s16 unk14;
    s8  unk16;
    u8  pad17;
    Arg1_150FC438 *unk18;
    u8  unk1C;
    u8  unk1D;
    u8  pad1E[2];
} Data_150FC438;

typedef union {
    s64 align;
    struct {
        u8  unk0;
        s8  unk1;
        s8  unk2;
        u8  pad3;
        s16 unk4;
        u8  unk6;
        u8  pad7;
        f32 unk8;
        f32 unkC;
        f32 unk10;
        f32 unk14;
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        f32 unk24;
        f32 unk28;
        f32 unk2C;
        f32 unk30;
        f32 unk34;
    } f;
} Child_150FC438;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x6F];
    u8 unk74;
    u8 pad75[0x15F];
    s32 unk1D4;
    u8 pad1D8[0x140];
    void *unk318;
    void *unk31C;
} O_150FB8C0;

typedef struct {
    u8 pad0[0x130];
    f32 unk130;
    f32 unk134;
    f32 unk138;
    f32 unk13C[3];
    u8 pad148[0x4F];
    u8 unk197;
} S31C_150FB8C0;

typedef struct {
    f32 x, y, z, w;
    u8 unkC8;
    s8 padC9;
    s16 unkCA;
    s16 unkCC;
    s16 unkCE;
    s16 unkD0;
    u8 unkD2;
    u8 unkD3;
    u8 unkD4;
    u8 unkD5;
    u8 unkD6;
    u8 unkD7;
    u8 unkD8;
    u8 unkD9;
    u8 unkDA;
    u8 unkDB;
    s32 unkDC;
    s32 unkE0;
    s32 unkE4;
    s32 unkE8;
    s32 unkEC;
    s32 unkF0;
    s32 unkF4;
    s8 unkF8;
    s8 unkF9;
    s8 padFA[2];
    u8 unkFC;
    u8 padFD[3];
    u8 pad100[0x10];
} Fx_150FB8C0;

typedef struct {
    u8 a;
    s8 b;
    s16 c;
    s8 d;
} Hd_150FB8C0;

typedef struct {
    u8 id;
    u8 pad[3];
    void *owner;
} Hd2_150FB8C0;

extern u8 D_800A1DD0[], D_800A1E34[];
extern f32 D_800A1DD4[][3], D_800A1DEC[][3], D_800A1E04[][3], D_800A1E1C[][3], D_800A1E38[][3], D_800A1E50[][3], D_800A1E68[][3], D_800A1E80[][3];
extern f32 D_800A1E98, D_800A1E9C, D_800A1EA0, D_800A1EA4, D_800A1EA8, D_800A1EAC;
extern O_150FB8C0 *func_1507515C(O_150FB8C0 *);
extern void func_15145EA4(void *, void *, s32, s32);
extern void func_151450B4(struct17 *, struct17 *, struct17 *);
extern s32 func_151C229C(f32 *, f32 *, f32 *, f32 *, s32, s32, f32, f32, f32, f32, f32, s32, O_150FB8C0 *, s32, s32, s32, s32, s32, s32, s32, s32, f32, s32, s32, s32, s32, s32);
extern s32 func_151602C0(void *, void *, s32, s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_15102B38(O_150FB8C0 *, u8, void *, void *, f32 *, s32, s32, f32, f32 *, s32, s32, s32, u8, s32);
extern void func_1515548C(void *, s32, s32, s32, s32, s32, s32);
extern void func_1515572C(void *, s32);

void func_150FB8C0(O_150FB8C0 *arg0, u8 arg1, f32 arg2, u8 arg3, u8 arg4, s32 arg5) {
    O_150FB8C0 *sp1C4;
    u8 sp1C3;
    f32 sp1B4[3];
    f32 sp1A8[3];
    f32 sp19C[3];
    f32 sp190[3];
    f32 sp184[3];
    f32 sp178[3];
    void *sp16C[3];
    void *sp160[3];
    s32 sp15C;
    f32 sp150[3];
    struct17 sp144;
    f32 t;
    f32 d0;
    f32 d1;
    f32 d2;
    f32 *sp130;
    u8 sp12F;
    s8 sp12E;
    Hd_150FB8C0 sp128;
    s32 sp11C[3];
    f32 sp114[2];
    u8 sp113;
    Fx_150FB8C0 spB8;
    Hd2_150FB8C0 spB0;

    if (arg0 == NULL || arg1 < 0 || arg1 >= 2) {
        return;
    }
    if (arg3 != 0) {
        sp1C4 = NULL;
        sp1C3 = 0;
    } else {
        sp1C4 = func_1507515C(arg0);
        if (sp1C4 == NULL) {
            return;
        }
        if (((S31C_150FB8C0 *)sp1C4->unk31C) == NULL) {
            return;
        }
        if (((S31C_150FB8C0 *)sp1C4->unk31C)->unk197 != 0) {
            sp1C3 = 1;
        } else {
            sp1C3 = 0;
        }
    }
    if (arg0->unk1D4 == 0) {
        return;
    }
    if (arg0->unk4 == 0x23) {
        sp16C[0] = D_800A1E38[arg1];
        if (sp1C3) {
            sp16C[1] = D_800A1E68[arg1];
        } else {
            sp16C[1] = D_800A1E50[arg1];
        }
        sp16C[2] = D_800A1E80[arg1];
        sp15C = (D_800A1E34[arg1] << 6) + arg0->unk1D4;
    } else {
        sp150[0] = (13.0f - (1.0f + func_150ADA68() * 6.0f)) * (arg1 ? -1 : 1);
        sp150[1] = D_800A1DEC[arg1][1];
        sp150[2] = D_800A1DEC[arg1][2];
        sp16C[0] = D_800A1DD4[arg1];
        sp16C[1] = sp1C3 ? D_800A1E04[arg1] : sp150;
        sp16C[2] = D_800A1E1C[arg1];
        sp15C = (D_800A1DD0[arg1] << 6) + arg0->unk1D4;
    }
    sp160[0] = sp1B4;
    if (sp1C3) {
        sp160[1] = sp184;
    } else {
        sp160[1] = sp1A8;
    }
    sp160[2] = sp178;
    func_15145EA4(sp16C, sp160, sp15C, sp1C3 ? 3 : 2);
    if (sp1C3) {
        extern void func_151450B4(struct17 *, struct17 *, struct17 *);

        sp130 = ((S31C_150FB8C0 *)sp1C4->unk31C)->unk13C;
        sp184[0] -= sp1B4[0];
        sp184[1] -= sp1B4[1];
        sp184[2] -= sp1B4[2];
        sp178[0] -= sp1B4[0];
        sp178[1] -= sp1B4[1];
        sp178[2] -= sp1B4[2];
                func_151450B4((struct17 *)sp184, (struct17 *)sp178, &sp144);
        d0 = sp1B4[0] - sp130[0];
        d1 = sp1B4[1] - sp130[1];
        d2 = sp1B4[2] - sp130[2];
        t = ((sp144.unk0 * d0) + (sp144.unk4 * d1) + (sp144.unk8 * d2)) /
            ((sp144.unk0 * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk130) + (sp144.unk4 * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk134) + (sp144.unk8 * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk138));
        sp19C[0] = sp130[0] + (t * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk130);
        sp19C[1] = sp130[1] + (t * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk134);
        sp19C[2] = sp130[2] + (t * ((S31C_150FB8C0 *)sp1C4->unk31C)->unk138);
        sp190[0] = ((S31C_150FB8C0 *)sp1C4->unk31C)->unk130;
        sp190[1] = ((S31C_150FB8C0 *)sp1C4->unk31C)->unk134;
        sp190[2] = ((S31C_150FB8C0 *)sp1C4->unk31C)->unk138;
    } else {
        sp1A8[0] -= sp1B4[0];
        sp1A8[1] -= sp1B4[1];
        sp1A8[2] -= sp1B4[2];
    }
    if (!sp1C3 || sp1C4->unk318 == NULL) {
        sp12F = 0xFF;
    } else {
        sp12F = ~(1 << ((u8 *)sp1C4->unk318)[0x23D]) & 0xFF;
    }
    if (D_800BE9F0 == 0x34 || D_800BE9F0 == 0x26) {
        sp12E = 5;
    } else {
        sp12E = 4;
    }
    func_151C229C(sp1B4, (sp1C3) ? NULL : sp1A8, (sp1C3) ? sp19C : NULL, (sp1C3) ? sp190 : NULL, (sp1C3) ? 1 : 0, 0,
                  252.0f, 0.003968254197f, (func_150ADA68() * 20.0f) + 75.0f, (func_150ADA68() * 250.0f) + 502.0f, 50.0f,
                  (func_150ADA20() % 31U) + 0xE1, (sp1C4 != NULL) ? sp1C4 : arg0, 1, 1, (D_800BE616 != 0) ? 0 : 1, 0x32, 0, 1, sp12E, 0x2C, arg2,
                  sp12F, 0, 0, arg4, arg5);
    if ((arg0->unk74 & 0xF) != 0xF) {
        if (!(func_150ADA20() & 7)) {
            sp128.a = 3;
            sp128.b = -1;
            sp128.c = (func_150ADA20() % 5U) + 4;
            sp128.d = 0;
            sp11C[0] = (s32)sp1B4[0];
            sp11C[1] = (s32)sp1B4[1];
            sp11C[2] = (s32)sp1B4[2];
            func_151602C0(&sp128, sp11C, (func_150ADA20() % 31U) + 0x46, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg4, arg5);
        }
        if (!(func_150ADA20() & 1)) {
            sp114[1] = (func_150ADA68() * 2.435000181f) + 3.534000158f;
            sp114[0] = (func_150ADA68() * 43.40000153f) + 48.40000153f;
            if (!sp1C3 || sp1C4->unk318 == NULL) {
                sp113 = 0xFF;
            } else {
                sp113 = ~(1 << ((u8 *)sp1C4->unk318)[0x23D]) & 0xFF;
            }
            func_15102B38(arg0, (arg0->unk4 == 0x23) ? D_800A1E34[arg1] : D_800A1DD0[arg1], (arg0->unk4 == 0x23) ? (void *)D_800A1E38[arg1] : (void *)D_800A1DD4[arg1],
                          (arg0->unk4 == 0x23) ? (void *)D_800A1E50[arg1] : (void *)D_800A1DEC[arg1], sp114,
                          (func_150ADA20() % 3U) + 4, (func_150ADA20() % 101U) + 0x9B, (func_150ADA68() * 1012.0f) + 1497.0f, sp1B4,
                          sp113, 0, -1, arg4, arg5);
        }
    }
    if (sp1C3 && sp1C4->unk318 != NULL) {
        if (arg1) {
            spB8.x = -59.0f;
        } else {
            spB8.x = 59.0f;
        }
        spB8.y = 68.0f;
        spB8.z = (func_150ADA68() * 20.0f) + 40.0f;
        spB8.w = (func_150ADA68() * 20.0f) + 80.0f;
        spB8.unkC8 = 0x80;
        spB8.unkCA = (func_150ADA20() & 3) + 5;
        spB8.unkCC = (s16)((arg1 ? 2 : 0) | 0x39) & 0xFFFF;
        spB8.unkCE = 3;
        spB8.unkD0 = 0x55;
        spB8.unkD2 = 0;
        spB8.unkD3 = 0xFF;
        spB8.unkD4 = 0xFF;
        spB8.unkD5 = 0xFF;
        spB8.unkD6 = (func_150ADA20() % 56U) + 0xC8;
        spB8.unkD7 = 0xFF;
        spB8.unkD8 = 0xFF;
        spB8.unkD9 = 0xFF;
        spB8.unkDA = 0xFF;
        spB8.unkDB = 0xFF;
        spB8.unkE0 = 0x200004;
        spB8.unkDC = 0;
        spB8.unkE4 = 0x1F0601;
        spB8.unkE8 = 3;
        spB8.unkEC = 0x22;
        spB8.unkF0 = 0x80;
        spB8.unkF4 = 0x20;
        spB8.unkF8 = 0;
        spB8.unkF9 = 7;
        spB8.unkFC = ((u8 *)sp1C4->unk318)[0x23D];
        func_1515548C(&spB8, 0, 0, 0, 0, 0, arg5);
        spB0.id = arg1;
        spB0.owner = sp1C4->unk318;
        func_1515572C(&spB0, 0x37);
    }
}

void func_151C3B0C(void *, f32, f32, f32, f32, s32, s32, s32);

void func_150FC368(void *arg0) {
    u8 var_v0;
    void *temp_v0;
    void *temp_v1;
    void *temp_v1_2;

    temp_v0 = *(void **)((u8 *)arg0 + 0x1A0);
    if ((temp_v0 == 0) || (*(s32 *)((u8 *)temp_v0 + 0) == 0) || (*(u8 *)((u8 *)temp_v0 + 4) == 0xFF) || (*(u8 *)((u8 *)arg0 + 0x1A4) != *(u8 *)((u8 *)temp_v0 + 0x3B)) || (temp_v1 = *(void **)((u8 *)temp_v0 + 0x31C), (temp_v1 == 0)) || (*(u8 *)((u8 *)temp_v1 + 0x84) != 0) || (*(u8 *)((u8 *)temp_v0 + 0x127) == 0xFF) || (temp_v1_2 = *(void **)((u8 *)temp_v0 + 0x318), (temp_v1_2 == 0))) {
        var_v0 = 0xFF;
    } else {
        var_v0 = ~(1U << *(u8 *)((u8 *)temp_v1_2 + 0x23D)) & 0xFF;
    }
    func_151C3B0C(arg0, 1.0f, 1.0f, 0.6f, 0.0f, 0xFF, 0xFF, var_v0);
}

void func_150FC438(Arg0_150FC438 *arg0, Arg1_150FC438 *arg1, u8 arg2, u8 arg3) {
    Data_150FC438 sp80;
    struct260 *temp_v0;
    s32 dest;
    Child_150FC438 sp40;

    sp80.unk0 = (s32)arg0;
    sp80.unk4 = arg0->unk3B;
    sp80.unk8 = 0.0f;
    sp80.unkC = 0;
    sp80.unkD = arg2;
    sp80.unk10 = 0;
    sp80.unk14 = 0;
    sp80.unk1D = arg3;
    if (arg1) {
        if (arg1->unk318) {
            sp80.unk16 = arg1->unk318->unk23D;
            goto block_150FC438_1;
        }
    }
    sp80.unk16 = -1;
block_150FC438_1:
    sp80.unk18 = arg1;
    if (arg1) {
        sp80.unk1C = arg1->unk3B;
    } else {
        sp80.unk1C = 0xFF;
    }

    temp_v0 = func_15149130(0x12C, -1, 0x27, -1, 0, 0x25, (struct37 *)0x20, 0xFF, 1);
    if (temp_v0 != 0) {
        dest = (s32)temp_v0 + 0x28;
        memcpy(dest, &sp80, sizeof(sp80));
        if (arg1) {
            if (arg1->unk318) {
                sp40.f.unk0 = 4;
                sp40.f.unk1 = -1;
                sp40.f.unk2 = -1;
                sp40.f.unk4 = 0x12C;
                sp40.f.unk6 = arg1->unk318->unk23D;
                sp40.f.unk8 = 12.0f;
                sp40.f.unkC = 20.0f;
                sp40.f.unk10 = 19.0f;
                sp40.f.unk14 = D_800A1EB0;
                sp40.f.unk18 = D_800A1EB4;
                sp40.f.unk1C = D_800A1EB8;
                sp40.f.unk20 = D_800A1EBC;
                sp40.f.unk24 = D_800A1EC0;
                sp40.f.unk28 = D_800A1EC4;
                sp40.f.unk2C = D_800A1EC8;
                sp40.f.unk30 = D_800A1ECC;
                sp40.f.unk34 = D_800A1ED0;
                *(struct260 **)(dest + 0x10) = func_15164780(&sp40, 0, 0xFF, 1);
            }
        }
    }
}

typedef struct {
    u8  pad0[0x1AA];
    s16 unk1AA;
} Sub31C_150FC614;

typedef struct {
    u8  pad0[0x31C];
    Sub31C_150FC614 *unk31C;
} Holder_150FC614;

typedef struct {
    s32 unk0;
    u8  unk4;
    u8  pad5[0x36];
    u8  unk3B;
    u8  pad3C[0x48];
    u16 unk84;
} Owner_150FC614;

typedef struct {
    Owner_150FC614 *unk0;
    u8  unk4;
    u8  pad5[3];
    f32 unk8;
    u8  unkC;
    u8  unkD;
    u8  padE[2];
    s32 unk10;
    s16 unk14;
    s8  unk16;
    u8  pad17;
    Holder_150FC614 *unk18;
    u8  unk1C;
    u8  unk1D;
    u8  pad1E[2];
} Data_150FC614;

typedef struct {
    u8  pad0;
    u8  unk1;
    u8  pad2[0xA];
    u8  unkC;
    u8  padD;
    s16 unkE;
    u8  pad10[0x18];
    Data_150FC614 unk28;
} Entity_150FC614;

typedef struct {
    u8  unk0;
    u8  unk1;
    s16 unk2;
    u8  unk4;
    u8  unk5;
    s8  unk6;
    u8  unk7;
} Spark_150FC614;

extern void func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void func_150FC614(Entity_150FC614 *arg0) {
    Data_150FC614 *d;
    Owner_150FC614 *owner;
    Spark_150FC614 sp50;

    d = &arg0->unk28;
    owner = d->unk0;
    if ((owner->unk0 == 0) || (owner->unk4 == 0xFF) || (owner->unk3B != d->unk4) || (D_800C35EA == 1)) {
        arg0->unkE = -1;
        return;
    }
    if (owner->unk84 != d->unk1D) {
        arg0->unkE = -1;
        return;
    }

    d->unk8 -= D_800BE9A4;
    while (d->unk8 < 0.0f) {
        func_150FB8C0((O_150FB8C0 *)owner, d->unkC, -d->unk8, d->unkD, arg0->unkC, arg0->unk1);
        d->unkC ^= 1;
        if (d->unk18 != NULL) {
            if (d->unk18->unk31C != NULL) {
                d->unk18->unk31C->unk1AA++;
            }
        }
        d->unk8 += 4.0f + (func_150ADA68() * 4.0f);
    }

    if (d->unk16 == -1) {
        return;
    }
    d->unk14 -= D_800BE9E4;
    if (d->unk14 < 0) {
        sp50.unk0 = 1;
        func_150ADA20();
        sp50.unk2 = 0x1E;
        sp50.unk5 = 1 << d->unk16;
        sp50.unk4 = (func_150ADA20() % 6U) + 3;
        sp50.unk6 = -1;
        func_151D8868(&sp50, 0, arg0->unkC, arg0->unk1);
        func_150ADA20();
        d->unk14 = 250;
    }
}

typedef struct {
    s32 unk0;
    u8  unk4;
    u8  pad5[0x13];
    s32 unk18;
    u8  unk1C;
} SubA_150FC818;

typedef struct {
    s32 unk0;
    union { s32 w4; u8 b4; } u4;
    u8  unk8;
    u8  unk9;
} ArgB_150FC818;

void func_150FC818(void *arg0, s32 arg1, u8 arg2) {
    ArgB_150FC818 *b = (ArgB_150FC818 *)arg1;
    SubA_150FC818 *a = (SubA_150FC818 *)((u8 *)arg0 + 0x28);
    s32 bu0;
    s32 bu0b;
    s32 au0;

    if (arg2 == 0) {
        bu0 = b->unk0;
        au0 = a->unk0;
        if ((au0 == bu0) || (a->unk4 == b->u4.b4)) {
            func_1516972C((struct102 *)arg0);
        } else if (a->unk18 != 0) {
            if ((bu0 == a->unk18) || (a->unk1C == b->u4.b4)) {
                func_1516972C((struct102 *)arg0);
            }
        }
    } else if (arg2 == 0x2D) {
        bu0b = b->unk0;
        if (bu0b == a->unk0) {
            a->unk0 = b->u4.w4;
            a->unk4 = b->unk9;
        } else if (a->unk0 == b->u4.w4) {
            a->unk0 = bu0b;
            a->unk4 = b->unk8;
        } else if (a->unk18 != 0) {
            if (bu0b == a->unk18) {
                a->unk18 = b->u4.w4;
                a->unk1C = b->unk9;
            } else if (a->unk18 == b->u4.w4) {
                a->unk18 = bu0b;
                a->unk1C = b->unk8;
            }
        }
    }
}
s32 func_150FC930(struct260 *arg0, s32 arg1, s32 arg2);
void func_150FC974(struct260 *arg0);

extern void func_150FC974(struct260 *);
void func_150FC9A4(struct260 *arg0);

extern void func_15149368(struct260 *);
void func_150FC9D0(struct260 *arg0);
