#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
void func_1502178C(struct127 *arg0, s32 arg1, s32 arg2);
s32 func_150229E4(struct127 *arg0);
s32 func_150535F4(struct127 *arg0);
void func_15053750(struct127 *arg0);
void func_15054F74(struct127 *arg0);
void func_150562FC(struct127 *arg0);
void func_15056B08(struct127 *arg0);
void func_150585F0(struct127 *arg0, f32 arg1);
void func_150597FC(struct127 *arg0);
void func_150AFC68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5);
extern f32 D_80099330;
extern f32 D_80099334;
extern f32 D_80099338;
extern f32 D_80099344;
// NON-MATCHING: work-in-progress...
// f32 func_1504A620(f32 arg0) {
//     f32 temp_f14;
//     f32 temp_f2;
//     f32 phi_f2;
//     f32 phi_f0;
//     s32 phi_v0;
//     f32 tmp0;
//
//     if (arg0 < 0.0f) {
//         return 0.0f;
//     }
//     if (arg0 == 0.0f) {
//         return 0.0f;
//     }
//
//     phi_f2 = 0.0f;
//
//     while (arg0 >= 2.0f ) {
//         arg0 = arg0 * 0.5f;
//         phi_f2 += D_800990B0;
//     }
//
//     tmp0 = D_800990B4;
//     while (arg0 < 1.0f) {
//         arg0 = arg0 * 2.0f;
//         phi_f2 -= tmp0;
//     }
//
//     temp_f14 = (phi_f2 - 1.0f) / (phi_f2 + 1.0f);
//     phi_f0 = 2.0f * temp_f14;
//     phi_v0 = 1;
//
//     do {
//         temp_f2 = phi_f2 + (phi_f0 / (f32) phi_v0);
//         phi_f2 = temp_f2;
//         phi_f0 = phi_f0 * (temp_f14 * temp_f14);
//         phi_v0 = phi_v0 + 2;
//     }
//     while (temp_f2 != phi_f2);
//
//     return temp_f2;
// }

typedef struct {
    /* 0x000 */ s32 state;
    /* 0x004 */ u8 pad4[0xE];
    /* 0x012 */ u16 unk12;
    /* 0x014 */ f32 x;
    /* 0x018 */ f32 y;
    /* 0x01C */ f32 z;
    /* 0x020 */ u8 pad20[0xC];
    /* 0x02C */ f32 oldx;
    /* 0x030 */ f32 oldy;
    /* 0x034 */ f32 oldz;
    /* 0x038 */ s16 unk38;
    /* 0x03A */ u8 pad3A[0xE];
    /* 0x048 */ f32 unk48;
    /* 0x04C */ u8 pad4C[0xA8];
    /* 0x0F4 */ s32 unkF4;
    /* 0x0F8 */ s32 unkF8;
    /* 0x0FC */ u8 padFC[0x2B];
    /* 0x127 */ u8 unk127;
    /* 0x128 */ u8 pad128[0xF];
    /* 0x137 */ u8 unk137;
    /* 0x138 */ u8 pad138[0x4C];
    /* 0x184 */ u32 unk184;
    /* 0x188 */ u8 pad188[0x42];
    /* 0x1CA */ u8 health;
    /* 0x1CB */ u8 pad1CB[0x91];
    /* 0x25C */ s32 unk25C;
    /* 0x260 */ u8 pad260[0x14];
    /* 0x274 */ u8 unk274;
    /* 0x275 */ u8 pad275[0x85];
    /* 0x2FA */ u8 unk2FA;
    /* 0x2FB */ u8 pad2FB[4];
    /* 0x2FF */ u8 unk2FF;
    /* 0x300 */ u8 unk300;
    /* 0x301 */ u8 pad301[0x17];
    /* 0x318 */ struct108 *camera;
    /* 0x31C */ u8 *unk31C;
    /* 0x320 */ u8 pad320[0xC];
} Obj1504A730;

typedef struct {
    u8 pad0[0x4F];
    u8 unk4F;
    u8 pad50[0x50];
} Struct131u;

extern u16 D_800CBDA0[];
extern s32 *D_800DBF94;
void func_1508295C(s32, s32, s32);
void func_1507D4F8(s32);
s32 func_1504C8BC(void *);
void func_150A11C4(void *);
void func_150A278C(void *);
void func_1507E73C(void *);
void func_1504AF10(struct127 *, s32, s32);
void func_15061B4C(void);
void func_15080C64(void);
void func_1504A730(void);

