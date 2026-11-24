#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
#define D_800DDDC8 D_800DDDC8_hdr
#define D_800DDDD8 D_800DDDD8_hdr
#define D_800DDE20 D_800DDE20_hdr
#define D_800DDE3C D_800DDE3C_hdr
#include "variables.h"
#undef D_800DDDC8
#undef D_800DDDD8
#undef D_800DDE20
#undef D_800DDE3C
extern f32 D_800DDDC8[];
extern f32 D_800DDDD8[];
extern u8 D_800DDE20[];
extern u8 D_800DDE3C[];

extern f32 func_15048A40(u8 arg0);


extern u8 D_800DDDA0[];
extern u8 D_800DDDC0;

typedef struct {
    char pad_0x00[0xE];
    s16 field_0x0E;
    char pad_0x10[0x1B];
    u8 field_0x2B;
    char pad_0x2C[0x2];
    s16 field_0x2E;
} ColorBlendTask;

void func_1517EE40(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s8 arg4, s32 arg5) {
    u8 *p;

    if ((s8)arg4 == ((s8 *)&D_800DDDAC)[arg5]) {
        if ((&D_800DDDB0)[arg5] >= (&D_800DDE28)[arg5]) {
            if (((u8 *)&D_800DDDC0)[arg5] == 0) {
                return;
            }
        }
    }

    p = &D_800DDDA0[arg5 * 3];
    p[1] = arg1;
    p[2] = arg2;
    p[0] = arg0;
    (&D_800DDE28)[arg5] = arg3;
    (&D_800DDDB0)[arg5] = 0;
    ((s8 *)&D_800DDDAC)[arg5] = (s8)arg4;
    ((u8 *)&D_800DDDC0)[arg5] = 0;
}

extern u8 D_800DDDAC;
extern s32 D_800DDDB0;
extern s32 D_800DDE28;
extern u8 D_800DDDC0;
s32 func_1517EF00(s32 arg0)
{
  s32 v1;
  s32 a1;
  v1 = ((&D_800DDDB0)[arg0] * 255) / a1;
  if ((&D_800DDDC0)[arg0] != 0)
  {
    v1 = (&D_800DDDC0)[arg0];
  }
  else
  {
    a1 = (&D_800DDE28)[arg0];
    if (a1 != 0)
    {
      v1 = ((&D_800DDDB0)[arg0] * 255) / a1;
      if (v1 >= 0x100)
      {
        v1 = 0xFF;
      }
    }
    else
    {
      v1 = 0xFF;
    }
    if (((s8 *) (&D_800DDDAC))[arg0] == 0)
    {
      v1 = 255 - v1;
    }
  }
  return v1;
}

extern s32 func_1517EF00(s32);


extern s32 func_1517EF00(s32);

s32 func_1517EFAC(s32 arg0) {
    if (func_1517EF00(arg0) == 0xFF) {
        return 1;
    }
    return 0;
}

s32 func_1517EFDC(void) {
    s32 s1 = -1;
    s32 s0;
    s32 v0;
    for (s0 = 0; s0 <= D_80082FA0; s0++) {
        if ((func_1517EF00(s0) > 0) || (0.0f < D_800DDDC8[s0])) {
            s1++;
        }
    }
    v0 = D_80082FA0;
    if (s1 == v0) {
        return 1;
    }
    return 0;
}

#define WGFX1517F08C(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

Gfx *func_1517F08C(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    typedef struct {
        char pad0[0x24];
        f32 unk24;
        f32 unk28;
        f32 unk2C;
        f32 unk30;
        char pad34[0x14C];
    } Struct1517F08C;
    WGFX1517F08C(arg0++, 0xE7000000, 0);
    WGFX1517F08C(arg0++, 0xFCFFFFFF, 0xFFFDF6FB);
    gDPSetPrimColor(arg0++, 0, 0, arg2, arg3, arg4, arg1);
    WGFX1517F08C(arg0++, 0xEF002CFF, 0x504344);

    gDPFillRectangle(arg0++,
        ((Struct1517F08C *)D_800BE628)[arg5].unk2C,
        ((Struct1517F08C *)D_800BE628)[arg5].unk24,
        ((Struct1517F08C *)D_800BE628)[arg5].unk30,
        ((Struct1517F08C *)D_800BE628)[arg5].unk28);

    return arg0;
}

