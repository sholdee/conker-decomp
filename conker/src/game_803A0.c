#include <ultra64.h>

#include "functions.h"
#include "variables.h"

u8 *func_15033E84(struct127 *arg0);

/* Sets the object's immunity timer and heading (in degrees, from its facing angle) and starts animation 0. */
void func_15052EF0(struct127 *obj) {
    obj->immune = 100;
    // binary angle (0x10000 per turn) to degrees, a quarter turn ahead of the facing angle
    obj->unk40 = (s16)(obj->unk7A + 0x4000) * 0.005493164f;
    func_1505E650(obj, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
}

/* Resets object `index` (unk13C, unk218) and sets its unk232; returns the object. */
struct127 *func_15052F58(s32 index, s32 arg1) {
    struct127 *obj;

    obj = &D_800CC2D0[index];
    obj->unk13C = 0;
    obj->unk218 = NULL;
    obj->unk232 = arg1;
    return obj;
}

/*
 * When the object linked by arg0->unk124 is falling below arg0's top (y + height * y_scale), or always if
 * `force` is set: records it on arg0 (unk13C), copies arg0's facing angle to it and resets its state; with
 * arg0 in interaction_state 0x20 it is also stopped and turned to face arg0's way.
 */
void func_15052F9C(struct127 *arg0, f32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 immune, s32 arg7,
                   s32 force, s32 arg9) {
    struct127 *obj;
    s32 anim;
    s32 callF28;
    u8 *info;
    u16 pad; // matching: unused stack slot
    s32 state;

    callF28 = FALSE;
    anim = -1;
    if (arg0->id == 0x48) {
        info = func_15033E84(arg0);
        if (info != NULL) {
            anim = info[6];
        }
    }
    obj = &D_800CC2D0[arg0->unk124];
    if (force || ((obj->y_position < arg0->y_position + height * arg0->y_scale) && (obj->y_velocity < 0.0f))) {
        arg0->unk13C = arg0->unk124 + 100;
        if (arg9 != 0) {
            D_800D1580 = arg9;
            func_1506E5FC();
        }
        obj->unk1CC = obj->y_position;
        obj->unk76 = arg0->unk7A;
        obj->unk31C->unk30 = 0;
        if (arg0->interaction_state == 0x20) {
            obj->y_velocity = 0.0f;
            state = 2;
            arg0->y_velocity = 0.0f;
            if (arg0->id == 0x48) {
                if (anim == 0x7A) {
                    arg0->unk65 = arg0->unk124 + 1;
                    arg0->unk5C = arg2;
                    arg0->unk101 |= arg3;
                    func_1505E650(obj, 5, 1.0f, 0.0f, 0.0f, 0.0f, 0);
                    func_15083568(obj, 0x79, 1.0f, 0);
                    callF28 = TRUE;
                }
            } else {
                arg0->y_scale = 0.75f;
                obj->y_position = arg0->y_position;
                state = 1;
                if (D_800BE9F0 == 6) {
                    state = 0x81;
                }
            }
            obj->unk76 = arg0->unk7A;
            obj->unk78 = obj->unk76;
            obj->unk7A = obj->unk76;
            obj->unk40 = (s16)(obj->unk76 + 0x4000) * 0.005493164f; // binary angle to degrees
            obj->unk31C->unk4E = state;
            obj->unk83 = 0;
            obj->disable_run = 0;
        // matching: an empty label here makes IDO fill the branch delay slot as the original
        end_hold:;
        } else {
            obj->unk65 = D_800C3E78 + 1;
            obj->unk5C = arg2;
            obj->unk101 |= arg3;
            obj->disable_run = 0;
        }
        obj->immune = immune;
        obj->unk239 = arg7;
        obj->unk31C->unk27 = 0;
        arg0->unk232 = arg5;
        arg0->unk218 = NULL;
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
 * unk31C target x/z to arg0's position, or to the translation of matrix `mtxIndex` of arg0->unk1D4.
 */
void func_1505327C(struct127 *arg0, f32 yVelocity, f32 gravity, s32 arg3, s32 mtxIndex) {
    s32 pad[2]; // matching: frame keeps 8 unused bytes above pos
    vertex pos;
    struct127 *obj;

    obj = &D_800CC2D0[arg0->unk124];
    if ((obj->disable_run == 0) && (obj->stunned == 0)) {
        if ((obj->unk31C != NULL) && (obj->unk31C->unk6B != 0)) {
            return;
        }
        obj->immune = 0xFF;
        obj->y_velocity = yVelocity;
        obj->gravity = gravity;
        obj->unk83 = 0xFF;
        obj->disable_run = 0xFF;
        obj->unk76 = arg0->unk7A;
        arg0->unk232 = arg3;
        arg0->unk218 = NULL;
        func_1505E650(obj, 50, 1.3f, 4.0f, 0.0f, 0.0f, 0);
        D_800CC2D0[arg0->unk124].unk31C->unk30 = 70;
        // unk28/unk2C are declared s32 in ActorCtl but hold floats
        *(f32 *)&D_800CC2D0[arg0->unk124].unk31C->unk28 = arg0->x_position;
        *(f32 *)&D_800CC2D0[arg0->unk124].unk31C->unk2C = arg0->z_position;
        if ((arg0->unk1D4 != NULL) && (mtxIndex != -1)) {
        // matching: an unused label keeps IDO from sinking the index shift below the -1 test
        joint:
            func_15043FF0(&pos, &((struct113 *)arg0->unk1D4)[mtxIndex]);
            *(f32 *)&D_800CC2D0[arg0->unk124].unk31C->unk28 = pos.x;
            *(f32 *)&D_800CC2D0[arg0->unk124].unk31C->unk2C = pos.z;
        }
        D_800CC2D0[arg0->unk124].unk31C->unk27 = 1;
    }
}
