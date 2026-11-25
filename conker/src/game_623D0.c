#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One queued reflection: a model's mirrored matrices plus how to draw them. */
typedef struct ReflectionEntry {
    /* 0x00 */ u32 mtx;        // segment address of the mirrored matrices
    /* 0x04 */ u8 alpha;
    /* 0x05 */ u8 color[3];
    /* 0x08 */ u8 objIndex;    // index into D_800CC2D0
    /* 0x09 */ u8 ownerId;     // unique_id of that object
    /* 0x0A */ u8 setId;       // attachment set id (attachment reflections only)
    /* 0x0B */ u8 isAttachment;
} ReflectionEntry;

extern u8 D_800C3F00;               // number of queued reflections
extern ReflectionEntry D_800C3F08[]; // reflection queue (15 entries)

/* Clears the reflection queue. */
void func_15034F20(void) {
    D_800C3F00 = 0;
}

void func_150A7A48(void *, void *, void *);
void guMtxF2L2(void *, void *);
void func_15034728(Actor *);
s32 func_150AD960(s32, s32, s32, s32);

/*
 * Mirrors `count` matrices from `srcMtx` into `dstMtx` and sets the entry's alpha from the distance
 * to the mirror. Modes 0 and 1 mirror in the horizontal plane y = planeY; mode 2 mirrors in the
 * vertical plane through (planeX, planeZ) at `angle`. `srcIsFixed`: the sources are fixed point
 * (Mtx); `dstToFixed`: convert the results to fixed point.
 */
void func_15034F30(Actor *obj, ReflectionEntry *entry, s32 mode, f32 (*srcMtx)[4][4],
                   f32 (*dstMtx)[4][4], s32 count, f32 planeX, f32 planeY, f32 planeZ, f32 unused,
                   f32 angle, f32 objX, f32 objY, f32 objZ, f32 fadeStart, f32 fadeEnd, f32 maxAlpha,
                   s32 srcIsFixed, s32 dstToFixed) {
    MtxF flip;
    MtxF rotA;
    MtxF rotB;
    s32 i;
    f32 (*src)[4];
    f32 dist;
    MtxF temp;
    struct255 *savedMtx;

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
        savedMtx = obj->unk1D4;
        obj->unk1D4 = (struct255 *)dstMtx;
        func_15034728(obj);
        obj->unk1D4 = savedMtx;
    }
}


/* Reserves the next reflection queue slot, or returns NULL when the queue is full. */
ReflectionEntry *func_150356C8(void) {
    if (D_800C3F00 == 15) {
        return NULL;
    }
    D_800C3F00++;
    return &D_800C3F08[D_800C3F00 - 1];
}


/* Per model-type bounds, D_800D1C90[id] (only the fields used here). */
typedef struct ModelBounds {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ u16 body_radius;
    /* 0x10 */ s16 body_height;
} ModelBounds;

s32 func_150A6360(s32, void *, f32, f32, f32, f32, f32, f32);
extern u8 D_800D9C10; // view matrices

/*
 * Returns 1 when the reflection of `obj`, whose mirrored root matrix is `mtx`, is outside the view.
 * In modes 0 and 1 the bounds centre is mirrored in the plane y = planeY.
 */
s32 func_15035714(s32 mode, Actor *obj, f32 (*mtx)[4], f32 planeY) {
    f32 height;
    f32 radiusXZ;
    f32 radiusY;
    f32 radius;
    f32 y;
    s32 recp;
    ModelBounds *bounds;
    s32 id;
    f32 delta;

    recp = (s32)D_800BE628;
    id = obj->id;
    bounds = (ModelBounds *)D_800D1C90[id];
    radius = bounds->body_radius;
    radiusXZ = radius;
    radiusY = radius;
    radiusXZ *= obj->xz_scale;
    radiusY *= obj->y_scale;
    height = bounds->body_height * obj->y_scale;
    if (mode == 1 || mode == 0) {
        delta = (obj->y_position + height) - planeY;
        y = planeY - delta;
    } else {
        y = obj->y_position + height;
    }
    if (func_150A6360(recp, &D_800D9C10, mtx[3][0], y, mtx[3][2], radiusXZ, radiusY, 20000.0f) == 0) {
        return 1;
    }
    return 0;
}

/* Model animation state (layout as in game_5D2C0.c; only the field used here). */
typedef struct AnimInstance {
    /* 0x000 */ u8 pad0[0x3F4];
    /* 0x3F4 */ u8 numMtx;
} AnimInstance;

/* A model attached to a game object, from the list headed by D_800C3EE0 (layout as in
 * game_5D2C0.c; only the fields used here). */
