#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct vtx151857DC;
extern struct vtx151857DC *(*D_8008D498[])(struct vtx151857DC *, struct vtx151857DC *, struct vtx151857DC *);
extern s32 (*D_8008D4C0[])(struct vtx151857DC *);

extern f32 D_800D3688;
extern s32 D_800DE01C;
extern s32 D_800DE020;
extern s32 D_800DE024;
extern s32 D_800DE030;
extern s32 D_800DE034;
extern s32 D_800DE038;
extern f32 D_800DE03C;
extern u8 D_800DE040;
extern u8 D_800DE041;

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B1600/func_15184150.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B1600/func_15184368.s")

extern s32 func_150AD9A0(s32, s32, s32);
extern void func_15184150(s32, f32 *, f32 *, f32 *);
extern f32 D_800D368C;

s32 func_15184DF0(s32 arg0, s32 arg1, f32 *arg2, f32 *arg3, f32 *arg4) {
    f32 x;
    f32 y;
    f32 z;
    s32 dist;

    x = D_800CC2D0[arg0].x_position;
    y = D_800CC2D0[arg0].y_position;
    z = D_800CC2D0[arg0].z_position;
    dist = func_150AD9A0(x - D_800DBFF0[arg1].unk2F8,
                         y - D_800DBFF0[arg1].unk2FC,
                         z - D_800DBFF0[arg1].unk300);
    if (dist > 2000) {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0;
    }
    func_15184150(arg0, arg2, arg3, arg4);
    *arg2 *= 1.299999952f;
    *arg3 *= 1.299999952f;
    *arg4 *= 1.299999952f;
    if ((*arg2 == 0.0f) || (*arg3 == 0.0f) || (*arg4 == 0.0f)) {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0;
    }
    return dist;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B1600/func_15184FA4.s")

struct vtx151857DC {
    f32 x;       /* 0x0 */
    f32 y;       /* 0x4 */
    f32 z;       /* 0x8 */
    s16 unkC;    /* 0xC */
    s16 unkE;    /* 0xE */
    s16 unk10;   /* 0x10 */
};

struct vtx151857DC *func_15185454(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;
    f32 y;

    t = (a0->z - a0->y) / ((a1->y - a0->y) - (a1->z - a0->z));
    a2->x = a0->x + (a1->x - a0->x) * t;
    y = a0->y + (a1->y - a0->y) * t;
    a2->z = y;
    a2->y = y;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_15185554(f32 *arg0) {
    return arg0[2] < arg0[1];
}

struct vtx151857DC *func_1518557C(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = (a0->z + a0->x) / ((a0->x - a1->x) + (a0->z - a1->z));
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = -a2->x;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_1518567C(f32 *arg0) {
    return arg0[0] < -arg0[2];
}

struct vtx151856A8 {
    f32 x;
    f32 y;
    f32 z;
    s16 unkC;
    s16 unkE;
    s16 unk10;
};

struct vtx151856A8 *func_151856A8(struct vtx151856A8 *a0, struct vtx151856A8 *a1, struct vtx151856A8 *a2) {
    f32 t;

    t = (a0->z + a0->y) / ((a0->y - a1->y) + (a0->z - a1->z));
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = -a2->y;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_151857B0(f32 *arg0) {
    return arg0[1] < -arg0[2];
}

struct vtx151857DC *func_151857DC(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = (a0->z - a0->x) / ((a1->x - a0->x) - (a1->z - a0->z));
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = a2->x;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_151858D4(f32 *arg0) {
    return arg0[2] < arg0[0];
}

struct vtx151857DC *func_151858FC(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = (D_800D3688 - a0->z) / (a1->z - a0->z);
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = D_800D3688;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

extern f32 D_800D3688;

s32 func_151859FC(f32 *arg0) {
    return D_800D3688 < arg0[2];
}

extern f32 D_800D368C;

struct vtx151857DC *func_15185A28(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = (D_800D368C - a0->z) / (a1->z - a0->z);
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = D_800D368C;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

extern f32 D_800D368C;

s32 func_15185B28(f32 *arg0) {
    return arg0[2] < D_800D368C;
}

struct vtx151857DC *func_15185B54(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = a0->x / (a0->x - a1->x);
    a2->x = 0.0f;
    a2->y = a0->y + (a1->y - a0->y) * t;
    a2->z = a0->z + (a1->z - a0->z) * t;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_15185C44(f32 *arg0) {
    return arg0[0] <= 0.0f;
}

struct vtx151857DC *func_15185C6C(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2) {
    f32 t;

    t = a0->y / (a0->y - a1->y);
    a2->y = 0.0f;
    a2->x = a0->x + (a1->x - a0->x) * t;
    a2->z = a0->z + (a1->z - a0->z) * t;
    a2->unkC = (s16)((f32)a0->unkC + (f32)(a1->unkC - a0->unkC) * t);
    a2->unkE = (s16)((f32)a0->unkE + (f32)(a1->unkE - a0->unkE) * t);
    a2->unk10 = (s16)((f32)a0->unk10 + (f32)(a1->unk10 - a0->unk10) * t);
    return a2 + 1;
}

s32 func_15185D5C(f32 *arg0) {
    return arg0[1] <= 0.0f;
}

s32 func_15185D84(f32 *arg0) {
    return arg0[0] > 0.0f;
}

s32 func_15185DAC(f32 *arg0) {
    return arg0[1] > 0.0f;
}

struct vtx151857DC *func_15185DD4(struct vtx151857DC *arg0, s32 arg1, s32 arg2, struct vtx151857DC *arg3) {
    struct vtx151857DC *cur;
    struct vtx151857DC *prev;
    struct vtx151857DC *(*emit)(struct vtx151857DC *, struct vtx151857DC *, struct vtx151857DC *);
    s32 (*test)(struct vtx151857DC *);
    s32 i;

    prev = &arg0[arg1 - 1];
    emit = D_8008D498[arg2];
    test = D_8008D4C0[arg2];
    cur = arg0;
    i = 0;
    if (arg1 > 0) {
        do {
            if (test(cur) != 0) {
                if (test(prev) != 0) {
                    *arg3++ = *cur;
                } else {
                    arg3 = emit(prev, cur, arg3);
                    *arg3++ = *cur;
                }
            } else {
                if (test(prev) != 0) {
                    arg3 = emit(prev, cur, arg3);
                }
            }
            i++;
            prev = cur;
            cur++;
        } while (i != arg1);
    }
    return arg3;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B1600/func_15185F24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B1600/func_1518652C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B1600/func_15186794.s")

void func_151872B0(s32 arg0) {
    D_800DE01C = 0x258;
    D_800DE020 = 0xB4;
    D_800DE024 = 0x5A;
    D_800DE030 = 0x3F8;
    D_800DE034 = 0;
    D_800DE038 = 1;
    D_800DE03C = 1.0f;
    D_800DE040 = 1;
    D_800DE041 = 1;

    switch (arg0) {
    case 0x21:
        D_800DE01C = 0x12C0;
        D_800DE020 = D_800DE01C - 1;
        D_800DE024 = D_800DE01C - 2;
        D_800DE030 = 0x3F2;
        D_800DE034 = 1;
        D_800DE038 = 4;
        D_800DE040 = 0;
        break;
    case 0x22:
        D_800DE01C = 0x960;
        D_800DE020 = D_800DE01C - 1;
        D_800DE024 = D_800DE01C - 2;
        D_800DE038 = 4;
        D_800DE040 = 0;
        break;
    case 0x14:
        D_800DE041 = 0;
        break;
    case 0x29:
        D_800DE01C = 0x4B0;
        D_800DE020 = 0x258;
        D_800DE024 = 0x12C;
        break;
    }
}

struct vtx151873E4 {
    f32 x;      /* 0x00 */
    f32 y;      /* 0x04 */
    f32 z;      /* 0x08 */
    s32 unkC;   /* 0x0C */
    s32 unk10;  /* 0x10 */
};              /* size 0x14 */

typedef struct {
    u8  pad0[0x10];
    s16 unk10;
    u8  pad12[0x2];
    s16 unk14;
    u8  pad16[0x39];
    u8  unk4F;
    u16 unk50;
    u16 unk52;
    u8  pad54[0x1A];
    u8  unk6E;
    u8  pad6F[0x31];
} unkfunc_151873E4; /* size 0xA0 */

extern void func_1510F800(s32);
extern void func_150A44F0(s32, s16 *, s32);
extern struct vtx151873E4 *func_150A5E44(s16 *, struct vtx151873E4 *, f32 *, struct vtx151873E4 *);

extern s32 D_800D3690;
extern s16 D_800D37E0[];
extern s32 D_800DF0E4;
extern s32 D_800DF0E8;
extern s32 D_800DF0EC;
extern s32 D_800DF0F0;
extern s32 D_800DF0F4;

s32 func_151873E4(s32 arg0, s32 arg1, s32 arg2, struct vtx151873E4 *arg3, f32 *arg4, struct vtx151873E4 *arg5, s32 arg6) {
    unkfunc_151873E4 *entry;
    s32 found;
    s32 i;
    s32 radius;

    if (D_800DBEF0 == 0) {
        return arg6;
    }

    func_1510F800(2);

    arg3 += arg6;
    D_800DF0E8 = arg0 - arg2;
    D_800DF0EC = arg0 + arg2;
    D_800DF0F0 = arg1 - arg2;
    D_800DF0F4 = arg1 + arg2;

    found = 0;
    for (i = 0; i < D_800DBEF0; i++) {
        entry = (unkfunc_151873E4 *)&D_800DBEF4[i];
        if (entry->unk6E != 0) {
            continue;
        }
        if (entry->unk4F & 0x60) {
            continue;
        }
        radius = (entry->unk52 < entry->unk50) ? entry->unk50 : entry->unk52;
        if (D_800DF0EC < entry->unk10 - radius) {
            continue;
        }
        if (entry->unk10 + radius < D_800DF0E8) {
            continue;
        }
        if (D_800DF0F4 < entry->unk14 - radius) {
            continue;
        }
        if (entry->unk14 + radius < D_800DF0F0) {
            continue;
        }
        D_800D37E0[found] = i;
        found++;
    }

    if (found != 0) {
        func_150A44F0(found, D_800D37E0, 0);
        D_800D3690 = D_800DBE3C;
        D_800DF0E4 = found;
        arg6 += func_150A5E44(D_800D37E0, arg3, arg4, arg5) - arg3;
    }
    return arg6;
}

typedef struct {
    u8 pad0[0x80];
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    f32 unk90;
    s16 unk94;
    s16 unk96;
    u8 unk98;
    u8 pad99[7];
} Particle151875E0;

typedef struct {
    u8 pad0[0x90];
    f32 unk90;
    f32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    Particle151875E0 *unkA4;
    s16 unkA8;
    s16 unkAA;
} Obj151875E0;

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void *allocate_memory(s32, s32, s32, s32);

void func_151875E0(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7) {
    Obj151875E0 *obj;
    s32 i;
    s32 a;

    obj = func_15167A68(6, 0, 0xB0, 1, 0xFF, 1);
    if (obj != NULL) {
        obj->unkA4 = allocate_memory(0x780, 1, 2, 1);
        if (obj->unkA4 == NULL) {
            func_1516972C((struct102 *)obj);
            return;
        }
        obj->unk90 = arg0;
        obj->unk94 = arg1;
        obj->unk98 = arg2;
        obj->unkA8 = 0;
        obj->unkAA = arg4;
        obj->unk9C = arg6;
        obj->unkA0 = arg7;
        for (i = 0, a = 0; a != 1080; a += 90, i++) {
            if (a < 360) {
                obj->unkA4[i].unk80 = 7 - (func_150ADA20() & 0xFU);
                obj->unkA4[i].unk84 = 0.0f;
                obj->unkA4[i].unk88 = (func_150ADA20() & 0x3FU) + (a - 30) * 1;
                obj->unkA4[i].unk98 = 0;
            } else if (a < 720) {
                obj->unkA4[i].unk80 = 0.0f;
                obj->unkA4[i].unk84 = (func_150ADA20() & 0x3FU) + (a - 570) * 1;
                obj->unkA4[i].unk88 = 7 - (func_150ADA20() & 0xFU);
                obj->unkA4[i].unk98 = 0;
            } else {
                obj->unkA4[i].unk80 = (func_150ADA20() & 0x3FU) + (a - 30) * 1;
                obj->unkA4[i].unk84 = 97 - (func_150ADA20() & 0xFU);
                obj->unkA4[i].unk88 = 0.0f;
                obj->unkA4[i].unk98 = 1;
            }
            if (arg3 >= 4) {
                obj->unkA4[i].unk8C = (func_150ADA20() % (u32)(arg3 >> 2)) + arg3;
            } else {
                obj->unkA4[i].unk8C = arg3;
            }
            obj->unkA4[i].unk90 = func_150ADA20() & 0xFU;
            obj->unkA4[i].unk94 = 0;
            obj->unkA4[i].unk96 = arg4 - (func_150ADA20() & arg5);
        }
    }
}

typedef struct {
    u8  pad0[0x8C];
    f32 unk8C;
    f32 unk90;
    s16 unk94;
    s16 unk96;
    u8  pad98[0x8];
} Elem_15187978; /* size 0xA0 */

typedef struct {
    u8  pad0[0xA4];
    Elem_15187978 *unkA4;
    s16 unkA8;
    s16 unkAA;
} Obj_15187978;

extern s32 func_10004074(void *);

void func_15187978(Obj_15187978 *arg0) {
    s32 i;

    if (arg0->unkA8 < 0) {
        func_10004074(arg0->unkA4);
        func_1516972C((struct102 *)arg0);
        return;
    }
    arg0->unkA8 += D_800BE9E4;
    if (arg0->unkAA < arg0->unkA8) {
        arg0->unkA8 = -10;
    } else if (arg0->unkA8 >= 0) {
        for (i = 0; i < 12; i++) {
            if (arg0->unkA4[i].unk94 < arg0->unkA4[i].unk96) {
                arg0->unkA4[i].unk94 += D_800BE9E4;
                if (arg0->unkA4[i].unk96 < arg0->unkA4[i].unk94) {
                    arg0->unkA4[i].unk94 = arg0->unkA4[i].unk96;
                }
                arg0->unkA4[i].unk90 += arg0->unkA4[i].unk8C - arg0->unkA4[i].unk8C / 3.0f * arg0->unkA4[i].unk94 / arg0->unkA4[i].unk96;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B1600/func_15187A98.s")

extern void func_151EFE00(f32 *);

void func_15187D6C(f32 *arg0, f32 arg1, f32 arg2, f32 arg3) {
    f32 sx;
    f32 cx;
    f32 sz;
    f32 sy;
    f32 cy;
    f32 cz;
    f32 t;

    arg1 *= 0.01745329238f;
    arg2 *= 0.01745329238f;
    arg3 *= 0.01745329238f;
    sx = sinf(arg1);
    cx = cosf(arg1);
    sy = sinf(arg2);
    cy = cosf(arg2);
    sz = sinf(arg3);
    cz = cosf(arg3);
    func_151EFE00(arg0);
    t = sx * sy;
    arg0[6] = sx;
    arg0[0] = (cy * cz) - (t * sz);
    arg0[1] = (cy * sz) + (t * cz);
    arg0[2] = -sy * cx;
    t = sx * cy;
    arg0[4] = -cx * sz;
    arg0[5] = cx * cz;
    arg0[8] = (sy * cz) + (t * sz);
    arg0[9] = (sy * sz) - (t * cz);
    arg0[10] = cx * cy;
}

