#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// matching: rodata loads; the literals 1/170 and 24.6f allocate FP registers differently
extern f32 D_800A08D0; // 1.0f / 170.0f
extern f32 D_800A08D4; // 24.6f

s32 func_1509BE40();

/* Unless camera->unk2C is 0x40: with unk5F0 bit 0x80 set, scales distances (unk348/unk34C/unk374/unk1A4/unk1A8)
 * by the clamped xz speed (0..170, as a 0..1 fraction) and sets unk84 flags from func_1509BE40 queries;
 * otherwise zeroes unk190/unk1A4/unk1A8 when func_151239CC(camera, 3) succeeds. On level 0x32, also mirrors
 * a func_1509BE40 query into unk84 bit 0x1000. */
void func_150D1530(struct108 *camera) {
    if (camera->unk2C == 0x40) {
        return;
    }

    if (camera->unk5F0 & 0x80) {
        f32 speedFrac;
        s32 found;

        speedFrac = (camera->unk3D0->xz_velocity < 0.0f)
                    ? 0.0f
                    : ((170.0f < camera->unk3D0->xz_velocity) ? 170.0f : camera->unk3D0->xz_velocity);
        speedFrac = speedFrac * D_800A08D0;
        if (func_15123934(camera, 8, 0, camera->unk134, 3) != 0) {
            camera->unk84 |= 0x1300080;
            camera->unk84 &= ~6;
            camera->unk1B4 = 1;
            camera->unk1E0 = 2;
        }
        camera->unk374 = -194.0f * speedFrac + 280.0f;
        camera->unk34C = camera->unk348 = 34.0f * speedFrac + 75.0f;
        if (D_800BE9F0 == 0x32) {
            found = func_1509BE40(4, camera->unk23D | 0x2000, 0xAC, 0x4027, 0x4035, 0x4036, 0x4037);
        } else {
            found = func_1509BE40(5, camera->unk23D | 0x2000, 0xAC, 0x400A, 0x400B, 0x400C, 0x400D, 0x400E);
        }
        if (found != 0) {
            camera->unk84 |= 0x10000000;
        } else {
            if (camera->unk3D0->unk81 != 0) {
                camera->unk190 = -30.0f;
            } else {
                camera->unk190 = 123.0f;
            }
            camera->unk84 &= ~0x10000000;
        }
        camera->unk1A4 = camera->unk1A8 = D_800A08D4 * speedFrac + 10.0f;
        if (camera->unk23C != 0) {
            // matching: the ROM has a real load/store pair per field; the original self-assigned them
            camera->unk1A4 = camera->unk1A4;
            camera->unk1A8 = camera->unk1A8;
        }
    } else if (func_151239CC(camera, 3) != 0) {
        camera->unk190 = camera->unk1A4 = camera->unk1A8 = 0.0f;
    }

    if (D_800BE9F0 == 0x32) {
        if (func_1509BE40(1, 0x4039, 6, 0x9000) != 0) {
            camera->unk84 |= 0x1000;
        } else {
            camera->unk84 &= ~0x1000;
        }
    }
}
