#include <ultra64.h>

/*
 * Collision state, linked at 0x800B0DC0..0x800B0DF0 in this order. It is defined before functions.h
 * and variables.h: an earlier declaration changes IDO's .bss order.
 */
u8 *D_800B0DC0;         /* triangle-data arena: cursor */
static u8 *D_800B0DC4;  /* triangle-data arena: start */
static u8 *D_800B0DC8;  /* triangle-data arena: end */
static s32 D_800B0DCC;  /* quadtree node count */
static s32 D_800B0DD0;  /* quadtree leaf count */
static s32 D_800B0DD4;  /* quadtree: smallest node size that is split */
static u16 D_800B0DD8;  /* quadtree: fewest triangles in a node that is split */
static u8 *D_800B0DDC;  /* quadtree buffer */
static u8 *D_800B0DE0;  /* quadtree buffer cursor */
static s32 D_800B0DE4;  /* triangles added for all maps */

#include "functions.h"
#include "variables.h"

#include "macros.h"

/* Size of the quadtree buffer of each level (func_15001A08). */
static const s32 D_80091AF0[69] = {
    15000, 10000, 20000, 30000, 30000, 30000, 30000, 10000, 30000, 30000,
    15000, 10000, 20000, 30000, 20000, 30000, 20000, 30000, 10000, 30000,
    40000, 30000, 30000, 30000, 30000, 30000, 20000, 10000, 20000, 10000,
    10000, 30000, 30000, 10000, 30000, 20000, 30000, 30000, 30000, 30000,
    30000, 30000, 30000, 30000, 10000, 20000, 10000, 10000, 30000, 10000,
    50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
};

/* Size of the triangle-data arena of each level (func_15001970). */
static const s32 D_80091C04[69] = {
    45000, 60000, 100000, 100000, 70000, 100000, 125000, 50000, 100000, 100000,
    61000, 60000, 100000, 100000, 70000, 100000, 60000, 100000, 24000, 110000,
    50000, 100000, 100000, 110000, 30000, 100000, 70000, 70000, 110000, 40000,
    40000, 100000, 100000, 20000, 100000, 83000, 91000, 100000, 100000, 100000,
    100000, 100000, 100000, 105000, 100000, 105000, 50000, 50000, 100000, 50000,
    100000, 100000, 120000, 100000, 135000, 100000, 36000, 100000, 100000, 120000,
    150000, 130000, 130000, 130000, 100000, 100000, 8000, 54000, 18000,
};

// Quadtree node, built into the D_800B0DDC buffer. Links are byte offsets from the node itself.
struct QuadNode {
    /* 0x0 */ s16 minY;
    /* 0x2 */ s16 maxY;
    /* 0x4 */ s16 next;     // next sibling (0: none)
    /* 0x6 */ u16 halfSize;
    /* 0x8 */ s16 x;        // centre
    /* 0xA */ s16 z;
    /* 0xC */ s16 child;    // first of four children (0: leaf; its triangle stream follows the node)
};

typedef struct YRange {
    s16 min;
    s16 max;
} YRange;

typedef struct XZRange {
    s16 minx;
    s16 minz;
    s16 maxx;
    s16 maxz;
} XZRange;

/**
 * Appends every triangle drawn by display list `dl` to the triangle table D_800DBE3C
 * and records its bounding box: x/z in D_800DBE40, y in D_800DBE44.
 */
