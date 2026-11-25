#include <ultra64.h>

#include "functions.h"
#include "variables.h"

extern f32 D_8009DC80; /* 0.01745329238f -- degrees to radians, asm/data/242740.rodata.s */

void func_1508EF80(Vec3f *arg0, Vec3f *arg1, f32 arg2, Vec3f *arg3) {
    f32 dx;
    f32 dz;
    f32 ang;
    Vec3f out;

    dx = arg0->x - arg1->x;
    dz = arg0->z - arg1->z;
    ang = arg2 * D_8009DC80;
    out.x = ((cosf(ang) * dx) + (sinf(ang) * dz)) + arg1->x;
    out.z = ((-sinf(ang) * dx) + (cosf(ang) * dz)) + arg1->z;
    arg3->x = out.x;
    arg3->z = out.z;
}
