#include <ultra64.h>

#define func_1513D524 func_1513D524_void_proto
#define func_15169260 func_15169260_s32
#include "functions.h"
#undef func_1513D524
#undef func_15169260
#include "variables.h"

extern void *func_1513D524(s32, u8, u8, u8, u8, u8, s32, u8, s32);
extern s32 D_800DC9F0;

typedef void (*Callback_151416E8)(void *, s32, u8);
extern Callback_151416E8 D_8008A02C[];
void func_150A7960(f32 *arg0, f32 arg1, s32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6);

struct conk14182C_pos {
    f32 x;
    f32 y;
    f32 z;
};

struct conk14182C {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
};

struct conkMotionSample {
    f32 field_0x0;
    u8 pad_0[0x4];
    f32 field_0x8;
};

struct conkMotionControl {
    u8 pad_0[0x170];
    s32 field_0x170;
    f32 field_0x174;
    struct conkMotionSample *field_0x178;
    struct conk14182C_pos field_0x17C;
};


void *func_151407D0(void *arg0, s32 arg1, void *arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s8 arg7, u8 arg8, s32 arg9) {
    void *ret;
    void *dst;

    *(u8 *)((s32)arg2 + 1) = 3;
    *(s32 *)((s32)arg2 + 0x40) |= 0x40400000;
    ret = func_1513D524((s32)arg2, arg3, arg4, arg5, 1, arg6, arg1, arg8, arg9);
    if (ret != NULL) {
        dst = (void *)((s32)ret + 0x110);
        memcpy(dst, arg0, arg1);
        *(s8 *)((s32)dst + 0x59) = arg7;
        *(s32 *)((s32)dst + 0x44) = 0;
store_done_151407D0:
        ;
    } else {
        return NULL;
    }
    if (ret == NULL) {
    } else {
        D_800DC9F0 += 1;
    }
    return ret;
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 x[4];
    s32 y[4];
    u8 *unk44;
    f32 unk48;
    u16 depth[4];
    u8 alpha[4];
    u8 pad58;
    s8 unk59;
} Sub151408A4;

typedef struct {
    u8 pad0[0x2B];
    u8 unk2B;
    u8 pad2C[8];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 pad4C[0xC];
    s32 unk58;
    u8 unk5C;
    u8 pad5D[0x63];
    u8 tmpl[0x40];
    u8 *buffers[4];
    Sub151408A4 sub;
} Obj151408A4;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    s16 flag;
    u8 pad8[8];
} Vtx_151408A4;

typedef struct {
    u8 pad0[0x44];
    s16 unk44;
    u8 pad46[6];
    s16 unk4C;
} View151408A4;

typedef struct {
    u8 pad0[0x24];
    f32 minY;
    f32 maxY;
    f32 minX;
    f32 maxX;
    u8 pad34[0x14C];
} Cam151408A4;

