#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 D_800DBDC0;
extern s32 D_800DBDC4;
extern s32 D_800DBDC8;
extern f32 D_800DBDCC;
extern f32 D_800DBDD0;

/* Node of the zone tree at D_800DBE48: square regions linked by byte offsets. */
typedef struct ZoneNode {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 next;     // byte offset to the next sibling, 0 = last
    /* 0x6 */ u16 halfSize; // half the region's side length
    /* 0x8 */ s16 x;        // region centre
    /* 0xA */ s16 z;
    /* 0xC */ s16 child;    // byte offset to the first child, 0 = leaf
} ZoneNode;

/* Ground-probe hit records written to D_800D3300 by func_150A3A70 and friends. */
typedef struct GroundHit {
    /* 0x0 */ s32 height;  // 24.8 fixed point
    /* 0x4 */ s32 *tri;    // collision triangle: three vertex offsets from vtxBase
    /* 0x8 */ s32 vtxBase; // 0 for the static world mesh
    /* 0xC */ s32 unkC;    // group or object index, depending on the probe type
} GroundHit;

// D_800D3300 is declared as a pointer in variables.h, but it is the hit array itself.
#define sGroundHits ((GroundHit *)&D_800D3300)

extern ZoneNode *D_800DBE48;

#define ABS(x) ((x) < 0 ? -(x) : (x))

extern s32 func_150A3A70(s32, s32);
extern void func_150A6760(s32);
extern void func_150F33F8(s32);
extern s32 func_1510FE30(s32);

extern void func_150AD8B0(f32 *, f32 *, f32 *);

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

    tri = sGroundHits[index].tri;
    vtxBase = sGroundHits[index].vtxBase;
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

void func_1510E950(s32 *arg0, s16 (*arg1)[3], s16 (*arg2)[3], f32 *arg3, f32 *arg4, s32 *arg5, s16 *arg6, f32 x, f32 y, f32 z, f32 argA, u16 argB, struct127 *argC, f32 argD, f32 argE, s32 argF);

/* Floor query with an explicit height window for the type-2 collision groups. */
void func_1510E7A4(s32 *outTri, s16 (*outVerts)[3], f32 *outFloorY, f32 *outGroupFloorY, s32 *outFlags,
                   s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, struct127 *obj, f32 minY, f32 maxY) {
    func_1510E950(outTri, outVerts, NULL, outFloorY, outGroupFloorY, outFlags, outType, x, y, z, refY, group, obj,
                  minY, maxY, 0);
}

/* Floor query with the default height window. */
void func_1510E82C(s32 *outTri, s16 (*outVerts)[3], f32 *outFloorY, f32 *outGroupFloorY, s32 *outFlags,
                   s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, struct127 *obj) {
    func_1510E950(outTri, outVerts, NULL, outFloorY, outGroupFloorY, outFlags, outType, x, y, z, refY, group, obj,
                  -10000.0f, 20000.0f, 0);
}

/* Floor query with the default height window; noFallback disables the upward-facing fallback triangle. */
void func_1510E8BC(s32 *outTri, s16 (*outVerts)[3], f32 *outFloorY, f32 *outGroupFloorY, s32 *outFlags,
                   s16 *outType, f32 x, f32 y, f32 z, f32 refY, u16 group, struct127 *obj, f32 unused12,
                   f32 unused13, s32 noFallback) {
    func_1510E950(outTri, outVerts, NULL, outFloorY, outGroupFloorY, outFlags, outType, x, y, z, refY, group, obj,
                  -10000.0f, 20000.0f, noFallback);
}

typedef struct Obj1510E950 {
    u8 pad0[0xF8];
    s32 unkF8;
    u8 padFC[0x17C - 0xFC];
    f32 unk17C;
    u8 pad180[0x19E - 0x180];
    s16 unk19E;
    u16 unk1A0;
    u8 pad1A2[0x274 - 0x1A2];
    u8 unk274;
} Obj1510E950;

