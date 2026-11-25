#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * An animated model: an animation playback state (as in game_83300.c's AnimPlaybackState, which
 * ends at 0x3E0) followed by a matrix buffer per frame buffer for the model's parts.
 */
typedef struct {
    /* 0x000 */ u8 pad0[0x45];
    /* 0x045 */ u8 unk45; /* byte 5 of the animation's data header */
    /* 0x046 */ u8 pad46[0x215 - 0x46];
    /* 0x215 */ u8 unk215; /* byte 5 of the previous animation's data header */
    /* 0x216 */ u8 pad216[0x3E0 - 0x216];
    /* 0x3E0 */ f32 (*matrices[2])[4]; /* model transform, one per frame buffer; set by the owner */
    /* 0x3E8 */ Mtx *partMtx[2];        /* numParts matrices, one array per frame buffer */
    /* 0x3F0 */ s32 unk3F0;
    /* 0x3F4 */ u8 numParts;
    /* 0x3F5 */ u8 bank; /* animation bank passed to func_1505E0C4 */
    /* 0x3F6 */ u8 unk3F6; /* set once the part matrices have been built */
} AnimModel; // size 0x3F8

extern void *allocate_memory(s32 size, s32 arg1, s32 arg2, s32 arg3);
extern s32 func_10004074(void *ptr);
void func_1502D824(AnimModel *anim, void *obj, u8 *buf);
s32 func_1502FE10(s32 model, Gfx ***dlists, u8 *numDLists, u8 **data, s32 *arg4, s32 *arg5, s32 *arg6);
void func_1505E0C4(void *actor, u16 *animEntry, AnimModel *state, s32 arg3, s32 restart, s32 anim, s32 bank,
                   f32 speed, f32 blendFrames, f32 arg9, f32 arg10, s32 flag8000);
void func_1507BDB0(AnimModel *anim, f32 rate, void *obj, s32 mode);
void func_150A81D0(Mtx **mtx, f32 (*transform)[4], AnimModel *anim, s32 arg3, s32 numParts, s16 *arg5, u8 *remap,
                   s32 scale);
void func_150A9984(Mtx *mtx, u8 count);

/* Locals of func_1503F4B0 that live on the stack at fixed offsets. */
struct func_1503F4B0_sp38 {
    Mtx *mtx;
    s32 pad4;
    s16 unk8;
    s16 padA;
    u8 *frameBuf;
};

/* Builds this frame buffer's part matrices from the current animation frame, then advances the animation. */
void func_1503F4B0(AnimModel *anim) {
    struct func_1503F4B0_sp38 sp38;
    f32 (*transform)[4];

    sp38.frameBuf = NULL;
    if (anim->matrices[D_800BE9C0] != NULL) {
        sp38.frameBuf = allocate_memory(0x320, 1, 2, 2);
        func_1502D824(anim, NULL, sp38.frameBuf);
        sp38.unk8 = 0x1000;
        transform = anim->matrices[D_800BE9C0];
        sp38.mtx = anim->partMtx[D_800BE9C0];
        func_150A81D0(&sp38.mtx, transform, anim, anim->unk3F0, anim->numParts, &sp38.unk8, NULL, 0);
        func_150A9984(anim->partMtx[D_800BE9C0], anim->numParts);
        anim->unk3F6 = TRUE;
    }
    func_1507BDB0(anim, D_800BE9A4, NULL, 0);
    if (sp38.frameBuf != NULL) {
        func_10004074(sp38.frameBuf);
    }
}

/* Starts animation `anim` from the model's bank. */
void func_1503F5B8(AnimModel *model, s32 restart, s32 anim, f32 speed, f32 blendFrames, s32 flag8000) {
    func_1505E0C4(NULL, NULL, model, 0, restart, anim, model->bank, speed, blendFrames, 0.0f, 0.0f, flag8000);
}

/*
 * Loads model `model` (see func_1502FE10) and creates its AnimModel in *out, playing animation 0.
 * Returns 1 on failure.
 */
s32 func_1503F62C(s32 model, s32 bank, Gfx ***dlists, u8 *numDLists, u8 **data, s32 *arg5, AnimModel **out) {
    s32 numParts;
    Mtx *mtx;
    s32 pad;

    *out = allocate_memory(sizeof(AnimModel), 1, 2, 2);
    if (*out == NULL) {
        return 1;
    }
    bzero(*out, 0x40);
    (*out)->unk215 = 1;
    (*out)->unk45 = 1;
    func_1502FE10(model, dlists, numDLists, data, arg5, &(*out)->unk3F0, &numParts);
    (*out)->numParts = numParts;
    (*out)->bank = bank;
    (*out)->unk3F6 = FALSE;
    (*out)->partMtx[0] = allocate_memory(numParts * sizeof(Mtx), 1, 2, 2);
    (*out)->partMtx[1] = allocate_memory(numParts * sizeof(Mtx), 1, 2, 2);
    model = (s32) *out;
    mtx = ((AnimModel *) model)->partMtx[0];
    if (mtx == NULL || ((AnimModel *) model)->partMtx[1] == NULL) {
        if (mtx == NULL) {
            func_10004074(mtx);
            model = (s32) *out;
        }
        mtx = ((AnimModel *) model)->partMtx[1];
        if (mtx == NULL) {
            func_10004074(mtx);
            if (!dlists && !dlists && !dlists) {
            }
            model = (s32) *out;
        }
        func_10004074((AnimModel *) model);
        return 1;
    }
    func_1503F5B8((AnimModel *) model, 1, 0, 1.0f, 0.0f, 0);
    return 0;
}

/* Releases an AnimModel and its part matrices. */
void func_1503F7B8(AnimModel *model) {
    func_100043B4((s32 *)model->partMtx[0], 4);
    func_100043B4((s32 *)model->partMtx[1], 4);
    func_10004074(model);
}
