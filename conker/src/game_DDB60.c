#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"


extern s32 func_15143E24(Actor *);
extern void func_1504715C(CollisionHit *, Actor *);
extern void func_151C329C(void *, u8, s32);
extern void func_15152190(DebrisBurst *, const s32 *, const f32 *, s32, f32, s32, u8, s32);
extern void func_15153F18(AngleRange *, SplashSpray *, CollisionHit *, u8, s32);

extern const s32 D_8009F810[1];
extern const f32 D_8009F814[1];
extern const s32 D_8009F818[1];
extern const f32 D_8009F81C[1];

/*
 * Spawns effects at actor's position, aimed by func_15143E24(other): actor id 0xA6 gets func_151C329C
 * 50 units above it and one func_15152190 burst; any other actor gets a different burst plus two
 * func_15153F18 passes (unk3C = 0, then 1) using the func_1504715C info for actor.
 */
void func_150B06B0(Actor *actor, Actor *other, u8 arg2, s32 arg3) {
    Vec3f pos;
    s8 angle;
    CollisionHit floor;

    pos.x = actor->x_position;
    pos.y = actor->y_position;
    pos.z = actor->z_position;
    angle = func_15143E24(other) + 0x40;
    func_1504715C(&floor, actor);
    if (actor->id == 0xA6) {
        Vec3f raisedPos;
        DebrisBurst burst;

        raisedPos.x = pos.x;
        raisedPos.y = pos.y + 50.0f;
        raisedPos.z = pos.z;
        func_151C329C(&raisedPos, arg2, arg3);
        burst.countBase = 10;
        burst.countRange = 10;
        burst.pos = pos;
        burst.speedBase = 7.0f;
        burst.speedRange = 17.0f;
        burst.angles.yawBase = angle - 40;
        burst.angles.yawRange = 80;
        burst.angles.pitchBase = -45;
        burst.angles.pitchRange = 26;
        burst.lifeBase = 25;
        burst.lifeRange = 25;
        burst.gravityBase = -0.8000000119f;
        burst.gravityRange = 0.3440000117f;
        burst.scaleBase = 0.7070000172f;
        burst.scaleRange = 0.5070000291f;
        burst.spin = 26.41100121f;
        func_15152190(&burst, D_8009F810, D_8009F814, 1, 0.0f, 0, arg2, arg3);
    } else {
        {
            DebrisBurst burst;

            burst.countBase = 10;
            burst.countRange = 10;
            burst.pos = pos;
            burst.speedBase = 7.0f;
            burst.speedRange = 17.0f;
            burst.angles.yawBase = angle - 40;
            burst.angles.yawRange = 80;
            burst.angles.pitchBase = -21;
            burst.angles.pitchRange = 13;
            burst.lifeBase = 25;
            burst.lifeRange = 25;
            burst.gravityBase = -0.8000000119f;
            burst.gravityRange = 0.3440000117f;
            burst.scaleBase = 0.04900000244f;
            burst.scaleRange = 0.1020000055f;
            burst.spin = 26.41100121f;
            func_15152190(&burst, D_8009F818, D_8009F81C, 1, 0.0f, 0, arg2, arg3);
        }
        {
            SplashSpraySpec spray;

            spray.spray.pos = pos;
            spray.spray.scaleBase = 3.049999952f;
            spray.spray.countBase = 7;
            spray.spray.countRange = 3;
            spray.angles.yawBase = angle - 60;
            spray.angles.yawRange = 120;
            spray.angles.pitchBase = -30;
            spray.angles.pitchRange = 16;
            spray.spray.itemCountBase = 3;
            spray.spray.itemCountRange = 2;
            spray.spray.lifeBase = 20;
            spray.spray.lifeRange = 20;
            spray.spray.unk30 = 155;
            spray.spray.unk32 = 100;
            spray.spray.unk3C = 16;
            spray.spray.unk3E = 15;
            spray.spray.unk40 = 0;
            spray.spray.kind = 0;
            spray.spray.scaleRange = 5.009999752f;
            spray.spray.sizeBase = 0.7000000477f;
            spray.spray.sizeRange = 0.5010000467f;
            spray.spray.speedBase = 15.10000038f;
            spray.spray.speedRange = 20.80000114f;
            spray.spray.unk38 = 0.5f;
            func_15153F18(&spray.angles, &spray.spray, &floor, arg2, arg3);
            spray.spray.kind = 1;
            func_15153F18(&spray.angles, &spray.spray, &floor, arg2, arg3);
        }
    }
}

/*
 * Picks a random spawn point on a 248-unit perimeter around owner: 122 units along the back edge
 * (-31.5 along forward), or 63 units along either side (61 out along right). Writes the point to outPos
 * (y taken from player->unk118) and a matching s16 yaw to outAngle.
 */
void func_150B0A60(Actor *owner, Actor *player, f32 *forward, f32 *right, f32 *outPos, s16 *outAngle) {
    f32 along;
    f32 spread;
    f32 side;

    *outAngle = (s16)(owner->unk40 * 0.7111111283f - 128.0f);
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
    outPos[0] += owner->x_position;
    outPos[1] += player->unk118;
    outPos[2] += owner->z_position;
}

/* State a DDB60Effect keeps at +0x28: func_150B0C58 fills it, func_150B0D20 updates it each frame. */
typedef struct {
    /* 0x00 */ Actor *owner;
    /* 0x04 */ u8 ownerId; // owner->unique_id when attached; a mismatch means the slot was reused
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 timer;  // one particle is emitted per 1.0 accumulated
} DDB60EffectData;

/* The effect object spawned by func_150B0C58. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ DDB60EffectData data;
} DDB60Effect;

void func_150B0C58(Actor *owner, u8 arg1, s32 arg2);

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

extern f32 D_8009F860[];
extern f32 D_8009F868[];
s32 func_1515C0F8(Actor *, f32 **);
void func_1514373C(f32, f32, f32 *, f32 *);
void func_15143794(s16, s16, f32, f32 *);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);

/*
 * DDB60Effect update: while the owner lives and player movement_obj[owner->unk124] has unk31C byte 0x4F == 2,
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
    f32 pos[3];
    f32 vel[3];
    s16 angle;

    owner = effect->data.owner;
    data = &effect->data;
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->base.header.state = -1;
        return;
    }
    idx = owner->unk124;
    emit = 0;
    if (movement_obj[idx].unk31C->transport_surface == 2) {
        speed = owner->xz_velocity;
        minSpeed = movement_obj[idx].unk44 * 1.350000024f;
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
            func_1514373C(owner->unk40 * 0.01745329238f, 1.0f, &forward[0], &forward[1]);
            right[0] = forward[1];
            right[1] = -forward[0];
            do {
                func_150B0A60(owner, &movement_obj[idx], forward, right, pos, &angle);
                func_15143794(angle, (func_150ADA20() % 26) - 41, func_150ADA68() * 3.769999981f + 4.059999943f, vel);
                vel[0] -= refVel->x * 0.9250000715f;
                vel[2] -= refVel->z * 0.9250000715f;
                func_151D9014(pos, vel, 5, func_150ADA68() * 0.1780000031f + -0.4420000315f,
                              (func_150ADA20() % 21) + 20, (func_150ADA20() % 101) + 155,
                              func_150ADA68() * 50.0f + 80.0f, func_150ADA68() < 0.04400000349f, 1.399999976f,
                              1.399999976f, 1, 0, 1, 0, effect->base.header.owner, effect->base.header.kind);
                data->timer -= 1.0f;
            } while (data->timer > 1.0f);
        }
    }
}
