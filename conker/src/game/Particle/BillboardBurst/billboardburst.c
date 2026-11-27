#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"



void particle_spawnModelBurstWithBillboard(f32 x, f32 y, f32 z, u8 owner, s32 list) {
    s32 i;
    ParticleSpawn78 spawn;
    s32 pad; // matching: unused stack slot
    BillboardSpawn billboard;
    f32 scale;
    f32 speed;
    f32 sinYaw;
    f32 cosYaw;
    f32 sinPitch;
    f32 cosPitch;
    f32 spread;
    u8 yaw;
    u8 pitch;

    spawn.elasticity = 0.4f;
    spawn.model = 4;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.rotVel.z = 0.0f;
    spawn.gravity = -1.3f;
    spawn.flags = 0x29E8;
    spawn.variant = 0;
    spawn.zone = 0;
    spawn.alpha = 0xFF;
    spawn.updateFn = 1;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.lightMode = 2;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = 0x4000;
    spawn.fadeRate = 0;

    for (i = (math_random() % 8U) + 3; i > 0; i--) {
        scale = (math_randomFloat() * 0.7f) + 0.3f;
        yaw = math_random() & 0xFF;
        pitch = -0x10 - (math_random() % 32U);
        speed = (math_randomFloat() * 10.0f) + 5.0f;
        sinYaw = math_cosBinAngleMasked(yaw - 0x40);
        cosYaw = math_cosBinAngleMasked(yaw);
        sinPitch = math_cosBinAngleMasked(pitch - 0x40);
        cosPitch = math_cosBinAngleMasked(pitch);
        spread = math_randomFloat() * 20.0f;
        spawn.pos.x = (spread * sinYaw) + x;
        spawn.pos.y = y;
        spawn.pos.z = (spread * cosYaw) + z;
        spawn.vel.x = speed * cosPitch * sinYaw;
        spawn.vel.y = -speed * sinPitch;
        spawn.vel.z = speed * cosPitch * cosYaw;
        spawn.lifetime = (math_random() % 32U) + 0x14;
        spawn.radius = scale;
        spawn.sizeXZ = scale;
        spawn.sizeY = scale;
        spawn.rot.x = math_randomFloat() * 360.0f;
        spawn.rot.y = math_randomFloat() * 360.0f;
        spawn.rot.z = math_randomFloat() * 360.0f;
        spawn.rotVel.x = 25.0f - (math_randomFloat() * 50.0f);
        spawn.rotVel.y = 25.0f - (math_randomFloat() * 50.0f);
        particle_newModelParticle(&spawn, 3, 0xFF, 0, owner, list);
    }

    billboard.pos.x = x;
    billboard.pos.y = y;
    billboard.pos.z = z;
    billboard.width = 0x1F4;
    billboard.height = 0x1F4;
    billboard.widthStep = (math_random() % 16U) + 0x32;
    billboard.heightStep = (math_random() % 16U) + 0x32;
    billboard.velX = 0;
    billboard.velZ = 0;
    billboard.velY = (math_random() % 201U) + 0x12C;
    billboard.accelY = 0;
    billboard.life = 0x258;
    math_pickPaletteColour(1, &billboard.primColor[0], &billboard.primColor[1], &billboard.primColor[2]);
    math_pickPaletteColour(1, &billboard.envColor[0], &billboard.envColor[1], &billboard.envColor[2]);
    billboard.intensity = 0xFF;
    billboard.alpha = 0xFF;
    billboard.fadeStep = 0xA;
    billboard.onExpire = 0;
    particle_spawnGrowingBillboard(&billboard, 1);
}
