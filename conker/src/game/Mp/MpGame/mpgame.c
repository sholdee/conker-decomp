#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/aim.h"
#include "conker/attach.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/math.h"
#include "conker/mp.h"
#include "conker/particle.h"
#include "conker/path.h"
#include "conker/player.h"
#include "conker/save.h"
#include "conker/script.h"
#include "conker/text.h"
#include "conker/trail.h"
#include "conker/view.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define VIEWS ((RenderView *)gViews)

/* This file's .bss. */
MpGame *gMpGame; // the loaded multiplayer game resource (mp_loadGame)





void mp_clearGame(void) {
    gMpGame = NULL;
}

/* Per-difficulty bot tuning (indexed by gBotDifficulty): this file's .data, in the original order. */
f32 gBotTargetDistScales[5] = { 1.5f, 1.2f, 0.8f, 0.4f, 0.15f };
s16 gBotAimTimeLimits[5] = { -1, -1, 600, 240, 0 }; // aim time limit, -1 = none
s8 gBotAttackIntervals[5] = { 80, 70, 60, 60, 45 };
s8 gBotFireHoldTimes[5] = { 10, 20, 30, 60, 45 };
s8 gBotAttackDelays[5] = { 120, 80, 40, 10, 0 };
s8 gBotAimAccuracy[5] = { 10, 20, 50, 80, 100 }; // percent chance
s8 D_800872F0[5] = { 30, 25, 20, 15, 15 };
f32 gBotAimRates[5] = { 0.05f, 0.1f, 0.125f, 0.25f, 1.0f }; // aim rate
f32 D_8008730C[5] = { 0.0f, 0.025f, 0.05f, 0.075f, 0.0f }; // not referenced by any code
s16 gBotAttackTimeLimits[5][3] = { // [level][range] attack timer limit
    { 900, 300, 80 }, { 600, 300, 80 }, { 420, 300, 80 }, { 360, 240, 60 }, { 300, 180, 60 },
};

/* Loads the multiplayer game resource gCurrentTaskId and resets its per-player state. */
s32 mp_loadGame(void) {
    f32 *scales;
    s32 i;
    s32 id;
    void *resource;

    id = gCurrentTaskId;
    resource = script_getTaskBss(id);
    if (resource == NULL) {
        return 0;
    }
    // matching: identical separate case bodies, as in the original jump table
    switch (id) {
    case 0xB9:
        gMpGame = resource;
        break;
    case 0xB8:
        gMpGame = resource;
        break;
    case 0xBB:
        gMpGame = resource;
        break;
    case 0xBA:
        gMpGame = resource;
        break;
    case 0xB7:
        gMpGame = resource;
        break;
    case 0xBD:
        gMpGame = resource;
        break;
    case 0xBE:
        gMpGame = resource;
        break;
    default:
        gMpGame = NULL;
        return 0;
    }
    mp_randomizeWeights();
    gMpGame->losPlayer = 0;
    gMpGame->losOther = 0;
    gMpGame->nextHuman = 0;
    gMpGame->nextBot = 0;
    gMpGame->targetListPlayer = 0;
    gMpGame->gameType = id;
    gMpGame->targetDistScale = gBotTargetDistScales[gBotDifficulty];
    gMpGame->numZones = 0;
    scales = gBotAimRates;
    for (i = 0; i < 16; i++) {
        gMpGame->sightBits[i] = 0;
        gMpGame->aimRate[i] = scales[gBotDifficulty];
        gMpGame->aimX[i] = 0.0f;
        gMpGame->aimY[i] = 0.0f;
        gMpGame->aimZ[i] = 0.0f;
    }
    gMpGame->unkC = D_800872F0[gBotDifficulty];
    return 0;
}

/*
 * Picks the opponent `player` should go for: the one whose distance scores best on a sine
 * curve between a minimum and maximum range. Returns its index or -1.
 */
