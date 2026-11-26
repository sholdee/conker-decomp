#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/shadow.h describes it. */
Vec3f D_800DDFB0[8]; /* light position per shadow slot */
Vec3f D_800DE010;    /* default light offset from the object */
/* Shadow settings, set per level by func_151872B0. */
s32 D_800DE01C; /* vertex budget cap */
s32 D_800DE020; /* below this budget D_800DE028 eases back up */
s32 D_800DE024; /* vertex budget floor */
f32 D_800DE028;
f32 D_800DE02C; /* shadow alpha */
s32 D_800DE030; /* shadow viewport depth scale, in 1/1024 */
s32 D_800DE034; /* nonzero: skip the light nodes and always use the default light offset */
s32 D_800DE038; /* max lights (shadows) per object */
f32 D_800DE03C; /* light distance scale */
u8 D_800DE040;  /* nonzero: keep the light at least 30 degrees above the object */
u8 D_800DE041;  /* nonzero: only faces flagged in the D_800D3668 table receive shadows */
u8 D_800DE048[0x1040]; /* the 64x64 shadow texture, at the first 64-byte boundary (D_800DF088) */
s32 D_800DF088;
Vp D_800DF090[4];
u16 D_800DF0D0[8]; /* perspective normalisation per shadow slot */
u8 D_800DF0E0;
u8 D_800DF0E1;
s32 D_800DF0E4;
s32 D_800DF0E8;
s32 D_800DF0EC;
s32 D_800DF0F0;
s32 D_800DF0F4;
MtxF D_800DF0F8[8];
Mtx D_800DF2F8[8][2];
f32 D_800DF6F8; /* light-to-object distance */

#include "functions.h"
#include "variables.h"

extern f32 D_800D3688;  /* shadow near plane (light-space z) */

static s32 D_8008D3E0 = 0; /* shadow vertex budget for this frame */
/* The viewport of the 64x64 shadow texture. */
static Vp D_8008D3E8 = { { { 128, 128, 511, 0 }, { 128, 128, 511, 0 } } };
/* The corners of an object's bounding box, as signs of its half-extents. */
static s8 D_8008D3F8[8][3] = {
    { 1, 1, 1 }, { 1, 1, -1 }, { 1, -1, -1 }, { 1, -1, 1 }, { -1, 1, 1 }, { -1, 1, -1 }, { -1, -1, -1 }, { -1, -1, 1 },
};
/* Render setup of the shadow receivers: shade times the inverted texture. The last entry stays zero. */
static Gfx D_8008D410[7] = {
    gsDPPipeSync(),
    gsDPSetCombineLERP(0, 0, 0, SHADE, 1, TEXEL0, SHADE, 0, 0, 0, 0, SHADE, 1, TEXEL0, SHADE, 0),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPClearGeometryMode(G_CULL_FRONT | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD),
    gsSPSetGeometryMode(G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};
/* Clears the shadow texture to white before the silhouette is drawn into it. */
static Gfx D_8008D448[] = {
    gsDPSetFillColor(0xFFFFFFFF),
    gsDPSetOtherMode(0x302C3F, 0x00000000),
    gsSPClearGeometryMode(G_ZBUFFER | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_SHADING_SMOOTH),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, 65, 65),
    gsDPFillRectangle(0, 0, 63, 63),
    gsDPPipeSync(),
    gsDPSetScissor(G_SC_NON_INTERLACE, 1, 1, 63, 63),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetOtherMode(0x082C3F, 0x00000000),
    gsSPEndDisplayList(),
};
/* Per clip plane: edge intersection, and the inside test. */
static ClipVtx *(*D_8008D498[])(ClipVtx *, ClipVtx *, ClipVtx *) = {
    func_15185454, func_1518557C, func_151856A8, func_151857DC, func_151858FC,
    func_15185A28, func_15185B54, func_15185B54, func_15185C6C, func_15185C6C,
};
static s32 (*D_8008D4C0[])(ClipVtx *) = {
    func_15185554, func_1518567C, func_151857B0, func_151858D4, func_151859FC,
    func_15185B28, func_15185C44, func_15185D84, func_15185DAC, func_15185D5C,
};

/* Half-extents of an object's bounding box, from its bone matrices or, failing that, its model size. */
s32 func_15184150(s32 objIndex, f32 *halfX, f32 *halfY, f32 *halfZ) {
    s32 id;
    Actor *obj;
    s32 count;
    s32 i;
    f32 dx;
    f32 dy;
    f32 dz;
    MtxF *bones;

    obj = &movement_obj[objIndex];
    id = obj->character_type;
    *halfZ = 0.0f;
    *halfY = 0.0f;
    *halfX = 0.0f;
    if (obj->ob_matrix_ptr == NULL || D_800C3E90 != 0) {
        return 0;
    }
    count = actor_getBoneMatrixCount(id);
    for (i = 0; i < count; i++) {
        bones = obj->ob_matrix_ptr;
        dx = bones[i][3][0];
        dy = bones[i][3][1];
        dz = bones[i][3][2];
        dx -= obj->xpos;
        dy -= obj->ypos;
        dz -= obj->zpos;
        if (dx < 0.0f) {
            dx = -dx;
        }
        if (*halfX < dx) {
            *halfX = dx;
        }
        if (dy < 0.0f) {
            dy = -dy;
        }
        if (*halfY < dy) {
            *halfY = dy;
        }
        if (dz < 0.0f) {
            dz = -dz;
        }
        if (dz > *halfZ) {
            *halfZ = dz;
        }
    }

    if (*halfX == 0.0f || *halfY == 0.0f || *halfZ == 0.0f) {
        dx = D_800D1C90[obj->character_type]->unk1A;
        dy = obj->scale_xz * dx;
        *halfZ = dy;
        *halfX = dy;
        dx = D_800D1C90[obj->character_type]->unk1C + D_800D1C90[obj->character_type]->unk1E;
        *halfY = obj->scale_y * dx;
    }
}

