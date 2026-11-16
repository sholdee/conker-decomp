#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 (*D_8008FAF0[])(void *, void *, s32);
extern s32 (*D_8008FAF8[])(void *);
extern void (*D_8008FB70[])(void *);
extern void *func_15167A68(s32, s32, s32, s32, u8, s32);
extern void func_151D5E30(void *);
extern f32 D_800AA390;
extern f32 D_800AA3AC;

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 pad11;
    s16 unk12;
    u8 unk14[0xC];
    u8 unk20[0xC];
    s8 unk2C;
    s8 unk2D;
    u8 pad2E[0x6];
    s8 unk34;
} Struct151B3184;

typedef struct {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
} Struct151B3F28Inner;

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 pad11[0x13F];
    Struct151B3F28Inner *unk150;
} Struct151B3F28;

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 pad11[0x27];
    f32 unk38;
    u8 pad3C[0xFC];
    f32 unk138;
    s32 unk13C;
    u8 unk140[0x10];
} Struct151B30B0;

void *func_151B30B0(s32 arg0, f32 arg1, s32 arg2, u8 arg3, s32 arg4) {
    Struct151B30B0 *temp_v0;
    Struct151B30B0 *sp24;
    f32 temp_f0;
    f32 temp_f2;

    temp_v0 = func_15167A68(0x33, arg4, arg2 + 0x150, 1, arg3, 1);
    if (temp_v0 == 0) {
        return NULL;
    }

    sp24 = temp_v0;
    memcpy(&temp_v0->unk10, (void *)arg0, 0x38);
    temp_f0 = temp_v0->unk38;
    temp_v0->unk10 |= 0xE;
    temp_f2 = (temp_f0 * temp_f0) / D_800AA390;
    temp_v0->unk138 = temp_f2 + temp_f2;
    temp_v0->unk13C = (s32)(temp_f0 * arg1 * 4096.0f);
    bzero(temp_v0->unk140, 0x10);
    return sp24;
}

void func_151B3184(Struct151B3184 *arg0) {
    s32 temp;
    u8 failed;

    failed = 0;
    if (arg0->unk10 & 1) {
        arg0->unk12 -= D_800BE9E4;
        if (arg0->unk12 < 0) {
            failed = 1;
        }
    }
    if (failed == 0) {
        temp = arg0->unk2C;
        if (temp != -1) {
            if (D_8008FAF0[temp](arg0, arg0->unk14, 1) == 0) {
                failed = 1;
            }
        }
        temp = arg0->unk2D;
        if (temp != -1) {
            if (D_8008FAF0[temp](arg0, arg0->unk20, 0) == 0) {
                failed = 1;
            }
        }
        if ((arg0->unk10 & 4) || (arg0->unk10 & 8)) {
            arg0->unk10 |= 2;
        } else {
            temp = arg0->unk34;
            if (temp != -1) {
                if (D_8008FAF8[temp](arg0) == 0) {
                    failed = 1;
                }
            }
        }
    }
    if (failed) {
        func_1516972C((struct102 *) arg0);
    }
}

typedef struct {
    f32 x, y, z;
} Vec3_151B32C8;

typedef struct {
    Vec3_151B32C8 p;
    f32 pad[3];
} Pt_151B32C8;

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 pad11[0x1D];
    u8 unk2E;
    u8 pad2F;
    f32 unk30;
    u8 pad34[0x14];
    Pt_151B32C8 pts[10];
    u8 pad138[4];
    s32 unk13C;
    u8 unk140[0x10];
} Struct151B32C8;

extern s32 (*D_8008FB10[])(Struct151B32C8 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *, u8 *, u8 *);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
void func_151D5D60(void *, s16, s32, void **, u8 *);
extern u8 D_80090DE8[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];

