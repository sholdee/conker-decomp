#include <ultra64.h>
#include "functions.h"
#include "variables.h"



void func_1518B6B0(f32 arg0, f32 arg1, f32 arg2, u8 arg3, s32 arg4) {
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
    sp.unk5C = 0;
    sp.alpha = 0xFF;
    sp.updateFn = 1;
    sp.update2Fn = 0;
    sp.floorHitFn = 0;
    sp.floorType3Fn = 0;
    sp.wallHitFn = 0;
    sp.destroyFn = 0;
    sp.squashFn = 0;
    sp.matrixFn = 0;
    sp.unk6A = 2;
    sp.owner = 0;
    sp.ownerId = 0;
    sp.fadeStart = 0x4000;
    sp.fadeRate = 0;

    for (i = (func_150ADA20() % 8U) + 3; i > 0; i--) {
        scale = (func_150ADA68() * 0.7f) + 0.3f;
        yaw = func_150ADA20() & 0xFF;
        pitch = -0x10 - (func_150ADA20() % 32U);
        speed = (func_150ADA68() * 10.0f) + 5.0f;
        sinYaw = func_151423D8((u8)(yaw - 0x40));
        cosYaw = func_151423D8(yaw);
        sinPitch = func_151423D8((u8)(pitch - 0x40));
        cosPitch = func_151423D8(pitch);
        spread = func_150ADA68() * 20.0f;
        sp.pos.x = (spread * sinYaw) + arg0;
        sp.pos.y = arg1;
        sp.pos.z = (spread * cosYaw) + arg2;
        sp.vel.x = speed * cosPitch * sinYaw;
        sp.vel.y = -speed * sinPitch;
        sp.vel.z = speed * cosPitch * cosYaw;
        sp.lifetime = (func_150ADA20() % 32U) + 0x14;
        sp.radius = scale;
        sp.sizeXZ = scale;
        sp.sizeY = scale;
        sp.rot.x = func_150ADA68() * 360.0f;
        sp.rot.y = func_150ADA68() * 360.0f;
        sp.rot.z = func_150ADA68() * 360.0f;
        sp.rotVel.x = 25.0f - (func_150ADA68() * 50.0f);
        sp.rotVel.y = 25.0f - (func_150ADA68() * 50.0f);
        func_15132A4C(&sp, 3, 0xFF, 0, arg3, arg4);
    }

    sp2.pos.x = arg0;
    sp2.pos.y = arg1;
    sp2.pos.z = arg2;
    sp2.width = 0x1F4;
    sp2.height = 0x1F4;
    sp2.widthStep = (func_150ADA20() % 16U) + 0x32;
    sp2.heightStep = (func_150ADA20() % 16U) + 0x32;
    sp2.velX = 0;
    sp2.velZ = 0;
    sp2.velY = (func_150ADA20() % 201U) + 0x12C;
    sp2.accelY = 0;
    sp2.life = 0x258;
    func_151429E0(1, &sp2.primColor[0], &sp2.primColor[1], &sp2.primColor[2]);
    func_151429E0(1, &sp2.envColor[0], &sp2.envColor[1], &sp2.envColor[2]);
    sp2.intensity = 0xFF;
    sp2.alpha = 0xFF;
    sp2.fadeStep = 0xA;
    sp2.onExpire = 0;
    func_1518CA80(&sp2, 1);
}
