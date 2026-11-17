#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_1A37E0[];
extern u8 D_800D9F68[];

#define WGFX1510CDB8(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

extern u16 D_800D9E70[4][3];
extern u8 D_800D9E88[4][3];
extern u8 D_800D9E98[4][3];
extern u8 D_800D9EA8[4][3];
extern u8 D_800D9EB4[3];
extern u8 D_800D9EB8[4][3];

extern f32 func_150489B0(u8);
void func_1510CB10(s32 arg0);
Gfx *func_1510CDB8(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_1510D374(s32 arg0);
void func_1510D608(s32 arg0, s32 arg1);

void func_1510D694(s32);
extern s32 func_10004074(s16 *);

void func_1510D630(s16 *arg0) {
    s16 *p;
    s16 *end;
    s16 count = arg0[0];

    p = arg0 + 1;
    if (count > 0) {
        end = arg0 + count + 1;
        do {
            func_1510D694(*p);
            p++;
        } while (end != p);
    }
    func_10004074(arg0);
}

void func_1510D694(s32 arg0) {
    if ((s8)D_800BC448[arg0] != 0) {
        if (D_800D9F68[arg0] != 0) {
            if (--D_800D9F68[arg0] == 0) {
                if (arg0 < D_800D9F58) {
                    D_800D9F58 = arg0;
                }
                if (D_800D9F5C < arg0) {
                    D_800D9F5C = arg0;
                }
                func_1510D608(arg0, 3);
            }
        }
    }
}
void func_1510D720(s32 arg0);
void func_1510D7AC(s32 arg0);

extern u8 D_800D9ED0;
void func_1510D864(void);

extern u8 D_800D9ED0;
struct Entry1510D874
{
  s32 unk0;
  s32 unk4;
  s32 unk8;
  u8 unkC;
  u8 unkD;
};
extern struct Entry1510D874 D_800D9ED8[];
void func_1510D874(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
Gfx *func_1510D8C0(Gfx *gfx, s32 arg1);
