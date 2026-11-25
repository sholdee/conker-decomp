#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Model instance created by func_1503F62C (see game_6C960.c); only the fields used here. */
typedef struct {
    /* 0x000 */ u8 pad0[0x3E0];
    /* 0x3E0 */ f32 (*rootMtx[2])[4]; /* per frame: the owner's float matrix */
    /* 0x3E8 */ s32 mtxBuf[2];        /* per frame: matrix buffer bound to segment 3 */
} ModelInstance;

/* Visibility mask: bit n set = draw in view n. */
typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 viewBits;
} EffectMask;

/* Spawn parameters, copied verbatim into an effect at 0x10. */
typedef struct {
    /* 0x00 */ u8 flags;
    /* 0x01 */ s8 updateFunc;   /* index into D_8008AD90, -1 = none */
    /* 0x02 */ s8 updateFunc2;  /* index into D_8008ADA0, -1 = none */
    /* 0x03 */ s8 preDrawFunc;  /* index into D_8008ADBC, -1 = none */
    /* 0x04 */ s8 postDrawFunc; /* index into D_8008ADCC, -1 = none */
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 lifetime;
    /* 0x08 */ s32 model;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 otherModeH;
    /* 0x14 */ s32 geometrySet;
    /* 0x18 */ s32 geometryClear;
    /* 0x1C */ s32 renderMode;   /* index into D_800A4AC8 (other-mode L words) */
    /* 0x20 */ u8 pad20[3];
    /* 0x23 */ u8 unk23;
    /* 0x24 */ s32 otherModeH2;
    /* 0x28 */ s32 otherModeH3;
    /* 0x2C */ s8 primMode;
    /* 0x2D */ s8 envMode;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ u8 color0[4];
    /* 0x34 */ u8 color1[4];
    /* 0x38 */ u8 pad38[4];
    /* 0x3C */ void *owner;   /* actor; see func_15169850 */
    /* 0x40 */ u8 ownerId;
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 pad42[2];
    /* 0x44 */ Vec3f pos;
    /* 0x50 */ EffectMask *mask;
    /* 0x54 */ s16 fadeStart; /* alpha fades once lifetime drops below this */
    /* 0x56 */ s16 fadeRate;
} EffectParams; /* size 0x58 */

/* Attachment state handed to func_151462C8. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 unk4;
    /* 0x08 */ s32 unk8[4]; /* func_1515D480 allocations, one per entry up to D_80082FA0 */
    /* 0x18 */ s32 unk18;   /* func_1515D440 allocation */
} EffectAttach;

/* Extension used by the D_8008AD90/D_8008ADA0 update callbacks below. */
typedef struct {
    /* 0x00 */ Vec3f rot;
    /* 0x0C */ f32 scale;
    /* 0x10 */ Vec3f vel;
    /* 0x1C */ Vec3f rotVel;
    /* 0x28 */ f32 gravity;
    /* 0x2C */ f32 drag;
    /* 0x30 */ u8 flags; /* 1: move, 2: spin, 4: gravity, 8: drag */
} EffectMotion;

typedef union {
    f32 raw[0xE];
    EffectMotion motion;
} EffectExtra; /* size 0x38 */

/* Model effect object (types 0x36 / 0x4C / 0x5B). */
typedef struct {
    /* 0x000 */ u8 header[0x10]; /* object link header (func_15167A68) */
    /* 0x010 */ EffectParams params;
    /* 0x068 */ ModelInstance *model;
    /* 0x06C */ s32 *displayLists;
    /* 0x070 */ u8 displayListCount;
    /* 0x071 */ u8 pad71[3];
    /* 0x074 */ s32 segment1;
    /* 0x078 */ s32 unk78;
    /* 0x07C */ f32 mtx[2][4][4]; /* per frame */
    /* 0x0FC */ EffectAttach attach;
    /* 0x118 */ s32 unk118;
    /* 0x11C */ u8 pad11C[4];
    /* 0x120 */ EffectExtra extra; /* optional extension (func_15157898) */
} Effect;

