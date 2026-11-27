#include <ultra64.h>
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/sprite.h"
#include "conker/texture.h"
#include "conker/trail.h"

/*
 * Bone trails: an emitter object (a TimedEffect made by trail_createAnchoredEmitter) follows a point on an actor's
 * bone and drags a trail object (made by effect_newEffectObj) behind it. The trail keeps a ring buffer of
 * points that it renders as a textured ribbon.
 */

struct AnchoredTrail;

/* Per anchor type: the update that places the anchor, and the message handler. */
static s32 (*gAnchorUpdateProcs[])(AnchoredTrailEmitter *) = { trail_updateBoneAnchor };
static void (*gAnchorMessageProcs[])(AnchoredTrailEmitter *, TrailBoneLink *, u8) = { trail_onBoneAnchorMessage };
/* The extra checks of a bone link (TrailBoneLink.condition). */
static s32 (*gAnchorConditionProcs[])(AnchoredTrailEmitter *) = { func_151D7724, func_151D7770, trail_checkAnchorInWater };

/* Per TrailBoneLink.bone: the anchor point in the bone's space, and the bone matrix it is on. */
static const Vec3f gTrailBoneAnchors[7] = {
    { -21.0f, -12.0f, 32.0f }, { -22.0f, -4.0f, 16.0f }, { -8.0f, 24.0f, 2.0f }, { -24.0f, 16.0f, 18.0f },
    { 6.0f, 14.0f, 35.0f },    { 35.0f, 27.0f, 59.0f },  { -79.0f, 18.0f, 36.0f },
};
static const u8 gTrailBoneMatrices[7] = { 15, 17, 15, 2, 6, 6, 14 };

/* Creates an emitter of the given anchor type with `extraSize` more bytes of data after the TrailAnchor. */
TimedEffect *trail_createAnchoredEmitter(s16 timer, u8 flags, u8 anchorType, f32 maxStep, s32 extraSize, u8 owner, s32 list) {
    TimedEffect *emitter;
    TrailAnchor anchor;

    anchor.trail = NULL;
    anchor.anchorType = anchorType;
    anchor.flags = 0;
    anchor.pos.x = 0.0f;
    anchor.pos.y = 0.0f;
    anchor.pos.z = 0.0f;
    anchor.maxStep = maxStep;

    emitter = effect_newTimedEffect(timer, -1, 0x42, -1, flags, 0x36, (extraSize + sizeof(TrailAnchor)), owner, list);
    if (emitter != NULL) {
        memcpy(&((AnchoredTrailEmitter *)emitter)->anchor, &anchor, sizeof(TrailAnchor));
    }
    return emitter;
}

/*
 * Emitter update: moves the anchor, and removes the emitter once its anchor is gone. A trail is attached
 * while the anchor stays valid and moves less than maxStep per frame, and detached otherwise.
 */
void trail_updateAnchoredEmitter(AnchoredTrailEmitter *emitter) {
    TrailAnchor *anchor;
    Vec3f prevPos;
    u8 wasValid;
    Vec3f step;

    anchor = &emitter->anchor;
    prevPos = anchor->pos;
    wasValid = anchor->flags & 1;
    if (gAnchorUpdateProcs[anchor->anchorType](emitter) == 0) {
        emitter->base.header.state = -1;
        emitter->base.header.flags |= 1;
        return;
    }
    if (anchor->flags & 1) {
        if (wasValid) {
            step.x = anchor->pos.x - prevPos.x;
            step.y = anchor->pos.y - prevPos.y;
            step.z = anchor->pos.z - prevPos.z;
            if (math_vec3Length(&step) < anchor->maxStep) {
                if (anchor->trail == NULL) {
                    trail_spawnAnchoredTrail(emitter);
                }
            } else {
                trail_detachAnchoredTrail(emitter);
            }
        } else {
            trail_detachAnchoredTrail(emitter);
        }
    } else {
        trail_detachAnchoredTrail(emitter);
    }
}

