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

void func_1510CB10(s32 arg0) {
    f32 wave;
    s32 step;
    s32 target;
    s32 diff;
    s32 flash;
    s32 cur;
    s32 amount;
    s32 i;

    if (D_800DBFF0[arg0].unk5F0 & 1) {
        flash = D_800D9EB8[arg0][0];
        step = D_800BE9E4 * 2;

        for (i = 0; i < 3; i++) {
            cur = D_800D9EA8[arg0][i];
            if (flash) {
                target = D_800D9EB8[arg0][i];
            } else {
                target = D_800D9EB4[i];
            }

            diff = target - cur;
            if (diff != 0) {
                amount = (diff < 0) ? -diff : diff;
                if (amount < step) {
                    cur = target;
                } else if (diff < 0) {
                    cur -= step;
                } else {
                    cur += step;
                }
                D_800D9EA8[arg0][i] = cur;
            }

            wave = (u32)D_800D9E98[arg0][i] * func_150489B0(D_800D9E70[arg0][i] >> 4);
            amount = (s32)wave + cur - 0x7F;
            if (amount >= 0) {
                D_800D9B68[arg0][i] += amount;
            } else {
                D_800D9B78[arg0][i] -= amount;
            }

            if (D_800D9B68[arg0][i] >= 0x80) {
                D_800D9B68[arg0][i] = 0x7F;
            }
            if (D_800D9B78[arg0][i] >= 0x80) {
                D_800D9B78[arg0][i] = 0x7F;
            }

            D_800D9E70[arg0][i] += D_800D9E88[arg0][i];
            D_800D9E70[arg0][i] &= 0xFFF;
        }

        D_800D9EB8[arg0][0] = 0;
    }
}

Gfx *func_1510CDB8(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3) {
    gDPSetPrimColor(arg0++, 0xF2, 0, D_800D9B68[arg3][0], D_800D9B68[arg3][1], D_800D9B68[arg3][2], arg1);

    gDPSetEnvColor(arg0++, D_800D9B78[arg3][0], D_800D9B78[arg3][1], D_800D9B78[arg3][2], arg2);

    return arg0;
}

extern void *allocate_memory(s32, s32, s32, s32);
extern s32 func_1510D0EC(s32, s32 *, s32, s32);

s32 func_1510CE60(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s16 **arg4) {
    s32 seg;
    s32 i;
    s32 out;
    s32 flags;
    s32 w1;
    s32 failed;
    s32 addr;
    s32 v;
    s32 count;
    s16 *list;
    s32 bit;
    s32 j;
    Gfx *gfx;
    u8 used[971];

    if (1) {}
    if (1) {}
    if (&arg4 == NULL) {}
    failed = 0;
    if (arg4 != NULL) {
        for (i = 0; i < 971; i++) {
            used[i] = 0;
        }
        count = 0;
    }
    for (i = 0; *(s8 *)&arg0[i] != (s8)G_ENDDL; i++) {
        gfx = &arg0[i];
        if (*(s8 *)&arg0[i] == (s8)G_SETTIMG) {
            w1 = gfx->words.w1;
            if ((arg1 == 0 || !(w1 & 0xFF000000)) && (u32)(w1 & 0xFF000000) < 0x06000000) {
                flags = w1 >> 22;
                seg = w1 & 0x0F000000;
                w1 &= 0xF03FFFFF;
                if (seg == 0) {
                    addr = func_1510D0EC(w1, &out, arg3, arg2);
                    gfx->words.w1 = addr;
                    if (arg4 != NULL) {
                        v = 1 << (w1 & 7);
                        if (!(used[w1 >> 3] & v)) {
                            used[w1 >> 3] |= v;
                            count++;
                        }
                    }
                } else {
                    flags = 0;
                }
                if (addr == 0x80000000) {
                    failed = 1;
                }
                if (flags != 0) {
                    if (flags & 1) {
                        gfx->words.w1 += out - 0x200;
                    } else if (flags & 2) {
                        gfx->words.w1 += out - 0x20;
                    }
                    gfx->words.w1 |= (flags & 0x3C) << 22;
                }
            }
        }
    }
    if (arg4 != NULL) {
        list = allocate_memory((count + 1) << 1, 1, 0, 2);
        *arg4 = list;
        if (list != NULL) {
            *list++ = count;
            bit = 1;
            i = 0;
            v = 0;
            j = 0;
            while (i != count) {
                if (used[j] & bit) {
                    list[i] = v;
                    i++;
                }
                if (bit != 0x80) {
                    bit <<= 1;
                } else {
                    bit = 1;
                    j++;
                }
                v++;
            }
        }
    }
    return failed == 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_139FC0/func_1510D0EC.s")

s32 func_1510D374(s32 arg0) {
    s32 sum;
    s32 i;

    sum = (s32)D_1A37E0;
    for (i = 0; i < arg0; i++) {
        sum += ((u16 *)D_80091D20)[i];
    }

    return sum;
}

typedef struct {
    u8 *data;
    s32 size;
} Entry1510D404;

extern u8 D_800DBDBA;

extern void func_10006240(u8 *, Entry1510D404 *, u32);
extern s32 func_10004074(s16 *);

#define FL(i) (*(s8 *)&D_800BC448[i])

void func_1510D404(void) {
    static s32 D_800DBDBC;
    s32 start;
    s32 end;
    s32 i;
    u8 *data;
    s32 size;

    if (D_800D9F5C == -1) {
        return;
    }
    if (D_800DBDBA == 0 && D_800D9F60 != 0) {
        return;
    }
    if (D_800DBDBA != 0) {
        D_800DBDBA -= 1;
    }
    start = D_800D9F58;
    end = D_800D9F5C;
    D_800D9F58 = 0xFFFF;
    D_800D9F5C = -1;
    if (start < 0 || end >= 0x1E53) {
        D_8003C8E0 = 0x0C000046;
        func_150AD770();
    }
    D_800DBDBC = -1;
    for (i = start; i <= end; i++) {
        if (FL(i) != 0) {
            if (FL(i) < 4) {
                FL(i)--;
                if (FL(i) == 0) {
                    D_800DBDBC = i;
                    if (func_10004074((s16 *)D_800B0E58[i]) == -1) {
                        /* empty error branch: the dead compare still weights -1 for register allocation */
                    }
                    D_800B0E58[i] = -1;
                    continue;
                } else {
                    if (i < D_800D9F58) {
                        D_800D9F58 = i;
                    }
                    if (D_800D9F5C < i) {
                        D_800D9F5C = i;
                    }
                }
            } else if (FL(i) & 0x40) {
                data = ((Entry1510D404 *)D_800B0E58[i])->data;
                size = ((Entry1510D404 *)D_800B0E58[i])->size;
                func_10006240(data + size, (Entry1510D404 *)D_800B0E58[i], D_8003809C);
                func_10004074((s16 *)data);
                FL(i) &= ~0x40;
                if (i < D_800D9F58) {
                    D_800D9F58 = i;
                }
                if (D_800D9F5C < i) {
                    D_800D9F5C = i;
                }
            }
        }
    }
    D_800DBDBC = -2;
}

void func_1510D608(s32 arg0, s32 arg1) {
    s8 *p = (s8 *)&D_800BC448[arg0];
    s8 temp = *p;
    if (temp != 0) {
        *p = (temp & 0x40) | arg1;
    }
}

void func_1510D694(s32);
extern s32 func_10004074(s16 *);
void func_1510D630(s16 *arg0);
void func_1510D694(s32 arg0);
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
