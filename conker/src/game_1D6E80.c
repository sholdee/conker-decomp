#include <ultra64.h>
#define func_10010F30 func_10010F30_void
#define func_150ADA20 func_150ADA20_orig
#include "functions.h"
#undef func_10010F30
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

extern s32 func_10010F30(s32, u16, u8, s16, u8);
extern s32 func_151EF610(void);
extern f32 D_800A8F74;
extern f32 D_800A8F78;
extern f32 D_800A8F7C;
extern f32 D_800A8F84;
extern f32 D_800A8F88;
extern f32 D_800A8F8C;
extern f32 D_800A8F90;
extern f32 D_800A8FEC;
extern f32 D_800A9014;
extern s32 func_15045800(f32 *, s32, f32, s32);
extern s32 func_151ABE40(f32 *, s32, s32, s32, s32);
extern s32 func_10010FFC(s32, s32, s32, s32, s32, s32);
extern void func_151AA264(void *, s32);
extern void func_151ABE00(void *);
extern void *func_15190770(void *, s32, s32, s32);
extern void func_151346EC(u8 *);
extern void func_1513470C(u8 *);
extern u8 func_151D8E20(void);
extern f32 D_800A8FF0;
extern f32 D_800A8F94;
extern f32 D_800A8F98;
extern f32 D_800A8F9C;
extern f32 D_800A8FA0;
extern s32 func_1515A920(void *, s32 *);
extern void func_1515A238(f32 *, f32 *, f32, f32, s32, f32, s32, s32, s32, s32, s32);

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0xDC];
    f32 unk118;
} struct_151AB2C4_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 pad9[0x3];
    void *unkC;
    u8 unk10;
    u8 pad11[0x3];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    u8 unk2A;
    u8 unk2B;
    s8 unk2C;
    u8 unk2D;
} struct_151AB2C4_desc;

typedef struct {
    s32 unk0;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    f32 unk8;
} struct_151AB2C4_payload;

extern void *func_1513418C(struct_151AB2C4_desc *, s32, u8, s32);

typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0xF8];
    f32 unk118;
} struct_151A9EC0_inner;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0xB];
    struct_151A9EC0_inner *unk18;
    u8 pad1C[0x18];
    f32 unk34;
} struct_151A9EC0_arg;

void func_151A99D0(u8 *arg0) {
    u8 *v1 = *((u8 **)(arg0 + 0x18));
    f32 sp30[3];

    goto dummy_label_151A99D0; dummy_label_151A99D0: ;
    sp30[0] = *((f32 *)(v1 + 0x14));
    if (D_800A8F74 < (*((f32 *)(v1 + 0x118)))) {
        sp30[1] = (*((f32 *)(v1 + 0x118))) + 100.0f;
    } else {
        sp30[1] = (*((f32 *)(v1 + 0x18))) + 150.0f;
    }
    sp30[2] = *((f32 *)(v1 + 0x1C));
    ((s32 *)sp30)[-1] = (s32)(arg0 + 0x34);
    if (func_15045800(sp30, 0, sp30[1] - 300.0f, (s32)(arg0 + 0x34)) != 0) {
        sp30[1] = *((f32 *)(arg0 + 0x34));
        func_151ABE40(sp30, ((s32 *)sp30)[-1], 3, arg0[0xC], arg0[0x1]);
    }
}

void func_151A9AA4(u8 *arg0) {
    u8 *v1 = *((u8 **)(arg0 + 0x18));
    f32 sp30[3];
    s32 temp;

    goto dummy_label_151A9AA4; dummy_label_151A9AA4: ;
    sp30[0] = *((f32 *)(v1 + 0x14));
    if (D_800A8F78 < (*((f32 *)(v1 + 0x118)))) {
        sp30[1] = (*((f32 *)(v1 + 0x118))) + 100.0f;
    } else {
        sp30[1] = (*((f32 *)(v1 + 0x18))) + 150.0f;
    }
    sp30[2] = *((f32 *)(v1 + 0x1C));
    temp = (s32)(arg0 + 0x34);
    if (func_15045800(sp30, 0, sp30[1] - 300.0f, temp) != 0) {
        sp30[1] = *((f32 *)(arg0 + 0x34));
        func_151ABE40(sp30, temp, 3, arg0[0xC], arg0[0x1]);
        func_10010FFC(0, 0x11, 0x5208, 0, 0, (s32)v1);
    }
    func_151ABE00(v1);
}

