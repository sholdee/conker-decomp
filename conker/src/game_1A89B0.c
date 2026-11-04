#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 D_800DDD58;
extern Gfx D_8008CFA0[];
Gfx *func_1517D074(Gfx *, s16, s16, s16, f32, s32, s32, u8, u8, s32);

extern s32 func_151EF610(void);
extern u8 D_80095CE1[];

s32 func_1517B500(s16 *arg0, s16 *arg1, s32 *arg2, s32 arg3)
{
    s32 diff;
    s32 limit;
    s32 rnd;

    *arg2 = (*arg2 * 3) / 2;
    limit = D_80095CE1[((u8 *) arg0)[8] * 10] << 8;
    if (*arg2 >= limit)
    {
        *arg2 = *arg2 - limit;
    }

    arg1[0] = arg0[0];
    arg1[1] = arg0[1];
    arg1[2] = arg0[2];

    diff = 0x29 - ((s8 *) arg0)[0xD];
    if (diff == 0)
    {
        ((s8 *) arg0)[0xD] = (((s8 *) arg0)[0xD] - (func_151EF610() % 8)) + 4;
    }
    else if (diff < 0)
    {
        rnd = func_151EF610();
        ((s8 *) arg0)[0xD] = ((((s8 *) arg0)[0xD] - ((func_151EF610() % -diff) >> 1)) - (rnd % 8)) + 4;
    }
    else
    {
        rnd = func_151EF610();
        ((s8 *) arg0)[0xD] = ((((func_151EF610() % diff) >> 1) + ((s8 *) arg0)[0xD]) + (rnd % 8)) - 4;
    }

    arg1[0] = arg0[0];
    arg1[1] = arg0[1];
    arg1[2] = arg0[2];
    return 0;
}

s32 func_1517B6E8(s16 *arg0, s16 *arg1, s32 *arg2, s32 arg3)
{
  s32 v0;
  v0 = *arg2;
  if (v0 >= 0x600)
  {
    arg1[1] = ((v0 >> 6) + arg0[1]) - ((0, 0x18));
    arg1[0] = (arg0[0] + ((*arg2) >> 8)) - 6;
    {
      s32 temp;
      s32 v2;
      v2 = ((*((u8 *) arg3)) << 7) / (((*arg2) >> 6) - 0x17);
      temp = *((u8 *) arg3);
      v2 >>= 4;
      if (v2 < temp)
      {
        *((u8 *) arg3) = v2;
      }
    }
    *arg2 = 0x600;
  }
  else
  {
    arg1[0] = arg0[0];
    arg1[1] = arg0[1];
  }
  arg1[2] = arg0[2];
  return 0;
}


s32 func_1517B7A8(s16 *a0, s16 *a1, s32 *a2, s32 a3) {
    s32 v0 = *a2;

    if (v0 >= 0x300) {
        if (v0 >= 0x501) {
            *a2 = v0 - 0x200;
        } else {
            *a2 = 0x300;
        }
    }

    a1[0] = a0[0];
    a1[1] = a0[1];
    a1[2] = a0[2];
    return 0;
}

s32 func_1517B7F8(void *arg0, void *arg1, s32 *arg2, s32 arg3) {
    s32 temp_v1;
    s32 temp_v0;

    temp_v0 = *arg2;
    if (temp_v0 >= 0xD00) {
        return 1;
    }
    temp_v1 = temp_v0 - 0x600;
    if (temp_v0 >= 0x100) {
        temp_v1 = 0x190000 - (temp_v1 * temp_v1);
        *(s16 *)((u8 *)arg1 + 2) = (s16) (*(s16 *)((u8 *)arg0 + 2) + (temp_v1 / 7680));
        temp_v0 = *arg2;
        if (temp_v0 >= 0x200) {
            if (temp_v0 >= 0xA01) {
                *arg2 = temp_v0 - 0x800;
            } else {
                *arg2 = 0x200;
            }
        }
    } else {
        *(s16 *)((u8 *)arg1 + 2) = (s16) *(s16 *)((u8 *)arg0 + 2);
    }
    *(s16 *)((u8 *)arg1 + 0) = (s16) *(s16 *)((u8 *)arg0 + 0);
    *(s16 *)((u8 *)arg1 + 4) = (s16) *(s16 *)((u8 *)arg0 + 4);
    return 0;
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    u8 unk8;
    u8 pad9[4];
    s8 unkD;
} Game1A89B0Position;

