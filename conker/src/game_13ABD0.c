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
void func_1510D630(s16 *arg0);
void func_1510D694(s32 arg0);

void func_1510D720(s32 arg0) {
    if ((s8)D_800BC448[arg0] != 0) {
        if (D_800D9F68[arg0] != 0) {
            if (--D_800D9F68[arg0] == 0) {
                if (arg0 < D_800D9F58) {
                    D_800D9F58 = arg0;
                }
                if (D_800D9F5C < arg0) {
                    D_800D9F5C = arg0;
                }
                func_1510D608(arg0, 2);
            }
        }
    }
}

void func_1510D7AC(s32 arg0) {
    register s8 *flag;

    flag = (s8 *)&D_800BC448[arg0];
    if (*flag != 0) {
        if (D_800D9F68[arg0] != 0) {
            if (--D_800D9F68[arg0] == 0) {
                if (*flag & 0x40) {
                    func_10004074(*(s16 **)D_800B0E58[arg0]);
                }
                func_10004074((s16 *)D_800B0E58[arg0]);
                D_800B0E58[arg0] = -1;
                *flag = 0;
            }
        }
    }
}

extern u8 D_800D9ED0;

void func_1510D864(void) {
    D_800D9ED0 = 0;
}

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
void func_1510D874(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
  struct Entry1510D874 *entry;
  u8 idx = D_800D9ED0;
  if (D_800D9ED0 < 8)
  {
    struct Entry1510D874 *e = &D_800D9ED8[idx];
    e->unk0 = arg0;
    e->unk4 = arg1;
    e->unk8 = arg2;
    entry = e;
    entry->unkC = arg3;
    e->unkD = arg4;
    D_800D9ED0 = idx + 1;
  }
}


Gfx *func_1510D8C0(Gfx *gfx, s32 arg1) {
    s32 i;

    for (i = 0; i < D_800D9ED0; i++) {
        if (arg1 == D_800D9ED8[i].unk0) {
            struct Entry1510D874 *e = &D_800D9ED8[i];

            gSPSegment(gfx++, e->unkC, e->unk4);
            if (e->unk8 != 0) {
                gSPSegment(gfx++, e->unkD, e->unk8);
            }
        }
    }
    return gfx;
}
