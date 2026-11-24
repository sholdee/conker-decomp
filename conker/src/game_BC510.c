#include <ultra64.h>

#define func_15048A40 func_15048A40_hdr
#define func_150ADA20 func_150ADA20_hdr
#define func_150A7CB0 func_150A7CB0_hdr
#include "functions.h"
#undef func_15048A40
#undef func_150ADA20
#undef func_150A7CB0
void func_150A7CB0(f32 mtx[4][4], f32 x, f32 y, f32 z);
#include "variables.h"
extern s32 func_150ADA20(void);
void func_1508F9F4(void);
void func_1508F0D4(void);
void func_1508F7BC(void);
extern u8 D_800CC49A;

extern s32 func_1510D0EC(s32, s32, s32, s32);
void func_15042D94(s32, s32, u8, s32 *, ...);
extern u8 D_D16[];

// need to figure out D_800D2460
void func_1508F060(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        D_800D2460[i][13] = 0;
    }
    D_800D24C0 = 0;
}

void func_1508F0A4(void) {
    func_1508F0D4();
    func_1508F9F4();
    func_1509093C();
}

void func_1508F0D4(void)
{
  s32 t;
  s32 i;
  s32 j;
  s32 pad1;
  s32 n;
  struct256 *p;
  struct256 **pp;
  s32 pad2;
  f32 cnt;
  f32 target;
  f32 base;
  f32 d;
  f32 cur;
  f32 inc;
  pp = &D_800D245C;
  p = D_800D245C;
  while (p != 0)
  {
    p->unk94 -= 5.0f;
    p->unk84 += p->unk90;
    p->unk88 += p->unk94;
    p->unk8C += p->unk98;
    p->unk9C += p->unkA8;
    p->unkA0 += p->unkAC;
    p->unkA4 += p->unkB0;
    t = p->unkB4;
    t -= D_800BE9E4;
    if (t <= 0)
    {
      *pp = p->unk80;
      func_100043B4((s32 *) p, 3);
    }
    else
    {
      p->unkB4 = t;
      pp = &p->unk80;
    }
    p = p->unk80;
  }

  if (D_800CC49A < D_800D2457)
  {
    if (p->unkA8)
    {
    }
    D_800D2444 = 0xF0;
    while (D_800CC49A != D_800D2457)
    {
      D_800D2457--;
      if (D_800D2428[0] == ((f32) D_8009DCB4[3]))
      {
        func_1508F7BC();
      }
    }

    func_10010F30(0x1A6, 0x7FFF, 0x40, 0, 0);
  }
  else
    if (D_800D2457 < D_800CC49A)
  {
    D_800D2444 = 0xF0;
    while (D_800CC49A != D_800D2457)
    {
      D_800D2457++;
      if (D_800D2428[0] == ((f32) D_8009DCB4[3]))
      {
        func_1508F9C4();
      }
    }

  }
  if (D_800D2444 != 0)
  {
    cur = D_800D2428[0];
    target = (f32) D_8009DCB4[3];
    if (cur == target)
    {
      D_800D2444 -= D_800BE9E4;
      if (D_800D2444 <= 0)
      {
        D_800D2444 = 0;
      }
      for (j = 0; j < 4; j += 2)
      {
        for (i = 0; i < D_800BE9E4; i++)
        {
          ((f32 *) (&D_800D2890))[j] += D_800D2438[j] * (-0.1000000015f);
          ((f32 *) (&D_800D2890))[j] *= 0.8500000238f;
          D_800D2438[j] += ((f32 *) (&D_800D2890))[j] * ((f32) D_800BE9E4);
        }

      }

    }
    else
      if (D_800D2457 < D_800D2456)
    {
      n = D_800D2456 - D_800D2457;
      D_800D2428[0] += 50.0f * ((f32) D_800BE9E4);
      if (target <= D_800D2428[0])
      {
        D_800D2428[0] = target;
        for (i = 0; i < n; i++)
        {
          func_1508F7BC();
        }

        func_10010F30(0x1A6, 0x7FFF, 0x40, 0, 0);
      }
    }
    else
    {
      n = D_800D2457 - D_800D2456;
      D_800D2428[0] += 50.0f * ((f32) D_800BE9E4);
      if (target <= D_800D2428[0])
      {
        D_800D2428[0] = target;
        for (i = 0; i < n; i++)
        {
          func_1508F9C4();
        }

      }
    }
  }
  else
  {
    cur = D_800D2428[0];
    base = (f32) D_8009DCB4[0];
    if (D_800D2428[0] != ((f32) D_8009DCB4[0]))
    {
      target = (((f32) D_8009DCB4[3]) - D_800D2428[0]) * 0.1000000015f;
      if (target < 0.009999999776f)
      {
        target = 0.009999999776f;
      }
      D_800D2428[0] -= target * ((f32) D_800BE9E4);
      if (D_800D2428[0] < base)
      {
        D_800D2428[0] = base;
      }
    }
  }
  for (i = 0; i < 6; i++)
  {
    if (D_800D2410[i] != 1.0f)
    {
      inc = D_800BE9E4 * 0.02999999933f;
      D_800D2410[i] += inc;
      if (D_800D2410[i] > 1.0f)
      {
        D_800D2410[i] = 1.0f;
      }
    }
  }

}

