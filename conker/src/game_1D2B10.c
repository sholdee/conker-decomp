#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

extern f32 D_800A8DB0;

extern void func_151A6F00(void *, s32, u8, s32);
extern void func_151D5334(void *, f32, f32, f32, s32, u8, s32);
extern void func_151D3FF4(void *, u8, s32);
extern void func_151D5514(void *, u8, s32);
extern void func_151541B8(void *, f32, s32, f32, f32, u8, s32);

typedef struct { s16 x; s16 y2; s16 z; s16 r6; s16 h8; s16 rA; u8 pad[4]; f32 ang; u8 b14; u8 b15; } F5660Src;
typedef struct {
    F5660Src *src; f32 f4; f32 f8; f32 fC; f32 f10; f32 f14; f32 f18; f32 f1c; f32 f20;
    s16 s24; s16 s26; f32 f28; f32 f2c; f32 f30; f32 f34; f32 f38; f32 f3c;
    u8 b40; u8 b41; u8 b42; u8 pad43; u8 pad44[4]; f32 f48; u8 pad4c[0x20]; s32 h6c;
} F5660Obj;
typedef struct { f32 x, y, z, f8c, fcc, fd0, fd4, fd8; s16 sdc, sde, se0; u8 be2, be3, be4, be5; s16 se6; u8 be8, be9, bea, beb; s32 sec; u8 bf0; f32 ff4, ff8, ffc, f100; } F5660P;
extern s32 func_151464B8(s32);
extern s32 func_15046C80(f32 *, s32, f32, f32 *);
extern void func_15149550(void *, s32, s32, s32, s32, s32);
void func_151A5660(u8 *arg0)
{
  F5660Obj *s0;
  F5660P P;
  f32 fa_2;
  f32 fb_2;
  f32 fc_2;
  f32 fd_2;
  f32 fa_0;
  f32 fb_0;
  f32 fc_0;
  f32 fa_1;
  f32 fb_1;
  f32 fc_1;
  f32 fd;
  f32 fe;
  f32 y0;
  f32 y1;
  u32 r;
  u32 r8;
  u8 a;
  f32 sp[3];
  s0 = (F5660Obj *) (arg0 + 0x28);
  if ((*((s32 *) (arg0 + 0x28))) == 0)
  {
    *((s16 *) (arg0 + 0xE)) = -1;
    return;
  }
  if ((s0->src->b14 != 1) && ((s0->h6c == 0) || (func_151464B8(s0->h6c) == 0)))
  {
    s0->fC = s0->fC + ((s0->f8 * s0->f4) * D_800BE9A4);
    if (s0->fC > 1.0f)
    {
      P.sdc = 0x6231;
      P.sde = 0x1A4D;
      P.be2 = 0;
      P.be3 = 0;
      P.be4 = 0;
      P.be5 = 0xFF;
      P.se6 = 1;
      P.be9 = 0xFF;
      P.sec = 0;
      P.bea = 0;
      P.beb = 0;
      P.bf0 = 0x10;
      P.ff8 = s0->f10;
 do { switch (s0->src->b15 & 3) { default: P.x = s0->src->x; P.z = s0->src->z; y1 = s0->src->y2 + s0->src->h8; y0 = s0->src->y2 - s0->src->h8;
            break;

          case 2:
            r8 = ((u32) (s0->src->ang * 0.7111111283f)) & 0xFF;
            fa_2 = func_151423D8(r8 - 0x40);
            fb_2 = func_151423D8(r8);
            fc_2 = (func_150ADA68() * (2.0f * s0->src->r6)) + (-s0->src->r6);
            fd_2 = (func_150ADA68() * (2.0f * s0->src->rA)) + (-s0->src->rA);
            P.x = s0->src->x + ((fc_2 * fb_2) + (fd_2 * fa_2));
            P.z = s0->src->z + ((fd_2 * fb_2) - (fc_2 * fa_2));
          {
            extern s32 padx;
            y1 = s0->src->y2 + s0->src->h8;
            y0 = s0->src->y2 - s0->src->h8;
          }
            break;

          case 0:
            a = func_150ADA20() & 0xFF;
            fa_0 = func_151423D8(a - 0x40);
            fb_0 = func_151423D8(a);
            fc_0 = func_150ADA68() * s0->src->r6;
            P.x = s0->src->x + (fc_0 * fb_0);
            P.z = s0->src->z - (fc_0 * fa_0);
          {
            extern s32 padx;
            y1 = s0->src->y2 + s0->src->h8;
            y0 = s0->src->y2;
          }
            break;

          case 1:
            a = func_150ADA20() & 0xFF;
            fa_1 = func_151423D8(a - 0x40);
            fb_1 = func_151423D8(a);
            fc_1 = func_150ADA68() * s0->src->r6;
            P.x = s0->src->x + (fc_1 * fb_1);
            P.z = s0->src->z - (fc_1 * fa_1);
          {
            extern s32 padx;
            y1 = s0->src->y2 + s0->src->h8;
            y0 = s0->src->y2 - s0->src->h8;
          }
            break;

        }

        if (s0->b40 != 0)
        {
          sp[0] = P.x;
          sp[1] = y1;
          sp[2] = P.z;
          if (func_15046C80(sp, 0, y0, &s0->f48) != 0)
          {
            P.y = s0->f48;
          }
          else
          {
            extern s32 padx;
            P.y = y0;
          }
        }
        else
        {
          P.y = y0;
        }
        fd = (func_150ADA68() * s0->f18) + s0->f14;
        fe = (func_150ADA68() * s0->f20) + s0->f1c;
        P.f8c = 22.60000038f * fd;
        P.fd0 = 13.78600025f * fd;
        P.fcc = 57.96900177f * fe;
        P.fd4 = 42.7140007f * fe;
        P.fd8 = 118.7630005f * fe;
        P.se0 = (func_150ADA20() % ((u32) (s0->s26 + 1))) + s0->s24;
        P.ff4 = (func_150ADA68() * s0->f2c) + s0->f28;
        P.ffc = (func_150ADA68() * s0->f34) + s0->f30;
        P.f100 = (func_150ADA68() * s0->f3c) + s0->f38;
        P.be8 = (func_150ADA20() % ((u32) (s0->b42 + 1))) + s0->b41;
        func_15149550(&P, 0xA, 0, 0, arg0[0xC], arg0[1]);
        s0->fC = s0->fC - 1.0f;
      }
      while (s0->fC > 1.0f);
    }
  }
}

