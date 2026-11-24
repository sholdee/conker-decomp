#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s8 D_800D239A;
extern s8 D_800D2398;
extern s8 D_800D2399;
extern s8 *D_800D23B0;
s32 func_1508802C();

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
} B3020Struct;

extern s16 D_80087290;
extern s16 D_80087294;
extern B3020Struct *D_800D2350;
s16 *func_1502B5C8(s32 *, s32, s32, s32);
s32 func_15085BE8(void);
void func_15085B70(s32 arg0);

extern u8 D_800D2358;
extern f32 D_800D2360[];
extern u8 D_800D237C[];

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 unk6;
    u8 unk7;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
} B3020Struct2;
s32 func_15085BE8(void);

extern f32 D_800D2360[];
extern u8 D_800D237C[];
extern f32 D_8009D9CC;
extern u8 D_8008729C;
extern s32 D_800D2354;
extern f32 sqrtf(f32);
f32 func_15086D94(f32, f32, f32, f32, f32);
u8 func_15085DA8(f32 arg0);
s32 func_15085DF8(f32 arg0, f32 arg1, f32 arg2, s8 arg3, s8 arg4);

#pragma intrinsic(sqrtf)

f32 func_15086BD0(s32 arg0, s32 arg1) {
    f32 temp_fa0;
    f32 temp_fa1;
    f32 temp_fv1;
    s16 (*base)[8];
    u8 *temp_a2;
    u8 *temp_v1;

    if ((arg0 == 0xFF) || (arg1 == 0xFF)) {
        return 0.0f;
    }
    base = (s16 (*)[8])D_800D2350;
    temp_v1 = (u8 *)base[arg0];
    temp_a2 = (u8 *)base[arg1];
    temp_fv1 = (f32) (*(s16 *)((u8 *)temp_v1 + 0) - *(s16 *)((u8 *)temp_a2 + 0));
    temp_fa0 = (f32) (*(s16 *)((u8 *)temp_v1 + 2) - *(s16 *)((u8 *)temp_a2 + 2));
    temp_fa1 = (f32) (*(s16 *)((u8 *)temp_v1 + 4) - *(s16 *)((u8 *)temp_a2 + 4));
    return sqrtf((temp_fv1 * temp_fv1) + (temp_fa0 * temp_fa0) + (temp_fa1 * temp_fa1));
}

void func_15086C68(void) {
}

extern void func_150A3194(s32, s32, s32, s32, s32);

extern B3020Struct *D_800D2350;

void func_15086C70(s32 arg0) {
    s16 temp = D_800D2350[arg0].unk4;
    func_150A3194(3, 0xB, D_800D2350[arg0].unk0, D_800D2350[arg0].unk2, temp);
}

extern s16 D_80087290;

s32 func_15086CBC(s32 arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    if (arg0 < 0) {
        goto fail;
    }

    if (arg0 >= D_80087290) {
fail:
        return 0;
    }

    *arg1 = D_800D2350[arg0].unk0;
    *arg2 = D_800D2350[arg0].unk2;
    *arg3 = D_800D2350[arg0].unk4;
    return 1;
}

s32 func_15086D48(s32 arg0) {
    s32 i;

    for (i = 0; i < D_80087290; i++) {
        if (arg0 == ((u8 *)D_800D2350)[i * 16 + 7]) {
            return i;
        }
    }
    return 0xFF;
}

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 links[5];
    u8 unkE;
    u8 unkF;
} Node15086D94;

f32 func_15086D94(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    s32 p0;
    s32 p1;
    s32 p2;
    s32 p3;
    u8 id;
    s32 i;
    s32 j;
    Node15086D94 *n1;
    Node15086D94 *n2;
    f32 a;
    f32 b;
    f32 c;
    f32 d1;
    f32 d2;
    f32 t;
    f32 best;
    u8 link;

    id = func_15085DA8(arg1);
    best = 100.0f;
    for (i = 0; i < D_80087290; i++) {
        n1 = (Node15086D94 *)&D_800D2350[i];
        if (n1->unkE == 1 && n1->unk6 == id) {
            for (j = 0; j < 5; j++) {
                link = n1->links[j];
                if (link != 0xFF && i < link) {
                    n2 = (Node15086D94 *)&D_800D2350[link];
                    if (n2->unkE == 1) {
                        a = n2->z - n1->z;
                        b = -(f32)(n2->x - n1->x);
                        c = -((f32)n1->x * a + b * (f32)n1->z);
                        d1 = arg0 * a + arg2 * b + c;
                        d2 = (arg0 + arg3) * a + (arg2 + arg4) * b + c;
                        if ((d2 < 0.0f && d1 >= 0.0f) || (d1 < 0.0f && d2 >= 0.0f)) {
                            if (d2 < 0.0f) {
                                d2 = -d2;
                            }
                            if (d1 < 0.0f) {
                                d1 = -d1;
                            }
                            t = d1 / (d1 + d2);
                            c = a;
                            a = -b;
                            b = c;
                            c = -((f32)n1->x * a + b * (f32)n1->z);
                            d1 = ((t * arg3) + arg0) * a + ((t * arg4) + arg2) * b + c;
                            d2 = (f32)n2->x * a + b * (f32)n2->z + c;
                            if (((d2 > 0.0f && d1 > 0.0f && d1 <= d2) || (d2 < 0.0f && d1 < 0.0f && d2 <= d1)) && t < best) {
                                best = t;
                            }
                        }
                    }
                }
            }
        }
    }
    if (best <= 1.0f) {
        return sqrtf(arg3 * arg3 + arg4 * arg4) * t;
    }
    return -1.0f;
}

extern u8 *D_800872A0;
extern s8 D_800D23A8;
extern s32 D_800D2394;
extern void *allocate_memory(s32, s32, s32, s32);
u8 func_150888A8(u8, u8, s32);
void func_15088824();

typedef struct {
    u8 pad0[9];
    u8 unk9[5];
    u8 unkE;
    u8 unkF;
} B3020Bytes;

