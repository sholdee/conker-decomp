#include <ultra64.h>
#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/sound.h"
#include "conker/zone.h"


/* Actor id 0x20 with a ob_matrix_ptr model: spawns one 0x29E9 particle near it with the actor as owner. */
s32 func_150BB760(Actor *actor) {
    ParticleSpawn particle;
    f32 width;
    f32 height;
    f32 sine;
    f32 cosine;
    f32 offsetX;
    f32 offsetZ;

    if (actor->character_type != 0x20) {
        return 1;
    }
    if (actor != NULL && actor->type != 0 && actor->ob_matrix_ptr != NULL) {
        width = actor->bodyRadius;
        height = actor->bodyHeight;
        sine = math_cosBinAngleMasked(actor->angle >> 8);
        cosine = math_cosBinAngleMasked((actor->angle >> 8) + 0x40);
        offsetX = actor->speed * sine * 0.5f;
        offsetZ = actor->speed * cosine * 0.5f;
        particle.model = 1;
        particle.flags = 0x29E9;
        particle.variant = 0;
        particle.zone = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 1;
        particle.update2Fn = 0;
        particle.floorHitFn = 2;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 0;
        particle.lightMode = 1;
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

            angle = math_random();
            spreadSine = math_cosBinAngleMasked(angle);
            spreadCosine = math_cosBinAngleMasked(angle - 0x40);
            distance = math_randomFloat() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            heading = (actor->angle >> 8) - (math_random() & 0x3F) - 0x20;
            headingSine = math_cosBinAngleMasked(heading);
            headingCosine = math_cosBinAngleMasked(heading - 0x40);
            speed = math_randomFloat() * 5.0f + 3.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            size = math_randomFloat() * 0.05f + 0.01f;
            size2 = math_randomFloat() * 0.05f + 0.01f;
            particle.sizeXZ = size;
            particle.sizeY = size2;
            particle.radius = (size + size2) * 0.5f;
            particle.rot.x = math_randomFloat() * 360.0f;
            particle.rot.y = math_randomFloat() * 360.0f;
            particle.rot.z = math_randomFloat() * 360.0f;
            particle.pos.x = actor->xpos + spreadX;
            particle.pos.y = actor->ypos + math_randomFloat() * height + 4.0f;
            particle.pos.z = actor->zpos + spreadZ;
            particle.vel.x = velocityX + offsetX;
            particle.vel.y = math_randomFloat() * 5.0f + 3.0f;
            particle.vel.z = velocityZ + offsetZ;
            particle.rotVel.x = 25.0f - math_randomFloat() * 50.0f;
            particle.rotVel.y = 25.0f - math_randomFloat() * 50.0f;
            particle.lifetime = (math_random() & 0x1F) + 0x3C;
            particle.elasticity = math_randomFloat() * 0.2f + 0.25f;
            particle_newModelParticle(&particle, 3, 0xFF, 0, 0xFF, 0);
        }
    }
}

/* Actor id 0x20: effect_initQuadFxTemplate/effect_spawnQuadFx request, then bursts of 0x29E9 particles,
 * particle_spawnSplash effects and two 0x39E8 particles, then sound_playAt. */