void func_151A5D2C(u16 soundHandle);

/* Effect state embedded at offset 0x58 of the struct260 passed to the two functions below. */
typedef struct {
    u8 pad0[0x14];
    /* 0x14 */ u16 soundHandle;
} EffectSoundState;

/* Releases the effect's sound (if any), then hands the effect to func_1514933C. */
void func_151A5CAC(struct260 *arg0) {
    EffectSoundState *state = (EffectSoundState *)((u8 *)arg0 + 0x58);

    // matching: the test reads through arg0 and the call through state, so IDO loads the handle twice
    if (*(u16 *)((u8 *)arg0 + 0x6C) != 0) {
        func_151A5D2C(state->soundHandle);
    }
    func_1514933C(arg0);
}

/* Releases the effect's sound (if any), then hands the effect to func_15149368. */
void func_151A5CEC(struct260 *arg0) {
    EffectSoundState *state = (EffectSoundState *)((u8 *)arg0 + 0x58);

    // matching: the test reads through arg0 and the call through state, so IDO loads the handle twice
    if (*(u16 *)((u8 *)arg0 + 0x6C) != 0) {
        func_151A5D2C(state->soundHandle);
    }
    func_15149368(arg0);
}

/* Releases a sound handle. */
void func_151A5D2C(u16 soundHandle) {
    func_100111C8(soundHandle);
}

typedef struct {
    s32 field_0x00;
    s16 field_0x04;
    u8 field_0x06;
    u8 field_0x07;
    s32 field_0x08;
    s32 field_0x0C;
    u8 field_0x10;
    u8 field_0x11;
    u8 field_0x12;
    u8 field_0x13;
    u8 field_0x14;
    u8 field_0x15;
    u8 field_0x16;
    u8 field_0x17;
    s32 field_0x18;
    s32 field_0x1C;
    u8 field_0x20;
    u8 pad_0x21;
    s16 field_0x22;
    s16 field_0x24;
    u8 pad_0x26[0x6];
} SparkSpawnParams;