void func_15001460(Gfx *dl) {
    s32 i;
    Gfx *gfx;
    s32 opcode;
    s32 count;
    s32 t;
    s32 numTris;
    s32 j;
    s32 k;
    u32 addr;
    s32 m;
    Vtx *verts[31];
    s16 numVerts;
    s16 first;
    s16 tris[4][3];
    s16 min[3];
    s16 max[3];

    if (dl == NULL) {
        return;
    }
    count = D_800DBE38;
    for (i = 0; *(s8 *)&dl[i] != (s8)G_ENDDL; i++) {
        gfx = &dl[i];
        numTris = 0;
        opcode = *(s8 *)gfx;
        if ((opcode >> 4) == 1) {
            // Conker's G_TRI4 (opcodes 0x10-0x1F): four triangles, 5-bit vertex indices
            tris[0][0] = (gfx->words.w1 >> 25) & 0x1F;
            tris[0][1] = (gfx->words.w1 >> 20) & 0x1F;
            tris[0][2] = (gfx->words.w1 >> 15) & 0x1F;
            tris[1][0] = (gfx->words.w1 >> 10) & 0x1F;
            tris[1][1] = (gfx->words.w1 >> 5) & 0x1F;
            tris[1][2] = gfx->words.w1 & 0x1F;
            tris[2][0] = (gfx->words.w0 >> 10) & 0x1F;
            tris[2][1] = (gfx->words.w0 >> 5) & 0x1F;
            tris[2][2] = gfx->words.w0 & 0x1F;
            tris[3][0] = (gfx->words.w0 >> 23) & 0x1F;
            tris[3][1] = (gfx->words.w0 >> 18) & 0x1F;
            tris[3][2] = ((gfx->words.w1 >> 30) & 3) | ((gfx->words.w0 >> 13) & 0x1C);
            numTris = 4;
        } else if (opcode == G_TRI2) {
            tris[0][0] = (gfx->words.w0 >> 17) & 0x1F;
            tris[0][1] = (gfx->words.w0 >> 9) & 0x1F;
            tris[0][2] = (gfx->words.w0 >> 1) & 0x1F;
            tris[1][0] = (gfx->words.w1 >> 17) & 0x1F;
            tris[1][1] = (gfx->words.w1 >> 9) & 0x1F;
            tris[1][2] = (gfx->words.w1 >> 1) & 0x1F;
            numTris = 2;
        } else if (opcode == G_VTX) {
            addr = gfx->words.w1;
            if ((addr >> 24) & 0xF) {
                // segmented address: rebase through the segment table
                addr = D_800D2C28[(addr >> 24) & 0xF] + addr - (D_800D2C68 << 24);
            }
            numVerts = ((s32)gfx->words.w0 >> 12) & 0xFF;
            first = ((s32)gfx->words.w0 >> 1) & 0x7F;
            first -= numVerts;
            // (sic) the bound is the vertex count, not first + count
            for (m = first; m < numVerts; m++) {
                verts[m] = (Vtx *)addr;
                addr += sizeof(Vtx);
            }
        } else if (opcode == G_TRI1) {
            for (k = 0; k < 3; k++) {
                tris[0][k] = ((u8 *)gfx)[k + 1] >> 1;
            }
            numTris = 1;
        }
        for (t = 0; t < numTris; t++) {
            if ((u32)verts[0] > 0x100000) {
                // matching: `opcode` doubles as the axis counter; a separate local reallocates registers
                for (opcode = 0; opcode < 3; opcode++) {
                    if (D_800DBE63 == 0) {
                        min[opcode] = max[opcode] = verts[tris[t][0]]->v.ob[opcode];
                        for (k = 1; k < 3; k++) {
                            if (verts[tris[t][k]]->v.ob[opcode] < min[opcode]) {
                                min[opcode] = verts[tris[t][k]]->v.ob[opcode];
                            } else if (max[opcode] < verts[tris[t][k]]->v.ob[opcode]) {
                                max[opcode] = verts[tris[t][k]]->v.ob[opcode];
                            }
                            if (verts[tris[t][k]]->v.ob[opcode] < min[opcode]) {
                                min[opcode] = verts[tris[t][k]]->v.ob[opcode];
                            } else if (max[opcode] < verts[tris[t][k]]->v.ob[opcode]) {
                                max[opcode] = verts[tris[t][k]]->v.ob[opcode];
                            }
                        }
                    } else {
                        min[opcode] = -0x8000;
                        max[opcode] = 0x7FFF;
                    }
                }
            }
            for (j = 0; j < 3; j++) {
                D_800DBE3C[count].verts[j] = verts[tris[t][j]];
            }
            D_800DBE40[count].minx = min[0];
            D_800DBE40[count].maxx = max[0];
            D_800DBE40[count].minz = min[2];
            D_800DBE40[count].maxz = max[2];
            D_800DBE44[count].min = min[1];
            D_800DBE44[count].max = max[1];
            count++;
        }
    }
    D_800DBE38 = count;
}