void func_151A9BA0(u8 *arg0) {
    u8 *v1 = *((u8 **)(arg0 + 0x18));
    f32 sp30[3];
    s32 temp;

    goto dummy_label_151A9BA0; dummy_label_151A9BA0: ;
    sp30[0] = *((f32 *)(v1 + 0x14));
    if (D_800A8F7C < (*((f32 *)(v1 + 0x118)))) {
        sp30[1] = (*((f32 *)(v1 + 0x118))) + 100.0f;
    } else {
        sp30[1] = (*((f32 *)(v1 + 0x18))) + 150.0f;
    }
    sp30[2] = *((f32 *)(v1 + 0x1C));
    temp = (s32)(arg0 + 0x34);
    if (func_15045800(sp30, 0, sp30[1] - 300.0f, temp) != 0) {
        sp30[1] = *((f32 *)(arg0 + 0x34));
        func_151ABE40(sp30, temp, 1, arg0[0xC], arg0[0x1]);
        func_10010FFC(0, 0x11, 0x5208, 0, 0, (s32)v1);
    }
}

extern f32 D_800A8F80;

void func_151A9CA0(u8 *arg0) {
    u8 *v1 = *((u8 **)(arg0 + 0x18));
    f32 sp30[3];

    if ((*((u8 **)(v1 + 0x31C)) != NULL) && ((*((u8 **)(v1 + 0x31C)))[0x95] != 0)) {
        return;
    }
    sp30[0] = *((f32 *)(v1 + 0x14));
    if (D_800A8F80 < (*((f32 *)(v1 + 0x118)))) {
        sp30[1] = (*((f32 *)(v1 + 0x118))) + 100.0f;
    } else {
        sp30[1] = (*((f32 *)(v1 + 0x18))) + 150.0f;
    }
    sp30[2] = *((f32 *)(v1 + 0x1C));
    if (func_15045800(sp30, 0, sp30[1] - 300.0f, (s32)(arg0 + 0x34)) != 0) {
        sp30[1] = *((f32 *)(arg0 + 0x34));
        func_151ABE40(sp30, (s32)(arg0 + 0x34), 1, arg0[0xC], arg0[0x1]);
        func_10010FFC(0, 8, 0x6978, 0, 0, (s32)v1);
    }
    func_151AA264(v1, (s32)(arg0 + 0x34));
    func_151ABE00(v1);
}

void func_151A9DC0(u8 *arg0) {
    u8 *v1 = *((u8 **)(arg0 + 0x18));
    f32 sp30[3];
    s32 temp;

    goto dummy_label_151A9DC0; dummy_label_151A9DC0: ;
    sp30[0] = *((f32 *)(v1 + 0x14));
    if (D_800A8F84 < (*((f32 *)(v1 + 0x118)))) {
        sp30[1] = (*((f32 *)(v1 + 0x118))) + 100.0f;
    } else {
        sp30[1] = (*((f32 *)(v1 + 0x18))) + 150.0f;
    }
    sp30[2] = *((f32 *)(v1 + 0x1C));
    temp = (s32)(arg0 + 0x34);
    if (func_15045800(sp30, 0, sp30[1] - 300.0f, temp) != 0) {
        sp30[1] = *((f32 *)(arg0 + 0x34));
        func_151ABE40(sp30, temp, 2, arg0[0xC], arg0[0x1]);
        func_10010FFC(0, 0x11, 0x5208, 0, 0, (s32)v1);
    }
}

void func_151A9EC0(u8 *arg0) {
    u8 *v1 = *((u8 **)(arg0 + 0x18));
    f32 sp30[3];
    s32 temp;

    goto dummy_label_151A9EC0; dummy_label_151A9EC0: ;
    sp30[0] = *((f32 *)(v1 + 0x14));
    if (D_800A8F88 < (*((f32 *)(v1 + 0x118)))) {
        sp30[1] = (*((f32 *)(v1 + 0x118))) + 100.0f;
    } else {
        sp30[1] = (*((f32 *)(v1 + 0x18))) + 150.0f;
    }
    sp30[2] = *((f32 *)(v1 + 0x1C));
    temp = (s32)(arg0 + 0x34);
    if (func_15045800(sp30, 0, sp30[1] - 300.0f, temp) != 0) {
        sp30[1] = *((f32 *)(arg0 + 0x34));
        func_151ABE40(sp30, temp, 2, arg0[0xC], arg0[0x1]);
        func_10010FFC(0, 8, 0x6978, 0, 0, (s32)v1);
    }
    func_151AA264(v1, temp);
    func_151ABE00(v1);
}

void func_151A9FC8(u8 *arg0) {
    u8 *v0 = *((u8 **)(arg0 + 0x18));
    f32 sp30[3];

    goto dummy_label_151A9FC8; dummy_label_151A9FC8: ;
    sp30[0] = *((f32 *)(v0 + 0x14));
    if (D_800A8F8C < (*((f32 *)(v0 + 0x118)))) {
        sp30[1] = (*((f32 *)(v0 + 0x118))) + 100.0f;
    } else {
        sp30[1] = (*((f32 *)(v0 + 0x18))) + 150.0f;
    }
    sp30[2] = *((f32 *)(v0 + 0x1C));
    ((s32 *)sp30)[-1] = (s32)(arg0 + 0x34);
    if (func_15045800(sp30, 0, sp30[1] - 300.0f, (s32)(arg0 + 0x34)) != 0) {
        sp30[1] = *((f32 *)(arg0 + 0x34));
        func_151ABE40(sp30, ((s32 *)sp30)[-1], 4, arg0[0xC], arg0[0x1]);
    }
}

