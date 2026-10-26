#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15017790(void) {
    func_1509C120();
    bzero(D_800D2E4C, 0x1B);
    bzero(&D_800D2E50, 0x10);
    bzero(&D_800D2E60, 9);
    bzero(&D_800D2E70, 0xCC);
    D_800D2E44 = (u8)0;
}

void func_150177F8(void) {
    func_1509BA04(1);
    bzero(&D_800D2F48, 0xC);
    func_150A00F0();
    D_800D2DB0 = D_800968C0;
    D_800D2E41 = 0xA;
    D_800D2E42 = 6;
    D_800D3858 = 0;
    D_800D2E44 = 0;
}

void func_15017868(void) {
    u16 *temp_v0;
    s32 i;

    for (i = 0; i < D_800D2F3C; i++) {
        temp_v0 = func_1509B704((s16)D_800D2F40[i]);
        if ((temp_v0 != 0) && ((temp_v0[0] & 0x1000) != 0)) {
            temp_v0[1] = D_800BE9F0;
        }

    }
    func_1509BA04(0);
    func_1509B4A0(*(((s16*)&D_800BE9F0)+1), 1); // er?
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_44C40/func_15017930.s")
typedef struct {
    /* 0x00 */ u64 mask[3];
    /* 0x18 */ u8 count;
    /* 0x1A */ s16 min_x;
    /* 0x1C */ s16 max_x;
    /* 0x1E */ s16 min_y;
    /* 0x20 */ s16 max_y;
    /* 0x22 */ s16 min_z;
    /* 0x24 */ s16 max_z;
} Struct15017B20; /* size 0x28 */

typedef struct {
    /* 0x00 */ u8 pad0[0x15];
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 pad17[0x34 - 0x17];
} Struct15017B20_In; /* size 0x34 */

extern void *allocate_memory(s32, s32, s32, s32);
extern void func_150A0D14(Struct15017B20_In *);
extern void func_15017FA4(Struct15017B20_In *, Struct15017B20 *);
extern Struct15017B20 *D_800D309C;
extern u64 D_800D30A0[3];
extern u64 D_800D30B8[3];

#define OVERLAP(a0, a1, b0, b1) (((a0 <= b0) && (b0 < a1)) || ((b0 < a0) && (a0 < b1)) || ((a0 < b1) && (b1 < a1)))

void func_15017B20(Struct15017B20_In *arg0, s32 arg1) {
    s32 pad[6];
    u64 bit;
    u64 bit2;
    u64 *var_fp;
    u64 *var_s2;
    u64 *var_s0;
    Struct15017B20_In *p;
    Struct15017B20 *a;
    Struct15017B20 *b;
    s32 i;
    s32 j;

    D_800D309C = allocate_memory(arg1 * sizeof(Struct15017B20), 1, 1, 0);
    for (i = 0; i < arg1; i++) {
        func_150A0D14(&arg0[i]);
        func_15017FA4(&arg0[i], &D_800D309C[i]);
    }
    bzero(D_800D30A0, sizeof(D_800D30A0));
    for (i = 0; i < arg1; i++) {
        if (((arg0[i].unk15 & 3) == 3) || (arg0[i].unk16 & 8)) {
            if (!(arg0[i].unk16 & 4)) {
                D_800D30A0[i >> 6] |= (u64)1 << (i & 0x3F);
            }
        }
    }
    bzero(D_800D30B8, sizeof(D_800D30B8));
    for (i = 0; i < arg1; i++) {
        if ((arg0[i].unk15 & 3) != 3) {
            if (!(arg0[i].unk16 & 8) && !(arg0[i].unk16 & 4)) {
                D_800D30B8[i >> 6] |= (u64)1 << (i & 0x3F);
            }
        }
    }
    var_fp = D_800D30B8;
    bit = 1;
    for (i = 0; i < arg1; i++) {
        var_s2 = D_800D30B8;
        D_800D309C[i].count = 0;
        if (*var_fp & bit) {
            bit2 = 1;
            var_s0 = (u64 *)&D_800D309C[i];
            bzero(D_800D309C[i].mask, sizeof(D_800D309C[i].mask));
            for (j = 0; j < arg1; j++) {
                if (*var_s2 & bit2) {
                    if (OVERLAP(D_800D309C[i].min_x, D_800D309C[i].max_x, D_800D309C[j].min_x, D_800D309C[j].max_x) &&
                        OVERLAP(D_800D309C[i].min_y, D_800D309C[i].max_y, D_800D309C[j].min_y, D_800D309C[j].max_y) &&
                        OVERLAP(D_800D309C[i].min_z, D_800D309C[i].max_z, D_800D309C[j].min_z, D_800D309C[j].max_z)) {
                        *var_s0 |= bit2;
                        D_800D309C[i].count++;
                    }
                }
                bit2 <<= 1;
                if (bit2 == 0) {
                    var_s2++;
                    var_s0++;
                    bit2 = 1;
                }
            }
        }
        bit <<= 1;
        if (bit == 0) {
            var_fp++;
            bit = 1;
        }
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_44C40/func_15017FA4.s")