extern u8 D_800DDDA0[];
Gfx *func_1517F08C(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

s32 func_1517F3A0(s32 arg0, s32 arg1) {
    s32 v0 = func_1517EF00(arg1);
    u8 *p;
    if (v0 == 0) {
        return arg0;
    }
    p = &D_800DDDA0[arg1 * 3];
    return func_1517F08C(arg0, v0, p[0], p[1], p[2], arg1);
}

s32 func_1517F40C(s32 arg0) {
    if ((&D_800DDDB0)[arg0] >= (&D_800DDE28)[arg0]) {
        return 1;
    }
    return 0;
}

void func_1517F448(s32 arg0) {
    if ((&D_800DDDB0)[arg0] != (&D_800DDE28)[arg0]) {
        (&D_800DDDB0)[arg0] += D_800BE9E4;
    }
}

extern u8 D_800DDD90[];
extern u8 D_800DDD9C[];
extern s16 D_800DDE10[];

void func_1517F488(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    u8 *p = &D_800DDD90[arg5 * 3];
    p[0] = arg0;
    p[1] = arg1;
    p[2] = arg2;
    D_800DDD9C[arg5] = arg3;
    D_800DDE10[arg5] = arg4;
}

s32 func_1517F4D8(s32 arg0, s32 arg1) {
    u8 *p;

    if (((u16 *)D_800DDE10)[arg1] == 0) {
        return arg0;
    }

    if (D_800DDD9C[arg1] == 0) {
        return arg0;
    }

    p = &D_800DDD90[arg1 * 3];
    return func_1517F08C(arg0, D_800DDD9C[arg1], p[0], p[1], p[2], arg1);
}

extern u8 D_8008D010[];
extern u8 D_800DDD88;
extern u8 D_800DDD89;
extern u8 D_800DDD8A;
extern u8 D_800DDD8B;
extern u8 D_800DDD8C;

s32 func_1517F564(s32 arg0) {
    f32 temp_f2[4];
    f32 sp20;
    f32 fade;
    u8 *p;
    s32 v0;
    s32 a1;
    s32 red;
    s32 green;
    s32 blue;

    if (D_800DDE08 == 0) {
        return arg0;
    }

    v0 = D_800DDD8B;
    if (v0 == 0) {
        return arg0;
    }

    if (D_800DDD8C != 0) {
        fade = (func_15048A40(D_800DDD89) + 1.0f) * 0.5f;
        a1 = (s32)((f32)D_800DDD8B * fade);
        sp20 = 0.0f;
    } else {
        a1 = v0;
        sp20 = temp_f2[0];
    }

    p = &D_8008D010[D_800DDD8A * 6];
    red = (s32)((f32)p[0] + ((f32)(p[3] - p[0]) * sp20));
    green = (s32)((f32)p[1] + ((f32)(p[4] - p[1]) * sp20));
    blue = (s32)((f32)p[2] + ((f32)(p[5] - p[2]) * sp20));
    return func_1517F08C(arg0, a1, red, green, blue, 0);
}

void func_1517F720(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    D_800DDE08 = arg1;
    D_800DDD88 = arg2;
    D_800DDD89 = 0;
    D_800DDD8A = arg0;
    D_800DDD8B = arg3;
    D_800DDD8C = arg4;
}

extern s32 D_800BE9E4;

void func_1517F75C(void) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (D_800BE9E4 < ((u16 *)D_800DDE10)[i]) {
            ((u16 *)D_800DDE10)[i] -= D_800BE9E4;
        } else {
            ((u16 *)D_800DDE10)[i] = 0;
        }
    }
}


extern s32 D_800BE9E4;
extern u16 D_800DDE08;
extern u8 D_800DDD88;
extern u8 D_800DDD89;
void func_1517F7B4(void)
{
  if (D_800DDE08)
  {
    if (D_800BE9E4 < D_800DDE08)
    {
      D_800DDE08 -= D_800BE9E4;
    }
    else
    {
      D_800DDE08 = 0;
    }
    D_800DDD89 += D_800DDD88 * D_800BE9E4;
  }
}


typedef struct {
    char pad0[0xC];
    f32 unkC;
    f32 unk10;
    char pad14[0x10];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    char pad30[0x150];
} Struct1517F814;
extern f32 D_800DDDE8[][2];
void func_150A7A00(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6, f32 *arg7);

