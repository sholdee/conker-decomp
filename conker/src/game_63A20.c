#include <ultra64.h>

// This file's .bss, defined before functions.h and variables.h declare the globals: IDO lays out .bss in
// the order it first sees each name, and that order also depends on how many types and variables are
// declared before it (see init/Sched/SchedInit/schedinit.c).
Vtx *D_800C3FC0[1];          // per actor: its 5-vertex quad buffer; [0] is D_80084300
static s16 D_800C3FC4[1][3]; // per actor (only actor 0 draws a quad): the smoothed position of quad vertex 0

#include "functions.h"
#include "variables.h"

/* Per actor: the 5 quad vertices. Actor 0 uses the template D_80084300; game_32200 copies it for the others. */
Vtx D_80084300[5] = {
    { { { -7, -17, -7 }, 0, { 0x2000, 0x2100 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -14, -19, 57 }, 0, { 0x2400, 0x2000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { -21, -21, 60 }, 0, { 0x2400, 0x2200 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 26, -7, 60 }, 0, { 0x2400, 0x2000 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 14, -5, 57 }, 0, { 0x2400, 0x2200 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

/* Render state for the quad: textured, shaded, no lighting or culling. */
static Gfx D_80084350[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_LOD | G_CULL_BOTH),
    gsDPSetCombineMode(G_CC_MODULATEI, G_CC_MODULATEI),
    gsDPSetOtherMode(0x082C3F, 0x552230),
    gsSPEndDisplayList(),
};

// matching: integer address math hides the base from IDO's alias analysis (s16 (*)[3] indexing reorders loads)
#define QUAD_ANCHOR(index) ((s16 *)((s32)&D_800C3FC4 + (index) * 6))

/*
 * Places vertex 0 of actor `index`'s quad. During some animation states it snaps to a fixed point of
 * pinMtx; otherwise it eases toward the midpoint of vertices 2 and 3 transformed by followMtx.
 */
void func_15036570(s32 index, MtxF pinMtx, MtxF followMtx) {
    f32 x;
    f32 y;
    f32 z;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    Vtx *vtx;
    Actor *actor;
    s32 state;
    f32 frame;
    s16 *anchor;
    f32 blend;
    f32 delta;

    vtx = D_800C3FC0[index];
    actor = &movement_obj[index];
    state = actor->animseq.uh;
    frame = actor->unk2D0->frame[0];
    if (state == 0x3E || state == 0x41 || (state == 0x138 && frame >= 36.0f) ||
        ((state == 0x3D || state == 0xD9) && frame > 21.0f)) {
        math_transformPoint(pinMtx, -7.0f, -17.0f, -7.0f, &x, &y, &z);
        anchor = QUAD_ANCHOR(index);
        anchor[0] = x;
        anchor[1] = y;
        anchor[2] = z;
        delta = targetZ - anchor[2]; // matching: dead store; without it IDO assigns the else branch's FP registers differently
    } else {
        if (((state == 0x3D || state == 0xD9) && frame > 7.0f) || state == 0x138 || state == 0x139) {
            blend = 1.0f;
        } else {
            blend = 0.6999999881f;
        }
        x = (vtx[2].v.ob[0] + vtx[3].v.ob[0]) * 0.5f;
        y = (vtx[2].v.ob[1] + vtx[3].v.ob[1]) * 0.5f;
        z = (vtx[2].v.ob[2] + vtx[3].v.ob[2]) * 0.5f;
        math_transformPoint(followMtx, x, y, z, &targetX, &targetY, &targetZ);
        anchor = QUAD_ANCHOR(index);
        delta = targetX - anchor[0];
        anchor[0] += delta * blend;
        delta = targetY - anchor[1];
        anchor[1] += delta * blend;
        delta = targetZ - anchor[2];
        anchor[2] += delta * blend;
        x = anchor[0];
        y = anchor[1];
        z = anchor[2];
    }
    vtx[0].v.ob[0] = x;
    vtx[0].v.ob[1] = y;
    vtx[0].v.ob[2] = z;
}


/*
 * Draws two triangles from actor `index`'s 5-vertex buffer (only actor 0, and only for models of type 2
 * or 0x13 that carry attachment set 8). The vertices take their color from that attachment's vertex 48;
 * vertex 0 is placed by func_15036570 and drawn with gIdentityMtx, vertices 1-4 with bone matrix 4.
 */
Gfx *func_150368C4(Gfx *gfx, s32 index, s32 view) {
    MtxF *bones;
    ModelAttachment *attachment;
    Vtx *color;
    s32 i;

    if (movement_obj[index].mv->context_b != 2 && movement_obj[index].mv->context_b != 0x13) {
        return gfx;
    }
    attachment = attach_find(&movement_obj[index], 8, 0);
    if (attachment == NULL) {
        return gfx;
    }
    if (index > 0) {
        return gfx;
    }
    color = &((Vtx *)attachment->vtx[gDrawBufferIndex])[48];
    for (i = 0; i < 5; i++) {
        D_800C3FC0[index][i].v.cn[0] = color->v.cn[0];
        D_800C3FC0[index][i].v.cn[1] = color->v.cn[1];
        D_800C3FC0[index][i].v.cn[2] = color->v.cn[2];
    }
    bones = movement_obj[index].ob_matrix_ptr;
    if (bones == NULL) {
        return gfx;
    }
    func_15036570(index, bones[9], bones[4]);
    gSPDisplayList(gfx++, D_80084350);
    gfx = light_reemitLights(gfx, movement_obj[index].unk304[view], movement_obj[index].unk301,
                        movement_obj[index].unk302, &movement_obj[index].unk314[gDrawBufferIndex]);
    if ((movement_obj[index].unk66 & 0xC) == 0xC) {
        gSPClearGeometryMode(gfx++, G_LIGHTING);
    } else if ((movement_obj[index].unk66 & 0xC) != 8 && !(attachment->flags & 1)) {
        gSPClearGeometryMode(gfx++, G_LIGHTING);
    } else if (D_800DCD7C != 0) {
        gSPSetGeometryMode(gfx++, G_LIGHTING);
    } else {
        gSPClearGeometryMode(gfx++, G_LIGHTING);
    }
    gfx = sprite_loadDescFrame(gfx, &D_800903BC, 0, NULL, 0, 0, 0, 2, 3);
    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(gfx++, &D_800C3FC0[index][0], 1, 0);
    gSPMatrix(gfx++, &bones[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(gfx++, &D_800C3FC0[index][1], 4, 1);
    gSP1Triangle(gfx++, 0, 1, 2, 0);
    gSP1Triangle(gfx++, 0, 3, 4, 0);
    return gfx;
}
