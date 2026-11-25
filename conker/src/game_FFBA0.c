#include <ultra64.h>
// matching: func_150ADA20 returns a u32 random number, but this file was built against a u8-returning
// prototype (callers use the low byte without masking it), so it keeps that view.
#define func_150ADA20 func_150ADA20_functions_h
#include "functions.h"
#undef func_150ADA20
u8 func_150ADA20(void);
#include "variables.h"
#include "effects.h"

/* Object data (at +0x28) of a trigger that spawns spark emitters. */
typedef struct EmitterTrigger {
    s16 *record;
    s32 minDelay;
    s32 delayRange;
    s32 countdown;
    f32 matrix[16];
    u8 params[2]; // [0] bit 0: spawn periodically; [1]: id matched by event 0x34
                  // matching: one array; two u8 fields swap the compare's registers in func_150D32FC
} EmitterTrigger;

/* Object data (at +0x28) of a spark emitter: created by func_150D278C, updated by func_150D2924. */
typedef struct SparkEmitter {
    s16 *record;
    f32 angle;
    f32 rate;
    f32 interval;
    f32 angleStep;
    f32 spawnAccum;
    f32 elapsed;
    f32 matrix[16];
    u8 color[3];
} SparkEmitter; // size 0x60

typedef struct EffectObject {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    union {
        EmitterTrigger trigger;
        SparkEmitter emitter;
    } data;
} EffectObject;

void func_150D278C(s16 *record, f32 *matrix, u8 unkC, s32 unk1);

/* While params[0] bit 0 is set, spawns a spark emitter after each random delay. */
void func_150D26F0(EffectObject *obj) {
    EmitterTrigger *trigger = &obj->data.trigger;

    if (trigger->params[0] & 1) {
        trigger->countdown -= D_800BE9E4;
        if (trigger->countdown < 0) {
            func_150D278C(trigger->record, trigger->matrix, obj->unkC, obj->unk1);
            trigger->countdown = func_150ADA20() % (u32)(trigger->delayRange + 1) + trigger->minDelay;
        }
    }
}

/* Spawns a spark emitter object with a random spin, rate and color. */
void func_150D278C(s16 *record, f32 *matrix, u8 unkC, s32 unk1) {
    SparkEmitter emitter;
    struct260 *object;

    emitter.record = record;
    emitter.angle = func_150ADA68() * 2.0f * 3.1415927f;
    emitter.rate = func_150ADA68() * 0.4f + 0.3f;
    emitter.interval = 1.0f / emitter.rate;
    emitter.angleStep = func_150ADA68() * 0.13000001f + 0.3f;
    if (func_150ADA20() & 1) {
        emitter.angleStep = -emitter.angleStep;
    }
    emitter.spawnAccum = 0.0f;
    emitter.elapsed = 0.0f;
    memcpy(emitter.matrix, matrix, sizeof(emitter.matrix));
    {
        const u8 colors[3][3] = { { 0xFF, 0xFF, 0xFF }, { 0xFF, 0x00, 0x00 }, { 0x00, 0xFF, 0x00 } };
        u8 i = func_150ADA20() % 3U;

        emitter.color[0] = colors[i][0];
        emitter.color[1] = colors[i][1];
        emitter.color[2] = colors[i][2];
    }
    object = func_15149130(func_150ADA20() % 101U + 100, -1, 0x32, -1, 1, 0, sizeof(emitter), unkC, unk1);
    if (object != NULL) {
        memcpy(&((EffectObject *)object)->data, &emitter, sizeof(emitter));
    }
}

/* Motion data (at +0x110) of a spark particle spawned by func_150D2924. */
typedef struct SparkMotion {
    f32 posX;
    f32 posY;
    f32 baseX;
    f32 baseY;
    f32 amplitudeX;
    f32 amplitudeY;
    f32 phaseX;
    f32 phaseY;
    f32 phaseSpeedX;
    f32 phaseSpeedY;
    f32 life;
    f32 fadeScale;
} SparkMotion;

typedef struct SparkParticle {
    u8 pad0[0x5C];
    u8 unk5C; // fades out with the remaining life
    u8 pad5D[0xB3];
    SparkMotion motion;
} SparkParticle;

/* matching: the first 0x54 bytes of ParticleDef; a full ParticleDef on the stack changes the frame. */
typedef struct ParticleDef54 {
    /* 0x00 */ u8 texture;      /* D_80090B60 index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the D_80089F64 handler */
    /* 0x02 */ u16 renderModes; /* low byte: D_800A4AC8 index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ Vec3f pos;
    /* 0x28 */ Vec3f rot;      /* a point instead for func_1513F114 (bend target) and beams (end point) */
    /* 0x34 */ Vec3f scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s16 *unk48;
    /* 0x4C */ u8 viewportMask;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 unk50;
} ParticleDef54;

extern void func_150A7960(f32 *matrix, f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);
extern void func_1514373C(f32 angle, f32 radius, f32 *outX, f32 *outZ);

/*
 * Accumulates the emitter's spawn rate and, for each whole spark due, spawns one on a circle around
 * the emitter's record (rotating by angleStep) with a random sway; sparks fade out 22 time units in.
 */
