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

s32 func_150A34B0(CollisionCylinder *cyl, f32 *p0, f32 *p1, f32 *hit) {
    if (cyl->unk14 == 1) {
        return FALSE;
    }
    if (!(cyl->unk15 & 3)) {
        return func_150A3504(cyl, p0, p1, hit);
    }
    return FALSE;
}

s32 func_150A3504(CollisionCylinder *cyl, f32 *p0, f32 *p1, f32 *out) {
    s32 i;
    f32 inv;
    f32 tmin;
    f32 tmax;
    f32 pts[2][3];
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
    f32 lo;
    f32 hi;
    f32 b;
    f32 t1;
    f32 t2;

    dx = p0[0] - cyl->x;
    dy = p0[1] - cyl->y;
    dz = p0[2] - cyl->z;
    for (i = 0; i < 2; i++) {
        c = cyl->cosRotY;
        s = cyl->sinRotY;
        pts[i][0] = dx * c - dz * s;
        dz = dz * c + dx * s;
        c = cyl->cosRotX;
        s = cyl->sinRotX;
        pts[i][2] = dz * c - dy * s;
        pts[i][1] = dy * c + dz * s;
        if (i == 0) {
            dx = p1[0] - cyl->x;
            dy = p1[1] - cyl->y;
            dz = p1[2] - cyl->z;
        }
    }
    org[0] = pts[0][0];
    org[2] = pts[0][2];
    dir[0] = pts[1][0] - pts[0][0];
    dir[1] = pts[1][1] - pts[0][1];
    org[1] = pts[0][1];
    dir[2] = pts[1][2] - pts[0][2];
    if (dir[1] != 0.0f) {
        f32 tmp;

        inv = 1.0f / dir[1];
        tmin = -org[1] * inv;
        tmax = (cyl->height - pts[0][1]) * inv;
        if (tmax < tmin) {
            tmp = tmin;
            tmin = tmax;
            tmax = tmp;
        }
        if (tmax < 0.0f || tmin > 1.0f) {
            return 0;
        }
        if (tmin < 0.0f) {
            tmin = 0.0f;
        }
        if (tmax > 1.0f) {
            tmax = 1.0f;
        }
    } else {
        if (org[1] < 0.0f || cyl->height < pts[0][1]) {
            return 0;
        }
        tmin = 0.0f;
        tmax = 1.0f;
    }
    a = dir[0] * dir[0] + dir[2] * dir[2];
    if (a == 0.0f) {
        return 0;
    }
    b = 2.0f * (dir[0] * org[0] + dir[2] * org[2]);
    disc = b * b - 4.0f * a * ((org[0] * org[0] + org[2] * org[2]) - cyl->radius * cyl->radius);
    if (disc < 0.0f) {
        return 0;
    }
    sq = sqrtf(disc);
    inv = 0.5f / a;
    t1 = (-b + sq) * inv;
    t2 = (-b - sq) * inv;
    lo = t1;
    hi = t2;
    if (hi < lo) {
        sq = lo;
        lo = hi;
        hi = sq;
    }
    if (hi < 0.0f || lo > 1.0f) {
        return 0;
    }
    if (lo < 0.0f) {
        lo = 0.0f;
    }
    if (hi > 1.0f) {
        hi = 1.0f;
    }
    if (hi < tmin || tmax < lo) {
        return 0;
    }
    if (lo < tmin) {
        a = tmin;
    } else {
        a = lo;
    }
    {
        f32 hz;
        f32 hy;
        f32 hx;

        hx = org[0] + dir[0] * a;
        hy = org[1] + dir[1] * a;
        hz = org[2] + dir[2] * a;
        ang = -(cyl->rotX * 0.01745329238f);
        s = func_150AD78C(ang);
        c = func_150AD780(ang);
        out[2] = hz * c - hy * s;
        out[1] = hy * c + hz * s;
        ang = -(cyl->rotY * 0.01745329238f);
        s = func_150AD78C(ang);
        c = func_150AD780(ang);
        out[0] = hx * c - out[2] * s;
        out[2] = out[2] * c + hx * s;
        out[0] += cyl->x;
        out[1] += cyl->y;
        out[2] += cyl->z;
    }
    return 1;
}
