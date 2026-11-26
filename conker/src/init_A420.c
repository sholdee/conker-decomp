#include <ultra64.h>

#include "functions.h"
#include "variables.h"

s32 math_distance3D(s32, s32, s32);

/*
 * Pan and volume of a positional sound. (dx, dz) is the sound's offset from the listener, which sets the pan
 * together with the listener's angle; (distX, distY, distZ) is the offset whose length sets the volume, from
 * full at nearDistance down to silent at farDistance (bit 0x8000 of farDistance: horizontal distance only).
 * The pan is skipped when pan is NULL. Returns the distance.
 */
s32 func_1000A420(s32 dx, s32 dy, s32 dz, f32 angle, s32 distX, s32 distY, s32 distZ, s32 farDistance,
                  s32 nearDistance, s32 *pan, s32 *volume, s32 *distanceOut) {
    f32 ratio;
    s32 distance;
    s16 value;
    s32 rounded;
    s32 rearBit;
    s32 gain;

    rearBit = 0x80; // cleared when the sound is in the front quarter (|angle| < 32)
    if (farDistance & 0x8000) {
        farDistance &= 0x7FFF;
        distance = func_150AD960(distX, distZ, 0, 0);
    } else {
        distance = math_distance3D(distX, distY, distZ);
    }
    gain = 0x7FFF - ((nearDistance - distance) << 15) / (nearDistance - farDistance);
    if (gain > 400) {
        if (pan != NULL) {
            if (func_150AD960(dx, dz, 0, 0) > 30) {
                ratio = sqrtf(dx * dx + dz * dz);
                if (ratio > 0.01f) {
                    ratio = dx / ratio;
                }
                rounded = func_150487E0(ratio) * (128.0 / M_PI);
                value = rounded;
                if (dz > 0) {
                    if (value < 0) {
                        value = -128 - value;
                    } else {
                        value = 128 - value;
                    }
                }
                // add the listener's angle (degrees to 256ths of a turn) and wrap to a signed byte
                value += angle * (128.0f / 180.0f);
                value = (s8)value;
                if (value >= 96 || value < -96) {
                    value = 0;
                } else if (value >= 32) {
                    value = 95 - value;
                } else if (value < -32) {
                    value = -95 - value;
                } else {
                    rearBit = 0;
                    // matching: IDO emits sll for `value *= 2`; the original adds
                    value += value;
                }
                *pan = (value + 64) | rearBit;
            } else {
                *pan = 64;
            }
        }
        // the level is recomputed here, as in the original code
        if (0x7FFF - ((nearDistance - distance) << 15) / (nearDistance - farDistance) < 0) {
            // matching: `* 0` keeps (nearDistance - farDistance) live, so IDO gives it $t1 and distance $t2
            gain = (nearDistance - farDistance) * 0;
        }
        if (gain >= 0x8000) {
            gain = 0x7FFF;
        }
        *volume = gain;
    } else {
        *volume = 0;
    }
    if (distanceOut != NULL) {
        *distanceOut = distance;
    }
    return distance;
}


/*
 * func_1000A420 for a sound spread along a path: the sound is placed at the point of path that is nearest to
 * the listener (x, y, z), interpolated along the nearest segment. (panX, panY, panZ) is the position the pan
 * is measured from.
 */
