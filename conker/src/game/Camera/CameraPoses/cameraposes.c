#include <ultra64.h>
#include "structs.h"

/* This file's .bss, before the headers: after them, its order would change with the number of declarations in them. */
CamPose *gCameraPoses; // the level's camera pose table (gNumCameraPoses entries)
CamPose *gTargetCameraPose; // the pose game/Camera/CameraCommands/cameracommands is moving to

struct Entry151298C0 {
    s16 unk0;
    s16 unk2;
    f32 unk4;
    f32 unk8;
    u8 padC[0x18];
};
static struct Entry151298C0 D_800DC028[4];

#include <math.h>
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/math.h"

s16 gNumCameraPoses = 0; // number of entries in gCameraPoses

/* Resets this camera's D_800DC028 entry when gNumCameraPoses is set. */
void func_151298C0(cam, arg1)
    Camera *cam;
    s32 arg1;
{
    if (gNumCameraPoses != 0) {
        D_800DC028[cam->CNumber].unk2 = 0;
        D_800DC028[cam->CNumber].unk4 = -1.0f;
        D_800DC028[cam->CNumber].unk8 = 1.0f / 90.0f;
    }
}


/*
 * Eases the eye toward the point FTgetRadius short of target on the line from posePos
 * (snapping when Snap is set), then re-queries collision_findFloorInRange for it when needed.
 */
void camera_updatePoseEye(Camera *cam) {
    f32 x;
    Vec3f dir;
    f32 z;
    f32 y;
    f32 dist;
    f32 surface;
    f32 dx;
    f32 dy;
    f32 dz;

    if (gNumCameraPoses != 0 && cam->DoingAim == 0) {
        cam->unk27C.x = cam->posePos.x;
        cam->unk27C.y = cam->posePos.y;
        cam->unk27C.z = cam->posePos.z;
        math_vec3Diff(&cam->unk27C, &cam->target, &dir);
        math_vec3Unit(&dir, &dir);
        dx = cam->target.x - cam->unk27C.x;
        dy = cam->target.y - cam->unk27C.y;
        dz = cam->target.z - cam->unk27C.z;
        dist = sqrtf(dx * dx + dy * dy + dz * dz) - cam->FTgetRadius;
        if (dist < 0.0f) {
            dist = 0.0f;
        }
        math_vec3Scale(&dir, dist, &dir);
        x = cam->unk27C.x + dir.x;
        if (cam->cmd.unk8 & 0xF) {
            y = cam->unk27C.y;
        } else {
            y = cam->unk27C.y + dir.y * 1.2f;
        }
        z = cam->unk27C.z + dir.z;
        if (cam->Snap) {
            cam->eye.x = x;
            cam->prevEye.x = x;
            cam->CurrentVelX = 0; // matching: int 0, not 0.0f (a float zero gets promoted to a register)
        } else {
            math_springTo(&cam->eye.x, x, &cam->CurrentVelX, 1.0f, 3.0f, cam->SpringTime);
        }
        if (cam->Snap) {
            cam->eye.y = y;
            cam->prevEye.y = y;
            cam->CurrentVelY = 0;
        } else {
            math_springTo(&cam->eye.y, y, &cam->CurrentVelY, 1.0f, 2.0f, cam->SpringTime);
        }
        if (cam->Snap) {
            cam->eye.z = z;
            cam->prevEye.z = z;
            cam->CurrentVelZ = 0;
        } else {
            math_springTo(&cam->eye.z, z, &cam->CurrentVelZ, 1.0f, 3.0f, cam->SpringTime);
        }
        camera_blendYaw(cam);
        if (cam->Snap || (cam->AvailableModes & 0x400000) ||
            !collision_getTriHeightAt(cam->eye.x, cam->eye.z, cam->TrianglePtr, cam->floorTri.v, &cam->floorY)) {
            collision_findFloorInRange(NULL, NULL, &surface, &cam->waterY, &cam->Attributes, 0, cam->eye.x, cam->eye.y,
                          cam->eye.z, cam->eye.y, 0, 0, -10000.0f, cam->eye.y);
            if (cam->zoneWaterY != -10000.0f) {
                cam->waterY = cam->zoneWaterY;
            }
            camera_updateStatus(cam);
        }
        cam->accelATarget = 1.0f;
        cam->accelA = 1.0f;
        cam->targetSpeed = 1.0f;
        cam->speed = 1.0f;
        cam->accelBTarget = 2.0f;
        cam->accelB = 2.0f;
        cam->targetLimit = 2.0f;
        cam->limit = 2.0f;
    }
}
