#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "libc/string.h"

/* Data of the child subParticle (after its 0x110-byte particle). */
typedef struct {
    u8 unk0;
    u8 pad1[0x13];
    EffectParticle *parent;
} SubParticleData;

/* Data of the child particleA (at 0xA8 in the particle). */
typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 angle[2];               /* random, 0 to 2pi */
    f32 unk20;
    f32 unk24;
    EffectParticle *parent;
    u8 unk2C;
    u8 pad2D[3];
} ParticleAData;

/* Data of the child glow (at 0x170, GlowParams.anim) and particleB (at 0xA8). */
typedef struct {
    EffectParticle *parent;
    u8 unk4;
    u8 pad5[3];
} ChildData;

/* This unit's .rodata front-end data: the effect tables of func_151026BC. Per effect kind: the two points
 * (vec1, vec2) given to func_15100600, and its arg3. */
static const Vec3f D_800A21A0[7] = {
    { 0.0f, 36.0f, 76.0f },  { -6.0f, -25.0f, 119.0f }, { 0.0f, 66.0f, 74.0f },  { -6.0f, -25.0f, 119.0f },
    { 0.0f, -79.0f, 49.0f }, { 7.0f, -54.0f, 57.0f },   { 9.0f, -51.0f, 65.0f },
};
static const Vec3f D_800A21F4[7] = {
    { 0.0f, 36.0f, 130.0f },  { -6.0f, -25.0f, 208.0f }, { 0.0f, 66.0f, 274.0f }, { -6.0f, -25.0f, 208.0f },
    { 0.0f, -114.0f, 49.0f }, { 7.0f, -113.0f, 57.0f },  { 9.0f, -64.0f, 72.0f },
};
static const u8 D_800A2248[7] = { 12, 0, 12, 9, 9, 9, 9 };

/*
 * Spawns an effect on owner: a main particle carrying an EffectParticleState plus five children (a particle,
 * a flash, a glow and two func_15130374 particles) that keep a pointer back to it. lifetime -1 means
 * endless. Returns the main particle, or NULL.
 */