s32 func_150BBB5C(Actor *actor) {
    ParticleSpawn particle;
    s8 i;
    QuadFxData request;
    f32 width;
    f32 height;
    s32 count1;
    s32 count2;
    f32 sine;
    f32 cosine;
    u8 alpha;
    f32 offsetX;
    f32 offsetZ;

    if (actor->character_type != 0x20) {
        return 1;
    }
    if (actor != NULL && actor->type != 0) {
        width = actor->bodyRadius;
        height = actor->bodyHeight;
        count1 = (math_random() & 3) + 6;
        count2 = (math_random() & 3) + 0xA;
        sine = math_cosBinAngleMasked(actor->angle >> 8);
        cosine = math_cosBinAngleMasked((actor->angle >> 8) + 0x40);
        offsetX = actor->speed * sine * 0.5f;
        offsetZ = actor->speed * cosine * 0.5f;
        // matching: the no-op mask is needed; without it IDO emits different code here
        alpha = (actor->ob_matrix_ptr != NULL) ? 0xFF - (((actor->floorMaterial >> 5) & 0x7FFFFFF) << 6) : 0xFF;
        request.frameStep = 0x80;
        request.frame = 0;
        request.x = actor->xpos;
        request.y = actor->floorY + 60.0f;
        request.z = actor->zpos;
        request.unk92 = width;
        request.unk94 = height;
        request.floorY = actor->floorY;
        request.callback = -1;
        request.animIndex = 0x2E;
        request.flags = 0x34;
        request.dropFlags = alpha;
        request.speed = 0;
        effect_initQuadFxTemplate(&request, 0xFF, 0xFF, 0xFF, 0xFF);
        effect_spawnQuadFx(&request, 0xFF, 0);
        particle.model = 1;
        particle.lifetime = 0x78;
        particle.flags = 0x29E9;
        particle.variant = 0;
        particle.zone = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 1;
        particle.update2Fn = 0;
        particle.floorHitFn = 2;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 0;
        particle.lightMode = 0;
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

            angle = math_random();
            spreadSine = math_cosBinAngleMasked(angle);
            spreadCosine = math_cosBinAngleMasked(angle - 0x40);
            distance = math_randomFloat() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            heading = math_random();
            headingSine = math_cosBinAngleMasked(heading);
            headingCosine = math_cosBinAngleMasked(heading - 0x40);
            speed = math_randomFloat() * 4.0f + 2.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            size = math_randomFloat() * 0.03f + 0.03f;
            size2 = math_randomFloat() * 0.03f + 0.03f;
            particle.sizeXZ = size;
            particle.sizeY = size2;
            particle.radius = (size + size2) * 0.5f;
            particle.rot.x = math_randomFloat() * 360.0f;
            particle.rot.y = math_randomFloat() * 360.0f;
            particle.rot.z = math_randomFloat() * 360.0f;
            particle.pos.x = actor->xpos + spreadX;
            particle.pos.y = actor->ypos + math_randomFloat() * height + 5.0f;
            particle.pos.z = actor->zpos + spreadZ;
            particle.vel.x = offsetX + velocityX;
            particle.vel.y = math_randomFloat() * 7.0f + 13.0f;
            particle.vel.z = offsetZ + velocityZ;
            particle.rotVel.x = 25.0f - math_randomFloat() * 50.0f;
            particle.rotVel.y = 25.0f - math_randomFloat() * 50.0f;
            particle_newModelParticle(&particle, 3, 0xFF, 0, 0xFF, 0);
            count1--;
        }
        while (count2 > 0) {
            Vec3f pos;
            Vec3f dir;
            f32 distance;
            f32 speed;
            s16 angle;
            f32 sine2;
            f32 cosine2;

            distance = math_randomFloat() * width;
            speed = math_randomFloat() * 7.0f + 7.0f;
            angle = math_random() & 0xFF;
            sine2 = math_cosBinAngleMasked(angle);
            cosine2 = math_cosBinAngleMasked(angle - 0x40);
            pos.x = actor->xpos + sine2 * distance;
            pos.y = actor->ypos + math_randomFloat() * height + 5.0f;
            pos.z = actor->zpos - cosine2 * distance;
            dir.x = cosine2 * speed + offsetX;
            dir.y = math_randomFloat() * 9.0f + 15.0f;
            dir.z = sine2 * speed + offsetZ;
            particle_spawnSplash(&pos, &dir, 1.6f, (math_random() & 7) + 0x19, (math_random() & 0x7F) + 0x80,
                          math_randomFloat() * 0.2f * width + 13.0f, (math_random() & 3) + 2, 0,
                          1.0f, 1.0f, 1, 1, 0, 0x10, 0xF, 0, 0xFF, 0);
            count2--;
        }
        particle.model = 0;
        particle.lifetime = (math_random() & 0xF) + 0x10;
        particle.elasticity = 0.6f;
        particle.rot.x = actor->vtilt;
        particle.rot.y = actor->theta;
        particle.rot.z = actor->lean_angle;
        particle.flags = 0x39E8;
        particle.scale.x = 1.0f;
        particle.scale.y = 1.0f;
        particle.scale.z = 1.0f;
        particle.rotVel.z = 0.0f;
        particle.gravity = -1.6f;
        particle.pos.x = actor->xpos;
        particle.pos.y = actor->ypos;
        particle.pos.z = actor->zpos;
        particle.radius = actor->scale_xz * 20.0f;
        particle.sizeXZ = actor->scale_xz + actor->scale_xz;
        particle.variant = 0;
        particle.zone = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 1;
        particle.update2Fn = 0;
        particle.floorHitFn = 0;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 0;
        particle.lightMode = 0;
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

            speed = actor->scale_y * (math_randomFloat() * 30.0f) + 10.0f;
            yaw = math_random() & 0x7F;
            pitch = -0x20 - (math_random() & 0x1F);
            yawSine = math_cosBinAngleMasked(yaw);
            yawCosine = math_cosBinAngleMasked(yaw - 0x40);
            pitchSine = math_cosBinAngleMasked(pitch);
            pitchCosine = math_cosBinAngleMasked(pitch - 0x40);
            horizontal = speed * pitchSine;
            particle.vel.x = horizontal * yawCosine + offsetX;
            particle.vel.y = -speed * pitchCosine;
            particle.vel.z = horizontal * yawSine + offsetZ;
            particle.rotVel.x = 30.0f - math_randomFloat() * 60.0f;
            particle.rotVel.y = 30.0f - math_randomFloat() * 60.0f;
            particle_newModelParticle(&particle, 3, 0xFF, 0, 0xFF, 0);
        }
        sound_playAt(0xAA, 0x6590, 0, 0, 0, actor->xpos, actor->ypos,
                      actor->zpos, 0xC8, 0x7D0);
    }
}

