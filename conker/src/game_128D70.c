#include <ultra64.h>
#include "functions.h"
#include "variables.h"




/* First argument of func_1515572C. */
typedef struct {
    /* 0x00 */ u8 side;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Camera *camera;
} CameraArg_150FB8C0;

extern u8 D_800A1DD0[], D_800A1E34[];
extern f32 D_800A1DD4[][3], D_800A1DEC[][3], D_800A1E04[][3], D_800A1E1C[][3], D_800A1E38[][3], D_800A1E50[][3], D_800A1E68[][3], D_800A1E80[][3];

/* Spawns the effect for one side (0 or 1) of `obj`. The side's points are transformed by one of obj's
 * ob_matrix_ptr matrices; when the target found by func_1507515C has unk197 set, the effect is aimed at the point
 * where the target's line meets the plane through the transformed points. Also spawns occasional sparks
 * and trails, and a camera effect for the target's camera. */
void func_150FB8C0(Actor *obj, u8 side, f32 timeOffset, u8 skipTarget, u8 arg4, s32 arg5) {
    Actor *target;
    u8 useTarget;
    f32 origin[3];
    f32 dir[3];
    f32 hitPoint[3];
    f32 lineDir[3];
    f32 edgeA[3];
    f32 edgeB[3];
    f32 *srcPoints[3];
    f32 *dstPoints[3];
    s32 matrix;
    f32 localPoint[3];
    Vec3f normal;
    f32 t;
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f *linePoint;
    u8 cameraMask;
    s8 variant;
    LightEffectHeader spark;
    s32 sparkPos[3];
    f32 trailParams[2];
    u8 trailCameraMask;
    ScreenSpriteInfo effect;
    CameraArg_150FB8C0 cameraArg;

    // matching: the always-false side < 0 test is needed
    if (obj == NULL || side < 0 || side >= 2) {
        return;
    }
    if (skipTarget) {
        target = NULL;
        useTarget = 0;
    } else {
        target = func_1507515C(obj);
        if (target == NULL) {
            return;
        }
        if (target->mv == NULL) {
            return;
        }
        if (target->mv->SniperState != 0) {
            useTarget = 1;
        } else {
            useTarget = 0;
        }
    }
    if (obj->ob_matrix_ptr == NULL) {
        return;
    }
    if (obj->character_type == 0x23) {
        srcPoints[0] = D_800A1E38[side];
        if (useTarget) {
            srcPoints[1] = D_800A1E68[side];
        } else {
            srcPoints[1] = D_800A1E50[side];
        }
        srcPoints[2] = D_800A1E80[side];
        matrix = (D_800A1E34[side] << 6) + (s32)obj->ob_matrix_ptr;
    } else {
        localPoint[0] = (13.0f - (1.0f + func_150ADA68() * 6.0f)) * (side ? -1 : 1);
        localPoint[1] = D_800A1DEC[side][1];
        localPoint[2] = D_800A1DEC[side][2];
        srcPoints[0] = D_800A1DD4[side];
        srcPoints[1] = useTarget ? D_800A1E04[side] : localPoint;
        srcPoints[2] = D_800A1E1C[side];
        matrix = (D_800A1DD0[side] << 6) + (s32)obj->ob_matrix_ptr;
    }
    dstPoints[0] = origin;
    if (useTarget) {
        dstPoints[1] = edgeA;
    } else {
        dstPoints[1] = dir;
    }
    dstPoints[2] = edgeB;
    func_15145EA4(srcPoints, dstPoints, matrix, useTarget ? 3 : 2);
    if (useTarget) {
        // matching: a declaration here opens a -g3 scope block (scheduling)
        extern void func_151450B4(Vec3f *, Vec3f *, Vec3f *);

        linePoint = &target->mv->aimEye;
        edgeA[0] -= origin[0];
        edgeA[1] -= origin[1];
        edgeA[2] -= origin[2];
        edgeB[0] -= origin[0];
        edgeB[1] -= origin[1];
        edgeB[2] -= origin[2];
        func_151450B4((Vec3f *)edgeA, (Vec3f *)edgeB, &normal);
        dx = origin[0] - linePoint->x;
        dy = origin[1] - linePoint->y;
        dz = origin[2] - linePoint->z;
        t = ((normal.x * dx) + (normal.y * dy) + (normal.z * dz)) /
            ((normal.x * target->mv->aimDir.x) + (normal.y * target->mv->aimDir.y) + (normal.z * target->mv->aimDir.z));
        hitPoint[0] = linePoint->x + (t * target->mv->aimDir.x);
        hitPoint[1] = linePoint->y + (t * target->mv->aimDir.y);
        hitPoint[2] = linePoint->z + (t * target->mv->aimDir.z);
        lineDir[0] = target->mv->aimDir.x;
        lineDir[1] = target->mv->aimDir.y;
        lineDir[2] = target->mv->aimDir.z;
    } else {
        dir[0] -= origin[0];
        dir[1] -= origin[1];
        dir[2] -= origin[2];
    }
    if (!useTarget || target->CameraPtr == NULL) {
        cameraMask = 0xFF;
    } else {
        cameraMask = ~(1 << target->CameraPtr->CNumber) & 0xFF;
    }
    if (level == 0x34 || level == 0x26) {
        variant = 5;
    } else {
        variant = 4;
    }
    func_151C229C(origin, useTarget ? NULL : dir, useTarget ? hitPoint : NULL, useTarget ? lineDir : NULL, useTarget ? 1 : 0, 0,
                  252.0f, 0.003968254197f, (func_150ADA68() * 20.0f) + 75.0f, (func_150ADA68() * 250.0f) + 502.0f, 50.0f,
                  (func_150ADA20() % 31U) + 225, target != NULL ? target : obj, 1, 1, D_800BE616 != 0 ? 0 : 1, 50, 0, 1, variant, 44, timeOffset,
                  cameraMask, 0, 0, arg4, arg5);
    if ((obj->unk74 & 0xF) != 0xF) {
        if (!(func_150ADA20() & 7)) {
            spark.flags = LIGHT_EFFECT_TIMED | LIGHT_EFFECT_OWNS_LIGHT;
            spark.update = -1;
            spark.lifetime = (func_150ADA20() % 5U) + 4;
            spark.lightType = 0;
            sparkPos[0] = (s32)origin[0];
            sparkPos[1] = (s32)origin[1];
            sparkPos[2] = (s32)origin[2];
            func_151602C0(&spark, sparkPos, (func_150ADA20() % 31U) + 70, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg4, arg5);
        }
        if (!(func_150ADA20() & 1)) {
            trailParams[1] = (func_150ADA68() * 2.435000181f) + 3.534000158f;
            trailParams[0] = (func_150ADA68() * 43.40000153f) + 48.40000153f;
            if (!useTarget || target->CameraPtr == NULL) {
                trailCameraMask = 0xFF;
            } else {
                trailCameraMask = ~(1 << target->CameraPtr->CNumber) & 0xFF;
            }
            func_15102B38(obj, (obj->character_type == 0x23) ? D_800A1E34[side] : D_800A1DD0[side], (obj->character_type == 0x23) ? D_800A1E38[side] : D_800A1DD4[side],
                          (obj->character_type == 0x23) ? D_800A1E50[side] : D_800A1DEC[side], trailParams,
                          (func_150ADA20() % 3U) + 4, (func_150ADA20() % 101U) + 155, (func_150ADA68() * 1012.0f) + 1497.0f, origin,
                          trailCameraMask, 0, -1, arg4, arg5);
        }
    }
    if (useTarget && target->CameraPtr != NULL) {
        if (side) {
            effect.x = -59.0f;
        } else {
            effect.x = 59.0f;
        }
        effect.y = 68.0f;
        effect.halfWidth = (func_150ADA68() * 20.0f) + 40.0f;
        effect.halfHeight = (func_150ADA68() * 20.0f) + 80.0f;
        effect.texture = 0x80;
        effect.timer = (func_150ADA20() & 3) + 5;
        // matching: the redundant (s16) and & 0xFFFF narrowing shifts the t-register allocation
        effect.flags = (s16)((side ? 2 : 0) | 0x39) & 0xFFFF;
        effect.fadeStart = 3;
        effect.fadeRate = 85;
        effect.kind = 0;
        effect.color1.red = 0xFF;
        effect.color1.green = 0xFF;
        effect.color1.blue = 0xFF;
        effect.color1.alpha = (func_150ADA20() % 56U) + 200;
        effect.color2.red = 0xFF;
        effect.color2.green = 0xFF;
        effect.color2.blue = 0xFF;
        effect.color2.alpha = 0xFF;
        effect.unk23 = 0xFF;
        effect.unk28 = 0x200004;
        effect.otherModeH = 0;
        effect.unk2C = 0x1F0601;
        effect.combineMode = 3;
        effect.unk34 = 34;
        effect.unk38 = 0x80;
        effect.unk3C = 0x20;
        effect.unk40 = 0;
        effect.unk41 = 7;
        effect.viewport = target->CameraPtr->CNumber;
        func_1515548C(&effect, 0, 0, 0, 0, 0, arg5);
        cameraArg.side = side;
        cameraArg.camera = target->CameraPtr;
        func_1515572C((s32)&cameraArg, 0x37);
    }
}


