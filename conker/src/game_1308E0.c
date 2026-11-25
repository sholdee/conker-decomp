#include <ultra64.h>
#include "functions.h"
#include "variables.h"


s32 func_1509BE40(s32, s32, s32, s32);

/* Sets flag bits in actor->unk84: 0x01000000 while in water, and level-specific bits
 * from func_1509BE40 queries for the levels D_800BE9F0 == 0x34, 0x30 and 0x2D. */
void func_15103430(struct108 *actor) {
    s32 queryFlags;

    queryFlags = actor->unk23D | 0x9000;
    if (actor->unk3D0->in_water == 1) {
        actor->unk84 |= 0x01000000;
    } else {
        actor->unk84 &= ~0x01000000;
    }

    if (D_800BE9F0 == 0x34) {
        if (func_1509BE40(1, 0x406D, 6, queryFlags) != 0) {
            actor->unk84 |= 0x200;
        } else {
            actor->unk84 &= ~0x200;
        }
    } else if (D_800BE9F0 == 0x30) {
        if (func_1509BE40(1, 0x403C, 6, queryFlags) != 0 ||
            func_1509BE40(1, 0x403D, 6, queryFlags) != 0) {
            actor->unk84 |= 0x200;
        } else {
            actor->unk84 &= ~0x200;
        }
    } else if (D_800BE9F0 == 0x2D) {
        if (func_1509BE40(1, 0x4053, 6, queryFlags) != 0 ||
            func_1509BE40(1, 0x4054, 6, queryFlags) != 0 ||
            func_1509BE40(1, 0x4056, 6, queryFlags) != 0 ||
            func_1509BE40(1, 0x4057, 6, queryFlags) != 0) {
            actor->unk84 |= 0x20000000;
        } else {
            actor->unk84 &= ~0x20000000;
        }

        if (func_1509BE40(1, 0x4058, 6, 0x9000) != 0 ||
            func_1509BE40(1, 0x4059, 6, 0x9000) != 0) {
            actor->unk84 |= 0x80000000;
        } else {
            actor->unk84 &= ~0x80000000;
        }
    } else if (D_800BE9F0 == 0x34) {
        // Unreachable (0x34 is handled above), but the original code has this check.
        if (func_1509BE40(1, 0x406E, 6, queryFlags) != 0) {
            actor->unk84 |= 0x20000000;
        } else {
            actor->unk84 &= ~0x20000000;
        }
    }
}
