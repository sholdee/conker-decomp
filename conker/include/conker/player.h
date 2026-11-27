#ifndef CONKER_PLAYER_H
#define CONKER_PLAYER_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "player": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Player/SpawnLoad/spawnload.c
 *   game/Player/StatsInit/statsinit.c
 *   game/Player/HudBlockInit/hudblockinit.c
 *   game/Player/SpawnPoint/spawnpoint.c
 *   game/Player/PlayerStats/playerstats.c
 *   game/Player/PlayerWarp/playerwarp.c
 */

/* game_43B20.c */
void player_clearIdleRequests(void);
void player_clearSpawnPoints(void);
void player_loadSpawnPoints(s32 levelId);

/* game/Player/StatsInit/statsinit.c */
void player_initMatch(s16 mode, s16 playerMask);
void func_1501748C(s16 playerMask);
void player_clearMatch(void);
void player_offsetLinkGroupObjects(s32 offset);
void player_loadLinkGroups(s32 levelId);

/* game/Player/HudBlockInit/hudblockinit.c */
void player_initHudBlockCount(void);
void player_resetHudBlocks(void);

/* game/Player/SpawnPoint/spawnpoint.c */
s32 player_getBaseCharacterType(Actor *obj);
s32 player_getSpawnPoint(s32 playerIndex, s32 spawnIndex, s32 useLink, s16 *outPos, Vec3f *outCameraEye, u8 *outTilt,
                  u8 *outFacing, s32 *outUnk14, s32 *outDoorWalkTime, s32 unused, s32 *outLinkFlag);
void player_setSpawnIndex(Actor *obj, s32 spawnIndex);
u8 player_getSpawnIndex(Actor *obj);
void player_moveToSpawn(Actor *obj, s32 spawnIndex, s32 unused);
extern const u8 D_8009D920[];
extern const u8 D_8009D938[];
extern const u8 D_8009D940[];
extern const u8 D_8009D944[];
extern const u8 D_8009D948[];
extern const u8 D_8009D94C[];
extern const u8 D_8009D950[];
extern const u8 gCharacterTypeGroupLengths[12];
extern s32 gTeam0SpawnMask;         /* team 0 spawn mask (bit per spawn point) */
extern s32 gTeam1SpawnMask;         /* team 1 spawn mask */
extern s32 gSpawnPointCount;         /* number of entries in gSpawnPoints */
extern SpawnPoint *gSpawnPoints; /* the level's spawn point table */

/* game/Player/PlayerStats/playerstats.c */
void player_setStat(s16 player, s16 stat, s32 value);
s32 player_getStat(s16 player, s16 stat);
void player_syncHealth(s16 players);
extern s8 gDefaultLives;      /* match default of stat 2 (lives) */
extern s8 gDefaultScore;      /* match default of stat 9 (score) */
extern s8 D_80087268;
extern s8 gDefaultHealth;      /* match default of stat 5 (health) */
extern s8 gDefaultCharacters[16];  /* per player: match default of stat 0 (character) */
extern u8 gModePlayerCounts[8];   /* indexed by gMatchState.mode */
extern s8 gSpawnGrenadeCount;
extern s16 gCarriedUnderwaterTime;
extern MatchState gMatchState;

/* game/Player/PlayerWarp/playerwarp.c */
extern s8 gPendingWarpLevels[16]; /* per player: level of the pending warp, -1 when none */
extern u8 gPlayerSpawnPoints[16]; /* per player: spawn point */
void player_resetWarps(s32 spawn0);
void player_cancelWarp(s32 player);
void player_respawnAt(Actor *actor, s32 spawnIndex);
void player_updateWarp(s32 player);
void player_startWarp(Actor *actor, s32 levelId, s32 spawnIndex);

#endif
