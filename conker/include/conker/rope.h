#ifndef CONKER_ROPE_H
#define CONKER_ROPE_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "rope": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_1D9F10.c
 *   game_1DF510.c
 *   game_1E0560.c
 */

/* Template copied over HoltenRopeEffect 0x10-0x47 when a rope is created (func_151B30B0). */
typedef struct HoltenRopeDesc {
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 timer;
    /* 0x04 */ Vec3f start;
    /* 0x10 */ Vec3f end;
    /* 0x1C */ s8 startCallback;
    /* 0x1D */ s8 endCallback;
    /* 0x1E */ u8 drawStyle;
    /* 0x1F */ u8 pad1F;
    /* 0x20 */ f32 halfWidth;
    /* 0x24 */ s8 updateCallback;
    /* 0x25 */ u8 pad25[3];
    /* 0x28 */ f32 length;
    /* 0x2C */ f32 accelScale;
    /* 0x30 */ f32 lagScale;
    /* 0x34 */ u8 type;
    /* 0x35 */ u8 pad35[3];
} HoltenRopeDesc; /* size 0x38 */

#endif
