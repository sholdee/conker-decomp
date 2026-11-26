#include <ultra64.h>
#include "functions.h"
#include "variables.h"

struct Queue *D_80088750 = NULL; /* ground decals (func_150BABE0); game_3F820.c makes it at level setup */

/* matching: the joint owner is passed as s32; a pointer parameter changes register allocation */

/*
 * Spawns the effects for one of obj's joints (side 0: joint 0x11, side 1: joint 0x1A): a sound,
 * two particle bursts and, when the ground below is found, a ground decal.
 */
// matching: each `*(u8 *)&arg2` reloads arg2 from its stack slot; plain or K&R arg2 is kept in a register
void func_150BABE0(Actor *obj, u8 side, u8 arg2) {
    Vec3f pos;
    s32 joint;
    s32 yaw;
    CollisionHit ground;
    Vec3f probe;
    SpriteDef desc;
    QuadSprite *decal;
    f32 size;

    /* the current camera's unk380 angle, degrees -> 256-step angle */
    yaw = camera_global_Cameras[gCurrentView].TrueCameraAngle * 0.7111111283f;
    if (obj == NULL) {
        return;
    }
    if (obj->type == 0 || obj->type == 8) {
        return;
    }
    if (obj->ob_matrix_ptr == NULL) {
        return;
    }
    switch (side) {
    case 0:
        joint = 0x11;
        break;
    case 1:
        joint = 0x1A;
        break;
    }
    // matching: an integer cast keeps ob_matrix_ptr out of a0 (a pointer argument is loaded straight into a0)
    func_15142314((s32)obj->ob_matrix_ptr, joint, &pos);
    pos.y = obj->floorY;
    func_15143E94(5, 0x4022);
    func_15165F80(-1, pos.x, pos.y + 6.0f, pos.z, 25, 18, 0, *(u8 *)&arg2, 1);
    func_1514C678(pos.x, pos.y, pos.z, func_150ADA68() * 50.0f + 40.0f, yaw + 60, yaw - 60,
                  func_150ADA20() % 11U + 30, 5, 0, 0.0f, 0, *(u8 *)&arg2);
    collision_initHitFromObject(&ground, obj);
    probe.x = pos.x;
    probe.y = pos.y + 100.0f;
    probe.z = pos.z;
    if (collision_probeFloorSurfABSet0Cached(&probe, 0, pos.y - 100.0f, &ground) != 0) {
        size = func_150ADA68() * 200.0f + 200.0f;
        desc.texture = 0x2A;
        desc.unk7 = 0;
        desc.flags = 0x7340;
        desc.lifetime = 300;
        desc.frame = 0;
        desc.frameSpeed = func_150ADA20() % 2001U + 2000;
        desc.unk10 = 0xFF;
        desc.unk11 = 0xFF;
        desc.rgba[0] = 0;
        desc.rgba[1] = 0;
        desc.rgba[2] = 0;
        desc.rgba[3] = 0xFF;
        desc.unk16 = 0;
        desc.unk17 = 0xD;
        desc.viewportMask = 0xFF;
        desc.unk1C = 0;
        desc.renderModes = 0x170002;
        decal = func_1513C73C(&desc, 0, 0, &ground.tri, pos.x, ground.height, pos.z, size, size,
                              func_150ADA20() & 0xFF, (func_150ADA20() & 1) * 2 + (func_150ADA20() & 1), 4, *(u8 *)&arg2, 0);
        if (decal != NULL && func_1518ABD0(D_80088750, decal, 1) == 0) {
            func_1516972C(&decal->header);
        }
    }
    func_1514C678(pos.x, pos.y, pos.z, func_150ADA68() * 40.0f + 60.0f, yaw + 60, yaw - 60,
                  func_150ADA20() % 7U + 6, 6, 0, 0.0f, 0, *(u8 *)&arg2);
}

