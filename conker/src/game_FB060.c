#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

extern f32 D_800A07B4; // matching: 401.0f; as a literal IDO allocates the FP temps in a different order

void func_15143794(s16 yaw, s16 pitch, f32 distance, f32 *out);

/* Ground contact found by func_15046C80 below a probe point (as in game_173D40.c). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 surface[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 type;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 unk20;
} GroundHit; // size 0x24

void func_1504715C(GroundHit *hit, Actor *actor);
s32 func_15046C80(Vec3f *probe, s32 arg1, f32 bottom, GroundHit *hit);
void func_1514C678(f32 x, f32 y, f32 z, f32 radius, s16 arg4, s16 arg5, s16 arg6, u8 arg7, s32 arg8,
                   f32 arg9, s32 argA, u8 argB);

/* Spawns two effects on the ground under the actor, probing from 1000 units above it to 3000 below. */
void func_150CDBB0(Actor *actor, u8 owner, s32 arg2) {
    Vec3f pos;

    if (actor == NULL) {
        return;
    }
    {
        GroundHit hit;

        func_1504715C(&hit, actor);
        pos.x = actor->x_position;
        pos.y = actor->y_position + 1000.0f;
        pos.z = actor->z_position;
        if (!func_15046C80(&pos, 0, actor->y_position - 3000.0f, &hit)) {
            return;
        }
        pos.y = hit.height;
    }
    func_1514C678(pos.x, pos.y, pos.z, 251.0f, 0, 0xFF, (func_150ADA20() & 0xF) + 35, 0x17, 0, 0.0f, 0, owner);
    func_1514C678(pos.x, pos.y, pos.z, 290.0f, 0, 0xFF, (func_150ADA20() % 21U) + 30, 0x18, 0, 0.0f, 0, owner);
}

/* A particle object (same layout as Particle in game_15D730.c); its kind's own data follows at +0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ ParticleDesc desc;
    /* 0x80 */ u8 pad80[0x28];
    /* 0xA8 */ f32 unkA8;
} Particle;

Particle *func_15130374(ParticleDesc *desc, u8 kind, s32 dataSize, u8 arg3, s32 arg4);

/* Spawns a randomly sized purple/yellow particle (kind 1) at (x, y, z), launched along `yaw` at a random pitch. */
s32 func_150CDCF4(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9,
                  s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14) {
    Particle *particle;
    ParticleDesc desc;
    f32 size;
    f32 unkA8;
    f32 speed;
    u32 random;

    desc.texture = 41;
    desc.renderMode = 0xE03;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.timer = (func_150ADA20() % 41U) + 50;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r2 = 176;
    desc.g2 = 160;
    desc.b2 = 42;
    desc.r = 64;
    desc.g = 11;
    desc.b = 106;
    desc.a = 255;
    desc.alpha = (func_150ADA20() % 101U) + 100;
    desc.brightness = 255;
    desc.primColorMode = 3;
    desc.envColorMode = 3;
    size = (func_150ADA68() * D_800A07B4) + 404.0f;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;
    desc.width = size;
    desc.height = size;
    random = func_150ADA20();
    speed = func_150ADA68();
    func_15143794(yaw, (random % 20U) - 19, ((speed * 150.0f) + 150.0f) * 0.1f, &desc.vel.x);
    desc.flags = 0xE05;
    desc.gravity = 0.0f;
    if (func_150ADA20() & 1) {
        desc.flags |= PARTICLE_FLIP_S;
    }
    if (func_150ADA20() & 1) {
        desc.flags |= PARTICLE_FLIP_T;
    }
    desc.updateFunc = 10;
    desc.deathFunc = -1;
    desc.fadeStart = 30;
    desc.fadeRate = 8;
    desc.scaleStart = 70;
    desc.scaleRate = 1.0196f;
    unkA8 = 0.95091f;
    particle = func_15130374(&desc, 1, sizeof(f32), arg14, 1);
    if (particle != NULL) {
        memcpy(&particle->unkA8, &unkA8, sizeof(f32));
    }
    return 1;
}

s32 func_15132A4C(ParticleSpawn80 *spawn, s32, s32, s32, u8, s32);

/* Spawns a spinning model-0x20 particle at (x, y, z), launched along `yaw` at a random pitch. */
s32 func_150CDF10(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9,
                  s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14) {
    ParticleSpawn80 spawn;
    f32 speed;
    u32 random;

    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.flags = 0x29E8;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.rotVel.y = 0.0f;
    spawn.radius = 1.0f;
    spawn.model = 0x20;
    spawn.pos.z = z;
    spawn.elasticity = 0.2f;
    random = func_150ADA20();
    speed = func_150ADA68();
    func_15143794(yaw, (random % 41U) - 61, ((speed * 101.0f) + 150.0f) * 0.1f, &spawn.vel.x);
    spawn.rotVel.x = ((func_150ADA68() * 260.0f) + -130.0f) * 0.1f;
    spawn.rotVel.z = ((func_150ADA68() * 260.0f) + -130.0f) * 0.1f;
    spawn.lifetime = (func_150ADA20() % 21U) + 50;
    spawn.gravity = ((func_150ADA68() * 311.0f) + -1217.0f) * 0.001f;
    spawn.sizeY = ((func_150ADA68() * 302.0f) + 52.0f) * 0.001f;
    spawn.sizeXZ = spawn.sizeY;
    spawn.variant = 0;
    spawn.unk5C = NULL;
    spawn.alpha = (func_150ADA20() % 76U) + 180;
    spawn.updateFn = 8;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.unk6A = 2;
    spawn.owner = NULL;
    spawn.ownerId = 0;
    spawn.fadeStart = 32;
    spawn.fadeRate = 7;
    func_15132A4C(&spawn, 3, 0xFF, 0, arg14, 0);
    return 1;
}
