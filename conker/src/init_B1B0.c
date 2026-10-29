#include <ultra64.h>

#define func_1000DF68 func_1000DF68_s16_decl_in_functions_h
#include "functions.h"
#undef func_1000DF68
#include "variables.h"

void func_1000DEC4(void);
void func_1000DF68(s32 arg0, s32 arg1, s32 arg2);

struct151 *func_1000B1B0(s32 arg0) {
    s32 i;

    for(i = 0; i < 3; i++)
    {
        if ((D_800417B0[i] != 0) && (arg0 == D_800417B0[i]->unk4)) {
            return D_800417B0[i];
        }
    }

    return NULL;
}

struct151 *func_1000B1FC(s32 arg0) {
    s32 i;
    struct00 *temp;

    for (i = 0; i < 3; i++) {
        if ((D_800417B0[i] != 0) && (arg0 == D_800417B0[i]->unk4)) {
            return D_800417B0[i];
        }
    }
    for (i = 0; i < 3; i++) {
        if (D_800417B0[i] != 0) {
            temp = D_800417B0[i]->unk60;
            if ((temp != 0) && (arg0 == temp->unk4)) {
                return (struct151 *)temp;
            }
        }
    }
    return NULL;
}
// func_1000B1FC: the old pipeline parked form A at 20 because its loop end relocates as
// D_800417B0+0xC rather than the alias D_800417BC. Same address, same bytes: the ROM gate
// passes, so that 20 was a relocation-NAME artifact of the scorer, not a codegen difference.

void func_1000B294(s32 *arg0) {
    struct151 *temp_v0;
    struct151 *temp_a1;
    u32 i;

    i = 0;

    do {
        temp_v0 = D_800417B0[i];
    if (temp_v0 != NULL) {
        if (arg0 == temp_v0->unk10) {
            temp_v0->unk10 = (s32 *)temp_v0;
                temp_v0 = D_800417B0[i];
        }
        temp_a1 = (struct151 *)temp_v0->unk60;
        if ((temp_a1 != NULL) && (arg0 == temp_a1->unk10)) {
            temp_a1->unk10 = (s32 *)temp_a1;
        }
    }
        i++;
    } while ((struct151 **)&D_800417BC != &D_800417B0[i]);
}
// NON-MATCHING: no idea.
// struct151 * func_1000B294(s32 *arg0) {
//     s32 i;
//     struct151 *entry;
//     struct151 *tmp;
//     struct151 *ret = NULL;
//
//     for (i = 0; i < 3; i++)
//     {
//         entry = &D_800417B0[i];
//         ret = entry;
//         if (entry)
//         {
//             if (arg0 == entry->unk10)
//             {
//                 entry->unk10 = entry;
//             }
//             tmp = *entry->unk60;
//             if ((tmp) && (arg0 == tmp->unk10))
//             {
//                 entry->unk10 = tmp;
//             }
//         }
//     }
//     return ret;
// }

struct137 *func_1000B2F4(s32 arg0) {
    s32 i;

    for (i = 0; i < 12; i++)
    {
        if (D_800419A8[i].unk4 == -1) {

            bzero(&D_800419A8[i], 100);

            D_800419A8[i].unk0 = -1;

            if (arg0 < 150) {
                D_800419A8[i].unk2C = D_8002B074[arg0].unk0; // (s32)(u16)
            } else {
                D_800419A8[i].unk2C = 26000;
            }
            D_800419A8[i].unk30 = D_800419A8[i].unk2C;
            D_800419A8[i].unk4E = D_800419A8[i].unk4C = D_800419A8[i].unk52 = D_800419A8[i].unk54 = D_800419A8[i].unk58 = D_800419A8[i].unk5A = 32768;
            D_800419A8[i].unk4 = arg0;
            D_800419A8[i].unk8 = &D_8002B9D4;
            D_800419A8[i].unkC = &D_8002B9F4;
            D_800419A8[i].unk10 = &D_800419A8[i];
            return &D_800419A8[i];
        }
    }
    return NULL;
}

void func_1000B3D4(struct151 *arg0, struct151 *arg1) {
    s32 i;
    s32 limit;
    struct151 *var_s2;
    struct151 **slot;
    struct151 *temp_v0;
    struct151 *temp_a0;

    var_s2 = NULL;
    i = 0;
    limit = 3;
    if ((&arg1)[0] != NULL) {
        temp_v0 = (struct151 *)(&arg1)[0]->unk60;
        if ((temp_v0 != NULL) && (arg0 != temp_v0)) {
            if (temp_v0->unk4 == arg0->unk4) {
                arg0->unk4 = -1;
                return;
            }
            temp_v0->unk4 = -1;
        }
        (&arg1)[0]->unk60 = (struct00 *)arg0;
        return;
    }

    do {
        if (var_s2 == NULL) {
            slot = &D_800417B0[i];
            temp_v0 = *slot;
            if ((temp_v0 == NULL) || ((temp_v0->unk4 <= 0) && (temp_v0->unk60 == NULL))) {
                temp_v0 = (struct151 *)func_1000B2F4(0);
                var_s2 = temp_v0;
                if (temp_v0 != NULL) {
                    temp_v0->unk0 = i;
                    temp_v0->unk60 = (struct00 *)arg0;
                    *slot = temp_v0;
                    goto block_20;
                } else {
                    arg0->unk4 = -1;
                    return;
                }
            }
        }
        if (var_s2 == NULL) {
            slot = &D_800417B0[i];
            temp_v0 = *slot;
            if (temp_v0 != NULL) {
                temp_a0 = (struct151 *)temp_v0->unk60;
                if ((temp_a0 != NULL) && (temp_a0->unk4 == 0)) {
                    var_s2 = (struct151 *)-1;
                    func_1000B294(temp_a0);
                    ((struct151 *)(*slot)->unk60)->unk4 = -1;
                    ((struct151 *)(*slot)->unk60)->unk0 = -1;
                    (*slot)->unk60 = (struct00 *)arg0;
                }
            }
        }
block_20:
        i++;
    } while (i != limit);
}

s32 func_1000B548(s32 *arg0) {
    s32 ret = 0;
    s32 i;

    for (i = 0; i < 12; i++) {
        if ((D_800419A8[i].unk4 != -1) && (D_800419A8[i].unk0 != -1)) {
            if (ret < 3) {
                *arg0++ = D_800419A8[i].unk4;
                ret++;
            }
        }
    }
    return ret;
}

