#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Object "parts" effects: an object's skeleton is broken into separately simulated parts (one per
 * bone) that fly off, spin, fall and bounce. gObjPartsStates[objIndex] holds the per-object state.
 */

ObjPartsState gObjPartsStates[25]; /* per object (movement_obj index) */

#define PARTS_STATE_IDLE    0
#define PARTS_STATE_RUNNING 1
#define PARTS_STATE_START   2
#define PARTS_STATE_SETUP   3

/* Parent bone index in gPartParents[kind][part] */
#define PART_PARENT_NONE -1
#define PART_SKIP        -2

typedef struct {
    u32 lo;
    u32 hi;
} PartMasks;

/* Per kind, per part: the parent part (bone), or PART_PARENT_NONE / PART_SKIP. */
static const s8 D_80098890[] = {
    PART_SKIP, PART_SKIP, PART_SKIP, PART_PARENT_NONE, PART_PARENT_NONE, 4, 5, 6, 4, 8, 4, 10, 4,
    4, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, 3, 18, 19, 20, PART_PARENT_NONE, 22, 23,
    PART_PARENT_NONE, 25, 26, PART_PARENT_NONE, 28, 29,
};
static const s8 D_800988B0[] = {
    PART_SKIP, PART_PARENT_NONE, PART_PARENT_NONE, 2, 3, PART_PARENT_NONE, 5, 6, PART_PARENT_NONE,
    8, 9, PART_PARENT_NONE, 11, 12, PART_PARENT_NONE, PART_PARENT_NONE, PART_PARENT_NONE, 16,
    PART_PARENT_NONE, 18, PART_PARENT_NONE, 20, PART_PARENT_NONE, 22, 23, 24, 25,
};
static const s8 D_800988CC[] = {
    PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP,
    PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP,
    PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP, PART_SKIP,
    PART_SKIP, PART_PARENT_NONE, 25, 26, 25, 28, 25, 30, PART_SKIP, PART_SKIP, PART_SKIP,
    PART_SKIP, PART_PARENT_NONE, 36, 37, 36, 39, 36, 41, PART_SKIP, PART_PARENT_NONE,
};
/* The part variants of kind 2 (model_breakIntoParts). */
static const PartMasks D_800988FC[] = {
    { 0xFE000000, 0 },
    { 0, 0x7F0 },
    { 0, 0x1000 },
};
static const u8 gPartCounts[] = { 27, 31, 45 }; /* part count per kind */

/* The per-kind tables. */
static void (*gPartInitFuncs[])(ObjPart *part, s32 objIndex) = { func_1503F078, func_1503F0AC, func_1503F0D8 }; /* per-part init */
static void (*gPartStartFuncs[])(s32 objIndex) = { func_1503EFC4, func_1503F108, func_1503F16C };               /* start */
static void (*gPartUpdateFuncs[])(s32 objIndex) = { func_1503EEC0, func_1503EEC0, func_1503F2B0 };               /* update */
static const s8 *gPartParents[] = { D_800988B0, D_80098890, D_800988CC };                                     /* part parents */
static u8 *D_80084460[] = { NULL, NULL, NULL };                                                             /* per-part flag bits */
static const PartMasks *gPartVariants[] = { NULL, NULL, D_800988FC };                                         /* part variants */

/* Start breaking `obj` into parts; variant -1 selects every part, otherwise gPartVariants[kind][variant]. */
void model_breakIntoParts(Actor *obj, s32 kind, s32 variant) {
    if (variant != -1) {
        func_1503DF0C(obj - movement_obj, kind, gPartVariants[kind][variant].lo, gPartVariants[kind][variant].hi);
        return;
    }
    func_1503DF0C(obj - movement_obj, kind, -1, -1);
}

