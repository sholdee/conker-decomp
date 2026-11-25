#include <ultra64.h>
/* functions.h declares func_150ADA20 as returning u8, but it returns the full 32-bit random value
 * (callers take it modulo n with divu), so its header declaration is renamed out of the way. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


/* Parameter block of func_15152190. */
typedef struct {
    s32 unk00;
    s32 unk04;
    struct17 unk08; // position
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    f32 unk30;
    f32 unk34;
    f32 unk38;
} Arg150B06B0_2190;

/* Parameter block of func_15153F18 (which is also passed &unk08 separately). */
typedef struct {
    s16 unk00;
    s16 unk02;
    s16 unk04;
    s16 unk06;
    struct17 unk08; // position
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
    s8 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    s32 unk48;
} Arg150B06B0_3F18;

/* Filled in by func_1504715C for an actor; only passed on to func_15153F18 here. */
typedef struct {
    u8 pad[0x24];
} Floor150B06B0;

extern s32 func_15143E24(struct127 *);
extern void func_1504715C(Floor150B06B0 *, struct127 *);
extern void func_151C329C(void *, u8, s32);
extern void func_15152190(Arg150B06B0_2190 *, const s32 *, const f32 *, s32, f32, s32, u8, s32);
extern void func_15153F18(Arg150B06B0_3F18 *, struct17 *, Floor150B06B0 *, u8, s32);

extern const s32 D_8009F810[1];
extern const f32 D_8009F814[1];
extern const s32 D_8009F818[1];
extern const f32 D_8009F81C[1];

/*
 * Spawns effects at actor's position, aimed by func_15143E24(other): actor id 0xA6 gets func_151C329C
 * 50 units above it and one func_15152190 burst; any other actor gets a different burst plus two
 * func_15153F18 passes (unk3C = 0, then 1) using the func_1504715C info for actor.
 */
void func_150B06B0(struct127 *actor, struct127 *other, u8 arg2, s32 arg3) {
    struct17 pos;
    s8 angle;
    Floor150B06B0 floor;

    pos.unk0 = actor->x_position;
    pos.unk4 = actor->y_position;
    pos.unk8 = actor->z_position;
    angle = func_15143E24(other) + 0x40;
    func_1504715C(&floor, actor);
    if (actor->id == 0xA6) {
        struct17 raisedPos;
        Arg150B06B0_2190 burst;

        raisedPos.unk0 = pos.unk0;
        raisedPos.unk4 = pos.unk4 + 50.0f;
        raisedPos.unk8 = pos.unk8;
        func_151C329C(&raisedPos, arg2, arg3);
        burst.unk00 = 10;
        burst.unk04 = 10;
        burst.unk08 = pos;
        burst.unk1C = 7.0f;
        burst.unk20 = 17.0f;
        burst.unk14 = angle - 40;
        burst.unk16 = 80;
        burst.unk18 = -45;
        burst.unk1A = 26;
        burst.unk2C = 25;
        burst.unk2E = 25;
        burst.unk24 = -0.8000000119f;
        burst.unk28 = 0.3440000117f;
        burst.unk30 = 0.7070000172f;
        burst.unk34 = 0.5070000291f;
        burst.unk38 = 26.41100121f;
        func_15152190(&burst, D_8009F810, D_8009F814, 1, 0.0f, 0, arg2, arg3);
    } else {
        {
            Arg150B06B0_2190 burst;

            burst.unk00 = 10;
            burst.unk04 = 10;
            burst.unk08 = pos;
            burst.unk1C = 7.0f;
            burst.unk20 = 17.0f;
            burst.unk14 = angle - 40;
            burst.unk16 = 80;
            burst.unk18 = -21;
            burst.unk1A = 13;
            burst.unk2C = 25;
            burst.unk2E = 25;
            burst.unk24 = -0.8000000119f;
            burst.unk28 = 0.3440000117f;
            burst.unk30 = 0.04900000244f;
            burst.unk34 = 0.1020000055f;
            burst.unk38 = 26.41100121f;
            func_15152190(&burst, D_8009F818, D_8009F81C, 1, 0.0f, 0, arg2, arg3);
        }
        {
            Arg150B06B0_3F18 spray;

            spray.unk08 = pos;
            spray.unk14 = 3.049999952f;
            spray.unk2C = 7;
            spray.unk2E = 3;
            spray.unk00 = angle - 60;
            spray.unk02 = 120;
            spray.unk04 = -30;
            spray.unk06 = 16;
            spray.unk30 = 3;
            spray.unk32 = 2;
            spray.unk34 = 20;
            spray.unk36 = 20;
            spray.unk38 = 155;
            spray.unk3A = 100;
            spray.unk44 = 16;
            spray.unk46 = 15;
            spray.unk48 = 0;
            spray.unk3C = 0;
            spray.unk18 = 5.009999752f;
            spray.unk1C = 0.7000000477f;
            spray.unk20 = 0.5010000467f;
            spray.unk24 = 15.10000038f;
            spray.unk28 = 20.80000114f;
            spray.unk40 = 0.5f;
            func_15153F18(&spray, &spray.unk08, &floor, arg2, arg3);
            spray.unk3C = 1;
            func_15153F18(&spray, &spray.unk08, &floor, arg2, arg3);
        }
    }
}