/* Emitter message handler: forwards to the anchor type's handler. */
void trail_onAnchoredEmitterMessage(AnchoredTrailEmitter *emitter, TrailBoneLink *msg, u8 msgType) {
    if (gAnchorMessageProcs[emitter->anchor.anchorType] != NULL) {
        // matching: the original reloads anchorType for the call; IDO would reuse the first load
        gAnchorMessageProcs[*(volatile u8 *)&emitter->anchor.anchorType](emitter, msg, msgType);
    }
}

void trail_releaseAnchoredEmitter(AnchoredTrailEmitter *emitter) {
    trail_detachAnchoredTrail(emitter);
}

void trail_destroyAnchoredEmitter(AnchoredTrailEmitter *emitter) {
    trail_releaseAnchoredEmitter(emitter);
    effect_defaultDestroyTimedEffect(&emitter->base);
}

void trail_freeAnchoredEmitter(AnchoredTrailEmitter *emitter) {
    trail_releaseAnchoredEmitter(emitter);
    effect_defaultFreeTimedEffect(&emitter->base);
}

/* Broadcasts message 0x3D ({actor, unique_id}): the actor is going away. */
void trail_broadcastActorGone(Actor *actor) {
    ActorRef msg;

    msg.actor = actor;
    msg.uniqueId = actor->unique_id;
    effect_sendEventToTimedEffects((s32)&msg, 0x3D);
}

/* Attaches a trail to `bone` of `actor`; `condition` picks an extra gAnchorConditionProcs check (-1 for none). */
void trail_attachToBone(Actor *actor, u8 bone, s8 condition, u8 owner, s32 list) {
    TrailBoneLink link;
    TimedEffect *emitter;

    link.actor = actor;
    link.actorId = actor->unique_id;
    link.bone = bone;
    link.condition = condition;

    emitter = trail_createAnchoredEmitter(0x12C, 0, 0, 12.0f, sizeof(TrailBoneLink), owner, list);
    if (emitter != NULL) {
        memcpy(&((AnchoredTrailEmitter *)emitter)->link, &link, sizeof(TrailBoneLink));
    }
}

/* Anchor type 0 message handler: removes the emitter when its actor goes away (message 0x3D). */
void trail_onBoneAnchorMessage(AnchoredTrailEmitter *emitter, TrailBoneLink *msg, u8 msgType) {
    // matching: the original reloads emitter and msg from their stack slots (address taken / int reload)
    AnchoredTrailEmitter **pEmitter = &emitter;
    TrailBoneLink *link;

    if (msgType == 0x3D) {
        link = &(*pEmitter)->link;
        if ((msg->actor == link->actor) || (((TrailBoneLink *)*(s32 *)&msg)->actorId == link->actorId)) {
            effect_destroyObject(&(*pEmitter)->base.header);
        }
    } else {
        link = &(*pEmitter)->link;
        effect_handleTimedEffectOwnerEvent((s32)msg, msgType, &link->actor, &link->actorId, *pEmitter);
    }
}

/*
 * Anchor type 0 update: puts the anchor on the actor's bone. Fails (0) once the actor is gone; the
 * anchor is invalid while the actor has no bone matrices, rides something with a camera, or has
 * unk7 set.
 */
s32 trail_updateBoneAnchor(AnchoredTrailEmitter *emitter) {
    TrailBoneLink *link;
    Actor *actor;
    TrailAnchor *anchor;

    anchor = &emitter->anchor;
    link = &emitter->link;
    actor = link->actor;
    if ((actor->type == 0) || (actor->unique_id != link->actorId)) {
        return 0;
    }
    if (actor->ob_matrix_ptr == NULL) {
        anchor->flags &= ~1;
        return 1;
    }
    if ((actor->mv != NULL) && (actor->mv->SniperState != 0) && (actor->CameraPtr != NULL)) {
        anchor->flags &= ~1;
        return 1;
    }
    if (actor->alpha != 0xFF) {
        anchor->flags &= ~1;
        return 1;
    }
    math_transformOffset(&gTrailBoneAnchors[link->bone], &anchor->pos, &actor->ob_matrix_ptr[gTrailBoneMatrices[link->bone]]);
    anchor->flags |= 1;
    if (link->condition != -1) {
        return gAnchorConditionProcs[link->condition](emitter);
    }
    return 1;
}

