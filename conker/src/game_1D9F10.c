#include <ultra64.h>
/* functions.h declares func_150ADA20 as returning u8; its asm returns a full word
 * (dsra32 v0), and func_151AF388 spills the result as a word. File-local prototype. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_151D9B8C func_151D9B8C_s32_decl_in_functions_h
#define func_15048A40 func_15048A40_void_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
#undef func_151D9B8C
#undef func_15048A40
f32 func_15048A40(u8);
void func_151D9B8C(u8, f32, u8, void *, struct17 *, s16, u8, u8, u8, u8, s32);
u32 func_150ADA20(void);
#include "variables.h"

extern f32 D_800A9D84;
extern f32 D_800A9D88;
extern f32 D_800A9D80;
extern void func_151D5D60();
void *func_1513418C(void *, s32, s32, s32);
void func_1516962C(s32, struct102 *, s32);
void func_151AE984(f32 *, f32, f32, f32, s32, u8);
void func_151AEAB4(f32 *, s32, u8);
void func_15143134(f32 *, f32 *, s32);
void func_15135DD0(f32 *, f32 *, f32, u8, s32);
void func_151AF388(f32 *, f32, f32, f32, s32);

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ f32 unk4[3];
    /* 0x10 */ f32 unk10[3];
} struct_AF4D0;

typedef struct {
    char pad_0[0x74];
    u8 field_0x74;
    char pad_0x75[0x15F];
    s32 field_0x1D4;
} GameObjectTransformOwner;

typedef struct {
    char pad_0[0x44];
    s32 field_0x44;
    char pad_0x48[0x4];
    f32 field_0x4C;
    s8 field_0x50;
    s8 field_0x51;
    s16 field_0x52;
} ObjectFxPayload;

typedef struct {
    char pad_0[0x98];
    s8 field_0x98;
    char pad_0x99[0x3];
    ObjectFxPayload *field_0x9C;
    char pad_0xA0[0xE];
    u8 field_0xAE;
} ObjectFxControl;

typedef struct {
    char pad_0[0x3C];
    f32 field_0x3C;
    char pad_0x40[0x36];
    u16 field_0x76;
    char pad_0x78[0x2A4];
    ObjectFxControl *field_0x31C;
} GameObjectFxOwner;

typedef struct {
    char pad_0[0x10];
    s32 field_0x10;
    s32 field_0x14;
    u8 field_0x18;
    char pad_0x19[0x3];
    s32 field_0x1C;
    u8 field_0x20;
    char pad_0x21[0x3];
    s32 field_0x24;
    f32 field_0x28;
} ObjectLinkPayload;

typedef struct {
    char pad_0[0x4];
    s16 field_0x04;
    u16 field_0x06;
    f32 field_0x08;
    f32 field_0x0C;
    s16 field_0x10;
    u16 field_0x12;
    u8 field_0x14;
    u8 field_0x15;
    char pad_0x16[0x2];
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
} ObjectSpawnParams;

typedef struct {
    s32 field_0x00;
    s32 field_0x04;
    u8 field_0x08;
} ObjectSpawnPayload;

extern struct_AF4D0 D_800A9DF0[];
extern f32 D_800AA0E4;
extern s8 D_800A9023[];
extern ObjectSpawnParams D_800A9180[];

void func_15143794(s32, s32, f32, f32 *);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);

typedef struct {
    f32 unk0;
    s16 unk4;
    u16 unk6;
} FxSoundEntry;

typedef struct {
    f32 unk0;
    s16 unk4;
    f32 unk8;
} FxLightEntry;

typedef struct {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    s8 unk4;
} FxEntry5;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
} FxOwner;

extern FxEntry5 D_800A9020[];
extern FxSoundEntry D_800A9040[];
extern FxSoundEntry D_800A9158[];
extern FxLightEntry D_800A925C[];
void func_1514C678(f32, f32, s32, f32, s32, s32, s32, s32, s32, f32, ObjectSpawnPayload *, s32);
u8 func_151D8E20(void);
void func_151DBE80(u8, f32, f32, s16, f32 *, s32, s32, s32, s32, s32);
s32 func_151ABE40(f32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
} FxSprayParams;

