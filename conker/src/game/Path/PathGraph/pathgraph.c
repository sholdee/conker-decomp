#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order
 * it first sees each name (see init/Sched/SchedInit/schedinit.c). */
PathNode *gPathNodes; // the level path graph nodes (path_load)
s32 gPathDistance;       // distance to the node found by path_findNearestNode, route length of path_search
static u8 gPathBandCount;      /* number of used height bands */
static f32 gPathBandTops[7];  /* upper y bound of each used band */
static u8 gPathBandLayers[6];   /* layer index of each used band */

#include <math.h>
#include "conker/mp.h"
#include "conker/path.h"
#include "conker/rom.h"
#include "conker/zone.h"

/* This file's .data. */
s16 gPathNodeCount = 0;     /* node count */
static s16 D_80087294 = 0; /* unk2 of the path graph resource */
u8 gPathGoalReached = 0xFF;   /* goal node reached by the last search, 0xFF if none */
u8 gPathNearerThanNode = 0xFF;   /* a node the next path_findNearestNode result must be nearer than, 0xFF if none */

/* Path graph resource: a node count and an unknown halfword, then the nodes. */
typedef struct PathGraph {
    /* 0x0 */ s16 count;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ PathNode nodes[1];
} PathGraph;

/* Loads path graph resource `id` and splits the height bands. */
void path_load(s32 id) {
    PathGraph *graph;

    graph = rom_loadPath(NULL, 2, 25, id);
    if (graph == NULL) {
        gPathNodeCount = 0;
        D_80087294 = 0;
        gPathNodes = NULL;
    } else {
        gPathNodeCount = graph->count;
        D_80087294 = graph->unk2;
        gPathNodes = graph->nodes;
    }
    path_splitBands();
}

/*
 * Groups the nodes by layer, records the used layers in gPathBandLayers and the y value separating
 * each pair of adjacent bands in gPathBandTops. Returns whether any two bands overlap in y.
 */
s32 path_splitBands(void) {
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
    for (i = 0; i < gPathNodeCount; i++) {
        layer = gPathNodes[i].layer;
        y = gPathNodes[i].y;
        if (y < minY[layer]) {
            minY[layer] = y;
        }
        if (maxY[layer] < y) {
            maxY[layer] = y;
        }
    }
    gPathBandCount = 0;
    for (i = 0; i < 6; i++) {
        if (minY[i] <= maxY[i]) {
            gPathBandLayers[gPathBandCount] = i;
            gPathBandCount++;
        }
    }
    for (i = 0; i < gPathBandCount - 1; i++) {
        lo = gPathBandLayers[i];
        hi = gPathBandLayers[i + 1];
        gPathBandTops[i] = (maxY[lo] + minY[hi]) * 0.5f;
        if (maxY[lo] > minY[hi]) {
            overlap = TRUE;
        }
    }
    gPathBandTops[i] = 100000.0f;
    return overlap;
}

/* Returns the layer of the height band containing `y`. */
u8 path_getLayerAt(f32 y) {
    s32 i = 0;

    while (gPathBandTops[i] <= y) {
        i++;
    }
    return gPathBandLayers[i];
}

/*
 * Returns the node nearest to (x, y, z) whose layer is `layer` and type is `type` (-1 matches any),
 * or 0xFF. gPathNearerThanNode, if set, names a node the result must be nearer than (it is then cleared).
 * With type 0 a node only counts when mp_castWallRay is negative for its offset. Stores the
 * distance in gPathDistance.
 */
s32 path_findNearestNode(f32 x, f32 y, f32 z, s8 type, s8 layer) {
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
    if (gPathNearerThanNode != 0xFF) {
        node = &gPathNodes[gPathNearerThanNode];
        dx = node->x - x;
        dy = node->y - y;
        dz = node->z - z;
        if (!checkSight || (checkSight && mp_castWallRay(x, y, z, dx, dz) < 0.0f)) {
            bestDistSq = dx * dx + dy * dy + dz * dz + 10.0f;
        }
        gPathNearerThanNode = 0xFF;
    }
    found = 0xFF;
    for (i = 0; i < gPathNodeCount; i++) {
        node = &gPathNodes[i];
        if ((layer == node->layer || layer == -1) && (type == node->type || type == -1)) {
            dx = node->x - x;
            dy = node->y - y;
            dz = node->z - z;
            distSq = dx * dx + dy * dy + dz * dz;
            if (distSq < bestDistSq && (!checkSight || (checkSight && mp_castWallRay(x, y, z, dx, dz) < 0.0f))) {
                bestDistSq = distSq;
                found = i;
            }
        }
    }
    gPathDistance = sqrtf(bestDistSq);
    return found;
}

