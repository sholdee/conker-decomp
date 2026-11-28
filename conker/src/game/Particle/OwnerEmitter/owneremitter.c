#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "conker/sprite.h"

/* Spawns an effect attached to `owner`. */
void particle_attachOwnerEmitter(Actor *owner, u8 fxOwner, s32 fxList) {
    FixedEmitterData data;
    FixedEmitter *effect;

    data.object = owner;
    data.objectId = owner->unique_id;
    data.spawnAccum = 0.0f;

    effect = effect_newTimedEffect((math_random() % 9U) + 15, -1, 0x52, -1, 1, 0x3F, 0xC, fxOwner, fxList);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Per-frame update: kills the effect once its owner is gone, otherwise emits particles around the owner at a random rate. */
void particle_updateOwnerEmitter(FixedEmitter *effect) {
    Actor *owner;
    FixedEmitterData *data;
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

    owner = effect->data.object;
    data = &effect->data;
    if ((owner->type == 0) || (owner->unique_id != data->objectId)) {
        effect->base.header.state = -1;
        return;
    }
    if (owner->ob_matrix_ptr != NULL) {
        data->spawnAccum += (0.3143000007f + (math_randomFloat() * 0.58100003f)) * gFrameDelta;
        if (data->spawnAccum > 1.0f) {
            size = (owner->scale_xz + owner->scale_y) * 0.5f;
            heading = (owner->angle >> 8) - 64;
            sprite_getActorBodyBounds(owner, &ownerPos, &radius, &unused);
            growth.y = 0.0f;
            do {
                randomYaw = math_random();
                math_dirFromBinAngles((randomYaw % 127U) + heading - 63, (math_random() % 31U) - 50, radius, &spawnPos);
                spawnPos.y *= owner->bodyHeightOverRadius;
                spawnPos.x += ownerPos.x;
                spawnPos.y += ownerPos.y;
                spawnPos.z += ownerPos.z;
                angles.x = math_randomFloat() * 360.0f;
                angles.y = math_randomFloat() * 360.0f;
                angles.z = math_randomFloat() * 360.0f;
                speed = (math_randomFloat() * 0.09400000423f) + 0.05100000277f;
                velocity.x = (spawnPos.x - ownerPos.x) * speed;
                velocity.y = (spawnPos.y - ownerPos.y) * speed;
                velocity.z = (spawnPos.z - ownerPos.z) * speed;
                growth.x = (math_randomFloat() * 18.64500046f) + -9.322500229f;
                growth.z = (math_randomFloat() * 18.64500046f) + -9.322500229f;
                effect_spawnTumblingPiece(&spawnPos, &angles, ((math_randomFloat() * 0.08000000566f) + 0.1000000015f) * size, &velocity, &growth,
                              (math_randomFloat() * 0.324000001f) + -0.5630000234f, 0.9448540211f, 7, (math_random() % 41U) + 60, 0, 0,
                              effect->base.header.owner, effect->base.header.kind);
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        }
    }
}

void particle_onOwnerEmitterEvent(FixedEmitter *effect, s32 event, u8 eventType) {
    effect_handleTimedEffectOwnerEvent(event, eventType, &effect->data.object, &effect->data.objectId, effect);
}