void func_150870D0(s32 arg0, s32 arg1, s32 arg2)
{
  u8 *p;
  s32 count;
  s32 i;
  B3020Bytes *e;
  u8 *q;
  u8 start;
  u8 best;
  u8 fwd;
  u8 back;
  u8 sel;
  u8 path[3];
  B3020Struct *new_var;
  u8 idx;
  D_800872A0 = 0;
  start = func_15086D48(arg0);
  if (start == 0xFF)
  {
    return;
  }
  new_var = &D_800D2350[start];
  best = D_800D23A8 * 0xFF;
  sel = 0xFF;
  q = (u8 *) new_var;
  for (i = 0; i < 5; i++, q++)
  {
    idx = q[9];
    if (idx != 0xFF)
    {
      e = (B3020Bytes *) (&D_800D2350[idx]);
      if (e->unkE == 0)
      {
        if (((D_800D23A8 == 0) && (e->unkF >= best)) || ((D_800D23A8 != 0) && (e->unkF <= best)))
        {
          best = e->unkF;
          sel = idx;
        }
      }
    }
  }

  if (sel != 0xFF)
  {
    fwd = func_150888A8(start, sel, 0);
    back = func_150888A8(sel, start, 0);
  }
  if (((sel == 0xFF) || (back == 0xFF)) || (fwd == 0xFF))
  {
    return;
  }
  path[1] = sel;
  path[2] = fwd;
  count = 0;
  do
  {
    path[0] = path[1];
    path[1] = path[2];
    path[2] = func_150888A8(path[0], path[1], 0);
    count++;
  }
  while (start != path[0]);
  D_800D2399 = arg2;
  D_800D2398 = arg1;
  arg1 += arg2;
  D_800D2394 = 0;
  D_800872A0 = allocate_memory(arg1 * 0x84, 1, 0, 0);
  for (i = 0; i < arg1; i++)
  {
    p = &D_800872A0[i * 0x84];
    func_15088824(p);
    p[0x2B] = back;
    p[0x2C] = start;
    p[0x2D] = sel;
    p[0x2E] = fwd;
    if (i < D_800D2398)
    {
      p[0x31] = i;
    }
    p[0x30] = i >= D_8008FD90;
    p[0x26] = count;
  }

  D_800D239A = 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_B4080/func_15087350.s")

extern u8 *D_800872A0;
void func_150891E8(s8 *, struct127 *);
void func_150896EC();

void func_15087CC0(void) {
    s8 *p;
    struct127 *obj;
    s32 i;

    D_800D23A9 = 0;
    if (D_800872A0 == 0) {
        return;
    }
    for (i = 0; i < D_800D2398; i++) {
        p = (s8 *)(i * 0x84 + (s32)D_800872A0);
        obj = &D_800CC2D0[p[0x31]];
        if (obj->camera != NULL) {
            if (p[0x30] >= 2) {
                func_150891E8(p, obj);
            } else if (((u8 *)p)[0x48] == 0) {
                func_150896EC(p, obj, 0);
            } else if (p[0x49] != 0) {
                func_150896EC(p, obj, 1);
            }
        }
    }
}

extern u8 *D_800872A0;
u8 func_150888A8(u8, u8, s32);

struct Struct15087DCC {
    char pad0[0x2B];
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    s8 unk2F;
    char pad30[0x84 - 0x30];
};

void func_15087DCC(s32 arg0, s32 arg1) {
    struct Struct15087DCC *p;
    if (D_800872A0 == 0) {
        return;
    }
    p = (struct Struct15087DCC *)(arg0 * sizeof(struct Struct15087DCC) + (s32)D_800872A0);
    if (arg1 == p->unk2F) {
        return;
    }
    if (arg1 != 0) {
        p->unk2D = func_150888A8(p->unk2B, p->unk2C, 1);
        p->unk2E = func_150888A8(p->unk2C, p->unk2D, 1);
    }
    p->unk2F = arg1;
}

struct Struct15087E54 {
    char pad0[0xC];
    f32 unkC;
    f32 unk10;
    char pad14[0x84 - 0x14];
};

struct Struct15087E54arg1 {
    char pad0[0x76];
    u16 unk76;
};

void func_15087E54(s32 arg0, struct Struct15087E54arg1 *arg1) {
    struct Struct15087E54 *p;
    u16 d;
    u16 r;
    if (D_800872A0 == 0) {
        return;
    }
    p = (struct Struct15087E54 *)(arg0 * sizeof(struct Struct15087E54) + (s32)D_800872A0);
    r = func_1505A630(p->unk10, p->unkC, 0) + 0x4000;
    d = (u16)(r - arg1->unk76);
    if (d & 0x8000) {
        if (d < 0xDBFF) {
            arg1->unk76 = r + 0x2400;
        }
    } else {
        if (d >= 0x2401) {
            arg1->unk76 = r - 0x2400;
        }
    }
}

extern s8 D_800D23A8;
extern u8 *D_800872A0;

struct Struct15087EF0 {
    f32 unk0;
    char pad4[0xC - 4];
    f32 unkC;
    f32 unk10;
    char pad14[0x84 - 0x14];
};

struct Struct15087EF0arg1 {
    char pad0[0x76];
    s16 unk76;
};

void func_15087EF0(s32 arg0, struct Struct15087EF0arg1 *arg1) {
    struct Struct15087EF0 *p;
    u16 r;
    if (D_800872A0 == 0) {
        return;
    }
    p = (struct Struct15087EF0 *)(arg0 * sizeof(struct Struct15087EF0) + (s32)D_800872A0);
    r = func_1505A630(p->unk10, p->unkC, 0);
    if (D_800D23A8 == 0) {
        if (p->unk0 < 0.5f) {
            r = (u16)(r + 0x4A00);
        } else {
            r = (u16)(r + 0x3600);
        }
    } else {
        if (0.5f <= p->unk0) {
            r = (u16)(r + 0x4A00);
        } else {
            r = (u16)(r + 0x3600);
        }
    }
    arg1->unk76 = r;
}

struct Struct15087FC4 {
    char pad0[0x31];
    u8 unk31;
    char pad32[0x84 - 0x32];
};

void func_15087FC4(s32 arg0, s32 arg1) {
    struct Struct15087FC4 *p;
    if (D_800872A0 == 0) {
        return;
    }
    p = (struct Struct15087FC4 *)(arg0 * sizeof(struct Struct15087FC4) + (s32)D_800872A0);
    p->unk31 = arg1;
}

extern u8 *D_800872A0;

struct Struct15087FEC {
    char pad0[4];
    f32 unk4;
    char pad8[0x84 - 8];
};

void func_15087FEC(s32 arg0, s32 arg1) {
    struct Struct15087FEC *p;
    if (D_800872A0 == 0) {
        return;
    }
    p = (struct Struct15087FEC *)(arg0 * sizeof(struct Struct15087FEC) + (s32)D_800872A0);
    p->unk4 = (f32)arg1 * (1.0f / 256.0f);
}

s32 func_1508802C(s8 *arg0, struct127 *arg1, s32 arg2) {
    struct126 *temp_v0;

    temp_v0 = arg1->unk31C;
    if (temp_v0->unk120 != 0) {
        return 1;
    }

    if ((D_800D18A0 & (1 << (arg1 - D_800CC2D0))) != 0) {
        return 1;
    }

    temp_v0->unk84 = 1;
    arg1->interaction_state = 0xC;
    arg1->unk232 = arg2;
    D_800D154C = arg1;
    D_800C3E78 = arg0[0x31];
    arg1->unk218 = func_1507BB28(0, arg2);
    arg1->unk21C = 0;
    arg1->immune = 0xFF;
    arg0[0x30] = 2;
    return 0;
}

s32 func_150880F8(s32 arg0, s32 arg1) {
    s8 *p;
    struct127 *obj;
    s32 ret;

    ret = 0;
    if (D_800872A0 == 0) {
        return 0;
    }

    p = (s8 *)(arg0 * 0x84 + (s32)D_800872A0);
    obj = &D_800CC2D0[p[0x31]];
    if (p[0x30] == 0) {
        ret = func_1508802C(p, obj, arg1);
    } else if (p[0x30] == 1) {
        p[0x30] = 2;
        obj->immune = 0xFF;
    }

    p[0x2A] = D_800D239A;
    D_800D239A++;
    return ret;
}

struct Struct150881CC {
    f32 unk0;
    char pad4[0x84 - 4];
};

s32 func_150881CC(s32 arg0) {
    struct Struct150881CC *p;
    if (D_800872A0 == 0) {
        return 0;
    }
    p = (struct Struct150881CC *)(arg0 * sizeof(struct Struct150881CC) + (s32)D_800872A0);
    return (s32)(p->unk0 * 256.0f);
}

struct Struct15088218 {
    char pad0[8];
    f32 unk8;
    char padC[0x24 - 0xC];
    s16 unk24;
    char pad26[0x84 - 0x26];
};

s32 func_15088218(s32 arg0) {
    struct Struct15088218 *p;
    s32 v;
    if (D_800872A0 == 0) {
        return 0;
    }
    p = (struct Struct15088218 *)(arg0 * sizeof(struct Struct15088218) + (s32)D_800872A0);
    v = p->unk24;
    return (v << 4) + (s32)(p->unk8 * 16.0f);
}

struct Struct15088270 {
    char pad0[0x14];
    f32 unk14;
    char pad18[0x84 - 0x18];
};

s32 func_15088270(s32 arg0) {
    struct Struct15088270 *p;
    if (D_800872A0 == 0) {
        return 0;
    }
    p = (struct Struct15088270 *)(arg0 * sizeof(struct Struct15088270) + (s32)D_800872A0);
    return (s32)p->unk14;
}

struct Struct150882B0 {
    char pad0[0x27];
    s8 unk27;
    char pad28[0x84 - 0x28];
};

s32 func_150882B0(s32 arg0) {
    struct Struct150882B0 *p;
    if (D_800872A0 == 0) {
        return 0;
    }
    p = (struct Struct150882B0 *)(arg0 * sizeof(struct Struct150882B0) + (s32)D_800872A0);
    return p->unk27;
}

struct Struct150882E4 {
    char pad0[8];
    f32 unk8;
    char padC[0x24 - 0xC];
    s16 unk24;
    char pad26[3];
    s8 unk29;
    char pad2A[0x84 - 0x2A];
};

s32 func_150882E4(s32 arg0, s32 arg1) {
    struct Struct150882E4 *p;
    struct Struct150882E4 *iter;
    s32 i;
    s32 wanted;

    if (D_800872A0 == 0) {
        return 0x10;
    }

    p = (struct Struct150882E4 *)(arg0 * sizeof(struct Struct150882E4) + (s32)D_800872A0);
    wanted = p->unk29 + arg1;
    i = 0;
    if (D_800D2398 > 0) {
        iter = (struct Struct150882E4 *)D_800872A0;
        do {
            if (wanted == iter->unk29) {
                if (i != arg0) {
                    arg0 = (p->unk24 << 4) + (s32)(p->unk8 * 16.0f);
                    arg0 = arg0 - (iter->unk24 << 4) - (s32)(iter->unk8 * 16.0f);
                    if (arg0 < 0) {
                        arg0 = -arg0;
                    }
                    return (arg0 << 8) | i;
                }
            }
            i++;
            iter++;
        } while (i < D_800D2398);
    }

    return 0x10;
}

extern f32 D_8009DA00;

struct Struct150883B0 {
    f32 unk0;
    char pad4[0xC - 4];
    f32 unkC;
    f32 unk10;
    char pad14[0x2F - 0x14];
    s8 unk2F;
    char pad30[1];
    s8 unk31;
    char pad32[0x84 - 0x32];
};

struct127 *func_150883B0(s32 arg0, s32 arg1, f32 *arg2, s32 *arg3) {
    struct Struct150883B0 *p;
    struct127 *ret;
    f32 sign;
    f32 dx;
    f32 dz;
    f32 x;
    f32 z;
    f32 base;
    f32 best;
    f32 d;
    s32 i;
    s8 self;

    if (D_800872A0 == 0) {
        return NULL;
    }
    p = (struct Struct150883B0 *)(arg0 * sizeof(struct Struct150883B0) + (s32)D_800872A0);
    if (arg1 >= 0) {
        sign = -1.0f;
    } else {
        sign = 1.0f;
    }
    arg1 = p->unk2F;
    x = D_800CC2D0[p->unk31].x_position;
    z = D_800CC2D0[p->unk31].z_position;
    dx = p->unkC * sign;
    dz = p->unk10 * sign;
    self = D_800CC2D0[p->unk31].unk124 - 1;
    base = -((dx * x) + (dz * z));
    ret = NULL;
    best = D_8009DA00;
    for (i = 0; i < D_800D2398; i++) {
        p = (struct Struct150883B0 *)(i * sizeof(struct Struct150883B0) + (s32)D_800872A0);
        if (self != p->unk31 && (arg1 == 0 || arg1 == p->unk2F)) {
            x = D_800CC2D0[p->unk31].x_position;
            z = D_800CC2D0[p->unk31].z_position;
            d = (dx * x) + (dz * z) + base;
            if (d > 0.0f && d < best) {
                best = d;
                *arg2 = p->unk0;
                *arg3 = p->unk2F;
                ret = &D_800CC2D0[p->unk31];
            }
        }
    }
    return ret;
}

struct Struct1508855C {
    char pad0[0x31];
    s8 unk31;
    char pad32[0x84 - 0x32];
};

s32 func_1508855C(struct127 *arg0) {
    u8 *base;
    s32 index;
    s32 i;
    s32 limit;
    struct Struct1508855C *p;

    index = ((s32)arg0 - (s32)D_800CC2D0) / (s32)sizeof(struct127);
    base = D_800872A0;
    if (base == 0) {
        return -1;
    }

    if (index == 0) {
        return 0;
    }

    i = 1;
    limit = D_800D2398 + D_800D2399;
    p = (struct Struct1508855C *)(base + sizeof(struct Struct1508855C));
    if (limit < 2) {
        goto fail;
    }

    do {
        if (index == p->unk31) {
            return i;
        }
        i++;
        p++;
    } while (i < limit);

fail:
    return -1;
}

void bcopy(const void *, void *, int);
#define D_800872A0_S32 ((s32)D_800872A0)

void func_150885EC(s32 arg0, s32 arg1) {
    s32 sp24;
    s32 sp20;

    if (D_800872A0_S32 != 0) {
        sp24 = ((s8 *)D_800872A0_S32 + arg1 * 0x84)[0x31];
        sp20 = ((s8 *)D_800872A0_S32 + arg1 * 0x84)[0x30];
        bcopy((void *)((arg0 * 0x84) + D_800872A0_S32),
              (void *)((arg1 * 0x84) + D_800872A0_S32), 0x84);
        *(s8 *)(D_800872A0_S32 + arg1 * 0x84 + 0x31) = sp24;
        *(s8 *)(D_800872A0_S32 + arg1 * 0x84 + 0x30) = sp20;
    }
}
#undef D_800872A0_S32

extern s32 D_800D2394;
extern s8 D_800D2398;
extern s8 D_800D2399;
typedef struct Struct15088824 Struct15088824;
void func_15088824();

s32 func_1508868C(s32 arg0) {
    s32 i;
    s32 offset;
    s32 base;

    base = (s32)D_800872A0;
    if (base == 0) {
        return -1;
    }

    offset = 1;
    i = 0;
    if (D_800D2399 > 0) {
        do {
            if ((D_800D2394 & offset) == 0) {
                D_800D2394 |= offset;
                i += D_800D2398;
                offset = i * 0x84;
                func_15088824(offset + base);
                D_800872A0[offset + 0x30] = 1;
                D_800872A0[offset + 0x31] = (arg0 - (s32)D_800CC2D0) / (s32)sizeof(struct127);
                return i;
            }
            i++;
            offset <<= 1;
        } while (i < D_800D2399);
    }
    return -1;
}

void func_15088780(void) {
    s32 func_1508855C();
    s32 temp_v0;

    if (D_800872A0 != 0) {
        temp_v0 = func_1508855C();
        D_800872A0[temp_v0 * 0x84 + 0x31] = 0;
        D_800D2394 &= ~(1 << (temp_v0 - D_800D2398));
    }
}

extern u8 *D_800872A0;

s32 func_150887F8(void) {
    u8 *p = D_800872A0;
    if (p == 0) {
        return 0;
    }
    return p[0x46] == 0xFF;
}

struct Struct15088824 {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u8 unk26;
    /* 0x27 */ u8 unk27;
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 pad34[0x15];
    /* 0x49 */ u8 unk49;
};

void func_15088824(Struct15088824 *arg0) {
    arg0->unk2B = 0;
    arg0->unk2C = 0;
    arg0->unk2D = 0;
    arg0->unk2E = 0;
    arg0->unk28 = 0;
    arg0->unk1C = 0;
    arg0->unk18 = 0;
    arg0->unk20 = -1;
    arg0->unk2F = 0;
    arg0->unk8 = 0.0f;
    arg0->unk0 = 0.5f;
    arg0->unk4 = 0.5f;
    arg0->unk24 = 0;
    arg0->unk26 = 0;
    arg0->unk27 = 0;
    arg0->unk31 = 0;
    arg0->unkC = 0.0f;
    arg0->unk14 = 0.0f;
    arg0->unk33 = 2;
    arg0->unk30 = 0;
    arg0->unk2A = 0x7F;
    arg0->unk49 = 0;
    arg0->unk10 = 1.0f;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_B4080/func_150888A8.s")

struct Struct15088A08 {
    char pad0[0x8];
    f32 unk8;
    char padC[0xC];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s16 unk24;
    s8 unk26;
    s8 unk27;
    s8 unk28;
    char pad29[2];
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    s8 unk2F;
    s8 unk30;
    s8 unk31;
};

extern u8 D_800D2390;
extern s8 D_800E0BE7;
extern u8 D_800BE616;
extern s8 D_8008FD90;
extern s16 D_800E0C10[];
extern s32 D_800E0C18[];

s32 func_15088A08(struct Struct15088A08 *arg0, f32 arg1) {
    s32 dir;
    s32 idx;
    s32 lim;
    s32 t;
    s32 v;

    dir = 0;
    arg0->unk8 += arg1;
    if (arg0->unk8 < 0.0f) {
        arg0->unk8 += 1.0f;
        arg0->unk2E = arg0->unk2D;
        arg0->unk2D = arg0->unk2C;
        arg0->unk2C = arg0->unk2B;
        arg0->unk2B = func_150888A8(arg0->unk2D, arg0->unk2C, arg0->unk2F);
        dir = -1;
    } else if (arg0->unk8 >= 1.0f) {
        arg0->unk8 -= 1.0f;
        arg0->unk2B = arg0->unk2C;
        arg0->unk2C = arg0->unk2D;
        arg0->unk2D = arg0->unk2E;
        arg0->unk2E = func_150888A8(arg0->unk2C, arg0->unk2D, arg0->unk2F);
        dir = 1;
        if (D_800D23A8 == 0) {
            if (((u8 *)D_800D2350)[arg0->unk2C * 16 + 0xF] == 5) {
                D_800D2390 = 1;
            }
            if (((u8 *)D_800D2350)[arg0->unk2C * 16 + 0xF] == 6) {
                D_800D2390 = 2;
            }
        } else {
            if (((u8 *)D_800D2350)[arg0->unk2C * 16 + 0xF] == 7) {
                D_800D2390 = 1;
            }
            if (((u8 *)D_800D2350)[arg0->unk2C * 16 + 0xF] == 8) {
                D_800D2390 = 2;
            }
            if (((u8 *)D_800D2350)[arg0->unk2C * 16 + 0xF] == 9) {
                D_800D2390 = 3;
            }
        }
    }

    if (dir != 0) {
        lim = D_800E0BE7;
        idx = arg0->unk31;
        arg0->unk24 += dir;
        arg0->unk27 = arg0->unk24 / arg0->unk26;
        if (D_800BE616 != 0) {
            if (lim < arg0->unk27) {
                arg0->unk27 = lim + 1;
            }
        }
        if (arg0->unk28 < arg0->unk27) {
            arg0->unk28 = arg0->unk27;
            if ((idx < D_8008FD90) && (arg0->unk30 == 0) && (arg0->unk1C > 0)) {
                if (lim >= arg0->unk28) {
                    t = arg0->unk1C - arg0->unk18;
                    if (t > 0x7D00) {
                        t = 0x7D00;
                    }
                    if ((D_800E0C10[idx] <= 0) || (t < D_800E0C10[idx])) {
                        D_800E0C10[idx] = t;
                    }
                    arg0->unk20 = t;
                }
                if (lim == arg0->unk28) {
                    v = arg0->unk1C;
                    if (v > 0x57E40) {
                        v = 0x57E40;
                    }
                    D_800E0C18[idx] = v;
                }
            }
            if (lim == arg0->unk28) {
                arg0->unk18 = -arg0->unk1C;
            } else if (arg0->unk28 < lim) {
                arg0->unk18 = arg0->unk1C;
            }
        }
    }

    return dir;
}

typedef struct Entry15088D58 {
    f32 value0;
    f32 value4;
    f32 phase8;
    f32 directionC;
    f32 direction10;
    u8 pad14[0x18];
    u8 node2C;
    u8 pad2D[2];
    s8 group2F;
    u8 pad30;
    s8 actor31;
    u8 pad32[0x52];
} Entry15088D58;

typedef struct Actor15088D58 {
    u8 pad0[0x14];
    f32 field14;
    f32 field18;
    f32 field1C;
    u8 pad20[0x20];
    f32 angle40;
    u8 pad44[0x32];
    s16 rotation76;
    s16 rotation78;
    s16 rotation7A;
    u8 pad7C[0xA8];
    u8 field124;
} Actor15088D58;

void func_15088F30();
f32 func_150497E0(f32 *, s32, f32);
f32 func_150498A4(f32 *, s32, f32, f32 *);
f32 func_15144BC8(f32);
extern f32 D_8009DA04;

void func_15088D58(Actor15088D58 *arg0) {
    Entry15088D58 *entry;
    f32 angle;
    f32 x_derivative;
    f32 z_derivative;
    f32 xs[4];
    f32 ys[4];
    f32 zs[4];
    s32 rotation;
    s32 index;
    s32 i;
    s32 link;
    s32 again;
    u8 *target;
    u8 *cursor;
    int tmp1;
    tmp1 = 0xFF;

    index = func_1508855C((struct127 *)arg0);
    if (index >= 0) {
        entry = (Entry15088D58 *)(D_800872A0 + index * 0x84);
        entry->value0 = 0.5f;
        entry->value4 = 0.5f;
        entry->phase8 = 0.5f;
        do {
            again = 0;
            i = 0;
            cursor = entry->node2C * 0x10 + (u8 *)D_800D2350;
            do {
                link = cursor[9];
                i++;
                if (tmp1 != link) {
                    target = link * 0x10 + (u8 *)D_800D2350;
                    if (target[0xE] == 4) {
                        again = target[0xF];
                    }
                }
                cursor++;
            } while (i != 5);
            if (again != 0) {
                func_15088A08((struct Struct15088A08 *)entry, 1.0f);
            }
        } while (again != 0);
        entry->phase8 = 0.0f;
        func_15088F30((f32 *)entry, xs, ys, zs);
        arg0->field14 = func_150498A4(xs, 0, entry->phase8, &x_derivative);
        arg0->field18 = func_150497E0(ys, 0, entry->phase8);
        arg0->field1C = func_150498A4(zs, 0, entry->phase8, &z_derivative);
        angle = func_15144BC8((f32)(func_1505A630(entry->direction10, entry->directionC, 0) + 0x8000) * 0.005493164f);
        arg0->angle40 = angle;
        rotation = (s32)(short)(s32)(angle * D_8009DA04) - 0x4000;
        arg0->rotation7A = rotation;
        arg0->rotation78 = rotation;
        arg0->rotation76 = rotation;
    }
}

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[3];
    u8 links[5];
    u8 type;
    u8 padF;
} Node15088F30;

typedef struct {
    f32 t;
    u8 pad4[0x27];
    u8 ids[4];
} Obj15088F30;

void func_15088F30(Obj15088F30 *arg0, f32 *outX, f32 *outY, f32 *outZ) {
    s32 i;
    s32 j;
    u8 id;
    Node15088F30 *node;
    Node15088F30 *next;
    f32 dx;
    f32 dz;

    for (i = 0; i < 4; i++) {
        if (arg0->ids[i] != 0xFF) {
            node = &((Node15088F30 *)D_800D2350)[arg0->ids[i]];
            next = node;
            for (j = 0; j < 5; j++) {
                id = node->links[j];
                if ((id != 0xFF) && (((Node15088F30 *)D_800D2350)[id].type == 4)) {
                    next = id + (Node15088F30 *)D_800D2350;
                    j = 5;
                }
            }
            dx = next->x - node->x;
            dz = next->z - node->z;
            outX[i] = node->x + (dx * arg0->t);
            outY[i] = node->y;
            outZ[i] = node->z + (dz * arg0->t);
        } else {
            outX[i] = i * 64;
            outY[i] = 0.0f;
            outZ[i] = 0.0f;
        }
    }
}

u8 func_1508907C(u8 node, u8 type, u8 exclude, u8 sub) {
    s32 i;
    u8 result;
    Node15088F30 *n;
    Node15088F30 *t;

    result = 0xFF;
    if (node >= D_80087290) {
        return 0xFF;
    }
    n = node + (Node15088F30 *)D_800D2350;
    for (i = 0; i < 5; i++) {
        if (n->links[i] != 0xFF && n->links[i] != exclude) {
            t = n->links[i] + (Node15088F30 *)D_800D2350;
            if (t->type == type || type == 0xFF) {
                if (t->padF == sub) {
                    return n->links[i];
                }
                result = n->links[i];
            }
        }
    }
    return result;
}


typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    f32 unkC;
    s32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
} CamParams150896EC;

