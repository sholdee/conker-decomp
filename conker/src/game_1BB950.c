#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void *memcpy(void *, const void *, u32);

// The string literals of func_1518E66C and func_1518E6D4: they lie in the front-end .rodata of the unit that
// begins at game_1BA670 (before game_1BA670's literals), so this file cannot own them until the unit is merged.
extern char D_800A7460[];
extern char D_800A749C[];

/* Drops the emitter's particles and detaches it from its owner. */
void particle_detachActorEmitter(ActorEmitterProcess *emitter) {
    particle_freeActorEmitterParticles(emitter);
    effect_detachHandle(emitter, emitter->params.owner);
}

/* Destroy handler of an emitter process. */
void particle_destroyActorEmitter(ActorEmitterProcess *emitter) {
    particle_detachActorEmitter(emitter);
    effect_defaultDestroy(&emitter->header);
}

/* Free handler of an emitter process. */
void particle_freeActorEmitter(ActorEmitterProcess *emitter) {
    particle_detachActorEmitter(emitter);
    effect_defaultFree(&emitter->header);
}

/* Particle spawn callback: texture 0x13 or 0x14 at random, opaque black. */
void particle_pickBlackTexture(u8 *texture, u8 *flip, u8 *r, u8 *g, u8 *b, u8 *alpha, u16 *renderModes) {
    *flip = 2;
    if (func_150ADA20() & 1) {
        *flip |= 1;
    }

    switch (func_150ADA20() & 1) {
    case 0:
        *texture = 0x13;
        break;
    case 1:
        *texture = 0x14;
        break;
    }

    *r = 0;
    *g = 0;
    *b = 0;
    *alpha = 255;
    *renderModes = 0x301;
}

/* Particle spawn callback: texture 0x16 tinted with palette 3 or 4 at random. */
void particle_pickPaletteTexture(u8 *texture, u8 *flip, u8 *r, u8 *g, u8 *b, u8 *alpha, u16 *renderModes) {
    if (func_150ADA20() & 1) {
        *flip |= 1;
    }
    *texture = 0x16;
    math_pickPaletteColour((func_150ADA20() & 1) ? 3 : 4, r, g, b);
    *alpha = 200;
    *renderModes = 0x401;
}

/* Expire callback: starts a kind 3 emitter on the owner and keeps this one alive for 128 more frames. */
s32 func_1518E66C(ActorEmitterProcess *emitter) {
    particle_startActorEmitter(emitter->params.owner, 3, 0, 0, emitter->header.owner, emitter->header.kind, D_800A7460);
    emitter->params.lifetime = 128;
    emitter->params.expireCb = -1;
    return 0;
}

/* Expire callback: starts a kind 4 emitter on the owner and keeps this one alive for 128 more frames. */
s32 func_1518E6D4(ActorEmitterProcess *emitter) {
    particle_startActorEmitter(emitter->params.owner, 4, 0, 0, emitter->header.owner, emitter->header.kind, D_800A749C);
    emitter->params.lifetime = 128;
    emitter->params.expireCb = -1;
    return 0;
}
/*
 * Vertex function: builds the particle's quad in its vertex buffer for viewport vp, tilted by how fast
 * it moves sideways on screen (D_800DD1E8 / D_800DD1D8 are the viewport's view direction).
 */
Vtx *particle_buildActorEmitterQuad(ActorEmitterParticle *p, s16 vp) {
    Vtx *vtx;
    Vtx *ret;
    f32 halfWidthCos;
    f32 viewZ;
    f32 viewX;
    f32 sinAngle;
    f32 cosAngle;
    f32 halfWidthSin;
    f32 t;
    f32 heightSin;
    f32 heightCos;
    f32 speed;
    f32 angle;
    s32 negative;
    u8 fresh;

    particle_getViewBuffer(p->head.vtxBuffers, vp, 4 * sizeof(Vtx), &vtx, &fresh);
    ret = vtx;
    // matching: IDO lays out the branches as in the target only with the NULL return in the else
    if (vtx != NULL) {
        if (fresh) {
            memcpy(p->head.vtxBuffers[vp], p->head.vtxTemplate, sizeof(p->head.vtxTemplate));
            memcpy(p->head.vtxBuffers[vp] + 4, p->head.vtxTemplate, sizeof(p->head.vtxTemplate));
        }
    } else {
        return NULL;
    }
    viewZ = D_800DD1D8[vp];
    viewX = D_800DD1E8[vp];
    negative = -1; // matching: the target loads -1, not 1, as the true value
    speed = (((p->head.def.rot.x - p->head.def.pos.x) * viewX) - (viewZ * (p->head.def.rot.z - p->head.def.pos.z))) * gFrameDeltaInv;
    if (speed < 0.0f) {
        speed = -speed;
        negative = FALSE;
    }
    angle = sqrtf(speed) * p->data.stretch;
    if (angle > 1.570796371f) {
        angle = 1.570796371f;
    }
    if (negative) {
        angle = -angle;
    }
    {
        u8 angle256;

        angle256 = angle * 40.74366379f; // radians to 256ths of a turn
        sinAngle = math_sinBinAngle(angle256);
        cosAngle = math_cosBinAngle(angle256);
    }
    halfWidthCos = p->head.def.width * cosAngle;
    halfWidthSin = p->head.def.width * sinAngle;
    heightSin = p->head.def.height * sinAngle;
    heightCos = p->head.def.height * cosAngle;
    vtx[0].v.ob[0] = p->head.def.pos.x + halfWidthCos * viewX;
    vtx[0].v.ob[1] = p->head.def.pos.y + halfWidthSin;
    vtx[0].v.ob[2] = p->head.def.pos.z - halfWidthCos * viewZ;
    t = -halfWidthCos;
    vtx[1].v.ob[0] = p->head.def.pos.x + t * viewX;
    vtx[1].v.ob[1] = p->head.def.pos.y - halfWidthSin;
    vtx[1].v.ob[2] = p->head.def.pos.z - t * viewZ;
    t = -(halfWidthCos + heightSin);
    vtx[2].v.ob[0] = p->head.def.pos.x + t * viewX;
    vtx[2].v.ob[1] = (p->head.def.pos.y + heightCos) - halfWidthSin;
    vtx[2].v.ob[2] = p->head.def.pos.z - t * viewZ;
    t = halfWidthCos - heightSin;
    vtx[3].v.ob[0] = p->head.def.pos.x + t * viewX;
    vtx[3].v.ob[1] = p->head.def.pos.y + heightCos + halfWidthSin;
    vtx[3].v.ob[2] = p->head.def.pos.z - t * viewZ;
    return ret;
}