typedef struct Ent1510E950 {
    u8 pad0[0x40];
    s32 unk40;
    s32 *unk44;
    u8 pad48[0x4F - 0x48];
    u8 unk4F;
    u8 pad50[0x58 - 0x50];
    u16 unk58;
    u8 pad5A[0xA0 - 0x5A];
} Ent1510E950;
extern void func_1510F800();
extern void func_150A64C8(s16, s16, s32 *, s32);
extern void func_150A44F0(s32, s32 *, s32);
extern s32 func_150A43E0(s32, s32, s32, s32 *);
extern s32 D_800D3830;
extern s32 func_150A4FA0(s32, s32);
extern s32 func_150A6568(s16, s16, s32 *, u16, s32, s32, s32, s32);
extern s32 D_800D37E0;
extern u8 D_80089120[];
extern f32 D_800DBE54;
extern f32 D_800DBE58;
extern u16 D_800DBE60;
extern f32 D_800DBE68;
extern f32 D_800DBE6C;
extern f32 D_800DBE70;
extern s32 *D_800DBF94;

#define ENTS sGroundHits
#define obj ((Obj1510E950 *)argC)
#define OBJS2 ((Ent1510E950 *)D_800DBEF4)

void func_1510E950(s32 *arg0, s16 (*arg1)[3], s16 (*arg2)[3], f32 *arg3, f32 *arg4, s32 *arg5, s16 *arg6, f32 x, f32 y, f32 z, f32 argA, u16 argB, struct127 *argC, f32 argD, f32 argE, s32 argF) {
    s32 i;
    s32 chosen;
    s32 hit;
    s32 first;
    s32 type;
    s32 flags;
    s32 chosenType;
    s32 count;
    s32 total;
    s32 ix;
    f32 lastY;
    s32 iz;
    s32 tri;
    s32 found;
    s32 base;
    s32 numTypes;
    f32 h;
    f32 lo;
    f32 maxY;
    f32 none;
    s32 typeFlag;
    s32 dummy;
    s32 idx;
    s32 id;
    GroundHit *e;
    s32 *t;
    s16 *v;
    s32 b2;
    s32 altIdx;
    s32 altType;
    s32 alt;
    s32 canAlt;
    s32 vi;
    volatile f32 range;

    none = -10000.0f;
    total = 0;
    tri = 0;
    flags = 0;
    hit = 0;
    if (arg0 != 0) {
        *arg0 = 0;
    }
    if (obj != NULL) {
        range = D_800DBE64;
    } else {
        range = 50.0f;
    }
    D_800DBE68 = x;
    D_800DBE6C = y;
    alt = 0;
    D_800DBE70 = z;
    *arg3 = none;
    if (arg5 == 0) {
        dummy = 0;
        arg5 = &dummy;
    }
    maxY = 20000.0f;
    D_800DBE54 = none;
    typeFlag = 0;
    D_800DBE60 = 0;
    if (obj != NULL) {
        obj->unk19E = 0;
    }
    numTypes = 4;
    first = 1;
    canAlt = 1;
    if (arg4 != 0) {
        *arg4 = none;
    }
    for (type = 0; type < numTypes; type++) {
        if (D_80089120[type] == 1) {
        if (type == 3 && obj != NULL && (obj->unkF8 & 0x200)) continue;
                ix = (s32)x;
                iz = (s32)z;
                i = 0;
                func_1510F800(type);
                found = 0;
                if (type == 2) {
                    s32 spBC;
                    s32 spB8;
                    s32 bestIdx;
                    f32 best;
                    s32 off;
                    s16 *vp;

                    spBC = func_150A6568((s16)ix, (s16)iz, &spB8, argB, ix, iz, (s32)argD, (s32)argE);
                    func_150A44F0(spBC, &D_800D37E0, 0);
                    if (arg4 != 0) {
                        func_150A44F0(spB8, &D_800D3830, 0);
                        bestIdx = 0;
                        count = func_150A43E0(ix, iz, spB8, &D_800D3830);
                        if (count != 0) {
                            best = none;
                            for (i = 0; i < count; i++) {
                                h = ENTS[i].height * 0.00390625f;
                                if (best < h && h < maxY) {
                                    bestIdx = i;
                                    best = h;
                                }
                            }
                            *arg4 = best;
                        }
                        if (*arg4 != none && arg2 != 0) {
                            t = ENTS[bestIdx].tri;
                            off = ENTS[bestIdx].vtxBase;
                            for (i = 0; i < 3; i++) {
                                vp = (s16 *)(t[i] + off);
                                arg2[i][0] = vp[0];
                                arg2[i][1] = vp[1];
                                arg2[i][2] = vp[2];
                            }
                        }
                    }
                    count = func_150A43E0(ix, iz, spBC, &D_800D37E0);
                } else if (type == 3) {
                    count = func_150A4FA0(ix, iz);
                } else {
                    count = func_150A3A70(ix, iz);
                }
                total += count;
                if (first) {
                    lastY = none;
                    chosen = -1;
                    chosenType = type;
                    first = 0;
                }
                for (i = 0; i < count; i++) {
                    f32 rg;
                    h = ENTS[i].height / 256.0f;
                    if (argF == 0 && canAlt != 0 && *arg3 == none && (none == none || h < none)) {
                        s32 padM;
                        if (func_1510E670(i)) {
                            alt = 1;
                            altIdx = i;
                            altType = type;
                        }
                    }
                    rg = range;
                    if ((u32)i < 0U) {
                        if (h < argA) {
                            continue;
                        }
                    }
                    if (D_800DBE54 < h) {
                        D_800DBE54 = h;
                    }
                    if (y < argA) {
                        if (h < argA + rg && lastY < h) {
                            hit = 1;
                        }
                    } else if (h < y + rg) {
                        if (h < argA) {
                            if (lastY < h) {
                                hit = 1;
                            }
                        } else if (lastY < argA) {
                            hit = 1;
                        } else if (h < lastY) {
                            hit = 1;
                        }
                    }
                    if (hit) {
                        hit = 0;
                        if (func_1510E670(i)) {
                            chosenType = type;
                            found = 1;
                            lastY = h;
                            chosen = i;
                        }
                    }
                }
                if (found || alt) {
                    if (!found && alt) {
                        chosen = altIdx;
                        chosenType = altType;
                    }
                    alt = 0;
                    if (arg6 != 0) {
                        *arg6 = chosenType;
                    }
                    if (chosen != -1) {
                        e = &ENTS[chosen];
                        canAlt = 0;
                        *arg3 = e->height / 256.0f;
                        if (*arg3 < none) {
                            *arg3 = none;
                        }
                        vi = (s32)e->tri;
                        tri = vi;
                        b2 = e->vtxBase;
                        if (type != 3) {
                            if (obj != NULL && b2 == 0) {
                                obj->unk19E = (vi - D_800DBE3C) / 12;
                            }
                        } else if (obj != NULL) {
                            obj->unk19E = (vi - D_800C6070[D_800CC2D4[e->unkC * 0x32C]]) / 12;
                        }
                        if (b2 == 0 && arg0 != 0) {
                            *arg0 = (s32)e->tri;
                        }
                        base = b2;
                    } else {
                        *arg3 = none;
                        base = 0;
                        if (arg0 != 0) {
                            *arg0 = D_800DBE3C;
                        }
                        tri = D_800DBE3C;
                    }
                    idx = (tri - D_800DBE3C) / 12;
                    if (type == 0 && arg5 != 0) {
                        if (*(s32 **)&D_800DBE5C != NULL) {
                            *arg5 = (*(s32 **)&D_800DBE5C)[idx];
                        } else {
                            *arg5 = 0;
                        }
                    } else if (type == 2 && arg5 != 0) {
                        D_800DBE60 = ENTS[chosen].unkC + 1;
                        if (OBJS2[D_800DBE60 - 1].unk44 != NULL) {
                            *arg5 = OBJS2[D_800DBE60 - 1].unk44[idx - OBJS2[D_800DBE60 - 1].unk58];
                        } else {
                            *arg5 = OBJS2[D_800DBE60 - 1].unk40;
                        }
                    } else if (arg5 != 0) {
                        *arg5 = 0;
                    }
                    if (type == 3) {
                        typeFlag = ENTS[chosen].unkC + 1;
                    }
                }
                if (count >= 2) {
                    i = 0;
                    if (*arg3 < y) {
                        lo = y;
                    } else {
                        lo = *arg3;
                    }
                    e = ENTS;
                    if (count > 0) {
                    do {
                        h = e->height / 256.0f;
                        if (lo < h && h < maxY) {
                            if (!func_1510E670(i)) {
                                s32 vt;
                                maxY = h;
                                vt = (s32)e->tri;
                                idx = (vt - D_800DBE3C) / 12;
                                if (type == 0) {
                                    if (*(s32 **)&D_800DBE5C != NULL) {
                                        flags = (*(s32 **)&D_800DBE5C)[idx];
                                    } else {
                                        flags = 0;
                                    }
                                } else if (type == 2) {
                                    s32 padQ;
                                    id = e->unkC + 1;
                                    if (OBJS2[id - 1].unk44 != NULL) {
                                        flags = OBJS2[id - 1].unk44[idx - OBJS2[id - 1].unk58];
                                    } else {
                                        flags = OBJS2[id - 1].unk40;
                                    }
                                } else {
                                    flags = 0;
                                }
                            }
                        }
                        i++;
                        e++;
                    } while (i != count);
                    }
                }
        }
    }
    if (arg4 != 0) {
        if (maxY != 10000.0f && (flags & 0x1F) == 8) {
            *arg4 = 10000.0f;
        }
        if (*arg4 == none && (*arg5 & 0x1F) == 8) {
            *arg4 = *arg3;
        }
    }
    if (tri != 0 && arg1 != 0) {
        extern s32 D_800DBE3C;
        for (i = 0; i < 3; i++) {
            v = (s16 *)(((s32 *)tri)[i] + base);
            arg1[i][0] = v[0];
            arg1[i][1] = v[1];
            arg1[i][2] = v[2];
        }
        if (arg0 != 0) {
            *arg0 = 0;
        }
    }
    if (obj != NULL) {
        obj->unk1A0 = D_800DBE60;
        if (D_800DBE60 != 0 && (y - *arg3) < 10.0f) {
            OBJS2[D_800DBE60 - 1].unk4F |= 0x84;
            D_800DBF94[D_800DBE60 - 1] |= 1 << (argC - D_800CC2D0);
        }
        if (typeFlag != 0 && (y - *arg3) < 10.0f) {
            obj->unk274 = typeFlag;
        } else {
            obj->unk274 = 0;
        }
    }
    if (total == 0) {
        *arg3 = none;
        if (arg5 != 0) {
            *arg5 = 0;
        }
    }
    if (obj != NULL) {
        obj->unk17C = maxY;
    }
    D_800DBE58 = maxY;
}


