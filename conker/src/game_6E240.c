#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .data: 16 zero bytes between game_6D800's .data and game_6E770's. */
static s32 D_800848C0[4] = { 0, 0, 0, 0 }; /* not referenced */

void func_15040D90(s32 arg0) {

}

void func_15040D9C(s32 arg0) {
    u16 *ptr;
    s32 i;
    s32 j;

    i = D_800BE9C0;
    ptr = D_8002AAE8[i == 0];
    for (j = 0; j != 8; j++) {
        for (i = 0; i != 8; i += 4) {
            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 200) = 0xF800;
            ptr = D_8002AAE8[D_800BE9C0];
            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 200) = 0xF800;
            ptr = D_800BE9C4;
            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 200) = 0;

            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 201) = 0xF800;
            ptr = D_8002AAE8[D_800BE9C0];
            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 201) = 0xF800;
            ptr = D_800BE9C4;
            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 201) = 0;

            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 202) = 0xF800;
            ptr = D_8002AAE8[D_800BE9C0];
            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 202) = 0xF800;
            ptr = D_800BE9C4;
            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 202) = 0;

            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 203) = 0xF800;
            ptr = D_8002AAE8[D_800BE9C0];
            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 203) = 0xF800;
            ptr = D_800BE9C4;
            *(ptr + (arg0 + i) + D_800BE620 * (j + 225) + 203) = 0;
        }
    }
}

void func_15040FCC(s32 arg0) {
    u16 *fb1;
    u16 *fb2;
    u16 *zb;
    s32 i;
    s32 j;

    j = D_800BE9C0 == 0;
    fb1 = D_8002AAE8[j];
    fb2 = D_8002AAE8[D_800BE9C0];
    zb = D_800BE9C4;
    j = 0;
    for (i = 0; i < 8; i++) {
        s32 idx;

        idx = arg0 + i + D_800BE620 * (j + 225) + 200;
        fb1[idx] = 0xF800;
        fb2[idx] = 0xF800;
        zb[idx] = 0;
    }
    for (j = 1; j < 7; j++) {
        for (i = 0; i < 8; i += 7) {
            s32 idx;

            idx = arg0 + i + D_800BE620 * (j + 225) + 200;
            fb1[idx] = 0xF800;
            fb2[idx] = 0xF800;
            zb[idx] = 0;
        }
    }
    for (j = 1; j < 7; j++) {
        for (i = 1; i < 7; i++) {
            s32 idx;

            idx = arg0 + i + D_800BE620 * (j + 225) + 200;
            fb1[idx] = 0;
            fb2[idx] = 0;
            zb[idx] = 0;
        }
    }
    for (i = 0; i < 8; i++) {
        s32 idx;

        idx = arg0 + i + D_800BE620 * (j + 225) + 200;
        fb1[idx] = 0xF800;
        fb2[idx] = 0xF800;
        zb[idx] = 0;
    }
}

void func_1504129C(void) {
    s32 i = 0;
    do {} while (++i < 0x18000000);
}
