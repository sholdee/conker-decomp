#include <ultra64.h>

#define func_1513D524 func_1513D524_void_proto
#define func_15169260 func_15169260_s32
#include "functions.h"
#undef func_1513D524
#undef func_15169260
#include "variables.h"

// matching: IDO orders the index addu as the original only when indexing through this (no-op) cast
#define VIEWS ((RenderView *)D_800BE628)

/*
 * Glow sprites: camera-facing quads whose size and alpha follow how directly they face the viewer, faded
 * out when the z-buffer shows them hidden.
 */

/* Attached object (freed with func_1517E134): follows the glow's position and takes its alpha. */
typedef struct {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ u8 padC[0x24];
    /* 0x30 */ u8 alpha;
} GlowAttach;

/* func_15141478: random flicker */
typedef struct {
    f32 base;
    f32 low;
    f32 high;
    f32 target;
    f32 timer;
    f32 period;
    f32 rate;
} GlowFlicker;

/* func_15141564: sine pulse */
typedef struct {
    f32 base;
    f32 amplitude;
    f32 phase;
    f32 speed;
} GlowPulse;

/* func_151415D4: periodic rise / hold / fall */
typedef struct {
    f32 peak;
    f32 base;
    f32 range;
    f32 time;
    f32 riseStart;
    f32 riseEnd;
    f32 fallStart;
    f32 period;
    f32 rampRate;
} GlowEnvelope;

/* func_15141928: placed around a pivot */
typedef struct {
    f32 distance;
    f32 spread;
    f32 *angles;
    f32 pivot[3];
} GlowOrbit;

/* Glow parameters: func_151407D0 copies them into the object at +0x110. */
typedef struct {
    /* 0x00 */ f32 width1;   /* width, height and offset when facing the viewer (intensity 1)... */
    /* 0x04 */ f32 height1;
    /* 0x08 */ f32 width0;   /* ...and at intensity 0 */
    /* 0x0C */ f32 height0;
    /* 0x10 */ f32 offset1;
    /* 0x14 */ f32 offset0;
    /* 0x18 */ f32 alpha1;
    /* 0x1C */ f32 alpha0;
    /* 0x20 */ f32 fadeRate; /* intensity lost per unit of distance */
    /* 0x24 */ s32 screenX[4]; /* per view, -1 when off screen */
    /* 0x34 */ s32 screenY[4];
    /* 0x44 */ GlowAttach *attach;
    /* 0x48 */ f32 scale;
    /* 0x4C */ u16 zbufSample[4]; /* z-buffer value at the glow's screen position, per view */
    /* 0x54 */ u8 visibility[4];  /* per view, eases towards 0xFF (visible) or 0 */
    /* 0x58 */ u8 type;           /* index into the D_80089F9C/D_80089FE4/D_8008A02C handler tables */
    /* 0x59 */ s8 handler;        /* index into D_80089F90, or -1 */
    /* 0x5A */ u8 pad5A[6];
    /* 0x60 */ union {
        GlowFlicker flicker;
        GlowPulse pulse;
        GlowEnvelope envelope;
        GlowOrbit orbit;
    } anim;
} GlowParams;

typedef struct GlowObj {
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ struct GlowObj *next;
    /* 0x00C */ u8 padC[0x1F];
    /* 0x02B */ u8 unk2B;
    /* 0x02C */ u8 pad2C[8];
    /* 0x034 */ f32 pos[3];
    /* 0x040 */ f32 aim[3]; /* pos + 500 * facing direction */
    /* 0x04C */ u8 pad4C[0xC];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0x63];
    /* 0x0C0 */ Vtx vtxTemplate[4];
    /* 0x100 */ Vtx *vtxBuf[4]; /* per view, double-buffered (func_151D5D60) */
    /* 0x110 */ GlowParams params;
} GlowObj;

/* Spawn descriptor handed to func_1513D524 (only the fields used here). */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0x3E];
    /* 0x40 */ s32 flags;
} GlowSpawn;

/* matching: the depth terms of a RenderView's viewports[0], read as scalars (the Vp_t array access allocates
 * FP registers differently in func_151408A4). */