void func_1508F7BC(void) {
    f32 sp48[4][4];
    f32 sp44;
    struct256 *temp_v0;

    D_800D2456 -= 1;
    D_800D2890.unk0 = (u32) (func_150ADA20() % 5U);
    D_800D2890.unk8 = sqrtf(25.0f - (D_800D2890.unk0 * D_800D2890.unk0));
    temp_v0 = allocate_memory(sizeof(*temp_v0), 1, 1, 0);
    temp_v0->unk80 = D_800D245C;
    D_800D245C = temp_v0;
    guMtxL2F(&sp48, (D_800D2456 << 7) + ((D_800BE9C0 ^ 1) << 6) + &D_800D2590);
    func_1503E5F8(&sp48, &temp_v0->unk84, &temp_v0->unk88, &temp_v0->unk8C, &temp_v0->unk9C, &temp_v0->unkA0, &temp_v0->unkA4, &sp44, &sp44, &sp44);

    temp_v0->unk90 = (u32)((func_150ADA20() & 0xF) + 10);
    temp_v0->unk94 = (u32)((func_150ADA20() & 0x1F) + 20);
    temp_v0->unk98 = 0.0f;
    temp_v0->unkA8 = (u32)(func_150ADA20() & 0xF);
    temp_v0->unkAC = (u32)(func_150ADA20() & 0xF);
    temp_v0->unkB0 = (u32)(func_150ADA20() & 0xF);
    temp_v0->unkB4 = 120;
}

void func_1508F9C4(void) {
    D_800D2410[D_800D2456++] = 0.0f;
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8 unkC;
    u8 state;
    u8 unkE;
    u8 unkF;
} BC510Bubble;
extern f32 func_15048A40(u8 arg0);

void func_1508F9F4(void) {
    s32 i;
    s32 v;
    s32 flag;
    s32 d;
    s32 n;
    s32 t;
    s32 u;
    s32 j;
    s32 x;

    u = D_800D24C0;
    if (u != 0) {
        u -= D_800BE9E4;
        if (u < 0) {
            u = 0;
        }
        D_800D24C0 = u;
    }
    if (D_800CC2D0[0].in_water) {
        u = D_800CC2D0[0].unkB2;
    } else {
        u = 0;
    }
    if (u > 90) {
        n = u / 240;
        if (n > 6) {
            n = 6;
        }
        n = 6 - n;
        n = n * 9;
        if (n == 0) {
            flag = 0;
        }
    } else {
        flag = 0;
    }
    for (i = 0; i < 6; i++) {
        BC510Bubble *p = (BC510Bubble *)D_800D2460[i];
        if (p->state == 2) {
            v = p->unkA;
            v -= D_800BE9E4;
            if (v <= 0) {
                v = 6;
                p->state = 3;
            }
            p->unkA = v;
        }
    sw:
        switch (p->state) {
        case 0:
    LX: ;
        l0:
            if (flag != 0 && D_800D24C0 == 0) {
                D_800D24C0 = 10;
                p->state = 1;
                p->unk0 = 0xFC;
                p->unk2 = 0x60;
                p->unk6 = 0;
                p->unkC = 0;
                p->unkE = func_150ADA20();
                p->unk8 = 0x500;
                p->unk4 = 0xFC - ((s32)(p->unk8 * func_15048A40(p->unkE)) >> 8);
                p->unkF = 0;
                p->unkA = n;
            }
            break;
        case 1:
            v = p->unk6;
            v += D_800BE9E4 * 200;
            if (v >= 0x666) {
                p->state = 2;
                v = 0x666;
            }
            p->unk6 = v;
            break;
        case 2:
            d = D_800BE9E4;
            j = 0;
            v = p->unkC;
            if (v < 0x20) {
                v = v + d;
                if (v > 0x20) {
                    v = 0x20;
                }
                p->unkC = v;
            }
            x = p->unk2;
            x -= (d * p->unkC) >> 4;
            *(volatile s16 *)&p->unk2 = x;
            t = *(volatile s16 *)&p->unk6;
            if (d > 0) {
                do {
                    j++;
                    t = (t * 0x104) >> 8;
                } while (j < d);
            }
            p->unk6 = t;
            p->unkE += d * 5;
            p->unk8 += d * 30;
            p->unk0 = ((s32)(func_15048A40(p->unkE) * p->unk8) >> 8) + p->unk4;
            p->unkF += D_800BE9E4 * 10;
        lbl:
            break;
        case 3:
            if (p->unkA == 0) {
                p->state = 0;
            }
            break;
        }
    }
}
typedef struct GameBC510Command {
    u32 w0;
    u32 w1;
} GameBC510Command;
GameBC510Command *func_150900F0(GameBC510Command *, s32);
GameBC510Command *func_150918EC(GameBC510Command *);
GameBC510Command *func_150938BC(GameBC510Command *);
GameBC510Command *func_15093B58(GameBC510Command *, s32, s16 *, s32, s32);
GameBC510Command *func_15090630(GameBC510Command *);
GameBC510Command *func_150911F4(GameBC510Command *);
void *func_15094F40(void *);
void *func_15096934(void *);
void func_1518CA04(s32);
extern f32 D_8008FE1C;
extern s32 D_800BEBA4;
extern GameBC510Command *D_800BE9C8[];

GameBC510Command *func_1508FD38(register GameBC510Command *arg0) {
    GameBC510Command *original;
    s32 index;
    s32 step;
    s32 overflow;
    s16 rows[33];

    original = arg0;
    if (D_800BE9F0 == 0x1D) return arg0;
    { GameBC510Command *packet0 = arg0++; packet0->w0 = 0xDB0E0000; packet0->w1 = *(u16 *)&D_800D2454; }
    { GameBC510Command *packet1 = arg0++; packet1->w0 = 0xDA380007; packet1->w1 = (u32)&D_800D23D0; }
    if (D_800BE616 != 0) {
        arg0 = func_150918EC(arg0);
        overflow = D_800BEBA4 < arg0 - *(D_800BE9C8 + D_800BE9C0) ? 1 : 0;
        if (overflow != 0) return original;
        return arg0;
    }
    if (D_800D2458 != 0) arg0 = func_150938BC(arg0);
    if (D_800C35EA == 1) return arg0;
    if (D_8008CD00 != 0) {
        step = 0x400;
        if (D_8008FE1C != 1.0f) {
            step = (s32)((f32)step / D_8008FE1C);
            for (index = 0; index < 33; index++) *(rows + index) = (s32)((f32)index * D_8008FE1C);
        } else {
            for (index = 0; index < 33; index++) *(rows + index) = index;
        }
        arg0 = func_15093B58(arg0, step, rows, 0x2C30, 1);
    }
    if (D_800D2428[0] != (f32)D_8009DCB4[0]) arg0 = func_150900F0(arg0, 0);
    else if (D_800D2588 != 0) {
        func_1518CA04(0xB8);
        D_800D2588 = 0;
    }
    arg0 = func_150911F4(arg0);
    arg0 = func_15094F40(arg0);
    arg0 = func_15090630(arg0);
    arg0 = func_15096934(arg0);
    overflow = D_800BEBA4 < arg0 - *(D_800BE9C8 + D_800BE9C0) ? 1 : 0;
    if (overflow != 0) return original;
    return arg0;
}
extern s16 D_8008FDCC;
extern s16 D_8009DC90[][3];
extern u8 D_800CC49A;
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);

