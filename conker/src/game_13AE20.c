#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
#include "variables.h"

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
} Point3s; /* size = 0x6 */

/* A blob shadow: a textured quad laid on the floor under its owner, tilted to the floor slope. */
typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ Mtx mtx[2];        /* translation, double-buffered by D_800BE9C0 */
    /* 0x090 */ Vtx vtx[2][4];     /* quad corners, double-buffered by D_800BE9C0 */
    /* 0x110 */ u8 *owner;
    /* 0x114 */ f32 x;
    /* 0x118 */ f32 z;
    /* 0x11C */ f32 floorY;
    /* 0x120 */ u8 ownerType;      /* 0 or 1: selects how owner is read */
    /* 0x121 */ u8 texture;        /* index into D_80091770 */
    /* 0x122 */ u8 alpha;
    /* 0x123 */ u8 visible;
    /* 0x124 */ u8 flags;
    /* 0x125 */ u8 pad125[3];
    /* 0x128 */ Point3s floorTri[3]; /* cached floor triangle, only allocated when created with flag 1 */
} Shadow; /* size = 0x128 or 0x13A */

/* Allocates a shadow for owner and sets up its two quads' texture coordinates. */
Shadow *func_1510D970(s32 ownerType, u8 *owner, s32 texture, s32 createFlags, s32 flags) {
    Shadow *shadow;
    s32 size;
    s32 i;

    size = 0x128;
    if (createFlags & 1) {
        size = 0x13A;
    }

    shadow = func_15167A68(0x4B, 0, size, 1, 0xFF, 1);
    if (shadow != NULL) {
        for (i = 0; i < 2; i++) {
            shadow->vtx[i][0].v.flag = 0;
            shadow->vtx[i][0].v.tc[0] = 0x800;
            shadow->vtx[i][0].v.tc[1] = 0;
            shadow->vtx[i][1].v.flag = 0;
            shadow->vtx[i][1].v.tc[0] = 0;
            shadow->vtx[i][1].v.tc[1] = 0;
            shadow->vtx[i][2].v.flag = 0;
            shadow->vtx[i][2].v.tc[0] = 0;
            shadow->vtx[i][2].v.tc[1] = 0x800;
            shadow->vtx[i][3].v.flag = 0;
            shadow->vtx[i][3].v.tc[0] = 0x800;
            shadow->vtx[i][3].v.tc[1] = 0x800;
        }

        shadow->ownerType = ownerType;
        shadow->owner = owner;
        shadow->texture = texture;
        shadow->flags = flags;

        if (createFlags & 1) {
            for (i = 0; i < 9; i++) {
                ((s16 *)shadow->floorTri)[i] = 0x7FFF;
            }
        }
    }

    return shadow;
}

/* This file's view of a struct127 game object (cf. GameObj in game_58F80.c), shadow owner type 0. */
typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ u8 id;
    /* 0x005 */ u8 pad5[0xF];
    /* 0x014 */ f32 x_position;
    /* 0x018 */ f32 y_position;
    /* 0x01C */ f32 z_position;
    /* 0x020 */ u8 pad20[0x5A];
    /* 0x07A */ u16 unk7A; /* high byte: yaw angle */
    /* 0x07C */ u8 pad7C[0xD0];
    /* 0x14C */ f32 xz_scale;
    /* 0x150 */ u8 pad150[0x10];
    /* 0x160 */ u16 unk160;
    /* 0x162 */ u8 pad162[0x1E];
    /* 0x180 */ f32 floorY;
    /* 0x184 */ u8 pad184[4];
    /* 0x188 */ Point3s **floorTriPtrs;
    /* 0x18C */ Point3s floorTri[3];
} GameObj;

