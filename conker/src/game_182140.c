#include <ultra64.h>
#define func_15169260 func_15169260_s32_decl
#include "functions.h"
#undef func_15169260
#include "variables.h"

struct Local1515572CData {
    s32 unk0;
    s32 unk4;
};

extern struct Local1515572CData D_800A6038;
extern void func_15169260(struct Local1515572CData *, s32, s32, u8);
extern s32 (*D_8008ACC8[])(void *);

typedef struct {
    u8 pad0[0x22];
    s16 unk22;
    u16 unk24;
    s16 unk26;
    s16 unk28;
    u8 pad2A[0x4];
    u8 unk2E;
    u8 pad2F[0x39];
    u8 unk68;
} Struct15154C90;

void func_15154C90(Struct15154C90 *arg0) {
    s32 temp;
    s8 failed;

    failed = 0;
    if (arg0->unk24 & 1) {
        arg0->unk22 -= D_800BE9E4;
        if (arg0->unk22 < 0) {
            failed = 1;
        }
    }
    temp = arg0->unk68 & 0xF;
    if (temp != 0) {
        if (failed == 0) {
            s32 ret;

            ret = D_8008ACC8[temp](arg0);
            if (ret == 0) {
                failed = 1;
            }
        }
    }
    if (arg0->unk24 & 8) {
        s16 temp_v1;

        temp_v1 = arg0->unk22;
        if (temp_v1 < arg0->unk26) {
            temp = (s16)(temp_v1 * (u32)arg0->unk28);
            if (temp < arg0->unk2E) {
                arg0->unk2E = temp;
            }
        }
    }
    if (failed) {
        func_1516972C((struct102 *) arg0);
    }
}

typedef struct {
    u8 pad0[0x6];
    u16 unk6;
    u16 unk8;
    u8 padA[0x2];
} Tex15154D80;
typedef struct {
    s32 unk0;
    s32 unk4;
} Mode15154D80;
typedef struct {
    u8 pad0[0x14];
    f32 unk14;
    f32 unk18;
    u8 pad1C[0x18];
    f32 unk34;
    f32 unk38;
} Cam15154D80;
typedef struct {
    u8 pad0[0x10];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8 unk20;
    u8 pad21[3];
    u16 unk24;
    u8 pad26[5];
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 pad44[3];
    u8 unk47;
    s32 unk48;
    s32 unk4C;
    s8 unk50;
    s8 unk51;
    u8 pad52[2];
    u8 unk54;
    u8 pad55[3];
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
} Arg15154D80;
extern Tex15154D80 D_80090B60[];
extern Mode15154D80 D_800A4AC8[];
extern s32 D_800D2C9C;
extern s32 func_15181CC8(s32);
extern s32 func_1517EF00(s32);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
void func_151441A4(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, u8 arg8, u8 arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13);
void func_151442FC(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13);

Gfx *func_15154D80(Gfx *gfx, Arg15154D80 *arg1, s16 arg2) {
    s32 s;
    u8 update;
    u16 texW;
    s32 t;
    s32 dsdx;
    s32 dtdy;
    u32 pad_texH;
    f32 x;
    f32 y;
    f32 hw;
    f32 sw;
    f32 sh;
    f32 xl;
    f32 yl;
    f32 hh;
    f32 texWf;
    f32 texHf;
    f32 xh;
    f32 yh;
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;
    Cam15154D80 *cam;

    update = 1;
    cam = (Cam15154D80 *)(D_800BE628 + D_80082FA4 * 0x180);
    if ((arg1->unk24 & 0x10) && (arg2 != arg1->unk54)) {
        return gfx;
    }
    if ((arg1->unk24 & 0x40) && (D_800C35EA == 1)) {
        return gfx;
    }
    if ((arg1->unk24 & (1 << (arg2 + 11))) && ((func_15181CC8(arg2) == 0) || (func_1517EF00(arg2) > 200))) {
        return gfx;
    }
    if (arg1->unk20 != 0xFF) {
        gfx = func_15142E24(gfx, (s32)&D_80090B60[arg1->unk20], 0, (arg1->unk24 & 0x200) ? 0 : 2, 0, 0, arg1->unk20, 0, 0, &update, (arg1->unk24 & 0x400) ? 0x3E : 3);
    }
    gfx = func_15142B7C(gfx, arg1->unk38, arg1->unk3C);
    func_151441A4(&primR, &primG, &primB, &primA, arg1->unk2F, arg1->unk30, arg1->unk31, arg1->unk32,
                  arg1->unk2B, arg1->unk2C, arg1->unk2D, arg1->unk2E, arg1->unk33, arg1->unk50);
    func_151442FC(&envR, &envG, &envB, &envA, arg1->unk2F, arg1->unk30, arg1->unk31, arg1->unk32,
                  arg1->unk2B, arg1->unk2C, arg1->unk2D, arg1->unk2E, arg1->unk33, arg1->unk51);
    gfx = func_15142C10(gfx, envR, envG, envB, envA, &update);
    gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &update);
    gfx = func_1513F4E4(gfx, arg1->unk47, &update);
    gfx = func_15142FBC(gfx, arg1->unk34 | D_800D2C9C | 0x2000 | 0xC00 | arg1->unk48 | arg1->unk4C,
                        D_800A4AC8[arg1->unk40].unk0 | D_800A4AC8[arg1->unk40].unk4, &update);
    x = cam->unk34 + arg1->unk10 * cam->unk14;
    y = cam->unk38 + arg1->unk14 * cam->unk18;
    while (0) {}
    hw = arg1->unk18 * cam->unk14;
    hh = arg1->unk1C * cam->unk18;
    xl = (x - hw) * 4.0f;
    yl = (y - hh) * 4.0f;
    xh = (x + hw) * 4.0f;
    yh = (y + hh) * 4.0f;
    texW = D_80090B60[arg1->unk20].unk6;
    texWf = (u32)D_80090B60[arg1->unk20].unk6;
    while (0) {}
    texHf = (u32)D_80090B60[arg1->unk20].unk8;
    sw = (xh - xl) + 1.0f;
    sh = (yh - yl) + 1.0f;
    if (arg1->unk24 & 0x80) {
        sw *= arg1->unk58;
        sh *= arg1->unk5C;
    }
    if (sw != 0.0f) {
        dsdx = (4096.0f * texWf) / sw;
    } else {
        dsdx = 0;
    }
    if (sh != 0.0f) {
        dtdy = (4096.0f * texHf) / sh;
    } else {
        dtdy = 0;
    }
    s = 0;
    t = 0;
    if (arg1->unk24 & 2) {
        s = (texW - 1) << 5;
        dsdx = -dsdx & 0xFFFF;
    }
    if (arg1->unk24 & 4) {
        t = (D_80090B60[arg1->unk20].unk8 - 1) << 5;
        dtdy = -dtdy & 0xFFFF;
    }
    if (arg1->unk24 & 0x100) {
        s = s + arg1->unk60;
        t = t + arg1->unk64;
    }
    gSPScisTextureRectangle(gfx++, (s32)xl, (s32)yl, (s32)xh, (s32)yh, 0, s, t, dsdx, dtdy);
    return gfx;
}

