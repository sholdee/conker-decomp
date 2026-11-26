#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* On event 0x2D, switch the object's id to the other end of the pair (with that end's tag). */
void func_151C4AB0(GameObj151C4AB0 *obj, GameStruct151C4AB0a *pair, u8 event) {
    GameStruct151C4AB0b *state = &obj->state;

    if (event == 0x2D) {
        if (state->unk90 == pair->idA) {
            state->unk90 = pair->idB;
            state->unk94 = pair->tagB;
        } else if (state->unk90 == pair->idB) {
            state->unk90 = pair->idA;
            state->unk94 = pair->tagA;
        }
    }
}

/*
 * Sets up the ray `hit` for a projectile starting at `origin`: reuses `knownHit` when given (re-aimed from
 * `origin`), otherwise casts along `dir` (or from `altOrigin` along `altDir`) when `castRay` is set.
 * Stores the travel distance times `distScale` in *remaining; on a miss it clears `clearMask` from *flags
 * and stores `defaultRemaining` instead. Returns 0 if the ray could not be set up.
 */
s32 weapon_setupProjectileRay(RayHit *knownHit, RayHit *hit, Vec3f *origin, Vec3f *dir, f32 *remaining,
                  f32 distScale, u8 castRay, Vec3f *altOrigin, Vec3f *altDir, u8 *flags, u8 clearMask,
                  f32 defaultRemaining, void *owner, s8 arg13, u8 collide, u8 arg15, s32 arg16, u8 recast) {
    RayHit recastHit;

    if (knownHit != NULL) {
        *hit = *knownHit;
        if (origin->x != hit->origin.x || origin->y != hit->origin.y || origin->z != hit->origin.z) {
            hit->dir.x = hit->pos.x - origin->x;
            hit->dir.y = hit->pos.y - origin->y;
            hit->dir.z = hit->pos.z - origin->z;
            hit->origin = *origin;
            if (math_vec3Normalize(&hit->dir, &hit->dir, &hit->dist, NULL) == 0) {
                return 0;
            }
        }
        if (recast && (hit->hitType == 2 || hit->hitType == 3)) {
            collision_castRay(owner, hit->origin.x, hit->origin.y, hit->origin.z, hit->dir.x, hit->dir.y, hit->dir.z,
                          &recastHit, 0.0f, 0, 1, 1, -1, 0, arg16);
            if (recastHit.hitType != 0) {
                *hit = recastHit;
            }
        }
        *remaining = hit->dist * distScale;
    } else if (castRay) {
        u8 found;
        s32 index;

        index = -1; // matching: IDO emits this store although every path below overwrites it
        if (altOrigin != NULL && altDir != NULL) {
            hit->origin = *altOrigin;
            hit->dir = *altDir;
        } else {
            hit->origin = *origin;
            hit->dir = *dir;
        }
        collision_castRay(owner, hit->origin.x, hit->origin.y, hit->origin.z, hit->dir.x, hit->dir.y, hit->dir.z, hit,
                      0.0f, 0, collide ? 0 : 1, (arg15 && collide) ? 0 : 1, arg13, 0, arg16);
        if (hit->surface == NULL) {
            index = -1;
        } else {
            index = hit->surface - db_Object_array;
        }
        if (hit->hitType == 0) {
            found = FALSE;
        } else if (hit->hitType == 1) {
            found = TRUE;
        } else if (altOrigin != NULL && altDir != NULL) {
            f32 dist;
            RayHit saved;

            saved = *hit; // matching: unused copy, but the 0x64-byte stack slot and copy are in the target
            hit->origin = *origin;
            hit->dir.x = hit->pos.x - origin->x;
            hit->dir.y = hit->pos.y - origin->y;
            hit->dir.z = hit->pos.z - origin->z;
            if (math_vec3Normalize(&hit->dir, &hit->dir, &hit->dist, NULL) == 0) {
                return 0;
            }
            dist = hit->dist;
            found = collision_castRayWorld(hit->pos.x, hit->pos.y, hit->pos.z, hit->dir.x, hit->dir.y, hit->dir.z, NULL,
                                  &hit->tri, &hit->pos.x, &hit->pos.y, &hit->pos.z, &hit->dist, &index,
                                  &hit->material, 0.0f);
            if (found) {
                hit->dist += dist;
                hit->origin = *origin;
            }
        } else {
            found = collision_castRayWorld(origin->x, origin->y, origin->z, hit->dir.x, hit->dir.y, hit->dir.z, NULL,
                                  &hit->tri, &hit->pos.x, &hit->pos.y, &hit->pos.z, &hit->dist, &index,
                                  &hit->material, 0.0f);
        }
        if (found) {
            *remaining = hit->dist * distScale;
            if (origin->x != hit->origin.x || origin->y != hit->origin.y || origin->z != hit->origin.z) {
                hit->dir.x = hit->pos.x - origin->x;
                hit->dir.y = hit->pos.y - origin->y;
                hit->dir.z = hit->pos.z - origin->z;
                hit->origin = *origin;
                if (math_vec3Normalize(&hit->dir, &hit->dir, &hit->dist, NULL) == 0) {
                    return 0;
                }
            }
            weapon_queueShotSound(owner, &hit->pos, &hit->dir, hit->material, hit->dist);
            if (index == -1) {
                hit->surface = NULL;
            } else {
                hit->surface = db_Object_array + index;
            }
        } else {
            *flags &= ~clearMask;
            *remaining = defaultRemaining;
            if (dir == NULL && altDir == NULL) {
                return 0;
            }
        }
    } else {
        *flags &= ~clearMask;
        *remaining = defaultRemaining;
        hit->origin = *origin;
        if (dir != NULL) {
            hit->dir = *dir;
        } else if (altDir != NULL) {
            hit->dir = *altDir;
        } else {
            return 0;
        }
    }
    return 1;
}
