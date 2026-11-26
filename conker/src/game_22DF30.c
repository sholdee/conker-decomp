#include <ultra64.h>
#include "structs.h"

/*
 * A data-only unit, linked after the game_142560..game_14C3F0 unit and game_14CE30. Its .data (0x22DF30) starts
 * where that unit's ends padded, and ends at 0x22DF74, padded to 0x22DF80 where game_14F8F0's starts; its .bss
 * (0x800DBFF0) follows game_14CE30's and ends at 0x800DBFF8, padded to 0x800DC000 where game_14FF90's starts.
 * No code between them reads this data. The debugger's name camera_global_Cameras suggests Rare's
 * camera_global.c.
 */

/* .bss, defined before variables.h and conker/camera.h declare it (an earlier extern changes the order). */
Camera *camera_global_Cameras; // original name (Oct 2000 debugger); one Camera per player
u8 D_800DBFF4[4]; // per-player camera hold timers; *(s32 *)D_800DBFF4 tests all four at once

#include "functions.h"
#include "variables.h"

/* .data */
Mtx D_80089470 = { {
    /* integer parts */
    { 0x00010000, 0x00000000, 0x00000001, 0x00000000 },
    { 0x00000000, 0x00010000, 0x00000000, 0x00000001 },
    /* fractional parts */
    { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
    { 0x00000000, 0x00000000, 0x00000000, 0x00000000 },
} }; // the identity, loaded as a fixed modelview matrix
s32 D_800894B0 = 0; // camera frames: counted by func_15122AE0, cleared by func_15005290
