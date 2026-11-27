#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Neutral zone handler 8 (gEffectZoneSpawnHandlers): the zone is a sight beam placement. It casts rays both ways from the
 * beam's midpoint (the beam runs ySize units along the zone's pitch/yaw) and spawns a sight beam between the
 * two surfaces hit. data1 holds the beam's timings and flags, data2 its colour and data3 its widths. */
s32 sight_placeBeam(TriggerZone *beam) {
    f32 originZ;
    f32 originY;
    f32 originX;
    f32 endZ;
    f32 endY;
    f32 endX;
    f32 dirZ;
    f32 dirY;
    f32 dirX;
    f32 revZ;
    f32 revY;
    f32 revX;
    f32 midZ;
    f32 midY;
    f32 midX;
    MtxF matrix;
    s32 hit1;
    s32 hit2;
    CollisionTri triangle1;
    CollisionTri triangle2;
    Vec3f point1;
    Vec3f point2;
    f32 distance1;
    f32 distance2;
    s32 object1;
    s32 object2;

    if (beam->inactive == 1) {
        return 1;
    }
    originX = beam->x;
    originY = beam->y;
    originZ = beam->z;
    math_mtxRotateRPYF(matrix, beam->pitch, beam->yaw, 0);
    math_transformPoint(matrix, 0.0f, beam->ySize, 0.0f, &endX, &endY, &endZ);
    endX += originX;
    endY += originY;
    endZ += originZ;
    dirX = endX - originX;
    dirY = endY - originY;
    dirZ = endZ - originZ;
    midX = dirX * 0.5f + originX;
    midY = dirY * 0.5f + originY;
    midZ = dirZ * 0.5f + originZ;
    revX = -dirX;
    revY = -dirY;
    revZ = -dirZ;
    hit1 = collision_castRayWorld(midX, midY, midZ, revX, revY, revZ, 0, &triangle1, &point1.x, &point1.y, &point1.z,
                         &distance1, &object1, 0, 0.0f);
    hit2 = collision_castRayWorld(midX, midY, midZ, dirX, dirY, dirZ, 0, &triangle2, &point2.x, &point2.y, &point2.z,
                         &distance2, &object2, 0, 0.0f);
    if (hit1 == 0 || hit2 == 0) {
        return 1;
    }
    sight_spawnBeam(&point1, &triangle1, &point2, &triangle2, beam, beam,
                  beam->data2 & 0xFF, (beam->data2 & 0xFF00) >> 8, (beam->data2 & 0xFF0000) >> 16,
                  (beam->data1 & 0x1000000) >> 24,
                  beam->data1 & 0xFF, (beam->data1 & 0xFF00) >> 8, (beam->data1 & 0xFF0000) >> 16,
                  (beam->data1 & 0x2000000) >> 25, (beam->data1 & 0x4000000) >> 26,
                  !((beam->data1 & 0x8000000) >> 27), !((beam->data1 & 0x20000000) >> 29),
                  ((beam->data1 & 0x40000000) >> 30) ? 1 : 0, (beam->data1 & 0x10000000) >> 28,
                  (beam->data3 & 0xFF) * 0.02463994361f,
                  ((beam->data3 & 0xFF00) >> 8) * 0.003079992952f,
                  !((beam->data1 & 0x80000000) >> 31), 255, 1);
    return 1;
}

/* Message 0x20 (see sight_notifyActorMoved). */
typedef struct {
    Actor *actor;
    TriggerZone *source;
} SightTouchMsg;

/* Message 0x3A: 0 = switch on, 1 = switch off, 2 = trigger. */
typedef struct {
    TriggerZone *source;
    u8 command;
} SightCommandMsg;

