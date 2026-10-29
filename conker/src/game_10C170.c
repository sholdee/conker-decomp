#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_10C170/func_150DECC0.s")

typedef struct {
    u8 pad0[0x24];
} Floor150DF334;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    struct17 unk28;
} Actor150DF334;

s32 func_1504697C(struct17 *, s32, f32, Floor150DF334 *);
void func_150E83AC(struct17 *, s16, u8, s32);
void func_151A9834(struct17 *, f32, f32, Floor150DF334 *, s32, s32, s32, s32, s32);

void func_150DF334(Actor150DF334 *arg0) {
    struct17 *pos;
    Floor150DF334 floor;
    u8 onGround;
    struct17 sp60;
    struct17 sp54;
    struct17 sp48;

    pos = &arg0->unk28;
    onGround = 0;
    sp54.unk0 = arg0->unk28.unk0;
    sp54.unk4 = arg0->unk28.unk4 + 500.0f;
    sp54.unk8 = arg0->unk28.unk8;
    if (func_1504697C(&sp54, 0, sp54.unk4 - 1000.0f, &floor) != 0) {
        sp60.unk0 = pos->unk0;
        sp60.unk4 = *(f32 *)&floor;
        sp60.unk8 = pos->unk8;
        onGround = 1;
    }
    func_150E83AC(onGround ? &sp60 : pos, (s16)(func_150ADA20() % 62U + 120), arg0->unkC, arg0->unk1);
    pos = onGround ? &sp60 : &arg0->unk28;
    sp48 = *pos;
    sp48.unk4 += 500.0f;
    func_151A9834(&sp48, sp48.unk4 - 1000.0f, 250.0f, &floor, func_150ADA20() % 3U + 3, 1, 0, arg0->unkC, arg0->unk1);
}

typedef struct {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u16 unk2A;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s32 unk30;
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
} Struct15149550Arg;

s32 func_15149550(Struct15149550Arg *arg0, u8 arg1, u8 arg2, s32 arg3, u8 arg4, s32 arg5);
extern void func_15143874(s16, f32, f32 *, f32 *);

void func_150DF4B8(struct17 *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4) {
    Struct15149550Arg sp88;
    f32 t;
    f32 s;
    s32 count;

    sp88.unk0 = *arg0;
    t = (func_150ADA68() * 0.5f) + 1.0f;
    s = (func_150ADA68() * 0.5f) + 1.0f;
    sp88.unkC = 22.6f * t;
    sp88.unk14 = 13.786f * t;
    sp88.unk20 = 0x6231; sp88.unk22 = 0x1A4D;
    sp88.unk10 = 57.969f * s;
    sp88.unk18 = 42.714f * s;
    sp88.unk1C = 118.763f * s;
    sp88.unk24 = (func_150ADA20() % 151U) + 150;
    sp88.unk26 = 0;
    sp88.unk27 = 0;
    sp88.unk28 = 0;
    sp88.unk29 = 0xFF;
    sp88.unk2A = 1;
    sp88.unk2C = (func_150ADA20() % 156U) + 100;
    sp88.unk2D = 0xFF;
    sp88.unk2E = 0;
    sp88.unk2F = 1;
    sp88.unk30 = 0;
    sp88.unk34 = 0xF;
    sp88.unk38 = 0.0f;
    sp88.unk3C = 10000.0f;
    sp88.unk40 = 0.0450000018f;
    sp88.unk44 = 0.699999988f;
    func_15149550(&sp88, 0xA, 0, 0, arg3, arg4);
    count = (func_150ADA20() % 3U) + 1;
    while (count > 0) {
        func_15143874(func_150ADA20() & 0xFF, 30.0f, &sp88.unk0.unk0, &sp88.unk0.unk8);
        sp88.unk0.unk0 += arg0->unk0;
        sp88.unk0.unk8 += arg0->unk8;
        t = (func_150ADA68() * 0.5f) + 0.75f;
        s = (func_150ADA68() * 0.5f) + 0.75f;
        sp88.unkC = 22.6f * t;
        sp88.unk14 = 13.786f * t;
        sp88.unk10 = 57.969f * s;
        sp88.unk18 = 42.714f * s;
        sp88.unk1C = 118.763f * s;
        sp88.unk24 = (func_150ADA20() % 151U) + 150;
        sp88.unk2C = (func_150ADA20() % 156U) + 100;
        sp88.unk2F = 0;
        func_15149550(&sp88, 0xA, 0, 0, arg3, arg4);
        count--;
    }
}
