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
void func_1504CA60(struct127 *arg0);
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
void func_15051558(Obj15051558 *arg0, f32 arg1, f32 arg2, u16 arg3);
f32 func_1505210C(struct127 *arg0, register f32 arg1, register f32 arg2, f32 *arg3, f32 arg4);
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

void func_150536D0(struct127 *arg0) {
    arg0->interaction_state = 13;
    arg0->unkE4 = 0;
    arg0->unkE6 = 0;
}

void func_150536E8(struct127 *arg0) {
    arg0->unk40 = ((s16) (arg0->unk7A + 16384)) * 0.005493164f;
}

void func_1505371C(struct127 *arg0) {
    arg0->unk40 = ((s16) (arg0->unk7A + 16384)) * 0.005493164f;
}

void func_15053750(struct127 *arg0) {
    f32 temp_f2;
    f32 temp_f16;
    s32 phi_v0;

    temp_f2 = arg0->unk154 - arg0->xz_scale;
    if (temp_f2 != 0.0f) {
        temp_f16 = arg0->unk15C;
        if (fabsf(temp_f2) < temp_f16) {
            func_15062BDC(arg0, arg0->unk154, arg0->y_scale);
        } else {
            if (temp_f2 < 0.0f) {
                phi_v0 = -1;
            } else {
                phi_v0 = 1;
            }
            func_15062BDC(arg0, ((f32) phi_v0 * temp_f16) + arg0->xz_scale, arg0->y_scale);
        }
    }

    temp_f2 = arg0->unk158 - arg0->y_scale;
    if (temp_f2 != 0.0f) {
        temp_f16 = arg0->unk15C;
        if (fabsf(temp_f2) < temp_f16) {
            func_15062BDC(arg0, arg0->xz_scale, arg0->unk158);
        } else {
            if (temp_f2 < 0.0f) {
                phi_v0 = -1;
            } else {
                phi_v0 = 1;
            }
            func_15062BDC(arg0, arg0->xz_scale, ((f32) phi_v0 * temp_f16) + arg0->y_scale);
        }
    }
}

