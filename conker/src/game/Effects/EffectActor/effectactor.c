#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The effect actor type's render setup and teardown display lists (object type table). */
Gfx gEffectActorRenderSetupDl[] = {
    gsDPPipeSync(),
    gsSPMatrix(&gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx gEffectActorRenderTeardownDl[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

/* Allocates an effect actor of extraSize type-specific bytes from a 0x44-byte template. */
EffectActor *effect_newEffectActor(EffectActorTemplate *template, s32 attachment, s32 arg2, u8 large, s32 extraSize, u8 arg5,
                           s32 arg6) {
    EffectActor *actor;
    s32 i;

    actor = effect_newObject(large ? 0x55 : 0x37, arg6, extraSize + 0xF8, 1, arg5, 1);
    if (actor == NULL) {
        return NULL;
    }
    memcpy(&actor->tmpl, template, sizeof(EffectActorTemplate));
    actor->attachment.model = attachment;
    actor->attachment.unk1C = arg2;
    actor->attachment.unk4 = 0;
    for (i = 0; i < 4; i++) {
        actor->attachment.handles[i] = NULL;
    }

    actor->attachment.sharedHandle = NULL;
    if (attachment) {
        for (i = 0; i <= gScreenLayout; i++) {
            actor->attachment.handles[i] = light_allocGfxLights(attachment);
        }
        actor->attachment.sharedHandle = func_1515D440();
    }
    return actor;
}

EffectActor *effect_newUnattachedEffectActor(EffectActorTemplate *template, u8 large, s32 extraSize, u8 arg3, s32 arg4) {
    return effect_newEffectActor(template, 0, 0, large, extraSize, arg3, arg4);
}

/* Update callbacks (EffectActorTemplate.updateCallback): each returns 0 when the actor is done. */
static s32 (*gEffectActorUpdateFuncs[])(EffectActor *) = { effect_moveEffectActor, effect_updateTumblingPiece, effect_fadeEffectActor };

/* Per-frame update: counts down the lifetime, runs the update callback, frees the actor when done. */
void effect_updateEffectActor(EffectActor *actor) {
    s32 callback;
    u8 expired;

    expired = 0;
    if (actor->tmpl.flags & 1) {
        actor->tmpl.lifetime -= D_800BE9E4;
        if (actor->tmpl.lifetime < 0) {
            expired = 1;
        }
    }
    if (expired == 0) {
        callback = actor->tmpl.updateCallback;
        if (callback != -1) {
            if (gEffectActorUpdateFuncs[callback](actor) == 0) {
                expired = 1;
            }
        }
    }
    if (expired) {
        effect_destroyObject(&actor->header);
    }
}

/* Matrix callbacks: build the actor's matrix for this frame. */
static s32 (*gEffectActorMatrixFuncs[])(Mtx *, EffectActor *) = { effect_buildEffectActorRotScaleMtx, effect_buildTumblingPieceMtx, effect_buildEffectActorScaleMtx };

/* Draws an effect actor: sets its render state and colors, then its matrix and display list. */
Gfx *effect_drawEffectActor(Gfx *gfx, EffectActor *actor, s16 arg2) {
    s32 pad; // matching: IDO's frame has an unused slot here
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;
    u8 update;

    update = 1;
    if (actor->tmpl.drawCallback != -1) {
        if (gEffectActorMatrixFuncs[actor->tmpl.drawCallback](&actor->mtx[gDrawBufferIndex], actor) == 0) {
            return gfx;
        }
    }
    gfx = math_gfxUpdateGeometryMode(gfx, actor->tmpl.render.setGeometryMode, actor->tmpl.render.clearGeometryMode);
    math_pickPrimColour(&primR, &primG, &primB, &primA, actor->tmpl.color1.red, actor->tmpl.color1.green, actor->tmpl.color1.blue, actor->tmpl.color1.alpha,
                  actor->tmpl.color0.red, actor->tmpl.color0.green, actor->tmpl.color0.blue, actor->tmpl.color0.alpha, 255, actor->tmpl.render.primMode);
    math_pickEnvColour(&envR, &envG, &envB, &envA, actor->tmpl.color1.red, actor->tmpl.color1.green, actor->tmpl.color1.blue, actor->tmpl.color1.alpha,
                  actor->tmpl.color0.red, actor->tmpl.color0.green, actor->tmpl.color0.blue, actor->tmpl.color0.alpha, 255, actor->tmpl.render.envMode);
    gfx = math_gfxSetEnvColor(gfx, envR, envG, envB, envA, &update);
    gfx = math_gfxSetPrimColor(gfx, 0, 0, primR, primG, primB, primA, &update);
    gfx = particle_setCombineMode(gfx, ((u8 *)&actor->tmpl.render.unk10)[3] /* matching: reads the low byte */, &update);
    gfx = math_gfxSetOtherMode(gfx, actor->tmpl.render.unk0 | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | actor->tmpl.render.unk14 | actor->tmpl.render.unk18,
                        gEffectRenderModes[actor->tmpl.render.renderMode].cycle1 | gEffectRenderModes[actor->tmpl.render.renderMode].cycle2, &update);
    if (actor->tmpl.flags & 2) {
        gfx = math_emitPointLights(gfx, &actor->attachment, actor->tmpl.attachMode, actor->tmpl.owner, actor->tmpl.ownerId, arg2,
                            &actor->tmpl.position, (actor->tmpl.flags & 4) ? 1 : 0, 0);
    }
    gSPMatrix(gfx++, &actor->mtx[gDrawBufferIndex], G_MTX_MODELVIEW | G_MTX_LOAD);
    gSPDisplayList(gfx++, gEffectActorDisplayLists[actor->tmpl.displayList]);
    return gfx;
}

/* Draw callback: rotates by data.motion.rot, scales uniformly and translates to the actor's position. */
s32 effect_buildEffectActorRotScaleMtx(Mtx *mtx, EffectActor *actor) {
    TumbleMotion *motion = &actor->data.motion;
    MtxF mf;

    math_mtxRotateRPYF(mf, motion->rot.x, motion->rot.y, motion->rot.z);
    mf[3][0] = actor->tmpl.position.x;
    mf[3][1] = actor->tmpl.position.y;
    mf[3][2] = actor->tmpl.position.z;
    mf[0][0] *= motion->scale;
    mf[0][1] *= motion->scale;
    mf[0][2] *= motion->scale;
    mf[1][0] *= motion->scale;
    mf[1][1] *= motion->scale;
    mf[1][2] *= motion->scale;
    mf[2][0] *= motion->scale;
    mf[2][1] *= motion->scale;
    mf[2][2] *= motion->scale;
    guMtxF2L(mf, mtx);
    return 1;
}

/* Update callback: damps horizontal velocity, applies gravity and integrates position and angle. */
s32 effect_moveEffectActor(EffectActor *actor) {
    TumbleMotion *motion;
    Vec3f prevVelocity;
    s32 i;

    motion = &actor->data.motion;
    prevVelocity = motion->vel;
    for (i = D_800BE9E4; i != 0; i--) {
        motion->vel.x *= motion->drag;
        motion->vel.z *= motion->drag;
    }
    {
        f32 accelX;
        f32 accelZ;
        f32 accelY;

        motion->vel.y += motion->gravity * gFrameDelta;
        accelX = (motion->vel.x - prevVelocity.x) * gFrameDeltaInv;
        accelY = motion->gravity;
        accelZ = (motion->vel.z - prevVelocity.z) * gFrameDeltaInv;
        actor->tmpl.position.x += (prevVelocity.x + 0.5f * accelX * gFrameDelta) * gFrameDelta;
        actor->tmpl.position.y += (prevVelocity.y + 0.5f * accelY * gFrameDelta) * gFrameDelta;
        actor->tmpl.position.z += (prevVelocity.z + 0.5f * accelZ * gFrameDelta) * gFrameDelta;
    }
    motion->rot.x += motion->rotVel.x * gFrameDelta;
    motion->rot.y += motion->rotVel.y * gFrameDelta;
    motion->rot.z += motion->rotVel.z * gFrameDelta;
    return 1;
}

/* A matrix wrapped in a struct so that it can be copied by assignment. */
typedef struct {
    MtxF m;
} MtxFBox;

/* Draw callback: an identity matrix scaled by data.size / 255 and translated to the actor's position. */
s32 effect_buildEffectActorScaleMtx(Mtx *mtx, EffectActor *actor) {
    f32 scale;
    s32 pad; // matching: IDO's frame has an unused slot here
    MtxFBox mf = { {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
    } };

    scale = actor->data.size * (1.0f / 255.0f);
    mf.m[3][0] = actor->tmpl.position.x;
    mf.m[3][1] = actor->tmpl.position.y;
    mf.m[3][2] = actor->tmpl.position.z;
    mf.m[0][0] *= scale;
    mf.m[0][1] *= scale;
    mf.m[0][2] *= scale;
    mf.m[1][0] *= scale;
    mf.m[1][1] *= scale;
    mf.m[1][2] *= scale;
    mf.m[2][0] *= scale;
    mf.m[2][1] *= scale;
    mf.m[2][2] *= scale;
    guMtxF2L(mf.m, mtx);
    return 1;
}

/* Releases the attachment's light buffers allocated by effect_newEffectActor. */
void effect_freeEffectActorLights(EffectActor *actor) {
    s32 i;

    void *handle; // matching: copied after each NULL test so IDO keeps the load in v0

    for (i = 0; i <= gScreenLayout; i++) {
        if (actor->attachment.handles[i] != NULL) {
            handle = actor->attachment.handles[i];
            heap_setTag(handle, 4);
        }
    }
    if (actor->attachment.sharedHandle != NULL) {
        handle = actor->attachment.sharedHandle;
        heap_setTag(handle, 4);
    }
}

void effect_destroyEffectActor(EffectActor *actor) {
    effect_freeEffectActorLights(actor);
    effect_defaultDestroy(&actor->header);
}

void effect_freeEffectActor(EffectActor *actor) {
    effect_freeEffectActorLights(actor);
    effect_defaultFree(&actor->header);
}

/* Update callback: once the lifetime drops below data.fade.duration, fades alpha out with it. */
s32 effect_fadeEffectActor(EffectActor *actor) {
    s32 alpha;

    if (actor->tmpl.lifetime < actor->data.fade.duration) {
        alpha = actor->tmpl.lifetime * actor->data.fade.alphaPerTick;
        if (alpha < actor->tmpl.color0.alpha) {
            actor->tmpl.color0.alpha = alpha;
        }
    }
    return 1;
}

/* Message handler: follows owner swaps (0x2D) and drops the owner when it goes away (0). */
void effect_effectActorMessage(EffectActor *actor, ActorMsg *msg, u8 msgId) {
    Actor *first;

    if (msgId == 0x2D) {
        first = msg->swap.actorA;
        if (actor->tmpl.owner == first) {
            actor->tmpl.owner = msg->swap.actorB;
            actor->tmpl.ownerId = msg->swap.idB;
        } else {
            if (actor->tmpl.owner == msg->swap.actorB) {
                actor->tmpl.owner = first;
                actor->tmpl.ownerId = msg->swap.idA;
            }
        dummy_label_356969: ; // matching: the empty labelled statement splits IDO's basic block
        }
    } else if (msgId == 0) {
        first = msg->ref.actor;
        if (actor->tmpl.owner != first) {
            if (msg->ref.uniqueId != actor->tmpl.ownerId) {
                return;
            }
        }
        actor->tmpl.owner = NULL;
        actor->tmpl.ownerId = 0;
    }
}
