#include <ultra64.h>
#include <math.h>
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "libc/string.h"

/* Random burst points around each pipe: level_initPipeBurstPoints (game/Level/PipeBurstSetup/pipeburstsetup) fills them, particle_updateRandomBurstEffect uses them. */
BurstPoint gPipeBurstPoints[3][10];

/* The streak particle type's render setup display list (object type table). */
Gfx gStreakParticleSetupDl[] = {
    gsDPPipeSync(),
    gsSPMatrix(&gIdentityMtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH),
    gsSPEndDisplayList(),
};

/* Allocates a streak particle (kind 0 or 1) with extraSize trailing bytes and fills it from desc. */
StreakParticle *particle_newOwnedStreakParticle(StreakParticleDesc *desc, u8 kind, s32 extraSize, u8 owner, s32 arg4) {
    StreakParticle *newParticle;
    StreakParticle *particle;
    s32 id;
    Vec3f pos;

    switch (kind) {
        case 0:
            id = 0x2C;
            break;
        case 1:
            id = 0x53;
            break;
        default:
            id = 0x2C;
            break;
    }

    newParticle = effect_newObject(id, arg4, extraSize + sizeof(StreakParticle), 1, owner, 1);
    if (newParticle == NULL) {
        return NULL;
    }
    particle = newParticle; // matching: the second pointer local survives the call as v1
    particle->vtxTemplate[0].v.flag = 0;
    particle->vtxTemplate[0].v.cn[0] = desc->tailColor.red;
    particle->vtxTemplate[0].v.cn[1] = desc->tailColor.green;
    particle->vtxTemplate[0].v.cn[2] = desc->tailColor.blue;
    particle->vtxTemplate[0].v.cn[3] = desc->tailColor.alpha;
    particle->vtxTemplate[0].v.tc[0] = 0;
    particle->vtxTemplate[0].v.tc[1] = 0;
    particle->vtxTemplate[1].v.flag = 0;
    particle->vtxTemplate[1].v.cn[0] = desc->headColor.red;
    particle->vtxTemplate[1].v.cn[1] = desc->headColor.green;
    do {} while (0); // matching: block boundary makes as1 keep the v1 copy of the pointer for the last stores
    particle->vtxTemplate[1].v.cn[2] = desc->headColor.blue;
    particle->vtxTemplate[1].v.cn[3] = desc->headColor.alpha;
    particle->vtxTemplate[2].v.flag = 0;
    particle->vtxTemplate[2].v.cn[0] = desc->headColor.red;
    particle->vtxTemplate[2].v.cn[1] = desc->headColor.green;
    particle->vtxTemplate[2].v.cn[2] = desc->headColor.blue;
    particle->vtxTemplate[2].v.cn[3] = desc->headColor.alpha;
    pos = desc->pos;
    particle->tailPos = pos;
    particle->headPos = pos;
    particle->headVel.x = desc->dir.x * desc->headSpeed;
    particle->headVel.y = desc->dir.y * desc->headSpeed;
    particle->headVel.z = desc->dir.z * desc->headSpeed;
    particle->tailVel.x = desc->dir.x * desc->tailSpeed;
    particle->tailVel.y = desc->dir.y * desc->tailSpeed;
    particle->tailVel.z = desc->dir.z * desc->tailSpeed;
    particle->flags = desc->flags;
    particle->lifetime = desc->lifetime;
    particle->renderMode = desc->renderMode;
    particle->width = desc->width;
    particle->alpha = desc->alpha;
    particle->fadeStart = desc->fadeStart;
    particle->fadeRate = desc->fadeRate;
    bzero(particle->vtxBuffers, sizeof(particle->vtxBuffers));
    return particle;
}

/* particle_newOwnedStreakParticle with owner 0xFF and arg4 0. */
void particle_newStreakParticle(StreakParticleDesc *desc, u8 kind, s32 extraSize) {
    particle_newOwnedStreakParticle(desc, kind, extraSize, 0xFF, 0);
}

