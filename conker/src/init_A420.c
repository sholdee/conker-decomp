#include <ultra64.h>

#include "functions.h"
#include "variables.h"

s32 func_150AD960(s32, s32, s32, s32);
s32 func_150AD9A0(s32, s32, s32);

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
    s32 level;

    rearBit = 0x80; // cleared when the sound is in the front quarter (|angle| < 32)
    if (farDistance & 0x8000) {
        farDistance &= 0x7FFF;
        distance = func_150AD960(distX, distZ, 0, 0);
    } else {
        distance = func_150AD9A0(distX, distY, distZ);
    }
    level = 0x7FFF - ((nearDistance - distance) << 15) / (nearDistance - farDistance);
    if (level > 400) {
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
            level = (nearDistance - farDistance) * 0;
        }
        if (level >= 0x8000) {
            level = 0x7FFF;
        }
        *volume = level;
    } else {
        *volume = 0;
    }
    if (distanceOut != NULL) {
        *distanceOut = distance;
    }
    return distance;
}

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} PathPoint_A750;

#define PTS (((PathPoint_A750 **)D_800D2104)[arg0])
#define DIST (dx * dx + dy * dy + dz * dz)

s32 func_1000A750(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 x, s32 y, s32 z, s32 arg8, s32 arg9,
                  s32 *argA, s32 *argB, s32 *argC) {
    s32 dx;
    s32 dy;
    s32 dz;
    s32 dist;
    s32 prev;
    s32 best;
    s32 bestIdx;
    s32 i;
    s32 prevBest;
    s32 nextBest;
    s32 count;
    s32 flag;
    PathPoint_A750 *pts;
    s16 resX;
    s16 resY;
    s16 resZ;
    struct17 seg;
    struct17 rel;
    f32 lenSq;
    f32 dot;

    flag = 0;
    count = D_800D2108[arg0];
    bestIdx = -2;
    if (count == 0) {
        return 0;
    }
    best = 0x7FFFFFFF;
    prev = best;
    pts = PTS;
    for (i = 0; i < count; i++) {
        dx = x - pts->x;
        dy = y - pts->y;
        dz = z - pts->z;
        dist = DIST;
        if (dist < best) {
            bestIdx = i;
            prevBest = prev;
            best = dist;
        } else if (i - 1 == bestIdx) {
            nextBest = dist;
        }
        prev = dist;
        pts++;
    }
    resX = PTS[bestIdx].x;
    resY = PTS[bestIdx].y;
    resZ = PTS[bestIdx].z;
    if (count >= 2 && best >= 0x6D61) {
        if (bestIdx >= count - 1 || prevBest < nextBest) {
            bestIdx--;
        }
        seg.unk0 = PTS[bestIdx + 1].x - PTS[bestIdx].x;
        seg.unk4 = PTS[bestIdx + 1].y - PTS[bestIdx].y;
        seg.unk8 = PTS[bestIdx + 1].z - PTS[bestIdx].z;
        rel.unk0 = x - PTS[bestIdx].x;
        rel.unk4 = y - PTS[bestIdx].y;
        rel.unk8 = z - PTS[bestIdx].z;
        dot = func_150AD900(&seg.unk0, &rel.unk0);
        if (dot < 0.0f) {
            flag = 1;
        } else {
            lenSq = func_150AD930(&seg.unk0);
            lenSq = lenSq * lenSq;
            if (lenSq < dot) {
                flag = 1;
            }
        }
        if (flag) {
            func_150AD900(&seg.unk0, &rel.unk0);
            if (prevBest < nextBest) {
                bestIdx++;
            } else {
                bestIdx--;
            }
            seg.unk0 = PTS[bestIdx + 1].x - PTS[bestIdx].x;
            seg.unk4 = PTS[bestIdx + 1].y - PTS[bestIdx].y;
            seg.unk8 = PTS[bestIdx + 1].z - PTS[bestIdx].z;
            rel.unk0 = x - PTS[bestIdx].x;
            rel.unk4 = y - PTS[bestIdx].y;
            rel.unk8 = z - PTS[bestIdx].z;
            dot = func_150AD900(&seg.unk0, &rel.unk0);
            lenSq = func_150AD930(&seg.unk0);
            lenSq = lenSq * lenSq;
        }
        if (lenSq != 0.0f) {
            if (dot < lenSq) {
                func_15049148(&seg, dot / lenSq, &seg);
            }
            resX = PTS[bestIdx].x + seg.unk0;
            resY = PTS[bestIdx].y + seg.unk4;
            resZ = PTS[bestIdx].z + seg.unk8;
        }
    }
    return func_1000A420(resX - arg1, resY - arg2, resZ - arg3, arg4, resX - x, resY - y, resZ - z, arg8, arg9, argA,
                         argB, argC);
}
s32 func_1000B060(f32 x, f32 y, s32 offset) {
    s16 value;
    s16 mode;
    f32 ratio;
    s32 rounded;

    ratio = sqrtf(x * x + y * y);
    if (D_8002C214 < ratio) {
        ratio = x / ratio;
    }
    mode = 0x80;
    rounded = (s32)((f64)func_150487E0(ratio) * D_8002C218);
    value = rounded;
    if (y > 0.0f) {
        if ((s16)rounded < 0) {
            value = -0x80 - (s16)rounded;
        } else {
            value = 0x80 - (s16)rounded;
        }
    }
    value += offset;
    value = (s8)value;
    if (value >= 0x60 || value < -0x60) {
        value = 0;
    } else if (value >= 0x20) {
        value = 0x5F - value;
    } else if (value < -0x20) {
        value = -0x5F - value;
    } else {
        mode = 0;
        value += value;
    }
    return (value + 0x40) | mode;
}
