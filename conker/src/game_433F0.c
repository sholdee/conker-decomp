#include <ultra64.h>
#include "functions.h"
#include "variables.h"


s8 func_151E5FAC(void);

void func_15015F40(s16 arg0, s32 arg1) {
    switch (arg1) {
        case 0x1A:
        case 0x24:
        case 0x2B:
        case 0x2D:
        case 0x30:
        case 0x33:
        case 0x34:
        case 0x3F:
            D_800BE616 = 1;
            D_800BE9E8 = func_151E5FAC() - 1;
            break;
        default:
            D_800BE616 = 0;
            D_800BE9E8 = arg0 - 1;
            break;
    }
}

extern s16 D_800BE668[20][2];

void func_15015FBC(s32 arg0, s32 arg1) {
    s32 i;
    s32 w2, h2, h4, w4;

    for (i = 0; i < 5; i++) {
        (&D_800BE638)[i] = arg0;
        D_800BE650[i] = arg1;
        if (i < 4) {
            if (i >= 2) {
                (&D_800BE638)[i] >>= 1;
            }
            if (i >= 1) {
                D_800BE650[i] >>= 1;
            }
        }
    }
    w2 = arg0 >> 1;
    w4 = arg0 >> 2;
    h2 = arg1 >> 1;
    h4 = arg1 >> 2;
    D_800BE668[0][0] = w2;
    D_800BE668[0][1] = h2;
    D_800BE668[1][0] = w2;
    D_800BE668[1][1] = h2;
    D_800BE668[2][0] = w2;
    D_800BE668[2][1] = h2;
    D_800BE668[3][0] = w2;
    D_800BE668[3][1] = h2;
    D_800BE668[4][0] = w2;
    D_800BE668[4][1] = h2;
    D_800BE668[5][0] = w2;
    D_800BE668[5][1] = h4;
    D_800BE668[6][0] = w2;
    D_800BE668[6][1] = h4 * 3;
    D_800BE668[7][0] = w2;
    D_800BE668[7][1] = h2;
    D_800BE668[8][0] = 0;
    D_800BE668[8][1] = 0;
    D_800BE668[9][0] = 0;
    D_800BE668[9][1] = 0;
    D_800BE668[10][0] = w4;
    D_800BE668[10][1] = h4;
    D_800BE668[11][0] = w4 * 3;
    D_800BE668[11][1] = h4;
    D_800BE668[12][0] = w2;
    D_800BE668[12][1] = h4 * 3;
    D_800BE668[13][0] = w2;
    D_800BE668[13][1] = h2;
    D_800BE668[14][0] = 0;
    D_800BE668[14][1] = 0;
    D_800BE668[15][0] = w4;
    D_800BE668[15][1] = h4;
    D_800BE668[16][0] = w4 * 3;
    D_800BE668[16][1] = h4;
    D_800BE668[17][0] = w4;
    D_800BE668[17][1] = h4 * 3;
    D_800BE668[18][0] = w4 * 3;
    D_800BE668[18][1] = h4 * 3;
    D_800BE668[19][0] = w2;
    D_800BE668[19][1] = h2;
}
// s32 func_15015FBC(s32 arg0, s32 arg1) {
//     s16 temp_lo;
//     s16 temp_lo_2;
//     s32 temp_a1_2;
//     s32 temp_a3;
//     s32 temp_v0;
//     s32 temp_v1;
//     u32 temp_a1;
//     void *phi_v1;
//     u32 phi_a1;
//
//     D_800BE638 = arg0;
//     D_800BE650 = arg1;
//     phi_v1 = D_800BE63C;
//     phi_a1 = D_800BE654;
// loop_1:
//     phi_v1->unk0 = arg0;
//     phi_a1->unk0 = arg1;
//     if (phi_a1 < D_800BE660) {
//         if (phi_a1 >= D_800BE658) {
//             phi_v1->unk0 = (s32) (arg0 >> 1);
//         }
//         if (phi_a1 >= D_800BE654) {
//             phi_a1->unk0 = (s32) ((s32) phi_a1->unk0 >> 1);
//         }
//     }
//     phi_v1->unk4 = arg0;
//     phi_a1->unk4 = arg1;
//     if (phi_a1 < D_800BE65C) {
//         if (phi_a1 >= D_800BE654) {
//             phi_v1->unk4 = (s32) (arg0 >> 1);
//         }
//         if (phi_a1 >= D_800BE650) {
//             phi_a1->unk4 = (s32) ((s32) phi_a1->unk4 >> 1);
//         }
//     }
//     temp_a1 = phi_a1 + 8;
//     phi_v1 = phi_v1 + 8;
//     phi_a1 = temp_a1;
//     if (temp_a1 != D_800BE664) {
//         goto loop_1;
//     }
//     temp_a3 = arg1 >> 2;
//     temp_lo = temp_a3 * 3;
//     temp_v1 = arg0 >> 2;
//     temp_v0 = arg0 >> 1;
//     temp_a1_2 = arg1 >> 1;
//     D_800BE668->unk0 = (s16) temp_v0;
//     D_800BE668->unk2 = (s16) temp_a1_2;
//     D_800BE668->unk4 = (s16) temp_v0;
//     D_800BE668->unk6 = (s16) temp_a1_2;
//     D_800BE668->unk8 = (s16) temp_v0;
//     D_800BE668->unkA = (s16) temp_a1_2;
//     D_800BE668->unkC = (s16) temp_v0;
//     D_800BE668->unkE = (s16) temp_a1_2;
//     temp_lo_2 = temp_v1 * 3;
//     D_800BE668->unk10 = (s16) temp_v0;
//     D_800BE668->unk12 = (s16) temp_a1_2;
//     D_800BE668->unk14 = (s16) temp_v0;
//     D_800BE668->unk16 = (s16) temp_a3;
//     D_800BE668->unk18 = (s16) temp_v0;
//     D_800BE668->unk1A = temp_lo;
//     D_800BE668->unk1C = (s16) temp_v0;
//     D_800BE668->unk1E = (s16) temp_a1_2;
//     D_800BE668->unk20 = (u16)0;
//     D_800BE668->unk22 = (u16)0;
//     D_800BE668->unk24 = (u16)0;
//     D_800BE668->unk26 = (u16)0;
//     D_800BE668->unk28 = (s16) temp_v1;
//     D_800BE668->unk2A = (s16) temp_a3;
//     D_800BE668->unk2C = temp_lo_2;
//     D_800BE668->unk2E = (s16) temp_a3;
//     D_800BE668->unk30 = (s16) temp_v0;
//     D_800BE668->unk32 = temp_lo;
//     D_800BE668->unk34 = (s16) temp_v0;
//     D_800BE668->unk36 = (s16) temp_a1_2;
//     D_800BE668->unk38 = (u16)0;
//     D_800BE668->unk3A = (u16)0;
//     D_800BE668->unk3C = (s16) temp_v1;
//     D_800BE668->unk3E = (s16) temp_a3;
//     D_800BE668->unk40 = temp_lo_2;
//     D_800BE668->unk42 = (s16) temp_a3;
//     D_800BE668->unk44 = (s16) temp_v1;
//     D_800BE668->unk46 = temp_lo;
//     D_800BE668->unk48 = temp_lo_2;
//     D_800BE668->unk4A = temp_lo;
//     D_800BE668->unk4C = (s16) temp_v0;
//     D_800BE668->unk4E = (s16) temp_a1_2;
//     return temp_v0;
// }