typedef struct Attachment {
    /* 0x00 */ u8 ownerId;        // unique_id of the owning Actor
    /* 0x01 */ u8 pad1[2];
    /* 0x03 */ u8 alpha;
    /* 0x04 */ u8 pad4[2];
    /* 0x06 */ u8 setId;
    /* 0x07 */ u8 pad7[0xC];
    /* 0x13 */ u8 hiddenDLists;   // bit i set: display list i is not drawn
    /* 0x14 */ u8 numDLists;
    /* 0x15 */ u8 pad15[0xF];
    /* 0x24 */ Gfx **dlists;
    /* 0x28 */ u8 pad28[0xC];
    /* 0x34 */ Mtx *mtx;
    /* 0x38 */ u8 pad38[0x10];
    /* 0x48 */ AnimInstance *anim;
    /* 0x4C */ u32 reflMtx[2];    // mirrored matrices, one per frame buffer
    /* 0x54 */ struct Attachment *next;
} Attachment;

extern u16 D_800C3E7A; // bone matrices used this frame
extern u32 D_800C3E88; // bone-matrix pool cursor
extern u32 D_800C3E8C; // bone-matrix pool start
extern void *allocate_memory(s32, s32, s32, s32);
void func_1502D54C(s32, s32 *);
s32 func_1502DB20(u8);
s32 func_15031070(Attachment *, Actor *, struct255 **, s32 *);
s32 func_150AD9A0(s32, s32, s32);

/*
 * Queues reflections of object `objIndex` and of its attachments, mirrored as described at
 * func_15034F30. Objects beyond the view range, or whose reflection is off screen, are skipped.
 */
void func_15035808(s32 mode, s32 objIndex, f32 planeX, f32 planeY, f32 planeZ, f32 unused, f32 angle,
                   f32 fadeStart, f32 fadeEnd, f32 maxAlpha) {
    ReflectionEntry *entry;
    u32 mtx;
    s32 color[3];
    struct255 *srcMtx;
    s32 partCount[4]; // matching: one s32 is written; the array keeps the stack layout
    Attachment *att;
    Attachment *next;
    s32 range;
    s32 numBones;
    s32 fade;
    s32 numMtx;
    u8 alpha;
    u8 srcIsFixed;
    f32 dist;
    s32 attAlpha;

    range = 1400;
    if (D_800C3F00 == 15) {
        return;
    }
    if (mode == 0 && D_800CC2D0[objIndex].interaction_state != 1) {
        return;
    }
    if (D_800CC2D0[objIndex].unk1D4 == NULL) {
        return;
    }
    if ((D_800CC2D0[objIndex].unk74 & 0xF) == 15) {
        return;
    }
// matching: an unused label here swaps IDO's s0/s1 choice for range and alpha
visible:;
    alpha = D_800CC2D0[objIndex].unk7;
    if (alpha == 0) {
        return;
    }
    numBones = func_1502DB20(D_800CC2D0[objIndex].id);
    if (D_800C3E7C * sizeof(Mtx) < D_800C3E88 + numBones * sizeof(Mtx) - D_800C3E8C) {
        return;
    }
    if (D_800C35EA == 1) {
        range = 10000;
    }
    dist = func_150AD9A0(D_800DBFF0->eye.x - D_800CC2D0[objIndex].x_position,
                         D_800DBFF0->eye.y - D_800CC2D0[objIndex].y_position,
                         D_800DBFF0->eye.z - D_800CC2D0[objIndex].z_position);
    if ((f32)range < dist) {
        return;
    }
    if ((f32)(range - 200) < dist) {
        fade = ((f32)range - dist) * 256.0f / (f32)(range - 200);
    } else {
        fade = 256;
    }
    if (alpha != 255) {
        fade *= alpha;
        fade >>= 8;
    }
    entry = func_150356C8();
    if (entry == NULL) {
        return;
    }
    entry->mtx = D_800C3E88;
    func_1502D54C(objIndex, color);
    entry->color[0] = color[0];
    entry->color[1] = color[1];
    entry->color[2] = color[2];
    entry->objIndex = objIndex;
    entry->isAttachment = FALSE;
    entry->ownerId = D_800CC2D0[objIndex].unique_id;
    srcMtx = D_800CC2D0[objIndex].unk1D4;
    mtx = entry->mtx;
    func_15034F30(&D_800CC2D0[objIndex], entry, mode, srcMtx, mtx, numBones, planeX, planeY, planeZ, unused,
                  angle, D_800CC2D0[objIndex].x_position, D_800CC2D0[objIndex].y_position,
                  D_800CC2D0[objIndex].z_position, fadeStart, fadeEnd, maxAlpha, 0, 0);
    entry->alpha = (entry->alpha * fade) >> 8;
    if (func_15035714(mode, &D_800CC2D0[objIndex], (f32 (*)[4])srcMtx, planeY) != 0) {
        D_800C3F00--;
        return;
    }
    D_800C3E88 += (numBones << 3) << 3; // matching: numBones * sizeof(Mtx) schedules differently
    D_800C3E7A += numBones;

    att = (Attachment *)D_800C3EE0;
    while (att != NULL) {
        next = att->next;
        if (D_800CC2D0[objIndex].unique_id != att->ownerId || att->alpha == 0) {
            att = next;
            continue;
        }
        entry = func_150356C8();
        if (entry == NULL) {
            return;
        }
        entry->color[0] = color[0];
        entry->color[1] = color[1];
        entry->color[2] = color[2];
        entry->objIndex = objIndex;
        entry->isAttachment = TRUE;
        entry->ownerId = D_800CC2D0[objIndex].unique_id;
        entry->setId = att->setId;
        if (func_15031070(att, &D_800CC2D0[objIndex], &srcMtx, partCount) == 0) {
            att = next;
            continue;
        }
        numMtx = 1;
        if (att->anim != NULL) {
            numMtx = att->anim->numMtx;
        }
        if (att->reflMtx[D_800BE9C0] == 0) {
            att->reflMtx[D_800BE9C0] = (u32)allocate_memory(numMtx * sizeof(Mtx), 1, 2, 0);
        }
        entry->mtx = att->reflMtx[D_800BE9C0];
        mtx = entry->mtx;
        attAlpha = ((s32)maxAlpha * att->alpha) >> 8;
        if (att->mtx == NULL && att->anim == NULL) {
            srcIsFixed = FALSE;
        } else {
            srcIsFixed = TRUE;
        }
        func_15034F30(NULL, entry, mode, srcMtx, mtx, numMtx, planeX, planeY, planeZ, unused, angle,
                      D_800CC2D0[objIndex].x_position, D_800CC2D0[objIndex].y_position,
                      D_800CC2D0[objIndex].z_position, fadeStart, fadeEnd, attAlpha, srcIsFixed, TRUE);
        att = next;
        entry->alpha = (entry->alpha * fade) >> 8;
    }
}