typedef struct {
    /* 0x000 */ u8 pad0[0x44];
    /* 0x044 */ s16 vpScaleZ; /* viewports[0].vp.vscale[2] */
    /* 0x046 */ u8 pad46[6];
    /* 0x04C */ s16 vpTransZ; /* viewports[0].vp.vtrans[2] */
} GlowViewDepth;

#define ZBUFFER ((u16 *)D_800BE9C4)

/* z-buffer decompression table, indexed by the exponent (top 3 bits). */
typedef struct {
    s32 shift;
    u32 base;
} ZDecode;

extern void *func_1513D524(s32, u8, u8, u8, u8, u8, s32, u8, s32);
extern s32 D_800DC9F0; /* live glow count */
extern ZDecode D_80089630[];
extern s32 (*D_80089F90[])(GlowObj *, s16);
extern void (*D_80089F9C[])(GlowObj *);
extern void (*D_80089FE4[])(GlowObj *);
typedef void (*GlowMsgFn)(GlowObj *, u8 *, u8);
extern GlowMsgFn D_8008A02C[];
extern GlowObj *D_800DCE50[2][104];
extern struct17 *func_15144B34(s32);
extern s32 func_15144CEC(f32 *, f32 *, f32 *, f32 *, f32 *, s32);
void func_151D5D60(void **, s16, s32, void **, u8 *);
void func_1517E134(void *);
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

/*
 * Spawns a glow through func_1513D524 and copies paramSize bytes of params into it.
 * Returns the new glow, or NULL.
 */
GlowObj *func_151407D0(GlowParams *params, s32 paramSize, GlowSpawn *spawn, u8 arg3, u8 arg4, u8 arg5, u8 arg6,
                       s8 handler, u8 arg8, s32 arg9) {
    GlowObj *obj;

    spawn->unk1 = 3;
    spawn->flags |= 0x40400000;
    obj = func_1513D524((s32)spawn, arg3, arg4, arg5, 1, arg6, paramSize, arg8, arg9);
    if (obj != NULL) {
        GlowParams *dst = &obj->params;

        memcpy(dst, params, paramSize);
        dst->handler = handler;
        dst->attach = NULL;
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
    struct17 toCam;
    f32 dirZ, dirY, dirX;
    f32 cx, cy, cz;
    struct17 *camPos;
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
    if (params->handler != -1 && D_80089F90[params->handler](obj, view) == 0) {
        return NULL;
    }
    params->screenY[view] = -1;
    params->screenX[view] = -1;
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

        func_151D5D60((void **)obj->vtxBuf, view, sizeof(obj->vtxTemplate), (void **)&vtx, &fresh);
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
        toCam.unk0 = camPos->unk0 - obj->pos[0];
        toCam.unk4 = camPos->unk4 - obj->pos[1];
        toCam.unk8 = camPos->unk8 - obj->pos[2];
        dirX = obj->aim[0] - obj->pos[0];
        dirY = obj->aim[1] - obj->pos[1];
        dirZ = obj->aim[2] - obj->pos[2];
        intensity = dirX * toCam.unk0 + dirY * toCam.unk4 + dirZ * toCam.unk8;
        if (intensity < 0.0f) {
            intensity = 0.0f;
        }
        if (intensity == 0.0f && !(obj->flags & 0x10000)) {
            return NULL;
        }
        if (fabsf(toCam.unk0) > 0.0001f || fabsf(toCam.unk8) > 0.0001f) {
            dist = sqrtf(toCam.unk0 * toCam.unk0 + toCam.unk4 * toCam.unk4 + toCam.unk8 * toCam.unk8);
            fade = 1.0f - params->fadeRate * dist;
            if (fade > 0.0f) {
                zraw = params->zbufSample[view];
                zbufDepth = ((((zraw >> 2) & 0x7FF) << D_80089630[zraw >> 13].shift) + D_80089630[zraw >> 13].base) >> 3;
                visible = TRUE;
                if (obj->flags & 0x10) {
                    if (onScreen != 0) {
                        params->screenX[view] = screen[0];
                        params->screenY[view] = screen[1];
                        // matching: indexing a Vp_t[2] member directly allocates FP registers differently
                        vp = (GlowViewDepth *)((u8 *)D_800BE628 + view * sizeof(RenderView) + D_800BE9C0 * sizeof(Vp_t));
                        glowDepth = (vp->vpTransZ + projZ * invDepth * vp->vpScaleZ) * 32.0f;
                    } else {
                        params->screenX[view] = -1;
                        params->screenY[view] = -1;
                        visible = FALSE;
                    }
                    if (glowDepth - zbufDepth >= 31) {
                        visible = FALSE;
                    }
                    if (visible) {
                        s32 pad3; // matching: unused local keeps the stack layout
                        params->visibility[view] += (u16)(0xFF - params->visibility[view]) >> 3;
                    } else {
                        params->visibility[view] -= params->visibility[view] >> 3;
                    }
                } else {
                    params->visibility[view] = 0xFF;
                }
                horiz = sqrtf(toCam.unk0 * toCam.unk0 + toCam.unk8 * toCam.unk8);
                invHoriz = 1.0f / horiz;
                invDist2 = 1.0f / dist;
                invDist = invDist2;
                /* aim is 500 units out, so this is cos(angle to the viewer) * fade */
                intensity *= fade * invDist * 0.002f;
                offset = params->offset0 + (params->offset1 - params->offset0) * intensity;
                cx = obj->pos[0] + offset * toCam.unk0;
                cy = obj->pos[1] + offset * toCam.unk4;
                cz = obj->pos[2] + offset * toCam.unk8;
                w = (params->width0 + (params->width1 - params->width0) * intensity) * params->scale;
                h = (params->height0 + (params->height1 - params->height0) * intensity) * params->scale;
                /* up = h * (-x * y, horiz^2, -z * y) / (horiz * dist), right = w * (-z, 0, x) / horiz */
                dz = invHoriz * invDist2; // matching: dz reused as the up-vector factor
                dz = h * dz;
                upX = -toCam.unk0 * toCam.unk4 * dz;
                rightScale = w * invHoriz;
                upZ = -toCam.unk8 * toCam.unk4 * dz;
                upY = h * horiz * invDist;
                rightX = -toCam.unk8 * rightScale;
                leftZ = -toCam.unk0 * rightScale;
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
                alpha = (params->alpha0 + (params->alpha1 - params->alpha0) * intensity) * 255.0f;
                obj->alpha = (params->visibility[view] * alpha) >> 8;
                if (params->attach != NULL) {
                    params->attach->alpha = (obj->alpha * obj->unk2B) >> 8;
                }
                return quad;
            }
        }
        }
    }
    return NULL;
}

