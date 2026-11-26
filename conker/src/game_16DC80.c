#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define VIEWS ((RenderView *)gViews)

/*
 * Glow sprites: camera-facing quads whose size and alpha follow how directly they face the viewer, faded
 * out when the z-buffer shows them hidden.
 */

/* matching: the depth terms of a RenderView's viewports[0], read as scalars (the Vp_t array access allocates
 * FP registers differently in func_151408A4). */
typedef struct {
    /* 0x000 */ u8 pad0[0x44];
    /* 0x044 */ s16 vpScaleZ; /* viewports[0].vp.vscale[2] */
    /* 0x046 */ u8 pad46[6];
    /* 0x04C */ s16 vpTransZ; /* viewports[0].vp.vtrans[2] */
} GlowViewDepth;


/* This file's .bss. */
s32 D_800DC9F0; /* live glow count */

/* not referenced: two sets of five words */
static const u32 D_800A51D0[10] = {
    0x00FF00FF, 0x00FF00FF, 0x000000FF, 0x00FF00FF, 0x00FF0000,
    0x00FF00FF, 0x00FF00FF, 0x000000FF, 0x00FF00FF, 0x00FF0000,
};

/*
 * Per-type handler tables of the glows. Each glow type keeps its own object type, so the tables are declared
 * without a prototype (mixed signatures).
 */
/* Visibility tests, indexed by GlowShape.handler: 0 hides the glow this frame. */
static s32 (*D_80089F90[])() = { func_151028AC, sight_isExtensionOn, func_15141818 };
/* Destroy handlers, indexed by GlowShape.type. */
static void (*D_80089F9C[])() = {
    func_151411A4, func_151012D8, func_151411A4, func_151411A4, func_151411A4, func_151411A4, func_151411A4,
    func_151411A4, func_151411A4, func_151411A4, beam_destroyGlow, func_151411A4, func_151411A4, func_151411A4,
    func_151411A4, func_151BE7F8, func_150F6484, func_151411A4,
};
/* Free handlers, indexed by GlowShape.type. */
static void (*D_80089FE4[])() = {
    func_151411C4, func_15101300, func_151411C4, func_151411C4, func_151411C4, func_151411C4, func_151411C4,
    func_151411C4, func_151411C4, func_151411C4, beam_freeGlow, func_151411C4, func_151411C4, func_151411C4,
    func_151411C4, func_151BE824, func_150F64B0, func_151411C4,
};
/* Message handlers, indexed by GlowShape.type. */
typedef void (*GlowMsgFn)();
static GlowMsgFn D_8008A02C[] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, beam_glowMessage, func_1518A360, func_150D7420,
    func_150D74DC, func_150D758C, func_151BE788, func_151BE788, func_150FADC8,
};
static IdList2 D_8008A074 = { { 0x1C, 0x56 } }; /* effect-object kinds */

/*
 * Spawns a glow through func_1513D524 and copies paramSize bytes of params into it.
 * Returns the new glow, or NULL.
 */
void *func_151407D0(void *params, s32 paramSize, ParticleDef *spawn, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s8 handler,
                    u8 arg8, s32 arg9) {
    GlowObj *obj;

    spawn->type = 3;
    spawn->flags |= 0x40400000;
    obj = func_1513D524(spawn, arg3, arg4, arg5, 1, arg6, paramSize, arg8, arg9);
    if (obj != NULL) {
        GlowParams *dst = &obj->params;

        memcpy(dst, params, paramSize);
        dst->shape.handler = handler;
        dst->shape.attach = NULL;
    } else {
        return NULL;
    }
    if (obj != NULL) {
        D_800DC9F0++;
    }
    return obj;
}

/*
 * Builds this frame's quad (4 vertices) for the glow in the given view and updates its visibility.
 * Returns the vertices, or NULL when the glow isn't drawn.
 */
