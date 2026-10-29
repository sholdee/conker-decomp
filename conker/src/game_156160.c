#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct Entry151298C0 {
    s16 unk0;
    s16 unk2;
    f32 unk4;
    f32 unk8;
    u8 padC[0x18];
};

extern f32 D_800A3610;
extern struct Entry151298C0 D_800DC028[];

#pragma GLOBAL_ASM("asm/nonmatchings/game_156160/func_15128CB0.s")

void func_151298C0(void *arg0, s32 arg1)
{
    struct Entry151298C0 *entries;

    if (D_80089550 != 0) {
        entries = D_800DC028;
        entries[*(volatile u8 *)((u8 *)arg0 + 0x23D)].unk2 = 0;
        entries[*(volatile u8 *)((u8 *)arg0 + 0x23D)].unk4 = -1.0f;
        entries[*(volatile u8 *)((u8 *)arg0 + 0x23D)].unk8 = D_800A3610;
    }
}

void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
s32 func_150A3FC4(f32, f32, s32, void *, void *);
void func_1510E7A4(s32 a0, s32 a1, void *a2, void *a3, void *arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, u16 arg10, s32 arg11, f32 arg12, f32 arg13);
void func_151236D0(void *);
void func_1512C150(void *);

typedef struct {
    /* 0x000 */ u8 pad0[0x84];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ u8 pad88[0x1B4];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 pad23D[0x3B];
    /* 0x278 */ f32 unk278;
    /* 0x27C */ f32 unk27C;
    /* 0x280 */ f32 unk280;
    /* 0x284 */ f32 unk284;
    /* 0x288 */ f32 unk288;
    /* 0x28C */ f32 unk28C;
    /* 0x290 */ f32 unk290;
    /* 0x294 */ u8 pad294[0x28];
    /* 0x2BC */ f32 unk2BC;
    /* 0x2C0 */ f32 unk2C0;
    /* 0x2C4 */ f32 unk2C4;
    /* 0x2C8 */ u8 pad2C8[0x30];
    /* 0x2F8 */ f32 unk2F8;
    /* 0x2FC */ f32 unk2FC;
    /* 0x300 */ f32 unk300;
    /* 0x304 */ f32 unk304;
    /* 0x308 */ f32 unk308;
    /* 0x30C */ f32 unk30C;
    /* 0x310 */ u8 pad310[0x4C];
    /* 0x35C */ f32 unk35C;
    /* 0x360 */ f32 unk360;
    /* 0x364 */ f32 unk364;
    /* 0x368 */ u8 pad368[0x58];
    /* 0x3C0 */ f32 unk3C0;
    /* 0x3C4 */ f32 unk3C4;
    /* 0x3C8 */ f32 unk3C8;
    /* 0x3CC */ u8 pad3CC[0x274];
    /* 0x640 */ s32 unk640;
    /* 0x644 */ s32 unk644;
    /* 0x648 */ s32 unk648;
    /* 0x64C */ u8 pad64C[0xB8];
    /* 0x704 */ s32 unk704;
    /* 0x708 */ u8 pad708[0x34];
    /* 0x73C */ s16 unk73C;
    /* 0x73E */ u8 pad73E[0x76];
    /* 0x7B4 */ f32 unk7B4;
    /* 0x7B8 */ u8 pad7B8[0x1AC];
    /* 0x964 */ f32 unk964;
    /* 0x968 */ f32 unk968;
    /* 0x96C */ f32 unk96C;
    /* 0x970 */ f32 unk970;
    /* 0x974 */ f32 unk974;
    /* 0x978 */ f32 unk978;
    /* 0x97C */ f32 unk97C;
    /* 0x980 */ f32 unk980;
} Obj15129934;

void func_15129934(Obj15129934 *arg0) {
    f32 x;
    struct17 sp68;
    f32 z;
    f32 y;
    f32 dist;
    f32 sp58;
    f32 dx;
    f32 dy;
    f32 dz;

    if ((D_80089550 != 0) && (arg0->unk73C == 0)) {
        arg0->unk27C = arg0->unk288;
        arg0->unk280 = arg0->unk28C;
        arg0->unk284 = arg0->unk290;
        func_15048F90((struct17 *)&arg0->unk27C, (struct17 *)&arg0->unk2BC, &sp68);
        func_1504917C(&sp68, &sp68);
        dx = arg0->unk2BC - arg0->unk27C;
        dy = arg0->unk2C0 - arg0->unk280;
        dz = arg0->unk2C4 - arg0->unk284;
        dist = sqrtf((dx * dx) + (dy * dy) + (dz * dz)) - arg0->unk278;
        if (dist < 0.0f) {
            dist = 0.0f;
        }
        func_15049148(&sp68, dist, &sp68);
        x = arg0->unk27C + sp68.unk0;
        if (arg0->unk704 & 0xF) {
            y = arg0->unk280;
        } else {
            y = arg0->unk280 + (sp68.unk4 * 1.200000048f);
        }
        z = arg0->unk284 + sp68.unk8;
        if (arg0->unk23C != 0) {
            arg0->unk2F8 = x;
            arg0->unk304 = x;
            arg0->unk3C0 = 0;
        } else {
            func_150495B0(&arg0->unk2F8, x, &arg0->unk3C0, 1.0f, 3.0f, arg0->unk7B4);
        }
        if (arg0->unk23C != 0) {
            arg0->unk2FC = y;
            arg0->unk308 = y;
            arg0->unk3C4 = 0;
        } else {
            func_150495B0(&arg0->unk2FC, y, &arg0->unk3C4, 1.0f, 2.0f, arg0->unk7B4);
        }
        if (arg0->unk23C != 0) {
            arg0->unk300 = z;
            arg0->unk30C = z;
            arg0->unk3C8 = 0;
        } else {
            func_150495B0(&arg0->unk300, z, &arg0->unk3C8, 1.0f, 3.0f, arg0->unk7B4);
        }
        func_1512C150(arg0);
        if ((arg0->unk23C != 0) || (arg0->unk84 & 0x400000) ||
            (func_150A3FC4(arg0->unk2F8, arg0->unk300, arg0->unk644, &arg0->unk648, &arg0->unk35C) == 0)) {
            func_1510E7A4(0, 0, &sp58, &arg0->unk360, &arg0->unk640, 0, arg0->unk2F8, arg0->unk2FC, arg0->unk300, arg0->unk2FC, 0, 0, -10000.0f, arg0->unk2FC);
            if (-10000.0f != arg0->unk364) {
                arg0->unk360 = arg0->unk364;
            }
            func_151236D0(arg0);
        }
        arg0->unk974 = 1.0f;
        arg0->unk964 = 1.0f;
        arg0->unk97C = 1.0f;
        arg0->unk96C = 1.0f;
        arg0->unk978 = 2.0f;
        arg0->unk968 = 2.0f;
        arg0->unk980 = 2.0f;
        arg0->unk970 = 2.0f;
    }
}
