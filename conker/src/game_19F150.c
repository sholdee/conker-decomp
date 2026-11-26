#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Render setup and teardown of the sprite particles (object types 15 and 67 of gObjectTypes). */
Gfx gFloorSpriteSetupDl[] = {
    gsSPMatrix(&D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPClearGeometryMode(G_CULL_BOTH | G_FOG | G_LOD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};
Gfx gFloorSpriteEndDl[] = {
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

/* Allocates a particle and initialises its animation state. */
Particle19F150 *particle_newFloorSprite(s16 lifetime, u16 color, u8 animIndex, s32 flags, s32 animSpeed, s32 arg5, u8 arg6,
                              s32 arg7) {
    Particle19F150 *particle = effect_newObject(arg5 == 0 ? 0xF : 0x43, arg7, sizeof(Particle19F150), 1, arg6, 1);

    if (particle == NULL) {
        return NULL;
    }
    particle->lifetime = lifetime;
    particle->animFrame = 0;
    particle->animSpeed = animSpeed;
    particle->color = color;
    particle->animIndex = animIndex;
    particle->flags = flags;
    particle->vtx[0].v.flag = 0;
    particle->vtx[1].v.flag = 0;
    particle->vtx[2].v.flag = 0;
    particle->vtx[3].v.flag = 0;
    return particle;
}

/*
 * Spawns a sprite particle at (x, y, z). Normally the quad is laid on the floor below a point `dist` away along
 * `angle`; with flags & 2 it is a flat horizontal square of half-size `size`.
 */
void particle_spawnFloorSprite(f32 x, f32 y, f32 z, s16 lifetime, u16 color, u8 animIndex, f32 angle, s32 dist, s32 size,
                   s32 flags, s32 animSpeed, s32 arg11, u8 arg12, s32 arg13) {
    Particle19F150 *particle;

    particle = particle_newFloorSprite(lifetime, color, animIndex, flags, animSpeed, arg11, arg12, arg13);
    if (particle != NULL) {
        if (!(flags & 2)) {
            if (particle_layQuadOnFloor(particle->vtx, x, y, z, angle, dist, size, gEffectAnimTextures[particle->animIndex]->width) == 0) {
                effect_destroyObject(&particle->header);
            }
        } else {
            particle->vtx[0].v.ob[0] = particle->vtx[3].v.ob[0] = x - size;
            particle->vtx[1].v.ob[0] = particle->vtx[2].v.ob[0] = size + x;
            particle->vtx[0].v.ob[1] = particle->vtx[1].v.ob[1] = particle->vtx[2].v.ob[1] = particle->vtx[3].v.ob[1] = y;
            particle->vtx[0].v.ob[2] = particle->vtx[1].v.ob[2] = z - size;
            particle->vtx[2].v.ob[2] = particle->vtx[3].v.ob[2] = size + z;
            particle->vtx[0].v.tc[0] = particle->vtx[3].v.tc[0] = 0x2000;
            particle->vtx[1].v.tc[0] = particle->vtx[2].v.tc[0] = (gEffectAnimTextures[animIndex]->width + 0x100) << 5;
            particle->vtx[0].v.tc[1] = particle->vtx[1].v.tc[1] = 0x2000;
            particle->vtx[2].v.tc[1] = particle->vtx[3].v.tc[1] = (gEffectAnimTextures[animIndex]->height + 0x100) << 5;
        }
    }
}

/* Spawns a sprite particle laid on collision triangle `tri` at (x, y, z). */
void particle_spawnTriSprite(f32 x, f32 y, f32 z, void *tri, s16 lifetime, u16 color, u8 animIndex, s32 size, s32 flags,
                   s32 animSpeed, s32 arg10, u8 arg11, s32 arg12) {
    Particle19F150 *particle;

    particle = particle_newFloorSprite(lifetime, color, animIndex, flags, animSpeed, arg10, arg11, arg12);
    if (particle != NULL) {
        if (particle_layQuadOnTri(particle->vtx, tri, x, y, z, 0.0f, size, gEffectAnimTextures[particle->animIndex]->width) == 0) {
            effect_destroyObject(&particle->header);
        }
    }
}

/* Per-frame update: counts the lifetime down by the frame delta and advances the animation. */
void particle_updateFloorSprite(Particle19F150 *particle) {
    s32 lifetime;
    s32 frameCount;

    lifetime = particle->lifetime;
    if (lifetime < -1) {
        particle->lifetime = lifetime + 1;
        if (particle->lifetime == -1) {
            effect_destroyObject(&particle->header);
        }
        return;
    }

    if (lifetime == 0) {
        particle->lifetime = -3;
        return;
    }

    if (lifetime == -1) {
        return;
    }

    if (!(particle->flags & 4)) {
        if (D_800BE9E4 < lifetime) {
            particle->lifetime = lifetime - D_800BE9E4;
        } else {
            particle->lifetime = 0;
        }
    }

    particle->animFrame += particle->animSpeed;
    frameCount = gEffectAnimTextures[particle->animIndex]->frameCount;
    if (particle->animFrame / 256 < frameCount) {
        return;
    }

    if (particle->flags & 4) {
        particle->lifetime = -3;
    } else {
        particle->animFrame = (frameCount << 8) - 0x100;
    }
}

/* Cached draw state shared by all sprite particles: texture, frame, cycle|combine mode, primitive colour. */
/*
 * Draws a sprite particle (once its lifetime is >= 2), setting the texture, cycle type, render mode, combiner and
 * primitive colour only when they differ from the cached state in D_800DD1B4..D_800DD1C6.
 */
Gfx *particle_drawFloorSprite(Gfx *gfx, Particle19F150 *particle, s32 arg2) {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    u32 *frames;
    s32 cycle;
    s32 combine;
    s32 newCycle;
    s32 newCombine;
    SpriteParams texState; // filled in by sprite_loadDescFrame; unused here
    s32 tex;
    s32 frame;
    s32 needSync;
    TextureDesc *anim;
    s32 othermodeH;
    s32 othermodeL; // sic: left unset (garbage) when the cycle type is already current

    needSync = 1;
    if (particle->lifetime >= 2) {
        anim = gEffectAnimTextures[particle->animIndex];
        frame = particle->animFrame >> 8;
        frames = (u32 *)anim->addr;
        tex = (s32)frames;
        if ((u32)frames >= 0x10000000) {
            tex = frames[frame];
            frame = 0;
        }
        if (tex != D_800DD1B4 || frame != D_800DD1B8) {
            gfx = sprite_loadDescFrame(gfx, anim, particle->animFrame, &texState, 0, 0, 0, 2, 3);
            D_800DD1B4 = tex;
            D_800DD1B8 = frame;
        }
        cycle = D_800DD1BC & 0xF;
        combine = D_800DD1BC & 0xF0;
        if (!(particle->flags & 1)) {
            newCycle = 1;
        } else {
            newCycle = 2;
        }
        if (anim->fmt == 5) {
            newCycle += 2;
        }
        if (cycle != newCycle) {
            cycle = newCycle;
            if (needSync) {
                gDPPipeSync(gfx++);
            }
            needSync = 0;
            switch (newCycle) {
            case 1:
                othermodeH = G_CYC_1CYCLE;
                othermodeL = G_RM_AA_ZB_XLU_DECAL | G_RM_AA_ZB_XLU_DECAL2;
                break;
            case 2:
                othermodeH = G_CYC_1CYCLE;
                othermodeL = G_RM_ZB_CLD_SURF | G_RM_ZB_CLD_SURF2;
                break;
            case 3:
                othermodeH = G_CYC_2CYCLE;
                othermodeL = G_RM_PASS | G_RM_AA_ZB_XLU_DECAL2;
                break;
            case 4:
                othermodeH = G_CYC_2CYCLE;
                othermodeL = G_RM_PASS | G_RM_ZB_CLD_SURF2;
                break;
            }
            gDPSetAlphaCompare(gfx++, G_AC_NONE);
            gSPClearGeometryMode(gfx++, G_FOG | G_LOD);
        }
        // sic: passes the high byte of the s32 needSync
        gfx = func_15142FBC(gfx, othermodeH | G_TP_PERSP | gSpriteTlutMode | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE |
                                     G_AD_DISABLE,
                            othermodeL, (u8 *)&needSync);
        if (!(particle->flags & 8)) {
            if (anim->fmt == 0) {
                if (particle->color) {
                    newCombine = 0x10;
                } else {
                    newCombine = 0x20;
                }
            } else if (anim->fmt == 5) {
                if (particle->color) {
                    newCombine = 0x60;
                } else {
                    newCombine = 0x50;
                }
            } else {
                newCombine = 0x30;
            }
        } else {
            newCombine = 0x40;
        }
        if (combine != newCombine) {
            combine = newCombine;
            if (needSync) {
                gDPPipeSync(gfx++);
            }
            switch (newCombine) {
            case 0x10:
                gDPSetCombineLERP(gfx++, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0,
                                  PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL0, 0);
                break;
            case 0x20:
                gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, PRIMITIVE, 0, TEXEL0, 0,
                                  0, 0, 0, TEXEL0, PRIMITIVE, 0, TEXEL0, 0);
                break;
            case 0x30:
                gDPSetCombineLERP(gfx++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0,
                                  0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0);
                break;
            case 0x40:
                gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0,
                                  0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0);
                break;
            case 0x50:
                gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, PRIMITIVE, 0, TEXEL1, 0,
                                  0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
                break;
            case 0x60:
                gDPSetCombineLERP(gfx++, PRIMITIVE, 0, TEXEL0, 0, PRIMITIVE, 0, TEXEL1, 0,
                                  0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
                break;
            }
        }
        D_800DD1BC = cycle | combine;
        r = (particle->color >> 8) & 0xF8;
        g = (particle->color >> 3) & 0xF8;
        b = (particle->color << 2) & 0xF8;
        if (particle->lifetime >= 26) {
            a = 0xFF;
        } else {
            a = particle->lifetime * 10;
        }
        if (r != D_800DD1C0 || g != D_800DD1C2 || b != D_800DD1C4 || a != D_800DD1C6) {
            D_800DD1C0 = r;
            D_800DD1C2 = g;
            D_800DD1C4 = b;
            D_800DD1C6 = a;
            if (needSync) {
                gDPPipeSync(gfx++);
            }
            gDPSetPrimColor(gfx++, 1, 0, r, g, b, a);
        }
        gSPVertex(gfx++, particle->vtx, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
    }
    return gfx;
}

/* Quad corner texture coordinates (S10.5) for 64-, 32- and 16-texel-wide textures. */
static const s16 gQuadTexCoords64[4][2] = { { 64 << 5, 0 }, { 0, 0 }, { 0, 64 << 5 }, { 64 << 5, 64 << 5 } };
static const s16 gQuadTexCoords32[4][2] = { { 32 << 5, 0 }, { 0, 0 }, { 0, 32 << 5 }, { 32 << 5, 32 << 5 } };
static const s16 gQuadTexCoords16[4][2] = { { 16 << 5, 0 }, { 0, 0 }, { 0, 16 << 5 }, { 16 << 5, 16 << 5 } };

/*
 * Fills `vtx` with a quad of half-size `size` lying on the slope of collision triangle `tri`, centred on (x, y, z),
 * with its texture coordinates (for a texture `width` texels wide) rotated by `angle` degrees.
 */
s32 particle_layQuadOnTri(Vtx *vtx, void *tri, f32 x, f32 y, f32 z, f32 angle, s32 size, u8 width) {
    f32 lenA;
    f32 lenB;
    f32 minLenSq;
    f32 denom;
    f32 unused1; // matching: unused1..unused4 keep the original stack layout
    f32 unused2;
    f32 unused3;
    f32 axis[2][3];
    f32 unused4;
    f32 theta;
    f32 slopeZ;
    f32 slopeX;
    f32 centerS;
    f32 centerT;
    f32 cosT;
    f32 sinT;
    const s16 (*cornerUV)[2];
    Vtx *quad; // matching: the texture-coordinate loop must index the parameter, the others this copy
    s32 n;
    s32 i;

    quad = vtx;
    n = size / 16;
    if (n == 0) {
        n = 1;
    }
    if (shadow_getTriSlopes(tri, NULL, &slopeX, &slopeZ) == 0) {
        return 0;
    }
    axis[0][0] = 0.0f;
    axis[0][2] = -n;
    axis[0][1] = slopeX * axis[0][0] + slopeZ * axis[0][2];
    denom = (slopeZ + slopeZ) * (-axis[0][1] - axis[0][1]) + ((-axis[0][2] - axis[0][2]) + (-axis[0][2] - axis[0][2]));
    if (denom == 0.0f) {
        denom = 0.001000000047f;
    }
    axis[1][0] = -n;
    axis[1][1] = ((slopeX * axis[1][0] + slopeX * axis[1][0]) * (-axis[0][2] - axis[0][2])) / denom;
    if (slopeZ == 0.0f) {
        axis[1][2] = 0.0f;
    } else {
        axis[1][2] = (axis[1][1] - slopeX * axis[1][0]) / slopeZ;
    }
    lenA = axis[0][0] * axis[0][0] + axis[0][1] * axis[0][1] + axis[0][2] * axis[0][2];
    if (lenA > 100000000.0f) {
        lenA = 100000000.0f;
    }
    minLenSq = n * n;
    if (lenA < minLenSq) {
        lenA = minLenSq;
    }
    lenA = sqrtf(lenA);
    lenB = axis[1][0] * axis[1][0] + axis[1][1] * axis[1][1] + axis[1][2] * axis[1][2];
    if (lenB > 100000000.0f) {
        lenB = 100000000.0f;
    }
    if (lenB < minLenSq) {
        lenB = minLenSq;
    }
    lenB = sqrtf(lenB);
    for (i = 0; i < 3; i++) {
        axis[0][i] = axis[0][i] * size / lenA;
        axis[1][i] = axis[1][i] * size / lenB;
        quad[0].v.ob[i] = axis[0][i];
        quad[1].v.ob[i] = axis[1][i];
        quad[2].v.ob[i] = -axis[0][i];
        quad[3].v.ob[i] = -axis[1][i];
    }
    theta = (angle - 225.0f) * 0.01745329238f;
    cosT = cosf(theta);
    sinT = sinf(theta);
    if (width == 0x10) {
        centerT = centerS = 256.0f;
        cornerUV = gQuadTexCoords16;
    } else if (width == 0x20) {
        centerT = centerS = 512.0f;
        cornerUV = gQuadTexCoords32;
    } else {
        centerT = centerS = 1024.0f;
        cornerUV = gQuadTexCoords64;
    }
    for (i = 0; i < 4; i++) {
        vtx[i].v.tc[0] = ((cornerUV[i][0] - centerS) * cosT - (cornerUV[i][1] - centerT) * sinT) + centerS + 8192.0f;
        vtx[i].v.tc[1] = ((cornerUV[i][0] - centerS) * sinT + (cornerUV[i][1] - centerT) * cosT) + centerT + 8192.0f;
    }
    for (i = 0; i < 4; i++) {
        quad[i].v.ob[0] = quad[i].v.ob[0] + x;
        quad[i].v.ob[1] = quad[i].v.ob[1] + y;
        quad[i].v.ob[2] = quad[i].v.ob[2] + z;
    }
    return 1;
}


/*
 * Finds the floor below the point `dist` away from (x, z) along `angle` degrees and lays a quad of half-size `size`
 * on it (see particle_layQuadOnTri). Returns 0 when there is no floor.
 */
s32 particle_layQuadOnFloor(Vtx *vtx, f32 x, f32 y, f32 z, f32 angle, s32 dist, s32 size, u8 width) {
    struct FloorHit {
        void *tri;
        f32 floorY;
        u32 unk8;
        s32 unkC;
    };
    f32 sinT;
    f32 cosT;
    struct FloorHit floor;

    cosT = cosf((angle - 90.0f) * 0.01745329238f);
    sinT = sinf((angle - 90.0f) * 0.01745329238f);
    x += dist * sinT;
    z += dist * cosT;
    collision_findFloor(&floor.tri, 0, &floor.floorY, 0, &floor.unk8, 0, x, y, z, y, 0, 0);
    if (floor.tri == NULL) {
        return 0;
    }
    if (floor.floorY == -10000.0f) {
        return 0;
    }
    return particle_layQuadOnTri(vtx, floor.tri, x, floor.floorY, z, angle, size, width);
}
