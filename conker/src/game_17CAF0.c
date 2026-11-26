#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151D9014 (game_2062D0.c) is declared in each file that calls it: game_1228D0.c and game/Effects/Blood/blood.c
// were compiled with a different view of it.
SteamParticle *func_151D9014(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

/*
 * Most configs below store random ranges as base/range pairs: an integer field is
 * (func_150ADA20() % (range + 1)) + base and a float field is func_150ADA68() * range + base
 * (a random float). Angles are 256-step binary angles; func_151423D8 is their cosine.
 */

/* Builds a cone basis from a triangle: the axes are two triangle edges, the direction is its normal,
 * flipped to face the same side as source->normal. */
void func_1514F640(TriangleSource *source, ConeBasis *cone) {
    cone->flags = 2;
    cone->spread = source->spread;
    if (func_15144E80(&source->triangle, &cone->axis0, &cone->axis1, &cone->direction) != 0) {
        if (func_15144A74(&cone->direction, &source->normal) < 0.0f) {
            cone->direction.x = -cone->direction.x;
            cone->direction.y = -cone->direction.y;
            cone->direction.z = -cone->direction.z;
        }
    }
}

/* Finishes whatever parts of the cone basis are still missing; returns 0 if a vector is degenerate. */
s32 func_1514F6E8(ConeBasis *cone) {
    if (!(cone->flags & 1)) {
        if (func_15145128(&cone->direction, &cone->direction, NULL, NULL) == 0) {
            return 0;
        }
        cone->direction.x *= 1000.0f;
        cone->direction.y *= 1000.0f;
        cone->direction.z *= 1000.0f;
        cone->flags |= 1;
    }
    if (!(cone->flags & 2)) {
        if (func_15146078(&cone->direction, &cone->axis0, &cone->axis1) == 0) {
            return 0;
        }
        cone->flags |= 2 | 4;
    }
    if (!(cone->flags & 4)) {
        if (func_15145128(&cone->axis0, &cone->axis0, NULL, NULL) == 0) {
            return 0;
        }
        if (func_15145128(&cone->axis1, &cone->axis1, NULL, NULL) == 0) {
            return 0;
        }
        cone->flags |= 4;
    }
    return 1;
}

/* Picks a random direction inside the cone and scales it by `speed`. */
void func_1514F808(ConeBasis *cone, f32 speed, Vec3f *out) {
    f32 second;
    f32 first;

    func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * cone->spread, &first, &second);
    out->x = (((cone->axis0.x * first) + (cone->axis1.x * second)) + cone->direction.x) * speed;
    out->y = (((cone->axis0.y * first) + (cone->axis1.y * second)) + cone->direction.y) * speed;
    out->z = (((cone->axis0.z * first) + (cone->axis1.z * second)) + cone->direction.z) * speed;
}

/* Spawns a random number of particles from config->position, each moving in a random direction inside
 * the cone around `direction` (scaled by 1000 here) spanned by axis0/axis1. */
void func_1514F8F8(JetSpray *config, Vec3f *direction, Vec3f *axis0, Vec3f *axis1, f32 spread, u8 arg5, s32 arg6) {
    s32 count;
    f32 velocity[3];
    f32 second;
    f32 first;
    f32 speed;

    direction->x *= 1000.0f;
    direction->y *= 1000.0f;
    direction->z *= 1000.0f;

    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    while (count != 0) {
        speed = (func_150ADA68() * config->speedRange) + config->speedBase;
        func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * spread, &first, &second);
        velocity[0] = ((axis0->x * first) + (axis1->x * second) + direction->x) * speed;
        velocity[1] = ((axis0->y * first) + (axis1->y * second) + direction->y) * speed;
        velocity[2] = ((axis0->z * first) + (axis1->z * second) + direction->z) * speed;
        func_151A2AD4(&config->pos, velocity,
                      (func_150ADA68() * config->sizeRange) + config->sizeBase,
                      (func_150ADA68() * config->sizeSlopeRange) + config->sizeSlopeBase,
                      (func_150ADA20() % (config->particleCountRange + 1)) + config->particleCountBase,
                      config->spread, config->drag, config->gravityBase, config->gravityRange,
                      config->delayBase, config->delayRange, config->alphaBase, config->alphaRange,
                      config->fadeStart, config->fadeOutStart, config->eventFunc, arg5, arg6);
        count--;
    }
}

void func_1514FB98(DirectedJetSpray *emitter, u8 arg1, s32 arg2) {
    Vec3f axis0;
    Vec3f axis1;

    if (func_15146078(&emitter->direction, &axis0, &axis1) != 0) {
        func_1514F8F8(&emitter->spray, &emitter->direction, &axis0, &axis1, emitter->spread, arg1, arg2);
    }
}

