#ifndef CONKER_MP_H
#define CONKER_MP_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "mp": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_B4080.c
 *   game_B7060.c
 */

/* A circular area registered with mp_addZone. */
typedef struct MpZone {
    /* 0x0 */ f32 radiusSq;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ s16 z;
    /* 0xA */ s16 padA;
} MpZone;

/*
 * Multiplayer game state (gMpGame), loaded from resources 0xB7..0xBE. Arrays are per player or
 * per slot. Some functions null-test arrays (grenades, zoneExitNode, attacker, ...) as if they were
 * pointers; the tests are always true but are kept, in the exact form that matches.
 */
typedef struct MpGame {
    /* 0x0000 */ s32 unk0;
    /* 0x0004 */ s32 unk4;
    /* 0x0008 */ s32 unk8;
    /* 0x000C */ s32 unkC;
    /* 0x0010 */ s32 numPlayers;
    /* 0x0014 */ s32 activeMask;
    /* 0x0018 */ s32 seenMask;
    /* 0x001C */ s32 unk1C[16];
    /* 0x005C */ s32 unk5C[16];
    /* 0x009C */ s32 layer[16]; /* height band, path_getLayerAt */
    /* 0x00DC */ s32 unkDC[16];
    /* 0x011C */ s32 target[16];              /* player each one is after, -1 = none */
    /* 0x015C */ s32 unk15C[16];
    /* 0x019C */ s32 unk19C[16];
    /* 0x01DC */ s32 unk1DC[16];
    /* 0x021C */ s32 aimTime[16];             /* frames spent on the current aim */
    /* 0x025C */ s32 unk25C[16];
    /* 0x029C */ s32 attackTimer[16];         /* fires when it turns non-negative */
    /* 0x02DC */ s32 unk2DC[16];
    /* 0x031C */ s32 unk31C[16];
    /* 0x035C */ s32 unk35C[16];
    /* 0x039C */ s32 unk39C[16];
    /* 0x03DC */ s32 attacker[16];            /* last hit by (| 0x80 when fresh), from D_800E0BB8 */
    /* 0x041C */ s32 unk41C[16];
    /* 0x045C */ s32 opponentDist[16];
    /* 0x049C */ s32 opponent[16];            /* chosen by mp_pickOpponentByPath */
    /* 0x04DC */ s32 grenades[16];
    /* 0x051C */ s32 nearestEnemyDist[16];
    /* 0x055C */ s32 zoneExitNode[16];
    /* 0x059C */ s32 unk59C[16];
    /* 0x05DC */ s32 targets[16][16];     /* per player: other players sorted by targetDists, -1 ends */
    /* 0x09DC */ s32 targetDists[16][16];
    /* 0x0DDC */ s32 teamScore[16];
    /* 0x0E1C */ u8 padE1C[0x48];
    /* 0x0E64 */ s32 team[16];
    /* 0x0EA4 */ s32 numSlots;
    /* 0x0EA8 */ s32 numSpawns;               /* entries in spawnNodes */
    /* 0x0EAC */ s32 respawnDelay;            /* slotTimer after a pickup */
    /* 0x0EB0 */ s32 slot[16];                /* slot held by each player, -1 = none */
    /* 0x0EF0 */ s32 slotEntity[32];          /* gfx_findObjectById id of each slot's pickup */
    /* 0x0F70 */ s32 slotOwner[32];           /* player holding each slot (| 0x2000 once taken), -1 = none */
    /* 0x0FF0 */ s32 itemOverride[32];        /* per player: 0 = none, 0x457 = no item */
    /* 0x1070 */ s32 slotTimer[32];           /* >0 counting down to respawn, 0 = available, -1 = held */
    /* 0x10F0 */ s32 slotSpawn[32];           /* spawnNodes index it sits at, -1 = none */
    /* 0x1170 */ s32 defaultItem;             /* item of a player without a slot */
    /* 0x1174 */ s32 slotItem[32];
    /* 0x11F4 */ s32 slotFlags[32];           /* 1 random spawn, 2 respawns instead of being held, 4 never moves */
    /* 0x1274 */ s32 slotHomeSpawn[32];
    /* 0x12F4 */ s32 slotDropTimer[32];       /* frames before a dropped pickup returns */
    /* 0x1374 */ s32 slotDropX[32];           /* where it was dropped */
    /* 0x13F4 */ s32 slotDropY[32];
    /* 0x1474 */ s32 slotDropZ[32];
    /* 0x14F4 */ s32 spawnNodes[32];          /* path node ids (mp_findNodeById) */
    /* 0x1574 */ s32 countdown[16];           /* frames, drawn in seconds in game type 0xB9 */
    /* 0x15B4 */ u8 weights[16][16];
    /* 0x16B4 */ f32 ticks;
    /* 0x16B8 */ f32 targetDistScale;
    /* 0x16BC */ s16 gameType; /* resource id */
    /* 0x16BE */ s16 pad16BE;
    /* 0x16C0 */ s32 sightBits[16];
    /* 0x1700 */ s8 losPlayer;
    /* 0x1701 */ s8 losOther;
    /* 0x1702 */ s8 nextBot;
    /* 0x1703 */ s8 nextHuman;
    /* 0x1704 */ s8 targetListPlayer;
    /* 0x1705 */ s8 sideGoal[16];             /* aim offset goals (mp_aimBot sideOffset/dropOffset) */
    /* 0x1715 */ s8 dropGoal[16];
    /* 0x1725 */ s8 side[16];                 /* eased toward the goals */
    /* 0x1735 */ s8 drop[16];
    /* 0x1745 */ s8 numZones;
    /* 0x1748 */ MpZone zoneList[8];
    /* 0x17A8 */ f32 aimX[16];                /* smoothed aim point */
    /* 0x17E8 */ f32 aimY[16];
    /* 0x1828 */ f32 aimZ[16];
    /* 0x1868 */ f32 aimRate[16];             /* aim easing per frame */
    /* 0x18A8 */ u8 pad18A8[0x24B8 - 0x18A8];
    /* 0x24B8 */ s32 unk24B8[9];              /* by unique_id - 20 (game type 0x3F), see mp_takeOverPlayer */
    /* 0x24DC */ s32 unk24DC[9];
} MpGame;