/* Calls func_151C3B0C for a particle. The last argument is a mask of every camera except the owner's,
 * or 0xFF when the owner is gone, has no camera or fails one of the other checks. */
void func_150FC368(Particle_150FC368 *particle) {
    u8 cameraMask;
    Actor *owner;

    owner = particle->owner;
    if ((owner == NULL) || (owner->type == 0) || (owner->character_type == 0xFF) ||
        (owner->unique_id != particle->ownerUniqueId) || (owner->mv == NULL) ||
        (owner->mv->bot != 0) || (owner->playernumber == 0xFF) || (owner->CameraPtr == NULL)) {
        cameraMask = 0xFF;
    } else {
        cameraMask = ~(1U << owner->CameraPtr->CNumber) & 0xFF;
    }
    func_151C3B0C(particle, 1.0f, 1.0f, 0.6f, 0.0f, 0xFF, 0xFF, cameraMask);
}

/* Spawns an emitter object that follows `owner` (see func_150FC614); when `other` has a camera, also
 * creates an effect for that camera. */
void func_150FC438(Actor *owner, Actor *other, u8 arg2, u8 arg3) {
    TwoSidedEmitterState state;
    TwoSidedEmitter *obj;
    TwoSidedEmitterState *dest;
    CamWobbleDef wobble;

    state.owner = owner;
    state.ownerUniqueId = owner->unique_id;
    state.spawnTimer = 0.0f;
    state.side = 0;
    state.unkD = arg2;
    state.unk10 = NULL;
    state.unk14 = 0;
    state.unk1D = arg3;
    if (other != NULL && other->CameraPtr != NULL) {
        state.unk16 = other->CameraPtr->CNumber;
    } else {
        state.unk16 = -1;
    }
    state.other = other;
    if (other != NULL) {
        state.otherUniqueId = other->unique_id;
    } else {
        state.otherUniqueId = 0xFF;
    }

    obj = (TwoSidedEmitter *)func_15149130(300, -1, 0x27, -1, 0, 0x25, 0x20, 0xFF, 1);
    if (obj != NULL) {
        dest = &obj->state;
        memcpy(dest, &state, sizeof(state));
        if (other != NULL && other->CameraPtr != NULL) {
            wobble.flags = CAM_WOBBLE_SCALED;
            wobble.updateFn = -1;
            wobble.scaleFn = -1;
            wobble.timer = 300;
            wobble.playerIndex = other->CameraPtr->CNumber;
            wobble.amplitude[0] = 12.0f;
            wobble.amplitude[1] = 20.0f;
            wobble.amplitude[2] = 19.0f;
            wobble.amplitude[3] = 0.1976999938f;
            wobble.amplitude[4] = 0.2345999926f;
            wobble.amplitude[5] = 0.1586000025f;
            wobble.speed[0] = 0.5690000057f;
            wobble.speed[1] = 0.7200000286f;
            wobble.speed[2] = 1.149000049f;
            wobble.speed[3] = 1.886599898f;
            wobble.speed[4] = 1.99119997f;
            wobble.speed[5] = 1.254899979f;
            dest->unk10 = func_15164780(&wobble, 0, 0xFF, 1);
        }
    }
}