void func_1517F814(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4) {
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    Struct1517F814 *v1;

    if (arg0 == 0) {
        func_150A7A00((s32)((u8 *)&D_800D9D10 + (arg4 << 6)), arg1, arg2, arg3, &sp3C, &sp38, &sp34, &sp30);
        if (sp30 == 0.0f) {
            sp30 = 1.0f;
        }
        v1 = &((Struct1517F814 *)D_800BE628)[arg4];
        sp3C = (v1->unkC * sp3C) / sp30;
        sp38 = (v1->unk10 * sp38) / sp30;
        sp3C += v1->unkC;
        sp38 = v1->unk10 - sp38;
        sp3C += v1->unk2C;
        sp38 += v1->unk24;
    } else {
        sp3C = arg1;
        sp38 = arg2;
        v1 = &((Struct1517F814 *)D_800BE628)[arg4];
    }
    if (sp3C > 10000.0f) {
        sp3C = 10000.0f;
    } else if (sp3C < -10000.0f) {
        sp3C = -10000.0f;
    }
    if (sp38 > 10000.0f) {
        sp38 = 10000.0f;
    } else if (sp38 < -10000.0f) {
        sp38 = -10000.0f;
    }
    D_800DDDE8[arg4][0] = sp3C - v1->unkC;
    D_800DDDE8[arg4][1] = sp38 - v1->unk10;
}

