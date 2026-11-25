#include <ultra64.h>
#define func_1502B7F0 func_1502B7F0__proto
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_1511FC2C func_1511FC2C__proto
#define func_1511FC44 func_1511FC44__proto
#include "functions.h"
#undef func_1502B7F0
#undef func_150ADA20
#undef func_1511FC2C
#undef func_1511FC44
#include "variables.h"

u32 func_150ADA20(void);

extern u8 D_800DBEA8[];
Gfx *func_15110544(Gfx *gfx, s32 a, s32 b, s32 c, s32 d, u8 r, u8 g, u8 bl);
Gfx *func_1501A6CC(Gfx *arg0, s32 a, s32 b, s32 c, s32 d);

/* One on-screen view (a player's region of the framebuffer); D_800BE628 points at an array of these.
 * Same layout as game_476D0.c's RenderView, with the projection fields this file uses. */
typedef struct RenderView {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 onScreen;
    /* 0x002 */ u8 pad2[2];
    /* 0x004 */ f32 width;
    /* 0x008 */ f32 height;
    /* 0x00C */ f32 halfWidth;
    /* 0x010 */ f32 halfHeight;
    /* 0x014 */ f32 unk14;
    /* 0x018 */ f32 unk18;
    /* 0x01C */ u8 pad1C[8];
    /* 0x024 */ f32 top;
    /* 0x028 */ f32 bottom;
    /* 0x02C */ f32 left;
    /* 0x030 */ f32 right;
    /* 0x034 */ f32 centerX;
    /* 0x038 */ f32 centerY;
    /* 0x03C */ u8 pad3C[4];
    /* 0x040 */ Vp viewports[2];
    /* 0x060 */ f32 aspect;
    /* 0x064 */ f32 unk64;
    /* 0x068 */ f32 unk68;
    /* 0x06C */ f32 fovX;
    /* 0x070 */ f32 fovY;
    /* 0x074 */ f32 frustumFovX;
    /* 0x078 */ f32 frustumFovY;
    /* 0x07C */ f32 pixelTanX;
    /* 0x080 */ f32 pixelTanY;
    /* 0x084 */ f32 unk84;
    /* 0x088 */ f32 frustumPlanes[4][3];
    /* 0x0B8 */ u16 perspNorm;
    /* 0x0BA */ u8 padBA[2];
    /* 0x0BC */ f32 projection[4][4];
    /* 0x0FC */ u8 padFC[0x84];
} RenderView; // size 0x180

/* variables.h types D_800BE628 as s32. */
#define RENDER_VIEWS ((RenderView *)D_800BE628)

typedef struct Game13D350ViewAngles {
    u8 pad0[0xC];
    f32 pitch;
    f32 yaw;
    f32 roll;
} Game13D350ViewAngles;

typedef Gfx *(*Game13D350ViewDraw)(Gfx *, f32, f32, f32, f32, f32, f32, s32);

Gfx *func_151106A8(Gfx *);
s32 func_151108C4(f32, f32, f32, f32, f32);
extern s16 D_80082FA6;
extern u8 D_800D9B69;
extern u8 D_800D9B6A;
extern u8 D_800D9B6B;
extern u8 D_800D9B6C;
extern u8 D_800D9B6D;
extern u8 D_800D9B6E;
extern u8 D_800D9B6F;
extern u8 D_800D9B70;
extern u8 D_800D9B71;
extern u8 D_800D9B72;
extern u8 D_800D9B73;
extern u8 D_800D9B79;
extern u8 D_800D9B7A;
extern u8 D_800D9B7B;
extern u8 D_800D9B7C;
extern u8 D_800D9B7D;
extern u8 D_800D9B7E;
extern u8 D_800D9B7F;
extern u8 D_800D9B80;
extern u8 D_800D9B81;
extern u8 D_800D9B82;
extern u8 D_800D9B83;
extern Game13D350ViewAngles D_800DCDE0[];
extern Game13D350ViewDraw D_800891D8[];

/* Draws the sky for the current view: clears the depth buffer, then (unless the view is covered) wraps the
 * camera angles into range and calls the current sky's draw routine if func_151108C4 says the sky shows. */
