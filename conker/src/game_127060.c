#include <ultra64.h>
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/effect.h"
#include "conker/flare.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/particle.h"
#include "conker/sprite.h"
#include "conker/text.h"
#include "conker/weapon.h"
#include "conker/zone.h"

static const Vec3f gDelayedBurstLightOffset = { 0.0f, -8.0f, -85.0f };
static const Vec3f gDelayedBurstPosOffset0 = { 24.0f, -8.0f, -85.0f };
static const Vec3f gDelayedBurstPosOffset1 = { -24.0f, -8.0f, -85.0f };
static const Vec3f gDelayedBurstRotOffset0 = { 30.0f, -70.0f, -85.0f };
static const Vec3f gDelayedBurstRotOffset1 = { -30.0f, -70.0f, -85.0f };
static const Vec3f gBoneImpactOffsets[3] = { { -31.0f, -6.0f, 24.0f }, { 31.0f, -6.0f, 24.0f }, { 1.0f, -51.0f, 53.0f } };

u8 gLineEmitterStarted = 0;

/* Spawns the burst effect on actor: two sparks and two glows that follow its second matrix. */
void effect_spawnDelayedBurst(Actor *actor, u8 owner, s32 kind) {
    BurstEffect *effect;
    BurstData data;
    BurstData *burst;

    data.owner = actor;
    data.ownerId = actor->unique_id;
    data.sparks[0] = NULL;
    data.sparks[1] = NULL;
    data.glows[0] = NULL;
    data.glows[1] = NULL;
    effect = effect_newTimedEffect(70, -1, 0x4D, -1, 1, 0x33, sizeof(data), owner, kind);
    if (effect != NULL) {
        burst = &effect->data;
        memcpy(burst, &data, sizeof(data));
        {
            BurstSparkData spark;
            ParticleDef spawn;

            spark.flags = 0;
            spark.scale = (actor->scale_xz + actor->scale_y) * 0.5f;
            spark.owner = actor;
            spark.ownerId = actor->unique_id;
            spark.widthBase = 15.045272f;
            spark.widthRange = 5.8676558f;
            spark.widthTarget = 12.111444f;
            spark.heightBase = 0.396174f;
            spark.widthTimer = 0.0f;
            spark.heightTimer = 0.0f;
            spark.alphaTimer = 0.0f;
            spawn.width = 0.0f;
            spawn.height = 0.0f;
            spark.alpha = 0.0f;
            spark.beamWidthTarget = 1.0f;
            spark.beamWidth = 1.0f;
            spark.beamLengthTarget = 1.0f;
            spark.beamLength = 1.0f;
            spark.heightRange = 0.1042563f;
            spark.heightRange2 = 0.5197371f;
            spark.heightTarget = 0.34404588f;
            spark.alphaBase = 160.0f;
            spark.alphaRange = 95.0f;
            spark.alphaTarget = 207.5f;
            spark.burst = effect;
            spawn.texture = 0x5F;
            spawn.type = 8;
            spawn.renderModes = 0x2203;
            spawn.lifetime = 300;
            spawn.frame = 0;
            spawn.frameSpeed = 0;
            spawn.rgba[0] = 0xFF;
            spawn.rgba[1] = 0xFF;
            spawn.rgba[2] = 0xFF;
            spawn.rgba[3] = 0xFF;
            spawn.pos = gZeroVec3f;
            spawn.rot = gZeroVec3f;
            spawn.scale.x = 1.0f;
            spawn.scale.y = 1.0f;
            spawn.scale.z = 1.0f;
            spawn.flags = 0x40EC0000;
            spawn.unk44 = 0;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 7;
            spawn.unk48 = 0;
            for (spark.index = 0; spark.index < 2; spark.index++) {
                burst->sparks[spark.index] = particle_newQuadParticle(&spawn, &gWhiteVtxColors, 0, 0x25, 0, 0x1D, ((math_random() & 1) ? 2 : 0) + 1, 0, 0, sizeof(spark), owner, kind);
                if (burst->sparks[spark.index] != NULL) {
                    memcpy(&burst->sparks[spark.index]->data, &spark, sizeof(spark));
                }
            }
        }
        {
            GlowShape shape;
            ParticleDef spawn;
            BurstGlowData glow;

            glow.textureTimer = 0;
            glow.targetScale = 1.0f;
            glow.owner = actor;
            glow.burst = effect;
            glow.ownerId = actor->unique_id;
            shape.alpha0 = 0.7f;
            shape.fadeRate = 1e-06f;
            shape.screenX[0] = -1;
            shape.screenY[0] = -1;
            shape.screenX[1] = -1;
            shape.screenY[1] = -1;
            shape.screenX[2] = -1;
            shape.screenY[2] = -1;
            shape.screenX[3] = -1;
            shape.screenY[3] = -1;
            shape.width1 = 50.3f;
            shape.height1 = 50.3f;
            shape.width0 = 30.9f;
            shape.height0 = 30.9f;
            shape.offset1 = 0.1f;
            shape.offset0 = 0.1f;
            shape.alpha1 = 1.0f;
            shape.attach = 0;
            shape.scale = 1.0f;
            shape.zbufSample[0] = 0;
            shape.zbufSample[1] = 0;
            shape.zbufSample[2] = 0;
            shape.zbufSample[3] = 0;
            shape.visibility[0] = 0;
            shape.visibility[1] = 0;
            shape.visibility[2] = 0;
            shape.visibility[3] = 0;
            shape.type = 15;
            shape.handler = -1;
            spawn.texture = 0x60;
            spawn.type = 3;
            spawn.renderModes = 0x2203;
            spawn.lifetime = 300;
            spawn.frame = 0;
            spawn.frameSpeed = 0;
            spawn.rgba[0] = 0xFF;
            spawn.rgba[1] = 0xFF;
            spawn.rgba[2] = 0xFF;
            spawn.rgba[3] = 0xFF;
            spawn.width = 100.0f;
            spawn.height = 100.0f;
            spawn.pos = gZeroVec3f;
            spawn.rot = gZeroVec3f;
            spawn.scale.x = 1.0f;
            spawn.scale.y = 1.0f;
            spawn.scale.z = 1.0f;
            spawn.flags = 0xCD2002;
            spawn.unk44 = 0xFF;
            spawn.unk45 = 0xFF;
            spawn.unk46 = 0;
            spawn.unk47 = 7;
            spawn.unk48 = 0;
            spawn.viewportMask = 0xFF;
            for (glow.index = 0; glow.index < 2; glow.index++) {
                burst->glows[glow.index] = flare_spawnGlow(&shape, 0x78, &spawn, 0, 0x2A, 0, 0, -1, owner, kind);
                if (burst->glows[glow.index] != NULL) {
                    memcpy(&burst->glows[glow.index]->data, &glow, sizeof(glow));
                }
            }
        }
    }
    {
        RumbleParams event;

        event.flags = 1;
        event.duration = 70;
        event.playerMask = 1;
        event.strength = 8;
        event.updateKind = -1;
        cont_spawnRumble(&event, 0, owner, kind);
        light_createAttached(actor, 1, &gDelayedBurstLightOffset, 3, 70, 40, 0xFF, 0xFF, 0xFF, 0xFF, 0, -1, 0, 0, owner, kind);
    }
}