/* Per-frame update of an emitter: destroys it once its owner is gone or changed, otherwise fires
 * func_150FB8C0 from alternating sides whenever the spawn timer runs out. */
void func_150FC614(TwoSidedEmitter *obj) {
    TwoSidedEmitterState *state;
    Actor *owner;
    RumbleParams spark;

    state = &obj->state;
    owner = state->owner;
    if ((owner->type == 0) || (owner->character_type == 0xFF) || (owner->unique_id != state->ownerUniqueId) || (D_800C35EA[0] == 1)) {
        obj->base.header.state = -1;
        return;
    }
    if (owner->animseq.uh != state->unk1D) {
        obj->base.header.state = -1;
        return;
    }

    state->spawnTimer -= D_800BE9A4;
    while (state->spawnTimer < 0.0f) {
        func_150FB8C0(owner, state->side, -state->spawnTimer, state->unkD, obj->base.header.owner, obj->base.header.kind);
        state->side ^= 1;
        if (state->other != NULL) {
            if (state->other->mv != NULL) {
                state->other->mv->shotsfired++;
            }
        }
        state->spawnTimer += 4.0f + (func_150ADA68() * 4.0f);
    }

    if (state->unk16 == -1) {
        return;
    }
    state->unk14 -= D_800BE9E4;
    if (state->unk14 < 0) {
        spark.flags = 1;
        func_150ADA20();
        spark.duration = 30;
        spark.playerMask = 1 << state->unk16;
        spark.strength = (func_150ADA20() % 6U) + 3;
        spark.updateKind = -1;
        func_151D8868(&spark, 0, obj->base.header.owner, obj->base.header.kind);
        func_150ADA20();
        state->unk14 = 250;
    }
}