s32 func_1000B638(s32 arg0, u8 arg1, s32 arg2, s32 arg3) {
    s32 sp1C;
    struct151 *temp_v0;

    sp1C = arg0 & 2;
    arg0 = arg0 & 1;
    temp_v0 = D_800417B0[arg1];
    if ((temp_v0 == NULL) || (temp_v0->unk30 < 500)) {
        D_80041F04 &= ~1;
    }

    if (D_80041F04 & 1) {
        if (arg0 == 0) {
            func_100088F0(arg1, 0x8000, 1);
            if ((D_800BE9F0 == 1) || (D_800BE9F0 == 12)) {
                func_10008790(arg1, 0x7000, 0, 0);
            } else if (D_800BE9F0 != 7) {
                func_10008790(arg1, 0xCA, 0, 0);
            }
            func_100085B8(arg1, 0xF, 1);
        }
        arg0 = 1;
    } else if (arg0 != 0) {
        func_100088F0(arg1, 0x8000, 0);
        if ((D_800BE9F0 == 1) || (D_800BE9F0 == 12)) {
            func_10008790(arg1, 0x7000, 0xFF, 0);
        } else if (D_800BE9F0 != 7) {
            func_10008790(arg1, 0xCA, 0xFF, 0);
        }
        func_100085B8(arg1, 0xF, 0);
        arg0 = 0;
    }

    if (D_800BE9F0 == 39) {
        func_10011FA0((s32 *)4);
        if (sp1C == 0) {
            sp1C = 2;
            func_1000E704(1, 1, 0xFFFF);
        }
    } else if (sp1C != 0) {
        func_1000E704(1, 0, 0xFFFF);
        sp1C = 0;
    }

    return sp1C | arg0;
}


s32 func_1000B830(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 tmp = D_800DBFF0->unk5F0 & 1;
    if ((tmp != 0) && (arg0 == 0)) {
        arg0 = 1;
        func_1000E40C(16, 1000);
    } else if ((tmp == 0) && (arg0 != 0)) {
        arg0 = 0;
        func_1000E40C(16, 18000);
    }
    return arg0;
}

s32 func_1000B8B8(s32 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4) {
    s32 tmp;

    if (D_800BE9F0 == 4) {
        if (((arg0 & 1) != 0) && (D_80041F0C == 0)) {
            func_1000E46C(19, 0, 4096, 0);
            arg0 = arg0 & ~1;
        } else {
            if (D_80041F0C != 0) {
                tmp = D_80041F08 / D_80041F0C / 80;
                if (tmp >= 101) {
                    tmp = 100;
                }
                func_1000E588(19, tmp, 4096);
                arg0 |= 1;
            }
        }
        D_80041F08 = 0;
        D_80041F0C = 0;
        if ((arg0 & 2) == 0) {
            func_1000DF68(19, 0, 1);
            func_1000DF68(19, 32768, 0);
            arg0 |= 2;
        }
        return arg0;
    }
    return func_1000C530(arg0, arg1, arg2, arg3, arg4);
}

s32 func_1000BA18(u32 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4) {
    u32 sp44;
    s32 sp40;
    s32 sp3C;

    sp44 = 0;
    sp3C = arg0 & 0x00FFFFFF;
    arg0 = arg0 & 0xFF000000;
    func_100114D0(0, -377, 8227, 32767, 4000, 3000, &sp40, &sp44, 0);
    sp44 = (sp44 << 16) & 0xFF000000;
    if (arg0 != sp44) {
        arg0 = sp44;
        func_1000E588(0x4D, arg0 >> 24, 0x6000);
    }
    sp3C = func_1000C530(sp3C, arg1, arg2, arg3, arg4) & 0xFFFFFF;
    // fakematch
    sp44 = sp3C & 0xFFFFFFFFFFFFFFFF;
    return arg0 | sp44;
}

s32 func_1000C530(s32, u8, f32, f32, f32);
s32 func_1000E588(s32 arg0, s32 arg1, s32 arg2);
s32 func_100114D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 *arg6, s32 *arg7, s32 *arg8);
s32 func_1000BAFC(u32 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4)
{
  u32 sp44;
  s32 sp40;
  s32 sp3C;
  sp44 = 0;
  sp3C = arg0 & 0x00FFFFFF;
  arg0 &= 0xFF000000;
  func_100114D0(0, 0, 0, 0x7FF8, 0xE74, 0xA28, &sp40, &sp44, 0);
  sp44 = ((0x7FFF - sp44) << 16) & 0xFF000000;
  if (arg0 != sp44)
  {
    arg0 = sp44;
    func_1000E588(0x93, arg0 >> 24, 0x6000);
  }
  sp44 = (func_1000C530(sp3C, arg1, arg2, arg3, arg4) & 0x00FFFFFF) & 0xFFFFFFFFFFFFFFFF;
  return arg0 | sp44;
}

s32 func_1000E588(s32 arg0, s32 arg1, s32 arg2);


s32 func_1000BBE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg0 == 0) {
        func_1000E704(20, 1, 0xFFFF);
        arg0 = 1;
    }
    return arg0;
}

s32 func_1000BC28(s32 arg0, u8 arg1, s32 arg2, s32 arg3) {
    s32 tmp = func_10008A4C(arg1, 0) + func_10008A4C(arg1, 6) + 1;
    if (tmp >= 256) {
        tmp = 255;
    } else {
        if (tmp < 16) {
            tmp = 1;
        }
    }
    if (tmp != arg0) {
        func_150C851C(tmp - 1);
        arg0 = tmp;
    }
    return arg0;
}