/* Spawns a sight beam object between two surface points, with emitters at both ends. */
SightBeamObject *sight_spawnBeam(Vec3f *start, CollisionTri *startTri, Vec3f *end, CollisionTri *endTri, TriggerZone *beam, TriggerZone *source, u8 red, u8 green, u8 blue, u8 hasMidEffect, s16 activeTime, s16 inactiveTime, s16 startTime, u8 blinks, u8 startsOff, u8 zoneMasked, u8 hasEndEffects, u8 arg11, u8 pulses, f32 pulseAngle, f32 pulseSpeed, u8 keepEnabled, u8 arg16, s32 arg17) {
    SightBeamObject *obj;
    ParticleDef spawn;
    SightBeamState state;
    Vec3i pos;

    collision_selectMap(0);
    state.zone0 = collision_findZoneLeaf(start->x, start->z);
    state.zone1 = collision_findZoneLeaf(end->x, end->z);
    if (state.zone0 == NULL || state.zone1 == NULL) {
        return NULL;
    }
    state.beam = beam;
    // matching: the right-nested grouping fixes the order IDO evaluates the flag bits in
    state.flags = (pulses ? 8 : 0) | ((zoneMasked ? 4 : 0) | ((startsOff ? 0 : 2) | ((blinks ? 1 : 0) | (keepEnabled ? 0x10 : 0))));
    if (!keepEnabled) {
        beam->inactive = 1;
    }
    state.pulseAngle = pulseAngle;
    state.pulseSpeed = pulseSpeed;
    state.unk94 = arg11;
    state.triangle0 = *startTri;
    state.triangle1 = *endTri;
    state.activeTime = activeTime;
    state.inactiveTime = inactiveTime;
    state.timer = startTime;
    state.period = activeTime + inactiveTime;
    state.texS = state.centerS = math_randomFloat() * 1024.0f + -2560.0f + 2048.0f;
    state.texT = state.centerT = math_randomFloat() * 1024.0f + 1536.0f + 2048.0f;
    state.radiusS = math_randomFloat() * 768.0f + 768.0f;
    state.radiusT = math_randomFloat() * 768.0f + 768.0f;
    state.angleS = math_randomFloat() * 6.283185482f;
    state.angleT = math_randomFloat() * 6.283185482f;
    state.speedS = (math_randomFloat() * 80.0f + 50.0f) * 0.001000000047f;
    state.speedT = (math_randomFloat() * 80.0f + 50.0f) * 0.001000000047f;
    {
        Vec3f ndir;

        spawn.scale.x = end->x - start->x;
        spawn.scale.y = end->y - start->y;
        spawn.scale.z = end->z - start->z;
        ndir.x = -spawn.scale.x;
        ndir.y = -spawn.scale.y;
        ndir.z = -spawn.scale.z;
        sight_spawnEmitter(state.emitter0, start, startTri, &ndir, state.zone0, red < blue, zoneMasked, arg16, 0);
        sight_spawnEmitter(state.emitter1, end, endTri, &spawn.scale, state.zone1, red < blue, zoneMasked, arg16, 0);
        if (state.emitter0[0] == NULL || state.emitter0[1] == NULL || state.emitter0[2] == NULL ||
            state.emitter1[0] == NULL || state.emitter1[1] == NULL || state.emitter1[2] == NULL) {
            if (state.emitter0[0] != NULL) {
                effect_destroyObject(&state.emitter0[0]->head.header);
            }
            if (state.emitter0[1] != NULL) {
                effect_destroyObject(&state.emitter0[1]->head.header);
            }
            if (state.emitter0[2] != NULL) {
                effect_destroyObject(&state.emitter0[2]->head.header);
            }
            if (state.emitter1[0] != NULL) {
                effect_destroyObject(&state.emitter1[0]->head.header);
            }
            if (state.emitter1[1] != NULL) {
                effect_destroyObject(&state.emitter1[1]->head.header);
            }
            if (state.emitter1[2] != NULL) {
                effect_destroyObject(&state.emitter1[2]->head.header);
            }
            return NULL;
        }
    }
    state.alternate = 0;
    state.phase = 0.0f;
    state.source = source;
    if (hasMidEffect) {
        LightEffectHeader hdr;

        pos.x = start->x + spawn.scale.x * 0.5f;
        pos.y = start->y + spawn.scale.y * 0.5f;
        pos.z = start->z + spawn.scale.z * 0.5f;
        hdr.flags = 2;
        hdr.update = 2;
        hdr.lifetime = 300;
        hdr.lightType = 0;
        state.midEffect = light_createJitterAt(&hdr, &pos, red, green, blue, 0xFF, 0, 0, 0x10, arg16, arg17);
    } else {
        state.midEffect = NULL;
    }
    spawn.texture = 0x59;
    spawn.type = 6;
    spawn.renderModes = 0x5A03;
    spawn.lifetime = 300;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.rgba[0] = red;
    spawn.rgba[1] = green;
    spawn.rgba[2] = blue;
    spawn.rgba[3] = 0xFF;
    spawn.width = 0.0f;
    spawn.height = 20.0f;
    spawn.pos = *start;
    spawn.rot = *end;
    spawn.flags = (level == 0x36 ? 0 : 0x02000000) | 0x6C0000 | 0x40000000;
    spawn.unk44 = 0xFF;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 6;
    obj = particle_newQuadParticleNoModel(&spawn, &gWhiteVtxColors, 0x1A, 0, 0, 0x18, 0, 0xA0, arg16, arg17);
    if (obj != NULL) {
        SightBeamState *objState;

        objState = &obj->state;
        memcpy(objState, &state, sizeof(SightBeamState));
        {
            Vec3f dir;
            f32 len;
            f32 inv;
            Vec3f p0;
            Vec3f p1;
            Vec3f q0;
            Vec3f q1;

            dir = spawn.scale;
            if (math_vec3Normalize(&dir, &dir, &len, &inv) != 0 && hasEndEffects) {
                p0.x = start->x + dir.x * 20.0f;
                p0.y = start->y + dir.y * 20.0f;
                p0.z = start->z + dir.z * 20.0f;
                p1.x = p0.x + dir.x * 500.0f;
                p1.y = p0.y + dir.y * 500.0f;
                p1.z = p0.z + dir.z * 500.0f;
                q0.x = end->x - dir.x * 20.0f;
                q0.y = end->y - dir.y * 20.0f;
                q0.z = end->z - dir.z * 20.0f;
                q1.x = q0.x - dir.x * 500.0f;
                q1.y = q0.y - dir.y * 500.0f;
                q1.z = q0.z - dir.z * 500.0f;
                objState->endEffect0 = sight_spawnExtension(obj, &p0, &p1, red, green, blue, state.zone0, zoneMasked, arg16, arg17);
                objState->endEffect1 = sight_spawnExtension(obj, &q0, &q1, red, green, blue, state.zone1, zoneMasked, arg16, arg17);
            } else {
                objState->endEffect0 = NULL;
                objState->endEffect1 = NULL;
            }
        }
        if (objState->midEffect != NULL) {
            sound_addDynamicSound(0x4D0, pos.x, pos.y, pos.z, 0x1F40, 0xFA, 0x64, (SoundRecordCallback)sight_onBeamSound, objState, 0, 8, (u8)math_random());
        }
    }
    return obj;
}

