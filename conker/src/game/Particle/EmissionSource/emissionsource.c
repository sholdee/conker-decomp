#include <ultra64.h>
#include <math.h>
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/light.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "conker/sprite.h"
#include "libc/string.h"



typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ u8 data[1]; // effect-specific data
} Game113D60AnyEffect;

typedef struct Game113D60Source {
    f32 chance;
    s8 start;
    s8 end;
    s8 motion;
    u8 pad7;
    s16 duration;
    s16 durationRange;
    u8 color;
    u8 padD[3];
} Game113D60Source;

typedef void (*Game113D60EndCallback)(Vec3f *, Actor *);
typedef void (*Game113D60StartCallback)(Vec3f *);
typedef void (*Game113D60MotionCallback)(Vec3f *, Vec3f *, Vec3f *, Game113D60Emission *);

/* The start, end and motion callbacks of a source (Game113D60Source.start, .end, .motion). */
static Game113D60StartCallback gEmissionStartFuncs[] = { particle_randomPointOnTwoSegments, particle_randomPointInSourceZones, particle_randomPointOnSixSegments };
static Game113D60EndCallback gEmissionEndFuncs[] = { particle_randomPointAroundActorNode, particle_getEmissionActorPos };
static Game113D60MotionCallback gEmissionMotionFuncs[] = { particle_aimEmissionAlongDelta, particle_aimEmissionTilted };

/* The emission sources (particle_startSourceEmission's sourceIndex). */
static const Game113D60Source gEmissionSources[] = {
    { 1.0f, -1, -1, -1, 0, 300, 0, 0 },
    { 0.1f, 0, 0, 0, 0, 120, 60, 0 },
    { 0.1f, 1, 0, 0, 0, 80, 40, 1 },
    { 0.6f, 2, 1, 1, 0, 100, 40, 0 },
};

/* Randomly starts an emission of the source effect `sourceIndex` from one of the start
 * points towards `actor`. */
void particle_startSourceEmission(Actor *actor, s32 arg1, TriggerZone *zone, u8 sourceIndex) {
    const Game113D60Source *source;
    Vec3f end;
    Vec3f start;
    Vec3f delta;
    Game113D60Emission emission;
    Game113D60Effect *effect;

    if ((actor != NULL) && ((gCutsceneStates[0] != 1) || (gCutsceneIds[0] != 9))) {
        source = &gEmissionSources[sourceIndex];
        if ((math_randomFloat() < source->chance) && (source->start != -1)) {
            if (source->end != -1) {
                gEmissionEndFuncs[source->end](&end, actor);
            } else {
                end.x = 0.0f;
                end.y = 0.0f;
                end.z = 0.0f;
            }
            if (source->start != -1) {
                gEmissionStartFuncs[source->start](&start);
            } else {
                start.x = 0.0f;
                start.y = 0.0f;
                start.z = 0.0f;
            }
            delta.x = end.x - start.x;
            delta.y = end.y - start.y;
            delta.z = end.z - start.z;
            if ((delta.x != 0) || (delta.z != 0)) {
                if (source->motion != -1) {
                    gEmissionMotionFuncs[source->motion](&start, &end, &delta, &emission);
                } else {
                    emission.stream.angleX = 0.0f;
                    emission.stream.amplitudeX = 0.0f;
                    emission.stream.phaseX = 0.0f;
                    emission.stream.angleY = 0.0f;
                    emission.stream.amplitudeY = 0.0f;
                    emission.stream.phaseY = 0.0f;
                }
                emission.stream.rate = math_randomFloat() * 0.150000006f + 0.200000003f;
                emission.stream.remaining = 0.0f;
                emission.stream.position = start;
                emission.stream.time = 0.0f;
                emission.scale = (math_randomFloat() * 100.0f + 150.0f) * 0.1000000015f;
                emission.duration = source->duration;
                emission.durationRange = source->durationRange;
                emission.color = source->color;
                effect = effect_newTimedEffectNoDraw(math_randomFloat() * 70.0f + 30.0f, -1, 15, 1, 11,
                                                           sizeof(Game113D60Emission), 0xFF, 1);
                if (effect != NULL) {
                    memcpy(&effect->emission, &emission, sizeof(Game113D60Emission));
                }
            }
        }
    }
}


/* Emits this frame's particles along a sine-wobbled direction and advances the wobble phases. */
void particle_updateSourceEmission(Game113D60Effect *effect) {
    Game113D60Emission *emission;
    f32 inverse;
    f32 time;
    f32 timeStep;
    f32 phaseStepX;
    f32 phaseStepY;
    f32 speedX;
    f32 speedY;
    f32 factor;
    f32 period;
    Vec3f direction;
    f32 phaseX;
    f32 phaseY;
    f32 sineX;
    f32 sineY;
    f32 randomX;
    f32 randomY;
    u32 randomDuration;
    u32 randomColor;

    emission = &effect->emission;
    effect->emission.stream.remaining += effect->emission.stream.rate * gFrameDelta;
    if (effect->emission.stream.remaining > 1.0f) {
        inverse = 1.0f / emission->stream.remaining;
        factor = 0.1f;
        time = emission->stream.time + gFrameDelta;
        timeStep = time * inverse;
        speedX = emission->stream.speedX * gFrameDelta;
        speedY = emission->stream.speedY * gFrameDelta;
        phaseX = emission->stream.phaseX;
        phaseY = emission->stream.phaseY;
        phaseStepX = speedX * inverse;
        phaseStepY = speedY * inverse;
        do {
            sineX = sinf(phaseX);
            sineY = sinf(phaseY);
            math_dirFromAngles(emission->stream.angleX + sineX * emission->stream.amplitudeX,
                          emission->stream.angleY + sineY * emission->stream.amplitudeY, 5.0f, &direction);
            randomX = math_randomFloat();
            randomY = math_randomFloat();
            randomColor = math_random();
            randomDuration = math_random();
            particle_spawnImpactStreak(&emission->stream.position, &direction, emission->scale, (randomX * 300.0f + 100.0f) * factor,
                          (randomY * 700.0f + 300.0f) * factor, effect->base.header.owner, effect->base.header.kind,
                          emission->color, randomColor % 86 + 170,
                          randomDuration % (emission->durationRange + 1) + emission->duration, -1);
            phaseX += phaseStepX;
            phaseY += phaseStepY;
            time -= timeStep;
            emission->stream.remaining -= 1.0f;
        } while (emission->stream.remaining > 1.0f);
        period = 6.283185482f;
        emission->stream.phaseX = math_wrapFloat(phaseX, period, 0.0f);
        emission->stream.phaseY = math_wrapFloat(phaseY, period, 0.0f);
        emission->stream.time = time;
    }
}

