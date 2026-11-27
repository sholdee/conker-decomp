#include <ultra64.h>

#include "effects.h"
#include <math.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/zone.h"
#include "libc/string.h"

/* The area effects of level_spawnZoneAreaEffect (game/Level/ZoneSpawn/zonespawn.c), indexed by the zone's data1. */
const Struct150139ACEntry gAreaEffectDefs[6] = {
    { 6.50000004e-07f, 2.180000067f, 1.230000019f, 1.610000014f, 4.829999924f,
      252, 50,
      0.3999999762f, 0.09999999404f, 0.01620000042f, 0.005299999844f, 0.8600000143f, 0.5940000415f,
      1, 0xFF, 0, 0, 0 },
    { 3.499999934e-07f, 1.419999957f, 0.5500000119f, 1.639999986f, 2.629999876f,
      252, 50,
      0.5999999642f, 0.1799999923f, 0.02630000003f, 0.01189999934f, 0.4650000334f, 0.1790000051f,
      1, 0xFF, 0, 0, 0 },
    { 3.499999934e-07f, 1.419999957f, 0.5500000119f, 1.639999986f, 2.629999876f,
      252, 50,
      0.5999999642f, 0.1799999923f, 0.02630000003f, 0.01189999934f, 0.4650000334f, 0.1790000051f,
      1, 0xFF, 0, 0, 0 },
    { 2.499999937e-05f, 0.8999999762f, 0.5999999642f, 0.8999999762f, 0.5999999642f,
      40, 110,
      0.25f, 0.25f, 0.01549999975f, 0.009999999776f, 0.200000003f, 0.1030000076f,
      0, 0x64, 0x64, 0, 0 },
    { 8.510000043e-05f, 0.6999999881f, 0.5f, 1.149999976f, 0.7400000095f,
      15, 30,
      0.1499999911f, 0.2399999946f, 0.03350000083f, 0.0223999992f, 0.0530000031f, 0.1920000017f,
      1, 0x77, 0x76, 0, 1 },
    { 5.149999924e-05f, 0.2800000012f, 0.2599999905f, 0.2199999988f, 0.2599999905f,
      10, 40,
      0.07999999821f, 0.1999999881f, 0.05900000036f, 0.009200000204f, 0.4010000229f, 0.3050000072f,
      1, 0x4E, 0x93, 0, 1 },
};

/* Pseudo-random value mixed from the CPU cycle counter and math_random. */
#define RANDOM() (osGetCount() * math_random())
/* Low 16 bits of RANDOM(); multiply by 1/65535 for a fraction in [0, 1]. */
#define RANDOM_U16() (RANDOM() & 0xFFFF)

/* Clears gParticleModelUseCounts[] and the counters that go with it. */
void particle_resetModelList(void) {
    s32 i;

    for (i = 0; i < 233; i++) {
        gParticleModelUseCounts[i] = 0;
    }

    gParticleModelsHead = NULL;
    gParticleModelsTail = NULL;
    gModelParticleCount = 0;
}

/*
 * Spawns math_getZoneVolume(region) * density * 2e-6 particles at random points of the region,
 * each with a random type from a fixed list.
 */