/*
 * Returns the path node to head for from (x, y, z): the nearest node, or the next node of the
 * route from it towards `goals` (path_search) when the point is inside the node's zone, the
 * node is large, or the point lies along the link to that next node.
 */
s32 path_getNextNode(f32 x, f32 y, f32 z, s8 nGoals, s8 nAvoid, u8 *goals, u8 *avoid) {
    s32 nearest;
    TriggerZone *zone;
    PathNode *node;
    s32 result;
    s32 i;
    s32 best;
    PathNode *other;
    s32 n;
    u8 *links;
    f32 bestDistSq;
    f32 size;
    f32 dx;
    f32 dz;
    f32 invLen;
    f32 t;
    f32 d;
    f32 distSq;

    result = 0xFF;
    nearest = path_findNearestNode(x, y, z, 0, path_getLayerAt(y));
    if (nGoals > 0 && nearest != 0xFF) {
        zone = mp_findNodeZone(nearest);
        if (zone != NULL && zone_containsPoint(zone, x, y, z)) {
            result = path_search(nearest, nGoals, nAvoid, goals, avoid);
        } else {
            node = &gPathNodes[nearest];
            size = node->unk8 * 2.0f;
            if (size * size > 100000000.0f) {
                result = path_search(nearest, nGoals, nAvoid, goals, avoid);
            } else {
                /* Find the link whose line (in xz) passes closest to the point, ahead of the node. */
                bestDistSq = 100000000.0f;
                best = 0xFF;
                i = 0;
                links = (u8 *)node; // matching: cursor from the node base (links[9]), not node->links
                do {
                    n = links[9];
                    i = (i + 1) | 0; // matching: the no-op | keeps IDO's slti loop test
                    if (n != 0xFF) {
                        other = (PathNode *)((u8 *)gPathNodes + n * sizeof(PathNode)); // matching: byte offset, not &gPathNodes[n]
                        dx = other->x - node->x;
                        dz = other->z - node->z;
                        invLen = 1.0f / sqrtf(dx * dx + dz * dz);
                        dx *= invLen;
                        dz *= invLen;
                        d = -(node->x * dx + node->z * dz);
                        t = x * dx + z * dz + d;
                        if (t > 0.0f) {
                            dx = x - (node->x + dx * t);
                            dz = z - (node->z + dz * t);
                            distSq = dx * dx + dz * dz;
                            if (distSq < bestDistSq) {
                                best = n;
                                bestDistSq = distSq;
                            }
                        }
                    }
                    links++;
                } while (i < 5);
                if (best != 0xFF) {
                    n = path_search(nearest, nGoals, nAvoid, goals, avoid);
                    if (n == best) {
                        result = n;
                    }
                }
            }
        }
    }
    if (result != 0xFF) {
        nearest = result;
    }
    return nearest;
}

/*
 * Shortest-path search over the path graph from `start` to the nearest of the `nGoals` nodes in
 * `goals`; entering one of the `nAvoid` nodes in `avoid` costs an extra 5000. Returns the next node
 * to move to (skipping ahead over a run of nodes that have a zone, see mp_findNodeZone), `start`
 * itself if it is a goal, or 0xFF. Stores the route length in gPathDistance.
 */
