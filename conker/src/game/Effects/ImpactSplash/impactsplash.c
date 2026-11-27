#include <ultra64.h>
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/particle.h"

/* matching: no header declares particle_spawnImpactMark (emissionsource.c): followeffects.c and sightbeam.c were compiled with other views */
void particle_spawnImpactMark(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, u8 owner, s32 kind);

/* Spawns an impact burst at pos: a ground splash when there is floor just below, effects, an optional variant-specific effect pair and particles. */
void effect_spawnImpactBlast(Actor *owner, Actor *obj, Vec3f *pos, u8 variant, s32 arg4, u8 fxOwner, s32 fxList) {
    Vec3f impact;
    u8 hitGround;
    Vec3f top;
    CollisionHit hit;
    s32 effectA;
    s32 effectB;

    if (owner == NULL) {
        return;
    }

    hitGround = FALSE;
    particle_spawnLightFlash(pos, fxOwner, fxList);

    top.x = pos->x;
    top.y = pos->y + 100.0f;
    top.z = pos->z;
    collision_initHitFromObject(&hit, obj);

    if (collision_probeFloorSurfABSet0Cached(&top, 0, pos->y - 100.0f, &hit) && (hit.flags & 1)) {
        impact.x = top.x;
        hitGround = TRUE;
        impact.y = hit.height;
        impact.z = top.z;
        particle_spawnImpactMark(math_randomFloat() * 125.0f + 204.0f, math_random() % 101U + 155, &hit.tri, &impact,
                      math_random() % 302U + 500, 0, 1, 0, 0, 0, fxOwner, 0);
    }

    particle_startRandomEmitter(hitGround ? &impact : pos, math_random() % 62U + 120, fxOwner, fxList);
    particle_startPlayerRumbles(pos, 506.0f, 1013.0f, 0.0009871669f, 15, 20, fxOwner, fxList);
    particle_startPlayerCameraWobbles(pos, 506.0f, 1013.0f, 0.0009871669f, 5, fxOwner, fxList);
    particle_spawnBlastSpray(pos, fxOwner, fxList);

    if (variant != 0) {
        if (variant == 2) {
            effectA = 0x42;
            effectB = 0x41;
        } else {
            effectA = 0x2B;
            effectB = 0x2A;
        }
        particle_runAttackPairAt(pos, 0, owner, obj, 0, effectA, effectB, arg4);
    }

    actor_playImpactSound(0, pos->x, pos->y, pos->z, 1);
    particle_spawnGrowFade(pos, math_randomFloat() * 4.0f + 12.0f, 1.6409999f, math_random() % 56U + 200, 0.0f, fxOwner, fxList);
}