void func_1503DF0C(s32 objIndex, s32 kind, s32 maskLo, s32 maskHi) {
    gObjPartsStates[objIndex].partMaskLo |= maskLo;
    gObjPartsStates[objIndex].partMaskHi |= maskHi;
    gObjPartsStates[objIndex].kind = kind;
    gObjPartsStates[objIndex].state = PARTS_STATE_START;
}


/* Per-frame driver of an object's parts state machine. */
void model_updateParts(s32 objIndex) {
    ObjPartsState *ps;
    ObjPart *parts;
    ObjPart *part;
    const s8 *parents;
    s32 state;
    s32 count;
    s32 i;

    ps = &gObjPartsStates[objIndex];
    state = ps->state;
    if (state == PARTS_STATE_START) {
        movement_obj[objIndex].unk74 |= 0x80;
        state = PARTS_STATE_SETUP;
        if (movement_obj[objIndex].ob_matrix_ptr == NULL) {
            return;
        }
    }
    if (state == PARTS_STATE_SETUP) {
        count = gPartCounts[ps->kind];
        if (ps->parts == NULL) {
            parts = allocate_memory(count * sizeof(ObjPart), 1, 0, 0);
            if (parts == NULL) {
                return;
            }
            ps->parts = parts;
            for (i = 0; i < count; i++) {
                parts[i].active = FALSE;
            }
        }
        parents = gPartParents[ps->kind];
        for (i = 0; i < count; i++) {
            if (parents[i] == PART_SKIP) {
                continue;
            }
            part = &ps->parts[i];
            if (!model_isPartSelected(i, objIndex)) {
                continue;
            }
            if (part->active) {
                continue;
            }
            model_initPart(objIndex, i, 0, part, 0);
            gPartInitFuncs[ps->kind](part, objIndex);
            part->active = TRUE;
        }
        gPartStartFuncs[ps->kind](objIndex);
        state = PARTS_STATE_RUNNING;
        ps->state = state;
    }
    if (state == PARTS_STATE_RUNNING) {
        gPartUpdateFuncs[ps->kind](objIndex);
        if (ps->state != PARTS_STATE_IDLE) {
            model_moveFreeParts(objIndex);
            model_rebuildBonesFromParts(objIndex);
        }
    }
}

/* Whether part `partIndex` of object `objIndex` is selected in its part masks. */
s32 model_isPartSelected(s32 partIndex, s32 objIndex) {
    if (partIndex < 32) {
        if (gObjPartsStates[objIndex].partMaskLo & (1 << partIndex)) {
            return TRUE;
        }
    } else {
        if (gObjPartsStates[objIndex].partMaskHi & (1 << partIndex)) {
            return TRUE;
        }
    }
    return FALSE;
}


/* Stop an object's parts effect: apply the per-part flag bits, free the parts and clear the busy flag. */
void model_stopParts(s32 objIndex) {
    s32 i;
    u8 *flagBits;
    s32 count;

    if (gObjPartsStates[objIndex].state != PARTS_STATE_IDLE) {
        flagBits = D_80084460[gObjPartsStates[objIndex].kind];
        if (flagBits != NULL) {
            count = gPartCounts[gObjPartsStates[objIndex].kind];
            for (i = 0; i < count; i++) {
                if (flagBits[i] != 0xFF && model_isPartSelected(i, objIndex)) {
                    movement_obj[objIndex].unk94 |= 1 << flagBits[i];
                }
            }
        }
        gObjPartsStates[objIndex].state = PARTS_STATE_IDLE;
        if (gObjPartsStates[objIndex].parts != NULL) {
            func_10004074((s32)gObjPartsStates[objIndex].parts); // matching: the cast keeps the pointer in v1 and copies it to a0; uncast IDO loads it into a0
        }
        gObjPartsStates[objIndex].parts = NULL;
        movement_obj[objIndex].unk74 &= ~0x80;
    }
}