Gfx *func_1510FEA0(Gfx *gfx, s32 arg1) {
    RenderView *view;
    f32 pitch;
    f32 yaw;
    f32 roll;
    s32 visible;
    f32 x;
    f32 y;
    f32 z;

    // matching: each byte is its own symbol (a lui per store), not an index into D_800D9B68/D_800D9B78
    D_800D9B68[0][0] = 0;
    D_800D9B69 = 0;
    D_800D9B6A = 0;
    D_800D9B78[0][0] = 0;
    D_800D9B79 = 0;
    D_800D9B7A = 0;
    D_800D9B6B = 0;
    D_800D9B6C = 0;
    D_800D9B6D = 0;
    D_800D9B7B = 0;
    D_800D9B7C = 0;
    D_800D9B7D = 0;
    D_800D9B6E = 0;
    D_800D9B6F = 0;
    D_800D9B70 = 0;
    D_800D9B7E = 0;
    D_800D9B7F = 0;
    D_800D9B80 = 0;
    D_800D9B71 = 0;
    D_800D9B72 = 0;
    D_800D9B73 = 0;
    D_800D9B81 = 0;
    D_800D9B82 = 0;
    D_800D9B83 = 0;
    gSPClearGeometryMode(gfx++, G_CULL_BOTH);
    gfx = func_151106A8(gfx);
    gDPPipeSync(gfx++);
    if (D_800BE635 != 0) {
        gfx = func_15110544(gfx, 0, 0, D_800BE620, D_800BE624, 0, 0, 0);
    }
    if (D_800C35EA == 1 && D_800C3681 != 0) {
        return gfx;
    }
    // matching: byte offsets into D_800DBFF0[D_80082FA4] (struct108 unk388/unk380/unk2F8..unk300); IDO
    // schedules the struct-member loads differently
    pitch = 360.0f - (*(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x388) + D_800DCDE0[D_80082FA4].pitch);
    while (pitch < -180.0f) {
        pitch += 360.0f;
    }
    while (pitch >= 180.0f) {
        pitch -= 360.0f;
    }
    yaw = 360.0f - (*(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x380) + D_800DCDE0[D_80082FA4].yaw);
    while (yaw < 0.0f) {
        yaw += 360.0f;
    }
    while (yaw >= 360.0f) {
        yaw -= 360.0f;
    }
    // struct108 (structs.h) has no f32 field at 0x5EC yet
    roll = *(f32 *)&D_800DBFF0[D_80082FA4].pad5EC + D_800DCDE0[D_80082FA4].roll;
    while (roll < -180.0f) {
        roll += 360.0f;
    }
    while (roll >= 180.0f) {
        roll -= 360.0f;
    }
    x = *(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x2F8);
    y = *(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x2FC);
    z = *(f32 *)((u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0 + 0x300);
    if (D_800C3682 != 0) {
        view = &RENDER_VIEWS[D_80082FA4];
        return func_15110544(gfx, view->left, view->top, view->right - 1.0f, view->bottom, 0x16, 0x1C, 0x1B);
    }
    visible = func_151108C4(x, y, z, pitch, yaw);
    if (visible != 0) {
        gfx = D_800891D8[D_800B0DF0->unk8](gfx, x, y, z, pitch, yaw, roll, visible);
    }
    gfx = (Gfx *)func_1501A490((s32)gfx, D_80082FA6, 0, 0, 0, 0);
    gSPSetGeometryMode(gfx++, G_CULL_BACK);
    return gfx;
}

void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);

/* Builds the camera rotation matrix in mtx: yaw, then pitch, then roll (func_150A8050 makes a rotation from
 * three angles, func_150A7A48 multiplies). */
void func_151102CC(f32 mtx[4][4], f32 pitch, f32 yaw, f32 roll) {
    f32 rotation[4][4];

    func_150A8050(mtx, 0.0f, yaw, 0.0f);
    func_150A8050(rotation, pitch, 0.0f, 0.0f);
    func_150A7A48(mtx, rotation, mtx);
    func_150A8050(rotation, 0.0f, 0.0f, roll);
    func_150A7A48(mtx, rotation, mtx);
}

/* Builds view viewIndex's view-projection matrix in mtx from the camera angles. */
void func_15110360(s32 viewIndex, f32 mtx[4][4], f32 pitch, f32 yaw, f32 roll) {
    func_151102CC(mtx, pitch, yaw, roll);
    func_150A7A48(mtx, RENDER_VIEWS[viewIndex].projection, mtx);
}

/* Fills the current view with the background colour D_800DBEA8 and, on view 0 in split screen, draws the
 * black divider bars (only the horizontal one when D_80082FA0 is 1). */
