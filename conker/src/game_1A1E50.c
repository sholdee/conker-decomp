#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/effect.h describes it. */
s32 D_800DD400; /* not referenced */
u8 D_800DD404;
u8 D_800DD405;
u8 D_800DD406;

#include "functions.h"
#include "variables.h"

u8 D_8008CD00 = 0;

/* Per-level update hooks, indexed by the level header's updateCallback (0: none). */
static void (*D_8008CD04[])(s32) = {
    NULL,
    func_150D82B0,
    func_150BEF70,
    func_150EA8E0,
    func_150DEC90,
    sprite_runQueuedCommand,
    func_150CEF60,
    func_150DF920,
    func_150C79BC,
    func_150C84F4,
    func_15100340,
    func_150DE320,
    NULL,
    func_1510AA20,
    func_150D0730,
    func_15103690,
    func_150FCFB0,
    func_150FDD10,
    func_150F568C,
    func_150D0E90,
    func_150CF578,
    func_150F5310,
    func_150F2A60,
    func_150B003C,
    func_150D7630,
    func_150F20F0,
    func_150F51E8,
    NULL,
};

/* Per-level draw hooks, indexed by the level header's drawCallback1 and drawCallback2 (0: none). */
static Gfx *(*D_8008CD74[2])(Gfx *, s32) = { NULL, NULL };
static Gfx *(*D_8008CD7C[5])(Gfx *, s32) = { NULL, NULL, NULL, NULL, NULL };

#define WGFX15174AA4(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

void func_151749A0(s32 arg0, s32 arg1) {
    D_800DD406 += D_800BE9E4;
    if (arg0 < D_800DD406) {
        D_800DD405++;
        if (D_800DD405 >= arg1) {
            D_800DD405 = 0;
        }
        D_800DD406 = 0;
    }
}

void func_151749F8(s32 arg0, s32 arg1) {
    u8 temp;

    func_15165F70();
    flamethrower_updateTexScrolls(D_800B0E00[0], D_800B0E00[1]);
    texture_updateUvWobbles();
    temp = D_800B0DF0->updateCallback;
    if (temp != 0) {
        D_8008CD04[temp](arg1);
    }
    if (gIsMultiplayer == 0) {
        sprite_updateLevelSprites();
        func_151880C0();
    }
    gTextAnimClock += D_800BE9E4 << 6;
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
