#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "libc/string.h"

/* Emitter effect: while its owner is alive, spawns effect 0x5A at random points on its shape. */
void particle_updateShapeEmitter(EmitterEffect *effect) {
    ShapeEmitter *emitter;
    EmittedPoint point;
    f32 upper;

    if (math_isZoneHiddenFromViews(effect->data.emitter.zone) == 0) {
        emitter = &effect->data.emitter;
        emitter->accumulator += (9.04e-08f + math_randomFloat() * 9.87e-08f) * emitter->rate * gFrameDelta;
        if (emitter->accumulator > 1.0f) {
            point.w = 0.0f;
            do {
                EmitterEffect *spawned;

                math_randomZoneFootprintPoint(emitter->shape, &point.x, &point.z, &upper, &point.y);
                spawned = effect_newTimedEffect(math_random() % 131U + 80, -1, 0x5A, -1, TRUE, 0,
                                                  sizeof(point), effect->base.header.owner, effect->base.header.kind);
                if (spawned != NULL) {
                    memcpy(&spawned->data, &point, sizeof(point));
                }
                emitter->accumulator -= 1.0f;
            } while (emitter->accumulator > 1.0f);
        }
    }
}

/* Particle source effect: emits particles from its position at a random rate. */
void particle_updateSourceEmitter(EmitterEffect *effect) {
    ParticleSourceData *source;

    source = &effect->data.source;
    source->accumulator += (1352.0f + math_randomFloat() * 1698.0f) * 0.0001f * gFrameDelta;
    if (source->accumulator > 1.0f) {
        ParticleDesc spawn;
        WobbleData data;
        SpriteParticle *particle;

        data.drag = 0.9704700112f;
        spawn.texture = 0x6C;
        spawn.renderMode = 0x5103;
        spawn.unk0 = 0x200005;
        spawn.fadeStart = 70;
        spawn.fadeRate = 3;
        spawn.flags = 0x80DE07;
        spawn.primColorMode = 8;
        spawn.envColorMode = 6;
        data.phase[0] = 0;
        data.phase[1] = 0;
        spawn.unk4 = 0;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.updateFunc = 22;
        spawn.deathFunc = -1;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.scaleStart = 70;
        spawn.r = 0x47;
        spawn.g = 0xC2;
        spawn.b = 0;
        spawn.a = 0xFF;
        spawn.r2 = 0xC0;
        spawn.g2 = 0x49;
        spawn.b2 = 0;
        spawn.brightness = 0xFF;
        spawn.scaleRate = 1.021028996f;
        spawn.pos = source->pos;
        spawn.offset.x = 0.0f;
        spawn.offset.y = 0.0f;
        spawn.offset.z = 0.0f;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        do {
            data.phaseStep[0] = math_random() % 5U + 4;
            data.phaseStep[1] = math_random() % 5U + 4;
            data.amplitude[0] = math_randomFloat() * 11.0f;
            data.amplitude[1] = math_randomFloat() * 11.0f;
            spawn.gravity = math_randomFloat() * 0.172f + 0.09800000489f;
            spawn.flags &= ~0xC0;
            spawn.flags |= ((math_random() & 1) ? 0x40 : 0) | ((math_random() & 1) ? 0x80 : 0);
            spawn.alpha = math_random() % 101U + 100;
            spawn.timer = math_random() % 51U + 80;
            spawn.width = spawn.height = math_randomFloat() * 80.0f + 92.0f;
            particle = particle_newSpriteParticleWithFloor(&spawn, 0, 0, 0x10, effect->base.header.owner, effect->base.header.kind);
            if (particle != NULL) {
                memcpy(&particle->data.wobble, &data, sizeof(data));
            }
            source->accumulator -= 1.0f;
        } while (source->accumulator > 1.0f);
    }
}

/* Attaches a fog flash effect to obj: flashes it with fog colour (r, g, b, a) each frame,
 * for duration frames or, when duration is -1, until obj goes away. */
