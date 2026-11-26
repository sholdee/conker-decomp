#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init_50A0.c). */
void *D_800DBE80;      // the current sky data (star field, tile formats or gradient texture)
Vtx *D_800DBE88[4][2]; // per view, per framebuffer
u8 D_800DBEA8[3];      // background colour
u8 D_800DBEAB;
u8 D_800DBEAC;
Mtx *D_800DBEB0;       // sky projection matrices, 2 per view

#include "functions.h"
#include "variables.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define RENDER_VIEWS ((RenderView *)gViews)



/* A sky mode's draw routine (D_800891D8 is indexed by D_800B0DF0->unk8): camera position, angles and the sky
 * extent from func_151108C4. */
typedef Gfx *(*SkyDrawFunc)(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent);

/* The textures of each textured sky set (D_800B0DF0->unk9), five per set. */
static s32 D_80089130[5] = { 0x17E1, 0x1829, 0x1799, 0x1871, 0x18B9 };
static s32 D_80089144[5] = { 0x1679, 0x16C1, 0x162B, 0x1709, 0x1751 };
static s32 D_80089158[5] = { 0x1CCB, 0x1D13, 0x1C83, 0x1D5B, 0x1DA3 };
static s32 D_8008916C[5] = { 0x1949, 0x1991, 0x1901, 0x19D9, 0x1A21 };
static s32 D_80089180[5] = { 0x147B, 0x150B, 0x12CB, 0x159B, 0x15E3 };
static s32 D_80089194[5] = { 0x135B, 0x13A3, 0x1313, 0x13EB, 0x1433 };
static s32 D_800891A8[5] = { 0x1AB1, 0x1AF9, 0x1A69, 0x1B41, 0x1B89 };
s32 *D_800891BC[] = { D_80089130, D_80089144, D_80089158, D_8008916C, D_80089180, D_800891A8, D_80089194 };

/*
 * The draw routine of each sky mode (D_800B0DF0->unk8); most ignore some of the camera arguments.
 * The table comes before func_15111AF4: the address the last entry takes makes as1 treat that function's
 * entry label as address-taken, which its scheduling needs.
 */
static SkyDrawFunc D_800891D8[] = {
    func_151103C8, func_150F9AB0, func_150D1810, func_15110600, func_15111AF4,
};

/* Per-format texture load tables (two formats), wrapped so they can be copied to the stack. */
struct Game13D350U8x2 { u8 v[2]; };
struct Game13D350S16x2 { s16 v[2]; };
static struct Game13D350U8x2 D_800891EC = { { 1, 4 } };          // texel sizes (G_IM_SIZ_*)
static struct Game13D350U8x2 D_800891F0 = { { 1, 2 } };          // load shifts
static struct Game13D350S16x2 D_800891F4 = { { 0x800, 0x400 } }; // palette offsets
static struct Game13D350U8x2 D_800891F8 = { { 255, 15 } };       // TLUT counts - 1
static struct Game13D350U8x2 D_800891FC = { { 0, 1 } };          // render tiles

/* One skybox face: its size and two opposite corners (the axis whose coordinates match is the face's plane). */
typedef struct SkyFace {
    u16 width;
    u16 height;
    s16 x0;
    s16 y0;
    s16 z0;
    s16 x1;
    s16 y1;
    s16 z1;
} SkyFace;

