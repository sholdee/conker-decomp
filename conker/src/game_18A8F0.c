#include <ultra64.h>
#define func_1515D4D4 func_1515D4D4_prototype
#include "functions.h"
#undef func_1515D4D4
#include "variables.h"


s32 *allocate_memory(s32, s32, s32, s32);
s32 func_150A1DA0(u8 *, struct178 *, s32);
f32 func_1515F008(s16 *, s32);
void func_1515F040(f32, s32);
void func_1515F0AC(f32, s32);
extern void func_10004074(void *);
extern f32 D_800A6530;

struct Struct1515FBC4 {
    u8 pad0[0x98];
    struct127 *unk98;
};

struct Struct1515FBC4 *func_15105C24(s32);

s32 *func_1515D440(void) {
    s32 *temp;

    temp = allocate_memory(0x10, 1, 2, 0);
    bzero(temp, 0x10);
    return temp;
}

s32 *func_1515D480(s32 arg0) {
    s32 *temp;
    s32 size;
    s32 *pp = &arg0;

    size = *pp * 0x60;
    temp = allocate_memory(size, 1, 2, 0);
    bzero(temp, size);
    return temp;
}

extern u8 D_800DCD27;

void func_1515D4D4(s32 arg0, s32 arg1, s32 arg2, u8 arg3) {
    if (arg3 >= D_800DCD27) {
        ((u8 *)&D_800DCD20)[0] = arg0;
        ((u8 *)&D_800DCD20)[1] = arg1;
        ((u8 *)&D_800DCD20)[2] = arg2;
        D_800DCD7C = 1;
        D_800DCD27 = arg3;
    }
}

struct Node1515D520 {
    struct Node1515D520 *next;
};

s32 *func_1515D520(void) {
    struct Node1515D520 *node;
    struct Node1515D520 *p;
    struct Node1515D520 *q;

    node = (struct Node1515D520 *)allocate_memory(0x34, 1, 2, 2);
    if (node != 0) {
        bzero(node, 0x34);
        if (D_800DCD78 != 0) {
            p = (struct Node1515D520 *)D_800DCD78;
            q = p->next;
            while (q != 0) {
                p = q;
                q = q->next;
            }
            p->next = node;
        } else {
            D_800DCD78 = (s32)node;
        }
        node->next = 0;
    }
    return (s32 *)node;
}

s32 func_1515D5F8(s32, s32, s32, s32, s32, s32, s32, s32, s32, u8);

s32 func_1515D5AC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, u8 arg9) {
    return func_1515D5F8(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
}

struct Obj1515D5F8 {
    u8 pad0[4];
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 padC[2];
    s16 unkE;
    s16 unk10;
    s16 unk12;
    u8 pad14[0x2C - 0x14];
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
};

s32 func_1515D5F8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, u8 arg9) {
    struct Obj1515D5F8 *obj = (struct Obj1515D5F8 *)func_1515D520();

    if (obj != NULL) {
        obj->unk4 = 0;
        obj->unk5 = arg4;
        obj->unk6 = arg5;
        obj->unk7 = arg6;
        obj->unk8 = arg7;
        obj->unk9 = arg8;
        obj->unkB = arg9;
        obj->unkA = 0;
        obj->unkE = arg0;
        obj->unk10 = arg1;
        obj->unk12 = arg2;
        obj->unk2F = arg3;
        obj->unk2C = 0x7F;
        obj->unk2D = 0;
        obj->unk2E = 0;
    }
    return (s32)obj;
}


struct Node1515D69C {
    struct Node1515D69C *next;
    char pad4[8];
    s8 unkC;
    char pad10[0x23];
    s8 unk30;
};

void func_1515D69C(void) {
    struct Node1515D69C *p;

    for (p = (struct Node1515D69C *)D_800DCD78; p != 0; p = p->next) {
        p->unkC = 0;
        p->unk30 = 0;
    }
}

void func_1515D6C8(void) {
}

extern s32 D_800DCD10[];
void func_1515EF74(s16 *arg0);

#define MW1515D6D0(w0v, w1v) { Gfx *_g = (Gfx *)arg0; arg0 += 8; _g->words.w0 = (w0v); _g->words.w1 = (w1v); }

#define MWD(off, v) MW1515D6D0(0xDB100000 | (off), v)