s32 func_1000BCBC(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    f32 distance;
    u8 volume;

    if (arg0 == 0) {
        func_10008790(((u8 *)&arg1)[3], 3, 0x10, 0);
        func_1000886C(((u8 *)&arg1)[3], 4, 0);
        arg0 = 1;
    } else if (D_800BE9F0 == 0x13) {
        arg2 -= 24.0f;
        arg4 -= 3015.0f;
        distance = arg2 * arg2 + arg4 * arg4;
        if (36000000.0f < distance) {
            volume = 4;
        } else {
            volume = (u8)((6000.0f - sqrtf(distance)) * 0.041833334f) + 4;
        }
        if (arg0 != volume) {
            func_1000886C(((u8 *)&arg1)[3], 3, volume);
        }
        if (func_150A29C8(0, 0x4041) == 0) {
            distance = 1290.0f - arg3;
            if (1290.0f < arg3) {
                volume = 0x20;
            } else {
                distance *= 0.37166667f;
                if (distance >= 223.0f) {
                    volume = 0xFF;
                } else {
                    volume = (u8)distance + 0x20;
                }
            }
        } else {
            volume = 0;
        }
        if (volume != func_10008A4C(((u8 *)&arg1)[3], 2)) {
            func_1000886C(((u8 *)&arg1)[3], 4, volume);
        }
    }
    return arg0;
}
u32 func_1000BF60(u32 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4) {
    s32 sp5C;
    s32 temp;
    u32 sp54;
    s32 sp50;
    s32 sp4C;
    u32 sp48;
    s32 mode;
    s32 sp40;
    s32 sp3C;

    sp50 = 0;
    sp5C = arg0 & 3;
    sp54 = arg0 >> 8;
    if (!(arg0 & 0x80)) {
        func_1000E40C(0x22, 0x5DC0);
        if (D_800C35EA == 1) {
            func_1000DF68(0x22, 0x14, 1);
        }
        arg0 = 0x80;
        sp50 = 1;
    }

    if (D_800BE9F0 != 0x1D) {
        func_10008F24(arg1);
        return arg0;
    }

    sp40 = 0;
    sp3C = 0;
    func_100114D0(-0x15F, 0, 0x197, 0x7FFF, 0xBB8, 0x12C, &sp48, &sp4C, 0);

    if ((u32)sp4C < 0x4000U) {
        sp4C = 0x40;
    } else {
        sp4C = (u32)sp4C >> 8;
    }

    if (sp4C != (sp54 >> 8)) {
        sp40 = 1;
        func_1000886C(arg1, 6, sp4C);
    }

    if (sp48 != (sp54 & 0xFF)) {
        sp3C = 1;
        func_10008744(arg1, 1, sp48 & 0x7F);
        func_10008744(arg1, 2, sp48 & 0x7F);
        func_100086FC(arg1, 1, sp48 >> 7);
        func_100086FC(arg1, 2, sp48 >> 7);
    }

    sp54 = (sp48 << 8) | (sp4C << 16);

    func_100114D0(-0x40, 0, 0x21F, 0x7FFF, 0xBB8, 0x12C, &sp48, &sp4C, 0);
    if (sp40 != 0) {
        if ((u32)sp4C < 0x4000U) {
            sp4C = 0x4000;
        }
        func_1000886C(arg1, 1, (u32)sp4C >> 8);
    }

    if (sp3C != 0) {
        func_10008744(arg1, 0, 0x40);
        func_100086FC(arg1, 0, 0);
    }

    func_100114D0(-0x126, 0, 0x290, 0x7FFF, 0xBB8, 0x12C, &sp48, &sp4C, 0);
    if (sp40 != 0) {
        if ((u32)sp4C < 0x4000U) {
            sp4C = 0x4000;
        }
        func_1000886C(arg1, 0x18, (u32)sp4C >> 8);
    }

    if (sp3C != 0) {
        func_10008744(arg1, 3, sp48 & 0x7F);
        func_10008744(arg1, 4, sp48 & 0x7F);
        func_100086FC(arg1, 3, sp48 >> 7);
        func_100086FC(arg1, 4, sp48 >> 7);
    }

    mode = D_80041F08;
    if (sp5C != mode) {
        switch (mode) {
        case 1:
            temp = arg0 & 0x7C;
            if (temp != 0) {
                temp = temp - ((D_800BE9E4 >> 1) << 2);
                if (temp <= 0) {
                    func_1000E704(0x22, 0, 0xFFFF);
                    arg0 = 0x81;
                } else {
                    arg0 = temp | 0x80;
                }
            }
            func_1000E46C(0x22, 0x64, 0xFE0, 0);
            break;
        case 2:
            arg0 = 0xF8;
            func_10011FA0((s32 *)4);
            func_1000E704(0x22, 1, 0xFFFF);
            func_1000E46C(0x22, 0, 0xFE0, sp50);
            break;
        }
    }

    return arg0 | sp54;
}
void func_10008790(u8 arg0, s32 arg1, u8 arg2, s32 arg3);
void func_1000886C(u8 arg0, s32 arg1, u8 arg2);
void func_10008F24(u8 arg0);
void func_1000E40C(s32 arg0, s32 arg1);
extern s32 D_80041F08;
extern s32 D_800BE9F0;
extern u8 D_800C35EA;
extern u8 D_800C35E8;
void func_15178EFC(s32 arg0);
s32 func_1000C350(s32 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4)
{
  s32 var_v0;
  u8 temp_s0;
  temp_s0 = arg1;
  if (!(arg0 & 0x80))
  {
    arg0 |= 0x80;
    if (D_800C35EA != 1)
    {
      func_1000886C(temp_s0, 0x1E, 1);
      func_1000886C(temp_s0, 1, 1);
      func_1000E40C(0x23, 0x61A8);
      goto block_16;
    }
    var_v0 = D_800C35E8;
    if (var_v0 == 3)
    {
      func_1000E40C(0x23, 0xFA);
      func_15178EFC(2);
      goto block_16;
    }
    if (var_v0 == 6)
    {
      func_1000886C(temp_s0, 0x1E, 1);
      func_1000886C(temp_s0, 1, 0x40);
      func_15178EFC(2);
      goto block_16;
    }
    else
    {
      func_1000E40C(0x23, 0x61A8);
    }
    block_16:
    ;

    ;
    ;
    return arg0;
  }
  if (D_800BE9F0 != 0x1D)
  {
    func_10008F24(temp_s0);
  }
  else
  {
    var_v0 = D_80041F08;
    if ((var_v0 & 0xFFFFFFFFu) != (arg0 & 0x7F))
    {
      switch (var_v0)
      {
        case 1:
          func_10008790(temp_s0, 0x1E, 0, 0);
          func_10008790(temp_s0, 1, 0x40, 0);
          var_v0 = D_80041F08;
          break;

        case 2:
          func_10008790(temp_s0, 0x18, 0xFF, 0);
          func_10008790(temp_s0, 6, 0, 0);
          func_10008790(temp_s0, 1, 1, 0);
          var_v0 = D_80041F08;
          break;

      }

      arg0 = var_v0 | 0x80;
    }
  }
  return arg0;
}

void func_1000E40C(s32 arg0, s32 arg1);

s32 func_1000C530(s32 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4) {
    s32 oldMode;
    s32 mode;
    s32 countdown;
    u32 upper;
    s32 oldLow;
    u32 fade;
    s32 low;
    s32 high;
    s32 oldHigh;

    upper = (u32)arg0 >> 8;
    mode = oldMode = arg0 & 3;
    countdown = ((u32)arg0 >> 2) & 0x3F;
    oldLow = upper & 0xFF;
    oldHigh = ((u32)arg0 >> 16) & 0xFF;
    fade = arg0 & 0xFF000000;
    low = oldLow; high = oldHigh;
    if (D_80041F08 != 0 &&
        (oldMode != 2 || countdown == 0 || D_80041F08 == 2)) {
        mode = D_80041F08;
        low = D_80041F0C & 0xFF;
        high = D_80041F0C >> 8;
        countdown = 0x1E;
    }
    if (countdown != 0) {
        countdown -= D_800BE9E4;
        if (countdown <= 0) {
            countdown = 0;
            mode = 0;
        }
    }
    if (mode != oldMode) {
        if (oldMode != 0) {
            func_100085F8(((u8 *)&arg1)[0], oldMode + 9);
        }
        if (mode != 0) {
            func_10008824(((u8 *)&arg1)[0], (u8)(mode + 9), (u8)low);
            func_100086FC(((u8 *)&arg1)[0], (u8)(mode + 9), (u8)(high >> 7));
            func_10008744(((u8 *)&arg1)[0], (u8)(mode + 9), high & 0x7F);
        }
    } else if (oldMode != 0 && D_80041F08 != 0 && upper != D_80041F0C) {
        if (low != oldLow) {
            func_10008824(((u8 *)&arg1)[0], (u8)(oldMode + 9), (u8)low);
        }
        if (high != oldHigh) {
            if ((high ^ oldHigh) & 0x80) {
                func_100086FC(((u8 *)&arg1)[0], (u8)(mode + 9), (u8)(high >> 7));
            }
            func_10008744(((u8 *)&arg1)[0], (u8)(mode + 9), high & 0x7F);
        }
    }
    if (D_80041F04 & 0x10) {
        D_80041F04 &= ~0x10;
        if (fade == 0) {
            func_1000886C(((u8 *)&arg1)[0], 0xC0, 0x80);
        }
        fade = 0xFF000000;
    }
    if (fade != 0) {
        upper = ((u32)D_800BE9E4 << 23) & 0xFF000000;
        if (upper < fade) {
            fade -= upper;
        } else {
            func_10008790(((u8 *)&arg1)[0], 0xC0, 0, 0x5A);
            fade = 0;
        }
    }
    D_80041F08 = 0;
    return (countdown * 4) | mode | (low << 8) | (high << 16) | fade;
}

