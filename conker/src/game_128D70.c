#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* State block that func_150FC438 copies into its spawned object at offset 0x28. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerUniqueId;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 spawnTimer;
    /* 0x0C */ u8 side;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ struct260 *unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s8 unk16; // owner camera's unk23D, or -1
    /* 0x17 */ u8 pad17;
    /* 0x18 */ Actor *other;
    /* 0x1C */ u8 otherUniqueId;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 pad1E[2];
} EmitterState; // size 0x20

/* The object spawned by func_150FC438. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ EmitterState state;
} EmitterObj;


/* Effect descriptor passed to func_15164780. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 unk1;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 pad7;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
} EmitterEffect; // size 0x38

extern struct260 *func_15164780(EmitterEffect *, s32, u8, s32);


/* First argument of func_151602C0. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s8 unk4;
} SparkArg_150FB8C0;

/* First argument of func_1515572C. */
typedef struct {
    /* 0x00 */ u8 side;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Camera *camera;
} CameraArg_150FB8C0;

extern u8 D_800A1DD0[], D_800A1E34[];
extern f32 D_800A1DD4[][3], D_800A1DEC[][3], D_800A1E04[][3], D_800A1E1C[][3], D_800A1E38[][3], D_800A1E50[][3], D_800A1E68[][3], D_800A1E80[][3];
extern Actor *func_1507515C(Actor *);
extern void func_15145EA4(f32 **, f32 **, s32, s32);
extern s32 func_151C229C(f32 *, f32 *, f32 *, f32 *, s32, s32, f32, f32, f32, f32, f32, s32, Actor *, s32, s32, s32, s32, s32, s32, s32, s32, f32, s32, s32, s32, s32, s32);
extern s32 func_151602C0(SparkArg_150FB8C0 *, s32 *, s32, s32, s32, s32, s32, s32, s32, u8, s32);
extern void func_15102B38(Actor *, u8, f32 *, f32 *, f32 *, s32, s32, f32, f32 *, s32, s32, s32, u8, s32);
extern void func_1515548C(ScreenSpriteInfo *, s32, s32, s32, s32, s32, s32);
extern void func_1515572C(CameraArg_150FB8C0 *, s32);

