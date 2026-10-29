#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern void *func_1515FF74(void *, s32, u8, s32);
extern f32 D_800A07FC;
struct Sub150CE150
{
  s8 unk0;
  s8 unk1;
  s8 unk2;
  s8 pad3;
  s16 unk4;
  s8 unk6;
  s8 pad7;
};
struct Buf150CE150
{
  void *unk0;
  u8 unk4;
  s8 pad5;
  s8 pad6;
  s8 pad7;
  f32 unk8;
  f32 unkC;
  s32 unk10;
  s8 unk14;
  char pad15[0x8F0 - 0x15];
};
void *func_150CE150(void *arg0, s16 arg1, u8 arg2, s32 arg3)
{
  struct Sub150CE150 sp910;
  struct Buf150CE150 sp20;
  void *temp_v0;
  if (arg0 == 0)
  {
    return 0;
    return temp_v0;
  }
  sp20.unk8 = D_800A07FC;
  sp910.unk0 = 1;
  sp910.unk1 = 1;
  sp910.unk2 = 0;
  sp910.unk4 = arg1;
  sp910.unk6 = 2;
  sp20.unk10 = 0x2710;
  sp20.unk14 = 0;
  sp20.unkC = D_800A07FC;
  sp20.unk0 = arg0;
  sp20.unk4 = *((u8 *) (((s32) arg0) + 0x3B));
  temp_v0 = func_1515FF74(&sp910, 0x8F0, arg2, arg3);
  if (temp_v0 != 0)
  {
    temp_v0 = memcpy((void *) (((s32) temp_v0) + 0x18), &sp20, 0x8F0);
  }
}


typedef struct {
    /* 0x000 */ void *unk0;
    /* 0x004 */ u8 unk4;
    /* 0x008 */ f32 unk8;
    /* 0x00C */ f32 unkC;
    /* 0x010 */ s32 step;
    /* 0x014 */ u8 flags;
    /* 0x018 */ s32 x0;
    /* 0x01C */ s32 y0;
    /* 0x020 */ s32 x1;
    /* 0x024 */ s32 y1;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ s16 xmap[640];
    /* 0x530 */ s16 ymap[480];
} Zoom150CE450;

extern f32 D_800A07F0[3];
void func_15143134(void *, void *, s32);
s32 func_15144CEC(f32 *, f32 *, f32 *, f32 *, f32 *, s32);

s32 func_150CE200(void *arg0) {
    Zoom150CE450 *p = (Zoom150CE450 *)((u8 *)arg0 + 0x18);
    f32 pos[3];
    f32 sp3C;
    f32 sp38;
    f32 scale;

    if (*(s32 *)p->unk0 == 0) {
        return 0;
    }
    if (p->unk4 != *((u8 *)p->unk0 + 0x3B)) {
        return 0;
    }
    if (*(s32 *)((u8 *)p->unk0 + 0x1D4) == 0) {
        p->flags &= ~1;
    } else {
        func_15143134(D_800A07F0, pos, *(s32 *)((u8 *)p->unk0 + 0x1D4) + 0xD00);
        if (func_15144CEC(pos, &p->unk28, &sp3C, &sp38, &scale, D_80082FA4)) {
            f32 dx;
            f32 dy;
            dx = p->unk8 * scale;
            dy = p->unkC * scale;
            p->x0 = p->unk28 - dx;
            p->y0 = p->unk2C - dy;
            p->x1 = p->unk28 + dx;
            p->y1 = p->unk2C + dy;
            if (p->x0 > 291.0f || p->x1 < 0.0f || p->y0 > 215.0f || p->y1 < 0.0f) {
                p->flags &= ~1;
            } else {
                if (p->x0 < 0.0f) {
                    p->x0 = 0;
                }
                if (p->y0 < 0.0f) {
                    p->y0 = 0;
                }
                if (p->x1 > 291.0f) {
                    p->x1 = 291;
                }
                if (p->y1 > 215.0f) {
                    p->y1 = 215;
                }
                p->flags |= 1;
            lbl_ret:
                return 1;
            }
        } else {
            p->flags &= ~1;
        }
    }
    return 1;
}