// void func_1504A730(void) {
//     u8 sp5C[6];
//     f32 temp_f0;
//     f32 temp_f0_2;
//     f32 temp_f0_3;
//     s32 temp_t0;
//     s32 temp_v0;
//     s8 temp_s2;
//     s8 temp_v1;
//     u16 temp_a3;
//     u16 temp_t8;
//     u8 temp_v0_5;
//     u8 temp_v0_7;
//     u8 temp_v0_9;
//     void *temp_a0;
//     void *temp_a2;
//     struct126 *temp_v0_2;
//     struct126 *temp_v0_3;
//     struct108 *temp_v0_4;
//     void *temp_v0_6;
//     void *temp_v0_8;
//     void *temp_v1_2;
//     struct127 *phi_s0;
//     s32 phi_s1;
//     f32 phi_f20;
//     f32 phi_f0;
//     s32 phi_s3;
//     s8 phi_s2;
//     void *phi_a1;
//     s32 phi_t0;
//     s32 phi_v1;
//
//     D_800BE9EC = (u8)0;
//     D_800D1548 += 1; //(s32) (D_800D1548 + 1);
//     D_800CC250 += D_800BE9A0; //(s32) (D_800CC250 + D_800BE9A0);
//     D_800C3E78 = (u8)0;
//     D_800CC2BC = 0;
//     func_1508295C(D_800BE9F0, 0, 0);
//     D_800CC2B8 ^= 1; //(u8) (D_800CC2B8 ^ 1);
//
//     // phi_s1 = (u8)0;
//     for (phi_s1 = 0; phi_s1 < 25; phi_s1++) {
//         phi_s0 = &D_800CC2D0[phi_s1];
//         if (phi_s0 == 0) {
//             phi_s2 = phi_s1 + 1;
//         } else {
//             phi_s0->unk274 = (u8)0;
//             phi_s0->unk300 = (u8)0;
//             phi_s0->unkF4 = (s32) (phi_s0->unkF4 & 0xFFEE5FFF);
//             D_800C3E78 = phi_s1;
//             if (phi_s0->y_position <= (f32) phi_s0->unk38) {
//                 if (phi_s0->unk127 == 0xFF) {
//                     func_15060F28(phi_s0, 0);
//                     phi_s2 = phi_s1 + 1;
//                 } else if (D_800BE616 == 0) {
//                     func_1507D4F8(phi_s1);
//                     phi_s2 = phi_s1 + 1;
//                 } else {
// block_8:
//                     if (phi_s0->unk2FA == 0) {
//                         phi_s2 = phi_s1 + 1;
//                     } else {
//                         D_800D154C = phi_s0;
//                         temp_s2 = phi_s1 + 1;
//                         phi_s0->unk12 += 1; // (u16) (phi_s0->unk12 + 1);
//                         D_800D35DC = temp_s2;
//                         D_800CC2BA = (u16)0;
//                         phi_s3 = 0;
//                         if ((D_800BE616 == 0) || (temp_v0 = func_1504C8BC(phi_s0), phi_s3 = temp_v0, phi_s2 = temp_s2, (temp_v0 != 0))) {
//                             if ((s32) phi_s1 < (s32) D_8008FD8C) {
//                                 temp_v0_2 = phi_s0->unk31C;
//                                 temp_v0_2->unk195 = (s8) (temp_v0_2->unk195 - D_800BE9E4);
//                                 temp_v0_3 = phi_s0->unk31C;
//                                 if ((s32) temp_v0_3->unk195 < 0) {
//                                     temp_v0_3->unk195 = (u8)0;
//                                 }
//                                 phi_s0->unk31C->unk57 = (u8)0;
//                                 if ((s32) phi_s1 < 4) {
//                                     D_800CC284 = (s32) D_800BE728[phi_s1]; // * 4));
//                                 } else {
//                                     bzero(&sp5C, 6);
//                                     D_800CC284 = &sp5C;
//                                 }
//                             }
//                             temp_v0_4 = phi_s0->camera;
//                             if (temp_v0_4 != 0) {
//                                 temp_f0 = temp_v0_4->unk384;
//                                 if (D_800991A8 == temp_f0) {
//                                     D_800CC280 = (s32) (temp_v0_4->unk37C * D_800991AC);
//                                 } else {
//                                     D_800CC280 = (s32) (temp_f0 * D_800991B0);
//                                 }
//                             } else {
//                                 D_800CC280 = 0;
//                             }
//                             if (phi_s0->unk127 != 0xFF) {
//                                 func_150A11C4(phi_s0);
//                             } else if (phi_s0->unkF8 & 0x400000) {
//                                 func_150A278C(phi_s0);
//                             }
//                             phi_s0->unk2C = (f32) phi_s0->unk14;
//                             phi_s0->unk30 = (f32) phi_s0->y_position;
//                             phi_s0->unk34 = (f32) phi_s0->unk1C;
//                             D_800CC27C = (s8) (((u32) phi_s0->unk184 >> 0x1C) & 7);
//                             temp_f0_2 = D_800BE9A4;
//                             phi_f20 = 2.0f;
//                             if (D_800BE616 != 0) {
//                                 phi_f20 = 2.0f;
//                                 if ((D_8008FDBC & 0x100) != 0) {
//                                     phi_f20 = 5.0f;
//                                 }
//                             }
//                             phi_f0 = temp_f0_2;
//                             if (phi_f20 < temp_f0_2) {
//                                 phi_f0 = phi_f20;
//                             }
//                             D_800D1550[0] = (f32) (phi_s0->unk48 * phi_f0);
//                             D_800CC264 = (s16) (s32) (D_800D1550[1364] * 100.0f); // wtf?
//                             if ((s32) D_800CC27C >= 6) {
//                                 D_800CC27C = (u8)0;
//                             }
//                             temp_v0_5 = (D_800B0DF0)->unk49;
//                             if (temp_v0_5 != 0) {
//                                 D_80086004[temp_v0_5](phi_s0);
//                             }
//                             if ((phi_s0->unk25C & 0x200) == 0) {
//                                 // temp_v0_6 = D_80086014[phi_s0->unk0];
//                                 if (D_80086014[phi_s0->unk0] != 0) {
//                                     D_80086014[phi_s0->unk0](phi_s0);
//                                 }
//                             }
//                             if (phi_s0->unk0 != 0) {
//                                 func_1507E73C(phi_s0);
//                                 func_1504AF10(phi_s0, 0, 0);
//                                 if (((s32) phi_s1 < (s32) D_8008FD8C) && (D_800D18A8 == 0)) {
//                                     func_15085710(phi_s0->unk127, 5, phi_s0->health);
//                                 }
//                             }
//                             phi_s0->unk2FF = (u8)1;
//                             if (phi_s3 == 2) {
//                                 temp_t8 = D_800CC2BA;
//                                 D_800BE9E4 = (s32) temp_t8;
//                                 D_800BE9A4 = (f32) ((f32) temp_t8 * 0.5f);
//                                 temp_f0_3 = D_800BE9A4;
//                                 if (temp_f0_3 != 0.0f) {
//                                     D_800BE9A8 = (f32) (1.0f / temp_f0_3);
//                                 } else {
//                                     D_800BE9A8 = 0.0f;
//                                 }
//                             }
//                             if ((phi_s0->unk14 != phi_s0->unk2C) || (phi_s0->unk18 != phi_s0->unk30) || (phi_s0->unk1C != phi_s0->unk34)) {
//                                 phi_s0->unk300 = (u8)1;
//                             }
//                             temp_v0_7 = (D_800B0DF0)->unk4A;
//                             phi_s2 = temp_s2;
//                             if (temp_v0_7 != 0) {
//                                 D_8008600C[temp_v0_7](phi_s0);
//                                 phi_s2 = temp_s2;
//                             }
//                         }
//                     }
//                 }
//             } else {
//                 goto block_8;
//             }
//         }
//     }
// //     temp_v1 = D_8008FD8C;
// //     if ((s32) temp_v1 > 0) {
// //         phi_a1 = &D_800CC2D0;
// //         phi_t0 = 0;
// //         phi_v1 = (s32) temp_v1;
// // loop_56:
// //         if (phi_a1->unk0 != 0) {
// //             temp_a0 = phi_a1->unk31C;
// //             if (temp_a0 != 0) {
// //                 temp_a2 = &D_800CBDA0 + (phi_t0 * 2);
// //                 if (phi_a1->unk137 != 0) {
// //                     temp_a3 = *temp_a2;
// //                     if (temp_a3 != 0) {
// //                         temp_v0_8 = D_800DBEF4 + (temp_a3 * 0xA0);
// //                         temp_v0_8->unk-51 = (u8) (temp_v0_8->unk-51 | 0x84);
// //                         temp_v1_2 = D_800DBF94 + (*temp_a2 * 4);
// //                         temp_v1_2->unk-4 = (s32) (temp_v1_2->unk-4 | (1 << phi_t0));
// //                     }
// //                     phi_a1->unk31C->unk56 = (u8)5;
// // block_64:
// //                     phi_v1 = (s32) D_8008FD8C;
// //                 } else {
// //                     temp_v0_9 = temp_a0->unk56;
// //                     if (temp_v0_9 != 0) {
// //                         temp_a0->unk56 = (u8) (temp_v0_9 - 1);
// //                         goto block_64;
// //                     }
// //                 }
// //             }
// //         }
// //         temp_t0 = phi_t0 + 1;
// //         phi_a1 = phi_a1 + 0x32C;
// //         phi_t0 = temp_t0;
// //         if (temp_t0 < phi_v1) {
// //             goto loop_56;
// //         }
// //     }
//     func_15061B4C();
//     func_15080C64();
//     D_800D154C = &D_800CC2D0;
//     D_800C3E78 = (u8)0;
//     D_800D35DC = (u8)0;
// }

