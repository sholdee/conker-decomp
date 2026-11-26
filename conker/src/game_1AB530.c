#include <ultra64.h>
#include "structs.h"

/*
 * .bss, linked at 0x800DDD60..0x800DDD70 in this order. Defined before conker/flare.h declares them: IDO
 * lays out .bss in first-declaration order, and an extern there would count.
 */
u8 D_800DDD60; /* flare render state has been set up this frame */
u8 D_800DDD61;
LensFlare *D_800DDD64; /* head of the lens flare list */

#include "functions.h"
#include "variables.h"

/* The sun position of the levels whose LevelHeader.unk10 is 1. */
static f32 D_8008CFF0[3] = { -73.0f, 80.0f, -95.0f };

/* Per LevelHeader.unk10: the sun position of the FLARE_SUN flares (NULL: no sun). */
f32 *D_8008CFFC[2] = { NULL, D_8008CFF0 };

/* Sprite sizes, one row per flare type. */
static const u8 D_800A7250[2][7] = {
    { 128, 64, 128, 255, 128, 64, 128 },
    { 32, 64, 32, 32, 32, 32, 32 },
};

/*
 * Lens flares. Each flare has a world position that is projected to the screen every frame; the z-buffer
 * value under that screen point decides whether it is hidden. A visible flare draws a chain of sprites on
 * the line from its screen position through the screen centre.
 */

#define FLARE_SUN       1 /* position comes from D_8008CFFC[D_800B0DF0->unk10] */
#define FLARE_FLASH     2 /* also drives the screen flash level D_800DD2D0 */
#define FLARE_DIRECTED  4 /* depth is only updated while the viewer is inside the cone dir/minFacing */

/* Appends a new flare of the given type and alpha to the end of the flare list. */
LensFlare *func_1517E080(s32 type, s32 alpha) {
    LensFlare *flare;
    LensFlare **link;

    flare = D_800DDD64;
    if (flare != NULL) {
        while (flare->next != NULL) {
            flare = flare->next;
        }
        link = &flare->next;
    } else {
        link = &D_800DDD64;
    }

    flare = allocate_memory(sizeof(LensFlare), 1, 0, 1);
    if (flare == NULL) {
        return NULL;
    }
    *link = flare;
    flare->next = NULL;
    flare->brightness = 0;
    flare->flags = 0;
    flare->screenY = 0.0f;
    flare->screenX = 0.0f;
    flare->type = type;
    flare->alpha = alpha;
    return flare;
}

/* Unlinks a flare from the flare list and frees it. */
void func_1517E134(LensFlare *flare) {
    LensFlare *prev;

    if (flare == D_800DDD64) {
        D_800DDD64 = flare->next;
    } else {
        prev = D_800DDD64;
        while (prev != NULL && prev->next != flare) {
            prev = prev->next;
        }
        if (prev == NULL) {
            return;
        }
        prev->next = flare->next;
    }
    func_10004074(flare);
}

/* Samples the z-buffer (gZBuffer) under each on-screen flare. */
void func_1517E1AC(void) {
    LensFlare *cur;
    f32 x;
    f32 y;

    for (cur = D_800DDD64; cur != NULL; cur = cur->next) {
        x = cur->screenX;
        if (x >= 0.0f && x < D_800BE620) {
            y = cur->screenY;
            if (y >= 0.0f && y < D_800BE624) {
                cur->zbufSample = gZBuffer[(s32)x + (s32)y * D_800BE620];
            }
        }
    }
}

/* Per-type byte tables, indexed by LensFlare.type. */
typedef struct {
    u8 byType[2];
} FlareTypeBytes;

/*
 * Projects every flare, tests it against the z-buffer and draws the visible ones.
 * A strength of -1 fades the flare out.
 */
