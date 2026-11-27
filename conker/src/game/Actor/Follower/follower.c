#include <ultra64.h>
#include "conker/actor.h"
#include "conker/event.h"
#include "conker/game.h"

/* matching: not in actor.h, as game/Effects/TintedObject/tintedobject.c was compiled with an s32 return (definition: game/Actor/Movement/movement.c) */
void actor_seatRider(Actor *mount, f32 height, s32 baseMtx, s32 attachFlags, s32 hideRider, s32 mountWalk, s32 immune,
                   s32 hitRoute, s32 force, s32 soundCmd);

/*
 * Per-frame update: copy the owning object's angles (unk13C is the owner's index + 100), then either
 * start the runState action or, in searchState, target the first eligible object in the active mask.
 */
void actor_updateFollower(Actor *self) {
    s32 searchState;
    s32 runState;
    s32 mountWalk;
    s32 baseMtx;
    Actor *other;
    Actor *owner;
    f32 angle;
    s32 i;
    s32 mask;

    searchState = 1;
    if (self->character_type == 0x23) {
        searchState = 10;
        runState = 11;
        mountWalk = 12;
        baseMtx = 1;
    } else {
        runState = 2;
        mountWalk = 3;
        baseMtx = 3;
    }
    self->taskint[1] = 0;
    self->forcedCullMask = 0;
    self->taskint[0] -= self->taskint[0] >> 3;
    if (1) {} // matching: splits the block here; without it IDO swaps the owner/state registers
    owner = movement_obj;
    if (gIsMultiplayer != 0) {
        self->unk66 &= ~0x20;
    }
    mask = self->lifting;
    if (mask != 0) {
        owner += mask - 100;
        if (owner->CameraPtr != NULL) {
            self->forcedCullMask = 1 << owner->CameraPtr->CNumber;
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
    actor_updateCommon(self);
    if (runState == self->intelwalk) {
        actor_seatRider(self, 100.0f, baseMtx, 4, 0, mountWalk, 0, 0, 0, 0);
        return;
    }
    if (searchState == self->intelwalk && (~gEliminatedPlayers & gCollidedActors) && event_getOwnedObject(self, 0) == NULL) {
        mask = ~gEliminatedPlayers & gCollidedActors;
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
                actor_startRiderJump(self, 44.0f, 3.2f, runState, baseMtx);
                other->mv->gun_before_reload = 600;
                other->mv->ReloadTimer = 0;
                return;
            }
        }
    }
}