void func_150ED578(struct127 *arg0);
void func_1504ADD0(void);
s32 func_1504AEF4(s32 arg0, s32 arg1);
void func_1504AF10(struct127 *arg0, s32 arg1, s32 arg2);

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ u8 unk20;
    /* 0x021 */ u8 unk21;
    /* 0x022 */ s8 unk22;
    /* 0x023 */ u8 pad23[0x45];
    /* 0x068 */ s16 unk68;
    /* 0x06A */ u8 pad6A[0x2B];
    /* 0x095 */ u8 unk95;
    /* 0x096 */ u8 pad96[0x8A];
    /* 0x120 */ u8 unk120;
    /* 0x121 */ u8 pad121[0x77];
    /* 0x198 */ u8 unk198;
} Sub1504B0FC;

typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ f32 unk18;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ f32 unk20;
    /* 0x024 */ f32 unk24;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ f32 unk30;
    /* 0x034 */ u8 pad34[0x8];
    /* 0x03C */ f32 unk3C;
    /* 0x040 */ u8 pad40[0x36];
    /* 0x076 */ u16 unk76;
    /* 0x078 */ u8 pad78[0x6];
    /* 0x07E */ u8 unk7E;
    /* 0x07F */ u8 unk7F;
    /* 0x080 */ u8 unk80;
    /* 0x081 */ u8 pad81[0x3];
    /* 0x084 */ u16 unk84;
    /* 0x086 */ u8 pad86[0x27];
    /* 0x0AD */ u8 unkAD;
    /* 0x0AE */ u8 padAE[0x4];
    /* 0x0B2 */ u16 unkB2;
    /* 0x0B4 */ u8 padB4[0x4];
    /* 0x0B8 */ f32 unkB8;
    /* 0x0BC */ f32 unkBC;
    /* 0x0C0 */ f32 unkC0;
    /* 0x0C4 */ u8 padC4[0x40];
    /* 0x104 */ u8 unk104;
    /* 0x105 */ u8 pad105[0xB];
    /* 0x110 */ u8 unk110;
    /* 0x111 */ u8 pad111[0x7];
    /* 0x118 */ f32 unk118;
    /* 0x11C */ u8 pad11C[0x6C];
    /* 0x188 */ void *unk188;
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ u8 pad190[0x16];
    /* 0x1A6 */ s16 unk1A6;
    /* 0x1A8 */ u8 pad1A8[0x22];
    /* 0x1CA */ u8 unk1CA;
    /* 0x1CB */ u8 pad1CB;
    /* 0x1CC */ f32 unk1CC;
    /* 0x1D0 */ u8 pad1D0[0x14C];
    /* 0x31C */ Sub1504B0FC *unk31C;
} Obj1504B0FC;

