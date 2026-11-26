#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One sample in the trail's ring buffer. */
typedef struct TrackerTrailPoint {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 length;   /* distance from the previous (older) sample */
    /* 0x10 */ f32 invSpeed; /* 1 / the owner's horizontal speed when sampled */
    /* 0x14 */ u8 alpha;
    /* 0x18 */ f32 texture;
    /* 0x1C */ f32 width;
    /* 0x20 */ TrailVec2 normal; /* horizontal unit normal; offsets the ribbon's two edges */
} TrackerTrailPoint; /* size 0x28 */

void *memcpy(void *, const void *, u32);
extern s32 gEffectRenderModes[][2];

/* Appends the owner's position to the trail and recomputes the previous sample's normal. */
s32 trail_sampleTrackerTrail(TrackerTrail *actor) {
    TrackerTrailData *state;
    TrackerTrailPoint *points;
    TrackerEffect *owner;
    TrackerData *motion;

    state = actor->state;
    points = actor->points;
    owner = state->owner;
    motion = &owner->data;
    if (D_800BE9E4 > 0) {
        actor->head.tmpl.pos = motion->pos;
        actor->head.tmpl.flags |= 4;
        {
            TrackerTrailPoint *newest;
            f32 speed;

            newest = &points[actor->head.end];
            newest->position = actor->head.tmpl.pos;
            newest->length = motion->dist;
            speed = sqrtf(motion->delta.x * motion->delta.x + motion->delta.z * motion->delta.z);
            if (speed == 0.0f) {
                newest->invSpeed = 1.0f;
            } else {
                newest->invSpeed = 1.0f / speed;
            }
            newest->width = 100.0f;
            newest->texture = state->texture;
            newest->alpha = 255;
            state->texture += newest->length * 21.85000038f;
            state->texture = func_15144528(state->texture, 8192.0f, 0.0f);
            if (speed) {
                newest->normal.x = -motion->delta.z * newest->invSpeed;
                newest->normal.z = motion->delta.x * newest->invSpeed;
            } else {
                newest->normal.x = 0.0f;
                newest->normal.z = 0.0f;
            }
            actor->head.end++;
            if (actor->head.end == actor->head.tmpl.itemCount) {
                actor->head.end = 0;
            }
            actor->head.count++;
            if (actor->head.end == actor->head.start) {
                actor->head.start++;
                if (actor->head.start == actor->head.tmpl.itemCount) {
                    actor->head.start = 0;
                }
                actor->head.count--;
            }
            if (actor->head.count >= 3) {
                s16 unused; // matching: unused, but it reserves the target's stack slot
                s16 middleIndex;
                s16 olderIndex;

                middleIndex = actor->head.end - 1;
                if (middleIndex < 0) {
                    middleIndex = actor->head.tmpl.itemCount - 1;
                }
                middleIndex--;
                if (middleIndex < 0) {
                    middleIndex = actor->head.tmpl.itemCount - 1;
                }
                olderIndex = middleIndex - 1;
                if (olderIndex < 0) {
                    olderIndex = actor->head.tmpl.itemCount - 1;
                }
                if (newest->length) {
                    f32 nextX;
                    f32 nextZ;
                    f32 middleInvSpeed;
                    TrailVec2 prevDir;
                    TrackerTrailPoint *middle;

                    middle = &points[middleIndex];
                    if (middle->length) {
                        TrackerTrailPoint *older;
                        TrailVec2 normal;
                        f32 lengthSq;

                        older = &points[olderIndex];
                        middleInvSpeed = middle->invSpeed;
                        nextX = (newest->position.x - middle->position.x) * newest->invSpeed;
                        nextZ = (newest->position.z - middle->position.z) * newest->invSpeed;
                        prevDir.x = (older->position.x - middle->position.x) * middleInvSpeed;
                        prevDir.z = (older->position.z - middle->position.z) * middleInvSpeed;
                        normal.x = -(nextZ - prevDir.z);
                        // matching: as two statements; one expression changes trail_drawTrackerTrail's vtx reloads
                        normal.z = nextX;
                        normal.z -= prevDir.x;
                        lengthSq = normal.x * normal.x + normal.z * normal.z;
                        if (lengthSq > 9.999999747e-05f) {
                            f32 invLength;

                            invLength = 1.0f / sqrtf(lengthSq);
                            normal.x *= invLength;
                            normal.z *= invLength;
                            middle->normal = normal;
                        } else {
                            middle->normal.x = 0.0f;
                            middle->normal.z = 0.0f;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

/* Trims the trail to state->maxLength and fades width and alpha from head to tail. */
s32 trail_shapeTrackerTrail(TrackerTrail *actor) {
    TrackerTrailData *state;
    TrackerTrailPoint *points;
    f32 total;
    f32 invTotal;
    s32 i;
    s32 newer;
    TrackerTrailPoint *newerPoint;
    TrackerTrailPoint *older;
    f32 t;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 olderX;
    f32 olderY;
    f32 olderZ;

    state = actor->state;
    points = actor->points;
    if (state->owner != NULL) {
        Actor *object = state->owner->data.owner;

        state->ownerScale = object->scale_xz;
    }
    if (actor->head.count >= 2) {
        total = 0; // matching: integer 0 (not 0.0f) for the target's FP register allocation
        i = actor->head.end - 1;
        if (i < 0) {
            i = actor->head.tmpl.itemCount - 1;
        }
        while (i != actor->head.start) {
            newer = i;
            i--;
            if (i < 0) {
                i = actor->head.tmpl.itemCount - 1;
            }
            newerPoint = &points[newer];
            total += newerPoint->length;
            if (state->maxLength < total) {
                /* Pull the older sample in so the trail is exactly maxLength long. */
                if (newerPoint->length != 0.0f) {
                    t = (total - state->maxLength) / newerPoint->length;
                    older = &points[i];
                    olderX = older->position.x;
                    olderY = older->position.y;
                    olderZ = older->position.z;
                    dx = olderX - newerPoint->position.x;
                    dy = olderY - newerPoint->position.y;
                    dz = olderZ - newerPoint->position.z;
                    older->position.x = olderX - dx * t;
                    older->position.y = olderY - dy * t;
                    older->position.z = olderZ - dz * t;
                    newerPoint->length *= 1.0f - t;
                }
                while (i != actor->head.start) {
                    actor->head.start++;
                    if (actor->head.start == actor->head.tmpl.itemCount) {
                        actor->head.start = 0;
                    }
                    actor->head.count--;
                }
                total = state->maxLength;
            }
        }
        // matching: integer 0 and the empty statement (a -g3 line entry) as in the target
        invTotal = (total != 0.0f) ? 1.0f / total : 0;
        ;
    }
    if (actor->head.count >= 2) {
        f32 widthStep;
        f32 alphaStep;
        f32 distance;
        s32 j;

        j = actor->head.end;
        distance = total;
        widthStep = state->widthScale * state->ownerScale * invTotal;
        alphaStep = state->alphaScale * invTotal;
        do {
            TrackerTrailPoint *sample;

            j--;
            if (j < 0) {
                j = actor->head.tmpl.itemCount - 1;
            }
            sample = &points[j];
            sample->width = state->widthBase * state->ownerScale + distance * widthStep;
            sample->alpha = distance * alphaStep;
            distance -= sample->length;
        } while (j != actor->head.start);
    }
    if ((actor->head.tmpl.flags & 1) && (actor->head.tmpl.flags & 8)) {
        state->alpha = actor->head.tmpl.life * 6;
    }
    return 1;
}

/* Draws the trail as a textured ribbon: one quad per pair of samples, head to tail. */
Gfx *trail_drawTrackerTrail(TrackerTrail *actor, Gfx *gfx, s16 arg2) {
    TrackerTrailData *state;
    Vec3f next;
    Vec3f first;
    TrackerTrailPoint *points;
    s32 index;
    s32 previous;
    u8 flag;
    u8 nextAlpha;
    f32 firstEdgeX;
    f32 firstEdgeZ;
    f32 nextEdgeX;
    f32 nextEdgeZ;
    f32 firstTexture;
    f32 nextTexture;
    u16 firstAlpha;
    Vtx *vtx;

    if (actor->head.count < 2) {
        return gfx;
    }
    state = actor->state;
    points = actor->points;
    particle_getViewBuffer(actor->unk84, arg2, (actor->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    flag = 1;
    gfx = func_15142E24(gfx, &gEffectTextures[188], 0, 0, 0, 0, 0xBC, 0, 0, &flag, 3);
    gfx = func_15142B7C(gfx, 0x220005, 0x1D0600);
    gfx = particle_setCombineMode(gfx, 0x4C, &flag);
    gfx = func_15142FBC(gfx, gSpriteTlutMode | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, gEffectRenderModes[1][0] | gEffectRenderModes[1][1], &flag);
    previous = actor->head.end - 1;
    if (previous < 0) {
        previous = actor->head.tmpl.itemCount - 1;
    }
    index = previous - 1;
    if (index < 0) {
        index = actor->head.tmpl.itemCount - 1;
    }
    first = points[previous].position;
    firstEdgeX = points[previous].width * points[previous].normal.x;
    firstEdgeZ = points[previous].width * points[previous].normal.z;
    firstAlpha = (points[previous].alpha * state->alpha) >> 8;
    firstTexture = points[previous].texture;
    next = points[index].position;
    nextEdgeX = points[index].width * points[index].normal.x;
    nextEdgeZ = points[index].width * points[index].normal.z;
    nextAlpha = (points[index].alpha * state->alpha) >> 8;
    nextTexture = points[index].texture;
    vtx->v.ob[0] = first.x + firstEdgeX;
    vtx->v.ob[1] = first.y;
    vtx->v.ob[2] = first.z + firstEdgeZ;
    vtx->v.cn[3] = firstAlpha;
    vtx->v.tc[1] = firstTexture;
    vtx->v.tc[0] = 0x7FF;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = first.x - firstEdgeX;
    vtx->v.ob[1] = first.y;
    vtx->v.ob[2] = first.z - firstEdgeZ;
    vtx->v.cn[3] = firstAlpha;
    vtx->v.tc[1] = firstTexture;
    vtx->v.tc[0] = 0;
    vtx->v.flag = 0;
    vtx++;
    do {
        vtx->v.ob[0] = next.x + nextEdgeX;
        vtx->v.ob[1] = next.y;
        vtx->v.ob[2] = next.z + nextEdgeZ;
        vtx->v.cn[3] = nextAlpha;
        vtx->v.tc[1] = nextTexture;
        vtx->v.tc[0] = 0x7FF;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = next.x - nextEdgeX;
        vtx->v.ob[1] = next.y;
        vtx->v.ob[2] = next.z - nextEdgeZ;
        vtx->v.cn[3] = nextAlpha;
        vtx->v.tc[1] = nextTexture;
        vtx->v.tc[0] = 0;
        vtx->v.flag = 0;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP2Triangles(gfx++, 0, 1, 2, 0, 1, 3, 2, 0);
        if (firstTexture < nextTexture) {
            /* The texture coordinate wrapped: repeat the last edge one texture period back. */
            memcpy(vtx, vtx - 2, 2 * sizeof(Vtx));
            vtx[-2].v.tc[1] -= 0x2000;
            vtx++;
            vtx[-2].v.tc[1] -= 0x2000;
            vtx++;
        }
        previous = index;
        index--;
        if (index < 0) {
            index = actor->head.tmpl.itemCount - 1;
        }
        if (previous != actor->head.start) {
            next = points[index].position;
            nextEdgeX = points[index].width * points[index].normal.x;
            firstTexture = points[previous].texture;
            nextEdgeZ = points[index].width * points[index].normal.z;
            nextTexture = points[index].texture;
            nextAlpha = (points[index].alpha * state->alpha) >> 8;
        }
    } while (previous != actor->head.start);
    return gfx;
}

/* Detaches the trail from its owner. */
void trail_detachTrackerTrail(TrackerTrail *actor) {
    TrackerTrailData *state;
    TrackerEffect *owner;

    state = actor->state;
    owner = state->owner;
    if (owner != NULL) {
        owner->data.trail = NULL;
    }
}

void trail_destroyTrackerTrail(TrackerTrail *actor) {
    trail_detachTrackerTrail(actor);
    effect_defaultDestroyEffectObj(actor);
}

void trail_freeTrackerTrail(TrackerTrail *actor) {
    trail_detachTrackerTrail(actor);
    effect_defaultFreeEffectObj(actor);
}