/* Per-frame update: count down the lifetime, move head and tail, fade out; frees the particle when expired. */
void particle_updateStreakParticle(StreakParticle *arg0) {
    StreakParticle *particle;
    u8 expired;
    s16 fadedAlpha;

    // matching: copying the parameter to a local steers IDO's register choice
    particle = arg0;
    expired = FALSE;
    if (particle->flags & 1) {
        particle->lifetime -= gFrameTicks;
        if (particle->lifetime < 0) {
            expired = TRUE;
        }
    }
    if (!expired) {
        particle->headPos.x += particle->headVel.x * gFrameDelta;
        particle->headPos.y += particle->headVel.y * gFrameDelta;
        particle->headPos.z += particle->headVel.z * gFrameDelta;
        particle->tailPos.x += particle->tailVel.x * gFrameDelta;
        particle->tailPos.y += particle->tailVel.y * gFrameDelta;
        particle->tailPos.z += particle->tailVel.z * gFrameDelta;
        if (particle->flags & 8) {
            if (particle->lifetime < particle->fadeStart) {
                fadedAlpha = particle->lifetime * particle->fadeRate;
                if (fadedAlpha < particle->alpha) {
                    particle->alpha = fadedAlpha;
                }
            }
        }
    }
    if (expired) {
        effect_destroyObject(&particle->header);
    }
}

/* Draws a streak particle for one view as a single triangle facing that view's eye. */
Gfx *particle_drawStreakParticle(Gfx *gfx, StreakParticle *particle, s16 viewIndex) {
    StreakParticle *p;
    Vec3f axis;
    Vec3f toEye;
    Vec3f side;
    Vec3f *eye;
    f32 lengthSquared;
    u8 needSync;
    s16 cullFlags;
    s32 geometryFlags;
    Vtx *vtx;
    u8 mode;
    u8 combiner;
    u8 fresh;
    f32 scale;

    // matching: the p alias orders IDO's spill slots (expression ids follow first use)
    p = particle;
    particle_getViewBuffer(particle->vtxBuffers, viewIndex, sizeof(particle->vtxTemplate), &vtx, &fresh);
    if (fresh) {
        memcpy(particle->vtxBuffers[viewIndex], particle->vtxTemplate, sizeof(particle->vtxTemplate));
        memcpy(particle->vtxBuffers[viewIndex] + 3, particle->vtxTemplate, sizeof(particle->vtxTemplate));
    }
    eye = &camera_global_Cameras[viewIndex].eye;
    needSync = TRUE;
    cullFlags = particle->flags & 6;
    mode = particle->renderMode & 0xFF;
    combiner = (particle->renderMode & 0xFF00) >> 8;
    geometryFlags = 0;
    if (cullFlags & 2) {
        geometryFlags = G_CULL_FRONT;
    }
    if (cullFlags & 4) {
        geometryFlags |= G_CULL_BACK;
    }
    gfx = math_gfxUpdateGeometryMode(gfx, geometryFlags | G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH,
                        G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD);
    gfx = math_gfxSetEnvColor(gfx, 0, 0, 0, particle->alpha, &needSync);
    gfx = math_gfxSetPrimColor(gfx, 0, 0, 0, 0, 0, 0, &needSync);
    gfx = particle_setCombineMode(gfx, combiner, &needSync);
    gfx = math_gfxSetOtherMode(gfx, 0x82CA0, gEffectRenderModes[mode].cycle1 | gEffectRenderModes[mode].cycle2, &needSync);
    axis.x = p->headPos.x - p->tailPos.x;
    axis.y = p->headPos.y - p->tailPos.y;
    axis.z = p->headPos.z - p->tailPos.z;
    toEye.x = p->headPos.x - eye->x;
    toEye.y = p->headPos.y - eye->y;
    toEye.z = p->headPos.z - eye->z;
    side.x = axis.y * toEye.z - toEye.y * axis.z;
    side.y = axis.z * toEye.x - toEye.z * axis.x;
    side.z = axis.x * toEye.y - toEye.x * axis.y;
    lengthSquared = side.x * side.x + side.y * side.y + side.z * side.z;
    if (lengthSquared == 0.0f) {
        side.x = 0.0f;
        side.y = 0.0f;
        side.z = 0.0f;
    } else {
        scale = p->width / sqrtf(lengthSquared);
        side.x *= scale;
        side.y *= scale;
        side.z *= scale;
    }
    vtx[0].v.ob[0] = p->tailPos.x;
    vtx[0].v.ob[1] = p->tailPos.y;
    vtx[0].v.ob[2] = p->tailPos.z;
    vtx[1].v.ob[0] = p->headPos.x + side.x;
    vtx[1].v.ob[1] = p->headPos.y + side.y;
    vtx[1].v.ob[2] = p->headPos.z + side.z;
    vtx[2].v.ob[0] = p->headPos.x - side.x;
    vtx[2].v.ob[1] = p->headPos.y - side.y;
    vtx[2].v.ob[2] = p->headPos.z - side.z;
    gSPVertex(gfx++, vtx, 3, 0);
    gSP1Triangle(gfx++, 0, 1, 2, 0);
    return gfx;
}

