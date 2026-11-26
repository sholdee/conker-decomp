#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Refreshes the cached height/width/depth (ActorCtl height, width, depth) of every object that has one. */
void actor_updateSizes(void) {
    s32 i;
    Actor *obj = movement_obj;

    for (i = 0; i < D_8008FD8C; i++, obj++) {
        ActorCtl *ext = obj->mv;
        if (ext != NULL) {
            actor_computeSize(obj, &ext->height, &ext->width, &ext->depth);
        }
    }
}

/* actor_computeSize: the original has compiled-out debug code after the size selection (ra and
 * $f20 are saved and a1-a3 homed although no call survives). The dbg* names below are
 * stand-ins that are never referenced by the emitted code; the dead blocks only steer
 * uopt's live-range splitting (height: split, in $f18 across the selection; depth: $f20;
 * width: stack). See ~/conker-ws/actor_computeSize/notes.md for the full ledger. */
void dbg3(f32, f32, f32);
void dbgp(f32 *);
void dbgu();
extern f32 dbgx[];
/* Computes an object's height, width and depth (each output may be NULL): fixed sizes for
 * special states and object ids, otherwise its unkE6/unkE4, scaled by scale_y/scale_xz. */
void actor_computeSize(Actor *obj, s16 *outHeight, s16 *outWidth, s16 *outDepth) {
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
    if (obj->type == 0) {
        width = depth = 60.0f;
        height = 180.0f;
    } else if (obj->type == 45) {
        height = obj->CameraPtr->unk960;
        width = depth = obj->CameraPtr->unk95C;
    } else if (obj->type == 4) {
        height = obj->bodyHeight * 6.0f;
        width = depth = obj->bodyRadius * 3.0f;
    } else if (obj->type == 44) {
        width = depth = 80.0f;
        height = 160.0f;
    } else if (obj->drawtype == 5) {
        height = obj->bodyHeight * 2.0f;
        width = depth = obj->bodyRadius;
    } else {
        switch (obj->character_type) {
        case 0: case 1: case 2: case 3: case 4:
        case 59:
        case 117: case 128: case 130: case 136: case 144: case 150: case 152:
        case 156: case 157: case 159: case 160: case 176: case 177: case 178: case 180:
            ext = obj->mv;
            if (ext != NULL && ext->duck_tilt != 0) {
                width = depth = 60.0f;
                height = 80.0f;
            } else if (ext != NULL && ext->env_suit != 0) {
                width = depth = 70.0f;
                height = 300.0f;
            } else if (obj->inwater) {
                height = 180.0f;
                width = 90.0; depth = 90.0f; // matching: double literal un-CSEs the constant
            } else {
                width = 60.0f;
                depth = 60.0f;
                height = 180.0f;
                if (ext != NULL) {
                    if ((obj->CameraPtr != NULL && obj->CameraPtr->DoingAim != 0) ||
                        ext->SniperState != 0 || ext->unk1B3 != 0) {
                        width *= 2.0f; depth *= 2.0f;
                    }
                    height += ext->unk1A6;
                }
            }
            if (ext != NULL && (ext->transport & 0xF) == 1 && ext->transport_surface == 0 &&
                obj->speed < 40.0f) {
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
            if (gIsMultiplayer != 0) {
                width = depth = 150.0f;
                height = 300.0f;
            } else {
                height = 250.0f;
                width = 90.0; depth = 90.0f; // matching: double literal un-CSEs the constant
            }
            break;
        default:
            applyScale = 0;
            height = obj->bodyHeight * 2.0f;
            width = depth = obj->bodyRadius;
            break;
        }
        if (obj->lifting > 100 && movement_obj[obj->lifting - 100].character_type == 19) {
            depth += 120.0f * obj->scale_xz;
            height += 100.0f * obj->scale_y;
        }
        if (applyScale) {
            f32 xzScale;
            xzScale = obj->scale_xz;
            height *= obj->scale_y;
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
