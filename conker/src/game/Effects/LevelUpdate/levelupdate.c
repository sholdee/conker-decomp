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
    attach_emptyLevelUpdateHook1,
    prop_emptyLevelUpdateHook2,
    effect_levelUpdateDistantTracers,
    effect_levelUpdateRain,
    sprite_runQueuedCommand,
    effect_levelUpdateRotatingZone,
    effect_levelUpdateMaterialScroll,
    func_150C79BC,
    effect_levelUpdateBounceRecords,
    effect_levelUpdateFlagLights,
    func_150DE320,
    NULL,
    prop_levelUpdateRainOnly,
    effect_playRandomZoneSounds,
    effect_levelUpdateSoundCooldowns,
    effect_levelUpdateSprayCooldowns,
    effect_levelUpdateCutsceneTreads,
    beam_levelUpdateRain,
    level_updateHazardProps,
    effect_scrollTileTextures,
    camera_levelUpdateWaveChains,
    func_150F2A60,
    func_150B003C,
    effect_advanceTexAnimClock,
    beam_levelUpdateRandomSounds,
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
    gLevelTexFrameTime += gFrameTicks;
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

    effect_emptyLevelUpdateHook();
    flamethrower_updateTexScrolls(gLevelDisplayLists[0], gLevelDisplayLists[1]);
    texture_updateUvWobbles();
    temp = gLevelHeader->updateCallback;
    if (temp != 0) {
        gLevelUpdateHooks[temp](arg1);
    }
    if (gIsMultiplayer == 0) {
        sprite_updateLevelSprites();
        effect_updateColorAnims();
    }
    gTextAnimClock += gFrameTicks << 6;
}

Gfx *effect_drawLevelHook1(Gfx *gfx, s32 arg1, s32 arg2) {
    arg1 = arg2;
    WGFX15174AA4(gfx++, 0xE7000000, 0x00000000);
    WGFX15174AA4(gfx++, 0xDA380003, &gIdentityMtx);
    if (gLevelHeader->drawCallback1 != 0) {
        gfx = gLevelDrawHooks1[gLevelHeader->drawCallback1](gfx, arg1);
    }
    WGFX15174AA4(gfx++, 0xDA380003, &gIdentityMtx);
    return gfx;
}

Gfx *effect_drawLevelHook2(Gfx *gfx, s32 arg1, s32 arg2) {
    arg1 = arg2;
    WGFX15174AA4(gfx++, 0xE7000000, 0x00000000);
    WGFX15174AA4(gfx++, 0xDA380003, &gIdentityMtx);
    if (gLevelHeader->drawCallback2 != 0) {
        gfx = gLevelDrawHooks2[gLevelHeader->drawCallback2](gfx, arg1);
    }
    WGFX15174AA4(gfx++, 0xDA380003, &gIdentityMtx);
    return gfx;
}
