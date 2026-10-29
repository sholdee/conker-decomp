#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_15052590(struct127 *arg0);
void func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
extern f32 D_8009FFF0;

void func_150BDB70(struct127 *arg0)
{
  s32 sp5C;
  s32 sp58;
  s32 sp54;
  s32 sp50;
  struct127 *other;
  struct127 *obj;
  f32 angle;
  s32 i;
  s32 mask;
  sp5C = 1;
  if (arg0->id == 0x23)
  {
    sp5C = 10;
    sp58 = 11;
    sp54 = 12;
    sp50 = 1;
  }
  else
  {
    sp58 = 2;
    sp54 = 3;
    sp50 = 3;
  }
  arg0->unk2E8 = 0;
  arg0->pad2FC[0] = 0;
  arg0->unk2E4 -= arg0->unk2E4 >> 3;
  if (!D_800D18A0)
  {
  }
  obj = D_800CC2D0;
  if (D_800BE616 != 0)
  {
    arg0->pad66 &= ~0x20;
  }
  mask = arg0->unk13C;
  if (mask != 0)
  {
    obj += mask - 100;
    if (obj->camera != 0)
    {
      arg0->pad2FC[0] = 1 << ((struct127 *) obj->camera)->unk23D;
    }
    mask = (s32) obj->unk31C;
    if ((mask != 0) && (((struct127 *) mask)->unk197 != 0))
    {
      arg0->unk7A = (s32) (((struct127 *) mask)->unk16C * D_8009FFF0);
      arg0->unk76 = arg0->unk7A;
      angle = ((struct127 *) obj->unk31C)->unk170;
      if (angle > 180.0f)
      {
        angle -= 360.0f;
      }
      angle *= 10.0f;
      arg0->unk2E4 = angle;
      arg0->unk2E8 = 1;
      arg0->pad66 |= 0x20;
    }
  }
  func_15052590(arg0);
  if (sp58 == arg0->unk232)
  {
    func_15052F9C(arg0, 100.0f, sp50, 4, 0, sp54, 0, 0, 0, 0);
    return;
  }
  if (((sp5C == arg0->unk232) && ((~D_800D18A0) & D_800CC268)) && (func_15072208(arg0, 0) == 0))
  {
    mask = (~D_800D18A0) & D_800CC268;
    i = 0;
    do
    {
      while ((!((1 << i) & mask)) && (i < 25))
      {
        i++;
      }

      if (i >= 25)
      {
        return;
      }
      other = &D_800CC2D0[i];
      if (((((other->unk127 != 0xFF) && (other->unk65 == 0)) && ((*(((u8 *) other->unk31C) + 0x19B)) == 0)) && (((struct127 *) other->unk31C)->unk197 == 0)) && (other->unk13C == 0))
      {
        arg0->unk124 = i;
        func_1505327C(arg0, 44.0f, 3.2000000477f, sp58, sp50);
        *((u16 *) (((u8 *) other->unk31C) + 0x24)) = 600;
        *((u16 *) (((u8 *) other->unk31C) + 0x19E)) = 0;
        return;
      }
      i++;
    }
    while (i < 25);
  }
}
