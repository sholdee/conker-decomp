#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef void (*ObjectFunc)();

/* A level object placement as stored in the level's object lists (0x44 bytes each). */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 rotX;
    /* 0x08 */ s16 rotY;
    /* 0x0A */ s16 rotZ;
    /* 0x0C */ Gfx *displayList; // NULL: load model `modelId`; 1 or 2: use level model `modelId`
    /* 0x10 */ s32 modelId;
    /* 0x14 */ union {
        u32 index;         // into D_80088C90 when below 0xFFFF
        ObjectFunc func;   // otherwise the update function itself
    } behavior;
    /* 0x18 */ void *data;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ f32 scaleX;
    /* 0x24 */ f32 scaleY;
    /* 0x28 */ f32 scaleZ;
    /* 0x2C */ u8 unk4C;
    /* 0x2D */ u8 unk4D;
    /* 0x2E */ u16 unk50;
    /* 0x30 */ u16 unk52;
    /* 0x32 */ u8 flags;
    /* 0x33 */ u8 id;            // 0: assign a free one
    /* 0x34 */ u8 inactive;
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ s32 material;
    /* 0x3C */ u8 state;
    /* 0x3D */ u8 pad3D;
    /* 0x3E */ u16 effectFlags;
    /* 0x40 */ u16 drawDistance;
    /* 0x42 */ u8 pad42[2];
} ObjectSpawn; // size 0x44


/* Defaults for objects of the second (grouped) list, indexed by model id (D_800A26C0). */
typedef struct {
    /* 0x0 */ u8 unk4C;
    /* 0x1 */ u8 unk4D;
    /* 0x2 */ s16 unk52;
    /* 0x4 */ s16 unk50;
    /* 0x6 */ u16 drawDistance;
    /* 0x8 */ u8 flags;
    /* 0x9 */ u8 state;
    /* 0xA */ u16 effectFlags;
} GroupedObjectDefaults;


/* A display list command, viewed as its opcode, the G_MOVEMEM index byte and the address word. */
typedef struct {
    /* 0x0 */ s8 opcode;
    /* 0x1 */ u8 pad1[2];
    /* 0x3 */ u8 index;
    /* 0x4 */ u32 addr;
} DlCommand;

extern GroupedObjectDefaults D_800A26C0[];
extern void *D_800DBED0[4];
extern void *D_800DBEE0;
extern void *D_800DBEE4;
void *allocate_memory(s32, u32, s32, s32);
ModelAsset *func_1502B6BC(s32 *, s32, s32, s32, s32, s32);
void func_1510CE60(Gfx *, s32, s32, s32, s32);
extern u16 D_800BE2A0;
extern u16 D_800BE2A2;
extern u16 D_800BE2A4;
void func_15004AAC(WorldObject *obj, ObjectSpawn *spawn);
s32 func_15004BF0(s32 fromTop);
void func_15004CE0(DlCommand *dl, u32 base);

