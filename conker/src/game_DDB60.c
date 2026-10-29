#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


typedef struct {
    s32 unk00;
    s32 unk04;
    struct17 unk08;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 unk30;
    f32 unk34;
    f32 unk38;
} Arg150B06B0_2190;

typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    struct17 unk08;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    s8 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s32 unk48;
} Arg150B06B0_3F18;

typedef struct {
    u8 pad[0x24];
} Floor150B06B0;

extern s32 func_15143E24(struct127 *);
extern void func_1504715C(Floor150B06B0 *, struct127 *);
extern void func_151C329C(void *, u8, s32);
extern void func_15152190(Arg150B06B0_2190 *, const s32 *, const f32 *, s32, f32, s32, u8, s32);
extern void func_15153F18(Arg150B06B0_3F18 *, struct17 *, Floor150B06B0 *, u8, s32);

extern const s32 D_8009F810[1];
extern const f32 D_8009F814[1];
extern const s32 D_8009F818[1];
extern const f32 D_8009F81C[1];

void func_150B06B0(struct127 *arg0, struct127 *arg1, u8 arg2, s32 arg3) {
    struct17 pos;
    s8 angle;
    Floor150B06B0 floor;

    pos.unk0 = arg0->x_position;
    pos.unk4 = arg0->y_position;
    pos.unk8 = arg0->z_position;
    angle = func_15143E24(arg1) + 0x40;
    func_1504715C(&floor, arg0);
    if (arg0->id == 0xA6) {
        struct17 pos2;
        Arg150B06B0_2190 b;

        pos2.unk0 = pos.unk0;
        pos2.unk4 = pos.unk4 + 50.0f;
        pos2.unk8 = pos.unk8;
        func_151C329C(&pos2, arg2, arg3);
        b.unk00 = 0xA;
        b.unk04 = 0xA;
        b.unk08 = pos;
        b.unk1C = 7.0f;
        b.unk20 = 17.0f;
        b.unk14 = angle - 0x28;
        b.unk16 = 0x50;
        b.unk18 = -0x2D;
        b.unk1A = 0x1A;
        b.unk2C = 0x19;
        b.unk2E = 0x19;
        b.unk24 = -0.8000000119f;
        b.unk28 = 0.3440000117f;
        b.unk30 = 0.7070000172f;
        b.unk34 = 0.5070000291f;
        b.unk38 = 26.41100121f;
        func_15152190(&b, D_8009F810, D_8009F814, 1, 0.0f, 0, arg2, arg3);
    } else {
        {
        Arg150B06B0_2190 b;

        b.unk00 = 0xA;
        b.unk04 = 0xA;
        b.unk08 = pos;
        b.unk1C = 7.0f;
        b.unk20 = 17.0f;
        b.unk14 = angle - 0x28;
        b.unk16 = 0x50;
        b.unk18 = -0x15;
        b.unk1A = 0xD;
        b.unk2C = 0x19;
        b.unk2E = 0x19;
        b.unk24 = -0.8000000119f;
        b.unk28 = 0.3440000117f;
        b.unk30 = 0.04900000244f;
        b.unk34 = 0.1020000055f;
        b.unk38 = 26.41100121f;
        func_15152190(&b, D_8009F818, D_8009F81C, 1, 0.0f, 0, arg2, arg3);
        }
        {
        Arg150B06B0_3F18 c;
        c.unk08 = pos;
        c.unk14 = 3.049999952f;
        c.unk2C = 7;
        c.unk2E = 3;
        c.unk00 = angle - 0x3C;
        c.unk02 = 0x78;
        c.unk04 = -0x1E;
        c.unk06 = 0x10;
        c.unk30 = 3;
        c.unk32 = 2;
        c.unk34 = 0x14;
        c.unk36 = 0x14;
        c.unk38 = 0x9B;
        c.unk3A = 0x64;
        c.unk44 = 0x10;
        c.unk46 = 0xF;
        c.unk48 = 0;
        c.unk3C = 0;
        c.unk18 = 5.009999752f;
        c.unk1C = 0.7000000477f;
        c.unk20 = 0.5010000467f;
        c.unk24 = 15.10000038f;
        c.unk28 = 20.80000114f;
        c.unk40 = 0.5f;
        func_15153F18(&c, &c.unk08, &floor, arg2, arg3);
        c.unk3C = 1;
        func_15153F18(&c, &c.unk08, &floor, arg2, arg3);
        }
    }
}

extern f32 D_8009F870;

