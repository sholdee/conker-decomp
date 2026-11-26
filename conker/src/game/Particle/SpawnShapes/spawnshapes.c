#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Per-kind particle spawn callback, indexed by `kind` in D_8008AA00. `shape` is 0 line,
 * 1 even circle, 2 random circle, 3 random vertical arc; returning 0 stops the emitter.
 */
typedef s32 (*EmitterCallback)(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX,
                               f32 originY, f32 originZ, s32 angle, s32 baseAngle, f32 radius,
                               s32 arg11, f32 arg12, void *data, u8 arg14);
extern EmitterCallback D_8008AA00[];

/*
 * Spawns `count` particles of `kind` evenly spaced on a horizontal circle of `radius`
 * around (x, y, z), starting at `startAngle` and stepping by `angleStep` (256 = full turn).
 * Stops early if the kind's callback returns 0.
 */
void particle_emitOnCircle(f32 x, f32 y, f32 z, f32 radius, u8 startAngle, s8 angleStep, s16 count,
                   u8 kind, s32 arg8, f32 arg9, void *data, u8 arg11) {
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
        dirX = math_cosBinAngleMasked(angle - 0x40);
        dirZ = math_cosBinAngleMasked(angle);
        px = radius * dirX + x;
        pz = radius * dirZ + z;
        callback = D_8008AA00[kind];
        if (callback != NULL &&
            callback(1, i, px, py, pz, x, y, z, angle, startAngle, radius, arg8, arg9, data,
                     arg11) == 0) {
            return;
        }
    }
}

/*
 * Spawns particles of `kind` along the line from (x0, y0, z0) to (x1, y1, z1), `count`
 * points including both ends. Stops early if the kind's callback returns 0.
 */
void particle_emitOnLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 count, u8 kind,
                   s32 arg8, f32 arg9, void *data, u8 arg11) {
    f32 px;
    f32 py;
    f32 pz;
    Vec3f step;
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
            D_8008AA00[kind](0, i, px, py, pz, x0, y0, z0, 0, 0, 0.0f, arg8, arg9, data,
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

/*
 * Spawns `count` particles of `kind` on a horizontal circle of `radius` around (x, y, z),
 * each at a random angle between `angleA` and `angleB` inclusive.
 * Stops early if the kind's callback returns 0.
 */
void particle_emitOnRandomCircle(f32 x, f32 y, f32 z, f32 radius, s16 angleA, s16 angleB, s16 count, u8 kind,
                   s32 arg8, f32 arg9, void *data, u8 arg11) {
    f32 dirX;
    f32 dirZ;
    f32 px;
    f32 py;
    f32 pz;
    s16 range;
    s16 low;
    EmitterCallback callback;
    u8 angle;
    s16 i;

    if (angleB < angleA) {
        range = angleA - angleB + 1;
        low = angleB;
    } else {
        range = angleB - angleA + 1;
        low = angleA;
    }
    for (i = 0; i < count; i++) {
        py = y;
        angle = math_random() % range + low;
        dirX = math_cosBinAngleMasked(angle - 0x40);
        dirZ = math_cosBinAngleMasked(angle);
        px = radius * dirX + x;
        pz = radius * dirZ + z;
        callback = D_8008AA00[kind];
        if (callback != NULL &&
            callback(2, i, px, py, pz, x, y, z, angle, angleA, radius, arg8, arg9, data,
                     arg11) == 0) {
            return;
        }
    }
}

/*
 * Spawns `count` particles of `kind` on a vertical arc of `radius` around (x, y, z) in the
 * plane facing `yaw`, each at a random elevation between `angleA` and `angleB` inclusive.
 * Stops early if the kind's callback returns 0.
 */
void particle_emitOnRandomArc(f32 x, f32 y, f32 z, f32 radius, s16 yaw, s16 angleA, s16 angleB, s16 count,
                   u8 kind, s32 arg9, f32 arg10, void *data, u8 arg12) {
    f32 yawX;
    f32 yawZ;
    f32 elevY;
    f32 elevH;
    f32 px;
    f32 py;
    f32 pz;
    s16 range;
    s16 low;
    u8 angle;
    s16 i;

    yawX = math_cosBinAngleMasked(yaw - 0x40);
    yawZ = math_cosBinAngleMasked(yaw);
    if (angleB < angleA) {
        range = angleA - angleB + 1;
        low = angleB;
    } else {
        range = angleB - angleA + 1;
        low = angleA;
    }
    for (i = 0; i < count; i++) {
        angle = math_random() % range + low;
        elevY = math_cosBinAngleMasked(angle - 0x40);
        elevH = math_cosBinAngleMasked(angle);
        px = radius * yawX * elevH + x;
        py = y - radius * elevY;
        pz = radius * yawZ * elevH + z;
        if (D_8008AA00[kind] != NULL &&
            D_8008AA00[kind](3, i, px, py, pz, x, y, z, angle, yaw, radius, arg9, arg10, data,
                             arg12) == 0) {
            return;
        }
    }
}