extern u8 D_80082FC0[];
extern u8 D_80083140[];

/*
 * Draws the queued reflections of attachment `att`: its display lists once per reflection, with
 * the reflection's matrices in segment 3 and its alpha in the env colour.
 */
Gfx *func_15035D6C(Gfx *gfx, Attachment *att, s32 red, s32 green, s32 blue) {
    ReflectionEntry *entry;
    s32 alpha;
    s32 i;
    s32 j;

    if (D_800C3F00 == 0) {
        return gfx;
    }

    gSPSetGeometryMode(gfx++, G_CULL_FRONT);

    for (i = 0; i < D_800C3F00; i++) {
        entry = &D_800C3F08[i];
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
            gSPSegment(gfx++, 8, D_80082FC0);
        } else {
            gSPSegment(gfx++, 8, D_80083140);
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

extern Gfx *func_1502CCFC(Gfx *, s32, s32, u32, s32, s32 *, s32, s32);

/* Draws the queued object reflections (not the attachment ones) for `player`. */
Gfx *func_15035FE8(Gfx *gfx, s32 player) {
    ReflectionEntry *entry;
    s32 color[4];
    s32 i;

    if (D_800C3F00 == 0) {
        return gfx;
    }
    gSPSetGeometryMode(gfx++, G_SHADE | G_SHADING_SMOOTH);
    gSPClearGeometryMode(gfx++, G_FOG | G_LOD);
    gDPSetAlphaCompare(gfx++, G_AC_NONE);
    for (i = 0; i < D_800C3F00; i++) {
        entry = &D_800C3F08[i];
        if (entry->isAttachment) {
            continue;
        }
        color[0] = entry->color[0];
        color[1] = entry->color[1];
        color[2] = entry->color[2];
        color[3] = 255;
        gfx = func_1502CCFC(gfx, entry->objIndex, player, entry->mtx, entry->alpha, color, 0, 1);
    }
    return gfx;
}

void func_1507C3E0(Actor *, s16 *, s16 *, s16 *);

/*
 * Queues a reflection in the horizontal plane y = unk118 for every moving object of kind 0 or 1
 * that is above that plane while unk180 lies below it, when the gap is larger than the height from
 * func_1507C3E0.
 */
void func_15036148(void) {
    s32 pad; // matching: stack slot
    s16 height;
    s16 unusedA;
    s16 unusedB;
    s32 i;

    if (!(D_800DBFF0->status & 1)) {
        for (i = 0; i < 25; i++) {
            if (D_800CC2D0[i].interaction_state != 0 && (D_800CC2D0[i].unk5 == 0 || D_800CC2D0[i].unk5 == 1)) {
                if (D_800CC2D0[i].unk118 != -10000.0f && D_800CC2D0[i].unk118 < D_800CC2D0[i].y_position &&
                    D_800CC2D0[i].unk180 < D_800CC2D0[i].unk118 && D_800CC2D0[i].unk28 > 5.0f) {
                    func_1507C3E0(&D_800CC2D0[i], &height, &unusedA, &unusedB);
                    if ((f32)height < D_800CC2D0[i].unk118 - D_800CC2D0[i].unk180) {
                        func_15035808(1, i, D_800CC2D0[i].x_position, D_800CC2D0[i].unk118,
                                      D_800CC2D0[i].z_position, 0.0f, 0.0f, 100.0f, 150.0f, 127.0f);
                    }
                }
            }
        }
    }
}
