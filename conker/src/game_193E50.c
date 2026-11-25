#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Object type 0xD: a puff of three quads, each on its own matrix with a different yaw. */
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

/* Spawns a puff at (x, y, z); its quads are yawed 90-153 degrees apart from a random start. */
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
            angle += (func_150ADA20() & 0x3F) + 90;
        }
        func_1517E05C(x, y, z);
    }
}

extern void *D_8008CA4C[];
extern void func_150A7960(void *, f32, f32, f32, f32 *, f32 *, f32 *);
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);

/* Per-frame update: halfway through, spawns a particle from each quad; frees the object when done. */
void func_15166B50(void *self) {
    MtxF mf;
    f32 x;
    f32 y;
    f32 z;
    PuffObj *obj;
    s32 i;

    obj = self;
    if (--obj->timer == 5) {
        f32 size = 4000.0f; // matching: a named local keeps the constant loaded once for both uses
        BillboardTemplate spawn;

        spawn.anim = D_8008CA4C[0];
        spawn.unk4 = 0;
        spawn.unk8 = 4;
        spawn.frame = 0;
        spawn.velX = 0;
        spawn.velZ = 0;
        spawn.unk1A = 0;
        spawn.callback = 5;
        spawn.velY = 0;
        spawn.accelY = 0;
        spawn.width = obj->scale * size;
        spawn.height = obj->scale * size;
        spawn.life = 0x200;
        spawn.fadeStep = 0;
        spawn.unk2B = 0;
        spawn.primR = 255;
        spawn.primG = 255;
        spawn.primB = 255;
        spawn.primA = 255;
        spawn.flags = 0;
        for (i = 0; i < 3; i++) {
            guMtxL2F(mf, &obj->mtx[i]);
            func_150A7960(mf, -400.0f, 10.0f, 0.0f, &x, &y, &z);
            spawn.frameStep = (func_150ADA20() & 0x7F) + 85;
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

/* Draws the three quads; their texture scrolls in S as the timer runs down. */
Gfx *func_15166D68(Gfx *gfx, PuffObj *obj, s32 arg2) {
    s32 texS;
    s32 i;

    for (i = 0; i < 3; i++) {
        gSPMatrix(gfx++, &obj->mtx[i], G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPVertex(gfx++, D_8008B3E0, 6, 0);
        texS = 0x2800 - ((obj->timer << 12) / 10);
        gSPModifyVertex(gfx++, 0, G_MWO_POINT_ST, (texS << 16) + 0x2000);
        gSPModifyVertex(gfx++, 1, G_MWO_POINT_ST, (texS << 16) + 0x2000);
        gSPModifyVertex(gfx++, 2, G_MWO_POINT_ST, (texS << 16) + 0x2400);
        gSPModifyVertex(gfx++, 3, G_MWO_POINT_ST, ((texS + 0x800) << 16) + 0x2000);
        gSPModifyVertex(gfx++, 4, G_MWO_POINT_ST, ((texS + 0x800) << 16) + 0x2000);
        gSPModifyVertex(gfx++, 5, G_MWO_POINT_ST, ((texS + 0x800) << 16) + 0x2400);
        gSP1Triangle(gfx++, 5, 3, 0, 0);
        gSP1Triangle(gfx++, 2, 5, 0, 0);
        gSP1Triangle(gfx++, 1, 4, 5, 0);
        gSP1Triangle(gfx++, 1, 5, 2, 0);
    }
    return gfx;
}

Gfx *func_15094F70(Gfx *gdl, void *texture, s32 frame, void *sprite, s32 tmem, s32 tile, s32 sfrac, s32 cm,
                  s32 assetArg);
extern s32 D_8009054C;

/* Loads texture D_8009054C, using the frame and mode already in D_800DD220/D_800DD224. */
Gfx *func_15166F6C(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3) {
    D_800DD228 = &D_8009054C;
    return func_15094F70(gfx, D_800DD228, D_800DD220, &D_800DD230, 0, 0, 0, D_800DD224, 3);
}

extern Mtx D_80089470;

/* Loads D_80089470 as the modelview matrix. */
Gfx *func_15166FD8(Gfx *gfx, s32 arg1, s32 arg2) {
    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    return gfx;
}
