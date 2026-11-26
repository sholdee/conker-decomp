#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Defined in game_803A0.c. Not in actor.h: game_1048D0.c was compiled with an s32 return. */
void func_15052F9C(Actor *arg0, f32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 immune, s32 arg7,
                   s32 force, s32 arg9);

/* Action (intelwalk) started by a jump (1) or run (2) press; +2 when D_800BE616 is set, +4 for id 0x21. */
const u8 D_800A0810[8] = { 0x06, 0x01, 0x21, 0x22, 0x11, 0x11, 0x00, 0x00 };
/* Three 2-entry tables indexed by isType21 (at 0, 2 and 4); the last two bytes are not read here. */
const u8 D_800A0818[8] = { 0x10, 0x0C, 0x0A, 0x0D, 0x01, 0x19, 0x9C, 0xBE };

/*
 * Per-frame control update: picks an action (unk232) from the button word D_800CC288, sets the
 * target speed (unk44) and heading (unk78), then eases unkC4 toward the clamped turn. Objects
 * with id 0x21 use their own tables and a random wandering heading.
 */
void func_150CE740(Actor *obj) {
    ActorCtl *owner;
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
    if ((v == 0) && (D_800BE616 == 0)) {
        goto control_done; // matching: a goto (not an if block) lets IDO keep D_800BE616 in a register
    }
    action = 0;
    if (D_800BE616 != 0) {
        if (obj->mv->bot != 0) {
            func_1504C854(obj);
        } else {
            D_800CC288 = D_800BE710[dc_glbl_CharNum];
        }
    } else {
        D_800CC288 = D_800BE710[v - 100];
    }
    if ((D_800CC288 & 0x8000) && (obj->nojump == 0) && (obj->shad == 0.0f)) {
        action = 1;
    }
    if ((D_800CC288 & 0x6000) && (obj->shad == 0.0f) &&
        ((isType21 && (obj->norun != 0xFF)) || (obj->norun == 0))) {
        action = 2;
    }
    if (action) {
        if (isType21) {
            action += 4;
        }
        if (D_800BE616 != 0) {
            action += 2;
        }
        obj->norun = 180;
        obj->intelwalk = D_800A0810[action - 1];
        obj->scriptCmd = NULL;
        if ((obj->intelwalk == 0x22) && (obj->taskint[0] == 0)) {
            obj->mv->shotsfired++;
        }
    }
    obj->owner = 0;
    if ((obj->norun == 0) && ((D_800BE616 == 0) || (obj->mv->bot == 0))) {
        if (D_800BE616 != 0) {
            /* camera yaw in degrees -> 16-bit binary angle (65536 / 360) */
            D_800CC280 = camera_global_Cameras[dc_glbl_CharNum].CameraAngle * 182.0444488525390625f;
            heading = func_1505A630(D_800CC284->stick_x, D_800CC284->stick_y, 0) + D_800CC280;
            obj->stickspeed = func_1505A5CC(D_800CC284);
        } else {
            obj->stickspeed = movement_obj[obj->owner].stickspeed * 1.75f;
            heading = movement_obj[obj->owner].mv->press_angle;
        }
        if (obj->unk223 == 13) {
            obj->stickspeed = 0.0f;
        }
        if (isType21) {
            if (obj->stickspeed < 5.0f) {
                heading = func_150ADA20();
                obj->stickspeed = (func_150ADA20() % 60U) + 20.0f;
            }
            obj->fangle = ((func_150ADA20() % 10000U) + heading) - 5000;
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
            if (obj->unk223 == 1) {
                v = 3;
                if (obj->speed < 35.0f) {
                    v = 4;
                }
                obj->unk1E6 = v;
                obj->unk1E5 = v;
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
    if ((D_800BE616 == 0) || (dc_glbl_CharNum >= D_8008FD8C) || (obj->mv->death_State == 0)) {
        func_15052590(obj);
    }
    if ((movement_obj[0].mv->jump_on_char != 0) && ((obj->intelwalk != D_800A0818[isType21]) || (obj->stunned != 0))) {
        movement_obj[0].mv->jump_on_char = 0;
        movement_obj[0].norun = 0;
        movement_obj[0].count5 = 0;
        movement_obj[0].imune = 0;
        obj->intelwalk = 0x11;
    }
    if (D_800BE616 == 0) {
        /* D_800A0818 holds three 2-entry tables indexed by isType21 */
        // matching: (&table[i])[n] adds n after i; table[i + n] folds n into the address
        if (obj->intelwalk == D_800A0818[isType21]) {
            func_15052F9C(obj, 156.0f, (&D_800A0818[isType21])[4], 4, 0, (&D_800A0818[isType21])[2], 0xFF, 0, 0, 0);
        } else if ((D_800CC268 & 1) && (movement_obj[obj->owner].attach_char == 0) && (obj->task_attrib & 0x400)) {
            obj->owner = 0;
            func_1505327C(obj, 50.0f, 3.0f, D_800A0818[isType21], (&D_800A0818[isType21])[4]);
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
    if ((D_800BE616 != 0) && (D_800CC268 != 0)) {
        owner = obj->mv;
        if ((owner != NULL) && (owner->on_fire != 0)) {
            func_15063168(obj);
        }
    }
}