GameBC510Command *func_150900F0(GameBC510Command *arg0, s32 arg1) {
    s32 i;
    s32 count;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    f32 mtx[4][4];
    f32 base[4][4];
    struct256 *p;
    u32 *dl;

    if (D_800D2588 == 0) {
        D_800D2588 = func_1518C900(0xB8);
    }
    { GameBC510Command *c = arg0++; c->w0 = 0xD9F1FDFE; c->w1 = 0; }
    { GameBC510Command *c = arg0++; c->w0 = 0xD7000002; c->w1 = 0xFFFFFFFF; }
    { GameBC510Command *c = arg0++; c->w0 = 0xD9FFFFFF; c->w1 = 0x400; }
    { GameBC510Command *c = arg0++; c->w0 = 0xE7000000; c->w1 = 0; }
    { GameBC510Command *c = arg0++; c->w0 = 0xFA000000; c->w1 = 0xFF; }
    if (arg1 == 0) {
        { GameBC510Command *c = arg0++; c->w0 = 0xEF08AC0F; c->w1 = 0x55204C; }
        { GameBC510Command *c = arg0++; c->w0 = 0xFC127E24; c->w1 = 0xFFFFF9FC; }
        { GameBC510Command *c = arg0++; c->w0 = 0xFB000000; c->w1 = 0; }
    } else {
        { GameBC510Command *c = arg0++; c->w0 = 0xDB0E0000; c->w1 = *(u16 *)&D_800D2454; }
        { GameBC510Command *c = arg0++; c->w0 = 0xDA380007; c->w1 = (u32)&D_800D23D0; }
        { GameBC510Command *c = arg0++; c->w0 = 0xEF08AC0F; c->w1 = 0x5041CC; }
        { GameBC510Command *c = arg0++; c->w0 = 0xFC12D225; c->w1 = 0xFFA7FFFF; }
        { GameBC510Command *c = arg0++; c->w0 = 0xFB000000; c->w1 = (D_8008FDCC & 0xFF) | 0xFFFFFF00; }
    }
    count = D_800D2456;
    dl = (u32 *)D_800D2588;
    if (arg1 != 0) {
        x = (f32)D_8009DCB4[3] - 54.0f;
        y = (f32)D_8009DCB4[4] - 52.0f;
        z = (f32)D_8009DCB4[5] - 344.7000122f;
        count = D_800CC49A;
        func_150A8050(base, 50.0f, -20.0f, -20.0f);
        scale = 1.0f;
        {
            u32 *q = dl;
            u32 cmd;
            do {
                cmd = (u8)(q[0] >> 24);
                switch (cmd) {
                case 0xFC:
                    q[0] = 0;
                    break;
                case 0xEF:
                    q[1] = 0x5041C8;
                    break;
                }
                q += 2;
            } while (cmd != 0xDF);
        }
    } else {
        x = D_800D2428[0];
        y = D_800D2428[1];
        z = D_800D2428[2];
        func_150A8050(base, D_800D2438[0] + 50.0f, -20.0f, D_800D2438[2] + -20.0f);
    }
    func_15043EC8(base, 1.0f, 1.0f, 1.0f, x, y, z);
    for (i = 0; i < count; i++) {
        if (arg1 == 0) {
            scale = D_800D2410[i];
        }
        func_150A7CB0(mtx, scale, scale, scale);
        mtx[3][0] = D_8009DC90[i][0];
        mtx[3][1] = D_8009DC90[i][1];
        mtx[3][2] = D_8009DC90[i][2];
        func_150A7A48(mtx, base, mtx);
        guMtxF2L(mtx, (Mtx *)(&((Mtx (*)[2])&D_800D2590)[i][D_800BE9C0]));
        { GameBC510Command *c = arg0++; c->w0 = 0xDA380003; c->w1 = (u32)(&((Mtx (*)[2])&D_800D2590)[i][D_800BE9C0]); }
        { GameBC510Command *c = arg0++; c->w0 = 0xDE000000; c->w1 = (u32)dl; }
    }
    if (arg1 == 0) {
        for (p = D_800D245C; p != NULL; p = p->unk80) {
            func_15043E68((Mtx *)((u8 *)p + (D_800BE9C0 << 6)), p->unk9C, p->unkA0, p->unkA4, p->unk84, p->unk88, p->unk8C);
            { GameBC510Command *c = arg0++; c->w0 = 0xDA380003; c->w1 = (u32)((u8 *)p + (D_800BE9C0 << 6)); }
            { GameBC510Command *c = arg0++; c->w0 = 0xDE000000; c->w1 = (u32)dl; }
        }
    }
    return arg0;
}
typedef struct BC510Entry {
    s16 x;
    s16 y;
    s16 unk4;
    s16 scale;
    s16 unk8;
    s16 timer;
    u8 unkC;
    u8 state;
    u8 unkE;
    u8 sound;
} BC510Entry;

typedef struct BC510Sprite {
    s32 unk0;
    s16 unk4;
    s16 w;
    s16 h;
    u8 alpha;
    u8 unkB;
    u8 unkC;
    u8 flip;
    u8 padE[2];
} BC510Sprite;

