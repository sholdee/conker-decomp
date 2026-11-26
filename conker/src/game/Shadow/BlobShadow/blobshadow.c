#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Render setup of the blob shadows (an I 8-bit 64x64 texture times the primitive alpha, decal blending); the
 * shadow type's entry in the object type table D_8008B4D4 points to it. */
Gfx gBlobShadowSetupDl[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPClearGeometryMode(G_FOG | G_LIGHTING | G_LOD | G_CULL_FRONT),
    gsSPSetGeometryMode(G_CULL_BACK),
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0),
    gsDPSetPrimColor(0, 0, 0, 0, 0, 0),
    gsDPSetEnvColor(0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD),
    gsDPSetTile(G_IM_FMT_I, G_IM_SIZ_8b, 8, 0, G_TX_RENDERTILE, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD,
                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD),
    gsDPSetTileSize(G_TX_RENDERTILE, 0, 0, (64 - 1) << G_TEXTURE_IMAGE_FRAC, (64 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetOtherMode(G_AD_DISABLE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE | 0xF, // 0xF: blend mask bits
                     G_AC_NONE | G_ZS_PIXEL | G_RM_AA_ZB_XLU_DECAL | G_RM_AA_ZB_XLU_DECAL2),
    gsSPEndDisplayList(),
};

/* A blob shadow: a textured quad laid on the floor under its owner, tilted to the floor slope. */
struct Shadow {
    /* 0x00 */ EffectHeader header;
    /* 0x010 */ Mtx mtx[2];        /* translation, double-buffered by D_800BE9C0 */
    /* 0x090 */ Vtx vtx[2][4];     /* quad corners, double-buffered by D_800BE9C0 */
    /* 0x110 */ void *owner;       /* an Actor (ownerType 0) or a WorldObject (ownerType 1) */
    /* 0x114 */ f32 x;
    /* 0x118 */ f32 z;
    /* 0x11C */ f32 floorY;
    /* 0x120 */ u8 ownerType;      /* 0 or 1: selects how owner is read */
    /* 0x121 */ u8 texture;        /* index into D_80091770 */
    /* 0x122 */ u8 alpha;
    /* 0x123 */ u8 visible;
    /* 0x124 */ u8 flags;
    /* 0x125 */ u8 pad125[3];
    /* 0x128 */ Vec3s floorTri[3]; /* cached floor triangle, only allocated when created with flag 1 */
}; /* size = 0x128 or 0x13A */

/* Allocates a shadow for owner and sets up its two quads' texture coordinates. */
Shadow *shadow_createBlob(s32 ownerType, void *owner, s32 texture, s32 createFlags, s32 flags) {
    Shadow *shadow;
    s32 size;
    s32 i;

    size = 0x128;
    if (createFlags & 1) {
        size = 0x13A;
    }

    shadow = effect_newObject(0x4B, 0, size, 1, 0xFF, 1);
    if (shadow != NULL) {
        for (i = 0; i < 2; i++) {
            shadow->vtx[i][0].v.flag = 0;
            shadow->vtx[i][0].v.tc[0] = 0x800;
            shadow->vtx[i][0].v.tc[1] = 0;
            shadow->vtx[i][1].v.flag = 0;
            shadow->vtx[i][1].v.tc[0] = 0;
            shadow->vtx[i][1].v.tc[1] = 0;
            shadow->vtx[i][2].v.flag = 0;
            shadow->vtx[i][2].v.tc[0] = 0;
            shadow->vtx[i][2].v.tc[1] = 0x800;
            shadow->vtx[i][3].v.flag = 0;
            shadow->vtx[i][3].v.tc[0] = 0x800;
            shadow->vtx[i][3].v.tc[1] = 0x800;
        }

        shadow->ownerType = ownerType;
        shadow->owner = owner;
        shadow->texture = texture;
        shadow->flags = flags;

        if (createFlags & 1) {
            for (i = 0; i < 9; i++) {
                ((s16 *)shadow->floorTri)[i] = 0x7FFF;
            }
        }
    }

    return shadow;
}

/* Places the shadow under its owner and builds its quad, tilted to the floor and faded with height. */
void shadow_updateBlob(Shadow *shadow) {
    s32 i;
    s32 alpha;
    Actor *obj;
    s32 pad[4]; // matching: -g3 gives unused locals stack slots; these pads keep corner, x, floorY... at the target offsets
    f32 corner[2][3];
    f32 height;
    s32 pad5; // matching: stack slot pad
    f32 ratio;
    f32 slopeZ;
    f32 slopeX;
    f32 tmp;
    f32 lenB;
    f32 lenA;
    f32 dist;
    f32 x;
    f32 ownerY;
    f32 z;
    f32 floorY;
    f32 scale;
    Vec3s **triPtrs;
    Vec3s *tri;
    u8 angle;
    f32 sinYaw;
    f32 cosYaw;
    s32 pad6; // matching: stack slot pad
    WorldObject *prop;

    shadow->visible = FALSE;
    switch (shadow->ownerType) {
        case 0:
            obj = shadow->owner;
            x = obj->xpos;
            ownerY = obj->ypos;
            z = obj->zpos;
            floorY = obj->floorY;
            if (obj->character_type == 0x25) {
                ownerY = floorY;
            }
            triPtrs = obj->floorTriVerts;
            tri = obj->floorTri.v;
            scale = (f32)(u32)obj->shadow_size * obj->scale_xz; // matching: the (u32) gives the unsigned-convert fixup
            angle = (obj->langle >> 8) + 0x20;
            break;
        case 1:
            prop = shadow->owner;
            if (prop->inactive == 1) {
                return;
            }
            tri = prop->shadow->floorTri;
            if (func_150A3FC4(prop->x, prop->z, 0, tri, &floorY) == 0) {
                collision_findFloorInRange(0, tri, &floorY, 0, 0, 0, prop->x, prop->y, prop->z, prop->y,
                              prop->object_num + 1, 0, -10000.0f, prop->y);
            }
            triPtrs = NULL;
            x = prop->x;
            ownerY = prop->y;
            z = prop->z;
            scale = (prop->shadowSize * 4) * prop->xScale;
            angle = 0;
            break;
    }
    shadow->x = x;
    shadow->z = z;
    shadow->floorY = floorY;
    if (floorY > ownerY) {
        return;
    }
onFloor: // matching: an unused label splits the block, so the call setup stays after the floorY > ownerY branch
    if (shadow_getTriSlopes(triPtrs, tri, &slopeX, &slopeZ)) {
        height = ownerY - floorY;
        if (scale == 0.0f) {
            scale = 1.0f;
        }
        dist = 200.0f + height;
        if (dist == 0.0f) {
            dist = 0.0001f;
        }
        if (dist > 800.0f) {
            dist = 800.0f;
        }
        ratio = scale / dist;
        if (shadow->flags & 1) {
            alpha = shadow->alpha;
            if (alpha != 0xFF) {
                alpha += D_800BE9E4 * 5;
                if (alpha >= 0x100) {
                    alpha = 0xFF;
                }
            }
        } else {
            alpha = 32000.0f / dist;
            if (alpha > 0xA0) {
                alpha = 0xA0;
            }
        }
        shadow->alpha = alpha;

        corner[0][0] = 0.0; // matching: double literals here (and below); 0.0f changes the code
        corner[0][2] = -100.0f;
        if (angle != 0) {
            tmp = corner[0][0];
            sinYaw = func_15048A40(angle);
            cosYaw = func_150489B0(angle);
            corner[0][0] = corner[0][2] * sinYaw + corner[0][0] * cosYaw;
            corner[0][2] = corner[0][2] * cosYaw - tmp * sinYaw;
        }
        corner[1][0] = -100.0f;
        corner[1][2] = 0.0;
        corner[0][1] = slopeX * corner[0][0] + corner[0][2] * slopeZ;
        if (angle != 0) {
            tmp = corner[1][0];
            corner[1][0] = corner[1][2] * sinYaw + corner[1][0] * cosYaw;
            corner[1][2] = corner[1][2] * cosYaw - tmp * sinYaw;
        }
        corner[1][1] = slopeX * corner[1][0] + corner[1][2] * slopeZ;

        lenA = corner[0][0] * corner[0][0] + corner[0][1] * corner[0][1] + corner[0][2] * corner[0][2];
        if (lenA > 100000000.0f) {
            lenA = 100000000.0f;
        }
        if (lenA < 10000.0f) {
            lenA = 10000.0f;
        }
        lenA = sqrtf(lenA);
        lenB = corner[1][0] * corner[1][0] + corner[1][1] * corner[1][1] + corner[1][2] * corner[1][2];
        if (lenB > 100000000.0f) {
            lenB = 100000000.0f;
        }
        if (lenB < 10000.0f) {
            lenB = 10000.0f;
        }
        lenB = sqrtf(lenB);

        for (i = 0; i < 3; i++) {
            corner[0][i] = corner[0][i] * 100.0f * ratio / lenA;
            corner[1][i] = corner[1][i] * 100.0f * ratio / lenB;
            shadow->vtx[D_800BE9C0][0].v.ob[i] = corner[0][i];
            shadow->vtx[D_800BE9C0][1].v.ob[i] = corner[1][i];
            shadow->vtx[D_800BE9C0][2].v.ob[i] = -corner[0][i];
            shadow->vtx[D_800BE9C0][3].v.ob[i] = -corner[1][i];
        }
        shadow->visible = TRUE;
    }
}


/* Draws the shadow quad, loading its texture and alpha only when they differ from the last shadow drawn. */
Gfx *shadow_drawBlob(Gfx *gfx, Shadow *shadow, s16 view) {
    s32 alpha;
    s32 ownerAlpha;
    s32 tex;

    if (shadow->visible) {
        func_150A7D00(&shadow->mtx[D_800BE9C0], shadow->x, shadow->floorY, shadow->z);
        gSPMatrix(gfx++, &shadow->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        if (D_800DD1B4 != shadow->texture) {
            (&D_800DD1B0)[1] = shadow->texture; // matching: D_800DD1B4 through another symbol keeps the split %hi/%lo
            tex = texture_getAddress(D_80091770[shadow->texture], 0, 0x3E, 0);
            gDPPipeSync(gfx++);
            gDPSetTextureImage(gfx++, G_IM_FMT_I, G_IM_SIZ_16b, 1, tex);
            gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 0x7FF, 0);
        }
        gDPPipeSync(gfx++);
        gSPClearGeometryMode(gfx++, G_CULL_BOTH);
        alpha = shadow->alpha;
        switch (shadow->ownerType) {
            case 0:
                ownerAlpha = actor_getAlpha(shadow->owner, view);
                break;
            case 1: {
                WorldObject *prop = shadow->owner;

                ownerAlpha = (prop->alpha * prop->dst_fade_alpha[view] + 0xFF) >> 8;
                break;
            }
        }
        alpha = (alpha * ownerAlpha + 0xFF) >> 8;
        if (alpha != D_800DD1C6) {
            D_800DD1C6 = alpha;
            gDPPipeSync(gfx++);
            gDPSetPrimColor(gfx++, 1, 0, 0, 0, 0, alpha);
        }
        gSPVertex(gfx++, shadow->vtx[D_800BE9C0], 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
        gDPPipeSync(gfx++);
    }
    return gfx;
}

/*
 * Solves the plane of a triangle (given as vertices or as vertex pointers) for its slopes dy/dx and dy/dz,
 * trying each corner and edge order until the system is solvable. Returns 0 (slopes 0) if none is.
 */
s32 shadow_getTriSlopes(Vec3s **triPtrs, Vec3s *tri, f32 *slopeX, f32 *slopeZ) {
    f32 ax;
    f32 ay;
    f32 az;
    f32 bx;
    f32 by;
    f32 bz;
    f32 tmp;
    f32 cross0;
    f32 cross1;
    f32 slope;
    s32 attempt;
    s32 origin;
    s32 next;
    s32 last;
    s32 status;

    attempt = 0;
    if (triPtrs == NULL && tri == NULL) {
        *slopeZ = 0.0f;
        *slopeX = 0.0f;
        return 0;
    }

    do {
        status = 1;
        if (attempt >= 3) {
            origin = attempt - 3;
        } else {
            origin = attempt;
        }
        if (origin != 2) {
            next = origin + 1;
        } else {
            next = 0;
        }
        if (next != 2) {
            last = next + 1;
        } else {
            last = 0;
        }

        if (tri != NULL) {
            ax = tri[next].x - tri[origin].x;
            ay = tri[next].y - tri[origin].y;
            az = tri[next].z - tri[origin].z;
            bx = tri[last].x - tri[origin].x;
            by = tri[last].y - tri[origin].y;
            bz = tri[last].z - tri[origin].z;
        } else {
            ax = triPtrs[next]->x - triPtrs[origin]->x;
            ay = triPtrs[next]->y - triPtrs[origin]->y;
            az = triPtrs[next]->z - triPtrs[origin]->z;
            bx = triPtrs[last]->x - triPtrs[origin]->x;
            by = triPtrs[last]->y - triPtrs[origin]->y;
            bz = triPtrs[last]->z - triPtrs[origin]->z;
        }

        if (attempt >= 3) {
            tmp = ax;
            ax = bx;
            bx = tmp;
            tmp = ay;
            ay = by;
            by = tmp;
            tmp = az;
            az = bz;
            bz = tmp;
        }
        cross0 = bx * az;
        cross1 = ax * bz;
        if (cross0 == cross1) {
            status = 0;
        }
        if (bx == 0.0f) {
            status = 0;
        }
        attempt++;
        if (attempt == 6) {
            status = 2;
        }
    } while (status == 0);

    if (status == 2) {
        *slopeX = 0.0f;
        *slopeZ = 0.0f;
        return 0;
    }

    slope = (bx * ay - ax * by) / (cross0 - cross1);
    *slopeZ = slope;
    *slopeX = (by - slope * bz) / bx;
    return 1;
}


/* Loads the fixed matrix gIdentityMtx as the modelview matrix (arg1 and arg2 are unused). */
Gfx *func_1510E634(Gfx *gfx, s32 arg1, s32 arg2) {
    gSPMatrix(gfx++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    return gfx;
}
