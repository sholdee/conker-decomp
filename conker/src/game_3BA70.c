#include <ultra64.h>

#include "functions.h"
#include "variables.h"


extern void bzero(void *, int);
void func_15008A10(void);
void func_15012770(void);
extern u8 D_80088980;
extern u8 D_80088B40;
extern struct14 *D_800BE4F0;
extern u8 D_800BE500[5];
extern u8 D_800D9890;
extern u8 D_800D9920;
extern u8 D_800D9921;
extern u8 D_800D9928;
extern u8 D_800D9929;
extern u8 D_800D9938;
extern u8 D_800D9939;
extern u8 D_800D992A[];
extern u8 D_800D993A[];
extern struct134 *D_800D9AA0[];
extern u8 D_800E0950;
extern s32 D_800E0964;
void func_15012470(void);
extern u8 D_800D9946[];
extern u8 D_800D9950[];
extern s32 *D_800D98D0[];
extern s32 *D_800D9894;
extern u8 *D_80088870;
void func_1500E5C0(void)
{
  s32 i;
  s32 j;
 do { func_15012470(); func_15008A10(); func_15012770(); i = (s32) (&D_800E0950); for (;;) { i += 4; ((u8 *) i)[-3] = 0; ((u8 *) i)[-2] = 0; ((u8 *) i)[-1] = 0; ((u8 *) i)[-4] = 0; if (((s32) (&D_800E0964)) != i) { continue; } break; } D_800D9921 = 0; D_800D9920 = 0; D_800D9928 = 0; D_800D9938 = 0; D_800D9929 = 0; } while (0);
  for (D_800D9939 = 0, i = 0, j = 0;;)
  {
    D_800D992A[j + 1] = 0;
    D_800D993A[j + 1] = 0;
    D_800D992A[j + 2] = 0;
    D_800D993A[j + 2] = 0;
    D_800D992A[j + 3] = 0;
    D_800D993A[j + 3] = 0;
    j += 4;
    D_800D992A[j - 4] = 0;
    D_800D993A[j - 4] = 0;
    if ((&D_800D9946[0]) != (&D_800D993A[j]))
    {
      continue;
    }
    break;
  }

  {
    s32 **var_v0_3 = D_800D98D0;
    D_800D9890 = 0;
    D_800D9894 = 0;
    var_v0_3[0] = 0;
    var_v0_3[1] = 0;
    var_v0_3[2] = 0;
    var_v0_3[3] = 0;
  }
  D_80088870 = 0;
  bzero(&D_800BE500, 5);
  D_800D9950[0] = (D_800D9950[1] = (D_800D9950[2] = 0));
  D_80088980 = 0;
  D_800D9AA0[0] = 0;
  D_800D9AA0[1] = 0;
  D_800D9AA0[2] = 0;
  D_800BE4F0 = 0;
  D_80088B40 = 0;
}


void func_1500E70C(s32 arg0) {
    if (arg0 == 0x2B) {
        func_15011C70();
    }
}

extern s8 D_800DCA20;
extern f32 D_800DCA24;
extern s8 D_800DCA28;
extern f32 D_800DCA2C;

void func_1500E738(void) {
    switch (D_80082FA0) {
    case 0:
        *(u8 *)&D_800DCA20 = 0;
        D_800DCA24 = 1.0f;
        break;
    case 1:
        *(u8 *)&D_800DCA20 = 1;
        D_800DCA24 = 0.5f;
        break;
    case 2:
    case 3:
        *(u8 *)&D_800DCA20 = 2;
        D_800DCA24 = 0.25f;
        break;
    }
    switch (D_8008FD8C) {
    case 2:
        *(u8 *)&D_800DCA28 = 1;
        D_800DCA2C = 0.5f;
        return;
    case 3:
    case 4:
        *(u8 *)&D_800DCA28 = 2;
        D_800DCA2C = 0.25f;
        return;
    case 5:
    case 6:
    case 7:
    case 8:
        *(u8 *)&D_800DCA28 = 3;
        D_800DCA2C = 0.125f;
        return;
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        *(u8 *)&D_800DCA28 = 4;
        D_800DCA2C = 0.0625f;
        return;
    case 1:
    default:
        *(u8 *)&D_800DCA28 = 0;
        D_800DCA2C = 1.0f;
        return;
    }
}

void func_1500E890(void) {
    func_15008E00();
    func_15008E10(0);
    func_15008E10(1);
}

void func_1500E8C0(void) {

    func_15195AA8(D_800B0E00[0], D_800902E8, 0, -1, 0, 0, 0, -6);
    func_15195AA8(D_800B0E04, D_800902E8, 0, -1, 0, 1, 0, -6);
    {
    struct145 tmp;
    *(f32 *)&tmp.unk28 = 0.2142857164f;
    tmp.unk30 = 5.0f;
    tmp.unk34 = 6.0f;
    tmp.unk20 = 8.0f;
    tmp.unk24 = 7.0f;
    tmp.unk2C = 0.0f;
    tmp.unk48 = 3;
    tmp.unk4C = 2;
    tmp.unk0 = 0x34;
    tmp.unk2 = 0x12;
    tmp.unk4 = -0x28;
    tmp.unk6 = 0xF;
    tmp.unk38 = 0x9B;
    tmp.unk3A = 0x64;
    tmp.unk44 = 0x29;
    tmp.unk46 = 0x29;
    tmp.unk10 = 400.0f;
    tmp.unk14 = 0.0f;
    tmp.unk18 = 0.0f;
    tmp.unk3C = 0.6000000238f;
    tmp.unk40 = 0.3999999762f;
    tmp.unk8 = 800.0f;
    tmp.unkC = -3000.0f;
    tmp.unk1C = 900.0f - tmp.unk10;
    func_15189900(&tmp, 1);
    }
    func_1000FA64(1567, (s16)((D_800BE9F0 == 6) ? 52 : 7), 0, 0, 12000, 1000, 400, (s32)func_1000EF40, 0, 0, 72, 0);
}
void func_1500EAA0(void) {
    func_15195AA8(D_800B0E00[0], D_80090320, 0, -1, 0, 0, 0, -8);
    func_15195AA8(D_800B0E00[1], D_80090320, 0, -1, 0, 1, 0, -8);
}