Gfx *func_151B32C8(Gfx *gfx, Struct151B32C8 *arg1, s16 arg2) {
    s32 i;
    Vtx *vtx;
    Vec3_151B32C8 a;
    Vec3_151B32C8 b;
    s32 t;
    f32 *pos;
    f32 mx;
    f32 my;
    f32 mz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 cx;
    f32 cy;
    f32 cz;
    f32 len;
    f32 nx;
    f32 ny;

    if (arg1->unk10 & 4) {
        return gfx;
    }
    if (arg1->unk10 & 8) {
        return gfx;
    }
    if (arg1->unk10 & 2) {
        return gfx;
    }
    func_151D5D60(arg1->unk140, arg2, 0x190, (void **)&vtx, 0);
    if (vtx != NULL) {
        u8 update;
        s32 r0;
        s32 r1;
        s32 r2;
        s32 r3;
        s32 r4;
        s32 r5;
        s32 r6;
        s32 r7;
        s32 r8;
        s32 r9;
        s32 r10;
        u8 r12;
        u8 r13;
        f32 scale;
        Pt_151B32C8 *p;

    pos = &D_800DBFF0[arg2].unk2F8;
    update = 1;
lbl1:
    if (D_8008FB10[arg1->unk2E](arg1, &r0, &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &r9, &r10, &r12, &r13) == 0) {
        return gfx;
    }
    p = arg1->pts;
    gfx = func_15142B7C(gfx, r0, r1);
    gfx = func_15142C10(gfx, r6, r7, r8, r9, &update);
    gfx = func_15142CF0(gfx, 0, 0, r2, r3, r4, r5, &update);
    gfx = func_1513F4E4(gfx, r13, &update);
    gfx = func_15142E24(gfx, (s32)D_80090DE8, 0, 0, 0, 0, 0x36, 0, 0, &update, 3);
    gfx = func_15142FBC(gfx, r10 | G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[r12].unk0 | D_800A4AC8[r12].unk4, &update);
    {
    f32 nz;

    a = p[0].p;
    b = p[1].p;
    dx = b.x - a.x;
    dy = b.y - a.y;
    dz = b.z - a.z;
    mx = a.x - pos[0];
    my = a.y - pos[1];
    mz = a.z - pos[2];
    cx = (dy * mz) - (my * dz);
    cy = (dz * mx) - (mz * dx);
    cz = (dx * my) - (mx * dy);
    len = (cx * cx) + (cy * cy) + (cz * cz);
    if (len == 0.0f) {
        cx = 0.0f;
        cy = 0.0f;
        cz = 0.0f;
    } else {
        dz = arg1->unk30 / sqrtf(len);
        cx = cx * dz;
        cy = cy * dz;
        cz = cz * dz;
    }
    vtx->v.ob[0] = a.x + cx;
    vtx->v.ob[1] = a.y + cy;
    vtx->v.ob[2] = a.z + cz;
    vtx->v.tc[0] = 0;
    vtx->v.tc[1] = 0;
    vtx->v.cn[0] = 0xFF;
    vtx->v.cn[1] = 0x64;
    vtx->v.cn[2] = 0x64;
    vtx->v.cn[3] = 0xFF;
    vtx++;
    vtx->v.ob[0] = a.x - cx;
    vtx->v.ob[1] = a.y - cy;
    vtx->v.ob[2] = a.z - cz;
    vtx->v.tc[0] = 0x3C0;
    vtx->v.tc[1] = 0;
    vtx->v.cn[0] = 0xFF;
    vtx->v.cn[1] = 0x64;
    vtx->v.cn[2] = 0x64;
    vtx->v.cn[3] = 0xFF;
    vtx++;
    t = arg1->unk13C;
    for (i = 1; i < 10; i++) {
        a = p[i - 1].p;
        b = p[i].p;
        dx = b.x - a.x;
        dy = b.y - a.y;
        dz = b.z - a.z;
        mx = b.x - pos[0];
        my = b.y - pos[1];
        mz = b.z - pos[2];
        cx = (dy * mz) - (my * dz);
        cy = (dz * mx) - (mz * dx);
        cz = (dx * my) - (mx * dy);
        len = (cx * cx) + (cy * cy) + (cz * cz);
        if (len == 0.0f) {
            cx = 0.0f;
            cy = 0.0f;
            cz = 0.0f;
        } else {
            dz = arg1->unk30 / sqrtf(len);
            cx = cx * dz;
            cy = cy * dz;
            cz = cz * dz;
        }
        vtx->v.ob[0] = b.x + cx;
        vtx->v.ob[1] = b.y + cy;
        vtx->v.ob[2] = b.z + cz;
        vtx->v.tc[0] = 0;
        vtx->v.tc[1] = t;
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0x64;
        vtx->v.cn[2] = 0x64;
        vtx->v.cn[3] = 0xFF;
        vtx++;
        vtx->v.ob[0] = b.x - cx;
        vtx->v.ob[1] = b.y - cy;
        vtx->v.ob[2] = b.z - cz;
        vtx->v.tc[0] = 0x3C0;
        vtx->v.tc[1] = t;
        vtx->v.cn[0] = 0xFF;
        vtx->v.cn[1] = 0x64;
        vtx->v.cn[2] = 0x64;
        vtx->v.cn[3] = 0xFF;
        vtx++;
        t += arg1->unk13C;
        gSPVertex(gfx++, vtx - 4, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 1, 3, 2, 0);
    }
    }
    }
    return gfx;
}

