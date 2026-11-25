#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A level object whose vertices a wave deformer moves (func_151149AC returns one; see Prop in game_10B7D0.c). */
typedef struct {
    /* 0x00 */ f32 rotation[3];
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ struct { s16 x, y, z; } position;
    /* 0x16 */ u16 vertexCount;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ Vtx *frameVerts[2]; // deformed copies, indexed by the frame buffer D_800BE9C0
    /* 0x28 */ Vtx *baseVerts;
    /* 0x2C */ f32 scale[3];
    /* 0x38 */ u8 pad38[0x37];
    /* 0x6F */ u8 unk6F;
} WaveObject;

/* The part of a WaveDeformer that the func_150A6FA0 family of vertex routines reads. */
typedef struct {
    /* 0x00 */ f32 falloff;   // 1 / (radius / 100)^2
    /* 0x04 */ f32 amplitude;
    /* 0x08 */ u16 radius;
    /* 0x0A */ s16 center[3]; // relative to the object's position
    /* 0x10 */ u16 phase;     // 12-bit; the routines advance it by speed every frame
    /* 0x12 */ s8 speed;
    /* 0x13 */ s8 phaseStep;  // phase offset per 1/100 unit of distance from the center
    /* 0x14 */ u8 unk14[6];   // only used by mode 1
    /* 0x1A */ u8 pad1A[2];
} WaveParams;

/* WaveDeformer.flags */
#define WAVE_MODE_MASK 3     // which vertex routine runs (0, 1 or 2)
#define WAVE_WEIGHTED 4      // per-vertex falloff weights
#define WAVE_DIRECTIONAL 0x10 // phase follows distance along a heading instead of from the center

/* Moves an object's vertices in a sine wave (one entry of D_800DD410, size 0x2C). */
typedef struct {
    /* 0x00 */ u8 objectId;
    /* 0x01 */ u8 flags;
    /* 0x02 */ u8 sharesObject; // an earlier deformer already targets this object
    /* 0x03 */ u8 pad3;
    /* 0x04 */ WaveParams params;
    /* 0x20 */ f32 *vertexWeight; // WAVE_WEIGHTED only
    /* 0x24 */ s8 *vertexPhase;
    /* 0x28 */ u8 *vertexAngle;   // mode 2 only; byte angle (256 per turn)
} WaveDeformer;

/* variables.h declares D_800DD410 as s32[2]; it holds the WaveDeformer pointers, D_800DD418 counts them. */
#define gWaveDeformers ((WaveDeformer **)D_800DD410)

extern WaveObject *func_151149AC(u8);
void *allocate_memory(s32, s32, s32, s32);
extern void func_1511490C(f32 mtx[4][4], WaveObject *obj);
extern void func_150A7960(f32 *mtx, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);
extern void func_150A6FA0(Vtx *dst, u16 count, WaveParams *params, s8 *phase, f32 *weight, Vtx *src);
extern void func_150A70C0(Vtx *dst, u16 count, WaveParams *params, s8 *phase, Vtx *src);
extern void func_150A71C8(Vtx *dst, u16 count, WaveParams *params, u8 *unk, s8 *phase, Vtx *src);
extern void func_150A7360(Vtx *dst, u16 count, WaveParams *params, u8 *unk, s8 *phase, f32 *weight, Vtx *src);
extern void func_150A751C(Vtx *dst, u16 count, WaveParams *params, u8 *angle, s8 *phase, f32 *weight, Vtx *src);

/**
 * Creates a wave deformer for object `objectId` centred at (x, y, z) and precomputes each vertex's
 * phase offset (and, with the matching flags, its falloff weight and its angle around the center).
 */
