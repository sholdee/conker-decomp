#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
s32 func_150ADA20(void);
#include "variables.h"

s32 func_15147DA0(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 *arg11,
                  s32 arg12, u8 arg13, s32 arg14);

/* Particle description passed to the particle spawners (func_15132A4C, func_1513264C). */
typedef struct GameE8C10Particle {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ vertex rotation; /* degrees */
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex rotationVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[0x3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ s8 unk69;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B[0x1];
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerUniqueId;
    /* 0x71 */ u8 pad71[0x1];
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8 pad76[0x2];
    /* 0x78 */ s32 unk78;
} GameE8C10Particle; /* size 0x7C */

void *func_15132A4C(void *, s32, s32, s32, u8, s32);

s32 func_150BB760(struct127 *actor) {
    GameE8C10Particle particle;
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
        particle.unk56 = 1;
        particle.unk50 = 0x29E9;
        particle.unk58 = 0;
        particle.unk5C = 0;
        particle.unk60 = 0xFF;
        particle.unk61 = 1;
        particle.unk62 = 0;
        particle.unk63 = 2;
        particle.unk64 = 0;
        particle.unk65 = 0;
        particle.unk66 = 0;
        particle.unk67 = 0;
        particle.unk68 = 0;
        particle.unk6A = 1;
        particle.owner = actor;
        particle.unk1C = 1.0f;
        particle.unk20 = 1.0f;
        particle.unk24 = 1.0f;
        particle.rotationVel.z = 0.0f;
        particle.gravity = -1.0f;
        particle.ownerUniqueId = actor->unique_id;
        particle.unk72 = 0x20;
        particle.unk74 = 7;
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
            particle.unk8 = size;
            particle.unkC = size2;
            particle.unk0 = (size + size2) * 0.5f;
            particle.rotation.x = func_150ADA68() * 360.0f;
            particle.rotation.y = func_150ADA68() * 360.0f;
            particle.rotation.z = func_150ADA68() * 360.0f;
            particle.pos.x = actor->x_position + spreadX;
            particle.pos.y = actor->y_position + func_150ADA68() * height + 4.0f;
            particle.pos.z = actor->z_position + spreadZ;
            particle.vel.x = velocityX + offsetX;
            particle.vel.y = func_150ADA68() * 5.0f + 3.0f;
            particle.vel.z = velocityZ + offsetZ;
            particle.rotationVel.x = 25.0f - func_150ADA68() * 50.0f;
            particle.rotationVel.y = 25.0f - func_150ADA68() * 50.0f;
            particle.unk54 = (func_150ADA20() & 0x1F) + 0x3C;
            particle.unk4 = func_150ADA68() * 0.2f + 0.25f;
            func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0);
        }
    }
}

typedef struct GameE8C10SpawnRequest {
    u8 pad0[0x80];
    s32 field80;
    u8 pad84[0x4];
    s16 field88;
    s16 field8A;
    s16 field8C;
    s16 field8E;
    s16 field90;
    s16 field92;
    s16 field94;
    s16 field96;
    s16 field98;
    u8 pad9A[0x6];
    s8 fieldA0;
    s8 fieldA1;
    s8 fieldA2;
    u8 padA3[0x5];
} GameE8C10SpawnRequest;

void func_1516865C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_15168800(void *arg0, s32 arg1, s32 arg2);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32,
                  s32, s32, s32, s32, s32, s32, s32, s32);

