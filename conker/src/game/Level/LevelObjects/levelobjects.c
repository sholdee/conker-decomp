#include <ultra64.h>

#include "structs.h"
#include "effects.h"
#include "conker/collision.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/level.h"
#include "conker/prop.h"
#include "conker/rom.h"
#include "conker/texture.h"

/*
 * This file saw allocate_memory with a u32 tag, not the s32 tag of conker/heap.h, so it does not include
 * conker/heap.h and declares allocate_memory here.
 */
/* matching: this file saw a u32 tag (see level_spawnObjects: the hoisted 1 it passes is shared with the 1U compare) */
void *allocate_memory(s32 size, u32 tag, s32 align, s32 mode); /* init/Heap/Allocator/allocator.c */

static u16 gCullRadiusTooSmallCount; // objects whose preset radius is too small (level_setObjectCullRadius)
static u16 gCullRadiusComputedCount; // objects whose radius was computed
static u16 gCullRadiusLargeEnoughCount; // objects whose preset radius is large enough

/* Creates the level's objects (db_Object_array) from its two placement lists and allocates per-object buffers. */
void level_spawnObjects(s32 levelId, s32 arg1) {
    s32 i;
    s32 found;
    s32 id;
    u32 loaded;
    s32 j;
    ObjectSpawn *spawns;
    ObjectSpawn *groupedSpawns;
    ModelAsset *model;
    ObjectFunc behavior;
    s32 off; // matching: unused, but IDO reserves its stack slot
    WorldObject *obj;
    s8 setStateBit2;
    s32 isBehavior22;
    ModelAsset *levelModel;
    ObjectSpawn *base;

    D_800DBF88 = 0xFF;
    spawns = NULL;
    groupedSpawns = NULL;
    setStateBit2 = FALSE;
    gCullRadiusTooSmallCount = gCullRadiusComputedCount = gCullRadiusLargeEnoughCount = 0;
    i = rom_loadPathTo(&spawns, 2, 0xB, levelId);
    gUngroupedObjectCount = i / sizeof(ObjectSpawn);
    gWorldObjectCount = gUngroupedObjectCount;
    gGroupedObjectCount = rom_loadPathTo(&groupedSpawns, 3, 0xC, levelId, 2) / sizeof(ObjectSpawn);
    gWorldObjectCount += gGroupedObjectCount;
    db_Object_array = allocate_memory(gWorldObjectCount * sizeof(WorldObject), 1, 2, 0);
    if (db_Object_array == NULL) {
        gWorldObjectCount = 0;
        gGroupedObjectCount = 0;
        gUngroupedObjectCount = 0;
        return;
    }
    // base[i] is object i's spawn: the first list, then the grouped list
    base = spawns;
    for (i = 0; i < gWorldObjectCount; i++) {
        if (i == gUngroupedObjectCount) {
            base = &groupedSpawns[-i]; // so that base[i] is groupedSpawns[0]
        }
        found = 0;
        isBehavior22 = FALSE;
        obj = &db_Object_array[i];
        obj->x = base[i].x;
        obj->y = base[i].y;
        obj->z = base[i].z;
        obj->t = base[i].rotX;
        obj->u = base[i].rotY;
        obj->v = base[i].rotZ;
        obj->xScale = base[i].scaleX;
        obj->yScale = base[i].scaleY;
        obj->zScale = base[i].scaleZ;
        obj->displayList = base[i].displayList;
        obj->state = base[i].state & ~8;
        obj->fadeDistance = 0;
        obj->inactive = base[i].inactive;
        obj->moveData.data = base[i].data;
        behavior = base[i].behavior.func;
        if (base[i].behavior.index < 0xFFFF) {
            // matching: the (s32) casts keep the compares on a copy of the loaded word
            if ((s32)base[i].behavior.index == 0x28 || (s32)base[i].behavior.index == 0x42) {
                obj->state |= 4;
                setStateBit2 = TRUE;
            }
            obj->moveProc = gObjectBehaviors[base[i].behavior.index].moveProc;
            obj->onContact = gObjectBehaviors[base[i].behavior.index].onContact;
            obj->contactNonPlayers = gObjectBehaviors[base[i].behavior.index].contactNonPlayers;
            if (base[i].behavior.index == 22) {
                isBehavior22 = TRUE;
            }
        } else {
            obj->moveProc = behavior;
            obj->onContact = NULL;
        }
        obj->floorHitAttrib = base[i].material;
        obj->touch.floor.unk7C = 0;
        obj->touch.floor.tri = 0;
        *(s32 *)&obj->touch.floor.y = 0; // matching: an integer zero store, not 0.0f
        obj->sound_id = 0;
        obj->attach = 0;
        obj->effectFlags = 0;
        obj->task_flag = TASK_FLAG_LOCKED;
        obj->spinX = obj->spinY = obj->spinZ = 0.0f;
        obj->moveX = obj->moveY = obj->moveZ = obj->spinZ; // matching: 0 via spinZ, not a literal
        obj->yOffset = 0;
        obj->alpha = 0xFF;
        obj->dst_fade_alpha[0] = obj->dst_fade_alpha[1] = obj->dst_fade_alpha[2] = obj->dst_fade_alpha[3] = 0xFF;
        obj->object_num = i;
        obj->shadow = NULL;
        obj->unk98 = 0;
        if (obj->displayList == NULL) {
            id = base[i].modelId;
            for (j = gUngroupedObjectCount; j < i; j++) {
                if (base[j].displayList == base[i].displayList && base[j].modelId == base[i].modelId
                    && db_Object_array[j].displayList != NULL) {
                    found = j + 1;
                    break;
                }
            }
            if (!found) {
                model = rom_loadPathTable(&loaded, 0, 0, 2, 3, id);
                obj->model = model;
            }
            if (found) {
                obj->srcVertices = db_Object_array[found - 1].srcVertices;
                obj->displayList = db_Object_array[found - 1].displayList;
                obj->unk9C = db_Object_array[found - 1].unk9C;
                obj->numVertices = db_Object_array[found - 1].numVertices;
                obj->materials = db_Object_array[found - 1].materials;
            } else {
                obj->srcVertices = model->vertices;
                obj->displayList = model->displayList;
                obj->unk9C = model->unk8;
                obj->numVertices = (Vtx *)obj->displayList - obj->srcVertices;
                obj->materials = model->materials;
            }
            if (i >= gUngroupedObjectCount) {
                obj->unk4C = gGroupedObjectDefaults[id].unk4C;
                obj->shadowSize = gGroupedObjectDefaults[id].shadowSize;
                obj->cullRadiusXZ = gGroupedObjectDefaults[id].cullRadiusXZ;
                obj->cullRadiusY = gGroupedObjectDefaults[id].cullRadiusY;
                obj->object_status = gGroupedObjectDefaults[id].flags;
                obj->state = gGroupedObjectDefaults[id].state & ~8;
                obj->effectFlags = gGroupedObjectDefaults[id].effectFlags;
                if (setStateBit2) {
                    obj->state |= 4;
                }
                obj->drawDistance = gGroupedObjectDefaults[id].drawDistance;
                obj->unique_id = base[i].id;
            } else {
                obj->cullRadiusXZ = base[i].cullRadiusXZ;
                obj->cullRadiusY = base[i].cullRadiusY;
                obj->object_status = base[i].flags;
                obj->unk4C = base[i].unk4C;
                obj->shadowSize = base[i].shadowSize;
                obj->drawDistance = base[i].drawDistance;
                if (base[i].id != 0) {
                    obj->unique_id = base[i].id;
                } else {
                    obj->unique_id = level_findFreeObjectId(TRUE);
                }
            }
            if (id == 0x71) {
                obj->floorHitAttrib = 0xE;
            }
            if (obj->moveProc != NULL || isBehavior22) {
                if (obj->inactive == 0) {
                    obj->viewFlags = 0xF;
                } else {
                    obj->viewFlags = 0;
                }
            } else {
                obj->viewFlags = 0x80;
            }
            obj->modelId = id;
        } else {
            obj->modelId = base[i].modelId | 0x8000;
            if ((s32)obj->displayList == 1 || (s32)obj->displayList == 2) {
                for (j = 0; j < i; j++) {
                    if (base[j].displayList == base[i].displayList && base[j].modelId == base[i].modelId
                        && db_Object_array[j].displayList != NULL) {
                        found = j + 1;
                        break;
                    }
                }
                if (!found) {
                    id = base[i].modelId;
                    levelModel = gLevelModels[id].model;
                    obj->numVertices = ((u32)levelModel->displayList - (u32)levelModel->vertices) / sizeof(Vtx);
                    obj->srcVertices = levelModel->vertices;
                    obj->materials = levelModel->materials;
                    obj->unk9C = levelModel->unk8;
                } else {
                    obj->numVertices = db_Object_array[found - 1].numVertices;
                    obj->srcVertices = db_Object_array[found - 1].srcVertices;
                    obj->materials = db_Object_array[found - 1].materials;
                    obj->unk9C = db_Object_array[found - 1].unk9C;
                }
                if ((s32)obj->displayList == 2) {
                    obj->viewFlags = 0;
                } else {
                    obj->viewFlags = 0x80;
                }
                if (!found) {
                    obj->displayList = levelModel->displayList;
                } else {
                    obj->displayList = db_Object_array[found - 1].displayList;
                }
                obj->effectFlags = base[i].effectFlags;
                obj->model = NULL;
            }
            obj->cullRadiusXZ = base[i].cullRadiusXZ;
            obj->cullRadiusY = base[i].cullRadiusY;
            obj->object_status = base[i].flags;
            obj->unk4C = base[i].unk4C;
            obj->shadowSize = base[i].shadowSize;
            obj->drawDistance = base[i].drawDistance;
            if (base[i].id != 0) {
                obj->unique_id = base[i].id;
            } else {
                obj->unique_id = level_findFreeObjectId(TRUE);
            }
        }
        level_setObjectCullRadius(obj, &base[i]);
        if (obj->moveProc == NULL && !isBehavior22) {
            obj->viewFlags |= 0x20;
        }
        if (!found) {
            texture_resolveDl(obj->displayList, 0, 1, 0x3F, 0);
            level_rebaseDlMatrices((DlCommand *)obj->displayList, (u32)model);
        }
    }
    for (i = 0; i < gWorldObjectCount; i++) {
        obj = &db_Object_array[i];
        if (obj->srcVertices == NULL) {
            continue;
        }
        if ((obj->state & 1) == 1U) {
            obj->vertices[0] = allocate_memory(obj->numVertices * sizeof(Vtx), 1, 2, 0);
            obj->vertices[1] = allocate_memory(obj->numVertices * sizeof(Vtx), 1, 2, 0);
        } else {
            obj->vertices[0] = obj->vertices[1] = obj->srcVertices;
        }
        if (obj->vertices[0] != obj->srcVertices) {
            for (j = 0; j < obj->numVertices; j++) {
                obj->vertices[0][j] = obj->srcVertices[j];
            }
            if (obj->vertices[0] != obj->vertices[1]) {
                for (j = 0; j < obj->numVertices; j++) {
                    obj->vertices[1][j] = obj->srcVertices[j];
                }
            }
        }
    }
    gObjectMtxsA = allocate_memory(gWorldObjectCount * 0x40, 1, 2, 0);
    gObjectMtxsB = allocate_memory(gWorldObjectCount * 0x40, 1, 2, 0);
    gObjectBlocks = allocate_memory(gWorldObjectCount * sizeof(void *), 1, 0, 0);
    gObjectBlockDelays = allocate_memory(gWorldObjectCount, 1, 0, 0);
    for (i = 0; i < 4; i++) {
        gViewDrawLists[i] = allocate_memory(gWorldObjectCount * 4, 1, 0, 0);
    }
    gWorldObjectContacts = allocate_memory(gWorldObjectCount * sizeof(s32), 1, 0, 0);
    for (i = 0; i < gWorldObjectCount; i++) {
        gWorldObjectContacts[i] = 0;
    }
    for (i = gUngroupedObjectCount; i < gWorldObjectCount; i++) {
        if (db_Object_array[i].unique_id == 0) {
            db_Object_array[i].unique_id = level_findFreeObjectId(FALSE);
        }
    }
}

