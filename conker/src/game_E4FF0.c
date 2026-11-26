#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Bounds points in the model's root space: centre, base and side (func_150B7B40). */
const Vec3f D_8009FD30 = { 0.0f, 0.0f, 0.0f };
const Vec3f D_8009FD3C = { 0.0f, 83.0f, 0.0f };
const Vec3f D_8009FD48 = { 136.0f, -88.0f, 0.0f };


/*
 * Spawns break-up effects for `obj`: debris and a particle around its bounds (func_150B82D0,
 * func_150B85C0), an effect on the ground below it, and two chunks, each with a burst, flung
 * from joints 4 and 5 of its model (or from fixed points above it when it has no model).
 */
void func_150B7B40(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f joints[2];
    Vec3f root;
    f32 scaleXZ;
    f32 scaleY;
    f32 scale;
    f32 dirX;
    f32 dirZ;
    ParticleSpawn chunk;
    s32 pad2[3]; // matching: unused stack slots; keep the frame layout
    s8 i;
    f32 velX;
    f32 velZ;
    Vec3f center;
    f32 height;
    f32 radius;
    f32 aspect;

    if (obj != NULL) {
        if (obj->ob_matrix_ptr != NULL) {
            // matching: integer casts keep ob_matrix_ptr out of a0 (a pointer argument is loaded straight into a0)
            func_15142314((s32)obj->ob_matrix_ptr, 4, &joints[0].x);
            func_15142314((s32)obj->ob_matrix_ptr, 5, &joints[1].x);
            func_15142314((s32)obj->ob_matrix_ptr, 0, &root.x);
        } else {
            root.x = obj->xpos;
            root.y = obj->ypos;
            root.z = obj->zpos;
            joints[0].x = root.x + 20.0f;
            joints[0].y = root.y + 130.0f;
            joints[0].z = root.z;
            joints[1].x = root.x - 20.0f;
            joints[1].y = root.y + 130.0f;
            joints[1].z = root.z;
        }
        dirX = func_151423D8(obj->angle >> 8);
        dirZ = func_151423D8((obj->angle >> 8) + 0x40);
        velX = obj->speed * dirX;
        velZ = obj->speed * dirZ;
        {
            Vec3f bottom;
            Vec3f side;

            if (obj->ob_matrix_ptr != NULL) {
                MtxF *mtx;
                f32 dx;
                f32 dz;

                mtx = &obj->ob_matrix_ptr[2];
                func_15143134(&D_8009FD30, &center, mtx);
                func_15143134(&D_8009FD3C, &bottom, mtx);
                func_15143134(&D_8009FD48, &side, mtx);
                height = fabsf(center.y - bottom.y);
                dx = side.x - bottom.x;
                dz = side.z - bottom.z;
                radius = sqrtf(dx * dx + dz * dz);
            } else {
                sprite_getActorBodyBounds(obj, &center, &radius, &height);
            }
        }
        if (radius != 0.0f) {
            aspect = height / radius;
        } else {
            aspect = 1.0f;
        }
        func_150B82D0(&center, &height, &radius, &aspect, arg1, arg2);
        func_150B85C0(&center, &height, &radius, arg1, arg2);
        {
            CollisionHit ground;
            Vec3f probe;

            probe.x = center.x;
            probe.y = center.y + 100.0f;
            probe.z = center.z;
            collision_initHitFromObject(&ground, obj);
            if (collision_probeFloorSurfABSet0Cached(&probe, 0, center.y - 2000.0f, &ground) != 0) {
                probe.y = ground.height;
                func_1514C678(probe.x, probe.y + 31.0f, probe.z, radius, 0, 0xFF,
                              (func_150ADA20() % 11U) + 30, 25, 0, 0, 0, arg1);
            }
        }
        chunk.model = 0;
        chunk.lifetime = 300;
        chunk.elasticity = 0.6f;
        chunk.rot.x = obj->vtilt;
        chunk.rot.y = obj->theta;
        chunk.rot.z = obj->lean_angle;
        chunk.scale.x = 1.0f;
        chunk.scale.y = 1.0f;
        chunk.scale.z = 1.0f;
        chunk.flags = 0x197D;
        chunk.variant = 0;
        chunk.unk5C = NULL;
        chunk.alpha = 0xFF;
        chunk.updateFn = 1;
        chunk.update2Fn = 0;
        chunk.floorHitFn = 1;
        chunk.floorType3Fn = 0;
        chunk.wallHitFn = 1;
        chunk.destroyFn = 0;
        chunk.squashFn = 0;
        chunk.matrixFn = 0;
        chunk.unk6A = 0;
        chunk.owner = NULL;
        chunk.ownerId = 0;
        chunk.fadeStart = 1;
        chunk.fadeRate = 0xFF;
        chunk.rotVel.z = 0; // matching: an int 0 here; 0.0f makes IDO keep a shared zero in $f22
        chunk.gravity = -1.6f;
        scaleXZ = obj->scale_xz;
        if (scaleXZ > 1.0f) {
            scaleXZ = 1.0f;
        }
        if (scaleXZ < 0.5f) {
            scaleXZ = 0.5f;
        }
        scaleY = obj->scale_y;
        if (scaleY > 1.0f) {
            scaleY = 1.0f;
        }
        if (scaleY < 0.5f) {
            scaleY = 0.5f;
        }
        chunk.radius = ((28.0f * scaleY) + (28.0f * scaleXZ)) * 0.5f;
        chunk.sizeXZ = 3.0f * scaleXZ;
        chunk.sizeY = 3.0f * scaleY;
        for (i = 0; i < 2; i++) {
            f32 speed;
            s16 yaw;
            s16 pitch;
            f32 sinYaw;
            f32 cosYaw;
            f32 sinPitch;
            f32 cosPitch;
            f32 horizontal;
            PuffSpray burst;

            speed = (func_150ADA68() * 30.0f * obj->scale_y) + 10.0f;
            yaw = func_150ADA20() & 0x7F;
            pitch = -0x20 - (func_150ADA20() & 0x1F);
            sinYaw = func_151423D8(yaw);
            cosYaw = func_151423D8(yaw - 0x40);
            sinPitch = func_151423D8(pitch);
            cosPitch = func_151423D8(pitch - 0x40);
            horizontal = speed * sinPitch;
            chunk.pos = joints[i];
            chunk.vel.x = (horizontal * cosYaw) + velX;
            chunk.vel.y = -speed * cosPitch;
            chunk.vel.z = (horizontal * sinYaw) + velZ;
            chunk.rotVel.x = 30.0f - (func_150ADA68() * 60.0f);
            chunk.rotVel.y = 30.0f - (func_150ADA68() * 60.0f);
            func_15132A4C(&chunk, 3, 2, 0, arg1, 0);
            scale = ((obj->scale_xz + obj->scale_y) * 0.5f) / 0.96f;
            burst.pos = joints[i];
            burst.countBase = 4;
            burst.countRange = 4;
            burst.angles.yawRange = 0xFF;
            burst.angles.pitchBase = -0x3F;
            burst.angles.pitchRange = 0x4B;
            burst.sizeBase = 3.0f * scale;
            burst.sizeRange = 3.0f * scale;
            burst.growthBase = 0.6f;
            burst.growthRange = 0.231f;
            burst.speedBase = 15.0f * scale;
            burst.speedRange = 10.0f * scale;
            burst.angles.yawBase = 0;
            burst.itemCountBase = 3;
            burst.itemCountRange = 3;
            burst.lifeBase = 50;
            burst.lifeRange = 20;
            burst.green = 1;
            burst.headerFlags = 0x21;
            burst.red = 0x48;
            burst.handlers[0] = 1;
            burst.handlers[1] = 0;
            burst.handlers[2] = 0;
            burst.handlers[3] = 0;
            burst.handlers[4] = 0;
            burst.handlers[5] = 0;
            burst.blue = 0;
            burst.alphaBase = 0x9B;
            burst.alphaRange = 0x64;
            burst.unk58 = 3;
            burst.unk5C = 0xFF;
            burst.render.unk0 = 0;
            burst.render.setGeometryMode = 0x220005;
            burst.render.clearGeometryMode = 0x1D0600;
            burst.render.renderMode = 1;
            burst.render.unk10 = 0x3B;
            burst.render.unk14 = 0x80;
            burst.render.unk18 = 0x20;
            burst.render.primMode = 0;
            burst.render.envMode = 7;
            burst.unk80 = 10;
            burst.unk82 = 25;
            burst.unk84 = obj->floorMaterial;
            func_15151A38(&burst, arg1, arg2);
        }
    }
}

