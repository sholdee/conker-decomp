#include <ultra64.h>

#define func_1502B7F0 func_1502B7F0__proto
#include "functions.h"
#undef func_1502B7F0
#include "variables.h"

#include "macros.h"

typedef struct func_15002560_node {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
} func_15002560_node;

typedef struct Point16 {
    s16 x;
    s16 y;
    s16 z;
} Point16;

typedef struct Face16 {
    Point16 *pts[3];
} Face16;

typedef struct Point32 {
    f32 x;
    f32 y;
    f32 z;
} Point32;

typedef struct Face32 {
    Point32 pts[3];
} Face32;

typedef struct YRange {
    s16 min;
    s16 max;
} YRange;

extern s32 D_800DBE44;

void func_15002560(u16 *arg0, u16 *arg1);
void func_15002008(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 *arg4);
s32 func_15049260(Face32 arg0);

void func_15000AC0(void) {
    D_800D9E64 = (u8)0;
}

// jump table
extern u8 D_800DBEA8[];
extern Vtx *D_800DBE88[][2];
extern s32 *D_800DBE80;
extern s32 *D_800891BC[];
extern void *D_800DBEB0;
extern u8 D_800DBEAB;
extern u8 D_800DBEAC;
s32 func_1502B7F0();
extern s32 func_1510D0EC(s32, s32, s32, s32);
void func_15111858(void);

