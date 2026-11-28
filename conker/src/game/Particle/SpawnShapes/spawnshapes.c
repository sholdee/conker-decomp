#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"
#include "conker/pipeexplode.h"
#include "conker/trail.h"

/* The particle spawn callback of each kind (EmitterCallback; the callbacks take their own parameter types,
 * so most entries give IDO warning 709, as in the other handler tables). */
EmitterCallback gParticleKindCallbacks[28] = {
    effect_emitStompPuff, func_150C2558, trail_spawnKind2Trail,
    func_150C2700, particle_spawnFastSpark, effect_spawnJointSpark,
    effect_spawnJointPiece, particle_spawnYawStreak, effect_spawnPitchYawStreak,
    pipeexplode_spawnChunk, pipeexplode_spawnTunableChunk, effect_spawnImpactSprayParticle,
    particle_spawnYawSteam, effect_spawnSegmentTrail, particle_spawnYawSpark,
    particle_spawnYawModelParticle, particle_spawnYawSpraySpark, particle_spawnYawDampedSprite,
    particle_spawnYawTumbler, effect_spawnDampedParticle, effect_spawnTumblingBurst,
    effect_spawnPitchYawSplash, effect_spawnSmallYawColorSprite, particle_spawnYawColorSprite,
    particle_spawnYawSpinModel, effect_spawnBreakUpGroundParticle, NULL,
    NULL,
};

/*
 * Spawns `count` particles of `kind` evenly spaced on a horizontal circle of `radius`
 * around (x, y, z), starting at `startAngle` and stepping by `angleStep` (256 = full turn).
 * Stops early if the kind's callback returns 0.
 */
void particle_emitOnCircle(f32 x, f32 y, f32 z, f32 radius, u8 startAngle, s8 angleStep, s16 count,
                   u8 kind, s32 arg8, f32 arg9, void *data, u8 owner) {
    f32 dirX;
    f32 dirZ;
    f32 px;
    f32 py;
    f32 pz;
    EmitterCallback callback;
    u8 angle = startAngle;
    s16 i;

    for (i = 0; i < count; i++, angle += angleStep) {
        py = y;
        dirX = math_cosBinAngleMasked(angle - 0x40);
        dirZ = math_cosBinAngleMasked(angle);
        px = radius * dirX + x;
        pz = radius * dirZ + z;
        callback = gParticleKindCallbacks[kind];
        if (callback != NULL &&
            callback(1, i, px, py, pz, x, y, z, angle, startAngle, radius, arg8, arg9, data,
                     owner) == 0) {
            return;
        }
    }
}

/*
 * Spawns particles of `kind` along the line from (x0, y0, z0) to (x1, y1, z1), `count`
 * points including both ends. Stops early if the kind's callback returns 0.
 */
void particle_emitOnLine(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 count, u8 kind,
                   s32 arg8, f32 arg9, void *data, u8 owner) {
    f32 px;
    f32 py;
    f32 pz;
    Vec3f step;
    s16 i;
    f32 inverse;

    i = 0;
    if (count < 2.0f) {
        return;
    }

    // matching: the original tests the parity even though both branches are identical
    if ((s32)count & 1) {
        inverse = 1.0f / (count - 1.0f);
        step.x = (x1 - x0) * inverse;
        step.y = (y1 - y0) * inverse;
        step.z = (z1 - z0) * inverse;
    } else {
        inverse = 1.0f / (count - 1.0f);
        step.x = (x1 - x0) * inverse;
        step.y = (y1 - y0) * inverse;
        step.z = (z1 - z0) * inverse;
    }
    px = x0;
    py = y0;
    pz = z0;
    do {
        if (gParticleKindCallbacks[kind] != NULL &&
            gParticleKindCallbacks[kind](0, i, px, py, pz, x0, y0, z0, 0, 0, 0.0f, arg8, arg9, data,
                             owner) == 0) {
            return;
        }
        count -= 1.0f;
        px += step.x;
        i++;
        py += step.y;
        pz += step.z;
    } while (count > 0.0f);
}

/*
 * Spawns `count` particles of `kind` on a horizontal circle of `radius` around (x, y, z),
 * each at a random angle between `angleA` and `angleB` inclusive.
 * Stops early if the kind's callback returns 0.
 */
void particle_emitOnRandomCircle(f32 x, f32 y, f32 z, f32 radius, s16 angleA, s16 angleB, s16 count, u8 kind,
                   s32 arg8, f32 arg9, void *data, u8 owner) {
    f32 dirX;
    f32 dirZ;
    f32 px;
    f32 py;
    f32 pz;
    s16 range;
    s16 low;
    EmitterCallback callback;
    u8 angle;
    s16 i;

    if (angleB < angleA) {
        range = angleA - angleB + 1;
        low = angleB;
    } else {
        range = angleB - angleA + 1;
        low = angleA;
    }
    for (i = 0; i < count; i++) {
        py = y;
        angle = math_random() % range + low;
        dirX = math_cosBinAngleMasked(angle - 0x40);
        dirZ = math_cosBinAngleMasked(angle);
        px = radius * dirX + x;
        pz = radius * dirZ + z;
        callback = gParticleKindCallbacks[kind];
        if (callback != NULL &&
            callback(2, i, px, py, pz, x, y, z, angle, angleA, radius, arg8, arg9, data,
                     owner) == 0) {
            return;
        }
    }
}

/*
 * Spawns `count` particles of `kind` on a vertical arc of `radius` around (x, y, z) in the
 * plane facing `yaw`, each at a random elevation between `angleA` and `angleB` inclusive.
 * Stops early if the kind's callback returns 0.
 */
void particle_emitOnRandomArc(f32 x, f32 y, f32 z, f32 radius, s16 yaw, s16 angleA, s16 angleB, s16 count,
                   u8 kind, s32 arg9, f32 arg10, void *data, u8 owner) {
    f32 yawX;
    f32 yawZ;
    f32 elevY;
    f32 elevH;
    f32 px;
    f32 py;
    f32 pz;
    s16 range;
    s16 low;
    u8 angle;
    s16 i;

    yawX = math_cosBinAngleMasked(yaw - 0x40);
    yawZ = math_cosBinAngleMasked(yaw);
    if (angleB < angleA) {
        range = angleA - angleB + 1;
        low = angleB;
    } else {
        range = angleB - angleA + 1;
        low = angleA;
    }
    for (i = 0; i < count; i++) {
        angle = math_random() % range + low;
        elevY = math_cosBinAngleMasked(angle - 0x40);
        elevH = math_cosBinAngleMasked(angle);
        px = radius * yawX * elevH + x;
        py = y - radius * elevY;
        pz = radius * yawZ * elevH + z;
        if (gParticleKindCallbacks[kind] != NULL &&
            gParticleKindCallbacks[kind](3, i, px, py, pz, x, y, z, angle, yaw, radius, arg9, arg10, data,
                             owner) == 0) {
            return;
        }
    }
}
