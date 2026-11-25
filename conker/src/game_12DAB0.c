#include <ultra64.h>
/* functions.h declares these two void, but both return a value. */
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

/*
 * Spawns an effect on owner: a main particle carrying an EffectParticleState plus five children (a particle,
 * a flash, a glow and two func_15130374 particles) that keep a pointer back to it. lifetime -1 means
 * endless. Returns the main particle, or NULL.
 */
EffectParticle *func_15100600(Actor *owner, Vec3f *vec1, Vec3f *vec2, u8 arg3, s16 lifetime, u8 r,
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
        data.sound = func_10010F30(0x4D0, 2000, 0x40, 0, 0);
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
