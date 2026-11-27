#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"



void particle_spawnModelBurstWithBillboard(f32 arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4) {
    s32 i;
    ParticleSpawn78 sp;
    s32 pad; // matching: unused stack slot
    BillboardSpawn sp2;
    f32 scale;
    f32 speed;
    f32 sinYaw;
    f32 cosYaw;
    f32 sinPitch;
    f32 cosPitch;
    f32 spread;
    u8 yaw;
    u8 pitch;

    sp.elasticity = 0.4f;
    sp.model = 4;
    sp.scale.x = 1.0f;
    sp.scale.y = 1.0f;
    sp.scale.z = 1.0f;
    sp.rotVel.z = 0.0f;
    sp.gravity = -1.3f;
    sp.flags = 0x29E8;
    sp.variant = 0;
    sp.zone = 0;
    sp.alpha = 0xFF;
    sp.updateFn = 1;
    sp.update2Fn = 0;
    sp.floorHitFn = 0;
    sp.floorType3Fn = 0;
    sp.wallHitFn = 0;
    sp.destroyFn = 0;
    sp.squashFn = 0;
    sp.matrixFn = 0;
    sp.lightMode = 2;
    sp.owner = 0;
    sp.ownerId = 0;
    sp.fadeStart = 0x4000;
    sp.fadeRate = 0;

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
        sp.pos.x = (spread * sinYaw) + arg0;
        sp.pos.y = arg1;
        sp.pos.z = (spread * cosYaw) + arg2;
        sp.vel.x = speed * cosPitch * sinYaw;
        sp.vel.y = -speed * sinPitch;
        sp.vel.z = speed * cosPitch * cosYaw;
        sp.lifetime = (math_random() % 32U) + 0x14;
        sp.radius = scale;
        sp.sizeXZ = scale;
        sp.sizeY = scale;
        sp.rot.x = math_randomFloat() * 360.0f;
        sp.rot.y = math_randomFloat() * 360.0f;
        sp.rot.z = math_randomFloat() * 360.0f;
        sp.rotVel.x = 25.0f - (math_randomFloat() * 50.0f);
        sp.rotVel.y = 25.0f - (math_randomFloat() * 50.0f);
        particle_newModelParticle(&sp, 3, 0xFF, 0, arg3, arg4);
    }

    sp2.pos.x = arg0;
    sp2.pos.y = arg1;
    sp2.pos.z = arg2;
    sp2.width = 0x1F4;
    sp2.height = 0x1F4;
    sp2.widthStep = (math_random() % 16U) + 0x32;
    sp2.heightStep = (math_random() % 16U) + 0x32;
    sp2.velX = 0;
    sp2.velZ = 0;
    sp2.velY = (math_random() % 201U) + 0x12C;
    sp2.accelY = 0;
    sp2.life = 0x258;
    math_pickPaletteColour(1, &sp2.primColor[0], &sp2.primColor[1], &sp2.primColor[2]);
    math_pickPaletteColour(1, &sp2.envColor[0], &sp2.envColor[1], &sp2.envColor[2]);
    sp2.intensity = 0xFF;
    sp2.alpha = 0xFF;
    sp2.fadeStep = 0xA;
    sp2.onExpire = 0;
    particle_spawnGrowingBillboard(&sp2, 1);
}
