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

#pragma GLOBAL_ASM("asm/nonmatchings/game_1AB530/func_1517E28C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1AB530/func_1517E4A8.s")

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

#pragma GLOBAL_ASM("asm/nonmatchings/game_1AB530/func_1517EAAC.s")

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