s32 mp_pickOpponentInRange(s32 player) {
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

    states = gMpGame->team;
    mask = gMpGame->activeMask;
    count = gMpGame->numPlayers;
    off = player * 4; // matching: dead store that IDO still schedules
    bestIdx = -1;
    if (gMpGame->unk15C[player] == 0) {
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
            (gMpGame->sightBits[player] & (bit << 16))) {
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
 * Advances to the next player (targetListPlayer) and rebuilds its target list: the opposing players
 * sorted by a weighted distance (targets / targetDists rows).
 */
s32 mp_updateTargetList(void) {
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

    teams = gMpGame->team;
    mask = gMpGame->activeMask;
    count = gMpGame->numPlayers;
    kinds = gMpGame->unk15C;
    active = gMpGame->unk39C;
    layers = gMpGame->layer;
    ids = gMpGame->targets[0];
    dists = gMpGame->targetDists[0];
    gMpGame->targetListPlayer++;
    if (gMpGame->targetListPlayer == gMpGame->numPlayers) {
        gMpGame->targetListPlayer = 0;
    }
    cur = gMpGame->targetListPlayer;
    row = cur * 16;
    if (!((1 << cur) & mask)) {
        // matching: one-line loop (the -g3 line numbers steer the unrolled stores' order)
        for (i = 0; i < 16; i++) ids[row + i] = -1;
        return 0;
    }
    n = 0;
    minDist = 32000;
    myTeam = teams[cur];
    myMask = gMpGame->sightBits[cur];
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
                if (i >= gHumanCount) {
                    dist *= 2;
                }
                if (gMpGame->gameType != 0xB8 && (kinds[i] == 2 || kinds[i] == 3)) {
                    dist = -1;
                }
            }
            if (gMpGame->gameType == 0xB7) {
                if (dist == -1) {
                    dist = rawDist;
                }
                if (other->lifting != 0) {
                    if (i >= gHumanCount) {
                        dist *= 2;
                    }
                    dist = dist / (player_getStat(i, 6) + 2);
                }
            }
            if (i < gHumanCount && dist != -1) {
                dist *= gMpGame->targetDistScale;
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
    if (gMpGame->nearestEnemyDist) {
        gMpGame->nearestEnemyDist[cur] = minDist;
    }
    return 0;
}

/* Per-frame update of every player's multiplayer state; returns a mask of players that respawned. */
s32 mp_update(void) {
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
    activeMask = gMpGame->activeMask;
    seenMask = gMpGame->seenMask;
    count = gMpGame->numPlayers;
    teams = gMpGame->team;
    arr35C = gMpGame->unk35C;
    arr31C = gMpGame->unk31C;
    arr19C = gMpGame->unk19C;
    aimTimes = gMpGame->aimTime;
    attackTimers = gMpGame->attackTimer;
    arr25C = gMpGame->unk25C;
    arr41C = gMpGame->unk41C;
    attackers = gMpGame->attacker;
    teamScores = gMpGame->teamScore;
    countdowns = gMpGame->countdown;
    arr1C = gMpGame->unk1C;
    arr39C = gMpGame->unk39C;
    layers = gMpGame->layer;
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
            if (team != 0 && gMpGame->gameType == 0xB9) {
                obj->family = 1;
            }
            old = attackTimers[i];
            aimTimes[i] += gFrameTicks;
            attackTimers[i] += gFrameTicks;
            arr25C[i] -= gFrameTicks;
            if (arr25C[i] <= 0) {
                arr25C[i] = 0;
            }
            arr41C[i] -= gFrameTicks;
            if (arr41C[i] <= 0) {
                arr41C[i] = 0;
            }
            countdowns[i] -= gFrameTicks;
            if (countdowns[i] < 0) {
                countdowns[i] = 0;
            }
            if (arr19C[i] == 3 && (arr25C[i] & 0x1F) >= 0x1D) {
                mp_sendPlayerCommand61(i, 0x8000);
            }
            attackers[i] = D_800E0BB8[i];
            D_800E0BB8[i] &= 0x7F;
            if (gMpGame->grenades != NULL) {
                gMpGame->grenades[i] = obj->mv->numGrenades;
            }
            arr39C[i] = player_getStat(i, 4);
            layers[i] = path_getLayerAt(obj->ypos);
            if (arr39C[i] == 0 || isNew || obj->snapTimer != 0) {
                if (arr35C[i] == 0) {
                    if (obj->pathNode != 0xFF) {
                        gMpGame->unk59C[i] = obj->pathNode;
                    } else {
                        gMpGame->unk59C[i] = -1;
                        if (!attackers) { // matching: an extra use of attackers steers IDO's register allocation
                        }
                    }
                    arr35C[i] = 1;
                    arr25C[i] = 0;
                    arr19C[i] = 0;
                    gMpGame->unk5C[i] = 1;
                    gMpGame->target[i] = -1;
                    teamScores[team]--;
                    if ((attackers[i] & 0x7F) < count) {
                        other = teams[attackers[i] & 0x7F];
                        if (team != other) {
                            teamScores[other]++;
                        }
                    }
                    if (gMpGame->gameType != 0xB9) {
                        gMpGame->itemOverride[i] = 0;
                        mp_releaseSlot(i, 1);
                    }
                    result |= bit;
                }
            } else {
                arr35C[i] = 0;
            }
            flag = 1;
            if (arr31C[i] != 0 && gMpGame->gameType != 0xB8) {
                reset = 0;
                if (arr19C[i] != 1 && aimTimes[i] < 30) {
                    type = obj->intelwalk;
                    obj->targetIndex = arr31C[i] - 2;
                    if (type != 4 && type != 5) {
                        mp_sendPlayerCommand14(i, 4);
                    }
                    if (type == 4) {
                        aimTimes[i] = 0;
                    }
                    if (type == 5) {
                        aimTimes[i] = 30;
                        gMpGame->aimRate[i] = gBotAimRates[gBotDifficulty];
                        gMpGame->aimX[i] = (math_randomFloat() * 50.0f) + obj->xpos;
                        gMpGame->aimY[i] = obj->ypos + 40.0f;
                        gMpGame->aimZ[i] = (math_randomFloat() * 50.0f) + obj->zpos;
                        if ((arr31C[i] - 2) >= 2) {
                            tgt = &movement_obj[arr31C[i] - 2];
                            gMpGame->aimX[i] += (tgt->xpos - obj->xpos) * 0.5f;
                            gMpGame->aimZ[i] += (tgt->zpos - obj->zpos) * 0.5f;
                        }
                    }
                }
                flag = 0;
                bit = 0;
                if (gBotAttackIntervals[gBotDifficulty] < attackTimers[i]) {
                    attackTimers[i] = 0;
                    old = -1;
                }
                if (attackTimers[i] >= 0 && old < 0) {
                    bit = 1;
                }
                if (gBotAimTimeLimits[gBotDifficulty] != -1 && aimTimes[i] >= gBotAimTimeLimits[gBotDifficulty]) {
                    reset = 1;
                }
                gMpGame->side[i] += (gMpGame->sideGoal[i] - gMpGame->side[i]) >> 2;
                gMpGame->drop[i] += (gMpGame->dropGoal[i] - gMpGame->drop[i]) >> 2;
                if (mp_aimBot(i, arr31C[i] - 2, bit, reset, gMpGame->side[i], gMpGame->drop[i]) && arr19C[i] != 1) {
                    aimTimes[i] = 0;
                }
                if (attackTimers[i] == 0) {
                    gMpGame->sideGoal[i] = 0;
                    gMpGame->dropGoal[i] = 0;
                    if (gBotAimAccuracy[gBotDifficulty] < (s32)(math_randomFloat() * 100.0f)) {
                        if (math_randomFloat() > 0.5f) {
                            gMpGame->sideGoal[i] = -50;
                        } else {
                            gMpGame->sideGoal[i] = 50;
                        }
                        if (math_randomFloat() > 0.5f) {
                            gMpGame->dropGoal[i] = 50;
                        } else {
                            gMpGame->dropGoal[i] = 0;
                        }
                    }
                }
            }
            if (flag && gMpGame->gameType != 0xB8) {
                attackTimers[i] = -gBotAttackDelays[gBotDifficulty];
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

    gMpGame->seenMask = seenMask;
    if (gFrameTicks != 0) {
        gMpGame->ticks = gFrameTicks;
    } else {
        gMpGame->ticks = 1.0f;
    }
    if (gMpGame->zoneExitNode != NULL) {
        mp_assignZoneNodes();
    }
    if (gMpGame->gameType == 0xB9) {
        half = (s32)(VIEWS[0].right - VIEWS[0].left) >> 1;
        text_setColor(0xFF, 0xFF, 0xFF, 0xFF);
        for (j = 0; j <= gScreenLayout; j++) {
            if (movement_obj[j].team == 0) {
                vp = &VIEWS[j];
                text_printAt(vp->left + half, vp->top + 6.0f, 1, "%d", countdowns[j] / 60);
            }
        }

    }
    return result;
}

/* Returns player's score record value (0 for a non-human player). */
s32 mp_getTeamKills(s32 player) {
    if (player >= gHumanCount) {
        return 0;
    }

    return gMenuState->stats[player].teamKills;
}

/* Clears player's score record value. */
void mp_clearTeamKills(s32 player) {
    if (player < gHumanCount) {
        gMenuState->stats[player].teamKills = 0;
    }
}

/* Registers a zone of radius r around (x, y, z), up to 8. */
void mp_addZone(f32 x, f32 y, f32 z, f32 r) {
    s32 n;

    if (gMpGame != NULL && gMpGame->numZones < 8) {
        n = gMpGame->numZones++;
        gMpGame->zoneList[n].x = x;
        gMpGame->zoneList[n].y = y;
        gMpGame->zoneList[n].z = z;
        gMpGame->zoneList[n].radiusSq = r * r;
    }
}

/*
 * Flood-fills the path graph from node `id`, marking visited nodes; nodes farther than
 * s->radiusSq from (s->x, s->z) end the walk and are collected (up to 8) with their squared distance.
 */
void mp_floodFillPath(u8 id, PathSearch *s) {
    PathNode *node;
    f32 dx;
    f32 dist;
    s32 j;
    u8 link;

    /* BUG (original game): the bit index should be id & 7, so ids 8 apart share a visited bit */
    s->visited[id >> 3] |= 1 << (id & 3);
    node = &gPathNodes[id];
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
                    mp_floodFillPath(node->links[j], s);
                }
            }
        }
    }
}