/* Returns the height of the highest type-2 group surface at (x, z) found from height y, or -10000. */
f32 func_1510F648(f32 x, f32 y, f32 z) {
    f32 height;
    s32 groups;
    s32 iy;

    func_1510F800(2);
    iy = (s32)y;
    func_150A64C8((s32)x, (s32)z, &groups, iy);
    func_150A44F0(groups, &D_800D3830, 0);
    if (func_150A43E0((s32)x, (s32)z, groups, &D_800D3830)) {
        height = sGroundHits[0].height * 0.00390625f;
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

extern void func_150A49F4(void);

/* Prepares the collision probe; callers pass the probe type, which is unused. */
void func_1510F800() { // matching: a declared s32 parameter changes the code
    func_150A49F4();
}

void func_1510E388(void *, void *, f32 *, f32 *);
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
    GroundHit *hits;
    f32 fy;
    f32 bestDist;
    f32 dist;

    func_1510F800(0);
    count = func_150A3A70(x, z);
    if (count == 0) {
        return -10000;
    }

    hits = sGroundHits;
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
    struct108 *camera;
    s32 x;
    s32 z;
    s32 zone;
    s16 *cameraZone;

    D_800DBDC0 = index;
    // matching: &D_800DBFF0[index] computes the multiply after the base load
    camera = (struct108 *)((u8 *)D_800DBFF0 + index * sizeof(struct108));
    x = camera->unk2F8;
    z = camera->unk300;
    D_800DBDCC = x;
    D_800DBDD0 = z;
    zone = func_1510FD20(x, z);
    D_800DBDC8 = zone;
    zone = func_1510FE30(zone);
    D_800DBDC4 = zone;
    cameraZone = &((s16 *)&D_800DBE30)[index];
    if (zone != *cameraZone) {
        *cameraZone = zone;
        func_150A6760(index);
    }
    if (D_800BE9F0 == 60) {
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
