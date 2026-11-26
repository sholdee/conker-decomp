#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* The three quads of a puff, drawn on each quad's matrix (func_15166D68). */
static Vtx D_8008B3E0[] = {
    {{{ -400, -34, -40 }, 0, { 0, 0 }, { 0, 0, 0, 0 }}},
    {{{ -400, -34,  40 }, 0, { 0, 0 }, { 0, 0, 0, 0 }}},
    {{{ -400,  34,   0 }, 0, { 0, 0 }, { 0, 0, 0, 0 }}},
    {{{    0, -34, -40 }, 0, { 0, 0 }, { 0, 0, 0, 0 }}},
    {{{    0, -34,  40 }, 0, { 0, 0 }, { 0, 0, 0, 0 }}},
    {{{    0,  34,   0 }, 0, { 0, 0 }, { 0, 0, 0, 0 }}},
};

/* Render setup of object type 0xD (its entry in the object type table D_8008B4A8). */
Gfx D_8008B440[] = {
    gsSPSetGeometryMode(G_CULL_BACK),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsDPSetBlendColor(0, 0, 0, 1),
    gsDPSetOtherMode(0x082CAF, 0x00504B50),
    gsSPEndDisplayList(),
};

/* Spawns a puff at (x, y, z); its quads are yawed 90-153 degrees apart from a random start. */
void func_151669A0(s32 x, s32 y, s32 z, f32 scale, u8 arg4, s32 arg5) {
    PuffQuadEffect *obj;
    s32 angle;
    s32 i;
    f32 fx;
    f32 fy;
    f32 fz;

    obj = func_15167A68(0xD, arg5, sizeof(PuffQuadEffect), 1, arg4, 1);
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
        sprite_setDisturbPoint(x, y, z);
    }
}


/* Per-frame update: halfway through, spawns a particle from each quad; frees the object when done. */
void func_15166B50(void *self) {
    MtxF mf;
    f32 x;
    f32 y;
    f32 z;
    PuffQuadEffect *obj;
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
            func_15167D84(&spawn, 0, 0, -1, obj->header.owner, obj->header.kind);
        }
    } else if (obj->timer == 0) {
        func_1516972C(&obj->header);
    }
}

/* Draws the three quads; their texture scrolls in S as the timer runs down. */
Gfx *func_15166D68(Gfx *gfx, PuffQuadEffect *obj, s32 arg2) {
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


/* Loads texture D_8009054C, using the frame and mode already in D_800DD220/D_800DD224. */
Gfx *func_15166F6C(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3) {
    D_800DD228 = &D_8009054C;
    return sprite_loadDescFrame(gfx, D_800DD228, D_800DD220, &D_800DD230, 0, 0, 0, D_800DD224, 3);
}


/* Loads D_80089470 as the modelview matrix. */
Gfx *func_15166FD8(Gfx *gfx, s32 arg1, s32 arg2) {
    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    return gfx;
}
