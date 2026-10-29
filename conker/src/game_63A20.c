#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 pad6[0x1A];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 pad26[0xA];
    s16 unk30;
    s16 unk32;
    s16 unk34;
} Struct_15036570;

void func_150A7960(s32, f32, f32, f32, f32 *, f32 *, f32 *);

void func_15036570(s32 arg0, s32 arg1, s32 arg2)
{
  f32 x;
  f32 y;
  f32 z;
  f32 tx;
  f32 ty;
  f32 tz;
  Struct_15036570 *obj;
  struct127 *o;
  s32 id;
  f32 dist;
  s16 *v;
  f32 t;
  f32 d;
  obj = ((Struct_15036570 **) (&D_800C3FC0))[arg0];
  o = &D_800CC2D0[arg0];
  id = (*o).unk84.uh;
  dist = (*o).unk2D0->unk8;
  if ((((id == 0x3E) || (id == 0x41)) || ((id == 0x138) && (dist >= 36.0f))) || (((id == 0x3D) || (id == 0xD9)) && (dist > 21.0f)))
  {
    func_150A7960(arg1, -7.0f, -17.0f, -7.0f, &x, &y, &z);
    v = (s16 *) (((s32) (&D_800C3FC4)) + (arg0 * 6));
    v[0] = x;
    v[1] = y;
    v[2] = z;
    d = tz - v[2];
  }
  else
  {
    if (((((id == 0x3D) || (id == 0xD9)) && (dist > 7.0f)) || (id == 0x138)) || (id == 0x139))
    {
      t = 1.0f;
    }
    else
    {
      t = 0.6999999881f;
    }
    x = (obj->unk30 + obj->unk20) * 0.5f;
    y = (obj->unk32 + obj->unk22) * 0.5f;
    z = (obj->unk34 + obj->unk24) * 0.5f;
    func_150A7960(arg2, x, y, z, &tx, &ty, &tz);
    v = (s16 *) (((s32) (&D_800C3FC4)) + (arg0 * 6));
    d = tx - v[0];
    v[0] = v[0] + (d * t);
    d = ty - v[1];
    v[1] = v[1] + (d * t);
    d = tz - v[2];
    v[2] = v[2] + (d * t);
    x = v[0];
    y = v[1];
    z = v[2];
  }
  obj->unk0 = x;
  obj->unk2 = y;
  obj->unk4 = z;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_63A20/func_150368C4.s")
