#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 D_80088A00 = 0; /* not referenced */
static s32 D_80088A04 = 0; /* 1 while the effect runs */

s32 D_800D9A00; /* not referenced */
static s32 D_800D9A04; /* centre angle of the bolt ends, in degrees */
static s32 D_800D9A08; /* angle spread of the bolt ends, in degrees */
static s32 D_800D9A0C; /* largest intensity */
static s32 D_800D9A10; /* phase timer */
static s32 D_800D9A14; /* intensity */

void func_150E5AE0(void) {
    s32 n;
    s32 i;
    s32 j;
    s32 a;
    s32 b;
    f32 h1;
    f32 h2;
    f32 ang;
    f32 x1;
    f32 z1;
    f32 x2;
    f32 ang2;
    f32 z2;

    if (D_80088A04 == 1) {
        D_800D9A10 += D_800BE9E4;
        if (D_800D9A10 >= 0x83) {
            D_800D9A10 = -(func_151EF610() % 30);
        }
        if (D_800D9A10 >= 0x1F) {
            if (D_800D9A14 < D_800D9A0C) {
                D_800D9A14 += D_800BE9E4 * 2;
            }
        } else if (D_800BE9E4 < D_800D9A14) {
            D_800D9A14 -= D_800BE9E4;
        } else {
            D_80088A04 = 0;
            D_800D9A14 = 0;
        }
        n = (D_800D9A14 + 10) / 10;
        j = D_800D9A14 + 1;
        effect_scatterGroundHitsAhead(D_800D9A14);
        for (i = 0; i < n; i++, j -= 10) {
            if ((j >= 10) || ((func_151EF610() % 11) - j == 0)) {
                a = ((func_151EF610() % D_800D9A08) * 2) - D_800D9A08 + D_800D9A04;
                b = ((func_151EF610() % D_800D9A08) * 2) - D_800D9A08 + D_800D9A04;
                h2 = func_151EF610() % 200;
                h1 = func_151EF610() % 200 + h2 - 100.0f;
                if (((b - 5.0f) < a) && (a < (b + 5.0f))) {
                    h1 += 150.0f;
                    h2 += 150.0f;
                }
                if (func_151EF610() % 2) {
                    a += 180.0f;
                } else {
                    b += 180.0f;
                }
                ang = a * 0.01745329238f;
                x1 = sinf(ang) * 3000.0f;
                z1 = cosf(ang) * 3000.0f;
                ang2 = b * 0.01745329238f;
                x2 = sinf(ang2) * 3000.0f;
                z2 = cosf(ang2) * 3000.0f;
                x1 += camera_global_Cameras->eye.x;
                h1 += camera_global_Cameras->eye.y;
                z1 += camera_global_Cameras->eye.z;
                x2 += camera_global_Cameras->eye.x;
                h2 += camera_global_Cameras->eye.y;
                z2 += camera_global_Cameras->eye.z;
                beam_spawnTracer(0, x1, h1, z1, x2, h2, z2,
                              160.0f, 10.0f, 4.0f, 360.0f, 0xF, 0x43, 0, 0, 0, 0, 0, 0, -1, 0, 0,
                              0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            }
        }
    }
}
