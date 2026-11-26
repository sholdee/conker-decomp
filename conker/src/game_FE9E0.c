#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .rodata front-end data (as literals, IDO allocates the FP registers of func_150D1530 differently). */
static const struct {
    f32 speedScale; /* 1 / 170: the speed as a 0..1 fraction */
    f32 fovRange;   /* field of view gained at full speed */
} D_800A08D0 = { 1.0f / 170.0f, 24.599998f };


/* Unless camera->Mode is 0x40: with unk5F0 bit 0x80 set, scales distances (unk348/unk34C/unk374/unk1A4/unk1A8)
 * by the clamped xz speed (0..170, as a 0..1 fraction) and sets unk84 flags from script_query queries;
 * otherwise zeroes unk190/unk1A4/unk1A8 when func_151239CC(camera, CAMERA_MODE_TRANSPORT) succeeds. On level 0x32, also mirrors
 * a script_query query into unk84 bit 0x1000. */
void func_150D1530(Camera *camera) {
    if (camera->Mode == 0x40) {
        return;
    }

    if (camera->SpecialFlags & 0x80) {
        f32 speedFrac;
        s32 found;

        speedFrac = (camera->CharPtr->speed < 0.0f)
                    ? 0.0f
                    : ((170.0f < camera->CharPtr->speed) ? 170.0f : camera->CharPtr->speed);
        speedFrac = speedFrac * D_800A08D0.speedScale;
        if (func_15123934(camera, 8, 0, camera->Tightness, CAMERA_MODE_TRANSPORT) != 0) {
            camera->AvailableModes |= 0x1300080;
            camera->AvailableModes &= ~6;
            camera->DistIndex = 1;
            camera->DistMask = 2;
        }
        camera->IdealTripodMagnitude = -194.0f * speedFrac + 280.0f;
        camera->unk34C = camera->height = 34.0f * speedFrac + 75.0f;
        if (level == 0x32) {
            found = script_query(4, camera->CNumber | 0x2000, 0xAC, 0x4027, 0x4035, 0x4036, 0x4037);
        } else {
            found = script_query(5, camera->CNumber | 0x2000, 0xAC, 0x400A, 0x400B, 0x400C, 0x400D, 0x400E);
        }
        if (found != 0) {
            camera->AvailableModes |= 0x10000000;
        } else {
            if (camera->CharPtr->count3 != 0) {
                camera->YOffsetTget = -30.0f;
            } else {
                camera->YOffsetTget = 123.0f;
            }
            camera->AvailableModes &= ~0x10000000;
        }
        camera->TgetFOVX = camera->TgetFOVY = D_800A08D0.fovRange * speedFrac + 10.0f;
        if (camera->Snap != 0) {
            // matching: the ROM has a real load/store pair per field; the original self-assigned them
            camera->TgetFOVX = camera->TgetFOVX;
            camera->TgetFOVY = camera->TgetFOVY;
        }
    } else if (func_151239CC(camera, CAMERA_MODE_TRANSPORT) != 0) {
        camera->YOffsetTget = camera->TgetFOVX = camera->TgetFOVY = 0.0f;
    }

    if (level == 0x32) {
        if (script_query(1, 0x4039, 6, 0x9000) != 0) {
            camera->AvailableModes |= 0x1000;
        } else {
            camera->AvailableModes &= ~0x1000;
        }
    }
}
