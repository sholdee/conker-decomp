#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .data: the bolt effect type's render setup and teardown display lists (its entry in D_8008C1A8
 * points to them), then the bolt handler tables. */
Gfx gBoltRenderSetupDl[] = {
    gsDPPipeSync(),
    gsSPMatrix(&gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx gBoltRenderTeardownDl[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};
/* Per BoltDesc.updateKind. */
static s32 (*gBoltUpdateHandlers[])(BoltObj *bolt, u8 *moved) = { beam_followBoundsBolt, beam_followBoneBolt };
/* Per BoltDesc.kind: cleanup, then message handlers. */
static void (*gBoltCleanupHandlers[])(BoltObj *bolt) = { NULL, beam_unlinkChainBolt, NULL, NULL };
static void (*gBoltMsgHandlers[])(BoltObj *bolt, s32 arg1, u8 arg2) = { NULL, NULL, beam_boundsBoltMessage, beam_boneBoltMessage };
/* Per BoltChain.kind: message handlers. */
static void (*gBoltChainMsgHandlers[])(EffectHeader *obj, s32 arg1, u8 arg2) = { NULL, effect_boltRingMessage, func_1510550C };

/*
 * Allocates a bolt for desc with extraSize bytes of user data, samples and jitters it, and spawns its
 * end flares. arg2 and arg3 go through to the allocators; callers pass their own header bytes 0xC and 0x1.
 */
BoltObj *beam_newBolt(BoltDesc *desc, s32 extraSize, u8 arg2, s32 arg3) {
    BoltObj *bolt;
    s32 segments;
    u8 i;
    s32 points;

    segments = desc->segments;
    points = desc->segments + 2;
    bolt = effect_newObject(0x40, arg3, segments * sizeof(BoltSample) + points * sizeof(Vec3f) + extraSize + sizeof(BoltObj), 1, arg2, 1);
    if (bolt == NULL) {
        return NULL;
    }
    memcpy(&bolt->desc, desc, sizeof(BoltDesc));
    bolt->userData = (u8 *)bolt + segments * sizeof(BoltSample) + points * sizeof(Vec3f) + sizeof(BoltObj);
    bolt->step = 1.0f / (desc->segments + 1);
    bolt->rejitterTimer = func_150ADA20() % (desc->rejitterRange + 1) + desc->rejitterBase;
    beam_sampleBolt(bolt);
    beam_jitterBolt(bolt);
    bolt->flares[0] = NULL;
    bolt->flares[1] = NULL;
    bzero(bolt->vtxBuf, sizeof(bolt->vtxBuf));
    /* three vertices per joint point */
    bolt->vtxBufSize = (bolt->desc.segments + 2) * 3 * sizeof(Vtx);
    {
        ParticleDesc spawn;

        spawn.texture = 0x2B;
        spawn.renderMode = 0x4404;
        spawn.unk0 = 0x200005;
        spawn.unk4 = 0;
        spawn.timer = 300;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.r = 0xFF;
        spawn.g = 0xFF;
        spawn.b = 0xFF;
        spawn.a = 0xFF;
        spawn.r2 = desc->color.red;
        spawn.g2 = desc->color.green;
        spawn.b2 = desc->color.blue;
        spawn.alpha = desc->color.alpha;
        spawn.brightness = 0xFF;
        spawn.offset = gZeroVec3f;
        spawn.vel = gZeroVec3f;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;
        spawn.scaleStart = 1;
        spawn.flags = 0xC000;
        spawn.primColorMode = 6;
        spawn.envColorMode = 6;
        spawn.updateFunc = -1;
        spawn.deathFunc = -1;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.viewMask = 0;
        spawn.views = 0xFF;
        spawn.minSize = 0;
        spawn.gravity = 0.0f;
        spawn.scaleRate = 1.0f;
        spawn.maxSize = 1000000.0f;
        spawn.width = spawn.height = desc->width * 0.07692307979f * 200.0f;
        for (i = 0; i < 2; i++) {
            spawn.pos = desc->ctrl[(i == 0) ? 0 : 3];
            spawn.flags = (func_150ADA20() ? 0x40 : 0) | (func_150ADA20() ? 0x80 : 0);
            bolt->flares[i] = particle_newSpriteParticleWithFloor(&spawn, 0, 0, 0, arg2, arg3);
        }
    }
    return bolt;
}

/* Per-frame update: follow the owner, count down life, re-jitter on a timer, keep the flares on the end points. */
void beam_updateBolt(BoltObj *bolt) {
    s32 (*update)(BoltObj *, u8 *);
    u8 moved;
    BoltFlare *flare;

    moved = FALSE;
    if (bolt->desc.updateKind != -1) {
        update = gBoltUpdateHandlers[bolt->desc.updateKind];
        if (update != NULL && !update(bolt, &moved)) {
            effect_destroyObject(&bolt->header);
        }
    }

    if (bolt->desc.flags & 1) {
        bolt->desc.life -= D_800BE9E4;
        if (bolt->desc.life < 0) {
            effect_destroyObject(&bolt->header);
        }
    }

    if (moved) {
        beam_sampleBolt(bolt);
    }

    bolt->rejitterTimer -= D_800BE9E4;
    if (bolt->rejitterTimer < 0) {
        beam_jitterBolt(bolt);
        bolt->rejitterTimer = (func_150ADA20() % (bolt->desc.rejitterRange + 1)) + bolt->desc.rejitterBase;
    }

    flare = bolt->flares[0];
    if (flare != NULL) {
        flare->pos = bolt->desc.ctrl[0];
    }

    flare = bolt->flares[1];
    if (flare != NULL) {
        flare->pos = bolt->desc.ctrl[3];
    }
}

/* The Bezier samples that follow the bolt object. */
BoltSample *beam_getBoltSamples(BoltObj *bolt) {
    return (BoltSample *)(bolt + 1);
}

/* The jittered joint points that follow the samples. */
Vec3f *beam_getBoltJoints(BoltObj *bolt) {
    s32 segments = bolt->desc.segments;
    return (Vec3f *)((BoltSample *)(bolt + 1) + segments);
}

/* Samples the Bezier at t = step, 2 * step, ...: position, tangent and a frame around the tangent. */
void beam_sampleBolt(BoltObj *bolt) {
    BoltSample *samples;
    f32 step;
    BoltSample *out;
    s32 i;
    f32 t;
    f32 b0;
    f32 b1;
    f32 b2;
    f32 b3;
    f32 d0;
    f32 d1;
    f32 d2;
    f32 d3;

    samples = beam_getBoltSamples(bolt);
    step = bolt->step;
    t = step;
    for (i = 0; i < bolt->desc.segments; i++) {
        out = &samples[i];
        out->t = t;
        b0 = beam_bezierB0(t);
        b1 = beam_bezierB1(t);
        b2 = beam_bezierB2(t);
        b3 = beam_bezierB3(t);
        d0 = beam_bezierB0Deriv(t);
        d1 = beam_bezierB1Deriv(t);
        d2 = beam_bezierB2Deriv(t);
        d3 = beam_bezierB3Deriv(t);
        out->pos.x = bolt->desc.ctrl[0].x * b0 + bolt->desc.ctrl[1].x * b1 + bolt->desc.ctrl[2].x * b2 + bolt->desc.ctrl[3].x * b3;
        out->pos.y = bolt->desc.ctrl[0].y * b0 + bolt->desc.ctrl[1].y * b1 + bolt->desc.ctrl[2].y * b2 + bolt->desc.ctrl[3].y * b3;
        out->pos.z = bolt->desc.ctrl[0].z * b0 + bolt->desc.ctrl[1].z * b1 + bolt->desc.ctrl[2].z * b2 + bolt->desc.ctrl[3].z * b3;
        out->tangent.x = bolt->desc.ctrl[0].x * d0 + bolt->desc.ctrl[1].x * d1 + bolt->desc.ctrl[2].x * d2 + bolt->desc.ctrl[3].x * d3;
        out->tangent.y = bolt->desc.ctrl[0].y * d0 + bolt->desc.ctrl[1].y * d1 + bolt->desc.ctrl[2].y * d2 + bolt->desc.ctrl[3].y * d3;
        out->tangent.z = bolt->desc.ctrl[0].z * d0 + bolt->desc.ctrl[1].z * d1 + bolt->desc.ctrl[2].z * d2 + bolt->desc.ctrl[3].z * d3;
        if (math_buildPerpendicularBasis(&out->tangent, &out->normal, &out->binormal) == 0) {
            out->normal.x = 0.0f;
            out->normal.y = 0.0f;
            out->normal.z = 0.0f;
            out->binormal.x = 0.0f;
            out->binormal.y = 0.0f;
            out->binormal.z = 0.0f;
        }
        t += step;
    }
}

/* Cubic Bernstein basis B0(t) = (1 - t)^3. */
f32 beam_bezierB0(f32 t) {
    f32 u = 1.0f - t;
    return u * u * u;
}

/* B1(t) = 3t(1 - t)^2. */
f32 beam_bezierB1(f32 t) {
    f32 u = 1.0f - t;
    return 3.0f * t * u * u;
}

/* B2(t) = 3t^2(1 - t). */
f32 beam_bezierB2(f32 t) {
    return 3.0f * t * t * (1.0f - t);
}

/* B3(t) = t^3. */
f32 beam_bezierB3(f32 t) {
    return t * t * t;
}

/* B0'(t) = -3(1 - t)^2. */
f32 beam_bezierB0Deriv(f32 t) {
    f32 u = 1.0f - t;
    return -3.0f * u * u;
}

/* B1'(t) = 9t^2 - 12t + 3. */
f32 beam_bezierB1Deriv(f32 t) {
    return 9.0f * t * t - 12.0f * t + 3.0f;
}

/* B2'(t) = -9t^2 + 6t. */
f32 beam_bezierB2Deriv(f32 t) {
    return -9.0f * t * t + 6.0f * t;
}

/* B3'(t) = 3t^2. */
f32 beam_bezierB3Deriv(f32 t) {
    return 3.0f * t * t;
}

/* Rebuilds the joint points: the fixed end points plus each sample pushed a random distance sideways. */
void beam_jitterBolt(BoltObj *bolt) {
    BoltSample *samples;
    Vec3f *points;
    s32 i;
    s32 j;

    samples = beam_getBoltSamples(bolt);
    points = beam_getBoltJoints(bolt);
    points[0] = bolt->desc.ctrl[0];
    points[bolt->desc.segments + 1] = bolt->desc.ctrl[3];
    for (i = 1, j = 0; i < bolt->desc.segments + 1; i++, j++) {
        Vec3f *point = &points[i];
        BoltSample *sample = &samples[j];
        f32 offset1;
        f32 offset0;
        math_dirFromBinAngleXZ(func_150ADA20() & 0xFF, func_150ADA68() * bolt->desc.jitter, &offset0, &offset1);
        point->x = sample->normal.x * offset0 + sample->binormal.x * offset1 + sample->pos.x;
        point->y = sample->normal.y * offset0 + sample->binormal.y * offset1 + sample->pos.y;
        point->z = sample->normal.z * offset0 + sample->binormal.z * offset1 + sample->pos.z;
    }
}

extern s32 gEffectRenderModes[][2];

/* Draws the bolt as a camera-facing strip: three vertices (one side, centre, other side) per joint point. */
Gfx *beam_drawBolt(Gfx *gdl, BoltObj *bolt, s16 view) {
    Vec3f *camera;
    BoltObj *curve = bolt; // matching: a second copy of the bolt pointer steers IDO's register choice
    Vtx_t *vtx;
    Vec3f delta;
    Vec3f toCamera;
    Vec3f normal;
    f32 lengthSquared;
    u8 flag;
    Vec3f *points;
    s32 i;
    Vec3f previous;

    if (0) { gdl = gdl; } // matching: dead IR resets uopt alias state so the first strip reloads vtx
    particle_getViewBuffer(bolt->vtxBuf, view, bolt->vtxBufSize, &vtx, NULL);
    if (vtx == NULL) {
        return gdl;
    }
    camera = math_getCameraPos(view);
    flag = 1;
    points = beam_getBoltJoints(bolt);
    gdl = math_gfxLoadTextureFrame(gdl, &gEffectTextures[44], 0, 0, 0, 0, 0x2C, 0, 0, &flag, 3);
    gdl = math_gfxSetEnvColor(gdl, bolt->desc.color.red, bolt->desc.color.green, bolt->desc.color.blue, bolt->desc.color.alpha, &flag);
    gdl = math_gfxUpdateGeometryMode(gdl, 0x200005, 0x1F0600);
    gdl = particle_setCombineMode(gdl, 0x44, &flag);
    gdl = math_gfxSetOtherMode(gdl, gSpriteTlutMode | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, gEffectRenderModes[1][0] | gEffectRenderModes[1][1], &flag);
    delta.x = points[1].x - points[0].x;
    delta.y = points[1].y - points[0].y;
    delta.z = points[1].z - points[0].z;
    toCamera.x = points[0].x - camera->x;
    toCamera.y = points[0].y - camera->y;
    toCamera.z = points[0].z - camera->z;
    normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
    normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
    normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
    lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
    if (lengthSquared > 0.0001f) {
        f32 scale = bolt->desc.width / sqrtf(lengthSquared);

        normal.x *= scale;
        normal.y *= scale;
        normal.z *= scale;
    } else {
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
    }
    vtx->ob[0] = points[0].x + normal.x;
    vtx->ob[1] = points[0].y + normal.y;
    vtx->ob[2] = points[0].z + normal.z;
    vtx->tc[0] = 0;
    vtx->tc[1] = 0;
    vtx++;
    vtx->ob[0] = points[0].x;
    vtx->ob[1] = points[0].y;
    vtx->ob[2] = points[0].z;
    vtx->tc[0] = 0x200;
    vtx->tc[1] = 0;
    vtx++;
    vtx->ob[0] = points[0].x - normal.x;
    vtx->ob[1] = points[0].y - normal.y;
    vtx->ob[2] = points[0].z - normal.z;
    vtx->tc[0] = 0x400;
    vtx->tc[1] = 0;
    vtx++;
    previous = points[0];
    for (i = 1; i < curve->desc.segments + 2; i++) {
        Vec3f *point = &points[i];

        delta.x = previous.x - point->x;
        delta.y = previous.y - point->y;
        delta.z = previous.z - point->z;
        toCamera.x = point->x - camera->x;
        toCamera.y = point->y - camera->y;
        toCamera.z = point->z - camera->z;
        normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
        normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
        normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
        lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
        if (lengthSquared > 0.0001f) {
            f32 scale = bolt->desc.width / sqrtf(lengthSquared);

            normal.x *= scale;
            normal.y *= scale;
            normal.z *= scale;
        } else {
            normal.x = 0.0f;
            normal.y = 0.0f;
            normal.z = 0.0f;
        }
        vtx->ob[0] = point->x - normal.x;
        vtx->ob[1] = point->y - normal.y;
        vtx->ob[2] = point->z - normal.z;
        vtx->tc[0] = 0;
        vtx->tc[1] = 0;
        vtx++;
        vtx->ob[0] = point->x;
        vtx->ob[1] = point->y;
        vtx->ob[2] = point->z;
        vtx->tc[0] = 0x200;
        vtx->tc[1] = 0;
        vtx++;
        vtx->ob[0] = point->x + normal.x;
        vtx->ob[1] = point->y + normal.y;
        vtx->ob[2] = point->z + normal.z;
        vtx->tc[0] = 0x400;
        vtx->tc[1] = 0;
        vtx++;
        gSPVertex(gdl++, vtx - 6, 6, 0);
        gSP1Triangle(gdl++, 0, 1, 3, 0);
        gSP1Triangle(gdl++, 1, 4, 3, 0);
        gSP1Triangle(gdl++, 1, 2, 5, 0);
        gSP1Triangle(gdl++, 1, 5, 4, 0);
        previous = *point;
    }
    return gdl;
}

/* Destroys the bolt's resources: the kind's cleanup callback, both end flares and the vertex buffer. */
void beam_releaseBolt(BoltObj *bolt) {
    void (*cleanup)(BoltObj *);
    BoltFlare *flare;

    cleanup = gBoltCleanupHandlers[bolt->desc.kind];
    if (cleanup != NULL) {
        cleanup(bolt);
    }
    flare = bolt->flares[0];
    if (flare != NULL) {
        effect_destroyObject(&flare->header);
    }
    flare = bolt->flares[1];
    if (flare != NULL) {
        effect_destroyObject(&flare->header);
    }
    particle_freeViewBuffers(bolt->vtxBuf);
}

/* Destroys the bolt's resources, then hands it to effect_defaultDestroy. */
void beam_destroyBolt(BoltObj *bolt) {
    beam_releaseBolt(bolt);
    effect_defaultDestroy(&bolt->header);
}

/* Destroys the bolt's resources, then hands it to effect_defaultFree. */
void beam_freeBolt(BoltObj *bolt) {
    beam_releaseBolt(bolt);
    effect_defaultFree(&bolt->header);
}

/* Dispatches to the bolt kind's handler in gBoltMsgHandlers. */
void beam_boltMessage(BoltObj *bolt, s32 arg1, u8 arg2) {
    void (*fn)(BoltObj *, s32, u8) = gBoltMsgHandlers[bolt->desc.kind];
    if (fn != NULL) {
        fn(bolt, arg1, arg2);
    }
}

/* Creates a bolt chain from start to end with (1 << depth) + 1 randomly spaced points. */
BoltChainObj *beam_newBoltChain(Vec3f *start, Vec3f *end, s32 depth, BoltChainConfig *config, f32 spread, u8 kind, u8 arg6, s32 arg7) {
    BoltChainObj *obj;
    BoltChain chain;
    BoltChain *data;

    chain.count = (1 << depth) + 1;
    chain.link = NULL;
    chain.index = 0;
    chain.config = *config;
    chain.kind = kind;

    obj = effect_newTimedEffect(0, -1, 0x3E, -1, 0, 0x30, (chain.count * sizeof(Vec3f) + 0x48), arg6, arg7);
    if (obj != NULL) {
        data = &obj->chain;
        memcpy(data, &chain, sizeof(BoltChain));
        data->points = (Vec3f *)((u8 *)data + 0x48); // matching: = obj->points, but computed from data
        data->points[0] = *start;
        data->points[chain.count - 1] = *end;
        beam_placeChainPoints(obj, 0, chain.count - 1, spread);
    }
    return obj;
}

/* Recursively places the points between lo and hi on the line, each midpoint randomly within +-spread of halfway. */
void beam_placeChainPoints(BoltChainObj *obj, s16 lo, s16 hi, f32 spread) {
    BoltChain *chain = &obj->chain;
    Vec3f *a;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 t;
    s16 mid;

    if (hi - lo < 2) {
        return;
    }
    mid = lo + ((hi - lo) >> 1);
    a = &chain->points[lo];
    dx = chain->points[hi].x - chain->points[lo].x;
    dy = chain->points[hi].y - chain->points[lo].y;
    dz = chain->points[hi].z - chain->points[lo].z;
    t = func_150ADA68() * (spread + spread) + (0.5f - spread);
    chain->points[mid].x = a->x + dx * t;
    chain->points[mid].y = a->y + dy * t;
    chain->points[mid].z = a->z + dz * t;
    beam_placeChainPoints(obj, lo, mid, spread);
    beam_placeChainPoints(obj, mid, hi, spread);
}

/* Chain update: once the current link is gone, spawns a bolt from points[index] to points[index + 1]; ends the chain after the last. */
void beam_updateBoltChain(BoltChainObj *obj) {
    BoltChain *chain;
    BoltDesc desc;
    BoltChainObj *owner;
    Vec3f offset;
    BoltChainConfig *config;

    chain = &obj->chain;
    if (chain->link == NULL) {
        if (chain->index >= chain->count - 1) {
            obj->base.header.state = -1;
            obj->base.header.flags |= 1;
            return;
        }
        owner = obj;
        desc.flags = 1;
        desc.updateKind = -1;
        config = &chain->config;
        desc.width = func_150ADA68() * config->widthRange + config->widthBase;
        desc.jitter = config->jitter;
        desc.rejitterBase = config->rejitterBase;
        desc.rejitterRange = config->rejitterRange;
        desc.ctrl[0] = chain->points[chain->index];
        desc.ctrl[3] = chain->points[chain->index + 1];
        math_dirFromBinAngles(func_150ADA20() & 0xFF,
            func_150ADA20() % (config->bendSpread + 1) - (config->bendSpread >> 1) - 0x40,
            func_150ADA68() * config->bendRange + config->bendBase, &offset);
        desc.ctrl[1].x = desc.ctrl[0].x + offset.x;
        desc.ctrl[1].y = desc.ctrl[0].y + offset.y;
        desc.ctrl[1].z = desc.ctrl[0].z + offset.z;
        math_dirFromBinAngles(func_150ADA20() & 0xFF,
            func_150ADA20() % (config->bendSpread + 1) - (config->bendSpread >> 1) - 0x40,
            func_150ADA68() * config->bendRange + config->bendBase, &offset);
        desc.ctrl[2].x = desc.ctrl[3].x + offset.x;
        desc.ctrl[2].y = desc.ctrl[3].y + offset.y;
        desc.ctrl[2].z = desc.ctrl[3].z + offset.z;
        desc.color.red = config->r;
        desc.color.green = config->g;
        desc.color.blue = config->b;
        desc.life = func_150ADA20() % (config->lifeRange + 1) + config->lifeBase;
        desc.color.alpha = func_150ADA20() % (config->alphaRange + 1) + config->alphaBase;
        desc.kind = 1;
        desc.segments = func_150ADA20() % (config->segmentsRange + 1) + config->segmentsBase;
        chain->link = beam_newBolt(&desc, sizeof(owner), obj->base.header.owner, obj->base.header.kind);
        if (chain->link != NULL) {
            memcpy(chain->link->userData, &owner, sizeof(owner));
        }
        chain->index++;
    }
}

/* Kind-1 (chain link) cleanup: tells the owning chain that its link is gone. */
void beam_unlinkChainBolt(BoltObj *bolt) {
    BoltChainObj **owner = bolt->userData;
    BoltChain *chain = &(*owner)->chain;
    chain->link = NULL;
}

/* Kills the chain's live link, if any. */
void beam_killChainLink(BoltChainObj *obj) {
    // matching: volatile makes IDO load the link twice, as the original does
    if (*(BoltObj *volatile *)&obj->chain.link != NULL) {
        effect_destroyObject(*(EffectHeader *volatile *)&obj->chain.link);
    }
}

void beam_destroyBoltChain(BoltChainObj *obj) {
    beam_killChainLink(obj);
    effect_defaultDestroyTimedEffect(&obj->base);
}

void beam_freeBoltChain(BoltChainObj *obj) {
    beam_killChainLink(obj);
    effect_defaultFreeTimedEffect(&obj->base);
}

/* Dispatches to the chain kind's handler in gBoltChainMsgHandlers. */
void beam_boltChainMessage(BoltChainObj *obj, s32 arg1, u8 arg2) {
    if (gBoltChainMsgHandlers[obj->chain.kind] != NULL) {
        gBoltChainMsgHandlers[*(volatile u8 *)&obj->chain.kind](&obj->base.header, arg1, arg2); // matching: the original reloads the kind
    }
}

/* User data of a kind-2 bolt: starts on the owner actor's bounds, in direction yaw/pitch. */
typedef struct BoltAnchor {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;         /* owner's unique_id when spawned */
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 yaw;
    /* 0x08 */ s16 pitch;
    /* 0x0A */ u8 padA[2];
} BoltAnchor;

/* User data of a kind-3 bolt: starts at localPos in one of the owner actor's bone matrices. */
typedef struct BoltBoneAnchor {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId;         /* owner's unique_id when spawned */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ Vec3f localPos;
    /* 0x14 */ u8 bone;
    /* 0x15 */ u8 pad15[3];
} BoltBoneAnchor;

/*
 * Spawns a kind-2 bolt from a point on the actor's bounds (direction yaw/pitch) to where the ray
 * from that point away from the actor hits. life -1 means it never runs out.
 */
BoltObj *beam_spawnBoundsBolt(Actor *actor, s16 yaw, s16 pitch, s16 life, s32 segments, f32 jitter, f32 width, s16 rejitterBase, s16 rejitterRange, u8 *color, u8 argA, s32 argB) {
    BoltAnchor anchor;
    BoltObj *bolt;
    BoltDesc desc;
    RayHit hit;
    Vec3f center;
    s32 countsDown;
    if (actor->ob_matrix_ptr == NULL) {
        return NULL;
    }

    anchor.owner = actor;
    anchor.ownerId = actor->unique_id;
    anchor.yaw = yaw;
    anchor.pitch = pitch;

    if (life == -1) {
        desc.life = 300;
    } else {
        desc.life = life;
    }
    if (life == -1) {
        countsDown = 0;
    } else {
        countsDown = 1;
    }
    desc.flags = countsDown;
    desc.updateKind = 0;
    desc.segments = segments;
    desc.jitter = jitter;
    desc.width = width;
    desc.rejitterBase = rejitterBase;
    desc.rejitterRange = rejitterRange;
    desc.color = *(Color8 *)color;
    desc.kind = 2;

    // matching: a block holding a declaration emits a scope marker that keeps IDO from hoisting &desc.color
    {
        void beam_getBoundsPoint(Actor *actor, s32 yaw, s32 pitch, Vec3f *out, Vec3f *centerOut);

        beam_getBoundsPoint(actor, yaw, pitch, &desc.ctrl[0], &center);
        collision_castRay(actor, desc.ctrl[0].x, desc.ctrl[0].y, desc.ctrl[0].z, desc.ctrl[0].x - center.x, desc.ctrl[0].y - center.y, desc.ctrl[0].z - center.z, &hit, 0.0f, 0, 1, 1, -1, 0, 0);
    }
    if (hit.hitType == 0) {
        return NULL;
    }
    desc.ctrl[3] = hit.pos;

    beam_getStraightCtrlPoints(&desc.ctrl[0], &desc.ctrl[3], &desc.ctrl[1], &desc.ctrl[2]);
    bolt = beam_newBolt(&desc, sizeof(BoltAnchor), argA, argB);
    if (bolt != NULL) {
        memcpy(bolt->userData, &anchor, sizeof(BoltAnchor));
    }
    return bolt;
}

/* gBoltMsgHandlers handler for kind 2. */
void beam_boundsBoltMessage(BoltObj *bolt, s32 arg1, u8 arg2) {
    effect_handleOwnerActorEvent(arg1, arg2, bolt->userData, (u8 *)bolt->userData + 4, bolt);
}

/* updateKind 0: keeps a kind-2 bolt's start on its owner; returns 0 once the owner is gone. */
s32 beam_followBoundsBolt(BoltObj *bolt, u8 *moved) {
    BoltAnchor *anchor;
    Actor *owner;
    Vec3f oldStart;

    anchor = bolt->userData;
    owner = anchor->owner;
    if ((owner->type == 0) || (owner->unique_id != anchor->ownerId)) {
        return 0;
    }
    *moved = FALSE;
    oldStart = bolt->desc.ctrl[0];
    beam_getBoundsPoint(owner, anchor->yaw, anchor->pitch, &bolt->desc.ctrl[0], NULL);
    if (oldStart.x != bolt->desc.ctrl[0].x || oldStart.y != bolt->desc.ctrl[0].y || oldStart.z != bolt->desc.ctrl[0].z) {
        *moved = TRUE;
        beam_getStraightCtrlPoints(&bolt->desc.ctrl[0], &bolt->desc.ctrl[3], &bolt->desc.ctrl[1], &bolt->desc.ctrl[2]);
    }
    return 1;
}

/* The point on the actor's bounds in direction yaw/pitch (vertically scaled by unkF0); optionally the bounds centre. */
void beam_getBoundsPoint(Actor *actor, s32 yaw, s32 pitch, Vec3f *out, Vec3f *centerOut) {
    Vec3f center;
    f32 radius;
    f32 unused;

    sprite_getActorBodyBounds(actor, &center, &radius, &unused);
    math_dirFromBinAngles(yaw, pitch, radius, out);
    out->y *= actor->unkF0;
    out->x += center.x;
    out->y += center.y;
    out->z += center.z;
    if (centerOut != NULL) {
        *centerOut = center;
    }
}

/* Inner control points of a straight bolt: one third and two thirds of the way from start to end. */
void beam_getStraightCtrlPoints(Vec3f *start, Vec3f *end, Vec3f *ctrl1, Vec3f *ctrl2) {
    f32 dx;
    f32 dy;
    f32 dz;

    dx = end->x - start->x;
    dy = end->y - start->y;
    dz = end->z - start->z;
    ctrl1->x = start->x + dx * 0.333f;
    ctrl1->y = start->y + dy * 0.333f;
    ctrl1->z = start->z + dz * 0.333f;
    ctrl2->x = start->x + dx * 0.6667f;
    ctrl2->y = start->y + dy * 0.6667f;
    ctrl2->z = start->z + dz * 0.6667f;
}

/* Calls weapon_spawnImpactBurst at the point on the actor's bounds in direction yaw/pitch. */
void func_15107B78(Actor *actor, s16 yaw, s16 pitch, u8 arg3, s32 arg4) {
    Vec3f pos;
    Vec3f center;
    f32 radius;
    f32 unused;

    if (actor->ob_matrix_ptr != NULL) {
        sprite_getActorBodyBounds(actor, &center, &radius, &unused);
        math_dirFromBinAngles(yaw, pitch, radius, &pos);
        pos.y *= actor->unkF0;
        pos.x += center.x;
        pos.y += center.y;
        pos.z += center.z;
        weapon_spawnImpactBurst(&pos, arg3, arg4);
    }
}

/*
 * Spawns a kind-3 bolt from localPos in the actor's bone matrix along direction yaw/pitch, up to 300
 * units or to what the ray hits. life -1 means it never runs out.
 */
BoltObj *beam_spawnBoneBolt(Actor *actor, u8 bone, const Vec3f *localPos, s16 yaw, s16 pitch, s16 life, s32 segments, f32 jitter, f32 width, s16 rejitterBase, s16 rejitterRange, u8 *color, u8 argC, s32 argD) {
    BoltBoneAnchor anchor;
    BoltObj *bolt;
    BoltDesc desc;
    RayHit hit;
    Vec3f dir;
    s32 countsDown;
    if (actor->ob_matrix_ptr == NULL) {
        return NULL;
    }

    anchor.owner = actor;
    anchor.ownerId = actor->unique_id;
    anchor.localPos = *localPos;
    anchor.bone = bone;

    if (life == -1) {
        desc.life = 300;
    } else {
        desc.life = life;
    }
    if (life == -1) {
        countsDown = 0;
    } else {
        countsDown = 1;
    }
    desc.flags = countsDown;
    desc.updateKind = 1;
    desc.segments = segments;
    desc.jitter = jitter;
    desc.width = width;
    desc.rejitterBase = rejitterBase;
    desc.rejitterRange = rejitterRange;
    desc.color = *(Color8 *)color;
    desc.kind = 3;

    beam_transformBoneLocal(actor, bone, localPos, &desc.ctrl[0]);
    math_dirFromBinAngles(yaw, pitch, 1.0f, &dir);
    collision_castRay(actor, desc.ctrl[0].x, desc.ctrl[0].y, desc.ctrl[0].z, dir.x, dir.y, dir.z, &hit, 300.0f, 0, 1, 1, -1, 0, 0);

    if (hit.hitType == 0) {
        desc.ctrl[3].x = desc.ctrl[0].x + dir.x * 300.0f;
        desc.ctrl[3].y = desc.ctrl[0].y + dir.y * 300.0f;
        desc.ctrl[3].z = desc.ctrl[0].z + dir.z * 300.0f;
    } else {
        desc.ctrl[3] = hit.pos;
    }

    beam_getStraightCtrlPoints(&desc.ctrl[0], &desc.ctrl[3], &desc.ctrl[1], &desc.ctrl[2]);
    bolt = beam_newBolt(&desc, sizeof(BoltBoneAnchor), argC, argD);
    if (bolt != NULL) {
        memcpy(bolt->userData, &anchor, sizeof(BoltBoneAnchor));
    }
    return bolt;
}

/* updateKind 1: keeps a kind-3 bolt's start on its owner's bone; returns 0 once the owner is gone. */
s32 beam_followBoneBolt(BoltObj *bolt, u8 *moved) {
    BoltBoneAnchor *anchor;
    Actor *owner;
    Vec3f oldStart;

    anchor = bolt->userData;
    owner = anchor->owner;
    if ((owner->type == 0) || (owner->unique_id != anchor->ownerId)) {
        return 0;
    }
    if (owner->ob_matrix_ptr == NULL) {
        return 0;
    }
    *moved = FALSE;
    oldStart = bolt->desc.ctrl[0];
    beam_transformBoneLocal(owner, anchor->bone, &anchor->localPos, &bolt->desc.ctrl[0]);
    if (oldStart.x != bolt->desc.ctrl[0].x || oldStart.y != bolt->desc.ctrl[0].y || oldStart.z != bolt->desc.ctrl[0].z) {
        *moved = TRUE;
        beam_getStraightCtrlPoints(&bolt->desc.ctrl[0], &bolt->desc.ctrl[3], &bolt->desc.ctrl[1], &bolt->desc.ctrl[2]);
    }
    return 1;
}

/* Transforms localPos by the actor's bone matrix (64 bytes each, at ob_matrix_ptr). */
void beam_transformBoneLocal(Actor *actor, u8 bone, const Vec3f *localPos, Vec3f *out) {
    math_transformOffset(localPos, out, &actor->ob_matrix_ptr[bone]);
}

/* gBoltMsgHandlers handler for kind 3. */
void beam_boneBoltMessage(BoltObj *bolt, s32 arg1, u8 arg2) {
    effect_handleOwnerActorEvent(arg1, arg2, bolt->userData, (u8 *)bolt->userData + 4, bolt);
}
