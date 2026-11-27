#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"

s32 particle_selectObjGroundBurst(s32 arg0, s32 arg1) {
    return 10;
}

/* Spawns an effect burst at the object's x/z and unk180 height (particle_emitOnRandomCircle, particle_startPlayerRumbles, particle_spawnLightFlash). */
void particle_spawnObjGroundBurst(Actor *obj, s32 arg1, s32 arg2) {
    Vec3f position;

    position.x = obj->xpos;
    position.y = obj->floorY;
    position.z = obj->zpos;
    particle_emitOnRandomCircle(position.x, position.y, position.z, 135.0f, 0, 255,
                  (math_random() % 15U) + 27, 7, 0, 0.0f, 0, 255);
    particle_startPlayerRumbles(&position, 1502.0f, 3000.0f, 0.00033333333f, 12, 15, 255, 0);
    particle_spawnLightFlash(&position, 255, 0);
}

/*
 * Spawns one streak particle at (x, y, z) heading along `yaw` (binary angle) with a random pitch,
 * random speeds, random colours and lifetime. math_cosBinAngleMasked is cos, so cos(a - 64) is sin(a).
 */
s32 particle_spawnYawStreak(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s32 yaw, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 owner) {
    f32 horizontal;
    StreakParticleDesc desc;
    s16 pitch;
    f32 sinYaw;
    f32 cosYaw;
    f32 sinPitch;

    sinYaw = math_cosBinAngleMasked(yaw - 64);
    cosYaw = math_cosBinAngleMasked(yaw);
    pitch = (math_random() % 21U) - 30;
    sinPitch = math_cosBinAngleMasked(pitch - 64);
    horizontal = math_cosBinAngleMasked(pitch);
    horizontal = 10.0f * horizontal; // matching: a separate statement steers IDO's scheduling of the product
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;
    desc.dir.x = horizontal * sinYaw;
    desc.dir.y = -10.0f * sinPitch;
    desc.dir.z = horizontal * cosYaw;

    desc.headSpeed = (math_randomFloat() * 0.8590000272f) + 3.608000278f;
    desc.tailSpeed = (math_randomFloat() * 0.3090000153f) + 2.371000051f;
    math_pickPaletteColour(8, &desc.headColor.red, &desc.headColor.green, &desc.headColor.blue);
    math_pickPaletteColour(8, &desc.tailColor.red, &desc.tailColor.green, &desc.tailColor.blue);
    desc.headColor.alpha = 255;
    desc.tailColor.alpha = 255;
    desc.flags = 9;
    desc.lifetime = (math_random() % 7U) + 18;

    desc.width = (math_randomFloat() * 1.389999986f) + 3.210000038f;
    desc.renderMode = 0x1601;
    desc.alpha = 255;
    desc.fadeStart = 8;
    desc.fadeRate = 31;
    particle_newOwnedStreakParticle(&desc, 1, 0, owner, 1);
    return 1;
}
