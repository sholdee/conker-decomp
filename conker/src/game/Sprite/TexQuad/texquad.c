#include <ultra64.h>
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/sprite.h"
#include "conker/texture.h"
#include "libc/string.h"

/* not referenced: four s16 channel masks per row (all on, each channel off in turn, all off) */
const s16 D_800A6470[6][4] = {
    { 255, 255, 255, 255 }, { 0, 255, 255, 255 }, { 255, 0, 255, 255 },
    { 255, 255, 0, 255 },   { 255, 255, 255, 0 }, { 0, 0, 0, 0 },
};

/* Render setup and reset display lists of the textured quads; the quad type's entry in the object
 * type table D_8008B4D4 points to them. */
Gfx gTexQuadSetupDl[] = {
    gsDPPipeSync(),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx gTexQuadResetDl[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

/* Update and destroy callbacks of a quad (TexturedQuad.updateFunc, destroyFunc); none is defined. */
static s32 (*gTexQuadUpdateFuncs[])(TexturedQuad *) = { NULL };
static s32 (*gTexQuadDestroyFuncs[])(TexturedQuad *) = { NULL };

/*
 * Update handler: counts the lifetime down (flag 1), runs the update callback and advances the texture
 * animation. When the quad dies, the destroy callback (if any) decides whether it is freed.
 */
void sprite_updateTexQuad(TexturedQuad *quad) {
    s32 func;
    u8 dead;

    dead = FALSE;
    if (quad->flags & 1) {
        quad->lifetime -= gFrameTicks;
        if (quad->lifetime < 0) {
            dead = TRUE;
        }
    }
    if (!dead) {
        func = quad->updateFunc;
        if (func != -1) {
            if (gTexQuadUpdateFuncs[func](quad) == 0) {
                dead = TRUE;
            }
        }
        if (quad->frameSpeed != 0) {
            dead = math_advanceFrameSeq(quad->texture, &quad->frameSpeed, &quad->frame, quad->animFlags);
        }
    }
    if (dead) {
        func = quad->destroyFunc;
        if (func != -1) {
            if (gTexQuadDestroyFuncs[func](quad) != 0) {
                effect_destroyObject(&quad->header);
            }
        } else {
            effect_destroyObject(&quad->header);
        }
    }
}

/* Draw handler: sets up colours, texture and other modes, then draws the quad as two triangles. */
Gfx *sprite_drawTexQuad(Gfx *gfx, TexturedQuad *quad, s32 view) {
    s32 pad; // matching: unused stack slot in the original frame
    u8 needSync;
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;

    needSync = TRUE;
    math_pickPrimColour(&primR, &primG, &primB, &primA, quad->color0[0], quad->color0[1], quad->color0[2],
                  quad->color0[3], quad->color1[0], quad->color1[1], quad->color1[2], quad->color1[3],
                  quad->colorLevel, quad->primMode);
    math_pickEnvColour(&envR, &envG, &envB, &envA, quad->color0[0], quad->color0[1], quad->color0[2],
                  quad->color0[3], quad->color1[0], quad->color1[1], quad->color1[2], quad->color1[3],
                  quad->colorLevel, quad->envMode);
    gfx = math_gfxUpdateGeometryMode(gfx, quad->geometrySet, quad->geometryClear);
    gfx = math_gfxSetEnvColor(gfx, envR, envG, envB, envA, &needSync);
    gfx = math_gfxSetPrimColor(gfx, 0, 0, primR, primG, primB, primA, &needSync);
    gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[quad->texture], quad->frame, 0, 0, 0, quad->texture, 0, 0, &needSync, 3);
    gfx = particle_setCombineMode(gfx, quad->combiner, &needSync);
    gfx = math_gfxSetOtherMode(gfx,
                        G_PM_NPRIMITIVE | ((quad->flags & 2) ? G_CYC_2CYCLE : G_CYC_1CYCLE) | G_TP_PERSP |
                            G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE |
                            G_CD_NOISE | G_AD_NOISE,
                        gEffectRenderModes[quad->renderMode].cycle1 | gEffectRenderModes[quad->renderMode].cycle2, &needSync);
    gSPVertex(gfx++, quad->vtx, 4, 0);
    gSP1Triangle(gfx++, 0, 1, 2, 0);
    gSP1Triangle(gfx++, 0, 2, 3, 0);
    return gfx;
}

void func_1515BE48(void) {
}

/* Creates an actor tracker (object type 0x32) for desc->actor; returns NULL if there is no actor. */
ActorTracker *sprite_newActorTracker(ActorTrackerDesc *desc, s32 extraSize, u8 owner, s32 list) {
    ActorTracker *tracker;

    if (desc->actor == NULL) {
        return NULL;
    }
    tracker = effect_newObject(0x32, list, extraSize + 0x50, 1, owner, 1);
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
void sprite_destroyActorTracker(ActorTracker *tracker) {
    effect_detachHandle(tracker, tracker->desc.actor);
    effect_defaultDestroy(&tracker->header);
}

/* Free handler: detaches the tracker from its actor, then frees it. */
void sprite_freeActorTracker(ActorTracker *tracker) {
    effect_detachHandle(tracker, tracker->desc.actor);
    effect_defaultFree(&tracker->header);
}

/*
 * Update handler: follows the actor's position and records the motion since last frame. The tracker
 * dies when its lifetime runs out (flag 1) or the actor is gone or has been replaced.
 */
void sprite_updateActorTracker(ActorTracker *tracker) {
    s32 dead;
    Actor *actor;

    dead = FALSE;
    if (tracker->desc.flags & 1) {
        tracker->desc.lifetime -= gFrameTicks;
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
        effect_destroyObject(&tracker->header);
    }
}

/* Actor-event handler: dies with its actor and follows actor swaps (see effect_handleOwnerActorEvent). */
void sprite_actorTrackerMessage(ActorTracker *tracker, s32 event, u8 eventType) {
    effect_handleOwnerActorEvent(event, eventType, &tracker->desc.actor, &tracker->desc.actorId, tracker);
}

/* Finds `actor`'s tracker attachment (type 0x16); on success stores a pointer to its motion in *motion. */
s32 sprite_findActorMotion(Actor *actor, const Vec3f **motion) {
    Attachment *node;
    ActorTracker *tracker;

    if (actor == NULL) {
        return FALSE;
    }
    if (effect_findAttachmentByType(actor->attachments, 0x16, &node)) {
        tracker = node->handle;
        *motion = &tracker->motion;
        return TRUE;
    }
    return FALSE;
}

/* Resets unk44/unk48 of every actor tracker in both object lists. */
void sprite_resetActorTrackers(void) {
    s32 list;
    ActorTracker *tracker;

    for (list = 0; list < 2; list++) {
        for (tracker = (ActorTracker *)gObjectLists[list][0x32]; tracker != NULL; tracker = (ActorTracker *)tracker->header.next) {
            tracker->unk44 = 0;
            tracker->unk48 = -1;
        }
    }
}

/*
 * Gets an actor's collision bounds: for actor ids below 0xBB, radius hitRadius, *height hitHeight and a centre
 * raised by hitOffsetY; for other ids, 1.0 and 1.0 at the actor's position.
 */
void sprite_getActorBounds(Actor *actor, Vec3f *center, f32 *radius, f32 *height) {
    if ((actor->character_type < 0xBB) && (actor->character_type != 0xFF)) {
        *radius = actor->hitRadius;
        *height = actor->hitHeight;
        center->x = actor->xpos;
        center->y = actor->ypos + actor->hitOffsetY;
        center->z = actor->zpos;
    } else {
        *radius = 1.0f;
        *height = 1.0f;
        center->x = actor->xpos;
        center->y = actor->ypos;
        center->z = actor->zpos;
    }
}

/* Like sprite_getActorBounds, using the body cylinder (bodyRadius/bodyHeight/bodyOffsetY). */
void sprite_getActorBodyBounds(Actor *actor, Vec3f *center, f32 *radius, f32 *height) {
    if ((actor->character_type < 0xBB) && (actor->character_type != 0xFF)) {
        *radius = actor->bodyRadius;
        *height = actor->bodyHeight;
        center->x = actor->xpos;
        center->y = actor->ypos + actor->bodyOffsetY;
        center->z = actor->zpos;
    } else {
        *radius = 1.0f;
        *height = 1.0f;
        center->x = actor->xpos;
        center->y = actor->ypos;
        center->z = actor->zpos;
    }
}
