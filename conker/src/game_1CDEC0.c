#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void *memcpy(void *, const void *, u32);
s32 func_1510F8CC(s32);
void func_1514C678(f32 x, f32 y, f32 z, f32 radius, s16 arg4, s16 arg5, s16 arg6, u8 arg7, s32 variant, f32 arg9, s32 argA, u8 owner);
extern void *func_15167D84(void *, s32, s32, s8, u8, s32);

/* Data of the spark emitter effect, stored at 0x28 in the effect object from func_151491F4. */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ u8 variant;
    /* 0x10 */ f32 accum; /* particles owed; one is emitted per whole unit */
} SparkEmitter; // size 0x14

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ SparkEmitter emitter;
} SparkEmitterEffect;

/* Starts a spark emitter effect at the actor's position; the spark variant depends on the actor's unk184. */
void func_151A0A10(struct127 *actor, s16 duration, u8 owner, s32 arg3) {
    struct260 *effect;
    SparkEmitter emitter;

    if (actor != NULL) {
        emitter.x = actor->x_position;
        emitter.z = actor->z_position;
        if (D_800C35EA != 1) {
            emitter.y = actor->unk180;
            emitter.accum = 0.0f;
            switch (func_1510F8CC(actor->unk184)) {
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
            effect = func_151491F4(duration, -1, 1, 1, 0, sizeof(SparkEmitter), owner, arg3);
            if (effect != NULL) {
                memcpy(&((SparkEmitterEffect *)effect)->emitter, &emitter, sizeof(SparkEmitter));
            }
        }
    }
}

/* Per-frame update of the spark emitter: emits one spark for every whole unit the accumulator gains. */
void func_151A0AF8(SparkEmitterEffect *effect) {
    SparkEmitter *emitter;

    effect->emitter.accum += 0.1750000119f * D_800BE9A4;
    emitter = &effect->emitter;
    while (emitter->accum > 1.0f) {
        func_1514C678(emitter->x, emitter->y, emitter->z, func_150ADA68() * 25.0f + 15.0f,
                      0, 0xFF, 5, 4, emitter->variant, 0.0f, 0, effect->owner);
        emitter->accum -= 1.0f;
    }
}

/* 0x38-byte template that func_15167D84 copies into a new particle (fields as in game_1944C0's Particle). */
typedef struct {
    /* 0x00 */ void *anim;
    /* 0x04 */ u8 pad04[8];
    /* 0x0C */ s16 frame;
    /* 0x0E */ s16 frameStep;
    /* 0x10 */ s16 x, y, z;
    /* 0x16 */ s16 velX, velZ;
    /* 0x1A */ u8 pad1A[2];
    /* 0x1C */ s8 subX, subZ, subY;
    /* 0x1F */ s8 callback;
    /* 0x20 */ s16 velY;
    /* 0x22 */ s16 accelY;
    /* 0x24 */ s16 width, height;
    /* 0x28 */ s16 life;
    /* 0x2A */ s8 fadeStep;
    /* 0x2B */ u8 pad2B;
    /* 0x2C */ u8 primR, primG, primB, primA;
    /* 0x30 */ s8 envR, envG, envB;
    /* 0x33 */ u8 pad33;
    /* 0x34 */ s16 flags;
} ParticleTemplate;

extern u8 D_8008F8D0[];
extern u8 D_8009187C[];
extern u8 D_800918A0[];

/*
 * Particle kind 4 spawn callback (D_8008AA00): a fast spark thrown out in a random
 * direction around `angle`, grey level and sprite chosen by `variant`.
 */
s32 func_151A0C0C(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY, f32 originZ,
                  s32 angle, s32 baseAngle, f32 radius, s32 variant, f32 arg12, s32 arg13, u8 owner) {
    ParticleTemplate spark;
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
    // matching: the u8 conversion into these s8 fields fixes the store schedule
    spark.subX = (u8)(s32)(x * 256.0f);
    spark.subZ = (u8)(s32)(x * 256.0f);
    spark.subY = (u8)(s32)(x * 256.0f);
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
        spark.anim = D_800918A0;
        break;
    case 1:
        spark.anim = D_8009187C;
        break;
    }
    spark.life = 200;
    func_15167D84(&spark, 0, 0, -1, owner, 1);
    return 1;
}