/** Resets the triangle counters and allocates this map's triangle-data arena (D_800B0DC0..D_800B0DC8). */
void func_15001970(void) {
    s32 i;
    s32 size;

    D_800B0DE4 = 0;
    D_800DBE63 = 0;
    for (i = 0; i < 4; i++) {
        D_800DBE18[i] = 0;
    }
    size = D_80091C04[level];
    D_800B0DC0 = allocate_memory(size, 1, 0, 0);
    D_800B0DC8 = size + D_800B0DC0;
    D_800B0DC4 = D_800B0DC0;
}

void __osPiGetAccess(void);
void __osPiRelAccess(void);

/**
 * Allocates the quadtree buffer for the current map (D_800B0DDC) and resets the node counters.
 * The buffer is doubled when the cartridge word at ROM offset 0xD24 is not the expected one.
 */
void func_15001A08(void) {
    s32 size;
    u32 stat;
    s32 romWord;
    s32 i;

    size = D_80091AF0[level];

    __osPiGetAccess();
    stat = IO_READ(PI_STATUS_REG);
    while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY)) {
        stat = IO_READ(PI_STATUS_REG);
    }
    romWord = IO_READ(D_80000308 | 0xB0000D24);
    __osPiRelAccess();

    if (romWord != 0x98CCE31A) {
        size <<= 1;
    }

    D_800B0DDC = allocate_memory(size, 1, 0, 0);
    D_800B0DE0 = D_800B0DDC;
    D_800B0DCC = 0;
    D_800B0DD0 = 0;
    for (i = 0; i < 4; i++) {
        D_800DBE30[i] = 0;
    }
}

void func_15001B08(void) {
}

/** Appends a new quadtree node (with no child or sibling) to the 4-byte-aligned end of the buffer. */
QuadNode *func_15001B10(void) {
    QuadNode *node;

    D_800B0DE0 = (u8 *)ALIGN4(D_800B0DE0);
    D_800B0DCC++;

    node = (QuadNode *)D_800B0DE0;
    node->child = 0;
    node->next = 0;

    D_800B0DE0 += sizeof(QuadNode);
    return node;
}

/** Appends one byte to the quadtree buffer. */
void func_15001B5C(u8 value) {
    *D_800B0DE0++ = value;
}

/** Appends a big-endian halfword to the quadtree buffer. */
void func_15001B8C(u16 value) {
    D_800B0DE0[0] = value >> 8;
    D_800B0DE0[1] = value & 0xFF;
    D_800B0DE0 += 2;
}

/**
 * Builds the quadtree of map `map` over its D_800DBE4C triangles and stores the root in
 * D_800DBE08[map]. Optionally returns the total node count and the leaf count.
 */
void func_15001BC8(s32 map, s16 *nodeCount, s16 *leafCount) {
    QuadNode *root;
    u16 *triList;
    s32 i;

    func_1510F800(map);
    D_800B0DD8 = 10;
    D_800B0DD4 = 512;
    triList = allocate_memory((D_800DBE4C + 1) << 1, 1, 0, 0);

    triList[0] = D_800DBE4C;
    for (i = 0; i < D_800DBE4C; i++) {
        triList[i + 1] = i;
    }

    root = func_15001DE0(0, 0, 0x8000, 0x8000, 0, triList);
    D_800DBE08[map] = root;
    func_15002560(root, NULL);
    func_10004074(triList);

    if (1) { } // matching: IDO's register allocation for the out-params needs this empty block

    if (nodeCount != NULL) {
        *nodeCount = D_800B0DCC;
    }
    if (leafCount != NULL) {
        *leafCount = D_800B0DD0;
    }
}