extern Effect *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void func_1503F62C(s32, s32, void *, void *, void *, void *, void *);
extern void func_1503F5B8(ModelInstance *, s32, s32, f32, f32, s32);
extern s32 func_1515D480(s32);
extern s32 func_1515D440(void);
extern s32 (*D_8008AD90[])(Effect *);
extern s32 (*D_8008ADA0[])(Effect *);
extern void func_1503F4B0(ModelInstance *);

/*
 * Spawns a model effect from `params`, with `extraSize` bytes of extension at 0x120. The object type
 * is 0x5B or 0x4C when params->flags has 0x80 or 0x10 set, else 0x36.
 */
Effect *func_15157010(EffectParams *params, s32 arg1, f32 arg2, s32 attachArg, s32 arg4, s32 extraSize,
                      u8 owner, s32 list) {
    Effect *fx;
    s32 type;
    s32 i;

    type = 0x36;
    if (params->flags & 0x80) {
        type = 0x5B;
    } else if (params->flags & 0x10) {
        type = 0x4C;
    }
    fx = func_15167A68(type, list, extraSize + 0x120, 1, owner, 1);
    if (fx == NULL) {
        return NULL;
    }
    memcpy(&fx->params, params, sizeof(EffectParams));
    func_1503F62C(fx->params.model, fx->params.unkC, &fx->displayLists, &fx->displayListCount, &fx->segment1,
                  &fx->unk78, &fx->model);
    guMtxIdentF(fx->mtx[0]);
    guMtxIdentF(fx->mtx[1]);
    fx->model->rootMtx[0] = fx->mtx[0];
    fx->model->rootMtx[1] = fx->mtx[1];
    func_1503F5B8(fx->model, 1, arg1, arg2, 0.0f, 0);
    fx->attach.unk0 = attachArg;
    fx->unk118 = arg4;
    fx->attach.unk4 = 0;
    for (i = 0; i < 4; i++) {
        fx->attach.unk8[i] = 0;
    }
    fx->attach.unk18 = 0;
    if (attachArg) {
        for (i = 0; i <= D_80082FA0; i++) {
            fx->attach.unk8[i] = func_1515D480(attachArg);
        }
        fx->attach.unk18 = func_1515D440();
    }
    return fx;
}

/* Frees the attachment allocations made by func_15157010. */
void func_151571C4(Effect *fx) {
    s32 i;

    for (i = 0; i <= D_80082FA0; i++) {
        if (fx->attach.unk8[i] != 0) {
            func_100043B4((s32 *)fx->attach.unk8[i], 4);
        }
    }
    if (fx->attach.unk18 != 0) {
        func_100043B4((s32 *)fx->attach.unk18, 4);
    }
}

extern void func_1518CA04(s32);
extern void func_1503F7B8(ModelInstance *);

/* Destroy handler: frees the attachments and model instance, then the object (func_15169804). */
void func_15157248(Effect *fx) {
    func_151571C4(fx);
    func_1518CA04(fx->params.model);
    func_1503F7B8(fx->model);
    func_15169804((struct102 *)fx);
}

/* As func_15157248, but releases the object through func_15169824. */
void func_1515728C(Effect *fx) {
    func_151571C4(fx);
    func_1518CA04(fx->params.model);
    func_1503F7B8(fx->model);
    func_15169824((struct102 *)fx);
}

/*
 * Update handler: counts the lifetime down (flag 1), runs both update callbacks and animates the
 * model. With flag 0x20 the alpha fades out over the last fadeStart ticks. The effect is destroyed
 * when its lifetime runs out or a callback returns 0.
 */
