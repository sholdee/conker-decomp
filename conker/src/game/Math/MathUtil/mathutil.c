#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Defined in game/Effects/ObjectList/objectlist.c as void effect_broadcastEvent(s32 event, u8 eventType). This file was compiled without
   a prototype: func_15146508 passes the two actors after the message as well. */
void effect_broadcastEvent();

/* This file's .data: nine palettes of four RGB colours (math_pickPaletteColour). */
static u8 gColourPalettes[9 * 12] = {
    0x83, 0x94, 0xAA, 0x50, 0x5E, 0x76, 0x28, 0x34, 0x40, 0x1A, 0x24, 0x30,
    0xFF, 0xFF, 0xDE, 0xCA, 0xAA, 0x7A, 0xE9, 0xD4, 0x9F, 0xD5, 0xBC, 0x8A,
    0xFF, 0xFF, 0x00, 0xF0, 0xDC, 0x00, 0xFF, 0xDC, 0x14, 0xF5, 0xEB, 0x0A,
    0xDC, 0xDC, 0xDC, 0xBE, 0xBE, 0xBE, 0xA0, 0xA0, 0xA0, 0x82, 0x82, 0x82,
    0x6E, 0x6E, 0x6E, 0x50, 0x50, 0x50, 0x3C, 0x3C, 0x3C, 0x28, 0x28, 0x28,
    0x96, 0x96, 0xFF, 0x64, 0x64, 0xFF, 0x32, 0x32, 0xFF, 0x00, 0x00, 0xFF,
    0xFF, 0x96, 0x96, 0xFF, 0x64, 0x64, 0xFF, 0x32, 0x32, 0xFF, 0x00, 0x00,
    0x96, 0xFF, 0x96, 0x64, 0xFF, 0x64, 0x32, 0xFF, 0x32, 0x00, 0xFF, 0x00,
    0xFF, 0xFF, 0xFF, 0xFA, 0xFA, 0xFA, 0xE6, 0xE6, 0xE6, 0xC8, 0xC8, 0xC8,
};

/* This file's .bss: the debug breadcrumbs of math_transformOffset. */
static s32 D_800DCA00;   /* the step reached */
static void *D_800DCA04; /* the matrix */
static f32 D_800DCA08;   /* the offset */
static f32 D_800DCA0C;
static f32 D_800DCA10;

/* Debug tunable stub: returns the midpoint of min and max (the name and source location are unused). */
s32 math_debugTunableRange(s32 id, const char *name, s32 min, s32 max, const char *file, s32 line) {
    return (max + min) >> 1;
}

/* Debug tunable stub: returns the default value unchanged. */
s32 math_debugTunable(s32 id, const char *name, s32 min, s32 max, s32 value, const char *file, s32 line) {
    return value;
}

/* Returns arg4 unchanged. */
s32 func_151422F8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    return arg4;
}

/* Matrix storage, read as fixed point or as floats depending on gBoneMatricesFlushed. */
typedef union {
    MtxS16 fx;
    MtxF mf;
} MtxView;

/* Copies the translation of bone matrix bones[bone] to out (fixed-point when gBoneMatricesFlushed is set). */
void math_getBoneTranslation(bones, bone, out)
    void *bones;
    s32 bone;
    Vec3f *out;
{
    MtxView *m;

    // matching: a byte offset (not &mtx[index]) gives IDO's base + offset operand order
    m = (MtxView *)((u8 *)bones + bone * sizeof(Mtx));
    if (gBoneMatricesFlushed != 0) {
        // matching: the original recomputes m here; IDO keeps both computations
        m = (MtxView *)((u8 *)bones + bone * sizeof(Mtx));
        {
            f32 scale = 1.0f / 65536.0f;
            out->x = ((f32)(m->fx.intPart[3][0] << 16) + m->fx.fracPart[3][0]) * scale;
            out->y = ((f32)(m->fx.intPart[3][1] << 16) + m->fx.fracPart[3][1]) * scale;
            out->z = ((f32)(m->fx.intPart[3][2] << 16) + m->fx.fracPart[3][2]) * scale;
        }
        return;
    }
    out->x = m->mf[3][0];
    out->y = m->mf[3][1];
    out->z = m->mf[3][2];
}

/* Cosine of a binary angle (256 steps per turn), from the quarter-wave table gCosTable. */
f32 math_cosBinAngleMasked(u8 angle) {
    s32 index;
    s32 a = angle;
    u16 quadrant;

    if (a & 0x40) {
        index = 0x40 - (a & 0x3F);
    } else {
        index = a & 0x3F;
    }
    quadrant = a & 0xC0;
    if ((quadrant == 0) || (quadrant == 0xC0)) {
        return gCosTable[index];
    }
    return -gCosTable[index];
}

/* Resolves an actor unique id (0xFF: `self`) to an actor with a model (ob_matrix_ptr), or NULL. */
Actor *math_findActorById(u8 uniqueId, Actor *self) {
    s32 id;
    Actor *actor;

    id = uniqueId;
    if (id == 0xFF) {
        if (self->ob_matrix_ptr != NULL) {
            return self;
        }
        return NULL;
    }

    if ((self != NULL) && (self->type != 0) && (id == self->unique_id)) {
        if (self->ob_matrix_ptr != NULL) {
            return self;
        }
        return NULL;
    }

    actor = collision_findObjectById(uniqueId);
    if ((actor != NULL) && (actor->ob_matrix_ptr != NULL)) {
        return actor;
    }
    return NULL;
}

/*
 * Builds out = scale * rotation * translation: math_mtxRotateRPYF makes the rotation, rows 0/2 are scaled
 * by scaleXZ and row 1 by scaleY, columns by (sx, sy, sz).
 */
void math_mtxRotScaleAxes(Mtx *out, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 sx, f32 sy, f32 sz,
                   f32 x, f32 y, f32 z) {
    MtxF mf;

    math_mtxRotateRPYF(mf, rotX, rotY, rotZ);
    mf[3][0] = x;
    mf[3][1] = y;
    mf[3][2] = z;
    mf[0][0] *= sx * scaleXZ;
    mf[0][1] *= sy * scaleXZ;
    mf[0][2] *= sz * scaleXZ;
    mf[1][0] *= sx * scaleY;
    mf[1][1] *= sy * scaleY;
    mf[1][2] *= sz * scaleY;
    mf[2][0] *= sx * scaleXZ;
    mf[2][1] *= sy * scaleXZ;
    mf[2][2] *= sz * scaleXZ;
    guMtxF2L(mf, out);
}

/* Builds a matrix at (x, y, z) whose forward axis points at (atX, atY, atZ), scaled per axis. */
void math_mtxLookAtScale(Mtx *out, f32 scaleXZ, f32 scaleY, f32 sx, f32 sy, f32 sz, f32 x, f32 y, f32 z,
                   f32 atX, f32 atY, f32 atZ) {
    MtxF mf;
    f32 inv;
    f32 fx, fy, fz;
    f32 ux, uy, uz;
    f32 rx, rz;

    fx = atX - x;
    fy = atY - y;
    fz = atZ - z;
    inv = 1.0f / sqrtf(fx * fx + fy * fy + fz * fz);
    fx *= inv;
    fy *= inv;
    fz *= inv;
    rx = fz;
    rz = -fx;
    inv = 1.0f / sqrtf(rx * rx + rz * rz);
    rx *= inv;
    rz *= inv;
    ux = fy * rz;
    uy = fz * rx - fx * rz;
    uz = -fy * rx;
    inv = 1.0f / sqrtf(ux * ux + uy * uy + uz * uz);
    mf[0][0] = rx * sx * scaleXZ;
    mf[1][0] = ux * inv * sx * scaleY;
    mf[2][0] = fx * sx * scaleXZ;
    mf[3][0] = x;
    mf[0][1] = 0.0f;
    mf[1][1] = uy * inv * sy * scaleY;
    mf[2][1] = fy * sy * scaleXZ;
    mf[3][1] = y;
    mf[0][2] = rz * sz * scaleXZ;
    mf[1][2] = uz * inv * sz * scaleY;
    mf[2][2] = fz * sz * scaleXZ;
    mf[3][2] = z;
    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
    mf[3][3] = 1.0f;
    guMtxF2L(mf, out);
}