/** Gives map `map` a single-leaf quadtree that covers the whole map and holds all its triangles. */
void func_15001CEC(s32 map) {
    QuadNode *root;
    u16 *triList;
    s32 i;

    func_1510F800(map);
    root = func_15001B10();

    D_800DBE08[map] = root;

    root->minY = -0x8000;
    root->maxY = 0x7FFF;
    root->next = 0;
    root->halfSize = 0x8000;
    root->x = 0;
    root->z = 0;
    root->child = 0;

    triList = allocate_memory((D_800DBE4C + 1) << 1, 1, 0, 0);

    triList[0] = D_800DBE4C;
    for (i = 0; i < D_800DBE4C; i++) {
        triList[i + 1] = i;
    }

    func_15002008(-0x8000, 0x8000, -0x8000, 0x8000, triList);
    func_10004074(triList);
}

/**
 * Recursively builds the quadtree node centred on (x, z) with half-extents (halfX, halfZ) from the
 * triangles in `triList` (a count followed by triangle indices). A node becomes a leaf, followed in
 * the buffer by its triangle stream, when it overlaps fewer than D_800B0DD8 triangles or is narrower
 * than D_800B0DD4; otherwise it gets four children, chained through their `next` offsets.
 */
QuadNode *func_15001DE0(s32 x, s32 z, s32 halfX, s32 halfZ, s32 arg4, u16 *triList) {
    QuadNode *node;
    QuadNode *child0;
    QuadNode *child1;
    QuadNode *child2;

    s32 minY;
    s32 maxY;

    s32 minTris;
    s32 minSize;

    s32 pad; // matching: unused stack slot keeps the original frame layout
    s32 maxZ;
    s32 minZ;
    s32 maxX;
    s32 minX;
    u16 *subList;

    minTris = D_800B0DD8;
    minSize = D_800B0DD4;

    minX = x - halfX;
    maxX = x + halfX;
    minZ = z - halfZ;
    maxZ = z + halfZ;

    if ((func_15002248(minX, maxX, minZ, maxZ, &minY, &maxY, triList, &subList) < minTris) || (halfX < minSize)) {
        node = func_15001B10();
        D_800B0DD0++;
        node->minY = minY;
        node->maxY = maxY;
        node->halfSize = halfX;
        node->x = x;
        node->z = z;
        node->next = 0;
        node->child = 0;
        func_15002008(minX, maxX, minZ, maxZ, subList);
    } else {
        node = func_15001B10();
        node->minY = minY;
        node->maxY = maxY;
        node->halfSize = halfX;
        node->x = x;
        node->z = z;
        node->next = 0;

        child0 = func_15001DE0(x - (halfX >> 1), z - (halfZ >> 1), halfX >> 1, halfZ >> 1, 0, subList);
        node->child = (u8 *)child0 - (u8 *)node;
        child1 = func_15001DE0(x - (halfX >> 1), (halfZ >> 1) + z, halfX >> 1, halfZ >> 1, 0, subList);
        child0->next = (u8 *)child1 - (u8 *)child0;
        child2 = func_15001DE0((halfX >> 1) + x, z - (halfZ >> 1), halfX >> 1, halfZ >> 1, 0, subList);
        child1->next = (u8 *)child2 - (u8 *)child1;
        child2->next = (u8 *)func_15001DE0((halfX >> 1) + x, (halfZ >> 1) + z, halfX >> 1, halfZ >> 1, 0, subList) - (u8 *)child2;
    }
    func_10004074(subList);
    return node;
}

/**
 * Appends to the quadtree buffer the triangles of `triList` that overlap the box
 * [minX, maxX] x [minZ, maxZ], as a stream of index deltas: a byte for a delta below 0x80,
 * otherwise a big-endian halfword index with bit 15 set. The stream ends with a 0 byte.
 */