/* Spawns the streak burst and the particle described by desc. */
void particle_spawnStreakBurstEffect(desc, owner)
    StreakBurstEffectDesc *desc;
    u8 owner;
{
    StreakBurst burst;
    ParticleDesc particle;

    burst.countBase = desc->countBase;
    burst.countRange = desc->countRange;
    burst.pos = desc->pos;
    burst.yawBase = desc->yawBase;
    burst.yawRange = desc->yawRange;
    burst.pitchBase = desc->pitchBase;
    burst.pitchRange = desc->pitchRange;
    burst.headSpeedBase = desc->headSpeedBase;
    burst.headSpeedRange = desc->headSpeedRange;
    burst.tailSpeedBase = desc->tailSpeedBase;
    burst.tailSpeedRange = desc->tailSpeedRange;
    burst.headColor.red = desc->headColor.red;
    burst.headColor.green = desc->headColor.green;
    burst.headColor.blue = desc->headColor.blue;
    burst.headColor.alpha = desc->headColor.alpha;
    burst.tailColor.red = desc->tailColor.red;
    burst.tailColor.green = desc->tailColor.green;
    burst.tailColor.blue = desc->tailColor.blue;
    burst.tailColor.alpha = desc->tailColor.alpha;
    burst.flags = desc->flags;
    burst.lifetimeBase = desc->lifetimeBase;
    burst.lifetimeRange = desc->lifetimeRange;
    burst.renderMode = desc->renderMode;
    burst.widthBase = desc->widthBase;
    burst.widthRange = desc->widthRange;
    burst.alphaBase = desc->alphaBase;
    burst.alphaRange = desc->alphaRange;
    burst.fadeStart = desc->fadeStart;
    burst.fadeRate = desc->fadeRate;
    burst.kind = desc->kind;
    particle_spawnStreakBurst(&burst, owner);

    particle.texture = desc->particleTexture;
    particle.renderMode = desc->particleRenderMode;
    particle.unk4 = desc->particleUnk4;
    particle.unk0 = desc->particleUnk0;
    particle.frame = desc->particleFrame;
    particle.frameRate = desc->particleFrameRate;
    particle.r = desc->particleColor.red;
    particle.g = desc->particleColor.green;
    particle.b = desc->particleColor.blue;
    particle.a = desc->particleColor.alpha;
    particle.r2 = desc->particleColor2.red;
    particle.g2 = desc->particleColor2.green;
    particle.b2 = desc->particleColor2.blue;
    particle.pos = desc->pos;
    particle.flags = desc->particleFlags;
    particle.alpha = desc->particleColor2.alpha;
    particle.brightness = desc->particleBrightness;
    particle.updateFunc = desc->particleUpdateFunc;
    particle.deathFunc = desc->particleDeathFunc;
    particle.fadeStart = desc->particleFadeStart;
    particle.fadeRate = desc->particleFadeRate;
    particle.primColorMode = desc->particlePrimColorMode;
    particle.envColorMode = desc->particleEnvColorMode;
    particle.scaleStart = desc->particleScaleStart;
    particle.scaleRate = desc->particleScaleRate;
    particle.timer = desc->particleTimer;
    *(ParticleSize *)&particle.width = desc->particleSize; // matching: width and height are copied as one struct
    particle.vel.x = 0.0f;
    particle.vel.y = 0.0f;
    particle.vel.z = 0.0f;
    particle.gravity = 0.0f;
    particle_newOwnedSpriteParticle(&particle, desc->particleKind, 0, owner, 0);
}

/* When the timer runs out, sets off a burst (particle_spawnPipeBurst and light_createWithEffect) at a random table point
 * that has no zero coordinate, then restarts the timer at a random interval. */