Gfx *func_1517F9F4(Gfx *gfx, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 arg5) {
    typedef struct {
        char pad0[0x24];
        f32 unk24;
        f32 unk28;
        f32 unk2C;
        f32 unk30;
        char pad34[0x14C];
    } Struct1517F9F4;
    Struct1517F9F4 *vp;
    s32 temp;
    s32 clipped;
    s32 top;
    s32 bottom;
    s32 right;
    s32 t;

    if (lrx < ulx) {
        temp = ulx;
        ulx = lrx;
        lrx = temp;
    }
    if (lry < uly) {
        temp = uly;
        uly = lry;
        lry = temp;
    }
    vp = &((Struct1517F9F4 *)D_800BE628)[arg5];
    temp = vp->unk2C;
    top = vp->unk24;
    right = vp->unk30;
    bottom = vp->unk28;
    if (!((uly < bottom) && (top < lry) && (ulx < right) && (temp < lrx))) {
        return gfx;
    }

    if (ulx < temp) { clipped = temp; } else { if (right < ulx) { t = right; } else { t = ulx; } clipped = t; }
    ulx = clipped;
    if (uly < top) { clipped = top; } else { if (bottom < uly) { t = bottom; } else { t = uly; } clipped = t; }
    uly = clipped;
    if (lrx < temp) { clipped = temp; } else { if (right < lrx) { t = right; } else { t = lrx; } clipped = t; }
    lrx = clipped;
    if (lry < top) { clipped = top; } else { if (bottom < lry) { t = bottom; } else { t = lry; } clipped = t; }
    lry = clipped;
    gDPFillRectangle(gfx++, ulx, uly, lrx, lry);
    return gfx;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1AC2F0/func_1517FB9C.s")

typedef struct {
    char pad0[0x4];
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    char pad14[0x10];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    char pad34[0x14C];
} Struct15180580;
extern f32 *D_800DDE44;
extern f32 *D_800DDE48;
extern f32 *D_800DDE4C;
extern u8 D_800DDD78[];
extern u8 D_8008D018[];
extern s32 D_80090298[];
extern s32 func_1510D0EC(s32, s32 *, s32, s32);
Gfx *func_1517FB9C(Gfx *gfx, s32 arg1, s32 arg2, f32 arg3, f32 arg4, s32 arg5);

#define VP15180580 (((Struct15180580 *)D_800BE628)[arg1])

Gfx *func_15180580(Gfx *gfx, s32 arg1) {
    f32 v;

    if (D_800DDE40 != 0) {
        if (D_800C35EA == 1) {
            f32 sc;

            v = *D_800DDE44 * 0.01f;
            if (v > 1.0f) {
                v = 1.0f;
            } else if (v < 0.0f) {
                v = 0.0f;
            }
            sc = *D_800DDE48 * 0.01f;
            D_800DDDE8[arg1][0] = ((Struct15180580 *)D_800BE628)->unk4 * sc;
            sc = -(*D_800DDE4C * 0.01f);
            D_800DDDE8[arg1][1] = ((Struct15180580 *)D_800BE628)->unk8 * sc;
        } else {
            v = 1.0f;
        }
    } else {
        v = D_800DDDC8[arg1];
    }
    if (v == 0.0f) {
        return gfx;
    }
    WGFX1517F08C(gfx++, 0xDE000000, D_8008D018);
    WGFX1517F08C(gfx++, 0xE7000000, 0);
    WGFX1517F08C(gfx++, 0xF7000000, 0x10001);
    WGFX1517F08C(gfx++, 0xFA000000, 0xFF);
    WGFX1517F08C(gfx++, 0xFCFFFFFF, 0xFFFDF6FB);
    WGFX1517F08C(gfx++, 0xEF000CFF, 0x0F0A4004);
    WGFX1517F08C(gfx++, 0xD9E0FFFE, 0);
    WGFX1517F08C(gfx++, 0xD9FFFFFF, 0x200004);
    if (v < 1.0f) {
        s32 i;
        s16 ulx;
        s16 uly;
        s16 lrx;
        s16 lry;
        s16 size;
        s16 top;
        s16 left;
        s16 bottom;
        s16 right;

        top = VP15180580.unk24;
        left = VP15180580.unk2C;
        bottom = VP15180580.unk28;
        right = VP15180580.unk30;
        size = (s32)(-399.0f * v + 400.0f);
        uly = top;
        ulx = left;
        lry = (s32)((D_800DDDE8[arg1][1] + VP15180580.unk10) - size);
        lrx = right;
        gfx = func_1517F9F4(gfx, ulx, uly, lrx, lry, arg1);
        uly = (s32)((D_800DDDE8[arg1][1] + VP15180580.unk10) + size);
        ulx = left;
        lry = bottom;
        lrx = right;
        gfx = func_1517F9F4(gfx, ulx, uly, lrx, lry, arg1);
        uly = (s32)((D_800DDDE8[arg1][1] + VP15180580.unk10) - size);
        ulx = left;
        lry = (s32)((D_800DDDE8[arg1][1] + VP15180580.unk10) + size);
        lrx = (s32)((D_800DDDE8[arg1][0] + VP15180580.unkC) - size);
        gfx = func_1517F9F4(gfx, ulx, uly, lrx, lry, arg1);
        uly = (s32)((D_800DDDE8[arg1][1] + VP15180580.unk10) - size);
        ulx = (s32)((D_800DDDE8[arg1][0] + VP15180580.unkC) + size);
        lry = (s32)((D_800DDDE8[arg1][1] + VP15180580.unk10) + size);
        lrx = right;
        gfx = func_1517F9F4(gfx, ulx, uly, lrx, lry, arg1);
        if ((&D_800DDE1C)[arg1] != 0) {
            WGFX1517F08C(gfx++, 0xE7000000, 0);
            WGFX1517F08C(gfx++, 0xEF002C0F, 0x0F0A4004);
            WGFX1517F08C(gfx++, 0xFCFFFFFF, 0xFFFDF6FB);
            WGFX1517F08C(gfx++, 0xFA000000, 0xFF);
            for (i = 0; i < 2; i++) {
                uly = (s32)(VP15180580.unk10 - 1);
                lry = (s32)(VP15180580.unk10 + 1);
                gfx = func_1517F9F4(gfx, left - i, uly - i, right + i, lry + i, arg1);
                ulx = (s32)(VP15180580.unkC - 1);
                lrx = (s32)(VP15180580.unkC + 1);
                gfx = func_1517F9F4(gfx, ulx - i, top - i, lrx + i, bottom + i, arg1);
                if ((&D_800DDE1C)[arg1] == 1) {
                    s32 pos[2];

                    pos[0] = D_800DDDE8[arg1][0] + VP15180580.unkC;
                    pos[1] = D_800DDDE8[arg1][1] + VP15180580.unk10;
                    uly = pos[1] - 60;
                    ulx = pos[0] - 60;
                    lry = pos[1] - 58;
                    lrx = pos[0] - 30;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, arg1);
                    uly = pos[1] - 60;
                    ulx = pos[0] - 60;
                    lry = pos[1] - 30;
                    lrx = pos[0] - 58;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, arg1);
                    uly = pos[1] - 30;
                    ulx = pos[0] + 58;
                    lry = pos[1] - 60;
                    lrx = pos[0] + 60;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, arg1);
                    uly = pos[1] - 58;
                    ulx = pos[0] + 30;
                    lry = pos[1] - 60;
                    lrx = pos[0] + 60;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, arg1);
                    uly = pos[1] + 60;
                    ulx = pos[0] + 60;
                    lry = pos[1] + 30;
                    lrx = pos[0] + 58;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, arg1);
                    uly = pos[1] + 60;
                    ulx = pos[0] + 60;
                    lry = pos[1] + 58;
                    lrx = pos[0] + 30;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, arg1);
                    uly = pos[1] + 60;
                    ulx = pos[0] - 60;
                    lry = pos[1] + 30;
                    lrx = pos[0] - 58;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, arg1);
                    uly = pos[1] + 60;
                    ulx = pos[0] - 60;
                    lry = pos[1] + 58;
                    lrx = pos[0] - 30;
                    gfx = func_1517F9F4(gfx, ulx - i, uly - i, lrx + i, lry + i, arg1);
                }
                if (i == 0) {
                    WGFX1517F08C(gfx++, 0xE7000000, 0);
                    WGFX1517F08C(gfx++, 0xEF002C0F, 0x00504244);
                    WGFX1517F08C(gfx++, 0xF7000000, 0);
                    WGFX1517F08C(gfx++, 0xFA000000, 0xFFFFFF00);
                }
            }
        }
        WGFX1517F08C(gfx++, 0xE7000000, 0);
        WGFX1517F08C(gfx++, 0xFCFFFFFF, 0xFFFFF3F9);
        WGFX1517F08C(gfx++, 0xD7000002, 0xFFFFFFFF);
        WGFX1517F08C(gfx++, 0xFD900000, func_1510D0EC(D_80090298[1], NULL, 3, 0));
        WGFX1517F08C(gfx++, 0xF5900000, 0x07000000);
        WGFX1517F08C(gfx++, 0xF3000000, 0x077FF000);
        WGFX1517F08C(gfx++, 0xF5881000, 0x00098260);
        WGFX1517F08C(gfx++, 0xF2002002, 0x000FE0FE);
        WGFX1517F08C(gfx++, 0xEF002CFF, 0x005011C4);
        gfx = func_1517FB9C(gfx, size, 0x40, 0, 0, arg1);
        if ((&D_800DDE1C)[arg1] != 0) {
            WGFX1517F08C(gfx++, 0xE7000000, 0);
            WGFX1517F08C(gfx++, 0xFD700000, func_1510D0EC(D_80090298[2], NULL, 3, 0));
            WGFX1517F08C(gfx++, 0xF5700000, 0x07000000);
            WGFX1517F08C(gfx++, 0xF3000000, 0x077FF000);
            WGFX1517F08C(gfx++, 0xF5681000, 0x00098260);
            gfx = func_1517FB9C(gfx, size / 2, 0x40, 0, 0, arg1);
            if ((&D_800DDE1C)[arg1] == 2) {
                WGFX1517F08C(gfx++, 0xFCFFFFFF, 0xFFFCF279);
                WGFX1517F08C(gfx++, 0xFD100000, func_1510D0EC(D_80090298[D_800DDD78[arg1] + 3], NULL, 3, 0));
                WGFX1517F08C(gfx++, 0xF5100000, 0x07000000);
                WGFX1517F08C(gfx++, 0xF3000000, 0x073FF000);
                WGFX1517F08C(gfx++, 0xF5101000, 0x00094250);
                gfx = func_1517FB9C(gfx, size / 4, 0x20, 32.0f, 28.0f, arg1);
                gfx = func_1517FB9C(gfx, size / 4, 0x20, 28.0f, 32.0f, arg1);
                if (D_800DDD78[arg1] < 2) {
                    D_800DDD78[arg1]++;
                } else {
                    D_800DDD78[arg1] = 0;
                }
            }
        }
    } else {
        gDPFillRectangle(gfx++, VP15180580.unk2C, VP15180580.unk24, VP15180580.unk30, VP15180580.unk28);
    }
    WGFX1517F08C(gfx++, 0xD9FFFFFF, 1);
    return gfx;
}

