#include <ultra64.h>

// This file's .bss, defined before the headers that declare the globals: IDO lays out .bss in
// the order it first sees each name, and that order also depends on how many types and variables are
// declared before it (see init/Sched/SchedInit/schedinit.c), which the subsystem headers change often.
u8 gStepTicks;      // the ticks of this frame; bit 0 carries over to the next
f32 gFrameDeltaHistory[10]; // the last 10 frame deltas
u32 gTickRemainder;     // the leftover tick remainder
u8 gRebuildFrameDisplayLists;
u8 gLevelChangeRequest;
u8 gIsMultiplayer;
u8 gViewLayoutChanged;
s8 D_800BE618;
u8 gFrameDeltaHistoryNext; // the next slot of gFrameDeltaHistory
u8 gFrameDeltaHistoryCount; // the number of filled slots of gFrameDeltaHistory

#include "conker/actor.h"
#include "conker/anim.h"
#include "conker/audio.h"
#include "conker/bone.h"
#include "conker/boot.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/cutscene.h"
#include "conker/debug.h"
#include "conker/effect.h"
#include "conker/env.h"
#include "conker/flare.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/heap.h"
#include "conker/hud.h"
#include "conker/level.h"
#include "conker/libc.h"
#include "conker/light.h"
#include "conker/model.h"
#include "conker/mp.h"
#include "conker/mp3.h"
#include "conker/music.h"
#include "conker/player.h"
#include "conker/prop.h"
#include "conker/rom.h"
#include "conker/save.h"
#include "conker/sched.h"
#include "conker/screen.h"
#include "conker/script.h"
#include "conker/shadow.h"
#include "conker/sound.h"
#include "conker/sprite.h"
#include "conker/swarm.h"
#include "conker/text.h"
#include "conker/texture.h"
#include "conker/trail.h"
#include "conker/view.h"
#include "conker/zone.h"

/* Game main loop: runs frames until game_processMessage says to stop. */
void game_mainLoop(void) {
    libc_resetPaging();
    view_initFrameDisplayLists();
    save_forgetTextBanks();

    gStepTicks = 1;
    gEliminatedPlayers = 0;
    gTakeoverPlayers = 0;
    *(s32 *)gEliminatedSquareSizes = 0; // clears all four players with one word store
    gRacersUpdated = 0;

    while (game_processMessage() != 0) {
    }

    D_8002AC60 = 1;
    D_8002AC64 = 3500;
    gPadTable = cont_readPads();

    while (save_pollEepromWrite(1) != 0) {
    }
}

/*
 * Append one GBI command (w0, w1) at the frame's display-list write head gDisplayListHead.
 * matching: GBI macros on a post-incremented head, or taking &gDisplayListHead, move the pointer to v0.
 */
#define DL_APPEND(hi, lo)                       \
    {                                           \
        Gfx *cmd = gDisplayListHead++;                \
        cmd->words.w0 = (hi);                   \
        cmd->words.w1 = (u32)(lo);              \
    }

/* gSPLoadUcode(textStart, dataStart) at the display-list head, written out like DL_APPEND. */
#define DL_LOAD_UCODE(textStart, dataStart)     \
    {                                           \
        Gfx *cmd = gDisplayListHead++;                \
        cmd->words.w0 = _SHIFTL(G_RDPHALF_1, 24, 8); \
        cmd->words.w1 = (u32)(dataStart);       \
        cmd = gDisplayListHead++;                     \
        cmd->words.w0 = _SHIFTL(G_LOAD_UCODE, 24, 8) | (SP_UCODE_DATA_SIZE - 1); \
        cmd->words.w1 = (u32)(textStart);       \
    }

/*
 * Waits for the next message on gGameMesgQueue and handles it; type 1 runs and draws one game frame.
 * Returns 0 when the main loop should end.
 */