static const SkyFace D_800A2DA0[6] = {
    { 360, 360, -180, 180, -180, 180, -180, -180 },
    { 360, 360, -180, 180, -180, -180, -180, 180 },
    { 360, 360, -180, 180, 180, 180, -180, 180 },
    { 360, 360, 180, 180, -180, 180, -180, 180 },
    { 360, 360, -180, 180, 180, 180, 180, -180 },
    { 360, 360, -180, -180, 180, 180, -180, -180 },
};

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
    s32 i;

    for (i = 0; i < 4; i++) {
        D_800D9B68[i][0] = 0;
        D_800D9B68[i][1] = 0;
        D_800D9B68[i][2] = 0;
        D_800D9B78[i][0] = 0;
        D_800D9B78[i][1] = 0;
        D_800D9B78[i][2] = 0;
    }
    gSPClearGeometryMode(gfx++, G_CULL_BOTH);
    gfx = func_151106A8(gfx);
    gDPPipeSync(gfx++);
    if (D_800BE635 != 0) {
        gfx = func_15110544(gfx, 0, 0, gScreenWidth, gScreenHeight, 0, 0, 0);
    }
    if (D_800C35EA[0] == 1 && D_800C3681 != 0) {
        return gfx;
    }
    pitch = 360.0f - (camera_global_Cameras[gCurrentView].CameraPitch + D_800DCDE0[gCurrentView].rot.x);
    while (pitch < -180.0f) {
        pitch += 360.0f;
    }
    while (pitch >= 180.0f) {
        pitch -= 360.0f;
    }
    yaw = 360.0f - (camera_global_Cameras[gCurrentView].TrueCameraAngle + D_800DCDE0[gCurrentView].rot.y);
    while (yaw < 0.0f) {
        yaw += 360.0f;
    }
    while (yaw >= 360.0f) {
        yaw -= 360.0f;
    }
    roll = camera_global_Cameras[gCurrentView].Roll + D_800DCDE0[gCurrentView].rot.z;
    while (roll < -180.0f) {
        roll += 360.0f;
    }
    while (roll >= 180.0f) {
        roll -= 360.0f;
    }
    x = camera_global_Cameras[gCurrentView].eye.x;
    y = camera_global_Cameras[gCurrentView].eye.y;
    z = camera_global_Cameras[gCurrentView].eye.z;
    if (D_800C3682 != 0) {
        view = &RENDER_VIEWS[gCurrentView];
        return func_15110544(gfx, view->left, view->top, view->right - 1.0f, view->bottom, 0x16, 0x1C, 0x1B);
    }
    visible = func_151108C4(x, y, z, pitch, yaw);
    if (visible != 0) {
        gfx = D_800891D8[D_800B0DF0->unk8](gfx, x, y, z, pitch, yaw, roll, visible);
    }
    gfx = view_setScissor(gfx, gCurrentView, 0, 0, 0, 0);
    gSPSetGeometryMode(gfx++, G_CULL_BACK);
    return gfx;
}

/* Builds the camera rotation matrix in mtx: yaw, then pitch, then roll (func_150A8050 makes a rotation from
 * three angles, func_150A7A48 multiplies). */
void func_151102CC(MtxF mtx, f32 pitch, f32 yaw, f32 roll) {
    MtxF rotation;

    func_150A8050(mtx, 0.0f, yaw, 0.0f);
    func_150A8050(rotation, pitch, 0.0f, 0.0f);
    func_150A7A48(mtx, rotation, mtx);
    func_150A8050(rotation, 0.0f, 0.0f, roll);
    func_150A7A48(mtx, rotation, mtx);
}

/* Builds view viewIndex's view-projection matrix in mtx from the camera angles. */
void func_15110360(s32 viewIndex, MtxF mtx, f32 pitch, f32 yaw, f32 roll) {
    func_151102CC(mtx, pitch, yaw, roll);
    func_150A7A48(mtx, RENDER_VIEWS[viewIndex].projection, mtx);
}

/* Fills the current view with the background colour D_800DBEA8 and, on view 0 in split screen, draws the
 * black divider bars (only the horizontal one when gScreenLayout is 1). */