void func_15000AD0(s32 arg0) {
    s32 i;
    s32 j;
    s32 n;
    s32 start;
    s32 size;
    s32 from[3];
    s32 to[3];
    s32 cur[3];
    s32 color;
    s32 *buf;
    Vtx *vtx;

    D_800DBEA8[0] = D_800B0DF0->unk5;
    D_800DBEA8[1] = D_800B0DF0->unk6;
    D_800DBEA8[2] = D_800B0DF0->unk7;
    if (D_800BE616 != 0 && D_800B0DF0->unk8 == 4) {
        D_800B0DF0->unk8 = 2;
    }
    if (D_800B0DF0->unk8 == 4) {
        size = (D_80082FA0 != 1) ? 0x130 : 0x1C0;
        for (i = 0; i <= D_80082FA0; i++) {
            vtx = (Vtx *)allocate_memory(size * 32, 1, 2, 0);
            D_800DBE88[i][0] = vtx;
            D_800DBE88[i][1] = D_800DBE88[i][0] + size;
        }
        func_1502B7F0(&D_800DBE80, 2, 0xD, D_800B0DF0->unk9);
        if (D_80038080 != 0) {
            for (i = 0; i != 0x168; i++) {
                func_1510D0EC(D_800891BC[D_800B0DF0->unk9][0] + i, 0, 0x3F, 0);
            }
        }
        D_800DBEB0 = (void *)allocate_memory((D_80082FA0 + 1) * 0x80, 1, 2, 0);
        if (D_800BE9F0 == 0x3C) {
            D_800DBEAB = 1;
        } else {
            D_800DBEAB = 0;
        }
    } else if (D_800B0DF0->unk8 == 2) {
        D_800DBE80 = (s32 *)allocate_memory(0x1000, 1, 1, 0);
        buf = D_800DBE80;
        switch (D_800BE9F0) {
            case 0:
            case 41:
            case 45:
            case 48:
                from[0] = 0;
                from[1] = 0;
                from[2] = 0xBF;
                n = 0xA7;
                to[0] = 0xD0;
                to[1] = 0xD5;
                to[2] = 0xFF;
                break;
            case 43:
                from[0] = 0;
                from[1] = 0;
                from[2] = 0;
                n = 0x85;
                to[0] = 0;
                to[1] = 0;
                to[2] = 0x5D;
                break;
            case 36:
                from[0] = 0;
                from[1] = 0;
                from[2] = 0;
                n = 0x7B;
                to[0] = 0;
                to[1] = 0;
                to[2] = 0x1F;
                break;
            default:
                from[0] = 0;
                from[1] = 0x67;
                from[2] = 0x38;
                n = 0x23;
                to[0] = 0xC8;
                to[1] = 0xD5;
                to[2] = 0x2E;
                break;
        }
        for (i = 0; i < n; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = (to[j] - from[j]) * i / n + from[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        switch (D_800BE9F0) {
            case 0:
            case 41:
            case 45:
            case 48:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = n;
                n = 0xB1;
                to[0] = 0x12;
                to[1] = 0x4E;
                to[2] = 0x19;
                break;
            case 43:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = n;
                n = 0x9C;
                to[0] = 0x1C;
                to[1] = 0x15;
                to[2] = 0x93;
                break;
            case 36:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = n;
                n = 0xA2;
                to[0] = 0x3;
                to[1] = 0x1C;
                to[2] = 0x36;
                break;
            default:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = n;
                n = 0x63;
                to[0] = 0xFF;
                to[1] = 0x3E;
                to[2] = 0x12;
                break;
        }
        for (i = start; i < n; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = (to[j] - from[j]) * (i - start) / (n - start) + from[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        switch (D_800BE9F0) {
            case 0:
            case 41:
            case 45:
            case 48:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = n;
                n = 0xDD;
                to[0] = 0;
                to[1] = 0xC7;
                to[2] = 0;
                break;
            case 43:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = n;
                n = 0xB4;
                to[0] = 0xA;
                to[1] = 0x22;
                to[2] = 0xFF;
                break;
            case 36:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = n;
                n = 0xB7;
                to[0] = 0x21;
                to[1] = 0x24;
                to[2] = 0x31;
                break;
            default:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = n;
                n = 0xC8;
                to[0] = 0;
                to[1] = 0;
                to[2] = 0;
                break;
        }
        for (i = start; i < n; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = (to[j] - from[j]) * (i - start) / (n - start) + from[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        for (i = n; i < 0x100; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = to[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        D_800DBEB0 = (void *)allocate_memory((D_80082FA0 + 1) * 0x80, 1, 1, 0);
        for (i = 0; i <= D_80082FA0; i++) {
            vtx = (Vtx *)allocate_memory(0x80, 1, 1, 0);
            D_800DBE88[i][0] = vtx;
            D_800DBE88[i][1] = vtx + 4;
            for (j = 0; j < 2; j++) {
                D_800DBE88[i][j][0].v.ob[0] = -0x6D6;
                D_800DBE88[i][j][0].v.ob[1] = 0x6D6;
                D_800DBE88[i][j][0].v.ob[2] = -1000;
                D_800DBE88[i][j][0].v.flag = 0;
                D_800DBE88[i][j][0].v.tc[0] = 0x2000;
                D_800DBE88[i][j][1].v.ob[0] = 0x6D6;
                D_800DBE88[i][j][1].v.ob[1] = 0x6D6;
                D_800DBE88[i][j][1].v.ob[2] = -1000;
                D_800DBE88[i][j][1].v.flag = 0;
                D_800DBE88[i][j][1].v.tc[0] = 0x2000;
                D_800DBE88[i][j][2].v.ob[0] = 0x6D6;
                D_800DBE88[i][j][2].v.ob[1] = -0x6D6;
                D_800DBE88[i][j][2].v.ob[2] = -1000;
                D_800DBE88[i][j][2].v.flag = 0;
                D_800DBE88[i][j][2].v.tc[0] = 0x2000;
                D_800DBE88[i][j][3].v.ob[0] = -0x6D6;
                D_800DBE88[i][j][3].v.ob[1] = -0x6D6;
                D_800DBE88[i][j][3].v.ob[2] = -1000;
                D_800DBE88[i][j][3].v.flag = 0;
                D_800DBE88[i][j][3].v.tc[0] = 0x2000;
            }
        }
    } else if (D_800B0DF0->unk8 == 1) {
        func_15111858();
    }
    if (D_800D2E4C->unk14 & 1) {
        D_800DBEAC = 1;
    } else {
        D_800DBEAC = 0;
    }
}
// loops!
typedef struct XZRange {
    s16 minx;
    s16 minz;
    s16 maxx;
    s16 maxz;
} XZRange;

typedef struct GfxW {
    u32 w0;
    u32 w1;
} GfxW;

extern s32 D_800DBE40;

void func_15001460(GfxW *arg0)
{
  s32 i;
  GfxW *gfx;
  s32 cmd;
  s32 cnt;
  s32 s;
  s32 n;
  s32 j;
  s32 k;
  u32 addr;
  s32 m;
  Point16 *vtx[31];
  s16 num;
  s16 start;
  s16 tris[4][3];
  s16 min[3];
  s16 max[3];
  if (arg0 == 0)
  {
    return;
  }
  {
    cnt = D_800DBE38;
    for (i = 0; (*((s8 *) (&arg0[i]))) != (-0x21); i++)
    {
      gfx = &arg0[i];
      n = 0;
      cmd = *((s8 *) gfx);
      s = 0;
      if ((cmd >> 4) == 1)
      {
        tris[0][0] = (gfx->w1 >> 25) & 0x1F;
        tris[0][1] = (gfx->w1 >> 20) & 0x1F;
        tris[0][2] = (gfx->w1 >> 15) & 0x1F;
        tris[1][0] = (gfx->w1 >> 10) & 0x1F;
        tris[1][1] = (gfx->w1 >> 5) & 0x1F;
        tris[1][2] = gfx->w1 & 0x1F;
        tris[2][0] = (gfx->w0 >> 10) & 0x1F;
        tris[2][1] = (gfx->w0 >> 5) & 0x1F;
        tris[2][2] = gfx->w0 & 0x1F;
        tris[3][0] = (gfx->w0 >> 23) & 0x1F;
        tris[3][1] = (gfx->w0 >> 18) & 0x1F;
        tris[3][2] = ((gfx->w1 >> 30) & 3) | ((gfx->w0 >> 13) & 0x1C);
        n = 4;
      }
      else
        if (cmd == 6)
      {
        tris[0][0] = (gfx->w0 >> 17) & 0x1F;
        tris[0][1] = (gfx->w0 >> 9) & 0x1F;
        tris[0][2] = (gfx->w0 >> 1) & 0x1F;
        tris[1][0] = (gfx->w1 >> 17) & 0x1F;
        tris[1][1] = (gfx->w1 >> 9) & 0x1F;
        tris[1][2] = (gfx->w1 >> 1) & 0x1F;
        n = 2;
      }
      else
        if (cmd == 1)
      {
        addr = gfx->w1;
        if ((addr >> 24) & 0xF)
        {
          addr = ((&D_800D2C28)[(addr >> 24) & 0xF] + addr) - (D_800D2C68 << 24);
        }
        num = (((s32) gfx->w0) >> 12) & 0xFF;
        start = (((s32) gfx->w0) >> 1) & 0x7F;
        start -= num;
        for (m = start; m < num; m++)
        {
          vtx[m] = (Point16 *) addr;
          addr += 16;
        }

      }
      else
        if (cmd == 5)
      {
        for (k = 0; k < 3; k++)
        {
          tris[0][k] = ((u8 *) gfx)[k + 1] >> 1;
        }

        n = 1;
      }
      for (s = 0; s < n; s++)
      {
        if (((u32) vtx[0]) > 0x100000)
        {
          for (cmd = 0; cmd < 3; cmd++)
          {
            if (D_800DBE63 == 0)
            {
              min[cmd] = (max[cmd] = (&vtx[tris[s][0]]->x)[cmd]);
              for (k = 1; k < 3; k++)
              {
                if ((&vtx[tris[s][k]]->x)[cmd] < min[cmd])
                {
                  min[cmd] = (&vtx[tris[s][k]]->x)[cmd];
                }
                else
                  if (max[cmd] < (&vtx[tris[s][k]]->x)[cmd])
                {
                  max[cmd] = (&vtx[tris[s][k]]->x)[cmd];
                }
                if ((&vtx[tris[s][k]]->x)[cmd] < min[cmd])
                {
                  min[cmd] = (&vtx[tris[s][k]]->x)[cmd];
                }
                else
                  if (max[cmd] < (&vtx[tris[s][k]]->x)[cmd])
                {
                  max[cmd] = (&vtx[tris[s][k]]->x)[cmd];
                }
              }

            }
            else
            {
              min[cmd] = -0x8000;
              max[cmd] = 0x7FFF;
            }
          }

        }
        for (j = 0; j < 3; j++)
        {
          ((Face16 *) D_800DBE3C)[cnt].pts[j] = vtx[tris[s][j]];
        }

        ((XZRange *) D_800DBE40)[cnt].minx = min[0];
        ((XZRange *) D_800DBE40)[cnt].maxx = max[0];
        ((XZRange *) D_800DBE40)[cnt].minz = min[2];
        ((XZRange *) D_800DBE40)[cnt].maxz = max[2];
        ((YRange *) D_800DBE44)[cnt].min = min[1];
        ((YRange *) D_800DBE44)[cnt].max = max[1];
        cnt++;
      }

    }

    D_800DBE38 = cnt;
  }
}

void func_15001970(void) {
    s32 pad;
    s32 tmp;

    D_800B0DE4 = 0;
    D_800DBE63 = 0;
    *D_800DBE18 = 0;
    *D_800DBE1C = 0;
    D_800DBE20 = 0;
    tmp = D_80091C04[D_800BE9F0];
    D_800DBE24 = 0;
    D_800B0DC0 = allocate_memory(tmp, 1, 0, 0);
    D_800B0DC8 = tmp + D_800B0DC0;
    D_800B0DC4 = D_800B0DC0;
}

void __osPiGetAccess(void);
void __osPiRelAccess(void);

void func_15001A08(void) {
    s32 size;
    u32 stat;
    s32 id;

    size = D_80091AF0[D_800BE9F0];

    __osPiGetAccess();
    stat = IO_READ(PI_STATUS_REG);
    while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY)) {
        stat = IO_READ(PI_STATUS_REG);
    }
    id = IO_READ(D_80000308 | 0xB0000D24);
    __osPiRelAccess();

    if (id != 0x98CCE31A) {
        size <<= 1;
    }

    D_800B0DDC = allocate_memory(size, 1, 0, 0);
    D_800B0DE0 = D_800B0DDC;
    D_800B0DCC = 0;
    D_800B0DD0 = 0;
    D_800DBE30 = 0;
    D_800DBE32 = 0;
    D_800DBE34 = 0;
    D_800DBE36 = 0;
}

void func_15001B08(void) {
}

u16* func_15001B10(void) {
    u16 *temp_v0;

    D_800B0DE0 = ALIGN4(D_800B0DE0);
    D_800B0DCC += 1;

    temp_v0 = (u16*)D_800B0DE0;
    temp_v0[6] = 0;
    temp_v0[2] = 0;

    D_800B0DE0 += 14;
    return temp_v0;
}

void func_15001B5C(u8 arg0) {
    *D_800B0DE0++ = arg0;
}

void func_15001B8C(u16 arg0) {
   D_800B0DE0[0] = (arg0 >> 8);
   D_800B0DE0[1] = arg0 & 0xFF;
   D_800B0DE0 += 2;
}

void func_15001BC8(s32 arg0, s16 *arg1, s16 *arg2) {
    u16 *tmp;
    s16 *sp20;
    s32 i;

    func_1510F800(arg0);
    D_800B0DD8 = 10;
    D_800B0DD4 = 512;
    sp20 = allocate_memory((D_800DBE4C + 1) << 1, 1, 0, 0);

    *sp20 = D_800DBE4C;
    for (i = 0; i < D_800DBE4C; i++) {
        sp20[i + 1] = i;
    }

    tmp = func_15001DE0(0, 0, 32768, 32768, 0, sp20);
    D_800DBE08[arg0] = tmp;
    func_15002560(tmp, 0);
    func_10004074(sp20);

    // regalloc fix:
    if (1) { }

    if (arg1 != 0) {
        *arg1 = D_800B0DCC;
    }
    if (arg2 != 0) {
        *arg2 = D_800B0DD0;
    }
}

void func_15001CEC(s32 arg0) {
    u16 *temp_v0;
    s16 *sp20;
    s32 i;

    func_1510F800(arg0);
    temp_v0 = func_15001B10();

    D_800DBE08[arg0] = temp_v0;

    *((s16*)temp_v0 + 0) = 32768;
    temp_v0[1] = 32767;
    temp_v0[2] = 0;
    temp_v0[3] = -32768;
    temp_v0[4] = 0;
    temp_v0[5] = 0;
    temp_v0[6] = 0;

    sp20 = allocate_memory((D_800DBE4C + 1) << 1, 1, 0, 0);

    *sp20 = D_800DBE4C;
    for (i = 0; i < D_800DBE4C; i++) {
        sp20[i + 1] = i;
    }

    func_15002008(-0x8000, 0x8000, -0x8000, 0x8000, sp20);
    func_10004074(sp20);
}

u16 *func_15001DE0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    u16 *temp_v0;
    u16 *temp_v0_2;
    u16 *temp_v0_3;
    u16 *temp_v0_4;

    s32 sp7C;
    s32 sp78;

    s32 tmp0;
    s32 tmp1;

    s32 sp44;
    s32 sp48;
    s32 sp4C;
    s32 sp50;
    s32 sp54;
    s32 sp58;

    tmp0 = D_800B0DD8;
    tmp1 = D_800B0DD4;

    sp54 = arg0 - arg2;
    sp50 = arg0 + arg2;
    sp4C = arg1 - arg3;
    sp48 = arg1 + arg3;

    if ((func_15002248(sp54, sp50, sp4C, sp48, &sp7C, &sp78, arg5, &sp58) < tmp0) || (arg2 < tmp1)) {
        temp_v0 = func_15001B10();
        D_800B0DD0 = (D_800B0DD0 + 1);
        temp_v0[0] = sp7C;
        temp_v0[1] = sp78;
        temp_v0[3] = arg2;
        temp_v0[4] = arg0;
        temp_v0[5] = arg1;
        temp_v0[2] = 0;
        temp_v0[6] = 0;
        func_15002008(sp54, sp50, sp4C, sp48, sp58);
    } else {
        temp_v0 = func_15001B10();
        temp_v0[0] = sp7C;
        temp_v0[1] = sp78;
        temp_v0[3] = arg2;
        temp_v0[4] = arg0;
        temp_v0[5] = arg1;
        temp_v0[2] = 0;

        temp_v0_2 = func_15001DE0(arg0 - (arg2 >> 1), arg1 - (arg3 >> 1), arg2 >> 1, arg3 >> 1, 0, sp58);
        temp_v0[6] = (s32)temp_v0_2 - (s32)temp_v0;
        temp_v0_3 = func_15001DE0(arg0 - (arg2 >> 1), (arg3 >> 1) + arg1, arg2 >> 1, arg3 >> 1, 0, sp58);
        temp_v0_2[2] = (s32)temp_v0_3 - (s32)temp_v0_2;
        temp_v0_4 = func_15001DE0((arg2 >> 1) + arg0, arg1 - (arg3 >> 1), arg2 >> 1, arg3 >> 1, 0, sp58);
        temp_v0_3[2] = (s32)temp_v0_4 - (s32)temp_v0_3;
        temp_v0_4[2] = (s32)func_15001DE0((arg2 >> 1) + arg0, (arg3 >> 1) + arg1, arg2 >> 1, arg3 >> 1, 0, sp58) - (s32)temp_v0_4;
    }
    func_10004074(sp58);
    return temp_v0;
}

void func_15002008(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 *arg4) {
    Face16 *face;
    s32 count;
    s32 idx;
    s32 last;
    s32 diff;
    s32 i;
    s32 j;
    Face32 sp78;

    last = -1;
    count = arg4[0];

    func_150492CC((f32)(arg1 - arg0), 32000.0f, (f32)(arg3 - arg2));

    for (i = 1; i <= count; i++) {
        idx = arg4[i];
        face = &((Face16 *)D_800DBE3C)[idx];

        for (j = 0; j < 3; j++) {
            if ((u32)face->pts[j] > 0x80000000) {
                sp78.pts[j].x = (f32)(face->pts[j]->x - (arg0 + arg1) / 2);
                sp78.pts[j].y = (f32)face->pts[j]->y;
                sp78.pts[j].z = (f32)(face->pts[j]->z - (arg2 + arg3) / 2);
            } else {
                sp78.pts[j].x = 0.0f;
                sp78.pts[j].y = 0.0f;
                sp78.pts[j].z = 0.0f;
            }
        }

        if (func_15049260(sp78) == 0) {
            diff = idx - last;
            if (last == -1) {
                func_15001B8C(idx | 0x8000);
            } else if (diff < 0x80) {
                func_15001B5C(diff);
            } else {
                func_15001B8C(idx | 0x8000);
            }
            last = idx;
        }
    }

    func_15001B5C(0);
}

int func_15002248(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, s32 *arg5, u16 *arg6, u16 **arg7) {
    s32 i;
    s32 j;
    s32 count;
    s32 total;
    s32 idx;
    u16 *out;
    Face16 *face;
    s32 temp;
    s32 flag;
    s32 res;
    s32 pad;
    s32 mask;
    Face32 sp74;

    pad = (1 << (D_80082FA0 + 1)) - 1;
    mask = -(1 << (D_80082FA0 + 1));

    func_150492CC((f32)(arg1 - arg0), 32000.0f, (f32)(arg3 - arg2));

    total = arg6[0] + 1;
    count = 0;
    out = allocate_memory(total << 1, 1, 0, 0);
    *arg7 = out;
    *arg4 = 0;
    *arg5 = 0;
    flag = 0;

    for (i = 1; i < total; i++) {
        idx = arg6[i];
        face = &((Face16 *)D_800DBE3C)[idx];

        for (j = 0; j < 3; j++) {
            if ((u32)face->pts[j] > 0x80000000) {
                sp74.pts[j].x = (f32)(face->pts[j]->x - ((arg0 + arg1) >> 1));
                sp74.pts[j].y = (f32)face->pts[j]->y;
                sp74.pts[j].z = (f32)(face->pts[j]->z - ((arg2 + arg3) >> 1));
            } else {
                sp74.pts[j].x = 0.0f;
                sp74.pts[j].y = 0.0f;
                sp74.pts[j].z = 0.0f;
            }
        }

        res = func_15049260(sp74);
        if (res == 0) {
            count++;
            out[count] = idx;

            if (flag) {
                temp = (((YRange *)D_800DBE44)[idx].min - pad) & mask;
                if (temp < *arg4) {
                    *arg4 = temp;
                }
                temp = ((((YRange *)D_800DBE44)[idx].max + pad) & mask) + pad;
                if (*arg5 < temp) {
                    *arg5 = temp;
                }
            } else {
                flag = 1;
                *arg4 = (((YRange *)D_800DBE44)[idx].min - pad) & mask;
                *arg5 = ((((YRange *)D_800DBE44)[idx].max + pad) & mask) + pad;
            }
        }
    }

    out[0] = count;
    return count;
}

void func_15002560(u16 *arg0, u16 *arg1) {
    s32 temp_v0;
    func_15002560_node *temp_s0;
    func_15002560_node *var_s1;

loop:
    if (arg0 != 0) {
        if (((func_15002560_node *)arg0)->unk4 == 0) {
            if (arg1 != 0) {
                temp_v0 = (s32)arg1 - (s32)arg0;
            } else {
                temp_v0 = 0;
            }
            ((func_15002560_node *)arg0)->unk4 = temp_v0;
        }
        temp_v0 = ((func_15002560_node *)arg0)->unkC;
        if (temp_v0 == 0) {
            return;
        }
        var_s1 = (func_15002560_node *)((u8 *)arg0 + temp_v0);
        if (var_s1->unk4 != 0) {
            do {
                temp_s0 = (func_15002560_node *)((u8 *)var_s1 + var_s1->unk4);
                func_15002560((u16 *)var_s1, (u16 *)temp_s0);
                var_s1 = temp_s0;
            } while (temp_s0->unk4 != 0);
        }
        arg0 = (u16 *)var_s1;
        goto loop;
    }
}

void func_150025FC(void) {
    s32 tmp0;
    s32 tmp1;
    s32 tmp2;
    s32 tmp3;

    tmp0 = D_800DBE2A;
    tmp1 = D_800DBE2C;

    D_800DBE28 = (((tmp0 + 7) / 8) + 0xF) & 0xFFF0;
    tmp2 = func_1502B5C8(0, 2, 5, D_800BE9F0 << 1);
    tmp3 = func_1502B5C8(0, 2, 5, (D_800BE9F0 << 1) + 1);
    if (tmp2 == 0) {
        D_800BE5D0 = 0;
    } else {
        func_150183D0(D_800DBE28, tmp1, tmp2, tmp3);
        func_10004074(tmp3);
    }
}

void func_150026C4(s32 arg0) {
    D_800DBE38 = 0;
    func_1510F800(arg0);
}

void func_150026E8(s32 arg0) {
    D_800DBE18[arg0] = D_800DBE38;
    D_800DBE4C = D_800DBE38;
    D_800B0DE4 += D_800DBE38;
}

void func_15002724(s32 arg0) {
    D_800DBE38 += func_150027F8(arg0);
}

void func_15002754(void) {
    s32 count;
    s32 idx;

    D_800DBDD8[D_800DBE50] = D_800B0DC0 = ALIGN4(D_800B0DC0);
    count = D_800DBE38;
    idx = D_800DBE50;
    D_800B0DC0 += count * 12;
    D_800DBDE8[idx] = D_800B0DC0;
    D_800B0DC0 += count * 8;
    D_800DBDF8[idx] = D_800B0DC0;
    D_800B0DC0 += count * 4;
    func_1510F800(idx);
    D_800DBE38 = 0;
}


int func_150027F8(s32 arg0) {
    s32 var_v0;
    s32 var_v1;
    s32 temp_a1;
    if (arg0 != 0) {
        goto non_null;
    }
    return 0;
non_null:
    var_v0 = 0;
    var_v1 = 0;
    temp_a1 = *(s8 *)arg0;
    if (temp_a1 != -0x21) {
        do {
            var_v0++;
            if ((temp_a1 >> 4) == 1) {
                var_v1 += 4;
            } else if (temp_a1 == 6) {
                var_v1 += 2;
            } else if (temp_a1 == 5) {
                var_v1 += 1;
            }
            temp_a1 = *(s8 *)(arg0 + (var_v0 << 3));
        } while (temp_a1 != -0x21);
    }
    return var_v1;
}

s32 func_15002878(void) {
    s32 i;
    s32 tmp;
    s32 highest;

    highest = *D_800DBE18;

    for (i = 0; i < 3; i++) {
        tmp = D_800DBE1C[i];
        if (highest < tmp) {
            highest = tmp;
        }
    }

    return highest;
}

// 12 loops!
typedef struct F28BC_Ent {
    u32 addr;
    s32 count;
    u32 extra;
} F28BC_Ent;

typedef struct F28BC_Tri {
    s32 v[3];
} F28BC_Tri;

extern u16 D_800C5918[];
extern u16 D_800C5EF8[];
extern s8 **D_800C4488[];

#define CMD (ncmd + (j << 3))

#define PE ((F28BC_Ent **)&D_800C5C08[arg0])
#define PT ((F28BC_Tri **)&D_800C6070[arg0])
s32 func_150028BC(s32 arg0) {
    F28BC_Ent *e;
    s32 vaddr;
    s32 n;
    u32 c;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 cnt;
    s32 nent;
    s32 nout;
    u32 attr;
    s8 *ncmd;
    s32 base;
    s32 verts[32];
    s8 *cmd;
    s32 x;
    s32 start;
    s32 idx[3];
    F28BC_Ent tmp;

    attr = 0;
    if (*PE != NULL) {
        return 0;
    }
    n = 1;
    cnt = 0;
    for (i = 0; i < n; i++) {
        cmd = D_800C4488[arg0][i];
        j = 0;
        while (cmd[j << 3] != -0x21) {
            if (cmd[j << 3] == 1) {
                cnt++;
            }
            j++;
        }
    }
    *PE = (F28BC_Ent *)allocate_memory(cnt * sizeof(F28BC_Ent), 1, 0, 2);
    if (*PE == NULL) {
        return 1;
    }
    *PT = (F28BC_Tri *)allocate_memory(D_800C5918[arg0] * sizeof(F28BC_Tri), 1, 0, 2);
    if (*PT == NULL) {
        func_10004074((s32)*PE);
        *PE = NULL;
        return 1;
    }
    nent = 0;
    nout = 0;
    base = (&D_800D19A0)[arg0];
    for (i = 0; i < n; i++) {
        ncmd = D_800C4488[arg0][i];
        j = 0;
        while (*CMD != -0x21) {
            x = *(u8 *)CMD;
            k = 0;
            if (x == 1) {
                {
                    extern u16 D_800C5918[];

                    cnt = (*(s32 *)CMD >> 12) & 0xFF;
                    start = (*(s32 *)CMD >> 1) & 0x7F;
                    start -= cnt;
                    vaddr = (*(s32 *)(CMD + 4) & 0xFFFFFF) - 0x38;
                    c = (u32)((*(s32 *)(CMD + 4) - 0x38) & 0xFFFFFF) >> 4;
                    for (k = 0; k < cnt; k++) {
                        verts[k + start] = (k << 4) + vaddr;
                    }
                    if (nent != 0 && base + c * 16 == (*PE)[nent - 1].addr + (*PE)[nent - 1].count * 16 &&
                        attr == (*PE)[nent - 1].extra) {
                        (*PE)[nent - 1].count += cnt;
                    } else {
                        (*PE)[nent].addr = c * 16 + base;
                        (*PE)[nent].extra = attr;
                        (*PE)[nent].count = cnt;
                        nent++;
                    }
                }
            } else if (x == 0xDA) {
                attr = *(u32 *)(CMD + 4) & 0xFFFFFF;
                attr >>= 6;
            } else if (x == 5) {
                for (k = 0; k < 3; k++) {
                    vaddr = *(u8 *)(CMD + k + 1);
                    (*PT)[nout].v[k] = verts[vaddr >> 1];
                }
                nout++;
            } else if (x == 6) {
                for (m = 0; m < 2; m++) {
                    for (k = 0; k < 3; k++) {
                        vaddr = *(u8 *)(CMD + m * 4 + k + 1);
                        (*PT)[nout].v[k] = verts[vaddr >> 1];
                    }
                    nout++;
                }
            } else if ((x >> 4) == 1) {
                for (m = 0; m < 4; m++) {
                    if (m == 0) {
                        idx[0] = (*(s32 *)(CMD + 4) >> 25) & 0x1F;
                        idx[1] = (*(s32 *)(CMD + 4) >> 20) & 0x1F;
                        idx[2] = (*(s32 *)(CMD + 4) >> 15) & 0x1F;
                    } else if (m == 1) {
                        idx[0] = (*(s32 *)(CMD + 4) >> 10) & 0x1F;
                        idx[1] = (*(s32 *)(CMD + 4) >> 5) & 0x1F;
                        idx[2] = *(s32 *)(CMD + 4) & 0x1F;
                    } else if (m == 2) {
                        idx[0] = (*(s32 *)CMD >> 10) & 0x1F;
                        idx[1] = (*(s32 *)CMD >> 5) & 0x1F;
                        idx[2] = *(s32 *)CMD & 0x1F;
                    } else {
                        idx[0] = (*(s32 *)CMD >> 23) & 0x1F;
                        idx[1] = (*(s32 *)CMD >> 18) & 0x1F;
                        idx[2] = ((*(s32 *)CMD >> 13) & 0x1C) | ((*(s32 *)(CMD + 4) >> 30) & 3);
                    }
                    for (k = 0; k < 3; k++) {
                        (*PT)[nout].v[k] = verts[idx[k]];
                    }
                    nout++;
                }
            }
            j++;
        }
    }
    D_800C5EF8[arg0] = nent;
    j = 1;
loop:
    if (j < D_800C5EF8[arg0]) {
        e = &(*PE)[j];
        if (e->addr < e[-1].addr) {
            tmp = *e;
            *e = e[-1];
            (*PE)[j - 1] = tmp;
            if (j >= 2) {
                j--;
            }
        } else {
            j++;
        }
        goto loop;
    }
    return 0;
}
#undef PE
#undef PT
#undef CMD

void func_15002F40(s32 arg0) {
    if (arg0 == 50 || arg0 == 51) {
        D_800DBE64 = 150.0f;
        return;
    }
    if (D_800BE616) {
        D_800DBE64 = 85.0f;
        return;
    }
    D_800DBE64 = 50.0f;
}

s32 func_15002FA0(s32 arg0) {
    return arg0 ^ 0xF0F0F0F0;
}

void func_15002FB4(s32 arg0) {
    struct00 *sp3C; // TBD
    s32 sp38;
    s32 sp34;
    struct104 *temp_v0;
    s32 sp2C;

    sp3C = func_1502B6BC(&sp34, 0, &sp38, 2, 21, arg0);
    if ((sp38 != 0) && ((sp3C->unk4 << 8) != 0)) {
        D_800B0DF0 = sp3C->unk0;
        if (sp34 == 0) {
            D_8003C8E0 = 0xC000037;
            func_150AD770();
        }
        if (D_800B0DF0 == 0) {
            D_8003C8E0 = 0xC000037;
            func_150AD770();
        }
    }
    func_151EF040(func_15002FA0(0x40F0FAAC), &sp2C);
    sp2C = func_15002FA0(sp2C);
    if (sp2C != 0xE7B00F09) {
        temp_v0 = D_800B0DF0;
        temp_v0->unk12 >>= 1;
    }

    temp_v0 = D_800B0DF0;
    if (temp_v0->unk12 != 0) {
        D_800D2F3C = temp_v0->unk12;
        D_800D2F40 = sp3C->unk8;
    } else {
        D_800D2F3C = 0U;
        D_800D2F40 = 0;
    }

    if ((D_8008FD90 >= 2) && (temp_v0->unk8 == 4)) {
        temp_v0->unk8 = 2U;
    }
}

void func_1500310C(void) {
}