s32 game_processMessage(void) {
    s16 *message;
    s16 i;

    message = NULL;
    osRecvMesg(&gGameMesgQueue, (OSMesg *)&message, OS_MESG_BLOCK);
    D_8002AC60 = 0;
    D_8002AC64 = 800;
    switch (*message) {
    case 2:
        flare_sampleDepths();
        if (gGlowCount != 0) {
            flare_sampleGlowDepths();
        }
        gPendingGfxTasks--;
        if (gLevelChangeRequest != 0 && gPendingGfxTasks == 0) {
            heap_free(gDisplayListBuffers[0]);
            heap_free(gDisplayListBuffers[1]);
            gDisplayListBuffers[0] = NULL;
            return 0;
        }
        break;
    case 1:
        if (gLevelChangeRequest == 0) {
            save_updateFrontEnd();
            if (gLevelChangeRequest == 0) {
                if (gRebuildFrameDisplayLists != 0) {
                    view_initFrameDisplayLists();
                    gRebuildFrameDisplayLists = 0;
                }
                if (gFovChangePending != 0) {
                    view_reapplyFovChanges();
                }
                if (gClipChangePending != 0) {
                    view_reapplyClipPlanes();
                }
                game_beginFrame();
                if (gPaused != 0 || game_updateFrame() != 0) {
                    gDisplayListHead = view_setColorImage(gDisplayListHead);
                    light_clearPlayerMasks();
                    effect_preDrawObjects();
                    for (i = 0; i <= gScreenLayout; i++) {
                        gCurrentView = gViewPlayers[i];
                        game_prepareView(gCurrentView);
                        gDisplayListHead = game_drawWorldPass(gDisplayListHead, gCurrentView);
                    }
                    if (D_800BEBA0 == 0) {
                        DL_LOAD_UCODE(gUcodeText, gUcodeData);
                        DL_APPEND(0xD9C2F9FA, 0);        // gSPClearGeometryMode(0x3D0605)
                        DL_APPEND(0xD9FFFFFF, 0x220005); // gSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_LIGHTING | G_SHADING_SMOOTH)
                        DL_APPEND(0xDA380003, &gIdentityMtx); // gSPMatrix(G_MTX_LOAD)
                        DL_APPEND(0xDC08000A, &gLookAts[gDrawBufferIndex].l[0]); // gSPLookAtX
                        DL_APPEND(0xDC08060A, &gLookAts[gDrawBufferIndex].l[1]); // gSPLight(1)
                    }
                    {
                        Gfx *savedHead;

                        for (i = 0; i <= gScreenLayout; i++) {
                            savedHead = gDisplayListHead;
                            gCurrentView = gViewPlayers[i];
                            gDisplayListHead = light_beginView(gDisplayListHead, gCurrentView);
                            gDisplayListHead = game_drawMainPass(gDisplayListHead, gCurrentView);
                            // Out of display-list space: drop this player's commands.
                            // matching: the ?: materialises the flag (move v1,zero / li v1,1)
                            if ((gDisplayListHead - gDisplayListBuffers[gDrawBufferIndex] > gDisplayListLimit) ? 1 : 0) {
                                gDisplayListHead = savedHead;
                            }
                        }
                        gCurrentView = 0;
                        game_updateAfterMainPass();
                        gDisplayListHead = game_drawScreenPass(gDisplayListHead);
                        for (i = 0; i <= gScreenLayout; i++) {
                            gCurrentView = gViewPlayers[i];
                            gDisplayListHead = light_beginView(gDisplayListHead, gCurrentView);
                            gDisplayListHead = game_drawObjectGroup5Pass(gDisplayListHead, gCurrentView);
                            if (gPaused == 0 && gScreenLayout > 0) {
                                gDisplayListHead = game_drawViewOverlays(gDisplayListHead, gCurrentView);
                            }
                        }
                        gDisplayListHead = game_drawModeOverlayPass(gDisplayListHead);
                        if (gPaused == 0 && gScreenLayout == 0) {
                            for (i = 0; i <= gScreenLayout; i++) {
                                gCurrentView = gViewPlayers[i];
                                gDisplayListHead = game_drawViewOverlays(gDisplayListHead, gCurrentView);
                            }
                        }
                        gDisplayListHead = hud_drawMain(gDisplayListHead);
                        gDisplayListHead = text_drawQueue(gDisplayListHead);
                    }
                    bone_collapseAllMaskedBones();
                    actor_flushBoneMatrices();
                    trail_limitHistorySpacing(0);
                    collision_updateModelUnload();
                    camera_tickSnapTimers();
                    DL_APPEND(_SHIFTL(G_RDPFULLSYNC, 24, 8), 0); // gDPFullSync
                    DL_APPEND(_SHIFTL(G_ENDDL, 24, 8), 0); // gSPEndDisplayList
                    rom_waitReads();
                    texture_updateAging();
                    gfx_freeHiddenObjectBlocks();
                    sprite_freeTypeTextures();
                    cutscene_sendGfxTask(0, 1);
                    gLevelLoadState = 0;
                    if (gPaused != 0) {
                        gPauseCooldown = 2;
                    } else if (gPauseCooldown > 0) {
                        gPauseCooldown--;
                    }
                    if (gPaused != gPauseRequested) {
                        gPaused = gPauseRequested;
                        if (gPaused != 0) {
                            music_pauseAll(1);
                            sound_setMuted(1);
                            if (func_151F2CDC() != 0) {
                                func_151F2BE8();
                            }
                        } else {
                            music_pauseAll(0);
                            sound_setMuted(0);
                            if (func_151F2CDC() != 0) {
                                func_151F2C4C();
                            }
                        }
                    }
                }
            }
        }
        break;
    case 4:
        while (save_pollEepromWrite(1) != 0) {
        }
        for (;;) {
        }
    }
    return 1;
}

