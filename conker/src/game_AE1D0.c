#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Maps a model id to the hit-sphere table (D_80086C60 / D_8009CBCC index) it uses. */
typedef struct {
    /* 0x0 */ u16 model;
    /* 0x2 */ u8  sphereTable;
    /* 0x3 */ u8  pad3;
} ModelHitEntry; /* size 0x4 */

extern ModelHitEntry D_8009BD30[23];


/* One hit sphere bound to a bone of a model. */
typedef struct {
    /* 0x0 */ u8  bone;
    /* 0x1 */ u8  part;
    /* 0x2 */ u16 radius;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ s16 z;
} HitSphere; /* size 0xA */

/* struct124 (include/structs.h) has the wrong types at 0xE/0x10: the game reads a u16 at
 * 0xE (golden `lhu $t2,0xE($t1)` + the bgez/0x4F800000 unsigned-to-float fixup) and an s16
 * at 0x10 (golden `lh $t6,0x10($t5)`), not an s16 and an s32.  Corrected file-locally. */
typedef struct {
    /* 0x00 */ u8  pad0[0xE];
    /* 0x0E */ u16 body_radius;
    /* 0x10 */ s16 body_height;
} ModelBounds;

extern HitSphere *D_80086C60[];
extern u8 D_8009CBCC[];

/* One 4x4 matrix per model bone (struct127.unk1D4 points at an array of them). */
typedef f32 BoneMtx[4][4];

void func_15143134(struct17 *in, struct17 *out, BoneMtx *mtx);
void func_1505D1C4(f32, f32, f32, s32, s32, s32, s32, s32);

/* Melee hit test: takes the point `dist` ahead of `self` (rotated by its pitch unkB8 and yaw
 * unk40), rejects it against obj's whole-body sphere, then tests it against each of obj's hit
 * spheres (table `type`) grown by `radius`.  On a hit, spawns the impact effect (unless obj is
 * immune) and returns bone + 1; returns 0 for a miss. */
s32 func_15080D20(struct127 *self, struct127 *obj, s32 type, f32 dist, f32 radius, s32 arg5) {
    f32 scale;
    f32 angle;
    f32 s;
    f32 c;
    f32 x;
    f32 height;
    f32 r;
    f32 dx;
    f32 dy;
    f32 pos[3];
    f32 dz;
    ModelBounds *bounds;
    s32 count;
    BoneMtx *bones;
    struct17 local;
    struct17 world;
    HitSphere *p;
    s32 i;
    HitSphere *spheres;
    s32 id;

    scale = self->xz_scale;
    dist *= scale;
    radius *= scale;
    angle = self->unkB8 * 0.01745329238f;
    s = sinf(angle);
    c = cosf(angle);
    pos[0] = 0.0f;
    pos[1] = -dist * s;
    pos[2] = dist * c;
    angle = self->unk40 * 0.01745329238f;
    s = sinf(angle);
    c = cosf(angle);
    x = pos[0] * c + pos[2] * s;
    pos[2] = -pos[0] * s + pos[2] * c;
    pos[0] = x;
    pos[0] += self->x_position;
    pos[1] += self->y_position;
    pos[2] += self->z_position;
    id = obj->id;
    bounds = (ModelBounds *)D_800D1C90[id];
    // matching: the body radius reuses `angle`; a separate local shifts IDO's register allocation
    angle = bounds->body_radius * obj->xz_scale;
    height = bounds->body_height * obj->y_scale;
    dx = pos[0] - obj->x_position;
    dy = pos[1] - (obj->y_position + height);
    dz = pos[2] - obj->z_position;
    if (angle * angle < dx * dx + dy * dy + dz * dz) {
        return 0;
    }
    spheres = D_80086C60[type];
    count = D_8009CBCC[type];
    i = 0;
    bones = (BoneMtx *)obj->unk1D4;
    if (count > 0) {
        // matching: p = spheres in its own block; a plain assignment changes IDO's s-register allocation
        do { p = spheres; } while (0);
        do {
            local.unk0 = p->x;
            local.unk4 = p->y;
            local.unk8 = p->z;
            // matching: byte offset keeps IDO's base+offset addu order (&bones[i] emits offset+base)
            func_15143134(&local, &world, (BoneMtx *)((u8 *)bones + p->bone * sizeof(BoneMtx)));
            dx = pos[0] - world.unk0;
            dy = pos[1] - world.unk4;
            dz = pos[2] - world.unk8;
            i++;
            r = p->radius * obj->xz_scale + radius;
            if (!(r * r < dx * dx + dy * dy + dz * dz)) {
                if (obj->immune == 0) {
                    func_1505D1C4(obj->x_position, obj->y_position, obj->z_position, arg5, self - D_800CC2D0, 0, 0, 0);
                }
                return p->bone + 1;
            }
            p++;
        } while (i != count);
    }
    return 0;
}

/* Returns the index of obj's model in the D_8009BD30 hit table, or -1 if it has none. */
s32 func_1508108C(struct127 *obj) {
    s32 i;

    for (i = 0; i < 23; i++) {
        if (obj->id == D_8009BD30[i].model) {
            return i;
        }
    }
    return -1;
}


