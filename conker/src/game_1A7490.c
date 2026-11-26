#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Spawns one particle of object type `type` at (x, y, z) with random horizontal speed and spin. */
void func_15179FE0(u8 texture, s16 x, s16 y, s16 z, s8 velY, u16 timer, u16 unkAC, u8 speedRange,
                   u8 spinRange, u8 dragRange, s32 type) {
    SpinDebrisParticle *particle;
    s32 rnd;

    particle = func_15167A68(type, 0, sizeof(SpinDebrisParticle), 1, 0xFF, 1);
    if (particle != NULL) {
        particle->x = x;
        particle->y = y;
        particle->z = z;
        particle->velY = velY;
        particle->velX = (func_150ADA20() % speedRange) - (speedRange >> 1);
        particle->velZ = (func_150ADA20() % speedRange) - (speedRange >> 1);
        if (unkAC == 5) {
            particle->x += particle->velX * 4;
            particle->y -= particle->velY * 4;
            particle->z += particle->velZ * 4;
        }
        particle->rotX = 0;
        particle->rotY = 0;
        particle->rotZ = 0;
        particle->spinX = (func_150ADA20() % spinRange) - (spinRange >> 1);
        particle->spinY = (func_150ADA20() % spinRange) - (spinRange >> 1);
        particle->spinZ = (func_150ADA20() % spinRange) - (spinRange >> 1);
        rnd = func_150ADA20() & 0x7F;
        particle->scale = (rnd + 140) * 0.01f;
        particle->timer = timer;
        particle->texture = texture;
        particle->unkAC = unkAC;
        particle->dragRange = dragRange;
        particle->viewFlags = 0;
        particle->alpha = 0xFF;
    }
}

/* Update callback for particles from func_15179FE0: move, spin, slow down, then fade out once the timer runs
 * out. Frees the particle when it is invisible or has left every view it was drawn in. */
void func_1517A1EC(SpinDebrisParticle *particle) {
    s32 timer;

    particle->x += particle->velX;
    particle->y -= particle->velY;
    particle->z += particle->velZ;
    particle->rotX += particle->spinX;
    particle->rotY += particle->spinY;
    particle->rotZ += particle->spinZ;
    if (particle->unkAC == 5) {
        particle->velY++;
    }
    if (particle->timer < particle->unkAC) {
        s32 sign;
        u32 drag;

        drag = func_150ADA20() % particle->dragRange;
        sign = (particle->velX < 0) ? -1 : 1;
        particle->velX += -sign * drag;
        drag = func_150ADA20() % particle->dragRange;
        sign = (particle->velZ < 0) ? -1 : 1;
        particle->velZ += -sign * drag;
    }
    timer = particle->timer;
    timer -= D_800BE9E4;
    if (timer < 0) {
        s32 alpha;
        timer = 0;
        alpha = particle->alpha;
        alpha -= D_800BE9E4 * 4;
        if (alpha < 0) {
            alpha = 0;
        }
        particle->alpha = alpha;
    }
    particle->timer = timer;
    if (particle->alpha == 0 ||
        (particle->viewFlags && (particle->viewFlags >> 4) == (particle->viewFlags & 0xF))) {
        func_1516972C(&particle->header);
    }
}

/* Identity hook used by func_1517A958 for object types 0xC and 0x59. */
Gfx *func_1517A394(Gfx *gfx) {
    return gfx;
}

#define D_800D35E4 ((&D_800D35E8)[-1])
extern s16 D_800DD450; /* primitive alpha last set in the display list; -1 = unknown */
extern u8 D_8008CDF0[];

/* Draw callback: culls the particle against view `view` (types 0xC and 0x59 are never culled), then emits its
 * texture, matrix, primitive alpha and render mode, and the shared particle display list D_8008CDF0. */
