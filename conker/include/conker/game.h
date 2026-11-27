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
void game_startMatch(s16 arg0, s16 arg1, s16 arg2);
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
Gfx *game_drawObjectGroup0Pass(Gfx *gfx, s16 player);
Gfx *game_drawObjectGroup1Pass(Gfx *gfx, s16 player);
void game_prepareView(s16 player);
void game_beginFrame(void);
s32 game_updateFrame(void);
void game_updateAfterMainPass(void);
Gfx *game_drawWorldPass(Gfx *gfx, s16 player);
Gfx *game_drawMainPass(Gfx *gfx, s16 player);
Gfx *game_drawScreenPass(Gfx *gfx);
Gfx *game_drawViewOverlays(Gfx *gfx, s32 player);
Gfx *game_drawObjectGroup5Pass(Gfx *gfx, s32 player);
Gfx *game_drawModeOverlayPass(Gfx *gfx);
void game_updateFrameTiming(void);
extern u8 gStepTicks; // the ticks of this frame
extern f32 gFrameDeltaHistory[10]; // the last 10 frame deltas
extern u32 gTickRemainder; // the leftover tick remainder
extern u8 gRebuildFrameDisplayLists;
extern u8 gLevelChangeRequest;
extern u8 gIsMultiplayer;
extern u8 gViewLayoutChanged;
extern s8 D_800BE618;
extern u8 gFrameDeltaHistoryNext;
extern u8 gFrameDeltaHistoryCount;