/* Actor id 0x20: like func_150BBB5C but smaller, with no request; stops early if spawning fails. */
s32 func_150BC488(Actor *actor) {
    f32 width;
    s32 count;
    s32 ok;
    s32 total;
    s32 n;
    ParticleSpawn particle;

    if (actor->character_type != 0x20) {
        return 1;
    }
    if (actor != NULL && actor->type != 0) {
        width = actor->bodyRadius;
        count = (math_random() & 3) + 6;
        particle.model = 1;
        particle.flags = 0x29E9;
        particle.variant = 0;
        particle.zone = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 1;
        particle.update2Fn = 0;
        particle.floorHitFn = 2;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 0;
        particle.lightMode = 0;
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

            angle = math_random();
            spreadSine = math_cosBinAngleMasked(angle);
            spreadCosine = math_cosBinAngleMasked(angle - 0x40);
            distance = math_randomFloat() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            heading = math_random();
            headingSine = math_cosBinAngleMasked(heading);
            headingCosine = math_cosBinAngleMasked(heading - 0x40);
            speed = math_randomFloat() * 4.0f + 4.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            velocityY = actor->yspd * 0.05f;
            sizeA = math_randomFloat() * 0.04f + 0.02f;
            sizeB = math_randomFloat() * 0.04f + 0.02f;
            particle.sizeXZ = sizeA;
            particle.sizeY = sizeB;
            particle.radius = (sizeA + sizeB) * 0.5f;
            particle.rot.x = math_randomFloat() * 360.0f;
            particle.rot.y = math_randomFloat() * 360.0f;
            particle.rot.z = math_randomFloat() * 360.0f;
            particle.pos.x = actor->xpos + spreadX;
            particle.pos.y = actor->ypos;
            particle.pos.z = actor->zpos + spreadZ;
            particle.vel.x = velocityX;
            particle.vel.y = velocityY;
            particle.vel.z = velocityZ;
            particle.rotVel.x = 25.0f - math_randomFloat() * 50.0f;
            particle.rotVel.y = 25.0f - math_randomFloat() * 50.0f;
            particle.lifetime = (math_random() & 0x3F) + 0x64;
            if (particle_newModelParticle(&particle, 3, 0xFF, 0, 0xFF, 0) == NULL) {
                ok = 0;
            }
            count--;
        }
        for (n = (math_random() & 3) + 2; n > 0; n--) {
            Vec3f position;
            Vec3f velocity;
            f32 speed;
            s16 angle;
            f32 sine;
            f32 cosine;

            speed = math_randomFloat() * 7.0f + 7.0f;
            angle = math_random() & 0xFF;
            sine = math_cosBinAngleMasked(angle);
            cosine = math_cosBinAngleMasked(angle - 0x40);
            position.x = actor->xpos;
            position.y = actor->ypos + math_randomFloat() * 5.0f;
            position.z = actor->zpos;
            velocity.x = cosine * speed;
            velocity.y = 0.0f;
            velocity.z = sine * speed;
            particle_spawnSplash(&position, &velocity, 1.6f, (math_random() & 7) + 0x19,
                          (math_random() & 0x7F) + 0x80,
                          math_randomFloat() * 0.2f * width + 5.0f, (math_random() & 1) + 2, 0,
                          1.0f, 1.0f, 1, 1, 0, 0x10, 0xF, 0, 0xFF, 0);
        }
        total = (math_random() & 1) + (math_random() & 1);
        if (total != 0) {
            s8 j;

            particle.model = 0;
            particle.lifetime = (math_random() & 0xF) + 0x10;
            particle.elasticity = 0.6f;
            particle.rot.x = actor->vtilt;
            particle.rot.y = actor->theta;
            particle.rot.z = actor->lean_angle;
            particle.flags = 0x39E8;
            particle.scale.x = 1.0f;
            particle.scale.y = 1.0f;
            particle.scale.z = 1.0f;
            particle.rotVel.z = 0.0f;
            particle.gravity = -1.6f;
            particle.pos.x = actor->xpos;
            particle.pos.y = actor->ypos;
            particle.pos.z = actor->zpos;
            particle.radius = actor->scale_xz * 20.0f;
            particle.sizeXZ = actor->scale_xz * 2.0f;
            particle.variant = 0;
            particle.zone = 0;
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
            particle.lightMode = 0;
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

                speed = math_randomFloat() * 5.0f + 8.0f;
                heading = math_random() & 0x7F;
                pitch = 0x20 - (math_random() & 0x3F);
                headingSine = math_cosBinAngleMasked(heading);
                headingCosine = math_cosBinAngleMasked(heading - 0x40);
                pitchSine = math_cosBinAngleMasked(pitch);
                pitchCosine = math_cosBinAngleMasked(pitch - 0x40);
                horizontal = speed * pitchSine;
                particle.vel.x = horizontal * headingCosine;
                particle.vel.y = -speed * pitchCosine;
                particle.vel.z = horizontal * headingSine;
                particle.rotVel.x = 30.0f - math_randomFloat() * 60.0f;
                particle.rotVel.y = 30.0f - math_randomFloat() * 60.0f;
                particle_newModelParticle(&particle, 3, 0xFF, 0, 0xFF, 0);
            }
        }
    }
}