void func_150CE450(void *arg0, u16 *fb) {
    Zoom150CE450 *p = (Zoom150CE450 *)((u8 *)arg0 + 0x18);
    s16 *xm;
    s16 *ym;
    u32 acc;
    u32 i;
    s32 lo;
    s32 src;
    s32 y;
    s32 x;

    if ((p->flags & 1) == 0) {
        return;
    }

    xm = p->xmap;
    ym = p->ymap;

    acc = 0;
    lo = p->x0;
    src = lo;
    for (i = lo; i <= p->x1; i++) {
        xm[i] = src;
        acc += p->step;
        if (acc >= 0x10000) {
            src = i;
            acc -= 0x10000;
        }
    }

    acc = 0;
    lo = p->y0;
    src = lo;
    for (i = lo; i <= p->y1; i++) {
        ym[i] = src;
        acc += p->step;
        if (acc >= 0x10000) {
            src = i;
            acc -= 0x10000;
        }
    }

    for (y = p->y1; y >= p->y0; y--) {
        if ((y != p->y1) && (ym[y + 1] == ym[y])) {
            memcpy((u8 *)fb + (((y * D_800BE620) + p->x0) * 2),
                   (u8 *)fb + ((((y + 1) * D_800BE620) + p->x0) * 2),
                   ((p->x1 - p->x0) * 2) + 2);
        } else {
            for (x = p->x1; x >= p->x0; x--) {
                fb[(y * D_800BE620) + x] = fb[(ym[y] * D_800BE620) + xm[x]];
            }
        }
    }
}

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA_150CE694;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8 b4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_150CE694;

