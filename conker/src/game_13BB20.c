#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 D_800DBDC0;
extern s32 D_800DBDC4;
extern s32 D_800DBDC8;
extern f32 D_800DBDCC;
extern f32 D_800DBDD0;


#define ABS(x) ((x) < 0 ? -(x) : (x))

/* Returns whether ground hit `index`'s triangle faces up (its normal has y >= 0). */
s32 func_1510E670(s32 index) {
    s32 *tri;
    s32 vtxBase;
    s16 *v0;
    s16 *v1;
    s16 *v2;
    f32 edge1[3];
    f32 edge2[3];
    f32 normal[3];
    f32 normalY;

    tri = D_800D3300[index].tri;
    vtxBase = D_800D3300[index].vtxBase;
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
        func_150AD8B0(edge1, edge2, normal);
        normalY = normal[1];
        if (normalY < 0.0f) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
    return FALSE;
}


/* Floor query (see func_1510E950) with an explicit height window for the prop probe. */
void func_1510E7A4(s32 *outTri, s16 (*outVerts)[3], f32 *outFloorY, f32 *outPropFloorY, s32 *outFlags,
                   s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj, f32 minY, f32 maxY) {
    func_1510E950(outTri, outVerts, NULL, outFloorY, outPropFloorY, outFlags, outType, x, y, z, refY, group, obj,
                  minY, maxY, 0);
}

/* Floor query (see func_1510E950) with the default height window. */
void func_1510E82C(s32 *outTri, s16 (*outVerts)[3], f32 *outFloorY, f32 *outPropFloorY, s32 *outFlags,
                   s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj) {
    func_1510E950(outTri, outVerts, NULL, outFloorY, outPropFloorY, outFlags, outType, x, y, z, refY, group, obj,
                  -10000.0f, 20000.0f, 0);
}

/* Floor query (see func_1510E950) with the default height window and a noFallback choice. */
void func_1510E8BC(s32 *outTri, s16 (*outVerts)[3], f32 *outFloorY, f32 *outPropFloorY, s32 *outFlags,
                   s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *obj, f32 unused12,
                   f32 unused13, s32 noFallback) {
    func_1510E950(outTri, outVerts, NULL, outFloorY, outPropFloorY, outFlags, outType, x, y, z, refY, group, obj,
                  -10000.0f, 20000.0f, noFallback);
}

extern f32 D_800DBE54;
extern f32 D_800DBE58;
extern u16 D_800DBE60;

