#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/*
 * Bone trails: an emitter object (a struct260 made by func_151D71B0) follows a point on an actor's
 * bone and drags a trail object (made by func_15147A80) behind it. The trail keeps a ring buffer of
 * points that it renders as a textured ribbon.
 */

struct Trail;

/* Emitter data at 0x28: where the trail is anchored this frame. */
typedef struct {
    /* 0x00 */ struct Trail *trail; /* attached trail, or NULL */
    /* 0x04 */ u8 anchorType;       /* index into D_8008FCA0 (update) and D_8008FCA4 (message handler) */
    /* 0x05 */ u8 flags;            /* bit 0: pos is valid this frame */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 maxStep; /* moving further than this in one frame detaches the trail */
} TrailAnchor;

/* Emitter data at 0x40 for anchor type 0: a point on an actor's bone. */
typedef struct {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId;  /* actor->unique_id when attached; a mismatch means the slot was reused */
    /* 0x05 */ u8 bone;     /* index into D_800AB280 (offset) and D_800AB2D4 (bone matrix) */
    /* 0x06 */ s8 condition; /* index into D_8008FCA8, or -1 for none */
} TrailBoneLink;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD; /* bit 0: remove */
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ TrailAnchor anchor;
    /* 0x40 */ TrailBoneLink link;
} TrailEmitter;

extern s32 (*D_8008FCA0[])(TrailEmitter *);
extern void (*D_8008FCA4[])(TrailEmitter *, TrailBoneLink *, u8);
extern s32 (*D_8008FCA8[])(TrailEmitter *);
extern Vec3f D_800AB280[];
extern u8 D_800AB2D4[];

extern f32 func_15143E64(Vec3f *);
void func_15143134(Vec3f *offset, f32 *out, Mtx *mtx);
void func_151D77C8(TrailEmitter *emitter);
void func_151D7830(TrailEmitter *emitter);

/* Creates an emitter of the given anchor type with `extraSize` more bytes of data after the TrailAnchor. */
struct260 *func_151D71B0(s16 arg0, u8 arg1, u8 anchorType, f32 maxStep, s32 extraSize, u8 arg5, s32 arg6) {
    struct260 *emitter;
    TrailAnchor anchor;

    anchor.trail = NULL;
    anchor.anchorType = anchorType;
    anchor.flags = 0;
    anchor.pos.x = 0.0f;
    anchor.pos.y = 0.0f;
    anchor.pos.z = 0.0f;
    anchor.maxStep = maxStep;

    emitter = func_15149130(arg0, -1, 0x42, -1, arg1, 0x36, (extraSize + sizeof(TrailAnchor)), arg5, arg6);
    if (emitter != NULL) {
        memcpy(&((TrailEmitter *)emitter)->anchor, &anchor, sizeof(TrailAnchor));
    }
    return emitter;
}

/*
 * Emitter update: moves the anchor, and removes the emitter once its anchor is gone. A trail is attached
 * while the anchor stays valid and moves less than maxStep per frame, and detached otherwise.
 */
void func_151D7264(TrailEmitter *emitter) {
    TrailAnchor *anchor;
    Vec3f prevPos;
    u8 wasValid;
    Vec3f step;

    anchor = &emitter->anchor;
    prevPos = anchor->pos;
    wasValid = anchor->flags & 1;
    if (D_8008FCA0[anchor->anchorType](emitter) == 0) {
        emitter->unkE = -1;
        emitter->unkD |= 1;
        return;
    }
    if (anchor->flags & 1) {
        if (wasValid) {
            step.x = anchor->pos.x - prevPos.x;
            step.y = anchor->pos.y - prevPos.y;
            step.z = anchor->pos.z - prevPos.z;
            if (func_15143E64(&step) < anchor->maxStep) {
                if (anchor->trail == NULL) {
                    func_151D7830(emitter);
                }
            } else {
                func_151D77C8(emitter);
            }
        } else {
            func_151D77C8(emitter);
        }
    } else {
        func_151D77C8(emitter);
    }
}

