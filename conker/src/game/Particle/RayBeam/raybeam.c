#include <ultra64.h>
#include <math.h>
#include "conker/actor.h"
#include "conker/beam.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/flare.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "libc/string.h"

/* The data of the beam's startFx, at BeamStartFx.wobble (0xA8 in the sprite). */
typedef struct {
    /* 0x00 */ Wobble2D wobble;
    /* 0x28 */ RayBeamLink link;
} BeamStartFxData;

/* This unit's .rodata front-end data: the effect tables of particle_startObjRayBeam. Per effect kind: the two points
 * (vec1, vec2) given to particle_spawnRayBeam, and its bone. */
static const Vec3f gObjRayBeamStarts[7] = {
    { 0.0f, 36.0f, 76.0f },  { -6.0f, -25.0f, 119.0f }, { 0.0f, 66.0f, 74.0f },  { -6.0f, -25.0f, 119.0f },
    { 0.0f, -79.0f, 49.0f }, { 7.0f, -54.0f, 57.0f },   { 9.0f, -51.0f, 65.0f },
};
static const Vec3f gObjRayBeamTargets[7] = {
    { 0.0f, 36.0f, 130.0f },  { -6.0f, -25.0f, 208.0f }, { 0.0f, 66.0f, 274.0f }, { -6.0f, -25.0f, 208.0f },
    { 0.0f, -114.0f, 49.0f }, { 7.0f, -113.0f, 57.0f },  { 9.0f, -64.0f, 72.0f },
};
static const u8 gObjRayBeamBones[7] = { 12, 0, 12, 9, 9, 9, 9 };

/*
 * Spawns a beam on owner: a main particle carrying a RayBeamData plus five children (the hitFx quad, the endPoint
 * light, the line glow, and the startFx and endFx particle_newOwnedSpriteParticle sprites) that keep a pointer back to
 * it. lifetime -1 means endless. Returns the beam, or NULL.
 */