Gfx *func_151103C8(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3) {
    RenderView *view;

    view = &RENDER_VIEWS[D_80082FA4];
    gfx = func_15110544(gfx, view->left, view->top, view->right - 1.0f, view->bottom,
                        D_800DBEA8[0], D_800DBEA8[1], D_800DBEA8[2]);
    if (D_80082FA0 != 0) {
        if (D_80082FA4 == 0) {
            gDPPipeSync(gfx++);
            gfx = (Gfx *)func_1501A490((s32)gfx, 0xFF, 0, 0, 0, 0);
            gDPSetFillColor(gfx++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));
            gfx = func_1501A6CC(gfx, 0, (D_800BE624 >> 1) - 6, D_800BE620, (D_800BE624 >> 1) + 6);
            if (D_80082FA0 != 1) {
                gfx = func_1501A6CC(gfx, (D_800BE620 >> 1) - 1, 0, (D_800BE620 >> 1) + 1, D_800BE624);
            }
        }
    }
    return gfx;
}

/* Fills the rectangle (ulx, uly)-(lrx, lry) of the framebuffer with colour (r, g, b) in fill mode. */
Gfx *func_15110544(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry, u8 r, u8 g, u8 b) {
    s32 color;

    gDPPipeSync(gfx++);
    gDPSetOtherMode(gfx++, 0x302C0F, 4);
    gfx = func_1501A680(gfx);
    color = GPACK_RGBA5551(r, g, b, 1);
    gDPSetFillColor(gfx++, (color << 16) | color);
    return func_1501A6CC(gfx, ulx, uly, lrx, lry);
}

/* On maps 0x1B and 0x1E (and 0x31 while camera 0's unk3D4 has state 3) fills the whole drawable area with the
 * background colour D_800DBEA8. */
Gfx *func_15110600(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3) {
    // unk78 is the high byte of struct127's u16 unk78
    if (D_800BE9F0 == 0x1B || D_800BE9F0 == 0x1E ||
        (D_800BE9F0 == 0x31 && *(u8 *)&D_800DBFF0->unk3D4->unk78 == 3)) {
        gfx = func_15110544(gfx, 2, 0, D_800BE620 - 2, D_800BE624,
                            D_800DBEA8[0], D_800DBEA8[1], D_800DBEA8[2]);
    }
    return gfx;
}

extern u8 D_800BEAC2;

/* On view 0, clears the depth buffer (fills it with the maximum depth) and switches back to the framebuffer;
 * in split screen also draws the black divider bars, and blacks out the whole screen while D_800BEAC2 is set. */
Gfx *func_151106A8(Gfx *gfx) {
    if (D_80082FA4 == 0) {
        gDPPipeSync(gfx++);
        gDPSetOtherMode(gfx++, 0x302C0F, 4);
        gDPSetCombineMode(gfx++, G_CC_SHADE, G_CC_SHADE);
        gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_800BE620, D_800BE9C4);
        gDPSetFillColor(gfx++, (GPACK_ZDZ(G_MAXFBZ, 0) << 16) | GPACK_ZDZ(G_MAXFBZ, 0));
        gfx = (Gfx *)func_1501A490((s32)gfx, 0xFF, 0, 0, 0, 0);
        gDPFillRectangle(gfx++, 2, 0, D_800BE620 - 2, D_800BE624);
        gDPPipeSync(gfx++);
        gfx = func_1501A680(gfx);
        if (D_80082FA0 != 0) {
            gDPPipeSync(gfx++);
            gfx = (Gfx *)func_1501A490((s32)gfx, 0xFF, 0, 0, 0, 0);
            gDPSetFillColor(gfx++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));
            gfx = func_1501A6CC(gfx, 0, (D_800BE624 >> 1) - 6, D_800BE620, (D_800BE624 >> 1) + 6);
            if (D_80082FA0 != 1) {
                gfx = func_1501A6CC(gfx, (D_800BE620 >> 1) - 1, 0, (D_800BE620 >> 1) + 1, D_800BE624);
            }
            if (D_800BEAC2 != 0) {
                gfx = func_1501A6CC(gfx, 0, 0, D_800BE620, D_800BE624);
            }
        }
    }
    return gfx;
}

extern u8 D_800C35E8;

/* Returns how much of the sky shows for the camera at (x, y, z) looking at (pitch, yaw) on the current map
 * D_800BE9F0: 0 hides it, 320 (a full screen height) draws all of it; func_15110CFC turns it into a row count. */
