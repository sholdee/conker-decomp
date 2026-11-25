#ifndef CONKER_PROP_H
#define CONKER_PROP_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "prop": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_362B0.c
 *   game_EC420.c
 *   game_F21D0.c
 *   game_F4D20.c
 *   game_FC550.c
 *   game_1188E0.c
 *   game_137ED0.c
 *   game_142560.c
 *   game_142A70.c
 *   game_1435C0.c
 *   game_143870.c
 *   game_143DE0.c
 *   game_144950.c
 *   game_1449A0.c
 *   game_144C70.c
 *   game_147470.c
 *   game_1478C0.c
 *   game_147C70.c
 *   game_1483E0.c
 *   game_14C3F0.c
 *   game_14CE30.c
 *   game/done/game_14D0D0.c
 *   game_1A48C0.c
 *   game_1AFC80.c
 */

/* Behaviour scratch state (WorldObject.touch) of the scenery movers in game_1188E0.c, game_143DE0.c and
 * game_1449A0.c (each file reaches it through its WORK(obj) macro). */
typedef union MoverWork {
    /* 0x7C */ s32 targetPos[3];  /* position to move to */
    /* 0x7C */ f32 targetSpin[3]; /* rotation to turn to */
    struct {
        /* 0x7C */ s32 target; /* animation position to reach, 0..0xFFFF */
        /* 0x80 */ u8 *frames; /* vertex frames loaded from ROM */
        /* 0x84 */ s32 pos;
    } anim;
    struct {
        /* 0x7C */ f32 damping;
        /* 0x80 */ f32 acceleration;
        /* 0x84 */ f32 velocity;
    } swing;
    struct {
        /* 0x7C */ f32 rebound; /* fraction of the velocity kept (reversed) when the target is crossed */
        /* 0x80 */ f32 acceleration; /* per tick */
        /* 0x84 */ f32 velocity;
    } bounce;
} MoverWork;

#endif
