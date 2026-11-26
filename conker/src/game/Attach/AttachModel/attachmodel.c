#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c); conker/attach.h describes it. */
u8 gAttachModelRefCounts[0x1E4];                  /* per-model load reference counts */
s16 *gAttachModelTexIds[0x1E4];                /* per model: written by texture_resolveDl */
AttachModelHeader *gAttachModelHeaders[0x1E4];  /* per model: loaded attachment model header */

#include "functions.h"
#include "variables.h"

// NEAR-MISS 16 -> MATCH (wave 61). The parked candidate carried a `u8 c` local that the
// original never had. Frame-layout law measured here (game/Attach/AttachModel/attachmodel, -O2 -g3):
//
//     temp_area_top = (frame - 4) - roundup4(sum of named-local sizes)
//     frame         = round8(0x28 + roundup4(locals) + 4 * n_temps)
//
// so golden's first spill at 0x38 with frame 0x40 proves the function has EXACTLY ONE
// named local (4 bytes), and its second spill at 0x2c (three slots lower, not two) proves
// four compiler temps, one more than any two-local spelling can produce. `ret` is that
// one local; the `move v1,v0` pair that looked like a named `c` is IDO's own temporary,
// created by the post-increment `gAttachModelRefCounts[arg0]++` -- which yields the old value as a
// separate live range while the CSE of the load feeds the addiu. Writing the increment as
// `= gAttachModelRefCounts[arg0] + 1` collapses the two live ranges and costs 847.
Gfx *attach_loadModelDl(s32 arg0) {
    AttachModelHeader *ret;

    if (gAttachModelRefCounts[arg0] == 0) {
        ret = rom_loadPathTable(0, 0, 0, 2, 9, arg0);
        if (ret == NULL) {
            return NULL;
        }
        // the table pointer is passed as one display list here
        texture_resolveDl((Gfx *)ret->dlists, 0, 1, 0x3E, &gAttachModelTexIds[arg0]);
        func_15168E54((Gfx *)ret->dlists, (s32)ret);
        gAttachModelHeaders[arg0] = ret;
    } else {
        ret = gAttachModelHeaders[arg0];
    }
    if (gAttachModelRefCounts[arg0] != 0xFF) {
        gAttachModelRefCounts[arg0]++;
    }
    return (Gfx *)ret->dlists; // the table is used as one display list
}

void attach_releaseModel(s32 arg0) {
    if (arg0 != 0x1E4) {
        if (gAttachModelRefCounts[arg0] != 0) {
            if (--gAttachModelRefCounts[arg0] == 0) {
                texture_releaseList(gAttachModelTexIds[arg0]);
                func_100043B4(gAttachModelHeaders[arg0], 4);
            }
        }
    }
}