/* Flood fill over the path graph from a point, collecting the nodes just outside a radius. */
typedef struct PathSearch {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 z;
    /* 0x08 */ f32 radiusSq;
    /* 0x0C */ f32 dist[8];
    /* 0x2C */ s16 count;
    /* 0x2E */ u8 ids[8];
    /* 0x36 */ u8 visited[0x20]; /* bitset of visited node ids */
} PathSearch;

/* game_B4080.c */
f32 mp_nodeDistance(s32 a, s32 b);
void func_15086C68(void);
TriggerZone *mp_findNodeZone(s32 index);
s32 mp_getNodePos(s32 index, f32 *x, f32 *y, f32 *z);
/* (s32 id); declared without a prototype: game_215960.c passes a second argument */
s32 mp_findNodeById();
f32 mp_castWallRay(f32 x, f32 y, f32 z, f32 dx, f32 dz);
void mp_initRacers(s32 startId, s32 numRacers, s32 numSpare);
void mp_updateRacer(s32 index, s32 started, f32 lookahead);
void mp_updateRaceCameras(void);
void mp_setRacerRoute(s32 index, s32 route);
void mp_clampRacerAngle(s32 index, Actor *obj);
void mp_alignRacerToTrack(s32 index, Actor *obj);
void mp_bindRacer(s32 index, s32 actorIndex);
void mp_setRacerLateral(s32 index, s32 lateral);
s32 mp_endRace(Racer *racer, Actor *obj, s32 mode);
s32 mp_crossFinishLine(s32 index, s32 mode);
s32 mp_getRacerLateral(s32 index);
s32 mp_getRacerDistance(s32 index);
s32 mp_getRacerSpeed(s32 index);
s32 mp_getRacerLap(s32 index);
s32 mp_findRacerByRank(s32 index, s32 offset);
Actor *mp_findAdjacentRacer(s32 index, s32 dir, f32 *lateral, s32 *route);
s32 mp_findRacer(Actor *obj);
void mp_copyRacer(s32 src, s32 dst);
s32 mp_claimSpareRacer(Actor *obj);
void mp_releaseSpareRacer(Actor *obj);
s32 func_150887F8(void);
void mp_resetRacer(Racer *racer);
u8 mp_pickNextNode(u8 prev, u8 cur, s8 route);
s32 mp_advanceRacer(Racer *racer, f32 step);
void mp_placeAtStart(Actor *obj);
void mp_getSplinePoints(Racer *racer, f32 *outX, f32 *outY, f32 *outZ);
u8 mp_findNeighbour(u8 node, u8 type, u8 exclude, u8 param);
void mp_updateRaceOverCamera(Racer *racer, Actor *obj);
void mp_updateFlyThroughCamera(Racer *racer, Actor *obj, s32 leave);
extern Racer *gRacers; /* per-racer state of the race mode */
extern f32 gRaceTargetX;    /* camera target circling the actor until the start */
extern f32 gRaceTargetY;
extern f32 gRaceTargetZ;
extern s8 gRaceDirection;     /* race direction: 0 = towards higher node params */
extern s8 D_800D23A9;

