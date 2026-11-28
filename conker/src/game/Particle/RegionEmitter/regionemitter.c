#include <ultra64.h>
#include "effects.h"
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/mp.h"
#include "conker/particle.h"
#include "conker/sound.h"

/*
 * Per-frame update of a particle emitter effect: accumulates rate * scale * gFrameDelta and spawns one
 * particle per whole unit at a random point of the emitter's region, optionally snapped to the ground.
 */
void particle_updateRegionEmitter(u8 *effect) {
    RegionEmitterState *emitter;
    ParticleEmitterDesc particle;
    f32 low;
    f32 high;
    f32 xScale;
    f32 yScale;

    emitter = (RegionEmitterState *)(effect + 0x28);
    if (emitter->region == NULL) {
        *(s16 *)(effect + 0xE) = -1;
        return;
    }
    if (emitter->region->inactive == 1) {
        return;
    }
    if (emitter->zone != NULL && math_isZoneHiddenFromViews(emitter->zone) != 0) {
        return;
    }
    emitter->pending += emitter->scale * emitter->rate * gFrameDelta;
    if (emitter->pending > 1.0f) {
        particle.maxC = 0x6231;
        particle.minC = 0x1A4D;
        particle.color[0] = 0;
        particle.color[1] = 0;
        particle.color[2] = 0;
        particle.color[3] = 0xFF;
        particle.flags = 1;
        particle.colorLevel = 0xFF;
        particle.extraSize = 0;
        particle.updateFn1 = 0;
        particle.expireFn = 0;
        particle.effect = 0x10;
        particle.maxHeight = emitter->maxHeight;
        do {
            switch (ZONE_AREA_TYPE(emitter->region)) {
            case 2: {
                u8 angle;
                f32 cosine;
                f32 sine;
                f32 x;
                f32 z;

                angle = emitter->region->yaw * 0.7111111f;
                cosine = math_cosBinAngleMasked(angle - 0x40);
                sine = math_cosBinAngleMasked(angle);
                x = math_randomFloat() * (2.0f * emitter->region->xSize) + -emitter->region->xSize;
                z = math_randomFloat() * (2.0f * emitter->region->zSize) + -emitter->region->zSize;
                particle.pos.x = emitter->region->x + (x * sine + z * cosine);
                particle.pos.z = emitter->region->z + (z * sine - x * cosine);
                high = emitter->region->y + emitter->region->ySize;
                low = emitter->region->y - emitter->region->ySize;
            } break;
            case 0: {
                u8 angle;
                f32 cosine;
                f32 sine;
                f32 x;

                angle = math_random() & 0xFF;
                cosine = math_cosBinAngleMasked(angle - 0x40);
                sine = math_cosBinAngleMasked(angle);
                x = math_randomFloat() * emitter->region->xSize;
                particle.pos.x = emitter->region->x + x * sine;
                particle.pos.z = emitter->region->z - x * cosine;
                high = emitter->region->y + emitter->region->ySize;
                low = emitter->region->y;
            } break;
            case 1: {
                u8 angle;
                f32 cosine;
                f32 sine;
                f32 x;

                angle = math_random() & 0xFF;
                cosine = math_cosBinAngleMasked(angle - 0x40);
                sine = math_cosBinAngleMasked(angle);
                x = math_randomFloat() * emitter->region->xSize;
                particle.pos.x = emitter->region->x + x * sine;
                particle.pos.z = emitter->region->z - x * cosine;
                high = emitter->region->y + emitter->region->ySize;
                low = emitter->region->y - emitter->region->ySize;
            } break;
            default:
                particle.pos.x = emitter->region->x;
                particle.pos.z = emitter->region->z;
                high = emitter->region->y + emitter->region->ySize;
                low = emitter->region->y - emitter->region->ySize;
                break;
            }
            if (emitter->useGround != 0) {
                Vec3f probe;

                probe.x = particle.pos.x;
                probe.y = high;
                probe.z = particle.pos.z;
                if (collision_probeFloorSurfABSet0Cached(&probe, 0, low, &emitter->ground) != 0) {
                    particle.pos.y = emitter->ground.height;
                } else {
                    particle.pos.y = low;
                }
            } else {
                particle.pos.y = low;
            }
            xScale = math_randomFloat() * emitter->xScaleRange + emitter->xScaleBase;
            yScale = math_randomFloat() * emitter->yScaleRange + emitter->yScaleBase;
            particle.maxA = 22.6f * xScale;
            particle.minA = 13.786f * xScale;
            particle.maxB = 57.969f * yScale;
            particle.minB = 42.714f * yScale;
            particle.altB = 118.763f * yScale;
            particle.life = math_random() % (emitter->lifeRange + 1) + emitter->lifeBase;
            particle.scale = math_randomFloat() * emitter->scaleRange + emitter->scaleBase;
            particle.shrinkRate = math_randomFloat() * emitter->shrinkRateRange + emitter->shrinkRateBase;
            particle.growRate = math_randomFloat() * emitter->growRateRange + emitter->growRateBase;
            particle.alpha = math_random() % (emitter->alphaRange + 1) + emitter->alphaBase;
            particle_spawnChannelParticle(&particle, 10, 0, 0, effect[0xC], effect[1]);
            emitter->pending -= 1.0f;
        } while (emitter->pending > 1.0f);
    }
}