/* Builds the beam's quad for one view: a strip of the beam's width facing the camera. */
void *sight_buildBeamQuad(SightBeamObject *obj, s16 view) {
    Vtx *vertices;
    void *result;
    Vec3f *camera;
    Vec3f dir;
    Vec3f closest;
    f32 dx, dy, dz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    f32 scale;
    u8 fresh;
    SightBeamState *state;

    state = &obj->state;
    if (!(state->flags & 2)) {
        return NULL;
    }
    if (state->flags & 4) {
        if (!(state->zone0->views & (1 << view)) && !(state->zone1->views & (1 << view))) {
            return NULL;
        }
    }
    particle_getViewBuffer(obj->head.vtxBuffers, view, sizeof(obj->head.vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(obj->head.vtxBuffers[view], obj->head.vtxTemplate, sizeof(obj->head.vtxTemplate));
            memcpy(obj->head.vtxBuffers[view] + 4, obj->head.vtxTemplate, sizeof(obj->head.vtxTemplate));
        }
    } else {
        return NULL;
    }
    camera = math_getCameraPos(view);
    dir.x = obj->head.def.rot.x - obj->head.def.pos.x;
    dir.y = obj->head.def.rot.y - obj->head.def.pos.y;
    dir.z = obj->head.def.rot.z - obj->head.def.pos.z;
    math_closestPointOnSegment(&obj->head.def.pos, &dir, camera, &closest, 0);
    dx = closest.x - camera->x;
    dy = closest.y - camera->y;
    dz = closest.z - camera->z;
    nx = dir.y * dz - dy * dir.z;
    ny = dir.z * dx - dz * dir.x;
    nz = dir.x * dy - dx * dir.y;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = obj->head.def.height / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->v.ob[0] = obj->head.def.rot.x - nx;
    vertices->v.ob[1] = obj->head.def.rot.y - ny;
    vertices->v.ob[2] = obj->head.def.rot.z - nz;
    vertices->v.tc[0] = state->texS;
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = obj->head.def.pos.x - nx;
    vertices->v.ob[1] = obj->head.def.pos.y - ny;
    vertices->v.ob[2] = obj->head.def.pos.z - nz;
    vertices->v.tc[0] = state->texT;
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = obj->head.def.pos.x + nx;
    vertices->v.ob[1] = obj->head.def.pos.y + ny;
    vertices->v.ob[2] = obj->head.def.pos.z + nz;
    vertices->v.tc[0] = state->texT;
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = obj->head.def.rot.x + nx;
    vertices->v.ob[1] = obj->head.def.rot.y + ny;
    vertices->v.ob[2] = obj->head.def.rot.z + nz;
    vertices->v.tc[0] = state->texS;
    vertices->v.flag = 0;
    return result;
}

