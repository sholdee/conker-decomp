#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_15BD90/func_1512E8E0.s")

void func_1512FE00(struct108 *arg0, s32 arg1) {
    switch (arg1) {
    case 1:
        func_15123934(arg0, arg0->unk2C, 0, 0, 7);
        break;
    case 2:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 &= ~8;
        break;
    case 3:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x10000000;
        break;
    case 8:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x80;
        break;
    case 9:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 &= ~8;
        arg0->unk84 |= 0x80;
        break;
    case 11:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x4000;
        break;
    case 12:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x8000;
        break;
    case 13:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x20000;
        break;
    case 16:
        func_15123934(arg0, 8, 0, arg0->unk134, 7);
        break;
    case 14:
        func_15123934(arg0, 2, 0, arg0->unk134, 7);
        break;
    case 18:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x100000;
        break;
    case 19:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x400000;
        break;
    case 21:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk190 = 0.0f;
        break;
    case 22:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x800;
        arg0->unk190 = -30.0f;
        break;
    case 25:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x40;
        break;
    case 23:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x1000000;
        break;
    case 24:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x80000000;
        break;
    case 26:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x10;
        break;
    case 27:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x20;
        break;
    case 28:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x10;
        arg0->unk84 &= ~8;
        break;
    case 29:
        func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 7);
        arg0->unk84 |= 0x1000010;
        arg0->unk84 &= ~8;
        break;
    case 30:
        func_15128774(arg0, arg0->unk3D0);
        break;
    }
}