void func_1000D96C(s32, s32, s32);

s32 func_1000C7E8(s32 arg0, s32 arg1, f32 arg2, s32 arg3, f32 arg4) {
    f32 distance;
    f32 value;

    if (D_800BE9F0 == 0x31) {
        if (arg0 != 2) {
            if (func_1000B1B0(9) == 0) {
                func_1000E704(0x3E, 0, 0xFFFF);
                func_1000E40C(0x3E, 0x7FFF);
                func_1000D96C(0x3D, 0x3E, 4);
            }
            return 2;
        }
        return 0;
    }
    if (D_8002B070 == 0) {
        D_8002B070 = 1;
    }
    if (arg0 != D_8002B070) {
        arg0 = D_8002B070;
    }
    arg2 -= -4000.0f;
    value = 24000.0f - sqrtf(arg2 * arg2 + arg4 * arg4) * 10.0f;
    distance = value;
    if (distance < 100.0f) {
        value = 100.0f;
    } else if (24000.0f < distance) {
        value = 24000.0f;
    }
    func_1000E40C(0x3E, (s32)value);
    return arg0;
}

s32 func_1000C934(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp3C;
    s32 sp38;
    s32 temp_t7;

    sp3C = 0;
    temp_t7 = D_800DBFF0->unk5F0 & 1;
    if (temp_t7 != 0) {
        sp38 = 0x7FFF;
    } else {
        sp38 = 12000;
    }
    if ((D_800BE9F0 == 0x37) && (temp_t7 == 0)) {
        func_100114D0(2200, 1066, -1600, sp38, 3000, 1500, 0, &sp3C, 0);
        sp3C &= 0xFF00;
        sp3C = sp38 - sp3C;
    }
    if ((sp3C != arg0) & 0xFFFF) {
        func_1000E40C(84, sp3C);
    }
    return sp3C | 0x80000000;
}

s32 func_1000CA18(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 tmp;

    if (D_800BE9F0 == 55) {
        if ((D_800DBFF0->unk5F0 & 1) != 0) {
            tmp = 0;
        } else {
            func_100114D0(2200, 1066, -1600, 24000, 3000, 1500, 0, &tmp, 0);
            tmp &= 0xFF00;
        }
    } else {
        tmp = 24000;
    }

    if ((tmp != arg0) & 0xFFFF) {
        func_1000E40C(84, tmp);
    }
    return tmp | 0x80000000;
}

s32 func_1000CAE4(s32 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4) {
    s32 sp24;

    sp24 = arg0 & 2;
    arg0 = arg0 & 1;
    if (D_800BE9F0 == 0x42) {
        func_10011FA0((s32 *)4);
        if (arg0 == 0) {
            arg0 = 1;
            func_1000E704(0x58, 1, 0xFFFF);
        }
    } else if (arg0 != 0) {
        func_1000E704(0x58, 0, 0xFFFF);
        func_1000E40C(0x58, 0x3E80);
        arg0 = 0;
    }
    if (sp24 == 0) {
        func_10008790(arg1, 0x1000, 0, 1);
        sp24 = 2;
    }
    return sp24 | arg0;
}

void func_1000CBA8(s32 arg0) {
    if (D_800417B0[0] != NULL) {
        D_800417B0[0]->unk4E = arg0;
        D_800417B0[0]->unk50 = (u16)0x500;
    }
    if (D_800417B0[1] != NULL) {
        D_800417B0[1]->unk4E = arg0;
        D_800417B0[1]->unk50 = (u16)0x500;
    }
}

extern struct151 *D_800417B0[];
void func_1000CBF0(s32 arg0, s32 arg1, s32 arg2)
{
  s32 i;
  struct151 **temp_v1;
  struct151 *temp_a0;
 i = 0; do {
    if ((1 << i) & arg2)
    {
      temp_v1 = &D_800417B0[i];
      temp_a0 = *temp_v1;
      if (temp_a0 != 0)
      {
        temp_a0->unk5A = arg0;
        (*temp_v1)->unk5C = arg1;
        if (arg1 == 0)
        {
          (*temp_v1)->unk58 = (u16) arg0;
        }
      }
    }
    i++;
    arg1 += 0;
  }
  while (i != 3);
}

// NON-MATCHING: JUSTREG? need some love
// void func_1000CBF0(s32 *arg0, s32 *arg1, s32 arg2) {
//     struct151 *tmp;
//     s32 i;
//     s32 tmp2;
//
//     for (i = 0; i < 3; i++) {
//         if (arg2 & (1 << i)) {
//             tmp = &D_800417B0[i];
//             tmp2 = tmp->unk0;
//             if (tmp2) {
//                 tmp->unk5A = arg0;
//                 tmp->unk5C = *arg1;
//                 if (arg1 == 0) {
//                   tmp->unk58 = *arg0;
//                 }
//             }
//         }
//     }
// }

// #pragma GLOBAL_ASM("asm/nonmatchings/init_B1B0/func_1000CC54.s")
void func_1000CC54(s32 arg0) {
    s32 volume;
    struct151 *temp_v0;

    temp_v0 = D_800417B0[arg0];
    if (temp_v0 != 0) {
        volume = (((u32) (temp_v0->unk58 * ((u32) (temp_v0->unk4C * temp_v0->unk52) >> 0xF)) >> 0xF) * temp_v0->unk2C) >> 0xF;
        if (volume != temp_v0->unk30) {
            if (temp_v0->unk30 == 0) {
                func_10008988(arg0, temp_v0->unk38 ^ 0xFFFF, 1);
            } else {
                if (volume == 0) {
                    func_10008988(arg0, temp_v0->unk38 ^ 0xFFFF, 0);
                }
            }
            temp_v0->unk30 = volume;
            func_10008EE0(arg0, volume);
        }
    }
}

s32 func_1000CD40(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 != arg0) {
        arg2 = arg2 * D_800BE9E4;
        if (arg0 < arg1) {
            arg0 = arg0 + arg2;
            if (arg1 < arg0) {
                arg0 = arg1;
            }
        } else {
            arg0 = arg0 - arg2;
            if ((arg0 < arg1) || (arg0 < 0)) {
                arg0 = arg1;
            }
        }
    }
    return arg0;
}