/* Creates the level's objects (D_800DBEF4) from its two placement lists and allocates per-object buffers. */
void func_150039E0(s32 level, s32 arg1) {
    s32 i;
    s32 found;
    s32 id;
    s32 loaded;
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
    D_800BE2A0 = D_800BE2A2 = D_800BE2A4 = 0;
    i = func_1502B7F0(&spawns, 2, 0xB, level);
    D_800DBF00 = i / sizeof(ObjectSpawn);
    D_800DBEF0 = D_800DBF00;
    D_800DBF98 = func_1502B7F0(&groupedSpawns, 3, 0xC, level, 2) / sizeof(ObjectSpawn);
    D_800DBEF0 += D_800DBF98;
    D_800DBEF4 = allocate_memory(D_800DBEF0 * sizeof(WorldObject), 1, 2, 0);
    if (D_800DBEF4 == NULL) {
        D_800DBEF0 = 0;
        D_800DBF98 = 0;
        D_800DBF00 = 0;
        return;
    }
    // base[i] is object i's spawn: the first list, then the grouped list
    base = spawns;
    for (i = 0; i < D_800DBEF0; i++) {
        if (i == D_800DBF00) {
            base = &groupedSpawns[-i]; // so that base[i] is groupedSpawns[0]
        }
        found = 0;
        isBehavior22 = FALSE;
        obj = &D_800DBEF4[i];
        obj->x = base[i].x;
        obj->y = base[i].y;
        obj->z = base[i].z;
        obj->rotX = base[i].rotX;
        obj->rotY = base[i].rotY;
        obj->rotZ = base[i].rotZ;
        obj->scaleX = base[i].scaleX;
        obj->scaleY = base[i].scaleY;
        obj->scaleZ = base[i].scaleZ;
        obj->displayList = base[i].displayList;
        obj->state = base[i].state & ~8;
        obj->fadeDistance = 0;
        obj->inactive = base[i].inactive;
        obj->param.data = base[i].data;
        behavior = base[i].behavior.func;
        if (base[i].behavior.index < 0xFFFF) {
            // matching: the (s32) casts keep the compares on a copy of the loaded word
            if ((s32)base[i].behavior.index == 0x28 || (s32)base[i].behavior.index == 0x42) {
                obj->state |= 4;
                setStateBit2 = TRUE;
            }
            obj->update = D_80088C90[base[i].behavior.index].update;
            obj->onContact = D_80088C90[base[i].behavior.index].onContact;
            obj->unk92 = D_80088C90[base[i].behavior.index].unk8;
            if (base[i].behavior.index == 22) {
                isBehavior22 = TRUE;
            }
        } else {
            obj->update = behavior;
            obj->onContact = NULL;
        }
        obj->material = base[i].material;
        obj->work.floor.unk7C = 0;
        obj->work.floor.tri = 0;
        *(s32 *)&obj->work.floor.y = 0; // matching: an integer zero store, not 0.0f
        obj->sound = 0;
        obj->attach = 0;
        obj->effectFlags = 0;
        obj->behaviorState = 4;
        obj->spinX = obj->spinY = obj->spinZ = 0.0f;
        obj->moveX = obj->moveY = obj->moveZ = obj->spinZ; // matching: 0 via spinZ, not a literal
        obj->yOffset = 0;
        obj->alpha = 0xFF;
        obj->viewAlpha[0] = obj->viewAlpha[1] = obj->viewAlpha[2] = obj->viewAlpha[3] = 0xFF;
        obj->index = i;
        obj->shadow = NULL;
        obj->unk98 = 0;
        if (obj->displayList == NULL) {
            id = base[i].modelId;
            for (j = D_800DBF00; j < i; j++) {
                if (base[j].displayList == base[i].displayList && base[j].modelId == base[i].modelId
                    && D_800DBEF4[j].displayList != NULL) {
                    found = j + 1;
                    break;
                }
            }
            if (!found) {
                model = func_1502B6BC(&loaded, 0, 0, 2, 3, id);
                obj->model = model;
            }
            if (found) {
                obj->srcVertices = D_800DBEF4[found - 1].srcVertices;
                obj->displayList = D_800DBEF4[found - 1].displayList;
                obj->unk9C = D_800DBEF4[found - 1].unk9C;
                obj->numVertices = D_800DBEF4[found - 1].numVertices;
                obj->materials = D_800DBEF4[found - 1].materials;
            } else {
                obj->srcVertices = model->vertices;
                obj->displayList = model->displayList;
                obj->unk9C = model->unk8;
                obj->numVertices = (Vtx *)obj->displayList - obj->srcVertices;
                obj->materials = model->materials;
            }
            if (i >= D_800DBF00) {
                obj->unk4C = D_800A26C0[id].unk4C;
                obj->unk4D = D_800A26C0[id].unk4D;
                obj->unk50 = D_800A26C0[id].unk50;
                obj->unk52 = D_800A26C0[id].unk52;
                obj->flags = D_800A26C0[id].flags;
                obj->state = D_800A26C0[id].state & ~8;
                obj->effectFlags = D_800A26C0[id].effectFlags;
                if (setStateBit2) {
                    obj->state |= 4;
                }
                obj->drawDistance = D_800A26C0[id].drawDistance;
                obj->id = base[i].id;
            } else {
                obj->unk50 = base[i].unk50;
                obj->unk52 = base[i].unk52;
                obj->flags = base[i].flags;
                obj->unk4C = base[i].unk4C;
                obj->unk4D = base[i].unk4D;
                obj->drawDistance = base[i].drawDistance;
                if (base[i].id != 0) {
                    obj->id = base[i].id;
                } else {
                    obj->id = func_15004BF0(TRUE);
                }
            }
            if (id == 0x71) {
                obj->material = 0xE;
            }
            if (obj->update != NULL || isBehavior22) {
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
                        && D_800DBEF4[j].displayList != NULL) {
                        found = j + 1;
                        break;
                    }
                }
                if (!found) {
                    id = base[i].modelId;
                    levelModel = D_800B0E50[id].model;
                    obj->numVertices = ((u32)levelModel->displayList - (u32)levelModel->vertices) / sizeof(Vtx);
                    obj->srcVertices = levelModel->vertices;
                    obj->materials = levelModel->materials;
                    obj->unk9C = levelModel->unk8;
                } else {
                    obj->numVertices = D_800DBEF4[found - 1].numVertices;
                    obj->srcVertices = D_800DBEF4[found - 1].srcVertices;
                    obj->materials = D_800DBEF4[found - 1].materials;
                    obj->unk9C = D_800DBEF4[found - 1].unk9C;
                }
                if ((s32)obj->displayList == 2) {
                    obj->viewFlags = 0;
                } else {
                    obj->viewFlags = 0x80;
                }
                if (!found) {
                    obj->displayList = levelModel->displayList;
                } else {
                    obj->displayList = D_800DBEF4[found - 1].displayList;
                }
                obj->effectFlags = base[i].effectFlags;
                obj->model = NULL;
            }
            obj->unk50 = base[i].unk50;
            obj->unk52 = base[i].unk52;
            obj->flags = base[i].flags;
            obj->unk4C = base[i].unk4C;
            obj->unk4D = base[i].unk4D;
            obj->drawDistance = base[i].drawDistance;
            if (base[i].id != 0) {
                obj->id = base[i].id;
            } else {
                obj->id = func_15004BF0(TRUE);
            }
        }
        func_15004AAC(obj, &base[i]);
        if (obj->update == NULL && !isBehavior22) {
            obj->viewFlags |= 0x20;
        }
        if (!found) {
            func_1510CE60(obj->displayList, 0, 1, 0x3F, 0);
            func_15004CE0((DlCommand *)obj->displayList, (u32)model);
        }
    }
    for (i = 0; i < D_800DBEF0; i++) {
        obj = &D_800DBEF4[i];
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
    D_800DBEE0 = allocate_memory(D_800DBEF0 * 0x40, 1, 2, 0);
    D_800DBEE4 = allocate_memory(D_800DBEF0 * 0x40, 1, 2, 0);
    D_800DBEF8 = allocate_memory(D_800DBEF0 * sizeof(s32), 1, 0, 0);
    D_800DBEFC = allocate_memory(D_800DBEF0, 1, 0, 0);
    for (i = 0; i < 4; i++) {
        D_800DBED0[i] = allocate_memory(D_800DBEF0 * 4, 1, 0, 0);
    }
    D_800DBF94 = allocate_memory(D_800DBEF0 * sizeof(s32), 1, 0, 0);
    for (i = 0; i < D_800DBEF0; i++) {
        D_800DBF94[i] = 0;
    }
    for (i = D_800DBF00; i < D_800DBEF0; i++) {
        if (D_800DBEF4[i].id == 0) {
            D_800DBEF4[i].id = func_15004BF0(FALSE);
        }
    }
}

