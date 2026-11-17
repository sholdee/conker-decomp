#include <ultra64.h>
/* functions.h declares func_150A29C8(u8, u16); golden reads both args as full words. */
#define func_150A29C8 func_150A29C8_hdr
#include "functions.h"
#undef func_150A29C8
#define D_800D3098 D_800D3098_array_decl_in_variables_h
#include "variables.h"
#undef D_800D3098

extern f32 D_8009F5A0;
extern f32 D_8009F5A4;

struct Func150A0D14Arg {
    u8 pad0[0xC];
    f32 unkC;
    f32 unk10;
    u8 pad14[0x10];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
};

/* Partial view of struct108 (the camera record) covering the target fields. */
struct Func150A09D0Camera {
    u8  pad0[0x938];
    f32 unk938;
    f32 unk93C;
    f32 unk940;
    f32 unk944;
    u8  unk948;
    u8  unk949;
    u8  pad94A[0x56];
};

/* struct178 with the fields past 0x6 named (structs.h has them as a blob). */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8  pad8[0x8];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8  unk14;
    /* 0x15 */ u8  unk15;
    /* 0x16 */ u8  pad16;
    /* 0x17 */ u8  unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ u8  pad24[0x10];
} ObjRec; /* size 0x34 */

/* variables.h declares D_800D3098 as `struct178 D_800D3098[73]`; the symbol really
   holds a POINTER to the object table.  Shadowed above so this file sees the real type. */
extern ObjRec *D_800D3098;

void func_1000FC18(u16 arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4);
s32 func_150A1DA0(u8 *arg0, struct178 *arg1, s32 arg2);
void func_150A23E4(struct178 *arg0);
void func_150A09D0(s32 arg0);
void func_150A0D14(struct Func150A0D14Arg *arg0);

void func_15035808(s32, s32, f32, f32, f32, f32, f32, f32, f32, f32);
s32 func_150AD9A0(s16, s16, s16);
void func_150A0D8C(void);
s32 func_150A1040(s32 arg0);

struct Func150A104CArg {
    u8 pad0[0x84];
    u32 unk84;
    u8 pad88[0x2F8 - 0x88];
    f32 unk2F8;
    f32 unk2FC;
    f32 unk300;
    f32 unk304;
    f32 unk308;
    f32 unk30C;
    u8 pad310[0x364 - 0x310];
    f32 unk364;
};
void func_150A104C(struct Func150A104CArg *arg0, ObjRec **arg1, s32 arg2);
void func_150A23E4(struct178 *arg0);

extern u8 D_800D3090;
extern u8 D_800D9EB8[][3];
void func_150E7290(s32 arg0, s32 arg1, s32 arg2);
void func_150E68B0(u8 *arg0, s32 arg1, u8 *arg2, u8 arg3);
void func_151CE51C(u8 *arg0, u8 *arg1);
void func_150A25D4(u8 *arg0, u8 *arg1, s32 arg2, s64 arg3);

s32 func_150A1DA0(u8 *arg0, struct178 *arg1, s32 arg2);
void func_15172E7C(u8 *arg0, s32 arg1, s32 arg2);
void func_150A278C(u8 *arg0);

extern u64 D_800D30F0[][3];
void func_150A2864(u32 arg0, s32 arg1);
void func_150A2940(s32 arg0, s32 arg1, s32 arg2);
s32 func_150A29C8(u32 arg0, u32 arg1);
s32 func_150A2AEC(u32 arg0, s32 arg1, s32 *arg2);

extern u64 D_800D30F0[][3];
s32 func_150A2CA4(u32 arg0);
s32 func_150A2D84(s32 a0, s32 a1);

s32 func_150A2E4C(struct178 *arg0, f32 x, f32 y, f32 z);
s32 func_150A2EE4(s32 a0, s32 a1);
s32 func_150A2FA4(s32 a0, s32 a1);
s32 func_150A3058(s32 arg0, s16 arg1, s16 arg2, s16 arg3);
ObjRec *func_150A3194(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_150A32B4(struct178 *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_150A3330(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4);
void func_150A3398(s32 arg0, u16 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_150A3444(s32 a0, s16 a1, s16 a2, s16 a3);

s32 func_150A3504();

s32 func_150A34B0(u8 *arg0) {
    s32 ret;

    if (arg0[0x14] == 1) {
        return 0;
    }

    ret = 0;
    if (!(arg0[0x15] & 3)) {
        return func_150A3504();
    }
    return ret;
}

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 padA;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 pad14[0x10];
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
} Cylinder;

extern f32 D_8009F674;
extern f32 D_8009F678;

s32 func_150A3504(Cylinder *cyl, f32 *p0, f32 *p1, f32 *out) {
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
        c = cyl->unk30;
        s = cyl->unk2C;
        pts[i][0] = dx * c - dz * s;
        dz = dz * c + dx * s;
        c = cyl->unk28;
        s = cyl->unk24;
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
        ang = -(cyl->unkC * D_8009F674);
        s = func_150AD78C(ang);
        c = func_150AD780(ang);
        out[2] = hz * c - hy * s;
        out[1] = hy * c + hz * s;
        ang = -(cyl->unk10 * D_8009F678);
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
