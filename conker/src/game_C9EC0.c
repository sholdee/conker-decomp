#include <ultra64.h>
#include "functions.h"
#include "variables.h"


typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} GameC9EC0Struct;

extern GameC9EC0Struct D_80087430[];
extern GameC9EC0Struct D_80088420[];
s32 func_1509CA10(s32 arg0) {
    return D_80087430[arg0].unk0;
}

typedef struct {
    u16 unk0;
    u16 unk2;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} GameC9EC0StructB;

extern GameC9EC0StructB D_80087438[];

s32 func_1509CA30(s32 arg0) {
    return D_80087438[arg0].unk0;
}

extern GameC9EC0StructB D_8008743A[];

s32 func_1509CA50(s32 arg0) {
    return D_8008743A[arg0].unk0 & 0xFFF;
}

extern GameC9EC0Struct D_80087434[];

s32 func_1509CA78(s32 arg0) {
    return D_80087434[arg0].unk0;
}

s32 func_1509CA98(s32 arg0) {
    s32 i;

    i = 0;
loop:
    if (D_80087430[i].unk0 && arg0 == ((D_80087430[i].unk4 & 0x1FFFFFFF) + 1)) {
        return i;
    }
    if (D_80087430[i + 1].unk0 && arg0 == ((D_80087430[i + 1].unk4 & 0x1FFFFFFF) + 1)) {
        return i + 1;
    }
    if (D_80087430[i + 2].unk0 && arg0 == ((D_80087430[i + 2].unk4 & 0x1FFFFFFF) + 1)) {
        return i + 2;
    }
    if (D_80087430[i + 3].unk0 && arg0 == ((D_80087430[i + 3].unk4 & 0x1FFFFFFF) + 1)) {
        return i + 3;
    }
    i += 4;
    if (i != 0xCC) {
        goto loop;
    }

    return 0xCC;
}

s32 func_1509CB68(void) {
    s32 count;
    s32 i;

    count = 0;
    i = 0;
    do {
        if (D_80087430[i].unk0 != 0) {
            count++;
        }
        if (D_80087430[i + 1].unk0 != 0) {
            count++;
        }
        if (D_80087430[i + 2].unk0 != 0) {
            count++;
        }
        if (D_80087430[i + 3].unk0 != 0) {
            count++;
        }
        i += 4;
    } while (D_80088420 != &D_80087430[i]);
    return count;
}

extern GameC9EC0Struct D_8008743C[];

s32 func_1509CBD4(s32 arg0) {
    u16 *p;
    s32 ret;
    s32 i;

    p = (u16 *)D_8008743C[arg0].unk0;
    for (i = 0; p[i] != 0xFFFF; i++) {
        ret = func_1509CBD4(p[i]);
        if (!(((u8 *)D_800D2E4C)[p[i] >> 3] & (1 << (p[i] & 7))) || ret == 0) {
            return 0;
        }
    }
    return 1;
}


extern void func_1509CE64(void *, void *);
extern GameC9EC0Struct D_8008743C[];
void func_1509CCB4();

void func_1509CC94(void) {
    func_1509CCB4();
}

void func_1509CCB4(s32 arg0) {
    func_1509CE64((void *)D_8008743C[arg0].unk0, func_1509CCB4);
}

extern s32 D_800D2E70;
extern struct102 *D_800D2E4C;
extern GameC9EC0Struct D_80087440[];
void func_1509CCB4();
void func_1509CCB4(s32 arg0);
s32 func_1509CCF4(s32 arg0)
{
  u16 *p;
  s32 count;
  count = 0;
  p = (u16 *) D_80087440[arg0].unk0;
  while ((*p) != 0xFFFF)
  {
    ((u8 *) D_800D2E4C)[(*p) >> 3] |= 1 << ((*p) & 7);
    if ((((u8 *) (&D_800D2E70))[*p] != 3) != ((0, 0U)))
    {
      ((u8 *) (&D_800D2E70))[*p] = 3;
      func_1509CCB4(*p);
      count++;
    }
    p++;
  }

  return count;
}


void func_1509CDDC(s32 arg0) {
    s32 i;
    s32 total;

    func_1509CCF4(arg0);
    do {
        total = 0;
        for (i = 0; i != 0xCC; i++) {
            if ((*(u8 (*)[0xF0])&D_800D2E70)[i] == 3) {
                total += func_1509CCF4(i);
            }
        }
    } while (total != 0);
}

