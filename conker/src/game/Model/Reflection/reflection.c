#include <ultra64.h>
#include "functions.h"
#include "variables.h"

static u8 gReflectionCount;                 // number of queued reflections
static ReflectionEntry gReflections[15]; // reflection queue

/* Clears the reflection queue. */
void model_clearReflections(void) {
    gReflectionCount = 0;
}

/*
 * Mirrors `count` matrices from `srcMtx` into `dstMtx` and sets the entry's alpha from the distance
 * to the mirror. Modes 0 and 1 mirror in the horizontal plane y = planeY; mode 2 mirrors in the
 * vertical plane through (planeX, planeZ) at `angle`. `srcIsFixed`: the sources are fixed point
 * (Mtx); `dstToFixed`: convert the results to fixed point.
 */
void model_mirrorMatrices(Actor *obj, ReflectionEntry *entry, s32 mode, MtxF *srcMtx,
                   MtxF *dstMtx, s32 count, f32 planeX, f32 planeY, f32 planeZ, f32 unused,
                   f32 angle, f32 objX, f32 objY, f32 objZ, f32 fadeStart, f32 fadeEnd, f32 maxAlpha,
                   s32 srcIsFixed, s32 dstToFixed) {
    MtxF flip;
    MtxF rotA;
    MtxF rotB;
    s32 i;
    f32 (*src)[4];
    f32 dist;
    MtxF temp;
    MtxF *savedMtx;

    guMtxIdentF(flip);
    if (mode == 1 || mode == 0) {
        flip[1][1] = -1.0f;
        // matching: both loops need the byte offset (i << 6) spelled out; srcMtx[i] is CSE'd differently
        for (i = 0; i < count; i++) {
            if (srcIsFixed) {
                guMtxL2F(temp, (Mtx *)((i << 6) + (u8 *)srcMtx));
                src = temp;
            } else {
                srcMtx[i][0][3] = 0.0f;
                srcMtx[i][1][3] = 0.0f;
                srcMtx[i][2][3] = 0.0f;
                srcMtx[i][3][3] = 1.0f;
                src = (void *)((i << 6) + (u8 *)srcMtx);
            }
            func_150A7A48(src, flip, dstMtx[i]);
            dstMtx[i][3][1] = src[3][1] - 2.0f * (src[3][1] - planeY);
            if (dstToFixed) {
                bcopy((i << 6) + (u8 *)dstMtx, temp, sizeof(temp));
                guMtxF2L2(temp, (Mtx *)dstMtx + i);
            }
        }
        dist = objY - planeY;
        if (fadeEnd < dist) {
            entry->alpha = 0;
        } else if (dist < fadeStart) {
            if (mode == 0) {
                if (dist < 10.0f) {
                    entry->alpha = maxAlpha / 10.0f * dist;
                } else {
                    entry->alpha = maxAlpha;
                }
            } else {
                entry->alpha = maxAlpha;
            }
        } else {
            dist -= fadeStart;
            dist = maxAlpha * dist / (fadeEnd - fadeStart);
            dist = maxAlpha - dist;
            entry->alpha = dist;
        }
    } else if (mode == 2) {
        flip[0][0] = -1.0f;
        func_150A8050(rotA, 0.0f, -angle, 0.0f);
        func_150A8050(rotB, 0.0f, angle, 0.0f);
        for (i = 0; i < count; i++) {
            if (srcIsFixed) {
                guMtxL2F(temp, (Mtx *)((i << 6) + (u8 *)srcMtx));
                src = temp;
            } else {
                srcMtx[i][0][3] = 0.0f;
                srcMtx[i][1][3] = 0.0f;
                srcMtx[i][2][3] = 0.0f;
                srcMtx[i][3][3] = 1.0f;
                src = (void *)((i << 6) + (u8 *)srcMtx);
            }
            bcopy(src, dstMtx[i], sizeof(temp));
            dstMtx[i][3][0] -= planeX;
            dstMtx[i][3][1] -= planeY;
            dstMtx[i][3][2] -= planeZ;
            func_150A7A48(dstMtx[i], rotA, dstMtx[i]);
            func_150A7A48(dstMtx[i], flip, dstMtx[i]);
            func_150A7A48(dstMtx[i], rotB, dstMtx[i]);
            dstMtx[i][3][0] += planeX;
            dstMtx[i][3][1] += planeY;
            dstMtx[i][3][2] += planeZ;
            if (dstToFixed) {
                bcopy((i << 6) + (u8 *)dstMtx, temp, sizeof(temp));
                guMtxF2L2(temp, (Mtx *)dstMtx + i);
            }
        }
        dist = func_150AD960(planeX, planeZ, objX, objZ);
        if (fadeEnd < dist) {
            entry->alpha = 0;
        } else if (dist < fadeStart) {
            entry->alpha = maxAlpha;
        } else {
            dist -= fadeStart;
            dist = maxAlpha * dist / (fadeEnd - fadeStart);
            dist = maxAlpha - dist;
            entry->alpha = dist;
        }
    } else {
        return;
    }
    if (obj != NULL && obj->unk9C != 0) {
        savedMtx = obj->ob_matrix_ptr;
        obj->ob_matrix_ptr = dstMtx;
        bone_collapseMaskedBones(obj);
        obj->ob_matrix_ptr = savedMtx;
    }
}

