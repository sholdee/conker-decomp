#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_1000E40C(s32, s32);
s32 func_150AD960(s32, s32, s32, s32);
s32 func_150AD9A0(s32, s32, s32);

s32 func_1000A420(s32 arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 *arg9, s32 *argA, s32 *argB) {
    f32 ratio;
    s32 ret;
    s16 value;
    s32 rounded;
    s32 mode;
    s32 result;

    mode = 0x80;
    if (arg7 & 0x8000) {
        arg7 &= 0x7FFF;
        ret = func_150AD960(arg4, arg6, 0, 0);
    } else {
        ret = func_150AD9A0(arg4, arg5, arg6);
    }
    result = 0x7FFF - ((arg8 - ret) << 15) / (arg8 - arg7);
    if (result >= 401) {
        if (arg9 != NULL) {
            if (func_150AD960(arg0, arg2, 0, 0) >= 31) {
                ratio = sqrtf(arg0 * arg0 + arg2 * arg2);
                if (D_8002C200 < ratio) {
                    ratio = arg0 / ratio;
                }
                rounded = (s32)((f64)func_150487E0(ratio) * D_8002C208);
                value = rounded;
                if (arg2 > 0) {
                    if ((s16)rounded < 0) {
                        value = -0x80 - (s16)rounded;
                    } else {
                        value = 0x80 - (s16)rounded;
                    }
                }
                value += arg3 * D_8002C210;
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
                *arg9 = (value + 0x40) | mode;
            } else {
                *arg9 = 0x40;
            }
        }
        if (0x7FFF - ((arg8 - ret) << 15) / (arg8 - arg7) < 0) {
            result = (arg8 - arg7) * 0;
        }
        if (result >= 0x8000) {
            result = 0x7FFF;
        }
        *argA = result;
    } else {
        *argA = 0;
    }
    if (argB != NULL) {
        *argB = ret;
    }
    return ret;
}
// NON-MATCHING: mostly just regalloc
// s32 func_150AD960(s32, s32, s32, s32);
// s32 func_150AD9A0(s32, s32, s32);
// s32 func_1000A420(s32 arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 *arg9, s32 *argA, s32 *argB) {
//     f64 dbl;
//     s32 temp_v1;
//     f32 phi_f12;
//     s32 sp24;
//     s16 tmp0;
//     s8 temp_t5;
//     s32 ret;
//
//     sp24 = 128;
//     if ((arg7 & 0x8000) != 0) {
//         // if arg7 is negative, take abs
//         arg7 = arg7 & 0x7FFF;
//         ret = func_150AD960(arg4, arg6, 0, 0);
//     } else {
//         ret = func_150AD9A0(arg4, arg5, arg6);
//     }
//     temp_v1 = 0x7FFF - ((arg8 - ret) << 15) / (arg8 - arg7);
//     if (temp_v1 >= 401) {
//         if (arg9 != 0) {
//             if (func_150AD960(arg0, arg2, 0, 0) >= 31) {
//                 phi_f12 = sqrtf((f32) ((arg0 * arg0) + (arg2 * arg2)));
//                 if (D_8002C200 < phi_f12) { // rodata, 0.009999999776482582
//                     phi_f12 = arg0 / phi_f12;
//                 }
//                 dbl = (func_150487E0(phi_f12) * D_8002C208);
//                 tmp0 = dbl;
//                 if (arg2 > 0) {
//                     if (tmp0 < 0) {
//                         tmp0 = -128 - tmp0;
//                     } else {
//                         tmp0 = 128 - tmp0;
//                     }
//                 }
//                 temp_t5 = tmp0 + (arg3 * D_8002C210);
//                 if ((temp_t5 >= 96) || ((temp_t5 < (-96)))) {
//                     tmp0 = 0;
//                 } else if ( temp_t5 >= 32) {
//                     tmp0 = 95 - temp_t5;
//                 } else if (temp_t5 < -32) {
//                     tmp0 = -95 - temp_t5;
//                 } else {
//                     sp24 = 0;
//                     tmp0 = temp_t5 * 2;
//                 }
//                 *arg9 = (tmp0 + 64) | sp24;
//             } else {
//                 *arg9 = 64;
//             }
//         }
//         if ((0x7FFF - ((s32) ((arg8 - ret) << 15) / (arg8 - arg7))) < 0) {
//             temp_v1 = 0;
//         }
//         if (temp_v1 >= 0x8000) {
//             temp_v1 = 0x7FFF;
//         }
//         *argA = temp_v1;
//     } else {
//         *argA = 0;
//     }
//     if (argB != 0) {
//         *argB = ret;
//     }
//     return ret;
// }

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} PathPoint_A750;

s32 func_1000A420(s32, s32, s32, f32, s32, s32, s32, s32, s32, s32 *, s32 *, s32 *);

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
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

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
// NON-MATCHING: fair amount to fix up
// s32 func_1000B060(f32 arg0, f32 arg1, s32 arg2) {
//     s16 phi_a1;
//     f32 sp18;
//     s16 temp_t8;
//     f64 temp_f6;
//     s8 temp_t9;
//     s16 phi_v1_2;
//
//     sp18 = sqrtf((arg0 * arg0) + (arg1 * arg1));
//     if (D_8002C214 < sp18) {
//         sp18 = arg0 / sp18;
//     }
//     phi_a1 = 128;
//     temp_f6 = func_150487E0(sp18) * D_8002C218;
//     phi_v1_2 = temp_f6;
//     if (0.0f < arg1) {
//         temp_t8 = temp_f6;
//         if ((s32) temp_t8 < 0) {
//             phi_v1_2 = (s16) (-128 - temp_t8);
//         } else {
//             phi_v1_2 = (s16) (128 - temp_t8);
//         }
//     }
//     temp_t9 = phi_v1_2 + arg2;
//     if (( temp_t9 >= 96) || ( temp_t9 < -96)) {
//         phi_v1_2 = 0;
//         phi_a1 = 128;
//     } else if ((s32) temp_t9 >= 32) {
//         phi_v1_2 = (s16) (0x5F - temp_t9);
//         phi_a1 = 128;
//     } else if ((s32) temp_t9 < -32) {
//         phi_v1_2 = (s16) (-0x5F - temp_t9);
//         phi_a1 = 128;
//     } else {
//         phi_v1_2 = (s16) (temp_t9 * 2);
//         phi_a1 = 0;
//     }
//     return (phi_v1_2 + 64) | phi_a1;
// }