/*
 * Burst effect update: ends when the owner is gone, keeps the sparks and glows on the owner's
 * second matrix (hiding them while it has none), and shows them once the effect is 10 frames old.
 */
void effect_updateDelayedBurst(BurstEffect *effect) {
    Actor *actor;
    BurstData *burst;

    actor = effect->data.owner;
    burst = &effect->data;
    if ((actor->type == 0) || (actor->unique_id != burst->ownerId)) {
        effect->base.header.state = -1;
        return;
    }
    if (actor->ob_matrix_ptr != NULL) {
        Vec3f positions[2];
        Vec3f directions[2];
        s32 pad; // matching: unused stack slot
        const Vec3f *offsets[4];
        Vec3f *results[4];
        BurstSpark *spark;
        BurstGlow *glow;
        u8 i;

        offsets[0] = &gDelayedBurstPosOffset0;
        offsets[1] = &gDelayedBurstPosOffset1;
        offsets[2] = &gDelayedBurstRotOffset0;
        offsets[3] = &gDelayedBurstRotOffset1;
        results[0] = &positions[0];
        results[1] = &positions[1];
        results[2] = &directions[0];
        results[3] = &directions[1];
        math_transformOffsets(offsets, results, actor->ob_matrix_ptr + 1, 4);
        for (i = 0; i < 2; i++) {
            if (burst->sparks[i] != NULL) {
                spark = burst->sparks[i];
                spark->data.flags |= 1;
                spark->head.def.pos = positions[i];
                spark->head.def.rot = directions[i];
            }
            if (burst->glows[i] != NULL) {
                glow = burst->glows[i];
                glow->head.def.flags |= 2;
                glow->head.def.flags &= ~4;
                glow->head.def.pos = positions[i];
                glow->head.def.rot = directions[i];
            }
        }
    } else {
        BurstSparkData *sparkData;
        u8 i;

        for (i = 0; i < 2; i++) {
            if (burst->sparks[i] != NULL) {
                sparkData = &burst->sparks[i]->data;
                sparkData->flags &= ~1;
            }
            if (burst->glows[i] != NULL) {
                burst->glows[i]->head.def.flags &= ~2;
            }
        }
    }
    {
        u8 i;
        s32 pad; // matching: unused local; removing it reschedules the loop

        for (i = 0; i < 2; i++) {
            if (burst->sparks[i] != NULL) {
                BurstSparkData *sparkData = &burst->sparks[i]->data;

                if (effect->base.header.state < 10) {
                    sparkData->beamWidthTarget = 0.0f;
                    sparkData->beamLengthTarget = 0.0f;
                } else {
                    sparkData->beamWidthTarget = 6.0f;
                    sparkData->beamLengthTarget = 15.0f;
                }
            }
            if (burst->glows[i] != NULL) {
                BurstGlowData *glowData = &burst->glows[i]->data;

                if (effect->base.header.state < 10) {
                    glowData->targetScale = 0.0f;
                } else {
                    glowData->targetScale = 1.306000113f;
                }
            }
        }
    }
}