/* Spawns the effect for one side (0 or 1) of `obj`. The side's points are transformed by one of obj's
 * unk1D4 matrices; when the target found by func_1507515C has unk197 set, the effect is aimed at the point
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
    SparkArg_150FB8C0 spark;
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
        if (target->unk31C == NULL) {
            return;
        }
        if (target->unk31C->aimState != 0) {
            useTarget = 1;
        } else {
            useTarget = 0;
        }
    }
    if (obj->unk1D4 == NULL) {
        return;
    }
    if (obj->id == 0x23) {
        srcPoints[0] = D_800A1E38[side];
        if (useTarget) {
            srcPoints[1] = D_800A1E68[side];
        } else {
            srcPoints[1] = D_800A1E50[side];
        }
        srcPoints[2] = D_800A1E80[side];
        matrix = (D_800A1E34[side] << 6) + (s32)obj->unk1D4;
    } else {
        localPoint[0] = (13.0f - (1.0f + func_150ADA68() * 6.0f)) * (side ? -1 : 1);
        localPoint[1] = D_800A1DEC[side][1];
        localPoint[2] = D_800A1DEC[side][2];
        srcPoints[0] = D_800A1DD4[side];
        srcPoints[1] = useTarget ? D_800A1E04[side] : localPoint;
        srcPoints[2] = D_800A1E1C[side];
        matrix = (D_800A1DD0[side] << 6) + (s32)obj->unk1D4;
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
        // matching: this block-scope redeclaration of the callee is needed; removing it mismatches
        extern void func_151450B4(Vec3f *, Vec3f *, Vec3f *);

        linePoint = &target->unk31C->aimEye;
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
            ((normal.x * target->unk31C->aimDir.x) + (normal.y * target->unk31C->aimDir.y) + (normal.z * target->unk31C->aimDir.z));
        hitPoint[0] = linePoint->x + (t * target->unk31C->aimDir.x);
        hitPoint[1] = linePoint->y + (t * target->unk31C->aimDir.y);
        hitPoint[2] = linePoint->z + (t * target->unk31C->aimDir.z);
        lineDir[0] = target->unk31C->aimDir.x;
        lineDir[1] = target->unk31C->aimDir.y;
        lineDir[2] = target->unk31C->aimDir.z;
    } else {
        dir[0] -= origin[0];
        dir[1] -= origin[1];
        dir[2] -= origin[2];
    }
    if (!useTarget || target->camera == NULL) {
        cameraMask = 0xFF;
    } else {
        cameraMask = ~(1 << target->camera->playerIndex) & 0xFF;
    }
    if (D_800BE9F0 == 0x34 || D_800BE9F0 == 0x26) {
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
            spark.unk0 = 3;
            spark.unk1 = -1;
            spark.unk2 = (func_150ADA20() % 5U) + 4;
            spark.unk4 = 0;
            sparkPos[0] = (s32)origin[0];
            sparkPos[1] = (s32)origin[1];
            sparkPos[2] = (s32)origin[2];
            func_151602C0(&spark, sparkPos, (func_150ADA20() % 31U) + 70, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0, arg4, arg5);
        }
        if (!(func_150ADA20() & 1)) {
            trailParams[1] = (func_150ADA68() * 2.435000181f) + 3.534000158f;
            trailParams[0] = (func_150ADA68() * 43.40000153f) + 48.40000153f;
            if (!useTarget || target->camera == NULL) {
                trailCameraMask = 0xFF;
            } else {
                trailCameraMask = ~(1 << target->camera->playerIndex) & 0xFF;
            }
            func_15102B38(obj, (obj->id == 0x23) ? D_800A1E34[side] : D_800A1DD0[side], (obj->id == 0x23) ? D_800A1E38[side] : D_800A1DD4[side],
                          (obj->id == 0x23) ? D_800A1E50[side] : D_800A1DEC[side], trailParams,
                          (func_150ADA20() % 3U) + 4, (func_150ADA20() % 101U) + 155, (func_150ADA68() * 1012.0f) + 1497.0f, origin,
                          trailCameraMask, 0, -1, arg4, arg5);
        }
    }
    if (useTarget && target->camera != NULL) {
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
        effect.viewport = target->camera->playerIndex;
        func_1515548C(&effect, 0, 0, 0, 0, 0, arg5);
        cameraArg.side = side;
        cameraArg.camera = target->camera;
        func_1515572C(&cameraArg, 0x37);
    }
}


void func_151C3B0C(void *, f32, f32, f32, f32, s32, s32, s32);

typedef struct {
    /* 0x000 */ u8 pad0[0x1A0];
    /* 0x1A0 */ Actor *owner;
    /* 0x1A4 */ u8 ownerUniqueId;
} Particle_150FC368;

/* Calls func_151C3B0C for a particle. The last argument is a mask of every camera except the owner's,
 * or 0xFF when the owner is gone, has no camera or fails one of the other checks. */
void func_150FC368(Particle_150FC368 *particle) {
    u8 cameraMask;
    Actor *owner;

    owner = particle->owner;
    if ((owner == NULL) || (owner->interaction_state == 0) || (owner->id == 0xFF) ||
        (owner->unique_id != particle->ownerUniqueId) || (owner->unk31C == NULL) ||
        (owner->unk31C->unk84 != 0) || (owner->unk127 == 0xFF) || (owner->camera == NULL)) {
        cameraMask = 0xFF;
    } else {
        cameraMask = ~(1U << owner->camera->playerIndex) & 0xFF;
    }
    func_151C3B0C(particle, 1.0f, 1.0f, 0.6f, 0.0f, 0xFF, 0xFF, cameraMask);
}

/* Spawns an emitter object that follows `owner` (see func_150FC614); when `other` has a camera, also
 * creates an effect for that camera. */
