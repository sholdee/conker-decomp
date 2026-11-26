#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* While params[0] bit 0 is set, spawns a spark emitter after each random delay. */
void particle_updateSparkSpawner(SparkEffect *obj) {
    EmitterTrigger *trigger = &obj->data.trigger;

    if (trigger->params[0] & 1) {
        trigger->countdown -= D_800BE9E4;
        if (trigger->countdown < 0) {
            particle_spawnSparkEmitter(trigger->record, trigger->matrix, obj->base.header.owner, obj->base.header.kind);
            trigger->countdown = math_randomU8() % (u32)(trigger->delayRange + 1) + trigger->minDelay;
        }
    }
}

/* Spawns a spark emitter object with a random spin, rate and color. */
void particle_spawnSparkEmitter(s16 *record, MtxF matrix, u8 unkC, s32 unk1) {
    RingSparkEmitter emitter;
    TimedEffect *object;

    emitter.record = record;
    emitter.angle = math_randomFloat() * 2.0f * 3.1415927f;
    emitter.rate = math_randomFloat() * 0.4f + 0.3f;
    emitter.interval = 1.0f / emitter.rate;
    emitter.angleStep = math_randomFloat() * 0.13000001f + 0.3f;
    if (math_randomU8() & 1) {
        emitter.angleStep = -emitter.angleStep;
    }
    emitter.spawnAccum = 0.0f;
    emitter.elapsed = 0.0f;
    memcpy(emitter.matrix, matrix, sizeof(emitter.matrix));
    {
        const u8 colors[3][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0x00, 0x00 }, { 0x00, 0xFF, 0x00 } };
        u8 i = math_randomU8() % 3U;

        emitter.color[0] = colors[i][0];
        emitter.color[1] = colors[i][1];
        emitter.color[2] = colors[i][2];
    }
    object = effect_newTimedEffect(math_randomU8() % 101U + 100, -1, 0x32, -1, 1, 0, sizeof(emitter), unkC, unk1);
    if (object != NULL) {
        memcpy(&((SparkEffect *)object)->data, &emitter, sizeof(emitter));
    }
}

/*
 * Accumulates the emitter's spawn rate and, for each whole spark due, spawns one on a circle around
 * the emitter's record (rotating by angleStep) with a random sway; sparks fade out 22 time units in.
 */
void particle_updateSparkEmitter(SparkEffect *obj) {
    RingSparkEmitter *emitter;
    s16 *record;

    emitter = &obj->data.emitter;
    emitter->spawnAccum += emitter->rate * gFrameDelta;
    emitter->elapsed += gFrameDelta;
    if (emitter->spawnAccum > 1.0f) {
        ParticleDef54 spawn;
        void *spark;
        SparkMotion motion;

        record = emitter->record;
        motion.fadeScale = 9.0f;
        motion.phaseX = 0.0f;
        motion.phaseY = 0.0f;
        spawn.texture = 0x59;
        spawn.type = 0;
        spawn.renderModes = 0x5A03;
        spawn.lifetime = 0x16;
        spawn.frame = 0;
        spawn.frameSpeed = 0;
        spawn.rgba[0] = emitter->color[0];
        spawn.rgba[1] = emitter->color[1];
        spawn.rgba[2] = emitter->color[2];
        spawn.rgba[3] = 0xFF;
        spawn.height = 1.0f;
        spawn.pos.x = record[0];
        spawn.pos.y = record[1];
        spawn.pos.z = record[2];
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.flags = 0x026C0000;
        spawn.unk45 = 0xFF;
        spawn.unk46 = 0;
        spawn.unk47 = 6;
        spawn.unk48 = 0;
        spawn.viewportMask = 0xFF;
        do {
            motion.life = 22.0f - emitter->elapsed;
            if (motion.life > 0.0f) {
                Vec3f point;

                math_dirFromAngleXZ(emitter->angle, record[3], &point.x, &point.z);
                point.y = record[4];
                math_transformPoint(emitter->matrix, point.x, point.y, point.z, &spawn.rot.x, &spawn.rot.y, &spawn.rot.z);
                spawn.width = math_randomFloat() * 25.0f + 50.0f;
                motion.posX = motion.baseX = math_randomFloat() * 1024.0f + -2560.0f + 2048.0f;
                motion.baseY = math_randomFloat() * 1024.0f + 1536.0f + 2048.0f;
                motion.posY = motion.baseY;
                motion.amplitudeX = math_randomFloat() * 768.0f + 768.0f;
                motion.amplitudeY = math_randomFloat() * 768.0f + 768.0f;
                motion.phaseSpeedX = (math_randomFloat() * 80.0f + 50.0f) * 0.001f;
                motion.phaseSpeedY = (math_randomFloat() * 80.0f + 50.0f) * 0.001f;
                spawn.unk44 = motion.fadeScale * motion.life;
                spark = particle_newQuadParticle(&spawn, &gWhiteVtxColors, 0, 0x26, 0, 0x1F, 0, 0, 0, 0x30, obj->base.header.owner, obj->base.header.kind);
                if (spark != NULL) {
                    memcpy(&((RingSparkParticle *)spark)->motion, &motion, sizeof(motion));
                }
            }
            emitter->angle += emitter->angleStep;
            emitter->elapsed -= emitter->interval;
            emitter->spawnAccum -= 1.0f;
        } while (emitter->spawnAccum > 1.0f);
    }
}