/* Builds a rotation matrix at (x, y, z), rows 0/2 scaled by scaleXZ and row 1 by scaleY. */
void math_mtxRotScaleXZ(Mtx *out, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z) {
    MtxF mf;

    math_mtxRotateRPYF(mf, rotX, rotY, rotZ);
    mf[3][0] = x;
    mf[3][1] = y;
    mf[3][2] = z;
    mf[0][0] *= scaleXZ;
    mf[0][1] *= scaleXZ;
    mf[0][2] *= scaleXZ;
    mf[1][0] *= scaleY;
    mf[1][1] *= scaleY;
    mf[1][2] *= scaleY;
    mf[2][0] *= scaleXZ;
    mf[2][1] *= scaleXZ;
    mf[2][2] *= scaleXZ;
    guMtxF2L(mf, out);
}

/* Float version of math_mtxRotScaleXZ. */
void math_mtxFRotScaleXZ(MtxF mf, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z) {
    math_mtxRotateRPYF(mf, rotX, rotY, rotZ);
    mf[3][0] = x;
    mf[3][1] = y;
    mf[3][2] = z;
    mf[0][0] *= scaleXZ;
    mf[0][1] *= scaleXZ;
    mf[0][2] *= scaleXZ;
    mf[1][0] *= scaleY;
    mf[1][1] *= scaleY;
    mf[1][2] *= scaleY;
    mf[2][0] *= scaleXZ;
    mf[2][1] *= scaleXZ;
    mf[2][2] *= scaleXZ;
}

/* Picks one of the four RGB colours of palette `palette` at random. */
void math_pickPaletteColour(u8 palette, u8 *r, u8 *g, u8 *b) {
    u8 *color;

    color = &gColourPalettes[(math_random() & 3) * 3 + palette * 12];
    *r = color[0];
    *g = color[1];
    *b = color[2];
}

typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s16 unk3C;
} Unk2D0View;

s32 func_15142A5C(Actor *actor) {
    Unk2D0View *unk2D0 = (Unk2D0View *)actor->unk2D0;
    s32 ret = 0;

    if (unk2D0->unk3C > 0) {
        return 1;
    }
    return ret;
}

/* The zero vector. */
const Vec3f gZeroVec3f = { 0.0f, 0.0f, 0.0f };

/* Falloff curve sampled at 100 steps, read by math_circleHalfChord; close to sqrt(1 - (i / 100)^2). */
const f32 gUnitCircleTable[102] = {
    1.0f, 0.99995f, 0.9998f, 0.99955f, 0.9992f, 0.998749f, 0.998198f, 0.997547f,
    0.996795f, 0.995942f, 0.994987f, 0.993932f, 0.992774f, 0.991514f, 0.990152f, 0.988686f,
    0.987117f, 0.985444f, 0.983667f, 0.981784f, 0.979796f, 0.977701f, 0.9755f, 0.973191f,
    0.970773f, 0.968246f, 0.965609f, 0.96286f, 0.96f, 0.957027f, 0.953939f, 0.950737f,
    0.947418f, 0.943981f, 0.940425f, 0.93675f, 0.932952f, 0.929032f, 0.924987f, 0.920815f,
    0.916515f, 0.912086f, 0.907524f, 0.902829f, 0.897998f, 0.893029f, 0.887919f, 0.882667f,
    0.877269f, 0.871723f, 0.866026f, 0.860175f, 0.854166f, 0.847998f, 0.841665f, 0.835165f,
    0.828493f, 0.821645f, 0.814617f, 0.807403f, 0.8f, 0.792402f, 0.784602f, 0.776595f,
    0.768375f, 0.759934f, 0.751266f, 0.742361f, 0.733212f, 0.72381f, 0.714143f, 0.704202f,
    0.693974f, 0.683448f, 0.672607f, 0.661438f, 0.649924f, 0.638044f, 0.62578f, 0.613107f,
    0.600001f, 0.58643f, 0.572364f, 0.557764f, 0.542587f, 0.526784f, 0.510295f, 0.493053f,
    0.474975f, 0.455962f, 0.435891f, 0.41461f, 0.39192f, 0.367561f, 0.341176f, 0.312252f,
    0.280002f, 0.243107f, 0.199001f, 0.141072f, 0.001145f, 0.0f,
};

/* Cubic Lagrange interpolation weights for samples at -1, 0, 1, 2 (t in [0, 1]). */
f32 math_lagrangeWeight0(f32 t) {
    return ((1.0f - t) * (t - 2.0f) * t) * (1.0f / 6.0f);
}
f32 math_lagrangeWeight1(f32 t) {
    return ((t + 1.0f) * (t - 1.0f) * (t - 2.0f)) * 0.5f;
}
f32 math_lagrangeWeight2(f32 t) {
    return ((2.0f - t) * (t + 1.0f) * t) * 0.5f;
}
f32 math_lagrangeWeight3(f32 t) {
    return ((t + 1.0f) * (t - 1.0f) * t) * (1.0f / 6.0f);
}
/* Emits geometry-mode changes, skipping bits already in the cached state. */
Gfx *math_gfxUpdateGeometryMode(Gfx *gfx, s32 setBits, s32 clearBits) {
    if (~gCachedGeomClearBits & clearBits) {
        gSPClearGeometryMode(gfx++, clearBits);
        gCachedGeomClearBits |= clearBits;
    }

    if (~gCachedGeomSetBits & setBits) {
        gSPSetGeometryMode(gfx++, setBits);
        gCachedGeomSetBits |= setBits;
    }

    return gfx;
}

/* Sets the env colour unless it is already current; emits a pipe sync first if *needSync is 1. */
Gfx *math_gfxSetEnvColor(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *needSync) {
    if ((r != gCachedEnvR) || (g != gCachedEnvG) || (b != gCachedEnvB) || (a != gCachedEnvA)) {
        if (*needSync == 1) {
            gDPPipeSync(gfx++);
            *needSync = 0;
        }

        gDPSetEnvColor(gfx++, r, g, b, a);

        gCachedEnvR = r;
        gCachedEnvG = g;
        gCachedEnvB = b;
        gCachedEnvA = a;
    }

    return gfx;
}

/* Sets the prim colour unless it is already current; emits a pipe sync first if *needSync is 1. */
Gfx *math_gfxSetPrimColor(Gfx *gfx, s32 minLevel, s32 lodFrac, s32 r, s32 g, s32 b, s32 a, u8 *needSync) {
    if ((minLevel != gCachedPrimMinLevel) || (lodFrac != gCachedPrimLodFrac) || (r != gCachedPrimR) ||
        (g != gCachedPrimG) || (b != gCachedPrimB) || (a != gCachedPrimA)) {
        if (*needSync == 1) {
            gDPPipeSync(gfx++);
            *needSync = 0;
        }

        gDPSetPrimColor(gfx++, minLevel, lodFrac, r, g, b, a);

        gCachedPrimMinLevel = minLevel;
        gCachedPrimLodFrac = lodFrac;
        gCachedPrimR = r;
        gCachedPrimG = g;
        gCachedPrimB = b;
        gCachedPrimA = a;
    }

    return gfx;
}

/* Loads frame `frame >> 16` of `desc` through sprite_loadDescFrameAt unless the same texture (math_getTextureFrameId of
 * `texture` and `mode`) is already loaded with the same clamp mode and origin (cached state). */
Gfx *math_gfxLoadTextureFrame(Gfx *gfx, TextureDesc *desc, s32 frame, s32 cm, s32 s, s32 t, s32 texture, u8 mode,
                   SpriteParams *sprite, u8 *needSync, s32 assetArg) {
    s32 id;

    id = math_getTextureFrameId(desc, texture, frame >> 16, mode);
    if ((id != gCachedTextureId) || (cm != gCachedTextureCm) || (s != gCachedTextureS) || (t != gCachedTextureT) ||
        (sprite != gCachedTextureSprite)) {
        if (*needSync == 1) {
            *needSync = 0;
        }
        if ((level == 0x18) || (level == 0x13) || (level == 6) || (level == 0x3B) ||
            (level == 2) || (gIsMultiplayer != 0)) {
            assetArg = 3;
        }
        gfx = sprite_loadDescFrameAt(gfx, desc, frame >> 8, sprite, 0, 0, 0, cm, s, t, assetArg);
        gCachedTextureId = id;
        gCachedTextureCm = cm;
        gCachedTextureS = s;
        gCachedTextureT = t;
        gCachedTextureSprite = gCachedTextureSprite; /* sic: sprite is never cached */
    }
    return gfx;
}