// D_800DBE5C points at the world mesh's per-triangle materials, or is NULL.
// World-mesh triangles are 12 bytes (three vertex offsets) from D_800DBE3C on.
#define WORLD_TRI_INDEX(tri) (((s32)(tri) - (s32)D_800DBE3C) / 12)

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
void func_1510E950(s32 *outTri, s16 (*outVerts)[3], s16 (*outPropVerts)[3], f32 *outFloorY, f32 *outPropFloorY,
                   s32 *outFlags, s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, Actor *object,
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
    s32 dummyFlags;
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
        reach = D_800DBE64;
    } else {
        reach = 50.0f;
    }
    D_800DBE68 = x;
    D_800DBE6C = y;
    haveFallback = FALSE;
    D_800DBE70 = z;
    *outFloorY = noFloor;
    if (outFlags == NULL) {
        dummyFlags = 0;
        outFlags = &dummyFlags;
    }
    ceilingY = 20000.0f;
    D_800DBE54 = noFloor;
    objectNum = 0;
    D_800DBE60 = 0;
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
        if (D_80089120[type] == 1) {
            if (type == PROBE_OBJECT && object != NULL && (object->noshove & 0x200)) {
                continue;
            }
            ix = (s32)x;
            iz = (s32)z;
            i = 0; // matching: dead store, but IDO schedules the loop differently without it
            func_1510F800(type);
            found = FALSE;
            if (type == PROBE_PROP) {
                s32 floorProps;
                s32 allProps;
                s32 bestPropHit;
                f32 bestPropY;
                s32 propVtxBase;
                s16 *propVtx;

                floorProps = func_150A6568((s16)ix, (s16)iz, &allProps, group, ix, iz, (s32)minY, (s32)maxY);
                func_150A44F0(floorProps, D_800D37E0, 0);
                if (outPropFloorY != NULL) {
                    func_150A44F0(allProps, D_800D3830, 0);
                    bestPropHit = 0;
                    count = func_150A43E0(ix, iz, allProps, D_800D3830);
                    if (count != 0) {
                        bestPropY = noFloor;
                        for (i = 0; i < count; i++) {
                            height = D_800D3300[i].height * 0.00390625f;
                            if (bestPropY < height && height < ceilingY) {
                                bestPropHit = i;
                                bestPropY = height;
                            }
                        }
                        *outPropFloorY = bestPropY;
                    }
                    if (*outPropFloorY != noFloor && outPropVerts != NULL) {
                        tri = D_800D3300[bestPropHit].tri;
                        propVtxBase = D_800D3300[bestPropHit].vtxBase;
                        for (i = 0; i < 3; i++) {
                            propVtx = (s16 *)(tri[i] + propVtxBase);
                            outPropVerts[i][0] = propVtx[0];
                            outPropVerts[i][1] = propVtx[1];
                            outPropVerts[i][2] = propVtx[2];
                        }
                    }
                }
                count = func_150A43E0(ix, iz, floorProps, D_800D37E0);
            } else if (type == PROBE_OBJECT) {
                count = func_150A4FA0(ix, iz);
            } else {
                count = func_150A3A70(ix, iz);
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

                height = D_800D3300[i].height / 256.0f;
                if (noFallback == 0 && allowFallback != 0 && *outFloorY == noFloor
                    && (noFloor == noFloor || height < noFloor)) { // matching: tautology IDO still compiles
                    s32 pad; // matching: IDO reserves a stack slot for this block-scoped unused local
                    if (func_1510E670(i)) {
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
                if (D_800DBE54 < height) {
                    D_800DBE54 = height;
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
                    if (func_1510E670(i)) {
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
                    hit = &D_800D3300[bestHit];
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
                        object->floorTriIndex = (triAddr - (s32)D_800C6070[movement_obj[hit->unkC].character_type]) / 12;
                    }
                    if (vtxBase == 0 && outTri != NULL) {
                        *outTri = (s32)hit->tri;
                    }
                    floorVtxBase = vtxBase;
                } else {
                    *outFloorY = noFloor;
                    floorVtxBase = 0;
                    if (outTri != NULL) {
                        *outTri = (s32)D_800DBE3C;
                    }
                    floorTri = (s32)D_800DBE3C;
                }
                triIndex = WORLD_TRI_INDEX(floorTri);
                if (type == PROBE_WORLD && outFlags != NULL) {
                    if (D_800DBE5C != NULL) {
                        *outFlags = D_800DBE5C[triIndex];
                    } else {
                        *outFlags = 0;
                    }
                } else if (type == PROBE_PROP && outFlags != NULL) {
                    D_800DBE60 = D_800D3300[bestHit].unkC + 1;
                    if (db_Object_array[D_800DBE60 - 1].materials != NULL) {
                        *outFlags = db_Object_array[D_800DBE60 - 1].materials[triIndex - db_Object_array[D_800DBE60 - 1].firstTri];
                    } else {
                        *outFlags = db_Object_array[D_800DBE60 - 1].floorHitAttrib;
                    }
                } else if (outFlags != NULL) {
                    *outFlags = 0;
                }
                if (type == PROBE_OBJECT) {
                    objectNum = D_800D3300[bestHit].unkC + 1;
                }
            }
            if (count >= 2) {
                if (*outFloorY < y) {
                    ceilingFrom = y;
                } else {
                    ceilingFrom = *outFloorY;
                }
                for (i = 0; i < count; i++) {
                    height = D_800D3300[i].height / 256.0f;
                    if (ceilingFrom < height && height < ceilingY) {
                        if (!func_1510E670(i)) {
                            s32 ceilingTri;

                            ceilingY = height;
                            ceilingTri = (s32)D_800D3300[i].tri;
                            triIndex = WORLD_TRI_INDEX(ceilingTri);
                            if (type == PROBE_WORLD) {
                                if (D_800DBE5C != NULL) {
                                    ceilingFlags = D_800DBE5C[triIndex];
                                } else {
                                    ceilingFlags = 0;
                                }
                            } else if (type == PROBE_PROP) {
                                s32 pad; // matching: IDO reserves a stack slot for this block-scoped unused local
                                propNum = D_800D3300[i].unkC + 1;
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
        extern MeshTri *D_800DBE3C; // matching: a declaration here keeps IDO from using beqzl on this test
        for (i = 0; i < 3; i++) {
            vtx = (s16 *)(((s32 *)floorTri)[i] + floorVtxBase);
            outVerts[i][0] = vtx[0];
            outVerts[i][1] = vtx[1];
            outVerts[i][2] = vtx[2];
        }
        if (outTri != NULL) {
            *outTri = 0;
        }
    }
    if (object != NULL) {
        object->propNum = D_800DBE60;
        if (D_800DBE60 != 0 && (y - *outFloorY) < 10.0f) {
            db_Object_array[D_800DBE60 - 1].object_status |= 0x84;
            gWorldObjectContacts[D_800DBE60 - 1] |= 1 << (object - movement_obj);
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
    D_800DBE58 = ceilingY;
}

/* Returns the height of the first prop surface found at (x, z) from height y, or -10000. */
f32 func_1510F648(f32 x, f32 y, f32 z) {
    f32 height;
    s32 groups;
    s32 iy;

    func_1510F800(2);
    iy = (s32)y; // matching: converting y before the call keeps the original order
    func_150A64C8((s32)x, (s32)z, &groups, iy);
    func_150A44F0(groups, D_800D3830, 0);
    if (func_150A43E0((s32)x, (s32)z, groups, D_800D3830)) {
        height = D_800D3300[0].height * 0.00390625f;
    } else {
        height = -10000.0f;
    }
    return height;
}

/*
 * Collects into outLeaves every leaf zone whose square, grown by `radius`, contains (x, z).
 * Returns the number of leaves written.
 */
s32 func_1510F720(s32 x, s32 z, s32 radius, ZoneNode **outLeaves) {
    ZoneNode *node;
    s32 count;
    s32 offset;

    node = D_800DBE48;
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
void func_1510F800() { // matching: a declared s32 parameter changes the code
    func_150A49F4();
}

extern f32 D_800A2D9C; // matching: 0.02f as a literal grows the frame by 8 bytes

/*
 * Gets the slope of the given triangle: outSlopeX/Z receive it raw, outDirX/Z its direction
 * normalised to length 50 (or (1, 0) on flat ground). The fifth argument is unused.
 */
void func_1510F820(void *tri, void *verts, f32 *outDirX, f32 *outDirZ, f32 *unused, f32 *outSlopeX,
                   f32 *outSlopeZ) {
    f32 slopeX;
    f32 slopeZ;
    f32 length;
    f32 zero;

    zero = 0.0f; // matching: IDO keeps 0.0f in $f12 for both the compare and the store
    func_1510E388(tri, verts, &slopeX, &slopeZ);
    length = sqrtf((slopeX * slopeX) + (slopeZ * slopeZ));
    *outSlopeX = slopeX;
    *outSlopeZ = slopeZ;
    length = length * D_800A2D9C;
    if (zero == length) {
        *outDirX = 1.0f;
        *outDirZ = zero;
        return;
    }
    *outDirX = slopeX / length;
    *outDirZ = slopeZ / length;
}

/* Returns a surface's type: the low 5 bits of its flags. */
s32 func_1510F8CC(s32 flags) {
    return flags & 0x1F;
}

/*
 * Finds the world-mesh floor at (x, z) closest to height y, preferring hits at or below y.
 * Returns its height (or -10000 when there is none) and stores its triangle in *outTri.
 */
s32 func_1510F8D8(s32 x, s32 y, s32 z, s32 *outTri) {
    s32 count;
    s32 bestIndex;
    s32 i;
    GroundCandidate *hits;
    f32 fy;
    f32 bestDist;
    f32 dist;

    func_1510F800(0);
    count = func_150A3A70(x, z);
    if (count == 0) {
        return -10000;
    }

    hits = D_800D3300;
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
void func_1510FC34(s32 index) {
    Camera *camera;
    s32 x;
    s32 z;
    s32 zone;
    s16 *cameraZone;

    D_800DBDC0 = index;
    // matching: &camera_global_Cameras[index] computes the multiply after the base load
    camera = (Camera *)((u8 *)camera_global_Cameras + index * sizeof(Camera));
    x = camera->eye.x;
    z = camera->eye.z;
    D_800DBDCC = x;
    D_800DBDD0 = z;
    zone = func_1510FD20(x, z);
    D_800DBDC8 = zone;
    zone = func_1510FE30(zone);
    D_800DBDC4 = zone;
    cameraZone = &D_800DBE30[index];
    if (zone != *cameraZone) {
        *cameraZone = zone;
        func_150A6760(index);
    }
    if (level == 60) {
        func_150F33F8(index);
    }
}

/* Returns the leaf zone (as an address) whose square contains (x, z), or 0. */
s32 func_1510FD20(s32 x, s32 z) {
    ZoneNode *node;

    node = D_800DBE48;
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
                return (s32)node;
            }
            if (node->next != 0) {
                node = (ZoneNode *)((u8 *)node + node->next);
            } else {
                node = NULL;
            }
        }
    }
    return 0;
}

/* Returns the index of leaf zone `zone` (counted in tree order), or 0 if it is not in the tree. */
s32 func_1510FE30(s32 zone) {
    ZoneNode *node;
    s32 index;
    s16 offset;

    node = D_800DBE48;
    index = 0;

    while (node != NULL) {
        if ((s32)node == zone) {
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