/* Condition 0: the anchor is invalid while hiddenParts bit 1 is set or unk84 is 4, 10 or 12. */
s32 func_151D7724(AnchoredTrailEmitter *emitter) {
    Actor *actor = emitter->link.actor;
    TrailAnchor *anchor = &emitter->anchor;

    if ((actor->hiddenParts & 2) || actor->animseq.uh == 4 || actor->animseq.uh == 10 || actor->animseq.uh == 12) {
        anchor->flags &= ~1;
    }
    return 1;
}

/* Condition 1: the anchor is invalid while the actor's unk84 is 0. */
s32 func_151D7770(AnchoredTrailEmitter *emitter) {
    TrailAnchor *anchor = &emitter->anchor;
    Actor *actor = emitter->link.actor;

    if (actor->animseq.uh == 0) {
        anchor->flags &= ~1;
    }
    return 1;
}

/* Condition 2: the anchor is invalid while the actor is in water. */
s32 trail_checkAnchorInWater(AnchoredTrailEmitter *emitter) {
    TrailAnchor *anchor = &emitter->anchor;
    Actor *actor = emitter->link.actor;

    if (actor->inwater) {
        anchor->flags &= ~1;
    }
    return 1;
}

/* Detaches the emitter's trail, which then fades out. */
void trail_detachAnchoredTrail(AnchoredTrailEmitter *emitter) {
    TrailAnchor *anchor = &emitter->anchor;

    if (anchor->trail != NULL) {
        AnchoredTrailData *data = anchor->trail->data;

        anchor->trail->head.updateKind2 = 0;
        anchor->trail->head.tmpl.flags &= ~2;
        anchor->trail->head.tmpl.flags |= 8;
        anchor->trail->head.tmpl.flags |= 1;
        anchor->trail->head.tmpl.life = 20;
        data->emitter = NULL;
        anchor->trail = NULL;
    }
}

/* Spawns a 25-point trail at the anchor and attaches it to the emitter. */
void trail_spawnAnchoredTrail(AnchoredTrailEmitter *emitter) {
    AnchoredTrail *trail;
    AnchoredTrailData data;
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
    header.shapeKind = 4;
    header.material = 0;

    trail = effect_newEffectObj(&header, 0x20, sizeof(AnchoredTrailData), 0xD, 0x10, 0x10, 0, 0, 0, emitter->base.header.owner, emitter->base.header.kind);
    if (trail != NULL) {
        memcpy(trail->data, &data, sizeof(AnchoredTrailData));
        emitter->anchor.trail = trail;
    }
}

/* Detached trail update: the points keep falling and the tail position follows the oldest point. */
s32 trail_fallAnchoredTrail(AnchoredTrail *trail) {
    AnchoredTrailPoint *points;
    s32 i;

    points = trail->points;
    if ((trail->head.count < 2) && (trail->head.tmpl.flags & 8)) {
        return 0;
    }
    i = trail->head.end;
    while (i != trail->head.start) {
        i--;
        if (i < 0) {
            i = trail->head.tmpl.itemCount - 1;
        }
        trail_fallPoint(&points[i].pos, &points[i].velY, gFrameDelta);
    }
// matching: an (unused) label here makes IDO start a new block for the tail update
tail:
    if (trail->head.count > 0) {
        trail->head.modelPos = points[trail->head.start].pos;
    } else {
        trail->head.modelPos.x = 0.0f;
        trail->head.modelPos.y = 0.0f;
        trail->head.modelPos.z = 0.0f;
    }
    return 1;
}

/*
 * Attached trail update: the head follows the emitter, and points are emitted (4 per second of
 * gFrameDelta) along the way from the last emitted point, each pre-aged by the time since it was due.
 */
