#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"


void *func_15132A4C(ParticleSpawn *, s32, s32, s32, u8, s32);

/* Actor id 0x20 with a unk1D4 model: spawns one 0x29E9 particle near it with the actor as owner. */
s32 func_150BB760(struct127 *actor) {
    ParticleSpawn particle;
    f32 width;
    f32 height;
    f32 sine;
    f32 cosine;
    f32 offsetX;
    f32 offsetZ;

    if (actor->id != 0x20) {
        return 1;
    }
    if (actor != NULL && actor->interaction_state != 0 && actor->unk1D4 != NULL) {
        width = actor->unkE4;
        height = actor->unkE6;
        sine = func_151423D8(actor->unk76 >> 8);
        cosine = func_151423D8((actor->unk76 >> 8) + 0x40);
        offsetX = actor->xz_velocity * sine * 0.5f;
        offsetZ = actor->xz_velocity * cosine * 0.5f;
        particle.model = 1;
        particle.flags = 0x29E9;
        particle.variant = 0;
        particle.unk5C = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 1;
        particle.update2Fn = 0;
        particle.floorHitFn = 2;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 0;
        particle.unk6A = 1;
        particle.owner = actor;
        particle.scale.x = 1.0f;
        particle.scale.y = 1.0f;
        particle.scale.z = 1.0f;
        particle.rotVel.z = 0.0f;
        particle.gravity = -1.0f;
        particle.ownerId = actor->unique_id;
        particle.fadeStart = 0x20;
        particle.fadeRate = 7;
        {
            u8 angle;
            f32 spreadSine;
            f32 spreadCosine;
            f32 distance;
            f32 spreadX;
            f32 spreadZ;
            u8 heading;
            f32 headingSine;
            f32 headingCosine;
            f32 speed;
            f32 velocityX;
            f32 velocityZ;
            f32 size;
            f32 size2;

            angle = func_150ADA20();
            spreadSine = func_151423D8(angle);
            spreadCosine = func_151423D8(angle - 0x40);
            distance = func_150ADA68() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            heading = (actor->unk76 >> 8) - (func_150ADA20() & 0x3F) - 0x20;
            headingSine = func_151423D8(heading);
            headingCosine = func_151423D8(heading - 0x40);
            speed = func_150ADA68() * 5.0f + 3.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            size = func_150ADA68() * 0.05f + 0.01f;
            size2 = func_150ADA68() * 0.05f + 0.01f;
            particle.sizeXZ = size;
            particle.sizeY = size2;
            particle.radius = (size + size2) * 0.5f;
            particle.rot.x = func_150ADA68() * 360.0f;
            particle.rot.y = func_150ADA68() * 360.0f;
            particle.rot.z = func_150ADA68() * 360.0f;
            particle.pos.x = actor->x_position + spreadX;
            particle.pos.y = actor->y_position + func_150ADA68() * height + 4.0f;
            particle.pos.z = actor->z_position + spreadZ;
            particle.vel.x = velocityX + offsetX;
            particle.vel.y = func_150ADA68() * 5.0f + 3.0f;
            particle.vel.z = velocityZ + offsetZ;
            particle.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
            particle.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
            particle.lifetime = (func_150ADA20() & 0x1F) + 0x3C;
            particle.elasticity = func_150ADA68() * 0.2f + 0.25f;
            func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0);
        }
    }
}

typedef struct GameE8C10SpawnRequest {
    u8 pad0[0x80];
    s32 unk80;
    u8 pad84[0x4];
    s16 unk88;
    s16 unk8A;
    s16 unk8C;
    s16 unk8E;
    s16 unk90;
    s16 unk92;
    s16 unk94;
    s16 unk96;
    s16 unk98;
    u8 pad9A[0x6];
    s8 unkA0;
    s8 unkA1;
    s8 unkA2;
    u8 padA3[0x5];
} GameE8C10SpawnRequest;

void func_1516865C(GameE8C10SpawnRequest *request, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_15168800(GameE8C10SpawnRequest *request, s32 arg1, s32 arg2);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32,
                  s32, s32, s32, s32, s32, s32, s32, s32);

/* Actor id 0x20: func_1516865C/func_15168800 request, then bursts of 0x29E9 particles,
 * func_151DA6F8 effects and two 0x39E8 particles, then func_10010F88. */
