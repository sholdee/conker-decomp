#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Update handlers of a strip (StripObj.updateIdx); 0 removes the strip. */
static s32 (*gStripUpdateHandlers[])(StripObj *) = { beam_stripFollowTarget, beam_stripFollowTargetBone };

/* Render setup and reset display lists of the strips; the strip type's entry in the object type table
 * D_8008B4D4 points to them. */
Gfx gStripRenderSetupDl[] = {
    gsDPPipeSync(),
    gsSPMatrix(&gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetConvert(0, 0, 0, 0, 0, 255),
    gsSPEndDisplayList(),
};
Gfx gStripRenderResetDl[] = {
    gsDPPipeSync(),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetConvert(G_CV_K0, G_CV_K1, G_CV_K2, G_CV_K3, G_CV_K4, G_CV_K5),
    gsSPEndDisplayList(),
};

/* Destroy and free handlers per strip kind (StripObj.kind) */
static void (*gStripDestroyHandlers[])(StripObj *) = { beam_destroyStripDefault, beam_destroyStripDetach };
static void (*gStripFreeHandlers[])(StripObj *) = { beam_freeStripDefault, beam_freeStripDetach };
/* Message handlers per strip kind */
static void (*gStripMsgHandlers[])(StripObj *, ActorMsg *, u8) = { NULL, beam_stripTargetMessage };

/** Allocates a strip (0xC8-byte header plus `payloadSize` bytes of payload, its target) and caches its length. */
StripObj *beam_newStrip(StripDesc *desc, s32 payloadSize) {
    StripObj *strip;

    strip = effect_newObject(0x46, 0, payloadSize + 0xC8, 1, 0xFF, 1);
    if (strip == NULL) {
        return NULL;
    }
    memcpy(&strip->p0, desc, sizeof(StripDesc));
    {
        Vec3f diff;

        diff.x = strip->p0.x - strip->p1.x;
        diff.y = strip->p0.y - strip->p1.y;
        diff.z = strip->p0.z - strip->p1.z;
        strip->length = math_vec3Length(&diff);
    }
    strip->unk10 = 1;
    strip->unk14 = 0;
    return strip;
}

/** Destroy handler: hands the strip to effect_defaultDestroy. */
void beam_destroyStripDefault(StripObj *strip) {
    effect_defaultDestroy(&strip->header);
}

/** Free handler: hands the strip to effect_defaultFree. */
void beam_freeStripDefault(StripObj *strip) {
    effect_defaultFree(&strip->header);
}

/** Calls the gStripDestroyHandlers handler for the strip kind, or the first one when it is out of range. */
void beam_destroyStrip(StripObj *strip) {
    s32 idx = strip->kind;

    if (idx < 0) {
        idx = 0;
    } else if (idx >= 2) {
        idx = 0;
    }
    gStripDestroyHandlers[idx](strip);
}

/** Calls the gStripFreeHandlers handler for the strip kind, or the first one when it is out of range. */
void beam_freeStrip(StripObj *strip) {
    s32 idx = strip->kind;

    if (idx < 0) {
        idx = 0;
    } else if (idx >= 2) {
        idx = 0;
    }
    gStripFreeHandlers[idx](strip);
}

/** Detaches the strip from its target character, then destroys it (beam_destroyStripDefault). */
void beam_destroyStripDetach(StripObj *strip) {
    effect_detachHandle(strip, strip->target.obj);
    beam_destroyStripDefault(strip);
}

/** Detaches the strip from its target character, then frees it (beam_freeStripDefault). */
void beam_freeStripDetach(StripObj *strip) {
    effect_detachHandle(strip, strip->target.obj);
    beam_freeStripDefault(strip);
}

/** Runs the strip's update handler; removes the strip when it returns 0, else refreshes the length if it moved. */
void beam_updateStrip(StripObj *strip) {
    s32 idx;
    u8 remove;
    Vec3f prev;

    if (strip->unk10 & 1) {
        remove = FALSE;
        idx = strip->updateIdx;
        prev.x = strip->p0.x - strip->p1.x;
        prev.y = strip->p0.y - strip->p1.y;
        prev.z = strip->p0.z - strip->p1.z;

        if (idx != -1) {
            if (gStripUpdateHandlers[idx](strip) == 0) {
                remove = TRUE;
            }
        }
        if (!remove) {
            Vec3f diff;
            s32 pad; // matching: reserves the stack word the target frame has

            diff.x = strip->p0.x - strip->p1.x;
            diff.y = strip->p0.y - strip->p1.y;
            diff.z = strip->p0.z - strip->p1.z;
            if (diff.x != prev.x || diff.y != prev.y || diff.z != prev.z) {
                strip->length = math_vec3Length(&diff);
            }
        }
        if (remove) {
            effect_destroyObject(&strip->header);
        }
    }
}

extern s32 gEffectRenderModes[][2];

/** Draws the strip for viewport `view` as a quad from p0 to p1 widened sideways to face the camera. */
Gfx *beam_drawStrip(Gfx *gfx, StripObj *strip, s16 view) {
    s32 pad; // matching: unused stack word in the target frame
    u8 update;
    Vtx *vtx;
    Vec3f *eye;
    Vec3f dir;
    Vec3f toEye;
    Vec3f side;
    f32 lensq;
    f32 scale;
    s32 texV;

    if (strip->unk10 & 1) {
        // matching: dead reads/stores; they fix the order of IDO's FP spill slots
        side.z = dir.z;
        scale = dir.y * toEye.z - toEye.y * dir.z;
        lensq = dir.z * toEye.x - toEye.z * dir.x;
        toEye.z = dir.z = 0.0f;

        update = TRUE;
        vtx = strip->quad[gDrawBufferIndex];
        eye = &camera_global_Cameras[view].eye;
        gfx = math_gfxUpdateGeometryMode(gfx, 1, 0x600);
        gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[54], 0, 0, 0, 0, 0x36, 0, 0, &update, 3);
        if (camera_global_Cameras[view].SpecialFlags & 1) {
            gfx = math_gfxSetEnvColor(gfx, gTintEnvColors[view][0], gTintEnvColors[view][1], gTintEnvColors[view][2], 0xFF, &update);
            gfx = math_gfxSetPrimColor(gfx, 0xF2, 0, gTintPrimColors[view][0], gTintPrimColors[view][1], gTintPrimColors[view][2], 0xFF, &update);
            gfx = particle_setCombineMode(gfx, 30, &update);
        } else {
            gfx = math_gfxSetEnvColor(gfx, 0, 0, 0, 0, &update);
            gfx = math_gfxSetPrimColor(gfx, 0, 0, 0, 0, 0, 0, &update);
            gfx = particle_setCombineMode(gfx, 19, &update);
        }
        gfx = math_gfxSetOtherMode(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, gEffectRenderModes[1][0] | gEffectRenderModes[1][1], &update);

        scale = strip->p1.z;
        dir.x = strip->p1.x - strip->p0.x;
        dir.y = strip->p1.y - strip->p0.y;
        dir.z = strip->p1.z - strip->p0.z;
        toEye.x = strip->p1.x - eye->x;
        toEye.y = strip->p1.y - eye->y;
        toEye.z = strip->p1.z - eye->z;
        side.x = dir.y * toEye.z - toEye.y * dir.z;
        side.y = dir.z * toEye.x - toEye.z * dir.x;
        side.z = dir.x * toEye.y - toEye.x * dir.y;
        lensq = side.x * side.x + side.y * side.y + side.z * side.z;
        if (lensq == 0.0f) {
            side.x = 0.0f;
            side.y = 0.0f;
            side.z = 0.0f;
        } else {
            scale = 8.399999619f / sqrtf(lensq);
            side.x *= scale;
            side.y *= scale;
            side.z *= scale;
        }
        texV = strip->length * strip->texScale * 64.0f;
        vtx->v.ob[0] = strip->p1.x + side.x;
        vtx->v.ob[1] = strip->p1.y + side.y;
        vtx->v.ob[2] = strip->p1.z + side.z;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = 0;
        vtx++;
        vtx->v.ob[0] = strip->p1.x - side.x;
        vtx->v.ob[1] = strip->p1.y - side.y;
        vtx->v.ob[2] = strip->p1.z - side.z;
        vtx->v.tc[0] = 0x3C0;
        vtx->v.tc[1] = 0;
        vtx++;
        vtx->v.ob[0] = strip->p0.x - side.x;
        vtx->v.ob[1] = strip->p0.y - side.y;
        vtx->v.ob[2] = strip->p0.z - side.z;
        vtx->v.tc[0] = 0x3C0;
        vtx->v.tc[1] = texV;
        vtx++;
        vtx->v.ob[0] = strip->p0.x + side.x;
        vtx->v.ob[1] = strip->p0.y + side.y;
        vtx->v.ob[2] = strip->p0.z + side.z;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = texV;
        vtx++;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
    }
    return gfx;
}

