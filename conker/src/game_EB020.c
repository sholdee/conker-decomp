#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Defined in game_803A0.c. Not in actor.h: game_1048D0.c was compiled with an s32 return. */
void func_15052F9C(Actor *arg0, f32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 immune, s32 arg7,
                   s32 force, s32 arg9);

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
    if (self->character_type == 0x23) {
        searchState = 10;
        runState = 11;
        runArg = 12;
        stateArg = 1;
    } else {
        runState = 2;
        runArg = 3;
        stateArg = 3;
    }
    self->taskint[1] = 0;
    self->unk2FC = 0;
    self->taskint[0] -= self->taskint[0] >> 3;
    if (1) {} // matching: splits the block here; without it IDO swaps the owner/state registers
    owner = movement_obj;
    if (D_800BE616 != 0) {
        self->unk66 &= ~0x20;
    }
    mask = self->lifting;
    if (mask != 0) {
        owner += mask - 100;
        if (owner->CameraPtr != NULL) {
            self->unk2FC = 1 << owner->CameraPtr->CNumber;
        }
        // matching: the state pointer must reuse mask's register (a separate local grows the frame)
        mask = (s32) owner->mv;
        if (mask != 0 && ((ActorCtl *) mask)->SniperState != 0) {
            self->langle = (s32) (((ActorCtl *) mask)->aimYaw * (65536.0f / 360.0f));
            self->angle = self->langle;
            angle = owner->mv->aimPitch;
            if (angle > 180.0f) {
                angle -= 360.0f;
            }
            angle *= 10.0f;
            self->taskint[0] = angle;
            self->taskint[1] = 1;
            self->unk66 |= 0x20;
        }
    }
    func_15052590(self);
    if (runState == self->intelwalk) {
        func_15052F9C(self, 100.0f, stateArg, 4, 0, runArg, 0, 0, 0, 0);
        return;
    }
    if (searchState == self->intelwalk && (~D_800D18A0 & D_800CC268) && func_15072208(self, 0) == NULL) {
        mask = ~D_800D18A0 & D_800CC268;
        for (i = 0; i < 25; i++) {
            while (!(mask & (1 << i)) && i < 25) {
                i++;
            }
            if (i >= 25) {
                return;
            }
            other = &movement_obj[i];
            if (other->playernumber != 0xFF && other->attach_char == 0 && other->mv->GrenadeThrowState == 0 &&
                other->mv->SniperState == 0 && other->lifting == 0) {
                self->owner = i;
                func_1505327C(self, 44.0f, 3.2f, runState, stateArg);
                other->mv->gun_before_reload = 600;
                other->mv->ReloadTimer = 0;
                return;
            }
        }
    }
}