typedef struct {
    s32 shift;
    u32 base;
} DepthDec151408A4;
extern DepthDec151408A4 D_80089630[];
extern s32 (*D_80089F90[])(void *, s16);
extern struct17 *func_15144B34(s32);
extern s32 func_15144CEC(void *, f32 *, f32 *, f32 *, f32 *, s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void *func_151408A4(Obj151408A4 *arg0, s16 arg1) {
    Vtx_151408A4 *vertices;
    void *result;
    Sub151408A4 *sub;
    struct17 v;
    f32 dirZ, dirY, dirX;
    f32 cx, cy, cz;
    struct17 *pos;
    f32 dot;
    f32 dist;
    f32 w;
    f32 h;
    Cam151408A4 *cam;
    f32 fade;
    f32 invDist;
    f32 horiz;
    f32 invH;
    f32 size;
    f32 b;
    f32 p;
    f32 q;
    f32 r;
    f32 s;
    f32 u;
    f32 a;
    f32 screen[2];
    f32 scale1;
    f32 unused;
    f32 scale2;
    s32 onScreen;
    u8 fresh;
    s16 alpha;

    sub = &arg0->sub;
    if (sub->unk59 != -1 && D_80089F90[sub->unk59](arg0, arg1) == 0) {
        return NULL;
    }
    sub->y[arg1] = -1;
    sub->x[arg1] = -1;
    if (arg0->unk58 & 2) {
        onScreen = func_15144CEC(&arg0->unk34, screen, &scale1, &unused, &scale2, arg1);
        if (onScreen == 0 || screen[0] < ((Cam151408A4 *)D_800BE628)[arg1].minX || ((Cam151408A4 *)D_800BE628)[arg1].maxX < screen[0] || screen[1] < ((Cam151408A4 *)D_800BE628)[arg1].minY || ((Cam151408A4 *)D_800BE628)[arg1].maxY < screen[1]) {
            return NULL;
        }
        pos = func_15144B34(arg1);
        {
        f32 dz, dy, dx;
        f32 lenSq;
        f32 scale;
        s32 visible;
        s32 depth;
        View151408A4 *view;
        s32 alphaPad;
        s16 zval;
        u16 zraw;
        func_151D5D60(arg0->buffers, arg1, 0x40, (void **)&vertices, &fresh);
        result = vertices;
        if (vertices != NULL) {
            if (fresh) {
                memcpy(arg0->buffers[arg1], arg0->tmpl, 0x40);
                memcpy(arg0->buffers[arg1] + 0x40, arg0->tmpl, 0x40);
            }
        } else {
            return NULL;
        }
        if (!(arg0->unk58 & 4)) {
            dx = arg0->unk40 - arg0->unk34;
            dy = arg0->unk44 - arg0->unk38;
            dz = arg0->unk48 - arg0->unk3C;
            lenSq = dx * dx + dy * dy + dz * dz;
            if (lenSq == 0.0f) {
                arg0->unk48 = arg0->unk3C + 500.0f;
            } else {
                scale = 500.0f / sqrtf(lenSq);
                arg0->unk40 = arg0->unk34 + dx * scale;
                arg0->unk44 = arg0->unk38 + dy * scale;
                arg0->unk48 = arg0->unk3C + dz * scale;
            }
            arg0->unk58 |= 4;
        }
        v.unk0 = pos->unk0 - arg0->unk34;
        v.unk4 = pos->unk4 - arg0->unk38;
        v.unk8 = pos->unk8 - arg0->unk3C;
        dirX = arg0->unk40 - arg0->unk34;
        dirY = arg0->unk44 - arg0->unk38;
        dirZ = arg0->unk48 - arg0->unk3C;
        dot = dirX * v.unk0 + dirY * v.unk4 + dirZ * v.unk8;
        if (dot < 0.0f) {
            dot = 0.0f;
        }
        if (dot == 0.0f && !(arg0->unk58 & 0x10000)) {
            return NULL;
        }
        if (fabsf(v.unk0) > 0.0001f || fabsf(v.unk8) > 0.0001f) {
            dist = sqrtf(v.unk0 * v.unk0 + v.unk4 * v.unk4 + v.unk8 * v.unk8);
            fade = 1.0f - sub->unk20 * dist;
            if (fade > 0.0f) {
                zraw = sub->depth[arg1];
                depth = ((((zraw >> 2) & 0x7FF) << D_80089630[zraw >> 13].shift) + D_80089630[zraw >> 13].base) >> 3;
                visible = 1;
                if (arg0->unk58 & 0x10) {
                    if (onScreen != 0) {
                        sub->x[arg1] = screen[0];
                        sub->y[arg1] = screen[1];
                        view = (View151408A4 *)((u8 *)D_800BE628 + arg1 * sizeof(Cam151408A4) + D_800BE9C0 * 0x10);
                        zval = (view->unk4C + scale1 * scale2 * view->unk44) * 32.0f;
                    } else {
                        sub->x[arg1] = -1;
                        sub->y[arg1] = -1;
                        visible = 0;
                    }
                    if (zval - depth >= 31) {
                        visible = 0;
                    }
                    if (visible) {
                        s32 pad;
                        sub->alpha[arg1] += (u16)(0xFF - sub->alpha[arg1]) >> 3;
                    } else {
                        sub->alpha[arg1] -= sub->alpha[arg1] >> 3;
                    }
                } else {
                    sub->alpha[arg1] = 0xFF;
                }
                horiz = sqrtf(v.unk0 * v.unk0 + v.unk8 * v.unk8);
                invH = 1.0f / horiz;
                size = 1.0f / dist; invDist = size;
                dot *= fade * invDist * 0.002000000095f;
                b = (sub->unk14 + (sub->unk10 - sub->unk14) * dot);
                cx = arg0->unk34 + b * v.unk0;
                cy = arg0->unk38 + b * v.unk4;
                cz = arg0->unk3C + b * v.unk8;
                w = (sub->unk8 + (sub->unk0 - sub->unk8) * dot) * sub->unk48;
                h = (sub->unkC + (sub->unk4 - sub->unkC) * dot) * sub->unk48;
                dz = invH * size; dz = h * dz;
                p = -v.unk0 * v.unk4 * dz;
                a = w * invH;
                s = -v.unk8 * v.unk4 * dz;
                r = h * horiz * invDist;
                q = -v.unk8 * a;
                u = -v.unk0 * a;
                vertices->x = p + q + cx;
                vertices->y = r + cy;
                vertices->z = s - u + cz;
                vertices->flag = 0;
                vertices++;
                vertices->x = p - q + cx;
                vertices->y = r + cy;
                vertices->z = s + u + cz;
                vertices->flag = 0;
                vertices++;
                vertices->x = -p - q + cx;
                vertices->y = cy - r;
                vertices->z = -s + u + cz;
                vertices->flag = 0;
                vertices++;
                vertices->x = -p + q + cx;
                vertices->y = cy - r;
                vertices->z = -s - u + cz;
                vertices->flag = 0;
                alpha = (sub->unk1C + (sub->unk18 - sub->unk1C) * dot) * 255.0f;
                arg0->unk5C = (sub->alpha[arg1] * alpha) >> 8;
                if (sub->unk44 != NULL) {
                    sub->unk44[0x30] = (arg0->unk5C * arg0->unk2B) >> 8;
                }
                return result;
            }
        }
        }
    }
    return NULL;
}

void func_151411A4(struct210 *arg0) {
    func_1513CA6C(arg0);
}

void func_151411C4(struct210 *arg0) {
    func_1513CAA0(arg0);
}

extern s32 D_800DC9F0;
extern void (*D_80089F9C[])(void *);
void func_1517E134(void *);

struct conk1411E4 { u8 pad0[0x154]; void *unk154; u8 pad158[0x10]; u8 unk168; };

void func_151411E4(struct conk1411E4 *arg0) {
    void **pp = &arg0->unk154;

    if (*pp != 0) {
        func_1517E134(*pp);
    }
    D_800DC9F0 -= 1;
    D_80089F9C[arg0->unk168](arg0);
}

extern s32 D_800DC9F0;
extern void (*D_80089FE4[])(void *);

void func_15141250(void *arg0) {
    s32 **pp = (s32 **)((u8 *)arg0 + 0x154);
    if (*pp != 0) {
        func_1517E134(*pp);
    }
    D_800DC9F0 -= 1;
    D_80089FE4[*(u8 *)((u8 *)arg0 + 0x168)](arg0);
}

typedef struct Sub151412BC {
    u8 pad0[0x24];
    s32 x[4];
    s32 y[4];
    u8 pad44[8];
    s16 out[4];
} Sub151412BC;

typedef struct Node151412BC {
    u8 pad0[8];
    struct Node151412BC *next;
    u8 padC[0x4C];
    s32 unk58;
    u8 pad5C[0xB4];
    Sub151412BC sub;
} Node151412BC;

extern u8 D_800DCE50[];

void func_151412BC(void)
{
  s32 p;
  u8 i;
  u8 j;
  Node151412BC *new_var;
  Node151412BC *node;
  Sub151412BC *sub;
  for (p = 0; p < 2; p++)
  {
    for (i = 0; i < 4; i++)
    {
      node = ((Node151412BC *(*)[104]) D_800DCE50)[p][((s32 *) (&D_800A5168))[i]];
      while (node != 0)
      {
        if (node->unk58 & 0x2000)
        {
          node->sub.out[3] = 0;
          node->sub.out[2] = 0;
          node->sub.out[1] = 0;
          new_var = node;
          node->sub.out[0] = 0;
          if (node->unk58 & 0x10)
          {
            j = 0;
            sub = &new_var->sub;
            for (; j <= D_80082FA0; j++)
            {
              if ((((sub->x[j] >= 0) && (sub->x[j] < D_800BE620)) && (sub->y[j] >= 0)) && (sub->y[j] < D_800BE624))
              {
                sub->out[j] = (((u16 *) D_800BE9C4) + (sub->y[j] * D_800BE620))[sub->x[j]];
              }
              else
              {
                sub->out[j] = 0x7FFF;
              }
            }

          }
        }
        node = node->next;
      }

    }

  }

}

void func_1514143C(void *arg0) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
    } Vec;
    typedef struct {
        u8 pad[0x44];
        Vec *unk44;
    } Mid;
    typedef struct {
        u8 pad0[0x34];
        f32 unk34;
        f32 unk38;
        f32 unk3C;
        u8 pad40[0xD0];
        Mid unk110;
    } Local;
    Local *a = arg0;
    Mid *m;

    if (a->unk110.unk44 != 0) {
    def_:
        m = &a->unk110;
    use:
        m->unk44->unk0 = a->unk34;
        m->unk44->unk4 = a->unk38;
        m->unk44->unk8 = a->unk3C;
    }
}