/* game/Game/LevelChange/levelchange.c */
void game_requestLevelChange(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern u8 gFrameSteps; // frame steps of this frame (half the ticks, see game_updateFrameTiming)
extern f32 gFrameDelta; // frame delta: half the ticks of this frame
extern f32 gFrameDeltaInv; // 1 / gFrameDelta (0 when that is 0)
extern s32 gTotalTicks; // ticks since the start
extern s32 gTotalTicksMod30; // gTotalTicks % 30
extern u8 gLevelLoadState;
extern f32 gAvgFrameDelta; // the average of the last 10 frame deltas
extern f32 gAvgFrameDeltaInv; // 1 / gAvgFrameDelta
extern u8 gDrawBufferIndex; // which of the two matrix buffers is being built this frame
extern u16 *gZBuffer; // z-buffer (gScreenWidth x gScreenHeight)
extern Gfx *gDisplayListBuffers[2]; // display-list buffer per framebuffer (gDrawBufferIndex)
extern Gfx *gDisplayListHead; // display-list write head
extern Gfx *gDisplayListPrologueEnds[2]; // per framebuffer: end of the prologue built by view_initFrameDisplayLists
extern OSMesg gSiEventMesg;
extern s32 gFrameTicks; // frames elapsed this update
extern s32 gLastPlayerIndex; // number of players minus one
extern u8 D_800BE9EC;
extern u8 gClearToWhite;

/* Level ids: the values of `level` (and of IdRecord.LoadSection, the level a task was loaded in). Original names
 * from the October 2000 debugger's level table (its soak test); Rare's identifiers with the prefix LEVEL_, so
 * the ones Rare wrote with a leading underscore (cut or test levels) get a double underscore. The US code
 * compares the same values: paired prerelease functions use the same constants up to 66, and
 * script_canAutoSave lists the multiplayer, frontend, cutscene and cut levels. */
enum LevelId {
    /*  0 */ LEVEL_W1_BULL_PADDOCK,
    /*  1 */ LEVEL_W1_BARN_INSIDE,
    /*  2 */ LEVEL_W1_BRONTO_CAVERN,
    /*  3 */ LEVEL__W2_MOLEHOLE,
    /*  4 */ LEVEL_W2_OUTSIDE_MOLEHOLE,
    /*  5 */ LEVEL_INTRO_CUTSCENE,
    /*  6 */ LEVEL_THE_WINDMILL,
    /*  7 */ LEVEL_W1_DOG_FISH,
    /*  8 */ LEVEL_ANIMTEST,
    /*  9 */ LEVEL__HITS_TEST,
    /* 10 */ LEVEL_W2_BOILERROOM,
    /* 11 */ LEVEL_W1_COG_LAND,
    /* 12 */ LEVEL_W1_BARN_OUTSIDE,
    /* 13 */ LEVEL__W2_WARZONE,
    /* 14 */ LEVEL_W2_TANK_FACTORY,
    /* 15 */ LEVEL__W2_MISSILE_SILO,
    /* 16 */ LEVEL_W1_BARN_ARENA,
    /* 17 */ LEVEL__W1_DOG_FISH_NURSERY,
    /* 18 */ LEVEL_W1_INSIDE_SAFE,
    /* 19 */ LEVEL_W2_HARBOUR,
    /* 20 */ LEVEL_W1_DINO_ARENA,
    /* 21 */ LEVEL__W3_ED_TEST,
    /* 22 */ LEVEL__MARK_TEST,
    /* 23 */ LEVEL_W1_DOG_FISH_NURSERY_PART1,
    /* 24 */ LEVEL_W5_THRONE_ROOM,
    /* 25 */ LEVEL_W1_VOLCANO_INSIDE,
    /* 26 */ LEVEL_WMP_HEIST,
    /* 27 */ LEVEL_W2_LIFTTUNNEL,
    /* 28 */ LEVEL_W2_CANYON,
    /* 29 */ LEVEL_FRONTEND,
    /* 30 */ LEVEL_W2_FLAMETUNNEL,
    /* 31 */ LEVEL__PRESENT_CUTSCENE,
    /* 32 */ LEVEL_FRONTEND_CUTSCENE,
    /* 33 */ LEVEL_WFRONTEND_TABLE,
    /* 34 */ LEVEL_WFRONTEND_HELL,
    /* 35 */ LEVEL_W2_WARBOSS,
    /* 36 */ LEVEL_WMP_TANK,
    /* 37 */ LEVEL_GENERIC_CUTSCENES,
    /* 38 */ LEVEL_W2_MACHINEGUNNEST,
    /* 39 */ LEVEL_W1_BEE_CAVERN,
    /* 40 */ LEVEL_W2_TANKTUNNEL,
    /* 41 */ LEVEL_W1_TRAINING,
    /* 42 */ LEVEL_WMP_ARENA,
    /* 43 */ LEVEL_WMP_BEACH,
    /* 44 */ LEVEL_W1_POO_CABIN,
    /* 45 */ LEVEL_WMP_MAIN_WAR,
    /* 46 */ LEVEL_W1_TRAINING_CAVE,
    /* 47 */ LEVEL_W2_LAB,
    /* 48 */ LEVEL_WMP_BEACH_WAR,
    /* 49 */ LEVEL_W1_ROCK_SOLID,
    /* 50 */ LEVEL_W1_VOLCANO_RACE,
    /* 51 */ LEVEL_WMP_VOLCANO_RACE,
    /* 52 */ LEVEL_WMP_TUNNELS,
    /* 53 */ LEVEL_W1_POO_BOSS,
    /* 54 */ LEVEL_W5_BANK,
    /* 55 */ LEVEL_W1_U_BEND,
    /* 56 */ LEVEL_W5_BLADE_BOARDROOM,
    /* 57 */ LEVEL_W5_BANK_ENTRANCE,
    /* 58 */ LEVEL_W5_BANK_END,
    /* 59 */ LEVEL_W1_WINDMILL_EXPLODED,
    /* 60 */ LEVEL_W3_HAUNTED_HOUSE,
    /* 61 */ LEVEL_W3_HAUNTED_OUTHOUSE,
    /* 62 */ LEVEL__WMP_HAUNTED_HOUSE,
    /* 63 */ LEVEL_WMP_RAPTOR,
    /* 64 */ LEVEL_W5_END_ARENA,
    /* 65 */ LEVEL_W3_HORROR_INTRO,
    /* 66 */ LEVEL_W1_BRONTO_THROAT,
    /* 67 */ LEVEL_W1_RAPTOR_NEST,
    /* 68 */ LEVEL_W1_RAPTOR_NEST_FLOODED
};

extern s32 level; // the current map
extern s32 gNextLevel; // current scene / game mode id
extern s32 gPrevLevel; // the previous map
extern s32 gNextMatchPlayerMask;
extern s32 gNextMatchMode;
extern s32 gRawFrameTicks; // the ticks of this frame, before the gFrameRateDivider divider
extern u8 gFrameRateDivider; // frame rate divider (0: none; 1: time stands still)
extern OSMesgQueue gGameMesgQueue; // graphics task done messages
extern OSMesg gGameMesgBuf[16]; // gGameMesgQueue's buffer
extern OSScMsg gTaskDoneMsgs[2]; // "task done" messages, one per frame buffer
extern s16 gPendingGfxTasks; // graphics tasks sent
extern u8 gCullLevelDisplayList;
extern u8 gTextLanguage;
extern u8 gPaused;
extern u8 gPauseRequested;
extern u8 gPauseCooldown;
extern u8 gRumbleStopRequest;

/* game/Game/FrameStub/framestub.c */
void func_1501C860(void);

/* game/Game/UpdateStub/updatestub.c */
void func_1501C870(void);

#endif