void func_151572D0(Effect *fx) {
    s32 value; // matching: one local for both callback indices and the faded alpha
    u8 dead;
    s16 lifetime;

    dead = FALSE;
    if (fx->params.flags & 1) {
        fx->params.lifetime -= D_800BE9E4;
        if (fx->params.lifetime < 0) {
            dead = TRUE;
        }
    }
    if (!dead) {
        value = fx->params.updateFunc;
        if (value != -1) {
            if (D_8008AD90[value](fx) == 0) {
                dead = TRUE;
            }
        }
        value = fx->params.updateFunc2;
        if (value != -1) {
            if (D_8008ADA0[value](fx) == 0) {
                dead = TRUE;
            }
        }
    }
    if (!dead) {
        func_1503F4B0(fx->model);
    }
    if (!dead && (fx->params.flags & 0x20)) {
        lifetime = fx->params.lifetime;
        if (lifetime < fx->params.fadeStart) {
            value = lifetime * fx->params.fadeRate;
            if (value < fx->params.color0[3]) {
                fx->params.color0[3] = value;
            }
        }
    }
    if (dead) {
        func_1516972C((struct102 *)fx);
    }
}

typedef struct {
    s32 w0;
    s32 w1;
} OtherModeL;

extern Gfx *(*D_8008ADBC[])(Gfx *, Effect *, s16, u8 *, u8 *);
extern Gfx *(*D_8008ADCC[])(Gfx *, Effect *, s16, u8 *);
extern OtherModeL D_800A4AC8[];
extern u8 D_80083740[];
extern u8 D_800838C0[];
extern u8 D_80083A40[];
extern u8 D_80083BC0[];

