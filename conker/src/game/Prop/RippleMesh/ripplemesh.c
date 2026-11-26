#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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

/* Behaviour scratch state (WorldObject.touch) of the rippling mesh. param bits 0-7: wobble size, 16-31: ripple radius. */
typedef struct {
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ Game147C70Ripple *ripple;
    /* 0x84 */ s32 wobblePhases; /* four packed u8 phases */
} Game147C70Work;

#define WORK(obj) ((Game147C70Work *)&(obj)->touch)

/* Starts a ripple: full amplitude, phase reset and all vertex heights cleared. */
void prop_startRipple(WorldObject *mesh) {
    Game147C70Ripple *ripple;
    s32 i;

    ripple = WORK(mesh)->ripple;
    ripple->active = 1;
    ripple->wave = 0.0f;
    ripple->phase = 0;
    ripple->phaseStep = 7;
    ripple->amplitude = 260.0f;
    ripple->decayThreshold = 100.0f;
    for (i = 0; i < mesh->numVertices; i++) {
        ripple->heights[i] = 0.0f;
    }
}


/*
 * Per-frame update of a wobbling, rippling mesh: lazily allocates the ripple state, advances the
 * ripple wave, then writes each base vertex (wobbled by its lane phase and lifted by its ripple
 * height) through the object's transform into this frame's output vertex buffer.
 */
void prop_updateRippleMesh(WorldObject *mesh) {
    s32 i;
    s32 j;
    MtxF matrix;
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

    base = &mesh->srcVertices->v;
    wobbleSize = mesh->moveData.word & 0xFF;
    if (WORK(mesh)->ripple == NULL) {
        WORK(mesh)->ripple = allocate_memory(sizeof(Game147C70Ripple), 1, 0, 0);
        ripple = WORK(mesh)->ripple;
        ripple->phaseOffsets = phaseOffsets = allocate_memory(mesh->numVertices, 1, 0, 0);
        ripple->heights = allocate_memory(mesh->numVertices * sizeof(f32), 1, 0, 0);
        heights = ripple->heights; // matching: IDO reloads heights from the struct only with this split form
        ripple->followRates = followRates = allocate_memory(mesh->numVertices * sizeof(f32), 1, 0, 0);
        ripple->falloffs = falloffs = allocate_memory(mesh->numVertices * sizeof(f32), 1, 0, 0);
        radius = (mesh->moveData.word >> 16) & 0xFFFF;
        if (radius == 0.0f) {
            invRadius = 1.0f;
        } else {
            invRadius = 1.0f / radius;
        }
        for (i = 0; i < mesh->numVertices; i++) {
            phaseOffsets[i] = func_150ADA20() % 255;
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
        for (i = 0; i < mesh->numVertices; i++) {
            /* vertices at the same position get the same offset, so they move together */
            phaseOffsets[i] = func_150ADA20();
            for (j = 0; j < mesh->numVertices; j++) {
                if (base[i].ob[0] == base[j].ob[0] && base[i].ob[1] == base[j].ob[1] && base[i].ob[2] == base[j].ob[2]) {
                    phaseOffsets[j] = phaseOffsets[i];
                }
            }
        }
        ripple->wave = 0.0f;
        ripple->active = 0;
        for (i = 0; i < mesh->numVertices; i++) {
            heights[i] = 0.0f;
        }
        ripple->unk20 = 0;
        ripple->unk24 = 0;
    }
    ripple = WORK(mesh)->ripple;
    phaseOffsets = ripple->phaseOffsets;
    heights = ripple->heights;
    followRates = ripple->followRates;
    falloffs = ripple->falloffs;
    if ((mesh->task_flag & TASK_FLAG_STATE) == TASK_FLAG_ON) {
        prop_startRipple(mesh);
        mesh->task_flag &= ~TASK_FLAG_STATE;
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
        ripple->wave = math_sinBinAngle(ripple->phase);
        ripple->wave = ripple->amplitude * ripple->wave;
        if (ripple->amplitude <= 3.0f) {
            ripple->wave = 0.0f;
            ripple->active = 0;
        }
        for (i = 0; i < mesh->numVertices; i++) {
            f32 delta = ripple->wave - heights[i];

            heights[i] += delta * followRates[i];
        }
    }
    gfx_getObjectModelMtx(matrix, mesh);
    out = &mesh->vertices[D_800BE9C0]->v; /* this frame's buffer */
    lanePhases[0] = WORK(mesh)->wobblePhases & 0xFF;
    lanePhases[1] = (WORK(mesh)->wobblePhases >> 8) & 0xFF;
    lanePhases[2] = (WORK(mesh)->wobblePhases >> 16) & 0xFF;
    lanePhases[3] = (WORK(mesh)->wobblePhases >> 24) & 0xFF;
    offsetY = 0;
    for (i = 0; i < mesh->numVertices; i++, out++, base++) {
        if (!(out->flag & 0x4000)) {
            lane = phaseOffsets[i] & 3;
            angle = phaseOffsets[i] + lanePhases[lane];
            if (phaseOffsets[i] & 0x80) {
                angle = -angle;
            }
            /* u8-angle trig lookups (cosine / sine, judging by the X/Z circle they trace) */
            offsetX = math_sinBinAngle(angle) * wobbleSize;
            offsetZ = math_cosBinAngle(angle) * wobbleSize;
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
    WORK(mesh)->wobblePhases = lanePhases[0] | (lanePhases[1] << 8) | (lanePhases[2] << 16) | (lanePhases[3] << 24);
    prop_animateTexture(mesh, &ripple->unk20, &ripple->unk24);
}
