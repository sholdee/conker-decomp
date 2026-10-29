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

void func_1510B690(void) {
    s32 i;
    f32 sp4C[4][4];
    f32 (*d9d10)[4][4];

    d9d10 = (f32 (*)[4][4])D_800D9D10;
    for (i = 0; i <= D_80082FA0; i = (s16)(i + 1)) {
        guMtxL2F(sp4C, (Mtx *)&((struct259 *)D_800BE628)[i].unkB8[(D_800BE9C0 << 6) + 0x48]);
        guMtxL2F(D_800D9C10[i], (Mtx *)(((s32 *)&D_800DC2A0)[D_800BE9C0] + (i << 6)));
        func_150A7A48(D_800D9C10[i], sp4C, d9d10[i]);
        func_1501B22C(i);
    }
}

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

Gfx *func_1510B7B4(Gfx *gfx, s32 idx) {
    WGFX2(gfx++, 0xE7000000, 0x00000000);
    WGFX2(gfx++, 0xF9000000, 0x00000001);
    WGFX2(gfx++, 0xDA380003, &D_80089470);
    WGFX2(gfx++, 0xDB0E0000, *(u16 *)((u8 *)((struct259 *)D_800BE628 + idx) + 0xB8));
    WGFX2(gfx++, 0xDB040004, 0x00000003);
    WGFX2(gfx++, 0xDB04000C, 0x00000003);
    WGFX2(gfx++, 0xDB040014, 0x0000FFFD);
    WGFX2(gfx++, 0xDB04001C, 0x0000FFFD);
    WGFX2(gfx++, 0xD9EFFFFF, 0x00000000);
    WGFX2(gfx++, 0xDA380007, (u8 *)&((struct259 *)D_800BE628)[idx].unkB8[(D_800BE9C0 << 6) + 0x48]);
    WGFX2(gfx++, 0xDA380005, ((s32 *)&D_800DC2A0)[D_800BE9C0] + (idx << 6));
    WGFX2(gfx++, 0xEF082C3F, 0x00552230);
    return gfx;
}

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

void func_1510B958(s32 arg0) {
    Rec1510B958 *record;

    record = (Rec1510B958 *)((u8 *)D_800BE628 + arg0 * 0x180);
    D_800D35E0 = (((record->unk74 / record->unk6C) - 1.0f) * -1.0f) + record->unk64;
    D_800D35E4 = (((record->unk78 / record->unk70) - 1.0f) * -1.0f) + record->unk68;
}

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
s32 func_1510B9D0(s32 arg0, s16 idx) {
    struct108 *cam;
    s32 flag;

    cam = &D_800DBFF0[idx];
    D_800BEBA0 = 1;
    flag = 0;
    if (D_800C35EA == 1 && D_800C3662 != 0) {
        flag = 1;
    }
    if (D_800D18A0 & (1 << idx)) {
        flag = 1;
    }
    arg0 = (s32)func_1510B7B4((Gfx *)arg0, idx);
    WG(0xDC08000A, &D_800D9B90[D_800BE9C0].l[0]);
    WG(0xDC08060A, &D_800D9B90[D_800BE9C0].l[1]);
    func_1510CB10(idx);
    arg0 = (s32)func_1510CDB8((Gfx *)arg0, 0xFF, 0xFF, idx);
    arg0 = (s32)func_151733D8((Gfx *)arg0, 1);
    WG(0xDA380003, &D_80089470);
    if (!flag) {
        switch (D_800BE9F0) {
        case 0x14:
            arg0 = (s32)func_150C8600((Gfx *)arg0);
            break;
        case 0x1A:
            arg0 = (s32)func_15100464((Gfx *)arg0);
            break;
        case 0x13:
            arg0 = (s32)func_150DFBD0((Gfx *)arg0);
            break;
        case 0x35:
            arg0 = (s32)func_150CF5E8((Gfx *)arg0);
            break;
        case 0x33:
            arg0 = (s32)func_150D765C((Gfx *)arg0);
            break;
        }
    }
    func_1510F800(0);
    if (D_800DCD7C == 0) {
        WG(0xDC280C0A, D_800D9BD0[idx][D_800BE9C0]);
    }
    arg0 = (s32)func_1515D914((Gfx *)arg0, idx, (s32)D_800DBFF0[idx].unk2F8, (s32)D_800DBFF0[idx].unk2FC, (s32)D_800DBFF0[idx].unk300,
                        0, D_800D9E10[idx], D_800D9E20, D_800D9BD0[idx][D_800BE9C0], &D_800D9E21, 1, 0, 0x25, &D_800D9E28);
    if (!flag) {
        if (D_800BE9F0 != 5) {
            if (D_800BEAAA) {
                WG(0xE7000000, 0);
                if (D_800DBE62) {
                    if (D_800BE9F0 != 0x14 && D_800BE9F0 != 0x33 && D_800BE9F0 != 0x32) {
                        arg0 = (s32)func_150A5378(D_800B0E00[0], (Gfx *)arg0, D_800D9C10[idx], 0, 0, 1 << idx);
                    } else {
                        WG(0xDE000000, D_800B0E00[0]);
                    }
                    if (D_800B0E04) {
                        WG(0xE7000000, 0);
                        arg0 = (s32)func_1510CDB8((Gfx *)arg0, 0xFF, 0xFF, idx);
                        arg0 = (s32)func_151733D8((Gfx *)arg0, 1);
                        if (D_800BE9F0 == 0x35) {
                            arg0 = (s32)func_150CF5E8((Gfx *)arg0);
                        }
                        WG(0xDE000000, D_800B0E04);
                    }
                    D_800BEBA0 = 0;
                    if (*((u8 *)cam + 0x8B8) && (cam->unk84 & 8)) {
                        arg0 = (s32)func_1512E5F0((Gfx *)arg0, cam);
                        WG(0xE7000000, 0);
                    }
                } else {
                    WG(0xDE000000, D_800B0E00[0]);
                }
            } else if (D_800BEAAA) {
                arg0 = (s32)func_150A50C0(D_800B0E00[0], (Gfx *)arg0, D_800D9C10[idx], 0, 0, 0);
            } else {
                WG(0xDE000000, D_800B0E00[0]);
            }
        }
        arg0 = (s32)func_1515E544((Gfx *)arg0, D_800D9E10[idx], D_800D9E20, D_800D9E21, D_800D9BD0[idx][D_800BE9C0]);
        if (D_800B0E08) {
            WG(0xE7000000, 0);
            arg0 = (s32)func_1510CDB8((Gfx *)arg0, 0xFF, 0xFF, idx);
            WG(0xDE000000, D_800B0E08);
        }
    }
    arg0 = (s32)func_151742EC((Gfx *)arg0, idx);
    WG(0xE7000000, 0);
    return arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_138520/func_1510BF60.s")

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