/* Per-frame update: blink timer, texture scroll, emitter flashing and brightness pulse. */
s32 sight_updateBeam(SightBeamObject *obj) {
    SightBeamState *state = &obj->state;
    s16 period;
    LightEffect *midEffect;
    TriggerZone *beam;
    SightEmitter *emitter0;
    SightEmitter *emitter1;
    SightEmitter *emitter2;
    SightEmitter *emitter3;

    if (state->flags & 1) {
        period = state->period;
        state->timer += gFrameTicks;
        while (state->timer >= period) {
            state->timer -= period;
        }
        if (state->timer < state->activeTime) {
            state->flags |= 2;
        } else {
            state->flags &= ~2;
        }
    }
    if (state->flags & 2) {
        state->angleS += state->speedS * gFrameDelta;
        state->angleT += state->speedT * gFrameDelta;
        state->angleS = math_wrapRadians(state->angleS);
        state->angleT = math_wrapRadians(state->angleT);
        state->texS = math_sin(state->angleS) * state->radiusS + state->centerS;
        state->texT = math_sin(state->angleT) * state->radiusT + state->centerT;
        midEffect = state->midEffect;
        if (midEffect != NULL) {
            midEffect->light->disabled = 0;
        }
        beam = state->beam;
        if (beam != NULL && (state->flags & 0x10)) {
            beam->inactive = 0;
        }
    } else {
        midEffect = state->midEffect;
        if (midEffect != NULL) {
            midEffect->light->disabled = 1;
        }
        beam = state->beam;
        if (beam != NULL) {
            beam->inactive = 1;
        }
    }
    state->phase += 0.08000000566f * gFrameDelta;
    while (state->phase > 1.0f) {
        if (state->flags & 2) {
            state->alternate ^= 1;
        }
        state->phase -= 1.0f;
    }
    if (state->alternate) {
        emitter0 = state->emitter0[1];
        emitter0->head.flags &= ~0x20000;
        emitter1 = state->emitter0[2];
        emitter1->head.flags |= 0x20000;
        emitter2 = state->emitter1[1];
        emitter2->head.flags &= ~0x20000;
        emitter3 = state->emitter1[2];
        emitter3->head.flags |= 0x20000;
    } else {
        emitter0 = state->emitter0[1];
        emitter0->head.flags |= 0x20000;
        emitter1 = state->emitter0[2];
        emitter1->head.flags &= ~0x20000;
        emitter2 = state->emitter1[1];
        emitter2->head.flags |= 0x20000;
        emitter3 = state->emitter1[2];
        emitter3->head.flags &= ~0x20000;
    }
    if (state->flags & 8) {
        obj->head.def.unk44 = math_sin(state->pulseAngle) * 112.5f + 142.5f;
        state->pulseAngle = math_wrapRadians(state->pulseAngle + state->pulseSpeed * gFrameDelta);
    }
    return 1;
}

/* Tells sight beams that `actor` moved, so beams keyed to `source` can test it (message 0x20). */
void sight_notifyActorMoved(Actor *actor, TriggerZone *source) {
    struct {
        Actor *actor;
        TriggerZone *source;
    } msg;
    if (actor != NULL) {
        msg.actor = actor;
        msg.source = source;
        particle_broadcastToQuadParticles((s32)&msg, 0x20);
    }
}

/* Message handler of a sight beam object. */
void sight_handleBeamMessage(SightBeamObject *obj, void *msg, u8 msgId) {
    // matching: taking obj's address keeps it in its stack slot, reloaded at each use
    SightBeamObject **objp = &obj;
    SightBeamState *state;

    switch (msgId) {
    case 0x20:
        state = &(*objp)->state;
        if ((state->flags & 0x10) && ((SightTouchMsg *)msg)->source == state->source) {
            if (sight_doesBeamHitActor(((SightTouchMsg *)msg)->actor, *objp) != 0) {
                sight_breakBeam(((SightTouchMsg *)msg)->actor, *objp);
            }
        }
        break;
    case 0x3A:
        state = &(*objp)->state;
        if (((SightCommandMsg *)msg)->source == state->source) {
            switch (((SightCommandMsg *)msg)->command) {
            case 0:
                state->flags |= 2;
                break;
            case 1:
                state->flags &= ~2;
                break;
            case 2:
                sight_breakBeam(NULL, *objp);
                break;
            }
        }
        break;
    }
}

/* Sound callback (registered by sight_spawnBeam): mutes the hum while the beam is off. */
s32 sight_onBeamSound(SightSoundHandle *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s16 *volume) {
    SightBeamState *state = sound->state;
    if (!(state->flags & 2)) {
        *volume = 0;
    }
    return 0;
}

