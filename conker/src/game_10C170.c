#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_10C170/func_150DECC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_10C170/func_150DF334.s")

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
