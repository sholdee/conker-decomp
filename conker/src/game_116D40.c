#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_116D40/func_150E9890.s")


void func_1514470C(void *, f32 *);
void func_15143794(s16, s16, f32, f32 *);
void func_150E9890(f32 *, f32 *, f32, u8, s32);

typedef struct {
    void *unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} GameStruct150E9E34Sub;

typedef struct {
    u8 pad0[1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    GameStruct150E9E34Sub unk28;
} GameStruct150E9E34;

void func_150E9E34(GameStruct150E9E34 *arg0) {
    GameStruct150E9E34Sub *sub;

    f32 sp80[3];
    f32 sp74[3];
    f32 temp_f20;
    s32 temp_s1;
    s32 temp_s2;

    sub = &arg0->unk28;
    sub->unk14 += (sub->unk4 + (func_150ADA68() * sub->unk8)) * D_800BE9A4;
    if (sub->unk14 > 1.0f) {
        
        do {
            temp_f20 = func_150ADA68() * sub->unk10 + sub->unkC;
            func_1514470C(sub->unk0, sp80);
            temp_s1 = func_150ADA20();
            temp_s2 = func_150ADA20();
            func_15143794((s16)(temp_s1 & 0xFF), (s16)(0x40 - (temp_s2 % 129U)),
                          func_150ADA68() * 600.0f * 0.009999999776f, sp74);
            func_150E9890(sp80, sp74, temp_f20, arg0->unkC, arg0->unk1);
            sub->unk14 -= 1.0f;
        } while (sub->unk14 > 1.0f);
    }
}

