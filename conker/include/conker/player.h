#ifndef CONKER_PLAYER_H
#define CONKER_PLAYER_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "player": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/done/game_43B20.c
 *   game_447B0.c
 *   game_44A90.c
 *   game_B21B0.c
 *   game_B2BC0.c
 *   game_1A0100.c
 */

/* game_43B20.c */
void func_15016670(void);
void func_15016680(void);
void func_15016690(s32 arg0);

/* game_447B0.c */
void func_15017300(s16 arg0, s16 arg1);
void func_1501748C(s16 arg0);
void func_15017498(void);
void func_150174C0(s32 arg0);
void func_15017578(s32 arg0);

/* game_44A90.c */
void func_150175E0(void);
void func_15017640(void);

/* game_B21B0.c */
s32 func_15084D00(Actor *obj);
s32 func_15084D70(s32 playerIndex, s32 spawnIndex, s32 useLink, s16 *outPos, Vec3f *outUnk8, u8 *outUnk6,
                  u8 *outFacing, s32 *outUnk14, s32 *outUnk15, s32 unused, s32 *outLinkFlag);
void func_15085410(Actor *obj, s32 spawnIndex);
u8 func_15085420(Actor *obj);
void func_15085430(Actor *obj, s32 spawnIndex, s32 unused);
extern const u8 D_8009D920[];
extern const u8 D_8009D938[];
extern const u8 D_8009D940[];
extern const u8 D_8009D944[];
extern const u8 D_8009D948[];
extern const u8 D_8009D94C[];
extern const u8 D_8009D950[];
extern const u8 D_8009D954[12];
extern s32 D_800D2120;         /* team 0 spawn mask (bit per spawn point) */
extern s32 D_800D2124;         /* team 1 spawn mask */
extern s32 D_800D2128;         /* number of entries in D_800D212C */
extern SpawnPoint *D_800D212C; /* the level's spawn point table */

/* game_B2BC0.c */
void func_15085710(s16 player, s16 stat, s32 value);
s32 func_150859AC(s16 player, s16 stat);
void func_15085ABC(s16 players);
extern s8 D_80087260;      /* match default of stat 2 (unk8) */
extern s8 D_80087264;      /* match default of stat 9 (unkC) */
extern s8 D_80087268;
extern s8 D_8008726C;      /* match default of stat 5 (health) */
extern s8 D_80087270[16];  /* per player: match default of stat 0 (unk0) */
extern u8 D_80087280[8];   /* indexed by D_800D2138.unk0 */
extern s8 D_800D2130;
extern s16 D_800D2132;
extern MatchState D_800D2138;

/* game_1A0100.c */
extern s8 D_800DD2B0[16]; /* per player: level of the pending warp, -1 when none */
extern u8 D_800DD2C0[16]; /* per player: spawn point */
void func_15172C50(s32 spawn0);
void func_15172CA8(s32 player);
void func_15172D28(Actor *actor, s32 spawnIndex);
void func_15172D80(s32 player);
void func_15172E7C(Actor *actor, s32 levelId, s32 spawnIndex);

#endif
