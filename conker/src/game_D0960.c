#include <ultra64.h>
#include "functions.h"
#include "variables.h"



/* Tests the segment p0-p1 against cyl (only an active ZONE_AREA_CYLINDER zone);
   on a hit writes the entry point to hit and returns TRUE. */
s32 func_150A34B0(TriggerZone *cyl, f32 *p0, f32 *p1, f32 *hit) {
    if (cyl->inactive == 1) {
        return FALSE;
    }
    if (ZONE_AREA_TYPE(cyl) == ZONE_AREA_CYLINDER) {
        return func_150A3504(cyl, p0, p1, hit);
    }
    return FALSE;
}

/* Segment p0-p1 vs. cylinder: moves both points into the cylinder's local space, clips the
   segment to the height slab [0, height] and to the radius, and on overlap writes the first
   contact point (back in world space) to hit and returns TRUE.
   func_150AD78C / func_150AD780 are sin / cos of an angle in radians. */
s32 func_150A3504(TriggerZone *cyl, f32 *p0, f32 *p1, f32 *hit) {
    s32 i;
    f32 scale;
    f32 tSlabMin;
    f32 tSlabMax;
    f32 local[2][3];
    f32 s;
    f32 c;
    f32 ang;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dir[3];
    f32 org[3];
    f32 disc;
    f32 a;
    f32 sq;
    f32 tCylMin;
    f32 tCylMax;
    f32 b;
    f32 t1; // matching: the roots go through their own stack slots before lo/hi
    f32 t2;

    /* Rotate p0, then p1, into cylinder space (rotate by yaw, then pitch). */
    dx = p0[0] - cyl->x;
    dy = p0[1] - cyl->y;
    dz = p0[2] - cyl->z;
    for (i = 0; i < 2; i++) {
        c = cyl->cosYaw;
        s = cyl->sinYaw;
        local[i][0] = dx * c - dz * s;
        dz = dz * c + dx * s;
        c = cyl->cosPitch;
        s = cyl->sinPitch;
        local[i][2] = dz * c - dy * s;
        local[i][1] = dy * c + dz * s;
        if (i == 0) {
            dx = p1[0] - cyl->x;
            dy = p1[1] - cyl->y;
            dz = p1[2] - cyl->z;
        }
    }
    org[0] = local[0][0];
    org[2] = local[0][2];
    dir[0] = local[1][0] - local[0][0];
    dir[1] = local[1][1] - local[0][1];
    org[1] = local[0][1];
    dir[2] = local[1][2] - local[0][2];
    /* Range of t in [0, 1] where the segment lies within the height slab. */
    if (dir[1] != 0.0f) {
        f32 tmp;

        scale = 1.0f / dir[1];
        tSlabMin = -org[1] * scale;
        tSlabMax = (cyl->ySize - local[0][1]) * scale;
        if (tSlabMax < tSlabMin) {
            tmp = tSlabMin;
            tSlabMin = tSlabMax;
            tSlabMax = tmp;
        }
        if (tSlabMax < 0.0f || tSlabMin > 1.0f) {
            return FALSE;
        }
        if (tSlabMin < 0.0f) {
            tSlabMin = 0.0f;
        }
        if (tSlabMax > 1.0f) {
            tSlabMax = 1.0f;
        }
    } else {
        if (org[1] < 0.0f || cyl->ySize < local[0][1]) {
            return FALSE;
        }
        tSlabMin = 0.0f;
        tSlabMax = 1.0f;
    }
    /* Range of t in [0, 1] where the segment lies within the radius (quadratic in t). */
    a = dir[0] * dir[0] + dir[2] * dir[2];
    if (a == 0.0f) {
        return FALSE;
    }
    b = 2.0f * (dir[0] * org[0] + dir[2] * org[2]);
    disc = b * b - 4.0f * a * ((org[0] * org[0] + org[2] * org[2]) - cyl->xSize * cyl->xSize);
    if (disc < 0.0f) {
        return FALSE;
    }
    sq = sqrtf(disc);
    scale = 0.5f / a;
    t1 = (-b + sq) * scale;
    t2 = (-b - sq) * scale;
    tCylMin = t1;
    tCylMax = t2;
    if (tCylMax < tCylMin) {
        sq = tCylMin; // matching: the swap reuses sq rather than a fresh temp
        tCylMin = tCylMax;
        tCylMax = sq;
    }
    if (tCylMax < 0.0f || tCylMin > 1.0f) {
        return FALSE;
    }
    if (tCylMin < 0.0f) {
        tCylMin = 0.0f;
    }
    if (tCylMax > 1.0f) {
        tCylMax = 1.0f;
    }
    if (tCylMax < tSlabMin || tSlabMax < tCylMin) {
        return FALSE;
    }
    /* First t inside both ranges. */
    if (tCylMin < tSlabMin) {
        a = tSlabMin; // matching: reuses a for t; a separate local grows the frame
    } else {
        a = tCylMin;
    }
    {
        f32 hz;
        f32 hy;
        f32 hx;

        hx = org[0] + dir[0] * a;
        hy = org[1] + dir[1] * a;
        hz = org[2] + dir[2] * a;
        /* Rotate the contact point back to world space (by -pitch, then -yaw). */
        ang = -(cyl->pitch * 0.01745329238f);
        s = func_150AD78C(ang);
        c = func_150AD780(ang);
        hit[2] = hz * c - hy * s;
        hit[1] = hy * c + hz * s;
        ang = -(cyl->yaw * 0.01745329238f);
        s = func_150AD78C(ang);
        c = func_150AD780(ang);
        hit[0] = hx * c - hit[2] * s;
        hit[2] = hit[2] * c + hx * s;
        hit[0] += cyl->x;
        hit[1] += cyl->y;
        hit[2] += cyl->z;
    }
    return TRUE;
}