Gfx *func_151103C8(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent) {
    RenderView *view;

    view = &RENDER_VIEWS[gCurrentView];
    gfx = func_15110544(gfx, view->left, view->top, view->right - 1.0f, view->bottom,
                        D_800DBEA8[0], D_800DBEA8[1], D_800DBEA8[2]);
    if (gScreenLayout != 0) {
        if (gCurrentView == 0) {
            gDPPipeSync(gfx++);
            gfx = view_setScissor(gfx, 0xFF, 0, 0, 0, 0);
            gDPSetFillColor(gfx++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));
            gfx = view_fillRect(gfx, 0, (gScreenHeight >> 1) - 6, gScreenWidth, (gScreenHeight >> 1) + 6);
            if (gScreenLayout != 1) {
                gfx = view_fillRect(gfx, (gScreenWidth >> 1) - 1, 0, (gScreenWidth >> 1) + 1, gScreenHeight);
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
    gfx = view_setColorImage(gfx);
    color = GPACK_RGBA5551(r, g, b, 1);
    gDPSetFillColor(gfx++, (color << 16) | color);
    return view_fillRect(gfx, ulx, uly, lrx, lry);
}

/* On maps 0x1B and 0x1E (and 0x31 while camera 0's unk3D4 has state 3) fills the whole drawable area with the
 * background colour D_800DBEA8. */
Gfx *func_15110600(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent) {
    // unk78 is the high byte of Actor's u16 unk78
    if (level == 0x1B || level == 0x1E ||
        (level == 0x31 && camera_global_Cameras->mv->rt_context_b == 3)) {
        gfx = func_15110544(gfx, 2, 0, gScreenWidth - 2, gScreenHeight,
                            D_800DBEA8[0], D_800DBEA8[1], D_800DBEA8[2]);
    }
    return gfx;
}


/* On view 0, clears the depth buffer (fills it with the maximum depth) and switches back to the framebuffer;
 * in split screen also draws the black divider bars, and blacks out the whole screen while D_800BEAC2 is set. */
Gfx *func_151106A8(Gfx *gfx) {
    if (gCurrentView == 0) {
        gDPPipeSync(gfx++);
        gDPSetOtherMode(gfx++, 0x302C0F, 4);
        gDPSetCombineMode(gfx++, G_CC_SHADE, G_CC_SHADE);
        gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, gScreenWidth, gZBuffer);
        gDPSetFillColor(gfx++, (GPACK_ZDZ(G_MAXFBZ, 0) << 16) | GPACK_ZDZ(G_MAXFBZ, 0));
        gfx = view_setScissor(gfx, 0xFF, 0, 0, 0, 0);
        gDPFillRectangle(gfx++, 2, 0, gScreenWidth - 2, gScreenHeight);
        gDPPipeSync(gfx++);
        gfx = view_setColorImage(gfx);
        if (gScreenLayout != 0) {
            gDPPipeSync(gfx++);
            gfx = view_setScissor(gfx, 0xFF, 0, 0, 0, 0);
            gDPSetFillColor(gfx++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));
            gfx = view_fillRect(gfx, 0, (gScreenHeight >> 1) - 6, gScreenWidth, (gScreenHeight >> 1) + 6);
            if (gScreenLayout != 1) {
                gfx = view_fillRect(gfx, (gScreenWidth >> 1) - 1, 0, (gScreenWidth >> 1) + 1, gScreenHeight);
            }
            if (D_800BEAC2 != 0) {
                gfx = view_fillRect(gfx, 0, 0, gScreenWidth, gScreenHeight);
            }
        }
    }
    return gfx;
}


/* Returns how much of the sky shows for the camera at (x, y, z) looking at (pitch, yaw) on the current map
 * level: 0 hides it, 320 (a full screen height) draws all of it; func_15110CFC turns it into a row count. */
