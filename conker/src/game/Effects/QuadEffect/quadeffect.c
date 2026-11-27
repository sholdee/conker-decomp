#include <ultra64.h>
#include <math.h>
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/sprite.h"
#include "conker/texture.h"

/* Render setup display list of the quad effects; the effect type's entry in the object type table
 * D_8008B4D4 points to it. */
Gfx gQuadEffectRenderSetupDl[] = {
    gsSPClearGeometryMode(G_CULL_BOTH),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM),
    gsDPSetAlphaCompare(G_AC_THRESHOLD),
    gsDPSetBlendColor(0, 0, 0, 1),
    gsSPEndDisplayList(),
};

/* not referenced */
const u8 D_800A6C80[20] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x65, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
};

// matching: a 0.01745329238f literal swaps the spill slots of effect and angle; the factor is a const
// struct in .rodata
static const struct {
    f32 degToRad;
} D_800A6C94 = { 0.01745329238f };

static s16 gQuadEffectTexFrame; // frame offset of the next texture frame (effect_beginQuadEffectDraw)


/* Spawns a quad effect at (x, y, z); direction is in degrees, or -1 for a quad lying flat at height y. */
void effect_spawnQuadEffect(s32 direction, s32 x, s32 y, s32 z, s32 growthRate, s32 life, s32 arg6, u8 owner, s32 list) {
    s32 i;
    QuadEffect *effect;
    f32 angle;

    effect = effect_newObject(4, list, sizeof(QuadEffect), 1, owner, 1);
    if (effect != NULL) {
        if (direction == -1) {
            for (i = 0; i < 2; i++) {
                effect->quads[i][0].v.ob[1] = y;
                effect->quads[i][1].v.ob[1] = y;
                effect->quads[i][2].v.ob[1] = y;
                effect->quads[i][3].v.ob[1] = y;
                effect->quads[i][0].v.flag = 0;
                effect->quads[i][1].v.flag = 0;
                effect->quads[i][2].v.flag = 0;
                effect->quads[i][3].v.flag = 0;
            }
            effect->mode = 0;
        } else {
            angle = direction * D_800A6C94.degToRad;
            effect->dirZ = sinf(angle) * 127.0f;
            effect->dirX = -cosf(angle) * 127.0f;
            effect->mode = 1;
        }

        for (i = 0; i < 2; i++) {
            effect->quads[i][0].v.tc[0] = 0x2000;
            effect->quads[i][0].v.tc[1] = 0x2000;
            effect->quads[i][1].v.tc[0] = 0x2800;
            effect->quads[i][1].v.tc[1] = 0x2000;
            effect->quads[i][2].v.tc[0] = 0x2800;
            effect->quads[i][2].v.tc[1] = 0x2800;
            effect->quads[i][3].v.tc[0] = 0x2000;
            effect->quads[i][3].v.tc[1] = 0x2800;
        }

        effect->y = y;
        effect->size = 1;
        effect->life = life;
        effect->lifeMax = life;
        effect->x = x;
        effect->z = z;
        effect->growthRate = growthRate;
        effect->unk94 = arg6;
        sprite_setDisturbPoint(x, y, z);
    }
}

/* Spawns a mode-2 quad effect at (x, y, z), rotated by angleA and angleB. */
void effect_spawnRotatedQuadEffect(s32 angleA, s32 angleB, s32 x, s32 y, s32 z, s32 growthRate, s32 life, s32 arg7, u8 owner,
                   s32 list) {
    s32 i;
    QuadEffect *effect;

    effect = effect_newObject(4, list, sizeof(QuadEffect), 1, owner, 1);
    if (effect != NULL) {
        effect->mode = 2;
        for (i = 0; i < 2; i++) {
            effect->quads[i][0].v.tc[0] = 0x2000;
            effect->quads[i][0].v.tc[1] = 0x2000;
            effect->quads[i][1].v.tc[0] = 0x2800;
            effect->quads[i][1].v.tc[1] = 0x2000;
            effect->quads[i][2].v.tc[0] = 0x2800;
            effect->quads[i][2].v.tc[1] = 0x2800;
            effect->quads[i][3].v.tc[0] = 0x2000;
            effect->quads[i][3].v.tc[1] = 0x2800;
        }

        effect->size = 1;
        effect->life = life;
        effect->lifeMax = life;
        effect->x = x;
        effect->y = y;
        effect->z = z;
        effect->growthRate = growthRate;
        effect->unk94 = arg7;
        effect->angleA = angleA;
        effect->angleB = angleB;
        sprite_setDisturbPoint(x, y, z);
    }
}

/* Per-frame update: grows the quad and frees the effect when its life runs out. */
void effect_updateQuadEffect(QuadEffect *effect) {
    s32 life;

    life = effect->life;
    effect->size = effect->size + effect->growthRate * gFrameTicks;
    life -= gFrameTicks;
    if (life <= 0) {
        effect_destroyObject(&effect->header);
        return;
    }
    effect->life = life;
}


