#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"


/* A live func_15132A4C particle (layout as Particle in game_15F680.c). The droplets spawned in
 * this file carry 4 bytes of extra data: the height of the floor below them. */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk01;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unk0C;
    /* 0x00D */ u8 padD[3];
    /* 0x010 */ f32 radius;
    /* 0x014 */ f32 elasticity;
    /* 0x018 */ f32 sizeXZ;
    /* 0x01C */ f32 sizeY;
    /* 0x020 */ Vec3f rot;
    /* 0x02C */ Vec3f scale;
    /* 0x038 */ Vec3f pos;
    /* 0x044 */ Vec3f vel;
    /* 0x050 */ Vec3f rotVel;
    /* 0x05C */ f32 gravity;
    /* 0x060 */ u32 flags;
    /* 0x064 */ s16 lifetime;
    /* 0x066 */ u16 model;
    /* 0x068 */ u8 pad68[8];
    /* 0x070 */ u8 alpha;
    /* 0x071 */ u8 pad71[0x170 - 0x71];
    /* 0x170 */ f32 floorY;
} Particle;

/* Ground probe result written by func_15046C80 (ParticleFloor in game_15F680.c). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 pad4[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 type;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 unk20;
} ParticleFloor; // size 0x24

extern Particle *func_15132A4C(ParticleSpawn *, s32, s32, s32, u8, s32);
extern void func_1518CA80(BillboardSpawn *, u8);
extern void func_151429E0(u8, u8 *, u8 *, u8 *);
extern s32 func_15046C80(Vec3f *, s32, f32, ParticleFloor *);

/*
 * Particle callback: once the particle is below its floor height, spawns 4-7 splash droplets and
 * a func_1518CA80 effect there (colour 1 for model 4, else 0) and returns 0. Returns 1 while the
 * particle is still above its floor.
 */
s32 func_1518AD20(Particle *particle) {
    f32 *floorY; // matching: IDO keeps &particle->floorY live in a register only via this pointer

    floorY = &particle->floorY;
    if (particle->pos.y < particle->floorY) {
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
        Particle *droplet;
        s32 scaled;
        s32 colour;

        spawn.model = particle->model;
        spawn.lifetime = 50;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.flags = 0x9E8;
        spawn.elasticity = 0.4f;
        spawn.rotVel.z = 0.0f;
        spawn.gravity = -1.3f;
        spawn.pos.x = particle->pos.x;
        spawn.pos.y = *floorY;
        spawn.pos.z = particle->pos.z;
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
            size = (func_150ADA68() * (particle->radius * 0.5f)) + 0.05f;
            yaw = func_150ADA20() & 0xFF;
            pitch = -0x20 - (func_150ADA20() & 0x1F);
            speed = (func_150ADA68() * 10.0f) + 5.0f;
            sinYaw = func_151423D8(yaw - 0x40);
            cosYaw = func_151423D8(yaw);
            sinPitch = func_151423D8(pitch - 0x40);
            cosPitch = func_151423D8(pitch);
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
            droplet = func_15132A4C(&spawn, 3, 0xFF, sizeof(f32), particle->unk0C, particle->unk01);
            if (droplet != NULL) {
                droplet->floorY = *floorY;
            }
        }
        switch (particle->model) {
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
        effect.pos.x = particle->pos.x;
        effect.pos.y = *floorY;
        effect.pos.z = particle->pos.z;
        effect.height = effect.width = particle->radius * 200.0f;
        effect.heightStep = effect.widthStep = ((func_150ADA68() * 10.0f) + 20.0f) * particle->radius;
        effect.velX = 0;
        effect.velZ = 0;
        effect.velY = (func_150ADA20() % 201U) + 300;
        effect.accelY = 0;
        effect.life = 600;
        func_151429E0(colour, &effect.primColor[0], &effect.primColor[1], &effect.primColor[2]);
        func_151429E0(colour, &effect.envColor[0], &effect.envColor[1], &effect.envColor[2]);
        effect.intensity = 0xFF;
        scaled = particle->radius * 511.0f;
        if (scaled > 255.0f) {
            effect.alpha = 0xFF;
        } else {
            effect.alpha = scaled;
        }
        effect.fadeStep = 10;
        effect.onExpire = 0;
        func_1518CA80(&effect, 1);
        return 0;
    }
    return 1;
}

/* Returns 0 once the particle is below its floor height, 1 while it is above. */
s32 func_1518B1AC(Particle *particle) {
    if (particle->pos.y < particle->floorY) {
        return 0;
    }
    return 1;
}

/*
 * Fades the particle in by height above its floor: alpha = min(lifetime * 4, height / 2, 255).
 * Returns 0 once it is below the floor, 1 otherwise.
 */
s32 func_1518B1D8(Particle *particle) {
    f32 height;
    s32 lifetimeAlpha;
    s32 alpha;
    s32 heightAlpha;

    height = particle->pos.y - particle->floorY;
    if (height < 0.0f) {
        return 0;
    }

    lifetimeAlpha = particle->lifetime * 4;
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

    particle->alpha = alpha;
    return 1;
}

void func_1518B2A8(Vec3f *pos, f32 radius, f32 size, s32 count, u16 model, s32 arg5, u8 arg6);

/* func_1518B2A8 with model 4. */
void func_1518B264(Vec3f *pos, f32 radius, f32 size, s32 count, u8 arg4) {
    func_1518B2A8(pos, radius, size, count, 4, 0xFF, arg4);
}

/*
 * Spawns up to count droplets of the given model at random points within radius of pos. A droplet
 * is spawned only where func_15046C80 finds a floor below the point, together with 1-4 smaller ones
 * above it; every droplet remembers that floor height (see func_1518AD20). arg5 is unused.
 */
void func_1518B2A8(Vec3f *pos, f32 radius, f32 size, s32 count, u16 model, s32 arg5, u8 arg6) {
    ParticleSpawn spawn;
    ParticleFloor floor;
    u8 angle;
    f32 dist;
    Particle *droplet;
    s32 n;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;

    if (pos != NULL) {
        spawn.model = model;
        floor.unk18 = 0;
        floor.unk1C = 0;
        floor.type = 0;
        floor.unk20 = 0;
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
            spawn.pos.x = pos->x + func_151423D8(angle) * dist;
            spawn.pos.y = (pos->y + radius) - (func_150ADA68() * 2.0f * radius);
            spawn.pos.z = pos->z - func_151423D8(angle - 0x40) * dist;
            spawn.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
            spawn.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
            if (func_15046C80(&spawn.pos, 0, -10000.0f, &floor) != 0) {
                spawn.update2Fn = 2;
                droplet = func_15132A4C(&spawn, 3, 0xFF, sizeof(f32), arg6, 0);
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
                    spawn.pos.x = func_151423D8(angle) * dist + x;
                    spawn.pos.y = func_150ADA68() * 25.0f + y;
                    spawn.pos.z = z - func_151423D8(angle - 0x40) * dist;
                    spawn.rotVel.x = 25.0f - func_150ADA68() * 50.0f;
                    spawn.rotVel.y = 25.0f - func_150ADA68() * 50.0f;
                    spawn.update2Fn = 3;
                    droplet = func_15132A4C(&spawn, 3, 0xFF, sizeof(f32), arg6, 0);
                    if (droplet != NULL) {
                        droplet->floorY = floor.height;
                    }
                }
            }
        }
    }
}