s32 func_151108C4(f32 x, f32 y, f32 z, f32 pitch, f32 yaw) {
    s32 ret;
    s32 byHeight;

    switch (level) {
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
        if (zone_isActorOutsideId(0, 0x18) == 1) {
            if (zone_isActorOutsideId(0, 0x2E) == 0) {
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
        if (D_800C35EA[0] == 1 && (D_800C35E8[0] == 2 || D_800C35E8[0] == 1)) {
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

extern u8 D_1673[];

/* Draws one sky face, with texture set texSet (tiles from firstTex on): a width x height grid of tileW x tileH
 * textured quads in the plane `axis` ('x', 'y' or 'z') through (x, y, z). Quads outside the view
 * (func_150A6210) and, on the side sets, rows past the sky extent `limit` are skipped; visible ones are queued
 * in *vtxBufPtr and drawn four at a time. The vertex/queue cursors are carried between faces through the
 * pointer arguments. quad is scratch (callers pass NULL). */
Gfx *func_15110CFC(Gfx *gfx, Vtx **vtxBufPtr, s32 firstTex, Vtx *quad, s32 arg4, u32 width, u32 height,
                   u32 tileW, u32 tileH, f32 x, f32 y, f32 z, u8 axis, MtxF mtx, s32 limit,
                   s32 *vtxCountPtr, s32 *vtxIdxPtr, SkyTile *queue, s32 *lastFormatPtr,
                   u8 *sizes, u8 *shifts, s16 *palOffsets, u8 *tlutCounts, u8 *tiles, s32 texSet) {
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
    s32 texIndex;
    s32 texAddr;
    Vtx *v;
    s32 maxS;
    s32 maxT;
    s32 i;
    s32 padE; // matching: padE, padM, padR, padB and the unused v size IDO's -g3 frame
    s32 padM[1];
    s32 nCols;
    s32 nRows;
    s32 tileW8;
    s32 tileH8;
    s32 colIdx;
    s32 rowIdx;
    s32 format;
    s32 lastFormat;
    s32 padR;
    s32 vtxCount;
    s32 vtxIdx;

    tileW8 = tileW * 8;
    tileH8 = tileH * 8;
    vtxBuf = *vtxBufPtr;
    vtxCount = *vtxCountPtr;
    vtxIdx = *vtxIdxPtr;
    lastFormat = *lastFormatPtr;
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
    if (texSet < 4) {
        if (limit > 320) {
            limit = 320;
        }
        limit = (limit * nRows + 319) / 320;
    } else {
        limit = 0x7FFF;
    }
    curV = startV - tileH8;
    for (row = 0; row < height; row += tileH, curV -= tileH8, rowIdx++) {
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
                    y1 = curV + tileH8;
                    z1 = u + tileW8;
                    y2 = curV;
                    z2 = u + tileW8;
                    y3 = curV + tileH8;
                    z3 = u;
                    break;
                case 'Y':
                case 'y':
                    y0 = y1 = y2 = y3 = y;
                    x0 = u;
                    z0 = curV;
                    x1 = u + tileW8;
                    z1 = curV + tileH8;
                    x2 = u + tileW8;
                    z2 = curV;
                    x3 = u;
                    z3 = curV + tileH8;
                    break;
                case 'Z':
                case 'z':
                    z0 = z1 = z2 = z3 = z;
                    x0 = u;
                    y0 = curV;
                    x1 = u + tileW8;
                    y1 = curV + tileH8;
                    x2 = u + tileW8;
                    y2 = curV;
                    x3 = u;
                    y3 = curV + tileH8;
                    break;
            }
            if (func_150A6210(mtx, (x0 + x1) >> 1, (y0 + y1) >> 1, (z0 + z1) >> 1, tileW8, tileH8, 20000.0f) &&
                (D_800DBEAB || rowIdx < limit)) {
                texIndex = colIdx * nRows + firstTex + rowIdx;
                if (D_800DBEAC && D_800B0DF0->unk9 == 1 && texSet == 2 && rowIdx >= 4 && rowIdx < 7 &&
                    colIdx >= 2 && colIdx < 4) {
                    texIndex = (s32)&D_1673[colIdx * 3 + rowIdx - 10];
                }
                texAddr = texture_getAddress(texIndex, 0, 3, 0);
                if (!D_800DBEAB || rowIdx < limit) {
                    maxT = (u16)(s32)((tileH + 0.5f) * 32.0f);
                    maxS = (u16)(s32)((tileW + 0.5f) * 32.0f);
                    // matching: the quad pointer lives in the unused parameter, built with byte arithmetic
                    quad = (Vtx *)((u8 *)vtxBuf + vtxIdx * 16 + vtxCount * 16);
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
                    quad[0].v.ob[0] = x0;
                    quad[0].v.ob[1] = y0;
                    quad[0].v.ob[2] = z0;
                    quad[0].v.tc[1] = 16;
                    quad[0].v.tc[0] = 16;
                    quad[0].v.flag = 0;
                    quad[1].v.ob[0] = x2;
                    quad[1].v.ob[1] = y2;
                    quad[1].v.ob[2] = z2;
                    quad[1].v.tc[1] = 16;
                    quad[1].v.tc[0] = maxS;
                    quad[1].v.flag = 0;
                    quad[2].v.ob[0] = x1;
                    quad[2].v.ob[1] = y1;
                    quad[2].v.ob[2] = z1;
                    quad[2].v.tc[1] = maxT;
                    quad[2].v.tc[0] = maxS;
                    quad[2].v.flag = 0;
                    quad[3].v.ob[0] = x3;
                    quad[3].v.ob[1] = y3;
                    quad[3].v.ob[2] = z3;
                    quad[3].v.tc[1] = maxT;
                    quad[3].v.tc[0] = 16;
                    quad[3].v.flag = 0;
                    nCols = width / tileW;
                    queue[vtxCount >> 2].texture = texAddr;
                    queue[vtxCount >> 2].format =
                        // matching: index + base (not D_800DBE80[...]) sets IDO's addu operand order
                        *((colIdx * nRows + rowIdx + texSet * nCols * nRows) + (u8 *)D_800DBE80);
                    vtxCount += 4;
                    if (vtxCount == 16) {
                        s32 padB[2];
                        gSPVertex(gfx++, &vtxBuf[vtxIdx], 16, 0);
                        for (i = 0; i < 16; i += 4) {
                            format = queue[i >> 2].format;
                            gDPPipeSync(gfx++);
                            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, queue[i >> 2].texture);
                            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, ((sizes[format] + 0x800) >> shifts[format]) - 1, 0);
                            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, queue[i >> 2].texture + palOffsets[format]);
                            gDPLoadSync(gfx++);
                            gDPLoadTLUTCmd(gfx++, G_TX_LOADTILE - 1, tlutCounts[format]);
                            if (format != lastFormat) {
                                lastFormat = format;
                                gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, tiles[format], G_ON);
                            }
                            gSP1Triangle(gfx++, i + 2, i, i + 1, 0);
                            gSP1Triangle(gfx++, i + 3, i, i + 2, 0);
                        }
                        vtxCount = 0;
                        vtxIdx += 16;
                    }
                } else { // matching: the empty else arms steer IDO's register allocation
                }
            } else {
            }
            col += tileW;
            colIdx++;
            u += tileW8;
        }
    }
    *vtxCountPtr = vtxCount;
    *vtxIdxPtr = vtxIdx;
    *vtxBufPtr = vtxBuf;
    return gfx;
}

