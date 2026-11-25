#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A pair of linked ids, each with a byte that goes with it. */
typedef struct {
    /* 0x0 */ s32 idA;
    /* 0x4 */ s32 idB;
    /* 0x8 */ u8 tagA;
    /* 0x9 */ u8 tagB;
} GameStruct151C4AB0a;

/* State block stored at offset 0x110 of the object. */
typedef struct {
    /* 0x00 */ u8 pad0[0x90];
    /* 0x90 */ s32 unk90;
    /* 0x94 */ u8 unk94;
} GameStruct151C4AB0b;

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ GameStruct151C4AB0b state;
} GameObj151C4AB0;

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

/* Result of a ray cast (func_15081690 / func_150AC9C0). */
typedef struct Game1F1F60Hit {
    /* 0x00 */ void *actor;
    /* 0x04 */ f32 dist;
    /* 0x08 */ Vec3f hit;
    /* 0x14 */ Vec3f normal;
    /* 0x20 */ Vec3f point;
    /* 0x2C */ Vec3f start;
    /* 0x38 */ Vec3f end; /* the ray direction */
    /* 0x44 */ s16 surface[9];
    /* 0x56 */ s16 field_56;
    /* 0x58 */ s8 field_58;
    /* 0x59 */ u8 type;
    /* 0x5A */ s8 field_5A;
    /* 0x5B */ u8 pad5B;
    /* 0x5C */ WorldObject *field_5C;
    /* 0x60 */ s32 field_60;
} Game1F1F60Hit; /* 0x64 */

void func_15081690(void *actor, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, Game1F1F60Hit *hit, f32 maxDist, s32,
                   s32, s32, s32, s32, s32);
s32 func_15145128(f32 *in, f32 *out, f32 *length, f32 *);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, s16 *, f32 *, f32 *, f32 *, f32 *, s32 *, void *, f32);
void func_151C2050(Actor *arg0, f32 *arg1, f32 *arg2, s32 arg3, f32 arg4);

/*
 * Sets up the ray `hit` for a projectile starting at `origin`: reuses `knownHit` when given (re-aimed from
 * `origin`), otherwise casts along `dir` (or from `altOrigin` along `altDir`) when `castRay` is set.
 * Stores the travel distance times `distScale` in *remaining; on a miss it clears `clearMask` from *flags
 * and stores `defaultRemaining` instead. Returns 0 if the ray could not be set up.
 */
s32 func_151C4B0C(Game1F1F60Hit *knownHit, Game1F1F60Hit *hit, Vec3f *origin, Vec3f *dir, f32 *remaining,
                  f32 distScale, u8 castRay, Vec3f *altOrigin, Vec3f *altDir, u8 *flags, u8 clearMask,
                  f32 defaultRemaining, void *owner, s8 arg13, u8 collide, u8 arg15, s32 arg16, u8 recast) {
    Game1F1F60Hit recastHit;

    if (knownHit != NULL) {
        *hit = *knownHit;
        if (origin->x != hit->start.x || origin->y != hit->start.y || origin->z != hit->start.z) {
            hit->end.x = hit->hit.x - origin->x;
            hit->end.y = hit->hit.y - origin->y;
            hit->end.z = hit->hit.z - origin->z;
            hit->start = *origin;
            if (func_15145128(&hit->end.x, &hit->end.x, &hit->dist, NULL) == 0) {
                return 0;
            }
        }
        if (recast && (hit->type == 2 || hit->type == 3)) {
            func_15081690(owner, hit->start.x, hit->start.y, hit->start.z, hit->end.x, hit->end.y, hit->end.z,
                          &recastHit, 0.0f, 0, 1, 1, -1, 0, arg16);
            if (recastHit.type != 0) {
                *hit = recastHit;
            }
        }
        *remaining = hit->dist * distScale;
    } else if (castRay) {
        u8 found;
        s32 index;

        index = -1; // matching: IDO emits this store although every path below overwrites it
        if (altOrigin != NULL && altDir != NULL) {
            hit->start = *altOrigin;
            hit->end = *altDir;
        } else {
            hit->start = *origin;
            hit->end = *dir;
        }
        func_15081690(owner, hit->start.x, hit->start.y, hit->start.z, hit->end.x, hit->end.y, hit->end.z, hit,
                      0.0f, 0, collide ? 0 : 1, (arg15 && collide) ? 0 : 1, arg13, 0, arg16);
        if (hit->field_5C == NULL) {
            index = -1;
        } else {
            index = hit->field_5C - D_800DBEF4;
        }
        if (hit->type == 0) {
            found = FALSE;
        } else if (hit->type == 1) {
            found = TRUE;
        } else if (altOrigin != NULL && altDir != NULL) {
            f32 dist;
            Game1F1F60Hit saved;

            saved = *hit; // matching: unused copy, but the 0x64-byte stack slot and copy are in the target
            hit->start = *origin;
            hit->end.x = hit->hit.x - origin->x;
            hit->end.y = hit->hit.y - origin->y;
            hit->end.z = hit->hit.z - origin->z;
            if (func_15145128(&hit->end.x, &hit->end.x, &hit->dist, NULL) == 0) {
                return 0;
            }
            dist = hit->dist;
            found = func_150AC9C0(hit->hit.x, hit->hit.y, hit->hit.z, hit->end.x, hit->end.y, hit->end.z, NULL,
                                  hit->surface, &hit->hit.x, &hit->hit.y, &hit->hit.z, &hit->dist, &index,
                                  &hit->field_60, 0.0f);
            if (found) {
                hit->dist += dist;
                hit->start = *origin;
            }
        } else {
            found = func_150AC9C0(origin->x, origin->y, origin->z, hit->end.x, hit->end.y, hit->end.z, NULL,
                                  hit->surface, &hit->hit.x, &hit->hit.y, &hit->hit.z, &hit->dist, &index,
                                  &hit->field_60, 0.0f);
        }
        if (found) {
            *remaining = hit->dist * distScale;
            if (origin->x != hit->start.x || origin->y != hit->start.y || origin->z != hit->start.z) {
                hit->end.x = hit->hit.x - origin->x;
                hit->end.y = hit->hit.y - origin->y;
                hit->end.z = hit->hit.z - origin->z;
                hit->start = *origin;
                if (func_15145128(&hit->end.x, &hit->end.x, &hit->dist, NULL) == 0) {
                    return 0;
                }
            }
            func_151C2050(owner, &hit->hit.x, &hit->end.x, hit->field_60, hit->dist);
            if (index == -1) {
                hit->field_5C = NULL;
            } else {
                hit->field_5C = D_800DBEF4 + index;
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
        hit->start = *origin;
        if (dir != NULL) {
            hit->end = *dir;
        } else if (altDir != NULL) {
            hit->end = *altDir;
        } else {
            return 0;
        }
    }
    return 1;
}
