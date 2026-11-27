#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include "libc/string.h"

/* Per-behaviour state that follows a particle (a DefParticleHead). */
typedef union {
    ParticleAxes axes;  /* particle_computeQuadAxes, particle_buildAxisQuad */
    ActorRef owner;     /* particle_onQuadParticleOwnerMsg */
    struct {
        f32 unk0;
        f32 unk4;
        Vec3f vel;
        Vec3f rotVel;
        Vec3f scaleVel;
        f32 gravity;
    } drift;            /* particle_updateDriftMotion */
    struct {
        u8 pad0[0x18];
        f32 rate;
    } grow;             /* particle_updateGrowMotion */
    struct {
        Vec3f vel;
        f32 gravity;
        f32 drag;
        Vec3f rotVel;
    } ballistic;        /* particle_updateBallisticMotion */
} DefParticleMotion;

#define PARTICLE_MOTION(particle) ((DefParticleMotion *)((particle) + 1))


/* Sprite update (QuadSprite.updateFn; 0 = none) and expire (QuadSprite.expireFn) callbacks; an update returning 0
 * expires the sprite, an expire callback returning nonzero keeps it. */
static s32 (*gQuadSpriteUpdateFuncs[])() = {
    NULL, blood_fadeQuadSpriteByLife, particle_fadeQuadSpriteByLife, particle_fadeMoveHitSprite, effect_fadeQuadSpriteByLife, effect_fadeTrailSpriteByLife, effect_updateFadeQuadSprite, effect_updateTaskFlicker,
    particle_updateFlickerSprite, effect_updateFlickerSprite, particle_fadeSideDustSprite, particle_fadeSideSpraySprite, effect_capQuadSpriteAlphaByLife, blood_capQuadSpriteAlphaByLife,
};
static s32 (*gQuadSpriteExpireFuncs[])() = { NULL, effect_detachTaskFlicker, particle_detachFlickerSprite, effect_expireFlickerSprite };

/* Particle update (DefParticleHead.updateFn1, updateFn2; 0 = none) and expire callbacks, as for sprites. */
static s32 (*gQuadParticleUpdate1Funcs[])() = {
    NULL, particle_updateDriftMotion, particle_followBoneGlow, particle_fallAndGrow, particle_moveTumbling, particle_moveFlickerParticle, particle_moveFlickerTrail, beam_updateMotionPull,
    beam_updateMotion, beam_updateQuad, flamethrower_moveJetLine, particle_growQuadParticle, particle_wobbleDrip, particle_fallDrip, particle_updateSteamEffect,
    particle_moveImpactStreak, effect_moveReleasedParticle, effect_swingParticleSize, particle_growQuadAtRate, particle_updateDriftQuad, func_150C308C, particle_updateRayBeam,
    weapon_updateProjectile, func_150AE35C, particle_updateGrowMotion, particle_updateBallisticMotion, sight_updateBeam, weapon_updateBulletParticle, trail_integrateScalarMotion,
    beam_updateAimed, beam_updateGlow, flamethrower_moveFlameLine, particle_updateJitterBallisticEased, particle_updateJitterBallistic, func_150E4174, particle_bobQuad,
    effect_updateTrailPuff, flare_glowOrbit, particle_updateBoneBurstParticle, cutscene_updateTrackParticle, effect_moveTravelParticle, effect_updateBoneBeamParticle,
};
static s32 (*gQuadParticleUpdate2Funcs[])() = {
    NULL, particle_fadeFallGrowParticle, particle_fadeTumbling, flare_moveGlowAttachment, particle_flickerSize, particle_growFlickerParticle, particle_shrinkFlickerParticle, beam_shrinkMotion,
    beam_shrinkMotionProgress, particle_wobbleSpriteSize, beam_updateLinkedQuad, flamethrower_fadeJetLine, particle_easeSpriteSizeOnce, particle_flickerChannelParticle, particle_growChannelParticle,
    particle_shrinkChannelParticle, particle_shrinkRisingChannelParticle, particle_fadeQuadParticleByLife, particle_fadeDrip, particle_shrinkSteamEffect, particle_updateSquashEffect, particle_fadeImpactStreak,
    particle_updateSpray, particle_updateDustPuff, particle_fadeInGrow, particle_shrinkBallisticChannelParticle, blood_updateGrowingParticle, effect_fadeLandingParticleByLife, particle_shrinkBallisticQuad,
    particle_capQuadAlphaByLife, particle_updateDriftQuad, particle_flickerRayBeam, flare_glowFlicker, flare_glowEnvelope, flamethrower_fadeFlameLine, particle_updateJitterParticle,
    particle_updateRelaunchedJitter, effect_updateBurstSpark, particle_swaySpark, trail_updateTrailGlow, particle_updateZoneEmitter, flare_glowPulse, effect_updateBurstGlow,
    effect_updateActorBeam,
};
static s32 (*gQuadParticleExpireFuncs[])() = { NULL, particle_spawnBillboardOnExpire, particle_relaunchJitterParticle };

/* Vertex functions of a particle (DefParticleHead.vertexFn), see particle_buildRotatedQuad. The prototype keeps the viewport an
 * s16; most entries take their own view of the particle. */
static Vtx *(*gQuadParticleVertexFuncs[])(DefParticleHead *, s16) = {
    particle_buildRotatedQuad, flare_buildGlowQuad, particle_buildFlickerQuad, particle_buildBendingQuad, particle_buildCameraFacingBeamQuad, particle_buildActorEmitterQuad, beam_fillMotionVerts,
    beam_drawQuad, beam_drawLinkedQuad, flamethrower_buildJetLineQuad, particle_buildChannelQuad, particle_buildQuadParticleBillboard, particle_buildAxisQuad, particle_buildDripQuad,
    particle_buildSquashQuad, particle_buildImpactStreakQuad, effect_buildJointParticleQuad, particle_buildDriftQuad, particle_buildRayBeamQuad, particle_buildCentredAxisQuad, weapon_buildProjectileTail,
    collision_buildTransformedUprightQuad, effect_buildBoneBeamQuad, particle_buildRotatedRectQuadUnrolled, sight_buildBeamQuad, weapon_drawBulletParticle, flamethrower_buildFlameLineQuad, particle_buildBeamQuadFwd,
    particle_buildBeamQuadRev, effect_buildBurstSparkVtx, particle_buildRotatedRectQuad, particle_buildSparkBeamQuad, particle_buildYawedVerticalQuad, effect_buildTrailPuffQuad, particle_buildUprightQuadFwd,
    particle_buildUprightQuadRev, effect_buildActorBeamQuad, particle_buildBoneBurstQuad, effect_buildTravelParticleQuad,
};