extern f32 D_800D3680;
extern f32 D_800D3684;
extern f32 D_800D368C;
extern s32 math_distance3D(s32, s32, s32);

/*
 * Sets up the shadow projection for object objIndex in shadow slot `slot`: picks the nearest light node
 * beyond minDist (or a default offset), eases the light toward it, builds the light's view/projection
 * matrices and the shadow alpha. Returns the chosen light's distance (1 for the default light), or 0 when no
 * shadow should be drawn.
 */
f32 func_15184368(s32 objIndex, s32 slot, f32 halfX, f32 halfY, f32 halfZ, f32 minDist) {
    Vec3f delta;
    Vec3f objPos;
    Vec3f lightPos;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 maxX;
    f32 maxY;
    f32 maxZ;
    f32 dist;
    f32 scale;
    f32 minY;
    MtxF proj;
    MtxF bias;
    s32 i;
    s32 done;
    s32 found;
    PointLight *node;
    f32 nodePos[3];
    f32 perspScale;
    f32 v;
    Actor *obj;
    Vec3f *eye;

    obj = &movement_obj[objIndex];
    done = 0;
    dist = 5000.0f;
    objPos.x = obj->xpos;
    perspScale = 2.0f;
    objPos.y = obj->ypos;
    found = 0;
    objPos.z = obj->zpos;
    if (D_800DE034 == 0) {
        node = gPointLights;
        while (node != NULL) {
            if (node->unk4 == 0 && node->disabled == 0) {
                nodePos[0] = node->x;
                nodePos[1] = node->y;
                nodePos[2] = node->z;
                v = math_distance3D(nodePos[0] - objPos.x, nodePos[1] - objPos.y, nodePos[2] - objPos.z);
                if (v < dist && minDist < v) {
                    dist = v;
                    found++;
                    lightPos.x = nodePos[0];
                    lightPos.y = nodePos[1];
                    lightPos.z = nodePos[2];
                }
            }
            node = node->next;
        }
    }
    if (found == 0) {
        if (minDist != 0.0f) {
            return 0.0f;
        }
        if (D_800DE010.y <= 0.0f) {
            D_800DE010.x = 100.0f;
            D_800DE010.y = 600.0f;
            D_800DE010.z = -200.0f;
        }
        dist = 1.0f;
        lightPos.x = D_800DE010.x * D_800DE03C + objPos.x;
        lightPos.y = D_800DE010.y * D_800DE03C + objPos.y;
        lightPos.z = D_800DE010.z * D_800DE03C + objPos.z;
    }
    if (*(s32 *)D_800DBFF4 != 0 || D_800DE038 >= 2) {
        eye = &D_800DDFB0[slot];
        eye->x = lightPos.x;
        eye->y = lightPos.y;
        eye->z = lightPos.z;
    } else {
        eye = &D_800DDFB0[slot];
        eye->x = eye->x + (lightPos.x - eye->x) * 0.3f;
        eye->y = eye->y + (lightPos.y - eye->y) * 0.3f;
        eye->z = eye->z + (lightPos.z - eye->z) * 0.3f;
    }
    func_15048F90(&objPos, eye, &delta);
    D_800DF6F8 = func_150AD930(&delta.x);
    if (D_800DF6F8 < 200.0f) {
        f32 ratio;
        ratio = 200.0f / D_800DF6F8;
        D_800DF6F8 = 200.0f;
        v = delta.x * ratio;
        D_800DDFB0[slot].x += v - delta.x;
        delta.x = v;
        v = delta.y * ratio;
        D_800DDFB0[slot].y += v - delta.y;
        delta.y = v;
        v = delta.z * ratio;
        D_800DDFB0[slot].z += v - delta.z;
        delta.z = v;
    }
    if (D_800DE040 != 0) {
        minY = sqrtf(delta.x * delta.x + delta.z * delta.z) * 0.57735f;
        if (minY < 10.0f) {
            minY = 10.0f;
        }
    } else {
        minY = 10.0f;
    }
    if (delta.y < minY) {
        D_800DDFB0[slot].y += minY - delta.y;
        delta.y = minY;
        v = math_distance3D(delta.x, delta.y, delta.z);
        scale = D_800DF6F8 / v;
        D_800DF6F8 = v;
        D_800DDFB0[slot].x += delta.x * scale - delta.x;
        D_800DDFB0[slot].y += delta.y * scale - delta.y;
        D_800DDFB0[slot].z += delta.z * scale - delta.z;
    }
    if (5000.0f * D_800DE03C < D_800DF6F8) {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0.0f;
    }
    if (D_800DF0E1 != 0) {
        halfY *= 0.25f;
        D_800D3688 = -(D_800DE028 * D_800DE03C * 0.5f) - D_800DF6F8;
    } else {
        D_800D3688 = -D_800DE028 * D_800DE03C - D_800DF6F8;
    }
    func_15047390(D_800DF0F8[slot], D_800DDFB0[slot].x, D_800DDFB0[slot].y, D_800DDFB0[slot].z, objPos.x, objPos.y, objPos.z, 0.0f, 1.0f, 0.0f);
    maxY = 0.0f;
    maxX = 0.0f;
    maxZ = -1000000.0f;
    for (i = 0; i < 8; i++) {
        func_150A7960(D_800DF0F8[slot], D_8008D3F8[i][0] * halfX + objPos.x, D_8008D3F8[i][1] * halfY + objPos.y,
                      D_8008D3F8[i][2] * halfZ + objPos.z, &sx, &sy, &sz);
        if (sz == 0.0f) {
            sz = 0.0001f;
        }
        sx /= sz;
        sy /= sz;
        if (sx < 0.0f) {
            sx = -sx;
        }
        if (maxX < sx) {
            maxX = sx;
        }
        if (sy < 0.0f) {
            sy = -sy;
        }
        if (maxY < sy) {
            maxY = sy;
        }
        if (maxZ < sz) {
            maxZ = sz;
        }
    }
    if (maxX < 0.01f || maxY < 0.01f) {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0.0f;
    }
    D_800D3680 = 1.0f / maxX;
    D_800D3684 = 1.0f / maxY;
    if (D_800D3680 < 1.0f) {
        D_800D3680 = 1.0f;
    }
    if (D_800D3684 < 1.0f) {
        D_800D3684 = 1.0f;
    }
    if (maxZ < -1.0f) {
        D_800D368C = maxZ;
        do {
            f32 m = 1.0f;
            guPerspectiveF(proj, &D_800DF0D0[slot], func_150484A0(1.0f, D_800D3684) * 57.29577637f,
                           func_150484A0(1.0f, D_800D3680) * 57.29577637f, -D_800D368C, -D_800D3688, perspScale);
            func_150A7A48(D_800DF0F8[slot], proj, proj);
            for (i = 0; i < 16; i++) {
                v = ((f32 *)proj)[i];
                if (v < 0.0f) {
                    v = -v;
                }
                if (m < v) {
                    m = v;
                }
            }
            if (m < 32767.0f) {
                done = 1;
            } else {
                perspScale *= 0.5f;
            }
        } while (done == 0);
        func_150A7CB0(bias, 0.5f, 0.5f, 1.0f);
        func_150A7A48(proj, bias, proj);
        guMtxF2L(proj, &D_800DF2F8[slot][D_800BE9C0]);
    } else {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0.0f;
    }
    /* The shadow texture lives in D_800DE048, 64-byte aligned. */
    D_800DF088 = ((s32)D_800DE048 + 0x3F) & ~0x3F;
    D_800DE02C = 0xFF - (movement_obj[objIndex].unk1DD[0] >> 1);
    if (D_800DE02C > 185.0f) {
        D_800DE02C = 185.0f;
    }
    if (movement_obj[objIndex].unkCC < -10.0f) {
        D_800DE02C += D_800DE02C * (movement_obj[objIndex].unkCC + 10.0f) * 0.02f;
        if (D_800DE02C <= 5.0f) {
            return 0.0f;
        }
    }
    return dist;
}

