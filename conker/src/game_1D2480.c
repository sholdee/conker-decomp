#include <ultra64.h>
#define func_15169260 func_15169260_s32
#include "functions.h"
#undef func_15169260
#include "variables.h"


extern f32 D_800A8D50;
extern f32 D_800A8D54;
extern f32 D_800A8D5C;
extern f32 D_800A8D60;

struct Data151A561C {
    s32 unk0;
};

extern struct Data151A561C D_800A8D70;
void func_15169260(struct Data151A561C *, s32, s32, u8);

struct vec151A3504 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct frame151A3504 {
    void *unk0;
    u8 unk4;
    u8 unk5;
    u8 pad6[0x2];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    u8 pad1E[0x2];
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 pad2F;
    u8 unk30;
    u8 pad31[0x3];
    struct vec151A3504 unk34;
    f32 unk40;
    u8 unk44;
    u8 pad45[0x3];
    f32 unk48;
    u8 unk4C;
    u8 pad4D[0x3];
};

struct header151A3504 {
    struct vec151A3504 unk0;
    s16 unkC;
    u16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[0x2];
    s32 unk18;
};

struct260 *func_151A3504(struct frame151A3504 *, u8);
void func_151A4590(s32, u8);
void func_151A499C(void *, u8);
void *func_1513418C(void *, s32, u8, s32);
struct260 *func_15130374(void *, u8, s32, u8, s32);
u16 func_10010154(u16, void *, s32, s16, u16);
s32 func_15160A58(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
struct struct151A4E34;
s32 func_151A4E34(struct struct151A4E34 **arg0, s32 arg1);

struct frame151A3390 {
    s32 unk0;
    u8 unk4;
    u8 unk5;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    f32 unk20;
    f32 unk24;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
};
void func_151A3390(void *arg0, u8 arg1);
struct260 *func_151A3504(struct frame151A3504 *arg0, u8 arg1);

typedef struct TimedVec151A361C {
    f32 x;
    f32 y;
    f32 z;
} TimedVec151A361C;

typedef struct TimedEntry151A361C {
    TimedVec151A361C value0;
    f32 fieldC;
    s16 field10;
    s16 field12;
    u8 field14;
    u8 field15;
    u8 pad16[2];
} TimedEntry151A361C;

typedef struct TimedState151A361C {
    u8 *owner;
    u8 identity;
    u8 transformIndex;
    u8 pad06[2];
    TimedVec151A361C input08;
    f32 field14;
    s16 field18;
    s16 field1A;
    s16 field1C;
    u8 pad1E[2];
    f32 field20;
    f32 field24;
    u8 field28;
    u8 field29;
    s8 field2A;
    u8 pad2B[5];
    u8 flags30;
    u8 pad31[3];
    TimedVec151A361C previous34;
    f32 field40;
    u8 field44;
    u8 pad45[3];
    f32 field48;
    u8 field4C;
} TimedState151A361C;

typedef struct TimedObject151A361C {
    u8 pad0[0xC];
    u8 fieldC;
    u8 pad0D[3];
    TimedVec151A361C position10;
    u8 pad1C[2];
    u16 flags1E;
    u8 pad20[5];
    u8 field25;
    u8 pad26[6];
    s8 field2C;
    s8 field2D;
    s8 field2E;
    u8 pad2F[0x25];
    TimedVec151A361C output54;
    u8 pad60[0x34];
    TimedEntry151A361C *entries94;
    TimedState151A361C *state98;
} TimedObject151A361C;
s32 func_151A361C(TimedObject151A361C *arg0);

typedef struct {
    f32 x, y, z;
} Vec3f_151A3BE4;

typedef struct {
    Vec3f_151A3BE4 pos;
    f32 width;
    s16 unk10;
    u8 pad12[2];
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} Point151A3BE4;

typedef struct {
    u8 pad0[0x2B];
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 pad2F;
    u8 unk30;
    u8 pad31[0x4C - 0x31];
    u8 unk4C;
} Style151A3BE4;

typedef struct {
    u8 pad0[0x10];
    Vec3f_151A3BE4 pos;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x84 - 0x2F];
    u8 unk84[0x10];
    Point151A3BE4 *points;
    Style151A3BE4 *style;
} Owner151A3BE4;

extern s32 D_800D2C9C;
extern u8 D_80090CD4[];
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_151A3BE4(Owner151A3BE4 *arg0, Gfx *gfx, s16 arg2);

struct frame151A4590 {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 pad9[3];
    void *unkC;
    u8 unk10;
    u8 pad11[3];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    u8 unk2A;
    u8 unk2B;
    s8 unk2C;
    u8 unk2D;
};
void func_151A4590(s32 arg0, u8 arg1);
void func_151A4638(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, void *arg6);

struct Sub151A483C {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
};

