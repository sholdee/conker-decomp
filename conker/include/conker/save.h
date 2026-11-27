#ifndef CONKER_SAVE_H
#define CONKER_SAVE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "save": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Save/SaveData/savedata.c
 *   game/Save/EepromTimedWrite/eepromtimedwrite.c
 *   game/Save/FrontEnd/frontend.c
 */

/* Per-team x jitter for stacked names (game/Save/FrontEnd/frontend.c); a struct so the initial copy is one word. */
typedef struct Offs4 {
    s8 v[4];
} Offs4;

/* game/Save/SaveData/savedata.c */
void save_resetGameState(void);
void save_readFile(s32 eepromSlot);
void save_writeSlot(s32 saveSlot);
void save_eraseSlot(s32 saveSlot);
void save_resetModeSettings(void);
void save_readModeSettings(void);
void save_writeModeSettings(void);
void save_readSettings(void);
void save_writeSettings(void);
void func_15007644(void);
void save_requestReadSettings(void);
void save_requestWriteSettings(void);
void save_requestReadModeSettings(void);
void save_requestWriteModeSettings(); /* defined with no parameters, but game/Save/FrontEnd/frontend.c passes one */
void save_requestReadFile(s32 eepromSlot);
void save_requestWriteSlot(s32 saveSlot);
void save_requestEraseSlot(s32 saveSlot);
void save_beginScan(void);
extern s8 gEepromRequest; // the pending EEPROM request
extern ModeSettings *gModeSettings; // saved settings, one record per multiplayer mode
extern u8 gSaveScanning; // scanning the EEPROM save files
extern s16 gScanBestFlagCount;
extern u8 D_800BE3DE;
extern s8 gCheckpointLevel;
extern u8 gCheckpointSpawn;
extern u32 D_800BE3E4; // saved clock (seconds)
extern s32 gPlayTime; // play time (seconds)
extern s8 gEepromRequestArg; // argument of the pending EEPROM request gEepromRequest
extern u8 *gSaveEntryBits; /* bit width of each of the 69 gPlayedCutscenes entries in a save file */
extern SaveSlotInfo gSaveSlots[3]; // the three save slots

/* game/Save/EepromTimedWrite/eepromtimedwrite.c */
s32 save_writeEeprom(OSMesgQueue *mq, u8 block, u8 *buffer, s32 size); /* timed EEPROM write request */
s32 save_pollEepromWrite(s32 arg0);

/* game/Save/FrontEnd/frontend.c */
void save_resetSettings(void);
void save_validateSettings(void);
s32 save_invertBits(s32 value);
void save_quitToFrontEnd(void);
void save_updatePauseMenu(void);
void func_151DE6CC(void);
void save_updateStartupScene(void);
void func_151DE7D4(void);
void func_151DE81C(void);
void func_151DE85C(void);
void func_151DE8E8(void);
void save_updateMenuPages(void);
void func_151DF1BC(void);
void save_updateListPage(void);
void save_updateSaveSlotPage(void);
void save_updateModeSelectPage(void);
void save_updateOptionsPage(void);
void save_updateJoinPage(void);
void save_updateCpuPlayersPage(void);
void save_updateTeamPage(void);
void func_151E2284(void);
void save_applyModeSettings(void);
void save_storeModeSettings(void);
s32 save_addPartnerPlayers(s32 *out);
void save_setupMatch(void);
void save_startListSelection(void);
void save_startSavedGame(void);
void save_drawOptionMenu(s32 optionMask, s32 carousel, s32 showArrows, u8 alpha, s8 fade);
void save_updateModeIntro(void);
void save_playModeIntro(void);
void save_updateResultsScreen(void);
void save_endRound(void);
void func_151E4DC4(void);
void func_151E4DD8(void);
void func_151E4E00(void);
void func_151E4E64(void);
void save_updateFadeTransition(void);
void save_resetMenuState(void);
void save_initMenu(void);
void save_updateFrontEnd(void);
void save_updateFadeIn(void);
void save_endMatch(s32 flag);
void save_resetControllerMap(void);
void save_updatePulse(void);
void func_151E562C(void);
s8 save_isFreeForAll(void);
s32 save_checkMatchOver(void);
s32 save_getTeam(s32 player);
s8 save_getScreenCount(void);
/* void save_recordHit(Actor *source, Actor *victim, s32 cause) is declared in the files that call it:
   game/Actor/ActorMotion/actormotion.c was compiled with a u8 cause. */