/* Reserves the next reflection queue slot, or returns NULL when the queue is full. */
ReflectionEntry *model_allocReflection(void) {
    if (gReflectionCount == 15) {
        return NULL;
    }
    gReflectionCount++;
    return &gReflections[gReflectionCount - 1];
}

/*
 * Returns 1 when the reflection of `obj`, whose mirrored root matrix is `mtx`, is outside the view.
 * In modes 0 and 1 the bounds centre is mirrored in the plane y = planeY.
 */
s32 model_isReflectionCulled(s32 mode, Actor *obj, f32 (*mtx)[4], f32 planeY) {
    f32 height;
    f32 radiusXZ;
    f32 radiusY;
    f32 radius;
    f32 y;
    RenderView *recp;
    ModelDef *bounds;
    s32 id;
    f32 delta;

    recp = gViews;
    id = obj->character_type;
    bounds = gModelDefs[id];
    radius = bounds->bodyRadius;
    radiusXZ = radius;
    radiusY = radius;
    radiusXZ *= obj->scale_xz;
    radiusY *= obj->scale_y;
    height = bounds->bodyHeight * obj->scale_y;
    if (mode == 1 || mode == 0) {
        delta = (obj->ypos + height) - planeY;
        y = planeY - delta;
    } else {
        y = obj->ypos + height;
    }
    if (func_150A6360(recp, gViewMatrices[0], mtx[3][0], y, mtx[3][2], radiusXZ, radiusY, 20000.0f) == 0) {
        return 1;
    }
    return 0;
}

s32 math_distance3D(s32, s32, s32);

/*
 * Queues reflections of object `objIndex` and of its attachments, mirrored as described at
 * model_mirrorMatrices. Objects beyond the view range, or whose reflection is off screen, are skipped.
 */
