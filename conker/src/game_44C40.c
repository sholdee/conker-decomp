#include <ultra64.h>

#include "functions.h"
#include "variables.h"

#define OBJ_TYPE(obj) ((obj)->kind >> 2)
#define OBJ_SHAPE(obj) ((obj)->kind & 3)

extern void *allocate_memory(s32, s32, s32, s32);

/* Clears the per-id record flag bits (D_800D2E4C) and state tables. */
void func_15017790(void) {
    func_1509C120();
    bzero(D_800D2E4C, 0x1B);
    bzero(&D_800D2E50, 0x10);
    bzero(&D_800D2E60, 9);
    bzero(D_800D2E70, 0xCC);
    D_800D2E44 = 0;
}

/* Resets the per-id record list (D_800D2F48) and related state. */
void func_150177F8(void) {
    func_1509BA04(1);
    bzero(&D_800D2F48, 0xC);
    func_150A00F0();
    D_800D2DB0 = 999.0f;
    D_800D2E41 = 10;
    D_800D2E42 = 6;
    D_800D3858 = 0;
    D_800D2E44 = 0;
}

IdRecord *func_1509B704(s16 id);

/* Stamps every listed record that has flag 0x1000 with D_800BE9F0, then reloads the records. */
void func_15017868(void) {
    IdRecord *node;
    s32 i;

    for (i = 0; i < D_800D2F3C; i++) {
        node = func_1509B704(D_800D2F40[i]);
        if (node != NULL && (node->key & 0x1000)) {
            node->unk2 = D_800BE9F0;
        }
    }
    func_1509BA04(0);
    func_1509B4A0((s16)D_800BE9F0, 1);
}

void func_15017B20(TriggerZone *objects, s32 count);

/* Loads the level's object table and builds the list of type-1 objects in modes 0, 2 and 6. */
void func_15017930(s32 arg0) {
    u32 i;
    u32 count;
    s32 type;

    D_800D3094 = func_1502B7F0((void **)&D_800D3098, 3, 0xC, arg0, 5);
    count = D_800D3094 / sizeof(TriggerZone);
    D_800D3094 = count;
    func_15017B20(D_800D3098, count);
    D_800D3274 = 0;
    D_800D3270 = NULL;
    for (i = 0; i < D_800D3094; i++) {
        if (OBJ_TYPE(&D_800D3098[i]) == 1 &&
            (D_800D3098[i].subtype == 2 || D_800D3098[i].subtype == 0 || D_800D3098[i].subtype == 6)) {
            D_800D3274++;
        }
        type = OBJ_TYPE(&D_800D3098[i]);
        if (type == 2 || type == 7) {
            D_800D3098[i].unk20 = 0;
        }
    }
    if (D_800D3274 != 0) {
        D_800D3270 = allocate_memory(D_800D3274 * sizeof(TriggerZone *), 1, 0, 0);
        if (D_800D3270 != NULL) {
            TriggerZone **out = D_800D3270;

            for (i = 0; i < D_800D3094; i++) {
                if (OBJ_TYPE(&D_800D3098[i]) == 1 &&
                    (D_800D3098[i].subtype == 2 || D_800D3098[i].subtype == 0 || D_800D3098[i].subtype == 6)) {
                    *out++ = &D_800D3098[i];
                }
            }
        }
    }
    bzero(D_800D30F0, sizeof(D_800D30F0));
    bzero(D_800D30D0, sizeof(D_800D30D0));
}

extern void func_150A0D14(TriggerZone *obj);
void func_15017FA4(TriggerZone *obj, ZoneBounds *bounds);

/* Whether the ranges [a0, a1) and [b0, b1) overlap. */
#define OVERLAP(a0, a1, b0, b1) (((a0 <= b0) && (b0 < a1)) || ((b0 < a0) && (a0 < b1)) || ((a0 < b1) && (b1 < a1)))

/*
 * Computes the bounds of every level object and the object bitsets D_800D30A0 (shape 3 or flag 8,
 * without flag 4) and D_800D30B8 (all others without flag 4). For each object in D_800D30B8 it
 * records which other D_800D30B8 objects its bounds overlap.
 */
