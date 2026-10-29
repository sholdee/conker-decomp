#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ f32 unk8[3];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ s8 unk2D;
    /* 0x2E */ u8 unk2E[2];
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ s8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 unk3A[2];
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s8 unk40;
    /* 0x41 */ u8 pad41[3];
    /* 0x44 */ f32 unk44;
} Struct151DC6A0;

typedef struct {
    f32 v[3];
} Vec3Copy_151DC6A0;

void func_15103254(s16, u8, f32, struct127 *, s32, u8, s32);
void func_15150178(Struct151DC6A0 *arg0, f32 *arg1, s32 arg2, u8 arg3, s32 arg4);
void func_151D3F14(struct127 *, u8, s32);
void func_15182670(u8, u8, u8, u8, s32, s32, u8, s32);

void func_151DC6A0(struct127 *arg0, u8 arg1, s32 arg2) {
    func_15103254(func_150ADA20() % 5 + 12, func_150ADA20() % 56 + 200, func_150ADA68() * 600.0f + 1700.0f, arg0, 0xFF, arg1, arg2);
    {
    Struct151DC6A0 sp38;
    *(Vec3Copy_151DC6A0 *)sp38.unk8 = *(Vec3Copy_151DC6A0 *)arg0;
    sp38.unk14 = 0xC;
    sp38.unk16 = 7;
    sp38.unk0 = 0;
    sp38.unk2 = 0xFF;
    sp38.unk4 = -0x40;
    sp38.unk6 = 0x3C;
    sp38.unk20 = 0x24;
    sp38.unk22 = 0x3C;
    sp38.unk2C = 0xC8;
    sp38.unk2D = 0x37;
    sp38.unk38 = 0;
    sp38.unk39 = 0xC;
    sp38.unk40 = 1;
    sp38.unk18 = 8.0f;
    sp38.unk1C = 10.0f;
    sp38.unk24 = -1.097000003f;
    sp38.unk28 = 0.3700000048f;
    sp38.unk30 = 280.0f;
    sp38.unk34 = 390.0f;
    sp38.unk3C = 0.6000000238f;
    sp38.unk44 = 1.0f;
    func_15150178(&sp38, sp38.unk8, 0, arg1, arg2);
    }
    func_151D3F14(arg0, arg1, arg2);
    func_15182670(0xFF, 0xFF, 0xFF, func_150ADA20() % 51 + 150, func_150ADA20() % 5 + 8, 0, arg1, arg2);
}

void func_151DC6A0(struct127 *arg0, u8 arg1, s32 arg2);

struct dc8bc_a { s32 unk0[3]; };

void func_151DC8BC(struct127 *arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5) {
    struct {
        struct dc8bc_a unk0;
        f32 unkC;
        u8 unk10;
    } sp34;
    struct260 *temp_v0;

    if (arg2 != 0) {
        func_151DC6A0(arg0, arg4, arg5);
    }

    sp34.unk0 = *(struct dc8bc_a *)arg0;
    sp34.unkC = 0.0f;
    sp34.unk10 = arg3;

    temp_v0 = func_15149130((s16)arg1, -1, 0x5B, -1, 1, 0, (struct37 *)0x14, arg4, arg5);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0x14);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_209B50/func_151DC97C.s")
