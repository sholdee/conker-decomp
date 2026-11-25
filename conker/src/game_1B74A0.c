#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef f32 MtxF[4][4];

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

/* Object spawn parameters (see game_179F30.c); only passed through here. */
typedef struct SpawnParams SpawnParams;

/* Emitter parameters; func_151407D0 copies them into the spawned object at +0x110. */
typedef struct EmitterParams {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[0x20];
    /* 0x44 */ u8 *unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ u8 pad4C[8];
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 unk55;
    /* 0x56 */ u8 unk56;
    /* 0x57 */ u8 unk57;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
} EmitterParams;

/* Tells a spawned emitter how to follow its owner: two points (p0, p1) given in the space of
 * bone `bone` of `target`, which func_1518A094 transforms every frame. */
typedef struct EmitterAttach {
    /* 0x00 */ Vec3f p0;
    /* 0x0C */ Vec3f p1;
    /* 0x18 */ struct127 *target;
    /* 0x1C */ u8 bone;
    /* 0x1D */ u8 targetId; // the target's unique_id
    /* 0x1E */ s8 handler;  // index into D_8008D5A0, or -1
    /* 0x1F */ u8 pad1F;
} EmitterAttach;

/* The object func_151407D0 spawns for func_15189FF0. Bytes 0x18.. hold a copy of the
 * SpawnParams, so `color` and `p0`/`p1` mirror SpawnParams unk10 and unk1C..unk38. */
typedef struct Emitter {
    /* 0x000 */ u8 pad0[0x28];
    /* 0x028 */ u8 color[4];
    /* 0x02C */ u8 pad2C[8];
    /* 0x034 */ Vec3f p0; // world-space attach points, updated by func_1518A094
    /* 0x040 */ Vec3f p1;
    /* 0x04C */ u8 pad4C[0xC];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 pad5C[0x110 - 0x5C];
    /* 0x110 */ EmitterParams params;
    /* 0x16C */ u8 pad16C[4];
    /* 0x170 */ EmitterAttach attach;
} Emitter;

/* Byte view of the attach target (struct127 declares these offsets as padding). */
typedef struct {
    /* 0x00 */ u8 pad0[0x68];
    /* 0x68 */ u8 unk68;
    /* 0x69 */ u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B[4];
    /* 0x6F */ u8 unk6F;
} AttachTargetBytes;

/* Payload of message 0x2D: two objects and their unique ids. */
typedef struct {
    /* 0x0 */ struct127 *objA;
    /* 0x4 */ struct127 *objB;
    /* 0x8 */ u8 idA;
    /* 0x9 */ u8 idB;
} SwapTargetMsg;