/* Spawns one particle at (x, y, z) flying off along `yaw` at a random pitch; it slows down by 0.95 per frame. */
s32 func_150BAFEC(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw,
                  s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    f32 speed;
    SparkParticle *particle;
    ParticleDesc68 desc;
    s32 pad; // matching: unused stack slot
    f32 damping;
    s16 pitch;
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;

    cosYaw = func_151423D8(yaw - 0x40);
    sinYaw = func_151423D8(yaw);
    desc.texture = 0x29;
    desc.renderMode = 0xE03;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.timer = func_150ADA20() % 36U + 70;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r2 = 0xB0;
    desc.g2 = 0xA0;
    desc.b2 = 0x2A;
    desc.r = 0x40;
    desc.g = 0xB;
    desc.b = 0x6A;
    desc.a = 0xFF;
    desc.alpha = func_150ADA20() % 52U + 90;
    desc.brightness = 0xFF;
    desc.primColorMode = 3;
    desc.envColorMode = 3;
    desc.width = desc.height = func_150ADA68() * 165.0f + 800.0f;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;
    pitch = func_150ADA20() % 11U - 12;
    cosPitch = func_151423D8(pitch - 0x40);
    sinPitch = func_151423D8(pitch);
    speed = func_150ADA68() * 11.0f + 15.0f;
    desc.flags = 0xE05;
    desc.vel.x = speed * sinPitch * cosYaw;
    desc.vel.y = -speed * cosPitch;
    desc.vel.z = speed * sinPitch * sinYaw;
    desc.gravity = 0.0f;
    if (func_150ADA20() & 1) {
        desc.flags |= 0x40;
    }
    if (func_150ADA20() & 1) {
        desc.flags |= 0x80;
    }
    desc.updateFunc = 2;
    desc.deathFunc = -1;
    desc.fadeStart = 22;
    desc.fadeRate = 11;
    desc.scaleStart = 1;
    desc.scaleRate = 1.02f;
    damping = 0.95f;
    particle = func_15130374(&desc, 0, sizeof(damping), argE, 1);
    if (particle != NULL) {
        memcpy(&particle->damping, &damping, sizeof(damping));
    }
    return 1;
}

/* Per-frame update of a func_150BAFEC particle: damps its horizontal velocity once per elapsed frame. */
s32 func_150BB260(SparkParticle *particle, s32 arg1) {
    s32 i;
    f32 *damping; // matching: reading particle->damping directly changes the loop code

    damping = &particle->damping;
    for (i = D_800BE9E4; i != 0; i--) {
        particle->head.desc.vel.x *= *damping;
        particle->head.desc.vel.z *= *damping;
    }
    return 1;
}

/* Init callback: 50 frames of life, fading out over all of them at 5 alpha per frame. */
void func_150BB408(FadeQuadSprite *sprite) {
    sprite->quad.def.flags |= 1;
    sprite->fadeStart = sprite->quad.def.lifetime = 50;
    sprite->fadeStep = 5;
    func_1513F6C0(&sprite->quad, 6, sprite->quad.expireFn);
}

/* Update callback: once life drops below fadeStart, caps alpha at life * fadeStep. */
s32 func_150BB450(FadeQuadSprite *sprite) {
    s16 alpha;

    if (sprite->quad.def.lifetime < sprite->fadeStart) {
        alpha = sprite->quad.def.lifetime * sprite->fadeStep;
        if (alpha < sprite->quad.def.unk10) {
            sprite->quad.def.unk10 = alpha;
        }
    }
    return 1;
}

/* Spawns one piece at (x, y, z) thrown along `yaw` at a random pitch, with random spin and size. */
s32 func_150BB498(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s32 yaw,
                  s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14) {
    s16 pitch;
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;
    f32 speed;
    ParticleSpawn piece;
    f32 scale;

    cosYaw = func_151423D8(yaw - 0x40);
    sinYaw = func_151423D8(yaw);

    piece.flags = 0x9E8;
    piece.rot.x = 0.0f;
    piece.rot.y = 0.0f;
    piece.rot.z = 0.0f;
    piece.scale.x = 1.0f;
    piece.scale.y = 1.0f;
    piece.scale.z = 1.0f;
    piece.pos.x = x;
    piece.pos.y = y;
    piece.pos.z = z;
    piece.elasticity = 0.2f;

    pitch = func_150ADA20() % 13U - 52;
    cosPitch = func_151423D8(pitch - 0x40);
    sinPitch = func_151423D8(pitch);

    speed = func_150ADA68() * 9.0f + 3.0f;
    piece.vel.x = speed * sinPitch * cosYaw;
    piece.vel.y = -speed * cosPitch;
    piece.vel.z = speed * sinPitch * sinYaw;

    piece.rotVel.x = func_150ADA68() * 50.0f + -25.0f;
    piece.rotVel.y = 0.0f;
    piece.rotVel.z = func_150ADA68() * 50.0f + -25.0f;
    piece.lifetime = func_150ADA20() % 68U + 38;

    piece.gravity = func_150ADA68() * -0.1f + -0.3f;
    piece.radius = 1.0f;

    scale = func_150ADA68() * 18.80000114f + 5.6f;
    piece.model = 31;
    piece.variant = 0;
    piece.unk5C = 0;
    piece.sizeXZ = scale;
    piece.alpha = 0xFF;
    piece.updateFn = 1;
    piece.update2Fn = 0;
    piece.floorHitFn = 0;
    piece.floorType3Fn = 0;
    piece.wallHitFn = 0;
    piece.destroyFn = 0;
    piece.squashFn = 0;
    piece.matrixFn = 0;
    piece.unk6A = 0;
    piece.owner = 0;
    piece.ownerId = 0;
    piece.fadeStart = 1;
    piece.fadeRate = 0xFF;
    piece.sizeY = scale;

    func_15132A4C(&piece, 3, 0xFF, 0, arg14, 0);
    return 1;
}
