#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawns an effect attached to `owner`. */
void func_150BA4C0(Actor *owner, u8 arg1, s32 arg2) {
    EffectE7970Data data;
    TimedEffect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.spawnAccum = 0.0f;

    effect = func_15149130((func_150ADA20() % 9U) + 15, -1, 0x52, -1, 1, 0x3F, 0xC, arg1, arg2);
    if (effect != NULL) {
        memcpy(&((EffectE7970 *)effect)->data, &data, sizeof(data));
    }
}

/* Per-frame update: kills the effect once its owner is gone, otherwise emits particles around the owner at a random rate. */
void func_150BA55C(EffectE7970 *effect) {
    Actor *owner;
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
    if ((owner->type == 0) || (owner->unique_id != data->ownerId)) {
        effect->base.header.state = -1;
        return;
    }
    if (owner->ob_matrix_ptr != NULL) {
        data->spawnAccum += (0.3143000007f + (func_150ADA68() * 0.58100003f)) * D_800BE9A4;
        if (data->spawnAccum > 1.0f) {
            size = (owner->scale_xz + owner->scale_y) * 0.5f;
            heading = (owner->angle >> 8) - 64;
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
                              effect->base.header.owner, effect->base.header.kind);
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        }
    }
}

void func_150BA8F0(EffectE7970 *effect, s32 arg1, u8 arg2) {
    func_15149514(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