/* Effect state embedded at offset 0x58 of the TimedEffect passed to the two functions below. */
typedef struct {
    u8 pad0[0x14];
    /* 0x14 */ u16 soundHandle;
} EffectSoundState;

/* Releases the effect's sound (if any), then hands the effect to effect_defaultDestroyTimedEffect. */
void particle_destroyRegionEmitter(TimedEffect *effect) {
    EffectSoundState *state = (EffectSoundState *)((u8 *)effect + 0x58);

    // matching: the test reads through effect and the call through state, so IDO loads the handle twice
    if (*(u16 *)((u8 *)effect + 0x6C) != 0) {
        particle_stopEffectSound(state->soundHandle);
    }
    effect_defaultDestroyTimedEffect(effect);
}

/* Releases the effect's sound (if any), then hands the effect to effect_defaultFreeTimedEffect. */
void particle_freeRegionEmitter(TimedEffect *effect) {
    EffectSoundState *state = (EffectSoundState *)((u8 *)effect + 0x58);

    // matching: the test reads through effect and the call through state, so IDO loads the handle twice
    if (*(u16 *)((u8 *)effect + 0x6C) != 0) {
        particle_stopEffectSound(state->soundHandle);
    }
    effect_defaultFreeTimedEffect(effect);
}

/* Releases a sound handle. */
void particle_stopEffectSound(u16 soundHandle) {
    sound_stop(soundHandle);
}

/*
 * Spawns an effect via particle_spawnSurfaceQuadSprite at pos, scaled by size on both axes, living duration + 20 frames.
 * Returns NULL if duration <= 0, so the duration == -1 cases below are unreachable (the original has them).
 */
QuadSprite *particle_spawnSurfaceMark(f32 size, u8 alpha, CollisionTri *surface, Vec3f *pos, s16 duration, u8 lit, u8 arg6,
                          u8 owner, s32 list) {
    SpriteDef2C params;

    if (duration <= 0) {
        return NULL;
    }
    params.fadeStart = 20;
    params.fadeRate = 12;
    params.texture = 0x38;
    params.handlerSet = 0;
    params.flags = ((duration == -1) ? 0 : 1) | (lit ? 0x400 : 0) | 0x100 | 0x200 | 0x1000 | 0x2000 | 0x8000 | 0x20000;
    if (duration == -1) {
        params.lifetime = 300;
    } else {
        params.lifetime = duration + 20;
    }
    params.frame = 0;
    if (duration == -1) {
        params.frameSpeed = 0;
    } else {
        params.frameSpeed = 0x140000 / (duration + 20);
    }
    params.alpha = alpha;
    params.colorLevel = 0xFF;
    params.rgba[0] = 0xFF;
    params.rgba[1] = 0x83;
    params.rgba[2] = 30;
    params.rgba[3] = 0xFF;
    params.renderModes = (arg6 ? 2 : 3) + 0x440000;
    params.primMode = 0;
    params.envMode = 6;
    params.viewportMask = 0xFF;
    return particle_spawnSurfaceQuadSprite(&params, 0, 0, surface, pos->x, pos->y, pos->z, size, size,
                         math_random() & 0xFF, (math_random() & 1) + ((math_random() & 1) * 2),
                         lit ? 3 : 0, lit ? 0xFF : 0, 0, owner, list);
}