s32 func_150BBB5C(struct127 *actor) {
    ParticleSpawn particle;
    s8 i;
    GameE8C10SpawnRequest request;
    f32 width;
    f32 height;
    s32 count1;
    s32 count2;
    f32 sine;
    f32 cosine;
    u8 alpha;
    f32 offsetX;
    f32 offsetZ;

    if (actor->id != 0x20) {
        return 1;
    }
    if (actor != NULL && actor->interaction_state != 0) {
        width = actor->unkE4;
        height = actor->unkE6;
        count1 = (func_150ADA20() & 3) + 6;
        count2 = (func_150ADA20() & 3) + 0xA;
        sine = func_151423D8(actor->unk76 >> 8);
        cosine = func_151423D8((actor->unk76 >> 8) + 0x40);
        offsetX = actor->xz_velocity * sine * 0.5f;
        offsetZ = actor->xz_velocity * cosine * 0.5f;
        // matching: the no-op mask is needed; without it IDO emits different code here
        alpha = (actor->unk1D4 != NULL) ? 0xFF - ((((u32)actor->unk184 >> 5) & 0x7FFFFFF) << 6) : 0xFF;
        request.unk8A = 0x80;
        request.unk88 = 0;
        request.unk8C = actor->x_position;
        request.unk8E = actor->unk180 + 60.0f;
        request.unk90 = actor->z_position;
        request.unk92 = width;
        request.unk94 = height;
        request.unk96 = actor->unk180;
        request.unkA2 = -1;
        request.unkA0 = 0x2E;
        request.unk98 = 0x34;
        request.unk80 = alpha;
        request.unkA1 = 0;
        func_1516865C(&request, 0xFF, 0xFF, 0xFF, 0xFF);
        func_15168800(&request, 0xFF, 0);
        particle.model = 1;
        particle.lifetime = 0x78;
        particle.flags = 0x29E9;
        particle.variant = 0;
        particle.unk5C = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 1;
        particle.update2Fn = 0;
        particle.floorHitFn = 2;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 0;
        particle.unk6A = 0;
        particle.owner = NULL;
        particle.ownerId = 0;
        particle.fadeStart = 0x20;
        particle.fadeRate = 7;
        particle.elasticity = 0.4f;
        particle.scale.x = 1.0f;
        particle.scale.y = 1.0f;
        particle.scale.z = 1.0f;
        particle.rotVel.z = 0.0f;
        particle.gravity = -1.5f;
        while (count1 > 0) {
            u8 angle;
            f32 spreadSine;
            f32 spreadCosine;
            f32 distance;
            f32 spreadX;
            f32 spreadZ;
            u8 heading;
            f32 headingSine;
            f32 headingCosine;
            f32 speed;
            f32 velocityX;
            f32 velocityZ;
            f32 size;
            f32 size2;

            angle = func_150ADA20();
            spreadSine = func_151423D8(angle);
            spreadCosine = func_151423D8(angle - 0x40);
            distance = func_150ADA68() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            heading = func_150ADA20();
            headingSine = func_151423D8(heading);
            headingCosine = func_151423D8(heading - 0x40);
            speed = func_150ADA68() * 4.0f + 2.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            size = func_150ADA68() * 0.03f + 0.03f;
            size2 = func_150ADA68() * 0.03f + 0.03f;
            particle.sizeXZ = size;
            particle.sizeY = size2;
            particle.radius = (size + size2) * 0.5f;
            particle.rot.x = func_150ADA68() * 360.0f;
            particle.rot.y = func_150ADA68() * 360.0f;
            particle.rot.z = func_150ADA68() * 360.0f;
            particle.pos.x = actor->x_position + spreadX;
            particle.pos.y = actor->y_position + func_150ADA68() * height + 5.0f;
            particle.pos.z = actor->z_position + spreadZ;
            particle.vel.x = offsetX + velocityX;
            particle.vel.y = func_150ADA68() * 7.0f + 13.0f;
            particle.vel.z = offsetZ + velocityZ;
            particle.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
            particle.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
            func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0);
            count1--;
        }
        while (count2 > 0) {
            f32 pos[3];
            f32 dir[3];
            f32 distance;
            f32 speed;
            s16 angle;
            f32 sine2;
            f32 cosine2;

            distance = func_150ADA68() * width;
            speed = func_150ADA68() * 7.0f + 7.0f;
            angle = func_150ADA20() & 0xFF;
            sine2 = func_151423D8(angle);
            cosine2 = func_151423D8(angle - 0x40);
            pos[0] = actor->x_position + sine2 * distance;
            pos[1] = actor->y_position + func_150ADA68() * height + 5.0f;
            pos[2] = actor->z_position - cosine2 * distance;
            dir[0] = cosine2 * speed + offsetX;
            dir[1] = func_150ADA68() * 9.0f + 15.0f;
            dir[2] = sine2 * speed + offsetZ;
            func_151DA6F8(pos, dir, 1.6f, (func_150ADA20() & 7) + 0x19, (func_150ADA20() & 0x7F) + 0x80,
                          func_150ADA68() * 0.2f * width + 13.0f, (func_150ADA20() & 3) + 2, 0,
                          1.0f, 1.0f, 1, 1, 0, 0x10, 0xF, 0, 0xFF, 0);
            count2--;
        }
        particle.model = 0;
        particle.lifetime = (func_150ADA20() & 0xF) + 0x10;
        particle.elasticity = 0.6f;
        particle.rot.x = actor->unkB8;
        particle.rot.y = actor->unk40;
        particle.rot.z = actor->unkC4;
        particle.flags = 0x39E8;
        particle.scale.x = 1.0f;
        particle.scale.y = 1.0f;
        particle.scale.z = 1.0f;
        particle.rotVel.z = 0.0f;
        particle.gravity = -1.6f;
        particle.pos.x = actor->x_position;
        particle.pos.y = actor->y_position;
        particle.pos.z = actor->z_position;
        particle.radius = actor->xz_scale * 20.0f;
        particle.sizeXZ = actor->xz_scale + actor->xz_scale;
        particle.variant = 0;
        particle.unk5C = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 1;
        particle.update2Fn = 0;
        particle.floorHitFn = 0;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 0;
        particle.unk6A = 0;
        particle.owner = NULL;
        particle.ownerId = 0;
        particle.fadeStart = 0x10;
        particle.fadeRate = 0xF;
        particle.sizeY = particle.sizeXZ;
        for (i = 0; i < 2; i++) {
            f32 speed;
            s16 yaw;
            s16 pitch;
            f32 yawSine;
            f32 yawCosine;
            f32 pitchSine;
            f32 pitchCosine;
            f32 horizontal;

            speed = actor->y_scale * (func_150ADA68() * 30.0f) + 10.0f;
            yaw = func_150ADA20() & 0x7F;
            pitch = -0x20 - (func_150ADA20() & 0x1F);
            yawSine = func_151423D8(yaw);
            yawCosine = func_151423D8(yaw - 0x40);
            pitchSine = func_151423D8(pitch);
            pitchCosine = func_151423D8(pitch - 0x40);
            horizontal = speed * pitchSine;
            particle.vel.x = horizontal * yawCosine + offsetX;
            particle.vel.y = -speed * pitchCosine;
            particle.vel.z = horizontal * yawSine + offsetZ;
            particle.rotVel.x = 30.0f - func_150ADA68() * 60.0f;
            particle.rotVel.y = 30.0f - func_150ADA68() * 60.0f;
            func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0);
        }
        func_10010F88(0xAA, 0x6590, 0, 0, 0, actor->x_position, actor->y_position,
                      actor->z_position, 0xC8, 0x7D0);
    }
}

