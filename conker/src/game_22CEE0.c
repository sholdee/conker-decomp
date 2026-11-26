#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A data-only unit: the handler tables of the script commands. Bits 12-15 of a command word select the
 * class (entry class - 1), bits 0-11 the command or object id. The interpreter (game_DAFA0.s) runs the
 * start and end handlers of D_80088420 / D_8008845C and the query and command handlers of D_80088498 /
 * D_800884D4; the dispatchers func_1509BE40 / func_1509BFB0 (game_C8950) call the last two. A null entry is
 * a class that has no handler of that kind.
 *
 * Its .data (0x22CEE0) follows the resource manifest D_80087430 (0x22BEF0..0x22CEE0, extracted). The
 * manifest's names end at 0x243B90 with the 16-byte padding of a unit, before game_C9EC0's .rodata, so the
 * manifest is the data of a unit between game_C98F0 and game_C9EC0, not of game_C8950 (the reader of these
 * tables), whose .data ends at 0x22BEF0. None of the code units after it (game_C9EC0 .. game_CCD00) uses
 * the tables, and the handlers come from eleven files (game_C3E20 .. game_CD860). These tables may be the
 * tail of the manifest's unit; they are code data, so they are defined here in C.
 *
 * The handlers have different parameter types, so the tables are declared without a prototype.
 */

/* Start handlers, by class: (id, record). */
s32 (*D_80088420[15])() = {
    NULL,           /* 1 */
    func_15097910,  /* 2: game_C4DC0 */
    NULL,           /* 3 */
    NULL,           /* 4 */
    NULL,           /* 5 */
    NULL,           /* 6 */
    func_1509EFF0,  /* 7: game_CBDB0 */
    func_1509DD50,  /* 8: game_CB1C0 */
    NULL,           /* 9 */
    NULL,           /* 10 */
    func_150A0264,  /* 11: game_CD5A0 */
    NULL,           /* 12 */
    NULL,           /* 13 */
    func_1509D180,  /* 14: game_C9EC0 */
    func_1509DF20,  /* 15: game_CB3D0 */
};

/* End handlers, by class: (id, record). */
s32 (*D_8008845C[15])() = {
    NULL,
    func_150979CC,
    NULL,
    NULL,
    NULL,
    NULL,
    func_1509F248,
    func_1509DDC4,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    func_1509D780,
    func_1509DFB4,
};

/* Query handlers, by class: (id, opcode, record). */
s32 (*D_80088498[15])() = {
    func_1509C7C8,  /* 1: game_C98F0 */
    func_15099C14,  /* 2: game_C4DC0 */
    func_1509ED74,  /* 3: game_CBDB0 */
    func_150A04D8,  /* 4: game_CD860 */
    func_1509FE0C,  /* 5: game_CCD00 */
    func_1509E6F0,  /* 6: game_CBAF0 */
    func_1509F33C,  /* 7: game_CBDB0 */
    func_1509DEC4,  /* 8: game_CB1C0 */
    func_1509759C,  /* 9: game_C3E20 */
    func_1509E8A0,  /* 10: game_CBAF0 */
    func_150A0374,  /* 11: game_CD5A0 */
    func_1509F800,  /* 12: game_CBDB0 */
    NULL,           /* 13 */
    func_1509DBBC,  /* 14: game_C9EC0 */
    func_1509E3DC,  /* 15: game_CB3D0 */
};

/* Command handlers, by class: (id, opcode, record). */
s32 (*D_800884D4[15])() = {
    func_1509C440,
    func_15097A8C,
    func_1509E900,
    func_150A03B0,
    func_1509F850,
    func_1509E640,
    func_1509F284,
    func_1509DDFC,
    func_15096D78,
    func_1509E730,
    func_150A02D0,
    func_1509F818,
    NULL,
    func_1509D8FC,
    func_1509DFC4,
};