void func_15017B20(TriggerZone *objects, s32 count) {
    s32 pad[6]; // matching: stack frame size
    u64 bitI;
    u64 bitJ;
    u64 *wordI;
    u64 *wordJ;
    u64 *overlapWord;
    s32 i;
    s32 j;

    D_800D309C = allocate_memory(count * sizeof(ZoneBounds), 1, 1, 0);
    for (i = 0; i < count; i++) {
        func_150A0D14(&objects[i]);
        func_15017FA4(&objects[i], &D_800D309C[i]);
    }
    bzero(D_800D30A0, sizeof(D_800D30A0));
    for (i = 0; i < count; i++) {
        if ((OBJ_SHAPE(&objects[i]) == 3) || (objects[i].flags & 8)) {
            if (!(objects[i].flags & 4)) {
                D_800D30A0[i >> 6] |= (u64)1 << (i & 63);
            }
        }
    }
    bzero(D_800D30B8, sizeof(D_800D30B8));
    for (i = 0; i < count; i++) {
        if (OBJ_SHAPE(&objects[i]) != 3) {
            if (!(objects[i].flags & 8) && !(objects[i].flags & 4)) {
                D_800D30B8[i >> 6] |= (u64)1 << (i & 63);
            }
        }
    }
    wordI = D_800D30B8;
    bitI = 1;
    for (i = 0; i < count; i++) {
        wordJ = D_800D30B8;
        D_800D309C[i].numOverlaps = 0;
        if (*wordI & bitI) {
            bitJ = 1;
            overlapWord = (u64 *)&D_800D309C[i]; // matching: same as .overlaps, but that form schedules differently
            bzero(D_800D309C[i].overlaps, sizeof(D_800D309C[i].overlaps));
            for (j = 0; j < count; j++) {
                if (*wordJ & bitJ) {
                    if (OVERLAP(D_800D309C[i].minX, D_800D309C[i].maxX, D_800D309C[j].minX, D_800D309C[j].maxX) &&
                        OVERLAP(D_800D309C[i].minY, D_800D309C[i].maxY, D_800D309C[j].minY, D_800D309C[j].maxY) &&
                        OVERLAP(D_800D309C[i].minZ, D_800D309C[i].maxZ, D_800D309C[j].minZ, D_800D309C[j].maxZ)) {
                        *overlapWord |= bitJ;
                        D_800D309C[i].numOverlaps++;
                    }
                }
                bitJ <<= 1;
                if (bitJ == 0) {
                    wordJ++;
                    overlapWord++;
                    bitJ = 1;
                }
            }
        }
        bitI <<= 1;
        if (bitI == 0) {
            wordI++;
            bitI = 1;
        }
    }
}

void func_150A7960(MtxF m, f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);
void func_150A7A48(MtxF a, MtxF b, MtxF c);
void func_150A7DA0(MtxF mtx, f32, f32, f32);

/*
 * Computes obj's axis-aligned bounds: the extremes (and the origin) of the unit cube's corners transformed by the
 * object's scale (from its size and shape) and rotation, offset by its position. Unknown shapes
 * get zero-size bounds at the position.
 */
void func_15017FA4(TriggerZone *obj, ZoneBounds *bounds) {
    s32 i;
    s32 shape;
    s32 invalid;
    s32 minX, maxX, minY, maxY, minZ, maxZ;
    MtxF transform;
    MtxF rotation;
    f32 signX, signY, signZ;
    f32 x, y, z;

    invalid = 0;
    shape = OBJ_SHAPE(obj);
    switch (shape) {
    default:
        invalid = 1;
        break;
    case 0:
        func_150A7DA0(rotation, 0.0f, 1.0f, 0.0f);
        func_150A7CB0(transform, obj->radius, obj->height * 0.5f, obj->radius);
        func_150A7A48(rotation, transform, transform);
        func_150A8050(rotation, obj->pitch, obj->yaw, 0.0f);
        func_150A7A48(transform, rotation, transform);
        break;
    case 1:
        func_150A7CB0(transform, obj->radius, obj->radius, obj->radius);
        break;
    case 2:
        func_150A7DA0(rotation, 0.0f, 1.0f, 0.0f);
        func_150A7CB0(transform, obj->radius, obj->height * 0.5f, obj->depth);
        func_150A7A48(rotation, transform, transform);
        func_150A8050(rotation, obj->pitch, obj->yaw, 0.0f);
        func_150A7A48(transform, rotation, transform);
        break;
    }
    minZ = 0;
    minY = 0;
    minX = 0;
    maxZ = 0;
    maxY = 0;
    maxX = 0;
    if (!invalid) {
        for (i = 0; i < 8; i++) {
            if (i & 1) {
                signX = 1.0f;
            } else {
                signX = -1.0f;
            }
            if (i & 2) {
                signY = 1.0f;
            } else {
                signY = -1.0f;
            }
            if (i & 4) {
                signZ = 1.0f;
            } else {
                signZ = -1.0f;
            }
            func_150A7960(transform, signX, signY, signZ, &x, &y, &z);
            if (x < minX) {
                minX = x;
            } else if (maxX < x) {
                maxX = x;
            }
            if (y < minY) {
                minY = y;
            } else if (maxY < y) {
                maxY = y;
            }
            if (z < minZ) {
                minZ = z;
            } else if (maxZ < z) {
                maxZ = z;
            }
        }
    }
    bounds->minX = obj->x + minX;
    bounds->maxX = obj->x + maxX;
    bounds->minY = obj->y + minY;
    bounds->maxY = obj->y + maxY;
    bounds->minZ = obj->z + minZ;
    bounds->maxZ = obj->z + maxZ;
}
