#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c); conker/effect.h describes it. */
s32 D_800DD400; /* not referenced */
u8 D_800DD404;
u8 gLevelTexFrame;
u8 gLevelTexFrameTime;

#include "functions.h"
#include "variables.h"

u8 D_8008CD00 = 0;

/* Per-level update hooks, indexed by the level header's updateCallback (0: none). */
static void (*gLevelUpdateHooks[])(s32) = {
    NULL,
    func_150D82B0,
    func_150BEF70,
    func_150EA8E0,
    effect_levelUpdateRain,
    sprite_runQueuedCommand,
    func_150CEF60,
    func_150DF920,
    func_150C79BC,
    effect_levelUpdateBounceRecords,
    func_15100340,
    func_150DE320,
    NULL,
    func_1510AA20,
    effect_playRandomZoneSounds,
    effect_levelUpdateSoundCooldowns,
    func_150FCFB0,
    func_150FDD10,
    func_150F568C,
    func_150D0E90,
    effect_scrollTileTextures,
    func_150F5310,
    func_150F2A60,
    func_150B003C,
    effect_advanceTexAnimClock,
    func_150F20F0,
    effect_updateSurfaceSound,
    NULL,
};

/* Per-level draw hooks, indexed by the level header's drawCallback1 and drawCallback2 (0: none). */
static Gfx *(*gLevelDrawHooks1[2])(Gfx *, s32) = { NULL, NULL };
static Gfx *(*gLevelDrawHooks2[5])(Gfx *, s32) = { NULL, NULL, NULL, NULL, NULL };

#define WGFX15174AA4(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

void effect_advanceLevelTexFrame(s32 arg0, s32 arg1) {
    gLevelTexFrameTime += D_800BE9E4;
    if (arg0 < gLevelTexFrameTime) {
        gLevelTexFrame++;
        if (gLevelTexFrame >= arg1) {
            gLevelTexFrame = 0;
        }
        gLevelTexFrameTime = 0;
    }
}

void effect_updateLevel(s32 arg0, s32 arg1) {
    u8 temp;

    func_15165F70();
    flamethrower_updateTexScrolls(gLevelDisplayLists[0], gLevelDisplayLists[1]);
    texture_updateUvWobbles();
    temp = D_800B0DF0->updateCallback;
    if (temp != 0) {
        gLevelUpdateHooks[temp](arg1);
    }
    if (gIsMultiplayer == 0) {
        sprite_updateLevelSprites();
        effect_updateColorAnims();
    }
    gTextAnimClock += D_800BE9E4 << 6;
}

Gfx *effect_drawLevelHook1(Gfx *gfx, s32 arg1, s32 arg2) {
    arg1 = arg2;
    WGFX15174AA4(gfx++, 0xE7000000, 0x00000000);
    WGFX15174AA4(gfx++, 0xDA380003, &gIdentityMtx);
    if (D_800B0DF0->drawCallback1 != 0) {
        gfx = gLevelDrawHooks1[D_800B0DF0->drawCallback1](gfx, arg1);
    }
    WGFX15174AA4(gfx++, 0xDA380003, &gIdentityMtx);
    return gfx;
}

Gfx *effect_drawLevelHook2(Gfx *gfx, s32 arg1, s32 arg2) {
    arg1 = arg2;
    WGFX15174AA4(gfx++, 0xE7000000, 0x00000000);
    WGFX15174AA4(gfx++, 0xDA380003, &gIdentityMtx);
    if (D_800B0DF0->drawCallback2 != 0) {
        gfx = gLevelDrawHooks2[D_800B0DF0->drawCallback2](gfx, arg1);
    }
    WGFX15174AA4(gfx++, 0xDA380003, &gIdentityMtx);
    return gfx;
}
