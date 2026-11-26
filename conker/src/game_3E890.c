#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Extra data copied to offset 0x170 of each emitter func_150113E0 spawns. */
typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
} EmitterExtra; /* size 0x1C */

/* The particle object func_151407D0 returns, as func_150113E0 uses it. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ u8 pad110[0x60];
    /* 0x170 */ EmitterExtra extra;
} GlowEmitter;

/* Spawns four emitters sharing one setup; only the packet position and rotation change. */
void func_150113E0(void) {
    GlowShape params;
    ParticleDef packet;
    EmitterExtra extra;
    GlowEmitter *emitter;

    extra.unk00 = 1.0420000553f;
    extra.unk04 = 0.9010000229f;
    extra.unk08 = 1.2009999752f;
    extra.unk0C = 1.0f;
    extra.unk10 = 0.0f;
    extra.unk14 = 7.0f;
    extra.unk18 = 0.7023000121f;

    packet.texture = 3;
    packet.renderModes = 0x3403;
    packet.lifetime = 300;
    packet.frame = 0;
    packet.frameSpeed = 0;
    packet.rgba[0] = 230;
    packet.rgba[1] = 100;
    packet.rgba[2] = 0;
    packet.rgba[3] = 255;
    packet.width = 100.0f;
    packet.height = 100.0f;
    packet.pos.x = 520.0f;
    packet.pos.y = 72.0f;
    packet.pos.z = 2337.0f;
    packet.rot.x = 520.0f;
    packet.rot.y = 72.0f;
    packet.rot.z = 2220.0f;
    packet.scale.x = 1.0f;
    packet.scale.y = 1.0f;
    packet.scale.z = 1.0f;
    packet.flags = 0xD2012;
    packet.unk45 = 255;
    packet.unk44 = 255;
    packet.unk46 = 0;
    packet.unk47 = 6;

    params.height1 = 175.5f;
    params.width1 = 175.5f;
    params.height0 = 115.4000015259f;
    params.width0 = 115.4000015259f;
    params.offset1 = 0.4099999964f;
    params.offset0 = 0.2000000030f;
    params.alpha1 = 0.3499999940f;
    params.alpha0 = 0.3499999940f;
    params.fadeRate = 9.999999975e-07f;
    params.visibility[0] = 0;
    params.visibility[1] = 0;
    params.visibility[2] = 0;
    params.visibility[3] = 0;
    params.scale = 1.0f;
    params.type = 0;

    emitter = func_151407D0(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->extra, &extra, sizeof(extra));
    }

    packet.pos.x = -520.0f;
    packet.rot.x = -520.0f;
    packet.pos.y = 72.0f;
    packet.rot.y = 72.0f;
    packet.pos.z = 2337.0f;
    packet.rot.z = 2220.0f;
    emitter = func_151407D0(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->extra, &extra, sizeof(extra));
    }

    packet.pos.y = 70.0f;
    packet.rot.y = 70.0f;
    packet.pos.z = 926.0f;
    packet.rot.z = 926.0f;
    packet.pos.x = -710.0f;
    packet.rot.x = -584.0f;
    emitter = func_151407D0(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->extra, &extra, sizeof(extra));
    }

    packet.pos.x = 506.0f;
    packet.rot.x = 506.0f;
    packet.pos.y = 47.0f;
    packet.rot.y = 47.0f;
    packet.pos.z = -339.0f;
    packet.rot.z = -208.0f;
    emitter = func_151407D0(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->extra, &extra, sizeof(extra));
    }
}

/* Per-owner data stored in the effect object func_1501175C spawns. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 id;
    /* 0x08 */ struct MotionEffect *effect;
    /* 0x0C */ LightEffect *light;
} Controller; /* size 0x10 */

typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Controller controller;
} ControllerEffect;

