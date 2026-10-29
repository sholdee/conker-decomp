#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void *allocate_memory(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

typedef struct Node_1517E134 {
    char pad0[0xC];
    f32 unkC;
    f32 unk10;
    char pad14[0x10];
    struct Node_1517E134 *next;
    s16 unk28;
    char pad2A[4];
    s8 unk2E;
    s8 unk2F;
    s8 unk30;
} Node_1517E134;

Node_1517E134 *func_1517E080(s32 arg0, s32 arg1) {
    Node_1517E134 *cur;
    Node_1517E134 **slot;

    cur = (Node_1517E134 *)D_800DDD64;
    if (cur != NULL) {
        if (cur->next != NULL) {
            do {
                cur = cur->next;
            } while (cur->next != NULL);
        }
        slot = &cur->next;
    } else {
        slot = (Node_1517E134 **)&D_800DDD64;
    }

    cur = allocate_memory(0x34, 1, 0, 1);
    if (cur == NULL) {
        return NULL;
    }
    *slot = cur;
    cur->next = NULL;
    cur->unk28 = 0;
    cur->unk2E = 0;
    cur->unk10 = 0.0f;
    cur->unkC = 0.0f;
    cur->unk2F = arg0;
    cur->unk30 = arg1;
    return cur;
}

typedef struct Node_1517E1AC {
    char pad0[0xC];
    f32 unkC;
    f32 unk10;
    char pad14[0x10];
    struct Node_1517E1AC *next;
    char pad28[0x2];
    u16 unk2A;
} Node_1517E1AC;

extern void func_10004074(void);

void func_1517E134(Node_1517E134 *arg0) {
    Node_1517E134 *cur;
    Node_1517E134 *prev;

    cur = (Node_1517E134 *)D_800DDD64;
    if (arg0 == cur) {
        D_800DDD64 = (s32)arg0->next;
    } else {
        prev = cur;
        if (cur != NULL) {
            cur = cur->next;
            while (arg0 != cur) {
                prev = cur;
                if (cur == NULL) {
                    break;
                }
                cur = cur->next;
            }
        }
        if (prev != NULL) {
            prev->next = arg0->next;
        } else {
            return;
        }
    }
    func_10004074();
}

void func_1517E1AC(void) {
    Node_1517E1AC *cur;
    s32 *width;
    s32 *height;
    s32 *framebuffer;
    s32 screenWidth;
    s32 ix;
    s32 iy;
    f32 x;
    f32 y;
    f32 zero;

    cur = (Node_1517E1AC *)D_800DDD64;
    framebuffer = (s32 *)((s32)&D_800BE9C4);
    if (cur != NULL) {
        width = (s32 *)((s32)&D_800BE620);
        height = (s32 *)((s32)&D_800BE624);
        zero = 0.0f;
        do {
            x = cur->unkC;
            if (zero <= x) {
                screenWidth = *width;
                if (x < (f32)screenWidth) {
                    y = cur->unk10;
                    if ((zero <= y) && (y < (f32)*height)) {
                        iy = (s32)y;
                        ix = (s32)x;
                        cur->unk2A = *(((u16 *)*framebuffer) + ix + (iy * screenWidth));
                    }
                }
            }
            cur = cur->next;
        } while (cur != NULL);
    }
}

typedef struct {
    u8 unk0[2];
} Pair_1517E28C;

extern Pair_1517E28C D_8008D004;
extern Pair_1517E28C D_8008D008;
extern Pair_1517E28C D_8008D00C;
extern u8 D_800DDD60;

typedef struct Node_1517E28C {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    char pad14[0x10];
    struct Node_1517E28C *next;
    s16 unk28;
    u16 unk2A;
    u16 unk2C;
    u8  unk2E;
    u8  unk2F;
    u8  unk30;
} Node_1517E28C;

extern s32 func_1517EAAC(f32 x, f32 y, f32 z, f32 *outX, f32 *outY);
struct Struct1517EC1C;
extern s32 func_1517EC1C(struct Struct1517EC1C *node, s32 *outDist);
extern Gfx *func_1517E4A8(Gfx *gfx, Node_1517E28C *node, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 *arg6);