void func_151AA09C(u8 *arg0) {
    u8 *v1 = *((u8 **)(arg0 + 0x18));
    f32 sp30[3];

    goto dummy_label_151AA09C; dummy_label_151AA09C: ;
    sp30[0] = *((f32 *)(v1 + 0x14));
    if (D_800A8F90 < (*((f32 *)(v1 + 0x118)))) {
        sp30[1] = (*((f32 *)(v1 + 0x118))) + 100.0f;
    } else {
        sp30[1] = (*((f32 *)(v1 + 0x18))) + 150.0f;
    }
    sp30[2] = *((f32 *)(v1 + 0x1C));
    ((s32 *)sp30)[-1] = (s32)(arg0 + 0x34);
    if (func_15045800(sp30, 0, sp30[1] - 300.0f, (s32)(arg0 + 0x34)) != 0) {
        sp30[1] = *((f32 *)(arg0 + 0x34));
        func_151ABE40(sp30, ((s32 *)sp30)[-1], 4, arg0[0xC], arg0[0x1]);
    }
}

void func_151AA170(s32 arg0) {
}

struct s_151AA210_arg {
    char pad0[0x18];
    s32 unk18;
    u8 unk1C;
};

struct s_151AA210_buf {
    s32 unk0;
    u8 unk4;
};

extern void func_15147D64(s32, s32);
extern void func_1519F3B8(struct s_151AA210_arg *);

void func_151AA17C(struct s_151AA210_arg *arg0) {
    s32 temp;
    struct s_151AA210_buf sp1C;

    temp = arg0->unk18;
    sp1C.unk0 = temp;
    sp1C.unk4 = arg0->unk1C;
    temp = (s32)&sp1C;
    func_15147D64(temp, 0xA);
    func_151494E0(temp, 0xA);
    func_1519F3B8(arg0);
}

extern void func_1519F400(void);

void func_151AA1D0(void) {
    func_1519F400();
}

void func_151AA1F0(void) {
    func_1519F400();
}

void func_151AA210(struct s_151AA210_arg *arg0) {
    s32 temp;
    struct s_151AA210_buf sp1C;

    temp = arg0->unk18;
    sp1C.unk0 = temp;
    sp1C.unk4 = arg0->unk1C;
    temp = (s32)&sp1C;
    func_15147D64(temp, 0xA);
    func_151494E0(temp, 0xA);
    func_1519F3B8(arg0);
}

void func_151AA264(void *arg0, s32 arg1) {
    struct_151AB2C4_desc sp18;

    if (*((u8 *)arg1 + 0x1C) & 1) {
        sp18.unk0 = 0;
        sp18.unk4 = 0;
        sp18.unk8 = *((u8 *)arg0 + 0x3B);
        sp18.unk10 = 1;
        sp18.unk14 = 0.0f;
        sp18.unk18 = 0.0f;
        sp18.unk1C = 0.0f;
        sp18.unk20 = 25.0f;
        sp18.unk24 = D_800A8F94;
        sp18.unk28 = 0x1E;
        sp18.unk2A = 0xE;
        sp18.unk2B = 2;
        sp18.unk2C = -1;
        sp18.unk2D = 0;
        sp18.unkC = arg0;
        func_1513418C(&sp18, 0, 0xFF, 0);
    }
}

void func_151AA30C(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, struct_151A9EC0_arg *arg6) {
    f32 sp5C[3];
    f32 sp50[3];

    if (((struct_151A9EC0_inner *)*(void **)((u8 *)arg6 + 0x1C))->unk118 < arg1) {
        return;
    }

    if (func_1515A920(*(void **)((u8 *)arg6 + 0x1C), &((s32 *)sp50)[-2]) == 0) {
        ((s32 *)sp50)[-2] = 0;
    }

    sp50[0] = 0.0f;
    sp50[1] = -(((func_150ADA68() * 196.0f) + 199.0f) * D_800A8F98 * arg4);
    sp50[2] = 0.0f;
    sp5C[0] = arg0;
    sp5C[1] = arg1;
    sp5C[2] = arg2;

    ((f32 *)sp50)[-6] = func_150ADA68();
    ((f32 *)sp50)[-5] = func_150ADA68();
    ((s32 *)sp50)[-4] = func_150ADA20();

    func_1515A238(sp5C, sp50, ((((f32 *)sp50)[-6] * D_800A8F9C) + 454.0f) * D_800A8FA0,
                  0.972683012f, ((s32 *)sp50)[-2], (((f32 *)sp50)[-5] * 101.0f) + 101.0f,
                  (((s32 *)sp50)[-4] % 0x1FU) + 0x32, (func_150ADA20() % 0x65U) + 0x64,
                  1, arg6->unkC, arg6->unk1);
}

typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ u8  pad1[0x3];
    /* 0x04 */ void *unk4;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8  unk24;
    /* 0x25 */ u8  pad25[0x1];
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ u8  unk2E;
    /* 0x2F */ u8  unk2F;
    /* 0x30 */ s8  unk30;
    /* 0x31 */ u8  pad31[0x3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ s8  unk39;
} struct_10E4_AA48C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8  unk4;
    /* 0x05 */ u8  pad5[0x3];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} struct_AA48C_b;

typedef struct {
    /* 0x00 */ u8  pad0[0x58];
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ f32 field_0x5C;
    /* 0x60 */ f32 field_0x60;
    /* 0x64 */ u8  pad64[0x24];
    /* 0x88 */ s32 field_0x88;
} struct_AA48C_effect;

extern f32 D_800A8FA4;
extern f32 D_800A8FA8;
extern void *func_15134DAC(struct_10E4_AA48C *, s32);

void *func_151AA48C(void *arg0, s32 arg1) {
    struct_10E4_AA48C sp34;
    void *temp_v0;
    struct_AA48C_b sp18;

    sp18.unk0 = arg1;
    sp18.unk4 = 1;
    sp34.unk0 = *((u8 *)arg0 + 0x3B);

    sp34.unk4 = arg0;
    sp34.unk8 = 0xF;
    sp34.unkC = -11.0f;
    sp34.unk10 = -2.0f;
    sp34.unk14 = 8.0f;
    sp34.unk18 = -11.0f;
    sp34.unk1C = -2.0f;
    sp34.unk20 = 20.0f;
    sp34.unk24 = 0;
    sp34.unk26 = 0x3C;
    sp34.unk28 = 0x3C;
    sp34.unk2A = 0x12C;
    sp34.unk2E = 1;
    sp34.unk2F = 0;
    sp34.unk30 = 1;
    sp34.unk34 = 0.5f;
    sp34.unk38 = 1;
    sp34.unk39 = 0;

    sp18.unk8 = 0;
    sp18.unkC = 0x11111;
    sp18.unk10 = D_800A8FA4;
    sp18.unk14 = D_800A8FA8;

    temp_v0 = func_15134DAC(&sp34, 0x18);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x80, &sp18, 0x18);
    }
    return temp_v0;
}