/*
 * Shadow bounds for object objIndex in view `view`: returns its distance from that view's position and its
 * half-extents scaled by 1.3, or 0 (clearing the z clip planes) when it is too far away or degenerate.
 */
s32 func_15184DF0(s32 objIndex, s32 view, f32 *halfX, f32 *halfY, f32 *halfZ) {
    f32 x;
    f32 y;
    f32 z;
    s32 dist;

    x = movement_obj[objIndex].xpos;
    y = movement_obj[objIndex].ypos;
    z = movement_obj[objIndex].zpos;
    dist = math_distance3D(x - camera_global_Cameras[view].eye.x,
                         y - camera_global_Cameras[view].eye.y,
                         z - camera_global_Cameras[view].eye.z);
    if (dist > 2000) {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0;
    }
    func_15184150(objIndex, halfX, halfY, halfZ);
    *halfX *= 1.3f;
    *halfY *= 1.3f;
    *halfZ *= 1.3f;
    if ((*halfX == 0.0f) || (*halfY == 0.0f) || (*halfZ == 0.0f)) {
        D_800D3688 = 0.0f;
        D_800D368C = D_800D3688;
        return 0;
    }
    return dist;
}

extern Mtx D_89470;

/*
 * Renders object objIndex in black into the 64x64 shadow texture at D_800DF088 through slot's light
 * projection, then (for low lod values) softens it by redrawing the texture over itself once or twice.
 */
