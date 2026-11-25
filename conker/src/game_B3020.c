#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A node of the level's path graph (loaded by func_15085B70), 0x10 bytes. */
typedef struct PathNode {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 layer; /* height band, see func_15085DA8 */
    /* 0x7 */ u8 unk7;
    /* 0x8 */ u8 unk8;
    /* 0x9 */ u8 links[5]; /* neighbouring nodes, 0xFF = none */
    /* 0xE */ u8 type;
    /* 0xF */ u8 param;
} PathNode;

/* Path graph resource: a node count and an unknown halfword, then the nodes. */
typedef struct PathGraph {
    /* 0x0 */ s16 count;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ PathNode nodes[1];
} PathGraph;

extern s16 D_80087290; /* node count */
extern s16 D_80087294;
extern PathNode *D_800D2350; /* nodes */
PathGraph *func_1502B5C8(s32 *, s32, s32, s32);
s32 func_15085BE8(void);

/* Loads path graph resource `id` and splits the height bands. */
void func_15085B70(s32 id) {
    PathGraph *graph;

    graph = func_1502B5C8(NULL, 2, 25, id);
    if (graph == NULL) {
        D_80087290 = 0;
        D_80087294 = 0;
        D_800D2350 = NULL;
    } else {
        D_80087290 = graph->count;
        D_80087294 = graph->unk2;
        D_800D2350 = graph->nodes;
    }
    func_15085BE8();
}

extern u8 D_800D2358; /* number of used height bands */
extern f32 D_800D2360[]; /* upper y bound of each used band */
extern u8 D_800D237C[]; /* layer index of each used band */

/*
 * Groups the nodes by layer, records the used layers in D_800D237C and the y value separating
 * each pair of adjacent bands in D_800D2360. Returns whether any two bands overlap in y.
 */
s32 func_15085BE8(void) {
    s32 i;
    s32 overlap;
    s32 layer;
    f32 y;
    s32 lo;
    s32 hi;
    f32 minY[6];
    f32 maxY[6];

    overlap = FALSE;
    for (i = 0; i < 6; i++) {
        minY[i] = 10000.0f;
        maxY[i] = -10000.0f;
    }
    for (i = 0; i < D_80087290; i++) {
        layer = D_800D2350[i].layer;
        y = D_800D2350[i].y;
        if (y < minY[layer]) {
            minY[layer] = y;
        }
        if (maxY[layer] < y) {
            maxY[layer] = y;
        }
    }
    D_800D2358 = 0;
    for (i = 0; i < 6; i++) {
        if (minY[i] <= maxY[i]) {
            D_800D237C[D_800D2358] = i;
            D_800D2358++;
        }
    }
    for (i = 0; i < D_800D2358 - 1; i++) {
        lo = D_800D237C[i];
        hi = D_800D237C[i + 1];
        D_800D2360[i] = (maxY[lo] + minY[hi]) * 0.5f;
        if (maxY[lo] > minY[hi]) {
            overlap = TRUE;
        }
    }
    D_800D2360[i] = 100000.0f;
    return overlap;
}

extern u8 D_8008729C;
extern s32 D_800D2354;
extern f32 sqrtf(f32);
f32 func_15086D94(f32, f32, f32, f32, f32);

/* Returns the layer of the height band containing `y`. */
u8 func_15085DA8(f32 y) {
    s32 i = 0;

    while (D_800D2360[i] <= y) {
        i++;
    }
    return D_800D237C[i];
}

/*
 * Returns the node nearest to (x, y, z) whose layer is `layer` and type is `type` (-1 matches any),
 * or 0xFF. D_8008729C, if set, names a node the result must be nearer than (it is then cleared).
 * With type 0 a node only counts when func_15086D94 is negative for its offset. Stores the
 * distance in D_800D2354.
 */
