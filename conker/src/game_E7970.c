#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_8009FE64;
void func_15143E94(s32 arg0, s32 arg1);
void func_15165F80(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_1514C678(f32 arg0, f32 arg1, f32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, f32 arg9, s32 argA, s32 argB);

/* Per-effect data: the object the effect is attached to and a spawn accumulator. */
typedef struct {
    /* 0x0 */ struct127 *owner;
    /* 0x4 */ u8 ownerId; // owner's unique_id when the effect was created
    /* 0x8 */ f32 spawnAccum;
} EffectE7970Data; // size 0xC

/* Effect slot as returned by func_15149130 (a struct260); the effect data lives at 0x28. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ EffectE7970Data data;
} EffectE7970;

/* Spawns an effect attached to `owner`. */
void func_150BA4C0(struct127 *owner, u8 arg1, s32 arg2) {
    EffectE7970Data data;
    struct260 *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.spawnAccum = 0.0f;

    effect = func_15149130((func_150ADA20() % 9U) + 15, -1, 0x52, -1, 1, 0x3F, (struct37 *)0xC, arg1, arg2);
    if (effect != NULL) {
        memcpy(&((EffectE7970 *)effect)->data, &data, sizeof(data));
    }
}

void func_15143794(s16, s16, f32, f32 *);
void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
void func_1518A3C0(f32 *arg0, f32 *arg1, f32 arg2, f32 *arg3, f32 *arg4, f32 arg5, f32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC);

/* Per-frame update: kills the effect once its owner is gone, otherwise emits particles around the owner at a random rate. */
void func_150BA55C(EffectE7970 *effect) {
    struct127 *owner;
    EffectE7970Data *data;
    s32 heading;
    f32 ownerPos[3];
    f32 radius;
    f32 unused;
    f32 growth[3];
    f32 size;
    f32 spawnPos[3];
    f32 angles[3];
    f32 velocity[3];
    f32 speed;
    u32 randomYaw;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->interaction_state == 0) || (owner->unique_id != data->ownerId)) {
        effect->unkE = -1;
        return;
    }
    if (owner->unk1D4 != NULL) {
        data->spawnAccum += (0.3143000007f + (func_150ADA68() * 0.58100003f)) * D_800BE9A4;
        if (data->spawnAccum > 1.0f) {
            size = (owner->xz_scale + owner->y_scale) * 0.5f;
            heading = (owner->unk76 >> 8) - 64;
            func_1515C244(owner, ownerPos, &radius, &unused);
            growth[1] = 0.0f;
            do {
                randomYaw = func_150ADA20();
                func_15143794((randomYaw % 127U) + heading - 63, (func_150ADA20() % 31U) - 50, radius, spawnPos);
                spawnPos[1] *= owner->unkF0;
                spawnPos[0] += ownerPos[0];
                spawnPos[1] += ownerPos[1];
                spawnPos[2] += ownerPos[2];
                angles[0] = func_150ADA68() * 360.0f;
                angles[1] = func_150ADA68() * 360.0f;
                angles[2] = func_150ADA68() * 360.0f;
                speed = (func_150ADA68() * 0.09400000423f) + 0.05100000277f;
                velocity[0] = (spawnPos[0] - ownerPos[0]) * speed;
                velocity[1] = (spawnPos[1] - ownerPos[1]) * speed;
                velocity[2] = (spawnPos[2] - ownerPos[2]) * speed;
                growth[0] = (func_150ADA68() * 18.64500046f) + -9.322500229f;
                growth[2] = (func_150ADA68() * 18.64500046f) + -9.322500229f;
                func_1518A3C0(spawnPos, angles, ((func_150ADA68() * 0.08000000566f) + 0.1000000015f) * size, velocity, growth,
                              (func_150ADA68() * 0.324000001f) + -0.5630000234f, 0.9448540211f, 7, (func_150ADA20() % 41U) + 60, 0, 0,
                              effect->unkC, effect->unk1);
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        }
    }
}

void func_150BA8F0(EffectE7970 *effect, s32 arg1, u8 arg2) {
    func_15149514(arg1, arg2, (s32)&effect->data.owner, (s32)&effect->data.ownerId, (s32)effect);
}

/* Ground probe result filled by func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ s16 points[9];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 unk1D;
    /* 0x20 */ s32 unk20;
} GroundHit; // size 0x24

void func_1504715C(GroundHit *hit, struct127 *obj);
s32 func_15046C80(struct17 *pos, s32 arg1, f32 minY, GroundHit *hit);

/* Copies obj's position into pos (using unk180 as the height when set); with a hit buffer, also probes the ground below. */
s32 func_150BA930(struct17 *pos, struct127 *obj, GroundHit *hit, s32 arg3) {
    struct17 probe;

    pos->unk0 = obj->x_position;
    if (-9500.0f < obj->unk180) {
        pos->unk4 = obj->unk180;
    } else {
        pos->unk4 = obj->y_position;
    }
    pos->unk8 = obj->z_position;
    if (hit == NULL) {
        return 1;
    }
    probe.unk0 = pos->unk0;
    probe.unk4 = pos->unk4 + 100.0f;
    probe.unk8 = pos->unk8;
    func_1504715C(hit, obj);
    return func_15046C80(&probe, 0, pos->unk4 - 100.0f, hit);
}

s32 func_150BAA00(s32 arg0, s32 arg1) {
    return 9;
}

void func_150BAA14(struct127 *obj, s32 arg1, s32 arg2) {
    struct17 pos;
    GroundHit hit;
    s32 angle;
    u8 onGround;
    s16 pad; // matching: unused slot moves IDO's spill of func_150ADA68()'s result to sp38

    if (obj->unk1D4 == NULL) {
        return;
    }

    onGround = func_150BA930(&pos, obj, &hit, (u8)arg1);
    func_151D5404(&pos, 1307.0f, 2000.0f, 0.0005f, 0xC, 0xF, 0xFF, 0);
    func_15143E94(5, 0x4022);

    if (onGround) {
        angle = (s32)(D_800DBFF0[D_80082FA4].unk380 * D_8009FE64);
        func_15165F80(-1, (s32)pos.unk0, (s32)(pos.unk4 + 6.0f), (s32)pos.unk8,
                      0x19, 0x12, 0, 0xFF, 1);
        func_1514C678(pos.unk0, pos.unk4, pos.unk8,
                      (func_150ADA68() * 50.0f) + 40.0f, angle + 60, angle - 60,
                      (func_150ADA20() % 11U) + 30, 5, 0, 0.0f, 0, 0xFF);
    }
}