/* Saves the display list and source vertices of object D_800DBF88, unless it is 0xFF (none). */
void func_15004574(void) {
    if (D_800DBF88 != 0xFF) {
        D_800DBF8C = (s32)D_800DBEF4[D_800DBF88].displayList;
        D_800DBF90 = (s32)D_800DBEF4[D_800DBF88].srcVertices;
    }
}

/* Does nothing. */
void func_150045BC(void) {
}

extern MtxF D_800DBF48;
extern void func_151150B0(WorldObject *obj);
extern void func_15116058(WorldObject *obj);
extern void func_15116110(WorldObject *obj);
void func_150A44F0(s32, u16 *, s32);
void func_150A7960(MtxF mf, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);
void func_1510E7A4(s32 *outTri, s16 (*outVerts)[3], f32 *outFloorY, f32 *outPropFloorY, s32 *outFlags,
                   s16 *outType, f32 x, f32 y, f32 z, f32 refY, s32 group, void *obj, f32 minY, f32 maxY);
void func_1510F800(s32);
void func_1511490C(MtxF mf, WorldObject *obj);
void func_15004A4C(void);

/*
 * Prepares the objects for the level: builds each object's matrices for both frames (or, for objects with
 * state bit 0, transforms its vertices into its per-frame copies), then probes the floor under objects
 * with view flag 0x20.
 */
