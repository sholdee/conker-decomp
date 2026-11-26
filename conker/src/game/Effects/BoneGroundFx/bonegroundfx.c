#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Get the position of one of the actor's bones (picked by `side`, 0x10 by default).  With a `hit`
   buffer, also look for the ground below that point and return collision_probeFloorSurfABSet0Cached's result. */
s32 effect_getQuadrupedFootPos(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side) {
    Vec3f probe;
    s32 bone;

    switch (side) {
    case 4:
        bone = 0x17;
        break;
    case 5:
        bone = 0x14;
        break;
    case 6:
        bone = 0xC;
        break;
    case 7:
    default:
        bone = 0x10;
        break;
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

/* Returns 0x11 for `arg0` 0 or 1, -1 otherwise. */
s32 effect_selectQuadrupedFootEffect(s32 arg0, s32 arg1) {
    if (arg0 == 0 || arg0 == 1) {
        return 0x11;
    }
    return -1;
}

/* Effect 0xB at the ground below the actor's bone on `side`, plus effect 2 there when strength is
   at least 76. */
// matching: s32 actor (cast at each use) makes IDO reload it from the arg slot for each cast
void effect_spawnQuadrupedFootprint(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    CollisionHit hit;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (effect_getQuadrupedFootPos(&pos, (Actor *)actor, &hit, side) != 0) {
            effect_spawnFootprint(0xB, &hit, strength * (1.0f / 85.0f) * 3.177f,
                          (u32)(((Actor *)actor)->theta * (256.0f / 360.0f)), &pos, side);
            if (strength >= 76) {
                effect_spawnSplashSpray(2, &pos, hit.material, 3.232f, 2.436f);
            }
        }
    }
}

/* Same as effect_getQuadrupedFootPos with bones 0x19 and 0x16 for sides 4 and 5. */
s32 effect_getQuadrupedFootPosAlt(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side) {
    Vec3f probe;
    s32 bone;

    switch (side) {
    case 4:
        bone = 0x19;
        break;
    case 5:
        bone = 0x16;
        break;
    case 6:
        bone = 0xC;
        break;
    case 7:
    default:
        bone = 0x10;
        break;
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

/* Returns 0x12 for `arg0` 0 or 1, -1 otherwise. */
s32 effect_selectQuadrupedFootEffectAlt(s32 arg0, s32 arg1) {
    if (arg0 == 0 || arg0 == 1) {
        return 0x12;
    }
    return -1;
}

/* Same as effect_spawnQuadrupedFootprint with the effect_getQuadrupedFootPosAlt bones and a scale factor of 2 instead of 3.177. */
// matching: s32 actor (cast at each use) makes IDO reload it from the arg slot for each cast
void effect_spawnQuadrupedFootprintAlt(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    CollisionHit hit;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (effect_getQuadrupedFootPosAlt(&pos, (Actor *)actor, &hit, side) != 0) {
            effect_spawnFootprint(0xB, &hit, strength * (1.0f / 85.0f) * 2,
                          (u32)(((Actor *)actor)->theta * (256.0f / 360.0f)), &pos, side);
            if (strength >= 76) {
                effect_spawnSplashSpray(2, &pos, hit.material, 3.232f, 2.436f);
            }
        }
    }
}

/* Sets or clears flag bits in unk84 from script_query queries. */
void func_150BEEB0(Camera *arg0) {
    if (script_query(1, 0x4063, 6, 0x2000) || script_query(1, 0x4001, 6, 0x9000)) {
        arg0->AvailableModes |= 0x1010;
    } else {
        arg0->AvailableModes &= ~0x1010;
    }
    if (script_query(1, 0x4069, 6, 0x2000)) {
        arg0->AvailableModes |= 0x1000000;
    } else {
        arg0->AvailableModes &= ~0x1000000;
    }
}