typedef struct BC510Tex {
    s32 id;
    s32 unk4;
    s32 unk8;
} BC510Tex;

extern f32 func_15048A40(u8 arg0);
extern BC510Tex D_800903D4;
extern BC510Tex D_80091840;
GameBC510Command *func_15094F70(GameBC510Command *, BC510Tex *, s32, BC510Sprite *, s32, s32, s32, s32, s32);
GameBC510Command *func_15095A48(GameBC510Command *, BC510Sprite *, f32, f32);

#define ENT ((BC510Entry *)D_800D2460)
GameBC510Command *func_15090630(GameBC510Command *gfx) {
    s32 i;
    BC510Sprite sprite;
    s32 first;
    s32 x;
    s32 y;
    s32 bits;
    f32 f;
    s32 scale;
    s32 t;
    s32 tex;

    sprite.alpha = 0xFF;
    sprite.flip = 0;
    { GameBC510Command *c = gfx++; c->w0 = 0xE7000000; c->w1 = 0; }
    { GameBC510Command *c = gfx++; c->w0 = 0xFCFFB5FF; c->w1 = 0xFFFCFE38; }
    { GameBC510Command *c = gfx++; c->w0 = 0xEF102C0F; c->w1 = 0x0C184244; }
    first = 1;
    i = 0;
    do {
        if (ENT[i].state == 1 || ENT[i].state == 2) {
            if (first) {
                func_1510D0EC(D_80091840.id, (s32)&tex, 3, 0);
                first = 0;
                gfx = func_15094F70(gfx, &D_800903D4, 0, &sprite, 0, 0, 0, 2, 3);
            }
            f = func_15048A40(ENT[i].sound);
            x = ENT[i].x;
            y = ENT[i].y;
            sprite.w = ENT[i].scale * (0.200000003f * f + 1.0f);
            sprite.h = ENT[i].scale * (-0.200000003f * f + 1.0f);
            gfx = func_15095A48(gfx, &sprite, x, y);
        }
        i++;
    } while (i < 6);
    first = 1;
    i = 0;
    do {
        bits = 0;
        if (ENT[i].state == 3) {
            scale = ENT[i].scale;
            x = ENT[i].x;
            y = ENT[i].y;
            sprite.w = scale;
            sprite.h = scale;
            if ((u8)func_150ADA20() < 0x80U) {
                bits = 1;
            }
            if ((u8)func_150ADA20() < 0x80U) {
                bits |= 2;
            }
            sprite.flip = bits;
            if (first) {
                first = 0;
                gfx = func_15094F70(gfx, &D_80091840, 0, &sprite, 0, 0, 0, 2, 3);
            }
            gfx = func_15095A48(gfx, &sprite, x, y);
            t = ENT[i].timer;
            t -= D_800BE9E4;
            if (t < 0) {
                t = 0;
            }
            ENT[i].timer = t;
        }
        i++;
    } while (i != 6);
    return gfx;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_BC510/func_1509093C.s")

typedef struct BC510Stage {
    Mtx mtx[2];
    u32 dl;
    u32 seg1[2];
    u8 pad8C[0xA0 - 0x8C];
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    u8 padAC[4];
    u16 tex[4];
    u8 padB8[2];
    u8 alpha;
    u8 padBB[5];
} BC510Stage;
extern u8 D_80083440[];
extern u8 D_800835C0[];
extern u8 D_80084160[];
extern u8 D_80084190[];
#define STAGE (*(BC510Stage *)&D_800D24C8)

GameBC510Command *func_150911F4(GameBC510Command *gfx) {
    s32 sp90[6];
    f32 y;
    f32 x;
    s32 t;

    if (STAGE.alpha == 0) {
        return gfx;
    }
    { GameBC510Command *c = gfx++; c->w0 = 0xDE000000; c->w1 = (u32)D_80084160; }
    { GameBC510Command *c = gfx++; c->w0 = 0xD9FFFFFF; c->w1 = 0x400; }
    { GameBC510Command *c = gfx++; c->w0 = 0xD9F1FDFE; c->w1 = 0; }
    { GameBC510Command *c = gfx++; c->w0 = 0xD9BDFFFF; c->w1 = 0; }
    if (STAGE.alpha < 0xFF) {
        GameBC510Command *c = gfx++; c->w0 = 0xDB060020; c->w1 = (u32)D_80083440;
    } else {
        GameBC510Command *c = gfx++; c->w0 = 0xDB060020; c->w1 = (u32)D_800835C0;
    }
    y = -11.0f; y += STAGE.unkA8;
    x = -1.0f - STAGE.unkA0;
    func_15043D90(&STAGE.mtx[D_800BE9C0], x, y, 0.0f,
                  4.050000191f, 4.050000191f, 4.050000191f, 667.0f, 152.0f, -1668.0f);
    { GameBC510Command *c = gfx++; c->w0 = 0xDA380003; c->w1 = (u32)&STAGE.mtx[D_800BE9C0]; }
    { GameBC510Command *c = gfx++; c->w0 = 0xF8000000; c->w1 = 0xFF; }
    { GameBC510Command *c = gfx++; c->w0 = 0xFA00F200; c->w1 = 0; }
    { GameBC510Command *c = gfx++; c->w0 = 0xFB000000; c->w1 = (STAGE.alpha & 0xFF); }
    { GameBC510Command *c = gfx++; c->w0 = 0xDB06000C; c->w1 = (u32)&STAGE.mtx[D_800BE9C0]; }
    { GameBC510Command *c = gfx++; c->w0 = 0xDB060004; c->w1 = STAGE.seg1[D_800BE9C0]; }
    if (STAGE.tex[0]) {
        GameBC510Command *c;
        t = func_1510D0EC(STAGE.tex[0], (s32)sp90, 3, 0);
        c = gfx++; c->w0 = 0xDB060018; c->w1 = t;
    }
    if (STAGE.tex[1]) {
        GameBC510Command *c;
        t = func_1510D0EC(STAGE.tex[1], (s32)sp90, 3, 0);
        c = gfx++; c->w0 = 0xDB06001C; c->w1 = t;
    }
    if (STAGE.tex[2]) {
        GameBC510Command *c;
        t = func_1510D0EC(STAGE.tex[2], (s32)sp90, 3, 0);
        c = gfx++; c->w0 = 0xDB060028; c->w1 = t;
    }
    if (STAGE.tex[3]) {
        GameBC510Command *c;
        t = func_1510D0EC(STAGE.tex[3], (s32)sp90, 3, 0);
        c = gfx++; c->w0 = 0xDB06002C; c->w1 = t;
    }
    { GameBC510Command *c = gfx++; c->w0 = 0xDE000000; c->w1 = STAGE.dl; }
    { GameBC510Command *c = gfx++; c->w0 = 0xDE000000; c->w1 = (u32)D_80084190; }
    return gfx;
}

Gfx* func_15091534(Gfx* arg0, struct257 *arg1, u8 *arg2) {
    s32 temp_v0;

    *arg2 = 0;
    temp_v0 = func_1510D0EC((s32)arg1 + (s32)D_D16, 0, 3, 0);

    if (temp_v0 != (s32)0x80000000) {
        gDPSetTextureImage(arg0++, G_IM_FMT_CI, G_IM_SIZ_16b, 1, temp_v0);
        gDPSetTile(arg0++, G_IM_FMT_CI, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD);
        gDPLoadSync(arg0++);
        gDPLoadBlock(arg0++, G_TX_LOADTILE, 0, 0, 1023, 0);
        gDPPipeSync(arg0++);
        gDPSetTile(arg0++, G_IM_FMT_CI, G_IM_SIZ_4b, 4, 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_CLAMP, 6, G_TX_NOLOD);
        gDPSetTileSize(arg0++, G_TX_RENDERTILE, 0, 0, 0xFC, 0xFC);
        gDPSetTextureImage(arg0++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, temp_v0 + 0x800);
        gDPLoadSync(arg0++);
        gDPLoadTLUTCmd(arg0++, 6, 15);
        gDPSetOtherMode(arg0++, 0x00AC3F, 0x00504244);

        *arg2 = 1;
    }

    return arg0;
}

void func_150916B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    volatile s32 pad[2];
    s32 seconds;
    s32 hundredths;

    if (arg2 >= 0x57030) {
        arg2 = 0x57030;
    }

    hundredths = ((arg2 % 60) * 100) / 60;
    if (hundredths >= 100) {
        hundredths = 99;
    }

    arg0 -= 8;
    if (arg2 >= 0) {
        seconds = arg2 / 60;
        func_15042D94((arg0 - (((seconds / 60) >= 10) ? 5 : 0)) - 8, arg1, (u8)arg3, &D_8009DCC0, seconds / 60);
        func_15042D94(arg0, arg1, arg3, &D_8009DCC4, seconds % 60);
        func_15042D94(arg0 + 0x10, arg1, arg3, &D_8009DCCC, hundredths);
        return;
    }

    func_15042D94(arg0 - 8, arg1, (u8)arg3, &D_8009DCD4);
    func_15042D94(arg0, arg1, arg3, &D_8009DCD8);
    func_15042D94(arg0 + 0x10, arg1, arg3, &D_8009DCE0);
}
typedef struct HudView {
    u8 pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x180 - 0x34];
} HudView;

