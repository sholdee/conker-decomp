#include <ultra64.h>
#define func_10010F30 func_10010F30_void
#define func_1513D4B8 func_1513D4B8_void
#include "functions.h"
#undef func_10010F30
#undef func_1513D4B8
extern s32 func_10010F30(s32, u16, u8, s16, u8);
extern void *func_1513D4B8(void *, void *, u8, u8, u8, u8, u8, s32, u8, s32);
#include "variables.h"

extern void func_151D5D60();
extern s32 func_15144E80(struct17 *, struct17 *, struct17 *, struct17 *);
extern s32 func_15145128(struct17 *, struct17 *, f32 *, f32 *);

typedef struct {
    char pad_0[0x197];
    u8 field_0x197;
} ObjectState197;

typedef struct {
    char pad_0[0x31C];
    ObjectState197 *field_0x31C;
} ObjectWithState197;

typedef struct {
    ObjectWithState197 *field_0x0;
    char pad_0[0x1E];
    u8 field_0x22;
    char pad_1[0x1];
    s32 field_0x24;
    s32 field_0x28;
    s32 field_0x2C;
    s32 field_0x30;
    s32 field_0x34;
} Field124ChildBlock;

typedef struct {
    char pad_0[0x18];
    s32 field_0x18;
    char pad_1[0x8C];
    s32 field_0xA8;
    char pad_2[0x24];
    s32 field_0xD0;
    u8 field_0xD4;
    char pad_3[0x4F];
    s32 field_0x124;
    char pad_4[0x48];
    s32 field_0x170;
    u8 field_0x174;
} Field124Owner;

typedef struct {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    char pad40[0x80];
    char unkC0[0x40];
    s16 *unk100[4];
    u8 unk110;
    u8 unk111;
    u8 unk112[0xC];
} Struct15101F50;


typedef struct {
    u8 *obj;
    u8 unk4;
    u8 pad5[3];
    struct17 unk8;
    struct17 unk14;
    u8 unk20;
    s8 unk21;
    u8 unk22;
    u8 unk23;
    u8 pad24[0x14];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    s16 unk90;
    u8 pad92[2];
} Data15100600;

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10, unk11, unk12, unk13;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44, unk45, unk46, unk47;
    u8 pad48[0x10];
} Spawn15100600;

typedef struct {
    u8 unk0;
    u8 pad1[0x13];
    void *unk14;
} Data2_15100600;

typedef struct {
    u8 unk0;
    s8 unk1;
    s16 unk2;
    u8 unk4;
    u8 pad5[3];
} Hdr15100600;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Hdr2_15100600;

typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14, unk15, unk16, unk17;
    u8 unk18, unk19, unk1A, unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    u8 unk64;
    u8 unk65;
    u8 pad66[0xA];
} Effect15100600;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    void *unk28;
    u8 unk2C;
    u8 pad2D[3];
} Data3_15100600;

typedef struct {
    f32 unk0, unk4, unk8, unkC, unk10, unk14, unk18, unk1C, unk20;
    s32 unk24, unk28, unk2C, unk30, unk34, unk38, unk3C, unk40, unk44;
    f32 unk48;
    s16 unk4C, unk4E, unk50, unk52;
    u8 unk54, unk55, unk56, unk57, unk58;
    u8 pad59[3];
} Emit15100600;

typedef struct {
    void *unk0;
    u8 unk4;
    u8 pad5[3];
} Data4_15100600;

extern void *func_15130374(void *, u8, s32, u8, s32);
extern void *func_151407D0(void *, s32, void *, u8, u8, u8, u8, s8, u8, s32);
extern void *func_151602C0(void *, void *, s32, u8, u8, u8, u8, u8, s32, u8, s32);
s32 func_15100600(u8 *arg0, f32 *arg1, f32 *arg2, u8 arg3, s16 arg4, u8 arg5, u8 arg6, u8 arg7, f32 arg8, f32 arg9, f32 argA, f32 argB, u8 argC, u8 argD, s32 argE);

typedef struct {
    u8 pad0[0x24];
    struct102 *unk24;
    struct102 *unk28;
    struct102 *unk2C;
    struct102 *unk30;
    struct102 *unk34;
} Block15101148;

