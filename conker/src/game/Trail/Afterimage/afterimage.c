#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c); conker/trail.h describes it. */
MtxS16 *gBoneTrailMtxs;
u8 D_800DF7C4;
BoneTrail *gBoneTrailLists[2];

#include "functions.h"
#include "variables.h"

static void (*gBoneTrailUpdateProcs[])(BoneTrail *) = { trail_shiftActorHistory, trail_shiftObjectHistory }; /* per-type update: capture a history frame */
static void (*gBoneTrailFreeProcs[])(BoneTrail *) = { trail_freeActorAfterimage, trail_freeObjectAfterimage }; /* per-type free */
static Gfx *(*gBoneTrailDrawProcs[])(Gfx *, BoneTrail *, s16) = { trail_drawActorAfterimage, trail_drawObjectAfterimage }; /* per-type draw */

/*
 * Bone-matrix trails ("afterimages") drawn behind objects. Each trail keeps
 * numFrames past copies of its owner's bone matrices, double-buffered on
 * D_800BE9C0, and draws them with increasing transparency.
 * gBoneTrailLists[0] lists type-0 trails (Actor owners),
 * gBoneTrailLists[1] lists type-1 trails (world object owners, one matrix).
 */

/* Starts (or extends) a type-0 trail behind a game object. */
void trail_startActorAfterimage(Actor *object, s32 duration, s32 maxDist) {
    BoneTrail *trail;
    s32 numBones;
    s32 size;
    s32 pad; // matching: unused slot, moves size's spill to sp+0x20
    MtxS16 *frames;

    for (trail = gBoneTrailLists[0]; trail != NULL; trail = trail->next) {
        if (object == trail->owner.object) {
            if (trail->timer < duration) {
                trail->timer = duration;
            }
            trail->maxDist = maxDist;
            return;
        }
    }

    trail = allocate_memory(sizeof(BoneTrail), 1, 0, 0);
    if (trail != NULL) {
        numBones = actor_getBoneMatrixCount(object->character_type);
        if (numBones == 0) {
            heap_free(trail);
            return;
        }

        size = numBones * 3 * 2 * sizeof(Mtx);
        frames = allocate_memory(size, 1, 1, 1);
        if (frames == NULL) {
            heap_free(trail);
            return;
        }

        trail->type = 0;
        trail->numFrames = 3;
        trail->numFilled = 0;
        trail->alpha = 20;
        trail->maxDist = maxDist;
        trail->timer = duration;
        trail->frames = frames;
        trail->owner.object = object;
        trail->unk14 = 0;
        bzero(frames, size);
        trail_appendBoneTrail(trail, &gBoneTrailLists[0]);
    }
}

/* Starts (or extends) a type-1 trail behind a WorldObject object. */
void trail_startObjectAfterimage(WorldObject *owner, s32 duration, s32 maxDist) {
    BoneTrail *trail;
    MtxS16 *frames;

    for (trail = gBoneTrailLists[1]; trail != NULL; trail = trail->next) {
        if (owner == trail->owner.worldObj) {
            if (trail->timer < duration) {
                trail->timer = duration;
            }
            trail->maxDist = maxDist;
            return;
        }
    }

    trail = allocate_memory(0x14, 1, 0, 0);
    if (trail != NULL) {
        frames = allocate_memory(3 * 2 * sizeof(Mtx), 1, 1, 0);
        if (frames == NULL) {
            heap_free(trail);
            return;
        }

        trail->type = 1;
        trail->numFrames = 3;
        trail->numFilled = 0;
        trail->alpha = 20;
        trail->maxDist = maxDist;
        trail->timer = duration;
        trail->frames = frames;
        trail->owner.worldObj = owner;
        bzero(frames, 3 * 2 * sizeof(Mtx));
        trail_appendBoneTrail(trail, &gBoneTrailLists[1]);
    }
}

/* Appends a trail to the end of a list. */
void trail_appendBoneTrail(BoneTrail *trail, BoneTrail **list) {
    BoneTrail *last;

    trail->next = NULL;
    if (*list != NULL) {
        last = *list;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = trail;
    } else {
        *list = trail;
    }
}