/* Returns whether the beam segment passes through the actor's bounding sphere. */
s32 sight_doesBeamHitActor(Actor *actor, SightBeamObject *obj) {
    s32 kind;
    f32 radius;
    f32 scale;
    Vec3f center;
    f32 halfHeight;
    Vec3f start;
    f32 endX;
    f32 endY;
    f32 endZ;
    Vec3f dir;
    f32 length;
    Vec3f nearHit;
    Vec3f farHit;
    f32 nearT;
    f32 farT;
    s16 height;
    s16 width;
    s16 depth;

    kind = actor->character_type;
    if (kind < 187 && kind != 0xFF) {
        radius = actor->scale_xz * gModelDefs[kind]->unk1A;
    } else {
        radius = 50.0f;
    }
    actor_computeSize(actor, &height, &width, &depth);
    if (height == 0.0f) {
        return 0;
    }
    halfHeight = height * 0.5f;
    center.x = actor->xpos;
    center.y = actor->ypos + halfHeight;
    center.z = actor->zpos;
    scale = radius / halfHeight;
    start.x = obj->head.def.pos.x;
    start.y = (obj->head.def.pos.y - center.y) * scale + center.y;
    start.z = obj->head.def.pos.z;
    endX = obj->head.def.rot.x;
    endY = (obj->head.def.rot.y - center.y) * scale + center.y;
    endZ = obj->head.def.rot.z;
    dir.x = endX - start.x;
    dir.y = endY - start.y;
    dir.z = endZ - start.z;
    {
        f32 invLength;
        if (math_vec3Normalize(&dir, &dir, &length, &invLength) == 0) {
            return 0;
        }
        if (math_raySphereWithin(&start, &dir, &center, radius, length, &nearHit, &farHit, &nearT, &farT) != 0) {
            return 1;
        }
    }
    return 0;
}

const s32 gSightBeamBreakHits[] = { 0x17, 0x18 };
/* matching: compiled with s32 where emissionsource.c's definition has u8 and s16 (args 2, 5-11), so the calls do not narrow */
void particle_spawnImpactMark(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32, s32, s32);

/* Breaks the beam: debris and sparks at both ends, sounds, and (if `actor` tripped it) a reaction. */
void sight_breakBeam(Actor *actor, SightBeamObject *obj) {
    SightBeamState *state;
    SurfaceJetSpray burst;
    Vec3i pos;
    LightEffectHeader header;
    RumbleParams params8868;

    state = &obj->state;
    burst.source.spread = 491.0f;
    burst.spray.speedBase = 0.06049999967f;
    burst.spray.speedRange = 0.0522999987f;
    burst.spray.sizeBase = 30.0f;
    burst.spray.sizeRange = 83.0f;
    burst.spray.countBase = 5;
    burst.spray.countRange = 3;
    burst.spray.particleCountBase = 5;
    burst.spray.particleCountRange = 4;
    burst.spray.delayBase = 18;
    burst.spray.delayRange = 9;
    burst.spray.alphaBase = 100;
    burst.spray.alphaRange = 100;
    burst.spray.fadeStart = 12;
    burst.spray.fadeOutStart = 15;
    burst.spray.eventFunc = 0;
    burst.spray.sizeSlopeBase = 211.0f;
    burst.spray.sizeSlopeRange = 150.0f;
    burst.spray.spread = 21.0f;
    burst.spray.drag = 0.6980000138f;
    burst.spray.gravityBase = -0.4450000226f;
    burst.spray.gravityRange = 0.4410000145f;
    burst.spray.pos = obj->head.def.pos;
    burst.source.normal = obj->head.def.scale;
    burst.source.triangle = state->triangle0;
    particle_spawnSurfaceJetSpray(&burst, obj->head.header.owner, obj->head.header.kind);
    burst.spray.pos = obj->head.def.rot;
    burst.source.normal.x = -obj->head.def.scale.x;
    burst.source.normal.y = -obj->head.def.scale.y;
    burst.source.normal.z = -obj->head.def.scale.z;
    burst.source.triangle = state->triangle1;
    particle_spawnSurfaceJetSpray(&burst, obj->head.header.owner, obj->head.header.kind);
    pos.x = obj->head.def.pos.x;
    pos.y = obj->head.def.pos.y;
    pos.z = obj->head.def.pos.z;
    header.flags = LIGHT_EFFECT_TIMED | LIGHT_EFFECT_OWNS_LIGHT;
    header.update = -1;
    header.lifetime = math_random() % 9U + 17;
    header.lightType = 0;
    light_createWithEffect(&header, &pos, math_random() % 9U + 5, 0xFF, 0x59, 0, 0xFF, 0, 0, obj->head.header.owner, obj->head.header.kind);
    pos.x = obj->head.def.rot.x;
    pos.y = obj->head.def.rot.y;
    pos.z = obj->head.def.rot.z;
    light_createWithEffect(&header, &pos, math_random() % 9U + 5, 0xFF, 0x59, 0, 0xFF, 0, 0, obj->head.header.owner, obj->head.header.kind);
    particle_spawnImpactMark(math_randomFloat() * 101.0f + 55.0f, (u8)(math_random() % 66U + 190), &state->triangle0, &obj->head.def.pos, -1, 1, 1, 0, 0, 0, obj->head.header.owner, 0);
    particle_spawnImpactMark(math_randomFloat() * 101.0f + 55.0f, (u8)(math_random() % 66U + 190), &state->triangle1, &obj->head.def.rot, -1, 1, 1, 0, 0, 0, obj->head.header.owner, 0);
    if (actor != NULL) {
        camera_startWobblePreset(2, actor->CameraPtr->CNumber, 0, obj->head.header.owner, obj->head.header.kind);
        params8868.flags = 1;
        params8868.duration = 40;
        params8868.playerMask = 1;
        params8868.strength = 8;
        params8868.updateKind = -1;
        cont_spawnRumble(&params8868, 0, 0xFF, 0);
    }
    if (actor != NULL) {
        actor_hitScripted(actor, gSightBeamBreakHits[state->unk94], 0, -1);
    }
    sound_playAt(0x2F5, 0x7FFF, math_random() % 500U, 0, 0, obj->head.def.pos.x, obj->head.def.pos.y, obj->head.def.pos.z, 2000, 2200);
    sound_addDynamicSound(0x2F6, obj->head.def.rot.x, obj->head.def.rot.y, obj->head.def.rot.z, 0x7FFF, 2200, 2000, (SoundRecordCallback)sound_delayedPlayCallback, (void *)5, 0, 0, math_random() % 500U);
    effect_destroyObject(&obj->head.header);
}