EffectParticle *func_15100600(Actor *owner, const Vec3f *vec1, const Vec3f *vec2, u8 arg3, s16 lifetime, u8 r,
                              u8 g, u8 b, f32 arg8, f32 arg9, f32 argA, f32 argB, u8 argC, u8 argD, s32 argE) {
    EffectParticle *particle;
    ParticleDef def;
    EffectParticleState data;
    u8 hasLifetime;
    EffectParticleState *state;

    if (lifetime == -1) {
        hasLifetime = FALSE;
        lifetime = 300;
    } else {
        hasLifetime = TRUE;
    }
    data.ownerSlot = owner - movement_obj;
    if (owner->CameraPtr != NULL) {
        data.unk22 = owner->CameraPtr->CNumber;
    } else {
        data.unk22 = 0xFF;
    }
    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.unk20 = arg3;
    data.unk23 = (s32)(argC ? 2 : 0); // matching: the (s32) cast changes IDO's register choice
    data.unk8 = *vec1;
    data.unk14 = *vec2;
    data.unk3C = arg8;
    data.unk40 = 0.1019999981f;
    data.unk48 = arg9;
    data.unk4C = argA;
    data.unk54 = argB;
    data.unk58 = -32768.0f;
    data.unk64 = 250.0f;
    data.unk6C = 25.0f;
    data.unk38 = 0.0f;
    data.unk50 = 0.0f;
    data.unk5C = 0.0f;
    data.unk60 = 0.0f;
    data.unk68 = 0.0f;
    data.unk78 = 0.0f;
    data.unk84 = 0.0f;
    data.unk70 = 0.4499999881f;
    data.unk74 = 255.0f;
    data.unk7C = 128.0f;
    data.unk80 = 127.0f;
    data.unk88 = 20.0f;
    data.unk8C = 0.400000006f;
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
    def.unk44 = 0xFF;
    def.unk45 = 0xFF;
    def.unk46 = 0;
    def.unk47 = 6;
    particle = func_1513D4B8(&def, &D_800A4AA0, 0x15, 0x1F, 0, 0x12, 0, sizeof(EffectParticleState), argD, argE);
    if (particle != NULL) {
        state = &particle->state;
        memcpy(state, &data, sizeof(EffectParticleState));
        {
            ParticleDef subDef;
            SubParticleData subData;

            subData.parent = particle;
            {
                LightEffectHeader flashHeader;
                Vec3i flashPos;
                EffectParticle *flashData;
                ParticleDesc descA;
                ParticleAData particleAData;

                subData.unk0 = 0;
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
                subDef.unk44 = 0xFF;
                subDef.unk45 = 0xFF;
                subDef.unk46 = 0;
                subDef.unk47 = 6;
                if ((state->subParticle = func_1513D4B8(&subDef, &D_800A4AA0, 0, 0, 0, 0x13, 0, sizeof(SubParticleData),
                                                   argD, argE)) != NULL) {
                    memcpy(state->subParticle + 0x110, &subData, sizeof(SubParticleData));
                }
                flashPos.x = 0;
                flashPos.y = 0;
                flashPos.z = 0;
                flashData = particle;
                flashHeader.flags = (hasLifetime ? 1 : 0) | 2;
                flashHeader.update = -1;
                flashHeader.lifetime = lifetime;
                flashHeader.lightType = 7;
                if ((state->flash = func_151602C0(&flashHeader, &flashPos, 5, r, g, b, 0xFF, 0, sizeof(flashData),
                                                  argD, argE)) != NULL) {
                    memcpy(&state->flash->data.object, &flashData, sizeof(flashData));
                }
                particleAData.unk2C = 0;
                particleAData.unk0 = 0.0f;
                particleAData.unk4 = 0.0f;
                particleAData.unkC = 10.25000095f;
                particleAData.unk8 = 10.25000095f;
                particleAData.unk14 = 2.750000238f;
                particleAData.unk10 = 2.750000238f;
                particleAData.parent = particle;
                particleAData.angle[0] = func_150ADA68() * 6.283185482f;
                particleAData.angle[1] = func_150ADA68() * 6.283185482f;
                // matching: '+ -0.3f' (add.s), not '- 0.3f' (sub.s)
                particleAData.unk20 = (func_150ADA68() * 0.5999999642f) + -0.2999999821f;
                particleAData.unk24 = (func_150ADA68() * 0.5999999642f) + -0.2999999821f;
                descA.texture = 0x2B;
                descA.renderMode = 0x3303;
                descA.unk0 = 0x200005;
                descA.unk4 = 0;
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
                descA.flags = ((func_150ADA20() & 1) ? PARTICLE_FLIP_S : 0) |
                             ((hasLifetime ? PARTICLE_LIFETIME : 0) | ((func_150ADA20() & 1) ? PARTICLE_FLIP_T : 0)) |
                             PARTICLE_HANDLERS | PARTICLE_DRAW_FUNC;
                descA.primColorMode = 6;
                descA.envColorMode = 6;
                descA.updateFunc = 11;
                descA.deathFunc = -1;
                descA.drawFunc = 0;
                descA.handlerSet = 1;
                if ((state->particleA = func_15130374(&descA, 1, sizeof(ParticleAData), argD, argE)) != NULL) {
                    memcpy(state->particleA + 0xA8, &particleAData, sizeof(ParticleAData));
                }
                {
                    GlowShape glowParams;
                    ParticleDef glowDef;
                    ChildData glowData;

                    glowData.parent = particle;
                    glowData.unk4 = 0;
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
                    glowDef.unk45 = 0xFF;
                    glowDef.unk44 = 0xFF;
                    glowDef.unk46 = 0;
                    glowDef.unk47 = 6;
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
                    if ((state->glow = func_151407D0(&glowParams, 0x68, &glowDef, 0, 0, 0, 0, 0, argD, argE)) != NULL) {
                        memcpy(state->glow + 0x170, &glowData, sizeof(ChildData));
                    }
                    {
                        ParticleDesc descB;
                        ChildData particleBData;

                        particleBData.unk4 = 0;
                        particleBData.parent = particle;
                        descB.texture = 0x15;
                        descB.renderMode = 0x3203;
                        descB.unk0 = 0x200005;
                        descB.unk4 = 0;
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
                        descB.flags = ((func_150ADA20() & 1) ? PARTICLE_FLIP_S : 0) |
                                      ((hasLifetime ? PARTICLE_LIFETIME : 0) |
                                       ((func_150ADA20() & 1) ? PARTICLE_FLIP_T : 0)) |
                                      PARTICLE_HANDLERS | PARTICLE_DRAW_FUNC;
                        descB.primColorMode = 6;
                        descB.envColorMode = 6;
                        descB.updateFunc = -1;
                        descB.deathFunc = -1;
                        descB.drawFunc = 1;
                        descB.handlerSet = 5;
                        if ((state->particleB = func_15130374(&descB, 1, sizeof(ChildData), argD, argE)) != NULL) {
                            memcpy(state->particleB + 0xA8, &particleBData, sizeof(ChildData));
                        }
                    }
                }
            }
        }
    }
    return particle;
}

