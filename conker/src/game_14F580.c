#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15122980(Camera *arg0);

/**
 * Unless flag 4 is set, recomputes the heading (unk37C, degrees; unk39C in radians)
 * from the vector between unk2F8 and the position unk2BC, then calls func_15122980.
 */
void func_151220D0(Camera *arg0) {
    Vec3f vector;
    f32 heading;

    if (!(arg0->contPad->button & 4)) {
        func_15048F90((Vec3f *)&arg0->eye, (Vec3f *)&arg0->target, &vector);
        vector.x += arg0->prevTarget.x - arg0->target.x;
        vector.z += arg0->prevTarget.z - arg0->target.z;
        heading = func_15048FC8(&vector);
        arg0->yaw = heading;
        arg0->yawRad = heading * 0.01745329238f;
    }
    func_15122980(arg0);
}

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

    if (arg0->contPad->button & 4) {
        func_15122980(arg0);
        return;
    }
    arg0->unk5DC -= arg0->unk5DC * 0.07999999821f;
    arg0->orbitSpeed = 0.0f;
    func_15125330(arg0);
    target = (Vec3f *)arg0->unk614;
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
    heading += -90.0f + arg0->unk5DC;
    while (heading < 0.0f) {
        heading += 360.0f;
    }
    while (heading > 360.0f) {
        heading -= 360.0f;
    }
    if (arg0->unk698 == 0) {
        if (arg0->snap) {
            arg0->unk630 = 0.0f;
            arg0->yaw = heading;
        } else if (arg0->flags & 0x40000000) {
            func_15049688(&arg0->yaw, heading, &arg0->unk630, 3.0f, 6.0f, arg0->dt);
        } else if (arg0->flags & 0x4000000) {
            func_15049688(&arg0->yaw, heading, &arg0->unk630, 3.0f, 6.0f, arg0->dt);
        } else {
            func_15049688(&arg0->yaw, heading, &arg0->unk630, 1.5f, 2.0f, arg0->dt);
        }
        arg0->yawRad = arg0->yaw * 0.01745329238f;
    }
    func_1512A390(arg0);
    func_15123A54(arg0);
    func_1512E140(arg0);
    if (arg0->flags & 0x40000000) {
        func_15123A54(arg0);
        arg0->liftTarget = 30.0f;
        func_1512E140(arg0);
    }
}
