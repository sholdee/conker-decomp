#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    f32 unk0;
    f32 unk4;
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
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    s32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    s32 unk64;
} SomeStruct1512;

extern SomeStruct1512 D_800DC2C0[];

void func_1512D6F0(u8 *arg0) {
    SomeStruct1512 *sp = &D_800DC2C0[arg0[0x23D]];
    sp->unk50 = 5;
    sp->unk54 = 0.0f;
    sp->unk58 = 0.0f;
    sp->unk5C = 0.0f;
    sp->unk60 = 0.0f;
    sp->unk2C = 0.0f;
    sp->unk28 = -1.0f;
}

extern u8 D_800895D0[][10];

void func_1512D748(struct108 *arg0, s32 arg1, s32 arg2) {
    SomeStruct1512 *sp = &D_800DC2C0[arg0->unk23D];
    u8 *tbl;
    volatile f32 sp4;
    f32 f0;
    f32 f2;
    f32 f18;
    f32 f16;
    f32 f12;
    f32 f14;

    if (((u8 *)arg0->unk3D4)[0x120] == 0) {
        tbl = D_800895D0[arg1];
        sp->unk0 = (u32)tbl[0];
        sp->unk4 = (u32)tbl[1];
        sp->unk8 = (u32)tbl[2];
        sp->unkC = (u32)tbl[3];
        sp->unk10 = (u32)tbl[4];
        sp->unk14 = (u32)tbl[5];
        sp->unk18 = (u32)tbl[6];
        sp->unk1C = (u32)tbl[7];
        sp->unk20 = (u32)tbl[8];
        sp->unk24 = (u32)tbl[9];
        f0 = sp->unk10;
        f2 = sp->unk14;
        f12 = f2 + f0;
        f14 = sp->unk18 + f12;
        f16 = sp->unk1C;
        f18 = sp->unk20;
        sp->unk30 = f0;
        sp->unk34 = f12;
        sp->unk38 = f14;
        sp->unk3C = f16 + f14;
        sp->unk28 = 0.0f;
        sp->unk2C = 0.0f;
        sp->unk40 = f18 / f0;
        sp4 = sp->unk24;
        sp->unk44 = (f18 - sp4) / f2;
        sp->unk48 = 0.0f;
        sp->unk4C = sp4 / f16;
        sp->unk50 = 0;
        sp->unk64 = arg2;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_15ABA0/func_1512D980.s")
