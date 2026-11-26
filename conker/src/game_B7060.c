#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define VIEWS ((RenderView *)D_800BE628)

extern MpGame *D_800D23B0;





void func_15089BB0(void) {
    D_800D23B0 = NULL;
}

/* Per-difficulty bot tuning (indexed by D_800E0BD0): this file's .data, in the original order. */
f32 D_800872B0[5] = { 1.5f, 1.2f, 0.8f, 0.4f, 0.15f };
s16 D_800872C4[5] = { -1, -1, 600, 240, 0 }; // aim time limit, -1 = none
s8 D_800872D0[5] = { 80, 70, 60, 60, 45 };
s8 D_800872D8[5] = { 10, 20, 30, 60, 45 };
s8 D_800872E0[5] = { 120, 80, 40, 10, 0 };
s8 D_800872E8[5] = { 10, 20, 50, 80, 100 }; // percent chance
s8 D_800872F0[5] = { 30, 25, 20, 15, 15 };
f32 D_800872F8[5] = { 0.05f, 0.1f, 0.125f, 0.25f, 1.0f }; // aim rate
f32 D_8008730C[5] = { 0.0f, 0.025f, 0.05f, 0.075f, 0.0f }; // not referenced by any code
s16 D_80087320[5][3] = { // [level][range] attack timer limit
    { 900, 300, 80 }, { 600, 300, 80 }, { 420, 300, 80 }, { 360, 240, 60 }, { 300, 180, 60 },
};

/* Loads the multiplayer game resource D_800D2E48 and resets its per-player state. */
s32 func_15089BC0(void) {
    f32 *scales;
    s32 i;
    s32 id;
    void *resource;

    id = D_800D2E48;
    resource = func_1509B570(id);
    if (resource == NULL) {
        return 0;
    }
    // matching: identical separate case bodies, as in the original jump table
    switch (id) {
    case 0xB9:
        D_800D23B0 = resource;
        break;
    case 0xB8:
        D_800D23B0 = resource;
        break;
    case 0xBB:
        D_800D23B0 = resource;
        break;
    case 0xBA:
        D_800D23B0 = resource;
        break;
    case 0xB7:
        D_800D23B0 = resource;
        break;
    case 0xBD:
        D_800D23B0 = resource;
        break;
    case 0xBE:
        D_800D23B0 = resource;
        break;
    default:
        D_800D23B0 = NULL;
        return 0;
    }
    func_1508BC20();
    D_800D23B0->unk1700 = 0;
    D_800D23B0->unk1701 = 0;
    D_800D23B0->unk1703 = 0;
    D_800D23B0->unk1702 = 0;
    D_800D23B0->unk1704 = 0;
    D_800D23B0->gameType = id;
    D_800D23B0->unk16B8 = D_800872B0[D_800E0BD0];
    D_800D23B0->numZones = 0;
    scales = D_800872F8;
    for (i = 0; i < 16; i++) {
        D_800D23B0->unk16C0[i] = 0;
        D_800D23B0->aimRate[i] = scales[D_800E0BD0];
        D_800D23B0->aimX[i] = 0.0f;
        D_800D23B0->aimY[i] = 0.0f;
        D_800D23B0->aimZ[i] = 0.0f;
    }
    D_800D23B0->unkC = D_800872F0[D_800E0BD0];
    return 0;
}

/*
 * Picks the opponent `player` should go for: the one whose distance scores best on a sine
 * curve between a minimum and maximum range. Returns its index or -1.
 */