s32 func_1000A750(s32 path, s32 panX, s32 panY, s32 panZ, f32 angle, s32 x, s32 y, s32 z, s32 farDistance,
                  s32 nearDistance, s32 *pan, s32 *volume, s32 *distanceOut) {
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 prevDist;
    s32 bestDist;
    s32 best;
    s32 i;
    s32 beforeBestDist;
    s32 afterBestDist;
    s32 count;
    s32 outside;
    PathPoint *point;
    s16 soundX;
    s16 soundY;
    s16 soundZ;
    Vec3f segment;
    Vec3f offset;
    f32 lengthSq;
    f32 dot;

    outside = FALSE;
    count = D_800D2108[path];
    best = -2;
    if (count == 0) {
        return 0;
    }
    bestDist = 0x7FFFFFFF;
    prevDist = bestDist;
    for (i = 0, point = D_800D2104[path]; i < count; i++, point++) {
        dx = x - point->x;
        dy = y - point->y;
        dz = z - point->z;
        dist = dx * dx + dy * dy + dz * dz;
        if (dist < bestDist) {
            best = i;
            beforeBestDist = prevDist;
            bestDist = dist;
        } else if (i - 1 == best) {
            afterBestDist = dist;
        }
        prevDist = dist;
    }
    soundX = D_800D2104[path][best].x;
    soundY = D_800D2104[path][best].y;
    soundZ = D_800D2104[path][best].z;
    if (count >= 2 && bestDist > 28000) {
        // use the segment from best toward its nearer neighbour
        if (best >= count - 1 || beforeBestDist < afterBestDist) {
            best--;
        }
        segment.x = D_800D2104[path][best + 1].x - D_800D2104[path][best].x;
        segment.y = D_800D2104[path][best + 1].y - D_800D2104[path][best].y;
        segment.z = D_800D2104[path][best + 1].z - D_800D2104[path][best].z;
        offset.x = x - D_800D2104[path][best].x;
        offset.y = y - D_800D2104[path][best].y;
        offset.z = z - D_800D2104[path][best].z;
        dot = func_150AD900(&segment.x, &offset.x);
        if (dot < 0.0f) {
            outside = TRUE;
        } else {
            lengthSq = func_150AD930(&segment.x);
            lengthSq = lengthSq * lengthSq;
            if (lengthSq < dot) {
                outside = TRUE;
            }
        }
        if (outside) {
            // the listener projects outside that segment: try the one on the other side
            func_150AD900(&segment.x, &offset.x);
            if (beforeBestDist < afterBestDist) {
                best++;
            } else {
                best--;
            }
            segment.x = D_800D2104[path][best + 1].x - D_800D2104[path][best].x;
            segment.y = D_800D2104[path][best + 1].y - D_800D2104[path][best].y;
            segment.z = D_800D2104[path][best + 1].z - D_800D2104[path][best].z;
            offset.x = x - D_800D2104[path][best].x;
            offset.y = y - D_800D2104[path][best].y;
            offset.z = z - D_800D2104[path][best].z;
            dot = func_150AD900(&segment.x, &offset.x);
            lengthSq = func_150AD930(&segment.x);
            lengthSq = lengthSq * lengthSq;
        }
        if (lengthSq != 0.0f) {
            if (dot < lengthSq) {
                func_15049148(&segment, dot / lengthSq, &segment);
            }
            soundX = D_800D2104[path][best].x + segment.x;
            soundY = D_800D2104[path][best].y + segment.y;
            soundZ = D_800D2104[path][best].z + segment.z;
        }
    }
    return func_1000A420(soundX - panX, soundY - panY, soundZ - panZ, angle, soundX - x, soundY - y, soundZ - z,
                         farDistance, nearDistance, pan, volume, distanceOut);
}
/*
 * The pan of func_1000A420 for a direction (dx, dz) on the floor plane, turned by angle (256ths of a turn).
 */
s32 func_1000B060(f32 dx, f32 dz, s32 angle) {
    s16 value;
    s16 rearBit;
    f32 ratio;
    s32 rounded;

    ratio = sqrtf(dx * dx + dz * dz);
    if (ratio > 0.01f) {
        ratio = dx / ratio;
    }
    rearBit = 0x80; // cleared when the direction is in the front quarter (|angle| < 32)
    rounded = func_150487E0(ratio) * (128.0 / M_PI);
    value = rounded;
    if (dz > 0.0f) {
        if (value < 0) {
            value = -128 - value;
        } else {
            value = 128 - value;
        }
    }
    // wrap to a signed byte
    value += angle;
    value = (s8)value;
    if (value >= 96 || value < -96) {
        value = 0;
    } else if (value >= 32) {
        value = 95 - value;
    } else if (value < -32) {
        value = -95 - value;
    } else {
        rearBit = 0;
        // matching: IDO emits sll for `value *= 2`; the original adds
        value += value;
    }
    return (value + 64) | rearBit;
}