void save_loadTextBank(s32 bank);
void save_forgetTextBanks(void);
void save_updateMenuCamera(s32 index);
void func_151E7DC0(void);
void save_applySoundMode(void);
void save_checkCodeTamper(void);
void save_spawnMenuCharacter(s32 slotIdx, s32 charIdx);
void save_clearMenuCharacters(void);
void save_startCredits(void);
void save_updateCredits(void);
void save_updateAfterCredits(void);
extern s8 D_8008FD70;
extern u8 gFrontEndFadeStep;
extern s32 gMatchTimeLeft;
extern s8 D_8008FD7C;
extern u8 D_8008FD80;
extern s8 D_8008FD84[4];
extern s8 D_8008FD88;
extern s8 gPlayerCount;
extern s8 gHumanCount;
extern s8 gHumansRemaining;
extern s8 gPauseConfirm;
extern s8 D_8008FD9C;
extern s8 gExtraPlayerSlots; /* extra (non-controller) slots in the player list */
extern u8 gMenuLoadStep;
extern s8 gCurrentSaveSlot;
extern s16 D_8008FDAC;
extern s8 gMenuListRow; /* highlighted row of the save_updateListPage list */
extern u8 D_8008FDB4;
extern u8 D_8008FDB8;
extern u16 gMatchFlags;
extern u16 gMatchHudFlags;
extern s8 D_8008FDC4;
extern s8 gFreeForAll;
extern s16 D_8008FDCC; /* save_updatePauseMenu reads its low byte */
extern f32 D_8008FDD0;
extern MenuState *gMenuState;
extern s32 gTextBank2;
extern s8 D_8008FDDC;
extern s8 D_8008FDE0;
extern s8 D_8008FDE4;
extern f32 D_8008FDE8;
extern void (*gFrontEndStateHandlers[12])(void);
extern f32 gHudScaleX;
extern f32 gHudScaleY;
extern s32 D_8008FE24;
extern s8 gFrontEndInputDelay;
extern s8 gPauseCursor;
extern s8 gSettingsLoaded;
extern u8 D_8008FE34;
extern f32 D_8008FE38;
extern f32 D_8008FE3C;
extern s8 gRosterSize; /* number of players in gRoster */
extern s8 gRoster[16]; /* controller of each player; -2: extra slot, -1: unused */
extern s8 gSettingDefaults[23]; /* settings defaults */
extern s8 gSettingMins[23]; /* settings minimums */
extern s8 gSettingMaxes[23]; /* settings maximums */
extern s8 D_8008FE9C[8];
extern u32 D_8008FEA4[8];
extern s8 D_8008FEC4[32];
extern s8 D_8008FEE4[8]; /* per team preset: row of gPlayerCountLimits */
extern const u8 gTeamColorsRGB[10][3]; /* team colours (RGB) */
extern s8 D_8008FEEC;
extern s8 D_8008FEF0;
extern s8 D_8008FEF8;
extern s8 gMenuListCategory;
extern u8 gMenuListSelection;
extern u8 D_8008FF08[28];
extern Offs4 D_8008FF34;
extern s8 D_8008FF38[4]; /* indexed from 1 through D_8008FF37 */
extern u8 D_8008FF40[6];
extern f32 D_8008FF48[6];
extern f32 D_8008FF60[6];
extern s8 D_8008FF78[6];
extern s8 D_8008FF80[6];
extern u8 D_8008FF88[8];
extern f32 D_8008FF90[8];
extern u8 D_8008FFB0[2][6];
extern s16 gCreditsLine;
extern s32 gFrontEndTimer;
extern u8 gMenuBrightness; /* menu brightness, scales the page alpha */
extern u8 gMenuPageFade; /* current page's fade level, 0-0xFF */
extern s16 D_800E0AA0[4];
extern s8 gPlayerRanks[16];
extern s16 D_800E0AD0[16];
extern s8 gPlayerCharacters[4];
extern u8 gFrontEndState;
extern u8 gPrevFrontEndState;
extern u8 gFrontEndFade;
extern u8 gMenuPulse;
extern u8 D_800E0B99;
extern Actor *gMenuCharacters[4]; /* characters spawned by save_spawnMenuCharacter */
extern s8 D_800E0BB0;
extern s8 D_800E0BB1;
extern u8 D_800E0BB8[16];
extern s8 D_800E0BC8[4];
extern u16 D_800E0BCC;
extern s8 gPartnerSlots[2];
extern s8 gBotDifficulty;
extern u8 D_800E0BD1;
extern s8 D_800E0BD2;
extern s8 D_800E0BD3;
extern s32 gTextBank;
extern u8 **gTextStrings; // text strings of the current language ([8 + n]: player names)
extern u8 D_800E0BDC;
extern s8 gSettings[0x1C];
extern s8 gRosterTeams[16]; /* team of each gRoster entry */
extern s16 D_800E0C10[4];
extern s32 D_800E0C18[2];
extern s8 gMatchResult;

#endif