/*
 * Picks a random spawn point on a 248-unit perimeter around owner: 122 units along the back edge
 * (-31.5 along forward), or 63 units along either side (61 out along right). Writes the point to outPos
 * (y taken from player->unk118) and a matching s16 yaw to outAngle.
 */
void func_150B0A60(struct127 *owner, struct127 *player, f32 *forward, f32 *right, f32 *outPos, s16 *outAngle) {
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
    /* 0x00 */ struct127 *owner;
    /* 0x04 */ u8 ownerId; // owner->unique_id when attached; a mismatch means the slot was reused
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 timer;  // one particle is emitted per 1.0 accumulated
} DDB60EffectData;

/* The effect object spawned by func_150B0C58. */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE; // -1 when the owner is gone
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ DDB60EffectData data;
} DDB60Effect;

void func_150B0C58(struct127 *owner, u8 arg1, s32 arg2);

/* func_150B0C58 with the default arguments (0xFF, 1). */
void func_150B0C34(struct127 *owner) {
    func_150B0C58(owner, 0xFF, 1);
}

/* Attaches a DDB60Effect (updated by func_150B0D20) to owner. */
void func_150B0C58(struct127 *owner, u8 arg1, s32 arg2) {
    DDB60EffectData data;
    DDB60Effect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.timer = 0.0f;

    effect = (DDB60Effect *)func_15149130(300, -1, 0x58, -1, 0, 0x43, (struct37 *)sizeof(data), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Effect callback: hands func_15169850 the owner reference. */
void func_150B0CE0(DDB60Effect *effect, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, (s32)&effect->data.owner, (s32)&effect->data.ownerId, (s32)effect);
}

extern f32 D_8009F860[];
extern f32 D_8009F868[];
s32 func_1515C0F8(struct127 *, f32 **);
void func_1514373C(f32, f32, f32 *, f32 *);
void func_15143794(s16, s16, f32, f32 *);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32, s32, s32, s32, s32, s32, s32);

/*
 * DDB60Effect update: while the owner lives and player D_800CC2D0[owner->unk124] has unk31C byte 0x4F == 2,
 * emits particles around the owner when its speed (at least 1.35 * player->unk44) exceeds 26.
 */
void func_150B0D20(DDB60Effect *effect) {
    DDB60EffectData *data;
    struct127 *owner;
    s32 emit;
    f32 speed;
    f32 minSpeed;
    s32 idx;
    f32 forward[2];
    f32 right[2];
    f32 *refVel;
    f32 pos[3];
    f32 vel[3];
    s16 angle;

    owner = effect->data.owner;
    data = &effect->data;
    if (owner->interaction_state == 0 || owner->unique_id != data->ownerId) {
        effect->unkE = -1;
        return;
    }
    idx = owner->unk124;
    emit = 0;
    if (D_800CC2D0[idx].unk31C->pad4F[0] == 2) {
        speed = owner->xz_velocity;
        minSpeed = D_800CC2D0[idx].unk44 * 1.350000024f;
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
                func_150B0A60(owner, &D_800CC2D0[idx], forward, right, pos, &angle);
                func_15143794(angle, (func_150ADA20() % 26) - 41, func_150ADA68() * 3.769999981f + 4.059999943f, vel);
                vel[0] -= refVel[0] * 0.9250000715f;
                vel[2] -= refVel[2] * 0.9250000715f;
                func_151D9014(pos, vel, 5, func_150ADA68() * 0.1780000031f + -0.4420000315f,
                              (func_150ADA20() % 21) + 20, (func_150ADA20() % 101) + 155,
                              func_150ADA68() * 50.0f + 80.0f, func_150ADA68() < 0.04400000349f, 1.399999976f,
                              1.399999976f, 1, 0, 1, 0, effect->unkC, effect->unk1);
                data->timer -= 1.0f;
            } while (data->timer > 1.0f);
        }
    }
}