Gfx *func_1517A3A0(Gfx *gfx, SpinDebrisParticle *particle, s16 view) {
    if (particle->header.type != 0xC && particle->header.type != 0x59 &&
        func_1510AEE0(D_800D9C10[view], particle->x, particle->y, particle->z, D_800D9B20, D_800D9B1C,
                      D_800D35E0, D_800D35E4, NULL, NULL) != 0) {
        if (particle->header.type == 8 && (particle->viewFlags & (1 << view))) {
            particle->viewFlags |= 0x10 << view;
        }
        return gfx;
    }
    if (particle->header.type != 9) {
        particle->viewFlags |= 1 << view;
    }
    gfx = func_1517A9A8(gfx, particle->texture);
    func_15043D90(&particle->mtx[D_800BE9C0], particle->rotX, particle->rotY, particle->rotZ,
                  particle->scale, particle->scale, particle->scale, particle->x, particle->y, particle->z);
    gSPMatrix(gfx++, &particle->mtx[D_800BE9C0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    if (D_800DD450 != particle->alpha) {
        gDPPipeSync(gfx++);
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, particle->alpha);
        // matching: D_800DD450 spelled via its neighbour so IDO does not CSE its address with the read above
        ((s16 *)D_800DD460)[-8] = particle->alpha;
    }
    {
        s32 pad; // matching: unused stack slot (needSync lands at sp+0x43)
        s32 modeL;
        u8 needSync;
        needSync = 0;
        modeL = ((particle->header.type == 8) || (particle->header.type == 9)) ? 0x5049D8 : 0x504240;
        gfx = func_15142FBC(gfx, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C |
                                     G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            modeL, &needSync);
    }
    gSPDisplayList(gfx++, D_8008CDF0);
    return gfx;
}

/* Spawns one ground debris chunk (object type 9) of size `size` at (x, z), resting on the ground below y. */
void func_1517A644(f32 size, u8 texture, s16 x, s16 y, s16 z) {
    SpinDebrisParticle *particle;
    s32 range;
    f32 groundY;
    s32 rnd;
    s32 half;

    range = size;
    if (range != 0) {
        particle = func_15167A68(9, 0, sizeof(SpinDebrisParticle), 1, 0xFF, 1);
        if (particle != NULL) {
            func_1510E82C(0, 0, &groundY, 0, 0, 0, x, y, z, y, 0, 0);
            particle->y = particle->groundY = groundY + 3.0f;
            particle->x = x;
            particle->z = z;
            particle->rotX = 90;
            particle->rotY = func_150ADA20() % 360;
            particle->rotZ = 0;
            particle->velX = 0;
            particle->velZ = 0;
            particle->texture = texture;
            rnd = (func_150ADA20() % 80) + 100;
            particle->scale = rnd * 0.01f;
            particle->velY = size * 0.4f;
            rnd = func_150ADA20() % (u32)range;
            half = size * 0.5f;
            particle->spinX = rnd - half;
            particle->spinY = (func_150ADA20() % (u32)range) - half;
            particle->spinZ = (func_150ADA20() % (u32)range) - half;
            particle->timer = 0;
            particle->alpha = 0xFF;
        }
    }
}

/* Update callback for func_1517A644 chunks: fly up, fall back to the ground, then lie flat and fade out over
 * 60 frames before being freed. */
void func_1517A84C(SpinDebrisParticle *particle) {
    s32 timer;
    s32 velY;
    s32 groundY;
    s32 height;

    timer = particle->timer;
    if (timer == 0) {
        particle->x += particle->velX;
        velY = particle->velY;
        particle->y += velY;
        particle->z += particle->velZ;
        if (velY > 0) {
            particle->velY = velY - 1;
            if (particle->velY <= 0) {
                particle->velY = -3;
            }
        }
        groundY = particle->groundY;
        height = particle->y - groundY;
        if (height <= 0) {
            particle->rotX = 90;
            particle->rotZ = 0;
            particle->timer = 60;
            particle->y = groundY;
        } else {
            particle->rotX += particle->spinX;
            particle->rotY += particle->spinY;
            particle->rotZ += particle->spinZ;
        }
    } else {
        timer -= D_800BE9E4;
        if (timer > 0) {
            particle->timer = timer;
            particle->alpha = (timer << 8) / 60;
        } else {
            func_1516972C(&particle->header);
        }
    }
}

/* Begin-draw hook: forgets the cached primitive alpha so the first particle drawn sets it again. */
Gfx *func_1517A958(Gfx *gfx, s32 type, s32 arg2, s32 arg3) {
    D_800DD450 = -1;
    if ((type == 0xC) || (type == 0x59)) {
        gfx = func_1517A394(gfx);
    }
    return gfx;
}

extern u8 D_80090614[];

/* Loads particle texture `texture` unless it is already the current one. */
Gfx *func_1517A9A8(Gfx *gfx, s32 texture) {
    struct {
        s32 unk0[5];
    } texState; // filled in by func_15094F70; unused here

    if (texture != D_800DD1B0) {
        gfx = func_15094F70(gfx, D_80090614, (u32)texture << 8, &texState, 0, 0, 0, 2, 3);
        D_800DD1B0 = texture;
    }
    return gfx;
}

/* Spawns `count` ground debris chunks scattered around (x, y, z) when `size` is at least 12. */
void func_1517AA20(f32 size, s32 x, s32 y, s32 z, s32 count) {
    s32 offsetX;
    s32 offsetZ;
    s32 i;
    s32 range;
    f32 chunkSize;

    if (!(size < 12.0f)) {
        range = size * 8.0f;
        size *= 0.5f;
        for (i = 0; i < count; i++) {
            offsetX = func_150ADA20() & 0x3F;
            offsetZ = func_150ADA20() & 0x3F;
            offsetX -= 32;
            offsetZ -= 32;
            chunkSize = (f32)((func_150ADA20() % (u32)range) >> 4) + size;
            func_1517A644(chunkSize, func_150ADA20() & 1, x + offsetX, y, z + offsetZ);
        }
    }
}

/* Draw callback for another object type in the same table; forwards to func_1510B7B4. */
Gfx *func_1517AB7C(Gfx *gfx, s32 arg1, s16 player) {
    return func_1510B7B4(gfx, player);
}