Gfx *func_15184FA4(Gfx *gfx, s32 objIndex, s32 slot, s32 view) {
    s32 lod;
    s32 col[3];

    if (movement_obj[objIndex].playernumber == 0xFF) {
        lod = movement_obj[objIndex].current_lod + 1;
    } else {
        lod = movement_obj[objIndex].current_lod;
    }
    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_CI, G_IM_SIZ_8b, 64, D_800DF088);
    gSPDisplayList(gfx++, D_8008D448);
    gSPPerspNormalize(gfx++, D_800DF0D0[slot]);
    gSPViewport(gfx++, &D_8008D3E8);
    gSPMatrix(gfx++, &D_800DF2F8[slot][D_800BE9C0], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &D_89470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    col[0] = col[1] = col[2] = 0;
    gfx = actor_emitModel(gfx, objIndex, view, movement_obj[objIndex].ob_matrix_ptr, actor_getAlpha(&movement_obj[objIndex], view), col, 3, 1);
    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPPipeSync(gfx++);
    if (lod < 2) {
        gDPSetCombineLERP(gfx++, 0, 0, 0, TEXEL0, 0, 0, 0, SHADE, 0, 0, 0, TEXEL0, 0, 0, 0, SHADE);
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, 255);
        gDPSetOtherMode(gfx++, 0x3C3F, 0x0F0A4000);
        gDPLoadTextureBlock(gfx++, D_800DF088, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_WRAP, G_TX_WRAP, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(gfx++, 0, 0, 63 << 2, 63 << 2, G_TX_RENDERTILE, 1 << 4, 1 << 4, 1 << 10, 1 << 10);
        if (lod == 0) {
            gDPPipeSync(gfx++);
            gDPLoadTextureBlock(gfx++, D_800DF088, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_WRAP, G_TX_WRAP, 6, 6, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(gfx++, 0, 0, 63 << 2, 63 << 2, G_TX_RENDERTILE, 1 << 4, 1 << 4, 1 << 10, 1 << 10);
        }
        gDPPipeSync(gfx++);
    }
    gSPSetGeometryMode(gfx++, G_SHADING_SMOOTH | G_ZBUFFER);
    gfx = view_setScissor(view_setColorImage(gfx), gCurrentView, 0, 0, 0, 0);
    gfx = view_emitRspSetup(gfx, view);
    movement_obj[objIndex].unk123 = 1;
    return gfx;
}

/*
 * Clip-plane edge intersections (D_8008D498): each writes the point where edge a->b crosses its plane to
 * out, interpolating the model-space position too, and returns out + 1. The paired tests (D_8008D4C0)
 * say whether a vertex is on the kept side.
 */

/* Plane y == z. */
ClipVtx *func_15185454(ClipVtx *a, ClipVtx *b, ClipVtx *out) {
    f32 t;
    f32 y;

    t = (a->z - a->y) / ((b->y - a->y) - (b->z - a->z));
    out->x = a->x + (b->x - a->x) * t;
    y = a->y + (b->y - a->y) * t;
    out->z = y;
    out->y = y;
    out->modelX = a->modelX + (b->modelX - a->modelX) * t;
    out->modelY = a->modelY + (b->modelY - a->modelY) * t;
    out->modelZ = a->modelZ + (b->modelZ - a->modelZ) * t;
    return out + 1;
}

s32 func_15185554(ClipVtx *v) {
    return v->z < v->y;
}

/* Plane x == -z. */
ClipVtx *func_1518557C(ClipVtx *a, ClipVtx *b, ClipVtx *out) {
    f32 t;

    t = (a->z + a->x) / ((a->x - b->x) + (a->z - b->z));
    out->x = a->x + (b->x - a->x) * t;
    out->y = a->y + (b->y - a->y) * t;
    out->z = -out->x;
    out->modelX = a->modelX + (b->modelX - a->modelX) * t;
    out->modelY = a->modelY + (b->modelY - a->modelY) * t;
    out->modelZ = a->modelZ + (b->modelZ - a->modelZ) * t;
    return out + 1;
}

s32 func_1518567C(ClipVtx *v) {
    return v->x < -v->z;
}

/* Plane y == -z. */
ClipVtx *func_151856A8(ClipVtx *a, ClipVtx *b, ClipVtx *out) {
    f32 t;

    t = (a->z + a->y) / ((a->y - b->y) + (a->z - b->z));
    out->x = a->x + (b->x - a->x) * t;
    out->y = a->y + (b->y - a->y) * t;
    out->z = -out->y;
    out->modelX = a->modelX + (b->modelX - a->modelX) * t;
    out->modelY = a->modelY + (b->modelY - a->modelY) * t;
    out->modelZ = a->modelZ + (b->modelZ - a->modelZ) * t;
    return out + 1;
}

s32 func_151857B0(ClipVtx *v) {
    return v->y < -v->z;
}

/* Plane x == z. */
ClipVtx *func_151857DC(ClipVtx *a, ClipVtx *b, ClipVtx *out) {
    f32 t;

    t = (a->z - a->x) / ((b->x - a->x) - (b->z - a->z));
    out->x = a->x + (b->x - a->x) * t;
    out->y = a->y + (b->y - a->y) * t;
    out->z = out->x;
    out->modelX = a->modelX + (b->modelX - a->modelX) * t;
    out->modelY = a->modelY + (b->modelY - a->modelY) * t;
    out->modelZ = a->modelZ + (b->modelZ - a->modelZ) * t;
    return out + 1;
}

s32 func_151858D4(ClipVtx *v) {
    return v->z < v->x;
}

/* Near plane z == D_800D3688. */
ClipVtx *func_151858FC(ClipVtx *a, ClipVtx *b, ClipVtx *out) {
    f32 t;

    t = (D_800D3688 - a->z) / (b->z - a->z);
    out->x = a->x + (b->x - a->x) * t;
    out->y = a->y + (b->y - a->y) * t;
    out->z = D_800D3688;
    out->modelX = a->modelX + (b->modelX - a->modelX) * t;
    out->modelY = a->modelY + (b->modelY - a->modelY) * t;
    out->modelZ = a->modelZ + (b->modelZ - a->modelZ) * t;
    return out + 1;
}

s32 func_151859FC(ClipVtx *v) {
    return D_800D3688 < v->z;
}

/* Far plane z == D_800D368C. */
ClipVtx *func_15185A28(ClipVtx *a, ClipVtx *b, ClipVtx *out) {
    f32 t;

    t = (D_800D368C - a->z) / (b->z - a->z);
    out->x = a->x + (b->x - a->x) * t;
    out->y = a->y + (b->y - a->y) * t;
    out->z = D_800D368C;
    out->modelX = a->modelX + (b->modelX - a->modelX) * t;
    out->modelY = a->modelY + (b->modelY - a->modelY) * t;
    out->modelZ = a->modelZ + (b->modelZ - a->modelZ) * t;
    return out + 1;
}

s32 func_15185B28(ClipVtx *v) {
    return v->z < D_800D368C;
}

/* Plane x == 0 (splits the shadow into quadrants). */
ClipVtx *func_15185B54(ClipVtx *a, ClipVtx *b, ClipVtx *out) {
    f32 t;

    t = a->x / (a->x - b->x);
    out->x = 0.0f;
    out->y = a->y + (b->y - a->y) * t;
    out->z = a->z + (b->z - a->z) * t;
    out->modelX = a->modelX + (b->modelX - a->modelX) * t;
    out->modelY = a->modelY + (b->modelY - a->modelY) * t;
    out->modelZ = a->modelZ + (b->modelZ - a->modelZ) * t;
    return out + 1;
}

s32 func_15185C44(ClipVtx *v) {
    return v->x <= 0.0f;
}

/* Plane y == 0. */
ClipVtx *func_15185C6C(ClipVtx *a, ClipVtx *b, ClipVtx *out) {
    f32 t;

    t = a->y / (a->y - b->y);
    out->y = 0.0f;
    out->x = a->x + (b->x - a->x) * t;
    out->z = a->z + (b->z - a->z) * t;
    out->modelX = a->modelX + (b->modelX - a->modelX) * t;
    out->modelY = a->modelY + (b->modelY - a->modelY) * t;
    out->modelZ = a->modelZ + (b->modelZ - a->modelZ) * t;
    return out + 1;
}

s32 func_15185D5C(ClipVtx *v) {
    return v->y <= 0.0f;
}

s32 func_15185D84(ClipVtx *v) {
    return v->x > 0.0f;
}

s32 func_15185DAC(ClipVtx *v) {
    return v->y > 0.0f;
}

/*
 * One Sutherland-Hodgman pass: clips the polygon in[0..count-1] against clip plane `plane`, writing the
 * resulting vertices to out. Returns the end of the output.
 */
ClipVtx *func_15185DD4(ClipVtx *in, s32 count, s32 plane, ClipVtx *out) {
    ClipVtx *cur;
    ClipVtx *prev;
    ClipVtx *(*intersect)(ClipVtx *, ClipVtx *, ClipVtx *);
    s32 (*inside)(ClipVtx *);
    s32 i;

    prev = &in[count - 1];
    intersect = D_8008D498[plane];
    inside = D_8008D4C0[plane];
    cur = in;
    for (i = 0; i < count; i++) {
        if (inside(cur)) {
            if (inside(prev)) {
                *out++ = *cur;
            } else {
                out = intersect(prev, cur, out);
                *out++ = *cur;
            }
        } else if (inside(prev)) {
            out = intersect(prev, cur, out);
        }
        prev = cur;
        cur++;
    }
    return out;
}

/*
 * Clips polygon in[0..count-1] to the shadow frustum and emits it as textured vertices in out, with the
 * shadow texture coordinates and fade alpha. When split is set, the result is also cut at x == 0 and
 * y == 0 into four quadrants, whose vertex ranges end at quadEnd[1..4] (quadEnd[0] = 0). Returns the
 * number of vertices written.
 */
s32 func_15185F24(ShadowVtx *in, s32 count, s32 maxVerts, Vtx *out, s32 *quadEnd, s32 split) {
    ClipVtx buf[2][26];
    s32 i;
    s32 j;
    ClipVtx *src;
    ClipVtx *dst;
    f32 scale;
    s16 *model;
    s32 halfEnd[3];
    s32 *p;

    src = (ClipVtx *)in;
    dst = buf[0];
    for (i = 0; i < count; i++) {
        model = in[i].model;
        *dst = *src;
        dst->modelX = model[0];
        dst->modelY = model[1];
        dst->modelZ = model[2];
        dst++;
        src++;
    }
    // matching: vertex counts come from unsigned byte differences, not pointer subtraction
    for (i = 0; i < 6; i++) {
        count = ((u32)func_15185DD4(buf[i & 1], count, i, buf[(i & 1) ^ 1]) - (u32)buf[(i & 1) ^ 1]) / sizeof(ClipVtx);
    }
    halfEnd[0] = 0;
    quadEnd[0] = 0;
    if (split != 0) {
        for (j = 0; j < 2; j++) {
            halfEnd[j + 1] = ((u32)func_15185DD4(buf[i & 1], count, j + 6, &buf[(i & 1) ^ 1][halfEnd[j]]) - (u32)buf[(i & 1) ^ 1]) / sizeof(ClipVtx);
        }
        i++;
        for (j = 0, p = quadEnd; j < 4;) {
            count = halfEnd[1] - halfEnd[0];
            *p = ((u32)func_15185DD4(&buf[i & 1][halfEnd[0]], count, (j++ >> 1) + 8, &buf[(i & 1) ^ 1][*p++]) - (u32)buf[(i & 1) ^ 1]) / sizeof(ClipVtx);
            count = halfEnd[2] - halfEnd[1];
            *p = ((u32)func_15185DD4(&buf[i & 1][halfEnd[1]], count, (j++ >> 1) + 8, &buf[(i & 1) ^ 1][*p++]) - (u32)buf[(i & 1) ^ 1]) / sizeof(ClipVtx);
        }
        count = *p;
        i++;
    } else {
        quadEnd[1] = quadEnd[2] = quadEnd[3] = quadEnd[4] = count;
    }
    dst = buf[i & 1];
    for (i = 0; i < count; i++) {
        out[i].v.ob[0] = dst->modelX;
        out[i].v.ob[1] = dst->modelY;
        out[i].v.ob[2] = dst->modelZ;
        scale = 1020.0f / dst->z;
        dst->x *= scale;
        dst->y *= scale;
        out[i].v.tc[0] = 1024.0f - dst->x;
        out[i].v.tc[1] = dst->y + 1024.0f;
        out[i].v.cn[0] = 0;
        out[i].v.cn[1] = 0;
        out[i].v.cn[2] = 0;
        out[i].v.cn[3] = (u32)(D_800DE02C - ((dst->z - D_800D368C) / (D_800D3688 - D_800D368C)) * D_800DE02C);
        out[i].v.flag = 0;
        dst++;
    }
    return count;
}

extern u32 *D_800D3694;
extern MeshTri *D_800D3690;

/*
 * Gathers the shadow-receiving faces within `radius` of (x, z): decodes each nearby cell's delta-encoded
 * face list into a bitmap (keeping only flagged faces when D_800DE041 is set) and hands it to
 * func_150A5B90 to fill out[] (up to end). Returns the number of vertices written.
 */
s32 func_1518652C(s32 x, s32 z, s32 radius, ShadowVtx *out, f32 *mtx, ShadowVtx *end) {
    ZoneNode *cells[16];
    s32 count;
    u32 bits[0x1760 / 32];
    u32 face;
    u8 *p;
    u32 maxFace;
    s32 c;

    maxFace = 0;
    func_1510F800(0);
    count = func_1510F720(x, z, radius, cells);
    if (count != 0) {
        bzero(bits, sizeof(bits));
        while (count != 0) {
            face = 0;
            count--;
            p = (u8 *)(cells[count] + 1); // the leaf's delta-encoded face list follows its node
            if (D_800DE041 != 0) {
                while (*p != 0) {
                    c = *p;
                    if (c & 0x80) {
                        face = *p++ << 8;
                        face |= *p++;
                        face &= 0x7FFF;
                    } else {
                        face += c;
                        p++;
                    }
                    if (face < 0x1760) {
                        if (D_800D3668[face] == 1) {
                            bits[face >> 5] |= 1 << (face & 0x1F);
                            if (maxFace < face) {
                                maxFace = face;
                            }
                        }
                    }
                }
            } else {
                while (*p != 0) {
                    c = *p;
                    if (c & 0x80) {
                        face = *p++ << 8;
                        face |= *p++;
                        face &= 0x7FFF;
                    } else {
                        face += c;
                        p++;
                    }
                    if (face < 0x1760) {
                        bits[face >> 5] |= 1 << (face & 0x1F);
                        if (maxFace < face) {
                            maxFace = face;
                        }
                    }
                }
            }
        }
        D_800D3690 = D_800DBE3C;
        D_800D3694 = &bits[(maxFace >> 5) + 1];
        D_800DF0E8 = x - radius;
        D_800DF0EC = x + radius;
        D_800DF0F0 = z - radius;
        D_800DF0F4 = z + radius;
        return func_150A5B90(bits, out, mtx, end) - out;
    }
    return 0;
}


/*
 * Rare microcode command 0x10: four triangles sharing vertex c, (c, v-3, v-2) (c, v-2, v-1) (c, v, v+1)
 * (c, v-1, v), packed as 5-bit vertex indices.
 */
#define gSPFan4Triangles(pkt, c, v)                                                                         \
    {                                                                                                     \
        Gfx *_g = (Gfx *)(pkt);                                                                           \
        _g->words.w0 = _SHIFTL(c, 23, 5) | 0x10000000 | _SHIFTL(v - 3, 18, 5) | _SHIFTL((v - 2) >> 2, 15, 3) | \
                       _SHIFTL(c, 10, 5) | _SHIFTL(v - 2, 5, 5) | _SHIFTL(v - 1, 0, 5);                    \
        _g->words.w1 = _SHIFTL((v - 2), 30, 2) | _SHIFTL(c, 25, 5) | _SHIFTL(v, 20, 5) |                   \
                       _SHIFTL(v + 1, 15, 5) | _SHIFTL(c, 10, 5) | _SHIFTL(v - 1, 5, 5) | _SHIFTL(v, 0, 5);  \
    }

/*
 * Draws the real-time shadows for view 0: for each visible object (up to 8 shadows in all), renders its
 * silhouette into the shadow texture for each of its nearest lights, gathers the level geometry around it,
 * and draws that geometry textured with the shadow, clipped to the light frustum. The vertex budget
 * D_8008D3E0 adapts to last frame's use. Drops everything (returns the original gfx) if the display list
 * would overflow.
 */
Gfx *func_15186794(Gfx *gfx, s32 view) {
    ShadowVtx *recv;
    s32 recvCount;
    Vtx *vtx;
    u32 vtxTotal;
    u32 idx;
    s32 split;
    u32 j;
    s32 batch;
    s32 quad;
    s32 nv;
    s32 quadEnd[5];
    s32 i;
    s32 slot;
    s32 base;
    f32 *mtx;
    Gfx *gfxShadows;
    s32 v; // matching: only written, see the object tests below
    s32 modelSub;
    f32 lightDist;
    f32 halfX;
    f32 halfY;
    f32 halfZ;
    Gfx *vtxCmd;
    Gfx *gfxStart;
    s32 modelId;

    slot = 0;
    gfxStart = gfx;
    if (view != 0) {
        return gfx;
    }
    if (D_800DE01C < D_8008D3E0) {
        D_8008D3E0 = D_800DE01C;
        D_800DE028 *= 0.8f;
    } else {
        if (D_800B0DF0->unk1E == 0) {
            D_800B0DF0->unk1E = 400;
        }
        if (D_8008D3E0 < D_800DE020) {
            D_800DE028 += ((f32)(u32)D_800B0DF0->unk1E - D_800DE028) * 0.95f;
        }
        if (D_8008D3E0 < D_800DE024) {
            D_8008D3E0 = D_800DE024;
        }
    }
    vtx = allocate_memory((D_8008D3E0 + 28) * sizeof(Vtx), 3, 1, 1);
    if (vtx == NULL) {
        return gfx;
    }
    recv = allocate_memory(D_8008D3E0 * sizeof(ShadowVtx), 2, 1, 1);
    if (recv == NULL) {
        func_10004074(vtx);
        return gfx;
    }
    memcpy(&D_800DF090[view], &gViews[gCurrentView].viewports[D_800BE9C0], sizeof(Vp));
    D_800DF090[view].vp.vtrans[2] = (s32)(D_800DF090[view].vp.vtrans[2] * (u32)D_800DE030) >> 10;
    gfxShadows = gfx;
    vtxTotal = 0;
    for (i = 0; i < 25; i++) {
        // matching: the dead store to v and the repeated unk74 test (as u32) steer IDO's register allocation
        if (movement_obj[i].type == 0 || movement_obj[i].ob_matrix_ptr == NULL ||
            ((v = movement_obj[i].unk74) & (1 << view)) || movement_obj[i].drawtype != 0 ||
            (movement_obj[i].unk66 & 0x10) != 0x10 ||
            (u32)(movement_obj[i].unk74 & (1 << view)) == (u32)(1 << view) ||
            movement_obj[i].animseq.uh == 0x2D8 ||
            (movement_obj[i].mv != NULL && (movement_obj[i].mv->ladder != 0 || movement_obj[i].mv->rope != 0)) ||
            (D_800DF0E0 & (1 << i))) {
            continue;
        }
        if (movement_obj[i].attach_char != 0 && movement_obj[i].character_type != 0x62) {
            continue;
        }
        if (movement_obj[i].lodCount != 0) {
            modelId = func_150849CC(&movement_obj[i], &modelSub);
            model_loadForActor(&movement_obj[i], modelId, modelSub, 0);
        } else {
            modelId = movement_obj[i].character_type;
            modelSub = 0;
        }
        if (D_800C48F0[modelId] == NULL) {
            continue;
        }
        if (func_15184DF0(i, view, &halfX, &halfY, &halfZ) == 0) {
            continue;
        }
        lightDist = 0.0f;
        for (j = 0; j < D_800DE038 && slot < 8 && vtxTotal < D_8008D3E0; j++) {
            lightDist = func_15184368(i, slot, halfX, halfY, halfZ, lightDist);
            if (lightDist == 0.0f) {
                break;
            }
            mtx = &D_800DF0F8[slot][0][0];
            recvCount = func_151873E4(movement_obj[i].xpos, movement_obj[i].zpos, 230, recv, mtx, &recv[D_8008D3E0 - 2],
                                  func_1518652C(movement_obj[i].xpos, movement_obj[i].zpos, 230, recv, mtx, &recv[D_8008D3E0 - 2]));
            if (recvCount != 0) {
                idx = 0;
                gfx = func_15184FA4(gfx, i, slot, view);
                batch = 0;
                if (movement_obj[i].playernumber != 0xFF && movement_obj[i].current_lod < 2) {
                    split = 1;
                } else if (i + 1 == movement_obj[movement_obj[i].owner].attach_char &&
                           movement_obj[movement_obj[i].owner].playernumber != 0xFF && movement_obj[i].character_type == 0x53) {
                    split = 1;
                } else {
                    split = 0;
                }
                gSPDisplayList(gfx++, D_8008D410);
                gSPViewport(gfx++, &D_800DF090[view]);
                gDPSetOtherMode(gfx++, 0x082C3F, 0x5049D8);
                gDPLoadTextureBlock(gfx++, D_800DF088, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_CLAMP, G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
                vtxCmd = gfx++;
                for (idx = 0; idx < recvCount && vtxTotal < D_8008D3E0; idx++) {
                    if ((s32)idx % 3 == 2) {
                        func_15185F24(&recv[idx - 2], 3, 0x1A, vtx, quadEnd, split);
                        for (quad = 0; quad < 4; quad++) {
                            nv = quadEnd[quad + 1] - quadEnd[quad];
                            batch += nv;
                            vtx += nv;
                            vtxTotal += nv;
                            if (batch > 16) {
                                gSPVertex(vtxCmd, &vtx[-batch], batch - nv, 0);
                                vtxCmd = gfx++;
                                batch = nv;
                            }
                            base = batch - nv;
                            nv -= 2;
                            while (nv > 0) {
                                if (nv >= 4) {
                                    gSPFan4Triangles(gfx++, base, (base + nv));
                                    nv -= 4;
                                } else if (nv >= 2) {
                                    gSP2Triangles(gfx++, base, (base + nv), (base + nv) + 1, 0, base, (base + nv) - 1, (base + nv), 0);
                                    nv -= 2;
                                } else {
                                    gSP1Triangle(gfx++, base, (base + nv), (base + nv) + 1, 0);
                                    nv -= 1;
                                }
                            }
                        }
                    }
                }
                if (batch != 0) {
                    gSPVertex(vtxCmd, &vtx[-batch], batch, 0);
                } else {
                    gfx = vtxCmd;
                }
                gDPPipeSync(gfx++);
                slot++;
            }
        }
    }
    if (slot != 0) {
        gfx = view_emitRspSetup(gfx, view);
        gSPViewport(gfx++, &gViews[gCurrentView].viewports[D_800BE9C0]);
    } else {
        gfx = gfxShadows;
    }
    if (vtxTotal >= D_8008D3E0) {
        D_8008D3E0 = vtxTotal + 48;
    } else {
        D_8008D3E0 = vtxTotal;
    }
    D_800DF0E0 = 0;
    D_800DF0E1 = 0;
    func_10004074(recv);
    // matching: the ?: materialises a 0/1 flag before the branch
    if ((D_800BEBA4 < gfx - gDisplayListBuffers[D_800BE9C0]) ? 1 : 0) {
        return gfxStart;
    }
    return gfx;
}

/* Per-level shadow settings: vertex budget, viewport depth scale, light selection and face filtering. */
void func_151872B0(s32 levelId) {
    D_800DE01C = 600;
    D_800DE020 = 180;
    D_800DE024 = 90;
    D_800DE030 = 1016;
    D_800DE034 = 0;
    D_800DE038 = 1;
    D_800DE03C = 1.0f;
    D_800DE040 = 1;
    D_800DE041 = 1;

    switch (levelId) {
    case 0x21:
        D_800DE01C = 4800;
        D_800DE020 = D_800DE01C - 1;
        D_800DE024 = D_800DE01C - 2;
        D_800DE030 = 1010;
        D_800DE034 = 1;
        D_800DE038 = 4;
        D_800DE040 = 0;
        break;
    case 0x22:
        D_800DE01C = 2400;
        D_800DE020 = D_800DE01C - 1;
        D_800DE024 = D_800DE01C - 2;
        D_800DE038 = 4;
        D_800DE040 = 0;
        break;
    case 0x14:
        D_800DE041 = 0;
        break;
    case 0x29:
        D_800DE01C = 1200;
        D_800DE020 = 600;
        D_800DE024 = 300;
        break;
    }
}

/*
 * Appends to out[count..] the shadow-receiving geometry of every db_Object_array entry whose bounds overlap
 * the square of half-size `range` around (x, z). Returns the new vertex count.
 */
s32 func_151873E4(s32 x, s32 z, s32 range, ShadowVtx *out, f32 *mtx, ShadowVtx *end, s32 count) {
    WorldObject *entry;
    s32 found;
    s32 i;
    s32 extent;

    if (gWorldObjectCount == 0) {
        return count;
    }

    func_1510F800(2);

    out += count;
    D_800DF0E8 = x - range;
    D_800DF0EC = x + range;
    D_800DF0F0 = z - range;
    D_800DF0F4 = z + range;

    found = 0;
    for (i = 0; i < gWorldObjectCount; i++) {
        entry = &db_Object_array[i];
        if (entry->inactive != 0) {
            continue;
        }
        if (entry->object_status & 0x60) {
            continue;
        }
        extent = (entry->cullRadiusY < entry->cullRadiusXZ) ? entry->cullRadiusXZ : entry->cullRadiusY;
        if (D_800DF0EC < entry->x - extent) {
            continue;
        }
        if (entry->x + extent < D_800DF0E8) {
            continue;
        }
        if (D_800DF0F4 < entry->z - extent) {
            continue;
        }
        if (entry->z + extent < D_800DF0F0) {
            continue;
        }
        D_800D37E0[found] = i;
        found++;
    }

    if (found != 0) {
        func_150A44F0(found, D_800D37E0, 0);
        D_800D3690 = D_800DBE3C;
        D_800DF0E4 = found;
        count += func_150A5E44(D_800D37E0, out, mtx, end) - out;
    }
    return count;
}
