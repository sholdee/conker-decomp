#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Spawns two effects on the ground under the actor, probing from 1000 units above it to 3000 below. */
void particle_spawnGroundEffects(Actor *actor, u8 owner, s32 arg2) {
    Vec3f pos;

    if (actor == NULL) {
        return;
    }
    {
        CollisionHit hit;

        collision_initHitFromObject(&hit, actor);
        pos.x = actor->xpos;
        pos.y = actor->ypos + 1000.0f;
        pos.z = actor->zpos;
        if (!collision_probeFloorSurfABSet0Cached(&pos, 0, actor->ypos - 3000.0f, &hit)) {
            return;
        }
        pos.y = hit.height;
    }
    particle_emitOnRandomCircle(pos.x, pos.y, pos.z, 251.0f, 0, 0xFF, (func_150ADA20() & 0xF) + 35, 0x17, 0, 0.0f, 0, owner);
    particle_emitOnRandomCircle(pos.x, pos.y, pos.z, 290.0f, 0, 0xFF, (func_150ADA20() % 21U) + 30, 0x18, 0, 0.0f, 0, owner);
}

/* Spawns a randomly sized purple/yellow particle (kind 1) at (x, y, z), launched along `yaw` at a random pitch. */
s32 particle_spawnYawColorSprite(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9,
                  s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14) {
    SpriteParticle *particle;
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
    size = (func_150ADA68() * 401.0f) + 404.0f;
    desc.width = size;
    desc.height = size;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;
    random = func_150ADA20();
    speed = func_150ADA68();
    math_dirFromBinAngles(yaw, (random % 20U) - 19, ((speed * 150.0f) + 150.0f) * 0.1f, &desc.vel);
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
    particle = particle_newOwnedSpriteParticle(&desc, 1, sizeof(f32), arg14, 1);
    if (particle != NULL) {
        memcpy(&particle->data.drag, &unkA8, sizeof(f32));
    }
    return 1;
}

/* Spawns a spinning model-0x20 particle at (x, y, z), launched along `yaw` at a random pitch. */
s32 particle_spawnYawSpinModel(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 yaw, s32 arg9,
                  s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14) {
    s32 pad; // matching: unused stack slot
    ParticleSpawn spawn;
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
    math_dirFromBinAngles(yaw, (random % 41U) - 61, ((speed * 101.0f) + 150.0f) * 0.1f, &spawn.vel);
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
    particle_newModelParticle(&spawn, 3, 0xFF, 0, arg14, 0);
    return 1;
}
