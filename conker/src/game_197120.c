#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Per-frame update of a bouncing debris particle: integrate motion, land/bounce on the floor, fade out. */
void func_15169C70(Debris *debris) {
    f32 dt;
    s32 floorHeight;
    s32 timer;
    s32 alpha;

    dt = D_800BE9E4;
    debris->x += debris->velX * dt;
    debris->z += debris->velZ * dt;
    debris->rotX += debris->spinX * dt;
    debris->rotY += debris->spinY * dt;
    debris->rotZ += debris->spinZ * dt;
    debris->velY += -0.5f * dt;
    debris->y += debris->velY * dt;
    if (debris->flags & 1) {
        debris->phase += D_800BE9E4 * 8;
        debris->scale = (func_15048A40(debris->phase) * 0.400000006f + 1.0f) * debris->baseScale;
    }
    if (debris->flags & 2) {
        debris->rotY = func_150484A0(debris->velX, debris->velZ) * 57.29577637f;
        debris->rotX = func_150484A0(-debris->velY, sqrtf(debris->velX * debris->velX +
                                                           debris->velZ * debris->velZ)) *
                       57.29577637f;
    }
    collision_selectMap(0);
    floorHeight = collision_findMeshFloorNear(debris->x, debris->y, debris->z, 0) + debris->floorOffset;
    if (debris->y < floorHeight && debris->velY < 0.0f) {
        debris->y = floorHeight;
        if (debris->kind == 1) {
            func_15171200(4, 0xB, debris->x, debris->y, debris->z, debris->velY * debris->bounce,
                          0.5f, 1.0f, 60.0f, 2, 255, -0.400000006f, debris->header.owner, debris->header.kind);
            // matching: the (u32) step selects IDO's unsigned int-to-float sequence
            func_151718F0(debris->x, (debris->y - (f32)(u32)debris->floorOffset) + 30.0f,
                          debris->z, 50.0f, 40, 0.0030517578125f, 0, 20, debris->header.owner,
                          debris->header.kind);
            func_151718F0(debris->x, (debris->y - (f32)(u32)debris->floorOffset) + 15.0f,
                          debris->z, 30.0f, 40, 0.0030517578125f, 0, 10, debris->header.owner,
                          debris->header.kind);
            attach_releaseModel(debris->sound);
            func_1516972C(&debris->header);
            return;
        }
        if (debris->kind == 4) {
            func_15171200(3, 0xB, debris->x, debris->y, debris->z, debris->velY * debris->bounce,
                          0.5f, 0.6999999881f, 40.0f, 2, 60, -0.5f, debris->header.owner, debris->header.kind);
            attach_releaseModel(debris->sound);
            func_1516972C(&debris->header);
            return;
        }
        if (debris->kind == 3) {
            func_15171200(2, 0xCB, debris->x, debris->y, debris->z, 6.0f, 0.5f, 1.0f, 60.0f, 5, 60,
                          -0.6999999881f, debris->header.owner, debris->header.kind);
            func_151709B4(debris->x, debris->y + 10.0f, debris->z, 4, debris->velY * 200.0f,
                          debris->header.owner, debris->header.kind);
            attach_releaseModel(debris->sound);
            func_1516972C(&debris->header);
            return;
        }
        if (debris->kind == 6) {
            debris->velZ = 0.0f;
            debris->velY = 0.0f;
            debris->velX = 0.0f;
            debris->spinZ = 0.0f;
            debris->spinY = 0.0f;
            debris->spinX = 0.0f;
            debris->bounce = 0.0f;
            debris->kind = 0;
            debris->timer = 60;
            return;
        }
        if (debris->kind == 8) {
            attach_releaseModel(debris->sound);
            func_1516972C(&debris->header);
            return;
        }
        debris->velX *= debris->friction;
        debris->velZ *= debris->friction;
        debris->velY *= debris->bounce;
        if (debris->velY < 2.0f) {
            // matching: integer 0 (not 0.0f) makes IDO store $zero directly
            debris->velX = 0;
            debris->velY = 0;
            debris->velZ = 0;
            debris->spinX = 0;
            debris->spinY = 0;
            debris->spinZ = 0;
        } else {
            func_151718F0(debris->x, (debris->y - (f32)(u32)debris->floorOffset) + 15.0f,
                          debris->z, 20.0f, 50, 0.000457763671875f, 0, 4, debris->header.owner,
                          debris->header.kind);
            if (debris->kind == 2) {
                if (func_150ADA20() & 0x8000) {
                    // matching: the original treats this random draw as signed (sra + signed int->float)
                    debris->spinX = (f32)((s32)func_150ADA20() >> 16) * 0.000152587890625f + 40.0f;
                }
            } else if (debris->kind == 5) {
                func_151709B4(debris->x, debris->y + 10.0f, debris->z, 4, debris->velY * 200.0f,
                              debris->header.owner, debris->header.kind);
            }
        }
        debris->spinX *= debris->spinDamp;
        debris->spinZ *= debris->spinDamp;
    }
    timer = debris->timer;
    if (timer != 0) {
        timer -= D_800BE9E4;
        if (timer <= 0) {
            debris->state = 2;
            alpha = debris->alpha;
            alpha -= D_800BE9E4 * 4;
            if (alpha <= 0) {
                attach_releaseModel(debris->sound);
                func_1516972C(&debris->header);
                return;
            }
            debris->alpha = alpha;
            timer = 1;
        }
        debris->timer = timer;
    }
}


