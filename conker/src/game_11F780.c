#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_150F2994(Camera *actor, s32 flag);
s32 func_1509BE40(s32, s32, s32, s32);

typedef f32 BoneMtx[4][4];

typedef struct {
    u8 pad0[0x1D4];
    BoneMtx *bones;
} BoneOwner;

static const f32 D_800A1920[3] = { 0.0f, -45.0f, 0.0f };

void func_15143134(const f32 *in, f32 *out, BoneMtx *mtx);
extern void func_151C329C(f32 *pos, s32, s32);

/* Transforms the fixed point D_800A1920 by the bone selected by `slot` (3-8); returns 0 for other slots. */
s32 func_150F22D0(f32 *out, BoneOwner *owner, u8 slot) {
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
    func_15143134(D_800A1920, out, &owner->bones[bone]);
    return 1;
}

s32 func_150F237C(s32 arg0, s32 arg1) {
    return 14;
}

/* Spawns a func_151C329C effect at the bone point of `slot`, if the owner has a skeleton. */
void func_150F2390(BoneOwner *owner, s32 slot, s32 arg2) {
    f32 pos[3];

    if (owner->bones != NULL) {
        func_150F22D0(pos, owner, slot);
        func_151C329C(pos, 0xFF, 0);
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

typedef struct {
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
} SwayObject;

typedef struct {
    Sway sway;
    f32 pos[3];
    f32 rotX;
    f32 rotZ;
} SwayBlock;

typedef struct {
    u8 pad0[0x50];
    s32 swayOffset; // byte offset of the SwayBlock within data[]
    u8 pad54[0x24];
    Mtx mtx[2]; // indexed by D_800BE9C0
    u8 data[1];
} SwayModel;

extern void func_150A7960(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/* Steps the object's sway and stores the resulting X/Z rotation. */
s32 func_150F23E0(SwayObject *obj) {
    Sway *sway = &obj->sway;

    obj->rotX = sway->amplitudeX * sinf(sway->phaseX);
    obj->rotZ = sway->amplitudeZ * sinf(sway->phaseZ);
    sway->phaseX = func_15144B68(sway->phaseX + sway->speedX * D_800BE9A4);
    sway->phaseZ = func_15144B68(sway->phaseZ + sway->speedZ * D_800BE9A4);
    return 1;
}

/* Same as func_150F23E0 for the model's embedded SwayBlock. */
s32 func_150F2480(SwayModel *model) {
    SwayBlock *block = (SwayBlock *)&model->data[model->swayOffset];

    block->rotX = block->sway.amplitudeX * sinf(block->sway.phaseX);
    block->rotZ = block->sway.amplitudeZ * sinf(block->sway.phaseZ);
    block->sway.phaseX = func_15144B68(block->sway.phaseX + block->sway.speedX * D_800BE9A4);
    block->sway.phaseZ = func_15144B68(block->sway.phaseZ + block->sway.speedZ * D_800BE9A4);
    return 1;
}

/* Builds the model's matrix for this frame from its SwayBlock rotation and position. */
s32 func_150F2518(SwayModel *model, s32 arg1) {
    f32 mtx[4][4];
    SwayBlock *block;

    // matching: the integer cast of `model` costs IDO one temp register, which the target shows
    block = (SwayBlock *)((s32)model + model->swayOffset + 0xF8);
    func_150A8050(mtx, block->rotX, 0.0f, block->rotZ);
    mtx[3][0] = block->pos[0];
    mtx[3][1] = block->pos[1];
    mtx[3][2] = block->pos[2];
    guMtxF2L(mtx, &model->mtx[D_800BE9C0]);
    return 1;
}

/* Places the point (0, -495, 0) of the linked object's exaggerated sway frame into arg0->unk14. */
s32 func_150F25A0(struct242 *arg0) {
    SwayObject **link;
    f32 outZ;
    f32 outY;
    f32 outX;
    f32 mtx[4][4];

    link = (SwayObject **)&arg0->unk18;
    func_150A8050(mtx, (*link)->rotX * 3.0f, 0.0f, (*link)->rotZ * 3.0f);
    mtx[3][0] = (*link)->x;
    mtx[3][1] = (*link)->y;
    mtx[3][2] = (*link)->z;
    func_150A7960(mtx, 0.0f, -495.0f, 0.0f, &outX, &outY, &outZ);
    arg0->unk14->unkE = outX;
    arg0->unk14->unk10 = outY;
    arg0->unk14->unk12 = outZ;
    return 1;
}

void func_150F26A0(Camera *actor) {
    struct127 *state;
    s32 mode;

    state = actor->player;
    if (state->unk102 != 0 && state->stunned == 0) {
        func_150F2994(actor, TRUE);
        if (actor->mode != 0x100 && actor->cmdRunning == 0) {
            if (func_15123934(actor, 0x80, 0, 0, 3) != 0) {
                actor->flags |= 0x300400;
                actor->flags &= ~6;
                actor->preset = 1;
                actor->presetMask = 3;
                func_15124B18(actor);
            }
            actor->height = 29.0f;
            actor->unk34C = 29.0f;
            actor->targetDistance = 200.0f;
            actor->liftTarget = -19.0f;
            if (actor->unk3E8 == 0) {
                // matching: the no-op (f32 *) cast changes how IDO schedules the distance load
                if (0.5f * actor->targetDistance < *(f32 *)&actor->distance - actor->targetDistance) {
                    func_15128774(actor, actor->player);
                }
            }
        }
    } else {
        state->unk7 = 0xFF;
        if (actor->zone == 59) {
            func_150F2994(actor, TRUE);
            if (actor->mode != 0x100 && actor->cmdRunning == 0) {
                if (func_15123934(actor, 8, 0, 0, 3) != 0) {
                    actor->flags |= 0x300000;
                    actor->flags &= ~4;
                    actor->preset = 1;
                    actor->presetMask = 3;
                }
                actor->unk134 = 0;
                actor->height = 125.0f;
                actor->unk34C = 125.0f;
                actor->targetDistance = 220.0f;
                actor->liftTarget = 30.0f;
            }
        } else {
            mode = actor->mode;
            if ((mode == 8 || mode == 0x80) && actor->cmdRunning == 0) {
                func_151239CC(actor, 3);
            }
            if (actor->preset == 1) {
                actor->preset = 2;
                func_15124B18(actor);
            }
            func_150F2994(actor, FALSE);
        }
    }
    if (func_1509BE40(1, 0x405A, 6, 0x9000) != 0) {
        actor->flags |= 0x80000000;
    } else {
        actor->flags &= ~0x80000000;
    }
    if (func_1509BE40(1, 0x405E, 6, 0x9000) != 0) {
        actor->flags |= 0x10000;
    } else {
        actor->flags &= ~0x10000;
    }
    if (func_1509BE40(1, 0x405F, 6, 0x2000) != 0) {
        actor->flags |= 0x10;
    } else {
        actor->flags &= ~0x10;
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
    if (D_800D2E4C->pad15[0] & 8) {
        func_1509BFB0(0, 0x4063, flag);
        func_1509BFB0(0, 0x4064, flag);
        func_1509BFB0(0, 0x4065, flag);
    }
}
