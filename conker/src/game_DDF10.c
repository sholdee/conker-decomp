#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151D9014 (game_2062D0.c) is declared in each file that calls it: game_1228D0.c, game_161520.c and
// game_1DC720.c were compiled with a different view of it.
SteamParticle *func_151D9014(Vec3f *pos, Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);




/*
 * Picks a random spawn point on a 248-unit perimeter around owner: 122 units along the back edge
 * (-31.5 along forward), or 63 units along either side (61 out along right). Writes the point to outPos
 * (y taken from player->unk118) and a matching s16 yaw to outAngle.
 */
void func_150B0A60(Actor *owner, Actor *player, f32 *forward, f32 *right, f32 *outPos, s16 *outAngle) {
    f32 along;
    f32 spread;
    f32 side;

    *outAngle = (s16)(owner->theta * 0.7111111283f - 128.0f);
    along = func_150ADA68() * 248.0f;
    spread = func_150ADA68() + -0.5f;
    if (along < 122.0f) {
        outPos[0] = forward[0] * -31.5f + (right[0] * spread) * 122.0f;
        outPos[1] = 0.0f;
        outPos[2] = forward[1] * -31.5f + (right[1] * spread) * 122.0f;
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
        outPos[0] = forward[0] * sideLength * spread + right[0] * side;
        outPos[1] = 0.0f;
        outPos[2] = forward[1] * sideLength * spread + right[1] * side;
    }
    outPos[0] += owner->xpos;
    outPos[1] += player->unk118;
    outPos[2] += owner->zpos;
}

/* func_150B0C58 with the default arguments (0xFF, 1). */
void func_150B0C34(Actor *owner) {
    func_150B0C58(owner, 0xFF, 1);
}

/* Attaches a DDB60Effect (updated by func_150B0D20) to owner. */
void func_150B0C58(Actor *owner, u8 arg1, s32 arg2) {
    DDB60EffectData data;
    DDB60Effect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.timer = 0.0f;

    effect = (DDB60Effect *)func_15149130(300, -1, 0x58, -1, 0, 0x43, sizeof(data), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Effect callback: hands func_15169850 the owner reference. */
void func_150B0CE0(DDB60Effect *effect, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

/* Emit-rate base and random range per emitter. */
const f32 D_8009F860[2] = { 1.0f, 1.259000063f };
const f32 D_8009F868[2] = { 0.0f, 1.078999996f };

/*
 * DDB60Effect update: while the owner lives and player movement_obj[owner->unk124] has mv->transport_surface == 2,
 * emits particles around the owner when its speed (at least 1.35 * player->unk44) exceeds 26.
 */
void func_150B0D20(DDB60Effect *effect) {
    DDB60EffectData *data;
    Actor *owner;
    s32 emit;
    f32 speed;
    f32 minSpeed;
    s32 idx;
    f32 forward[2];
    f32 right[2];
    Vec3f *refVel;
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
        data->timer += (D_8009F860[emit] + func_150ADA68() * D_8009F868[emit]) * D_800BE9A4;
        if (data->timer > 1.0f) {
            if (func_1515C0F8(owner, &refVel) == 0) {
                refVel = &D_800A5480;
            }
            func_1514373C(owner->theta * 0.01745329238f, 1.0f, &forward[0], &forward[1]);
            right[0] = forward[1];
            right[1] = -forward[0];
            do {
                func_150B0A60(owner, &movement_obj[idx], forward, right, &pos, &angle);
                func_15143794(angle, (func_150ADA20() % 26) - 41, func_150ADA68() * 3.769999981f + 4.059999943f, &vel);
                vel.x -= refVel->x * 0.9250000715f;
                vel.z -= refVel->z * 0.9250000715f;
                func_151D9014(&pos, &vel, 5, func_150ADA68() * 0.1780000031f + -0.4420000315f,
                              (func_150ADA20() % 21) + 20, (func_150ADA20() % 101) + 155,
                              func_150ADA68() * 50.0f + 80.0f, func_150ADA68() < 0.04400000349f, 1.399999976f,
                              1.399999976f, 1, 0, 1, 0, effect->base.header.owner, effect->base.header.kind);
                data->timer -= 1.0f;
            } while (data->timer > 1.0f);
        }
    }
}