s32 func_1515D6D0(s32 arg0, s32 arg1) {
    MW1515D6D0(0xDB020000, 0);
    MW1515D6D0(0xD9FFFFFF, 0x20000);
    func_1515EF74((s16 *)((u8 *)((struct259 *)D_800BE628 + arg1) + (D_800BE9C0 << 6) + 0x100));
    MWD(0x00, ((D_800DCD10[0] >> 16) << 16) | ((D_800DCD10[1] >> 16) & 0xFFFF));
    MWD(0x08, ((D_800DCD10[0] >> 16) << 16) | ((D_800DCD10[1] >> 16) & 0xFFFF));
    MWD(0x04, (D_800DCD10[2] >> 16) << 16);
    MWD(0x0C, (D_800DCD10[2] >> 16) << 16);
    MWD(0x10, ((D_800DCD10[0] & 0xFFFF) << 16) | ((D_800DCD10[1] & 0xFFFF) & 0xFFFF));
    MWD(0x18, ((D_800DCD10[0] & 0xFFFF) << 16) | ((D_800DCD10[1] & 0xFFFF) & 0xFFFF));
    MWD(0x14, (D_800DCD10[2] & 0xFFFF) << 16);
    MWD(0x1C, (D_800DCD10[2] & 0xFFFF) << 16);
    MWD(0x20, 0);
    MWD(0x24, D_800DCD10[3] << 16);
    MWD(0x28, 0);
    MWD(0x2C, D_800DCD10[3] << 16);
    return arg0;
}

