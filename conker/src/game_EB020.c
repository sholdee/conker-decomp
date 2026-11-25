#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15052590(Actor *obj);
void func_15052F9C(Actor *obj, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void func_1505327C(Actor *obj, f32 arg1, f32 arg2, s32 arg3, s32 arg4);

/*
 * Per-frame update: copy the owning object's angles (unk13C is the owner's index + 100), then either
 * start the runState action or, in searchState, target the first eligible object in the active mask.
 */
void func_150BDB70(Actor *self) {
    s32 searchState;
    s32 runState;
    s32 runArg;
    s32 stateArg;
    Actor *other;
    Actor *owner;
    f32 angle;
    s32 i;
    s32 mask;

    searchState = 1;
    if (self->id == 0x23) {
        searchState = 10;
        runState = 11;
        runArg = 12;
        stateArg = 1;
    } else {
        runState = 2;
        runArg = 3;
        stateArg = 3;
    }
    self->unk2E8 = 0;
    self->unk2FC = 0;
    self->unk2E4 -= self->unk2E4 >> 3;
    if (1) {} // matching: splits the block here; without it IDO swaps the owner/state registers
    owner = D_800CC2D0;
    if (D_800BE616 != 0) {
        self->unk66 &= ~0x20;
    }
    mask = self->unk13C;
    if (mask != 0) {
        owner += mask - 100;
        if (owner->camera != NULL) {
            self->unk2FC = 1 << owner->camera->playerIndex;
        }
        // matching: the state pointer must reuse mask's register (a separate local grows the frame)
        mask = (s32) owner->unk31C;
        if (mask != 0 && ((ActorCtl *) mask)->aimState != 0) {
            self->unk7A = (s32) (((ActorCtl *) mask)->aimYaw * (65536.0f / 360.0f));
            self->unk76 = self->unk7A;
            angle = owner->unk31C->aimPitch;
            if (angle > 180.0f) {
                angle -= 360.0f;
            }
            angle *= 10.0f;
            self->unk2E4 = angle;
            self->unk2E8 = 1;
            self->unk66 |= 0x20;
        }
    }
    func_15052590(self);
    if (runState == self->unk232) {
        func_15052F9C(self, 100.0f, stateArg, 4, 0, runArg, 0, 0, 0, 0);
        return;
    }
    if (searchState == self->unk232 && (~D_800D18A0 & D_800CC268) && func_15072208(self, 0) == NULL) {
        mask = ~D_800D18A0 & D_800CC268;
        for (i = 0; i < 25; i++) {
            while (!(mask & (1 << i)) && i < 25) {
                i++;
            }
            if (i >= 25) {
                return;
            }
            other = &D_800CC2D0[i];
            if (other->unk127 != 0xFF && other->unk65 == 0 && other->unk31C->unk19B == 0 &&
                other->unk31C->aimState == 0 && other->unk13C == 0) {
                self->unk124 = i;
                func_1505327C(self, 44.0f, 3.2f, runState, stateArg);
                other->unk31C->unk24 = 600;
                other->unk31C->unk19E = 0;
                return;
            }
        }
    }
}