void func_15002008(s32 minX, s32 maxX, s32 minZ, s32 maxZ, u16 *triList) {
    MeshTri *tri;
    s32 count;
    s32 index;
    s32 prev;
    s32 delta;
    s32 i;
    s32 j;
    Tri3f pos;

    prev = -1;
    count = triList[0];

    func_150492CC(maxX - minX, 32000.0f, maxZ - minZ);

    for (i = 1; i <= count; i++) {
        index = triList[i];
        tri = &((MeshTri *)D_800DBE3C)[index]; // matching: the no-op cast keeps IDO's addu operand order

        for (j = 0; j < 3; j++) {
            if ((u32)tri->verts[j] > 0x80000000) {
                pos.pts[j].x = tri->verts[j]->v.ob[0] - (minX + maxX) / 2;
                pos.pts[j].y = tri->verts[j]->v.ob[1];
                pos.pts[j].z = tri->verts[j]->v.ob[2] - (minZ + maxZ) / 2;
            } else {
                pos.pts[j].x = 0.0f;
                pos.pts[j].y = 0.0f;
                pos.pts[j].z = 0.0f;
            }
        }

        if (func_15049260(pos) == 0) {
            delta = index - prev;
            if (prev == -1) {
                func_15001B8C(index | 0x8000);
            } else if (delta < 0x80) {
                func_15001B5C(delta);
            } else {
                func_15001B8C(index | 0x8000);
            }
            prev = index;
        }
    }

    func_15001B5C(0);
}

/**
 * Collects into a new list (*subList: count, then indices) the triangles of `triList` that
 * overlap the box [minX, maxX] x [minZ, maxZ], and returns their combined y range in
 * *minY / *maxY, widened to a multiple of 2 << D_80082FA0. Returns the count.
 */
int func_15002248(s32 minX, s32 maxX, s32 minZ, s32 maxZ, s32 *minY, s32 *maxY, u16 *triList, u16 **subList) {
    s32 i;
    s32 j;
    s32 count;
    s32 total;
    s32 index;
    u16 *out;
    MeshTri *tri;
    s32 bound;
    s32 haveRange;
    s32 result;
    s32 lowBits;
    s32 highBits;
    Tri3f pos;

    lowBits = (1 << (D_80082FA0 + 1)) - 1;
    highBits = -(1 << (D_80082FA0 + 1));

    func_150492CC(maxX - minX, 32000.0f, maxZ - minZ);

    total = triList[0] + 1;
    count = 0;
    out = allocate_memory(total << 1, 1, 0, 0);
    *subList = out;
    *minY = 0;
    *maxY = 0;
    haveRange = FALSE;

    for (i = 1; i < total; i++) {
        index = triList[i];
        tri = &((MeshTri *)D_800DBE3C)[index]; // matching: the no-op cast keeps IDO's addu operand order

        for (j = 0; j < 3; j++) {
            if ((u32)tri->verts[j] > 0x80000000) {
                pos.pts[j].x = tri->verts[j]->v.ob[0] - ((minX + maxX) >> 1);
                pos.pts[j].y = tri->verts[j]->v.ob[1];
                pos.pts[j].z = tri->verts[j]->v.ob[2] - ((minZ + maxZ) >> 1);
            } else {
                pos.pts[j].x = 0.0f;
                pos.pts[j].y = 0.0f;
                pos.pts[j].z = 0.0f;
            }
        }

        result = func_15049260(pos);
        if (result == 0) {
            count++;
            out[count] = index;

            if (haveRange) {
                bound = (D_800DBE44[index].min - lowBits) & highBits;
                if (bound < *minY) {
                    *minY = bound;
                }
                bound = ((D_800DBE44[index].max + lowBits) & highBits) + lowBits;
                if (*maxY < bound) {
                    *maxY = bound;
                }
            } else {
                haveRange = TRUE;
                *minY = (D_800DBE44[index].min - lowBits) & highBits;
                *maxY = ((D_800DBE44[index].max + lowBits) & highBits) + lowBits;
            }
        }
    }

    out[0] = count;
    return count;
}

/**
 * Threads the quadtree for traversal: every node without a sibling gets its `next` link pointed
 * at `parentNext`, the node that follows its parent (none at the root).
 */
