#include <ultra64.h>

#include "functions.h"
#include "variables.h"

#if 0
f32 D_800964C0 = 175.5;
f32 D_800964C4 = 115.4000015258789;
f32 D_800964C8 = 1.0420000553131104;
f32 D_800964CC = 0.9010000228881836;
f32 D_800964D0 = 1.2009999752044678;
f32 D_800964D4 = 0.7023000121116638;
f32 D_800964D8 = 2337.0;
f32 D_800964DC = 2220.0;
f32 D_800964E0 = 0.4099999964237213;
f32 D_800964E4 = 0.20000000298023224;
f32 D_800964E8 = 0.3499999940395355;
f32 D_800964EC = 0.3499999940395355;
f32 D_800964F0 = 9.999999974752427e-07;
f32 D_800964F4 = 2337.0;
f32 D_800964F8 = 2220.0;
f32 D_800964FC = 926.0;
f32 D_80096500 = -710.0;
f32 D_80096504 = -339.0;
// dont think these belong here?
f32 D_80096508 = 15.045271873474121;
f32 D_8009650C = 5.8676557540893555;
#endif

// typedef struct {
//     f32 sp38;
//     f32 sp3C;
//     f32 sp40;
//     f32 sp44;
//     f32 sp48;
//     f32 sp4C;
//     f32 sp50;
//     s8  sp54;
//     s8  pad55[0x1];
//     s16 sp56;
//     s16 sp58;
//     u8  pad5A[0x2];
//     s32 sp5C;
//     s32 sp60;
//     s8  sp64;
//     s8  sp65;
//     s8  sp66;
//     s8  sp67;
//     f32 sp68;
//     f32 sp6C;
//     f32 sp70;
//     f32 sp74;
//     f32 sp78;
//     f32 sp7C;
//     f32 sp80;
//     f32 sp84;
//     f32 sp88;
//     f32 sp8C;
//     f32 sp90;
//     s32 sp94;
//     s8  sp98;
//     s8  sp99;
//     s8  sp9A;
//     s8  sp9B;
//     f32 spAC;
//     f32 spB0;
//     f32 spB4;
//     f32 spB8;
//     f32 spBC;
//     f32 spC0;
//     f32 spC4;
//     f32 spC8;
//     f32 spCC;
//     u8  padD0[0x24];
//     f32 spF4;
//     u8 padF8[0x8];
//     s8 sp100;
//     s8 sp101;
//     s8 sp102;
//     s8 sp103;
//     s8 sp104;
// } structaaa;

/*FUNCSTART*/
typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
} Payload_150113E0;

typedef struct {
    u8 unk00;
    u8 pad01;
    s16 unk02;
    s16 unk04;
    u8 pad06[2];
    s32 unk08;
    s32 unk0C;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
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
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 pad48[0x10];
} Struct54_150113E0;

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
    u8 pad24[0x24];
    f32 unk48;
    u8 pad4C[0x8];
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
} StructAC_150113E0;

void *func_151407D0(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32);