struct Arg1515548C {
    u8 pad0[8];
    f32 unk8;
    f32 unkC;
    u8 pad10[4];
    u16 unk14;
    u8 pad16[0x42];
};

extern s32 func_151555AC();
extern u8 *func_15167A68(s32, s32, s32, s32, u8, s32);

u8 *func_1515548C(struct Arg1515548C *arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    u8 *ret;
    f32 pos[2];

    if ((arg2 != 0) && (arg3 > 0)) {
        pos[0] = arg0->unk8;
        pos[1] = arg0->unkC;
        if (func_151555AC(arg0, pos) != 0) {
            return 0;
        }
    }
    ret = func_15167A68((arg0->unk14 & 0x20) ? 0x57 : 0x5D, arg6, arg4 + 0x70, 1, arg5, 1);
    if (ret == 0) {
        return 0;
    }
    memcpy(ret + 0x10, arg0, 0x58);
    ret[0x68] = 0;
    ret[0x68] |= arg1;
    return ret;
}

struct Local15155564 { u8 pad[0x2A]; u8 unk2A; };
extern void (*D_8008AD04[])(struct Local15155564 *, s32, u8);

void func_15155564(struct Local15155564 *arg0, s32 arg1, u8 arg2) {
    if (D_8008AD04[arg0->unk2A] != NULL) {
        D_8008AD04[arg0->unk2A](arg0, arg1, arg2);
    }
}

struct Node151555AC {
    u8 pad0[8];
    struct Node151555AC *next;
    u8 padC[4];
    f32 pos[2];
    f32 size[2];
    u8 pad20[0xA];
    u8 unk2A;
};
struct Ids151555AC { s32 id[2]; };
extern struct Ids151555AC D_800A6030;
extern u8 D_800DCE50[];

s32 func_151555AC(f32 *arg0, f32 *arg1, s32 *arg2, s32 arg3) {
    s32 j;
    struct Ids151555AC ids;
    u8 i;
    struct Node151555AC *node;
    struct Node151555AC *next;
    s32 found;
    s32 k;
    f32 w;
    f32 h;
    i = 0;
    for (j = 0; j < 2; j++) {
        ids = D_800A6030;
        i &= 0;
        do {
            node = ((struct Node151555AC *(*)[0x68])D_800DCE50)[j][ids.id[i]];
            while (node != NULL) {
                next = node->next;
                k = arg3;
                found = 0;
                while (k > 0 && !found) {
                    if (node->unk2A == arg2[k - 1]) {
                        found = 1;
                    } else {
                        k--;
                    }
                }
                if (found) {
                    w = arg1[0] + node->size[0];
                    h = arg1[1] + node->size[1];
                    if (arg0[0] - w <= node->pos[0] && node->pos[0] <= arg0[0] + w &&
                        arg0[1] - h <= node->pos[1] && node->pos[1] <= arg0[1] + h) {
                        return 1;
                    }
                }
                node = next;
            }
lbl:
            i++;
        } while (i < 2);
        i = 0;
    }
}
void func_1515572C(s32 arg0, u8 arg1) {
    struct Local1515572CData sp18;

    sp18 = D_800A6038;
    func_15169260(&sp18, 2, arg0, arg1);
}