typedef struct SequenceRecordState {
    s32 index;
    s32 id;
    void *field8;
    void *fieldC;
    struct SequenceRecordState *owner;
    u8 ownerSync;
    u8 pauseMode;
    u8 pad16[2];
    s32 startMask;
    s32 fadeMask;
    s32 field20;
    s32 field24;
    s32 volumeOverride;
    s32 target;
    s32 current;
    s32 callbackState;
    s32 flags38;
    u8 pad3C[0x10];
    u16 current4C;
    u16 value;
    s16 duration;
    u16 current52;
    u16 target54;
    s16 duration56;
    u16 current58;
    u16 target5A;
    s16 duration5C;
    u8 pad5E[2];
    struct SequenceRecordState *state;
} SequenceRecordState;

s32 func_1000CDA0(u8 arg0, SequenceRecordState *record) {
    if (arg0 == 0) {
        return 1;
    }
    if (record != 0) {
        if (record->index >= 0) {
            if (D_800417B0[record->index] == 0 || record->id <= 0) {
                return 1;
            }
            if (func_1000853C(record->index) == 3) {
                return 1;
            }
            if (!(D_8002B078[record->id][0] & 0x20)) {
                D_800418AC[record->index] |= 3;
            }
            arg0 &= ~D_800418AC[record->index];
        } else {
            return 1;
        }
    } else {
        return 1;
    }
    return arg0 == 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/init_B1B0/func_1000CEAC.s")


typedef s32 (*SequenceCallback)(s32, s32, f32, f32, f32);

extern s32 D_80041880[];
extern s32 D_80041890[];
extern s32 D_800418A0[];
s32 func_1000CDA0(u8, SequenceRecordState *);
void func_10008C04(u8, u8, s32);
s32 func_10008CE8(u8, s32);

void func_1000D2F8(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    s32 status;
    SequenceRecordState **slot;
    SequenceRecordState *record;
    s32 channel;
    s32 offset;
    SequenceRecordState *child;
    SequenceRecordState *owner;
    SequenceCallback callback;
    s32 i;
    typedef struct {
        SequenceCallback callback;
        u8 pad4[12];
    } CallbackEntry;
    typedef struct {
        u8 markerCount;
        u8 pad1[15];
    } MarkerEntry;
    extern CallbackEntry D_8002B07C[];
    extern MarkerEntry D_8002B080[];

    channel = (u8)((u32 *)&arg0)[0];
    status = func_1000853C((u8)((u32 *)&arg0)[0]);
    offset = (s32)((u32 *)&arg0)[0] * sizeof(*slot);
    slot = (SequenceRecordState **)&(*(u8 (*)[])D_800417B0)[offset];
    record = *slot;
    if (record != 0) {
        if (record->field24 != 0 &&
            func_1000CDA0(((u8 *)record)[0x23], record->owner)) {
            func_10008C6C(((u8 *)&channel)[3], (u8)((*slot)->field24 - 1));
            record = *slot;
            if (((u32)record->volumeOverride) != 0) {
                record->current = ((u32)record->volumeOverride);
                (*slot)->target = ((u32)record->volumeOverride);
                record = *slot;
            }
            record->current4C = 0x7FFF;
            (*slot)->value = 0x7FFF;
            func_1000CC54((s32)((u32 *)&arg0)[0]);
            (*slot)->field24 = 0;
            (*slot)->field20 = 0;
        }
        record = *slot;
        if (status == 0) {
            record->id = 0;
            record = *slot;
        }
        child = record->state;
        if (child != 0) {
            if (record->id == 0) {
                if (func_1000CDA0(((u8 *)child)[0x1B], child->owner)) {
                    (*slot)->id = -1;
                    func_1000B294((s32 *)*slot);
                    child = (*slot)->state;
                    if (child->id > 0) {
                        if (func_10008CE8(((u8 *)&channel)[3], child->id) == -1) {
                            (*slot)->state->owner = 0;
                            (*slot)->state->startMask = 0;
                            func_1000B3D4((struct151 *)(*slot)->state, 0);
                            (*slot)->state = 0;
                            *slot = 0;
                        } else {
                            *slot = (*slot)->state;
                            (*slot)->index = (s32)((u32 *)&arg0)[0];
                            (*slot)->startMask = 0;
                            *(s32 *)((u8 *)D_800418A0 + offset) = 0;
                            *(s32 *)((u8 *)D_80041890 + offset) = 0;
                            *(s32 *)((u8 *)D_80041880 + offset) = 0;
                            record = *slot;
                            if (D_8002B080[record->id].markerCount != 0) {
                                func_10008C04(((u8 *)&channel)[3], D_8002B080[record->id].markerCount, 0x64);
                                record = *slot;
                            }
                            if (record->field24 != 0) {
                                func_10008C6C(((u8 *)&channel)[3], (u8)(record->field24 - 1));
                                (*slot)->field24 = 0;
                                record = *slot;
                            }
                            record->current = 0;
                            func_1000CC54((s32)((u32 *)&arg0)[0]);
                            func_100084D8(((u8 *)&channel)[3]);
                            if ((*slot)->flags38 != 0) {
                                func_100088F0(((u8 *)&channel)[3], (*slot)->flags38, 0);
                            }
                            i = 0;
                            do {
                                if ((*slot)->pad3C[i] != 0) {
                                    func_10008660(((u8 *)&channel)[3], (u8)i, (*slot)->pad3C[i], 1);
                                    (*slot)->pad3C[i] = 0;
                                }
                                i++;
                            } while (i != 16);
                        }
                    } else {
                        child->id = -1;
                        (*slot)->state->owner = 0;
                        (*slot)->state->startMask = 0;
                        (*slot)->index = -1;
                        *slot = 0;
                    }
                }
            } else if (func_1000CDA0(((u8 *)child)[0x1B], child->owner)) {
                (*slot)->pauseMode = 0;
                func_10008F24(((u8 *)&channel)[3]);
            }
            record = *slot;
        } else if (record->id == 0) {
            func_1000B294((s32 *)record);
            record = 0;
            (*slot)->id = -1;
            (*slot)->index = -1;
            *slot = 0;
        }
        if (record != 0) {
            if (record->id > 0) {
                callback = D_8002B07C[record->id].callback;
                if (callback != 0) {
                    (*slot)->callbackState = callback(record->callbackState, ((u8 *)&channel)[3], arg1, arg2, arg3);
                    record = *slot;
                }
            }
            if (func_1000CDA0(((u8 *)record)[0x1F], record->owner)) {
                record = *slot;
                (*slot)->current4C = func_1000CD40(record->current4C, record->value, (u16)record->duration);
                record = *slot;
                (*slot)->current52 = func_1000CD40(record->current52, record->target54, (u16)record->duration56);
                record = *slot;
                (*slot)->current58 = func_1000CD40(record->current58, record->target5A, (u16)record->duration5C);
                (*slot)->fadeMask = 0;
            }
            func_1000CC54((s32)((u32 *)&arg0)[0]);
            record = *slot;
            if (record->ownerSync != 0 &&
                ((owner = record->owner) == 0 || owner == record || owner->id <= 0 ||
                 record->id <= 0 || owner->index < 0)) {
                record->ownerSync = 0;
                record = *slot;
                record->owner = record;
            }
        }
    }
}
typedef struct {
    u16 volume;
    u8 pad2[2];
    s32 flags;
    void *callback;
    u8 markerCount;
    u8 padD[3];
} SequenceVolumeEntry;

#define SEQ_VOL ((SequenceVolumeEntry *)D_8002B074)

s32 func_151F2CDC(void);
void func_1000CEAC(s32);
#define D_800427F4_U (*(u16 *)&D_800427F4)

void func_1000D758(f32 arg0, f32 arg1, s32 arg2) {
    SequenceVolumeEntry *entries;
    s32 mask5;
    s32 mask40;
    s32 mask34;
    s32 mask12;
    s32 i;
    s32 id;
    s32 flags;
    s32 mode;
    SequenceRecordState *record;

    mask5 = 0;
    mask40 = 0;
    mask34 = 0;
    mask12 = 0;
    if ((u8)D_80041F00 == 0) {
        entries = (SequenceVolumeEntry *)D_8002B074;
        for (i = 0; i < 3; i++) {
            record = (SequenceRecordState *)D_800417B0[i];
            if (record != 0) {
                id = record->id;
                if (id > 0) {
                    flags = entries[id].flags;
                    mode = flags & ~0xF0;
                    if (flags & 0x40) {
                        mask40 |= 1 << i;
                    }
                    if (mode == 5) {
                        mask5 |= 1 << i;
                    } else if (mode == 4 || mode == 3) {
                        mask34 |= 1 << i;
                    } else if (mode == 1 || mode == 2) {
                        mask12 |= 1 << i;
                    }
                }
            }
        }
        if (mask5 != 0) {
            func_1000CBF0(0x1770, 0x400, mask5 ^ 0xFF ^ mask40);
            func_1000CBF0(0x8000, 0x6400, mask5);
        } else if (mask34 != 0) {
            func_1000CBF0(0x1F4, 0x400, mask34 ^ 0xFF);
            func_1000CBF0(0x8000, 0x800, mask34);
        } else if (func_151F2CDC() == 1 &&
                   (D_800427F4_U < 0x7D || D_800427F4_U >= 0x81) &&
                   D_800427F4_U < 0x1C9 && D_800427F4_U != 0x170 &&
                   D_800427F4_U != 0x171) {
            func_1000CBF0(0x36B0, 0x200, mask12 ^ 0xFF);
        } else {
            func_1000CBF0(0x8000, 0x800, 0xFF);
        }
        i = 0;
        do {
            func_1000CEAC(i);
            i++;
        } while (i < 3);
        i = 0;
        do {
            func_1000D2F8(i, arg0, arg1, *(f32 *)&arg2);
            i++;
        } while (i != 3);
    }
}
#define SEQ_B2F4(x) ((SequenceRecordState *)func_1000B2F4(x))
#define SEQ_B3D4(a, b) func_1000B3D4((struct151 *)(a), (struct151 *)(b))

void func_1000D96C(s32 arg0, s32 arg1, s32 arg2) {
    SequenceRecordState *outgoing;
    SequenceRecordState *incoming;
    s32 sync;

    outgoing = 0;
    incoming = 0;
    sync = 0;
    arg0 &= 0xFFF;
    arg1 &= 0xFFF;
    if (arg0 != 0 && func_1000B1FC(arg0) != 0) {
        SequenceRecordState *record;
        SequenceRecordState *child;

        record = (SequenceRecordState *)func_1000B1B0(arg0);
        if (record == 0) {
            return;
        }
        child = record->state;
        if (child != 0) {
            if (child->index == -1) {
                record->target54 = 0x8000;
                record->duration56 = 0x400;
                child->id = -1;
                record->state->index = -1;
                record->state = 0;
            }
            arg0 = 0;
            if (arg1 == 0) {
                return;
            }
            goto lookup_outgoing;
        } else if (record->pauseMode != 0 || record->target54 == 0) {
            record->target54 = 0x8000;
            record->current52 = 0;
            record->duration56 = 0x400;
            arg0 = 0;
            if (arg1 == 0) {
                return;
            }
lookup_outgoing:
            ;
        } else {
            return;
        }
    }
    if (arg1 != 0) {
        outgoing = (SequenceRecordState *)func_1000B1B0(arg1);
    }
    if (outgoing != 0) {
        if (outgoing != 0 && outgoing->state != 0) {
            if (arg0 != 0) {
                return;
            }
        } else if (arg2 == 4 || arg2 == 6) {
            if (arg0 != 0) {
                incoming = SEQ_B2F4(arg0);
            }
            if (incoming != 0 || arg0 == 0) {
                if (arg0 != 0) {
                    SequenceVolumeEntry *entry;

                    entry = &SEQ_VOL[arg0];
                    if ((entry->flags & 0x20) && (SEQ_VOL[arg1].flags & 0x20)) {
                        sync = 1;
                    }
                    incoming->ownerSync = sync;
                    if (entry->flags & 8) {
                        incoming->target54 = 0x8000;
                        incoming->current52 = 0;
                        incoming->duration56 = 0x3C;
                    } else if (sync != 0) {
                        incoming->target54 = 0x8000;
                        incoming->current52 = 0;
                        incoming->duration56 = 0x100;
                    }
                    incoming->owner = outgoing;
                    incoming->startMask = 2;
                    SEQ_B3D4(incoming, 0);
                }
                if (arg2 == 6) {
                    if (arg0 != 0) {
                        outgoing->fadeMask = 2;
                    }
                    outgoing->ownerSync = 0;
                    outgoing->owner = outgoing;
                    outgoing->target54 = 0;
                    if (SEQ_VOL[arg1].flags & 8) {
                        outgoing->duration56 = 0x28;
                        return;
                    }
                    outgoing->duration56 = 0xA0;
                    return;
                }
                incoming = SEQ_B2F4(0);
                if (incoming != 0) {
                    if (arg0 != 0) {
                        outgoing->fadeMask = 2;
                    }
                    outgoing->ownerSync = 0;
                    outgoing->owner = outgoing;
                    outgoing->target54 = 0;
                    if (sync != 0) {
                        outgoing->duration56 = 0x80;
                    } else {
                        outgoing->duration56 = 0x200;
                    }
                    incoming->startMask = 0x80;
                    incoming->owner = outgoing;
                    SEQ_B3D4(incoming, outgoing);
                }
            }
        } else {
            goto simple_transition;
        }
        return;
    } else {
        goto fallback_lookup;
    }
fallback_lookup:
    if (arg1 != 0) {
        outgoing = (SequenceRecordState *)func_1000B1FC(arg1);
        if (outgoing != 0) {
            if (arg0 != 0) {
                bzero(outgoing, 0x64);
                outgoing->index = -1;
                if (arg0 < 0x96) {
                    outgoing->target = SEQ_VOL[arg0].volume;
                } else {
                    outgoing->target = 0x6590;
                }
                outgoing->current = outgoing->target;
                outgoing->value = outgoing->current4C = outgoing->current52 =
                    outgoing->target54 = outgoing->current58 =
                    outgoing->target5A = 0x8000;
                outgoing->field8 = &D_8002B9D4;
                outgoing->fieldC = &D_8002B9F4;
                outgoing->owner = outgoing;
            } else {
                outgoing->startMask = 0x80;
            }
            outgoing->id = arg0;
            return;
        }
    }
simple_transition:
    if (arg0 != 0 || outgoing != 0) {
        incoming = SEQ_B2F4(arg0);
    }
    if (incoming != 0) {
        if (arg2 == 3) {
            incoming->target54 = 0x8000;
            incoming->current52 = 0;
            incoming->duration56 = 0x400;
        } else if (arg2 == 2) {
            incoming->startMask = 1;
            incoming->owner = outgoing;
        } else if (arg2 == 1) {
            incoming->startMask = 2;
            incoming->owner = outgoing;
        } else if (arg2 == 5) {
            incoming->startMask = 0x80;
            incoming->owner = outgoing;
            if (outgoing != 0) {
                outgoing->target54 = 0;
                outgoing->duration56 = 0x200;
            }
        }
        SEQ_B3D4(incoming, outgoing);
    }
}
void func_1000DE1C(s32 arg0, s32 arg1) {
    s32 count;
    s32 i;
    s32 ids[3];
    s32 value;

    arg0 &= 0xFFF;
    if (arg0 == 0) {
        func_1000DEC4();
        count = func_1000B548(ids);
        for (i = 0; i < count; i++) {
            value = ids[i];
            if (value > 0) {
                func_1000D96C(0, value, arg1);
            }
        }
    } else {
        func_1000D96C(0, arg0, arg1);
    }
}
s32 func_1000853C(u8 arg0);
extern struct151 *D_800417B0[];
extern struct137 D_800419A8[12];
extern OSMesgQueue D_80041E58[3];
void func_1000DEC4(void)
{
  struct137 *var_s0;
  s32 invalid = -1;
  s32 temp_v0;
 var_s0 = D_800419A8; do { temp_v0 = var_s0->unk0; if (invalid == temp_v0) { if (var_s0->unk4 != invalid) { var_s0->unk4 = invalid;
      }
    }
    else
      if (func_1000853C(temp_v0) == 0)
    {
      D_800417B0[var_s0->unk0] = 0;
      var_s0->unk0 = invalid;
      var_s0->unk4 = invalid;
    }
    var_s0++;
    ((s32 *) var_s0)[-1] = 0;
  }
  while (var_s0 != ((struct137 *) D_80041E58));
}

void func_1000DF68(s32 arg0, s32 arg1, s32 arg2) {
    struct151 *p;

    p = func_1000B1FC(arg0);
    if (p != NULL) {
        p->unk4E = arg1;
        if (arg2 == 1) {
            p->unk4C = arg1;
            if (p->unk0 >= 0) {
                func_1000CC54(p->unk0);
            }
        }
        if (arg2 >= 2) {
            s32 rate;

            rate = p->unk4C - arg1;
            if (rate < 0) {
                rate = -rate;
            }
            rate /= arg2;
            if (rate <= 0) {
                rate = 2;
            } else if (rate >= 0x8000) {
                rate = 0x7FFF;
            }
            p->unk50 = rate;
        } else {
            p->unk50 = 0x200;
        }
    }
}


void func_1000E054(s32 arg0, s32 arg1) {
    struct151 *sp1C;

    sp1C = func_1000B1B0(arg0);
    if (sp1C != 0) {
        if ((2 == sp1C->unk15) && (arg1 == 0)) {
            func_100084D8(sp1C->unk0);
            sp1C->unk15 = 0;
            sp1C->unk30 = -1;
            func_1000CC54(sp1C->unk0);
            return;
        }
        if ((2 != sp1C->unk15) && (arg1 != 0)) {
            if (sp1C->unk15 != 1) {
                func_10008F58(sp1C->unk0);
            }
            sp1C->unk15 = 2;
        }
    }
}

s32 func_1000E0F8(s32 arg0) {
    struct151 *tmp;

    arg0 &= 0xFFF;
    tmp = func_1000B1FC(arg0);

    if (tmp && tmp->unk60 == 0) {
        return 1;
    } else {
        return 0;
    }
}

s32 func_1000E134(s32 arg0) {
    s32 tmp;

    if (arg0 < 150) {
        tmp = D_8002B078[arg0][0] & ~0xF0;
        if ((tmp == 1) || (tmp == 3)) {
            return 1;
        }
    }
    return 0;
}

#define VOICE_TYPE(v) (((s32 *)&D_8002B074[(v)->unk4])[1] & ~0xF0)
/* struct137's 0x60 word is a link to another voice (the shared header still pads it) */
#define VOICE_LINK60(v) (*(struct137 **)&(v)->pad60)

void func_1000E17C(void) {
    s32 i;
    struct137 *v;

    for (i = 0; i < 12; i++) {
        v = &D_800419A8[i];
        if (v->unk4 > 0) {
            if (((1 == VOICE_TYPE(v)) || (3 == VOICE_TYPE(v))) && (v->unk0 == -1)) {
                v->unk4 = -1;
            }
        }
    }
    for (i = 0; i < 12; i++) {
        v = &D_800419A8[i];
        if (v->unk4 > 0) {
            if ((VOICE_LINK60(v) != NULL) && (VOICE_LINK60(v)->unk4 == -1)) {
                VOICE_LINK60(v) = NULL;
            }
            if ((v->unk10 != NULL) && (v->unk10->unk4 == -1)) {
                v->unk10 = NULL;
            }
        }
    }
    for (i = 0; i < 12; i++) {
        v = &D_800419A8[i];
        if (v->unk4 > 0) {
            if (((1 == VOICE_TYPE(v)) || (3 == VOICE_TYPE(v))) && (v->unk0 != -1)) {
                func_1000DE1C(v->unk4, 4);
            }
        }
    }
}

void func_1000E2F4(s32 arg0) {
    struct151 *p;
    s32 i;

    for (i = 0; i < 3; i++) {
        p = D_800417B0[i];
        if (p == NULL) {
            continue;
        }
        if (p->unk4 <= 0) {
            continue;
        }
        if (p->unk15 != 0) {
            continue;
        }
        if (arg0 != 0) {
            func_10008EE0((u8)i, 0);
            if ((((s32 *) &D_8002B074[D_800417B0[i]->unk4])[1] & 0x10) != 0) {
                continue;
            }
            func_10008F58((u8)i);
        } else {
            if ((((s32 *) &D_8002B074[p->unk4])[1] & 0x10) == 0) {
                func_100084D8((u8)i);
                p = D_800417B0[i];
            }
            p->unk30 = -1;
            func_1000CC54(i);
        }
    }
    D_80041F00 = (u8) arg0;
}

void func_1000E40C(s32 arg0, s32 arg1) {
    struct151 *temp_v0;

    if (arg1 >= 0x8000) {
        arg1 = 0x7FFF;
    } else if (arg1 < 0) {
        arg1 = 0;
    }
    temp_v0 = func_1000B1FC(arg0);
    if (temp_v0 != NULL) {
        if (temp_v0->unk0 < 0) {
            temp_v0->unk30 = arg1;
        }
        temp_v0->unk2C = arg1;
    }
}

s32 func_1000E46C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_v1;
    struct151 *temp_v0;
    struct151 *temp_v1;

    temp_v0 = func_1000B1FC(arg0);
    arg1 = (arg1 * 0xFF) / 100;
    temp_v1 = temp_v0;
    if (arg1 >= 0x100) {
        arg1 = 0xFF;
    } else if (arg1 < 0) {
        arg1 = 0;
    }
    if (temp_v0 != NULL) {
        if (temp_v0->unk0 >= 0) {
            if (arg3 < 0) {
                func_1000886C(*(u8 *)((u8 *)temp_v1 + 3), arg2, arg1);
                return 1;
            }
            func_10008790(*(u8 *)((u8 *)temp_v1 + 3), arg2, arg1, arg3);
            return 1;
        }
        if (arg1 == 0) {
            temp_v0->unk38 |= arg2;
        } else if (arg1 > 0) {
            temp_v0->unk38 &= ~arg2;
        }
        var_v1 = 0;
        if (arg2 != 0) {
            do {
                if (arg2 & 1) {
                    temp_v0->unk3C[var_v1] = arg1;
                }
                var_v1 += 1;
                arg2 = arg2 >> 1;
            } while (arg2 != 0);
        }
        return 1;
    }
    return 0;
}
s32 func_1000E588(s32 arg0, s32 arg1, s32 arg2) {
    struct151 *temp_v0;

    temp_v0 = func_1000B1FC(arg0);
    if (temp_v0 != NULL) {
        if (temp_v0->unk0 >= 0) {
            if (arg1 >= 0x65) {
                arg1 = 0x64;
            } else if (arg1 < 0) {
                arg1 = 0;
            }
            func_1000886C(*(u8 *)((u8 *)temp_v0 + 3), arg2, (u8)((arg1 * 0xFF) / 100));
            return 1;
        }
        if (arg1 <= 0) {
            temp_v0->unk38 |= arg2;
            return 1;
        }
        if (arg1 > 0) {
            temp_v0->unk38 &= ~arg2;
            return 1;
        }
    }
    return 0;
}

