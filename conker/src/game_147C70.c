#include <ultra64.h>
/* functions.h declares func_15048A40 as returning void; it returns f32 */
#define func_15048A40 func_15048A40_void_unused
#include "functions.h"
#undef func_15048A40
#include "variables.h"

extern f32 func_15048A40(u8 angle);
extern f32 func_150489B0(u8 angle);
extern void *allocate_memory(s32, s32, s32, s32);
extern void func_1511490C(f32 mtx[4][4], void *obj);
extern void func_150A7960(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);
extern void func_1511A494(void *obj, s32 *arg1, s32 *arg2);

typedef struct {
    u8 *phaseOffsets;  /* per-vertex wobble angle offset */
    f32 *heights;      /* per-vertex ripple displacement */
    f32 *followRates;  /* how fast each vertex follows the wave */
    f32 *falloffs;     /* per-vertex displacement scale by distance from the centre */
    f32 wave;
    f32 amplitude;
    f32 decayThreshold;
    u8 phase;
    u8 phaseStep;
    u8 active;
    s32 unk20;
    s32 unk24;
} Game147C70Ripple;

typedef struct {
    u8 pad0[0x16];
    u16 vertexCount;
    u8 pad18[8];
    Vtx_t *outVertices[2];
    Vtx_t *baseVertices;
    u8 pad2C[0x10];
    s32 params; /* bits 0-7: wobble size, bits 16-31: ripple radius */
    u8 pad40[0x33];
    u8 flags73;
    u8 pad74[0xC];
    Game147C70Ripple *ripple;
    s32 wobblePhases; /* four packed u8 phases */
} Game147C70State;

/* Starts a ripple: full amplitude, phase reset and all vertex heights cleared. */
void func_1511A7C0(Game147C70State *mesh) {
    Game147C70Ripple *ripple;
    s32 i;

    ripple = mesh->ripple;
    ripple->active = 1;
    ripple->wave = 0.0f;
    ripple->phase = 0;
    ripple->phaseStep = 7;
    ripple->amplitude = 260.0f;
    ripple->decayThreshold = 100.0f;
    for (i = 0; i < mesh->vertexCount; i++) {
        ripple->heights[i] = 0.0f;
    }
}

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

/*
 * Per-frame update of a wobbling, rippling mesh: lazily allocates the ripple state, advances the
 * ripple wave, then writes each base vertex (wobbled by its lane phase and lifted by its ripple
 * height) through the object's transform into this frame's output vertex buffer.
 */
