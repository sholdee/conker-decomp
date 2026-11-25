#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* An entry of the object table at D_800D3098 (struct178, a blob in structs.h).
   Its collision volume is a vertical cylinder at (x, y, z), rotated by rotX then rotY
   (degrees); the sines and cosines of the two angles are cached at 0x24-0x30. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 padA;
    /* 0x0C */ f32 rotX;
    /* 0x10 */ f32 rotY;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 pad16[0xE];
    /* 0x24 */ f32 sinRotX;
    /* 0x28 */ f32 cosRotX;
    /* 0x2C */ f32 sinRotY;
    /* 0x30 */ f32 cosRotY;
} CollisionCylinder; /* size 0x34 */

s32 func_150A3504(CollisionCylinder *cyl, f32 *p0, f32 *p1, f32 *hit);

/* Tests the segment p0-p1 against cyl (skipped when unk14 == 1 or unk15 & 3 is set);
   on a hit writes the entry point to hit and returns TRUE. */
s32 func_150A34B0(CollisionCylinder *cyl, f32 *p0, f32 *p1, f32 *hit) {
    if (cyl->unk14 == 1) {
        return FALSE;
    }
    if (!(cyl->unk15 & 3)) {
        return func_150A3504(cyl, p0, p1, hit);
    }
    return FALSE;
}

/* Segment p0-p1 vs. cylinder: moves both points into the cylinder's local space, clips the
   segment to the height slab [0, height] and to the radius, and on overlap writes the first
   contact point (back in world space) to hit and returns TRUE.
   func_150AD78C / func_150AD780 are sin / cos of an angle in radians. */
s32 func_150A3504(CollisionCylinder *cyl, f32 *p0, f32 *p1, f32 *hit) {
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

    /* Rotate p0, then p1, into cylinder space (rotate by rotY, then rotX). */
    dx = p0[0] - cyl->x;
    dy = p0[1] - cyl->y;
    dz = p0[2] - cyl->z;
    for (i = 0; i < 2; i++) {
        c = cyl->cosRotY;
        s = cyl->sinRotY;
        local[i][0] = dx * c - dz * s;
        dz = dz * c + dx * s;
        c = cyl->cosRotX;
        s = cyl->sinRotX;
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
        tSlabMax = (cyl->height - local[0][1]) * scale;
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
        if (org[1] < 0.0f || cyl->height < local[0][1]) {
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
    disc = b * b - 4.0f * a * ((org[0] * org[0] + org[2] * org[2]) - cyl->radius * cyl->radius);
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
        /* Rotate the contact point back to world space (by -rotX, then -rotY). */
        ang = -(cyl->rotX * 0.01745329238f);
        s = func_150AD78C(ang);
        c = func_150AD780(ang);
        hit[2] = hz * c - hy * s;
        hit[1] = hy * c + hz * s;
        ang = -(cyl->rotY * 0.01745329238f);
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
