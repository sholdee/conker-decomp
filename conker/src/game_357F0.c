#include <ultra64.h>
#include "functions.h"
#include "variables.h"

f32 sinf(f32);
f32 cosf(f32);

/* Uniform random value in [0, 1], from the CPU counter times the RNG byte. */
#define RAND_UNIT() ((f32)((osGetCount() * func_150ADA20()) & 0xFFFF) * (1.0f / 65535.0f))

#define DEG_TO_RAD ((f32)(M_PI / 180.0))

/*
 * Picks a random point inside `region`: writes its x/z position and the y range [*bottom, *top].
 *   shape 0: disc of radius radiusX, y from y to y + height
 *   shape 1: disc of radius radiusX, y within +-radiusX
 *   shape 2: box of half-extents radiusX x radiusZ rotated by yaw, y from y to y + height
 *   shape 3: the centre point, y within +-height
 */
void func_15008340(TriggerZone *region, f32 *x, f32 *z, f32 *top, f32 *bottom) {
    // matching: each case's `break` sits after its block (with -g3 its line steers the dispatch scheduling)
    switch (region->kind & 3) {
    default:
        *x = region->x;
        *z = region->z;
        *top = (f32)region->y + (f32)region->height;
        *bottom = (f32)region->y - (f32)region->height;
        break;
    case 2: {
        f32 angle;
        f32 s;
        f32 c;
        f32 dx;
        f32 dz;

        angle = region->yaw * DEG_TO_RAD;
        s = sinf(angle);
        c = cosf(angle);
        dx = RAND_UNIT() * (2.0f * region->radius) + -region->radius;
        dz = RAND_UNIT() * (2.0f * region->depth) + -region->depth;
        *x = region->x + (dx * c + dz * s);
        *z = region->z + (dz * c - dx * s);
        *top = (f32)region->y + (f32)region->height;
        *bottom = region->y;
    } break;
    case 0: {
        f32 angle;
        f32 s;
        f32 c;
        f32 dist;

        angle = RAND_UNIT() * 2.0f * (f32)M_PI;
        s = sinf(angle);
        c = cosf(angle);
        dist = RAND_UNIT() * region->radius;
        *x = region->x + dist * c;
        *z = region->z - dist * s;
        *top = (f32)region->y + (f32)region->height;
        *bottom = region->y;
    } break;
    case 1: {
        f32 angle;
        f32 s;
        f32 c;
        f32 dist;

        angle = RAND_UNIT() * 2.0f * (f32)M_PI;
        s = sinf(angle);
        c = cosf(angle);
        dist = RAND_UNIT() * region->radius;
        *x = region->x + dist * c;
        *z = region->z - dist * s;
        *top = (f32)region->y + (f32)region->radius;
        *bottom = (f32)region->y - (f32)region->radius;
    } break;
    }
}