/*
 * Spawns 35-60 tumbling pieces at random directions around `center`, at distance `radius`
 * with the vertical offset scaled by `yScale`, each flying outwards.
 */
void func_150B82D0(Vec3f *center, f32 *height, f32 *radius, f32 *yScale, u8 arg4, s32 arg5) {
    s32 count;
    Vec3f pos;
    Vec3f rot;
    Vec3f vel;
    Vec3f accel;
    s16 yaw;
    s16 pitch;

    for (count = (func_150ADA20() % 26U) + 35; count > 0; count--) {
        yaw = func_150ADA20() & 0xFF;
        pitch = (func_150ADA20() % 61U) - 63;
        func_15143794(yaw, pitch, *radius, &pos);
        pos.y *= *yScale;
        pos.x += center->x;
        pos.y += center->y;
        pos.z += center->z;
        rot.x = func_150ADA68() * 360.0f;
        rot.y = func_150ADA68() * 360.0f;
        rot.z = func_150ADA68() * 360.0f;
        func_15143794(yaw, pitch, ((func_150ADA68() * 1221.0f) + 1806.0f) * 0.01f, &vel);
        // matching: random ranges are written `rand * range + min`; `- 1570.0f` would emit sub.s
        accel.x = ((func_150ADA68() * 3140.0f) + -1570.0f) * 0.01f;
        accel.y = 0.0f;
        accel.z = ((func_150ADA68() * 3140.0f) + -1570.0f) * 0.01f;
        effect_spawnTumblingPiece(&pos, &rot, ((func_150ADA68() * 104.0f) + 78.0f) * 0.001f, &vel, &accel,
                      ((func_150ADA68() * 633.0f) + -1064.0f) * 0.001f, 0.9425339699f, 1,
                      (func_150ADA20() % 51U) + 75, 0, 0, 0xFF, 1);
    }
}