s32 func_151AA5A4(struct_AA48C_effect *arg0) {
    arg0->field_0x88 = 0;
    if (func_150ADA20() & 1) {
        func_10010F88(0xA, 0x55F0, (s16)((func_151EF610() % 0x4B0) - 0x258), 0, 0,
                      (s32)arg0->field_0x58, (s32)arg0->field_0x5C,
                      (s32)arg0->field_0x60, 0x1F4, 0x9C4);
        return 1;
    }
    func_10010F88(0xB, 0x55F0, (s16)((func_151EF610() % 0x4B0) - 0x258), 0, 0,
                  (s32)arg0->field_0x58, (s32)arg0->field_0x5C,
                  (s32)arg0->field_0x60, 0x1F4, 0x9C4);
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1D6E80/func_151AA6D8.s")

typedef struct Sub151AAA4C {
    char pad0[0x8];
    /* 0x88 */ u32 unk8;
    /* 0x8C */ u32 unkC;
    /* 0x90 */ f32 unk10;
    /* 0x94 */ f32 unk14;
} Sub151AAA4C;
typedef struct Obj151AAA4C {
    char pad0[0x80];
    Sub151AAA4C unk80;
} Obj151AAA4C;


f32 func_151AAA4C(Obj151AAA4C *arg0) {
    Sub151AAA4C *s;
    f32 r;

    s = &arg0->unk80;
    r = (func_151423D8((s->unk8 >> 16) - 0x40) * s->unk14) + s->unk10;
    s->unk8 += s->unkC * D_800BE9E4;
    return r;
}

typedef struct Flag151AAABC {
    char pad0[0x4];
    /* 0x4 */ u8 unk4;
} Flag151AAABC;

typedef struct EffA151AAABC {
    char pad0[0x80];
    /* 0x80 */ Flag151AAABC unk80;
} EffA151AAABC;

typedef struct EffB151AAABC {
    char pad0[0x58];
    /* 0x58 */ Flag151AAABC unk58;
} EffB151AAABC;

typedef struct Sub151AAABC {
    char pad0[0x14];
    /* 0x14 */ EffA151AAABC *unk14;
    char pad18[0x4];
    /* 0x1C */ EffB151AAABC *unk1C;
} Sub151AAABC;

typedef struct Obj151AAABC {
    char pad0[0x18];
    /* 0x18 */ void *unk18;
    char pad1C[0x58 - 0x1C];
    /* 0x58 */ Sub151AAABC unk58;
} Obj151AAABC;

void *func_151AB2C4(struct_151AB2C4_arg *, s32);

void func_151AAABC(Obj151AAABC *arg0) {
    void *v;
    Sub151AAABC *p;

    v = arg0->unk18;
    p = &arg0->unk58;
    if (p->unk14 != NULL) {
        Flag151AAABC *q;

        q = &p->unk14->unk80;
        q->unk4 = 1;
    } else {
        p->unk14 = func_151AA48C(v, (s32)arg0);
    }
    if (p->unk1C != NULL) {
        Flag151AAABC *r;

        r = &p->unk1C->unk58;
        r->unk4 = 1;
    } else {
        p->unk1C = func_151AB2C4(v, (s32)arg0);
    }
}

void func_151352EC(struct102 *arg0);

void func_151AAB50(struct102 *arg0) {
    s32 *p = (s32 *)(*(s32 *)((u8 *)arg0 + 0x80) + 0x58);
    p[5] = 0;
    func_151352EC(arg0);
}

void func_1513530C(struct102 *arg0);

void func_151AAB78(struct102 *arg0) {
    s32 *p = (s32 *)(*(s32 *)((u8 *)arg0 + 0x80) + 0x58);
    p[5] = 0;
    func_1513530C(arg0);
}

s32 func_151AABA0(u8 *arg0) {
    s32 ret = 1;
    if (arg0[0x84] == 0) {
        ret = 0;
    }
    arg0[0x84] = 0;
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1D6E80/func_151AABC4.s")

typedef struct {
    u8 pad_0x0[0x1B];
    u8 field_0x1B;
} AlphaFadePayload;

typedef struct {
    u8 pad_0x0[0x1C];
    s16 field_0x1C;
    u8 pad_0x1E[0x7A];
    AlphaFadePayload *field_0x98;
} AlphaFadeOwner;

s32 func_151AADBC(AlphaFadeOwner *arg0) {
    AlphaFadePayload *p;
    s32 v1;

    p = arg0->field_0x98;
    v1 = arg0->field_0x1C << 4;
    if (v1 >= 0x100) {
        v1 = 0xFF;
    }
    if (v1 < p->field_0x1B) {
        p->field_0x1B = v1;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    s16 unk4;
    u8 unk6;
    u8 unk7;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    s32 unk18;
    s32 unk1C;
    u8 unk20;
    u8 pad21[7];
} Particle_151AADF8;

typedef struct {
    void *unk0;
    u8 unk4;
    u8 pad5[3];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    u8 pad1A[2];
    f32 unk1C;
} Payload_151AADF8;

typedef struct {
    u8 pad0[4];
    u8 unk4;
} Flag_151AADF8;

typedef struct {
    u8 pad0[0xB0];
    Flag_151AADF8 unkB0;
} Eff_151AADF8;

typedef struct {
    u8 pad0[0x18];
    Eff_151AADF8 *unk18;
} Sub_151AADF8;

typedef struct {
    u8 pad0[0x18];
    struct_151A9EC0_inner *unk18;
    u8 pad1C[0x34 - 0x1C];
    f32 unk34;
    u8 unk38[0x20];
    Sub_151AADF8 unk58;
} Obj_151AADF8;

extern u8 D_800AB414[];

void func_151AADF8(Obj_151AADF8 *arg0) {
    struct_151A9EC0_inner *v;
    Sub_151AADF8 *p;
    Flag_151AADF8 *q;
    f32 pos[3];
    Particle_151AADF8 part;
    Payload_151AADF8 payload;
    f32 size;
    u8 color;

    v = arg0->unk18;
    p = &arg0->unk58;
    if (p->unk18 != NULL) {
        q = &p->unk18->unkB0;
        q->unk4 = 1;
    dummy:
        return;
    }
    pos[0] = v->unk14;
    if (-9500.0f < *(f32 *)((u8 *)v + 0x118)) {
        pos[1] = *(f32 *)((u8 *)v + 0x118) + 100.0f;
    } else {
        pos[1] = v->unk18 + 150.0f;
    }
    pos[2] = v->unk1C;
    if (func_15045800(pos, 0, pos[1] - 300.0f, (s32)&arg0->unk34) != 0) {
        color = func_151D8E20();
        pos[1] = arg0->unk34;
        size = (func_150ADA68() * 30.0f) + 50.0f;
        payload.unk0 = arg0;
        payload.unk4 = 1;
        payload.unk16 = 10;
        payload.unk18 = 10;
        payload.unk8 = 26214.40039f;
        payload.unkC = 32768.0f;
        payload.unk10 = 0.0f;
        payload.unk14 = 0;
        payload.unk1C = 0.2310000062f;
        part.unkC = (s32)(payload.unkC * 0.5f + payload.unk8);
        part.unk6 = 0x38;
        part.unk0 = 0x60300;
        part.unk4 = 0x12C;
        part.unk10 = 0x96;
        part.unk8 = 0;
        part.unk11 = 0xFF;
        part.unk12 = D_800AB414[color * 3];
        part.unk13 = D_800AB414[color * 3 + 1];
        part.unk14 = D_800AB414[color * 3 + 2];
        part.unk15 = 0xFF;
        part.unk18 = 0x440001;
        part.unk16 = 0;
        part.unk17 = 6;
        p->unk18 = (Eff_151AADF8 *)func_1513C73C((s32)&part, 7, 1, (s32)arg0->unk38, pos[0], pos[1], pos[2], size, size,
                            func_150ADA20() & 0xFF, (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2), 0x20, 0xFF, 1);
        if (p->unk18 != NULL) {
            memcpy(&p->unk18->unkB0, &payload, 0x20);
        }
    }
}

extern f32  func_150ADA68(void);

typedef struct {
    /* 0x00 */ u8  pad0[0x4];
    /* 0x04 */ u8  unk4;
    /* 0x05 */ u8  pad5[0x3];
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  pad1A[0x2];
    /* 0x1C */ f32 unk1C;
} struct_AB090;

typedef struct {
    /* 0x00 */ u8  pad0[0x24];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8  pad28[0x88];
    /* 0xB0 */ struct_AB090 unkB0;
} struct_AB090_root;

s32 func_151AB090(struct_AB090_root *arg0) {
    s8 ret = 1;
    struct_AB090 *v1 = &arg0->unkB0;

    if (*((u8 *)arg0 + 0xB4) == 0) {
        ret = 0;
    }
    v1->unk4 = 0;
    v1->unk14 -= D_800BE9E4;
    if (v1->unk14 < 0) {
        v1->unk14 = (func_150ADA20() % (u32)(v1->unk18 + 1)) + v1->unk16;
        v1->unk10 = func_150ADA68() * v1->unkC + v1->unk8;
    }
    arg0->unk24 += (s32)((v1->unk10 - (f32)arg0->unk24) * v1->unk1C);
    return ret;
}

extern s32 func_1513F6C0(void *arg0, s32 arg1, s32 arg2);

s32 func_151AB180(u8 *arg0) {
    s32 *temp = (s32 *)((*(u8 **)((u8 *)arg0 + 0xB0)) + 0x58);
    temp[6] = 0;
    *(s32 *)((u8 *)arg0 + 0x18) |= 2;
    *(u8 **)((u8 *)arg0 + 0xB0) = 0;
    func_1513F6C0(arg0, 0, 0);
    return 0;
}

void func_151AB1C4(u8 *arg0) {
    u8 *v1 = *((u8 **)(arg0 + 0x18));
    f32 sp30[3];
    s32 temp;

    goto dummy_label_151AB1C4; dummy_label_151AB1C4: ;
    sp30[0] = *((f32 *)(v1 + 0x14));
    if (D_800A8FEC < (*((f32 *)(v1 + 0x118)))) {
        sp30[1] = (*((f32 *)(v1 + 0x118))) + 100.0f;
    } else {
        sp30[1] = (*((f32 *)(v1 + 0x18))) + 150.0f;
    }
    sp30[2] = *((f32 *)(v1 + 0x1C));
    temp = (s32)(arg0 + 0x34);
    if (func_15045800(sp30, 0, sp30[1] - 300.0f, temp) != 0) {
        sp30[1] = *((f32 *)(arg0 + 0x34));
        func_151ABE40(sp30, temp, 3, arg0[0xC], arg0[0x1]);
        func_10010FFC(0, 0x11, 0x5208, 0, 0, (s32)v1);
    }
    func_151AA264(v1, temp);
}

void *func_151AB2C4(struct_151AB2C4_arg *arg0, s32 arg1) {
    struct_151AB2C4_desc sp28;
    struct_151AB2C4_payload sp1C;
    void *temp_v0;

    sp1C.unk0 = arg1;
    sp1C.unk6 = 0;
    sp1C.unk4 = 1;
    sp1C.unk8 = arg0->unk118;
    sp28.unk0 = 0;
    sp28.unk4 = 0;
    sp28.unk8 = arg0->unk3B;
    sp28.unk10 = 1;
    sp28.unk14 = 0.0f;
    sp28.unk18 = 0.0f;
    sp28.unk1C = 0.0f;
    sp28.unk28 = 0x12C;
    sp28.unk2A = 0xA;
    sp28.unk2B = 3;
    sp28.unk2C = 0;
    sp28.unk2D = 1;
    sp28.unkC = arg0;
    sp28.unk20 = 25.0f;
    sp28.unk24 = D_800A8FF0;

    temp_v0 = func_1513418C(&sp28, 0xC, 0xFF, 0);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x58, &sp1C, 0xC);
    }
    return temp_v0;
}

typedef struct Game1D6E80Burst {
    s32 flags, zero4;
    s16 texture, lifetime;
    s32 zeroC, zero10;
    u8 zero14, zero15, zero16, zero17, zero18, zero19, zero1A;
    u8 alpha, maximum, kind;
    s16 duration, period, zero22;
    f32 zero24, width, height;
    f32 x, y, z;
    f32 zero3C, zero40, zero44;
    f32 vx, vy, vz, rotation;
    s32 mode, unused5C;
    s8 field60, field61, field62, field63;
    u8 pad64[0xC];
} Game1D6E80Burst;

typedef struct Game1D6E80BurstMotion {
    f32 size;
    u8 mode0, mode1, count0, count1;
    f32 amount0, amount1, speed, damping;
    u8 mode2, mode3, count2, count3;
    f32 amount2, amount3, sourceScale;
} Game1D6E80BurstMotion;

void *func_151303BC(s32, u8, s32);
extern f32 D_800A8FF4, D_800A8FF8, D_800A8FFC, D_800A9000;
extern f32 D_800A9004, D_800A9008, D_800A900C, D_800A9010;

void func_151AB3A4(f32 arg0, f32 arg1, f32 arg2, f32 arg3,
                  f32 arg4, f32 arg5, void *arg6) {
    Game1D6E80Burst burst;
    void *spawned;
    Game1D6E80BurstMotion motion;
    burst.kind = 0x2F;
    burst.texture = 0xC01;
    burst.flags = 0x200005;
    burst.zero4 = 0;
    burst.zero1A = 0;
    burst.zero19 = 0;
    burst.zero18 = 0;
    burst.zero17 = 0;
    burst.zero16 = 0;
    burst.zero15 = 0;
    burst.zero14 = 0;
    burst.zero10 = 0;
    burst.zeroC = 0;
    burst.maximum = 0xFF;
    burst.zero22 = 0;
    burst.zero24 = 0.0f;
    burst.mode = 0x3207;
    burst.field60 = 5;
    burst.field61 = 5;
    burst.field62 = 4;
    burst.field63 = -1;
    burst.x = arg0;
    burst.y = arg1;
    burst.zero3C = 0.0f;
    burst.zero40 = 0.0f;
    burst.zero44 = 0.0f;
    burst.duration = 30;
    burst.period = 8;
    burst.z = arg2;
    motion.sourceScale = *(f32 *)((u8 *)arg6 + 0x60);
    motion.speed = 4.0f;
    motion.damping = 0.25f;
    motion.mode0 = 0;
    motion.mode1 = 0;
    motion.mode2 = 0;
    motion.mode3 = 0;
    burst.vx = -arg3 * 0.2f;
    burst.vy = 0.0f;
    burst.vz = -arg5 * 0.2f;
    if (func_150ADA20() & 1) burst.mode |= 0x40;
    if (func_150ADA20() & 1) burst.mode |= 0x80;
    burst.lifetime = func_150ADA20() % 51U + 106;
    burst.alpha = func_150ADA20() % 101U + 100;
    motion.size = burst.width = burst.height = func_150ADA68() * 62.0f + 41.0f;
    motion.count0 = func_150ADA20() % 5U + 4;
    motion.count1 = func_150ADA20() % 5U + 4;
    motion.amount0 = (func_150ADA68() * 0.25f + D_800A8FF8) * motion.size;
    motion.amount1 = (func_150ADA68() * 0.25f + D_800A8FFC) * motion.size;
    motion.count2 = func_150ADA20() % 5U + 4;
    motion.count3 = func_150ADA20() % 5U + 4;
    motion.amount2 = (func_150ADA68() * D_800A9000 + D_800A9004) * motion.size;
    motion.amount3 = (func_150ADA68() * D_800A9008 + D_800A900C) * motion.size;
    burst.rotation = (func_150ADA68() * 60.0f + 21.0f) * D_800A9010;
    spawned = func_151303BC((s32)&burst, 2, 0x28);
    if (spawned != NULL) memcpy((u8 *)spawned + 0xA8, &motion, 0x28);
}

typedef struct {
    u8 pad_0x0[0x4];
    u8 field_0x4;
    u8 pad_0x5[0x1];
    u16 field_0x6;
} SoundHandlePlayback;

typedef struct {
    u8 pad_0x0[0x1C];
    s32 field_0x1C;
} SoundHandleState;

typedef struct {
    u8 pad_0x0[0x58];
    s32 field_0x58;
    u8 pad_0x5C[0x2];
    volatile u16 field_0x5E;
} SoundHandleOwner;

s32 func_151AB6B8(SoundHandleOwner *arg0) {
    u8 ret;
    SoundHandlePlayback *temp_v1;

    ret = 1;
    temp_v1 = (SoundHandlePlayback *)&arg0->field_0x58;
    if (arg0->field_0x5E != 0) {
        if (D_800DBFF0->unk5F0 & 1) {
        } else {
            func_100111C8(temp_v1->field_0x6);
            temp_v1->field_0x6 = 0;
            goto done_sound;
        }
    }
    if (temp_v1->field_0x6 == 0) {
        if (D_800DBFF0->unk5F0 & 1) {
            temp_v1->field_0x6 = func_10010F30(0x355, 0x7D00, 0x40, 0, 0);
        }
    }

done_sound:
    if (temp_v1->field_0x4 == 0) {
        ret = 0;
    }
    temp_v1->field_0x4 = 0;
    return ret;
}

void func_151AB788(SoundHandleOwner *arg0) {
    union {
        SoundHandleState *v0;
        f64 align;
    } sp18;

    sp18.v0 = (SoundHandleState *)(arg0->field_0x58 + 0x58);
    if (arg0->field_0x5E != 0) {
        func_100111C8(arg0->field_0x5E);
    }
    sp18.v0->field_0x1C = 0;
    func_151346EC((u8 *)arg0);
}

void func_151AB7D8(SoundHandleOwner *arg0) {
    union {
        SoundHandleState *v0;
        f64 align;
    } sp18;

    sp18.v0 = (SoundHandleState *)(arg0->field_0x58 + 0x58);
    if (arg0->field_0x5E != 0) {
        func_100111C8(arg0->field_0x5E);
    }
    sp18.v0->field_0x1C = 0;
    func_1513470C((u8 *)arg0);
}

void func_151AB828(u8 *arg0) {
    u8 *a3;

    a3 = arg0;
    func_15141DA4(*(void **)(a3 + 0x18), 0, 4);
}

void func_151AB854(u8 *arg0) {
    u8 *v1;
    struct185 tmp;

    v1 = *(u8 **)(arg0 + 0x18);
    if ((v1[4] == 0) || (v1[4] == 1) || (v1[4] == 2) || (v1[4] == 3) || (v1[4] == 4) || (v1[4] == 0x96)) {
        tmp.unk0 = (struct127 *)v1;
        tmp.unk4 = v1[0x3B];
        tmp.unk6 = 0x12C;
        tmp.unk8 = 0;
        tmp.unk9 = 0;
        if (v1[4] == 0x96) {
            tmp.unkA = 3;
        } else {
            tmp.unkA = 0;
        }
        if (v1[0x127] != 0xFF) {
            *(s16 *)(*(u8 **)(v1 + 0x31C) + 0x66) = 0x1F4;
        }
        func_15190770(&tmp, 0, arg0[0xC], arg0[1]);
    }
}

void func_151AB920(s32 arg0, s32 arg1) {
}

void func_151AB930(struct127 *arg0) {
    struct260 *temp_v0;
    struct {
        struct127 *unk0;
        u8 unk4;
        u8 pad5[3];
        s32 unk8;
        f32 unkC;
        f32 unk10;
        f32 unk14;
        u8 unk18;
    } sp28;

    sp28.unk0 = arg0;
    sp28.unk4 = ((struct127 *)arg0)->unique_id;
    sp28.unk8 = arg0->unk84.uh;
    sp28.unkC = 0.0f;
    sp28.unk10 = D_800A9014;
    sp28.unk14 = 3.0f;
    sp28.unk18 = func_151D8E20();
    temp_v0 = func_151491F4(0x3C, -1, 0xE, 1, 9, 0x1C, 0xFF, 0);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp28, 0x1C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1D6E80/func_151AB9C8.s")

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA_151ABD54;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB_151ABD54;

void func_151ABD54(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_151ABD54 *temp_v0 = (SubA_151ABD54 *)((u8 *)arg0 + 0x28);
    ArgB_151ABD54 *b = (ArgB_151ABD54 *)arg1;

    if (arg2 == 0) {
        if ((b->unk0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }
}

struct tmp151ABE00 {
    s32 unk0;
    s8  unk4;
};

void func_151ABE00(void *arg0) {
    struct tmp151ABE00 tmp;

    tmp.unk0 = (s32)arg0;
    tmp.unk4 = *(u8 *)((char *)arg0 + 0x3B);

    func_1516944C(0x20, (s32)&tmp, 0xC);
}
