#include <ultra64.h>
#include "conker/actor.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/game.h"
#include "conker/save.h"
#include "conker/screen.h"

/* matching: not in actor.h, as game/Effects/TintedObject/tintedobject.c was compiled with an s32 return (definition: game/Actor/Movement/movement.c) */
void actor_seatRider(Actor *mount, f32 height, s32 baseMtx, s32 attachFlags, s32 hideRider, s32 mountWalk, s32 immune,
                   s32 hitRoute, s32 force, s32 soundCmd);

/* Action (intelwalk) started by a jump (1) or run (2) press; +2 when gIsMultiplayer is set, +4 for id 0x21. */
static const u8 gMountActionWalks[8] = { 0x06, 0x01, 0x21, 0x22, 0x11, 0x11, 0x00, 0x00 };
/* Three 2-entry tables indexed by isType21: at 0 the mount's riding intelwalk, at 2 the mountWalk and at 4 the
   baseMtx / mtxIndex for actor_seatRider and actor_startRiderJump; the last two bytes are not read here. */
static const u8 gMountSeatParams[8] = { 0x10, 0x0C, 0x0A, 0x0D, 0x01, 0x19, 0x9C, 0xBE };

/*
 * Per-frame control update: picks an action (unk232) from the button word gActorPressedButtons, sets the
 * target speed (unk44) and heading (unk78), then eases unkC4 toward the clamped turn. Objects
 * with id 0x21 use their own tables and a random wandering heading.
 */