/* Lets an object's type-0 trail start fading out. */
void trail_fadeActorAfterimage(Actor *object) {
    BoneTrail *trail;

    for (trail = gBoneTrailLists[0]; trail != NULL; trail = trail->next) {
        if (object == trail->owner.object) {
            trail->timer = 0;
        }
    }
}

/* Removes and frees an object's type-0 trails immediately. */
void trail_removeActorAfterimages(Actor *object) {
    BoneTrail *trail;
    BoneTrail *prev;
    BoneTrail *next;

    prev = NULL;
    for (trail = gBoneTrailLists[0]; trail != NULL; trail = next) {
        next = trail->next;
        if (object == trail->owner.object) {
            if (prev == NULL) {
                gBoneTrailLists[0] = next;
            } else {
                prev->next = next;
            }
            heap_setTag(trail->frames, 2);
            heap_free(trail);
        } else {
            prev = trail;
        }
    }
}

/* Advances every trail in list `listIndex`: counts down timers, captures history,
 * fades alpha in while the timer runs and out after it expires, freeing the
 * trail once fully faded. */
void trail_updateBoneTrails(s32 listIndex) {
    BoneTrail **head;
    BoneTrail *trail;
    BoneTrail *prev;
    BoneTrail *next;
    s32 alpha;

    head = &gBoneTrailLists[listIndex];
    prev = NULL;
    for (trail = *head; trail != NULL; trail = next) {
        next = trail->next;
        if (trail->timer != 0) {
            trail->timer -= D_800BE9E4;
        }
        if (trail->timer < 0) {
            trail->timer = 0;
        }
        gBoneTrailUpdateProcs[trail->type](trail);

        alpha = trail->alpha;
        if (trail->timer != 0) {
            if (alpha != 255) {
                alpha += D_800BE9E4 * 16;
                if (alpha >= 256) {
                    alpha = 255;
                }
                trail->alpha = alpha;
            }
            prev = trail;
        } else if (alpha != 0) {
            alpha -= D_800BE9E4 * 8;
            if (alpha < 0) {
                alpha = 0;
            }
            if (alpha == 0) {
                if (prev == NULL) {
                    *head = next;
                } else {
                    prev->next = next;
                }
                gBoneTrailFreeProcs[trail->type](trail);
                heap_free(trail);
            } else {
                trail->alpha = alpha;
                prev = trail;
            }
        }
    }
}

/* Draws every trail in list `listIndex`. */
Gfx *trail_drawBoneTrails(Gfx *gfx, s32 listIndex, s16 player) {
    BoneTrail *trail;

    trail = gBoneTrailLists[listIndex];
    if (trail == NULL) {
        return gfx;
    }

    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPSetGeometryMode(gfx++, G_SHADE | G_SHADING_SMOOTH);
    gSPClearGeometryMode(gfx++, G_FOG | G_LOD);
    gDPPipeSync(gfx++);
    gDPSetAlphaCompare(gfx++, G_AC_NONE);
    gDPSetConvert(gfx++, 0, 0, 0, 0, 0, 255);
    for (; trail != NULL; trail = trail->next) {
        gfx = gBoneTrailDrawProcs[trail->type](gfx, trail, player);
    }
    return gfx;
}

/*
 * Type-0 update: shifts the history one frame. Frame i of the current half
 * (D_800BE9C0) takes frame i + 1 of the other half, and the last frame takes
 * the owner's current bone matrices.
 */
void trail_shiftActorHistory(BoneTrail *trail) {
    s32 i;
    s32 numFrames;
    s32 numBones;
    s32 curBase;
    s32 prevBase;
    MtxS16 *src;
    MtxS16 *dst;
    s32 size;

    numFrames = trail->numFrames;
    numBones = actor_getBoneMatrixCount(trail->owner.object->character_type);
    curBase = D_800BE9C0 * (numBones * numFrames);
    prevBase = (D_800BE9C0 ^ 1) * (numBones * numFrames);
    for (i = 0; i < numFrames; i++) {
        size = numBones * sizeof(Mtx);
        if (i == numFrames - 1) {
            dst = (MtxS16 *)trail->owner.object->ob_matrix_ptr;
            if (dst == NULL) {
                trail->numFilled = 0;
                return;
            }
            src = dst; // matching: sharing dst with the NULL-checked pointer steers regalloc
        } else {
            src = &trail->frames[(i + 1) * numBones + prevBase];
        }
        dst = &trail->frames[i * numBones + curBase];
        bcopy(src, dst, size);
    }
    if (trail->numFilled != trail->numFrames) {
        trail->numFilled++;
    }
}