void func_15101090(u8 *arg0) {
    Block15101148 *temp = (Block15101148 *)(arg0 + 0x110);

    if (*(u16 *)(arg0 + 0x1A0)) {
        func_100111C8(*(s16 *)(arg0 + 0x1A0));
    }
    if (temp->unk28 != NULL) {
        func_1516972C(temp->unk28);
    }
    if (temp->unk2C != NULL) {
        func_1516972C(temp->unk2C);
    }
    if (temp->unk24 != NULL) {
        func_1516972C(temp->unk24);
    }
    if (temp->unk30 != NULL) {
        func_1516972C(temp->unk30);
    }
    if (temp->unk34 != NULL) {
        func_1516972C(temp->unk34);
    }
    func_1513CA6C((struct210 *)arg0);
}



void func_15101148(u8 *arg0) {
    Block15101148 *temp = (Block15101148 *)(arg0 + 0x110);

    if (temp->unk28 != NULL) {
        func_1516979C(temp->unk28);
    }
    if (temp->unk2C != NULL) {
        func_1516979C(temp->unk2C);
    }
    if (temp->unk24 != NULL) {
        func_1516979C(temp->unk24);
    }
    if (temp->unk30 != NULL) {
        func_1516979C(temp->unk30);
    }
    if (temp->unk34 != NULL) {
        func_1516979C(temp->unk34);
    }
    func_1513CAA0((struct210 *)arg0);
}

void func_151011E8(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x124 + 0x110);
    temp->field_0x28 = 0;
    func_1513CA6C((struct210 *)arg0);
}

void func_15101210(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x124 + 0x110);
    temp->field_0x28 = 0;
    func_1513CAA0((struct210 *)arg0);
}

void func_151617C4(struct210 *arg0);

void func_15101238(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x18 + 0x110);

    temp->field_0x2C = 0;
    func_151617C4((struct210 *)arg0);
}

void func_151617E4(struct210 *arg0);

void func_15101260(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x18 + 0x110);

    temp->field_0x2C = 0;
    func_151617E4((struct210 *)arg0);
}

void func_1513173C(struct210 *);

void func_15101288(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0xD0 + 0x110);
    temp->field_0x24 = 0;
    func_1513173C((struct210 *)arg0);
}

void func_1513175C(struct210 *);

void func_151012B0(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0xD0 + 0x110);
    temp->field_0x24 = 0;
    func_1513175C((struct210 *)arg0);
}

void func_151012D8(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x170 + 0x110);
    temp->field_0x30 = 0;
    func_151411A4((struct210 *)arg0);
}

void func_15101300(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0x170 + 0x110);
    temp->field_0x30 = 0;
    func_151411C4((struct210 *)arg0);
}

void func_15101328(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0xA8 + 0x110);
    temp->field_0x34 = 0;
    func_1513173C((struct210 *)arg0);
}

void func_15101350(Field124Owner *arg0) {
    Field124ChildBlock *temp = (Field124ChildBlock *)(arg0->field_0xA8 + 0x110);
    temp->field_0x34 = 0;
    func_1513175C((struct210 *)arg0);
}

