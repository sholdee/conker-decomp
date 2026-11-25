#include <ultra64.h>
/* functions.h declares func_150ADA20 as returning u8, but it returns a full word
 * (the callers here keep all 32 bits), so this file declares it itself. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

void func_151AE984(f32 *, f32, f32, f32, s32, u8);
void func_151AEAB4(f32 *, s32, u8);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);

/* Spawns a random spark or debris particle at (x, y, z); arg6[0xC] is passed through. */
void func_151AE890(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, u8 *arg6) {
    f32 pos[3];
    f32 roll;

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    roll = func_150ADA68();
    if (roll < 0.15000000596f) {
        return;
    }
    if (roll < 0.25f) {
        s32 flag;

        roll = func_150ADA68();
        flag = (roll > 0.0f) ? 1 : 0;
        func_151AEAB4(pos, flag, arg6[0xC]);
    } else {
        s32 flag;

        roll = func_150ADA68();
        flag = (roll > 0.8050000668f) ? 1 : 0;
        func_151AE984(pos, dirX, dirY, dirZ, flag, arg6[0xC]);
    }
}

void func_151AE984(f32 *arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4,
                   u8 arg5) {
    f32 scale;
    f32 sp58[3];
    f32 shade;
    u32 sp50;
    u32 sp4C;
    f32 sp48;

    scale = 0.10300001f;
    sp58[0] = -arg1 * scale;
    sp58[1] = -arg2 * scale;
    sp58[2] = -arg3 * scale;
    sp48 = func_150ADA68();
    sp4C = func_150ADA20();
    sp50 = func_150ADA20();
    shade = (func_150ADA68() * 219.0f) + 77.0f;
    func_151D9014(arg0, sp58, 0, (sp48 * 0.60200006f) + -1.0450001f,
                  (sp4C % 31U) + 0x3C, (sp50 % 101U) + 0x9B,
                  shade, (u8)arg4,
                  1.0f, 1.0f, 0, 0, 1, 0, (u8)arg5, 1);
}

f32 func_150ADA68(void);
f32 func_151423D8(u8);
extern f32 D_800A9D98;
extern f32 D_800A9D9C;
extern f32 D_800A9DA0;
extern f32 D_800A9DA4;
void func_151AEAB4(f32 *, s32, u8);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32, s32, u8, s32, s16, s16, s32, u8, s32);
void func_151AEAB4(f32 *arg0, s32 arg1, u8 arg2)
{
  s32 pad94;
  struct 
  {
    u32 sp60;
    u32 sp64;
    f32 sp68;
    u8 pad6C[4];
    f32 sp70;
    f32 sp74;
    f32 sp78;
    f32 sp7C;
    u8 pad80[6];
    u8 sp86;
    u8 sp87;
    f32 sp88[3];
  } sp;
  f32 temp_f12;
  f32 temp_f2;
  sp.sp87 = func_150ADA20();
  sp.sp86 = (u8) (func_150ADA20() & 0x7F) - 0x3F;
  sp.sp7C = func_151423D8(sp.sp87);
  sp.sp78 = func_151423D8((u8) (sp.sp87 - 0x40));
  sp.sp74 = func_151423D8(sp.sp86);
  sp.sp70 = func_151423D8((u8) (sp.sp86 - 0x40));
  temp_f2 = ((func_150ADA68() * 80.0f) + 20.0f) * D_800A9D98;
  temp_f12 = temp_f2 * sp.sp74;
  sp.sp88[0] = temp_f12 * sp.sp78;
  sp.sp88[1] = (-temp_f2) * sp.sp70;
  sp.sp88[2] = temp_f12 * sp.sp7C;
  goto next;
  next:
  *((f32 *) (((u8 *) (&sp)) - 4)) = func_150ADA68();

  sp.sp60 = func_150ADA20();
  sp.sp64 = func_150ADA20();
  sp.sp68 = func_150ADA68();
  func_151DA6F8(arg0, sp.sp88, ((*((f32 *) (((u8 *) (&sp)) - 4))) * D_800A9D9C) + D_800A9DA0, (s16) ((sp.sp60 % 41U) + 0x3C), (sp.sp64 % 101U) + 0x9B, (sp.sp68 * 4.0f) + D_800A9DA4, (func_150ADA20() % 5U) + 3, ((u8 *) (&arg1))[3], 1.0f, 1.0f, 0, 0, 0, 0x10, 0xF, 0, arg2, 1);
}