typedef struct {
    u8 pad0[0x34];
    f32 t;
    f32 x;
    f32 y;
    f32 z;
    u8 nodes[4];
    u8 pad48;
    s8 state;
    u8 pad4A[2];
    CamParams150896EC cam;
} Obj150896EC;

#define obj ((Obj150891E8 *)arg0)
typedef struct {
    u8 pad0[0x30];
    s8 state;
    u8 pad31[0x19];
    u8 cur;
    u8 next;
    CamParams150896EC cam;
} Obj150891E8;

void func_1512D560();

void func_150891E8(s8 *arg0, struct127 *arg1) {
    f32 dx;
    f32 dy;
    f32 dz;
    s32 a2;
    s32 b2;
    s32 i;
    f32 min2;
    u32 j;
    u8 link;
    f32 min;
    Node15088F30 *entry;
    Node15088F30 *other;
    u8 a;
    u8 b;
    u8 start;
    u8 sub;
    CamParams150896EC *p;
    struct108 *cam;

    if (arg1->camera == NULL) {
        return;
    }
    if (D_8008FDBC & 0x40) {
        sub = 1;
    } else {
        sub = 0;
    }
    cam = arg1->camera;
    if (obj->state == 2) {
        for (i = 0; i < D_80087290; i++) {
            entry = (Node15088F30 *)&D_800D2350[i];
            if (entry->type == 5 && entry->padF == 2) {
                obj->cur = i;
                obj->next = i;
                for (j = 0; j < 5; j++) {
                    if (entry->links[j] != 0xFF) {
                        other = (Node15088F30 *)&D_800D2350[entry->links[j]];
                        if ((!D_800D23A8 && other->padF == 3) || (D_800D23A8 && other->padF != 3)) {
                            obj->next = entry->links[j];
                        }
                    }
                }
                i = D_80087290;
            }
        }
        b = obj->next;
        min = 1000000000.0f;
        a = obj->cur;
        start = b;
        do {
            entry = (Node15088F30 *)&D_800D2350[b];
            a2 = func_1508907C(b, 0xFF, a, sub);
            a = b;
            b = a2;
            dx = arg1->x_position - entry->x;
            dz = arg1->z_position - entry->z;
            min2 = dx * dx + dz * dz;
            if (min2 < min) {
                min = min2;
                obj->cur = a;
                obj->next = a2;
            }
        } while (b != start);
        func_1512D560(arg1->camera, 5, 0);
        obj->state = 3;
    }
    p = &obj->cam;
    b2 = obj->next;
    a2 = obj->cur;
    entry = (Node15088F30 *)&D_800D2350[a2];
    other = (Node15088F30 *)&D_800D2350[b2];
    dx = arg1->x_position - other->x;
    dz = arg1->z_position - other->z;
    min2 = dx * dx + dz * dz;
    dx = arg1->x_position - entry->x;
    dz = arg1->z_position - entry->z;
    min = dx * dx + dz * dz;
    if (min2 < min) {
        obj->cur = b2;
        obj->next = func_1508907C(obj->cur, 0xFF, a2, sub);
        entry = other;
        dx = arg1->x_position - entry->x;
        dz = arg1->z_position - entry->z;
        min = min2;
    }
    dy = arg1->y_position - entry->y;
    min = sqrtf(dy * dy + min);
    dx /= min;
    dy /= min;
    dz /= min;
    if (min > 1000.0f) {
        min = 1000.0f;
    }
    p->unk0 = 0;
    p->unk20 = arg1->x_position;
    p->unk24 = arg1->y_position;
    p->unk28 = arg1->z_position;
    p->unk14 = arg1->x_position - dx * min;
    p->unk18 = arg1->y_position - dy * min;
    p->unk1C = arg1->z_position - dz * min;
    p->unk4 = 0;
    p->unk30 = 30.0f;
    p->unk34 = 30.0f;
    p->unk2C = 0.0f;
    func_1512D560(arg1->camera, 7, p);
    cam->unk19C = 30.0f;
    cam->unk1A4 = 30.0f;
    cam->unk1A0 = 30.0f;
    cam->unk1A8 = 30.0f;
}
#undef obj


