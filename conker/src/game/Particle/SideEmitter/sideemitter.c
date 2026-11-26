#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Two object-attached particle emitters (effects 0x23 and the caller-chosen type of particle_attachBoneSideEmitter):
 * their spawners, per-frame updates and message handlers. Each effect is a TimedEffect whose data at
 * 0x28 is a FixedEmitterData.
 */

/* Live particle returned by particle_newOwnedSpriteParticle; its caller-sized data starts at 0xA8. */
typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ f32 drag; /* read by update func 10 (particle_updateDragXZ) */
} Particle1BFDD0;

/* Attaches effect 0x23 to the object. */
void particle_attachSideEmitter(Actor *object) {
    Effect1BFDD0 *effect;
    FixedEmitterData data;

    if (object == NULL) {
        return;
    }
    data.object = object;
    data.objectId = object->unique_id;
    data.spawnAccum = 0.0f;

    effect = effect_newTimedEffectNoDraw(0x23, -1, 0x14, 1, 0x10, 0xC, 0xFF, 1);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

/* Effect 0x23 update: emits 0.31-0.91 particles per gFrameDelta tick, 20 units to either side of the object,
 * moving against the direction sprite_findActorMotion returns. */
void particle_updateSideEmitter(Effect1BFDD0 *effect) {
    FixedEmitterData *data;
    ParticleDesc desc;
    u8 side;
    f32 drag;
    const Vec3f *dir;
    f32 speed;
    Particle1BFDD0 *particle;

    data = &effect->data;
    data->spawnAccum += (312.0f + (math_randomFloat() * 599.0f)) * 0.001f * gFrameDelta;
    if (data->spawnAccum > 1.0f && sprite_findActorMotion(data->object, &dir) != 0) {
        desc.texture = 0x29;
        desc.renderMode = 0xE03;
        desc.unk0 = 0x200005;
        desc.r2 = 0xB0;
        desc.g2 = 0xA0;
        desc.b2 = 0x2A;
        desc.r = 0x40;
        desc.g = 0xB;
        desc.b = 0x6A;
        desc.unk4 = 0;
        desc.frame = 0;
        desc.frameRate = 0;
        desc.a = 0xFF;
        desc.brightness = 0xFF;
        desc.primColorMode = 3;
        desc.envColorMode = 3;
        desc.flags = PARTICLE_LIFETIME | PARTICLE_MOVE | PARTICLE_FADE | PARTICLE_SCALE | PARTICLE_SCALE_MUL;
        desc.updateFunc = 10;
        desc.deathFunc = -1;
        desc.fadeStart = 20;
        desc.fadeRate = 12;
        desc.scaleStart = 35;
        speed = -1.516f;
        desc.gravity = 0.0f;
        desc.scaleRate = 1.04f;
        drag = 0.95000005f;
        do {
            side = math_random() & 1;
            desc.timer = (math_random() % 31U) + 20;
            desc.alpha = (math_random() % 156U) + 100;
            desc.width = desc.height = (math_randomFloat() * 150.0f) + 100.0f;
            math_dirFromBinAngleXZ(data->object->langle >> 8, side ? 20.0f : -20.0f, &desc.pos.x, &desc.pos.z);
            desc.pos.x += data->object->xpos;
            desc.pos.z += data->object->zpos;
            desc.pos.y = data->object->floorY + 20.0f;
            desc.flags &= ~(PARTICLE_FLIP_S | PARTICLE_FLIP_T);
            if (math_random() & 1) {
                desc.flags |= PARTICLE_FLIP_S;
            }
            if (math_random() & 1) {
                desc.flags |= PARTICLE_FLIP_T;
            }
            desc.vel.x = dir->x * speed;
            desc.vel.y = dir->y * speed;
            desc.vel.z = dir->z * speed;
            particle = particle_newOwnedSpriteParticle(&desc, 1, sizeof(f32), effect->base.header.owner, effect->base.header.kind);
            if (particle != NULL) {
                memcpy(&particle->drag, &drag, sizeof(drag));
            }
            data->spawnAccum -= 1.0f;
        } while (data->spawnAccum > 1.0f);
    }
}

/* Attaches an effect of the given type to the object; returns the effect or NULL. */
Effect1BFDD0 *particle_attachBoneSideEmitter(Actor *object, s16 type, u8 arg2, s32 arg3) {
    Effect1BFDD0 *effect;
    FixedEmitterData data;

    if (object == NULL) {
        return NULL;
    }

    data.object = object;
    data.objectId = object->unique_id;
    data.spawnAccum = 0.0f;

    effect = effect_newTimedEffectNoDraw(type, -1, 0x18, 1, 0x16, 0xC, arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return effect;
}

/* Update of particle_attachBoneSideEmitter's effect: emits 0.79-1.99 particles per gFrameDelta tick from bone 1 of the
 * object, flung out to either side. */
void particle_updateBoneSideEmitter(Effect1BFDD0 *effect) {
    FixedEmitterData *data;
    ParticleDesc desc;
    s16 facing;
    s16 yaw;
    f32 drag;
    s32 rand;
    Vec3f offset;
    Particle1BFDD0 *particle;

    data = &effect->data;
    data->spawnAccum += (792.0f + (math_randomFloat() * 1202.0f)) * 0.001f * gFrameDelta;
    if (data->spawnAccum > 1.0f) {
        if (data->object->ob_matrix_ptr == NULL) {
            // the object has no bone matrices: spins forever
            while (data->spawnAccum > 1.0f) {
            }
        } else {
            facing = (data->object->langle >> 8) + 0x40;
            desc.texture = 0x29;
            desc.renderMode = 0xE03;
            desc.unk0 = 0x200005;
            desc.r2 = 0xB0;
            desc.g2 = 0xA0;
            desc.b2 = 0x2A;
            desc.r = 0x40;
            desc.g = 0xB;
            desc.b = 0x6A;
            desc.unk4 = 0;
            desc.frame = 0;
            desc.frameRate = 0;
            desc.a = 0xFF;
            desc.brightness = 0xFF;
            desc.primColorMode = 3;
            desc.envColorMode = 3;
            desc.flags = PARTICLE_LIFETIME | PARTICLE_MOVE | PARTICLE_FADE | PARTICLE_SCALE | PARTICLE_SCALE_MUL;
            desc.updateFunc = 10;
            desc.deathFunc = -1;
            desc.fadeStart = 25;
            desc.fadeRate = 10;
            desc.scaleStart = 35;
            desc.gravity = 0.0f;
            desc.scaleRate = 1.02f;
            drag = 0.9709f;
            do {
                yaw = (math_random() % 51U) - 130;
                offset.x = (math_randomFloat() * 20.0f) + 30.0f;
                offset.y = (math_randomFloat() * 60.0f) + -20.0f;
                offset.z = 0.0f;
                if (math_random() & 1) {
                    offset.x = -offset.x;
                    yaw += facing;
                } else {
                    yaw = facing - yaw;
                }
                desc.timer = (math_random() % 11U) + 30;
                desc.alpha = (math_random() % 101U) + 100;
                desc.width = desc.height = (math_randomFloat() * 150.0f) + 100.0f;
                math_transformOffset(&offset, &desc.pos, data->object->ob_matrix_ptr + 1);
                rand = math_random();
                math_dirFromBinAngles(yaw, (rand % 28U) - 25, math_randomFloat() * 3998.0f * 0.001f, &desc.vel);
                desc.flags &= ~(PARTICLE_FLIP_S | PARTICLE_FLIP_T);
                if (math_random() & 1) {
                    desc.flags |= PARTICLE_FLIP_S;
                }
                if (math_random() & 1) {
                    desc.flags |= PARTICLE_FLIP_T;
                }
                particle = particle_newOwnedSpriteParticle(&desc, 1, sizeof(f32), effect->base.header.owner, effect->base.header.kind);
                if (particle != NULL) {
                    memcpy(&particle->drag, &drag, sizeof(drag));
                }
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        }
    }
}

/* Message handler: message 0x2D moves the effect to the other object of a swap; message 0 ends the
 * effect when it names the effect's object. */
void particle_onSideEmitterMsg(Effect1BFDD0 *effect, void *msg, u8 msgType) {
    FixedEmitterData *data = &effect->data;
    ActorSwapMsg *swap = msg;
    ActorRef *removal = msg;
    Actor *removed;

    if (msgType == 0x2D) {
        if (data->object == swap->actorA) {
            data->object = swap->actorB;
            data->objectId = swap->idB;
        } else if (data->object == swap->actorB) {
            FixedEmitterData *dst = data; // matching: the -g3 scope keeps the store out of the b delay slot

            dst->object = swap->actorA;
            dst->objectId = swap->idA;
        }
    } else if (msgType == 0) {
        removed = removal->actor;
        if (removed == data->object || removal->uniqueId == data->objectId) {
            effect_destroyObject(&effect->base.header);
        }
    }
}

/* Message handler of particle_attachBoneSideEmitter's effect: the same as particle_onSideEmitterMsg. */
void particle_onBoneSideEmitterMsg(Effect1BFDD0 *effect, void *msg, u8 msgType) {
    FixedEmitterData *data = &effect->data;
    ActorSwapMsg *swap = msg;
    ActorRef *removal = msg;
    Actor *removed;

    if (msgType == 0x2D) {
        if (data->object == swap->actorA) {
            data->object = swap->actorB;
            data->objectId = swap->idB;
        } else if (data->object == swap->actorB) {
            FixedEmitterData *dst = data; // matching: the -g3 scope keeps the store out of the b delay slot

            dst->object = swap->actorA;
            dst->objectId = swap->idA;
        }
    } else if (msgType == 0) {
        removed = removal->actor;
        if (removed == data->object || removal->uniqueId == data->objectId) {
            effect_destroyObject(&effect->base.header);
        }
    }
}
