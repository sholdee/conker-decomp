#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s16 D_80082FA6;
Gfx *func_1501A680(Gfx *arg0);

typedef struct {
    f32 unk0;
    f32 unk4;
    u8  unk8[0x178];
} View15173D00; // size 0x180, layout of D_800BE628 records

Gfx *func_15173D00(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s16 arg6) {
    Gfx *ret;

    gDPPipeSync(gfx++);
    gDPSetRenderMode(gfx++, 0, 0);
    gDPSetTexturePersp(gfx++, G_TP_NONE);
    gDPSetCycleType(gfx++, G_CYC_COPY);
    gDPSetAlphaCompare(gfx++, G_AC_NONE);
    gDPSetTextureLUT(gfx++, G_TT_NONE);
    gDPSetColorDither(gfx++, G_CD_DISABLE);
    gDPSetAlphaDither(gfx++, G_AD_DISABLE);
    gDPLoadTextureBlock(gfx++, (arg2 * D_800BE620 * 2) + D_800BE9C4, G_IM_FMT_RGBA, G_IM_SIZ_16b, (s32)((View15173D00 *)D_800BE628)[arg6].unk4, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, arg4);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, (s32)((View15173D00 *)D_800BE628)[arg6].unk4, 1);
    gSPTextureRectangle(gfx++, 0, 0, 12, 4, G_TX_RENDERTILE, arg1 << 5, 0, 0x1000, 0x400);
    gDPLoadTextureBlock(gfx++, D_8002AAE8[D_800BE9C0] + (arg2 * D_800BE620 * 2), G_IM_FMT_RGBA, G_IM_SIZ_16b, (s32)((View15173D00 *)D_800BE628)[arg6].unk4, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, arg5);
    gSPTextureRectangle(gfx++, 0, 0, 12, 4, G_TX_RENDERTILE, arg1 << 5, 0, 0x1000, 0x400);

    ret = (Gfx *)func_1501A490((s32)func_1501A680(gfx), D_80082FA6, 0, 0, 0, 0);
    gDPSetTexturePersp(ret, G_TP_PERSP);
    return ret + 1;
}

typedef struct Blk151745F0 {
    s32 unk0[9];
} Blk151745F0;

typedef struct Rec151745F0 {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ char padD[3];
    /* 0x10 */ Blk151745F0 unk10;
    /* 0x34 */ s8 unk34;
    /* 0x35 */ char pad35[3];
    /* 0x38 */ s32 unk38;
} Rec151745F0;

extern Rec151745F0 D_800DD348[3];

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
extern void (*D_8008A080[])(Rec151745F0 *, s32, s32, s32);
extern s32 D_800DD300[];
extern u16 D_800DD310[][4];
extern u16 D_800DD328[][4];
extern s32 D_800DD340;
void func_1501A764(s16 arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6);

Gfx *func_151742EC(Gfx *arg0, s32 arg1) {
    f32 spB4;
    f32 spB0;
    f32 spAC;
    Rec151745F0 *rec;
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
        if (rec->unkC == D_800BE9C0 + 1) {
            code = *(u16 *)((u8 *)D_800DD310 + i * 8);
            tbl = &D_80089630[code >> 13];
            pos = (tbl->base + (((code >> 2) & 0x7FF) << tbl->shift)) >> 3;
            {
            s32 d;
            d = pos - D_800DD300[i];
            ad = (d < 0) ? -d : d;
            }
            if (ad < 300) {
                idx = rec->unk34;
                col = D_800DD328[i][0];
                r = (D_800DD328[i][0] >> 8) & 0xF8;
                g = (D_800DD328[i][0] >> 3) & 0xF8;
                b = (D_800DD328[i][0] << 2) & 0xF8;
                D_800DD340 = col;
                if (idx != 0xFFFFFFFF) {
                    D_8008A080[idx](rec, r, g, b);
                }
            }
            rec->unkC = 0;
        }
        i++; rec++;
    } while (i < 3);
    for (i = 0; i < 3; i++) {
        if (D_800DD348[i].unkC == 3) {
            *(u16 *)((u8 *)D_800DD310 + i * 8) = 0xFFFF;
            D_800DD348[i].unkC = D_800BE9C0 + 1;
            func_1501A764((s16)arg1, D_800DD348[i].unk0, D_800DD348[i].unk4, D_800DD348[i].unk8, &spB4, &spB0, &spAC);
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


s32 func_151745F0(f32 arg0, f32 arg1, f32 arg2, Blk151745F0 *arg3, s8 arg4, s32 arg5) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_800DD348[i].unkC == 0) {
            D_800DD348[i].unk0 = arg0;
            D_800DD348[i].unk4 = arg1;
            D_800DD348[i].unkC = 3;
            D_800DD348[i].unk8 = arg2;
            D_800DD348[i].unk34 = arg4;
            D_800DD348[i].unk38 = arg5;
            if (arg3 != NULL) {
                D_800DD348[i].unk10 = *arg3;
            }
            return 0;
        }
    }
    return 1;
}