/* Sets the other-mode words unless already current; emits a pipe sync first if *needSync is 1. */
Gfx *math_gfxSetOtherMode(Gfx *gfx, s32 modeH, s32 modeL, u8 *needSync) {
    if ((modeH != gCachedOtherModeH) || (modeL != gCachedOtherModeL)) {
        if (*needSync == 1) {
            gDPPipeSync(gfx++);
            *needSync = 0;
        }

        gDPSetOtherMode(gfx++, modeH | 0xF, modeL);

        gCachedOtherModeH = modeH;
        gCachedOtherModeL = modeL;
    }

    return gfx;
}

s16 func_15143044(u8 arg0, s32 arg1) {
    return 0x7FFF - arg0;
}


/* The asset id of frame `frame` that math_gfxLoadTextureFrame compares with the cached one: by `mode`, a fixed texture, the
 * id `texture` itself or from D_80091564, frame of `desc`, or (the default) frame of gEffectTextures[texture]. */
s32 math_getTextureFrameId(TextureDesc *desc, s32 texture, s32 frame, u8 mode) {
    s32 ret;

    switch (mode) {
        case 4:
            ret = D_800915B0.addr;
            break;
        case 3:
            ret = D_80091514.addr;
            break;
        case 1:
            ret = 0;
            break;
        case 2:
            ret = D_80091564[texture];
            break;
        case 5:
            ret = texture;
            break;
        case 6:
            texture = desc->addr;
            if (desc->addr >= 0x10000000) {
                ret = ((s32 *)desc->addr)[frame];
            } else {
                ret = texture;
            }
            break;
        default:
            ret = ((s32 *)gEffectTextures[texture].addr)[frame];
            break;
    }
    return ret;
}
/*
 * Transforms `offset` by mtx into out (just mtx's translation when offset is NULL or zero).
 * D_800DCA00 records the progress through the function (a debug breadcrumb).
 */
void math_transformOffset(const Vec3f *offset, Vec3f *out, void *mtx) {
    MtxF mf;

    D_800DCA00 = 1;
    if ((offset != NULL) && ((offset->x != 0.0f) || (offset->y != 0.0f) || (offset->z != 0.0f))) {
        D_800DCA00 = 2;
        D_800DCA08 = offset->x;
        D_800DCA0C = offset->y;
        D_800DCA10 = offset->z;
        if (gBoneMatricesFlushed) {
            D_800DCA00 = 3;
            D_800DCA04 = mtx;
            guMtxL2F(mf, mtx);
            math_transformPoint(mf, offset->x, offset->y, offset->z, &out->x, &out->y, &out->z);
            D_800DCA00 = 4;
        // matching: an (unused) label here changes how IDO lays out the branch to the final store
        state4_done:;
        } else {
            D_800DCA00 = 5;
            D_800DCA04 = mtx;
            math_transformPoint(mtx, offset->x, offset->y, offset->z, &out->x, &out->y, &out->z);
            D_800DCA00 = 6;
        }
    } else {
        D_800DCA00 = 7;
        math_getBoneTranslation(mtx, 0, out);
        D_800DCA00 = 8;
    }
    D_800DCA00 = 0;
}

/* Picks a random (x, z) in the region's footprint, with the region's top and bottom heights. */
void math_randomZoneFootprintPoint(TriggerZone *region, f32 *x, f32 *z, f32 *top, f32 *bottom) {
    switch (ZONE_AREA_TYPE(region)) {
    default:
        *x = region->x;
        *z = region->z;
        *top = region->y + region->ySize;
        *bottom = region->y - region->ySize;
        return;
    case ZONE_AREA_BOX:
        /* rotated box */
        {
            u8 angle;
            f32 sine;
            f32 cosine;
            f32 offsetX;
            f32 offsetZ;

            angle = region->yaw * (256.0f / 360.0f);
            sine = math_cosBinAngleMasked(angle - 0x40);
            cosine = math_cosBinAngleMasked(angle);
            offsetX = math_randomFloat() * (2.0f * region->xSize) + -region->xSize;
            offsetZ = math_randomFloat() * (2.0f * region->zSize) + -region->zSize;
            *x = region->x + (offsetX * cosine + offsetZ * sine);
            *z = region->z + (offsetZ * cosine - offsetX * sine);
            *top = region->y + region->ySize;
            *bottom = region->y;
        }
        return;
    case ZONE_AREA_CYLINDER:
        /* disc, from the base up */
        {
            u8 angle;
            f32 sine;
            f32 cosine;
            f32 radius;

            angle = math_random() & 0xFF;
            sine = math_cosBinAngleMasked(angle - 0x40);
            cosine = math_cosBinAngleMasked(angle);
            radius = math_randomFloat() * region->xSize;
            *x = region->x + radius * cosine;
            *z = region->z - radius * sine;
            *top = region->y + region->ySize;
            *bottom = region->y;
        }
        return;
    case ZONE_AREA_SPHERE:
        /* disc, centred vertically */
        {
            u8 angle;
            f32 sine;
            f32 cosine;
            f32 radius;

            angle = math_random() & 0xFF;
            sine = math_cosBinAngleMasked(angle - 0x40);
            cosine = math_cosBinAngleMasked(angle);
            radius = math_randomFloat() * region->xSize;
            *x = region->x + radius * cosine;
            *z = region->z - radius * sine;
            *top = region->y + region->ySize;
            *bottom = region->y - region->ySize;
        }
        return;
    }
}

/* Direction (yaw, pitch in radians) scaled by length, into out[3]. */
void math_dirFromAngles(f32 yaw, f32 pitch, f32 length, Vec3f *out) {
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;
    f32 horizontal;

    cosYaw = cosf(yaw);
    sinYaw = sinf(yaw);
    cosPitch = cosf(pitch);
    sinPitch = sinf(pitch);
    horizontal = length * cosPitch;
    out->x = horizontal * sinYaw;
    out->y = -length * sinPitch;
    out->z = horizontal * cosYaw;
}

/* Planar direction (angle in radians) scaled by length. */
void math_dirFromAngleXZ(f32 angle, f32 length, f32 *x, f32 *z) {
    f32 c;
    f32 s;

    c = cosf(angle);
    s = sinf(angle);
    *x = length * s;
    *z = length * c;
}

/* Binary-angle (256 per turn) version of math_dirFromAngles. */
void math_dirFromBinAngles(s16 yaw, s16 pitch, f32 length, Vec3f *out) {
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;
    u8 quarterYaw;
    u8 quarterPitch;
    f32 horizontal;

    cosYaw = math_cosBinAngleMasked(yaw);
    quarterYaw = yaw - 0x40;
    sinYaw = math_cosBinAngleMasked(quarterYaw);
    cosPitch = math_cosBinAngleMasked(pitch);
    quarterPitch = pitch - 0x40;
    sinPitch = math_cosBinAngleMasked(quarterPitch);
    horizontal = length * cosPitch;
    out->x = horizontal * sinYaw;
    out->y = -length * sinPitch;
    out->z = horizontal * cosYaw;
}

void func_15143834(s16 yaw, s16 pitch, f32 length, Vec3f *out) {
    math_dirFromBinAngles(yaw, pitch, length, out);
}

/* Binary-angle version of math_dirFromAngleXZ. */
void math_dirFromBinAngleXZ(s16 angle, f32 length, f32 *x, f32 *z) {
    f32 c;
    f32 s;
    u8 quarter;

    c = math_cosBinAngleMasked(angle);
    quarter = angle - 0x40;
    s = math_cosBinAngleMasked(quarter);
    *x = length * s;
    *z = length * c;
}

/*
 * Searches zones[start..end) for a record equal to `key` in the fields selected by `fields`.
 * With bit 0x1000 every selected field must match, otherwise any one of them; the last hit wins.
 */
