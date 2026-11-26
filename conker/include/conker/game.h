#ifndef CONKER_GAME_H
#define CONKER_GAME_H

#include <ultra64.h>
#include <PR/sched.h>
#include "structs.h"

/*
 * Subsystem "game": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Game/ClearBss/clearbss.c
 *   game/Game/LevelStart/levelstart.c
 *   game/Game/LevelInit/levelinit.c
 *   game/Game/MainLoop/mainloop.c
 *   game/Game/LevelChange/levelchange.c
 *   game/Game/FrameStub/framestub.c
 *   game/Game/UpdateStub/updatestub.c
 */

/* game/Game/ClearBss/clearbss.c */
void game_clearBss(void);

/* game/Game/LevelStart/levelstart.c */
void func_15007A70(s16 arg0, s16 arg1, s16 arg2);
void game_loadLevel(void);
void game_clearLevelPointers(void);
void game_clearPendingCutscene(void);
void game_resetCutscenes(s32 arg0);
void game_startPendingCutscene(void);

/* game/Game/LevelInit/levelinit.c */
void game_initLevelSubsystems(s32 levelId);

/* game/Game/MainLoop/mainloop.c */
void game_mainLoop(void);
s32 game_processMessage(void);
void game_updateActors(void);
Gfx *func_15018E88(Gfx *gfx, s16 player);
Gfx *func_15018F08(Gfx *gfx, s16 player);
void game_prepareView(s16 player);
void game_beginFrame(void);
s32 game_updateFrame(void);
void func_15019414(void);
Gfx *game_drawWorldPass(Gfx *gfx, s16 player);
Gfx *game_drawMainPass(Gfx *gfx, s16 player);
Gfx *game_drawScreenPass(Gfx *gfx);
Gfx *game_drawViewOverlays(Gfx *gfx, s32 player);
Gfx *func_15019CC8(Gfx *gfx, s32 player);
Gfx *func_15019E60(Gfx *gfx);
void game_updateFrameTiming(void);
extern u8 D_800BE5E0; // the ticks of this frame
extern f32 gFrameDeltaHistory[10]; // the last 10 frame deltas
extern u32 gTickRemainder; // the leftover tick remainder
extern u8 gRebuildFrameDisplayLists;
extern u8 gLevelChangeRequest;
extern u8 gIsMultiplayer;
extern u8 D_800BE617;
extern s8 D_800BE618;
extern u8 gFrameDeltaHistoryNext;
extern u8 gFrameDeltaHistoryCount;

/* game/Game/LevelChange/levelchange.c */
void game_requestLevelChange(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern u8 gFrameSteps; // frame steps of this frame (half the ticks, see game_updateFrameTiming)
extern f32 gFrameDelta; // frame delta: half the ticks of this frame
extern f32 gFrameDeltaInv; // 1 / gFrameDelta (0 when that is 0)
extern s32 gTotalTicks; // ticks since the start
extern s32 D_800BE9B0; // gTotalTicks % 30
extern u8 D_800BE9B4;
extern f32 gAvgFrameDelta; // the average of the last 10 frame deltas
extern f32 gAvgFrameDeltaInv; // 1 / gAvgFrameDelta
extern u8 gDrawBufferIndex; // which of the two matrix buffers is being built this frame
extern u16 *gZBuffer; // z-buffer (gScreenWidth x gScreenHeight)
extern Gfx *gDisplayListBuffers[2]; // display-list buffer per framebuffer (gDrawBufferIndex)
extern Gfx *gDisplayListHead; // display-list write head
extern Gfx *gDisplayListPrologueEnds[2]; // per framebuffer: end of the prologue built by view_initFrameDisplayLists
extern OSMesg D_800BE9E0;
extern s32 D_800BE9E4; // frames elapsed this update
extern s32 D_800BE9E8; // number of players minus one
extern u8 D_800BE9EC;
extern u8 D_800BE9ED;
extern s32 level; // the current map
extern s32 gNextLevel; // current scene / game mode id
extern s32 gPrevLevel; // the previous map
extern s32 D_800BEA00;
extern s32 D_800BEA04;
extern s32 gRawFrameTicks; // the ticks of this frame, before the gFrameRateDivider divider
extern u8 gFrameRateDivider; // frame rate divider (0: none; 1: time stands still)
extern OSMesgQueue gGameMesgQueue; // graphics task done messages
extern OSMesg gGameMesgBuf[16]; // gGameMesgQueue's buffer
extern OSScMsg gTaskDoneMsgs[2]; // "task done" messages, one per frame buffer
extern s16 gPendingGfxTasks; // graphics tasks sent
extern u8 D_800BEAAA;
extern u8 gTextLanguage;
extern u8 gPaused;
extern u8 gPauseRequested;
extern u8 gPauseCooldown;
extern u8 D_800BEAC3;

/* game/Game/FrameStub/framestub.c */
void func_1501C860(void);

/* game/Game/UpdateStub/updatestub.c */
void func_1501C870(void);

#endif
