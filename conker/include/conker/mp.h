#ifndef CONKER_MP_H
#define CONKER_MP_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "mp": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_B4080.c
 */

/* A circular area registered with func_1508B20C. */
typedef struct MpZone {
    /* 0x0 */ f32 radiusSq;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ s16 z;
    /* 0xA */ s16 padA;
} MpZone;

/*
 * Multiplayer game state (D_800D23B0), loaded from resources 0xB7..0xBE. Arrays are per player or
 * per slot. Some functions null-test arrays (grenades, unk55C, attacker, ...) as if they were
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
    /* 0x009C */ s32 layer[16]; /* height band, func_15085DA8 */
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
    /* 0x049C */ s32 opponent[16];            /* chosen by func_1508B9BC */
    /* 0x04DC */ s32 grenades[16];
    /* 0x051C */ s32 nearestEnemyDist[16];
    /* 0x055C */ s32 unk55C[16];
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
    /* 0x0EF0 */ s32 slotEntity[32];          /* func_151149AC id of each slot's pickup */
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
    /* 0x14F4 */ s32 spawnNodes[32];          /* path node ids (func_15086D48) */
    /* 0x1574 */ s32 countdown[16];           /* frames, drawn in seconds in game type 0xB9 */
    /* 0x15B4 */ u8 weights[16][16];
    /* 0x16B4 */ f32 ticks;
    /* 0x16B8 */ f32 unk16B8;
    /* 0x16BC */ s16 gameType; /* resource id */
    /* 0x16BE */ s16 pad16BE;
    /* 0x16C0 */ s32 unk16C0[16];
    /* 0x1700 */ s8 unk1700;
    /* 0x1701 */ s8 unk1701;
    /* 0x1702 */ s8 unk1702;
    /* 0x1703 */ s8 unk1703;
    /* 0x1704 */ s8 unk1704;
    /* 0x1705 */ s8 sideGoal[16];             /* aim offset goals (func_1508CAD8 sideOffset/dropOffset) */
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
    /* 0x24B8 */ s32 unk24B8[9];              /* by unique_id - 20 (game type 0x3F), see func_1508E89C */
    /* 0x24DC */ s32 unk24DC[9];
} MpGame;

#endif
