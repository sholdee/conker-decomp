#include <ultra64.h>
#include <PR/sched.h>
#include "structs.h"

/*
 * This file's .bss, linked at 0x800BE9A0..0x800BEAD0 in this order. Defined before functions.h and
 * variables.h declare the globals (conker/game.h): IDO lays out .bss in the order it first sees each
 * name, and an earlier extern would count. The order also depends on how many types and variables
 * are declared before it (see init/Sched/SchedInit/schedinit.c).
 */
u8 gFrameSteps;               // frame steps of this frame (half the ticks, see game_updateFrameTiming)
f32 gFrameDelta;              // frame delta: half the ticks of this frame
f32 gFrameDeltaInv;              // 1 / gFrameDelta (0 when that is 0)
s32 gTotalTicks;              // ticks since the start
s32 D_800BE9B0;              // gTotalTicks % 30
u8 D_800BE9B4;
f32 gAvgFrameDelta;              // the average of the last 10 frame deltas
f32 gAvgFrameDeltaInv;              // 1 / gAvgFrameDelta
u8 gDrawBufferIndex;               // which of the two matrix buffers is being built this frame
u16 *gZBuffer;             // z-buffer (gScreenWidth x gScreenHeight)
Gfx *gDisplayListBuffers[2];          // display-list buffer per framebuffer (gDrawBufferIndex)
Gfx *gDisplayListHead;             // display-list write head
Gfx *gDisplayListPrologueEnds[2];          // per framebuffer: end of the prologue built by view_initFrameDisplayLists
OSMesg D_800BE9E0;
s32 D_800BE9E4;              // frames elapsed this update
s32 D_800BE9E8;              // number of players minus one
u8 D_800BE9EC;
u8 D_800BE9ED;
s32 level;                   // the current map
s32 gNextLevel;              // current scene / game mode id
s32 gPrevLevel;              // the previous map
s32 D_800BE9FC;              /* not referenced */
s32 D_800BEA00;
s32 D_800BEA04;
s32 gRawFrameTicks;              // the ticks of this frame, before the gFrameRateDivider divider
u8 gFrameRateDivider;               // frame rate divider (0: none; 1: time stands still)
OSMesgQueue gGameMesgQueue;      // graphics task done messages (created by boot_runGame)
OSMesg gGameMesgBuf[16];       // gGameMesgQueue's buffer
OSScMsg gTaskDoneMsgs[2];       // "task done" messages, one per frame buffer
s16 gPendingGfxTasks;              // graphics tasks sent
u8 D_800BEAAA;
u8 gTextLanguage;
u8 D_800BEAB0[16];           /* not referenced */
u8 gPaused;
u8 gPauseRequested;
u8 gPauseCooldown;
u8 D_800BEAC3;

#include "functions.h"
#include "variables.h"


void game_requestLevelChange(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    cont_stopAllRumble();
    gCarriedUnderwaterTime = 0;
    switch (arg0) {
    case 0:
        break;
    case 4:
    case 6:
        D_800BEA00 = arg4;
        D_800BEA04 = arg3;
        gNextLevel = arg1;
        gPlayerSpawnPoints[0] = arg2;
        gLevelChangeRequest = 2;
        break;
    case 1:
        gLevelChangeRequest = 3;
        gNextLevel = arg1;
        gCarriedUnderwaterTime = movement_obj[0].underwaterTime;
        break;
    case 2:
        gLevelChangeRequest = 3;
        gNextLevel = arg1;
        gPlayerSpawnPoints[0] = arg2;
        gCarriedUnderwaterTime = movement_obj[0].underwaterTime;
        break;
    case 3:
        if (gIsMultiplayer == 0) {
            gLevelChangeRequest = 4;
            player_clearSpawnPoints();
        }
        break;
    case 5:
        gNextLevel = 5;
        gLevelChangeRequest = 1;
        break;
    }
}
