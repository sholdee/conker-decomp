#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// particle_spawnSteamParticle (game/Particle/Steam/steam.c) is declared in each file that calls it: game/Beam/ModelBeam/modelbeam.c and game/Effects/Blood/blood.c
// were compiled with a different view of it.
SteamParticle *particle_spawnSteamParticle(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);




/*
 * Picks a random spawn point on a 248-unit perimeter around owner: 122 units along the back edge
 * (-31.5 along forward), or 63 units along either side (61 out along right). Writes the point to outPos
 * (y taken from player->waterHeight) and a matching s16 yaw to outAngle.
 */
void effect_pickPerimeterPoint(Actor *owner, Actor *player, f32 *forward, f32 *right, Vec3f *outPos, s16 *outAngle) {
    f32 along;
    f32 spread;
    f32 side;

    *outAngle = owner->theta * 0.7111111283f - 128.0f;
    along = math_randomFloat() * 248.0f;
    spread = math_randomFloat() + -0.5f;
    if (along < 122.0f) {
        outPos->x = forward[0] * -31.5f + (right[0] * spread) * 122.0f;
        outPos->y = 0.0f;
        outPos->z = forward[1] * -31.5f + (right[1] * spread) * 122.0f;
    } else {
        f32 sideLength = 63.0f; // matching: a named local, not the literal, keeps 63.0f in one register

        side = 61.0f;
        along = along - 122.0f;
        if (along < sideLength) {
            side = -side;
            *outAngle += 0x40;
        } else {
            *outAngle -= 0x40;
        }
        outPos->x = forward[0] * sideLength * spread + right[0] * side;
        outPos->y = 0.0f;
        outPos->z = forward[1] * sideLength * spread + right[1] * side;
    }
    outPos->x += owner->xpos;
    outPos->y += player->waterHeight;
    outPos->z += owner->zpos;
}

/* effect_spawnPerimeterEmitter with the default arguments (0xFF, 1). */
void effect_attachPerimeterEmitter(Actor *owner) {
    effect_spawnPerimeterEmitter(owner, 0xFF, 1);
}

/* Attaches a DDB60Effect (updated by effect_updatePerimeterEmitter) to owner. */
void effect_spawnPerimeterEmitter(Actor *owner, u8 arg1, s32 arg2) {
    DDB60EffectData data;
    DDB60Effect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.timer = 0.0f;

    effect = effect_newTimedEffect(300, -1, 0x58, -1, 0, 0x43, sizeof(data), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Effect callback: hands effect_handleOwnerActorEvent the owner reference. */
void effect_perimeterEmitterMessage(DDB60Effect *effect, s32 arg1, u8 arg2) {
    effect_handleOwnerActorEvent(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

/* Emit-rate base and random range per emitter. */
static const f32 gPerimeterEmitRates[] = { 1.0f, 1.259000063f };
static const f32 gPerimeterEmitRateRanges[] = { 0.0f, 1.078999996f };

/*
 * DDB60Effect update: while the owner lives and player movement_obj[owner->unk124] has mv->transport_surface == 2,
 * emits particles around the owner when its speed (at least 1.35 * player->unk44) exceeds 26.
 */
void effect_updatePerimeterEmitter(DDB60Effect *effect) {
    DDB60EffectData *data;
    Actor *owner;
    s32 emit;
    f32 speed;
    f32 minSpeed;
    s32 idx;
    f32 forward[2];
    f32 right[2];
    const Vec3f *refVel;
    Vec3f pos;
    Vec3f vel;
    s16 angle;

    owner = effect->data.owner;
    data = &effect->data;
    if (owner->type == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return;
    }
    idx = owner->owner;
    emit = 0;
    if (movement_obj[idx].mv->transport_surface == 2) {
        speed = owner->speed;
        minSpeed = movement_obj[idx].stickspeed * 1.350000024f;
        if (speed < minSpeed) {
            speed = minSpeed;
        }
        if (speed > 26.0f) {
            if (1) { // matching: the extra block shifts IDO's register priorities (data stays in $s4)
                emit = 1;
            }
        }
    }
    if (emit) {
        data->timer += (gPerimeterEmitRates[emit] + math_randomFloat() * gPerimeterEmitRateRanges[emit]) * gFrameDelta;
        if (data->timer > 1.0f) {
            if (sprite_findActorMotion(owner, &refVel) == 0) {
                refVel = &gZeroVec3f;
            }
            math_dirFromAngleXZ(owner->theta * 0.01745329238f, 1.0f, &forward[0], &forward[1]);
            right[0] = forward[1];
            right[1] = -forward[0];
            do {
                effect_pickPerimeterPoint(owner, &movement_obj[idx], forward, right, &pos, &angle);
                math_dirFromBinAngles(angle, (math_random() % 26) - 41, math_randomFloat() * 3.769999981f + 4.059999943f, &vel);
                vel.x -= refVel->x * 0.9250000715f;
                vel.z -= refVel->z * 0.9250000715f;
                particle_spawnSteamParticle(&pos, &vel, 5, math_randomFloat() * 0.1780000031f + -0.4420000315f,
                              (math_random() % 21) + 20, (math_random() % 101) + 155,
                              math_randomFloat() * 50.0f + 80.0f, math_randomFloat() < 0.04400000349f, 1.399999976f,
                              1.399999976f, 1, 0, 1, 0, effect->base.header.owner, effect->base.header.kind);
                data->timer -= 1.0f;
            } while (data->timer > 1.0f);
        }
    }
}