void func_15002560(QuadNode *node, QuadNode *parentNext) {
    s32 offset;
    QuadNode *next;
    QuadNode *child;

    // matching: for (;;) keeps the null test at the loop top; IDO rotates a plain while
    for (;;) {
        if (node == NULL) {
            return;
        }
        if (node->next == 0) {
            if (parentNext != NULL) {
                offset = (u8 *)parentNext - (u8 *)node;
            } else {
                offset = 0;
            }
            node->next = offset;
        }
        offset = node->child;
        if (offset == 0) {
            return;
        }
        child = (QuadNode *)((u8 *)node + offset);
        while (child->next != 0) {
            next = (QuadNode *)((u8 *)child + child->next);
            func_15002560(child, next);
            child = next;
        }
        // the last child inherits this node's parentNext
        node = child;
    }
}

/**
 * Loads this map's two resources (2 * map and 2 * map + 1 of resource table 5) and hands them to
 * func_150183D0 with a row stride of ceil(D_800DBE2A / 8) bytes, rounded up to 16. Clears
 * D_800BE5D0 when the map has no such resource.
 */
void func_150025FC(void) {
    s32 width;
    s32 height;
    u8 *res0;
    s32 *res1;

    width = D_800DBE2A;
    height = D_800DBE2C;

    D_800DBE28 = (((width + 7) / 8) + 15) & 0xFFF0;
    res0 = func_1502B5C8(NULL, 2, 5, level * 2);
    res1 = func_1502B5C8(NULL, 2, 5, level * 2 + 1);
    if (res0 == NULL) {
        D_800BE5D0 = 0;
    } else {
        func_150183D0(D_800DBE28, height, res0, res1);
        func_10004074(res1);
    }
}

/** Starts collecting the triangles of map `map`. */
void func_150026C4(s32 map) {
    D_800DBE38 = 0;
    func_1510F800(map);
}

/** Records the number of triangles collected for map `map`. */
void func_150026E8(s32 map) {
    D_800DBE18[map] = D_800DBE38;
    D_800DBE4C = D_800DBE38;
    D_800B0DE4 += D_800DBE38;
}

/** Adds the triangle count of display list `dl` to the running count. */
void func_15002724(Gfx *dl) {
    D_800DBE38 += func_150027F8(dl);
}

/**
 * Carves the current map's triangle table (D_800DBDD8), x/z bounds (D_800DBDE8) and
 * y bounds (D_800DBDF8) out of the triangle-data arena, sized for the counted triangles.
 */
void func_15002754(void) {
    s32 count;
    s32 map;

    D_800DBDD8[D_800DBE50] = D_800B0DC0 = (u8 *)ALIGN4(D_800B0DC0);
    count = D_800DBE38;
    map = D_800DBE50;
    D_800B0DC0 += count * sizeof(MeshTri);
    D_800DBDE8[map] = D_800B0DC0;
    D_800B0DC0 += count * sizeof(XZRange);
    D_800DBDF8[map] = D_800B0DC0;
    D_800B0DC0 += count * sizeof(YRange);
    func_1510F800(map);
    D_800DBE38 = 0;
}

/** Returns the number of triangles drawn by display list `dl` (G_TRI1, G_TRI2 and Conker's G_TRI4). */
s32 func_150027F8(Gfx *dl) {
    s32 i;
    s32 numTris;
    s32 opcode;

    if (dl == NULL) {
        return 0;
    }
    i = 0;
    numTris = 0;
    for (opcode = *(s8 *)dl; opcode != (s8)G_ENDDL; opcode = *(s8 *)&dl[i]) {
        i++;
        if ((opcode >> 4) == 1) {
            numTris += 4;
        } else if (opcode == G_TRI2) {
            numTris += 2;
        } else if (opcode == G_TRI1) {
            numTris += 1;
        }
    }
    return numTris;
}

/** Returns the largest per-map triangle count (D_800DBE18[0..3]). */
s32 func_15002878(void) {
    s32 i;
    s32 count;
    s32 highest;

    highest = D_800DBE18[0];

    for (i = 1; i < 4; i++) {
        count = D_800DBE18[i];
        if (highest < count) {
            highest = count;
        }
    }

    return highest;
}


