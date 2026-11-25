#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef s32 (*EmitterCallback)(s32, s16, f32, f32, f32, f32, f32, f32,
                                    s32, s32, f32, s32, f32, s32, u8);
extern EmitterCallback D_8008AA00[];

/*
 * Spawns `count` particles of `kind` evenly spaced on a horizontal circle of `radius`
 * around (x, y, z), starting at `startAngle` and stepping by `angleStep` (256 = full turn).
 * Stops early if the kind's callback returns 0.
 */
void func_1514C2F0(f32 x, f32 y, f32 z, f32 radius, u8 startAngle, s8 angleStep, s16 count,
                   u8 kind, s32 arg8, f32 arg9, s32 arg10, u8 arg11) {
    f32 dirX;
    f32 dirZ;
    f32 px;
    f32 py;
    f32 pz;
    EmitterCallback callback;
    u8 angle = startAngle;
    s16 i;

    for (i = 0; i < count; i++, angle += angleStep) {
        py = y;
        dirX = func_151423D8(angle - 0x40);
        dirZ = func_151423D8(angle);
        px = radius * dirX + x;
        pz = radius * dirZ + z;
        callback = D_8008AA00[kind];
        if (callback != NULL &&
            callback(1, i, px, py, pz, x, y, z, angle, startAngle, radius, arg8, arg9, arg10,
                     arg11) == 0) {
            return;
        }
    }
}

/*
 * Spawns particles of `kind` along the line from (x0, y0, z0) to (x1, y1, z1), `count`
 * points including both ends. Stops early if the kind's callback returns 0.
 */
void func_1514C470(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 count, u8 kind,
                   s32 arg8, f32 arg9, s32 arg10, u8 arg11) {
    f32 px;
    f32 py;
    f32 pz;
    vertex step;
    s16 i;
    f32 inverse;

    i = 0;
    if (count < 2.0f) {
        return;
    }

    // matching: the original tests the parity even though both branches are identical
    if ((s32)count & 1) {
        inverse = 1.0f / (count - 1.0f);
        step.x = (x1 - x0) * inverse;
        step.y = (y1 - y0) * inverse;
        step.z = (z1 - z0) * inverse;
    } else {
        inverse = 1.0f / (count - 1.0f);
        step.x = (x1 - x0) * inverse;
        step.y = (y1 - y0) * inverse;
        step.z = (z1 - z0) * inverse;
    }
    px = x0;
    py = y0;
    pz = z0;
    do {
        if (D_8008AA00[kind] != NULL &&
            D_8008AA00[kind](0, i, px, py, pz, x0, y0, z0, 0, 0, 0.0f, arg8, arg9, arg10,
                             arg11) == 0) {
            return;
        }
        count -= 1.0f;
        px += step.x;
        i++;
        py += step.y;
        pz += step.z;
    } while (count > 0.0f);
}

void func_1514C678(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16 arg5, s16 arg6, u8 arg7,
                   s32 arg8, f32 arg9, s32 argA, u8 argB) {
    EmitterCallback callback;
    f32 temp_arg1 = arg1;
    f32 temp_arg0 = arg0;
    f32 temp_arg2 = arg2;
    s16 range;
    s16 base;
    u8 angle;
    s16 i;
    f32 s;
    f32 c;
    f32 z;

    if (arg5 < arg4) {
        range = arg4 - arg5 + 1;
        base = arg5;
    } else {
        range = arg5 - arg4 + 1;
        base = arg4;
    }
    i = 0;
    if (arg6 > 0) {
        do {
            angle = (u32)func_150ADA20() % range + base;
            s = func_151423D8(angle - 0x40);
            c = func_151423D8(angle);
            z = arg3 * c + temp_arg2;
            callback = D_8008AA00[arg7];
            if (callback != 0) {
                if (callback(2, i, arg3 * s + temp_arg0, temp_arg1, z, temp_arg0, temp_arg1, temp_arg2, angle, arg4, arg3,
                                 arg8, arg9, argA, argB) == 0) {
                    return;
                }
            }
            i++;
        } while (i < arg6);
    }
}

void func_1514C858(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7,
                   u8 arg8, s32 arg9, f32 argA, s32 argB, u8 argC) {
    f32 s0;
    f32 c0;
    s16 range;
    s16 base;
    u8 angle;
    s16 i;
    f32 s;
    f32 c;
    f32 x;
    f32 y;
    f32 z;

    s0 = func_151423D8(arg4 - 0x40);
    c0 = func_151423D8(arg4);
    if (arg6 < arg5) {
        range = arg5 - arg6 + 1;
        base = arg6;
    } else {
        range = arg6 - arg5 + 1;
        base = arg5;
    }
    i = 0;
    if (arg7 > 0) {
        do {
            angle = (u32)func_150ADA20() % range + base;
            s = func_151423D8(angle - 0x40);
            c = func_151423D8(angle);
            x = arg3 * s0 * c + arg0;
            y = arg1 - arg3 * s;
            z = arg3 * c0 * c + arg2;
            if (D_8008AA00[arg8] != 0) {
                if (D_8008AA00[arg8](3, i, x, y, z, arg0, arg1, arg2,
                             angle, arg4, arg3, arg9, argA, argB, argC) == 0) {
                    return;
                }
            }
            i++;
        } while (i < arg7);
    }
}