extern void (*D_8008FB68[])(void *, s32, u8);

void func_151B3A34(struct102 *arg0, s32 arg1, u8 arg2) {
    void (*fn)(void *, s32, u8) = D_8008FB68[*(u8 *)((u8 *)arg0 + 0x44)];
    if (fn != NULL) {
        fn(arg0, arg1, arg2);
    }
}

typedef struct {
    f32 x, y, z;
} Vec3f_151B3A7C;

typedef struct {
    Vec3f_151B3A7C pos;
    Vec3f_151B3A7C vel;
} Node151B3A7C;

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
    u8 pad11[3];
    Vec3f_151B3A7C start;
    Vec3f_151B3A7C end;
    u8 pad2C[0x10];
    f32 unk3C;
    f32 unk40;
    u8 pad44[4];
    Node151B3A7C nodes[10];
    f32 unk138;
} Struct151B3A7C;

s32 func_151B3A7C(Struct151B3A7C *arg0) {
    s32 pad1;
    s32 pad2;
    s32 pad3;
    s32 pad4;
    Vec3f_151B3A7C cur;
    f32 dx;
    f32 dy;
    f32 dz;
    s32 i;

    dx = -arg0->start.x + arg0->end.x;
    dy = -arg0->start.y + arg0->end.y;
    dz = -arg0->start.z + arg0->end.z;
    cur = arg0->start;
    for (i = 0; i < 10; i++) {
        Node151B3A7C *node = &arg0->nodes[i];
        node->pos = cur;
        node->vel.x = 0.0f;
        node->vel.y = 0.0f;
        node->vel.z = 0.0f;
        cur.x += dx * 0.1111111119f;
        cur.y += dy * 0.1111111119f;
        cur.z += dz * 0.1111111119f;
    }
    arg0->unk10 &= ~2;
    return 1;
}

s32 func_151B3CF0(Struct151B3A7C *arg0) {
    Vec3f_151B3A7C delta;
    Vec3f_151B3A7C mid;
    Vec3f_151B3A7C a;
    Vec3f_151B3A7C b;
    f32 inv;
    f32 nx;
    f32 nz;
    f32 px;
    f32 py;
    f32 heading;
    f32 angle;
    f32 r;
    s32 i;

    delta.x = arg0->end.x - arg0->start.x;
    delta.y = arg0->end.y - arg0->start.y;
    delta.z = arg0->end.z - arg0->start.z;
    if ((0.0001f < fabsf(delta.x)) || (0.0001f < fabsf(delta.z))) {
        mid.x = arg0->start.x + (delta.x * 0.5f);
        mid.y = arg0->start.y + (delta.y * 0.5f);
        mid.z = arg0->start.z + (delta.z * 0.5f);
        a.x = arg0->start.x - mid.x;
        a.y = arg0->start.y - mid.y;
        a.z = arg0->start.z - mid.z;
        b.x = arg0->end.x - mid.x;
        b.y = arg0->end.y - mid.y;
        b.z = arg0->end.z - mid.z;
        inv = 1.0f / sqrtf((delta.x * delta.x) + (delta.z * delta.z));
        nx = delta.z * inv;
        nz = delta.x * inv;
        px = ((b.z * nx) + (b.x * nz)) - ((a.z * nx) + (a.x * nz));
        py = b.y - a.y;
        heading = func_150484A0(py, px);
        r = sqrtf((px * px) + (py * py)) * 0.5f;
        angle = heading - 3.1415927f;
        for (i = 0; i < 10; i++, angle += 0.34906587f) {
            Node151B3A7C *point = &arg0->nodes[i];
            f32 s = sinf(angle);
            f32 c = cosf(angle);

            point->pos.x = (r * c * nz) + mid.x;
            point->pos.y = (r * s) + mid.y;
            point->pos.z = (r * c * nx) + mid.z;
        }
    }
    return 1;
}

