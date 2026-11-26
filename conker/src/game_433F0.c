#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/*
 * Sets gIsMultiplayer for the listed maps, which take their player count from func_151E5FAC();
 * every other map uses `count`. D_800BE9E8 gets that count minus one.
 */
void func_15015F40(s16 count, s32 map) {
    switch (map) {
        case 0x1A:
        case 0x24:
        case 0x2B:
        case 0x2D:
        case 0x30:
        case 0x33:
        case 0x34:
        case 0x3F:
            gIsMultiplayer = 1;
            D_800BE9E8 = func_151E5FAC() - 1;
            break;
        default:
            gIsMultiplayer = 0;
            D_800BE9E8 = count - 1;
            break;
    }
}

/* Fills the per-layout view sizes and view centres for a screen of `width` x `height`. */
void func_15015FBC(s32 width, s32 height) {
    s32 i;
    s32 halfW, halfH, quarterH, quarterW;

    for (i = 0; i < 5; i++) {
        D_800BE638[i] = width;
        D_800BE650[i] = height;
        if (i < 4) {
            if (i >= 2) {
                D_800BE638[i] >>= 1;
            }
            if (i >= 1) {
                D_800BE650[i] >>= 1;
            }
        }
    }
    halfW = width >> 1;
    quarterW = width >> 2;
    halfH = height >> 1;
    quarterH = height >> 2;

    /* Layout 0: one full-screen view. */
    D_800BE668[0][0][0] = halfW;
    D_800BE668[0][0][1] = halfH;
    D_800BE668[0][1][0] = halfW;
    D_800BE668[0][1][1] = halfH;
    D_800BE668[0][2][0] = halfW;
    D_800BE668[0][2][1] = halfH;
    D_800BE668[0][3][0] = halfW;
    D_800BE668[0][3][1] = halfH;
    D_800BE668[0][4][0] = halfW;
    D_800BE668[0][4][1] = halfH;

    /* Layout 1: top and bottom halves. */
    D_800BE668[1][0][0] = halfW;
    D_800BE668[1][0][1] = quarterH;
    D_800BE668[1][1][0] = halfW;
    D_800BE668[1][1][1] = quarterH * 3;
    D_800BE668[1][2][0] = halfW;
    D_800BE668[1][2][1] = halfH;
    D_800BE668[1][3][0] = 0;
    D_800BE668[1][3][1] = 0;
    D_800BE668[1][4][0] = 0;
    D_800BE668[1][4][1] = 0;

    /* Layout 2: two top quarters and a centred bottom quarter. */
    D_800BE668[2][0][0] = quarterW;
    D_800BE668[2][0][1] = quarterH;
    D_800BE668[2][1][0] = quarterW * 3;
    D_800BE668[2][1][1] = quarterH;
    D_800BE668[2][2][0] = halfW;
    D_800BE668[2][2][1] = quarterH * 3;
    D_800BE668[2][3][0] = halfW;
    D_800BE668[2][3][1] = halfH;
    D_800BE668[2][4][0] = 0;
    D_800BE668[2][4][1] = 0;

    /* Layout 3: four quarters. */
    D_800BE668[3][0][0] = quarterW;
    D_800BE668[3][0][1] = quarterH;
    D_800BE668[3][1][0] = quarterW * 3;
    D_800BE668[3][1][1] = quarterH;
    D_800BE668[3][2][0] = quarterW;
    D_800BE668[3][2][1] = quarterH * 3;
    D_800BE668[3][3][0] = quarterW * 3;
    D_800BE668[3][3][1] = quarterH * 3;
    D_800BE668[3][4][0] = halfW;
    D_800BE668[3][4][1] = halfH;
}