void func_1510F820(void *, void *, f32 *, f32 *, f32 *, f32 *, f32 *);
f32 func_1505210C(struct127 *arg0, f32 arg1, f32 arg2, f32 *arg3, f32 arg4);
void func_15060A30(s32, void *);
void func_1507CD64(void *, s32);
f32 func_1504B0FC(Obj1504B0FC *arg0, f32 arg1);
extern s8 D_80099140[];
extern f32 D_800991D4;
void func_1504BA38(struct127 *arg0);
void func_1504BAF0(struct127 *arg0);
void func_1504BB88(struct127 *arg0);
void func_1504BC38(struct127 *arg0);

typedef struct {
    u8  pad0[0x8];
    s16 unk8;
    u8  padA;
    u8  unkB;
    u8  padC[2];
    u16 unkE;
    u16 unk10;
    u8  pad12[2];
    u16 unk14;
    u8  unk16;
    u8  pad17[0x3C - 0x17];
    u8  unk3C;
} Struct1504BE2C;

void func_1507EB2C(void *arg0);
extern u16 D_800CC2B2;
extern f32 D_800CC2B4;
void func_1504BE2C(struct127 *arg0, u16 *arg1, f32 *arg2, s8 *arg3);
s32 func_1504C078(void);
s32 func_1504C0B8(void);

void func_15064A14(void *);
s32 func_15064B94(u8 *, f32, f32, s32, f32);
extern f32 D_800991DC;
extern f32 D_800991E0;
extern f32 D_800991E4;
extern f32 D_800991E8;
extern f32 D_800991EC;
extern f32 D_800991F0;
extern f32 D_800991F4;
extern f32 D_800991F8;
extern f32 D_800991FC;
extern f32 D_80099200;
extern u8 D_800CC2B1;
extern u16 D_800CC2B2;
extern f32 D_800CC2B4;
void func_1504C0E8(u8 *arg0, s32 arg1, f32 arg2);
void func_1504C854(struct127 *arg0);
s32 func_1504C8BC(void *arg0);
void func_1504C9E4(struct127 *arg0, s8 arg1, u8 arg2);

