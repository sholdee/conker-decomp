#include <ultra64.h>

#include "conker/beam.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/flare.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/particle.h"

/* Spawns four emitters sharing one setup; only the packet position and rotation change. */
void level_setupThroneRoom(void) {
    GlowShape params;
    ParticleDef packet;
    GlowFlicker extra;
    GlowObj *emitter;

    extra.base = 1.0420000553f;
    extra.low = 0.9010000229f;
    extra.high = 1.2009999752f;
    extra.target = 1.0f;
    extra.timer = 0.0f;
    extra.period = 7.0f;
    extra.rate = 0.7023000121f;

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
    packet.colorLevel = 255;
    packet.alpha = 255;
    packet.primMode = 0;
    packet.envMode = 6;

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

    emitter = flare_spawnGlow(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->params.anim.flicker, &extra, sizeof(extra));
    }

    packet.pos.x = -520.0f;
    packet.rot.x = -520.0f;
    packet.pos.y = 72.0f;
    packet.rot.y = 72.0f;
    packet.pos.z = 2337.0f;
    packet.rot.z = 2220.0f;
    emitter = flare_spawnGlow(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->params.anim.flicker, &extra, sizeof(extra));
    }

    packet.pos.y = 70.0f;
    packet.rot.y = 70.0f;
    packet.pos.z = 926.0f;
    packet.rot.z = 926.0f;
    packet.pos.x = -710.0f;
    packet.rot.x = -584.0f;
    emitter = flare_spawnGlow(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->params.anim.flicker, &extra, sizeof(extra));
    }

    packet.pos.x = 506.0f;
    packet.rot.x = 506.0f;
    packet.pos.y = 47.0f;
    packet.rot.y = 47.0f;
    packet.pos.z = -339.0f;
    packet.rot.z = -208.0f;
    emitter = flare_spawnGlow(&params, 0x7C, &packet, 0, 32, 0, 0, -1, 255, 1);
    if (emitter != NULL) {
        memcpy(&emitter->params.anim.flicker, &extra, sizeof(extra));
    }
}

/* Spawns an effect tied to owner, with a moving child effect and a light attached to it. */
void level_spawnModelBeam(Actor *owner, u8 fxOwner, s32 list) {
    ModelBeamEffect *object;
    ModelBeamData data;
    ModelBeamData *controller;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.beam = NULL;
    data.flash = NULL;
    object = effect_newTimedEffect(300, -1, 0x54, -1, 0, 0x34, sizeof(ModelBeamData), fxOwner, list);
    if (object != NULL) {
        controller = &object->data;
        memcpy(controller, &data, sizeof(data));
        // matching: the -g3 scope entry makes IDO load the shared constants at the top of this block
        {
            BeamPart motion;
            ParticleDef packet;
            Vec3i header2;
            LightEffectHeader header;

            motion.flags = 0;
            motion.scale = (owner->scale_xz + owner->scale_y) * 0.5f;
            motion.owner = owner;
            motion.ownerId = owner->unique_id;
            motion.widthBase = 15.04527187f;
            motion.widthRange = 5.867655754f;
            motion.widthTarget = 12.11144447f;
            motion.heightBase = 0.3961740136f;
            motion.widthTimer = 0.0f;
            motion.heightTimer = 0.0f;
            motion.alphaTimer = 0.0f;
            packet.width = 0.0f;
            packet.height = 0.0f;
            motion.alpha = 0.0f;
            motion.beamWidthTarget = 1.0f;
            motion.beamWidth = 1.0f;
            motion.beamLengthTarget = 1.0f;
            motion.beamLength = 1.0f;
            motion.effect = object;
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
            motion.heightRange = 0.1042563021f;
            motion.heightRange2 = 0.5197371244f;
            motion.heightTarget = 0.3440458775f;
            motion.alphaBase = 160.0f;
            motion.alphaRange = 95.0f;
            motion.alphaTarget = 207.5f;
            packet.pos = gZeroVec3f;
            packet.rot = gZeroVec3f;
            packet.scale.x = 1.0f;
            packet.scale.y = 1.0f;
            packet.scale.z = 1.0f;
            packet.flags = 0x40EC0000; // bit pattern of 7.375f
            packet.alpha = 0;
            packet.colorLevel = 255;
            packet.primMode = 0;
            packet.envMode = 7;
            packet.zone = 0;
            controller->beam = particle_newQuadParticle(&packet, &gWhiteVtxColors, 0, 0x25, 0, 0x1D,
                                               ((math_random() & 1) ? 2 : 0) + 1, 0, 0, sizeof(motion), fxOwner, list);
            if (controller->beam != NULL) {
                memcpy(&controller->beam->part, &motion, sizeof(motion));
            }
            header.flags = 2;
            header.update = -1;
            header.lifetime = 300;
            header.lightType = 0;
            header2.x = 0;
            header2.y = 0;
            header2.z = 0;
            controller->flash = light_createWithEffect(&header, &header2, 40, 255, 255, 255, 255, 1, 0, fxOwner, list);
        }
    }
}

/* Spawns timed effect 85 (beam_updateFlashBurst): light bursts on the owner while it lives. */
void level_spawnFlashBurst(Actor *owner, u8 fxOwner, s32 list) {
    FlashBurstData data;
    FlashBurstEffect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.timer = 0;

    effect = effect_newTimedEffect(300, -1, 0x55, -1, 0, 0x42, sizeof(FlashBurstData), fxOwner, list);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}