/*
 * For each registered zone, assigns every computer player standing in it the nearest
 * path node reachable from the zone centre (zoneExitNode), then clears the zone list.
 */
void mp_assignZoneNodes(void) {
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

    zone = gMpGame->zoneExitNode;
    list = gMpGame->zoneList;
    for (i = 0; i < gPlayerCount; i++) {
        if (zone[i] != -1) {
            zone[i] = -2;
        }
    }
    for (i = 0; i < gMpGame->numZones; i++) {
        found = 0;
        s.radiusSq = list[i].radiusSq;
        x = list[i].x;
        y = list[i].y;
        z = list[i].z;
        for (j = gHumanCount; j < gPlayerCount; j++) {
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
                        layer = path_getLayerAt(y);
                        found = 1;
                        startNode = path_findNearestNode(x, y, z, 0, layer);
                        if (startNode != -1) {
                            s.x = x;
                            s.z = z;
                            s.count = 0;
                            bzero(s.visited, 0x20);
                            mp_floodFillPath(startNode, &s);
                        }
                    }
                    for (k = 0; k < s.count; k++) {
                        node = &gPathNodes[s.ids[k]];
                        dx = node->x - obj->xpos;
                        d = node->z - obj->zpos;
                        d = dx * dx + d * d;
                        if (d < s.dist[k] && d < bestDist) {
                            best = s.ids[k];
                            bestDist = d;
                        }
                    }
                    if (best != 0xFF) {
                        node = &gPathNodes[best];
                        if (zone[j] != -2) {
                            gMpGame->unk5C[j] = 1;
                        }
                        zone[j] = node->id;
                    }
                }
            }
        }
    }
    for (i = 0; i < gPlayerCount; i++) {
        if (zone[i] < 0) {
            zone[i] = -1;
        }
    }
    gMpGame->numZones = 0;
}