void func_150045C4(s32 arg0) {
    s32 i;
    WorldObject *obj;
    MtxF mtx;
    s32 j;
    s32 floorFlags;
    f32 floorY;
    s32 floorTri;
    f32 x;
    f32 y;
    f32 z;

    func_15004A4C();
    guMtxIdentF(D_800DBF08[0]);
    guMtxIdentF(D_800DBF48);
    for (i = 0; i < D_800DBEF0; i++) {
        obj = &D_800DBEF4[i];
        if (obj->update == NULL || obj->update == func_15116058 || obj->update == func_15116110) {
            obj->attach = 3;
        }
        if (!(obj->state & 1)) {
            func_1511490C(mtx, obj);
            guMtxF2L(mtx, &(*D_80089250[D_800BE9C0])[i]);
            bcopy(&(*D_80089250[D_800BE9C0])[i], &(*D_80089250[!D_800BE9C0])[i], sizeof(Mtx));
        }
        if (obj->update == func_151150B0) {
            obj->update = NULL;
        }
        if ((obj->state & 1) == 1) {
            func_1511490C(mtx, obj);
            for (j = 0; j < obj->numVertices; j++) {
                func_150A7960(mtx, obj->srcVertices[j].v.ob[0], obj->srcVertices[j].v.ob[1],
                              obj->srcVertices[j].v.ob[2], &x, &y, &z);
                obj->vertices[0][j].v.ob[0] = x;
                obj->vertices[0][j].v.ob[1] = y;
                obj->vertices[0][j].v.ob[2] = z;
            }
            bcopy(obj->vertices[0], obj->vertices[1], obj->numVertices * sizeof(Vtx));
        }
        func_1510F800(2);
        if ((obj->viewFlags & 0x80) == 0x80 && (obj->flags & 0x60) != 0x20) {
            D_800D37E0[0] = i;
            func_150A44F0(1, D_800D37E0, 1);
        }
    }
    for (i = 0; i < D_800DBEF0; i++) {
        obj = &D_800DBEF4[i];
        if (obj->inactive == 1) {
            continue;
        }
        if ((obj->viewFlags & 0x20) == 0x20) {
            func_1510E7A4(&floorTri, NULL, &floorY, NULL, &floorFlags, NULL, obj->x, obj->y, obj->z, obj->y,
                          i + 1, NULL, -10000.0f, 20000.0f);
            obj->work.floor.tri = floorTri;
            obj->work.floor.y = floorY;
        }
    }
}
/*
 * Relocates a display list: adds `offset` to the address of each G_VTX (and G_DL) command and
 * `matrixOffset` to the address of each G_MOVEMEM of a matrix.
 */
void func_150049A4(DlCommand *dl, s32 offset, s32 matrixOffset) {
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
void func_15004A4C(void) {
    s32 i;

    for (i = 0; i < D_800DBEF0; i++) {
        D_800DBEF8[i] = 0;
        D_800DBEFC[i] = 0;
    }
}

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

/*
 * Sets the object's bounding radius (unk50, unk52) from its farthest scaled vertex if it has none,
 * and counts in D_800BE2A0/2/4 whether a preset radius is too small, was computed, or is large enough.
 */
void func_15004AAC(WorldObject *obj, ObjectSpawn *spawn) {
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
        scaleZ = obj->scaleZ;
        z = 0.0f; // matching: these dead stores of z and x set IDO's FP register order
        scaleX = obj->scaleX;
        x = 0.0f;
        scaleY = obj->scaleY;
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

        if (obj->unk50 == 0) {
            radius = root;
            obj->unk50 = radius;
            obj->unk52 = radius;
            D_800BE2A2++;
        } else {
            radius = root;
            if (obj->unk50 < radius || obj->unk52 < radius) {
                D_800BE2A0++;
            } else {
                D_800BE2A4++;
            }
        }
    }
}

/*
 * Returns an object id that no object uses yet. With `fromTop` 0, counts up from 1 over the grouped
 * objects (0xFF if all are taken); otherwise counts down from 0xFF - D_800DBF00 over all objects.
 */
s32 func_15004BF0(s32 fromTop) {
    s32 i;
    s32 id;

    // matching: the parameter doubles as the start value so that IDO keeps it in a0
    if (fromTop == 0) {
        id = 1;
        for (i = fromTop = D_800DBF00; i < D_800DBEF0; i++) {
            if (id == D_800DBEF4[i].id) {
                id++;
                i = fromTop - 1; // rescan with the next id
            }
            if (id >= 0x100) {
                id = 0xFF;
                break;
            }
        }
    } else {
        fromTop = 0xFF - D_800DBF00;
        id = fromTop;
        for (i = 0; i < D_800DBEF0; i++) {
            if (id == D_800DBEF4[i].id) {
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
void func_15004CE0(DlCommand *dl, u32 base) {
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