s32 func_151108C4(f32 x, f32 y, f32 z, f32 pitch, f32 yaw) {
    s32 ret;
    s32 byHeight;

    switch (D_800BE9F0) {
    case 0x6:
    case 0x3B:
        ret = y * 40.0f / 1800.0f + 200.0f;
        break;
    case 0x14:
        ret = y * 22.0f / 1800.0f + 113.0f;
        break;
    case 0xC:
        ret = y * 90.0f / 1800.0f + 150.0f;
        break;
    case 0x0:
        if (y < 1600.0f && pitch > 26.0f) {
            ret = 0;
        } else {
            ret = y * 51.0f / 1800.0f + 134.0f;
        }
        break;
    case 0x7:
        if (z < -1100.0f) {
            ret = 320;
        } else if (pitch > 20.0f) {
            ret = 0;
        } else {
            ret = y * 54.0f / 1800.0f + 167.0f;
        }
        break;
    case 0x41:
        ret = 137;
        break;
    case 0x3C:
        if (z < -1650.0f || (x > 3495.0f && x < 4578.0f && z < 2960.0f)) {
            ret = 320;
        } else {
            ret = 0;
        }
        break;
    case 0xB:
        byHeight = FALSE;
        if (func_150A29C8(0, 0x18) == 1) {
            if (func_150A29C8(0, 0x2E) == 0) {
                if (pitch > 0.0f) {
                    ret = 0;
                } else {
                    byHeight = TRUE;
                }
            } else {
                ret = 0;
            }
        } else {
            byHeight = TRUE;
        }
        if (byHeight) {
            if (y < 1218.0f) {
                ret = 30;
            } else {
                s32 lim = 320; // matching: IDO loads the 320 before the y < 2000 test only from a local

                if (y < 2000.0f) {
                    ret = 160;
                } else {
                    ret = lim;
                }
            }
        }
        break;
    case 0x13:
        if (D_800C35EA == 1 && (D_800C35E8 == 2 || D_800C35E8 == 1)) {
            ret = 320;
        } else if (y < 1900.0f) {
            ret = 270;
        } else {
            ret = 320;
        }
        break;
    case 0x1C:
        ret = y * 34.0f / 1800.0f + 130.0f;
        break;
    case 0xE:
        ret = 165;
        break;
    case 0x39:
        ret = 84;
        break;
    case 0x2D:
        ret = y * 53.0f / 1800.0f + 139.0f;
        break;
    case 0x30:
        ret = y * 53.0f / 1800.0f + 139.0f;
        break;
    default:
        ret = 320;
        break;
    }
    return ret;
}

typedef struct {
    s32 tex;
    s32 idx;
} TexEntry15110CFC;

extern s32 func_150A6210(s32, f32, f32, f32, f32, f32, f32);
extern s32 func_1510D0EC(s32, s32, s32, s32);
extern u8 D_1673[];
extern u8 D_800DBEAB;
extern u8 D_800DBEAC;
extern s32 *D_800DBE80;

