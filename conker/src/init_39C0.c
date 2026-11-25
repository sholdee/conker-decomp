#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_100039C0(void) {
    OSViMode *mode;

    D_800BE620 = 292; // screen width px
    D_800BE624 = 216; // screen height px
    D_800380A0 = (f32) D_800BE620 / 292.0f;
    D_800380A4 = (f32) D_800BE624 / 216.0f;
    D_800BE9C4 = allocate_memory(D_800BE620 * D_800BE624 * 2, 255, 3, 0);
    func_10003ACC(0, 0, 0);
    func_15015FBC(D_800BE620, D_800BE624);
    if (D_80000300 == 2) {
        mode = &D_8002ABE0;
    } else {
        mode = &D_8002AB90;
    }
    osViSetMode(mode);
    osViSwapBuffer((void *)D_8002AAE8[D_800BE9C0 ^ 1]);
}

void func_10003ACC(s32 red, s32 green, s32 blue) {
    u16 *pixels;
    u16 *second;
    u32 byteCount;
    s32 count;
    s32 i;
    u32 color;

    byteCount = (u32)D_800BE620 * (u32)D_800BE624 * 2;
    pixels = D_8002AAE8[0];
    if (pixels != 0) {
        color = (((u32)red << 8) & 0xF800) | (((u32)green << 3) & 0x7C0) | ((blue >> 2) & 0x3E) | 1;
        count = (s32)byteCount >> 1;
        second = pixels;
        for (i = 0; i < count; i++) {
            pixels = &(*(u16 (*)[])second)[i + 1];
            pixels[-1] = color;
        }
        second = D_8002AAE8[1];
        for (i = 0; i < count; i++) {
            second[i] = color;
        }
    }
}