RayBeam *particle_spawnRayBeam(Actor *owner, const Vec3f *vec1, const Vec3f *vec2, u8 bone, s16 lifetime, u8 r,
                              u8 g, u8 b, f32 arg8, f32 scrollBase, f32 scrollRange, f32 argB, u8 aimed, u8 fxOwner, s32 fxList) {
    RayBeam *particle;
    ParticleDef def;
    RayBeamData data;
    u8 hasLifetime;
    RayBeamData *state;

    if (lifetime == -1) {
        hasLifetime = FALSE;
        lifetime = 300;
    } else {
        hasLifetime = TRUE;
    }
    data.ownerSlot = owner - movement_obj;
    if (owner->CameraPtr != NULL) {
        data.ownerCamera = owner->CameraPtr->CNumber;
    } else {
        data.ownerCamera = 0xFF;
    }
    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.bone = bone;
    data.flags = (s32)(aimed ? 2 : 0); // matching: the (s32) cast changes IDO's register choice
    data.localStart = *vec1;
    data.localTarget = *vec2;
    data.widthTarget = arg8;
    data.widthBlend = 0.1019999981f;
    data.scrollBase = scrollBase;
    data.scrollRange = scrollRange;
    data.scrollPhaseRate = argB;
    data.texOffset = -32768.0f;
    data.texVelocityRange = 250.0f;
    data.texVelocityTimerRange = 25.0f;
    data.width = 0.0f;
    data.scrollPhase = 0.0f;
    data.texVelocity = 0.0f;
    data.texVelocityTarget = 0.0f;
    data.texVelocityTimer = 0.0f;
    data.alphaTarget = 0.0f;
    data.alphaTimer = 0.0f;
    data.texVelocityBlend = 0.4499999881f;
    data.alpha = 255.0f;
    data.alphaBase = 128.0f;
    data.alphaRange = 127.0f;
    data.alphaTimerRange = 20.0f;
    data.alphaBlend = 0.400000006f;
    if (owner->CameraPtr != NULL) {
        data.sound = sound_play(0x4D0, 2000, 0x40, 0, 0);
    } else {
        data.sound = 0;
    }
    def.texture = 0x59;
    def.type = 1;
    def.renderModes = 0x3103;
    def.lifetime = lifetime;
    def.frame = 0;
    def.frameSpeed = 0;
    def.rgba[0] = r;
    def.rgba[1] = g;
    def.rgba[2] = b;
    def.rgba[3] = 0xFF;
    def.width = 100.0f;
    def.height = 100.0f;
    def.pos.x = 0.0f;
    def.pos.y = 0.0f;
    def.pos.z = 0.0f;
    def.rot.x = 0.0f;
    def.rot.y = 0.0f;
    def.rot.z = 0.0f;
    def.scale.x = 1.0f;
    def.scale.y = 1.0f;
    def.scale.z = 1.0f;
    def.flags = (hasLifetime ? 1 : 0) | 0x40000 | 0x80000 | 0x200000 | 0x400000 | 0x80000000;
    def.alpha = 0xFF;
    def.colorLevel = 0xFF;
    def.primMode = 0;
    def.envMode = 6;
    particle = particle_newQuadParticleNoModel(&def, &gWhiteVtxColors, 0x15, 0x1F, 0, 0x12, 0, sizeof(RayBeamData), fxOwner, fxList);
    if (particle != NULL) {
        state = &particle->data;
        memcpy(state, &data, sizeof(RayBeamData));
        {
            ParticleDef subDef;
            BeamHitFxData subData;

            subData.beam = particle;
            {
                LightEffectHeader flashHeader;
                Vec3i flashPos;
                RayBeam *flashData;
                ParticleDesc descA;
                BeamStartFxData particleAData;

                subData.flags = 0;
                subDef.texture = 3;
                subDef.type = 2;
                subDef.renderModes = 0x3201;
                subDef.lifetime = lifetime;
                subDef.frame = 0;
                subDef.frameSpeed = 0;
                subDef.rgba[0] = r;
                subDef.rgba[1] = g;
                subDef.rgba[2] = b;
                subDef.rgba[3] = 0xFF;
                subDef.height = 0.0f;
                subDef.width = 0.0f;
                subDef.pos.x = 0.0f;
                subDef.pos.y = 0.0f;
                subDef.pos.z = 0.0f;
                subDef.rot.x = 0.0f;
                subDef.rot.y = 0.0f;
                subDef.rot.z = 0.0f;
                subDef.scale.x = 1.0f;
                subDef.scale.y = 1.0f;
                subDef.scale.z = 1.0f;
                subDef.flags = (hasLifetime ? 1 : 0) | 0x40000 | 0x80000 | 0x400000;
                subDef.alpha = 0xFF;
                subDef.colorLevel = 0xFF;
                subDef.primMode = 0;
                subDef.envMode = 6;
                if ((state->hitFx = particle_newQuadParticleNoModel(&subDef, &gWhiteVtxColors, 0, 0, 0, 0x13, 0, sizeof(BeamHitFxData),
                                                   fxOwner, fxList)) != NULL) {
                    memcpy(&state->hitFx->data, &subData, sizeof(BeamHitFxData));
                }
                flashPos.x = 0;
                flashPos.y = 0;
                flashPos.z = 0;
                flashData = particle;
                flashHeader.flags = (hasLifetime ? 1 : 0) | 2;
                flashHeader.update = -1;
                flashHeader.lifetime = lifetime;
                flashHeader.lightType = 7;
                if ((state->endPoint = light_createWithEffect(&flashHeader, &flashPos, 5, r, g, b, 0xFF, 0, sizeof(flashData),
                                                  fxOwner, fxList)) != NULL) {
                    memcpy(&state->endPoint->data.object, &flashData, sizeof(flashData));
                }
                particleAData.link.flags = 0;
                particleAData.wobble.offset[0] = 0.0f;
                particleAData.wobble.offset[1] = 0.0f;
                particleAData.wobble.base[1] = 10.25000095f;
                particleAData.wobble.base[0] = 10.25000095f;
                particleAData.wobble.amplitude[1] = 2.750000238f;
                particleAData.wobble.amplitude[0] = 2.750000238f;
                particleAData.link.beam = particle;
                particleAData.wobble.phase[0] = math_randomFloat() * 6.283185482f; /* random, 0 to 2pi */
                particleAData.wobble.phase[1] = math_randomFloat() * 6.283185482f;
                // matching: '+ -0.3f' (add.s), not '- 0.3f' (sub.s)
                particleAData.wobble.speed[0] = (math_randomFloat() * 0.5999999642f) + -0.2999999821f;
                particleAData.wobble.speed[1] = (math_randomFloat() * 0.5999999642f) + -0.2999999821f;
                descA.texture = 0x2B;
                descA.renderMode = 0x3303;
                descA.setGeometryMode = 0x200005;
                descA.clearGeometryMode = 0;
                descA.timer = lifetime;
                descA.frame = 0;
                descA.frameRate = 0;
                descA.r = 0xFF;
                descA.g = 0xFF;
                descA.b = 0xFF;
                descA.a = 0xFF;
                descA.r2 = r;
                descA.g2 = g;
                descA.b2 = b;
                descA.alpha = 0xFF;
                descA.brightness = 0xFF;
                descA.height = 0.0f;
                descA.width = 0.0f;
                descA.pos.x = 0.0f;
                descA.pos.y = 0.0f;
                descA.pos.z = 0.0f;
                descA.offset.x = 0.0f;
                descA.offset.y = 0.0f;
                descA.offset.z = 0.0f;
                descA.vel.x = 0.0f;
                descA.vel.y = 0.0f;
                descA.vel.z = 0.0f;
                descA.gravity = 0.0f;
                descA.fadeStart = 1;
                descA.fadeRate = 0xFF;
                descA.scaleStart = 1;
                descA.scaleRate = 1.0f;
                descA.flags = ((math_random() & 1) ? PARTICLE_FLIP_S : 0) |
                             ((hasLifetime ? PARTICLE_LIFETIME : 0) | ((math_random() & 1) ? PARTICLE_FLIP_T : 0)) |
                             PARTICLE_HANDLERS | PARTICLE_DRAW_FUNC;
                descA.primColorMode = 6;
                descA.envColorMode = 6;
                descA.updateFunc = 11;
                descA.deathFunc = -1;
                descA.drawFunc = 0;
                descA.handlerSet = 1;
                if ((state->startFx = particle_newOwnedSpriteParticle(&descA, 1, sizeof(BeamStartFxData), fxOwner, fxList)) != NULL) {
                    memcpy(&state->startFx->wobble, &particleAData, sizeof(BeamStartFxData));
                }
                {
                    GlowShape glowParams;
                    ParticleDef glowDef;
                    RayBeamLink glowData;

                    glowData.beam = particle;
                    glowData.flags = 0;
                    glowDef.texture = 3;
                    glowDef.type = 3;
                    glowDef.renderModes = 0x3403;
                    glowDef.lifetime = lifetime;
                    glowDef.frame = 0;
                    glowDef.frameSpeed = 0;
                    glowDef.rgba[0] = r;
                    glowDef.rgba[1] = g;
                    glowDef.rgba[2] = b;
                    glowDef.rgba[3] = 0xFF;
                    glowDef.width = 100.0f;
                    glowDef.height = 100.0f;
                    glowDef.pos.x = 0.0f;
                    glowDef.pos.y = 0.0f;
                    glowDef.pos.z = 0.0f;
                    glowDef.rot.x = 0.0f;
                    glowDef.rot.y = 0.0f;
                    glowDef.rot.z = 500.0f;
                    glowDef.scale.x = 1.0f;
                    glowDef.scale.y = 1.0f;
                    glowDef.scale.z = 1.0f;
                    glowDef.flags = (hasLifetime ? 1 : 0) | 0x2 | 0x10 | 0x2000 | 0x20000 | 0x40000 | 0x80000 |
                                    0x400000;
                    glowDef.colorLevel = 0xFF;
                    glowDef.alpha = 0xFF;
                    glowDef.primMode = 0;
                    glowDef.envMode = 6;
                    glowParams.height0 = 0.0f;
                    glowParams.width0 = 0.0f;
                    glowParams.alpha0 = 0.0f;
                    glowParams.visibility[0] = 0;
                    glowParams.visibility[1] = 0;
                    glowParams.visibility[2] = 0;
                    glowParams.visibility[3] = 0;
                    glowParams.scale = 1.0f;
                    glowParams.type = 1;
                    glowParams.height1 = 60.00000381f;
                    glowParams.width1 = 60.00000381f;
                    glowParams.offset1 = 0.5f;
                    glowParams.offset0 = 0.5f;
                    glowParams.alpha1 = 0.3000000119f;
                    glowParams.fadeRate = 9.999999975e-07f;
                    if ((state->line = flare_spawnGlow(&glowParams, 0x68, &glowDef, 0, 0, 0, 0, 0, fxOwner, fxList)) != NULL) {
                        memcpy(&state->line->link, &glowData, sizeof(RayBeamLink));
                    }
                    {
                        ParticleDesc descB;
                        RayBeamLink particleBData;

                        particleBData.flags = 0;
                        particleBData.beam = particle;
                        descB.texture = 0x15;
                        descB.renderMode = 0x3203;
                        descB.setGeometryMode = 0x200005;
                        descB.clearGeometryMode = 0;
                        descB.timer = lifetime;
                        descB.frame = 0;
                        descB.frameRate = 0;
                        descB.r = 0xFF;
                        descB.g = 0xFF;
                        descB.b = 0xFF;
                        descB.a = 0xFF;
                        descB.r2 = r;
                        descB.g2 = g;
                        descB.b2 = b;
                        descB.alpha = 0xFF;
                        descB.brightness = 0xFF;
                        descB.height = 125.0f;
                        descB.width = 125.0f;
                        descB.pos.x = 0.0f;
                        descB.pos.y = 0.0f;
                        descB.pos.z = 0.0f;
                        descB.offset.x = 0.0f;
                        descB.offset.y = 0.0f;
                        descB.offset.z = 0.0f;
                        descB.vel.x = 0.0f;
                        descB.vel.y = 0.0f;
                        descB.vel.z = 0.0f;
                        descB.gravity = 0.0f;
                        descB.fadeStart = 1;
                        descB.fadeRate = 0xFF;
                        descB.scaleStart = 1;
                        descB.scaleRate = 1.0f;
                        descB.flags = ((math_random() & 1) ? PARTICLE_FLIP_S : 0) |
                                      ((hasLifetime ? PARTICLE_LIFETIME : 0) |
                                       ((math_random() & 1) ? PARTICLE_FLIP_T : 0)) |
                                      PARTICLE_HANDLERS | PARTICLE_DRAW_FUNC;
                        descB.primColorMode = 6;
                        descB.envColorMode = 6;
                        descB.updateFunc = -1;
                        descB.deathFunc = -1;
                        descB.drawFunc = 1;
                        descB.handlerSet = 5;
                        if ((state->endFx = particle_newOwnedSpriteParticle(&descB, 1, sizeof(RayBeamLink), fxOwner, fxList)) != NULL) {
                            memcpy(&state->endFx->link, &particleBData, sizeof(RayBeamLink));
                        }
                    }
                }
            }
        }
    }
    return particle;
}