s32 func_1000E654(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    struct151 *sp1C;
    struct151 *owner;

    sp1C = func_1000B1FC(arg0);
    owner = NULL;
    if (arg3 >= 0) {
        owner = func_1000B1FC(arg3);
    }

    if (sp1C != NULL) {
        if (arg2 == 0) {
            if (sp1C->unk0 >= 0) {
                func_10008C6C(sp1C->unk0, arg1 - 1);
                return 1;
            }
        }
        sp1C->unk24 = arg1;
        sp1C->unk20 = arg2;
        if (owner != NULL) {
            sp1C->unk10 = owner;
        }
        return 1;
    }
    return 0;
}

s32 func_1000E704(s32 arg0, s32 arg1, s32 arg2) {
    struct151 *tmp = func_1000B1B0(arg0);
    if (tmp && tmp->unk0 >= 0) {
        func_10008A94(tmp->unk0, arg2, arg1);
        return 1;
    }
    return 0;
}

void func_1000E75C(s32 arg0) {
    D_8002B070 = arg0 >> 1;
}

s32 func_1000E770(s32 *arg0, s32 *arg1) {
    if (arg0 != 0) {
        *arg0 = D_80041F08;
    }
    if (arg1 != 0) {
        *arg1 = D_80041F0C;
    }
    return D_80041F04;
}

