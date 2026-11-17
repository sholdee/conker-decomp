#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 cosf(f32);
extern f32 sinf(f32);
extern f32 D_800A7154;
extern f32 D_800A7158;
extern s32 D_800BE9E4;
extern void func_1510E82C(s32 *, s32, f32 *, s32, s32 *, s32, f32, f32, f32, f32, s32, s32);

struct Obj15171CA0 {
    char pad0[0x16];
    s16 unk16;
    char pad18[0x26 - 0x18];
    s16 unk26;
    char pad28[0x36 - 0x28];
    s16 unk36;
    char pad38[0x46 - 0x38];
    s16 unk46;
    char pad48[0x50 - 0x48];
    s16 unk50;
    s16 unk52;
    s16 unk54;
    u16 unk56;
    u8  unk58;
    u8  unk59;
};

extern struct Obj15171CA0 *func_15167A68(s32, s32, s32, s32, s32, s32);

struct Obj15171CA0 *func_15171CA0(s16 arg0, u16 arg1, u8 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, s32 arg7) {
    struct Obj15171CA0 *ret = func_15167A68(arg5 == 0 ? 0xF : 0x43, arg7, 0x60, 1, arg6, 1);
    if (ret == 0) {
        return NULL;
    }
    ret->unk50 = arg0;
    ret->unk52 = 0;
    ret->unk54 = arg4;
    ret->unk56 = arg1;
    ret->unk58 = arg2;
    ret->unk59 = arg3;
    ret->unk16 = 0;
    ret->unk26 = 0;
    ret->unk36 = 0;
    ret->unk46 = 0;
    return ret;
}

struct Some15171F04 {
    u8 pad0[4];
    u8 unk4;
    u8 pad5;
    u16 unk6;
    u16 unk8;
};
extern struct Some15171F04 *D_8008CA4C[];
s32 func_15172B20(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, u8 arg7);
typedef struct {
    s16 ob[3];
    u16 flag;
    s16 tc[2];
    u8 cn[4];
} VtxD4C;

#define VTX ((VtxD4C *)((u8 *)temp + 0x10))

void func_15171D4C(f32 arg0, f32 arg1, f32 arg2, s16 arg3, u16 arg4, u8 arg5, f32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, u8 argC, s32 argD) {
    struct Obj15171CA0 *temp;

    temp = func_15171CA0(arg3, arg4, arg5, arg9, argA, argB, argC, argD);
    if (temp != NULL) {
        if (!(arg9 & 2)) {
            if (func_15172B20((u8 *)temp + 0x10, arg0, arg1, arg2, arg6, arg7, arg8, D_8008CA4C[temp->unk58]->unk6) == 0) {
                func_1516972C((struct102 *)temp);
            }
        } else {
            VTX[0].ob[0] = VTX[3].ob[0] = arg0 - (f32)arg8;
            VTX[1].ob[0] = VTX[2].ob[0] = (f32)arg8 + arg0;
            VTX[0].ob[1] = VTX[1].ob[1] = VTX[2].ob[1] = VTX[3].ob[1] = arg1;
            VTX[0].ob[2] = VTX[1].ob[2] = arg2 - (f32)arg8;
            VTX[2].ob[2] = VTX[3].ob[2] = (f32)arg8 + arg2;
            VTX[0].tc[0] = VTX[3].tc[0] = 0x2000;
            VTX[1].tc[0] = VTX[2].tc[0] = (D_8008CA4C[arg5]->unk6 + 0x100) << 5;
            VTX[0].tc[1] = VTX[1].tc[1] = 0x2000;
            VTX[2].tc[1] = VTX[3].tc[1] = (D_8008CA4C[arg5]->unk8 + 0x100) << 5;
        }
    }
}

#undef VTX

s32 func_151725FC(void *arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7);

