#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One entry per spray type: a bone and two points in its local space. */
typedef struct {
    /* 0x00 */ u8 bone;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ f32 from[3];
    /* 0x10 */ f32 to[3];
} struct_AF4D0;

extern struct_AF4D0 D_800A9DF0[];

/* Blood spray of kind `type` between two points attached to one of obj's bones (skipped when unk74&0xF is 0xF). */
void func_151AF4D0(obj, type, sceneTag, arg3)
    Actor *obj;
    u8 type;
    u8 sceneTag;
    s32 arg3;
{
    f32 from[3];
    f32 to[3];
    Mtx *boneMtx;
    struct_AF4D0 *entry;

    if (obj != NULL && obj->ob_matrix_ptr != NULL && (obj->unk74 & 0xF) != 0xF) {
        entry = &D_800A9DF0[type];
        boneMtx = (Mtx *)obj->ob_matrix_ptr + entry->bone;
        func_15143134(entry->from, from, boneMtx);
        func_15143134(entry->to, to, boneMtx);
        func_15135DD0(from, to, ((func_150ADA68() * 170.0f) + 71.0f) * 0.01f, sceneTag, arg3);
    }
}