/* Saves the display list and source vertices of object D_800DBF88, unless it is 0xFF (none). */
void func_15004574(void) {
    if (D_800DBF88 != 0xFF) {
        D_800DBF8C = db_Object_array[D_800DBF88].displayList;
        D_800DBF90 = db_Object_array[D_800DBF88].srcVertices;
    }
}

/* Does nothing. */
void func_150045BC(void) {
}

/*
 * Prepares the objects for the level: builds each object's matrices for both frames (or, for objects with
 * state bit 0, transforms its vertices into its per-frame copies), then probes the floor under objects
 * with view flag 0x20.
 */
void level_prepareObjects(s32 arg0) {
    s32 i;
    WorldObject *obj;
    MtxF mtx;
    s32 j;
    u32 floorFlags;
    f32 floorY;
    void *floorTri;
    f32 x;
    f32 y;
    f32 z;

    level_clearObjectBlocks();
    guMtxIdentF(gObjectIdentityMtxs[0]);
    guMtxIdentF(gObjectIdentityMtxs[1]);
    for (i = 0; i < gWorldObjectCount; i++) {
        obj = &db_Object_array[i];
        if (obj->moveProc == NULL || obj->moveProc == prop_scrollTexture || obj->moveProc == prop_registerTextureScroll) {
            obj->attach = 3;
        }
        if (!(obj->state & 1)) {
            gfx_getObjectModelMtx(mtx, obj);
            guMtxF2L(mtx, &(*gObjectMtxBuffers[gDrawBufferIndex])[i]);
            bcopy(&(*gObjectMtxBuffers[gDrawBufferIndex])[i], &(*gObjectMtxBuffers[!gDrawBufferIndex])[i], sizeof(Mtx));
        }
        if (obj->moveProc == prop_emptyBehavior22) {
            obj->moveProc = NULL;
        }
        if ((obj->state & 1) == 1) {
            gfx_getObjectModelMtx(mtx, obj);
            for (j = 0; j < obj->numVertices; j++) {
                math_transformPoint(mtx, obj->srcVertices[j].v.ob[0], obj->srcVertices[j].v.ob[1],
                              obj->srcVertices[j].v.ob[2], &x, &y, &z);
                obj->vertices[0][j].v.ob[0] = x;
                obj->vertices[0][j].v.ob[1] = y;
                obj->vertices[0][j].v.ob[2] = z;
            }
            bcopy(obj->vertices[0], obj->vertices[1], obj->numVertices * sizeof(Vtx));
        }
        collision_selectMap(2);
        if ((obj->viewFlags & 0x80) == 0x80 && (obj->object_status & 0x60) != 0x20) {
            gSurfSetA[0] = i;
            collision_transformPropTris(1, gSurfSetA, 1);
        }
    }
    for (i = 0; i < gWorldObjectCount; i++) {
        obj = &db_Object_array[i];
        if (obj->inactive == 1) {
            continue;
        }
        if ((obj->viewFlags & 0x20) == 0x20) {
            collision_findFloorInRange(&floorTri, NULL, &floorY, NULL, &floorFlags, NULL, obj->x, obj->y, obj->z, obj->y,
                          i + 1, NULL, -10000.0f, 20000.0f);
            obj->touch.floor.tri = floorTri;
            obj->touch.floor.y = floorY;
        }
    }
}
/*
 * Relocates a display list: adds `offset` to the address of each G_VTX (and G_DL) command and
 * `matrixOffset` to the address of each G_MOVEMEM of a matrix.
 */