/* Releases the beam's child objects, then destroys it. */
void particle_destroyRayBeam(RayBeam *beam) {
    RayBeamData *data = &beam->data;

    // matching: tested as u16 but passed as s16, so IDO reuses the lhu without an andi
    if (*(u16 *)&data->sound) {
        sound_stop(data->sound);
    }
    if (data->hitFx != NULL) {
        effect_destroyObject(&data->hitFx->head.header);
    }
    if (data->endPoint != NULL) {
        effect_destroyObject(data->endPoint);
    }
    if (data->startFx != NULL) {
        effect_destroyObject(&data->startFx->head.header);
    }
    if (data->line != NULL) {
        effect_destroyObject(&data->line->head.header);
    }
    if (data->endFx != NULL) {
        effect_destroyObject(&data->endFx->head.header);
    }
    particle_destroyQuadParticleDefault(&beam->head);
}

/* Counterpart of particle_destroyRayBeam for the beam's child objects and particle_freeQuadParticleDefault. */
void particle_freeRayBeam(RayBeam *beam) {
    RayBeamData *data = &beam->data;

    if (data->hitFx != NULL) {
        effect_freeObject(&data->hitFx->head.header);
    }
    if (data->endPoint != NULL) {
        effect_freeObject(data->endPoint);
    }
    if (data->startFx != NULL) {
        effect_freeObject(&data->startFx->head.header);
    }
    if (data->line != NULL) {
        effect_freeObject(&data->line->head.header);
    }
    if (data->endFx != NULL) {
        effect_freeObject(&data->endFx->head.header);
    }
    particle_freeQuadParticleDefault(&beam->head);
}

