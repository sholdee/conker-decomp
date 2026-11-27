#include <ultra64.h>
#include "conker/anim.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/prop.h"
#include "conker/sprite.h"
#include "conker/texture.h"
#include "conker/view.h"

s16 gDebrisPrimAlpha; /* primitive alpha last set in the display list; -1 = unknown */

/* The debris quad: a hexagon of six vertices fanned into four triangles. */
static Vtx gSpinDebrisVerts[6] = {
    { { { 7, 4, 0 }, 0, { 0x2412, 0x209F }, { 0xFE, 0xFE, 0xFE, 0xFF } } },
    { { { -7, 4, 0 }, 0, { 0x2008, 0x208E }, { 0xFE, 0xFE, 0xFE, 0xFF } } },
    { { { 0, 1, 3 }, 0, { 0x2210, 0x1FAB }, { 0xFE, 0xFE, 0xFE, 0xFF } } },
    { { { 7, 14, 0 }, 0, { 0x23F2, 0x2350 }, { 0xFE, 0xFE, 0xFE, 0xFF } } },
    { { { -7, 14, 0 }, 0, { 0x2015, 0x2340 }, { 0xFE, 0xFE, 0xFE, 0xFF } } },
    { { { 0, 21, 4 }, 0, { 0x21FA, 0x25DE }, { 0xFE, 0xFE, 0xFE, 0xFF } } },
};

/* The particle drawn by particle_drawSpinDebris. */
static Gfx gSpinDebrisDl[] = {
    gsSPVertex(gSpinDebrisVerts, 6, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 3, 1, 0),
    gsSP1Triangle(4, 1, 3, 0),
    gsSP1Triangle(5, 4, 3, 0),
    gsSPEndDisplayList(),
};

static Gfx gSpinDebrisCommonSetupDl[9];

/* Render setups of the object types (drawBeginDL in gObjectTypes): types 8 and 9 draw in the z-buffered
 * translucent mode, types 12 and 89 without the z-buffer. */
Gfx gSpinDebrisZbSetupDl[] = {
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsSPDisplayList(gSpinDebrisCommonSetupDl),
    gsSPEndDisplayList(),
};
Gfx gSpinDebrisNoZSetupDl[] = {
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsSPDisplayList(gSpinDebrisCommonSetupDl),
    gsSPEndDisplayList(),
};
static Gfx gSpinDebrisCommonSetupDl[9] = {
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0),
    gsDPSetTextureLOD(G_TL_TILE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPClearGeometryMode(G_CULL_BOTH | G_FOG | G_LOD),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0),
    gsSPEndDisplayList(),
};

/* Spawns one particle of object type `type` at (x, y, z) with random horizontal speed and spin. */
void particle_spawnSpinDebris(u8 texture, s16 x, s16 y, s16 z, s8 velY, u16 timer, u16 unkAC, u8 speedRange,
                   u8 spinRange, u8 dragRange, s32 type) {
    SpinDebrisParticle *particle;
    s32 rnd;

    particle = effect_newObject(type, 0, sizeof(SpinDebrisParticle), 1, 0xFF, 1);
    if (particle != NULL) {
        particle->x = x;
        particle->y = y;
        particle->z = z;
        particle->velY = velY;
        particle->velX = (math_random() % speedRange) - (speedRange >> 1);
        particle->velZ = (math_random() % speedRange) - (speedRange >> 1);
        if (unkAC == 5) {
            particle->x += particle->velX * 4;
            particle->y -= particle->velY * 4;
            particle->z += particle->velZ * 4;
        }
        particle->rotX = 0;
        particle->rotY = 0;
        particle->rotZ = 0;
        particle->spinX = (math_random() % spinRange) - (spinRange >> 1);
        particle->spinY = (math_random() % spinRange) - (spinRange >> 1);
        particle->spinZ = (math_random() % spinRange) - (spinRange >> 1);
        rnd = math_random() & 0x7F;
        particle->scale = (rnd + 140) * 0.01f;
        particle->timer = timer;
        particle->texture = texture;
        particle->unkAC = unkAC;
        particle->dragRange = dragRange;
        particle->viewFlags = 0;
        particle->alpha = 0xFF;
    }
}