void func_1504CA60(struct127 *arg0) {
    f32 phi_f12;
    f32 temp_f2;

    temp_f2 = arg0->unk180 + 20.0f;
    arg0->unk1CC = arg0->y_position;
    phi_f12 = 18.0f;
    if (30.0f < arg0->unk28) {
        phi_f12 = 80.0f;
    }
    if (arg0->unk81 != 0) {
        phi_f12 = 80.0f;
        temp_f2 -= 30.0f;
        if ((arg0->unk28 < 5.0f) && (arg0->y_velocity < 0.0f)) {
            arg0->unk81 = 0;
            arg0->y_velocity = 45.0f;
        }
    } else if (arg0->unk28 < 20.0f) {
        if ((func_150ADA20() & 0x3F) == 0) {
            arg0->y_velocity = 25.0f;
        }
    }
    func_15058EA4(arg0, temp_f2, 3.8f, temp_f2, -3.5f, 80.0f, -phi_f12);
    arg0->unk80 = 1;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_79F10/func_1504CB98.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_79F10/func_150511E8.s")
typedef struct {
    /* 0x000 */ f32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ u8 pad8[0xF];
    /* 0x017 */ u8 unk17;
    /* 0x018 */ u8 pad18[0xC];
    /* 0x024 */ s16 unk24;
    /* 0x026 */ u8 pad26[0x18];
    /* 0x03E */ u16 unk3E;
    /* 0x040 */ u8 pad40[4];
    /* 0x044 */ s8 unk44;
    /* 0x045 */ u8 pad45[9];
    /* 0x04E */ u8 unk4E;
    /* 0x04F */ u8 unk4F;
    /* 0x050 */ u8 pad50[0x34];
    /* 0x084 */ u8 unk84;
    /* 0x085 */ u8 pad85[0x95];
    /* 0x11A */ u8 unk11A;
    /* 0x11B */ u8 pad11B[5];
    /* 0x120 */ u8 unk120;
    /* 0x121 */ u8 pad121[0x7A];
    /* 0x19B */ u8 unk19B;
} Sub15051558;

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ f32 x;
    /* 0x018 */ f32 y;
    /* 0x01C */ f32 z;
    /* 0x020 */ f32 yvel;
    /* 0x024 */ f32 unk24;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ f32 oldx;
    /* 0x030 */ f32 oldy;
    /* 0x034 */ f32 oldz;
    /* 0x038 */ u8 pad38[4];
    /* 0x03C */ f32 speed;
    /* 0x040 */ f32 unk40;
    /* 0x044 */ f32 unk44;
    /* 0x048 */ u8 pad48[0xC];
    /* 0x054 */ f32 unk54;
    /* 0x058 */ u8 pad58[0x1E];
    /* 0x076 */ u16 unk76;
    /* 0x078 */ u16 unk78;
    /* 0x07A */ u16 unk7A;
    /* 0x07C */ u8 pad7C[7];
    /* 0x083 */ u8 unk83;
    /* 0x084 */ u16 unk84;
    /* 0x086 */ u8 pad86[3];
    /* 0x089 */ u8 unk89;
    /* 0x08A */ u8 unk8A;
    /* 0x08B */ u8 pad8B[0x22];
    /* 0x0AD */ u8 unkAD;
    /* 0x0AE */ u8 padAE[0xA];
    /* 0x0B8 */ f32 unkB8;
    /* 0x0BC */ u8 padBC[8];
    /* 0x0C4 */ f32 unkC4;
    /* 0x0C8 */ u8 padC8[0x38];
    /* 0x100 */ u8 unk100;
    /* 0x101 */ u8 pad101[3];
    /* 0x104 */ u8 unk104;
    /* 0x105 */ u8 pad105[0xB];
    /* 0x110 */ u8 unk110;
    /* 0x111 */ u8 pad111;
    /* 0x112 */ u16 unk112;
    /* 0x114 */ u8 pad114[4];
    /* 0x118 */ f32 unk118;
    /* 0x11C */ u8 pad11C[0x20];
    /* 0x13C */ u8 unk13C;
    /* 0x13D */ u8 pad13D[0x25];
    /* 0x162 */ u8 unk162;
    /* 0x163 */ u8 pad163;
    /* 0x164 */ f32 unk164;
    /* 0x168 */ f32 unk168;
    /* 0x16C */ f32 unk16C;
    /* 0x170 */ f32 unk170;
    /* 0x174 */ u8 pad174[0x10];
    /* 0x184 */ u32 unk184;
    /* 0x188 */ void *unk188;
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ u8 pad190[0x3A];
    /* 0x1CA */ u8 health;
    /* 0x1CB */ u8 pad1CB[0xA9];
    /* 0x274 */ u8 unk274;
    /* 0x275 */ u8 pad275[0xA7];
    /* 0x31C */ Sub15051558 *unk31C;
    /* 0x320 */ u8 pad320[0xC];
} Obj15051558;

extern f32 D_800990C0[];
extern f32 D_800990E0[];
void func_1510F820(void *, void *, f32 *, f32 *, f32 *, f32 *, f32 *);
void func_15087EF0(s32, void *);
void func_1505A250(f32, f32, f32, f32 *, f32 *);
f32 func_1505210C(struct127 *arg0, f32 arg1, f32 arg2, f32 *arg3, f32 arg4);