/* Event handler: events 0x4C-0x4F set the tick callback (30 or none); 0x4C and 0x4F also call light_setAmbient (all 0 / all 0xFF). */
void effect_handleLightSwitchEvent(TimedEffect *effect, s32 arg1, u8 event) {
    if (event == 0x4C) {
        effect->tickCallback = -1;
        light_setAmbient(0, 0, 0, 0xFF);
    } else if (event == 0x4D) {
        effect->tickCallback = 30;
    } else if (event == 0x4E) {
        effect->tickCallback = -1;
    } else if (event == 0x4F) {
        effect->tickCallback = -1;
        light_setAmbient(0xFF, 0xFF, 0xFF, 0xFF);
    }
}

/* A particle created by particle_newSpriteParticleWithFloor; this caller's data lives at 0xA8. */
typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ f32 data[5];
} ImpactParticle;


/*
 * Spawns an impact at offset gBoneImpactOffsets[point] in actor's sixth matrix: two particle_spawnSplashSprayAtAngles /
 * particle_spawnDropSprayAtAngles bursts, a particle with a random rise, and weapon_spawnImpactBurst at the point.
 */
void effect_spawnBonePointImpact(Actor *actor, u8 point, u8 owner, s32 kind) {
    Vec3f pos;
    CollisionHit ground;

    if (actor->ob_matrix_ptr) {
        math_transformOffset(&gBoneImpactOffsets[point], &pos, &actor->ob_matrix_ptr[5]);
        collision_initHitFromObject(&ground, actor);
        {
            SplashSpraySpec first;

            first.spray.pos = pos;
            first.spray.scaleBase = 4.0f;
            first.spray.countBase = 9;
            first.spray.countRange = 6;
            first.angles.yawRange = 0xFF;
            first.angles.pitchBase = -40;
            first.angles.yawBase = 0;
            first.angles.pitchRange = 0x32;
            first.spray.itemCountBase = 3;
            first.spray.itemCountRange = 1;
            first.spray.lifeBase = 0x1E;
            first.spray.lifeRange = 0x32;
            first.spray.alphaBase = 0x64;
            first.spray.alphaRange = 0x64;
            first.spray.fadeStart = 0x10;
            first.spray.fadeRate = 0xF;
            first.spray.material = 0;
            first.spray.kind = 1;
            first.spray.scaleRange = 3.879999876f;
            first.spray.sizeBase = 0.694f;
            first.spray.sizeRange = 0.4500000179f;
            first.spray.speedBase = 12.0f;
            first.spray.speedRange = 13.90000057f;
            first.spray.burstChance = 1.0f;
            particle_spawnSplashSprayAtAngles(&first.angles, &first.spray, &ground, owner, kind);
        }
        {
            DropSpraySpec second;

            second.spray.pos = pos;
            second.spray.countBase = 7;
            second.spray.countRange = 6;
            second.angles.yawBase = 0;
            second.angles.yawRange = 0xFF;
            second.angles.pitchBase = -63;
            second.angles.pitchRange = 0x18;
            second.spray.lifeBase = 0x23;
            second.spray.lifeRange = 0xB;
            second.spray.alphaBase = 0x64;
            second.spray.alphaRange = 0x64;
            second.spray.unk30 = 1;
            second.spray.kind = 1;
            second.spray.wobbles = 1;
            second.spray.speedBase = 8.0f;
            second.spray.speedRange = 10.0f;
            second.spray.gravityBase = -1.266f;
            second.spray.gravityRange = 0.4660000205f;
            second.spray.sizeBase = 123.0f;
            second.spray.sizeRange = 104.0f;
            second.spray.unk34 = 1.0f;
            second.spray.bounceChance = 0.3f;
            particle_spawnDropSprayAtAngles(&second.angles, &second.spray, &ground, owner, kind);
        }
        {
            ParticleDesc spawn;
            f32 data[5];
            ImpactParticle *particle;

            data[0] = 0.0f;
            data[2] = 1369.429932f;
            data[1] = math_randomFloat() * 3.0f + 10.0f;
            data[3] = math_randomFloat() * 100.0f + 100.0f;
            data[4] = data[3] / (data[1] * data[1]);
            spawn.alpha = data[3];
            spawn.texture = 0x79;
            spawn.renderMode = 0x4403;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.r2 = 0xDC;
            spawn.g2 = 0xDC;
            spawn.b2 = 0xFF;
            spawn.a = 0xFF;
            spawn.r = 0xFF;
            spawn.g = 0xFF;
            spawn.b = 0xFF;
            spawn.brightness = 0xFF;
            spawn.pos = pos;
            spawn.offset = gZeroVec3f;
            spawn.fadeStart = 1;
            spawn.fadeRate = 0xFF;
            spawn.scaleStart = 1;
            spawn.scaleRate = 1.0f;
            spawn.flags = 0x4C000 | ((math_random() & 1) ? 0x40 : 0) | ((math_random() & 1) ? 0x80 : 0);
            spawn.primColorMode = 6;
            spawn.envColorMode = 6;
            spawn.updateFunc = 0x27;
            spawn.deathFunc = -1;
            spawn.drawFunc = -1;
            spawn.handlerSet = 0;
            spawn.viewMask = 0;
            spawn.views = 0xFF;
            spawn.vel = gZeroVec3f;
            spawn.timer = 300;
            spawn.width = 0.0f;
            spawn.height = 0.0f;
            spawn.gravity = 0.0f;
            particle = particle_newSpriteParticleWithFloor(&spawn, 1, 0, sizeof(data), owner, kind);
            if (particle != NULL) {
                memcpy(particle->data, data, sizeof(data));
            }
        }
        weapon_spawnImpactBurst(&pos, owner, kind);
    }
}