/* Emitter message handler: forwards to the anchor type's handler. */
void func_151D73A8(TrailEmitter *emitter, TrailBoneLink *msg, u8 msgType) {
    if (D_8008FCA4[emitter->anchor.anchorType] != NULL) {
        // matching: the original reloads anchorType for the call; IDO would reuse the first load
        D_8008FCA4[*(volatile u8 *)&emitter->anchor.anchorType](emitter, msg, msgType);
    }
}

void func_151D7404(TrailEmitter *emitter) {
    func_151D77C8(emitter);
}

void func_151D7424(struct260 *emitter) {
    func_151D7404(emitter);
    func_1514933C(emitter);
}

void func_151D7450(struct260 *emitter) {
    func_151D7404(emitter);
    func_15149368(emitter);
}

/* Broadcasts message 0x3D ({actor, unique_id}): the actor is going away. */
void func_151D747C(Actor *actor) {
    struct {
        Actor *actor;
        u8 actorId;
    } msg;

    msg.actor = actor;
    msg.actorId = actor->unique_id;
    func_151494E0((s32)&msg, 0x3D);
}

/* Attaches a trail to `bone` of `actor`; `condition` picks an extra D_8008FCA8 check (-1 for none). */
void func_151D74B0(Actor *actor, u8 bone, s8 condition, u8 arg3, s32 arg4) {
    TrailBoneLink link;
    struct260 *emitter;

    link.actor = actor;
    link.actorId = actor->unique_id;
    link.bone = bone;
    link.condition = condition;

    emitter = func_151D71B0(0x12C, 0, 0, 12.0f, sizeof(TrailBoneLink), arg3, arg4);
    if (emitter != NULL) {
        memcpy(&((TrailEmitter *)emitter)->link, &link, sizeof(TrailBoneLink));
    }
}

/* Anchor type 0 message handler: removes the emitter when its actor goes away (message 0x3D). */
void func_151D7538(TrailEmitter *emitter, TrailBoneLink *msg, u8 msgType) {
    // matching: the original reloads emitter and msg from their stack slots (address taken / int reload)
    TrailEmitter **pEmitter = &emitter;
    TrailBoneLink *link;

    if (msgType == 0x3D) {
        link = &(*pEmitter)->link;
        if ((msg->actor == link->actor) || (((TrailBoneLink *)*(s32 *)&msg)->actorId == link->actorId)) {
            func_1516972C((struct102 *)*pEmitter);
        }
    } else {
        link = &(*pEmitter)->link;
        func_15149514((s32)msg, msgType, &link->actor, &link->actorId, *pEmitter);
    }
}

/*
 * Anchor type 0 update: puts the anchor on the actor's bone. Fails (0) once the actor is gone; the
 * anchor is invalid while the actor has no bone matrices, rides something with a camera, or has
 * unk7 set.
 */
s32 func_151D75C4(TrailEmitter *emitter) {
    TrailBoneLink *link;
    Actor *actor;
    TrailAnchor *anchor;

    anchor = &emitter->anchor;
    link = &emitter->link;
    actor = link->actor;
    if ((actor->interaction_state == 0) || (actor->unique_id != link->actorId)) {
        return 0;
    }
    if (actor->unk1D4 == NULL) {
        anchor->flags &= ~1;
        return 1;
    }
    if ((actor->unk31C != NULL) && (actor->unk31C->aimState != 0) && (actor->camera != NULL)) {
        anchor->flags &= ~1;
        return 1;
    }
    if (actor->unk7 != 0xFF) {
        anchor->flags &= ~1;
        return 1;
    }
    func_15143134(&D_800AB280[link->bone], &anchor->pos.x, (Mtx *)actor->unk1D4 + D_800AB2D4[link->bone]);
    anchor->flags |= 1;
    if (link->condition != -1) {
        return D_8008FCA8[link->condition](emitter);
    }
    return 1;
}

/* Condition 0: the anchor is invalid while unk94 bit 1 is set or unk84 is 4, 10 or 12. */
s32 func_151D7724(TrailEmitter *emitter) {
    Actor *actor = emitter->link.actor;
    TrailAnchor *anchor = &emitter->anchor;

    if ((actor->unk94 & 2) || actor->unk84.uh == 4 || actor->unk84.uh == 10 || actor->unk84.uh == 12) {
        anchor->flags &= ~1;
    }
    return 1;
}

