#include <ultra64.h>
#include <math.h>
#include "conker/attach.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/math.h"
#include "conker/particle.h"


/* Per-frame update of a bouncing debris particle: integrate motion, land/bounce on the floor, fade out. */
void particle_updateBounceDebris(Debris *debris) {
    f32 dt;
    s32 floorHeight;
    s32 timer;
    s32 alpha;

    dt = gFrameTicks;
    debris->x += debris->velX * dt;
    debris->z += debris->velZ * dt;
    debris->rotX += debris->spinX * dt;
    debris->rotY += debris->spinY * dt;
    debris->rotZ += debris->spinZ * dt;
    debris->velY += -0.5f * dt;
    debris->y += debris->velY * dt;
    if (debris->flags & 1) {
        debris->phase += gFrameTicks * 8;
        debris->scale = (math_sinBinAngle(debris->phase) * 0.400000006f + 1.0f) * debris->baseScale;
    }
    if (debris->flags & 2) {
        debris->rotY = math_atan2(debris->velX, debris->velZ) * 57.29577637f;
        debris->rotX = math_atan2(-debris->velY, sqrtf(debris->velX * debris->velX +
                                                           debris->velZ * debris->velZ)) *
                       57.29577637f;
    }
    collision_selectMap(0);
    floorHeight = collision_findMeshFloorNear(debris->x, debris->y, debris->z, 0) + debris->floorOffset;
    if (debris->y < floorHeight && debris->velY < 0.0f) {
        debris->y = floorHeight;
        if (debris->kind == 1) {
            effect_scatterModels(4, 0xB, debris->x, debris->y, debris->z, debris->velY * debris->bounce,
                          0.5f, 1.0f, 60.0f, 2, 255, -0.400000006f, debris->header.owner, debris->header.kind);
            // matching: the (u32) step selects IDO's unsigned int-to-float sequence
            effect_spawnDustRing(debris->x, (debris->y - (f32)(u32)debris->floorOffset) + 30.0f,
                          debris->z, 50.0f, 40, 0.0030517578125f, 0, 20, debris->header.owner,
                          debris->header.kind);
            effect_spawnDustRing(debris->x, (debris->y - (f32)(u32)debris->floorOffset) + 15.0f,
                          debris->z, 30.0f, 40, 0.0030517578125f, 0, 10, debris->header.owner,
                          debris->header.kind);
            attach_releaseModel(debris->sound);
            effect_destroyObject(&debris->header);
            return;
        }
        if (debris->kind == 4) {
            effect_scatterModels(3, 0xB, debris->x, debris->y, debris->z, debris->velY * debris->bounce,
                          0.5f, 0.6999999881f, 40.0f, 2, 60, -0.5f, debris->header.owner, debris->header.kind);
            attach_releaseModel(debris->sound);
            effect_destroyObject(&debris->header);
            return;
        }
        if (debris->kind == 3) {
            effect_scatterModels(2, 0xCB, debris->x, debris->y, debris->z, 6.0f, 0.5f, 1.0f, 60.0f, 5, 60,
                          -0.6999999881f, debris->header.owner, debris->header.kind);
            particle_spawnStyleBurst(debris->x, debris->y + 10.0f, debris->z, 4, debris->velY * 200.0f,
                          debris->header.owner, debris->header.kind);
            attach_releaseModel(debris->sound);
            effect_destroyObject(&debris->header);
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
            effect_destroyObject(&debris->header);
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
            effect_spawnDustRing(debris->x, (debris->y - (f32)(u32)debris->floorOffset) + 15.0f,
                          debris->z, 20.0f, 50, 0.000457763671875f, 0, 4, debris->header.owner,
                          debris->header.kind);
            if (debris->kind == 2) {
                if (math_random() & 0x8000) {
                    // matching: the original treats this random draw as signed (sra + signed int->float)
                    debris->spinX = (f32)((s32)math_random() >> 16) * 0.000152587890625f + 40.0f;
                }
            } else if (debris->kind == 5) {
                particle_spawnStyleBurst(debris->x, debris->y + 10.0f, debris->z, 4, debris->velY * 200.0f,
                              debris->header.owner, debris->header.kind);
            }
        }
        debris->spinX *= debris->spinDamp;
        debris->spinZ *= debris->spinDamp;
    }
    timer = debris->timer;
    if (timer != 0) {
        timer -= gFrameTicks;
        if (timer <= 0) {
            debris->state = 2;
            alpha = debris->alpha;
            alpha -= gFrameTicks * 4;
            if (alpha <= 0) {
                attach_releaseModel(debris->sound);
                effect_destroyObject(&debris->header);
                return;
            }
            debris->alpha = alpha;
            timer = 1;
        }
        debris->timer = timer;
    }
}


/* Drop callback: once it reaches the floor it dies and leaves a splash sprite; in the air, x/z drag. */
void particle_updateDropToSplash(DropParticle *drop) {
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
            splash.anim = gEffectAnimTextures[36];
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
            splash.callback = 3; /* gObjectCallbacks[3] = particle_updateSplashFx */
            effect_spawnSpriteFx(&splash, drop->owner, drop->list);
        }
    } else {
        drop->velX *= 0.9800000191f;
        drop->velZ *= 0.9800000191f;
    }
}

/* Splash callback: from frame 3 on, spawns a sprite with a random texture (0x20-0x22) and angle each frame. */
void particle_updateSplashFx(SplashFx *fx) {
    u32 angle;
    u32 texture;
    s32 intensity;

    if (fx->frame >= 0x300) {
        texture = math_random() % 3;
        intensity = fx->unk25;
        angle = math_random() % 360;
        particle_spawnFloorSprite(fx->x, fx->y - 10, fx->z, 100, 0, texture + 0x20, angle, 0,
                      (intensity * 25) / 255 + 25, 0, 0x100, 0, fx->owner, fx->list);
    }
}

/* Particle kind handler: fades the particle in or out, applies gravity and drift, and bounces it
 * off the floor. Returns 1 once it has faded out completely. */
s32 particle_updateStyleBounce(StyleParticle *particle) {
    s32 alpha;
    s32 floorY;

    alpha = particle->alpha;
    if (particle->lifetime != 0) {
        if (alpha != 255) {
            alpha += gFrameTicks * 16;
            if (alpha >= 256) {
                alpha = 255;
            }
            particle->alpha = alpha;
        }
    } else if (alpha != 0) {
        alpha -= gFrameTicks * 4;
        if (alpha < 0) {
            alpha = 0;
        }
        particle->alpha = alpha;
    }

    if (particle->lifetime == 0 && alpha == 0) {
        return 1;
    }

    particle->velocityY -= 300;
    particle_moveStyleXZ(particle);
    floorY = collision_findMeshFloorNear(particle->x, particle->y, particle->z, 0) + 10;
    particle->u.bounce.floorHi = floorY >> 8;
    // matching: the redundant mask steers IDO's scratch registers for the code that follows
    particle->u.bounce.floorLo = floorY & 0xFF;
    if (particle->y < floorY) {
        particle->y = floorY;
        particle->velocityY *= -0.5f;
        particle_scaleStyleVelXZ(particle, 230);
    }
    return 0;
}
