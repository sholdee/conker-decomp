#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/attach.h describes it. */
u8 D_800DF7D0[0x1E4];                  /* per-model load reference counts */
s16 *D_800DF9B8[0x1E4];                /* per model: written by func_1510CE60 */
AttachModelHeader *D_800E0148[0x1E4];  /* per model: loaded attachment model header */

#include "functions.h"
#include "variables.h"

// NEAR-MISS 16 -> MATCH (wave 61). The parked candidate carried a `u8 c` local that the
// original never had. Frame-layout law measured here (game_1B9DB0, -O2 -g3):
//
//     temp_area_top = (frame - 4) - roundup4(sum of named-local sizes)
//     frame         = round8(0x28 + roundup4(locals) + 4 * n_temps)
//
// so golden's first spill at 0x38 with frame 0x40 proves the function has EXACTLY ONE
// named local (4 bytes), and its second spill at 0x2c (three slots lower, not two) proves
// four compiler temps, one more than any two-local spelling can produce. `ret` is that
// one local; the `move v1,v0` pair that looked like a named `c` is IDO's own temporary,
// created by the post-increment `D_800DF7D0[arg0]++` -- which yields the old value as a
// separate live range while the CSE of the load feeds the addiu. Writing the increment as
// `= D_800DF7D0[arg0] + 1` collapses the two live ranges and costs 847.
Gfx *func_1518C900(s32 arg0) {
    AttachModelHeader *ret;

    if (D_800DF7D0[arg0] == 0) {
        ret = func_1502B6BC(0, 0, 0, 2, 9, arg0);
        if (ret == NULL) {
            return NULL;
        }
        // the table pointer is passed as one display list here
        func_1510CE60((Gfx *)ret->dlists, 0, 1, 0x3E, &D_800DF9B8[arg0]);
        func_15168E54((Gfx *)ret->dlists, (s32)ret);
        D_800E0148[arg0] = ret;
    } else {
        ret = D_800E0148[arg0];
    }
    if (D_800DF7D0[arg0] != 0xFF) {
        D_800DF7D0[arg0]++;
    }
    return (Gfx *)ret->dlists; // the table is used as one display list
}

void func_1518CA04(s32 arg0) {
    if (arg0 != 0x1E4) {
        if (D_800DF7D0[arg0] != 0) {
            if (--D_800DF7D0[arg0] == 0) {
                func_1510D630(D_800DF9B8[arg0]);
                func_100043B4(D_800E0148[arg0], 4);
            }
        }
    }
}
