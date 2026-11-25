#include <ultra64.h>
#include "functions.h"
/* variables.h types D_800D212C as struct187 *, which lacks the signed link fields used here;
   shadow the declaration file-locally and use SpawnPoint instead. */
#define D_800D212C D_800D212C_struct187_decl_in_variables_h
#include "variables.h"
#undef D_800D212C

extern u8 D_8009D954[];
extern u8 *D_80087240[];

/* Looks obj->id up in the 7 rows of D_80087240 (row lengths in D_8009D954) and returns
 * the first entry of the row that contains it, or the id itself when no row does.
 * Same search as func_1503D5F0 (game_6A3D0). */
s32 func_15084D00(struct127 *obj) {
    s32 id = obj->id;
    s32 i;
    s32 j;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < D_8009D954[i]; j++) {
            if (id == D_80087240[i][j]) {
                return D_80087240[i][0];
            }
        }
    }
    return id;
}

/* A spawn point (0x18 bytes) in the D_800D212C table, D_800D2128 entries long. */
typedef struct {
    /* 0x00 */ s16 pos[3];
    /* 0x06 */ u8 unk6;   /* angle, 256 units per turn */
    /* 0x07 */ u8 facing; /* angle, 256 units per turn */
    /* 0x08 */ f32 unk8[3];
    /* 0x14 */ u8 unk14;
    /* 0x15 */ s8 unk15;
    /* 0x16 */ s8 link;   /* index of the spawn point to use instead; 0 = none, -1 = invalid */
    /* 0x17 */ u8 pad17;
} SpawnPoint;

/* Multiplayer score state (ScoreState in game_215960); only the field used here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x42];
    /* 0x42 */ s8 unk42;
} ScoreState;

extern SpawnPoint *D_800D212C;
extern ScoreState *D_8008FDD4;
extern s8 D_800E0BE3;

/* Copies spawn point `spawnIndex` (or, when it is -1, the player's stored one) into the out
 * parameters. In multiplayer (D_800BE616) with D_800D18A8 set, it instead picks the team's spawn
 * point farthest from the other team's players, plus a random term. When `useLink` is set, the
 * point's link is followed. Remembers the chosen index in the player's state; returns 1 when
 * there is no spawn table, else 0. */
s32 func_15084D70(s32 playerIndex, s32 spawnIndex, s32 useLink, s16 *outPos, f32 *outUnk8, u8 *outUnk6,
                  u8 *outFacing, s32 *outUnk14, s32 *outUnk15, s32 unused, s32 *outLinkFlag) {
    s32 remember;
    struct126 *state;
    s32 mask; // matching: also the player loop counter and link index; separate locals change regalloc
    s32 team;
    s32 best;
    s32 i;
    f32 nearestDistSq;
    f32 bestDistSq;
    f32 dx;
    f32 dz;
    s32 count;
    s32 lastSpawn;
    s32 j; // spawn loop counter, then the chosen spawn index

    remember = TRUE;
    if (D_800BE9F0 == 0x2B) {
        remember = FALSE;
        D_800D18A8 = 0;
    }
    if (D_800BE616 && D_8008FDD4->unk42 == 0 && D_800E0BE3 != 0) {
        remember = FALSE;
    }
    if (playerIndex != 0xFF) {
        if (spawnIndex == -1) {
            spawnIndex = D_800CC2D0[playerIndex].unk31C->pad11B[0];
        }
        if (remember) {
            D_800CC2D0[playerIndex].unk31C->pad11B[0] = 0;
        }
    }
    spawnIndex &= 0xFFF;
    if (spawnIndex >= D_800D2128 || spawnIndex < 0) {
        spawnIndex = 0;
    }
    if (D_800D212C != NULL) {
        if (D_800BE616 && D_800D18A8) {
            u8 candidates[32];

            /* the byte at 0x128 (inside struct127's pad128) is the player's team */
            team = *(u8 *)&D_800CC2D0[playerIndex].pad128;
            state = D_800CC2D0[playerIndex].unk31C;
            count = 0;
            best = 0;
            if (team == 0) {
                mask = D_800D2120;
            } else {
                mask = D_800D2124;
            }
            for (j = 0; j < D_800D2128; j++) {
                if ((1 << j) & mask) {
                    candidates[count] = j;
                    count++;
                }
            }
            bestDistSq = 0.0f;
            /* state byte 0x1B0 (past the end of struct126) is the last multiplayer spawn index */
            lastSpawn = *(s8 *)((u8 *)state + 0x1B0);
            for (i = 0; i < count; i++) {
                if (lastSpawn != candidates[i]) {
                    nearestDistSq = 100000000.0f;
                    for (mask = 0; mask < D_8008FD8C; mask++) {
                        if (team != *(u8 *)&D_800CC2D0[mask].pad128) {
                            dx = D_800CC2D0[mask].x_position - D_800D212C[candidates[i]].pos[0];
                            dz = D_800CC2D0[mask].z_position - D_800D212C[candidates[i]].pos[2];
                            dx = dx * dx + dz * dz;
                            if (dx < nearestDistSq) {
                                nearestDistSq = dx;
                            }
                        }
                    }
                    dz = func_150ADA68() * 500.0f;
                    nearestDistSq += dz * dz;
                    if (bestDistSq < nearestDistSq) {
                        best = i;
                        bestDistSq = nearestDistSq;
                    }
                }
            }
            if (D_8008FDD4->unk42 != 0 || D_800E0BE3 == 0) {
                spawnIndex = candidates[best];
            }
            state = D_800CC2D0[playerIndex].unk31C;
            *(s8 *)((u8 *)state + 0x1B0) = spawnIndex;
            useLink = FALSE;
        }
        j = spawnIndex;
        if (useLink && (mask = D_800D212C[spawnIndex].link) != 0) {
            j = mask;
            if (mask == -1 || mask >= D_800D2128) {
                j = spawnIndex;
            }
            if (outLinkFlag != NULL) {
                mask = D_800D212C[j].unk15;
                if (mask >= 0) {
                    *outLinkFlag = 0;
                } else if (mask == -1) {
                    *outLinkFlag = 0;
                } else {
                    *outLinkFlag = 1;
                }
            }
        } else if (useLink && D_800D212C[spawnIndex].link == 0) {
            D_800DDE38 = 2;
        }
        outUnk8[0] = D_800D212C[j].unk8[0];
        outUnk8[1] = D_800D212C[j].unk8[1];
        outUnk8[2] = D_800D212C[j].unk8[2];
        outPos[0] = D_800D212C[j].pos[0];
        outPos[1] = D_800D212C[j].pos[1];
        outPos[2] = D_800D212C[j].pos[2];
        *outUnk6 = D_800D212C[j].unk6;
        *outFacing = D_800D212C[j].facing;
        *outUnk14 = D_800D212C[j].unk14;
        *outUnk15 = D_800D212C[j].unk15;
        if (D_800BE616 && D_800D18A8 == 0) {
            f32 angle = *outFacing * 3.1415927410f * (1.0f / 128);
            f32 offset = func_150ADA68() * 80.0f - 40.0f;
            outPos[0] += sinf(angle) * offset;
            outPos[2] += cosf(angle) * offset;
        }
        if (playerIndex != 0xFF && remember) {
            D_800CC2D0[playerIndex].unk31C->pad11B[0] = spawnIndex;
        }
        return 0;
    }
    return 1;
}

