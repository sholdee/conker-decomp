#include <ultra64.h>
#define func_10010F88 func_10010F88_void_decl_in_functions_h
#include "functions.h"
#undef func_10010F88
#include "variables.h"

extern f32 D_800A1BD8;
extern f32 sinf(f32);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);
extern s32 func_15146078(struct17 *, struct17 *, struct17 *);

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    struct17 unk28;
    struct17 unk34;
    struct17 unk40;
    f32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 unk65;
    u8 unk66;
    u8 unk67;
    u8 unk68;
    u8 pad69;
    u8 unk6A;
    u8 pad6B;
    s32 unk6C;
    u8 unk70;
    u8 pad71;
    s16 unk72;
    s16 unk74;
    u8 pad76[2];
    s32 unk78;
} Func150F802CSpawn;

struct Func150F7E20Sub {
    u8 pad0[0x18];
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
};

struct Func150F7E20Obj {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x28];
    u8 unk5C;
};



typedef struct {
    u8 pad0[4];
    f32 unk4;
    u8 pad8[0x30];
    struct17 unk38;
    u8 pad44[0x20];
    s32 unk64;
    f32 unk68;
    f32 unk6C;
    u8 unk70;
    u8 pad71[3];
    struct17 unk74;
    struct17 unk80;
    f32 unk8C;
    f32 unk90;
    f32 unk94;
    struct17 unk98;
    struct17 unkA4;
    s8 unkB0;
    u8 padB1[3];
} Func150F7470Big;

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
} Func150F7470Obj;

extern s32 func_151C4B0C(s32, Func150F7470Big *, struct17 *, struct17 *, f32 *, f32, u8, s32, struct17 *, u8 *, s32,
                         f32, Func150F7470Obj *, s32, u8, u8, s32, u8);
extern void func_15145974(struct17 *, f32 *, f32 *);
s32 func_1513264C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6);
s32 func_10010F88(s32 arg0, u16 arg1, s16 arg2, u8 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 arg9);


