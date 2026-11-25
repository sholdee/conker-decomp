#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Object type 0xD: a three-quad puff (each quad on its own randomly rotated matrix). */
typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[3];
    Mtx mtx[3];
    u8 timer; // counts down from 10; particles spawn at 5, freed at 0
    u8 padD1;
    s16 x;
    s16 y;
    s16 z;
    f32 scale;
} PuffObj;

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_1517E05C(s32, s32, s32);
extern s32 D_8009054C;
extern s32 D_800DD220;
extern s32 D_800DD224;
extern s32 D_800DD228[];
extern s32 D_800DD230;

/* Spawns a puff object at (x, y, z) with the given scale. */
void func_151669A0(s32 x, s32 y, s32 z, f32 scale, u8 arg4, s32 arg5) {
    PuffObj *obj;
    s32 angle;
    s32 i;
    f32 fx;
    f32 fy;
    f32 fz;

    obj = func_15167A68(0xD, arg5, sizeof(PuffObj), 1, arg4, 1);
    if (obj != NULL) {
        obj->timer = 10;
        obj->x = x;
        obj->y = y;
        obj->z = z;
        obj->scale = scale;

        angle = func_150ADA20() & 0x7F;
        fx = x;
        fy = y;
        fz = z;
        for (i = 0; i < 3; i++) {
            if (scale != 1.0f) {
                func_15043D90(&obj->mtx[i], 0.0f, angle, 0.0f, scale, scale, scale, fx, fy, fz);
            } else {
                func_15043E68(&obj->mtx[i], 0.0f, angle, 0.0f, fx, fy, fz);
            }
            angle += func_150ADA20() & 0x3F;
            angle += 90;
        }
        func_1517E05C(x, y, z);
    }
}

/* Particle spawn request for func_15167D84 (same layout as Spawn150E4E04 in game_111670.c). */
typedef struct {
    /* 0x00 */ void *data;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s16 unk0C;
    /* 0x0E */ s16 unk0E;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 velX;
    /* 0x18 */ s16 velZ;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ s16 velY;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 r;
    /* 0x2D */ u8 g;
    /* 0x2E */ u8 b;
    /* 0x2F */ u8 a;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
} PuffSpawn;

extern void *D_8008CA4C[];
extern void func_150A7960(void *, f32, f32, f32, f32 *, f32 *, f32 *);
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);

/* Per-frame update: halfway through, spawns a particle from each quad; frees the object when done. */
void func_15166B50(void *self) {
    f32 mf[4][4];
    f32 x;
    f32 y;
    f32 z;
    PuffObj *obj;
    s32 i;

    obj = self;
    if (--obj->timer == 5) {
        f32 size = 4000.0f; // matching: a named local keeps the constant loaded once for both uses
        PuffSpawn spawn;

        spawn.data = D_8008CA4C[0];
        spawn.unk04 = 0;
        spawn.unk08 = 4;
        spawn.unk0C = 0;
        spawn.velX = 0;
        spawn.velZ = 0;
        spawn.unk1A = 0;
        spawn.unk1F = 5;
        spawn.velY = 0;
        spawn.unk22 = 0;
        spawn.unk24 = obj->scale * size;
        spawn.unk26 = obj->scale * size;
        spawn.unk28 = 0x200;
        spawn.unk2A = 0;
        spawn.unk2B = 0;
        spawn.r = 255;
        spawn.g = 255;
        spawn.b = 255;
        spawn.a = 255;
        spawn.unk34 = 0;
        for (i = 0; i < 3; i++) {
            guMtxL2F(mf, &obj->mtx[i]);
            func_150A7960(mf, -400.0f, 10.0f, 0.0f, &x, &y, &z);
            spawn.unk0E = (func_150ADA20() & 0x7F) + 85;
            spawn.x = x;
            spawn.y = y;
            spawn.z = z;
            func_15167D84(&spawn, 0, 0, -1, obj->unkC, obj->unk1);
        }
    } else if (obj->timer == 0) {
        func_1516972C((struct102 *)obj);
    }
}

extern Vtx D_8008B3E0[];

Gfx *func_15166D68(Gfx *pkt, PuffObj *obj, s32 arg2) {
    Mtx *mtx;
    s32 r;
    s32 s;
    s32 i;

    for (i = 0, mtx = obj->mtx; i != 0xC0; i += 0x40) {
        gSPMatrix(pkt++, mtx, G_MTX_LOAD);
        gSPVertex(pkt++, D_8008B3E0, 6, 0);
        s = 0x2800 - ((obj->timer << 12) / 10);
        gSPModifyVertex(pkt++, 0, G_MWO_POINT_ST, (s << 16) + 0x2000);
        gSPModifyVertex(pkt++, 1, G_MWO_POINT_ST, (s << 16) + 0x2000);
        gSPModifyVertex(pkt++, 2, G_MWO_POINT_ST, (s << 16) + 0x2400);
        gSPModifyVertex(pkt++, 3, G_MWO_POINT_ST, ((s + 0x800) << 16) + 0x2000);
        gSPModifyVertex(pkt++, 4, G_MWO_POINT_ST, ((s + 0x800) << 16) + 0x2000);
        gSPModifyVertex(pkt++, 5, G_MWO_POINT_ST, ((s + 0x800) << 16) + 0x2400);
        gSP1Triangle(pkt++, 5, 3, 0, 0);
        gSP1Triangle(pkt++, 2, 5, 0, 0);
        gSP1Triangle(pkt++, 1, 4, 5, 0);
        gSP1Triangle(pkt++, 1, 5, 2, 0);
        mtx++;
    }
    return pkt;
}

void func_15166F6C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_800DD228[0] = (s32)&D_8009054C;
    func_15094F70(arg0, D_800DD228[0], D_800DD220, (s32)&D_800DD230, 0, 0, 0, D_800DD224, 3);
}

extern Mtx D_80089470;

Gfx *func_15166FD8(Gfx *pkt, s32 arg1, s32 arg2) {
    gSPMatrix(pkt++, &D_80089470, G_MTX_LOAD);
    return pkt;
}