void func_1509CE64(void *arg0, void *arg1) {
    u16 *p;
    void (*cb)(s32);

    p = (u16 *)arg0;
    cb = (void (*)(s32))arg1;
    while (*p != 0xFFFF) {
        cb(*p);
        ((u8 *)&D_800D2E70)[*p] = 3;
        ((u8 *)D_800D2E4C)[*p >> 3] |= (1 << (*p & 7));
        p++;
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u32 unkC;
} Res1509CF28;

typedef struct {
    s32 *unk0;
    s32 unk4;
} Ent1509CF28;

extern s32 D_800D2FB0;
extern Ent1509CF28 *func_1502B6BC(s32, s32, u32 *, s32, s32, s32, s32);

s32 func_1509CF28(s32 arg0, s32 *arg1)
{
  Ent1509CF28 *table;
  u32 i;
  s32 done;
  u32 count;
  s32 *p;
  s32 flags;
  s32 pad1;
  s32 pad2;
  count = 0;
  D_800D2FB0 = (s32) func_1502B6BC(0, 0, &count, 3, 8, 0, arg0);
  if (D_800D2FB0 == 0)
  {
    *arg1 = 0;
    return 0;
  }
  table = (Ent1509CF28 *) D_800D2FB0;
  table->unk4 = count - 1;
  *arg1 = count - 1;
  for (i = 1; i < count; i++)
  {
    done = 0;
    p = ((Ent1509CF28 *) D_800D2FB0)[i].unk0;
    do
    {
      p[0] += (s32) ((Ent1509CF28 *) D_800D2FB0)[i].unk0;
      flags = p[1];
      if (flags & 0x80000000)
      {
        if (1)
        {
          done = 1;
        }
      }
      p[1] = flags & (~0xF0000000);
      p += 2;
    }
    while (!done);
    ((Res1509CF28 *) (&table[1])[i - 1].unk0)->unkC = (((Res1509CF28 *) (&table[1])[i - 1].unk0)->unkC & 0x0FFFFFFF) >> 1;
  }

  return 1;
}

extern s32 D_800D2FB0;
extern s32 func_10004074(s32);

void func_1509D054(void) {
    s32 temp;
    if (D_800D2FB0) {
        temp = D_800D2FB0;
        func_10004074(temp);
        D_800D2FB0 = 0;
    }
}

s32 func_1509D08C(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3) {
    u8 *base;
    u8 *entry;
    s16 **resource;
    s16 *dependencies;
    s16 dependency;
    u32 index;
    u32 count;

    base = (u8 *)D_800D2FB0;
    entry = ((u8 (*)[8])base)[arg1];
    if (base == 0) {
        if (arg2 != 0) {
            *arg2 = 0xBF;
        }
        if (arg3 != 0) {
            *arg3 = 0;
        }
        return 0;
    }
    resource = *(s16 ***)(entry + 8);
    if (resource == 0) {
        if (arg2 != 0) {
            *arg2 = 0xBF;
        }
        if (arg3 != 0) {
            *arg3 = 0;
        }
        return 0;
    }
    if (arg2 != 0) {
        *arg2 = resource[2][0];
        resource = *(s16 ***)(entry + 8);
    }
    if (arg3 != 0) {
        *arg3 = resource[0][0];
        resource = *(s16 ***)(entry + 8);
    }
    index = 0;
    count = *(u32 *)((u8 *)resource + 0xC);
    if (count != 0) {
        dependencies = resource[2];
        do {
            dependency = *dependencies;
            index++;
            if (!(
                  ((u8 *)D_800D2E4C)[dependency >> 3] & 1 << (*dependencies & 7))) {
                return 0;
            }
            dependencies++;
        } while (index < count);
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_C9EC0/func_1509D180.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_C9EC0/func_1509D780.s")

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
} Func1509D8FCArg;

typedef struct {
    f32 unk0;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
} Func1509D8FCMsg;

extern void *func_1505EEF4(s32);
extern void func_15197A7C(void *);
extern void func_151645C4(u8);
extern void func_1515F170(s32 arg0, u8 arg1);
extern void func_151616D0(u8, u8, s32);
extern void func_150DEC28(u8, u8);
extern u8 D_800A0960[][16];

s32 func_1509D8FC(s32 arg0, s32 arg1, Func1509D8FCArg *arg2) {
    switch (arg0) {
        case 52:
            if (arg1 == 3) {
                func_15197A7C(func_1505EEF4(arg2->unk4 & 0xFFF));
            }
            break;

        case 50:
            if (arg1 == 2) {
                func_151403A8(0, 0x53);
            } else if (arg1 == 3) {
                func_151403A8(0, 0x54);
            }
            break;
        case 45:
            func_151645C4(arg1 == 6);
            break;
        case 40:
            if (arg1 == 2) {
                func_1515F170(4, 1);
                func_1515F170(5, 0);
            } else if (arg1 == 3) {
                func_1515F170(4, 0);
                func_1515F170(5, 1);
            }
            break;
        case 31:
            if (arg1 == 5) {
                u8 buf[8];
                buf[0] = arg2->unk8;
                func_151403A8((s32)buf, 0x36);
            }
        case 30:
            if (arg1 == 2) {
                u8 buf2[4];
                buf2[0] = arg2->unk8;
                func_151494E0((s32)buf2, 0x34);
            }
            break;
        case 29:
            if (arg1 == 5) {
                Func1509D8FCMsg msg;
                msg.unk0 = arg2->unkC;
                msg.unk4 = arg2->unk8;
                msg.unk5 = arg2->unk10;
                msg.unk6 = arg2->unk14;
                msg.unk7 = arg2->unk18;
                func_1516944C(0x35, (s32)&msg, 0x33);
            }
            break;
        case 28:
            if (arg1 == 5) {
                s32 idx = arg2->unk8;
                if (idx < 0 || idx >= 3) {
                    return 1;
                }
                func_151494E0((s32)D_800A0960[idx], 0x32);
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14: {
            s32 x;
            u8 a;
            u8 b;

            x = arg0 & 0x3F;
            if (x == 11) {
                a = 1;
            } else if (x == 12) {
                a = 2;
            } else if (x == 13) {
                a = 3;
            } else if (x == 14) {
                a = 4;
            } else {
                a = 0;
            }
            if (arg1 == 2) {
                b = 0x17;
            } else if (arg1 == 3) {
                b = 0x18;
            } else {
                b = 0x17;
            }
            func_151616D0(a, b, 0);
            break;
        }
        case 17:
            func_150DEC28(arg2->unk8, arg1 == 3);
            break;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_C9EC0/func_1509DBBC.s")