extern s32 D_8008CEB4;
extern Game1A89B0Position *D_800DDD18;

void func_1517B89C(s32 first, s32 last) {
    s32 left;
    s32 right;
    s32 pivot;

    left = first;
    if (first < last) {
        right = last;
        while (1) {
            if (D_8008CEB4 == 0) {
                pivot = D_800DDD18[last].unk0;
                while (D_800DDD18[left].unk0 < pivot && left < last) left++;
                while (pivot < D_800DDD18[right].unk0 && first < right) right--;
            } else {
                pivot = D_800DDD18[last].unk4;
                while (D_800DDD18[left].unk4 < pivot && left < last) left++;
                while (pivot < D_800DDD18[right].unk4 && first < right) right--;
            }
            if (left < right) {
                {
                    Game1A89B0Position swap = D_800DDD18[left];
                    D_800DDD18[left] = D_800DDD18[right];
                    D_800DDD18[right] = swap;
                }
                left++;
                right--;
            } else {
                break;
            }
        }
        {
            Game1A89B0Position pivotSwap = D_800DDD18[left];
            D_800DDD18[left] = D_800DDD18[last];
            D_800DDD18[last] = pivotSwap;
            func_1517B89C(first, left - 1);
            func_1517B89C(left + 1, last);
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1A89B0/func_1517BBAC.s")

extern void func_10006240(s32, s32, s32);
extern void func_10004074(s32);

struct s1517CFC4_a { s32 unk0; s32 unk4; s32 unk8; s32 unkC; };

void func_1517CFC4(void) {
    s32 i;
    s32 *q;
    struct s1517CFC4_a *p;

    for (q = (s32 *)&D_800DD478, p = (struct s1517CFC4_a *)&D_800DDA90, i = 0; i != 15; i++, p++) {
        if (p->unk8 != 0) {
            if (q[i * 26] != 0) {
                func_10006240(p->unk8, q[i * 26], D_8003809C);
                func_10004074(p->unk8);
            }
            p->unk8 = 0;
        }
    }
}


Gfx *func_1517D074(Gfx *gfx, s16 x, s16 y, s16 z, f32 angle, s32 radius, s32 height, u8 width, u8 depth, s32 timg)
{
    u32 i;
    s16 c;
    s16 s;
    Vtx *v;

    i = D_800DDD58;
    v = &((Vtx *)D_800DDD10[D_800BE9C0])[i * 4];
    if ((u32)D_800DDD58 >= (u32)D_800DDD0C)
    {
        return gfx;
    }

    gSPDisplayList(gfx++, &D_8008CFA0);
    gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFCF438);
    gSPClearGeometryMode(gfx++, G_CULL_BACK);
    gDPSetOtherMode(gfx++, 0x182C3F, 0x0C184B50);

    angle *= 0.01745329238f;
    c = cosf(angle) * radius;
    s = sinf(angle) * radius;

    v[0].v.ob[0] = v[3].v.ob[0] = x + c;
    v[1].v.ob[1] = y;
    v[0].v.ob[1] = v[1].v.ob[1];
    v[0].v.ob[2] = v[3].v.ob[2] = z - s;
    v[1].v.ob[0] = v[2].v.ob[0] = x - c;
    v[2].v.ob[1] = v[3].v.ob[1] = (s16)(y + height + height);
    v[1].v.ob[2] = v[2].v.ob[2] = z + s;
    v[1].v.tc[0] = v[2].v.tc[0] = (width << 5) - 1;
    v[2].v.tc[1] = v[3].v.tc[1] = (depth << 5) - 1;

    gSPVertex(gfx++, &((Vtx *)D_800DDD10[D_800BE9C0])[i * 4], 4, 0);
    gDPLoadTextureBlock(gfx++, timg, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, (depth * 5 + 3) >> 2, 0,
                        G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
    gDPSetTile(gfx++, G_IM_FMT_I, G_IM_SIZ_4b, ((width >> 1) + 7) >> 3, (width * depth) >> 2, 1, 0,
               G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
    gDPSetTileSize(gfx++, 1, 0, 0, (width - 1) << 2, (depth - 1) << 2);

    ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4].v.flag =
        ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + 1].v.flag =
        ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + 2].v.flag =
        ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + 3].v.flag = 0;

    gSP1Triangle(gfx++, 0, 2, 1, 0);
    gSP1Triangle(gfx++, 0, 3, 2, 0);
    gSPSetGeometryMode(gfx++, G_SHADING_SMOOTH);
    D_800DDD58++;
    return gfx;
}

