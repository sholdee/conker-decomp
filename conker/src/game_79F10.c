#include <ultra64.h>

#include "functions.h"
#define D_800D3098 D_800D3098_header
#include "variables.h"
#undef D_800D3098


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
void func_1505A250(f32, f32, f32, f32 *, f32 *);
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
typedef struct {
    /* 0x000 */ f32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s16 unk8;
    /* 0x00A */ u8 padA;
    /* 0x00B */ u8 unkB;
    /* 0x00C */ u8 padC[4];
    /* 0x010 */ s16 unk10;
    /* 0x012 */ s16 unk12;
    /* 0x014 */ u8 pad14[3];
    /* 0x017 */ u8 unk17;
    /* 0x018 */ u8 pad18[8];
    /* 0x020 */ u8 unk20;
    /* 0x021 */ u8 unk21;
    /* 0x022 */ s8 unk22;
    /* 0x023 */ s8 unk23;
    /* 0x024 */ u8 pad24[2];
    /* 0x026 */ u8 unk26;
    /* 0x027 */ u8 pad27;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ s8 unk30;
    /* 0x031 */ u8 unk31;
    /* 0x032 */ u16 unk32;
    /* 0x034 */ u16 unk34;
    /* 0x036 */ u8 unk36;
    /* 0x037 */ u8 pad37;
    /* 0x038 */ s32 unk38;
    /* 0x03C */ u8 unk3C;
    /* 0x03D */ u8 pad3D;
    /* 0x03E */ u16 unk3E;
    /* 0x040 */ f32 unk40;
    /* 0x044 */ s8 unk44;
    /* 0x045 */ u8 pad45;
    /* 0x046 */ s16 unk46;
    /* 0x048 */ s16 unk48;
    /* 0x04A */ u8 pad4A[2];
    /* 0x04C */ u16 unk4C;
    /* 0x04E */ u8 unk4E;
    /* 0x04F */ u8 unk4F;
    /* 0x050 */ s8 unk50;
    /* 0x051 */ u8 pad51;
    /* 0x052 */ u8 unk52;
    /* 0x053 */ u8 unk53;
    /* 0x054 */ s8 unk54;
    /* 0x055 */ s8 unk55;
    /* 0x056 */ u8 pad56[0x15];
    /* 0x06B */ u8 unk6B;
    /* 0x06C */ u8 pad6C[9];
    /* 0x075 */ u8 unk75;
    /* 0x076 */ u8 pad76[2];
    /* 0x078 */ u8 unk78;
    /* 0x079 */ u8 pad79[4];
    /* 0x07D */ u8 matrix_physics;
    /* 0x07E */ u8 pad7E[6];
    /* 0x084 */ u8 unk84;
    /* 0x085 */ s8 unk85;
    /* 0x086 */ s8 unk86;
    /* 0x087 */ u8 pad87;
    /* 0x088 */ s16 unk88;
    /* 0x08A */ u16 unk8A;
    /* 0x08C */ u16 unk8C;
    /* 0x08E */ u8 pad8E[7];
    /* 0x095 */ u8 unk95;
    /* 0x096 */ u8 unk96;
    /* 0x097 */ u8 unk97;
    /* 0x098 */ u8 unk98;
    /* 0x099 */ u8 pad99[3];
    /* 0x09C */ struct Sub9C *unk9C;
    /* 0x0A0 */ f32 unkA0;
    /* 0x0A4 */ u8 padA4[0xB];
    /* 0x0AF */ u8 unkAF;
    /* 0x0B0 */ u8 padB0[0x58];
    /* 0x108 */ u8 unk108;
    /* 0x109 */ u8 unk109;
    /* 0x10A */ u8 pad10A[0x10];
    /* 0x11A */ u8 unk11A;
    /* 0x11B */ u8 pad11B[5];
    /* 0x120 */ u8 unk120;
    /* 0x121 */ u8 pad121[0xB];
    /* 0x12C */ u16 unk12C;
    /* 0x12E */ u8 pad12E[0x3E];
    /* 0x16C */ f32 unk16C;
    /* 0x170 */ f32 unk170;
    /* 0x174 */ u8 pad174[0x23];
    /* 0x197 */ u8 unk197;
    /* 0x198 */ u8 unk198;
    /* 0x199 */ u8 pad199;
    /* 0x19A */ u8 grenade_count;
    /* 0x19B */ u8 unk19B;
    /* 0x19C */ u8 pad19C[6];
    /* 0x1A2 */ u16 unk1A2;
    /* 0x1A4 */ u8 pad1A4[8];
    /* 0x1AC */ u8 unk1AC;
    /* 0x1AD */ u8 pad1AD[4];
    /* 0x1B1 */ u8 unk1B1;
    /* 0x1B2 */ u8 pad1B2;
    /* 0x1B3 */ u8 unk1B3;
} S31C;

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ s32 unk2C;
} Cam;

typedef struct Sub9C {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ f32 unk2C;
} Sub9C;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 padA[6];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 pad14[0x20];
} Ent34;
extern Ent34 *D_800D3098;

typedef struct Actor {
    /* 0x000 */ s32 interaction_state;
    /* 0x004 */ u8 id;
    /* 0x005 */ u8 pad5[0xF];
    /* 0x014 */ f32 x_position;
    /* 0x018 */ f32 y_position;
    /* 0x01C */ f32 z_position;
    /* 0x020 */ f32 y_velocity;
    /* 0x024 */ f32 gravity;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ f32 old_x_position;
    /* 0x030 */ f32 old_y_position;
    /* 0x034 */ f32 old_z_position;
    /* 0x038 */ u8 pad38[4];
    /* 0x03C */ f32 xz_velocity;
    /* 0x040 */ f32 unk40;
    /* 0x044 */ f32 unk44;
    /* 0x048 */ f32 unk48;
    /* 0x04C */ f32 animation_speed;
    /* 0x050 */ u8 pad50[4];
    /* 0x054 */ f32 unk54;
    /* 0x058 */ u8 pad58[0xD];
    /* 0x065 */ u8 unk65;
    /* 0x066 */ u8 pad66[0x10];
    /* 0x076 */ u16 unk76;
    /* 0x078 */ u16 unk78;
    /* 0x07A */ u16 unk7A;
    /* 0x07C */ u8 pad7C[3];
    /* 0x07F */ u8 unk7F;
    /* 0x080 */ u8 unk80;
    /* 0x081 */ u8 unk81;
    /* 0x082 */ u8 unk82;
    /* 0x083 */ u8 unk83;
    /* 0x084 */ u16 unk84;
    /* 0x086 */ u8 unk86;
    /* 0x087 */ u8 unk87;
    /* 0x088 */ u8 unk88;
    /* 0x089 */ u8 disable_run;
    /* 0x08A */ u8 disable_jump;
    /* 0x08B */ u8 pad8B[0x19];
    /* 0x0A4 */ u8 unkA4;
    /* 0x0A5 */ u8 padA5[3];
    /* 0x0A8 */ u8 unkA8;
    /* 0x0A9 */ u8 unkA9;
    /* 0x0AA */ u8 unkAA;
    /* 0x0AB */ u8 padAB[2];
    /* 0x0AD */ u8 in_water;
    /* 0x0AE */ u8 unkAE;
    /* 0x0AF */ u8 padAF[3];
    /* 0x0B2 */ s16 unkB2;
    /* 0x0B4 */ u8 padB4[4];
    /* 0x0B8 */ f32 unkB8;
    /* 0x0BC */ f32 unkBC;
    /* 0x0C0 */ f32 target_speed;
    /* 0x0C4 */ f32 unkC4;
    /* 0x0C8 */ u8 padC8[4];
    /* 0x0CC */ s16 unkCC;
    /* 0x0CE */ s16 unkCE;
    /* 0x0D0 */ u8 unkD0;
    /* 0x0D1 */ u8 padD1[0x13];
    /* 0x0E4 */ s16 unkE4;
    /* 0x0E6 */ u8 padE6[0xE];
    /* 0x0F4 */ s32 unkF4;
    /* 0x0F8 */ s32 unkF8;
    /* 0x0FC */ u8 padFC[4];
    /* 0x100 */ u8 unk100;
    /* 0x101 */ u8 pad101;
    /* 0x102 */ u8 unk102;
    /* 0x103 */ u8 pad103;
    /* 0x104 */ u8 stunned;
    /* 0x105 */ u8 unk105;
    /* 0x106 */ u8 pad106[5];
    /* 0x10B */ u8 unk10B;
    /* 0x10C */ s16 unk10C;
    /* 0x10E */ u8 pad10E[2];
    /* 0x110 */ u8 unk110;
    /* 0x111 */ u8 pad111[3];
    /* 0x114 */ f32 unk114;
    /* 0x118 */ f32 unk118;
    /* 0x11C */ f32 unk11C;
    /* 0x120 */ s16 unk120;
    /* 0x122 */ u8 pad122[3];
    /* 0x125 */ u8 immune;
    /* 0x126 */ u8 pad126[0x11];
    /* 0x137 */ u8 unk137;
    /* 0x138 */ u8 pad138[4];
    /* 0x13C */ u8 unk13C;
    /* 0x13D */ u8 pad13D[0xB];
    /* 0x148 */ f32 unk148;
    /* 0x14C */ u8 pad14C[0x16];
    /* 0x162 */ u8 unk162;
    /* 0x163 */ u8 pad163;
    /* 0x164 */ f32 unk164;
    /* 0x168 */ f32 unk168;
    /* 0x16C */ f32 unk16C;
    /* 0x170 */ f32 unk170;
    /* 0x174 */ u8 pad174[0xC];
    /* 0x180 */ f32 unk180;
    /* 0x184 */ s32 unk184;
    /* 0x188 */ s32 unk188;
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ u8 pad190[0x12];
    /* 0x1A2 */ u8 unk1A2[0x28];
    /* 0x1CA */ u8 health;
    /* 0x1CB */ u8 pad1CB;
    /* 0x1CC */ f32 unk1CC;
    /* 0x1D0 */ u8 pad1D0[0x14];
    /* 0x1E4 */ u8 unk1E4;
    /* 0x1E5 */ u8 pad1E5[0x33];
    /* 0x218 */ void *unk218;
    /* 0x21C */ u8 pad21C[8];
    /* 0x224 */ s16 unk224;
    /* 0x226 */ u8 pad226[2];
    /* 0x228 */ u8 unk228;
    /* 0x229 */ u8 pad229[9];
    /* 0x232 */ u8 unk232;
    /* 0x233 */ u8 pad233[6];
    /* 0x239 */ u8 unk239;
    /* 0x23A */ u8 pad23A[0x17];
    /* 0x251 */ u8 unk251;
    /* 0x252 */ u8 pad252[0xA6];
    /* 0x2F8 */ u16 unk2F8;
    /* 0x2FA */ u8 pad2FA[0x1E];
    /* 0x318 */ Cam *camera;
    /* 0x31C */ S31C *unk31C;
    /* 0x320 */ u8 pad320[0xC];
} Actor;