typedef struct { f32 x, y, z; } F3_15101378;
#pragma pack(1)
typedef struct { s32 a, b, c, d; } Pq_15101378;
#pragma pack()
typedef struct { Pq_15101378 q; u16 h; } Pk_15101378;
#pragma pack()
typedef struct { u8 flag; u8 pad1; Pk_15101378 data; } XS_15101378;
typedef struct { u8 pad0[0x34]; F3_15101378 p34; u8 pad1[0x110 - 0x40]; XS_15101378 s; } X_15101378;
typedef struct { u8 pad0[0x9]; u8 f9; u8 pad1[4]; s16 sE; s16 s10; s16 s12; } P_15101378;
typedef struct { u8 pad0[0x14]; P_15101378 *p14; } Y_15101378;
typedef struct { u8 pad[4]; u8 flag; } VS_15101378;
typedef struct { u8 pad0[0x40]; F3_15101378 p40; u8 pad1[0xA8 - 0x4C]; VS_15101378 s; } V_15101378;
typedef struct { u8 pad0[0x38]; f32 f38; f32 f3C; F3_15101378 p40; u8 pad1[0xA8 - 0x4C]; f32 fA8; f32 fAC; u8 pad2[0xD4 - 0xB0]; u8 flagD4; } Z_15101378;
typedef struct { u8 pad0[0x34]; F3_15101378 p34; F3_15101378 p40; u8 pad1[0x58 - 0x4C]; s32 s58; u8 pad2[0x174 - 0x5C]; u8 flag174; } W_15101378;
typedef struct { u8 pad0[0xB4]; f32 fB4; F3_15101378 pB8; u8 padC4[0xE8 - 0xC4]; F3_15101378 pE8; Pk_15101378 pF4; u8 pad2[3]; u8 u109; } C_15101378;
typedef struct { s32 s0; u8 pad4[0x3B - 4]; u8 u3B; u8 pad3C[0x74 - 0x3C]; u8 u74; u8 pad75[0x1D4 - 0x75]; u8 *p1D4; u8 pad1D8[0x31C - 0x1D8]; C_15101378 *c; } O_15101378;
typedef struct {
    O_15101378 *owner;
    u8 u4;
    u8 pad5[3];
    f32 at8[3];
    f32 at14[3];
    u8 u20; u8 pad21[2]; u8 u23;
    Z_15101378 *z24;
    X_15101378 *x28;
    Y_15101378 *y2C;
    W_15101378 *w30;
    V_15101378 *v34;
    f32 f38;
    u8 pad3C[8];
    f32 f44;
} B_15101378;
typedef struct {
    u8 pad0[0x34];
    F3_15101378 p34;
    F3_15101378 p40;
    u8 pad4C[0x110 - 0x4C];
    B_15101378 b;
} A_15101378;
extern void func_15143134(void *, void *, void *);
extern f32 func_15143E64(void *);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, void *, void *, void *, s32, s32, s32, f32);