typedef struct {
    /* 0x00 */ Vec3f start;
    /* 0x0C */ Vec3f end;
} Game113D60Segment;

static const Game113D60Segment D_800A11D0 = { { -2214.0f, 1150.0f, 4079.0f }, { -1230.0f, 1150.0f, 3722.0f } };
static const Game113D60Segment D_800A11E8 = { { 261.0f, 1000.0f, 3971.0f }, { 1053.0f, 1000.0f, 4149.0f } };
static const Game113D60Segment *D_80088A3C = &D_800A11D0;
static const Game113D60Segment *D_80088A40 = &D_800A11E8;

/* Start callback: a random point on one of two segments. */
void particle_randomPointOnTwoSegments(Vec3f *out) {
    const Game113D60Segment *segment;
    f32 t;

    if (math_randomFloat() < 0.5632223487f) {
        segment = D_80088A3C;
    } else {
        segment = D_80088A40;
    }
    t = math_randomFloat();
    out->x = (segment->end.x - segment->start.x) * t + segment->start.x;
    out->y = (segment->end.y - segment->start.y) * t + segment->start.y;
    out->z = (segment->end.z - segment->start.z) * t + segment->start.z;
}

/* The two zones that level_registerEmissionSourceZone registers (by unk18). */
TriggerZone *gEmissionSourceZones[2];

/* End callback: the position of one of the two entries of the gEmissionSourceZones table. */
void particle_randomPointInSourceZones(Vec3f *out) {
    math_randomPointInZone(gEmissionSourceZones[math_random() & 1], out);
}

static const Game113D60Segment D_800A1200[6] = {
    { { -5626.0f, 194.0f, 4084.0f }, { -6566.0f, 207.0f, 5929.0f } },
    { { -6566.0f, 207.0f, 5929.0f }, { -6830.0f, -231.0f, 8012.0f } },
    { { -6830.0f, -231.0f, 8012.0f }, { -6615.0f, -781.0f, 9425.0f } },
    { { 4176.0f, 214.0f, 4711.0f }, { 3843.0f, 240.0f, 6531.0f } },
    { { 3843.0f, 240.0f, 6531.0f }, { 3730.0f, -266.0f, 8330.0f } },
    { { 3730.0f, -266.0f, 8330.0f }, { 3603.0f, -866.0f, 9595.0f } },
};
static const Game113D60Segment *D_80088A44[] = {
    &D_800A1200[0], &D_800A1200[1], &D_800A1200[2], &D_800A1200[3], &D_800A1200[4], &D_800A1200[5],
};

/* Start callback: a random point on one of six segments. */
void particle_randomPointOnSixSegments(Vec3f *out) {
    const Game113D60Segment *segment;
    f32 t;

    segment = D_80088A44[math_random() % 6];
    t = math_randomFloat();
    out->x = (segment->end.x - segment->start.x) * t + segment->start.x;
    out->y = (segment->end.y - segment->start.y) * t + segment->start.y;
    out->z = (segment->end.z - segment->start.z) * t + segment->start.z;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ Vec3f unk38;
} Bone150E6FAC;


/* End callback: a random point on a ring around node 0x16 of the actor's model, or the actor's
 * position when the model has no such node. */
void particle_randomPointAroundActorNode(Vec3f *out, Actor *actor) {
    Attachment *node;
    f32 sine;
    f32 radius;
    s16 angle;
    f32 cosine;

    if (effect_findAttachmentByType(actor->attachments, 0x16, &node) != 0) {
        Bone150E6FAC *bone;

        radius = (math_randomFloat() * 100.0f) + 80.0f;
        angle = math_random() & 0xFF;
        cosine = math_cosBinAngleMasked(angle - 0x40);
        sine = math_cosBinAngleMasked(angle);
        bone = node->handle;
        out->x = actor->xpos + (bone->unk38.x * -80.0f) + (cosine * radius);
        out->y = actor->ypos + (bone->unk38.y * -80.0f) + 100.0f;
        out->z = actor->zpos + (bone->unk38.z * -80.0f) + (sine * radius);
    } else {
        out->x = actor->xpos;
        out->y = actor->ypos;
        out->z = actor->zpos;
    }
}

/* End callback: the actor's position. */
void particle_getEmissionActorPos(Vec3f *out, Actor *actor) {
    out->x = actor->xpos;
    out->y = actor->ypos;
    out->z = actor->zpos;
}

/* Motion callback: aims the emission along `delta` with a random wobble. */
void particle_aimEmissionAlongDelta(Vec3f *start, Vec3f *end, Vec3f *delta, Game113D60Emission *emission) {
    f32 dx;
    f32 dz;

    emission->stream.angleX = math_atan2(delta->x, delta->z);
    emission->stream.amplitudeX = math_randomFloat() * 0.09000000358f;
    emission->stream.phaseX = math_randomFloat() * 6.283185482f;
    emission->stream.speedX = math_randomFloat() * 0.5f;
    dx = delta->x;
    dz = delta->z;
    emission->stream.angleY = math_atan2(sqrtf((dx * dx) + (dz * dz)), delta->y) - 1.570796371f;
    emission->stream.amplitudeY = math_randomFloat() * 0.0700000003f;
    emission->stream.phaseY = math_randomFloat() * 6.283185482f;
    emission->stream.speedY = math_randomFloat() * 0.5f;
}