/*
 * Builds the beam's camera-facing quad (start to end, `width` to each side) in this frame's
 * vertex buffer, filling a fresh buffer from the template first. Returns the vertices, or NULL.
 */
Vtx *particle_buildSparkBeamQuad(GameFFBA0Beam *beam, s16 index) {
    Vtx *vertices;
    Vtx *result;
    Vec3f *camera;
    Vec3f delta;
    Vec3f end;
    Vec3f middle;
    Vec3f view;
    Vec3f side;
    f32 lengthSquared;
    f32 *texture;
    u8 fresh;

    particle_getViewBuffer(beam->head.vtxBuffers, index, sizeof(beam->head.vtxTemplate), &vertices, &fresh);
    result = vertices;
    // matching: an early `if (vertices == NULL) return NULL;` lays the blocks out differently
    if (vertices != NULL) {
        if (fresh) {
            memcpy(beam->head.vtxBuffers[index], beam->head.vtxTemplate, sizeof(beam->head.vtxTemplate));
            memcpy(beam->head.vtxBuffers[index] + 4, beam->head.vtxTemplate, sizeof(beam->head.vtxTemplate));
        }
    } else {
        return NULL;
    }
    texture = beam->texture;
    camera = math_getCameraPos(index);
    delta.x = beam->head.def.rot.x - beam->head.def.pos.x;
    delta.y = beam->head.def.rot.y - beam->head.def.pos.y;
    delta.z = beam->head.def.rot.z - beam->head.def.pos.z;
    end.x = beam->head.def.pos.x + delta.x;
    end.y = beam->head.def.pos.y + delta.y;
    end.z = beam->head.def.pos.z + delta.z;
    middle.x = beam->head.def.pos.x + delta.x * 0.5f;
    middle.y = beam->head.def.pos.y + delta.y * 0.5f;
    middle.z = beam->head.def.pos.z + delta.z * 0.5f;
    view.x = middle.x - camera->x;
    view.y = middle.y - camera->y;
    view.z = middle.z - camera->z;
    math_cross3(&delta.x, &view.x, &side.x);
    lengthSquared = side.x * side.x + side.y * side.y + side.z * side.z;
    if (lengthSquared != 0.0f) {
        f32 scale = beam->head.def.width / sqrtf(lengthSquared);

        side.x = side.x * scale;
        side.y = side.y * scale;
        side.z = side.z * scale;
    } else {
        // matching: integer zeros; 0.0f changes the code
        side.x = 0;
        side.y = 0;
        side.z = 0;
    }
    vertices->v.ob[0] = beam->head.def.pos.x - side.x;
    vertices->v.ob[1] = beam->head.def.pos.y - side.y;
    vertices->v.ob[2] = beam->head.def.pos.z - side.z;
    vertices->v.tc[0] = texture[1];
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = end.x - side.x;
    vertices->v.ob[1] = end.y - side.y;
    vertices->v.ob[2] = end.z - side.z;
    vertices->v.tc[0] = texture[0];
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = end.x + side.x;
    vertices->v.ob[1] = end.y + side.y;
    vertices->v.ob[2] = end.z + side.z;
    vertices->v.tc[0] = texture[0];
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = beam->head.def.pos.x + side.x;
    vertices->v.ob[1] = beam->head.def.pos.y + side.y;
    vertices->v.ob[2] = beam->head.def.pos.z + side.z;
    vertices->v.tc[0] = texture[1];
    vertices->v.flag = 0;
    return result;
}

/* Sways a spark around its base position and fades it out; returns 0 once its life runs out. */
s32 particle_swaySpark(RingSparkParticle *particle) {
    SparkMotion *motion = &particle->motion;

    motion->phaseX += motion->phaseSpeedX * gFrameDelta;
    motion->phaseY += motion->phaseSpeedY * gFrameDelta;
    motion->phaseX = math_wrapRadians(motion->phaseX);
    motion->phaseY = math_wrapRadians(motion->phaseY);
    motion->posX = sinf(motion->phaseX) * motion->amplitudeX + motion->baseX;
    motion->posY = sinf(motion->phaseY) * motion->amplitudeY + motion->baseY;
    motion->life -= gFrameDelta;
    particle->unk5C = motion->fadeScale * motion->life;
    if (motion->life > 0.0f) {
        return 1;
    }
    return 0;
}

/* On event 0x34 carrying this trigger's id, spawns a spark emitter immediately. */
void particle_onSparkTrigger(SparkEffect *obj, u8 *msg, u8 event) {
    EmitterTrigger *trigger = &obj->data.trigger;

    if (event == 0x34 && msg[0] == trigger->params[1]) {
        particle_spawnSparkEmitter(trigger->record, trigger->matrix, obj->base.header.owner, obj->base.header.kind);
    }
}
