#include <ultra64.h>
/* functions.h declares func_150A29C8(u8, u16); golden reads both args as full words. */
#define func_150A29C8 func_150A29C8_hdr
#include "functions.h"
#undef func_150A29C8
#define D_800D3098 D_800D3098_array_decl_in_variables_h
#include "variables.h"
#undef D_800D3098

extern f32 D_8009F5A0;
extern f32 D_8009F5A4;

struct Func150A0D14Arg {
    u8 pad0[0xC];
    f32 unkC;
    f32 unk10;
    u8 pad14[0x10];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
};

/* Partial view of struct108 (the camera record) covering the target fields. */
struct Func150A09D0Camera {
    u8  pad0[0x938];
    f32 unk938;
    f32 unk93C;
    f32 unk940;
    f32 unk944;
    u8  unk948;
    u8  unk949;
    u8  pad94A[0x56];
};

/* struct178 with the fields past 0x6 named (structs.h has them as a blob). */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8  padA[0x6];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8  unk14;
    /* 0x15 */ u8  unk15;
    /* 0x16 */ u8  pad16;
    /* 0x17 */ u8  unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ u8  pad24[0x10];
} ObjRec; /* size 0x34 */

/* variables.h declares D_800D3098 as `struct178 D_800D3098[73]`; the symbol really
   holds a POINTER to the object table.  Shadowed above so this file sees the real type. */
extern ObjRec *D_800D3098;

void func_1000FC18(u16 arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4);
s32 func_150A1DA0(u8 *arg0, struct178 *arg1, s32 arg2);
void func_150A23E4(struct178 *arg0);

void func_150A09D0(s32 arg0) {
    u32 count;
    s32 i;

    arg0 = arg0;
    for (i = 0; i < (u32)D_800D3094; i++) {
        switch ((s32)D_800D3098[i].unk15 >> 2) {
        case 2:
            if (D_800D3098[i].unk20 == 1) {
                D_800D3098[i].unk20 ^= 3;
            } else if (D_800D3098[i].unk20 == 2) {
                func_1000FC18(D_800D3098[i].unk1C >> 16,
                              D_800D3098[i].unk0,
                              D_800D3098[i].unk2,
                              D_800D3098[i].unk4,
                              D_800D3098[i].unk18);
                D_800D3098[i].unk20 = 0;
            }
            if (D_800C35EA == 1) {
                if (func_150A1DA0((u8 *)D_800CC2D0, (struct178 *)&D_800D3098[i], 0) == 0) {
                    func_150A23E4((struct178 *)&D_800D3098[i]);
                }
            }
            break;
        case 7:
            if ((D_800D3098[i].unk15 & 3) == 3) {
                D_800D3098[i].unk20 = 0;
            } else {
                if (D_800D3098[i].unk20 == 1) {
                    D_800D3098[i].unk20 = 2;
                } else if (D_800D3098[i].unk20 == 2) {
                    if (func_1000E46C(D_800D3098[i].unk17, 0, D_800D3098[i].unk18, 0)) {
                        if (func_1000E46C(D_800D3098[i].unk17, 100, D_800D3098[i].unk1C, 0)) {
                            D_800D3098[i].unk20 = 0;
                        }
                    }
                }
            }
            break;
        case 4:
            if (D_800D3098[i].unk17 != 9) {
                count = D_800D3098[i].unk1C & 0xFF;
                if (count != 0) {
                    if (D_800D3098[i].unk14 == 0) {
                        count--;
                    } else {
                        count = 0;
                    }
                    D_800D3098[i].unk1C = (D_800D3098[i].unk1C & ~0xFF) | count;
                    if (count == 0) {
                        count = D_800D3098[i].unk1C;
                        D_800DDF5C[(count >> 8) & 0xFF] = 0;
                        D_800D3098[i].unk1C &= 0xFFFF0000;
                    }
                }
            }
            /* fallthrough */
        case 0:
            if (D_800D3098[i].unk17 == 0xC) {
                if (D_800D3098[i].unk14 == 0) {
                    s32 j;

                    for (j = 0; j <= D_80082FA0; j++) {
                        struct Func150A09D0Camera *cam;

                        cam = (struct Func150A09D0Camera *)(D_800DBFF0 + j);
                        cam->unk93C = (f32)D_800D3098[i].unk0;
                        cam->unk940 = (f32)D_800D3098[i].unk2;
                        cam->unk944 = (f32)D_800D3098[i].unk4;
                        cam->unk938 = (f32)D_800D3098[i].unk6;
                        cam->unk949 = D_800D3098[i].unk18;
                        cam->unk948 = 1;
                    }
                }
            }
            break;
        }
    }
}