/* Actor id 0x20: like func_150BBB5C but smaller, with no request; stops early if spawning fails. */
s32 func_150BC488(struct127 *actor) {
    f32 width;
    s32 count;
    s32 ok;
    s32 total;
    s32 n;
    ParticleSpawn particle;

    if (actor->id != 0x20) {
        return 1;
    }
    if (actor != NULL && actor->interaction_state != 0) {
        width = actor->unkE4;
        count = (func_150ADA20() & 3) + 6;
        particle.model = 1;
        particle.flags = 0x29E9;
        particle.variant = 0;
        particle.unk5C = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 1;
        particle.update2Fn = 0;
        particle.floorHitFn = 2;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 0;
        particle.unk6A = 0;
        particle.owner = NULL;
        particle.ownerId = 0;
        particle.fadeStart = 0x20;
        particle.fadeRate = 7;
        ok = 1;
        particle.scale.x = 1.0f;
        particle.scale.y = 1.0f;
        particle.scale.z = 1.0f;
        particle.elasticity = 0.4f;
        particle.rotVel.z = 0.0f;
        particle.gravity = -1.5f;
        while (count != 0 && ok != 0) {
            u8 angle;
            f32 spreadSine;
            f32 spreadCosine;
            f32 distance;
            f32 spreadX;
            f32 spreadZ;
            u8 heading;
            f32 headingSine;
            f32 headingCosine;
            f32 speed;
            f32 velocityX;
            f32 velocityZ;
            f32 velocityY;
            f32 sizeA;
            f32 sizeB;

            angle = func_150ADA20();
            spreadSine = func_151423D8(angle);
            spreadCosine = func_151423D8(angle - 0x40);
            distance = func_150ADA68() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            heading = func_150ADA20();
            headingSine = func_151423D8(heading);
            headingCosine = func_151423D8(heading - 0x40);
            speed = func_150ADA68() * 4.0f + 4.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            velocityY = actor->y_velocity * 0.05f;
            sizeA = func_150ADA68() * 0.04f + 0.02f;
            sizeB = func_150ADA68() * 0.04f + 0.02f;
            particle.sizeXZ = sizeA;
            particle.sizeY = sizeB;
            particle.radius = (sizeA + sizeB) * 0.5f;
            particle.rot.x = func_150ADA68() * 360.0f;
            particle.rot.y = func_150ADA68() * 360.0f;
            particle.rot.z = func_150ADA68() * 360.0f;
            particle.pos.x = actor->x_position + spreadX;
            particle.pos.y = actor->y_position;
            particle.pos.z = actor->z_position + spreadZ;
            particle.vel.x = velocityX;
            particle.vel.y = velocityY;
            particle.vel.z = velocityZ;
            particle.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
            particle.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
            particle.lifetime = (func_150ADA20() & 0x3F) + 0x64;
            if (func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0) == NULL) {
                ok = 0;
            }
            count--;
        }
        for (n = (func_150ADA20() & 3) + 2; n > 0; n--) {
            f32 position[3];
            f32 velocity[3];
            f32 speed;
            s16 angle;
            f32 sine;
            f32 cosine;

            speed = func_150ADA68() * 7.0f + 7.0f;
            angle = func_150ADA20() & 0xFF;
            sine = func_151423D8(angle);
            cosine = func_151423D8(angle - 0x40);
            position[0] = actor->x_position;
            position[1] = actor->y_position + func_150ADA68() * 5.0f;
            position[2] = actor->z_position;
            velocity[0] = cosine * speed;
            velocity[1] = 0.0f;
            velocity[2] = sine * speed;
            func_151DA6F8(position, velocity, 1.6f, (func_150ADA20() & 7) + 0x19,
                          (func_150ADA20() & 0x7F) + 0x80,
                          func_150ADA68() * 0.2f * width + 5.0f, (func_150ADA20() & 1) + 2, 0,
                          1.0f, 1.0f, 1, 1, 0, 0x10, 0xF, 0, 0xFF, 0);
        }
        total = (func_150ADA20() & 1) + (func_150ADA20() & 1);
        if (total != 0) {
            s8 j;

            particle.model = 0;
            particle.lifetime = (func_150ADA20() & 0xF) + 0x10;
            particle.elasticity = 0.6f;
            particle.rot.x = actor->unkB8;
            particle.rot.y = actor->unk40;
            particle.rot.z = actor->unkC4;
            particle.flags = 0x39E8;
            particle.scale.x = 1.0f;
            particle.scale.y = 1.0f;
            particle.scale.z = 1.0f;
            particle.rotVel.z = 0.0f;
            particle.gravity = -1.6f;
            particle.pos.x = actor->x_position;
            particle.pos.y = actor->y_position;
            particle.pos.z = actor->z_position;
            particle.radius = actor->xz_scale * 20.0f;
            particle.sizeXZ = actor->xz_scale * 2.0f;
            particle.variant = 0;
            particle.unk5C = 0;
            particle.alpha = 0xFF;
            particle.updateFn = 1;
            particle.update2Fn = 0;
            particle.sizeY = particle.sizeXZ;
            particle.floorHitFn = 0;
            particle.floorType3Fn = 0;
            particle.wallHitFn = 0;
            particle.destroyFn = 0;
            particle.squashFn = 0;
            particle.matrixFn = 0;
            particle.unk6A = 0;
            particle.owner = NULL;
            particle.ownerId = 0;
            particle.fadeStart = 0x10;
            particle.fadeRate = 0xF;
            for (j = 0; j < total; j++) {
                f32 speed;
                s16 heading;
                s16 pitch;
                f32 headingSine;
                f32 headingCosine;
                f32 pitchSine;
                f32 pitchCosine;
                f32 horizontal;

                speed = func_150ADA68() * 5.0f + 8.0f;
                heading = func_150ADA20() & 0x7F;
                // matching: the (u32) cast is needed
                pitch = 0x20 - (func_150ADA20() & 0x3F);
                headingSine = func_151423D8(heading);
                headingCosine = func_151423D8(heading - 0x40);
                pitchSine = func_151423D8(pitch);
                pitchCosine = func_151423D8(pitch - 0x40);
                horizontal = speed * pitchSine;
                particle.vel.x = horizontal * headingCosine;
                particle.vel.y = -speed * pitchCosine;
                particle.vel.z = horizontal * headingSine;
                particle.rotVel.x = 30.0f - func_150ADA68() * 60.0f;
                particle.rotVel.y = 30.0f - func_150ADA68() * 60.0f;
                func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0);
            }
        }
    }
}

