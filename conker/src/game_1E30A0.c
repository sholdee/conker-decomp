#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    char pad_0[0x88];
    s32 field_0x88;
} Game1E30A0Data;


extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void D_1500310C(void);
void func_15002FB4(s32 arg0);

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ struct17 unk10;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ struct17 unk28;
    /* 0x34 */ struct17 unk34;
    /* 0x40 */ struct17 unk40;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8  unk58;
    /* 0x59 */ u8  pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8  unk60;
    /* 0x61 */ u8  unk61;
    /* 0x62 */ u8  unk62;
    /* 0x63 */ u8  unk63;
    /* 0x64 */ u8  unk64;
    /* 0x65 */ u8  unk65;
    /* 0x66 */ u8  unk66;
    /* 0x67 */ u8  unk67;
    /* 0x68 */ u8  unk68;
    /* 0x69 */ u8  pad69;
    /* 0x6A */ u8  unk6A;
    /* 0x6B */ u8  pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8  unk70;
    /* 0x71 */ u8  pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x78 */ s32 pad78;
} S151B5BF0;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
} S151B5BF0Payload;

s32 func_151B5BF0(struct17 *arg0, struct17 *arg1, f32 arg2, struct17 *arg3,
                  struct17 *arg4, f32 arg5, s16 arg6, u8 arg7, s32 arg8,
                  f32 arg9, u8 arg10, u8 arg11, u8 arg12, s32 arg13) {
    void *ret;
    S151B5BF0 sp48;
    S151B5BF0Payload sp40;

    sp40.unk4 = arg2;
    sp40.unk0 = 0.0f;
    sp48.unk0 = 24.70000076f * arg2;
    sp48.unk4 = arg9;
    sp48.unkC = arg2;
    sp48.unk8 = arg2;
    sp48.unk10 = *arg3;
    sp48.unk1C = 1.0f;
    sp48.unk20 = 1.0f;
    sp48.unk24 = 1.0f;
    sp48.unk28 = *arg0;
    sp48.unk4C = arg5;
    sp48.unk50 = 0xB908;
    if (arg6 == -1) {
        sp48.unk54 = 0x12C;
    } else {
        sp48.unk54 = arg6 + (0x100 >> arg10);
        sp48.unk50 = 0xB988;
    }
    if (arg1 != NULL) {
        sp48.unk34 = *arg1;
        sp48.unk50 |= 0x20;
    } else {
        sp48.unk34.unk0 = 0.0f;
        sp48.unk34.unk4 = 0.0f;
        sp48.unk34.unk8 = 0.0f;
    }
    {
        u32 *start;
        u32 *end;
        u32 *p;
        s32 sum;
        sum = 0;
        p = (u32 *)func_15002FB4;
        end = (u32 *)D_1500310C;
        for (; p < end; p++) {
            sum += *p;
            sum <<= 1;
        }
        if (sum != 0x80D2D760) {
            D_8008FDA8 = -1;
        }
    }
    if (arg4 != NULL) {
        sp48.unk40 = *arg4;
        sp48.unk50 |= 0x40;
    } else {
        sp48.unk40.unk0 = 0.0f;
        sp48.unk40.unk4 = 0.0f;
        sp48.unk40.unk8 = 0.0f;
    }
    if (arg11 != 0) {
        sp48.unk50 |= 1;
    }
    sp48.unk56 = 2;
    sp48.unk58 = 0;
    sp48.unk5C = 0;
    sp48.unk60 = arg7;
    sp48.unk61 = 5;
    sp48.unk62 = 0;
    sp48.unk63 = 6;
    sp48.unk64 = 0;
    sp48.unk65 = 0;
    sp48.unk66 = 2;
    sp48.unk67 = 0;
    sp48.unk68 = 2;
    sp48.unk6A = 0;
    sp48.unk6C = 0;
    sp48.unk70 = 0;
    sp48.unk72 = (0x100 >> arg10);
    sp48.unk74 = 0xFF / (0x100 >> arg10);
    ret = func_15132A4C(&sp48, 3, 0xFF, 8, arg12, arg13);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0x170, &sp40, 8);
    }
    return (s32)ret;
}

void func_151B5E8C(void) {
}

typedef struct Sub151B5E94 {
    f32 unk0;
    f32 unk4;
} Sub151B5E94;

typedef struct Obj151B5E94 {
    u8 pad0[0x18];
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x18];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0x12C];
    Sub151B5E94 unk170;
} Obj151B5E94;

extern f32 D_800AA434;
extern f32 D_800AA438;
extern f32 D_800AA43C;
extern f32 D_800AA440;
extern f32 sinf(f32);
extern void func_15133894(Obj151B5E94 *);

void func_151B5E94(Obj151B5E94 *arg0) {
    f32 temp;
    Sub151B5E94 *p;

    p = &arg0->unk170;
    p->unk0 += D_800AA434 * D_800BE9A4;
    if (p->unk0 > D_800AA438) {
        p->unk0 = 0.0f;
        func_10010F88(0x502, 0x5DC0, 0, 0, -1, arg0->unk38, arg0->unk3C, arg0->unk40, 100, 500);
    }
    if (p->unk0 < D_800AA43C) {
        f32 s = sinf(p->unk0);

        temp = p->unk4 + s * (D_800AA440 * p->unk4);
        arg0->unk1C = temp;
        arg0->unk18 = temp;
    } else {
        temp = p->unk4;
        arg0->unk1C = temp;
        arg0->unk18 = temp;
    }
    func_15133894(arg0);
}

void func_151B5FCC(Game1E30A0Data *arg0) {
    if (arg0->field_0x88 != 0) {
        func_100111C8(arg0->field_0x88);
        arg0->field_0x88 = 0;
    }
}
