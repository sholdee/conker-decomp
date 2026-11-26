#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include <libc/stddef.h>

/* User data of the model spawned by func_1513BAE8. */
typedef struct {
    /* 0x00 */ f32 scrollS;
    /* 0x04 */ f32 scrollT;
    /* 0x08 */ f32 scrollTPhase;
    /* 0x0C */ f32 scalePhaseS;
    /* 0x10 */ f32 scalePhaseT;
} TexWarpState;

typedef struct {
    s32 start;
    s32 count;
} VtxRange;

extern s32 (*D_80089C18[])(DynModel *);
extern s32 (*D_80089C28[])(DynModel *, s16);
extern Vec3f D_800A49C0[9];
extern VtxRange D_800A4A2C[9];

/* Loads the descriptor's model and allocates a DynModel holding copies of its vertices. */
DynModel *func_1513B5E0(DynModelDesc *desc, u8 arg1, s32 userDataSize, u8 arg3, s32 arg4) {
    DynModel *obj;
    ModelAsset *asset;
    s32 buffersSize;
    s32 vtxSize;
    u8 *buffer;
    s32 loaded;
    s32 count;
    s32 i;

    asset = func_1502B6BC(&loaded, 0, &count, 2, desc->unk30, desc->unk34);
    {
        s32 type;

        vtxSize = (u8 *)asset->displayList - (u8 *)asset->vertices;
        buffersSize = vtxSize * 2 * (D_80082FA0 + 1);
        if (arg1) {
            type = 0x54;
        } else {
            type = 0x38;
        }
        obj = func_15167A68(type, arg4, userDataSize + buffersSize + offsetof(DynModel, buffers), 2, arg3, 1);
        if (obj == NULL) {
            return NULL;
        }
        memcpy(&obj->desc, desc, sizeof(DynModelDesc));
        obj->asset = asset;
    }
    for (i = 0, buffer = obj->buffers; i < D_80082FA0 + 1; i++) {
        obj->vertices[0][i] = (Vtx *)buffer;
        obj->vertices[1][i] = (Vtx *)(buffer + vtxSize);
        memcpy(buffer, asset->vertices, vtxSize);
        memcpy(obj->vertices[1][i], asset->vertices, vtxSize);
        // matching: the unsigned (size_t) step lets IDO reuse vtxSize * 2 from above
        buffer += vtxSize * 2 * sizeof(u8);
    }
    obj->displayList = asset->displayList;
    obj->buffersSize = buffersSize;
    func_1510CE60(obj->displayList, 0, 1, 0x3E, 0);
    return obj;
}

/* Per-frame update: counts down the lifetime, runs the update callback, frees when done. */
void func_1513B798(DynModel *obj) {
    s32 func;
    u8 done = FALSE;

    if (obj->desc.flags & DYNMODEL_FLAG_LIFETIME) {
        obj->desc.lifetime -= D_800BE9E4;
        if (obj->desc.lifetime < 0) {
            done = TRUE;
        }
    }
    if (!done) {
        func = obj->desc.updateFunc;
        if (func != -1) {
            if (D_80089C18[func](obj) == 0) {
                done = TRUE;
            }
        }
    }
    if (done) {
        func_1516972C(&obj->header);
    }
}

