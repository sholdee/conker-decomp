#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern u8 D_80084930;
extern u8 D_800848D0[];
extern u8 D_80085930;
extern u8 D_80085931;
extern u8 D_80085932;
extern u8 D_80085933[];

#define WGFX(pkt, a, b)             \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

Gfx *func_150412C0(Gfx *gfx) {
    WGFX(gfx++, 0xD7000002, 0xFFFFFFFF);
    WGFX(gfx++, 0xE7000000, 0x00000000);
    WGFX(gfx++, 0xFCFFFFFF, 0xFFFCF279);
    WGFX(gfx++, 0xEF002C0F, 0x0055204C);
    WGFX(gfx++, 0xFD90003F, &D_80084930);
    WGFX(gfx++, 0xFD900000, &D_80084930);
    WGFX(gfx++, 0xF5900000, 0x07000000);
    WGFX(gfx++, 0xE6000000, 0x00000000);
    WGFX(gfx++, 0xF3000000, 0x077FF200);
    WGFX(gfx++, 0xE7000000, 0x00000000);
    WGFX(gfx++, 0xF5800800, 0x00000000);
    WGFX(gfx++, 0xF2000000, 0x000FC1FC);
    return gfx;
}

s32 func_15041480(u8 arg0);
Gfx *func_15041508(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);


Gfx *func_150413FC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3) {
    s32 i;

    i = 0;
    while (arg3[i] != 0) {
        gfx = func_15041508(gfx, arg1 + i * 8, arg2, func_15041480(arg3[i]));
        i++;
    }
    return gfx;
}

s32 func_15041480(u8 arg0) {
    s32 i;

    for (i = 0; ; ) {
        if (arg0 == D_800848D0[i]) {
            return i;
        }
        if (arg0 == D_800848D0[i + 1]) {
            return i + 1;
        }
        if (arg0 == D_800848D0[i + 2]) {
            return i + 2;
        }
        if (arg0 == D_800848D0[i + 3]) {
            return i + 3;
        }
        i += 4;
        if (i != 0x50) {
            continue;
        }
        break;
    }

    return i;
}

Gfx *func_15041508(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3)
{
  s32 rem;
  rem = arg3 % 8;
 { Gfx *_g = (Gfx *) (gfx++); _g->words.w0 = (((unsigned int) ((((unsigned int) 0xe4) & ((0x01 << 8) - 1)) << 24)) | ((unsigned int) ((((unsigned int) ((arg1 + 8) << 2)) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) ((arg2 + 12) << 2)) & ((0x01 << 12) - 1)) << 0)); _g->words.w1 = (((unsigned int) ((((unsigned int) 0) & ((0x01 << 3) - 1)) << 24)) | ((unsigned int) ((((unsigned int) (arg1 << 2)) & ((0x01 << 12) - 1)) << 12))) | ((unsigned int) ((((unsigned int) (arg2 << 2)) & ((0x01 << 12) - 1)) << 0)); { Gfx *_g = (Gfx *) (gfx++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xe1) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = (unsigned int) (((unsigned int) ((((unsigned int) ((rem * 8) << 5)) & ((0x01 << 16) - 1)) << 16)) | ((unsigned int) ((((unsigned int) ((((arg3 - rem) / 8) * 0x180) & 0xFFFFu)) & ((0x01 << 16) - 1)) << 0))); } ; { Gfx *_g = (Gfx *) (gfx++); _g->words.w0 = (unsigned int) ((((unsigned int) 0xf1) & ((0x01 << 8) - 1)) << 24); _g->words.w1 = (unsigned int) (((unsigned int) ((((unsigned int) 0x400) & ((0x01 << 16) - 1)) << 16)) | ((unsigned int) ((((unsigned int) 0x400) & ((0x01 << 16) - 1)) << 0))); }
    ;
  }
  ;
  return gfx;
}
void func_150415E0(s32 arg0, s32 *arg1, s32 *arg2, f32 *arg3, s32 *arg4, f32 *arg5);
s32 func_15042C40(u8 arg0);
