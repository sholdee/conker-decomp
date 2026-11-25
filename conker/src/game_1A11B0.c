#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s16 D_80082FA6;
Gfx *func_1501A680(Gfx *gfx);

/* Per-player view record (0x180 bytes) in the array D_800BE628 points to; see game_126F60.c. */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 width;
    /* 0x08 */ u8 pad8[0x1C];
    /* 0x24 */ f32 top;
    /* 0x28 */ f32 bottom;
    /* 0x2C */ f32 left;
    /* 0x30 */ f32 right;
    /* 0x34 */ u8 pad34[0x14C];
} ProbeView; // size 0x180

#define VIEWS ((ProbeView *)D_800BE628)

/* Copies 4 pixels starting at (x, y) of the z-buffer into zDest and of the current frame buffer into
 * colorDest, using copy-mode texture rectangles that load one screen row of view's width. */
Gfx *func_15173D00(Gfx *gfx, s32 x, s32 y, s32 unused, s32 zDest, s32 colorDest, s16 view) {
    gDPPipeSync(gfx++);
    gDPSetRenderMode(gfx++, 0, 0);
    gDPSetTexturePersp(gfx++, G_TP_NONE);
    gDPSetCycleType(gfx++, G_CYC_COPY);
    gDPSetAlphaCompare(gfx++, G_AC_NONE);
    gDPSetTextureLUT(gfx++, G_TT_NONE);
    gDPSetColorDither(gfx++, G_CD_DISABLE);
    gDPSetAlphaDither(gfx++, G_AD_DISABLE);
    gDPLoadTextureBlock(gfx++, (y * D_800BE620 * 2) + D_800BE9C4, G_IM_FMT_RGBA, G_IM_SIZ_16b, (s32)VIEWS[view].width, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, zDest);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, (s32)VIEWS[view].width, 1);
    gSPTextureRectangle(gfx++, 0, 0, 3 << 2, 1 << 2, G_TX_RENDERTILE, x << 5, 0, 4 << 10, 1 << 10);
    gDPLoadTextureBlock(gfx++, D_8002AAE8[D_800BE9C0] + (y * D_800BE620 * 2), G_IM_FMT_RGBA, G_IM_SIZ_16b, (s32)VIEWS[view].width, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, colorDest);
    gSPTextureRectangle(gfx++, 0, 0, 3 << 2, 1 << 2, G_TX_RENDERTILE, x << 5, 0, 4 << 10, 1 << 10);

    gfx = (Gfx *)func_1501A490((s32)func_1501A680(gfx), D_80082FA6, 0, 0, 0, 0);
    gDPSetTexturePersp(gfx++, G_TP_PERSP);
    return gfx;
}

/* Caller data copied into a probe and handed back to its callback. */
typedef struct {
    s32 unk0[9];
} ProbeData;

/* A world point whose visibility is tested by sampling the z-buffer (up to 3 at once). */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ u8 state; // 0 free, 3 queued, else frame buffer index + 1 it was sampled in
    /* 0x10 */ ProbeData data;
    /* 0x34 */ s8 callback; // index into D_8008A080, -1 for none
    /* 0x38 */ s32 unk38;
} Probe;

extern Probe D_800DD348[3];

typedef struct {
    s32 shift;
    u32 base;
} Tbl80089630;

typedef struct {
    u8 pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x180 - 0x34];
} View151742EC;

extern Tbl80089630 D_80089630[];
extern void (*D_8008A080[])(Probe *, s32, s32, s32);
extern s32 D_800DD300[];
extern u16 D_800DD310[][4];
extern u16 D_800DD328[][4];
extern s32 D_800DD340;
void func_1501A764(s16 arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6);

Gfx *func_151742EC(Gfx *arg0, s32 arg1) {
    f32 spB4;
    f32 spB0;
    f32 spAC;
    Probe *rec;
    s32 i;
    u16 code;
    s8 idx;
    Tbl80089630 *tbl;
    u32 pos;
    s32 col;
    s32 ad;
    s32 r;
    s32 g;
    s32 b;
    s32 n;
    s32 x;
    s32 y;
    View151742EC *view;

    rec = D_800DD348;
    i = 0;
    n = 0;
    if (arg1 != 0) {
        return arg0;
    }
    do {
        if (rec->state == D_800BE9C0 + 1) {
            code = *(u16 *)((u8 *)D_800DD310 + i * 8);
            tbl = &D_80089630[code >> 13];
            pos = (tbl->base + (((code >> 2) & 0x7FF) << tbl->shift)) >> 3;
            {
            s32 d;
            d = pos - D_800DD300[i];
            ad = (d < 0) ? -d : d;
            }
            if (ad < 300) {
                idx = rec->callback;
                col = D_800DD328[i][0];
                r = (D_800DD328[i][0] >> 8) & 0xF8;
                g = (D_800DD328[i][0] >> 3) & 0xF8;
                b = (D_800DD328[i][0] << 2) & 0xF8;
                D_800DD340 = col;
                if (idx != 0xFFFFFFFF) {
                    D_8008A080[idx](rec, r, g, b);
                }
            }
            rec->state = 0;
        }
        i++; rec++;
    } while (i < 3);
    for (i = 0; i < 3; i++) {
        if (D_800DD348[i].state == 3) {
            *(u16 *)((u8 *)D_800DD310 + i * 8) = 0xFFFF;
            D_800DD348[i].state = D_800BE9C0 + 1;
            func_1501A764((s16)arg1, D_800DD348[i].x, D_800DD348[i].y, D_800DD348[i].z, &spB4, &spB0, &spAC);
            x = spB4;
            y = spB0;
            D_800DD300[i] = spAC;
            view = &((View151742EC *)D_800BE628)[arg1];
            if (x >= (s32)view->unk2C && x < (s32)view->unk30 && y >= (s32)view->unk24 && y < (s32)view->unk28) {
                arg0 = func_15173D00(arg0, x, y, 1, (s32)((u8 *)D_800DD310 + i * 8), (s32)((u8 *)D_800DD328 + i * 8), arg1);
            }
            if (n != 0) {
                *(u16 *)((u8 *)D_800DD310 + i * 8) = 0xFFFF;
            }
        }
    }
    return arg0;
}


/* Queues a visibility probe at (x, y, z); returns 1 when all probe slots are in use. */
s32 func_151745F0(f32 x, f32 y, f32 z, ProbeData *data, s8 callback, s32 arg5) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_800DD348[i].state == 0) {
            D_800DD348[i].x = x;
            D_800DD348[i].y = y;
            D_800DD348[i].state = 3;
            D_800DD348[i].z = z;
            D_800DD348[i].callback = callback;
            D_800DD348[i].unk38 = arg5;
            if (data != NULL) {
                D_800DD348[i].data = *data;
            }
            return 0;
        }
    }
    return 1;
}
