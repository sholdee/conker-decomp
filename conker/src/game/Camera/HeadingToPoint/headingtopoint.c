#include <ultra64.h>
#include <math.h>
#include "conker/camera.h"
#include "conker/math.h"

/**
 * Turns the heading (unk37C) towards the point at 0x614, offset by the angle at 0x5DC,
 * then runs the follow-up updates.
 */
void camera_turnHeadingToPoint(Camera *camera) {
    f32 deltaZ;
    f32 deltaX;
    f32 distance;
    f32 heading;
    Vec3f *target;

    if (camera->PadPtr->button & 4) {
        camera_followYaw(camera);
        return;
    }
    camera->PanButtonAddOn -= camera->PanButtonAddOn * 0.07999999821f;
    camera->orbitSpeed = 0.0f;
    camera_updateYaw(camera);
    target = camera->PivotPtr;
    if (target == NULL) {
        camera_followYaw(camera);
        return;
    }
    deltaX = target->x - camera->target.x;
    deltaZ = target->z - camera->target.z;
    distance = sqrtf(deltaX * deltaX + deltaZ * deltaZ);
    if (distance == 0.0f) {
        camera_followYaw(camera);
        return;
    }
    heading = asinf(-deltaX / distance);
    if (deltaZ > 0.0f) {
        heading = 270.0f - heading * 57.29577637f;
    } else {
        heading = heading * 57.29577637f + 90.0f;
    }
    heading += -90.0f + camera->PanButtonAddOn;
    while (heading < 0.0f) {
        heading += 360.0f;
    }
    while (heading > 360.0f) {
        heading -= 360.0f;
    }
    if (camera->unk698 == 0) {
        if (camera->Snap) {
            camera->AutoRotateAngleVel = 0.0f;
            camera->CameraAngle = heading;
        } else if (camera->AvailableModes & 0x40000000) {
            math_springAngleTo(&camera->CameraAngle, heading, &camera->AutoRotateAngleVel, 3.0f, 6.0f, camera->SpringTime);
        } else if (camera->AvailableModes & 0x4000000) {
            math_springAngleTo(&camera->CameraAngle, heading, &camera->AutoRotateAngleVel, 3.0f, 6.0f, camera->SpringTime);
        } else {
            math_springAngleTo(&camera->CameraAngle, heading, &camera->AutoRotateAngleVel, 1.5f, 2.0f, camera->SpringTime);
        }
        camera->CameraAngleRad = camera->CameraAngle * 0.01745329238f;
    }
    camera_updateFloorHeights(camera);
    camera_updateFollow(camera);
    camera_easeHeight(camera);
    if (camera->AvailableModes & 0x40000000) {
        camera_updateFollow(camera);
        camera->YOffsetTget = 30.0f;
        camera_easeHeight(camera);
    }
}
