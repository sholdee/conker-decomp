#include <ultra64.h>
#include <math.h>
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"

/* Integer world position chosen by effect_pickGroundHitCentre as the centre of effect_scatterGroundHits's scatter. */
static Vec3s gGroundHitCentre;

/* Stores the point 500 units ahead of camera_global_Cameras (along its pitch 0x398 and yaw 0x3A0) in gGroundHitCentre. */
void effect_pickGroundHitCentre(void) {
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
    gGroundHitCentre.x = x;
    gGroundHitCentre.y = y;
    gGroundHitCentre.z = z;
}


/* Drops `count` random ground hits within 500 units of gGroundHitCentre and spawns each surface's effect. */
void effect_scatterGroundHits(s32 count) {
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
    centerX = gGroundHitCentre.x;
    centerZ = gGroundHitCentre.z;
    for (i = 0; i < count; i++) {
        y = 10000;
        x = math_randomU8() % 500 + centerX;
        z = math_randomU8() % 500 + centerZ;
        y = collision_findMeshFloorNear(x, y, z, &surface);
        if (noHit != y && surface != 0) {
            idx = (surface - (s32)gCurMapTris) / 12;
            if (idx >= 0 && idx < gCurMapTriCount) {
                material = collision_getSurfaceType(gMeshTriMaterials[idx]);
                if (material != 0) {
                    effect_spawnMaterialImpact(x, y, z, 0, material, (s16 **)surface, 0xFF);
                }
            }
        }
    }
}

/* Scatters one ground effect per 30 units of `amount` in front of camera_global_Cameras. */
void effect_scatterGroundHitsAhead(s32 amount) {
    effect_pickGroundHitCentre();
    effect_scatterGroundHits(amount / 30);
}
