#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} struct150948C0;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[2];
    s32 unk4;
} struct1509499C_arg0;

extern f32 *D_800D2C20;
void func_1509499C(struct1509499C_arg0 *, s32 *);
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

void func_150948C0(void *arg0, s32 arg1) {
    struct150948C0 *sp50[16];
    s32 i;
    struct150948C0 *point;
    s32 out;

    func_1509499C(arg0, (s32 *)sp50);
    for (i = 0; i != 0x10; i++) {
        point = sp50[i];
        if (point != 0) {
            out = (i * 0xC) + arg1;
            func_150A7960(D_800D2C20, (f32)point->unk0, (f32)point->unk2, (f32)point->unk4,
                          (f32 *)out, (f32 *)(out + 4), (f32 *)(out + 8));
        }
    }
}

void func_1509499C(struct1509499C_arg0 *arg0, s32 *arg1) {
    s32 packed;
    s32 value;
    s32 i;
    s32 lo;

    value = arg0->unk4;
    packed = arg0->unk1;
    lo = (s16)(packed & 0xF);
    i = 0;
    packed = (s16)((packed >> 4) + 1);

    for (; i < lo; i++) {
        arg1[i] = 0;
    }

    i = lo;
    for (; i < packed; i++) {
        arg1[i] = value;
        value += 0x10;
    }

    for (; i < 0x10; i++) {
        arg1[i] = 0;
    }
}

void func_15094AB8(Vtx *arg0, s16 (*arg1)[2], s32 arg2, f32 arg3, s32 arg4, s32 arg5) {
    f32 cx;
    f32 cy;
    f32 s;
    f32 c;
    s32 i;

    s = sinf(arg3 * 0.01745329238f);
    c = cosf(arg3 * 0.01745329238f);
    cx = (arg4 / 2) << 5;
    cy = (arg5 / 2) << 5;
    for (i = 0; i < arg2; i++) {
        arg0[i].v.tc[0] = ((arg1[i][0] - cx) * c) - ((arg1[i][1] - cy) * s) + cx;
        arg0[i].v.tc[1] = ((arg1[i][0] - cx) * s) + ((arg1[i][1] - cy) * c) + cy;
    }
}

void func_15094E98(void) {
}

extern Mtx D_800D2CA8[];
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);
void func_150A7A00(void *, f32, f32, f32, f32 *, f32 *, f32 *, f32 *);

typedef struct {
    u8 pad0[0xC];
    f32 unkC;
    f32 unk10;
    u8 pad14[0x20];
    f32 unk34;
    f32 unk38;
    u8 pad3C[0x144];
} struct1509563C;void func_15094EA0(s32 arg0);

extern Gfx D_800873D0[];
extern u8 * D_800D2CA0;
Gfx *func_15094F40(Gfx *arg0);

typedef struct {
    u32 unk0;
    u8 unk4;
    u8 pad5;
    u16 unk6;
    u16 unk8;
    u8 unkA;
    u8 unkB;
} struct15095060_arg0;

typedef struct {
    u32 unk0;
    u16 unk4;
    u16 unk6;
    u8 unk8;
    u8 unk9;
    u8 unkA;
} struct15095060_d;

typedef struct {
    u8 pad0[0x10];
    struct15095060_d *unk10;
} struct15095060_arg2;

extern void func_15095060(struct15095060_arg0 *, s32, struct15095060_arg2 *);
extern Gfx *func_150950D4(Gfx *, struct15095060_d *, s32, s32, s32, s32, s32, s32, s32, s32);
extern struct15095060_d D_800D2C90;
void func_15094F70(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_15094FE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10);
void func_15095060(struct15095060_arg0 *arg0, s32 arg1, struct15095060_arg2 *arg2);

extern s32 func_1510D0EC(s32, s32, s32, s32);
extern u8 D_8009DEB0[];
extern u8 D_8009DEB4[];
extern u8 D_8009DEB8[];
extern u8 D_8009DEBC[];
extern s32 D_800D2C9C;
Gfx *func_150950D4(Gfx *gfx, struct15095060_d *tex, s32 tmem, s32 tile, s32 frame, s32 sfrac, s32 cm, s32 s, s32 t, s32 arg9);
s32 func_1509563C(f32 arg0, f32 arg1, f32 arg2, f32 *arg3, f32 *arg4, f32 *arg5, f32 *arg6, f32 arg7);

s32 func_1509563C(f32, f32, f32, f32 *, f32 *, f32 *, f32 *, f32);
void func_15095B08(s32, f32, f32, f32, s32, s32 *);
s32 func_15095A90(s32, s32, f32, f32, f32, s32, s32, s32, s32);
s32 func_15095D34(s32, s32, s32, s32, s32);

struct Pt15095760 {
    s16 x;
    s16 y;
    s16 z;
};

#define WGFX15095760(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}
s32 func_15095760(Gfx *arg0, struct Pt15095760 *arg1);

typedef struct Cmd150958B0 {
    u32 w0;
    u32 w1;
} Cmd150958B0;
Cmd150958B0 *func_150958B0(Cmd150958B0 *arg0, s16 *arg1, s32 *arg2);
void func_15095A48(s32 arg0, s32 arg1, f32 arg2, f32 arg3);
s32 func_15095A90(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

extern f32 D_800380A4;
extern f32 D_800D2C70;
extern f32 D_800D2C74;
extern f32 D_800D2C78;
extern f32 D_800D2C7C;
extern f32 D_800D2C80;
extern f32 D_800D2C84;
extern f32 D_800D2C88;

typedef struct {
    u8 pad0[4];
    u16 unk4;
    u16 unk6;
} Struct15095B08_sub;

typedef struct {
    u8 pad0[6];
    s16 unk6;
    s16 unk8;
    u8 padA[6];
    Struct15095B08_sub *unk10;
} Struct15095B08_arg0;

typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    u8 pad34[0x180 - 0x34];
} Struct15095B08_cam;
void func_15095B08(s32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, s32 *arg5);

s32 func_15095D34(s32, s32, s32, s32, s32);
void func_15095D0C(s32 arg0, s32 arg1);

extern Gfx D_80087408[];
Gfx *func_15096934(Gfx *arg0);