/* Spawns a beam extension effect from `start` to `end`, owned by beam object `owner`. */
SightExtension *sight_spawnExtension(SightBeamObject *owner, Vec3f *start, Vec3f *end, u8 red, u8 green, u8 blue, ZoneNode *zone, u8 zoneMasked, u8 arg8, s32 arg9) {
    SightExtension *effect;
    GlowShape params;
    ParticleDef desc;
    SightBeamObject *ownerCopy; // matching: copied to the stack before the memcpy below

    desc.pos = *start;
    desc.rot = *end;
    ownerCopy = owner;
    params.width1 = params.height1 = 30.10000038f;
    params.width0 = params.height0 = 40.0f;
    params.alpha1 = 1.0f;
    params.offset1 = 0.0f;
    params.offset0 = 0.0f;
    desc.texture = 0x15;
    desc.renderModes = 0x3403;
    desc.lifetime = 300;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.rgba[0] = red;
    params.alpha0 = 0.3000000119f;
    desc.rgba[1] = green;
    desc.rgba[2] = blue;
    desc.rgba[3] = 0xFF;
    desc.width = 0.0f;
    desc.height = 0.0f;
    desc.scale.x = 1.0f;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    desc.flags = (zoneMasked ? 0x01000000 : 0) | 0x024D2006 | 0x40000000;
    desc.unk45 = 0xFF;
    desc.unk44 = 0xFF;
    desc.unk46 = 0;
    desc.unk47 = 6;
    desc.type = 3;
    params.visibility[0] = 0;
    params.visibility[1] = 0;
    params.visibility[2] = 0;
    params.visibility[3] = 0;
    params.scale = 1.0f;
    params.type = 0;
    desc.unk48 = zone;
    params.fadeRate = 9.999999975e-07f;
    effect = flare_spawnGlow(&params, 100, &desc, 0, 0, 0, 0, 1, arg8, arg9);
    if (effect != NULL) {
        memcpy(&effect->owner, &ownerCopy, sizeof(ownerCopy));
    }
    return effect;
}

