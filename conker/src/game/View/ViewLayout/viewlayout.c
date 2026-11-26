#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/*
 * Sets gIsMultiplayer for the listed maps, which take their player count from func_151E5FAC();
 * every other map uses `count`. D_800BE9E8 gets that count minus one.
 */
void view_setPlayerCount(s16 count, s32 map) {
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
void view_initLayouts(s32 width, s32 height) {
    s32 i;
    s32 halfW, halfH, quarterH, quarterW;

    for (i = 0; i < 5; i++) {
        gLayoutViewWidths[i] = width;
        gLayoutViewHeights[i] = height;
        if (i < 4) {
            if (i >= 2) {
                gLayoutViewWidths[i] >>= 1;
            }
            if (i >= 1) {
                gLayoutViewHeights[i] >>= 1;
            }
        }
    }
    halfW = width >> 1;
    quarterW = width >> 2;
    halfH = height >> 1;
    quarterH = height >> 2;

    /* Layout 0: one full-screen view. */
    gLayoutViewCentres[0][0][0] = halfW;
    gLayoutViewCentres[0][0][1] = halfH;
    gLayoutViewCentres[0][1][0] = halfW;
    gLayoutViewCentres[0][1][1] = halfH;
    gLayoutViewCentres[0][2][0] = halfW;
    gLayoutViewCentres[0][2][1] = halfH;
    gLayoutViewCentres[0][3][0] = halfW;
    gLayoutViewCentres[0][3][1] = halfH;
    gLayoutViewCentres[0][4][0] = halfW;
    gLayoutViewCentres[0][4][1] = halfH;

    /* Layout 1: top and bottom halves. */
    gLayoutViewCentres[1][0][0] = halfW;
    gLayoutViewCentres[1][0][1] = quarterH;
    gLayoutViewCentres[1][1][0] = halfW;
    gLayoutViewCentres[1][1][1] = quarterH * 3;
    gLayoutViewCentres[1][2][0] = halfW;
    gLayoutViewCentres[1][2][1] = halfH;
    gLayoutViewCentres[1][3][0] = 0;
    gLayoutViewCentres[1][3][1] = 0;
    gLayoutViewCentres[1][4][0] = 0;
    gLayoutViewCentres[1][4][1] = 0;

    /* Layout 2: two top quarters and a centred bottom quarter. */
    gLayoutViewCentres[2][0][0] = quarterW;
    gLayoutViewCentres[2][0][1] = quarterH;
    gLayoutViewCentres[2][1][0] = quarterW * 3;
    gLayoutViewCentres[2][1][1] = quarterH;
    gLayoutViewCentres[2][2][0] = halfW;
    gLayoutViewCentres[2][2][1] = quarterH * 3;
    gLayoutViewCentres[2][3][0] = halfW;
    gLayoutViewCentres[2][3][1] = halfH;
    gLayoutViewCentres[2][4][0] = 0;
    gLayoutViewCentres[2][4][1] = 0;

    /* Layout 3: four quarters. */
    gLayoutViewCentres[3][0][0] = quarterW;
    gLayoutViewCentres[3][0][1] = quarterH;
    gLayoutViewCentres[3][1][0] = quarterW * 3;
    gLayoutViewCentres[3][1][1] = quarterH;
    gLayoutViewCentres[3][2][0] = quarterW;
    gLayoutViewCentres[3][2][1] = quarterH * 3;
    gLayoutViewCentres[3][3][0] = quarterW * 3;
    gLayoutViewCentres[3][3][1] = quarterH * 3;
    gLayoutViewCentres[3][4][0] = halfW;
    gLayoutViewCentres[3][4][1] = halfH;
}