/* Per-frame updates called from game_updateFrame, most of them only outside gPaused mode. */
void game_updateActors(void) {
    zone_updateGlobal(gFrameTicks);
    actor_updateVisibility();
    func_1501C870();
    if (gPaused == 0) {
        if (gCutsceneStates[0] == 1) {
            cutscene_updateActors();
        } else {
            actor_updateAll();
        }
        player_syncHealth(gMatchState.playerMask);
    }
    actor_updateSizes();
    actor_updatePlayerStates();
}

/* Draws group 0 of player's display lists (effect_drawObjectGroup), then effect_drawLevelHook1 for the level. */
Gfx *game_drawObjectGroup0Pass(Gfx *gfx, s16 player) {
    if (gIsMultiplayer == 0) {
        sprite_drawLevelSprites(&gfx, 1);
    }
    gfx = effect_drawObjectGroup(gfx, 0, player, 0);
    gfx = effect_drawObjectGroup(gfx, 0, player, 1);
    return effect_drawLevelHook1(gfx, level, player);
}

/* Counterpart of game_drawObjectGroup0Pass: effect_drawLevelHook2, then group 1 of player's display lists. */
Gfx *game_drawObjectGroup1Pass(Gfx *gfx, s16 player) {
    gfx = effect_drawLevelHook2(gfx, level, player);
    gfx = effect_drawObjectGroup(gfx, 1, player, 0);
    gfx = effect_drawObjectGroup(gfx, 1, player, 1);

    sprite_drawLevelSprites(&gfx, 2);

    return gfx;
}

/* Per-player setup before drawing player's view (D_800D3674 follows the sign of camera->CameraPitch). */
void game_prepareView(s16 player) {
    Camera *camera = &camera_global_Cameras[player];
    gDebugStatusCode = 0xA000000;
    if ((screen_isViewFadeDone(player) == 0) && (gPaused == 0)) {
        screen_advanceViewFade(player);
    }
    light_updatePlayerLight(player);
    D_800D3674 = (camera->CameraPitch < 0.0f) ? -1000 : 5000;
    player_updateWarp(player);
    gDebugStatusCode = 0;
}

/* Start of frame: reset the display-list head to this frame's buffer and update timing. */
void game_beginFrame(void) {
    gDisplayListHead = gDisplayListPrologueEnds[gDrawBufferIndex];
    gPadTable = cont_readPads();
    game_updateFrameTiming();
    if ((gPauseRequested != 0) && (gPaused == 0)) {
        effect_broadcastEvent(0, 0x47);
    }
    cont_updateRumble();
    cont_startReadTimer();
    actor_resetBonePool();
    if (0) {} // matching: IDO picks v0 (not v1) for the camera_global_Cameras load only with this
    music_update(camera_global_Cameras->focus.x, camera_global_Cameras->focus.y, camera_global_Cameras->focus.z);
    sound_update();
    if (gFrontEndState == 2) {
        math_seedRandom(0x81280783);
    }
    audio_updateMix();
}


// gPageSlots is the 508-entry TLB slot table (see the handwritten miss handler at 0x10006138,
// which uses the same {page, age} layout); this pass ages every slot by one frame.
typedef struct {
    s32 page;
    s32 age;
} TlbSlot;

#define TLB_SLOT_COUNT 508