s32 func_151B3F28(Struct151B3F28 *arg0, f32 *arg1, u8 arg2) {
    Struct151B3F28Inner **temp_v0;
    s32 ret;

    ret = 1;
    if (arg2 == 0) {
        goto no_arg;
    }

    temp_v0 = &arg0->unk150;
    if ((*temp_v0)->unk0 == 0) {
        goto failed;
    }
    if (*(u8 *)((u8 *) temp_v0 + 4) != (*temp_v0)->unk3B) {
        goto failed;
    }
    arg1[0] = (*temp_v0)->unk14;
    arg1[1] = (*temp_v0)->unk18;
    arg1[2] = (*temp_v0)->unk1C;
    arg0->unk10 &= ~4;
    goto done;

failed:
    ret = 0;
    arg0->unk10 |= 0xC;
failed_after_store:
    goto final_return;

no_arg:
    arg1[0] = 0.0f;
    arg1[1] = D_800AA3AC;
    arg1[2] = 0.0f;
    arg0->unk10 &= ~8;

done:
final_return:
    return ret;
}

s32 func_151B3FDC(Struct151B3A7C *arg0) {
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f_151B3A7C mid;
    Vec3f_151B3A7C relStart;
    Vec3f_151B3A7C relEnd;
    f32 len;
    f32 inv;
    f32 c;
    f32 s;
    f32 pad1;
    f32 u;
    f32 v;
    f32 pad2[7];
    f32 half;
    f32 dist;
    f32 d2;
    f32 ux;
    f32 uy;
    f32 w;
    f32 angle;
    f32 sn;
    f32 cs;
    f32 a;
    f32 b;
    f32 t;
    s32 i;
    Node151B3A7C *node;

    dx = arg0->end.x - arg0->start.x;
    dy = arg0->end.y - arg0->start.y;
    dz = arg0->end.z - arg0->start.z;
    if ((fabsf(dx) > 9.999999747e-05f) || (fabsf(dz) > 9.999999747e-05f)) {
        f32 h;
        mid.x = arg0->start.x + (dx * 0.5f);
        mid.y = arg0->start.y + (dy * 0.5f);
        mid.z = arg0->start.z + (dz * 0.5f);
        relStart.x = arg0->start.x - mid.x;
        relStart.y = arg0->start.y - mid.y;
        relStart.z = arg0->start.z - mid.z;
        relEnd.x = arg0->end.x - mid.x;
        relEnd.y = arg0->end.y - mid.y;
        relEnd.z = arg0->end.z - mid.z;
        len = sqrtf((dx * dx) + (dz * dz));
        inv = 1.0f / len;
        c = dz * inv;
        s = dx * inv;
        u = ((relEnd.z * c) + (relEnd.x * s)) - ((relStart.z * c) + (relStart.x * s));
        v = relEnd.y - relStart.y;
        d2 = (u * u) + (v * v);
        dist = sqrtf(d2);
        inv = 1.0f / dist;
        half = dist * 0.5f;
        ux = u * inv;
        uy = v * inv;
        h = d2 * 0.25f;
        if (arg0->unk138 < h) {
            return func_151B3A7C(arg0);
        }
        w = sqrtf(arg0->unk138 - h);
        angle = 0.0f;
        node = arg0->nodes;
        for (i = 0; i < 10; i++, angle += 0.3490658700f) {
            Node151B3A7C *n = &node[i];
            sn = sinf(angle);
            cs = cosf(angle);
            a = half * cs;
            b = w * sn;
            t = (a * ux) - (b * uy);
            n->pos.x = (t * s) + mid.x;
            n->pos.y = ((a * uy) - (b * ux)) + mid.y;
            n->pos.z = (t * c) + mid.z;
        }
    }
    return 1;
}