typedef struct HudBars {
    u8 b[4];
} HudBars;

typedef struct HudScore {
    u8 pad0[0x46];
    s16 score[4];
} HudScore;

typedef struct HudNames {
    u8 pad0[0x20];
    char *name[1];
} HudNames;

typedef struct HudTime {
    u8 pad0[0x1C];
    s32 unk1C;
    s32 unk20;
    u8 pad24[3];
    s8 unk27;
    u8 pad28[0x84 - 0x28];
} HudTime;

#define HUD_VIEWS ((HudView *)D_800BE628)
#define HUD_U8(p, off) (*(u8 *)((u8 *)(p) + (off)))
#define HUD_S8(p, off) (*(s8 *)((u8 *)(p) + (off)))
#define HUD_U16(p, off) (*(u16 *)((u8 *)(p) + (off)))
#define HUD_GW(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (b); }

extern HudBars D_800873B0;
extern Gfx D_80087390[];
extern u8 D_800873B4[];
extern u8 D_800873BC[];
extern u8 D_800873C4[];
extern u8 D_800873C8[];
extern u16 D_8008FDC0;
extern HudScore *D_8008FDD4;
extern s8 D_800E0AC0[];
extern HudNames *D_800E0BD8;
extern HudTime *D_800872A0;
extern s8 D_800E0BE7;
extern u8 D_CF1[];
extern u8 D_CF2[];
extern u8 D_D13[];
extern u8 D_D14[];
void func_150916B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