/*
 * Picks the best opponent for player unk4: the active player with the lowest path cost to it,
 * scaled by weights[unk4][] / 255 (and 0.8 for its current target). Stores it in opponent and
 * the distance in opponentDist. Returns 0.
 */
s32 mp_pickOpponentByPath(void) {
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
    count = gMpGame->numPlayers;
    current = gMpGame->unk4;
    active = gMpGame->unk39C;
    neighbors = gMpGame->targets[0];
    costs = gMpGame->targetDists[0];
    targets = gMpGame->target;
    if (count >= 9) {
        mode = 4;
    }
    mp_updateLineOfSight(0, mode);
    mp_updateTargetList();
    weights = gMpGame->weights[current];
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
    gMpGame->opponent[current] = winner;
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
        gMpGame->opponentDist[current] = distance;
    } else {
        gMpGame->opponentDist[current] = minimum;
    }
    return 0;
}

/*
 * Fills each player's row of weights with a random triangle wave between 0x55 and 0xFF,
 * starting from a random unused column.
 */
void mp_randomizeWeights(void) {
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

    n = gPlayerCount;
    row = gMpGame->weights[0];
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
        pos = math_randomFloat() * fn;
        if (math_randomFloat() > 0.5f) {
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
s32 mp_pickTeamOpponents(void) {
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

    myIdx = gMpGame->unk4;
    numPlayers = gMpGame->numPlayers;
    kinds = gMpGame->unk15C;
    active = gMpGame->unk39C;
    teams = gMpGame->team;
    bestCosts = gMpGame->opponentDist;
    bestSlots = gMpGame->opponent;
    links = gMpGame->targets[0];
    costs = gMpGame->targetDists[0];
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

/* Line-of-sight test between players a and b; sets their visibility bits in sightBits. */
void mp_checkLineOfSight(s32 a, s32 b) {
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
    from = actor_findOnPath(a & 0xFFFF0FFF);
    to = actor_findOnPath(b & 0xFFFF0FFF);
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
        } else if (to->mv->bot && gBotAimTimeLimits[gBotDifficulty] != -1) {
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
        } else if (fromFlag && gBotAimTimeLimits[gBotDifficulty] != -1) {
            x1 = to->aim.x;
            y1 = to->aim.y;
            z1 = to->aim.z;
        } else {
            y1 = to->ypos + 40.0f;
        }
        distance = sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) +
                         (z1 - z0) * (z1 - z0));
        if (collision_castRayWorld(x0, y0, z0, x1 - x0, y1 - y0, z1 - z0, NULL, NULL, &hitX,
                          &hitY, &hitZ, &hitDistance, NULL, NULL, 0.0f) == 0) {
            blocked = 1;
        } else if (distance < hitDistance) {
            blocked = 1;
        }
        seen = blocked;
        if (blocked && mp_castWallRay(x0, y0, z0, x1 - x0, z1 - z0) > 0.0f) {
            blocked = 0;
        }
    }
    if (blocked) {
        gMpGame->sightBits[a] |= 1 << b;
        gMpGame->sightBits[b] |= 1 << a;
    }
    if (seen) {
        gMpGame->sightBits[a] |= 0x10000 << b;
        gMpGame->sightBits[b] |= 0x10000 << a;
    }
}

/*
 * Refreshes the line-of-sight bits (sightBits) between players: if `checks` is nonzero,
 * each human against its three nearest opponents, then `checks` round-robin pairs.
 * Returns player's bits.
 */