typedef struct Game1AC2F0Mode {
    u8 unknown000[0x120];
    u8 state;
    u8 unknown121[3];
    s16 timer;
} Game1AC2F0Mode;

typedef struct Game1AC2F0Player {
    s32 kind;
    u8 unknown004[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 unknown020[0x130];
    f32 height;
    u8 unknown154[0x80];
    u8 *model;
    u8 unknown1D8[0x144];
    Game1AC2F0Mode *mode;
    u8 unknown320[0xC];
} Game1AC2F0Player;

void func_150A7960(void *, f32, f32, f32, f32 *, f32 *, f32 *);
extern f32 D_800A7260[];
extern f32 D_800A7270[];
extern u8 D_800DDD70[];
extern u8 D_800DDD74[];
extern u16 D_800DDD80[];
void func_151814FC(void) {
    s32 i;
    Game1AC2F0Player *player;

    for (i = 0; i <= D_80082FA0; i++) {
        if (D_800DDE20[i] != 0) {

            player = &((Game1AC2F0Player *)D_800CC2D0)[i];
            D_800DDDC8[i] += D_800DDDD8[i] * D_800BE9E4;
            if (player->mode == NULL) {
                D_800DDDC8[i] = 1.0f;
            } else if (player->mode->state == 3) {
                if (D_800A7260[D_80082FA0] <= D_800DDDC8[i]) {
                    D_800DDDC8[i] = D_800A7260[D_80082FA0];
                    player->mode->state = 4;
                    D_800DDD70[i] = 30;
                    D_800DDD74[i] = 0;
                }
            } else if (player->mode->state == 4) {
                if (D_800DDD70[i]) {
                    f32 scale;

                    if (D_800DDD70[i] >= D_800BE9E4) {
                        D_800DDD70[i] -= D_800BE9E4;
                    } else {
                        D_800DDD70[i] = 0;
                    }
                    D_800DDD74[i] += D_800BE9E4 * 20;
                    if (D_800DDD70[i] < 30) {
                        scale = (D_800DDD70[i] * 0.01f) / 30.0f;
                    } else {
                        scale = 0.01f;
                    }
                    D_800DDDC8[i] = D_800A7260[D_80082FA0] + scale * func_15048A40(D_800DDD74[i]);
                } else {
                    D_800DDDC8[i] = D_800A7260[D_80082FA0];
                    player->mode->state = 5;
                }
            } else if (player->mode->state == 5) {
                D_800DDDC8[i] = D_800A7260[D_80082FA0];
            } else if (player->mode->state == 6) {
                D_800DDDD8[i] = 0.009f;
                if (D_800A7270[D_80082FA0] <= D_800DDDC8[i]) {
                    D_800DDDC8[i] = D_800A7270[D_80082FA0];
                    player->mode->state = 7;
                    player->mode->timer = 30;
                    D_800DDDD8[i] = 0.0f;
                }
            } else if (player->mode->state == 8) {
                D_800DDDD8[i] = 0.004f;
                if (D_800DDDC8[i] >= 1.0f) {
                    D_800DDDC8[i] = 1.0f;
                    player->mode->state = 9;
                }
            }
        } else if (D_800DDE3C[i] != 0) {
            Game1AC2F0Player *actor;
            f32 x;
            f32 y;
            f32 z;
            f32 prevX;
            f32 prevY;
            f32 rate;
            f32 diff;
            f32 step;
            s32 sign;

            actor = &((Game1AC2F0Player *)D_800CC2D0)[i];
            D_800DDDC8[i] += D_800DDDD8[i] * D_800BE9E4;
            if (actor->model != NULL) {
                u8 *model;
                f32 height;

                model = actor->model;
                if (actor->kind == 1) {
                    model += 0x300;
                }
                height = actor->height * 40.0f;
                func_150A7960(model, 0.0f, height, 0.0f, &x, &y, &z);
            } else {
                x = actor->x;
                y = actor->y;
                z = actor->z;
            }
            prevX = ((f32 *)((u8 *)D_800DDDE8 + i * 8))[0];
            prevY = ((f32 *)((u8 *)D_800DDDE8 + i * 8))[1];
            func_1517F814(0, x, y, z, i);
            x = ((f32 *)((u8 *)D_800DDDE8 + i * 8))[0];
            y = ((f32 *)((u8 *)D_800DDDE8 + i * 8))[1];
            if (D_800BE9B4 != 0) {
                rate = 1.0f;
            } else {
                rate = 0.05f;
            }
            diff = x - prevX;
            step = diff;
            if (fabsf(diff) > 15.0f) {
                sign = (diff < 0) ? -1 : 1;
                step = sign * 15.0f;
                prevX = x - step;
            }
            step *= rate;
            prevX += step;
            diff = y - prevY;
            step = diff;
            if (fabsf(diff) > 11.25f) {
                sign = (diff < 0) ? -1 : 1;
                step = sign * 11.25f;
                prevY = y - step;
            }
            step *= rate;
            prevY += step;
            ((f32 *)((u8 *)D_800DDDE8 + i * 8))[0] = prevX;
            ((f32 *)((u8 *)D_800DDDE8 + i * 8))[1] = prevY;
            if (D_800DDE3C[i] == 1) {
                if (D_800DDDC8[i] <= 0.9f) {
                    D_800DDD70[i] = 30;
                    D_800DDDC8[i] = 0.9f;
                    D_800DDDD8[i] = 0.0f;
                    D_800DDE3C[i] = 2;
                }
            } else if (D_800DDE3C[i] == 2) {
                if (D_800DDD70[i]) {
                    f32 scale;

                    if (D_800DDD70[i] >= D_800BE9E4) {
                        D_800DDD70[i] -= D_800BE9E4;
                    } else {
                        D_800DDD70[i] = 0;
                    }
                    D_800DDD74[i] -= D_800BE9E4 * 20;
                    if (D_800DDD70[i] < 30) {
                        scale = (D_800DDD70[i] * 0.01f) / 30.0f;
                    } else {
                        scale = 0.01f;
                    }
                    D_800DDDC8[i] = func_15048A40(D_800DDD74[i]) * scale + 0.9f;
                } else {
                    D_800DDDC8[i] = 0.9f;
                    D_800DDE3C[i] = 3;
                    D_800DDD80[i] = 60;
                }
            } else if (D_800DDE3C[i] == 3) {
                if (D_800BE9E4 < D_800DDD80[i]) {
                    D_800DDD80[i] -= D_800BE9E4;
                } else {
                    D_800DDE3C[i] = 4;
                    D_800DDDD8[i] = -0.009f;
                }
            } else if (D_800DDDC8[i] <= 0.0f) {
                D_800DDE3C[i] = 0;
                D_800DDDC8[i] = 0.0f;
            }
        } else if (D_800DDDD8[i] != 0.0f) {
            f32 speed;

            speed = D_800DDDD8[i];
            if (speed > 0.0f) {
                if (D_800DDDC8[i] >= 1.0f) {
                    D_800DDDD8[i] = 0.0f;
                    D_800DDDC8[i] = 1.0f;
                }
            } else if (D_800DDDC8[i] <= 0.0f) {
                D_800DDDD8[i] = 0.0f;
                D_800DDDC8[i] = 0.0f;
            }
            D_800DDDC8[i] += D_800DDDD8[i] * D_800BE9E4;
        }
    }
}


s32 func_15181CC8(s32 arg0) {
    if (D_800DDDC8[arg0] == 0.0f) {
        return 1;
    }
    return 0;
}

extern f32 D_800A72AC;
extern f32 D_800DDDE8[][2];

void func_15181D00(s32 arg0, s32 arg1) {
    if (arg1 == 0) {
        D_800DDDC8[arg0] = 0.0f;
    } else {
        D_800DDDD8[arg0] = 0.0f;
        D_800DDDC8[arg0] = D_800A72AC;
        D_800DDDE8[arg0][0] = 0.0f;
        D_800DDDE8[arg0][1] = 0.0f;
    }
    (&D_800DDE1C)[arg0] = arg1;
}

extern f32 D_800A72B0;
extern f32 D_800A72B4;

void func_15181D70(s32 arg0) {
    D_800DDDD8[arg0] = D_800A72B0;
    D_800DDDC8[arg0] = 0.0f;
    D_800DDDE8[arg0][0] = 0.0f;
    D_800DDDE8[arg0][1] = 0.0f;
    D_800DDE20[arg0] = 1;
}

void func_15181DC8(s32 arg0) {
    D_800DDDD8[arg0] = 0;
    D_800DDDC8[arg0] = 0.0f;
    D_800DDDE8[arg0][0] = 0.0f;
    D_800DDDE8[arg0][1] = 0.0f;
    D_800DDE20[arg0] = 0;
}

void func_15181E18(s32 arg0) {
    u8 *p;

    if (D_800BE616 != 0) {
        p = &D_800DDDA0[arg0 * 3];
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        (&D_800DDE28)[arg0] = 0x19;
        (&D_800DDDB0)[arg0] = 0;
        ((u8 *)&D_800DDDAC)[arg0] = 0;
        ((u8 *)&D_800DDDC0)[arg0] = 0;
    } else {
        D_800DDE3C[arg0] = 1;
        D_800DDDC8[arg0] = 1.0f;
        D_800DDDE8[arg0][0] = 0.0f;
        D_800DDDE8[arg0][1] = 0.0f;
        D_800DDDD8[arg0] = D_800A72B4;
    }
}

typedef struct {
    char pad0[0x4];
    f32 unk4;
    f32 unk8;
    char padC[0x18];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    char pad34[0x14C];
} Struct15181EE0;

extern f32 D_800A72C0;
extern s32 D_800902C8;
extern s32 D_800902CC;
extern s32 func_1510D0EC(s32, s32 *, s32, s32);

Gfx *func_15181EE0(Gfx *gfx) {
    s16 yl;
    s16 yh;
    s32 pad1;
    s16 top;
    s16 left;
    s16 bottom;
    s16 right;
    s32 y1;
    s32 y2;
    s32 xl;
    s32 xh;
    s32 dsdx;
    s32 dtdy;
    f32 t;
    f32 alpha;

    if (D_800DDE18 == 0) {
        return gfx;
    }
    alpha = *(f32 *)D_800DDE18;
    alpha *= 0.009999999776f;
    alpha = (alpha < 0.0f) ? 0.0f : ((alpha > 1.0f) ? 1 : alpha);
    if (alpha == 0.0f) {
        return gfx;
    }
    gDPPipeSync(gfx++);
    gDPSetFillColor(gfx++, 0x10001);
    gSPClearGeometryMode(gfx++, 0x1F0001);
    gSPSetGeometryMode(gfx++, 0x200004);
    gDPSetOtherMode(gfx++, 0x302C0F, 4);
    top = ((Struct15181EE0 *)D_800BE628)->unk24;
    left = ((Struct15181EE0 *)D_800BE628)->unk2C;
    bottom = ((Struct15181EE0 *)D_800BE628)->unk28;
    right = ((Struct15181EE0 *)D_800BE628)->unk30;
    t = ((Struct15181EE0 *)D_800BE628)->unk8 * 0.6999999881f;
    t += 30.0f;
    t *= alpha;
    y1 = (((Struct15181EE0 *)D_800BE628)->unk24 - 30.0f) + t;
    gfx = func_1517F9F4(gfx, left, top, right, (s16)y1, 0);
    t = ((Struct15181EE0 *)D_800BE628)->unk8 * (1.0f - D_800A72C0);
    t += 30.0f;
    t *= alpha;
    y2 = (((Struct15181EE0 *)D_800BE628)->unk28 + 30.0f) - t;
    gfx = func_1517F9F4(gfx, left, (s16)y2, right, bottom, 0);
    gDPPipeSync(gfx++);
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, 0, G_ON);
    gDPSetOtherMode(gfx++, 0x002CFF, 0x5011C4);
    gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFFF3F9);
    gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, func_1510D0EC(D_800902C8, 0, 3, 0));
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_4b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 7, 0, G_TX_CLAMP, 7, 0);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 2, 2, 0x1FE, 0xFE);
    dtdy = 0x222;
    dsdx = (128.0f / ((Struct15181EE0 *)D_800BE628)->unk4) * 1024.0f;
    yl = y1;
    yh = y1 + 30;
    xl = left;
    xh = right;
    gSPScisTextureRectangle(gfx++, xl << 2, yl << 2, xh << 2, yh << 2, G_TX_RENDERTILE, 0, 0, dsdx, dtdy);
    gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, func_1510D0EC(D_800902CC, 0, 3, 0));
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_4b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 7, 0, G_TX_CLAMP, 7, 0);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 2, 2, 0x1FE, 0xFE);
    yl = y2 - 30;
    yh = y2;
    gSPScisTextureRectangle(gfx++, xl << 2, yl << 2, xh << 2, yh << 2, G_TX_RENDERTILE, 0, 0, dsdx, dtdy);
    gSPSetGeometryMode(gfx++, 1);
    return gfx;
}