Gfx *func_15110CFC(Gfx *gfx, Vtx **vtxBufPtr, s32 texBase, s32 arg3, s32 arg4, u32 width, u32 height,
                   u32 tileW, u32 tileH, f32 x, f32 y, f32 z, u8 axis, f32 (*arg13)[4], s32 limit,
                   s32 *vtxCountPtr, s32 *vtxIdxPtr, TexEntry15110CFC *entries, s32 *lastIdxPtr,
                   u8 *sizes, u8 *shifts, s16 *palOffs, u8 *tlutCounts, u8 *tiles, s32 mode) {
    Vtx *vtxBuf;
    s32 row;
    s32 col;
    s32 u;
    s32 curV;
    s32 startU;
    s32 startV;
    s32 x0;
    s32 y0;
    s32 z0;
    s32 x1;
    s32 y1;
    s32 z1;
    s32 x2;
    s32 y2;
    s32 z2;
    s32 x3;
    s32 y3;
    s32 z3;
    s32 tex;
    s32 texAddr;
    Vtx *v;
    s32 tcS;
    s32 tcT;
    s32 i;
    s32 padE;
    s32 padM[1];
    s32 nCols;
    s32 nRows;
    s32 w8;
    s32 h8;
    s32 colIdx;
    s32 rowIdx;
    s32 idx;
    s32 lastIdx;
    s32 padR;
    s32 vtxCount;
    s32 vtxIdx;

    w8 = tileW * 8;
    h8 = tileH * 8;
    vtxBuf = *vtxBufPtr;
    vtxCount = *vtxCountPtr;
    vtxIdx = *vtxIdxPtr;
    lastIdx = *lastIdxPtr;
    x *= 8.0f;
    y *= 8.0f;
    z *= 8.0f;
    if (axis == 'x') {
        startU = z;
        startV = y;
    } else if (axis == 'y') {
        startU = x;
        startV = z;
    } else if (axis == 'z') {
        startU = x;
        startV = y;
    }
    nRows = height / tileH;
    nCols = width / tileW;
    rowIdx = 0;
    if (mode < 4) {
        if (limit > 320) {
            limit = 320;
        }
        limit = (s32)(limit * nRows + 319) / 320;
    } else {
        limit = 0x7FFF;
    }
    curV = startV - h8;
    for (row = 0; row < height; row += tileH, curV -= h8, rowIdx++) {
        colIdx = 0;
        col = 0;
        u = startU;
        while (col < width) {
            switch (axis) {
                case 'X':
                case 'x':
                    x0 = x1 = x2 = x3 = x;
                    y0 = curV;
                    z0 = u;
                    y1 = curV + h8;
                    z1 = u + w8;
                    y2 = curV;
                    z2 = u + w8;
                    y3 = curV + h8;
                    z3 = u;
                    break;
                case 'Y':
                case 'y':
                    y0 = y1 = y2 = y3 = y;
                    x0 = u;
                    z0 = curV;
                    x1 = u + w8;
                    z1 = curV + h8;
                    x2 = u + w8;
                    z2 = curV;
                    x3 = u;
                    z3 = curV + h8;
                    break;
                case 'Z':
                case 'z':
                    z0 = z1 = z2 = z3 = z;
                    x0 = u;
                    y0 = curV;
                    x1 = u + w8;
                    y1 = curV + h8;
                    x2 = u + w8;
                    y2 = curV;
                    x3 = u;
                    y3 = curV + h8;
                    break;
            }
            if (func_150A6210((s32)arg13, (x0 + x1) >> 1, (y0 + y1) >> 1, (z0 + z1) >> 1, (s32)w8,
                              (s32)h8, 20000.0f) &&
                (D_800DBEAB || rowIdx < limit)) {
                tex = colIdx * nRows + texBase + rowIdx;
                if (D_800DBEAC && D_800B0DF0->unk9 == 1 && mode == 2 && rowIdx >= 4 && rowIdx < 7 &&
                    colIdx >= 2 && colIdx < 4) {
                    tex = (s32)&D_1673[colIdx * 3 + rowIdx - 10];
                }
                texAddr = func_1510D0EC(tex, 0, 3, 0);
                if (!D_800DBEAB || rowIdx < limit) {
                    tcT = (u16)(s32)((tileH + 0.5f) * 32.0f);
                    tcS = (u16)(s32)((tileW + 0.5f) * 32.0f);
                    arg3 = (s32)vtxBuf + vtxIdx * 16 + vtxCount * 16;
                    if (axis == 'y' && (col == 0 || col == width - tileW)) {
                        x0 -= 3;
                        x3 -= 3;
                        x1 += 3;
                        x2 += 3;
                        z0 -= 3;
                        z1 += 3;
                        z2 -= 3;
                        z3 += 3;
                    }
                    ((Vtx *)arg3)[0].v.ob[0] = x0;
                    ((Vtx *)arg3)[0].v.ob[1] = y0;
                    ((Vtx *)arg3)[0].v.ob[2] = z0;
                    ((Vtx *)arg3)[0].v.tc[1] = 16;
                    ((Vtx *)arg3)[0].v.tc[0] = 16;
                    ((Vtx *)arg3)[0].v.flag = 0;
                    ((Vtx *)arg3)[1].v.ob[0] = x2;
                    ((Vtx *)arg3)[1].v.ob[1] = y2;
                    ((Vtx *)arg3)[1].v.ob[2] = z2;
                    ((Vtx *)arg3)[1].v.tc[1] = 16;
                    ((Vtx *)arg3)[1].v.tc[0] = tcS;
                    ((Vtx *)arg3)[1].v.flag = 0;
                    ((Vtx *)arg3)[2].v.ob[0] = x1;
                    ((Vtx *)arg3)[2].v.ob[1] = y1;
                    ((Vtx *)arg3)[2].v.ob[2] = z1;
                    ((Vtx *)arg3)[2].v.tc[1] = tcT;
                    ((Vtx *)arg3)[2].v.tc[0] = tcS;
                    ((Vtx *)arg3)[2].v.flag = 0;
                    ((Vtx *)arg3)[3].v.ob[0] = x3;
                    ((Vtx *)arg3)[3].v.ob[1] = y3;
                    ((Vtx *)arg3)[3].v.ob[2] = z3;
                    ((Vtx *)arg3)[3].v.tc[1] = tcT;
                    ((Vtx *)arg3)[3].v.tc[0] = 16;
                    ((Vtx *)arg3)[3].v.flag = 0;
                    nCols = width / tileW;
                    entries[vtxCount >> 2].tex = texAddr;
                    entries[vtxCount >> 2].idx =
                        *((colIdx * nRows + rowIdx + mode * nCols * nRows) + (u8 *)D_800DBE80);
                    vtxCount += 4;
                    if (vtxCount == 16) {
                        s32 padB[2];
                        gSPVertex(gfx++, &vtxBuf[vtxIdx], 16, 0);
                        for (i = 0; i < 16; i += 4) {
                            idx = entries[i >> 2].idx;
                            gDPPipeSync(gfx++);
                            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, entries[i >> 2].tex);
                            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, ((sizes[idx] + 0x800) >> shifts[idx]) - 1, 0);
                            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, entries[i >> 2].tex + palOffs[idx]);
                            gDPLoadSync(gfx++);
                            gDPLoadTLUTCmd(gfx++, G_TX_LOADTILE - 1, tlutCounts[idx]);
                            if (idx != lastIdx) {
                                lastIdx = idx;
                                gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, tiles[idx], G_ON);
                            }
                            gSP1Triangle(gfx++, i + 2, i, i + 1, 0);
                            gSP1Triangle(gfx++, i + 3, i, i + 2, 0);
                        }
                        vtxCount = 0;
                        vtxIdx += 16;
                    }
                } else {
                }
            } else {
            }
            col += tileW;
            colIdx++;
            u += w8;
        }
    }
    *vtxCountPtr = vtxCount;
    *vtxIdxPtr = vtxIdx;
    *vtxBufPtr = vtxBuf;
    return gfx;
}