typedef struct {
    f32 pos[3];
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} FxSpraySpawn;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 pad1C[4];
} FxSprayPart;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 pad1E[2];
} FxSprayStyle;

extern s8 D_800A9021[];
extern FxSprayParams D_800A9068[];
extern u8 D_800AB3F4[];
extern u8 D_800AB404[];
extern s32 func_15147DA0(FxSpraySpawn *, FxSprayPart *, s32, s32, s32, s32, s32, s32, s32, s32, s32, FxSprayStyle *, s32, s32, s32);
s32 func_151AC078(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, ObjectSpawnPayload *argD, u8 argE);
s32 func_151AC3CC(s16 *arg0);

u8 func_151D8E20(void);
void func_151DAB58(u8, f32, u8, f32 *, s32, u8, s32);

typedef struct {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
} ObjectEffectSample;

typedef struct {
    f32 field_0x00;
    char pad_0x04[0x17];
    u8 field_0x1B;
    char pad_0x1C[0x4];
    u8 field_0x20;
} ObjectEffectPayloadData;

typedef struct {
    char pad_0[0x1];
    u8 field_0x01;
    char pad_0x02[0xA];
    u8 field_0x0C;
    char pad_0x0D[0x20];
    s8 field_0x2D;
    char pad_0x2E[0x66];
    ObjectEffectSample *field_0x94;
    ObjectEffectPayloadData *field_0x98;
} ObjectEffectState;
s32 func_151AC408(ObjectEffectState *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5);
s32 func_151AC550(ObjectEffectState *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5);
s32 func_151AC61C(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, ObjectSpawnPayload *argD, u8 argE);

typedef struct {
    char pad_0[0x2C];
    f32 field_0x2C;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3C;
    char pad_0x40[0xC];
    f32 field_0x4C;
    f32 field_0x50;
    char pad_0x54[0x6C];
    char field_0xC0[0x40];
    s16 *field_0x100[1];
} ObjectGeometryState;
s16 *func_151AC810(ObjectGeometryState *arg0, s16 arg1);

extern f32 D_800BE9A4;
s32 func_151AC9EC(ObjectGeometryState *arg0);
s32 func_151ACA20(s16 *arg0);

extern u8 *func_15167A68(s32, s32, s32, s32, s32, s32);
s32 func_151ACB38(u8 *arg0, u8 *arg1);

s32 func_151ACA60(struct127 *arg0, f32 arg1, s32 arg2) {
    ObjectLinkPayload *temp;

    if (arg0 == 0) {
        return 0;
    }
    temp = (ObjectLinkPayload *)func_15167A68(0x30, 0, arg2 + 0x30, 1, 0xFF, 1);
    if (temp == 0) {
        return 0;
    }
    if (func_151ACB38((u8 *)arg0, &temp->field_0x18) == 0) {
        func_1516979C((struct102 *)temp);
        return 0;
    }
    temp->field_0x1C = (s32)arg0;
    temp->field_0x20 = arg0->unique_id;
    temp->field_0x24 = arg0 - D_800CC2D0;
    temp->field_0x28 = arg1;
    temp->field_0x10 = 1;
    temp->field_0x14 = 0;
    return (s32)temp;
}

s32 func_151ACB38(u8 *arg0, u8 *arg1) {
    s32 ret = 0;
    if (arg0[0x3B] == 1) {
        arg1[0] = 1;
        ret = 1;
    }
    return ret;
}

s32 func_151AE3A8(u8 *arg0);
s32 func_151ACB60(u8 *arg0)
{
  s32 result;
  s32 v0 = *((s32 *) ((*((s32 *) ((*((s32 *) (arg0 + 0x1C))) + 0x31C))) + 0x9C));
  if (v0 != 0)
  {
    result = func_151AE3A8(arg0);
    if (arg0)
    {
    }
    return result;
  }
}


void func_151ACB94(s32 arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0 + 0x1C, arg0 + 0x20, arg0);
}

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 (*unk38)[6];
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 pad43;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ Vtx *unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 pad50[2];
    /* 0x52 */ s16 unk52;
} Obj151ACBD4;

typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u8 padA[2];
} Tex151ACBD4;

extern Tex151ACBD4 D_80090B60[];
extern void *allocate_memory(s32, s32, s32, s32);

Obj151ACBD4 *func_151ACBD4(void *arg0, s32 arg1)
{
  Obj151ACBD4 *obj;
  Tex151ACBD4 *tex;
  s32 i;
  s32 n;
  s32 m;
  s32 j;
  s32 x;
  s32 y0;
  s32 y1;
  s32 z;
  s32 shift;
  s32 w;
  u8 r;
  u8 g;
  u8 b;
  s32 t;
  s32 h;
  s32 u;
  s32 c0;
  s32 c1;
  s32 c2;
  s32 c3;
  obj = (Obj151ACBD4 *) func_15167A68((D_800BE9F0 == 0x2C) ? (0x4E) : (0x2F), 0, arg1 + 0x58, 1, 0xFF, 1);
  if (obj == 0)
  {
    return 0;
  }
  memcpy(((u8 *) obj) + 0x10, arg0, 0x24);
  obj->unk44 = 0;
  obj->unk52 = 0;
  obj->unk34 = 1.0f / obj->unk2C;
  m = 19;
 obj->unk38 = allocate_memory(0x1E0, 1, 0, 0); obj->unk42 = m + 1;
  x = obj->unk14;
  z = obj->unk1C;
  y1 = obj->unk18;
  y0 = obj->unk24;
  for (i = 0; i < (m + 1); i++)
  {
    obj->unk38[i][0] = x;
    obj->unk38[i][1] = ((i * (y1 - y0)) / m) + y0;
    obj->unk38[i][2] = z;
    obj->unk38[i][3] = 0.0f;
 obj->unk38[i][4] = 0.0f; obj->unk38[i][5] = 0.0f; } obj->unk3C = ((f32) (y1 - y0)) / ((f32) m); obj->unk48 = allocate_memory(0xA00, 1, 2, 0); if (D_800BE9F0 == 0xB) { obj->unk40 = 0x37; shift = 4; } else { obj->unk40 = 0x36; shift = 5; } g = (r = 0xFF); if (D_800BE9F0 == 0x39) { r = 0x58; g = 0x4B; b = 0x2C; } else { b = 0xFF; } tex = &D_80090B60[obj->unk40];
  w = tex->unk6 >> 2;
  h = tex->unk8 - 1;
  c0 = 0x100 << 5;
  c1 = (w + 0x100) << 5;
  c2 = ((w * 2) + 0x100) << 5;
  c3 = ((w * 3) + 0x100) << 5;
  for (j = 0; j < 2; j++)
  {
    if (m)
    {
      for (i = 0, u = 0x100; i < 20; i++, u += h)
      {
        t = u << shift;
        obj->unk48[(((j * 20) + i) * 4) + 0].v.flag = 0;
 do { obj->unk48[(((j * 20) + i) * 4) + 0].v.tc[0] = c0; } while (0);
        obj->unk48[(((j * 20) + i) * 4) + 0].v.tc[1] = t;
        obj->unk48[(((j * 20) + i) * 4) + 0].v.cn[0] = r;
        obj->unk48[(((j * 20) + i) * 4) + 0].v.cn[1] = g;
        obj->unk48[(((j * 20) + i) * 4) + 0].v.cn[2] = b;
        obj->unk48[(((j * 20) + i) * 4) + 0].v.cn[3] = 0xFF;
        obj->unk48[(((j * 20) + i) * 4) + 1].v.flag = 0;
        obj->unk48[(((j * 20) + i) * 4) + 1].v.tc[0] = c1;
        obj->unk48[(((j * 20) + i) * 4) + 1].v.tc[1] = t;
        obj->unk48[(((j * 20) + i) * 4) + 1].v.cn[0] = r;
        obj->unk48[(((j * 20) + i) * 4) + 1].v.cn[1] = g;
        obj->unk48[(((j * 20) + i) * 4) + 1].v.cn[2] = b;
        obj->unk48[(((j * 20) + i) * 4) + 1].v.cn[3] = 0xFF;
        obj->unk48[(((j * 20) + i) * 4) + 2].v.flag = 0;
        obj->unk48[(((j * 20) + i) * 4) + 2].v.tc[0] = c2;
        obj->unk48[(((j * 20) + i) * 4) + 2].v.tc[1] = t;
        obj->unk48[(((j * 20) + i) * 4) + 2].v.cn[0] = r;
        obj->unk48[(((j * 20) + i) * 4) + 2].v.cn[1] = g;
        obj->unk48[(((j * 20) + i) * 4) + 2].v.cn[2] = b;
        obj->unk48[(((j * 20) + i) * 4) + 2].v.cn[3] = 0xFF;
        obj->unk48[(((j * 20) + i) * 4) + 3].v.flag = 0;
        obj->unk48[(((j * 20) + i) * 4) + 3].v.tc[0] = c3;
        obj->unk48[(((j * 20) + i) * 4) + 3].v.tc[1] = t;
        obj->unk48[(((j * 20) + i) * 4) + 3].v.cn[0] = r;
        obj->unk48[(((j * 20) + i) * 4) + 3].v.cn[1] = g;
        obj->unk48[(((j * 20) + i) * 4) + 3].v.cn[2] = b;
        obj->unk48[(((j * 20) + i) * 4) + 3].v.cn[3] = 0xFF;
      }

    }
  }

  obj->unk4C = 0;
  return obj;
}