/* Releases the beam's child objects, then destroys it. */
void func_15101090(RayBeam *beam) {
    RayBeamData *data = &beam->data;

    // matching: tested as u16 but passed as s16, so IDO reuses the lhu without an andi
    if (*(u16 *)&data->unk90) {
        sound_stop(data->unk90);
    }
    if (data->hitFx != NULL) {
        func_1516972C(&data->hitFx->head.header);
    }
    if (data->endPoint != NULL) {
        func_1516972C(data->endPoint);
    }
    if (data->startFx != NULL) {
        func_1516972C(&data->startFx->head.header);
    }
    if (data->line != NULL) {
        func_1516972C(&data->line->head.header);
    }
    if (data->endFx != NULL) {
        func_1516972C(&data->endFx->head.header);
    }
    func_1513CA6C(&beam->head);
}

/* Counterpart of func_15101090 for the beam's child objects and func_1513CAA0. */
void func_15101148(RayBeam *beam) {
    RayBeamData *data = &beam->data;

    if (data->hitFx != NULL) {
        func_1516979C(&data->hitFx->head.header);
    }
    if (data->endPoint != NULL) {
        func_1516979C(data->endPoint);
    }
    if (data->startFx != NULL) {
        func_1516979C(&data->startFx->head.header);
    }
    if (data->line != NULL) {
        func_1516979C(&data->line->head.header);
    }
    if (data->endFx != NULL) {
        func_1516979C(&data->endFx->head.header);
    }
    func_1513CAA0(&beam->head);
}

/* The functions below detach a child object from its beam before handing it on. */

void func_151011E8(BeamHitFx *fx) {
    RayBeamData *data = &fx->data.beam->data;

    data->hitFx = NULL;
    func_1513CA6C(&fx->head);
}

void func_15101210(BeamHitFx *fx) {
    RayBeamData *data = &fx->data.beam->data;

    data->hitFx = NULL;
    func_1513CAA0(&fx->head);
}

void func_15101238(LightEffect *endPoint) {
    RayBeamData *data = &((RayBeam *)endPoint->data.object)->data;

    data->endPoint = NULL;
    func_151617C4(endPoint);
}

void func_15101260(LightEffect *endPoint) {
    RayBeamData *data = &((RayBeam *)endPoint->data.object)->data;

    data->endPoint = NULL;
    func_151617E4(endPoint);
}

void func_15101288(BeamStartFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->startFx = NULL;
    func_1513173C(&fx->head);
}

void func_151012B0(BeamStartFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->startFx = NULL;
    func_1513175C(&fx->head);
}

void func_151012D8(BeamLine *line) {
    RayBeamData *data = &line->link.beam->data;

    data->line = NULL;
    func_151411A4(&line->head);
}

void func_15101300(BeamLine *line) {
    RayBeamData *data = &line->link.beam->data;

    data->line = NULL;
    func_151411C4(&line->head);
}

void func_15101328(BeamEndFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->endFx = NULL;
    func_1513173C(&fx->head);
}

void func_15101350(BeamEndFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->endFx = NULL;
    func_1513175C(&fx->head);
}

/*
 * Places the beam: start at the owner's bone, end at the owner's aim target or at the first
 * collision along the beam, then moves or hides the child objects to match.
 * Returns 0 when the owner is gone.
 */