s32 func_151452C4(struct17 *arg0, struct17 *arg1, struct17 *arg2, f32 arg3, struct17 *arg4, struct17 *arg5, f32 *arg6, f32 *arg7);

/* Ray vs. per-bone hit-sphere test for one actor.  Broad phase: build the whole-object
 * bounding sphere from D_800D1C90[obj->id] and ray-test it; if that hits, walk the model's
 * hit-sphere table, skipping bones masked off in obj->unk9C, transform each sphere's local
 * offset by its bone matrix and keep the nearest / highest-priority hit.
 *
 * ORIGINAL-GAME BUG, reproduced deliberately: the tail does `*outDist = tNear;` after the
 * loop, overwriting the tracked best distance with the LAST call's near-t rather than the
 * best one.  Golden does exactly this (0x15081460..0x15081484); it is not to be "fixed".
 */
s32 func_1508114C(struct127 *obj, struct17 *rayOrigin, struct17 *rayDir, struct17 *outNear, struct17 *outFar, struct17 *outCentre, f32 *outDist, s32 *outIndex, s32 *outBone, s32 *outPart, s16 *outOffset) {
    s32 idx;
    HitSphere *spheres;
    s32 count;
    s32 type;
    s32 bestIndex;
    f32 tNear;
    f32 tFar;
    s32 i;
    struct17 centre;
    struct17 bestCentre;
    struct17 near;
    struct17 far;
    struct17 local;
    struct17 bestNear;
    struct17 bestFar;
    s32 bestPart;
    HitSphere *p;
    BoneMtx *bones;
    f32 radius;

    if (obj->unk1D4 == NULL) {
        return -1;
    }
    idx = func_1508108C(obj);
    if (idx == -1) {
        return -1;
    }
    type = D_8009BD30[idx].sphereTable;
    spheres = D_80086C60[type];
    count = D_8009CBCC[type];
    *outDist = -1.0f;
    centre.unk0 = obj->x_position;
    centre.unk4 = (((ModelBounds *)D_800D1C90[obj->id])->body_height * obj->y_scale) + obj->y_position;
    centre.unk8 = obj->z_position;
    radius = ((ModelBounds *)D_800D1C90[obj->id])->body_radius * obj->y_scale;
    if (func_151452C4(rayOrigin, rayDir, &centre, radius, &near, &far, &tNear, &tFar) == 0) {
        return 0;
    }
    bestPart = -1;
    i = 0;
    if (count > 0) {
        p = spheres;
        do {
            if (((1 << p->bone) & obj->unk9C) != 0) {
                goto next;
            }
            bones = (BoneMtx *)obj->unk1D4;
            local.unk0 = p->x;
            local.unk4 = p->y;
            local.unk8 = p->z;
            // matching: byte offset keeps IDO's base+offset addu order (&bones[i] emits offset+base)
            func_15143134(&local, &centre, (BoneMtx *)((u8 *)bones + p->bone * sizeof(BoneMtx)));
            if (func_151452C4(rayOrigin, rayDir, &centre, p->radius * obj->xz_scale, &near, &far, &tNear, &tFar) != 0) {
                if ((*outDist == -1.0f) || ((tNear < *outDist) && (p->part >= bestPart))) {
                    *outDist = tNear;
                    bestNear = near;
                    bestFar = far;
                    bestCentre = centre;
                    bestIndex = i;
                    bestPart = p->part;
                }
            }
        next:
            i++;
            p++;
        } while (i != count);
    }
    if (*outDist == -1.0f) {
        return 0;
    }
    *outDist = tNear;
    *outIndex = bestIndex;
    *outPart = spheres[bestIndex].part;
    *outBone = spheres[bestIndex].bone;
    outOffset[0] = spheres[bestIndex].x;
    outOffset[1] = spheres[bestIndex].y;
    outOffset[2] = spheres[bestIndex].z;
    *outNear = bestNear;
    *outFar = bestFar;
    if (outCentre != NULL) {
        *outCentre = bestCentre;
    }
    return 1;
}


s32 func_15080D20(struct127 *, struct127 *, s32, f32, f32, s32);

/* Finds the first live object of the given model whose hit spheres the point `dist` ahead of
 * `self` (with reach `radius`) touches.  Stores it in *outTarget and returns bone + 1, or 0. */
/* include/functions.h declares outTarget (really a struct127 **) as s32. */
s32 func_15081574(s32 self, f32 dist, f32 radius, s32 outTarget, s32 arg4, s32 model) {
    s32 i;
    struct127 *obj;
    s32 idx;
    s32 bone;

    *(struct127 **)outTarget = NULL;
    for (i = 0; i < 25; i++) {
        obj = &D_800CC2D0[i];
        if (obj->interaction_state == 0 || obj->unk1D4 == NULL || model != obj->id) {
            continue;
        }
        idx = func_1508108C(obj);
        if (idx == -1) {
            continue;
        }
        bone = func_15080D20((struct127 *)self, obj, D_8009BD30[idx].sphereTable, dist, radius, arg4);
        if (bone != 0) {
            *(struct127 **)outTarget = obj;
            return bone;
        }
    }
    return 0;
}
