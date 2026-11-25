#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"


extern s32 func_15143E24(Actor *);
extern void func_1504715C(CollisionHit *, Actor *);
extern void func_151C329C(void *, u8, s32);
extern void func_15152190(DebrisBurst *, const s32 *, const f32 *, s32, f32, s32, u8, s32);
extern void func_15153F18(AngleRange *, SplashSpray *, CollisionHit *, u8, s32);

const s32 D_8009F810[1] = { 0xE2 };
const f32 D_8009F814[1] = { 1.0f };
const s32 D_8009F818[1] = { 0x39 };
const f32 D_8009F81C[1] = { 1.0f };

/*
 * Spawns effects at actor's position, aimed by func_15143E24(other): actor id 0xA6 gets func_151C329C
 * 50 units above it and one func_15152190 burst; any other actor gets a different burst plus two
 * func_15153F18 passes (unk3C = 0, then 1) using the func_1504715C info for actor.
 */
void func_150B06B0(Actor *actor, Actor *other, u8 arg2, s32 arg3) {
    Vec3f pos;
    s8 angle;
    CollisionHit floor;

    pos.x = actor->xpos;
    pos.y = actor->ypos;
    pos.z = actor->zpos;
    angle = func_15143E24(other) + 0x40;
    func_1504715C(&floor, actor);
    if (actor->character_type == 0xA6) {
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
void func_150B0C58(Actor *owner, u8 arg1, s32 arg2);

void func_150B0D20(DDB60Effect *effect);
