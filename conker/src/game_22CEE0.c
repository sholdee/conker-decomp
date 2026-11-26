#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A data-only unit: the handler tables of the script commands. Bits 12-15 of a command word select the
 * class (entry class - 1), bits 0-11 the command or object id. The interpreter (game_DAFA0.s) runs the
 * start and end handlers of gScriptStartHandlers / gScriptEndHandlers and the query and command handlers of gScriptQueryHandlers /
 * gScriptCommandHandlers; the dispatchers script_query / script_command (game_C8950) call the last two. A null entry is
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
s32 (*gScriptStartHandlers[15])() = {
    NULL,           /* 1 */
    script_spawnActor,  /* 2: game_C4DC0 */
    NULL,           /* 3 */
    NULL,           /* 4 */
    NULL,           /* 5 */
    NULL,           /* 6 */
    script_playSound,  /* 7: game_CBDB0 */
    script_spawnSwarm,  /* 8: game_CB1C0 */
    NULL,           /* 9 */
    NULL,           /* 10 */
    script_startTimer,  /* 11: game_CD5A0 */
    NULL,           /* 12 */
    NULL,           /* 13 */
    script_startEffect,  /* 14: game_C9EC0 */
    func_1509DF20,  /* 15: game_CB3D0 */
};

/* End handlers, by class: (id, record). */
s32 (*gScriptEndHandlers[15])() = {
    NULL,
    script_removeActor,
    NULL,
    NULL,
    NULL,
    NULL,
    script_stopSound,
    script_destroySwarm,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    script_endEffect,
    func_1509DFB4,
};

/* Query handlers, by class: (id, opcode, record). */
s32 (*gScriptQueryHandlers[15])() = {
    script_cutsceneQuery,  /* 1: game_C98F0 */
    script_actorQuery,  /* 2: game_C4DC0 */
    script_worldObjectQuery,  /* 3: game_CBDB0 */
    script_zoneQuery,  /* 4: game_CD860 */
    script_taskQuery,  /* 5: game_CCD00 */
    script_streamQuery,  /* 6: game_CBAF0 */
    func_1509F33C,  /* 7: game_CBDB0 */
    script_swarmQuery,  /* 8: game_CB1C0 */
    script_cameraQuery,  /* 9: game_C3E20 */
    script_musicQuery,  /* 10: game_CBAF0 */
    script_timerQuery,  /* 11: game_CD5A0 */
    func_1509F800,  /* 12: game_CBDB0 */
    NULL,           /* 13 */
    script_effectQuery,  /* 14: game_C9EC0 */
    script_miscQuery,  /* 15: game_CB3D0 */
};

/* Command handlers, by class: (id, opcode, record). */
s32 (*gScriptCommandHandlers[15])() = {
    script_cutsceneCommand,
    script_actorCommand,
    script_worldObjectCommand,
    script_zoneCommand,
    script_taskCommand,
    script_streamCommand,
    script_actorSoundCommand,
    script_swarmCommand,
    script_cameraCommand,
    script_musicCommand,
    script_timerCommand,
    func_1509F818,
    NULL,
    script_effectCommand,
    script_miscCommand,
};
