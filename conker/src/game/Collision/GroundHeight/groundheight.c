#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_151045E0(void *arg0, s32 arg1, f32 arg2) {
}

s32 func_151045F4(s32 arg0, s32 arg1) {
    return 0x1;
}

void func_15104608(f32 x, f32 y, f32 z, s32 arg3) {
}

s32 func_15104620(s32 arg0, s32 arg1) {
    return 0x1;
}

#define ABS(x) (((x) < 0.0f) ? -(x) : (x))

s32 collision_getFloorNearY(f32 x, f32 z, f32 y, f32 *out) {
    s32 count;
    s32 i;
    s32 j;
    s32 n;
    s32 k;
    s32 list[21];
    f32 vx[3];
    f32 vz[3];
    f32 bestd;
    f32 a;
    f32 b;
    f32 c;
    f32 s;
    s16 *v;
    s32 *t;
    s32 best;

    collision_selectMap(0);
    count = func_150A3A70(x, z);
    if (count == 0) {
        return 0;
    }
    n = 0;
    for (i = 0; i < count; i++) {
        t = D_800D3300[i].tri;
        for (j = 0; j < 3; j++) {
            v = (s16 *)(D_800D3300[i].vtxBase + t[j]);
            vx[j] = v[0];
            vz[j] = v[2];
        }
        b = vz[1] - vz[0];
        a = vx[0] - vx[1];
        c = -(b * vx[0] + vz[0] * a);
        s = b * vx[2] + vz[2] * a + c;
        if (0.0f < s) {
            list[n++] = i;
        }
    }
    best = -1;
    if (n == 0) {
        return 0;
    }
    bestd = 10000.0f;
    for (k = 0; k < n; k++) {
        i = list[k];
        b = D_800D3300[i].height * 0.00390625f - y;
        if (b < 0.0f) b = -(D_800D3300[i].height * 0.00390625f - y);
        if (b < bestd || best == -1) {
            bestd = b;
            best = i;
        }
    }
    *out = D_800D3300[best].height * 0.00390625f;
    return 1;
}
