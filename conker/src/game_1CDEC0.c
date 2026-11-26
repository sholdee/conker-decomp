#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void *memcpy(void *, const void *, u32);

/* Starts a spark emitter effect at the actor's position; the spark variant depends on the actor's unk184. */
void func_151A0A10(Actor *actor, s16 duration, u8 owner, s32 arg3) {
    TimedEffect *effect;
    PointSparkEmitter emitter;

    if (actor != NULL) {
        emitter.x = actor->xpos;
        emitter.z = actor->zpos;
        if (D_800C35EA[0] != 1) {
            emitter.y = actor->floorY;
            emitter.accum = 0.0f;
            switch (func_1510F8CC(actor->floorMaterial)) {
            case 10:
                emitter.variant = 0;
                break;
            case 15:
            case 17:
                emitter.variant = 1;
                break;
            default:
                emitter.variant = 0;
                break;
            }
            effect = func_151491F4(duration, -1, 1, 1, 0, sizeof(PointSparkEmitter), owner, arg3);
            if (effect != NULL) {
                memcpy(&((SparkEmitterEffect *)effect)->emitter, &emitter, sizeof(PointSparkEmitter));
            }
        }
    }
}

/* Per-frame update of the spark emitter: emits one spark for every whole unit the accumulator gains. */
void func_151A0AF8(SparkEmitterEffect *effect) {
    PointSparkEmitter *emitter;

    effect->emitter.accum += 0.1750000119f * D_800BE9A4;
    emitter = &effect->emitter;
    while (emitter->accum > 1.0f) {
        func_1514C678(emitter->x, emitter->y, emitter->z, func_150ADA68() * 25.0f + 15.0f,
                      0, 0xFF, 5, 4, emitter->variant, 0.0f, 0, effect->base.header.owner);
        emitter->accum -= 1.0f;
    }
}

extern u8 D_8008F8D0[];

/*
 * Particle kind 4 spawn callback (D_8008AA00): a fast spark thrown out in a random
 * direction around `angle`, grey level and sprite chosen by `variant`.
 */
s32 func_151A0C0C(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ,
                  s32 angle, s32 baseAngle, f32 radius, s32 variant, f32 arg12, s32 arg13, u8 owner) {
    BillboardTemplate spark;
    s16 tilt;
    f32 sinTilt;
    f32 cosTilt;
    f32 sinAngle;
    f32 cosAngle;
    f32 speed;

    tilt = func_150ADA20() % 15U - 63;
    sinTilt = func_151423D8(tilt);
    cosTilt = func_151423D8(tilt - 64);
    sinAngle = func_151423D8(angle);
    cosAngle = func_151423D8(angle - 64);
    speed = func_150ADA68() * 503.0f + 801.0f;
    spark.velX = speed * sinTilt * cosAngle;
    spark.velY = -speed * cosTilt;
    spark.velZ = speed * sinTilt * sinAngle;
    spark.accelY = func_150ADA20() % 41U - 130;
    spark.fadeStep = func_150ADA20() % 3U + 6;
    spark.width = spark.height = func_150ADA20() % 451U + 250;
    spark.x = x;
    spark.y = y;
    spark.z = z;
    /* all three fractions come from x in the original */
    // matching: a u8 conversion stored through an s8 lvalue fixes the store schedule
    *(s8 *)&spark.subX = (u8)(s32)(x * 256.0f);
    *(s8 *)&spark.subZ = (u8)(s32)(x * 256.0f);
    *(s8 *)&spark.subY = (u8)(s32)(x * 256.0f);
    spark.envR = 0;
    spark.envG = 0;
    spark.envB = 0;
    spark.primA = 0xFF;
    spark.callback = -1;
    spark.flags = 0;
    spark.frame = 0;
    spark.frameStep = 0;
    spark.primR = D_8008F8D0[variant];
    spark.primG = D_8008F8D0[variant];
    spark.primB = D_8008F8D0[variant];
    switch (variant) {
    case 0:
        spark.anim = &D_800918A0;
        break;
    case 1:
        spark.anim = &D_8009187C;
        break;
    }
    spark.life = 200;
    func_15167D84(&spark, 0, 0, -1, owner, 1);
    return 1;
}