void func_15171F04(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s16 arg4, u16 arg5, u8 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, u8 arg11, s32 arg12) {
    struct Obj15171CA0 *temp;

    temp = func_15171CA0(arg4, arg5, arg6, arg8, arg9, arg10, arg11, arg12);
    if (temp != NULL) {
        if (func_151725FC((u8 *)temp + 0x10, arg3, arg0, arg1, arg2, 0.0f, arg7, D_8008CA4C[temp->unk58]->unk6) == 0) {
            func_1516972C((struct102 *)temp);
        }
    }
}

void func_15171FC0(struct Obj15171CA0 *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->unk50;
    if (temp_v0 < -1) {
        arg0->unk50 = temp_v0 + 1;
        if (arg0->unk50 == -1) {
            func_1516972C((struct102 *)arg0);
        }
        return;
    }

    if (temp_v0 == 0) {
        arg0->unk50 = -3;
        return;
    }

    if (temp_v0 == -1) {
        return;
    }

    if ((arg0->unk59 & 4) == 0) {
        if (D_800BE9E4 < temp_v0) {
            arg0->unk50 = temp_v0 - D_800BE9E4;
        } else {
            arg0->unk50 = 0;
        }
    }

    arg0->unk52 += arg0->unk54;
    temp_v0 = D_8008CA4C[arg0->unk58]->unk4;
    if ((arg0->unk52 / 256) < temp_v0) {
        return;
    }

    if (arg0->unk59 & 4) {
        arg0->unk50 = -3;
    } else {
        arg0->unk52 = (temp_v0 << 8) - 0x100;
    }
}

extern s32 D_800DD1B4;
extern s32 D_800DD1B8;
extern s16 D_800DD1BC;
extern s16 D_800DD1C0;
extern s16 D_800DD1C2;
extern s16 D_800DD1C4;
extern s16 D_800DD1C6;
extern s32 D_800D2C9C;
struct Some151720C4 {
    s32 unk0;
    u8 pad4[6];
    u8 unkA;
};
struct Sprite151720C4 { s32 unk0[5]; };
#define WGFX151720C4(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (u32)(a); _g->words.w1 = (u32)(b); }
Gfx *func_15094F70(Gfx *, s32, s32, s32, s32, s32, s32, s32, s32);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