void func_150D2924(EffectObject *obj) {
    SparkEmitter *emitter;
    s16 *record;

    emitter = &obj->data.emitter;
    emitter->spawnAccum += emitter->rate * D_800BE9A4;
    emitter->elapsed += D_800BE9A4;
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

                func_1514373C(emitter->angle, record[3], &point.x, &point.z);
                point.y = record[4];
                func_150A7960(emitter->matrix, point.x, point.y, point.z, &spawn.rot.x, &spawn.rot.y, &spawn.rot.z);
                spawn.width = func_150ADA68() * 25.0f + 50.0f;
                motion.posX = motion.baseX = func_150ADA68() * 1024.0f + -2560.0f + 2048.0f;
                motion.baseY = func_150ADA68() * 1024.0f + 1536.0f + 2048.0f;
                motion.posY = motion.baseY;
                motion.amplitudeX = func_150ADA68() * 768.0f + 768.0f;
                motion.amplitudeY = func_150ADA68() * 768.0f + 768.0f;
                motion.phaseSpeedX = (func_150ADA68() * 80.0f + 50.0f) * 0.001f;
                motion.phaseSpeedY = (func_150ADA68() * 80.0f + 50.0f) * 0.001f;
                spawn.unk44 = motion.fadeScale * motion.life;
                spark = func_1513D2F0(&spawn, &D_800A4AA0, 0, 0x26, 0, 0x1F, 0, 0, 0, 0x30, obj->unkC, obj->unk1);
                if (spark != NULL) {
                    memcpy(&((SparkParticle *)spark)->motion, &motion, sizeof(motion));
                }
            }
            emitter->angle += emitter->angleStep;
            emitter->elapsed -= emitter->interval;
            emitter->spawnAccum -= 1.0f;
        } while (emitter->spawnAccum > 1.0f);
    }
}

void func_150AD8B0(f32 *a, f32 *b, f32 *out);
Vec3f *func_15144B34(s32 viewport);
void func_151D5D60(Vtx **buffers, s16 index, s32 size, Vtx **out, u8 *fresh);

typedef struct GameFFBA0Beam {
    u8 pad0[0x2C];
    f32 width;
    u8 pad30[4];
    Vec3f start;
    Vec3f end;
    u8 pad4C[0x74];
    Vtx vertexTemplate[4];
    Vtx *buffers[4];
    f32 texture[2];
} GameFFBA0Beam;

/*
 * Builds the beam's camera-facing quad (start to end, `width` to each side) in this frame's
 * vertex buffer, filling a fresh buffer from the template first. Returns the vertices, or NULL.
 */
Vtx *func_150D2D6C(GameFFBA0Beam *beam, s16 index) {
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

    func_151D5D60(beam->buffers, index, sizeof(beam->vertexTemplate), &vertices, &fresh);
    result = vertices;
    // matching: an early `if (vertices == NULL) return NULL;` lays the blocks out differently
    if (vertices != NULL) {
        if (fresh) {
            memcpy(beam->buffers[index], beam->vertexTemplate, sizeof(beam->vertexTemplate));
            memcpy(beam->buffers[index] + 4, beam->vertexTemplate, sizeof(beam->vertexTemplate));
        }
    } else {
        return NULL;
    }
    texture = beam->texture;
    camera = func_15144B34(index);
    delta.x = beam->end.x - beam->start.x;
    delta.y = beam->end.y - beam->start.y;
    delta.z = beam->end.z - beam->start.z;
    end.x = beam->start.x + delta.x;
    end.y = beam->start.y + delta.y;
    end.z = beam->start.z + delta.z;
    middle.x = beam->start.x + delta.x * 0.5f;
    middle.y = beam->start.y + delta.y * 0.5f;
    middle.z = beam->start.z + delta.z * 0.5f;
    view.x = middle.x - camera->x;
    view.y = middle.y - camera->y;
    view.z = middle.z - camera->z;
    func_150AD8B0(&delta.x, &view.x, &side.x);
    lengthSquared = side.x * side.x + side.y * side.y + side.z * side.z;
    if (lengthSquared != 0.0f) {
        f32 scale = beam->width / sqrtf(lengthSquared);

        side.x = side.x * scale;
        side.y = side.y * scale;
        side.z = side.z * scale;
    } else {
        // matching: integer zeros; 0.0f changes the code
        side.x = 0;
        side.y = 0;
        side.z = 0;
    }
    vertices->v.ob[0] = beam->start.x - side.x;
    vertices->v.ob[1] = beam->start.y - side.y;
    vertices->v.ob[2] = beam->start.z - side.z;
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
    vertices->v.ob[0] = beam->start.x + side.x;
    vertices->v.ob[1] = beam->start.y + side.y;
    vertices->v.ob[2] = beam->start.z + side.z;
    vertices->v.tc[0] = texture[1];
    vertices->v.flag = 0;
    return result;
}

/* Sways a spark around its base position and fades it out; returns 0 once its life runs out. */
s32 func_150D317C(SparkParticle *particle) {
    SparkMotion *motion = &particle->motion;

    motion->phaseX += motion->phaseSpeedX * D_800BE9A4;
    motion->phaseY += motion->phaseSpeedY * D_800BE9A4;
    motion->phaseX = func_15144B68(motion->phaseX);
    motion->phaseY = func_15144B68(motion->phaseY);
    motion->posX = sinf(motion->phaseX) * motion->amplitudeX + motion->baseX;
    motion->posY = sinf(motion->phaseY) * motion->amplitudeY + motion->baseY;
    motion->life -= D_800BE9A4;
    particle->unk5C = motion->fadeScale * motion->life;
    if (motion->life > 0.0f) {
        return 1;
    }
    return 0;
}

/* On event 0x34 carrying this trigger's id, spawns a spark emitter immediately. */
void func_150D32FC(EffectObject *obj, u8 *msg, u8 event) {
    EmitterTrigger *trigger = &obj->data.trigger;

    if (event == 0x34 && msg[0] == trigger->params[1]) {
        func_150D278C(trigger->record, trigger->matrix, obj->unkC, obj->unk1);
    }
}