/* Actor id 0x20: sometimes spawns one 0x29E9 particle moving forward or backward along unk76. */
s32 func_150BCBBC(Actor *actor) {
    f32 width;
    f32 height;
    s32 count;
    s32 ok;
    ParticleSpawn particle;

    if (actor->character_type != 0x20) {
        return 1;
    }
    if (actor != NULL && actor->type != 0) {
        width = actor->bodyRadius;
        height = actor->bodyHeight;
        count = math_random() & 1;
        particle.model = 1;
        particle.lifetime = 0x96;
        particle.flags = 0x29E9;
        particle.variant = 0;
        particle.zone = 0;
        particle.alpha = 0xFF;
        particle.updateFn = 1;
        particle.update2Fn = 0;
        particle.floorHitFn = 2;
        particle.floorType3Fn = 0;
        particle.wallHitFn = 0;
        particle.destroyFn = 0;
        particle.squashFn = 0;
        particle.matrixFn = 0;
        particle.lightMode = 0;
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

            angle = math_random();
            spreadSine = math_cosBinAngleMasked(angle);
            spreadCosine = math_cosBinAngleMasked(angle - 0x40);
            distance = math_randomFloat() * width;
            spreadX = spreadSine * distance;
            spreadZ = -spreadCosine * distance;
            distance = math_randomFloat() * 0.03f + 0.01f;
            sizeA = math_randomFloat() * 0.03f + 0.01f;
            // matching: the unsigned offset and the casts are needed as written
            if (math_random() & 1) {
                heading = (u8)((actor->angle >> 8) + (s32)(8 - (math_random() & 0xF)));
            } else {
                heading = (u8)((actor->angle >> 8) + (s32)(8 - (math_random() & 0xF)) - 0x80);
            }
            headingSine = math_cosBinAngleMasked(heading);
            headingCosine = math_cosBinAngleMasked(heading - 0x40);
            speed = math_randomFloat() * 5.0f + 3.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            particle.sizeXZ = distance;
            particle.sizeY = sizeA;
            particle.radius = (distance + sizeA) * 0.5f;
            particle.rot.x = math_randomFloat() * 360.0f;
            particle.rot.y = math_randomFloat() * 360.0f;
            particle.rot.z = math_randomFloat() * 360.0f;
            particle.pos.x = actor->xpos + spreadX;
            particle.pos.y = actor->ypos + math_randomFloat() * height + 4.0f;
            particle.pos.z = actor->zpos + spreadZ;
            particle.vel.x = velocityX;
            particle.vel.y = math_randomFloat() * 5.0f + 3.0f;
            particle.vel.z = velocityZ;
            particle.rotVel.x = 25.0f - math_randomFloat() * 50.0f;
            particle.rotVel.y = 25.0f - math_randomFloat() * 50.0f;
            if (particle_newModelParticle(&particle, 3, 0xFF, 0, 0xFF, 0) == NULL) {
                ok = 0;
            }
            count--;
        }
    }
}


