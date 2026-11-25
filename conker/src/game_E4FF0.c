#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* matching: the first 0x78 bytes of ParticleSpawn; a full ParticleSpawn on the stack changes the frame. */
typedef struct ParticleSpawn78 {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ Vec3f rot;
    /* 0x1C */ Vec3f scale;
    /* 0x28 */ Vec3f pos;
    /* 0x34 */ Vec3f vel;
    /* 0x40 */ Vec3f rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 variant;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s16 *unk5C;
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ s8 drawFn;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct Actor *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[2];
} ParticleSpawn78;

/* Burst config read by func_15151A38 (game_17CAF0.c's struct Local15151A38Arg). */
typedef struct {
    /* 0x00 */ Vec3f position;
    /* 0x0C */ f32 sizeBase;
    /* 0x10 */ f32 sizeRange;
    /* 0x14 */ f32 growthBase;
    /* 0x18 */ f32 growthRange;
    /* 0x1C */ f32 speedBase;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ s16 countBase;
    /* 0x26 */ s16 countRange;
    /* 0x28 */ s16 yawBase;
    /* 0x2A */ s16 yawRange;
    /* 0x2C */ s16 pitchBase;
    /* 0x2E */ s16 pitchRange;
    /* 0x30 */ s16 fadeBase;
    /* 0x32 */ s16 fadeRange;
    /* 0x34 */ s16 lifeBase;
    /* 0x36 */ s16 lifeRange;
    /* 0x38 */ u8 green;
    /* 0x39 */ u8 enabled;
    /* 0x3A */ u8 red;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ u8 blue;
    /* 0x55 */ u8 alphaBase;
    /* 0x56 */ u8 alphaRange;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60; /* 0x60..0x7D: params block for func_15147DA0 */
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s32 unk70;
    /* 0x74 */ s32 unk74;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ u8 unk7C;
    /* 0x7D */ u8 unk7D;
    /* 0x7E */ u8 pad7E[2];
    /* 0x80 */ s16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ s32 unk84;
} BurstConfig;

/* Ground probe result filled by func_1504715C / func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 pad4[0x24 - 0x4];
} Ground;

extern f32 D_8009FD30[3];
extern f32 D_8009FD3C[3];
extern f32 D_8009FD48[3];

// matching: the model is passed as s32; a pointer type moves the unk1D4 temp from v0 to a3
void func_15143134(f32 *in, f32 *out, void *mtx);
void func_1515C244(Actor *, f32 *, f32 *, f32 *);
void func_150B82D0(Vec3f *, f32 *, f32 *, f32 *, u8, s32);
void func_150B85C0(Vec3f *, f32 *, f32 *, u8, s32);
void func_1504715C(Ground *, Actor *);
s32 func_15046C80(Vec3f *, s32, f32, Ground *);
void func_1514C678(f32, f32, f32, f32, s32, s32, s32, s32, s32, f32, s32, u8);
s32 func_15132A4C(ParticleSpawn78 *, s32, s32, s32, u8, s32);
void func_15151A38(BurstConfig *, u8, s32);

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
    s32 pad5; // matching: unused stack slot; keeps the frame layout
    ParticleSpawn78 chunk;
    s32 pad2[3]; // matching: unused stack slots; keep the frame layout
    s8 i;
    f32 velX;
    f32 velZ;
    Vec3f center;
    f32 height;
    f32 radius;
    f32 aspect;

    if (obj != NULL) {
        if (obj->unk1D4 != NULL) {
            func_15142314((s32)obj->unk1D4, 4, &joints[0].x);
            func_15142314((s32)obj->unk1D4, 5, &joints[1].x);
            func_15142314((s32)obj->unk1D4, 0, &root.x);
        } else {
            root.x = obj->x_position;
            root.y = obj->y_position;
            root.z = obj->z_position;
            joints[0].x = root.x + 20.0f;
            joints[0].y = root.y + 130.0f;
            joints[0].z = root.z;
            joints[1].x = root.x - 20.0f;
            joints[1].y = root.y + 130.0f;
            joints[1].z = root.z;
        }
        dirX = func_151423D8(obj->unk76 >> 8);
        dirZ = func_151423D8((obj->unk76 >> 8) + 0x40);
        velX = obj->xz_velocity * dirX;
        velZ = obj->xz_velocity * dirZ;
        {
            f32 bottom[3];
            f32 side[3];

            if (obj->unk1D4 != NULL) {
                void *mtx;
                f32 dx;
                f32 dz;

                mtx = (u8 *)obj->unk1D4 + 0x80; /* a matrix inside the model (struct255 is opaque here) */
                func_15143134(D_8009FD30, &center.x, mtx);
                func_15143134(D_8009FD3C, bottom, mtx);
                func_15143134(D_8009FD48, side, mtx);
                height = fabsf(center.y - bottom[1]);
                dx = side[0] - bottom[0];
                dz = side[2] - bottom[2];
                radius = sqrtf(dx * dx + dz * dz);
            } else {
                func_1515C244(obj, &center.x, &radius, &height);
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
            Ground ground;
            Vec3f probe;

            probe.x = center.x;
            probe.y = center.y + 100.0f;
            probe.z = center.z;
            func_1504715C(&ground, obj);
            if (func_15046C80(&probe, 0, center.y - 2000.0f, &ground) != 0) {
                probe.y = ground.height;
                func_1514C678(probe.x, probe.y + 31.0f, probe.z, radius, 0, 0xFF,
                              (func_150ADA20() % 11U) + 30, 25, 0, 0, 0, arg1);
            }
        }
        chunk.model = 0;
        chunk.lifetime = 300;
        chunk.elasticity = 0.6f;
        chunk.rot.x = obj->unkB8;
        chunk.rot.y = obj->unk40;
        chunk.rot.z = obj->unkC4;
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
        scaleXZ = obj->xz_scale;
        if (scaleXZ > 1.0f) {
            scaleXZ = 1.0f;
        }
        if (scaleXZ < 0.5f) {
            scaleXZ = 0.5f;
        }
        scaleY = obj->y_scale;
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
            BurstConfig burst;

            speed = (func_150ADA68() * 30.0f * obj->y_scale) + 10.0f;
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
            scale = ((obj->xz_scale + obj->y_scale) * 0.5f) / 0.96f;
            burst.position = joints[i];
            burst.countBase = 4;
            burst.countRange = 4;
            burst.yawRange = 0xFF;
            burst.pitchBase = -0x3F;
            burst.pitchRange = 0x4B;
            burst.sizeBase = 3.0f * scale;
            burst.sizeRange = 3.0f * scale;
            burst.growthBase = 0.6f;
            burst.growthRange = 0.231f;
            burst.speedBase = 15.0f * scale;
            burst.speedRange = 10.0f * scale;
            burst.yawBase = 0;
            burst.fadeBase = 3;
            burst.fadeRange = 3;
            burst.lifeBase = 50;
            burst.lifeRange = 20;
            burst.green = 1;
            burst.enabled = 0x21;
            burst.red = 0x48;
            burst.unk3C = 1;
            burst.unk40 = 0;
            burst.unk44 = 0;
            burst.unk48 = 0;
            burst.unk4C = 0;
            burst.unk50 = 0;
            burst.blue = 0;
            burst.alphaBase = 0x9B;
            burst.alphaRange = 0x64;
            burst.unk58 = 3;
            burst.unk5C = 0xFF;
            burst.unk60 = 0;
            burst.unk64 = 0x220005;
            burst.unk68 = 0x1D0600;
            burst.unk6C = 1;
            burst.unk70 = 0x3B;
            burst.unk74 = 0x80;
            burst.unk78 = 0x20;
            burst.unk7C = 0;
            burst.unk7D = 7;
            burst.unk80 = 10;
            burst.unk82 = 25;
            burst.unk84 = obj->unk184;
            func_15151A38(&burst, arg1, arg2);
        }
    }
}

