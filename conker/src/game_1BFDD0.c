#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/*
 * Two object-attached particle emitters (effects 0x23 and the caller-chosen type of func_15192D48):
 * their spawners, per-frame updates and message handlers. Each effect is a struct260 whose data at
 * 0x28 is an ObjectEmitterData.
 */

/* Data of an effect that follows an object. */
typedef struct {
    /* 0x00 */ Actor *object;
    /* 0x04 */ u8 objectId; /* object->unique_id when attached */
    /* 0x08 */ f32 spawnAccum;
} ObjectEmitterData; /* size 0xC */

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 owner;
    /* 0x0D */ u8 padD[0x28 - 0xD];
    /* 0x28 */ ObjectEmitterData data;
} Effect1BFDD0;

/* Live particle returned by func_15130374; its caller-sized data starts at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ f32 drag; /* read by update func 10 (func_151316DC) */
} Particle1BFDD0;

/* Attaches effect 0x23 to the object. */
void func_15192920(Actor *object) {
    Effect1BFDD0 *effect;
    ObjectEmitterData data;

    if (object == NULL) {
        return;
    }
    data.object = object;
    data.objectId = object->unique_id;
    data.spawnAccum = 0.0f;

    effect = (Effect1BFDD0 *)func_151491F4(0x23, -1, 0x14, 1, 0x10, 0xC, 0xFF, 1);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
}

extern s32 func_1515C0F8(Actor *, Vec3f **);
extern void func_15143874(s16 angle, f32 radius, f32 *x, f32 *z);
extern Particle1BFDD0 *func_15130374(ParticleDesc *, u8, s32, u8, s32);

/* Effect 0x23 update: emits 0.31-0.91 particles per D_800BE9A4 tick, 20 units to either side of the object,
 * moving against the direction func_1515C0F8 returns. */
void func_151929A4(Effect1BFDD0 *effect) {
    ObjectEmitterData *data;
    ParticleDesc desc;
    u8 side;
    f32 drag;
    Vec3f *dir;
    f32 speed;
    Particle1BFDD0 *particle;

    data = &effect->data;
    data->spawnAccum += (312.0f + (func_150ADA68() * 599.0f)) * 0.001f * D_800BE9A4;
    if (data->spawnAccum > 1.0f && func_1515C0F8(data->object, &dir) != 0) {
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
            side = func_150ADA20() & 1;
            desc.timer = (func_150ADA20() % 31U) + 20;
            desc.alpha = (func_150ADA20() % 156U) + 100;
            desc.width = desc.height = (func_150ADA68() * 150.0f) + 100.0f;
            func_15143874(data->object->unk7A >> 8, side ? 20.0f : -20.0f, &desc.pos.x, &desc.pos.z);
            desc.pos.x += data->object->x_position;
            desc.pos.z += data->object->z_position;
            desc.pos.y = data->object->floorY + 20.0f;
            desc.flags &= ~(PARTICLE_FLIP_S | PARTICLE_FLIP_T);
            if (func_150ADA20() & 1) {
                desc.flags |= PARTICLE_FLIP_S;
            }
            if (func_150ADA20() & 1) {
                desc.flags |= PARTICLE_FLIP_T;
            }
            desc.vel.x = dir->x * speed;
            desc.vel.y = dir->y * speed;
            desc.vel.z = dir->z * speed;
            particle = func_15130374(&desc, 1, sizeof(f32), effect->owner, effect->kind);
            if (particle != NULL) {
                memcpy(&particle->drag, &drag, sizeof(drag));
            }
            data->spawnAccum -= 1.0f;
        } while (data->spawnAccum > 1.0f);
    }
}

/* Attaches an effect of the given type to the object; returns the effect or NULL. */
Effect1BFDD0 *func_15192D48(Actor *object, s16 type, u8 arg2, s32 arg3) {
    Effect1BFDD0 *effect;
    ObjectEmitterData data;

    if (object == NULL) {
        return NULL;
    }

    data.object = object;
    data.objectId = object->unique_id;
    data.spawnAccum = 0.0f;

    effect = (Effect1BFDD0 *)func_151491F4(type, -1, 0x18, 1, 0x16, 0xC, arg2, arg3);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return effect;
}

extern void func_15143134(const Vec3f *, Vec3f *, Mtx *);
extern void func_15143794(s16 yaw, s16 pitch, f32 speed, Vec3f *out);

/* Update of func_15192D48's effect: emits 0.79-1.99 particles per D_800BE9A4 tick from bone 1 of the
 * object, flung out to either side. */