s32 func_150BBB5C(struct127 *actor) {
    GameE8C10Particle particle;
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
        request.field8A = 0x80;
        request.field88 = 0;
        request.field8C = actor->x_position;
        request.field8E = actor->unk180 + 60.0f;
        request.field90 = actor->z_position;
        request.field92 = width;
        request.field94 = height;
        request.field96 = actor->unk180;
        request.fieldA2 = -1;
        request.fieldA0 = 0x2E;
        request.field98 = 0x34;
        request.field80 = alpha;
        request.fieldA1 = 0;
        func_1516865C(&request, 0xFF, 0xFF, 0xFF, 0xFF);
        func_15168800(&request, 0xFF, 0);
        particle.unk56 = 1;
        particle.unk54 = 0x78;
        particle.unk50 = 0x29E9;
        particle.unk58 = 0;
        particle.unk5C = 0;
        particle.unk60 = 0xFF;
        particle.unk61 = 1;
        particle.unk62 = 0;
        particle.unk63 = 2;
        particle.unk64 = 0;
        particle.unk65 = 0;
        particle.unk66 = 0;
        particle.unk67 = 0;
        particle.unk68 = 0;
        particle.unk6A = 0;
        particle.owner = NULL;
        particle.ownerUniqueId = 0;
        particle.unk72 = 0x20;
        particle.unk74 = 7;
        particle.unk4 = 0.4f;
        particle.unk1C = 1.0f;
        particle.unk20 = 1.0f;
        particle.unk24 = 1.0f;
        particle.rotationVel.z = 0.0f;
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
            particle.unk8 = size;
            particle.unkC = size2;
            particle.unk0 = (size + size2) * 0.5f;
            particle.rotation.x = func_150ADA68() * 360.0f;
            particle.rotation.y = func_150ADA68() * 360.0f;
            particle.rotation.z = func_150ADA68() * 360.0f;
            particle.pos.x = actor->x_position + spreadX;
            particle.pos.y = actor->y_position + func_150ADA68() * height + 5.0f;
            particle.pos.z = actor->z_position + spreadZ;
            particle.vel.x = offsetX + velocityX;
            particle.vel.y = func_150ADA68() * 7.0f + 13.0f;
            particle.vel.z = offsetZ + velocityZ;
            particle.rotationVel.x = 25.0f - func_150ADA68() * 50.0f;
            particle.rotationVel.y = 25.0f - func_150ADA68() * 50.0f;
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
        particle.unk56 = 0;
        particle.unk54 = (func_150ADA20() & 0xF) + 0x10;
        particle.unk4 = 0.6f;
        particle.rotation.x = actor->unkB8;
        particle.rotation.y = actor->unk40;
        particle.rotation.z = actor->unkC4;
        particle.unk50 = 0x39E8;
        particle.unk1C = 1.0f;
        particle.unk20 = 1.0f;
        particle.unk24 = 1.0f;
        particle.rotationVel.z = 0.0f;
        particle.gravity = -1.6f;
        particle.pos.x = actor->x_position;
        particle.pos.y = actor->y_position;
        particle.pos.z = actor->z_position;
        particle.unk0 = actor->xz_scale * 20.0f;
        particle.unk8 = actor->xz_scale + actor->xz_scale;
        particle.unk58 = 0;
        particle.unk5C = 0;
        particle.unk60 = 0xFF;
        particle.unk61 = 1;
        particle.unk62 = 0;
        particle.unk63 = 0;
        particle.unk64 = 0;
        particle.unk65 = 0;
        particle.unk66 = 0;
        particle.unk67 = 0;
        particle.unk68 = 0;
        particle.unk6A = 0;
        particle.owner = NULL;
        particle.ownerUniqueId = 0;
        particle.unk72 = 0x10;
        particle.unk74 = 0xF;
        particle.unkC = particle.unk8;
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
            particle.rotationVel.x = 30.0f - func_150ADA68() * 60.0f;
            particle.rotationVel.y = 30.0f - func_150ADA68() * 60.0f;
            func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0);
        }
        func_10010F88(0xAA, 0x6590, 0, 0, 0, actor->x_position, actor->y_position,
                      actor->z_position, 0xC8, 0x7D0);
    }
}

