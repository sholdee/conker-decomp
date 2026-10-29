#include <ultra64.h>
#include "functions.h"
#include "variables.h"


s32 func_15167A68(s32, s32, s32, s32, s32, s32);
extern void (*D_8008CA20[])(struct102 *);

s32 func_15169900(s32 arg0, s32 arg1) {
    s32 ret;

    ret = func_15167A68(0x5E, 0, 0x4C, 0, arg1, 1);
    if (ret != 0) {
        bcopy((void *)arg0, (void *)(ret + 0x10), 0x3C);
    }
    return ret;
}

s32 func_15169968(s32 arg0) {
    return func_15169900(arg0, 0xFF);
}

void func_15169988(struct102 *arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_t1;

    temp_v0 = *(s8 *)((u8 *)arg0 + 0x40);
    if (temp_v0 != 0) {
        D_8008CA20[temp_v0](arg0);
    }

    temp_v0 = *(s16 *)((u8 *)arg0 + 0x26);
    temp_v0 += *(s16 *)((u8 *)arg0 + 0x28) * D_800BE9E4;
    temp_t1 = *(u8 *)((u8 *)arg0 + 0x41) << 8;
    if (temp_v0 >= temp_t1) {
        temp_v0 -= temp_t1;
    } else if (temp_v0 < 0) {
        temp_v0 += temp_t1;
    }

    temp_v1 = *(s16 *)((u8 *)arg0 + 0x24);
    *(s16 *)((u8 *)arg0 + 0x26) = temp_v0;
    if (temp_v1 != 0) {
        temp_v1 -= D_800BE9E4;
        if (temp_v1 <= 0) {
            func_1516972C(arg0);
            return;
        }
        *(s16 *)((u8 *)arg0 + 0x24) = temp_v1;
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
} Struct15169A48_mode;
extern Struct15169A48_mode D_800A4AC8[];
extern s32 D_800D2C9C;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0xE];
    s16 unk26;
    u8 pad28[0x4];
    f32 unk2C;
    f32 unk30;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    u16 unk3C;
    u16 unk3E;
    u8 pad40[2];
    u8 unk42;
    u8 unk43;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 unk48;
    u8 unk49;
} Struct15169A48;

Gfx *func_15094F70(Gfx *, s32, s32, s32, s32, s32, s32, s32, s32);
Gfx *func_1509629C(Gfx *, s32, f32, f32, f32, f32, s32, s32, s32, s32, s32, s32);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);

Gfx *func_15169A48(Gfx *gfx, Struct15169A48 *arg1, s32 arg2) {
    s32 pad;
    s32 flags;
    u8 update;
    s32 shift;
    s32 mode;
    s32 pad2;

    if (arg1->unk38 == 0 || arg1->unk3A == 0) {
        return gfx;
    }
    update = 1;
    gfx = func_1513F4E4(gfx, arg1->unk47, &update);
    gDPSetPrimColor(gfx++, 1, 0, arg1->unk42, arg1->unk43, arg1->unk44, arg1->unk45);
    if (arg1->unk49 & 1) {
        if (arg1->unk49 & 2) {
            shift = (arg1->unk34 * 4) >> 5;
        } else {
            shift = 0;
        }
        gfx = func_15094F70(gfx, arg1->unk14, 0, 0, 0x100, 1, shift, 2, 3);
    }
    if (arg1->unk49 & 0x10) {
        flags = 1;
    } else {
        flags = 0;
    }
    if (arg1->unk49 & 4) {
        flags |= 2;
    }
    if (arg1->unk49 & 8) {
        flags |= 4;
    }
    if (arg1->unk49 & 0x20) {
        gDPSetPrimDepth(gfx++, 0x795A, 0);
    }
    if (arg1->unk48 == 2) {
        mode = 0x100000;
    } else {
        mode = 0;
    }
    gfx = func_15142FBC(gfx, mode | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE | G_CYC_1CYCLE, D_800A4AC8[arg1->unk46].unk0 | D_800A4AC8[arg1->unk46].unk4 | 4, &update);
    return func_1509629C(gfx, arg1->unk10, arg1->unk38, arg1->unk3A, arg1->unk2C, arg1->unk30,
                         arg1->unk3C, arg1->unk3E, flags, arg1->unk34, arg1->unk36, arg1->unk26 >> 8);
}
