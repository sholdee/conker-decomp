#include <ultra64.h>
#define func_150A7CB0 func_150A7CB0_hdr
#include "functions.h"
#undef func_150A7CB0
void func_150A7CB0(f32 mtx[4][4], f32 x, f32 y, f32 z);
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

typedef struct Node15184368 {
    struct Node15184368 *next;
    u8 unk4;
    u8 pad5[4];
    u8 unk9;
    u8 padA[4];
    s16 unkE;
    s16 unk10;
    s16 unk12;
} Node15184368;

extern struct17 D_800DE010;
extern struct17 D_800DDFB0[];
extern f32 D_800DF6F8;
extern f32 D_800DE028;
extern f32 D_800DE02C;
extern u8 D_800DF0E1;
extern f32 D_800DF0F8[][4][4];
extern u16 D_800DF0D0[];
extern Mtx D_800DF2F8[][2];
extern s32 D_800DF088;
extern u8 D_800DE048[];
extern f32 D_800D3680;
extern f32 D_800D3684;
extern f32 D_800D368C;
extern s8 D_8008D3F8[8][3];
extern s32 func_150AD9A0(s32, s32, s32);
extern void func_15047390(f32 mf[4][4], f32, f32, f32, f32, f32, f32, f32, f32, f32);
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);
f32 func_15184368(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);

extern s32 func_150AD9A0(s32, s32, s32);
extern void func_15184150(s32, f32 *, f32 *, f32 *);
extern f32 D_800D368C;
s32 func_15184DF0(s32 arg0, s32 arg1, f32 *arg2, f32 *arg3, f32 *arg4);

extern Gfx D_8008D448[];
extern Vp D_8008D3E8;
extern Mtx D_89470;
extern s16 D_80082FA6;
Gfx *func_1501A680(Gfx *arg0);
extern s32 func_1506196C(struct127 *, s32);
extern Gfx *func_1502CCFC(Gfx *, s32, s32, struct255 *, s32, s32 *, s32, s32);
Gfx *func_1510B7B4(Gfx *gfx, s32 idx);
Gfx *func_15184FA4(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);

struct vtx151857DC {
    f32 x;       /* 0x0 */
    f32 y;       /* 0x4 */
    f32 z;       /* 0x8 */
    s16 unkC;    /* 0xC */
    s16 unkE;    /* 0xE */
    s16 unk10;   /* 0x10 */
};
struct vtx151857DC *func_15185454(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2);
s32 func_15185554(f32 *arg0);
struct vtx151857DC *func_1518557C(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2);
s32 func_1518567C(f32 *arg0);

struct vtx151856A8 {
    f32 x;
    f32 y;
    f32 z;
    s16 unkC;
    s16 unkE;
    s16 unk10;
};
struct vtx151856A8 *func_151856A8(struct vtx151856A8 *a0, struct vtx151856A8 *a1, struct vtx151856A8 *a2);
s32 func_151857B0(f32 *arg0);
struct vtx151857DC *func_151857DC(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2);
s32 func_151858D4(f32 *arg0);
struct vtx151857DC *func_151858FC(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2);

extern f32 D_800D3688;
s32 func_151859FC(f32 *arg0);

extern f32 D_800D368C;
struct vtx151857DC *func_15185A28(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2);

extern f32 D_800D368C;
s32 func_15185B28(f32 *arg0);
struct vtx151857DC *func_15185B54(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2);
s32 func_15185C44(f32 *arg0);
struct vtx151857DC *func_15185C6C(struct vtx151857DC *a0, struct vtx151857DC *a1, struct vtx151857DC *a2);
s32 func_15185D5C(f32 *arg0);
s32 func_15185D84(f32 *arg0);
s32 func_15185DAC(f32 *arg0);
struct vtx151857DC *func_15185DD4(struct vtx151857DC *arg0, s32 arg1, s32 arg2, struct vtx151857DC *arg3);

struct vin15185F24 {
    f32 x;
    f32 y;
    f32 z;
    s16 *col;
    s32 unk10;
};
s32 func_15185F24(struct vin15185F24 *arg0, s32 arg1, s32 arg2, Vtx_t *arg3, s32 *arg4, s32 arg5);

extern s32 func_1510F720(s32, s32, s32, u8 **);
extern s32 func_150A5B90(u32 *, s32, s32, s32);
extern u32 *D_800D3694;
extern void func_1510F800(s32);
extern s32 D_800D3690;
extern s32 D_800DF0E8;
extern s32 D_800DF0EC;
extern s32 D_800DF0F0;
extern s32 D_800DF0F4;
s32 func_1518652C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_151872B0(s32 arg0);

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
s32 func_151873E4(s32 arg0, s32 arg1, s32 arg2, struct vtx151873E4 *arg3, f32 *arg4, struct vtx151873E4 *arg5, s32 arg6);

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

#pragma GLOBAL_ASM("asm/nonmatchings/game_1B4A90/func_15187A98.s")

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

