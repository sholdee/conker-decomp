#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/sound.h"

/* Spawn region an emitter scatters its particles over. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radiusX;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 radiusZ;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 rotation; // degrees
    /* 0x14 */ u8 state;
    /* 0x15 */ u8 flags; // bits 0-1: region shape
} EmitterRegion;

/* Particle emitter state, embedded at offset 0x28 of the effect object. */
typedef struct {
    /* 0x00 */ EmitterRegion *region;
    /* 0x04 */ f32 rate;
    /* 0x08 */ f32 scale;
    /* 0x0C */ f32 pending; // particles owed; one is spawned per whole unit
    /* 0x10 */ f32 maxHeight;
    /* 0x14 */ f32 xScaleBase;
    /* 0x18 */ f32 xScaleRange;
    /* 0x1C */ f32 yScaleBase;
    /* 0x20 */ f32 yScaleRange;
    /* 0x24 */ s16 lifeBase;
    /* 0x26 */ s16 lifeRange;
    /* 0x28 */ f32 scaleBase;
    /* 0x2C */ f32 scaleRange;
    /* 0x30 */ f32 shrinkRateBase;
    /* 0x34 */ f32 shrinkRateRange;
    /* 0x38 */ f32 growRateBase;
    /* 0x3C */ f32 growRateRange;
    /* 0x40 */ u8 useGround;
    /* 0x41 */ u8 alphaBase;
    /* 0x42 */ u8 alphaRange;
    /* 0x43 */ u8 pad43[5];
    /* 0x48 */ CollisionHit ground;
    /* 0x6C */ ZoneNode *zone;
} RegionEmitterState;


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
    if (emitter->region->state == 1) {
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
            switch (emitter->region->flags & 3) {
            case 2: {
                u8 angle;
                f32 cosine;
                f32 sine;
                f32 x;
                f32 z;

                angle = emitter->region->rotation * 0.7111111f;
                cosine = math_cosBinAngleMasked(angle - 0x40);
                sine = math_cosBinAngleMasked(angle);
                x = math_randomFloat() * (2.0f * emitter->region->radiusX) + -emitter->region->radiusX;
                z = math_randomFloat() * (2.0f * emitter->region->radiusZ) + -emitter->region->radiusZ;
                particle.pos.x = emitter->region->x + (x * sine + z * cosine);
                particle.pos.z = emitter->region->z + (z * sine - x * cosine);
                high = emitter->region->y + emitter->region->height;
                low = emitter->region->y - emitter->region->height;
            } break;
            case 0: {
                u8 angle;
                f32 cosine;
                f32 sine;
                f32 x;

                angle = math_random() & 0xFF;
                cosine = math_cosBinAngleMasked(angle - 0x40);
                sine = math_cosBinAngleMasked(angle);
                x = math_randomFloat() * emitter->region->radiusX;
                particle.pos.x = emitter->region->x + x * sine;
                particle.pos.z = emitter->region->z - x * cosine;
                high = emitter->region->y + emitter->region->height;
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
                x = math_randomFloat() * emitter->region->radiusX;
                particle.pos.x = emitter->region->x + x * sine;
                particle.pos.z = emitter->region->z - x * cosine;
                high = emitter->region->y + emitter->region->height;
                low = emitter->region->y - emitter->region->height;
            } break;
            default:
                particle.pos.x = emitter->region->x;
                particle.pos.z = emitter->region->z;
                high = emitter->region->y + emitter->region->height;
                low = emitter->region->y - emitter->region->height;
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

/* Message 0x35: the list of trigger ids that fired. */
typedef struct Game1D3420Message {
    /* 0x00 */ s32 count;
    /* 0x04 */ u8 *ids;
    /* 0x08 */ s32 value;
} Game1D3420Message;

typedef struct Game1D3420Target {
    /* 0x00 */ TriggerZone *emitter; /* inactive is 1 while the effect can still be triggered */
    /* 0x04 */ u8 id;
    /* 0x05 */ u8 pad05[3];
    /* 0x08 */ ZoneNode *zone; /* the effect's spawn zone (ParticleDef.zone) */
    /* 0x0C */ u8 flags; /* bit 0: play sounds, bit 1: copied into the effect's hasZone */
    /* 0x0D */ u8 pad0D[3];
    /* 0x10 */ s32 value;
} Game1D3420Target;

struct Game1D3420Listener {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ Game1D3420Target target;
};

/* Message handler: on message 0x35 whose id list contains this listener's target id, fires the
 * target emitter's effect once and records the message value. */
void particle_handleRegionEmitterMessage(Game1D3420Listener *listener, void *data, u8 type) {
    Game1D3420Message *msg = data;
    s32 i;
    s32 found;
    Game1D3420Target *target;

    if (type != 0x35) {
        return;
    }
    target = &listener->target;
    i = 0;
    found = FALSE;
    while (i < msg->count && !found) {
        if (target->id == msg->ids[i]) {
            found = TRUE;
        } else {
            i++;
        }
    }
    if (found && target->emitter->inactive == 1) {
        particle_fireRegionEmitter(target->emitter, target->zone, target->flags & 1, target->flags & 2,
                      listener->base.header.owner, listener->base.header.kind);
        target->emitter->inactive = 0;
        target->value = msg->value;
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

/* A quad vertex (the Vtx layout). */
struct Game1D2B10QuadVertex {
    s16 x, y, z, flag;
    s16 s, t;
    u8 r, g, b, a;
};

struct Game1D2B10QuadOwner {
    u8 unknown0[0x2C];
    f32 width;
    f32 height;
    f32 x, y, z;
    u8 unknown40[0x80];
    Game1D2B10QuadVertex vertexTemplate[4];
    u8 *buffers[5]; /* each holds two copies of vertexTemplate */
    f32 texScroll; /* texture s of the quad's first edge */
    f32 texScrollSpeed;
    f32 texSpan; /* texture s span */
    u8 unknown120[6];
    u8 alpha;
};

/* Writes the 4 vertices of a vertical quad rotated by table angle `index` into the owner's vertex
 * buffer `index` (initialised from the template the first time) and returns them, or NULL. */
Game1D2B10QuadVertex *particle_buildYawedVerticalQuad(Game1D2B10QuadOwner *owner, s16 index) {
    Game1D2B10QuadVertex *vertices;
    Game1D2B10QuadVertex *result;
    f32 dz;
    f32 dx;
    s16 sStart;
    s16 sEnd;
    s32 pad; // matching: unused stack slot in the original frame
    u8 isNew;

    particle_getViewBuffer(owner->buffers, index, 0x40, &vertices, &isNew);
    result = vertices;
    if (vertices != NULL) {
        if (isNew) {
            memcpy(owner->buffers[index], owner->vertexTemplate, sizeof(owner->vertexTemplate));
            memcpy(owner->buffers[index] + sizeof(owner->vertexTemplate), owner->vertexTemplate,
                   sizeof(owner->vertexTemplate));
        }
    } else {
        return NULL;
    }
    {
        s16 tMax = 0x7C0;
        sStart = owner->texScroll;
        sEnd = owner->texScroll + owner->texSpan;
        dz = gCameraAngleSines[index] * owner->width;
        dx = gCameraAngleCosines[index] * owner->width;
        vertices->x = owner->x + dx;
        vertices->y = owner->y;
        vertices->z = owner->z - dz;
        vertices->s = sStart;
        vertices->t = tMax;
        vertices->a = owner->alpha;
        vertices->flag = 0;
        vertices++;
        vertices->x = owner->x - dx;
        vertices->y = owner->y;
        vertices->z = owner->z + dz;
        // matching: the comma joins below shift ugen's tree slots onto the heap state particle_fireRegionEmitter leaves
        vertices->s = sStart, vertices->t = 0;
        vertices->a = owner->alpha;
        vertices->flag = 0;
        vertices++;
        vertices->x = owner->x - dx;
        vertices->y = owner->y + owner->height;
        vertices->z = owner->z + dz;
        vertices->s = sEnd;
        vertices->t = 0, vertices->a = 0, vertices->flag = 0;
        vertices++;
        vertices->x = owner->x + dx;
        vertices->y = owner->y + owner->height;
        vertices->z = owner->z - dz;
        vertices->s = sEnd;
        vertices->t = tMax;
        vertices->a = 0;
        vertices->flag = 0;
    }
    return result;
}