/* Motion callback: aims the emission along `delta` with a fixed tilt and a random wobble. */
void particle_aimEmissionTilted(Vec3f *start, Vec3f *end, Vec3f *delta, Game113D60Emission *emission) {
    emission->stream.angleX = math_atan2(delta->x, delta->z);
    emission->stream.amplitudeX = 0.471238941f;
    emission->stream.phaseX = math_randomFloat() * 6.283185482f;
    emission->stream.speedX = math_randomFloat() * 0.200000003f;
    emission->stream.angleY = -0.5654867291f;
    emission->stream.amplitudeY = 0.3141592741f;
    emission->stream.phaseY = math_randomFloat() * 6.283185482f;
    emission->stream.speedY = math_randomFloat() * 0.150000006f;
}

/* Occasionally spawns one of the two sprite_newScreenSprite effects, plus a sound and an event. */
void func_150E7290(arg0, owner, kind)
    u8 arg0;
    u8 owner;
    s32 kind;
{
    if (math_randomFloat() < 0.00419999985f) {
        s32 ids[2] = { 4, 5 };
        Vec2f position;
        RumbleParams event;
        s32 pad; // matching: unused stack slot
        u32 flip4;
        u32 flip2;
        u32 random;
        f32 randomSize;

        if (math_randomFloat() < 0.7500000596f) {
            position.x = (math_randomFloat() * 300.0f) + -150.0f;
            position.y = (math_randomFloat() * 200.0f) + -100.0f;
            randomSize = math_randomFloat();
            random = math_random();
            flip2 = (math_random() & 1) ? 2 : 0;
            flip4 = (math_random() & 1) ? 4 : 0;
            particle_spawnScreenSprite(&position, ((randomSize * 150.0f) + 200.0f) * 0.1f, (random % 201) + 500,
                          (u8)(flip4 | flip2) | 9, (math_random() % 26) + 100, 0xFF, 0x40, 3, ids, 2, owner,
                          kind);
        } else {
            randomSize = math_randomFloat();
            flip4 = math_random(); // matching: a separate variable here does not match
            particle_spawnScreenEdgeSprite(((randomSize * 100.0f) + 150.0f) * 0.1f, (flip4 % 201) + 500, 9,
                          (math_random() % 26) + 100, 0xFF, 0x40, 3, ids, 2, owner, kind);
        }
        sound_play(0x360, 0x7FFF, 0, 0, 0);
        camera_startWobblePreset(1, arg0, NULL, owner, kind);
        event.flags = 1;
        event.duration = (math_random() % 26) + 25;
        event.playerMask = 1;
        event.strength = (math_random() % 6) + 3;
        event.updateKind = -1;
        cont_spawnRumble(&event, 0, 0xFF, 0);
    }
}

/* Spawns a sprite_newScreenSprite effect of the given size at `position`. */
void particle_spawnScreenSprite(Vec2f *position, f32 size, s16 unk12, u8 flags, u8 unk1E, u8 unk23, s16 unk16, s16 unk18, s32 *ids,
                   s32 idCount, u8 owner, s32 kind) {
    s32 pad; // matching: unused stack slot
    ScreenSpriteInfo arg;
    u8 id[1] = { 0x44 };

    // matching: x and y are copied as one Vec2f
    *(Vec2f *)&arg.x = *position;
    arg.halfWidth = size;
    arg.halfHeight = size;
    arg.texture = id[0];
    arg.timer = unk12;
    arg.flags = flags | 0x40;
    arg.fadeStart = unk16;
    arg.fadeRate = unk18;
    arg.kind = 4;
    arg.color1.red = 0xFF;
    arg.color1.green = 0xE6;
    arg.color1.blue = 0xBE;
    arg.color1.alpha = unk1E;
    arg.color2.red = 0xFF;
    arg.color2.green = 0xFF;
    arg.color2.blue = 0xFF;
    arg.color2.alpha = 0xFF;
    arg.colorLevel = unk23;
    arg.render.otherModeH = 1;
    arg.render.setGeometryMode = 0;
    arg.render.clearGeometryMode = 0;
    arg.render.primMode = 0;
    arg.render.envMode = 10;
    arg.render.renderMode = 7;
    arg.render.combiner = 60;
    arg.render.colorDither = 0x80;
    arg.render.alphaDither = 0x20;
    sprite_newScreenSprite(&arg, 0, ids, idCount, 0, owner, kind);
}