GameBC510Command *func_150918EC(GameBC510Command *gfx) {
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 idx;
    s32 h;
    s32 left;
    s32 t;
    s32 n;
    s32 s;
    s32 v;
    s32 y2;
    s32 width;
    s32 lap_total;
    s32 right;
    struct126 *a1;
    struct127 *p;
    s32 w;
    s32 extra;
    s32 laps;
    s32 lap;
    s32 y3;
    s32 F3c;
    s32 step;
    u8 ok;
    u8 loaded;
    u8 mode;
    HudBars bars;
    s8 grenades;
    s16 rows[33];

    mode = 0;
    grenades = 0;
    bars = D_800873B0;
    width = HUD_VIEWS->unk30 - HUD_VIEWS->unk2C;
    gSPDisplayList((Gfx *)gfx++, D_80087390);
    func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
    step = 0x400;
    if (D_8008FE1C != 1.0f) {
        step = (s32)((f32)step / D_8008FE1C);
        for (j = 0; j < 33; j++) *(rows + j) = (s32)((f32)j * D_8008FE1C);
    } else {
        for (j = 0; j < 33; j++) *(rows + j) = j;
    }

    if (D_8008FDC0 & 0xC02) {
        loaded = 0;
        ok = 0;
        for (i = 0; i <= D_80082FA0; i++) {
            if (D_80087270[i] != 10) {
                y3 = HUD_VIEWS[i].unk2C;
                y = HUD_VIEWS[i].unk24 + rows[6];
                if (D_8008FDC0 & 2) {
                    if (!loaded) {
                        n = func_1510D0EC((s32)D_D14, 0, 3, 0);
                        loaded = 1;
                        if (n != (s32)0x80000000) {
                            HUD_GW(gfx++, 0xFD180000, n);
                            HUD_GW(gfx++, 0xF5180000, 0x07094250);
                            HUD_GW(gfx++, 0xE6000000, 0);
                            HUD_GW(gfx++, 0xF3000000, 0x073FF000);
                            HUD_GW(gfx++, 0xE7000000, 0);
                            HUD_GW(gfx++, 0xF5181000, 0x00094250);
                            HUD_GW(gfx++, 0xF2000000, 0x0007C07C);
                            ok = 1;
                        }
                        HUD_GW(gfx++, 0xEF002C3F, 0x00504244);
                        HUD_GW(gfx++, 0xE7000000, 0);
                        HUD_GW(gfx++, 0xFB000000, 0xFFFFFF80);
                    }
                    x = rows[5] + y3;
                    n = func_150859AC(i, 3);
                    y2 = D_800873C4[n & 3] << 5; t = D_800873C8[n >> 2] << 5;
                    if (ok) {
                        gSPScisTextureRectangle(gfx++, x * 4, y * 4, (rows[5] + x) * 4, (rows[7] + y) * 4, 0, y2, t, step, step);
                    }
                }
                if (D_8008FDC0 & 0x400) {
                    x = (width >> 1) + y3 - 8;
                    if (D_8008FE1C != 1.0f) {
                        x -= 8;
                    }
                    func_15042D94(x, y, 0x41, (s32 *)"%d", D_8008FDD4->score[i]);
                }
                if (D_8008FDC0 & 0x800) {
                    x = (width >> 1) + y3 + 0x10;
                    if (D_8008FE1C != 1.0f) {
                        x += 0x10;
                    }
                    func_15042D94(x, y, 0x41, (s32 *)"(%s)", D_800E0BD8->name[D_800E0AC0[i]]);
                }
            }
        }
    }

    if (D_8008FDC0 & 0x14) {
        loaded = 0;
        ok = 0;
        for (i = 0; i <= D_80082FA0; i++) {
            if (D_80087270[i] != 10) {
                y3 = HUD_VIEWS[i].unk2C + rows[5];
                right = HUD_VIEWS[i].unk30 - rows[5];
                y = HUD_VIEWS[i].unk24 + rows[5];
                if (D_8008FDC0 & 4) {
                    p = &D_800CC2D0[i];
                    n = 1;
                    h = p->unk31C->unk75;
                    h &= 0x7F;
                    if (p->interaction_state == 0x1F) {
                        h = 0;
                    }
                    switch (h) {
                    case 0x9:
                        lap = 0;
                        break;
                    case 0x16:
                        lap = 1;
                        break;
                    case 0x14:
                    case 0x3F:
                        lap = 3;
                        break;
                    case 0x20:
                        lap = 5;
                        break;
                    case 0x18:
                    case 0x41:
                        lap = 6;
                        break;
                    case 0x15:
                        lap = 7;
                        break;
                    case 0x22:
                        lap = 9;
                        break;
                    case 0x23:
                        lap = 10;
                        break;
                    case 0x21:
                        lap = 12;
                        break;
                    case 0x24:
                        lap = 13;
                        break;
                    case 0x25:
                        lap = 14;
                        break;
                    case 0x26:
                    case 0x3A:
                        lap = -1;
                        if (p->health != 0) {
                            grenades = 1;
                        }
                        break;
                    case 0x38:
                        lap = -1;
                        break;
                    case 0x32:
                    case 0x33:
                        n = HUD_S8(p->unk31C, 0x1AE);
                        lap = 11;
                        break;
                    case 0x40:
                        lap = 4;
                        extra = -3;
                        break;
                    default:
                        if (D_800BE9F0 == 0x1A) {
                            lap = 4;
                            extra = 4;
                        } else {
                            lap = -1;
                        }
                        break;
                    }
                    if (p->unk13C != 0) {
                        lap = -1;
                    }
                    if (lap >= 0) {
                        j = 0;
                        if (!loaded) {
                            loaded = 1;
                            gfx = (GameBC510Command *)func_15091534((Gfx *)gfx, NULL, &ok);
                        }
                        x = (right - rows[25]) * 4;
                        t = D_800873B4[lap / 3] << 5;
                        left = rows[D_800873BC[lap / 3]] * 4;
                        if (lap == 4) {
                            j = 4;
                            left -= 8;
                            t += extra << 5;
                        }
                        if (lap == 1) {
                            left -= 4;
                        }
                        do {
                            if (ok) {
                                gSPTextureRectangle(gfx++, x, y * 4 + j, rows[22] * 4 + x, y * 4 + left + j, 0, ((lap % 3) * 25) << 5, t, step, step);
                            }
                            x -= 0x48;
                        } while (--n > 0);
                    }
                }
                if (D_8008FDC0 & 0x10) {
                    if (!loaded) {
                        gfx = (GameBC510Command *)func_15091534((Gfx *)gfx, NULL, &ok);
                    }
                    s = func_150859AC(i, 4);
                    x = rows[22] + y3;
                    h = ((s + 1) >> 1) * 5;
                    if (h != 0 && ok) {
                        gSPTextureRectangle(gfx++, (x - rows[h]) * 4, y * 4, x * 4, rows[4] * 4 + y * 4, 0, 0x620, 0xA0, step, step);
                    }
                    h = (s >> 1) * 5;
                    if (h != 0 && ok) {
                        gSPTextureRectangle(gfx++, (x - rows[h]) * 4, rows[5] * 4 + y * 4, x * 4, rows[9] * 4 + y * 4, 0, 0x620, 0xA0, step, step);
                    }
                }
            }
        }
    }

    if (D_8008FDC0 & 4) {
        u8 pad3a;
        s32 ex3;
        s32 pad3b;
        loaded = 0;
        ok = 0;
        for (i = 0; i <= D_80082FA0; i++) {
            if (D_80087270[i] != 10) {

                right = HUD_VIEWS[i].unk30 - 5.0f;
                y = HUD_VIEWS[i].unk24 + 5.0f;
                n = 1;
                lap = -1;
                ex3 = 0;
                if (D_800CC2D0[i].interaction_state == 0x1E) {
                    j = D_800CC2D0[i].unk31C->unk128;
                    if (j & 1) {
                        lap = 6;
                        F3c = 4;
                    } else if (j & 2) {
                        lap = 3;
                    } else if (j & 0x80) {
                        lap = 0;
                        F3c = 6;
                    } else if (j & 0x10) {
                        lap = 9;
                        F3c = -3;
                    }
                    if (lap >= 0 && lap != 3) {
                        if (F3c > 0) {
                            bars.b[i] = HUD_U16(D_800CC2D0[i].unk31C, 0x12C) >> F3c;
                        } else {
                            bars.b[i] = HUD_U16(D_800CC2D0[i].unk31C, 0x12C) << -F3c;
                        }
                        mode = 1;
                    }
                } else {
                    switch (ex3 = 3, D_800CC2D0[i].unk31C->unk75 & 0x7F) {
                    case 0x37:
                        lap = 7;
                        break;
                    case 0x38:
                        lap = 4;
                        break;
                    case 0x39:
                        lap = 10;
                        break;
                    case 0x30:
                    case 0x31:
                        n = HUD_S8(D_800CC2D0[i].unk31C, 0x1AE);
                        lap = 6;
                        break;
                    case 0x3E:
                        bars.b[i] = HUD_U16(D_800CC2D0[i].unk31C, 0x12C) >> 4;
                        mode = 2;
                        break;
                    }
                }
                if (lap >= 0) {
                    if (!loaded) {
                        loaded = 1;
                        gfx = (GameBC510Command *)func_15091534((Gfx *)gfx, (struct257 *)1, &ok);
                    }
                    x = right - rows[25];
                    y2 = (lap % 3) * 25;
                    lap = lap / 3;
                    t = D_800873B4[lap] + ex3;
                    y2 <<= 5;
                    t <<= 5;
                    do {
                        if (ok) {
                            gSPTextureRectangle(gfx++, x * 4, y * 4, (x + rows[22]) * 4, (rows[D_800873BC[lap]] + y) * 4, 0, y2, t, step, step);
                        }
                        x -= 0x12;
                    } while (--n > 0);
                }
            }
        }
    }

    if (mode) {
        gDPPipeSync((Gfx *)gfx++);
        HUD_GW(gfx++, 0xEF002C3F, 0x0F0A4004);
        HUD_GW(gfx++, 0xFCFFFFFF, 0xFFFDF6FB);
        HUD_GW(gfx++, 0xFA000000, 0xFF0000FF);
        if (mode == 2) {
            laps = 15;
        } else {
            laps = 30;
        }
        for (i = 0; i <= D_80082FA0; i++) {
            if (bars.b[i]) {
                x = (s32)HUD_VIEWS[i].unk30 - laps;
                w = (s32)HUD_VIEWS[i].unk24 + 10;
                gDPFillRectangle((Gfx *)gfx++, x - bars.b[i], w, x, w + 4);
            }
        }
    }

    if (grenades) {
        s32 pad2;
        s32 pad3;
        s32 pad4;
        s32 F5b;
        s32 F5d;
        F5d = 0xE7000000;
        j = func_1510D0EC((s32)D_D13, 0, 3, 0);
        HUD_GW(gfx++, 0xFD100000, j);
        HUD_GW(gfx++, 0xF5100000, 0x07094250);
        HUD_GW(gfx++, 0xE6000000, 0);
        HUD_GW(gfx++, 0xF3000000, 0x073FF000);
        HUD_GW(gfx++, F5d, 0);
        HUD_GW(gfx++, 0xF5101000, 0x00094250);
        HUD_GW(gfx++, 0xF2000000, 0x0007C07C);
        HUD_GW(gfx++, F5d, 0);
        HUD_GW(gfx++, 0xEF002C3F, 0x00504244);
        for (i = 0; i <= D_80082FA0; i++) {
            j = D_800CC2D0[i].unk31C->unk75;
            if ((j &= 0x7F) == 0x26 || j == 0x3A) {
                x = (s32)HUD_VIEWS[i].unk30 - 15;
                x *= 4;
                w = (s32)HUD_VIEWS[i].unk24 + 5;
                w *= 4;
                if (0x1A == D_800BE9F0 || 0x3F == D_800BE9F0) {
                    F5d = 11;
                    F5b = 14;
                    y2 = 0x200;
                    v = 0x1E0;
                } else if (D_800CC2D0[i].id == 0x75 || D_800CC2D0[i].id == 0xB1) {
                    F5d = 5;
                    F5b = 20;
                    y2 = 0xC0;
                    v = 0x100;
                } else if (D_800CC2D0[i].id == 0x3B) {
                    F5d = 11;
                    F5b = 14;
                    y2 = 0x200;
                    v = 0x1E0;
                } else {
                    F5d = 8;
                    F5b = 9;
                    y2 = 0x240;
                    v = 0x60;
                }
                for (j = 0; j < D_800CC2D0[i].unk31C->grenade_count; j++) {
                    gSPScisTextureRectangle(gfx++, x, w, rows[F5d] * 4 + x, rows[F5b] * 4 + w, 0, y2, v, 0x400, step);
                    x -= 0x2C;
                }
            }
        }
    }

    if ((D_8008FDC0 & 0x80) && D_800872A0 != NULL) {
        lap_total = D_800E0BE7;
        for (i = 0; i <= D_80082FA0; i++) {
            w = (s32)HUD_VIEWS[i].unk24 + 5;
            func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
            if (D_800872A0[i].unk20 > 0) {
                x = (s32)HUD_VIEWS[i].unk2C;
                func_150916B4(x + 0x17, w + 0x19, D_800872A0[i].unk20, 0xC0);
            }
            x = (s32)HUD_VIEWS[i].unk2C + (width >> 1);
            lap = (s8)(D_800872A0[i].unk27 + 1);
            if (lap <= lap_total) {
                func_15042D94(x, w, 0xC1, (s32 *)"%s", D_800E0BD8->name[D_800E0AC0[i]]);
                func_15042D94((s32)HUD_VIEWS[i].unk2C + 0x1D, w + 0xE, 0xC1, (s32 *)"lap %d/%d", lap, lap_total);
                func_150916B4(x, (s32)HUD_VIEWS[i].unk28 - 0xC, D_800872A0[i].unk1C, 0xC0);
            }
        }
    }

    if (D_800BEAC1 == 0 && (D_8008FDC0 & 0x1000)) {
        s32 shift;
        s32 size;
        f32 scale;
        f32 ax;
        f32 az;
        scale = 0.01300000027f;
        i = 0;
        x = (s32)HUD_VIEWS->unk30 - rows[32];
        x *= 4;
        if (D_8008FE1C != 1.0f) {
            shift = 6;
            scale *= 1.559999943f;
            size = 0x63;
        } else {
            shift = 4;
            size = 0x40;
        }
        laps = (D_8008FDBC & 0x40) ? (s32)D_CF2 : (s32)D_CF1;
        right = func_1510D0EC(laps, 0, 3, 0);
        HUD_GW(gfx++, 0xFD700000, right);
        HUD_GW(gfx++, 0xF5700000, 0x07098260);
        HUD_GW(gfx++, 0xE6000000, 0);
        HUD_GW(gfx++, 0xF3000000, 0x077FF000);
        HUD_GW(gfx++, 0xE7000000, 0);
        HUD_GW(gfx++, 0xF5681000, 0x00098260);
        HUD_GW(gfx++, 0xF2000000, 0x000FC0FC);
        HUD_GW(gfx++, 0xEF002C3F, 0x00504244);
        for (i = 0; i < D_8008FD90; i++) {
            if (func_150859AC(i, 3) > 0) {
                w = rows[21] + ((s32)HUD_VIEWS[i].unk24 + rows[24]);
                w *= 4;
                gDPPipeSync((Gfx *)gfx++);
                HUD_GW(gfx++, 0xFB000000, 0xFFFFFF80);
                HUD_GW(gfx++, 0xFC129A25, 0xFF37FFFF);
                s = x - rows[32] * 4;
                idx = w - rows[32] * 4;
                gSPScisTextureRectangle(gfx++, s, idx, size * 4 + s, size * 4 + idx, 0, 0, 0, step, step);
                gDPPipeSync((Gfx *)gfx++);
                HUD_GW(gfx++, 0xFC62FEC5, 0xFFFFFBFD);
                for (laps = 0, p = D_800CC2D0; laps != 25; laps++, p++) {
                    if (laps == i || (laps < D_8008FD8C && func_150859AC(laps, 3) > 0) || (s8)p->pad320[8] != 0) {
                        gDPPipeSync((Gfx *)gfx++);
                        if (laps == i) {
                            HUD_GW(gfx++, 0xFB000000, 0x00FF00FF);
                        } else if (laps < D_8008FD8C) {
                            HUD_GW(gfx++, 0xFB000000, 0xFF0000FF);
                        } else {
                            HUD_GW(gfx++, 0xFB000000, 0xFFFFFFFF);
                        }
                        ax = p->x_position * scale;
                        az = p->z_position * scale;
                        v = (w + 3.400000095f) + (s32)az;
                        n = (x + 3.599999905f) + (s32)ax;
                        gSPScisTextureRectangle(gfx++, n - shift, v - shift, n + shift, v + shift, 0, 0, 0, 0x400, 0x400);
                    }
                }
            }
        }
    }
    if (D_800BEAC1 == 0 && (D_8008FDC0 & 0x20)) {
        gfx = func_15093B58(gfx, step, rows, 0x2C30, 0);
    }
    return gfx;
}

