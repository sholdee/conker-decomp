#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order
 * it first sees each name (see init_50A0.c). */
s8 D_800D2130;
s16 D_800D2132;
MatchState D_800D2138;

#include "functions.h"
#include "variables.h"

/* This file's .data: the match defaults that game_447B0 and game_A9D90 pass to func_15085710. */
s8 D_80087260 = 100;   // stat 2 (unk8)
s8 D_80087264 = 0;     // stat 9 (unkC)
s8 D_80087268 = 0;
s8 D_8008726C = 6;     // stat 5 (health)
s8 D_80087270[16] = { 0 }; // per player: stat 0 (unk0)
u8 D_80087280[8] = { 1, 2, 2, 3, 4, 2, 3, 4 }; // indexed by D_800D2138.mode

/* &D_800D2138.players[i] */
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
        D_800D2138.players[player].unk0 = value;
        break;
    case 1:
        D_800D2138.players[player].unk4 = value;
        break;
    case 2:
        if (D_8008FDBC & 1) {
            value = 6;
        }
        D_800D2138.players[player].unk8 = value;
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
        D_800D2138.players[player].health = value;
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
        D_800D2138.players[player].unkA = value;
        break;
    case 9:
        D_800D2138.players[player].unkC = value;
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

/* Returns one of player `player`'s stats: 0 = bit `player` of D_800D2138.playerMask,
 * 1..6 = unk0, unk4, unk8, health, unkA, unkC; 0 for player >= 0xFF. */
s32 func_150859AC(s16 player, s16 stat) {
    if (player >= 0xFF) {
        return 0;
    }
    switch (stat) {
    case 0:
        return D_800D2138.playerMask & (1 << player);
    case 1:
        return D_800D2138.players[player].unk0;
    case 2:
        return D_800D2138.players[player].unk4;
    case 3:
        return D_800D2138.players[player].unk8;
    case 4:
        return D_800D2138.players[player].health;
    case 5:
        return D_800D2138.players[player].unkA;
    case 6:
        return D_800D2138.players[player].unkC;
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
