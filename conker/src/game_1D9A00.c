#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151D9014 (game_2062D0.c) is declared in each file that calls it: game/Beam/ModelBeam/modelbeam.c and game/Effects/Blood/blood.c
// were compiled with a different view of it.
SteamParticle *func_151D9014(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

/* Calls func_151DBCBC at the current sample's (x, z) at the given height, then sets the payload's
 * unk20 to 4. */
s32 func_151AC550(SampleTrail *state, f32 x, f32 y, f32 z, f32 height, CollisionTri *surface) {
    EffectPayload *payload = state->payload;
    EffectSample *samples = state->samples;
    Vec3f position;

    position.x = samples[state->head.start].x;
    position.y = height;
    position.z = samples[state->head.start].z;
    func_151DBCBC(func_151D8E20(), payload->unk0 * 7.0f, payload->unk1B, surface, &position,
                  state->head.header.owner, state->head.header.kind);
    payload->unk20 = 4;
    return 1;
}

/*
 * Emitter callback (see D_8008AA00): spawns one func_151D9014 particle at (x, y, z) moving
 * along `yaw` with a random pitch and speed, using the randomized parameters of `kind`.
 */
s32 func_151AC61C(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY,
                  f32 originZ, s32 yaw, s32 baseYaw, f32 radius, s32 kind, f32 arg12,
                  EmitterOwner *owner, u8 arg14) {
    EmitterOwner *emitter; // matching: the owner copy, scale and entry each reserve the frame slots IDO used
    f32 scale;
    const s8 *entry;
    Vec3f position;
    Vec3f velocity;
    const EmitterConfig *config;

    emitter = owner;
    scale = 1.0f;
    entry = &gImpactSets[kind].emitter;
    config = &gImpactEmitterConfigs[*entry];
    position.x = x;
    position.y = y;
    position.z = z;
    func_15143794(yaw, (func_150ADA20() % (config->pitchRange + 1)) + config->pitch,
                  func_150ADA68() * config->speedRange + config->speed, &velocity);
    func_151D9014(&position, &velocity, emitter->unk8,
                  func_150ADA68() * config->unk20Range + config->unk20,
                  (func_150ADA20() % (config->unk10Range + 1)) + config->unk10,
                  (func_150ADA20() % (config->unk14Range + 1)) + config->unk14,
                  func_150ADA68() * config->unk18Range + config->unk18,
                  func_150ADA68() < config->unk28Chance, scale, scale, 1, emitter->ground, 1, 0, arg14,
                  emitter->unk4);
    return 1;
}

/*
 * Returns this view's 4 vertices for the quad (initializing both buffers from the template
 * when freshly allocated), placed at the particle's position and rotated by the view's
 * D_800DD1D8/D_800DD1E8 factors; NULL if no buffer is available.
 */
Vtx *func_151AC810(QuadParticle *particle, s16 view) {
    Vtx *vtx;
    Vtx *result;
    f32 width;
    f32 height;
    f32 offsetZ;
    f32 offsetX;
    u8 fresh;

    func_151D5D60(particle->buffers, view, 0x40, &vtx, &fresh);
    result = vtx;
    if (vtx != NULL) {
        if (fresh) {
            memcpy(particle->buffers[view], particle->template, 0x40);
            memcpy(particle->buffers[view] + 4, particle->template, 0x40);
        }
    } else {
        return NULL;
    }

    width = particle->width * particle->unk4C;
    height = particle->height * particle->unk50;
    offsetZ = D_800DD1D8[view] * width;
    offsetX = D_800DD1E8[view] * width;

    vtx[0].v.flag = 0;
    vtx[1].v.flag = 0;
    vtx[2].v.flag = 0;
    vtx[3].v.flag = 0;
    vtx[0].v.ob[0] = vtx[3].v.ob[0] = particle->x + offsetX;
    vtx[0].v.ob[1] = vtx[1].v.ob[1] = particle->y;
    vtx[0].v.ob[2] = vtx[3].v.ob[2] = particle->z - offsetZ;
    vtx[1].v.ob[0] = vtx[2].v.ob[0] = particle->x - offsetX;
    vtx[2].v.ob[1] = vtx[3].v.ob[1] = particle->y + height;
    vtx[1].v.ob[2] = vtx[2].v.ob[2] = particle->z + offsetZ;

    return result;
}

/* Grows the quad's width and height by unk4C per second (gFrameDelta = frame time). */
s32 func_151AC9EC(QuadParticle *particle) {
    f32 growth = particle->unk4C * gFrameDelta;

    particle->width += growth;
    particle->height += growth;
    return 1;
}

/* Caps unk5C at unk1C * 16 once unk1C drops below 16. */
s32 func_151ACA20(QuadParticle *particle) {
    s16 limit = 255;

    if (particle->unk1C < 16) {
        limit = particle->unk1C * 16;
    }
    if (limit < particle->unk5C) {
        particle->unk5C = limit;
    }
    return 1;
}
