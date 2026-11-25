#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Particle spawn region; the low two bits of `flags` pick its shape. */
typedef struct SpawnRegion {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radiusX; /* circle radius, or the box's half-extent along its local x */
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 radiusZ; /* the box's half-extent along its local z */
    /* 0x0C */ s32 unkC;
    /* 0x10 */ f32 yaw;     /* degrees */
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 flags;
} SpawnRegion;

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
void func_15008340(SpawnRegion *region, f32 *x, f32 *z, f32 *top, f32 *bottom) {
    // matching: each case's `break` sits after its block (with -g3 its line steers the dispatch scheduling)
    switch (region->flags & 3) {
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
        dx = RAND_UNIT() * (2.0f * region->radiusX) + -region->radiusX;
        dz = RAND_UNIT() * (2.0f * region->radiusZ) + -region->radiusZ;
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
        dist = RAND_UNIT() * region->radiusX;
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
        dist = RAND_UNIT() * region->radiusX;
        *x = region->x + dist * c;
        *z = region->z - dist * s;
        *top = (f32)region->y + (f32)region->radiusX;
        *bottom = (f32)region->y - (f32)region->radiusX;
    } break;
    }
}
