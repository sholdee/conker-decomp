#include <ultra64.h>

#define func_150ADA20 func_150ADA20_u8_unused
#include "functions.h"
#undef func_150ADA20
#include "variables.h"


void func_150AE5B0(struct108 *arg0) {
    if ((D_800D2E4C->unk4 & 0x80) == 0) {
        if (func_1509BE40(0, 0x2000, 0xBB) != -1) {
            if (func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 8) != 0) {
                arg0->unk84 |= 0x1000000;
                func_151254F4(arg0, D_800CC335 - 1);
            }
        } else if (func_151239CC(arg0, 8) != 0) {
            func_151254F4(arg0, 0);
        }
    }
    arg0->unk84 &= -0x4001;
    if ((D_800D2E4C->unk1 & 4) == 0) {
        if (func_1509BE40(1, 0x2000, 0x95, func_1509BE40(0, 0x2014, 0xB7) | 0x2000) != 0) {
            arg0->unk84 |= 0x1000000;
            if (((arg0->unk2C & 1) != 0) && (func_15123934(arg0, arg0->unk2C, 0, arg0->unk134, 0) != 0)) {
                arg0->unk1B4 = 3;
                arg0->unk84 &= -5;
                func_15124B18(arg0);
            }
        } else if (func_151239CC(arg0, 0) != 0) {
            func_15124B18(arg0);
            arg0->unk84 &= 0xFEFFFFFF;
        }
    }
    if (func_1509BE40(1, 0x4082, 6, 0x9000) != 0) {
        arg0->unk84 |= 0x10000;
    } else {
        arg0->unk84 &= 0xFFFEFFFF;
    }
}


typedef struct {
    f32 pos[3];
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} Spawn150AE790;

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
} Part150AE790;

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
} Style150AE790;

typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    s8 unk1F;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s8 unk30;
    s8 unk31;
    s8 unk32;
    s16 unk34;
} Packet150AE790;

typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x180 - 0x3C];
    f32 unk180;
    u32 unk184;
    u8 pad188[0x1D4 - 0x188];
    s32 unk1D4;
} Obj150AE790;

typedef struct {
    Obj150AE790 *obj;
    u8 unk4;
    u8 unk5;
} Sub150AE790;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Sub150AE790 sub;
} Arg150AE790;

extern s32 D_8008CB14;
extern s32 func_150ADA20(void);
void func_15142314(s32, s32, f32 *);
extern s32 func_15147DA0(Spawn150AE790 *, Part150AE790 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, Style150AE790 *, s32, s32, s32);
s32 func_15167D84(void *, s32, s32, s32, s32, s32);

void func_150AE790(Arg150AE790 *arg0)
{
  Sub150AE790 *sub;
  Obj150AE790 *obj;
  Spawn150AE790 spawn;
  Part150AE790 part;
  s32 pad0;
  Style150AE790 style;
  f32 sinA;
  f32 cosA;
  f32 speed;
  f32 t;
  f32 sinB;
  f32 cosB;
  Packet150AE790 pkt;
  s32 count;
  u8 a;
  u8 b;
  s32 h;
  f32 f;
  f32 g;
  sub = &arg0->sub;
  if (arg0->sub.obj == 0)
  {
    return;
  }
  obj = sub->obj;
  if (sub->obj->unk0 == 0)
  {
    return;
  }
  h = obj->unk1D4;
  if ((h != 0) && (sub->obj->unk3B == sub->unk4))
  {
    func_15142314(sub->obj->unk1D4, sub->unk5, spawn.pos);
    spawn.unk10 = 1;
    spawn.unkE = 1;
    part.unk19 = 0;
    part.unk1A = 0xFF - (((u8) (sub->obj->unk184 >> 5)) << 6);
    part.unk14 = sub->obj->unk180;
    part.unk18 = 8;
    part.unk1B = 0xFF;
    count = (func_150ADA20() % 9U) + 0xC;
    style.unk0 = 0;
    style.unk4 = 1;
    style.unk8 = 0x160600;
    style.unkC = 3;
    style.unk10 = 0x10;
    style.unk14 = 0x80;
    style.unk18 = 0x20;
    style.unk1C = 0;
    style.unk1D = 9;
    for (; count != 0; count--)
    {
      a = func_150ADA20() & 0xFF;
      b = func_150ADA20() % 51U;
      sinA = func_151423D8(a);
      cosA = func_151423D8(a - 0x40);
      sinB = func_151423D8(b);
      cosB = func_151423D8(b - 0x40);
      speed = (func_150ADA68() * 0.5f) + 18.79999924f;
      spawn.unk15 = 4;
      spawn.unkC = (func_150ADA20() & 0xF) + 0x23;
      part.unk0 = (func_150ADA68() * 2.299999952f) + 2.900000095f;
      part.unk10 = (func_150ADA68() * 0.1000000015f) + 0.6999999881f;
      t = speed * sinB;
      part.unk4 = t * cosA;
      part.unk8 = (-speed) * cosB;
      part.unkC = t * sinA;
      func_15147DA0(&spawn, &part, 0, 1, 2, 0, 0, 0, 0, 0, 0, &style, 0, arg0->unkC, arg0->unk1);
    }

    f = func_150ADA68() + 2.5f;
    g = (func_150ADA68() * 200.0f) + 600.0f;
    pkt.unk1A = g;
    pkt.unk04 = g / f;
    pkt.unk26 = 0;
    pkt.unk24 = 0;
    pkt.unk2C = 0xFF;
    pkt.unk30 = 0;
    pkt.unk31 = 0;
    pkt.unk32 = 0;
    pkt.unk2D = 0xFF;
    pkt.unk2E = 0xFF;
    pkt.unk28 = 0x64;
    pkt.unk2F = 0xFF;
    pkt.unk2A = 0;
    pkt.unk0C = 0;
    pkt.unk0E = 0;
    pkt.unk1C = 0;
    pkt.unk1E = 0;
    pkt.unk1D = 0;
    pkt.unk16 = 0;
    pkt.unk18 = 0;
    pkt.unk22 = 0;
    pkt.unk20 = 0;
    pkt.unk00 = D_8008CB14;
    pkt.unk10 = spawn.pos[0];
    pkt.unk12 = spawn.pos[1];
    pkt.unk14 = spawn.pos[2];
    pkt.unk1F = 0xB;
    pkt.unk34 = 0;
    pkt.unk2B = 0;
    pkt.unk08 = 0;
    func_15167D84(&pkt, 0, 0, -1, arg0->unkC, arg0->unk1);
  }
}
// some funky xor going on
typedef struct {
    u8 unk0;
    u8 pad1;
    s16 unk2;
} Ent150AEB9C;
typedef struct {
    s32 count;
    Ent150AEB9C *ents;
} EntList150AEB9C;
typedef struct {
    u8 pad0[0x28];
    void *unk28;
    u8 unk2C;
    u8 unk2D;
} Spawn150AEB9C;
extern EntList150AEB9C D_800886E0[];
extern Ent150AEB9C *D_800886E4;
extern u32 D_15001B08[];
extern u8 D_800DCE50[];
void func_15001A08(void);