Gfx *func_1517E28C(Gfx *gfx, s32 arg1) {
    s32 alpha;
    s32 cull;
    s32 dist;
    Node_1517E28C *node;
    f32 *pos;
    Pair_1517E28C param0 = D_8008D004;
    Pair_1517E28C param1 = D_8008D008;
    Pair_1517E28C param2 = D_8008D00C;
    u8 *flag;

    node = (Node_1517E28C *)D_800DDD64;
    D_800DDD60 = 0;
    if (node != NULL) {
        do {
            if (node->unk30 != 0) {
                cull = 0;
                if (node->unk2E & 1) {
                    if (D_800DCDD0 == 0) {
                        cull = 1;
                    } else {
                        pos = (f32 *)D_8008CFFC[D_800B0DF0->unk10];
                        if (func_1517EAAC(pos[0], pos[1], pos[2], &node->unkC, &node->unk10) != 1) {
                            cull = 1;
                        }
                        alpha = (node->unk2A != 0xFFFC) ? -1 : 0xFF;
                    }
                } else {
                    if (func_1517EC1C((struct Struct1517EC1C *)node, &dist) != 1) {
                        cull = 1;
                    }
                    alpha = ((s32)(node->unk2C - dist) < 0x1E) ? 0x100 : -1;
                }
                flag = (node->unk2E & 2) ? &D_800DD2D0 : NULL;
                if (!cull) {
                    gfx = func_1517E4A8(gfx, node, param0.unk0[node->unk2F], param1.unk0[node->unk2F],
                                        param2.unk0[node->unk2F], alpha, flag);
                } else if (flag != NULL) {
                    *flag = 0;
                }
            }
            node = node->next;
        } while (node != NULL);
    }
    return gfx;
}

typedef struct {
    u8 pad0[6];
    s16 w;
    s16 h;
    u8 alpha;
    u8 r;
    u8 g;
    u8 b;
    u8 padE[2];
    void *tex;
} Sprite_1517E4A8;

extern u8 D_800903AC;
extern u8 D_800A7250[][7];
extern u8 D_800D2DA8;
extern u8 D_800D2DA9;
extern u8 D_800D2DAA;
extern Gfx *func_150950D4(Gfx *, void *, s32, s32, s32, s32, s32, s32, s32, s32);
extern Gfx *func_15095A48(Gfx *, Sprite_1517E4A8 *, f32, f32);
extern Gfx *func_1517EA4C(Gfx *arg0);

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

Gfx *func_1517E4A8(Gfx *gfx, Node_1517E28C *node, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 *arg6) {
    s32 cur;
    s32 i;
    f32 dx;
    f32 dy;
    s32 dist;
    s32 target;
    s32 step;
    s32 v;
    f32 x;
    f32 y;
    Sprite_1517E4A8 sprite;

    dx = node->unkC - D_800BE620 * 0.5f;
    dy = node->unk10 - D_800BE624 * 0.5f;
    if (arg6 != NULL) {
        if (arg5 == -1) {
            target = 0;
        } else {
            dist = ((s32)dx * (s32)dx + (s32)dy * (s32)dy) * arg4 / 10;
            if (dist < 120) {
                target = (120 - dist) * 176 / 120;
            } else {
                target = 0;
            }
        }
        step = D_800BE9E4 * 5;
        cur = *arg6;
        if (ABS(target - cur) < step) {
            cur = target;
        } else if (cur < target) {
            cur += step;
        } else {
            cur -= step;
        }
        *arg6 = cur;
    }
    dist = ((s32)dx * (s32)dx + (s32)dy * (s32)dy) * arg4 / 10;
    if (node->unkC < 2.0f || (f32)(D_800BE620 - 2) <= node->unkC || node->unk10 < 0.0f || (f32)D_800BE624 <= node->unk10) {
        cur = 0;
    } else if (arg5 == -1) {
        cur = 0;
    } else {
        cur = (0x1ADB00 / (dist + 10000)) * arg5 / 256;
    }
    step = D_800BE9E4 * 10;
    if (ABS(cur - node->unk28) < step) {
        node->unk28 = cur;
    } else if (cur < node->unk28) {
        node->unk28 = node->unk28 - step;
    } else {
        node->unk28 = node->unk28 + step;
    }
    if (dist < 200000 && node->unk28 > 0 && fabsf(node->unkC) < 2000.0f && fabsf(node->unk10) < 2000.0f) {
        D_800D2DA8 = 0xFF;
        D_800D2DA9 = 0xFF;
        D_800D2DAA = 0xFF;
        sprite.tex = &D_800903AC;
        sprite.r = 0;
        sprite.g = 0;
        sprite.b = 0;
        if (D_800DDD60 == 0) {
            gfx = func_150950D4(func_1517EA4C(gfx), &D_800903AC, 0, 0, 0, 0, 2, 0x100, 0x100, 3);
            D_800DDD60 = 1;
        }
        cur = node->unk30;
        cur = (node->unk28 * cur) >> 8;
        for (i = 0; i < arg3; i++) {
            x = node->unkC - dx * ((i + 1) * 2) / 5.0f;
            y = node->unk10 - dy * ((i + 1) * 2) / 5.0f;
            v = D_800A7250[arg2][i];
            y += v / 64.0f;
            sprite.alpha = cur;
            sprite.w = (f32)(v << 12) * D_800380A0 / 255.0f;
            sprite.h = (f32)(v << 12) * D_800380A4 / 255.0f;
            D_800D2DAB = 0;
            gfx = func_15095A48(gfx, &sprite, x, y);
            gDPPipeSync(gfx++);
        }
    }
    return gfx;
}