/* Like particle_spawnScreenSprite, but placed at a random screen edge. */
void particle_spawnScreenEdgeSprite(f32 size, s16 unk12, u8 flags, u8 unk1E, u8 unk23, s16 unk16, s16 unk18, s32 *ids, s32 idCount,
                   u8 owner, s32 kind) {
    s32 pad; // matching: unused stack slot
    ScreenSpriteInfo arg;
    u8 edge;

    edge = math_random() & 3;

    arg.timer = unk12;
    arg.flags = flags & 0xFFF9;
    arg.fadeStart = unk16;
    arg.fadeRate = unk18;
    arg.kind = 5;
    arg.color1.red = 0xFF;
    arg.color1.green = 0xE6;
    arg.color1.blue = 0xBE;
    arg.color1.alpha = unk1E;
    arg.color2.red = 0xFF;
    arg.color2.green = 0xFF;
    arg.color2.blue = 0xFF;
    arg.color2.alpha = 0xFF;
    arg.colorLevel = unk23;
    arg.render.otherModeH = 1;
    arg.render.setGeometryMode = 0;
    arg.render.clearGeometryMode = 0;
    arg.render.primMode = 0;
    arg.render.envMode = 10;
    arg.render.renderMode = 7;
    arg.render.combiner = 60;
    arg.render.colorDither = 0x80;
    arg.render.alphaDither = 0x20;
    arg.halfHeight = size;
    arg.halfWidth = size;

    switch (edge) {
    case 0:
    case 1:
        {
            s32 sideIds[3] = { 0x48, 0x49, 0x4A };

            arg.texture = sideIds[math_random() % 3];
            arg.y = (math_randomFloat() * 160.0f) + -80.0f;
            switch (edge) {
            case 0:
                arg.flags |= 2;
                arg.x = 145.0f - size;
                break;
            case 1:
                arg.x = size - 145.0f;
                break;
            }
        }
        break;
    case 2:
    case 3:
        {
            s32 topIds[3] = { 0x45, 0x46, 0x47 };

            arg.texture = topIds[math_random() % 3];
            arg.x = (math_randomFloat() * 260.0f) + -130.0f;
            switch (edge) {
            case 2:
                arg.flags |= 4;
                arg.y = 110.0f - size;
                break;
            case 3:
                arg.y = size - 110.0f;
                break;
            }
        }
        break;
    }

    sprite_newScreenSprite(&arg, 0, ids, idCount, 0, owner, kind);
}

/* Creates a burst effect (updated by particle_updateScreenBurst) whose `count` points lie on a random
 * curve across the screen. */
TimedEffect *particle_startScreenBurst(s16 count, f32 rate, u8 owner, s32 kind) {
    TimedEffect *obj;
    Game113D60BurstHead head;
    RumbleParams event;
    Vec2f control[4];
    f32 *points;
    f32 t;
    f32 step;
    s16 i;

    if (count < 2) {
        return NULL;
    }

    camera_startEnvelope(&camera_global_Cameras[gLastPlayerIndex], 0, 1);

    event.flags = 1;
    event.duration = (math_random() % 11) + 30;
    event.strength = 8;
    event.updateKind = -1;
    event.playerMask = 1;
    cont_spawnRumble(&event, 0, 0xFF, 0);

    head.rate = rate;
    head.accum = 0.0f;
    head.count = count;
    head.next = 0;

    obj = effect_newTimedEffectNoDraw(300, -1, 16, 1, 12, (count * sizeof(Vec2f)) + 0x10, owner, kind);
    if (obj != NULL) {
        memcpy(&((Game113D60BurstEffect *)obj)->burst.head, &head, sizeof(head));

        control[0].x = -146.0f;
        control[0].y = (math_randomFloat() * 160.0f) - 80.0f;
        control[1].x = -50.0f;
        control[1].y = (math_randomFloat() * 160.0f) - 80.0f;
        control[2].x = 50.0f;
        control[2].y = (math_randomFloat() * 160.0f) - 80.0f;
        control[3].x = 146.0f;
        control[3].y = (math_randomFloat() * 160.0f) - 80.0f;

        t = -1.0f;
        points = ((Game113D60BurstEffect *)obj)->burst.points;
        step = 3.0f / (count - 1);

        for (i = 0; i < count; i++) {
            points[i * 2] = (math_lagrangeWeight0(t) * control[0].x) + (math_lagrangeWeight1(t) * control[1].x) +
                          (math_lagrangeWeight2(t) * control[2].x) + (math_lagrangeWeight3(t) * control[3].x);
            points[(i * 2) + 1] = (math_lagrangeWeight0(t) * control[0].y) + (math_lagrangeWeight1(t) * control[1].y) +
                          (math_lagrangeWeight2(t) * control[2].y) + (math_lagrangeWeight3(t) * control[3].y);
            t += step;
        }
    }
    return obj;
}

/* Spawns the burst's next points, `rate` per frame, and ends the effect after the last one. */
void particle_updateScreenBurst(Game113D60BurstEffect *effect) {
    s32 pad[2]; // matching: unused stack slots
    Vec2f position;
    f32 randomSize;
    s32 flip2;
    s32 flip4;
    Game113D60Burst *burst;

    burst = &effect->burst;
    burst->head.accum += burst->head.rate * gFrameDelta;
    while ((burst->head.accum > 1.0f) && (burst->head.next != burst->head.count)) {
        position.x = burst->points[burst->head.next * 2];
        position.y = burst->points[burst->head.next * 2 + 1];
        randomSize = math_randomFloat();
        flip4 = (math_random() & 1) ? 4 : 0;
        flip2 = (math_random() & 1) ? 2 : 0;
        particle_spawnScreenSprite(&position, (randomSize * 12.0f) + 30.0f, 300, flip2 | flip4,
                      (math_randomFloat() * 25.0f) + 100.0f, 0xFF, 1, 0xFF, NULL, 0, effect->base.header.owner,
                      effect->base.header.kind);
        sound_play(0x360, 0x7FFF, (position.x * 0.4315068424f) + 64.0f, 0, 0);
        burst->head.next++;
        burst->head.accum -= 1.0f;
    }
    if (burst->head.next >= burst->head.count) {
        effect->base.header.state = -1;
    }
}

/* Spawns a particle_spawnSurfaceQuadSprite effect of the given size at `position`, lasting `duration` frames
 * (-1: a fixed 300 frames without the timed flag). */