void func_15182670(s32 arg0, s32 arg1, s32 arg2, s32 arg3, volatile s16 arg4, u8 arg5, u8 arg6, s32 arg7) {
    struct {
        u8 unk0;
        u8 unk1;
        u8 unk2;
        u8 unk3;
        u8 unk4;
        u8 pad5;
        s16 unk6;
        s32 pad8;
    } sp34;
    struct260 *temp_v0;

    if (arg4 > 0) {
        sp34.unk0 = ((u8 *)&arg0)[3];
        sp34.unk1 = ((u8 *)&arg1)[3];
        sp34.unk2 = ((u8 *)&arg2)[3];
        sp34.unk3 = ((u8 *)&arg3)[3];
        sp34.unk4 = arg5;
        sp34.unk6 = ((u8 *)&arg3)[3] / arg4;

        temp_v0 = func_15149130(arg4, -1, 0x39, 3, 1, 0, (struct37 *)0x8, arg6, arg7);
        if (temp_v0 != NULL) {
            memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 8);
        }
    }
}

void func_15182748(ColorBlendTask *arg0) {
    arg0->field_0x2B = arg0->field_0x2E * arg0->field_0x0E;
}

s32 func_15182768(s32 arg0, s32 arg1, s16 arg2) {
    u8 *p = (u8 *)(*(s32 *)&arg1 + 0x28);
    if (arg2 == *(u8 *)(*(s32 *)&arg1 + 0x2C)) {
        arg0 = func_1517F08C(arg0, p[3], p[0], p[1], p[2], p[4]);
    }
    return arg0;
}
