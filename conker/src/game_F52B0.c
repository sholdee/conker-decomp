#include <ultra64.h>
#include "functions.h"
#include "variables.h"


s32 func_1509BE40(s32, s32, s32, ...);
void func_15123070(struct108 *);
void func_151254F4(struct108 *, s32);
extern s32 D_80088800;
extern f32 D_800A04E0;
extern f32 D_800A04E4;

void func_150C7E00(struct108 *arg0)
{
  s32 sp34;
  s32 sp30;
  s32 sp2C;
  f32 temp_f0;
  f32 var_f2;
  struct127 *temp_v0;
  struct127 *temp_v1;
  sp34 = func_1509BE40(0, func_1509BE40(0, 0x2010, 0xB7) | 0x2000, 0xBC);
  sp30 = func_1509BE40(0, 0x2000, 0xBB);
  sp2C = func_1509BE40(0, func_1509BE40(0, 0x2010, 0xB7) | 0x2000, 0xBB);
  if (func_1509BE40(0, 0x5071, 0x1A) == 0)
  {
    if ((sp30 != (-1)) && (sp34 == 0))
    {
      if (func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 3) != 0)
      {
        arg0->unk84 |= 0x01000000;
        func_151254F4(arg0, D_800CC335 - 1);
        arg0->unk674 = 0.0f;
      }
    }
    else
    {
      if (sp30)
      {
      }
      if ((sp30 != (-1)) && (sp34 != 0))
      {
        func_151239CC(arg0, 2);
        if (func_15123934(arg0, 8, 0, arg0->unk134, 3) != 0)
        {
          func_151254F4(arg0, D_800CC335 - 1);
 if (0) { }
          arg0->unk84 &= ~4;
          arg0->unk674 = 0.0f;
          func_15123070(arg0);
        }
        arg0->unk5F0 |= 0x200;
        arg0->unk348 = 500.0f;
        arg0->unk34C = 500.0f;
        arg0->unk374 = 800.0f;
      }
      else
      {
        if (func_151239CC(arg0, 3) != 0)
        {
          func_151254F4(arg0, 0);
          arg0->unk84 &= 0xFEFFFFFF;
          arg0->unk3D4->unk198 = 0;
          arg0->unk73C = 0;
          if (arg0->unk6C8 == 0)
          {
            arg0->unk674 = 0.0f;
          }
        }
        arg0->unk5F0 &= ~0x200;
      }
    }
  }
  else
  {
    if (func_1509BE40(0, 0x5072, 0x1A) == 0)
    {
      if ((*arg0->unk36C) & 4)
      {
        arg0->unk190 = 20.0f;
      }
      if (func_1509BE40(0, 0x2000, 0x93) == 0)
      {
        arg0->unk190 = 0.0f;
        return;
      }
      func_1509BFB0(1, 0x9000, 0x17, func_1509BE40(1, func_1509BE40(0, 0x2011, 0xB7) | 0x2000, 0x9C, 0x2000));
      temp_f0 = (((f32) func_1509BE40(1, func_1509BE40(0, 0x2011, 0xB7) | 0x2000, 0x9A, 0x2000)) - 300.0f) / 900.0f;
      var_f2 = (temp_f0 < 0.0f) ? (0.0f) : ((temp_f0 > 1.0f) ? (1.0f) : (temp_f0));
      if (sp34 == 0)
      {
        arg0->unk348 = (arg0->unk34C = 200.0f);
        arg0->unk190 = 100.0f;
      }
      else
      {
        arg0->unk190 = (103.0f * var_f2) + D_800A04E0;
        arg0->unk374 = (224.0f * var_f2) + D_800A04E4;
        arg0->unk348 = (arg0->unk34C = 231.0f * var_f2);
      }
      func_1509BFB0(2, 0x9000, 6, 1, 0x20000);
      func_1509BFB0(2, 0x9000, 6, 0, 4);
      if (sp2C != (-1))
      {
        arg0->unk670 = 0.0f;
      }
      D_80088800 = 0;
      if (arg0->unk5F0 & 4)
      {
        temp_v0 = func_15083E90(0x10);
        if (temp_v0 != 0)
        {
          temp_v1 = arg0->unk3D0;
          if (temp_v1->unk65 != 0)
          {
            temp_v1->x_position = temp_v0->x_position;
            arg0->unk3D0->y_position = temp_v0->y_position;
            arg0->unk3D0->z_position = temp_v0->z_position;
          }
        }
      }
      if (func_151239CC(arg0, 3) != 0)
      {
        arg0->unk5F0 &= ~0x200;
        func_151254F4(arg0, 0);
        arg0->unk84 &= 0xFEFFFFFF;
        arg0->unk3D4->unk198 = 0;
        arg0->unk73C = 0;
        if (arg0->unk6C8 == 0)
        {
          s32 pad;
          arg0->unk674 = 0.0f;
        }
      }
    }
    else
      if ((func_151239CC(arg0, 3) != 0) || (D_80088800 == 0))
    {
      arg0->unk84 |= 4;
      D_80088800 = 1;
      func_151254F4(arg0, 0);
      arg0->unk3D4->unk198 = 0;
      arg0->unk73C = 0;
      arg0->unk1B4 = 3;
      func_15124B18(arg0);
      arg0->unk190 = 0.0f;
      if (arg0->unk6C8 == 0)
      {
        arg0->unk674 = 0.0f;
      }
    }
    if ((*arg0->unk36C) & 4)
    {
      arg0->unk190 = 0.0f;
    }
  }
}