s32 func_150BC488(struct127 *actor) {
    f32 width;
    s32 count;
    s32 ok;
    s32 total;
    s32 n;
    GameE8C10Particle particle;

    if (actor->id != 0x20) {
        return 1;
    }
    if (actor != NULL && actor->interaction_state != 0) {
        width = actor->unkE4;
        count = (func_150ADA20() & 3) + 6;
        particle.unk56 = 1;
        particle.unk50 = 0x29E9;
        particle.unk58 = 0;
        particle.unk5C = 0;
        particle.unk60 = 0xFF;
        particle.unk61 = 1;
        particle.unk62 = 0;
        particle.unk63 = 2;
        particle.unk64 = 0;
        particle.unk65 = 0;
        particle.unk66 = 0;
        particle.unk67 = 0;
        particle.unk68 = 0;
        particle.unk6A = 0;
        particle.owner = NULL;
        particle.ownerUniqueId = 0;
        particle.unk72 = 0x20;
        particle.unk74 = 7;
        ok = 1;
        particle.unk1C = 1.0f;
        particle.unk20 = 1.0f;
        particle.unk24 = 1.0f;
        particle.unk4 = 0.4f;
        particle.rotationVel.z = 0.0f;
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
            particle.unk8 = sizeA;
            particle.unkC = sizeB;
            particle.unk0 = (sizeA + sizeB) * 0.5f;
            particle.rotation.x = func_150ADA68() * 360.0f;
            particle.rotation.y = func_150ADA68() * 360.0f;
            particle.rotation.z = func_150ADA68() * 360.0f;
            particle.pos.x = actor->x_position + spreadX;
            particle.pos.y = actor->y_position;
            particle.pos.z = actor->z_position + spreadZ;
            particle.vel.x = velocityX;
            particle.vel.y = velocityY;
            particle.vel.z = velocityZ;
            particle.rotationVel.x = 25.0f - func_150ADA68() * 50.0f;
            particle.rotationVel.y = 25.0f - func_150ADA68() * 50.0f;
            particle.unk54 = (func_150ADA20() & 0x3F) + 0x64;
            if (func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0) == NULL) {
                ok = 0;
            }
            count--;
        }
        {
            n = (func_150ADA20() & 3) + 2;
            for (; n > 0; n--) {
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
        }
        total = (func_150ADA20() & 1) + (func_150ADA20() & 1);
        if (total != 0) {
            s8 j;

            particle.unk56 = 0;
            particle.unk54 = (func_150ADA20() & 0xF) + 0x10;
            particle.unk4 = 0.6f;
            particle.rotation.x = actor->unkB8;
            particle.rotation.y = actor->unk40;
            particle.rotation.z = actor->unkC4;
            particle.unk50 = 0x39E8;
            particle.unk1C = 1.0f;
            particle.unk20 = 1.0f;
            particle.unk24 = 1.0f;
            particle.rotationVel.z = 0.0f;
            particle.gravity = -1.6f;
            particle.pos.x = actor->x_position;
            particle.pos.y = actor->y_position;
            particle.pos.z = actor->z_position;
            particle.unk0 = actor->xz_scale * 20.0f;
            particle.unk8 = actor->xz_scale * 2.0f;
            particle.unk58 = 0;
            particle.unk5C = 0;
            particle.unk60 = 0xFF;
            particle.unk61 = 1;
            particle.unk62 = 0;
            particle.unkC = particle.unk8;
            particle.unk63 = 0;
            particle.unk64 = 0;
            particle.unk65 = 0;
            particle.unk66 = 0;
            particle.unk67 = 0;
            particle.unk68 = 0;
            particle.unk6A = 0;
            particle.owner = NULL;
            particle.ownerUniqueId = 0;
            particle.unk72 = 0x10;
            particle.unk74 = 0xF;
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
                pitch = 0x20 - ((u32)func_150ADA20() & 0x3F);
                headingSine = func_151423D8(heading);
                headingCosine = func_151423D8(heading - 0x40);
                pitchSine = func_151423D8(pitch);
                pitchCosine = func_151423D8(pitch - 0x40);
                horizontal = speed * pitchSine;
                particle.vel.x = horizontal * headingCosine;
                particle.vel.y = -speed * pitchCosine;
                particle.vel.z = horizontal * headingSine;
                particle.rotationVel.x = 30.0f - func_150ADA68() * 60.0f;
                particle.rotationVel.y = 30.0f - func_150ADA68() * 60.0f;
                func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0);
            }
        }
    }
}