/* Actor id 0x20: sometimes spawns one 0x29E9 particle moving forward or backward along unk76. */
s32 func_150BCBBC(struct127 *actor) {
    f32 width;
    f32 height;
    s32 count;
    s32 ok;
    ParticleSpawn particle;

    if (actor->id != 0x20) {
        return 1;
    }
    if (actor != NULL && actor->interaction_state != 0) {
        width = actor->unkE4;
        height = actor->unkE6;
        count = func_150ADA20() & 1;
        particle.model = 1;
        particle.lifetime = 0x96;
        particle.flags = 0x29E9;
        particle.variant = 0;
        particle.unk5C = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 1;
        particle.update2Fn = 0;
        particle.floorHitFn = 2;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 0;
        particle.unk6A = 0;
        particle.owner = NULL;
        particle.ownerId = 0;
        particle.fadeStart = 0x20;
        particle.fadeRate = 7;
        ok = 1;
        particle.scale.x = 1.0f;
        particle.scale.y = 1.0f;
        particle.scale.z = 1.0f;
        particle.elasticity = 0.3f;
        particle.rotVel.z = 0.0f;
        particle.gravity = -1.0f;
        while (count != 0 && ok != 0) {
            u8 angle;
            f32 spreadSine;
            f32 spreadCosine;
            f32 distance;
            f32 spreadX;
            f32 spreadZ;
            f32 sizeA;
            s32 heading;
            f32 headingSine;
            f32 headingCosine;
            f32 speed;
            f32 velocityX;
            f32 velocityZ;

            angle = func_150ADA20();
            spreadSine = func_151423D8(angle);
            spreadCosine = func_151423D8(angle - 0x40);
            distance = func_150ADA68() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            distance = func_150ADA68() * 0.03f + 0.01f;
            sizeA = func_150ADA68() * 0.03f + 0.01f;
            // matching: the unsigned offset and the casts are needed as written
            if (func_150ADA20() & 1) {
                heading = (u8)((actor->unk76 >> 8) + (s32)(8 - (func_150ADA20() & 0xF)));
            } else {
                heading = (u8)((actor->unk76 >> 8) + (s32)(8 - (func_150ADA20() & 0xF)) - 0x80);
            }
            headingSine = func_151423D8(heading);
            headingCosine = func_151423D8(heading - 0x40);
            speed = func_150ADA68() * 5.0f + 3.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            particle.sizeXZ = distance;
            particle.sizeY = sizeA;
            particle.radius = (distance + sizeA) * 0.5f;
            particle.rot.x = func_150ADA68() * 360.0f;
            particle.rot.y = func_150ADA68() * 360.0f;
            particle.rot.z = func_150ADA68() * 360.0f;
            particle.pos.x = actor->x_position + spreadX;
            particle.pos.y = actor->y_position + func_150ADA68() * height + 4.0f;
            particle.pos.z = actor->z_position + spreadZ;
            particle.vel.x = velocityX;
            particle.vel.y = func_150ADA68() * 5.0f + 3.0f;
            particle.vel.z = velocityZ;
            particle.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
            particle.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
            if (func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0) == NULL) {
                ok = 0;
            }
            count--;
        }
    }
}