/*MWSTART*/
#ifndef CMDDEF
#define CMD(a, b) { Gfx *_g = arg0++; _g->words.w0 = (a); _g->words.w1 = (b); }
#endif
struct Obj1515EC78;
struct Light1515EC78;
void func_1515E43C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4, u8 *arg5, u8 *arg6, u8 *arg7);
void func_1515E278(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4, u8 *arg5, u8 *arg6);
void func_1515EC78(struct Obj1515EC78 *arg0, s32 arg1, struct Light1515EC78 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
struct Node1515D914 {
    struct Node1515D914 *next;
    u8 unk4;
    u8 pad5[3];
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 padB;
    u8 unkC;
    u8 padD;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    u8 pad14[0x1B];
    u8 unk2F;
};

Gfx *func_1515D914(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5, u8 *arg6, s32 arg7, u8 *arg8, s8 *arg9, s32 argA, u8 *argB, s32 argC, s32 *argD) {
    s32 i;
    s32 j;
    s32 cnt;
    s32 special;
    u8 *ent;
    s16 pos[3];
    u8 colA[3];
    u8 colB[3];
    struct Node1515D914 *node;
    struct Node1515D914 *lights[11];
    s32 dists[11];
    s32 dist;
    u8 spB0[3];
    u8 spAC[3];
    u8 spAB;
    u8 spAA;
    f32 d;
    f32 vec[3];
    s32 lpos[3];
    f32 *fp;

    func_1515E278(arg2, arg3, arg4, argA, colA, arg5, (u8 *)argC);
    j = 1 << arg1;
    if (argC & 2) {
        func_1515E43C(arg2, arg3, arg4, argA, spB0, spAC, &spAB, &spAA);
        for (i = 0; i < 3; i++) {
            s32 k;
            k = (u8)((((0xFF - colA[i]) * spAB) >> 8) + colA[i]);
            colB[i] = (spB0[i] * k) >> 8;
            k = (u8)((((0xFF - colA[i]) * spAA) >> 8) + colA[i]);
            colA[i] = (spAC[i] * k) >> 8;
        }
    }
    for (i = 0; i < 3; i++) {
        if (argB != 0) {
            if (!(argC & 8)) {
                colA[i] = (colA[i] + argB[i] * 3) >> 2;
            }
            argB[i] = colA[i];
        }
        arg8[i] = arg8[i + 4] = colA[i];
        continue;
    }
    for (i = 0; i < arg7; i++) {
        dists[i] = 0x7FFFFFFF;
    }
    if (!(argC & 1)) {
        pos[0] = arg2;
        pos[1] = arg3;
        pos[2] = arg4;
    } else {
        vec[0] = D_800DBFF0[arg1].unk2BC - D_800DBFF0[arg1].unk2F8;
        vec[1] = D_800DBFF0[arg1].unk2C0 - D_800DBFF0[arg1].unk2FC;
        vec[2] = D_800DBFF0[arg1].unk2C4 - D_800DBFF0[arg1].unk300;
        d = sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
        if (d != 0.0f) {
            d = 1000.0f / d;
            pos[0] = vec[0] * d + (f32)arg2;
            pos[1] = vec[1] * d + (f32)arg3;
            pos[2] = vec[2] * d + (f32)arg4;
        } else {
            pos[0] = arg2;
            pos[1] = arg3;
            pos[2] = arg4;
        }
    }
    node = (struct Node1515D914 *)D_800DCD78;
    cnt = 0;
    if (node != 0) {
        do {
            if (((node->unkC & (1 << arg1)) || (argC & 0x20) || !(node->unk8 & 1)) && (node->unk8 & argA) && (node->unk9 == 0) && !(node->unkA & (1 << arg1))) {
                if (node->unk4 == 0) {
                G:
                    lpos[0] = node->unkE;
                    if (lpos[0] != -0x8000) {
                        lpos[1] = node->unk10;
                        lpos[2] = node->unk12;
                    } else {
                        fp = (f32 *)((node->unk12 & 0xFFFF) | (node->unk10 << 16));
                        lpos[0] = fp[0];
                        lpos[1] = fp[1];
                        lpos[2] = fp[2];
                    }
                    dist = (pos[0] - lpos[0]) * (pos[0] - lpos[0]) + (pos[1] - lpos[1]) * (pos[1] - lpos[1]) + (pos[2] - lpos[2]) * (pos[2] - lpos[2]);
                    if (!(argC & 4) && ((node->unk2F << 17) < dist)) {
                        dist = 0x7FFFFFFF;
                    }
                }
                for (i = 0; i < arg7 - 1; i++) {
                    if (dist < dists[i]) {
                        for (j = arg7 - 2; j > i; j--) {
                            dists[j] = dists[j - 1];
                            lights[j] = lights[j - 1];
                        }
                        dists[i] = dist;
                        lights[i] = node;
                        if (cnt != arg7 - 1) {
                            cnt++;
                        }
                        goto found;
                    }
                }
            found:;
            }
            node = node->next;
        } while (node != 0);
    }
    if (arg7 != 0) {
        if (cnt & 1) {
            special = 1;
            if (argD != 0) {
                argD[cnt] = 0;
            }
        } else {
            special = 0;
        }
        if (arg9 != 0) {
            if (special != 0) {
                *arg9 = (cnt + special + 1) | 0x80;
            } else {
                *arg9 = cnt + special + 1;
            }
        }
        CMD(0xDB020000, (cnt + special) * 0x30 + 0x30);
    } else {
        if (arg9 != 0) {
            *arg9 = cnt;
        }
        CMD(0xDB020000, cnt * 0x30);
    }
    if (arg7 != 0) {
        u8 *p;
        p = arg6 + (D_800BE9C0 * arg7 + cnt) * 0x30;
        p[8] = ((s8 *)&D_800DCD30)[arg1 * 3];
        p[9] = ((s8 *)&D_800DCD30)[arg1 * 3 + 1];
        p[0xA] = ((s8 *)&D_800DCD30)[arg1 * 3 + 2];
        if (argC & 2) {
            p[0] = p[4] = colB[0];
            p[1] = p[5] = colB[1];
            p[2] = p[6] = colB[2];
        } else {
            p[0] = p[4] = 0;
            p[1] = p[5] = 0;
            p[2] = p[6] = 0;
        }
        p -= cnt * 0x30;
        for (i = 0; i < cnt; i++, p += 0x30) {
            node = lights[i];
            if (argD != 0) {
                argD[i] = (s32)node;
            }
            if (argC & 0x20) {
                node->unkC |= 1 << arg1;
            }
            if (node->unk4 == 0) {
                func_1515EC78((struct Obj1515EC78 *)node, arg1, (struct Light1515EC78 *)p, arg2, arg3, arg4, argC);
            }
            CMD(((((u32)(i * 0x30 + 0x60) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA, (u32)p);
            continue;
        }
        if (special != 0) CMD(((((u32)(i * 0x30 + 0x60) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA, (u32)&D_800DCD40);
        CMD(((((u32)((i + special + 1) * 0x30U + 0x30) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA, ((D_800BE9C0 * arg7 + i) * 0x30) + (s32)arg6);
        CMD(((((u32)((i + special + 2) * 0x30U + 0x30) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA, (u32)arg8);
    }
    return arg0;
}


struct Node1515E43C {
    char pad0[0x18];
    u32 unk18;
    u32 unk1C;
};

struct Node1515EB84 {
    struct Node1515EB84 *next;
    struct178 *unk4;
};

struct Node1515E43C *func_1515EB84(s32, s32, s32, s32, struct Node1515EB84 *);
void func_1515E43C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4, u8 *arg5, u8 *arg6, u8 *arg7);

void func_1515E278(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4, u8 *arg5, u8 *arg6) {
    struct Node1515E43C *node;
    s32 i;
    s32 t;
    u8 sp50[3];
    u8 sp4C[3];
    u8 sp48[3];
    u8 sp47;
    u8 sp46;
    s32 s;
    u8 *dst;
    s32 v;

    node = func_1515EB84(arg0, arg1, arg2, arg3, (struct Node1515EB84 *)D_800DCD80);
    if (node != NULL) {
        arg4[0] = node->unk18 & 0xFF;
        arg4[1] = (node->unk18 >> 8) & 0xFF;
        arg4[2] = (node->unk18 >> 16) & 0xFF;
    } else {
        v = (0x100 - (s32)((((u32)arg5 >> 5) & 3) << 6));
        dst = arg4;
        for (i = 0; i < 3; i++) {
            *dst++ = ((*(u8 (*)[3])&D_800DCD20)[i] * v) >> 8;
        }
    }
    if ((s32)arg6 & 0x10) {
        func_1515E43C(arg0, arg1, arg2, arg3, sp4C, sp48, &sp47, &sp46);
        dst = arg4;
        for (i = 0; i < 3; i++) {
            s = (u8)((((0xFF - *dst) * sp47) >> 8) + *dst);
            sp50[i] = (sp4C[i] * s) >> 8;
            s = (u8)((((0xFF - *dst) * sp46) >> 8) + *dst);
            *dst = (sp48[i] * s) >> 8;
            t = *dst + ((sp50[i] * 7) >> 3);
            if (t > 0xFF) {
                t = 0xFF;
            }
            *dst = t;
            dst++;
        }
    }
}


void func_1515E43C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4, u8 *arg5, u8 *arg6, u8 *arg7) {
    struct Node1515E43C *node;

    node = func_1515EB84(arg0, arg1, arg2, 0, D_800DCD84);
    if (node != NULL) {
        arg4[0] = node->unk18 & 0xFF;
        arg4[1] = (node->unk18 >> 8) & 0xFF;
        arg4[2] = (node->unk18 >> 16) & 0xFF;
        arg5[0] = node->unk1C & 0xFF;
        arg5[1] = (node->unk1C >> 8) & 0xFF;
        arg5[2] = (node->unk1C >> 16) & 0xFF;
        arg6[0] = (node->unk1C >> 24) & 0xFF;
        arg7[0] = (node->unk18 >> 24) & 0xFF;
    } else {
        arg4[0] = ((u8 *)D_800DCD24)[0];
        arg4[1] = ((u8 *)D_800DCD24)[1];
        arg4[2] = ((u8 *)D_800DCD24)[2];
        arg5[0] = D_800DCD28[0];
        arg5[1] = D_800DCD28[1];
        arg5[2] = D_800DCD28[2];
        arg6[0] = D_800DCD3C;
        arg7[0] = D_800DCD3D;
    }
}

u32 *func_1515E544(u32 *arg0, s32 arg1, u8 arg2, u8 arg3, s32 arg4) {
    s32 count;
    s32 special;
    s32 index;

    count = arg3 & 0x7F;
    if (count != arg3) {
        special = 1;
    } else {
        special = 0;
    }
    { u32 *command = arg0; arg0 += 2; command[0] = 0xDB020000; command[1] = count * 0x30; }
    for (index = 0; index < count - special - 1; index++) {
        u32 *command = arg0; arg0 += 2; command[0] = ((((u32)(index * 0x30 + 0x60) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA; command[1] = ((D_800BE9C0 * arg2 + index) * 0x30) + arg1;
    }
    if (special != 0) {
        u32 *command = arg0; arg0 += 2; command[0] = ((((u32)(index * 0x30 + 0x60) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA; command[1] = (u32)&D_800DCD40;
    }
    { u32 *command = arg0; arg0 += 2; command[0] = ((((u32)((index + special + 1) * 0x30U + 0x30) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA; command[1] = ((D_800BE9C0 * arg2 + index) * 0x30) + arg1; }
    { u32 *command = arg0; arg0 += 2; command[0] = ((((u32)((index + special + 2) * 0x30U + 0x30) >> 3) & 0xFF) << 8) | 0xDC280000 | 0xA; command[1] = arg4; }
    return arg0;
}

struct Light1515E888 {
    u8 pad0[5];
    u8 r;
    u8 g;
    u8 b;
    u8 pad8[6];
    s16 x;
    s16 y;
    s16 z;
    u8 pad14[0x1B];
    u8 radius;
};

extern u8 D_800D9E21;
extern struct Light1515E888 *D_800D9E28[];
void func_1515E278(s32, s32, s32, s32, u8 *, u8 *, u8 *);

void func_1515E888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5, u8 *arg6, u8 *arg7)
{
  f32 sq;
  f32 dist;
  f32 pos[3];
  f32 color[3];
  s32 i;
  f32 *new_var;
  s32 n;
  struct Light1515E888 *light;
  struct Light1515E888 **p;
  f32 f;
  s32 v;
  func_1515E278(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
  color[0] = arg5[0];
  color[1] = arg5[1];
  color[2] = arg5[2];
  v = D_800D9E21 & 0x7F;
  n = v - 1;
  i = 0;
  if (n > 0)
  {
    p = D_800D9E28;
    new_var = pos;
    loop:
    light = *p;

    if (light != 0)
    {
      pos[0] = light->x;
      pos[1] = light->y;
      pos[2] = light->z;
      dist = light->radius * 2048.0f;
      sq = (((new_var[0] - arg1) * (new_var[0] - arg1)) + ((new_var[1] - arg2) * (new_var[1] - arg2))) + ((new_var[2] - arg3) * (new_var[2] - arg3));
      f = dist / sq;
      if (f > 1.0f)
      {
        f = 1.0f;
      }
      color[0] += f * light->r;
      color[1] += f * light->g;
      color[2] += f * light->b;
    }
    i++;
    p++;
    if (i != n)
    {
      goto loop;
    }
  }
  v = color[0];
  if (v >= 0x100)
  {
    v = 0xFF;
  }
  arg5[0] = v;
  v = color[1];
  if (v >= 0x100)
  {
    v = 0xFF;
  }
  arg5[1] = v;
  v = color[2];
  if (v >= 0x100)
  {
    v = 0xFF;
  }
  arg5[2] = v;
}

struct Node1515E43C *func_1515EB84(s32 arg0, s32 arg1, s32 arg2, s32 arg3, struct Node1515EB84 *node) {
    u8 sp34[0x32C];
    struct178 *entry;

    *(f32 *)&sp34[0x14] = (f32)arg0;
    *(f32 *)&sp34[0x18] = (f32)arg1;
    *(f32 *)&sp34[0x180] = (f32)arg1;
    *(f32 *)&sp34[0x2C] = (f32)arg0;
    *(f32 *)&sp34[0x30] = (f32)arg1;
    *(f32 *)&sp34[0x1C] = (f32)arg2;
    *(f32 *)&sp34[0x34] = (f32)arg2;

    while (node != 0) {
        entry = node->unk4;
        if (entry->unk6[0xE] == 0) {
            if ((arg3 == 0) || (((*(u32 *)&entry->unk6[0x12] >> 24) & arg3) != 0)) {
                if ((0x18 == ((s32)entry->unk6[0xF] >> 2)) || ((*(u32 *)&entry->unk6[0x12] >> 31) == 0)) {
                    if (func_150A1DA0(sp34, entry, 0) == 0) {
                        return (struct Node1515E43C *)node->unk4;
                    }
                }
            }
        }
        node = node->next;
    }
    return 0;
}

struct Light1515EC78 {
    u8 col[3];
    u8 pad3;
    u8 colc[3];
    u8 pad7;
    s8 dir[3];
    u8 padB;
    u8 unkC;
    u8 padD[0x13];
    s16 unk20[4];
    s16 unk28[4];
};

struct Obj1515EC78 {
    u8 pad0[5];
    u8 col[3];
    u8 pad8[6];
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14[4][3];
    u8 pad2C[3];
    u8 unk2F;
    u8 unk30;
};

extern f32 D_800D9C10[][4][4];
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

void func_1515EC78(struct Obj1515EC78 *arg0, s32 arg1, struct Light1515EC78 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 i;
    f32 out[3];
    f32 dx;
    f32 dy;
    f32 dz;
    f32 s;
    s32 n[3];
    s32 pos[3];
    f32 *p;

    pos[0] = arg0->unkE;
    if (pos[0] != -0x8000) {
        pos[1] = arg0->unk10;
        pos[2] = arg0->unk12;
    } else {
        p = (f32 *)((arg0->unk12 & 0xFFFF) | (arg0->unk10 << 16));
        pos[0] = p[0];
        pos[1] = p[1];
        pos[2] = p[2];
    }
    if (arg6 & 2) {
        dx = pos[0] - arg3;
        dy = pos[1] - arg4;
        dz = pos[2] - arg5;
        s = sqrtf(dx * dx + dy * dy + dz * dz);
        if (s) {
            s = 127.0f / s;
            dx *= s;
            dy *= s;
            dz *= s;
        } else {
            dz = dy = 0.0f;
            dx = 127.0f;
        }
        n[0] = dx;
        n[1] = dy;
        n[2] = dz;
    } else {
        n[0] = 0;
        n[1] = 0;
        n[2] = 0x7F;
    }
        if (arg0->unk30 & (1 << arg1)) {
        pos[0] = arg0->unk14[arg1][0];
        pos[1] = arg0->unk14[arg1][1];
        pos[2] = arg0->unk14[arg1][2];
    } else {
        func_150A7960(D_800D9C10[arg1][0], pos[0], pos[1], pos[2], &out[0], &out[1], &out[2]);
        pos[0] = out[0];
        pos[1] = out[1];
        pos[2] = out[2];
        arg0->unk14[arg1][0] = pos[0];
        arg0->unk14[arg1][1] = pos[1];
        arg0->unk14[arg1][2] = pos[2];
        arg0->unk30 |= (1 << arg1);
    }
    for (i = 0; i < 3; i++) {
        arg2->col[i] = arg0->col[i];
        arg2->colc[i] = arg0->col[i];
        arg2->dir[i] = n[i];
        arg2->unk20[i] = pos[i];
        arg2->unk28[i] = pos[i];
    }
    arg2->unkC = arg0->unk2F;
}

void func_1515EF74(s16 *arg0) {
    func_1515F040(1.0f / func_1515F008(arg0, 0), 0);
    func_1515F040(1.0f / func_1515F008(arg0, 5), 1);
    func_1515F040(1.0f / func_1515F008(arg0, 0xA), 2);
    func_1515F0AC(-func_1515F008(arg0, 0xE), 3);
}

f32 func_1515F008(s16 *arg0, s32 arg1) {
    s32 hi = arg0[arg1];
    hi = (hi << 16) | (u16)arg0[arg1 + 0x10];
    return (f32)hi * 1.52587890625e-05f;
}

extern s32 D_800DCD10[];

void func_1515F040(f32 arg0, s32 arg1) {
    f32 var_fv0;

    var_fv0 = 32767.0f;
    if (var_fv0 <= arg0) {
        arg0 = var_fv0;
    } else if (arg0 < -32768.0f) {
        arg0 = -32768.0f;
    }
    arg0 *= 65536.0f;
    D_800DCD10[arg1] = (s32) arg0;
}

extern s32 D_800DCD10[];

void func_1515F0AC(f32 arg0, s32 arg1) {
    f32 var_fv0;

    var_fv0 = 32767.0f;
    if (arg0 >= var_fv0) {
        arg0 = var_fv0;
    } else if (arg0 < -32768.0f) {
        arg0 = -32768.0f;
    }
    D_800DCD10[arg1] = (s32) arg0;
}

struct Node1515F10C {
    struct Node1515F10C *next;
};

void func_1515F10C(struct Node1515F10C *arg0) {
    struct Node1515F10C *node;
    struct Node1515F10C *prev;

    node = (struct Node1515F10C *)D_800DCD78;
    prev = 0;
    if (node != arg0) {
        do {
            prev = node;
            node = node->next;
        } while (node != arg0);
    }
    if (prev != 0) {
        prev->next = node->next;
    } else {
        D_800DCD78 = (s32)node->next;
    }
    func_10004074(node);
}

struct Node1515F170 {
    struct Node1515F170 *next;
    char pad4[5];
    s8 unk9;
    char padA[1];
    u8 unkB;
};

void func_1515F170(s32 arg0, u8 arg1) {
    struct Node1515F170 *p;

    for (p = (struct Node1515F170 *)D_800DCD78; p != 0; p = p->next) {
        if (arg0 == p->unkB) {
            p->unk9 = arg1;
        }
    }
}

struct Node1515F1B0 {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    s32 unkC;
};

struct Node1515F1B0 *func_1515F1B0(s32 arg0) {
    struct Node1515F1B0 *temp;

    temp = (struct Node1515F1B0 *)func_10003C6C(0x10, 1, 2, 0, 1);
    if (temp == 0) {
        return 0;
    }
    temp->unk0 = arg0;
    if (arg0 != 0) {
        temp->unk4 = func_1514462C(arg0);
    } else {
        temp->unk4 = 0.0f;
    }
    if (arg0 != 0) {
        temp->unk8 = func_15144598((struct134 *)arg0);
    } else {
        temp->unk8 = 0.0f;
    }
    temp->unkC = 0;
    return temp;
}

void func_1515F25C(void **arg0, void *arg1) {
    *(void **)((u8 *)arg1 + 0xC) = *arg0;
    *arg0 = arg1;
}

extern void (*D_8008B090[])(s32);

void func_1515F270(s32 arg0, s32 *arg1) {
    s32 idx = arg1[6];

    if (idx >= 0 && idx < 12) {
        if (D_8008B090[idx] != 0) {
            D_8008B090[idx](arg0);
        }
    }
}

s32 func_1505D024(struct127 *arg0, s32 arg1, u16 arg2, s32 arg3);

s32 func_1515F2B8(struct127 *arg0, s32 arg1) {
    return func_1505D024(arg0, 0x6001D, arg0->unk7A, -1);
}

extern void (*D_8008B0C0[])(void);

void func_1515F2E8(u8 *arg0, s32 *arg1) {
    s32 idx;

    if (arg0[0x3B] == 1) {
        idx = arg1[7];
        if (idx >= 0 && idx < 3) {
            D_8008B0C0[idx]();
        }
    }
}

void func_1515F338(s32 arg0, s32 arg1) {
    u8 temp_v1;
    u8 temp_t0;
    u8 temp_t1;

    temp_v1 = ((u8 *)&D_800DCD20)[0];
    temp_t0 = ((u8 *)&D_800DCD20)[1];
    temp_t1 = ((u8 *)&D_800DCD20)[2];
    if (D_800DCD94 != 97.0f) {
        D_800DCD94 += (97.0f - D_800DCD94) * 0.5f;
        temp_v1 = (u8)(u32)D_800DCD94;
    }
    if (D_800DCD98 != 96.0f) {
        D_800DCD98 += (96.0f - D_800DCD98) * 0.5f;
        temp_t0 = (u8)(u32)D_800DCD98;
    }
    if (D_800DCD9C != 98.0f) {
        D_800DCD9C += (98.0f - D_800DCD9C) * 0.5f;
        temp_t1 = (u8)(u32)D_800DCD9C;
    }
    func_1515D4D4(temp_v1, temp_t0, temp_t1, 0);
}

void func_1515F5C4(s32 arg0, s32 arg1) {
    u8 temp_v1;
    u8 temp_t0;
    u8 temp_t1;

    temp_v1 = ((u8 *)&D_800DCD20)[0];
    temp_t0 = ((u8 *)&D_800DCD20)[1];
    temp_t1 = ((u8 *)&D_800DCD20)[2];
    if (D_800DCD94 != 229.0f) {
        D_800DCD94 += (229.0f - D_800DCD94) * 0.5f;
        temp_v1 = (u8)(u32)D_800DCD94;
    }
    if (D_800DCD98 != 253.0f) {
        D_800DCD98 += (253.0f - D_800DCD98) * 0.5f;
        temp_t0 = (u8)(u32)D_800DCD98;
    }
    if (D_800DCD9C != 160.0f) {
        D_800DCD9C += (160.0f - D_800DCD9C) * 0.5f;
        temp_t1 = (u8)(u32)D_800DCD9C;
    }
    func_1515D4D4(temp_v1, temp_t0, temp_t1, 0);
}

struct Stack1515F850 {
    s32 pad14;
    s32 pad18;
    u8 sp1C;
    u8 sp1D;
    u8 sp1E;
    u8 pad1F;
    f32 sp20;
    f32 sp24;
    f32 sp28;
};

void func_1515F850(s32 arg0, s32 arg1) {
    s32 pad;
    struct Stack1515F850 stack;
    f32 temp_f0;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;

    stack.sp1C = ((u8 *)&D_800DCD20)[0];
    stack.sp1D = ((u8 *)&D_800DCD20)[1];
    stack.sp1E = ((u8 *)&D_800DCD20)[2];
    temp_f0 = sinf(D_800DCDA0);
    temp_f14 = (temp_f0 * 29.5f) + 127.5f;
    temp_f16 = (temp_f0 * 71.0f) + 109.0f;
    stack.sp28 = temp_f14;
    stack.sp24 = temp_f16;
    temp_f18 = (temp_f0 * 26.0f) + 26.0f;
    stack.sp20 = temp_f18;
    D_800DCDA0 += D_800A6530 * D_800BE9A4;
    D_800DCDA0 = func_15144B68(D_800DCDA0);
    if (stack.sp28 != D_800DCD94) {
        D_800DCD94 += (stack.sp28 - D_800DCD94) * 0.5f;
        stack.sp1C = (u8)(u32)D_800DCD94;
    }
    if (stack.sp24 != D_800DCD98) {
        D_800DCD98 += (stack.sp24 - D_800DCD98) * 0.5f;
        stack.sp1D = (u8)(u32)D_800DCD98;
    }
    if (stack.sp20 != D_800DCD9C) {
        D_800DCD9C += (stack.sp20 - D_800DCD9C) * 0.5f;
        stack.sp1E = (u8)(u32)D_800DCD9C;
    }
    func_1515D4D4(stack.sp1C, stack.sp1D, stack.sp1E, 0);
}

long func_1515FB70(u8 *a0, u8 *a1)
{
  long offset;
  s32 v0;
  s32 *slot;
  v0 = (*((u8 *) (a0 + 0x3B))) == 1;
  if ((*((u8 *) (a0 + 0x3B))) == 1)
  {
    v0 = *((s32 *) (a1 + 0x1C));
    if (v0 >= 0)
    {
      offset = 0x1C;
      if (!v0)
      {
        slot = (s32 *) (a1 + offset);
      }
    }
  }
}


s32 func_1515FB94(struct127 *arg0, s32 arg1) {
    return func_1505D024(arg0, 0x6002D, arg0->unk7A, -1);
}

void func_1515FBC4(struct127 *arg0, s32 arg1) {
    struct Struct1515FBC4 *temp_v0;
    struct127 *temp_v1;
    s32 temp_a3;

    temp_v1 = 0;
    temp_v0 = func_15105C24(arg1);
    if (temp_v0 != 0) {
        temp_v1 = temp_v0->unk98;
    }
    if (temp_v1 != 0) {
        temp_a3 = ((s32)temp_v1 - (s32)D_800CC2D0) / (s32)sizeof(struct127);
    } else {
        temp_a3 = -1;
    }
    func_1505D024(arg0, 0x6002E, arg0->unk7A, temp_a3);
}

s32 func_1505D024(struct127 *arg0, s32 arg1, u16 arg2, s32 arg3);

s32 func_1515FC34(struct127 *arg0, s32 arg1) {
    return func_1505D024(arg0, 0x33, 0xC000, -1);
}

struct Struct1515FC60 {
    s16 unk0;
    u8 pad2[2];
    s16 unk4;
    s16 unk6;
    u8 pad8[0x14];
    u32 unk1C;
};

union Union1515FC60 {
    f32 f;
    f32 pad[2];
};

extern f32 D_800A6534;
s32 func_150AD960(s32, s32, s32, s32);
void func_15136C3C(struct127 *, s32, s32, s32, s32, s32, s32, s32);
void func_1507CD64(struct127 *, s32);

void func_1515FC60(struct127 *arg0, struct Struct1515FC60 *arg1) {
    f32 temp_f0;
    union Union1515FC60 temp_f18;
    s32 temp_v0;

    if (arg0->unk31C != NULL) {
        if (arg0->unk31C->unk120 == 0) {
            temp_v0 = arg1->unk1C;
            if (temp_v0 != 0) {
                temp_f18.f = (f32)(u32)temp_v0 * D_800A6534;
                temp_v0 = func_150AD960(arg1->unk0, arg1->unk4, (s32)arg0->x_position, (s32)arg0->z_position);
                temp_f0 = ((f32)arg1->unk6 * temp_f18.f) - (f32)arg0->unkE4;
                if (temp_f0 > (f32)temp_v0) {
                    return;
                }
            }
            func_1505D024(arg0, 0x2F, 0, -1);
            func_15136C3C(arg0, 1, 1, 1, 1, 0, 0xFF, 1);
            func_15145A50(arg0);
            func_1507CD64(arg0, 6);
        }
    }
}