void func_1511A838(Game147C70State *mesh) {
    s32 i;
    s32 j;
    f32 matrix[4][4];
    f32 outX;
    f32 outY;
    f32 outZ;
    f32 wobbleSize;
    f32 invRadius;
    f32 factor;
    f32 offsetX;
    f32 offsetZ;
    s32 prevPhase;
    u8 lanePhases[4];
    u8 *phaseOffsets;
    f32 *heights;
    f32 *followRates;
    f32 *falloffs;
    Vtx_t *base;
    Game147C70Ripple *ripple;
    s32 offsetY;
    f32 radius;
    f32 dist;
    u8 angle;
    s32 lane;
    Vtx_t *out;
    f32 dx;
    f32 dz;

    base = mesh->baseVertices;
    wobbleSize = mesh->params & 0xFF;
    if (mesh->ripple == NULL) {
        mesh->ripple = allocate_memory(sizeof(Game147C70Ripple), 1, 0, 0);
        ripple = mesh->ripple;
        ripple->phaseOffsets = phaseOffsets = allocate_memory(mesh->vertexCount, 1, 0, 0);
        ripple->heights = allocate_memory(mesh->vertexCount * sizeof(f32), 1, 0, 0);
        heights = ripple->heights;
        ripple->followRates = followRates = allocate_memory(mesh->vertexCount * sizeof(f32), 1, 0, 0);
        ripple->falloffs = falloffs = allocate_memory(mesh->vertexCount * sizeof(f32), 1, 0, 0);
        radius = (mesh->params >> 16) & 0xFFFF;
        if (radius == 0.0f) {
            invRadius = 1.0f;
        } else {
            invRadius = 1.0f / radius;
        }
        for (i = 0; i < mesh->vertexCount; i++) {
            phaseOffsets[i] = (u32)func_150ADA20() % 255;
            dx = base[i].ob[0];
            dz = base[i].ob[2];
            dx *= dx;
            dz *= dz;
            dist = sqrtf(dx + dz);
            falloffs[i] = (radius - dist) * invRadius;
            followRates[i] = (radius - dist * 1.2f) * invRadius;
            if (followRates[i] < 0.0f) {
                followRates[i] = 0.0f;
            }
        }
        for (i = 0; i < mesh->vertexCount; i++) {
            /* vertices at the same position get the same offset, so they move together */
            phaseOffsets[i] = func_150ADA20();
            for (j = 0; j < mesh->vertexCount; j++) {
                if (base[i].ob[0] == base[j].ob[0] && base[i].ob[1] == base[j].ob[1] && base[i].ob[2] == base[j].ob[2]) {
                    phaseOffsets[j] = phaseOffsets[i];
                }
            }
        }
        ripple->wave = 0.0f;
        ripple->active = 0;
        for (i = 0; i < mesh->vertexCount; i++) {
            heights[i] = 0.0f;
        }
        ripple->unk20 = 0;
        ripple->unk24 = 0;
    }
    ripple = mesh->ripple;
    phaseOffsets = ripple->phaseOffsets;
    heights = ripple->heights;
    followRates = ripple->followRates;
    falloffs = ripple->falloffs;
    if ((mesh->flags73 & 3) == 3) {
        func_1511A7C0(mesh);
        mesh->flags73 &= ~3;
    }
    if (ripple->active) {
        prevPhase = ripple->phase;
        ripple->phase += ripple->phaseStep * D_800BE9E4;
        if (ripple->amplitude < ripple->decayThreshold) {
            factor = 0.5f;
        } else {
            factor = 0.8f;
        }
        if (ripple->phase < prevPhase || (ripple->phase > 128 && prevPhase <= 128)) {
            ripple->amplitude *= factor;
        }
        ripple->wave = func_15048A40(ripple->phase);
        ripple->wave = ripple->amplitude * ripple->wave;
        if (ripple->amplitude <= 3.0f) {
            ripple->wave = 0.0f;
            ripple->active = 0;
        }
        for (i = 0; i < mesh->vertexCount; i++) {
            f32 delta = ripple->wave - heights[i];

            heights[i] += delta * followRates[i];
        }
    }
    func_1511490C(matrix, mesh);
    out = mesh->outVertices[D_800BE9C0]; /* this frame's buffer */
    lanePhases[0] = mesh->wobblePhases & 0xFF;
    lanePhases[1] = (mesh->wobblePhases >> 8) & 0xFF;
    lanePhases[2] = (mesh->wobblePhases >> 16) & 0xFF;
    lanePhases[3] = (mesh->wobblePhases >> 24) & 0xFF;
    offsetY = 0;
    for (i = 0; i < mesh->vertexCount; i++, out++, base++) {
        if (!(out->flag & 0x4000)) {
            lane = phaseOffsets[i] & 3;
            angle = phaseOffsets[i] + lanePhases[lane];
            if (phaseOffsets[i] & 0x80) {
                angle = -angle;
            }
            /* u8-angle trig lookups (cosine / sine, judging by the X/Z circle they trace) */
            offsetX = func_15048A40(angle) * wobbleSize;
            offsetZ = func_150489B0(angle) * wobbleSize;
            if (ripple->active) {
                offsetY = heights[i] * falloffs[i];
            }
            func_150A7960(matrix, base->ob[0] + offsetX, base->ob[1] + offsetY, base->ob[2] + offsetZ, &outX, &outY, &outZ);
        } else {
            func_150A7960(matrix, base->ob[0], base->ob[1], base->ob[2], &outX, &outY, &outZ);
        }
        out->ob[0] = outX;
        out->ob[1] = outY;
        out->ob[2] = outZ;
    }
    lanePhases[0] += 1;
    lanePhases[1] += 2;
    lanePhases[2] += 3;
    lanePhases[3] += 4;
    mesh->wobblePhases = lanePhases[0] | (lanePhases[1] << 8) | (lanePhases[2] << 16) | (lanePhases[3] << 24);
    func_1511A494(mesh, &ripple->unk20, &ripple->unk24);
}