extern s32 *D_800DBE80;
extern s32 *D_800891BC[];
s32 func_1502B7F0();
void func_10004074();
void func_1510D694(s32);
void func_15111858(void);

void func_1511172C(s32 arg0) {
    s32 i;
    struct104 *var_v0;
    struct104 * volatile *var_s3;
    s32 **var_s0;

    var_s3 = (struct104 * volatile *)&D_800B0DF0;
    if (arg0 == 1) {
        var_s3 = (struct104 * volatile *)&D_800B0DF0;
        (*var_s3)->unk8 = 1;
        var_s0 = &D_800DBE80;
        if (D_800DBE80 != 0) {
            s32 *temp_v0;

            temp_v0 = D_800DBE80;
            func_10004074(temp_v0);
        }
        func_15111858();
        return;
    }

    var_v0 = *var_s3;
    if (arg0 != var_v0->unk9) {
        if (var_v0->unk8 == 4) {
            if (D_80038080 != 0) {
                for (i = 0; i != 0x168; i++) {
                    func_1510D694(D_800891BC[(*var_s3)->unk9][0] + i);
                }
            }
            var_s0 = &D_800DBE80;
            func_10004074(*var_s0);
            (*var_s3)->unk9 = arg0;
            func_1502B7F0(var_s0, 2, 0xD, (*var_s3)->unk9);
        }
    }
}

extern void *allocate_memory(s32, s32, s32, s32);

typedef struct Star15111858 {
    u16 x;
    u16 y;
    u16 z;
    u8 unk6;
    u8 unk7;
} Star15111858;

void func_15111858(void) {
    Star15111858 *p;
    s32 i;

    D_800DBE80 = allocate_memory(0xFA00, 1, 0, 0);
    p = (Star15111858 *)D_800DBE80;
    for (i = 0; i != 8000; i++) {
        f32 r;
        f32 ang;
        f32 v;
        f32 lat;
        s32 h;
        f32 tmp;

        ang = (f32)(func_150ADA20() % 36000U) * 0.0001745329209f;
        v = (f32)(s32)((func_150ADA20() % 1584400U) - 792100) * 0.009999999776f;
        if (v >= 0.0f) {
            lat = 89.0f - sqrtf(v);
        } else {
            lat = sqrtf(-v) + -89.0f;
        }
        lat *= 0.01745329238f;
        h = func_150AD78C(lat) * 256.0f;
        tmp = h;
        p[i].y = *(u16 *)&tmp;
        r = sqrtf(0x10000 - h * h);
        tmp = (s32)(func_150AD78C(ang) * r);
        p[i].x = *(u16 *)&tmp;
        tmp = (s32)(func_150AD780(ang) * r);
        p[i].z = *(u16 *)&tmp;
        p[i].unk7 = (func_150ADA20() % 191U) + 0x40;
        p[i].unk6 = func_150ADA20() % 5U;
    }
}

typedef struct Game13D350Face {
    u16 unk0;
    u16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
} Game13D350Face;

