#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern struct127 *func_1505EEF4();

/* Script operand value meaning "leave this field unchanged". */
#define ARG_KEEP 0x7FFFFFFF

/* A word that holds either a float or the raw value a script stored there. */
typedef union {
    f32 f;
    s32 i;
} FloatOrWord;

/* 0xA0-byte entry of the D_800DBEF4 table, looked up by its id with func_151149AC. */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x38 - 0x16];
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8 pad40[0x4F - 0x40];
    /* 0x4F */ u8 flags4F;
    /* 0x50 */ u8 pad50[0x5A - 0x50];
    /* 0x5A */ s16 unk5A[3];
    /* 0x60 */ f32 unk60[3];
    /* 0x6C */ u8 pad6C[2];
    /* 0x6E */ u8 unk6E;
    /* 0x6F */ u8 unk6F;
    /* 0x70 */ u8 pad70[2];
    /* 0x72 */ u8 id;
    /* 0x73 */ u8 flags73;
    /* 0x74 */ u8 pad74[4];
    /* 0x78 */ s32 unk78;
    /* 0x7C */ FloatOrWord unk7C[3];
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

extern Struct1509E900 *func_151149AC(u8 id);
extern s32 func_15086D48(s32);
extern s32 func_15086CBC(s32, f32 *, f32 *, f32 *);
extern void func_150E4928(Struct1509E900 *);
extern void func_15173C90(s32, s32, s32);
extern Struct80088C90 D_80088C90[];

/* Script "set" handler for the object with id `id`: applies operation `op` with operands cmd[2..]. */
s32 func_1509E900(s32 id, s32 op, s32 *cmd) {
    Struct1509E900 *obj;
    f32 x;
    f32 y;
    f32 z;
    s32 index;
    s32 pad[2]; // matching: unused stack slots (target frame is 0x40)

    obj = func_151149AC(id);
    if (obj == NULL) {
        return 0;
    }
    switch (op) {
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
        if (cmd[2] != ARG_KEEP) {
            obj->x = cmd[2];
        }
        if (cmd[3] != ARG_KEEP) {
            obj->y = cmd[3];
        }
        if (cmd[4] != ARG_KEEP) {
            obj->z = cmd[4];
        }
        return 1;
    case 12:
        index = func_15086D48(cmd[2]);
        if (index != -1) {
            func_15086CBC(index, &x, &y, &z);
            obj->x = x;
            obj->y = cmd[3] + (s16)y;
            obj->z = z;
        }
        return 1;
    case 3:
        /* 16.16 fixed-point operands; 0x78000000 means "unchanged" */
        if ((f32)cmd[2] != 2013265920.0f) {
        unused_label: // matching: a label ends the block, so as1 won't pull the 65536.0f load into a bc1tl slot
            obj->unk7C[0].f = (f32)cmd[2] / 65536.0f;
        }
        if ((f32)cmd[3] != 2013265920.0f) {
            obj->unk7C[1].f = (f32)cmd[3] / 65536.0f;
        }
        if ((f32)cmd[4] != 2013265920.0f) {
            obj->unk7C[2].f = (f32)cmd[4] / 65536.0f;
        }
        return 1;
    case 4:
        if (cmd[2] != ARG_KEEP) {
            obj->unk7C[0].i = cmd[2];
        }
        if (cmd[3] != ARG_KEEP) {
            obj->unk7C[1].i = cmd[3];
        }
        if (cmd[4] != ARG_KEEP) {
            obj->unk7C[2].i = cmd[4];
        }
        return 1;
    case 5:
        obj->unk3C = cmd[2];
        return 1;
    case 6:
        obj->flags73 &= ~3;
        obj->flags73 |= cmd[2] & 3;
        return 1;
    case 7:
        if (cmd[2] != 0) {
            obj->flags73 |= 4;
        } else {
            obj->flags73 &= ~4;
        }
        return 1;
    case 8:
        obj->unk3C &= 0xFFFF;
        obj->unk3C |= cmd[2] << 16;
        obj->unk7C[0].i = cmd[3];
        return 1;
    case 9:
        func_150E4928(obj);
        break;
    case 10:
        obj->unk38 = D_80088C90[cmd[2]].unk0;
        obj->unk78 = D_80088C90[cmd[2]].unk4;
        obj->unk92 = D_80088C90[cmd[2]].unk8;
        break;
    case 11:
        func_15173C90(cmd[2], cmd[3], obj->id);
        break;
    case 13:
        obj->flags4F = (obj->flags4F & ~cmd[3]) | cmd[2];
        break;
    case 14:
        if (cmd[2] != ARG_KEEP) {
            obj->unk5A[0] = cmd[2];
        }
        if (cmd[3] != ARG_KEEP) {
            obj->unk5A[1] = cmd[3];
        }
        if (cmd[4] != ARG_KEEP) {
            obj->unk5A[2] = cmd[4];
        }
        break;
    case 15:
        if (cmd[2] != ARG_KEEP) {
            obj->unk60[0] = cmd[2];
        }
        if (cmd[3] != ARG_KEEP) {
            obj->unk60[1] = cmd[3];
        }
        if (cmd[4] != ARG_KEEP) {
            obj->unk60[2] = cmd[4];
        }
        break;
    case 16:
        if (cmd[2] != ARG_KEEP) {
            obj->unk0 = cmd[2];
        }
        if (cmd[3] != ARG_KEEP) {
            obj->unk4 = cmd[3];
        }
        if (cmd[4] != ARG_KEEP) {
            obj->unk8 = cmd[4];
        }
        break;
    case 17:
        obj->unk88 = cmd[2];
        break;
    case 18:
        obj->unk8A = cmd[2];
        break;
    }
    return 0;
}