/* Live particle returned by func_15130374; its extra data (dataSize bytes) starts at 0xA8. */
typedef struct {
    /* 0x00 */ DescParticleHead head;
    /* 0xA8 */ f32 extra;
} FloatDataParticle;

/* Spawns one particle at `center`, sized from `radius` and `height`. */
void func_150B85C0(Vec3f *center, f32 *height, f32 *radius, u8 arg3, s32 arg4) {
    ParticleDesc spawn;

    spawn.texture = 0x29;
    spawn.renderMode = 0xE03;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.timer = (func_150ADA20() % 11U) + 35;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r2 = 0xB0;
    spawn.g2 = 0xA0;
    spawn.b2 = 0x2A;
    spawn.r = 0x40;
    spawn.g = 0x0B;
    spawn.b = 0x6A;
    spawn.a = 0xFF;
    spawn.alpha = (func_150ADA20() % 51U) + 100;
    spawn.brightness = 0xFF;
    spawn.primColorMode = 3;
    spawn.envColorMode = 3;
    spawn.width = *radius * 20.0f;
    spawn.height = *height * 20.0f;
    spawn.pos = *center;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    spawn.vel.x = 0.0f;
    spawn.vel.y = 0.0f;
    spawn.vel.z = 0.0f;
    spawn.gravity = 0.0f;
    spawn.flags = (func_150ADA20() & 1 ? 0x40 : 0) | ((func_150ADA20() & 1 ? 0x80 : 0) | 0xE01) | 0x4000 | 0x8000;
    spawn.updateFunc = -1;
    spawn.deathFunc = -1;
    spawn.fadeStart = 25;
    spawn.fadeRate = 10;
    spawn.scaleStart = 40;
    spawn.drawFunc = -1;
    spawn.handlerSet = 0;
    spawn.scaleRate = 1.009518027f;
    func_15130374(&spawn, 1, 0, arg3, arg4);
}

void *memcpy(void *, const void *, u32);

/* Spawns one particle at (x, y, z) moving along `angle`; its extra data is a float. */
s32 func_150B879C(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle, s32 arg9,
                  s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    f32 extra;
    FloatDataParticle *particle;
    ParticleDesc spawn;

    spawn.texture = 0x29;
    spawn.renderMode = 0xE03;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    extra = 0.9172409773f;
    spawn.timer = (func_150ADA20() & 0xF) + 25;
    spawn.frame = 0;
    spawn.frameRate = 0;
    spawn.r2 = 0xB0;
    spawn.g2 = 0xA0;
    spawn.b2 = 0x2A;
    spawn.r = 0x40;
    spawn.g = 0x0B;
    spawn.b = 0x6A;
    spawn.a = 0xFF;
    spawn.alpha = (func_150ADA20() % 71U) + 60;
    spawn.brightness = 0xFF;
    spawn.primColorMode = 3;
    spawn.envColorMode = 3;
    spawn.width = spawn.height = ((func_150ADA68() * 15738.0f) + 24699.0f) * 0.01f;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;
    spawn.offset.x = 0.0f;
    spawn.offset.y = 0.0f;
    spawn.offset.z = 0.0f;
    func_15143874(angle, ((func_150ADA68() * 154.0f) + 205.0f) * 0.1f, &spawn.vel.x, &spawn.vel.z);
    spawn.vel.y = 0.0f;
    spawn.gravity = 0.0f;
    spawn.flags = (func_150ADA20() & 1 ? 0x40 : 0) | ((func_150ADA20() & 1 ? 0x80 : 0) | 0xE05) | 0x4000 | 0x8000;
    spawn.updateFunc = 10;
    spawn.deathFunc = -1;
    spawn.fadeStart = 28;
    spawn.fadeRate = 9;
    spawn.scaleStart = 25;
    spawn.drawFunc = -1;
    spawn.handlerSet = 0;
    spawn.scaleRate = 1.033332944f;
    particle = func_15130374(&spawn, 1, sizeof(extra), argE, 1);
    if (particle != NULL) {
        memcpy(&particle->extra, &extra, sizeof(extra));
    }
    return 1;
}