/* Sets the spawn index func_15084D70 falls back to for this player object. */
void func_15085410(struct127 *obj, s32 spawnIndex) {
    obj->unk31C->pad11B[0] = spawnIndex;
}

/* Returns the player object's stored spawn index. */
u8 func_15085420(struct127 *obj) {
    return obj->unk31C->pad11B[0];
}

extern f32  func_1506AD30(struct127 *, f32, s32);
extern void func_1507C3E0(struct127 *, s16 *, s16 *, s16 *);
extern void func_1505D6F0(struct127 *, s32);
extern void func_15005818(struct108 *, struct127 *, f32 *);
extern void func_15169040(void *, u8);

/* Moves obj to spawn point `spawnIndex` (see func_15084D70): sets its position and angles,
 * clears its motion, places its camera and passes {obj, unique_id} to func_15169040. */
void func_15085430(struct127 *obj, s32 spawnIndex, s32 unused) {
    s16 pos[3];
    f32 unk8[3];
    u8 unk6;
    u8 facing;
    s32 unk15;
    s32 unk14;
    struct127 *savedObject;
    s32 savedIndex;
    struct {
        struct127 *obj;
        u8 uniqueId;
    } event;

    func_15084D70(obj->unk127, spawnIndex, D_800D18A8, pos, unk8, &unk6, &facing, &unk14, &unk15, unused, NULL);
    savedObject = D_800D154C;
    savedIndex = D_800C3E78;
    D_800D154C = obj;
    D_800C3E78 = obj - D_800CC2D0;
    func_1506AD30(obj, 800.0f, 1);
    D_800D154C = savedObject;
    D_800C3E78 = savedIndex;
    obj->x_position = pos[0];
    obj->y_position = pos[1];
    obj->z_position = pos[2];
    obj->old_x_position = obj->x_position;
    obj->old_y_position = *(volatile f32 *)&obj->y_position; // matching: stops IDO carrying y_position across the loop below
    obj->old_z_position = obj->z_position;
    obj->unk76 = obj->unk78 = obj->unk7A = facing << 8;
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * (360.0f / 65536);
    obj->unkB8 = (unk6 * 360) >> 8;
    while (obj->unkB8 > 180.0f) {
        obj->unkB8 -= 360.0f;
    }
    obj->unk28 = 0.0f;
    obj->unk180 = obj->y_position;
    if (obj->unk31C != NULL) {
        func_1507C3E0(obj, (s16 *)&obj->unk31C->unk114, (s16 *)&obj->unk31C->unk116, (s16 *)&obj->unk31C->unk118);
    }
    obj->unk1CC = obj->y_position;
    obj->unk2F8 &= ~0x10;
    obj->unk22C |= 0x200;
    obj->unk164 = 0.0f;
    obj->unk168 = 0.0f;
    obj->unk16C = 0.0f;
    obj->unk170 = 0.0f;
    if (obj->unk127 != 0xFF) {
        /* state byte 0x23 is inside struct126's pad17 */
        *((u8 *)obj->unk31C + 0x23) = unk15;
        if (D_800BE616 && D_800D18A8) {
            func_1505D6F0(obj, obj - D_800CC2D0);
        }
        if (obj->unk31C->unk78 == 0 && obj->unk31C->unk19B != 4 && obj->unk31C->unk19B != 7) {
            obj->unk83 = 0;
        }
    } else {
        obj->unk83 = 0;
    }
    if (obj->camera != NULL) {
        func_15005818(obj->camera, obj, unk8);
    }
    event.obj = obj;
    event.uniqueId = obj->unique_id;
    func_15169040(&event, 4);
}

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
        if (!D_800BE616 || !*(s8 *)&D_800E0C20) {
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
            func_15085710(i, 5, D_800CC2D0[i].health);
        }
    }
}