void particle_spawnAreaParticles(TriggerZone *region, f32 density) {
    f32 count;

    count = math_getZoneVolume(region) * density * 1.999999995e-06f;
    if (count > 1.0f) {
        f32 inv65535;
        ParticleSpawn part;
        f32 top;
        f32 bottom;

        part.radius = 1.0f;
        part.elasticity = 1.0f;
        part.scale.x = 1.0f;
        part.scale.y = 1.0f;
        part.scale.z = 1.0f;
        part.flags = 0x5D40;
        part.lifetime = 300;
        part.variant = 0;
        part.alpha = 0xFF;
        part.updateFn = 14;
        part.update2Fn = 0;
        part.floorHitFn = 0;
        part.floorType3Fn = 0;
        part.wallHitFn = 0;
        part.destroyFn = 0;
        part.squashFn = 0;
        part.matrixFn = 2;
        part.lightMode = 0;
        part.owner = 0;
        part.ownerId = 0;
        part.fadeStart = 1;
        part.fadeRate = 0xFF;
        inv65535 = 1.525902189e-05f;
        part.rotVel.y = 0.0f;
        do {
            s32 pad2; // matching: stack slot
            const u16 types[9] = { 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4E, 0x4F, 0x50, 0x51 };
            f32 range; // matching: reused for the distance to the nearer bound (frame size)
            f32 y;
            f32 below;

            zone_getRandomPoint(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.vel.y = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                below = fabsf(y - bottom);
                range = (below < above) ? below : above;
                part.gravity = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.vel.x = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.vel.z = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            part.rot.x = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.y = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.z = RANDOM_U16() * inv65535 * 360.0f;
            part.rotVel.x = RANDOM_U16() * inv65535 * 1.600000024f;
            part.rotVel.z = RANDOM_U16() * inv65535 * 1.600000024f;
            part.model = types[RANDOM() % 9];
            if (part.model == 0x4B || part.model == 0x48 || part.model == 0x4E || part.model == 0x4F ||
                part.model == 0x50 || part.model == 0x51) {
                part.sizeXZ = part.sizeY = RANDOM_U16() * inv65535 * 0.25f + 0.5f;
            } else {
                part.sizeXZ = part.sizeY = RANDOM_U16() * inv65535 * 0.25f + 1.0f;
            }
            collision_selectMap(0);
            part.zone = collision_findZoneLeaf(part.pos.x, part.pos.z);
            particle_newModelParticle(&part, 3, 0xFF, 0, 0xFF, 1);
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* Spawns math_getZoneVolume(region) * density * 2e-6 particle_newSpriteParticleWithFloor particles of random size. */
void particle_spawnAreaSprites(TriggerZone *region, f32 density) {
    f32 count;

    count = math_getZoneVolume(region) * density * 1.999999995e-06f;
    if (count > 1.0f) {
        f32 minFrac;
        f32 fracRange;
        f32 inv65535;
        ParticleDesc68 part;
        SteamWobble extra;
        DescParticleHead *obj;
        f32 top;
        f32 bottom;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        f32 temp; // random size, then distance to the bottom
        s32 pad2; // matching: stack slots
        s32 pad3;

        minFrac = 0.200000003f;
        part.renderMode = 0x4403;
        part.setGeometryMode = 0x200005;
        part.a = 0xFF;
        fracRange = 0.25f;
        part.clearGeometryMode = 0;
        part.frame = 0;
        part.frameRate = 0;
        part.r = 0;
        part.g = 0;
        part.b = 0;
        part.r2 = 0;
        part.g2 = 0;
        part.b2 = 0;
        part.brightness = 0xFF;
        part.offset.x = 0.0f;
        part.offset.y = 0.0f;
        part.offset.z = 0.0f;
        part.scaleStart = 1;
        part.scaleRate = 1.0f;
        part.timer = 300;
        part.fadeStart = 1;
        part.fadeRate = 0xFF;
        part.primColorMode = 6;
        part.envColorMode = 8;
        part.updateFunc = 23;
        part.deathFunc = -1;
        part.drawFunc = -1;
        part.handlerSet = 0;
        extra.phaseX = 0;
        extra.phaseY = 0;
        extra.burstScale = 1.0f;
        extra.altBurstScale = 1.0f;
        inv65535 = 1.525902189e-05f;
        do {
            s32 pad4; // matching: stack slot
            s32 flag40;

            temp = RANDOM_U16() * inv65535 * 350.0f + 50.0f;
            if (math_random() & 1) {
                part.texture = 0x58;
                extra.size = part.width = part.height = temp;
            } else {
                part.texture = 0x40;
                y = temp * 0.6999999881f; // matching: y's register doubles as the smaller size
                part.height = y;
                part.width = y;
                extra.size = y;
            }
            part.alpha = RANDOM() % 101 + 50;
            extra.stepX = (RANDOM() & 3) + 2;
            extra.stepY = (RANDOM() & 3) + 2;
            extra.amplitudeX = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            extra.amplitudeY = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            // matching: a separate statement makes IDO call math_random for 0x40 first
            flag40 = (math_random() & 1) ? 0x40 : 0;
            part.flags = flag40 | ((math_random() & 1) ? 0x80 : 0) | 0x4000 | 0x8000 | 0x10000 | 0x20000;
            zone_getRandomPoint(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.vel.y = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                temp = fabsf(y - bottom);
                range = (temp < above) ? temp : above;
                part.gravity = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.vel.x = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.vel.z = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            collision_selectMap(0);
            part.viewMask = collision_findZoneLeaf(part.pos.x, part.pos.z);
            obj = particle_newSpriteParticleWithFloor(&part, 2, 0, 28, 255, 1);
            if (obj != NULL) {
                memcpy(obj + 1, &extra, sizeof(extra));
            }
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* Spawns math_getZoneVolume(region) * density * 2e-7 particle_newQuadParticle particles using gWhiteVtxColors. */
void particle_spawnAreaQuads(TriggerZone *region, f32 density) {
    f32 count;
    s32 pad; // matching: stack slot

    count = math_getZoneVolume(region) * density * 2.000000023e-07f;
    if (count > 1.0f) {
        f32 inv65535;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        ParticleDef4C part;
        QuadBob extra;
        f32 top;
        f32 bottom;
        f32 temp;
        DefParticleHead *obj;
        s32 pad2; // matching: stack slots
        s32 pad3;

        part.texture = 0x57;
        part.type = 0;
        part.renderModes = 0x3B03;
        part.lifetime = 300;
        part.frame = 0;
        part.frameSpeed = 0;
        part.rgba[3] = 0xFF;
        part.scale.x = 0.0f;
        part.scale.y = 0.0f;
        part.scale.z = 0.0f;
        part.flags = 0x037C0000;
        part.colorLevel = 0xFF;
        part.primMode = 0;
        part.envMode = 6;
        extra.rotVel.y = 0.0f;
        part.rgba[0] = 0xFF;
        part.rgba[1] = 0xFF;
        part.rgba[2] = 0xFF;
        inv65535 = 1.525902189e-05f;
        do {
            temp = RANDOM_U16() * inv65535 * 12.0f + 10.0f;
            part.width = temp;
            part.height = temp;
            zone_getRandomPoint(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            extra.center = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                temp = fabsf(y - bottom);
                range = (temp < above) ? temp : above;
                extra.amplitude = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            extra.phase = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            extra.speed = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            extra.rotVel.x = RANDOM_U16() * inv65535;
            extra.rotVel.z = RANDOM_U16() * inv65535;
            part.rot.x = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.y = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.z = RANDOM_U16() * inv65535 * 360.0f;
            part.alpha = RANDOM() % 156 + 100;
            collision_selectMap(0);
            part.zone = collision_findZoneLeaf(part.pos.x, part.pos.z);
            obj = particle_newQuadParticle(&part, &gWhiteVtxColors, 0x23, 0, 0, 0x1E, 0, 3, 0xFF, 0x1C, 0xFF, 1);
            if (obj != NULL) {
                memcpy(obj + 1, &extra, sizeof(extra));
            }
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* Like particle_spawnAreaParticles, at a tenth of the density and always with particle type 0x53. */
void func_1500D3AC(TriggerZone *region, f32 density) {
    f32 count;

    count = math_getZoneVolume(region) * density * 2.000000023e-07f;
    if (count > 1.0f) {
        f32 inv65535;
        ParticleSpawn part;
        f32 top;
        f32 bottom;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        f32 below;

        part.radius = 1.0f;
        part.elasticity = 1.0f;
        part.scale.x = 1.0f;
        part.scale.y = 1.0f;
        part.scale.z = 1.0f;
        part.flags = 0x4D40;
        part.lifetime = 300;
        part.variant = 0;
        part.alpha = 0xFF;
        part.updateFn = 14;
        part.update2Fn = 0;
        part.floorHitFn = 0;
        part.floorType3Fn = 0;
        part.wallHitFn = 0;
        part.destroyFn = 0;
        part.squashFn = 0;
        part.matrixFn = 2;
        part.lightMode = 0;
        part.owner = 0;
        part.ownerId = 0;
        part.fadeStart = 1;
        part.fadeRate = 0xFF;
        part.model = 0x53;
        inv65535 = 1.525902189e-05f;
        part.rotVel.y = 0.0f;
        do {
            zone_getRandomPoint(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.vel.y = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                below = fabsf(y - bottom);
                range = (below < above) ? below : above;
                part.gravity = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.vel.x = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.vel.z = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            part.rot.x = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.y = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.z = RANDOM_U16() * inv65535 * 360.0f;
            part.rotVel.x = RANDOM_U16() * inv65535 * 1.600000024f;
            part.rotVel.z = RANDOM_U16() * inv65535 * 1.600000024f;
            part.sizeXZ = part.sizeY = RANDOM_U16() * inv65535 * 0.25f + 1.0f;
            collision_selectMap(0);
            part.zone = collision_findZoneLeaf(part.pos.x, part.pos.z);
            particle_newModelParticle(&part, 3, 0xFF, 0, 0xFF, 1);
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* Like particle_spawnAreaParticles with another particle type list; type 0x39 is drawn at 35% scale. */
void func_1500D8C8(TriggerZone *region, f32 density) {
    f32 count;

    count = math_getZoneVolume(region) * density * 1.999999995e-06f;
    if (count > 1.0f) {
        f32 inv65535;
        ParticleSpawn part;
        f32 top;
        f32 bottom;

        part.radius = 1.0f;
        part.elasticity = 1.0f;
        part.scale.x = 1.0f;
        part.scale.y = 1.0f;
        part.scale.z = 1.0f;
        part.flags = 0x4D40;
        part.lifetime = 300;
        part.variant = 0;
        part.alpha = 0xFF;
        part.updateFn = 14;
        part.update2Fn = 0;
        part.floorHitFn = 0;
        part.floorType3Fn = 0;
        part.wallHitFn = 0;
        part.destroyFn = 0;
        part.squashFn = 0;
        part.matrixFn = 2;
        part.lightMode = 0;
        part.owner = 0;
        part.ownerId = 0;
        part.fadeStart = 1;
        part.fadeRate = 0xFF;
        inv65535 = 1.525902189e-05f;
        part.rotVel.y = 0.0f;
        do {
            s32 pad2; // matching: stack slot
            const u16 types[14] = { 0x76, 0x77, 0x78, 0x79, 0x53, 0x3E, 0x3D, 0x39, 0x02, 0x03, 0x4E, 0x4F, 0x50, 0x51 };
            f32 range; // matching: reused for the distance to the nearer bound
            f32 y;
            f32 below;

            zone_getRandomPoint(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.vel.y = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                below = fabsf(y - bottom);
                range = (below < above) ? below : above;
                part.gravity = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.vel.x = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.vel.z = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            part.rot.x = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.y = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.z = RANDOM_U16() * inv65535 * 360.0f;
            part.rotVel.x = RANDOM_U16() * inv65535 * 1.600000024f;
            part.rotVel.z = RANDOM_U16() * inv65535 * 1.600000024f;
            part.model = types[RANDOM() % 14];
            part.sizeXZ = part.sizeY = RANDOM_U16() * inv65535 * 0.25f + 0.5f;
            if (part.model == 0x39) {
                part.sizeXZ *= 0.349999994f;
                part.sizeY *= 0.349999994f;
            }
            collision_selectMap(0);
            part.zone = collision_findZoneLeaf(part.pos.x, part.pos.z);
            particle_newModelParticle(&part, 3, 0xFF, 0, 0xFF, 1);
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* Like particle_spawnAreaSprites with larger particle_newSpriteParticleWithFloor particles and random colour bytes. */
void particle_spawnAreaLargeSprites(TriggerZone *region, f32 density) {
    f32 count;

    count = math_getZoneVolume(region) * density * 1.999999995e-06f;
    if (count > 1.0f) {
        f32 minFrac;
        f32 fracRange;
        f32 inv65535;
        ParticleDesc68 part;
        SteamWobble extra;
        DescParticleHead *obj;
        f32 top;
        f32 bottom;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        f32 temp; // random size, then distance to the bottom

        minFrac = 0.3000000119f;
        part.renderMode = 0x5103;
        part.setGeometryMode = 0x200005;
        part.a = 0xFF;
        part.brightness = 0xFF;
        fracRange = 0.150000006f;
        part.clearGeometryMode = 0;
        part.frame = 0;
        part.frameRate = 0;
        part.g = 0;
        part.b = 0;
        part.g2 = 0;
        part.b2 = 0;
        part.offset.x = 0.0f;
        part.offset.y = 0.0f;
        part.offset.z = 0.0f;
        part.scaleStart = 1;
        part.scaleRate = 1.0f;
        part.timer = 300;
        part.fadeStart = 1;
        part.fadeRate = 0xFF;
        part.primColorMode = 8;
        part.envColorMode = 6;
        part.updateFunc = 23;
        part.deathFunc = -1;
        part.drawFunc = -1;
        part.handlerSet = 0;
        extra.phaseX = 0;
        extra.phaseY = 0;
        extra.burstScale = 1.0f;
        extra.altBurstScale = 1.0f;
        part.texture = 108;
        inv65535 = 1.525902189e-05f;
        do {
            temp = RANDOM_U16() * inv65535 * 600.0f + 200.0f;
            part.height = temp;
            part.width = temp;
            extra.size = temp;
            part.alpha = RANDOM() % 71 + 50;
            extra.stepX = RANDOM() % 3;
            extra.stepY = RANDOM() % 3;
            extra.amplitudeX = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            extra.amplitudeY = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            part.flags = ((math_random() & 1) ? 0x40 : 0) | ((math_random() & 1) ? 0x80 : 0) | 0x4000 |
                         0x8000 | 0x20000 | 0x800000;
            part.r2 = RANDOM() % 56 + 80;
            part.r = RANDOM() % 46 + 180;
            zone_getRandomPoint(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.vel.y = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                temp = fabsf(y - bottom);
                range = (temp < above) ? temp : above;
                part.gravity = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.vel.x = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.vel.z = RANDOM_U16() * inv65535 * 0.01000000071f + 0.002000000095f;
            collision_selectMap(0);
            part.viewMask = collision_findZoneLeaf(part.pos.x, part.pos.z);
            obj = particle_newSpriteParticleWithFloor(&part, 2, 0, 28, 255, 1);
            if (obj != NULL) {
                memcpy(obj + 1, &extra, sizeof(extra));
            }
            count -= 1.0f;
        } while (count > 1.0f);
    }
}
