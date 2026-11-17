#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800D9C10[][4][4];
extern f32 D_800A2C20;
extern f32 D_800D9B28[4][4];
extern s16 D_800DD2F2;
extern s16 D_800DD2F4;
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);
void func_1501B22C(s32 arg0);

typedef struct {
    char pad_0[0xC];
    f32 field_0x0C;
    f32 field_0x10;
} D_800BE628View;


void func_1510B070(s32 arg0) {
    D_800BE628View *be628;
    f32 temp_f0;
    f32 temp_f2;

    D_800D9B20 = (f32)D_800DD2F2;
    D_800D9B1C = (f32)D_800DD2F4;
    D_800D3670 = 100.0f - D_800D9B1C;
    be628 = (D_800BE628View *)D_800BE628;
    func_151EF954(
        D_800D9B28,
        -(temp_f0 = be628->field_0x0C),
        temp_f0,
        -(temp_f2 = be628->field_0x10),
        temp_f2,
        1.0f,
        D_800A2C20,
        1.0f);
}

extern f32 D_800C3648;
extern f32 D_800C364C;
extern f32 D_800C3650;
f32 cosf(f32);
f32 sinf(f32);
void func_1510B5F8(s32 arg0, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);

typedef struct {
    u8 pad0[0xC];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8 pad24[0x6C - 0x24];
    f32 unk6C;
    f32 unk70;
    f32 aspect;
    f32 fovy;
    f32 unk7C;
    f32 unk80;
    u8 pad84[0x180 - 0x84];
} Cam1510B128;

#define CAM ((Cam1510B128 *)D_800BE628)
void func_1510B128(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    f32 near;
    f32 far;
    f32 pad;

    CAM[arg0].aspect = CAM[arg0].unk6C + arg1 + D_800C3648;
    CAM[arg0].fovy = CAM[arg0].unk70 + arg2 + D_800C3648;
    {
        f32 t;
        f32 c;
        f32 s;
        f32 angle;
        angle = CAM[arg0].aspect * 0.008726646192f;
        c = cosf(angle);
        t = (c * CAM[arg0].unkC) / sinf(angle);
        CAM[arg0].unk1C = (CAM[arg0].unk14 * t) * CAM[arg0].unk7C * arg3;
        angle = CAM[arg0].fovy * 0.008726646192f;
        c = cosf(angle);
        t = (c * CAM[arg0].unk10) / sinf(angle);
        CAM[arg0].unk20 = (CAM[arg0].unk18 * t) * CAM[arg0].unk80 * arg3;
    }
    if (D_800C364C != 0.0f) {
        near = D_800C364C;
    } else if (arg4 != 0.0f) {
        near = arg4;
    } else {
        near = D_800D9B20;
    }
    if (D_800C3650 != 0.0f) {
        far = D_800C3650;
    } else {
        far = D_800D9B1C;
    }
    func_1510B5F8(arg0, (u16 *)(arg0 * 0x180 + D_800BE628 + 0xB8), CAM[arg0].fovy, CAM[arg0].aspect, near, far, arg3);
}

void func_1510B128(s32, f32, f32, f32, f32);

void func_1510B32C(s32 arg0, f32 arg1, f32 arg2, f32 arg3) {
    func_1510B128(arg0, arg1, arg2, arg3, 0.0f);
    D_800D9AC0[arg0].unk0 = arg3;
    D_800D9AC0[arg0].unk4 = arg1;
    D_800D9AC0[arg0].unk8 = arg2;
    D_800D9AF0 = 1;
}

void func_1510B3B0(void) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (D_800D9AC0[i].unk0 != -1.0f) {
            func_1510B128(i, D_800D9AC0[i].unk4, D_800D9AC0[i].unk8, D_800D9AC0[i].unk0, 0.0f);
            D_800D9AC0[i].unk0 = -1.0f;
        }
    }
    D_800D9AF0 = 0;
}

void func_1510B5F8(s32 arg0, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);

typedef struct {
    u8 pad0[0x74];
    f32 aspect;
    f32 fovy;
    u8 pad7C[8];
    f32 scale;
    u8 pad88[0x30];
    u16 perspNorm;
    u8 padBA[0x180 - 0xBA];
} Cam1510B458;

#define CAM_1510B458 ((Cam1510B458 *)D_800BE628)

void func_1510B458(s32 arg0, f32 near, f32 far) {
    if (near == 0.0f) {
        near = D_800D9B20;
    }
    if (far == 0.0f) {
        far = D_800D9B1C;
    }
    func_1510B5F8(arg0, (u16 *)(D_800BE628 + arg0 * 0x180 + 0xB8), CAM_1510B458[arg0].fovy, CAM_1510B458[arg0].aspect, near, far, CAM_1510B458[arg0].scale);
    D_800D9AF8[arg0].unk0 = near;
    D_800D9AF8[arg0].unk4 = far;
    D_800D9B18 = 1;
}
#undef CAM_1510B458