s8 func_15101378(A_15101378 *arg0)
{
  s8 ret;
  C_15101378 *c;
  O_15101378 *owner;
  u8 hit;
  Pk_15101378 pk;
  F3_15101378 d;
  u8 ok;
  u8 *a2;
  F3_15101378 e;
  s32 tmp;
  s32 mode;
  s32 pad2;
  B_15101378 *b;
  u8 *ptr;
  ret = 1;
  owner = arg0->b.owner;
  b = &arg0->b;
  if ((owner->s0 == 0) || (owner->u3B != b->u4))
  {
    ret = 0;
  }
  else
  {
    ok = 1;
    ptr = owner->p1D4;
    if ((ptr != 0) && ((D_800C35EA != 1) || ((owner->u74 & 0xF) != 0xF)))
    {
      a2 = ((u8 (*)[0x40]) ptr)[b->u20];
      func_15143134(b->at8, &arg0->p34, a2);
      if (b->u23 & 2)
      {
        c = owner->c;
        mode = c->u109;
        if (mode != 0)
        {
          arg0->p40 = owner->c->pB8;
          b->f44 = owner->c->fB4;
          pk = owner->c->pF4;
          d.x = arg0->p40.x - arg0->p34.x;
          d.y = arg0->p40.y - arg0->p34.y;
          d.z = arg0->p40.z - arg0->p34.z;
        }
        else
        {
          ok = 0;
          mode = 2;
          b->f44 = 2000.0f;
          d.x = owner->c->pE8.x * 2000.0f;
          d.y = owner->c->pE8.y * 2000.0f;
          d.z = owner->c->pE8.z * 2000.0f;
          arg0->p40.x = arg0->p34.x + d.x;
          arg0->p40.y = arg0->p34.y + d.y;
          arg0->p40.z = arg0->p34.z + d.z;
        }
      }
      else
      {
        func_15143134(b->at14, &e, a2);
        e.x = e.x - arg0->p34.x;
        e.y = e.y - arg0->p34.y;
        e.z = e.z - arg0->p34.z;
        if (func_150AC9C0(arg0->p34.x, arg0->p34.y, arg0->p34.z, e.x, e.y, e.z, &tmp, &pk, &arg0->p40.x, &arg0->p40.y, &arg0->p40.z, 0, 0, 0, 0.0f) != (hit * 0))
        {
          d.x = arg0->p40.x - arg0->p34.x;
          d.y = arg0->p40.y - arg0->p34.y;
          d.z = arg0->p40.z - arg0->p34.z;
          hit = 1;
          b->f44 = func_15143E64(&d);
        }
        else
        {
          hit = 0;
        }
        mode = hit;
        lbl1:
        ;

        ;
      }
    }
    else
    {
      mode = 0;
    }
    if (mode != 0)
    {
      X_15101378 *x;
      XS_15101378 *xs;
      VS_15101378 *vs;
      V_15101378 *v;
      Y_15101378 *y;
      Z_15101378 *z;
      W_15101378 *w;
      b->u23 |= 1;
      if (b->x28 != 0)
      {
        x = b->x28;
        xs = &x->s;
        if ((mode == 1) && (ok != 0))
        {
          if (b->f44 > 40.0f)
          {
            f32 inv = 1.0f / b->f44;
            x->p34.x = arg0->p40.x - ((d.x * inv) * 20.0f);
            x->p34.y = arg0->p40.y - ((d.y * inv) * 20.0f);
            x->p34.z = arg0->p40.z - ((d.z * inv) * 20.0f);
          }
          else
          {
            x->p34 = arg0->p40;
          }
          xs->data = pk;
          xs->flag |= 1;
        }
        else
        {
          xs->flag &= ~1;
        }
      }
      if (b->v34 != 0)
      {
        v = b->v34;
        vs = &v->s;
        if (((mode == 2) || (mode == 3)) && (ok != 0))
        {
          v->p40 = arg0->p40;
          vs->flag |= 1;
        }
        else
        {
          vs->flag &= ~1;
        }
      }
      if (b->y2C != 0)
      {
        y = b->y2C;
        do {} while (0);
        y->p14->sE = (s32) arg0->p40.x;
        y->p14->s10 = (s32) arg0->p40.y;
        y->p14->s12 = (s32) arg0->p40.z;
        if (ok != 0)
        {
          y->p14->f9 = 0;
        }
        else
        {
          y->p14->f9 = 1;
        }
      }
      if (b->z24 != 0)
      {
        z = b->z24;
        z->p40 = arg0->p34;
        z->f38 = z->fA8 * b->f38;
        z->f3C = z->fAC * b->f38;
        z->flagD4 |= 1;
      }
      if (b->w30 != 0)
      {
        w = b->w30;
        w->p34 = arg0->p34;
        w->p40 = arg0->p40;
        w->s58 &= ~4;
        w->flag174 |= 1;
      }
    }
    else
    {
      X_15101378 *x;
      XS_15101378 *xs;
      VS_15101378 *vs;
      V_15101378 *v;
      Y_15101378 *y;
      Z_15101378 *z;
      W_15101378 *w;
      lbl2:
      ;

      ;
      b->u23 &= ~1;
      if (b->x28 != 0)
      {
        x = b->x28;
        x->s.flag &= ~1;
      }
      if (b->y2C != 0)
      {
        y = b->y2C;
        y->p14->f9 = 1;
      }
      if (b->z24 != 0)
      {
        z = b->z24;
        z->flagD4 &= ~1;
      }
      if (b->w30 != 0)
      {
        w = b->w30;
        w->flag174 &= ~1;
      }
      if (b->v34 != 0)
      {
        v = b->v34;
        v->s.flag &= ~1;
      }
    }
  }
  return ret;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_12E540/func_151019C4.s")
s16 *func_15101F50(Struct15101F50 *arg0, s16 arg1);

extern f32 D_800BE9A4;

typedef struct {
    u8 pad0[0x14];
    u8 *unk14;
} Sub151022AC_2C;

typedef struct {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x28];
    u8 unk5C;
} Sub151022AC_28;

typedef struct {
    u8 pad0[0x38];
    f32 unk38;
    f32 unk3C;
} Sub151022AC_34;

typedef struct {
    u8 pad0[0x24];
    u8 *unk24;
    Sub151022AC_28 *unk28;
    Sub151022AC_2C *unk2C;
    u8 pad30[4];
    Sub151022AC_34 *unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0xC];
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
} Block151022AC;
s32 func_151022AC(u8 *arg0);
s32 func_151025F4(u8 *arg0, s32 arg1);

extern f32 D_800A21A0[][3];
extern f32 D_800A21F4[][3];
extern u8 D_800A2248[];
s32 func_15100600(u8 *, f32 *, f32 *, u8, s16, u8, u8, u8, f32, f32, f32, f32, u8, u8, s32);
s32 func_151026BC(u8 *arg0, s16 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5);
void func_151027E8(struct210 *arg0);
s32 func_1510281C(Field124Owner *arg0, s16 arg1);
s32 func_15102884(struct210 *arg0, s32 arg1);
s32 func_151028AC(Field124Owner *arg0, s16 arg1);