/* Data of the emitter effect started by effect_startLineEmitter and updated by effect_updateLineEmitter. */
typedef struct {
    /* 0x00 */ TriggerZone *from; /* endpoints, resolved with math_randomPointInZone */
    /* 0x04 */ TriggerZone *to;
    /* 0x08 */ f32 pending; /* fractional count of spawns due */
} LineEmitterData;

/* Timed effect of effect_updateLineEmitter. */
struct LineEmitter {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ LineEmitterData data;
};

/* Starts the (single) emitter effect between zones records 35 and 34. */
void effect_startLineEmitter(u8 owner, s32 kind) {
    LineEmitterData data;
    LineEmitter *effect;

    if (gLineEmitterStarted == 0) {
        data.from = &zones[35];
        data.to = &zones[34];
        data.pending = 0.0f;

        effect = effect_newTimedEffect(300, -1, 0x57, -1, 0, 0x46, sizeof(data), owner, kind);
        if (effect != NULL) {
            gLineEmitterStarted = 1;
        }
        if (effect != NULL) { // matching: a second test; one merged if mismatches
            memcpy(&effect->data, &data, sizeof(data));
        }
    }
}

/* Emitter update: spawns particle_spawnSpiral effects between the two endpoints, 0.08-0.26 per frame (times gFrameDelta). */
void effect_updateLineEmitter(LineEmitter *effect) {
    LineEmitterData *data;
    Vec3f from;
    Vec3f to;
    u8 i;
    f32 duration;
    VariantList *variants;

    data = &effect->data;
    data->pending += (0.08000000566f + math_randomFloat() * 0.18f) * gFrameDelta;
    while (data->pending > 1.0f) {
        math_randomPointInZone(data->from, &from);
        math_randomPointInZone(data->to, &to);
        variants = &gSpiralVariantLists[1];
        i = math_random() % variants->count;
        duration = math_randomFloat() * 25.0f + 30.0f;
        // matching: the "* 1" is kept; a plain random + 1.0f schedules differently
        particle_spawnSpiral(&from, &to, variants->ids[i], duration, 1.0f / duration,
                      variants->scales[i] * (math_randomFloat() * 1 + 1), 1, math_random() % 3 + 750,
                      effect->base.header.owner, effect->base.header.kind);
        data->pending -= 1.0f;
    }
}