void func_1512D560();
extern s32 D_800BE9E4;

void func_150896EC(Obj150896EC *arg0, struct127 *arg1, s32 arg2) {
    struct108 *cam;
    s32 i;
    s32 active;
    f32 angle;
    u8 next;
    B3020Struct *entry;
    CamParams150896EC *p;
    f32 xs[4];
    f32 ys[4];
    f32 zs[4];
    f32 lxs[4];
    f32 lys[4];
    f32 lzs[4];

    if (arg1->camera == NULL) {
        return;
    }
    cam = arg1->camera;
    p = &arg0->cam;
    if (arg2 != 0) {
        p->unk10 = 0x78;
        p->unkC = 999.0f;
        func_1512D560(arg1->camera, 0xA, p);
        func_1512D560(arg1->camera, 6, cam->unk23D);
        arg0->state = 0;
        return;
    }
    if (arg0->state == 0) {
        next = D_800D23A8 + 1;
        i = 0;
        if (D_80087290 > 0) {
            do {
                entry = &D_800D2350[i];
                if (((u8 *)entry)[0xE] == 6 && next == ((u8 *)entry)[0xF]) {
                    arg0->nodes[0] = i;
                    i = D_80087290;
                }
                i++;
            } while (i < D_80087290);
        }
        arg0->nodes[1] = func_1508907C(arg0->nodes[0], 6, 0xFF, 0);
        arg0->nodes[2] = func_1508907C(arg0->nodes[1], 6, arg0->nodes[0], 0);
        arg0->nodes[3] = func_1508907C(arg0->nodes[2], 6, arg0->nodes[1], 0);
        arg0->x = arg1->x_position;
        arg0->y = arg1->y_position;
        arg0->z = arg1->z_position;
        arg0->t = 0.0f;
        func_1512D560(arg1->camera, 5, cam->unk23D);
        arg0->state = 1;
    }
    arg0->t += 0.01700000092f * D_800BE9E4;
    if (arg0->t >= 1.0f) {
        if (arg0->nodes[2] == 0xFF) {
            arg0->t = 1.0f;
        } else {
            arg0->t -= 1.0f;
            arg0->nodes[0] = arg0->nodes[1];
            arg0->nodes[1] = arg0->nodes[2];
            arg0->nodes[2] = arg0->nodes[3];
            arg0->nodes[3] = func_1508907C(arg0->nodes[2], 6, arg0->nodes[1], 0);
        }
    }
    active = 0;
    for (i = 0; i < 4; i++) {
        if (arg0->nodes[i] != 0xFF) {
            func_15086CBC(arg0->nodes[i], &xs[i], &ys[i], &zs[i]);
            active = 1;
            func_15086CBC(func_1508907C(arg0->nodes[i], 4, 0xFF, 0), &lxs[i], &lys[i], &lzs[i]);
        } else if (active == 1) {
            active = 0;
            angle = arg1->unk40 * 0.0174500000000000002;
            xs[i] = arg0->x - sinf(angle) * 500.0f;
            ys[i] = arg0->y + 180.0f;
            zs[i] = arg0->z - cosf(angle) * 500.0f;
            lxs[i] = arg0->x;
            lys[i] = arg0->y + 120.0f;
            lzs[i] = arg0->z;
        } else {
            xs[i] = xs[i - 1] + (xs[i - 1] - xs[i - 2]);
            ys[i] = ys[i - 1] + (ys[i - 1] - ys[i - 2]);
            zs[i] = zs[i - 1] + (zs[i - 1] - zs[i - 2]);
            lxs[i] = lxs[i - 1] + (lxs[i - 1] - lxs[i - 2]);
            lys[i] = lys[i - 1] + (lys[i - 1] - lys[i - 2]);
            lzs[i] = lzs[i - 1] + (lzs[i - 1] - lzs[i - 2]);
        }
    }
    angle = arg0->t;
    p = &arg0->cam;
    p->unk14 = func_150497E0(xs, 0, angle);
    p->unk18 = func_150497E0(ys, 0, angle);
    p->unk1C = func_150497E0(zs, 0, angle);
    p->unk20 = func_150497E0(lxs, 0, angle);
    p->unk24 = func_150497E0(lys, 0, angle);
    p->unk28 = func_150497E0(lzs, 0, angle);
    p->unk0 = 0;
    p->unk4 = 0;
    p->unk30 = 30.0f;
    p->unk34 = 30.0f;
    p->unk2C = 0.0f;
    func_1512D560(arg1->camera, 7, p);
}

void func_15089BB0(void) {
    D_800D23B0 = 0;
}

void *func_1509B570(s32);
void func_1508BC20(void);
extern f32 D_800872B0[], D_800872F8[];
extern s8 D_800872F0[], D_800E0BD0;
extern s32 D_800D2E48;

s32 func_15089BC0(void) {
    f32 *scales;
    s32 i;
    s32 id;
    void *resource;

    id = D_800D2E48;
    resource = func_1509B570(id);
    if (resource == NULL) return 0;
    switch (id) {
    case 0xB9:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xB8:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xBB:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xBA:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xB7:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xBD:
        D_800D23B0 = (s8 *)resource;
        break;
    case 0xBE:
        D_800D23B0 = (s8 *)resource;
        break;
    default:
        D_800D23B0 = 0;
        return 0;
    }
    func_1508BC20();
    *(s8 *)((u8 *)D_800D23B0 + 0x1700) = 0;
    *(s8 *)((u8 *)D_800D23B0 + 0x1701) = 0;
    *(s8 *)((u8 *)D_800D23B0 + 0x1703) = 0;
    *(s8 *)((u8 *)D_800D23B0 + 0x1702) = 0;
    *(s8 *)((u8 *)D_800D23B0 + 0x1704) = 0;
    *(s16 *)((u8 *)D_800D23B0 + 0x16BC) = id;
    *(f32 *)((u8 *)D_800D23B0 + 0x16B8) = D_800872B0[D_800E0BD0];
    *(s8 *)((u8 *)D_800D23B0 + 0x1745) = 0;
    scales = D_800872F8;
    for (i = 0; i < 16; i++) {
        *(s32 *)((u8 *)D_800D23B0 + i * 4 + 0x16C0) = 0;
        *(f32 *)((u8 *)D_800D23B0 + i * 4 + 0x1868) = scales[D_800E0BD0];
        *(f32 *)((u8 *)D_800D23B0 + i * 4 + 0x17A8) = 0.0f;
        *(f32 *)((u8 *)D_800D23B0 + i * 4 + 0x17E8) = 0.0f;
        *(f32 *)((u8 *)D_800D23B0 + i * 4 + 0x1828) = 0.0f;
    }
    *(s32 *)((u8 *)D_800D23B0 + 0xC) = D_800872F0[D_800E0BD0];
    return 0;
}