s32 path_search(s32 start, s8 nGoals, s8 nAvoid, u8 *goals, u8 *avoid) {
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
    u8 link;
    u8 result;
    f32 dx;
    f32 dy;
    f32 dz;
    PathNode *node;
    PathNode *other;
    s32 dist[200];
    u8 openList[200]; /* nodes still to visit, 0xFF = free slot */
    u8 prev[200];
    u8 path[64]; /* route, goal first */
    u8 hasZone[64];

    gPathGoalReached = 0xFF;
    gPathDistance = 0;
    if (nGoals <= 0) {
        return 0xFF;
    }
    if (start < 0 || start >= gPathNodeCount) {
        return 0xFF;
    }
    /* `found` is first "start is not a goal". */
    found = TRUE;
    for (j = 0; j < nGoals; j++) {
        if (start == goals[j]) {
            found = FALSE;
        }
    }
    count = 0;
    if (!found) {
        gPathGoalReached = start;
        return start;
    }
    for (i = 0; i < gPathNodeCount; i++) {
        dist[i] = 1000000;
    }

    /* Seed the open list with the neighbours of start. */
    node = &gPathNodes[start];
    for (i = 0; i < 5; i++) {
        link = node->links[i];
        if (link != 0xFF) {
            penalty = 0;
            for (j = 0; j < nAvoid; j++) {
                if (link == avoid[j]) {
                    penalty = 5000;
                }
            }
            other = &gPathNodes[link];
            openList[count] = link;
            prev[node->links[i]] = start;
            dx = node->x - other->x;
            dy = node->y - other->y;
            dz = node->z - other->z;
            dist[node->links[i]] = (s16)sqrtf(dx * dx + dy * dy + dz * dz) + penalty;
            count++;
        }
    }

    pathLen = 0;
    do {
        /* Take the open node with the smallest distance. */
        best = 1000000;
        k = 0xFF;
        for (n = 0; n < count; n++) {
            if (openList[n] != 0xFF) {
                if (dist[openList[n]] < best) {
                    best = dist[openList[n]];
                    k = n;
                }
            }
        }
        if (k != 0xFF) {
            if (k == count - 1) {
                count--;
            }
            i = openList[k];
            openList[k] = 0xFF;
            found = FALSE;
            for (j = 0; j < nGoals; j++) {
                if (i == goals[j]) {
                    found = TRUE;
                }
            }
            if (found) {
                /* Walk back from the goal to start. */
                gPathGoalReached = i;
                n = i;
                if (&path[pathLen] == NULL) {} // matching: code-less &path use in this loop sets uopt's register order for path in the tail
                while (n != start) {
                    path[pathLen++] = n;
                    n = prev[n];
                }
                path[pathLen++] = n;
                k = 0xFF;
            } else {
                /* Relax the neighbours of the visited node. */
                curDist = dist[i];
                node = &gPathNodes[i];
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
                        other = &gPathNodes[n];
                        dx = node->x - other->x;
                        dy = node->y - other->y;
                        dz = node->z - other->z;
                        d = (s32)sqrtf(dx * dx + dy * dy + dz * dz) + curDist + penalty;
                        if (d < dist[n]) {
                            /* Update n in the open list, or add it in the first free slot. */
                            slot = 0xFF;
                            for (k = 0; k < count; k++) {
                                if (slot == 0xFF && openList[k] == 0xFF) {
                                    slot = k;
                                }
                                if (n == openList[k]) {
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
                                openList[slot] = n;
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
    /* path[pathLen + 1] is start, path[pathLen] the node after it. */
    pathLen -= 2;
    result = path[pathLen];
    for (i = 0; i <= pathLen + 1; ) {
        hasZone[i] = mp_findNodeZone(path[i]) != NULL;
        i++; // matching: increment on its own line (-g3 line numbers steer as1)
    }
    /* Sum the route length, treating each run of zone nodes as one straight hop. */
    total = 0.0f;
    doneFirst = FALSE;
    if (hasZone[pathLen] && (!hasZone[pathLen + 1] || (pathLen > 0 && !hasZone[pathLen - 1]))) {
        doneFirst = TRUE;
    }
    while (pathLen > 0) {
        i = pathLen - 1;
        d = path[pathLen];
        if (hasZone[pathLen - 1]) {
            while (i >= 0 && hasZone[i]) {
                i--;
            }
            i++;
        }
        total += mp_nodeDistance(d, path[i]);
        if (!doneFirst) {
            doneFirst = TRUE;
            if (hasZone[pathLen]) {
                result = path[i];
            }
        }
        pathLen = i;
    }
    gPathDistance = total;
    return result;
}
