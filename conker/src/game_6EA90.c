#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern u8 D_80084930;
extern u8 D_800848D0[];
extern u8 D_80085930;
extern u8 D_80085931;
extern u8 D_80085932;
extern u8 D_80085933[];

#define WGFX(pkt, a, b)             \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}
Gfx *func_150412C0(Gfx *gfx);

s32 func_15041480(u8 arg0);
Gfx *func_15041508(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);
Gfx *func_150413FC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
s32 func_15041480(u8 arg0);
Gfx *func_15041508(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);


void func_150415E0(s32 arg0, s32 *arg1, s32 *arg2, f32 *arg3, s32 *arg4, f32 *arg5) {
    if ((arg0 >= 0xA8) && (arg0 < 0x100)) {
        *arg5 = 1.0f;
        switch (arg0) {
            case 0xA8:
            case 0xA9:
                *arg1 = 0x20;
                *arg2 = 0x12;
                *arg3 = 3.1f;
                *arg4 = 1;
                break;
            case 0xAA:
            case 0xAB:
            case 0xAC:
            case 0xAD:
                *arg1 = 0x20;
                *arg2 = 0x1B;
                *arg3 = 2.0f;
                *arg4 = 1;
                *arg5 = 0.603f;
                break;
            case 0xAE:
                *arg1 = 0x10;
                *arg2 = 0x11;
                *arg3 = -1.6f;
                *arg4 = 1;
                break;
            case 0xAF:
            case 0xB0:
                *arg1 = 0x2E;
                *arg2 = 0x10;
                *arg3 = 2.0f;
                *arg4 = 1;
                break;
            case 0xB6:
            case 0xB7:
            case 0xB8:
            case 0xB9:
                *arg1 = 0x10;
                *arg2 = 0xC;
                *arg3 = 2.0f;
                *arg4 = 0;
                break;
            case 0xBA:
                *arg4 = 0;
                *arg3 = 0.2f;
                *arg1 = 0x37;
                *arg2 = 0xC;
                break;
            case 0xB1:
                *arg1 = 0x26;
                *arg2 = 0x18;
                *arg3 = 2.0f;
                *arg4 = 1;
                *arg5 = 0.726f;
                break;
            default:
                *arg1 = 0xC;
                *arg2 = 0x10;
                *arg3 = 2.0f;
                *arg4 = 0;
                break;
        }
    } else {
        *arg3 = 0.0f;
        *arg1 = 0;
        *arg2 = 0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_6EA90/func_150417AC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_6EA90/func_150428D4.s")

s32 func_15042C40(u8 arg0) {
    s32 i;
    u8 c;
    u8 *ptr;
    s32 original;

    original = arg0;
    if ((original >= 0x61) && (original < 0x7B)) {
        c = original - 0x20;
    } else {
        c = original;
    }

    if (original == 0x20) {
        return 0x60;
    }

    i = 3;
    if (D_80085930 == c) {
        return 0;
    }
    if (D_80085931 == c) {
        return 1;
    }
    if (D_80085932 == c) {
        return 2;
    }

    ptr = D_80085933;
    for (i = 3; ; ) {
        if (c == ptr[0]) {
            return i;
        }
        if (c == ptr[1]) {
            return i + 1;
        }
        if (c == ptr[2]) {
            return i + 2;
        }
        if (c == ptr[3]) {
            return i + 3;
        }
        i += 4;
        ptr += 4;
        if (i != 0x5F) {
            continue;
        }
        break;
    }

    return original;
}
