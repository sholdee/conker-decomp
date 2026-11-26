#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawns an effect attached to `owner`. */
void particle_attachOwnerEmitter(Actor *owner, u8 arg1, s32 arg2) {
    EffectE7970Data data;
    TimedEffect *effect;

    data.owner = owner;
    data.ownerId = owner->unique_id;
    data.spawnAccum = 0.0f;

    effect = effect_newTimedEffect((func_150ADA20() % 9U) + 15, -1, 0x52, -1, 1, 0x3F, 0xC, arg1, arg2);
    if (effect != NULL) {
        memcpy(&((EffectE7970 *)effect)->data, &data, sizeof(data));
    }
}

/* Per-frame update: kills the effect once its owner is gone, otherwise emits particles around the owner at a random rate. */
void particle_updateOwnerEmitter(EffectE7970 *effect) {
    Actor *owner;
    EffectE7970Data *data;
    s32 heading;
    Vec3f ownerPos;
    f32 radius;
    f32 unused;
    Vec3f growth;
    f32 size;
    Vec3f spawnPos;
    Vec3f angles;
    Vec3f velocity;
    f32 speed;
    u32 randomYaw;

    owner = effect->data.owner;
    data = &effect->data;
    if ((owner->type == 0) || (owner->unique_id != data->ownerId)) {
        effect->base.header.state = -1;
        return;
    }
    if (owner->ob_matrix_ptr != NULL) {
        data->spawnAccum += (0.3143000007f + (func_150ADA68() * 0.58100003f)) * gFrameDelta;
        if (data->spawnAccum > 1.0f) {
            size = (owner->scale_xz + owner->scale_y) * 0.5f;
            heading = (owner->angle >> 8) - 64;
            sprite_getActorBodyBounds(owner, &ownerPos, &radius, &unused);
            growth.y = 0.0f;
            do {
                randomYaw = func_150ADA20();
                math_dirFromBinAngles((randomYaw % 127U) + heading - 63, (func_150ADA20() % 31U) - 50, radius, &spawnPos);
                spawnPos.y *= owner->unkF0;
                spawnPos.x += ownerPos.x;
                spawnPos.y += ownerPos.y;
                spawnPos.z += ownerPos.z;
                angles.x = func_150ADA68() * 360.0f;
                angles.y = func_150ADA68() * 360.0f;
                angles.z = func_150ADA68() * 360.0f;
                speed = (func_150ADA68() * 0.09400000423f) + 0.05100000277f;
                velocity.x = (spawnPos.x - ownerPos.x) * speed;
                velocity.y = (spawnPos.y - ownerPos.y) * speed;
                velocity.z = (spawnPos.z - ownerPos.z) * speed;
                growth.x = (func_150ADA68() * 18.64500046f) + -9.322500229f;
                growth.z = (func_150ADA68() * 18.64500046f) + -9.322500229f;
                effect_spawnTumblingPiece(&spawnPos, &angles, ((func_150ADA68() * 0.08000000566f) + 0.1000000015f) * size, &velocity, &growth,
                              (func_150ADA68() * 0.324000001f) + -0.5630000234f, 0.9448540211f, 7, (func_150ADA20() % 41U) + 60, 0, 0,
                              effect->base.header.owner, effect->base.header.kind);
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        }
    }
}

void func_150BA8F0(EffectE7970 *effect, s32 arg1, u8 arg2) {
    effect_handleTimedEffectOwnerEvent(arg1, arg2, &effect->data.owner, &effect->data.ownerId, effect);
}