s32 func_15085DF8(f32 x, f32 y, f32 z, s8 type, s8 layer) {
    PathNode *node;
    f32 bestDistSq;
    f32 distSq;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 checkSight;
    s32 i;
    s32 found;

    checkSight = FALSE;
    bestDistSq = 100000000.0f;
    if (type == 0) {
        checkSight = TRUE;
    }
    if (D_8008729C != 0xFF) {
        node = &D_800D2350[D_8008729C];
        dx = node->x - x;
        dy = node->y - y;
        dz = node->z - z;
        if (!checkSight || (checkSight && func_15086D94(x, y, z, dx, dz) < 0.0f)) {
            bestDistSq = dx * dx + dy * dy + dz * dz + 10.0f;
        }
        D_8008729C = 0xFF;
    }
    found = 0xFF;
    for (i = 0; i < D_80087290; i++) {
        node = &D_800D2350[i];
        if ((layer == node->layer || layer == -1) && (type == node->type || type == -1)) {
            dx = node->x - x;
            dy = node->y - y;
            dz = node->z - z;
            distSq = dx * dx + dy * dy + dz * dz;
            if (distSq < bestDistSq && (!checkSight || (checkSight && func_15086D94(x, y, z, dx, dz) < 0.0f))) {
                bestDistSq = distSq;
                found = i;
            }
        }
    }
    D_800D2354 = sqrtf(bestDistSq);
    return found;
}

