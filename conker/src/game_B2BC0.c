#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-player stats, read and written through func_150859AC / func_15085710. D_800D2138 is an s32
 * followed by an array of these at D_800D213C. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s8 unk8;   /* 0..100 */
    /* 0x09 */ s8 health; /* 0..99 */
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 padB;
    /* 0x0C */ s32 unkC;  /* >= 0 */
    /* 0x10 */ u8 pad10[0xC];
} PlayerStats;

extern PlayerStats D_800D213C[];

/* &D_800D213C[i] */
// matching: computed from D_800D2138 as an integer so the +4 is added before the base, not folded into it
#define PLAYER_STATS(i) ((PlayerStats *)((s32)&D_800D2138 + ((i) * (s32)sizeof(PlayerStats) + 4)))

/* Changes one of player `player`'s stats: 0/1/2/5/8/9 set unk0/unk4/unk8/health/unkA/unkC to
 * `value`; 3/6/10 add to unk8 (max 100), health (max 99) and unkC; 4/7/11 subtract from unk8,
 * health (min 0) and unkC (min 0). While D_8008FDBC bit 0 is set, setting unk8 sets 6 and
 * subtracting from it does nothing. */
void func_15085710(s16 player, s16 stat, s32 value) {
    PlayerStats *stats;

    if (player >= 0xFF) {
        return;
    }
    switch (stat) {
    case 0:
        D_800D213C[player].unk0 = value;
        break;
    case 1:
        D_800D213C[player].unk4 = value;
        break;
    case 2:
        if (D_8008FDBC & 1) {
            value = 6;
        }
        D_800D213C[player].unk8 = value;
        break;
    case 3:
        stats = PLAYER_STATS(player);
        stats->unk8 += value;
        stats->unk8 = (stats->unk8 > 100) ? 100 : stats->unk8;
        break;
    case 4:
        if (!(D_8008FDBC & 1)) {
            stats = PLAYER_STATS(player);
            stats->unk8 -= value;
        }
        break;
    case 5:
        D_800D213C[player].health = value;
        break;
    case 6:
        stats = PLAYER_STATS(player);
        stats->health += value;
        stats->health = (stats->health > 99) ? 99 : stats->health;
        break;
    case 7:
        stats = PLAYER_STATS(player);
        stats->health -= value;
        stats->health = (stats->health < 0) ? 0 : stats->health;
        break;
    case 8:
        D_800D213C[player].unkA = value;
        break;
    case 9:
        D_800D213C[player].unkC = value;
        break;
    case 10:
        if (!D_800BE616 || !D_800E0C20) {
            // matching: a block-scoped pointer leaves this case's delay slot unfilled
            PlayerStats *target = PLAYER_STATS(player);
            target->unkC += value;
        }
        break;
    case 11:
        stats = PLAYER_STATS(player);
        stats->unkC -= value;
        stats->unkC = (stats->unkC < 0) ? 0 : stats->unkC;
        break;
    }
}

/* Returns one of player `player`'s stats: 0 = bit `player` of D_800D2340,
 * 1..6 = unk0, unk4, unk8, health, unkA, unkC; 0 for player >= 0xFF. */
s32 func_150859AC(s16 player, s16 stat) {
    if (player >= 0xFF) {
        return 0;
    }
    switch (stat) {
    case 0:
        return (u16)D_800D2340 & (1 << player);
    case 1:
        return D_800D213C[player].unk0;
    case 2:
        return D_800D213C[player].unk4;
    case 3:
        return D_800D213C[player].unk8;
    case 4:
        return D_800D213C[player].health;
    case 5:
        return D_800D213C[player].unkA;
    case 6:
        return D_800D213C[player].unkC;
    }
}

/* Copies each object's health into the stats of the players (0..3) whose bit is set in `players`;
 * skipped while D_800D18A8 is set. */
void func_15085ABC(s16 players) {
    s32 i;

    if (D_800D18A8 != 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if ((1 << i) & players) {
            func_15085710(i, 5, movement_obj[i].energy);
        }
    }
}