Gfx *func_15142B7C(Gfx *gfx, s32 setBits, s32 clearBits);
Gfx *func_15142C10(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142CF0(Gfx *gfx, s32 minLevel, s32 lodFrac, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142FBC(Gfx *gfx, s32 modeH, s32 modeL, u8 *needSync);
Gfx *func_1513F4E4(Gfx *gfx, u8 arg1, u8 *needSync);
void func_151441A4(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, s32 a0, u8 r1, u8 g1, u8 b1,
                   u8 a1, u8 level, u8 mode);
void func_151442FC(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1,
                   u8 a1, u8 level, u8 mode);
Gfx *func_151462C8(Gfx *gfx, EffectAttach *attach, u8 arg2, void *owner, u8 ownerId, s16 view, Vec3f *pos, u8 arg7,
                   s32 arg8);

/*
 * Draw handler for `view`: runs the pre-draw callback (which may cancel the draw), honours the view
 * mask, sets up geometry mode, prim/env colours and other modes, binds segments 3 (model matrices),
 * 1 and 8 (opaque or translucent variant), then calls the model's display lists.
 */
Gfx *func_15157420(Gfx *gfx, Effect *fx, s16 view) {
    s32 i;
    u8 needSync;
    s16 primR, primG, primB, primA;
    s16 envR, envG, envB, envA;
    u8 proceed;

    needSync = TRUE;
    if (fx->params.preDrawFunc != -1) {
        gfx = D_8008ADBC[fx->params.preDrawFunc](gfx, fx, view, &needSync, &proceed);
        if (!proceed) {
            return gfx;
        }
    }
    if ((fx->params.flags & 8) && (fx->params.mask != NULL) && !(fx->params.mask->viewBits & (1 << view))) {
        return gfx;
    }
setup: // matching: a label here keeps as1 from filling the branch delay slots with the next load
    gfx = func_15142B7C(gfx, fx->params.geometrySet, fx->params.geometryClear);
    func_151441A4(&primR, &primG, &primB, &primA, fx->params.color0[0], fx->params.color0[1], fx->params.color0[2],
                  fx->params.color0[3], fx->params.color1[0], fx->params.color1[1], fx->params.color1[2],
                  fx->params.color1[3], 0xFF, fx->params.primMode);
    func_151442FC(&envR, &envG, &envB, &envA, fx->params.color0[0], fx->params.color0[1], fx->params.color0[2],
                  fx->params.color0[3], fx->params.color1[0], fx->params.color1[1], fx->params.color1[2],
                  fx->params.color1[3], 0xFF, fx->params.envMode);
    gfx = func_15142C10(gfx, envR, envG, envB, envA, &needSync);
    gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &needSync);
    gfx = func_1513F4E4(gfx, fx->params.unk23, &needSync);
    gfx = func_15142FBC(gfx,
                        fx->params.otherModeH | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | fx->params.otherModeH2 |
                            fx->params.otherModeH3,
                        D_800A4AC8[fx->params.renderMode].w0 | D_800A4AC8[fx->params.renderMode].w1, &needSync);
    if (fx->params.flags & 2) {
        gfx = func_151462C8(gfx, &fx->attach, fx->params.unk41, fx->params.owner, fx->params.ownerId, view,
                            &fx->params.pos, (fx->params.flags & 4) ? 1 : 0, 0);
    }
    gDPPipeSync(gfx++);
    gDPSetFogColor(gfx++, 0, 0, 0, 0xFF);
    gSPSegment(gfx++, 3, fx->model->mtxBuf[D_800BE9C0]);
    gSPSegment(gfx++, 1, fx->segment1);
    if (fx->params.color0[3] < 0xFF || fx->params.color1[3] < 0xFF) {
        gSPSegment(gfx++, 8, (fx->params.flags & 0x40) ? D_80083A40 : D_80083740);
    } else {
        gSPSegment(gfx++, 8, (fx->params.flags & 0x40) ? D_80083BC0 : D_800838C0);
    }
    for (i = 0; i < fx->displayListCount; i++) {
        gSPDisplayList(gfx++, fx->displayLists[i]);
    }
    if (fx->params.postDrawFunc != -1) {
        gfx = D_8008ADCC[fx->params.postDrawFunc](gfx, fx, view, &needSync);
    }
    return gfx;
}

/* Update callback (D_8008ADA0[0]): resets this frame's root matrix to identity. */
s32 func_15157860(Effect *fx) {
    guMtxIdentF(fx->mtx[D_800BE9C0]);
    return TRUE;
}

/* func_15157010 plus a 0x38-byte extension copied from `extra`. */
Effect *func_15157898(EffectParams *params, EffectExtra *extra, s32 arg2, f32 arg3, s32 attachArg, s32 arg5,
                      s32 extraSize, u8 owner, s32 list) {
    Effect *fx;

    fx = func_15157010(params, arg2, arg3, attachArg, arg5, extraSize + sizeof(EffectExtra), owner, list);
    if (fx == NULL) {
        return NULL;
    }
    memcpy(&fx->extra, extra, sizeof(EffectExtra));
    return fx;
}

/* Update callback (D_8008ADA0[1]): root matrix = rotation extra.rot, uniform scale, translation pos. */
s32 func_15157918(Effect *fx) {
    EffectMotion *motion;

    motion = &fx->extra.motion;
    func_150A8050(fx->mtx[D_800BE9C0], motion->rot.x, motion->rot.y, motion->rot.z);
    fx->mtx[D_800BE9C0][3][0] = fx->params.pos.x;
    fx->mtx[D_800BE9C0][3][1] = fx->params.pos.y;
    fx->mtx[D_800BE9C0][3][2] = fx->params.pos.z;
    fx->mtx[D_800BE9C0][0][0] *= motion->scale;
    fx->mtx[D_800BE9C0][0][1] *= motion->scale;
    fx->mtx[D_800BE9C0][0][2] *= motion->scale;
    fx->mtx[D_800BE9C0][1][0] *= motion->scale;
    fx->mtx[D_800BE9C0][1][1] *= motion->scale;
    fx->mtx[D_800BE9C0][1][2] *= motion->scale;
    fx->mtx[D_800BE9C0][2][0] *= motion->scale;
    fx->mtx[D_800BE9C0][2][1] *= motion->scale;
    fx->mtx[D_800BE9C0][2][2] *= motion->scale;
    return TRUE;
}

/*
 * Update callback (D_8008AD90[0]): integrates extra.motion. Drag is applied once per elapsed tick,
 * gravity accelerates vel.y, and pos advances by vel*t + a*t^2/2; rot advances by rotVel.
 */
s32 func_15157AA8(Effect *fx) {
    EffectMotion *motion;
    Vec3f oldVel;
    f32 accelX;
    f32 accelY;
    f32 accelZ;
    s32 i;

    motion = &fx->extra.motion;
    if (motion->flags & 1) {
        oldVel = motion->vel;
        if (motion->flags & 8) {
            for (i = D_800BE9E4; i != 0; i--) {
                motion->vel.x *= motion->drag;
                motion->vel.z *= motion->drag;
            }
        }
        if (motion->flags & 4) {
            motion->vel.y += motion->gravity * D_800BE9A4;
            accelY = motion->gravity;
        } else {
            accelY = 0.0f;
        }
        accelX = (motion->vel.x - oldVel.x) * D_800BE9A8;
        accelZ = (motion->vel.z - oldVel.z) * D_800BE9A8;
        fx->params.pos.x += (oldVel.x + 0.5f * accelX * D_800BE9A4) * D_800BE9A4;
        fx->params.pos.y += (oldVel.y + 0.5f * accelY * D_800BE9A4) * D_800BE9A4;
        fx->params.pos.z += (oldVel.z + 0.5f * accelZ * D_800BE9A4) * D_800BE9A4;
    }
    if (motion->flags & 2) {
        motion->rot.x += motion->rotVel.x * D_800BE9A4;
        motion->rot.y += motion->rotVel.y * D_800BE9A4;
        motion->rot.z += motion->rotVel.z * D_800BE9A4;
    }
    return TRUE;
}

/* Event handler: lets func_15169850 track or retarget this effect's owner. */
void func_15157D88(Effect *fx, s32 event, u8 eventType) {
    func_15169850(event, eventType, (s32)&fx->params.owner, (s32)&fx->params.ownerId, (s32)fx);
}

void func_15157DEC(Effect *fx, f32 *xform);

/* Update callback (D_8008ADA8[3]): root matrix from the extension, see func_15157DEC. */
s32 func_15157DC8(Effect *fx) {
    func_15157DEC(fx, fx->extra.raw);
    return TRUE;
}

/*
 * Sets this frame's root matrix to rotation xform[0..2], translated to pos, with the X and Z
 * axes scaled by xform[3] and the Y axis by xform[4].
 */
void func_15157DEC(Effect *fx, f32 *xform) {
    func_150A8050(fx->mtx[D_800BE9C0], xform[0], xform[1], xform[2]);
    fx->mtx[D_800BE9C0][3][0] = fx->params.pos.x;
    fx->mtx[D_800BE9C0][3][1] = fx->params.pos.y;
    fx->mtx[D_800BE9C0][3][2] = fx->params.pos.z;
    fx->mtx[D_800BE9C0][0][0] *= xform[3];
    fx->mtx[D_800BE9C0][0][1] *= xform[3];
    fx->mtx[D_800BE9C0][0][2] *= xform[3];
    fx->mtx[D_800BE9C0][1][0] *= xform[4];
    fx->mtx[D_800BE9C0][1][1] *= xform[4];
    fx->mtx[D_800BE9C0][1][2] *= xform[4];
    fx->mtx[D_800BE9C0][2][0] *= xform[3];
    fx->mtx[D_800BE9C0][2][1] *= xform[3];
    fx->mtx[D_800BE9C0][2][2] *= xform[3];
}

extern Mtx D_80089470;

/* Pre-draw callback (D_8008ADBC[3]): loads the identity modelview and this view's projection. */
Gfx *func_15157F80(Gfx *gfx, Effect *fx, s32 view, u8 *needSync, u8 *proceed) {
    gSPMatrix(gfx++, &D_80089470, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, &((Mtx *)D_800DCC10)[view], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    *proceed = TRUE;
    return gfx;
}

/* Post-draw callback (D_8008ADCC[0]): reloads this view's projection matrices for the current frame. */
Gfx *func_15157FE8(Gfx *gfx, Effect *fx, s32 view, u8 *needSync) {
    gSPMatrix(gfx++, (Mtx *)((u8 *)&((struct259 *)D_800BE628)[view] + 0x100) + D_800BE9C0,
              G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(gfx++, ((Mtx **)&D_800DC2A0)[D_800BE9C0] + view, G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    return gfx;
}

extern s32 D_800A6060[3]; /* { 0x36, 0x4C, 0x5B }: the model effect types */

/* Sends an event to every model effect. */
void func_15158078(s32 event, u8 eventType) {
    func_15169260((s32)D_800A6060, 3, event, eventType);
}