s32 func_15086C70(s32 arg0);
s32 func_150A32B4(void *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_15086364(s32 arg0, s8 arg1, s8 arg2, u8 *arg3, u8 *arg4);

s32 func_15086098(f32 x, f32 y, f32 z, s8 arg3, s8 arg4, s32 arg5, s32 arg6) {
    s32 idx;
    s32 ptr;
    PathNode *entry;
    s32 result;
    s32 i;
    s32 best;
    PathNode *other;
    s32 n;
    u8 *links;
    f32 bestDist;
    f32 rad;
    f32 dx;
    f32 dz;
    f32 ex;
    f32 ez;
    f32 inv;
    f32 t;
    f32 d;
    f32 px;
    f32 pz;
    f32 dist;

    result = 0xFF;
    idx = func_15085DF8(x, y, z, 0, func_15085DA8(y));
    if (arg3 > 0 && idx != 0xFF) {
        ptr = func_15086C70(idx);
        if (ptr != 0 && func_150A32B4((void *)ptr, (s32)x, (s32)y, (s32)z) != 0) {
            result = func_15086364(idx, arg3, arg4, arg5, arg6);
        } else {
            entry = &D_800D2350[idx];
            rad = (u32)((u8 *)entry)[8] * 2.0f;
            if (100000000.0f < rad * rad) {
                result = func_15086364(idx, arg3, arg4, arg5, arg6);
            } else {
                bestDist = 100000000.0f;
                best = 0xFF;
                i = 0;
                links = (u8 *)entry;
                do {
                    n = links[9];
                    i = (i + 1) | 0;
                    if (n != 0xFF) {
                        other = (PathNode *)((u8 *)D_800D2350 + n * 16);
                        dx = other->x - entry->x;
                        dz = other->z - entry->z;
                        inv = 1.0f / sqrtf(dx * dx + dz * dz);
                        dx *= inv;
                        dz *= inv;
                        d = -(entry->x * dx + entry->z * dz);
                        t = x * dx + z * dz + d;
                        if (t > 0.0f) {
                            dx = x - (entry->x + dx * t);
                            dz = z - (entry->z + dz * t);
                            dist = dx * dx + dz * dz;
                            if (dist < bestDist) {
                                best = n;
                                bestDist = dist;
                            }
                        }
                    }
                    links++;
                } while (i < 5);
                if (best != 0xFF) {
                    n = func_15086364(idx, arg3, arg4, arg5, arg6);
                    if (n == best) {
                        result = n;
                    }
                }
            }
        }
    }
    if (result != 0xFF) {
        idx = result;
    }
    return idx;
}

extern u8 D_80087298;
f32 func_15086BD0(s32 arg0, s32 arg1);

s32 func_15086364(s32 start, s8 nGoals, s8 nAvoid, u8 *goals, u8 *avoid) {
    s32 i;
    s32 j;
    s32 k;
    s32 count;
    s32 pathLen;
    s32 found;
    s32 penalty;
    s32 best;
    s32 slot;
    s32 d;
    s32 n;
    f32 total;
    s32 curDist;
    s32 doneFirst;
    s32 cur;
    u8 nb2;
    u8 result;
    f32 dx;
    f32 dy;
    f32 dz;
    PathNode *node;
    PathNode *other;
    s32 dist[200];
    u8 open[200];
    u8 prev[200];
    u8 path[64];
    u8 flags[64];

    D_80087298 = -1;
    D_800D2354 = 0;
    if (nGoals <= 0) {
        return 0xFF;
    }
    if (start < 0 || start >= D_80087290) {
        return 0xFF;
    }
    found = 1;
    for (j = 0; j < nGoals; j++) {
        if (start == goals[j]) {
            found = 0;
        }
    }
    count = 0;
    if (!found) {
        D_80087298 = start;
        return start;
    }
    for (i = 0; i < D_80087290; i++) {
        dist[i] = 1000000;
    }
    node = &D_800D2350[start];
    for (i = 0; i < 5; i++) {
        nb2 = node->links[i];
        if (nb2 != 0xFF) {
            penalty = 0;
            for (j = 0; j < nAvoid; j++) {
                if (nb2 == avoid[j]) {
                    penalty = 5000;
                }
            }
            other = &D_800D2350[nb2];
            open[count] = nb2;
            prev[node->links[i]] = start;
            dx = node->x - other->x;
            dy = node->y - other->y;
            dz = node->z - other->z;
            dist[node->links[i]] = (s16)(s32)sqrtf(dx * dx + dy * dy + dz * dz) + penalty;
            count++;
        }
    }
    pathLen = 0;
    do {
        best = 1000000;
        k = 0xFF;
        for (n = 0; n < count; n++) {
            if (open[n] != 0xFF) {
                if (dist[open[n]] < best) {
                    best = dist[open[n]];
                    k = n;
                }
            }
        }
        if (k != 0xFF) {
            if (k == count - 1) {
                count--;
            }
            i = open[k];
            open[k] = 0xFF;
            found = 0;
            for (j = 0; j < nGoals; j++) {
                if (i == goals[j]) {
                    found = 1;
                }
            }
            if (found) {
                D_80087298 = i;
                n = i;
                if (&path[pathLen] == 0) {} /* no code; gives uopt a real &path use in this loop (register order in the tail) */
                while (n != start) {
                    path[pathLen++] = n;
                    n = prev[n];
                }
                path[pathLen++] = n;
                k = 0xFF;
            } else {
                curDist = dist[i];
                node = &D_800D2350[i];
                cur = i;
                for (i = 0; i < 5; i++) {
                    n = node->links[i];
                    if (n != 0xFF && n != start) {
                        penalty = 0;
                        for (j = 0; j < nAvoid; j++) {
                            if (node->links[i] == avoid[j]) {
                                penalty = 5000;
                            }
                        }
                        other = &D_800D2350[n];
                        dx = node->x - other->x;
                        dy = node->y - other->y;
                        dz = node->z - other->z;
                        d = (s32)sqrtf(dx * dx + dy * dy + dz * dz) + curDist + penalty;
                        if (d < dist[n]) {
                            slot = 0xFF;
                            for (k = 0; k < count; k++) {
                                if (slot == 0xFF && open[k] == 0xFF) {
                                    slot = k;
                                }
                                if (n == open[k]) {
                                    dist[n] = d;
                                    prev[n] = cur;
                                    k = count + 10;
                                }
                            }
                            if (k == count) {
                                prev[n] = cur;
                                dist[n] = d;
                                if (slot == 0xFF) {
                                    slot = count;
                                    count++;
                                }
                                open[slot] = n;
                            }
                        }
                    }
                }
                k = 0;
            }
        }
    } while (k != 0xFF);
    if (pathLen < 2) {
        return 0xFF;
    }
    pathLen -= 2;
    result = path[pathLen];
    for (i = 0; i <= pathLen + 1; ) {
        flags[i] = func_15086C70(path[i]) != 0;
        i++;
    }
    total = 0.0f;
    doneFirst = 0;
    if (flags[pathLen] && (!flags[pathLen + 1] || (pathLen > 0 && !flags[pathLen - 1]))) {
        doneFirst = 1;
    }
    while (pathLen > 0) {
        i = pathLen - 1;
        d = path[pathLen];
        if (flags[pathLen - 1]) {
            while (i >= 0 && flags[i]) {
                i--;
            }
            i++;
        }
        total += func_15086BD0(d, path[i]);
        if (!doneFirst) {
            doneFirst = 1;
            if (flags[pathLen]) {
                result = path[i];
            }
        }
        pathLen = i;
    }
    D_800D2354 = (s32)total;
    return result;
}