/* This file's view of the 0xA0-byte objects at D_800DBEF4 (cf. Game13F9D0Obj in game_13F9D0.c), owner type 1. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Point3s pos;
    /* 0x16 */ u8 pad16[0x16];
    /* 0x2C */ f32 scaleX;
    /* 0x30 */ u8 pad30[0x1D];
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ u8 pad4E[0x20];
    /* 0x6E */ u8 unk6E;
    /* 0x6F */ u8 pad6F[2];
    /* 0x71 */ u8 unk71;
    /* 0x72 */ u8 pad72[0x18];
    /* 0x8A */ u8 alpha;
    /* 0x8B */ u8 viewAlpha[5];
    /* 0x90 */ u8 pad90[4];
    /* 0x94 */ Shadow *shadow;
} Game13F9D0Obj;

extern f32 func_15048A40(u8 angle); /* sin */
extern f32 func_150489B0(u8 angle); /* cos */
extern s32 func_150A3FC4(f32, f32, s32, Point3s *, f32 *);
s32 func_1510E388(Point3s **triPtrs, Point3s *tri, f32 *slopeX, f32 *slopeZ);
extern void func_1510E7A4(s32, Point3s *, f32 *, s32, s32, s32, f32, f32, f32, f32, s32, s32, f32, f32);

/* Places the shadow under its owner and builds its quad, tilted to the floor and faded with height. */
void func_1510DA84(Shadow *shadow) {
    s32 i;
    s32 alpha;
    GameObj *obj;
    s32 pad[4]; // matching: -g3 gives unused locals stack slots; these pads keep corner, x, y... at the target offsets
    f32 corner[2][3];
    f32 height;
    s32 pad5;
    f32 ratio;
    f32 slopeZ;
    f32 slopeX;
    f32 tmp;
    f32 lenB;
    f32 lenA;
    f32 dist;
    f32 x;
    f32 top;
    f32 z;
    f32 y;
    f32 scale;
    Point3s **triPtrs;
    Point3s *tri;
    u8 angle;
    f32 sinYaw;
    f32 cosYaw;
    s32 pad6;
    Game13F9D0Obj *prop;

    shadow->visible = FALSE;
    switch (shadow->ownerType) {
        case 0:
            obj = (GameObj *)shadow->owner;
            x = obj->x_position;
            top = obj->y_position;
            z = obj->z_position;
            y = obj->floorY;
            if (obj->id == 0x25) {
                top = y;
            }
            triPtrs = obj->floorTriPtrs;
            tri = obj->floorTri;
            scale = (f32)(u32)obj->unk160 * obj->xz_scale; // matching: the (u32) gives the unsigned-convert fixup
            angle = (obj->unk7A >> 8) + 0x20;
            break;
        case 1:
            prop = (Game13F9D0Obj *)shadow->owner;
            if (prop->unk6E == 1) {
                return;
            }
            tri = prop->shadow->floorTri;
            if (func_150A3FC4(prop->pos.x, prop->pos.z, 0, tri, &y) == 0) {
                func_1510E7A4(0, tri, &y, 0, 0, 0, prop->pos.x, prop->pos.y, prop->pos.z, prop->pos.y,
                              prop->unk71 + 1, 0, -10000.0f, prop->pos.y);
            }
            triPtrs = NULL;
            x = prop->pos.x;
            top = prop->pos.y;
            z = prop->pos.z;
            scale = (f32)(prop->unk4D * 4) * prop->scaleX;
            angle = 0;
            break;
    }
    shadow->x = x;
    shadow->z = z;
    shadow->floorY = y;
    if (y > top) {
        return;
    }
onFloor: // matching: an unused label splits the block, so the call setup stays after the y > top branch
    if (func_1510E388(triPtrs, tri, &slopeX, &slopeZ)) {
        height = top - y;
        if (scale == 0.0f) {
            scale = 1.0f;
        }
        dist = 200.0f + height;
        if (dist == 0.0f) {
            dist = 0.0001f;
        }
        if (dist > 800.0f) {
            dist = 800.0f;
        }
        ratio = scale / dist;
        if (shadow->flags & 1) {
            alpha = shadow->alpha;
            if (alpha != 0xFF) {
                alpha += D_800BE9E4 * 5;
                if (alpha >= 0x100) {
                    alpha = 0xFF;
                }
            }
        } else {
            alpha = 32000.0f / dist;
            if (alpha > 0xA0) {
                alpha = 0xA0;
            }
        }
        shadow->alpha = alpha;

        corner[0][0] = 0.0; // matching: double literals here (and below); 0.0f changes the code
        corner[0][2] = -100.0f;
        if (angle != 0) {
            tmp = corner[0][0];
            sinYaw = func_15048A40(angle);
            cosYaw = func_150489B0(angle);
            corner[0][0] = corner[0][2] * sinYaw + corner[0][0] * cosYaw;
            corner[0][2] = corner[0][2] * cosYaw - tmp * sinYaw;
        }
        corner[1][0] = -100.0f;
        corner[1][2] = 0.0;
        corner[0][1] = slopeX * corner[0][0] + corner[0][2] * slopeZ;
        if (angle != 0) {
            tmp = corner[1][0];
            corner[1][0] = corner[1][2] * sinYaw + corner[1][0] * cosYaw;
            corner[1][2] = corner[1][2] * cosYaw - tmp * sinYaw;
        }
        corner[1][1] = slopeX * corner[1][0] + corner[1][2] * slopeZ;

        lenA = corner[0][0] * corner[0][0] + corner[0][1] * corner[0][1] + corner[0][2] * corner[0][2];
        if (lenA > 100000000.0f) {
            lenA = 100000000.0f;
        }
        if (lenA < 10000.0f) {
            lenA = 10000.0f;
        }
        lenA = sqrtf(lenA);
        lenB = corner[1][0] * corner[1][0] + corner[1][1] * corner[1][1] + corner[1][2] * corner[1][2];
        if (lenB > 100000000.0f) {
            lenB = 100000000.0f;
        }
        if (lenB < 10000.0f) {
            lenB = 10000.0f;
        }
        lenB = sqrtf(lenB);

        for (i = 0; i < 3; i++) {
            corner[0][i] = corner[0][i] * 100.0f * ratio / lenA;
            corner[1][i] = corner[1][i] * 100.0f * ratio / lenB;
            shadow->vtx[D_800BE9C0][0].v.ob[i] = corner[0][i];
            shadow->vtx[D_800BE9C0][1].v.ob[i] = corner[1][i];
            shadow->vtx[D_800BE9C0][2].v.ob[i] = -corner[0][i];
            shadow->vtx[D_800BE9C0][3].v.ob[i] = -corner[1][i];
        }
        shadow->visible = TRUE;
    }
}

