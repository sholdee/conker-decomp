#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    u32 unk0;
    u16 unk4;
    u16 unk6;
    u8 unk8;
    u8 unk9;
    u8 unkA;
} TexDesc_150D1810;

extern s32 D_800DBE80;
extern Vtx *D_800DBE88[];
extern Mtx *D_800DBEB0;
extern Mtx D_80089470;
Gfx *func_1501A680(Gfx *arg0);
Gfx *func_150950D4(Gfx *, TexDesc_150D1810 *, s32, s32, s32, s32, s32, s32, s32, s32);
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);
void func_151102CC(f32 a0[4][4], f32 a1, f32 a2, f32 a3);
void guMtxF2L(f32 mf[4][4], Mtx *m);

Gfx *func_150D1810(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5, f32 arg6) {
    f32 mtx[4][4];
    f32 mf[4][4];
    s32 a;
    s32 b;
    s32 tc;
    Vtx *vtx;
    TexDesc_150D1810 tex;

    tc = (s32)(arg4 * 81.0f);
    if ((D_800BE9F0 == 0x33) || (D_800BE9F0 == 0x32) || (D_800BE9F0 == 0x3F)) {
        a = -0x62;
        b = 0x27;
    } else {
        a = -0x13;
        b = 0x65;
    }
    tex.unk0 = D_800DBE80;
    tex.unk4 = 4;
    tex.unk6 = 0x100;
    tex.unk8 = 0;
    tex.unk9 = 3;
    tex.unkA = 0;
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++, 0x082C0F, 0x0F0A4004);
    { Gfx *g = gfx++; g->words.w0 = 0xFCFFFFFF; g->words.w1 = 0xFFFCF279; }
    gfx = (Gfx *)func_1501A490((s32)gfx, *(s16 *)((u8 *)&D_80082FA4 + 2), 0, 0, 0, 0);
    gfx = func_1501A680(gfx);
    gfx = func_150950D4(gfx, &tex, 0, 0, 0, 0, 2, 0x100, 0x100, 3);
    func_151102CC(mtx, 0.0f, 0.0f, arg6);
    func_150A7A48(mtx, (f32 (*)[4])((u8 *)D_800BE628 + D_80082FA4 * 0x180 + 0xBC), mf);
    guMtxF2L(mf, &D_800DBEB0[D_80082FA4 * 2 + D_800BE9C0]);
    gSPMatrix(gfx++, &D_800DBEB0[D_80082FA4 * 2 + D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gfx++, &D_80089470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPPerspNormalize(gfx++, *(u16 *)((u8 *)D_800BE628 + D_80082FA4 * 0x180 + 0xB8));
    vtx = ((Vtx *(*)[2])D_800DBE88)[D_80082FA4][D_800BE9C0];
    vtx[0].v.tc[1] = vtx[1].v.tc[1] = ((a + 0x100) << 5) + tc;
    vtx[2].v.tc[1] = vtx[3].v.tc[1] = ((b + 0x1FF) << 5) + tc;
    gSPVertex(gfx++, vtx, 4, 0);
    gSP2Triangles(gfx++, 0, 1, 2, 0, 0, 2, 3, 0);
    return gfx;
}
