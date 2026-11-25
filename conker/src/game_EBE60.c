#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Get the position of one of the actor's bones (picked by `side`, 0x10 by default).  With a `hit`
   buffer, also look for the ground below that point and return func_15046C80's result. */
s32 func_150BE9B0(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side) {
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
    func_15142314(actor->ob_matrix_ptr, bone, pos);
    if (hit == NULL) {
        return 1;
    }

    probe.x = pos->x;
    probe.y = pos->y + 10.0f;
    probe.z = pos->z;
    func_1504715C(hit, actor);
    return func_15046C80(&probe, 0, pos->y - 50.0f, hit);
}

/* Returns 0x11 for `arg0` 0 or 1, -1 otherwise. */
s32 func_150BEAA8(s32 arg0, s32 arg1) {
    if (arg0 == 0 || arg0 == 1) {
        return 0x11;
    }
    return -1;
}

/* Effect 0xB at the ground below the actor's bone on `side`, plus effect 2 there when strength is
   at least 76. */
// matching: s32 actor (cast at each use) makes IDO reload it from the arg slot for each cast
void func_150BEACC(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    CollisionHit hit;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (func_150BE9B0(&pos, (Actor *)actor, &hit, side) != 0) {
            func_15141F78(0xB, &hit, strength * (1.0f / 85.0f) * 3.177f,
                          (u32)(((Actor *)actor)->theta * (256.0f / 360.0f)), &pos, side);
            if (strength >= 76) {
                func_15142180(2, &pos, hit.material, 3.232f, 2.436f);
            }
        }
    }
}

/* Same as func_150BE9B0 with bones 0x19 and 0x16 for sides 4 and 5. */
s32 func_150BEC30(Vec3f *pos, Actor *actor, CollisionHit *hit, u8 side) {
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
    func_15142314(actor->ob_matrix_ptr, bone, pos);
    if (hit == NULL) {
        return 1;
    }

    probe.x = pos->x;
    probe.y = pos->y + 10.0f;
    probe.z = pos->z;
    func_1504715C(hit, actor);
    return func_15046C80(&probe, 0, pos->y - 50.0f, hit);
}

/* Returns 0x12 for `arg0` 0 or 1, -1 otherwise. */
s32 func_150BED28(s32 arg0, s32 arg1) {
    if (arg0 == 0 || arg0 == 1) {
        return 0x12;
    }
    return -1;
}

/* Same as func_150BEACC with the func_150BEC30 bones and a scale factor of 2 instead of 3.177. */
// matching: s32 actor (cast at each use) makes IDO reload it from the arg slot for each cast
void func_150BED4C(s32 actor, s32 side, s32 strength) {
    Vec3f pos;
    CollisionHit hit;

    if (((Actor *)actor)->ob_matrix_ptr != NULL) {
        if (func_150BEC30(&pos, (Actor *)actor, &hit, side) != 0) {
            func_15141F78(0xB, &hit, strength * (1.0f / 85.0f) * 2,
                          (u32)(((Actor *)actor)->theta * (256.0f / 360.0f)), &pos, side);
            if (strength >= 76) {
                func_15142180(2, &pos, hit.material, 3.232f, 2.436f);
            }
        }
    }
}

/* Sets or clears flag bits in unk84 from func_1509BE40 queries. */
void func_150BEEB0(Camera *arg0) {
    if (func_1509BE40(1, 0x4063, 6, 0x2000) || func_1509BE40(1, 0x4001, 6, 0x9000)) {
        arg0->AvailableModes |= 0x1010;
    } else {
        arg0->AvailableModes &= ~0x1010;
    }
    if (func_1509BE40(1, 0x4069, 6, 0x2000)) {
        arg0->AvailableModes |= 0x1000000;
    } else {
        arg0->AvailableModes &= ~0x1000000;
    }
}