Gfx *func_1517E28C(Gfx *gfx, s32 arg1) {
    s32 strength;
    s32 hidden;
    s32 zbufDepth;
    LensFlare *flare;
    f32 *sunPos;
    FlareTypeBytes sizeRows = { 0, 1 }; /* row in D_800A7250 */
    FlareTypeBytes counts = { 7, 3 };   /* number of sprites */
    FlareTypeBytes falloffs = { 1, 30 }; /* distance falloff */
    u8 *flash;

    D_800DDD60 = FALSE;
    for (flare = D_800DDD64; flare != NULL; flare = flare->next) {
        if (flare->alpha != 0) {
            hidden = FALSE;
            if (flare->flags & FLARE_SUN) {
                if (D_800DCDD0 == 0) {
                    hidden = TRUE;
                } else {
                    sunPos = D_8008CFFC[D_800B0DF0->unk10];
                    if (func_1517EAAC(sunPos[0], sunPos[1], sunPos[2], &flare->screenX, &flare->screenY) != 1) {
                        hidden = TRUE;
                    }
                    // 0xFFFC is the cleared z-buffer value: nothing is in front of the sun
                    strength = (flare->zbufSample != 0xFFFC) ? -1 : 0xFF;
                }
            } else {
                if (func_1517EC1C(flare, &zbufDepth) != 1) {
                    hidden = TRUE;
                }
                strength = (flare->depth - zbufDepth < 30) ? 0x100 : -1;
            }
            flash = (flare->flags & FLARE_FLASH) ? &D_800DD2D0 : NULL;
            if (!hidden) {
                gfx = func_1517E4A8(gfx, flare, sizeRows.byType[flare->type], counts.byType[flare->type],
                                    falloffs.byType[flare->type], strength, flash);
            } else if (flash != NULL) {
                *flash = 0;
            }
        }
    }
    return gfx;
}


#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

/*
 * Draws one flare: eases the flash level and the flare's brightness towards their targets, then draws
 * `count` sprites stepping from the flare towards (and past) the screen centre.
 */
Gfx *func_1517E4A8(Gfx *gfx, LensFlare *flare, s32 sizeRow, s32 count, s32 falloff, s32 strength, u8 *flash) {
    s32 intensity;
    s32 i;
    f32 dx;
    f32 dy;
    s32 dist;
    s32 target;
    s32 step;
    s32 size;
    f32 x;
    f32 y;
    SpriteParams sprite;

    dx = flare->screenX - D_800BE620 * 0.5f;
    dy = flare->screenY - D_800BE624 * 0.5f;
    if (flash != NULL) {
        if (strength == -1) {
            target = 0;
        } else {
            dist = ((s32)dx * (s32)dx + (s32)dy * (s32)dy) * falloff / 10;
            if (dist < 120) {
                target = (120 - dist) * 176 / 120;
            } else {
                target = 0;
            }
        }
        step = D_800BE9E4 * 5;
        intensity = *flash;
        if (ABS(target - intensity) < step) {
            intensity = target;
        } else if (intensity < target) {
            intensity += step;
        } else {
            intensity -= step;
        }
        *flash = intensity;
    }
    dist = ((s32)dx * (s32)dx + (s32)dy * (s32)dy) * falloff / 10;
    if (flare->screenX < 2.0f || D_800BE620 - 2 <= flare->screenX || flare->screenY < 0.0f ||
        D_800BE624 <= flare->screenY) {
        intensity = 0;
    } else if (strength == -1) {
        intensity = 0;
    } else {
        intensity = (0x1ADB00 / (dist + 10000)) * strength / 256;
    }
    step = D_800BE9E4 * 10;
    if (ABS(intensity - flare->brightness) < step) {
        flare->brightness = intensity;
    } else if (intensity < flare->brightness) {
        flare->brightness = flare->brightness - step;
    } else {
        flare->brightness = flare->brightness + step;
    }
    if (dist < 200000 && flare->brightness > 0 && fabsf(flare->screenX) < 2000.0f &&
        fabsf(flare->screenY) < 2000.0f) {
        D_800D2DA8 = 0xFF;
        D_800D2DA9 = 0xFF;
        D_800D2DAA = 0xFF;
        sprite.tex = &D_800903AC;
        sprite.animSpeed = 0;
        sprite.animTimer = 0;
        sprite.flip = 0;
        if (!D_800DDD60) {
            gfx = func_150950D4(func_1517EA4C(gfx), &D_800903AC, 0, 0, 0, 0, 2, 0x100, 0x100, 3);
            D_800DDD60 = TRUE;
        }
        intensity = flare->alpha;
        intensity = (flare->brightness * intensity) >> 8;
        for (i = 0; i < count; i++) {
            x = flare->screenX - dx * ((i + 1) * 2) / 5.0f;
            y = flare->screenY - dy * ((i + 1) * 2) / 5.0f;
            size = D_800A7250[sizeRow][i];
            y += size / 64.0f;
            sprite.alpha = intensity;
            sprite.scaleX = (size << 12) * D_800380A0 / 255.0f;
            sprite.scaleY = (size << 12) * D_800380A4 / 255.0f;
            D_800D2DAB = 0;
            gfx = func_15095A48(gfx, &sprite, x, y);
            gDPPipeSync(gfx++);
        }
    }
    return gfx;
}