void func_15053894(struct127 *arg0) {
    arg0->unk7F += 1;
    if (arg0->unk7F >= 0x1F) {
        func_15060F28(arg0, 1);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_80B80/func_150538CC.s")

void func_150548D8(s32 arg0) {
}

void func_150548E4(struct127 *arg0) {
    struct127 *temp_s0;
    struct127 *phi_s2;
    s32 phi_v0;
    s32 i;

    temp_s0 = &D_800CC2D0[arg0->unk65 - 1];

    for (i = 0; i < D_8008FD8C; i++) {
        phi_s2 = &D_800CC2D0[i];

        if ((phi_s2->unk274 == (((s32) ((s32)arg0 - (s32)D_800CC2D0) / (s32)sizeof(struct127)) + 1)) && (temp_s0->unk232 == 1)) {
            temp_s0->unk218 = 0;
            if ((func_150ADA20() & 1) != 0) {
                temp_s0->unk232 = (u8)2U;
                phi_v0 = 0x1DB;
            } else {
                temp_s0->unk232 = (u8)3U;
                phi_v0 = 0x1DC;
            }
            func_10010344(phi_v0, temp_s0, 0x7D00, 0x1F4, 0x9C4);
        }
    }
}

void func_15054A0C(struct127 *arg0) {
    if (arg0->unk2D0 != 0) {
        if ((arg0->unk2D0->unk18 - 1.0f) <= arg0->unk2D0->unk8) {
            func_15060F28(arg0, 0);
        }
    }
}

void func_15054A5C(struct127 *arg0, struct127 *arg1) {
    arg1->unk31C->unk11A = 0;
    func_151027E8(arg1);
    func_15060F28(arg0, 0);
}

void func_151026BC(void *, s32, s32, s32, s32, s32);
void func_15188810(u8 *, s32, s32);
void func_150836CC(u8 *, s32);
void func_1507C324(void *, void *);

void func_15054A94(u8 *arg0) {
    u8 *anim;
    u8 *other;
    u8 *otherAnim;
    s32 motion;
    s32 attach;
    s32 action;
    s32 hold;
    s32 enter;
    u8 *state;
    s32 index;
    u8 ready;

    anim = *(u8 **)(arg0 + 0x2D0);
    if (anim == NULL || arg0[0x65] == 0) {
        return;
    }
    other = (u8 *)D_800CC2D0 + (arg0[0x65] - 1) * 0x32C;
    otherAnim = *(u8 **)(other + 0x2D0);
    if (otherAnim == NULL) {
        return;
    }
    if (arg0[4] == 0x41) {
        if (*(u16 *)(other + 0x84) != 0x48 && *(u16 *)(other + 0x84) != 0x4B) {
            func_15060F28((struct127 *)arg0, 0);
            return;
        }
        if (*(u16 *)(arg0 + 0x84) == 0 && *(f32 *)(anim + 8) == 0.0f) {
            func_15188810(arg0, 0x64, 0);
        }
        if (*(u16 *)(arg0 + 0x84) == 0 && *(u16 *)(other + 0x84) == 0x4B) {
            func_1505E650((struct127 *)arg0, 1, 1.0f, 3.0f, 0.0f, 0.0f, 0);
        }
        if (*(u16 *)(arg0 + 0x84) == 1 && *(f32 *)(anim + 0x18) - 1.0f <= *(f32 *)(anim + 8)) {
            func_15060F28((struct127 *)arg0, 0);
        }
        return;
    }
    if (arg0[4] != 0x1D && arg0[4] != 0x6E && arg0[4] != 0x2F) {
        return;
    }
    if (arg0[0x65] == 0 || other[0x65] != 0 || other[0x1CA] == 0) {
        func_15054A5C((struct127 *)arg0, (struct127 *)other);
        return;
    }
    state = *(u8 **)(other + 0x31C);
    if (state[0x78] != 0x12 && state[0x78] != 0x18 && state[0x78] != 0x41 && state[0x11A] != 3) {
        func_15054A5C((struct127 *)arg0, (struct127 *)other);
        return;
    }
    if (arg0[4] == 0x1D) {
        enter = 0xC4;
        hold = 0xC7;
        action = 0x39;
    } else if (arg0[4] == 0x6E) {
        enter = 0xFB;
        hold = 0xFC;
        action = 0x51;
    } else {
        enter = 0x2C8;
        hold = 0x2C9;
        action = 0x51;
    }
    if (other[0x13C] != 0) {
        func_15054A5C((struct127 *)arg0, (struct127 *)other);
        return;
    }
    if (arg0[4] == 0x1D && other[0x104] != 0) {
        func_15054A5C((struct127 *)arg0, (struct127 *)other);
        return;
    }
    attach = 1;
    if (hold == *(u16 *)(other + 0x84)) {
        motion = 2;
        if (*(s32 *)(arg0 + 0x2E4) == 0) {
            if (*(f32 *)(otherAnim + 8) >= 6.0f && *(f32 *)(otherAnim + 8) < 11.0f) {
                *(s32 *)(arg0 + 0x2E4) = 1;
                func_15083568((struct127 *)arg0, action, 1.0f, 0);
            }
        } else if (*(f32 *)(otherAnim + 8) >= 11.0f) {
            *(s32 *)(arg0 + 0x2E4) = 0;
            func_150836CC(arg0, action);
        }
    } else {
        if (*(s32 *)(arg0 + 0x2E4) == 1) {
            *(s32 *)(arg0 + 0x2E4) = 0;
            func_150836CC(arg0, action);
        }
        if (enter == *(u16 *)(other + 0x84)) {
            motion = 0;
        } else if (*(u16 *)(other + 0x84) == 0xC6) {
            motion = 3;
        } else {
            motion = 1;
            attach = 0;
        }
    }
    func_1505E650((struct127 *)arg0, motion, 1.0f, 0.0f, 0.0f, 0.0f, 0);
    if (attach) {
        func_1507C324(arg0, other);
    }
    state = *(u8 **)(other + 0x31C);
    if (state[0x78] == 0x12) {
        ready = 1;
    } else {
        ready = state[0x197];
        if (state[0x84] == 0) {
            index = (other - (u8 *)D_800CC2D0) / 0x32C;
            if (index < 4 && !(*(*(u16 ***)D_800BE728)[index] & 0x10)) {
                ready = 0;
            }
        }
    }
    if (*(u16 *)(other + 0x84) == 0x10A && *(s16 *)(otherAnim + 0x3C) <= 0 && ready &&
        (*(u16 *)(state + 0x8A) & 0x2000)) {
        if (*(s32 *)(arg0 + 0x2E8) == 0) {
            func_151026BC(other, -1, 3, 1, 0xFF, 1);
            *(s32 *)(arg0 + 0x2E8) = 1;
        }
    } else if (*(s32 *)(arg0 + 0x2E8) == 1) {
        *(s32 *)(arg0 + 0x2E8) = 0;
        func_151027E8(other);
    }
}

void func_15054F74(struct127 *arg0) {
    s16 phi_v1;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    s16 x;
    s16 z;

    if (arg0->id == 12) {
        if ((D_800C3E78 + 1) != D_800CC2D0[arg0->unk124].unk65) {
            if (arg0->unk232 == 18) {
                func_15052F9C(arg0, 50.0f, 0, 4, 0, 12, 0, 8, 0, 0);
            } else if (D_800CC268 & 1) {
                if (arg0->unk25C & 0x400) {
                    arg0->unk124 = 0;
                    func_1505327C(arg0, 39.0f, 3.0f, 18, 0);
                }
            }
        }
    } else if (arg0->id == 0x4B) {
        func_1505A184(arg0->unk7A ^ 0x8000, 500.0f, 0.0f, &sp50, &sp48, &sp4C);
        x = (s16)arg0->x_position + sp50;
        z = (s16)arg0->z_position + sp48;
        arg0->unk2EC = (x << 0x10) | (z & 0xFFFF);
        if (arg0->unk84.uh != 0xF) {
            phi_v1 = func_1505A630(D_800CC2D0[0].x_position - arg0->x_position, arg0->z_position - D_800CC2D0[0].z_position, 0);
        } else {
            phi_v1 = arg0->unk7A - 0x800;
        }
        phi_v1 -= arg0->unk7A;
        phi_v1 /= 3;
        arg0->unk7A += phi_v1;
        arg0->xz_scale = 2.0f;
        arg0->y_scale = 2.0f;
        arg0->unk40 = 90.0f;
    label:
        ;
    } else if (((arg0->id == 0x1B) || (arg0->id == 0x7E)) && (arg0->unk2E4 != 0)) {
        if (D_800BE616 != 0) {
            func_1508B20C(arg0->x_position, arg0->y_position, arg0->z_position, 900.0f);
        }
        if (D_800BE9E4 < arg0->unk2E4) {
            if ((arg0->unk2E4 >= 0x78) && (arg0->unk2E4 < (D_800BE9E4 + 0x78))) {
                func_10010154(0x3A1, arg0, 0x7D00, 0x1F4, 0x3E8);
            }
            arg0->unk2E4 -= D_800BE9E4;
        } else {
            arg0->unk232 = 6;
            arg0->unk218 = 0;
        }
    }
}
// void func_15054F74(struct127 *arg0) {
//     s16 phi_v1;
//
//     f32 sp50;
//     f32 sp4C;
//     f32 sp48;
//
//     f32 temp_f2;
//     f32 temp_f0;
//
//     if (arg0->id == 12) {
//         if ((D_800C3E78 + 1) != D_800CC2D0[arg0->unk124].unk65) {
//             if (arg0->unk232 == 18) {
//                 func_15052F9C(arg0, 50.f, 0, 4, 0, 12, 0, 8, 0, 0);
//             } else if ((D_800CC268 & 1) != 0) {
//                 if ((arg0->unk25C & 0x400) != 0) {
//                     arg0->unk124 = 0;
//                     func_1505327C(arg0, 39.0f, 3.0f, 18, 0);
//                 }
//             }
//         }
//     } else {
//         if (arg0->id == 0x4B) {
//             func_1505A184(arg0->unk7A ^ 0x8000, 500.0f, 0, &sp50, &sp48, &sp4C);
//             temp_f0 = arg0->unk14;
//             temp_f2 = arg0->unk1C;
//             arg0->unk2EC = (((s32) ((f32) (s16) (s32) temp_f0 + sp50) << 0x10) | ((s32) ((f32) (s16) (s32) temp_f2 + sp48) & 0xFFFF));
//             if (arg0->unk84.uh  != 0xF) {
//                 phi_v1 = func_1505A630(D_800CC2D0->unk14 - temp_f0, temp_f2 - D_800CC2D0->unk1C, 0);
//             } else {
//                 phi_v1 = (s16) (arg0->unk7A - 0x800);
//             }
//             arg0->unk7A += (s16) (phi_v1 - arg0->unk7A) / 3;
//             arg0->xz_scale = 2.0f;
//             arg0->y_scale = 2.0f;
//             arg0->unk40 = 90.0f;
//         } else if (((arg0->id == 0x1B) || ((arg0->id == 0x7E))) && (arg0->unk2E4 != 0)) {
//             if (D_800BE616 != 0) {
//                 func_1508B20C(arg0->unk14, arg0->unk18, 500.0f, 900.0f);
//             }
//             if ((arg0->unk2E4 > D_800BE9E4) && (arg0->unk2E4 >= 0x78) && (arg0->unk2E4 < (D_800BE9E4 + 0x78))) {
//                 func_10010154(0x3A1, arg0, 0x7D00, 0x1F4, 0x3E8);
//                 arg0->unk2E4 -= D_800BE9E4;
//             } else {
//                 arg0->unk232 = 6U;
//                 arg0->unk218 = 0;
//             }
//         }
//     }
// }

void func_15055260(s32 arg0, s32 arg1, s32 arg2) {
    func_150335C8(arg0, arg1, 0x4D, arg2, 0, 0);
}

void func_1514AD9C(f32 *, s32, s32);
void func_1503192C(struct127 *, s32, s32, s32);
s32 func_1505C7D8(struct127 *, u8);
void func_15059C84(struct127 *);
void func_1505A770(struct127 *);
void func_151DCDE0(struct17 *, s32, s32);

void func_1505528C(struct127 *arg0) {
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    s32 phi_a2;
    s32 sp74;
    f32 sp70;
    s32 sp6C;
    s32 temp_v1;
    s32 sp64;

    sp74 = (u16)arg0->unk278;
    sp6C = 0;
    sp70 = 0.0f;

    switch (sp74) {
        case 0x1F:
            arg0->unkB8 = (arg0->unkB8 + (arg0->xz_velocity * (f32) D_800BE9E4));
            if (arg0->unk2E4 == 1) {
                func_10010344(0x230, arg0, 0x7D00, 0x1F4, 0x9C4);
                arg0->unk2E4 = 0;
            }
            break;
        case 0x42: {
            s32 temp_v0;

            sp70 = 25.0f;
            if ((arg0->gravity != 0.0f) && ((arg0->y_velocity > 0.0f) || (arg0->unk28 > sp70))) {
                arg0->unkB8 = (-func_150484A0(arg0->y_velocity, arg0->xz_velocity) * D_80099408);
            }
            temp_v0 = func_15081574((s32)arg0, 40.0f, 10.0f, (s32)&sp64, 0xF, 0x53);
            if (temp_v0 != 0) {
                func_15055260(arg0, sp64, temp_v0 - 1);
                func_15060F28(arg0, 1);
                return;
            }
            } break;
        case 0x4E:
            sp70 = 25.0f;
            if (arg0->unk28 > sp70) {
                arg0->unkB8 = (arg0->unkB8 + (f32) (D_800BE9E4 * 3));
            }
            break;
        case 0x35:
            arg0->unkB8 = (arg0->unkB8 + (f32) (D_800BE9E4 * 0x10));
            break;
        case 0x88:
            arg0->unkB8 = -func_150484A0(arg0->y_velocity, arg0->xz_velocity) * D_8009940C;
            arg0->unkD0 = 0x10;
            if (func_1505C7D8(arg0, D_800C3E78) != 0) {
                func_15060F28(arg0, 1);
                return;
            }
            break;
    }
    func_15059C84(arg0);
    arg0->stunned = 1;
    func_1505A770(arg0);
    sp84 = arg0->x_position;
    sp80 = arg0->y_position;
    sp7C = arg0->z_position;
    func_15058898(arg0, arg0->old_y_position);
    if ((D_80099410 != arg0->unk118) && (arg0->y_position <= arg0->unk118) && (arg0->unk118 <= arg0->old_y_position)) {
        arg0->xz_velocity *= D_80099414;
        arg0->unk2E4 = 2;
        arg0->gravity = 0.0f;
        arg0->y_velocity = -3.0f;
    } else if ((sp84 != arg0->x_position) || (sp80 != arg0->y_position) || (sp7C != arg0->z_position)) {
        sp6C = 1;
        if ((sp74 == 0x1F) && (arg0->unk2E4 != 2)) {
            func_10010344(((func_150ADA20() % 5U) + 0x3A4), arg0, 0x7D00, 0x1F4, 0x9C4);
        }
    }
    func_1505B5F8(arg0, arg0->unk180);
    if (arg0->unk28 <= sp70) {
        arg0->unk7F++;
        sp6C |= 2;
        if ((arg0->unk184 & 0x1F) == 0xE) {
            func_1514AD9C(&arg0->x_position, 0xFF, 0);
            func_15060F28(arg0, 1);
            return;
        }
        if (sp74 == 0x42) {
            f32 phi_f0;
            f32 temp_f2;

            D_800D1580 = 0xFF0100B0;
            if (arg0->gravity != 0.0f) {
                func_1506E5FC();
            }
            temp_f2 = arg0->y_position;
            arg0->gravity = 0;
            phi_f0 = temp_f2 - arg0->old_y_position;
            arg0->y_velocity = 0;
            arg0->xz_velocity = 0;
            arg0->y_position = arg0->unk180 + sp70;
            if (phi_f0 != 0.0f) {
                phi_f0 = (arg0->y_position - arg0->old_y_position) / phi_f0;
            } else {
                phi_f0 = 0;
            }
            arg0->x_position = arg0->old_x_position + ((arg0->x_position - arg0->old_x_position) * phi_f0);
            arg0->z_position = arg0->old_z_position + ((arg0->z_position - arg0->old_z_position) * phi_f0);
        } else if (arg0->y_position < arg0->unk118) {
            arg0->gravity = 0.0f;
            arg0->y_velocity = 0.0f;
            arg0->xz_velocity = 0.0f;
        } else if ((arg0->y_velocity > 5.0f) && (sp74 == 0x1F) && (arg0->unk2E4 != 2)) {
            func_10010344(0x39E, arg0, 0x7D00, 0x1F4, 0x9C4);
            arg0->unk2E4 = 2;
        }
    }
    arg0->unk40 = ((arg0->unk76 + 0x4000) * 0.005493164f);
    if ((sp6C != 0) && (sp74 == 0x35)) {
        func_10010630(0x360, arg0, 0x4E20, 0x1F4, 0x9C4);
        if (arg0->unk118 < arg0->y_position) {
            struct17 tmp0;

            tmp0.unk0 = arg0->x_position;
            tmp0.unk4 = arg0->y_position + 20.0f;
            tmp0.unk8 = arg0->z_position;
            func_151DCDE0(&tmp0, 0xFF, 1);
        }
        func_15060F28(arg0, 1);
        return;
    }
    temp_v1 = --arg0->unk83;
    if ((temp_v1 == 0) || (arg0->unk7F >= 0x1F)) {
        func_15060F28(arg0, 1);
        return;
    }
    if (temp_v1 < 0xA) {
        phi_a2 = (temp_v1 * 0xFF) / 0xA;
    } else if (arg0->unk7F >= 0x15) {
        phi_a2 = ((-arg0->unk7F * 0xFF) + 0x1DE2) / 0xA;
    } else {
        phi_a2 = 0xFF;
    }
    func_1503192C(arg0, sp74, phi_a2, 0);
    if (sp74 == 0x1F) {
        struct108 *temp_a0;
        f32 temp_f14;

        temp_a0 = D_800DBFF0;
        temp_f14 = ((temp_a0->unk2F8 - arg0->x_position) * (temp_a0->unk2F8 - arg0->x_position)) + ((temp_a0->unk2FC - arg0->y_position) * (temp_a0->unk2FC - arg0->y_position)) + ((temp_a0->unk300 - arg0->z_position) * (temp_a0->unk300 - arg0->z_position));
        if (temp_f14 < 900.0f) {
            func_1512D748(temp_a0, 8, 0);
            func_10010F88(0x2E1, 0x6D60, 0, 0, 0, arg0->x_position, arg0->y_position, arg0->z_position, 0x1F4, 0x3E8);
            func_10010A3C(arg0);
            func_15060F28(arg0, 1);
        }
    }
}

void func_15055A2C(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    s32 phi_a0 = 1549;
    s32 phi_v1 = 1000;

    if (arg4 == 1) {
        phi_a0 = 1549;
        phi_v1 = 100;
    } else {
        phi_a0 = D_8009919C[func_150ADA20() % 6U];
    }
    func_10010F88(phi_a0, 32700, func_150ADA20() % 500U, 0, 0, arg1,  arg2, arg3, phi_v1, 3000);
}

void func_15055B0C(struct127 *arg0, s32 arg1) {
    arg0->interaction_state = 39;
    func_1505E650(arg0, arg0->unk84.uh, 0, 0.0f, 0.0f, 0.0f, 0);
    arg0->unkE4 = 0;
    arg0->unkE6 = 0;
    arg0->unk21C = arg1;
}

void func_15055B64(struct127 *arg0) {
    arg0->unk40 = (s16)(arg0->unk7A + 16384) * 0.005493164f;
    arg0->unkF8 |= 0x1000000;

    if (D_800BE9A0 < arg0->unk21C) {
        arg0->unk21C -= D_800BE9E4;
    } else {
        arg0->unk21C = 0;
        if (arg0->unk1D4 == 0) {
            func_15060F28(arg0, 1);
        }
    }
}

void func_15055BF8(struct127 *arg0) {
    arg0->unk40 = (s16)(arg0->unk7A + 16384) * 0.005493164f;
    arg0->unkF8 |= 0x1000000;
    arg0->y_position -= D_800BE9A4;

    if (arg0->y_position < (arg0->unk180 - 100.0f)) {
        func_15060F28(arg0, 0);
    }
}

void func_15055C88(struct127 *arg0) {
    s32 sp2C = 2099172; // 0x2007E4;
    if (((arg0->unk9C & 0x1000) == 0) && func_1506E46C(arg0, &sp2C, 1)) {
        func_15060A30(sp2C, arg0);
    }
    arg0->unk31C->unk36 = 100;
    func_1506160C(arg0, 2, 0, 6, 0);
    if (0) {};
    arg0->disable_run = 200;
    arg0->unk83 = 200;
    arg0->unk1CC = D_80099418;
    arg0->interaction_state = 41;
    arg0->gravity = 0.0f;
    arg0->y_velocity = 0.0f;
    arg0->xz_velocity = 0.0f;
    func_1507CD64(arg0, 9);
}

void func_15055D48(struct127 *arg0) {
    f32 temp_f0;

    arg0->unk31C->unk36 += D_800BE9A0;
    if (arg0->unk31C->unk36 >= 0x8D) {
        D_800D18A0 |= (1 << (((s32)arg0 - (s32)D_800CC2D0) / (s32)sizeof(struct127)));
        if (D_800D18A0 != 0) {
            arg0->immune = 0xFF;
            arg0->stunned = 0xFF;
            arg0->disable_run = 0xFF;
            arg0->unkF8 |= 0x200;
            arg0->unkE4 = 0;
            arg0->unkE6 = 0;
            arg0->unk7 = 0;
            arg0->health = 0;
            arg0->interaction_state = 5;
            arg0->xz_velocity = 0.0f;
            arg0->y_velocity = 0.0f;
            arg0->gravity = 0.0f;
            return;
        }
        func_1506D538();
    }
    temp_f0 = D_8009941C;
    arg0->health = 0;
    arg0->xz_scale *= temp_f0;
    arg0->y_scale *= temp_f0;
    arg0->xz_velocity *= temp_f0;
}