void func_1514FBFC(SurfaceJetSpray *emitter, u8 arg1, s32 arg2) {
    Vec3f axis0;
    f32 length0;
    f32 inverse0;
    Vec3f axis1;
    f32 length1;
    f32 inverse1;
    Vec3f normal;
    f32 normalLength;
    f32 normalInverse;

    if (func_15144E80(&emitter->source.triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &emitter->source.normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        func_1514F8F8(&emitter->spray, &normal, &axis0, &axis1, emitter->source.spread, arg1, arg2);
    }
}

void func_1514FCE8(JetSpraySpec *config, u8 arg1, s32 arg2) {
    s32 count;

    count = (func_150ADA20() % (config->spray.countRange + 1)) + config->spray.countBase;
    while (count != 0) {
        func_151A2A14(&config->spray.pos,
                      (func_150ADA20() % (config->angles.yawRange + 1)) + config->angles.yawBase,
                      (func_150ADA20() % (config->angles.pitchRange + 1)) + config->angles.pitchBase,
                      (func_150ADA68() * config->spray.speedRange) + config->spray.speedBase,
                      (func_150ADA68() * config->spray.sizeRange) + config->spray.sizeBase,
                      (func_150ADA68() * config->spray.sizeSlopeRange) + config->spray.sizeSlopeBase,
                      (func_150ADA20() % (config->spray.particleCountRange + 1)) + config->spray.particleCountBase,
                      config->spray.spread, config->spray.drag, config->spray.gravityBase, config->spray.gravityRange,
                      config->spray.delayBase, config->spray.delayRange, config->spray.alphaBase,
                      config->spray.alphaRange, config->spray.fadeStart, config->spray.fadeOutStart,
                      config->spray.eventFunc, arg1, arg2);
        count--;
    }
}

void func_1514FEFC(TriangleSource *source, DropSpray *config, CollisionHit *ground, u8 arg3, s32 arg4) {
    ConeBasis cone;

    func_1514F640(source, &cone);
    func_1514FF44(&cone, config, ground, arg3, arg4);
}

/* Spawns a random number of particles, each moving in a random direction inside the cone. */
void func_1514FF44(ConeBasis *cone, DropSpray *config, CollisionHit *ground, u8 arg3, s32 arg4) {
    s32 count;
    Vec3f velocity;

    if (func_1514F6E8(cone) != 0) {
        count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
        while (count != 0) {
            func_1514F808(cone, (func_150ADA68() * config->speedRange) + config->speedBase, &velocity);
            func_151D9014(&config->pos, &velocity, config->kind,
                          (func_150ADA68() * config->gravityRange) + config->gravityBase,
                          (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase,
                          (func_150ADA20() % (config->unk25 + 1)) + config->unk24,
                          (func_150ADA68() * config->sizeRange) + config->sizeBase,
                          func_150ADA68() < config->unk34, 1.4f, 1.4f, 1, ground, config->wobbles,
                          func_150ADA68() < config->bounceChance, arg3, arg4);
            count--;
        }
    }
}

/* Like func_1514FF44, but each direction is a random yaw/pitch from `angles`. */
void func_15150178(AngleRange *angles, DropSpray *config, CollisionHit *ground, u8 arg3, s32 arg4) {
    s32 count;
    Vec3f velocity;

    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    while (count != 0) {
        func_15143794((func_150ADA20() % (angles->yawRange + 1)) + angles->yawBase,
                      (func_150ADA20() % (angles->pitchRange + 1)) + angles->pitchBase,
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &velocity);
        func_151D9014(&config->pos, &velocity, config->kind,
                      (func_150ADA68() * config->gravityRange) + config->gravityBase,
                      (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase,
                      (func_150ADA20() % (config->unk25 + 1)) + config->unk24,
                      (func_150ADA68() * config->sizeRange) + config->sizeBase,
                      func_150ADA68() < config->unk34, 1.4f, 1.4f, 1, ground, config->wobbles,
                      func_150ADA68() < config->bounceChance, arg3, arg4);
        count--;
    }
}

/* Spawns a random number of pieces in random directions around config->origin; each picks a random
 * entry of the variant tables and gets a copy of config->extraData. */
void func_15150400(DebrisScatter *config, AngleRange *angles, u8 arg2, s32 arg3) {
    s32 count;
    ParticleSpawn spawn;
    s16 yaw;
    s16 pitch;

    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.rotVel.y = 0.0f;
    spawn.flags = 0x1 | 0x8 | 0x20 | 0x40 | 0x80 | 0x100 | ((config->flags & 1) ? 0x800 : 0) | 0x2000 | 0x10000;
    spawn.variant = 0;
    spawn.unk5C = 0;
    spawn.alpha = 0xFF;
    spawn.update2Fn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 2;
    spawn.drawFn = -1;
    spawn.unk6A = 0;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = 12;
    spawn.fadeRate = 21;
    spawn.sound = 0;
    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (angles->yawRange + 1)) + angles->yawBase;
        pitch = (func_150ADA20() % (angles->pitchRange + 1)) + angles->pitchBase;
        if (func_150ADA68() < config->floorHitChance) {
            spawn.updateFn = 13;
            spawn.floorHitFn = config->floorHitFn;
        } else {
            spawn.updateFn = 8;
            spawn.floorHitFn = 0;
        }
        spawn.elasticity = (func_150ADA68() * config->elasticityRange) + config->elasticityBase;
        spawn.rot.x = func_150ADA68() * 360.0f;
        spawn.rot.y = func_150ADA68() * 360.0f;
        spawn.rot.z = func_150ADA68() * 360.0f;
        func_15143794(yaw, pitch, (func_150ADA68() * config->speedRange) + config->speedBase, &spawn.vel);
        spawn.rotVel.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.rotVel.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.gravity = (func_150ADA68() * config->gravityRange) + config->gravityBase;
        spawn.lifetime = (func_150ADA20() % (config->lifetimeRange + 1)) + config->lifetimeBase;
        func_15143794(yaw, pitch, config->radius, &spawn.pos);
        spawn.pos.x += config->origin.x;
        spawn.pos.y += config->origin.y;
        spawn.pos.z += config->origin.z;
        {
        u32 variant;
        void *piece;
        variant = func_150ADA20() % config->variantCount;
        spawn.model = config->variantIds[variant];
        spawn.sizeY = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * config->variantScales[variant];
        spawn.sizeXZ = spawn.sizeY;
        spawn.radius = config->radiusScale * spawn.sizeXZ;
        piece = func_1513264C(&spawn, (config->flags & 1) ? 3 : 0, 0xFF, config->floor, config->extraSize, arg2, arg3);
        if (config->extraSize > 0 && piece != NULL) {
            memcpy((u8 *)piece + 0x170, config->extraData, config->extraSize);
        }
        }
        count--;
    }
}

/* Like func_15150400, but the variant tables are passed in and each variant is used at most once. */
void func_1515080C(DebrisSpray *config, const s32 *variantIds, const f32 *variantScales, s32 variantCount, void *extraData, s32 extraSize, u8 arg6, u8 arg7, s8 arg8, CollisionHit *floor, f32 radius, u8 argB, u8 argC, u8 argD, s32 argE) {
    u8 *used;
    ParticleSpawn spawn;
    s32 remaining;
    s32 count;
    s32 slot;
    s32 done;
    s16 yaw;
    s16 pitch;

    used = allocate_memory(variantCount, 2, 2, 1);
    if (used != NULL) {
        {
        s32 i;
        for (i = 0; i < variantCount; i++) {
            used[i] = FALSE;
        }
        }
        remaining = variantCount;
        spawn.radius = 1.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.elasticity = 0.5610000491f;
        spawn.rotVel.y = 0.0f;
        spawn.flags = 0x8 | 0x20 | 0x40 | (arg7 ? 0x80 : 0) | 0x100 | 0x800 | 0x1000 | 0x2000 | 0x10000 | (argB ? 0x4000 : 0) | (argC ? 0x100000 : 0);
        count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
        if (variantCount < count) {
            count = variantCount;
        }
        spawn.variant = arg6;
        spawn.unk5C = 0;
        spawn.alpha = 0xFF;
        spawn.update2Fn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 2;
        spawn.unk6A = 0;
        spawn.owner = 0;
        spawn.ownerId = 0;
        spawn.fadeStart = 12;
        spawn.fadeRate = 21;
        spawn.drawFn = arg8;
        while (count != 0) {
            yaw = (func_150ADA20() % (config->angles.yawRange + 1)) + config->angles.yawBase;
            pitch = (func_150ADA20() % (config->angles.pitchRange + 1)) + config->angles.pitchBase;
            func_15143794(yaw, pitch, (func_150ADA68() * config->speedRange) + config->speedBase, &spawn.vel);
            func_15143794(yaw, pitch, radius, &spawn.pos);
            spawn.pos.x += config->pos.x;
            spawn.pos.y += config->pos.y;
            spawn.pos.z += config->pos.z;
            spawn.rot.x = func_150ADA68() * 360.0f;
            spawn.rot.y = func_150ADA68() * 360.0f;
            spawn.rot.z = func_150ADA68() * 360.0f;
            spawn.rotVel.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.rotVel.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.gravity = (func_150ADA68() * config->gravityRange) + config->gravityBase;
            spawn.lifetime = (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase;
            {
            /* pick the skip-th unused variant, wrapping around */
            u32 skip;
            skip = func_150ADA20() % remaining;
            slot = 0;
            done = FALSE;
            do {
                while (used[slot]) {
                    slot++;
                }
                if (skip != 0) {
                    skip--;
                    slot++;
                    if (slot >= variantCount) {
                        slot = 0;
                    }
                } else {
                    done = TRUE;
                }
            } while (!done);
            }
            spawn.model = variantIds[slot];
            spawn.sizeY = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * variantScales[slot];
            remaining--;
            spawn.sizeXZ = spawn.sizeY;
            used[slot] = TRUE;
            {
            void *piece;
            if (func_150ADA68() < config->floorHitChance) {
                spawn.flags |= 1;
                spawn.floorHitFn = config->floorHitFn;
                spawn.updateFn = 13;
            } else {
                spawn.flags &= ~1;
                spawn.floorHitFn = 0;
                spawn.updateFn = 8;
            }
            piece = func_1513264C(&spawn, 3, 0xFF, floor, extraSize, argD, argE);
            if (extraData != NULL && piece != NULL) {
                memcpy((u8 *)piece + 0x170, extraData, extraSize);
            }
            }

            count--;
        }
    }
}

/* Casts a random number of rays from config->origin and spawns a decal (func_151D9B8C) on each
 * suitable surface hit closer than config->maxDistance. */
void func_15150D1C(DecalSpray *config, u8 arg1, s32 arg2) {
    CollisionTri *hitTriangle;
    Vec3f direction;
    CollisionTri triangle;
    Vec3f hit;
    f32 distance;
    s32 surface;
    s32 count;

    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    hitTriangle = &triangle; // matching: IDO keeps the buffer address in a saved register only via a variable
    while (count != 0) {
        func_15143794((func_150ADA20() % (config->angles.yawRange + 1)) + config->angles.yawBase,
                      (func_150ADA20() % (config->angles.pitchRange + 1)) + config->angles.pitchBase,
                      100.0f, &direction);
        if (func_150AC9C0(config->pos.x, config->pos.y, config->pos.z,
                          direction.x, direction.y, direction.z, NULL, hitTriangle,
                          &hit.x, &hit.y, &hit.z, &distance, &surface, 0, 0.0f) != 0
            && func_15145C90(surface) != 0 && distance < config->maxDistance) {
            func_151D9B8C(config->kind, (func_150ADA68() * config->sizeRange) + config->sizeBase,
                          (func_150ADA20() % 156U) + 100, hitTriangle, &hit,
                          (func_150ADA20() % (config->unk2A + 1)) + config->unk28,
                          1, 1, 1, arg1, arg2);
        }
        count--;
    }
}

/* Spawns a random number of randomly rotated pieces (func_1518A3C0) flying out of config->pos. */
void func_15150F90(PieceSpray *config, u8 arg1, u8 arg2, s32 arg3) {
    Vec3f jitter;
    f32 width;
    s32 count;
    Vec3f rotation;
    Vec3f direction;

    width = config->jitter + config->jitter;
    jitter.y = 0.0f;
    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    while (count != 0) {
        rotation.x = func_150ADA68() * 360.0f;
        rotation.y = func_150ADA68() * 360.0f;
        rotation.z = func_150ADA68() * 360.0f;
        func_15143794((func_150ADA20() % (config->angles.yawRange + 1)) + config->angles.yawBase,
                      (func_150ADA20() % (config->angles.pitchRange + 1)) + config->angles.pitchBase,
                      func_150ADA68() * config->directionRange + config->directionBase,
                      &direction);
        jitter.x = func_150ADA68() * width - config->jitter;
        jitter.z = func_150ADA68() * width - config->jitter;
        func_1518A3C0(&config->pos, &rotation,
                      func_150ADA68() * config->speedRange + config->speedBase,
                      &direction, &jitter,
                      func_150ADA68() * config->sizeRange + config->sizeBase,
                      config->scale, config->kind,
                      (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase,
                      arg1, 0, arg2, arg3);
        count--;
    }
}

/* Copied into the spawned object at +0x110. */
typedef struct {
    Vec3f velocity;
    f32 unkC;
    f32 unk10;
    Vec3f spin;
} Local151511FCExtra;

/* Spawns a random number of objects off a surface triangle, each moving in a random direction inside
 * the cone around the triangle's normal. */
void func_151511FC(SurfaceBurst *config, u8 arg1, s32 arg2) {
    s32 count;
    Vec3f axis0;
    f32 length0;
    f32 inverse0;
    Vec3f axis1;
    f32 length1;
    f32 inverse1;
    Vec3f normal;
    f32 normalLength;
    f32 normalInverse;
    ParticleDef desc;
    Local151511FCExtra extra;
    f32 speed;
    f32 second;
    f32 first;
    void *object;

    desc.fadeStart = config->fadeStart;
    desc.fadeRate = config->fadeRate;
    desc.texture = config->texture;
    desc.type = config->type;
    desc.renderModes = config->renderModes;
    desc.frame = 0;
    desc.frameSpeed = 0;
    desc.rgba[0] = 0xFF;
    desc.rgba[1] = 0xFF;
    desc.rgba[2] = 0xFF;
    desc.rgba[3] = 0xFF;
    desc.pos = config->position;
    desc.scale.x = 1.0f;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    desc.flags = config->flags | 0x08000000;
    desc.unk45 = 0xFF;
    desc.unk46 = config->unk46;
    desc.unk47 = config->unk47;
    extra.unk10 = config->unk6C;
    if (func_15144E80(&config->triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &config->normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        extra.spin.y = 0.0f;
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
        while (count != 0) {
            func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * config->spread, &first, &second);
            speed = (func_150ADA68() * config->speedRange) + config->speedBase;
            desc.lifetime = (func_150ADA68() * config->lifetimeRange) + config->lifetimeBase;
            desc.width = desc.height = (func_150ADA68() * config->scaleRange) + config->scaleBase;
            desc.rot.x = func_150ADA68() * 360.0f;
            desc.rot.y = func_150ADA68() * 360.0f;
            desc.rot.z = func_150ADA68() * 360.0f;
            desc.unk44 = (func_150ADA20() % (config->unk44Range + 1)) + config->unk44Base;
            extra.velocity.x = ((axis0.x * first) + (axis1.x * second) + normal.x) * speed;
            extra.velocity.y = ((axis0.y * first) + (axis1.y * second) + normal.y) * speed;
            extra.velocity.z = ((axis0.z * first) + (axis1.z * second) + normal.z) * speed;
            extra.unkC = (func_150ADA68() * config->unk50) + config->unk4C;
            extra.spin.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            extra.spin.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            // matching: the comma expression makes IDO load &D_800A4AA0 inside the loop's register plan
            object = func_1513D2F0(&desc, (0, &D_800A4AA0), 25, 0, 0, 23, ((func_150ADA20() & 1) * 2) + (func_150ADA20() & 1),
                                   config->unk60, config->unk64, 32, arg1, arg2);
            if (object != NULL) {
                memcpy((u8 *)object + 0x110, &extra, sizeof(extra));
            }
            count--;
        }
    }
}

/* Spawns a random number of particles off a surface triangle, each moving in a random direction
 * inside the cone around the triangle's normal. */
void func_15151670(SurfacePuffSpray *config, u8 arg1, s32 arg2) {
    EffectTemplate spawn;
    TrailMotion motion;
    u8 kind;
    Vec3f axis0;
    f32 length0, inverse0;
    Vec3f axis1;
    f32 length1, inverse1;
    Vec3f normal;
    f32 normalLength, normalInverse;
    f32 speed;
    f32 second;
    f32 first;
    s32 count;
    u32 rand;

    kind = arg1; // matching: IDO keeps arg1 in a saved register only through a copy
    if (func_15144E80(&config->source.triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &config->source.normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        spawn.kind = 1;
        spawn.flags = config->headerFlags;
        motion.texture = config->green;
        motion.shade = config->blue;
        motion.flags = config->red;
        motion.unk1C = config->unk9C;
        motion.unk1E = config->unk9E;
        spawn.pos = config->pos;
        spawn.material = config->unkA0;
        count = func_150ADA20() % (config->countRange + 1) + config->countBase;
        while (count != 0) {
            rand = func_150ADA20();
            func_15143874(rand & 0xFF, func_150ADA68() * config->source.spread, &first, &second);
            speed = func_150ADA68() * config->speedRange + config->speedBase;
            motion.alpha = func_150ADA20() % (config->alphaRange + 1) + config->alphaBase;
            spawn.itemCount = func_150ADA20() % (config->itemCountRange + 1) + config->itemCountBase;
            spawn.life = func_150ADA20() % (config->lifeRange + 1) + config->lifeBase;
            motion.halfWidth = func_150ADA68() * config->sizeRange + config->sizeBase;
            motion.gravity = func_150ADA68() * config->growthRange + config->growthBase;
            motion.velocity.x = (axis0.x * first + axis1.x * second + normal.x) * speed;
            motion.velocity.y = (axis0.y * first + axis1.y * second + normal.y) * speed;
            motion.velocity.z = (axis0.z * first + axis1.z * second + normal.z) * speed;
            trail_createMotionTrail(&spawn, &motion, 0, config->handlers[0], config->handlers[1],
                          config->handlers[2], config->handlers[3], config->handlers[4],
                          config->handlers[5], config->unk74, config->unk78,
                          &config->render, 0, kind, arg2);
            count--;
        }
    }
}

/* Spawns a random number of particles at config->position, each moving at a random yaw (around y)
 * and pitch; func_151423D8 is the cosine of a 256-step angle. */
void func_15151A38(PuffSpray *config, u8 arg1, s32 arg2) {
    EffectTemplate spawn;
    TrailMotion motion;
    s32 count;
    s16 yaw;
    s16 pitch;
    f32 speed;
    f32 sinPitch;
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 horizontal;

    spawn.kind = 1;
    spawn.flags = config->headerFlags;
    motion.texture = config->green;
    motion.shade = config->blue;
    motion.flags = config->red;
    spawn.pos = config->pos;
    motion.unk1C = config->unk80;
    motion.unk1E = config->unk82;
    spawn.material = config->unk84;
    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (config->angles.yawRange + 1)) + config->angles.yawBase;
        pitch = (func_150ADA20() % (config->angles.pitchRange + 1)) + config->angles.pitchBase;
        cosPitch = func_151423D8(pitch);
        sinPitch = func_151423D8(pitch - 0x40);
        cosYaw = func_151423D8(yaw);
        sinYaw = func_151423D8(yaw - 0x40);
        speed = (func_150ADA68() * config->speedRange) + config->speedBase;
        motion.alpha = (func_150ADA20() % (config->alphaRange + 1)) + config->alphaBase;
        spawn.itemCount = (func_150ADA20() % (config->itemCountRange + 1)) + config->itemCountBase;
        spawn.life = (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase;
        motion.halfWidth = (func_150ADA68() * config->sizeRange) + config->sizeBase;
        motion.gravity = (func_150ADA68() * config->growthRange) + config->growthBase;
        horizontal = speed * cosPitch;
        motion.velocity.x = horizontal * sinYaw;
        motion.velocity.y = -speed * sinPitch;
        motion.velocity.z = horizontal * cosYaw;
        trail_createMotionTrail(&spawn, &motion, 0, config->handlers[0], config->handlers[1], config->handlers[2], config->handlers[3],
                      config->handlers[4], config->handlers[5], config->unk58, config->unk5C, &config->render, 0, arg1, arg2);
        count--;
    }
}

/* Spawns a random number of pieces off a surface triangle, each moving in a random direction inside
 * the cone around the triangle's normal and using a random entry of the variant tables. */
void func_15151D6C(SurfaceDebrisBurst *config, const s32 *variantIds, const f32 *variantScales, u32 variantCount, u8 arg4, s32 arg5) {
    s32 count;
    s32 variant;
    ParticleSpawn78 spawn;
    Vec3f axis0;
    f32 length0;
    f32 inverse0;
    Vec3f axis1;
    f32 length1;
    f32 inverse1;
    Vec3f normal;
    f32 normalLength;
    f32 normalInverse;
    f32 second;
    f32 first;
    f32 speed;
    f32 pad[2]; // matching: unused stack slots in the original frame

    if (func_15144E80(&config->source.triangle, &axis0, &axis1, &normal) != 0) {
        func_15145128(&axis0, &axis0, &length0, &inverse0);
        func_15145128(&axis1, &axis1, &length1, &inverse1);
        func_15145128(&normal, &normal, &normalLength, &normalInverse);
        if (func_15144A74(&normal, &config->source.normal) < 0.0f) {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        normal.x *= 1000.0f;
        normal.y *= 1000.0f;
        normal.z *= 1000.0f;
        spawn.radius = 1.0f;
        spawn.elasticity = 1.0f;
        spawn.scale.x = 1.0f;
        spawn.scale.y = 1.0f;
        spawn.scale.z = 1.0f;
        spawn.pos = config->pos;
        spawn.rotVel.y = 0.0f;
        spawn.flags = 0x29E8;
        count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
        spawn.variant = 0;
        spawn.unk5C = 0;
        spawn.alpha = 0xFF;
        spawn.updateFn = 8;
        spawn.update2Fn = 0;
        spawn.floorHitFn = 0;
        spawn.floorType3Fn = 0;
        spawn.wallHitFn = 0;
        spawn.destroyFn = 0;
        spawn.squashFn = 0;
        spawn.matrixFn = 2;
        spawn.unk6A = 0;
        spawn.owner = 0;
        spawn.ownerId = 0;
        spawn.fadeStart = 12;
        spawn.fadeRate = 21;
        while (count != 0) {
            speed = (func_150ADA68() * config->speedRange) + config->speedBase;
            func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * config->source.spread, &first, &second);
            spawn.vel.x = ((axis0.x * first) + (axis1.x * second) + normal.x) * speed;
            spawn.vel.y = ((axis0.y * first) + (axis1.y * second) + normal.y) * speed;
            spawn.vel.z = ((axis0.z * first) + (axis1.z * second) + normal.z) * speed;
            spawn.rot.x = func_150ADA68() * 360.0f;
            spawn.rot.y = func_150ADA68() * 360.0f;
            spawn.rot.z = func_150ADA68() * 360.0f;
        // matching: the labels end basic blocks, which keeps as1 from filling the calls' delay slots
        pickVariant:
            variant = func_150ADA20() % variantCount;
            spawn.model = variantIds[variant];
            spawn.sizeY = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * variantScales[variant];
            spawn.sizeXZ = spawn.sizeY;
        randomizeSpin:
            spawn.rotVel.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.rotVel.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
            spawn.gravity = (func_150ADA68() * config->gravityRange) + config->gravityBase;
            spawn.lifetime = (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase;
            func_15132A4C(&spawn, 3, 0xFF, 0, arg4, arg5);
            count--;
        }
    }
}

/* Spawns a random number of pieces in random directions around config->origin, each using a random
 * entry of the variant tables. */
void func_15152190(DebrisBurst *config, const s32 *variantIds, const f32 *variantScales, u32 variantCount, f32 radius, u8 arg5, u8 arg6, s32 arg7) {
    s32 count;
    s32 variant;
    ParticleSpawn78 spawn;
    s16 yaw;
    s16 pitch;

    spawn.radius = 1.0f;
    spawn.elasticity = 1.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.rotVel.y = 0.0f;
    spawn.flags = (arg5 ? 0x100000 : 0) | 0x29E8;
    spawn.variant = 0;
    spawn.unk5C = 0;
    spawn.alpha = 0xFF;
    spawn.updateFn = 8;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 2;
    spawn.unk6A = 0;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = 12;
    spawn.fadeRate = 21;
    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (config->angles.yawRange + 1)) + config->angles.yawBase;
        pitch = (func_150ADA20() % (config->angles.pitchRange + 1)) + config->angles.pitchBase;
        spawn.rot.x = func_150ADA68() * 360.0f;
        spawn.rot.y = func_150ADA68() * 360.0f;
        spawn.rot.z = func_150ADA68() * 360.0f;
        func_15143794(yaw, pitch, (func_150ADA68() * config->speedRange) + config->speedBase, &spawn.vel);
        spawn.rotVel.x = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.rotVel.z = (func_150ADA68() * (config->spin + config->spin)) - config->spin;
        spawn.gravity = (func_150ADA68() * config->gravityRange) + config->gravityBase;
        spawn.lifetime = (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase;
        func_15143794(yaw, pitch, radius, &spawn.pos);
        spawn.pos.x += config->pos.x;
        spawn.pos.y += config->pos.y;
        spawn.pos.z += config->pos.z;
    // matching: the label ends a basic block, which keeps as1 from filling a call's delay slot
    pickVariant:
        variant = func_150ADA20() % variantCount;
        spawn.model = variantIds[variant];
        spawn.sizeY = (config->scaleBase + (func_150ADA68() * config->scaleRange)) * variantScales[variant];
        spawn.sizeXZ = spawn.sizeY;
        {
            void *piece; // matching: storing the unused result changes as1's ordering of the argument stores
            piece = func_15132A4C(&spawn, 3, 0xFF, 0, arg6, arg7);

        }
        count--;
    }
}

void func_15152520(ModelBurst *config, u8 arg1, s32 arg2) {
    s32 pad; // matching: unused stack slot
    EffectParams params;
    EffectMotion motion;
    s32 count;

    params.unk38 = 0;
    params.flags = config->flags | 2;
    params.updateFunc = 0;
    params.updateFunc2 = 1;
    params.preDrawFunc = -1;
    params.postDrawFunc = -1;
    params.model = config->model;
    params.unkC = config->unk38;
    params.render.unk14 = 0x80;
    params.render.unk18 = 0x20;
    params.render.unk0 = 0;
    params.render.setGeometryMode = 0x220405;
    params.render.clearGeometryMode = 0x40200;
    params.render.primMode = 0;
    params.render.envMode = 8;
    params.render.renderMode = 1;
    params.render.unk10 = 0x38;
    params.owner = 0;
    params.ownerId = 0;
    params.unk41 = 2;
    params.pos = config->position;
    motion.drag = config->drag;
    params.color0 = config->color0;
    params.color1 = config->color1;
    params.fadeStart = config->fadeStart;
    params.fadeRate = config->fadeRate;
    motion.flags = config->motionFlags;
    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    while (count != 0) {
        params.lifetime = (func_150ADA20() % (config->lifetimeRange + 1)) + config->lifetimeBase;
        motion.rot.x = func_150ADA68() * 360.0f;
        motion.rot.y = func_150ADA68() * 360.0f;
        motion.rot.z = func_150ADA68() * 360.0f;
        motion.scale = (func_150ADA68() * config->scaleRange) + config->scaleBase;
        func_15143794((func_150ADA20() % (config->angles.yawRange + 1)) + config->angles.yawBase,
                      (func_150ADA20() % (config->angles.pitchRange + 1)) + config->angles.pitchBase,
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &motion.vel);
        motion.rotVel.x = (func_150ADA68() * config->rotVelRange.x) + config->rotVelBase.x;
        motion.rotVel.y = (func_150ADA68() * config->rotVelRange.y) + config->rotVelBase.y;
        motion.rotVel.z = (func_150ADA68() * config->rotVelRange.z) + config->rotVelBase.z;
        motion.gravity = (func_150ADA68() * config->gravityRange) + config->gravityBase;
        func_15157898(&params, &motion, config->unk3C, (func_150ADA68() * config->unk44) + config->unk40, 0, 0, 0, arg1, arg2);
        count--;
    }
}

/* The color functions of DriftBurst.colorFunc. */
static void (*D_8008AC60[])(Color8 *) = { func_15152ABC };

void func_15152874(DriftBurst *config, u8 arg1, s32 arg2) {
    DriftSpawn spawn;
    s32 count;

    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    spawn.pos = config->pos;
    spawn.drag = config->drag;
    spawn.minVy = config->minVy;
    while (count != 0) {
        func_15143794((func_150ADA20() % (config->angles.yawRange + 1)) + config->angles.yawBase,
                      (func_150ADA20() % (config->angles.pitchRange + 1)) + config->angles.pitchBase,
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &spawn.vel);
        spawn.gravity = (func_150ADA68() * config->gravityRange) + config->gravityBase;
        spawn.life = (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase;
        spawn.size.width = (func_150ADA68() * config->widthRange) + config->widthBase;
        spawn.size.height = (func_150ADA68() * config->heightRange) + config->heightBase;
        if (config->colorFunc != -1) {
            D_8008AC60[config->colorFunc](&spawn.color);
        } else {
            spawn.color.red = 0xFF;
            spawn.color.green = 0xFF;
            spawn.color.blue = 0xFF;
            spawn.color.alpha = 0xFF;
        }
        func_150CCEB0(&spawn, arg1, arg2);
        count--;
    }
}

/* Five RGB colors. */
static const u8 D_800A5FE0[][3] = {
    { 0xDE, 0xA2, 0x2A }, { 0x38, 0x88, 0xCC }, { 0x97, 0xC1, 0x2A }, { 0xBD, 0x24, 0x09 }, { 0xB0, 0xAA, 0x8E },
};

/* A D_8008AC60 color function: a random color of D_800A5FE0 with a random alpha in [155, 255]. */
void func_15152ABC(Color8 *out) {
    u8 *pad; // matching: unused stack slot above rgb
    const u8 *rgb;

    rgb = D_800A5FE0[(u8)(func_150ADA20() % 5U)];
    out->alpha = (func_150ADA20() % 101U) + 155;
    out->red = rgb[0];
    out->green = rgb[1];
    out->blue = rgb[2];
}

void func_15152B38(config, arg1, arg2)
    TrailBurst *config;
    u8 arg1;
    s32 arg2;
{
    EffectTemplate param;
    TrailBurstParams spawn;
    s32 count;

    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    param.pos = config->pos;
    param.flags = config->headerFlags;
    param.kind = 10;
    spawn.flags = config->flags;
    spawn.primMode = config->primMode;
    spawn.envMode = config->envMode;
    spawn.renderMode = config->renderMode;
    spawn.unk21 = config->unk21;
    spawn.setGeometryMode = config->setGeometryMode;
    spawn.clearGeometryMode = config->clearGeometryMode;
    spawn.fadeStart = config->fadeStart;
    spawn.fadeRate = config->fadeRate;
    spawn.unk38 = config->unk38;
    spawn.unk39 = config->unk39;
    spawn.shrinkStart = config->shrinkStart;
    spawn.shrinkPaused = config->shrinkPaused;
    spawn.shrinkFactor = config->shrinkFactor;
    while (count != 0) {
        param.itemCount = (func_150ADA20() % (config->capacityRange + 1)) + config->capacityBase;
        param.life = (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase;
        spawn.width = (func_150ADA68() * config->sizeRange) + config->sizeBase;
        spawn.gravity = (func_150ADA68() * config->gravityRange) + config->gravityBase;
        func_15143794((func_150ADA20() % (config->angles.yawRange + 1)) + config->angles.yawBase,
                      (func_150ADA20() % (config->angles.pitchRange + 1)) + config->angles.pitchBase,
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &spawn.vel);
        spawn.color.red = (func_150ADA20() % (config->colorRange.red + 1)) + config->colorBase.red;
        spawn.color.green = (func_150ADA20() % (config->colorRange.green + 1)) + config->colorBase.green;
        spawn.color.blue = (func_150ADA20() % (config->colorRange.blue + 1)) + config->colorBase.blue;
        spawn.color.alpha = (func_150ADA20() % (config->colorRange.alpha + 1)) + config->colorBase.alpha;
        spawn.color2.red = (func_150ADA20() % (config->color2Range.red + 1)) + config->color2Base.red;
        spawn.color2.green = (func_150ADA20() % (config->color2Range.green + 1)) + config->color2Base.green;
        spawn.color2.blue = (func_150ADA20() % (config->color2Range.blue + 1)) + config->color2Base.blue;
        spawn.color2.alpha = (func_150ADA20() % (config->color2Range.alpha + 1)) + config->color2Base.alpha;
        spawn.colorLevel = (func_150ADA20() % (config->colorLevelRange + 1)) + config->colorLevelBase;
        trail_spawnFallingTrail(&param, 0, &spawn, 0, arg1, arg2);
        count--;
    }
}

/* Like func_15151A38, with a fixed color and fixed trail_createMotionTrail parameters. */
void func_15152F70(SimplePuffSpray *config, u8 arg1) {
    EffectTemplate spawn;
    s32 count;
    TrailMotion1C motion;
    s16 yaw;
    TrailRender params;
    s16 pitch;
    f32 speed;
    f32 cosPitch;
    f32 sinPitch;
    f32 cosYaw;
    f32 sinYaw;

    spawn.kind = 1;
    spawn.flags = 1;
    motion.texture = config->green;
    motion.shade = config->blue;
    motion.flags = config->red;
    motion.alpha = config->alpha;
    spawn.pos = config->pos;
    params.unk0 = 0;
    params.setGeometryMode = 1;
    params.clearGeometryMode = 0x160600;
    params.renderMode = 3;
    params.unk10 = 0x10;
    params.unk14 = 0x80;
    params.unk18 = 0x20;
    params.primMode = 0;
    params.envMode = 9;
    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    while (count != 0) {
        yaw = (func_150ADA20() % (config->angles.yawRange + 1)) + config->angles.yawBase;
        pitch = (func_150ADA20() % (config->angles.pitchRange + 1)) + config->angles.pitchBase;
        cosPitch = func_151423D8(pitch);
        sinPitch = func_151423D8(pitch - 0x40);
        cosYaw = func_151423D8(yaw);
        sinYaw = func_151423D8(yaw - 0x40);
        speed = (func_150ADA68() * config->speedRange) + config->speedBase;
        spawn.itemCount = (func_150ADA20() % (config->itemCountRange + 1)) + config->itemCountBase;
        spawn.life = (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase;
        motion.halfWidth = (func_150ADA68() * config->sizeRange) + config->sizeBase;
        motion.gravity = (func_150ADA68() * config->growthRange) + config->growthBase;
        motion.velocity.x = (speed * cosPitch) * sinYaw;
        motion.velocity.y = -speed * sinPitch;
        motion.velocity.z = (speed * cosPitch) * cosYaw;
        trail_createMotionTrail(&spawn, &motion, 0, config->handlers[0], config->handlers[1], config->handlers[2], config->handlers[3],
                      config->handlers[4], config->handlers[5], 0, 0, &params, 0, arg1, 1);
        count--;
    }
}


/* Spawns a random number of particles (func_15167D84) at config->position, each moving at a random
 * yaw and pitch. Positions are integer units plus a 1/256 fraction. */
void func_15153298(BillboardBurst *config, u8 arg1, s32 arg2) {
    BillboardTemplate packet;
    f32 speed;
    f32 sinPitch;
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 horizontal;
    s16 yaw;
    s16 pitch;
    s32 count;

    packet.x = config->position.x;
    packet.y = config->position.y;
    packet.z = config->position.z;
    packet.subX = (u16)(s32)(config->position.x * 256.0f);
    packet.subZ = (u16)(s32)(config->position.y * 256.0f);
    packet.subY = (u16)(s32)(config->position.z * 256.0f);
    packet.primR = config->color;
    packet.primG = config->color;
    packet.primB = config->color;
    packet.envR = 0;
    packet.envG = 0;
    packet.envB = 0;
    packet.primA = config->alpha;
    packet.callback = config->callback;
    packet.flags = config->flags;
    packet.frame = 0;
    packet.frameStep = config->frameStep;
    packet.anim = config->anim;
    count = func_150ADA20() % (config->countRange + 1) + config->countBase;
    while (count != 0) {
        yaw = func_150ADA20() % (config->angles.yawRange + 1) + config->angles.yawBase;
        pitch = func_150ADA20() % (config->angles.pitchRange + 1) + config->angles.pitchBase;
        cosPitch = func_151423D8(pitch);
        sinPitch = func_151423D8(pitch - 64);
        cosYaw = func_151423D8(yaw);
        sinYaw = func_151423D8(yaw - 64);
        speed = func_150ADA68() * config->speedRange + config->speedBase;
        packet.life = func_150ADA20() % (config->lifeRange + 1) + config->lifeBase;
        horizontal = speed * cosPitch;
        packet.velX = horizontal * sinYaw;
        packet.velY = -speed * sinPitch;
        packet.velZ = horizontal * cosYaw;
        packet.accelY = func_150ADA20() % (config->accelYRange + 1) + config->accelYBase;
        packet.fadeStep = func_150ADA20() % (config->fadeRange + 1) + config->fadeBase;
        packet.width = func_150ADA20() % (config->widthRange + 1) + config->widthBase;
        packet.height = func_150ADA20() % (config->heightRange + 1) + config->heightBase;
        func_15167D84(&packet, 0, 0, -1, arg1, arg2);
        count--;
    }
}

/* Spawns a random number of particles (func_15130280) at config->position, each moving at a random
 * yaw and pitch, and copies config->extra into each. */
void func_15153634(SpriteBurst *config, u8 arg1, u8 arg2, s32 arg3) {
    ParticleDesc spawn;
    f32 extra;
    s32 count;
    s16 yaw;
    s16 pitch;
    f32 speed;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;
    f32 cosYaw;
    f32 horizontal;
    void *object;

    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    extra = config->extra;
    spawn.texture = config->texture;
    spawn.renderMode = config->renderMode;
    spawn.unk4 = config->unkC;
    spawn.unk0 = config->unk8;
    spawn.frame = 0;
    spawn.r = config->r;
    spawn.g = config->g;
    spawn.b = config->b;
    spawn.a = config->a;
    spawn.r2 = config->r2;
    spawn.g2 = config->g2;
    spawn.b2 = config->b2;
    spawn.pos = config->pos;
    spawn.flags = config->flags;
    spawn.brightness = config->brightness;
    spawn.updateFunc = config->updateFunc;
    spawn.deathFunc = config->deathFunc;
    spawn.fadeStart = config->fadeStart;
    spawn.fadeRate = config->fadeRate;
    spawn.primColorMode = config->primColorMode;
    spawn.envColorMode = config->envColorMode;
    spawn.scaleStart = config->scaleStart;
    spawn.scaleRate = config->scaleRate;
    spawn.views = arg1;
    while (count != 0) {
        yaw = (func_150ADA20() % (config->yawRange + 1)) + config->yawBase;
        pitch = (func_150ADA20() % (config->pitchRange + 1)) + config->pitchBase;
        cosPitch = func_151423D8(pitch);
        sinPitch = func_151423D8(pitch - 0x40);
        cosYaw = func_151423D8(yaw);
        sinYaw = func_151423D8(yaw - 0x40);
        speed = (func_150ADA68() * config->speedRange) + config->speedBase;
        spawn.timer = (func_150ADA20() % (config->timerRange + 1)) + config->timerBase;
        spawn.frameRate = (func_150ADA20() % (config->frameRateRange + 1)) + config->frameRateBase;
        horizontal = speed * cosPitch;
        spawn.width = spawn.height = (func_150ADA68() * config->sizeRange) + config->sizeBase;
        spawn.vel.x = horizontal * sinYaw;
        spawn.vel.y = -speed * sinPitch;
        spawn.vel.z = horizontal * cosYaw;
        spawn.gravity = (func_150ADA68() * config->gravityRange) + config->gravityBase;
        spawn.alpha = (func_150ADA20() % (config->alphaRange + 1)) + config->alphaBase;
        object = func_15130280(&spawn, config->unk64, 0, 4, arg2, arg3);
        if (object != NULL) {
            memcpy((u8 *)object + 0xA8, &extra, sizeof(extra));
        }
        count--;
    }
}

/* Spawns a random number of particles (func_15156190) at config->position, each pointing at a random
 * yaw and pitch (a direction of length 10). */
void func_151539B4(StreakBurst *config, u8 arg1) {
    StreakParticleDesc spawn;
    s32 count;
    s16 yaw;
    s16 pitch;
    f32 cosPitch;
    f32 sinPitch;
    f32 cosYaw;
    f32 horizontal;
    f32 sinYaw;

    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    spawn.pos = config->pos;
    spawn.headColor.red = config->headColor.red;
    spawn.headColor.green = config->headColor.green;
    spawn.headColor.blue = config->headColor.blue;
    spawn.headColor.alpha = config->headColor.alpha;
    spawn.tailColor.red = config->tailColor.red;
    spawn.tailColor.green = config->tailColor.green;
    spawn.tailColor.blue = config->tailColor.blue;
    spawn.tailColor.alpha = config->tailColor.alpha;
    spawn.flags = config->flags;
    spawn.renderMode = config->renderMode;
    spawn.fadeStart = config->fadeStart;
    spawn.fadeRate = config->fadeRate;
    while (count != 0) {
        yaw = (func_150ADA20() % (config->yawRange + 1)) + config->yawBase;
        pitch = (func_150ADA20() % (config->pitchRange + 1)) + config->pitchBase;
        cosPitch = func_151423D8(pitch);
        sinPitch = func_151423D8(pitch - 0x40);
        cosYaw = func_151423D8(yaw);
        horizontal = 10.0f * cosPitch;
        sinYaw = func_151423D8(yaw - 0x40);
        spawn.dir.x = horizontal * sinYaw;
        spawn.dir.y = -10.0f * sinPitch;
        spawn.dir.z = horizontal * cosYaw;
        spawn.headSpeed = (func_150ADA68() * config->headSpeedRange) + config->headSpeedBase;
        spawn.tailSpeed = (func_150ADA68() * config->tailSpeedRange) + config->tailSpeedBase;
        spawn.lifetime = (func_150ADA20() % (config->lifetimeRange + 1)) + config->lifetimeBase;
        spawn.width = (func_150ADA68() * config->widthRange) + config->widthBase;
        spawn.alpha = (func_150ADA20() % (config->alphaRange + 1)) + config->alphaBase;
        func_15156190(&spawn, config->kind, 0, arg1, 0);
        count--;
    }
}

void func_15153C84(TriangleSource *source, SplashSpray *config, CollisionHit *ground, u8 arg3, s32 arg4) {
    ConeBasis cone;

    func_1514F640(source, &cone);
    func_15153CCC(&cone, config, ground, arg3, arg4);
}

/* Spawns a random number of particles (func_151DA6F8), each moving in a random direction inside the
 * cone. */
void func_15153CCC(ConeBasis *cone, SplashSpray *config, CollisionHit *ground, u8 arg3, s32 arg4) {
    s32 count;
    Vec3f velocity;

    if (func_1514F6E8(cone) != 0) {
        count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
        while (count != 0) {
            func_1514F808(cone, (func_150ADA68() * config->speedRange) + config->speedBase, &velocity);
            func_151DA6F8(&config->pos, &velocity,
                          (func_150ADA68() * config->sizeRange) + config->sizeBase,
                          (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase,
                          (func_150ADA20() % (config->unk32 + 1)) + config->unk30,
                          (func_150ADA68() * config->scaleRange) + config->scaleBase,
                          (func_150ADA20() % (config->itemCountRange + 1)) + config->itemCountBase,
                          func_150ADA68() < config->unk38, 1.7f, 1.7f, 1, config->kind, ground,
                          config->unk3C, config->unk3E, config->unk40, arg3, arg4);
            count--;
        }
    }
}

/* Like func_15153CCC, but each direction is a random yaw/pitch from `angles`. */
void func_15153F18(AngleRange *angles, SplashSpray *config, CollisionHit *ground, u8 arg3, s32 arg4) {
    s32 count;
    Vec3f velocity;

    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    while (count != 0) {
        func_15143794((func_150ADA20() % (angles->yawRange + 1)) + angles->yawBase,
                      (func_150ADA20() % (angles->pitchRange + 1)) + angles->pitchBase,
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &velocity);
        func_151DA6F8(&config->pos, &velocity,
                      (func_150ADA68() * config->sizeRange) + config->sizeBase,
                      (func_150ADA20() % (config->lifeRange + 1)) + config->lifeBase,
                      (func_150ADA20() % (config->unk32 + 1)) + config->unk30,
                      (func_150ADA68() * config->scaleRange) + config->scaleBase,
                      (func_150ADA20() % (config->itemCountRange + 1)) + config->itemCountBase,
                      func_150ADA68() < config->unk38, 1.7f, 1.7f, 1, config->kind, ground,
                      config->unk3C, config->unk3E, config->unk40, arg3, arg4);
        count--;
    }
}

/* Spawns a single object at `position` that grows and fades out over `duration`. With a nonzero arg4
 * it gets no random x/z rotation, spins only around y and grows along one axis. */
void *func_151541B8(const Vec3f *position, f32 duration, f32 growth, f32 alpha, f32 arg4, u8 arg5, s32 arg6) {
    void *object;
    s32 oneAxis;
    ParticleSpawn78 spawn;
    Extra1515452C extra;
    s32 pad[2]; // matching: unused stack slots

    if (duration <= 0.0f) {
        return NULL;
    }

    extra.elapsed = 0.0f;
    extra.duration = duration;
    extra.growth = growth;
    extra.alpha = alpha;
    extra.fade = alpha / (duration * duration);
    if (arg4 != 0) {
        oneAxis = TRUE;
    } else {
        oneAxis = FALSE;
    }
    extra.growOneAxis = oneAxis;

    spawn.sizeXZ = 0.0f;
    spawn.sizeY = arg4;
    spawn.radius = 1.0f;
    spawn.elasticity = 1.0f;
    if (arg4 != 0) {
        spawn.rot.x = 0.0f;
    } else {
        spawn.rot.x = func_150ADA68() * 360.0f;
    }
    spawn.rot.y = func_150ADA68() * 360.0f;
    if (arg4 != 0) {
        spawn.rot.z = 0.0f;
    } else {
        spawn.rot.z = func_150ADA68() * 360.0f;
    }
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos = *position;
    spawn.vel.x = 0.0f;
    spawn.vel.y = 0.0f;
    spawn.vel.z = 0.0f;
    if (arg4 != 0) {
        spawn.rotVel.x = 0.0f;
    } else {
        spawn.rotVel.x = (func_150ADA68() * 8.512000084f) + -4.256000042f;
    }
    if (arg4 != 0) {
        spawn.rotVel.y = (func_150ADA68() * 8.512000084f) + -4.256000042f;
    } else {
        spawn.rotVel.y = 0.0f;
    }
    if (arg4 != 0) {
        spawn.rotVel.z = 0.0f;
    } else {
        spawn.rotVel.z = (func_150ADA68() * 8.512000084f) + -4.256000042f;
    }
    spawn.gravity = 0.0f;
    spawn.flags = 0x100140;
    spawn.model = 84;
    spawn.variant = 0;
    spawn.unk5C = 0;
    spawn.alpha = alpha;
    spawn.updateFn = 15;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 2;
    spawn.unk6A = 0;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    spawn.lifetime = 300;

    object = func_15132A4C(&spawn, 0, 0, sizeof(extra), arg5, arg6);
    if (object != NULL) {
        memcpy((u8 *)object + 0x170, &extra, sizeof(extra));
    }
    return object;
}

/* Per-frame update for func_151541B8's objects: grow, fade, move; returns 0 once expired. */
s32 func_1515452C(Object15132A4C *object) {
    f32 size;
    Extra1515452C *extra;

    extra = &object->extra.grow;
    size = extra->growth * sqrtf(extra->elapsed);
    object->unk18 = size;
    if (!(extra->growOneAxis & 1)) {
        object->unk1C = size;
    }
    {
        f32 elapsed = extra->elapsed;
        object->alpha = (s8)(u32)(extra->alpha - (extra->fade * elapsed * elapsed));
        extra->elapsed += gFrameDelta;
        if (extra->duration < extra->elapsed) {
            return 0;
        }
        object->position.x += object->velocity.x * gFrameDelta;
        object->position.y += object->velocity.y * gFrameDelta;
        object->position.z += object->velocity.z * gFrameDelta;
        return 1;
    }
}

/* Spawns a random number of particles (func_151C5F44), each moving at a random yaw and a pitch from
 * config's range. */
void func_15154684(ParticleBurst *config, u8 arg1, s32 arg2) {
    f32 pad; // matching: unused stack slot above velocity
    Vec3f velocity;
    s32 count;

    count = (func_150ADA20() % (config->countRange + 1)) + config->countBase;
    while (count > 0) {
        func_15143794(func_150ADA20() & 0xFF,
                      (func_150ADA20() % (config->pitchRange + 1)) + config->pitchBase,
                      (func_150ADA68() * config->speedRange) + config->speedBase,
                      &velocity);
        func_151C5F44(config->pos, &velocity,
                      (func_150ADA68() * config->gravityRange) + config->gravityBase,
                      config->unk20,
                      (func_150ADA20() % (config->alphaRange + 1)) + config->alphaBase,
                      (func_150ADA20() % (config->timerRange + 1)) + config->timerBase,
                      (func_150ADA68() * config->sizeRange) + config->sizeBase,
                      func_150ADA68() < config->altChance,
                      config->ground, arg1, arg2);
        count--;
    }
}

/* Spawns a single object at `position` whose size and alpha swell and shrink along half a sine wave
 * over `duration`. */
void *func_15154884(Vec3f *position, f32 duration, f32 size, f32 alpha, u8 arg4, s32 arg5) {
    void *object;
    ParticleSpawn spawn;
    Extra15154A88 extra;

    if (duration <= 0.0f) {
        return NULL;
    }

    extra.remaining = duration;
    extra.size = size;
    extra.phase = 0.0f;
    spawn.sizeXZ = 0.0f;
    spawn.sizeY = 0.0f;
    spawn.radius = 1.0f;
    spawn.elasticity = 1.0f;
    extra.alpha = alpha;
    extra.phaseRate = 3.141592741f / duration;
    spawn.rot.x = func_150ADA68() * 360.0f;
    spawn.rot.y = func_150ADA68() * 360.0f;
    spawn.rot.z = func_150ADA68() * 360.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    spawn.pos = *position;
    spawn.vel.x = 0.0f;
    spawn.vel.y = 0.0f;
    spawn.vel.z = 0.0f;
    spawn.rotVel.x = (func_150ADA68() * 26.29600143f) + -13.14800072f;
    spawn.rotVel.y = 0.0f;
    spawn.rotVel.z = (func_150ADA68() * 26.29600143f) + -13.14800072f;
    spawn.gravity = 0.0f;
    spawn.flags = 0x140;
    spawn.model = 85;
    spawn.variant = 0;
    spawn.unk5C = 0;
    spawn.alpha = 0xFF;
    spawn.updateFn = 18;
    spawn.update2Fn = 0;
    spawn.floorHitFn = 0;
    spawn.floorType3Fn = 0;
    spawn.wallHitFn = 0;
    spawn.destroyFn = 0;
    spawn.squashFn = 0;
    spawn.matrixFn = 2;
    spawn.unk6A = 0;
    spawn.owner = 0;
    spawn.ownerId = 0;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    spawn.lifetime = 300;

    object = func_15132A4C(&spawn, 0, 0, sizeof(extra), arg4, arg5);
    if (object != NULL) {
        memcpy((u8 *)object + 0x170, &extra, sizeof(extra));
    }
    return object;
}

/* Per-frame update for func_15154884's objects: pulse size and alpha, move; returns 0 once expired. */
s32 func_15154A88(Object15132A4C *object) {
    Extra15154A88 *extra;
    f32 wave;
    f32 size;

    extra = &object->extra.pulse;
    wave = sinf(extra->phase);
    size = extra->size * wave;
    object->unk1C = size;
    object->unk18 = size;
    object->alpha = (f32)(u8)(u32)extra->alpha * wave;
    extra->remaining -= gFrameDelta;
    if (extra->remaining <= 0.0f) {
        return 0;
    }

    extra->phase += extra->phaseRate * gFrameDelta;
    object->position.x += object->velocity.x * gFrameDelta;
    object->position.y += object->velocity.y * gFrameDelta;
    object->position.z += object->velocity.z * gFrameDelta;
    return 1;
}