/* Message handler of the zone effects (gTimedEffectMessageFuncs[0x2C]): on message 0x35 whose id list contains the
 * effect's id, fires the effect's zone once (while its inactive is 1) and records the message's player. Bit 0 of
 * flags plays sounds, bit 1 is copied into the zone emitter's hasZone; node is the spawn zone (ParticleDef.zone). */
void particle_handleZoneEffectMessage(ZoneEffect *listener, void *data, u8 type) {
    ZoneTriggerMsg *msg = data;
    s32 i;
    s32 found;
    ZoneEffectData *target;

    if (type != 0x35) {
        return;
    }
    target = &listener->data;
    i = 0;
    found = FALSE;
    while (i < msg->count && !found) {
        if (target->id == msg->ids[i]) {
            found = TRUE;
        } else {
            i++;
        }
    }
    if (found && target->zone->inactive == 1) {
        particle_fireRegionEmitter(target->zone, target->node, target->flags & 1, target->flags & 2,
                      listener->base.header.owner, listener->base.header.kind);
        target->zone->inactive = 0;
        target->actor = msg->player;
    }
}

/* Spawns an effect object at the emitter; with playSound also starts a light_createJitterAt handle at the
 * emitter's mid-height and plays sound 0x1AA there. */
void particle_fireRegionEmitter(TriggerZone *emitter, ZoneNode *zone, u8 playSound, u8 arg3, u8 owner, s32 list) {
    ParticleDef spawn;
    ZoneEmitterState init;
    ZoneEmitter *effect;
    ZoneEmitterState *state;
    LightEffectHeader header;
    Vec3i pos;

    init.region = emitter;
    init.texScroll = 0.0f;
    init.texScrollSpeed = (math_randomFloat() * 80.0f) + -380.0f; // matching: add of a negative literal, not sub.s
    init.texSpan = (math_randomFloat() * 1300.0f) + 2700.0f;
    init.light = NULL;
    init.phase = math_random();
    init.phaseSpeed = ((u8)math_random() & 3) + 4; // matching: the u8 narrowing changes the andi codegen
    init.alpha = 0xFF;
    init.pending = 0.0f;
    init.hasZone = arg3;
    spawn.texture = 0x6B;
    spawn.type = 10;
    spawn.renderModes = 0x4C03;
    spawn.lifetime = 240;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0xFF;
    spawn.rgba[2] = 0xFF;
    spawn.rgba[3] = 0xFF;
    spawn.width = emitter->xSize;
    spawn.height = emitter->ySize;
    spawn.pos.x = emitter->x;
    spawn.pos.y = emitter->y;
    spawn.pos.z = emitter->z;
    spawn.flags = 0x01EC0009;
    spawn.alpha = 0xFF;
    spawn.colorLevel = 0xFF;
    spawn.primMode = 0;
    spawn.envMode = 0;
    spawn.zone = zone;
    spawn.viewportMask = 0xFF;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    effect = particle_newQuadParticle(&spawn, &gWhiteVtxColors, 0, 0x28, 0, 0x20, 0, 0, 0, sizeof(init), owner, list);
    if (effect != NULL) {
        state = &effect->state;
        memcpy(state, &init, sizeof(init));
        if (playSound) {
            pos.x = emitter->x;
            pos.y = emitter->y + (emitter->ySize >> 1);
            pos.z = emitter->z;
            header.flags = 2;
            header.update = 2;
            header.lifetime = 300;
            header.lightType = 6;
            state->light = light_createJitterAt(&header, &pos, 0xFF, 0x79, 0, 0xFF, 0, 0, 14, owner, list);
        }
    }
    if (playSound) {
        sound_addDynamicSound(0x1AA, emitter->x, emitter->y, emitter->z, 0x4000, 1000, 750, sound_stopWhenSilentCallback, emitter,
                      0, 8, math_random() & 0x300);
    }
}

