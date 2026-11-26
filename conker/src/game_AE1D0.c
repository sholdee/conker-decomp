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

extern HitSphere *D_80086C60[];
extern u8 D_8009CBCC[];

/* Point vs. hit-sphere test: takes the point `dist` ahead of `self` (rotated by its pitch unkB8
 * and yaw unk40), rejects it against obj's whole-body sphere, then tests it against each of obj's
 * hit spheres (table `type`) grown by `radius`.  On a hit, calls func_1505D1C4 at obj's position
 * (unless obj is immune) and returns bone + 1; returns 0 for a miss. */
s32 func_15080D20(Actor *self, Actor *obj, s32 type, f32 dist, f32 radius, s32 arg5) {
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
    ModelDef *bounds;
    s32 count;
    MtxF *bones;
    Vec3f local;
    Vec3f world;
    HitSphere *p;
    s32 i;
    HitSphere *spheres;
    s32 id;

    scale = self->scale_xz;
    dist *= scale;
    radius *= scale;
    angle = self->vtilt * 0.01745329238f;
    s = sinf(angle);
    c = cosf(angle);
    pos[0] = 0.0f;
    pos[1] = -dist * s;
    pos[2] = dist * c;
    angle = self->theta * 0.01745329238f;
    s = sinf(angle);
    c = cosf(angle);
    x = pos[0] * c + pos[2] * s;
    pos[2] = -pos[0] * s + pos[2] * c;
    pos[0] = x;
    pos[0] += self->xpos;
    pos[1] += self->ypos;
    pos[2] += self->zpos;
    id = obj->character_type;
    bounds = D_800D1C90[id];
    // matching: the body radius reuses `angle`; a separate local shifts IDO's register allocation
    angle = bounds->bodyRadius * obj->scale_xz;
    height = bounds->bodyHeight * obj->scale_y;
    dx = pos[0] - obj->xpos;
    dy = pos[1] - (obj->ypos + height);
    dz = pos[2] - obj->zpos;
    if (angle * angle < dx * dx + dy * dy + dz * dz) {
        return 0;
    }
    spheres = D_80086C60[type];
    count = D_8009CBCC[type];
    i = 0;
    bones = obj->ob_matrix_ptr;
    if (count > 0) {
        // matching: p = spheres in its own block; a plain assignment changes IDO's s-register allocation
        do { p = spheres; } while (0);
        do {
            local.x = p->x;
            local.y = p->y;
            local.z = p->z;
            // matching: byte offset keeps IDO's base+offset addu order (&bones[i] emits offset+base)
            func_15143134(&local, &world, (MtxF *)((u8 *)bones + p->bone * sizeof(MtxF)));
            dx = pos[0] - world.x;
            dy = pos[1] - world.y;
            dz = pos[2] - world.z;
            i++;
            r = p->radius * obj->scale_xz + radius;
            if (!(r * r < dx * dx + dy * dy + dz * dz)) {
                if (obj->imune == 0) {
                    func_1505D1C4(obj->xpos, obj->ypos, obj->zpos, arg5, self - movement_obj, 0, 0, 0);
                }
                return p->bone + 1;
            }
            p++;
        } while (i != count);
    }
    return 0;
}

/* Returns the index of obj's model in the D_8009BD30 hit table, or -1 if it has none. */
s32 func_1508108C(Actor *obj) {
    s32 i;

    for (i = 0; i < 23; i++) {
        if (obj->character_type == D_8009BD30[i].model) {
            return i;
        }
    }
    return -1;
}

/* Ray vs. per-bone hit-sphere test for one actor.  Broad phase: build the whole-object
 * bounding sphere from D_800D1C90[obj->id] and ray-test it; if that hits, walk the model's
 * hit-sphere table, skipping bones masked off in obj->unk9C, transform each sphere's local
 * offset by its bone matrix and keep the nearest / highest-priority hit.
 *
 * ORIGINAL-GAME BUG, reproduced deliberately: the tail does `*outDist = tNear;` after the
 * loop, overwriting the tracked best distance with the LAST call's near-t rather than the
 * best one.  Golden does exactly this (0x15081460..0x15081484); it is not to be "fixed".
 */
s32 func_1508114C(Actor *obj, Vec3f *rayOrigin, Vec3f *rayDir, Vec3f *outNear, Vec3f *outFar, Vec3f *outCentre, f32 *outDist, s32 *outIndex, s32 *outBone, s32 *outPart, s16 *outOffset) {
    s32 idx;
    HitSphere *spheres;
    s32 count;
    s32 type;
    s32 bestIndex;
    f32 tNear;
    f32 tFar;
    s32 i;
    Vec3f centre;
    Vec3f bestCentre;
    Vec3f near;
    Vec3f far;
    Vec3f local;
    Vec3f bestNear;
    Vec3f bestFar;
    s32 bestPart;
    HitSphere *p;
    MtxF *bones;
    f32 radius;

    if (obj->ob_matrix_ptr == NULL) {
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
    centre.x = obj->xpos;
    centre.y = D_800D1C90[obj->character_type]->bodyHeight * obj->scale_y + obj->ypos;
    centre.z = obj->zpos;
    radius = D_800D1C90[obj->character_type]->bodyRadius * obj->scale_y;
    if (func_151452C4(rayOrigin, rayDir, &centre, radius, &near, &far, &tNear, &tFar) == 0) {
        return 0;
    }
    bestPart = -1;
    i = 0;
    if (count > 0) {
        p = spheres;
        do {
            // matching: empty then-branch; `if (!(mask))` makes IDO rotate the s-registers
            if ((1 << p->bone) & obj->unk9C) {
                /* bone masked off in obj->unk9C */
            } else {
                bones = obj->ob_matrix_ptr;
                local.x = p->x;
                local.y = p->y;
                local.z = p->z;
                // matching: byte offset keeps IDO's base+offset addu order (&bones[i] emits offset+base)
                func_15143134(&local, &centre, (MtxF *)((u8 *)bones + p->bone * sizeof(MtxF)));
                if (func_151452C4(rayOrigin, rayDir, &centre, p->radius * obj->scale_xz, &near, &far, &tNear, &tFar) != 0) {
                    if (*outDist == -1.0f || (tNear < *outDist && p->part >= bestPart)) {
                        *outDist = tNear;
                        bestNear = near;
                        bestFar = far;
                        bestCentre = centre;
                        bestIndex = i;
                        bestPart = p->part;
                    }
                }
            }
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


/* Finds the first live object of the given model whose hit spheres the point `dist` ahead of
 * `self` (with reach `radius`) touches.  Stores it in *outTarget and returns bone + 1, or 0. */
s32 func_15081574(Actor *self, f32 dist, f32 radius, Actor **outTarget, s32 arg4, s32 model) {
    s32 i;
    Actor *obj;
    s32 idx;
    s32 bone;

    *outTarget = NULL;
    for (i = 0; i < 25; i++) {
        obj = &movement_obj[i];
        if (obj->type == 0 || obj->ob_matrix_ptr == NULL || model != obj->character_type) {
            continue;
        }
        idx = func_1508108C(obj);
        if (idx == -1) {
            continue;
        }
        bone = func_15080D20(self, obj, D_8009BD30[idx].sphereTable, dist, radius, arg4);
        if (bone != 0) {
            *outTarget = obj;
            return bone;
        }
    }
    return 0;
}