/* Message handler: type 0 destroys the emitter when it names the owner or the other object, type 0x2D
 * swaps two objects, retargeting the emitter's owner or other object. */
void func_150FC818(TwoSidedEmitter *obj, ActorMsg *msg, u8 msgType) {
    TwoSidedEmitterState *state = &obj->state;
    Actor *msgObj;
    Actor *msgObjA;
    Actor *owner;

    if (msgType == 0) {
        msgObj = msg->ref.actor;
        owner = state->owner;
        if ((owner == msgObj) || (state->ownerUniqueId == msg->ref.uniqueId)) {
            func_1516972C(&obj->base.header);
        } else if (state->other != NULL) {
            if ((msgObj == state->other) || (state->otherUniqueId == msg->ref.uniqueId)) {
                func_1516972C(&obj->base.header);
            }
        }
    } else if (msgType == 0x2D) {
        msgObjA = msg->swap.actorA;
        if (msgObjA == state->owner) {
            state->owner = msg->swap.actorB;
            state->ownerUniqueId = msg->swap.idB;
        } else if (state->owner == msg->swap.actorB) {
            state->owner = msgObjA;
            state->ownerUniqueId = msg->swap.idA;
        } else if (state->other != NULL) {
            if (msgObjA == state->other) {
                state->other = msg->swap.actorB;
                state->otherUniqueId = msg->swap.idB;
            } else if (state->other == msg->swap.actorB) {
                state->other = msgObjA;
                state->otherUniqueId = msg->swap.idA;
            }
        }
    }
}
