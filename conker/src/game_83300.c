#include <ultra64.h>

#define func_1506160C func_1506160C_hdr
#define func_1505841C func_1505841C_hdr
#define func_1000F85C func_1000F85C_hdr
#define func_1000F91C func_1000F91C_hdr
#define func_10010BE8 func_10010BE8_hdr
#include "functions.h"
#undef func_1506160C
#undef func_1505841C
#undef func_1000F85C
#undef func_1000F91C
#undef func_10010BE8
void func_1000F85C(u16, s32, s32);
void func_1000F91C(u16, s32, s16, s32, s32, s32, s32, s32, s32, s32);
u16 func_10010E78(u16, s32, u16, s32, s32, s32, s32, s32, s32, s32, s32);
u16 func_10010BE8(u16, s32, s32, s32, s32, s32, s32);
#define D_800D1550 D_800D1550_hdr
#define D_800CC298 D_800CC298_hdr
#include "variables.h"
#undef D_800D1550
#undef D_800CC298
extern f32 D_800D1550;
typedef struct Game83300Anim298 {
    f32 unk0;
    f32 unk4;
    s8 unk8;
    s8 unk9;
    s8 unkA;
    u8 unkB[4];
    u8 unkF;
    u8 unk10;
} Game83300Anim298;
extern Game83300Anim298 D_800CC298;


void func_15058F24(struct127 *arg0, f32, f32);
void func_1505D6F0(struct127 *arg0, s32 arg1);
u8   func_150599C8(struct127 *arg0, u8 arg1, u16 arg2);
void func_1505A250(f32 arg0, f32 arg1, f32 arg2, f32 *arg3, f32 *arg4);
void func_1505A770(void *arg0);
void func_15059C84(void *arg0);
f32  func_1505A3A8(f32 arg0, void *arg1, f32 arg2, f32 arg3, u8 arg4);