struct Obj150BCFB8 {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 bounce;
    /* 0x18 */ u8 pad18[0x3C - 0x18];
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ u8 pad40[0x44 - 0x40];
    /* 0x44 */ vertex vel;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ u8 pad5C[0x60 - 0x5C];
    /* 0x60 */ s32 flags;
};

/* Ground-hit callback: comes to rest when falling slowly, otherwise bounces, scaling all motion by obj->bounce. */
s32 func_150BCFB8(struct Obj150BCFB8 *obj, s32 arg1, s32 arg2, s32 arg3, f32 floorY) {
    obj->unk3C = obj->unk10 * 1.1f + floorY;
    if (-2.0f < obj->vel.y) {
        obj->vel.x = 0.0f;
        obj->flags &= ~0x6F;
        obj->vel.y = 0.0f;
        obj->vel.z = 0.0f;
    } else {
        obj->vel.x *= obj->bounce;
        obj->vel.y = -obj->vel.y * obj->bounce;
        obj->vel.z *= obj->bounce;
        obj->unk50 *= obj->bounce;
        obj->unk54 *= obj->bounce;
        obj->unk58 *= obj->bounce;
    }
    return 1;
}

/* Live particle created by func_15130280. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ u8 data[4]; /* per-particle handler state */
} Particle;