/* The functions below detach a child object from its beam before handing it on. */

void particle_destroyBeamHitFx(BeamHitFx *fx) {
    RayBeamData *data = &fx->data.beam->data;

    data->hitFx = NULL;
    particle_destroyQuadParticleDefault(&fx->head);
}

void particle_freeBeamHitFx(BeamHitFx *fx) {
    RayBeamData *data = &fx->data.beam->data;

    data->hitFx = NULL;
    particle_freeQuadParticleDefault(&fx->head);
}

void particle_destroyBeamEndPoint(LightEffect *endPoint) {
    RayBeamData *data = &((RayBeam *)endPoint->data.object)->data;

    data->endPoint = NULL;
    light_defaultDestroy(endPoint);
}

void particle_freeBeamEndPoint(LightEffect *endPoint) {
    RayBeamData *data = &((RayBeam *)endPoint->data.object)->data;

    data->endPoint = NULL;
    light_defaultFree(endPoint);
}

void particle_destroyBeamStartFx(BeamStartFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->startFx = NULL;
    particle_destroySpriteDefault(&fx->head);
}

void particle_freeBeamStartFx(BeamStartFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->startFx = NULL;
    particle_freeSpriteDefault(&fx->head);
}

void particle_destroyBeamLine(BeamLine *line) {
    RayBeamData *data = &line->link.beam->data;

    data->line = NULL;
    flare_destroyGlowDefault(&line->head);
}

void particle_freeBeamLine(BeamLine *line) {
    RayBeamData *data = &line->link.beam->data;

    data->line = NULL;
    flare_freeGlowDefault(&line->head);
}

void particle_destroyBeamEndFx(BeamEndFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->endFx = NULL;
    particle_destroySpriteDefault(&fx->head);
}

void particle_freeBeamEndFx(BeamEndFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->endFx = NULL;
    particle_freeSpriteDefault(&fx->head);
}

/*
 * Places the beam: start at the owner's bone, end at the owner's aim target or at the first
 * collision along the beam, then moves or hides the child objects to match.
 * Returns 0 when the owner is gone.
 */