#define CAM_1510B51C ((Cam1510B458 *)D_800BE628)

void func_1510B51C(void) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (D_800D9AF8[i].unk0 != -1.0f) {
            func_1510B5F8(i, (u16 *)(D_800BE628 + i * 0x180 + 0xB8), CAM_1510B51C[i].fovy, CAM_1510B51C[i].aspect, D_800D9AF8[i].unk0, D_800D9AF8[i].unk4, CAM_1510B51C[i].scale);
            D_800D9AF8[i].unk0 = -1.0f;
        }
    }
    D_800D9B18 = 0;
}
#undef CAM_1510B51C


void func_1510B5F8(s32 arg0, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale) {
    guPerspectiveF((f32 (*)[4])((u8 *)D_800BE628 + arg0 * 0x180 + 0xBC), perspNorm, fovy, aspect, near, far, scale);
    guMtxF2L((f32 (*)[4])((u8 *)D_800BE628 + arg0 * 0x180 + 0xBC), (Mtx *)((u8 *)D_800BE628 + arg0 * 0x180 + D_800BE9C0 * 0x40 + 0x100));
}
void func_1510B690(void);

extern s32 D_800BE628;
extern u8 D_800BE9C0;
extern struct10 *D_800DC2A0;
extern u8 D_80089470;

#define WGFX2(pkt, a, b)            \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}
Gfx *func_1510B7B4(Gfx *gfx, s32 idx);

typedef struct Rec1510B958 {
    char pad0[0x64];
    /* 0x64 */ f32 unk64;
    /* 0x68 */ f32 unk68;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 unk70;
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 unk78;
    char pad7C[0x180 - 0x7C];
} Rec1510B958;

extern f32 D_800D35E0;
extern f32 D_800D35E8; /* 0x800D35E4 == (&D_800D35E8)[-1]; it has no symbol of its own */
#define D_800D35E4 ((&D_800D35E8)[-1])
void func_1510B958(s32 arg0);

extern u8 D_800C3662;
extern u8 D_800BEBA0;
extern LookAt D_800D9B90[];
extern u8 D_800D9BD0[][2][8];
extern s32 D_800D9E10[];
extern u8 D_800D9E20;
extern u8 D_800D9E21;
extern s32 D_800D9E28;
extern s32 D_800B0E08;
void func_1510CB10(s32 arg0);
Gfx *func_1510CDB8(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3);
void *func_151733D8(void *, s32);
Gfx *func_150C8600(Gfx *);
Gfx *func_15100464(Gfx *);
Gfx *func_150DFBD0(Gfx *);
Gfx *func_150CF5E8(Gfx *);
Gfx *func_150D765C(Gfx *);
void func_1510F800(s32);
void *func_1515D914(void *, s16, s32, s32, s32, s32, s32, s32, void *, void *, s32, s32, s32, void *);
void *func_1515E544(void *, s32, u8, u8, void *);
Gfx *func_150A5378(s32, Gfx *, void *, s32, s32, s32);
Gfx *func_150A50C0(s32, Gfx *, void *, s32, s32, s32);
Gfx *func_1512E5F0(Gfx *, struct108 *);
Gfx *func_151742EC(Gfx *, s16);

#define WG(a, b) { Gfx *_g = (Gfx *)arg0; arg0 += sizeof(Gfx); _g->words.w0 = (u32)(a); _g->words.w1 = (u32)(b); }
s32 func_1510B9D0(s32 arg0, s16 idx);

typedef struct {
    /* 0x00 */ s16 tc0;
    /* 0x02 */ s16 tc1;
} UvWobbleSrc;

typedef struct {
    /* 0x00 */ UvWobbleSrc *unk00;
    /* 0x04 */ UvWobbleSrc *unk04;
    /* 0x08 */ u8 unk08;
    /* 0x09 */ u8 unk09;
    /* 0x0A */ u8 unk0A;
    /* 0x0B */ u8 unk0B;
    /* 0x0C */ u8 unk0C;
    /* 0x0D */ u8 unk0D;
    /* 0x0E */ u16 unk0E;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
} UvWobble;

extern UvWobble *D_800D9E60[];
extern Vtx *D_800B0E10[];
extern Vtx *D_800B0E14;
extern s32 D_800917B8;
extern f32 sinf(f32);
extern f32 cosf(f32);
extern void *allocate_memory(s32, s32, s32, s32);
s32 func_1510BF60(s32 dl, s32 segAddr, Vtx **outVtx, s32 skip);
UvWobble *func_1510C4AC(s32 dl, s32 arg1, s32 arg2, s32 arg3);
void func_1510C8A8(void);
