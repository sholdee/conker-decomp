#include <ultra64.h>
#include "variables.h"

// func_150E7FEC (game/Particle/EmissionSource/emissionsource.c) is declared in each file that calls it: game/Effects/FollowEffects/followeffects.c and game/Sight/SightBeam/sightbeam.c were
// compiled with a different view of it.
void func_150E7FEC(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6, u8 arg7, u8 arg8,
                   u8 arg9, u8 owner, s32 kind);

/* Spawns an impact burst at pos: a ground splash when there is floor just below, effects, an optional variant-specific effect pair and particles. */
void func_15103E40(Actor *owner, Actor *obj, Vec3f *pos, u8 variant, s32 arg4, u8 arg5, s32 arg6) {
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
    particle_spawnLightFlash(pos, arg5, arg6);

    top.x = pos->x;
    top.y = pos->y + 100.0f;
    top.z = pos->z;
    collision_initHitFromObject(&hit, obj);

    if (collision_probeFloorSurfABSet0Cached(&top, 0, pos->y - 100.0f, &hit) && (hit.flags & 1)) {
        impact.x = top.x;
        hitGround = TRUE;
        impact.y = hit.height;
        impact.z = top.z;
        func_150E7FEC(func_150ADA68() * 125.0f + 204.0f, func_150ADA20() % 101U + 155, &hit.tri, &impact,
                      func_150ADA20() % 302U + 500, 0, 1, 0, 0, 0, arg5, 0);
    }

    func_150E83AC(hitGround ? &impact : pos, func_150ADA20() % 62U + 120, arg5, arg6);
    particle_startPlayerRumbles(pos, 506.0f, 1013.0f, 0.0009871669f, 15, 20, arg5, arg6);
    particle_startPlayerCameraWobbles(pos, 506.0f, 1013.0f, 0.0009871669f, 5, arg5, arg6);
    particle_spawnBlastSpray(pos, arg5, arg6);

    if (variant != 0) {
        if (variant == 2) {
            effectA = 0x42;
            effectB = 0x41;
        } else {
            effectA = 0x2B;
            effectB = 0x2A;
        }
        func_151D40D4(pos, 0, owner, obj, 0, effectA, effectB, arg4);
    }

    func_15055A2C(0, pos->x, pos->y, pos->z, 1);
    particle_spawnGrowFade(pos, func_150ADA68() * 4.0f + 12.0f, 1.6409999f, func_150ADA20() % 56U + 200, 0.0f, arg5, arg6);
}