void func_151411A4(struct210 *arg0) {
    func_1513CA6C(arg0);
}

void func_151411C4(struct210 *arg0) {
    func_1513CAA0(arg0);
}

/* Destroys a glow: frees its attachment and runs its type's D_80089F9C handler. */
void func_151411E4(GlowObj *obj) {
    GlowAttach **attach = &obj->params.attach; // matching: the NULL test and the call load it separately

    if (*attach != NULL) {
        func_1517E134(*attach);
    }
    D_800DC9F0--;
    D_80089F9C[obj->params.type](obj);
}

/* Destroys a glow: frees its attachment and runs its type's D_80089FE4 handler. */
void func_15141250(GlowObj *obj) {
    GlowAttach **attach = &obj->params.attach; // matching: the NULL test and the call load it separately

    if (*attach != NULL) {
        func_1517E134(*attach);
    }
    D_800DC9F0--;
    D_80089FE4[obj->params.type](obj);
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
            /* matching: D_800A5168 is declared f32 in variables.h; it is a table of 4 list indices */
            obj = D_800DCE50[p][((s32 *)&D_800A5168)[i]];
            while (obj != NULL) {
                if (obj->flags & 0x2000) {
                    obj->params.zbufSample[3] = 0;
                    obj->params.zbufSample[2] = 0;
                    obj->params.zbufSample[1] = 0;
                    cur = obj; // matching: a second copy of obj steers register allocation
                    obj->params.zbufSample[0] = 0;
                    if (obj->flags & 0x10) {
                        params = &cur->params;
                        for (j = 0; j <= D_80082FA0; j++) {
                            if (params->screenX[j] >= 0 && params->screenX[j] < D_800BE620 && params->screenY[j] >= 0 && params->screenY[j] < D_800BE624) {
                                params->zbufSample[j] = ZBUFFER[params->screenX[j] + params->screenY[j] * D_800BE620];
                            } else {
                                params->zbufSample[j] = 0x7FFF;
                            }
                        }
                    }
                }
                obj = obj->next;
            }
        }
    }
}

