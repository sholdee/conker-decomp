#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One of the 12 fading pieces of a burst effect. */
typedef struct {
    Mtx mtx[2];      /* 0x00 double-buffered by D_800BE9C0 */
    f32 rotX;        /* 0x80 */
    f32 rotY;        /* 0x84 */
    f32 rotZ;        /* 0x88 */
    f32 speed;       /* 0x8C */
    f32 offset;      /* 0x90 distance travelled along the local x axis */
    s16 elapsed;     /* 0x94 */
    s16 duration;    /* 0x96 */
    u8 rotMode;      /* 0x98 0: func_150A8050, 1: func_15187D6C */
    u8 pad99[7];
} BurstPiece; /* size 0xA0 */

/* Burst effect object: 12 pieces flying outwards from a point and fading. */
typedef struct {
    u8 pad0[0x10];
    Mtx mtx[2];          /* 0x10 */
    f32 x;               /* 0x90 */
    f32 y;               /* 0x94 */
    f32 z;               /* 0x98 */
    f32 scaleYZ;         /* 0x9C */
    f32 scaleX;          /* 0xA0 */
    BurstPiece *pieces;  /* 0xA4 */
    s16 timer;           /* 0xA8 */
    s16 duration;        /* 0xAA */
} BurstEffect; /* size 0xB0 */

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void *allocate_memory(s32, s32, s32, s32);
extern s32 func_10004074(void *);
extern void func_150A7DA0(f32 mtx[4][4], f32 x, f32 y, f32 z);
extern void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);
extern void guMtxF2L2(f32 mf[4][4], Mtx *m);
extern void func_151EFE00(f32 *);
extern Vtx D_8008D538[];
void func_15187D6C(f32 *arg0, f32 arg1, f32 arg2, f32 arg3);

/* Spawns a burst effect at (x, y, z) with 12 pieces in three rings of four. */
void func_151875E0(f32 x, f32 y, f32 z, s32 speed, s32 duration, s32 durationJitter, f32 scaleYZ, f32 scaleX) {
    BurstEffect *burst;
    s32 i;
    s32 a;

    burst = func_15167A68(6, 0, sizeof(BurstEffect), 1, 0xFF, 1);
    if (burst != NULL) {
        burst->pieces = allocate_memory(12 * sizeof(BurstPiece), 1, 2, 1);
        if (burst->pieces == NULL) {
            func_1516972C((struct102 *)burst);
            return;
        }
        burst->x = x;
        burst->y = y;
        burst->z = z;
        burst->timer = 0;
        burst->duration = duration;
        burst->scaleYZ = scaleYZ;
        burst->scaleX = scaleX;
        for (i = 0, a = 0; a != 1080; a += 90, i++) {
            if (a < 360) {
                burst->pieces[i].rotX = 7 - (func_150ADA20() & 0xFU);
                burst->pieces[i].rotY = 0.0f;
                burst->pieces[i].rotZ = (func_150ADA20() & 0x3FU) + (a - 30) * 1;
                burst->pieces[i].rotMode = 0;
            } else if (a < 720) {
                burst->pieces[i].rotX = 0.0f;
                burst->pieces[i].rotY = (func_150ADA20() & 0x3FU) + (a - 570) * 1;
                burst->pieces[i].rotZ = 7 - (func_150ADA20() & 0xFU);
                burst->pieces[i].rotMode = 0;
            } else {
                burst->pieces[i].rotX = (func_150ADA20() & 0x3FU) + (a - 30) * 1;
                burst->pieces[i].rotY = 97 - (func_150ADA20() & 0xFU);
                burst->pieces[i].rotZ = 0.0f;
                burst->pieces[i].rotMode = 1;
            }
            if (speed >= 4) {
                burst->pieces[i].speed = (func_150ADA20() % (u32)(speed >> 2)) + speed;
            } else {
                burst->pieces[i].speed = speed;
            }
            burst->pieces[i].offset = func_150ADA20() & 0xFU;
            burst->pieces[i].elapsed = 0;
            burst->pieces[i].duration = duration - (func_150ADA20() & durationJitter);
        }
    }
}