/* The frame's game-logic update; returns 1 when the frame should be drawn. */
s32 game_updateFrame(void) {
    s32 i;
    u8 player;

    texture_clearBindings();
    script_purgeTasks(0);
    script_runTasks(2);
    gAmbientPriority = 0;
    heap_expireBlocks();
    cutscene_advanceTime(0);
    model_clearReflections();
    view_updateMatrices();
    collision_selectMap(0);
    gfx_flagObjectUpdates();
    if (gPaused == 0) {
        gfx_runObjectUpdates(1);
    }
    if (gPaused == 0) {
        gfx_dispatchObjectContacts();
    }
    collision_updateTriggers();
    game_updateActors();
    cutscene_dispatchEvents(1, 0);
    cutscene_evalTracks(0);
    cutscene_dispatchEvents(0, 0);
    cutscene_applyTracks(0);
    cutscene_checkEnd(0);
    if (gRacersUpdated != 0) {
        mp_updateRaceCameras();
    }
    camera_updateAll();
    actor_hideTanksInSniperView();
    collision_selectMap(0);
    for (i = 0; i <= gScreenLayout; i++) {
        collision_updateCameraZone(i);
    }
    view_updateMatrices();
    for (player = 0; player <= gScreenLayout; player++) {
        sprite_setViewMtx(player);
    }
    gfx_buildDrawLists(0);
    effect_updateLevel(level, 0);
    func_1501C860();
    prop_emptyFrameHook(gDrawBufferIndex);
    trail_updateBoneTrails(0);
    if (gPaused == 0) {
        effect_runFrameHooks();
        effect_preUpdateObjects();
    }
    actor_updateObjects();
    if (gIsMultiplayer == 0) {
        zone_updateReflections();
    }
    gfx_updateObjectMatrices();
    trail_updateBoneTrails(1);
    env_reapplyFog(level);
    actor_updateBlobShadows();
    gfx_updateObjectShadows();
    if (gActionsPending != 0) {
        actor_runPending();
    }
    if (gPaused == 0) {
        swarm_updateModelSlots();
        effect_updateObjects();
        prop_updateWaveDeformers();
    }
    if (gPaused == 0) {
        screen_updateIrises();
    }
    screen_updateViewFlashes();
    screen_updateTint();
    model_queueWaterReflections();
    func_1515D6C8();
    for (i = 0; i < TLB_SLOT_COUNT; i++) {
        ((TlbSlot *)gPageSlots)[i].age++;
    }
    return 1;
}

void game_updateAfterMainPass(void) {
    if (gPaused == 0) {
        effect_runTypeGlobalHooks();
    }
    anim_ageMorphCache();
    if (gIsMultiplayer == 0) {
        hud_updateOverlay();
    }
}

/* Sets up player's viewport and draws the first pass of their scene. */
Gfx *game_drawWorldPass(Gfx *gfx, s16 player) {
    func_1510B958(player);
    gSPViewport(gfx++, &gViews[player].viewports[gDrawBufferIndex]);
    gfx = view_setScissor(gfx, player, 0, 0, 0, 0);
    if (gPaused != 0) {
        return gfx;
    }
    if (D_80084480 != 0) {
        return gfx;
    }
    gfx = env_drawSky(gfx, level);
    if ((screen_isViewFadedOut(player) != 0) || (gEliminatedPlayers & (1 << player))) {
        return gfx;
    }
    gfx = light_beginView(gfx, player);
    return view_buildWorldDisplayList(gfx, player);
}

/* Draws the main pass of player's scene. */
Gfx *game_drawMainPass(Gfx *gfx, s16 player) {
    Camera *camera = &camera_global_Cameras[player];

    if (gPaused != 0) {
        return gfx;
    }
    if (D_80084480 != 0) {
        return gfx;
    }
    if ((screen_isViewFadedOut(player) != 0) || (gEliminatedPlayers & (1 << player))) {
        return gfx;
    }
    if (gScreenLayout > 0) {
        gfx = light_beginView(view_emitRspSetup(gfx, player), player);
        func_1510B958(player);
        gSPViewport(gfx++, &gViews[player].viewports[gDrawBufferIndex]);
        gfx = view_setScissor(gfx, player, 0, 0, 0, 0);
    }
    gfx = gfx_drawObjectList(gfx, 1, player);
    if (gFrameEchoPass == 2) {
        gfx = screen_drawFrameEcho(gfx);
    }
    gfx = effect_drawObjectGroup(gfx, 6, player, 0);
    gfx = effect_drawObjectGroup(gfx, 6, player, 1);
    gfx = shadow_drawRealTime(gfx, player);
    gfx = view_setScissor(gfx, player, 0, 0, 0, 0);
    gfx = actor_drawPass(gfx, 1, player);
    gfx = model_drawReflections(gfx, player);
    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_LOAD);
    if (camera->SpecialFlags & 1) {
        gfx = game_drawObjectGroup0Pass(gfx, player);
        gfx = gfx_drawObjectList(gfx, 0, player);
        gfx = game_drawObjectGroup1Pass(gfx, player);
    } else {
        gfx = game_drawObjectGroup1Pass(gfx, player);
        gfx = gfx_drawObjectList(gfx, 0, player);
        gfx = game_drawObjectGroup0Pass(gfx, player);
    }
    gfx = trail_drawBoneTrails(gfx, 0, player);
    gfx = trail_drawBoneTrails(gfx, 1, player);
    gfx = actor_drawPass(gfx, 2, player);
    gfx = effect_drawObjectGroup(gfx, 2, player, 0);
    return effect_drawObjectGroup(gfx, 2, player, 1);
}