/* Draws the model in one viewport, after the draw callback (if any) has updated it. */
Gfx *func_1513B83C(Gfx *gfx, DynModel *obj, s16 viewport) {
    s32 func;

    if ((obj->desc.flags & DYNMODEL_FLAG_VIEWPORT_MASK) && !(obj->desc.viewportMask & (1 << viewport))) {
        return gfx;
    }

    func = obj->desc.drawFunc;
    if (func != -1) {
        if (D_80089C28[func](obj, viewport) == 0) {
            return gfx;
        }
    }

    gSPMatrix(gfx++, &obj->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPSegment(gfx++, 1, obj->vertices[D_800BE9C0][viewport]);
    gSPDisplayList(gfx++, obj->displayList);

    return gfx;
}

s32 func_1513B968(DynModel *obj, s16 viewport) {
    func_150A7B80(&obj->mtx[D_800BE9C0]);
    return 1;
}

void func_1513B9A8(DynModel *obj) {
    func_100043B4((s32 *)obj->asset, 4);
    func_15169804(&obj->header);
}

void func_1513B9DC(DynModel *obj) {
    func_100043B4((s32 *)obj->asset, 4);
    func_15169824(&obj->header);
}

void func_1513BA10(DynModel *obj) {
    D_80089C44[obj->desc.kind]();
}

void func_1513BA44(DynModel *obj) {
    D_80089C54[obj->desc.kind]();
}

void func_1513BA78(DynModel *obj, s32 arg1, u8 arg2) {
    switch (obj->desc.kind) {
    case 1:
        func_15109064(obj, arg1, arg2);
        break;
    case 2:
        func_151BA468(obj, arg1, arg2);
        break;
    }
}

s32 func_1513BAD4(DynModel *obj, s16 viewport) {
    return 0;
}

/* Spawns the scrolling, warping texture model (update func_1513BEB0, draw func_1513BBFC). */
DynModel *func_1513BAE8(void) {
    DynModel *obj;
    DynModelDesc desc;
    TexWarpState state;

    desc.updateFunc = 2;
    desc.drawFunc = 5;
    desc.lifetime = 300;
    desc.unk30 = 9;
    state.scrollS = 0.0f;
    state.scrollT = 0.0f;
    state.scalePhaseS = 0.0f;
    state.scalePhaseT = 0.0f;
    state.scrollTPhase = 0.0f;
    desc.flags = 0;
    desc.unk34 = 0x1AE;
    desc.unk8 = 1;
    desc.unkC = 0x220205;
    desc.unk10 = 0x40600;
    desc.unk24 = 0;
    desc.unk25 = 0;
    desc.unk14 = 1;
    desc.unk18 = 0x36;
    desc.unk1C = 0x80;
    desc.unk20 = 0x20;
    desc.kind = 3;

    obj = func_1513B5E0(&desc, 1, sizeof(TexWarpState), 0xFF, 1);
    if (obj != NULL) {
        if (obj->buffersSize != 0x1180) {
            func_1516972C(&obj->header);
        } else {
            memcpy(obj->buffers + obj->buffersSize, &state, sizeof(TexWarpState));
        }
    }
    return obj;
}

/* Draw callback: recomputes each vertex group's texture coordinates relative to the viewport position. */
s32 func_1513BBFC(DynModel *obj, s16 viewport) {
    TexWarpState *state;
    f32 scaleS;
    f32 scaleT;
    f32 offsetS;
    f32 offsetT;
    s32 k;
    Vec3f *viewPos;
    f32 dirX;
    f32 dirZ;
    s32 j;

    func_1513B968(obj, viewport);
    state = (TexWarpState *)(obj->buffers + obj->buffersSize);
    scaleS = sinf(state->scalePhaseS) * 1.210000038f + 2.854000092f;
    scaleT = sinf(state->scalePhaseT) * 1.994000077f + 8.328000069f;
    viewPos = func_15144B34(viewport);
    offsetS = state->scrollS - (s32)(state->scrollS * (1.0f / 1024.0f)) * 1024.0f;
    offsetT = state->scrollT - (s32)(state->scrollT * (1.0f / 1024.0f)) * 1024.0f;
    for (k = 0; k < 9; k++) {
        f32 dx;
        f32 dz;

        dx = D_800A49C0[k].x - viewPos->x;
        dz = D_800A49C0[k].z - viewPos->z;
        if (dx != 0.0f || dz != 0.0f) {
            f32 invLen;

            invLen = 1.0f / sqrtf(dx * dx + dz * dz);
            dirX = dz * invLen;
            dirZ = -dx * invLen;
        } else {
            dirX = 1.0f;
            dirZ = 0.0f;
        }
        for (j = D_800A4A2C[k].start; j < D_800A4A2C[k].start + D_800A4A2C[k].count; j++) {
            Vtx *vtx;
            f32 x;
            f32 y;
            f32 z;
            f32 ex;
            f32 ey;
            f32 ez;

            vtx = &obj->vertices[D_800BE9C0][viewport][j];
            x = vtx->v.ob[0];
            y = vtx->v.ob[1];
            z = vtx->v.ob[2];
            ex = x - D_800A49C0[k].x;
            ey = y - D_800A49C0[k].y;
            ez = z - D_800A49C0[k].z;
            vtx->v.tc[0] = (ex * dirX + ez * dirZ) * scaleS - offsetS;
            vtx->v.tc[1] = ey * scaleT - offsetT;
        }
    }
    return 1;
}

/* Update callback: advances the texture scroll and warp phases, wrapping the scroll to [0, 4096). */
s32 func_1513BEB0(DynModel *obj) {
    TexWarpState *state;

    state = (TexWarpState *)(obj->buffers + obj->buffersSize);
    state->scrollTPhase += 0.03700000048f * D_800BE9A4;
    state->scrollTPhase = func_15144B68(state->scrollTPhase);
    state->scrollS += 1.899999976f * D_800BE9A4;
    state->scrollT = sinf(state->scrollTPhase) * 77.30000305f;
    state->scalePhaseS += 0.01900000125f * D_800BE9A4;
    state->scalePhaseT += 0.01000000071f * D_800BE9A4;
    state->scalePhaseS = func_15144B68(state->scalePhaseS);
    state->scalePhaseT = func_15144B68(state->scalePhaseT);

    while (state->scrollS > 4096.0f) {
        state->scrollS -= 4096.0f;
    }
    while (state->scrollS < 0.0f) {
        state->scrollS += 4096.0f;
    }
    while (state->scrollT > 4096.0f) {
        state->scrollT -= 4096.0f;
    }
    while (state->scrollT < 0.0f) {
        state->scrollT += 4096.0f;
    }
    return 1;
}