/* Render setup and reset display lists of the particles; their entry in the object type table points to them. */
Gfx gQuadParticleSetupDl[] = {
    gsDPPipeSync(),
    gsSPMatrix(&gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx gQuadParticleEndDl[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

/* Destroy and free handlers of a sprite (SpriteDef.unk7) and of a particle (ParticleDef.type). */
static void (*gQuadSpriteDestroyFuncs[])(QuadSprite *) = { particle_destroyQuadSpriteDefault, particle_destroyQuadSpriteDefault };
static void (*gQuadSpriteFreeFuncs[])(QuadSprite *) = { particle_freeQuadSpriteDefault, particle_freeQuadSpriteDefault };
static void (*gQuadParticleDestroyFuncs[])() = {
    particle_destroyQuadParticleDefault, particle_destroyRayBeam, particle_destroyBeamHitFx, flare_destroyGlow, particle_destroyQuadParticleDefault, particle_destroyQuadParticleDefault, sight_destroyBeam,
    beam_destroyAimed, effect_destroyBurstSpark, beam_destroyModelBeam, effect_destroyZoneEmitter, effect_destroyActorBeam, particle_destroyQuadParticleDefault,
};
static void (*gQuadParticleFreeFuncs[])() = {
    particle_freeQuadParticleDefault, particle_freeRayBeam, particle_freeBeamHitFx, flare_freeGlow, particle_freeQuadParticleDefault, particle_freeQuadParticleDefault, sight_freeBeam,
    beam_freeAimed, effect_freeBurstSpark, beam_freeModelBeam, effect_freeZoneEmitter, effect_freeActorBeam, particle_freeQuadParticleDefault,
};
/* Event handler of a sprite (particle_onQuadSpriteEvent). */
static void (*gQuadSpriteEventFuncs[])(s32, s32, u8) = { particle_ignoreQuadSpriteEvent };
/* Event handlers of a particle (ParticleDef.type, particle_onQuadParticleEvent); several take their own view of the particle and
 * of the event data. */
static void (*gQuadParticleEventFuncs[])(DefParticleHead *, s32, u8) = {
    particle_ignoreQuadParticleEvent, particle_onQuadParticleOwnerMsg, effect_boneBeamMessage, sight_handleBeamMessage, flare_glowMessage, beam_aimedMessage, effect_burstSparkMessage,
    effect_burstSparkMessage, func_151C4AB0, effect_actorBeamMessage, particle_onBoneBurstParticleEvent,
};

/* Per-frame update of a sprite: lifetime, fading and texture animation. */
void particle_updateQuadSprite(QuadSprite *sprite) {
    s32 speed;
    s8 expired;
    s32 numFrames;
    s32 lastFrame;
    s32 faded;

    if (!(sprite->active & 1)) {
        return;
    }

    expired = FALSE;
    if (sprite->def.flags & 1) {
        sprite->def.lifetime -= gFrameTicks;
        if (sprite->def.lifetime < 0) {
            expired = TRUE;
        }
    }
    if (sprite->updateFn != 0) {
        if (gQuadSpriteUpdateFuncs[sprite->updateFn](sprite) == 0) {
            expired = TRUE;
        }
    }

    if ((sprite->def.flags & 0x8000) && !expired) {
        if (sprite->def.lifetime < sprite->def.fadeStart) {
            faded = sprite->def.lifetime * sprite->def.fadeRate;
            if (faded < sprite->def.unk10) {
                sprite->def.unk10 = faded;
            }
        }
    }
    if (sprite->def.flags & 0x10000) {
        if (!expired) {
            if (sprite->def.lifetime < sprite->def.fadeStart) {
                faded = sprite->def.lifetime * sprite->def.fadeRate;
                if (faded < sprite->def.unk10) {
                    sprite->def.rgba[3] = faded;
                }
            }
        }
    }

    if (!expired) {
        speed = sprite->def.frameSpeed;
        if (speed != 0) {
            numFrames = gEffectTextures[sprite->def.texture].frameCount;
            lastFrame = (numFrames << 16) - 1;
            sprite->def.frame += speed * gFrameTicks;
            if (lastFrame < sprite->def.frame) {
                if (sprite->def.flags & 2) {
                    expired = TRUE;
                } else if (sprite->def.flags & 0x40) {
                    sprite->def.frameSpeed = 0;
                    sprite->def.frame = lastFrame;
                } else if (sprite->def.flags & 4) {
                    sprite->def.frame = lastFrame - (sprite->def.frame % lastFrame);
                    sprite->def.frameSpeed = -speed;
                } else {
                    do {
                        sprite->def.frame -= lastFrame;
                    } while (lastFrame < sprite->def.frame);
                }
            } else if (sprite->def.frame < 0) {
                if (sprite->def.flags & 8) {
                    expired = TRUE;
                } else if (sprite->def.flags & 0x80) {
                    sprite->def.frameSpeed = 0;
                    sprite->def.frame = 0;
                } else if (sprite->def.flags & 4) {
                    sprite->def.frame = (-sprite->def.frame) % lastFrame;
                    sprite->def.frameSpeed = -speed;
                } else {
                    do {
                        sprite->def.frame += lastFrame;
                    } while (sprite->def.frame < 0);
                }
            }
        }
    }

    if (expired) {
        if (sprite->expireFn != 0) {
            if (gQuadSpriteExpireFuncs[sprite->expireFn](sprite) != 0) {
                effect_destroyObject(&sprite->header);
            }
        } else {
            effect_destroyObject(&sprite->header);
        }
    }
}

/* The default vertex colours of a sprite or particle: opaque white. */
const VtxColor gWhiteVtxColors[4] = {
    { 0xFF, 0xFF, 0xFF, 0xFF, 0 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0 },
};

/* Allocates a sprite from a template; extraSize bytes follow it. */
QuadSprite *particle_newQuadSprite(void *def, const void *colors, u8 updateFn, u8 expireFn, u8 flip, s32 model, s32 arg6,
                         Vec3f *pos, s32 extraSize, u8 arg9, s32 argA) {
    QuadSprite *sprite;
    u8 kind;
    s32 i;

    if (colors == NULL) {
        colors = &gWhiteVtxColors;
    }
    if (((SpriteDef *)def)->flags & 0x40000) {
        kind = 0x4A;
    } else {
        kind = 0x1B;
    }
    sprite = effect_newObject(kind, argA, extraSize + sizeof(QuadSprite), 1, arg9, 1);
    if (sprite == NULL) {
        return NULL;
    }
    memcpy(&sprite->def, def, sizeof(SpriteDef));
    particle_setQuadTexCoords(sprite->vtx, sprite->def.texture, flip);
    particle_setQuadColors(sprite->vtx, colors);
    sprite->updateFn = updateFn;
    sprite->expireFn = expireFn;
    sprite->link.model = model;
    sprite->active = 1;
    sprite->unk14 = 0;
    sprite->link.unk1C = arg6;
    for (i = 0; i < 4; i++) {
        sprite->link.handles[i] = NULL;
    }

    sprite->link.sharedHandle = NULL;
    if (model) {
        for (i = 0; i <= gScreenLayout; i++) {
            sprite->link.handles[i] = light_allocGfxLights(model);
        }
        sprite->link.sharedHandle = func_1515D440();
    }
    if (pos) {
        sprite->pos = *pos;
    }
    return sprite;
}

/* Spawns a flat (xz-plane) sprite at (x, y, z). */
QuadSprite *particle_spawnFlatQuadSprite(void *def, const void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width, f32 length,
                         u8 angle, u8 flip, s32 model, s32 arg6, s32 extraSize, u8 argE, s32 argF) {
    QuadSprite *sprite;
    Vec3f pos;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    sprite = particle_newQuadSprite(def, colors, updateFn, expireFn, flip, model, arg6, &pos, extraSize, argE, argF);
    if (sprite == NULL) {
        return NULL;
    }
    particle_layQuadSpriteFlat(sprite, x, y, z, width, length, angle);
    return sprite;
}

/* Spawns a flat (xz-plane) sprite without a model or stored position. */
QuadSprite *particle_spawnFlatQuadSpriteNoModel(void *def, const void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width,
                         f32 length, u8 angle, u8 flip, s32 extraSize, u8 argC, s32 argD) {
    QuadSprite *sprite = particle_newQuadSprite(def, colors, updateFn, expireFn, flip, 0, 0, NULL, extraSize, argC, argD);

    if (sprite == NULL) {
        return NULL;
    }
    particle_layQuadSpriteFlat(sprite, x, y, z, width, length, angle);
    return sprite;
}

/* Spawns a sprite oriented by particle_layQuadSpriteOnSurface, with the default colours. */
QuadSprite *particle_spawnSurfaceQuadSprite(void *def, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                         f32 height, u8 angle, u8 flip, s32 model, s32 arg6, s32 extraSize, u8 argE, s32 argF) {
    QuadSprite *sprite;
    Vec3f pos;

    pos.x = x;
    pos.y = y;
    pos.z = z;

    sprite = particle_newQuadSprite(def, 0, updateFn, expireFn, flip, model, arg6, &pos, extraSize, argE, argF);
    if (sprite == NULL) {
        return NULL;
    }

    if (particle_layQuadSpriteOnSurface(sprite, NULL, surface, x, y, z, width, height, angle) == 0) {
        effect_destroyObject(&sprite->header);
        return NULL;
    }
    return sprite;
}

/* As particle_spawnSurfaceQuadSprite, without a model or stored position. */
QuadSprite *particle_spawnSurfaceQuadSpriteNoModel(void *def, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                         f32 height, u8 angle, u8 flip, s32 extraSize, u8 argC, s32 argD) {
    QuadSprite *sprite = particle_newQuadSprite(def, 0, updateFn, expireFn, flip, 0, 0, NULL, extraSize, argC, argD);

    if (sprite == NULL) {
        return NULL;
    }

    if (particle_layQuadSpriteOnSurface(sprite, NULL, surface, x, y, z, width, height, angle) == 0) {
        effect_destroyObject(&sprite->header);
        return NULL;
    }
    return sprite;
}

/* As particle_spawnSurfaceQuadSpriteNoModel; arg1 is unused and the default colours are used. */
QuadSprite *func_1513C804(void *def, s32 arg1, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z,
                         f32 width, f32 height, u8 angle, u8 flip, s32 extraSize, u8 argD, s32 argE) {
    QuadSprite *sprite = particle_newQuadSprite(def, 0, updateFn, expireFn, flip, 0, 0, NULL, extraSize, argD, argE);

    if (sprite == NULL) {
        return NULL;
    }
    if (particle_layQuadSpriteOnSurface(sprite, NULL, surface, x, y, z, width, height, angle) == 0) {
        effect_destroyObject(&sprite->header);
        return NULL;
    }
    return sprite;
}

void particle_destroyQuadSpriteDefault(QuadSprite *sprite) {
    particle_freeQuadSpriteLights(sprite);
    effect_defaultDestroy(&sprite->header);
}

void particle_freeQuadSpriteDefault(QuadSprite *sprite) {
    particle_freeQuadSpriteLights(sprite);
    effect_defaultFree(&sprite->header);
}

/* Releases a sprite's light buffers. */
void particle_freeQuadSpriteLights(QuadSprite *sprite) {
    s32 i;

    // matching: the (s32) round-trips make IDO copy the buffer pointer through an int temp
    for (i = 0; i <= gScreenLayout; i++) {
        if (sprite->link.handles[i]) {
            heap_setTag((void *)(s32)sprite->link.handles[i], 4);
        }
    }
    if (sprite->link.sharedHandle != NULL) {
        heap_setTag((void *)(s32)sprite->link.sharedHandle, 4);
    }
}

void particle_destroyQuadSprite(QuadSprite *sprite) {
    s32 index;

    if (sprite->def.flags & 0x1000) {
        index = sprite->def.unk7;
    } else {
        index = 0;
    }
    gQuadSpriteDestroyFuncs[index](sprite);
}

void particle_freeQuadSprite(QuadSprite *sprite) {
    s32 index;

    if (sprite->def.flags & 0x1000) {
        index = sprite->def.unk7;
    } else {
        index = 0;
    }
    gQuadSpriteFreeFuncs[index](sprite);
}

/* Frees a particle's per-viewport vertex buffers. */
void particle_freeQuadVtxBuffers(DefParticleHead *particle) {
    particle_freeViewBuffers(particle->vtxBuffers);
}

void particle_destroyQuadParticleDefault(DefParticleHead *particle) {
    particle_freeQuadVtxBuffers(particle);
    particle_freeQuadParticleLights(particle);
    effect_defaultDestroy(&particle->header);
}

void particle_freeQuadParticleDefault(DefParticleHead *particle) {
    particle_freeQuadVtxBuffers(particle);
    particle_freeQuadParticleLights(particle);
    effect_defaultFree(&particle->header);
}

/* Releases a particle's light buffers. */
void particle_freeQuadParticleLights(DefParticleHead *arg0) {
    s32 i;

    // matching: the (s32) round-trips make IDO copy the buffer pointer through an int temp
    for (i = 0; i <= gScreenLayout; i++) {
        if (arg0->link.handles[i]) {
            heap_setTag((void *)(s32)arg0->link.handles[i], 4);
        }
    }
    if (arg0->link.sharedHandle != NULL) {
        heap_setTag((void *)(s32)arg0->link.sharedHandle, 4);
    }
}

void particle_destroyQuadParticle(DefParticleHead *particle) {
    s32 index;

    if (particle->def.flags & 0x400000) {
        index = particle->def.type;
    } else {
        index = 0;
    }
    gQuadParticleDestroyFuncs[index](particle);
}

void particle_freeQuadParticle(DefParticleHead *particle) {
    s32 index;

    if (particle->def.flags & 0x400000) {
        index = particle->def.type;
    } else {
        index = 0;
    }
    gQuadParticleFreeFuncs[index](particle);
}

/* The render modes of the effects (the blender words of the two cycles), indexed by the low byte of a render mode
 * field; most effect files read it with their own view. */
const RenderModePair gEffectRenderModes[28] = {
{ 0x004049D8, 0x001049D8 },
{ 0x004049D8, 0x001049D8 },
{ 0x00404DD8, 0x00404DD8 },
{ 0x00404350, 0x00104350 },
{ 0x00404358, 0x00104358 },
{ 0x004049F8, 0x001049F8 },
{ 0x00404DF8, 0x00104DF8 },
{ 0x004041C8, 0x001041C8 },
{ 0x00404240, 0x00104240 },
{ 0x00443078, 0x00113078 },
{ 0xC8000000, 0x00112230 },
{ 0xC8000000, 0x00104A50 },
{ 0xC8000000, 0x001049F8 },
{ 0x0C080000, 0x001041C8 },
{ 0x0C080000, 0x001049F8 },
{ 0x0C080000, 0x001049D8 },
{ 0x00442078, 0x00112078 },
{ 0x0C080000, 0x00104A50 },
{ 0x0C083238, 0x03023238 },
{ 0x00442048, 0x00112048 },
{ 0x00442230, 0x00112230 },
{ 0x004049F8, 0x001049F8 },
{ 0x00442078, 0x00112078 },
{ 0x00404340, 0x00104340 },
{ 0x00404870, 0x00104870 },
{ 0x00404840, 0x00104840 },
{ 0x0C080000, 0x00104350 },
{ 0x00404340, 0x00104340 },
};

/* Draws a sprite's quad (and attached model) for one viewport. */
Gfx *particle_drawQuadSprite(Gfx *gfx, QuadSprite *sprite, s16 viewport) {
    s32 geometry;
    u8 needSync;
    u8 blendMode;
    u8 combiner;
    s16 cullFlags;
    s32 pad; // matching: unused stack slot
    s16 primR;
    s16 primG;
    s16 primB;
    s16 primA;
    s16 envR;
    s16 envG;
    s16 envB;
    s16 envA;

    if (sprite->active & 1) {
        if ((sprite->def.flags & 0x2000) && !(sprite->def.viewportMask & (1 << viewport))) {
            return gfx;
        }
        needSync = TRUE;
        blendMode = sprite->def.renderModes & 0xFF;
        combiner = (sprite->def.renderModes & 0xFFFF0000) >> 16;
        cullFlags = sprite->def.flags & 0x30;
        gfx = math_emitPointLights(gfx, &sprite->link, 0, 0, 0, viewport, &sprite->pos, 2,
                            (sprite->def.flags & 0x4000) ? sprite->def.unk1C : 0);
        math_pickPrimColour(&primR, &primG, &primB, &primA, sprite->def.rgba[0], sprite->def.rgba[1], sprite->def.rgba[2],
                      sprite->def.rgba[3], 0xFF, 0xFF, 0xFF, sprite->def.unk10, sprite->def.unk11,
                      (sprite->def.flags & 0x100) ? sprite->def.unk16 : 3);
        math_pickEnvColour(&envR, &envG, &envB, &envA, sprite->def.rgba[0], sprite->def.rgba[1], sprite->def.rgba[2],
                      sprite->def.rgba[3], 0xFF, 0xFF, 0xFF, sprite->def.unk10, sprite->def.unk11,
                      (sprite->def.flags & 0x200) ? sprite->def.unk17 : 4);
        geometry = 0;
        if (cullFlags & 0x10) {
            geometry = G_CULL_FRONT;
        }
        if (cullFlags & 0x20) {
            geometry |= G_CULL_BACK;
        }
        gfx = math_gfxUpdateGeometryMode(gfx, ((sprite->def.flags & 0x400) ? G_LIGHTING : 0) | geometry | G_ZBUFFER, G_CULL_BOTH);
        if (sprite->def.texture != 0xFF) {
            gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[sprite->def.texture], sprite->def.frame, 2, 0, 0, sprite->def.texture,
                                0, 0, &needSync, (sprite->def.flags & 0x20000) ? 0x3E : 3);
        }
        gfx = math_gfxSetEnvColor(gfx, envR, envG, envB, envA, &needSync);
        gfx = math_gfxSetPrimColor(gfx, 0, 0, primR, primG, primB, primA, &needSync);
        gfx = particle_setCombineMode(gfx, combiner, &needSync);
        gfx = math_gfxSetOtherMode(gfx,
                            G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE |
                                ((sprite->def.texture == 0xFF) ? 0 : (u32)gSpriteTlutMode /* matching: unsigned ternary */) | G_TF_BILERP | G_TC_FILT |
                                G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            gEffectRenderModes[blendMode].cycle1 | gEffectRenderModes[blendMode].cycle2, &needSync);

        gSPVertex(gfx++, sprite->vtx, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
    }
    return gfx;
}

void particle_onQuadSpriteEvent(s32 arg0, s32 arg1, u8 arg2) {
    gQuadSpriteEventFuncs[0](arg0, arg1, arg2);
}

void particle_ignoreQuadSpriteEvent(s32 arg0, s32 arg1, u8 arg2) {
}


/* Per-frame update of a particle: lifetime, fading and texture animation. */
void particle_updateQuadParticle(DefParticleHead *particle) {
    s32 speed;
    s8 expired;
    s32 numFrames;
    s32 lastFrame;
    s32 faded;

    if (!(particle->active & 1)) {
        return;
    }

    expired = FALSE;
    if (particle->def.flags & 1) {
        particle->def.lifetime -= gFrameTicks;
        if (particle->def.lifetime < 0) {
            expired = TRUE;
        }
    }
    if (particle->updateFn1 != 0) {
        if (gQuadParticleUpdate1Funcs[particle->updateFn1](particle) == 0) {
            expired = TRUE;
        }
    }
    if (particle->updateFn2 != 0) {
        if (gQuadParticleUpdate2Funcs[particle->updateFn2](particle) == 0) {
            expired = TRUE;
        }
    }

    if ((particle->def.flags & 0x8000000) && !expired) {
        if (particle->def.lifetime < particle->def.fadeStart) {
            faded = particle->def.lifetime * particle->def.fadeRate;
            if (faded < particle->def.unk44) {
                particle->def.unk44 = faded;
            }
        }
    }
    if (particle->def.flags & 0x10000000) {
        if (!expired) {
            if (particle->def.lifetime < particle->def.fadeStart) {
                faded = particle->def.lifetime * particle->def.fadeRate;
                if (faded < particle->def.unk44) {
                    particle->def.rgba[3] = faded;
                }
            }
        }
    }

    if (!expired) {
        speed = particle->def.frameSpeed;
        if (speed != 0) {
            numFrames = gEffectTextures[particle->def.texture].frameCount;
            lastFrame = (numFrames << 16) - 1;
            particle->def.frame += speed * gFrameTicks;
            if (lastFrame < particle->def.frame) {
                if (particle->def.flags & 0x800) {
                    expired = TRUE;
                } else if (particle->def.flags & 0x400) {
                    particle->def.frame = lastFrame - (particle->def.frame % lastFrame);
                    particle->def.frameSpeed = -speed;
                } else {
                    do {
                        particle->def.frame -= lastFrame;
                    } while (lastFrame < particle->def.frame);
                }
            } else if (particle->def.frame < 0) {
                if (particle->def.flags & 0x1000) {
                    expired = TRUE;
                } else if (particle->def.flags & 0x400) {
                    particle->def.frame = (-particle->def.frame) % lastFrame;
                    particle->def.frameSpeed = -speed;
                } else {
                    do {
                        particle->def.frame += lastFrame;
                    } while (particle->def.frame < 0);
                }
            }
        }
    }

    if (expired) {
        if (particle->expireFn != 0) {
            if (gQuadParticleExpireFuncs[particle->expireFn](particle) != 0) {
                expired = FALSE;
            }
        }
    }
    if (expired) {
        effect_destroyObject(&particle->header);
    }
}

/* Allocates a particle from a template; motionSize extra bytes follow it for its behaviour state. */
void *particle_newQuadParticle(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 model,
                    s32 arg8, s32 motionSize, u8 argA, s32 argB) {
    DefParticleHead *particle;
    s32 flags;
    s32 kind;
    s32 i;

    flags = ((ParticleDef *)def)->flags;
    if (flags & 0x800000) {
        kind = 0x56;
    } else if (flags & 0x2000000) {
        kind = 0x49;
    } else {
        kind = 0x1C;
    }
    particle = effect_newObject(kind, argB, motionSize + sizeof(DefParticleHead), 1, argA,
                             (flags & 0x80000000U) ? 2 : 1);
    if (particle == NULL) {
        return NULL;
    }
    memcpy(&particle->def, def, sizeof(ParticleDef));
    particle->updateFn1 = updateFn1;
    particle->updateFn2 = updateFn2;
    particle->expireFn = expireFn;
    particle->vertexFn = vertexFn;
    particle->unk74 = 0;
    bzero(particle->vtxBuffers, sizeof(particle->vtxBuffers));
    particle_setQuadTexCoords(particle->vtxTemplate, particle->def.texture, flip);
    particle_setQuadColors(particle->vtxTemplate, colors);
    particle->active = 1;
    particle->unk14 = 0;
    particle->ground.height = -10000.0f;
    particle->ground.surface = NULL;
    particle->ground.source = 0;
    particle->ground.flags = 0;
    particle->ground.material = 0;
    particle->link.model = model;
    particle->link.unk1C = arg8;
    particle->link.unk4 = 0;
    for (i = 0; i < 4; i++) {
        particle->link.handles[i] = NULL;
    }

    particle->link.sharedHandle = NULL;
    if (model) {
        for (i = 0; i <= gScreenLayout; i++) {
            particle->link.handles[i] = light_allocGfxLights(model);
        }
        particle->link.sharedHandle = func_1515D440();
    }
    return particle;
}

void *particle_newQuadParticleNoModel(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip,
                    s32 motionSize, u8 arg8, s32 arg9) {
    return particle_newQuadParticle(def, colors, updateFn1, updateFn2, expireFn, vertexFn, flip, 0, 0, motionSize, arg8, arg9);
}

/* As particle_newQuadParticleNoModel with the default colours. */
void *particle_newQuadParticleWhite(void *def, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 motionSize,
                    u8 arg7, s32 arg8) {
    return particle_newQuadParticle(def, &gWhiteVtxColors, updateFn1, updateFn2, expireFn, vertexFn, flip, 0, 0, motionSize, arg7, arg8);
}

/* Spawns a particle whose quad is spanned by two axes from particle_computeQuadAxes (vertex function 12). */
void *particle_spawnAxisQuadParticle(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width,
                    f32 height, Vec3s **triPtrs, CollisionTri *surface, s32 model, s32 argC, u8 argD,
                    s32 motionSize, u8 argF, s32 arg10) {
    DefParticleHead *particle;

    if (colors == 0) {
        colors = &gWhiteVtxColors;
    }
    particle = particle_newQuadParticle(def, colors, updateFn1, updateFn2, expireFn, 12, flip, model, argC,
                             motionSize + sizeof(PARTICLE_MOTION(particle)->axes), argF, arg10);
    if ((particle != NULL) &&
        (particle_computeQuadAxes(&PARTICLE_MOTION(particle)->axes, angle, width, height, triPtrs, surface, argD) == 0)) {
        effect_destroyObject(&particle->header);
        return NULL;
    }
    return particle;
}

void *particle_spawnAxisQuadParticleNoModel(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle,
                    f32 width, f32 height, Vec3s **triPtrs, CollisionTri *surface, u8 argB, s32 motionSize, u8 argD,
                    s32 argE) {
    return particle_spawnAxisQuadParticle(def, colors, updateFn1, updateFn2, expireFn, flip, angle, width, height, triPtrs, surface, 0,
                         0, argB, motionSize, argD, argE);
}

/*
 * Computes the two half-axes of a particle quad rotated by angle. With onSlope the quad lies on the
 * slope shadow_getTriSlopes returns (each axis keeps its length); otherwise it lies in the plane of the
 * basis math_getTriEdges returns.
 */
s32 particle_computeQuadAxes(ParticleAxes *axes, s16 angle, f32 width, f32 height, Vec3s **triPtrs, CollisionTri *surface,
                   u8 onSlope) {
    f32 slopeX;
    f32 slopeZ;
    f32 cosA;
    f32 sinA;
    f32 widthSin;
    f32 t;
    f32 heightSin;
    f32 heightCos;
    f32 q1;
    f32 p2;
    f32 q2;
    f32 r2;
    Vec3f v;
    f32 k2;
    f32 cosB;
    f32 sinB;
    Vec3f basisU;
    Vec3f basisV;
    Vec3f basisN;
    f32 len;
    f32 unused2;
    f32 w;

    if (onSlope != 0) {
        cosA = math_cosBinAngleMasked(angle - 0x40);
        sinA = math_cosBinAngleMasked(angle);
        widthSin = width * sinA;
        heightSin = height * sinA;
        if (shadow_getTriSlopes(triPtrs, surface->v, &slopeX, &slopeZ) == 0) {
            return 0;
        }
        {
            f32 r1;

            heightCos = height * cosA;
            t = heightSin * slopeZ;
            // matching: q1, r1 (and p2, q2, r2 below) are dead, but the original computed them
            q1 = fabsf(heightSin);
            r1 = sqrtf((heightSin * heightSin) + (t * t));
            if (0 != heightCos) { // matching: operand order changes the FP compare
                w = heightCos * slopeX;
                v.z = w;
                v.y = fabsf(heightCos) / sqrtf((heightCos * heightCos) + (w * w));
            } else {
                v.z = 0.0f;
                v.y = 0.0f;
            }
            if (heightSin != 0) {
                v.x = t;
                k2 = fabsf(heightSin) / sqrtf((heightSin * heightSin) + (t * t));
            } else {
                v.x = 0.0f;
                k2 = 0.0f;
            }
            axes->u.x = v.y * heightCos;
            axes->u.y = (v.y * v.z) + (k2 * v.x) + 0.5f;
            axes->u.z = k2 * heightSin;
            t = -(width * cosA);
            p2 = t * slopeZ;
            q2 = fabsf(t);
            r2 = sqrtf((t * t) + (p2 * p2));
            if (widthSin != 0) {
                w = widthSin * slopeX; v.z = w; v.y = fabsf(widthSin) / sqrtf((widthSin * widthSin) + (w * w));
            } else {
                v.z = 0.0f; v.y = 0.0f;
            }
            if (t != 0) {
                v.x = t * slopeZ;
                k2 = fabsf(t) / sqrtf((t * t) + ((t * slopeZ) * (t * slopeZ)));
            } else {
                v.x = 0.0f;
                k2 = 0.0f;
            }
            axes->v.x = v.y * widthSin;
            axes->v.y = (v.y * v.z) + (k2 * v.x) + 0.5f;
            axes->v.z = k2 * t;
        }
    } else {
        cosB = math_cosBinAngleMasked(angle - 0x40);
        sinB = math_cosBinAngleMasked(angle);
        if (math_getTriEdges(surface, &basisU, &basisV, &basisN) == 0) {
            return 0;
        }
        if (math_vec3Normalize(&basisU, &basisU, &len, &unused2) == 0) {
            return 0;
        }
        if (math_vec3Normalize(&basisV, &basisV, &len, &unused2) == 0) {
            return 0;
        }
        {
            f32 a;
            f32 b;

            a = width * sinB;
            b = width * cosB;
            axes->u.x = (a * basisU.x) + (b * basisV.x);
            axes->u.y = (a * basisU.y) + (b * basisV.y);
            axes->u.z = (a * basisU.z) + (b * basisV.z);
            a = -height * cosB;
            b = height * sinB;
            axes->v.x = (a * basisU.x) + (b * basisV.x);
            axes->v.y = (a * basisU.y) + (b * basisV.y);
            axes->v.z = (a * basisU.z) + (b * basisV.z);
        }
        return 1;
    }
end: // matching: an unused label keeps the final return 1 in its own basic block
    return 1;
}


/* Draws a particle's quad (and attached model) for one viewport. */
Gfx *particle_drawQuadParticle(Gfx *gfx, DefParticleHead *particle, s16 viewport) {
    s32 pad; // matching: unused stack slot
    u8 needSync;
    s16 primR;
    s16 primG;
    s16 primB;
    s16 primA;
    s16 envR;
    s16 envG;
    s16 envB;
    s16 envA;
    u8 blendMode;
    u8 combiner;
    s16 cullFlags;
    s32 geometry;
    Vtx *vtx;

    // matching: the (s32) round-trips below make IDO copy the buffer pointer through an int temp
    if (particle->active & 1) {
        needSync = TRUE;
        if ((particle->def.flags & 8) && !(particle->def.viewportMask & (1 << viewport))) {
            if (particle->vtxBuffers[viewport]) {
                heap_setTag((void *)(s32)particle->vtxBuffers[viewport], 2);
                particle->vtxBuffers[viewport] = NULL;
            }
            return gfx;
        }
        blendMode = particle->def.renderModes & 0xFF;
        combiner = (particle->def.renderModes & 0xFF00) >> 8;
        cullFlags = particle->def.flags & 0xC000;
        if ((particle->def.flags & 0x1000000) && particle->def.unk48 != NULL &&
            !(particle->def.unk48->views & (1 << viewport))) {
            if (particle->vtxBuffers[viewport]) {
                heap_setTag((void *)(s32)particle->vtxBuffers[viewport], 2);
                particle->vtxBuffers[viewport] = NULL;
            }
            return gfx;
        }
        vtx = gQuadParticleVertexFuncs[particle->vertexFn](particle, viewport);
        if (vtx == NULL) {
            if (particle->vtxBuffers[viewport]) {
                heap_setTag((void *)(s32)particle->vtxBuffers[viewport], 2);
                particle->vtxBuffers[viewport] = NULL;
            }
            return gfx;
        }
        gfx = math_emitPointLights(gfx, &particle->link, 0, 0, 0, viewport, &particle->def.pos, 2,
                            (particle->def.flags & 0x4000000) ? particle->def.unk50 : 0);
        math_pickPrimColour(&primR, &primG, &primB, &primA, particle->def.rgba[0], particle->def.rgba[1],
                      particle->def.rgba[2], particle->def.rgba[3], 0xFF, 0xFF, 0xFF, particle->def.unk44,
                      particle->def.unk45, (particle->def.flags & 0x40000) ? particle->def.unk46 : 4);
        math_pickEnvColour(&envR, &envG, &envB, &envA, particle->def.rgba[0], particle->def.rgba[1],
                      particle->def.rgba[2], particle->def.rgba[3], 0xFF, 0xFF, 0xFF, particle->def.unk44,
                      particle->def.unk45, (particle->def.flags & 0x80000) ? particle->def.unk47 : 5);
        geometry = 0;
        if (cullFlags & 0x4000) {
            geometry = G_CULL_FRONT;
        }
        if (cullFlags & 0x8000) {
            geometry |= G_CULL_BACK;
        }
        if (particle->def.flags & 0x100000) {
            geometry |= G_LIGHTING;
        }
        gfx = math_gfxUpdateGeometryMode(gfx, geometry | G_ZBUFFER, G_CULL_BOTH);
        if (particle->def.texture != 0xFF) {
            s32 clamp;

            clamp = (particle->def.flags & 0x200000) ? 0 : 2;
            gfx = math_gfxLoadTextureFrame(gfx, &gEffectTextures[particle->def.texture], particle->def.frame, clamp, 0, 0,
                                particle->def.texture, 0, 0, &needSync,
                                (particle->def.flags & 0x40000000) ? 0x3E : 3);
        }
        gfx = math_gfxSetEnvColor(gfx, envR, envG, envB, envA, &needSync);
        gfx = math_gfxSetPrimColor(gfx, 0xF2, 0, primR, primG, primB, primA, &needSync);
        gfx = particle_setCombineMode(gfx, combiner, &needSync);
        gfx = math_gfxSetOtherMode(gfx,
                            G_PM_NPRIMITIVE | ((particle->def.flags & 0x20000000) ? G_CYC_2CYCLE : G_CYC_1CYCLE) |
                                G_TP_PERSP | G_TD_CLAMP | G_TL_TILE |
                                ((particle->def.texture == 0xFF) ? 0 : (u32)gSpriteTlutMode /* matching: unsigned ternary */) | G_TF_BILERP | G_TC_FILT |
                                G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            gEffectRenderModes[blendMode].cycle1 | gEffectRenderModes[blendMode].cycle2, &needSync);
        gSPVertex(gfx++, vtx, 4, 0);
        gSP2Triangles(gfx++, 0, 1, 2, 0, 0, 2, 3, 0);
    }
    return gfx;
}


/* Dispatches to the gQuadParticleEventFuncs handler for the particle's type. */
void particle_onQuadParticleEvent(DefParticleHead *particle, s32 arg1, u8 arg2) {
    s32 index;

    if (!(particle->def.flags & 0x400000)) {
        index = 0;
    } else {
        switch (particle->def.type) {
            case 12:
                index = 10;
                break;
            case 11:
                index = 9;
                break;
            case 0:
            case 2:
                index = 0;
                break;
            case 1:
                index = 1;
                break;
            case 5:
                index = 2;
                break;
            case 6:
                index = 3;
                break;
            case 3:
                index = 4;
                break;
            case 7:
                index = 5;
                break;
            case 8:
                index = 6;
                break;
            case 9:
                index = 7;
                break;
            case 4:
                index = 8;
                break;
            default:
                index = 0;
                break;
        }
    }
    gQuadParticleEventFuncs[index](particle, arg1, arg2);
}

void particle_ignoreQuadParticleEvent(DefParticleHead *particle, s32 arg1, u8 arg2) {
}

/* Message handler: frees the particle when its owner goes away (0x1A), or retargets it on an actor swap (0x2D). */
void particle_onQuadParticleOwnerMsg(DefParticleHead *particle, ActorMsg *msg, u8 eventId) {
    ActorRef *link = &PARTICLE_MOTION(particle)->owner;

    if (eventId == 0x1A) {
        if ((link->actor == msg->ref.actor) || (link->uniqueId == msg->ref.uniqueId)) {
            effect_destroyObject(&particle->header);
        }
    } else if (eventId == 0x2D) {
        if (link->actor == msg->swap.actorA) {
            link->actor = msg->swap.actorB;
            link->uniqueId = msg->swap.idB;
        } else if (link->actor == msg->swap.actorB) {
            link->actor = msg->swap.actorA;
            link->uniqueId = msg->swap.idA;
        }
    }
}

void func_1513E134(void) {
}

/* Lays a sprite's quad flat in the xz plane around (x, y, z), rotated by angle. */
void particle_layQuadSpriteFlat(QuadSprite *sprite, f32 x, f32 y, f32 z, f32 width, f32 length, u8 angle) {
    // matching: permuter-found shape; the one-element arrays, (&param)[0] reads, unused p0..p3 and the
    // empty if steer IDO's stack and register allocation (each plain form was tried and mismatches)
    f32 sinA;
    f32 cosA[1];
    f32 widthSin;
    f32 lengthCos;
    f32 lengthSin;
    f32 widthCos;
    f32 frontX;
    f32 t[1];
    f32 p0;
    f32 p1;
    f32 p2;
    f32 p3;
    f32 u[1];

    cosA[0] = math_cosBinAngleMasked(angle - 0x40);
    sinA = math_cosBinAngleMasked(angle);
    widthSin = (&width)[0] * sinA;
    frontX = (&x)[0] + widthSin;
    lengthCos = cosA[0] * (&length)[0];
    if (((!t) && (!t)) && (!t)) {
    }
    sprite->vtx[0].v.ob[0] = frontX - lengthCos;
    sprite->vtx[0].v.ob[1] = y;
    lengthSin = (&length)[0] * sinA;
    u[0] = z + lengthSin;
    widthCos = cosA[0] * (&width)[0];
    sprite->vtx[0].v.ob[2] = u[0] + widthCos;
    t[0] = x - widthSin;
    sprite->vtx[1].v.ob[0] = t[0] - lengthCos;
    sprite->vtx[1].v.ob[1] = y;
    sprite->vtx[1].v.ob[2] = u[0] - widthCos;
    sprite->vtx[2].v.ob[0] = t[0] + lengthCos;
    sprite->vtx[2].v.ob[1] = y;
    t[0] = z - lengthSin;
    sprite->vtx[2].v.ob[2] = t[0] - widthCos;
    sprite->vtx[3].v.ob[0] = frontX + lengthCos;
    sprite->vtx[3].v.ob[1] = y;
    sprite->vtx[3].v.ob[2] = t[0] + widthCos;
}

void func_1513E2A4(void) {
}

/*
 * Places a sprite's quad (half-size width x height, rotated by angle) around (x, y, z): in the plane of
 * the basis math_getTriEdges returns for surface, or with no surface on the slope from shadow_getTriSlopes.
 */
s32 particle_layQuadSpriteOnSurface(QuadSprite *sprite, Vec3s **triPtrs, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                  f32 height, u8 angle) {
    Vtx_t *vtx;
    f32 cosA;
    f32 sinA;
    f32 ux;
    f32 uy;
    f32 uz;
    f32 vx;
    f32 vy;
    f32 vz;
    Vec3f basisU;
    Vec3f basisV;
    f32 widthSin;
    f32 widthCos;
    f32 heightCos;
    f32 heightSin;
    f32 slopeX;
    f32 slopeZ;
    s16 i;
    f32 corners[4][2];
    f32 riseX;
    f32 riseZ;
    f32 scaleZ;
    f32 scaleX;
    f32 unused;
    Vtx_t *v;

    vtx = &sprite->vtx[0].v;
    cosA = math_cosBinAngleMasked(angle - 0x40);
    sinA = math_cosBinAngleMasked(angle);
    if (surface != NULL) {
        v = vtx;
        if (math_getTriEdges(surface, &basisU, &basisV, NULL) == 0) {
            return 0;
        }
        math_vec3Normalize(&basisU, &basisU, NULL, NULL);
        math_vec3Normalize(&basisV, &basisV, NULL, NULL);
        {
            f32 pad; // matching: -g3 scope

            widthSin = width * sinA;
            widthCos = width * cosA;
            ux = widthSin * basisU.x + widthCos * basisV.x;
            uy = widthSin * basisU.y + widthCos * basisV.y;
            uz = widthSin * basisU.z + widthCos * basisV.z;
            heightCos = -height * cosA;
            heightSin = height * sinA;
            vx = heightCos * basisU.x + heightSin * basisV.x;
            vy = heightCos * basisU.y + heightSin * basisV.y;
            vz = heightCos * basisU.z + heightSin * basisV.z;
        }
        v[0].ob[0] = x - ux + vx;
        v[0].ob[1] = y - uy + vy;
        v[0].ob[2] = z - uz + vz;
        v[1].ob[0] = x + ux + vx;
        v[1].ob[1] = y + uy + vy;
        v[1].ob[2] = z + uz + vz;
        v[2].ob[0] = x + ux - vx;
        v[2].ob[1] = y + uy - vy;
        v[2].ob[2] = z + uz - vz;
        v[3].ob[0] = x - ux - vx;
        v[3].ob[1] = y - uy - vy;
        v[3].ob[2] = z - uz - vz;
        return 1;
    }
    if (shadow_getTriSlopes(triPtrs, surface->v, &slopeX, &slopeZ) == 0) {
        return 0;
    }
    heightSin = height * sinA;
    widthSin = width * sinA;
    heightCos = height * cosA;
    widthCos = width * cosA;
    corners[0][0] = widthSin - heightCos;
    corners[0][1] = heightSin + widthCos;
    corners[1][0] = -widthSin - heightCos;
    corners[1][1] = heightSin - widthCos;
    corners[2][0] = -widthSin + heightCos;
    corners[2][1] = -heightSin - widthCos;
    corners[3][0] = widthSin + heightCos;
    corners[3][1] = -heightSin + widthCos;
    for (i = 0; i < 4; i++) {
        f32 pad2; // matching: -g3 scope

        riseZ = corners[i][1] * slopeZ;
        // matching: dead stores the original made
        scaleZ = fabsf(corners[i][1]);
        scaleZ = fabsf(corners[i][1] * corners[i][1]);
        scaleZ = fabsf(riseZ); scaleZ = fabsf(riseZ - 1.0f);
        if (corners[i][0] != 0) {
            riseX = corners[i][0] * slopeX;
            scaleX = fabsf(corners[i][0]) / sqrtf(corners[i][0] * corners[i][0] + riseX * riseX);
        } else {
            riseX = 0.0f;
            scaleX = 0.0f;
        }
        if (corners[i][1] != 0) {
            scaleZ = fabsf(corners[i][1]) / sqrtf(corners[i][1] * corners[i][1] + riseZ * riseZ);
        } else {
            riseZ = 0.0f;
            scaleZ = 0.0f;
        }
        vtx->ob[0] = corners[i][0] * scaleX + x;
        vtx->ob[1] = scaleX * riseX + y + scaleZ * riseZ + 0.5f;
        vtx->ob[2] = corners[i][1] * scaleZ + z;
        vtx++;
    }
    return 1;
}

/* Moves, rotates and scales a particle by its drift velocities times the frame time, with optional gravity. */
s32 particle_updateDriftMotion(DefParticleHead *particle) {
    DefParticleMotion *motion;

    motion = PARTICLE_MOTION(particle);
    if (particle->def.flags & 0x20) {
        motion->drift.vel.y += motion->drift.gravity * gFrameDelta;
    }
    if (particle->def.flags & 0x40) {
        particle->def.pos.x += motion->drift.vel.x * gFrameDelta;
        particle->def.pos.y += motion->drift.vel.y * gFrameDelta;
        particle->def.pos.z += motion->drift.vel.z * gFrameDelta;
    }
    if (particle->def.flags & 0x80) {
        particle->def.rot.x += motion->drift.rotVel.x * gFrameDelta;
        particle->def.rot.y += motion->drift.rotVel.y * gFrameDelta;
        particle->def.rot.z += motion->drift.rotVel.z * gFrameDelta;
    }
    if (particle->def.flags & 0x100) {
        particle->def.scale.x += motion->drift.scaleVel.x * gFrameDelta;
        particle->def.scale.y += motion->drift.scaleVel.y * gFrameDelta;
        particle->def.scale.z += motion->drift.scaleVel.z * gFrameDelta;
    }
    particle->def.unk45 = 0xFF;
    return 1;
}

/*
 * Vertex functions (gQuadParticleVertexFuncs): each fetches the particle's vertex buffer for a viewport (refilled
 * from vtxTemplate when new), writes the quad's corner positions and returns it, or NULL.
 */

/* Quad of width x height, rotated by def.rot, scaled by def.scale and placed at def.pos. */
Vtx *particle_buildRotatedQuad(DefParticleHead *particle, s16 viewport) {
    MtxF mtx;
    s32 pad; // matching: unused stack slot
    Vtx *vertices;
    Vtx *result;
    f32 corners[4][3];
    u8 fresh;
    s16 i;

    particle_getViewBuffer(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    {
        s16 pad; // matching: -g3 scope; its .bgnb splits the &mtx init from the first load

        corners[0][0] = particle->def.width;
        corners[0][1] = particle->def.height;
        corners[1][0] = -particle->def.width;
        corners[1][1] = particle->def.height;
        corners[2][0] = -particle->def.width;
        corners[2][1] = -particle->def.height;
        corners[3][0] = particle->def.width;
        corners[3][1] = -particle->def.height;
        math_mtxRotateRPYF(mtx, particle->def.rot.x, particle->def.rot.y, particle->def.rot.z);
        mtx[3][0] = particle->def.pos.x;
        mtx[3][1] = particle->def.pos.y;
        mtx[3][2] = particle->def.pos.z;
        mtx[0][0] *= particle->def.scale.x;
        mtx[0][1] *= particle->def.scale.x;
        mtx[0][2] *= particle->def.scale.x;
        mtx[1][0] *= particle->def.scale.y;
        mtx[1][1] *= particle->def.scale.y;
        mtx[1][2] *= particle->def.scale.y;
        mtx[2][0] *= particle->def.scale.z;
        mtx[2][1] *= particle->def.scale.z;
        mtx[2][2] *= particle->def.scale.z;
        for (i = 0; i < 4; i++) {
            f32 *c = corners[i];

            math_transformPoint(mtx, c[0], c[1], 0.0f, &c[0], &c[1], &c[2]);
            vertices->v.ob[0] = c[0];
            vertices->v.ob[1] = c[1];
            vertices->v.ob[2] = c[2];
            vertices->v.flag = 0;
            vertices++;
        }
    }
    return result;
}

/*
 * Upright quad of width x height facing the viewport's camera (gCameraAngleSines / gCameraAngleCosines hold its
 * direction), scaled vertically by def.scale.y; reversed mirrors the corner order.
 */
Vtx *particle_buildUprightQuad(particle, reversed, viewport)
    DefParticleHead *particle;
    u8 reversed;
    s16 viewport;
{
    Vtx *vertices;
    Vtx *result;
    s16 corners[4][3];
    f32 heightDelta, heightScale, offsetZ, offsetX;
    u8 fresh;

    particle_getViewBuffer(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    offsetZ = gCameraAngleSines[viewport] * particle->def.width;
    offsetX = gCameraAngleCosines[viewport] * particle->def.width;
    heightScale = particle->def.scale.y * particle->def.height;
    heightDelta = particle->def.height - heightScale;
    vertices[0].v.flag = 0;
    vertices[1].v.flag = 0;
    vertices[2].v.flag = 0;
    vertices[3].v.flag = 0;
    corners[3][0] = corners[0][0] = particle->def.pos.x + offsetX;
    corners[1][1] = corners[0][1] = particle->def.pos.y - heightScale;
    corners[3][2] = corners[0][2] = particle->def.pos.z - offsetZ;
    corners[2][0] = corners[1][0] = particle->def.pos.x - offsetX;
    corners[3][1] = corners[2][1] = particle->def.pos.y + heightDelta;
    corners[2][2] = corners[1][2] = particle->def.pos.z + offsetZ;
    if (reversed != 0) {
        vertices[0].v.ob[0] = corners[1][0];
        vertices[0].v.ob[1] = corners[1][1];
        vertices[0].v.ob[2] = corners[1][2];
        vertices[1].v.ob[0] = corners[2][0];
        vertices[1].v.ob[1] = corners[2][1];
        vertices[1].v.ob[2] = corners[2][2];
        vertices[2].v.ob[0] = corners[3][0];
        vertices[2].v.ob[1] = corners[3][1];
        vertices[2].v.ob[2] = corners[3][2];
        vertices[3].v.ob[0] = corners[0][0];
        vertices[3].v.ob[1] = corners[0][1];
        vertices[3].v.ob[2] = corners[0][2];
    } else {
        vertices[0].v.ob[0] = corners[0][0];
        vertices[0].v.ob[1] = corners[0][1];
        vertices[0].v.ob[2] = corners[0][2];
        vertices[1].v.ob[0] = corners[1][0];
        vertices[1].v.ob[1] = corners[1][1];
        vertices[1].v.ob[2] = corners[1][2];
        vertices[2].v.ob[0] = corners[2][0];
        vertices[2].v.ob[1] = corners[2][1];
        vertices[2].v.ob[2] = corners[2][2];
        vertices[3].v.ob[0] = corners[3][0];
        vertices[3].v.ob[1] = corners[3][1];
        vertices[3].v.ob[2] = corners[3][2];
    }
    return result;
}

Vtx *particle_buildUprightQuadFwd(void *particle, s16 viewport) {
    return particle_buildUprightQuad(particle, FALSE, viewport);
}

Vtx *particle_buildUprightQuadRev(void *particle, s16 viewport) {
    return particle_buildUprightQuad(particle, TRUE, viewport);
}

/* Quad of width x height, rotated by def.rot and placed at def.pos (unrolled form of particle_buildRotatedRectQuad). */
Vtx *particle_buildRotatedRectQuadUnrolled(DefParticleHead *particle, s16 viewport) {
    Vtx *vertices;
    Vtx *result;
    MtxF mtx;
    f32 corners[4][2];
    f32 out[4][3];
    u8 fresh;

    particle_getViewBuffer(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    math_mtxRotateRPYF(mtx, particle->def.rot.x, particle->def.rot.y, particle->def.rot.z);
    mtx[3][0] = particle->def.pos.x;
    mtx[3][1] = particle->def.pos.y;
    mtx[3][2] = particle->def.pos.z;
    corners[0][0] = particle->def.width;
    corners[0][1] = particle->def.height;
    corners[1][0] = -particle->def.width;
    corners[1][1] = particle->def.height;
    corners[2][0] = -particle->def.width;
    corners[2][1] = -particle->def.height;
    corners[3][0] = particle->def.width;
    corners[3][1] = -particle->def.height;
    math_transformPoint(mtx, corners[0][0], corners[0][1], 0.0f, &out[0][0], &out[0][1], &out[0][2]);
    math_transformPoint(mtx, corners[1][0], corners[1][1], 0.0f, &out[1][0], &out[1][1], &out[1][2]);
    math_transformPoint(mtx, corners[2][0], corners[2][1], 0.0f, &out[2][0], &out[2][1], &out[2][2]);
    math_transformPoint(mtx, corners[3][0], corners[3][1], 0.0f, &out[3][0], &out[3][1], &out[3][2]);
    vertices->v.ob[0] = out[0][0];
    vertices->v.ob[1] = out[0][1];
    vertices->v.ob[2] = out[0][2];
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = out[1][0];
    vertices->v.ob[1] = out[1][1];
    vertices->v.ob[2] = out[1][2];
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = out[2][0];
    vertices->v.ob[1] = out[2][1];
    vertices->v.ob[2] = out[2][2];
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = out[3][0];
    vertices->v.ob[1] = out[3][1];
    vertices->v.ob[2] = out[3][2];
    vertices->v.flag = 0;
    return result;
}

/*
 * Camera-facing upright quad that bends towards the point def.rot: the bend angle grows with the
 * square root of that point's sideways offset (scaled by def.scale.x), capped at 90 degrees.
 */
Vtx *particle_buildBendingQuad(DefParticleHead *particle, s16 viewport) {
    Vtx *vertices;
    Vtx *result;
    f32 sinDir;
    f32 cosDir;
    f32 bottom;
    f32 top;
    f32 s;
    f32 c;
    f32 wc;
    f32 ws;
    f32 dist;
    f32 sign;
    f32 bs;
    f32 bc;
    f32 ts;
    f32 tc;
    f32 angle;
    u8 fresh;

    particle_getViewBuffer(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    { // matching: this -g3 block scope steers the scheduler
        sinDir = gCameraAngleSines[viewport];
        cosDir = gCameraAngleCosines[viewport];
        bottom = -(particle->def.scale.y * particle->def.height);
        top = particle->def.height + bottom;
        dist = (((particle->def.rot.x - particle->def.pos.x) * cosDir) -
                (sinDir * (particle->def.rot.z - particle->def.pos.z))) * gFrameDeltaInv;
        sign = -1.0f;
        if (dist < 0.0f) {
            sign = 1.0f;
            dist = -dist;
        }
        angle = sqrtf(dist) * particle->def.scale.x;
        if (angle > 1.57079637050628662109375f) {
            angle = 1.57079637050628662109375f;
        }
        angle *= sign;
        s = sinf(angle);
        c = cosf(angle);
        wc = particle->def.width * c;
        ws = particle->def.width * s;
        ts = top * s;
        tc = top * c;
        bs = bottom * s;
        bc = bottom * c;
        vertices[0].v.ob[0] = particle->def.pos.x + ((wc - bs) * cosDir);
        vertices[0].v.ob[1] = (particle->def.pos.y + bc) + ws;
        vertices[0].v.ob[2] = particle->def.pos.z - ((wc - bs) * sinDir);
        vertices[0].v.flag = 0;
        dist = -(wc + bs);
        vertices[1].v.ob[0] = particle->def.pos.x + (dist * cosDir);
        vertices[1].v.ob[1] = (particle->def.pos.y + bc) - ws;
        vertices[1].v.ob[2] = particle->def.pos.z - (dist * sinDir);
        vertices[1].v.flag = 0;
        vertices[2].v.ob[0] = particle->def.pos.x + ((-(wc + ts)) * cosDir);
        vertices[2].v.ob[1] = (particle->def.pos.y + tc) - ws;
        vertices[2].v.ob[2] = particle->def.pos.z - ((-(wc + ts)) * sinDir);
        vertices[2].v.flag = 0;
        dist = wc - ts;
        vertices[3].v.ob[0] = particle->def.pos.x + (dist * cosDir);
        vertices[3].v.ob[1] = (particle->def.pos.y + tc) + ws;
        vertices[3].v.ob[2] = particle->def.pos.z - (dist * sinDir);
        vertices[3].v.flag = 0;
    }
    return result;
}

/* Quad spanned by the two axes in motion.axes (vertex function 12). */
void particle_buildAxisQuad(DefParticleHead *head, s16 viewport) {
    DefParticleHead *particle = head;

    particle_buildQuadFromAxes(particle, &PARTICLE_MOTION(particle)->axes.u, &PARTICLE_MOTION(particle)->axes.v, viewport);
}

/*
 * Combiner modes: each 16-byte entry holds the a, b, c, d inputs of cycle 0 colour, cycle 0 alpha,
 * cycle 1 colour and cycle 1 alpha, in that order.
 */
static const u8 gCombineModes[92][16] = {
    { 1, 31, 5, 3, 1, 7, 5, 3, 1, 31, 5, 3, 1, 7, 5, 3 },
    { 1, 3, 5, 31, 1, 3, 5, 7, 1, 3, 5, 31, 1, 3, 5, 7 },
    { 1, 31, 5, 3, 1, 7, 5, 3, 1, 31, 5, 3, 1, 7, 5, 3 },
    { 1, 31, 5, 3, 1, 7, 5, 7, 1, 31, 5, 3, 1, 7, 5, 7 },
    { 31, 31, 5, 3, 1, 7, 5, 7, 31, 31, 5, 3, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 4, 7, 1, 31, 5, 31, 1, 7, 4, 7 },
    { 1, 31, 5, 31, 1, 7, 4, 7, 1, 31, 5, 31, 1, 7, 4, 7 },
    { 1, 31, 5, 31, 1, 7, 4, 7, 1, 31, 5, 31, 1, 7, 4, 7 },
    { 1, 31, 5, 3, 1, 7, 5, 7, 1, 31, 5, 3, 1, 7, 5, 7 },
    { 1, 31, 5, 3, 1, 7, 5, 7, 31, 31, 31, 0, 7, 7, 7, 0 },
    { 1, 3, 5, 31, 1, 7, 5, 7, 31, 31, 31, 0, 7, 7, 7, 0 },
    { 1, 31, 5, 31, 1, 7, 4, 7, 1, 31, 5, 31, 1, 7, 4, 7 },
    { 31, 31, 31, 1, 1, 7, 3, 7, 31, 31, 31, 1, 1, 7, 3, 7 },
    { 3, 5, 1, 5, 3, 7, 1, 7, 3, 5, 1, 5, 3, 7, 1, 7 },
    { 3, 5, 8, 5, 3, 7, 1, 7, 3, 5, 8, 5, 3, 7, 1, 7 },
    { 3, 5, 1, 5, 3, 7, 2, 7, 31, 31, 31, 0, 7, 7, 7, 0 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 1, 31, 3, 31, 1, 7, 3, 7, 1, 31, 3, 31, 1, 7, 3, 7 },
    { 1, 31, 2, 31, 1, 7, 2, 7, 0, 31, 3, 31, 0, 7, 3, 7 },
    { 31, 31, 31, 1, 7, 7, 7, 1, 31, 31, 31, 1, 7, 7, 7, 1 },
    { 31, 31, 31, 1, 1, 7, 3, 7, 31, 31, 31, 1, 1, 7, 3, 7 },
    { 7, 31, 3, 31, 7, 7, 7, 3, 7, 31, 3, 31, 7, 7, 7, 3 },
    { 31, 31, 31, 4, 4, 7, 5, 7, 31, 31, 31, 4, 4, 7, 5, 7 },
    { 1, 31, 5, 31, 7, 7, 7, 1, 1, 31, 5, 31, 7, 7, 7, 1 },
    { 31, 31, 31, 4, 7, 7, 7, 5, 31, 31, 31, 4, 7, 7, 7, 5 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 1, 7, 4, 7, 1, 31, 4, 31, 1, 7, 4, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 3, 31, 1, 7, 5, 7, 1, 31, 3, 31, 1, 7, 5, 7 },
    { 1, 5, 15, 3, 3, 7, 1, 7, 1, 5, 15, 3, 3, 7, 1, 7 },
    { 1, 31, 5, 31, 1, 7, 3, 7, 1, 31, 5, 31, 1, 7, 3, 7 },
    { 3, 5, 8, 5, 3, 7, 1, 7, 3, 5, 8, 5, 3, 7, 1, 7 },
    { 2, 1, 13, 1, 2, 1, 0, 1, 4, 31, 0, 31, 0, 7, 4, 7 },
    { 31, 31, 31, 1, 1, 7, 5, 7, 31, 31, 31, 1, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 1, 7, 4, 7, 31, 31, 31, 1, 1, 7, 4, 7 },
    { 31, 31, 31, 3, 1, 7, 4, 7, 31, 31, 31, 3, 1, 7, 4, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 3, 7, 1, 31, 5, 31, 1, 7, 3, 7 },
    { 1, 31, 5, 31, 1, 7, 3, 7, 1, 31, 5, 31, 1, 7, 3, 7 },
    { 1, 31, 5, 31, 1, 7, 3, 7, 1, 31, 5, 31, 1, 7, 3, 7 },
    { 31, 31, 31, 1, 7, 7, 7, 1, 31, 31, 31, 1, 7, 7, 7, 1 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 7, 7, 7, 1, 31, 31, 31, 1, 7, 7, 7, 1 },
    { 1, 31, 5, 31, 7, 7, 7, 1, 6, 5, 3, 0, 7, 7, 7, 0 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 0, 31, 4, 31, 7, 7, 7, 0 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 31, 31, 31, 6, 7, 7, 7, 6, 31, 31, 31, 6, 7, 7, 7, 6 },
    { 31, 31, 31, 4, 7, 7, 7, 4, 31, 31, 31, 4, 7, 7, 7, 4 },
    { 1, 31, 4, 31, 1, 7, 4, 7, 1, 31, 4, 31, 1, 7, 4, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 4, 7, 5, 7, 1, 31, 5, 31, 4, 7, 5, 7 },
    { 31, 31, 31, 4, 7, 7, 7, 4, 31, 31, 31, 4, 7, 7, 7, 4 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 1, 7, 5, 7, 31, 31, 31, 1, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 1, 7, 5, 7, 31, 31, 31, 1, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 7, 7, 7, 1, 31, 31, 31, 1, 7, 7, 7, 1 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 1, 7, 4, 7, 31, 31, 31, 1, 1, 7, 4, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 7, 7, 7, 6, 1, 31, 4, 31, 7, 7, 7, 6 },
    { 5, 31, 4, 31, 1, 7, 5, 7, 5, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 7, 7, 7, 1, 1, 31, 4, 31, 7, 7, 7, 1 },
    { 31, 31, 31, 4, 4, 7, 5, 7, 31, 31, 31, 4, 4, 7, 5, 7 },
    { 31, 31, 31, 1, 1, 7, 3, 7, 31, 31, 31, 1, 1, 7, 3, 7 },
    { 31, 31, 31, 1, 1, 7, 4, 7, 31, 31, 31, 1, 1, 7, 4, 7 },
    { 1, 31, 5, 31, 1, 7, 4, 7, 1, 31, 5, 31, 1, 7, 4, 7 },
    { 1, 31, 4, 31, 1, 7, 4, 7, 1, 31, 4, 31, 1, 7, 4, 7 },
    { 31, 31, 31, 6, 7, 7, 7, 4, 31, 31, 31, 6, 7, 7, 7, 4 },
    { 1, 31, 3, 31, 1, 7, 3, 7, 6, 1, 5, 0, 7, 7, 7, 0 },
    { 3, 5, 1, 5, 1, 7, 5, 7, 3, 5, 1, 5, 1, 7, 5, 7 },
    { 1, 3, 5, 3, 1, 7, 3, 7, 1, 3, 5, 3, 1, 7, 3, 7 },
    { 31, 31, 31, 1, 7, 7, 7, 5, 31, 31, 31, 1, 7, 7, 7, 5 },
    { 31, 31, 31, 5, 7, 7, 7, 5, 31, 31, 31, 5, 7, 7, 7, 5 },
    { 31, 31, 31, 6, 1, 7, 5, 7, 31, 31, 31, 6, 1, 7, 5, 7 },
    { 31, 31, 31, 6, 1, 7, 4, 7, 31, 31, 31, 6, 1, 7, 4, 7 },
    { 31, 31, 31, 5, 1, 7, 4, 7, 31, 31, 31, 5, 1, 7, 4, 7 },
    { 1, 31, 5, 31, 5, 7, 4, 7, 1, 31, 5, 31, 5, 7, 4, 7 },
    { 31, 31, 31, 5, 5, 7, 4, 7, 31, 31, 31, 5, 5, 7, 4, 7 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 0, 0, 7, 4, 7 },
};

/* Sets the colour combiner to mode combiner unless it is already current. */
Gfx *particle_setCombineMode(Gfx *gfx, u8 combiner, u8 *needSync) {
    s32 key;
    const u8 *mode;

    key = func_15143044(combiner, 0);
    if (key != gCachedCombineKey) {
        if (*needSync == TRUE) {
            gDPPipeSync(gfx++);
            *needSync = FALSE;
        }
        mode = gCombineModes[combiner];
        gDPSetCombine(gfx++,
                      GCCc0w0(mode[0], mode[2], mode[4], mode[6]) | GCCc1w0(mode[8], mode[10]),
                      GCCc0w1(mode[1], mode[3], mode[5], mode[7]) |
                          GCCc1w1(mode[9], mode[12], mode[14], mode[11], mode[13], mode[15]));
        gCachedCombineKey = key;
    }
    return gfx;
}

/* Sets a particle's update, expiry and vertex function indices. */
void particle_setQuadParticleFuncs(DefParticleHead *particle, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn) {
    particle->updateFn1 = updateFn1;
    particle->updateFn2 = updateFn2;
    particle->expireFn = expireFn;
    particle->vertexFn = vertexFn;
}

/* Sets a sprite's update and expiry function indices. */
void particle_setQuadSpriteFuncs(QuadSprite *sprite, u8 updateFn, u8 expireFn) {
    sprite->updateFn = updateFn;
    sprite->expireFn = expireFn;
}

/* Grows a particle's width and height by motion.grow.rate times the frame time. */
s32 particle_updateGrowMotion(DefParticleHead *particle) {
    f32 rate = PARTICLE_MOTION(particle)->grow.rate;

    particle->def.width += rate * gFrameDelta;
    particle->def.height += rate * gFrameDelta;
    return 1;
}

/*
 * Ballistic motion: applies drag once per elapsed tick and gravity, then moves the particle by the
 * average of its old and new velocity; also spins it by motion.ballistic.rotVel.
 */
s32 particle_updateBallisticMotion(DefParticleHead *particle) {
    DefParticleMotion *motion;
    Vec3f oldVel;
    s32 i;

    motion = PARTICLE_MOTION(particle);
    oldVel = motion->ballistic.vel;
    for (i = gFrameTicks; i != 0; i--) {
        motion->ballistic.vel.x *= motion->ballistic.drag;
        motion->ballistic.vel.y *= motion->ballistic.drag;
        motion->ballistic.vel.z *= motion->ballistic.drag;
    }
    {
        f32 ax;
        f32 ay;
        f32 az;

        motion->ballistic.vel.y += motion->ballistic.gravity * gFrameDelta;
        ax = (motion->ballistic.vel.x - oldVel.x) * gFrameDeltaInv;
        ay = (motion->ballistic.vel.y - oldVel.y) * gFrameDeltaInv;
        az = (motion->ballistic.vel.z - oldVel.z) * gFrameDeltaInv;
        particle->def.pos.x += (oldVel.x + 0.5f * ax * gFrameDelta) * gFrameDelta;
        particle->def.pos.y += (oldVel.y + 0.5f * ay * gFrameDelta) * gFrameDelta;
        particle->def.pos.z += (oldVel.z + 0.5f * az * gFrameDelta) * gFrameDelta;
    }
    particle->def.rot.x += motion->ballistic.rotVel.x * gFrameDelta;
    particle->def.rot.y += motion->ballistic.rotVel.y * gFrameDelta;
    particle->def.rot.z += motion->ballistic.rotVel.z * gFrameDelta;
    return 1;
}

void particle_buildBeamQuadFwd(DefParticleHead *particle, s16 viewport) {
    BeamScale scale;

    scale.width = 1.0f;
    scale.length = 1.0f;
    particle_buildBeamQuad(particle, FALSE, &scale, viewport);
}

void particle_buildBeamQuadRev(DefParticleHead *particle, s16 viewport) {
    BeamScale scale;

    scale.width = 1.0f;
    scale.length = 1.0f;
    particle_buildBeamQuad(particle, TRUE, &scale, viewport);
}

/*
 * Beam from def.pos towards def.rot (its length scaled by def.height * scale->length), def.width *
 * scale->width wide and turned to face the viewport's camera; reversed rotates the corner order.
 */
Vtx *particle_buildBeamQuad(DefParticleHead *particle, u8 reversed, BeamScale *scale, s16 viewport) {
    Vtx *vertices;
    Vtx *result;
    Vec3f *camera;
    Vec3f delta;
    Vec3f end;
    Vec3f middle;
    Vec3f view;
    Vec3f side;
    f32 lengthSquared;
    f32 length;
    f32 width;
    u8 fresh;

    particle_getViewBuffer(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    camera = math_getCameraPos(viewport);
    width = particle->def.width * scale->width;
    length = particle->def.height * scale->length;
    delta.x = particle->def.rot.x - particle->def.pos.x;
    delta.y = particle->def.rot.y - particle->def.pos.y;
    delta.z = particle->def.rot.z - particle->def.pos.z;
    delta.x *= length;
    delta.y *= length;
    delta.z *= length;
    end.x = particle->def.pos.x + delta.x;
    end.y = particle->def.pos.y + delta.y;
    end.z = particle->def.pos.z + delta.z;
    middle.x = particle->def.pos.x + delta.x * 0.5f;
    middle.y = particle->def.pos.y + delta.y * 0.5f;
    middle.z = particle->def.pos.z + delta.z * 0.5f;
    view.x = middle.x - camera->x;
    view.y = middle.y - camera->y;
    view.z = middle.z - camera->z;
    math_cross3(&delta.x, &view.x, &side.x);
    lengthSquared = side.x * side.x + side.y * side.y + side.z * side.z;
    if (lengthSquared != 0.0f) {
        f32 sideScale = width / sqrtf(lengthSquared);

        side.x = side.x * sideScale;
        side.y = side.y * sideScale;
        side.z = side.z * sideScale;
    } else {
        side.x = 0;
        side.y = 0;
        side.z = 0;
    }
    if (reversed != 0) {
        vertices->v.ob[0] = particle->def.pos.x + side.x;
        vertices->v.ob[1] = particle->def.pos.y + side.y;
        vertices->v.ob[2] = particle->def.pos.z + side.z;
        vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = particle->def.pos.x - side.x;
        vertices->v.ob[1] = particle->def.pos.y - side.y;
        vertices->v.ob[2] = particle->def.pos.z - side.z;
        vertices->v.flag = 0;
        // matching: IDO ugen reads stale tree-node memory left by earlier functions in this file;
        // these empty statements pad the node count so that state lines up
        ;
        vertices++;
        vertices->v.ob[0] = end.x - side.x;
        vertices->v.ob[1] = end.y - side.y;
        vertices->v.ob[2] = end.z - side.z;
        vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = end.x + side.x;
        vertices->v.ob[1] = end.y + side.y;
        vertices->v.ob[2] = end.z + side.z;
        vertices->v.flag = 0;
    } else {
        vertices->v.ob[0] = particle->def.pos.x - side.x;
        vertices->v.ob[1] = particle->def.pos.y - side.y;
        vertices->v.ob[2] = particle->def.pos.z - side.z;
        vertices->v.flag = 0;
        ;;;;;;;;
        vertices++;
        vertices->v.ob[0] = end.x - side.x;
        vertices->v.ob[1] = end.y - side.y;
        vertices->v.ob[2] = end.z - side.z;
        vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = end.x + side.x;
        vertices->v.ob[1] = end.y + side.y;
        vertices->v.ob[2] = end.z + side.z;
        vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = particle->def.pos.x + side.x;
        vertices->v.ob[1] = particle->def.pos.y + side.y;
        vertices->v.ob[2] = particle->def.pos.z + side.z;
        vertices->v.flag = 0;
    }
    return result;
}

/* Sets the texture coordinates of a quad to cover the whole texture, mirrored per the flip bits. */
void particle_setQuadTexCoords(Vtx *vtx, u8 texture, u8 flip) {
    TextureDesc *tex;
    u16 maxS;
    u16 maxT;
    s32 coord;

    if (texture != 0xFF) {
        tex = &gEffectTextures[texture];
        maxS = tex->width - 1;
        maxT = tex->height - 1;

        if (flip & 1) {
            coord = maxS << 6;
        } else {
            coord = 0;
        }
        vtx[3].v.tc[0] = coord;
        vtx[0].v.tc[0] = coord;

        if (flip & 1) {
            coord = 0;
        } else {
            coord = maxS << 6;
        }
        vtx[2].v.tc[0] = coord;
        vtx[1].v.tc[0] = coord;

        if (flip & 2) {
            coord = maxT << 6;
        } else {
            coord = 0;
        }
        vtx[1].v.tc[1] = coord;
        vtx[0].v.tc[1] = coord;

        if (flip & 2) {
            coord = 0;
        } else {
            coord = maxT << 6;
        }
        vtx[3].v.tc[1] = coord;
        vtx[2].v.tc[1] = coord;
    }
}

/* Copies four vertex colours into a quad. */
void particle_setQuadColors(Vtx *vtx, const VtxColor *colors) {
    s32 i;

    for (i = 0; i < 4; i++) {
        vtx->v.flag = colors->flag;
        vtx->v.cn[0] = colors->r;
        vtx->v.cn[1] = colors->g;
        vtx->v.cn[2] = colors->b;
        vtx->v.cn[3] = colors->a;
        vtx->v.flag = 0;
        vtx++;
        colors++;
    }
}

/* Quad of width x height, rotated by def.rot and placed at def.pos. */
Vtx *particle_buildRotatedRectQuad(DefParticleHead *particle, s16 viewport) {
    Vtx *vertices;
    Vtx *result;
    MtxF mtx;
    f32 corners[4][3];
    u8 i;
    u8 fresh;

    particle_getViewBuffer(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    {
        s16 pad; // matching: -g3 scope

        corners[0][0] = particle->def.width;
        corners[0][1] = particle->def.height;
        corners[0][2] = 0.0f;
        corners[1][0] = -particle->def.width;
        corners[1][1] = particle->def.height;
        corners[1][2] = 0.0f;
        corners[2][0] = -particle->def.width;
        corners[2][1] = -particle->def.height;
        corners[2][2] = 0.0f;
        corners[3][0] = particle->def.width;
        corners[3][1] = -particle->def.height;
        corners[3][2] = 0.0f;
        math_mtxRotateRPYF(mtx, particle->def.rot.x, particle->def.rot.y, particle->def.rot.z);
        mtx[3][0] = particle->def.pos.x;
        mtx[3][1] = particle->def.pos.y;
        mtx[3][2] = particle->def.pos.z;
        for (i = 0; i < 4; i++) {
            f32 *c = corners[i];

            math_transformPoint(mtx, c[0], c[1], 0.0f, &c[0], &c[1], &c[2]);
            vertices->v.ob[0] = c[0];
            vertices->v.ob[1] = c[1];
            vertices->v.ob[2] = c[2];
            vertices->v.flag = 0;
            vertices++;
        }
    }
    return result;
}

/* The object list types of the glow sprites (game/Flare/Glow/glow) and of the effect objects, for effect_sendEventToTypes. */
const s32 gQuadParticleTypes[4] = { 0x1C, 0x49, 0x56, 0x62 };
static const s32 gQuadSpriteTypes[3] = { 0x1B, 0x61, 0x4A };

void particle_broadcastToQuadParticles(s32 arg0, u8 arg1) {
    effect_sendEventToTypes(gQuadParticleTypes, 4, arg0, arg1);
}

void particle_broadcastToQuadSprites(s32 arg0, u8 arg1) {
    effect_sendEventToTypes(gQuadSpriteTypes, 3, arg0, arg1);
}

/* Quad centred on def.pos with half-axes axisU * width and axisV * height. */
Vtx *particle_buildQuadFromAxes(DefParticleHead *particle, Vec3f *axisU, Vec3f *axisV, s16 viewport) {
    Vtx *vertices;
    Vtx *result;
    f32 ux, uy, uz, vx, vy, vz;

    {
        u8 fresh;

        particle_getViewBuffer(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
        result = vertices;
        if (vertices != NULL) {
            if (fresh != 0) {
                memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
                memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
            }
        } else {
            return NULL;
        }
    }
    ux = axisU->x * particle->def.width;
    uy = axisU->y * particle->def.width;
    uz = axisU->z * particle->def.width;
    vx = axisV->x * particle->def.height;
    vy = axisV->y * particle->def.height;
    vz = axisV->z * particle->def.height;
    vertices[0].v.ob[0] = particle->def.pos.x + ux + vx;
    vertices[0].v.ob[1] = particle->def.pos.y + uy + vy;
    vertices[0].v.ob[2] = particle->def.pos.z + uz + vz;
    vertices[0].v.flag = 0;
    vertices[1].v.ob[0] = particle->def.pos.x - ux + vx;
    vertices[1].v.ob[1] = particle->def.pos.y - uy + vy;
    vertices[1].v.ob[2] = particle->def.pos.z - uz + vz;
    vertices[1].v.flag = 0;
    vertices[2].v.ob[0] = particle->def.pos.x - ux - vx;
    vertices[2].v.ob[1] = particle->def.pos.y - uy - vy;
    vertices[2].v.ob[2] = particle->def.pos.z - uz - vz;
    vertices[2].v.flag = 0;
    vertices[3].v.ob[0] = particle->def.pos.x + ux - vx;
    vertices[3].v.ob[1] = particle->def.pos.y + uy - vy;
    vertices[3].v.ob[2] = particle->def.pos.z + uz - vz;
    vertices[3].v.flag = 0;
    return result;
}

/* Sort key: texture, then animation frame, so that particles sharing a texture load draw together. */
#define PARTICLE_SORT_KEY(p) (((p)->def.texture << 8) + ((p)->def.frame >> 16))

/* List callback: insertion-sorts draw list gObjectLists[group][list] by PARTICLE_SORT_KEY. */
s32 particle_sortDrawList(s32 arg0, s32 list, s32 group, s16 phase) {
    DefParticleHead head;
    DefParticleHead **slot;
    DefParticleHead *cur;
    DefParticleHead *prev;
    DefParticleHead *next;
    DefParticleHead *node;
    s32 key;
    s32 nodeKey;

    if (phase != 0) {
        return arg0;
    }
    slot = (DefParticleHead **)&gObjectLists[group][list];
    node = *slot;
    head.def.texture = 0;
    head.def.frame = 0;
    head.header.next = &node->header;
    /* BUG (original game): dereferences the list head before the NULL check below
       (and again after the sort); an empty list writes to address 4. */
    node->header.prev = &head.header;
    if (node != NULL) {
        for (cur = (DefParticleHead *)node->header.next; cur != NULL; cur = next) {
            prev = (DefParticleHead *)cur->header.prev;
            next = (DefParticleHead *)cur->header.next;
            key = PARTICLE_SORT_KEY(cur);
            for (node = prev; node != NULL; node = (DefParticleHead *)node->header.prev) {
                nodeKey = PARTICLE_SORT_KEY(node);
                if (key < nodeKey) {
                    continue;
                }
                if (node != prev) {
                    prev->header.next = cur->header.next;
                    if (cur->header.next != NULL) {
                        cur->header.next->prev = cur->header.prev;
                    }
                    cur->header.next = node->header.next;
                    if (cur->header.next != NULL) {
                        cur->header.next->prev = &cur->header;
                    }
                    cur->header.prev = &node->header;
                    node->header.next = &cur->header;
                }
                break;
            }
        }
    }
    *slot = (DefParticleHead *)head.header.next;
    head.header.next->prev = NULL;
    return arg0;
}