/* Event handler: events 0x4E and 0x4F call effect_destroyObject on the object. */
void func_150FACE4(EffectHeader *obj, s32 arg1, u8 event) {
    if ((event == 0x4E) || (event == 0x4F)) {
        effect_destroyObject(obj);
    }
}

/* Calls light_setGroupDisabled for 8 and 0xB, then effect_sendEventToTimedEffects with zones record 25 (0x30) and 0x4D. */
void effect_turnLightsOn(void) {
    light_setGroupDisabled(8, 0);
    light_setGroupDisabled(0xB, 1);
    effect_sendEventToTimedEffects((s32)&zones[25], 0x30);
    effect_sendEventToTimedEffects(0, 0x4D);
}

/* Counterpart of effect_turnLightsOn: light_setGroupDisabled for 8 and 7, effect_sendEventToTimedEffects with record 25 (0x31) and 0x4C. */
void effect_turnLightsOff(void) {
    light_setGroupDisabled(8, 1);
    light_setGroupDisabled(7, 0);
    effect_sendEventToTimedEffects((s32)&zones[25], 0x31);
    effect_sendEventToTimedEffects(0, 0x4C);
}

struct Obj150FADC8 {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ s32 flags;
};

/* Event handler: event 0x53 sets flag 2 of the object, event 0x54 clears it. */
void func_150FADC8(Obj150FADC8 *obj, s32 arg1, u8 event) {
    if (event == 0x53) {
        obj->flags |= 2;
    } else if (event == 0x54) {
        obj->flags &= ~2;
    }
}

/* Objects created by effect_newModelEffect / sprite_newScreenSprite; this caller stores a tag in each. */
typedef struct {
    /* 0x000 */ u8 pad0[0x120];
    /* 0x120 */ s16 tag;
} TaggedModel;

typedef struct {
    /* 0x00 */ u8 pad0[0x70];
    /* 0x70 */ s16 tag;
} TaggedSprite;

/*
 * Spawns a effect_newModelEffect object and a row of 8 sprite_newScreenSprite sprites (the first with kind 0xCE,
 * the rest 0xC5/0xC4), each tagged with duration in its data.
 */