Vtx *func_151408A4(GlowObj *obj, s16 view) {
    Vtx *vtx;
    Vtx *quad;
    GlowParams *params;
    Vec3f toCam;
    f32 dirZ, dirY, dirX;
    f32 cx, cy, cz;
    Vec3f *camPos;
    f32 intensity;
    f32 dist;
    f32 w;
    f32 h;
    s32 pad; // matching: unused local keeps the stack layout
    f32 fade;
    f32 invDist;
    f32 horiz;
    f32 invHoriz;
    f32 invDist2;
    f32 offset;
    f32 upX;
    f32 rightX;
    f32 upY;
    f32 upZ;
    f32 leftZ;
    f32 rightScale;
    f32 screen[2];
    f32 projZ;
    f32 depth;
    f32 invDepth;
    s32 onScreen;
    u8 fresh;
    s16 alpha;

    params = &obj->params;
    if (params->shape.handler != -1 && D_80089F90[params->shape.handler](obj, view) == 0) {
        return NULL;
    }
    params->shape.screenY[view] = -1;
    params->shape.screenX[view] = -1;
    if (obj->flags & 2) {
        onScreen = func_15144CEC(obj->pos, screen, &projZ, &depth, &invDepth, view);
        if (onScreen == 0 || screen[0] < VIEWS[view].left || VIEWS[view].right < screen[0] || screen[1] < VIEWS[view].top || VIEWS[view].bottom < screen[1]) {
            return NULL;
        }
        camPos = func_15144B34(view);
        {
        f32 dz, dy, dx;
        f32 lenSq;
        f32 scale;
        s32 visible;
        s32 zbufDepth;
        GlowViewDepth *vp;
        s32 pad2; // matching: unused local keeps the stack layout
        s16 glowDepth;
        u16 zraw;

        func_151D5D60(obj->vtxBuf, view, sizeof(obj->vtxTemplate), &vtx, &fresh);
        quad = vtx;
        if (vtx != NULL) {
            if (fresh) {
                memcpy(obj->vtxBuf[view], obj->vtxTemplate, sizeof(obj->vtxTemplate));
                memcpy(obj->vtxBuf[view] + 4, obj->vtxTemplate, sizeof(obj->vtxTemplate));
            }
        } else {
            return NULL;
        }
        if (!(obj->flags & 4)) {
            /* Put the aim point 500 units from pos. */
            dx = obj->aim[0] - obj->pos[0];
            dy = obj->aim[1] - obj->pos[1];
            dz = obj->aim[2] - obj->pos[2];
            lenSq = dx * dx + dy * dy + dz * dz;
            if (lenSq == 0.0f) {
                obj->aim[2] = obj->pos[2] + 500.0f;
            } else {
                scale = 500.0f / sqrtf(lenSq);
                obj->aim[0] = obj->pos[0] + dx * scale;
                obj->aim[1] = obj->pos[1] + dy * scale;
                obj->aim[2] = obj->pos[2] + dz * scale;
            }
            obj->flags |= 4;
        }
        toCam.x = camPos->x - obj->pos[0];
        toCam.y = camPos->y - obj->pos[1];
        toCam.z = camPos->z - obj->pos[2];
        dirX = obj->aim[0] - obj->pos[0];
        dirY = obj->aim[1] - obj->pos[1];
        dirZ = obj->aim[2] - obj->pos[2];
        intensity = dirX * toCam.x + dirY * toCam.y + dirZ * toCam.z;
        if (intensity < 0.0f) {
            intensity = 0.0f;
        }
        if (intensity == 0.0f && !(obj->flags & 0x10000)) {
            return NULL;
        }
        if (fabsf(toCam.x) > 0.0001f || fabsf(toCam.z) > 0.0001f) {
            dist = sqrtf(toCam.x * toCam.x + toCam.y * toCam.y + toCam.z * toCam.z);
            fade = 1.0f - params->shape.fadeRate * dist;
            if (fade > 0.0f) {
                zraw = params->shape.zbufSample[view];
                zbufDepth = ((((zraw >> 2) & 0x7FF) << D_80089630[zraw >> 13].shift) + D_80089630[zraw >> 13].base) >> 3;
                visible = TRUE;
                if (obj->flags & 0x10) {
                    if (onScreen != 0) {
                        params->shape.screenX[view] = screen[0];
                        params->shape.screenY[view] = screen[1];
                        // matching: indexing a Vp_t[2] member directly allocates FP registers differently
                        vp = (GlowViewDepth *)((u8 *)gViews + view * sizeof(RenderView) + D_800BE9C0 * sizeof(Vp_t));
                        glowDepth = (vp->vpTransZ + projZ * invDepth * vp->vpScaleZ) * 32.0f;
                    } else {
                        params->shape.screenX[view] = -1;
                        params->shape.screenY[view] = -1;
                        visible = FALSE;
                    }
                    if (glowDepth - zbufDepth >= 31) {
                        visible = FALSE;
                    }
                    if (visible) {
                        s32 pad3; // matching: unused local keeps the stack layout
                        params->shape.visibility[view] += (u16)(0xFF - params->shape.visibility[view]) >> 3;
                    } else {
                        params->shape.visibility[view] -= params->shape.visibility[view] >> 3;
                    }
                } else {
                    params->shape.visibility[view] = 0xFF;
                }
                horiz = sqrtf(toCam.x * toCam.x + toCam.z * toCam.z);
                invHoriz = 1.0f / horiz;
                invDist2 = 1.0f / dist;
                invDist = invDist2;
                /* aim is 500 units out, so this is cos(angle to the viewer) * fade */
                intensity *= fade * invDist * 0.002f;
                offset = params->shape.offset0 + (params->shape.offset1 - params->shape.offset0) * intensity;
                cx = obj->pos[0] + offset * toCam.x;
                cy = obj->pos[1] + offset * toCam.y;
                cz = obj->pos[2] + offset * toCam.z;
                w = (params->shape.width0 + (params->shape.width1 - params->shape.width0) * intensity) * params->shape.scale;
                h = (params->shape.height0 + (params->shape.height1 - params->shape.height0) * intensity) * params->shape.scale;
                /* up = h * (-x * y, horiz^2, -z * y) / (horiz * dist), right = w * (-z, 0, x) / horiz */
                dz = invHoriz * invDist2; // matching: dz reused as the up-vector factor
                dz = h * dz;
                upX = -toCam.x * toCam.y * dz;
                rightScale = w * invHoriz;
                upZ = -toCam.z * toCam.y * dz;
                upY = h * horiz * invDist;
                rightX = -toCam.z * rightScale;
                leftZ = -toCam.x * rightScale;
                vtx->v.ob[0] = upX + rightX + cx;
                vtx->v.ob[1] = upY + cy;
                vtx->v.ob[2] = upZ - leftZ + cz;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = upX - rightX + cx;
                vtx->v.ob[1] = upY + cy;
                vtx->v.ob[2] = upZ + leftZ + cz;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = -upX - rightX + cx;
                vtx->v.ob[1] = cy - upY;
                vtx->v.ob[2] = -upZ + leftZ + cz;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = -upX + rightX + cx;
                vtx->v.ob[1] = cy - upY;
                vtx->v.ob[2] = -upZ - leftZ + cz;
                vtx->v.flag = 0;
                alpha = (params->shape.alpha0 + (params->shape.alpha1 - params->shape.alpha0) * intensity) * 255.0f;
                obj->alpha = (params->shape.visibility[view] * alpha) >> 8;
                if (params->shape.attach != NULL) {
                    params->shape.attach->alpha = (obj->alpha * obj->unk2B) >> 8;
                }
                return quad;
            }
        }
        }
    }
    return NULL;
}