/* Bone `i` of the object's skeleton: obj->ob_matrix_ptr is an array of 4x4 float matrices. */
// matching: a byte offset; (obj)->ob_matrix_ptr[i] changes the code of the callers
#define OBJ_BONE(obj, i) ((f32 (*)[4])((u8 *)(obj)->ob_matrix_ptr + (i) * 64))

/*
 * Initialise `part` from bone (partIndex + boneOffset) of the object's skeleton: its transform relative
 * to the parent bone, and a unit velocity pointing away from the object's centre.
 */
void model_initPart(s32 objIndex, s32 partIndex, s32 boneOffset, ObjPart *part, s32 noParent) {
    s32 pad; // matching: unused stack slot above the matrices
    MtxF local;
    MtxF parentInv;
    Actor *obj;
    s32 parent;
    f32 len;
    f32 dx;
    f32 dy;
    f32 dz;

    obj = &movement_obj[objIndex];
    bcopy(OBJ_BONE(obj, partIndex + boneOffset), local, sizeof(local));
    if (noParent) {
        parent = PART_PARENT_NONE;
    } else {
        parent = gPartParents[gObjPartsStates[objIndex].kind][partIndex];
    }
    if (parent != PART_PARENT_NONE) {
        func_150499A0(OBJ_BONE(obj, parent), parentInv);
        local[1][3] = 0.0f;
        local[0][3] = 0.0f;
        local[2][3] = 0.0f;
        local[3][3] = 1.0f;
        func_150A7A48(local, parentInv, local);
    }
    model_decomposeMatrix(local, &part->initPos[0], &part->initPos[1], &part->initPos[2],
                  &part->initRot[0], &part->initRot[1], &part->initRot[2],
                  &part->initScale[0], &part->initScale[1], &part->initScale[2]);
    part->pos[0] = part->initPos[0];
    part->pos[1] = part->initPos[1];
    part->pos[2] = part->initPos[2];
    part->rot[0] = part->initRot[0];
    part->rot[1] = part->initRot[1];
    part->rot[2] = part->initRot[2];
    part->scale[0] = part->initScale[0];
    part->scale[1] = part->initScale[1];
    part->scale[2] = part->initScale[2];

    dx = part->initPos[0] - obj->xpos;
    dy = (part->pos[1] - obj->ypos) - 30.0f;
    dz = part->pos[2] - obj->zpos;
    len = sqrtf((dx * dx) + (dy * dy) + (dz * dz));
    if (len == 0.0f) {
        len = 1.0f;
    }
    len = 1.0f / len;
    part->vel[0] = dx * len;
    part->vel[1] = dy * len;
    part->vel[2] = dz * len;
}

/* Decompose a 4x4 transform into translation, rotation (degrees) and per-axis scale. */
void model_decomposeMatrix(MtxF mtx, f32 *x, f32 *y, f32 *z, f32 *rotX, f32 *rotY, f32 *rotZ,
                   f32 *scaleX, f32 *scaleY, f32 *scaleZ) {
    MtxF m;
    f32 cross[4];
    s32 i;
    f32 len;
    f32 dot;
    f32 rx;
    f32 ry;
    f32 rz;

    bcopy(mtx, m, sizeof(m));
    *x = m[3][0];
    *y = m[3][1];
    *z = m[3][2];
    for (i = 0; i < 3; i++) {
        len = func_150AD930(m[i]);
        if (len == 0.0f) {
            len = 0.0001f;
        }
        if (i == 0) {
            *scaleX = len;
        } else if (i == 1) {
            *scaleY = len;
        } else {
            *scaleZ = len;
        }
        func_15049148((Vec3f *)m[i], 1.0f / len, (Vec3f *)m[i]);
    }
    /* A mirrored basis: flip the scale signs and axes back to a rotation. */
    func_150AD8B0(m[1], m[2], cross);
    dot = func_150AD900(m[0], cross);
    if (dot < 0.0f) {
        *scaleX = -*scaleX;
        *scaleY = -*scaleY;
        *scaleZ = -*scaleZ;
        for (i = 0; i < 3; i++) {
            m[i][0] = -m[i][0];
            m[i][1] = -m[i][1];
            m[i][2] = -m[i][2];
        }
    }
    ry = func_150487E0(-m[0][2]);
    if (func_150AD780(ry) != 0.0f) {
        rx = func_150484A0(m[1][2], m[2][2]);
        rz = func_150484A0(m[0][1], m[0][0]);
    } else {
        rx = func_150484A0(m[1][0], m[1][1]);
        rz = 0.0f;
    }
    *rotX = rx * 57.29577637f;  /* 180 / pi */
    *rotY = ry * 57.29577637f;
    *rotZ = rz * 57.29577637f;
}

