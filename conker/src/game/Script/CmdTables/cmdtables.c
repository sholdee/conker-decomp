#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * A data-only unit: the handler tables of the script commands. Bits 12-15 of a command word select the
 * class (entry class - 1), bits 0-11 the command or object id. The interpreter (game/Script/ScriptInterp/scriptinterp.s) runs the
 * start and end handlers of gScriptStartHandlers / gScriptEndHandlers and the query and command handlers of gScriptQueryHandlers /
 * gScriptCommandHandlers; the dispatchers script_query / script_command (game/Script/TaskSet/taskset) call the last two. A null entry is
 * a class that has no handler of that kind.
 *
 * Its .data (0x22CEE0) follows the resource manifest D_80087430 (0x22BEF0..0x22CEE0, extracted). The
 * manifest's names end at 0x243B90 with the 16-byte padding of a unit, before game/Script/EffectCmd/effectcmd's .rodata, so the
 * manifest is the data of a unit between game/Script/CutsceneCmd/cutscenecmd and game/Script/EffectCmd/effectcmd, not of game/Script/TaskSet/taskset (the reader of these
 * tables), whose .data ends at 0x22BEF0. None of the code units after it (game/Script/EffectCmd/effectcmd .. game/Script/TaskCmd/taskcmd) uses
 * the tables, and the handlers come from eleven files (game/Script/CameraCmd/cameracmd .. game/Script/ZoneCmd/zonecmd). These tables may be the
 * tail of the manifest's unit; they are code data, so they are defined here in C.
 *
 * The handlers have different parameter types, so the tables are declared without a prototype.
 */

/* Start handlers, by class: (id, record). */
s32 (*gScriptStartHandlers[15])() = {
    NULL,           /* 1 */
    script_spawnActor,  /* 2: game/Script/ActorCmd/actorcmd */
    NULL,           /* 3 */
    NULL,           /* 4 */
    NULL,           /* 5 */
    NULL,           /* 6 */
    script_playSound,  /* 7: game/Script/ObjectCmd/objectcmd */
    script_spawnSwarm,  /* 8: game/Script/SwarmCmd/swarmcmd */
    NULL,           /* 9 */
    NULL,           /* 10 */
    script_startTimer,  /* 11: game/Script/TimerCmd/timercmd */
    NULL,           /* 12 */
    NULL,           /* 13 */
    script_startEffect,  /* 14: game/Script/EffectCmd/effectcmd */
    script_initSpringValue,  /* 15: game/Script/MiscCmd/misccmd */
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
    script_emptyMiscEnd,
};

/* Query handlers, by class: (id, opcode, record). */
s32 (*gScriptQueryHandlers[15])() = {
    script_cutsceneQuery,  /* 1: game/Script/CutsceneCmd/cutscenecmd */
    script_actorQuery,  /* 2: game/Script/ActorCmd/actorcmd */
    script_worldObjectQuery,  /* 3: game/Script/ObjectCmd/objectcmd */
    script_zoneQuery,  /* 4: game/Script/ZoneCmd/zonecmd */
    script_taskQuery,  /* 5: game/Script/TaskCmd/taskcmd */
    script_streamQuery,  /* 6: game/Script/MusicCmd/musiccmd */
    script_soundQuery,  /* 7: game/Script/ObjectCmd/objectcmd */
    script_swarmQuery,  /* 8: game/Script/SwarmCmd/swarmcmd */
    script_cameraQuery,  /* 9: game/Script/CameraCmd/cameracmd */
    script_musicQuery,  /* 10: game/Script/MusicCmd/musiccmd */
    script_timerQuery,  /* 11: game/Script/TimerCmd/timercmd */
    script_emptyQuery12,  /* 12: game/Script/ObjectCmd/objectcmd */
    NULL,           /* 13 */
    script_effectQuery,  /* 14: game/Script/EffectCmd/effectcmd */
    script_miscQuery,  /* 15: game/Script/MiscCmd/misccmd */
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
    script_trapCommand12,
    NULL,
    script_effectCommand,
    script_miscCommand,
};
