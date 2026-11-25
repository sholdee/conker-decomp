#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} TrailVec3;

typedef struct {
    f32 x, z;
} TrailVec2;

typedef struct {
    /* 0x00 */ Actor *object;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0xC];
    /* 0x18 */ TrailVec3 position;
    /* 0x24 */ TrailVec3 velocity;
    /* 0x30 */ f32 speed;
} TrailOwnerMotion;

/* The object a trail follows. */
typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ TrailOwnerMotion motion;
} TrailOwner;

typedef struct {
    /* 0x00 */ TrailOwner *owner;
    /* 0x04 */ f32 texture; /* running texture coordinate of the newest sample */
    /* 0x08 */ u8 alpha;
    /* 0x0C */ f32 maxLength;
    /* 0x10 */ f32 widthBase;
    /* 0x14 */ f32 widthScale;
    /* 0x18 */ f32 alphaScale;
    /* 0x1C */ f32 ownerScale;
} TrailState;

/* One sample in the trail's ring buffer. */
typedef struct {
    /* 0x00 */ TrailVec3 position;
    /* 0x0C */ f32 length;   /* distance from the previous (older) sample */
    /* 0x10 */ f32 invSpeed; /* 1 / the owner's horizontal speed when sampled */
    /* 0x14 */ u8 alpha;
    /* 0x18 */ f32 texture;
    /* 0x1C */ f32 width;
    /* 0x20 */ TrailVec2 normal; /* horizontal unit normal; offsets the ribbon's two edges */
} TrailPoint; /* size 0x28 */

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ TrailVec3 position;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u16 flags;
    /* 0x20 */ u8 pad20[5];
    /* 0x25 */ u8 capacity;
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 tail; /* oldest sample */
    /* 0x2E */ s8 head; /* next slot to write */
    /* 0x2F */ u8 pad2F[0x84 - 0x2F];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailState *state;
} TrailActor;