s32 func_150BCBBC(struct127 *actor) {
    f32 width;
    f32 height;
    s32 count;
    s32 ok;
    GameE8C10Particle particle;

    if (actor->id != 0x20) {
        return 1;
    }
    if (actor != NULL && actor->interaction_state != 0) {
        width = actor->unkE4;
        height = actor->unkE6;
        count = func_150ADA20() & 1;
        particle.unk56 = 1;
        particle.unk54 = 0x96;
        particle.unk50 = 0x29E9;
        particle.unk58 = 0;
        particle.unk5C = 0;
        particle.unk60 = 0xFF;
        particle.unk61 = 1;
        particle.unk62 = 0;
        particle.unk63 = 2;
        particle.unk64 = 0;
        particle.unk65 = 0;
        particle.unk66 = 0;
        particle.unk67 = 0;
        particle.unk68 = 0;
        particle.unk6A = 0;
        particle.owner = NULL;
        particle.ownerUniqueId = 0;
        particle.unk72 = 0x20;
        particle.unk74 = 7;
        ok = 1;
        particle.unk1C = 1.0f;
        particle.unk20 = 1.0f;
        particle.unk24 = 1.0f;
        particle.unk4 = 0.3f;
        particle.rotationVel.z = 0.0f;
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
            if (func_150ADA20() & 1) {
                heading = (u8)((actor->unk76 >> 8) + (s32)(8 - ((u32)func_150ADA20() & 0xF)));
            } else {
                heading = (u8)((actor->unk76 >> 8) + (s32)(8 - ((u32)func_150ADA20() & 0xF)) - 0x80);
            }
            headingSine = func_151423D8(heading);
            headingCosine = func_151423D8(heading - 0x40);
            speed = func_150ADA68() * 5.0f + 3.0f;
            velocityX = headingCosine * speed;
            velocityZ = headingSine * speed;
            particle.unk8 = distance;
            particle.unkC = sizeA;
            particle.unk0 = (distance + sizeA) * 0.5f;
            particle.rotation.x = func_150ADA68() * 360.0f;
            particle.rotation.y = func_150ADA68() * 360.0f;
            particle.rotation.z = func_150ADA68() * 360.0f;
            particle.pos.x = actor->x_position + spreadX;
            particle.pos.y = actor->y_position + func_150ADA68() * height + 4.0f;
            particle.pos.z = actor->z_position + spreadZ;
            particle.vel.x = velocityX;
            particle.vel.y = func_150ADA68() * 5.0f + 3.0f;
            particle.vel.z = velocityZ;
            particle.rotationVel.x = 25.0f - func_150ADA68() * 50.0f;
            particle.rotationVel.y = 25.0f - func_150ADA68() * 50.0f;
            if (func_15132A4C(&particle, 3, 0xFF, 0, 0xFF, 0) == NULL) {
                ok = 0;
            }
            count--;
        }
    }
}


struct Obj150BCFB8 {
    u8 pad0[0x10];
    f32 x10;
    f32 x14;
    u8 pad18[0x3C - 0x18];
    f32 x3C;
    u8 pad40[0x44 - 0x40];
    f32 x44;
    f32 x48;
    f32 x4C;
    f32 x50;
    f32 x54;
    f32 x58;
    u8 pad5C[0x60 - 0x5C];
    s32 x60;
};