void func_150F7470(struct17 *arg0, struct17 *arg1, s32 arg2, struct17 *arg3, u8 arg4, s32 arg5, f32 arg6, f32 arg7,
                   f32 arg8, Func150F7470Obj *arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13, s32 arg14, s8 arg15,
                   s32 arg16, u8 arg17, u8 arg18, s32 arg19) {
    Func150F7470Big big;
    Func150F802CSpawn spawn;
    s32 ret;
    struct17 dirB;
    u8 hasB;
    struct17 dirA;
    u8 hasA;

    hasB = 0;
    hasA = arg4;
    if (arg4) {
        dirA = *arg3;
    }
    big.unk70 = (arg11 ? 1 : 0) | (arg12 ? 2 : 0) | (arg13 ? 4 : 0);
    big.unkB0 = arg15;
    if (arg2 != 0 && arg3 != NULL) {
        if (!hasA) {
            if (!func_15145128(arg3, &dirA, NULL, NULL)) {
                return;
            }
            hasA = 1;
        }
    } else if (arg1 != NULL) {
        if (!func_15145128(arg1, &dirB, NULL, NULL)) {
            return;
        }
        hasB = 1;
    }
    if (!func_151C4B0C(arg5, &big, arg0, hasB ? &dirB : NULL, &big.unk68, arg7, arg10, arg2, hasA ? &dirA : NULL,
                       &big.unk70, 1, arg8, arg9, arg15, arg12, arg13, 0, arg17)) {
        return;
    }
    if (!func_15146078(&big.unk38, &big.unk74, &big.unk80)) {
        return;
    }
    big.unk64 = arg14;
    big.unk90 = 0.0f;
    big.unk8C = 0.0f;
    big.unk94 = 70.0f * arg7;
    big.unk98 = *arg0;
    big.unk6C = arg6;
    big.unkA4.unk0 = big.unk38.unk0 * 70.0f;
    big.unkA4.unk4 = big.unk38.unk4 * 70.0f;
    big.unkA4.unk8 = big.unk38.unk8 * 70.0f;
    big.unk70 |= (big.unk4 > 150.0f) ? 1 : 0;
    spawn.unk00 = 1.0f;
    spawn.unk04 = 1.0f;
    spawn.unk0C = 0.02000000142f;
    spawn.unk08 = 0.02000000142f;
    func_15145974(&big.unk38, &spawn.unk14, &spawn.unk10);
    spawn.unk1C = 1.0f;
    spawn.unk20 = 1.0f;
    spawn.unk24 = 1.0f;
    spawn.unk18 = 0.0f;
    spawn.unk28 = *arg0;
    spawn.unk34.unk0 = big.unk38.unk0 * arg6;
    spawn.unk34.unk4 = big.unk38.unk4 * arg6;
    spawn.unk34.unk8 = big.unk38.unk8 * arg6;
    spawn.unk40 = *(struct17 *)&D_800A5480;
    spawn.unk4C = 0.0f;
    spawn.unk50 = 0x120;
    spawn.unk54 = 0x12C;
    spawn.unk56 = 0x2F;
    spawn.unk58 = 0;
    spawn.unk5C = 0;
    spawn.unk60 = 0xFF;
    spawn.unk61 = 0x13;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 0;
    spawn.unk6A = 0;
    spawn.unk6C = (s32)arg9;
    spawn.unk70 = arg9->unk3B;
    spawn.unk72 = 1;
    spawn.unk74 = 0xFF;
    if (arg16 != 0) {
        spawn.unk78 = func_10010F88(arg16, 0x4650, -500, 0, -1, spawn.unk28.unk0, spawn.unk28.unk4, spawn.unk28.unk8,
                                    1000, 2000);
    } else {
        spawn.unk78 = 0;
    }
    ret = func_1513264C(&spawn, 0, 0, 0, 0xB4, arg18, arg19);
    if (ret != 0) {
        memcpy((void *)(ret + 0x170), &big, sizeof(big));
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_124920/func_150F78B4.s")

s32 func_150F7E20(struct Func150F7E20Obj *arg0) {
    f32 temp;
    struct Func150F7E20Sub *sub;

    sub = (struct Func150F7E20Sub *)((u8 *)arg0 + 0x110);
    arg0->unk5C = sub->unk18 * D_800A1BD8;
    temp = (sinf(sub->unk1C) * sub->unk28) + sub->unk24;
    arg0->unk30 = temp;
    arg0->unk2C = temp;
    sub->unk1C = func_15144B68(sub->unk1C + (sub->unk20 * D_800BE9A4));
    sub->unk18 -= D_800BE9A4;
    if (sub->unk18 < 0.0f) {
        return 0;
    }
    return 1;
}

void func_150F7F58(struct210 *arg0, s16 arg1) {
    func_15140410(arg0, &arg0->unk110, &arg0->unk11C, arg1);
}

extern f32 func_150ADA68(void);
void func_151541B8(void *, f32, u32, f32, f32, u8, s32);
void func_151D3F14(void *, u8, s32);
struct126 *func_150FF288(struct127 *);
s32 func_150FF6E0(struct17 *, struct17 *, struct17 *, struct17 *, struct17 *, struct127 *, struct126 *);
void func_150FF474(struct17 *, struct17 *, s32, s32);
void func_151D4408(struct17 *, struct17 *, void *, struct127 *, f32, s32, s32);

typedef u8 Func150F9950Entry[0x40];

void func_150F7F8C(void *arg0, u8 arg1, s32 arg2) {
    f32 temp1;
    f32 temp0;

    temp0 = func_150ADA68();
    temp1 = func_150ADA68();
    func_151541B8(arg0, temp0 * 3.0f + 9.0f, 0x3F030C35, temp1 * 70.0f + 70.0f, 0.0f, arg1, arg2);
    func_151D3F14(arg0, arg1, arg2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_124920/func_150F802C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_124920/func_150F81BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_124920/func_150F85A0.s")

void func_150F884C(s32 arg0, s32 arg1) {
    s32 sp18[2];
    sp18[0] = arg1;
    func_151494E0((s32)&sp18, 0x3F);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_124920/func_150F887C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_124920/func_150F892C.s")

extern u8 D_800A1C40[];

void func_150F9720(u8 arg0) {
    struct { s32 unk0; u8 unk4; } sp18;
    sp18.unk0 = 0;
    sp18.unk4 = D_800A1C40[arg0 * 2];
    func_151494E0((s32)&sp18, 0x42);
    sp18.unk4 = D_800A1C40[arg0 * 2 + 1];
    func_151494E0((s32)&sp18, 0x42);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_124920/func_150F9788.s")

void func_150F9788(struct260 *);

void func_150F9794(struct260 *arg0) {
    func_150F9788(arg0);
    func_1514933C(arg0);
}

void func_150F97C0(struct260 *arg0) {
    func_150F9788(arg0);
    func_15149368(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_124920/func_150F97EC.s")

void func_150F9950(struct127 *arg0, s32 arg1, s32 arg2) {
    struct17 sp60[6];
    struct17 sp54;
    struct17 sp48;
    struct17 sp3C;
    struct17 sp30;
    struct126 *temp;

    temp = func_150FF288(arg0);
    if (temp != 0) {
        if (func_150FF6E0(sp60, &sp54, &sp48, &sp3C, &sp30, arg0, temp) != 0) {
            func_151D3F14(&sp54, ((u8 *)&arg1)[3], arg2);
            func_151D4408(&sp48, &sp3C, (*(Func150F9950Entry **)&arg0->unk1D4)[*((u8 *)temp + 2)], arg0, 1.0f, ((u8 *)&arg1)[3], arg2);
            func_150FF474(&sp54, sp60, ((u8 *)&arg1)[3], arg2);
        }
    }
}