extern f32 func_15144528(f32, f32, f32);
void *memcpy(void *, const void *, u32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *, void *, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern void func_151478F4(void *);
extern void func_15147928(void *);
extern u8 D_80091430[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;

/* Appends the owner's position to the trail and recomputes the previous sample's normal. */
s32 func_151D15D0(TrailActor *actor) {
    TrailState *state;
    TrailPoint *points;
    TrailOwner *owner;
    TrailOwnerMotion *motion;

    state = actor->state;
    points = actor->points;
    owner = state->owner;
    motion = &owner->motion;
    if (D_800BE9E4 > 0) {
        actor->position = motion->position;
        actor->flags |= 4;
        {
            TrailPoint *newest;
            f32 speed;

            newest = &points[actor->head];
            newest->position = actor->position;
            newest->length = motion->speed;
            speed = sqrtf(motion->velocity.x * motion->velocity.x + motion->velocity.z * motion->velocity.z);
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
                newest->normal.x = -motion->velocity.z * newest->invSpeed;
                newest->normal.z = motion->velocity.x * newest->invSpeed;
            } else {
                newest->normal.x = 0.0f;
                newest->normal.z = 0.0f;
            }
            actor->head++;
            if (actor->head == actor->capacity) {
                actor->head = 0;
            }
            actor->count++;
            if (actor->head == actor->tail) {
                actor->tail++;
                if (actor->tail == actor->capacity) {
                    actor->tail = 0;
                }
                actor->count--;
            }
            if (actor->count >= 3) {
                s16 unused; // matching: unused, but it reserves the target's stack slot
                s16 middleIndex;
                s16 olderIndex;

                middleIndex = actor->head - 1;
                if (middleIndex < 0) {
                    middleIndex = actor->capacity - 1;
                }
                middleIndex--;
                if (middleIndex < 0) {
                    middleIndex = actor->capacity - 1;
                }
                olderIndex = middleIndex - 1;
                if (olderIndex < 0) {
                    olderIndex = actor->capacity - 1;
                }
                if (newest->length) {
                    f32 nextX;
                    f32 nextZ;
                    f32 middleInvSpeed;
                    TrailVec2 prevDir;
                    TrailPoint *middle;

                    middle = &points[middleIndex];
                    if (middle->length) {
                        TrailPoint *older;
                        TrailVec2 normal;
                        f32 lengthSq;

                        older = &points[olderIndex];
                        middleInvSpeed = middle->invSpeed;
                        nextX = (newest->position.x - middle->position.x) * newest->invSpeed;
                        nextZ = (newest->position.z - middle->position.z) * newest->invSpeed;
                        prevDir.x = (older->position.x - middle->position.x) * middleInvSpeed;
                        prevDir.z = (older->position.z - middle->position.z) * middleInvSpeed;
                        normal.x = -(nextZ - prevDir.z);
                        // matching: as two statements; one expression changes func_151D1C98's vtx reloads
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
s32 func_151D197C(TrailActor *actor) {
    TrailState *state;
    TrailPoint *points;
    f32 total;
    f32 invTotal;
    s32 i;
    s32 newer;
    TrailPoint *newerPoint;
    TrailPoint *older;
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
        Actor *object = state->owner->motion.object;

        state->ownerScale = object->xz_scale;
    }
    if (actor->count >= 2) {
        total = 0; // matching: integer 0 (not 0.0f) for the target's FP register allocation
        i = actor->head - 1;
        if (i < 0) {
            i = actor->capacity - 1;
        }
        while (i != actor->tail) {
            newer = i;
            i--;
            if (i < 0) {
                i = actor->capacity - 1;
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
                while (i != actor->tail) {
                    actor->tail++;
                    if (actor->tail == actor->capacity) {
                        actor->tail = 0;
                    }
                    actor->count--;
                }
                total = state->maxLength;
            }
        }
        // matching: integer 0 and the empty statement (a -g3 line entry) as in the target
        invTotal = (total != 0.0f) ? 1.0f / total : 0;
        ;
    }
    if (actor->count >= 2) {
        f32 widthStep;
        f32 alphaStep;
        f32 distance;
        s32 j;

        j = actor->head;
        distance = total;
        widthStep = state->widthScale * state->ownerScale * invTotal;
        alphaStep = state->alphaScale * invTotal;
        do {
            TrailPoint *sample;

            j--;
            if (j < 0) {
                j = actor->capacity - 1;
            }
            sample = &points[j];
            sample->width = state->widthBase * state->ownerScale + distance * widthStep;
            sample->alpha = distance * alphaStep;
            distance -= sample->length;
        } while (j != actor->tail);
    }
    if ((actor->flags & 1) && (actor->flags & 8)) {
        state->alpha = actor->unk1C * 6;
    }
    return 1;
}

/* Draws the trail as a textured ribbon: one quad per pair of samples, head to tail. */
Gfx *func_151D1C98(TrailActor *actor, Gfx *gfx, s16 arg2) {
    TrailState *state;
    TrailVec3 next;
    TrailVec3 first;
    TrailPoint *points;
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

    if (actor->count < 2) {
        return gfx;
    }
    state = actor->state;
    points = actor->points;
    func_151D5D60(actor->unk84, arg2, (actor->capacity * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
    if (vtx == NULL) {
        return gfx;
    }
    flag = 1;
    gfx = func_15142E24(gfx, D_80091430, 0, 0, 0, 0, 0xBC, 0, 0, &flag, 3);
    gfx = func_15142B7C(gfx, 0x220005, 0x1D0600);
    gfx = func_1513F4E4(gfx, 0x4C, &flag);
    gfx = func_15142FBC(gfx, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[1][0] | D_800A4AC8[1][1], &flag);
    previous = actor->head - 1;
    if (previous < 0) {
        previous = actor->capacity - 1;
    }
    index = previous - 1;
    if (index < 0) {
        index = actor->capacity - 1;
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
            index = actor->capacity - 1;
        }
        if (previous != actor->tail) {
            next = points[index].position;
            nextEdgeX = points[index].width * points[index].normal.x;
            firstTexture = points[previous].texture;
            nextEdgeZ = points[index].width * points[index].normal.z;
            nextTexture = points[index].texture;
            nextAlpha = (points[index].alpha * state->alpha) >> 8;
        }
    } while (previous != actor->tail);
    return gfx;
}

/* Detaches the trail from its owner. */
void func_151D223C(TrailActor *actor) {
    TrailState *state;
    TrailOwner *owner;

    state = actor->state;
    owner = state->owner;
    if (owner != NULL) {
        owner->motion.unk8 = 0;
    }
}

void func_151D2258(TrailActor *actor) {
    func_151D223C(actor);
    func_151478F4(actor);
}

void func_151D2284(TrailActor *actor) {
    func_151D223C(actor);
    func_15147928(actor);
}