typedef struct {
    /* 0x0 */ u16 buttons;
    /* 0x2 */ s8 stick_x;
    /* 0x3 */ s8 stick_y;
    /* 0x4 */ u16 unk4;
} ContPad;

#define D_800CC284 (*(ContPad **)&D_800CC284)
extern u8 D_800B85A4[];
extern u16 D_8009917C[];
extern u8 D_800BEA0C;
extern f32 D_800CBD90;
extern f32 D_800CBD94;
extern f32 D_800CBD98;
extern s8 D_800CBDD3;
extern s8 D_800CBDD4;
extern f32 D_800CBDF4;
extern f32 D_800CBDF8;
extern f32 D_800CBDFC;
extern u8 D_800CC26C;
extern f32 D_800CC270;
extern f32 D_800CC274;
extern f32 D_800CC278;
extern u16 D_800CC27E;
extern s32 D_800CC28C;
extern s32 D_800CC290;
s32 func_1000F568(s32, s32);
void func_1502EA0C(Actor *, s32, s32, s32, s32, s32);
void func_1502EA50(Actor *);
void func_15044380(f32, f32, f32, Actor *, s32, s32);
void func_15055E50(Actor *, u8);
u8 func_150599C8(Actor *, u8, u16);
f32 func_1505A3A8(f32, Actor *, f32, f32, u8);
void func_1505A9AC(Actor *, f32);
void func_1505D024(Actor *, s32, u16, s32);
void func_1505D6F0(Actor *, u8);
void func_1505F298(Actor *, ContPad *);
void func_1506045C(Actor *, u16, s32, f32, s32);
void func_15063168(Actor *);
s32 func_1506E46C(Actor *, s32 *, s32);
void func_1507FEA0(Actor *);
void func_15087E54(u8, Actor *);
s32 func_150A3FC4(f32, f32, s32, f32 *, f32 *);
void func_150D5A6C(Actor *, s32, s32);
void func_150D7790(Actor *, s32);
void func_1510E8BC(s32 *, f32 *, f32 *, f32 *, s32 *, u8 *, f32, f32, f32, f32, s32, Actor *, f32, f32, s32);
void func_15121490(Actor *, ContPad *);
void func_151279A0(Actor *, ContPad *);
void func_151AE264(Actor *);

