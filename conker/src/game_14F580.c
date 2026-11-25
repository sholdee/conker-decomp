#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A3470;
extern f32 D_800A3474;
extern f32 D_800A3478;
extern f32 D_800A347C;
void func_15122980(struct108 *arg0);

/**
 * Unless flag 4 is set, recomputes the heading (unk37C, degrees; unk39C in radians)
 * from the vector between unk2F8 and the position unk2BC, then calls func_15122980.
 */
void func_151220D0(struct108 *arg0) {
    struct17 vector;
    f32 heading;

    if (!(*arg0->unk36C & 4)) {
        func_15048F90((struct17 *)&arg0->unk2F8, (struct17 *)&arg0->unk2BC, &vector);
        vector.unk0 += *(f32 *)&arg0->unk2C8 - arg0->unk2BC;
        vector.unk8 += *(f32 *)&arg0->unk2D0 - arg0->unk2C4;
        heading = func_15048FC8(&vector);
        arg0->unk37C = heading;
        arg0->unk39C = heading * 0.01745329238f;
    }
    func_15122980(arg0);
}

/**
 * Turns the heading (unk37C) towards the point at 0x614, offset by the angle at 0x5DC,
 * then runs the follow-up updates.
 */
void func_15122170(struct108 *arg0) {
    f32 deltaZ;
    f32 deltaX;
    f32 distance;
    f32 heading;

    if (*arg0->unk36C & 4) {
        func_15122980(arg0);
        return;
    }
    *(f32 *)&arg0->pad5DC[0] -= *(f32 *)&arg0->pad5DC[0] * D_800A3470;
    *(f32 *)&arg0->pad6B4[0x10] = 0.0f;
    func_15125330(arg0);
    if (*(struct17 **)&arg0->pad600[0x14] == NULL) {
        func_15122980(arg0);
        return;
    }
    deltaX = (*(struct17 **)&arg0->pad600[0x14])->unk0 - arg0->unk2BC;
    deltaZ = (*(struct17 **)&arg0->pad600[0x14])->unk8 - arg0->unk2C4;
    distance = sqrtf(deltaX * deltaX + deltaZ * deltaZ);
    if (distance == 0.0f) {
        func_15122980(arg0);
        return;
    }
    heading = func_15048C30(-deltaX / distance);
    if (deltaZ > 0.0f) {
        heading = 270.0f - heading * D_800A3474;
    } else {
        heading = heading * D_800A3478 + 90.0f;
    }
    heading += -90.0f + *(f32 *)&arg0->pad5DC[0];
    while (heading < 0.0f) {
        heading += 360.0f;
    }
    while (heading > 360.0f) {
        heading -= 360.0f;
    }
    if (arg0->unk698 == 0) {
        if (arg0->unk23C) {
            *(f32 *)&arg0->pad61C[0x14] = 0.0f;
            arg0->unk37C = heading;
        } else if (arg0->unk84 & 0x40000000) {
            func_15049688(&arg0->unk37C, heading, (f32 *)&arg0->pad61C[0x14], 3.0f, 6.0f, arg0->unk7B4);
        } else if (arg0->unk84 & 0x4000000) {
            func_15049688(&arg0->unk37C, heading, (f32 *)&arg0->pad61C[0x14], 3.0f, 6.0f, arg0->unk7B4);
        } else {
            func_15049688(&arg0->unk37C, heading, (f32 *)&arg0->pad61C[0x14], 1.5f, 2.0f, arg0->unk7B4);
        }
        arg0->unk39C = arg0->unk37C * D_800A347C;
    }
    func_1512A390(arg0);
    func_15123A54(arg0);
    func_1512E140(arg0);
    if (arg0->unk84 & 0x40000000) {
        func_15123A54(arg0);
        arg0->unk190 = 30.0f;
        func_1512E140(arg0);
    }
}