void particle_updateRandomBurstEffect(RandomBurstEffect *effect) {
    RandomTimer *timer;
    u32 row;
    u32 col;
    LightEffectHeader header;
    Vec3i intPos;

    effect->timer.value -= gFrameTicks;
    if (effect->timer.value < 0) {
        row = math_random() % 3U;
        col = math_random() % 10U;
        if (gPipeBurstPoints[row][col].pos.x != 0.0f && gPipeBurstPoints[row][col].pos.y != 0.0f && gPipeBurstPoints[row][col].pos.z != 0.0f) {
            particle_spawnPipeBurst(&gPipeBurstPoints[row][col].pos.x, effect->base.header.owner);
            header.flags = 3;
            header.update = -1;
            header.lifetime = (math_random() % 11U) + 5;
            header.lightType = 0;
            intPos.x = gPipeBurstPoints[row][col].pos.x;
            intPos.y = gPipeBurstPoints[row][col].pos.y;
            intPos.z = gPipeBurstPoints[row][col].pos.z;
            light_createWithEffect(&header, &intPos, (math_random() % 156U) + 100, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0,
                          effect->base.header.owner, effect->base.header.kind);
        }
        timer = &effect->timer;
        timer->value = (math_random() % (timer->range + 1)) + timer->base;
    }
}

/* Spawns the burst effect used by particle_updateRandomBurstEffect at pos (3 floats), with randomised particle size and flags. */
void particle_spawnPipeBurst(f32 *pos, u8 owner) {
    StreakBurstEffectDesc desc;

    memcpy(&desc.pos, pos, sizeof(desc.pos));
    desc.countBase = 10;
    desc.countRange = 30;
    desc.pitchBase = -63;
    desc.yawRange = 255;
    desc.pitchRange = 128;
    desc.unk8A = desc.headColor.red = desc.particleColor2.red = 0xFF;
    desc.unk8B = desc.headColor.green = desc.particleColor2.green = 0xFF;
    desc.headColor.blue = desc.particleColor2.blue = 0xFF;
    desc.unk8C = 0xFF;
    desc.headColor.alpha = 0xFF;
    desc.tailColor.red = 0xFF;
    desc.tailColor.green = 0xFF;
    desc.tailColor.blue = 0xFF;
    desc.tailColor.alpha = 0xFF;
    desc.flags = 9;
    desc.yawBase = 0;
    desc.lifetimeBase = 5;
    desc.lifetimeRange = 10;
    desc.renderMode = 0x1601;
    desc.alphaBase = 100;
    desc.alphaRange = 155;
    desc.fadeStart = 4;
    desc.fadeRate = 63;
    desc.kind = 0;
    desc.particleTexture = 43;
    desc.particleRenderMode = 0x1A01;
    desc.particleUnk0 = 0x200005;
    desc.particleUnk4 = 0;
    desc.tailSpeedRange = desc.tailSpeedBase = 1.6899999f;
    desc.headSpeedBase = 6.97f;
    desc.headSpeedRange = 3.37f;
    desc.widthBase = 2.01f;
    desc.widthRange = 3.02f;
    desc.particleTimer = (math_random() % 3U) + 7;
    desc.particleFrame = 0;
    desc.particleFrameRate = 0;
    desc.particleColor.red = 0xFF;
    desc.particleColor.green = 0xFF;
    desc.particleColor.blue = 0xFF;
    desc.particleColor.alpha = 0xFF;
    math_random();
    desc.particleColor2.alpha = 0xFF;
    desc.particleBrightness = 0xFF;
    desc.particleSize.width = desc.particleSize.height = (math_randomFloat() * 150.0f) + 196.0f;
    desc.particleFadeStart = 4;
    desc.particleFadeRate = 63;
    desc.particleScaleStart = 22;
    desc.particleScaleRate = 1.25f;
    desc.particleFlags = 0xE01;
    if (math_random() & 1) {
        desc.particleFlags |= 0x40;
    }
    if (math_random() & 1) {
        desc.particleFlags |= 0x80;
    }
    desc.particlePrimColorMode = 4;
    desc.particleEnvColorMode = 4;
    desc.particleUpdateFunc = -1;
    desc.particleDeathFunc = -1;
    desc.particleKind = 1;
    desc.unk89 = 1;
    particle_spawnStreakBurstEffect(&desc, owner);
}

/* Frees a streak particle's per-view vertex buffers. */
void particle_freeStreakVtxBuffers(StreakParticle *particle) {
    particle_freeViewBuffers(particle->vtxBuffers);
}

void particle_destroyStreakParticle(StreakParticle *particle) {
    particle_freeStreakVtxBuffers(particle);
    effect_defaultDestroy(&particle->header);
}

void particle_freeStreakParticle(StreakParticle *particle) {
    particle_freeStreakVtxBuffers(particle);
    effect_defaultFree(&particle->header);
}