s32 func_15089F9C(s32 arg0) {
    f32 dx;
    f32 dz;
    f32 val;
    f32 dist;
    f32 minDist;
    f32 maxDist;
    f32 best;
    s32 mask;
    s32 count;
    s32 bit;
    s32 i;
    s32 bestIdx;
    struct127 *other;
    struct127 *self;
    s32 *states;
    s32 off;

    states = (s32 *)(D_800D23B0 + 0xE64);
    mask = *(s32 *)(D_800D23B0 + 0x14);
    count = *(s32 *)(D_800D23B0 + 0x10);
    off = arg0 * 4;
    bestIdx = -1;
    if (*(s32 *)(D_800D23B0 + off + 0x15C) == 0) {
        minDist = 750.0f;
        maxDist = 3500.0f;
    } else {
        minDist = 1500.0f;
        maxDist = 8000.0f;
    }
    best = -1000000.0f;
    for (i = 0; i < count; i++) {
        bit = 1 << i;
        if ((states[i] != 3) && (mask & bit) &&
            (*(s32 *)(D_800D23B0 + off + 0x16C0) & (bit << 16))) {
            other = &D_800CC2D0[i];
            if (other != NULL) {
                self = &D_800CC2D0[arg0];
                dx = self->x_position - other->x_position;
                dz = self->z_position - other->z_position;
                dist = sqrtf((dx * dx) + (dz * dz));
            } else {
                dist = 32000.0f;
            }
            if (maxDist < dist) {
                dist = maxDist;
            }
            if (dist < minDist) {
                dist = minDist;
            }
            val = sinf(((dist - minDist) * (6.283185482f / (maxDist - minDist))) + -1.570796371f) + 1.0f;
            if (best < val) {
                best = val;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}

s32 func_1508A1BC(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 mask;
    s32 n;
    s32 dist;
    s32 myTeam;
    s32 cur;
    s32 myMask;
    s32 *active;
    s32 *teams;
    s32 *states;
    u32 row;
    s32 count;
    s32 rawDist;
    s32 minDist;
    s32 *kinds;
    s32 *ids;
    s32 *dists;
    f32 dx;
    struct127 *other;
    struct127 *self;

    states = (s32 *)(D_800D23B0 + 0xE64);
    mask = *(s32 *)(D_800D23B0 + 0x14);
    count = *(s32 *)(D_800D23B0 + 0x10);
    kinds = (s32 *)(D_800D23B0 + 0x15C);
    active = (s32 *)(D_800D23B0 + 0x39C);
    teams = (s32 *)(D_800D23B0 + 0x9C);
    ids = (s32 *)(D_800D23B0 + 0x5DC);
    dists = (s32 *)(D_800D23B0 + 0x9DC);
    D_800D23B0[0x1704]++;
    if (*(s32 *)(D_800D23B0 + 0x10) == D_800D23B0[0x1704]) {
        D_800D23B0[0x1704] = 0;
    }
    cur = D_800D23B0[0x1704];
    row = cur * 16;
    if (!((1 << cur) & mask)) {
        for (i = 0; i < 16; i++) ids[row + i] = -1;
        return 0;
    }
    n = 0;
    minDist = 32000;
    myTeam = states[cur];
    myMask = ((s32 *)(D_800D23B0 + 0x16C0))[cur];
    for (i = 0; i < count; i++) {
        if (active[i] > 0 && myTeam != states[i]) {
            other = &D_800CC2D0[i];
            self = &D_800CC2D0[cur];
            if (other != NULL) {
                dx = self->x_position - other->x_position;
                dist = sqrtf((dx * dx) + ((self->z_position - other->z_position) * (self->z_position - other->z_position)));
            } else {
                dist = 32000;
            }
            rawDist = dist;
            if (teams[cur] != teams[i]) {
                dist += 500;
            }
            if ((0x10000 << i) & myMask) {
                if (dist > 3000 && other->unk65 != 0) {
                    dist = -1;
                }
            } else {
                if (i >= D_8008FD90) {
                    dist *= 2;
                }
                if (*(s16 *)(D_800D23B0 + 0x16BC) != 0xB8 && (kinds[i] == 2 || kinds[i] == 3)) {
                    dist = -1;
                }
            }
            if (*(s16 *)(D_800D23B0 + 0x16BC) == 0xB7) {
                if (dist == -1) {
                    dist = rawDist;
                }
                if (other->unk13C != 0) {
                    if (i >= D_8008FD90) {
                        dist *= 2;
                    }
                    dist = dist / (func_150859AC(i, 6) + 2);
                }
            }
            if (i < D_8008FD90 && dist != -1) {
                dist *= *(f32 *)(D_800D23B0 + 0x16B8);
            }
            if (dist >= 0) {
                j = 0;
                while (j < n && (dists + row)[j] < dist) {
                    j++;
                }
                for (k = n; k > j; k--) {
                    ids[(s32)row + k] = ids[(s32)row + k - 1];
                    dists[(s32)row + k] = dists[(s32)row + k - 1];
                }
                ids[row + j] = i;
                n++;
                dists[row + j] = dist;
            }
            if (rawDist < minDist) {
                minDist = rawDist;
            }
        }
    }
    for (i = n; i < 16; i++) ids[row + i] = -1;
    if (D_800D23B0 + 0x51C) {
        ((s32 *)(D_800D23B0 + 0x51C))[cur] = minDist;
    }
    return 0;
}

extern s8 D_800E0BD0;
extern u8 D_800E0BB8[];
extern s8 D_800872D0[];
extern s16 D_800872C4[];
extern s8 D_800872E0[];
extern s8 D_800872E8[];
extern f32 D_800872F8[];
void func_15042D94(s32, s32, s32, char *, ...);
s32 func_1508CAD8(s32, s32, s32, s32, s32, s32);
void func_1508EC5C(s32, s32);
void func_1508EBF8(s32, s32);
s32 func_1508DAEC(s32, s32);
void func_1508B3F8(void);

#define MGR_W(off) ((s32 *)(D_800D23B0 + (off)))
#define MGR_B(off) ((s8 *)(D_800D23B0 + (off)))
#define MGR_F(off) ((f32 *)(D_800D23B0 + (off)))

s32 func_1508A6FC(void)
{
  s32 i;
  s32 bit;
  s32 isNew;
  s32 team;
  struct127 *obj;
  s32 other;
  s32 type;
  s32 result;
  s32 respawn;
  s32 old;
  s32 reset;
  struct127 *tgt;
  s32 mask14;
  s32 mask18;
  s32 count;
  s32 *arr1C;
  s32 *teams;
  s32 *arr35C;
  s32 *arr31C;
  s32 *arr39C;
  s32 *arr19C;
  s32 *arr21C;
  s32 *arr29C;
  s32 *arr25C;
  s32 *arr41C;
  s32 *arr3DC;
  s32 *arr9C;
  s32 *arrDDC;
  s32 *arr1574;
  u8 flag;
  s32 half;
  s32 j;
  s32 pad;
  s32 flags;
  u8 *vp;
  result = 0;
  i = 0;
  mask14 = ((s32 *) (D_800D23B0 + 0x14))[0];
  mask18 = ((s32 *) (D_800D23B0 + 0x18))[0];
  count = ((s32 *) (D_800D23B0 + 0x10))[0];
  teams = (s32 *) (D_800D23B0 + 0xE64);
  arr35C = (s32 *) (D_800D23B0 + 0x35C);
  arr31C = (s32 *) (D_800D23B0 + 0x31C);
  arr19C = (s32 *) (D_800D23B0 + 0x19C);
  arr21C = (s32 *) (D_800D23B0 + 0x21C);
  arr29C = (s32 *) (D_800D23B0 + 0x29C);
  arr25C = (s32 *) (D_800D23B0 + 0x25C);
  arr41C = (s32 *) (D_800D23B0 + 0x41C);
  arr3DC = (s32 *) (D_800D23B0 + 0x3DC);
  arrDDC = (s32 *) (D_800D23B0 + 0xDDC);
  arr1574 = (s32 *) (D_800D23B0 + 0x1574);
  arr1C = (s32 *) (D_800D23B0 + 0x1C);
  arr39C = (s32 *) (D_800D23B0 + 0x39C);
  arr9C = (s32 *) (D_800D23B0 + 0x9C);
  for (i = 0; i < count; i++)
  {
    bit = 1 << i;
    arr1C[i] = 0;
    isNew = 0;
    if (!(mask14 & bit))
    {
      if (!(mask18 & bit))
      {
        isNew = 1;
      }
      mask18 |= bit;
    }
    if ((mask14 & bit) || isNew)
    {
      obj = &D_800CC2D0[i];
      team = teams[i];
      if ((team != 0) && ((*((s16 *) (D_800D23B0 + 0x16BC))) == 0xB9))
      {
        obj->unk1E4 = 1;
      }
      old = arr29C[i];
      arr21C[i] += D_800BE9E4;
      arr29C[i] += D_800BE9E4;
      arr25C[i] -= D_800BE9E4;
      if (arr25C[i] <= 0)
      {
        arr25C[i] = 0;
      }
      arr41C[i] -= D_800BE9E4;
      if (arr41C[i] <= 0)
      {
        arr41C[i] = 0;
      }
      arr1574[i] -= D_800BE9E4;
      if (arr1574[i] < 0)
      {
        arr1574[i] = 0;
      }
      if ((arr19C[i] == 3) && ((arr25C[i] & 0x1F) >= 0x1D))
      {
        func_1508EC5C(i, 0x8000);
      }
      arr3DC[i] = D_800E0BB8[i];
      D_800E0BB8[i] &= 0x7F;
      if (((s32 *) (D_800D23B0 + 0x4DC)) != 0)
      {
        ((s32 *) (D_800D23B0 + 0x4DC))[i] = obj->unk31C->grenade_count;
      }
      arr39C[i] = func_150859AC(i, 4);
      arr9C[i] = func_15085DA8(obj->y_position);
      if (((arr39C[i] == 0) || isNew) || (obj->pad2FC[1] != 0))
      {
        if (arr35C[i] == 0)
        {
          if (obj->unk20F != 0xFF)
          {
            ((s32 *) (D_800D23B0 + 0x59C))[i] = obj->unk20F;
          }
          else
          {
            ((s32 *) (D_800D23B0 + 0x59C))[i] = -1;
            if (!arr3DC)
            {
            }
          }
          arr35C[i] = 1;
          arr25C[i] = 0;
          arr19C[i] = 0;
          ((s32 *) (D_800D23B0 + 0x5C))[i] = 1;
          ((s32 *) (D_800D23B0 + 0x11C))[i] = -1;
          arrDDC[team]--;
          if ((arr3DC[i] & 0x7F) < count)
          {
            other = teams[arr3DC[i] & 0x7F];
            if (team != other)
            {
              arrDDC[other]++;
            }
          }
          if ((*((s16 *) (D_800D23B0 + 0x16BC))) != 0xB9)
          {
            ((s32 *) (D_800D23B0 + 0xFF0))[i] = 0;
            func_1508DAEC(i, 1);
          }
          result |= bit;
        }
      }
      else
      {
        arr35C[i] = 0;
      }
      flag = 1;
      if ((arr31C[i] != 0) && ((*((s16 *) (D_800D23B0 + 0x16BC))) != 0xB8))
      {
        reset = 0;
        if ((arr19C[i] != 1) && (arr21C[i] < 30))
        {
          type = obj->unk232;
          obj->unk222 = arr31C[i] - 2;
          if ((type != 4) && (type != 5))
          {
            func_1508EBF8(i, 4);
          }
          if (type == 4)
          {
            arr21C[i] = 0;
          }
          if (type == 5)
          {
            arr21C[i] = 30;
            ((f32 *) (D_800D23B0 + 0x1868))[i] = D_800872F8[D_800E0BD0];
            ((f32 *) (D_800D23B0 + 0x17A8))[i] = (func_150ADA68() * 50.0f) + obj->x_position;
            ((f32 *) (D_800D23B0 + 0x17E8))[i] = obj->y_position + 40.0f;
            ((f32 *) (D_800D23B0 + 0x1828))[i] = (func_150ADA68() * 50.0f) + obj->z_position;
            if ((arr31C[i] - 2) >= 2)
            {
              tgt = &D_800CC2D0[arr31C[i] - 2];
              ((f32 *) (D_800D23B0 + 0x17A8))[i] += (tgt->x_position - obj->x_position) * 0.5f;
              ((f32 *) (D_800D23B0 + 0x1828))[i] += (tgt->z_position - obj->z_position) * 0.5f;
            }
          }
        }
        flag = 0;
        bit = 0;
        if (D_800872D0[D_800E0BD0] < arr29C[i])
        {
          arr29C[i] = 0;
          old = -1;
        }
        if ((arr29C[i] >= 0) && (old < 0))
        {
          bit = 1;
        }
        if ((D_800872C4[D_800E0BD0] != (-1)) && (arr21C[i] >= D_800872C4[D_800E0BD0]))
        {
          reset = 1;
        }
        ((s8 *) (D_800D23B0 + 0x1725))[i] += (((s8 *) (D_800D23B0 + 0x1705))[i] - ((s8 *) (D_800D23B0 + 0x1725))[i]) >> 2;
        ((s8 *) (D_800D23B0 + 0x1735))[i] += (((s8 *) (D_800D23B0 + 0x1715))[i] - ((s8 *) (D_800D23B0 + 0x1735))[i]) >> 2;
        if (func_1508CAD8(i, arr31C[i] - 2, bit, reset, ((s8 *) (D_800D23B0 + 0x1725))[i], ((s8 *) (D_800D23B0 + 0x1735))[i]) && (arr19C[i] != 1))
        {
          arr21C[i] = 0;
        }
        if (arr29C[i] == 0)
        {
          ((s8 *) (D_800D23B0 + 0x1705))[i] = 0;
          ((s8 *) (D_800D23B0 + 0x1715))[i] = 0;
          if (D_800872E8[D_800E0BD0] < ((s32) (func_150ADA68() * 100.0f)))
          {
            if (func_150ADA68() > 0.5f)
            {
              ((s8 *) (D_800D23B0 + 0x1705))[i] = -50;
            }
            else
            {
              ((s8 *) (D_800D23B0 + 0x1705))[i] = 50;
            }
            if (func_150ADA68() > 0.5f)
            {
              ((s8 *) (D_800D23B0 + 0x1715))[i] = 50;
            }
            else
            {
              ((s8 *) (D_800D23B0 + 0x1715))[i] = 0;
            }
          }
        }
      }
      if (flag && ((*((s16 *) (D_800D23B0 + 0x16BC))) != 0xB8))
      {
        arr29C[i] = -D_800872E0[D_800E0BD0];
      }
      flags = obj->unk22C & 0x200;
      obj->unk22C &= ~0x200;
      if (flags || (arr3DC[i] & 0x80))
      {
        arr19C[i] = 0;
        arr1C[i] = 1;
      }
      if ((*(((u8 *) obj->unk31C) + 0x1AC)) != 0)
      {
        arr25C[i] = 20;
      }
    }
    else
    {
      arr39C[i] = 0;
      arr9C[i] = -1;
    }
  }

  ((s32 *) (D_800D23B0 + 0x18))[0] = mask18;
  if (D_800BE9E4 != 0)
  {
    ((f32 *) (D_800D23B0 + 0x16B4))[0] = D_800BE9E4;
  }
  else
  {
    ((f32 *) (D_800D23B0 + 0x16B4))[0] = 1.0f;
  }
  if (((s32 *) (D_800D23B0 + 0x55C)) != 0)
  {
    func_1508B3F8();
  }
  if ((*((s16 *) (D_800D23B0 + 0x16BC))) == 0xB9)
  {
    half = ((s32) (((f32 *) D_800BE628)[0x30 / 4] - ((f32 *) D_800BE628)[0x2C / 4])) >> 1;
    func_1504332C(0xFF, 0xFF, 0xFF, 0xFF);
    for (j = 0; j <= D_80082FA0; j++)
    {
      if ((*((u8 *) (&D_800CC2D0[j].pad128))) == 0)
      {
        vp = ((u8 *) D_800BE628) + (j * 0x180);
        func_15042D94((s32) ((*((f32 *) (vp + 0x2C))) + ((f32) half)), (s32) ((*((f32 *) (vp + 0x24))) + 6.0f), 1, "%d", arr1574[j] / 60);
      }
    }

  }
  return result;
}

extern s8 D_8008FD90;
extern s16 *D_8008FDD4;

s32 func_1508B194(s32 arg0) {
    if (arg0 >= D_8008FD90) {
        return 0;
    }

    return *(s16 *)((u8 *)D_8008FDD4 + arg0 * 12 + 0x70);
}

void func_1508B1D4(s32 arg0) {
    if (arg0 < D_8008FD90) {
        *(s16 *)((u8 *)D_8008FDD4 + arg0 * 12 + 0x70) = 0;
    }
}

typedef struct {
    f32 radiusSq;
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} Entry1508B20C;

typedef struct {
    u8 pad0[0x1745];
    s8 count;
    u8 pad1746[2];
    Entry1508B20C entries[8];
} Mgr1508B20C;

#define MGR ((Mgr1508B20C *)D_800D23B0)

void func_1508B20C(f32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    s32 n;

    if (MGR != NULL && MGR->count < 8) {
        n = MGR->count++;
        MGR->entries[n].x = arg0;
        MGR->entries[n].y = arg1;
        MGR->entries[n].z = arg2;
        MGR->entries[n].radiusSq = arg3 * arg3;
    }
}

#undef MGR

typedef struct {
    f32 x;
    f32 z;
    f32 radiusSq;
    f32 dist[8];
    s16 count;
    u8 ids[8];
    u8 visited[1];
} Search1508B2A8;

void func_1508B2A8(u8 id, Search1508B2A8 *s) {
    Node15088F30 *node;
    f32 dx;
    f32 dist;
    s32 j;
    u8 link;

    /* BUG (original game): the bit index should be id & 7, so ids 8 apart share a visited bit */
    s->visited[id >> 3] |= 1 << (id & 3);
    node = id + (Node15088F30 *)D_800D2350;
    dx = node->x - s->x;
    dist = node->z - s->z;
    dist = (dx * dx) + (dist * dist);
    if (s->radiusSq < dist) {
        if (s->count < 8) {
            s->ids[s->count] = id;
            s->dist[s->count] = dist;
            s->count++;
        }
    } else {
        for (j = 0; j < 5; j++) {
            if (node->links[j] != 0xFF) {
                link = node->links[j];
                if (!(s->visited[link >> 3] & (1 << (link & 3)))) {
                    func_1508B2A8(node->links[j], s);
                }
            }
        }
    }
}


typedef struct {
    f32 radiusSq;
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
} Entry1508B3F8;

typedef struct {
    f32 x;
    f32 z;
    f32 radiusSq;
    f32 dist[8];
    s16 count;
    u8 ids[8];
    u8 visited[0x20];
} Search1508B3F8;

void func_1508B3F8(void) {
    s32 i;
    s32 j;
    s32 found;
    s32 k;
    u8 best;
    s32 r;
    s32 *zone;
    struct127 *obj;
    f32 x;
    f32 y;
    f32 z;
    f32 dx;
    s32 t;
    f32 d;
    Entry1508B3F8 *e;
    Search1508B3F8 s;
    f32 bestDist;
    B3020Struct *node;

    zone = (s32 *)(D_800D23B0 + 0x55C);
    e = (Entry1508B3F8 *)(D_800D23B0 + 0x1748);
    for (i = 0; i < D_8008FD8C; i++) {
        if (zone[i] != -1) {
            zone[i] = -2;
        }
    }
    for (i = 0; i < *(s8 *)(D_800D23B0 + 0x1745); i++) {
        found = 0;
        s.radiusSq = e[i].radiusSq;
        x = e[i].x;
        y = e[i].y;
        z = e[i].z;
        for (j = D_8008FD90; j < D_8008FD8C; j++) {
            obj = &D_800CC2D0[j];
            d = obj->y_position - y;
            if (d < 200.0f && d > -100.0f) {
                dx = obj->x_position - x;
                d = obj->z_position - z;
                if (dx * dx + d * d < s.radiusSq + 100.0f) {
                    bestDist = 100000000.0f;
                    best = 0xFF;
                    if (!found) {
                        s.count = 0;
                        t = func_15085DA8(y);
                        found = 1;
                        r = func_15085DF8(x, y, z, 0, t);
                        if (r != -1) {
                            s.x = x;
                            s.z = z;
                            s.count = 0;
                            bzero(s.visited, 0x20);
                            func_1508B2A8(r, (Search1508B2A8 *)&s);
                        }
                    }
                    for (k = 0; k < s.count; k++) {
                        node = &D_800D2350[s.ids[k]];
                        dx = node->unk0 - obj->x_position;
                        d = node->unk4 - obj->z_position;
                        d = dx * dx + d * d;
                        if (d < s.dist[k] && d < bestDist) {
                            best = s.ids[k];
                            bestDist = d;
                        }
                    }
                    if (best != 0xFF) {
                        node = &D_800D2350[best];
                        if (zone[j] != -2) {
                            ((s32 *)(D_800D23B0 + 0x5C))[j] = 1;
                        }
                        zone[j] = ((u8 *)node)[7];
                    }
                }
            }
        }
    }
    for (i = 0; i < D_8008FD8C; i++) {
        if (zone[i] < 0) {
            zone[i] = -1;
        }
    }
    *(s8 *)(D_800D23B0 + 0x1745) = 0;
}

s32 func_1508C5B8(s32, s32);
s32 func_1508A1BC(void);

typedef struct B9BCActor {
    u8 pad0[0x14];
    f32 field14;
    f32 field18;
    f32 field1C;
} B9BCActor;

s32 func_1508B9BC(void) {
    s32 mode;
    s32 count;
    s32 *targets;
    u8 *weights;
    s32 winner;
    s32 minimum;
    s32 index;
    s32 cost;
    f32 distance;
    s32 current;
    s32 end;
    s32 *active;
    s32 *neighbors;
    s32 *costs;
    s32 cursor;
    f32 value;

    mode = 1;
    count = *(s32 *)((u8 *)D_800D23B0 + 0x10);
    current = *(s32 *)((u8 *)D_800D23B0 + 4);
    active = (s32 *)((u8 *)D_800D23B0 + 0x39C);
    neighbors = (s32 *)((u8 *)D_800D23B0 + 0x5DC);
    costs = (s32 *)((u8 *)D_800D23B0 + 0x9DC);
    targets = (s32 *)((u8 *)D_800D23B0 + 0x11C);
    if (count >= 9) {
        mode = 4;
    }
    func_1508C5B8(0, mode);
    func_1508A1BC();
    weights = (u8 *)D_800D23B0 + current * 16 + 0x15B4;
    winner = -1;
    minimum = 10000000;
    index = 0;
    if (count > 0) {
        do {
            if (active[index] > 0) {
                cost = -1;
                if (index != current) {
                    cursor = index * 16;
                    end = cursor + 16;
                    while (cursor < end && neighbors[cursor] != -1) {
                        if (neighbors[cursor] == current) {
                            cost = costs[cursor];
                            cursor = end;
                        } else {
                            cursor++;
                        }
                    }
                    if (cost != -1) {
                        value = (f32)(u32)weights[index] * 0.003921569f;
                        if (index == targets[current]) {
                            value *= 0.8f;
                        }
                        cost = (s32)((f32)cost * value);
                        if (cost < minimum) {
                            minimum = cost;
                            winner = index;
                        }
                    }
                }
            }
            index++;
        } while (index != count);
    }
    *(s32 *)((u8 *)D_800D23B0 + current * 4 + 0x49C) = winner;
    if (winner != -1) {
        f32 dx;
        f32 dz;
        B9BCActor *actor;
        B9BCActor *other;

        actor = (B9BCActor *)((u8 *)D_800CC2D0 + current * 0x32C);
        other = (B9BCActor *)((u8 *)D_800CC2D0 + winner * 0x32C);
        dx = actor->field14 - other->field14;
        dz = actor->field1C - other->field1C;
        distance = sqrtf(dx * dx + dz * dz);
        *(s32 *)((u8 *)D_800D23B0 + current * 4 + 0x45C) = (s32)distance;
    } else {
        *(s32 *)((u8 *)D_800D23B0 + current * 4 + 0x45C) = minimum;
    }
    return 0;
}

void func_1508BC20(void) {
    s32 i;
    f32 fn;
    s32 pos;
    s32 val;
    s32 dir;
    s32 delta;
    s32 step;
    s32 h;
    u8 *base;
    s8 n;
    s8 used[16];

    n = D_8008FD8C;
    base = (u8 *)D_800D23B0 + 0x15B4;
    h = n >> 1;
    if (h < 2) {
        h = 2;
    }
    step = 0xAA / h;
    fn = n;
    for (i = 0; i < n; i++) {
        for (h = 0; h < 16; h++) used[h] = 0;
        dir = -1;
        val = 0x55;
        delta = step;
        pos = func_150ADA68() * fn;
        if (func_150ADA68() > 0.5f) {
            dir = 1;
        }
        while (used[pos] != 0) {
            pos += dir;
            if (pos < 0) {
                pos = n - 1;
            }
            if (pos >= n) {
                pos = 0;
            }
        }
        used[pos] = 1;
        for (h = 0; h < n; h++) {
            if (val < 0x100) {
                base[pos] = val;
            } else {
                base[pos] = 0xFF;
            }
            pos++;
            if (pos >= n) {
                pos = 0;
            }
            val += delta;
            if (val >= 0x100) {
                delta = -delta;
            }
        }
        base += 0x10;
    }
}

s32 func_1508BF14(void) {
    s32 numSlots;
    s32 owner;
    s32 pass;
    s32 myIdx;
    s32 i;
    s32 j;
    s32 end;
    s32 cost;
    s32 bestCost;
    s8 counts[16];
    s32 *owners;
    s32 *links;
    s32 *costs;
    s32 *statuses;
    s32 *weights;
    s32 *bestCosts;
    s32 *bestSlots;
    s32 bestSlot;
    s32 n;

    myIdx = ((s32 *)D_800D23B0)[1];
    numSlots = ((s32 *)D_800D23B0)[4];
    statuses = (s32 *)(D_800D23B0 + 0x15C);
    weights = (s32 *)(D_800D23B0 + 0x39C);
    owners = (s32 *)(D_800D23B0 + 0xE64);
    bestCosts = (s32 *)(D_800D23B0 + 0x45C);
    bestSlots = (s32 *)(D_800D23B0 + 0x49C);
    links = (s32 *)(D_800D23B0 + 0x5DC);
    costs = (s32 *)(D_800D23B0 + 0x9DC);
    owner = owners[myIdx];

    for (i = 0; i < numSlots; i++) counts[i] = 0;

    for (pass = 0; pass != 2; pass++) {
        for (i = 0; i < numSlots; i++) {
            n = 0;
            if ((statuses[i] == 2) || (statuses[i] == 3)) {
                n = 1;
            }
            if ((n && (pass == 0)) || (!n && (pass != 0))) {
                if (owners[i] == owner) {
                    bestSlot = -1;
                    bestCost = 10000000;
                    for (j = 0; j < numSlots; j++) {
                        if (weights[j] > 0) {
                            if (owners[j] != owner) {
                                cost = -1;
                                n = j * 16;
                                end = n + 16;
                                while ((n < end) && (links[n] != -1)) {
                                    if (links[n] == i) {
                                        cost = costs[n];
                                        n = end;
                                    } else {
                                        n++;
                                    }
                                }
                                if (cost != -1) {
                                    cost <<= counts[j];
                                    if (cost < bestCost) {
                                        bestCost = cost;
                                        bestSlot = j;
                                    }
                                }
                            }
                        }
                    }
                    counts[bestSlot]++;
                    bestCosts[i] = bestCost;
                    bestSlots[i] = bestSlot;
                }
            }
        }
    }
    return 0;
}

s32 func_1508C194(s32 arg0) {
    return 0;
}

typedef struct GameB4080SightInner {
    u8 pad0[0x75];
    u8 kind75;
    u8 pad76[0xE];
    u8 flag84;
    u8 pad85[0x8F];
    s16 height114;
} GameB4080SightInner;

typedef struct GameB4080SightActor {
    s32 type;
    u8 pad4[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0x184];
    s16 pointX;
    s16 pointY;
    s16 pointZ;
    u8 pad1AA[0x172];
    GameB4080SightInner *inner;
} GameB4080SightActor;

void *func_1505EEF4(s32);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *,
                  f32 *, f32 *, f32 *, f32 *, s32 *, void *, f32);
extern s16 D_800872C4[];
extern s8 D_800E0BD0;

void func_1508C1A4(s32 arg0, s32 arg1) {
    s32 blocked;
    s32 seen;
    GameB4080SightActor *from;
    GameB4080SightActor *to;
    s32 fromFlag;
    s32 toFlag;
    GameB4080SightInner *fromInner;
    f32 distance;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 hitDistance;
    f32 x0;
    f32 x1;
    f32 y0;
    f32 y1;
    f32 z0;
    f32 z1;

    seen = 0;
    blocked = 0;
    from = func_1505EEF4(arg0 & 0xFFFF0FFF);
    to = func_1505EEF4(arg1 & 0xFFFF0FFF);
    if (from != NULL && to != NULL) {
        x0 = from->x;
        z0 = from->z;
        fromInner = from->inner;
        fromFlag = fromInner->flag84;
        if (fromFlag) {
            y0 = from->y + fromInner->height114 * 0.75f;
            if ((fromInner->kind75 & 0x7F) == 9) {
                y0 -= 50.0f;
            }
        } else if (from->type == 0x1F) {
            y0 = from->y + 80.0f;
        } else if (to->inner->flag84 && D_800872C4[D_800E0BD0] != -1) {
            x0 = from->pointX;
            y0 = from->pointY;
            z0 = from->pointZ;
        } else {
            y0 = from->y + 40.0f;
        }
        x1 = to->x;
        z1 = to->z;
        toFlag = to->inner->flag84;
        if (toFlag) {
            y1 = to->y + to->inner->height114 * 0.75f;
            if ((to->inner->kind75 & 0x7F) == 9) {
                y1 -= 50.0f;
            }
        } else if (to->type == 0x1F) {
            y1 = to->y + 80.0f;
        } else if (fromFlag && D_800872C4[D_800E0BD0] != -1) {
            x1 = to->pointX;
            y1 = to->pointY;
            z1 = to->pointZ;
        } else {
            y1 = to->y + 40.0f;
        }
        distance = sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) +
                         (z1 - z0) * (z1 - z0));
        if (func_150AC9C0(x0, y0, z0, x1 - x0, y1 - y0, z1 - z0, NULL, NULL, &hitX,
                          &hitY, &hitZ, &hitDistance, NULL, NULL, 0.0f) == 0) {
            blocked = 1;
        } else if (distance < hitDistance) {
            blocked = 1;
        }
        seen = blocked;
        if (blocked && func_15086D94(x0, y0, z0, x1 - x0, z1 - z0) > 0.0f) {
            blocked = 0;
        }
    }
    if (blocked) {
        *(s32 *)((u8 *)D_800D23B0 + arg0 * 4 + 0x16C0) |= 1 << arg1;
        *(s32 *)((u8 *)D_800D23B0 + arg1 * 4 + 0x16C0) |= 1 << arg0;
    }
    if (seen) {
        *(s32 *)((u8 *)D_800D23B0 + arg0 * 4 + 0x16C0) |= 0x10000 << arg1;
        *(s32 *)((u8 *)D_800D23B0 + arg1 * 4 + 0x16C0) |= 0x10000 << arg0;
    }
}