typedef struct {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ f32 vel[3];
} RopeNode151AD174;

typedef struct {
    /* 0x000 */ char pad0[0xA8];
    /* 0x0A8 */ f32 unkA8;
    /* 0x0AC */ u8 unkAC;
    /* 0x0AD */ u8 unkAD;
    /* 0x0AE */ u8 unkAE;
} RopeCtl151AD174;

typedef struct {
    /* 0x000 */ char pad0[0x3C];
    /* 0x03C */ f32 unk3C;
    /* 0x040 */ char pad40[0x36];
    /* 0x076 */ u16 unk76;
    /* 0x078 */ char pad78[0x31C - 0x78];
    /* 0x31C */ RopeCtl151AD174 *unk31C;
} RopeActor151AD174;

typedef struct {
    /* 0x00 */ char pad0[0x38];
    /* 0x38 */ RopeNode151AD174 *nodes;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ char pad40[2];
    /* 0x42 */ u8 count;
    /* 0x43 */ char pad43;
    /* 0x44 */ RopeActor151AD174 *unk44;
    /* 0x48 */ char pad48[4];
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 unk50;
    /* 0x51 */ u8 unk51;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u16 unk54;
} Rope151AD174;

f32 func_150489B0(u8);
u16 func_10010E78(u16 arg0, s32 arg1, u16 arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9,
                   s32 argA);