void particle_attachFogFlash(Actor *obj, u8 r, u8 g, u8 b, u8 a, u8 rate, s16 duration, u8 owner, s32 kind) {
    FogFlashData data;
    EmitterEffect *effect;

    data.owner = obj;
    data.ownerId = obj->unique_id;
    data.a = a;
    data.rate = rate;
    data.r = r;
    data.g = g;
    data.b = b;

    effect = effect_newTimedEffect((duration == -1) ? 300 : duration, -1, 0x5D, -1, (duration == -1) ? FALSE : TRUE,
                                     0x45, sizeof(data), owner, kind);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Effect callback for the fog flash effect: hands effect_handleOwnerActorEvent the owner reference. */
void particle_onFogFlashEvent(EmitterEffect *effect, s32 arg1, u8 arg2) {
    effect_handleOwnerActorEvent(arg1, arg2, &effect->data.fog.owner, &effect->data.fog.ownerId, effect);
}

/* Fog flash effect: re-flashes the owner's fog each frame, and ends once the owner is gone. */
void particle_updateFogFlash(EmitterEffect *effect) {
    FogFlashData *fog = &effect->data.fog;
    Actor *owner = fog->owner;

    if ((owner->type == 0) || (owner->unique_id != fog->ownerId)) {
        effect->base.header.state = -1;
    } else {
        actor_flashFog(owner, fog->r, fog->g, fog->b, fog->a, FALSE, fog->rate);
    }
}

/* Runs sight_spawnFloorImpact on obj and flashes it green. */
void func_150ECC00(Actor *obj, u8 owner, s32 kind) {
    sight_spawnFloorImpact(obj, owner, kind);
    particle_attachFogFlash(obj, 0, 0xFF, 0, 0xFF, 4, -1, owner, kind);
}

/* Current colour (r, g, b), its target and its easing velocity. */
static f32 gEasedColorR = 255.0f;
static f32 gEasedColorG = 255.0f;
static f32 gEasedColorB = 255.0f;
static f32 gEasedColorTargetR = 255.0f;
static f32 gEasedColorTargetG = 255.0f;
static f32 gEasedColorTargetB = 255.0f;
static f32 gEasedColorVelR = 0.0f;
static f32 gEasedColorVelG = 0.0f;
static f32 gEasedColorVelB = 0.0f;

/* Per-frame handler driven by setting 0x403B: while it is on, picks a target colour (one channel
 * at 128 per settings 0x4038-0x403A) and applies settings 0x9C/0x9A of mode 0x2007; otherwise
 * resets the colour to white. Then eases the current colour toward the target and sends it out. */
void func_150ECC70(Camera *obj) {
    s32 value9C;
    s32 value9A;
    s32 half;

    if (script_query(1, 0x403B, 6, 0x2000) != 0) {
        s32 mode;

        script_command(0, 0x4031, 1);
        script_command(0, 0x4032, 1);
        script_command(0, 0x4033, 1);
        obj->AvailableModes |= 0x80021200;
        if (script_query(1, 0x4038, 6, 0x2000) != 0) {
            gEasedColorTargetR = 128.0f;
            gEasedColorTargetG = 255.0f;
            gEasedColorTargetB = 255.0f;
        } else if (script_query(1, 0x4039, 6, 0x2000) != 0) {
            gEasedColorTargetR = 255.0f;
            gEasedColorTargetG = 128.0f;
            gEasedColorTargetB = 255.0f;
        } else if (script_query(1, 0x403A, 6, 0x2000) != 0) {
            gEasedColorTargetR = 255.0f;
            gEasedColorTargetG = 255.0f;
            gEasedColorTargetB = 128.0f;
        } else {
            gEasedColorTargetR = 255.0f;
            gEasedColorTargetG = 255.0f;
            gEasedColorTargetB = 255.0f;
        }
        mode = script_query(0, 0x2007, 0xB7) | 0x2000;
        if (script_query(1, 0x4010, 6, 0x2000) == 0) {
            obj->Tightness = 0;
            obj->YOffsetTget = 0.0f;
            return;
        }
        // matching: mode always has 0x2000 set, but IDO still emits this test
        if (mode == 0) {
            return;
        }
        obj->Tightness = 1;
        value9C = script_query(1, mode, 0x9C, 0x2000);
        script_command(1, 0x9000, 0x17, value9C);
        value9A = script_query(1, mode, 0x9A, 0x2000);
        half = value9A >> 1;
        if (value9A < 900) {
            value9A = 900;
        } else if (value9A > 1190) {
            value9A = 1190;
        }
        if (half < 180) {
            half = 180;
        } else if (half > 231) {
            half = 231;
        }
        script_command(2, 0x9000, 8, value9A, half);
        obj->YOffsetTget = 220.0f;
    } else {
        gEasedColorTargetR = 255.0f;
        gEasedColorTargetG = 255.0f;
        gEasedColorTargetB = 255.0f;
        script_command(0, 0x4031, 0);
        script_command(0, 0x4032, 0);
        script_command(0, 0x4033, 0);
        obj->AvailableModes &= ~0x80021200;
        obj->AvailableModes |= 8;
    }
    math_springTo(&gEasedColorR, gEasedColorTargetR, &gEasedColorVelR, 4.0f, 6.0f, obj->SpringTime);
    math_springTo(&gEasedColorG, gEasedColorTargetG, &gEasedColorVelG, 4.0f, 6.0f, obj->SpringTime);
    math_springTo(&gEasedColorB, gEasedColorTargetB, &gEasedColorVelB, 4.0f, 6.0f, obj->SpringTime);
    script_command(1, 0x30F3, 0x12, (u8)gEasedColorR);
    script_command(1, 0x30F4, 0x12, (u8)gEasedColorG);
    script_command(1, 0x30F5, 0x12, (u8)gEasedColorB);
}