u8 func_151B42A4(Struct151B3A7C *arg0) {
    u8 ret;
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f_151B3A7C mid;
    Vec3f_151B3A7C relStart;
    Vec3f_151B3A7C relEnd;
    f32 len;
    f32 inv;
    f32 pad3[5];
    f32 c;
    f32 s;
    f32 aZ;
    f32 ux;
    f32 uy;
    f32 u;
    f32 v;
    f32 half;
    f32 dist;
    f32 d2;
    f32 hpad;
    f32 pad4;
    f32 w;
    f32 angle2;
    f32 angleP;
    f32 sn;
    f32 cs;
    f32 sn2;
    f32 a;
    f32 b;
    f32 t;
    f32 k;
    Vec3f_151B3A7C cur;
    Vec3f_151B3A7C np;
    f32 dX;
    f32 dY;
    f32 dZ;
    f32 vx;
    f32 vy;
    f32 aX;
    f32 aY;
    s32 i;
    Node151B3A7C *node;

    ret = 1;
    dx = -arg0->start.x + arg0->end.x;
    dy = -arg0->start.y + arg0->end.y;
    dz = arg0->end.z - arg0->start.z;
    if ((fabsf(dx) > 9.999999747e-05f) || (fabsf(dz) > 9.999999747e-05f)) {
        f32 h;
        mid.x = arg0->start.x + (dx * 0.5f);
        mid.y = arg0->start.y + (dy * 0.5f);
        mid.z = arg0->start.z + (dz * 0.5f);
        relStart.x = arg0->start.x - mid.x;
        relStart.y = arg0->start.y - mid.y;
        relStart.z = arg0->start.z - mid.z;
        relEnd.x = arg0->end.x - mid.x;
        relEnd.y = arg0->end.y - mid.y;
        relEnd.z = arg0->end.z - mid.z;
        len = sqrtf((dx * dx) + (dz * dz));
        inv = 1.0f / len;
        c = dz * inv;
        s = dx * inv;
        u = ((relEnd.z * c) + (relEnd.x * s)) - ((relStart.z * c) + (relStart.x * s));
        v = relEnd.y - relStart.y;
        d2 = (u * u) + (v * v);
        dist = sqrtf(d2);
        inv = 1.0f / dist;
        half = dist * 0.5f;
        ux = u * inv;
        uy = v * inv;
        h = d2 * 0.25f;
        if (arg0->unk138 < h) {
            ret = func_151B3A7C(arg0);
        } else {
            f32 angle;
            w = sqrtf(arg0->unk138 - h);
            node = arg0->nodes;
            for (i = 0, angle = 0.0f, angle2 = 0.0f; i < 10; i++, angle += 0.3490658700f, angle2 += 0.3490658700f) {
                Node151B3A7C *n = &node[i];
                cur = n->pos;
                sn = sinf(angle);
                cs = cosf(angle);
                sn2 = sinf(angle2);
                a = half * cs;
                b = w * sn;
                t = (a * ux) - (b * uy);
                np.x = (t * s) + mid.x;
                np.y = ((a * uy) - (b * ux)) + mid.y;
                np.z = (t * c) + mid.z;
                if (arg0->unk10 & 2) {
                    n->pos = np;
                    n->vel.x = 0.0f;
                    n->vel.y = 0.0f;
                    n->vel.z = 0.0f;
                } else if (*(s32 *)D_800DBFF4 != 0) {
                    n->pos = np;
                    n->vel = *(Vec3f_151B3A7C *)&D_800A5480;
                } else {
                    dX = (cur.x - np.x) * sn2 * arg0->unk40;
                    dY = (cur.y - np.y) * sn2 * arg0->unk40;
                    dZ = (cur.z - np.z) * sn2 * arg0->unk40;
                    n->pos.x = np.x + dX;
                    n->pos.y = np.y + dY;
                    n->pos.z = np.z + dZ;
                    aX = arg0->unk3C * dX;
                    aY = arg0->unk3C * dY;
                    aZ = arg0->unk3C * dZ;
                    n->pos.x += (n->vel.x * D_800BE9A4) + (0.5f * aX * D_800BE9A4 * D_800BE9A4);
                    n->pos.y += (n->vel.y * D_800BE9A4) + (0.5f * aY * D_800BE9A4 * D_800BE9A4);
                    n->pos.z += (n->vel.z * D_800BE9A4) + (0.5f * aZ * D_800BE9A4 * D_800BE9A4);
                    n->vel.x = n->vel.x + (aX * D_800BE9A4);
                    n->vel.y = n->vel.y + (aY * D_800BE9A4);
                    n->vel.z = n->vel.z + (aZ * D_800BE9A4);
                }
            }
        }
    }
    arg0->unk10 &= ~2;
    return ret;
}

typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x198];
    s32 unk1D4;
} Struct151B47D8Actor;

typedef struct {
    Struct151B47D8Actor *unk0;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
    f32 unk8[3];
} Struct151B47D8Part;

typedef struct {
    u8 pad0[0x10];
    u8 unk10;
} Struct151B47D8;

typedef struct {
    Struct151B47D8Part p0;
    Struct151B47D8Part p1;
} Struct151B47D8Pair;

void func_15143134(void *, s32, s32);