void func_151411A4(DefParticleHead *arg0) {
    func_1513CA6C(arg0);
}

void func_151411C4(DefParticleHead *arg0) {
    func_1513CAA0(arg0);
}

/* Destroys a glow: frees its attachment and runs its type's D_80089F9C handler. */
void func_151411E4(GlowObj *obj) {
    LensFlare **attach = &obj->params.shape.attach; // matching: the NULL test and the call load it separately

    if (*attach != NULL) {
        func_1517E134(*attach);
    }
    D_800DC9F0--;
    D_80089F9C[obj->params.shape.type](obj);
}

/* Destroys a glow: frees its attachment and runs its type's D_80089FE4 handler. */
void func_15141250(GlowObj *obj) {
    LensFlare **attach = &obj->params.shape.attach; // matching: the NULL test and the call load it separately

    if (*attach != NULL) {
        func_1517E134(*attach);
    }
    D_800DC9F0--;
    D_80089FE4[obj->params.shape.type](obj);
}

/* Samples the z-buffer at each occlusion-tested glow's screen position, per view. */
void func_151412BC(void) {
    s32 p;
    u8 i;
    u8 j;
    GlowObj *cur;
    GlowObj *obj;
    GlowParams *params;

    for (p = 0; p < 2; p++) {
        for (i = 0; i < 4; i++) {
            obj = (GlowObj *)D_800DCE50[p][D_800A5168[i]];
            while (obj != NULL) {
                if (obj->flags & 0x2000) {
                    obj->params.shape.zbufSample[3] = 0;
                    obj->params.shape.zbufSample[2] = 0;
                    obj->params.shape.zbufSample[1] = 0;
                    cur = obj; // matching: a second copy of obj steers register allocation
                    obj->params.shape.zbufSample[0] = 0;
                    if (obj->flags & 0x10) {
                        params = &cur->params;
                        for (j = 0; j <= gScreenLayout; j++) {
                            if (params->shape.screenX[j] >= 0 && params->shape.screenX[j] < gScreenWidth && params->shape.screenY[j] >= 0 && params->shape.screenY[j] < gScreenHeight) {
                                params->shape.zbufSample[j] = gZBuffer[params->shape.screenX[j] + params->shape.screenY[j] * gScreenWidth];
                            } else {
                                params->shape.zbufSample[j] = 0x7FFF;
                            }
                        }
                    }
                }
                obj = (GlowObj *)obj->header.next;
            }
        }
    }
}