s8 particle_updateRayBeam(RayBeam *beam) {
    s8 result;
    ActorCtl *aim;
    Actor *owner;
    u8 hit;
    CollisionTri hitInfo;
    Vec3f dir;
    u8 endIsHit; // matching: always tested as endIsHit != 0; a bare test changes the frame size
    f32 (*boneMtx)[4];
    Vec3f target;
    s32 unkOut;
    s32 hitType;
    s32 pad; // matching: stack slot needed for the frame size
    RayBeamData *data;
    MtxF *bones;

    result = 1;
    owner = beam->data.owner;
    data = &beam->data;
    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        result = 0;
    } else {
        endIsHit = 1;
        bones = owner->ob_matrix_ptr;
        if (bones != NULL && (gCutsceneStates[0] != 1 || (owner->cullMask & 0xF) != 0xF)) {
            boneMtx = bones[data->bone];
            math_transformOffset(&data->localStart, &beam->head.def.pos, boneMtx);
            if (data->flags & 2) {
                aim = owner->mv;
                hitType = aim->rayHit.hitType;
                if (hitType != 0) {
                    beam->head.def.rot = owner->mv->rayHit.pos;
                    data->length = owner->mv->rayHit.dist;
                    hitInfo = owner->mv->rayHit.tri;
                    dir.x = beam->head.def.rot.x - beam->head.def.pos.x;
                    dir.y = beam->head.def.rot.y - beam->head.def.pos.y;
                    dir.z = beam->head.def.rot.z - beam->head.def.pos.z;
                } else {
                    endIsHit = 0;
                    hitType = 2;
                    data->length = 2000.0f;
                    dir.x = owner->mv->rayHit.dir.x * 2000.0f;
                    dir.y = owner->mv->rayHit.dir.y * 2000.0f;
                    dir.z = owner->mv->rayHit.dir.z * 2000.0f;
                    beam->head.def.rot.x = beam->head.def.pos.x + dir.x;
                    beam->head.def.rot.y = beam->head.def.pos.y + dir.y;
                    beam->head.def.rot.z = beam->head.def.pos.z + dir.z;
                }
            } else {
                math_transformOffset(&data->localTarget, &target, boneMtx);
                target.x = target.x - beam->head.def.pos.x;
                target.y = target.y - beam->head.def.pos.y;
                target.z = target.z - beam->head.def.pos.z;
                if (collision_castRayWorld(beam->head.def.pos.x, beam->head.def.pos.y, beam->head.def.pos.z, target.x, target.y,
                                  target.z, &unkOut, &hitInfo, &beam->head.def.rot.x, &beam->head.def.rot.y, &beam->head.def.rot.z, 0,
                                  0, 0, 0.0f) != hit * 0) { // matching: reading hit here makes IDO keep it in v1
                    dir.x = beam->head.def.rot.x - beam->head.def.pos.x;
                    dir.y = beam->head.def.rot.y - beam->head.def.pos.y;
                    dir.z = beam->head.def.rot.z - beam->head.def.pos.z;
                    hit = 1;
                    data->length = math_vec3Length(&dir);
                } else {
                    hit = 0;
                }
                hitType = hit;
            lbl1:; // matching: unused labels split the basic blocks the way IDO needs
            }
        } else {
            hitType = 0;
        }
        if (hitType != 0) {
            BeamHitFx *hitFx;
            BeamHitFxData *hitFxData;
            RayBeamLink *endFxLink;
            BeamEndFx *endFx;
            LightEffect *endPoint;
            BeamStartFx *startFx;
            BeamLine *line;

            data->flags |= 1;
            if (data->hitFx != NULL) {
                hitFx = data->hitFx;
                hitFxData = &hitFx->data;
                if (hitType == 1 && endIsHit != 0) {
                    if (data->length > 40.0f) {
                        f32 invLength = 1.0f / data->length;

                        hitFx->head.def.pos.x = beam->head.def.rot.x - dir.x * invLength * 20.0f;
                        hitFx->head.def.pos.y = beam->head.def.rot.y - dir.y * invLength * 20.0f;
                        hitFx->head.def.pos.z = beam->head.def.rot.z - dir.z * invLength * 20.0f;
                    } else {
                        hitFx->head.def.pos = beam->head.def.rot;
                    }
                    hitFxData->hit = hitInfo;
                    hitFxData->flags |= 1;
                } else {
                    hitFxData->flags &= ~1;
                }
            }
            if (data->endFx != NULL) {
                endFx = data->endFx;
                endFxLink = &endFx->link;
                if ((hitType == 2 || hitType == 3) && endIsHit != 0) {
                    endFx->head.desc.pos = beam->head.def.rot;
                    endFxLink->flags |= 1;
                } else {
                    endFxLink->flags &= ~1;
                }
            }
            if (data->endPoint != NULL) {
                endPoint = data->endPoint;
                do {} while (0); // matching: adds the uopt temp that puts the t0 spill at 0x5C
                endPoint->light->x = beam->head.def.rot.x;
                endPoint->light->y = beam->head.def.rot.y;
                endPoint->light->z = beam->head.def.rot.z;
                if (endIsHit != 0) {
                    endPoint->light->disabled = 0;
                } else {
                    endPoint->light->disabled = 1;
                }
            }
            if (data->startFx != NULL) {
                startFx = data->startFx;
                startFx->head.desc.pos = beam->head.def.pos;
                startFx->head.desc.width = startFx->wobble.offset[0] * data->width;
                startFx->head.desc.height = startFx->wobble.offset[1] * data->width;
                startFx->link.flags |= 1;
            }
            if (data->line != NULL) {
                line = data->line;
                line->head.def.pos = beam->head.def.pos;
                line->head.def.rot = beam->head.def.rot;
                line->head.def.flags &= ~4;
                line->link.flags |= 1;
            }
        } else {
            BeamHitFx *hitFx;
            BeamHitFxData *hitFxData;
            RayBeamLink *endFxLink;
            BeamEndFx *endFx;
            LightEffect *endPoint;
            BeamStartFx *startFx;
            BeamLine *line;

        lbl2:;
            data->flags &= ~1;
            if (data->hitFx != NULL) {
                hitFx = data->hitFx;
                hitFx->data.flags &= ~1;
            }
            if (data->endPoint != NULL) {
                endPoint = data->endPoint;
                endPoint->light->disabled = 1;
            }
            if (data->startFx != NULL) {
                startFx = data->startFx;
                startFx->link.flags &= ~1;
            }
            if (data->line != NULL) {
                line = data->line;
                line->link.flags &= ~1;
            }
            if (data->endFx != NULL) {
                endFx = data->endFx;
                endFx->link.flags &= ~1;
            }
        }
    }
    return result;
}

