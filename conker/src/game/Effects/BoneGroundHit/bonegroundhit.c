#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Get the position of one of the actor's bones (0x21 for side 1, 0x1D for side 2; any other side
   fails with 0).  With a `hit` buffer, also look for the ground below that point and return
   collision_probeFloorSurfABSet0Cached's result.  (Same as effect_getFootPos with different bones.) */
s32 effect_getAltFootPos(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side) {
    Vec3f probe;
    s32 bone;

    if ((side != 1) && (side != 2)) {
        return 0;
    }

    if (side == 1) {
        bone = 0x21;
    } else {
        bone = 0x1D;
    }

    math_getBoneTranslation(actor->ob_matrix_ptr, bone, pos);
    if (hit == NULL) {
        return 1;
    }

    probe.x = pos->x;
    probe.y = pos->y + 10.0f;
    probe.z = pos->z;
    collision_initHitFromObject(hit, actor);
    return collision_probeFloorSurfABSet0Cached(&probe, 0, pos->y - 50.0f, hit);
}

/* Effect id for an actor's effect kind: kind 1 in LEVEL_W2_OUTSIDE_MOLEHOLE is 6 when func_151420F8
   accepts the actor and 3 otherwise; everything else is unmapped (-1). */
s32 effect_selectAltFootEffect(s32 kind, Actor *actor) {
    if (kind == 1) {
        if (level == LEVEL_W2_OUTSIDE_MOLEHOLE) {
            if (func_151420F8(actor) != 0) {
                return 6;
            }
            return 3;
        }
        return -1;
    }
    return -1;
}

/* Effect 0xA at the ground below the actor's bone on `side`. */
// matching: `actor` is an int cast at each use; with a pointer parameter IDO merges the two
// loads of it that the original makes (as effect_spawnFootprintAtFoot in game/Effects/ActorDamageFx/actordamagefx.c).
void effect_spawnFootprintAtAltFoot(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    CollisionHit hit;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (effect_getAltFootPos(&pos, (Actor *)actor, &hit, side) != 0) {
            effect_spawnFootprint(0xA, &hit, strength * (1.0f / 85.0f),
                          (u32)(((Actor *)actor)->theta * (256.0f / 360.0f)), &pos, side);
        }
    }
}