void level_relocateDl(DlCommand *dl, s32 offset, s32 matrixOffset) {
    s32 i;
    DlCommand *p;
    s8 cmd;

    i = 0;
    // matching: the byte reload through a u8 pointer picks IDO's register for p
    if ((s8)*(u8 *)&dl[i] != (s8)G_ENDDL) {
        p = &dl[i];
        cmd = p->opcode;
        do {
            i++;
            switch (cmd) {
                case G_DL: // never matches: cmd is signed
                    p->addr += offset;
                    break;
                case G_VTX:
                    p->addr += offset;
                    break;
                case (s8)G_MOVEMEM:
                    if (p->index == G_MV_MATRIX) {
                        p->addr += matrixOffset;
                    }
                    break;
            }
            p = &dl[i];
            cmd = p->opcode;
        } while ((s8)G_ENDDL - cmd != 0); // matching: as a subtraction, for the bne operand order
    }
}

/* Clears every object's handle and release delay. */
void level_clearObjectBlocks(void) {
    s32 i;

    for (i = 0; i < gWorldObjectCount; i++) {
        gObjectBlocks[i] = 0;
        gObjectBlockDelays[i] = 0;
    }
}


/*
 * Sets the object's bounding radius (cullRadiusXZ, cullRadiusY) from its farthest scaled vertex if it has none,
 * and counts in gCullRadiusTooSmallCount/2/4 whether a preset radius is too small, was computed, or is large enough.
 */