/* Drop callback: once it reaches the floor it dies and leaves a splash sprite; in the air, x/z drag. */
void func_1516A3F4(DropParticle *drop) {
    s32 intensity;
    s32 bottom;
    s32 floorY;
    SpriteFxTemplate splash;
    s32 dim;

    bottom = drop->y + drop->unk2A;
    floorY = drop->floorY;
    if (floorY >= bottom) {
        collision_selectMap(0);
        floorY = drop->floorY = collision_findMeshFloorNear(drop->x, drop->y, drop->z, 0);
    }
    if (floorY >= bottom) {
        drop->life = 0;
        if (bottom - floorY < 50) {
            splash.anim = D_8008CA4C[36];
            splash.frame = 0;
            splash.frameStep = 0x100;
            splash.x = drop->x;
            splash.y = drop->floorY + 10;
            splash.z = drop->z;
            intensity = drop->unk3B;
            splash.fadeStep = 0;
            dim = (intensity << 12) / 5100 + 204;
            splash.scaleX = dim;
            splash.scaleY = dim;
            splash.unk15 = intensity;
            splash.callback = 3; /* D_8008CA20[3] = func_1516A538 */
            func_15167AD8(&splash, drop->owner, drop->list);
        }
    } else {
        drop->velX *= 0.9800000191f;
        drop->velZ *= 0.9800000191f;
    }
}

/* Splash callback: from frame 3 on, spawns a sprite with a random texture (0x20-0x22) and angle each frame. */
void func_1516A538(SplashFx *fx) {
    u32 angle;
    u32 texture;
    s32 intensity;

    if (fx->frame >= 0x300) {
        texture = func_150ADA20() % 3;
        intensity = fx->unk25;
        angle = func_150ADA20() % 360;
        func_15171D4C(fx->x, fx->y - 10, fx->z, 100, 0, texture + 0x20, angle, 0,
                      (intensity * 25) / 255 + 25, 0, 0x100, 0, fx->owner, fx->list);
    }
}

/* Particle kind handler: fades the particle in or out, applies gravity and drift, and bounces it
 * off the floor. Returns 1 once it has faded out completely. */
s32 func_1516A648(StyleParticle *particle) {
    s32 alpha;
    s32 floorY;

    alpha = particle->alpha;
    if (particle->lifetime != 0) {
        if (alpha != 255) {
            alpha += D_800BE9E4 * 16;
            if (alpha >= 256) {
                alpha = 255;
            }
            particle->alpha = alpha;
        }
    } else if (alpha != 0) {
        alpha -= D_800BE9E4 * 4;
        if (alpha < 0) {
            alpha = 0;
        }
        particle->alpha = alpha;
    }

    if (particle->lifetime == 0 && alpha == 0) {
        return 1;
    }

    particle->velocityY -= 300;
    func_1516F864(particle);
    floorY = collision_findMeshFloorNear(particle->x, particle->y, particle->z, 0) + 10;
    particle->u.bounce.floorHi = floorY >> 8;
    // matching: the redundant mask steers IDO's scratch registers for the code that follows
    particle->u.bounce.floorLo = floorY & 0xFF;
    if (particle->y < floorY) {
        particle->y = floorY;
        particle->velocityY *= -0.5f;
        func_1516F94C(particle, 230);
    }
    return 0;
}
