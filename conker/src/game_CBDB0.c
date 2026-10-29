#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern struct127 *func_1505EEF4();

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0x38 - 0x16];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 pad40[0x4F - 0x40];
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 pad50[0x5A - 0x50];
    /* 0x5A */ s16 unk5A;
    /* 0x5C */ s16 unk5C;
    /* 0x5E */ s16 unk5E;
    /* 0x60 */ f32 unk60;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ f32 unk68;
    /* 0x6C */ u8 pad6C[2];
    /* 0x6E */ u8 unk6E;
    /* 0x6F */ u8 unk6F;
    /* 0x70 */ u8 pad70[2];
    /* 0x72 */ u8 unk72;
    /* 0x73 */ u8 unk73;
    /* 0x74 */ u8 pad74[4];
    /* 0x78 */ s32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ f32 unk80;
    /* 0x84 */ f32 unk84;
    /* 0x88 */ s16 unk88;
    /* 0x8A */ u8 unk8A;
    /* 0x8B */ u8 pad8B[0x92 - 0x8B];
    /* 0x92 */ u8 unk92;
} Struct1509E900;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Struct80088C90;

extern s32 func_151149AC(u32);
extern s32 func_15086D48(s32);
extern s32 func_15086CBC(s32, f32 *, f32 *, f32 *);
extern void func_150E4928(void *);
extern void func_15173C90(s32, s32, s32);
extern Struct80088C90 D_80088C90[];