/* Switches the sky to `sky`: 1 frees the sky data and generates the star field; otherwise, while a textured sky
 * (mode 4) is showing a different one, unloads its textures and loads sky `sky`'s data. */
void func_1511172C(s32 sky) {
    s32 i;

    if (sky == 1) {
        D_800B0DF0->unk8 = 1;
        if (D_800DBE80 != NULL) {
            void *data = D_800DBE80; // matching: the copy loads the pointer into v0 before the call

            func_10004074(data);
        }
        func_15111858();
        return;
    }
    if (sky != D_800B0DF0->unk9 && D_800B0DF0->unk8 == 4) {
        if (D_80038080 != 0) {
            for (i = 0; i != 360; i++) {
                texture_releaseId(D_800891BC[D_800B0DF0->unk9][0] + i);
            }
        }
        func_10004074(D_800DBE80);
        D_800B0DF0->unk9 = sky;
        rom_loadPathTo(&D_800DBE80, 2, 13, D_800B0DF0->unk9);
    }
}


/* One star of the star-field sky: a point on a sphere of radius 256, stored as the high halves of f32s. */
typedef struct Star {
    u16 x;
    u16 y;
    u16 z;
    u8 unk6; // 0..4
    u8 unk7; // 64..254
} Star;

#define NUM_STARS 8000

/* Allocates the star field in D_800DBE80 and scatters NUM_STARS random stars over the sphere, latitudes kept
 * within 89 degrees of the equator. */