/* Condition 1: the anchor is invalid while the actor's unk84 is 0. */
s32 func_151D7770(TrailEmitter *emitter) {
    TrailAnchor *anchor = &emitter->anchor;
    Actor *actor = emitter->link.actor;

    if (actor->unk84.uh == 0) {
        anchor->flags &= ~1;
    }
    return 1;
}

/* Condition 2: the anchor is invalid while the actor is in water. */
s32 func_151D779C(TrailEmitter *emitter) {
    TrailAnchor *anchor = &emitter->anchor;
    Actor *actor = emitter->link.actor;

    if (actor->in_water) {
        anchor->flags &= ~1;
    }
    return 1;
}

/* A trail point. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 velY;
    /* 0x10 */ f32 length; /* distance to the next newer point */
    /* 0x14 */ u8 alpha;
    /* 0x18 */ f32 texU;
} TrailPoint;

/* Trail data at 0x98. */
typedef struct {
    /* 0x00 */ TrailEmitter *emitter; /* NULL once detached */
    /* 0x04 */ Vec3f lastPos;        /* where the last point was emitted */
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 spawnAccum;
    /* 0x18 */ f32 texPhase;
} TrailData;

typedef struct Trail {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Vec3f pos; /* head of the trail */
    /* 0x1C */ s16 timer;  /* while fading: alpha is timer * 12 */
    /* 0x1E */ u16 flags;  /* bit 1: the head follows the emitter; bit 3: fading */
    /* 0x20 */ u8 pad20[5];
    /* 0x25 */ u8 capacity; /* size of the points ring buffer */
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 tail; /* oldest point */
    /* 0x2E */ s8 head; /* next point to write */
    /* 0x2F */ u8 pad2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 pad31[0x23];
    /* 0x54 */ Vec3f tailPos;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailData *data;
} Trail;

extern Trail *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Detaches the emitter's trail, which then fades out. */
void func_151D77C8(TrailEmitter *emitter) {
    TrailAnchor *anchor = &emitter->anchor;

    if (anchor->trail != NULL) {
        TrailData *data = anchor->trail->data;

        anchor->trail->unk30 = 0;
        anchor->trail->flags &= ~2;
        anchor->trail->flags |= 8;
        anchor->trail->flags |= 1;
        anchor->trail->timer = 20;
        data->emitter = NULL;
        anchor->trail = NULL;
    }
}

/* Spawns a 25-point trail at the anchor and attaches it to the emitter. */
void func_151D7830(TrailEmitter *emitter) {
    Trail *trail;
    TrailData data;
    EffectTemplate header;
    // matching: frame size (compiler temps below header)
    s32 pad;

    data.emitter = emitter;
    data.lastPos = emitter->anchor.pos;
    data.unk10 = 0.0f;
    data.spawnAccum = 0.0f;
    data.texPhase = 0.0f;
    header.itemCount = 25;

    header.pos = emitter->anchor.pos;
    header.life = 300;
    header.flags = 0x76;
    header.kind = 0x12;
    header.unk14 = 4;
    header.unk18 = 0;

    trail = func_15147A80(&header, 0x20, sizeof(TrailData), 0xD, 0x10, 0x10, 0, 0, 0, emitter->unkC, emitter->unk1);
    if (trail != NULL) {
        memcpy(trail->data, &data, sizeof(TrailData));
        emitter->anchor.trail = trail;
    }
}

extern f32 D_800BE9A4;

void func_151D8718(Vec3f *pos, f32 *velY, f32 dt);

/* Detached trail update: the points keep falling and the tail position follows the oldest point. */
s32 func_151D792C(Trail *trail) {
    TrailPoint *points;
    s32 i;

    points = trail->points;
    if ((trail->count < 2) && (trail->flags & 8)) {
        return 0;
    }
    i = trail->head;
    while (i != trail->tail) {
        i--;
        if (i < 0) {
            i = trail->capacity - 1;
        }
        func_151D8718(&points[i].pos, &points[i].velY, D_800BE9A4);
    }
// matching: an (unused) label here makes IDO start a new block for the tail update
tail:
    if (trail->count > 0) {
        trail->tailPos = points[trail->tail].pos;
    } else {
        trail->tailPos.x = 0.0f;
        trail->tailPos.y = 0.0f;
        trail->tailPos.z = 0.0f;
    }
    return 1;
}