s32 mp_updateLineOfSight(s32 player, s32 checks) {
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

    cur = gMpGame->losPlayer;
    other = gMpGame->losOther;
    list = gMpGame->team;
    if (checks != 0) {
        for (i = 0; i < gHumanCount; i++) {
            x0 = movement_obj[i].xpos;
            x1 = movement_obj[i].xpos;
            for (k = 0; k < 3; k++) {
                idx[k] = -1;
                dist[k] = 900000000.0f;
            }
            for (k = gHumanCount; k < gPlayerCount; k++) {
                if (list[i] != list[k] &&
                    !(gMpGame->sightBits[i] & (0x10001 << k))) {
                    /* BUG (original game): measures player i against itself, not k */
                    dx = x0 - movement_obj[i].xpos;
                    dz = x1 - movement_obj[i].zpos;
                    if (i == gMpGame->target[k]) {
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
                    gMpGame->sightBits[i] &= ~(0x10001 << j);
                    gMpGame->sightBits[j] &= ~(0x10001 << i);
                    mp_checkLineOfSight(i, j);
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
                if (other >= gPlayerCount) {
                    cur++;
                    if (cur >= gPlayerCount - 1) {
                        cur = 0;
                    }
                    obj = &movement_obj[cur];
                    team = obj->team;
                    other = cur + 1;
                }
                gMpGame->sightBits[cur] &= ~(0x10001 << other);
                gMpGame->sightBits[other] &= ~(0x10001 << cur);
                mask = (1 << cur) | (1 << other);
                m++;
            } while (m < 64 && ((gEliminatedPlayers & mask) ||
                                    team == movement_obj[other].team));
            mp_checkLineOfSight(cur, other);
        } while (--checks != 0);
    }
    gMpGame->losPlayer = cur;
    gMpGame->losOther = other;
    return gMpGame->sightBits[player];
}

/*
 * Advances nextBot round-robin over the gPlayerCount actors; returns the next active one with
 * mv->bot set that isn't in gEliminatedPlayers, or -1.
 */
s32 mp_nextBot(void) {
    s32 count;
    s32 ret;
    s8 cur;
    Actor *obj;
    ActorCtl *state;

    count = 0;
    ret = -1;
    cur = gMpGame->nextBot;
    do {
        cur++;
        if (cur >= gPlayerCount) {
            cur = 0;
        }
        obj = &movement_obj[cur];
        if (obj->type != 0) {
            state = obj->mv;
            if (state != NULL && state->bot != 0 && !(gEliminatedPlayers & (1 << cur))) {
                count = gPlayerCount;
                ret = cur;
            }
        }
        count++;
    } while (count < gPlayerCount);

    gMpGame->nextBot = cur;
    return ret;
}

/* Advances nextHuman to the next human player and returns it. */
s32 mp_nextHuman(void) {
    MpGame **p2; // matching: reading back through a pointer to gMpGame
    s8 temp;

    p2 = &gMpGame;
    gMpGame->nextHuman++;
    temp = gMpGame->nextHuman;
    if (temp >= gHumanCount) {
        gMpGame->nextHuman = 0;
        temp = (*p2)->nextHuman;
    }
    return temp;
}

/*
 * Steers bot `player` toward player `target`: eases the smoothed aim point (aimX/aimY/aimZ)
 * toward the target (its position, or its 0x1A4 point if aimAtPoint), offset sideways by
 * sideOffset and down by dropOffset, turns the actor's yaw/pitch (or the camera pad's stick)
 * to it and presses Z_TRIG/R_TRIG when it should fire. Returns 1 while the yaw error exceeds 45 degrees.
 */
s32 mp_aimBot(s32 player, s32 target, s32 fire, s32 aimAtPoint, s32 sideOffset, s32 dropOffset) {
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
        victim = actor_findOnPath(target & 0xFFFF0FFF);
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
        if (gMpGame->gameType == 0xB9 || gBotDifficulty >= 3) {
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
                V = (dy * V) / gMpGame->ticks;
                dx = (victim->xpos - victim->old_xpos) * V;
                dz = (victim->zpos - victim->old_zpos) * V;
            }
        }
        if (aimAtPoint == 0) {
            dx = (victim->xpos + dx) - gMpGame->aimX[player];
            dz = (victim->zpos + dz) - gMpGame->aimZ[player];
        } else {
            dx = (victim->aim.x + dx) - gMpGame->aimX[player];
            dz = (victim->aim.z + dz) - gMpGame->aimZ[player];
        }
        scale = gMpGame->aimRate[player] * gFrameTicks;
        if (type == 3) {
            scale = 1.0f;
            dropOffset = 0;
            sideOffset = 0;
        }
        if (scale > 1.0f) {
            scale = 1.0f;
        }
        gMpGame->aimX[player] += dx * scale;
        gMpGame->aimZ[player] += dz * scale;
        dx = gMpGame->aimX[player] - obj->xpos;
        dz = gMpGame->aimZ[player] - obj->zpos;
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
        ang = actor_getDirectionAngle(dx, -dz, 0) * 0.005493164f;
        if (type != 3) {
            curYaw = obj->mv->aimYaw;
        } else {
            curYaw = obj->theta - 90.0f;
        }
        ang = math_wrapDegrees(ang);
        curYaw = math_wrapDegrees(curYaw);
        diff = math_wrapDegrees(curYaw - ang);
        if (diff > 180.0f) {
            diff = -360.0f + diff;
        }
        if (diff > 45.0f || diff < -45.0f) {
            result = 1;
        }
        if (aimAtPoint == 0) {
            cur = ((victim->ypos + height) - dropOffset) - gMpGame->aimY[player];
        } else {
            cur = (victim->aim.y - (f32)dropOffset) - gMpGame->aimY[player];
        }
        gMpGame->aimY[player] += cur * scale;
        cur = obj->mv->aimEye.y - gMpGame->aimY[player];
        ang = actor_getDirectionAngle(dist, cur, 0) * 0.005493164f;
        curYaw = obj->mv->aimPitch;
        while (ang > 180.0f) {
            ang -= 360.0f;
        }
        while (curYaw > 180.0f) {
            curYaw -= 360.0f;
        }
        V = math_wrapDegrees(curYaw - ang);
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
            math_normalizeDegrees(&obj->mv->aimYaw);
            obj->mv->aimYawTarget = obj->mv->aimYaw;
            obj->mv->aimPitchTarget = obj->mv->aimPitch;
            obj->mv->stick_x = 0;
            obj->mv->stick_y = 0;
        } else if (hasCam != 0) {
            ptr = &cam->TgetFOVX;
            cam->PadPtr->stick_x = yaw;
            cam->PadPtr->stick_y = -roll;
        } else {
            ptr = &obj->mv->unk90;
            obj->mv->stick_x = yaw;
            obj->mv->stick_y = -roll;
        }
        if (mode != 0) {
            if (type == 0x1B) {
                if (h < -50.0f && (gMpGame->sightBits[player] & (0x10001 << target))) {
                    obj->mv->buttons |= Z_TRIG;
                    obj->mv->buttonsTimer = 2;
                }
            } else if (type == 3) {
                f32 f;

                f = (dist - 136.0f) / 664.0f;
                if (f < 0.0f) {
                    f = 0.0f;
                }
                f = (sinf((gTotalTicks & 0x3F) * 0.09815625101f) * 0.174999997f) + f;
                if (f > 1.0f) {
                    f = 1.0f;
                }
                f = (f * 0.02569000237f) + 0.01724099927f;
                obj->mv->stick_y = 0;
                obj->mv->stick_x = 0;
                obj->angle -= (s32)(yaw * 16.0f);
                obj->langle = obj->angle;
                obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
                trail_setPissSpeedScale(obj, f);
            } else if (gMpGame->attackTimer[player] >= 0) {
                obj->mv->buttons = Z_TRIG;
                if (mode == 1) {
                    obj->mv->buttonsTimer = 2;
                    if (gBotFireHoldTimes[gBotDifficulty] < gMpGame->attackTimer[player]) {
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
 * Copies player's target to its state (mv->targetIndex). Once its attackTimer passes the limit for
 * this level and distance it is reset, with mp_sendPlayerCommand(player, 0x61, 0x2000) if the target
 * is in sight; while the timer is 0, unk12A is set at random.
 */
s32 mp_updateBotAttack(s32 player) {
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
    targets = (s8 *)gMpGame->target;
    timers = (s8 *)gMpGame->attackTimer;
    obj = &movement_obj[player];
    off = player * 4;
    target = (s32 *)(targets + off);
    obj->mv->targetIndex = *target;
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
    limit = ((s16 *)gBotAttackTimeLimits)[gBotDifficulty * 3 + range]; // matching: the [level][range] form computes the address differently
    if (limit < *(s32 *)(timers + off)) {
        *(s32 *)(timers + off) = 0;
        if (*(s32 *)((s8 *)gMpGame->sightBits + off) & (0x10000 << *target)) {
            mp_sendPlayerCommand(player, 0x61, 0x2000);
        }
    }
    if (*(s32 *)(timers + off) == 0) {
        obj->mv->unk12A = 0;
        if (gBotAimAccuracy[gBotDifficulty] < (s32)(math_randomFloat() * 100.0f)) {
            obj->mv->unk12A = 1;
        }
    }
}

/* Resets the pickup slots: nobody holds one and each respawns on the next update. */
s32 mp_resetSlots(void) {
    s32 i;
    s32 *slots = gMpGame->slot;
    s32 *owners = gMpGame->slotOwner;
    s32 *overrides = gMpGame->itemOverride;
    s32 *timers = gMpGame->slotTimer;
    s32 *spawns = gMpGame->slotSpawn;
    s32 *dropTimers = gMpGame->slotDropTimer;

    for (i = 0; i < gMpGame->numPlayers; i++) {
        slots[i] = -1;
    }
    for (i = 0; i < gMpGame->numSlots; i++) {
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
s32 mp_releaseSlot(s32 player, s32 mode) {
    MpGame *game;
    s32 slot;
    s32 next;
    Actor *obj;

    game = gMpGame;
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
        if ((gMpGame->slotFlags[slot] & 2) == 0) {
            gMpGame->slotOwner[slot] = -1;
            gMpGame->slotSpawn[slot] = -1;
            next = gMpGame->respawnDelay;
            if (mode == 1) {
                gMpGame->slotDropTimer[slot] = 300;
                obj = &movement_obj[player];
                next = 4;
                gMpGame->slotDropX[slot] = obj->xpos;
                gMpGame->slotDropY[slot] = obj->ypos + 50.0f;
                gMpGame->slotDropZ[slot] = obj->zpos;
            }
            gMpGame->slotTimer[slot] = next;
        }
    }
    return 0;
}




/*
 * Per-frame pickup update. Gives each player the item of its override, its slot or the default
 * (swapping attachments when it changes), then moves the slot pickups: drop timers, respawns at
 * a spawn node, and handing a touched pickup to its player (0x8AE adds 3 grenades, up to 6).
 * Declared int but returns no value, in the pre-ANSI way: script_miscCommand returns what it leaves in v0.
 */
s32 mp_updatePickups(void) {
    static s8 gPickupCheckPlayer = 0;
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

    base = gMpGame;
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
            setId = mp_getItemAttachSet(prev);
            if (setId != -1) {
                setId &= ~0x1000;
                mp_sendPlayerCommand(i, 0x4C, setId);
                if (setId == 0x1D) {
                    mp_sendPlayerCommand(i, 0x4C, 0x1E);
                }
                movement_obj[i].mv->weapon_status = 0;
            }
            setId = mp_getItemAttachSet(cur);
            if (setId > 0x1000) {
                setId &= ~0x1000;
                mp_sendPlayerCommand(i, 0x4B, setId);
            }

            if (gMpGame->gameType == 0xB8) {
                if (prev == 0x80) {
                    actor_fadeFogPulse(&movement_obj[i]);
                    movement_obj[i].imune = 0;
                }
                movement_obj[i].mv->unk128 = cur;
                if (cur == 1) {
                    movement_obj[i].mv->unk12C = 600;
                } else if (cur == 0x80) {
                    movement_obj[i].mv->unk12C = 1800;
                    actor_startFogPulse(&movement_obj[i], 0, 0, 0xFF, 0xBF, 4);
                } else if (cur == 0x10) {
                    movement_obj[i].mv->unk12C = 3;
                }
            } else {
                text_lockObjectText(i, 1, cur, 0);
                if (cur == 0 && prev != 3) {
                    movement_obj[i].mv->rt_context_b = 0;
                }
                if (prev == 0x20) {
                    mp_sendPlayerCommand(i, 0x45, 4);
                }
                if (cur == 0x20) {
                    mp_sendPlayerCommand(i, 0x45, 3);
                }
            }
        }
        base = gMpGame;
        if (gPickupCheckPlayer == i && base->gameType != 0xB8) {
            setId = mp_getItemAttachSet(cur);
            found = 0;
            numAttached = attach_getAll(&movement_obj[i], attached);
            if (numAttached == 0 && movement_obj[i].mv->weapon_status == 2 && setId != 0x55 && setId != 0x2E) {
                collision_createPartSet(&movement_obj[i], setId, 1.0f, 0);
                if (setId == 0x1D) {
                    collision_createPartSet(&movement_obj[i], 0x1E, 1.0f, 0);
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
                            attach_free(attached[j], 0);
                            movement_obj[i].mv->weapon_status = 0;
                        } else if (setId == attached[j]->setId) {
                            found = 1;
                        }
                    }
                    if (found == 0 && movement_obj[i].mv->weapon_status == 2 && setId != 0x55 && setId != 0x2E) {
                        collision_createPartSet(&movement_obj[i], setId, 1.0f, 0);
                        if (setId == 0x1D) {
                            collision_createPartSet(&movement_obj[i], 0x1E, 1.0f, 0);
                        }
                    }
                } else {
                    for (j = 0; j < numAttached; j++) {
                        if (attached[j]->setId != 0x2A && attached[j]->setId != 0x22 &&
                            attached[j]->setId != 0x2C && attached[j]->setId != 0x64 &&
                            attached[j]->setId != 0x2D && attached[j]->setId != 0x72 &&
                            attached[j]->setId != 0x73 && attached[j]->setId != 0x76 &&
                            attached[j]->setId != 0x77 && attached[j]->setId != 0x78 && attached[j]->setId != 7) {
                            attach_free(attached[j], 0);
                            movement_obj[i].mv->weapon_status = 0;
                        }
                    }
                }
            }
            if (setId != 0x55) {
                s32 m;
                for (m = gMpGame->numPlayers; m < 25; m++) {
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
                    particle_stopObjEffect(other);
                    actor_remove(other, 0);
                    break;
                }
                // matching: empty tests of the original source; without them IDO allocates registers differently
                if (setId == 0x7B) {}
                if (numAttached == 0x7C) {}
                if (setId == 0x55) {}
            }
            base = gMpGame;
        }
        count = base->numPlayers;
    }
    gPickupCheckPlayer++;
    if (gPickupCheckPlayer >= count) {
        gPickupCheckPlayer = 0;
    }
    for (i = 0; i < base->numSlots; i++) {
        pickup = gfx_findObjectById(entities[i]);
        if (pickup == NULL) {
            base = gMpGame;
            continue;
        }
        if (dropTimers[i] != 0) {
            dropTimers[i] -= gFrameTicks;
            if (dropTimers[i] <= 0) {
                timers[i] = 4;
                dropTimers[i] = 0;
            }
        }
        if (timers[i] > 0) {
            timers[i] -= gFrameTicks;
            if (timers[i] <= 0) {
                timers[i] = 0;
                pickup->inactive = 0;
                if (dropTimers[i] != 0) {
                    pickup->x = dropX[i];
                    pickup->y = dropY[i];
                    pickup->z = dropZ[i];
                } else if (!(flags[i] & 4)) {
                    if (flags[i] & 1) {
                        x = gMpGame->numSpawns;
                        do {
                            r = math_randomFloat() * 0.9999899864f * x;
                            for (k = 0; k < gMpGame->numSpawns; k++) {
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
                    idx = mp_findNodeById(gMpGame->spawnNodes[r]);
                    if (idx != -1) {
                        mp_getNodePos(idx, &x, &y, &z);
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
                    timers[i] = gMpGame->respawnDelay;
                } else {
                    mp_releaseSlot(j, 0);
                    if (flags[i] & 2) {
                        timers[i] = gMpGame->respawnDelay;
                    } else {
                        timers[i] = -1;
                        owners[i] = j | 0x2000;
                        spawns[i] = -1;
                    }
                    dropTimers[i] = 0;
                    slots[j] = i;
                    gMpGame->unk5C[j] = 1;
                }
                kind = gMpGame->gameType;
                if (kind == 0xB9) {
                    movement_obj[j].mv->unk12C = 600;
                }
                kind = gMpGame->gameType;
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
        base = gMpGame;
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
s32 mp_getItemAttachSet(s32 type) {
    if (gMpGame->gameType != 0xB8) {
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
s32 mp_takeOverPlayer(s32 player, s32 source) {
    Actor *obj;
    s32 savedRot;
    s32 bit0;
    s32 tmp;
    u8 slot;
    MpGame *res;

    obj = &movement_obj[player];
    if (gEliminatedSquareSizes[player] > 50) {
        if (!(gTakeoverPlayers & (1 << player))) {
            bit0 = 1 << player;
            gTakeoverPlayers |= bit0;
            gEliminatedPlayers &= ~bit0;
            gEliminatedPlayers |= (1 << source);
            tmp = player_getStat(source, 3);
            player_setStat(player, 2, tmp);
            player_setStat(source, 2, 0);
            if (gMpGame->seenMask != 0) {
                gMpGame->seenMask &= ~bit0;
                gMpGame->seenMask |= (1 << source);
            }
            if (gMpGame->layer) {
                gMpGame->layer[player] = gMpGame->layer[source];
            }
            if (gMpGame->unk39C) {
                gMpGame->unk39C[player] = gMpGame->unk39C[source];
            }
            if (gMpGame->attacker) {
                gMpGame->attacker[player] = gMpGame->attacker[source];
            }
            if (gMpGame->slot) {
                gMpGame->slot[player] = gMpGame->slot[source];
                if (gMpGame->slot[player] >= 0) {
                    gMpGame->slotOwner[gMpGame->slot[player]] = player;
                }
            }
            gMpGame->sightBits[player] = gMpGame->sightBits[source];
            savedRot = obj->mv->shotsfired;
            actor_swapSlots(&movement_obj[source], obj);
            if (level == LEVEL_WMP_RAPTOR) {
                if (obj->lifting >= 100) {
                    slot = obj->lifting - 100;
                    res = script_getTaskBss(gCurrentTaskId);
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
            camera_placeAtObject(obj->CameraPtr, obj);
            gHumansRemaining++;
        }
    }
    return 0;
}

/* Calls script_command(1, actor | 0x2000, id, param) for player's actor (unk13F). */
void mp_sendPlayerCommand(s32 player, s32 id, s32 param) {
    s32 temp = movement_obj[player].listpos;
    script_command(1, temp | 0x2000, id, param);
}

/* mp_sendPlayerCommand(player, 0x14, param). */
void mp_sendPlayerCommand14(s32 player, s32 param) {
    s32 temp = movement_obj[player].listpos;
    script_command(1, temp | 0x2000, 0x14, param);
}

/* mp_sendPlayerCommand(player, 0x61, param). */
void mp_sendPlayerCommand61(s32 player, s32 param) {
    s32 temp = movement_obj[player].listpos;
    script_command(1, temp | 0x2000, 0x61, param);
}

/* Finds the entry of kind `kind` linked to (type, id); returns its id or -1. */
s32 mp_findLinkedEntry(u16 type, u16 id, u16 kind) {
    u32 i;
    u32 j;
    u32 k;
    u16 key;

    key = (type << 12) + id;
    for (i = 0; i < gLinkGroupCount; i++) {
        for (j = 0; j < gLinkGroups[i].count; j++) {
            if (key == gLinkGroups[i].members[j]) {
                for (k = 0; k < gLinkGroups[i].count; k++) {
                    if (kind == (gLinkGroups[i].members[k] >> 12) && k != j) {
                        return gLinkGroups[i].members[k] & 0xFFF;
                    }
                }
                return -1;
            }
        }
    }
    return -1;
}

/* Clears link group `index`. */
void mp_clearLinkGroup(s32 index) {
    if (index < gLinkGroupCount) {
        gLinkGroups[index].count = 0;
        gLinkGroups[index].unk4 = 0;
        gLinkGroups[index].unk0 = 0;
    }
}

/* Fires every entry of the link group containing (type, id), then clears the group. */
void mp_fireLinkGroup(u16 type, u16 id) {
    u32 i;
    u32 j;
    s32 operand;
    u16 key;

    key = (type << 12) + id;
    for (i = 0; i < gLinkGroupCount; i++) {
        for (j = 0; j < gLinkGroups[i].count; j++) {
            if (gLinkGroups[i].members[j] == key) {
                for (j = 0; j < gLinkGroups[i].count; j++) {
                    operand = gLinkGroups[i].members[j] & 0xFFF;
                    switch (gLinkGroups[i].members[j] >> 12) {
                    case 2:
                        gfx_removeObject(operand);
                        break;
                    case 3:
                        aim_removePoint(operand);
                        break;
                    }
                }
                mp_clearLinkGroup(i);
                return;
            }
        }
    }
}