struct Entry1517D578 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8  unk6;
    s16 unk8;
    s16 unkA;
    f32 unkC;
};
extern struct Entry1517D578 D_800DDD28[];

void func_1517D578(s16 arg0, s16 arg1, s16 arg2, f32 arg3, s32 arg4, s32 arg5, u8 arg6)
{
    struct Entry1517D578 *e;
    u8 idx = D_8008CEB0;
    if (D_8008CEB0 < 3)
    {
        e = &D_800DDD28[idx];
        e->unk0 = arg0;
        e->unk2 = arg1;
        e->unk4 = arg2;
        e->unkC = arg3;
        e->unk8 = arg4;
        e->unkA = arg5;
        e->unk6 = arg6;
        D_8008CEB0 = idx + 1;
    }
}

void func_1517D5FC(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    func_1517D578(arg0, arg1, arg2, D_800DBFF0[arg3].unk380, arg4, arg5, D_800DDD1C >> 3);
}

s32 func_1517D690(s32 arg0, s32 arg1) {
    s32 saved;

    saved = D_800DDD58;
    if ((D_800DDD1C = D_800DDD1C + 5) >= 0xC8) {
        D_800DDD1C -= 0xC8;
    }

    if (D_8008CEB0 != 0) {
        do {
            D_8008CEB0 = D_8008CEB0 - 1;
            if (arg1 != 0) {
                arg0 = func_1517D074(arg0,
                                      D_800DDD28[D_8008CEB0].unk0,
                                      D_800DDD28[D_8008CEB0].unk2,
                                      D_800DDD28[D_8008CEB0].unk4,
                                      D_800DDD28[D_8008CEB0].unkC,
                                      (s32)(u16)D_800DDD28[D_8008CEB0].unk8,
                                      (s32)(u16)D_800DDD28[D_8008CEB0].unkA,
                                      0x10, 0x20,
                                      ((D_800DDD28[D_8008CEB0].unk6 * 0x280) << 1) + arg1);
            }
        } while (D_8008CEB0 != 0);
    }

    D_800DDD58 = saved;
    return arg0;
}

extern s32 D_800DDD24;
Gfx *func_1510CDB8(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3);