void func_1514470C(struct178 *, vertex *);
void func_15143794(s16, s16, f32, vertex *);
s32 func_1513264C(ParticleSpawn *, s32, s32, s32, s32, u8, s32);
Particle *func_15130280(ParticleDesc *, u8, s32, s32, u8, s32);
void func_151D5334(vertex *, f32, f32, f32, s32, u8, s32);

/* Burst of particles and spawned particles from two random anchors (entries 0x4C/0x4D of D_800D3098). */
void func_150BD070(u8 arg0, s32 arg1) {
    struct178 *anchors[2];
    const u8 idx[2] = {0x4C, 0x4D};
    s16 count;

    // D_800D3098 holds a pointer to the struct178 array (variables.h types it as the array itself)
    anchors[0] = *(struct178 **)D_800D3098 + idx[0];
    anchors[1] = *(struct178 **)D_800D3098 + idx[1];
    {
        ParticleSpawn particle;
        s32 rnd;

        count = func_150ADA20() % 9U + 13;
        particle.flags = 0x12DE8;
        particle.model = 1;
        particle.variant = 0;
        particle.unk5C = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 8;
        particle.update2Fn = 0;
        particle.floorHitFn = 0;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 2;
        particle.drawFn = -1;
        particle.unk6A = 2;
        particle.owner = NULL;
        particle.ownerId = 0;
        particle.fadeStart = 0xC;
        particle.fadeRate = 0x15;
        particle.sound = 0;
        particle.radius = 1.0f;
        particle.elasticity = 1.0f;
        particle.scale.x = 1.0f;
        particle.scale.y = 1.0f;
        particle.scale.z = 1.0f;
        particle.rotVel.y = 0.0f;
        while (count > 0) {
            particle.lifetime = func_150ADA20() % 26U + 40;
            particle.sizeXZ = particle.sizeY = func_150ADA68() * 0.2470000088f + 0.2030000091f;
            particle.rot.x = func_150ADA68() * 360.0f;
            particle.rot.y = func_150ADA68() * 360.0f;
            particle.rot.z = func_150ADA68() * 360.0f;
            // matching: the "+ -c" addends here must stay; writing "- c" changes the emitted code
            particle.rotVel.x = func_150ADA68() * 45.46600342f + -22.73300171f;
            particle.rotVel.z = func_150ADA68() * 45.46600342f + -22.73300171f;
            func_1514470C(anchors[func_150ADA20() & 1], &particle.pos);
            rnd = func_150ADA20();
            func_15143794((u32)rnd % 31U - 84, func_150ADA20() % 26U - 45, func_150ADA68() * 21.80000114f + 30.2f, &particle.vel);
            particle.gravity = func_150ADA68() * 1.129000068f + -1.909f;
            func_1513264C(&particle, 3, 0xFF, 0, 0, arg0, arg1);
            count--;
        }
    }
    {
        ParticleDesc spawn;
        f32 scale;
        s32 rnd;
        vertex pos;
        vertex pos2;

        count = func_150ADA20() % 9U + 17;
        spawn.unk0 = 0x200005;
        spawn.renderMode = 0x5103;
        spawn.unk4 = 0;
        spawn.frame = 0;
        spawn.frameRate = 0;
        spawn.r = 0xD4;
        spawn.g = 0xAA;
        spawn.b = 8;
        spawn.a = 0xFF;
        spawn.r2 = 0x15;
        spawn.g2 = 4;
        spawn.b2 = 0;
        spawn.brightness = 0xFF;
        spawn.texture = 0x6C;
        spawn.fadeStart = 0x23;
        spawn.fadeRate = 7;
        spawn.scaleRate = 1.012537f;
        spawn.offset = *(vertex *)&D_800A5480;
        rnd = (func_150ADA20() & 1) ? 0x40 : 0;
        spawn.flags = ((func_150ADA20() & 1) ? 0x80 : 0) | 7 | rnd | 0x8000 | 0x4000 | 0x800 | 0x400 | 0x200 | 0x20000 | 0x800000;
        spawn.viewMask = 0;
        spawn.primColorMode = 8;
        spawn.envColorMode = 6;
        spawn.updateFunc = 0x10;
        spawn.deathFunc = -1;
        spawn.drawFunc = -1;
        spawn.handlerSet = 0;
        spawn.views = 0xFF;
        spawn.minSize = 0;
        spawn.maxSize = 0.0f;
        scale = 0.917015f;
        while (count > 0) {
            Particle *spawned;

            spawn.scaleStart = spawn.timer = func_150ADA20() % 31U + 30;
            spawn.alpha = func_150ADA20() % 101U + 155;
            spawn.width = spawn.height = func_150ADA68() * 755.0f + 1262.0f;
            func_1514470C(anchors[func_150ADA20() & 1], &spawn.pos);
            rnd = func_150ADA20();
            func_15143794((u32)rnd % 41U - 89, func_150ADA20() % 51U - 64, func_150ADA68() * 45.60000229f + 18.80000114f, &spawn.vel);
            spawn.gravity = func_150ADA68() * 0.559f + -0.777f;
            spawned = func_15130280(&spawn, 1, 0, 4, arg0, arg1);
            if (spawned != NULL) {
                memcpy(spawned->data, &scale, sizeof(scale));
            }
            count--;
        }
        pos.x = anchors[0]->unk0;
        pos.y = anchors[0]->pad2;
        pos.z = anchors[0]->unk4;
        func_151D5334(&pos, 0.0f, 1000.0f, 0.001f, 5, arg0, arg1);
        pos2.x = anchors[0]->unk0;
        pos2.y = anchors[0]->pad2;
        pos2.z = anchors[0]->unk4;
        func_151D5404((struct17 *)&pos2, 506.0f, 1013.0f, 0.0009871669f, 0xF, 0x14, arg0, arg1);
    }
}