/* Moves the attachment, if any, to the glow's position. */
void func_1514143C(GlowObj *obj) {
    GlowParams *params;

    if (obj->params.shape.attach != NULL) {
    // matching: these two labels (block boundaries) make IDO reload attach after the test
    attached:
        params = &obj->params;
    copy:
        params->shape.attach->pos[0] = obj->pos[0];
        params->shape.attach->pos[1] = obj->pos[1];
        params->shape.attach->pos[2] = obj->pos[2];
    }
}

/* Flicker: every random interval, picks a new random target scale; the scale eases towards it. */
s32 func_15141478(GlowObj *obj) {
    GlowFlicker *flicker;
    GlowParams *params;

    params = &obj->params;
    flicker = &obj->params.anim.flicker;
    flicker->timer -= gFrameDelta;
    if (flicker->timer < 0.0f) {
        flicker->timer = func_150ADA68() * flicker->period;
        if (func_150ADA20() & 3) {
            flicker->target = func_150ADA68() * (flicker->base - flicker->low) + flicker->low;
        } else {
            flicker->target = func_150ADA68() * (flicker->high - flicker->base) + flicker->base;
        }
    }
    params->shape.scale = params->shape.scale + (flicker->target - params->shape.scale) * flicker->rate;
    return 1;
}

/* Pulse: scale = base + amplitude * sin(phase), the phase advancing by speed per second. */
s32 func_15141564(GlowObj *obj) {
    GlowPulse *pulse = &obj->params.anim.pulse;
    f32 *phase;

    // matching: (*pulse).base rather than pulse->base changes IDO's FP load order
    obj->params.shape.scale = pulse->amplitude * sinf(pulse->phase) + (*pulse).base;
    phase = &pulse->phase;
    *phase = pulse->speed * gFrameDelta + *phase;
    *phase = func_15144B68(*phase);
    return 1;
}