#define WGFX1517EA4C(pkt, a, b)     \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

Gfx *func_1517EA4C(Gfx *arg0) {
    WGFX1517EA4C(arg0++, 0xE7000000, 0);
    WGFX1517EA4C(arg0++, 0xFCFFB3FF, 0xFF65FEFF);
    WGFX1517EA4C(arg0++, 0xEF002C0F, 0x00504344);
    return arg0;
}

void func_15110360(s32 arg0, f32 arg1[4][4], f32 arg2, f32 arg3, f32 arg4);
void func_150A7A00(void *, f32, f32, f32, f32 *, f32 *, f32 *, f32 *);

s32 func_1517EAAC(f32 x, f32 y, f32 z, f32 *outX, f32 *outY) {
    u8 *player;
    f32 pitch;
    f32 yaw;
    f32 roll;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 sw;
    f32 mtx[4][4];
    f32 cx;
    f32 cy;
    f32 scale;

    player = (u8 *)D_800DBFF0 + D_80082FA4 * 0x9A0;
    pitch = -*(f32 *)(player + 0x388);
    yaw = -*(f32 *)(player + 0x380);
    roll = *(f32 *)(player + 0x5EC);
    func_15110360(D_80082FA4, mtx, pitch, yaw, roll);
    cx = *(f32 *)((u8 *)D_800BE628 + D_80082FA4 * 0x180 + 0xC);
    cy = *(f32 *)((u8 *)D_800BE628 + D_80082FA4 * 0x180 + 0x10);
    func_150A7A00(mtx, x, y, z, &sx, &sy, &sz, &sw);
    if (sw > 10.0f) {
        scale = 1.0f / sw;
        sx = sx * cx * scale;
        sy = sy * cy * scale;
        *outX = cx + sx;
        *outY = cy - sy;
        return 1;
    }
    *outX = 0.0f;
    *outY = 0.0f;
    return 0;
}

extern f32 sqrtf(f32);
#pragma intrinsic (sqrtf)
extern u32 D_80089630[][2];
s32 func_1509563C(f32, f32, f32, f32 *, f32 *, f32 *, f32 *, f32);

typedef struct Struct1517EC1C {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8 pad24[0x6];
    u16 unk2A;
    s16 unk2C;
    u8 unk2E;
} Struct1517EC1C;

typedef struct Struct1517EC1CView {
    u8 pad0[0x44];
    s16 unk44;
    u8 pad46[0x6];
    s16 unk4C;
} Struct1517EC1CView;

typedef union Struct1517EC1CPair {
    struct {
        f32 unk0;
        f32 unk4;
    } f;
    f64 align;
} Struct1517EC1CPair;

s32 func_1517EC1C(Struct1517EC1C *arg0, s32 *arg1) {
    f32 temp_f0;
    f32 temp_f2;
    f32 temp_f12;
    f32 temp_f14;
    u16 temp_a3;
    s32 idx;
    Struct1517EC1CPair sp28;

    if (func_1509563C(arg0->unk0, arg0->unk4, arg0->unk8, &arg0->unkC, &arg0->unk10, &sp28.f.unk4, &sp28.f.unk0, 4000.0f) == 1) {
        if (arg0->unk2E & 4) {
            temp_f2 = arg0->unk0 - D_800DBFF0->unk2F8;
            temp_f12 = arg0->unk4 - D_800DBFF0->unk2FC;
            temp_f14 = arg0->unk8 - D_800DBFF0->unk300;

            temp_f2 = ((temp_f2 * arg0->unk14) + (temp_f12 * arg0->unk18) + (temp_f14 * arg0->unk1C)) / sqrtf((temp_f2 * temp_f2) + (temp_f12 * temp_f12) + (temp_f14 * temp_f14));
            if (temp_f2 < arg0->unk20) {
                return 1;
            }
        }

        temp_a3 = arg0->unk2A;
        idx = temp_a3 >> 13;
        *arg1 = ((((temp_a3 >> 2) & 0x7FF) << D_80089630[idx][0]) + D_80089630[idx][1]) >> 3;
        arg0->unk2C = (u32)((((f32)((Struct1517EC1CView *)((u8 *)D_800BE628 + (D_800BE9C0 << 4)))->unk4C) + ((sp28.f.unk4 / sp28.f.unk0) * (f32)((Struct1517EC1CView *)((u8 *)D_800BE628 + (D_800BE9C0 << 4)))->unk44)) * 32.0f);
        return 1;
    }
    return 0;
}