/*
 * Attached trail update: the head follows the emitter, and points are emitted (4 per second of
 * D_800BE9A4) along the way from the last emitted point, each pre-aged by the time since it was due.
 */
s32 func_151D7A38(Trail *trail) {
    TrailData *data;
    TrailPoint *points;
    Vec3f anchorPos;
    TrailEmitter *emitter;
    TrailPoint *point;
    f32 invCount;
    f32 age;
    f32 ageStep;
    f32 diffX;
    f32 diffY;
    f32 diffZ;
    Vec3f pos;
    f32 stepX;
    f32 stepY;
    f32 stepZ;

    data = trail->data;
    points = trail->points;
    emitter = data->emitter;
    if (!(emitter->anchor.flags & 1)) {
        return 0;
    }
    anchorPos = emitter->anchor.pos;
    trail->pos = anchorPos;
    data->spawnAccum += 0.25f * D_800BE9A4;
    if (data->spawnAccum > 1.0f) {
        invCount = 1.0f / data->spawnAccum;
        age = data->unk10 + D_800BE9A4;
        pos = data->lastPos;
        diffX = anchorPos.x - data->lastPos.x;
        diffY = anchorPos.y - data->lastPos.y;
        diffZ = anchorPos.z - data->lastPos.z;
        ageStep = age * invCount;
        stepX = diffX * invCount;
        stepY = diffY * invCount;
        stepZ = diffZ * invCount;
        do {
            s32 pad; // matching: stack layout

            point = &points[trail->head];
            point->pos = pos;
            point->velY = 0.0f;
            point->length = 0.0f;
            point->alpha = 0;
            point->texU = 0.0f;
            func_151D8718(&point->pos, &point->velY, age);
            trail->head++;
            age -= ageStep;
            if (trail->head == trail->capacity) {
                trail->head = 0;
            }
            trail->count++;
            if (trail->head == trail->tail) {
                /* full: drop the oldest point */
                trail->tail++;
                if (trail->tail == trail->capacity) {
                    trail->tail = 0;
                }
                trail->count--;
            }
            pos.x += stepX;
            pos.y += stepY;
            pos.z += stepZ;
            data->spawnAccum -= 1.0f;
        } while (data->spawnAccum > 1.0f);
        data->lastPos = pos;
        data->unk10 = age;
    }
    return 1;
}

extern f32 func_15144528(f32, f32, f32);

/*
 * Shapes the trail, walking from the newest point to the oldest: limits its length to 80 (shortening
 * the last segment and dropping older points), scrolls the texture along it, and fades the alpha out
 * over the oldest 70%.
 */
