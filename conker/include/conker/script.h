#ifndef CONKER_SCRIPT_H
#define CONKER_SCRIPT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "script": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game_A28B0.c
 *   game_C3E20.c
 *   game_C4DC0.c
 *   game_C8950.c
 *   game_C98F0.c
 *   game_C9EC0.c
 *   game_CB1C0.c
 *   game_CB3D0.c
 *   game_CBAF0.c
 *   game_CBDB0.c
 *   game_CCD00.c
 *   game_CD5A0.c
 *   game_CD860.c
 */

/* One argument word of a script command; handlers also read just its low halfword or low byte. */
typedef union CmdArg {
    s32 w;
    struct {
        s16 hi;
        s16 lo;
    } h;
    struct {
        u8 b0;
        u8 b1;
        u8 b2;
        u8 lo;
    } b;
} CmdArg;

/* A script command record: a command word, then its argument words. The interpreter (game_DAFA0.s) passes
 * the record to the start/end handlers of D_80088420 / D_8008845C (func_1509D180, func_1509D780) as
 * (id, cmd), and to the handlers of D_80088498 / D_800884D4 as (id, arg, cmd); the dispatchers
 * func_1509BE40 / func_1509BFB0 build one in D_800D2F60. Arguments that are speeds, scales or distances are
 * 16.16 fixed point (hence the many `* 1.52587890625e-05f`, i.e. 1/65536); angles in binary units convert
 * to degrees with 0.0054931640625f (360/65536) and back with 182.04444885f. */
typedef struct ScriptCmd {
    /* 0x00 */ s32 id;     /* bits 12-15: handler table + 1, bits 0-11: command or object id */
    /* 0x04 */ CmdArg arg; /* the handler's opcode, phase or first argument */
    /* 0x08 */ CmdArg a[7]; /* the parameters (D_800D3840 - 2 of them) */
} ScriptCmd;

/* Object id arguments carry flags above the low 12 bits. */
#define CMD_OBJECT_ID(arg) ((arg).w & 0xFFF)

#endif