extern vertex D_8009FF40[4];
extern vertex D_8009FF70[4];
extern vertex D_8009FFA0[4];
void func_15143134(vertex *src, vertex *dst, struct255 *frame);
void func_1514C470(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 arg6, s32 arg7,
                   s32 arg8, f32 arg9, vertex *dir, u8 arg11);

/* Transforms three 4-point tables by the actor's unk1D4 and emits two segments (point 2->3 and 3->0). */
void func_150BD740(struct127 *actor, u8 arg1, s32 arg2) {
    vertex points[4];
    vertex dirFrom[4];
    vertex dirTo[4];
    s32 i;
    vertex dir[4];

    if (actor != NULL) {
        if (actor->unk1D4 != NULL) {
            for (i = 0; i < 4; i++) {
                func_15143134(&D_8009FF40[i], &points[i], actor->unk1D4);
                func_15143134(&D_8009FF70[i], &dirFrom[i], actor->unk1D4);
                func_15143134(&D_8009FFA0[i], &dirTo[i], actor->unk1D4);
            }
            dir[2].x = dirTo[2].x - dirFrom[2].x;
            dir[2].y = dirTo[2].y - dirFrom[2].y;
            dir[2].z = dirTo[2].z - dirFrom[2].z;
            func_1514C470(points[2].x, points[2].y, points[2].z, points[3].x, points[3].y, points[3].z,
                          func_150ADA68() * 6.0f + 14.0f, 13, 0, 0.0f, &dir[2], arg1);
            dir[3].x = dirTo[3].x - dirFrom[3].x;
            dir[3].y = dirTo[3].y - dirFrom[3].y;
            dir[3].z = dirTo[3].z - dirFrom[3].z;
            func_1514C470(points[3].x, points[3].y, points[3].z, points[0].x, points[0].y, points[0].z,
                          func_150ADA68() * 6.0f + 14.0f, 13, 0, 0.0f, &dir[3], arg1);
        }
    }
}