s32 trail_followAnchoredTrail(AnchoredTrail *trail) {
    AnchoredTrailData *data;
    AnchoredTrailPoint *points;
    Vec3f anchorPos;
    AnchoredTrailEmitter *emitter;
    AnchoredTrailPoint *point;
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
    trail->head.tmpl.pos = anchorPos;
    data->spawnAccum += 0.25f * gFrameDelta;
    if (data->spawnAccum > 1.0f) {
        invCount = 1.0f / data->spawnAccum;
        age = data->unk10 + gFrameDelta;
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

            point = &points[trail->head.end];
            point->pos = pos;
            point->velY = 0.0f;
            point->length = 0.0f;
            point->alpha = 0;
            point->texU = 0.0f;
            trail_fallPoint(&point->pos, &point->velY, age);
            trail->head.end++;
            age -= ageStep;
            if (trail->head.end == trail->head.tmpl.itemCount) {
                trail->head.end = 0;
            }
            trail->head.count++;
            if (trail->head.end == trail->head.start) {
                /* full: drop the oldest point */
                trail->head.start++;
                if (trail->head.start == trail->head.tmpl.itemCount) {
                    trail->head.start = 0;
                }
                trail->head.count--;
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

/*
 * Shapes the trail, walking from the newest point to the oldest: limits its length to 80 (shortening
 * the last segment and dropping older points), scrolls the texture along it, and fades the alpha out
 * over the oldest 70%.
 */
s32 trail_shapeAnchoredTrail(AnchoredTrail *trail) {
    AnchoredTrailData *data;
    AnchoredTrailPoint *points;
    f32 totalLength;
    AnchoredTrailPoint *point;
    Vec3f *newerPos;
    Vec3f delta;
    s32 i;
    f32 cut;
    f32 pad; // matching: an unused local IDO still gives a stack slot
    f32 invLength;
    s32 j;
    AnchoredTrailPoint *texPoint;
    f32 distance;
    s32 k;
    AnchoredTrailPoint *fadePoint;
    f32 fadeDist;
    f32 opaqueLength;
    f32 fadeLength;
    f32 invFadeLength;

    data = trail->data;
    points = trail->points;
    if (trail->head.count >= 2) {
        totalLength = 0;
        i = trail->head.end;
        if (trail->head.tmpl.flags & 2) {
            newerPos = &trail->head.tmpl.pos;
        } else {
            i--;
            if (i < 0) {
                i = trail->head.tmpl.itemCount - 1;
            }
            newerPos = &points[i].pos;
        }
        do {
            f32 length;

            i--;
            if (i < 0) {
                i = trail->head.tmpl.itemCount - 1;
            }
            point = &points[i];
            delta.x = point->pos.x - newerPos->x;
            delta.y = point->pos.y - newerPos->y;
            delta.z = point->pos.z - newerPos->z;
            length = math_vec3Length(&delta);
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
                while (i != trail->head.start) {
                    trail->head.start++;
                    if (trail->head.start == trail->head.tmpl.itemCount) {
                        trail->head.start = 0;
                    }
                    trail->head.count--;
                }
            }
            newerPos = &point->pos;
        } while (i != trail->head.start);
    }
    if (trail->head.count >= 2) {
        data->texPhase += -41.0f * gFrameDelta;
        data->texPhase = math_wrapFloat(data->texPhase, 16383.0f, -16384.0f);
        distance = 0;
        j = trail->head.end;
        do {
            j--;
            if (j < 0) {
                j = trail->head.tmpl.itemCount - 1;
            }
            texPoint = &points[j];
            distance += texPoint->length;
            texPoint->texU = math_wrapFloat(data->texPhase + distance * 46.87999725f, 16383.0f, -16384.0f);
        } while (j != trail->head.start);
    }
    if (trail->head.count >= 2) {
        fadeDist = 0;
        opaqueLength = totalLength * 0.2990000248f;
        fadeLength = totalLength - opaqueLength;
        invFadeLength = 1.0f / fadeLength;
        k = trail->head.end;
        do {
            f32 into;

            k--;
            if (k < 0) {
                k = trail->head.tmpl.itemCount - 1;
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
        } while (k != trail->head.start);
    }
    return 1;
}

/*
 * Draws the trail as a ribbon 11 units wide, spread sideways by view's cos/sin (gCameraAngleCosines/gCameraAngleSines), from
 * the head back to the oldest point. A fading trail scales the alpha by timer * 12.
 */
Gfx *trail_drawAnchoredTrail(AnchoredTrail *trail, Gfx *gfx, s16 view) {
    AnchoredTrailData *data;
    Vec3f pos;
    Vec3f newerPos;
    s32 i;
    s32 newest;
    AnchoredTrailPoint *points;
    u8 needSync;
    u8 newerAlpha;
    u8 alpha;
    u8 fade;
    s32 newer;
    AnchoredTrailPoint *point;
    f32 newerU;
    f32 u;
    f32 sideX;
    Vtx *vtx;
    f32 sideZ;

    if (trail->head.count < 2) {
        return gfx;
    }
    data = trail->data;
    points = trail->points;
    particle_getViewBuffer(trail->vtxBuf, view, (trail->head.tmpl.itemCount * 2 + 10) * sizeof(Vtx), &vtx, 0);
    if (vtx == NULL) {
        return gfx;
    }
    needSync = 1;
    gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[31], 0, 0, 0, 0, 0x1F, 0, 0, &needSync, 0x3E);
    gfx = math_gfxUpdateGeometryMode(gfx, 0x200005, 0x1F0600);
    gfx = particle_setCombineMode(gfx, 0x4C, &needSync);
    gfx = math_gfxSetOtherMode(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, gEffectRenderModes[1].cycle1 | gEffectRenderModes[1].cycle2, &needSync);
    if (trail->head.tmpl.flags & 2) {
        i = trail->head.end - 1;
        if (i < 0) {
            i = trail->head.tmpl.itemCount - 1;
        }
        newerPos = trail->head.tmpl.pos;
        newerAlpha = 0;
        newerU = data->texPhase;
    } else {
        newest = trail->head.end - 1;
        if (newest < 0) {
            newest = trail->head.tmpl.itemCount - 1;
        }
        i = newest - 1;
        if (i < 0) {
            i = trail->head.tmpl.itemCount - 1;
        }
        newerPos = points[newest].pos;
        newerAlpha = points[newest].alpha;
        newerU = points[newest].texU;
    }
    point = (AnchoredTrailPoint *)((u8 *)points + i * sizeof(AnchoredTrailPoint)); // matching: byte offset puts points first in the addu
    pos = point->pos;
    sideX = gCameraAngleCosines[view] * 5.5f;
    alpha = point->alpha;
    sideZ = gCameraAngleSines[view] * 5.5f;
    u = point->texU;
    if (trail->head.tmpl.flags & 8) {
        fade = trail->head.tmpl.life * 12;
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
            i = trail->head.tmpl.itemCount - 1;
        }
        point = &points[i];
        pos = point->pos;
        alpha = point->alpha;
        newerU = points[newer].texU;
        u = point->texU;
    } while (newer != trail->head.start);
    return gfx;
}

/* Lets a point fall for dt: integrates velY under gravity and moves pos->y. */
void trail_fallPoint(Vec3f *pos, f32 *velY, f32 dt) {
    f32 v = *velY;

    *velY += 0.2340000123f * dt;
    pos->y += v * dt + 0.1170000061f * (dt * dt);
}

/* Detaches the trail from its emitter. */
void trail_detachFromEmitter(AnchoredTrail *trail) {
    AnchoredTrailData *data = trail->data;
    AnchoredTrailEmitter *emitter = data->emitter;

    if (emitter != NULL) {
        emitter->anchor.trail = NULL;
    }
}

void trail_destroyAnchoredTrail(AnchoredTrail *trail) {
    trail_detachFromEmitter(trail);
    effect_defaultDestroyEffectObj(trail);
}

void trail_freeAnchoredTrail(AnchoredTrail *trail) {
    trail_detachFromEmitter(trail);
    effect_defaultFreeEffectObj(trail);
}
