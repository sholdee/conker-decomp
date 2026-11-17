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

typedef struct Game368C4State {
    u8 pad0[0x66];
    u8 mode;
    u8 pad67[0x16D];
    u8 *transforms;
    u8 pad1D8[0x129];
    u8 lighting0, lighting1;
    u8 pad303;
    s32 lights[4];
    u8 *lightData;
    u8 pad318[4];
    u8 *model;
} Game368C4State;

s32 func_1515E544(s32, s32, u8, u8, void *);
s32 func_15094F70(s32, void *, s32, void *, s32, s32, s32, s32, s32);
extern u8 D_80084350[], D_80089470[], D_800903BC[];

typedef struct Game63A20Command {
    u32 w0;
    u32 w1;
} Game63A20Command;

#define GAME63A20_STATE(index) ((Game368C4State *)((u8 *)D_800CC2D0 + (index) * 0x32C))

Game63A20Command *func_150368C4(Game63A20Command *arg0, s32 arg1, s32 arg2) {
    u8 *transform;
    u8 *effect;
    u8 *colors;
    s32 index;

    if (GAME63A20_STATE(arg1)->model[0x75] != 2 &&
        GAME63A20_STATE(arg1)->model[0x75] != 0x13) {
        return arg0;
    }
    effect = (u8 *)func_1503195C((struct127 *)GAME63A20_STATE(arg1), 8, 0);
    if (effect == NULL) {
        return arg0;
    }
    if (arg1 > 0) {
        return arg0;
    }
    colors = *(u8 **)(effect + 0x28 + D_800BE9C0 * 4) + 0x300;
    for (index = 0; index < 5; index++) {
        (*(u8 **)((u8 *)&D_800C3FC0 + arg1 * 4))[index * 0x10 + 0xC] = colors[0xC];
        (*(u8 **)((u8 *)&D_800C3FC0 + arg1 * 4))[index * 0x10 + 0xD] = colors[0xD];
        (*(u8 **)((u8 *)&D_800C3FC0 + arg1 * 4))[index * 0x10 + 0xE] = colors[0xE];
    }
    transform = GAME63A20_STATE(arg1)->transforms;
    if (transform == NULL) {
        return arg0;
    }
    func_15036570(arg1, (s32)(transform + 0x240), (s32)(transform + 0x100));
    { Game63A20Command *command = arg0++; command->w0 = 0xDE000000; command->w1 = (u32)D_80084350; }
    arg0 = (Game63A20Command *)func_1515E544((s32)arg0, GAME63A20_STATE(arg1)->lights[arg2],
                                                    GAME63A20_STATE(arg1)->lighting0, GAME63A20_STATE(arg1)->lighting1,
                                                    GAME63A20_STATE(arg1)->lightData + D_800BE9C0 * 8);
    if ((GAME63A20_STATE(arg1)->mode & 0xC) == 0xC) {
        { Game63A20Command *command = arg0++; command->w0 = 0xD9FDFFFF; command->w1 = 0; }
    } else if ((GAME63A20_STATE(arg1)->mode & 0xC) != 8 && !(effect[4] & 1)) {
        { Game63A20Command *command = arg0++; command->w0 = 0xD9FDFFFF; command->w1 = 0; }
    } else if (D_800DCD7C != 0) {
        { Game63A20Command *command = arg0++; command->w0 = 0xD9FFFFFF; command->w1 = 0x20000; }
    } else {
        { Game63A20Command *command = arg0++; command->w0 = 0xD9FDFFFF; command->w1 = 0; }
    }
    arg0 = (Game63A20Command *)func_15094F70((s32)arg0, (void *)D_800903BC, 0, NULL, 0, 0, 0, 2, 3);
    { Game63A20Command *command = arg0++; command->w0 = 0xDA380003; command->w1 = (u32)D_80089470; }
    { Game63A20Command *command = arg0++; command->w0 = 0x01001002; command->w1 = (u32)*(u8 **)((u8 *)&D_800C3FC0 + arg1 * 4); }
    { Game63A20Command *command = arg0++; command->w0 = 0xDA380003; command->w1 = (u32)(transform + 0x100); }
    { Game63A20Command *command = arg0++; command->w0 = 0x0100400A; command->w1 = (u32)(*(u8 **)((u8 *)&D_800C3FC0 + arg1 * 4) + 0x10); }
    { Game63A20Command *command = arg0++; command->w0 = 0x05000204; command->w1 = 0; }
    { Game63A20Command *command = arg0++; command->w0 = 0x05000608; command->w1 = 0; }
    return arg0;
}
