#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_1199D0/func_150EC520.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1199D0/func_150EC6B0.s")

void func_150ECA68(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s16 arg6, u8 arg7, s32 arg8) {
    struct {
        void *unk0;
        u8 unk4;
        u8 unk5;
        u8 unk6;
        u8 unk7;
        u8 unk8;
        u8 unk9;
    } sp3C;
    struct260 *temp_v0;

    sp3C.unk0 = arg0;
    sp3C.unk4 = *((u8 *)arg0 + 0x3B);
    sp3C.unk8 = arg4;
    sp3C.unk9 = arg5;
    sp3C.unk5 = arg1;
    sp3C.unk6 = arg2;
    sp3C.unk7 = arg3;

    temp_v0 = func_15149130((arg6 == -1) ? 0x12C : arg6, -1, 0x5D, -1, (arg6 == -1) ? 0 : 1, 0x45, (struct37 *)0xC, arg7, arg8);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp3C, 0xC);
    }
}

void func_150ECB4C(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

typedef struct {
    /* 0x00 */ struct127 *field_0x00;
    /* 0x04 */ u8 field_0x04;
    /* 0x05 */ u8 field_0x05;
    /* 0x06 */ u8 field_0x06;
    /* 0x07 */ u8 field_0x07;
    /* 0x08 */ u8 field_0x08;
    /* 0x09 */ u8 field_0x09;
} ObjectColorCommandData;

typedef struct {
    /* 0x00 */ u8 pad_0[0xE];
    /* 0x0E */ s16 field_0x0E;
    /* 0x10 */ u8 pad_1[0x18];
    /* 0x28 */ ObjectColorCommandData field_0x28;
} ObjectColorCommandTask;

extern void func_1502EA98(struct127*, u8, u8, u8, u8, s32, u8);

void func_150ECB8C(ObjectColorCommandTask *arg0) {
    ObjectColorCommandData* p = &arg0->field_0x28;
    struct127* ptr = p->field_0x00;

    if ((ptr->interaction_state == 0) || (ptr->unique_id != p->field_0x04)) {
        arg0->field_0x0E = -1;
    } else {
        func_1502EA98(ptr, p->field_0x05, p->field_0x06, p->field_0x07, p->field_0x08, 0, p->field_0x09);
    }
}

extern void func_151C9AC0(s32, u8, s32);
extern void func_150ECA68(void *, u8, u8, u8, u8, u8, s16, u8, s32);

void func_150ECC00(s32 arg0, u8 arg1, s32 arg2) {
    func_151C9AC0(arg0, arg1, arg2);
    func_150ECA68(arg0, 0, 0xFF, 0, 0xFF, 4, -1, arg1, arg2);
}

typedef struct Game1199D0Camera {
    u8 pad00[0x84];
    s32 flags;
    u8 pad88[0xAC];
    s32 field134;
    u8 pad138[0x58];
    f32 field190;
    u8 pad194[0x620];
    f32 delta;
} Game1199D0Camera;

void func_150495B0(f32 *, f32, f32 *, f32, f32, f32);
s32 func_1509BE40(s32, s32, s32, ...);
extern f32 D_80088AD0;
extern f32 D_80088AD4;
extern f32 D_80088AD8;
extern f32 D_80088ADC;
extern f32 D_80088AE0;
extern f32 D_80088AE4;
extern f32 D_80088AE8;
extern f32 D_80088AEC;
extern f32 D_80088AF0;

void func_150ECC70(Game1199D0Camera *arg0) {
    s32 value;
    s32 width;
    s32 height;

    if (func_1509BE40(1, 0x403B, 6, 0x2000) != 0) {
        s32 mode;

        func_1509BFB0(0, 0x4031, 1);
        func_1509BFB0(0, 0x4032, 1);
        func_1509BFB0(0, 0x4033, 1);
        arg0->flags = (s32) (arg0->flags | 0x80021200);
        if (func_1509BE40(1, 0x4038, 6, 0x2000) != 0) {
            D_80088ADC = 128.0f;
            D_80088AE0 = 255.0f;
            D_80088AE4 = 255.0f;
        } else if (func_1509BE40(1, 0x4039, 6, 0x2000) != 0) {
            D_80088ADC = 255.0f;
            D_80088AE0 = 128.0f;
            D_80088AE4 = 255.0f;
        } else if (func_1509BE40(1, 0x403A, 6, 0x2000) != 0) {
            D_80088ADC = 255.0f;
            D_80088AE0 = 255.0f;
            D_80088AE4 = 128.0f;
        } else {
            D_80088ADC = 255.0f;
            D_80088AE0 = 255.0f;
            D_80088AE4 = 255.0f;
        }
        mode = func_1509BE40(0, 0x2007, 0xB7) | 0x2000;
        if (func_1509BE40(1, 0x4010, 6, 0x2000) == 0) {
            arg0->field134 = 0;
            arg0->field190 = 0.0f;
            return;
        }
        if (mode == 0) {
            return;
        }
        arg0->field134 = 1;
        value = func_1509BE40(1, mode, 0x9C, 0x2000);
        func_1509BFB0(1, 0x9000, 0x17, value);
        width = func_1509BE40(1, mode, 0x9A, 0x2000);
        height = width >> 1;
        if (width < 900) {
            width = 900;
        } else if (width > 1190) {
            width = 1190;
        }
        if (height < 180) {
            height = 180;
        } else if (height > 231) {
            height = 231;
        }
        func_1509BFB0(2, 0x9000, 8, width, height);
        arg0->field190 = 220.0f;
    } else {
        D_80088ADC = 255.0f;
        D_80088AE0 = 255.0f;
        D_80088AE4 = 255.0f;
        func_1509BFB0(0, 0x4031, 0);
        func_1509BFB0(0, 0x4032, 0);
        func_1509BFB0(0, 0x4033, 0);
        arg0->flags &= 0x7FFDEDFF;
        arg0->flags |= 8;
    }
    func_150495B0(&D_80088AD0, D_80088ADC, &D_80088AE8, 4.0f, 6.0f, arg0->delta);
    func_150495B0(&D_80088AD4, D_80088AE0, &D_80088AEC, 4.0f, 6.0f, arg0->delta);
    func_150495B0(&D_80088AD8, D_80088AE4, &D_80088AF0, 4.0f, 6.0f, arg0->delta);
    func_1509BFB0(1, 0x30F3, 0x12, (u32) D_80088AD0 & 0xFF);
    func_1509BFB0(1, 0x30F4, 0x12, (u32) D_80088AD4 & 0xFF);
    func_1509BFB0(1, 0x30F5, 0x12, (u32) D_80088AD8 & 0xFF);
}