void model_queueReflection(s32 mode, s32 objIndex, f32 planeX, f32 planeY, f32 planeZ, f32 unused, f32 angle,
                   f32 fadeStart, f32 fadeEnd, f32 maxAlpha) {
    ReflectionEntry *entry;
    void *mtx;
    s32 color[3];
    MtxF *srcMtx;
    Mtx *boneMtx[4]; // matching: only [0] is written (the bone matrix, unused here); the array keeps the stack layout
    ModelAttachment *att;
    ModelAttachment *next;
    s32 range;
    s32 numBones;
    s32 fade;
    s32 numMtx;
    u8 alpha;
    u8 srcIsFixed;
    f32 dist;
    s32 attAlpha;

    range = 1400;
    if (gReflectionCount == 15) {
        return;
    }
    if (mode == 0 && movement_obj[objIndex].type != 1) {
        return;
    }
    if (movement_obj[objIndex].ob_matrix_ptr == NULL) {
        return;
    }
    if ((movement_obj[objIndex].unk74 & 0xF) == 15) {
        return;
    }
// matching: an unused label here swaps IDO's s0/s1 choice for range and alpha
visible:;
    alpha = movement_obj[objIndex].unk7;
    if (alpha == 0) {
        return;
    }
    numBones = actor_getBoneMatrixCount(movement_obj[objIndex].character_type);
    if (gBonePoolSize * sizeof(Mtx) < gBonePoolCursor + numBones * sizeof(Mtx) - gBonePoolStart) {
        return;
    }
    if (gCutsceneStates[0] == 1) {
        range = 10000;
    }
    dist = math_distance3D(camera_global_Cameras->eye.x - movement_obj[objIndex].xpos,
                         camera_global_Cameras->eye.y - movement_obj[objIndex].ypos,
                         camera_global_Cameras->eye.z - movement_obj[objIndex].zpos);
    if (range < dist) {
        return;
    }
    if (range - 200 < dist) {
        fade = (range - dist) * 256.0f / (range - 200);
    } else {
        fade = 256;
    }
    if (alpha != 255) {
        fade *= alpha;
        fade >>= 8;
    }
    entry = model_allocReflection();
    if (entry == NULL) {
        return;
    }
    entry->mtx = gBonePoolCursor;
    func_1502D54C(objIndex, color);
    entry->color[0] = color[0];
    entry->color[1] = color[1];
    entry->color[2] = color[2];
    entry->objIndex = objIndex;
    entry->isAttachment = FALSE;
    entry->ownerId = movement_obj[objIndex].unique_id;
    srcMtx = movement_obj[objIndex].ob_matrix_ptr;
    mtx = entry->mtx;
    model_mirrorMatrices(&movement_obj[objIndex], entry, mode, srcMtx, mtx, numBones, planeX, planeY, planeZ, unused,
                  angle, movement_obj[objIndex].xpos, movement_obj[objIndex].ypos,
                  movement_obj[objIndex].zpos, fadeStart, fadeEnd, maxAlpha, 0, 0);
    entry->alpha = (entry->alpha * fade) >> 8;
    if (model_isReflectionCulled(mode, &movement_obj[objIndex], *srcMtx, planeY) != 0) {
        gReflectionCount--;
        return;
    }
    gBonePoolCursor += (numBones << 3) << 3; // matching: numBones * sizeof(Mtx) schedules differently
    gBoneMatricesUsed += numBones;

    att = gAttachments;
    while (att != NULL) {
        next = att->next;
        if (movement_obj[objIndex].unique_id != att->ownerId || att->alpha == 0) {
            att = next;
            continue;
        }
        entry = model_allocReflection();
        if (entry == NULL) {
            return;
        }
        entry->color[0] = color[0];
        entry->color[1] = color[1];
        entry->color[2] = color[2];
        entry->objIndex = objIndex;
        entry->isAttachment = TRUE;
        entry->ownerId = movement_obj[objIndex].unique_id;
        entry->setId = att->setId;
        if (attach_getMatrices(att, &movement_obj[objIndex], &srcMtx, boneMtx) == 0) {
            att = next;
            continue;
        }
        numMtx = 1;
        if (att->anim != NULL) {
            numMtx = att->anim->numParts;
        }
        if (att->reflMtx[D_800BE9C0] == NULL) {
            att->reflMtx[D_800BE9C0] = allocate_memory(numMtx * sizeof(Mtx), 1, 2, 0);
        }
        entry->mtx = att->reflMtx[D_800BE9C0];
        mtx = entry->mtx;
        attAlpha = ((s32)maxAlpha * att->alpha) >> 8;
        if (att->mtx == NULL && att->anim == NULL) {
            srcIsFixed = FALSE;
        } else {
            srcIsFixed = TRUE;
        }
        model_mirrorMatrices(NULL, entry, mode, srcMtx, mtx, numMtx, planeX, planeY, planeZ, unused, angle,
                      movement_obj[objIndex].xpos, movement_obj[objIndex].ypos,
                      movement_obj[objIndex].zpos, fadeStart, fadeEnd, attAlpha, srcIsFixed, TRUE);
        att = next;
        entry->alpha = (entry->alpha * fade) >> 8;
    }
}

