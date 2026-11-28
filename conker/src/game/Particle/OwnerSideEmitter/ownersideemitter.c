#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "libc/string.h"

/* Live particle returned by particle_newOwnedSpriteParticle; its extra data starts at 0xA8. */

/* Spawns the task that particle_updateOwnerSideTask updates for this owner. */
void particle_startOwnerSideTask(Actor *owner) {
    Effect1BFDD0 *task;
    FixedEmitterData data;

    // matching: an integer test (not != NULL) makes IDO reload owner from its arg slot for each use
    if ((s32)owner != 0) {
        data.object = owner;
        data.objectId = owner->unique_id;
        data.spawnAccum = 0.0f;

        task = effect_newTimedEffectNoDraw(0x3C, -1, 0x15, 1, 0x11, 0xC, 0xFF, 1);
        if (task != NULL) {
            memcpy(&task->data, &data, sizeof(data));
        }
    }
}

/* Per-frame update: emits particles from two points beside the owner at a randomised rate. */
void particle_updateOwnerSideTask(Effect1BFDD0 *task) {
    FixedEmitterData *data;
    SpriteParticle *particle;
    Vec3f *origin;
    f32 size;
    ParticleDesc64 spawn;
    f32 zero; // matching: 0.0f held in one FP register for every store
    f32 extra;
    Vec3f emitPointA;
    Vec3f emitPointB;

    data = &task->data;
    data->spawnAccum += (697.0f + (math_randomFloat() * 342.0f)) * 0.001000000047f * gFrameDelta;
    if (data->spawnAccum > 1.0f) {
        if (data->object->ob_matrix_ptr != NULL) {
            {
                Vec3f offset = {19.0f, 0.0f, 25.0f};
                math_transformOffset(&offset, &emitPointA, &data->object->ob_matrix_ptr[32]);
            }
            {
                Vec3f offset = {19.0f, 0.0f, 25.0f};
                math_transformOffset(&offset, &emitPointB, &data->object->ob_matrix_ptr[25]);
            }
            zero = 0.0f;
            spawn.texture = 0x29;
            spawn.renderMode = 0xE03;
            spawn.setGeometryMode = 0x200005;
            spawn.clearGeometryMode = 0;
            spawn.r2 = 0xB0;
            spawn.g2 = 0xA0;
            spawn.b2 = 0x2A;
            spawn.r = 0x40;
            spawn.g = 0xB;
            spawn.b = 0x6A;
            spawn.frame = 0;
            spawn.frameRate = 0;
            spawn.a = 0xFF;
            spawn.brightness = 0xFF;
            spawn.primColorMode = 3;
            spawn.envColorMode = 3;
            spawn.gravity = zero;
            spawn.flags = 0xE05;
            spawn.updateFunc = 0xA;
            spawn.deathFunc = -1;
            spawn.fadeStart = 30;
            spawn.fadeRate = 8;
            spawn.scaleStart = 50;
            spawn.scaleRate = 1.02700007f;
            extra = 0.9240000248f;
            do {
                {
                    s16 angle;
                    u8 baseAngle;

                    spawn.timer = (math_random() % 41U) + 30;
                    spawn.alpha = (math_random() % 156U) + 100;
                    size = (math_randomFloat() * 104.0f) + 199.0f;
                    spawn.height = spawn.width = size;
                    origin = (math_random() & 1) ? &emitPointA : &emitPointB;
                    baseAngle = (data->object->langle >> 8) + 0x40;
                    angle = (math_random() % 100U) + baseAngle - 49;
                    math_dirFromBinAngleXZ(angle, 36.0f, &spawn.pos.x, &spawn.pos.z);
                    spawn.pos.y = zero; // matching: dead store steers the scheduler
                    spawn.pos.x += origin->x;
                    spawn.pos.y = origin->y;
                    spawn.pos.z += origin->z;
                    math_dirFromBinAngleXZ(angle, ((math_randomFloat() * 1497.0f) + 995.0f) * 0.009999999776f,
                                  &spawn.vel.x, &spawn.vel.z);
                    spawn.vel.y = zero;
                }
                spawn.flags &= ~0xC0;
                if (math_random() & 1) {
                    spawn.flags |= 0x40;
                }
                if (math_random() & 1) {
                    spawn.flags |= 0x80;
                }
                particle = particle_newOwnedSpriteParticle(&spawn, 1, sizeof(extra), task->base.header.owner, task->base.header.kind);
                if (particle != NULL) {
                    memcpy(&particle->data.drag, &extra, sizeof(extra));
                }
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        } else {
            while (data->spawnAccum > 1.0f) {
                data->spawnAccum -= 1.0f;
            }
        }
    }
}

/* Message handler for the task: follows an owner swap, or destroys itself when its owner goes away. */
void particle_onOwnerSideTaskMsg(Effect1BFDD0 *task, ActorMsg *msg, u8 kind) {
    FixedEmitterData *data = &task->data;

    if (kind == 0x2D) {
        if (data->object == msg->swap.actorA) {
            data->object = msg->swap.actorB;
            data->objectId = msg->swap.idB;
        } else if (data->object == msg->swap.actorB) {
            // matching: the copy keeps IDO from moving the byte store into the branch delay slot
            FixedEmitterData *dst = data;

            dst->object = msg->swap.actorA;
            dst->objectId = msg->swap.idA;
        }
    } else if (kind == 0) {
        Actor *owner = msg->ref.actor;

        if (owner == data->object || msg->ref.uniqueId == data->objectId) {
            effect_destroyObject(&task->base.header);
        }
    }
}