typedef struct Game13D350Tex {
    s32 addr;
    s32 idx;
} Game13D350Tex;

typedef struct Game13D350Color {
    u8 r;
    u8 g;
    u8 b;
} Game13D350Color;

struct Game13D350U8x2 { u8 v[2]; };
struct Game13D350S16x2 { s16 v[2]; };
extern struct Game13D350U8x2 D_800891EC;
extern struct Game13D350U8x2 D_800891F0;
extern struct Game13D350S16x2 D_800891F4;
extern struct Game13D350U8x2 D_800891F8;
extern struct Game13D350U8x2 D_800891FC;
extern Game13D350Face D_800A2DA0[];
extern Mtx *D_800DBEB0;
extern s32 D_800DBE88[][2];
extern u8 D_80089470;


Gfx *func_15111AF4(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, f32 pitch, f32 yaw, f32 roll, s32 dist);
/* Last entry of the D_800891D8 draw table (still asm data). The original file defined that table
 * before this function; the `.word func_15111AF4` it emits makes as1 treat the function's entry
 * label as address-taken, so the switch's `jr` gets an edge back to it. That turns everything up
 * to the switch into a "loop" for as1's cross-block scheduler (candidate limit 100 instead of
 * 10), which is what hoists `lwc1 yaw` above the 0x41 test and the else-arm `move v0,s0` into the
 * colour branch's delay slot. Without this word the function cannot match. */
static void *D_800891E8[] = { func_15111AF4 };

