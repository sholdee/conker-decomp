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

/* Emitter update handlers; each sets *keepGoing and returns 0 to stop the update. */
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
    } else {
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
            u8 keepGoing;

            if (D_8008D5A0[attach->handler](emitter, &keepGoing)) {
                if (!keepGoing) {
                    return 1;
                }
            } else {
                return 0;
            }
        }
        if (attach->target->unk1D4 != NULL) {
            f32 *mtx;
            f32 *out;

            mtx = (f32 *)((u8 *)attach->target->unk1D4 + (attach->bone << 6));
            out = &emitter->p0.x;
            func_150A7960(mtx, attach->p0.x, attach->p0.y, attach->p0.z, &out[0], &out[1], &out[2]);
            out = &emitter->p1.x;
            func_150A7960(mtx, attach->p1.x, attach->p1.y, attach->p1.z, &out[0], &out[1], &out[2]);
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

s32 func_1518A214(void *arg0, u8 *arg1) {
    s32 temp_v0;
    EmitterParams *temp_v0_2;
    f32 temp_f0;

    temp_v0 = ((AttachTargetBytes *)((Emitter *)arg0)->attach.target)->unk68 - 0xF;
    temp_v0_2 = (EmitterParams *)((u8 *)arg0 + 0x110);
    switch (temp_v0) {
    default:
        temp_f0 = 0.6000000238f;
        temp_v0_2->unk18 = temp_f0;
        temp_v0_2->unk1C = temp_f0;
        ((Emitter *)arg0)->color[2] = 0;
        ((Emitter *)arg0)->color[1] = 0;
        ((Emitter *)arg0)->color[0] = 0xFF;
        *arg1 = 1;
        break;
    case 0:
        ((Emitter *)arg0)->flags &= ~2;
        *arg1 = 0;
        break;
    case 1:
        temp_f0 = 0.3000000119f;
        temp_v0_2->unk18 = temp_f0;
        temp_v0_2->unk1C = temp_f0;
        ((Emitter *)arg0)->color[0] = 0x80;
        ((Emitter *)arg0)->color[1] = 0;
        ((Emitter *)arg0)->color[2] = 0;
        *arg1 = 1;
        break;
    case 2:
        temp_f0 = 0.6000000238f;
        temp_v0_2->unk18 = temp_f0;
        temp_v0_2->unk1C = temp_f0;
        ((Emitter *)arg0)->color[0] = 0xFF;
        ((Emitter *)arg0)->color[1] = 0;
        ((Emitter *)arg0)->color[2] = 0;
        *arg1 = 1;
        break;
    }
    return 1;
}

s32 func_1518A2E8(Emitter *arg0, u8 *arg1) {
    if (((AttachTargetBytes *)arg0->attach.target)->unk6A != 0) {
        arg0->flags &= ~2;
        *arg1 = 0;
    } else {
        *arg1 = 1;
    }
    return 1;
}

s32 func_1518A324(Emitter *arg0, u8 *arg1) {
    if (((AttachTargetBytes *)arg0->attach.target)->unk6F == 0) {
        arg0->flags &= ~2;
        *arg1 = 0;
    } else {
        *arg1 = 1;
    }
    return 1;
}

void func_1518A360(void *arg0, SwapTargetMsg *arg1, u8 arg2) {
    EmitterAttach *v0 = &((Emitter *)arg0)->attach;
    if (arg2 == 0x2D) {
        if (v0->target == arg1->objA) {
            v0->target = arg1->objB;
            v0->targetId = arg1->idB;
        } else if (v0->target == arg1->objB) {
            v0->target = arg1->objA;
            v0->targetId = arg1->idA;
        }
    }
}
