#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_1A37E0[];
extern u8 D_800D9F68[];

extern u16 D_800D9E70[4][3];
extern u8 D_800D9E88[4][3];
extern u8 D_800D9E98[4][3];
extern u8 D_800D9EA8[4][3];
extern u8 D_800D9EB4[3];
extern u8 D_800D9EB8[4][3];

extern f32 func_150489B0(u8);

/* Steps slot's prim/env tint channels toward their targets and adds a sine pulse. */
void func_1510CB10(s32 slot) {
    f32 wave;
    s32 step;
    s32 target;
    s32 diff;
    s32 override;
    s32 cur;
    s32 amount;
    s32 i;

    if (D_800DBFF0[slot].unk5F0 & 1) {
        override = D_800D9EB8[slot][0];
        step = D_800BE9E4 * 2;

        for (i = 0; i < 3; i++) {
            cur = D_800D9EA8[slot][i];
            if (override) {
                target = D_800D9EB8[slot][i];
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
                D_800D9EA8[slot][i] = cur;
            }

            wave = D_800D9E98[slot][i] * func_150489B0(D_800D9E70[slot][i] >> 4);
            amount = (s32)wave + cur - 127;
            if (amount >= 0) {
                D_800D9B68[slot][i] += amount;
            } else {
                D_800D9B78[slot][i] -= amount;
            }

            if (D_800D9B68[slot][i] >= 128) {
                D_800D9B68[slot][i] = 127;
            }
            if (D_800D9B78[slot][i] >= 128) {
                D_800D9B78[slot][i] = 127;
            }

            D_800D9E70[slot][i] += D_800D9E88[slot][i];
            D_800D9E70[slot][i] &= 0xFFF;
        }

        D_800D9EB8[slot][0] = 0;
    }
}

/* Emits the prim and env colors for slot's current tint. */
Gfx *func_1510CDB8(Gfx *gfx, s32 primAlpha, s32 envAlpha, s32 slot) {
    gDPSetPrimColor(gfx++, 0xF2, 0, D_800D9B68[slot][0], D_800D9B68[slot][1], D_800D9B68[slot][2], primAlpha);
    gDPSetEnvColor(gfx++, D_800D9B78[slot][0], D_800D9B78[slot][1], D_800D9B78[slot][2], envAlpha);
    return gfx;
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

typedef struct {
    u8 *data;
    s32 size;
} Entry1510D404;

extern u8 D_800DBDBA;
extern void func_10006240(u8 *, Entry1510D404 *, u32);
extern s32 func_10004074(s16 *);
s32 func_1510D374(s32 arg0);

s32 func_1510D0EC(s32 arg0, s32 *arg1, s32 arg2, s32 arg3) {
    s32 size;
    s32 offset;
    s32 dst;
    s32 odd;
    s32 buf;
    s32 len;

    if (arg0 < D_800D9F58) {
        D_800D9F58 = arg0;
    }
    if (D_800D9F5C < arg0) {
        D_800D9F5C = arg0;
    }
    if (arg0 >= 0x1E52 || arg0 < 0) {
        return 0x80000000;
    }
    size = ((u16 *)D_80091D20)[arg0];
    if (size == 0) {
        D_800B0E58[arg0] = 0x80000000;
    } else if (D_800B0E58[arg0] == -1) {
        D_800DBDBA = 5;
        if (arg2 == 0x3F) {
            arg2 = 0x3E;
        }
        offset = func_1510D374(arg0);
        len = size;
        if (offset & 1) {
            offset--;
            odd = 1;
        } else {
            odd = 0;
        }
        size = len + odd;
        if (size & 1) {
            len++;
        }
        buf = func_10003C6C((len + odd + 0xF) & ~0xF, 1, 2, 1, 2);
        if (buf == 0) {
            return 0x80000000;
        }
        func_10004514(offset, (void *)buf, (len + odd + 0xF) & ~0xF, 1);
        len = ((u16 *)D_800B87A0)[arg0];
        dst = func_10003C6C(len, 1, 1, 0, 2);
        if (dst == 0) {
            func_10004074((s16 *)buf);
            return 0x80000000;
        }
        func_10006240((u8 *)(buf + odd), (Entry1510D404 *)dst, D_8003809C);
        func_10004074((s16 *)buf);
        D_800B0E58[arg0] = dst;
        D_800D9F68[arg0] = 0;
    }
    if (arg1 != NULL) {
        len = ((u16 *)D_800B87A0)[arg0];
        *arg1 = len;
    }
    len = D_800B0E58[arg0];
    if (((s8 *)D_800BC448)[arg0] < arg2) {
        ((s8 *)D_800BC448)[arg0] = arg2;
    }
    if (arg3 != 0) {
        if (D_800D9F68[arg0] < 0xFF) {
            D_800D9F68[arg0]++;
        }
    }
    if (0) { func_10004074((s16 *)&dst); }
    return len;
}

s32 func_1510D374(s32 arg0) {
    s32 sum;
    s32 i;

    sum = (s32)D_1A37E0;
    for (i = 0; i < arg0; i++) {
        sum += ((u16 *)D_80091D20)[i];
    }

    return sum;
}

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