void func_150B0A60(s32 *arg0, s32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4, s16 *arg5) {
    f32 r1;
    f32 v;
    f32 ang;

    *arg5 = (s16)(*(f32 *)((s32)arg0 + 0x40) * D_8009F870 - 128.0f);
    r1 = func_150ADA68() * 248.0f;
    v = func_150ADA68() + -0.5f;
    if (r1 < 122.0f) {
        arg4[0] = arg2[0] * -31.5f + (arg3[0] * v) * 122.0f;
        arg4[1] = 0.0f;
        arg4[2] = arg2[1] * -31.5f + (arg3[1] * v) * 122.0f;
    } else {
        f32 k = 63.0f;
        ang = 61.0f;
        r1 = r1 - 122.0f;
        if (r1 < k) {
            ang = -ang;
            *arg5 += 0x40;
        } else {
            *arg5 -= 0x40;
        }
        arg4[0] = arg2[0] * k * v + arg3[0] * ang;
        arg4[1] = 0.0f;
        arg4[2] = arg2[1] * k * v + arg3[1] * ang;
    }
    arg4[0] += *(f32 *)((s32)arg0 + 0x14);
    arg4[1] += *(f32 *)((s32)arg1 + 0x118);
    arg4[2] += *(f32 *)((s32)arg0 + 0x1C);
}

void func_150B0C58(void *, u8, s32);

s32 func_150B0C34(s32 arg0) {
    func_150B0C58((void *)arg0, 0xFF, 1);
}

void func_150B0C58(void *arg0, u8 arg1, s32 arg2) {
    struct {
        void *unk0;
        u8 unk4;
        u8 pad5;
        u8 pad6;
        u8 pad7;
        f32 unk8;
    } sp34;
    struct260 *temp_v0;

    sp34.unk0 = arg0;
    sp34.unk4 = *(u8 *)((s32)arg0 + 0x3B);
    sp34.unk8 = 0.0f;

    temp_v0 = func_15149130(0x12C, -1, 0x58, -1, 0, 0x43, (struct37 *)0xC, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x28), &sp34, 0xC);
    }
}

void func_150B0CE0(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x28, arg0 + 0x2C, arg0);
}

typedef struct {
    s32 *obj;
    u8 id;
    u8 pad5[3];
    f32 timer;
} Sub28_DDB60;

extern f32 D_8009F860[];
extern f32 D_8009F868[];
s32 func_1515C0F8(s32 *, f32 **);
void func_1514373C(f32, f32, f32 *, f32 *);
void func_15143794(s16, s16, f32, f32 *);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);

void func_150B0D20(u8 *arg0)
{
  Sub28_DDB60 *sub;
  s32 *obj;
  s32 flag;
  f32 dist;
  f32 lim;
  s32 idx;
  f32 dir[2];
  f32 perp[2];
  f32 *vel;
  f32 pos[3];
  f32 rot[3];
  s16 angle;
  obj = *((s32 **) (arg0 + 0x28));
  sub = (Sub28_DDB60 *) (arg0 + 0x28);
  if (((*obj) == 0) || (sub->id != (*(((u8 *) obj) + 0x3B))))
  {
    *((s16 *) (arg0 + 0xE)) = -1;
    return;
  }
  idx = *(((u8 *) obj) + 0x124);
  flag = 0;
  if ((*(((u8 *) D_800CC2D0[idx].unk31C) + 0x4F)) == 2)
  {
    dist = *((f32 *) (((u8 *) obj) + 0x3C));
    lim = (*((f32 *) (((u8 *) (&D_800CC2D0[idx])) + 0x44))) * 1.350000024f;
    if (dist < lim)
    {
      dist = lim;
    }
    if (dist > 26.0f)
    {
      if (1)
      {
        flag = 1;
      }
    }
  }
  if (flag)
  {
    sub->timer += (D_8009F860[flag] + (func_150ADA68() * D_8009F868[flag])) * D_800BE9A4;
    if (sub->timer > 1.0f)
    {
      if (func_1515C0F8(obj, &vel) == 0)
      {
        vel = &D_800A5480;
      }
      func_1514373C((*((f32 *) (((u8 *) obj) + 0x40))) * 0.01745329238f, 1.0f, &dir[0], &dir[1]);
      perp[0] = dir[1];
      perp[1] = -dir[0];
      do
      {
        func_150B0A60(obj, (s32 *) (&D_800CC2D0[idx]), dir, perp, pos, &angle);
        func_15143794(angle, (func_150ADA20() % 26) - 41, (func_150ADA68() * 3.769999981f) + 4.059999943f, rot);
        rot[0] -= vel[0] * 0.9250000715f;
        rot[2] -= vel[2] * 0.9250000715f;
        func_151D9014(pos, rot, 5, (func_150ADA68() * 0.1780000031f) + (-0.4420000315f), (func_150ADA20() % 21) + 20, (func_150ADA20() % 101) + 155, (func_150ADA68() * 50.0f) + 80.0f, func_150ADA68() < 0.04400000349f, 1.399999976f, 1.399999976f, 1, 0, 1, 0, arg0[0xC], arg0[1]);
        sub->timer -= 1.0f;
      }
      while (sub->timer > 1.0f);
    }
  }
}
