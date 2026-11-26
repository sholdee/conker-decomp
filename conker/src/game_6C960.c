#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Builds this frame buffer's part matrices from the current animation frame, then advances the animation. */
void func_1503F4B0(AnimInstance *anim) {
    u8 *frameBuf;
    s16 pad42; // matching: unused, keeps unk8 at sp+0x40
    s16 unk8;
    s32 pad3C; // matching: unused, keeps mtx at sp+0x38
    Mtx *mtx;
    f32 (*transform)[4];

    frameBuf = NULL;
    if (anim->matrices[D_800BE9C0] != NULL) {
        frameBuf = allocate_memory(0x320, 1, 2, 2);
        actor_loadAnimFrames(anim, NULL, frameBuf);
        unk8 = 0x1000;
        transform = anim->matrices[D_800BE9C0];
        mtx = anim->partMtx[D_800BE9C0];
        func_150A81D0(&mtx, transform, anim, anim->unk3F0, anim->numParts, &unk8, NULL, 0);
        func_150A9984(anim->partMtx[D_800BE9C0], anim->numParts);
        anim->unk3F6 = TRUE;
    }
    func_1507BDB0(anim, gFrameDelta, NULL, 0);
    if (frameBuf != NULL) {
        func_10004074(frameBuf);
    }
}

/* Starts animation `anim` from the model's bank. */
void func_1503F5B8(AnimInstance *model, s32 restart, s32 anim, f32 speed, f32 blendFrames, s32 flag8000) {
    actor_startAnimState(NULL, NULL, model, 0, restart, anim, model->bank, speed, blendFrames, 0.0f, 0.0f, flag8000);
}

/*
 * Loads model `model` (see func_1502FE10) and creates its AnimInstance in *out, playing animation 0.
 * Returns 1 on failure.
 */
s32 func_1503F62C(s32 model, s32 bank, Gfx ***dlists, u8 *numDLists, u8 **data, s32 *arg5, AnimInstance **out) {
    s32 numParts;
    Mtx *mtx;
    s32 pad; // matching: unused stack slot

    *out = allocate_memory(sizeof(AnimInstance), 1, 2, 2);
    if (*out == NULL) {
        return 1;
    }
    bzero(*out, 0x40);
    (*out)->header[1][5] = 1;
    (*out)->header[0][5] = 1;
    func_1502FE10(model, dlists, numDLists, data, arg5, &(*out)->unk3F0, &numParts);
    (*out)->numParts = numParts;
    (*out)->bank = bank;
    (*out)->unk3F6 = FALSE;
    (*out)->partMtx[0] = allocate_memory(numParts * sizeof(Mtx), 1, 2, 2);
    (*out)->partMtx[1] = allocate_memory(numParts * sizeof(Mtx), 1, 2, 2);
    // matching: model's variable is reused for *out; a separate AnimInstance * local makes IDO spill model
    model = (s32) *out;
    mtx = ((AnimInstance *) model)->partMtx[0];
    if (mtx == NULL || ((AnimInstance *) model)->partMtx[1] == NULL) {
        // Frees only the buffers that failed to allocate (NULL), leaking the other one.
        if (mtx == NULL) {
            func_10004074(mtx);
            model = (s32) *out;
        }
        mtx = ((AnimInstance *) model)->partMtx[1];
        if (mtx == NULL) {
            func_10004074(mtx);
            // matching: dead uses of dlists; without them IDO swaps s0/s1 for model and out
            if (!dlists && !dlists && !dlists) {
            }
            model = (s32) *out;
        }
        func_10004074((AnimInstance *) model);
        return 1;
    }
    func_1503F5B8((AnimInstance *) model, 1, 0, 1.0f, 0.0f, 0);
    return 0;
}

/* Releases an AnimInstance and its part matrices. */
void func_1503F7B8(AnimInstance *model) {
    func_100043B4(model->partMtx[0], 4);
    func_100043B4(model->partMtx[1], 4);
    func_10004074(model);
}