void func_150A0D14(struct Func150A0D14Arg *arg0) {
    f32 angle;

    angle = arg0->unkC * D_8009F5A0;
    arg0->unk24 = func_150AD78C(angle);
    arg0->unk28 = func_150AD780(angle);
    angle = arg0->unk10 * D_8009F5A4;
    arg0->unk2C = func_150AD78C(angle);
    arg0->unk30 = func_150AD780(angle);
}

void func_15035808(s32, s32, f32, f32, f32, f32, f32, f32, f32, f32);
s32 func_150AD9A0(s16, s16, s16);

void func_150A0D8C(void) {
    u32 i;
    s32 j;
    struct127 *obj;
    s16 dy;
    f32 fade;

    for (i = 0; i < (u32)D_800D3094; i++) {
        if (D_800D3098[i].unk14 == 0 && ((s32)D_800D3098[i].unk15 >> 2) == 6) {
            for (j = 0; j < 25; j++) {
                obj = &D_800CC2D0[j];
                if (obj->interaction_state == 0) continue;
                if (obj->unk5 != 0 && obj->unk5 != 1) continue;
                if (func_150A1DA0((u8 *)obj, (struct178 *)&D_800D3098[i], 1) != 0) continue;
                {
                    dy = obj->y_position - D_800D3098[i].unk2;
                    if (D_800D3098[i].unk17 == 0) {
                        if (func_150AD9A0((s16)(obj->x_position - D_800D3098[i].unk0), dy,
                                          (s16)(obj->z_position - D_800D3098[i].unk4)) < D_800D3098[i].unk6) {
                            fade = (f32)D_800D3098[i].unk6 * 0.200000003f;
                            if (fade > 100.0f) {
                                fade = 100.0f;
                            }
                            func_15035808(2, j, D_800D3098[i].unk0, D_800D3098[i].unk2, D_800D3098[i].unk4,
                                          0.0f, D_800D3098[i].unk10, (f32)D_800D3098[i].unk6 - fade, (f32)D_800D3098[i].unk6, 255.0f);
                        }
                    } else {
                        func_15035808(1, j, obj->x_position, obj->unk180, obj->z_position,
                                      0.0f, 0.0f, 100.0f, 150.0f, 255.0f);
                    }
                }
            }
        }
    }
}

s32 func_150A1040(s32 arg0) {
    return arg0 + 0x400;
}

struct Func150A104CArg {
    u8 pad0[0x84];
    u32 unk84;
    u8 pad88[0x2F8 - 0x88];
    f32 unk2F8;
    f32 unk2FC;
    f32 unk300;
    f32 unk304;
    f32 unk308;
    f32 unk30C;
    u8 pad310[0x364 - 0x310];
    f32 unk364;
};