void particle_spawnImpactMark(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, u8 owner, s32 kind) {
    s32 pad; // matching: unused stack slot
    SpriteDef arg;
    s32 argB;
    s32 argC;

    arg.fadeStart = 20;
    arg.fadeRate = 12;
    arg.texture = 0x4F;
    arg.handlerSet = 0;
    arg.flags = ((duration == -1) ? 0 : 1) | (arg5 ? 0x400 : 0) | 0x100 | 0x200 | 0x1000 | 0x8000 | 0x40000;
    arg.lifetime = (duration == -1) ? 300 : duration + 20;
    arg.frame = 0;
    arg.frameSpeed = 0;
    arg.alpha = arg1;
    arg.colorLevel = 0xFF;
    arg.rgba[0] = arg7;
    arg.rgba[1] = arg8;
    arg.rgba[2] = arg9;
    arg.rgba[3] = 0xFF;
    arg.renderModes = ((arg6 != 0) ? 2 : 1) + 0x480000;
    arg.primMode = 0;
    arg.envMode = 6;
    if (arg5 != 0) {
        argB = 3;
        argC = 0xFF;
    } else {
        argB = 0;
        argC = 0;
    }
    particle_spawnSurfaceQuadSprite(&arg, 0, 0, surface, position->x, position->y, position->z, size, size, math_random() & 0xFF,
                  (math_random() & 1) + ((math_random() & 1) * 2), argB, argC, 0, owner, kind);
}

/* Effect points; func_150E81A8 uses 4-7. */
static const Vec3f D_800A1290[8] = {
    { -780.8950195f, -32.0f, 2153.626953f },
    { -172.0f, -32.0f, 942.0f },
    { -852.0f, -32.0f, -638.0f },
    { -242.0f, -32.0f, -4208.0f },
    { -7.0f, -798.0f, 7530.0f },
    { -2207.0f, -480.0f, 7286.0f },
    { -1488.0f, -454.0f, 5528.0f },
    { 610.0f, -452.0f, 5605.0f },
};

/* For points 4-7 of the D_800A1290 table: spawns the particle_spawnLightFlash and particle_spawnJetSprayBursts effects
 * 200 units above the point, plus an event. */
void func_150E81A8(index, owner, kind)
    u8 index;
    u8 owner;
    s32 kind;
{
    Vec3f position;
    JetSpraySpec arg;
    RumbleParams event;

    if (index == 4 || index == 5 || index == 6 || index == 7) {
        position = D_800A1290[index];
        position.y += 200.0f;
        particle_spawnLightFlash(&position, owner, kind);

        arg.angles.yawBase = 0;
        arg.angles.yawRange = 0xFF;
        arg.angles.pitchBase = -0x40;
        arg.angles.pitchRange = 0x4D;
        arg.spray.countBase = 10;
        arg.spray.countRange = 5;
        arg.spray.pos = position;
        arg.spray.speedBase = 252.0f;
        arg.spray.speedRange = 117.0f;
        arg.spray.sizeBase = 308.0f;
        arg.spray.sizeRange = 256.0f;
        arg.spray.sizeSlopeBase = 699.0f;
        arg.spray.sizeSlopeRange = 511.0f;
        arg.spray.particleCountBase = 4;
        arg.spray.particleCountRange = 7;
        arg.spray.spread = 27.0f;
        arg.spray.drag = 0.7820000648f;
        arg.spray.gravityBase = -0.9300000668f;
        arg.spray.gravityRange = 0.7300000191f;
        arg.spray.delayBase = 25;
        arg.spray.delayRange = 15;
        arg.spray.alphaBase = 100;
        arg.spray.alphaRange = 100;
        arg.spray.fadeStart = 12;
        arg.spray.fadeOutStart = 20;
        arg.spray.eventFunc = 0;

        particle_spawnJetSprayBursts(&arg, owner, kind);

        event.flags = 1;
        event.duration = (math_random() % 11) + 30;
        event.strength = 8;
        event.updateKind = -1;
        event.playerMask = 1;
        cont_spawnRumble(&event, 0, 0xFF, 0);
    }
}

typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 accum;
} Game113D60PointData;

/* Creates effect 0x28 at `position`, lasting `duration` frames (-1: 300). */
void particle_startRandomEmitter(const Vec3f *position, s16 duration, u8 owner, s32 kind) {
    Game113D60PointData data;
    TimedEffect *effect;

    data.position = *position;
    data.accum = 0.0f;

    effect = effect_newTimedEffect((duration == -1) ? 300 : duration, -1, 0x28, -1, (duration == -1) ? 0 : 1, 0, sizeof(data),
                           owner, kind);
    if (effect != NULL) {
        memcpy(((Game113D60AnyEffect *)effect)->data, &data, sizeof(data));
    }
}