/* Sets up the render state for flare sprites: colour PRIM, alpha PRIM * TEXEL0. */
Gfx *func_1517EA4C(Gfx *gfx) {
    gDPPipeSync(gfx++);
    gDPSetCombineLERP(gfx++, 0, 0, 0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0, 0, 0, 0, PRIMITIVE, PRIMITIVE, 0, TEXEL0,
                      0);
    gDPSetOtherMode(gfx++, 0x2C0F, 0x00504344);
    return gfx;
}

/* matching: RenderView.viewports as Vp_t with scalar fields (indexing Vp_t.vscale[2] changes IDO's FP register order). */
typedef struct {
    /* 0x00 */ s16 scaleX;
    /* 0x02 */ s16 scaleY;
    /* 0x04 */ s16 scaleZ;
    /* 0x06 */ s16 scaleW;
    /* 0x08 */ s16 transX;
    /* 0x0A */ s16 transY;
    /* 0x0C */ s16 transZ;
    /* 0x0E */ s16 transW;
} FlareViewport;

#define VIEWPORTS ((FlareViewport *)D_800BE628->viewports)

/*
 * Projects a world point to the current player's screen. Returns 1 and the screen position when the point
 * is in front of the camera, otherwise 0 with (0, 0).
 */
s32 func_1517EAAC(f32 x, f32 y, f32 z, f32 *outX, f32 *outY) {
    Camera *player;
    f32 pitch;
    f32 yaw;
    f32 roll;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 sw;
    MtxF mtx;
    f32 cx;
    f32 cy;
    f32 scale;

    // matching: IDO adds base + offset in this order only for explicit byte arithmetic, not &camera_global_Cameras[i]
    player = (Camera *)((u8 *)camera_global_Cameras + D_80082FA4 * sizeof(Camera));
    pitch = -player->CameraPitch;
    yaw = -player->TrueCameraAngle;
    roll = player->Roll;
    func_15110360(D_80082FA4, mtx, pitch, yaw, roll);
    cx = D_800BE628[D_80082FA4].halfWidth;
    cy = D_800BE628[D_80082FA4].halfHeight;
    func_150A7A00(mtx, x, y, z, &sx, &sy, &sz, &sw);
    if (sw > 10.0f) {
        scale = 1.0f / sw;
        sx = sx * cx * scale;
        sy = sy * cy * scale;
        *outX = cx + sx;
        *outY = cy - sy;
        return 1;
    }
    *outX = 0.0f;
    *outY = 0.0f;
    return 0;
}


// matching: the f64 member gives the clip z/w pair the 8-byte-aligned stack slot IDO used
typedef union {
    struct {
        f32 w;
        f32 z;
    } f;
    f64 align;
} ClipZW;

/*
 * Projects a flare and computes its z-buffer depth. Stores the decoded z-buffer sample in *outZbufDepth.
 * Returns 0 when func_1509563C rejects the flare (max distance 4000), and 1 otherwise; a FLARE_DIRECTED
 * flare whose cone the viewer is outside of returns 1 early without updating anything.
 */
s32 func_1517EC1C(LensFlare *flare, s32 *outZbufDepth) {
    f32 dx;
    f32 dy;
    f32 dz;
    u16 sample;
    s32 exponent;
    ClipZW clip;

    if (func_1509563C(flare->pos[0], flare->pos[1], flare->pos[2], &flare->screenX, &flare->screenY, &clip.f.z,
                      &clip.f.w, 4000.0f) == 1) {
        if (flare->flags & FLARE_DIRECTED) {
            dx = flare->pos[0] - camera_global_Cameras->eye.x;
            dy = flare->pos[1] - camera_global_Cameras->eye.y;
            dz = flare->pos[2] - camera_global_Cameras->eye.z;

            // matching: the facing cosine reuses dx's register
            dx = (dx * flare->dir[0] + dy * flare->dir[1] + dz * flare->dir[2]) /
                 sqrtf(dx * dx + dy * dy + dz * dz);
            if (dx < flare->minFacing) {
                return 1;
            }
        }

        sample = flare->zbufSample;
        exponent = sample >> 13;
        *outZbufDepth = ((((sample >> 2) & 0x7FF) << D_80089630[exponent].shift) + D_80089630[exponent].base) >> 3;
        // matching: `viewport + i`, not `viewport[i]` (that changes IDO's FP register order)
        flare->depth = (u32)(((VIEWPORTS + D_800BE9C0)->transZ +
                              (clip.f.z / clip.f.w) * (VIEWPORTS + D_800BE9C0)->scaleZ) * 32.0f);
        return 1;
    }
    return 0;
}