/* Rebuild the object's bone matrices from its simulated parts. */
void model_rebuildBonesFromParts(s32 objIndex) {
    s32 count;
    const s8 *parents;
    s32 i;
    s32 parent;
    f32 (*bone)[4];
    MtxF m;
    ObjPart *part;

    if (movement_obj[objIndex].ob_matrix_ptr != NULL) {
        count = gPartCounts[gObjPartsStates[objIndex].kind];
        parents = gPartParents[gObjPartsStates[objIndex].kind];
        for (i = 0; i < count; i++) {
            if (parents[i] == PART_SKIP) {
                continue;
            }
            part = &gObjPartsStates[objIndex].parts[i];
            if (!part->active) {
                continue;
            }
            bone = OBJ_BONE(&movement_obj[objIndex], i);
            parent = gPartParents[gObjPartsStates[objIndex].kind][i];
            if (parent != PART_PARENT_NONE) {
                /* bone = scale * rotation * (translation * parent bone) */
                func_150A7DA0(m, part->pos[0], part->pos[1], part->pos[2]);
                func_150A7A48(m, OBJ_BONE(&movement_obj[objIndex], parent), m);
                part = &gObjPartsStates[objIndex].parts[i];
                func_150A8050(bone, part->rot[0], part->rot[1], part->rot[2]);
                func_150A7A48(bone, m, bone);
                part = &gObjPartsStates[objIndex].parts[i];
                func_150A7CB0(m, part->scale[0], part->scale[1], part->scale[2]);
                func_150A7A48(m, bone, bone);
            } else {
                func_150A8050(bone, part->rot[0], part->rot[1], part->rot[2]);
                part = &gObjPartsStates[objIndex].parts[i];
                func_15043EC8(bone, part->scale[0], part->scale[1], part->scale[2],
                              part->pos[0], part->pos[1], part->pos[2]);
            }
        }
    }
}

/* Move the free (parentless) parts by their velocities and apply gravity. */
void model_moveFreeParts(s32 objIndex) {
    ObjPart *part;
    s32 count;
    s32 i;
    f32 dt;

    count = gPartCounts[gObjPartsStates[objIndex].kind];
    dt = D_800BE9E4;
    for (i = 0; i < count; i++) {
        if (!gObjPartsStates[objIndex].parts[i].active) {
            continue;
        }
        if (gPartParents[gObjPartsStates[objIndex].kind][i] != PART_PARENT_NONE) {
            continue;
        }
        part = &gObjPartsStates[objIndex].parts[i];
        part->pos[0] += part->vel[0] * dt;
        part->pos[1] += part->vel[1] * dt;
        part->pos[2] += part->vel[2] * dt;
        part->rot[0] += part->rotVel[0] * dt;
        part->rot[1] += part->rotVel[1] * dt;
        part->rot[2] += part->rotVel[2] * dt;
        part->vel[1] += part->gravity * dt;
    }
}

/* The ROM has 8 bytes of `jr ra; nop` between model_moveFreeParts and model_randomizePart; splat has
 * no symbol for them so they fold into model_moveFreeParts's size. They are an empty leaf
 * function (same shape as game/Swarm/SwarmCentre/swarmcentre.c func_15178750_pad). */
