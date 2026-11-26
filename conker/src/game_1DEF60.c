#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void *memcpy(void *, const void *, u32);

/* Live particle returned by func_15130374; its extra data starts at 0xA8. */
typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ f32 unkA8;
} Particle1DEF60;


/* Spawns the task that func_151B1B34 updates for this owner. */
void func_151B1AB0(Actor *owner) {
    Task151B1AB0 *task;
    Data151B1AB0 data;

    // matching: an integer test (not != NULL) makes IDO reload owner from its arg slot for each use
    if ((s32)owner != 0) {
        data.owner = owner;
        data.ownerUnk3B = owner->unique_id;
        data.timer = 0.0f;

        task = func_151491F4(0x3C, -1, 0x15, 1, 0x11, 0xC, 0xFF, 1);
        if (task != NULL) {
            memcpy(&task->data, &data, sizeof(data));
        }
    }
}

/* Per-frame update: emits particles from two points beside the owner at a randomised rate. */
void func_151B1B34(Task151B1AB0 *task) {
    Data151B1AB0 *data;
    Particle1DEF60 *particle;
    Vec3f *origin;
    f32 size;
    ParticleDesc64 spawn;
    f32 zero; // matching: 0.0f held in one FP register for every store
    f32 extra;
    Vec3f emitPointA;
    Vec3f emitPointB;

    data = &task->data;
    data->timer += (697.0f + (func_150ADA68() * 342.0f)) * 0.001000000047f * gFrameDelta;
    if (data->timer > 1.0f) {
        if (data->owner->ob_matrix_ptr != NULL) {
            {
                Vec3f offset = {19.0f, 0.0f, 25.0f};
                func_15143134(&offset, &emitPointA, &data->owner->ob_matrix_ptr[32]);
            }
            {
                Vec3f offset = {19.0f, 0.0f, 25.0f};
                func_15143134(&offset, &emitPointB, &data->owner->ob_matrix_ptr[25]);
            }
            zero = 0.0f;
            spawn.texture = 0x29;
            spawn.renderMode = 0xE03;
            spawn.unk0 = 0x200005;
            spawn.unk4 = 0;
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

                    spawn.timer = (func_150ADA20() % 41U) + 30;
                    spawn.alpha = (func_150ADA20() % 156U) + 100;
                    size = (func_150ADA68() * 104.0f) + 199.0f;
                    spawn.height = spawn.width = size;
                    origin = (func_150ADA20() & 1) ? &emitPointA : &emitPointB;
                    baseAngle = (data->owner->langle >> 8) + 0x40;
                    angle = (func_150ADA20() % 100U) + baseAngle - 49;
                    func_15143874(angle, 36.0f, &spawn.pos.x, &spawn.pos.z);
                    spawn.pos.y = zero; // matching: dead store steers the scheduler
                    spawn.pos.x += origin->x;
                    spawn.pos.y = origin->y;
                    spawn.pos.z += origin->z;
                    func_15143874(angle, ((func_150ADA68() * 1497.0f) + 995.0f) * 0.009999999776f,
                                  &spawn.vel.x, &spawn.vel.z);
                    spawn.vel.y = zero;
                }
                spawn.flags &= ~0xC0;
                if (func_150ADA20() & 1) {
                    spawn.flags |= 0x40;
                }
                if (func_150ADA20() & 1) {
                    spawn.flags |= 0x80;
                }
                particle = func_15130374(&spawn, 1, sizeof(extra), task->base.header.owner, task->base.header.kind);
                if (particle != NULL) {
                    memcpy(&particle->unkA8, &extra, sizeof(extra));
                }
                data->timer -= 1.0f;
            } while (data->timer > 1.0f);
        } else {
            while (data->timer > 1.0f) {
                data->timer -= 1.0f;
            }
        }
    }
}

/* Message handler for the task: follows an owner swap, or destroys itself when its owner goes away. */
void func_151B1FAC(Task151B1AB0 *task, ActorMsg *msg, u8 kind) {
    Data151B1AB0 *data = &task->data;

    if (kind == 0x2D) {
        if (data->owner == msg->swap.actorA) {
            data->owner = msg->swap.actorB;
            data->ownerUnk3B = msg->swap.idB;
        } else if (data->owner == msg->swap.actorB) {
            // matching: the copy keeps IDO from moving the byte store into the branch delay slot
            Data151B1AB0 *dst = data;

            dst->owner = msg->swap.actorA;
            dst->ownerUnk3B = msg->swap.idA;
        }
    } else if (kind == 0) {
        Actor *owner = msg->ref.actor;

        if (owner == data->owner || msg->ref.uniqueId == data->ownerUnk3B) {
            func_1516972C(&task->base.header);
        }
    }
}