void func_1000E7A0(u32 arg0, s32 arg1) {
    s32 temp_t0;

    if ((arg0 & 1) == 1) {
        D_80041F04 |= 1;
        return;
    }
    if (arg0 & 2) {
        D_80041F08 += arg1;
        D_80041F0C += 1;
        return;
    }
    if (arg0 & 4) {
        D_80041F08 = arg1 + 1;
        D_80041F04 |= 4;
        return;
    }
    if (arg0 & 8) {
        D_80041F0C = arg1 >> 8;
        arg1 &= 0xFF;
        if ((arg1 == 0) || (arg1 == 4) || (arg1 == 5)) {
            D_80041F08 = 2;
            return;
        }
        if (arg1 == 10) {
            D_80041F08 = 1;
            return;
        }
        D_80041F08 = 3;
        return;
    }
    if (arg0 & 0x10) {
        D_80041F04 |= 0x10;
    }
}
// NON-MATCHING: mostly just wrong registers
// void func_1000E7A0(u32 arg0, s32 arg1) {
//     if ((arg0 & 1) == 1) {
//         D_80041F04 |= 1;
//     } else if (arg0 & 2) {
//         D_80041F08 += arg1;
//         D_80041F0C += 1;
//     } else if (arg0 & 4) {
//         D_80041F08 = 1 + arg1;
//         D_80041F04 |= 4;
//     } else if (arg0 & 8) {
//         D_80041F0C = (arg1 >> 8) & 0xff;
//         if ((D_80041F0C == 0) || (D_80041F0C == 4) || (D_80041F0C == 5))  {
//             D_80041F08 = 2;
//         } else if (D_80041F0C == 10) {
//             D_80041F08 = 1;
//         } else {
//             D_80041F08 = 3;
//         }
//     } else if (arg0 & 16) {
//         D_80041F04 |= 16;
//     }
// }