/*
 * Builds this frame's quad for the beam as seen from viewport `viewport`: four vertices from start to
 * end, offset sideways by the beam's width (facing the eye point), with a scrolling texture.
 * Returns the vertices, or NULL when the beam is not drawn.
 */
Vtx *particle_buildRayBeamQuad(RayBeam *beam, s16 viewport) {
    RayBeamData *data = &beam->data;
    Vtx *vtx;
    Vtx *result;
    Vec3f *eye;
    Vec3f dir;
    Vec3f closest;
    f32 dx, dy, dz;
    f32 sideZ, sideY, sideX;
    f32 sideLenSq;
    f32 scale;
    f32 scrollSpeed;
    Vec3f start;
    Vec3f end;
    f32 drawLen;
    u8 isNew;
    s32 texStart, texEnd, texSpan;
    f32 maxLen;

    if (viewport == data->ownerCamera && (data->owner == NULL || data->owner->mv->SniperState != 0)) {
        return NULL;
    }
    if (data->flags & 1) {
        particle_getViewBuffer(beam->head.vtxBuffers, viewport, sizeof(beam->head.vtxTemplate), &vtx, &isNew);
        result = vtx;
        if (vtx != NULL) {
            if (isNew) {
                memcpy(beam->head.vtxBuffers[viewport], beam->head.vtxTemplate, sizeof(beam->head.vtxTemplate));
                memcpy(beam->head.vtxBuffers[viewport] + 4, beam->head.vtxTemplate, sizeof(beam->head.vtxTemplate));
            }
        } else {
            return NULL;
        }
        eye = math_getCameraPos(viewport);
        maxLen = 3508.0f;
        start = beam->head.def.pos;
        scale = data->length;
        if (maxLen < scale) {
            f32 ratio = maxLen / scale;

            dir.x = (beam->head.def.rot.x - start.x) * ratio;
            dir.y = (beam->head.def.rot.y - start.y) * ratio;
            dir.z = (beam->head.def.rot.z - start.z) * ratio;
            end.x = start.x + dir.x;
            end.y = start.y + dir.y;
            end.z = start.z + dir.z;
            drawLen = maxLen;
        } else {
            end = beam->head.def.rot;
            dir.x = end.x - start.x;
            dir.y = end.y - start.y;
            dir.z = end.z - start.z;
            drawLen = data->length;
        }
        math_closestPointOnSegment(&start, &dir, eye, &closest, 0);
        dx = closest.x - eye->x;
        dy = closest.y - eye->y;
        dz = closest.z - eye->z;
        sideX = dir.y * dz - dy * dir.z;
        sideY = dir.z * dx - dz * dir.x;
        sideZ = dir.x * dy - dx * dir.y;
        sideLenSq = sideX * sideX + sideY * sideY + sideZ * sideZ;
        if (sideLenSq == 0.0f) {
            sideZ = sideY = sideX = 0.0f;
        } else {
            scale = data->width / sqrtf(sideLenSq);
            sideX *= scale;
            sideY *= scale;
            sideZ *= scale;
        }
        scrollSpeed = sinf(data->scrollPhase);
        scrollSpeed = data->scrollBase + data->scrollRange * scrollSpeed;
        texStart = data->texOffset;
        texEnd = data->texOffset + drawLen * scrollSpeed;
        texSpan = texEnd - texStart;
        {
            s32 wrapped;

            // keep the texture coordinates within s16 range
            if (texEnd >= 0x8000 || texStart < -0x8000) {
                wrapped = texStart;
                while (wrapped > 0x2000) {
                    wrapped -= 0x2000;
                }
                while (wrapped < 0) {
                    wrapped += 0x2000;
                }
                texStart = wrapped - 0x8000;
                texEnd = texStart + texSpan;
            }
        }
        vtx->v.ob[0] = end.x + sideX;
        vtx->v.ob[1] = end.y + sideY;
        vtx->v.ob[2] = end.z + sideZ;
        vtx->v.tc[1] = 0;
        vtx->v.tc[0] = texEnd;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = end.x - sideX;
        vtx->v.ob[1] = end.y - sideY;
        vtx->v.ob[2] = end.z - sideZ;
        vtx->v.tc[1] = 0x7C0;
        vtx->v.tc[0] = texEnd;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = start.x - sideX;
        vtx->v.ob[1] = start.y - sideY;
        vtx->v.ob[2] = start.z - sideZ;
        vtx->v.tc[1] = 0x7C0;
        vtx->v.tc[0] = texStart;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = start.x + sideX;
        vtx->v.ob[1] = start.y + sideY;
        vtx->v.ob[2] = start.z + sideZ;
        vtx->v.tc[1] = 0;
        vtx->v.tc[0] = texStart;
        vtx->v.flag = 0;
        return result;
    }
end: // matching: an unused label here keeps IDO's block layout
    return NULL;
}