s32 func_151B47D8(Struct151B47D8 *arg0, Struct151B47D8Pair *arg1, s32 arg2, u8 arg3) {
    Struct151B47D8Actor *a = arg1->p0.unk0;
    Struct151B47D8Actor *b = arg1->p1.unk0;

    if (a->unk1D4 == 0 || b->unk1D4 == 0) {
        arg0->unk10 |= 0xC;
        return 1;
    }
    if (a->unk0 == 0 || a->unk3B != arg1->p0.unk4 || b->unk0 == 0 || b->unk3B != arg1->p1.unk4) {
        return 0;
    }
    arg0->unk10 &= ~(arg3 ? 4 : 8);
    func_15143134(arg3 ? arg1->p0.unk8 : arg1->p1.unk8, arg2,
                  arg3 ? a->unk1D4 + (arg1->p0.unk5 << 6) : b->unk1D4 + (arg1->p1.unk5 << 6));
    return 1;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 velocity[3];
} Struct151B48DCPoint;

typedef struct {
    u8 pad0[0x10];
    u8 flags;
    u8 pad11;
    s16 timer;
    u8 primaryState[0xC];
    u8 secondaryState[0xC];
    s8 primaryCallback;
    s8 secondaryCallback;
    u8 pad2E[6];
    s8 updateCallback;
    u8 pad35[3];
    f32 radius;
    f32 gain;
    f32 stiffness;
    u8 pad44[4];
    Struct151B48DCPoint points[10];
} Struct151B48DC;

s32 func_151B48DC(Struct151B48DC *arg0) {
    f32 start;
    f32 end;
    f32 step;
    f32 x;
    s32 i;

    start = *(f32 *)arg0->primaryState;
    end = *(f32 *)arg0->secondaryState;
    step = -(start - end) * 0.1f;
    x = start;
    for (i = 0; i < 10; i++, x += step) {
        Struct151B48DCPoint *point = &arg0->points[i];

        point->x = x;
        point->y = 0.0f;
        point->z = 0.0f;
    }
    arg0->flags &= ~2;
    return 1;
}

s32 func_151B498C(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5,
                   s32 *arg6, s32 *arg7, s32 *arg8, s32 *arg9, s32 *arg10, s32 *arg11,
                   u8 *arg12, u8 *arg13) {
    *arg1 = 0x220005;
    *arg2 = 0x40600;
    *arg3 = 0xFF;
    *arg4 = 0xFF;
    *arg5 = 0xFF;
    *arg6 = 0xFF;
    *arg7 = 0xFF;
    *arg8 = 0xFF;
    *arg9 = 0xFF;
    *arg10 = 0xFF;
    *arg11 = 0;
    *arg12 = 5;
    *arg13 = 0x2B;
    return 1;
}

typedef struct {
    u8 pad0[0xA4];
    u8 unkA4;
} Model151B4A14;

typedef struct {
    u8 pad0[0x150];
    Model151B4A14 *unk150;
} Obj151B4A14;

extern void func_1502EC34(void *, u32 *, u32 *, u32 *, u32 *);

u8 func_151B4A14(Obj151B4A14 *arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 *arg5,
                 s32 *arg6, s32 *arg7, s32 *arg8, s32 *arg9, s32 *arg10, s32 *arg11,
                 u8 *arg12, u8 *arg13) {
    u8 ret;
    s32 mode1;
    s32 mode2;
    Model151B4A14 *model;
    u32 r;
    u32 g;
    u32 b;
    u32 a;
    s32 full;
    u32 alpha;

    ret = 1;
    full = 0xFF;
    mode1 = 0x200005;
    mode2 = 0x60600;
    model = arg0->unk150;
    func_1502EC34(model, &r, &g, &b, &a);
    if (model->unkA4 & 1) {
        *arg1 = mode1;
        *arg2 = mode2;
        *arg3 = r;
        *arg4 = g;
        *arg5 = b;
        *arg6 = full;
        *arg9 = alpha = a;
        *arg8 = alpha;
        *arg7 = alpha;
        *arg10 = full;
        *arg11 = 0x100000;
        *arg12 = 5;
        *arg13 = 0x2F;
    } else {
        ret = func_151B498C(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
    }
    return ret;

}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E0560/func_151B4B78.s")

void func_151B4C1C(u8 *arg0) {
    void (*fn)(void *);

    func_151D5E30(arg0 + 0x140);
    fn = D_8008FB70[arg0[0x44]];
    if (fn != NULL) {
        fn(arg0);
    }
}

void func_151B4C1C(u8 *);

void func_151B4C6C(struct102 *arg0) {
    func_151B4C1C((u8 *)arg0);
    func_15169824(arg0);
}

void func_151B4C98(struct102 *arg0) {
    func_151B4C1C((u8 *)arg0);
    func_15169824(arg0);
}
