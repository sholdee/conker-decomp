#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_188440/func_1515AF90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_188440/func_1515B21C.s")

extern void func_1515572C(s16 *, s32);

void func_1515B5F4(s16 arg0) {
    s16 sp1C[2];

    sp1C[0] = arg0;
    func_1515572C(sp1C, 0xB);
}

void func_1515B62C(struct102 *arg0, s16 *arg1, u8 arg2) {
    if (arg2 == 0xB) {
        if (*(s16 *)((u8 *)arg0 + 0x70) == arg1[0]) {
            func_1516972C(arg0);
        }
    }
}

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[0x17];
} struct_1515B674;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8  pad14[8];
} struct_1515B674b;

extern u8 (*D_8008FD04[])(void);
extern s32 func_151D8E20(void);
extern void *func_1515548C(struct_1515B674 *, s32, s32, s32, s32, u8, s32);

void func_1515B674(s16 arg0) {
    struct_1515B674 sp9C;
    struct_1515B674b sp80;
    void *temp_v0;
    s16 i;

    i = (func_150ADA20() % 1U) + 10;
    sp9C.unk14 = 0x19;
    sp9C.unk1A = 1;
    sp9C.unk16 = 0x14;
    sp9C.unk18 = 0xC;
    sp9C.unk1B = 0xFF;
    sp9C.unk1C = 0xFF;
    sp9C.unk1D = 0xFF;
    sp9C.unk1F = 0xFF;
    sp80.unk0 = arg0;
    sp9C.unk44 = arg0;
    sp9C.unk20 = 0xFF;
    sp9C.unk21 = 0xFF;
    sp9C.unk22 = 0xFF;
    sp9C.unk23 = 0xFF;
    sp9C.unk24 = 0;
    sp9C.unk28 = 0;
    sp9C.unk2C = 0;
    sp9C.unk40 = 0;
    sp9C.unk41 = 9;
    sp9C.unk30 = 7;
    sp9C.unk34 = 0x3C;
    sp9C.unk38 = 0x80;
    sp9C.unk3C = 0x20;
    while (i--) {
        sp80.unk4 = ((func_150ADA68() * 105.0f) + 105.0f) * 0.001000000047f;
        sp80.unk8 = func_150ADA68() * 3008.0f * 0.001000000047f;
        sp9C.unkC = sp9C.unk8 = sp80.unkC = ((func_150ADA68() * 1579.0f) + 489.0f) * 0.009999999776f;
        sp80.unk10 = ((func_150ADA68() * 301.0f) + 94.0f) * 0.009999999776f;
        sp9C.unk0 = (func_150ADA68() * 200.0f) + -100.0f;
        sp9C.unk4 = (func_150ADA68() * 148.0f) + -100.0f;
        sp9C.unk10 = D_8008FD04[func_151D8E20()]();
        sp9C.unk12 = (func_150ADA20() % 51U) + 0x32;
        sp9C.unk1E = (func_150ADA20() % 101U) + 0x64;
        if (func_150ADA20() & 1) {
            sp9C.unk14 |= 2;
        }
        if (func_150ADA20() & 1) {
            sp9C.unk14 |= 4;
        }
        temp_v0 = func_1515548C(&sp9C, 2, 0, 0, 0x18, 0xFF, 1);
        if (temp_v0 != NULL) {
            memcpy((u8 *)temp_v0 + 0x70, &sp80, 0x18);
        }
    }
}

typedef struct Obj1515B994 {
    char pad0[0x14];
    /* 0x14 */ f32 unk14;
    char pad18[4];
    /* 0x1C */ f32 unk1C;
    char pad20[0x74 - 0x20];
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ f32 unk80;
} Obj1515B994;

s32 func_1515B994(void *arg0) {
    typedef struct {
        u8 pad0[0x14];
        f32 x;
        u8 pad18[4];
        f32 z;
        u8 pad20[0x54];
        f32 acceleration;
        f32 velocity;
        f32 base;
        f32 scale;
    } Motion;
    Motion *motion = arg0;
    f32 previous;

    previous = motion->velocity;
    motion->x += previous * D_800BE9A4 + 0.5f * motion->acceleration * D_800BE9A4;
    motion->velocity = motion->velocity + motion->acceleration * D_800BE9A4;
    motion->z = motion->base + motion->scale * (motion->velocity + previous) * 0.5f;
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_188440/func_1515BA10.s")

extern void func_1515AF90(s16);

void func_1515BA1C(s16 arg0) {
    func_1515AF90(arg0);
}

void func_1515BA48(s32 arg0) {
}

extern void func_1515B674(s16);

void func_1515BA54(s16 arg0) {
    func_1515B674(arg0);
}

extern void func_1515B5F4(s16);

void func_1515BA80(s16 arg0) {
    func_1515B5F4(arg0);
}

void func_1515BAAC(s16 arg0) {
    func_1515B5F4(arg0);
}