void actor_updateMount(Actor *obj) {
    ActorCtl *ctl;
    u16 heading;
    u16 prevFacing;
    u8 isType21;
    u8 action;
    s16 turn;
    s16 v; // matching: one s16 for both the control index and the 3/4 anim value merges their registers

    isType21 = 0;
    prevFacing = obj->langle;
    if (obj->character_type == 0x21) {
        isType21 = 1;
    }
    v = obj->lifting;
    if ((v == 0) && (gIsMultiplayer == 0)) {
        goto control_done; // matching: a goto (not an if block) lets IDO keep gIsMultiplayer in a register
    }
    action = 0;
    if (gIsMultiplayer != 0) {
        if (obj->mv->bot != 0) {
            actor_updateButtonTimers(obj);
        } else {
            gActorPressedButtons = gButtonsPressed[dc_glbl_CharNum];
        }
    } else {
        gActorPressedButtons = gButtonsPressed[v - 100];
    }
    if ((gActorPressedButtons & 0x8000) && (obj->nojump == 0) && (obj->shad == 0.0f)) {
        action = 1;
    }
    if ((gActorPressedButtons & 0x6000) && (obj->shad == 0.0f) &&
        ((isType21 && (obj->norun != 0xFF)) || (obj->norun == 0))) {
        action = 2;
    }
    if (action) {
        if (isType21) {
            action += 4;
        }
        if (gIsMultiplayer != 0) {
            action += 2;
        }
        obj->norun = 180;
        obj->intelwalk = gMountActionWalks[action - 1];
        obj->scriptCmd = NULL;
        if ((obj->intelwalk == 0x22) && (obj->taskint[0] == 0)) {
            obj->mv->shotsfired++;
        }
    }
    obj->owner = 0;
    if ((obj->norun == 0) && ((gIsMultiplayer == 0) || (obj->mv->bot == 0))) {
        if (gIsMultiplayer != 0) {
            /* camera yaw in degrees -> 16-bit binary angle (65536 / 360) */
            gCameraYawAngle = camera_global_Cameras[dc_glbl_CharNum].CameraAngle * 182.0444488525390625f;
            heading = actor_getDirectionAngle(gCurrentPad->stick_x, gCurrentPad->stick_y, 0) + gCameraYawAngle;
            obj->stickspeed = actor_getStickDeflection(gCurrentPad);
        } else {
            obj->stickspeed = movement_obj[obj->owner].stickspeed * 1.75f;
            heading = movement_obj[obj->owner].mv->press_angle;
        }
        if (obj->moveMode == 13) {
            obj->stickspeed = 0.0f;
        }
        if (isType21) {
            if (obj->stickspeed < 5.0f) {
                heading = math_random();
                obj->stickspeed = (math_random() % 60U) + 20.0f;
            }
            obj->fangle = ((math_random() % 10000U) + heading) - 5000;
            obj->stickspeed = (obj->stickspeed * 0.5f) + 22.0f;
            if ((u8)(((obj->angle - obj->fangle) >> 8) - 0x50) < 0x60) {
                obj->scriptCmd = NULL;
                obj->intelwalk = 0x10;
            }
        } else if (obj->stickspeed < 5.0f) {
            obj->stickspeed = 0.0f;
            if (obj->intelwalk != 5) {
                obj->fangle = obj->angle;
            }
        } else {
            obj->fangle = heading;
            if (obj->moveMode == 1) {
                v = 3;
                if (obj->speed < 35.0f) {
                    v = 4;
                }
                obj->faceTurnRate = v;
                obj->moveTurnRate = v;
                if ((u8)(((obj->angle - obj->fangle) >> 8) - 0x50) < 0x60) {
                    if (obj->speed > 35.0f) {
                        obj->intelwalk = 4;
                    } else {
                        obj->intelwalk = 5;
                    }
                } else {
                    obj->intelwalk = 3;
                }
                obj->scriptCmd = NULL;
            }
        }
    }
control_done:
    if ((gIsMultiplayer == 0) || (dc_glbl_CharNum >= gPlayerCount) || (obj->mv->death_State == 0)) {
        actor_updateCommon(obj);
    }
    if ((movement_obj[0].mv->jump_on_char != 0) && ((obj->intelwalk != gMountSeatParams[isType21]) || (obj->stunned != 0))) {
        movement_obj[0].mv->jump_on_char = 0;
        movement_obj[0].norun = 0;
        movement_obj[0].count5 = 0;
        movement_obj[0].imune = 0;
        obj->intelwalk = 0x11;
    }
    if (gIsMultiplayer == 0) {
        /* gMountSeatParams holds three 2-entry tables indexed by isType21 */
        // matching: (&table[i])[n] adds n after i; table[i + n] folds n into the address
        if (obj->intelwalk == gMountSeatParams[isType21]) {
            actor_seatRider(obj, 156.0f, (&gMountSeatParams[isType21])[4], 4, 0, (&gMountSeatParams[isType21])[2], 0xFF, 0, 0, 0);
        } else if ((gCollidedActors & 1) && (movement_obj[obj->owner].attach_char == 0) && (obj->task_attrib & 0x400)) {
            obj->owner = 0;
            actor_startRiderJump(obj, 50.0f, 3.0f, gMountSeatParams[isType21], (&gMountSeatParams[isType21])[4]);
        }
    }
    turn = 0;
    if ((isType21 && (obj->speed > 5.0f)) || (!isType21 && (obj->animseq.uh == 10))) {
        turn = obj->angle - obj->fangle;
        if (turn > 0x3000) {
            turn = 0x3000;
        }
        if (turn < -0x3000) {
            turn = -0x3000;
        }
        if (isType21 && (obj->lifting == 0)) {
            turn >>= 1;
        }
    }
    obj->lean_angle += ((turn * 0.005f) - obj->lean_angle) * 0.15f;
    if (isType21) {
        obj->taskint[0] += ((s16)((prevFacing - obj->langle) * 4) - (s16)obj->taskint[0]) / 8;
        obj->taskint[1] += (-turn - obj->taskint[1]) / 8;
    }
    if ((gIsMultiplayer != 0) && (gCollidedActors != 0)) {
        ctl = obj->mv;
        if ((ctl != NULL) && (ctl->on_fire != 0)) {
            actor_igniteTouching(obj);
        }
    }
}
