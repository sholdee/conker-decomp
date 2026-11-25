#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/**
 * Turns the heading (unk37C) towards the point at 0x614, offset by the angle at 0x5DC,
 * then runs the follow-up updates.
 */
void func_15122170(Camera *arg0) {
    f32 deltaZ;
    f32 deltaX;
    f32 distance;
    f32 heading;
    Vec3f *target;

    if (arg0->PadPtr->button & 4) {
        func_15122980(arg0);
        return;
    }
    arg0->PanButtonAddOn -= arg0->PanButtonAddOn * 0.07999999821f;
    arg0->orbitSpeed = 0.0f;
    func_15125330(arg0);
    target = arg0->PivotPtr;
    if (target == NULL) {
        func_15122980(arg0);
        return;
    }
    deltaX = target->x - arg0->target.x;
    deltaZ = target->z - arg0->target.z;
    distance = sqrtf(deltaX * deltaX + deltaZ * deltaZ);
    if (distance == 0.0f) {
        func_15122980(arg0);
        return;
    }
    heading = func_15048C30(-deltaX / distance);
    if (deltaZ > 0.0f) {
        heading = 270.0f - heading * 57.29577637f;
    } else {
        heading = heading * 57.29577637f + 90.0f;
    }
    heading += -90.0f + arg0->PanButtonAddOn;
    while (heading < 0.0f) {
        heading += 360.0f;
    }
    while (heading > 360.0f) {
        heading -= 360.0f;
    }
    if (arg0->unk698 == 0) {
        if (arg0->Snap) {
            arg0->AutoRotateAngleVel = 0.0f;
            arg0->CameraAngle = heading;
        } else if (arg0->AvailableModes & 0x40000000) {
            func_15049688(&arg0->CameraAngle, heading, &arg0->AutoRotateAngleVel, 3.0f, 6.0f, arg0->SpringTime);
        } else if (arg0->AvailableModes & 0x4000000) {
            func_15049688(&arg0->CameraAngle, heading, &arg0->AutoRotateAngleVel, 3.0f, 6.0f, arg0->SpringTime);
        } else {
            func_15049688(&arg0->CameraAngle, heading, &arg0->AutoRotateAngleVel, 1.5f, 2.0f, arg0->SpringTime);
        }
        arg0->CameraAngleRad = arg0->CameraAngle * 0.01745329238f;
    }
    func_1512A390(arg0);
    func_15123A54(arg0);
    func_1512E140(arg0);
    if (arg0->AvailableModes & 0x40000000) {
        func_15123A54(arg0);
        arg0->YOffsetTget = 30.0f;
        func_1512E140(arg0);
    }
}
