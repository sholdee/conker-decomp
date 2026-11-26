#ifndef CONKER_PATH_H
#define CONKER_PATH_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "path": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Path/PathGraph/pathgraph.c
 */

/* game/Path/PathGraph/pathgraph.c */
void path_load(s32 id);
s32 path_splitBands(void);
u8 path_getLayerAt(f32 y);
s32 path_findNearestNode(f32 x, f32 y, f32 z, s8 type, s8 layer);
s32 path_getNextNode(f32 x, f32 y, f32 z, s8 nGoals, s8 nAvoid, u8 *goals, u8 *avoid);
s32 path_search(s32 start, s8 nGoals, s8 nAvoid, u8 *goals, u8 *avoid);
extern s16 gPathNodeCount;        /* node count */
extern u8 gPathGoalReached;         /* goal node reached by the last search, 0xFF if none */
extern u8 gPathNearerThanNode;         /* a node the next path_findNearestNode result must be nearer than, 0xFF if none */
extern PathNode *gPathNodes;  /* the level path graph nodes (path_load) */
extern s32 gPathDistance;        /* distance to the node found by path_findNearestNode, route length of path_search */

#endif