void effect_spawnScreenCash(s16 duration, u8 owner, s32 kind) {
    s16 pad; // matching: unused stack slot
    s16 tag;

    tag = duration;
    {
        EffectParams model;
        void *spawned; // matching: one variable for both spawned objects (separate ones reallocate)

        model.unk38 = 0;
        model.flags = 0x61;
        model.updateFunc = 3;
        model.updateFunc2 = 4;
        model.preDrawFunc = 2;
        model.postDrawFunc = 0;
        model.lifetime = duration;
        model.model = 0xA5;
        model.unkC = 0x17;
        model.render.otherModeH = 0;
        model.render.setGeometryMode = 0x200405;
        model.render.clearGeometryMode = 0x60200;
        model.render.envMode = 8;
        model.render.renderMode = 1;
        model.render.combiner = 0x38;
        model.render.primMode = 0;
        model.render.colorDither = 0x80;
        model.render.alphaDither = 0x20;
        model.color0.red = 0xFF;
        model.color0.green = 0xFF;
        model.color0.blue = 0xFF;
        model.color0.alpha = 0xFF;
        model.color1.red = 0xFF;
        model.color1.green = 0xFF;
        model.color1.blue = 0xFF;
        model.color1.alpha = 0xFF;
        model.owner = 0;
        model.ownerId = 0;
        model.unk41 = 1;
        model.pos = gZeroVec3f;
        model.fadeStart = 10;
        model.fadeRate = 25;
        spawned = effect_newModelEffect(&model, 0, 1.0, 0, 0, 2, owner, kind);
        if (spawned != NULL) {
            memcpy(&((TaggedModel *)spawned)->tag, &tag, sizeof(tag));
        }
        {
            ScreenSpriteInfo sprite;
            u8 i;
            const s16 kinds[8] = { 0xCE, 0xC5, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4, 0xC4 };

            sprite.y = 62.0f;
            sprite.timer = duration;
            sprite.flags = 0x29;
            sprite.fadeStart = 10;
            sprite.fadeRate = 25;
            sprite.halfWidth = 16.0f;
            sprite.halfHeight = 16.0f;
            sprite.kind = 0;
            sprite.color1.red = 0xFF;
            sprite.color1.green = 0xFF;
            sprite.color1.blue = 0xFF;
            sprite.color1.alpha = 0xFF;
            sprite.color2.red = 0xFF;
            sprite.color2.green = 0xFF;
            sprite.color2.blue = 0xFF;
            sprite.color2.alpha = 0xFF;
            sprite.colorLevel = 0xFF;
            sprite.render.otherModeH = 0;
            sprite.render.setGeometryMode = 0x200405;
            sprite.render.clearGeometryMode = 0x60200;
            sprite.render.primMode = 0;
            sprite.render.envMode = 7;
            sprite.render.renderMode = 25;
            sprite.render.combiner = 0x22;
            sprite.render.colorDither = 0x80;
            sprite.render.alphaDither = 0x20;
            sprite.viewport = 0;
            sprite.scaleX = 1.0f;
            sprite.scaleY = 1.0f;
            sprite.offsetS = 0.0f;
            sprite.offsetT = 0.0f;
            for (i = 0; i < 8; i++) {
                sprite.x = -65.0f + i * 24.75f;
                sprite.texture = kinds[i];
                spawned = sprite_newScreenSprite(&sprite, 14, 0, 0, 2, owner, kind);
                if (spawned != NULL) {
                    memcpy(&((TaggedSprite *)spawned)->tag, &tag, sizeof(tag));
                }
            }
        }
    }
}

/* Update callback (gModelEffectUpdate2FuncsTail[2]): places the model at (-95, -80, 0), unrotated, scaled by 0.3. */
s32 effect_placeScreenCashModel(ModelEffect *fx) {
    f32 xform[5];

    fx->params.pos.z = 0.0f;
    fx->params.pos.x = -95.0f;
    fx->params.pos.y = -80.0f;
    xform[0] = 0.0f;
    xform[1] = 0.0f;
    xform[2] = 0.0f;
    xform[3] = 0.3f;
    xform[4] = 0.3f;
    effect_setModelEffectRootXform(fx, xform);
    return 1;
}

/* Passes text_preDrawCashModel's result, with the same remaining arguments, to effect_preDrawModelEffectOrtho. */
Gfx *effect_preDrawScreenCashModel(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed) {
    return effect_preDrawModelEffectOrtho(text_preDrawCashModel(gfx, fx, view, needSync, proceed), fx, view, needSync, proceed);
}