void func_15177410(u8 flags, u8 objectId, s16 x, s16 y, s16 z, f32 heading, u16 radius, f32 amplitude,
                   s8 speed, s8 phaseStep, u8 argA, u8 argB, u8 argC, u8 argD, u8 argE, u8 argF) {
    WaveDeformer *deformer;
    WaveObject *obj;
    WaveDeformer *other;
    s32 i;
    s8 shared;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 center[3];
    f32 scale;
    s32 absDist;
    s32 dist;
    f32 distSq;
    f32 cosH;
    f32 sinH;
    f32 angle;

    deformer = allocate_memory(sizeof(WaveDeformer), 1, 0, 0);
    if (deformer != NULL) {
        u8 headingAngle;

        gWaveDeformers[D_800DD418++] = deformer;
        obj = func_151149AC(objectId);
        shared = FALSE;
        deformer->objectId = objectId;
        deformer->flags = flags;
        for (i = 0; i < D_800DD418 - 1; i++) {
            other = gWaveDeformers[i];
            if (other->objectId == objectId) {
                shared = TRUE;
                break;
            }
        }
        deformer->sharesObject = shared;
        center[0] = x - obj->position.x;
        center[1] = y - obj->position.y;
        center[2] = z - obj->position.z;
        deformer->params.center[0] = center[0];
        deformer->params.center[1] = center[1];
        deformer->params.center[2] = center[2];
        deformer->params.radius = radius;
        deformer->params.amplitude = amplitude;
        deformer->params.phase = 0;
        deformer->params.speed = speed;
        deformer->params.phaseStep = phaseStep;
        if (radius != 0) {
            scale = 0.01f;
            deformer->params.falloff = 1.0f / ((radius * scale) * (radius * scale));
        } else {
            deformer->params.falloff = 0.0f;
            scale = 0.01f;
        }
        if ((flags & WAVE_MODE_MASK) == 1) {
            deformer->params.unk14[0] = argA;
            deformer->params.unk14[1] = argB;
            deformer->params.unk14[2] = argC;
            deformer->params.unk14[3] = argD;
            deformer->params.unk14[4] = argE;
            deformer->params.unk14[5] = argF;
        }
        deformer->vertexPhase = allocate_memory(obj->vertexCount, 1, 0, 0);
        if ((flags & WAVE_MODE_MASK) == 2) {
            deformer->vertexAngle = allocate_memory(obj->vertexCount, 1, 0, 0);
        }
        if ((flags & WAVE_WEIGHTED) == WAVE_WEIGHTED) {
            deformer->vertexWeight = allocate_memory(obj->vertexCount * sizeof(f32), 1, 0, 0);
        }
        if ((flags & WAVE_DIRECTIONAL) == WAVE_DIRECTIONAL) {
            angle = -heading * 0.0174532924f; // degrees to radians
            sinH = sinf(angle);
            cosH = cosf(angle);
            headingAngle = heading * 0.7111111283f; // degrees to 256 per turn
        }
        for (i = 0; i < obj->vertexCount; i++) {
            if ((flags & WAVE_DIRECTIONAL) == 0) {
                dx = obj->baseVerts[i].v.ob[0] - center[0];
                dx = dx * dx;
                dy = obj->baseVerts[i].v.ob[1] - center[1];
                dz = obj->baseVerts[i].v.ob[2] - center[2];
                dy = dy * dy;
                dz = dz * dz;
                distSq = dx + dy + dz;
                if (distSq != 0) {
                    distSq = sqrtf(distSq);
                }
                absDist = distSq * scale;
                dist = absDist;
            } else if ((flags & WAVE_DIRECTIONAL) == WAVE_DIRECTIONAL) {
                dx = obj->baseVerts[i].v.ob[0] - center[0];
                dz = obj->baseVerts[i].v.ob[2] - center[2];
                dz = -dx * sinH + dz * cosH;
                absDist = dz * scale;
                dist = absDist;
                if (absDist < 0) {
                    absDist = -absDist;
                }
            }
            deformer->vertexPhase[i] = phaseStep * dist;
            if ((flags & WAVE_WEIGHTED) == WAVE_WEIGHTED) {
                absDist = absDist - radius * scale;
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
                    u8 vertexAngle;
                    vertexAngle = func_150484A0(dx, dz) * 40.7436637878f; // radians to 256 per turn
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
void func_15177A94(void) {
    s32 i;
    WaveDeformer *deformer;
    WaveObject *obj;
    Vtx *src;
    f32 mtx[4][4];
    f32 x;
    f32 y;
    f32 z;
    s32 j;

    for (i = 0; i < D_800DD418; i++) {
        deformer = gWaveDeformers[i];
        obj = func_151149AC(deformer->objectId);
        if (obj->unk6F & 0xF) {
            // the first deformer on an object starts from its base vertices, later ones stack
            if (!deformer->sharesObject) {
                src = obj->baseVerts;
            } else {
                src = obj->frameVerts[D_800BE9C0];
            }
            if ((deformer->flags & WAVE_MODE_MASK) == 1) {
                if ((deformer->flags & WAVE_WEIGHTED) == WAVE_WEIGHTED) {
                    func_150A7360(obj->frameVerts[D_800BE9C0], obj->vertexCount, &deformer->params,
                                  deformer->params.unk14, deformer->vertexPhase, deformer->vertexWeight, src);
                } else {
                    func_150A71C8(obj->frameVerts[D_800BE9C0], obj->vertexCount, &deformer->params,
                                  deformer->params.unk14, deformer->vertexPhase, src);
                }
            } else if ((deformer->flags & WAVE_MODE_MASK) == 0) {
                if ((deformer->flags & WAVE_WEIGHTED) == WAVE_WEIGHTED) {
                    func_150A6FA0(obj->frameVerts[D_800BE9C0], obj->vertexCount, &deformer->params,
                                  deformer->vertexPhase, deformer->vertexWeight, src);
                } else {
                    func_150A70C0(obj->frameVerts[D_800BE9C0], obj->vertexCount, &deformer->params,
                                  deformer->vertexPhase, src);
                }
            } else if ((deformer->flags & WAVE_MODE_MASK) == 2) {
                if ((deformer->flags & WAVE_WEIGHTED) == WAVE_WEIGHTED) {
                    func_150A751C(obj->frameVerts[D_800BE9C0], obj->vertexCount, &deformer->params,
                                  deformer->vertexAngle, deformer->vertexPhase, deformer->vertexWeight, src);
                }
            }
        }
    }
    for (i = 0; i < D_800DD418; i++) {
        deformer = gWaveDeformers[i];
        if (func_151149AC(deformer->objectId)->unk6F & 0xF) {
            j = 0;
            if (!deformer->sharesObject) {
                obj = func_151149AC(deformer->objectId);
                if (obj->rotation[0] == 0.0f && obj->rotation[1] == 0.0f && obj->rotation[2] == 0.0f &&
                    obj->scale[0] == 1.0f && obj->scale[1] == 1.0f && obj->scale[2] == 1.0f) {
                    // no rotation or scale: just translate (y already holds the wave offset)
                    for (j = 0; j < obj->vertexCount; j++) {
                        obj->frameVerts[D_800BE9C0][j].v.ob[0] = obj->baseVerts[j].v.ob[0] + obj->position.x;
                        obj->frameVerts[D_800BE9C0][j].v.ob[1] += obj->position.y;
                        obj->frameVerts[D_800BE9C0][j].v.ob[2] = obj->baseVerts[j].v.ob[2] + obj->position.z;
                    }
                } else {
                    func_1511490C(mtx, obj);
                    if (i) {}
                    for (j = 0; j < obj->vertexCount; j++) {
                        func_150A7960((f32 *)mtx, obj->baseVerts[j].v.ob[0], obj->frameVerts[D_800BE9C0][j].v.ob[1],
                                      obj->baseVerts[j].v.ob[2], &x, &y, &z);
                        obj->frameVerts[D_800BE9C0][j].v.ob[0] = x;
                        obj->frameVerts[D_800BE9C0][j].v.ob[1] = y;
                        obj->frameVerts[D_800BE9C0][j].v.ob[2] = z;
                    }
                }
            }
        }
    }
}