static void func_1503EB70(void) {
}

/*
 * Randomise a freshly initialised part: scale its outward velocity by a random factor per axis
 * (spreadXZ on x/z, spreadY on y; `strong` picks a factor from the upper half of the range),
 * give it a random spin and gravity.
 */
void model_randomizePart(ObjPart *part, f32 spreadXZ, f32 spreadY, s32 strong) {
    s32 i;
    f32 spread[3];
    s32 mask;
    s32 base;

    if (strong) {
        mask = 0x80;
        base = 0x7F;
    } else {
        mask = 0xFF;
        base = 0;
    }
    spreadXZ *= 0.02745098062f;
    spreadY *= 0.02745098062f;
    spread[0] = spread[2] = spreadXZ;
    spread[1] = spreadY;
    for (i = 0; i < 3; i++) {
        part->vel[i] *= ((func_150ADA20_u8() & mask) + base) * spread[i] + 2.0f;
    }
    for (i = 0; i < 3; i++) {
        part->rotVel[i] = ((func_150ADA20_u8() & 0xFF) - 128) * 0.03125f;
    }
    part->gravity = -0.3f;
}


/* Bounce falling free parts off the floor below them; slow ones come to rest. */
void model_bounceParts(s32 objIndex) {
    ObjPartsState *ps;
    ObjPart *part;
    f32 floorY;
    f32 velY;
    s32 i;
    s32 count;

    collision_selectMap(0);
    ps = &gObjPartsStates[objIndex];
    count = gPartCounts[ps->kind];
    for (i = 0; i < count; i++) {
        if (ps->parts[i].active) {
            part = &ps->parts[i];
            if (gPartParents[ps->kind][i] == PART_PARENT_NONE && part->gravity != 0.0f) {
                floorY = collision_findMeshFloorNear(part->pos[0], part->pos[1], part->pos[2], 0) + 5;
                if (part->pos[1] < floorY) {
                    velY = part->vel[1];
                    part->pos[1] = floorY;
                    if (velY < -4.0f) {
                        part->vel[0] *= 0.8f;
                        part->vel[1] = velY * -0.5f;
                        part->vel[2] *= 0.8f;
                        part->rotVel[0] *= -1.0f;
                        part->rotVel[1] *= 0.4f;
                        part->rotVel[2] *= -1.0f;
                    } else {
                        part->rotVel[2] = 0.0f;
                        part->rotVel[1] = 0.0f;
                        part->rotVel[0] = 0.0f;
                        part->gravity = 0.0f;
                        part->vel[2] = 0.0f;
                        part->vel[1] = 0.0f;
                        part->vel[0] = 0.0f;
                    }
                }
            }
        }
    }
}

void func_1503EEB8(void) {
}

/* Update for kinds 0 and 1: bounce the parts until the timer runs out, then call actor_remove. */
void func_1503EEC0(s32 objIndex) {
    ObjPartsState *ps;
    s32 timer;

    model_bounceParts(objIndex);
    ps = &gObjPartsStates[objIndex];
    timer = ps->timer;
    timer -= D_800BE9E4;
    ps->timer = timer;
    if (timer <= 0) {
        actor_remove(&movement_obj[objIndex], 1);
    }
}

/* Whether the object's selected parts overlap each non-empty mask word of gPartVariants[kind][variant]. */
s32 func_1503EF4C(s32 kind, s32 variant, s32 objIndex) {
    const PartMasks *row;
    PartMasks *masks;

    row = gPartVariants[kind];
    // matching: byte arithmetic gives `addu row, offset` (&row[variant] swaps the operands)
    masks = (PartMasks *)((u8 *)row + variant * sizeof(PartMasks));
    // matching: `(base + i)->field` keeps the `and` operand order of the target
    if (masks->lo != 0 && !(masks->lo & (gObjPartsStates + objIndex)->partMaskLo)) {
        goto fail;
    }
    if (masks->hi != 0 && !(masks->hi & (gObjPartsStates + objIndex)->partMaskHi)) {
        goto fail;
    }
    return TRUE;
fail: // matching: one shared `return FALSE` block
    return FALSE;
}