void func_151AD174(Rope151AD174 *arg0)
{
  s32 angle;
  f32 force;
  s32 cnt;
  s32 n;
  s32 idx;
  f32 pos[3];
  f32 vel[3];
  f32 oldPos[20][3];
  f32 oldVel[20][3];
  s32 i;
  s32 j;
  f32 mid[3];
  f32 dir[3];
  f32 total;
  f32 dist;
  f32 hdist;
  f32 t;
  f32 h;
  f32 len;
  f32 zero;
  f32 scale;
  f32 pad;
  total = 0.0f;
  if (arg0->unk52 != 0)
  {
    arg0->unk52 -= D_800BE9E4;
    if (arg0->unk52 < 0)
    {
      arg0->unk52 = 0;
    }
  }
  n = arg0->count - 1;
  if (arg0->unk44 != 0)
  {
    idx = arg0->unk44->unk31C->unkAE;
    angle = (arg0->unk44->unk31C->unkAC + (arg0->unk44->unk76 >> 8)) & 0xFF;
    force = arg0->unk44->unk31C->unkA8 * 1.899999976f;
    arg0->nodes[idx].vel[0] += force * func_150489B0(angle);
    arg0->nodes[idx].vel[2] -= force * func_15048A40(angle);
    angle = arg0->unk44->unk76 >> 8;
    force = arg0->unk44->unk3C * 19.0f;
    arg0->nodes[idx].vel[0] += force * func_150489B0(angle);
    arg0->nodes[idx].vel[2] -= force * func_15048A40(angle);
    arg0->nodes[idx].vel[1] -= 1000.0f;
    cnt = arg0->unk44->unk31C->unkAD - 1;
    if (cnt <= 0)
    {
      arg0->unk44->unk31C->unkA8 = 0.0f;
      cnt = 0;
    }
    arg0->unk44->unk31C->unkAD = cnt;
  }
  if (arg0->unk4C != 0.0f)
  {
    arg0->nodes[arg0->unk51].vel[0] -= arg0->unk4C * func_150489B0(arg0->unk50);
    arg0->nodes[arg0->unk51].vel[2] += arg0->unk4C * func_15048A40(arg0->unk50);
    arg0->unk4C *= 0.8999999762f;
  }
  for (i = 0; i < (n + 1); i++)
  {
    for (j = 0; j < 3; j++)
    {
      oldPos[i][j] = arg0->nodes[i].pos[j];
      oldVel[i][j] = arg0->nodes[i].vel[j];
    }

  }

  len = 9.0f;
  zero = 0.0f;
  for (i = n - 1; i >= 0; i--)
  {
    for (j = 0; j < 3; j++)
    {
      pos[j] = oldPos[i][j];
      vel[j] = arg0->nodes[i].vel[j];
      dir[j] = pos[j] - oldPos[i + 1][j];
    }

    {
      f32 dx;
      f32 dy;
      f32 dz;
      dx = pos[0] - oldPos[i + 1][0];
      dy = pos[1] - oldPos[i + 1][1];
      dz = pos[2] - oldPos[i + 1][2];
      dist = sqrtf(((dx * dx) + (dy * dy)) + (dz * dz));
      if (dist == 0.0f)
      {
        dist = 1.0f;
      }
      len = 9.0f;
      t = sqrtf((dx * dx) + (dz * dz)) / dist;
      t = len * t;
      h = t * t;
      t = (len * len) - h;
      if (h <= (len * len))
      {
        t = sqrtf(t);
      }
      else
      {
        t = 0.0f;
      }
      h = len * len;
      dist = t / dist;
      dir[0] = dx * dist;
      dir[1] = dy * dist;
      dir[2] = dz * dist;
      dir[0] = zero - dir[0];
      dir[1] = (-len) - dir[1];
      dir[2] = zero - dir[2];
    }
    for (j = 0; j < 3; j++)
    {
      vel[j] += dir[j];
      vel[j] *= 0.9599999785f;
      if (i != 0)
      {
        mid[j] = ((oldPos[i - 1][j] + oldPos[i + 1][j]) * 0.5f) - pos[j];
        vel[j] += mid[j] * 1.24;
      }
      pos[j] += vel[j];
      if ((D_800BE9F0 == 0x39) && (pos[0] > 1150.0f))
      {
        pos[0] = 1150.0f;
      }
    }

    h = pos[2] - arg0->nodes[i + 1].pos[2];
    pad = pos[0] - arg0->nodes[i + 1].pos[0];
    hdist = pos[1] - arg0->nodes[i + 1].pos[1];
    total += fabsf(vel[0]);
    total += fabsf(vel[2]);
    t = sqrtf((h * h) + ((pad * pad) + (hdist * hdist)));
    if (t != 0.0f)
    {
      scale = arg0->unk3C / t;
    }
    else
    {
      scale = 1.0f;
    }
    for (j = 0; j < 3; j++)
    {
      arg0->nodes[i].pos[j] = arg0->nodes[i + 1].pos[j] + ((pos[j] - arg0->nodes[i + 1].pos[j]) * scale);
      arg0->nodes[i].vel[j] = arg0->nodes[i].pos[j] - oldPos[i][j];
    }

  }

  if ((total > 250.0f) && ((arg0->unk54 == 0) || (func_1000F3D0(arg0->unk54) == 0)))
  {
    arg0->unk54 = func_10010E78(0, (func_150ADA20() % 9) + 0x507, 0x7530, 0, 0, 0, (s32) pos[0], (s32) pos[1], (s32) pos[2], 0xFA, -0x7E0C);
  }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1D9F10/func_151AD92C.s")

void func_151AE0E4(u8 *arg0, u8 arg1);
void func_151AE264(GameObjectFxOwner *arg0);

void func_151AE06C(struct127 *arg0, struct133 *arg1) {
    u8 sp1F;
    u8 temp_a1;
    u8 sp1E;

    if (func_151ACB38((u8 *)arg0, &sp1F) != 0) {
        temp_a1 = arg1->unk1B;
        if (*((u8 *)arg0->unk31C + 0x98) == 0) {
            func_151AE0E4((u8 *)arg0, temp_a1);
        } else if (temp_a1 == *((u8 *)arg0->unk31C + 0x98)) {
        } else {
            sp1E = temp_a1;
            func_151AE264((GameObjectFxOwner *)arg0);
            func_151AE0E4((u8 *)arg0, sp1E);
        }
    }
}

typedef struct {
    u8 pad0[4];
    f32 unk4;
    u8 pad8[0x10];
} Key151AE2BC;

typedef struct {
    u8 pad0[0x38];
    Key151AE2BC *unk38;
    u8 pad3C[6];
    u8 unk42;
} Track151AE2BC;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8 padC[0xC];
} Key151AE0E4;