/* Update callback for particles from particle_spawnSpinDebris: move, spin, slow down, then fade out once the timer runs
 * out. Frees the particle when it is invisible or has left every view it was drawn in. */
void particle_updateSpinDebris(SpinDebrisParticle *particle) {
    s32 timer;

    particle->x += particle->velX;
    particle->y -= particle->velY;
    particle->z += particle->velZ;
    particle->rotX += particle->spinX;
    particle->rotY += particle->spinY;
    particle->rotZ += particle->spinZ;
    if (particle->unkAC == 5) {
        particle->velY++;
    }
    if (particle->timer < particle->unkAC) {
        s32 sign;
        u32 drag;

        drag = math_random() % particle->dragRange;
        sign = (particle->velX < 0) ? -1 : 1;
        particle->velX += -sign * drag;
        drag = math_random() % particle->dragRange;
        sign = (particle->velZ < 0) ? -1 : 1;
        particle->velZ += -sign * drag;
    }
    timer = particle->timer;
    timer -= gFrameTicks;
    if (timer < 0) {
        s32 alpha;
        timer = 0;
        alpha = particle->alpha;
        alpha -= gFrameTicks * 4;
        if (alpha < 0) {
            alpha = 0;
        }
        particle->alpha = alpha;
    }
    particle->timer = timer;
    if (particle->alpha == 0 ||
        (particle->viewFlags && (particle->viewFlags >> 4) == (particle->viewFlags & 0xF))) {
        effect_destroyObject(&particle->header);
    }
}

/* Identity hook used by particle_beginSpinDebrisDraw for object types 0xC and 0x59. */
Gfx *particle_emptySpinDebrisDrawHook(Gfx *gfx) {
    return gfx;
}

#define D_800D35E4 ((&gCullVtxFlags)[-1])

/* Draw callback: culls the particle against view `view` (types 0xC and 0x59 are never culled), then emits its
 * texture, matrix, primitive alpha and render mode, and the shared particle display list gSpinDebrisDl. */
Gfx *particle_drawSpinDebris(Gfx *gfx, SpinDebrisParticle *particle, s16 view) {
    if (particle->header.type != 0xC && particle->header.type != 0x59 &&
        prop_isOutsideFrustum(gViewMatrices[view], particle->x, particle->y, particle->z, gDefaultNear, gDefaultFar,
                      gFrustumSlopeX, D_800D35E4, NULL, NULL) != 0) {
        if (particle->header.type == 8 && (particle->viewFlags & (1 << view))) {
            particle->viewFlags |= 0x10 << view;
        }
        return gfx;
    }
    if (particle->header.type != 9) {
        particle->viewFlags |= 1 << view;
    }
    gfx = particle_loadDebrisTexture(gfx, particle->texture);
    math_mtxRotScaleTrans(&particle->mtx[gDrawBufferIndex], particle->rotX, particle->rotY, particle->rotZ,
                  particle->scale, particle->scale, particle->scale, particle->x, particle->y, particle->z);
    gSPMatrix(gfx++, &particle->mtx[gDrawBufferIndex], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (gDebrisPrimAlpha != particle->alpha) {
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, particle->alpha);
        // matching: gDebrisPrimAlpha spelled via its neighbour so IDO does not CSE its address with the read above
        ((s16 *)gVtxMorphCache)[-8] = particle->alpha;
    }
    {
        s32 pad; // matching: unused stack slot (needSync lands at sp+0x43)
        s32 modeL;
        u8 needSync;
        needSync = 0;
        modeL = ((particle->header.type == 8) || (particle->header.type == 9)) ? 0x5049D8 : 0x504240;
        gfx = math_gfxSetOtherMode(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | gSpriteTlutMode |
                                     G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            modeL, &needSync);
    }
    gSPDisplayList(gfx++, gSpinDebrisDl);
    return gfx;
}