/* Moves the attachment, if any, to the glow's position. */
void func_1514143C(GlowObj *obj) {
    GlowParams *params;

    if (obj->params.attach != NULL) {
    // matching: these two labels (block boundaries) make IDO reload attach after the test
    attached:
        params = &obj->params;
    copy:
        params->attach->pos[0] = obj->pos[0];
        params->attach->pos[1] = obj->pos[1];
        params->attach->pos[2] = obj->pos[2];
    }
}

/* Flicker: every random interval, picks a new random target scale; the scale eases towards it. */
s32 func_15141478(GlowObj *obj) {
    GlowFlicker *flicker;
    GlowParams *params;

    params = &obj->params;
    flicker = &obj->params.anim.flicker;
    flicker->timer -= D_800BE9A4;
    if (flicker->timer < 0.0f) {
        flicker->timer = func_150ADA68() * flicker->period;
        if (func_150ADA20() & 3) {
            flicker->target = func_150ADA68() * (flicker->base - flicker->low) + flicker->low;
        } else {
            flicker->target = func_150ADA68() * (flicker->high - flicker->base) + flicker->base;
        }
    }
    params->scale = params->scale + (flicker->target - params->scale) * flicker->rate;
    return 1;
}

/* Pulse: scale = base + amplitude * sin(phase), the phase advancing by speed per second. */
s32 func_15141564(GlowObj *obj) {
    GlowPulse *pulse = &obj->params.anim.pulse;
    f32 *phase;

    // matching: (*pulse).base rather than pulse->base changes IDO's FP load order
    obj->params.scale = pulse->amplitude * sinf(pulse->phase) + (*pulse).base;
    phase = &pulse->phase;
    *phase = pulse->speed * D_800BE9A4 + *phase;
    *phase = func_15144B68(*phase);
    return 1;
}

/* Envelope: a periodic cycle of the scale: base, ramp up, peak, ramp down. */
s32 func_151415D4(GlowObj *obj) {
    GlowEnvelope *env = &obj->params.anim.envelope;
    f32 t;

    if (env->time < env->riseStart) {
        obj->params.scale = env->base;
    } else if (env->time < env->riseEnd) {
        f32 s;
        s = (env->time - env->riseStart) * env->rampRate;
        obj->params.scale = env->base + env->range * s;
    } else if (env->time < env->fallStart) {
        obj->params.scale = env->peak;
    } else {
        t = 1.0f - (env->time - env->fallStart) * env->rampRate;
        obj->params.scale = env->base + env->range * t;
    }
    env->time = env->time + D_800BE9A4;
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
    if (((GlowMsgFn volatile *)D_8008A02C)[*(volatile u8 *)&obj->params.type] != NULL) {
        D_8008A02C[*(volatile u8 *)&obj->params.type](obj, msg, msgType);
    }
    params = &obj->params;
    switch (msgType) {
        case 0x22:
        case 0x24:
        case 0x25:
            if (*msg == (*params).type) {
                switch (msgType) {
                    case 0x22:
                        func_1516972C((struct102 *)obj);
                        break;
                    case 0x24:
                        params->handler = -1;
                        break;
                    case 0x25:
                        params->handler = 2;
                        break;
                }
            }
            break;
    }
}

struct Data_151417C4 {
    s32 unk0;
    s32 unk4;
};
void func_15169260(struct Data_151417C4 *, s32, u8 *, u8);

/* Sends func_15169260 the one-byte payload arg0, addressed with the two words of D_8008A074. */
void func_151417C4(u8 arg0, u8 arg1) {
    u8 payload[1];
    struct Data_151417C4 data;

    data = *(struct Data_151417C4 *)D_8008A074;
    payload[0] = arg0;
    func_15169260(&data, 2, payload, arg1);
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
    f32 mtx[4][4];

    func_150A8050(mtx, angleX, 0.0f, angleZ);
    mtx[3][0] = pivot[0];
    mtx[3][1] = pivot[1];
    mtx[3][2] = pivot[2];
    func_150A7960(&mtx[0][0], 0.0f, distance, 0.0f, &obj->pos[0], &obj->pos[1], &obj->pos[2]);
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
