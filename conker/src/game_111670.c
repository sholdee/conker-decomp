#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Integer world position chosen by func_150E41C0 as the centre of func_150E42F8's scatter. */
static Vec3s D_800D99F0;

/* Stores the point 500 units ahead of camera_global_Cameras (along its pitch 0x398 and yaw 0x3A0) in D_800D99F0. */
void func_150E41C0(void) {
    s32 x;
    s32 y;
    s32 z;
    f32 angle;
    f32 dy;
    f32 dz;
    f32 s;
    f32 c;
    f32 dx;

    x = camera_global_Cameras->eye.x;
    y = camera_global_Cameras->eye.y;
    z = camera_global_Cameras->eye.z;
    angle = camera_global_Cameras->CameraPitchRad;
    s = sinf(angle);
    c = cosf(angle);
    dx = 0.0f;
    dy = 500.0f * s;
    dz = -500.0f * c;
    angle = camera_global_Cameras->TrueCameraAngleRad;
    s = sinf(angle);
    c = cosf(angle);
    x += dx * c + dz * s;
    z += dz * c - dx * s;
    y += dy;
    D_800D99F0.x = x;
    D_800D99F0.y = y;
    D_800D99F0.z = z;
}


/* Drops `count` random ground hits within 500 units of D_800D99F0 and spawns each surface's effect. */
void func_150E42F8(s32 count) {
    s32 x;
    s32 y;
    s32 z;
    s32 centerX;
    s32 material;
    s32 centerZ;
    s32 idx;
    s32 i;
    f32 noHit;
    s32 surface;

    surface = 0;
    noHit = -10000.0f;
    centerX = D_800D99F0.x;
    centerZ = D_800D99F0.z;
    for (i = 0; i < count; i++) {
        y = 10000;
        x = func_150ADA20_u8() % 500 + centerX;
        z = func_150ADA20_u8() % 500 + centerZ;
        y = func_1510F8D8(x, y, z, &surface);
        if (noHit != y && surface != 0) {
            idx = (surface - (s32)D_800DBE3C) / 12;
            if (idx >= 0 && idx < D_800DBE4C) {
                material = func_1510F8CC(D_800DBE5C[idx]);
                if (material != 0) {
                    func_150E4550(x, y, z, 0, material, (s16 **)surface, 0xFF);
                }
            }
        }
    }
}

/* Scatters one ground effect per 30 units of `amount` in front of camera_global_Cameras. */
void func_150E4514(s32 amount) {
    func_150E41C0();
    func_150E42F8(amount / 30);
}