/* Full-screen pass drawn once after every player's main pass. */
Gfx *game_drawScreenPass(Gfx *gfx) {
    static s8 gPauseBlurFrames = 0; // frames drawn since gPaused was set, counting up to 5

    if (gFrameEchoPass == 1) {
        gfx = screen_drawFrameEcho(gfx);
    }
    if (gScreenLayout != 0) {
        gCurrentView = gScreenLayout + 1;
        gSPViewport(gfx++, &(gCurrentView + gViews)->viewports[0]); // matching: index + base operand order
    } else {
        gSPViewport(gfx++, &gViews[0].viewports[0]);
    }
    gfx = view_setScissor(gfx, 0xFF, 0, 0, 0, 0);
    if (gPaused != 0) {
        if (gPauseBlurFrames == 0) {
            gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gScreenWidth, gZBuffer);
            gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, gScreenWidth, gScreenHeight);
            gfx = screen_drawImage(gfx, gFramebuffers[1 - gDrawBufferIndex], 0, 1);
            gfx = view_setColorImage(gfx);
            gPauseBlurFrames = 1;
        } else if (gPauseBlurFrames < 5) {
            gPauseBlurFrames++;
            screen_blurImage(gZBuffer);
        }
        gfx = screen_drawImage(gfx, gZBuffer, 0, 1);
    } else {
        gPauseBlurFrames = 0;
    }
    if (screen_isViewFadedOut(0) == 0) {
        gfx = flare_drawAll(gfx, level);
        gfx = flare_drawScreenFlash(gfx);
        gfx = screen_drawLetterbox(gfx);
    }
    gfx = screen_drawTint(gfx);
    if ((gPaused == 0) && (gPauseRequested == 0)) {
        gfx = hud_drawOverlay(gfx);
    }
    gfx = effect_drawObjectGroup(gfx, 3, 0, 0);
    gfx = effect_drawObjectGroup(gfx, 3, 0, 1);
    return sprite_endDraw(gfx);
}

/* Draws player's late pass (group 4 display lists) into their viewport. */
Gfx *game_drawViewOverlays(Gfx *gfx, s32 player) {
    gSPViewport(gfx++, &gViews[player].viewports[gDrawBufferIndex]);
    gfx = view_setScissor(gfx, player, 0, 0, 0, 0);
    gfx = screen_drawViewFlash(gfx, player);
    gfx = screen_drawViewFade(gfx, player);
    gfx = screen_drawIris(gfx, player);
    if ((gEliminatedPlayers & (1 << player)) || (gTakeoverPlayers & (1 << player))) {
        gfx = func_1507DB6C(gfx, player);
    }
    gfx = effect_drawObjectGroup(gfx, 4, player, 0);
    return effect_drawObjectGroup(gfx, 4, player, 1);
}

/* gViewMtxBuffers is one Mtx array per framebuffer, indexed by player. */
#define FRAME_MTX(fb, player) (&gViewMtxBuffers[fb][player])