void func_150A104C(struct Func150A104CArg *arg0, ObjRec **arg1, s32 arg2)
{
  u8 sp4C[0x32C];
  *((f32 *) (&sp4C[0x14])) = arg0->unk2F8;
  *((f32 *) (&sp4C[0x18])) = arg0->unk2FC;
  *((f32 *) (&sp4C[((((((((((0x1C & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu])) = arg0->unk300;
  *((f32 *) (&sp4C[0x180])) = *((f32 *) (&sp4C[0x18]));
  *((f32 *) (&sp4C[0x2C])) = arg0->unk304;
  *((f32 *) (&sp4C[0x30])) = arg0->unk308;
  *((f32 *) (&sp4C[0x34])) = arg0->unk30C;
  arg0->unk364 = -10000.0f;
  arg0->unk84 &= ~0x40000;
  while (arg2 != 0)
  {
    if (!arg1)
    {
    }
    if (((*arg1) != 0) && (func_150A1DA0(sp4C, (struct178 *) (*arg1), 0) == 0))
    {
      if ((((s32) (*arg1)->unk15) >> 2) == 1)
      {
        switch ((*arg1)->unk17)
        {
          case 2:
            arg0->unk364 = (*arg1)->unk2 + (*((s16 *) (((u8 *) (*arg1)) + 8)));
            break;

          case 0:
            arg0->unk84 |= 0x40000;
            break;

          case 6:
            arg0->unk84 &= ~0x40000;
            break;

        }

      }
    }
    arg2--;
    arg1++;
  }

}

typedef struct {
    u64 w[3];
} Mask150A;

typedef struct {
    /* 0x00 */ u64 mask[3];
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 pad19;
    /* 0x1A */ s16 xmin;
    /* 0x1C */ s16 xmax;
    /* 0x1E */ s16 ymin;
    /* 0x20 */ s16 ymax;
    /* 0x22 */ s16 zmin;
    /* 0x24 */ s16 zmax;
    /* 0x26 */ u8 pad26[2];
} Region150A; /* size 0x28 */

extern ObjRec **D_800D3270;
extern s32 D_800D3274;
extern s32 D_800D3278;
extern Region150A *D_800D309C;
extern u64 D_800D30A0[3];
extern u64 D_800D30B8[3];
extern u64 D_800D30F0[][3];
extern u16 D_800D30D0[16];
extern u8 D_800DF0E0;
extern u8 D_800DF0E1;

void func_150806A8(s32);
void func_150829D8(struct127 *);
void func_1512D560(struct108 *, s32, ObjRec *);
void func_15055C88(struct127 *arg0);
void func_1000D96C(s32 arg0, s32 arg1, s32 arg2);
void func_15080500(struct127 *, ObjRec *, s32, s32, s32);
void func_151AE06C(struct127 *, ObjRec *);
void func_1501D348(s32, s32, s32, s32, s32);
s32 __osPiGetAccess(void);
void __osPiRelAccess(void);
s32 osPiRawReadIo(u32, u32 *);
void func_150A24C0(ObjRec *, s32);
void func_15172E7C(u8 *arg0, s32 arg1, s32 arg2);
void func_150A25D4(u8 *arg0, u8 *arg1, s32 arg2, s64 arg3);

void func_150A11C4(struct127 *arg0) {
    u32 i;
    u32 base;
    u64 *p;
    u64 *q; /* also holds the player y (s16) until the region scan: one variable, one register */
    u32 minval;
    s32 sound;
    s32 x;
    s32 y; /* unused (y lives in q); keeps the -g3 frame layout */
    s16 z;
    struct108 *cam;
    Region150A *r;
    Region150A **tbl; /* unused; keeps the -g3 frame layout */
    u64 mask[3];
    s32 found;
    s32 k;
    u64 bits;
    u64 bits2;
    s32 idx;

    minval = 1000;
    sound = arg0->unk127;
    cam = arg0->camera;
    if (cam == NULL) {
        sound = 0xFF;
    } else if (D_800D3274 != 0) {
        func_150A104C((struct Func150A104CArg *)cam, D_800D3270, D_800D3274);
    }
    x = (s16)(s32)arg0->x_position;
    q = (u64 *)(s16)(s32)arg0->y_position;
    z = (s32)arg0->z_position;
    if (arg0->unk127 != 0xFF) {
        func_150806A8(arg0->unk127);
    }
    func_150829D8(arg0);
    *(s16 *)&arg0->unk120 = -10000;
    D_800D3278 = 0;
    idx = arg0 - D_800CC2D0;
    found = 0;
    if (idx < 16) {
        if (D_800D30D0[idx] > 0) {
            r = &D_800D309C[D_800D30D0[idx] - 1];
            if (r->xmin < x && x < r->xmax && r->ymin < (s32)q && (s32)q < r->ymax && r->zmin < z && z < r->zmax) {
                for (i = 0; i < 3; i++) {
                    mask[i] = r->mask[i];
                }
                found = 1;
            }
        }
        D_800D30D0[idx] = 0;
    }
    if (!found) {
        for (i = 0; i < 3; i++) {
            mask[i] = D_800D30B8[i];
        }
    }
    p = mask;
    bits = *p;
    for (i = 0, base = 0; i < (u32)D_800D3094; i++) {
        if (bits == 0) {
            base += 64;
            i = base;
            bits = *++p;
            if (i >= (u32)D_800D3094) {
                continue;
            }
        }
        if (bits & 1) {
            r = &D_800D309C[i];
            if (r->xmin < x && x < r->xmax && r->ymin < (s32)q && (s32)q < r->ymax && r->zmin < z && z < r->zmax) {
                for (k = 0; k < 3; k++) {
                    mask[k] &= r->mask[k];
                }
                if (idx < 16 && r->unk18 < minval) {
                    D_800D30D0[idx] = i + 1;
                    minval = r->unk18;
                }
            } else {
                *p ^= 1LL << (i - base);
            }
        }
        bits >>= 1;
    }
    p = mask;
    base = 0;
    for (i = 0; i < 3; i++) {
        mask[i] |= D_800D30A0[i];
    }
    if (idx < 16) {
        q = D_800D30F0[idx];
        bits2 = *q;
        *q = 0;
    } else {
        q = NULL;
    }
    bits = *p;
    for (i = 0; i < (u32)D_800D3094; i++) {
        if (bits == 0) {
            base += 64;
            i = base;
            if (i >= (u32)D_800D3094) {
                continue;
            }
            bits = *++p;
            if (q != NULL) {
                q++;
                bits2 = *q;
                *q = 0;
            }
        }
        if (bits & 1) {
            if (func_150A1DA0((u8 *)arg0, (struct178 *)&D_800D3098[i], 0) == 0) {
                switch (D_800D3098[i].unk15 >> 2) {
                case 0:
                    if (cam != NULL && D_800D3098[i].unk17 != 0xC) {
                        func_1512D560(cam, 1, &D_800D3098[i]);
                        *(s16 *)((u8 *)cam + 0x700) = i;
                    }
                    break;
                case 1:
                    switch (D_800D3098[i].unk17) {
                    case 0:
                        arg0->unkF8 &= ~4;
                        break;
                    case 1:
                        arg0->unkF8 &= ~2;
                        break;
                    case 2:
                        *(s16 *)&arg0->unk120 = D_800D3098[i].unk2 + D_800D3098[i].unk8;
                        break;
                    case 3:
                        arg0->unkF8 |= 2;
                        break;
                    case 4:
                        func_15055C88(arg0);
                        break;
                    case 5: {
                        u16 angle;
                        s32 flag;
                        f32 height;

                        if (*((u8 *)arg0->unk31C + 0x95) == 0 && arg0->stunned == 0 && arg0->health != 0 &&
                            arg0->disable_run == 0 &&
                            (*((u8 *)arg0->unk31C + 0x11A) == 0 || D_800BE9F0 == 6) && arg0->unk44 > 8.0f) {
                            height = D_800D3098[i].unk2 + D_800D3098[i].unk8;
                            flag = 0;
                            angle = (u32)(D_800D3098[i].unk10 * 182.0444489f);
                            angle -= *(u16 *)((u8 *)arg0->unk31C + 0x4C);
                            if (height < arg0->y_position + 250.0f && arg0->unk28 == 0.0f) {
                                angle -= 0x8000;
                                flag = 1;
                            }
                            if (angle > 0x2000 && angle < 0x6000) {
                                *((u8 *)arg0->unk31C + 0x97) = flag;
                                *((u8 *)arg0->unk31C + 0x96) = 0;
                                arg0->disable_jump = 0x12;
                                *((u8 *)arg0->unk31C + 0x95) = i + 1;
                                if (D_800D3098[i].unk18 != 0) {
                                    *((u8 *)arg0->unk31C + 0x95) |= 0x80;
                                }
                            }
                        }
                    }
                        break;
                    case 6:
                        arg0->unkF8 |= 4;
                        break;
                    case 7:
                        arg0->unkF4 |= 0x100000;
                        break;
                    case 8:
                        if (D_800D3098[i].unk18 != 0) {
                            D_800DF0E1 |= 1 << idx;
                        } else {
                            D_800DF0E0 |= 1 << idx;
                        }
                        break;
                    case 9:
                        arg0->unkF8 |= 0x8000000;
                        break;
                    }
                    break;
                case 2:
                    func_150A23E4((struct178 *)&D_800D3098[i]);
                    break;
                case 7:
                    if (D_800D3098[i].unk20 == 0) {
                        if (func_1000E46C(D_800D3098[i].unk17, 100, D_800D3098[i].unk18, 0)) {
                            if (func_1000E46C(D_800D3098[i].unk17, 0, D_800D3098[i].unk1C, 0)) {
                                D_800D3098[i].unk20 = 1;
                            }
                        }
                    } else {
                        D_800D3098[i].unk20 = 1;
                    }
                    break;
                case 14:
                    if (sound != 0xFF && !(bits2 & 1)) {
                        func_1000D96C(D_800D3098[i].unk17, D_800D3098[i].unk18, D_800D3098[i].unk1C & 0xFF);
                        if (D_800D3098[i].unk17) {
                            func_1000E46C(D_800D3098[i].unk17, 0, D_800D3098[i].unk20 & 0xFFFF, 0);
                        }
                    }
                    break;
                case 25:
                    if (D_800D3098[i].unk17 == 0) {
                        D_80041FD8 = D_800D3098[i].unk18;
                    }
                    break;
                case 4:
                    func_150A24C0(&D_800D3098[i], sound);
                    break;
                case 8: {
                    u32 raw;

                    __osPiGetAccess();
                    osPiRawReadIo(func_150A1040(0xB0000054), &raw);
                    __osPiRelAccess();
                    if (func_150A1040(raw) == 0x1090C2B && D_800C35EA == 0 && arg0->health != 0) {
                        func_15172E7C((u8 *)arg0, D_800D3098[i].unk17, D_800D3098[i].unk18);
                        if (D_800BE9F0 != D_800D3098[i].unk17) {
                            arg0->immune = 0xFF;
                            arg0->unk1CC = arg0->y_position;
                        }
                    }
                }
                    break;
                case 11:
                    func_15080500(arg0, &D_800D3098[i], 1, D_800D3098[i].unk17, D_800D3098[i].unk18);
                    break;
                case 12:
                    func_150A25D4((u8 *)&D_800D3098[i], (u8 *)arg0, sound, bits2);
                    break;
                case 3:
                    if (D_800D3098[i].unk17 == 5) {
                        func_151AE06C(arg0, &D_800D3098[i]);
                    }
                    break;
                case 13:
                    if (sound != 0xFF && !(bits2 & 1) && D_800E0B94 == 0 && arg0->health != 0 && D_800C35EA == 0) {
                        if (D_800D3098[i].unk17 == 0) {
                            D_800D3098[i].unk14 = 1;
                        }
                        func_1501D348(D_800BE9F0, D_800D3098[i].unk18, 0, 0, 0);
                        D_800D3098[i].unk1C++;
                    }
                    break;
                }
                if (q != NULL) {
                    *q |= 1LL << (i - base);
                }
            }
        }
        bits >>= 1;
        bits2 >>= 1;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_CDE80/func_150A1DA0.s")

void func_150A23E4(struct178 *arg0) {
    s32 var_t0;
    s32 var_v1;
    u8 temp_t1;
    s32 temp_t3;
    s32 temp_t4;
    u32 temp_v0;

    if (*(s32 *)&arg0->unk6[0x1A] == 0) {
        var_v1 = 0;
        var_t0 = 8;
        if ((arg0->unk6[0xF] & 3) == 0) {
            var_v1 = 0x8000;
        }
        temp_t1 = arg0->unk6[0x11];
        temp_t3 = temp_t1 & 0x7F;
        if ((s32)temp_t1 >> 7) {
            var_t0 = 0x108;
        }
        temp_t4 = temp_t3 * 327;
        temp_v0 = *(u32 *)&arg0->unk6[0x16];
        func_1000FA64(temp_v0 >> 16, arg0->unk0, arg0->pad2, arg0->unk4, temp_t4,
            *(s32 *)&arg0->unk6[0x12] | var_v1, temp_v0 & 0xFFFF, 0, 0, 0, var_t0, 0);
        *(s32 *)&arg0->unk6[0x1A] = 1;
trailing_label:
        ;
        return;
    }
    *(s32 *)&arg0->unk6[0x1A] = 1;
    return;
}

s32 func_15183290(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8  pad8[0xF];
    /* 0x17 */ u8  unk17;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 unk1F;
    /* 0x20 */ u32 unk20;
} ObjRecB;

void func_150A24C0(ObjRec *arg0, s32 arg1) {
    s32 temp_v0;
    s32 var_v0;
    s32 type;
    s32 sub;
    s32 posY;
    s32 b1A;
    s32 b1C;
    s32 b1D;
    s32 w20;

    if (arg1 != 0xFF) {
        if (((arg0->unk1C & 0xFF) == 0) || (arg0->unk17 == 9)) {
            type = arg0->unk17;
            sub = arg0->unk18 & 0xFF;
            posY = ((ObjRecB *)arg0)->unk18;
            b1A = ((ObjRecB *)arg0)->unk1A;
            b1C = ((ObjRecB *)arg0)->unk1C;
            b1D = ((ObjRecB *)arg0)->unk1D;
            w20 = arg0->unk20;
            temp_v0 = func_15183290(arg0->unk0, arg0->unk2 + (b1D * 0x10), arg0->unk4,
                                    sub, type, arg0->unk6, arg1,
                                    posY, b1A, b1C, w20);
            if ((temp_v0 != -1) && (arg0->unk17 != 9)) {
                arg0->unk1C = (temp_v0 << 8) | 0xF0 | (arg0->unk1C & 0xFFFF0000);
            }
        } else {
            var_v0 = (arg0->unk1C & 0xFF);
            if (var_v0 < 2) {
                var_v0 = 2;
            }
            arg0->unk1C = (arg0->unk1C & ~0xFF) | var_v0;
        }
    }
}

extern u8 D_800D3090;
extern u8 D_800D9EB8[][3];
void func_150E7290(s32 arg0, s32 arg1, s32 arg2);
void func_150E68B0(u8 *arg0, s32 arg1, u8 *arg2, u8 arg3);
void func_151CE51C(u8 *arg0, u8 *arg1);

void func_150A25D4(u8 *arg0, u8 *arg1, s32 arg2, s64 arg3) {
    switch (arg0[0x17]) {
    case 0:
        break;
    case 1:
        if (arg2 != 0xFF) {
            func_150E7290(arg2, 0xFF, 0);
        }
        break;
    case 2:
        if (arg2 != 0xFF) {
            D_800D9EB8[arg2][0] = *(s32 *)(arg0 + 0x18);
            D_800D9EB8[arg2][1] = *(s32 *)(arg0 + 0x1C);
            D_800D9EB8[arg2][2] = *(s32 *)(arg0 + 0x20);
        }
        break;
    case 3:
        if (arg2 != 0xFF) {
            func_150E68B0(arg1, arg2, arg0, arg0[0x1B]);
        }
        break;
    case 6: {
        u8 *p = &D_800D3090;
        if (!(arg3 & 1)) {
            *p = 30;
            func_10010344(0x66F, arg1, 0x5DC0, 0x190, 0x1F4);
        }
        *p += D_800BE9E4;
        if (*p >= 31) {
            if (*(u16 *)(arg1 + 0xB2) > 0x5A0) {
                *(u16 *)(arg1 + 0xB2) = 0x4B0;
            } else if (*(u16 *)(arg1 + 0xB2) < 0xF1) {
                *(u16 *)(arg1 + 0xB2) = 0x5A;
            } else {
                *(u16 *)(arg1 + 0xB2) -= 0xF0;
            }
            *p = 0;
        }
    }
        break;
    case 8:
        func_151C8674((s32)arg1, (s32)arg0);
        break;
    case 9:
        func_1515F270((s32)arg1, (s32 *)arg0);
        break;
    case 10:
        func_151CE51C(arg1, arg0);
        break;
    }
}

s32 func_150A1DA0(u8 *arg0, struct178 *arg1, s32 arg2);
void func_15172E7C(u8 *arg0, s32 arg1, s32 arg2);

void func_150A278C(u8 *arg0) {
    struct178 *base;
    s32 i;

    if (arg0[0x65] != 0) {
        return;
    }
    for (i = 0; i < (u32)D_800D3094; i++) {
        struct178 *p;
        base = (struct178 *)*(s32 *)&D_800D3098;
        if (8 == (s32)base[i].unk6[0xF] >> 2) {
            if (func_150A1DA0(arg0, &base[i], 0) == 0) {
                p = (struct178 *)*(s32 *)&D_800D3098 + i;
                func_15172E7C(arg0, p->unk6[0x11], *(s32 *)&p->unk6[0x12]);
            }
        }
    }
}

extern u64 D_800D30F0[][3];

void func_150A2864(u32 arg0, s32 arg1) {
    s32 i;

    arg0 &= 0xFFF;
    if (arg0 < (u32)D_800D3094) {
        (*(ObjRec **)&D_800D3098)[arg0].unk14 = arg1;
    }
    if (arg1 == 1) {
        for (i = 0; i < 0x10; i++) {
            D_800D30F0[i][arg0 >> 6] &= ~(1ULL << (arg0 & 0x3F));
        }
    }
}


void func_150A2940(s32 arg0, s32 arg1, s32 arg2) {
    typedef u8 entry[0x34];

    arg0 &= 0xFFF;
    if ((u32)arg0 < (u32)D_800D3094) {
        if (arg1 != -1) {
            *(s32 *)&(*(entry **)&D_800D3098)[arg0][0x18] = arg1;
        }
        if (arg2 != -1) {
            (*(entry **)&D_800D3098)[arg0][0x17] = arg2;
        }
        *(s32 *)&(*(entry **)&D_800D3098)[arg0][0x1C] = 0;
    }
}

s32 func_150A29C8(u32 arg0, u32 arg1) {
    arg1 &= 0xFFF;
    if ((u32)D_800D3094 < arg1) {
        return 1;
    }
    if (arg0 < 0x10 && (*(u16 *)&D_800D2340 & (1 << arg0))) {
        if ((D_800D30F0[arg0][arg1 >> 6] >> (arg1 & 0x3F)) & 1) {
            return 0;
        }
        return 1;
    }
    return func_150A1DA0((u8 *)&D_800CC2D0[arg0], (struct178 *)&D_800D3098[arg1], 1);
}

s32 func_150A2AEC(u32 arg0, s32 arg1, s32 *arg2) {
    s32 i;
    u32 id;

    if (arg0 < 0x10 && (*(u16 *)&D_800D2340 & (1 << arg0))) {
        for (i = 0; i < arg1; i++) {
            id = arg2[i];
            id &= 0xFFF;
            if (id < (u32)D_800D3094) {
                if ((D_800D30F0[arg0][id >> 6] >> (id & 0x3F)) & 1) {
                    return id;
                }
            }
        }
    } else {
        for (i = 0; i < arg1; i++) {
            id = arg2[i];
            id &= 0xFFF;
            if (id < (u32)D_800D3094) {
                if (func_150A1DA0((u8 *)&D_800CC2D0[arg0], (struct178 *)&D_800D3098[id], 1) == 0) {
                    return id;
                }
            }
        }
    }
    return -1;
}

extern u64 D_800D30F0[][3];

s32 func_150A2CA4(u32 arg0) {
    s32 i;

    arg0 &= 0xFFF;
    if ((u32)D_800D3094 < arg0) {
        return -1;
    }
    for (i = 0; i != 0x10; i++) {
        if (*(u16 *)((u8 *)&D_800D2138 + 0x208) & (1 << i)) {
            if ((D_800D30F0[i][arg0 >> 6] >> (arg0 & 0x3F)) & 1) {
                return i;
            }
        }
    }
    return -1;
}

s32 func_150A2D84(s32 a0, s32 a1) {
    struct108 *p;
    u8 sp18[0x32C];

    p = &D_800DBFF0[a0];
    a1 &= 0xFFF;
    if ((u32)D_800D3094 < (u32)a1) {
        return 1;
    }
    *(f32 *)&sp18[0x14] = p->unk2F8;
    *(f32 *)&sp18[0x18] = p->unk2FC;
    *(f32 *)&sp18[0x1C] = p->unk300;
    *(f32 *)&sp18[0x180] = *(f32 *)&sp18[0x18];
    *(f32 *)&sp18[0x2C] = p->unk304;
    *(f32 *)&sp18[0x30] = p->unk308;
    *(f32 *)&sp18[0x34] = p->unk30C;
    return func_150A1DA0(sp18, (struct178 *)&D_800D3098[a1], 0);
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_CDE80/func_150A2E4C.s")

s32 func_150A2E4C(struct178 *arg0, f32 x, f32 y, f32 z);

s32 func_150A2EE4(s32 a0, s32 a1) {
    struct178 *base;
    s32 ret;

    a1 &= 0xFFF;
    if ((u32)D_800D3094 < (u32)a1) {
        return 1;
    }
    base = (struct178 *)*(s32 *)&D_800D3098;
    ret = 0;
    if ((base[a1].unk6[0xF] & 3) == 3) {
        return func_150A2E4C(base + a1, D_800CC2D0[a0].x_position, D_800CC2D0[a0].y_position, D_800CC2D0[a0].z_position);
    }
    return ret;
}

s32 func_150A2FA4(s32 a0, s32 a1) {
    struct178 *base;
    struct108 *p;
    s32 ret;

    p = &D_800DBFF0[a0];
    a1 &= 0xFFF;
    if ((u32)D_800D3094 < (u32)a1) {
        return 1;
    }
    ret = 0;
    if (((base = (struct178 *)*(s32 *)&D_800D3098)[a1].unk6[0xF] & 3) == 3) {
        return func_150A2E4C(base + a1, p->unk2F8, p->unk2FC, p->unk300);
    }
    return ret;
}

s32 func_150A3058(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    u8 pad[4];
    u8 sp48[0x32C];
    struct178 *base;
    s32 i;

    *(f32 *)&sp48[0x14] = (f32)arg1;
    *(f32 *)&sp48[0x2C] = (f32)arg1;
    *(f32 *)&sp48[0x18] = (f32)arg2;
    *(f32 *)&sp48[0x180] = (f32)arg2;
    *(f32 *)&sp48[0x30] = (f32)arg2;
    *(f32 *)&sp48[0x1C] = (f32)arg3;
    *(f32 *)&sp48[0x34] = (f32)arg3;

    for (i = 0; i < (u32)D_800D3094; i++) {
        base = (struct178 *)*(s32 *)&D_800D3098;
        if (base[i].unk6[0xE] == 0) {
            if (3 == ((s32)base[i].unk6[0xF] >> 2)) {
                if (base[i].unk6[0x11] == arg0) {
                    if (func_150A1DA0(sp48, &base[i], 0) == 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

ObjRec *func_150A3194(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 i;
    struct127 actor;

    actor.x_position = (f32)arg2;
    actor.old_x_position = (f32)arg2;
    actor.y_position = (f32)arg3;
    actor.unk180 = (f32)arg3;
    actor.old_y_position = (f32)arg3;
    actor.z_position = (f32)arg4;
    actor.old_z_position = (f32)arg4;

    for (i = 0; i < (u32)D_800D3094; i++) {
        if (D_800D3098[i].unk14 == 0) {
            if (arg0 == ((s32)D_800D3098[i].unk15 >> 2)) {
                if (arg1 == D_800D3098[i].unk17) {
                    if (func_150A1DA0((u8 *)&actor, (struct178 *)&D_800D3098[i], 0) == 0) {
                        return &D_800D3098[i];
                    }
                }
            }
        }
    }
    return 0;
}

s32 func_150A32B4(struct178 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 sp24[0x32C];
    u8 pad[8];

    *(f32 *)&sp24[0x14] = (f32)arg1;
    *(f32 *)&sp24[0x2C] = (f32)arg1;
    *(f32 *)&sp24[0x18] = (f32)arg2;
    *(f32 *)&sp24[0x180] = (f32)arg2;
    *(f32 *)&sp24[0x30] = (f32)arg2;
    *(f32 *)&sp24[0x1C] = (f32)arg3;
    *(f32 *)&sp24[0x34] = (f32)arg3;
    if (func_150A1DA0(sp24, arg0, 0) == 0) {
        return 1;
    }
    return 0;
}

void func_150A3330(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    typedef u8 entry[0x34];

    *arg1 = (*(entry **)&D_800D3098)[arg0][0x17];
    *arg2 = *(s32 *)&(*(entry **)&D_800D3098)[arg0][0x18];
    *arg3 = *(s32 *)&(*(entry **)&D_800D3098)[arg0][0x1C];
    *arg4 = *(s32 *)&(*(entry **)&D_800D3098)[arg0][0x20];
}

void func_150A3398(s32 arg0, u16 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    typedef u8 entry[0x34];

    if (arg1 & 1) {
        (*(entry **)&D_800D3098)[arg0][0x17] = arg2;
    }
    if (arg1 & 2) {
        *(s32 *)&(*(entry **)&D_800D3098)[arg0][0x18] = arg3;
    }
    if (arg1 & 4) {
        *(s32 *)&(*(entry **)&D_800D3098)[arg0][0x1C] = arg4;
    }
    if (arg1 & 8) {
        *(s32 *)&(*(entry **)&D_800D3098)[arg0][0x20] = arg5;
    }
}

void func_150A3444(s32 a0, s16 a1, s16 a2, s16 a3) {
    ((struct178 *)*(s32 *)&D_800D3098)[a0].unk0 = a1;
    *(s16 *)&((struct178 *)*(s32 *)&D_800D3098)[a0].pad2 = a2;
    ((struct178 *)*(s32 *)&D_800D3098)[a0].unk4 = a3;
}

s32 func_150A3504();
s32 func_150A34B0(u8 *arg0);

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 padA;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8 pad14[0x10];
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
} Cylinder;

extern f32 D_8009F674;
extern f32 D_8009F678;
s32 func_150A3504(Cylinder *cyl, f32 *p0, f32 *p1, f32 *out);