TriggerZone *math_findMatchingZone(s32 start, s32 end, u16 fields, TriggerZone *key) {
    s32 i;
    TriggerZone *result;
    u16 checked;
    u16 matched;

    result = NULL;
    if (key == NULL) {
        return NULL;
    }
    math_clampRange(&start, &end, 0, gZoneCount);
    for (i = start; i < end; i++) {
        checked = 0;
        matched = 0;
        if (fields & 1) {
            if ((key->x == zones[i].x) && (key->y == zones[i].y) && (key->z == zones[i].z)) {
                checked |= 1;
                matched |= 1;
            }
        } else {
            checked |= 1;
        }
        if (fields & 0x2) {
            if ((key->xSize == zones[i].xSize) && (key->ySize == zones[i].ySize) && (key->zSize == zones[i].zSize)) {
                checked |= 0x2;
                matched |= 0x2;
            }
        } else {
            checked |= 0x2;
        }
        if (fields & 0x4) {
            if (key->pitch == zones[i].pitch) {
                checked |= 0x4;
                matched |= 0x4;
            }
        } else {
            checked |= 0x4;
        }
        if (fields & 0x8) {
            if (key->yaw == zones[i].yaw) {
                checked |= 0x8;
                matched |= 0x8;
            }
        } else {
            checked |= 0x8;
        }
        if (fields & 0x10) {
            if (key->inactive == zones[i].inactive) {
                checked |= 0x10;
                matched |= 0x10;
            }
        } else {
            checked |= 0x10;
        }
        if (fields & 0x20) {
            if (key->kind == ZONE_TYPE(&zones[i])) {
                checked |= 0x20;
                matched |= 0x20;
            }
        } else {
            checked |= 0x20;
        }
        if (fields & 0x40) {
            if (key->flags == zones[i].flags) {
                checked |= 0x40;
                matched |= 0x40;
            }
        } else {
            checked |= 0x40;
        }
        if (fields & 0x80) {
            if (key->subTypeData == zones[i].subTypeData) {
                checked |= 0x80;
                matched |= 0x80;
            }
        } else {
            checked |= 0x80;
        }
        if (fields & 0x100) {
            if (key->data1 == zones[i].data1) {
                checked |= 0x100;
                matched |= 0x100;
            }
        } else {
            checked |= 0x100;
        }
        if (fields & 0x200) {
            if (key->data2 == zones[i].data2) {
                checked |= 0x200;
                matched |= 0x200;
            }
        } else {
            checked |= 0x200;
        }
        if (fields & 0x400) {
            if (key->data3 == zones[i].data3) {
                checked |= 0x400;
                matched |= 0x400;
            }
        } else {
            checked |= 0x400;
        }
        if (fields & 0x1000) {
            if (checked == 0x7FF) {
                result = &zones[i];
            }
        } else {
            if (matched != 0) {
                result = &zones[i];
            }
        }
    }
    return result;
}
/* Orders the bounds lo/hi, then orders *start/*end and clamps them into [lo, hi]. XOR swaps. */
void math_clampRange(s32 *start, s32 *end, s32 lo, s32 hi) {
    s32 t;
    s32 t2;

    if (hi < lo) {
        t = lo ^ hi;
        t2 = hi ^ t;
        hi = t2;
        lo = t ^ t2;
    }
    if (*end < *start) {
        t = *end ^ *start;
        *start = t;
        t2 = *end ^ t;
        *end = t2;
        *start = *start ^ t2;
    }
    if (*start < lo) {
        *start = lo;
    }
    if (hi < *end) {
        *end = hi;
    }
}

/* Clamps *value into [lo, hi] (bounds in either order); returns 1 if raised, 2 if lowered, else 0. */
s32 math_clampInt(s32 *value, s32 lo, s32 hi) {
    s32 t;
    s32 t2;

    if (hi < lo) {
        t = lo ^ hi;
        t2 = hi ^ t;
        hi = t2;
        lo = t ^ t2;
    }
    if (*value < lo) {
        *value = lo;
        return 1;
    }
    if (hi < *value) {
        *value = hi;
        return 2;
    }
    return 0;
}

/* The actor's facing angle (unk7A) as a binary angle, a quarter turn ahead. */
s32 func_15143E08(Actor *actor) {
    return ((actor->langle >> 8) + 64) & 0xFF;
}

/* The actor's facing angle in 256ths of a turn, relative to mv's angle when it has one. */
s32 func_15143E24(Actor *actor) {
    ActorCtl *ref = actor->mv;
    if (ref != NULL) {
        return (s16)((actor->langle - ref->pssd_waist_tilt) >> 8);
    }
    return (s16)(actor->langle >> 8);
}
/* Length of a vector. */
f32 math_vec3Length(const Vec3f *v) {
    f32 x = v->x;
    f32 y = v->y;
    f32 z = v->z;

    return sqrtf(x * x + y * y + z * z);
}

/*
 * If any actor is alive and some i passes zone_isActorOutsideId(i, arg1) == 0, plays arg0 through camera_startEnvelope
 * and spawns a short random cont_spawnRumble effect. Returns whether it did.
 */
u8 func_15143E94(s32 arg0, s32 arg1) {
    u8 ret;
    s8 i;
    s8 count;
    u8 found;
    u8 found2;
    RumbleParams spawn;

    ret = 0;
// matching: an unused label here splits the block, changing IDO's scheduling of the setup
start:
    count = gScreenLayout + 1;
    i = 0;
    found = 0;
    while (!found && i < count) {
        if (movement_obj[i].energy != 0) {
            found = 1;
        } else {
            i++;
        }
    }
    i = 0;
    if (found) {
        found2 = 0;
        while (!found2 && i < count) {
            if (zone_isActorOutsideId(i, arg1) == 0) {
                found2 = 1;
            } else {
                i++;
            }
        }
        if (found2) {
            camera_startEnvelope(&camera_global_Cameras[D_800BE9E8], arg0, 1);
            spawn.flags = 1;
            spawn.duration = (math_random() & 0xF) + 0x14;
            spawn.strength = (math_random() & 3) + 4;
            spawn.updateKind = -1;
            spawn.playerMask = 1;
            cont_spawnRumble(&spawn, 0, 0xFF, 0);
            ret = 1;
        }
    }
    return ret;
}

/*
 * Advances a 16.16 position through sequence `id` (gEffectTextures[id].frameCount steps long) by speed * D_800BE9E4.
 * Past either end it stops (1), clamps (2/0x10), bounces (4) or wraps; returns 1 when it stopped.
 */
s32 math_advanceFrameSeq(u8 id, u32 *speed, s32 *pos, u8 flags) {
    s32 max;
    s32 ret;
    s32 n;

    ret = 0;
    n = gEffectTextures[id].frameCount;
    max = (n << 16) - 1;
    *pos += *speed * D_800BE9E4;
    if (max < *pos) {
        if (flags & 1) {
            ret = 1;
        } else if (flags & 2) {
            *speed = 0;
            *pos = max;
        } else if (flags & 4) {
            *pos = max - (*pos % max);
            *speed = -*speed;
        } else {
            do {
                *pos -= max;
            } while (max < *pos);
        }
    } else if ((*pos < 0) && !(flags & 8)) {
        if (flags & 0x10) {
            *speed = 0;
            *pos = 0;
        } else if (flags & 4) {
            *pos = -*pos % max;
            *speed = -*speed;
        } else {
            do {
                *pos += max;
            } while (*pos < 0);
        }
    }
    return ret;
}
/* Colour-combiner style blend: picks the output RGBA from colour 0, colour 1 and `weight` per `mode`. */
void math_pickPrimColour(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, s32 a0, u8 r1, u8 g1, u8 b1, u8 a1, u8 weight, u8 mode) {
    switch (mode) {
    case 2:
        *outR = r1;
        *outG = g1;
        *outB = b1;
        *outA = a1;
        return;
    case 0:
        *outR = *outG = *outB = *outA = 0;
        return;
    case 1:
        *outR = *outG = *outB = 0;
        *outA = a1;
        return;
    case 3:
        *outR = (r0 * weight) >> 8;
        *outG = (g0 * weight) >> 8;
        *outB = (b0 * weight) >> 8;
        *outA = 0;
        return;
    case 4:
        *outR = (r0 * weight) >> 8;
        *outG = (g0 * weight) >> 8;
        *outB = (b0 * weight) >> 8;
        *outA = 0;
        return;
    default:
        *outR = (r0 * weight) >> 8;
        *outG = (g0 * weight) >> 8;
        *outB = (b0 * weight) >> 8;
        *outA = 0;
        return;
    }
}
/* Second-cycle variant of math_pickPrimColour. */
void math_pickEnvColour(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, u8 a1, u8 weight, u8 mode) {
    switch (mode) {
    case 2:
    case 3:
        *outR = r0;
        *outG = g0;
        *outB = b0;
        *outA = a0;
        return;
    case 13:
        *outR = r0;
        *outG = g0;
        *outB = b0;
        *outA = 0;
        return;
    case 0:
        *outR = *outG = *outB = *outA = 0;
        return;
    case 1:
        *outB = weight;
        *outG = weight;
        *outR = weight;
        *outA = 0;
        return;
    case 7:
    case 12:
        *outR = *outG = *outB = 0;
        *outA = a1;
        return;
    case 8:
        *outR = *outG = *outB = 0;
        *outA = a0;
        return;
    case 4:
        *outB = weight;
        *outG = weight;
        *outR = weight;
        *outA = (a0 * a1) >> 8;
        return;
    case 5:
        *outB = weight;
        *outG = weight;
        *outR = weight;
        *outA = (a0 * a1) >> 8;
        return;
    case 6:
        *outR = r0;
        *outG = g0;
        *outB = b0;
        *outA = a1;
        return;
    case 10:
        *outR = r1;
        *outG = g1;
        *outB = b1;
        *outA = a1;
        return;
    case 11:
        *outR = r1;
        *outG = g1;
        *outB = b1;
        *outA = a0;
        return;
    case 9:
        *outB = weight;
        *outG = weight;
        *outR = weight;
        *outA = a1;
        return;
    default:
        *outB = weight;
        *outG = weight;
        *outR = weight;
        *outA = (a0 * a1) >> 8;
        return;
    }
}
/* Wraps value into [lo, hi] (integer steps of hi - lo + 1). */
s32 math_wrapInt(s32 value, s32 hi, s32 lo) {
    s32 range;
    if (hi < value) {
        range = hi - lo + 1;
        do {
            value -= range;
        } while (hi < value);
    }
    if (value < lo) {
        range = hi - lo + 1;
        do {
            value += range;
        } while (value < lo);
    }
    return value;
}

