#include <ultra64.h>
#include <math.h>
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/heap.h"

#define BURST_NUM_PIECES 12

/* One of the BURST_NUM_PIECES fading pieces of a burst effect. */
typedef struct {
    Mtx mtx[2];      /* 0x00 double-buffered by gDrawBufferIndex */
    f32 rotX;        /* 0x80 */
    f32 rotY;        /* 0x84 */
    f32 rotZ;        /* 0x88 */
    f32 speed;       /* 0x8C */
    f32 offset;      /* 0x90 distance travelled along the local x axis */
    s16 elapsed;     /* 0x94 */
    s16 duration;    /* 0x96 */
    u8 rotMode;      /* 0x98 0: math_mtxRotateRPYF, 1: effect_buildEulerRotation */
    u8 pad99[7];
} FadeBurstPiece; /* size 0xA0 */

/* Burst effect object: 12 pieces flying outwards from a point and fading. */
struct FadeBurstEffect {
    EffectHeader header;
    Mtx mtx[2];          /* 0x10 */
    f32 x;               /* 0x90 */
    f32 y;               /* 0x94 */
    f32 z;               /* 0x98 */
    f32 scaleYZ;         /* 0x9C */
    f32 scaleX;          /* 0xA0 */
    FadeBurstPiece *pieces;  /* 0xA4 */
    s16 timer;           /* 0xA8 */
    s16 duration;        /* 0xAA */
}; /* size 0xB0 */

/* The burst effect type's render setup display list (object type table gObjectTypes). */
Gfx gFadeBurstRenderSetupDl[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineLERP(PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0, PRIMITIVE, 0, SHADE, 0),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BOTH),
    gsDPSetOtherMode(0x082CAF, 0x00504A50),
    gsSPEndDisplayList(),
};

