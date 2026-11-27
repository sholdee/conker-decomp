#include <ultra64.h>

#include "conker/math.h"

// acos(x) for x in steps of 512 from 0 to 30720, steps of 32 from 30720 to 32736, and steps
// of 8 from 32736 to 32760.
static u16 gAcosTableCoarse[61] = {
    32768, 32442, 32116, 31789,
    31463, 31136,  30809, 30481,
    30153, 29824,  29495, 29164,
    28833, 28501,  28167, 27832,
    27496, 27159,  26820, 26480,
    26138, 25793,  25447, 25099,
    24749, 24396,  24041, 23683,
    23322, 22958,  22590, 22219,
    21845, 21467,  21084, 20697,
    20305, 19909,  19506, 19098,
    18684, 18263,  17835, 17399,
    16954, 16501,  16037, 15563,
    15077, 14577,  14063, 13532,
    12983, 12413,  11819, 11197,
    10542,  9848,   9105,  8300,
    7414 };

static u16 gAcosTableMedium[64] = {
    7414, 7355, 7296, 7237,
    7176, 7116, 7055, 6993,
    6931, 6868, 6805, 6741,
    6676, 6611, 6546, 6479,
    6412, 6345, 6276, 6207,
    6137, 6067, 5995, 5923,
    5850, 5776, 5701, 5625,
    5548, 5470, 5391, 5310,
    5229, 5146, 5062, 4976,
    4889, 4801, 4711, 4619,
    4525, 4430, 4332, 4232,
    4130, 4025, 3917, 3806,
    3692, 3575, 3453, 3328,
    3197, 3060, 2918, 2768,
    2609, 2441, 2259, 2062,
    1844, 1597, 1304, 922 };

static u16 gAcosTableFine[4] = { 922, 798, 652, 461 };

/*
 * Fixed-point arc cosine / arc sine. Inputs are s16 with 0x7FFF ~ 1.0; angles use 0x10000 = 2*pi,
 * so acos(0) = 0x8000. The float wrappers are math_acosLookup / math_asinLookup in game_75810.c.
 */

/**
 * Returns acos(x) for 0 <= x <= 0x7FFF by linear interpolation in three tables of increasing
 * resolution (acos gets steep near 1.0): step 512 below 30720, step 32 below 32736, step 8 above.
 */
s32 math_acosFixedAbs(s32 x) {
    s32 index;
    s32 lo;
    s32 hi;
    u16 *table;
    s32 shift;
    s32 fracMask;

    if (x >= 32736) {
        fracMask = 7;
        shift = 3;
        table = gAcosTableFine;
        x -= 32736;
    } else if (x >= 30720) {
        fracMask = 31;
        shift = 5;
        table = gAcosTableMedium;
        x -= 30720;
    } else {
        fracMask = 511;
        shift = 9;
        table = gAcosTableCoarse;
    }

    index = x >> shift;
    lo = table[index];
    hi = table[index + 1];

    return lo - (((lo - hi) * (x & fracMask)) >> shift);
}

/** Returns acos(x) as a u16 angle (0x10000 = 2*pi), using acos(-x) = pi - acos(x). */
u16 math_acosFixed(s16 x) {
    s32 angle;

    if (x >= 0) {
        angle = x;
    } else {
        angle = -x;
    }

    angle = math_acosFixedAbs(angle);
    if (x < 0) {
        angle = 0xFFFF - angle;
    }
    return angle;
}

/** Returns asin(x) as an s16 angle (0x8000 = pi/2), using asin(x) = pi/2 - acos(x). */
s16 math_asinFixed(s16 x) {
    s32 angle;

    if (x >= 0) {
        angle = x;
    } else {
        angle = -x;
    }

    angle = math_acosFixedAbs(angle);
    if (x >= 0) {
        angle = 0x7FFF - angle;
    } else {
        angle = angle - 0x8000;
    }
    return angle;
}