void func_15192DF0(Effect1BFDD0 *effect) {
    ObjectEmitterData *data;
    ParticleDesc desc;
    s16 facing;
    s16 yaw;
    f32 drag;
    s32 rand;
    Vec3f offset;
    Particle1BFDD0 *particle;

    data = &effect->data;
    data->spawnAccum += (792.0f + (func_150ADA68() * 1202.0f)) * 0.001f * D_800BE9A4;
    if (data->spawnAccum > 1.0f) {
        if (data->object->unk1D4 == NULL) {
            // the object has no bone matrices: spins forever
            while (data->spawnAccum > 1.0f) {
            }
        } else {
            facing = (data->object->unk7A >> 8) + 0x40;
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
                yaw = (func_150ADA20() % 51U) - 130;
                offset.x = (func_150ADA68() * 20.0f) + 30.0f;
                offset.y = (func_150ADA68() * 60.0f) + -20.0f;
                offset.z = 0.0f;
                if (func_150ADA20() & 1) {
                    offset.x = -offset.x;
                    yaw += facing;
                } else {
                    yaw = facing - yaw;
                }
                desc.timer = (func_150ADA20() % 11U) + 30;
                desc.alpha = (func_150ADA20() % 101U) + 100;
                desc.width = desc.height = (func_150ADA68() * 150.0f) + 100.0f;
                func_15143134(&offset, &desc.pos, (Mtx *)data->object->unk1D4 + 1);
                rand = func_150ADA20();
                func_15143794(yaw, (rand % 28U) - 25, func_150ADA68() * 3998.0f * 0.001f, &desc.vel);
                desc.flags &= ~(PARTICLE_FLIP_S | PARTICLE_FLIP_T);
                if (func_150ADA20() & 1) {
                    desc.flags |= PARTICLE_FLIP_S;
                }
                if (func_150ADA20() & 1) {
                    desc.flags |= PARTICLE_FLIP_T;
                }
                particle = func_15130374(&desc, 1, sizeof(f32), effect->owner, effect->kind);
                if (particle != NULL) {
                    memcpy(&particle->drag, &drag, sizeof(drag));
                }
                data->spawnAccum -= 1.0f;
            } while (data->spawnAccum > 1.0f);
        }
    }
}

/* Payload of message 0x2D: two objects trade places. */
typedef struct {
    /* 0x00 */ Actor *from;
    /* 0x04 */ Actor *to;
    /* 0x08 */ u8 fromId;
    /* 0x09 */ u8 toId;
} ObjectSwapMessage;

/* Payload of message 0: names an object by pointer or by unique id. */
typedef struct {
    /* 0x00 */ Actor *object;
    /* 0x04 */ u8 objectId;
} ObjectRemovedMessage;

/* Message handler: message 0x2D moves the effect to the other object of a swap; message 0 ends the
 * effect when it names the effect's object. */
void func_15193234(Effect1BFDD0 *effect, void *msg, u8 msgType) {
    ObjectEmitterData *data = &effect->data;
    ObjectSwapMessage *swap = msg;
    ObjectRemovedMessage *removal = msg;
    Actor *removed;

    if (msgType == 0x2D) {
        if (data->object == swap->from) {
            data->object = swap->to;
            data->objectId = swap->toId;
        } else if (data->object == swap->to) {
            ObjectEmitterData *dst = data; // matching: the -g3 scope keeps the store out of the b delay slot

            dst->object = swap->from;
            dst->objectId = swap->fromId;
        }
    } else if (msgType == 0) {
        removed = removal->object;
        if (removed == data->object || removal->objectId == data->objectId) {
            func_1516972C((struct102 *)effect);
        }
    }
}

/* Message handler of func_15192D48's effect: the same as func_15193234. */
void func_151932E0(Effect1BFDD0 *effect, void *msg, u8 msgType) {
    ObjectEmitterData *data = &effect->data;
    ObjectSwapMessage *swap = msg;
    ObjectRemovedMessage *removal = msg;
    Actor *removed;

    if (msgType == 0x2D) {
        if (data->object == swap->from) {
            data->object = swap->to;
            data->objectId = swap->toId;
        } else if (data->object == swap->to) {
            ObjectEmitterData *dst = data; // matching: the -g3 scope keeps the store out of the b delay slot

            dst->object = swap->from;
            dst->objectId = swap->fromId;
        }
    } else if (msgType == 0) {
        removed = removal->object;
        if (removed == data->object || removal->objectId == data->objectId) {
            func_1516972C((struct102 *)effect);
        }
    }
}
