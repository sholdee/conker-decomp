#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* D_800C3FC4 (declared as one s32): a smoothed position per actor, as s16 x, y, z. */
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
    frame = actor->unk2D0->frame;
    if (state == 0x3E || state == 0x41 || (state == 0x138 && frame >= 36.0f) ||
        ((state == 0x3D || state == 0xD9) && frame > 21.0f)) {
        func_150A7960(pinMtx, -7.0f, -17.0f, -7.0f, &x, &y, &z);
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
        func_150A7960(followMtx, x, y, z, &targetX, &targetY, &targetZ);
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

extern Gfx D_80084350[];
extern u8 D_800903BC[];

/*
 * Draws two triangles from actor `index`'s 5-vertex buffer (only actor 0, and only for models of type 2
 * or 0x13 that carry attachment set 8). The vertices take their color from that attachment's vertex 48;
 * vertex 0 is placed by func_15036570 and drawn with D_80089470, vertices 1-4 with bone matrix 4.
 */
Gfx *func_150368C4(Gfx *gfx, s32 index, s32 view) {
    MtxF *bones;
    ModelAttachment *attachment;
    Vtx *color;
    s32 i;

    if (movement_obj[index].mv->context_b != 2 && movement_obj[index].mv->context_b != 0x13) {
        return gfx;
    }
    attachment = func_1503195C(&movement_obj[index], 8, 0);
    if (attachment == NULL) {
        return gfx;
    }
    if (index > 0) {
        return gfx;
    }
    color = &((Vtx *)attachment->vtx[D_800BE9C0])[48];
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
    gfx = func_1515E544(gfx, movement_obj[index].unk304[view], movement_obj[index].unk301,
                        movement_obj[index].unk302, movement_obj[index].unk314[D_800BE9C0]);
    if ((movement_obj[index].unk66 & 0xC) == 0xC) {
        gSPClearGeometryMode(gfx++, G_LIGHTING);
    } else if ((movement_obj[index].unk66 & 0xC) != 8 && !(attachment->flags & 1)) {
        gSPClearGeometryMode(gfx++, G_LIGHTING);
    } else if (D_800DCD7C != 0) {
        gSPSetGeometryMode(gfx++, G_LIGHTING);
    } else {
        gSPClearGeometryMode(gfx++, G_LIGHTING);
    }
    gfx = func_15094F70(gfx, D_800903BC, 0, NULL, 0, 0, 0, 2, 3);
    gSPMatrix(gfx++, &D_80089470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(gfx++, &D_800C3FC0[index][0], 1, 0);
    gSPMatrix(gfx++, &bones[4], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(gfx++, &D_800C3FC0[index][1], 4, 1);
    gSP1Triangle(gfx++, 0, 1, 2, 0);
    gSP1Triangle(gfx++, 0, 3, 4, 0);
    return gfx;
}