/*
 * Type-1 update: the one-matrix version of trail_shiftActorHistory. The newest frame is
 * the owner's matrix from view 0's draw list; an owner that is not in the list
 * clears the history.
 */
void trail_shiftObjectHistory(BoneTrail *arg) {
    BoneTrail *trail;
    s32 i;
    s32 view;
    s32 numFrames;
    s32 curBase;
    s32 prevBase;
    MtxS16 *ownerMtx;
    MtxS16 *src;
    WorldObject *objects;

    trail = arg; // matching: the copy puts trail in s2 and numFrames in s3
    view = 0;
    numFrames = trail->numFrames;
    curBase = D_800BE9C0 * numFrames;
    prevBase = (D_800BE9C0 ^ 1) * numFrames;
    for (i = 0; i < gViewDrawCounts[view]; i++) {
        objects = db_Object_array;
        if (trail->owner.worldObj == &objects[(*gViewDrawListPtrs[0])[i].index]) {
            break;
        }
    }
    if (i == gViewDrawCounts[view]) {
        trail->numFilled = 0;
        return;
    }
    ownerMtx = (MtxS16 *)&(*gObjectMtxBuffers[D_800BE9C0])[(*gViewDrawListPtrs[0])[i].index];
    for (i = 0; i < numFrames; i++) {
        if (i == numFrames - 1) {
            src = ownerMtx;
        } else {
            src = &trail->frames[i + prevBase + 1];
        }
        bcopy(src, &trail->frames[curBase + i], sizeof(Mtx));
    }
    if (trail->numFilled != trail->numFrames) {
        trail->numFilled++;
    }
}

/* Translation component `c` (0 = x, 1 = y, 2 = z) of a fixed-point matrix. */
#define MTX_TRANSLATION(mtx, c) ((((mtx)->intPart[3][c] << 16) + (mtx)->fracPart[3][c]) * (1.0f / 65536.0f))

/*
 * Limits how far apart consecutive history frames of list `listIndex` may be:
 * when a frame's root bone is more than maxDist from the next newer frame's
 * (or the owner's current one), every bone of the frame is pulled toward the
 * newer frame to that distance.
 */
void trail_limitHistorySpacing(s32 listIndex) {
    BoneTrail *trail;
    s32 numFrames;
    s32 numBones;
    s32 base;
    s32 i;
    s32 j;
    f32 maxDist;
    MtxS16 *cur;
    MtxS16 *newer;
    MtxS16 *bones;
    f32 dist;
    f32 ratio;
    f32 cx;
    f32 cy;
    f32 cz;
    f32 nx;
    f32 ny;
    f32 nz;

    for (trail = gBoneTrailLists[listIndex]; trail != NULL; trail = trail->next) {
        if (trail->maxDist != 0) {
            numFrames = trail->numFrames;
            numBones = actor_getBoneMatrixCount(trail->owner.object->character_type);
            base = D_800BE9C0 * numBones * numFrames;
            maxDist = trail->maxDist;
            for (i = trail->numFilled - 1; i >= 0; i--) {
                cur = &trail->frames[i * numBones + base];
                if (i + 1 == trail->numFilled) {
                    bones = (MtxS16 *)trail->owner.object->ob_matrix_ptr;
                    if (bones == NULL) {
                        trail->numFilled = 0;
                        break;
                    }
                    newer = bones; // matching: a separate NULL-checked temp keeps the branch layout
                } else {
                    newer = &trail->frames[(i + 1) * numBones + base];
                }
                cx = MTX_TRANSLATION(cur, 0);
                cy = MTX_TRANSLATION(cur, 1);
                cz = MTX_TRANSLATION(cur, 2);
                nx = MTX_TRANSLATION(newer, 0);
                ny = MTX_TRANSLATION(newer, 1);
                nz = MTX_TRANSLATION(newer, 2);
                dist = sqrtf((cx - nx) * (cx - nx) + (cy - ny) * (cy - ny) + (cz - nz) * (cz - nz));
                if (maxDist < dist && dist != 0.0f) {
                    ratio = maxDist / dist;
                    for (j = 0; j < numBones; j++) {
                        cx = MTX_TRANSLATION(cur, 0);
                        cy = MTX_TRANSLATION(cur, 1);
                        cz = MTX_TRANSLATION(cur, 2);
                        nx = MTX_TRANSLATION(newer, 0);
                        ny = MTX_TRANSLATION(newer, 1);
                        nz = MTX_TRANSLATION(newer, 2);
                        cx = (cx - nx) * ratio + nx;
                        cy = (cy - ny) * ratio + ny;
                        cz = (cz - nz) * ratio + nz;
                        cur->intPart[3][0] = cx;
                        cur->intPart[3][1] = cy;
                        cur->intPart[3][2] = cz;
                        cur->fracPart[3][0] = 0;
                        cur->fracPart[3][1] = 0;
                        cur->fracPart[3][2] = 0;
                        cur++;
                        newer++;
                    }
                }
            }
        }
    }
}

