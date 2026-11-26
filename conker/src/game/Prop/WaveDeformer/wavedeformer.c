#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init/Sched/SchedInit/schedinit.c); conker/prop.h describes it. */
WaveDeformer *gWaveDeformers[2];
u8 gNumWaveDeformers;

#include "functions.h"
#include "variables.h"

/* WaveDeformer.flags */
#define WAVE_MODE_MASK 3     // which vertex routine runs (0, 1 or 2)
#define WAVE_WEIGHTED 4      // per-vertex falloff weights
#define WAVE_DIRECTIONAL 0x10 // phase follows distance along a heading instead of from the center

/**
 * Creates a wave deformer for object `objectId` centred at (x, y, z) and precomputes each vertex's
 * phase offset (and, with the matching flags, its falloff weight and its angle around the center).
 */
void prop_createWaveDeformer(u8 flags, u8 objectId, s16 x, s16 y, s16 z, f32 heading, u16 radius, f32 amplitude,
                   s8 speed, s8 phaseStep, u8 argA, u8 argB, u8 argC, u8 argD, u8 argE, u8 argF) {
    WaveDeformer *deformer;
    WorldObject *obj;
    WaveDeformer *other;
    s32 i;
    s8 shared;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 center[3];
    f32 toUnits;
    s32 absDist;
    s32 dist;
    f32 distance;
    f32 cosH;
    f32 sinH;
    f32 radians;

    deformer = allocate_memory(sizeof(WaveDeformer), 1, 0, 0);
    if (deformer != NULL) {
        u8 headingAngle;

        gWaveDeformers[gNumWaveDeformers++] = deformer;
        obj = gfx_findObjectById(objectId);
        shared = FALSE;
        deformer->objectId = objectId;
        deformer->flags = flags;
        for (i = 0; i < gNumWaveDeformers - 1; i++) {
            other = gWaveDeformers[i];
            if (other->objectId == objectId) {
                shared = TRUE;
                break;
            }
        }
        deformer->sharesObject = shared;
        center[0] = x - obj->x;
        center[1] = y - obj->y;
        center[2] = z - obj->z;
        deformer->params.center[0] = center[0];
        deformer->params.center[1] = center[1];
        deformer->params.center[2] = center[2];
        deformer->params.radius = radius;
        deformer->params.amplitude = amplitude;
        deformer->params.phase = 0;
        deformer->params.speed = speed;
        deformer->params.phaseStep = phaseStep;
        toUnits = 0.01f; // distances are measured in hundreds of model units
        if (radius != 0) {
            deformer->params.falloff = 1.0f / ((radius * toUnits) * (radius * toUnits));
        } else {
            deformer->params.falloff = 0.0f;
        }
        if ((flags & WAVE_MODE_MASK) == 1) {
            deformer->params.unk14[0] = argA;
            deformer->params.unk14[1] = argB;
            deformer->params.unk14[2] = argC;
            deformer->params.unk14[3] = argD;
            deformer->params.unk14[4] = argE;
            deformer->params.unk14[5] = argF;
        }
        deformer->vertexPhase = allocate_memory(obj->numVertices, 1, 0, 0);
        if ((flags & WAVE_MODE_MASK) == 2) {
            deformer->vertexAngle = allocate_memory(obj->numVertices, 1, 0, 0);
        }
        if ((flags & WAVE_WEIGHTED) == WAVE_WEIGHTED) {
            deformer->vertexWeight = allocate_memory(obj->numVertices * sizeof(f32), 1, 0, 0);
        }
        if ((flags & WAVE_DIRECTIONAL) == WAVE_DIRECTIONAL) {
            radians = -heading * 0.0174532924f; // degrees to radians
            sinH = sinf(radians);
            cosH = cosf(radians);
            headingAngle = heading * 0.7111111283f; // degrees to 256 per turn
        }
        for (i = 0; i < obj->numVertices; i++) {
            if ((flags & WAVE_DIRECTIONAL) == 0) {
                dx = obj->srcVertices[i].v.ob[0] - center[0];
                dx = dx * dx;
                dy = obj->srcVertices[i].v.ob[1] - center[1];
                dz = obj->srcVertices[i].v.ob[2] - center[2];
                dy = dy * dy;
                dz = dz * dz;
                distance = dx + dy + dz;
                if (distance != 0) {
                    distance = sqrtf(distance);
                }
                absDist = distance * toUnits;
                dist = absDist;
            } else if ((flags & WAVE_DIRECTIONAL) == WAVE_DIRECTIONAL) {
                dx = obj->srcVertices[i].v.ob[0] - center[0];
                dz = obj->srcVertices[i].v.ob[2] - center[2];
                dz = -dx * sinH + dz * cosH;
                absDist = dz * toUnits;
                dist = absDist;
                if (absDist < 0) {
                    absDist = -absDist;
                }
            }
            deformer->vertexPhase[i] = phaseStep * dist;
            if ((flags & WAVE_WEIGHTED) == WAVE_WEIGHTED) {
                absDist = absDist - radius * toUnits;
                if (absDist >= 0) {
                    deformer->vertexWeight[i] = 0.0f;
                } else {
                    deformer->vertexWeight[i] = deformer->params.falloff * amplitude * (absDist * absDist);
                }
            }
            if ((flags & WAVE_MODE_MASK) == 2) {
                if ((flags & WAVE_DIRECTIONAL) == WAVE_DIRECTIONAL) {
                    deformer->vertexAngle[i] = headingAngle;
                } else {
                    u8 vertexAngle; // matching: storing through a u8 local, not directly
                    vertexAngle = math_atan2(dx, dz) * 40.7436637878f; // radians to 256 per turn
                    deformer->vertexAngle[i] = vertexAngle;
                }
            }
        }
    }
}

