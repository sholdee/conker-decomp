#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* not referenced: four s16 channel masks per row (all on, each channel off in turn, all off) */
const s16 D_800A6470[6][4] = {
    { 255, 255, 255, 255 }, { 0, 255, 255, 255 }, { 255, 0, 255, 255 },
    { 255, 255, 0, 255 },   { 255, 255, 255, 0 }, { 0, 0, 0, 0 },
};

/* Render setup and reset display lists of the textured quads; the quad type's entry in the object
 * type table D_8008B4D4 points to them. */
Gfx D_8008B050[] = {
    gsDPPipeSync(),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx D_8008B068[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

/* Update and destroy callbacks of a quad (TexturedQuad.updateFunc, destroyFunc); none is defined. */
static s32 (*D_8008B078[])(TexturedQuad *) = { NULL };
static s32 (*D_8008B07C[])(TexturedQuad *) = { NULL };

extern RenderModePair D_800A4AC8[];

void *memcpy(void *, const void *, u32);

/*
 * Update handler: counts the lifetime down (flag 1), runs the update callback and advances the texture
 * animation. When the quad dies, the destroy callback (if any) decides whether it is freed.
 */
void func_1515BAE0(TexturedQuad *quad) {
    s32 func;
    u8 dead;

    dead = FALSE;
    if (quad->flags & 1) {
        quad->lifetime -= D_800BE9E4;
        if (quad->lifetime < 0) {
            dead = TRUE;
        }
    }
    if (!dead) {
        func = quad->updateFunc;
        if (func != -1) {
            if (D_8008B078[func](quad) == 0) {
                dead = TRUE;
            }
        }
        if (quad->frameSpeed != 0) {
            dead = func_1514401C(quad->texture, &quad->frameSpeed, &quad->frame, quad->animFlags);
        }
    }
    if (dead) {
        func = quad->destroyFunc;
        if (func != -1) {
            if (D_8008B07C[func](quad) != 0) {
                func_1516972C(&quad->header);
            }
        } else {
            func_1516972C(&quad->header);
        }
    }
}

/* Draw handler: sets up colours, texture and other modes, then draws the quad as two triangles. */
Gfx *func_1515BBF0(Gfx *gfx, TexturedQuad *quad, s32 view) {
    s32 pad; // matching: unused stack slot in the original frame
    u8 needSync;
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;

    needSync = TRUE;
    func_151441A4(&primR, &primG, &primB, &primA, quad->color0[0], quad->color0[1], quad->color0[2],
                  quad->color0[3], quad->color1[0], quad->color1[1], quad->color1[2], quad->color1[3],
                  quad->colorLevel, quad->primMode);
    func_151442FC(&envR, &envG, &envB, &envA, quad->color0[0], quad->color0[1], quad->color0[2],
                  quad->color0[3], quad->color1[0], quad->color1[1], quad->color1[2], quad->color1[3],
                  quad->colorLevel, quad->envMode);
    gfx = func_15142B7C(gfx, quad->geometrySet, quad->geometryClear);
    gfx = func_15142C10(gfx, envR, envG, envB, envA, &needSync);
    gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &needSync);
    gfx = func_15142E24(gfx, &D_80090B60[quad->texture], quad->frame, 0, 0, 0, quad->texture, 0, 0, &needSync, 3);
    gfx = func_1513F4E4(gfx, quad->combiner, &needSync);
    gfx = func_15142FBC(gfx,
                        G_PM_NPRIMITIVE | ((quad->flags & 2) ? G_CYC_2CYCLE : G_CYC_1CYCLE) | G_TP_PERSP |
                            G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE |
                            G_CD_NOISE | G_AD_NOISE,
                        D_800A4AC8[quad->renderMode].cycle1 | D_800A4AC8[quad->renderMode].cycle2, &needSync);
    gSPVertex(gfx++, quad->vtx, 4, 0);
    gSP1Triangle(gfx++, 0, 1, 2, 0);
    gSP1Triangle(gfx++, 0, 2, 3, 0);
    return gfx;
}

void func_1515BE48(void) {
}

/* Creates an actor tracker (object type 0x32) for desc->actor; returns NULL if there is no actor. */
ActorTracker *func_1515BE50(ActorTrackerDesc *desc, s32 extraSize, u8 owner, s32 list) {
    ActorTracker *tracker;

    if (desc->actor == NULL) {
        return NULL;
    }
    tracker = func_15167A68(0x32, list, extraSize + 0x50, 1, owner, 1);
    if (tracker == NULL) {
        return NULL;
    }
    memcpy(&tracker->desc, desc, sizeof(ActorTrackerDesc));
    tracker->pos.x = desc->actor->xpos;
    tracker->pos.y = desc->actor->ypos;
    tracker->pos.z = desc->actor->zpos;
    tracker->prevPos.x = desc->actor->xpos;
    tracker->prevPos.y = desc->actor->ypos;
    tracker->prevPos.z = desc->actor->zpos;
    tracker->unk44 = 0;
    tracker->unk48 = -1;
    tracker->unk10 = 1;
    tracker->unk14 = 0;
    tracker->motion.x = 0.0f;
    tracker->motion.y = 0.0f;
    tracker->motion.z = 0.0f;
    return tracker;
}

/* Destroy handler: detaches the tracker from its actor, then destroys it. */
void func_1515BF50(ActorTracker *tracker) {
    func_1514EDF0(tracker, tracker->desc.actor);
    func_15169804(&tracker->header);
}

/* Free handler: detaches the tracker from its actor, then frees it. */
void func_1515BF7C(ActorTracker *tracker) {
    func_1514EDF0(tracker, tracker->desc.actor);
    func_15169824(&tracker->header);
}

/*
 * Update handler: follows the actor's position and records the motion since last frame. The tracker
 * dies when its lifetime runs out (flag 1) or the actor is gone or has been replaced.
 */
void func_1515BFA8(ActorTracker *tracker) {
    s32 dead;
    Actor *actor;

    dead = FALSE;
    if (tracker->desc.flags & 1) {
        tracker->desc.lifetime -= D_800BE9E4;
        if (tracker->desc.lifetime < 0) {
            dead = TRUE;
        }
    }
    actor = tracker->desc.actor;
    if ((actor->type == 0) || (actor->unique_id != tracker->desc.actorId)) {
        dead = TRUE;
    }
    if (!dead) {
        tracker->prevPos = tracker->pos;
        tracker->pos.x = tracker->desc.actor->xpos;
        tracker->pos.y = tracker->desc.actor->ypos;
        tracker->pos.z = tracker->desc.actor->zpos;
        tracker->motion.x = (tracker->prevPos.x - tracker->pos.x) * gFrameDeltaInv;
        tracker->motion.y = (tracker->prevPos.y - tracker->pos.y) * gFrameDeltaInv;
        tracker->motion.z = (tracker->prevPos.z - tracker->pos.z) * gFrameDeltaInv;
    }
    if (dead) {
        func_1516972C(&tracker->header);
    }
}

/* Actor-event handler: dies with its actor and follows actor swaps (see func_15169850). */
void func_1515C0B8(ActorTracker *tracker, s32 event, u8 eventType) {
    func_15169850(event, eventType, &tracker->desc.actor, &tracker->desc.actorId, tracker);
}

/* Finds `actor`'s tracker attachment (type 0x16); on success stores a pointer to its motion in *motion. */
s32 func_1515C0F8(Actor *actor, const Vec3f **motion) {
    Attachment *node;
    ActorTracker *tracker;

    if (actor == NULL) {
        return FALSE;
    }
    if (func_1514ECE0(actor->attachments, 0x16, &node)) {
        tracker = node->handle;
        *motion = &tracker->motion;
        return TRUE;
    }
    return FALSE;
}

/* Resets unk44/unk48 of every actor tracker in both object lists. */
void func_1515C158(void) {
    s32 list;
    ActorTracker *tracker;

    for (list = 0; list < 2; list++) {
        for (tracker = (ActorTracker *)D_800DCE50[list][0x32]; tracker != NULL; tracker = (ActorTracker *)tracker->header.next) {
            tracker->unk44 = 0;
            tracker->unk48 = -1;
        }
    }
}

/*
 * Gets an actor's collision bounds: for actor ids below 0xBB, radius unkD2, *arg3 unkD4 and a centre
 * raised by unkD6; for other ids, 1.0 and 1.0 at the actor's position.
 */
void func_1515C1A0(Actor *actor, Vec3f *center, f32 *radius, f32 *arg3) {
    if ((actor->character_type < 0xBB) && (actor->character_type != 0xFF)) {
        *radius = actor->unkD2;
        *arg3 = actor->unkD4;
        center->x = actor->xpos;
        center->y = actor->ypos + actor->unkD6;
        center->z = actor->zpos;
    } else {
        *radius = 1.0f;
        *arg3 = 1.0f;
        center->x = actor->xpos;
        center->y = actor->ypos;
        center->z = actor->zpos;
    }
}

/* Like func_1515C1A0, using the body cylinder (bodyRadius/bodyHeight/bodyOffsetY). */
void func_1515C244(Actor *actor, Vec3f *center, f32 *radius, f32 *arg3) {
    if ((actor->character_type < 0xBB) && (actor->character_type != 0xFF)) {
        *radius = actor->bodyRadius;
        *arg3 = actor->bodyHeight;
        center->x = actor->xpos;
        center->y = actor->ypos + actor->bodyOffsetY;
        center->z = actor->zpos;
    } else {
        *radius = 1.0f;
        *arg3 = 1.0f;
        center->x = actor->xpos;
        center->y = actor->ypos;
        center->z = actor->zpos;
    }
}
