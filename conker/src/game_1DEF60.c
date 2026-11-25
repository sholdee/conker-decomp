#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

void *memcpy(void *, const void *, u32);
void func_15143134(vertex *in, vertex *out, s32 angle);
void func_15143874(s16 angle, f32 dist, f32 *x, f32 *z);

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
    u8 pad3C[0x7A - 0x3C];
    u16 unk7A;
    u8 pad7C[0x1D4 - 0x7C];
    s32 unk1D4;
} Owner151B1AB0;

/* Per-task data kept at offset 0x28 of the task created by func_151B1AB0. */
typedef struct {
    Owner151B1AB0 *owner;
    u8 ownerUnk3B;
    f32 timer;
} Data151B1AB0;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x1B];
    Data151B1AB0 data;
} Task151B1AB0;

/* Message passed to func_151B1FAC: kind 0x2D swaps two owners, kind 0 names an owner to drop. */
typedef union {
    struct {
        Owner151B1AB0 *a;
        Owner151B1AB0 *b;
        u8 aUnk3B;
        u8 bUnk3B;
    } swap;
    struct {
        Owner151B1AB0 *owner;
        u8 ownerUnk3B;
    } remove;
} Msg151B1FAC;

/* matching: the first 0x64 bytes of ParticleDesc; a full ParticleDesc on the stack changes the frame. */
typedef struct ParticleDesc64 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode; /* low byte: D_800A4AC8 index, high byte: func_1513F4E4 arg */
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;      /* 16.16 animation frame */
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;     /* D_80090B60 index */
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex offset;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ u32 flags;      /* PARTICLE_* */
    /* 0x5C */ struct ParticleViewMask *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* D_80089760 index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* D_80089800 index, -1 for none */
} ParticleDesc64;

/* Live particle returned by func_15130374; its extra data starts at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ f32 unkA8;
} Particle1DEF60;

Particle1DEF60 *func_15130374(ParticleDesc64 *spawn, u8 kind, s32 dataSize, u8 arg3, s32 arg4);


/* Spawns the task that func_151B1B34 updates for this owner. */
void func_151B1AB0(Owner151B1AB0 *owner) {
    Task151B1AB0 *task;
    Data151B1AB0 data;

    // matching: an integer test (not != NULL) makes IDO reload owner from its arg slot for each use
    if ((s32)owner != 0) {
        data.owner = owner;
        data.ownerUnk3B = owner->unk3B;
        data.timer = 0.0f;

        task = (Task151B1AB0 *)func_151491F4(0x3C, -1, 0x15, 1, 0x11, 0xC, 0xFF, 1);
        if (task != NULL) {
            memcpy(&task->data, &data, sizeof(data));
        }
    }
}

/* Per-frame update: emits particles from two points beside the owner at a randomised rate. */
void func_151B1B34(Task151B1AB0 *task) {
    Data151B1AB0 *data;
    Particle1DEF60 *particle;
    vertex *origin;
    f32 size;
    ParticleDesc64 spawn;
    f32 zero; // matching: 0.0f held in one FP register for every store
    f32 extra;
    vertex emitPointA;
    vertex emitPointB;

    data = &task->data;
    data->timer += (697.0f + (func_150ADA68() * 342.0f)) * 0.001000000047f * D_800BE9A4;
    if (data->timer > 1.0f) {
        if (data->owner->unk1D4 != 0) {
            {
                vertex offset = {19.0f, 0.0f, 25.0f};
                func_15143134(&offset, &emitPointA, data->owner->unk1D4 + 0x800);
            }
            {
                vertex offset = {19.0f, 0.0f, 25.0f};
                func_15143134(&offset, &emitPointB, data->owner->unk1D4 + 0x640);
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
                    baseAngle = (data->owner->unk7A >> 8) + 0x40;
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
                particle = func_15130374(&spawn, 1, sizeof(extra), task->unkC, task->unk1);
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
void func_151B1FAC(Task151B1AB0 *task, Msg151B1FAC *msg, u8 kind) {
    Data151B1AB0 *data = &task->data;

    if (kind == 0x2D) {
        if (data->owner == msg->swap.a) {
            data->owner = msg->swap.b;
            data->ownerUnk3B = msg->swap.bUnk3B;
        } else if (data->owner == msg->swap.b) {
            // matching: the copy keeps IDO from moving the byte store into the branch delay slot
            Data151B1AB0 *dst = data;

            dst->owner = msg->swap.a;
            dst->ownerUnk3B = msg->swap.aUnk3B;
        }
    } else if (kind == 0) {
        Owner151B1AB0 *owner = msg->remove.owner;

        if (owner == data->owner || msg->remove.ownerUnk3B == data->ownerUnk3B) {
            func_1516972C((struct102 *)task);
        }
    }
}