void func_150CE694(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA_150CE694 *temp_v0 = (SubA_150CE694 *)((u8 *)arg0 + 0x18);
    ArgB_150CE694 *b = (ArgB_150CE694 *)arg1;
    s32 b0;

    if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else {
            if (temp_v0->unk0 == b->u4.w4) {
                temp_v0->unk0 = b->unk0;
                temp_v0->unk4 = b->unk8;
            }
trailing_150CE694:
            ;
        }
    } else if (arg2 == 0) {
        b0 = b->unk0;
        if ((b0 == temp_v0->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    }
}

extern u16 D_800BE648[];
extern u8 D_800A080F[];
extern u8 D_800A0818[];
void func_15052590(struct127 *arg0);
void func_15052F9C(struct127 *arg0, f32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void func_1505327C(struct127 *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
void func_15063168(struct127 *arg0);

void func_150CE740(struct127 *arg0) {
    struct126 *obj;
    u16 sp42;
    u16 sp40;
    u8 isT0;
    u8 mode;
    s16 turn;
    s16 v;

    isT0 = 0;
    sp40 = arg0->unk7A;
    if (arg0->id == 0x21) {
        isT0 = 1;
    }
    v = arg0->unk13C;
    if ((v == 0) && (D_800BE616 == 0)) {
        goto skip;
    }
    {
        mode = 0;
        if (D_800BE616 != 0) {
            if (arg0->unk31C->unk84 != 0) {
                func_1504C854(arg0);
            } else {
                D_800CC288 = D_800BE710[D_800C3E78];
            }
        } else {
            D_800CC288 = D_800BE648[v];
        }
        if ((D_800CC288 & 0x8000) && (arg0->disable_jump == 0) && (arg0->unk28 == 0.0f)) {
            mode = 1;
        }
        if ((D_800CC288 & 0x6000) && (arg0->unk28 == 0.0f) && (((isT0) && (arg0->disable_run != 0xFF)) || (arg0->disable_run == 0))) {
            mode = 2;
        }
        if (mode) {
            if (isT0) {
                mode += 4;
            }
            if (D_800BE616 != 0) {
                mode += 2;
            }
            arg0->disable_run = 0xB4;
            arg0->unk232 = D_800A080F[mode];
            arg0->unk218 = NULL;
            if ((arg0->unk232 == 0x22) && (arg0->unk2E4 == 0)) {
                arg0->unk31C->unk1AA++;
            }
        }
        arg0->unk124 = 0;
        if ((arg0->disable_run == 0) && ((D_800BE616 == 0) || (arg0->unk31C->unk84 == 0))) {
            if (D_800BE616 != 0) {
                D_800CC280 = D_800DBFF0[D_800C3E78].unk37C * 182.0444488525390625f;
                sp42 = func_1505A630(((s8 *)D_800CC284)[2], ((s8 *)D_800CC284)[3], 0) + D_800CC280;
                arg0->unk44 = func_1505A5CC((struct49 *)D_800CC284);
            } else {
                arg0->unk44 = D_800CC2D0[arg0->unk124].unk44 * 1.75f;
                sp42 = D_800CC2D0[arg0->unk124].unk31C->unk4C;
            }
            if (arg0->unk223 == 0xD) {
                arg0->unk44 = 0.0f;
            }
            if (isT0) {
                if (arg0->unk44 < 5.0f) {
                    sp42 = func_150ADA20();
                    arg0->unk44 = (func_150ADA20() % 60U) + 20.0f;
                }
                arg0->unk78 = ((func_150ADA20() % 10000U) + sp42) - 5000;
                arg0->unk44 = (arg0->unk44 * 0.5f) + 22.0f;
                if ((u8)(((s32)(arg0->unk76 - arg0->unk78) >> 8) - 0x50) < 0x60) {
                    arg0->unk218 = NULL;
                    arg0->unk232 = 0x10;
                }
            } else if (arg0->unk44 < 5.0f) {
                arg0->unk44 = 0.0f;
                if (arg0->unk232 != 5) {
                    arg0->unk78 = arg0->unk76;
                }
            } else {
                arg0->unk78 = sp42;
                if (arg0->unk223 == 1) {
                    v = 3;
                    if (arg0->xz_velocity < 35.0f) {
                        v = 4;
                    }
                    arg0->unk1E6 = v;
                    arg0->unk1E5 = v;
                    if ((u8)(((s32)(arg0->unk76 - arg0->unk78) >> 8) - 0x50) < 0x60) {
                        if (arg0->xz_velocity > 35.0f) {
                            arg0->unk232 = 4;
                        } else {
                            arg0->unk232 = 5;
                        }
                    } else {
                        arg0->unk232 = 3;
                    }
                    arg0->unk218 = NULL;
                }
            }
        }
    }
skip:
    if ((D_800BE616 == 0) || (D_800C3E78 >= D_8008FD8C) || (arg0->unk31C->unk120 == 0)) {
        func_15052590(arg0);
    }
    if ((D_800CC2D0[0].unk31C->unk27 != 0) && ((arg0->unk232 != D_800A0818[isT0]) || (arg0->stunned != 0))) {
        D_800CC2D0[0].unk31C->unk27 = 0;
        D_800CC2D0[0].disable_run = 0;
        D_800CC2D0[0].unk83 = 0;
        D_800CC2D0[0].immune = 0;
        arg0->unk232 = 0x11;
    }
        if (D_800BE616 == 0) {
        if (arg0->unk232 == (&D_800A0818[isT0])[0]) {
            func_15052F9C(arg0, 156.0f, (&D_800A0818[isT0])[4], 4, 0, (&D_800A0818[isT0])[2], 0xFF, 0, 0, 0);
        } else if ((D_800CC268 & 1) && (D_800CC2D0[arg0->unk124].unk65 == 0) && (arg0->unk25C & 0x400)) {
            arg0->unk124 = 0;
            func_1505327C(arg0, 50.0f, 3.0f, (&D_800A0818[isT0])[0], (&D_800A0818[isT0])[4]);
        }
    }
    turn = 0;
    if (((isT0) && (arg0->xz_velocity > 5.0f)) || ((!isT0) && (arg0->unk84.uh == 0xA))) {
        turn = arg0->unk76 - arg0->unk78;
        if (turn > 0x3000) {
            turn = 0x3000;
        }
        if (turn < -0x3000) {
            turn = -0x3000;
        }
        if ((isT0) && (arg0->unk13C == 0)) {
            turn >>= 1;
        }
    }
    arg0->unkC4 += ((turn * 0.004999999888241291f) - arg0->unkC4) * 0.15000000596046448f;
    if (isT0) {
        arg0->unk2E4 += ((s16)((sp40 - arg0->unk7A) * 4) - (s16)arg0->unk2E4) / 8;
        arg0->unk2E8 += (-turn - arg0->unk2E8) / 8;
    }
    if ((D_800BE616 != 0) && (D_800CC268 != 0)) {
        obj = arg0->unk31C;
        if ((obj != NULL) && (*((u8 *)obj + 0x1AC) != 0)) {
            func_15063168(arg0);
        }
    }
}