void func_15051558(Obj15051558 *arg0, f32 arg1, f32 arg2, u16 arg3) {
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    u8 sp87;

    sp94 = 0.0f;
    sp87 = 0;
    sp8C = 0.200000003f;
    sp88 = arg0->unk28;
    sp90 = 0.0f;
    if (arg0->unkAD == 0) {
        s32 sp80;
        f32 dx;
        f32 dy;
        f32 dz;
        f32 dist;
        f32 sp6C;
        f32 sp68;
        f32 sp64;
        u16 angle;
        f32 sp5C;
        u16 ang2;
        f32 sp54;
        s16 shake;
        s32 kind;
        u16 anim;

        sp80 = arg0->unk162;
        sp6C = 0.0f;
        sp68 = 0.0f;
        sp64 = 3.0f;
        if (arg0->unk31C->unk4E == 2) {
            func_1510F820(arg0->unk188, &arg0->unk18C, &sp6C, &sp68, &sp64, &sp94, &sp90);
            if ((1.39999998f < (sp94 * sp94) + (sp90 * sp90)) && ((sp94 * sp94) + (sp90 * sp90) < 1000.0f)) {
                arg0->unk8A = 6;
                if ((sp88 < 80.0f) && (arg0->unk89 == 0)) {
                    func_15087EF0(D_800C3E78, arg0);
                }
            }
            if (sp88 >= 0.0f) {
                sp6C = 0.0f;
                sp68 = 0.0f;
                sp94 = 0.0f;
                sp90 = 0.0f;
                sp64 = 3.0f;
            }
        } else if (((sp88 <= 0.0f) || (arg0->unk110 != 0)) && (arg0->unk31C->unk4F == 0)) {
            func_1510F820(NULL, &arg0->unk18C, &sp6C, &sp68, &sp64, &sp94, &sp90);
        }
        if ((arg0->unk31C->unk4E == 0) && (arg0->unk31C->unk84 == 0) &&
            (((D_800CC27C == 0) && ((((sp94 * sp94) + (sp90 * sp90)) > 1.0f) || (((arg0->unk184 >> 12) & 0xF) == 4))) ||
             ((D_800CC27C != 0) && (arg0->unk31C->unk17 != 0))) &&
            (((arg0->unk184 >> 12) & 0xF) != 3)) {
            D_800CC27C = 1;
            sp87 = 1;
        }
        kind = (arg0->unk274 != 0) ? D_800CC2D0[arg0->unk274 - 1].id : -1;
        if (D_800CC27C == 0) {
            sp6C = 0.0f;
            sp68 = 0.0f;
            sp64 = 3.0f;
        }
        switch (kind) {
        case 8:
            sp87 = 0;
            break;
        case 0x49:
            sp87 = 0;
            break;
        }
        if ((D_800CC27C != 0) && (arg0->unk28 < 5.0f) && (arg0->unk110 != 0)) {
            arg0->unk162 = 1;
            if (arg0->unk89 != 0xFE) {
                arg0->unk89 = D_800BE9A0 + 1;
            }
        } else {
            arg0->unk162 = 0;
        }
        dx = arg1 - arg0->oldx;
        dy = arg0->y - arg0->oldy;
        dz = arg2 - arg0->oldz;
        dist = 2.0f * sqrtf((dx * dx) + (dy * dy) + (dz * dz)) * D_800BE9A8;
        if (arg0->unk162 - sp80) {
            if (arg0->unk162 != 0) {
                arg0->unk164 = dx * D_800BE9A8;
                arg0->unk168 = dz * D_800BE9A8;
                arg0->speed = 0.0f;
            } else {
                arg0->speed = dist;
                if (dist < 22.0f) {
                    arg0->speed = 22.0f;
                }
                if (arg0->unk110 == 3) {
                    arg0->unk110 = 0;
                    if (arg0->unk89 != 0xFE) {
                        arg0->unk89 = 0;
                    }
                    arg0->speed = 17.8999996f;
                }
                arg0->unk76 = func_1505A630(arg1 - arg0->oldx, arg0->oldz - arg2, 0);
                arg0->unk164 = 0.0f;
                arg0->unk168 = 0.0f;
                arg0->unk16C = 0.0f;
                arg0->unk170 = 0.0f;
            }
        }
        if ((D_800CC27C != 0) && (arg0->unk28 < 5.0f)) {
            sp5C = 1.0f;
            arg0->unk8A = D_800BE9A0 + 8;
            arg0->unk112 = func_1505A630(-sp6C, sp68, 0);
            angle = func_1505A630(arg0->x - arg0->oldx, arg0->oldz - arg0->z, 0);
            D_800CC27C = 1;
            if (arg0->unk110 == 0) {
                sp64 = D_800990C0[D_800CC27C];
                if ((u8)(((arg0->unk112 - angle) >> 8) + 0x30) < 0x60) {
                    ang2 = (arg0->unk44 > 8.0f) ? arg3 : arg0->unk7A;
                    if (sp87 != 0) {
                        arg0->unk110 = 3;
                        anim = 0x28E;
                        sp5C = 2.0f;
                    } else {
                        anim = 0xD0;
                        if ((u8)(((arg0->unk112 - ang2) >> 8) + 0x58) < 0xB0) {
                            arg0->unk110 = 1;
                            anim = 0x101;
                        } else {
                            arg0->unk110 = 2;
                        }
                    }
                    if ((arg0->unk104 == 0) && (arg0->unk13C == 0) && (arg0->unk89 != 0xFF) && (arg0->unk83 < 0xFA)) {
                        if ((arg0->unk31C->unk11A != 1) && (arg0->unk31C->unk11A != 3) && (arg0->unk31C->unk120 == 0) && (arg0->unk31C->unk19B == 0)) {
                            func_1505E650((struct127 *)arg0, anim, sp5C, 6.0f, 0.0f, 0.0f, 0);
                        }
                    }
                }
            } else {
                sp64 = D_800990E0[D_800CC27C];
            }
        }
        func_1505A250(-sp6C, -sp68, sp64, &arg0->unk164, &arg0->unk168);
        arg0->unk16C += (0.0f - arg0->unk16C) * 0.150000006f;
        arg0->unk170 += (0.0f - arg0->unk170) * 0.150000006f;
        if (arg0->unk110 != 0) {
            arg0->unk31C->unk3E = func_1505A630(arg0->x - arg0->oldx, arg0->oldz - arg0->z, 0);
            if ((arg0->unk84 == 0x28E) || (arg0->unk84 == 0x101) || (arg0->unk84 == 0xD0)) {
                if ((arg0->unk31C->unk11A != 1) && (arg0->unk31C->unk11A != 3)) {
                    arg0->unk83 = D_800BE9A0 + 1;
                }
            }
            if (D_800CC27C == 0) {
                if (arg0->unk110 == 2) {
                    arg0->unk31C->unk3E = arg0->unk76 = arg0->unk112;
                    if ((arg0->speed < 2.0f) && (arg0->unk28 == 0.0f)) {
                        if (arg0->unk104 == 0) {
                            func_1505E650((struct127 *)arg0, 0xA5, 1.39999998f, 5.0f, 0.0f, 0.0f, 0);
                            arg0->unk83 = 0x3C;
                            arg0->unk89 = 0x3C;
                            arg0->unk8A = 0x1E;
                            arg0->unk76 ^= 0x8000;
                            arg0->unk7A = arg0->unk76;
                            arg0->unk100 |= 0x20;
                        }
                        arg0->unk110 = 0;
                    }
                } else if (arg0->speed < 12.0f) {
                    arg0->unk110 = 0;
                }
            }
        }
        if (sp88 <= 0.0f) {
            sp94 = func_1505210C((struct127 *)arg0, sp94, sp90, &sp90, 50.0f);
            if (arg0->unk31C->unk4E != 2) {
                if ((arg0->unk31C->unk4 == 0) && (arg0->unk110 == 0) && ((arg0->unk31C->unk4E & 0xF) != 1)) {
                    sp94 *= (arg0->unk31C->unk0 * 0.00899999961f) + 0.200000003f;
                    sp90 *= (arg0->unk31C->unk0 * 0.0140000004f) + 0.200000003f;
                } else {
                    sp94 *= 0.800000012f;
                    sp90 *= 0.800000012f;
                    if ((arg0->unk31C->unk4E & 0xF) == 1) {
                        sp94 *= 0.5f;
                        sp90 = 0.0f;
                    }
                }
            }
        }
        if (arg0->unk31C->unk4E == 2) {
            {
                f32 yv = arg0->yvel;
                sp54 = arg0->unk54 * 6.0f;
                if (yv < -30.0f) {
                    sp90 -= -30.0f;
                } else {
                    sp90 -= yv;
                }
            }
            sp8C = 0.400000006f;
            if (arg0->unk31C->unk44 > 0) {
                shake = (D_800CC250 & 4);
                shake -= 2;
                shake *= (u32)(arg0->unk31C->unk44 * 2);
                sp54 += shake;
                sp8C = 0.25f;
            }
            if ((fabsf(arg0->unkC4) > 70.0f) && (D_800BE616 == 0) && (arg0->health != 0) && (arg0->unk31C->unk24 == 0)) {
                D_800D1580 = 0xFF020146;
                func_1506E8D8();
                arg0->unk31C->unk24 = 1;
            }
            if (fabsf(arg0->unkC4) < 20.0f) {
                arg0->unk31C->unk24 = 0;
            }
            sp94 += sp54;
        }
        arg0->unkB8 += (sp90 - arg0->unkB8) * sp8C;
    } else {
        f32 f2;

        f2 = (arg0->unk118 - 30.0f) - arg0->y;
        f2 += arg0->speed * 6.0f;
        if (f2 > 250.0f) {
            f2 = 2.5f;
        } else {
            f2 *= 0.00999999978f;
        }
        if ((fabsf(arg0->unkB8) < 35.0f) && (f2 > 0.0f)) {
            sp94 += arg0->unk54 * f2;
        }
        func_1505A250(0.0f, 0.0f, 0.300000012f, &arg0->unk164, &arg0->unk168);
    }
    arg0->unkC4 += (sp94 - arg0->unkC4) * sp8C;
}
f32 func_1505210C(struct127 *arg0, register f32 arg1, register f32 arg2, f32 *arg3, f32 arg4) {
    f32 sp34;
    s32 pad;
    f32 sp2C;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f2;

    arg2 = -arg2;
    sp2C = arg0->unk40 * D_80099330;
    sp34 = func_150AD780(sp2C);
    temp_f0 = func_150AD78C(sp2C);
    temp_f12 = (arg1 * sp34) + (arg2 * temp_f0);
    arg2 = (-arg1 * temp_f0) + (arg2 * sp34);
    arg1 = func_150484A0(temp_f12, 1.0f) * D_80099334;
    temp_f2 = func_150484A0(arg2, 1.0f) * D_80099338;
    arg2 = temp_f2;
    if (arg1 >= 180.0f) {
        arg1 -= 360.0f;
    }
    if (temp_f2 >= 180.0f) {
        arg2 = temp_f2 - 360.0f;
    }
    temp_f0 = -arg4;
    if (arg4 < arg1) {
        arg1 = arg4;
    }
    if (arg1 < temp_f0) {
        arg1 = temp_f0;
    }
    if (arg4 < arg2) {
        arg2 = arg4;
    }
    if (arg2 < temp_f0) {
        arg2 = temp_f0;
    }
    *arg3 = arg2;
    return arg1;
}
void func_1505A770(struct127 *arg0);
void func_15052260(struct127 *arg0);
void func_15052408(struct127 *arg0);
void func_15052458(s32 arg0);
void func_15052464(struct127 *arg0);
void func_15052490(struct127 *arg0, u16 arg1, f32 arg2, f32 arg3);
void func_1505250C(struct127 *arg0, s32 arg1);
void func_15052590(struct127 *arg0);
extern f32 D_80099348;
extern f32 D_8009934C;
void func_15052760(void *arg0, s16 arg1);
void func_15052EF0(struct127 *arg0);
struct127 *func_15052F58(s32 arg0, s32 arg1);