/* Start for kind 0: 120-tick timer and a random vertical kick for every part. */
void func_1503EFC4(s32 objIndex) {
    ObjPartsState *ps = &gObjPartsStates[objIndex];
    s32 i;
    s32 count;

    ps->timer = 120;
    count = gPartCounts[ps->kind];
    for (i = 0; i < count; i++) {
        ps->parts[i].vel[1] = (s32)(func_150ADA20_u8() % 20u) - 5;
    }
}

/* Per-part init for kinds 0, 1 and 2 (gPartInitFuncs). */
void func_1503F078(ObjPart *part, s32 objIndex) {
    model_randomizePart(part, 2.0f, 2.0f, FALSE);
}

void func_1503F0AC(ObjPart *part, s32 objIndex) {
    model_randomizePart(part, 1.0f, 2.0f, TRUE);
}

void func_1503F0D8(ObjPart *part, s32 objIndex) {
    model_randomizePart(part, 2.06f, 3.0f, TRUE);
}

/* Start for kind 1: 140-tick timer; sets the object's unk94 to 6 and kicks part 4 upwards. */
void func_1503F108(s32 objIndex) {
    ObjPartsState *ps = &gObjPartsStates[objIndex];

    ps->timer = 140;
    movement_obj[objIndex].unk94 = 6;
    ps->parts[4].vel[1] = 10.0f;
}

/* Start for kind 2: 300-tick timer; updates unk94 flags for the selected variants 0..2. */
void func_1503F16C(s32 objIndex) {
    gObjPartsStates[objIndex].timer = 300;
    if (func_1503EF4C(2, 0, objIndex)) {
        movement_obj[objIndex].unk94 |= 0x40;
        movement_obj[objIndex].unk94 &= ~0x200;
    }
    if (func_1503EF4C(2, 1, objIndex)) {
        movement_obj[objIndex].unk94 |= 0x80;
        movement_obj[objIndex].unk94 &= ~0x100;
    }
    if (func_1503EF4C(2, 2, objIndex)) {
        movement_obj[objIndex].unk94 &= ~0x400;
    }
}

/* Update for kind 2: bounce the parts; when the timer runs out stop the effect and set unk94 flags. */
void func_1503F2B0(s32 objIndex) {
    ObjPartsState *ps;

    model_bounceParts(objIndex);
    ps = &gObjPartsStates[objIndex];
    ps->timer -= D_800BE9E4;
    if (ps->timer > 0) {
        return;
    }
    model_stopParts(objIndex);
    if (func_1503EF4C(2, 0, objIndex)) {
        movement_obj[objIndex].unk94 |= 8;
    }
    if (func_1503EF4C(2, 1, objIndex)) {
        movement_obj[objIndex].unk94 |= 4;
    }
    if (func_1503EF4C(2, 2, objIndex)) {
        movement_obj[objIndex].unk94 |= 2;
    }
}


/* model_decomposeMatrix on an Mtx, which is fixed point when D_800C3E90 is set and plain floats otherwise. */
/* mtx: a bone matrix, fixed point (Mtx) when D_800C3E90 is set, else float (MtxF) */
void model_decomposeBoneMatrix(void *mtx, f32 *x, f32 *y, f32 *z, f32 *rotX, f32 *rotY, f32 *rotZ,
                   f32 *scaleX, f32 *scaleY, f32 *scaleZ) {
    MtxF m;

    if (D_800C3E90) {
        guMtxL2F(m, mtx);
    } else {
        memcpy(m, mtx, sizeof(m));
    }
    model_decomposeMatrix(m, x, y, z, rotX, rotY, rotZ, scaleX, scaleY, scaleZ);
}