/* Type-0 free. */
void trail_freeActorAfterimage(BoneTrail *trail) {
    heap_setTag(trail->frames, 2);
}

/* Type-1 free. */
void trail_freeObjectAfterimage(BoneTrail *trail) {
    heap_setTag(trail->frames, 2);
}


/* Type-0 draw: draws the owner object once per history frame, oldest faintest. */
Gfx *trail_drawActorAfterimage(Gfx *gfx, BoneTrail *trail, s16 player) {
    s32 i;
    s32 numBones;
    s32 numFilled;
    s32 base;
    s32 index;
    s32 numFrames;
    s32 alpha;
    s32 hasFrames;
    MtxS16 **bonesOverride;
    s32 alphaScale;
    s32 lod;
    s32 pad[2]; // matching: unused stack slots
    s32 saved;

    hasFrames = trail->numFilled; // matching: loading into a local picks v0 over t6
    if (!hasFrames) {
        return gfx;
    }
    model_loadForActor(trail->owner.object, collision_getLodModelId(trail->owner.object, &lod), lod, 1);
    numFrames = trail->numFrames;
    alpha = trail->alpha;
    numBones = actor_getBoneMatrixCount(trail->owner.object->character_type);
    numFilled = trail->numFilled;
    base = D_800BE9C0 * numBones * numFrames;
    index = trail->owner.object - movement_obj;
    if (index < 0 || index >= 25) {
        return gfx;
    }
    saved = trail->owner.object->unk94;
    trail->owner.object->unk94 = trail->unk14;
    bonesOverride = &gBoneTrailMtxs; // matching: IDO keeps the global's address in a register only via a pointer
    for (i = 0; i < numFilled; i++) {
        alphaScale = ((i + 3) * 32 * alpha) >> 8;
        *bonesOverride = &trail->frames[base + i * numBones];
        D_800DF7C4 = alphaScale;
        gfx = actor_drawModel(gfx, index, player, 4, 1);
    }
    trail->owner.object->unk94 = saved;
    return gfx;
}

/* Type-1 draw: draws the owner world object once per history frame, oldest faintest. */
Gfx *trail_drawObjectAfterimage(Gfx *gfx, BoneTrail *trail, s16 player) {
    s32 i;
    s32 numFilled;
    s32 alpha;
    s32 numFrames;
    s32 base;
    s32 objAlpha;
    s32 pad[2]; // matching: unused stack slots
    s32 saved;

    if (trail->numFilled == 0) {
        return gfx;
    }
    numFilled = trail->numFilled;
    numFrames = trail->numFrames;
    alpha = trail->alpha;
    base = D_800BE9C0 * numFrames;
    saved = trail->owner.worldObj->alpha;
    for (i = 0; i < numFilled; i++) {
        objAlpha = ((i + 3) * 32 * alpha) >> 8;
        trail->owner.worldObj->alpha = objAlpha;
        gfx = gfx_drawObject(gfx, trail->owner.worldObj, (Mtx *)&trail->frames[base + i], player, 0, 0);
    }
    trail->owner.worldObj->alpha = saved;
    return gfx;
}

/* Sets unk14 of an object's type-0 trail. */
void func_151898C0(Actor *object, s32 value) {
    BoneTrail *trail;

    for (trail = gBoneTrailLists[0]; trail != NULL; trail = trail->next) {
        if (object == trail->owner.object) {
            trail->unk14 = value;
            return;
        }
    }
}