extern f32 D_800BE9A4;

struct conk141478b {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xC;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
};

struct conk141478a {
    u8 pad_0[0x48];
    f32 field_0x48;
};

struct conk141478 {
    u8 pad_0[0x110];
    struct conk141478a field_0x110;
    u8 pad_1[0x14];
    struct conk141478b field_0x170;
};

s32 func_15141478(struct conk141478 *arg0) {
    struct conk141478b *v1;
    struct conk141478a *a0;

    a0 = &arg0->field_0x110;
    v1 = &arg0->field_0x170;
    v1->field_0x10 -= D_800BE9A4;
    if (v1->field_0x10 < 0.0f) {
        v1->field_0x10 = func_150ADA68() * v1->field_0x14;
        if (func_150ADA20() & 3) {
            v1->field_0xC = func_150ADA68() * (v1->field_0x0 - v1->field_0x4) + v1->field_0x4;
        } else {
            v1->field_0xC = func_150ADA68() * (v1->field_0x8 - v1->field_0x0) + v1->field_0x0;
        }
    }
    a0->field_0x48 = a0->field_0x48 + (v1->field_0xC - a0->field_0x48) * v1->field_0x18;
    return 1;
}

s32 func_15141564(struct conk141478 *arg0) {
    f32 *v1 = &arg0->field_0x170.field_0x0;
    f32 *p;

    arg0->field_0x110.field_0x48 = v1[1] * sinf(arg0->field_0x170.field_0x8) + v1[0];
    p = &v1[2];
    *p = v1[3] * D_800BE9A4 + *p;
    *p = func_15144B68(*p);
    return 1;
}