Gfx *func_15111AF4(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, f32 pitch, f32 yaw, f32 roll, s32 dist) {
    s32 i;
    s32 end;
    s32 start;
    s32 j;
    s32 fmt;
    s32 idx;
    s32 *texList;
    f32 mtx[4][4];
    f32 mtx2[4][4];
    s32 vtx;
    s32 count;
    s32 vtxBase;
    Game13D350Tex texs[4];
    Gfx *t;
    s32 lastIdx;
    f32 fov;
    u8 color[3];
    struct Game13D350U8x2 sp104;
    struct Game13D350U8x2 sp100;
    struct Game13D350S16x2 spFC;
    struct Game13D350U8x2 spF8;
    struct Game13D350U8x2 spF4;

    sp104 = D_800891EC;
    sp100 = D_800891F0;
    spFC = D_800891F4;
    spF8 = D_800891F8;
    spF4 = D_800891FC;
    { Gfx *command = gfx++; command->words.w0 = 0xD7000002; command->words.w1 = 0xFFFFFFFF; }
    gfx = func_1501A680((Gfx *)func_1501A490((s32)gfx, D_80082FA6, 0, 0, 0, 0));
    fov = (D_80082FA0 != 1) ? 80.0f : 95.0f;
    if (D_800BE9F0 == 0x41) {
        yaw -= 119.0f;
        if (yaw < 0.0f) {
            yaw += 360.0f;
        }
    } else if (D_800BE9F0 == 0x3C ||
               (D_800BE9F0 == 0x13 && D_800C35EA == 1 && (D_800C35E8 == 2 || D_800C35E8 == 1))) {
        gfx = func_15110544(gfx, RENDER_VIEWS[D_80082FA4].left, RENDER_VIEWS[D_80082FA4].top,
                            RENDER_VIEWS[D_80082FA4].right - 1.0f, RENDER_VIEWS[D_80082FA4].bottom, 0, 0, 0);
    }
    color[0] = ((u8 *)D_800B0DF0)[5];
    color[1] = ((u8 *)D_800B0DF0)[6];
    color[2] = ((u8 *)D_800B0DF0)[7];
    { Gfx *command = gfx++; command->words.w0 = 0xE7000000; command->words.w1 = 0; }
    if (color[0] + color[1] + color[2] == 0x2FD) {
        { Gfx *command = gfx++; command->words.w0 = 0xFCFFFFFF; command->words.w1 = 0xFFFCF279; }
    } else {
        gDPSetEnvColor(gfx++, color[0], color[1], color[2], 0xFF);
        { Gfx *command = gfx++; command->words.w0 = 0xFC129A25; command->words.w1 = 0xFF37FFFF; }
    }
    { Gfx *command = gfx++; command->words.w0 = 0xEF08AC0F; command->words.w1 = 0x0F0A4004; }
    { Gfx *command = gfx++; command->words.w0 = 0xF5500000; command->words.w1 = 0x07000000; }
    { Gfx *command = gfx++; command->words.w0 = 0xF5481000; command->words.w1 = 0x00080200; }
    { Gfx *command = gfx++; command->words.w0 = 0xF2000000; command->words.w1 = 0x000F407C; }
    { Gfx *command = gfx++; command->words.w0 = 0xF5400800; command->words.w1 = 0x01080200; }
    { Gfx *command = gfx++; command->words.w0 = 0xF2000000; command->words.w1 = 0x010F407C; }
    { Gfx *command = gfx++; command->words.w0 = 0xF5000100; command->words.w1 = 0x06000000; }
    func_151102CC(mtx, pitch, yaw, roll);
    func_150A7A48(mtx, RENDER_VIEWS[D_80082FA4].projection, mtx2);
    guMtxF2L(mtx2, &D_800DBEB0[D_80082FA4 * 2 + D_800BE9C0]);
    { Gfx *command = gfx++; command->words.w0 = 0xDA380007; command->words.w1 = (u32)&D_800DBEB0[D_80082FA4 * 2 + D_800BE9C0]; }
    { Gfx *command = gfx++; command->words.w0 = 0xDA380003; command->words.w1 = (u32)&D_80089470; }
    { Gfx *command = gfx++; command->words.w0 = 0xDB0E0000; command->words.w1 = RENDER_VIEWS[D_80082FA4].perspNorm; }
    texList = D_800891BC[D_800B0DF0->unk9];
    count = 0;
    vtx = D_800DBE88[D_80082FA4][D_800BE9C0];
    vtxBase = 0;
    lastIdx = -1;
    if (dist < 0) {
        dist = 0;
    }
    end = (D_800BE9F0 == 0xC) ? 6 : 5;
    start = (D_800BE9F0 != 0x19) ? 0 : 4;
    for (i = start; i < end; i++) {
        switch (i) {
            case 0:
                if ((yaw <= 180.0f && fov < yaw) || (180.0f < yaw && yaw < 360.0f - fov)) {
                    continue;
                }
                break;
            case 1:
                if (270.0f + fov < yaw || yaw < 270.0f - fov) {
                    continue;
                }
                break;
            case 2:
                if (180.0f + fov < yaw || yaw < 180.0f - fov) {
                    continue;
                }
                break;
            case 3:
                if (90.0f + fov < yaw || yaw < 90.0f - fov) {
                    continue;
                }
                break;
            case 4:
                if (-90.0f + fov < pitch || pitch < -90.0f - fov) {
                    continue;
                }
                break;
            case 5:
                if (90.0f + fov < pitch || pitch < 90.0f - fov) {
                    continue;
                }
                if (dist < 300) {
                    continue;
                }
                break;
        }
        if (D_800A2DA0[i].unk4 == D_800A2DA0[i].unkA) {
            fmt = 0x78;
        } else if (D_800A2DA0[i].unk6 == D_800A2DA0[i].unkC) {
            fmt = 0x79;
        } else {
            fmt = 0x7A;
        }
        idx = (i == 5) ? 0 : i;
        gfx = func_15110CFC(gfx, (Vtx **)&vtx, texList[idx], 0, 2, D_800A2DA0[i].unk0, D_800A2DA0[i].unk2, 0x3C, 0x1E,
                            D_800A2DA0[i].unk4, D_800A2DA0[i].unk6, D_800A2DA0[i].unk8, fmt, mtx, dist, &count, &vtxBase,
                            (TexEntry15110CFC *)texs, &lastIdx, sp104.v, sp100.v, spFC.v, spF8.v, spF4.v, idx);
    }
    if (count != 0) {
        gSPVertex(gfx++, vtxBase * 16 + vtx, count, 0);
        for (j = 0; j < count; j += 4) {
            idx = texs[j >> 2].idx;
            gDPPipeSync(gfx++);
            { Gfx *command = gfx++; command->words.w0 = 0xFD100000; command->words.w1 = texs[j >> 2].addr; }
            { Gfx *command = gfx++; command->words.w0 = 0xF3000000; command->words.w1 = 0x077FF000; }
            { Gfx *command = gfx++; command->words.w0 = 0xFD100000; command->words.w1 = texs[j >> 2].addr + spFC.v[idx]; }
            gDPLoadSync(gfx++);
            gDPLoadTLUTCmd(gfx++, 6, spF8.v[idx]);
            if (idx != lastIdx) {
                lastIdx = idx;
                gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, spF4.v[idx], G_ON);
            }
            gSP1Triangle(gfx++, j + 2, j, j + 1, 0);
            gSP1Triangle(gfx++, j + 3, j, j + 2, 0);
        }
    }
    return gfx;
}