#define D_800CC280_ALT (((s32 *)&D_800CC27C)[1])
#define D_800CC2BC_ALT D_800CC2BC
static s32 D_800cc280; static s32 D_800cc2bc;
#define BLK typedef s32 blk_t
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
void func_15051558(Obj15051558 *arg0, f32 arg1, f32 arg2, u16 arg3);
void func_1505A770(struct127 *arg0);
void func_15052260(struct127 *arg0);
void func_1505250C(struct127 *arg0, s32 arg1);
s32 func_1505C7D8(struct127 *, u8);
void func_15059C84(struct127 *);
void func_1504CB98(Actor *arg0) {
    u16 sp1BE;
    u16 sp1BC;
    u8 padb1;
    u8 sp1BA;
    u8 sp1B9;
    f32 sp1B4;
    s16 temp_v0_17;
    u16 temp_a0_3;
    s32 vb34;
    f32 sp1A8;
    u8 padb2;
    s8 var_v1_3;
    s8 var_v1_4;
    u8 t3_id;
    u8 temp_v0_43;
    u8 temp_v0_5;
    u8 padb3;
    u8 padb4;
    s32 var_a1_2;
    s32 sp198;
    s8 sp197;
    f32 sp190;
    f32 sp18C;
    s32 sp188;
    s16 pad1;
    ContPad sp180;
    s32 sp17C;
    f32 sp178;
    s32 sp174;
    Ent34 *pE2;
    Ent34 *temp_a0_4;
    f32 tf13;
    u8 t3_4Eb;
    u8 t3_102;
    u8 padu1;
    u8 padu2;
    f32 vf12c;
    f32 var_f12;
    u32 sp158;
    f32 var_f14;
    f32 var_f18;
    f32 tf4;
    s32 var_a0;
    u16 vc34; u16 padvc;
    u16 var_a2_2; u16 padA2;
    s32 anim;
    u8 sg; u8 padsg1; u8 padsg2; u8 padsg3;
    f32 ang;
    u16 tmpi;
    u16 padx3;
    f32 tfA;
    f32 tfJ;
    Actor *sp124;
    f32 tf3;
    f32 tfF;
    f32 sp118;
    f32 sp114;
    f32 sp110;
    f32 sp10C;
    f32 sp108;
    f32 sp104;
    s32 sp100;
    f32 spFC;
    u16 angx;
    u16 padx1;
    s16 spF6;
    s16 pad10;
    u16 angt;
    u16 padx2;
    u16 dd;
    u16 padq1;
    u16 ua;
    s16 padq2;
    s32 idx;
    s32 t4E;
    f32 tf1;
    f32 spD8;
    f32 spD4;
    f32 spD0;
    f32 spCC;
    u8 spCB;
    f32 spC4;
    f32 spC0;
    f32 spBC;
    f32 spB8;
    s32 va_w3;
    s32 va_w4;
    s32 va_w5;
    f32 vf14b;
    struct Sub9C *p9C;
    u16 tlk;
    u16 t7A;
    f32 tf5;
    f32 tf6;
    f32 tf7;
    s16 sp92;
    s16 pad26;
    f32 tf8;
    f32 sp88;
    f32 sp84;
    s32 sp80;
    f32 tf9;
    f32 tf10;

    sp1B4 = 0.0f;
    sp198 = 0;
    sp197 = 0;
    D_800CC28C = (s32) D_800BE710[D_800C3E78];
    D_800CC290 = (s32) D_800CC284->buttons;
    D_800CC2B1 = 0;
    D_800CC2B2 = 0;
    D_800CC2B4 = 4.0f;
    sp1BE = arg0->unk76;
    sp17C = 0;
    sp180 = *D_800CC284;
    D_800CC2BC_ALT = 1;
    arg0->unk31C->unk1B3 = 0;
    if (arg0->unk31C->unk84 != 0) {
        if (arg0->stunned == 0) {
            func_15056B08((struct127 *) arg0);
        }
        sp180 = *D_800CC284;
        D_800CC284->stick_x = arg0->unk31C->unk85;
        D_800CC284->stick_y = arg0->unk31C->unk86;
        D_800CC284->buttons = arg0->unk31C->unk8A;
        D_800CC288 = (s32) arg0->unk31C->unk8C;
        sp188 = (s32) arg0->unk31C->unk8A;
        if (arg0->camera == NULL) {
            func_151279A0(arg0, D_800CC284);
            func_15121490(arg0, D_800CC284);
        }
        func_1504C854((struct127 *) arg0);
    } else {
        arg0->unk31C->unk8A = D_800CC284->buttons;
        D_800CC288 = (s32) D_800BE710[D_800C3E78];
        sp188 = (s32) arg0->unk31C->unk8A;
    }
    if ((D_80082FA0 >= (s32) D_800C3E78) && ((&D_800DDE3C)[D_800C3E78] != 0) && ((&D_800DDDC8)[D_800C3E78] > 0.75f) && (D_800E0B94 != 2)) {
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->buttons = 0;
        D_800CC288 = 0;
        sp17C = 1;
        D_800CC2BC_ALT |= 2;
    }
    if ((arg0->stunned != 0) && (arg0->health != 0) && ((((arg0->unk31C->unk26 & 1) != 0) && (D_800CC288 & 0x8000)) || ((arg0->unk31C->unk26 & 2) && (func_1505A5CC((struct49 *) D_800CC284) > 15.0f)))) {
        arg0->unk10C = 0;
        arg0->unk105 = 3;
        func_150585F0((struct127 *) arg0, 0.05f);
    }
    if (D_800BE616 != 0) {
        if ((s32) D_800BE9A0 < (s32) arg0->unk31C->unk1B1) {
            arg0->unk31C->unk1B1 -= D_800BE9A0;
            if (arg0->unkA4 != 4) {
                func_1502EA0C(arg0, 0xFF, 0xFF, 0xFF, 0xBF, 4);
            }
        } else if (arg0->unkA4 == 4) {
            arg0->unk31C->unk1B1 = 0;
            func_1502EA50(arg0);
        }
        if (((D_800BE9F0 == 0x2D) || (D_800BE9F0 == 0x30) || (D_800BE9F0 == 0x34) || (D_800BE9F0 == 0x1A) || (D_800BE9F0 == 0x3F)) && (D_800CC288 & 4) && !(D_800CC288 & 0xC000) && ((s32) arg0->unk28 == 0) && (arg0->y_velocity < 0.0f) && (arg0->disable_run == 0) && (arg0->unk13C == 0) && (arg0->unk31C->unk19B == 0) && (arg0->unk31C->unk78 == 0) && ((arg0->unk31C->unk75 & ~0x80) != 3) && (arg0->unk31C->grenade_count != 0)) {
            if ((arg0->id == 0x3B) || (D_800BE9F0 == 0x1A) || (D_800BE9F0 == 0x3F)) {
                arg0->unk31C->unk19B = 5;
            } else {
                arg0->unk31C->unk19B = 1;
            }
        }
    }
    arg0->unk31C->unk38 = (s32) D_800CC284->buttons;
    if (D_800C3638 != 0) {
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->buttons = 0;
        D_800CC288 = 0;
        D_800CC2BC_ALT |= 4;
    }
    func_1507FEA0(arg0);
    func_1504BB88((struct127 *) arg0);
    func_1504BA38((struct127 *) arg0);
    if ((arg0->unk228 != 0) || (arg0->unk2F8 & 0x10)) {
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->buttons = 0;
        D_800CC288 = 0;
        sp17C = 1;
        D_800cc2bc |= 8;
    }

    if (arg0->health == 0) {
        arg0->disable_run = 0x14;
        if ((arg0->stunned == 0) && (arg0->unk31C->unk120 == 0)) {
            if ((arg0->unk184 & 0x1F) == 0xE) {
                func_1507CD64(arg0, 0xD);
            } else if ((arg0->unk184 & 0x1F) == 0x13) {
                func_1507CD64(arg0, 0xD);
            } else {
                func_1507CD64(arg0, 0);
            }

        }
    }
    if (arg0->unk31C->unk78 != 0x1B) {
        if (((arg0->unk31C->unk198 == 0) || (arg0->unk31C->unk78 == 3)) && (arg0->unk31C->unk78 != 0x12) && (arg0->unk31C->unk78 != 2) && (arg0->unk31C->unk78 != 0x13) && (arg0->unk31C->unk78 != 0xA) && (arg0->unk31C->unk78 != 0x34)) {
            sp174 = (s32) arg0->unk31C->unk78;
            D_800CC2BC_ALT |= 0x10;
            sp1BC = func_1505A630((f32) D_800CC284->stick_x, (f32) D_800CC284->stick_y, 0);
            sp1B4 = func_1505A5CC((struct49 *) D_800CC284);
            t4E = arg0->unk31C->unk4E;
            if (t4E == 2) {
                temp_v0_17 = D_800CC284->stick_x * D_800CC264;
                tfA = (f32) D_800CC284->stick_y;
                tfA *= 0.6f;
                if (tfA > 45.0f) {
                    tfA = 45.0f;
                }
                if (tfA < -35.0f) {
                    tfA = -35.0f;
                }
                if (arg0->unk31C->unk50 > 0) {
                    arg0->unk31C->unk50 = arg0->unk31C->unk50 - D_800BE9A0;
                    temp_v0_17 = (s16) (temp_v0_17 / 3);
                    arg0->disable_jump = 6;
                    tfA -= 60.0f;
                } else if ((arg0->unk28 > 100.0f) || (arg0->unk83 != 0)) {
                    temp_v0_17 = (s16) (temp_v0_17 / 2);
                }
                sp1B4 = 135.0f + tfA;
                arg0->unk239 = 0x29;
                if (arg0->unk31C->unk84 == 0) {
                    if (arg0->disable_run == 0) {
                        arg0->unk76 -= temp_v0_17 / 9;
                    } else {
                        sp1B4 = 0.0f;
                    }
                    func_15087E54(D_800C3E78, arg0);
                    sp1BC = arg0->unk76 - D_800CC280;
                }
                t4E = arg0->unk31C->unk4E;
            }
            if (t4E == 0x81) {
                ua = 0x6000;
                temp_v0_17 = D_800CC284->stick_x * D_800CC264;
                arg0->unk76 -= temp_v0_17 / 8;
                if (D_800BE9F0 == 6) {
                    sp1B4 = 42.0f;
                    ua = func_1505A630(2540.0f - arg0->x_position, arg0->z_position - -160.0f, 0);
                    padq1 = 0xF000;
                    var_a2_2 = 0x8000;
                } else {
                    padq1 = 0xB000;
                    var_a2_2 = 0x2000;
                    sp1B4 = 65.0f;
                }
                dd = (arg0->unk76 - ua);
                if (padq1 < dd) {
                    arg0->unk76 = ua + padq1;
                }
                if (dd < var_a2_2) {
                    arg0->unk76 = ua + var_a2_2;
                }
                sp1BC = arg0->unk76 - D_800CC280;
                arg0->unk78 = arg0->unk76;
                { BLK; arg0->unk7A = arg0->unk76; }
            }
            if (arg0->unk31C->unk197 != 0) {
                sp1B4 = 0.1f;
                arg0->unk7A = (s32) (arg0->unk31C->unk16C * 182.04445f);
                sp1BC = arg0->unk7A - D_800CC280;
                if (arg0->unk31C->unk84 != 0) {
                    arg0->unk76 = arg0->unk7A;
                }
                tf1 = arg0->unk31C->unk170;
                if (tf1 > 180.0f) {
                    tf1 -= 360.0f;
                }
                tf1 *= 0.91022223f;
                sp197 = (s8) (s32) tf1;
                if ((D_800BE616 != 0) && ((t3_id = arg0->id, (t3_id == 0x3B)) || (t3_id == 0x80)) && ((sp174 == 0x18) || (sp174 == 0x41) || (sp174 == 0x23) || (sp174 == 0x24)) && (D_800CC284->buttons & 0x2000) && ((arg0->unk31C->unk109 == 3) || (arg0->unk31C->unk109 == 2))) {
                    sp158 = 0;
                    if ((s32) arg0->unk31C->unkAF < 0x3C) {
                        sp158 = 0x30;
                    }
                    if (func_10010894((struct127 *) arg0) == 0) {
                            if (sp158 >= (u32) (func_150ADA20() & 0x5F)) {
                            if (arg0->id == 0x80) {
                                if ((sp174 == 0x23) && !(func_150ADA20() & 7)) {
                                    func_15060A30(0x586, arg0);
                                } else {
                                    var_a1_2 = (sp174 == 0x23) ? 4 : 5;
                                    func_15060A30(func_1000F568(0x574, var_a1_2), arg0);
                                }
                            } else if ((D_800CC284->buttons & 8) && !(func_150ADA20() & 1)) {
                                func_15060A30(0x532, arg0);
                            } else {
                                if ((arg0->unk31C->unk109 == 3) && (arg0->unk31C->unk108 == 0xA) && !(func_150ADA20() & 3)) {
                                    func_15060A30(0x533, arg0);
                                } else {
                                    func_15060A30((func_150ADA20() & 3) + 0x534, arg0);
                                }
                            }
                        }
                    }
                    arg0->unk31C->unkAF = 0x78;
                }
                if (arg0->unk31C->unkAF != 0) {
                    if (D_800BE9E4 < (s32) arg0->unk31C->unkAF) {
                        arg0->unk31C->unkAF = arg0->unk31C->unkAF - D_800BE9E4;
                    } else {
                        arg0->unk31C->unkAF = 0;
                    }
                }
                { BLK; }
            } else {
                arg0->unk31C->unkAF = 0;
            }
            if (arg0->unk31C->unk48 > 0) {
                arg0->unk31C->unk48 = arg0->unk31C->unk48 - D_800CC264;
                sp1BC = arg0->unk76 - D_800CC280;
            }
            if (arg0->unk31C->unk46 > 0) {
                arg0->unk31C->unk46 = arg0->unk31C->unk46 - D_800CC264;
                if (D_800CC288 & 0xC000) {
                    if (arg0->unk28 == 0.0f) {
                        D_800D1580 = -0xFCFE90;
                        arg0->xz_velocity *= 0.4f;
                        arg0->y_velocity = 10.0f;
                        arg0->gravity = 5.0f;
                    }
                    arg0->unk31C->unk46 -= 0x190;
                }
                if (arg0->disable_run == 0) {
                    ang = (f32) ((D_800CC250 * 0xC) & 0xFF) * 0.024543693f;
                    if (sp1B4 < 5.0f) {
                        sp1BC = arg0->unk76 - D_800CC280;
                    }
                    sp1B4 = 28.0f;
                    arg0->unk31C->unk3C = 4;
                    tf1 = func_150AD78C(ang) * 10240.0f;
                    arg0->unk31C->unkB += 0x10;
                    arg0->unk31C->unk10 = (s16) (s32) tf1;
                }
            } else {}
            if ((arg0->unk84 == 0xBF) || (arg0->unk84 == 0xC1)) {
                sp197 = (s8) ((s8) D_800CC284->stick_y / 2);
            }
            if (arg0->unk31C->unk78 == 0x38) {
                sp197 -= (s32) arg0->unk31C->unk1A2 >> 8;
            }
            { BLK; }
        } else {
            sp1BC = 0;
            sp1B4 = 0.0f;
            D_800cc2bc |= 0x20;
            if ((arg0->unk31C->unk78 == 2) || (arg0->unk31C->unk78 == 0x12) || (arg0->unk31C->unk78 == 0x13) || (arg0->unk31C->unk78 == 0x10) || (arg0->unk31C->unk78 == 0xA) || (arg0->unk31C->unk78 == 0x34) || ((D_800CC284->buttons & 0x10) && (arg0->unk31C->unk78 == 0x16)) || (arg0->unk31C->unk78 == 9) || (arg0->unk31C->unk78 == 0x38) || (arg0->unk31C->unk78 == 0x39) || (arg0->unk31C->unk78 == 0x37) || (arg0->unk31C->unk78 == 0x3B) || (arg0->unk31C->unk78 == 0x15) || (arg0->unk31C->unk78 == 0x26) || (arg0->unk31C->unk78 == 0x3A)) {
                if ((arg0->unk31C->unk78 == 0x10) || (arg0->unk31C->unk78 == 0x14) || (arg0->unk31C->unk78 == 0x3F)) {
                    sp197 = (s8) ((s8) D_800CC284->stick_y / 2);
                }
                if ((arg0->unk31C->unk78 == 0xC) || (arg0->unk31C->unk78 == 0x16) || (arg0->unk31C->unk78 == 0x15) || (arg0->unk31C->unk78 == 0x38) || (arg0->unk31C->unk78 == 0x39) || (arg0->unk31C->unk78 == 0x37) || (arg0->unk31C->unk78 == 0x3B) || (arg0->unk31C->unk78 == 0x12) || (arg0->unk31C->unk78 == 9)) {
                    tf3 = arg0->unk31C->unk170;
                    if (tf3 > 180.0f) {
                        tf3 -= 360.0f;
                    }
                    tf3 *= 0.91022223f;
                    *(u8 *)&sp197 = (s32) tf3;
                    if (arg0->unk31C->unk78 == 0x38) {
                        sp197 -= (s32) arg0->unk31C->unk1A2 >> 8;
                    }
                }
                if ((arg0->unk31C->unk78 != 9) && (arg0->unk31C->unk78 != 0x38) && (arg0->unk31C->unk78 != 0x39) && (arg0->unk31C->unk78 != 0x37) && (arg0->unk31C->unk78 != 0x3B) && (arg0->unk31C->unk78 != 0x15) && (arg0->unk31C->unk78 != 0x26) && (arg0->unk31C->unk78 != 0x3A)) {
                    arg0->unk7A = (s32) ((arg0->unk31C->unk16C + 90.0f) * 182.04445f);
                    arg0->unk76 = arg0->unk7A;
                }
            } else if ((arg0->unk31C->unk78 != 0x14) && (arg0->unk31C->unk78 != 0x3F)) {
                D_800CC2BC_ALT |= 0x100;
                D_800CC284->buttons &= ~0x2000;
                D_800CC288 &= ~0x2000;
            }
            if (arg0->unk31C->unk78 != 9) {
                D_800CC2BC_ALT |= 0x200;
                D_800CC284->buttons &= ~0x8000;
                D_800CC288 &= ~0x8000;
            }
        }
    }
    if (!(arg0->unk31C->unk4E & 0xF) && (arg0->health != 0) && (arg0->stunned == 0) && (arg0->unk28 == 0.0f) && ((D_800BE616 != 0) || ((arg0->unk31C->unk78 == 9) && (arg0->unk31C->matrix_physics == 0)) || (arg0->unk31C->unk78 == 0x16) || (arg0->unk31C->unk78 == 0x3B)) && ((arg0->unk31C->unk11A == 2) || (arg0->unk31C->unk11A == 0))) { BLK;
        va_w3 = ((arg0->unk31C->unk78) && ((arg0->unk31C->unk197 == 0) || (arg0->unk31C->unk78 == 0x16)) && (arg0->unk31C->unk19B == 0)) ? 0xF : 3;
        if ((D_800CC284->buttons & va_w3) != 0) {
            va_w3 = (D_800CC284->buttons & va_w3);
            if ((arg0->unk31C->unk197 == 0) && (arg0->unk31C->unk198 == 0)) {
                if (D_800CC284->stick_y >= 6) {
                    va_w3 |= 8;
                } else if (D_800CC284->stick_y < -5) {
                    va_w3 |= 4;
                }
            }
            tlk = D_8009917C[va_w3];
            arg0->unk31C->unk52 = 2;
            if (tlk != 0xFFFF) {
                if ((arg0->unkAA == 0) || ((s32) arg0->unkAA >= 0x42)) {
                    arg0->unkAA = 0x41;
                }
                if (arg0->xz_velocity > 12.0f) {
                    arg0->unk31C->unk3C = 4;
                }
                if (arg0->unk76 != arg0->unk7A) {
                    arg0->unk31C->unk3C = 0;
                }
                arg0->unk7A = arg0->unk7A - ((s32) (D_800CC284->stick_x * D_800CC264) >> 4);
                sp1BC = ((arg0->unk7A + tlk) - arg0->unk31C->unk12) - D_800CC280;
                sp1B4 = 34.0f;
                if (arg0->unk31C->unk197 != 0) {
                    arg0->unk76 = sp1BC + D_800CC280;
                }
                arg0->unk31C->unk1B3 = 1;
            } else {
                arg0->xz_velocity = 0.0f;
            }
        } else if (arg0->unk31C->unk52 != 0) {
            arg0->unk76 = arg0->unk7A;
            if (arg0->unk31C->unk197 != 0) {
                arg0->xz_velocity = 0.0f;
            }
        }
        
    }
    if (arg0->unk31C->unk23 > 0) {
        arg0->disable_run = 0;
        arg0->unk83 = 0;
        sp1B4 = 18.0f;
        sp1BC = arg0->unk76 - D_800CC280;
        D_800CC284->stick_x = 0;
        D_800CC284->stick_y = 0;
        D_800CC284->buttons = 0;
        D_800CC288 = 0;
        sp17C = 1;
        arg0->unk31C->unk23 -= D_800BE9A0;
        D_800cc2bc |= 0x40;
    }
    if (arg0->unk31C->unk6B != 0) {
        func_15052260((struct127 *) arg0);
    }
    sp1A8 = arg0->unk118 - arg0->unk11C;
    if (fabsf(sp1A8) > 30.0f) {
        sp1A8 = 0.0f;
    }
    arg0->unk11C = arg0->unk118;
    if (arg0->unk31C->unk84 == 0) {
        arg0->unk31C->unk8A = D_800CC284->buttons;
        arg0->unk31C->unk8C = (u16) D_800CC288;
    }
    if ((arg0->unk84 == 0) && (arg0->animation_speed == 0.0f)) {
        func_1505E650((struct127 *) arg0, 0x24U, 1.0f, 1.0f, 0.0f, 0.0f, 0);
    }
    if (D_800CC284->stick_y == 0) {
        arg0->unk31C->unk53 = 0;
    }
    if (arg0->unk137 == 0xFF) {
        arg0->unk137 = 0;
    }
    sp1B9 = 0xC;
    if ((arg0->unk31C->unk8 > 0) || (arg0->unk31C->unk78 == 3)) {
        func_1504BE2C((struct127 *) arg0, &sp1BC, &sp1B4, &sp197);
    }
    func_1504C9E4((struct127 *) arg0, sp197, arg0->unk31C->unk78);
    if (arg0->stunned == 0) {
        if (arg0->unk31C->unk84 != 0) {
            sp1BC = arg0->unk78;
            sp1B4 = arg0->unk44;
        }
        if (sp1B4 > 0.0f) { BLK;
            vc34 = 0; va_w4 = 0; vb34 = 0; arg0->unk31C->unk4C = sp1BC + D_800cc280;
            if (arg0->unk31C->unk32 & 2) {
                vc34 = arg0->unk31C->unk34;
                va_w5 = arg0->unk31C->unk4C;
                va_w4 = arg0->unk31C->unk34 - va_w5;
                if (!((va_w4 + 0x2000) & 0xC000)) {
                    arg0->unk31C->unk4C = arg0->unk31C->unk34;
                    vc34 = arg0->unk31C->unk34;
                    va_w5 = arg0->unk31C->unk4C;
                    va_w4 = vc34 - va_w5;
                }
                if (!((va_w4 + 0xA000) & 0xC000)) {
                    arg0->unk31C->unk4C = vc34 + 0x8000;
                    va_w5 = arg0->unk31C->unk4C;
                }
                sp1BC = va_w5 - D_800cc280;
            }
            goto l1374; }
            arg0->unk31C->unk4C = arg0->unk7A;
        l1374:
        if (arg0->in_water != 0) {
            tf4 = func_1504B0FC((Obj1504B0FC *) arg0, sp1A8);
            sp1BC = arg0->unk31C->unk4C;
            if (arg0->unk31C->unk22 >= 0) {
                arg0->unk31C->unk22 = arg0->unk31C->unk22 - D_800BE9A0;
                if ((arg0->unk31C->unk22 >= 0x5B) && (arg0->unk31C->unk22 < 0x64)) {
                    arg0->unk31C->unk22 = 0;
                }
            }
            if (arg0->unk31C->unk21 == 0) {
                sp1B4 = tf4;
                sp1BC = arg0->unk76;
                if (!(D_800CC284->buttons & 0x4000)) {
                    if ((arg0->unk31C->unk22 <= 0) && (((arg0->unk118 - 60.0f) - 30.0f) < arg0->y_position)) {
                        arg0->unk31C->unk21 = 1;
                        arg0->disable_run = 0xC;
                    }
                }
            } else {
                if ((D_800CC288 & 0x4000) && (D_800BE616 == 0) && (D_800BE9F0 != 0x29)) {
                    if ((arg0->camera != NULL) && (arg0->camera->unk2C != 0x100) && !(arg0->unk31C->unk20 & 1)) {
                        arg0->unk31C->unk21 = 0;
                        arg0->unk31C->unk22 = 0x76;
                        if (((arg0->unk118 - 60.0f) - 30.0f) < arg0->y_position) {
                            arg0->unkB8 = 20.0f;
                        }
                        func_15060A30(0x66F, arg0);
                    }
                }
                arg0->unk31C->unk3C = 4;
                {
                BLK;
                arg0->unkAA = 0x28;
                }
            }
        } else {
            sp1BC += D_800cc280;
        }
        D_800CC27E = sp1BC;
        arg0->unk44 = sp1B4;
        if (arg0->unkAA != 0) {
            sp1B4 *= (f32)(u32)arg0->unkAA * 0.01f;
        }
        if ((s32) arg0->unk13C >= 0x64) {
            sp1B4 *= 0.8f;
        }
        if ((arg0->disable_run != 0) || ((s32) arg0->unk81 >= 0x79)) {
            sp1B4 = 0.0f;
        }
        func_1504BC38((struct127 *) arg0);
        tfF = 4.0f;
        var_f14 = 0.0f;
        if ((arg0->in_water != 0) && (arg0->unk31C->unk21 == 0)) {
            if ((arg0->stunned == 0) && (arg0->unk84 != 0x262)) {
                var_f14 = (f32) ((s8) D_800CC284->stick_x / 2);
            }
        } else {
            if ((arg0->unk31C->unk78 == 2) || (arg0->unk31C->unk78 == 0x13) || (arg0->unk31C->unk78 == 0xA) || (arg0->unk31C->unk78 == 0x34)) {
                if (arg0->unk31C->unk198 != 0) {
                    tf5 = -arg0->unk31C->unk170;
                    if ((arg0->unk31C->unk78 == 2) || (arg0->unk31C->unk78 == 0x13)) {
                        tf5 = tf5 + 30.0f;
                    } else {
                        tf5 = -tf5;
                    }
                    { BLK; var_f14 = (f32) (s8) (s32) tf5; }
                } else {
                    var_f14 = (f32) ((s8) D_800CC284->stick_y / 2);
                }
            } else {
                if (arg0->disable_run != 0xFF) {
                    temp_v0_17 = sp1BC - sp1BE;
                    if ((arg0->unk28 < 20.0f) && (arg0->xz_velocity < 1.5f)) {
                        var_f14 = arg0->unk44 * 0.3f;
                    } else {
                        var_f14 = arg0->unk44 * 1.75f;
                        tf6 = fabsf((f32) temp_v0_17 * 0.00390625f * 5.0f);
                        if (tf6 > 50.0f) {
                            tf6 = 50.0f;
                        }
                        if (tf6 < var_f14) {
                            var_f14 = tf6;
                        }
                        if (arg0->unk28 > 30.0f) {
                            var_f14 *= 0.6f;
                        }
                        if (temp_v0_17 >= 0) {
                            var_f14 = 0.0f - var_f14;
                        }
                    }
                }
                if (((arg0->unk13C != 0) && ((&((Actor *)D_800CC2D0)[D_800D154C->unk13C - 100])->id != 0x57)) || (arg0->unk31C->unk198 != 0) || (D_800CC284->buttons & 0x2000)) {
                    var_f14 = 0.0f;
                }
            }
        }
        if (arg0->unk31C->unk17 != 0) {
            if (arg0->unk84 == 0x279) {
                if (arg0->disable_run == 0) {
                    if (D_800CC284->buttons & 0x8000) {
                        arg0->gravity = 1.1f;
                    } else {
                        if (arg0->y_velocity > 0.0f) {
                            arg0->y_velocity = arg0->y_velocity * 0.85f;
                        }
                        arg0->gravity = 2.5f;
                    }
                }
                if (arg0->unk28 > 0.0f) {
                    arg0->unkC4 = arg0->unkC4 + (((arg0->unk54 * 4.0f) - arg0->unkC4) * 0.15f);
                    arg0->unkB8 = arg0->xz_velocity * 1.5f;
                }
                arg0->unkAA = 0x3C;
                arg0->unk80 = 1;
                arg0->unk83 = 0xFF;
            }
            var_f14 *= 0.6f;
            tfF = 12.0f;
        }
        if ((arg0->unk110 == 2) || (arg0->unk84 == 0xCD)) {
            var_f14 = -var_f14;
        }
        var_f14 = (var_f14 - arg0->unk54) / (tfF / *D_800D1550);
        if ((arg0->unk28 < 30.0f) || (arg0->unk31C->unk17 != 0)) {
            if (((var_f14 > 0.0f) && (arg0->unk31C->unk40 < 0.0f)) || ((tf7 = arg0->unk31C->unk40, (var_f14 < 0.0f)) && (tf7 > 0.0f))) {
                var_f14 = 0.0f;
            } else if (fabsf(var_f14 - tf7) > 2.0f) {
                if (tf7 < var_f14) {
                    var_f14 = tf7 + 2.0f;
                } else {
                    var_f14 = tf7 - 2.0f;
                }
            }
        }
        arg0->unk31C->unk40 = var_f14;
        if ((D_800BEAC0 == 0) && (D_800BEA0C == 0)) {
            arg0->unk54 += var_f14;
        }
        if (sp1B4 != 0.0f) {
            if (sp1BC != arg0->unk76) {
                if (arg0->xz_velocity < 5.0f) {
                    var_v1_3 = 0x19;
                } else {
                    var_v1_3 = (s8) ((s32) ((64.0f - arg0->xz_velocity) * 0.25f));
                    if ((s32) arg0->unk31C->unk31 >= 3) {
                        var_v1_3 += 1;
                    } else if ((s32) arg0->unk81 > 0) {
                        var_v1_3 = (s8) (s32) ((51.0f - arg0->xz_velocity) / 13.0f);
                        if (arg0->y_velocity < -10.0f) {
                            var_v1_3 = 0;
                        }
                    } else {}
                }
                sp1BA = ((s32) (sp1BC - arg0->unk76) >> 8); sg = sp1BA & 0x80;
                var_a0 = sp1BA;
                if (sp1BA >= 0x80) {
                    sp1BA = -sp1BA; var_a0 = sp1BA;
                }
                if (arg0->unk102 != 0) {
                    var_v1_3 = 5;
                } else if ((var_a0 >= 0x51) && (arg0->in_water == 0)) {
                    if ((arg0->unk31C->unk46 <= 0) && (arg0->unk31C->unk8 <= 0)) {
                        if ((arg0->unk31C->unk4E & 0xF) == 1) {
                            if (arg0->xz_velocity > 7.0f) {
                                var_v1_3 = 0;
                                arg0->xz_velocity = arg0->xz_velocity - 3.5f;
                            } else {
                                var_v1_3 = 6;
                            }
                        } else if (arg0->xz_velocity < 7.0f) {
                            if (arg0->xz_velocity > 3.0f) {
                                var_v1_3 = 0xC;
                                arg0->xz_velocity = 0.0f;
                                if ((s32) arg0->unk83 >= 5) {
                                    if ((arg0->unk31C->unk11A != 1) && (arg0->unk31C->unk11A != 3)) {
                                        arg0->unk83 = 4;
                                    }
                                }
                            }
                        } else {
                            sp1B4 = 0.0f;
                            var_v1_3 = 0;
                            if (arg0->unkAE == 0) {
                                arg0->xz_velocity -= 2.0f;
                            }
                        }
                    }
                }
                var_v1_3 = (s8) ((s32) (var_v1_3 * D_800CC264) / 100);
                if ((s32) arg0->unk13C >= 0x64) {
                    var_v1_3 = (s8) ((s32) (var_v1_3 * 2) / 3);
                } else if (arg0->unk110 != 0) {
                    var_v1_3 = 2;
                }
                if (arg0->in_water != 0) {
                    var_v1_3 = (s8) (var_v1_3 / 4);
                }
                if (arg0->unk31C->unk3C != 0) {
                    var_v1_3 = (s8) ((s32) (arg0->unk31C->unk3C * D_800CC264) / 100);
                }
                if (var_a0 < var_v1_3) {
                    var_v1_3 = var_a0;
                }
                if (sg) {
                    arg0->unk76 = arg0->unk76 - (var_v1_3 << 8);
                } else {
                    arg0->unk76 = arg0->unk76 + (var_v1_3 << 8);
                }
            }
        }
        arg0->unk31C->unk3C = 0;
        if (arg0->unk110 != 0) {
            sp1B9 = 0x10;
            if (arg0->unk110 & 1) {
                sp1BC = arg0->unk31C->unk3E;
            } else {
                sp1BC = arg0->unk31C->unk3E ^ 0x8000;
            }
        } else if ((sp1B4 == 0.0f) || (arg0->in_water != 0) || (arg0->unk31C->unk17 != 0) || (arg0->unk28 < 50.0f)) {
            sp1BC = arg0->unk76;
        }
        sp1BA = 0;
        if (arg0->unk31C->matrix_physics != 0) {
            sp1BC = 0x4000;
            { BLK; sp1B9 = 0x14; }
        } else if ((arg0->unk31C->unk17 != 0) && (arg0->unk13C == 0)) {
            sp124 = func_15083E90(6);
            if (sp124 != NULL) {
                if (func_1505A6F8(sp124, (struct127 *) arg0) < 600.0f) {
                    sp1BC = func_1505A630(sp124->x_position - arg0->x_position, arg0->z_position - sp124->z_position, 0);
                } else {
                    arg0->unk31C->unk3C = 5;
                }
            }
        } else { }
        if (arg0->unk31C->unk52 == 0) {
            sp1BA = func_150599C8(arg0, sp1B9, sp1BC);
        }
        if ((sp1BA >= 0x5B) && (arg0->xz_velocity > 12.0f) && (arg0->in_water == 0) && (arg0->unk81 == 0)) {
            sp1B4 = 0.0f;
        }
        if (arg0->unk76 == arg0->unk7A) {
            arg0->unk80 = 0;
        }
        tfJ = 0.0f;
        t4E = arg0->unk31C->unk4E; if ((t4E & 0xF) == 1) {
            if (arg0->unk31C->unk4F != 0) {
                tfJ = 0.27f;
            } else {
                tfJ = 0.8f;
                if (t4E == 0x81) {
                    tfJ = 0.2f;
                }
            }
            var_f14 = 0.65f;
        } else {
            var_f14 = 2.5f;
            if (arg0->unk84 == 0x16) {
                var_f14 = 3.2f;
                if (D_800CC284->buttons & 0x2000) {
                    sp1B4 = 0.0f;
                }
            } else if (arg0->unk13C != 0) {
                if (arg0->id == 0x13) {
                    var_f14 = 0.75f;
                }
                tfJ = 0.75f;
            } else if (arg0->unk110 != 0) {
                sp1B4 = 0.0f;
                var_f14 = 2.0f;
            } else {
                if (arg0->unk31C->unk46 > 0) {
                    tfJ = 4.0f;
                } else {
                    tfJ = 1.0f;
                }
            }
            if (arg0->unk31C->unk8 > 0) {
                tfJ = 1.0f;
                var_f14 = 2.0f;
            }
        }
        if (arg0->unk31C->matrix_physics != 0) {
            var_f14 = 0.0f;
        }
        func_1505A3A8(sp1B4, arg0, tfJ, var_f14, sp1BA);
        func_1505F298(arg0, D_800CC284);
    } else {
        if (arg0->in_water != 0) {
            func_1504B0FC((Obj1504B0FC *) arg0, sp1A8);
        }
        if (arg0->stunned != 0xFF) {
            func_1506045C(arg0, D_800CC284->buttons, D_800CC288, arg0->unk28, 0);
            arg0->unk54 = 0.0f;
            arg0->disable_run = 0;
            arg0->unk83 = 0;
            if (((arg0->unk84 == 0xF1) || (arg0->unk84 == 0xF6)) && (arg0->unk28 == 0.0f)) {
                arg0->y_velocity = 0.0f;
                arg0->gravity = 0.0f;
                arg0->unkF8 |= 0x01008000;
            }
            if (!(arg0->unk10B & 1) && (arg0->xz_velocity < 2.0f) && ((s32) arg0->stunned < 0xD2) && (arg0->unk28 < 10.0f) && (arg0->y_velocity < 0.5f)) {
                arg0->stunned = 1;
                arg0->unk10C = 1;
                arg0->unk105 = 3;
            }
            func_150585F0((struct127 *) arg0, 0.1f);
            if (arg0->unk10B & 0x40) {
                tf8 = arg0->y_position - D_800CC278;
                if (tf8 < 400.0f) {
                    if (tf8 < 10.0f) {
                        tf8 = 10.0f;
                    }
                    arg0->unk76 += (s32) (D_800CC264 * 0x1400) / 100;
                    arg0->y_velocity = (tf8 * 0.06f) + 6.0f;
                    arg0->xz_velocity = 3.0f;
                    func_1505A184(arg0->unk76, tf8, 0.0f, &sp118, &sp114, &sp110);
                    arg0->unk31C->unk28 = sp118 + D_800CC270;
                    arg0->unk31C->unk2C = sp114 + D_800CC274;
                    arg0->unk31C->unk30 = 2;
                } else {
                    arg0->unk10B = arg0->unk10B & 0xFFBF;
                    arg0->xz_velocity = 30.0f;
                    arg0->y_velocity = 90.0f;
                    arg0->gravity = 4.0f; { BLK; }
                }
            }
            if (arg0->unk10B & 0x80) {
                arg0->unk228 = 0;
                arg0->immune = 0x5A;
                if ((arg0->unk28 == 0.0f) && (arg0->y_velocity < 24.0f)) {
                    arg0->unk10C = 0;
                    if (arg0->unk10B & 0x20) {
                        arg0->unk31C->unk46 = 0xFA0;
                    }
                }
                if (sp1B4 > 5.0f) {
                    sp1BC += D_800CC280;
                    arg0->unk80 = 1;
                    func_150599C8(arg0, 6U, sp1BC);
                    func_1505A184(sp1BC, sp1B4, 0.0f, &sp10C, &sp108, &sp104);
                    arg0->unk16C = arg0->unk16C + ((sp10C - arg0->unk16C) * 0.1000000015f);
                    arg0->unk170 = arg0->unk170 + ((sp108 - arg0->unk170) * 0.1000000015f);
                    arg0->unk76 = arg0->unk7A;
                }
            }
        }
    }
    if ((arg0->unk31C->unk98 != 0) && (arg0->stunned == 0)) {
        spF6 = D_800CC284->stick_x;
        var_f12 = (f32) D_800CC284->stick_y;
        arg0->unk31C->unk31 = 0;
        arg0->unk54 = 0.0f;
        arg0->disable_run = 0xFF;
        arg0->xz_velocity *= 0.95f;
        arg0->y_velocity = 0.0f;
        arg0->gravity = 0.0f;
        arg0->unk81 = 1;
        if ((arg0->camera != NULL) && (arg0->camera->unk2C == 0x100)) {
            var_f12 = 0.0f;
            spF6 = 0;
        }
        spFC = var_f12;
        if ((var_f12 == 0.0f) || (var_f12 < -40.0f)) {
            spFC = 40.0f;
            sp100 = 0xA4;
            if (var_f12 < -40.0f) {
                var_f12 *= 2.0f;
            }
        } else {
            sp100 = 0x16A;
            if (var_f12 > 0.0f) {
                sp100 = 0x98;
            } else {
                spFC = -var_f12;
            }
        }
        p9C = arg0->unk31C->unk9C;
        var_f18 = p9C->unk2C;
        var_f18 = 0.08f / var_f18;
        arg0->unk31C->unkA0 += var_f12 * var_f18 * *D_800D1550;
        if (arg0->unk31C->unkA0 > 1.0f) {
            arg0->unk31C->unkA0 = 1.0f;
        } else if (arg0->unk31C->unkA0 < 0.0f) {
            func_151AE264(arg0);
            arg0->gravity = 5.0f;
        }
        arg0->unk1CC = arg0->y_position;
        arg0->unk31C->unk54 = 8;
        temp_a0_3 = arg0->unk76 + ((s32) (spF6 * D_800CC264) / 5);
        arg0->unk78 = temp_a0_3;
        arg0->unk7A = temp_a0_3;
        arg0->unk76 = temp_a0_3;
        func_1505E650((struct127 *) arg0, sp100, spFC * 0.025f, 6.0f, 0.0f, 0.0f, 0);
    }
    if ((arg0->unk31C->unk95 != 0) && (arg0->stunned == 0)) {
        if ((arg0->camera != NULL) && (arg0->camera->unk2C != 0x100)) {
            idx = (arg0->unk31C->unk95 & ~0x80) - 1;
            tf9 = (f32) D_800CC284->stick_y;
            temp_a0_4 = &D_800D3098[idx];
            if ((arg0->in_water != 0) && (arg0->y_position < arg0->unk118)) {
                arg0->unk31C->unk96 = 8;
            }
            if (arg0->unk31C->unk96 != 0) {
                arg0->unk31C->unk96 = arg0->unk31C->unk96 - 1;
                tf9 = 60.0f;
            }
            arg0->unk31C->unk28 = (f32) temp_a0_4->unk0;
            arg0->unk31C->unk2C = (f32) temp_a0_4->unk4;
            arg0->unk31C->unk30 = 0xA;
            arg0->unk31C->unk31 = 0;
            arg0->xz_velocity = 0.0f;
            arg0->unk54 = 0.0f;
            arg0->disable_run = 0xFF;
            arg0->gravity = 0.0f;
            arg0->unk81 = 1;
            arg0->unk80 = 1;
            arg0->unk76 = (s32) (temp_a0_4->unk10 * 182.04445f) - 0x4000;
            arg0->y_velocity = tf9 * 0.14f;
            if (arg0->unk31C->unk97 != 0) {
                arg0->y_velocity = 0.0f;
            }
            arg0->unk1CC = arg0->y_position;
            arg0->unk31C->unk54 = 8;
        }
    }
    if (arg0->unk31C->unk6B == 0) {
        func_15059C84((struct127 *) arg0);
        if (arg0->unk31C->unk30 > 0) {
            arg0->unk31C->unk30 = arg0->unk31C->unk30 - D_800BE9A0;
            arg0->x_position = arg0->x_position + ((arg0->unk31C->unk28 - arg0->x_position) * 0.2f);
            arg0->z_position = arg0->z_position + ((arg0->unk31C->unk2C - arg0->z_position) * 0.2f);
        }
        func_1502178C((struct127 *) arg0, 0, -1);
        func_1505A770((struct127 *) arg0);
        func_1505D6F0(arg0, D_800C3E78);
        if ((D_800CC26C != 0) && (arg0->unk31C->unk8 <= 0) && (arg0->unk13C == 0)) {
            temp_a0_3 = func_1505A630((&((Actor *)D_800CC2D0)[D_800CC26C])->x_position - arg0->x_position, arg0->z_position - (&((Actor *)D_800CC2D0)[D_800CC26C])->z_position, 0);
            
            if ((arg0->unk28 == 0.0f) && (sp1B4 > 15.0f) && (((D_800CC27E - temp_a0_3) - 0x4000) & 0x8000)) { BLK;
                spCC = 0.1f;
                spCB = 1;
                if ((&((Actor *)D_800CC2D0)[D_800CC26C])->id == 0x61) {
                    if (arg0->unk84 == 0x7A) {
                        t7A = (&((Actor *)D_800CC2D0)[D_800CC26C])->unk7A;
                        vb34 = arg0->unk76 - t7A; padx3 = (vb34 + 0x2000) & 0xC000;
                        if (padx3 & 0x4000) {
                            (&((Actor *)D_800CC2D0)[D_800CC26C])->unk218 = NULL;
                            (&((Actor *)D_800CC2D0)[D_800CC26C])->unk232 = 2;
                            (&((Actor *)D_800CC2D0)[D_800CC26C])->unk76 = t7A + padx3;
                            arg0->unk76 = (&((Actor *)D_800CC2D0)[D_800CC26C])->unk76;
                            arg0->unk31C->unk3C = 1;
                            { BLK; arg0->unk54 *= 0.4f; }
                        }
                    }
                } else {
                    spC4 = 0.3f;
                    spC0 = 0.5f;
                    tf10 = fabsf(arg0->y_position - (&((Actor *)D_800CC2D0)[D_800CC26C])->y_position);
                    spD8 = tf10;
                    if (tf10 > 30.0f) {
                        spD8 = 30.0f;
                    }
                    spD8 = (2.0f * (f32) (&((Actor *)D_800CC2D0)[D_800CC26C])->unkE4) - spD8;
                    arg0->unk54 *= 0.6f;
                    arg0->xz_velocity = ((Actor *)D_800CC2D0)[D_800CC26C].unk44;
                    if (((&((Actor *)D_800CC2D0)[D_800CC26C])->unk44) < ((&((Actor *)D_800CC2D0)[D_800CC26C])->xz_velocity)) {
                        spD4 = ((&((Actor *)D_800CC2D0)[D_800CC26C])->xz_velocity);
                    } else {
                        spD4 = ((&((Actor *)D_800CC2D0)[D_800CC26C])->unk44);
                    }
                    arg0->unk31C->unk3C = (u8) (u32) ((32.0f - spD4) * 0.125f);
                    if ((&((Actor *)D_800CC2D0)[D_800CC26C])->id == 0x34) {
                        if ((&((Actor *)D_800CC2D0)[D_800CC26C])->unk251 != 1) {
                            spCB = 0;
                        }
                        sp198 = 0x23;
                        arg0->unk31C->unk3C = 2;
                        spCC = 0.3f;
                        if (arg0->xz_velocity > 5.0f) {
                            spCC = 0.6f;
                        }
                    } else {
                        sp198 = 0x18;
                    }
                    if ((sp198 != arg0->unk84) && ((&((Actor *)D_800CC2D0)[D_800CC26C])->id != 0x17)) {
                        D_800D1580 = -0xF7FED5;
                        spC0 = 0.5f;
                        func_1506E8D8();
                    }
                    if ((&((Actor *)D_800CC2D0)[D_800CC26C])->unk224 != 0) {
                        spC4 = (f32) (&((Actor *)D_800CC2D0)[D_800CC26C])->unk224 * 0.01f;
                    }
                    if (spCB != 0) {
                        spC0 = 0.5f;
                        func_1505A184(arg0->unk76, spD8, 0.0f, &spD8, &spD4, &spD0);
                        arg0->x_position += (((((&((Actor *)D_800CC2D0)[D_800CC26C])->x_position - spD8) + (&((Actor *)D_800CC2D0)[D_800CC26C])->unkBC) - arg0->x_position) * spCC);
                        arg0->z_position += (((((&((Actor *)D_800CC2D0)[D_800CC26C])->z_position - spD4) + (&((Actor *)D_800CC2D0)[D_800CC26C])->unk148) - arg0->z_position) * spCC);
                        (&((Actor *)D_800CC2D0)[D_800CC26C])->unk76 = arg0->unk76;
                    }
                    (&((Actor *)D_800CC2D0)[D_800CC26C])->unk87 = 1;
                    spD8 = arg0->unk44 * spC4;
                    if ((&((Actor *)D_800CC2D0)[D_800CC26C])->id == 0x25) {
                        spD8 = arg0->unk44 * 0.6f;
                        arg0->xz_velocity += 2.0f;
                        spC0 = 1.0f;
                        arg0->unk31C->unk3C += 3;
                    }
                    if ((&((Actor *)D_800CC2D0)[D_800CC26C])->id != 0x34) {
                        if ((&((Actor *)D_800CC2D0)[D_800CC26C])->xz_velocity < spD8) {
                            (&((Actor *)D_800CC2D0)[D_800CC26C])->xz_velocity = (&((Actor *)D_800CC2D0)[D_800CC26C])->xz_velocity + (spC0 * D_800BE9A4);
                        }
                    } else {
                        arg0->xz_velocity = (&((Actor *)D_800CC2D0)[D_800CC26C])->xz_velocity;
                    }
                }
            }
        }
    }
    if ((D_800BE616 != 0) && (D_800CC268 != 0) && (arg0->unk31C->unk1AC != 0)) {
        func_15063168(arg0);
    }
    arg0->unk40 = (f32) (arg0->unk7A + 0x4000) * 0.005493164f;
    func_1502178C((struct127 *) arg0, 0, -1);
    sp190 = arg0->x_position;
    sp18C = arg0->z_position;
    if (!(arg0->unkF8 & 0x8000)) {
        func_15055E50(arg0, arg0->unk1E4);
        if (arg0->unkF4 & 0x100000) {
            sp178 = 30000.0f;
        } else {
            sp178 = arg0->old_y_position;
        }
        if ((-10000.0f != arg0->unk118) || ((arg0->unkF8 & 2) == 0) || ((func_150A3FC4(arg0->x_position, arg0->z_position, arg0->unk188, &arg0->unk18C, &spBC) == 0))) {
            func_1510E8BC(&arg0->unk188, (&arg0->unk18C), &spBC, &arg0->unk118, &arg0->unk184, arg0->unk1A2, arg0->x_position, arg0->y_position, arg0->z_position, sp178, 0, arg0, -10000.0f, arg0->y_position, arg0->unkF8 & 0x04000000);
            if (arg0->unk118 < ((f32) arg0->unk120)) {
                arg0->unk118 = ((f32) arg0->unk120);
            }
        }
        if (arg0->y_position < spBC) {
            spB8 = 30.0f;
        } else {
            spB8 = 15.0f;
        }
        if ((arg0->unkF8 & 4) && (arg0->unk31C->unk54 <= 0) && (arg0->unk31C->unk6B == 0) && (arg0->unk137 == 0) && (arg0->unk65 == 0) && ((arg0->unk31C->unk84 == 0) || (arg0->x_position != arg0->old_x_position) || (arg0->y_position != arg0->old_y_position) || (arg0->z_position != arg0->old_z_position))) {
            if (arg0->unkF8 & 0x08000000) {
                D_800CBDD3 = 1;
            }
            if (arg0->unk31C->unk4E == 2) {
                D_800CBDD4 = 1;
            }
            func_15044380(arg0->old_x_position, arg0->old_y_position, arg0->old_z_position, arg0, 1, 1);
            D_800CBDD4 = 0;
            D_800CBDD3 = 0;
        } else {
            D_800CBDF4 = D_800CBDF8 = -32768.0f;
        }
        if ((sp190 != arg0->x_position) || (sp18C != arg0->z_position)) {
            tf1 = -10000.0f;
            arg0->unkF4 |= 0x10000;
            if ((tf1 != arg0->unk118) || (spBC != tf1) || !(arg0->unkF8 & 2) || (func_150A3FC4(arg0->x_position, arg0->z_position, arg0->unk188, (&arg0->unk18C), &spBC) == 0)) {
                func_1510E8BC(&arg0->unk188, (&arg0->unk18C), &spBC, &arg0->unk118, &arg0->unk184, arg0->unk1A2, arg0->x_position, arg0->y_position, arg0->z_position, sp178, 0, arg0, tf1, arg0->y_position, arg0->unkF8 & 0x04000000);
                if (arg0->unk118 < ((f32) arg0->unk120)) {
                    arg0->unk118 = ((f32) arg0->unk120);
                }
            }
            temp_a0_3 = func_1505A630(arg0->x_position - sp190, sp18C - arg0->z_position, 0);
            angx = arg0->unk76 - temp_a0_3;
            if (arg0->unk31C->unk4E == 2) {
                if ((angx >= 0x1E01) && (angx < 0xE200)) {
                    tlk = arg0->unk76 - temp_a0_3 - 0x8000;
                    if (tlk & 0x8000) {
                        tmpi = (tlk >> 2) + 0x9800;
                    } else {
                        tmpi = (tlk >> 2) + 0x2800;
                    }
                    arg0->unk76 = temp_a0_3 - tmpi;
                    if (arg0->unk31C->unk44 <= 0) {
                        if ((arg0->health == 1) || ((angx >= 0x5E01) && (angx < 0xA200))) {
                            arg0->health = 0;
                            func_150D5A6C(arg0, 0xFF, 1);
                            func_1507CD64(arg0, 8);
                        } else if (arg0->health != 0) {
                            if (D_800BE616 == 0) {
                                D_800D1580 = -0xFDFE2A;
                                { BLK; func_1506E9D8(); }
                            }
                            arg0->health -= 1;
                        }
                        arg0->unk31C->unk44 = 0x14; if (angx >= 0x5E01) {}
                    }
                }
                if (arg0->xz_velocity > 30.0f) {
                    arg0->xz_velocity = arg0->xz_velocity * 0.7f;
                    arg0->disable_run = 0xC;
                }
                if ((arg0->health != 0) && (func_100107F8((struct127 *) arg0) == 0)) {
                    D_800D1580 = -0xF7FCF6;
                    func_1506E5FC();
                }
            } else {
                var_a0 = arg0->in_water; if (var_a0 == 0) {
                    if (arg0->unk28 > 60.0f) {
                        if ((angx >= 0x5E01) && (angx < 0xA200) && (var_a0 != 0x64) && (arg0->xz_velocity > 14.0f)) {
                            arg0->xz_velocity = 14.0f;
                        }
                    } else if ((arg0->unk31C->unk17 == 0) && (arg0->unk31C->unk1B3 == 0) && (arg0->unk13C == 0) && (angx >= 0x5E01) && (angx < 0xA200) && ((tf1 = arg0->xz_velocity, (tf1 > 10.0f)) || ((arg0->unk31C->unk4 == 0) && (tf1 > 2.0f)))) {
                        if (tf1 > 14.0f) {
                            arg0->xz_velocity = 14.0f;
                        }
                        if ((arg0->unk31C->unk4E == 0) && (arg0->unk31C->unk11A == 0)) {
                            sp198 = 0x7A;
                            if ((arg0->unk81 == 0) && (arg0->unk83 == 0)) {
                                arg0->unk40 = (f32) (temp_a0_3 - 0x4000) * 0.005493164f;
                            }
                        }
                    }
                }
            }
        }
        D_800CC27C = ((u32) arg0->unk184 >> 0x1C) & 7;
        if (D_800CC27C >= 7) {
            D_800CC27C = 0;
        }
        arg0->unk180 = spBC;
        arg0->unk31C->unk4F = 0;
        t4E = arg0->unk31C->unk4E; if ((t4E & 0xF) == 1) {
            if (arg0->gravity < 1.0f) {
                arg0->gravity = 5.0f;
            }
            if ((spBC + 45.0f) < arg0->unk118) {
                arg0->unk31C->unk4F = 1;
                spB8 = 0.0f;
            }
        } else if (t4E == 2) {
            if (spBC == -10000.0f) {
                arg0->health = 0;
                func_150D5A6C(arg0, 0xFF, 1);
                func_1507CD64(arg0, 8);
            }
            if (arg0->unk31C->unk50 <= 0) {
                func_1504CA60((struct127 *) arg0);
            }
            arg0->unkCC = 0x1E;
            arg0->unkCE = arg0->unkCC;
            if ((D_800BE9E4 < (s32) arg0->unk31C->unk12C) && (arg0->health != 0)) {
                arg0->unk31C->unk12C = arg0->unk31C->unk12C - D_800BE9E4;
                arg0->xz_velocity = 254.0f;
            } else {
                arg0->unk31C->unk12C = 0;
            }
            if ((arg0->unk31C->unk8C & 0x2000) && (arg0->health != 0)) {
                func_150D7790(arg0, arg0->unk31C->unk75 & 0x7F);
            }
        }
        if ((arg0->y_position < (spBC + spB8)) && (arg0->y_velocity < 0.0f) && ((spB8 > 20.0f) || (arg0->unk28 == 0.0f))) {
            arg0->y_position = spBC - 1.0f;
        }
        if ((arg0->unk31C->unk4E == 2) || (arg0->unk31C->matrix_physics != 0)) {
            func_1505B5F8((struct127 *) arg0, arg0->unk180);
        } else {
            func_1505A9AC(arg0, arg0->unk180);
        }
        if (arg0->unk31C->unk95 != 0) {
            arg0->unkB8 = 0.0f;
            if (arg0->stunned == 0) {
                if ((arg0->camera != NULL) && (arg0->camera->unk2C != 0x100)) {
                    vf14b = (f32) D_800CC284->stick_y;
                    idx = (arg0->unk31C->unk95 & ~0x80) - 1;
                    pE2 = &D_800D3098[idx];
                    if (arg0->unk31C->unk96 != 0) {
                        vf14b = 60.0f;
                    }
                    tf13 = (f32) (pE2->unk2 + pE2->unk8);
                    vf12c = (vf14b * 0.025f);
                    arg0->unk1CC = arg0->y_position;
                    if (arg0->unk31C->unk97 != 0) {
                        arg0->unk83 = 0xFF;
                        arg0->disable_run = 0xC;
                        vf12c = 1.0f;
                        arg0->unk100 |= 0x20;
                        anim = 0x31A;
                    } else {
                        if ((tf13 <= arg0->y_position) && (vf14b >= 0.0f)) {
                            vf12c = 1.0f;
                            anim = 0x206;
                        } else {
                            anim = 0x297;
                            if (vf14b >= 0.0f) {
                                anim = 0x193;
                            } else {
                                vf12c = -(vf14b * 0.025f);
                            }
                        }
                        arg0->unk184 &= ~0x1F;
                        if (D_800BE9F0 == 0x23) {
                            arg0->unk184 |= 9;
                        } else {
                            arg0->unk184 |= 3;
                        }
                        if ((tf13 - 80.0f) <= arg0->y_position) {
                            if (!(arg0->unk31C->unk95 & 0x80)) {
                                arg0->unk137 = 2;
                                vf12c = 3.0f;
                                anim = 0x262;
                                arg0->unk31C->unk95 = 0;
                            } else if (tf13 <= arg0->y_position) {
                                vf12c = 1.0f;
                                anim = 0x206;
                                arg0->y_velocity = 0.0f;
                                arg0->y_position = tf13;
                            }
                        } else if (arg0->y_position < (f32) pE2->unk2) {
                            arg0->disable_run = 0;
                            arg0->unk31C->unk95 = 0;
                            arg0->gravity = 4.0f;
                        }
                        if ((arg0->unk31C->unk95 == 0) && (arg0->in_water != 0)) {
                            arg0->gravity = 0.0f;
                        }
                    }
                    if (anim != -1) {
                        func_1505E650((struct127 *) arg0, anim, vf12c, 6.0f, 0.0f, 0.0f, 0);
                    }
                }
            }
            if (arg0->unk28 == 0.0f) {
                if (arg0->unk31C->unk97 == 0) {
                    arg0->unk31C->unk95 = 0;
                    arg0->disable_run = 6;
                    arg0->gravity = 4.0f;
                }
            }
        }
    }
    if ((arg0->unk28 == 0.0f) && (arg0->unk31C->unk4E != 2) && (arg0->health != 0) && ((arg0->unk184 & 0x1F) == 0xE)) {
        func_1505D024(arg0, 1, arg0->unk7A, -1);
        arg0->unk10B |= 0x80;
    }
    if ((arg0->unk180 < -9000.0f) && (arg0->y_velocity < -120.0f)) {
        arg0->unk31C->unk36 += D_800BE9A0;
        if ((s32) arg0->unk31C->unk36 >= 0x24) {
            func_1506D538();
        }
    } else {
        arg0->unk31C->unk36 = 0;
    }
    func_15051558((Obj15051558 *) arg0, sp190, sp18C, sp1BC);
    if (arg0->unk31C->unk4E == 0) {
        if ((arg0->unk28 > 5.0f) && (arg0->unk137 == 0) && (arg0->disable_run == 0) && (arg0->unk31C->unk78 == 0) && (arg0->unk31C->unk17 == 0) && (arg0->stunned == 0) && (arg0->unk31C->unk46 == 0) && (arg0->in_water == 0) && (arg0->unk13C == 0) && (-10000.0f != arg0->unk180) && (arg0->health != 0) && (arg0->unk31C->unk8 == 0) && (arg0->unk31C->unk4E == 0) && (arg0->unk110 == 0) && ((arg0->unk1CC - arg0->y_position) < 450.0f)) {
            anim = 0;
            if (D_800CBDF4 != -32768.0f) {
                sp92 = (s16) (s32) (((arg0->unk40 + D_800CBD94) - 90.0f) / 1.40625f);
                sp88 = D_800CBDF4;
                anim = 1;
            } else if ((D_800CBDF8 != -32768.0f) && (arg0->xz_velocity < 10.0f)) {
                sp92 = (s16) (s32) ((arg0->unk40 + D_800CBD98 + 90.0f) / 1.40625f);
                sp88 = D_800CBDF8;
                anim = 2;
            }
            if (anim != 0) {
                arg0->unk76 = (sp92 << 8);
                arg0->unk7A = arg0->unk76;
                arg0->y_position = sp88 - 25.0f;
                arg0->x_position = D_800CBDFC;
                arg0->z_position = D_800CBD90;
                arg0->unk31C->unk53 = 1;
                arg0->unk31C->unk55 = 0;
                arg0->disable_run = 0xFF;
                arg0->unk137 = 1;
                arg0->unk81 = 1;
                arg0->xz_velocity = 0.0f;
                arg0->y_velocity = 0.0f;
                arg0->gravity = 0.0f;
                arg0->unk1CC = arg0->y_position;
                func_1505E650((struct127 *) arg0, 0x42U, 1.0f, 1.0f, 0.0f, 0.0f, 0);
                arg0->unk31C->unk44 = 0xC;
            }
        }
        if (arg0->y_velocity < 5.0f) {
            if ((-10000.0f != arg0->unk118) && (arg0->y_position < (arg0->unk118 - 60.0f)) && (arg0->in_water != 1)) {
                sp84 = 4.0f;
                arg0->unk31C->unk55 = 0;
                arg0->unk31C->unk31 = 0;
                arg0->in_water = 1;
                arg0->unk164 = 0.0f;
                arg0->unk168 = 0.0f;
                arg0->unk162 = 0;
                arg0->unk1CC = arg0->y_position;
                arg0->xz_velocity *= 0.5f;
                arg0->unk76 = arg0->unk7A;
                arg0->unk31C->unk22 = 0;
                if (D_800BE9B4 == 0) {
                    arg0->unk31C->unk21 = 1;
                    arg0->unkB2 = 0;
                }
                if ((arg0->unk31C->unk20 & 1) && (D_800BE616 == 0)) {
                    func_15083568((struct127 *) arg0, 0x38, 0.0f, 0);
                    sp84 = 4.0f;
                }
                if (arg0->unk110 == 2) {
                    arg0->unk76 ^= 0x8000;
                    arg0->unk7A = arg0->unk76;
                    arg0->xz_velocity = 20.0f;
                    arg0->unkB8 = -30.0f;
                }
                arg0->unk110 = 0;
                if ((arg0->unk84 == 0x3C) || (arg0->unk84 == 0x3B)) {
                    sp84 = 0.0f;
                    arg0->y_velocity *= 0.5f;
                    arg0->gravity = -2.4f;
                    arg0->unkB8 = 89.0f;
                } else {
                    arg0->y_velocity *= 0.4f;
                    arg0->gravity = -3.0f;
                }
                arg0->stunned = 0;
                arg0->target_speed = 0.0f;
                arg0->unk81 = 0;
                arg0->unkA9 = 0;
                if ((arg0->unk31C->unk95 == 0) && (arg0->health != 0)) {
                    func_1506B100(0xD1, 2.0f, sp84);
                    arg0->disable_run = 0;
                }
                if (arg0->health == 0) {
                    arg0->disable_run = 0xFF;
                    func_1507CD64(arg0, 4);
                    arg0->stunned = 0xFF;
                }
            }
        }
    }
    func_1505250C((struct127 *) arg0, 1);
    if (arg0->unkCC < -0x5D) {
        arg0->health = 0;
        func_1507CD64(arg0, 3);
    }
    if (arg0->unkD0 != 0) {
        func_1505C7D8((struct127 *) arg0, D_800C3E78);
    }
    if ((s32) arg0->unk7F >= 9) {
        arg0->unk80 = 1;
    }
    if (arg0->unk83 != 0) {
        if (arg0->unk83 != 0xFF) {
            if ((s32) D_800BE9A0 >= (s32) arg0->unk83) {
                arg0->unk83 = 0;
            } else {
                arg0->unk83 = arg0->unk83 - D_800BE9A0;
            }
        }
        if ((arg0->unk84 == 0x36) || (arg0->unk84 == 0x19D)) {
            if ((arg0->xz_velocity > 8.0f) && (arg0->disable_run == 0)) {
                arg0->unk83 = 0;
                sp1B4 = arg0->xz_velocity;
            }
        }
    } else {
        arg0->unkAA = 0;
    }
    if (arg0->unkA8 != 0) {
        arg0->unkA8 = arg0->unkA8 - 1;
    }
    if (arg0->unk102 != 0) {
        arg0->unk102--;
        arg0->gravity = 5.0f;
        if (arg0->unk102 == 0) {
            func_1506AF74();
        }
    }
    if (arg0->unk100 & 1) {
        if (arg0->unk31C->unk78 == 0) {
            if ((arg0->unk31C->unk11A != 1) && (arg0->unk31C->unk11A != 3) && (arg0->in_water == 0) && (arg0->stunned == 0) && (D_800CC2B1 == 0) && ((arg0->disable_run == 0) || (arg0->unk84 == 0x279)) && (arg0->unk31C->unk8 <= 0) && (arg0->unk28 == 0.0f) && (D_800CC284->buttons & 0x2000)) {
                arg0->unk81 = 0;
                arg0->unk83 = 0;
                arg0->unk100 = arg0->unk100 & 0xFE;
                arg0->unk76 = arg0->unk7A;
            }
        }
    }
    if (((arg0->unk1CC - arg0->y_position) > 600.0f) && (arg0->unk31C->unk120 == 0)) {
        if ((arg0->unk31C->unk11A != 1) && (arg0->unk31C->unk11A != 3) && (-10000.0f == arg0->unk118)) {
            if ((arg0->unk84 != 0x101) && (arg0->in_water == 0)) {
                if (arg0->unk84 != 0xCF) {
                    sp80 = 0x2007E5;
                    if (func_1506E46C(arg0, &sp80, 1) != 0) {
                        func_15060A30(sp80, arg0);
                    }
                    { BLK; }
                }
                arg0->unk31C->unk31 = 0;
                func_1505E650((struct127 *) arg0, 0xCFU, 1.0f, 2.0f, 0.0f, 0.0f, 0);
                arg0->unk83 = 0xC8;
            }
        }
    }
    arg0->unk31C->unk4 = 0;
    func_1504C0E8((u8 *) arg0, sp198, sp1B4);
    arg0->unk31C->unk0 = 2.0f * sqrtf(((arg0->x_position - arg0->old_x_position) * (arg0->x_position - arg0->old_x_position)) + ((arg0->z_position - arg0->old_z_position) * (arg0->z_position - arg0->old_z_position)));
    arg0->unk31C->unk32 = 0;
    if ((arg0->unk31C->unk84 != 0) || (sp17C != 0)) {
        *D_800CC284 = sp180;
    }
    arg0->unk31C->unk88 = (s16) sp188;
}