/* Loads player's viewport and projection, then draws group 5 display lists. */
Gfx *game_drawObjectGroup5Pass(Gfx *gfx, s32 player) {
    gSPViewport(gfx++, &gViews[player].viewports[gDrawBufferIndex]);
    gfx = view_setScissor(gfx, player, 0, 0, 0, 0);
    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_LOAD);
    gSPMatrix(gfx++, &gViews[player].projectionMtx[gDrawBufferIndex], G_MTX_PROJECTION | G_MTX_LOAD);
    if (level == LEVEL_FRONTEND) {
        gfx = actor_drawPass(gfx, 6, player);
    }
    gfx = effect_drawObjectGroup(gfx, 5, 0, 0);
    gfx = effect_drawObjectGroup(gfx, 5, 0, 1);
    gSPMatrix(gfx++, &gViews[player].projectionMtx[gDrawBufferIndex], G_MTX_PROJECTION | G_MTX_LOAD);
    gSPMatrix(gfx++, FRAME_MTX(gDrawBufferIndex, player), G_MTX_PROJECTION);
    return gfx;
}

/* Sets the full-screen viewport and closes the frame's scene drawing. */
Gfx *game_drawModeOverlayPass(Gfx *gfx) {
    if (gScreenLayout != 0) {
        gCurrentView = gScreenLayout + 1;
        gSPViewport(gfx++, &(gCurrentView + gViews)->viewports[0]); // matching: index + base operand order
    } else {
        gSPViewport(gfx++, &gViews[0].viewports[0]);
    }
    return text_drawQueue(hud_drawModeOverlay(view_setScissor(gfx, 0xFF, 0, 0, 0, 0)));
}

/*
 * Frame timing: turns the ticks elapsed since the last frame (gFrameTicks) into the frame delta
 * gFrameDelta and its reciprocal gFrameDeltaInv, slowed down by gFrameRateDivider when that is nonzero, and keeps
 * a 10-frame running average in gAvgFrameDelta / gAvgFrameDeltaInv.
 */
void game_updateFrameTiming(void) {
    s32 ticks;
    u8 rate;
    f32 sum;
    u32 i;

    ticks = gFrameTicks;
    gRawFrameTicks = ticks;
    rate = gFrameRateDivider;
    if (rate) {
        s32 pad; // matching: a local in this block puts a -g3 block end on the join (as1 branch fill)
        if (rate == 1) {
            gFrameTicks = 0;
            gStepTicks = 0;
            ticks = 0;
            gFrameDelta = 0.0f;
        } else {
            i = ((u32 *)gFrameDeltaHistory)[10]; // matching: reads gTickRemainder (the word after the array) through gFrameDeltaHistory, keeping its address out of CSE
            i += ticks;
            gFrameDelta = (f32)ticks / (u32)rate;
            gFrameTicks = gFrameDelta;
            gFrameDelta *= 0.5f;
            if (i >= rate) {
                gFrameTicks = i / rate;
                i -= gFrameTicks * i;
                if (1) {} if (1) {} if (1) {} if (1) {} if (1) {} // matching: steers ticks/i register colouring
                gTickRemainder = i;
                ticks = gFrameTicks;
            } else {
                gTickRemainder = i;
                gFrameTicks = ticks = 0;
            }
        }
    } else {
        gFrameDelta = ticks * 0.5f;
    }
    gStepTicks &= 1;
    gStepTicks += ticks;
    gFrameSteps = gStepTicks >> 1;
    if (gFrameDelta != 0) {
        gFrameDeltaInv = 1.0f / gFrameDelta;
    } else {
        gFrameDelta = 0.0f;
        gFrameDeltaInv = 0.0f;
        ; // matching: empty statement (a compiled-out macro?); its line entry steers as1's scheduling
    }
    gTotalTicks += ticks;
    gTotalTicksMod30 = gTotalTicks % 30;
    gFrameDeltaHistory[gFrameDeltaHistoryNext] = gFrameDelta;
    gFrameDeltaHistoryNext++;
    if (gFrameDeltaHistoryNext == 10) {
        gFrameDeltaHistoryNext = 0;
    }
    if (gFrameDeltaHistoryCount < 10) {
        gFrameDeltaHistoryCount++;
        gAvgFrameDelta = gFrameDelta;
        gAvgFrameDeltaInv = gFrameDeltaInv;
    } else {
        sum = 0.0f;
        for (i = 0; i < 10; i++) {
            sum += gFrameDeltaHistory[i];
        }
        gAvgFrameDelta = sum * 0.1f;
        if (gAvgFrameDelta != 0) {
            gAvgFrameDeltaInv = 1.0f / gAvgFrameDelta;
        } else {
            gAvgFrameDeltaInv = 0.0f;
        }
    }
    script_updateTimers();
}