/* Layout of Effect (game/Effects/ModelEffect/modeleffect.c), as far as effect_updateFadeInEffect uses it. */
struct FadeInEffect {
    /* 0x000 */ u8 pad0[0x16];
    /* 0x016 */ s16 lifetime;  /* EffectParams.lifetime, counts down */
    /* 0x018 */ u8 pad18[0x2B];
    /* 0x043 */ u8 alpha;      /* EffectParams.color0 alpha */
    /* 0x044 */ u8 pad44[0x20];
    /* 0x064 */ s16 fadeStart; /* EffectParams.fadeStart */
    /* 0x066 */ s16 fadeRate;  /* EffectParams.fadeRate */
    /* 0x068 */ u8 pad68[0xB8];
    /* 0x120 */ s16 duration;  /* Effect.extra: the lifetime's starting value */
};

/*
 * Fades `alpha` in: while `timer` (counting down from `duration`) is within its first `fadeLength`
 * ticks, alpha ramps up by `fadeRate` per elapsed tick; after that it is fully opaque.
 */
void effect_fadeInAlpha(u8 *alpha, s16 timer, s16 duration, s16 fadeLength, s16 fadeRate) {
    if (duration - fadeLength < timer) {
        *alpha = (duration - timer) * fadeRate;
    } else {
        *alpha = 255;
    }
}

/* gModelEffectUpdateFuncs effect update callback: fades the effect's alpha in over its first fadeStart ticks. */
s32 effect_updateFadeInEffect(FadeInEffect *effect) {
    effect_fadeInAlpha(&effect->alpha, effect->lifetime, effect->duration, effect->fadeStart, effect->fadeRate);
    return 1;
}

/* Layout of ScreenSprite (game/Sprite/ScreenSprite/screensprite.c), as far as effect_updateFadeInSprite uses it. */
struct FadeInSprite {
    /* 0x00 */ u8 pad0[0x22];
    /* 0x22 */ s16 timer;     /* ScreenSpriteInfo.timer, counts down */
    /* 0x24 */ u8 pad24[2];
    /* 0x26 */ s16 fadeStart; /* ScreenSpriteInfo.fadeStart */
    /* 0x28 */ s16 fadeRate;  /* ScreenSpriteInfo.fadeRate */
    /* 0x2A */ u8 pad2A[4];
    /* 0x2E */ u8 alpha;      /* ScreenSpriteInfo.color1.alpha */
    /* 0x2F */ u8 pad2F[0x41];
    /* 0x70 */ s16 duration;  /* the timer's starting value */
};

/* gScreenSpriteUpdates sprite callback: fades the sprite's alpha in over its first fadeStart ticks. */
s32 effect_updateFadeInSprite(FadeInSprite *sprite) {
    effect_fadeInAlpha(&sprite->alpha, sprite->timer, sprite->duration, sprite->fadeStart, sprite->fadeRate);
    return 1;
}

/* Spawns the weapon_spawnBoneBeam / particle_spawnBoneDirParticle effects on bone 14 of `actor`: point0 goes to `position`,
 * point2 to `origin` and point3 to `origin + delta`, all in world space. */
void func_150FB324(Actor *actor, u8 arg1, s32 arg2) {
    Vec3f position;
    Vec3f origin;
    Vec3f delta;
    void *mtx;
    s32 pad; // matching: unused stack slot below mtx (removing it shifts the frame)
    const Vec3f point0 = { -108.0f, -9.0f, 85.0f };
    const Vec3f point1 = { -150.0f, -9.0f, 111.0f };
    const Vec3f point2 = { -32.0f, 4.0f, 40.0f };
    const Vec3f point3 = { -21.0f, 15.0f, 40.0f };
    const Vec3f *sources[3];
    Vec3f *destinations[3];

    if (actor->ob_matrix_ptr == NULL) {
        return;
    }
    sources[0] = &point0;
    sources[1] = &point2;
    sources[2] = &point3;
    destinations[0] = &position;
    destinations[1] = &origin;
    destinations[2] = &delta;
    mtx = actor->ob_matrix_ptr + 14;
    math_transformOffsets(sources, destinations, mtx, 3);
    delta.x -= origin.x;
    delta.y -= origin.y;
    delta.z -= origin.z;
    {
        u8 bone = 14;

        weapon_spawnBoneBeam(actor, bone, &point0, &point1, &position, arg1, arg2);
    }
    particle_spawnBoneDirParticle(&origin, &delta, mtx, actor, 1.0f, arg1, arg2);
}
