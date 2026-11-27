#include <ultra64.h>
#include "conker/collision.h"
#include "conker/flare.h"
#include "conker/particle.h"
#include "conker/sight.h"

/* Emitter handlers run by particle_followBoneGlow before it follows the bone. Returning 0 ends the emitter's
 * update (particle_followBoneGlow returns 0); setting *follow to FALSE skips this frame's bone follow. */
/* Declared without a prototype: func_151C9B64 takes its SightStatusObject. */
static s32 (*gBoneGlowFollowFuncs[])() = { func_1518A214, particle_hideBoneGlowWhileEyeClosed, particle_hideBoneGlowWithoutExpression, func_151C9B64 };

/* Spawns an emitter (type 0xB) that follows the bone described by `attach`. */
BoneGlow *particle_spawnBoneGlow(ParticleDef *spawn, GlowShape *params, EmitterAttach *attach, u8 arg3, u8 arg4,
                       u8 arg5, u8 arg6) {
    BoneGlow *emitter;

    params->type = 0xB;
    emitter = flare_spawnGlow(params, 0x80, spawn, arg3, arg4, arg5, arg6, -1, 0xFF, 1);
    if (emitter != NULL) {
        EmitterAttach *dst = &emitter->attach;

        memcpy(dst, attach, sizeof(EmitterAttach));
    } else { // matching: an early `if (emitter == NULL) return NULL;` changes the branch layout
        return NULL;
    }
    return emitter;
}

/* Moves the emitter's p0/p1 onto its target's bone. Returns 0 once the target is gone. */
s32 particle_followBoneGlow(BoneGlow *emitter) {
    EmitterAttach *attach;

    attach = &emitter->attach;
    if (attach->target == NULL) {
        attach->target = collision_findObjectById(attach->targetId);
        if (attach->target == NULL) {
            return 0;
        }
    }
    if ((attach->target->type != 0) && (attach->target->unique_id == attach->targetId)) {
        if (attach->handler != -1) {
            u8 follow;

            if (gBoneGlowFollowFuncs[attach->handler](emitter, &follow)) {
                if (!follow) {
                    return 1;
                }
            } else { // matching: early-return form reorders the branches
                return 0;
            }
        }
        if (attach->target->ob_matrix_ptr != NULL) {
            f32 (*mtx)[4];
            Vec3f *out;

            mtx = attach->target->ob_matrix_ptr[attach->bone];
            out = &emitter->head.def.pos;
            math_transformPoint(mtx, attach->p0.x, attach->p0.y, attach->p0.z, &out->x, &out->y, &out->z);
            out = &emitter->head.def.rot;
            math_transformPoint(mtx, attach->p1.x, attach->p1.y, attach->p1.z, &out->x, &out->y, &out->z);
            emitter->head.def.flags |= 2;
            emitter->head.def.flags &= ~4;
        } else {
            emitter->head.def.flags &= ~2;
        }
    } else {
        return 0;
    }
    return 1;
}

/* Handler: sets params unk18/unk1C and the color from the target's byte 0x68; 15 clears flag 2 and stops. */
s32 func_1518A214(BoneGlow *emitter, u8 *follow) {
    GlowShape *params;
    f32 value;

    params = &emitter->params;
    switch (emitter->attach.target->unk68[0] - 15) {
    default: // < 15 or > 17
        value = 0.6f;
        params->alpha1 = value;
        params->alpha0 = value;
        emitter->head.def.rgba[2] = 0;
        emitter->head.def.rgba[1] = 0;
        emitter->head.def.rgba[0] = 0xFF;
        *follow = TRUE;
        break;
    case 0: // 15
        emitter->head.def.flags &= ~2;
        *follow = FALSE;
        break;
    case 1: // 16
        value = 0.3f;
        params->alpha1 = value;
        params->alpha0 = value;
        emitter->head.def.rgba[0] = 0x80;
        emitter->head.def.rgba[1] = 0;
        emitter->head.def.rgba[2] = 0;
        *follow = TRUE;
        break;
    case 2: // 17
        value = 0.6f;
        params->alpha1 = value;
        params->alpha0 = value;
        emitter->head.def.rgba[0] = 0xFF;
        emitter->head.def.rgba[1] = 0;
        emitter->head.def.rgba[2] = 0;
        *follow = TRUE;
        break;
    }
    return 1;
}

/* Handler: while the target's byte 0x6A is set, clears flag 2 and stops. */
s32 particle_hideBoneGlowWhileEyeClosed(BoneGlow *emitter, u8 *follow) {
    if (emitter->attach.target->eyeClosed[0] != 0) {
        emitter->head.def.flags &= ~2;
        *follow = FALSE;
    } else {
        *follow = TRUE;
    }
    return 1;
}

/* Handler: once the target's byte 0x6F is clear, clears flag 2 and stops. */
s32 particle_hideBoneGlowWithoutExpression(BoneGlow *emitter, u8 *follow) {
    if (emitter->attach.target->expression == 0) {
        emitter->head.def.flags &= ~2;
        *follow = FALSE;
    } else {
        *follow = TRUE;
    }
    return 1;
}

/* Message handler: message 0x2D moves the emitter from one object of the pair to the other. */
void particle_onBoneGlowSwap(BoneGlow *emitter, ActorSwapMsg *msg, u8 msgId) {
    EmitterAttach *attach = &emitter->attach;

    if (msgId == 0x2D) {
        if (attach->target == msg->actorA) {
            attach->target = msg->actorB;
            attach->targetId = msg->idB;
        } else if (attach->target == msg->actorB) {
            attach->target = msg->actorA;
            attach->targetId = msg->idA;
        }
    }
}