// matching: these stay macros; naming them as locals changes the frame and register allocation
#define CMD (dl + (j << 3)) // command j of the display list, as bytes
#define W0 (*(s32 *)CMD)
#define W1 (*(s32 *)(CMD + 4))
#define RUNS (&D_800C5C08[map])
#define TRIS (&D_800C6070[map])

/**
 * Builds map `map`'s vertex runs (D_800C5C08[map], D_800C5EF8[map] of them, sorted by address)
 * and triangle list (D_800C6070[map]) by walking its display list. Returns 1 when out of memory.
 */
s32 func_150028BC(s32 map) {
    VtxRun *run;
    s32 vaddr;
    s32 numLists;
    u32 vtxIndex;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 count;
    s32 numRuns;
    s32 numTris;
    u32 mtxIndex;
    s8 *dl;
    s32 base;
    s32 verts[32];
    s8 *list;
    s32 opcode;
    s32 first;
    s32 tri[3];
    VtxRun swap;

    mtxIndex = 0;
    if (*RUNS != NULL) {
        return 0;
    }
    numLists = 1;
    count = 0;
    for (i = 0; i < numLists; i++) {
        list = (s8 *)D_800C4488[map][i]; // the display list read as bytes
        j = 0;
        while (list[j << 3] != (s8)G_ENDDL) {
            if (list[j << 3] == G_VTX) {
                count++;
            }
            j++;
        }
    }
    *RUNS = (VtxRun *)allocate_memory(count * sizeof(VtxRun), 1, 0, 2);
    if (*RUNS == NULL) {
        return 1;
    }
    *TRIS = (VtxTri *)allocate_memory(D_800C5918[map] * sizeof(VtxTri), 1, 0, 2);
    if (*TRIS == NULL) {
        func_10004074(*RUNS);
        *RUNS = NULL;
        return 1;
    }
    numRuns = 0;
    numTris = 0;
    base = (s32)D_800D19A0[map];
    for (i = 0; i < numLists; i++) {
        dl = (s8 *)D_800C4488[map][i];
        j = 0;
        while (*CMD != (s8)G_ENDDL) {
            opcode = *(u8 *)CMD;
            k = 0;
            if (opcode == G_VTX) {
                { // matching: this block (its extern takes no stack slot) steers IDO's frame layout
                    extern u16 D_800C5918[];

                    count = (W0 >> 12) & 0xFF;
                    first = (W0 >> 1) & 0x7F;
                    first -= count;
                    vaddr = (W1 & 0xFFFFFF) - 0x38;
                    vtxIndex = (u32)((W1 - 0x38) & 0xFFFFFF) >> 4;
                    for (k = 0; k < count; k++) {
                        verts[k + first] = (k << 4) + vaddr;
                    }
                    if (numRuns != 0 && base + vtxIndex * 16 == (u32)(*RUNS)[numRuns - 1].start + (*RUNS)[numRuns - 1].count * 16 &&
                        mtxIndex == (*RUNS)[numRuns - 1].mtxIndex) {
                        (*RUNS)[numRuns - 1].count += count;
                    } else {
                        (*RUNS)[numRuns].start = (Vtx *)(vtxIndex * 16 + base);
                        (*RUNS)[numRuns].mtxIndex = mtxIndex;
                        (*RUNS)[numRuns].count = count;
                        numRuns++;
                    }
                }
            } else if (opcode == G_MTX) {
                mtxIndex = (u32)W1 & 0xFFFFFF;
                mtxIndex >>= 6;
            } else if (opcode == G_TRI1) {
                for (k = 0; k < 3; k++) {
                    vaddr = *(u8 *)(CMD + k + 1); // matching: vertex index byte reuses vaddr
                    (*TRIS)[numTris].v[k] = verts[vaddr >> 1];
                }
                numTris++;
            } else if (opcode == G_TRI2) {
                for (m = 0; m < 2; m++) {
                    for (k = 0; k < 3; k++) {
                        vaddr = *(u8 *)(CMD + m * 4 + k + 1);
                        (*TRIS)[numTris].v[k] = verts[vaddr >> 1];
                    }
                    numTris++;
                }
            } else if ((opcode >> 4) == 1) {
                for (m = 0; m < 4; m++) {
                    if (m == 0) {
                        tri[0] = (W1 >> 25) & 0x1F;
                        tri[1] = (W1 >> 20) & 0x1F;
                        tri[2] = (W1 >> 15) & 0x1F;
                    } else if (m == 1) {
                        tri[0] = (W1 >> 10) & 0x1F;
                        tri[1] = (W1 >> 5) & 0x1F;
                        tri[2] = W1 & 0x1F;
                    } else if (m == 2) {
                        tri[0] = (W0 >> 10) & 0x1F;
                        tri[1] = (W0 >> 5) & 0x1F;
                        tri[2] = W0 & 0x1F;
                    } else {
                        tri[0] = (W0 >> 23) & 0x1F;
                        tri[1] = (W0 >> 18) & 0x1F;
                        tri[2] = ((W0 >> 13) & 0x1C) | ((W1 >> 30) & 3);
                    }
                    for (k = 0; k < 3; k++) {
                        (*TRIS)[numTris].v[k] = verts[tri[k]];
                    }
                    numTris++;
                }
            }
            j++;
        }
    }
    D_800C5EF8[map] = numRuns;
    // gnome sort of the runs by address
    // matching: for (;;) + break keeps the test at the loop top; IDO rotates a plain while
    j = 1;
    for (;;) {
        if (j >= D_800C5EF8[map]) {
            break;
        }
        run = &(*RUNS)[j];
        if (run->start < run[-1].start) {
            swap = *run;
            *run = run[-1];
            (*RUNS)[j - 1] = swap;
            if (j >= 2) {
                j--;
            }
        } else {
            j++;
        }
    }
    return 0;
}
#undef RUNS
#undef TRIS
#undef W1
#undef W0
#undef CMD

