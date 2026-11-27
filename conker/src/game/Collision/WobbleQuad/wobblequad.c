#include <ultra64.h>
#include <math.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"

/* The data of a wobbling model particle (gModelParticleUpdateFuncs[11], collision_updateWobble) after its
 * ModelParticle (MODEL_PARTICLE_DATA). */
typedef struct WobblePhases {
    /* 0x0 */ u8 pad0[8];
    /* 0x8 */ f32 phaseX; /* radians; rot.x = sin(phaseX) * 3.086 */
    /* 0xC */ f32 phaseZ; /* radians; rot.z = sin(phaseZ) * 5.914 */
} WobblePhases;

typedef struct WobbleModelParticle {
    /* 0x000 */ ModelParticle base;
    /* 0x170 */ WobblePhases wobble;
} WobbleModelParticle;

/* A quad particle (gQuadParticleVertexFuncs[21], collision_buildTransformedUprightQuad) that stands upright at
 * head.def.pos in the frame of the model particle `parent`. */
typedef struct UprightQuad {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ u8 pad110[0x50];
    /* 0x160 */ ModelParticle *parent;
} UprightQuad;

s32 collision_updateWobble(WobbleModelParticle *particle) {
    WobblePhases *phases;

    particle->wobble.phaseX += 0.065f * gFrameDelta;
    particle->wobble.phaseZ += 0.0484f * gFrameDelta;
    phases = &particle->wobble;
    phases->phaseX = math_wrapRadians(particle->wobble.phaseX);
    phases->phaseZ = math_wrapRadians(phases->phaseZ);
    particle->base.rot.x = sinf(phases->phaseX) * 3.0860002f;
    particle->base.rot.z = sinf(phases->phaseZ) * 5.9140005f;
    return 1;
}

s32 func_150AE35C(s32 particle) {
    return 1;
}

s16 *collision_buildTransformedUprightQuad(UprightQuad *particle, s16 viewport) {
    s16 *vertices;
    s16 *ret;
    f32 offsetX;
    f32 offsetZ;
    f32 z;
    f32 y;
    f32 x;
    f32 pad84[2];
    MtxF mtx;
    u8 fresh;

    particle_getViewBuffer(particle->head.vtxBuffers, viewport, 0x40, &vertices, &fresh);
    ret = vertices;
    if (vertices) {
        if (fresh) {
            memcpy(particle->head.vtxBuffers[viewport], particle->head.vtxTemplate, 0x40);
            memcpy(particle->head.vtxBuffers[viewport] + 4, particle->head.vtxTemplate, 0x40);
        }
    } else {
        return 0;
    }

    math_mtxFRotScaleXZ(mtx, particle->parent->sizeXZ, particle->parent->sizeY, particle->parent->rot.x, particle->parent->rot.y, particle->parent->rot.z, particle->parent->pos.x, particle->parent->pos.y, particle->parent->pos.z);
    math_transformPoint(mtx, particle->head.def.pos.x, particle->head.def.pos.y, particle->head.def.pos.z, &x, &y, &z);

    offsetX = gCameraAngleCosines[viewport] * particle->head.def.width;
    offsetZ = gCameraAngleSines[viewport] * particle->head.def.width;

    vertices[3] = 0;
    vertices[0xB] = 0;
    vertices[0x13] = 0;
    vertices[0x1B] = 0;
    vertices[0] = vertices[0x18] = x + offsetX;
    vertices[1] = vertices[9] = y;
    vertices[2] = vertices[0x1A] = z - offsetZ;
    vertices[8] = vertices[0x10] = x - offsetX;
    vertices[0x11] = vertices[0x19] = y + particle->head.def.height;
    vertices[0xA] = vertices[0x12] = z + offsetZ;

    return ret;
}