void func_15093818(s32 arg0) {
    if ((arg0 != 0) && (D_800D2458 == 0)) {
        D_800D2458 = 1;
        func_15093878();
        return;
    }
    if ((arg0 == 0) && (D_800D2458 != 0)) {
        D_800D2458 = 0;
    }
}

void func_15093878(void) {
    D_800D2448 = func_1518C900(0xBA);
    D_800D244C = allocate_memory(0x80, 1, 1, 0);
}

extern s32 D_800D2450;
extern u8 D_11AD[];
void func_150A7D00(s32, f32, f32, f32);

GameBC510Command *func_150938BC(GameBC510Command *arg0) {
    s32 x = 120;
    s32 y = 129;
    s32 z = -303;
    s32 i;
    s32 val;
    s32 tex;

    val = (D_800D2450 / 30) % 60;
    i = 4;
    do {
        tex = func_1510D0EC((s32)D_11AD + (val % 10), 0, 3, 0);
        if (tex == (s32)0x80000000) {
            return arg0;
        }
        gSPSegment((Gfx *)arg0++, i, tex);
        if (i == 3) {
            val = (D_800D2450 / 30) / 60;
        } else {
            val = val / 10;
        }
        i--;
    } while (i != 0);

    func_150A7D00((D_800BE9C0 << 6) + D_800D244C, x, y, z);
    gSPMatrix((Gfx *)arg0++, (D_800BE9C0 << 6) + D_800D244C, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList((Gfx *)arg0++, D_800D2448);
    return arg0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_BC510/func_15093B58.s")