/** Moves p1 to the target character's position; returns 0 once the character is gone. */
s32 beam_stripFollowTarget(StripObj *strip) {
    StripTarget *target = &strip->target;

    if (target->obj->type == 0) {
        return 0;
    }
    strip->p1.x = target->obj->xpos;
    strip->p1.y = target->obj->ypos;
    strip->p1.z = target->obj->zpos;
    return 1;
}

/**
 * Moves p1 to the target's bone-local offset transformed by its bone matrix, or to its position when it has
 * no matrices. Returns 0 once the character is gone or its slot was reused.
 */
s32 beam_stripFollowTargetBone(StripObj *strip) {
    StripTarget *target = &strip->target;
    MtxF *bones;

    if (target->obj->type == 0) {
        return 0;
    }
    if (target->obj->unique_id != target->uniqueId) {
        return 0;
    }
    bones = target->obj->ob_matrix_ptr;
    if (bones == NULL) {
        strip->p1.x = target->obj->xpos;
        strip->p1.y = target->obj->ypos;
        strip->p1.z = target->obj->zpos;
    } else {
        // matching: byte offset; &bones[target->bone] swaps the addu operands
        math_transformOffset(&target->offset, &strip->p1, (u8 *)bones + target->bone * sizeof(MtxF));
    }
    return 1;
}