/* Trail spawn structs; same layout as the ones in game_175250.c. */
typedef struct {
    /* 0x00 */ vertex pos;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 pad16[0xA];
} TrailSpawn;

typedef struct {
    /* 0x00 */ f32 halfWidth;
    /* 0x04 */ vertex velocity;
    /* 0x10 */ f32 gravity;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 flags;
    /* 0x19 */ u8 texture;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 pad1C[4];
} TrailMotion; /* size 0x20 */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 renderMode;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
} TrailRender; /* size 0x20 */

s32 func_15147DA0(TrailSpawn *spawn, TrailMotion *motion, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, TrailRender *render,
                  s32 arg12, u8 arg13, s32 arg14);

/* Spawns one trail at (x, y, z) moving along dir. */
s32 func_150BD954(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, f32 *dir, u8 arg14) {
    TrailSpawn spawn;
    TrailMotion motion;
    f32 speed;
    TrailRender render;

    render.unk0 = 0;
    render.unk4 = 1;
    render.unk8 = 0x160600;
    render.renderMode = 3;
    render.unk10 = 0x10;
    render.unk14 = 0x80;
    render.unk18 = 0x20;
    render.unk1C = 0;
    render.unk1D = 9;
    spawn.unk10 = 1;
    spawn.unkE = 1;
    spawn.pos.x = x;
    spawn.pos.y = y;
    motion.unk14 = y;
    motion.texture = 9;
    motion.unk1A = 0xFF;
    motion.flags = 8;
    spawn.pos.z = z;

    speed = (func_150ADA68() * 127.0f + 85.0f) * 0.001f;
    motion.velocity.x = dir[0] * speed;
    motion.velocity.y = dir[1] * speed;
    motion.velocity.z = dir[2] * speed;
    motion.unk1B = func_150ADA20() % 86U + 180;
    spawn.unk15 = (func_150ADA20() & 3) + 3;
    spawn.unkC = func_150ADA20() % 31U + 39;
    motion.halfWidth = (func_150ADA68() * 602.0f + 407.0f) * 0.01f;
    motion.gravity = (func_150ADA68() * 356.0f + 1017.0f) * 0.001f;
    func_15147DA0(&spawn, &motion, 0, 1, 11, 0, 0, 0, 0, 0, 0, &render, 0, arg14, 1);
    return 1;
}

struct Sub150BDB3C {
    /* 0x00 */ u8 pad0[0x1B];
    /* 0x1B */ u8 unk1B;
};

struct Obj150BDB3C {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u8 pad1E[0x98 - 0x1E];
    /* 0x98 */ struct Sub150BDB3C *unk98;
};

/* Caps obj->unk98->unk1B at obj->unk1C * 8. */
s32 func_150BDB3C(struct Obj150BDB3C *obj) {
    struct Sub150BDB3C *sub = obj->unk98;
    s32 limit = obj->unk1C * 8;

    if (limit < sub->unk1B) {
        sub->unk1B = limit;
    }
    return 1;
}
