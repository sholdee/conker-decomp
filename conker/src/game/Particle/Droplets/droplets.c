#include <ultra64.h>
#include "functions.h"
#include "variables.h"




/*
 * Particle callback: once the particle is below its floor height, spawns 4-7 splash droplets and
 * a particle_spawnGrowingBillboard effect there (colour 1 for model 4, else 0) and returns 0. Returns 1 while the
 * particle is still above its floor.
 */
s32 particle_splashOnFloor(FloorParticle *particle) {
    f32 *floorY; // matching: IDO keeps &particle->floorY live in a register only via this pointer

    floorY = &particle->floorY;
    if (particle->head.pos.y < particle->floorY) {
        ParticleSpawn spawn;
        BillboardSpawn effect;
        s16 count;
        f32 size;
        u8 yaw;
        u8 pitch;
        f32 speed;
        f32 sinYaw;
        f32 cosYaw;
        f32 sinPitch;
        f32 cosPitch;
        FloorParticle *droplet;
        s32 scaled;
        s32 colour;

        spawn.model = particle->head.model;
        spawn.lifetime = 50;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.flags = 0x9E8;
        spawn.elasticity = 0.4f;
        spawn.rotVel.z = 0.0f;
        spawn.gravity = -1.3f;
        spawn.pos.x = particle->head.pos.x;
        spawn.pos.y = *floorY;
        spawn.pos.z = particle->head.pos.z;
        spawn.variant = 0;
        spawn.unk5C = NULL;
        spawn.alpha = 0xFF;
        spawn.updateFn = 1;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 0;
        spawn.unk6A = 2;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;
        // matching: a separate assignment (not the for initialiser) gives the loop-entry order
        count = (func_150ADA20() & 3) + 4;
        for (; count > 0; count--) {
            size = (func_150ADA68() * (particle->head.radius * 0.5f)) + 0.05f;
            yaw = func_150ADA20() & 0xFF;
            pitch = -0x20 - (func_150ADA20() & 0x1F);
            speed = (func_150ADA68() * 10.0f) + 5.0f;
            sinYaw = math_cosBinAngleMasked(yaw - 0x40);
            cosYaw = math_cosBinAngleMasked(yaw);
            sinPitch = math_cosBinAngleMasked(pitch - 0x40);
            cosPitch = math_cosBinAngleMasked(pitch);
            spawn.sizeY = spawn.sizeXZ = spawn.radius = size;
            spawn.vel.x = speed * cosPitch * sinYaw;
            spawn.vel.y = -speed * sinPitch;
            spawn.vel.z = speed * cosPitch * cosYaw;
            spawn.rot.x = func_150ADA68() * 360.0f;
            spawn.rot.y = func_150ADA68() * 360.0f;
            spawn.rot.z = func_150ADA68() * 360.0f;
            spawn.rotVel.x = 25.0f - (func_150ADA68() * 50.0f);
            spawn.rotVel.y = 25.0f - (func_150ADA68() * 50.0f);
            if (size > 0.25f) {
                spawn.update2Fn = 2;
            } else {
                spawn.update2Fn = 4;
            }
            droplet = particle_newModelParticle(&spawn, 3, 0xFF, sizeof(f32), particle->head.header.owner, particle->head.header.kind);
            if (droplet != NULL) {
                droplet->floorY = *floorY;
            }
        }
        switch (particle->head.model) {
            case 4:
                colour = 1;
                break;
            case 5:
                colour = 0;
                break;
            default:
                colour = 0;
                break;
        }
        effect.pos.x = particle->head.pos.x;
        effect.pos.y = *floorY;
        effect.pos.z = particle->head.pos.z;
        effect.height = effect.width = particle->head.radius * 200.0f;
        effect.heightStep = effect.widthStep = ((func_150ADA68() * 10.0f) + 20.0f) * particle->head.radius;
        effect.velX = 0;
        effect.velZ = 0;
        effect.velY = (func_150ADA20() % 201U) + 300;
        effect.accelY = 0;
        effect.life = 600;
        math_pickPaletteColour(colour, &effect.primColor[0], &effect.primColor[1], &effect.primColor[2]);
        math_pickPaletteColour(colour, &effect.envColor[0], &effect.envColor[1], &effect.envColor[2]);
        effect.intensity = 0xFF;
        scaled = particle->head.radius * 511.0f;
        if (scaled > 255.0f) {
            effect.alpha = 0xFF;
        } else {
            effect.alpha = scaled;
        }
        effect.fadeStep = 10;
        effect.onExpire = 0;
        particle_spawnGrowingBillboard(&effect, 1);
        return 0;
    }
    return 1;
}

/* Returns 0 once the particle is below its floor height, 1 while it is above. */
s32 particle_dieBelowFloor(FloorParticle *particle) {
    if (particle->head.pos.y < particle->floorY) {
        return 0;
    }
    return 1;
}

