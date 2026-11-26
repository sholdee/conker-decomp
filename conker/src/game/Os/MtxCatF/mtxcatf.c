#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// Game copy of ultralib gu/mtxcatf.c, built at -O3 like the rest of libultra gu/
// (see the Makefile). -O3 emits the functions in reverse source order, so guMtxCatF
// comes first here to put guMtxXFMF first in ROM.

void guMtxCatF(float m[4][4], float n[4][4], float r[4][4]) {
    int i;
    int j;
    int k;
    float temp[4][4];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            temp[i][j] = 0.0f;
            for (k = 0; k < 4; k++) {
                temp[i][j] += m[i][k] * n[k][j];
            }
        }
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            r[i][j] = temp[i][j];
        }
    }
}

void guMtxXFMF(float mf[4][4], float x, float y, float z, float *ox, float *oy, float *oz) {
    *ox = mf[0][0] * x + mf[1][0] * y + mf[2][0] * z + mf[3][0];
    *oy = mf[0][1] * x + mf[1][1] * y + mf[2][1] * z + mf[3][1];
    *oz = mf[0][2] * x + mf[1][2] * y + mf[2][2] * z + mf[3][2];
}
