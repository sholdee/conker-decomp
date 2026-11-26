#include <ultra64.h>
#include "functions.h"
#include "variables.h"


static const Vec3f D_800A1920 = { 0.0f, -45.0f, 0.0f };

/* Transforms the fixed point D_800A1920 by the bone selected by `slot` (3-8); returns 0 for other slots. */
s32 func_150F22D0(Vec3f *out, Actor *owner, u8 slot) {
    s32 bone;

    switch (slot) {
    case 3:
        bone = 41;
        break;
    case 4:
        bone = 45;
        break;
    case 5:
        bone = 49;
        break;
    case 6:
        bone = 53;
        break;
    case 7:
        bone = 57;
        break;
    case 8:
        bone = 61;
        break;
    default:
        return 0;
    }
    func_15143134(&D_800A1920, out, &owner->ob_matrix_ptr[bone]);
    return 1;
}

s32 func_150F237C(s32 arg0, s32 arg1) {
    return 14;
}

/* Spawns a func_151C329C effect at the bone point of `slot`, if the owner has a skeleton. */
void func_150F2390(Actor *owner, s32 slot, s32 arg2) {
    Vec3f pos;

    if (owner->ob_matrix_ptr != NULL) {
        func_150F22D0(&pos, owner, slot);
        func_151C329C(&pos, 0xFF, 0);
    }
}

/* A two-axis sine sway: each axis advances its phase by `speed` per frame and swings by `amplitude`. */
typedef struct {
    f32 phaseX;
    f32 phaseZ;
    f32 speedX;
    f32 speedZ;
    f32 amplitudeX;
    f32 amplitudeZ;
} Sway; // size 0x18

struct SwayObject {
    u8 pad0[0x20];
    f32 rotX;
    f32 unk24;
    f32 rotZ;
    u8 pad2C[0xC];
    f32 x;
    f32 y;
    f32 z;
    u8 pad44[0x12C];
    Sway sway;
};

typedef struct {
    Sway sway;
    f32 pos[3];
    f32 rotX;
    f32 rotZ;
} SwayBlock;

/* Steps the object's sway and stores the resulting X/Z rotation. */
s32 func_150F23E0(SwayObject *obj) {
    Sway *sway = &obj->sway;

    obj->rotX = sway->amplitudeX * sinf(sway->phaseX);
    obj->rotZ = sway->amplitudeZ * sinf(sway->phaseZ);
    sway->phaseX = func_15144B68(sway->phaseX + sway->speedX * gFrameDelta);
    sway->phaseZ = func_15144B68(sway->phaseZ + sway->speedZ * gFrameDelta);
    return 1;
}

/* Same as func_150F23E0 for the model's embedded SwayBlock. */
s32 func_150F2480(DynModel *model) {
    SwayBlock *block = (SwayBlock *)&model->buffers[model->buffersSize];

    block->rotX = block->sway.amplitudeX * sinf(block->sway.phaseX);
    block->rotZ = block->sway.amplitudeZ * sinf(block->sway.phaseZ);
    block->sway.phaseX = func_15144B68(block->sway.phaseX + block->sway.speedX * gFrameDelta);
    block->sway.phaseZ = func_15144B68(block->sway.phaseZ + block->sway.speedZ * gFrameDelta);
    return 1;
}

/* Builds the model's matrix for this frame from its SwayBlock rotation and position. */
s32 func_150F2518(DynModel *model, s32 arg1) {
    MtxF mtx;
    SwayBlock *block;

    block = DYNMODEL_USERDATA(model);
    func_150A8050(mtx, block->rotX, 0.0f, block->rotZ);
    mtx[3][0] = block->pos[0];
    mtx[3][1] = block->pos[1];
    mtx[3][2] = block->pos[2];
    guMtxF2L(mtx, &model->mtx[D_800BE9C0]);
    return 1;
}

