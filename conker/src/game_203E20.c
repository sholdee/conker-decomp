#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern void func_150D6730(s32 arg0, s32 arg1, s32 arg2);
extern s32 func_15042E3C(s32 arg0, ...);
extern s32 func_15133EEC(s32 arg0, u16 arg1, u8 arg2, s32 arg3);
extern char D_800AB254[];
extern u32 D_80083740[];
extern u32 D_800838C0[];

typedef struct Obj151D7000 {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
} Obj151D7000;

typedef struct Sub151D7000 {
    Obj151D7000 *unk0;
    u8 unk4;
    u8 pad5[0x3];
    s32 unk8;
    s16 unkC;
    s16 unkE;
} Sub151D7000;

typedef struct Arg151D7000 {
    u8 pad0[0xE];
    s16 unkE;
    u8 pad10[0x38];
    Sub151D7000 *unk48;
} Arg151D7000;

void func_151D6970(s32 arg0, s32 arg1) {
    if ((D_800BE9F0 == 0x32) || (D_800BE9F0 == 0x33)) {
        func_150D6730(arg0, 0xFF, 1);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_203E20/func_151D69B4.s")

extern void *func_15157010(void *, s32, f32, s32, s32, s32, u8, s32);
extern void *func_150CFF10(u8, void *, s16, s32, s8, u8, u8, s32);
extern void *func_1509B570(s32);

typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ s8 unk01;
    /* 0x02 */ u8 unk02;
    /* 0x03 */ s8 unk03;
    /* 0x04 */ s8 unk04;
    /* 0x05 */ u8 pad05;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ Obj151D7000 *unk3C;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ struct17 unk44;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} S151D6BFC;

typedef struct {
    s32 unk0;
    Sub151D7000 unk4;
    s32 unk14;
} S151D6BFC_2;

typedef struct {
    u8 pad0[0x64];
    s32 unk64;
} S1509B570;

void func_151D6BFC(Obj151D7000 *arg0, u8 arg1, s32 arg2) {
    S151D6BFC sp50;
    s32 unused;
    sp50.unk38 = 0;
    sp50.unk00 = 0x27;
    sp50.unk01 = -1;
    sp50.unk02 = 3;
    sp50.unk03 = 1;
    sp50.unk04 = -1;
    sp50.unk06 = 0x96;
    sp50.unk08 = 0xA5;
    sp50.unk0C = 0x17;
    sp50.unk10 = 0;
    sp50.unk14 = 0x220405;
    sp50.unk18 = 0x40200;
    sp50.unk2D = 8;
    sp50.unk1C = 1;
    sp50.unk20 = 0x38;
    sp50.unk2C = 0;
    sp50.unk24 = 0x80;
    sp50.unk28 = 0x20;
    sp50.unk30 = 0xFF;
    sp50.unk31 = 0xFF;
    sp50.unk32 = 0xFF;
    sp50.unk33 = 0xFF;
    sp50.unk34 = 0xFF;
    sp50.unk35 = 0xFF;
    sp50.unk36 = 0xFF;
    sp50.unk37 = 0xFF;
    sp50.unk3C = arg0;
    sp50.unk40 = arg0->unk3B;
    sp50.unk41 = 1;
    sp50.unk44 = *(struct17 *)&D_800A5480;
    sp50.unk54 = 8;
    sp50.unk56 = 0x1F;
    func_15157010(&sp50, 0, 1.0f, 0, 0, 0, arg1, arg2);
    {
    Sub151D7000 sub;
    char buf[4] = "500";
    S1509B570 *p;
    s32 v;
    void *ret;
    sub.unk0 = arg0;
    sub.unk4 = arg0->unk3B;
    sub.unkC = 8;
    sub.unkE = 0x1F;
    sub.unk8 = func_150859AC(0, 6);
    p = func_1509B570(0x83);
    if (p != NULL) {
        v = p->unk64 + 1;
        if (v == 0xFF) {
            sub.unk8 += 100;
        } else if (v == 0) {
            sub.unk8 = 0;
        } else if (v < 3) {
            s32 t;
            t = sub.unk8 * v;
            sub.unk8 = t / 3;
        }
    } else {
        sub.unk8 = 0;
    }
    ret = func_150CFF10(0x63, buf, 0x96, 0x10, 3, 1, arg1, arg2);
    if (ret != NULL) {
        memcpy(*(void **)((u8 *)ret + 0x48), &sub, 0x10);
    }
    }
}

extern f32 func_15144AA8(s32);
extern f32 D_800AB278;

struct Foo203E20 {
    u8 pad0[0x4C];
    s32 *unk4C;
    u8 pad50[0x7C - 0x50];
    f32 mat[1][4][4];
};

s32 func_151D6E60(struct Foo203E20 *arg0) {
    f32 m;

    func_150A8050(arg0->mat[D_800BE9C0], 0.0f, func_15144AA8(0) + 25.0f, 0.0f);

    m = D_800AB278;
    arg0->mat[D_800BE9C0][3][0] = ((f32 *)arg0->unk4C)[5];
    arg0->mat[D_800BE9C0][3][1] = ((f32 *)arg0->unk4C)[6] + 120.0f;
    arg0->mat[D_800BE9C0][3][2] = ((f32 *)arg0->unk4C)[7];

    arg0->mat[D_800BE9C0][0][0] *= m;
    arg0->mat[D_800BE9C0][0][1] *= m;
    arg0->mat[D_800BE9C0][0][2] *= m;
    arg0->mat[D_800BE9C0][1][0] *= m;
    arg0->mat[D_800BE9C0][1][1] *= m;
    arg0->mat[D_800BE9C0][1][2] *= m;
    arg0->mat[D_800BE9C0][2][0] *= m;
    arg0->mat[D_800BE9C0][2][1] *= m;
    arg0->mat[D_800BE9C0][2][2] *= m;

    return 1;
}

s32 func_151D7000(Arg151D7000 *arg0) {
    Obj151D7000 *obj;
    Sub151D7000 *sub;
    s16 temp;
    s16 pad;
    s16 value;

    sub = arg0->unk48;
    obj = sub->unk0;
    if ((obj->unk0 == 0) || (obj->unk3B != sub->unk4)) {
        return 0;
    }

    temp = arg0->unkE;
    if (temp < sub->unkC) {
        value = temp * sub->unkE;
        if (value < 0) {
            value = 0;
        }
    } else {
        value = 0xFF;
    }

    func_150432CC((s32)obj, 0x109);
    func_1504332C(0x32, 0x7D, 0x1C, value);
    func_15042D78(0x81);
    return func_15042E3C((s32)D_800AB254, sub->unk8);
}

void func_151D70CC(s32 *arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0[0x12], arg0[0x12] + 4, (s32)arg0);
}

void func_151D710C(Gfx *arg0, void *arg1, s32 arg2, s32 arg3, u8 *arg4) {
    if (*(u8 *)((u8 *)arg1 + 0x43) < 0xFF) {
        gSPSegment(arg0++, 8, D_80083740);
    } else {
        gSPSegment(arg0++, 8, D_800838C0);
    }

    func_15133EEC(func_15133EEC((s32)arg0, 0xC3, 6, 3), 0xC3, 7, 3);
    *arg4 = 1;
}