/* Wraps value into [lo, hi]. */
f32 math_wrapFloat(f32 value, f32 hi, f32 lo) {
    f32 range;

    if (hi < value) {
        range = hi - lo;
        do {
            value -= range;
        } while (hi < value);
    }
    if (value < lo) {
        range = hi - lo;
        do {
            value += range;
        } while (value < lo);
    }
    return value;
}

/* Footprint area of a region: 2rx * 2rz for a box, pi r^2 for the round shapes. */
f32 math_getZoneArea(TriggerZone *region) {
    f32 area;
    s16 radius;

    switch (ZONE_AREA_TYPE(region)) {
    default:
        area = 1.0f;
        break;
    case ZONE_AREA_BOX:
        area = region->xSize * region->zSize * 4.0f;
        break;
    case ZONE_AREA_CYLINDER:
    case ZONE_AREA_SPHERE:
        radius = region->xSize;
        area = radius * radius * 3.1415927f;
        break;
    }
    return area;
}

/* Volume of a region: box rx * h * rz, cylinder pi r^2 h, sphere (pi / 3) r^3. */
f32 math_getZoneVolume(TriggerZone *region) {
    f32 volume;
    f32 radius;

    switch (ZONE_AREA_TYPE(region)) {
    default:
        volume = 1.0f;
        break;
    case ZONE_AREA_BOX:
        volume = region->xSize * region->ySize * region->zSize;
        break;
    case ZONE_AREA_CYLINDER:
        volume = region->xSize * region->xSize * 3.1415927f * region->ySize;
        break;
    case ZONE_AREA_SPHERE:
        radius = region->xSize;
        volume = ((radius * 1.0471976f) * radius) * radius;
        break;
    }
    return volume;
}

/* Picks a random point inside the region into out[3], applying its tilt for boxes and cylinders. */
void math_randomPointInZone(TriggerZone *region, Vec3f *out) {
    f32 radiusZ;
    f32 height;
    f32 radiusX;
    f32 sinPitch;
    f32 cosPitch;
    f32 sinYaw;
    f32 cosYaw;
    s32 mode;

    radiusX = region->xSize;
    height = region->ySize;
    radiusZ = region->zSize;
    sinPitch = region->sinPitch;
    cosPitch = region->cosPitch;
    sinYaw = region->sinYaw;
    cosYaw = region->cosYaw;
    mode = ZONE_AREA_TYPE(region);
    switch (mode) {
    default:
        out->x = region->x;
        out->y = region->y;
        out->z = region->z;
        return;
    case ZONE_AREA_BOX:
        {
            f32 y;
            f32 x;
            f32 mixed;
            f32 z;
            f32 savedX;
            f32 rotatedZ;

            x = math_randomFloat() * (2.0f * radiusX) - radiusX;
            y = math_randomFloat() * height;
            z = math_randomFloat() * (2.0f * radiusZ) - radiusZ;
            savedX = x;
            mixed = y * sinPitch + z * cosPitch;
            x = x * cosYaw + mixed * sinYaw;
            y = y * cosPitch - z * sinPitch;
            out->x = region->x + x;
            out->y = region->y + y;
            rotatedZ = mixed * cosYaw - savedX * sinYaw;
            out->z = region->z + rotatedZ;
        }
        return;
    case ZONE_AREA_CYLINDER:
        {
            f32 y;
            f32 x;
            f32 mixed;
            f32 z;
            f32 savedX;
            f32 radius;
            u8 angle;

            radius = math_randomFloat() * radiusX;
            angle = math_random() & 0xFF;
            x = math_cosBinAngleMasked(angle - 0x40) * radius;
            y = math_randomFloat() * height;
            z = math_cosBinAngleMasked(angle) * radius;
            savedX = x;
            mixed = y * sinPitch + z * cosPitch;
            x = x * cosYaw + mixed * sinYaw;
            y = y * cosPitch - z * sinPitch;
            out->x = region->x + x;
            out->y = region->y + y;
            out->z = region->z + (mixed * cosYaw - savedX * sinYaw);
        }
        return;
    case ZONE_AREA_SPHERE:
        {
            Vec3f offset;

            math_dirFromAngles(math_randomFloat() * 6.2831855f, math_randomFloat() * 6.2831855f,
                          math_randomFloat() * radiusX, &offset);
            out->x = region->x + offset.x;
            out->y = region->y + offset.y;
            out->z = region->z + offset.z;
        }
        return;
    }
}
/* Dot product. */
f32 math_vec3Dot(Vec3f *a, Vec3f *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* camera_global_Cameras[index].TrueCameraAngle wrapped into [0, 360] degrees. */
f32 math_getCameraYaw(index)
    s32 index;
{
    f32 v = camera_global_Cameras[index].TrueCameraAngle;
    if (360.0f < v) {
        do {
            v -= 360.0f;
        } while (360.0f < v);
    }
    if (v < 0.0f) {
        do {
            v += 360.0f;
        } while (v < 0.0f);
    }
    return v;
}

/* Position (unk2F8) of camera_global_Cameras[index]. */
Vec3f *math_getCameraPos(s32 index) {
    return &camera_global_Cameras[index].eye;
}

/* Wraps an angle into [0, 2 pi] radians. */
f32 math_wrapRadians(f32 angle) {
    f32 v = angle;
    if (6.2831855f < v) {
        do {
            v -= 6.2831855f;
        } while (6.2831855f < v);
    }
    if (v < 0.0f) {
        do {
            v += 6.2831855f;
        } while (v < 0.0f);
    }
    return v;
}

/* Wraps an angle into [0, 360] degrees. */
f32 math_wrapDegrees(f32 angle) {
    f32 v = angle;
    if (360.0f < v) {
        do {
            v -= 360.0f;
        } while (360.0f < v);
    }
    if (v < 0.0f) {
        do {
            v += 360.0f;
        } while (v < 0.0f);
    }
    return v;
}

/* Wraps a value into [0, 255] in steps of 255. */
s32 math_wrapByte(s16 value) {
    s16 v = value;

    while (v >= 256) {
        v -= 255;
    }
    while (v < 0) {
        v += 255;
    }

    return v;
}

/* Unsigned angular distance between two angles in radians, in [0, pi]. */
f32 math_angleDistance(f32 a, f32 b) {
    f32 diff;

    a = math_wrapRadians(a);
    diff = fabsf(a - math_wrapRadians(b));
    if (3.1415927f < diff) {
        diff = 6.2831855f - diff;
    }
    return diff;
}

/*
 * Projects world point pos through viewport's matrix (gViewProjMatrices) to screen coordinates in screen[2].
 * Returns 0 when the depth is outside (gDefaultNear, 6000). The optional outputs get the projection's
 * remaining terms and 1 / depth.
 */
s32 math_projectToScreen(pos, screen, outA, outDepth, outInvDepth, viewport)
    f32 *pos;
    f32 *screen;
    f32 *outA;
    f32 *outDepth;
    f32 *outInvDepth;
    u8 viewport;
{
    f32 a;
    f32 depth;
    f32 invDepth;
    f32 x;
    f32 y;

    if (outA == NULL) {
        outA = &a;
    }
    if (outDepth == NULL) {
        outDepth = &depth;
    }
    if (outInvDepth == NULL) {
        outInvDepth = &invDepth;
    }
    math_transformPointW(gViewProjMatrices[viewport], pos[0], pos[1], pos[2], screen, screen + 1,
                  outA, outDepth);
    if ((6000.0f <= *outDepth) || (*outDepth <= gDefaultNear)) {
        return 0;
    }
    if (*outDepth != 0.0f) {
        *outInvDepth = 1.0f / *outDepth;
    } else {
        return 0;
    }
    x = gViews[viewport].halfWidth;
    y = gViews[viewport].halfHeight;
    screen[0] = gViews[viewport].centerX + (screen[0] * (x + 5.0f)) * *outInvDepth;
    screen[1] = gViews[viewport].centerY - (screen[1] * (y + 5.0f)) * *outInvDepth;
    return 1;
}

/*
 * Edge vectors of a triangle: normal = (v0 - v1) x (v2 - v1), binormal = edge x normal, edge = v0 - v1.
 * Returns 0 for a degenerate triangle (two equal vertices). `normal` may be NULL.
 */
s32 math_getTriEdges(CollisionTri *tri, Vec3f *binormal, Vec3f *edgeOut, Vec3f *normal) {
    f32 c[3];
    f32 b[3];
    f32 a[3];
    Vec3f normalBuf;
    Vec3f edge0;
    Vec3f edge1;

    if (normal == NULL) {
        normal = &normalBuf;
    }

    a[0] = tri->v[0].x;
    a[1] = tri->v[0].y;
    a[2] = tri->v[0].z;
    b[0] = tri->v[1].x;
    b[1] = tri->v[1].y;
    b[2] = tri->v[1].z;
    c[0] = tri->v[2].x;
    c[1] = tri->v[2].y;
    c[2] = tri->v[2].z;

    if ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2])) {
        return 0;
    }
    if ((a[0] == c[0]) && (a[1] == c[1]) && (a[2] == c[2])) {
        return 0;
    }
    if ((b[0] == c[0]) && (b[1] == c[1]) && (b[2] == c[2])) {
        return 0;
    }

    edge0.x = a[0] - b[0];
    edge0.y = a[1] - b[1];
    edge0.z = a[2] - b[2];
    edge1.x = c[0] - b[0];
    edge1.y = c[1] - b[1];
    edge1.z = c[2] - b[2];

    math_vec3Cross(&edge0, &edge1, normal);
    math_vec3Cross(&edge0, normal, binormal);
    *edgeOut = edge0;
    return 1;
}

