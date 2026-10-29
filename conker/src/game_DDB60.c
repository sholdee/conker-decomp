#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_DDB60/func_150B06B0.s")

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