void level_setObjectCullRadius(WorldObject *obj, ObjectSpawn *spawn) {
    s32 i;
    f32 x;
    f32 y;
    f32 z;
    s32 distSq;
    s32 maxDistSq = 0;
    s32 radius;
    Vtx *vtx;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;

    i = 0;
    if (obj->numVertices > 0) {
        vtx = obj->srcVertices;
        scaleZ = obj->zScale;
        z = 0.0f; // matching: these dead stores of z and x set IDO's FP register order
        scaleX = obj->xScale;
        x = 0.0f;
        scaleY = obj->yScale;
        do {
            z = vtx[i].v.ob[2] * scaleZ;
            x = vtx[i].v.ob[0] * scaleX;
            y = vtx[i].v.ob[1] * scaleY;
            distSq = z * z + (x * x + y * y);
            if (maxDistSq < distSq) {
                maxDistSq = distSq;
            }
            i++;
        } while (i < obj->numVertices);
    }
    if (maxDistSq != 0) {
        f32 root = sqrtf(maxDistSq);

        if (obj->cullRadiusXZ == 0) {
            radius = root;
            obj->cullRadiusXZ = radius;
            obj->cullRadiusY = radius;
            gCullRadiusComputedCount++;
        } else {
            radius = root;
            if (obj->cullRadiusXZ < radius || obj->cullRadiusY < radius) {
                gCullRadiusTooSmallCount++;
            } else {
                gCullRadiusLargeEnoughCount++;
            }
        }
    }
}