s32 func_15080738(s32 arg0);
void func_150642AC(struct127 *arg0, s32 arg1, f32 arg2, s32 arg3, s32 arg4);
s32 func_15065A5C(struct127 *arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

u8  func_1505B9C4(void *arg0, struct127 *arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5, u8 arg6);
s32 func_1505C1E4(void *arg0, struct127 *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern struct255 *D_800CC4A4;
void func_1505F188(struct127 *arg0);
extern f32 D_8009962C;
extern f32 D_80099630;
extern f32 D_800994C0;
extern f32 D_800994C4;
extern f32 D_800994C8;
extern f32 D_800994CC;
extern f32 D_800994D0;

typedef struct {
    char pad_0[0x4];
    u16 field_0x4;
    s16 field_0x6;
    f32 field_0x8;
    f32 field_0xC;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    s32 field_0x28;
    s32 field_0x2C;
    s32 field_0x30;
    s32 field_0x34;
    s8 field_0x38;
    s8 field_0x39;
    char pad_0x3A[0x6];
    u8 field_0x40[1];
    u8 field_0x41;
    char pad_0x42[0x1CE];
    u8 field_0x210[1];
    u8 field_0x211;
    char pad_0x212[0x1CE];
} AnimPlaybackState;

typedef struct {
    char pad_0[0x4];
    u8 field_0x4;
    char pad_0x5[0x7F];
    u16 field_0x84;
    char pad_0x86[0x24A];
    AnimPlaybackState *field_0x2D0;
} AnimPlaybackOwner;

#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_15055E50.s")

void func_15056150(struct127 *arg0) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f2;
    s16 *temp_v1;

    if (arg0->unk2E8 != 0) {
        temp_v1 = D_800D2104[arg0->unk13F];
        temp_f2 = temp_v1[4];
        temp_f12 = temp_v1[6];
        temp_f14 = temp_v1[5];
        temp_f16 = arg0->unk2E8;
        temp_f16 = temp_f16 * 8.0f;
        temp_f18 = arg0->x_position - temp_f2;
        temp_f20 = arg0->z_position - temp_f12;
        temp_f22 = arg0->y_position - temp_f14;
        temp_f0 = sqrtf((temp_f18 * temp_f18) + (temp_f20 * temp_f20) + (temp_f22 * temp_f22) + D_80099440);
        temp_f18 *= 1.0f / temp_f0;
        temp_f20 *= 1.0f / temp_f0;
        temp_f22 *= 1.0f / temp_f0;
        if (temp_f16 < temp_f0) {
            arg0->x_position = temp_f18 * temp_f16 + temp_f2;
            arg0->z_position = temp_f20 * temp_f16 + temp_f12;
            arg0->y_position = temp_f22 * temp_f16 + temp_f14;
        }
    }
}

void func_15056258(struct127 *arg0) {
    f32 temp_f2;
    f32 temp_f4;
    f32 heightFactor;

    temp_f4 = arg0->unk11C;
    temp_f2 = arg0->unk118 - arg0->unk11C;
    arg0->unk11C = arg0->unk118;
    heightFactor = 2.0f * (arg0->y_position - ((arg0->unk118 - 60.0f) - 170.0f));
    if (!(heightFactor < 0.0f)) {
        if (!(fabsf(temp_f2) > 30.0f)) {
            if (heightFactor > 300.0f) {
                heightFactor = 300.0f;
            }
            arg0->y_position += (heightFactor * temp_f2 * D_80099444);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_150562FC.s")
// NON-MATCHING: 10% of the way there...
// void func_150562FC(struct127 *arg0) {
//     f32 sp70;
//     f32 sp6C;
//     f32 sp68;
//     u8 sp63;
//     s32 sp54;
//     f32 sp4C;
//     f32 temp_f0;
//     f32 temp_f12;
//     f32 temp_f14;
//
//     s32 *temp_a3;
//     // s32 temp_t1;
//     s32 temp_t1_2;
//     s32 temp_t2;
//     s32 temp_t9;
//     s32 temp_v0_6;
//     struct255 *temp_v1_2;
//     u16 temp_v0_2;
//     u16 temp_v0_3;
//     u16 temp_v0_4;
//     u16 temp_v0_5;
//     u16 temp_v0_7;
//     u16 temp_v0_8;
//     u16 temp_v1;
//     u8 temp_v0;
//     struct127 *temp_t0;
//     struct127 *temp_t0_2;
//     u8 *temp_v0_11;
//     s32 phi_a2;
//     s32 phi_t1;
//     s32 phi_t1_2;
//     s32 phi_t1_3;
//     s32 phi_a2_2;
//     f32 phi_f18;
//     s32 phi_t1_4;
//     f32 phi_f0;
//     s32 phi_t1_5;
//     s32 phi_t2;
//
//     sp63 = arg0->unk13D - 0x64;
//     sp54 = 0;
//     sp4C = 5.0f;
//     arg0->immune = (u8)0x1E;
//     temp_t0 = &D_800CC2D0[sp63];
//     arg0->unk180 = (f32) temp_t0->unk180;
//     arg0->unk1CC = (f32) arg0->y_position;
//     arg0->gravity = 4.5f;
//     arg0->y_velocity = 0.0f;
//     if ((s32) arg0->unk21C < 0x1E) {
//         arg0->unk21C += D_800BE9A0;
//     }
//     temp_v0 = arg0->id;
//     if (temp_v0 == 0x57) {
//         arg0->unk5C = 9;
//         if ((s32) arg0->unk21C >= 0xA) {
//             temp_v0_2 = temp_t0->unk84.uh;
//             sp54 = 1;
//             phi_a2 = 0xD;
//             if (temp_v0_2 == 0x114) {
//                 phi_a2 = 0x24;
//             }
//             phi_t1 = 0;
//             if (temp_v0_2 == 0x116) {
//                 phi_t1 = 1;
//             }
//             if (temp_v0_2 == 0x117) {
//                 phi_t1 = 2;
//             }
//             func_150649A0(sp63, D_800C3E78, phi_a2);
//             phi_t1_3 = phi_t1;
//             phi_a2_2 = phi_a2;
//         } else if (temp_v0 == 0x8C) {
//             arg0->unk5C = 4;
//             if ((s32) arg0->unk21C >= 0xA) {
//                 temp_v0_2 = temp_t0->unk84.uh;
//                 sp54 = 1;
//                 phi_t1_2 = 0;
//                 if (temp_v0_2 == 0x1A6) {
//                     phi_t1_2 = 1;
//                 }
//                 if (temp_v0_2 == 0x1A7) {
//                     phi_t1_2 = 2;
//                 }
//                 func_150649A0(sp63, D_800C3E78, 0xD);
//                 phi_t1_3 = phi_t1_2;
//                 phi_a2_2 = 0xD;
//             }
//         } else {
//             if ((temp_v0 == 0xA8) || (temp_v0 == 0xA9)) {
//                 arg0->unk5C = 9;
//                 temp_v0_2 = temp_t0->unk84.uh;
//                 sp54 = 1;
//                 phi_t1_3 = 0;
//                 if (temp_v0_2 == 0x1B0) {
//                     phi_t1_3 = 1;
//                 }
//                 phi_a2_2 = 0xD;
//                 if (temp_v0_2 == 0x1B1) {
//                     phi_t1_3 = 2;
//                     phi_a2_2 = 0xD;
//                 }
//             } else {
//                 phi_t1_3 = 0;
//                 phi_a2_2 = 0xD;
//                 if (temp_v0 == 0x5E) {
//                     arg0->unk5C = 9;
//                     temp_v0_2 = temp_t0->unk84.uh;
//                     sp54 = 1;
//                     sp4C = 16.0f;
//                     phi_t1_3 = 0;
//                     if (temp_v0_2 == 0x286) {
//                         phi_t1_3 = 2;
//                     }
//                     phi_a2_2 = 0xD;
//                     if (temp_v0_2 == 0x27E) {
//                         phi_t1_3 = 1;
//                         phi_a2_2 = 0xD;
//                     }
//                 }
//             }
//         }
//         temp_v0_6 = func_1505E7CC(phi_a2_2, arg0); //, phi_a2_2);
//         temp_t0_2 = &D_800CC2D0[sp63];
//         phi_t1_4 = phi_t1_3;
//         temp_t2 = temp_v0_6;
//         temp_a3 = D_800D1588[arg0->id] - 8;
//         if (temp_t0_2->interaction_state == 1) {
//             if (temp_t0_2->in_water != 0) {
//                 temp_t9 = temp_v0_6 * 3;
//                 arg0->unk5C = 4;
//                 sp54 = 1;
//                 if (((temp_a3 + (temp_t9 * 8)) + 14) != 0) { // unkE
//                     phi_t1_4 = 1;
//                 }
//             }
//         }
//         if ((s32) arg0->unk21C < 0x14) {
//             phi_f18 = (f32) (u32) arg0->unk21C / 50.0f;
//             if (D_800BE9B4 != 0) {
//                 phi_f18 = 1.0f;
//             }
//         } else {
//             phi_f18 = D_80099448;
//         }
//
//         sp70 = (f32) arg0->unk10F;
//
//         func_150599C8(arg0, 0x10, (temp_t0_2->unk7A + (arg0->unk13E << 8)) & 0xFFFF); //, temp_a3);
//         temp_t1_2 = phi_t1_4;
//         temp_v1_2 = temp_t0_2->unk1D4;
//         if (temp_v1_2 != 0) {
//             temp_f0 = (f32) ((s32) arg0->unk10C / 0x64);
//             if (temp_t0_2->interaction_state != 0x15) {
//                 temp_v0_8 = arg0->unk21C;
//                 if ((s32) temp_v0_8 >= 7) {
//                     arg0->y_position = (f32) temp_v1_2->unk240->unk1A;
//                     if ((s32) temp_v0_8 < 0xE) {
//                         // temp_f6 = (f32) temp_v0_8;
//                         temp_f0 = (temp_v0_8 / 14.0f) * temp_f0;
//                     }
//                     arg0->y_position = (f32) (arg0->y_position + temp_f0);
//                 }
//                 if (arg0->id == 0xE) {
//                     arg0->unkC4 = 0.0f;
//                 } else if (arg0->id == 0x13) {
//                     sp70 = 204.0f;
//                     arg0->unkC4 = 0.0f;
//                 }
//             }
//         }
//         phi_t1_5 = phi_t1_4;
//         phi_t2 = temp_t2;
//         if (temp_t0_2->interaction_state == 0x15) {
//             phi_t2 = func_1505E7CC(0x39, arg0);
//             arg0->unk7A = (u16) temp_t0_2->unk7A;
//             arg0->y_position = (f32) (temp_t0_2->y_position - 10.0f);
//             if ((arg0->unk28 > 100.0f) && ((s32) arg0->unk21C >= 0x15)) {
//                 phi_t1_5 = 1;
//             } else {
//                 sp4C = 0.0f;
//                 phi_t1_5 = phi_t1_4;
//             }
//             sp70 = 0.0f;
//             temp_f0 = arg0->unkC4;
//             arg0->xz_velocity = 0.0f;
//             arg0->unkC4 = (f32) (temp_f0 + ((temp_t0_2->unkC4 - temp_f0) * D_8009944C));
//         }
//         func_1505A184(D_800CC2D0[sp63].unk7A,  sp70, 0, &sp70, &sp6C, &sp68);
//         temp_t0_2 = &D_800CC2D0[sp63];
//         temp_f12 = temp_t0_2->x_position + sp70;
//
//         temp_f14 = temp_t0_2->z_position + sp6C;
//         arg0->x_position = (f32) (arg0->x_position + ((temp_f12 - arg0->x_position) * phi_f18));
//         arg0->z_position = (f32) (arg0->z_position + ((temp_f14 - arg0->z_position) * phi_f18));
//         temp_v0_11 = temp_a3 + (phi_t2 * 0x18) + (phi_t1_5 * 4);
//         func_1505E650(arg0, temp_v0_11[0xA],  temp_v0_11[0xC] / 100.0f, sp4C, 0.0f, 0.0f, 0);
//         if (sp54 != 0) {
//             arg0->unk65 = (s8) (sp63 + 1);
//             arg0->unk101 = (u8) (arg0->unk101 | 4);
//         } else {
//             arg0->unk65 = (u8)0;
//         }
//     }
// }

s32 func_1505693C(struct127 *arg0, s32 arg1) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f2;

    u16 *temp_v0 = (u16*)D_800D2104[(u16)arg0->unk13F];

    if (temp_v0[3] == 0) {
        return 1;
    }

    temp_f0 = temp_v0[3] * 8;
    temp_f0 = temp_f0 * temp_f0;
    temp_f2 = (s16)temp_v0[0] - D_800CC2D0[arg1].x_position;
    temp_f12 = (s16)temp_v0[2] - D_800CC2D0[arg1].z_position;

    if ((temp_f2 * temp_f2) + (temp_f12 * temp_f12) < temp_f0) {
        return 1;
    }
    return 0;
}

/* D_80099A3C is really u8[][10] (variables.h declares s16[]). Neither caller reads a return value. */
void func_15056A00(struct127 *arg0, u8 arg1, u8 arg2) {
    u8 sel;
    u16 w;
    u8 *p;

    sel = 0;
    if (((u8 (*)[10])D_80099A3C)[arg2][7] < arg1) {
        sel = 5;
    } else if (((u8 (*)[10])D_80099A3C)[arg2][2] < arg1) {
        sel = 2;
    }
    w = arg0->unk76;
    if ((arg0->unk78 - w) & 0x8000) {
        sel += 3;
    } else {
        sel += 4;
    }
    p = &((u8 (*)[10])D_80099A3C)[arg2][sel];
    if (*p != 0xFF) {
        arg0->unk218 = (struct216 *)((s32)arg0->unk218 - 5);
        arg0->unk21C = 0x4E20;
        arg0->unk223 = 0xD;
        arg0->unk44 = 0.0f;
        arg0->unk244 = *p;
        arg0->unkF4 &= ~0xE;
        arg0->unkF4 |= 4;
        arg0->unk78 = w;
        arg0->unk7A = w;
        arg0->unk138 = 0;
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_15056B08.s")
typedef struct Func1505841CActor {
    u8 pad0[0x28];
    f32 vertical;
    u8 pad2C[0x10];
    f32 speed;
    u8 pad40[0x10C];
    f32 scale;
    u8 pad150[0x80];
    s8 pitch;
    u8 pad1D1[0x52];
    u8 mode;
    u8 pad224[8];
    u16 motion_flags;
    u8 pad22E[0x16];
    u16 animation;
    u8 rate_flags;
    u8 pad247[2];
    u8 rate_low;
    u8 pad24A[6];
    u8 strength;
    u8 pad251[0x7F];
    void *field_2D0;
} Func1505841CActor;

void func_1505841C(Func1505841CActor *arg0, f32 arg1) {
    void *inner;
    f32 speed;
    f32 fade;
    f32 rate;
    f32 scaled_speed;
    s32 flags;

    flags = arg0->rate_flags;
    rate = (f32)(((flags & 0x1F) << 8) + arg0->rate_low) * D_80099468;
    if (!(flags & 0x80)) {
        speed = arg0->speed;
        if ((speed <= 1.0f) || ((arg0->motion_flags & 0x10) && (arg0->vertical == 0.0f))) {
            rate = arg1;
        } else {
            scaled_speed = speed;
            scaled_speed *= 0.5f / arg0->scale;
            rate *= 10.0f;
            rate = scaled_speed / rate + D_8009946C;
            if (!(flags & 0x20)) {
                rate += D_80099470;
            }
        }
    }
    if (arg0->mode == 0xD) {
        rate = (f32)(arg0->strength & 0x7F) * D_80099474;
    }
    if (flags == 0xFF) {
        rate = 0.0f;
    }
    func_1505E650((struct127 *)arg0, arg0->animation, rate,
                 (f32)arg0->pitch, 0.0f, 0.0f, 0);
    if (arg0->rate_flags == 0xFF) {
        fade = (f32)((s32 (*)[16])D_800418B0)[D_800419A0][0];
        if (fade >= 0.0f) {
            inner = arg0->field_2D0;
            *(f32 *)((u8 *)inner + 8) = (*(f32 *)((u8 *)inner + 0x18) * (32768.0f - fade)) / 32768.0f;
        }
    }
}

// NON-MATCHING: JUSTREG
// void func_1505841C(struct127 *arg0, f32 arg1) {
//     struct197 *temp_v0_2;
//     f32 temp_f0;
//     f32 temp_f1;
//     s32 idx;
//
//     temp_f0 = (((arg0->unk246 & 0x1F) << 8) + arg0->unk249) * D_80099468;
//     if ((arg0->unk246 & 0x80) == 0) {
//         if ((arg0->xz_velocity <= 1.0f) || (((arg0->unk22C & 0x10) != 0) && (arg0->unk28 == 0.0f))) {
//             temp_f0 = arg1;
//         } else {
//             // regalloc off here
//             temp_f1 = (arg0->xz_velocity * (0.5f / arg0->xz_scale));
//             temp_f0 = temp_f1 / (temp_f0 * 10.0f);
//             temp_f0 += D_8009946C;
//
//             if ((arg0->unk246 & 0x20) == 0) {
//                 temp_f0 += D_80099470;
//             }
//         }
//     }
//     if (arg0->unk223 == 0xD) {
//         temp_f0 = (arg0->unk250 & 0x7F) * D_80099474;
//     }
//     if (arg0->unk246 == 0xFF) {
//         temp_f0 = 0.0f;
//     }
//     func_1505E650(arg0, arg0->unk244, temp_f0, arg0->unk1D0, 0.0f, 0.0f, 0);
//
//     if (arg0->unk246 == 0xFF) {
//         // regalloc off here
//         idx = D_800419A0 << 4;
//         temp_f0 = D_800418B0[idx];
//         if (temp_f0 >= 0.0f) {
//             temp_v0_2 = arg0->unk2D0;
//             temp_v0_2->unk8 = (f32) ((temp_v0_2->unk18 * (32768.0f - temp_f0)) / 32768.0f);
//         }
//     }
// }

typedef struct Game83300Child585F0 {
    u8 pad0[0x7D];
    u8 field7D;
} Game83300Child585F0;

typedef struct Game83300State585F0 {
    s32 kind;
    u8 pad4[0x14];
    f32 height18;
    u8 pad1C[4];
    f32 field20, field24, field28;
    u8 pad2C[0x10];
    f32 field3C;
    u8 pad40[0x36];
    u16 angle76, angle78, angle7A;
    u8 pad7C[4];
    u8 field80, field81, pad82, field83;
    u8 pad84[0x29];
    u8 modeAD;
    u8 padAE[0xA];
    f32 fieldB8;
    u8 padBC[0x38];
    s32 flagsF4, flagsF8;
    u8 padFC[8];
    u8 count104;
    u8 pad105[4];
    u8 strength109, pad10A, flags10B;
    s16 timer10C;
    u8 pad10E[0xA];
    f32 ground118;
    u8 pad11C[0xAE];
    u8 field1CA, pad1CB;
    f32 height1CC;
    u8 pad1D0[0x18];
    u8 field1E8, pad1E9;
    u16 field1EA, field1EC;
    u8 pad1EE[0x2E];
    s16 cooldown21C;
    u8 pad21E[0x1A];
    u8 owner238, pad239, backup23A;
    u8 pad23B[0xE1];
    Game83300Child585F0 *child31C;
} Game83300State585F0;

void func_150585F0(u8 *arg0, f32 arg1) {
    Game83300State585F0 *actor;
    Game83300Child585F0 *child;
    f32 value;
    s32 mode;
    s32 count;
    s32 flags;
    s32 copiedAngle;
    u16 angle;
    s32 owner;

    actor = (void *)arg0;
    arg1 = (f32)(u32)actor->strength109 * D_80099478;
    func_1505A3A8(0.0f, actor, 1.0f, arg1, 0);
    if (actor->height1CC < D_8009947C) {
        actor->height1CC = actor->height18;
    }
    mode = actor->modeAD;
    if (mode != 0 && mode < 10) {
        value = actor->field20;
        actor->field24 = 0.0f;
        if (value < 60.0f) {
            actor->field20 = value * D_80099480;
        }
        if ((actor->ground118 - 60.0f) + 40.0f < actor->height18) {
            actor->field81 = 0;
            actor->field83 = 0;
            actor->modeAD = 0;
            actor->fieldB8 = 0.0f;
            actor->field24 = 4.0f;
        }
    }
    if (D_800BE616 == 0 || actor->kind != 1) {
        actor->cooldown21C = 0;
    }
    count = actor->count104;
    if (count != 255) {
        if (count != 254 && (actor->field1CA != 0 || actor->kind != 1)) {
            actor->count104 = count - 1;
        }
        actor->timer10C -= D_800CC264;
        if (actor->field28 < D_80099484 && (actor->flagsF4 & 0x100)) {
            actor->timer10C = 0;
        }
        if (actor->timer10C <= 0) {
            child = actor->child31C;
            if (child != 0) {
                child->field7D = 0;
            }
            actor->timer10C = 0;
            func_1505E874(D_800C3E78, (struct127 *)actor);
        }
        if (actor->count104 == 0) {
            copiedAngle = actor->angle7A;
            actor->field3C = 0.0f;
            actor->field81 = 0;
            actor->angle78 = copiedAngle;
            actor->angle76 = copiedAngle;
            if (actor->modeAD != 0) {
                actor->field20 = 0.0f;
                if (actor->kind == 1) {
                    func_1506B078();
                }
            } else if (actor->kind == 1) {
                actor->field24 = 4.0f;
                actor->flagsF8 &= 0xFFFF7FFF;
            }
            owner = actor->owner238;
            if (owner != 0) {
                actor->backup23A = owner;
            }
        } else {
            flags = actor->flags10B;
            if (!(flags & 2)) {
                angle = actor->angle76;
                if (flags & 4) {
                    angle ^= 0x8000;
                }
                actor->field80 = 10;
                func_150599C8((struct127 *)actor, 12, angle);
            }
        }
    }
}
// NON-MATCHING: 80% there
// void func_150585F0(struct127 *arg0) {
//     f32 temp_f0;
//     u16 phi_a2;
//
//     func_1505A3A8(arg0->unk109 * D_80099478, arg0, 1.0f, arg0->unk109 * D_80099478, 0);
//     if (arg0->unk1CC < D_8009947C) {
//         arg0->unk1CC = arg0->y_position;
//     }
//     if ((arg0->in_water != 0) && ((s32) arg0->in_water < 0xA)) {
//         arg0->gravity = 0.0f;
//         if (arg0->y_velocity < 60.0f) {
//             arg0->y_velocity *= D_80099480;
//         }
//         if (((arg0->unk118 - 60.0f) + 40.0f) < arg0->y_position) {
//             arg0->unk81 = (u8)0;
//             arg0->unk83 = (u8)0;
//             arg0->in_water = (u8)0U;
//             arg0->unkB8 = 0.0f;
//             arg0->gravity = 4.0f;
//         }
//     }
//     if ((D_800BE616 == 0) || (arg0->interaction_state != 1)) {
//         arg0->unk21C = (u16)0;
//     }
//     if (arg0->stunned != 0xFF) {
//         if (arg0->stunned != 0xFE) {
//             if ((arg0->health != 0) || (1 != arg0->interaction_state)) {
//                 arg0->stunned -= 1;
//             }
//         }
//         arg0->unk10C -= D_800CC264;
//         if ((arg0->unk28 < D_80099484) && ((arg0->unkF4 & 0x100) != 0)) {
//             arg0->unk10C = (u16)0;
//         }
//         if ((s32) arg0->unk10C <= 0) {
//             if (arg0->unk31C != 0) {
//                 arg0->unk31C->matrix_physics = (u8)0;
//             }
//             arg0->unk10C = (u16)0;
//             func_1505E874(0.0f, D_800C3E78);
//         }
//         if (arg0->stunned == 0) {
//             arg0->xz_velocity = 0.0f;
//             arg0->unk81 = (u8)0;
//             arg0->unk76 = arg0->unk78 = arg0->unk7A;
//             if (arg0->in_water != 0) {
//                 arg0->y_velocity = 0.0f;
//                 if (arg0->interaction_state == 1) {
//                     func_1506B078();
//                 }
//             } else if (arg0->interaction_state == 1) {
//                 arg0->unkF8 &= 0xFFFF7FFF;
//                 arg0->gravity = 4.0f;
//             }
//             if (arg0->unk238 != 0) {
//                 arg0->unk23A = arg0->unk238;
//             }
//         } else {
//             if ((arg0->unk10B & 2) == 0) {
//                 phi_a2 = arg0->unk76;
//                 if ((arg0->unk10B & 4) != 0) {
//                     phi_a2 ^= 0x8000;
//                 }
//                 arg0->unk80 = (u8)0xA;
//                 func_150599C8(arg0, 12, phi_a2);
//             }
//         }
//     }
// }

void func_1502178C(struct127 *arg0, s32 arg1, s32 arg2);
s32 func_150A3FC4(f32, f32, s32, void *, void *);
void func_1510E8BC(void *a0, void *a1, void *a2, void *a3, void *arg4, void *arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, s32 arg10, struct127 *arg11, f32 arg12, f32 arg13, s32 arg14);
f32 func_1510F648(f32, f32, f32);
s32 func_15044380(f32 arg0, f32 arg1, f32 arg2, void *arg3, s32 arg4, s32 arg5);
void func_150B9ED0(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
struct216 *func_1507BB28(s32, s32);
u8 func_1505DADC(struct127 *arg0, u16 *arg1, u8 arg2, u8 arg3, u8 arg4);
extern u8 D_800CBDD3;

void func_15058898(struct127 *arg0, f32 arg1)
{
  f32 sp7C;
  s32 idx;
  s32 var_v1;
  s32 i;
  f32 *p;
  arg0->unk118 = -10000.0f;
  func_1502178C(arg0, 0, -1);
  if (arg0->unkF8 & 0x8000)
  {
    if (arg0->unk5 == 7)
    {
      arg0->unk28 = 0.0f;
    }
    else
    {
      arg0->unk28 = 1.0f;
    }
    return;
  }
  if (arg0->unkF8 & 0x200000)
  {
    arg0->unk180 = (s16) arg0->unk224;
    return;
  }
  var_v1 = arg0->unkF8 & 2;
  idx = arg0->unk13C;
  i = idx - 100;
  if ((idx >= 100) && (i == 0))
  {
    var_v1 = D_800CC2D0[i].unkF8 & 2;
  }
  if ((!var_v1) || (!func_150A3FC4(arg0->x_position, arg0->z_position, arg0->pad188, &arg0->unk18C, &sp7C)))
  {
    if (arg0->unkF8 & 8)
    {
      p = &arg0->unk118;
    }
    else
    {
      p = 0;
    }
    func_1510E8BC(&arg0->pad188, &arg0->unk18C, &sp7C, p, &arg0->unk184, ((u8 *) arg0) + 0x1A2, arg0->x_position, arg0->y_position, arg0->z_position, arg1, 0, arg0, -10000.0f, arg0->y_position, arg0->unkF8 & 0x04000000);
  }
  else
    if (arg0->unkF8 & 8)
  {
    arg0->unk118 = func_1510F648(arg0->x_position, arg0->y_position, arg0->z_position);
  }
  i = arg0->unkF8;
  if ((arg0->unk13C != 0) && (arg0->id != 0x9A))
  {
    i = D_800CC2D0[arg0->unk13C - 100].unkF8;
  }
  if (((i & 4) && (arg0->unk13D == 0)) && (((((arg0->unk127 != 0xFF) || (arg0->x_position != arg0->old_x_position)) || (sp7C != arg0->old_y_position)) || (0.0f != arg0->unk28)) || (arg0->z_position != arg0->old_z_position)))
  {
    f32 x;
    f32 z;
    x = arg0->x_position;
    z = arg0->z_position;
    if ((arg0->id == 0x1B) || (arg0->id == 0x7E))
    {
      D_800CBDD3 = 1;
    }
    func_15044380(arg0->old_x_position, arg0->old_y_position, arg0->old_z_position, arg0, 1, 0);
    if ((arg0->id == 0x1B) || (arg0->id == 0x7E))
    {
      D_800CBDD3 = 0;
    }
    if ((x != arg0->x_position) || (z != arg0->z_position))
    {
      arg0->unkF4 |= 0x10000;
      if (arg0->unk24E != 0)
      {
        if (arg0->id == 0xC)
        {
          func_150B9ED0(arg0, x, z, 0xFF, 0);
        }
        arg0->unk232 = arg0->unk24E;
        arg0->unk24E = 0;
        arg0->unk218 = 0;
        arg0->unk78 = (arg0->unk7A = arg0->unk76);
      }
      if (arg0->unkF8 & 0x1000)
      {
        u16 angle = func_1505A630(arg0->x_position - x, z - arg0->z_position, 0);
        arg0->unk76 = ((angle - arg0->unk76) + angle) + 0x8000;
        if (arg0->interaction_state == 0x21)
        {
          arg0->xz_velocity *= 0.8999999762f;
        }
        else
          if (arg0->interaction_state == 0x26)
        {
          arg0->xz_velocity *= 0.3000000119f;
        }
        else
          if ((arg0->interaction_state != 4) || (arg0->unk103 == 0))
        {
          arg0->xz_velocity *= 0.3000000119f;
        }
        else
        {
          u16 sp60;
          arg0->xz_velocity *= 0.8999999762f;
          arg0->unk7A = arg0->unk76;
          if (func_1505DADC(arg0, &sp60, 0, arg0->unk124 - 1, 20) != 0xFF)
          {
            arg0->unk76 = sp60;
          }
        }
        if ((arg0->unk2CC != 0) && (arg0->xz_velocity > 5.0f))
        {
          D_800D1580 = arg0->unk2CC;
          func_1506E5FC();
        }
        if (arg0->interaction_state == 4)
        {
          arg0->unk102 = 0;
          arg0->gravity = 3.0f;
        }
      }
      if ((!(arg0->unkF8 & 2)) || (!func_150A3FC4(arg0->x_position, arg0->z_position, arg0->pad188, &arg0->unk18C, &sp7C)))
      {
        func_1510E8BC(&arg0->pad188, &arg0->unk18C, &sp7C, &arg0->unk118, &arg0->unk184, ((u8 *) arg0) + 0x1A2, arg0->x_position, arg0->y_position, arg0->z_position, arg1, 0, arg0, -10000.0f, arg0->y_position, arg0->unkF8 & 0x04000000);
      }
    }
  }
  {
    u8 t = arg0->unk255;
    if (1)
    {
      if (((t != 0) && (arg0->stunned == 0)) && (sp7C < (arg0->unk180 - ((f32) (arg0->unk256 * 8)))))
      {
        arg0->y_velocity = 0.0f;
        arg0->gravity = 0.0f;
        arg0->xz_velocity = 0.0f;
        arg0->unk218 = func_1507BB28(0, arg0->unk255);
        arg0->unk21C = 0;
        arg0->unk255 = 0;
        return;
      }
    }
  }
  arg0->unk180 = sp7C;
}

void func_15058EA4(struct127 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    if (arg1 < arg0->y_position) {
        arg0->gravity = arg2;
    } else if (arg0->y_position < arg3) {
        arg0->gravity = arg4;
    }
    if (arg5 < arg0->y_velocity) {
        arg0->y_velocity = arg5;
    } else if (arg0->y_velocity < arg6) {
        arg0->y_velocity = arg6;
    }
}

void func_15058F24(struct127 *arg0, f32 arg1, f32 arg2) {
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;

    temp_f12 = arg1;
    temp_f16 = 0.5f - arg1;
    if (arg1 >= 0.5f) {
        arg1 -= 0.5f;
    }
    if (arg2) {}
    temp_f14 = arg0->unk118 + 9.0f;
    if ((arg0->y_position < (arg0->unk118 + 9.0f)) || (arg0->in_water >= 0x64)) {
        if (arg0->in_water == 0) {
            arg0->y_position = temp_f14;
            arg0->in_water = 0x64;
            arg0->gravity = temp_f16 * -6.0f;
            arg0->y_velocity *= temp_f12;
        } else {
            if ((temp_f14 + 100.0f) < arg0->y_position) {
                arg0->in_water = 0;
            }
            if (temp_f16 < 0.0f) {
                arg0->gravity = temp_f16 * -6.0f;
            } else {
                temp_f12 = arg0->y_position - ((temp_f14 + 10.0f) - (120.0f * arg1));
                if ((fabsf(temp_f12) < 2.0f) && (fabsf(arg0->y_velocity) < 2.0f)) {
                    arg0->y_position -= temp_f12 * D_800994A4;
                    arg0->y_velocity = 0.0f;
                    arg0->gravity = 1.0f;
                } else {
                    if (temp_f12 > 0.0f) {
                        if (arg0->y_velocity > 0.0f) {
                            arg0->gravity = temp_f16 * 6.0f;
                            arg0->y_velocity /= 2.0f;
                        }
                    } else {
                        temp_f12 = temp_f16 * 80.0f;
                        if (temp_f12 < arg0->y_velocity) {
                            arg0->y_velocity = temp_f12;
                        }
                        arg0->gravity = temp_f16 * -6.0f;
                    }
                }
            }
        }
        temp_f12 = -100.0f * arg1;
        if (arg0->y_velocity < temp_f12) {
            arg0->y_velocity = temp_f12;
        }
    }
}

void func_15059140(struct127 *arg0) {
    f32 sp2C;

    sp2C = 0.5f;
    D_800CC268 = 0;
    if ((arg0->unk13D == 0) || ((s32) arg0->unk21C < 0x10)) {
        if (arg0->unk223 != 8) {
            func_15059C84(arg0);
            func_1505A770(arg0);
        }
    }
    func_1505D6F0(arg0, D_800C3E78);
    func_15055E50(arg0, arg0->unk1E4);
    if (arg0->id == 8) {
        func_15056150(arg0);
    }
    if (arg0->unk13D < 0x64) {
        func_15058898(arg0, arg0->old_y_position);
    }
    if (arg0->unkF4 & 0x40000) {
        if (((arg0->unkF4 & 0x12000) != 0) || (D_800CC268 != 0)) {
            arg0->unk21C = 0U;
        }
    }
    if (arg0->unkB0 != 0) {
        func_15058F24(arg0, arg0->unkB0 * D_800994A8, 1.0f);
    }
    if (arg0->unkF8 & 0x20000) {
        func_15056258(arg0);
    }
    if (arg0->in_water != 0) {
        func_15059444(arg0);
    }
    if (arg0->id == 0xA) {
        arg0->unk180 = D_800994AC;
    }
    if ((arg0->unkF8 & 0x180000) != 0) {
        func_150511E8(arg0);
    }
    func_1505B5F8(arg0, arg0->unk180);
    if (((arg0->unkF4 & 0x1000) != 0) && (arg0->unk28 < D_800994B0)) {
        func_15056B08(arg0);
    }
    if (arg0->unkF8 & 0x80000) {
        sp2C = D_800994B4;
    }
    func_1505A250(0, 0, sp2C, &arg0->unk164, &arg0->unk168);
    if (arg0->unkD0 != 0) {
        func_1505C7D8(arg0, D_800C3E78);
    }
    if (arg0->unk13D == 0) {
        if (D_800BE9A0 >= arg0->unk10F) {
            arg0->unk10F = 0U;
        } else {
            arg0->unk10F -= D_800BE9A0;
        }
    }
    if (arg0->unk107 != 0) {
        arg0->unk107 -= 1;
    }
    if (arg0->immune != 0xFF) {
        if (D_800BE9A0 >= arg0->immune) {
            arg0->immune = 0U;
        } else {
            arg0->immune -= D_800BE9A0;
        }
    }
}

void func_150593C4(struct127 *arg0, u16 arg1, f32 arg2, f32 arg3) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;

    func_1505A184(arg1, arg2, 0, &sp2C, &sp28, &sp24);
    arg0->unk16C += (sp2C - arg0->unk16C) * arg3;
    arg0->unk170 += (sp28 - arg0->unk170) * arg3;
}

void func_15059444(struct127 *arg0) {
    s32 sp1C;
    s32 id;
    s32 tmp;

    sp1C = -1;

    switch (D_800BE9F0) {
        case 4:
            func_150593C4(arg0, 0x4000, 50.0f, 0.05999999865889549f);
            break;
        case 6:
            sp1C = func_15083E0C(28);
            break;
        case 41:
            if (func_150A29C8(D_800C3E78, 0x4028) == 0) {
                id = 12;
            } else if (func_150A29C8(D_800C3E78, 0x400E) == 0) {
                id = 10;
            } else if (func_150A29C8(D_800C3E78, 0x400D) == 0) {
                id = 9;
            } else {
                id = 6;
            }
            sp1C = func_15083E0C(id);
            // FIXME: fakematch to force regalloc
            dummy_label_858530:;
            break;
        case 43:
            sp1C = func_15083E0C(18);
            break;
        case 65:
            if (func_150A29C8(D_800C3E78, 0x401F) == 0) {
                id = 27;
            } else {
                id = 26;
            }
            sp1C = func_15083E0C(id);
            break;
    }

    if (sp1C != -1) {
        func_150611E8(arg0, sp1C);
    }
}

void func_1505959C(struct127 *arg0, s32 arg1) {
    struct252 *temp_v0;
    s32 *temp_v1;
    s32 variant;

    arg0->unkF8 &= 0xFF7FFFFF;
    arg0->unk13D = arg1 + 100;
    arg0->unk21C = 0;
    arg0->gravity = 4.0f;
    arg0->unk25C &= ~8;

    D_800CC2D0[arg1].unk83 = 0xFF;
    D_800CC2D0[arg1].disable_run = 0xFF;
    variant = 0;
    if (arg0->id == 0x57) {
        variant = 1;
    }
    if (D_800CC2D0[arg1].id == 0x9B) {
        variant = 2;
    }
    if (arg0->id == 0x5E) {
        variant = 3;
    }
    if (arg0->id == 0x3C) {
        variant = 4;
        D_800CC2D0[arg1].unk13C = (u8)0;
        D_800CC2D0[arg1].unk76 = func_1505A630(arg0->x_position - D_800CC2D0[arg1].x_position, D_800CC2D0[arg1].z_position - arg0->z_position, 0);
    }
    if (arg0->id == 0x89) {
        variant = 5;
    }
    func_1505E650(&D_800CC2D0[arg1], D_800860C0[variant], D_800860CC[variant], 0.0f, 0.0f, 0.0f, 0);
    arg0->stunned = 0xFE;
    arg0->unk105 = 0;
    arg0->unk106 = func_1505E7CC(D_800860E4[variant], arg0);
    arg0->unk84.uh = 0xFFFF;
    func_1505E874(D_800C3E78, arg0);
    temp_v1 = (s32*)D_800D1588[arg0->id];
    if (temp_v1 != 0) {
        temp_v0 = (struct252 *)*(temp_v1 - 2);
        if ((temp_v0 != 0) && ((temp_v0[arg0->unk106].unk15 & 2) != 0)) {
            arg0->unk7A = D_800CC2D0[arg1].unk7A;
        }
        if ((temp_v0 == 0) || ((temp_v0[arg0->unk106].unk15 & 1) == 0)) {
            arg0->unk13E = (arg0->unk7A - D_800CC2D0[arg1].unk7A) >> 8;
        } else {
            arg0->unk13E = 0;
        }
    } else {
        arg0->unk13E = (arg0->unk7A - D_800CC2D0[arg1].unk7A) >> 8;
    }
}

void func_150597FC(struct127 *arg0) {
    s32 var_a1;
    s32 mask;
    struct126 *temp_a2;
    struct127 *temp_v0;
    s32 temp_v1;

    mask = D_800CC268;
    for (var_a1 = 0; var_a1 < D_8008FD8C; var_a1++) {
        if (((1 << var_a1) & mask) && (temp_v0 = &D_800CC2D0[var_a1], (temp_v0->unk13C == 0)) && (temp_v0->disable_run == 0) && (temp_v0->interaction_state == 1) && (temp_v0->stunned == 0) && (temp_v0->unk127 != 0xFF)) {
            break;
        }
    }
    if (var_a1 != D_8008FD8C) {
        temp_v0 = &D_800CC2D0[var_a1];
        if (temp_v0->unk13C == 0) {
            temp_a2 = temp_v0->unk31C;
            if ((temp_a2->unk27 == 0) && (temp_v0->health != 0) && (arg0->unk13D == 0) && ((arg0->stunned != 0) || (arg0->unk25C & 0x1000) || (arg0->id == 0x57)) && (arg0->unk28 == 0.0f) && (temp_v0->unk28 == 0.0f) && (arg0->unk25C & 8) && (temp_a2->unk19B == 0) && ((temp_v1 = arg0->id, (temp_v1 != 0xA9)) || (*(u8 *)((u8 *)temp_v0 + 0x128) == 0)) && ((temp_v1 != 0xA8) || (*(u8 *)((u8 *)temp_v0 + 0x128) != 0))) {
                temp_v0->unk13C = D_800C3E78 + 0x64;
                temp_v0->xz_velocity = 0.0f;
                *(s16 *)((u8 *)temp_a2 + 0x18) = 0;
                func_1505959C(arg0, var_a1);
            }
        }
    }
}

u8 func_150599C8(struct127 *arg0, u8 arg1, u16 arg2) {
    s16 temp_a3;
    s16 temp_t6;
    s16 temp_t8;
    s32 temp_v0;
    s32 temp_lo;
    s16 signedDelta;

    temp_v0 = (s32) (((arg1 << 8) + arg0->unk1E8) * D_800CC264) / 0x64;
    temp_a3 = arg2 - arg0->unk7A;
    if (arg0->unk1EA != 0) {
        temp_lo = (s32) (arg0->unk1EA * D_800CC264) / 0x64;
        temp_t6 = arg0->unk1EC + temp_lo;
        temp_t8 = arg0->unk1EC - temp_lo;
        if (temp_t6 < temp_a3) {
            temp_a3 = temp_t6;
        }
        if (temp_a3 < temp_t8) {
            temp_a3 = temp_t8;
        }
    }

    signedDelta = temp_a3;
    if (temp_a3 < 0) {
        temp_a3 = temp_a3 ^ 0xFFFF;
    }
    if (temp_a3 < temp_v0) {
        temp_v0 = temp_a3;
    }
    if ((arg0->unkF4 & 1) == 0) {
        if (arg0->unk80 != 0) {
            if (signedDelta < 0) {
                arg0->unk7A -= temp_v0;
            } else {
                arg0->unk7A += temp_v0;
            }
        }
    }
    return temp_a3 >> 8;
}

u8 func_15059B54(struct127 *arg0, u16 arg1) {
    s16 temp_t8;
    s16 temp_t8_2;
    s16 temp_t2;
    s32 temp_lo;
    s16 absDiff;
    s16 signedDelta;

    temp_t8 = arg0->unk78 - arg0->unk76;
    absDiff = temp_t8;
    if (absDiff < 0) {
        absDiff = absDiff ^ 0xFFFF;
    }
    if (arg0->unk1EA != 0) {
        temp_lo = (s32) (arg0->unk1EA * D_800CC264) / 0x64;
        temp_t8_2 = arg0->unk1EC + temp_lo;
        temp_t2 = arg0->unk1EC - temp_lo;
        if (temp_t8_2 < temp_t8) {
            temp_t8 = temp_t8_2;
        }
        if (temp_t8 < temp_t2) {
            temp_t8 = temp_t2;
        }
    }

    signedDelta = temp_t8;
    if (temp_t8 < 0) {
        temp_t8 = temp_t8 ^ 0xFFFF;
    }
    if (temp_t8 < arg1) {
        arg1 = temp_t8;
    }
    if ((arg0->unkF4 & 1) == 0) {
        if (signedDelta < 0) {
            arg0->unk76 -= arg1;
            arg0->unk1EC = -arg1;
        } else {
            arg0->unk76 += arg1;
            arg0->unk1EC = arg1;
        }
    }
    return absDiff >> 8;
}
// NON-MATCHING: 90% there, missing an s16 cast
// u8 func_15059B54(struct127 *arg0, u16 arg1) {
//     s16 temp_t8;
//     s16 temp_t8_2;
//     s16 temp_t2;
//     s32 temp_lo;
//     s16 phi_a2;
//
//     temp_t8 = arg0->unk78 - arg0->unk76;
//     phi_a2 = temp_t8;
//     if (phi_a2 < 0) {
//         phi_a2 = phi_a2 ^ 0xFFFF;
//     }
//     if (arg0->unk1EA != 0) {
//         temp_lo = (s32) (arg0->unk1EA * D_800CC264) / 0x64;
//         temp_t8_2 = arg0->unk1EC + temp_lo;
//         temp_t2 = arg0->unk1EC - temp_lo;
//         if (temp_t8_2 < temp_t8) {
//             temp_t8 = temp_t8_2;
//         }
//         if (temp_t8 < temp_t2) {
//             temp_t8 = temp_t2;
//         }
//     }
//
//     if (temp_t8 < 0) {
//         temp_t8 ^= 0xFFFF;
//     }
//     if (temp_t8 < arg1) {
//         arg1 = temp_t8;
//     }
//     if ((arg0->unkF4 & 1) == 0) {
//         if (temp_t8 < 0) {
//             arg0->unk76 -= arg1;
//             arg0->unk1EC = -arg1;
//         } else {
//             arg0->unk76 += arg1;
//             arg0->unk1EC = arg1;
//         }
//     }
//     return phi_a2 >> 8;
// }

typedef struct Game83300Point {
    f32 values[3];
} Game83300Point;

typedef struct Game83300Triangle {
    Game83300Point vertices[3];
} Game83300Triangle;

void func_15049350(Game83300Triangle);

void func_15059C84(void *arg0) {
    f32 x;
    f32 z;
    f32 y;
    f32 vertical;
    u16 angle;
    Game83300Triangle triangle;
    f32 speed;
    s32 i;
    f32 length;
    f32 scale;

    speed = *(f32 *)((u8 *)arg0 + 0x3C);
    angle = *(u16 *)((u8 *)arg0 + 0x76);
    if (D_800D1548 == 1) {
        return;
    }
    if (*((u8 *)arg0 + 0x1FC) & 8) {
        *((u8 *)arg0 + 0x1FC) &= ~8;
        return;
    }
    if (*((u8 *)arg0 + 0x127) != 0xFF) {
        if (*(s16 *)(*(u8 **)((u8 *)arg0 + 0x31C) + 0x46) > 0) {
            angle += *(u16 *)(*(u8 **)((u8 *)arg0 + 0x31C) + 0x10);
            *(u16 *)((u8 *)arg0 + 0x7A) = angle;
        }
    }
    if (*(f32 *)((u8 *)arg0 + 0x3C) < 1.0f && *(f32 *)((u8 *)arg0 + 0x3C) >= 0.0f &&
        !(*((u8 *)arg0 + 0x1FC) & 1)) {
        if (*((u8 *)arg0 + 0x7E) < 0xFA) {
            *((u8 *)arg0 + 0x7E) += 1;
        }
        if (*(f32 *)((u8 *)arg0 + 0x164) == 0.0f && *(f32 *)((u8 *)arg0 + 0x168) == 0.0f &&
            *(f32 *)((u8 *)arg0 + 0x16C) == 0.0f && *(f32 *)((u8 *)arg0 + 0x170) == 0.0f) {
            *(f32 *)((u8 *)arg0 + 0x1F4) = 0.0f;
            return;
        }
    } else {
        *((u8 *)arg0 + 0x7E) = 0;
    }
    if ((*((u8 *)arg0 + 0xAD) != 0 && (*(u8 **)((u8 *)arg0 + 0x31C) == NULL ||
                                        *(*(u8 **)((u8 *)arg0 + 0x31C) + 0x21) == 0)) ||
        (*(u16 *)((u8 *)arg0 + 0x22C) & 8)) {
        x = *(f32 *)((u8 *)arg0 + 0xB8);
        if (x > 90.0f) {
            x = 90.0f;
        } else if (x < -90.0f) {
            x = -90.0f;
        }
    } else {
        x = 0.0f;
    }
    if (*((u8 *)arg0 + 0x1FC) & 1) {
        *((u8 *)arg0 + 0x1FC) &= ~1;
        if (D_800D1550) {
            speed = 2.0f * (*(f32 *)((u8 *)arg0 + 0x14C) * *(f32 *)((u8 *)arg0 + 0x1F4) / D_800D1550);
        }
        *(f32 *)((u8 *)arg0 + 0x1F4) = *(f32 *)((u8 *)arg0 + 0x3C);
        angle = *(u16 *)((u8 *)arg0 + 0x7A);
    }
    *(f32 *)((u8 *)arg0 + 0x1F4) = speed;
    func_1505A184(angle, speed, x, &x, &z, &y);
    x += *(f32 *)((u8 *)arg0 + 0x164) + *(f32 *)((u8 *)arg0 + 0x16C);
    z += *(f32 *)((u8 *)arg0 + 0x168) + *(f32 *)((u8 *)arg0 + 0x170);
    if (*((u8 *)arg0 + 0xAD) == 0xA) {
        y = 0.0f;
    }
    if ((*(s32 *)arg0 != 1 || *(*(u8 **)((u8 *)arg0 + 0x31C) + 0x4F) == 0) &&
        *(f32 *)((u8 *)arg0 + 0x28) <= 0.0f && *((u8 *)arg0 + 0xAD) == 0 &&
        !(*(u16 *)((u8 *)arg0 + 0x22C) & 8)) {
        for (i = 0; i < 3; i++) {
            triangle.vertices[i].values[0] = *(s16 *)((u8 *)arg0 + 0x18C + i * 6);
            triangle.vertices[i].values[1] = *(s16 *)((u8 *)arg0 + 0x18E + i * 6);
            triangle.vertices[i].values[2] = *(s16 *)((u8 *)arg0 + 0x190 + i * 6);
        }
        func_15049350(triangle);
        if (D_800CC214 != 0.0f) {
            vertical = (-D_800CC210 * x - D_800CC218 * z) / D_800CC214;
            if (vertical > 4.0f || *(s32 *)arg0 != 1 ||
                *(*(u8 **)((u8 *)arg0 + 0x31C) + 0x4E) != 2) {
                length = sqrtf(x * x + z * z + vertical * vertical);
                if (length != 0.0f) {
                    scale = sqrtf(x * x + z * z);
                    x = x * scale / length;
                    y = vertical * scale / length;
                    z = z * scale / length;
                }
            }
        }
    }
    x *= D_800D1550;
    z *= D_800D1550;
    y *= D_800D1550;
    *(f32 *)((u8 *)arg0 + 0x14) += x;
    *(f32 *)((u8 *)arg0 + 0x1C) += z;
    *(f32 *)((u8 *)arg0 + 0x18) += y;
}
f32 func_150AD780(f32);
f32 func_150AD78C(f32);
void func_1505A184(u16 arg0, f32 arg1, f32 arg2, f32 *arg3, f32 *arg4, f32 *arg5) {
    f32 s2;
    union { f32 f; f64 d; } ang2;
    f32 c2;

    arg1 = arg1 * 0.5f;
    arg2 = arg2 * D_800994B8;
    *arg5 = func_150AD78C(arg2) * -arg1;
    arg1 = func_150AD780(arg2) * arg1;
    ang2.f = (f32)arg0 * D_800994BC;
    s2 = func_150AD780(ang2.f);
    c2 = func_150AD78C(ang2.f);
    *arg3 = s2 * arg1;
    *arg4 = -c2 * arg1;
}
void func_1505A250(f32 arg0, f32 arg1, f32 arg2, f32 *arg3, f32 *arg4) {
    f32 dx = arg0 - *arg3, dy = arg1 - *arg4, mag, sx, sy;
    if (dx == 0.0f) { if (dy == 0.0f) return; }
    mag = sqrtf((dx * dx) + (dy * dy));
    arg2 *= D_800D1550;
    sx = fabsf((dx / mag) * arg2);
    sy = fabsf((dy / mag) * arg2);
    if (0.0f <= dx) { *arg3 = *arg3 + sx; if (arg0 < *arg3) *arg3 = arg0; }
    else            { *arg3 = *arg3 - sx; if (*arg3 < arg0) *arg3 = arg0; }
    if (0.0f <= dy) { *arg4 = *arg4 + sy; if (arg1 < *arg4) *arg4 = arg1; }
    else            { *arg4 = *arg4 - sy; if (*arg4 < arg1) *arg4 = arg1; }
}


// PERMUTER CANDIDATE best 3576 (control flow BYTE-PERFECT; entire diff is a systematic FP-register
// rename: target promotes aa->callee-saved $f20 (+ frame to save it) & bb->$f14 via mtc1 up front,
// but IDO keeps this leaf's aa/bb in temps $f2/$f0 with an arg-home spill. Copy-param-to-local and
// scale-hoist tricks did not force the f20 promotion. Reconstruction below is logically exact.)
// f32 func_1505A3A8(f32 arg0, void *arg1, f32 arg2, f32 arg3, u8 arg4) {
//     struct127 *obj = (struct127 *)arg1;
//     f32 aa = arg2 * D_800D1550[0];
//     f32 bb = arg3 * D_800D1550[0];
//     f32 vel;
//     if (D_800CC27C != 0) {
//         if (obj->unk28 < 5.0f) {
//             if (obj->interaction_state != 0x1E) aa = 0.0f;
//         }
//     }
//     if (obj->interaction_state == 1) {
//         if (obj->in_water != 0) { aa = 2.0f; bb = bb * D_800994C0; }
//         if (obj->unkA8 != 0) bb = bb * 0.25f;
//         if (obj->unk81 != 0) {
//             if (obj->unk81 & 0x40) {
//                 if (obj->unk81 & 0x20) bb = 0.0f; else bb = D_800994C4;
//             } else {
//                 bb = bb * D_800994C8;
//                 if (arg4 >= 0x2E) aa = aa * 0.5f;
//                 if (arg4 >= 0x5B) arg0 = arg0 * 0.5f;
//             }
//         }
//         if (obj->unkAE != 0) bb = bb * D_800994CC;
//     }
//     if (arg0 < 0.0f) {
//         vel = obj->xz_velocity;
//         if (0.0f < vel) bb = bb + aa; else bb = aa;
//     } else {
//         vel = obj->xz_velocity;
//         if (vel < 0.0f) aa = aa + bb;
//     }
//     if (arg0 < vel) {
//         obj->xz_velocity = vel - bb;
//         if (obj->xz_velocity < arg0) obj->xz_velocity = arg0;
//     } else {
//         if ((arg0 - 1.0f) < vel) aa = aa * D_800994D0;
//         obj->xz_velocity = vel + aa;
//         if (arg0 < obj->xz_velocity) obj->xz_velocity = arg0;
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_1505A3A8.s")

f32 func_1505A5CC(struct49 *arg0) {
    f32 x, y;
    f32 ret;

    // regalloc nonsense
    ret = arg0->unk2;
    x = ret;
    y = arg0->unk3;

    x = sqrtf(x*x + y*y) * D_800994D4;
    ret = x;

    if (ret > 35.0f) {
        ret = 35.0f;
    }

    return ret;
}

u16 func_1505A630(f32 arg0, f32 arg1, s32 arg2) {
    return ((u16) (u32) (func_150484A0(-arg0, arg1) * D_800994D8)) + 16384;
}

f32 func_1505A6F8(struct127 *arg0, struct127 *arg1) {
    f32 x, z;

    x = arg0->x_position - arg1->x_position;
    x *= x;
    z = arg0->z_position - arg1->z_position;
    z *= z;
    return sqrtf(x + z);
}

// distance between two objects?
f32 func_1505A72C(struct127 *arg0, struct127 *arg1) {
    f32 x, y, z;

    x = arg0->x_position - arg1->x_position;
    x *= x;
    z = arg0->z_position - arg1->z_position;
    z *= z;
    y = arg0->y_position - arg1->y_position;
    y *= y;
    return sqrtf(x + z + y);
}

void func_1505A770(void *arg0) {
    f32 velocity;
    f32 decrement;
    s32 count;
    s32 i;

    if ((*(f32 *)((u8 *)arg0 + 0x28) != 0.0f) ||
        (velocity = *(f32 *)((u8 *)arg0 + 0x20), velocity > 0.0f)) {
        decrement = *(f32 *)((u8 *)arg0 + 0x24) * 0.1f;
        count = (s32)(D_800D1550 / 0.1f);
        for (i = 0; i < count; i++) {
            *(f32 *)((u8 *)arg0 + 0x20) -= decrement;
            *(f32 *)((u8 *)arg0 + 0x18) +=
                *(f32 *)((u8 *)arg0 + 0x20) * 0.05f;
        }
        if (*(f32 *)((u8 *)arg0 + 0x20) < -500.0f) {
            *(f32 *)((u8 *)arg0 + 0x20) = -500.0f;
        }
    } else {
        *(f32 *)((u8 *)arg0 + 0x20) = velocity -
            (*(f32 *)((u8 *)arg0 + 0x24) * D_800D1550);
        velocity = *(f32 *)((u8 *)arg0 + 0x20); *(f32 *)((u8 *)arg0 + 0x18) += velocity * D_800D1550 * 0.5f;
        if (velocity < -500.0f) {
            *(f32 *)((u8 *)arg0 + 0x20) = -500.0f;
        }
    }
}
// a big one, but could be ok
#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_1505A9AC.s")

#define arg0 ((u8 *)arg0_s)
void func_1505B5F8(struct127 *arg0_s, f32 arg1) {
    f32 previousSpeed;
    f32 height;
    f32 factor;
    f32 speed;
    s32 count;
    s32 remaining;
    s32 step;
    s32 sound;
    u8 *owner;

    previousSpeed = *(f32 *)(arg0 + 0x28);
    if (!(*(s32 *)(arg0 + 0xF8) & 0x8000)) {
        if (arg0[0x252] != 0 && (f32)(arg0[0x253] * 8) < *(f32 *)(arg0 + 0x1CC) - *(f32 *)(arg0 + 0x18) &&
            arg0[0x104] == 0) {
            count = arg0[0x252];
            arg0[0x104] = 0xFE;
            arg0[0x105] = 0;
            arg0[0x106] = func_1505E7CC(count, (struct127 *)arg0);
            *(s16 *)(arg0 + 0x84) = 0xFF;
            func_1505E874(D_800C3E78, (struct127 *)arg0);
            *(f32 *)(arg0 + 0x18) = *(f32 *)(arg0 + 0x30);
        }
        height = *(f32 *)(arg0 + 0x18);
        *(f32 *)(arg0 + 0x28) = height - arg1;
        if (height <= arg1) {
            *(s32 *)(arg0 + 0xF4) |= 0x8000;
            step = ((u8 *)D_800B0DF0)[0x13];
            if (step != 0 && previousSpeed != 0.0f) {
                func_15174690(D_800C3E78, 0, 0x1000 / step, 0, 0x199, 4, 0xAA, 0xFF, 0);
            }
            *(f32 *)(arg0 + 0x18) = arg1;
            *(f32 *)(arg0 + 0x28) = 0.0f;
            if (arg0[0x104] != 0 || !(*(s32 *)(arg0 + 0xF8) & 0x10000)) {
                if (*(s32 *)arg0 == 1) {
                    if (*(f32 *)(arg0 + 0x20) < -6.0f) {
                        *(f32 *)(arg0 + 0x20) = -6.0f;
                    }
                } else {
                    if (*(f32 *)(arg0 + 0x1CC) - *(f32 *)(arg0 + 0x18) > 450.0f) {
                        remaining = arg0[0x1CA];
                        if (remaining > 0) {
                            owner = *(u8 **)(arg0 + 0x144);
                            if (owner != NULL && !(owner[0xE] & 0x80)) {
                                arg0[0x1CA] = remaining - 1;
                            }
                        }
                    }
                    *(f32 *)(arg0 + 0x1CC) = arg1;
                    if (arg0[0x104] == 0 && (speed = *(f32 *)(arg0 + 0x24)) > 0.5f) {
                        if ((*(s32 *)(arg0 + 0xF8) & 0x800) && previousSpeed != 0.0f &&
                            *(f32 *)(arg0 + 0x20) < -4.0f * speed * D_800D1550) {
                            sound = *(s32 *)(arg0 + 0x2CC);
                            factor = (f32)(s8)arg0[0x2CB] * D_8009950C;
                            *(f32 *)(arg0 + 0x20) = (0.0f - *(f32 *)(arg0 + 0x20)) * factor;
                            if (sound != 0 && *(f32 *)(arg0 + 0x20) > 5.0f) {
                                D_800D1580 = sound;
                                func_1506E5FC();
                            }
                        } else if (*(s32 *)arg0 != 7) {
                            *(f32 *)(arg0 + 0x20) = -4.0f * D_800D1550;
                        }
                    } else {
                        if (*(s32 *)(arg0 + 0xF4) & 0x80) {
                            *(f32 *)(arg0 + 0x20) = -4.0f;
                        } else if (previousSpeed == 0.0f ||
                                   -1.5f * *(f32 *)(arg0 + 0x24) * D_800D1550 < *(f32 *)(arg0 + 0x20)) {
                            *(f32 *)(arg0 + 0x20) = 0.0f;
                        } else {
                            factor = (f32)(s8)arg0[0x2CB] * D_80099510;
                            *(f32 *)(arg0 + 0x20) = (0.0f - *(f32 *)(arg0 + 0x20)) * factor;
                        }
                        *(f32 *)(arg0 + 0x3C) *= D_80099514;
                    }
                }
            }
        }
    }
}
#undef arg0
// void func_1505B5F8(struct127 *arg0, f32 arg1) {
//     f32 tmp;
//     u8 temp_v0_2;
//
//     if ((arg0->unkF8 & 0x8000) == 0) {
//         if (arg0->unk252 != 0) {
//             if ((arg0->unk253 * 8) < (arg0->unk1CC - arg0->y_position)) {
//                 if (arg0->stunned == 0) {
//                     arg0->stunned = (u8)0xFEU;
//                     arg0->unk105 = (u8)0;
//                     arg0->unk106 = func_1505E7CC(arg0->unk252, arg0);
//                     arg0->unk84.uh = (u16)0xFF;
//                     func_1505E874(D_800C3E78, arg0);
//                     arg0->y_position = (f32) arg0->old_y_position;
//                 }
//             }
//         }
//         arg0->unk28 = arg0->y_position - arg1;
//         if (arg0->y_position <= arg1) {
//             arg0->unkF4 |= 0x8000;
//             temp_v0_2 = D_800B0DF0->unk13;
//             if (temp_v0_2 != 0) {
//                 if (arg0->unk28 != 0.0f) {
//                     func_15174690(D_800C3E78, 0, 0x1000 / (s32) temp_v0_2, 0, 0x199, 4, 0xAA, 0xFF, 0); // 0.0f, arg1,
//                 }
//             }
//             arg0->y_position = arg1;
//             tmp = arg0->unk28;
//             arg0->unk28 = 0.0f;
//             if ((arg0->stunned != 0) || ((arg0->unkF8 << 0xF) >= 0)) {
//                 if (arg0->interaction_state == 1) {
//                     if (arg0->y_velocity < -6.0f) {
//                         arg0->y_velocity = -6.0f;
//                     }
//                 } else {
//                     if ((arg0->unk1CC - arg0->y_position) > 450.0f) {
//                         if (arg0->health > 0) {
//                             if ((arg0->unk144 != 0) && ((arg0->unk144->unkE & 0x80) == 0)) {
//                                 arg0->health = (u8) (arg0->health - 1);
//                             }
//                         }
//                     }
//                     arg0->unk1CC = arg1;
//                     if ((arg0->stunned == 0) && (arg0->gravity > 0.5f)) {
//                         if (((arg0->unkF8 & 0x800) != 0) && (tmp != 0.0f) && ((arg0->y_velocity < (-4.0f * arg0->gravity * D_800D1550)))) {
//                             arg0->y_velocity = ((0.0f - arg0->y_velocity) * ((f32) arg0->unk2CB * D_8009950C));
//                             if ((arg0->unk2CC != 0) && (arg0->y_velocity > 5.0f)) {
//                                 D_800D1580 = arg0->unk2CC;
//                                 func_1506E5FC();
//                             }
//                         } else if (arg0->interaction_state != 7) {
//                             arg0->y_velocity = -4.0f * D_800D1550;
//                         }
//                     } else {
//                         if ((arg0->unkF4 & 0x80) != 0) {
//                             arg0->y_velocity = -4.0f;
//                         } else if ((tmp == 0.0f) || (((-1.5f * arg0->gravity * D_800D1550) < arg0->y_velocity))) {
//                             arg0->y_velocity = 0.0f;
//                         } else {
//                             arg0->y_velocity = (0.0f - arg0->y_velocity) * ((f32) arg0->unk2CB * D_80099510);
//                         }
//                         arg0->xz_velocity *= D_80099514;
//                     }
//                 }
//             }
//         }
//     }
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_1505B9C4.s")

s16 func_1505C140(struct127 *arg0, struct127 *arg1) {
    s16 ret = func_1505A630(arg0->x_position - arg1->x_position, arg1->z_position - arg0->z_position, 0);
    ret -= arg1->unk7A;

    if (ret < 0) {
        ret = -ret;
    }
    return ret;
}

struct252 *func_1505C1A4(struct127 *arg0) {
    s32 *temp_v1;
    struct252 *ret;

    if ((arg0->id != 0xFF) && (D_800D1588[arg0->id] != 0)) {
        temp_v1 = (s32*)D_800D1588[arg0->id] - 3;
        ret = *temp_v1;
    } else {
        ret = &D_8009A9F8;
    }
    return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_1505C1E4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_1505C7D8.s")

s32 func_1505D024(struct127 *arg0, s32 arg1, u16 arg2, s32 arg3) {
    s32 pad;
    s32 ret;
    s32 temp_lo;
    struct252 *sp30;

    sp30 = &D_8009A9F8;
    if (D_800C35EA == 1) {
        return 0;
    }
    if ((arg1 & 0x20000) && (arg0->immune != 0)) {
        return 0;
    }
    if ((arg1 & 0x40000) && (arg0->stunned != 0)) {
        return 0;
    }
    if (((arg1 << 0xB) >= 0) && (arg0->health == 0)) {
        return 0;
    }
    if (arg3 == -1) {
        D_800D1340 = (u8)0;
    } else {
        D_800D1340 = arg3 + 1;
    }
    D_800D1292 = arg2;
    D_800D1296 = arg2;
    if (arg1 & 0x10000) {
        sp30 = func_1505C1A4(D_800D154C);
    }

    temp_lo = arg0 - D_800CC2D0;
    ret = 1 << temp_lo;
    if (arg1 & 0x80000) {
        ret = func_1505C1E4(&D_800D121C, arg0, sp30, arg1 & 0xFF, temp_lo + 1, 0, 7);
    } else {
        func_1505B9C4(&D_800D121C, arg0, (s32)sp30, (s32)sp30, arg1 & 0xFF, temp_lo + 1, 7);
    }
    return ret;
}

void func_1505D1C4(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4, u16 arg5, s32 arg6, s32 arg7) {
    s32 pad;
    s32 tmp;

    tmp = D_800C3E78;
    func_1505F188(&D_800D121C);
    D_800D1230 = arg0;
    D_800D1234 = arg1;
    D_800D1238 = arg2;
    D_800D1340 = arg4 + 1;
    D_800D1359 = arg6;
    D_800D1292 = arg5;
    D_800C3E78 = 25;
    D_800D1510 = arg7;
    D_800D1330 = 100.0f;
    D_800D12EC = (arg3 & 0xFFFF) + 1;
    D_800D1368 = 0.5f;
    D_800D136C = 0.5f;
    func_1505C7D8(&D_800D121C, 25);
    D_800C3E78 = tmp;
}

void func_1505D2B8(struct127 *arg0, u8 arg1) {
    struct253 *temp_v0;

    temp_v0 = &D_8009A6D8[arg1];
    arg0->y_velocity = temp_v0->unk18;
    arg0->xz_velocity = temp_v0->unk14;
    arg0->gravity = temp_v0->unk1C;
    arg0->stunned = 0xFE;
    arg0->unk105 = 0;
    arg0->unk106 = func_1505E7CC(arg0->unk10E & 0x7F, arg0);
    arg0->unk10E = 0xFF;
    func_1505E874(D_800C3E78, arg0);
}

f32 func_1505D34C(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4) {
    f32 sp24;
    s32 pad;
    f32 temp_f0;
    f32 temp_f12;

    if (arg3 != 1.0f) {
        arg1 = arg1 * arg3;
        arg2 = arg2 * arg3;
    }
    temp_f12 = (arg0 - 90.0f) * D_80099520; // 0.01745329238474369f;
    sp24 = func_150AD780(temp_f12);
    temp_f0 = func_150AD78C(temp_f12);
    *arg4 = (-arg1 * temp_f0) + (arg2 * sp24);
    return (arg1 * sp24) + arg2 * temp_f0;
}

extern u8 D_800CC26C;
extern u8 D_800CC26D;

void func_1505D408(struct127 *arg0, struct127 *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7) {
    f32 temp_f0;
    f32 temp_f18;
    f32 var_f2;
    s32 temp_a2;
    s32 temp_t0;
    u8 var_v0;
    u8 var_v1;

    var_f2 = 0.400000006f;
    var_v0 = 0;
    var_v1 = 0;
    if (arg5 < 1.0f) {
        arg5 = 1.200000048f;
    }
    if ((arg2 == 0.0f) && (arg4 == 0.0f)) {
        arg2 = 1.0f;
    }
    D_800CC268 |= 1 << arg7;
    if (arg1->unkF8 & 0x10) {
        D_800CC26C = arg7;
        var_f2 = 0.8000000119f;
        if ((arg1->id == 0x61) && (arg0->interaction_state == 1)) {
            goto end;
        }
    }
    if (arg0->unkF8 & 1) {
        var_v0 = 1;
    }
    if (arg0->unkF8 & 0x200) {
        var_v0 |= 2;
    }
    if (arg1->unkF8 & 1) {
        var_v1 = 1;
    }
    if (arg1->unkF8 & 0x200) {
        var_v1 |= 2;
    }
    if (var_v1 >= var_v0) {
        if ((arg1->unkF8 & 0x400) && (arg1->unk28 == 0.0f)) {
            D_800CC26D = arg7 + 100;
        }
        temp_f0 = sqrtf(arg5);
        temp_f18 = 1.0f / temp_f0;
        arg2 *= temp_f18;
        arg3 *= temp_f18;
        arg4 *= temp_f18;
        arg6 -= temp_f0;
        arg6 *= var_f2;
        arg0->x_position += arg6 * arg2;
        arg0->z_position += arg6 * arg4;
        arg0->y_position += arg6 * arg3;
    }
end:
    ;
}
void func_1505D408(struct127 *arg0, struct127 *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7);

void func_1505D5D0(struct127 *arg0, struct127 *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7,
                   f32 arg8, f32 arg9, f32 arg10, f32 arg11, f32 arg12) {
    f32 temp_f0;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distSq;
    f32 sumRadius;

    temp_f0 = func_1505D34C(arg1->unk40, arg11, arg12, arg1->xz_scale, &arg12);
    arg8 *= arg1->xz_scale;
    arg9 *= arg1->y_scale;
    dx = arg2 - (arg1->x_position + temp_f0);
    dz = arg4 - (arg1->z_position + arg12);
    dy = arg3 - (arg1->y_position + (arg10 * arg1->y_scale));
    dy *= (arg5 + (arg8 / arg9)) * 0.5f;
    sumRadius = arg6 + arg8;
    distSq = ((dx * dx) + (dy * dy)) + (dz * dz);
    if (distSq < (sumRadius * sumRadius)) {
        func_1505D408(arg0, arg1, dx, dy, dz, distSq, sumRadius, arg7);
    }
}
void func_1505D6F0(struct127 *arg0, s32 arg1)
{
  f32 dx;
  f32 dy;
  f32 dz;
  f32 distSq;
  f32 sumRadius;
  f32 radius;
  f32 objRadius;
  f32 height;
  f32 x;
  f32 y;
  f32 z;
  s32 i;
  s32 count;
  struct127 *obj;
  struct126 *temp;
  D_800CC268 = 0;
  D_800CC26C = 0;
  D_800CC26D = 0;
  count = 25;
  if (((((((arg0->interaction_state != 1) || ((D_800CC2D0[arg1].unk31C->unk4E == 0) && (D_800CC2D0[arg1].unk31C->unk27 == 0))) && (arg0->unk13D < 100)) && (arg0->unkE4 != 0)) && (arg0->unk65 == 0)) && (!(arg0->unkF8 & 0x01000000))) && (!(arg0->unkF8 & 0x4000)))
  {
    radius = arg0->unkE4;
    height = arg0->unkEC;
    x = arg0->x_position;
    y = arg0->y_position + ((s16) arg0->unkE8);
    z = arg0->z_position;
    if (arg0->unkF8 & 0x02000000)
    {
      count = 1;
    }
    for (i = 0; i < count; i++)
    {
      obj = &D_800CC2D0[i];
      if (obj->interaction_state == 0)
      {
        continue;
      }
      if (obj->unkE4 < 2)
      {
        continue;
      }
      if ((obj->unkF8 & 0x02000000) && (arg0->unk127 == 0xFF))
      {
        continue;
      }
      if (obj->unkF8 & 0x01000000)
      {
        continue;
      }
      if (i == arg1)
      {
        continue;
      }
      if (obj->unk65 == (arg1 + 1))
      {
        continue;
      }
      if (obj->unk13D >= 100)
      {
        continue;
      }
      if (obj->unk2FA == 0)
      {
        continue;
      }
      objRadius = obj->unkE4;
      sumRadius = radius + objRadius;
      dx = x - obj->x_position;
      dz = z - obj->z_position;
      dy = y - (obj->y_position + ((s16) obj->unkE8));
      dy *= (height + obj->unkEC) * 0.5f;
      if (((((D_800BE616 != 0) && ((((dx * dx) + (dy * dy)) + (dz * dz)) < 30000.0f)) && (arg0->interaction_state == 1)) && (obj->interaction_state == 1)) && ((*(((u8 *) obj) + 0x128)) != (*(((u8 *) arg0) + 0x128))))
      {
        temp = arg0->unk31C;
        if ((temp->unk78 == 0) && (obj->unk31C->unk78 != 0))
        {
          temp->chased = 20;
        }
      }
      distSq = ((dx * dx) + (dy * dy)) + (dz * dz);
      if (distSq < (sumRadius * sumRadius))
      {
        func_1505D408(arg0, obj, dx, dy, dz, distSq, sumRadius, i);
      }
      if (obj->id == 0x34)
      {
        if (obj->unk251 == 2)
        {
          func_1505D5D0(arg0, obj, x, y, z, height, radius, i, 120.0f, 250.0f, 120.0f, -130.0f, 0.0f);
        }
        else
        {
          func_1505D5D0(arg0, obj, x, y, z, height, radius, i, 130.0f, 140.0f, 250.0f, -50.0f, 0.0f);
        }
      }
    }

  }
}
f32 func_1505DF10(struct127 *arg0, u8 arg1, u16 *arg2, f32 *arg3, f32 *arg4, s32 arg5);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, void *, void *, void *, f32 *, s32, s32, f32);

u8 func_1505DADC(struct127 *arg0, u16 *arg1, u8 arg2, u8 arg3, u8 arg4)
{
  u8 bestStunned;
  u8 i;
  u16 angle;
  struct127 *obj;
  f32 best;
  f32 dist;
  f32 distXZ;
  f32 dy;
  s32 sp00[2];
  s32 spBC[5];
  f32 spB8;
  f32 spB4;
  f32 spB0;
  f32 hitDist;
  u8 bestIdx;
  best = 10000000.0f;
  bestStunned = 0;
  bestIdx = 0xFF;
  if (arg3 == 0xFE)
  {
    best = 40000.0f;
  }
  for (i = 0; i < 25; i++)
  {
    obj = &D_800CC2D0[i];
    if ((((((obj->interaction_state != 0) && (obj->health != 0)) && (i != ((u8) (arg0 - D_800CC2D0)))) && (obj->unkF8 & 0x40)) && (i != arg3)) && (obj->unk2FA != 0))
    {
      dist = func_1505DF10(arg0, i, &angle, &distXZ, &dy, 0);
      if ((((dist < best) || ((obj->stunned == 0) && (bestStunned != 0))) && (((u8) ((((s32) (angle - arg0->unk7A)) >> 8) + arg4)) < (arg4 * 2))) && (((u8) ((((u8) (func_1505A630(sqrtf(distXZ), -dy, 0) >> 8)) - arg2) + 0x10)) < 0x20))
      {
        if ((func_150AC9C0(arg0->x_position, arg0->y_position + 80.0f, arg0->z_position, D_800CC2D0[i].x_position - arg0->x_position, D_800CC2D0[i].y_position - arg0->y_position, D_800CC2D0[i].z_position - arg0->z_position, sp00, spBC, &spB8, &spB4, &spB0, &hitDist, 0, 0, 0.0f) == 0) || (!((hitDist = hitDist * hitDist) < dist)))
        {
          bestStunned = obj->stunned;
          bestIdx = i;
          best = dist;
          *arg1 = angle;
        }
      }
 goto dummy_label_512857; dummy_label_512857: ;
    }
  }

  return bestIdx;
}
f32 func_1505DF10(struct127 *arg0, u8 arg1, u16 *arg2, f32 *arg3, f32 *arg4, s32 arg5);

s8 func_1505DDA8(struct127 *arg0, s32 arg1, u16 arg2, f32 arg3) {
    u8 i;
    u16 angle;
    f32 distXZ;
    f32 dy;
    f32 range;

    for (i = 0; i <= D_80082FA0; i++) {
        if (D_800CC2D0[i].interaction_state == 0) {
            continue;
        }
        range = arg0->xz_velocity * arg3 * 0.5f;
        if (func_1505DF10(arg0, i, &angle, &distXZ, &dy, 0) < range * range) {
            if ((u16)(arg0->unk76 - angle + arg2 / 2) < arg2) {
                return i;
            }
        }
    }
    return -1;
}
/* Both callers pass a 6th argument (always 0) that the body never reads. */
f32 func_1505DF10(struct127 *arg0, u8 arg1, u16 *arg2, f32 *arg3, f32 *arg4, s32 arg5) {
    struct127 *ptr;
    f32 temp_f12;
    f32 temp_f2;
    f32 temp_f14;

    ptr = &D_800CC2D0[arg1];
    temp_f12 = ptr->x_position - arg0->x_position;
    temp_f2 = arg0->y_position - ptr->y_position;
    temp_f14 = arg0->z_position - ptr->z_position;
    *arg4 = temp_f2;
    *arg2 = func_1505A630(temp_f12, temp_f14, 0);
    temp_f12 = temp_f12 * temp_f12;
    temp_f2 = temp_f2 * temp_f2;
    temp_f14 = temp_f14 * temp_f14;
    *arg3 = temp_f12 + temp_f14;
    return (temp_f12 + temp_f2) + temp_f14;
}
extern u16 D_800C4ED0[];

void func_1505DFDC(AnimPlaybackOwner *arg0) {
    s32 sp1C;
    AnimPlaybackState *temp;
    u16 *p;

    arg0->field_0x84 = 0xFFFF;
    temp = arg0->field_0x2D0;
    if (temp != NULL) {
        sp1C = arg0->field_0x4;
        temp->field_0x28 = 0;
        bzero(temp->field_0x40, 0x3A0);
        p = &D_800C4ED0[sp1C];
        temp->field_0x41 = *p + 1;
        temp->field_0x211 = *p + 1;
        temp->field_0x30 = 0;
        temp->field_0x34 = 0;
    }
}

void func_1505E060(AnimPlaybackState *arg0) {
    arg0->field_0x6 = arg0->field_0x4;
    arg0->field_0xC = arg0->field_0x8;
    arg0->field_0x14 = arg0->field_0x10;
    arg0->field_0x24 = arg0->field_0x20;
    arg0->field_0x1C = arg0->field_0x18;
    arg0->field_0x39 = arg0->field_0x38;
    arg0->field_0x2C = arg0->field_0x28;
    bcopy(arg0->field_0x40, arg0->field_0x210, 0x1D0);
}

s32 func_1502AF04(u8 *, s32, s32, s32);
s32 func_1502B110(u8 *, s32, s32, u32, ...);
extern s32 D_8008630C[];
extern u16 D_800993F8[];
extern f32 D_8009953C;

void func_1505E0C4(struct127 *arg0, u16 *arg1, u8 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, s32 arg11) {
    s32 index;
    s32 flag;
    s32 sp8C;
    s32 t1;
    s32 sp84;
    s32 sp80;
    s32 *v0;
    s16 cur;
    s32 mask;
    u8 sp48[0x2C];

    flag = 0;
    if (arg4 != 0) {
        flag = 1;
        arg8 = 0.0f;
    } else if (arg5 != (*(u16 *)(arg2 + 4) & 0xFFFF7FFF)) {
        flag = 1;
    } else if (arg0 != NULL && (arg5 != *(u16 *)((u8 *)arg0 + 0x84) || *(u16 *)((u8 *)arg0 + 0x84) == 0xFFFF || *((u8 *)arg0 + 0x103) != 0)) {
        flag = 1;
    }
    if (flag != 0) {
        if (arg0 != NULL) {
            index = ((s32)arg0 - (s32)&D_800CC2D0) / 812;
            t1 = 2;
            arg6 = *((u8 *)arg0 + 6);
        } else {
            t1 = 10;
        }
        if (arg1 != NULL) {
            sp8C = arg1[0];
        } else {
            sp8C = arg5 * 2;
        }
        if (arg0 != NULL) {
            *(u16 *)((u8 *)arg0 + 0x84) = arg5;
            *(f32 *)((u8 *)arg0 + 0x4C) = arg7;
            *(s32 *)((u8 *)arg0 + 0x1C4) = *(s32 *)(arg1 + 2);
            *(s8 *)((u8 *)arg0 + 0x138) = 0;
            *((u8 *)arg0 + 0x103) = 0;
            *(f32 *)((u8 *)arg0 + 0xB4) = 0.0f;
            if (index < D_8008FD8C) {
                u8 *p = (u8 *)&D_800CC2D0 + index * 0x32C;
                *(s8 *)(*(u8 **)(p + 0x31C) + 0x58) = 0;
                *(s16 *)(*(u8 **)(p + 0x31C) + 0x5A) = 0;
                *(s8 *)(*(u8 **)(p + 0x31C) + 0x59) = 0;
            }
        }
        if (*(s32 *)(arg2 + 0x28) != 0) {
            cur = *(s16 *)(arg2 + 0x3C);
            if (arg8 >= 1.0f) {
                *(s16 *)(arg2 + 0x3A) = 500 / (s16)arg8;
                if (*(s16 *)(arg2 + 0x3C) > 0 && arg5 == (*(u16 *)(arg2 + 6) & 0xFFFF7FFF)) {
                    *(s16 *)(arg2 + 0x3C) = 0x3FF - *(s16 *)(arg2 + 0x3C);
                    cur = 0;
                } else if (cur < 600) {
                    *(s16 *)(arg2 + 0x3C) = 0x3FF;
                }
            } else {
                *(s16 *)(arg2 + 0x3C) = 0;
            }
            if (cur < 600) {
                *(u16 *)(arg2 + 6) = *(u16 *)(arg2 + 4);
                *(f32 *)(arg2 + 0xC) = *(f32 *)(arg2 + 8);
                *(f32 *)(arg2 + 0x14) = *(f32 *)(arg2 + 0x10);
                *(f32 *)(arg2 + 0x24) = *(f32 *)(arg2 + 0x20);
                *(f32 *)(arg2 + 0x1C) = *(f32 *)(arg2 + 0x18);
                *(s8 *)(arg2 + 0x39) = *(s8 *)(arg2 + 0x38);
                *(s32 *)(arg2 + 0x2C) = *(s32 *)(arg2 + 0x28);
                bcopy(arg2 + 0x40, arg2 + 0x210, 0x1D0);
            }
        }
        *(u16 *)(arg2 + 4) = arg5;
        if (arg11 != 0) {
            *(u16 *)(arg2 + 4) |= 0x8000;
        }
        *(f32 *)(arg2 + 8) = arg9;
        *(f32 *)(arg2 + 0x10) = arg7;
        *(f32 *)(arg2 + 0x20) = arg10;
        if (arg1 != NULL) {
            *(s32 *)arg2 = D_8008630C[arg1[1]];
        } else {
            *(s32 *)arg2 = 0;
        }
        if (arg0 != NULL && *(u8 **)((u8 *)arg0 + 0x58) != NULL) {
            v0 = (s32 *)func_1502AF04(*(u8 **)((u8 *)arg0 + 0x58), (s32)sp48, sp8C, 2);
        } else {
            v0 = (s32 *)func_1502B110(NULL, 2, (s32)sp48, 3, t1, arg6, sp8C);
        }
        if (v0 != NULL) {
            sp84 = v0[1];
            sp80 = v0[0];
            *(s32 *)(arg2 + 0x28) = v0[2];
        }
        {
            s32 size = (sp84 + 0xF) & ~0xF;
            if (sp84 == 0) {
                func_1505DFDC((AnimPlaybackOwner *)arg0);
                return;
            }
            if (size < 200) {
                func_1000480C(sp80, arg2 + 0x40, size);
            } else {
                func_10004514(sp80, arg2 + 0x40, size, 1);
            }
        }
        *(f32 *)(arg2 + 0x18) = (u32)arg2[0x43];
        if (arg2[0x43] == 0xFF && arg0 != NULL) {
            switch (*((u8 *)arg0 + 4)) {
            case 0x92:
                *(f32 *)(arg2 + 0x18) = (u32)D_800993F8[*(u16 *)((u8 *)arg0 + 0x84)];
                break;
            case 0xB7:
            case 0xB8:
            case 0xB9:
                *(f32 *)(arg2 + 0x18) = D_8009953C;
                break;
            }
        }
        mask = 0x303;
        if (arg0 != NULL) {
            *((u8 *)arg0 + 0x24E) = 0;
            *((u8 *)arg0 + 0x1FC) |= 2;
            *((u8 *)arg0 + 0x1FC) &= ~1;
            if (*((u8 *)arg0 + 0x104) == 0) {
                mask = 0x30B;
            }
            *(s32 *)((u8 *)arg0 + 0xF4) &= ~mask;
            if (*(s32 *)((u8 *)arg0 + 0xF4) & 0x20) {
                *(s32 *)((u8 *)arg0 + 0xF4) |= 2;
                *(s32 *)((u8 *)arg0 + 0xF4) &= ~0x20;
            }
            if (arg2[0x47] & 0x20) {
                *((u8 *)arg0 + 0x1FD) = arg2[0x4B];
            } else {
                *((u8 *)arg0 + 0x1FD) = 0;
            }
        }
        if (arg2[0x45] == 0) {
            *(s32 *)(arg2 + 0x28) = 1;
        }
    } else {
        if (arg0 != NULL) {
            *(f32 *)((u8 *)arg0 + 0x4C) = arg7;
        }
        *(f32 *)(arg2 + 0x10) = arg7;
        if (arg11 != 0) {
            *(u16 *)(arg2 + 4) |= 0x8000;
        } else {
            *(u16 *)(arg2 + 4) &= 0xFFFF7FFF;
        }
        if (D_800C35EA == 1 && (*(u16 *)(arg2 + 4) & 0x8000)) {
            *(f32 *)(arg2 + 8) = arg9;
        }
    }
    if (arg0 != NULL && flag != 0) {
        func_150627D4(arg0);
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_1505E650.s")
u32 func_1505E7CC(s32 arg0, struct127 *arg1) {
    s32 idx;
    s32 *ptr;
    u32 count;
    u8 *base;
    u8 *entry;
    u32 i;

    idx = arg1->id;
    if (idx == 0xFF) {
        return 0;
    }
    ptr = (s32 *)D_800D1588[idx];
    if (ptr == NULL) {
        return 0;
    }
    count = ptr[-1];
    if (count == 0) {
        return 0;
    }
    count = count / 0x18;
    base = (u8 *)ptr[-2];
    if (base == NULL) {
        return 0;
    }
    if (count != 0) {
        i = 0;
        entry = base;
        do {
            if (arg0 == entry[0]) {
                return i;
            }
            i++;
            entry += 0x18;
        } while (i < count);
    }
    return 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_1505E874.s")
struct127 *func_1505ED34(void) {
    s32 i;
    struct127 *ptr;

    for (i = 0, ptr = D_800CC2D0; (i < 25) && (ptr->interaction_state != 0); i++, ptr++) {
    }

    if (i == 25) {
        ptr = D_800CC2D0;
        i = 0;
        if ((0x27 != ptr->interaction_state) || (D_800CC4A4 != NULL)) {
            do {
                i++;
                ptr++;
            } while ((i < 25) && ((0x27 != ptr->interaction_state) || (ptr->unk1D4 != NULL)));
        }

        if (i != 25) {
            if (ptr->interaction_state != 0) {
                func_15060F28(ptr, 0);
            }
        }
    }

    if (i == 25) {
        for (ptr = D_800CC2D0, i = 0; (i < 25) && (0x27 != ptr->interaction_state); i++, ptr++) {
        }

        if (i != 25) {
            if (ptr->interaction_state != 0) {
                func_15060F28(ptr, 0);
            }
        }
    }

    if (i == 25) {
        if (ptr->interaction_state != 0) {
            func_15060F28(ptr, 0);
        }
    }

    func_1505F188(ptr);
    return ptr;
}

struct127 *func_1505EEB0(s32 state, s32 *arg1) {
    struct127 *ptr;
    s32 i;

    for (ptr = D_800CC2D0, i = 0; (i < 25) && (state != ptr->interaction_state); i++, ptr++) {
    }

    *arg1 = i;
    return ptr;
}
// I HATE LOOPS.
// struct127 *func_1505EEB0(s32 state, s32 *arg1) {
//     struct127 *tmp = D_800CC2D0;
//     s32 i = 0;
//
//     if (state != tmp->interaction_state) {
//         for (i = 0; i < 25; i++) {
//             tmp = &D_800CC2D0[i];
//             if (state == tmp->interaction_state)
//                 break;
//         }
//     }
//
//     *arg1 = i;
//     return tmp;
// }

struct127 *func_1505EEF4(s32 arg0) {
    struct127 *tmp = (struct127 *)&D_800CC2D0;
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((tmp->interaction_state != 0) && (arg0 == tmp->unk13F)) {
            return tmp;
        }
        tmp++;
    }
    return NULL;
}
extern u8 D_800CC3F7;
extern struct127 D_800CC5FC;

struct127 *func_1505EFD0(s32 arg0) {
    struct127 *ptr;
    s32 i;

    ptr = &D_800CC5FC;
    if (D_800CC2D0[0].interaction_state != 0) {
        if (arg0 == D_800CC3F7) {
            return (struct127 *)(D_800CC2D4 - 4);
        }
    }

    for (i = 1; i < 25; i++, ptr++) {
        if ((ptr->interaction_state != 0) && (arg0 == ptr->unk127)) {
            return ptr;
        }
    }
    return NULL;
}
struct127 *func_1505F0AC(s32 arg0) {
    struct127 *tmp = (struct127 *)&D_800CC2D0;
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((tmp->interaction_state != 0) && (arg0 == tmp->id)) {
            return tmp;
        }
        tmp++;
    }
    return NULL;
}
void func_1505F188(struct127 *argp) {
#define arg0 ((u32)argp)
    s32 *p;
    f32 one;
    f32 initial;

    for (p = (s32 *)arg0; p < (s32 *)(arg0 + 0x32C); p++) {
        *p = 0;
    }
    one = 1.0f;
    initial = -10000.0f;
    *(s8 *)((u8 *)arg0 + 0x2FD) = 2;
    *(s16 *)((u8 *)arg0 + 0x38) = -0x2710;
    *(f32 *)((u8 *)arg0 + 0x14C) = one;
    *(f32 *)((u8 *)arg0 + 0x150) = one;
    *(f32 *)((u8 *)arg0 + 0x118) = initial;
    *(f32 *)((u8 *)arg0 + 0x180) = initial;
    *(f32 *)((u8 *)arg0 + 0x24) = 3.3f;
    *(u8 *)((u8 *)arg0 + 0x1DC) = 0xFF;
    *(u8 *)((u8 *)arg0 + 0x127) = 0xFF;
    *(u16 *)((u8 *)arg0 + 0x84) = 0xFFFF;
    *(u8 *)((u8 *)arg0 + 0x13F) = 0xFF;
    *(s32 *)((u8 *)arg0 + 0x2C4) = (s32) (arg0 + 4);
    *(s8 *)((u8 *)arg0 + 0x2C8) = 1;
    *(s8 *)((u8 *)arg0 + 0x2C9) = 1;
    *(u8 *)((u8 *)arg0 + 4) = 0xFF;
    *(s8 *)((u8 *)arg0 + 0x2CB) = 0x32;
    *(f32 *)((u8 *)arg0 + 0x48) = one;
    *(s8 *)((u8 *)arg0 + 0x6E) = (s8) ((func_150ADA20() % 50U) + 0x32);
    func_150615DC((void *) arg0);
    *(u8 *)((u8 *)arg0 + 0x1DD) = 0xFF;
    *(u8 *)((u8 *)arg0 + 0x1DE) = 0xFF;
    *(u8 *)((u8 *)arg0 + 0x1DF) = 0xFF;
    *(s16 *)((u8 *)arg0 + 0x18C) = 0;
    *(s16 *)((u8 *)arg0 + 0x18E) = 0;
    *(s16 *)((u8 *)arg0 + 0x190) = 0;
    *(s16 *)((u8 *)arg0 + 0x192) = 0;
    *(s16 *)((u8 *)arg0 + 0x194) = 0;
    *(s16 *)((u8 *)arg0 + 0x196) = 0xA;
    *(s16 *)((u8 *)arg0 + 0x198) = 0xA;
    *(s16 *)((u8 *)arg0 + 0x19A) = 0;
    *(s16 *)((u8 *)arg0 + 0x19C) = 0;
#undef arg0
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_1505F298.s")
void func_1506045C(struct127 *arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4) {
    s32 stunned;
    s32 state;
    s32 expired;
    s32 held;
    s32 result;
    s32 old_unk75;
    s32 idx;

    result = 0;
    if (D_800C35EA == 1) {
        return;
    }
    held = 0;
    stunned = 0;
    expired = 0;

    if ((*(u16 *)((u8 *)arg0->unk31C + 0x72) != 0) && (arg4 == 0)) {
        if (D_800BE9E4 < *(u16 *)((u8 *)arg0->unk31C + 0x72)) {
            *(u16 *)((u8 *)arg0->unk31C + 0x72) -= D_800BE9E4;
        } else {
            expired = 1;
            *(u16 *)((u8 *)arg0->unk31C + 0x72) = 0;
        }
    }

    state = arg0->unk31C->unk75 & ~0x80;
    if ((((u8 *)arg0->unk31C)[0x7C] != 0) && (state == 0)) {
        state = ((u8 *)arg0->unk31C)[0x7C];
    }

    if ((D_800BE9F0 != 0x29) && (D_800BE9F0 != 0x2E) && (arg0->unk31C->unk78 == 0) &&
        (*(u16 *)D_800CC284 & 0x20) && (func_15080738(state) != 0)) {
        state = 0x1A;
    }

    if (arg0->unk31C->unk78 == 0x1A) {
        state = 0x1A;
    }
    if (arg0->unk31C->unk78 == 0x2A) {
        state = 0x2A;
    }

    if ((arg0->unk13C != 0) || (((u8 *)arg0)[0x137] != 0)) {
        if (arg4 == 0) {
            state = 0;
        } else {
            held = 1;
        }
    }

    idx = arg0->unk65;
    if ((idx != 0) && (arg4 == 0)) {
        switch (D_800CC2D0[idx - 1].id) {
        case 0x23:
        case 0x8A:
            state = 0x1B;
            break;
        case 0xC:
            state = 8;
            break;
        case 0x28:
            state = 0;
            break;
        default:
            state = 0xE;
            break;
        }
    }

    if (((u8 *)arg0->unk31C)[0x17] != 0) {
        state = 0x3C;
    }

    if ((arg4 == 0) && arg0->unk31C->unk78 && (state != (arg0->unk31C->unk78 & ~0x80))) {
        old_unk75 = arg0->unk31C->unk75;
        arg0->unk31C->unk75 = arg0->unk31C->unk78;
        func_1506045C(arg0, arg1, arg2, arg3, 1);
        arg0->unk31C->unk75 = old_unk75;
        arg0->unk31C->unk78 = 0;
    }

    if (arg0->stunned != 0) {
        stunned = 1;
        arg2 = 0;
        arg1 = 0;
    }

    if ((state != 0) && (arg0->unk31C->unk4E == 0)) {
        result = func_15065A5C(arg0, arg1, arg2, arg3, arg4, state, stunned, expired, held);
    }

    if ((((u8 *)arg0->unk31C)[0x7B] != 0) || (arg2 & 0x4000)) {
        func_150642AC(arg0, arg2, arg3, held, result);
    }
}
void func_15060778(s32 arg0, struct127 *arg1, u16 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 channel;
    u32 handle;
    u16 *slot;

    handle = 0;
    slot = 0;
    if (arg6 & 8) {
        channel = 0;
    } else {
        channel = (*(u32 *)&((u8 *)arg1)[0x184] >> 3) & 0x30;
    }
    switch (arg6 & 3) {
    case 1:
        slot = (u16 *)&((u8 *)arg1)[0x8C];
        handle = *(u16 *)&((u8 *)arg1)[0x8C];
        break;
    case 2:
    case 3:
        slot = (u16 *)&((u8 *)arg1)[0x8E];
        handle = *(u16 *)&((u8 *)arg1)[0x8E];
        if ((arg6 & 3) == 2) {
            break;
        }
        if ((func_1001147C(handle) == arg0) & 0x7FFF) {
            if (arg6 & 4) {
                func_1000F91C(handle, (u16)(arg2 + channel * 50),
                              (s16)(((u8 *)arg1)[0x13F] * 10 + arg3), channel & 0xFF, 0,
                              (s32)*(f32 *)&((u8 *)arg1)[0x14],
                              (s32)*(f32 *)&((u8 *)arg1)[0x18],
                              (s32)*(f32 *)&((u8 *)arg1)[0x1C], arg4, arg5);
            } else {
                func_1000F85C(handle, 0x10, arg3);
                func_1000F85C(handle, 8, arg2 + channel * 50);
            }
            return;
        }
        arg0 |= 0x8000;
        break;
    }
    if (arg6 & 4) {
        handle = func_10010E78(handle, arg0, (u16)(arg2 + channel * 50),
                              (s16)arg3, channel, 0,
                              (s32)*(f32 *)&((u8 *)arg1)[0x14],
                              (s32)*(f32 *)&((u8 *)arg1)[0x18],
                              (s32)*(f32 *)&((u8 *)arg1)[0x1C], arg4, arg5);
    } else {
        handle = func_10010BE8(handle, arg0, (u16)(arg2 + channel * 50),
                              0x40, arg3, channel, 1);
    }
    if (slot != 0) {
        *slot = handle;
    }
}
void func_15060A30(s32 arg0, struct127 *arg1) {
    if (arg1->camera == NULL) {
        func_10010344(arg0, arg1, 0x6D60, 0x1F4, 0x9C4);
    } else {
        func_15060778(arg0, arg1, 0x5DC0, 0, 0x1F4, 0x9C4, 1);
    }
}
void func_15060A9C(s32 arg0, struct127 *arg1) {
    if (arg1->camera == NULL) {
        func_10010630(arg0, arg1, 0x5DC0, 0x1F4, 0x9C4);
    } else {
        func_15060778(arg0, arg1, 0x5DC0, 0, 0x1F4, 0x9C4, 0);
    }
}

void func_15060B04(s32 arg0, struct127 *arg1, s32 arg2) {
    if (arg1->camera == NULL) {
        func_10010630(arg0, arg1, arg2, 500, 2500);
    } else {
        func_15060778(arg0, arg1, arg2, 0, 500, 2500, 0);
    }
}

void func_15060B70(s32 arg0, void *arg1) {
    func_10010154(arg0, arg1, 0x6D60, 0x1F4, 0x9C4);
}

s32 func_15060BA4(struct127 *arg0, s32 arg1) {
    if (arg0->health == 6) {
        return 0;
    }
    arg0->health += arg1;
    if (arg0->health >= 7) {
        arg0->health = 6;
    }
    return 1;
}
extern u8 D_800C3E90;

void func_15060BE0(struct127 *arg0) {
    f32 pad0;
    f32 pad1;
    f32 pad2;
    f32 pad3;
    f32 sp4;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f2;
    f32 var_f0;
    f32 var_f18;
    f32 var_f2;
    s32 var_v0;
    struct124 *temp_v0;
    u8 *temp_a2;

    var_v0 = 0;
    if (arg0->id != 0xFF) {
        if ((arg0->unk1D4 != 0) && (D_800C3E90 == 0)) {
            temp_v0 = (struct124 *)D_800D1C90[arg0->id];
            temp_a2 = (u8 *)arg0->unk1D4;
            temp_a2 += *(u8 *)((u8 *)temp_v0 + 0x27) << 6;
            temp_f2 = (f32) *(s16 *)((u8 *)temp_v0 + 0x14) * arg0->y_scale;
            if (temp_f2) {
                temp_f12 = *(f32 *)(temp_a2 + 0x10);
                temp_f14 = *(f32 *)(temp_a2 + 0x14);
                temp_f16 = *(f32 *)(temp_a2 + 0x18);
                temp_f0 = sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16));
                var_f18 = temp_f0;
                if (temp_f0) {
                    var_f18 = temp_f2 / temp_f0;
                }
                var_f2 = temp_f12 * var_f18;
                var_f0 = temp_f14 * var_f18;
                sp4 = temp_f16 * var_f18;
            } else {
                var_f0 = (sp4 = 0.0f);
                var_f2 = var_f0;
            }
            var_v0 = 1;
            *(s16 *)((u8 *)arg0 + 0x1BC) = (s16) (s32) (*(f32 *)(temp_a2 + 0x30) + var_f2);
            *(s16 *)((u8 *)arg0 + 0x1BE) = (s16) (s32) (*(f32 *)(temp_a2 + 0x34) + var_f0);
            *(s16 *)((u8 *)arg0 + 0x1C0) = (s16) (s32) (*(f32 *)(temp_a2 + 0x38) + sp4);
        }
    }
    if (var_v0 == 0) {
        *(s16 *)((u8 *)arg0 + 0x1BC) = (s16) (s32) arg0->x_position;
        *(s16 *)((u8 *)arg0 + 0x1BE) = (s16) (s32) arg0->y_position;
        *(s16 *)((u8 *)arg0 + 0x1C0) = (s16) (s32) arg0->z_position;
    }
}
typedef struct Game83300ActorLink {
    s32 active;
    u8 pad4[0x61];
    u8 owner_index;
    u8 pad66[0x2C6];
} Game83300ActorLink;

void func_15060D54(Game83300ActorLink *arg0) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((((Game83300ActorLink *)&D_800CC2D0)[i].active != 0) &&
            ((arg0 - (Game83300ActorLink *)&D_800CC2D0) + 1 == ((Game83300ActorLink *)&D_800CC2D0)[i].owner_index)) {
            ((Game83300ActorLink *)&D_800CC2D0)[i].owner_index = 0;
        }
    }
}
// PERMUTER CANDIDATE best 1815 (object-level; the body below is structurally correct and only
// 2 instructions short of the target). Needs these locally-declared callees/globals:
//   extern s32 func_1514D310(struct127 *); extern void func_151695F0(struct127 *, u8);
//   extern void func_15084558(struct127 *); extern void func_1504AF10(struct127 *, s32, s32);
//   extern void func_1503E260(s32); extern void func_150626EC(struct127 *, s32);
//   extern void func_15060D54(struct127 *); extern s32 func_150303E4(struct127 *);
//   extern void func_15188AD0(s32); extern void func_10004074(void *); extern u8 *D_800D210C;
// Keys already found: the free-list loop only matches with a truncating increment
// (`s32 i; for (i = 0; i <= D_80082FA0; i = (u8)(i + 1))`) -- a plain `u8 i; i++` makes IDO
// CSE the zero-extension into a second loop-carried value, which steals s0, homes `i` at
// sp+0x27 and grows the frame 0x20->0x28 (2180). Binding the unk144 flags word to its own
// local puts it in a1 as the target does (1830->1815).
// Residual (unsteerable): (a) target keeps arg0->unk144 in v0 and emits a redundant
// `move v1,v0` live-range split before the else-else `sb ...,2(v1)`; IDO gives us v1 directly
// and never emits the copy. (b) target fills the else-else `b` delay slot by DUPLICATING the
// merge-point `lw a0,0x260(s1)` and branching past it, where IDO sinks our `sb` into the slot.
// (c) a coupled v0-vs-a0 JUSTREG cascade over the 0x264/0x268/0x26C/0x314/0x31C frees (target
// loads them to v0 + `move a0,v0`; IDO coalesces ours straight into a0). Named locals,
// declaration order, register/inner-scope hints and while/do-while forms all leave (a)-(c) fixed.
// void func_15060F28(struct127 *arg0, s32 arg1) {
//     u16 flags;
//     func_1514D310(arg0);
//     func_151695F0(arg0, 0);
//     if ((arg0->unk5 != 2) && (arg0->unk5 != 3)) { func_15084558(arg0); func_150626EC(arg0, arg1); }
//     if (arg1 == 1) { func_1504AF10(arg0, 1, 0); func_1503E260(arg0 - D_800CC2D0); }
//     if (arg0->unk13F != 0xFF) {
//         if (arg1 != 2) {
//             if (arg0->unk144 != NULL) {
//                 flags = arg0->unk144->unk0;
//                 if (flags & 0x20) { arg0->unk144->unk2 = 1; D_800D210C[arg0->unk13F] = 0; }
//                 else if (flags & 1) { D_800D210C[arg0->unk13F] = 0; }
//                 else { arg0->unk144->unk2 = 1; D_800D210C[arg0->unk13F] &= 0x80; }
//             } else { D_800D210C[arg0->unk13F] &= 0x80; }
//         } else { D_800D210C[arg0->unk13F] = 0; }
//     }
//     if (*(void **)((u8 *)arg0 + 0x260) != NULL) func_10004074(*(void **)((u8 *)arg0 + 0x260));
//     if (*(void **)((u8 *)arg0 + 0x264) != NULL) func_10004074(*(void **)((u8 *)arg0 + 0x264));
//     if (*(void **)((u8 *)arg0 + 0x268) != NULL) func_10004074(*(void **)((u8 *)arg0 + 0x268));
//     if (*(void **)((u8 *)arg0 + 0x26C) != NULL) func_10004074(*(void **)((u8 *)arg0 + 0x26C));
//     if (*(void **)((u8 *)arg0 + 0x1D8) != NULL) func_10004074(*(void **)((u8 *)arg0 + 0x1D8));
//     if (*(struct102 **)((u8 *)arg0 + 0x2D4) != NULL) func_1516972C(*(struct102 **)((u8 *)arg0 + 0x2D4));
//     if (*(s32 **)((u8 *)arg0 + 0x304) != NULL) {
//         s32 i;
//         for (i = 0; i <= D_80082FA0; i = (u8)(i + 1))
//             func_100043B4(*(s32 **)((u8 *)arg0 + (i * 4) + 0x304), 4);
//     }
//     if (*(s32 **)((u8 *)arg0 + 0x314) != NULL) func_100043B4(*(s32 **)((u8 *)arg0 + 0x314), 4);
//     if (arg0->unk2D0 != NULL) func_10004074(arg0->unk2D0);
//     if (*(void **)((u8 *)arg0 + 0x324) != NULL) func_10004074(*(void **)((u8 *)arg0 + 0x324));
//     if ((arg0->unk31C != NULL) && (*(void **)((u8 *)arg0->unk31C + 0x11C) != NULL))
//         func_10004074(*(void **)((u8 *)arg0->unk31C + 0x11C));
//     func_10010AA8(arg0); func_150303E4(arg0); func_15060D54(arg0); func_15188AD0((s32)arg0);
//     if (arg0->unk31C != NULL) func_10004074(arg0->unk31C);
//     func_1505F188(arg0);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_15060F28.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_150611E8.s")

// TODO: is this really struct127?
void func_150615DC(struct127 *arg0) {
    arg0->unk7 = 0xFF;
    arg0->unk8 = 0xFF;
    arg0->unk9 = 0;
    arg0->unkA = 0;
    arg0->unkE.ub[1] = 0;
    arg0->unkB = arg0->unkC = arg0->unkD = arg0->unkE.ub[0] = 0xFF;
}

void func_1506160C(struct127 *arg0, u8 arg1, u8 arg2, u8 arg3, s32 arg4) {
    s32 bit;
    s32 hi;

    if (arg1 >= 8) {
        if (arg1 != 10) {
            if (arg1 == 8) {
                bit = 1 << (u8)arg4;
                arg0->unkE.ub[1] |= bit;
                arg2 = 0;
            } else {
                bit = 1 << (u8)arg4;
                arg0->unkE.ub[1] &= ~bit;
                arg2 = 0xFF;
            }
            hi = bit << 4;
            if (arg0->unkE.ub[1] & hi) {
                ((u8 *)arg0 + (u8)arg4)[0xB] = arg2;
            }
            arg0->unkE.ub[1] &= ~hi;
        } else {
            arg0->unkE.ub[1] |= (1 << (u8)arg4) << 4;
            arg2 = 0;
        }
        if ((arg0->pad2FC[1] != 0) || (D_800DBFF4[(u8)arg4] != 0)) {
            ((u8 *)arg0 + (u8)arg4)[0xB] = arg2;
        }
    } else if (arg1 >= arg0->unkA) {
        if (arg1 == 4) {
            arg0->unkA = arg1;
            arg0->unk7 = 0;
            arg0->unk8 = 0xFF;
            arg0->unk9 = 0x20;
        } else if (arg1 == 5) {
            arg0->unkA = arg1;
            arg0->unk8 = 0;
            arg0->unk9 = 0x20;
        } else if (arg1 == 6) {
            arg0->unkA = arg1;
            arg0->unk8 = 0;
            arg0->unk9 = 8;
        } else if (arg1 == 2) {
            arg0->unkA = arg1;
            arg0->unk8 = arg2;
            arg0->unk9 = arg3;
        } else if (arg1 == 1) {
            arg0->unkA = 0;
            arg0->unk7 = arg2;
            arg0->unk8 = arg2;
        }
        if (*(s32 *)D_800DBFF4 != 0) {
            arg0->unk7 = arg0->unk8;
        }
    }
}
void func_150617BC(u8 *arg0) {
    s32 i;
    s32 mask;
    s32 value;

    if (*(s32 *)arg0 != 0) {
        mask = 1;
        for (i = 0; i <= D_80082FA0; i++) {
            if (arg0[0xF] & mask) {
                if (arg0[i + 0xB] > 0) {
                    if (((u8 *)&D_800DBFF4)[i] == 0) {
                        value = arg0[i + 0xB] - 50;
                        if (value < 0) {
                            value = 0;
                        }
                    } else {
                        value = 0;
                    }
                    arg0[i + 0xB] = value;
                }
            } else {
                if (arg0[i + 0xB] < 0xFF) {
                    if (((u8 *)&D_800DBFF4)[i] == 0) {
                        value = arg0[i + 0xB] + 50;
                        if (value > 0xFF) {
                            value = 0xFF;
                        }
                    } else {
                        value = 0xFF;
                    }
                    arg0[i + 0xB] = value;
                }
            }
            mask <<= 1;
        }
        if (arg0[0xA] != 0) {
            if (arg0[8] != arg0[7]) {
                if (arg0[7] < arg0[8]) {
                    value = arg0[9] + arg0[7];
                    if (arg0[8] < value) {
                        value = arg0[8];
                    }
                } else {
                    value = arg0[7] - arg0[9];
                    if (value < arg0[8]) {
                        value = arg0[8];
                    }
                }
                arg0[7] = value;
            }
            if (arg0[8] == arg0[7]) {
                if (arg0[0xA] == 6) {
                    func_15060F28((struct127 *)arg0, 1);
                } else if (arg0[0xA] == 5) {
                    func_15060F28((struct127 *)arg0, 2);
                } else {
                    arg0[0xA] = 0;
                }
            }
        }
    }
}
// ???
s32 func_1506196C(u8 *arg0, s32 arg1) {
    s32 temp_v1 = arg0[7] * (arg0 + arg1)[0xB];
    if (temp_v1 == 0xFE01) {
        temp_v1 = 0xFF;
    } else {
        temp_v1 = temp_v1 >> 8;
    }
    return temp_v1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_150619A8.s")

typedef struct Game83300Step4 {
    u8 b[4];
} Game83300Step4;
extern Game83300Step4 D_800860F0;

struct Game83300MorphObject;
void func_15062D10(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_15062E24(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_15062FC0(struct Game83300MorphObject *arg0, s32 arg1, u8 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void func_1501B660(struct127 *arg0, f32 *arg1, s32 arg2);
f32 func_150489B0(u8 arg0);
void func_150F1CB0(struct127 *arg0);
extern s32 *D_800C4020[];
extern s32 D_8009942C[];

#define ANIM D_800CC298
#define OBJB(o, off) (((u8 *)(o))[off])

void func_15061B4C(void) {
    struct127 *unused;
    s32 id;
    s32 i;
    u8 done[10];

    bzero(done, sizeof(done));
    for (i = 0; i < 25; i++) {
        if ((D_800CC2D0[i].interaction_state == 0) || (D_800CC2D0[i].unk2FA == 0)) {
            continue;
        }
        id = D_800CC2D0[i].id;
        if ((id == 0x13) && (done[0] == 0)) {
            func_15062D10(id, 0x5E, 0, -1, 0, 0);
            func_15062D10(id, 0x60, 0, -1, 0, 0);
            func_15062D10(id, 0x62, 0, -1, 0, 0);
            func_15062D10(id, 0x64, 0, -1, 0, 0);
            done[0] = 1;
        } else if ((id == 0x23) && (done[1] == 0)) {
            func_15062D10(id, 0x1E9, 0, -1, 0, 0);
            func_15062D10(id, 0x1EB, 0, -1, 0, 0);
            func_15062D10(id, 0x1ED, 0, -1, 0, 0);
            func_15062D10(id, 0x1EF, 0, -1, 0, 0);
            done[1] = 1;
        } else if ((id == 0x24) && (done[2] == 0)) {
            if ((func_150ADA20() % 1000U) < 300) {
                ANIM.unk8 = (func_150ADA20() % 5U) - 2;
            }
            func_15062D10(id, 4, -4, ANIM.unk8, 0, 0);
            if (D_800C4020[id] != NULL) {
                func_15062E24(id, D_800C4020[id][0], 0x37, 0, ANIM.unk4, 2, 1);
                func_15062E24(id, D_800C4020[id][1], 0x27, 0x37, ANIM.unk0, 1, 0);
            }
            ANIM.unk0 += -4.0f * D_800BE9E4;
            if (ANIM.unk0 <= 0.0f) {
                ANIM.unk0 += 360.0f;
            }
            ANIM.unk4 += -0.5f * D_800BE9E4;
            if (ANIM.unk4 <= 0.0f) {
                ANIM.unk4 += 360.0f;
            }
            done[2] = 1;
        } else if ((id == 0x2C) && (done[3] == 0)) {
            if (ANIM.unk9 >= 0x29) {
                ANIM.unkA = -1;
            } else if (ANIM.unk9 <= 0) {
                ANIM.unkA = 1;
            }
            ANIM.unk9 += ANIM.unkA;
            func_15062D10(id, 0xAA, ANIM.unk9, 0, 0, 1);
            done[3] = 1;
        } else if ((id == 0x3A) && (done[4] == 0)) {
            f32 weights[4];
            s32 j;
            Game83300Step4 step = D_800860F0;


            if (D_800CC2D0[i].unk1D4 != NULL) {
                for (j = 0; j < 4; j++) {
                    weights[j] = (func_150489B0(ANIM.unkB[j]) + 1.0f) * 0.5f;
                    ANIM.unkB[j] += step.b[j];
                }
                func_1501B660(&D_800CC2D0[i], weights, 1);
                done[4] = 1;
            }
        } else if ((id == 0x3F) && (done[5] == 0)) {
            func_15062D10(id, 4, 0, -4, 0, 0);
            done[5] = 1;
        } else if (id == 0x4B) {
            if (done[6] == 0) {
                done[6] = 1;
                func_15062D10(id, 0x173, -4, 0, 0, 0);
                func_15062D10(id, 0x175, -2, 0, 0, 0);
                func_15062D10(id, 0x177, -1, 0, 0, 0);
            }
            if (ANIM.unkF >= 4) {
                ANIM.unkF = 0;
                if (OBJB(&D_800CC2D0[i], 0x69) == 5) {
                    OBJB(&D_800CC2D0[i], 0x69) = 0;
                } else {
                    OBJB(&D_800CC2D0[i], 0x69)++;
                }
                if (OBJB(&D_800CC2D0[i], 0x69) >= 3) {
                    OBJB(&D_800CC2D0[i], 0x68) = 5 - OBJB(&D_800CC2D0[i], 0x69);
                } else {
                    OBJB(&D_800CC2D0[i], 0x68) = OBJB(&D_800CC2D0[i], 0x69);
                }
                OBJB(&D_800CC2D0[i], 0x68) += 0xF;
            } else {
                ANIM.unkF += D_800BE9E4;
            }
        } else if (id == 0x45) {
            s32 val = OBJB(&D_800CC2D0[i], 0x6D);

            if (val >= 10) {
                val -= 10;
            } else {
                val = val * 2 + 6;
            }
            OBJB(&D_800CC2D0[i], 0x68) = val;
        } else if (id == 0x5B) {
            s32 v;
            s32 k;

            for (k = 0; k < 2; k++) {
                v = OBJB(&D_800CC2D0[i], 0x6C + k);
                if (v >= 10) {
                    v -= 10;
                } else {
                    v = D_8009942C[v];
                }
                OBJB(&D_800CC2D0[i], 0x68 + k) = v;
            }
        } else if (id == 0x54) {
            f32 scale;

            if (D_800CC2D0[i].unk2EC != 0) {
                if (ANIM.unk10 < 0xEB) {
                    ANIM.unk10 += 0x14;
                } else {
                    ANIM.unk10 = 0xFF;
                }
            } else if (ANIM.unk10 >= 0x15) {
                ANIM.unk10 -= 0x14;
            } else {
                ANIM.unk10 = 0;
            }
            scale = (u32)ANIM.unk10 * 0.003921568859f;
            func_1501B660(&D_800CC2D0[i], &scale, 0);
        } else if (id == 0x8A) {
            s32 delta;
            s32 pad;

            if (D_800BE9F0 != 0x30) {
                if (D_800CC2D0[i].unk84.uh == 1) {
                    func_15062D10(id, 0x253, 0, 0x10, 0, 0);
                    func_15062D10(id, 0x255, 0, 8, 0, 0);
                    func_15062D10(id, 0x257, 0, 4, 0, 0);
                }
            } else {
                if (D_800CC2D0[i].unk84.uh == 1) {
                    delta = D_800BE9E4 * -40;
                } else {
                    delta = 0;
                }
                func_15062FC0((struct Game83300MorphObject *)&D_800CC2D0[i], 0, 0, 0x1000, 0x1000, 0xC, 0, delta);
            }
        } else if (id == 0x42) {
            if ((D_800CC2D0[i].unk84.uh == 2) || (D_800CC2D0[i].unk84.uh == 1)) {
                if (OBJB(&D_800CC2D0[i], 0x68) == 0xD) {
                    OBJB(&D_800CC2D0[i], 0x68) = 0xC;
                }
                if ((func_150ADA20() & 3) == 0) {
                    if (OBJB(&D_800CC2D0[i], 0x68) == 0x15) {
                        OBJB(&D_800CC2D0[i], 0x68) = 0xC;
                    } else {
                        OBJB(&D_800CC2D0[i], 0x68) = 0x15;
                    }
                }
            } else {
                OBJB(&D_800CC2D0[i], 0x68) = 0xD;
            }
        } else if (id == 0x7B) {
            func_150F1CB0(&D_800CC2D0[i]);
        } else if ((id == 0x92) && (done[9] == 0)) {
            func_15062D10(id, 4, 0, -2, 0, 0);
            func_15062D10(id, 0x13B, 0, -2, 0, 0);
            func_15062D10(id, 0x20D, 0, -2, 0, 0);
            done[9] = 1;
        } else if (id == 0x43) {
            func_15062D10(id, 4, 0x14, 8, 0, 0);
        }
    }
}
typedef struct { s16 x; s16 y; s16 z; u16 r; } Pt150623F4;
s32 func_150229E4(struct127 *);
s32 func_150A6360(void *, void *, f32, f32, f32, f32, f32, f32);
extern u8 D_800D9C10[];

s32 func_150623F4(struct127 *arg0) {
    f32 scale;
    s32 sum;
    s32 ok;
    s32 i;
    u8 idx;
    f32 dx;
    f32 dz;
    f32 height;
    s32 id;
    u8 *p;

    scale = (arg0->unk144 != NULL) ? (f32)*(u16 *)((u8 *)arg0->unk144 + 0x2A) : 0.0f;
    if (scale == 0.0f) {
        return 1;
    }
    if (arg0->unk127 != 0xFF) {
        return 1;
    }
    if ((D_800C3638 != 0) && (func_150229E4(arg0) != 0)) {
        return 1;
    }
    sum = 0;
    p = (u8 *)arg0;
    if (D_80082FA0 >= 0) {
        do {
            sum += p[0xB];
            p++;
        } while (p <= (0, D_80082FA0) + (u8 *)arg0);
    }
    if (sum == 0) {
        return 0;
    }
    if ((arg0->unk74 & 0xF) != 0xF) {
        return 1;
    }
    idx = arg0->unk13F;
    if ((*(u8 **)&D_800D2108)[0, idx] != 0) {
        ok = 1;
        for (i = 0; i < D_8008FD8C; i++) {
            dx = ((Pt150623F4 **)D_800D2104)[idx]->x - D_800CC2D0[i].x_position;
            dz = ((Pt150623F4 **)D_800D2104)[idx]->z - D_800CC2D0[i].z_position;
            if ((dx * dx) + (dz * dz) < ((Pt150623F4 **)D_800D2104)[idx]->r * ((Pt150623F4 **)D_800D2104)[idx]->r) {
                ok = 0;
                break;
            }
        }
        if (!ok) {
            return 1;
        }
    }
    id = arg0->id;
    height = *(s16 *)((u8 *)D_800D1C90[id] + 0x10) * arg0->y_scale;
    for (i = 0; i <= D_80082FA0; i++) {
        ok = 1;
        if (func_150A6360(i * 0x180 + (u8 *)D_800BE628, &D_800D9C10[i * 0x40], arg0->x_position,
                          arg0->y_position + height, arg0->z_position, scale, scale, 20000.0f) != 0) {
            ok = 0;
            break;
        }
    }
    if (!ok) {
        return 1;
    }
    return 0;
}
void func_150626EC(struct127 *arg0, s32 arg1) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((D_800CC2D0[i].interaction_state != 0) &&
            (((arg0 - D_800CC2D0) + 1) == D_800CC2D0[i].unk65) &&
            (D_800CC2D0[i].unk127 == 0xFF)) {
            func_15060F28(&D_800CC2D0[i], arg1);
        }
    }
}

void func_150627D4(struct127 *arg0) {
    arg0->unk2FB = 0;
    func_1503B840(arg0);
    func_15039CC8(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_15062800.s")

void func_15062AC4(struct127 *arg0) {
    if (arg0->unkE6 && arg0->unkE4) {
        f32 tmp = arg0->unkE4;
        arg0->unkEC = tmp / arg0->unkE6;
        arg0->unkF0 = arg0->unkE6 / tmp;
    } else {
        arg0->unkF0 = arg0->unkEC = 0.0f;
    }
}

void func_15062B1C(struct127 *arg0, f32 arg1) {
    arg0->unkE4 = arg0->xz_scale * arg1;
    func_15062AC4(arg0);
}

void func_15062B50(struct127 *arg0, f32 arg1) {
    arg0->unkE6 = arg0->y_scale * arg1;
    func_15062AC4(arg0);
}

void func_15062B84(struct127 *arg0) {
    if (arg0->unkD4 && arg0->unkD2) {
        f32 tmp = arg0->unkD2;
        arg0->unkDC = tmp / arg0->unkD4;
        arg0->unkE0 = arg0->unkD4 / tmp;
    } else {
        arg0->unkDC = 0.0f;
        arg0->unkE0 = 0.0f;
    }
}

void func_15062BDC(struct127 *arg0, f32 arg1, f32 arg2) {
    struct124 *temp_v0;

    arg0->xz_scale = arg1;
    arg0->y_scale = arg2;
    if (arg0->id != 255) {
        temp_v0 = D_800D1C90[arg0->id];
        arg0->unkD2 = (s32) (temp_v0->unk20 * arg0->xz_scale);
        arg0->unkD4 = (s32) (temp_v0->unk22 * arg0->y_scale);
        arg0->unkD6 = (s32) (temp_v0->unk24 * arg0->y_scale);
        arg0->unkE4 = (s32) (temp_v0->unk1A * arg0->xz_scale);
        arg0->unkE6 = (s32) (temp_v0->unk1C * arg0->y_scale);
        arg0->unkE8 = (s32) (temp_v0->unk1E * arg0->y_scale);
        func_15062AC4(arg0);
        func_15062B84(arg0);
    }
}

// PERMUTER CANDIDATE best 180 (EVERY instruction matches; remaining diff is a coherent 5-register
// cycle a0->a1->a3->v1->t0 rooted in the base pointer landing in a0 vs the target's a1 â€” an IDO
// free-list coin-flip. Hand-fixes: *(s32*)&arg1 homes arg1 at prologue; explicit `base` local; the
// nested default-then-adjust wrap shape; flag=arg5. Progression 3331->900->395->360->180.)
// typedef struct { s32 unk0; s32 unk4; } Entry62D10;
// extern Entry62D10 **D_800C4488[];
// void func_15062D10(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
//     Entry62D10 *base, *entry; s32 packed, modX, modY, newX, newY, tmp, flag;
//     if ((&D_800D19A0)[arg0] == 0) return;
//     flag = arg5;
//     base = D_800C4488[arg0][arg4];
//     entry = &base[*(s32 *)&arg1];
//     packed = entry->unk4;
//     modX = ((packed >> 12) & 0xFFF) + 2;
//     modY = (packed & 0xFFF) + 2;
//     packed = entry->unk0;
//     if (flag != 0) newX = arg2 & 0xFFF;
//     else { tmp = ((packed >> 12) & 0xFFF) + arg2; newX = tmp;
//            if (modX < tmp) newX = tmp - modX; else if (tmp < 0) newX = tmp + modX; }
//     if (flag != 0) newY = arg3 & 0xFFF;
//     else { tmp = (packed & 0xFFF) + arg3; newY = tmp;
//            if (modY < tmp) newY = tmp - modY; else if (tmp < 0) newY = tmp + modY; }
//     entry->unk0 = (packed & 0xFF000000) | (newY & 0xFFF) | ((newX & 0xFFF) << 12);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_83300/func_15062D10.s")
typedef struct Entry62E24 {
    u8 pad0[8];
    u16 unk8;
    u16 unkA;
} Entry62E24;

extern Entry62E24 *D_800C5338[];
void func_15094AB8(Vtx *arg0, s16 (*arg1)[2], s32 arg2, f32 arg3, s32 arg4, s32 arg5);

void func_15062E24(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 i;
    s32 w;
    s32 h;
    Vtx *vtx;

    if (D_800C6360[arg0] != 0) {
        for (i = 0; i < 25; i++) {
            if (D_800CC2D0[i].interaction_state == 0) continue;
            if (arg0 != D_800CC2D0[i].id) continue;
            if (D_800CC2D0[i].unk2FA == 0) continue;
                w = D_800C5338[arg0][arg5].unk8;
                h = D_800C5338[arg0][arg5].unkA;
                if (arg6 != 0) {
                    w *= 2;
                    h *= 2;
                }
                vtx = ((Vtx **)&D_800CC2D0[i].pad288[4])[D_800BE9C0];
                if (vtx != NULL) {
                    vtx += arg1;
                    func_15094AB8(vtx, (s16 (*)[2])D_800C6360[arg0] + arg3, arg2, arg4, h, w);
                }
        }
    }
}
typedef struct Game83300MorphVertex {
    u8 pad0[8];
    s16 x;
    s16 y;
    u8 padC[4];
} Game83300MorphVertex;

typedef struct Game83300MorphObject {
    u8 pad0[4];
    u8 modelIndex;
    u8 pad5[0x1C3];
    u8 channel;
    u8 pad1C9[0xC3];
    Game83300MorphVertex *buffers[4];
} Game83300MorphObject;

extern s32 *D_800C4020[];

void func_15062FC0(Game83300MorphObject *arg0, s32 arg1, u8 arg2,
                   s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    u8 *channelBase;
    Game83300MorphVertex *source;
    Game83300MorphVertex *target;
    s32 offset;
    s32 i;

    channelBase = (u8 *)arg0 + (arg0->channel * 8);
    target = ((Game83300MorphVertex **)(channelBase + 0x28C))[D_800BE9C0];
    source = ((Game83300MorphVertex **)(channelBase + 0x28C))[!D_800BE9C0];
    if (target == 0) {
        return;
    }
    if (arg1 != -1) {
        offset = D_800C4020[arg0->modelIndex][arg1] * 0x10;
        target = (Game83300MorphVertex *)((u8 *)target + offset);
        source = (Game83300MorphVertex *)((u8 *)source + offset);
    }
    if (arg6 != 0) {
        if (arg3 < target->x) {
            arg6 -= arg3;
        } else if (target->x < 0) {
            arg6 += arg3;
        }
    }
    if (arg7 != 0) {
        if (arg4 < target->y) {
            arg7 -= arg4;
        } else if (target->y < 0) {
            arg7 += arg4;
        }
    }
    for (i = 0; i < arg5; i++) {
        target[i].x = source[i].x + arg6;
        target[i].y = source[i].y + arg7;
    }
}
void func_15194FF4(struct127 *, struct127 *, s32);

void func_15063168(struct127 *arg0) {
    s32 i;
    s32 self;
    struct127 *obj;

    self = ((s32)arg0 - (s32)D_800CC2D0) / (s32)sizeof(struct127);
    for (i = 0; i < D_8008FD8C; i++) {
        if (i == self) {
            continue;
        }
        if (((1 << i) & D_800CC268) == 0) {
            continue;
        }
        obj = (struct127 *)D_800CC2D0[i].unk31C;
        if (obj == 0) {
            continue;
        }
        if (((u8 *)obj)[0x1AC] != 0) {
            continue;
        }
        func_15194FF4(arg0, &D_800CC2D0[i], 1);
    }
}

// PERMUTER CANDIDATE best 1960 (structural match: all field save/restore + 2 calls correct;
// target 8-aligns idx/cam spill slots leaving frame gaps at 0x28/0x4c and schedules a1=0 late).
// void func_15082A44(struct129 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
// void func_15063254(struct127 *arg0, s32 arg1, s32 arg2, f32 arg3) {
//     struct129 *obj; s32 idx; struct108 *cam;
//     f32 x, y, z, b8, u40, uC4; s32 rot;
//     idx = arg0->unk13F;
//     obj = arg0->unk144;
//     obj->unk4 = arg1; obj->unk3 = arg2; obj->unk20 = arg3; obj->unk24 = arg3; obj->unk2 = 0;
//     cam = arg0->camera;
//     z = arg0->z_position; x = arg0->x_position; y = arg0->y_position;
//     b8 = arg0->unkB8; u40 = arg0->unk40; uC4 = arg0->unkC4; rot = arg0->unk76;
//     func_15060F28(arg0, 0);
//     func_15082A44(obj, idx, 0, 0, (arg0 - D_800CC2D0) + 1);
//     arg0->x_position = x; arg0->y_position = y; arg0->camera = cam; arg0->z_position = z;
//     arg0->unkB8 = b8; arg0->unk40 = u40; arg0->unk76 = rot; arg0->unk7A = rot;
//     arg0->old_x_position = x; arg0->old_y_position = y;
//     arg0->unk1CC = arg0->y_position; arg0->old_z_position = arg0->z_position;
//     arg0->unkC4 = uC4; cam->unk3D4 = (struct127 *) arg0->unk31C;
// }
void func_15082A44(void *, s32, s32, s32, s32);
void func_15063254(struct127 *arg0, s32 arg1, s32 arg2, f32 arg3) {
    struct129 *ptr;
    s32 sp50;
    s32 index;
    struct108 *saved;
    f32 x;
    f32 y;
    f32 z;
    f32 b8;
    f32 f40;
    f32 c4;
    s32 rot;

    sp50 = arg0->unk13F;
    ptr = arg0->unk144;
    ptr->unk4 = arg1;
    ptr->unk3 = arg2;
    ptr->unk20 = arg3;
    ptr->unk24 = arg3;
    ptr->unk2 = 0;
    saved = arg0->camera;
    x = arg0->x_position;
    y = arg0->y_position;
    z = arg0->z_position;
    b8 = arg0->unkB8;
    f40 = arg0->unk40;
    c4 = arg0->unkC4;
    rot = arg0->unk76;
    index = ((s32)arg0 - (s32)&D_800CC2D0) / 812;
    func_15060F28(arg0, 0);
    func_15082A44(ptr, sp50, 0, 0, index + 1);
    arg0->camera = saved;
    arg0->x_position = x;
    arg0->y_position = y;
    arg0->z_position = z;
    arg0->unkB8 = b8;
    arg0->unk40 = f40;
    arg0->unkC4 = c4;
    arg0->unk76 = rot;
    arg0->unk7A = rot;
    arg0->old_x_position = x;
    arg0->old_y_position = y;
    arg0->old_z_position = arg0->z_position;
    arg0->unk1CC = arg0->y_position;
    arg0->camera->unk3D4 = (struct127 *)arg0->unk31C;
}