s32 func_151415D4(void *arg0) {
    typedef struct {
        f32 unk0, unk4, unk8, unkC, unk10, unk14, unk18, unk1C, unk20;
    } Mid;
    typedef struct {
        u8 pad[0x158];
        f32 unk158;
        u8 pad15C[0x14];
        Mid unk170;
    } Local;
    Local *a = arg0;
    Mid *m = &a->unk170;
    f32 d;
    if (m->unkC < m->unk10) {
        a->unk158 = m->unk4;
    } else if (m->unkC < m->unk14) {
        f32 s;
        s = (m->unkC - m->unk10) * m->unk20;
        a->unk158 = m->unk4 + (m->unk8 * s);
    } else if (m->unkC < m->unk18) {
        a->unk158 = m->unk0;
    } else {
        d = 1.0f - ((m->unkC - m->unk18) * m->unk20);
        a->unk158 = m->unk4 + (m->unk8 * d);

    }
    m->unkC = m->unkC + D_800BE9A4;

    while (m->unk1C < m->unkC) {
        m->unkC = m->unkC - m->unk1C;
    }
    return 1;
}

void func_151416E8(void *arg0, s32 arg1, s32 arg2) {
    Callback_151416E8 *callbacks;
    u8 *v1;

    callbacks = D_8008A02C;
    if (((Callback_151416E8 volatile *)callbacks)[*(volatile u8 *)((u8 *)arg0 + 0x168)] != 0) {
        callbacks[*(volatile u8 *)((u8 *)arg0 + 0x168)](arg0, arg1, (u8)arg2);
    }

    v1 = (u8 *)arg0 + 0x110;
    switch ((u8)arg2) {
    case 0x22:
    case 0x24:
    case 0x25:
        if (*(u8 *)arg1 == v1[0x58]) {
            switch ((u8)arg2) {
            case 0x22:
                func_1516972C((struct102 *)arg0);
                break;
            case 0x24:
                ((s8 *)v1)[0x59] = -1;
                break;
            case 0x25:
                v1[0x59] = 2;
                break;
            }
        }
        break;
    }
}

