#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Gets the position of the actor's bone 0x18 (side 1) or 0x15 (side 2). */
s32 func_150C19C0(Vec3f *pos, Actor *actor, u8 side) {
    s32 bone;

    switch (side) {
    case 1:
        bone = 0x18;
        break;
    case 2:
        bone = 0x15;
        break;
    }

    math_getBoneTranslation(actor->ob_matrix_ptr, bone, pos);
    return 1;
}

s32 func_150C1A2C(s32 arg0, s32 arg1) {
    return 7;
}


/*
 * Spawns an effect at the actor's bone for `side` (see func_150C19C0): a ring of particles and a
 * flat quad at the actor's unk180 height, and 5 to 12 debris particles flying outwards from it.
 */
void func_150C1A40(Actor *actor, s32 side, s32 arg2) {
    Vec3f pos;
    f32 scale;
    ParticleSpawn spawn;
    f32 radius;
    u8 angle;
    f32 speed;
    f32 cosine;
    f32 sine;
    FloorParticle *particle;
    u32 count;
    u8 startAngle;

    if (actor->ob_matrix_ptr == NULL) {
        return;
    }

    scale = (actor->scale_xz + actor->scale_y) * 0.5f;
    func_150C19C0(&pos, actor, side);
    startAngle = (actor->angle >> 8) + 0x40;
    radius = scale * 70.0f;
    particle_emitOnCircle(pos.x, actor->floorY, pos.z, radius, startAngle,
                  (side == 1) ? 16 : -16, 8, 0, 0, radius * 4.04f, 0, 255);
    effect_spawnQuadEffect(-1, pos.x, (s32)actor->floorY + 2.0f, pos.z, 8, 21, 0, 255, 0);
    spawn.model = 5;
    spawn.flags = 0x29E9;
    spawn.variant = 0;
    spawn.unk5C = NULL;
    spawn.alpha = 255;
    spawn.updateFn = 1;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 0;
    spawn.unk6A = 1;
    spawn.owner = actor;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.elasticity = 0.4f;
    spawn.rotVel.z = 0.0f;
    spawn.gravity = -0.9f;
    spawn.ownerId = actor->unique_id;
    spawn.fadeStart = 16;
    spawn.fadeRate = 15;
    count = (func_150ADA20_u8() & 7) + 5;
    while (count--) {
        // matching: masking the already-u8 random byte keeps IDO's andi/move pair
        angle = func_150ADA20_u8() & 0xFF;
        speed = (0.1f + func_150ADA68() * 0.9f) * 9.0f * scale;
        cosine = math_cosBinAngleMasked(angle - 0x40);
        sine = math_cosBinAngleMasked(angle);
        spawn.lifetime = (func_150ADA20_u8() & 15) + 30;
        spawn.pos.x = radius * cosine + pos.x;
        spawn.pos.y = actor->floorY + 2.0f;
        spawn.pos.z = radius * sine + pos.z;
        spawn.vel.x = speed * cosine;
        spawn.vel.y = 2.0f * speed;
        spawn.vel.z = speed * sine;
        spawn.sizeY = spawn.sizeXZ = spawn.radius = (0.1f + func_150ADA68() * 0.9f) * scale * 1.5f;
        spawn.rot.x = func_150ADA68() * 360.0f;
        spawn.rot.y = func_150ADA68() * 360.0f;
        spawn.rot.z = func_150ADA68() * 360.0f;
        spawn.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
        spawn.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
        particle = particle_newModelParticle(&spawn, 3, 255, sizeof(f32), 255, 0);
        if (particle != NULL) {
            particle->floorY = actor->floorY;
        }
    }
}


/*
 * Spawns a growing billboard particle of size `size` at (x, y, z), moving horizontally away from
 * (fromX, fromZ), with random colours from palette 0.
 */
s32 func_150C1E34(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, f32 fromX, f32 fromY, f32 fromZ,
                  s32 arg8, s32 arg9, s32 argA, s32 argB, f32 size) {
    BillboardSpawn effect;

    effect.pos.x = x;
    effect.pos.y = y;
    effect.pos.z = z;
    effect.velX = (x - fromX) * 10.0f;
    effect.velZ = (z - fromZ) * 10.0f;
    effect.width = effect.height = size;
    effect.heightStep = effect.widthStep = size * 0.1757;
    effect.velY = 150;
    effect.accelY = 20;
    effect.life = 600;
    math_pickPaletteColour(0, &effect.primColor[0], &effect.primColor[1], &effect.primColor[2]);
    math_pickPaletteColour(0, &effect.envColor[0], &effect.envColor[1], &effect.envColor[2]);
    effect.intensity = 255;
    effect.alpha = 255;
    effect.fadeStep = (func_150ADA20_u8() & 15) + 12;
    effect.onExpire = 1;
    particle_spawnGrowingBillboard(&effect, 1);
    return 1;
}