/* Envelope: a periodic cycle of the scale: base, ramp up, peak, ramp down. */
s32 func_151415D4(GlowObj *obj) {
    GlowEnvelope *env = &obj->params.anim.envelope;
    f32 t;

    if (env->time < env->riseStart) {
        obj->params.shape.scale = env->base;
    } else if (env->time < env->riseEnd) {
        f32 s;
        s = (env->time - env->riseStart) * env->rampRate;
        obj->params.shape.scale = env->base + env->range * s;
    } else if (env->time < env->fallStart) {
        obj->params.shape.scale = env->peak;
    } else {
        t = 1.0f - (env->time - env->fallStart) * env->rampRate;
        obj->params.shape.scale = env->base + env->range * t;
    }
    env->time = env->time + gFrameDelta;
    while (env->period < env->time) {
        env->time = env->time - env->period;
    }
    return 1;
}

/* Message handler: forwards to the type's D_8008A02C handler, then handles messages 0x22/0x24/0x25 aimed at
   this glow's type. */
void func_151416E8(GlowObj *obj, u8 *msg, u8 msgType) {
    GlowParams *params;

    // matching: volatile makes IDO reload the type and the callback for the call
    if (((GlowMsgFn volatile *)D_8008A02C)[*(volatile u8 *)&obj->params.shape.type] != NULL) {
        D_8008A02C[*(volatile u8 *)&obj->params.shape.type](obj, msg, msgType);
    }
    params = &obj->params;
    switch (msgType) {
        case 0x22:
        case 0x24:
        case 0x25:
            if (*msg == (*params).shape.type) {
                switch (msgType) {
                    case 0x22:
                        func_1516972C(&obj->header);
                        break;
                    case 0x24:
                        params->shape.handler = -1;
                        break;
                    case 0x25:
                        params->shape.handler = 2;
                        break;
                }
            }
            break;
    }
}


/* Sends func_15169260 the one-byte payload arg0, addressed with the two words of D_8008A074. */
void func_151417C4(u8 arg0, u8 arg1) {
    u8 payload[1];
    IdList2 data;

    data = D_8008A074;
    payload[0] = arg0;
    func_15169260(data.ids, 2, (s32)payload, arg1);
}

s32 func_15141818(s32 arg0, s32 arg1) {
    return 0;
}

/*
 * Places the glow at the point (0, distance, 0) rotated by angles (x, z) about pivot, aimed away from the pivot
 * (spread scales the aim offset).
 */
void func_1514182C(GlowObj *obj, f32 *pivot, f32 distance, f32 spread, f32 angleX, f32 angleZ) {
    f32 dx, dy, dz;
    MtxF mtx;

    func_150A8050(mtx, angleX, 0.0f, angleZ);
    mtx[3][0] = pivot[0];
    mtx[3][1] = pivot[1];
    mtx[3][2] = pivot[2];
    func_150A7960(mtx, 0.0f, distance, 0.0f, &obj->pos[0], &obj->pos[1], &obj->pos[2]);
    dx = (obj->pos[0] - pivot[0]) * spread;
    dy = (obj->pos[1] - pivot[1]) * spread;
    dz = (obj->pos[2] - pivot[2]) * spread;
    obj->aim[0] = obj->pos[0] + dx * 500.0f;
    obj->aim[1] = obj->pos[1] + dy * 500.0f;
    obj->aim[2] = obj->pos[2] + dz * 500.0f;
}

/* Orbit: places the glow around its pivot with func_1514182C. */
s32 func_15141928(GlowObj *obj) {
    f32 *angles = obj->params.anim.orbit.angles;

    func_1514182C(obj, obj->params.anim.orbit.pivot, obj->params.anim.orbit.distance, obj->params.anim.orbit.spread, angles[0], angles[2]);
    return 1;
}