/* Vertex function 0x20 of the zone emitters' particles (particle_fireRegionEmitter): writes the 4 vertices of a vertical
 * quad rotated by table angle `index` into the vertex buffer `index` (initialised from the template the first time) and
 * returns them, or NULL. */
Vtx *particle_buildYawedVerticalQuad(ZoneEmitter *owner, s16 index) {
    Vtx *vertices;
    Vtx *result;
    f32 dz;
    f32 dx;
    s16 sStart;
    s16 sEnd;
    s32 pad; // matching: unused stack slot in the original frame
    u8 isNew;

    particle_getViewBuffer(owner->head.vtxBuffers, index, sizeof(owner->head.vtxTemplate), &vertices, &isNew);
    result = vertices;
    if (vertices != NULL) {
        if (isNew) {
            memcpy(owner->head.vtxBuffers[index], owner->head.vtxTemplate, sizeof(owner->head.vtxTemplate));
            memcpy(owner->head.vtxBuffers[index] + 4, owner->head.vtxTemplate, sizeof(owner->head.vtxTemplate));
        }
    } else {
        return NULL;
    }
    {
        s16 tMax = 0x7C0;
        sStart = owner->state.texScroll;
        sEnd = owner->state.texScroll + owner->state.texSpan;
        dz = gCameraAngleSines[index] * owner->head.def.width;
        dx = gCameraAngleCosines[index] * owner->head.def.width;
        vertices->v.ob[0] = owner->head.def.pos.x + dx;
        vertices->v.ob[1] = owner->head.def.pos.y;
        vertices->v.ob[2] = owner->head.def.pos.z - dz;
        vertices->v.tc[0] = sStart;
        vertices->v.tc[1] = tMax;
        vertices->v.cn[3] = owner->state.alpha;
        vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = owner->head.def.pos.x - dx;
        vertices->v.ob[1] = owner->head.def.pos.y;
        vertices->v.ob[2] = owner->head.def.pos.z + dz;
        // matching: the comma joins below shift ugen's tree slots onto the heap state particle_fireRegionEmitter leaves
        vertices->v.tc[0] = sStart, vertices->v.tc[1] = 0;
        vertices->v.cn[3] = owner->state.alpha;
        vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = owner->head.def.pos.x - dx;
        vertices->v.ob[1] = owner->head.def.pos.y + owner->head.def.height;
        vertices->v.ob[2] = owner->head.def.pos.z + dz;
        vertices->v.tc[0] = sEnd;
        vertices->v.tc[1] = 0, vertices->v.cn[3] = 0, vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = owner->head.def.pos.x + dx;
        vertices->v.ob[1] = owner->head.def.pos.y + owner->head.def.height;
        vertices->v.ob[2] = owner->head.def.pos.z - dz;
        vertices->v.tc[0] = sEnd;
        vertices->v.tc[1] = tMax;
        vertices->v.cn[3] = 0;
        vertices->v.flag = 0;
    }
    return result;
}

