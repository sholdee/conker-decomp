#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Uniform random value in [0, 1], from the CPU counter times the RNG byte. */
#define RAND_UNIT() (((osGetCount() * math_random()) & 0xFFFF) * (1.0f / 65535.0f))

#define DEG_TO_RAD ((f32)(M_PI / 180.0))

/*
 * Picks a random point inside `region`: writes its x/z position and the y range [*bottom, *top].
 *   shape 0: disc of radius radiusX, y from y to y + height
 *   shape 1: disc of radius radiusX, y within +-radiusX
 *   shape 2: box of half-extents radiusX x radiusZ rotated by yaw, y from y to y + height
 *   shape 3: the centre point, y within +-height
 */
void zone_getRandomPoint(TriggerZone *region, f32 *x, f32 *z, f32 *top, f32 *bottom) {
    // matching: each case's `break` sits after its block (with -g3 its line steers the dispatch scheduling)
    switch (ZONE_AREA_TYPE(region)) {
    default:
        *x = region->x;
        *z = region->z;
        *top = (f32)region->y + region->ySize;
        *bottom = (f32)region->y - region->ySize;
        break;
    case ZONE_AREA_BOX: {
        f32 angle;
        f32 s;
        f32 c;
        f32 dx;
        f32 dz;

        angle = region->yaw * DEG_TO_RAD;
        s = sinf(angle);
        c = cosf(angle);
        dx = RAND_UNIT() * (2.0f * region->xSize) + -region->xSize;
        dz = RAND_UNIT() * (2.0f * region->zSize) + -region->zSize;
        *x = region->x + (dx * c + dz * s);
        *z = region->z + (dz * c - dx * s);
        *top = (f32)region->y + region->ySize;
        *bottom = region->y;
    } break;
    case ZONE_AREA_CYLINDER: {
        f32 angle;
        f32 s;
        f32 c;
        f32 dist;

        angle = RAND_UNIT() * 2.0f * (f32)M_PI;
        s = sinf(angle);
        c = cosf(angle);
        dist = RAND_UNIT() * region->xSize;
        *x = region->x + dist * c;
        *z = region->z - dist * s;
        *top = (f32)region->y + region->ySize;
        *bottom = region->y;
    } break;
    case ZONE_AREA_SPHERE: {
        f32 angle;
        f32 s;
        f32 c;
        f32 dist;

        angle = RAND_UNIT() * 2.0f * (f32)M_PI;
        s = sinf(angle);
        c = cosf(angle);
        dist = RAND_UNIT() * region->xSize;
        *x = region->x + dist * c;
        *z = region->z - dist * s;
        *top = (f32)region->y + region->xSize;
        *bottom = (f32)region->y - region->xSize;
    } break;
    }
}