struct Obj151A483C {
    u8 pad0[0x18];
    s16 unk18;
    s16 unk1A;
    u8 pad1C[0x2B - 0x1C];
    u8 unk2B;
    u8 unk2C;
    u8 pad2D[0x38 - 0x2D];
    f32 unk38;
    f32 unk3C;
    u8 pad40[0x70 - 0x40];
    u8 unk70;
    u8 unk71;
    u8 unk72;
    u8 pad73[0xA8 - 0x73];
    struct Sub151A483C unkA8;
};
s32 func_151A483C(struct Obj151A483C *arg0, u8 arg1);
s32 func_151A4900(struct Obj151A483C *arg0, u8 arg1);


extern f32 D_800A8D64;

struct frame151A499C {
    void *unk0;
    u8 unk4;
    f32 unk8;
    f32 unkC;
    u8 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};
void func_151A499C(void *arg0, u8 arg1);

struct Obj151A4A38 {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x38];
    u8 unk74;
    u8 pad75[1];
    u16 unk76;
    u8 pad78[0x15C];
    s32 unk1D4;
};

struct Sub151A4A38 {
    struct Obj151A4A38 *unk0;
    u8 unk4;
    u8 pad5[3];
    f32 unk8;
    f32 unkC;
    u8 unk10;
    u8 pad11[3];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

struct Ent151A4A38 {
    u8 pad0[1];
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 unkD;
    s16 unkE;
    u8 pad10[0x18];
    struct Sub151A4A38 unk28;
};

struct Block151A4A38 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct Arg151A4A38 {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    u8 pad5;
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    struct Block151A4A38 unk38;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    s32 unk5C;
    s8 unk60;
    s8 unk61;
    u8 unk62;
    u8 unk63;
    u8 unk64;
    u8 pad65[3];
    f32 unk68;
};

void func_15143134(void *, s32, s32);
void func_15153634(void *, s32, s32, s32);
void func_151A4A38(struct Ent151A4A38 *arg0);

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA_151A4CE0;

typedef struct {
    s32 unk0;
    union {
        u8 b4;
        s32 w4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_151A4CE0;

void func_151A4E9C(void *arg0);
void func_151A4CE0(struct260 *arg0, s32 arg1, u8 arg2);

void func_1516972C(struct102 *arg0);

typedef struct {
    s32 unk0;
    u8 unk4;
} SubA_151A4D88;

typedef struct {
    s32 unk0;
    union {
        u8 b4;
        s32 w4;
    } u4;
    u8 unk8;
    u8 unk9;
} ArgB_151A4D88;
void func_151A4D88(struct260 *arg0, s32 arg1, u8 arg2);

void func_15143134(void *, s32, s32);
struct struct151A4E34
{
  u8 pad0[0x74];
  u8 unk74;
  u8 pad75[0x1D4 - 0x75];
  struct elem151A4E34 *unk1D4;
};
s32 func_151A4E34(struct struct151A4E34 **arg0, s32 arg1);
void func_151A4E9C(void *arg0);


typedef struct Tgt151A4ECC {
    s32 unk0;
    char pad4[0x37];
    u8 unk3B;
    char pad3C[0x38];
    u8 unk74;
    char pad75[0x15F];
    s32 unk1D4;
} Tgt151A4ECC;

typedef struct Obj151A4ECC {
    char pad0[0xC];
    u8 unkC;
    u8 unkD;
    s16 unkE;
    char pad10[0x18];
    struct frame151A3504 unk28;
} Obj151A4ECC;
void func_151A4ECC(Obj151A4ECC *arg0);

void func_1516972C(struct102 *arg0);

struct struct151A4F7C {
    s32 unk0;
    u8 unk4;
};
void func_151A4F7C(struct102 *arg0, struct102 *arg1, u8 arg2);

struct Obj151A4FD0 {
    char pad0[0x10];
    s32 unk10;
    u8  unk14;
    u8  unk15;
    u8  unk16;
    u8  unk17;
    u8  unk18;
    u8  unk19;
    u8  unk1A;
};

extern struct Obj151A4FD0 *func_15167A68(s32, s32, s32, s32, s32, s32);

struct Obj151A4FD0 *func_151A4FD0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, u8 a6, s32 a7) {
    struct Obj151A4FD0 *ret = func_15167A68(0x5A, 0, a7 + 0x20, 0, 0xFF, 1);
    if (ret == 0) {
        return NULL;
    }
    ret->unk14 = a0;
    ret->unk10 = a1;
    ret->unk15 = a2;
    ret->unk16 = a3;
    ret->unk17 = a4;
    ret->unk19 = a6;
    ret->unk18 = 0;
    ret->unk1A = a5;
    return ret;
}

void func_151A5070(struct Obj151A4FD0 *arg0) {
    struct Obj151A4FD0 *temp_a1;
    s32 temp_v0;
    s32 temp_t8;
    s32 var_v0;

    temp_a1 = arg0;
    temp_v0 = temp_a1->unk10;
    if (temp_v0 != 0) {
        temp_v0 -= D_800BE9E4;
        if (temp_v0 <= 0) {
            func_1516972C((struct102 *)temp_a1);
            return;
        }
        temp_a1->unk10 = temp_v0;
    }

    if (temp_a1->unk19 == 0) {
        func_10011FA0((s32 *)2);
    }

    var_v0 = temp_a1->unk15;
    temp_t8 = D_800BE9E4 * 2;
    if (temp_a1->unk18 == 0) {
        var_v0 -= temp_t8;
        if (var_v0 < 0x90) {
            var_v0 = 0x90;
            temp_a1->unk18 = 1;
        }
    } else {
        var_v0 += temp_t8;
        if (var_v0 >= 0x100) {
            var_v0 = 0xFF;
            temp_a1->unk18 = 0;
        }
    }
    temp_a1->unk15 = var_v0;
}

extern void (*D_8008F900[])(void *, void *, s16);

void func_151A5130(void *arg0, void *arg1, s16 arg2)
{
  D_8008F900[*((u8 *) (((u8 *) arg1) + 0x14))](arg0, arg1, arg2);
}

/* Viewport/screen descriptor: D_800BE628 is an array of these, 0x180 bytes each. */
struct Scr151A5170 {
    u8  pad0[0x4];
    f32 unk4;
    f32 unk8;
    u8  padC[0x18];
    f32 unk24;
    u8  pad28[0x4];
    f32 unk2C;
    u8  pad30[0x150];
};

Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

#define SCR151A5170(obj) (&((struct Scr151A5170 *)D_800BE628)[(obj)->unk1A])

Gfx *func_151A5170(Gfx *gfx, struct Obj151A4FD0 *arg1, s32 arg2) {
    s32 x0;
    s32 i;
    s32 y0;
    s32 w;
    s32 h;
    s32 sw;
    s32 sh;
    u8 update;

    gDPPipeSync(gfx++);
    gDPSetCombineMode(gfx++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(gfx++, 1, 0, arg1->unk15, arg1->unk16, arg1->unk17, 255);
    func_1501A490((s32)gfx, arg2, 0, 0, 0, 3);
    update = 0;
    gfx = func_15142FBC(gfx, 0xC00, 0x0F0A4004, &update);

    sw = (s32)SCR151A5170(arg1)->unk4;
    sh = (s32)SCR151A5170(arg1)->unk8;

    for (i = 0; i < 2; i++) {
        if (i == 0) {
            x0 = (s32)SCR151A5170(arg1)->unk2C;
            h = 8;
            if (sw > 256) {
                w = 256;
            } else {
                w = sw;
            }
        } else {
            if (sw <= 256) {
                break;
            }
            x0 = (s32)(SCR151A5170(arg1)->unk2C + 256.0f);
            w = sw - 256;
            h = 0x36;
        }

        y0 = (s32)SCR151A5170(arg1)->unk24;

        while ((f32)y0 < SCR151A5170(arg1)->unk24 + (f32)sh) {
            if (SCR151A5170(arg1)->unk24 + (f32)sh < (f32)(y0 + h)) {
                h = (s32)(SCR151A5170(arg1)->unk24 + (f32)sh - (f32)y0);
            }

            gDPPipeSync(gfx++);
            gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_800BE620,
                               D_8002AAE8[D_800BE9C0]);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
                       ((w * G_IM_SIZ_16b_LINE_BYTES) + 7) >> 3, 0, G_TX_LOADTILE, 0,
                       G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPLoadTile(gfx++, G_TX_LOADTILE, x0 << G_TEXTURE_IMAGE_FRAC,
                        y0 << G_TEXTURE_IMAGE_FRAC, (x0 + w - 1) << G_TEXTURE_IMAGE_FRAC,
                        (y0 + h - 1) << G_TEXTURE_IMAGE_FRAC);
            gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
                       ((w * G_IM_SIZ_16b_LINE_BYTES) + 7) >> 3, 0, G_TX_RENDERTILE, 0,
                       G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOLOD);
            gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, w << G_TEXTURE_IMAGE_FRAC,
                           h << G_TEXTURE_IMAGE_FRAC);
            gSPTextureRectangle(gfx++, x0 << G_TEXTURE_IMAGE_FRAC, y0 << G_TEXTURE_IMAGE_FRAC,
                                (x0 + w) << G_TEXTURE_IMAGE_FRAC,
                                (y0 + h) << G_TEXTURE_IMAGE_FRAC,
                                G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);

            y0 += h;
        }
    }

    gDPPipeSync(gfx++);
    return func_15142FBC(gfx, 0x2C00, 0x0F0A4004, &update);
}

extern void (*D_8008F904[])(void *, void *, u8);

void func_151A55D4(void *arg0, void *arg1, u8 arg2)
{
  void (*f)(void *, void *, u8);
  f = D_8008F904[*((u8 *) (((u8 *) arg0) + 0x19))];
  if (f != NULL) {
    f(arg0, arg1, arg2);
  }
}

void func_151A561C(s32 arg0, u8 arg1) {
    struct Data151A561C sp1C;

    sp1C = D_800A8D70;
    func_15169260(&sp1C, 1, arg0, arg1);
}