void func_15187978(BurstEffect *arg0) {
    s32 i;

    if (arg0->timer < 0) {
        func_10004074(arg0->pieces);
        func_1516972C((struct102 *)arg0);
        return;
    }
    arg0->timer += D_800BE9E4;
    if (arg0->duration < arg0->timer) {
        arg0->timer = -10;
    } else if (arg0->timer >= 0) {
        for (i = 0; i < 12; i++) {
            if (arg0->pieces[i].elapsed < arg0->pieces[i].duration) {
                arg0->pieces[i].elapsed += D_800BE9E4;
                if (arg0->pieces[i].duration < arg0->pieces[i].elapsed) {
                    arg0->pieces[i].elapsed = arg0->pieces[i].duration;
                }
                arg0->pieces[i].offset += arg0->pieces[i].speed - arg0->pieces[i].speed / 3.0f * arg0->pieces[i].elapsed / arg0->pieces[i].duration;
            }
        }
    }
}

Gfx *func_15187A98(Gfx *gdl, BurstEffect *arg1, s32 arg2) {
    s32 i;
    s32 alpha;
    f32 spB8[4][4];
    f32 sp78[4][4];
    Gfx *g2;
    Gfx *g3;

    /* dead stores: give g2/g3 their uopt bit positions before the alpha expression (register coloring order) */
    g2 = NULL; g3 = NULL;
    if (arg1->timer >= 0) {
    func_150A7DA0(spB8, arg1->x, arg1->y, arg1->z);
    guMtxF2L2(spB8, &arg1->mtx[D_800BE9C0]);
    for (i = 0; i < 12; i++) {
        gSPMatrix(gdl++, &arg1->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        func_150A7DA0(spB8, arg1->pieces[i].offset, 0.0f, 0.0f);
        spB8[0][0] *= arg1->scaleX;
        spB8[1][1] *= arg1->scaleYZ;
        spB8[2][2] *= arg1->scaleYZ;
        if (arg1->pieces[i].rotMode == 0) {
            func_150A8050(sp78, arg1->pieces[i].rotX, arg1->pieces[i].rotY, arg1->pieces[i].rotZ);
        } else {
            func_15187D6C(sp78, arg1->pieces[i].rotX, arg1->pieces[i].rotY, arg1->pieces[i].rotZ);
        }
        func_150A7A48(spB8, sp78, spB8);
        guMtxF2L(spB8, &arg1->pieces[i].mtx[D_800BE9C0]);
        alpha = (arg1->pieces[i].duration - arg1->pieces[i].elapsed) * 0xFF / arg1->pieces[i].duration;
        { s32 pad; g2 = gdl++; g2->words.w0 = (_SHIFTL(G_SETPRIMCOLOR, 24, 8) | _SHIFTL(1, 8, 8) | _SHIFTL(0, 0, 8)); g2->words.w1 = (_SHIFTL(0xFF, 24, 8) | _SHIFTL(0xFF, 16, 8) | _SHIFTL(0xFF, 8, 8) | _SHIFTL(alpha, 0, 8)); }
        { s32 pad; g3 = gdl++; g3->words.w0 = (_SHIFTL((G_MTX),24,8)|_SHIFTL(((sizeof(Mtx))-1)/8,19,5)|_SHIFTL((0)/8,8,8)|_SHIFTL(((G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW)^G_MTX_PUSH),0,8)); g3->words.w1 = (unsigned int)(&arg1->pieces[i].mtx[D_800BE9C0]); }
        gSPVertex(gdl++, D_8008D538, 4, 0);
        gSP1Triangle(gdl++, 0, 1, 2, 0);
        gSP1Triangle(gdl++, 0, 1, 3, 0);
        gSP1Triangle(gdl++, 0, 2, 3, 0);
    }
    }
    return gdl;
}

void func_15187D6C(f32 *arg0, f32 arg1, f32 arg2, f32 arg3) {
    f32 sx;
    f32 cx;
    f32 sz;
    f32 sy;
    f32 cy;
    f32 cz;
    f32 t;

    arg1 *= 0.01745329238f;
    arg2 *= 0.01745329238f;
    arg3 *= 0.01745329238f;
    sx = sinf(arg1);
    cx = cosf(arg1);
    sy = sinf(arg2);
    cy = cosf(arg2);
    sz = sinf(arg3);
    cz = cosf(arg3);
    func_151EFE00(arg0);
    t = sx * sy;
    arg0[6] = sx;
    arg0[0] = (cy * cz) - (t * sz);
    arg0[1] = (cy * sz) + (t * cz);
    arg0[2] = -sy * cx;
    t = sx * cy;
    arg0[4] = -cx * sz;
    arg0[5] = cx * cz;
    arg0[8] = (sy * cz) + (t * sz);
    arg0[9] = (sy * sz) - (t * cz);
    arg0[10] = cx * cy;
}