/* Vertex function 0x13 of the beam's hit sprite (BeamHitFx): fills the viewport's vertex buffer with a quad centred on
 * pos, in the plane of the hit triangle, half-axes axisU * width and axisV * height; none while data.flags bit 0 is clear. */
Vtx *particle_buildCentredAxisQuad(BeamHitFx *particle, s16 viewport) {
    s32 pad; // matching: unused stack slot in the original frame
    Vtx *vtx;
    Vtx *ret;
    Vec3f axisU;
    Vec3f axisV;
    u8 isNew;

    particle_getViewBuffer(particle->head.vtxBuffers, viewport, sizeof(particle->head.vtxTemplate), &vtx, &isNew);
    ret = vtx; // matching: the result is returned from its own stack copy, not by reloading vtx
    // matching: an early `if (vtx == NULL) return NULL;` lays out the branches differently
    if (vtx != NULL) {
        if (isNew) {
            memcpy(particle->head.vtxBuffers[viewport], particle->head.vtxTemplate, sizeof(particle->head.vtxTemplate));
            memcpy(particle->head.vtxBuffers[viewport] + 4, particle->head.vtxTemplate, sizeof(particle->head.vtxTemplate));
        }
    } else {
        return NULL;
    }

    if (!(particle->data.flags & 1)) {
        return NULL;
    }

    if (math_getTriEdges(&particle->data.hit, &axisU, &axisV, NULL) == 0) {
        return NULL;
    }

    math_vec3Normalize(&axisU, &axisU, NULL, NULL);
    math_vec3Normalize(&axisV, &axisV, NULL, NULL);

    axisU.x *= particle->head.def.width;
    axisU.y *= particle->head.def.width;
    axisU.z *= particle->head.def.width;
    axisV.x *= particle->head.def.height;
    axisV.y *= particle->head.def.height;
    axisV.z *= particle->head.def.height;

    vtx[0].v.ob[0] = (particle->head.def.pos.x - axisU.x) + axisV.x;
    vtx[0].v.ob[1] = (particle->head.def.pos.y - axisU.y) + axisV.y;
    vtx[0].v.ob[2] = (particle->head.def.pos.z - axisU.z) + axisV.z;
    vtx[0].v.flag = 0;
    vtx[1].v.ob[0] = (particle->head.def.pos.x + axisU.x) + axisV.x;
    vtx[1].v.ob[1] = (particle->head.def.pos.y + axisU.y) + axisV.y;
    vtx[1].v.ob[2] = (particle->head.def.pos.z + axisU.z) + axisV.z;
    vtx[1].v.flag = 0;
    vtx[2].v.ob[0] = (particle->head.def.pos.x + axisU.x) - axisV.x;
    vtx[2].v.ob[1] = (particle->head.def.pos.y + axisU.y) - axisV.y;
    vtx[2].v.ob[2] = (particle->head.def.pos.z + axisU.z) - axisV.z;
    vtx[2].v.flag = 0;
    vtx[3].v.ob[0] = (particle->head.def.pos.x - axisU.x) - axisV.x;
    vtx[3].v.ob[1] = (particle->head.def.pos.y - axisU.y) - axisV.y;
    vtx[3].v.ob[2] = (particle->head.def.pos.z - axisU.z) - axisV.z;
    vtx[3].v.flag = 0;

    return ret;
}

/* Per-frame update (gQuadParticleUpdate2Funcs): advances the scroll phase and the texture offset, and eases the width,
 * the texture velocity and the alpha toward random targets, then passes them on to the child objects. */
