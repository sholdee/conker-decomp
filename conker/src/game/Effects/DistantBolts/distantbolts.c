#include <ultra64.h>
#include <math.h>
#include "conker/beam.h"
#include "conker/camera.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/math.h"

s32 D_80088A00 = 0; /* not referenced */
static s32 gDistantTracersActive = 0; /* 1 while the effect runs */

s32 D_800D9A00; /* not referenced */
static s32 gDistantTracerCentreAngle; /* centre angle of the bolt ends, in degrees */
static s32 gDistantTracerAngleSpread; /* angle spread of the bolt ends, in degrees */
static s32 gDistantTracerMaxIntensity; /* largest intensity */
static s32 gDistantTracerPhaseTimer; /* phase timer */
static s32 gDistantTracerIntensity; /* intensity */

void effect_updateDistantTracers(void) {
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

    if (gDistantTracersActive == 1) {
        gDistantTracerPhaseTimer += gFrameTicks;
        if (gDistantTracerPhaseTimer >= 0x83) {
            gDistantTracerPhaseTimer = -(math_rand() % 30);
        }
        if (gDistantTracerPhaseTimer >= 0x1F) {
            if (gDistantTracerIntensity < gDistantTracerMaxIntensity) {
                gDistantTracerIntensity += gFrameTicks * 2;
            }
        } else if (gFrameTicks < gDistantTracerIntensity) {
            gDistantTracerIntensity -= gFrameTicks;
        } else {
            gDistantTracersActive = 0;
            gDistantTracerIntensity = 0;
        }
        n = (gDistantTracerIntensity + 10) / 10;
        j = gDistantTracerIntensity + 1;
        effect_scatterGroundHitsAhead(gDistantTracerIntensity);
        for (i = 0; i < n; i++, j -= 10) {
            if ((j >= 10) || ((math_rand() % 11) - j == 0)) {
                a = ((math_rand() % gDistantTracerAngleSpread) * 2) - gDistantTracerAngleSpread + gDistantTracerCentreAngle;
                b = ((math_rand() % gDistantTracerAngleSpread) * 2) - gDistantTracerAngleSpread + gDistantTracerCentreAngle;
                h2 = math_rand() % 200;
                h1 = math_rand() % 200 + h2 - 100.0f;
                if (((b - 5.0f) < a) && (a < (b + 5.0f))) {
                    h1 += 150.0f;
                    h2 += 150.0f;
                }
                if (math_rand() % 2) {
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