/** Sends message 1 to `obj`'s type-0x15 attachments (its strips). */
void beam_sendStripMessage1(Actor *obj) {
    effect_sendAttachmentMessage(obj, 0x15, 1, 0);
}

/** Sends message 2 to `obj`'s type-0x15 attachments (its strips). */
void beam_sendStripMessage2(Actor *obj) {
    effect_sendAttachmentMessage(obj, 0x15, 2, 0);
}

/** Forwards a message to the gStripMsgHandlers handler for the strip kind, if there is one. */
void beam_stripMessage(StripObj *strip, ActorMsg *data, u8 msg) {
    void (*handler)(StripObj *, ActorMsg *, u8) = gStripMsgHandlers[strip->kind];

    if (handler != NULL) {
        handler(strip, data, msg);
    }
}

/**
 * Message 0: removes the strip if it targets data's character (same pointer or same unique id).
 * Message 0x2D: retargets the strip from either of data's two characters (A, B) to the other.
 */
void beam_stripTargetMessage(StripObj *strip, ActorMsg *data, u8 msg) {
    StripTarget *target = &strip->target;

    if (msg == 0) {
        if (data->ref.actor == target->obj || data->ref.uniqueId == target->uniqueId) {
            effect_destroyObject(&strip->header);
        }
    } else if (msg == 0x2D) {
        if (target->obj == data->swap.actorA) {
            target->obj = data->swap.actorB;
            target->uniqueId = data->swap.idB;
        } else if (target->obj == data->swap.actorB) {
            target->obj = data->swap.actorA;
            target->uniqueId = data->swap.idA;
        }
    }
}
