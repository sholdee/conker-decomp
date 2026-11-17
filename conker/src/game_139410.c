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
void func_1510B070(s32 arg0);

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
void func_1510B128(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);

void func_1510B128(s32, f32, f32, f32, f32);
void func_1510B32C(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
void func_1510B3B0(void);

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
void func_1510B458(s32 arg0, f32 near, f32 far);
#undef CAM_1510B458


#define CAM_1510B51C ((Cam1510B458 *)D_800BE628)
void func_1510B51C(void);
#undef CAM_1510B51C
void func_1510B5F8(s32 arg0, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
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

#pragma GLOBAL_ASM("asm/nonmatchings/game_139410/func_1510BF60.s")

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

UvWobble *func_1510C4AC(s32 dl, s32 arg1, s32 arg2, s32 arg3) {
    Vtx *vtx;
    s32 count;
    s32 i;
    UvWobble *wob;

    if (D_800D9E64 > 0) {
        return NULL;
    }

    wob = allocate_memory(0x14, 1, 0, 0);
    if (wob == NULL) {
        return NULL;
    }

    D_800D9E60[D_800D9E64++] = wob;
    wob->unk0C = arg1;
    wob->unk00 = NULL;
    wob->unk04 = NULL;
    wob->unk0E = 0;
    wob->unk10 = arg2;
    wob->unk12 = arg3;

    count = func_1510BF60(D_800B0E00[0], dl, &vtx, 0);
    if (count != 0) {
        wob->unk08 = count;
        wob->unk0A = vtx - D_800B0E10[0];
        wob->unk00 = allocate_memory(count * 4, 1, 0, 0);
        for (i = 0; i < count; i++) {
            wob->unk00[i].tc0 = vtx[i].v.tc[0];
            wob->unk00[i].tc1 = vtx[i].v.tc[1];
        }
    }

    count = func_1510BF60(D_800B0E04, D_800917B8, &vtx, 0);
    if (count != 0) {
        wob->unk09 = count;
        wob->unk0B = vtx - D_800B0E14;
        wob->unk04 = allocate_memory(count * 4, 1, 0, 0);
        for (i = 0; i < count; i++) {
            wob->unk04[i].tc0 = vtx[i].v.tc[0];
            wob->unk04[i].tc1 = vtx[i].v.tc[1];
        }
    }

    return wob;
}

void func_1510C8A8(void) {
    s32 i;
    s32 j;
    UvWobble *wob;
    Vtx *vtx;
    f32 ang;
    s16 offX;
    s16 offY;

    for (i = 0; i < D_800D9E64; i++) {
        wob = D_800D9E60[i];
        if (wob->unk0C == 0) {
            if (wob->unk00 != NULL) {
                f32 sn;
                f32 cs;

                wob->unk0E += wob->unk10 * D_800BE9E4;
                vtx = D_800B0E10[0];
                vtx += wob->unk0A;
                ang = wob->unk0E * 9.587380191e-05f;
                sn = sinf(ang);
                cs = cosf(ang);
                offX = (s16)(s32)(wob->unk12 * sn);
                offY = (s16)(s32)(wob->unk12 * cs);
                for (j = 0; j < wob->unk08; j++) {
                    vtx[j].v.tc[0] = wob->unk00[j].tc0 + offX;
                    vtx[j].v.tc[1] = wob->unk00[j].tc1 + offY;
                }
            }
            if (wob->unk04 != NULL) {
                vtx = D_800B0E10[1];
                vtx += wob->unk0B;
                for (j = 0; j < wob->unk09; j++) {
                    vtx[j].v.tc[0] = wob->unk04[j].tc0 + offX;
                    vtx[j].v.tc[1] = wob->unk04[j].tc1 + offY;
                }
            }
        }
    }
}