s32 func_1509E900(s32 arg0, s32 arg1, s32 *arg2) {
    Struct1509E900 *obj;
    f32 x;
    f32 y;
    f32 z;
    s32 idx;
    f32 f;
    s32 pad;

    obj = (Struct1509E900 *)func_151149AC(arg0 & 0xFF);
    if (obj == NULL) {
        return 0;
    }
    switch (arg1) {
    case 0:
        obj->unk6E = 0;
        if (obj->unk38 != 0) {
            obj->unk6F |= 0xF;
        }
        return 1;
    case 1:
        obj->unk6E = 1;
        return 1;
    case 2:
        if (arg2[2] != 0x7FFFFFFF) {
            obj->unk10 = arg2[2];
        }
        if (arg2[3] != 0x7FFFFFFF) {
            obj->unk12 = arg2[3];
        }
        if (arg2[4] != 0x7FFFFFFF) {
            obj->unk14 = arg2[4];
        }
        return 1;
    case 12:
        idx = func_15086D48(arg2[2]);
        if (idx != -1) {
            func_15086CBC(idx, &x, &y, &z);
            obj->unk10 = x;
            obj->unk12 = arg2[3] + (s16)y;
            obj->unk14 = z;
        }
        return 1;
    case 3:
        if ((f32)arg2[2] != 2013265920.0f) {
        lbl_in:
            obj->unk7C = (f32)arg2[2] / 65536.0f;
        }
        if ((f32)arg2[3] != 2013265920.0f) {
            obj->unk80 = (f32)arg2[3] / 65536.0f;
        }
        if ((f32)arg2[4] != 2013265920.0f) {
            obj->unk84 = (f32)arg2[4] / 65536.0f;
        }
        return 1;
    case 4:
        if (arg2[2] != 0x7FFFFFFF) {
            *(s32 *)&obj->unk7C = arg2[2];
        }
        if (arg2[3] != 0x7FFFFFFF) {
            *(s32 *)&obj->unk80 = arg2[3];
        }
        if (arg2[4] != 0x7FFFFFFF) {
            *(s32 *)&obj->unk84 = arg2[4];
        }
        return 1;
    case 5:
        obj->unk3C = arg2[2];
        return 1;
    case 6:
        obj->unk73 &= ~3;
        obj->unk73 |= arg2[2] & 3;
        return 1;
    case 7:
        if (arg2[2] != 0) {
            obj->unk73 |= 4;
        } else {
            obj->unk73 &= ~4;
        }
        return 1;
    case 8:
        obj->unk3C &= 0xFFFF;
        obj->unk3C |= arg2[2] << 16;
        *(s32 *)&obj->unk7C = arg2[3];
        return 1;
    case 9:
        func_150E4928(obj);
        break;
    case 10:
        obj->unk38 = D_80088C90[arg2[2]].unk0;
        obj->unk78 = D_80088C90[arg2[2]].unk4;
        obj->unk92 = D_80088C90[arg2[2]].unk8;
        break;
    case 11:
        func_15173C90(arg2[2], arg2[3], obj->unk72);
        break;
    case 13:
        obj->unk4F = (obj->unk4F & ~arg2[3]) | arg2[2];
        break;
    case 14:
        if (arg2[2] != 0x7FFFFFFF) {
            obj->unk5A = arg2[2];
        }
        if (arg2[3] != 0x7FFFFFFF) {
            obj->unk5C = arg2[3];
        }
        if (arg2[4] != 0x7FFFFFFF) {
            obj->unk5E = arg2[4];
        }
        break;
    case 15:
        if (arg2[2] != 0x7FFFFFFF) {
            obj->unk60 = arg2[2];
        }
        if (arg2[3] != 0x7FFFFFFF) {
            obj->unk64 = arg2[3];
        }
        if (arg2[4] != 0x7FFFFFFF) {
            obj->unk68 = arg2[4];
        }
        break;
    case 16:
        if (arg2[2] != 0x7FFFFFFF) {
            obj->unk0 = arg2[2];
        }
        if (arg2[3] != 0x7FFFFFFF) {
            obj->unk4 = arg2[3];
        }
        if (arg2[4] != 0x7FFFFFFF) {
            obj->unk8 = arg2[4];
        }
        break;
    case 17:
        obj->unk88 = arg2[2];
        break;
    case 18:
        obj->unk8A = arg2[2];
        break;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_CBDB0/func_1509ED74.s")

extern s32 D_800D3840;
s32 func_1509F6B0();
s32 func_1509F4E8();
s32 func_1509F5A8();
s32 func_1509F6E8();
s32 func_15103C14();
s32 func_1509F354();
s32 func_1509F47C();
void func_1509F5F4();

s32 func_1509EFF0(s32 arg0, u32 *arg1) {
    s32 ret;
    s32 a;
    s32 b;
    s32 c;

    if (D_800D3840 == 1) {
        ret = func_1509F6B0(arg0, 0x7FFF, 0, 0x40);
    } else {
        ret = arg1[1];
        switch (ret) {
        case 1:
            b = 0x40;
            if (D_800D3840 >= 4) {
                a = arg1[3];
                if (D_800D3840 == 5) {
                    b = arg1[4];
                }
            } else {
                a = 0;
            }
            ret = func_1509F6B0(arg0, arg1[2], a, b);
            break;
        case 0:
            ret = arg1[5];
            switch (ret & 0xF000) {
            case 0x2000:
                return func_1509F354(arg0, arg1[2] & 0xFFF, arg1[3], arg1[4], ret & 0xFFF, arg1[6], arg1[7]);
            case 0x3000:
                func_1509F47C(arg0, arg1[2] & 0xFFF, arg1[4], ret & 0xFFF, arg1[6], arg1[7]);
                return 0;
            case 0x4000:
                return func_1509F4E8(arg0, arg1[2] & 0xFFF, arg1[4], ret & 0xFFF, arg1[6]);
            default:
                ret = func_1509F5A8(arg0, arg1[2] & 0xFFF, arg1[4]);
                break;
            }
            break;
        case 2:
            c = 0;
            if (D_800D3840 >= 7 && arg1[6] != 0) {
                c = 1;
            }
            func_1509F5F4(arg0 & 0xFFF, arg1[2], arg1[3] & 0xFFF, arg1[4], arg1[5], c);
            ret = 0;
            break;
        case 3:
            ret = func_1509F6E8(arg0, arg1[3] & 0xFFF, arg1[2], arg1[4], arg1[5]);
            break;
        case 4:
            func_15103C14(arg0, arg1[3] & 0xFFF, arg1[2], arg1[4], arg1[5]);
            return 0x7000;
        default:
            return 0;
        }
    }
    if (ret != 0) {
        ret = (ret << 16) | 0x7000;
    }
    return ret;
}

s32 func_1509F248(s32 arg0, u32 *arg1) {
    if (arg0 != 0) {
        func_100111C8((*arg1 & 0xFFFF0000) >> 16);
    }
    return 1;
}

void func_1509F660(s32 arg0, s32 arg1);
void func_1509F77C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 func_1509F284(s32 arg0, s32 arg1, u32 *arg2) {
    switch (arg1) {
    case 1:
        func_1509F660(arg2[2] & 0xFFF, 0);
        return 1;
    case 0:
        func_1509F660(arg2[2] & 0xFFF, 1);
        return 1;
    case 2:
        func_1509F77C((arg2[0] & 0xFFFF0000) >> 16, arg2[2] & 0xFFF, arg2[3], arg2[4], arg2[5]);
        return 1;
    }
    return 0;
}

s32 func_1509F33C(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
}

s32 func_1509F354(arg0, arg1, arg2, arg3, arg4, arg5, arg6)
    s32 arg0; s32 arg1; s32 arg2; s32 arg3; s32 arg4; s16 arg5; s32 arg6;
{
    struct127 *temp;
    s32 range;
    u8 idx;

    temp = func_1505EEF4(arg4);
    if ((u32)arg3 >= 0x8000U) {
        arg3 = 0x7FFF;
    }
    if (temp == 0) {
        return arg2;
    }
    if (func_10010894(temp) == 0) {
        if (arg2 == 0) {
            arg2 = (u32)-1 >> -(u8)(arg1 - arg0 + 1);
        }
        range = (u8)(arg1 - arg0 + 1);
        do {
            idx = (u8)(func_150ADA20() % (u32)range);
        } while (((1 << idx) & arg2) == 0);

        range = idx;
        if (func_10010344((u16)(idx + arg0), temp, arg3, arg5, arg6) != 0) {
            arg2 ^= 1 << range;
        }
    }
    return arg2;
}

extern s32 func_151149AC(u32);
extern s32 func_1000F568(s32, u8);
extern void func_15114D24(s32, s32, s32, s16, s32, s32);

s32 func_1509F47C(arg0, arg1, arg2, arg3, arg4, arg5)
    s32 arg0; s32 arg1; s32 arg2; s32 arg3; s16 arg4; s32 arg5;
{
    s32 temp;

    temp = func_151149AC(arg3 & 0xFF);
    func_15114D24(temp, func_1000F568(arg0, (u8)(arg1 - arg0 + 1)), arg2, arg4, arg5, 0);
}

void func_10010F88(s32 arg0, u16 arg1, s16 arg2, u8 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 arg9);
extern struct178 D_800D3098[73];
extern s32 func_1000F568(s32, u8);
s32 func_1509F4E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
  struct178 *e;
  s32 h;
  e = &((struct178 *) (*((s32 *) (&D_800D3098))))[0, arg3];
  h = *((s16 *) (&e->unk6[0]));
  if (((u32) arg4) >= ((u32) h))
  {
    arg4 = h - 1;
  }
  func_10010F88(func_1000F568(arg0, (u8) ((arg1 - arg0) + 1)), (u16) arg2, 0, 0, 0, e->unk0, e->pad2, e->unk4, arg4, h);
}

extern s32 func_1000F568(s32, u8);


extern s32 func_1000F568(s32, u8);

s32 func_1509F5A8(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp;

    temp = func_1000F568(arg0, (u8)(arg1 - arg0 + 1));
    func_10010F30(temp, (u16)arg2, 0x40, 0, 0);
}

void func_1509F5F4(arg0, arg1, arg2, arg3, arg4, arg5)
    u16 arg0; s32 arg1; s32 arg2; s16 arg3; s32 arg4; s32 arg5;
{
    struct127 *temp;

    temp = func_1505EEF4(arg2);
    if (temp != 0) {
        if ((arg5 != 0) || (func_10010894(temp) == 0)) {
            func_10010344(arg0, temp, arg1, arg3, arg4);
        }
    }
}

extern struct127 *func_1505EEF4();

void func_1509F660(s32 arg0, s32 arg1) {
    struct127 *temp;

    temp = func_1505EEF4(arg0);
    if (temp != 0) {
        if (arg1 != 0) {
            func_10010A3C(temp);
        } else {
            func_100109D0(temp);
        }
    }
}

s32 func_1509F6B0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_10010F30(arg0, arg1, arg3, arg2, 0);
}

s32 func_1509F6E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    struct127 *temp;
    s32 x;
    s32 y;
    s32 z;

    temp = func_1505EEF4(arg1);
    if (temp == 0) {
        return 0;
    }
    x = temp->x_position;
    y = temp->y_position;
    z = temp->z_position;
    func_10010F88(arg0, (u16)arg2, 0, 0, 0, x, y, z, arg3, arg4);
}

void func_1509F77C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    struct127 *temp;
    s32 x;
    s32 y;
    s32 z;

    temp = func_1505EEF4(arg1);
    x = temp->x_position;
    y = temp->y_position;
    z = temp->z_position;
    func_1000F91C((u16)arg0, 0x7FFF, (s16)arg2, 0, 0,
                  x, y, z, arg3, arg4);
}

s32 func_1509F800(s32 arg0, s32 arg1, s32 arg2) {
    arg0 = arg0;
    arg1 = arg1;
    arg2 = arg2;
    return 0;
}

s32 func_1509F818(s32 arg0, s32 arg1, s32 arg2) {
    func_150AD770();
    return 0;
}