s32 func_15089F9C(s32 player) {
    f32 dx;
    f32 dz;
    f32 val;
    f32 dist;
    f32 minDist;
    f32 maxDist;
    f32 best;
    s32 mask;
    s32 count;
    s32 bit;
    s32 i;
    s32 bestIdx;
    Actor *other;
    Actor *self;
    s32 *states;
    s32 off;

    states = D_800D23B0->team;
    mask = D_800D23B0->activeMask;
    count = D_800D23B0->numPlayers;
    off = player * 4; // matching: dead store that IDO still schedules
    bestIdx = -1;
    if (D_800D23B0->unk15C[player] == 0) {
        minDist = 750.0f;
        maxDist = 3500.0f;
    } else {
        minDist = 1500.0f;
        maxDist = 8000.0f;
    }
    best = -1000000.0f;
    for (i = 0; i < count; i++) {
        bit = 1 << i;
        if (states[i] != 3 && (mask & bit) &&
            (D_800D23B0->unk16C0[player] & (bit << 16))) {
            other = &movement_obj[i];
            if (other != NULL) {
                self = &movement_obj[player];
                dx = self->xpos - other->xpos;
                dz = self->zpos - other->zpos;
                dist = sqrtf((dx * dx) + (dz * dz));
            } else {
                dist = 32000.0f;
            }
            if (maxDist < dist) {
                dist = maxDist;
            }
            if (dist < minDist) {
                dist = minDist;
            }
            val = sinf(((dist - minDist) * (6.283185482f / (maxDist - minDist))) + -1.570796371f) + 1.0f;
            if (best < val) {
                best = val;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}

/*
 * Advances to the next player (unk1704) and rebuilds its target list: the opposing players
 * sorted by a weighted distance (targets / targetDists rows).
 */
s32 func_1508A1BC(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 mask;
    s32 n;
    s32 dist;
    s32 myTeam;
    s32 cur;
    s32 myMask;
    s32 *active;
    s32 *layers;
    s32 *teams;
    u32 row;
    s32 count;
    s32 rawDist;
    s32 minDist;
    s32 *kinds;
    s32 *ids;
    s32 *dists;
    f32 dx;
    Actor *other;
    Actor *self;

    teams = D_800D23B0->team;
    mask = D_800D23B0->activeMask;
    count = D_800D23B0->numPlayers;
    kinds = D_800D23B0->unk15C;
    active = D_800D23B0->unk39C;
    layers = D_800D23B0->layer;
    ids = D_800D23B0->targets[0];
    dists = D_800D23B0->targetDists[0];
    D_800D23B0->unk1704++;
    if (D_800D23B0->unk1704 == D_800D23B0->numPlayers) {
        D_800D23B0->unk1704 = 0;
    }
    cur = D_800D23B0->unk1704;
    row = cur * 16;
    if (!((1 << cur) & mask)) {
        // matching: one-line loop (the -g3 line numbers steer the unrolled stores' order)
        for (i = 0; i < 16; i++) ids[row + i] = -1;
        return 0;
    }
    n = 0;
    minDist = 32000;
    myTeam = teams[cur];
    myMask = D_800D23B0->unk16C0[cur];
    for (i = 0; i < count; i++) {
        if (active[i] > 0 && myTeam != teams[i]) {
            other = &movement_obj[i];
            self = &movement_obj[cur];
            if (other != NULL) {
                dx = self->xpos - other->xpos;
                dist = sqrtf((dx * dx) + ((self->zpos - other->zpos) * (self->zpos - other->zpos)));
            } else {
                dist = 32000;
            }
            rawDist = dist;
            if (layers[cur] != layers[i]) {
                dist += 500;
            }
            if ((0x10000 << i) & myMask) {
                if (dist > 3000 && other->attach_char != 0) {
                    dist = -1;
                }
            } else {
                if (i >= D_8008FD90) {
                    dist *= 2;
                }
                if (D_800D23B0->gameType != 0xB8 && (kinds[i] == 2 || kinds[i] == 3)) {
                    dist = -1;
                }
            }
            if (D_800D23B0->gameType == 0xB7) {
                if (dist == -1) {
                    dist = rawDist;
                }
                if (other->lifting != 0) {
                    if (i >= D_8008FD90) {
                        dist *= 2;
                    }
                    dist = dist / (func_150859AC(i, 6) + 2);
                }
            }
            if (i < D_8008FD90 && dist != -1) {
                dist *= D_800D23B0->unk16B8;
            }
            if (dist >= 0) {
                /* insertion sort into this player's row; matching: the row + index forms fix addu operand order */
                j = 0;
                while (j < n && (dists + row)[j] < dist) {
                    j++;
                }
                for (k = n; k > j; k--) {
                    ids[(s32)row + k] = ids[(s32)row + k - 1];
                    dists[(s32)row + k] = dists[(s32)row + k - 1];
                }
                ids[row + j] = i;
                n++;
                dists[row + j] = dist;
            }
            if (rawDist < minDist) {
                minDist = rawDist;
            }
        }
    }
    for (i = n; i < 16; i++) ids[row + i] = -1; // matching: one-line loop, as above
    if (D_800D23B0->nearestEnemyDist) {
        D_800D23B0->nearestEnemyDist[cur] = minDist;
    }
    return 0;
}

/* Per-frame update of every player's multiplayer state; returns a mask of players that respawned. */
s32 func_1508A6FC(void) {
    s32 i;
    s32 bit;
    s32 isNew;
    s32 team;
    Actor *obj;
    s32 other;
    s32 type;
    s32 result;
    s32 pad1; // matching: unused stack slot in the original frame
    s32 old;
    s32 reset;
    Actor *tgt;
    s32 activeMask;
    s32 seenMask;
    s32 count;
    s32 *arr1C;
    s32 *teams;
    s32 *arr35C;
    s32 *arr31C;
    s32 *arr39C;
    s32 *arr19C;
    s32 *aimTimes;
    s32 *attackTimers;
    s32 *arr25C;
    s32 *arr41C;
    s32 *attackers;
    s32 *layers;
    s32 *teamScores;
    s32 *countdowns;
    u8 flag;
    s32 half;
    s32 j;
    s32 pad2; // matching: unused stack slot in the original frame
    s32 flags;
    RenderView *vp;

    result = 0;
    i = 0;
    activeMask = D_800D23B0->activeMask;
    seenMask = D_800D23B0->seenMask;
    count = D_800D23B0->numPlayers;
    teams = D_800D23B0->team;
    arr35C = D_800D23B0->unk35C;
    arr31C = D_800D23B0->unk31C;
    arr19C = D_800D23B0->unk19C;
    aimTimes = D_800D23B0->aimTime;
    attackTimers = D_800D23B0->attackTimer;
    arr25C = D_800D23B0->unk25C;
    arr41C = D_800D23B0->unk41C;
    attackers = D_800D23B0->attacker;
    teamScores = D_800D23B0->teamScore;
    countdowns = D_800D23B0->countdown;
    arr1C = D_800D23B0->unk1C;
    arr39C = D_800D23B0->unk39C;
    layers = D_800D23B0->layer;
    for (i = 0; i < count; i++) {
        bit = 1 << i;
        arr1C[i] = 0;
        isNew = 0;
        if (!(activeMask & bit)) {
            if (!(seenMask & bit)) {
                isNew = 1;
            }
            seenMask |= bit;
        }
        if ((activeMask & bit) || isNew) {
            obj = &movement_obj[i];
            team = teams[i];
            if (team != 0 && D_800D23B0->gameType == 0xB9) {
                obj->family = 1;
            }
            old = attackTimers[i];
            aimTimes[i] += D_800BE9E4;
            attackTimers[i] += D_800BE9E4;
            arr25C[i] -= D_800BE9E4;
            if (arr25C[i] <= 0) {
                arr25C[i] = 0;
            }
            arr41C[i] -= D_800BE9E4;
            if (arr41C[i] <= 0) {
                arr41C[i] = 0;
            }
            countdowns[i] -= D_800BE9E4;
            if (countdowns[i] < 0) {
                countdowns[i] = 0;
            }
            if (arr19C[i] == 3 && (arr25C[i] & 0x1F) >= 0x1D) {
                func_1508EC5C(i, 0x8000);
            }
            attackers[i] = D_800E0BB8[i];
            D_800E0BB8[i] &= 0x7F;
            if (D_800D23B0->grenades != NULL) {
                D_800D23B0->grenades[i] = obj->mv->numGrenades;
            }
            arr39C[i] = func_150859AC(i, 4);
            layers[i] = func_15085DA8(obj->ypos);
            if (arr39C[i] == 0 || isNew || obj->unk2FD != 0) {
                if (arr35C[i] == 0) {
                    if (obj->unk20F != 0xFF) {
                        D_800D23B0->unk59C[i] = obj->unk20F;
                    } else {
                        D_800D23B0->unk59C[i] = -1;
                        if (!attackers) { // matching: an extra use of attackers steers IDO's register allocation
                        }
                    }
                    arr35C[i] = 1;
                    arr25C[i] = 0;
                    arr19C[i] = 0;
                    D_800D23B0->unk5C[i] = 1;
                    D_800D23B0->target[i] = -1;
                    teamScores[team]--;
                    if ((attackers[i] & 0x7F) < count) {
                        other = teams[attackers[i] & 0x7F];
                        if (team != other) {
                            teamScores[other]++;
                        }
                    }
                    if (D_800D23B0->gameType != 0xB9) {
                        D_800D23B0->itemOverride[i] = 0;
                        func_1508DAEC(i, 1);
                    }
                    result |= bit;
                }
            } else {
                arr35C[i] = 0;
            }
            flag = 1;
            if (arr31C[i] != 0 && D_800D23B0->gameType != 0xB8) {
                reset = 0;
                if (arr19C[i] != 1 && aimTimes[i] < 30) {
                    type = obj->intelwalk;
                    obj->unk222 = arr31C[i] - 2;
                    if (type != 4 && type != 5) {
                        func_1508EBF8(i, 4);
                    }
                    if (type == 4) {
                        aimTimes[i] = 0;
                    }
                    if (type == 5) {
                        aimTimes[i] = 30;
                        D_800D23B0->aimRate[i] = D_800872F8[D_800E0BD0];
                        D_800D23B0->aimX[i] = (func_150ADA68() * 50.0f) + obj->xpos;
                        D_800D23B0->aimY[i] = obj->ypos + 40.0f;
                        D_800D23B0->aimZ[i] = (func_150ADA68() * 50.0f) + obj->zpos;
                        if ((arr31C[i] - 2) >= 2) {
                            tgt = &movement_obj[arr31C[i] - 2];
                            D_800D23B0->aimX[i] += (tgt->xpos - obj->xpos) * 0.5f;
                            D_800D23B0->aimZ[i] += (tgt->zpos - obj->zpos) * 0.5f;
                        }
                    }
                }
                flag = 0;
                bit = 0;
                if (D_800872D0[D_800E0BD0] < attackTimers[i]) {
                    attackTimers[i] = 0;
                    old = -1;
                }
                if (attackTimers[i] >= 0 && old < 0) {
                    bit = 1;
                }
                if (D_800872C4[D_800E0BD0] != -1 && aimTimes[i] >= D_800872C4[D_800E0BD0]) {
                    reset = 1;
                }
                D_800D23B0->side[i] += (D_800D23B0->sideGoal[i] - D_800D23B0->side[i]) >> 2;
                D_800D23B0->drop[i] += (D_800D23B0->dropGoal[i] - D_800D23B0->drop[i]) >> 2;
                if (func_1508CAD8(i, arr31C[i] - 2, bit, reset, D_800D23B0->side[i], D_800D23B0->drop[i]) && arr19C[i] != 1) {
                    aimTimes[i] = 0;
                }
                if (attackTimers[i] == 0) {
                    D_800D23B0->sideGoal[i] = 0;
                    D_800D23B0->dropGoal[i] = 0;
                    if (D_800872E8[D_800E0BD0] < (s32)(func_150ADA68() * 100.0f)) {
                        if (func_150ADA68() > 0.5f) {
                            D_800D23B0->sideGoal[i] = -50;
                        } else {
                            D_800D23B0->sideGoal[i] = 50;
                        }
                        if (func_150ADA68() > 0.5f) {
                            D_800D23B0->dropGoal[i] = 50;
                        } else {
                            D_800D23B0->dropGoal[i] = 0;
                        }
                    }
                }
            }
            if (flag && D_800D23B0->gameType != 0xB8) {
                attackTimers[i] = -D_800872E0[D_800E0BD0];
            }
            flags = obj->unk22C & 0x200;
            obj->unk22C &= ~0x200;
            if (flags || (attackers[i] & 0x80)) {
                arr19C[i] = 0;
                arr1C[i] = 1;
            }
            if (obj->mv->on_fire != 0) {
                arr25C[i] = 20;
            }
        } else {
            arr39C[i] = 0;
            layers[i] = -1;
        }
    }

    D_800D23B0->seenMask = seenMask;
    if (D_800BE9E4 != 0) {
        D_800D23B0->ticks = D_800BE9E4;
    } else {
        D_800D23B0->ticks = 1.0f;
    }
    if (D_800D23B0->unk55C != NULL) {
        func_1508B3F8();
    }
    if (D_800D23B0->gameType == 0xB9) {
        half = (s32)(VIEWS[0].right - VIEWS[0].left) >> 1;
        func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
        for (j = 0; j <= D_80082FA0; j++) {
            if (movement_obj[j].team == 0) {
                vp = &VIEWS[j];
                func_15042D94(vp->left + half, vp->top + 6.0f, 1, "%d", countdowns[j] / 60);
            }
        }

    }
    return result;
}

/* Returns player's score record value (0 for a non-human player). */
s32 func_1508B194(s32 player) {
    if (player >= D_8008FD90) {
        return 0;
    }

    return D_8008FDD4->stats[player].unkA;
}

/* Clears player's score record value. */
void func_1508B1D4(s32 player) {
    if (player < D_8008FD90) {
        D_8008FDD4->stats[player].unkA = 0;
    }
}

/* Registers a zone of radius r around (x, y, z), up to 8. */
void func_1508B20C(f32 x, f32 y, f32 z, f32 r) {
    s32 n;

    if (D_800D23B0 != NULL && D_800D23B0->numZones < 8) {
        n = D_800D23B0->numZones++;
        D_800D23B0->zoneList[n].x = x;
        D_800D23B0->zoneList[n].y = y;
        D_800D23B0->zoneList[n].z = z;
        D_800D23B0->zoneList[n].radiusSq = r * r;
    }
}

/*
 * Flood-fills the path graph from node `id`, marking visited nodes; nodes farther than
 * s->radiusSq from (s->x, s->z) end the walk and are collected (up to 8) with their squared distance.
 */
void func_1508B2A8(u8 id, PathSearch *s) {
    PathNode *node;
    f32 dx;
    f32 dist;
    s32 j;
    u8 link;

    /* BUG (original game): the bit index should be id & 7, so ids 8 apart share a visited bit */
    s->visited[id >> 3] |= 1 << (id & 3);
    node = &D_800D2350[id];
    dx = node->x - s->x;
    dist = node->z - s->z;
    dist = (dx * dx) + (dist * dist);
    if (s->radiusSq < dist) {
        if (s->count < 8) {
            s->ids[s->count] = id;
            s->dist[s->count] = dist;
            s->count++;
        }
    } else {
        for (j = 0; j < 5; j++) {
            if (node->links[j] != 0xFF) {
                link = node->links[j];
                if (!(s->visited[link >> 3] & (1 << (link & 3)))) {
                    func_1508B2A8(node->links[j], s);
                }
            }
        }
    }
}

/*
 * For each registered zone, assigns every computer player standing in it the nearest
 * path node reachable from the zone centre (unk55C), then clears the zone list.
 */
void func_1508B3F8(void) {
    s32 i;
    s32 j;
    s32 found;
    s32 k;
    u8 best;
    s32 startNode;
    s32 *zone;
    Actor *obj;
    f32 x;
    f32 y;
    f32 z;
    f32 dx;
    s32 layer;
    f32 d;
    MpZone *list;
    PathSearch s;
    f32 bestDist;
    PathNode *node;

    zone = D_800D23B0->unk55C;
    list = D_800D23B0->zoneList;
    for (i = 0; i < D_8008FD8C; i++) {
        if (zone[i] != -1) {
            zone[i] = -2;
        }
    }
    for (i = 0; i < D_800D23B0->numZones; i++) {
        found = 0;
        s.radiusSq = list[i].radiusSq;
        x = list[i].x;
        y = list[i].y;
        z = list[i].z;
        for (j = D_8008FD90; j < D_8008FD8C; j++) {
            obj = &movement_obj[j];
            d = obj->ypos - y;
            if (d < 200.0f && d > -100.0f) {
                dx = obj->xpos - x;
                d = obj->zpos - z;
                if (dx * dx + d * d < s.radiusSq + 100.0f) {
                    bestDist = 100000000.0f;
                    best = 0xFF;
                    if (!found) {
                        s.count = 0;
                        layer = func_15085DA8(y);
                        found = 1;
                        startNode = func_15085DF8(x, y, z, 0, layer);
                        if (startNode != -1) {
                            s.x = x;
                            s.z = z;
                            s.count = 0;
                            bzero(s.visited, 0x20);
                            func_1508B2A8(startNode, &s);
                        }
                    }
                    for (k = 0; k < s.count; k++) {
                        node = &D_800D2350[s.ids[k]];
                        dx = node->x - obj->xpos;
                        d = node->z - obj->zpos;
                        d = dx * dx + d * d;
                        if (d < s.dist[k] && d < bestDist) {
                            best = s.ids[k];
                            bestDist = d;
                        }
                    }
                    if (best != 0xFF) {
                        node = &D_800D2350[best];
                        if (zone[j] != -2) {
                            D_800D23B0->unk5C[j] = 1;
                        }
                        zone[j] = node->unk7;
                    }
                }
            }
        }
    }
    for (i = 0; i < D_8008FD8C; i++) {
        if (zone[i] < 0) {
            zone[i] = -1;
        }
    }
    D_800D23B0->numZones = 0;
}

/*
 * Picks the best opponent for player unk4: the active player with the lowest path cost to it,
 * scaled by weights[unk4][] / 255 (and 0.8 for its current target). Stores it in opponent and
 * the distance in opponentDist. Returns 0.
 */
s32 func_1508B9BC(void) {
    s32 mode;
    s32 count;
    s32 *targets;
    u8 *weights;
    s32 winner;
    s32 minimum;
    s32 index;
    s32 cost;
    f32 distance;
    s32 current;
    s32 end;
    s32 *active;
    s32 *neighbors;
    s32 *costs;
    s32 cursor;
    f32 value;

    mode = 1;
    count = D_800D23B0->numPlayers;
    current = D_800D23B0->unk4;
    active = D_800D23B0->unk39C;
    neighbors = D_800D23B0->targets[0];
    costs = D_800D23B0->targetDists[0];
    targets = D_800D23B0->target;
    if (count >= 9) {
        mode = 4;
    }
    func_1508C5B8(0, mode);
    func_1508A1BC();
    weights = D_800D23B0->weights[current];
    winner = -1;
    minimum = 10000000;
    for (index = 0; index < count; index++) {
        if (active[index] > 0) {
            cost = -1;
            if (index != current) {
                cursor = index * 16;
                end = cursor + 16;
                while (cursor < end && neighbors[cursor] != -1) {
                    if (neighbors[cursor] == current) {
                        cost = costs[cursor];
                        cursor = end;
                    } else {
                        cursor++;
                    }
                }
                if (cost != -1) {
                    value = weights[index] * (1.0f / 255.0f);
                    if (index == targets[current]) {
                        value *= 0.8f;
                    }
                    cost = cost * value;
                    if (cost < minimum) {
                        minimum = cost;
                        winner = index;
                    }
                }
            }
        }
    }
    D_800D23B0->opponent[current] = winner;
    if (winner != -1) {
        f32 dx;
        f32 dz;
        Actor *actor;
        Actor *other;

        actor = &movement_obj[current];
        other = &movement_obj[winner];
        dx = actor->xpos - other->xpos;
        dz = actor->zpos - other->zpos;
        distance = sqrtf(dx * dx + dz * dz);
        D_800D23B0->opponentDist[current] = (s32)distance;
    } else {
        D_800D23B0->opponentDist[current] = minimum;
    }
    return 0;
}

/*
 * Fills each player's row of weights with a random triangle wave between 0x55 and 0xFF,
 * starting from a random unused column.
 */
void func_1508BC20(void) {
    s32 i;
    f32 fn;
    s32 pos;
    s32 val;
    s32 dir;
    s32 delta;
    s32 step;
    s32 h;
    u8 *row;
    s8 n;
    s8 used[16];

    n = D_8008FD8C;
    row = D_800D23B0->weights[0];
    h = n >> 1;
    if (h < 2) {
        h = 2;
    }
    step = 0xAA / h;
    fn = n;
    for (i = 0; i < n; i++) {
        for (h = 0; h < 16; h++) used[h] = 0;
        dir = -1;
        val = 0x55;
        delta = step;
        pos = func_150ADA68() * fn;
        if (func_150ADA68() > 0.5f) {
            dir = 1;
        }
        while (used[pos] != 0) {
            pos += dir;
            if (pos < 0) {
                pos = n - 1;
            }
            if (pos >= n) {
                pos = 0;
            }
        }
        used[pos] = 1;
        for (h = 0; h < n; h++) {
            if (val < 0x100) {
                row[pos] = val;
            } else {
                row[pos] = 0xFF;
            }
            pos++;
            if (pos >= n) {
                pos = 0;
            }
            val += delta;
            if (val >= 0x100) {
                delta = -delta;
            }
        }
        row += 0x10;
    }
}

/*
 * For every player on unk4's team, picks the reachable opponent with the lowest target
 * distance, doubling the cost per teammate already on it (kinds 2/3 choose first).
 */
s32 func_1508BF14(void) {
    s32 numPlayers;
    s32 myTeam;
    s32 pass;
    s32 myIdx;
    s32 i;
    s32 j;
    s32 end;
    s32 cost;
    s32 bestCost;
    s8 counts[16];
    s32 *teams;
    s32 *links;
    s32 *costs;
    s32 *kinds;
    s32 *active;
    s32 *bestCosts;
    s32 *bestSlots;
    s32 bestSlot;
    s32 n;

    myIdx = D_800D23B0->unk4;
    numPlayers = D_800D23B0->numPlayers;
    kinds = D_800D23B0->unk15C;
    active = D_800D23B0->unk39C;
    teams = D_800D23B0->team;
    bestCosts = D_800D23B0->opponentDist;
    bestSlots = D_800D23B0->opponent;
    links = D_800D23B0->targets[0];
    costs = D_800D23B0->targetDists[0];
    myTeam = teams[myIdx];

    for (i = 0; i < numPlayers; i++) counts[i] = 0;

    for (pass = 0; pass != 2; pass++) {
        for (i = 0; i < numPlayers; i++) {
            n = 0;
            if (kinds[i] == 2 || kinds[i] == 3) {
                n = 1;
            }
            if ((n && pass == 0) || (!n && pass != 0)) {
                if (teams[i] == myTeam) {
                    bestSlot = -1;
                    bestCost = 10000000;
                    for (j = 0; j < numPlayers; j++) {
                        if (active[j] > 0) {
                            if (teams[j] != myTeam) {
                                cost = -1;
                                n = j * 16;
                                end = n + 16;
                                while (n < end && links[n] != -1) {
                                    if (links[n] == i) {
                                        cost = costs[n];
                                        n = end;
                                    } else {
                                        n++;
                                    }
                                }
                                if (cost != -1) {
                                    cost <<= counts[j];
                                    if (cost < bestCost) {
                                        bestCost = cost;
                                        bestSlot = j;
                                    }
                                }
                            }
                        }
                    }
                    counts[bestSlot]++;
                    bestCosts[i] = bestCost;
                    bestSlots[i] = bestSlot;
                }
            }
        }
    }
    return 0;
}

s32 func_1508C194(s32 player) {
    return 0;
}

/* Line-of-sight test between players a and b; sets their visibility bits in unk16C0. */
void func_1508C1A4(s32 a, s32 b) {
    s32 blocked;
    s32 seen;
    Actor *from;
    Actor *to;
    s32 fromFlag;
    s32 toFlag;
    ActorCtl *fromInner;
    f32 distance;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 hitDistance;
    f32 x0;
    f32 x1;
    f32 y0;
    f32 y1;
    f32 z0;
    f32 z1;

    seen = 0;
    blocked = 0;
    from = func_1505EEF4(a & 0xFFFF0FFF);
    to = func_1505EEF4(b & 0xFFFF0FFF);
    if (from != NULL && to != NULL) {
        x0 = from->xpos;
        z0 = from->zpos;
        fromInner = from->mv;
        fromFlag = fromInner->bot;
        if (fromFlag) {
            y0 = from->ypos + fromInner->height * 0.75f;
            if ((fromInner->context_b & 0x7F) == 9) {
                y0 -= 50.0f;
            }
        } else if (from->type == 0x1F) {
            y0 = from->ypos + 80.0f;
        } else if (to->mv->bot && D_800872C4[D_800E0BD0] != -1) {
            /* 0x1A4..0x1A9 is an s16 point; structs.h types it as u16 pad1A4/s16 unk1A6/u32 pad1A8 */
            x0 = from->aim.x;
            y0 = from->aim.y;
            z0 = from->aim.z;
        } else {
            y0 = from->ypos + 40.0f;
        }
        x1 = to->xpos;
        z1 = to->zpos;
        toFlag = to->mv->bot;
        if (toFlag) {
            y1 = to->ypos + to->mv->height * 0.75f;
            if ((to->mv->context_b & 0x7F) == 9) {
                y1 -= 50.0f;
            }
        } else if (to->type == 0x1F) {
            y1 = to->ypos + 80.0f;
        } else if (fromFlag && D_800872C4[D_800E0BD0] != -1) {
            x1 = to->aim.x;
            y1 = to->aim.y;
            z1 = to->aim.z;
        } else {
            y1 = to->ypos + 40.0f;
        }
        distance = sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) +
                         (z1 - z0) * (z1 - z0));
        if (func_150AC9C0(x0, y0, z0, x1 - x0, y1 - y0, z1 - z0, NULL, NULL, &hitX,
                          &hitY, &hitZ, &hitDistance, NULL, NULL, 0.0f) == 0) {
            blocked = 1;
        } else if (distance < hitDistance) {
            blocked = 1;
        }
        seen = blocked;
        if (blocked && func_15086D94(x0, y0, z0, x1 - x0, z1 - z0) > 0.0f) {
            blocked = 0;
        }
    }
    if (blocked) {
        D_800D23B0->unk16C0[a] |= 1 << b;
        D_800D23B0->unk16C0[b] |= 1 << a;
    }
    if (seen) {
        D_800D23B0->unk16C0[a] |= 0x10000 << b;
        D_800D23B0->unk16C0[b] |= 0x10000 << a;
    }
}

/*
 * Refreshes the line-of-sight bits (unk16C0) between players: if `checks` is nonzero,
 * each human against its three nearest opponents, then `checks` round-robin pairs.
 * Returns player's bits.
 */
s32 func_1508C5B8(s32 player, s32 checks) {
    u8 cur;
    u8 other;
    s32 i;
    s32 m;
    s32 j;
    s32 k;
    f32 x0;
    f32 x1;
    f32 dx;
    f32 dz;
    u8 team;
    Actor *obj;
    s8 idx[4];
    f32 dist[4];
    s32 *list;
    s32 mask;

    cur = D_800D23B0->unk1700;
    other = D_800D23B0->unk1701;
    list = D_800D23B0->team;
    if (checks != 0) {
        for (i = 0; i < D_8008FD90; i++) {
            x0 = movement_obj[i].xpos;
            x1 = movement_obj[i].xpos;
            for (k = 0; k < 3; k++) {
                idx[k] = -1;
                dist[k] = 900000000.0f;
            }
            for (k = D_8008FD90; k < D_8008FD8C; k++) {
                if (list[i] != list[k] &&
                    !(D_800D23B0->unk16C0[i] & (0x10001 << k))) {
                    /* BUG (original game): measures player i against itself, not k */
                    dx = x0 - movement_obj[i].xpos;
                    dz = x1 - movement_obj[i].zpos;
                    if (i == D_800D23B0->target[k]) {
                        dx *= 0.5f;
                        dz *= 0.5f;
                    }
                    dx = dx * dx + dz * dz;
                    j = 2;
                    while (j >= 0 && dx < dist[j]) {
                        dist[j + 1] = dist[j];
                        idx[j + 1] = idx[j];
                        idx[j] = k;
                        dist[j] = dx;
                        j--;
                    }
                }
            }
            for (k = 0; k < 3; k++) {
                if (idx[k] != -1) {
                    j = idx[k];
                    D_800D23B0->unk16C0[i] &= ~(0x10001 << j);
                    D_800D23B0->unk16C0[j] &= ~(0x10001 << i);
                    func_1508C1A4(i, j);
                }
            }
        }
    }
    if (checks != 0) {
        obj = &movement_obj[cur];
        do {
            team = obj->team;
            m = 0;
            do {
                other++;
                if (other >= D_8008FD8C) {
                    cur++;
                    if (cur >= D_8008FD8C - 1) {
                        cur = 0;
                    }
                    obj = &movement_obj[cur];
                    team = obj->team;
                    other = cur + 1;
                }
                D_800D23B0->unk16C0[cur] &= ~(0x10001 << other);
                D_800D23B0->unk16C0[other] &= ~(0x10001 << cur);
                mask = (1 << cur) | (1 << other);
                m++;
            } while (m < 64 && ((D_800D18A0 & mask) ||
                                    team == movement_obj[other].team));
            func_1508C1A4(cur, other);
        } while (--checks != 0);
    }
    D_800D23B0->unk1700 = cur;
    D_800D23B0->unk1701 = other;
    return D_800D23B0->unk16C0[player];
}

/*
 * Advances unk1702 round-robin over the D_8008FD8C actors; returns the next active one with
 * mv->bot set that isn't in D_800D18A0, or -1.
 */
s32 func_1508C9CC(void) {
    s32 count;
    s32 ret;
    s8 cur;
    Actor *obj;
    ActorCtl *state;

    count = 0;
    ret = -1;
    cur = D_800D23B0->unk1702;
    do {
        cur++;
        if (cur >= D_8008FD8C) {
            cur = 0;
        }
        obj = &movement_obj[cur];
        if (obj->type != 0) {
            state = obj->mv;
            if (state != NULL && state->bot != 0 && !(D_800D18A0 & (1 << cur))) {
                count = D_8008FD8C;
                ret = cur;
            }
        }
        count++;
    } while (count < D_8008FD8C);

    D_800D23B0->unk1702 = cur;
    return ret;
}

/* Advances unk1703 to the next human player and returns it. */
s32 func_1508CA88(void) {
    MpGame **p2; // matching: reading back through a pointer to D_800D23B0
    s8 temp;

    p2 = &D_800D23B0;
    D_800D23B0->unk1703++;
    temp = D_800D23B0->unk1703;
    if (temp >= D_8008FD90) {
        D_800D23B0->unk1703 = 0;
        temp = (*p2)->unk1703;
    }
    return temp;
}

/*
 * Steers bot `player` toward player `target`: eases the smoothed aim point (aimX/aimY/aimZ)
 * toward the target (its position, or its 0x1A4 point if aimAtPoint), offset sideways by
 * sideOffset and down by dropOffset, turns the actor's yaw/pitch (or the camera pad's stick)
 * to it and presses Z_TRIG/R_TRIG when it should fire. Returns 1 while the yaw error exceeds 45 degrees.
 */
s32 func_1508CAD8(s32 player, s32 target, s32 fire, s32 aimAtPoint, s32 sideOffset, s32 dropOffset) {
    Actor *victim;
    f32 dz;
    f32 dx;
    f32 dy;
    f32 V;
    f32 scale;
    f32 ang;
    f32 curYaw;
    f32 diff;
    f32 roll;
    f32 h;
    f32 dist;
    f32 mag;
    f32 yaw;
    f32 *ptr;
    f32 ratio;
    f32 height;
    s32 flagA;
    s32 doInner;
    s32 mode;
    f32 cur;
    Actor *obj;
    s32 type;
    s32 hasCam;
    s32 result;
    Camera *cam;
    s32 pad; // matching: unused stack slot in the original frame

    doInner = 1;
    result = 0;
    obj = &movement_obj[player];
    ratio = VIEWS[0].fovY / VIEWS[0].fovX;
    if (target != -1) {
        cam = obj->CameraPtr;
        if (cam != NULL) {
            hasCam = 1;
        } else {
            hasCam = 0;
        }
        type = obj->mv->rt_context_b;
        if (type == 9 || type == 0x38 || type == 0x15) {
            hasCam = 0;
        }
        victim = func_1505EEF4(target & 0xFFFF0FFF);
        if (victim == NULL) {
            return 1;
        }
        flagA = 0;
        mode = 0;
        if (type == 0x24 || type == 0x18 || type == 0x23 || type == 0x41) {
            flagA = 1;
        } else if (type == 9) {
            mode = 1;
        } else if (type == 0x15 || type == 0x1B || type == 3) {
            mode = 1;
            if (type != 0x1B) {
                mode = 3;
            }
            doInner = 0;
        } else if (type == 0x38) {
            mode = 2;
        }
        if (type == 3) {
            aimAtPoint = 0;
        }
        height = 40.0f;
        if (victim->type == 0x1F) {
            aimAtPoint = 0;
            height = 80.0f;
        }
        dx = 0.0f;
        dz = 0.0f;
        if (D_800D23B0->gameType == 0xB9 || D_800E0BD0 >= 3) {
            switch (obj->mv->rt_context_b) {
                case 0x18:
                case 0x41:
                    V = 0.01600000076f;
                    break;
                case 0x14:
                case 0x3F:
                    V = 0.02476780117f;
                    break;
                case 0x1B:
                    V = 0.01600000076f;
                    break;
                case 0x16:
                    V = 0.03200000152f;
                    break;
                default:
                    V = 0.0f;
                    break;
            }
            if (V != 0.0f) {
                dx = victim->xpos - obj->xpos;
                dz = victim->zpos - obj->zpos;
                dy = victim->ypos - obj->ypos;
                dy = sqrtf((dx * dx) + (dy * dy) + (dz * dz));
                V = (dy * V) / D_800D23B0->ticks;
                dx = (victim->xpos - victim->old_xpos) * V;
                dz = (victim->zpos - victim->old_zpos) * V;
            }
        }
        if (aimAtPoint == 0) {
            dx = (victim->xpos + dx) - D_800D23B0->aimX[player];
            dz = (victim->zpos + dz) - D_800D23B0->aimZ[player];
        } else {
            dx = ((f32)victim->aim.x + dx) - D_800D23B0->aimX[player];
            dz = ((f32)victim->aim.z + dz) - D_800D23B0->aimZ[player];
        }
        scale = D_800D23B0->aimRate[player] * D_800BE9E4;
        if (type == 3) {
            scale = 1.0f;
            dropOffset = 0;
            sideOffset = 0;
        }
        if (scale > 1.0f) {
            scale = 1.0f;
        }
        D_800D23B0->aimX[player] += dx * scale;
        D_800D23B0->aimZ[player] += dz * scale;
        dx = D_800D23B0->aimX[player] - obj->xpos;
        dz = D_800D23B0->aimZ[player] - obj->zpos;
        dist = sqrtf((dx * dx) + (dz * dz));
        if (dist < 1.0f) {
            dist = 1.0f;
        } else {
            f32 tx;

            if ((dist < 100.0f || type == 3) && dropOffset != 0) {
                dropOffset = 0;
                if (sideOffset == 0) {
                    sideOffset = 50;
                }
            }
            mag = sideOffset;
            if (type == 9) {
                mag = sideOffset * 1.25f;
            }
            mag = mag / dist;
            tx = dx;
            dx -= dz * mag;
            dz += tx * mag;
        }
        ang = func_1505A630(dx, -dz, 0) * 0.005493164f;
        if (type != 3) {
            curYaw = obj->mv->aimYaw;
        } else {
            curYaw = obj->theta - 90.0f;
        }
        ang = func_15144BC8(ang);
        curYaw = func_15144BC8(curYaw);
        diff = func_15144BC8(curYaw - ang);
        if (diff > 180.0f) {
            diff = -360.0f + diff;
        }
        if (diff > 45.0f || diff < -45.0f) {
            result = 1;
        }
        if (aimAtPoint == 0) {
            cur = ((victim->ypos + height) - dropOffset) - D_800D23B0->aimY[player];
        } else {
            cur = ((f32)victim->aim.y - (f32)dropOffset) - D_800D23B0->aimY[player];
        }
        D_800D23B0->aimY[player] += cur * scale;
        cur = obj->mv->aimEye.y - D_800D23B0->aimY[player];
        ang = func_1505A630(dist, cur, 0) * 0.005493164f;
        curYaw = obj->mv->aimPitch;
        while (ang > 180.0f) {
            ang -= 360.0f;
        }
        while (curYaw > 180.0f) {
            curYaw -= 360.0f;
        }
        V = func_15144BC8(curYaw - ang);
        while (V > 180.0f) {
            V -= 360.0f;
        }
        yaw = diff * 8.0f;
        roll = V * 8.0f;
        if (yaw < -90.0f) {
            yaw = -90.0f;
        }
        if (yaw > 90.0f) {
            yaw = 90.0f;
        }
        if (roll < -30.0f) {
            roll = -30.0f;
        }
        if (roll > 30.0f) {
            roll = 30.0f;
        }
        V = sqrtf((yaw * yaw) + (roll * roll)) * 1.5f;
        h = V - 20.0f;
        if (V < 20.0f) {
            h = 0.0f;
        }
        h = -80.0f + h;
        if (type == 3) {
            h = 35.0f;
            roll = 0.0f;
        }
        if (doInner != 0) {
            ptr = &obj->mv->unk90;
            obj->mv->aimYaw -= diff;
            obj->mv->aimPitch = ang;
            func_15048758(&obj->mv->aimYaw);
            obj->mv->aimYawTarget = obj->mv->aimYaw;
            obj->mv->aimPitchTarget = obj->mv->aimPitch;
            obj->mv->stick_x = 0;
            obj->mv->stick_y = 0;
        } else if (hasCam != 0) {
            ptr = &cam->TgetFOVX;
            cam->PadPtr->stick_x = (s32)yaw;
            cam->PadPtr->stick_y = (s32)-roll;
        } else {
            ptr = &obj->mv->unk90;
            obj->mv->stick_x = (s32)yaw;
            obj->mv->stick_y = (s32)-roll;
        }
        if (mode != 0) {
            if (type == 0x1B) {
                if (h < -50.0f && (D_800D23B0->unk16C0[player] & (0x10001 << target))) {
                    obj->mv->buttons |= Z_TRIG;
                    obj->mv->buttonsTimer = 2;
                }
            } else if (type == 3) {
                f32 f;

                f = (dist - 136.0f) / 664.0f;
                if (f < 0.0f) {
                    f = 0.0f;
                }
                f = (sinf((D_800BE9AC & 0x3F) * 0.09815625101f) * 0.174999997f) + f;
                if (f > 1.0f) {
                    f = 1.0f;
                }
                f = (f * 0.02569000237f) + 0.01724099927f;
                obj->mv->stick_y = 0;
                obj->mv->stick_x = 0;
                obj->angle -= (s32)(yaw * 16.0f);
                obj->langle = obj->angle;
                obj->theta = (f32)(s16)(obj->langle + 0x4000) * 0.005493164f;
                func_150DA5EC(obj, f);
            } else if (D_800D23B0->attackTimer[player] >= 0) {
                obj->mv->buttons = Z_TRIG;
                if (mode == 1) {
                    obj->mv->buttonsTimer = 2;
                    if (D_800872D8[D_800E0BD0] < D_800D23B0->attackTimer[player]) {
                        obj->mv->buttons &= ~Z_TRIG;
                        obj->mv->buttonsTimer = 0;
                    }
                } else if (mode == 2) {
                    if (fire != 0) {
                        obj->mv->buttonsTimer = 0xA;
                    } else {
                        obj->mv->buttons &= ~Z_TRIG;
                    }
                } else {
                    obj->mv->buttonsTimer = 2;
                }
            }
        } else if (fire != 0) {
            if (flagA != 0) {
                obj->mv->pressedButtons = 0;
                obj->mv->buttons = 0;
            } else {
                obj->mv->pressedButtons = Z_TRIG;
                obj->mv->pressedButtonsTimer = 1;
            }
        } else if (flagA != 0) {
            if (cam != NULL) {
                cam->PadPtr->button |= Z_TRIG;
            }
            obj->mv->buttons = Z_TRIG;
            obj->mv->buttonsTimer = 2;
        }
        obj->mv->buttons |= R_TRIG;
        obj->mv->buttonsTimer = 1;
        *ptr += (h - *ptr) * 0.125f;
        if (hasCam != 0) {
            cam->TgetFOVY = cam->TgetFOVX * ratio;
        }
        if (cam != NULL) {
            cam->PadPtr->button |= R_TRIG;
        }
    }
    return result;
}

/*
 * Copies player's target to its state (unk129). Once its attackTimer passes the limit for
 * this level and distance it is reset, with func_1508EB90(player, 0x61, 0x2000) if the target
 * is in sight; while the timer is 0, unk12A is set at random.
 */
s32 func_1508D850(s32 player) {
    s8 *timers;
    s8 *targets;
    s32 *target;
    f32 dx;
    Actor *obj;
    f32 dz;
    f32 dist;
    s32 range;
    s32 limit;
    s32 off;

    // matching: byte pointers plus a shared byte offset, as IDO keeps player * 4 in one register
    targets = (s8 *)D_800D23B0->target;
    timers = (s8 *)D_800D23B0->attackTimer;
    obj = &movement_obj[player];
    off = player * 4;
    target = (s32 *)(targets + off);
    obj->mv->unk129 = *target;
    if (*target < 0) {
        return 0;
    }
    dx = movement_obj[*target].xpos - obj->xpos;
    dz = movement_obj[*target].zpos - obj->zpos;
    dist = sqrtf((dx * dx) + (dz * dz));
    if (5000.0f < dist) {
        range = 0;
    } else if (dist > 2000.0f) {
        range = 1;
    } else {
        range = 2;
    }
    limit = ((s16 *)D_80087320)[D_800E0BD0 * 3 + range]; // matching: the [level][range] form computes the address differently
    if (limit < *(s32 *)(timers + off)) {
        *(s32 *)(timers + off) = 0;
        if (*(s32 *)((s8 *)D_800D23B0->unk16C0 + off) & (0x10000 << *target)) {
            func_1508EB90(player, 0x61, 0x2000);
        }
    }
    if (*(s32 *)(timers + off) == 0) {
        obj->mv->unk12A = 0;
        if (D_800872E8[D_800E0BD0] < (s32)(func_150ADA68() * 100.0f)) {
            obj->mv->unk12A = 1;
        }
    }
}

/* Resets the pickup slots: nobody holds one and each respawns on the next update. */
s32 func_1508DA1C(void) {
    s32 i;
    s32 *slots = D_800D23B0->slot;
    s32 *owners = D_800D23B0->slotOwner;
    s32 *overrides = D_800D23B0->itemOverride;
    s32 *timers = D_800D23B0->slotTimer;
    s32 *spawns = D_800D23B0->slotSpawn;
    s32 *dropTimers = D_800D23B0->slotDropTimer;

    for (i = 0; i < D_800D23B0->numPlayers; i++) {
        slots[i] = -1;
    }
    for (i = 0; i < D_800D23B0->numSlots; i++) {
        owners[i] = -1;
        overrides[i] = 0;
        spawns[i] = -1;
        timers[i] = 1;
        dropTimers[i] = 0;
    }
}

/*
 * Releases the slot held by `player` (returns 1 without doing so if `mode` < 0). Unless the
 * slot is flagged 2, it is reset; mode 1 also drops it at the player's position.
 */
s32 func_1508DAEC(s32 player, s32 mode) {
    MpGame *game;
    s32 slot;
    s32 next;
    Actor *obj;

    game = D_800D23B0;
    if (game == NULL) {
        return 0;
    }
    slot = game->slot[player];
    if (slot >= 0) {
        if (mode < 0) {
            return 1;
        }
        game->slot[player] = -1;
        obj = &movement_obj[player];
        if ((D_800D23B0->slotFlags[slot] & 2) == 0) {
            D_800D23B0->slotOwner[slot] = -1;
            D_800D23B0->slotSpawn[slot] = -1;
            next = D_800D23B0->respawnDelay;
            if (mode == 1) {
                D_800D23B0->slotDropTimer[slot] = 300;
                obj = &movement_obj[player];
                next = 4;
                D_800D23B0->slotDropX[slot] = obj->xpos;
                D_800D23B0->slotDropY[slot] = obj->ypos + 50.0f;
                D_800D23B0->slotDropZ[slot] = obj->zpos;
            }
            D_800D23B0->slotTimer[slot] = next;
        }
    }
    return 0;
}




/*
 * Per-frame pickup update. Gives each player the item of its override, its slot or the default
 * (swapping attachments when it changes), then moves the slot pickups: drop timers, respawns at
 * a spawn node, and handing a touched pickup to its player (0x8AE adds 3 grenades, up to 6).
 * Declared int but returns no value, in the pre-ANSI way: func_1509DFC4 returns what it leaves in v0.
 */
s32 func_1508DC24(void) {
    static s8 D_80087340 = 0;
    MpGame *base;
    s32 i;
    Actor *obj; // matching: unused stack slot in the original frame
    s32 count;
    s32 cur;
    s32 prev;
    s32 setId;
    s32 numAttached;
    s32 j;
    f32 x;
    f32 y;
    f32 z;
    s32 found;
    u16 pp;
    s32 kind;
    s32 *slots;
    s32 *entities;
    s32 *owners;
    s32 *overrides;
    s32 *timers;
    s32 *spawns;
    s32 *items;
    s32 *flags;
    s32 *homeSpawns;
    s32 *dropTimers;
    s32 *dropX;
    s32 *dropY;
    s32 *dropZ;
    ModelAttachment *attached[10];
    Actor *other;
    WorldObject *pickup;
    s32 idx;
    s32 r;
    s32 k;

    base = D_800D23B0;
    i = 0; // matching: zeroed here, before the field loads, not in the for
    slots = base->slot;
    entities = base->slotEntity;
    owners = base->slotOwner;
    overrides = base->itemOverride;
    timers = base->slotTimer;
    spawns = base->slotSpawn;
    items = base->slotItem;
    flags = base->slotFlags;
    homeSpawns = base->slotHomeSpawn;
    dropTimers = base->slotDropTimer;
    dropX = base->slotDropX;
    dropY = base->slotDropY;
    dropZ = base->slotDropZ;
    count = base->numPlayers;
    for (; i < count; i++) {
        kind = base->gameType;
        if (kind == 0xB9 && movement_obj[i].team != 0) {
            continue;
        }
        prev = movement_obj[i].mv->context_b & 0x7F;
        cur = overrides[i];
        if (movement_obj[i].mv->GrenadeThrowState >= 5) {
            cur = 0x3A;
        } else if (movement_obj[i].mv->GrenadeThrowState > 0) {
            cur = 0x26;
        }
        if (cur == 0) {
            if (slots[i] != -1) {
                cur = items[slots[i]];
            } else {
                cur = base->defaultItem;
            }
        } else if (cur == 0x457) {
            cur = 0;
        }
        if (kind == 0xB8) {
            prev = movement_obj[i].mv->unk128 & ~0x20;
        }
        if (cur != prev) {
            setId = func_1508E780(prev);
            if (setId != -1) {
                setId &= ~0x1000;
                func_1508EB90(i, 0x4C, setId);
                if (setId == 0x1D) {
                    func_1508EB90(i, 0x4C, 0x1E);
                }
                movement_obj[i].mv->weapon_status = 0;
            }
            setId = func_1508E780(cur);
            if (setId > 0x1000) {
                setId &= ~0x1000;
                func_1508EB90(i, 0x4B, setId);
            }

            if (D_800D23B0->gameType == 0xB8) {
                if (prev == 0x80) {
                    func_1502EA50(&movement_obj[i]);
                    movement_obj[i].imune = 0;
                }
                movement_obj[i].mv->unk128 = cur;
                if (cur == 1) {
                    movement_obj[i].mv->unk12C = 600;
                } else if (cur == 0x80) {
                    movement_obj[i].mv->unk12C = 1800;
                    func_1502EA0C(&movement_obj[i], 0, 0, 0xFF, 0xBF, 4);
                } else if (cur == 0x10) {
                    movement_obj[i].mv->unk12C = 3;
                }
            } else {
                func_15080620(i, 1, cur, 0);
                if (cur == 0 && prev != 3) {
                    movement_obj[i].mv->rt_context_b = 0;
                }
                if (prev == 0x20) {
                    func_1508EB90(i, 0x45, 4);
                }
                if (cur == 0x20) {
                    func_1508EB90(i, 0x45, 3);
                }
            }
        }
        base = D_800D23B0;
        if (D_80087340 == i && base->gameType != 0xB8) {
            setId = func_1508E780(cur);
            found = 0;
            numAttached = func_15033E28(&movement_obj[i], attached);
            if (numAttached == 0 && movement_obj[i].mv->weapon_status == 2 && setId != 0x55 && setId != 0x2E) {
                func_15083568(&movement_obj[i], setId, 1.0f, 0);
                if (setId == 0x1D) {
                    func_15083568(&movement_obj[i], 0x1E, 1.0f, 0);
                }
            }
            if (numAttached != 0 && cur != 0x20) {
                if (setId != -1) {
                    setId &= ~0x1000;
                    for (j = 0; j < numAttached; j++) {
                        if ((attached[j]->setId != 0x1E || setId != 0x1D) && setId != attached[j]->setId &&
                            attached[j]->setId != 0x2A && attached[j]->setId != 0x22 &&
                            attached[j]->setId != 0x2D && attached[j]->setId != 0x72 &&
                            attached[j]->setId != 0x73 && attached[j]->setId != 0x76 &&
                            attached[j]->setId != 0x77 && attached[j]->setId != 0x78 && attached[j]->setId != 7) {
                            func_15030158(attached[j], 0);
                            movement_obj[i].mv->weapon_status = 0;
                        } else if (setId == attached[j]->setId) {
                            found = 1;
                        }
                    }
                    if (found == 0 && movement_obj[i].mv->weapon_status == 2 && setId != 0x55 && setId != 0x2E) {
                        func_15083568(&movement_obj[i], setId, 1.0f, 0);
                        if (setId == 0x1D) {
                            func_15083568(&movement_obj[i], 0x1E, 1.0f, 0);
                        }
                    }
                } else {
                    for (j = 0; j < numAttached; j++) {
                        if (attached[j]->setId != 0x2A && attached[j]->setId != 0x22 &&
                            attached[j]->setId != 0x2C && attached[j]->setId != 0x64 &&
                            attached[j]->setId != 0x2D && attached[j]->setId != 0x72 &&
                            attached[j]->setId != 0x73 && attached[j]->setId != 0x76 &&
                            attached[j]->setId != 0x77 && attached[j]->setId != 0x78 && attached[j]->setId != 7) {
                            func_15030158(attached[j], 0);
                            movement_obj[i].mv->weapon_status = 0;
                        }
                    }
                }
            }
            if (setId != 0x55) {
                s32 m;
                for (m = D_800D23B0->numPlayers; m < 25; m++) {
                    other = &movement_obj[m];
                    if (other->type == 0) {
                        continue;
                    }
                    if (other->attach_char != i + 1) {
                        continue;
                    }
                    if (other->character_type != 0x6E) {
                        continue;
                    }
                    func_151027E8(other);
                    func_15060F28(other, 0);
                    break;
                }
                // matching: empty tests of the original source; without them IDO allocates registers differently
                if (setId == 0x7B) {}
                if (numAttached == 0x7C) {}
                if (setId == 0x55) {}
            }
            base = D_800D23B0;
        }
        count = base->numPlayers;
    }
    D_80087340++;
    if (D_80087340 >= count) {
        D_80087340 = 0;
    }
    for (i = 0; i < base->numSlots; i++) {
        pickup = func_151149AC(entities[i]);
        if (pickup == NULL) {
            base = D_800D23B0;
            continue;
        }
        if (dropTimers[i] != 0) {
            dropTimers[i] -= D_800BE9E4;
            if (dropTimers[i] <= 0) {
                timers[i] = 4;
                dropTimers[i] = 0;
            }
        }
        if (timers[i] > 0) {
            timers[i] -= D_800BE9E4;
            if (timers[i] <= 0) {
                timers[i] = 0;
                pickup->inactive = 0;
                if (dropTimers[i] != 0) {
                    pickup->x = dropX[i];
                    pickup->y = dropY[i];
                    pickup->z = dropZ[i];
                } else if (!(flags[i] & 4)) {
                    if (flags[i] & 1) {
                        x = D_800D23B0->numSpawns;
                        do {
                            r = func_150ADA68() * 0.9999899864f * x;
                            for (k = 0; k < D_800D23B0->numSpawns; k++) {
                                if (r == spawns[k]) {
                                    r = -1;
                                    k = 1000;
                                }
                            }
                        } while (r == -1);
                    } else {
                        r = homeSpawns[i];
                    }
                    spawns[i] = r;
                    idx = func_15086D48(D_800D23B0->spawnNodes[r]);
                    if (idx != -1) {
                        func_15086CBC(idx, &x, &y, &z);
                        pickup->x = x;
                        pickup->y = (s16)y + 60;
                        pickup->z = z;
                    }
                }
            }
        } else if (timers[i] == 0) {
            j = pickup->touch.i.b - 1; // work word 1: player + 1 that touched the pickup, 0 = none
            if (j != -1) {
                pickup->touch.i.b = 0;
                if (items[i] == 0x8AE) {
                    movement_obj[j].mv->numGrenades += 3;
                    if (movement_obj[j].mv->numGrenades >= 7) {
                        movement_obj[j].mv->numGrenades = 6;
                    }
                    timers[i] = D_800D23B0->respawnDelay;
                } else {
                    func_1508DAEC(j, 0);
                    if (flags[i] & 2) {
                        timers[i] = D_800D23B0->respawnDelay;
                    } else {
                        timers[i] = -1;
                        owners[i] = j | 0x2000;
                        spawns[i] = -1;
                    }
                    dropTimers[i] = 0;
                    slots[j] = i;
                    D_800D23B0->unk5C[j] = 1;
                }
                kind = D_800D23B0->gameType;
                if (kind == 0xB9) {
                    movement_obj[j].mv->unk12C = 600;
                }
                kind = D_800D23B0->gameType;
                if (kind == 0xB8) {
                    if (items[i] == (pp = movement_obj[j].mv->unk128)) { // matching: the u16 temporary
                        if (items[i] == 1) {
                            movement_obj[j].mv->unk12C = 600;
                        } else if (items[i] == 0x80) {
                            movement_obj[j].mv->unk12C = 1800;
                        } else if (items[i] == 0x10) {
                            movement_obj[j].mv->unk12C = 3;
                        }
                    }
                }
            }
        }
        base = D_800D23B0;
    }
}

void func_1508E6C8(void) {
}

/* Per item `type` value (100000 by default). */
s32 func_1508E6D0(s32 type) {
    switch (type) {
    case 0x3F:
        return 1800;
    case 0x14:
        return 3200;
    case 0x22:
        return 500;
    case 0x23:
        return 10000;
    case 0x16:
        return 100000;
    case 0x15:
        return 500;
    case 0x21:
        return 500;
    case 0x24:
        return 100000;
    case 0x19:
    case 0x40:
        return 500;
    case 9:
        return 5000;
    case 0x38:
        return 5000;
    case 0x18:
    case 0x41:
        return 100000;
    case 3:
        return 750;
    case 0x37:
        return 1500;
    default:
        return 100000;
    }
}

/* Returns the attachment set id for item `type`, or -1; bit 0x1000 is a flag on it. */
s32 func_1508E780(s32 type) {
    if (D_800D23B0->gameType != 0xB8) {
        switch (type) {
        case 0x14:
            return 0x28;
        case 0x3F:
            return 0x2F;
        case 0x22:
            return 0x56;
        case 0x23:
            return 0x16;
        case 0x16:
            return 0x24;
        case 0x15:
            return 0x4B;
        case 0x21:
            return 0x58;
        case 0x24:
            return 0x63;
        case 0x19:
            return 0x57;
        case 0x40:
            return 0x21;
        case 9:
            return 0x1D;
        case 0x38:
            return 0x7B;
        case 0x39:
            return 0x82;
        case 0x37:
            return 0x85;
        case 0x18:
            return 0x55;
        case 0x41:
            return 0x2E;
        case 0x25:
            return 0x1060;
        default:
            return -1;
        }
    } else {
        switch (type) {
        case 8:
            return -1;
        case 1:
            return 0x105C;
        case 16:
            return 0x1019;
        case 2:
            return 0x1059;
        default:
            return -1;
        }
    }
}

/* Player `player` takes over the multiplayer state of player `source`. */
s32 func_1508E89C(s32 player, s32 source) {
    Actor *obj;
    s32 savedRot;
    s32 bit0;
    s32 tmp;
    u8 slot;
    MpGame *res;

    obj = &movement_obj[player];
    if (D_800D18A4[player] > 50) {
        if (!(D_800D18A2 & (1 << player))) {
            bit0 = 1 << player;
            D_800D18A2 |= bit0;
            D_800D18A0 &= ~bit0;
            D_800D18A0 |= (1 << source);
            tmp = func_150859AC(source, 3);
            func_15085710(player, 2, tmp);
            func_15085710(source, 2, 0);
            if (D_800D23B0->seenMask != 0) {
                D_800D23B0->seenMask &= ~bit0;
                D_800D23B0->seenMask |= (1 << source);
            }
            if (D_800D23B0->layer) {
                D_800D23B0->layer[player] = D_800D23B0->layer[source];
            }
            if (D_800D23B0->unk39C) {
                D_800D23B0->unk39C[player] = D_800D23B0->unk39C[source];
            }
            if (D_800D23B0->attacker) {
                D_800D23B0->attacker[player] = D_800D23B0->attacker[source];
            }
            if (D_800D23B0->slot) {
                D_800D23B0->slot[player] = D_800D23B0->slot[source];
                if (D_800D23B0->slot[player] >= 0) {
                    D_800D23B0->slotOwner[D_800D23B0->slot[player]] = player;
                }
            }
            D_800D23B0->unk16C0[player] = D_800D23B0->unk16C0[source];
            savedRot = obj->mv->shotsfired;
            func_15062800(&movement_obj[source], obj);
            if (level == 0x3F) {
                if (obj->lifting >= 100) {
                    slot = obj->lifting - 100;
                    res = func_1509B570(D_800D2E48);
                    if (res != NULL) {
                        s32 idx;
                        s32 id;

                        idx = obj - movement_obj;
                        id = movement_obj[slot].unique_id - 20;
                        res->unk24B8[id] = idx | 0x2000;
                        res->unk24DC[id] = idx;
                    }
                }
            }
            obj->mv->bot = 0;
            obj->imune = 90;
            obj->mv->shotsfired = savedRot;
            obj->mv->unk1AF = 0x40;
            func_15128774(obj->CameraPtr, obj);
            D_8008FD94++;
        }
    }
    return 0;
}

/* Calls func_1509BFB0(1, actor | 0x2000, id, param) for player's actor (unk13F). */
void func_1508EB90(s32 player, s32 id, s32 param) {
    s32 temp = movement_obj[player].listpos;
    func_1509BFB0(1, temp | 0x2000, id, param);
}

/* func_1508EB90(player, 0x14, param). */
void func_1508EBF8(s32 player, s32 param) {
    s32 temp = movement_obj[player].listpos;
    func_1509BFB0(1, temp | 0x2000, 0x14, param);
}

/* func_1508EB90(player, 0x61, param). */
void func_1508EC5C(s32 player, s32 param) {
    s32 temp = movement_obj[player].listpos;
    func_1509BFB0(1, temp | 0x2000, 0x61, param);
}

/* Finds the entry of kind `kind` linked to (type, id); returns its id or -1. */
s32 func_1508ECC0(u16 type, u16 id, u16 kind) {
    u32 i;
    u32 j;
    u32 k;
    u16 key;

    key = (type << 12) + id;
    for (i = 0; i < D_80087380; i++) {
        for (j = 0; j < D_800D23C0[i].count; j++) {
            if (key == D_800D23C0[i].members[j]) {
                for (k = 0; k < D_800D23C0[i].count; k++) {
                    if (kind == (D_800D23C0[i].members[k] >> 12) && k != j) {
                        return D_800D23C0[i].members[k] & 0xFFF;
                    }
                }
                return -1;
            }
        }
    }
    return -1;
}

/* Clears link group `index`. */
void func_1508EDBC(s32 index) {
    if (index < D_80087380) {
        D_800D23C0[index].count = 0;
        D_800D23C0[index].unk4 = 0;
        D_800D23C0[index].unk0 = 0;
    }
}

/* Fires every entry of the link group containing (type, id), then clears the group. */
void func_1508EE0C(u16 type, u16 id) {
    u32 i;
    u32 j;
    s32 operand;
    u16 key;

    key = (type << 12) + id;
    for (i = 0; i < D_80087380; i++) {
        for (j = 0; j < D_800D23C0[i].count; j++) {
            if (D_800D23C0[i].members[j] == key) {
                for (j = 0; j < D_800D23C0[i].count; j++) {
                    operand = D_800D23C0[i].members[j] & 0xFFF;
                    switch (D_800D23C0[i].members[j] >> 12) {
                    case 2:
                        func_15114B94(operand);
                        break;
                    case 3:
                        aim_removePoint(operand);
                        break;
                    }
                }
                func_1508EDBC(i);
                return;
            }
        }
    }
}