/* Spins the emitter and, while math_isZoneHiddenFromViews allows it, throws particles from random points of its region. */
s32 particle_updateZoneEmitter(ZoneEmitter *obj) {
    ZoneEmitterState *state;

    state = &obj->state;
    if (state->hasZone) {
        mp_addZone(obj->head.def.pos.x, obj->head.def.pos.y, obj->head.def.pos.z, 500.0f);
    }
    state->texScroll += state->texScrollSpeed * gFrameDelta;
    state->texScroll = math_wrapFloat(state->texScroll, 2048.0f, 0.0f);
    state->phase += state->phaseSpeed * gFrameTicks;
    state->alpha = math_cosBinAngleMasked(state->phase - 0x40) * 63.0f + 192.0f;
    if (math_isZoneHiddenFromViews(obj->head.def.zone) == 0) {
        state->pending += (0.3930000067f + math_randomFloat() * 0.4370000064f) * gFrameDelta;
        if (state->pending > 1.0f) {
            ParticleDesc spawn;

            spawn.texture = 40;
            spawn.renderMode = 0x2203;
            spawn.setGeometryMode = 0x200005;
            spawn.clearGeometryMode = 0x1F0600;
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.a = 0xFF;
            spawn.r2 = 0xFF;
            spawn.g2 = 0xFF;
            spawn.offset.x = 0.0f;
            spawn.offset.y = 0.0f;
            spawn.offset.z = 0.0f;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.b2 = 0xFF;
            spawn.fadeStart = 12;
            spawn.fadeRate = 21;
            spawn.scaleStart = 1;
            spawn.scaleRate = 1.0f;
            spawn.flags = 0xC207;
            spawn.primColorMode = 6;
            spawn.envColorMode = 8;
            spawn.updateFunc = -1;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            do {
                math_dirFromBinAngleXZ((u8)math_random(), state->region->xSize * math_randomFloat(), &spawn.pos.x,
                              &spawn.pos.z);
                spawn.pos.x += state->region->x;
                spawn.pos.y = state->region->y;
                spawn.pos.z += state->region->z;
                spawn.timer = (math_random() & 0xF) + 25;
                spawn.alpha = math_random() % 101U + 100;
                spawn.width = spawn.height = math_randomFloat() * 205.0f + 98.0f;
                math_dirFromBinAngles((u8)math_random(), math_random() % 35U - 64, math_randomFloat() * 10.0f + 10.0f,
                              &spawn.vel);
                // matching: `+ -x` (a subtraction emits sub.s instead of add.s)
                spawn.gravity = math_randomFloat() * 0.2840000093f + -0.9100000262f;
                particle_newSpriteParticleWithFloor(&spawn, 1, 0, 0, obj->head.header.owner, obj->head.header.kind);
                state->pending -= 1.0f;
            } while (state->pending > 1.0f);
        }
    }
    return 1;
}

/* Shuts the emitter down: marks its region off, calls sound_stopByCallback with sound_stopWhenSilentCallback on it, and frees its light. */
void particle_shutZoneEmitter(ZoneEmitter *obj) {
    obj->state.region->inactive = 1;
    sound_stopByCallback(sound_stopWhenSilentCallback, obj->state.region, 0);
    if (obj->state.light != NULL) {
        effect_destroyObject(obj->state.light);
    }
}

void effect_destroyZoneEmitter(ZoneEmitter *obj) {
    particle_shutZoneEmitter(obj);
    particle_destroyQuadParticleDefault(&obj->head);
}

void effect_freeZoneEmitter(ZoneEmitter *obj) {
    particle_shutZoneEmitter(obj);
    particle_freeQuadParticleDefault(&obj->head);
}

/* TriggerZone handler: sends message 0x60034 to `target` with its facing angle and the index of the actor
 * of the zone's ZoneEffect (-1 if none). */
void func_151A6B68(Actor *target, TriggerZone *zone) {
    ZoneEffect *obj;
    Actor *actor;
    s32 actorIndex;

    actor = NULL;
    obj = effect_findZoneEffect(zone);
    if (obj != NULL) {
        actor = obj->data.actor;
    }
    if (actor != NULL) {
        actorIndex = actor - movement_obj;
    } else {
        actorIndex = -1;
    }
    actor_hitScripted(target, 0x60034, target->langle, actorIndex);
}

/* Returns the ZoneEffect (callbackSet 0x2C) of `zone` in either object list, among the object types listed
 * in gTimedEffectTypes, or NULL. */
ZoneEffect *effect_findZoneEffect(TriggerZone *zone) {
    u8 i;
    u8 list;
    ZoneEffect *obj;

    for (i = 0; i < 2; i++) {
        for (list = 0; list < 2; list++) {
            for (obj = (ZoneEffect *)gObjectLists[list][gTimedEffectTypes[i]]; obj != NULL;
                 obj = (ZoneEffect *)obj->base.header.next) {
                if (obj->base.callbackSet == 0x2C && zone == obj->data.zone) {
                    return obj;
                }
            }
        // matching: the label starts a new basic block, which keeps IDO from hoisting list++
        next_list:;
        }
    }
    return NULL;
}
