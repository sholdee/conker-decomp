#include <ultra64.h>
/* functions.h declares these two void, but both return a value. */
#define func_10010F30 func_10010F30_void
#define func_1513D4B8 func_1513D4B8_void
#include "functions.h"
#undef func_10010F30
#undef func_1513D4B8
#include "variables.h"
#include "effects.h"
#include "libc/string.h"

/* ParticleDesc.flags (see game_15D730.c) */
#define PARTICLE_LIFETIME 0x1
#define PARTICLE_FLIP_S   0x40
#define PARTICLE_FLIP_T   0x80
#define PARTICLE_HANDLERS 0x4000
#define PARTICLE_DRAW_FUNC 0x8000

/* Glow parameters handed to func_151407D0 (GlowParams in game_16DC80.c, without the animation union). */
typedef struct {
    /* 0x00 */ f32 width1;      /* width, height and offset when facing the viewer... */
    /* 0x04 */ f32 height1;
    /* 0x08 */ f32 width0;      /* ...and when facing away */
    /* 0x0C */ f32 height0;
    /* 0x10 */ f32 offset1;
    /* 0x14 */ f32 offset0;
    /* 0x18 */ f32 alpha1;
    /* 0x1C */ f32 alpha0;
    /* 0x20 */ f32 fadeRate;
    /* 0x24 */ s32 screenX[4];
    /* 0x34 */ s32 screenY[4];
    /* 0x44 */ void *attach;
    /* 0x48 */ f32 scale;
    /* 0x4C */ u16 zbufSample[4];
    /* 0x54 */ u8 visibility[4];
    /* 0x58 */ u8 type;
    /* 0x59 */ u8 pad59[3];
} GlowParams;

/* State of the effect, stored after its main particle. */
typedef struct {
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId;          /* owner->unique_id */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ struct17 unk8;
    /* 0x14 */ struct17 unk14;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ s8 ownerSlot;        /* owner's index in D_800CC2D0 */
    /* 0x22 */ u8 unk22;            /* owner->camera->playerIndex, 0xFF without a camera */
    /* 0x23 */ u8 unk23;
    /* 0x24 */ u8 *particleA;       /* func_15130374 particle */
    /* 0x28 */ u8 *subParticle;     /* func_1513D4B8 particle */
    /* 0x2C */ struct225 *flash;    /* func_151602C0 */
    /* 0x30 */ u8 *glow;            /* func_151407D0 glow */
    /* 0x34 */ u8 *particleB;       /* func_15130374 particle */
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ f32 unk5C;
    /* 0x60 */ f32 unk60;
    /* 0x64 */ f32 unk64;
    /* 0x68 */ f32 unk68;
    /* 0x6C */ f32 unk6C;
    /* 0x70 */ f32 unk70;
    /* 0x74 */ f32 unk74;
    /* 0x78 */ f32 unk78;
    /* 0x7C */ f32 unk7C;
    /* 0x80 */ f32 unk80;
    /* 0x84 */ f32 unk84;
    /* 0x88 */ f32 unk88;
    /* 0x8C */ f32 unk8C;
    /* 0x90 */ s16 sound;           /* func_10010F30 handle */
    /* 0x92 */ u8 pad92[2];
} EffectState; /* size 0x94 */

/* The main particle: a func_1513D4B8 particle (0x110 bytes) followed by its EffectState. */
typedef struct {
    u8 pad0[0x110];
    EffectState state;
} EffectParticle;

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

extern s32 func_10010F30(s32, u16, u8, s16, u8);
extern void *func_1513D4B8(ParticleDef *, s32 *, u8, u8, u8, u8, u8, s32, u8, s32);
extern u8 *func_15130374(ParticleDesc *, u8, s32, u8, s32);
extern u8 *func_151407D0(GlowParams *, s32, ParticleDef *, u8, u8, u8, u8, s8, u8, s32);
extern struct225 *func_151602C0(Header *, Header2 *, s32, u8, u8, u8, u8, u8, s32, u8, s32);

/*
 * Spawns an effect on owner: a main particle carrying an EffectState plus five children (a particle,
 * a flash, a glow and two func_15130374 particles) that keep a pointer back to it. lifetime -1 means
 * endless. Returns the main particle, or NULL.
 */
EffectParticle *func_15100600(struct127 *owner, struct17 *vec1, struct17 *vec2, u8 arg3, s16 lifetime, u8 r,
                              u8 g, u8 b, f32 arg8, f32 arg9, f32 argA, f32 argB, u8 argC, u8 argD, s32 argE) {
    EffectParticle *particle;
    ParticleDef def;
    EffectState data;
    u8 hasLifetime;
    EffectState *state;

    if (lifetime == -1) {
        hasLifetime = FALSE;
        lifetime = 300;
    } else {
        hasLifetime = TRUE;
    }
    data.ownerSlot = owner - D_800CC2D0;
    if (owner->camera != NULL) {
        data.unk22 = owner->camera->playerIndex;
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
    if (owner->camera != NULL) {
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
    particle = func_1513D4B8(&def, &D_800A4AA0, 0x15, 0x1F, 0, 0x12, 0, sizeof(EffectState), argD, argE);
    if (particle != NULL) {
        state = &particle->state;
        memcpy(state, &data, sizeof(EffectState));
        {
            ParticleDef subDef;
            SubParticleData subData;

            subData.parent = particle;
            {
                Header flashHeader;
                Header2 flashPos;
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
                flashPos.unk0 = 0;
                flashPos.unk4 = 0;
                flashPos.unk8 = 0;
                flashData = particle;
                flashHeader.unk0 = (hasLifetime ? 1 : 0) | 2;
                flashHeader.unk1 = -1;
                flashHeader.unk2 = lifetime;
                flashHeader.unk4 = 7;
                if ((state->flash = func_151602C0(&flashHeader, &flashPos, 5, r, g, b, 0xFF, 0, sizeof(flashData),
                                                  argD, argE)) != NULL) {
                    memcpy(&state->flash->unk18, &flashData, sizeof(flashData));
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
                    GlowParams glowParams;
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