/*
 * Returns an object id that no object uses yet. With `fromTop` 0, counts up from 1 over the grouped
 * objects (0xFF if all are taken); otherwise counts down from 0xFF - gUngroupedObjectCount over all objects.
 */
s32 level_findFreeObjectId(s32 fromTop) {
    s32 i;
    s32 id;

    // matching: the parameter doubles as the start value so that IDO keeps it in a0
    if (fromTop == 0) {
        id = 1;
        for (i = fromTop = gUngroupedObjectCount; i < gWorldObjectCount; i++) {
            if (id == db_Object_array[i].unique_id) {
                id++;
                i = fromTop - 1; // rescan with the next id
            }
            if (id >= 0x100) {
                id = 0xFF;
                break;
            }
        }
    } else {
        fromTop = 0xFF - gUngroupedObjectCount;
        id = fromTop;
        for (i = 0; i < gWorldObjectCount; i++) {
            if (id == db_Object_array[i].unique_id) {
                id--;
                i = -1; // rescan with the next id
            }
            if (id <= 0) {
                id = fromTop;
                break;
            }
        }
    }
    return id;
}
/* Rebases the segment-relative (below 0x80000000) matrix addresses of a display list's G_MOVEMEMs by `base`. */
void level_rebaseDlMatrices(DlCommand *dl, u32 base) {
    s32 i = 0;
    DlCommand *p = dl;
    s8 cmd;

    if (dl->opcode == (s8)G_ENDDL) {
        return;
    }
    p = &dl[i];
    cmd = p->opcode;
    do {
        i++;
        if (p->opcode == (s8)G_MOVEMEM && p->index == G_MV_MATRIX && p->addr < 0x80000000) {
            p->addr += base;
        }
        p = &dl[i];
        do { cmd = p->opcode; } while (0); // matching: the wrapper keeps this dead reload, as in the original
    } while (p->opcode != (s8)G_ENDDL);
}