s32 func_151D7CD0(Trail *trail) {
    TrailData *data;
    TrailPoint *points;
    f32 totalLength;
    TrailPoint *point;
    Vec3f *newerPos;
    Vec3f delta;
    s32 i;
    f32 cut;
    f32 pad; // matching: an unused local IDO still gives a stack slot
    f32 invLength;
    s32 j;
    TrailPoint *texPoint;
    f32 distance;
    s32 k;
    TrailPoint *fadePoint;
    f32 fadeDist;
    f32 opaqueLength;
    f32 fadeLength;
    f32 invFadeLength;

    data = trail->data;
    points = trail->points;
    if (trail->count >= 2) {
        totalLength = 0;
        i = trail->head;
        if (trail->flags & 2) {
            newerPos = &trail->pos;
        } else {
            i--;
            if (i < 0) {
                i = trail->capacity - 1;
            }
            newerPos = &points[i].pos;
        }
        do {
            f32 length;

            i--;
            if (i < 0) {
                i = trail->capacity - 1;
            }
            point = &points[i];
            delta.x = point->pos.x - newerPos->x;
            delta.y = point->pos.y - newerPos->y;
            delta.z = point->pos.z - newerPos->z;
            length = func_15143E64(&delta);
            totalLength += length;
            point->length = length;
            if (totalLength > 80.0f) {
                if (point->length != 0.0f) {
                    invLength = 1.0f / point->length;
                    cut = (totalLength - 80.0f) * invLength;
                    point->pos.x -= delta.x * cut;
                    point->pos.y -= delta.y * cut;
                    point->pos.z -= delta.z * cut;
                    point->length = point->length * (1.0f - cut);
                }
                totalLength = 80.0f;
                while (i != trail->tail) {
                    trail->tail++;
                    if (trail->tail == trail->capacity) {
                        trail->tail = 0;
                    }
                    trail->count--;
                }
            }
            newerPos = &point->pos;
        } while (i != trail->tail);
    }
    if (trail->count >= 2) {
        data->texPhase += -41.0f * D_800BE9A4;
        data->texPhase = func_15144528(data->texPhase, 16383.0f, -16384.0f);
        distance = 0;
        j = trail->head;
        do {
            j--;
            if (j < 0) {
                j = trail->capacity - 1;
            }
            texPoint = &points[j];
            distance += texPoint->length;
            texPoint->texU = func_15144528(data->texPhase + distance * 46.87999725f, 16383.0f, -16384.0f);
        } while (j != trail->tail);
    }
    if (trail->count >= 2) {
        fadeDist = 0;
        opaqueLength = totalLength * 0.2990000248f;
        fadeLength = totalLength - opaqueLength;
        invFadeLength = 1.0f / fadeLength;
        k = trail->head;
        do {
            f32 into;

            k--;
            if (k < 0) {
                k = trail->capacity - 1;
            }
            fadePoint = &points[k];
            if (opaqueLength < fadeDist) {
                s32 pad; // matching: stack layout

                into = fadeDist - opaqueLength;
                if (fadeLength < into) {
                    into = fadeLength;
                }
                fadePoint->alpha = (fadeLength - into) * invFadeLength * 60.0f;
            } else {
                fadePoint->alpha = 60;
            }
            fadeDist += fadePoint->length;
        } while (k != trail->tail);
    }
    return 1;
}

typedef struct {
    s32 unk0;
    s32 unk4;
} Mode151D80C4;
extern Mode151D80C4 D_800A4AC8[];
extern s32 D_800D2C9C;
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *needSync, s32 argA);
Gfx *func_15142FBC(Gfx *gfx, s32 modeH, s32 modeL, u8 *needSync);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);

extern u8 D_80090CD4[];

/*
 * Draws the trail as a ribbon 11 units wide, spread sideways by view's cos/sin (D_800DD1E8/D_800DD1D8), from
 * the head back to the oldest point. A fading trail scales the alpha by timer * 12.
 */