void func_150511E8(struct127 *arg0) {
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    f32 sp60;
    f32 sp5C;

    if (arg0->unk28 <= 0.0f) {
        s32 flag;
        f32 a;
        f32 b;
        f32 c;
        f32 d;
        f32 ret;

        a = 0.6999999881f;
        b = 0.3000000119f;
        c = 35.0f;
        d = 0.69999998807907104f;
        if ((arg0->id == 0x28) || (arg0->id == 0x77)) {
            a = 0.8999999762f;
            b = 0.200000003f;
            c = 7.0f;
            d = 0.6000000238f;
        }
        func_1510F820(arg0->pad188, &arg0->unk18C, &sp64, &sp60, &sp5C, &sp6C, &sp68);
        if (arg0->unkF8 & 0x100000) {
            f32 sp40;

            func_1510F820(arg0->pad188, &arg0->unk18C, &sp64, &sp60, &sp5C, &sp6C, &sp68);
            ret = func_1505210C(arg0, sp6C, sp68, &sp40, 50.0f);
            sp40 *= a;
            arg0->unkB8 += (sp40 - arg0->unkB8) * b;
            arg0->unkC4 += (ret * a - arg0->unkC4) * b;
        }
        if (!(arg0->unkF8 & 0x80000)) {
            return;
        }
        if (arg0->unk226 != 0) {
            c = (u32)arg0->unk226;
        }
        if ((fabsf(sp6C) < 0.09000000358f) && (fabsf(sp68) < 0.09000000358f)) {
            sp6C = 0.0f;
            sp5C = 0.20000000298f;
            sp68 = 0.0f;
        } else {
            f32 len;

            flag = 0;
            if (fabsf(sp6C) > 1.0f) {
                if (sp6C < 0.0f) {
                    sp6C = -1.00000000f;
                } else {
                    sp6C = 1.00000000f;
                }
                flag = 1;
            }
            if (fabsf(sp68) > 1.0f) {
                if (sp68 < 0.0f) {
                    sp68 = -1.00000000f;
                } else {
                    sp68 = 1.00000000f;
                }
                flag = 1;
            }
            if (flag && (arg0->id == 0x77)) {
                if (arg0->xz_velocity > 15.0f) {
                    arg0->xz_velocity *= 0.8000000119f;
                }
                if (arg0->disable_run < 8) {
                    arg0->disable_run = 6;
                }
            }
            len = sqrtf(sp6C * sp6C + sp68 * sp68);
            sp6C *= c;
            sp68 *= c;
            sp5C = len * d + 0.05000000075f;
        }
    } else {
        sp68 = 0.0f;
        sp6C = 0.0f;
        sp5C = 0.1000000015f;
    }
    func_1505A250(-sp6C, -sp68, sp5C, &arg0->unk164, &arg0->unk168);
}

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