Gfx *func_151720C4(Gfx *gfx, struct Obj15171CA0 *arg1, s32 arg2) {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    s32 ptr;
    s32 cyc;
    s32 mode;
    s32 newCyc;
    s32 newMode;
    struct Sprite151720C4 sprite;
    s32 tex;
    s32 frame;
    s32 update;
    struct Some151720C4 *entry;
    s32 othermodeH;
    s32 othermodeL;

    update = 1;
    if (arg1->unk50 >= 2) {
    entry = (struct Some151720C4 *)D_8008CA4C[arg1->unk58];
    frame = arg1->unk52 >> 8;
    ptr = entry->unk0;
    tex = ptr;
    if ((u32)ptr >= 0x10000000) {
        tex = ((s32 *)ptr)[frame];
        frame = 0;
    }
    if (tex != D_800DD1B4 || frame != D_800DD1B8) {
        gfx = func_15094F70(gfx, (s32)entry, arg1->unk52, (s32)&sprite, 0, 0, 0, 2, 3);
        D_800DD1B4 = tex;
        D_800DD1B8 = frame;
    }
    cyc = D_800DD1BC & 0xF;
    mode = D_800DD1BC & 0xF0;
    if (!(arg1->unk59 & 1)) {
        newCyc = 1;
    } else {
        newCyc = 2;
    }
    if (entry->unkA == 5) {
        newCyc += 2;
    }
    if (cyc != newCyc) {
        cyc = newCyc;
        if (update) {
            gDPPipeSync(gfx++);
        }
        update = 0;
        switch (newCyc) {
        case 1:
            othermodeH = 0;
            othermodeL = 0x504DD8;
            break;
        case 2:
            othermodeH = 0;
            othermodeL = 0x504B50;
            break;
        case 3:
            othermodeH = 0x100000;
            othermodeL = 0xC184DD8;
            break;
        case 4:
            othermodeH = 0x100000;
            othermodeL = 0xC184B50;
            break;
        }
        gDPSetAlphaCompare(gfx++, G_AC_NONE);
        gSPClearGeometryMode(gfx++, G_FOG | G_LOD);
    }
    gfx = func_15142FBC(gfx, othermodeH | G_TP_PERSP | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_DISABLE, othermodeL, (u8 *)&update);
    if (!(arg1->unk59 & 8)) {
        if (entry->unkA == 0) {
            if (arg1->unk56) {
                newMode = 0x10;
            } else {
                newMode = 0x20;
            }
        } else if (entry->unkA == 5) {
            if (arg1->unk56) {
                newMode = 0x60;
            } else {
                newMode = 0x50;
            }
        } else {
            newMode = 0x30;
        }
    } else {
        newMode = 0x40;
    }
    if (mode != newMode) {
        mode = newMode;
        if (update) {
            gDPPipeSync(gfx++);
        }
        switch (newMode) {
        case 0x10:
            WGFX151720C4(gfx++, 0xFC30B261, 0xFF67FFFF);
            break;
        case 0x20:
            WGFX151720C4(gfx++, 0xFCFFB3FF, 0xFF64FE7F);
            break;
        case 0x30:
            WGFX151720C4(gfx++, 0xFCFF97FF, 0xFF2DFEFF);
            break;
        case 0x40:
            WGFX151720C4(gfx++, 0xFCFFFFFF, 0xFFFCF279);
            break;
        case 0x50:
            WGFX151720C4(gfx++, 0xFCFFB5FF, 0xFFFCFE38);
            break;
        case 0x60:
            WGFX151720C4(gfx++, 0xFC30B5FF, 0xFFFFFE38);
            break;
        }
    }
    D_800DD1BC = cyc | mode;
    r = (arg1->unk56 >> 8) & 0xF8;
    g = (arg1->unk56 >> 3) & 0xF8;
    b = (arg1->unk56 << 2) & 0xF8;
    if (arg1->unk50 >= 0x1A) {
        a = 0xFF;
    } else {
        a = arg1->unk50 * 10;
    }
    if (r != D_800DD1C0 || g != D_800DD1C2 || b != D_800DD1C4 || a != D_800DD1C6) {
        D_800DD1C0 = r;
        D_800DD1C2 = g;
        D_800DD1C4 = b;
        D_800DD1C6 = a;
        if (update) {
            gDPPipeSync(gfx++);
        }
        gDPSetPrimColor(gfx++, 1, 0, r, g, b, a);
    }
    gSPVertex(gfx++, (u8 *)arg1 + 0x10, 4, 0);
    gSP1Triangle(gfx++, 0, 1, 2, 0);
    gSP1Triangle(gfx++, 0, 2, 3, 0);
    }
    return gfx;
}

typedef struct {
    s16 ob[3];
    u16 flag;
    s16 tc[2];
    u8 cn[4];
} Vtx151725FC;

extern s32 func_1510E388(s32, void *, f32 *, f32 *);
extern s16 D_800A6FD0[];
extern s16 D_800A6FE0[];
extern s16 D_800A6FF0[];