u8 *func_15033E84(struct127 *arg0);
void func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
// NON-MATCHING: plenty still to do here
// void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4) {
//     // s32 sp44;
//     f32 sp3C;
//     struct127 *temp_s0;
//     struct126 *temp_v0;
//
//     temp_s0 = &D_800CC2D0[arg0->unk124];
//     if ((temp_s0->disable_run == 0) && (temp_s0->stunned == 0)) {
//         temp_v0 = temp_s0->unk31C;
//         if ((temp_v0 == 0) || (temp_v0->unk6B == 0)) {
//             temp_s0->immune = 0xFFU;
//             temp_s0->y_velocity = arg1;
//             temp_s0->gravity = arg2;
//             temp_s0->unk83 = 0xFFU;
//             temp_s0->disable_run = 0xFFU;
//             temp_s0->unk76 = arg0->unk7A;
//             arg0->unk218 = 0;
//             arg0->unk232 = arg3;
//             // func_1505E650(arg1, arg2, temp_s0, 0x32, 1.2999999523162842f, 4.0f, 0.0f, 0.0f, 0);
//             func_1505E650(arg0, 50, 0x3FA66666, 4.0f, 0.0f, 0.0f, 0);
//             D_800CC2D0[arg0->unk124].unk31C->unk30 = 70;
//             D_800CC2D0[arg0->unk124].unk31C->unk28 = arg0->unk14;
//             D_800CC2D0[arg0->unk124].unk31C->unk2C = arg0->unk1C;
//             if ((arg0->unk1D4 != 0) && (arg4 != -1)) {
//                 func_15043FF0(&sp3C, arg0->unk1D4 + (arg4 << 6));
//                 D_800CC2D0[arg0->unk124].unk31C->unk28 = sp3C;
//                 // D_800CC2D0[arg0->unk124].unk31C->unk2C = sp44;
//             }
//             D_800CC2D0[arg0->unk124].unk31C->unk27 = 1;
//         }
//     }
// }