/** Sets D_800DBE64 for map `map`: 150 on maps 50 and 51, otherwise 85 when D_800BE616 is set, else 50. */
void func_15002F40(s32 map) {
    if (map == 50 || map == 51) {
        D_800DBE64 = 150.0f;
    } else if (D_800BE616) {
        D_800DBE64 = 85.0f;
    } else {
        D_800DBE64 = 50.0f;
    }
}

/** XOR (un)scrambling for the constants of the ROM check below. */
s32 func_15002FA0(s32 value) {
    return value ^ 0xF0F0F0F0;
}

/**
 * Loads map `map`'s settings resource into D_800B0DF0 (fatal error if it is missing) and
 * publishes its D_800D2F3C/D_800D2F40 table. If the cartridge word at 0xB0000A5C is not the
 * expected one, the table length unk12 is halved.
 */
void func_15002FB4(s32 map) {
    LevelSettingsAsset *res;
    s32 count;
    u32 loaded;
    LevelHeader *info;
    u32 romWord;

    res = func_1502B6BC(&loaded, 0, &count, 2, 21, map);
    if (count != 0 && (res->unk4 << 8) != 0) {
        D_800B0DF0 = res->header;
        if (loaded == 0) {
            D_8003C8E0 = 0xC000037;
            func_150AD770();
        }
        if (D_800B0DF0 == NULL) {
            D_8003C8E0 = 0xC000037;
            func_150AD770();
        }
    }
    // func_15002FA0(0x40F0FAAC) == 0xB0000A5C, func_15002FA0(0xE7B00F09) == 0x1740FFF9
    func_151EF040(func_15002FA0(0x40F0FAAC), &romWord);
    romWord = func_15002FA0(romWord);
    if (romWord != 0xE7B00F09) {
        info = D_800B0DF0;
        info->unk12 >>= 1;
    }

    info = D_800B0DF0;
    if (info->unk12 != 0) {
        D_800D2F3C = info->unk12;
        D_800D2F40 = res->idList;
    } else {
        D_800D2F3C = 0;
        D_800D2F40 = NULL;
    }

    if (D_8008FD90 >= 2 && info->unk8 == 4) {
        info->unk8 = 2;
    }
}

void func_1500310C(void) {
}