/* Spawns a random number of particles per frame at the effect's position. */
void particle_updateRandomEmitter(Game113D60EmitterEffect *effect) {
    Game113D60Emitter *emitter;

    emitter = &effect->emitter;
    emitter->accum += (0.09999999404f + math_randomFloat() * 0.08789999783f) * gFrameDelta;
    if (emitter->accum > 1.0f) {
        ParticleDesc arg;
        Game113D60ParticleExtra extra;
        Game113D60Particle *particle;

        arg.texture = 0x6C;
        arg.renderMode = 0x5103;
        arg.unk0 = 0x200005;
        arg.fadeStart = 0x73;
        arg.fadeRate = 2;
        arg.flags = 0x80DE07;
        arg.primColorMode = 8;
        arg.envColorMode = 6;
        arg.updateFunc = 0x19;
        arg.deathFunc = -1;
        extra.phase[0] = 0;
        extra.phase[1] = 0;
        arg.unk4 = 0;
        arg.frame = 0;
        arg.frameRate = 0;
        arg.drawFunc = -1;
        arg.handlerSet = 0;
        arg.scaleStart = 0x73;
        arg.r = 0x24;
        arg.g = 0x22;
        arg.b = 0x11;
        arg.a = 0xFF;
        arg.r2 = 0x7B;
        arg.g2 = 0x93;
        arg.b2 = 0xAA;
        arg.brightness = 0xFF;
        arg.scaleRate = 1.022418022f;
        arg.pos = emitter->position;
        arg.offset.x = 0.0f;
        arg.offset.y = 0.0f;
        arg.offset.z = 0.0f;
        arg.vel.x = 0.0f;
        arg.vel.y = 0.0f;
        arg.vel.z = 0.0f;
        do {
            extra.phaseStep[0] = math_random() % 5 + 4;
            extra.phaseStep[1] = math_random() % 5 + 4;
            extra.amplitude[0] = math_randomFloat() * 13.0f;
            extra.amplitude[1] = math_randomFloat() * 13.0f;
            arg.gravity = (math_randomFloat() * 40.0f + 68.0f) * 0.001000000047f;
            arg.flags &= ~0xC0;
            arg.flags |= ((math_random() & 1) ? 0x40 : 0) | ((math_random() & 1) ? 0x80 : 0);
            arg.alpha = math_random() % 156 + 100;
            arg.timer = math_random() % 34 + 141;
            arg.width = arg.height = math_randomFloat() * 71.0f + 80.0f;
            particle = particle_newSpriteParticleWithFloor(&arg, 1, 0, sizeof(extra), effect->base.header.owner, effect->base.header.kind);
            if (particle != NULL) {
                memcpy(&particle->extra, &extra, sizeof(extra));
            }
            emitter->accum -= 1.0f;
        } while (emitter->accum > 1.0f);
    }
}

/* Update callback of the particles particle_updateRandomEmitter spawns. */
s32 particle_wobbleRandomEmitterParticle(Game113D60Particle *particle, s32 arg1) {
    particle_wobbleSpriteOffset(&particle->head, particle->extra.amplitude, particle->extra.phase, particle->extra.phaseStep);
    return 1;
}

/* Creates the timer effect (0x35) that particle_updateShakeTimer updates. */
void particle_startShakeTimer(void) {
    TimedEffect *effect;
    f32 timer;

    timer = 10.0f;

    effect = effect_newTimedEffect(300, -1, 0x35, -1, 0, 0, sizeof(timer), 0xFF, 1);
    if (effect != NULL) {
        memcpy(((Game113D60AnyEffect *)effect)->data, &timer, sizeof(timer));
    }
}

/* Counts the timer down and fires particle_fireShake each time it runs out (rearmed to 201-538). */
void particle_updateShakeTimer(Game113D60TimerEffect *effect) {
    effect->timer -= gFrameDelta;
    if (effect->timer < 0.0f) {
        effect->timer = (math_randomFloat() * 337.0f) + 201.0f;
        particle_fireShake(effect);
    }
}

/* Fires a pair of effect objects through effect_sendEventToTypes plus an event, effects 0x33 and 0x36 when the
 * gDustRegions region list exists, and two sounds. */
void particle_fireShake() {
    s32 ids[2] = { 0x19, 0x48 };
    RumbleParams event;

    effect_sendEventToTypes(ids, 2, 0, 0x1B);
    camera_startWobblePreset(0, gLastPlayerIndex, NULL, 0xFF, 1);
    event.flags = 1;
    event.duration = (math_random() % 21) + 20;
    event.playerMask = 1;
    event.strength = (math_random() % 6) + 3;
    event.updateKind = -1;
    cont_spawnRumble(&event, 0, 0xFF, 1);
    if (gDustRegions != NULL) {
        particle_startDustRegionSpawner();
    }
    if (gDustRegions != NULL) {
        particle_startDebrisRegionSpawner();
    }
    sound_play(0x4C8, 0x7FFF, 0x40, 0x200 - (math_random() & 0x400), 0);
    sound_play(0x4CD, 0x5DC0, 0x40, 0x200 - (math_random() & 0x400), 0);
}

typedef struct {
    /* 0x0 */ f32 rate;
    /* 0x4 */ f32 rateRange;
    /* 0x8 */ f32 accum;
} Game113D60Data8A80;

/* Creates effect 0x33 for 30-70 frames. */
void particle_startDustRegionSpawner(void) {
    TimedEffect *effect;
    Game113D60Data8A80 data;

    data.rate = 1.199999957e-07f;
    data.rateRange = 3.199999981e-07f;
    data.accum = 0.0f;

    effect = effect_newTimedEffect((math_random() % 41) + 30, -1, 0x33, -1, 1, 0, sizeof(data), 0xFF, 1);
    if (effect != NULL) {
        memcpy(((Game113D60AnyEffect *)effect)->data, &data, sizeof(data));
    }
}

typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 rate;
    /* 0x10 */ f32 rateRange;
    /* 0x14 */ f32 accum;
} Game113D60Data8B1C;

/* Spawns effect 0x34 at random points of the weighted regions within 1800 units of the
 * position math_getCameraPos returns. */
void particle_updateDustRegionSpawner(Game113D60RegionSpawnerEffect *effect) {
    Vec3f *center;
    Game113D60Spawner *spawner;
    RegionNode *node;
    f32 pick;
    Game113D60Data8B1C data;
    Game113D60Data8B1C *dataPtr;
    f32 dx;
    f32 dy;
    f32 dz;

    dataPtr = &data; // matching: using &data directly at each use does not match
    center = math_getCameraPos(gLastPlayerIndex);
    spawner = &effect->spawner;
    spawner->accum += (spawner->rate + (math_randomFloat() * spawner->rateRange)) * gFrameDelta * gDustRegionsArea;
    if (spawner->accum > 1.0f) {
        do {
            pick = math_randomFloat() * gDustRegionsArea;
            node = gDustRegions;
            while (node->area < pick) {
                pick -= node->area;
                node = node->next;
            }
            math_randomPointInZone(node->region, &dataPtr->position);
            dx = data.position.x - center->x;
            dy = data.position.y - center->y;
            dz = data.position.z - center->z;
            if (dx * dx + dy * dy + dz * dz < 3240000.0f) {
                TimedEffect *child;

                data.rate = 0.3171199858f;
                data.rateRange = 1.046509981f;
                data.accum = 0.0f;
                child = effect_newTimedEffect((math_random() % 13) + 5, -1, 0x34, -1, 1, 0, sizeof(data),
                                      effect->base.header.owner, effect->base.header.kind);
                if (child != NULL) {
                    memcpy(((Game113D60AnyEffect *)child)->data, dataPtr, sizeof(data));
                }
            }
            spawner->accum -= 1.0f;
        } while (spawner->accum > 1.0f);
    }
}