/*
 * Draws the queued reflections of attachment `att`: its display lists once per reflection, with
 * the reflection's matrices in segment 3 and its alpha in the env colour.
 */
Gfx *model_drawAttachmentReflections(Gfx *gfx, ModelAttachment *att, s32 red, s32 green, s32 blue) {
    ReflectionEntry *entry;
    s32 alpha;
    s32 i;
    s32 j;

    if (gReflectionCount == 0) {
        return gfx;
    }

    gSPSetGeometryMode(gfx++, G_CULL_FRONT);

    for (i = 0; i < gReflectionCount; i++) {
        entry = &gReflections[i];
        if (entry->isAttachment != TRUE) {
            continue;
        }
        if (entry->ownerId != att->ownerId) {
            continue;
        }
        if (entry->setId != att->setId) {
            continue;
        }

        alpha = (entry->alpha * att->alpha) >> 8;

        gDPPipeSync(gfx++);
        gSPSegment(gfx++, 3, entry->mtx);
        gDPSetEnvColor(gfx++, red, green, blue, alpha);

        if (alpha < 255) {
            gSPSegment(gfx++, 8, gModelXluRenderModes);
        } else {
            gSPSegment(gfx++, 8, gModelOpaRenderModes);
        }

        for (j = 0; j < att->numDLists; j++) {
            if (!(att->hiddenDLists & (1 << j))) {
                gSPDisplayList(gfx++, att->dlists[j]);
            }
        }
    }

    gSPClearGeometryMode(gfx++, G_CULL_FRONT);

    return gfx;
}

/* Draws the queued object reflections (not the attachment ones) for `player`. */
Gfx *model_drawReflections(Gfx *gfx, s32 player) {
    ReflectionEntry *entry;
    s32 color[4];
    s32 i;

    if (gReflectionCount == 0) {
        return gfx;
    }
    gSPSetGeometryMode(gfx++, G_SHADE | G_SHADING_SMOOTH);
    gSPClearGeometryMode(gfx++, G_FOG | G_LOD);
    gDPSetAlphaCompare(gfx++, G_AC_NONE);
    for (i = 0; i < gReflectionCount; i++) {
        entry = &gReflections[i];
        if (entry->isAttachment) {
            continue;
        }
        color[0] = entry->color[0];
        color[1] = entry->color[1];
        color[2] = entry->color[2];
        color[3] = 255;
        gfx = actor_emitModel(gfx, entry->objIndex, player, entry->mtx, entry->alpha, color, 0, 1);
    }
    return gfx;
}

/*
 * Queues a reflection in the horizontal plane y = waterHeight for every moving object of kind 0 or 1
 * that is above that plane while unk180 lies below it, when the gap is larger than the height from
 * actor_computeSize.
 */
void func_15036148(void) {
    s32 pad; // matching: stack slot
    s16 height;
    s16 unusedA;
    s16 unusedB;
    s32 i;

    if (!(camera_global_Cameras->SpecialFlags & 1)) {
        for (i = 0; i < 25; i++) {
            if (movement_obj[i].type != 0 && (movement_obj[i].drawtype == 0 || movement_obj[i].drawtype == 1)) {
                if (movement_obj[i].waterHeight != -10000.0f && movement_obj[i].waterHeight < movement_obj[i].ypos &&
                    movement_obj[i].floorY < movement_obj[i].waterHeight && movement_obj[i].shad > 5.0f) {
                    actor_computeSize(&movement_obj[i], &height, &unusedA, &unusedB);
                    if (height < movement_obj[i].waterHeight - movement_obj[i].floorY) {
                        model_queueReflection(1, i, movement_obj[i].xpos, movement_obj[i].waterHeight,
                                      movement_obj[i].zpos, 0.0f, 0.0f, 100.0f, 150.0f, 127.0f);
                    }
                }
            }
        }
    }
}