/* Cross product out = a x b. */
void math_vec3Cross(Vec3f *a, Vec3f *b, Vec3f *out) {
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

/* Normalizes v into out; returns 0 for a zero vector. length and invLength are optional outputs. */
s32 math_vec3Normalize(Vec3f *v, Vec3f *out, f32 *length, f32 *invLength) {
    f32 sq;
    f32 invBuf;

    if (invLength == NULL) {
        invLength = &invBuf;
    }
    sq = v->x * v->x + v->y * v->y + v->z * v->z;
    if (sq == 0.0f) {
        return 0;
    }
    if (length != NULL) {
        *length = sqrtf(sq);
        *invLength = 1.0f / sqrtf(sq);
    } else {
        *invLength = 1.0f / sqrtf(sq);
    }
    out->x = *invLength * v->x;
    out->y = *invLength * v->y;
    out->z = *invLength * v->z;
    return 1;
}

/* Ray/sphere test (see math_raySphere) that also requires the hit to be closer than maxDist. */
s32 math_raySphereWithin(Vec3f *origin, Vec3f *dir, Vec3f *center, f32 radius, f32 maxDist, Vec3f *hitNear,
                  Vec3f *hitFar, f32 *tNear, f32 *tFar) {
    f32 t;

    if (math_raySphere(origin, dir, center, radius, hitNear, hitFar, tNear, tFar) != 0) {
        t = *tNear;
        if (t < 0.0f) {
            if (*tFar < 0.0f) {
                return 0;
            }
        }
        if (0.0f <= t) {
            if (*tFar < 0.0f) {
                return 1;
            }
        }
        if (t < maxDist) {
            return 1;
        }
        return 0;
    }
    return 0;
}

/*
 * Intersects the ray origin + t * dir (dir normalized) with a sphere. Writes both intersection points
 * and their t; returns 0 on a miss or when the near point is behind the origin.
 */
s32 math_raySphere(Vec3f *origin, Vec3f *dir, Vec3f *center, f32 radius, Vec3f *hitNear,
                  Vec3f *hitFar, f32 *tNear, f32 *tFar) {
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f d;
    Vec3f o;
    f32 proj;
    f32 rr;
    f32 root;
    f32 t0;
    f32 t1;
    f32 dd;

    dx = center->x - origin->x;
    dy = center->y - origin->y;
    dz = center->z - origin->z;
    d = *dir;
    o = *origin;
    dd = (dx * dx) + (dy * dy) + (dz * dz);
    proj = (dx * d.x) + (dy * d.y) + (dz * d.z);
    rr = radius * radius;
    if (rr < dd - (proj * proj)) {
        return 0;
    }
    root = sqrtf(rr - (dd - (proj * proj)));
    if (proj < root) {
        root = -root;
    }
    t0 = proj - root;
    t1 = proj + root;
    hitNear->x = (t0 * d.x) + o.x;
    hitNear->y = (t0 * d.y) + o.y;
    hitNear->z = (t0 * d.z) + o.z;
    *tNear = t0;
    hitFar->x = (t1 * d.x) + o.x;
    hitFar->y = (t1 * d.y) + o.y;
    hitFar->z = (t1 * d.z) + o.z;
    *tFar = t1;
    {
        Vec3f diff;

        diff.x = hitNear->x - origin->x;
        diff.y = hitNear->y - origin->y;
        diff.z = hitNear->z - origin->z;
        if (math_vec3Dot(&diff, dir) < 0.0f) {
            return 0;
        }
        return 1;
    }
}

/* Whether point lies within radius of camera_global_Cameras[index]'s position (unk2F8). */
s32 math_isNearCamera(u8 index, f32 radius, Vec3f *point) {
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f *pos;

    pos = math_getCameraPos(index);
    dx = point->x - pos->x;
    dy = point->y - pos->y;
    dz = point->z - pos->z;

    if ((radius * radius) < ((dx * dx) + (dy * dy) + (dz * dz))) {
        return 0;
    }
    return 1;
}

/* Closest point to `point` on the segment start .. start + dir, into out (t optional). */
void math_closestPointOnSegment(Vec3f *start, Vec3f *dir, Vec3f *point, Vec3f *out, f32 *t) {
    f32 tBuf;

    if (t == NULL) {
        t = &tBuf;
    }
    if (math_closestPointOnLine(start, dir, point, out, t) != 0) {
        if (*t < 0.0f) {
            *out = *start;
        } else if (1.0f < *t) {
            out->x = start->x + dir->x;
            out->y = start->y + dir->y;
            out->z = start->z + dir->z;
        }
    } else {
        *out = *start;
    }
}

/* Closest point to `point` on the line start + t * dir, into out; returns 0 when dir is zero. */
s32 math_closestPointOnLine(Vec3f *start, Vec3f *dir, Vec3f *point, Vec3f *out, f32 *t) {
    f32 tBuf[1];
    f32 denom;
    f32 dot0;
    f32 dot1;

    if (t == NULL) {
        // matching: the fallback slot is 3 words below the buffer (sp+0x10); no declaration places it there
        t = &tBuf[-3];
    }
    denom = (dir->x * dir->x) + (dir->y * dir->y) + (dir->z * dir->z);
    if (denom == 0.0f) {
        return 0;
    }
    dot0 = (dir->x * start->x) + (dir->y * start->y) + (dir->z * start->z);
    dot1 = (dir->x * point->x) + (dir->y * point->y) + (dir->z * point->z);
    *t = (dot1 - dot0) / denom;
    out->x = start->x + (*t * dir->x);
    out->y = start->y + (*t * dir->y);
    out->z = start->z + (*t * dir->z);
    return 1;
}

/*
 * Aim of an actor (or of the vehicle it rides, model 150 with matrix physics): writes the point
 * 2000 units along its yaw/pitch to target and, optionally, unit-1000 direction vectors for the
 * pitch and for the pitch plus pitchOffset (radians).
 */
void math_getActorAim(Actor *actor, Vec3f *target, Vec3f *dir, Vec3f *offsetDir, f32 pitchOffset) {
    s16 yaw;
    s16 pitch;
    volatile f32 angle;
    f32 offsetPitchRad;
    f32 horizontal[1];
    f32 pitchDeg;
    f32 temp;
    f32 yawRad[1];
    f32 pitchRad[1];

    if ((actor->character_type == 150) && (actor->mv->matrix_physics != 0)) {
        yaw = actor->langle + actor->mv->unk80;
    } else if (actor->mv != NULL) {
        yaw = actor->langle - actor->mv->pssd_waist_tilt;
    } else {
        yaw = actor->langle;
    }
    if ((actor->character_type == 150) && (actor->mv->matrix_physics != 0)) {
        pitch = actor->mv->unk82 + 1024;
    } else {
        pitch = actor->unk1D1 * 200;
    }
    // matching: this scope with its unused pad is a uopt block boundary, and the dead store to
    // pitch keeps pitchDeg from being copy-propagated
    {
        s32 pad0;

        pitchDeg = pitch * (360.0f / 65536.0f);
        pitch = 0;
    }
    // matching: the volatile round trip and the pitchDeg/temp copies steer IDO's FP register colouring
    angle = pitchDeg;
    temp = angle;
    pitchDeg = temp;
    temp = temp * 0.01745329238f; /* degrees to radians */
    pitchRad[0] = temp;
    actor_splitVelocity(yaw, 2000.0f, pitchDeg, &target->x, &target->z, &target->y);
    if (dir != NULL) {
        dir->y = cosf(pitchRad[0]) * 1000.0f;
        horizontal[0] = sinf(pitchRad[0]) * 1000.0f;
        yawRad[0] = yaw * 9.587380191e-05f; /* 2 pi / 65536 */
        dir->x = horizontal[0] * cosf(yawRad[0]);
        dir->z = -horizontal[0] * sinf(yawRad[0]);
        if (offsetDir != NULL) {
            offsetPitchRad = pitchRad[0] + pitchOffset;
            offsetDir->y = cosf(offsetPitchRad) * 1000.0f;
            horizontal[0] = sinf(offsetPitchRad) * 1000.0f;
            offsetDir->x = horizontal[0] * cosf(yawRad[0]);
            offsetDir->z = -horizontal[0] * sinf(yawRad[0]);
        }
    }
}

/* Heading (and optionally elevation, from -90) in degrees of a vector; math_atan2 is atan2. */
void math_getHeadingElevation(Vec3f *v, f32 *heading, f32 *elevation) {
    *heading = math_atan2(v->x, v->z) * 57.29577637f;
    if (elevation != NULL) {
        *elevation = (math_atan2(sqrtf(v->x * v->x + v->z * v->z), v->y) * 57.29577637f) - 90.0f;
    }
}

f32 math_circleHalfChord(f32 arg0, f32 scale, f32 arg2) {
    return gUnitCircleTable[(s32)(arg0 * arg2 * 100.0f)] * scale;
}

void func_15145A50(Actor *actor) {
    actor->drawtype = 3;
    if (level != 51) {
        if ((gIsMultiplayer != 0) || (actor->type == 5) || (actor->type == 1) ||
            (actor->type == 21)) {
            actor->type = 5;
            if (actor->mv != NULL) {
                actor->mv->rt_context_b = 0;
            }
        } else {
            func_15053694(actor);
        }
    }
}

/*
 * Ray (start, dir) against the actor's collision ellipsoid (centre/radius from sprite_getActorBounds, y squashed
 * by unkDC); writes the near and far hits, unsquashed by unkE0. Returns 0 on a miss.
 */
s32 math_rayActorEllipsoid(Vec3f *start, Vec3f *dir, Actor *actor, Vec3f *hitNear, Vec3f *hitFar,
                  f32 *radius, f32 *arg6, Vec3f *center) {
    Vec3f centerBuf;
    f32 radiusBuf;
    f32 arg6Buf;
    Vec3f s;
    Vec3f d;
    Vec3f c;

    /* sic: the fallbacks replace the caller's pointers when they are given */
    if (radius != NULL) {
        radius = &radiusBuf;
    }
    if (arg6 != NULL) {
        arg6 = &arg6Buf;
    }
    if (center != NULL) {
        center = &centerBuf;
    }
    sprite_getActorBounds(actor, center, radius, arg6);
    if (*arg6 == 0.0f) {
        return 0;
    }
    if (*radius == 0.0f) {
        return 0;
    }
    {
    f32 length;
    f32 tNear;
    f32 tFar;
    f32 squash;
    f32 unsquash;
    squash = actor->unkDC;
    unsquash = actor->unkE0;
    s.x = start->x;
    s.y = start->y * squash;
    s.z = start->z;
    d.x = dir->x;
    d.y = dir->y * squash;
    d.z = dir->z;
    {
    f32 invLength;
    if (math_vec3Normalize(&d, &d, &length, &invLength) == 0) {
        return 0;
    }
    c.x = center->x;
    c.y = center->y * squash;
    c.z = center->z;
    if (math_raySphereWithin(&s, &d, &c, *radius, length, hitNear, hitFar, &tNear, &tFar) == 0) {
        return 0;
    }
    hitNear->y *= unsquash;
    hitFar->y *= unsquash;
    }
    }
    return 1;
}

/* Whether entry `index` of db_Object_array has viewFlags bit 0x80 (hidden) (true for a negative index). */
u8 math_isObjectHidden(s32 index) {
    if (index < 0) {
        return 1;
    } else {
        return (db_Object_array[index].viewFlags & 0x80) == 0x80;
    }
}

/* Transforms `count` points (given by pointer) by the object's rotation and position. */
void math_transformObjectPointRefs(WorldObject *placement, const f32 **in, f32 **out, s32 count) {
    MtxF mf;
    const f32 **src;
    f32 **dst;
    const f32 *p;
    f32 *q;

    math_mtxRotateRPYF(mf, placement->t, placement->u, placement->v);
    mf[3][0] = placement->x;
    mf[3][1] = placement->y;
    mf[3][2] = placement->z;
    if (count <= 0) {
        return;
    }
    src = in;
    dst = out;
    do {
        p = *src;
        q = *dst;
        math_transformPoint(mf, p[0], p[1], p[2], q, q + 1, q + 2);
        count--;
        src++;
        dst++;
    } while (count > 0);
}

/* Transforms the `count` points in[] by the object's rotation and position into out[]. */
void math_transformObjectPoints(WorldObject *placement, Vec3f *in, Vec3f *out, s32 count) {
    MtxF mf;
    s32 i;

    math_mtxRotateRPYF(mf, placement->t, placement->u, placement->v);
    mf[3][0] = placement->x;
    mf[3][1] = placement->y;
    mf[3][2] = placement->z;
    for (i = 0; count > 0; i++) {
        math_transformPoint(mf, in[i].x, in[i].y, in[i].z, &out[i].x, &out[i].y, &out[i].z);
        count--;
    }
}

/* Array version of math_transformOffset: mfs each offsets[i] by mtx into results[i]. */
void math_transformOffsets(const Vec3f **offsets, Vec3f **results, void *mtx, s32 count) {
    const Vec3f *input;
    MtxF mf;
    Vec3f *output;
    Vec3f **outputs;

    if (gBoneMatricesFlushed != 0) {
        guMtxL2F(mf, mtx);
        if (count > 0) {
            const Vec3f **inputs = offsets;

            outputs = results;
            do {
                input = *inputs;
                if (input != NULL && (input->x != 0.0f || input->y != 0.0f || input->z != 0.0f)) {
                    output = *outputs;
                    math_transformPoint(mf, input->x, input->y, input->z, &output->x, &output->y, &output->z);
                } else {
                    math_getBoneTranslation(mtx, 0, *outputs);
                }
                count--;
                inputs++;
                outputs++;
            } while (count > 0);
        }
    } else {
        if (count > 0) {
            const Vec3f **inputs = offsets;

            outputs = results;
            do {
                input = *inputs;
                if (input != NULL && (input->x != 0.0f || input->y != 0.0f || input->z != 0.0f)) {
                    output = *outputs;
                    math_transformPoint(mtx, input->x, input->y, input->z, &output->x, &output->y, &output->z);
                } else {
                    math_getBoneTranslation(mtx, 0, *outputs);
                }
                count--;
                inputs++;
                outputs++;
            } while (count > 0);
        }
    }
}
/*
 * Builds two unit vectors u, v perpendicular to n (axis-aligned when n lies along an axis).
 * Returns 0 for a zero n.
 */
s32 math_buildPerpendicularBasis(Vec3f *n, Vec3f *u, Vec3f *v) {
    u8 count;
    u8 axis;
    u8 zeroAxis;
    u32 i;
    u32 j;
    u32 k;
    f32 len;
    f32 invLen;

    if ((n->x == 0.0f) && (n->y == 0.0f) && (n->z == 0.0f)) {
        return 0;
    }

    count = 0;
    if (n->x == 0.0f) {
        count++;
        zeroAxis = 0;
    } else {
        axis = 0;
    }
    if (n->y == 0.0f) {
        count++;
        zeroAxis = 1;
    } else {
        axis = 1;
    }
    if (n->z == 0.0f) {
        count++;
        zeroAxis = 2;
    } else {
        axis = 2;
    }

    // matching: double literals (0.0 / 1.0) here; f32 ones change IDO's FP register choice
    if (count == 2) {
        switch (axis) {
        case 0:
            u->x = 0.0;
            u->y = 1.0;
            u->z = 0.0;
            v->x = 0.0;
            v->y = 0.0;
            v->z = 1.0;
            break;
        case 1:
            u->x = 1.0;
            u->y = 0.0;
            u->z = 0.0;
            v->x = 0.0;
            v->y = 0.0;
            v->z = 1.0;
            break;
        case 2:
            u->x = 1.0;
            u->y = 0.0;
            u->z = 0.0;
            v->x = 0.0;
            v->y = 1.0;
            v->z = 0.0;
            break;
        }
        return 1;
    }

    i = 0;
    j = 1;
    k = 2;
    if ((count == 1) && (zeroAxis == 2)) {
        j = 2;
        k = 1;
    }
    ((f32 *)u)[i] = 1.0;
    ((f32 *)u)[j] = 1.0;
    ((f32 *)u)[k] = -((f32 *)n)[i] - (((f32 *)n)[j] / ((f32 *)n)[k]);
    math_vec3Cross(u, n, v);
    math_vec3Cross(v, n, u);
    math_vec3Normalize(u, u, &len, &invLen);
    math_vec3Normalize(v, v, &len, &invLen);
    return 1;
}
/*
 * Draws `slot` of a model: with the owning actor's own textures (mode 1, falling back to the shared
 * ones in mode 2 when the actor is gone or has none), or plainly through light_pickLights.
 */
void *math_emitPointLights(void *gfx, ModelLink *link, u8 mode, Actor *actor, u8 uniqueId, s16 slot, Vec3f *pos,
                    u8 flags, s32 arg8) {
    PointLightGfx *tex;
    Ambient *unk18;

    tex = link->handles[slot];
    if (tex != NULL) {
        unk18 = link->sharedHandle;
        if (unk18 != NULL) {
            if (mode == 1) {
                if (actor != NULL) {
                    if ((actor->type == 0) || (actor->unique_id != uniqueId) ||
                        (actor->character_type == 0xFF) || (actor->unk302 == 0)) {
                        mode = 2;
                    }
                } else {
                    mode = 0;
                }
            }
            switch (mode) {
            case 1:
                gfx = light_reemitLights(gfx, actor->unk304[slot],
                                    actor->unk301, actor->unk302,
                                    actor->unk314);
                break;
            case 2:
                gfx = light_reemitLights(gfx, gViewGfxLights[slot], gViewGfxLightMax, D_800D9E21, &gViewAmbients[slot][D_800BE9C0]);
                break;
            case 0:
            default:
                gfx = light_pickLights(gfx, slot, pos->x, pos->y, pos->z, arg8, tex, link->model,
                                    unk18, &link->unk4, link->unk1C, 0,
                                    ((flags & 1) ? 2 : 0) | (((flags & 2) ? 0x10 : 0) | 8), 0);
                break;
            }
        }
    }
    return gfx;
}

/* Whether no view (0 .. gScreenLayout) can see the zone. */
u8 math_isZoneHiddenFromViews(ZoneNode *zone) {
    s32 i;
    s16 mask;
    s32 ret;

    i = 0;
    mask = 0;
    for (; i <= gScreenLayout; i++) {
        mask |= 1 << i;
    }
    ret = (zone->views & mask) == 0;
    return ret;
}

/* Broadcasts message 45 (0x2D, an actor swap; see effect_timedHandlerMessage) for actors a and b. */
void func_15146508(Actor *a, Actor *b) {
    ActorSwapMsg msg;

    msg.actorA = a;
    msg.actorB = b;
    msg.idA = a->unique_id;
    msg.idB = b->unique_id;
    effect_broadcastEvent(&msg, 45, a, b);
}

/*
 * Transforms `count` offsets by the matrix of a model attachment on the actor: its animation's part
 * matrix, a parent attachment's matrix, a fixed matrix, or else one of the actor's bones. Returns 0
 * if it is unavailable.
 */
s32 math_transformAttachmentOffsets(Actor *actor, ModelAttachment *point, s32 index, const Vec3f **offsets, Vec3f **results,
                  s32 count) {
    s32 i;
    Mtx *mtx;
    ModelAttachment *part;
    Mtx *partMtx;
    Mtx *partBoneMtx;
    MtxF mf;

    if ((actor == NULL) || (point == NULL) || (actor->ob_matrix_ptr == NULL)) {
        return 0;
    }
    if (point->anim != NULL) {
        if (point->anim->unk3F6 == 0) {
            return 0;
        }
        mtx = point->anim->partMtx[D_800BE9C0] + index;
    } else if (point->parentSet) {
        part = attach_find(actor, point->parentSet, 0);
        if (part == NULL) {
            return 0;
        }
        if (attach_getMatrices(part, actor, &partMtx, &partBoneMtx) == 0) {
            return 0;
        }
        if (part->anim != NULL) {
            mtx = &partMtx[point->parentMtx];
        } else {
            mtx = partMtx;
        }
    // matching: an unused label ends this branch's block, changing IDO's branch layout
    end:;
    } else if (point->mtx != NULL) {
        mtx = point->mtx + D_800BE9C0;
    } else {
        math_transformOffsets(offsets, results, &actor->ob_matrix_ptr[point->bone], count);
        return 1;
    }
    if (mtx != NULL) {
        guMtxL2F(mf, mtx);
        for (i = 0; i < count; i++) {
            math_transformPoint(mf, offsets[i]->x, offsets[i]->y, offsets[i]->z, &results[i]->x, &results[i]->y,
                          &results[i]->z);
        }
    } else {
        return 0;
    }
    return 1;
}

/* Whether a position lies inside the world bounds (|x|, |z| <= 19500 and -9500 <= y <= 19500). */
s32 math_isInWorldBounds(Vec3f *pos) {
    f32 limit;
    f32 y;

    limit = 19500.0f;
    if ((limit < fabsf(pos->x)) || (limit < fabsf(pos->z)) || (y = pos->y, (limit < y)) || (y < -9500.0f)) {
        return 0;
    }
    return 1;
}

/*
 * Random drift: when the timer runs out it restarts at a random time below maxTime and picks a new
 * target in [lo, mid) (3 in 4) or [mid, hi); value then eases towards the target by rate.
 */
void math_randomDrift(f32 *timer, f32 maxTime, f32 *target, f32 lo, f32 mid, f32 hi, f32 rate, f32 *value) {
    *timer = *timer - gFrameDelta;
    if (*timer < 0.0f) {
        *timer = math_randomFloat() * maxTime;
        if ((math_random() & 3) != 0) {
            *target = (math_randomFloat() * (mid - lo)) + lo;
        } else {
            *target = (math_randomFloat() * (hi - mid)) + mid;
        }
    }
    *value = ((*target - *value) * rate) + *value;
}