/* Places the point (0, -495, 0) of the linked object's exaggerated sway frame into the effect's light. */
s32 func_150F25A0(LightEffect *effect) {
    SwayObject **link;
    f32 outZ;
    f32 outY;
    f32 outX;
    MtxF mtx;

    link = (SwayObject **)&effect->data;
    func_150A8050(mtx, (*link)->rotX * 3.0f, 0.0f, (*link)->rotZ * 3.0f);
    mtx[3][0] = (*link)->x;
    mtx[3][1] = (*link)->y;
    mtx[3][2] = (*link)->z;
    func_150A7960(mtx, 0.0f, -495.0f, 0.0f, &outX, &outY, &outZ);
    effect->light->x = outX;
    effect->light->y = outY;
    effect->light->z = outZ;
    return 1;
}

void func_150F26A0(Camera *actor) {
    Actor *state;
    s32 mode;

    state = actor->CharPtr;
    if (state->unk102 != 0 && state->stunned == 0) {
        func_150F2994(actor, TRUE);
        if (actor->Mode != 0x100 && actor->cmdRunning == 0) {
            if (func_15123934(actor, 0x80, 0, 0, CAMERA_MODE_TRANSPORT) != 0) {
                actor->AvailableModes |= 0x300400;
                actor->AvailableModes &= ~6;
                actor->DistIndex = 1;
                actor->DistMask = 3;
                func_15124B18(actor);
            }
            actor->height = 29.0f;
            actor->unk34C = 29.0f;
            actor->IdealTripodMagnitude = 200.0f;
            actor->YOffsetTget = -19.0f;
            if (actor->TonyHitsClear == 0) {
                // matching: the no-op (f32 *) cast changes how IDO schedules the distance load
                if (0.5f * actor->IdealTripodMagnitude < *(f32 *)&actor->TripodMagnitude - actor->IdealTripodMagnitude) {
                    func_15128774(actor, actor->CharPtr);
                }
            }
        }
    } else {
        state->unk7 = 0xFF;
        if (actor->CharContext == 59) {
            func_150F2994(actor, TRUE);
            if (actor->Mode != 0x100 && actor->cmdRunning == 0) {
                if (func_15123934(actor, 8, 0, 0, CAMERA_MODE_TRANSPORT) != 0) {
                    actor->AvailableModes |= 0x300000;
                    actor->AvailableModes &= ~4;
                    actor->DistIndex = 1;
                    actor->DistMask = 3;
                }
                actor->Tightness = 0;
                actor->height = 125.0f;
                actor->unk34C = 125.0f;
                actor->IdealTripodMagnitude = 220.0f;
                actor->YOffsetTget = 30.0f;
            }
        } else {
            mode = actor->Mode;
            if ((mode == 8 || mode == 0x80) && actor->cmdRunning == 0) {
                func_151239CC(actor, CAMERA_MODE_TRANSPORT);
            }
            if (actor->DistIndex == 1) {
                actor->DistIndex = 2;
                func_15124B18(actor);
            }
            func_150F2994(actor, FALSE);
        }
    }
    if (func_1509BE40(1, 0x405A, 6, 0x9000) != 0) {
        actor->AvailableModes |= 0x80000000;
    } else {
        actor->AvailableModes &= ~0x80000000;
    }
    if (func_1509BE40(1, 0x405E, 6, 0x9000) != 0) {
        actor->AvailableModes |= 0x10000;
    } else {
        actor->AvailableModes &= ~0x10000;
    }
    if (func_1509BE40(1, 0x405F, 6, 0x2000) != 0) {
        actor->AvailableModes |= 0x10;
    } else {
        actor->AvailableModes &= ~0x10;
    }
}

/* Calls func_1509BFB0(0, id, flag) for ids 0x4016-0x4023, 0x405D and 0x405F, and 0x4063-0x4065 when bit 3 of D_800D2E4C byte 0x15 is set. */
void func_150F2994(Camera *actor, s32 flag) {
    s32 i;

    for (i = 0; i < 14; i++) {
        func_1509BFB0(0, 0x4016 + i, flag);
    }
    func_1509BFB0(0, 0x405D, flag);
    func_1509BFB0(0, 0x405F, flag);
    if (D_800D2E4C[0x15] & 8) {
        func_1509BFB0(0, 0x4063, flag);
        func_1509BFB0(0, 0x4064, flag);
        func_1509BFB0(0, 0x4065, flag);
    }
}