struct Obj150BCFB8 {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 bounce;
    /* 0x18 */ u8 pad18[0x3C - 0x18];
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ u8 pad40[0x44 - 0x40];
    /* 0x44 */ Vec3f vel;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ u8 pad5C[0x60 - 0x5C];
    /* 0x60 */ s32 flags;
};

/* Ground-hit callback: comes to rest when falling slowly, otherwise bounces, scaling all motion by obj->bounce. */
s32 effect_bounceOnGround(struct Obj150BCFB8 *obj, f32 x, f32 y, f32 z, f32 floorY) {
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

/* Burst of particles and spawned particles from two random anchors (entries 0x4C/0x4D of zones). */
void func_150BD070(u8 owner, s32 list) {
    TriggerZone *anchors[2];
    const u8 idx[2] = {0x4C, 0x4D};
    s16 count;

    anchors[0] = zones + idx[0];
    anchors[1] = zones + idx[1];
    {
        ParticleSpawn particle;
        s32 rnd;

        count = math_random() % 9U + 13;
        particle.flags = 0x12DE8;
        particle.model = 1;
        particle.variant = 0;
        particle.zone = 0;
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
        particle.lightMode = 2;
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
            particle.lifetime = math_random() % 26U + 40;
            particle.sizeXZ = particle.sizeY = math_randomFloat() * 0.2470000088f + 0.2030000091f;
            particle.rot.x = math_randomFloat() * 360.0f;
            particle.rot.y = math_randomFloat() * 360.0f;
            particle.rot.z = math_randomFloat() * 360.0f;
            // matching: the "+ -c" addends here must stay; writing "- c" changes the emitted code
            particle.rotVel.x = math_randomFloat() * 45.46600342f + -22.73300171f;
            particle.rotVel.z = math_randomFloat() * 45.46600342f + -22.73300171f;
            math_randomPointInZone(anchors[math_random() & 1], &particle.pos);
            rnd = math_random();
            math_dirFromBinAngles(rnd % 31U - 84, math_random() % 26U - 45, math_randomFloat() * 21.80000114f + 30.2f, &particle.vel);
            particle.gravity = math_randomFloat() * 1.129000068f + -1.909f;
            particle_newModelParticleWithFloor(&particle, 3, 0xFF, 0, 0, owner, list);
            count--;
        }
    }
    {
        ParticleDesc spawn;
        f32 scale;
        s32 rnd;
        Vec3f pos;
        Vec3f pos2;

        count = math_random() % 9U + 17;
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
        spawn.offset = gZeroVec3f;
        rnd = (math_random() & 1) ? 0x40 : 0;
        spawn.flags = ((math_random() & 1) ? 0x80 : 0) | 7 | rnd | 0x8000 | 0x4000 | 0x800 | 0x400 | 0x200 | 0x20000 | 0x800000;
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
            SpriteParticle *spawned;

            spawn.scaleStart = spawn.timer = math_random() % 31U + 30;
            spawn.alpha = math_random() % 101U + 155;
            spawn.width = spawn.height = math_randomFloat() * 755.0f + 1262.0f;
            math_randomPointInZone(anchors[math_random() & 1], &spawn.pos);
            rnd = math_random();
            math_dirFromBinAngles(rnd % 41U - 89, math_random() % 51U - 64, math_randomFloat() * 45.60000229f + 18.80000114f, &spawn.vel);
            spawn.gravity = math_randomFloat() * 0.559f + -0.777f;
            spawned = particle_newSpriteParticleWithFloor(&spawn, 1, 0, 4, owner, list);
            if (spawned != NULL) {
                memcpy(&spawned->data.drag, &scale, sizeof(scale));
            }
            count--;
        }
        pos.x = anchors[0]->x;
        pos.y = anchors[0]->y;
        pos.z = anchors[0]->z;
        particle_startPlayerCameraWobbles(&pos, 0.0f, 1000.0f, 0.001f, 5, owner, list);
        pos2.x = anchors[0]->x;
        pos2.y = anchors[0]->y;
        pos2.z = anchors[0]->z;
        particle_startPlayerRumbles(&pos2, 506.0f, 1013.0f, 0.0009871669f, 0xF, 0x14, owner, list);
    }
}