extern s32 func_15114050(Struct1509E900 *, s32);
extern s32 func_15084000(s32);
extern s32 func_151140C4(Struct1509E900 *);

/* Script "query" handler for the object with id `id`: returns the value that operation `op` asks for. */
s32 func_1509ED74(s32 id, s32 op, s32 *cmd) {
    Struct1509E900 *obj;
    s32 i;
    s32 result;

    obj = func_151149AC(id);
    if (obj == NULL) {
        return 0;
    }
    switch (op) {
    case 19:
        return func_15114050(obj, -1);
    case 20:
        return func_15114050(obj, func_15084000(cmd[2] & 0xFFFF0FFF));
    case 21:
        for (i = 0; i < 25; i++) {
            if (D_800CC2D0[i].interaction_state == cmd[2] && func_15114050(obj, i)) {
                return 1;
            }
        }
        return 0;
    case 22:
        if ((obj->flags4F & 4) == 4) {
            return 1;
        }
        return 0;
    case 23:
        result = func_151140C4(obj);
        if (result < 0) {
            return 0;
        }
        return result | 0x2000;
    case 24:
        return obj->flags4F;
    case 25:
        if (obj->unk7C[2].i == obj->unk7C[0].i) {
            return 1;
        }
        return 0;
    case 26:
        return obj->unk3C;
    case 27:
        return obj->flags73 & 3;
    case 28:
        return obj->unk6E == 0;
    case 29:
        return obj->x;
    case 30:
        return obj->y;
    case 31:
        return obj->z;
    case 32:
        return obj->unk8;
    case 33:
        if (cmd[2] == 0) {
            return obj->unk7C[0].i;
        }
        if (cmd[2] == 1) {
            return obj->unk7C[1].i;
        }
        if (cmd[2] == 2) {
            return obj->unk7C[2].i;
        }
        break;
    case 34:
        if (cmd[2] == 0) {
            return obj->unk7C[0].f;
        }
        if (cmd[2] == 1) {
            return obj->unk7C[1].f;
        }
        if (cmd[2] == 2) {
            return obj->unk7C[2].f;
        }
        break;
    case 35:
        return (obj->flags73 & 4) != 0;
    }
    return 0;
}

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

extern s32 func_1000F568(s32, u8);
extern void func_15114D24(Struct1509E900 *, s32, s32, s16, s32, s32);

s32 func_1509F47C(arg0, arg1, arg2, arg3, arg4, arg5)
    s32 arg0; s32 arg1; s32 arg2; s32 arg3; s16 arg4; s32 arg5;
{
    Struct1509E900 *obj;

    obj = func_151149AC(arg3);
    func_15114D24(obj, func_1000F568(arg0, (u8)(arg1 - arg0 + 1)), arg2, arg4, arg5, 0);
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