Emitter *func_151407D0(EmitterParams *, s32, SpawnParams *, u8, u8, u8, u8, s32, s32, s32);
void func_150A7960(MtxF mtx, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/* Emitter handlers run by func_1518A094 before it follows the bone. Returning 0 ends the emitter's
 * update (func_1518A094 returns 0); setting *follow to FALSE skips this frame's bone follow. */
extern s32 (*D_8008D5A0[])(Emitter *, u8 *);

/* Spawns an emitter (type 0xB) that follows the bone described by `attach`. */
Emitter *func_15189FF0(SpawnParams *spawn, EmitterParams *params, EmitterAttach *attach, u8 arg3, u8 arg4,
                       u8 arg5, u8 arg6) {
    Emitter *emitter;

    params->unk58 = 0xB;
    emitter = func_151407D0(params, 0x80, spawn, arg3, arg4, arg5, arg6, -1, 0xFF, 1);
    if (emitter != NULL) {
        EmitterAttach *dst = &emitter->attach;

        memcpy(dst, attach, sizeof(EmitterAttach));
    } else { // matching: an early `if (emitter == NULL) return NULL;` changes the branch layout
        return NULL;
    }
    return emitter;
}

/* Moves the emitter's p0/p1 onto its target's bone. Returns 0 once the target is gone. */
s32 func_1518A094(Emitter *emitter) {
    EmitterAttach *attach;

    attach = &emitter->attach;
    if (attach->target == NULL) {
        attach->target = func_15083E90(attach->targetId);
        if (attach->target == NULL) {
            return 0;
        }
    }
    if ((attach->target->interaction_state != 0) && (attach->target->unique_id == attach->targetId)) {
        if (attach->handler != -1) {
            u8 follow;

            if (D_8008D5A0[attach->handler](emitter, &follow)) {
                if (!follow) {
                    return 1;
                }
            } else { // matching: early-return form reorders the branches
                return 0;
            }
        }
        if (attach->target->unk1D4 != NULL) {
            f32 (*mtx)[4];
            Vec3f *out;

            mtx = ((MtxF *)attach->target->unk1D4)[attach->bone];
            out = &emitter->p0;
            func_150A7960(mtx, attach->p0.x, attach->p0.y, attach->p0.z, &out->x, &out->y, &out->z);
            out = &emitter->p1;
            func_150A7960(mtx, attach->p1.x, attach->p1.y, attach->p1.z, &out->x, &out->y, &out->z);
            emitter->flags |= 2;
            emitter->flags &= ~4;
        } else {
            emitter->flags &= ~2;
        }
    } else {
        return 0;
    }
    return 1;
}

/* Handler: sets params unk18/unk1C and the color from the target's byte 0x68; 15 clears flag 2 and stops. */
s32 func_1518A214(Emitter *emitter, u8 *follow) {
    EmitterParams *params;
    f32 value;

    params = &emitter->params;
    switch (((AttachTargetBytes *)emitter->attach.target)->unk68 - 15) {
    default: // < 15 or > 17
        value = 0.6f;
        params->unk18 = value;
        params->unk1C = value;
        emitter->color[2] = 0;
        emitter->color[1] = 0;
        emitter->color[0] = 0xFF;
        *follow = TRUE;
        break;
    case 0: // 15
        emitter->flags &= ~2;
        *follow = FALSE;
        break;
    case 1: // 16
        value = 0.3f;
        params->unk18 = value;
        params->unk1C = value;
        emitter->color[0] = 0x80;
        emitter->color[1] = 0;
        emitter->color[2] = 0;
        *follow = TRUE;
        break;
    case 2: // 17
        value = 0.6f;
        params->unk18 = value;
        params->unk1C = value;
        emitter->color[0] = 0xFF;
        emitter->color[1] = 0;
        emitter->color[2] = 0;
        *follow = TRUE;
        break;
    }
    return 1;
}

/* Handler: while the target's byte 0x6A is set, clears flag 2 and stops. */
s32 func_1518A2E8(Emitter *emitter, u8 *follow) {
    if (((AttachTargetBytes *)emitter->attach.target)->unk6A != 0) {
        emitter->flags &= ~2;
        *follow = FALSE;
    } else {
        *follow = TRUE;
    }
    return 1;
}

/* Handler: once the target's byte 0x6F is clear, clears flag 2 and stops. */
s32 func_1518A324(Emitter *emitter, u8 *follow) {
    if (((AttachTargetBytes *)emitter->attach.target)->unk6F == 0) {
        emitter->flags &= ~2;
        *follow = FALSE;
    } else {
        *follow = TRUE;
    }
    return 1;
}

/* Message handler: message 0x2D moves the emitter from one object of the pair to the other. */
void func_1518A360(Emitter *emitter, SwapTargetMsg *msg, u8 msgId) {
    EmitterAttach *attach = &emitter->attach;

    if (msgId == 0x2D) {
        if (attach->target == msg->objA) {
            attach->target = msg->objB;
            attach->targetId = msg->idB;
        } else if (attach->target == msg->objB) {
            attach->target = msg->objA;
            attach->targetId = msg->idA;
        }
    }
}