extern void func_150A7D00(void *, s32, s32, s32);
extern s32 func_1510D0EC(s32, s32, s32, s32);
extern s32 func_1506196C(u8 *arg0, s32 arg1);
extern s32 D_80091770[];
extern s32 D_800DD1B4;
extern s32 D_800DD1B0[];
extern s16 D_800DD1C6;

typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ Mtx mtx[2];
    /* 0x090 */ Vtx vtx[2][4];
    /* 0x110 */ u8 *unk110;
    /* 0x114 */ s32 unk114;
    /* 0x118 */ s32 unk118;
    /* 0x11C */ s32 unk11C;
    /* 0x120 */ u8 unk120;
    /* 0x121 */ u8 unk121;
    /* 0x122 */ u8 unk122;
    /* 0x123 */ u8 unk123;
} Struct_1510E120;

Gfx *func_1510E120(Gfx *gfx, Struct_1510E120 *arg1, s16 arg2) {
    s32 scale;
    s32 alpha;
    s32 tex;

    if (arg1->unk123 != 0) {
        func_150A7D00(&arg1->mtx[D_800BE9C0], arg1->unk114, arg1->unk11C, arg1->unk118);
        gSPMatrix(gfx++, &arg1->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        if (D_800DD1B4 != arg1->unk121) {
            D_800DD1B0[1] = arg1->unk121; /* same address as D_800DD1B4; a distinct symbol keeps the split %hi/%lo */
            tex = func_1510D0EC(D_80091770[arg1->unk121], 0, 0x3E, 0);
            gDPPipeSync(gfx++);
            gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, tex);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
        }
        gDPPipeSync(gfx++);
        gSPClearGeometryMode(gfx++, G_CULL_BOTH);
        scale = arg1->unk122;
        switch (arg1->unk120) {
            case 0:
                alpha = func_1506196C(arg1->unk110, arg2);
                break;
            case 1:
                alpha = (arg1->unk110[0x8A] * arg1->unk110[arg2 + 0x8B] + 0xFF) >> 8;
                break;
        }
        scale = (scale * alpha + 0xFF) >> 8;
        if (scale != D_800DD1C6) {
            D_800DD1C6 = scale;
            gDPPipeSync(gfx++);
            gDPSetPrimColor(gfx++, 1, 0, 0, 0, 0, scale);
        }
        gSPVertex(gfx++, arg1->vtx[D_800BE9C0], 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
        gDPPipeSync(gfx++);
    }
    return gfx;
}

s32 func_1510E388(Point3s **pts, Point3s *verts, f32 *outA, f32 *outB) {
    f32 ax;
    f32 ay;
    f32 az;
    f32 bx;
    f32 by;
    f32 bz;
    f32 tmp;
    f32 d0;
    f32 d1;
    f32 t;
    s32 i;
    s32 i0;
    s32 i1;
    s32 i2;
    s32 sel;

    i = 0;
    if ((pts == NULL) && (verts == NULL)) {
        *outB = 0.0f;
        *outA = 0.0f;
        return 0;
    }

    do {
        sel = 1;
        if (i >= 3) {
            i0 = i - 3;
        } else {
            i0 = i;
        }
        if (i0 != 2) {
            i1 = i0 + 1;
        } else {
            i1 = 0;
        }
        if (i1 != 2) {
            i2 = i1 + 1;
        } else {
            i2 = 0;
        }

        if (verts != NULL) {
            ax = verts[i1].x - verts[i0].x;
            ay = verts[i1].y - verts[i0].y;
            az = verts[i1].z - verts[i0].z;
            bx = verts[i2].x - verts[i0].x;
            by = verts[i2].y - verts[i0].y;
            bz = verts[i2].z - verts[i0].z;
        } else {
            ax = pts[i1]->x - pts[i0]->x;
            ay = pts[i1]->y - pts[i0]->y;
            az = pts[i1]->z - pts[i0]->z;
            bx = pts[i2]->x - pts[i0]->x;
            by = pts[i2]->y - pts[i0]->y;
            bz = pts[i2]->z - pts[i0]->z;
        }

        if (i >= 3) {
            tmp = ax;
            ax = bx;
            bx = tmp;
            tmp = ay;
            ay = by;
            by = tmp;
            tmp = az;
            az = bz;
            bz = tmp;
        }
        d0 = bx * az;
        d1 = ax * bz;
        if (d0 == d1) {
            sel = 0;
        }
        if (bx == 0.0f) {
            sel = 0;
        }
        i++;
        if (i == 6) {
            sel = 2;
        }
    } while (sel == 0);

    if (sel == 2) {
        *outA = 0.0f;
        *outB = 0.0f;
        return 0;
    }

    t = ((bx * ay) - (ax * by)) / (d0 - d1);
    *outB = t;
    *outA = (by - (t * bz)) / bx;
    return 1;
}

extern Mtx D_80089470;

Gfx *func_1510E634(Gfx *pkt, s32 arg1, s32 arg2) {
    gSPMatrix(pkt++, &D_80089470, G_MTX_LOAD);
    return pkt;
}
