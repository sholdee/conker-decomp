#include <ultra64.h>
#include "structs.h"

/* This unit's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c). */
s32 gZoneLastCamera;                 // the camera that collision_updateCameraZone last placed
s32 gZoneLastCameraZone;                 // its zone
ZoneNode *gZoneLastCameraLeaf;           // its zone tree leaf
f32 gZoneLastCameraX;                 // its x
f32 gZoneLastCameraZ;                 // its z
void *gMapTriTables[4];            // per map: triangle table (carved from gTriArenaCursor)
void *gMapTriXZBounds[4];            // per map: x/z bounds
void *gMapTriYBounds[4];            // per map: y bounds
struct QuadNode *gMapQuadTrees[4]; // per map: quadtree root
s32 gMapTriCounts[4];              // per map: triangle count
s16 gQuadNodeBitmapStride;                 // quadtree node bitmap: row stride in bytes
s16 gMap0QuadNodeCount;                 // map 0 quadtree: node count
s16 gMap0QuadLeafCount;                 // map 0 quadtree: leaf count
s16 gViewCameraZones[4];              // per view: camera zone
s32 gMapTriCollected;                 // triangles collected for the current map
MeshTri *gCurMapTris;            // world mesh: triangles
struct XZRange *gCurMapTriXZBounds;     // world mesh: x/z bounds per triangle
struct YRange *gCurMapTriYBounds;      // world mesh: y bounds per triangle
ZoneNode *gCurMapQuadTree;           // zone tree root
s32 gCurMapTriCount;                 // world mesh: triangle count
s32 gCurMap;                 // map whose triangle table is carved next
f32 gFloorQueryFloorY;                 // last floor query: highest floor
f32 gFloorQueryCeilingY;                 // last floor query: ceiling
u32 *gMeshTriMaterials;                // per world-mesh triangle: material / surface flags
u16 gFloorQueryProp;                 // last floor query: prop hit (index + 1, 0: none)
u8 gMeshBuilt;                  // set once the world mesh is built
u8 D_800DBE63;
f32 gFloorQueryReach;                 // floor query reach
f32 gFloorQueryX;                 // last floor query: x
f32 gFloorQueryY;                 // last floor query: y
f32 gFloorQueryZ;                 // last floor query: z
f32 gRayProbeBottom;                 // last ray probe: bottom

#include "functions.h"
#include "variables.h"

/* Per collision set: 1 = enabled. */
u8 gFloorProbeTypeEnabled[4] = { 1, 1, 1, 1 };


#define ABS(x) ((x) < 0 ? -(x) : (x))

