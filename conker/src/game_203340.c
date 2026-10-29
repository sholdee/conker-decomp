#include <ultra64.h>
#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_203340/func_151D5E90.s")

void func_151D61B0(u16 *arg0) {
    s32 y;
    s32 x;
    u16 *p;
    u16 *row;
    u16 prev;
    u16 cur;
    u16 next;
    u16 r;
    u16 g;
    u16 b;
    s32 w = D_800BE620;
    s32 h = D_800BE624;

    row = arg0;
    for (y = 0; y < h; y++) {
        p = row + 1;
        cur = row[1];
        next = row[2];
        for (x = 1; x < w - 1; x++) {
            prev = cur;
            cur = next;
            next = p[2];
            r = (cur >> 11) & 0x1F;
            r += (prev >> 12) & 0xF;
            r += (next >> 12) & 0xF;
            g = (cur >> 6) & 0x1F;
            g += (prev >> 7) & 0xF;
            g += (next >> 7) & 0xF;
            b = (cur >> 1) & 0x1F;
            b += (prev >> 2) & 0xF;
            b += (next >> 2) & 0xF;
            prev = ((r & 0x3E) << 10) | ((g & 0x3E) << 5) | (b & 0x3E) | 1;
            p[1] = prev;
            p++;
        }
        row += w;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_203340/func_151D6418.s")

void func_151D66F0(s32 arg0, s32 arg1) {
    if (D_800BE9F0 == 6 && D_80038080 == 0) {
        return;
    }
    if (arg1 == 0) {
        arg0 = 0;
    }
    D_800BE574 = arg0;
    if (arg0 != 0) {
        D_800BE575 = arg1;
    } else {
        D_800BE575 = 0;
    }
    if (arg0 == 0) {
        if (D_800BE570 != 0) {
            func_100043B4((s32 *)D_800BE570, 3);
            D_800BE570 = 0;
        }
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_203340/func_151D6778.s")