typedef struct {
    u8 pad0[0x38];
    Key151AE0E4 *keys;
    f32 unk3C;
    u8 pad40[2];
    u8 unk42;
    u8 pad43;
    void *unk44;
    u8 pad48[0xA];
    s16 unk52;
} Node151AE0E4;

typedef struct {
    u8 pad0[0x98];
    u8 unk98;
    u8 pad99[3];
    Node151AE0E4 *unk9C;
    f32 unkA0;
} Ctl151AE0E4;

typedef struct {
    u8 pad0[0x14];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0x6A];
    u8 unk8A;
    u8 pad8B[0x79];
    u8 unk104;
    u8 pad105[0xC5];
    u8 unk1CA;
    u8 pad1CB[0x151];
    Ctl151AE0E4 *unk31C;
} Obj151AE0E4;

typedef struct Node151AE590 {
    u8 pad0[0x8];
    struct Node151AE590 *next;
    u8 padC[0x4];
    u8 unk10;
} Node151AE590;

Node151AE590 *func_151AE590(u8 arg0);
void func_151AE2BC(f32 *arg0, Track151AE2BC *arg1, f32 arg2, f32 arg3, f32 arg4);

void func_151AE0E4(u8 *arg0, u8 arg1) {
    Node151AE0E4 *node;
    f32 pos[3];
    s32 i;
    s32 n;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;

    node = (Node151AE0E4 *)func_151AE590(arg1);
    if (node == NULL) {
        while (1) {
        }
    }
    if (node->unk52 == 0 && ((Obj151AE0E4 *)arg0)->unk1CA != 0 && ((Obj151AE0E4 *)arg0)->unk104 == 0) {
        func_151AE2BC(&((Obj151AE0E4 *)arg0)->unk31C->unkA0, (Track151AE2BC *)node, ((Obj151AE0E4 *)arg0)->x, ((Obj151AE0E4 *)arg0)->y, ((Obj151AE0E4 *)arg0)->z);
        n = 140.0f / node->unk3C;
        i = (f32)(node->unk42 - n - 1) * ((Obj151AE0E4 *)arg0)->unk31C->unkA0;
        pos[0] = node->keys[i].x;
        pos[1] = node->keys[i].y;
        pos[2] = node->keys[i].z;
        dist = (pos[0] - ((Obj151AE0E4 *)arg0)->x) * (pos[0] - ((Obj151AE0E4 *)arg0)->x) + (pos[1] - ((Obj151AE0E4 *)arg0)->y) * (pos[1] - ((Obj151AE0E4 *)arg0)->y) + (pos[2] - ((Obj151AE0E4 *)arg0)->z) * (pos[2] - ((Obj151AE0E4 *)arg0)->z);
        if (!(10000.0f < dist)) {
            ((Obj151AE0E4 *)arg0)->unk31C->unk98 = arg1;
            ((Obj151AE0E4 *)arg0)->unk8A = 0x12;
            ((Obj151AE0E4 *)arg0)->unk31C->unk9C = node;
            node->unk44 = arg0;
        }
    }
}