/* Spawns particle_newOwnedSpriteParticle particles within 20 units of the spawner's position. */
void particle_updateFallingDustSpawner(Game113D60PointSpawnerEffect *effect) {
    Game113D60PointSpawner *spawner;
    TimedEffect *particle;
    s32 flip80;
    s32 flip40;
    ParticleDesc64 arg;
    s32 pad; // matching: unused stack slot
    f32 extra;
    f32 zero; // matching: writing 0.0f literals instead does not match

    spawner = &effect->spawner;
    spawner->accum += (spawner->rate + (math_randomFloat() * spawner->rateRange)) * gFrameDelta;
    if (spawner->accum > 1.0f) {
        zero = 0.0f;
        {
            f32 size;

            arg.texture = 0x16;
            arg.renderMode = 0xE01;
            arg.unk0 = 0x200005;
            arg.unk4 = 0;
            arg.frame = 0;
            arg.frameRate = 0;
            arg.flags = 0x801E05;
            arg.primColorMode = 3;
            arg.envColorMode = 3;
            arg.updateFunc = 0x1A;
            arg.deathFunc = -1;
            arg.offset.x = zero;
            arg.offset.y = zero;
            arg.offset.z = zero;
            arg.gravity = zero;
            arg.a = 0xFF;
            arg.brightness = 0xFF;
            arg.fadeStart = 30;
            arg.fadeRate = 8;
            arg.vel.x = zero;
            arg.vel.z = zero;
            do {
                extra = 0.9700999856f;
                arg.scaleStart = arg.timer = (math_random() & 0xF) + 40;
                arg.alpha = (math_random() % 119) + 100;
                size = (math_randomFloat() * 59.0f) + 80.0f;
                arg.height = arg.width = size;
                arg.scaleRate = ((math_randomFloat() * 3727.0f) + 99979.0f) * 9.999999747e-06f;
                arg.flags &= ~0xC0;
                if (math_random() & 1) {
                    flip80 = 0x80;
                } else {
                    flip80 = 0;
                }
                if (math_random() & 1) {
                    flip40 = 0x40;
                } else {
                    flip40 = 0;
                }
                arg.flags |= flip40 | flip80;
                arg.r2 = 0x44;
                arg.g2 = 0x3C;
                arg.b2 = 0x27;
                arg.r = 0xF;
                arg.g = 0x11;
                arg.b = 5;
                math_dirFromAngles(math_randomFloat() * 6.283185482f, math_randomFloat() * 6.283185482f, math_randomFloat() * 20.0f,
                              &arg.pos);
                arg.pos.x += spawner->position.x;
                arg.pos.y += spawner->position.y;
                arg.pos.z += spawner->position.z;
                arg.vel.y = ((math_randomFloat() * 471.0f) + -1242.0f) * 0.009999999776f;
                particle = particle_newOwnedSpriteParticle(&arg, 0, sizeof(extra), effect->base.header.owner, effect->base.header.kind);
                if (particle != NULL) {
                    memcpy((u8 *)particle + 0xA8, &extra, sizeof(extra));
                }
                spawner->accum -= 1.0f;
            } while (spawner->accum > 1.0f);
        }
    }
}

typedef struct {
    /* 0x0 */ f32 rate;
    /* 0x4 */ f32 rateRange;
    /* 0x8 */ f32 accum;
} Game113D60Data90DC;

/* Creates effect 0x36 for 5-30 frames. */
void particle_startDebrisRegionSpawner(void) {
    TimedEffect *effect;
    Game113D60Data90DC data;

    data.rate = 2.000000023e-07f;
    data.rateRange = 1.500000053e-07f;
    data.accum = 0.0f;

    effect = effect_newTimedEffect((math_random() % 26) + 5, -1, 0x36, -1, 1, 0, sizeof(data), 0xFF, 1);
    if (effect != NULL) {
        memcpy(((Game113D60AnyEffect *)effect)->data, &data, sizeof(data));
    }
}

typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 rate;
    /* 0x10 */ f32 rateRange;
    /* 0x14 */ f32 accum;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8 pad1C[0x14];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x38 */ s32 unk38;
} Game113D60Data9178;

/* Like particle_updateDustRegionSpawner, but spawns effect 0x37 within 1400 units. */
void particle_updateDebrisRegionSpawner(Game113D60RegionSpawnerEffect *effect) {
    Vec3f *center;
    Game113D60Spawner *spawner;
    RegionNode *node;
    f32 pick;
    Game113D60Data9178 data;
    f32 dx;
    f32 dy;
    f32 dz;

    center = math_getCameraPos(gLastPlayerIndex);
    spawner = &effect->spawner;
    spawner->accum += (spawner->rate + (math_randomFloat() * spawner->rateRange)) * gFrameDelta * gDustRegionsArea;
    if (spawner->accum > 1.0f) {
        do {
            pick = math_randomFloat() * gDustRegionsArea;
            node = gDustRegions;
            while (node->area < pick) {
                pick -= node->area;
                node = node->next;
            }
            math_randomPointInZone(node->region, &data.position);
            dx = data.position.x - center->x;
            dy = data.position.y - center->y;
            dz = data.position.z - center->z;
            if (dx * dx + dy * dy + dz * dz < 1960000.0f) {
                TimedEffect *child;

                data.rate = 0.1932400018f;
                data.rateRange = 0.3140099943f;
                data.accum = 0.0f;
                data.unk18 = 99999.89844f;
                data.unk30 = 0;
                data.unk34 = 0;
                data.unk35 = 0;
                data.unk38 = 0;
                child = effect_newTimedEffect((math_random() % 13) + 5, -1, 0x37, -1, 1, 0, sizeof(data),
                                      effect->base.header.owner, effect->base.header.kind);
                if (child != NULL) {
                    memcpy(((Game113D60AnyEffect *)child)->data, &data, sizeof(data));
                }
            }
            spawner->accum -= 1.0f;
        } while (spawner->accum > 1.0f);
    }
}