void func_15111858(void) {
    Star *stars;
    s32 i;

    D_800DBE80 = allocate_memory(NUM_STARS * sizeof(Star), 1, 0, 0);
    stars = D_800DBE80;
    for (i = 0; i != NUM_STARS; i++) {
        f32 ringRadius;
        f32 longitude;
        f32 v;
        f32 latitude;
        s32 height;
        f32 bits; // matching: read back through a u16 pointer (a union keeps it out of memory)

        longitude = (func_150ADA20() % 36000U) * 0.0001745329209f; // hundredths of a degree to radians
        v = (s32)((func_150ADA20() % 1584400U) - 792100) * 0.01f;
        if (v >= 0.0f) {
            latitude = 89.0f - sqrtf(v);
        } else {
            latitude = sqrtf(-v) + -89.0f;
        }
        latitude *= 0.01745329238f;
        height = func_150AD78C(latitude) * 256.0f;
        bits = height;
        stars[i].y = *(u16 *)&bits;
        ringRadius = sqrtf(0x10000 - height * height);
        bits = (s32)(func_150AD78C(longitude) * ringRadius);
        stars[i].x = *(u16 *)&bits;
        bits = (s32)(func_150AD780(longitude) * ringRadius);
        stars[i].z = *(u16 *)&bits;
        stars[i].unk7 = (func_150ADA20() % 191U) + 64;
        stars[i].unk6 = func_150ADA20() % 5U;
    }
}



/* Draws the textured skybox (sky mode 4): sets up a tinted CI texture pipeline and the rotation-only view
 * matrix, then draws each face the camera can see (faces 0-3 by yaw, 4 and 5 by pitch) as a grid of 60x30
 * tiles via func_15110CFC, flushing the last partial batch of queued tiles at the end. */