void func_1517D7B0(Gfx **arg0, s32 arg1) {
    Vtx *v;
    u32 i;
    u32 k;
    u32 j;
    u32 n;
    s32 mask;
    s32 tex;
    s16 width;
    s16 height;

    tex = 0xFFFF;
    if (D_800DDD10[D_800BE9C0] != 0) {
        *arg0 = (Gfx *)func_1517D690((s32)*arg0, D_800DDD24);
        if (D_800DDD20 != 0 && D_800DDD58 != 0) {
            k = 0;
            gSPDisplayList((*arg0)++, D_8008CFA0);
            *arg0 = func_1510CDB8(*arg0, 0xFF, 0xFF, 0);
            i = 0;
            while (i < D_800DDD58) {
                mask = 0;
                n = (D_800DDD58 - i >= 5) ? 16 : (D_800DDD58 - i) * 4;
                for (j = 0; j < n; j += 4) {
                    if (arg1 != ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + j + 1].v.flag) {
                        mask |= 1 << j;
                    }
                }
                if (mask != 0x1111) {
                    gSPVertex((*arg0)++, &((Vtx *)D_800DDD10[D_800BE9C0])[i * 4], n, 0);
                    for (j = 0; j < n; j += 4) {
                        while (k < n && (v = &((Vtx *)D_800DDD10[D_800BE9C0])[(s32)(i * 4)], tex != v[k].v.flag || ((1 << k) & mask))) {
                            k += 4;
                        }
                        if (k >= n && mask != 0x1111) {
                            for (k = 0, v = &((Vtx *)D_800DDD10[D_800BE9C0])[(s32)(i * 4)]; (1 << k) & mask; k += 4) {
                            }
                            if (tex != v[k].v.flag && k < n) {
                                tex = v[k].v.flag;
                                width = (v[k + 2].v.tc[0] + 1) >> 5;
                                height = (v[k + 2].v.tc[1] + 1) >> 5;
                                gDPLoadTextureBlock((*arg0)++, (&D_800DD478)[tex], G_IM_FMT_RGBA, G_IM_SIZ_16b, width,
                                                    (height * 5 + 3) >> 2, 0, G_TX_CLAMP, G_TX_CLAMP, 0, 0, 0, 0);
                                gDPSetTile((*arg0)++, G_IM_FMT_I, G_IM_SIZ_4b, ((width >> 1) + 7) >> 3, (width * height) >> 2, 1, 0,
                                           G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
                                gDPSetTileSize((*arg0)++, 1, 0, 0, (width - 1) << 2, (height - 1) << 2);
                            }
                        }
                        if (k < n && !((1 << k) & mask)) {
                            ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + k + 3].v.flag = 0;
                            ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + k].v.flag =
                                ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + k + 1].v.flag =
                                ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + k + 2].v.flag =
                                ((Vtx *)D_800DDD10[D_800BE9C0])[i * 4 + k + 3].v.flag;
                            gSP1Triangle((*arg0)++, k, k + 2, k + 1, 0);
                            gSP1Triangle((*arg0)++, k, k + 3, k + 2, 0);
                            mask |= 1 << k;
                        }
                    }
                }
                i += 4;
            }
            gSPSetGeometryMode((*arg0)++, G_SHADING_SMOOTH);
            gDPPipeSync((*arg0)++);
        }
    }
}


void func_1517BBAC(s32, s32, s32, s32, f32, s32, s32, s32);

void func_1517DE5C(void) {
    s32 i;
    struct s1517CFC4_a *p;

    if (D_800DDD20 != 0) {
        for (i = 0; i < 15; i++) {
            ((u8 (*)[16])&D_800DDA90)[i][0xC] = 0;
        }
        func_1517BBAC(D_800CC2D0->x_position, D_800CC2D0->y_position, D_800CC2D0->z_position, 1500,
                      D_800DBFF0->unk380, D_800DBFF0->unk2F8, D_800DBFF0->unk2FC, D_800DBFF0->unk300);
        for (i = 0; i < 15; i++) {
            if (((s32 *)&D_800DD478)[i * 26] != 0 && ((u8 (*)[16])&D_800DDA90)[i][0xC] == 0) {
                ((s32 *)&D_800DD478)[i * 26] = 0;
                ((s32 (*)[4])&D_800DDA90)[i][2] = 0;
            }
        }
    }
}

extern struct { s16 unk0; s16 unk2; s16 unk4; } D_800DD470;

void func_1517E05C(s32 arg0, s32 arg1, s32 arg2) {
    D_800DD470.unk0 = arg0;
    D_800DD470.unk2 = arg1;
    D_800DD470.unk4 = arg2;
}