/* Returns whether ground hit `index`'s triangle faces up (its normal has y >= 0). */
s32 collision_isHitFacingUp(s32 index) {
    s32 *tri;
    s32 vtxBase;
    s16 *v0;
    s16 *v1;
    s16 *v2;
    f32 edge1[3];
    f32 edge2[3];
    f32 normal[3];
    f32 normalY;

    tri = gGroundCandidates[index].tri;
    vtxBase = gGroundCandidates[index].vtxBase;
    if (tri != NULL) {
        v0 = (s16 *)(tri[0] + vtxBase);
        v1 = (s16 *)(tri[1] + vtxBase);
        v2 = (s16 *)(tri[2] + vtxBase);
        edge1[0] = v1[0] - v0[0];
        edge1[1] = v1[1] - v0[1];
        edge1[2] = v1[2] - v0[2];
        edge2[0] = v2[0] - v0[0];
        edge2[1] = v2[1] - v0[1];
        edge2[2] = v2[2] - v0[2];
        math_cross3(edge1, edge2, normal);
        normalY = normal[1];
        if (normalY < 0.0f) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
    return FALSE;
}


/* Floor query (see collision_findFloorCore) with an explicit height window for the prop probe. */
void collision_findFloorInRange(void **outTri, Vec3s *outVerts, f32 *outFloorY, f32 *outPropFloorY, u32 *outFlags,
                   s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj, f32 minY, f32 maxY) {
    collision_findFloorCore(outTri, outVerts, NULL, outFloorY, outPropFloorY, outFlags, outType, x, y, z, refY, group, obj,
                  minY, maxY, 0);
}

/* Floor query (see collision_findFloorCore) with the default height window. */
void collision_findFloor(void **outTri, Vec3s *outVerts, f32 *outFloorY, f32 *outPropFloorY, u32 *outFlags,
                   s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj) {
    collision_findFloorCore(outTri, outVerts, NULL, outFloorY, outPropFloorY, outFlags, outType, x, y, z, refY, group, obj,
                  -10000.0f, 20000.0f, 0);
}

/* Floor query (see collision_findFloorCore) with the default height window and a noFallback choice. */
void collision_findFloorEx(void **outTri, Vec3s *outVerts, f32 *outFloorY, f32 *outPropFloorY, u32 *outFlags,
                   s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj, f32 unused12,
                   f32 unused13, s32 noFallback) {
    collision_findFloorCore(outTri, outVerts, NULL, outFloorY, outPropFloorY, outFlags, outType, x, y, z, refY, group, obj,
                  -10000.0f, 20000.0f, noFallback);
}


// gMeshTriMaterials points at the world mesh's per-triangle materials, or is NULL.
// World-mesh triangles are 12 bytes (three vertex offsets) from gCurMapTris on.
#define WORLD_TRI_INDEX(tri) (((s32)(tri) - (s32)gCurMapTris) / 12)

#define PROBE_WORLD 0
#define PROBE_PROP 2
#define PROBE_OBJECT 3
#define NUM_PROBES 4

/*
 * Finds the floor under (x, y, z) across every enabled probe type (world mesh, props, objects):
 * the highest upward-facing triangle within reach of y (or of refY when y is below it).
 * Outputs: the floor height, its triangle and vertices, its material flags and probe type; for
 * props, the highest prop surface (and its vertices) below the ceiling; and the lowest downward-facing
 * ceiling above the floor. With an object, also records what it stands on and the ceiling height.
 * Unless noFallback is set, the first upward-facing hit is used when none is within reach.
 */
void collision_findFloorCore(void **outTri, Vec3s *outVerts, s16 (*outPropVerts)[3], f32 *outFloorY, f32 *outPropFloorY,
                   u32 *outFlags, s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *object,
                   f32 minY, f32 maxY, s32 noFallback) {
    s32 i;
    s32 bestHit;
    s32 inReach;
    s32 first;
    s32 type;
    s32 ceilingFlags;
    s32 bestType;
    s32 count;
    s32 total;
    s32 ix;
    f32 bestY;
    s32 iz;
    s32 floorTri;
    s32 found;
    s32 floorVtxBase;
    s32 numProbes;
    f32 height;
    f32 ceilingFrom;
    f32 ceilingY;
    f32 noFloor;
    s32 objectNum;
    u32 dummyFlags;
    s32 triIndex;
    s32 propNum;
    GroundCandidate *hit;
    s32 *tri;
    s16 *vtx;
    s32 vtxBase;
    s32 fallbackHit;
    s32 fallbackType;
    s32 haveFallback;
    s32 allowFallback;
    s32 triAddr;
    volatile f32 reach; // matching: IDO reloads it from the stack on every hit

    noFloor = -10000.0f;
    total = 0;
    floorTri = 0;
    ceilingFlags = 0;
    inReach = FALSE;
    if (outTri != NULL) {
        *outTri = 0;
    }
    if (object != NULL) {
        reach = gFloorQueryReach;
    } else {
        reach = 50.0f;
    }
    gFloorQueryX = x;
    gFloorQueryY = y;
    haveFallback = FALSE;
    gFloorQueryZ = z;
    *outFloorY = noFloor;
    if (outFlags == NULL) {
        dummyFlags = 0;
        outFlags = &dummyFlags;
    }
    ceilingY = 20000.0f;
    gFloorQueryFloorY = noFloor;
    objectNum = 0;
    gFloorQueryProp = 0;
    if (object != NULL) {
        object->floorTriIndex = 0;
    }
    numProbes = NUM_PROBES;
    first = TRUE;
    allowFallback = TRUE;
    if (outPropFloorY != NULL) {
        *outPropFloorY = noFloor;
    }
    for (type = 0; type < numProbes; type++) {
        if (gFloorProbeTypeEnabled[type] == 1) {
            if (type == PROBE_OBJECT && object != NULL && (object->noshove & 0x200)) {
                continue;
            }
            ix = x;
            iz = z;
            i = 0; // matching: dead store, but IDO schedules the loop differently without it
            collision_selectMap(type);
            found = FALSE;
            if (type == PROBE_PROP) {
                s32 floorProps;
                s32 allProps;
                s32 bestPropHit;
                f32 bestPropY;
                s32 propVtxBase;
                s16 *propVtx;

                floorProps = collision_findPropsInBox(ix, iz, &allProps, group, ix, iz, minY, maxY);
                collision_transformPropTris(floorProps, gSurfSetA, 0);
                if (outPropFloorY != NULL) {
                    collision_transformPropTris(allProps, gSurfSetB, 0);
                    bestPropHit = 0;
                    count = collision_gatherPropGroundTris(ix, iz, allProps, gSurfSetB);
                    if (count != 0) {
                        bestPropY = noFloor;
                        for (i = 0; i < count; i++) {
                            height = gGroundCandidates[i].height * 0.00390625f;
                            if (bestPropY < height && height < ceilingY) {
                                bestPropHit = i;
                                bestPropY = height;
                            }
                        }
                        *outPropFloorY = bestPropY;
                    }
                    if (*outPropFloorY != noFloor && outPropVerts != NULL) {
                        tri = gGroundCandidates[bestPropHit].tri;
                        propVtxBase = gGroundCandidates[bestPropHit].vtxBase;
                        for (i = 0; i < 3; i++) {
                            propVtx = (s16 *)(tri[i] + propVtxBase);
                            outPropVerts[i][0] = propVtx[0];
                            outPropVerts[i][1] = propVtx[1];
                            outPropVerts[i][2] = propVtx[2];
                        }
                    }
                }
                count = collision_gatherPropGroundTris(ix, iz, floorProps, gSurfSetA);
            } else if (type == PROBE_OBJECT) {
                count = collision_gatherActorGroundTris(ix, iz);
            } else {
                count = collision_gatherGroundTris(ix, iz);
            }
            total += count;
            if (first) {
                bestY = noFloor;
                bestHit = -1;
                bestType = type;
                first = FALSE;
            }
            for (i = 0; i < count; i++) {
                f32 curReach;

                height = gGroundCandidates[i].height / 256.0f;
                if (noFallback == 0 && allowFallback != 0 && *outFloorY == noFloor
                    && (noFloor == noFloor || height < noFloor)) { // matching: tautology IDO still compiles
                    s32 pad; // matching: IDO reserves a stack slot for this block-scoped unused local
                    if (collision_isHitFacingUp(i)) {
                        haveFallback = TRUE;
                        fallbackHit = i;
                        fallbackType = type;
                    }
                }
                curReach = reach;
                if ((u32)i < 0U) { // matching: never true; the dead test steers IDO's scheduling
                    if (height < refY) {
                        continue;
                    }
                }
                if (gFloorQueryFloorY < height) {
                    gFloorQueryFloorY = height;
                }
                if (y < refY) {
                    if (height < refY + curReach && bestY < height) {
                        inReach = TRUE;
                    }
                } else if (height < y + curReach) {
                    if (height < refY) {
                        if (bestY < height) {
                            inReach = TRUE;
                        }
                    } else if (bestY < refY) {
                        inReach = TRUE;
                    } else if (height < bestY) {
                        inReach = TRUE;
                    }
                }
                if (inReach) {
                    inReach = FALSE;
                    if (collision_isHitFacingUp(i)) {
                        bestType = type;
                        found = TRUE;
                        bestY = height;
                        bestHit = i;
                    }
                }
            }
            if (found || haveFallback) {
                if (!found && haveFallback) {
                    bestHit = fallbackHit;
                    bestType = fallbackType;
                }
                haveFallback = FALSE;
                if (outType != NULL) {
                    *outType = bestType;
                }
                if (bestHit != -1) {
                    hit = &gGroundCandidates[bestHit];
                    allowFallback = FALSE;
                    *outFloorY = hit->height / 256.0f;
                    if (*outFloorY < noFloor) {
                        *outFloorY = noFloor;
                    }
                    triAddr = (s32)hit->tri;
                    floorTri = triAddr;
                    vtxBase = hit->vtxBase;
                    if (type != PROBE_OBJECT) {
                        if (object != NULL && vtxBase == 0) {
                            object->floorTriIndex = WORLD_TRI_INDEX(triAddr);
                        }
                    } else if (object != NULL) {
                        object->floorTriIndex = (triAddr - (s32)gModelTris[movement_obj[hit->owner].character_type]) / 12;
                    }
                    if (vtxBase == 0 && outTri != NULL) {
                        *outTri = hit->tri;
                    }
                    floorVtxBase = vtxBase;
                } else {
                    *outFloorY = noFloor;
                    floorVtxBase = 0;
                    if (outTri != NULL) {
                        *outTri = gCurMapTris;
                    }
                    floorTri = (s32)gCurMapTris;
                }
                triIndex = WORLD_TRI_INDEX(floorTri);
                if (type == PROBE_WORLD && outFlags != NULL) {
                    if (gMeshTriMaterials != NULL) {
                        *outFlags = gMeshTriMaterials[triIndex];
                    } else {
                        *outFlags = 0;
                    }
                } else if (type == PROBE_PROP && outFlags != NULL) {
                    gFloorQueryProp = gGroundCandidates[bestHit].owner + 1;
                    if (db_Object_array[gFloorQueryProp - 1].materials != NULL) {
                        *outFlags = db_Object_array[gFloorQueryProp - 1].materials[triIndex - db_Object_array[gFloorQueryProp - 1].firstTri];
                    } else {
                        *outFlags = db_Object_array[gFloorQueryProp - 1].floorHitAttrib;
                    }
                } else if (outFlags != NULL) {
                    *outFlags = 0;
                }
                if (type == PROBE_OBJECT) {
                    objectNum = gGroundCandidates[bestHit].owner + 1;
                }
            }
            if (count >= 2) {
                if (*outFloorY < y) {
                    ceilingFrom = y;
                } else {
                    ceilingFrom = *outFloorY;
                }
                for (i = 0; i < count; i++) {
                    height = gGroundCandidates[i].height / 256.0f;
                    if (ceilingFrom < height && height < ceilingY) {
                        if (!collision_isHitFacingUp(i)) {
                            s32 ceilingTri;

                            ceilingY = height;
                            ceilingTri = (s32)gGroundCandidates[i].tri;
                            triIndex = WORLD_TRI_INDEX(ceilingTri);
                            if (type == PROBE_WORLD) {
                                if (gMeshTriMaterials != NULL) {
                                    ceilingFlags = gMeshTriMaterials[triIndex];
                                } else {
                                    ceilingFlags = 0;
                                }
                            } else if (type == PROBE_PROP) {
                                s32 pad; // matching: IDO reserves a stack slot for this block-scoped unused local
                                propNum = gGroundCandidates[i].owner + 1;
                                if (db_Object_array[propNum - 1].materials != NULL) {
                                    ceilingFlags = db_Object_array[propNum - 1].materials[triIndex - db_Object_array[propNum - 1].firstTri];
                                } else {
                                    ceilingFlags = db_Object_array[propNum - 1].floorHitAttrib;
                                }
                            } else {
                                ceilingFlags = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    if (outPropFloorY != NULL) {
        if (ceilingY != 10000.0f && (ceilingFlags & 0x1F) == 8) {
            *outPropFloorY = 10000.0f;
        }
        if (*outPropFloorY == noFloor && (*outFlags & 0x1F) == 8) {
            *outPropFloorY = *outFloorY;
        }
    }
    if (floorTri != 0 && outVerts != NULL) {
        /* matching: a declaration here keeps IDO from using beqzl on this test */
        extern MeshTri *gCurMapTris;
        for (i = 0; i < 3; i++) {
            vtx = (s16 *)(((s32 *)floorTri)[i] + floorVtxBase);
            outVerts[i].x = vtx[0];
            outVerts[i].y = vtx[1];
            outVerts[i].z = vtx[2];
        }
        if (outTri != NULL) {
            *outTri = 0;
        }
    }
    if (object != NULL) {
        object->propNum = gFloorQueryProp;
        if (gFloorQueryProp != 0 && (y - *outFloorY) < 10.0f) {
            db_Object_array[gFloorQueryProp - 1].object_status |= 0x84;
            gWorldObjectContacts[gFloorQueryProp - 1] |= 1 << (object - movement_obj);
        }
        if (objectNum != 0 && (y - *outFloorY) < 10.0f) {
            object->stand_on_char = objectNum;
        } else {
            object->stand_on_char = 0;
        }
    }
    if (total == 0) {
        *outFloorY = noFloor;
        if (outFlags != NULL) {
            *outFlags = 0;
        }
    }
    if (object != NULL) {
        object->ceilingY = ceilingY;
    }
    gFloorQueryCeilingY = ceilingY;
}

/* Returns the height of the first prop surface found at (x, z) from height y, or -10000. */
f32 collision_getPropSurfaceHeight(f32 x, f32 y, f32 z) {
    f32 height;
    s32 groups;
    s32 iy;

    collision_selectMap(2);
    iy = y; // matching: converting y before the call keeps the original order
    collision_findPropsBelow(x, z, &groups, iy);
    collision_transformPropTris(groups, gSurfSetB, 0);
    if (collision_gatherPropGroundTris(x, z, groups, gSurfSetB)) {
        height = gGroundCandidates[0].height * 0.00390625f;
    } else {
        height = -10000.0f;
    }
    return height;
}

/*
 * Collects into outLeaves every leaf zone whose square, grown by `radius`, contains (x, z).
 * Returns the number of leaves written.
 */
s32 collision_findZoneLeavesNear(s32 x, s32 z, s32 radius, ZoneNode **outLeaves) {
    ZoneNode *node;
    s32 count;
    s32 offset;

    node = gCurMapQuadTree;
    count = 0;
    while (node != NULL) {
        if (ABS(node->x - x) < radius + node->halfSize && ABS(node->z - z) < radius + node->halfSize) {
            if (node->child != 0) {
                offset = node->child;
            } else {
                *outLeaves++ = node;
                count++;
                offset = node->next;
            }
        } else {
            offset = node->next;
        }
        if (offset != 0) {
            node = (ZoneNode *)((u8 *)node + offset);
        } else {
            node = NULL;
        }
    }
    return count;
}

/* Prepares the collision probe; callers pass the probe type, which is unused. */
void collision_selectMap() { // matching: a declared s32 parameter changes the code
    collision_setCurMap();
}

/*
 * Gets the slope of the given triangle: outSlopeX/Z receive it raw, outDirX/Z its direction
 * normalised to length 50 (or (1, 0) on flat ground). The fifth argument is unused.
 */
void collision_getTriSlope(void *tri, void *verts, f32 *outDirX, f32 *outDirZ, f32 *unused, f32 *outSlopeX,
                   f32 *outSlopeZ) {
    f32 slopeX;
    f32 slopeZ;
    f32 length;

    shadow_getTriSlopes(tri, verts, &slopeX, &slopeZ);
    length = sqrtf((slopeX * slopeX) + (slopeZ * slopeZ));
    *outSlopeX = slopeX;
    *outSlopeZ = slopeZ;
    length = length * 0.02f;
    if (length == 0.0f) {
        *outDirX = 1.0f;
        *outDirZ = 0.0f;
        return;
    }
    *outDirX = slopeX / length;
    *outDirZ = slopeZ / length;
}

/* Returns a surface's type: the low 5 bits of its flags. */
s32 collision_getSurfaceType(s32 flags) {
    return flags & 0x1F;
}

/*
 * Finds the world-mesh floor at (x, z) closest to height y, preferring hits at or below y.
 * Returns its height (or -10000 when there is none) and stores its triangle in *outTri.
 */
s32 collision_findMeshFloorNear(s32 x, s32 y, s32 z, s32 *outTri) {
    s32 count;
    s32 bestIndex;
    s32 i;
    GroundCandidate *hits;
    f32 fy;
    f32 bestDist;
    f32 dist;

    collision_selectMap(0);
    count = collision_gatherGroundTris(x, z);
    if (count == 0) {
        return -10000;
    }

    hits = gGroundCandidates;
    bestIndex = 0;
    fy = y;
    bestDist = fy - hits[0].height * 0.00390625f;

    for (i = 1; i < count; i++) {
        dist = fy - hits[i].height * 0.00390625f;
        if ((bestDist >= 0.0f && dist < bestDist && dist >= 0.0f) || (bestDist < 0.0f && bestDist < dist)) {
            bestDist = dist;
            bestIndex = i;
        }
    }

    if (outTri != NULL) {
        *outTri = (s32)hits[bestIndex].tri;
    }

    return hits[bestIndex].height * 0.00390625f;
}

/*
 * Updates the current zone for camera `index` from its position, calling func_150A6760 when it
 * entered a different zone.
 */
void collision_updateCameraZone(s32 index) {
    Camera *camera;
    s32 x;
    s32 z;
    ZoneNode *node;
    s32 zone;
    s16 *cameraZone;

    gZoneLastCamera = index;
    // matching: &camera_global_Cameras[index] computes the multiply after the base load
    camera = (Camera *)((u8 *)camera_global_Cameras + index * sizeof(Camera));
    x = camera->eye.x;
    z = camera->eye.z;
    gZoneLastCameraX = x;
    gZoneLastCameraZ = z;
    node = collision_findZoneLeaf(x, z);
    gZoneLastCameraLeaf = node;
    zone = collision_getZoneLeafIndex(node);
    gZoneLastCameraZone = zone;
    cameraZone = &gViewCameraZones[index];
    if (zone != *cameraZone) {
        *cameraZone = zone;
        func_150A6760(index);
    }
    if (level == 60) {
        func_150F33F8(index);
    }
}

/* Returns the leaf zone whose square contains (x, z), or NULL. */
ZoneNode *collision_findZoneLeaf(s32 x, s32 z) {
    ZoneNode *node;

    node = gCurMapQuadTree;
    while (node != NULL) {
        if (node->child != 0) {
            if (ABS(x - node->x) <= node->halfSize && ABS(z - node->z) <= node->halfSize) {
                node = (ZoneNode *)((u8 *)node + node->child);
            } else if (node->next != 0) {
                node = (ZoneNode *)((u8 *)node + node->next);
            } else {
                node = NULL;
            }
        } else {
            if (ABS(x - node->x) <= node->halfSize && ABS(z - node->z) <= node->halfSize) {
                return node;
            }
            if (node->next != 0) {
                node = (ZoneNode *)((u8 *)node + node->next);
            } else {
                node = NULL;
            }
        }
    }
    return NULL;
}

/* Returns the index of leaf zone `zone` (counted in tree order), or 0 if it is not in the tree. */
s32 collision_getZoneLeafIndex(ZoneNode *zone) {
    ZoneNode *node;
    s32 index;
    s16 offset;

    node = gCurMapQuadTree;
    index = 0;

    while (node != NULL) {
        if (node == zone) {
            return index;
        }
        offset = node->child;
        if (offset != 0) {
            node = (ZoneNode *)((u8 *)node + offset);
        } else {
            offset = node->next;
            if (offset != 0) {
                node = (ZoneNode *)((u8 *)node + offset);
                index++;
            } else {
                node = NULL;
            }
        }
    }

    return 0;
}