struct210 *func_151A5D58(f32 arg0, u8 arg1, s32 arg2, f32 *arg3, s16 arg4, u8 arg5, u8 arg6, u8 arg7, s32 arg8) {
    SparkSpawnParams tmp;

    if (arg4 <= 0) {
        return NULL;
    }
    tmp.field_0x22 = 0x14;
    tmp.field_0x24 = 0xC;
    tmp.field_0x06 = 0x38;
    tmp.field_0x07 = 0;
    tmp.field_0x00 = ((arg4 == -1) ? 0 : 1) | (arg5 ? 0x400 : 0) | 0x100 | 0x200 | 0x1000 | 0x2000 | 0x8000 | 0x20000;
    if (arg4 == -1) {
        tmp.field_0x04 = 0x12C;
    } else {
        tmp.field_0x04 = arg4 + 0x14;
    }
    tmp.field_0x08 = 0;
    if (arg4 == -1) {
        tmp.field_0x0C = 0;
    } else {
        tmp.field_0x0C = 0x140000 / (arg4 + 0x14);
    }
    tmp.field_0x10 = arg1;
    tmp.field_0x11 = 0xFF;
    tmp.field_0x12 = 0xFF;
    tmp.field_0x13 = 0x83;
    tmp.field_0x14 = 0x1E;
    tmp.field_0x15 = 0xFF;
    tmp.field_0x18 = (arg6 ? 2 : 3) + 0x440000;
    tmp.field_0x16 = 0;
    tmp.field_0x17 = 6;
    tmp.field_0x20 = 0xFF;
    return func_1513C650((s32)&tmp, 0, 0, arg2, arg3[0], arg3[1], arg3[2], arg0, arg0,
                         func_150ADA20() & 0xFF, (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2),
                         arg5 ? 3 : 0, arg5 ? 0xFF : 0, 0, arg7, arg8);
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} Obj151A6068;

void func_151A6068(Obj151A6068 *arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5);

typedef struct Game1D3420Message {
    s32 count;
    u8 *ids;
    s32 value;
} Game1D3420Message;

typedef struct Game1D3420Target {
    u8 *object;
    u8 id;
    u8 pad05[3];
    s32 argument;
    u8 flags;
    u8 pad0D[3];
    s32 value;
} Game1D3420Target;
void func_151A5F70(u8 *arg0, void *arg1, u8 arg2);

typedef struct {
    Obj151A6068 *unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    f32 unk18;
} Ext151A6068;

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
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
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    u8 unk4C;
    u8 pad4D[0xB];
} Spawn151A6068;

extern s32 func_1516284C(Header *, s32 *, s32, s32, s32, u8, u8, s32, u8, u8, s32);
void func_151A6068(Obj151A6068 *arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, s32 arg5);

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    s8 unk60;
    s8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    s8 unk65;
    u8 pad66[2];
} Spawn151A6600;

typedef struct {
    s16 *unk0;
    f32 unk4;
    f32 unk8;
    u8 padC[8];
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 pad17;
    f32 unk18;
} Sub151A6600;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x27];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 pad40[0x20];
    s32 unk60;
    u8 pad64[0xAC];
    Sub151A6600 unk110;
    u8 pad12C[0x127 - 0x12C + 0x10];
} Obj151A6600;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);
extern void func_15143794(s16, s16, f32, struct17 *);
extern void func_15143874(s16, f32, f32 *, f32 *);
extern f32 func_15144528(f32, f32, f32);
extern s32 func_151464B8(s32);
s32 func_151A6600(Obj151A6600 *arg0);

extern void func_1000FD38(void *, void *, s32);
extern s32 func_1000EF40();
void func_151A6AB8(struct210 *arg0);

extern void func_151A6AB8(struct210 *);
void func_151A6B10(struct210 *arg0);
void func_151A6B3C(struct210 *arg0);

struct Func151A6BD8Obj;
extern struct Func151A6BD8Obj *func_151A6BD8(s32 arg0);

typedef struct Obj151A6B68 {
    char pad0[0x7A];
    /* 0x7A */ u16 unk7A;
} Obj151A6B68;

extern void func_1505D024(Obj151A6B68 *, s32, u16, s32);
void func_151A6B68(Obj151A6B68 *arg0, s32 arg1);

extern s32 D_800A5770;
extern s32 D_800DCE50[][0x68];
typedef struct Func151A6BD8Obj
{
  u8 pad0[8];
  struct Func151A6BD8Obj *next;
  u8 padC[7];
  u8 type;
  u8 pad14[0x14];
  s32 owner;
} Func151A6BD8Obj;
Func151A6BD8Obj *func_151A6BD8(s32 arg0);
void func_151A6C90(void *arg0, u8 arg1, s32 arg2);