/* Draws the effect: rebuilds this frame's quad vertices and fades alpha out over the effect's life. */
Gfx *effect_drawQuadEffect(Gfx *gdl, QuadEffect *effect, s32 view) {
    s32 mode;
    s32 x;
    s32 y;
    s32 z;
    s32 size;
    s32 alpha;
    s32 fadeStart;
    s32 half;
    s32 dx;
    s32 dz;
    s32 i;

    if (effect->life != 0) {
        x = effect->x;
        y = effect->y;
        z = effect->z;
        size = effect->size;
        half = effect->lifeMax >> 1;
        fadeStart = (effect->lifeMax * 3) / 4;
        if (effect->life < half) {
            effect->growthRate = (effect->growthRate * 14) / 15;
            if (effect->growthRate <= 0) {
                effect->growthRate = 1;
            }
        }
        if (effect->life < fadeStart) {
            alpha = (effect->life * 255) / fadeStart;
        } else {
            alpha = 255;
        }
        mode = effect->mode;
        if (mode == 0) {
            effect->quads[gDrawBufferIndex][0].v.ob[0] = x - size;
            effect->quads[gDrawBufferIndex][0].v.ob[2] = z - size;
            effect->quads[gDrawBufferIndex][1].v.ob[0] = x + size;
            effect->quads[gDrawBufferIndex][1].v.ob[2] = z - size;
            effect->quads[gDrawBufferIndex][2].v.ob[0] = x + size;
            effect->quads[gDrawBufferIndex][2].v.ob[2] = z + size;
            effect->quads[gDrawBufferIndex][3].v.ob[0] = x - size;
            effect->quads[gDrawBufferIndex][3].v.ob[2] = z + size;
        } else if (mode == 1) {
            dx = (effect->dirX * size) / 127;
            dz = (effect->dirZ * size) / 127;
            effect->quads[gDrawBufferIndex][0].v.ob[0] = x - dx;
            effect->quads[gDrawBufferIndex][0].v.ob[1] = y + size;
            effect->quads[gDrawBufferIndex][0].v.ob[2] = z - dz;
            effect->quads[gDrawBufferIndex][1].v.ob[0] = x + dx;
            effect->quads[gDrawBufferIndex][1].v.ob[1] = y + size;
            effect->quads[gDrawBufferIndex][1].v.ob[2] = z + dz;
            effect->quads[gDrawBufferIndex][2].v.ob[0] = x + dx;
            effect->quads[gDrawBufferIndex][2].v.ob[1] = y - size;
            effect->quads[gDrawBufferIndex][2].v.ob[2] = z + dz;
            effect->quads[gDrawBufferIndex][3].v.ob[0] = x - dx;
            effect->quads[gDrawBufferIndex][3].v.ob[1] = y - size;
            effect->quads[gDrawBufferIndex][3].v.ob[2] = z - dz;
        } else {
            f32 corners[4][3];
            f32 sinA;
            f32 cosA;
            f32 sinB;
            f32 cosB;
            f32 cx;
            f32 cy;
            f32 cz;

            corners[0][0] = -size;
            corners[0][1] = size;
            corners[0][2] = 0.0f;
            corners[1][0] = size;
            corners[1][1] = size;
            corners[1][2] = 0.0f;
            corners[2][0] = size;
            corners[2][1] = -size;
            corners[2][2] = 0.0f;
            corners[3][0] = -size;
            corners[3][1] = -size;
            corners[3][2] = 0.0f;
            sinA = math_sinBinAngle(effect->angleA);
            sinB = math_sinBinAngle(effect->angleB);
            cosA = math_cosBinAngle(effect->angleA);
            cosB = math_cosBinAngle(effect->angleB);
            for (i = 0; i < 4; i++) {
                // rotate about X by angleA, then about Y by angleB
                cx = corners[i][0];
                cy = corners[i][1];
                cz = corners[i][2];
                corners[i][1] = cosA * cy - sinA * cz;
                cz = cosA * cz + sinA * cy;
                corners[i][0] = cosB * cx - sinB * cz;
                corners[i][2] = cosB * cz + sinB * cx;
                effect->quads[gDrawBufferIndex][i].v.ob[0] = corners[i][0] + x;
                effect->quads[gDrawBufferIndex][i].v.ob[1] = corners[i][1] + y;
                effect->quads[gDrawBufferIndex][i].v.ob[2] = corners[i][2] + z;
            }
        }
        gDPPipeSync(gdl++);
        gDPSetPrimColor(gdl++, 0, 0, 255, 255, 255, alpha);
        gSPVertex(gdl++, effect->quads[gDrawBufferIndex], 4, 0);
        gSP1Triangle(gdl++, 0, 1, 2, 0);
        gSP1Triangle(gdl++, 0, 2, 3, 0);
    }
    return gdl;
}

/* Loads the next frame of texture D_800903F4 (advancing 0x80 per call, wrapping at 0x500) and sets a render mode. */
Gfx *effect_beginQuadEffectDraw(Gfx *gdl, s32 type, s32 list, s32 view) {
    u8 needSync;
    s16 frame;

    frame = gQuadEffectTexFrame;
    gDrawHookTexture = &D_800903F4;
    gQuadEffectTexFrame = frame + 0x80;
    needSync = TRUE;
    gDrawHookTextureFrame = frame;
    if (gQuadEffectTexFrame >= 0x500) {
        gQuadEffectTexFrame = 0;
    }
    gDrawHookTextureCm = 1;
    return math_gfxSetOtherMode(sprite_loadDescFrame(gdl, gDrawHookTexture, gDrawHookTextureFrame, &gDrawHookSprite, 0, 0, 0, gDrawHookTextureCm, 3),
                         gSpriteTlutMode | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, gEffectRenderModes[3].cycle1 | gEffectRenderModes[3].cycle2,
                         &needSync);
}