/*
 * Fades the particle in by height above its floor: alpha = min(lifetime * 4, height / 2, 255).
 * Returns 0 once it is below the floor, 1 otherwise.
 */
s32 particle_fadeInAboveFloor(FloorParticle *particle) {
    f32 height;
    s32 lifetimeAlpha;
    s32 alpha;
    s32 heightAlpha;

    height = particle->head.pos.y - particle->floorY;
    if (height < 0.0f) {
        return 0;
    }

    lifetimeAlpha = particle->head.lifetime * 4;
    if (lifetimeAlpha >= 256) {
        lifetimeAlpha = 255;
    }

    heightAlpha = (s32)height >> 1;
    if (heightAlpha >= 256) {
        heightAlpha = 255;
    }

    alpha = (heightAlpha < lifetimeAlpha) ? heightAlpha : lifetimeAlpha;

    if (alpha < 0) {
        return 0;
    }

    particle->head.alpha = alpha;
    return 1;
}

/* particle_spawnModelDroplets with model 4. */
void particle_spawnDroplets(f32 *pos, f32 radius, f32 size, s32 count, u8 arg4) {
    particle_spawnModelDroplets(pos, radius, size, count, 4, 0xFF, arg4);
}

/*
 * Spawns up to count droplets of the given model at random points within radius of pos. A droplet
 * is spawned only where collision_probeFloorSurfABSet0Cached finds a floor below the point, together with 1-4 smaller ones
 * above it; every droplet remembers that floor height (see particle_splashOnFloor). arg5 is unused.
 */
void particle_spawnModelDroplets(f32 *pos, f32 radius, f32 size, s32 count, u16 model, s32 arg5, u8 arg6) {
    ParticleSpawn spawn;
    CollisionHit floor;
    u8 angle;
    f32 dist;
    FloorParticle *droplet;
    s32 n;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;

    if (pos != NULL) {
        spawn.model = model;
        floor.material = 0;
        floor.flags = 0;
        floor.source = 0;
        floor.surface = 0;
        spawn.lifetime = 300;
        spawn.flags = 0x9E8;
        spawn.variant = 0;
        spawn.unk5C = NULL;
        spawn.alpha = 0xFF;
        spawn.updateFn = 1;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 0;
        spawn.unk6A = 2;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.fadeStart = 1;
        spawn.fadeRate = 0xFF;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        floor.height = 99999.9f;
        spawn.rotVel.z = 0.0f;
        spawn.vel.x = 0.0f;
        spawn.vel.y = 0.0f;
        spawn.vel.z = 0.0f;
        spawn.elasticity = 0.4f;
        spawn.gravity = -1.3f;
        for (; count > 0; count--) {
            angle = func_150ADA20() & 0xFF;
            dist = func_150ADA68() * radius;
            spawn.radius = spawn.sizeY = spawn.sizeXZ = size;
            spawn.rot.x = func_150ADA68() * 360.0f;
            spawn.rot.y = func_150ADA68() * 360.0f;
            spawn.rot.z = func_150ADA68() * 360.0f;
            spawn.pos.x = pos[0] + math_cosBinAngleMasked(angle) * dist;
            spawn.pos.y = (pos[1] + radius) - (func_150ADA68() * 2.0f * radius);
            spawn.pos.z = pos[2] - math_cosBinAngleMasked(angle - 0x40) * dist;
            spawn.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
            spawn.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
            if (collision_probeFloorSurfABSet0Cached(&spawn.pos, 0, -10000.0f, &floor) != 0) {
                spawn.update2Fn = 2;
                droplet = particle_newModelParticle(&spawn, 3, 0xFF, sizeof(f32), arg6, 0);
                if (droplet != NULL) {
                    droplet->floorY = floor.height;
                }
                n = (func_150ADA20() & 3) + 1;
                x = spawn.pos.x;
                y = size * 16.0f + spawn.pos.y;
                z = spawn.pos.z;
                for (; n > 0; n--) {
                    angle = func_150ADA20() & 0xFF;
                    dist = func_150ADA68() * size * 16.0f;
                    scale = func_150ADA68() * 0.3f + 0.1f;
                    spawn.radius = spawn.sizeXZ = spawn.sizeY = scale;
                    spawn.rot.x = func_150ADA68() * 360.0f;
                    spawn.rot.y = func_150ADA68() * 360.0f;
                    spawn.rot.z = func_150ADA68() * 360.0f;
                    spawn.pos.x = math_cosBinAngleMasked(angle) * dist + x;
                    spawn.pos.y = func_150ADA68() * 25.0f + y;
                    spawn.pos.z = z - math_cosBinAngleMasked(angle - 0x40) * dist;
                    spawn.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
                    spawn.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
                    spawn.update2Fn = 3;
                    droplet = particle_newModelParticle(&spawn, 3, 0xFF, sizeof(f32), arg6, 0);
                    if (droplet != NULL) {
                        droplet->floorY = floor.height;
                    }
                }
            }
        }
    }
}