/* game_B7060.c */
extern MpGame *gMpGame; /* the loaded multiplayer game resource (mp_loadGame) */
void mp_clearGame(void);
s32 mp_loadGame(void);
s32 mp_pickOpponentInRange(s32 player);
s32 mp_updateTargetList(void);
s32 mp_update(void);
s32 func_1508B194(s32 player);
void func_1508B1D4(s32 player);
void mp_addZone(f32 x, f32 y, f32 z, f32 r);
void mp_floodFillPath(u8 id, PathSearch *s);
void mp_assignZoneNodes(void);
s32 mp_pickOpponentByPath(void);
void mp_randomizeWeights(void);
s32 mp_pickTeamOpponents(void);
s32 func_1508C194(s32 player);
void mp_checkLineOfSight(s32 a, s32 b);
s32 mp_updateLineOfSight(s32 player, s32 checks);
s32 mp_nextBot(void);
s32 mp_nextHuman(void);
s32 mp_aimBot(s32 player, s32 target, s32 fire, s32 aimAtPoint, s32 sideOffset, s32 dropOffset);
s32 mp_updateBotAttack(s32 player);
s32 mp_resetSlots(void);
s32 mp_releaseSlot(s32 player, s32 mode);
s32 mp_updatePickups(void);
void func_1508E6C8(void);
s32 func_1508E6D0(s32 type);
s32 mp_getItemAttachSet(s32 type);
s32 mp_takeOverPlayer(s32 player, s32 source);
void mp_sendPlayerCommand(s32 player, s32 id, s32 param);
void func_1508EBF8(s32 player, s32 param);
void func_1508EC5C(s32 player, s32 param);
s32 mp_findLinkedEntry(u16 type, u16 id, u16 kind);
void mp_clearLinkGroup(s32 index);
void mp_fireLinkGroup(u16 type, u16 id);
extern f32 D_800872B0[5];
extern s16 gBotAimTimeLimits[5]; /* aim time limit, -1 = none */
extern s8 D_800872D0[5];
extern s8 D_800872D8[5];
extern s8 D_800872E0[5];
extern s8 D_800872E8[5]; /* percent chance */
extern s8 D_800872F0[5];
extern f32 gBotAimRates[5]; /* aim rate */
extern f32 D_8008730C[5]; /* not referenced by any code */
extern s16 gBotAttackTimeLimits[5][3]; /* [level][range] attack timer limit */

#endif