/* The debris models and their size scales. */
static const u8 gFallingDebrisModels[1] = { 7 };
static const f32 gFallingDebrisScales[1] = { 0.1f };

/* Spawns particle_newModelParticle debris particles (updated by particle_updateFallingDebris) within 50 units of the
 * spawner's position. */
void particle_updateFallingDebrisSpawner(Game113D60PointSpawnerEffect *effect) {
    Game113D60PointSpawner *spawner;
    Game113D60Debris *debris;
    ParticleSpawn78 arg;
    s32 i;
    Game113D60DebrisExtra extra; // copied into the particle uninitialized

    spawner = &effect->spawner;
    spawner->accum += (spawner->rate + (math_randomFloat() * spawner->rateRange)) * gFrameDelta;
    if (spawner->accum > 1.0f) {
        {
            f32 pad; // matching: unused stack slot

            arg.radius = 0.0f;
            arg.elasticity = 1.0f;
            arg.scale.x = 1.0f;
            arg.scale.y = 1.0f;
            arg.scale.z = 1.0f;
            arg.flags = 0x49E8;
            arg.vel.x = 0.0f;
            arg.vel.y = 0.0f;
            arg.vel.z = 0.0f;
            arg.rotVel.y = 0.0f;
            arg.variant = 0;
            arg.zone = 0;
            arg.alpha = 0xFF;
            arg.update2Fn = 0;
            arg.floorHitFn = 0;
            arg.floorType3Fn = 0;
            arg.wallHitFn = 0;
            arg.destroyFn = 0;
            arg.squashFn = 0;
            arg.matrixFn = 2;
            arg.lightMode = 2;
            arg.owner = 0;
            arg.ownerId = 0;
            arg.fadeStart = 1;
            arg.fadeRate = 0xFF;
            do {
                i = math_random() % 1;
                arg.model = gFallingDebrisModels[i];
                arg.sizeXZ = arg.sizeY = gFallingDebrisScales[i] * ((math_randomFloat() * 351.0f) + 400.0f) * 0.001000000047f;
                arg.rot.x = math_randomFloat() * 360.0f;
                arg.rot.y = math_randomFloat() * 360.0f;
                arg.rot.z = math_randomFloat() * 360.0f;
                arg.gravity = ((math_randomFloat() * 423.0f) + -1401.0f) * 0.001000000047f;
                arg.lifetime = (math_random() % 1) + 100;
                arg.rotVel.x = ((math_randomFloat() * 27374.0f) + -13687.0f) * 0.001000000047f;
                arg.rotVel.z = ((math_randomFloat() * 27374.0f) + -13687.0f) * 0.001000000047f;
                math_dirFromAngles(math_randomFloat() * 6.283185482f, math_randomFloat() * 6.283185482f, math_randomFloat() * 50.0f,
                              &arg.pos);
                arg.pos.x += spawner->position.x;
                arg.pos.y += spawner->position.y;
                arg.pos.z += spawner->position.z;
                arg.updateFn = 8;
                debris = particle_newModelParticle(&arg, 3, 0xFF, sizeof(extra), effect->base.header.owner, effect->base.header.kind);
                if (debris != NULL) {
                    memcpy(&debris->extra, &extra, sizeof(extra));
                }
                spawner->accum -= 1.0f;
            } while (spawner->accum > 1.0f);
        }
    }
}

/* Update callback of the debris particles: when a particle falls below its floor height it
 * ends (returns 0), spawning a particle_spawnSurfaceQuadSpriteNoModel splash first if its type is 0xA. */
s32 particle_updateFallingDebris(Game113D60Debris *debris) {
    Game113D60DebrisExtra *extra;
    SpriteDef splash;

    particle_moveBallistic(&debris->base);
    extra = &debris->extra;
    if (debris->base.pos.y < extra->floorY) {
        if ((extra->unk4 & 0x1F) == 0xA) {
            f32 size;

            size = (math_randomFloat() * 20.0f) + 20.0f;

            splash.texture = 0xB;
            splash.handlerSet = 0;
            splash.flags = 0x9701;
            splash.lifetime = 100;
            splash.frame = 0;
            splash.frameSpeed = 0;
            splash.alpha = 0xFF;
            splash.colorLevel = 0xFF;
            splash.rgba[0] = 0xFF;
            splash.rgba[1] = 0xFF;
            splash.rgba[2] = 0xFF;
            splash.rgba[3] = 0xFF;
            splash.renderModes = 0x3B0003;
            splash.primMode = 0;
            splash.envMode = 7;
            splash.fadeStart = 20;
            splash.fadeRate = 12;

            particle_spawnSurfaceQuadSpriteNoModel(&splash, 0, 0, &extra->floorTri, debris->base.pos.x, extra->floorY + 5.0f,
                          debris->base.pos.z, size, size, math_random() & 0xFF, 0, 0, debris->base.header.owner,
                          debris->base.header.kind);
        }
        return 0;
    }
    return 1;
}