s32 func_1508C5B8(s32 arg0, s32 arg1) {
    u8 cur;
    u8 other;
    s32 i;
    s32 m;
    s32 j;
    s32 k;
    f32 x0;
    f32 x1;
    f32 dx;
    f32 dz;
    u8 team;
    struct127 *obj;
    s8 idx[4];
    f32 dist[4];
    s32 *list;
    s32 mask;

    cur = D_800D23B0[0x1700];
    other = D_800D23B0[0x1701];
    list = (s32 *)(D_800D23B0 + 0xE64);
    if (arg1 != 0) {
        for (i = 0; i < D_8008FD90; i++) {
            x0 = D_800CC2D0[i].x_position;
            x1 = D_800CC2D0[i].x_position;
            for (k = 0; &dist[k] < &dist[3]; k++) {
                idx[k] = -1;
                dist[k] = 900000000.0f;
            }
            for (k = D_8008FD90; k < D_8008FD8C; k++) {
                if (list[i] != list[k] &&
                    !(((s32 *)(D_800D23B0 + 0x16C0))[i] & (0x10001 << k))) {
                    dx = x0 - D_800CC2D0[i].x_position;
                    dz = x1 - D_800CC2D0[i].z_position;
                    if (i == ((s32 *)(D_800D23B0 + 0x11C))[k]) {
                        dx *= 0.5f;
                        dz *= 0.5f;
                    }
                    dx = dx * dx + dz * dz;
                    j = 2;
                    while (j >= 0 && dx < dist[j]) {
                        dist[j + 1] = dist[j];
                        idx[j + 1] = idx[j];
                        idx[j] = k;
                        dist[j] = dx;
                        j--;
                    }
                }
            }
            for (k = 0; k < 3; k++) {
                if (idx[k] != -1) {
                    j = idx[k];
                    ((s32 *)(D_800D23B0 + 0x16C0))[i] &= ~(0x10001 << j);
                    ((s32 *)(D_800D23B0 + 0x16C0))[j] &= ~(0x10001 << i);
                    func_1508C1A4(i, j);
                }
            }
        }
    }
    if (arg1 != 0) {
        obj = &D_800CC2D0[cur];
        do {
            team = *(u8 *)&obj->pad128;
            m = 0;
            do {
                other++;
                if (other >= D_8008FD8C) {
                    cur++;
                    if (cur >= D_8008FD8C - 1) {
                        cur = 0;
                    }
                    obj = &D_800CC2D0[cur];
                    team = *(u8 *)&obj->pad128;
                    other = cur + 1;
                }
                ((s32 *)(D_800D23B0 + 0x16C0))[cur] &= ~(0x10001 << other);
                ((s32 *)(D_800D23B0 + 0x16C0))[other] &= ~(0x10001 << cur);
                mask = (1 << cur) | (1 << other);
                m++;
            } while (m < 64 && ((D_800D18A0 & mask) ||
                                    team == *(u8 *)&D_800CC2D0[other].pad128));
            func_1508C1A4(cur, other);
        } while (--arg1 != 0);
    }
    D_800D23B0[0x1700] = cur;
    D_800D23B0[0x1701] = other;
    return ((s32 *)(D_800D23B0 + 0x16C0))[arg0];
}