/* Its render teardown display list. */
Gfx gFadeBurstRenderTeardownDl[] = {
    gsSPMatrix(&gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

/* One burst piece: a thin pyramid along the x axis, its tip faded. */
static Vtx gFadeBurstPieceVtx[] = {
    { { { -625, 0, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0x10 } } },
    { { { 625, 31, 0 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 625, -31, 31 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
    { { { 625, -31, -31 }, 0, { 0, 0 }, { 0xFF, 0xFF, 0xFF, 0xFF } } },
};

/* Spawns a burst effect at (x, y, z) with 12 pieces in three rings of four. */
void effect_spawnFadeBurst(f32 x, f32 y, f32 z, s32 speed, s32 duration, s32 durationJitter, f32 scaleYZ, f32 scaleX) {
    FadeBurstEffect *burst;
    s32 i;

    burst = effect_newObject(6, 0, sizeof(FadeBurstEffect), 1, 0xFF, 1);
    if (burst != NULL) {
        burst->pieces = allocate_memory(BURST_NUM_PIECES * sizeof(FadeBurstPiece), 1, 2, 1);
        if (burst->pieces == NULL) {
            effect_destroyObject(&burst->header);
            return;
        }
        burst->x = x;
        burst->y = y;
        burst->z = z;
        burst->timer = 0;
        burst->duration = duration;
        burst->scaleYZ = scaleYZ;
        burst->scaleX = scaleX;
        for (i = 0; i < BURST_NUM_PIECES; i++) {
            if (i < 4) {
                burst->pieces[i].rotX = 7U - (math_random() & 15U);
                burst->pieces[i].rotY = 0.0f;
                burst->pieces[i].rotZ = (math_random() & 63U) + (i * 90 - 30);
                burst->pieces[i].rotMode = 0;
            } else if (i < 8) {
                burst->pieces[i].rotX = 0.0f;
                burst->pieces[i].rotY = (math_random() & 63U) + (i * 90 - 570);
                burst->pieces[i].rotZ = 7U - (math_random() & 15U);
                burst->pieces[i].rotMode = 0;
            } else {
                burst->pieces[i].rotX = (math_random() & 63U) + (i * 90 - 30);
                burst->pieces[i].rotY = 97U - (math_random() & 15U);
                burst->pieces[i].rotZ = 0.0f;
                burst->pieces[i].rotMode = 1;
            }
            if (speed >= 4) {
                burst->pieces[i].speed = (math_random() % (speed >> 2)) + speed;
            } else {
                burst->pieces[i].speed = speed;
            }
            burst->pieces[i].offset = math_random() & 15U;
            burst->pieces[i].elapsed = 0;
            burst->pieces[i].duration = duration - (math_random() & durationJitter);
        }
    }
}

/* Per-frame update: advances the burst timer and slides each piece outwards, slowing as it ages. */
void effect_updateFadeBurst(FadeBurstEffect *burst) {
    s32 i;

    if (burst->timer < 0) {
        heap_free(burst->pieces);
        effect_destroyObject(&burst->header);
        return;
    }
    burst->timer += gFrameTicks;
    if (burst->duration < burst->timer) {
        burst->timer = -10;
    } else if (burst->timer >= 0) {
        for (i = 0; i < BURST_NUM_PIECES; i++) {
            if (burst->pieces[i].elapsed < burst->pieces[i].duration) {
                burst->pieces[i].elapsed += gFrameTicks;
                if (burst->pieces[i].duration < burst->pieces[i].elapsed) {
                    burst->pieces[i].elapsed = burst->pieces[i].duration;
                }
                burst->pieces[i].offset += burst->pieces[i].speed
                    - burst->pieces[i].speed / 3.0f * burst->pieces[i].elapsed / burst->pieces[i].duration;
            }
        }
    }
}

/* Draws the 12 burst pieces, each fading out over its own duration. */
Gfx *effect_drawFadeBurst(Gfx *gdl, FadeBurstEffect *burst, s32 arg2) {
    s32 i;
    s32 alpha;
    MtxF mtx;
    MtxF rot;
    Gfx *primCmd;
    Gfx *mtxCmd;

    // matching: dead stores give these pointers an earlier uopt bit position than the alpha expression,
    // which steers IDO's register coloring order
    primCmd = NULL;
    mtxCmd = NULL;
    if (burst->timer >= 0) {
        math_mtxTranslateF(mtx, burst->x, burst->y, burst->z);
        guMtxF2L2(mtx, &burst->mtx[gDrawBufferIndex]);
        for (i = 0; i < BURST_NUM_PIECES; i++) {
            gSPMatrix(gdl++, &burst->mtx[gDrawBufferIndex], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            math_mtxTranslateF(mtx, burst->pieces[i].offset, 0.0f, 0.0f);
            mtx[0][0] *= burst->scaleX;
            mtx[1][1] *= burst->scaleYZ;
            mtx[2][2] *= burst->scaleYZ;
            if (burst->pieces[i].rotMode == 0) {
                math_mtxRotateRPYF(rot, burst->pieces[i].rotX, burst->pieces[i].rotY, burst->pieces[i].rotZ);
            } else {
                effect_buildEulerRotation(rot, burst->pieces[i].rotX, burst->pieces[i].rotY, burst->pieces[i].rotZ);
            }
            math_mtxMulF(mtx, rot, mtx);
            guMtxF2L(mtx, &burst->pieces[i].mtx[gDrawBufferIndex]);
            alpha = (burst->pieces[i].duration - burst->pieces[i].elapsed) * 255 / burst->pieces[i].duration;
            // matching: gDPSetPrimColor/gSPMatrix written out via named pointers (see the dead stores above);
            // each block's pad takes the stack slot of the macro's _g, and one line per command lets as1
            // schedule the w0/w1 stores freely (-g3 line numbers)
            {
                s32 pad;
                primCmd = gdl++; primCmd->words.w0 = _SHIFTL(G_SETPRIMCOLOR, 24, 8) | _SHIFTL(1, 8, 8) | _SHIFTL(0, 0, 8); primCmd->words.w1 = _SHIFTL(255, 24, 8) | _SHIFTL(255, 16, 8) | _SHIFTL(255, 8, 8) | _SHIFTL(alpha, 0, 8);
            }
            {
                s32 pad;
                mtxCmd = gdl++; mtxCmd->words.w0 = _SHIFTL(G_MTX, 24, 8) | _SHIFTL((sizeof(Mtx) - 1) / 8, 19, 5) | _SHIFTL((G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW) ^ G_MTX_PUSH, 0, 8); mtxCmd->words.w1 = (u32)&burst->pieces[i].mtx[gDrawBufferIndex];
            }
            gSPVertex(gdl++, gFadeBurstPieceVtx, 4, 0);
            gSP1Triangle(gdl++, 0, 1, 2, 0);
            gSP1Triangle(gdl++, 0, 1, 3, 0);
            gSP1Triangle(gdl++, 0, 2, 3, 0);
        }
    }
    return gdl;
}

/* Builds a rotation matrix from angles in degrees (the alternative axis order to math_mtxRotateRPYF). */
void effect_buildEulerRotation(MtxF mtx, f32 rx, f32 ry, f32 rz) {
    f32 sx;
    f32 cx;
    f32 sz;
    f32 sy;
    f32 cy;
    f32 cz;
    f32 t;

    rx *= 0.01745329238f; /* pi / 180 */
    ry *= 0.01745329238f;
    rz *= 0.01745329238f;
    sx = sinf(rx);
    cx = cosf(rx);
    sy = sinf(ry);
    cy = cosf(ry);
    sz = sinf(rz);
    cz = cosf(rz);
    func_151EFE00(mtx);
    t = sx * sy;
    mtx[1][2] = sx;
    mtx[0][0] = (cy * cz) - (t * sz);
    mtx[0][1] = (cy * sz) + (t * cz);
    mtx[0][2] = -sy * cx;
    t = sx * cy;
    mtx[1][0] = -cx * sz;
    mtx[1][1] = cx * cz;
    mtx[2][0] = (sy * cz) + (t * sz);
    mtx[2][1] = (sy * sz) - (t * cz);
    mtx[2][2] = cx * cy;
}