/**
 * Runs every wave deformer's vertex routine on its object's current frame vertices, then
 * re-applies the object's transform to the vertices of each object that a deformer targets.
 */
void prop_updateWaveDeformers(void) {
    s32 i;
    WaveDeformer *deformer;
    WorldObject *obj;
    Vtx *src;
    MtxF mtx;
    f32 x;
    f32 y;
    f32 z;
    s32 j;

    for (i = 0; i < gNumWaveDeformers; i++) {
        deformer = gWaveDeformers[i];
        obj = gfx_findObjectById(deformer->objectId);
        if (obj->viewFlags & 0xF) {
            // the first deformer on an object starts from its base vertices, later ones stack
            if (!deformer->sharesObject) {
                src = obj->srcVertices;
            } else {
                src = obj->vertices[D_800BE9C0];
            }
            if ((deformer->flags & WAVE_MODE_MASK) == 1) {
                if ((deformer->flags & WAVE_WEIGHTED) == WAVE_WEIGHTED) {
                    func_150A7360(obj->vertices[D_800BE9C0], obj->numVertices, &deformer->params,
                                  deformer->params.unk14, deformer->vertexPhase, deformer->vertexWeight, src);
                } else {
                    func_150A71C8(obj->vertices[D_800BE9C0], obj->numVertices, &deformer->params,
                                  deformer->params.unk14, deformer->vertexPhase, src);
                }
            } else if ((deformer->flags & WAVE_MODE_MASK) == 0) {
                if ((deformer->flags & WAVE_WEIGHTED) == WAVE_WEIGHTED) {
                    func_150A6FA0(obj->vertices[D_800BE9C0], obj->numVertices, &deformer->params,
                                  deformer->vertexPhase, deformer->vertexWeight, src);
                } else {
                    func_150A70C0(obj->vertices[D_800BE9C0], obj->numVertices, &deformer->params,
                                  deformer->vertexPhase, src);
                }
            } else if ((deformer->flags & WAVE_MODE_MASK) == 2) {
                if ((deformer->flags & WAVE_WEIGHTED) == WAVE_WEIGHTED) {
                    func_150A751C(obj->vertices[D_800BE9C0], obj->numVertices, &deformer->params,
                                  deformer->vertexAngle, deformer->vertexPhase, deformer->vertexWeight, src);
                }
            }
        }
    }
    for (i = 0; i < gNumWaveDeformers; i++) {
        deformer = gWaveDeformers[i];
        if (gfx_findObjectById(deformer->objectId)->viewFlags & 0xF) {
            if (!deformer->sharesObject) {
                obj = gfx_findObjectById(deformer->objectId);
                if (obj->t == 0.0f && obj->u == 0.0f && obj->v == 0.0f &&
                    obj->xScale == 1.0f && obj->yScale == 1.0f && obj->zScale == 1.0f) {
                    // no rotation or scale: just translate (y already holds the wave offset)
                    for (j = 0; j < obj->numVertices; j++) {
                        obj->vertices[D_800BE9C0][j].v.ob[0] = obj->srcVertices[j].v.ob[0] + obj->x;
                        obj->vertices[D_800BE9C0][j].v.ob[1] += obj->y;
                        obj->vertices[D_800BE9C0][j].v.ob[2] = obj->srcVertices[j].v.ob[2] + obj->z;
                    }
                } else {
                    gfx_getObjectModelMtx(mtx, obj);
                    if (i) {} // matching: an extra use of i makes IDO keep it in a register and spill the list cursor
                    for (j = 0; j < obj->numVertices; j++) {
                        func_150A7960(mtx, obj->srcVertices[j].v.ob[0], obj->vertices[D_800BE9C0][j].v.ob[1],
                                      obj->srcVertices[j].v.ob[2], &x, &y, &z);
                        obj->vertices[D_800BE9C0][j].v.ob[0] = x;
                        obj->vertices[D_800BE9C0][j].v.ob[1] = y;
                        obj->vertices[D_800BE9C0][j].v.ob[2] = z;
                    }
                }
            }
        }
    }
}
