#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Animated, textured quad (update func_1515BAE0, draw func_1515BBF0). */
typedef struct TexturedQuad {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 texture;          // index into D_80090B60
    /* 0x11 */ u8 flags;            // 1: lifetime counts down, 2: two-cycle mode
    /* 0x12 */ u8 animFlags;        // end-of-animation behaviour, see func_1514401C
    /* 0x13 */ u8 pad13;
    /* 0x14 */ s16 lifetime;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 frame;           // 16.16 frame position
    /* 0x1C */ u32 frameSpeed;
    /* 0x20 */ u8 color0[4];
    /* 0x24 */ u8 color1[4];
    /* 0x28 */ u8 colorLevel;
    /* 0x29 */ u8 primMode;
    /* 0x2A */ u8 envMode;
    /* 0x2B */ u8 renderMode;       // index into D_800A4AC8
    /* 0x2C */ u8 combiner;
    /* 0x2D */ u8 pad2D[3];
    /* 0x30 */ s32 geometrySet;
    /* 0x34 */ s32 geometryClear;
    /* 0x38 */ s8 updateFunc;       // index into D_8008B078, -1 = none
    /* 0x39 */ s8 destroyFunc;      // index into D_8008B07C, -1 = none
    /* 0x3A */ u8 pad3A[0x16];
    /* 0x50 */ Vtx vtx[4];
} TexturedQuad;

/* What an actor tracker is created from (func_1514EE70 builds one). */
typedef struct ActorTrackerDesc {
    /* 0x0 */ Actor *actor;
    /* 0x4 */ u8 actorId;           // the actor's unique_id
    /* 0x5 */ u8 flags;             // 1: lifetime counts down
    /* 0x6 */ s16 lifetime;
} ActorTrackerDesc;

/* Object type 0x32: follows an actor's position every frame and keeps its frame-to-frame motion. */
typedef struct ActorTracker {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ ActorTrackerDesc desc;
    /* 0x20 */ Vec3f pos;
    /* 0x2C */ Vec3f prevPos;
    /* 0x38 */ Vec3f motion;        // (prevPos - pos) * D_800BE9A8
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
} ActorTracker;

extern s32 (*D_8008B078[])(TexturedQuad *);
extern s32 (*D_8008B07C[])(TexturedQuad *);
extern EffectTexture D_80090B60[];
extern RenderModePair D_800A4AC8[];

s32 func_1514401C(u8 id, u32 *speed, s32 *pos, u8 flags);
Gfx *func_15142B7C(Gfx *gfx, s32 setBits, s32 clearBits);
Gfx *func_15142C10(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142CF0(Gfx *gfx, s32 minLevel, s32 lodFrac, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142E24(Gfx *gfx, EffectTexture *tex, s32 frame, s32 arg3, s32 arg4, s32 arg5, s32 texture, u8 arg7, s32 arg8,
                   u8 *needSync, s32 argA);
Gfx *func_15142FBC(Gfx *gfx, s32 modeH, s32 modeL, u8 *needSync);
void func_151441A4(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, s32 a0, u8 r1, u8 g1, u8 b1,
                   u8 a1, u8 weight, u8 mode);
void func_151442FC(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1,
                   u8 a1, u8 weight, u8 mode);
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
        tracker->motion.x = (tracker->prevPos.x - tracker->pos.x) * D_800BE9A8;
        tracker->motion.y = (tracker->prevPos.y - tracker->pos.y) * D_800BE9A8;
        tracker->motion.z = (tracker->prevPos.z - tracker->pos.z) * D_800BE9A8;
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
s32 func_1515C0F8(Actor *actor, Vec3f **motion) {
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
