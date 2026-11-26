#include <ultra64.h>

#include "functions.h"
#include "variables.h"


/* Sets the object's immunity timer and heading (in degrees, from its facing angle) and starts animation 0. */
void func_15052EF0(Actor *obj) {
    obj->imune = 100;
    // binary angle (0x10000 per turn) to degrees, a quarter turn ahead of the facing angle
    obj->theta = (s16)(obj->langle + 0x4000) * 0.005493164f;
    func_1505E650(obj, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

/* Resets object `index` (unk13C, unk218) and sets its unk232; returns the object. */
Actor *func_15052F58(s32 index, s32 arg1) {
    Actor *obj;

    obj = &movement_obj[index];
    obj->lifting = 0;
    obj->scriptCmd = NULL;
    obj->intelwalk = arg1;
    return obj;
}

/*
 * When the object linked by arg0->unk124 is falling below arg0's top (y + height * scale_y), or always if
 * `force` is set: records it on arg0 (unk13C), copies arg0's facing angle to it and resets its state; with
 * arg0 in type 0x20 it is also stopped and turned to face arg0's way.
 */
void func_15052F9C(Actor *arg0, f32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 immune, s32 arg7,
                   s32 force, s32 arg9) {
    Actor *obj;
    s32 anim;
    s32 callF28;
    ModelAttachment *info;
    u16 pad; // matching: unused stack slot
    s32 state;

    callF28 = FALSE;
    anim = -1;
    if (arg0->character_type == 0x48) {
        info = func_15033E84(arg0);
        if (info != NULL) {
            anim = info->setId;
        }
    }
    obj = &movement_obj[arg0->owner];
    if (force || ((obj->ypos < arg0->ypos + height * arg0->scale_y) && (obj->yspd < 0.0f))) {
        arg0->lifting = arg0->owner + 100;
        if (arg9 != 0) {
            D_800D1580 = arg9;
            func_1506E5FC();
        }
        obj->unk1CC = obj->ypos;
        obj->angle = arg0->langle;
        obj->mv->gpoint = 0;
        if (arg0->type == 0x20) {
            obj->yspd = 0.0f;
            state = 2;
            arg0->yspd = 0.0f;
            if (arg0->character_type == 0x48) {
                if (anim == 0x7A) {
                    arg0->attach_char = arg0->owner + 1;
                    arg0->basemtx = arg2;
                    arg0->unk101 |= arg3;
                    func_1505E650(obj, 5, 1.0f, 0.0f, 0.0f, 0.0f, 0);
                    func_15083568(obj, 0x79, 1.0f, 0);
                    callF28 = TRUE;
                }
            } else {
                arg0->scale_y = 0.75f;
                obj->ypos = arg0->ypos;
                state = 1;
                if (level == 6) {
                    state = 0x81;
                }
            }
            obj->angle = arg0->langle;
            obj->fangle = obj->angle;
            obj->langle = obj->angle;
            obj->theta = (s16)(obj->angle + 0x4000) * 0.005493164f; // binary angle to degrees
            obj->mv->transport = state;
            obj->count5 = 0;
            obj->norun = 0;
        // matching: an empty label here makes IDO fill the branch delay slot as the original
        end_hold:;
        } else {
            obj->attach_char = dc_glbl_CharNum + 1;
            obj->basemtx = arg2;
            obj->unk101 |= arg3;
            obj->norun = 0;
        }
        obj->imune = immune;
        obj->intelhitroute = arg7;
        obj->mv->jump_on_char = 0;
        arg0->intelwalk = arg5;
        arg0->scriptCmd = NULL;
        if (arg4 != 0) {
            func_1506160C(obj, 1, 0, 0, 0);
            obj->unk101 |= 0x40;
        }
    }
    if (callF28) {
        func_15060F28(arg0, 0);
    }
}

/*
 * Launches the object linked by arg0->unk124 (velocity/gravity given), starts animation 50 and sets its
 * mv target x/z to arg0's position, or to the translation of matrix `mtxIndex` of arg0->ob_matrix_ptr.
 */
void func_1505327C(Actor *arg0, f32 yVelocity, f32 gravity, s32 arg3, s32 mtxIndex) {
    s32 pad[2]; // matching: frame keeps 8 unused bytes above pos
    Vec3f pos;
    Actor *obj;

    obj = &movement_obj[arg0->owner];
    if ((obj->norun == 0) && (obj->stunned == 0)) {
        if ((obj->mv != NULL) && (obj->mv->unk6B != 0)) {
            return;
        }
        obj->imune = 0xFF;
        obj->yspd = yVelocity;
        obj->ygrav = gravity;
        obj->count5 = 0xFF;
        obj->norun = 0xFF;
        obj->angle = arg0->langle;
        arg0->intelwalk = arg3;
        arg0->scriptCmd = NULL;
        func_1505E650(obj, 50, 1.3f, 4.0f, 0.0f, 0.0f, 0);
        movement_obj[arg0->owner].mv->gpoint = 70;
        movement_obj[arg0->owner].mv->swing_xvel = arg0->xpos;
        movement_obj[arg0->owner].mv->swing_zvel = arg0->zpos;
        if ((arg0->ob_matrix_ptr != NULL) && (mtxIndex != -1)) {
        // matching: an unused label keeps IDO from sinking the index shift below the -1 test
        joint:
            math_mtxGetTranslation(&pos, &((Mtx *)arg0->ob_matrix_ptr)[mtxIndex]);
            movement_obj[arg0->owner].mv->swing_xvel = pos.x;
            movement_obj[arg0->owner].mv->swing_zvel = pos.z;
        }
        movement_obj[arg0->owner].mv->jump_on_char = 1;
    }
}
