#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 *allocate_memory(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
typedef struct {
    u8 pad_0[0x3E0];
    s32 field_0x3E0[2];
    s32 field_0x3E8[2];
    s32 field_0x3F0;
    u8 field_0x3F4;
    u8 field_0x3F5;
    u8 field_0x3F6;
} Game6C960Object;

void func_1502D824(Game6C960Object *arg0, s32 arg1, s32 *arg2);
void func_150A9984(s32 arg0, u8 arg1);
void func_1507BDB0(Game6C960Object *arg0, f32 arg1, s32 arg2, s32 arg3);
void func_10004074(s32 *arg0);

struct func_1503F4B0_sp38 {
    s32 unk0;
    s32 pad4;
    s16 unk8;
    s16 padA;
    s32 unkC;
};

void func_1503F4B0(Game6C960Object *arg0) {
    struct func_1503F4B0_sp38 sp38;
    s32 temp_a1;

    sp38.unkC = 0;
    if (arg0->field_0x3E0[D_800BE9C0] != 0) {
        sp38.unkC = (s32) allocate_memory(0x320, 1, 2, 2);
        func_1502D824(arg0, 0, (s32 *) sp38.unkC);
        sp38.unk8 = 0x1000;
        temp_a1 = arg0->field_0x3E0[D_800BE9C0];
        sp38.unk0 = arg0->field_0x3E8[D_800BE9C0];
        func_150A81D0(&sp38.unk0, temp_a1, arg0, arg0->field_0x3F0, arg0->field_0x3F4, &sp38.unk8, 0, 0);
        func_150A9984(arg0->field_0x3E8[D_800BE9C0], arg0->field_0x3F4);
        arg0->field_0x3F6 = 1;
    }
    func_1507BDB0(arg0, D_800BE9A4, 0, 0);
    if (sp38.unkC != 0) {
        func_10004074((s32 *) sp38.unkC);
    }
}

void func_1505E0C4(s32, s32, s32, s32, s32, s32, s32, f32, f32, f32, f32, s32);

void func_1503F5B8(Game6C960Object *arg0, s32 arg1, s32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    func_1505E0C4(0, 0, (s32) arg0, 0, arg1, arg2, arg0->field_0x3F5, arg3, arg4, 0.0f, 0.0f, arg5);
}

s32 func_1502FE10(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 *arg5, s32 *arg6);

s32 func_1503F62C(Game6C960Object *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, Game6C960Object **arg6)
{
  s32 sp3C;
  s32 *p;
  s32 pad;
  *arg6 = (Game6C960Object *) allocate_memory(0x3F8, 1, 2, 2);
  if ((*arg6) == 0)
  {
    return 1;
  }
  bzero(*arg6, 0x40);
  (*arg6)->pad_0[0x215] = 1;
  (*arg6)->pad_0[0x45] = 1;
  func_1502FE10((s32) arg0, arg2, arg3, arg4, arg5, &(*arg6)->field_0x3F0, &sp3C);
  (*arg6)->field_0x3F4 = sp3C;
  (*arg6)->field_0x3F5 = arg1;
  (*arg6)->field_0x3F6 = 0;
  (*arg6)->field_0x3E8[0] = (s32) allocate_memory(sp3C << 6, 1, 2, 2);
  (*arg6)->field_0x3E8[1] = (s32) allocate_memory(sp3C << 6, 1, 2, 2);
  arg0 = *arg6;
  p = (s32 *) arg0->field_0x3E8[0];
  if ((p == 0) || (arg0->field_0x3E8[1] == 0))
  {
    if (p == 0)
    {
      func_10004074(p);
      arg0 = *arg6;
    }
    p = (s32 *) arg0->field_0x3E8[1];
    if (p == 0)
    {
      func_10004074(p);
      if (((!arg2) && (!arg2)) && (!arg2))
      {
      }
      arg0 = *arg6;
    }
    func_10004074((s32 *) arg0);
    return 1;
  }
  func_1503F5B8(arg0, 1, 0, 1.0f, 0.0f, 0);
  return 0;
}

void func_1503F7B8(s32 *arg0) {
    func_100043B4((s32 *)arg0[250], 4);
    func_100043B4((s32 *)arg0[251], 4);
    func_10004074(arg0);
}