void func_15143794(s16, s16, f32, f32 *);
void *func_1518A3C0(f32 *, f32 *, f32, f32 *, f32 *, f32, f32, u8, s16, u8, s32, u8, s32);

/*
 * Spawns 35-60 tumbling pieces at random directions around `center`, at distance `radius`
 * with the vertical offset scaled by `yScale`, each flying outwards.
 */
void func_150B82D0(Vec3f *center, f32 *height, f32 *radius, f32 *yScale, u8 arg4, s32 arg5) {
    s32 count;
    f32 pos[3];
    f32 rot[3];
    f32 vel[3];
    f32 accel[3];
    s16 yaw;
    s16 pitch;

    for (count = (func_150ADA20() % 26U) + 35; count > 0; count--) {
        yaw = func_150ADA20() & 0xFF;
        pitch = (func_150ADA20() % 61U) - 63;
        func_15143794(yaw, pitch, *radius, pos);
        pos[1] *= *yScale;
        pos[0] += center->x;
        pos[1] += center->y;
        pos[2] += center->z;
        rot[0] = func_150ADA68() * 360.0f;
        rot[1] = func_150ADA68() * 360.0f;
        rot[2] = func_150ADA68() * 360.0f;
        func_15143794(yaw, pitch, ((func_150ADA68() * 1221.0f) + 1806.0f) * 0.01f, vel);
        // matching: random ranges are written `rand * range + min`; `- 1570.0f` would emit sub.s
        accel[0] = ((func_150ADA68() * 3140.0f) + -1570.0f) * 0.01f;
        accel[1] = 0.0f;
        accel[2] = ((func_150ADA68() * 3140.0f) + -1570.0f) * 0.01f;
        func_1518A3C0(pos, rot, ((func_150ADA68() * 104.0f) + 78.0f) * 0.001f, vel, accel,
                      ((func_150ADA68() * 633.0f) + -1064.0f) * 0.001f, 0.9425339699f, 1,
                      (func_150ADA20() % 51U) + 75, 0, 0, 0xFF, 1);
    }
}

/* Live particle returned by func_15130374; its extra data (dataSize bytes) starts at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ f32 extra;
} SpawnedParticle;

extern SpawnedParticle *func_15130374(ParticleDesc *spawn, u8 arg1, s32 dataSize, u8 arg3, s32 arg4);

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

extern void func_15143874(s16 angle, f32 speed, f32 *outX, f32 *outZ);
void *memcpy(void *, const void *, u32);

/* Spawns one particle at (x, y, z) moving along `angle`; its extra data is a float. */
s32 func_150B879C(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle, s32 arg9,
                  s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    f32 extra;
    SpawnedParticle *particle;
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