void func_150113E0(void) {
    StructAC_150113E0 spAC;
    Struct54_150113E0 sp54;
    Payload_150113E0 sp38;
    struct181 *temp_v0;

    sp38.unk00 = 1.0420000553f;
    sp38.unk04 = 0.9010000229f;
    sp38.unk08 = 1.2009999752f;
    sp38.unk0C = 1.0f;
    sp38.unk10 = 0.0f;
    sp38.unk14 = 7.0f;
    sp38.unk18 = 0.7023000121f;

    sp54.unk00 = 3;
    sp54.unk02 = 0x3403;
    sp54.unk04 = 0x12C;
    sp54.unk08 = 0;
    sp54.unk0C = 0;
    sp54.unk10 = 0xE6;
    sp54.unk11 = 0x64;
    sp54.unk12 = 0;
    sp54.unk13 = 0xFF;
    sp54.unk14 = 100.0f;
    sp54.unk18 = 100.0f;
    sp54.unk1C = 520.0f;
    sp54.unk20 = 72.0f;
    sp54.unk24 = 2337.0f;
    sp54.unk28 = 520.0f;
    sp54.unk2C = 72.0f;
    sp54.unk30 = 2220.0f;
    sp54.unk34 = 1.0f;
    sp54.unk38 = 1.0f;
    sp54.unk3C = 1.0f;
    sp54.unk40 = 0xD2012;
    sp54.unk45 = 0xFF;
    sp54.unk44 = 0xFF;
    sp54.unk46 = 0;
    sp54.unk47 = 6;

    spAC.unk04 = 175.5f;
    spAC.unk00 = 175.5f;
    spAC.unk0C = 115.4000015259f;
    spAC.unk08 = 115.4000015259f;
    spAC.unk10 = 0.4099999964f;
    spAC.unk14 = 0.2000000030f;
    spAC.unk18 = 0.3499999940f;
    spAC.unk1C = 0.3499999940f;
    spAC.unk20 = 9.999999975e-07f;
    spAC.unk54 = 0;
    spAC.unk55 = 0;
    spAC.unk56 = 0;
    spAC.unk57 = 0;
    spAC.unk48 = 1.0f;
    spAC.unk58 = 0;

    temp_v0 = func_151407D0(&spAC, 0x7C, &sp54, 0, 0x20, 0, 0, -1, 0xFF, 1);
    if (temp_v0 != 0) {
        memcpy(&temp_v0->unk170, &sp38, 0x1C);
    }

    sp54.unk1C = -520.0f;
    sp54.unk28 = -520.0f;
    sp54.unk20 = 72.0f;
    sp54.unk2C = 72.0f;
    sp54.unk24 = 2337.0f;
    sp54.unk30 = 2220.0f;
    temp_v0 = func_151407D0(&spAC, 0x7C, &sp54, 0, 0x20, 0, 0, -1, 0xFF, 1);
    if (temp_v0 != 0) {
        memcpy(&temp_v0->unk170, &sp38, 0x1C);
    }

    sp54.unk20 = 70.0f;
    sp54.unk2C = 70.0f;
    sp54.unk24 = 926.0f;
    sp54.unk30 = 926.0f;
    sp54.unk1C = -710.0f;
    sp54.unk28 = -584.0f;
    temp_v0 = func_151407D0(&spAC, 0x7C, &sp54, 0, 0x20, 0, 0, -1, 0xFF, 1);
    if (temp_v0 != 0) {
        memcpy(&temp_v0->unk170, &sp38, 0x1C);
    }

    sp54.unk1C = 506.0f;
    sp54.unk28 = 506.0f;
    sp54.unk20 = 47.0f;
    sp54.unk2C = 47.0f;
    sp54.unk24 = -339.0f;
    sp54.unk30 = -208.0f;
    temp_v0 = func_151407D0(&spAC, 0x7C, &sp54, 0, 0x20, 0, 0, -1, 0xFF, 1);
    if (temp_v0 != 0) {
        memcpy(&temp_v0->unk170, &sp38, 0x1C);
    }
}
/*FUNCEND*/
// void func_150113E0(void) {
//     struct181 *temp_v0;
//     structaaa tmp;
//
//     f32 temp_f16 = D_800964C0;
//     f32 temp_f18 = D_800964C4;
//
//     tmp.sp38 = D_800964C8;
//     tmp.sp3C = D_800964CC;
//     tmp.sp40 = D_800964D0;
//     tmp.sp48 = 0.0f;
//     tmp.sp4C = 7.0f;
//     tmp.sp50 = D_800964D4;
//     tmp.sp78 = D_800964D8;
//     tmp.sp84 = D_800964DC;
//     tmp.spBC = D_800964E0;
//     tmp.sp54 = 3;
//     tmp.sp56 = 0x3403;
//     tmp.sp58 = 0x12C;
//     tmp.sp64 = 0xE6;
//     tmp.sp44 = 1.0f;
//     tmp.sp5C = 0;
//     tmp.sp60 = 0;
//     tmp.sp65 = 0x64;
//     tmp.sp66 = 0;
//     tmp.sp67 = 0xFF;
//     tmp.sp68 = 100.0f;
//     tmp.sp6C = 100.0f;
//     tmp.sp70 = 520.0f;
//     tmp.sp74 = 72.0f;
//     tmp.sp7C = 520.0f;
//     tmp.sp80 = 72.0f;
//     tmp.sp88 = 1.0f;
//     tmp.sp8C = 1.0f;
//     tmp.sp90 = 1.0f;
//     tmp.sp94 = 0xD2012;
//     tmp.sp98 = 0xFF;
//     tmp.sp99 = 0xFF;
//     tmp.sp9A = 0;
//     tmp.sp9B = 6;
//     tmp.spB0 = tmp.spAC = temp_f16;
//     tmp.spB8 = tmp.spB4 = temp_f18;
//     tmp.sp100 = 0;
//     tmp.sp101 = 0;
//     tmp.sp102 = 0;
//     tmp.sp103 = 0;
//     tmp.sp104 = 0;
//     tmp.spF4 = 1.0f;
//     tmp.spC0 = D_800964E4;
//     tmp.spC4 = D_800964E8;
//     tmp.spC8 = D_800964EC;
//     tmp.spCC = D_800964F0;
//
//     temp_v0 = func_151407D0(520.0f, 72.0f, &tmp.spAC, 0x7C, &tmp.sp54, 0, 0x20, 0, 0, -1, 0xFF, 1);
//     if (temp_v0 != 0) {
//         memcpy(&temp_v0->unk170, &tmp, 12);
//     }
//     tmp.sp70 = -520.0f;
//     tmp.sp7C = -520.0f;
//     tmp.sp74 = 72.0f;
//     tmp.sp80 = 72.0f;
//     tmp.sp78 = D_800964F4;
//     tmp.sp84 = D_800964F8;
//     temp_v0 = func_151407D0(&tmp.spAC, 0x7C, &tmp.sp54, 0, 0x20, 0, 0, -1, 0xFF, 1);
//     if (temp_v0 != 0) {
//         memcpy(&temp_v0->unk170, &tmp, 12);
//     }
//
//     tmp.sp74 = 70.0f;
//     tmp.sp80 = 70.0f;
//     tmp.sp78 = D_800964FC;
//     tmp.sp84 = D_800964FC;
//     tmp.sp7C = -584.0f;
//     temp_v0 = func_151407D0(&tmp.spAC, 0x7C, &tmp.sp54, 0, 0x20, 0, 0, -1, 0xFF, 1);
//     if (temp_v0 != 0) {
//         memcpy(&temp_v0->unk170, &tmp, 12);
//     }
//     tmp.sp70 = 506.0f;
//     tmp.sp7C = 506.0f;
//     tmp.sp74 = 47.0f;
//     tmp.sp80 = 47.0f;
//     tmp.sp78 = D_80096504;
//     tmp.sp84 = -208.0f;
//     temp_v0 = func_151407D0(&tmp.spAC, 0x7C, &tmp.sp54, 0, 0x20, 0, 0, -1, 0xFF, 1);
//     if (temp_v0 != 0) {
//         memcpy(&temp_v0->unk170, &tmp, 12);
//     }
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_3E890/func_1501175C.s")

void func_15011A78(struct127 *arg0, u8 arg1, s32 arg2) {
    struct182 tmp;
    struct37 *temp_v0;

    tmp.unk0 = arg0;
    tmp.unk4 = arg0->unique_id;
    tmp.unk6 = 0;

    temp_v0 = func_15149130(300, -1, 85, -1, 0, 66, 8, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk28, &tmp, 8);
    }
}