s32 func_150BCFB8(struct Obj150BCFB8 *arg0, s32 a1, s32 a2, s32 a3, f32 arg5) {
    arg0->x3C = (arg0->x10 * 1.1f) + arg5;
    if (-2.0f < arg0->x48) {
        arg0->x44 = 0.0f;
        arg0->x60 &= ~0x6F;
        arg0->x48 = 0.0f;
        arg0->x4C = 0.0f;
    } else {
        arg0->x44 = arg0->x44 * arg0->x14;
        arg0->x48 = -arg0->x48 * arg0->x14;
        arg0->x4C = arg0->x4C * arg0->x14;
        arg0->x50 = arg0->x50 * arg0->x14;
        arg0->x54 = arg0->x54 * arg0->x14;
        arg0->x58 = arg0->x58 * arg0->x14;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    vertex pos;
    vertex unk3C;
    vertex vel;
    f32 unk54;
    u32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67;
    s16 unk68;
    u8 pad6A[2];
    f32 unk6C;
} Spawn_BD070; /* 0x70 */

void func_1514470C(struct178 *, vertex *);
void func_15143794(s16, s16, f32, vertex *);
s32 func_1513264C(void *, s32, s32, s32, s32, u8, s32);
void *func_15130280(void *, u8, s32, s32, u8, s32);
void func_151D5334(f32 *, f32, f32, f32, s32, u8, s32);

void func_150BD070(u8 arg0, s32 arg1) {
    struct178 *players[2];
    const u8 idx[2] = {0x4C, 0x4D};
    s16 count;

    players[0] = &(*(struct178 **)&D_800D3098)[idx[0]];
    players[1] = &(*(struct178 **)&D_800D3098)[idx[1]];
    {
        GameE8C10Particle particle;
        s32 r1;

        count = func_150ADA20() % 9U + 0xD;
        particle.unk50 = 0x12DE8;
        particle.unk56 = 1;
        particle.unk58 = 0;
        particle.unk5C = 0;
        particle.unk60 = 0xFF;
        particle.unk61 = 8;
        particle.unk62 = 0;
        particle.unk63 = 0;
        particle.unk64 = 0;
        particle.unk65 = 0;
        particle.unk66 = 0;
        particle.unk67 = 0;
        particle.unk68 = 2;
        particle.unk69 = -1;
        particle.unk6A = 2;
        particle.owner = NULL;
        particle.ownerUniqueId = 0;
        particle.unk72 = 0xC;
        particle.unk74 = 0x15;
        particle.unk78 = 0;
        particle.unk0 = 1.0f;
        particle.unk4 = 1.0f;
        particle.unk1C = 1.0f;
        particle.unk20 = 1.0f;
        particle.unk24 = 1.0f;
        particle.rotationVel.y = 0.0f;
        while (count > 0) {
            particle.unk54 = func_150ADA20() % 26U + 0x28;
            particle.unk8 = particle.unkC = func_150ADA68() * 0.2470000088f + 0.2030000091f;
            particle.rotation.x = func_150ADA68() * 360.0f;
            particle.rotation.y = func_150ADA68() * 360.0f;
            particle.rotation.z = func_150ADA68() * 360.0f;
            particle.rotationVel.x = func_150ADA68() * 45.46600342f + -22.73300171f;
            particle.rotationVel.z = func_150ADA68() * 45.46600342f + -22.73300171f;
            func_1514470C(players[func_150ADA20() & 1], &particle.pos);
            r1 = func_150ADA20();
            func_15143794((u32)r1 % 31U - 0x54, func_150ADA20() % 26U - 0x2D, func_150ADA68() * 21.80000114f + 30.2f, &particle.vel);
            particle.gravity = func_150ADA68() * 1.129000068f + -1.909f;
            func_1513264C(&particle, 3, 0xFF, 0, 0, arg0, arg1);
            count--;
        }
    }
    {
        Spawn_BD070 spawn;
        f32 scale;
        s32 r1;
        f32 pos[3];
        f32 pos2[3];

        count = func_150ADA20() % 9U + 0x11;
        spawn.unk0 = 0x200005;
        spawn.unk8 = 0x5103;
        spawn.unk4 = 0;
        spawn.unkC = 0;
        spawn.unk10 = 0;
        spawn.unk14 = 0xD4;
        spawn.unk15 = 0xAA;
        spawn.unk16 = 8;
        spawn.unk17 = 0xFF;
        spawn.unk18 = 0x15;
        spawn.unk19 = 4;
        spawn.unk1A = 0;
        spawn.unk1C = 0xFF;
        spawn.unk1D = 0x6C;
        spawn.unk1E = 0x23;
        spawn.unk20 = 7;
        spawn.unk24 = 1.012537f;
        spawn.unk3C = *(vertex *)&D_800A5480;
        r1 = (func_150ADA20() & 1) ? 0x40 : 0;
        spawn.unk58 = ((func_150ADA20() & 1) ? 0x80 : 0) | 7 | r1 | 0x8000 | 0x4000 | 0x800 | 0x400 | 0x200 | 0x20000 | 0x800000;
        spawn.unk5C = 0;
        spawn.unk60 = 8;
        spawn.unk61 = 6;
        spawn.unk62 = 0x10;
        spawn.unk63 = -1;
        spawn.unk64 = -1;
        spawn.unk65 = 0;
        spawn.unk66 = 0xFF;
        spawn.unk68 = 0;
        spawn.unk6C = 0.0f;
        scale = 0.917015f;
        while (count > 0) {
            void *ret;

            spawn.unk22 = spawn.unkA = func_150ADA20() % 31U + 0x1E;
            spawn.unk1B = func_150ADA20() % 101U + 0x9B;
            spawn.unk28 = spawn.unk2C = func_150ADA68() * 755.0f + 1262.0f;
            func_1514470C(players[func_150ADA20() & 1], &spawn.pos);
            r1 = func_150ADA20();
            func_15143794((u32)r1 % 41U - 0x59, func_150ADA20() % 51U - 0x40, func_150ADA68() * 45.60000229f + 18.80000114f, &spawn.vel);
            spawn.unk54 = func_150ADA68() * 0.559f + -0.777f;
            ret = func_15130280(&spawn, 1, 0, 4, arg0, arg1);
            if (ret != NULL) {
                memcpy((u8 *)ret + 0xA8, &scale, 4);
            }
            count--;
        }
        pos[0] = players[0]->unk0;
        pos[1] = players[0]->pad2;
        pos[2] = players[0]->unk4;
        func_151D5334(pos, 0.0f, 1000.0f, 0.001f, 5, arg0, arg1);
        pos2[0] = players[0]->unk0;
        pos2[1] = players[0]->pad2;
        pos2[2] = players[0]->unk4;
        func_151D5404((struct17 *)pos2, 506.0f, 1013.0f, 0.0009871669f, 0xF, 0x14, arg0, arg1);
    }
}

typedef struct { f32 x, y, z; } Vec3f_BD740;
extern Vec3f_BD740 D_8009FF40[4];
extern Vec3f_BD740 D_8009FF70[4];
extern Vec3f_BD740 D_8009FFA0[4];
void func_15143134(Vec3f_BD740 *src, Vec3f_BD740 *dst, s32 arg2);
void func_1514C470(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 arg6, s32 arg7,
                   s32 arg8, f32 arg9, Vec3f_BD740 *arg10, u8 arg11);

void func_150BD740(void *arg0, u8 arg1, s32 arg2) {
    Vec3f_BD740 pts[4];
    Vec3f_BD740 a[4];
    Vec3f_BD740 b[4];
    s32 i;
    Vec3f_BD740 d[4];

    if (arg0 != NULL) {
        if (*(s32 *)((u8 *)arg0 + 0x1D4) != 0) {
            for (i = 0; i < 4; i++) {
                func_15143134(&D_8009FF40[i], &pts[i], *(s32 *)((u8 *)arg0 + 0x1D4));
                func_15143134(&D_8009FF70[i], &a[i], *(s32 *)((u8 *)arg0 + 0x1D4));
                func_15143134(&D_8009FFA0[i], &b[i], *(s32 *)((u8 *)arg0 + 0x1D4));
            }
            d[2].x = b[2].x - a[2].x;
            d[2].y = b[2].y - a[2].y;
            d[2].z = b[2].z - a[2].z;
            func_1514C470(pts[2].x, pts[2].y, pts[2].z, pts[3].x, pts[3].y, pts[3].z,
                          func_150ADA68() * 6.0f + 14.0f, 0xD, 0, 0.0f, &d[2], arg1);
            d[3].x = b[3].x - a[3].x;
            d[3].y = b[3].y - a[3].y;
            d[3].z = b[3].z - a[3].z;
            func_1514C470(pts[3].x, pts[3].y, pts[3].z, pts[0].x, pts[0].y, pts[0].z,
                          func_150ADA68() * 6.0f + 14.0f, 0xD, 0, 0.0f, &d[3], arg1);
        }
    }
}

s32 func_150BD954(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, f32 *arg13, u8 arg14) {
    struct { f32 x; f32 y; f32 z; s16 unk0C; s16 unk0E; s32 unk10; u8 pad14; u8 unk15; s32 pad18; s32 pad1C; } sp90;
    struct { f32 unk00; f32 unk04; f32 unk08; f32 unk0C; f32 unk10; f32 unk14; u8 unk18; u8 unk19; u8 unk1A; u8 unk1B; s32 pad1C; } sp70;
    f32 r;
    struct { s32 unk00; s32 unk04; s32 unk08; s32 unk0C; s32 unk10; s32 unk14; s32 unk18; u8 unk1C; u8 unk1D; } sp4C;

    sp4C.unk00 = 0;
    sp4C.unk04 = 1;
    sp4C.unk08 = 0x160600;
    sp4C.unk0C = 3;
    sp4C.unk10 = 0x10;
    sp4C.unk14 = 0x80;
    sp4C.unk18 = 0x20;
    sp4C.unk1C = 0;
    sp4C.unk1D = 9;
    sp90.unk10 = 1;
    sp90.unk0E = 1;
    sp90.x = arg2;
    sp90.y = arg3;
    sp70.unk14 = arg3;
    sp70.unk19 = 9;
    sp70.unk1A = 0xFF;
    sp70.unk18 = 8;
    sp90.z = arg4;

    r = (func_150ADA68() * 127.0f + 85.0f) * 0.001f;
    sp70.unk04 = arg13[0] * r;
    sp70.unk08 = arg13[1] * r;
    sp70.unk0C = arg13[2] * r;
    sp70.unk1B = ((u32) func_150ADA20() % 86) + 180;
    sp90.unk15 = (func_150ADA20() & 3) + 3;
    sp90.unk0C = ((u32) func_150ADA20() % 31) + 39;
    sp70.unk00 = (func_150ADA68() * 602.0f + 407.0f) * 0.01f;
    sp70.unk10 = (func_150ADA68() * 356.0f + 1017.0f) * 0.001f;
    func_15147DA0(&sp90, &sp70, 0, 1, 0xB, 0, 0, 0, 0, 0, 0, (s32 *)&sp4C, 0, arg14, 1);
    return 1;
}

struct Sub150BDB3C {
    u8 pad1B[0x1B];
    u8 unk1B;
};

struct Obj150BDB3C {
    u8 pad98[0x1C];
    s16 unk1C;
    u8 pad1E[0x98 - 0x1E];
    struct Sub150BDB3C *unk98;
};

s32 func_150BDB3C(struct Obj150BDB3C *arg0) {
    struct Sub150BDB3C *sp = arg0->unk98;
    s32 val = arg0->unk1C << 3;

    if (val < sp->unk1B) {
        sp->unk1B = val;
    }
    return 1;
}