struct Data_151417C4 {
    s32 unk0;
    s32 unk4;
};
void func_15169260(struct Data_151417C4 *, s32, u8 *, u8);

void func_151417C4(u8 arg0, u8 arg1) {
    u8 sp24[4];
    struct Data_151417C4 sp1C;

    sp1C = *(struct Data_151417C4 *)D_8008A074;
    sp24[0] = arg0;
    func_15169260(&sp1C, 2, sp24, arg1);
}

s32 func_15141818(s32 arg0, s32 arg1) {
    return 0;
}

f32 func_1514182C(void *arg0, void *arg1, s32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    register f32 temp_f14;
    register f32 temp_f16;
    register f32 temp_f18;
    f32 sp34[4][4];

    func_150A8050(sp34, arg4, 0.0f, arg5);
    sp34[3][0] = ((struct conk14182C_pos *)arg1)->x;
    sp34[3][1] = ((struct conk14182C_pos *)arg1)->y;
    sp34[3][2] = ((struct conk14182C_pos *)arg1)->z;
    func_150A7960(&sp34[0][0], 0.0f, arg2, 0.0f, &((struct conk14182C *)arg0)->unk34, &((struct conk14182C *)arg0)->unk38, &((struct conk14182C *)arg0)->unk3C);
    temp_f14 = (((struct conk14182C *)arg0)->unk34 - ((struct conk14182C_pos *)arg1)->x) * arg3;
    temp_f16 = (((struct conk14182C *)arg0)->unk38 - ((struct conk14182C_pos *)arg1)->y) * arg3;
    temp_f18 = (((struct conk14182C *)arg0)->unk3C - ((struct conk14182C_pos *)arg1)->z) * arg3;
    ((struct conk14182C *)arg0)->unk40 = ((struct conk14182C *)arg0)->unk34 + (temp_f14 * 500.0f);
    ((struct conk14182C *)arg0)->unk44 = ((struct conk14182C *)arg0)->unk38 + (temp_f16 * 500.0f);
    ((struct conk14182C *)arg0)->unk48 = ((struct conk14182C *)arg0)->unk3C + (temp_f18 * 500.0f);
}
// f32 func_1514182C(void *arg0, void *arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5) {
//     f32 sp6C;
//     f32 sp68;
//     f32 sp64;
//     ? sp34;
//     f32 temp_f0;
//     f32 temp_f12;
//     f32 temp_f2;
//
//     func_150A8050(&sp34, arg4, 0, arg5);
//     sp64 = arg1->unk0;
//     sp68 = arg1->unk4;
//     sp6C = arg1->unk8;
//     func_150A7960(&sp34, 0, arg2, 0, arg0 + 0x34, arg0 + 0x38, arg0 + 0x3C);
//     temp_f0 = arg0->unk34;
//     temp_f2 = arg0->unk38;
//     temp_f12 = arg0->unk3C;
//     arg0->unk40 = (f32) (temp_f0 + ((temp_f0 - arg1->unk0) * arg3 * 500.0f));
//     arg0->unk44 = (f32) (temp_f2 + ((temp_f2 - arg1->unk4) * arg3 * 500.0f));
//     arg0->unk48 = (f32) (temp_f12 + ((temp_f12 - arg1->unk8) * arg3 * 500.0f));
//     return temp_f0;
// }

extern f32 func_1514182C(void *, void *, s32, f32, f32, f32);

s32 func_15141928(struct conkMotionControl *arg0) {
    struct conkMotionSample *temp_v0;

    temp_v0 = arg0->field_0x178;
    func_1514182C(arg0, &arg0->field_0x17C, arg0->field_0x170, arg0->field_0x174, temp_v0->field_0x0, temp_v0->field_0x8);
    return 1;
}
// s32 func_15141928(void *arg0) {
//     void *temp_v0 = arg0->unk178;
//     func_1514182C(arg0, arg0->unk17C, arg0->unk170, arg0->unk174, temp_v0->unk0, temp_v0->unk8);
//     return 1;
// }