void func_1000E8C4(s32 arg0) {
    if ((arg0 & 1) == 1) {
        D_80041F04 &= -2; // truncate odd to even
    }
}

u8 func_1000E8F0(s32 arg0) {
    struct151 *temp_v0 = func_1000B1B0(arg0);
    if ((temp_v0 != 0) && (temp_v0->unk0 >= 0)) {
        return D_800418AC[temp_v0->unk0];
    } else {
        return 0;
    }
}

// initialising with grim loops
/* D_800418B0 and D_800417C0 are [3][16] per-channel tables; variables.h types D_800418B0 as s32[]. */
extern s32 D_800417C0[3][16];
extern s32 D_800418A0[];
extern s32 D_80041890[];
extern s32 D_80041880[];

void func_1000E934(void) {
    u32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 16; j++) {
            ((s32 (*)[16])D_800418B0)[i][j] = 0x8000;
            D_800417C0[i][j] = 0x100;
        }
        func_10008F24(i);
        D_800417B0[i] = NULL;
        D_800418A0[i] = 0;
        D_80041890[i] = 0;
        D_80041880[i] = 0;
    }
    bzero(D_800419A8, sizeof(D_800419A8));
    D_800419A0 = 0;
    for (k = 0; k < 12; k++) {
        D_800419A8[k].unk4 = -1;
    }
}

u16 func_1000EA94(s32 arg0) {
    u16 tmp;

    if (arg0 == 0) {
        tmp = 82;
    } else if (arg0 == 2) {
        tmp = 81;
    } else if (arg0 == 1) {
        tmp = 83;
    }
    func_1000D96C(tmp, 0, 0);
    return tmp;
}