extern f32 D_800A9294;

void func_151AE264(GameObjectFxOwner *arg0) {
    f32 temp = arg0->field_0x3C * D_800A9294;
    ObjectFxPayload *v0 = arg0->field_0x31C->field_0x9C;
    v0->field_0x4C = temp;
    v0->field_0x50 = arg0->field_0x76 >> 8;
    v0->field_0x51 = arg0->field_0x31C->field_0xAE;
    v0->field_0x52 = 0x14;
    v0->field_0x44 = 0;
    arg0->field_0x31C->field_0x98 = 0;
    arg0->field_0x31C->field_0x9C = 0;
}

void func_151AE2BC(f32 *arg0, Track151AE2BC *arg1, f32 arg2, f32 arg3, f32 arg4) {
    s32 n;
    s32 i;
    f32 d;

    n = arg1->unk42;
    for (i = 0; i < n - 1; i++) {
        if (arg3 <= arg1->unk38[i].unk4) {
            break;
        }
    }
    if (i >= n - 2) {
        *arg0 = 1.0f;
        return;
    }
    if (i == 0) {
        *arg0 = 0.0f;
        return;
    }
    {
        Key151AE2BC *k = &arg1->unk38[i];

        d = k->unk4 - k[-1].unk4;
        d = (d != 0.0f) ? (arg3 - k[-1].unk4) / d : 0.0f;
    }
    *arg0 = (f32)(i - 1) / (f32)(n - 3) + d / (f32)(n - 1);
}

typedef struct {
    u8 pad0[0x9C];
    void *unk9C;
    f32 unkA0;
    u8 padA4[0xA];
    u8 unkAE;
} Ctl151AE3A8;

typedef struct {
    u8 pad0[0x14];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0x2FC];
    Ctl151AE3A8 *unk31C;
} Obj151AE3A8;

s32 func_151AE3A8(u8 *arg0) {
    f32 oz;
    f32 oy;
    f32 ox;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 speed;
    f32 frac;
    Ctl151AE3A8 *ctl;
    Obj151AE3A8 *obj;
    Node151AE0E4 *node;
    u8 angle;
    f32 s;
    f32 c;
    s32 n;
    s32 i;
    f32 t;
    s32 pad;
    f32 a[3];
    f32 b[3];

    obj = *(Obj151AE3A8 **)(arg0 + 0x1C);
    node = (Node151AE0E4 *)obj->unk31C->unk9C;
    angle = func_15143E08((struct127 *)obj) - 0x80;
    s = func_151423D8(angle - 0x40);
    c = func_151423D8(angle);
    ctl = obj->unk31C;
    frac = ctl->unkA0;
    speed = *(f32 *)(arg0 + 0x28);
    ox = obj->x;
    oy = obj->y;
    oz = obj->z;
    n = 140.0f / node->unk3C;
    {
        s32 pad3;

        i = (f32)(node->unk42 - n - 1) * frac;
    }
    ctl->unkAE = i;
    t = (f32)(node->unk42 - n - 1) * frac - (f32)i;
    a[0] = node->keys[i].x;
    a[1] = node->keys[i].y;
    a[2] = node->keys[i].z;
    b[0] = node->keys[i + 1].x;
    b[1] = node->keys[i + 1].y;
    b[2] = node->keys[i + 1].z;
    {
        s32 pad2;

        nx = a[0] + (b[0] - a[0]) * t;
        ny = a[1] + (b[1] - a[1]) * t;
        nz = a[2] + (b[2] - a[2]) * t;
        nx += speed * s;
        nz += speed * c;
    }
    obj->x = (nx - ox) + ox;
    obj->y = (ny - oy) + oy;
    obj->z = (nz - oz) + oz;

}


