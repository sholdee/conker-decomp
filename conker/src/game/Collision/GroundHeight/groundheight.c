#include <ultra64.h>
#include "conker/collision.h"


void func_151045E0(void *obj, s32 arg1, f32 arg2) {
}

s32 func_151045F4(s32 particle, s32 prevPos) {
    return 0x1;
}

void func_15104608(f32 x, f32 y, f32 z, s32 owner) {
}

s32 func_15104620(s32 particle, s32 view) {
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
    count = collision_gatherGroundTris(x, z);
    if (count == 0) {
        return 0;
    }
    n = 0;
    for (i = 0; i < count; i++) {
        t = gGroundCandidates[i].tri;
        for (j = 0; j < 3; j++) {
            v = (s16 *)(gGroundCandidates[i].vtxBase + t[j]);
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
        b = gGroundCandidates[i].height * 0.00390625f - y;
        if (b < 0.0f) b = -(gGroundCandidates[i].height * 0.00390625f - y);
        if (b < bestd || best == -1) {
            bestd = b;
            best = i;
        }
    }
    *out = gGroundCandidates[best].height * 0.00390625f;
    return 1;
}