void func_150AEB9C(u8 *arg0) {
    u32 *p;
    u32 sum;
    s32 i;
    s32 idx;
    u32 end;

    idx = 0;
    if (arg0 != NULL) {
        sum = 0;
        end = (u32)D_15001B08;
        for (p = (u32 *)func_15001A08; (u32)p < end; p++) {
            sum ^= *p;
            sum <<= 1;
        }
        if (sum != 0xB4E42D60) {
            *(s32 *)(D_800DCE50 + 0x8C) = 0;
            *(s32 *)(D_800DCE50 + 0x22C) = 0;
        }
        for (i = 0; i < D_800886E0[idx].count; i++) {
            Spawn150AEB9C *obj;
            obj = (Spawn150AEB9C *)func_151491F4(D_800886E0[idx].ents[i].unk2, 0, -1, 1, 0x27, 8, 0xFF, 0);
            if (obj == NULL) {
                break;
            } else {
                EntList150AEB9C *list;
                list = &D_800886E0[idx];
                obj->unk28 = arg0;
                obj->unk2C = arg0[0x3B];
                obj->unk2D = list->ents[i].unk0;
            }
        }
    }
}

void func_150AECCC(struct42 *arg0) {
    arg0->unk96 = arg0->unk96 + (arg0->unk94 * D_800BE9E4);
    if (arg0->unk96 >= 0x1401) {
        arg0->unk96 = 0x1400;
    }
    arg0->unk9E = arg0->unk9E - ((s32) arg0->unk96 >> 8);
    arg0->unkA4 = arg0->unkA4 + D_800BE9E4;
    if (arg0->unkA4 >= 0x1A) {
        arg0->unkA4 = 0x19;
    }
}

void func_150AED4C(struct114 *arg0) {
    arg0->unk34 += arg0->unk14 * D_800BE9E4;
    if (arg0->unk2A < arg0->unk34) {
        arg0->unk3A = 70;
        arg0->unk34 = arg0->unk2A;
    }
    arg0->unk36 = arg0->unk34;
}

s32 func_150AED9C(void *arg0) {
    void *temp_v0 = *(void **)((s32)arg0 + 0x98);
    s32 temp_v1 = *(s16 *)((s32)arg0 + 0x1C) << 3;
    s32 temp_t7;

    if (temp_v1 >= 0x100) {
        temp_v1 = 0xFF;
    }
    temp_t7 = temp_v1 & 0xFF;
    *(u8 *)((s32)temp_v0 + 0x1B) = temp_v1;
    if (temp_t7 < 0) {
        return 0;
    }
    return 1;
}

s32 func_150AEDD8(struct202 *arg0) {
    if (arg0->unk1C < 0x20) {
        arg0->unk28 = arg0->unk1C * 8;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA_150AEDF8;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_150AEDF8;

void func_150AEDF8(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_150AEDF8 *temp_v0 = (SubA_150AEDF8 *)((u8 *)arg0 + 0x28);
    ArgB_150AEDF8 *b = (ArgB_150AEDF8 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                ((volatile SubA_150AEDF8 *)temp_v0)->unk4 = b->unk8;
            }
trailing_label_150AEDF8:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}