extern Node151AE590 *D_800DCE50[][104];
extern s32 D_800A9270[];

Node151AE590 *func_151AE590(u8 arg0) {
    u8 i;
    u8 j;
    Node151AE590 *n;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            n = D_800DCE50[j][D_800A9270[i]];
            while (n != NULL) {
                Node151AE590 *next = n->next;

                if (n->unk10 == arg0) {
                    return n;
                }
                n = next;
            }
        }
    }
    return NULL;
}

void func_151AE640(s32 *arg0, s32 *arg1, u8 arg2)
{
  s32 *ptr = arg0;
  s32 temp;
  if (arg2 == ((unsigned short) 0))
  {
    temp = arg1[0];
    if (temp == ptr[0x11])
    {
      ptr[0x11] = 0;
    }
 dummy_label_988571: ;
  }
  else
    if (arg2 == 0x2D)
  {
    temp = arg1[0];
    if (temp == ptr[0x11])
    {
      ptr[0x11] = arg1[1];
    }
    else
      if (arg1[1] == ptr[0x11])
    {
      ptr[0x11] = temp;
    }
  }
}


extern f32 D_800A9D70;
s32 func_15045800(f32 *arg0, s32 arg1, f32 arg2, s32 arg3);
s32 func_151ABE40(f32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_10010FFC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_151AE6B0(u8 *arg0);
void *func_151AE7B0(struct102 *arg0, f32 arg1, s16 arg2, u8 arg3, s32 arg4);
void func_151AE890(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, u8 *arg6);
void func_151AE984(f32 *arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, u8 arg5);

f32 func_150ADA68(void);
f32 func_151423D8(u8);
extern f32 D_800A9D98;
extern f32 D_800A9D9C;
extern f32 D_800A9DA0;
extern f32 D_800A9DA4;
void func_151AEAB4(f32 *, s32, u8);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32, s32, u8, s32, s16, s16, s32, u8, s32);
void func_151AEAB4(f32 *arg0, s32 arg1, u8 arg2);


typedef struct {
    s16 field00;
    s16 field02;
    s16 field04;
    s16 field06;
    f32 position[3];
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    s16 field2C;
    s16 field2E;
    s16 field30;
    s16 field32;
    s16 field34;
    s16 field36;
    s16 field38;
    s16 field3A;
    u8 field3C;
    u8 pad3D[3];
    f32 field40;
    s16 field44;
    s16 field46;
    s32 field48;
} Packet_151AECA0;

typedef struct {
    f32 y;
    u8 unk4[0x19];
    u8 unk1D;
    u8 pad1E[6];
} Floor_151AECA0;

void func_15153F18(s16 *, void *, s32, s32, s32);
void func_1504715C(void *, void *);
s32 func_15046C80(f32 *, s32, f32, void *);
void func_15143874(s16, f32, f32 *, f32 *);
void func_15136698(f32, f32, u8, u8, s32, void *, f32 *, s32, s32, s32, s32);
void func_151AECA0(f32 *arg0, u8 arg1, s32 arg2);

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ struct102 *unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11[0x3];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ u8  unk2A;
    /* 0x2B */ u8  unk2B;
    /* 0x2C */ s8  unk2C;
    /* 0x2D */ u8  unk2D;
} struct_AF270;

extern f32 D_800A9DCC;
extern f32 D_800A9DD0;
void func_1516962C(s32, struct102 *, s32);
void *func_1513418C(void *, s32, s32, s32);
void *func_151AF270(struct102 *arg0, s32 arg1, s32 arg2);
void func_151AF338(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, void *arg6);

typedef u32 (*RandU32Fn_151AF388)(void);
typedef void (*SpawnFn_151AF388)(f32 *, f32 *, s32, f32, s32, s32, f32, s32,
                                 f32, f32, s32, s32, s32, s32, s32, s32);
void func_151AF388(f32 *arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4);
void func_151AF4D0(GameObjectTransformOwner *arg0, s32 arg1, s32 arg2, s32 arg3);
