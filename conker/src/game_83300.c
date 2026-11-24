#include <ultra64.h>

#define func_1506160C func_1506160C_hdr
#define func_1505841C func_1505841C_hdr
#define func_1000F85C func_1000F85C_hdr
#define func_1000F91C func_1000F91C_hdr
#define func_10010BE8 func_10010BE8_hdr
#define func_1505E874 func_1505E874_hdr
#include "functions.h"
#undef func_1506160C
#undef func_1505841C
#undef func_1000F85C
#undef func_1000F91C
#undef func_10010BE8
#undef func_1505E874
void func_1505E874(s32, struct127 *);
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
void func_150611E8(struct127 *arg0, s32 arg1);
void func_1505A770(void *arg0);
void func_15059C84(void *arg0);
void func_1505A3A8(f32 arg0, void *arg1, f32 arg2, f32 arg3, u8 arg4);

s32 func_15080738(s32 arg0);
void func_150642AC(struct127 *arg0, s32 arg1, f32 arg2, s32 arg3, s32 arg4);
s32 func_15065A5C(struct127 *arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void func_1505B9C4(void *arg0, struct127 *arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5, u8 arg6);
extern void (*D_80086694[])(void *, struct127 *, u8 *);
s32 func_1506E46C(struct127 *, s32 *, s32);
s32 func_1505C1E4(void *arg0, struct127 *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern struct255 *D_800CC4A4;
void func_1505F188(struct127 *arg0);
s32 func_1505C7D8(struct127 *arg0, s32 arg1);
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

typedef struct {
    s16 x;
    s16 z;
    s16 y;
    s16 h;
    u16 r;
} Seg15055E50;

typedef struct {
    Seg15055E50 *segs;
    u16 count;
    u16 id;
} Tbl15055E50;

extern Tbl15055E50 *D_800D18B0;

void func_15055E50(struct127 *arg0, s32 arg1) {
    s32 pad[5];
    f32 bestX;
    f32 bestZ;
    f32 dist;
    f32 inv;
    f32 dx;
    f32 dz;
    f32 sz;
    f32 r;
    f32 best;
    f32 y;
    s32 i;
    s32 ok;
    s32 found;
    u16 flags;
    s32 count;
    Seg15055E50 *seg;
    Seg15055E50 *segs;

    ok = 1;
    found = 0;
    if (arg0->id == 0x3A) {
        if (arg0->x_position > 1200.0f) {
            arg0->x_position = 1200.0f;
        }
        if (arg0->x_position < -1200.0f) {
            arg0->x_position = -1200.0f;
        }
        if (arg0->z_position > 1200.0f) {
            arg0->z_position = 1200.0f;
        }
        if (arg0->z_position < -1200.0f) {
            arg0->z_position = -1200.0f;
        }
    }
    if ((arg1 != 0) && (D_800D18B0 != NULL) && (arg0->unk13D < 100)) {
        best = 1000000000.0f;
        i = 0;
        while (arg1 != D_800D18B0[i].id) {
            if (D_800D18B0[i].segs == NULL) {
                return;
            }
            i++;
        }
        segs = D_800D18B0[i].segs;
        count = D_800D18B0[i].count;
        for (i = 0; i < count; i++) {
            seg = &segs[i];
            if (ok || (seg->r & 0x8000)) {
                y = arg0->y_position;
                if ((seg->y <= y) && (y <= (seg->h + seg->y))) {
                    flags = seg->r;
                    r = flags & 0x7FFF;
                    dx = arg0->x_position - seg->x;
                    sz = seg->z;
                    dz = arg0->z_position - sz;
                    flags &= 0x8000;
                    if (flags) {
                        dist = (dx * dx) + (dz * dz) + 0.1f;
                        if (dist < (r * r)) {
                            inv = 1.0f / sqrtf(dist);
                            arg0->x_position = seg->x + (dx * inv * r);
                            arg0->z_position = seg->z + (dz * inv * r);
                            arg0->unkF4 |= 0x2000;
                        }
                    } else {
                        found = 1;
                        dist = (dx * dx) + (dz * dz) + 0.1f;
                        if (dist < (r * r)) {
                            ok = 0;
                        } else {
                            if ((sqrtf(dist) - r) < best) {
                                inv = 1.0f / sqrtf(dist);
                                best = sqrtf(dist) - r;
                                bestX = seg->x + (dx * inv * r);
                                bestZ = sz;
                                bestZ += dz * inv * r;
                            }
                        }
                    }
                }
            }
        }
        if (ok && found) {
            arg0->x_position = bestX;
            arg0->z_position = bestZ;
            arg0->unkF4 |= 0x2000;
        }
    }
}

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
typedef struct Game83300AnimRate {
    u16 anim;
    u8 rate;
    u8 pad;
} Game83300AnimRate;
typedef struct Game83300AnimSet {
    u8 pad0[0xA];
    Game83300AnimRate e[3];
    u8 pad16[2];
} Game83300AnimSet;
void func_150649A0(s32, s32);

void func_150562FC(struct127 *arg0) {
    s32 *animPtr;
    Game83300AnimSet *anims;
    s16 *sub;
    u32 animIdx;
    f32 temp_f0;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    s32 temp_v0_8;
    u8 sp63;
    f32 lerp;
    s32 variant;
    s32 sp54;
    s32 a2;
    f32 sp4C;
    f32 tx;
    f32 tz;

    sp63 = arg0->unk13D - 100;
    sp54 = 0;
    sp4C = 5.0f;
    arg0->immune = 30;
    variant = 0;
    a2 = 13;
    arg0->unk180 = D_800CC2D0[sp63].unk180;
    arg0->unk1CC = arg0->y_position;
    arg0->gravity = 4.5f;
    arg0->y_velocity = 0.0;
    temp_v0_8 = arg0->unk21C;
    if (temp_v0_8 < 30) {
        arg0->unk21C = (u32)temp_v0_8 + D_800BE9A0;
    }
    if (arg0->id == 0x57) {
        arg0->unk5C = 9;
        if (arg0->unk21C < 10) {
            return;
        }
        sp54 = 1;
        if (D_800CC2D0[sp63].unk84.uh == 0x114) {
            a2 = 0x24;
        }
        if (D_800CC2D0[sp63].unk84.uh == 0x116) {
            variant = 1;
        }
        if (D_800CC2D0[sp63].unk84.uh == 0x117) {
            variant = 2;
        }
        func_150649A0(sp63, D_800C3E78);
    } else if (arg0->id == 0x8C) {
        arg0->unk5C = 4;
        if (arg0->unk21C < 10) {
            return;
        }
        sp54 = 1;
        if (D_800CC2D0[sp63].unk84.uh == 0x1A6) {
            variant = 1;
        }
        if (D_800CC2D0[sp63].unk84.uh == 0x1A7) {
            variant = 2;
        }
        func_150649A0(sp63, D_800C3E78);
    } else if ((arg0->id == 0xA8) || (arg0->id == 0xA9)) {
        arg0->unk5C = 9;
        sp54 = 1;
        if (D_800CC2D0[sp63].unk84.uh == 0x1B0) {
            variant = 1;
        }
        if (D_800CC2D0[sp63].unk84.uh == 0x1B1) {
            variant = 2;
        }
    } else if (arg0->id == 0x5E) {
        arg0->unk5C = 9;
        sp4C = 16.0f;
        sp54 = 1;
        if (D_800CC2D0[sp63].unk84.uh == 0x286) {
            variant = 2;
        }
        if (D_800CC2D0[sp63].unk84.uh == 0x27E) {
            variant = 1;
        }
    }
    animIdx = func_1505E7CC(a2, arg0);
    animPtr = (s32 *)D_800D1588[arg0->id];
    anims = (Game83300AnimSet *)animPtr[-2];
    if ((D_800CC2D0[sp63].interaction_state == 1) && (D_800CC2D0[sp63].in_water != 0)) {
        arg0->unk5C = 4;
        sp54 = 1; if (anims[animIdx].e[1].anim != 0) {
            variant = 1;
        }
    }
    if (arg0->unk21C < 20) {
        temp_v0_8 = arg0->unk21C;
        lerp = (u32)temp_v0_8 / 50.0f;
        if (D_800BE9B4 != 0) {
            lerp = 1.0f;
        }
    } else {
        lerp = 0.8000000119f;
    }
    sp70 = (u32)arg0->unk10F;
    func_150599C8(arg0, 0x10, D_800CC2D0[sp63].unk7A + (arg0->unk13E << 8));
    if (D_800CC2D0[sp63].unk1D4 != NULL) {
        temp_f0 = arg0->unk10C / 100;
        if (D_800CC2D0[sp63].interaction_state != 0x15) {
            sub = (s16 *)((u8 *)D_800CC2D0[sp63].unk1D4 + 0x240);
            if (arg0->unk21C >= 7) {
                arg0->y_position = sub[0xD];
                if (arg0->unk21C < 14) {
                    temp_f0 = ((u32)(s32)arg0->unk21C / 14.0f) * temp_f0;
                }
                arg0->y_position += temp_f0;
            }
            if (arg0->id == 0xE) {
                arg0->unkC4 = 0.0;
            } else if (arg0->id == 0x13) {
                sp70 = 204.0f;
                arg0->unkC4 = 0.0;
            }
        }
    }
    if (D_800CC2D0[sp63].interaction_state == 0x15) {
        animIdx = func_1505E7CC(0x39, arg0);
        arg0->unk7A = D_800CC2D0[sp63].unk7A;
        arg0->y_position = D_800CC2D0[sp63].y_position - 10.0f;
        if ((arg0->unk28 > 100.0f) && (arg0->unk21C >= 21)) {
            variant = 1;
        } else {
            sp4C = 0.0;
        }
        sp70 = 0.0f;
        arg0->unkC4 += (D_800CC2D0[sp63].unkC4 - arg0->unkC4) * 0.1199999973f;
        arg0->xz_velocity = 0.0f;
    }
    func_1505A184(D_800CC2D0[sp63].unk7A, sp70, 0.0, &sp70, &sp6C, &sp68);
    tx = D_800CC2D0[sp63].x_position + sp70;
    tz = D_800CC2D0[sp63].z_position + sp6C;
    arg0->x_position += (tx - arg0->x_position) * lerp;
    arg0->z_position += (tz - arg0->z_position) * lerp;
    {
        f32 rate;

        rate = (u32)anims[animIdx].e[variant].rate;
lbl:
        rate = rate / 100.0f;
        func_1505E650(arg0, anims[animIdx].e[variant].anim, rate, sp4C, 0.0, 0, 0);
    }
    if (sp54 != 0) {
        arg0->unk65 = sp63 + 1;
        arg0->unk101 |= 4;
    } else {
        arg0->unk65 = 0;
    }
}
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
typedef struct WayPt15056B08 {
    s16 x;
    s16 y;
    s16 z;
    u16 r;
} WayPt15056B08;

typedef struct ActorInfo15056B08 {
    u8 pad0[0x17];
    u8 unk17;
    u8 unk18;
    u8 unk19;
} ActorInfo15056B08;

struct Func1505841CActor;
void func_1505841C(struct Func1505841CActor *arg0, f32 arg1);
struct216 *func_1507BB28(s32, s32);
void func_1507BC14(struct127 *);
s32 func_150229E4(struct127 *);
void func_15047390(f32 mf[4][4], f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9);
void func_1504C9E4(struct127 *arg0, s8 arg1, u8 arg2);
u8 func_15059B54(struct127 *arg0, u16 arg1);
extern f32 D_800CC2E4;
extern f32 D_800CC2EC;

void func_15056B08(struct127 *arg0) {
    f32 tx;
    f32 tz;
    f32 ty;
    s32 state;
    f32 rangeSq;
    f32 dist;
    u8 idx;
    u8 mode;
    f32 sp158;
    f32 sp154;
    u8 sp153;
    u16 t22E;
    f32 speed;
    u16 sp14A;
    u8 sp149;
    f32 d2;
    struct127 *obj;
    f32 sp13C;
    u8 sp13B;
    ActorInfo15056B08 *info;
    f32 dx;
    f32 dz;
    f32 sp3C;

    sp13B = 1;
    if (arg0->unk23A) {
        arg0->unk218 = func_1507BB28(4, arg0->unk23A);
        arg0->unk21C = 0;
        arg0->unk23A = 0;
    }
    if ((D_800C3638 != 0) && (func_150229E4(arg0) != 0)) {
        return;
    }
    if ((arg0->unkF4 & 0x1000) && (arg0->unk28 < 0.200000003f)) {
        arg0->unk21C = 0;
    }
    func_1507BC14(arg0);
    mode = arg0->unk223;
    if ((mode == 0xE) || (mode == 0xF) || (mode == 0x10)) {
        mode = 1;
    }
    if (mode == 0x11) {
        mode = 0;
    }
    if (arg0->stunned != 0) {
        return;
    }
    sp14A = (((arg0->unk1E5 << 8) + arg0->unk1E7) * D_800CC264) / 100;
    sp149 = arg0->unk1E6;
    if (D_800D2108[arg0->unk13F] == 0) {
        arg0->xz_velocity = 0.0f;
        func_1505841C((struct Func1505841CActor *)arg0, 0.0f);
        return;
    }
    state = mode;
    info = (ActorInfo15056B08 *)D_800D1C90[arg0->id];
    if (arg0->unk23D == 0xFF) {
        rangeSq = 1000000000.0f;
    } else {
        rangeSq = arg0->unk23D * 8;
        rangeSq *= rangeSq;
    }
    if (((WayPt15056B08 **)D_800D2104)[arg0->unk13F]->r != 0) {
        tx = ((WayPt15056B08 **)D_800D2104)[arg0->unk13F]->x - arg0->x_position;
        tz = ((WayPt15056B08 **)D_800D2104)[arg0->unk13F]->z - arg0->z_position;
    }
    if ((arg0->unk25C & 4) && !(arg0->unk25C & 1) && ((state == 1) || (state == 3))) {
        arg0->unk21C = 0;
    }
    if ((arg0->unk25C & 1) && ((state == 0) || (state == 5))) {
        if (func_1505693C(arg0, arg0->pad259[2]) != 0) {
            arg0->unk218 = func_1507BB28(1, arg0->unk233);
            arg0->unk21C = 0;
            arg0->unkA8 = arg0->pad259[2] | 0x80;
            arg0->unk222 = arg0->pad259[2];
        }
    }
    if (arg0->pad259[1] != 0) {
        arg0->unk44 = arg0->pad259[1];
    }
    if ((state == 1) || (state == 0xC) || (state == 2) || (state == 3) || (state == 9) || (state == 0xA) ||
        ((state == 6) && !(arg0->unk22C & 1))) {
        idx = arg0->unk222;
        obj = &D_800CC2D0[idx];
        tx = obj->x_position;
        tz = obj->z_position;
        ty = obj->y_position;
        if (arg0->unk223 == 0x10) {
            f32 sp124;
            f32 sp120;
            f32 sp11C;

            sp120 = func_1505A72C(obj, arg0);
            sp124 = obj->xz_velocity * (sp120 / arg0->unk231);
            func_1505A184(obj->unk76, sp124, 0.0f, &sp124, &sp120, &sp11C);
            tx += sp124;
            tz += sp120;
        }
        dx = tx - arg0->x_position;
        dz = tz - arg0->z_position;
        dist = sqrtf((dx * dx) + (dz * dz));
        if ((state == 1) && (arg0->unk44 > 1.0f) && (sqrtf(rangeSq) < (dist * 0.8000000119f))) {
            arg0->unk21C = 0;
        }
        if (arg0->id != 0x16) {
            if ((state == 1) && (arg0->unk234 != 0) && ((dist < 370.0f) || (arg0->id == 0x21)) && (idx < 4) &&
                ((arg0->xz_velocity > 15.0f) || (arg0->id == 0x53))) {
                if (obj->unk31C != NULL) {
                    obj->unk31C->chased = 10;
                }
            }
            if ((state == 2) && (dist < 470.0f) && (idx < 4) && (((arg0->unk76 - obj->unk76) - 0x4000) & 0x8000)) {
                if (obj->unk31C != NULL) {
                    obj->unk31C->chasing = 6;
                }
            }
        }
        if (state != 9) {
            f32 range;
            s32 ok;
            u16 angle;

            if ((state != 0xA) && (state != 0xC)) {
                sp13B = 0;
            }
            if ((arg0->unk22C & 0x80) &&
                (fabsf((arg0->y_position - arg0->unk28) - (obj->y_position - obj->unk28)) > 30.0f)) {
                arg0->unk21C = 0;
            }
            if (arg0->unk223 != 0xF) {
                if (func_1505693C(arg0, arg0->unk222) == 0) {
                    arg0->unk21C = 0;
                }
            }
            if ((u16)arg0->unk22E != 0) {
                range = 0.0f;
            } else if (!arg0->unk236) {
                range = arg0->pad23C * 8;
            } else {
                range = (arg0->unk236 == 0xFF) ? 3000000.0f : arg0->unk236 * 8;
            }
            if (((state == 1) || (state == 6) || (state == 0xA)) && (arg0->unk234 != 0) && (dist < range)) {
                ok = 1;
                if ((arg0->id == 8) && (range < fabsf(arg0->y_position - obj->y_position))) {
                    ok = 0;
                }
                if (arg0->pad22A != 0) {
                    range = arg0->pad22A * 8.0f;
                    if ((D_800C3E78 != 0) && (fabsf(arg0->y_position - D_800CC2E8[0]) < range)) {
                        ok = 0;
                    }
                }
                if (ok) {
                    angle = func_1505A630(dx, -dz, 0);
                    if (arg0->unk237 &&
                        (((((arg0->unk76 - angle) >> 8) + (arg0->unk237 / 2)) & 0xFF) < arg0->unk237)) {
                        arg0->unk218 = func_1507BB28(2, arg0->unk234);
                        arg0->unk21C = 0;
                    }
                }
            }
        }
    }
    if (sp13B != 0) {
        s32 sp10C;

        t22E = *(u16 *)&arg0->unk22E;
        if (arg0->unk235 != 0) {
            sp10C = 0;
        } else {
            sp10C = 1;
        }
        if (t22E != 0) {
            if (D_800BE9A0 >= t22E) {
                arg0->unk22E = 0;
            } else {
                arg0->unk22E = t22E - D_800BE9A0;
            }
        }
        if (arg0->unk241 != 0) {
            s32 i;
            s32 best;
            f32 bestDist;
            f32 ox;
            f32 oz;
            f32 d;
            s32 cone;
            s32 angHi;

            best = 0;
            bestDist = arg0->unk23F * 8;
            for (i = 0; i < 25; i++) {
                obj = &D_800CC2D0[i];
                if ((obj->interaction_state != 0) && (obj->id == arg0->unk240) && (obj->unk2FA != 0)) {
                    ox = obj->x_position - arg0->x_position;
                    oz = arg0->z_position - obj->z_position;
                    d = sqrtf((ox * ox) + (oz * oz));
                    if (d < bestDist) {
                        cone = info->unk17 * 3;
                        cone /= 2;
                        angHi = (u8)(func_1505A630(ox, oz, 0) >> 8);
                        if ((((((arg0->unk7A >> 8) - angHi) + (cone / 2)) & 0xFF) < cone)) {
                            best = i + 1;
                            bestDist = d;
                        }
                    }
                }
            }
            if (best != 0) {
                arg0->unk218 = func_1507BB28(0, arg0->unk241);
                arg0->unk21C = 0;
                arg0->unk222 = best - 1;
                arg0->unkA8 = best + 0x7F;
                arg0->unk22E = 5;
                if (arg0->unk242 != 0) {
                    D_800D154C = &D_800CC2D0[best - 1];
                    D_800CC2D0[best - 1].unk218 = func_1507BB28(0, arg0->unk242);
                    D_800CC2D0[best - 1].unk21C = 0;
                    D_800CC2D0[best - 1].unk222 = D_800C3E78;
                    D_800D154C = arg0;
                }
            }
        }
        if (((u16)arg0->unk22E == 0) && (state != 4) && (state != 7) &&
            (((sp10C == 0) && (arg0->unk235 != 0)) ||
             ((sp10C == 1) && (arg0->unk233 != 0) && !(arg0->unk25C & 4)))) {
            u8 spEB;
            f32 px;
            f32 pz;
            f32 dsq;
            u8 t229;

            if (info->unk18 <= arg0->xz_velocity) {
                if (func_1505693C(arg0, 0) != 0) {
                    spEB = 0;
                    px = D_800CC2E4 - (s16)arg0->pad1A4;
                    pz = *(s16 *)&arg0->pad1A8 - D_800CC2EC;
                    if ((arg0->id == 0x9E) && (arg0->unk2E4 == 0)) {
                        rangeSq = 129600.0f;
                    }
                    dsq = (px * px) + (pz * pz);
                    if ((dsq < rangeSq) && (dsq < 100000000.0f)) {
                        if (func_1505693C(arg0, 0) != 0) {
                            if (arg0->unk229 == 0xFF) {
                                dsq = sqrtf(rangeSq);
                            } else {
                                dsq = arg0->unk229 * 8.0f;
                            }
                            if ((D_800C3E78 != 0) &&
                                (!arg0->unk229 || (fabsf(arg0->y_position - D_800CC2E8[0]) < dsq))) {
                                if (arg0->id == 0x9E) {
                                    f32 mtx[4][4];
                                    f32 sp94;
                                    f32 sp90;
                                    f32 sp8C;

                                    func_1505A184(arg0->unk7A, 9000.0f, arg0->unkB8, &sp94, &sp90, &sp8C);
                                    sp94 += arg0->x_position;
                                    sp90 += arg0->z_position;
                                    sp8C += arg0->y_position;
                                    func_15047390(mtx, arg0->x_position, arg0->y_position, arg0->z_position, sp94, sp8C,
                                                  sp90, 0.0f, 1.0f, 0);
                                    if (func_15049440(mtx, D_800CC2E4, D_800CC2E8[0], D_800CC2EC, 80.0f, 80.0f, 0.0f,
                                                      4000.0f, 320.0f, 320.0f) != 0) {
                                        spEB = 1;
                                    }
                                } else {
                                    s32 sp88;

                                    sp88 = info->unk17;
                                    if (sp10C == 0) {
                                        sp88 *= 3;
                                    }
                                    if (((((((arg0->unk7A >> 8) + arg0->unk2CA) - (u8)(func_1505A630(px, pz, 0) >> 8)) +
                                           (sp88 / 2)) & 0xFF) < sp88)) {
                                        spEB = 1;
                                    }
                                }
                            }
                        }
                    }
                    if (spEB) {
                        if (sp10C == 0) {
                            arg0->unk218 = func_1507BB28(3, arg0->unk235);
                        } else {
                            arg0->unk218 = func_1507BB28(1, arg0->unk233);
                        }
                        arg0->unk21C = 0;
                        arg0->unk222 = spEB - 1;
                        arg0->unkA8 = spEB + 0x7F;
                    }
                }
            }
        }
        if (state != 9) {
            if ((state == 6) || (state == 0xC)) {
                tx = D_800CC2D0[arg0->unk222].x_position;
                tz = D_800CC2D0[arg0->unk222].z_position;
                ty = D_800CC2D0[arg0->unk222].y_position;
            } else {
                tx = ((WayPt15056B08 **)D_800D2104)[arg0->unk13F][arg0->unk21E + 1].x;
                tz = ((WayPt15056B08 **)D_800D2104)[arg0->unk13F][arg0->unk21E + 1].z;
                ty = ((WayPt15056B08 **)D_800D2104)[arg0->unk13F][arg0->unk21E + 1].y;
            }
            if ((state == 5) || (state == 6)) {
                f32 sp84;
                f32 sp80;
                f32 sp7C;

                if ((state == 6) && (arg0->unk22C & 1)) {
                    arg0->unk86 = D_800CC2D0[arg0->unk222].unk7A >> 8;
                }
                arg0->unk86 += arg0->unk257;
                sp84 = arg0->unk258 * 16.0f;
                func_1505A184(arg0->unk86 << 8, sp84, 0.0f, &sp84, &sp80, &sp7C);
                tx += sp84;
                tz += sp80;
            }
        }
    }
    speed = arg0->unk44;
    if ((state != 7) && (state != 8)) {
        tx = tx - arg0->x_position;
        tz = arg0->z_position - tz;
        ty = arg0->y_position - ty;
        if ((state == 6) && (arg0->unk22C & 1) && ((fabsf(tx) + fabsf(tz)) < 100.0f)) {
            speed = 0.0f;
            tx = D_800CC2D0[arg0->unk222].x_position - arg0->x_position;
            tz = arg0->z_position - D_800CC2D0[arg0->unk222].z_position;
            arg0->unk78 = func_1505A630(tx, tz, 0);
        } else if ((arg0->unk44 >= 0.5f) && !(arg0->unkF4 & 0x40)) {
            arg0->unk78 = func_1505A630(tx, tz, 0);
        }
    }
    sp158 = arg0->unk247 / 10.0f;
    sp3C = (tx * tx) + (tz * tz);
    d2 = sp3C;
    if (arg0->unk22C & 8) {
        d2 += ty * ty;
    }
    d2 = sqrtf(d2);
    if ((state != 4) && (state != 7) && (state != 8)) {
        u8 k;
        u8 n;
        u16 sp78;
        u16 ang;
        s32 flags;

        sp78 = arg0->unk76;
        if (state == 2) {
            arg0->unk78 ^= 0x8000;
            if (!(arg0->unkF4 & 0x4000)) {
                arg0->unk78 += (((arg0->unk21C / 100) & 0x20) - 0x10) << 8;
            }
        }
        if (arg0->unk223 == 0x11) {
            arg0->unk78 += (((arg0->unk21C / 100) & 0x10) - 8) << 9;
        }
        if ((arg0->unkF4 & 0x200) && (*(f32 *)&arg0->unk1F4 < 3.0f)) {
            sp14A = 0;
            sp149 = 0;
        }
        sp13C = arg0->unk23E * 8;
        sp153 = func_15059B54(arg0, sp14A);
        if (state != 0xD) {
            if (((u8)arg0->unk24F != 0) && ((state != 0) || (sp13C < d2))) {
                k = (u8)arg0->unk24F - 1;
                if (((u8(*)[10])D_80099A3C)[k][0] < sp153) {
                    arg0->unk76 = sp78;
                    func_15056A00(arg0, sp153, k);
                }
            }
        }
        if (state == 0xD) {
            sp14A = 0;
            sp149 = 0;
        }
        if ((info->unk19 < sp153) && (state != 2)) {
            speed = 0.0f;
            if (arg0->unk24A && (arg0->xz_velocity > 15.0f)) {
                arg0->unk218 = func_1507BB28(6, arg0->unk24A);
                arg0->unk21C = 0;
                arg0->unk222 = 0;
            }
        }
        if (((state == 0) || (state == 0xA)) && (d2 < sp13C)) {
            n = D_800D2108[arg0->unk13F] - 1;
            if (arg0->unk21F != 0) {
                n = arg0->unk21F;
            }
            if (arg0->unk22C & 4) {
                arg0->x_position = ((WayPt15056B08 **)D_800D2104)[arg0->unk13F][arg0->unk21E + 1].x;
                arg0->z_position = ((WayPt15056B08 **)D_800D2104)[arg0->unk13F][arg0->unk21E + 1].z;
                arg0->unk1FC |= 8;
                if (arg0->unk22C & 8) {
                    arg0->y_position = ((WayPt15056B08 **)D_800D2104)[arg0->unk13F][arg0->unk21E + 1].y;
                }
            }
            if (!(arg0->unkF4 & 0x10)) {
                arg0->unk21C = 0;
            }
            arg0->unk21E += arg0->unk221;
            arg0->unk21E += n;
            arg0->unk21E %= n;
            if (arg0->unk21E < arg0->unk220) {
                arg0->unk21E = arg0->unk220;
            }
        }
        ang = arg0->unk78;
        if (arg0->unkF4 & 0x20000) {
            ang = func_1505A630(D_800CC2D0[arg0->unk222].x_position - arg0->x_position,
                                arg0->z_position - D_800CC2D0[arg0->unk222].z_position, 0);
        }
        sp153 = func_150599C8(arg0, sp149, ang);
        sp154 = arg0->unk248 / 10.0f;
        if ((arg0->unk3A != 0) && (arg0->unk28 == 0.0f) && !(arg0->unk3A & 3 & D_800D1548)) {
            arg0->unk84.uh = 0xFF;
            arg0->y_velocity = arg0->unk3A;
        }
        flags = arg0->unkF4;
        if (flags & 1) {
            speed = 0.0f;
        }
        if ((arg0->unk22C & 0x10) && (arg0->unk28 == 0.0f)) {
            speed = 0.0f;
            sp158 = 1.0f;
        }
        if (arg0->interaction_state == 1) {
            arg0->unk44 = speed;
            return;
        }
        if (flags & 0x80000) {
            f32 h;

            h = ty + 70.0f;
            if (D_800BE9F0 == 0x26) {
                h -= 45.0f * arg0->y_scale;
            }
            ang = func_1505A630(sqrtf(sp3C), h, 0);
            func_1504C9E4(arg0, (s16)ang / 200, 0);
        } else if (arg0->unk1D1 != 0) {
            func_1504C9E4(arg0, 0, 0);
        }
        if (arg0->unkF8 & 0x40000) {
            f32 sp68;
            f32 sp64;
            f32 sp60;
            f32 r;

            speed = arg0->unk44;
            func_1505A184(arg0->unk78, speed, 0.0f, &sp68, &sp64, &sp60);
            if ((speed < 1.0f) || (((sp78 - arg0->unk78) + 0x4000) & 0x8000)) {
                r = sp154;
            } else {
                r = sp158;
            }
            func_1505A250(sp68, sp64, r, (f32 *)&arg0->pad174, (f32 *)&arg0->pad178);
            arg0->unk76 = func_1505A630(*(f32 *)&arg0->pad174, -*(f32 *)&arg0->pad178, 0);
            r = sqrtf((*(f32 *)&arg0->pad178 * *(f32 *)&arg0->pad178) + (*(f32 *)&arg0->pad174 * *(f32 *)&arg0->pad174));
            arg0->xz_velocity = r + r;
        } else {
            func_1505A3A8(speed, arg0, sp158, sp154, sp153);
        }
    } else {
        func_150599C8(arg0, arg0->unk1E6, arg0->unk78);
        arg0->unk76 = arg0->unk78;
        arg0->xz_velocity = speed;
    }
    func_1505841C((struct Func1505841CActor *)arg0, sp158);
    if ((arg0->unk22C & 8) && !(arg0->unk1FC & 8)) {
        f32 a;
        f32 lim;

        a = (u32)func_1505A630(sqrtf(sp3C), ty, 0) * 0.005493164f;
        if (a > 180.0f) {
            a -= 360.0f;
        }
        a = (a - arg0->unkB8) * 0.25f;
        lim = sp14A * 0.005493164f;
        if (a > lim) {
            a = lim;
        }
        if (a < -lim) {
            a = -lim;
        }
        arg0->unkB8 = arg0->unkB8 + a;
    }
}
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
void func_1505A3A8(f32 arg0, void *arg1, f32 arg2, f32 arg3, u8 arg4) {
    struct127 *obj = (struct127 *)arg1;

    arg2 *= D_800D1550;
    arg3 *= D_800D1550;
    if (D_800CC27C != 0 && obj->unk28 < 5.0f && obj->interaction_state != 0x1E) {
        arg2 = 0.0f;
    }
    if (obj->interaction_state == 1) {
        if (obj->in_water != 0) {
            arg3 *= 0.400000006f;
            arg2 = 2.0f;
        }
        if (obj->unkA8 != 0) {
            arg3 *= 0.25f;
        }
        if (obj->unk81 != 0) {
            if (obj->unk81 & 0x40) {
                if (obj->unk81 & 0x20) {
                    arg3 = 0.0f;
                } else {
                    arg3 = 1.299999952f;
                }
            } else {
                arg3 *= 0.349999994f;
                if (arg4 >= 0x2E) {
                    arg2 *= 0.5f;
                }
                if (arg4 >= 0x5B) {
                    arg0 *= 0.5f;
                }
            }
        }
        if (obj->unkAE != 0) {
            arg3 *= 0.200000003f;
        }
    }
    if (arg0 < 0.0f) {
        if (0.0f < obj->xz_velocity) {
            arg3 += arg2;
        } else {
            arg3 = arg2;
        }
    } else if (obj->xz_velocity < 0.0f) {
        arg2 += arg3;
    }
    if (arg0 < obj->xz_velocity) {
        obj->xz_velocity -= arg3;
        if (obj->xz_velocity < arg0) {
            obj->xz_velocity = arg0;
        }
    } else {
        if ((arg0 - 1.0f) < obj->xz_velocity) {
            arg2 *= 0.009999999776f;
        }
        obj->xz_velocity += arg2;
        if (arg0 < obj->xz_velocity) {
            obj->xz_velocity = arg0;
        }
    }
}

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
void func_15055C88(struct127 *);
void func_1513783C(struct127 *, u8, s32);
void func_1507CD64(struct127 *arg0, s32 arg1);
void func_15060A9C(s32 arg0, struct127 *arg1);

void func_1505A9AC(struct127 *arg0, f32 arg1) {
    f32 dist;
    f32 thresh;
    u8 sp6F;
    u8 sp6E;
    s32 health;
    s32 temp;
    u8 sp63;
    s32 killed;
    s32 anim;
    s32 flags;
    f32 speed;
    s32 hurt;
    u8 found;
    u8 d;

    sp6F = 0;
    sp6E = 1;
    dist = arg0->y_position - arg1;
    if (arg0->unk31C->pad4F[0] != 0) {
        if (dist < 0.0f) {
            arg0->unk1CC = arg1;
            dist = 0.0f;
        }
        arg0->unk28 = dist;
        dist = arg0->y_position - (arg0->unk118 - 34.0f);
        if (dist < 70.0f && arg0->y_velocity < 18.0f) {
            if (arg0->y_velocity > -25.0f) {
                arg0->unk31C->pad4F[0] = 2;
            }
            if (dist < 0.0f) {
                if (arg0->y_velocity < -12.0f) {
                    arg0->unk81 = 0;
                    if (arg0->y_velocity < -25.0f) {
                        arg0->xz_velocity *= 0.400000006f;
                        arg0->y_velocity = -30.0f;
                    }
                    arg0->y_velocity *= 0.8500000238f;
                }
                arg0->gravity = -1.899999976f;
            } else {
                if ((arg0->y_velocity > 10.0f) && (arg0->y_velocity < 25.0f)) {
                    arg0->y_velocity = 10.0f;
                }
                arg0->gravity = 2.200000048f;
            }
        }
    } else if (arg0->unk65 != 0) {
        if (dist < 0.0f) {
            arg0->unk1CC = arg1;
            dist = 0.0f;
            arg0->y_position = arg1;
        }
        arg0->unk28 = dist;
    } else {
        if (dist < 0.0f) {
            f32 fall;
            
            sp63 = 0;
            if (arg0->in_water == 0) {
                *((u8 *)arg0 + 0x137) = 0;
                arg0->gravity = 5.0f;
            }
            if (arg0->in_water == 0) {
                arg0->unk31C->unk55 = 4;
            }
            if (arg0->unk28 > 0.0f) {
                arg0->unk31C->pad4F[2] = 1;
                arg0->unk31C->unk31 = 0;
            }
            arg0->unk100 &= 0xFD;
            if ((((u32)arg0->unk184 >> 12) & 0xF) == 1) {
                if (D_800BE9F0 == 0x14) {
                    func_1513783C(arg0, 0xFF, 1);
                    func_1507CD64(arg0, 8);
                    return;
                }
                func_15055C88(arg0);
                return;
            }
            fall = 600.0f;
            if (D_800BE9F0 == 0x43) {
                thresh = 1000.0f;
            } else {
                thresh = fall * 3.0f;
            }
            if ((arg0->unk1CC - arg0->y_position) > fall && arg0->unk31C->unk120 == 0 &&
                arg0->unk31C->unk78 != 1) {
                speed = 1.299999952f;
                do { arg0->unk31C->unk95 = 0; } while (0);
                do { arg0->unk31C->pad4F[2] = 2; } while (0);
                killed = 0;
                flags = 0;
                hurt = 0;
                anim = 0x30;
                if (D_800BE9F0 != 0x29) {
                    temp = arg0->unk184;
                    if (((((u32)temp >> 12) & 0xF) == 2) || ((temp == 0x13) & 0x1F)) {
                        anim = 0xC8;
                        func_15060B70(0x1B8, arg0);
                        speed = 1.0f;
                    } else {
                        health = arg0->health;
                        if (health != 0) {
                            if (arg0->unk31C->unk78 == 0x21) {
                                flags = 1;
                            }
                            if (arg0->id == 0x75) {
                                flags |= 2;
                            }
                        }
                        if ((arg0->unk1CC < 90000.0f) && (thresh < (arg0->unk1CC - arg0->y_position))) {
                            if (!(flags & 2)) {
                                arg0->health = 0;
                                if ((temp & 0x1F) != 0xE) {
                                    func_1513783C(arg0, 0xFF, 1);
                                    killed = 1;
                                }
                            } else {
                                hurt = 1;
                                if (health != 0) {
                                    arg0->health = health - 1;
                                }
                            }
                        }
                        if (flags == 0) {
                            health = arg0->health;
                            arg0->stunned = 8;
                            if (health != 0) {
                                arg0->health = health - 1;
                            }
                        }
                        if (flags & 1) {
                            speed = 1.0f;
                            anim = 0x19D;
                        } else if (flags & 2) {
                            if (hurt) {
                                anim = 0x27;
                            } else {
                                anim = 9;
                            }
                        } else if (arg0->health != 0) {
                            anim = 0x27;
                        }
                        if ((anim == 0x27) && (arg0->unk31C->unk78 == 0x25)) {
                            anim = 0x1AA;
                        }
                    }
                }
                if (killed == 0) {
                    func_1505E650(arg0, anim, speed, 2.0f, 6.0f, 0.0f, 0);
                }
                arg0->stunned = 0;
                arg0->unk83 = 0xFF;
                arg0->disable_run = 0xFE;
                if (flags != 0) {
                    arg0->unk228 = 10;
                } else {
                    arg0->unk228 = 30;
                }
                arg0->unk28 = 0.0f;
                arg0->unk76 = arg0->unk7A;
                arg0->unk100 &= 0x8B;
                arg0->y_position = arg1;
                arg0->unkA9 = 0;
                arg0->y_velocity = -4.0f;
                arg0->unk1CC = arg1;
                if (killed == 0) {
                    func_15174690(D_800C3E78, 0, 0x1F, 0, 0x199, 4, 0xAA, 0xFF, 0);
                } else {
                    func_1507CD64(arg0, 8);
                }
                return;
            }
            arg0->unk1CC = arg1;
            if (arg0->in_water != 0) {
                arg0->y_position = arg1;
                arg0->unk28 = 0.0f;
                return;
            }
            if (arg0->stunned == 0) {
                if ((arg0->unk7F == 0) && (arg0->xz_velocity < 9.0f)) {
                    arg0->unk76 = arg0->unk7A;
                }
                d = (arg0->unk76 - arg0->unk7A) >> 8;
                if (d > 0x80) {
                    d = -d;
                }
                if (!arg0->unk7F && (d < 0x50)) {
                    arg0->unk7F = 10;
                }
                if (arg0->unkA9 == 2) {
                    struct127 *obj;
                    found = 0;
                    if (arg0->unk274 != 0) {
                        obj = &D_800CC2D0[arg0->unk274 - 1];
                        if (obj->unk24C != 0) {
                            found = 1;
                            obj->unk218 = 0;
                            obj->unk232 = obj->unk24C;
                        }
                    }
                    arg0->xz_velocity = 0.0f;
                    arg0->unk83 = 0xFF;
                    sp63 = 0xFF;
                    func_1505E650(arg0, 0x4B, 1.0f, 2.0f, 0.0f, 0.0f, 0);
                    if (D_800C3E78 < 4) {
                        arg0->unk31C->unk57 = 1;
                    }
                    if (found == 0) {
                        func_15060A9C(0x19, arg0);
                    }
                } else {
                    s32 anim2;
                    anim2 = 0;
                    if ((s32)arg0->unk28 > 0) {
                        struct126 *s = arg0->unk31C;
                        if ((s->unk11A != 1) && (s->unk11A != 3) && (s->unk4E == 0)) {
                            if ((arg0->unk100 & 0x10) || (!(arg0->unk100 & 8) && (((arg0->unk44 == 0.0f) && (arg0->unkA8 == 0)) || (arg0->unk100 & 4)))) {
                                if (arg0->unk100 & 0x10) {
                                    arg0->unk83 = 8;
                                    if (!(arg0->unk100 & 0x40)) {
                                        arg0->xz_velocity = 0.0f;
                                        arg0->unk83 = 0xD;
                                        sp63 = 5;
                                    }
                                } else if (arg0->unk100 & 4) {
                                    arg0->xz_velocity *= 0.6999999881f;
                                    arg0->unk83 = 0x14;
                                } else {
                                    arg0->xz_velocity = 0.0f;
                                    arg0->unk83 = 0x10;
                                }
                                if (arg0->unk31C->unk78 == 0x21) {
                                    anim2 = 0x19D;
                                } else if (arg0->unk31C->unk78 == 0x25) {
                                    anim2 = 0xD8;
                                } else {
                                    anim2 = 0x36;
                                }
                            }
                        }
                    }
                    if ((arg0->unk84.uh == 0x3B) || (arg0->unk84.uh == 0x3C)) {
                        u32 t;
                        t = arg0->unk184 & 0x1F;
                        sp63 = 0xFE;
                        sp6E = 0;
                        arg0->unk83 = 0xFF;
                        if ((t == 0xA) || (t == 0xB) || (t == 0xC) || (t == 0xF) || (t == 0x13)) {
                            anim2 = 0xC8;
                            func_15060B70(0x1B8, arg0);
                        } else if ((t == 1) || (t == 2) || (t == 7) || (t == 0x11)) {
                            anim2 = 0xC8;
                            func_15060B70(0xA7, arg0);
                        } else {
                            anim2 = 0x27;
                            func_15060B70(0xA7, arg0);
                        }
                    }
                    if ((anim2 != 0) && !(arg0->unk100 & 0x20)) {
                        func_1505E650(arg0, anim2, 1.0f, 2.0f, 0.0f, 0.0f, 0);
                    }
                    if (!(arg0->unk100 & 0x80)) {
                        arg0->unk100 = 1;
                    }
                    if (sp6E == 0) {
                        arg0->unk100 = 0x90;
                    }
                }
                if ((arg0->unk28 > 0.0f) && !(arg0->unk100 & 8)) {
                    if (D_800CC27C != 0) {
                        if (arg0->xz_velocity > 20.0f) {
                            arg0->xz_velocity = 20.0f;
                        }
                    } else if (*((u8 *)arg0 + 0x110) == 0) {
                        arg0->unk164 = 0.0f;
                        arg0->unk168 = 0.0f;
                    }
                    arg0->disable_run = sp63;
                    if ((arg0->unk31C->unk4E == 1) && (arg0->y_velocity < -10.0f)) {
                        sp6F = 2;
                    }
                }
                arg0->unkA9 = 0;
            }
            if (!(arg0->unk100 & 8)) {
                dist = 0.0f;
            } else {
                dist = 0.1000000015f;
            }
            arg0->y_position = arg1;
            if ((arg0->stunned == 0) && !(sp6F & 2)) {
                arg0->y_velocity = -4.0f;
            } else {
                if (arg0->unkF4 & 0x80) {
                    arg0->y_velocity = -20.0f;
                } else if (arg0->y_velocity > -10.0f) {
                    arg0->y_velocity = 0.0f;
                } else {
                    arg0->y_velocity = arg0->y_velocity * -0.5f;
                    if (arg0->y_velocity > 28.0f) {
                        arg0->y_velocity = 28.0f;
                    }
                }
                if (arg0->unk109 != 0) {
                    arg0->xz_velocity *= 0.6000000238f;
                }
            }
            arg0->unk81 = 0;
            if (arg0->unk7F < 0xFA) {
                arg0->unk7F++;
            }
            arg0->unk100 &= 0x9F;
        } else if ((dist < 1.0f) && (arg0->unk28 > 0.0f)) {
            dist = 1.0f;
        }
        if (dist > 20.0f) {
            arg0->unk7F = 0;
        }
        if (arg0->in_water != 0) {
            arg0->unk1CC = arg0->y_position;
        }
        arg0->unk28 = dist;
    }
}

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
extern f32 D_80099518;
extern f32 D_800CC270, D_800CC274, D_800CC278;
extern u8 D_800BE616;
extern s16 D_800D1582;
f32 func_1506AD30(struct127 *, f32, s32, s32);
void func_151AE264(struct127 *);
void func_151E5FF4(void *, struct127 *, u8);
void func_15169070(s32, s32, void *, s32);
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define ENT3 ((u8 *)arg3 + arg4 * 0x28)
#define ENT2 ((u8 *)arg2 + arg4 * 0x28)
#define W(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define F(p, o) (*(f32 *)((u8 *)(p) + (o)))

void func_1505B9C4(void *arg0, struct127 *arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5, u8 arg6) {
    u8 sp67;
    s32 temp_a2;
    struct127 *temp_v1_3;
    s32 temp_a3;
    s16 id;
    u8 tu;
    s32 v;
    u8 *pv;
    u8 *a1;
    struct S { void *a; u8 b; } sp40;

    if (B(ENT3, 0) & 0x80) {
        sp67 = B(ENT3, 0) & 0x7F;
    } else {
        sp67 = B(ENT2, 0) & 0x7F;
    }
    arg1->unk10B = 0;
    if ((H(ENT2, 0x24) & 1) && (arg1->stunned != 0)) {
        arg6 = 0;
    }
    if (!(B(ENT3, 0xF) & 0x40)) {
        arg1->unk76 = H(arg0, 0x76) + (B(ENT3, 0x10) << 8);
        if (H(ENT2, 0x24) & 0x10) {
            arg1->unk76 = func_1505A630(arg1->x_position - F(arg0, 0x14), F(arg0, 0x1C) - arg1->z_position, 0);
        }
        if (B(arg0, 4) == 0x44) {
            arg1->unk76 = 0;
        }
    }
    if ((B(ENT2, 0xF) | B(ENT3, 0xF)) & 0x20) {
        arg1->unk10B |= 2;
    }
    arg1->unk3A = 0;
    arg1->unk109 = B(ENT3, 0x20);
    arg1->y_velocity = F(ENT3, 0x18);
    arg1->xz_velocity = F(ENT3, 0x14);
    arg1->gravity = F(ENT3, 0x1C);
    arg1->unk100 &= 0xD6;
    v = B(ENT3, 0xF) & arg6;
    if (v < arg1->health) {
        arg1->health -= v;
    } else {
        arg1->health = 0;
    }
    if (arg1->interaction_state == 1) {
        {
            extern s32 D_800ZZZZ;
            temp_v1_3 = D_800D154C;
            temp_a3 = D_800C3E78;
            D_800D154C = arg1;
            D_800C3E78 = (arg1 - D_800CC2D0);
            pv = (u8 *)D_800CC2D0[D_800C3E78].unk31C;
            if (B(pv, 0x4E) != 2) {
                B(pv, 0x4E) = 0;
            }
        }
        func_1506AD30(arg1, 800.0f, 1, temp_a3);
        D_800D154C = temp_v1_3;
        D_800C3E78 = temp_a3;
        arg1->unkF8 &= 0xFEFF7FFF;
        B(arg1->unk31C, 0x95) = 0;
        B(arg1->unk31C, 0x6B) = 0;
        if (B(arg1->unk31C, 0x98) != 0) {
            func_151AE264(arg1);
        }
        B(arg1->unk31C, 0x54) = 0;
        tu = arg1->unk65;
        B(arg1, 0x137) = 0;
        if (tu != 0) {
            extern s32 D_800ZZZZ;
            v = tu - 1;
            a1 = (u8 *)&D_800CC2D0[(u8)v];

            if (((0x8A != B(a1, 4)) && (0x23 != B(a1, 4))) || (arg1->health == 0) || ((D_800BE616 != 0) && (arg1->y_velocity > 30.0f))) {
                if (v) {}
                v = 0;
                if (0x8A == B(a1, 4)) {
                    v = 1;
                }
                if (B(a1, 4) == 0x23) {
                    v = 0xA;
                }
                if (B(a1, 4) == 0xC) {
                    v = 0xF;
                }
                if (v != 0) {
                    B(a1, 0x232) = v;
                    W(a1, 0x218) = 0;
                }
                B(a1, 0x2FC) = 0;
                B(a1, 0x13C) = 0;
                arg1->unk239 = 0;
                arg1->unk65 = 0;
            }
        }
    }
    v = B(ENT3, 8);
    if (v == 0) {
        v = B(ENT2, 8);
    }
    if (sp67 != 0x27) {
        D_80086694[v](arg0, arg1, &sp67);
    }
    if (D_800BE616 != 0) {
        tu = sp67;
        v = B(arg0, 0x124);
        a1 = v != 0 ? (u8 *)&D_800CC2D0[v - 1] : arg0;
        func_151E5FF4(a1, arg1, tu);
    }
    {
        extern s32 D_800ZZZZ;
        sp40.a = arg0;
        sp40.b = B(arg0, 0x3B);
        func_15169070(0, 0x68, (void *)(s32)&sp40, 2);
        sp40.a = arg1;
        sp40.b = arg1->unique_id;
        func_15169070(0, 0x68, (void *)(s32)&sp40, 2);
    }
    if (W(ENT2, 4) != 0) {
        D_800D1580 = W(ENT2, 4);
        if (func_1506E46C(arg1, &D_800D1580, 0) != 0) {
            func_10010630(D_800D1582, arg1, 0x6D60, 0x12C, 0x708);
        }
    } else {
        if (W(ENT3, 4) != 0) {
            D_800D1580 = W(ENT3, 4);
            if (func_1506E46C(arg1, &D_800D1580, 0) != 0) {
                func_10010630(D_800D1582, arg1, 0x6D60, 0x12C, 0x708);
            }
        }
    }
    B(arg0, 0x107) = 0x3C;
    if ((D_800BE616 == 0) || (arg1->interaction_state != 1)) {
        arg1->unk21C = 0;
        if (H(ENT2, 0x24) & 0x100) {
            arg1->unk218 = NULL;
        }
    }
    temp_a2 = arg5 - 1;
    arg1->unkD0 = 0;
    v = B(arg0, 0x124);
    if (v) {
        arg1->pad10A = v + 0x7F;
    } else {
        arg1->pad10A = D_800C3E78 | 0x80;
    }
    B(arg0, 0x108) = temp_a2 | 0x80;
    if (sp67 != 0x27) {
        if ((H(ENT2, 0x24) & 0x40) && (arg1->interaction_state == 1)) {
            *(s16 *)((u8 *)D_800CC2D0[temp_a2].unk31C + 0x46) = 0x1F40;
        } else {
            v = arg1->unk23B;
            if (v != 0) {
                sp67 = v;
            }
            arg1->stunned = 0xFE;
            arg1->unk105 = 0;
            arg1->unk106 = func_1505E7CC(sp67, arg1);
            arg1->unk84.uh = 0xFFFF;
            func_1505E874(temp_a2, arg1);
            if (H(ENT2, 0x24) & 0x200) {
                arg1->unk10B |= 0x20;
            }
            if (arg1->y_velocity > 0.0f) {
                arg1->unk28 += D_80099518;
            }
            if (H(ENT2, 0x24) & 8) {
                arg1->unk10B |= 0x40;
                D_800CC270 = F(arg0, 0x14);
                D_800CC274 = F(arg0, 0x1C);
                D_800CC278 = F(arg0, 0x18);
            }
        }
        if (H(ENT2, 0x24) & 2) {
            arg1->unk10B |= 0x80;
        }
    }
    if ((H(ENT2, 0x24) & 0x400) || (H(ENT3, 0x24) & 0x400)) {
        arg1->unk10B |= 4;
    }
    v = B(ENT2, 0x22);
    if (v & 0x80) {
        arg1->immune = v & 0x3F;
    }
}

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
s32 func_1505C7D8(struct127 *arg0, s32 arg1);
typedef struct {
    u8 pad0[9];
    u8 unk9;
    u8 unkA;
    u8 padB;
    s16 unkC;
    u8 unkE;
    u8 padF[0x24 - 0xF];
    u16 unk24;
    u8 pad26[2];
} Hit1505C7D8; /* size 0x28 */

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec1505C7D8;

extern s32 D_800B2CD0;
extern f32 D_8009951C;
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, void *, void *, void *, f32 *, s32, s32, f32);
void func_1507C3E0(struct127 *, s16 *, s32, s32);
f32 func_15143E64(f32 *);

s32 func_1505C7D8(struct127 *arg0, s32 arg1) {
    s32 i;
    s32 n;
    s32 idx;
    s32 ret;
    struct127 *obj;
    Hit1505C7D8 *hits;
    f32 dz2;
    Hit1505C7D8 *hit;
    f32 x;
    f32 y;
    f32 z;
    f32 radius;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 dist;
    f32 maxDist;
    f32 s;
    f32 c;
    f32 rx;
    f32 rz;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 h;
    f32 r;
    f32 t;
    f32 offZ;
    f32 height;
    f32 pad1;
    f32 pad2;
    f32 pad3;
    f32 offZ2;
    f32 pad4;
    f32 pad5;
    Vec1505C7D8 pos;
    f32 sp_E4;
    f32 sp_E0;
    f32 sp_DC;
    f32 d[3];
    f32 padCC;
    f32 hitDist;
    f32 padC4;
    f32 padC0;
    f32 padBC;
    s16 sp_BA;

    arg1 = (arg1 & 0x7F) + 1;
    ret = 0;
    if (arg0->unk114 < 254.0f) {
        arg0->unk114 -= D_800D1550;
    }
    if (arg0->unk114 < 0.0f) {
        arg0->unkD0 = 0;
        return 0;
    }
    if (D_800C35EA == 1) {
        return 0;
    }
    idx = arg0->unkD0 - 1;
    hits = (Hit1505C7D8 *)func_1505C1A4(arg0);
    if (hits[idx].unk24 & 0x80) {
        hit = (Hit1505C7D8 *)((u8 *)hits + idx * sizeof(Hit1505C7D8));
        y = arg0->y_position;
    } else {
        hit = (Hit1505C7D8 *)((u8 *)hits + idx * sizeof(Hit1505C7D8));
        dist = (u32)hit->unkA;
        dist *= arg0->xz_scale / 0.125f;
        func_1505A184((hit->unk9 << 8) + arg0->unk7A, dist, 0.0f, &x, &z, &y);
        x += arg0->x_position;
        y += arg0->y_position;
        z += arg0->z_position;
        radius = (u32)hit->unkE;
        radius = arg0->xz_scale * 4.0f * radius;
    }
    dist = hit->unkC;
    y += dist * (arg0->y_scale / 0.25f);
    if (arg0->unkF8 & 0x100) {
        n = 4;
    } else {
        n = 26;
    }
    for (i = 1; i < n; i++) {
        obj = &D_800CC2D0[i - 1];
        if (obj->interaction_state == 0) {
            continue;
        }
        if (i == arg1 || obj->health == 0 || obj->immune != 0) {
            continue;
        }
        hit = (Hit1505C7D8 *)((u8 *)hits + idx * sizeof(Hit1505C7D8));
        if (i == arg0->unk124 && !(hit->unk24 & 0x1000)) {
            continue;
        }
        if (hit->unk24 & 0x80) {
            s16 ox;

            dist = 2.0f;
            maxDist = 1.0f;
            dx = obj->x_position - arg0->x_position;
            dz = obj->z_position - arg0->z_position;
            if (obj->unkD8 != 0 || obj->unkDA != 0) {
                dx += func_1505D34C(obj->unk40, obj->unkDA, obj->unkD8, 1.0f, &offZ);
                dz += offZ;
            }
            rx = (arg0->unk40 - 90.0f) * 0.01745329238f;
            s = func_150AD780(-rx);
            c = func_150AD78C(-rx);
            rx = dx * s + dz * c;
            dz = -dx * c + dz * s;
            hx = hit->unk9 * 5;
            dz2 = hit->unkA * 5;
            ox = (s8)hit->unkE * 5;
            dy = ox * arg0->xz_scale;
            dx = rx - dy;
            hx *= arg0->xz_scale;
            dz2 *= arg0->xz_scale;
            dy = y - (obj->y_position + obj->unkD6);
            hx += obj->unkD2;
            dz2 += obj->unkD2;
            hy = obj->unkD4;
            hy += 25.0f;
            if (-hy < dy && dy < hy && -hx < dx && dx < hx && -dz2 < dz && dz < dz2) {
                dist = 0.0f;
            }
        } else {
            dx = x - obj->x_position;
            dz2 = z - obj->z_position;
            if (obj->unkD8 != 0 || obj->unkDA != 0) {
                dx -= func_1505D34C(obj->unk40, obj->unkDA, obj->unkD8, 1.0f, &offZ2);
                dz2 -= offZ2;
            }
            {
                typedef s32 T1505C7D8;

                dy = y - (obj->y_position + obj->unkD6);
                r = obj->unkD2;
                h = obj->unkD4;
                if (h == 0.0f) {
                    continue;
                }
            }
            t = h / (radius + h);
            maxDist = (r + radius) * (r + radius);
            dy *= (r / h) * t + (1.0f - t);
            dist = dx * dx + dy * dy + dz2 * dz2;
        }
        if (dist < maxDist) {
            if ((hit->unk24 & 0x2000) && i != arg0->unk13D) {
                if (*(Vec1505C7D8 **)((u8 *)arg0 + 0x2F4) != NULL) {
                    pos = **(Vec1505C7D8 **)((u8 *)arg0 + 0x2F4);
                } else {
                    func_1507C3E0(arg0, &sp_BA, 0, 0);
                    pos.x = arg0->x_position;
                    pos.y = arg0->y_position + (sp_BA >> 1);
                    pos.z = arg0->z_position;
                }
                rx = obj->x_position;
                if (obj->unk31C != NULL) {
                    height = *(s16 *)((u8 *)obj->unk31C + 0x114) * 0.5f;
                } else {
                    height = 20.0f;
                }
                t = height;
                t += obj->y_position;
                rz = obj->z_position;
                d[0] = rx - pos.x;
                d[1] = t - pos.y;
                d[2] = rz - pos.z;
                if (func_150AC9C0(pos.x, pos.y, pos.z, d[0], d[1], d[2], NULL, NULL, &sp_DC, &sp_E0, &sp_E4, &hitDist, 0, 0, 0.0f) != 0 &&
                    hitDist < func_15143E64(d)) {
                    continue;
                }
            }
            D_800CC2C0 = x;
            D_800CC2C4 = y;
            D_800CC2C8 = z;
            ret = func_1505C1E4(arg0, obj, hits, idx, i, ret, 0xF);
            idx = arg0->unkD0 - 1;
        }
    }
    if (ret != 0) {
        hit = (Hit1505C7D8 *)((u8 *)hits + idx * sizeof(Hit1505C7D8));
        if (((s32 (*)[0x32C / 4])&D_800B2CD0)[arg0->pad108][0] == 1) {
            if (arg0->unk254 != 0) {
                arg0->unk232 = arg0->unk254;
                arg0->unk218 = NULL;
            }
        }
        if (!(hit->unk24 & 4)) {
            arg0->unk21C = 0;
        }
        if (!(hit->unk24 & 0x800)) {
            arg0->unkD0 = 0;
        }
    }
    return ret;
}

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
typedef struct {
    u16 unk0;
    u8 unk2;
    u8 unk3;
} Game83300AnimSub;

typedef struct {
    u8 unk0;
    u8 pad1[3];
    s32 unk4;
    u8 unk8;
    u8 pad9;
    Game83300AnimSub sub[3];
    u8 pad16[2];
} Game83300AnimEntry; /* size 0x18 */

s32 func_1506E46C(struct127 *arg0, s32 *arg1, s32 arg2);
void func_15060A30(s32 arg0, struct127 *arg1);
void func_15060B04(s32 arg0, struct127 *arg1, s32 arg2);
void func_1507CD64(struct127 *arg0, s32 arg1);
void func_1517F488(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void func_1505E874(s32 arg0, struct127 *arg1) {
    s32 *ptr;
    Game83300AnimEntry *table;
    f32 scale;
    f32 speed;
    s32 i;
    u16 anim;
    u16 pad58;


    speed = 6.0f;
    if (arg1->id == 0xFF) {
        return;
    }
    anim = arg1->unk106;
    ptr = (s32 *)D_800D1588[arg1->id];
    table = (Game83300AnimEntry *)ptr[-2];
    if (table == NULL) {
        return;
    }
    if (arg1->unk105 == 0) {
        arg1->unk10F = table[anim].unk8;
        arg1->disable_jump = 0;
        if (arg1->unk31C != NULL) {
            *((u8 *)arg1->unk31C + 0x26) = 0;
        }
        if ((arg1->id == 0x5E) && (arg1->unk13D != 0)) {
            speed = 0.0f;
        }
        if (table[anim].unk4 != 0) {
            s32 sound;
            sound = table[anim].unk4;
            if (func_1506E46C(arg1, &sound, 1) != 0) {
                if (sound != 0x33) {
                    func_15060A30(sound, arg1);
                } else {
                    func_15060B04(sound, arg1, 0x5DC0);
                }
            }
        }
    }
    arg1->unkF4 &= 0xFFF3FEB1;
    if (arg1->unk105 < 3) {
        u8 rate;
        rate = table[anim].sub[arg1->unk105].unk3 & 0xFE;
        arg1->unk10B &= ~1;
        arg1->unk10B |= table[anim].sub[arg1->unk105].unk3 & 1;
        if ((arg1->unk127 != 0xFF) && (arg1->unk105 == 0) && (arg1->unk31C != NULL) &&
            (arg1->camera != NULL) && (*((u8 *)arg1->unk31C + 0x197) != 0)) {
            func_1517F488(0xFF, 0, 0, 0xB4, 0x14, arg1->camera->unk23D);
        }
        if ((arg1->unk127 != 0xFF) && (arg1->health == 0)) {
            i = arg1->unk105;
            while (table[anim].sub[i].unk0 != 0) {
                i++;
            }
            if (i < 2) {
                i = 2;
            }
            if (i == arg1->unk105 + 1) {
                s32 type;
                s32 var_a1;
                type = table[anim].unk0;
                if (arg1->id == 0x9A) {
                    var_a1 = 0xE;
                } else if (arg1->id == 0x53) {
                    var_a1 = 0xA;
                } else if (arg1->in_water != 0) {
                    var_a1 = 4;
                } else if ((((arg1->unk184 & 0x1F) == 0xE) || ((arg1->unk184 & 0x1F) == 0x13)) &&
                           (D_800BE9F0 != 0x34)) {
                    var_a1 = 0xD;
                } else {
                    switch (type) {
                        case 0x20:
                        case 0x21:
                        case 0x25:
                        case 0x26:
                        case 0x33:
                        case 0x34:
                        case 0x48:
                            var_a1 = 0;
                            break;
                        case 0x2D:
                        case 0x2E:
                        case 0x2F:
                            var_a1 = 7;
                            break;
                        case 0x2C:
                            var_a1 = 8;
                            break;
                        case 0x42:
                            var_a1 = 0xC;
                            break;
                        case 0x43:
                            var_a1 = 0xB;
                            break;
                        case 0xE:
                        case 0xF:
                            var_a1 = 0x10;
                            break;
                        case 0x10:
                            *((u8 *)arg1->unk31C + 0x1AC) = 1;
                            D_800D1580 = 0xFF010434;
                            var_a1 = 0;
                            func_1506E5FC();
                            D_800D1580 = 0xFF070169;
                            func_1506E8D8();
                            break;
                        default:
                            var_a1 = 1;
                            break;
                    }
                }
                func_1507CD64(arg1, var_a1);
                arg1->unk105 = 3;
                return;
            }
        }
        if (rate == 0xC8) {
            arg1->unkF4 |= 8;
            arg1->unk10B |= 1;
        }
        arg1->unk10C = rate * 100;
        scale = (u32)table[anim].sub[arg1->unk105].unk2;
        scale = scale / 100.0f;
        anim = table[anim].sub[arg1->unk105].unk0;
        if (anim != 0) {
            arg1->unk105++;
        }
        {
            s32 pad;

            if (scale == 0.0f) {
                scale = 1.0f;
            }
        }
    } else {
        arg1->unk10C = 20000;
        scale = 1.0f;
        anim = 0;
    }
    if (anim == 0) {
        arg1->unkF4 &= ~8;
        arg1->stunned = 0;
        arg1->unk10B = 0;
        arg1->unk10F = 0;
        if (arg1->interaction_state == 1) {
            *((u8 *)arg1->unk31C + 0x58) = 0;
            arg1->unkF8 &= 0xFEFFFDFF;
        }
    } else {
        func_1505E650(arg1, anim, scale, speed, 0.0f, 0.0f, 0);
    }
}
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
extern s32 func_1514D310(struct127 *);
extern void func_151695F0(struct127 *, s32);
extern void func_15084558(struct127 *);
extern void func_1504AF10(struct127 *, s32, s32);
extern void func_1503E260(s32);
extern s32 func_150303E4(struct127 *);
extern void func_15188AD0(struct127 *);
extern void func_10004074(void *);
extern u8 *D_800D210C;
void func_150626EC(struct127 *arg0, s32 arg1);

void func_15060F28(struct127 *arg0, s32 arg1) {
    func_1514D310(arg0);
    func_151695F0(arg0, 0);
    if ((arg0->unk5 != 2) && (arg0->unk5 != 3)) {
        func_15084558(arg0);
        func_150626EC(arg0, arg1);
    }
    if (arg1 == 1) {
        func_1504AF10(arg0, 1, 0);
        func_1503E260(arg0 - D_800CC2D0);
    }
    if (arg0->unk13F != 0xFF) {
        if (arg1 != 2) {
            if ((*(s32 *)((u8 *)arg0 + 0x144)) != 0) {
                if (((struct129 *)(*(s32 *)((u8 *)arg0 + 0x144)))->unk0 & 0x20) {
                    ((struct129 *)(*(s32 *)((u8 *)arg0 + 0x144)))->unk2 = 1;
                    D_800D210C[arg0->unk13F] = 0;
                } else if (((struct129 *)(*(s32 *)((u8 *)arg0 + 0x144)))->unk0 & 1) {
                    D_800D210C[arg0->unk13F] = 0;
                } else {
                    s32 pad;
                    ((struct129 *)(*(s32 *)((u8 *)arg0 + 0x144)))->unk2 = 1;
                    D_800D210C[arg0->unk13F] &= 0x80;
                }
            } else {
                D_800D210C[arg0->unk13F] &= 0x80;
            }
        } else {
            D_800D210C[arg0->unk13F] = 0;
        }
    }
    if (*(void **)((u8 *)arg0 + 0x260) != NULL) func_10004074(*(void **)((u8 *)arg0 + 0x260));
    if (*(s32 *)((u8 *)arg0 + 0x264) != 0) func_10004074((void *)*(s32 *)((u8 *)arg0 + 0x264));
    if (*(s32 *)((u8 *)arg0 + 0x268) != 0) func_10004074((void *)*(s32 *)((u8 *)arg0 + 0x268));
    if (*(s32 *)((u8 *)arg0 + 0x26C) != 0) func_10004074((void *)*(s32 *)((u8 *)arg0 + 0x26C));
    if (*(void **)((u8 *)arg0 + 0x1D8) != NULL) func_10004074(*(void **)((u8 *)arg0 + 0x1D8));
    if (*(struct102 **)((u8 *)arg0 + 0x2D4) != NULL) func_1516972C(*(struct102 **)((u8 *)arg0 + 0x2D4));
    if (*(s32 **)((u8 *)arg0 + 0x304) != NULL) {
        s32 i;
        for (i = 0; i <= D_80082FA0; i = (u8)(i + 1)) {
            func_100043B4(*(s32 **)((u8 *)arg0 + (i * 4) + 0x304), 4);
        }
    }
    if (*(s32 *)((u8 *)arg0 + 0x314) != 0) func_100043B4((s32 *)*(s32 *)((u8 *)arg0 + 0x314), 4);
    if (arg0->unk2D0 != NULL) func_10004074(arg0->unk2D0);
    if (*(void **)((u8 *)arg0 + 0x324) != NULL) func_10004074(*(void **)((u8 *)arg0 + 0x324));
    if ((arg0->unk31C != NULL) && (*(void **)((u8 *)arg0->unk31C + 0x11C) != NULL)) {
        func_10004074(*(void **)((u8 *)arg0->unk31C + 0x11C));
    }
    func_10010AA8(arg0);
    func_150303E4(arg0);
    func_15060D54((Game83300ActorLink *)arg0);
    func_15188AD0(arg0);
    if (arg0->unk31C != NULL) func_10004074((void *)(s32)arg0->unk31C);
    func_1505F188(arg0);
}
typedef struct PathPt150611E8 {
    s16 x;
    s16 y;
    s16 z;
    u16 speed;
} PathPt150611E8;

void func_150611E8(struct127 *arg0, s32 arg1) {
    s32 count;
    s32 closest;
    s32 i;
    s32 best;
    s16 x;
    s16 z;
    PathPt150611E8 *pts;
    f32 speed;
    f32 scale;
    s32 dx;
    s32 dz;
    u16 angle;
    s32 dist;

    count = D_800D2108[arg1];
    if (count < 3) {
        return;
    }
    x = arg0->x_position;
    z = arg0->z_position;
    best = 0x7FFFFFFF;
    for (i = 0; i < count; i++) {
        dx = x - ((PathPt150611E8 **)D_800D2104)[arg1][i].x;
        dz = z - ((PathPt150611E8 **)D_800D2104)[arg1][i].z;
        if (dx * dx + dz * dz < best) {
            closest = i;
            best = dx * dx + dz * dz;
        }
    }
    if (closest != 0 && closest != count - 1) {
        pts = ((PathPt150611E8 **)D_800D2104)[arg1];
        if (best > 28000) {
            dx = x - pts[closest + 1].x;
            dz = z - pts[closest + 1].z;
            dist = dx * dx + dz * dz;
            dx = x - pts[closest - 1].x;
            dz = z - pts[closest - 1].z;
            if (dx * dx + dz * dz < dist) {
                closest--;
            }
        }
        if (pts[closest + 1].speed == 0) {
            speed = 8.0f;
        } else {
            speed = (u32)pts[closest + 1].speed;
        }
        scale = speed * 0.0025f;
        if (scale > 1.0f) {
            scale = 1.0f;
        }
        angle = func_1505A630(pts[closest + 1].x - pts[closest].x, pts[closest].z - pts[closest + 1].z, 0);
        if (arg0->unk31C != NULL && speed > 20.0f) {
            *(s16 *)((u8 *)arg0->unk31C + 0x68) = angle | 1;
        }
        func_150593C4(arg0, angle, speed, scale);
    }
}

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

s32 func_150623F4(struct127 *arg0);
void func_150619A8(void) {
    s32 i;
    s32 count;
    u8 list[25];
    s32 j;
    struct127 *obj;
    struct127 *other;
    s32 idx;

    count = 0;
    for (i = 0; i < 25; i++) {
        obj = &D_800CC2D0[i];
        if (obj->interaction_state != 0) {
            if (obj->pad2FC[1] != 0) {
                obj->pad2FC[1]--;
            }
            func_150617BC((u8 *)obj);
            if (func_150623F4(obj) != 0) {
                obj->unk2FA |= 1;
            } else {
                obj->unk2FA &= ~1;
                if (((u8 *)obj)[0x20B] != 0 || ((u8 *)obj)[0x20C] != 0 || ((u8 *)obj)[0x20D] != 0 || ((u8 *)obj)[0x20E] != 0) {
                    list[count] = i;
                    count++;
                }
            }
        }
    }
    if (count != 0) {
        for (i = 0; i < count; i++) {
            idx = list[i];
            obj = &D_800CC2D0[idx];
            for (j = 0; j < 4; j++) {
                if (((u8 *)obj)[0x20B + j] != 0) {
                    other = func_15083E90(((u8 *)obj)[0x20B + j]);
                    if (other != NULL && (other->unk2FA & 1)) {
                        obj->unk2FA |= 1;
                        break;
                    }
                }
            }
        }
    }
}

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

void func_15033EC4(s32 arg0, s32 arg1);

#define OBJ_INDEX(obj) ((u32)((obj) - D_800CC2D0))

void func_15062800(struct127 *arg0, struct127 *arg1) {
    s32 idx0;
    s32 cam;
    struct127 tmp;
    struct129 *t;
    s32 i;
    s32 u;

    func_15146508(arg0, arg1);
    bcopy(arg1, &tmp, sizeof(struct127));
    bcopy(arg0, arg1, sizeof(struct127));
    bcopy(&tmp, arg0, sizeof(struct127));
    idx0 = OBJ_INDEX(arg0);
    cam = (s32)&D_800DBFF0[OBJ_INDEX(arg1)];
    arg1->unk127 = OBJ_INDEX(arg1);
    arg1->unique_id = OBJ_INDEX(arg1) + 1;
    arg1->unk13F = OBJ_INDEX(arg1);
    arg1->camera = &D_800DBFF0[OBJ_INDEX(arg1)];
    arg0->unk127 = OBJ_INDEX(arg0);
    arg0->unique_id = OBJ_INDEX(arg0) + 1;
    arg0->unk13F = OBJ_INDEX(arg0);
    arg0->camera = NULL;
    u = (s32)arg0->unk144;
    arg0->unk144 = arg1->unk144;
    arg1->unk144 = (struct129 *)u;
    D_800DBFF0[OBJ_INDEX(arg1)].unk3D4 = (struct127 *) arg1->unk31C;
    arg1->unk31C->pad196[1] = 0;
    func_15033EC4(arg1->unique_id, arg0->unique_id);
    if (*(struct102 **)arg1->pad2D4 != NULL) {
        func_1516972C(*(struct102 **)arg1->pad2D4);
        *(struct102 **)arg1->pad2D4 = NULL;
    }
    if (*(struct102 **)arg0->pad2D4 != NULL) {
        func_1516972C(*(struct102 **)arg0->pad2D4);
        *(struct102 **)arg0->pad2D4 = NULL;
    }
    func_150615DC(arg1);
    for (i = 0; i < 25; i++) {
        if (i == OBJ_INDEX(arg1) || i == OBJ_INDEX(arg0)) {
            continue;
        }
        if (D_800CC2D0[i].interaction_state == 0) {
            continue;
        }
        u = D_800CC2D0[i].unk65;
        if (u != 0) {
            u--;
            if (u == OBJ_INDEX(arg1)) {
                D_800CC2D0[i].unk65 = OBJ_INDEX(arg0) + 1;
            } else if (u == OBJ_INDEX(arg0)) {
                D_800CC2D0[i].unk65 = OBJ_INDEX(arg1) + 1;
            }
        }
    }
    if (arg1->unk13C >= 100) {
        u8 x = arg1->unk13C - 100;
        D_800CC2D0[x].unk13D = OBJ_INDEX(arg1) + 100;
    }
    func_150627D4(arg1);
}

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

typedef struct { s32 unk0; s32 unk4; } Entry62D10;
extern Entry62D10 **D_800C4488[];
extern s32 D_800D19A0;
void func_15062D10(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    Entry62D10 *base, *entry; s32 packed, modX, modY, newX, newY, tmp, flag;
    newX = 0; modX = 0; modY = 0;
    if ((&D_800D19A0)[arg0] == 0) return;
    base = D_800C4488[arg0][arg4];
    packed = ((Entry62D10 *)((u8 *)base + arg1 * 8))->unk4;
    modX = ((packed >> 12) & 0xFFF) + 2;
    modY = (packed & 0xFFF) + 2;
    entry = ((Entry62D10 *)((u8 *)base + arg1 * 8));
    packed = entry->unk0;
    flag = arg5;
    if (flag != 0) newX = arg2 & 0xFFF;
    else { tmp = ((packed >> 12) & 0xFFF) + arg2; newX = tmp;
           if (modX < tmp) newX = tmp - modX; else if (tmp < 0) newX = tmp + modX; }
    if (flag != 0) newY = arg3 & 0xFFF;
    else { tmp = (packed & 0xFFF) + arg3; newY = tmp;
           if (modY < tmp) newY = tmp - modY; else if (tmp < 0) newY = tmp + modY; }
    packed = (packed & 0xFF000000) | (newY & 0xFFF) | ((newX & 0xFFF) << 12);
    entry->unk0 = packed;
}
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