s32 func_15053430();
s32 func_1507D754(struct127 *arg0);
void func_151669A0(s32 arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5);
void func_15055B0C(struct127 *arg0, s32 arg1);
s32 func_15053430();
s32 func_150535F4(struct127 *arg0);
void func_15053694(struct127 *arg0);
void func_150536D0(struct127 *arg0);
void func_150536E8(struct127 *arg0);
void func_1505371C(struct127 *arg0);
void func_15053750(struct127 *arg0);
void func_15053894(struct127 *arg0);
void func_150548D8(s32 arg0);
void func_150548E4(struct127 *arg0);
void func_15054A0C(struct127 *arg0);
void func_15054A5C(struct127 *arg0, struct127 *arg1);

void func_151026BC(void *, s32, s32, s32, s32, s32);
void func_15188810(u8 *, s32, s32);
void func_150836CC(u8 *, s32);
void func_1507C324(void *, void *);
void func_15054A94(u8 *arg0);
void func_15054F74(struct127 *arg0);
void func_15055260(s32 arg0, s32 arg1, s32 arg2);

void func_1514AD9C(f32 *, s32, s32);
void func_1503192C(struct127 *, s32, s32, s32);
s32 func_1505C7D8(struct127 *, u8);
void func_15059C84(struct127 *);
void func_1505A770(struct127 *);
void func_151DCDE0(struct17 *, s32, s32);
void func_1505528C(struct127 *arg0);
void func_15055A2C(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
void func_15055B0C(struct127 *arg0, s32 arg1);
void func_15055B64(struct127 *arg0);
void func_15055BF8(struct127 *arg0);
void func_15055C88(struct127 *arg0);
void func_15055D48(struct127 *arg0);