void func_150FC438(Actor *owner, Actor *other, u8 arg2, u8 arg3) {
    EmitterState state;
    EmitterObj *obj;
    EmitterState *dest;
    EmitterEffect effect;

    state.owner = owner;
    state.ownerUniqueId = owner->unique_id;
    state.spawnTimer = 0.0f;
    state.side = 0;
    state.unkD = arg2;
    state.unk10 = NULL;
    state.unk14 = 0;
    state.unk1D = arg3;
    if (other != NULL && other->camera != NULL) {
        state.unk16 = other->camera->playerIndex;
    } else {
        state.unk16 = -1;
    }
    state.other = other;
    if (other != NULL) {
        state.otherUniqueId = other->unique_id;
    } else {
        state.otherUniqueId = 0xFF;
    }

    obj = (EmitterObj *)func_15149130(300, -1, 0x27, -1, 0, 0x25, 0x20, 0xFF, 1);
    if (obj != NULL) {
        dest = &obj->state;
        memcpy(dest, &state, sizeof(state));
        if (other != NULL && other->camera != NULL) {
            effect.unk0 = 4;
            effect.unk1 = -1;
            effect.unk2 = -1;
            effect.unk4 = 300;
            effect.unk6 = other->camera->playerIndex;
            effect.unk8 = 12.0f;
            effect.unkC = 20.0f;
            effect.unk10 = 19.0f;
            effect.unk14 = 0.1976999938f;
            effect.unk18 = 0.2345999926f;
            effect.unk1C = 0.1586000025f;
            effect.unk20 = 0.5690000057f;
            effect.unk24 = 0.7200000286f;
            effect.unk28 = 1.149000049f;
            effect.unk2C = 1.886599898f;
            effect.unk30 = 1.99119997f;
            effect.unk34 = 1.254899979f;
            dest->unk10 = func_15164780(&effect, 0, 0xFF, 1);
        }
    }
}

typedef struct {
    u8 unk0;
    u8 pad1;
    s16 unk2;
    u8 unk4;
    u8 unk5;
    s8 unk6;
    u8 unk7;
} Spark_150FC614;

extern void func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* Per-frame update of an emitter: destroys it once its owner is gone or changed, otherwise fires
 * func_150FB8C0 from alternating sides whenever the spawn timer runs out. */
void func_150FC614(EmitterObj *obj) {
    EmitterState *state;
    Actor *owner;
    Spark_150FC614 spark;

    state = &obj->state;
    owner = state->owner;
    if ((owner->interaction_state == 0) || (owner->id == 0xFF) || (owner->unique_id != state->ownerUniqueId) || (D_800C35EA[0] == 1)) {
        obj->unkE = -1;
        return;
    }
    if (owner->unk84.uh != state->unk1D) {
        obj->unkE = -1;
        return;
    }

    state->spawnTimer -= D_800BE9A4;
    while (state->spawnTimer < 0.0f) {
        func_150FB8C0(owner, state->side, -state->spawnTimer, state->unkD, obj->unkC, obj->unk1);
        state->side ^= 1;
        if (state->other != NULL) {
            if (state->other->unk31C != NULL) {
                state->other->unk31C->unk1AA++;
            }
        }
        state->spawnTimer += 4.0f + (func_150ADA68() * 4.0f);
    }

    if (state->unk16 == -1) {
        return;
    }
    state->unk14 -= D_800BE9E4;
    if (state->unk14 < 0) {
        spark.unk0 = 1;
        func_150ADA20();
        spark.unk2 = 30;
        spark.unk5 = 1 << state->unk16;
        spark.unk4 = (func_150ADA20() % 6U) + 3;
        spark.unk6 = -1;
        func_151D8868(&spark, 0, obj->unkC, obj->unk1);
        func_150ADA20();
        state->unk14 = 250;
    }
}

/* Message handler: type 0 destroys the emitter when it names the owner or the other object, type 0x2D
 * swaps two objects, retargeting the emitter's owner or other object. */
void func_150FC818(EmitterObj *obj, ActorMsg *msg, u8 msgType) {
    EmitterState *state = &obj->state;
    Actor *msgObj;
    Actor *msgObjA;
    Actor *owner;

    if (msgType == 0) {
        msgObj = msg->ref.actor;
        owner = state->owner;
        if ((owner == msgObj) || (state->ownerUniqueId == msg->ref.uniqueId)) {
            func_1516972C((struct102 *)obj);
        } else if (state->other != NULL) {
            if ((msgObj == state->other) || (state->otherUniqueId == msg->ref.uniqueId)) {
                func_1516972C((struct102 *)obj);
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