/* Motion data copied into the effect spawned by func_1513D2F0. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 id;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ ControllerEffect *controller;
    /* 0x54 */ f32 size;
    /* 0x58 */ u8 unk58;
} ControllerMotion; /* size 0x5C */

typedef struct MotionEffect {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ ControllerMotion motion;
} MotionEffect;

/* Spawns an effect tied to owner, with a moving child effect and a light attached to it. */
void func_1501175C(Actor *owner, u8 arg1, s32 arg2) {
    ControllerEffect *object;
    Controller data;
    Controller *controller;

    data.owner = owner;
    data.id = owner->unique_id;
    data.effect = NULL;
    data.light = NULL;
    object = func_15149130(300, -1, 0x54, -1, 0, 0x34, sizeof(Controller), arg1, arg2);
    if (object != NULL) {
        controller = &object->controller;
        memcpy(controller, &data, sizeof(data));
        // matching: the -g3 scope entry makes IDO load the shared constants at the top of this block
        {
            ControllerMotion motion;
            ParticleDef packet;
            Vec3i header2;
            LightEffectHeader header;

            motion.unk58 = 0;
            motion.size = (owner->scale_xz + owner->scale_y) * 0.5f;
            motion.owner = owner;
            motion.id = owner->unique_id;
            motion.unk08 = 15.04527187f;
            motion.unk10 = 5.867655754f;
            motion.unk18 = 12.11144447f;
            motion.unk0C = 0.3961740136f;
            motion.unk24 = 0.0f;
            motion.unk28 = 0.0f;
            motion.unk38 = 0.0f;
            packet.width = 0.0f;
            packet.height = 0.0f;
            motion.unk3C = 0.0f;
            motion.unk48 = 1.0f;
            motion.unk40 = 1.0f;
            motion.unk4C = 1.0f;
            motion.unk44 = 1.0f;
            motion.controller = object;
            packet.texture = 0x5F;
            packet.type = 9;
            packet.renderModes = 0x2203;
            packet.lifetime = 300;
            packet.frame = 0;
            packet.frameSpeed = 0;
            packet.rgba[0] = 255;
            packet.rgba[1] = 255;
            packet.rgba[2] = 255;
            packet.rgba[3] = 255;
            motion.unk14 = 0.1042563021f;
            motion.unk20 = 0.5197371244f;
            motion.unk1C = 0.3440458775f;
            motion.unk2C = 160.0f;
            motion.unk30 = 95.0f;
            motion.unk34 = 207.5f;
            packet.pos = D_800A5480;
            packet.rot = D_800A5480;
            packet.scale.x = 1.0f;
            packet.scale.y = 1.0f;
            packet.scale.z = 1.0f;
            packet.flags = 0x40EC0000; // bit pattern of 7.375f
            packet.unk44 = 0;
            packet.unk45 = 255;
            packet.unk46 = 0;
            packet.unk47 = 7;
            packet.unk48 = 0;
            controller->effect = func_1513D2F0(&packet, &D_800A4AA0, 0, 0x25, 0, 0x1D,
                                               ((func_150ADA20() & 1) ? 2 : 0) + 1, 0, 0, sizeof(motion), arg1, arg2);
            if (controller->effect != NULL) {
                memcpy(&controller->effect->motion, &motion, sizeof(motion));
            }
            header.flags = 2;
            header.update = -1;
            header.lifetime = 300;
            header.lightType = 0;
            header2.x = 0;
            header2.y = 0;
            header2.z = 0;
            controller->light = light_createWithEffect(&header, &header2, 40, 255, 255, 255, 255, 1, 0, arg1, arg2);
        }
    }
}

/* Spawns timed effect 85 (beam_updateFlashBurst): light bursts on the owner while it lives. */
void func_15011A78(Actor *owner, u8 arg1, s32 arg2) {
    FlashBurstData data;
    FlashBurstEffect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.timer = 0;

    effect = func_15149130(300, -1, 0x55, -1, 0, 0x42, sizeof(FlashBurstData), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}