s8 func_15101378(RayBeam *beam) {
    s8 result;
    BeamOwnerAim *aim;
    BeamOwner *owner;
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
        bones = owner->boneMtx;
        if (bones != NULL && (D_800C35EA[0] != 1 || (owner->unk74 & 0xF) != 0xF)) {
            boneMtx = bones[data->bone];
            func_15143134(&data->localStart, &beam->head.def.pos, boneMtx);
            if (data->flags & 2) {
                aim = owner->aim;
                hitType = aim->targetType;
                if (hitType != 0) {
                    beam->head.def.rot = owner->aim->targetPos;
                    data->length = owner->aim->targetDist;
                    hitInfo = owner->aim->targetHit;
                    dir.x = beam->head.def.rot.x - beam->head.def.pos.x;
                    dir.y = beam->head.def.rot.y - beam->head.def.pos.y;
                    dir.z = beam->head.def.rot.z - beam->head.def.pos.z;
                } else {
                    endIsHit = 0;
                    hitType = 2;
                    data->length = 2000.0f;
                    dir.x = owner->aim->targetDir.x * 2000.0f;
                    dir.y = owner->aim->targetDir.y * 2000.0f;
                    dir.z = owner->aim->targetDir.z * 2000.0f;
                    beam->head.def.rot.x = beam->head.def.pos.x + dir.x;
                    beam->head.def.rot.y = beam->head.def.pos.y + dir.y;
                    beam->head.def.rot.z = beam->head.def.pos.z + dir.z;
                }
            } else {
                func_15143134(&data->localTarget, &target, boneMtx);
                target.x = target.x - beam->head.def.pos.x;
                target.y = target.y - beam->head.def.pos.y;
                target.z = target.z - beam->head.def.pos.z;
                if (func_150AC9C0(beam->head.def.pos.x, beam->head.def.pos.y, beam->head.def.pos.z, target.x, target.y,
                                  target.z, &unkOut, &hitInfo, &beam->head.def.rot.x, &beam->head.def.rot.y, &beam->head.def.rot.z, 0,
                                  0, 0, 0.0f) != hit * 0) { // matching: reading hit here makes IDO keep it in v1
                    dir.x = beam->head.def.rot.x - beam->head.def.pos.x;
                    dir.y = beam->head.def.rot.y - beam->head.def.pos.y;
                    dir.z = beam->head.def.rot.z - beam->head.def.pos.z;
                    hit = 1;
                    data->length = func_15143E64(&dir);
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
                startFx->head.desc.width = startFx->baseSizeX * data->width;
                startFx->head.desc.height = startFx->baseSizeY * data->width;
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
Vtx *func_151019C4(RayBeam *beam, s16 viewport) {
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

    if (viewport == data->unk22 && (data->owner == NULL || data->owner->aim->unk197 != 0)) {
        return NULL;
    }
    if (data->flags & 1) {
        func_151D5D60(beam->head.vtxBuffers, viewport, sizeof(beam->head.vtxTemplate), &vtx, &isNew);
        result = vtx;
        if (vtx != NULL) {
            if (isNew) {
                memcpy(beam->head.vtxBuffers[viewport], beam->head.vtxTemplate, sizeof(beam->head.vtxTemplate));
                memcpy(beam->head.vtxBuffers[viewport] + 4, beam->head.vtxTemplate, sizeof(beam->head.vtxTemplate));
            }
        } else {
            return NULL;
        }
        eye = func_15144B34(viewport);
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
        func_15145548(&start, &dir, eye, &closest, 0);
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

/* A quad particle (same layout as Particle in game_169510.c), followed by its motion state at 0x110. */
struct Struct15101F50 {
    /* 0x00 */ DefParticleHead head;
    /* 0x110 */ u8 flags;
    /* 0x111 */ u8 unk111;
    /* 0x112 */ CollisionTri surface;
};

/* Fills the viewport's vertex buffer with a quad centred on pos, half-axes axisU * width and axisV * height. */
Vtx *func_15101F50(Struct15101F50 *particle, s16 viewport) {
    s32 pad; // matching: unused stack slot in the original frame
    Vtx *vtx;
    Vtx *ret;
    Vec3f axisU;
    Vec3f axisV;
    u8 isNew;

    func_151D5D60(particle->head.vtxBuffers, viewport, sizeof(particle->head.vtxTemplate), &vtx, &isNew);
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

    if (!(particle->flags & 1)) {
        return NULL;
    }

    if (func_15144E80(&particle->surface, &axisU, &axisV, NULL) == 0) {
        return NULL;
    }

    func_15145128(&axisU, &axisU, NULL, NULL);
    func_15145128(&axisV, &axisV, NULL, NULL);

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

typedef struct {
    u8 pad0[0x14];
    u8 *unk14;
} Sub151022AC_2C;

typedef struct {
    TimedEffect base;
    u8 rgba[4];
} Sub151022AC_24;

typedef struct {
    u8 pad0[0x2C];
    f32 width;
    f32 height;
    u8 pad34[0x28];
    u8 alpha;
} Sub151022AC_28;

typedef struct {
    u8 pad0[0x38];
    f32 unk38;
    f32 unk3C;
} Sub151022AC_34;

/* Randomly wandering spin, scale and alpha, pushed out to the linked objects each frame. */
typedef struct {
    u8 pad0[0x24];
    Sub151022AC_24 *linked24;
    Sub151022AC_28 *linked28;
    Sub151022AC_2C *linked2C;
    u8 pad30[4];
    Sub151022AC_34 *linked34;
    f32 scale;
    f32 targetScale;
    f32 scaleBlend;
    u8 pad44[0xC];
    f32 angle;
    f32 rate;
    f32 rotation;
    f32 velocity;
    f32 targetVelocity;
    f32 velocityRange;
    f32 timer;
    f32 timerRange;
    f32 velocityBlend;
    f32 alpha;
    f32 targetAlpha;
    f32 alphaBase;
    f32 alphaRange;
    f32 alphaTimer;
    f32 alphaTimerRange;
    f32 alphaBlend;
} Flicker151022AC;

struct Owner151022AC {
    u8 pad0[0x5C];
    u8 alpha;
    u8 pad5D[0xB3];
    Flicker151022AC flicker;
};

/* Per-frame update: advances the angle and spin, and eases scale and alpha toward random targets. */
s32 func_151022AC(Owner151022AC *obj) {
    Flicker151022AC *state;
    Sub151022AC_28 *quad;
    Sub151022AC_2C *particles;
    Sub151022AC_34 *child;

    state = &obj->flicker;
    state->angle += state->rate * gFrameDelta;
    state->angle = func_15144B68(state->angle);
    state->rotation += state->velocity * gFrameDelta;
    while (state->rotation > 32767.0f) {
        state->rotation -= 65536.0f;
    }
    while (state->rotation < -32768.0f) {
        state->rotation += 65536.0f;
    }
    state->timer -= gFrameDelta;
    if (state->timer < 0.0f) {
        state->timer = func_150ADA68() * state->timerRange;
        state->targetVelocity = func_150ADA68() * (state->velocityRange + state->velocityRange) - state->velocityRange;
    }
    state->velocity += (state->targetVelocity - state->velocity) * state->velocityBlend;
    state->scale += (state->targetScale - state->scale) * state->scaleBlend;
    if (state->linked28 != NULL) {
        quad = state->linked28;
        quad->width = quad->height = (func_150ADA68() * 0.2f + 0.4f) * state->scale;
    }
    state->alphaTimer -= gFrameDelta;
    if (state->alphaTimer < 0.0f) {
        state->alphaTimer = func_150ADA68() * state->alphaTimerRange;
        state->targetAlpha = func_150ADA68() * state->alphaRange + state->alphaBase;
    }
    state->alpha += (state->targetAlpha - state->alpha) * state->alphaBlend;
    obj->alpha = state->alpha;
    if (state->linked28 != NULL) {
        state->linked28->alpha = obj->alpha;
    }
    if (state->linked24 != NULL) {
        state->linked24->rgba[3] = obj->alpha;
    }
    if (state->linked2C != NULL) {
        particles = state->linked2C;
        particles->unk14[0x2F] = (func_150ADA20() & 3) + 2;
    }
    if (state->linked34 != NULL) {
        child = state->linked34;
        child->unk38 = child->unk3C = func_150ADA68() * 10.0f + 120.0f;
    }
    return 1;
}

/* A 2D sine wobble: offset = base + amplitude * sin(phase), with phase advancing at speed. */
typedef struct {
    f32 offset[2];
    f32 base[2];
    f32 amplitude[2];
    f32 phase[2];
    f32 speed[2];
} Wobble151025F4;

struct Owner151025F4 {
    DescParticleHead head;
    Wobble151025F4 wobble;
};

/* Per-frame update of the owner's wobble offset. */
s32 func_151025F4(Owner151025F4 *obj, s32 arg1) {
    Wobble151025F4 *wobble = &obj->wobble;
    s32 pad; // matching: unused stack slot in the original frame
    f32 sinX;
    f32 sinY;

    sinX = sinf(wobble->phase[0]);
    sinY = sinf(wobble->phase[1]);
    wobble->offset[0] = wobble->amplitude[0] * sinX + wobble->base[0];
    wobble->offset[1] = wobble->amplitude[1] * sinY + wobble->base[1];
    wobble->phase[0] += wobble->speed[0] * gFrameDelta;
    wobble->phase[1] += wobble->speed[1] * gFrameDelta;
    wobble->phase[0] = func_15144B68(wobble->phase[0]);
    wobble->phase[1] = func_15144B68(wobble->phase[1]);
    return 1;
}

/* Starts effect `kind` on obj, using the effect tables for its parameters; picks one of two styles by object id. */
EffectParticle *func_151026BC(Actor *obj, s16 arg1, u8 kind, u8 arg3, u8 arg4, s32 arg5) {
    s32 unk5;
    s32 unk6;

    if (obj == NULL) {
        return NULL;
    }
    switch (obj->character_type) {
        case 0x58:
        case 0x5B:
        case 0x80:
            unk5 = 0x32;
            unk6 = 0xFF;
            break;
        case 0x5A:
        case 0x5F:
        case 0x74:
        case 0x75:
        case 0x7A:
            unk5 = 0xFF;
            unk6 = 0;
            break;
        default:
            if (obj->mv->rt_context_b == 0x3B) {
                unk5 = 0xFF;
                unk6 = 0;
            } else {
                unk5 = 0x32;
                unk6 = 0xFF;
            }
            break;
    }
    // matching: the int 20 is converted separately; 20.0f would share one register with the other 20.0f
    return func_15100600(obj, &D_800A21A0[kind], &D_800A21F4[kind], D_800A2248[kind], arg1, unk5, unk6, 0,
                         20.0f, 20, 4.0f, 0.08f, arg3, arg4, arg5);
}

/* Sends message 0x1A for `actor` (stops the effect started by func_151026BC). */
void func_151027E8(Actor *actor) {
    ActorRef msg;

    msg.actor = actor;
    msg.uniqueId = actor->unique_id;
    func_151403A8((s32)&msg, 0x1A);
}


/* State block at +0x110 of the objects referenced by Owner1510281C. */
typedef struct {
    Actor *linked;
    u8 pad4[0x1E];
    u8 unk22;
} Block1510281C;

typedef struct {
    u8 pad0[0x110];
    Block1510281C block;
} Obj1510281C;

struct Owner1510281C {
    u8 pad0[0xD0];
    Obj1510281C *unkD0;
    u8 unkD4;
    u8 padD5[0x9B];
    Obj1510281C *unk170;
    u8 unk174;
};

/* Returns 0 if unkD0's block is keyed to `key` and its linked object is missing or busy, else bit 0 of unkD4. */
s32 func_1510281C(Owner1510281C *owner, s16 key) {
    Block1510281C *block = &owner->unkD0->block;
    Actor *linked;

    if (key == block->unk22) {
        linked = block->linked;
        if (linked == NULL || linked->mv->SniperState != 0) {
            return 0;
        }
    }
    if (!(owner->unkD4 & 1)) {
        return 0;
    }
    return 1;
}

struct Obj15102884 {
    u8 pad0[0xAC];
    u8 unkAC;
};

/* Returns bit 0 of obj->unkAC. */
s32 func_15102884(Obj15102884 *obj, s32 arg1) {
    if (!(obj->unkAC & 1)) {
        return 0;
    }
    return 1;
}

/* Same test as func_1510281C, on unk170 and unk174. */
s32 func_151028AC(Owner1510281C *owner, s16 key) {
    Block1510281C *block = &owner->unk170->block;
    Actor *linked;

    if (key == block->unk22) {
        linked = block->linked;
        if (linked == NULL || linked->mv->SniperState != 0) {
            return 0;
        }
    }
    if (!(owner->unk174 & 1)) {
        return 0;
    }
    return 1;
}