/* Spawns one ground debris chunk (object type 9) of size `size` at (x, z), resting on the ground below y. */
void particle_spawnGroundChunk(f32 size, u8 texture, s16 x, s16 y, s16 z) {
    SpinDebrisParticle *particle;
    s32 range;
    f32 groundY;
    s32 rnd;
    s32 half;

    range = size;
    if (range != 0) {
        particle = effect_newObject(9, 0, sizeof(SpinDebrisParticle), 1, 0xFF, 1);
        if (particle != NULL) {
            collision_findFloor(0, 0, &groundY, 0, 0, 0, x, y, z, y, 0, 0);
            particle->y = particle->groundY = groundY + 3.0f;
            particle->x = x;
            particle->z = z;
            particle->rotX = 90;
            particle->rotY = math_random() % 360;
            particle->rotZ = 0;
            particle->velX = 0;
            particle->velZ = 0;
            particle->texture = texture;
            rnd = (math_random() % 80) + 100;
            particle->scale = rnd * 0.01f;
            particle->velY = size * 0.4f;
            rnd = math_random() % range;
            half = size * 0.5f;
            particle->spinX = rnd - half;
            particle->spinY = (math_random() % range) - half;
            particle->spinZ = (math_random() % range) - half;
            particle->timer = 0;
            particle->alpha = 0xFF;
        }
    }
}

/* Update callback for particle_spawnGroundChunk chunks: fly up, fall back to the ground, then lie flat and fade out over
 * 60 frames before being freed. */
void particle_updateGroundChunk(SpinDebrisParticle *particle) {
    s32 timer;
    s32 velY;
    s32 groundY;
    s32 height;

    timer = particle->timer;
    if (timer == 0) {
        particle->x += particle->velX;
        velY = particle->velY;
        particle->y += velY;
        particle->z += particle->velZ;
        if (velY > 0) {
            particle->velY = velY - 1;
            if (particle->velY <= 0) {
                particle->velY = -3;
            }
        }
        groundY = particle->groundY;
        height = particle->y - groundY;
        if (height <= 0) {
            particle->rotX = 90;
            particle->rotZ = 0;
            particle->timer = 60;
            particle->y = groundY;
        } else {
            particle->rotX += particle->spinX;
            particle->rotY += particle->spinY;
            particle->rotZ += particle->spinZ;
        }
    } else {
        timer -= gFrameTicks;
        if (timer > 0) {
            particle->timer = timer;
            particle->alpha = (timer << 8) / 60;
        } else {
            effect_destroyObject(&particle->header);
        }
    }
}

/* Begin-draw hook: forgets the cached primitive alpha so the first particle drawn sets it again. */
Gfx *particle_beginSpinDebrisDraw(Gfx *gfx, s32 type, s32 list, s32 view) {
    gDebrisPrimAlpha = -1;
    if ((type == 0xC) || (type == 0x59)) {
        gfx = particle_emptySpinDebrisDrawHook(gfx);
    }
    return gfx;
}


/* Loads particle texture `texture` unless it is already the current one. */
Gfx *particle_loadDebrisTexture(Gfx *gfx, s32 texture) {
    SpriteParams texState; // filled in by sprite_loadDescFrame; unused here

    if (texture != gCachedTextureId) {
        // matching: the unsigned shift changes IDO's code
        gfx = sprite_loadDescFrame(gfx, &gDebrisTexture, (u32)texture << 8, &texState, 0, 0, 0, 2, 3);
        gCachedTextureId = texture;
    }
    return gfx;
}

/* Spawns `count` ground debris chunks scattered around (x, y, z) when `size` is at least 12. */
void particle_spawnGroundChunks(f32 size, s32 x, s32 y, s32 z, s32 count) {
    s32 offsetX;
    s32 offsetZ;
    s32 i;
    s32 range;
    f32 chunkSize;

    if (!(size < 12.0f)) {
        range = size * 8.0f;
        size *= 0.5f;
        for (i = 0; i < count; i++) {
            offsetX = math_random() & 0x3F;
            offsetZ = math_random() & 0x3F;
            offsetX -= 32;
            offsetZ -= 32;
            chunkSize = ((math_random() % range) >> 4) + size;
            particle_spawnGroundChunk(chunkSize, math_random() & 1, x + offsetX, y, z + offsetZ);
        }
    }
}

/* Draw callback for another object type in the same table; forwards to view_emitRspSetup. */
Gfx *particle_endSpinDebrisDraw(Gfx *gfx, s32 type, s16 player) {
    return view_emitRspSetup(gfx, player);
}