/* Spawns the three parts of a beam emitter on the surface triangle at `pos`. */
void sight_spawnEmitter(SightEmitter **emitters, Vec3f *pos, CollisionTri *triangle, Vec3f *dir, ZoneNode *zone, u8 altColors, u8 zoneMasked, u8 arg7, s32 arg8) {
    Vec3f edge0;
    Vec3f edge1;
    Vec3f normal;
    s32 i;
    ParticleSpawn78 desc;
    void *unused; // matching: reserves a stack slot
    SightEmitterAnim anim;

    if (math_getTriEdges(triangle, &edge0, &edge1, &normal) != 0) {
        math_getHeadingElevation(&normal, &desc.rot.y, &desc.rot.x);
        desc.radius = 1.0f;
        desc.elasticity = 1.0f;
        desc.scale.x = 1.0f;
        desc.scale.y = 1.0f;
        desc.rot.x += 90.0f;
        desc.scale.z = 1.0f;
        desc.sizeY = 0.4000000059604645f;
        desc.sizeXZ = 0.4000000059604645f;
        desc.rot.z = 0.0f;
        desc.pos = *pos;
        desc.vel.x = 0.0f;
        desc.vel.y = 0.0f;
        desc.vel.z = 0.0f;
        desc.rotVel.x = 0.0f;
        desc.rotVel.y = 0.0f;
        desc.rotVel.z = 0.0f;
        desc.gravity = 0.0f;
        desc.flags = (zoneMasked ? 0x400 : 0) | 0x100 | 0x800 | 0x10000 | 0x4000 | 0x40000 | 0x80000;
        desc.lifetime = 300;
        desc.model = 0x36;
        desc.variant = 0;
        desc.unk5C = zone;
        desc.alpha = 0xFF;
        desc.updateFn = 0;
        desc.update2Fn = 0;
        desc.floorHitFn = 0;
        desc.floorType3Fn = 0;
        desc.wallHitFn = 0;
        desc.destroyFn = 0;
        desc.squashFn = 0;
        desc.matrixFn = 2;
        desc.unk6A = 0;
        desc.owner = 0;
        desc.ownerId = 0;
        desc.fadeStart = 1;
        desc.fadeRate = 0xFF;
        desc.drawFn = -1;
        emitters[0] = particle_newModelParticle(&desc, 3, 0xFF, 0, arg7, arg8);
        if (emitters[0] != NULL) {
            for (i = 0; i < 2; i++) {
                particle_buildUniformMatrix(&emitters[0]->head.mtx[i], &emitters[0]->head);
            }
        }
        desc.drawFn = 2;
        anim.unk0 = 2;
        anim.unk6 = 4;
        anim.unkE = 6;
        anim.unk8 = 0x3E;
        anim.unk10 = 0x3E;
        if (altColors) {
            anim.unk4 = 0x87;
            anim.unkC = 0x88;
        } else {
            anim.unk4 = 0x8B;
            anim.unkC = 0x8C;
        }
        desc.model = 0x37;
        desc.matrixFn = 0;
        emitters[1] = particle_newModelParticle(&desc, 3, 0xFF, 0x14, arg7, arg8);
        if (emitters[1] != NULL) {
            for (i = 0; i < 2; i++) {
                particle_buildUniformMatrix(&emitters[1]->head.mtx[i], &emitters[1]->head);
            }
            memcpy(&emitters[1]->anim, &anim, sizeof(SightEmitterAnim));
        }
        desc.model = 0x38;
        if (altColors) {
            anim.unk4 = 0x89;
            anim.unkC = 0x8A;
        } else {
            anim.unk4 = 0x8D;
            anim.unkC = 0x8E;
        }
        emitters[2] = particle_newModelParticle(&desc, 3, 0xFF, 0x14, arg7, arg8);
        if (emitters[2] != NULL) {
            for (i = 0; i < 2; i++) {
                particle_buildUniformMatrix(&emitters[2]->head.mtx[i], &emitters[2]->head);
            }
            memcpy(&emitters[2]->anim, &anim, sizeof(SightEmitterAnim));
        }
    }
}

/* Frees the beam's effects and emitters, then removes the beam object with particle_destroyQuadParticleDefault. */
void sight_destroyBeam(SightBeamObject *obj) {
    // matching: one variable first holds the object, then its state (as IDO allocated it)
    SightBeamState *state;
    void *part;

    state = &obj->state;
    if (obj->state.midEffect != NULL) {
        effect_destroyObject(state->midEffect);
        sound_stopByCallback((SoundRecordCallback)sight_onBeamSound, state, 0);
    }
    state = *(SightBeamState **)&obj; // matching: read through &obj so IDO reloads it into this register
    part = ((SightBeamObject *)state)->state.endEffect0;
    state = &((SightBeamObject *)state)->state;
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->endEffect1;
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter0[0];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter0[1];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter0[2];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter1[0];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter1[1];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter1[2];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    particle_destroyQuadParticleDefault(&obj->head);
}