Gfx *func_151D80C4(Trail *trail, Gfx *gfx, s16 view) {
    TrailData *data;
    Vec3f pos;
    Vec3f newerPos;
    s32 i;
    s32 newest;
    TrailPoint *points;
    u8 needSync;
    u8 newerAlpha;
    u8 alpha;
    u8 fade;
    s32 newer;
    TrailPoint *point;
    f32 newerU;
    f32 u;
    f32 sideX;
    Vtx *vtx;
    f32 sideZ;

    if (trail->count < 2) {
        return gfx;
    }
    data = trail->data;
    points = trail->points;
    func_151D5D60(trail->unk84, view, (trail->capacity * 2 + 10) * sizeof(Vtx), (void **)&vtx, 0);
    if (vtx == NULL) {
        return gfx;
    }
    needSync = 1;
    gfx = func_15142E24(gfx, (s32)D_80090CD4, 0, 0, 0, 0, 0x1F, 0, 0, &needSync, 0x3E);
    gfx = func_15142B7C(gfx, 0x200005, 0x1F0600);
    gfx = func_1513F4E4(gfx, 0x4C, &needSync);
    gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[1].unk0 | D_800A4AC8[1].unk4, &needSync);
    if (trail->flags & 2) {
        i = trail->head - 1;
        if (i < 0) {
            i = trail->capacity - 1;
        }
        newerPos = trail->pos;
        newerAlpha = 0;
        newerU = data->texPhase;
    } else {
        newest = trail->head - 1;
        if (newest < 0) {
            newest = trail->capacity - 1;
        }
        i = newest - 1;
        if (i < 0) {
            i = trail->capacity - 1;
        }
        newerPos = points[newest].pos;
        newerAlpha = points[newest].alpha;
        newerU = points[newest].texU;
    }
    point = (TrailPoint *)((u8 *)points + i * sizeof(TrailPoint)); // matching: byte offset puts points first in the addu
    pos = point->pos;
    sideX = D_800DD1E8[view] * 5.5f;
    alpha = point->alpha;
    sideZ = D_800DD1D8[view] * 5.5f;
    u = point->texU;
    if (trail->flags & 8) {
        fade = trail->timer * 12;
    } else {
        fade = 0xFF;
    }
    newerAlpha = (newerAlpha * fade) >> 8;
    vtx->v.ob[0] = newerPos.x + sideX;
    vtx->v.ob[1] = newerPos.y;
    vtx->v.ob[2] = newerPos.z - sideZ;
    vtx->v.tc[0] = newerU;
    vtx->v.tc[1] = 0x800;
    vtx->v.cn[0] = 0xFF;
    vtx->v.cn[1] = 0xFF;
    vtx->v.cn[2] = 0xFF;
    vtx->v.cn[3] = newerAlpha;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = newerPos.x - sideX;
    vtx->v.ob[1] = newerPos.y;
    vtx->v.ob[2] = newerPos.z + sideZ;
    vtx->v.tc[0] = newerU;
    vtx->v.tc[1] = 0;
    vtx->v.cn[0] = 0xFF;
    vtx->v.cn[1] = 0xFF;
    vtx->v.cn[2] = 0xFF;
    vtx->v.cn[3] = newerAlpha;
    vtx->v.flag = 0;
    vtx++;
    do {
        alpha = (alpha * fade) >> 8;
        vtx->v.ob[0] = pos.x + sideX;
        vtx->v.ob[1] = pos.y;
        vtx->v.ob[2] = pos.z - sideZ;
        vtx->v.tc[0] = u;
        vtx->v.tc[1] = 0x800;
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = alpha;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = pos.x - sideX;
        vtx->v.ob[1] = pos.y;
        vtx->v.ob[2] = pos.z + sideZ;
        vtx->v.tc[0] = u;
        vtx->v.tc[1] = 0;
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0xFF;
        vtx->v.cn[2] = 0xFF;
        vtx->v.cn[3] = alpha;
        vtx->v.flag = 0;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 1, 3, 2, 0);
        if (u < newerU) {
            /* texU wrapped around: restart the strip from copies of the last two vertices, one period back */
            memcpy(vtx, vtx - 2, sizeof(Vtx) * 2);
            vtx[-2].v.tc[0] -= 0x8000;
            vtx++;
            vtx[-2].v.tc[0] -= 0x8000;
            vtx++;
        }
        newer = i;
        i--;
        if (i < 0) {
            i = trail->capacity - 1;
        }
        point = &points[i];
        pos = point->pos;
        alpha = point->alpha;
        newerU = points[newer].texU;
        u = point->texU;
    } while (newer != trail->tail);
    return gfx;
}

/* Lets a point fall for dt: integrates velY under gravity and moves pos->y. */
void func_151D8718(Vec3f *pos, f32 *velY, f32 dt) {
    f32 v = *velY;

    *velY += 0.2340000123f * dt;
    pos->y += v * dt + 0.1170000061f * (dt * dt);
}

/* Detaches the trail from its emitter. */
void func_151D8764(Trail *trail) {
    TrailData *data = trail->data;
    TrailEmitter *emitter = data->emitter;

    if (emitter != NULL) {
        emitter->anchor.trail = NULL;
    }
}

extern void func_151478F4(Trail *trail);

void func_151D8780(Trail *trail) {
    func_151D8764(trail);
    func_151478F4(trail);
}

extern void func_15147928(Trail *trail);

void func_151D87AC(Trail *trail) {
    func_151D8764(trail);
    func_15147928(trail);
}