s32 func_1508C9CC(void) {
    s32 count;
    s32 ret;
    s8 cur;
    struct127 *obj;
    struct126 *unk31C;

    count = 0;
    ret = -1;
    cur = D_800D23B0[0x1702];
    do {
        cur = (s8)(cur + 1);
        if (cur >= D_8008FD8C) {
            cur = 0;
        }

        obj = &D_800CC2D0[cur];
        if (obj->interaction_state != 0) {
            unk31C = obj->unk31C;
            if ((unk31C != 0) && (unk31C->unk84 != 0) && ((D_800D18A0 & (1 << cur)) == 0)) {
                count = D_8008FD8C;
                ret = cur;
            }
        }
        count++;
    } while (count < D_8008FD8C);

    D_800D23B0[0x1702] = cur;
    return ret;
}

s32 func_1508CA88(void) {
    s8 **p2;
    s8 temp;

    p2 = &D_800D23B0;
    D_800D23B0[0x1703]++;
    temp = D_800D23B0[0x1703];
    if (temp >= D_8008FD90) {
        D_800D23B0[0x1703] = 0;
        temp = (*p2)[0x1703];
    }
    return temp;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_B4080/func_1508CAD8.s")

extern s16 D_80087320[][3];
void func_1508EB90(s32 arg0, s32 arg1, s32 arg2);
extern f32 D_8009DA8C;

s32 func_1508D850(s32 arg0) {
    s8 *timers;
    s8 *targets;
    s32 *target;
    f32 dx;
    struct127 *obj;
    f32 dz;
    f32 dist;
    s32 range;
    s32 limit;
    s32 off;

    targets = (s8 *)(D_800D23B0 + 0x11C);
    timers = (s8 *)(D_800D23B0 + 0x29C);
    obj = &D_800CC2D0[arg0];
    off = arg0 * 4;
    target = (s32 *)(targets + off);
    obj->unk31C->pad129[0] = *target;
    if (*target < 0) {
        return 0;
    }
    dx = D_800CC2D0[*target].x_position - obj->x_position;
    dz = D_800CC2D0[*target].z_position - obj->z_position;
    dist = sqrtf((dx * dx) + (dz * dz));
    if (5000.0f < dist) {
        range = 0;
    } else if (dist > 2000.0f) {
        range = 1;
    } else {
        range = 2;
    }
    limit = ((s16 *)D_80087320)[D_800E0BD0 * 3 + range];
    if (limit < *(s32 *)(timers + off)) {
        *(s32 *)(timers + off) = 0;
        if (*(s32 *)(D_800D23B0 + off + 0x16C0) & (0x10000 << *target)) {
            func_1508EB90(arg0, 0x61, 0x2000);
        }
    }
    if (*(s32 *)(timers + off) == 0) {
        obj->unk31C->pad129[1] = 0;
        if (D_800872E8[D_800E0BD0] < (s32)(func_150ADA68() * 100.0f)) {
            obj->unk31C->pad129[1] = 1;
        }
    }
}

s32 func_1508DA1C(void) {
    s32 i;
    s32 *a = (s32 *)((u8 *)D_800D23B0 + 0xEB0);
    s32 *b = (s32 *)((u8 *)D_800D23B0 + 0xF70);
    s32 *c = (s32 *)((u8 *)D_800D23B0 + 0xFF0);
    s32 *d = (s32 *)((u8 *)D_800D23B0 + 0x1070);
    s32 *e = (s32 *)((u8 *)D_800D23B0 + 0x10F0);
    s32 *f = (s32 *)((u8 *)D_800D23B0 + 0x12F4);

    for (i = 0; i < *(s32 *)((u8 *)D_800D23B0 + 0x10); i++) {
        a[i] = -1;
    }
    for (i = 0; i < *(s32 *)((u8 *)D_800D23B0 + 0xEA4); i++) {
        b[i] = -1;
        c[i] = 0;
        e[i] = -1;
        d[i] = 1;
        f[i] = 0;
    }
}

extern struct127 D_800CC2D0[26];
extern s8 *D_800D23B0;
s32 func_1508DAEC(s32 arg0, s32 arg1)
{
  s8 *base;
  s32 value;
  s32 *entry;
  s8 *slot;
  s32 slotOffset;
  int mode;
  struct127 *obj;
  base = D_800D23B0;
  value = arg0 * 4;
  entry = (s32 *) (((s32) base) + value);
  if (base == 0)
  {
    return 0;
  }
  value = entry[0x3AC];
  if (value >= 0)
  {
    if (arg1 < 0)
    {
      return 1;
    }
    entry[0x3AC] = -1;
    slotOffset = value * 4;
    obj = &D_800CC2D0[arg0];
    slot = D_800D23B0 + slotOffset;
    if (((*((s32 *) (slot + 0x11F4))) & 2) == 0)
    {
      *((s32 *) (slot + 0xF70)) = -1;
      *((s32 *) ((D_800D23B0 + slotOffset) + 0x10F0)) = -1;
      mode = *((s32 *) (D_800D23B0 + 0xEAC));
      if (arg1 == 1)
      {
        *((s32 *) ((D_800D23B0 + slotOffset) + 0x12F4)) = 0x12C;
        obj = &D_800CC2D0[arg0];
        mode = 4;
        *((s32 *) ((D_800D23B0 + slotOffset) + 0x1374)) = (s32) obj->x_position;
        *((s32 *) ((D_800D23B0 + slotOffset) + 0x13F4)) = (s32) (obj->y_position + 50.0f);
        *((s32 *) ((D_800D23B0 + slotOffset) + 0x1474)) = (s32) obj->z_position;
      }
      *((s32 *) ((D_800D23B0 + slotOffset) + 0x1070)) = mode;
    }
  }
  return 0;
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_B4080/func_1508DC24.s")

void func_1508E6C8(void) {
}

s32 func_1508E6D0(s32 arg0) {
    switch (arg0) {
    case 0x3F:
        return 0x708;
    case 0x14:
        return 0xC80;
    case 0x22:
        return 0x1F4;
    case 0x23:
        return 0x2710;
    case 0x16:
        return 0x186A0;
    case 0x15:
        return 0x1F4;
    case 0x21:
        return 0x1F4;
    case 0x24:
        return 0x186A0;
    case 0x19:
    case 0x40:
        return 0x1F4;
    case 0x9:
        return 0x1388;
    case 0x38:
        return 0x1388;
    case 0x18:
    case 0x41:
        return 0x186A0;
    case 0x3:
        return 0x2EE;
    case 0x37:
        return 0x5DC;
    default:
        return 0x186A0;
    }
}

s32 func_1508E780(s32 arg0) {
    if (*(s16 *)((u8 *)D_800D23B0 + 0x16BC) != 0xB8) {
        switch (arg0) {                             /* switch 1 */
        case 0x14:                                  /* switch 1 */
            return 0x28;
        case 0x3F:                                  /* switch 1 */
            return 0x2F;
        case 0x22:                                  /* switch 1 */
            return 0x56;
        case 0x23:                                  /* switch 1 */
            return 0x16;
        case 0x16:                                  /* switch 1 */
            return 0x24;
        case 0x15:                                  /* switch 1 */
            return 0x4B;
        case 0x21:                                  /* switch 1 */
            return 0x58;
        case 0x24:                                  /* switch 1 */
            return 0x63;
        case 0x19:                                  /* switch 1 */
            return 0x57;
        case 0x40:                                  /* switch 1 */
            return 0x21;
        case 0x9:                                   /* switch 1 */
            return 0x1D;
        case 0x38:                                  /* switch 1 */
            return 0x7B;
        case 0x39:                                  /* switch 1 */
            return 0x82;
        case 0x37:                                  /* switch 1 */
            return 0x85;
        case 0x18:                                  /* switch 1 */
            return 0x55;
        case 0x41:                                  /* switch 1 */
            return 0x2E;
        case 0x25:                                  /* switch 1 */
            return 0x1060;
        default:                                    /* switch 1 */
            return -1;
        }
    } else {
        switch (arg0) {                             /* switch 2; irregular */
        case 8:                                     /* switch 2 */
            return -1;
        case 1:                                     /* switch 2 */
            return 0x105C;
        case 16:                                    /* switch 2 */
            return 0x1019;
        case 2:                                     /* switch 2 */
            return 0x1059;
        default:                                    /* switch 2 */
            return -1;
        }
    }
}

void func_15062800(struct127 *, struct127 *);

s32 func_1508E89C(s32 arg0, s32 arg1) {
    struct127 *obj;
    s32 savedRot;
    s32 bit0;
    s32 tmp;
    u8 slot;
    s8 *res;

    obj = &D_800CC2D0[arg0];
    if (((u8 *)&D_800D18A4)[arg0] > 50) {
        if (!(D_800D18A2 & (1 << arg0))) {
            bit0 = 1 << arg0;
            D_800D18A2 |= bit0;
            D_800D18A0 &= ~bit0;
            D_800D18A0 |= (1 << arg1);
            tmp = func_150859AC(arg1, 3);
            func_15085710(arg0, 2, tmp);
            func_15085710(arg1, 2, 0);
            if (*(s32 *)(D_800D23B0 + 0x18) != 0) {
                *(s32 *)(D_800D23B0 + 0x18) &= ~bit0;
                *(s32 *)(D_800D23B0 + 0x18) |= (1 << arg1);
            }
            if (D_800D23B0 + 0x9C) {
                ((s32 *)(D_800D23B0 + 0x9C))[arg0] = ((s32 *)(D_800D23B0 + 0x9C))[arg1];
            }
            if (D_800D23B0 + 0x39C) {
                ((s32 *)(D_800D23B0 + 0x39C))[arg0] = ((s32 *)(D_800D23B0 + 0x39C))[arg1];
            }
            if (D_800D23B0 + 0x3DC) {
                ((s32 *)(D_800D23B0 + 0x3DC))[arg0] = ((s32 *)(D_800D23B0 + 0x3DC))[arg1];
            }
            if (D_800D23B0 + 0xEB0) {
                ((s32 *)(D_800D23B0 + 0xEB0))[arg0] = ((s32 *)(D_800D23B0 + 0xEB0))[arg1];
                if (((s32 *)(D_800D23B0 + 0xEB0))[arg0] >= 0) {
                    ((s32 *)(D_800D23B0 + 0xF70))[((s32 *)(D_800D23B0 + 0xEB0))[arg0]] = arg0;
                }
            }
            ((s32 *)(D_800D23B0 + 0x16C0))[arg0] = ((s32 *)(D_800D23B0 + 0x16C0))[arg1];
            savedRot = obj->unk31C->unk1AA;
            func_15062800(&D_800CC2D0[arg1], obj);
            if (D_800BE9F0 == 0x3F) {
                if (obj->unk13C >= 100) {
                    slot = obj->unk13C - 100;
                    res = func_1509B570(D_800D2E48);
                    if (res != NULL) {
                        s32 idx;
                        s32 id;

                        idx = obj - D_800CC2D0;
                        id = D_800CC2D0[slot].unique_id - 20;
                        *(s32 *)(res + id * 4 + 0x24B8) = idx | 0x2000;
                        *(s32 *)(res + id * 4 + 0x24DC) = idx;
                    }
                }
            }
            obj->unk31C->unk84 = 0;
            obj->immune = 90;
            obj->unk31C->unk1AA = savedRot;
            *((u8 *)obj->unk31C + 0x1AF) = 0x40;
            func_15128774(obj->camera, obj);
            (*(s8 *)&D_8008FD94)++;
        }
    }
    return 0;
}

extern struct127 D_800CC40F[];

void func_1508EB90(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp = *(u8*)&D_800CC40F[arg0];
    func_1509BFB0(1, temp | 0x2000, arg1, arg2);
}

void func_1508EBF8(s32 arg0, s32 arg1) {
    s32 temp = *(u8*)&D_800CC40F[arg0];
    func_1509BFB0(1, temp | 0x2000, 0x14, arg1);
}

void func_1508EC5C(s32 arg0, s32 arg1) {
    s32 temp = *(u8*)&D_800CC40F[arg0];
    func_1509BFB0(1, temp | 0x2000, 0x61, arg1);
}

typedef struct {
    u16 unk0;
    u16 count;
    s32 unk4;
    u16 entries[8];
} GameB4080Group;

s32 func_1508ECC0(u16 arg0, u16 arg1, u16 arg2) {
    u32 i;
    u32 j;
    u32 k;
    u16 key;

    key = (arg0 << 12) + arg1;
    for (i = 0; i < D_80087380; i++) {
        for (j = 0; j < (*(GameB4080Group **)&D_800D23C0)[i].count; j++) {
            if (key == (*(GameB4080Group **)&D_800D23C0)[i].entries[j]) {
                for (k = 0; k < (*(GameB4080Group **)&D_800D23C0)[i].count; k++) {
                    if (arg2 == ((*(GameB4080Group **)&D_800D23C0)[i].entries[k] >> 12) && k != j) {
                        return (*(GameB4080Group **)&D_800D23C0)[i].entries[k] & 0xFFF;
                    }
                }
                return -1;
            }
        }
    }
    return -1;
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8[4];
} Struct1508EDBC;

void func_1508EDBC(s32 arg0) {
    if (arg0 < D_80087380) {
        ((Struct1508EDBC *)D_800D23C0)[arg0].unk2 = 0;
        ((Struct1508EDBC *)D_800D23C0)[arg0].unk4 = 0;
        ((Struct1508EDBC *)D_800D23C0)[arg0].unk0 = 0;
    }
}

void func_1503DDD0(s32);
void func_15114B94(u32);

void func_1508EE0C(u16 arg0, u16 arg1) {
    u32 i;
    u32 j;
    s32 operand;
    u16 key;

    key = (arg0 << 12) + arg1;
    for (i = 0; i < D_80087380; i++) {
        for (j = 0; j < (*(GameB4080Group **)&D_800D23C0)[i].count; j++) {
            if ((*(GameB4080Group **)&D_800D23C0)[i].entries[j] == key) {
                for (j = 0; j < (*(GameB4080Group **)&D_800D23C0)[i].count; j++) {
                    operand = (*(GameB4080Group **)&D_800D23C0)[i].entries[j] & 0xFFF;
                    switch ((*(GameB4080Group **)&D_800D23C0)[i].entries[j] >> 12) {
                    case 2:
                        func_15114B94(operand);
                        break;
                    case 3:
                        func_1503DDD0(operand);
                        break;
                    }
                }
                func_1508EDBC(i);
                return;
            }
        }
    }
}