/* Same as sight_destroyBeam, but finishes with particle_freeQuadParticleDefault. */
void sight_freeBeam(SightBeamObject *obj) {
    // matching: one variable first holds the object, then its state (as IDO allocated it)
    SightBeamState *state;
    void *part;

    state = &obj->state;
    if (obj->state.midEffect != NULL) {
        effect_destroyObject(state->midEffect);
        sound_stopByCallback((SoundRecordCallback)sight_onBeamSound, state, 0);
    }
    state = *(SightBeamState **)&obj; // matching: read through &obj so IDO reloads it into this register
    part = ((SightBeamObject *)state)->state.endEffect0;
    state = &((SightBeamObject *)state)->state;
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->endEffect1;
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter0[0];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter0[1];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter0[2];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter1[0];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter1[1];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    part = state->emitter1[2];
    if (part != NULL) {
        effect_destroyObject(part);
    }
    particle_freeQuadParticleDefault(&obj->head);
}

/* Returns whether the beam that owns this extension effect is switched on. */
s32 sight_isExtensionOn(SightExtension *effect, s32 arg1) {
    SightBeamState *state;

    state = &effect->owner->state;
    if (state->flags & 2) {
        return 1;
    }
    return 0;
}

/* Sends `command` (see SightCommandMsg) to the sight beams keyed to `source`. */
void sight_sendBeamCommand(s32 command, TriggerZone *source) {
    struct {
        TriggerZone *source;
        u8 command;
    } msg;
    msg.source = source;
    msg.command = command;
    particle_broadcastToQuadParticles((s32)&msg, 0x3A);
}

/* Explosion at an actor's position: ground scorch, smoke, debris and sound. */
void sight_explodeAtActor(Actor *actor, u8 arg1, s32 arg2) {
    Vec3f origin;
    Vec3f impact;
    u8 hit;
    u8 airborne;
    Vec3f query;
    CollisionHit collision;

    if (actor == NULL) {
        return;
    }
    origin.x = actor->xpos;
    origin.y = actor->ypos + 135.0f;
    origin.z = actor->zpos;
    hit = 0;
    airborne = actor->unk2D8[0] != 0.0f;
    if (!airborne) {
        query.x = origin.x;
        query.y = origin.y;
        query.z = origin.z;
        collision_initHitFromObject(&collision, actor);
        if (collision_probeFloorSurfABSet0Cached(&query, 0, origin.y - 200.0f, &collision) != 0 && (collision.flags & 1)) {
            impact.x = query.x;
            hit = 1;
            impact.y = collision.height;
            impact.z = query.z;
            particle_spawnImpactMark(math_randomFloat() * 125.0f + 204.0f, (math_random() % 101U + 155) & 0xFF,
                          &collision.tri, &impact, math_random() % 302U + 500, 0, 1, 0, 0, 0, arg1, 0);
        }
    }
    particle_startPlayerRumbles(&origin, 506.0f, 1013.0f, 0.000987166888f, 15, 20, arg1, arg2);
    particle_startPlayerCameraWobbles(&origin, 506.0f, 1013.0f, 0.000987166888f, 5, arg1, arg2);
    particle_spawnBlastSpray(&origin, arg1, arg2);
    particle_spawnLightFlash(&origin, arg1, arg2);
    particle_startRandomEmitter(hit ? &impact : &origin, math_random() % 62U + 120, arg1, arg2);
    particle_spawnGrowFade(&origin, 2.0f * math_randomFloat() + 8.0f, 2.4641099f,
        math_random() % 56U + 200, 0.0f, arg1, arg2);
    if (!airborne) {
        blood_spawnHitEffects(actor, 0, 0, 1, 0, 0, arg1, arg2);
    }
    sound_playAt(0x60D, 0x7D00, 0, 0, 0, origin.x,
        origin.y, origin.z, 500, 1000);
    if (airborne) {
        sight_spawnFloorImpact(actor, arg1, arg2);
    }
}

/* Spawns effect_spawnImpact's effect (type 2) just above the actor's floor height. */
void sight_spawnFloorImpact(Actor *actor, u8 arg1, s32 arg2) {
    Vec3f pos;
    CollisionHit collision;

    pos.x = actor->xpos;
    pos.y = actor->floorY + 2.0f;
    pos.z = actor->zpos;
    collision_initHitFromObject(&collision, actor);
    effect_spawnImpact(&pos, &collision, 2, arg1, arg2);
}

/* Light effect update: turns the light off while its status block's unk6F is set. */
s32 sight_updateStatusLight(LightEffect *obj) {
    SightStatusBlock *status = obj->data.object;
    if (status->unk6F == 0) {
        obj->light->disabled = 0;
    } else {
        obj->light->disabled = 1;
    }
    return 1;
}

/* Reports through `visible` whether the status block's unk6F is clear; clears flag 2 otherwise. */
s32 func_151C9B64(SightStatusObject *obj, u8 *visible) {
    SightStatusBlock *status = obj->status;
    if (status->unk6F == 0) {
        *visible = 1;
    } else {
        obj->flags &= ~2;
        *visible = 0;
    }
    return 1;
}