Gfx *func_15111AF4(Gfx *gfx, f32 x, f32 y, f32 z, f32 pitch, f32 yaw, f32 roll, s32 extent) {
    s32 i;
    s32 end;
    s32 start;
    s32 j;
    s32 axis;
    s32 texSet;
    s32 *texList;
    MtxF rotation;
    MtxF viewProj;
    Vtx *vtx;
    s32 vtxCount;
    s32 vtxIdx;
    SkyTile queue[4];
    s32 format;
    s32 lastFormat;
    f32 fov;
    u8 color[3];
    struct Game13D350U8x2 sizes;
    struct Game13D350U8x2 shifts;
    struct Game13D350S16x2 palOffsets;
    struct Game13D350U8x2 tlutCounts;
    struct Game13D350U8x2 tiles;

    sizes = D_800891EC;
    shifts = D_800891F0;
    palOffsets = D_800891F4;
    tlutCounts = D_800891F8;
    tiles = D_800891FC;
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gfx = view_setColorImage(view_setScissor(gfx, gCurrentView, 0, 0, 0, 0));
    fov = (gScreenLayout != 1) ? 80.0f : 95.0f;
    if (level == 0x41) {
        yaw -= 119.0f;
        if (yaw < 0.0f) {
            yaw += 360.0f;
        }
    } else if (level == 0x3C ||
               (level == 0x13 && D_800C35EA[0] == 1 && (D_800C35E8[0] == 2 || D_800C35E8[0] == 1))) {
        gfx = func_15110544(gfx, RENDER_VIEWS[gCurrentView].left, RENDER_VIEWS[gCurrentView].top,
                            RENDER_VIEWS[gCurrentView].right - 1.0f, RENDER_VIEWS[gCurrentView].bottom, 0, 0, 0);
    }
    color[0] = D_800B0DF0->unk5;
    color[1] = D_800B0DF0->unk6;
    color[2] = D_800B0DF0->unk7;
    gDPPipeSync(gfx++);
    if (color[0] + color[1] + color[2] == 3 * 255) {
        gDPSetCombineMode(gfx++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    } else {
        gDPSetEnvColor(gfx++, color[0], color[1], color[2], 255);
        gDPSetCombineLERP(gfx++, TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0,
                          TEXEL0, 0, ENVIRONMENT, 0, TEXEL0, 0, ENVIRONMENT, 0);
    }
    gDPSetOtherMode(gfx++, 0x08AC0F, 0x0F0A4004);
    gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_8b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
               G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 61 << G_TEXTURE_IMAGE_FRAC, 31 << G_TEXTURE_IMAGE_FRAC);
    gDPSetTile(gfx++, G_IM_FMT_CI, G_IM_SIZ_4b, 4, 0, G_TX_RENDERTILE + 1, 0, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD,
               G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE + 1, 0, 0, 61 << G_TEXTURE_IMAGE_FRAC, 31 << G_TEXTURE_IMAGE_FRAC);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_4b, 0, 256, G_TX_LOADTILE - 1, 0, 0, 0, 0, 0, 0, 0);
    func_151102CC(rotation, pitch, yaw, roll);
    func_150A7A48(rotation, RENDER_VIEWS[gCurrentView].projection, viewProj);
    guMtxF2L(viewProj, &D_800DBEB0[gCurrentView * 2 + D_800BE9C0]);
    gSPMatrix(gfx++, &D_800DBEB0[gCurrentView * 2 + D_800BE9C0], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPPerspNormalize(gfx++, RENDER_VIEWS[gCurrentView].perspNorm);
    texList = D_800891BC[D_800B0DF0->unk9];
    vtxCount = 0;
    vtx = D_800DBE88[gCurrentView][D_800BE9C0];
    vtxIdx = 0;
    lastFormat = -1;
    if (extent < 0) {
        extent = 0;
    }
    end = (level == 0xC) ? 6 : 5;
    start = (level != 0x19) ? 0 : 4;
    for (i = start; i < end; i++) {
        switch (i) {
            case 0:
                if ((yaw <= 180.0f && yaw > fov) || (yaw > 180.0f && yaw < 360.0f - fov)) {
                    continue;
                }
                break;
            case 1:
                if (yaw > 270.0f + fov || yaw < 270.0f - fov) {
                    continue;
                }
                break;
            case 2:
                if (yaw > 180.0f + fov || yaw < 180.0f - fov) {
                    continue;
                }
                break;
            case 3:
                if (yaw > 90.0f + fov || yaw < 90.0f - fov) {
                    continue;
                }
                break;
            case 4:
                if (pitch > -90.0f + fov || pitch < -90.0f - fov) {
                    continue;
                }
                break;
            case 5:
                if (pitch > 90.0f + fov || pitch < 90.0f - fov) {
                    continue;
                }
                if (extent < 300) {
                    continue;
                }
                break;
        }
        if (D_800A2DA0[i].x0 == D_800A2DA0[i].x1) {
            axis = 'x';
        } else if (D_800A2DA0[i].y0 == D_800A2DA0[i].y1) {
            axis = 'y';
        } else {
            axis = 'z';
        }
        texSet = (i == 5) ? 0 : i;
        gfx = func_15110CFC(gfx, &vtx, texList[texSet], 0, 2, D_800A2DA0[i].width, D_800A2DA0[i].height, 60, 30,
                            D_800A2DA0[i].x0, D_800A2DA0[i].y0, D_800A2DA0[i].z0, axis, rotation, extent, &vtxCount, &vtxIdx,
                            queue, &lastFormat, sizes.v, shifts.v, palOffsets.v, tlutCounts.v, tiles.v, texSet);
    }
    if (vtxCount != 0) {
        gSPVertex(gfx++, &vtx[vtxIdx], vtxCount, 0);
        for (j = 0; j < vtxCount; j += 4) {
            format = queue[j >> 2].format;
            gDPPipeSync(gfx++);
            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, queue[j >> 2].texture);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 2047, 0);
            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, queue[j >> 2].texture + palOffsets.v[format]);
            gDPLoadSync(gfx++);
            gDPLoadTLUTCmd(gfx++, G_TX_LOADTILE - 1, tlutCounts.v[format]);
            if (format != lastFormat) {
                lastFormat = format;
                gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, tiles.v[format], G_ON);
            }
            gSP1Triangle(gfx++, j + 2, j, j + 1, 0);
            gSP1Triangle(gfx++, j + 3, j, j + 2, 0);
        }
    }
    return gfx;
}