s32 func_151725FC(void *arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7) {
    f32 lenA;
    f32 lenB;
    f32 minsq;
    f32 denom;
    f32 unused1;
    f32 unused2;
    f32 unused3;
    f32 v[2][3];
    f32 unused4;
    f32 angle;
    f32 z;
    f32 x;
    f32 cx;
    f32 cy;
    f32 cs;
    f32 sn;
    s16 (*tbl)[2];
    Vtx151725FC *vtx;
    s32 n;
    s32 i;

    vtx = arg0;
    n = arg6 / 16;
    if (n == 0) {
        n = 1;
    }
    if (func_1510E388(arg1, 0, &x, &z) == 0) {
        return 0;
    }
    v[0][0] = 0.0f;
    v[0][2] = -n;
    v[0][1] = x * v[0][0] + z * v[0][2];
    denom = (z + z) * (-v[0][1] - v[0][1]) + ((-v[0][2] - v[0][2]) + (-v[0][2] - v[0][2]));
    if (denom == 0.0f) {
        denom = 0.001000000047f;
    }
    v[1][0] = -n;
    v[1][1] = ((x * v[1][0] + x * v[1][0]) * (-v[0][2] - v[0][2])) / denom;
    if (z == 0.0f) {
        v[1][2] = 0.0f;
    } else {
        v[1][2] = (v[1][1] - x * v[1][0]) / z;
    }
    lenA = v[0][0] * v[0][0] + v[0][1] * v[0][1] + v[0][2] * v[0][2];
    if (lenA > 100000000.0f) {
        lenA = 100000000.0f;
    }
    minsq = n * n;
    if (lenA < minsq) {
        lenA = minsq;
    }
    lenA = sqrtf(lenA);
    lenB = v[1][0] * v[1][0] + v[1][1] * v[1][1] + v[1][2] * v[1][2];
    if (lenB > 100000000.0f) {
        lenB = 100000000.0f;
    }
    if (lenB < minsq) {
        lenB = minsq;
    }
    lenB = sqrtf(lenB);
    for (i = 0; i < 3; i++) {
        v[0][i] = v[0][i] * arg6 / lenA;
        v[1][i] = v[1][i] * arg6 / lenB;
        vtx[0].ob[i] = v[0][i];
        vtx[1].ob[i] = v[1][i];
        vtx[2].ob[i] = -v[0][i];
        vtx[3].ob[i] = -v[1][i];
    }
    angle = (arg5 - 225.0f) * 0.01745329238f;
    cs = cosf(angle);
    sn = sinf(angle);
    if ((u8)arg7 == 0x10) {
        cy = cx = 256.0f;
        tbl = (s16 (*)[2])D_800A6FF0;
    } else if ((u8)arg7 == 0x20) {
        cy = cx = 512.0f;
        tbl = (s16 (*)[2])D_800A6FE0;
    } else {
        cy = cx = 1024.0f;
        tbl = (s16 (*)[2])D_800A6FD0;
    }
    for (i = 0; i < 4; i++) {
        ((Vtx151725FC *)arg0)[i].tc[0] = ((tbl[i][0] - cx) * cs - (tbl[i][1] - cy) * sn) + cx + 8192.0f;
        ((Vtx151725FC *)arg0)[i].tc[1] = ((tbl[i][0] - cx) * sn + (tbl[i][1] - cy) * cs) + cy + 8192.0f;
    }
    for (i = 0; i < 4; i++) {
        vtx[i].ob[0] = vtx[i].ob[0] + arg2;
        vtx[i].ob[1] = vtx[i].ob[1] + arg3;
        vtx[i].ob[2] = vtx[i].ob[2] + arg4;
    }
    return 1;
}


s32 func_15172B20(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, u8 arg7) {
    struct sp15172B20 {
        s32 unk0;
        f32 unk4;
        s32 unk8;
        s32 unkC;
    };
    f32 temp_f0;
    f32 temp_f6;
    struct sp15172B20 sp40;
    f32 temp_f12;

    temp_f12 = (arg4 - 90.0f) * D_800A7154;
    temp_f6 = cosf(temp_f12);
    temp_f0 = sinf(temp_f12);
    arg1 += (f32) arg5 * temp_f0;
    arg3 += (f32) arg5 * temp_f6;
    func_1510E82C(&sp40.unk0, 0, &sp40.unk4, 0, &sp40.unk8, 0, arg1, arg2, arg3, arg2, 0, 0);
    if (sp40.unk0 == 0) {
        return 0;
    }
    if (sp40.unk4 == D_800A7158) {
        return 0;
    }
    return func_151725FC(arg0, sp40.unk0, arg1, sp40.unk4, arg3, arg4, arg6, arg7);
}