s32 particle_flickerRayBeam(RayBeam *beam) {
    RayBeamData *state;
    BeamHitFx *quad;
    LightEffect *particles;
    BeamEndFx *child;

    state = &beam->data;
    state->scrollPhase += state->scrollPhaseRate * gFrameDelta;
    state->scrollPhase = math_wrapRadians(state->scrollPhase);
    state->texOffset += state->texVelocity * gFrameDelta;
    while (state->texOffset > 32767.0f) {
        state->texOffset -= 65536.0f;
    }
    while (state->texOffset < -32768.0f) {
        state->texOffset += 65536.0f;
    }
    state->texVelocityTimer -= gFrameDelta;
    if (state->texVelocityTimer < 0.0f) {
        state->texVelocityTimer = math_randomFloat() * state->texVelocityTimerRange;
        state->texVelocityTarget = math_randomFloat() * (state->texVelocityRange + state->texVelocityRange) - state->texVelocityRange;
    }
    state->texVelocity += (state->texVelocityTarget - state->texVelocity) * state->texVelocityBlend;
    state->width += (state->widthTarget - state->width) * state->widthBlend;
    if (state->hitFx != NULL) {
        quad = state->hitFx;
        quad->head.def.width = quad->head.def.height = (math_randomFloat() * 0.2f + 0.4f) * state->width;
    }
    state->alphaTimer -= gFrameDelta;
    if (state->alphaTimer < 0.0f) {
        state->alphaTimer = math_randomFloat() * state->alphaTimerRange;
        state->alphaTarget = math_randomFloat() * state->alphaRange + state->alphaBase;
    }
    state->alpha += (state->alphaTarget - state->alpha) * state->alphaBlend;
    beam->head.def.alpha = state->alpha;
    if (state->hitFx != NULL) {
        state->hitFx->head.def.alpha = beam->head.def.alpha;
    }
    if (state->startFx != NULL) {
        state->startFx->head.desc.alpha = beam->head.def.alpha;
    }
    if (state->endPoint != NULL) {
        particles = state->endPoint;
        particles->light->radius = (math_random() & 3) + 2;
    }
    if (state->endFx != NULL) {
        child = state->endFx;
        child->head.desc.width = child->head.desc.height = math_randomFloat() * 10.0f + 120.0f;
    }
    return 1;
}

/* Sprite update (gSpriteUpdateFuncs[11]) of the beam's start sprite: advances its wobble offset. */
s32 particle_updateWobbleOffset(BeamStartFx *obj, Vec3f *prevPos) {
    Wobble2D *wobble = &obj->wobble;
    s32 pad; // matching: unused stack slot in the original frame
    f32 sinX;
    f32 sinY;

    sinX = sinf(wobble->phase[0]);
    sinY = sinf(wobble->phase[1]);
    wobble->offset[0] = wobble->amplitude[0] * sinX + wobble->base[0];
    wobble->offset[1] = wobble->amplitude[1] * sinY + wobble->base[1];
    wobble->phase[0] += wobble->speed[0] * gFrameDelta;
    wobble->phase[1] += wobble->speed[1] * gFrameDelta;
    wobble->phase[0] = math_wrapRadians(wobble->phase[0]);
    wobble->phase[1] = math_wrapRadians(wobble->phase[1]);
    return 1;
}

/* Starts effect `kind` on obj, using the effect tables for its parameters; picks one of two styles by object id. */
RayBeam *particle_startObjRayBeam(Actor *obj, s16 lifetime, u8 kind, u8 aimed, u8 owner, s32 list) {
    s32 r;
    s32 g;

    if (obj == NULL) {
        return NULL;
    }
    switch (obj->character_type) {
        case 0x58:
        case 0x5B:
        case 0x80:
            r = 0x32;
            g = 0xFF;
            break;
        case 0x5A:
        case 0x5F:
        case 0x74:
        case 0x75:
        case 0x7A:
            r = 0xFF;
            g = 0;
            break;
        default:
            if (obj->mv->rt_context_b == 0x3B) {
                r = 0xFF;
                g = 0;
            } else {
                r = 0x32;
                g = 0xFF;
            }
            break;
    }
    // matching: the int 20 is converted separately; 20.0f would share one register with the other 20.0f
    return particle_spawnRayBeam(obj, &gObjRayBeamStarts[kind], &gObjRayBeamTargets[kind], gObjRayBeamBones[kind], lifetime, r, g, 0,
                         20.0f, 20, 4.0f, 0.08f, aimed, owner, list);
}

/* Sends message 0x1A for `actor` (stops the effect started by particle_startObjRayBeam). */
void particle_stopObjEffect(Actor *actor) {
    ActorRef msg;

    msg.actor = actor;
    msg.uniqueId = actor->unique_id;
    particle_broadcastToQuadParticles((s32)&msg, 0x1A);
}


/* Draw test of the beam's startFx: 0 in the owner's camera view (ownerCamera) when the owner is NULL or aims in first
 * person (SniperState), else bit 0 of link.flags. */
s32 particle_isBeamStartFxVisible(BeamStartFx *startFx, s16 view) {
    RayBeamData *data = &startFx->link.beam->data;
    Actor *owner;

    if (view == data->ownerCamera) {
        owner = data->owner;
        if (owner == NULL || owner->mv->SniperState != 0) {
            return 0;
        }
    }
    if (!(startFx->link.flags & 1)) {
        return 0;
    }
    return 1;
}

/* Returns bit 0 of endFx->link.flags. */
s32 particle_isBeamEndFxOn(BeamEndFx *endFx, s32 view) {
    if (!(endFx->link.flags & 1)) {
        return 0;
    }
    return 1;
}

/* Same test as particle_isBeamStartFxVisible, for the beam's line (gGlowVisibilityTests). */
s32 particle_isBeamLineVisible(BeamLine *line, s16 view) {
    RayBeamData *data = &line->link.beam->data;
    Actor *owner;

    if (view == data->ownerCamera) {
        owner = data->owner;
        if (owner == NULL || owner->mv->SniperState != 0) {
            return 0;
        }
    }
    if (!(line->link.flags & 1)) {
        return 0;
    }
    return 1;
}
