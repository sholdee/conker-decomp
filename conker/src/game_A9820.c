#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_1507C3E0(Actor *obj, s16 *height, s16 *width, s16 *depth);

/* Refreshes the cached height/width/depth (ActorCtl unk114-unk118) of every object that has one. */
void func_1507C370(void) {
    s32 i;
    Actor *obj = D_800CC2D0;

    for (i = 0; i < D_8008FD8C; i++, obj++) {
        ActorCtl *ext = obj->unk31C;
        if (ext != NULL) {
            func_1507C3E0(obj, &ext->unk114, &ext->unk116, &ext->unk118);
        }
    }
}

/* func_1507C3E0: the original has compiled-out debug code after the size selection (ra and
 * $f20 are saved and a1-a3 homed although no call survives). The dbg* names below are
 * stand-ins that are never referenced by the emitted code; the dead blocks only steer
 * uopt's live-range splitting (height: split, in $f18 across the selection; depth: $f20;
 * width: stack). See ~/conker-ws/func_1507C3E0/notes.md for the full ledger. */
void dbg3(f32, f32, f32);
void dbgp(f32 *);
void dbgu();
extern f32 dbgx[];
/* Computes an object's height, width and depth (each output may be NULL): fixed sizes for
 * special states and object ids, otherwise its unkE6/unkE4, scaled by y_scale/xz_scale. */
void func_1507C3E0(Actor *obj, s16 *outHeight, s16 *outWidth, s16 *outDepth) {
    f32 height;
    f32 width;
    f32 depth;
    ActorCtl *ext;
    s32 applyScale;
    s32 pad[9]; // matching: stack frame size and slot of zero
    f32 zero;
    f32 x1;
    f32 x2;
    f32 x3;
    f32 x4;
    f32 x5;
    s32 dbg;
    s32 pad2[8]; // matching: stack frame size

    dbg = 0; // matching: guards the dead debug blocks below
    applyScale = 1;
    zero = 0.0f;
    if (obj->interaction_state == 0) {
        width = depth = 60.0f;
        height = 180.0f;
    } else if (obj->interaction_state == 45) {
        height = obj->camera->unk960;
        width = depth = obj->camera->unk95C;
    } else if (obj->interaction_state == 4) {
        height = obj->unkE6 * 6.0f;
        width = depth = obj->unkE4 * 3.0f;
    } else if (obj->interaction_state == 44) {
        width = depth = 80.0f;
        height = 160.0f;
    } else if (obj->unk5 == 5) {
        height = obj->unkE6 * 2.0f;
        width = depth = obj->unkE4;
    } else {
        switch (obj->id) {
        case 0: case 1: case 2: case 3: case 4:
        case 59:
        case 117: case 128: case 130: case 136: case 144: case 150: case 152:
        case 156: case 157: case 159: case 160: case 176: case 177: case 178: case 180:
            ext = obj->unk31C;
            if (ext != NULL && ext->unk4 != 0) {
                width = depth = 60.0f;
                height = 80.0f;
            } else if (ext != NULL && ext->unk17 != 0) {
                width = depth = 70.0f;
                height = 300.0f;
            } else if (obj->in_water) {
                height = 180.0f;
                width = 90.0; depth = 90.0f; // matching: double literal un-CSEs the constant
            } else {
                width = 60.0f;
                depth = 60.0f;
                height = 180.0f;
                if (ext != NULL) {
                    if ((obj->camera != NULL && obj->camera->aimState != 0) ||
                        ext->aimState != 0 || ext->unk1B3 != 0) {
                        width *= 2.0f; depth *= 2.0f;
                    }
                    height += ext->unk1A6;
                }
            }
            if (ext != NULL && (ext->unk4E & 0xF) == 1 && ext->unk4F == 0 &&
                obj->xz_velocity < 40.0f) {
                width = depth = 110.0f;
                height = 500.0f;
            }
            break;
        case 37: case 46:
            height = 70.0f;
            width = 90.0; depth = 90.0f; // matching: double literal un-CSEs the constant
            break;
        case 54:
            height = 257.0f;
            width = 90.0; depth = 90.0f; // matching: double literal un-CSEs the constant
            break;
        case 154:
            height = 60.0f;
            width = depth = 45.0f;
            applyScale = 0;
            break;
        case 172:
            width = depth = 30.0f;
            height = 90.0f;
            applyScale = 0;
            break;
        case 137: case 186:
            height = 60.0f;
            width = depth = 30.0f;
            applyScale = 0;
            break;
        case 83:
            if (D_800BE616 != 0) {
                width = depth = 150.0f;
                height = 300.0f;
            } else {
                height = 250.0f;
                width = 90.0; depth = 90.0f; // matching: double literal un-CSEs the constant
            }
            break;
        default:
            applyScale = 0;
            height = obj->unkE6 * 2.0f;
            width = depth = obj->unkE4;
            break;
        }
        if (obj->unk13C > 100 && D_800CC2D0[obj->unk13C - 100].id == 19) {
            depth += 120.0f * obj->xz_scale;
            height += 100.0f * obj->y_scale;
        }
        if (applyScale) {
            f32 xzScale;
            xzScale = obj->xz_scale;
            height *= obj->y_scale;
            width *= xzScale;
            depth *= xzScale;
            goto end; // matching: falling out of the if/else schedules the tail differently
        }
    }
end:
    if (!dbg) { } else { // matching: a plain if (dbg) hoists the height/width loads
        dbg3(height, depth, depth);
        x1 = dbgx[0x10]; x2 = dbgx[0x11]; x3 = dbgx[0x12]; x4 = dbgx[0x13]; x5 = dbgx[0x14];
        dbgx[0x15] = x1 * x2 * x3 * x4 * x5; dbgx[0x16] = x1 + x2 + x3 + x4 + x5;
        dbgp(&zero); dbgp(&width); dbgu(); dbgu();
    }
    if (outHeight != NULL) {
        *outHeight = height;
    }
    if (outWidth != NULL) {
        *outWidth = width;
    }
    if (outDepth != NULL) {
        *outDepth = depth;
    }
    if (dbg) { dbgu(); dbgu(); dbgu(); dbgu(); dbgu(); dbgu(); dbgu(); dbgu(); } // matching: block count drives uopt cost-splitting
}
