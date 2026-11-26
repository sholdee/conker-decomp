#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern Gfx *(*D_8008CD74[])(Gfx *, s32);
extern Gfx *(*D_8008CD7C[])(Gfx *, s32);

#define WGFX15174AA4(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

void func_151749A0(s32 arg0, s32 arg1) {
    D_800DD406 += D_800BE9E4;
    if (arg0 < (u8)D_800DD406) {
        D_800DD405++;
        if ((u8)D_800DD405 >= arg1) {
            D_800DD405 = 0;
        }
        D_800DD406 = 0;
    }
}

extern void (*D_8008CD04[])(s32);

void func_151749F8(s32 arg0, s32 arg1) {
    u8 temp;

    func_15165F70();
    func_15195FF0(D_800B0E00[0], D_800B0E00[1]);
    func_1510C8A8();
    temp = D_800B0DF0->updateCallback;
    if (temp != 0) {
        D_8008CD04[temp](arg1);
    }
    if (D_800BE616 == 0) {
        func_1517DE5C();
        func_151880C0();
    }
    D_800CBD4E += D_800BE9E4 << 6;
}

Gfx *func_15174AA4(Gfx *gfx, s32 arg1, s32 arg2) {
    arg1 = arg2;
    WGFX15174AA4(gfx++, 0xE7000000, 0x00000000);
    WGFX15174AA4(gfx++, 0xDA380003, &D_80089470);
    if (D_800B0DF0->drawCallback1 != 0) {
        gfx = D_8008CD74[D_800B0DF0->drawCallback1](gfx, arg1);
    }
    WGFX15174AA4(gfx++, 0xDA380003, &D_80089470);
    return gfx;
}

Gfx *func_15174B48(Gfx *gfx, s32 arg1, s32 arg2) {
    arg1 = arg2;
    WGFX15174AA4(gfx++, 0xE7000000, 0x00000000);
    WGFX15174AA4(gfx++, 0xDA380003, &D_80089470);
    if (D_800B0DF0->drawCallback2 != 0) {
        gfx = D_8008CD7C[D_800B0DF0->drawCallback2](gfx, arg1);
    }
    WGFX15174AA4(gfx++, 0xDA380003, &D_80089470);
    return gfx;
}
