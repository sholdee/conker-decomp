#include <ultra64.h>
#include "conker/game.h"
#include "conker/script.h"


/* 12-byte slot record. The first word is a bitfield: golden reads it whole (`lw` + `srl 31`)
   for the guard and updates the top byte in place (`lbu`/`sb`) for each field write, which is
   exactly what these bitfield assignments produce. */
typedef struct ScriptTimer {
    u32 active : 1;  /* claimed by the start handler */
    u32 running : 1; /* counting (command 0 sets it, command 1 clears it) */
    u32 unit : 4;    /* index into gTimerUnitTicks / gTimerUnitWraps */
    u32 unk0_b6 : 2;
    u32 unk0_rest : 24;
    u32 count; /* units counted, wrapped by gTimerUnitWraps[unit] */
    u32 ticks; /* ticks towards the next count */
} ScriptTimer;

static ScriptTimer gScriptTimers[10];

/* Defined here (not extern): as1 then knows the array's alignment and shares one `lui at`
   between stores in the same 8-byte window of the peeled records. */
void script_resetTimers(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        gScriptTimers[i].active = 0;
        gScriptTimers[i].count = 0;
        gScriptTimers[i].ticks = 0;
    }
}

/* Per unit (indexed by ScriptTimer.unit): the ticks that make one count, and the count's wrap
   (0: none). The unit-4 period 0x5180 is 86400 cut to 16 bits. */
static u16 gTimerUnitTicks[] = { 2, 6, 60, 3600, 0x5180, 0 };
static u16 gTimerUnitWraps[] = { 0, 10, 60, 60, 24, 0 };

void script_updateTimers(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if ((gScriptTimers[i].active != 0) && (gScriptTimers[i].running == 1)) {
            gScriptTimers[i].ticks = gScriptTimers[i].ticks + gFrameTicks;
            if (gScriptTimers[i].ticks >= gTimerUnitTicks[gScriptTimers[i].unit]) {
                gScriptTimers[i].ticks = gScriptTimers[i].ticks - gTimerUnitTicks[gScriptTimers[i].unit];
                gScriptTimers[i].count = gScriptTimers[i].count + 1;
                if (gTimerUnitWraps[gScriptTimers[i].unit] != 0) {
                    gScriptTimers[i].count = gScriptTimers[i].count % gTimerUnitWraps[gScriptTimers[i].unit];
                }
            }
        }
    }
}


/* Start handler (gScriptStartHandlers): claims timer `timer`, stopped, counting in unit cmd->arg (ScriptTimer.unit). */
s32 script_startTimer(s32 timer, ScriptCmd *cmd) {
    ScriptTimer *r = &gScriptTimers[timer];

    if (r->active != 0) {
        return 0;
    }
    r->active = 1;
    r->running = 0;
    r->unit = cmd->arg.w;
    r->count = 0;
    return 1;
}

/* Command handler (gScriptCommandHandlers): 0 runs the timer, 1 stops it, 2 sets its count to cmd->a[0]. */
s32 script_timerCommand(s32 timer, s32 opcode, ScriptCmd *cmd) {
    switch (opcode) {
    case 0:
        gScriptTimers[timer].running = 1;
        return 1;
    case 1:
        gScriptTimers[timer].running = 0;
        return 1;
    case 2:
        gScriptTimers[timer].count = cmd->a[0].w;
        return 1;
    }
    return 0;
}

s32 script_timerQuery(s32 timer, s32 opcode, s32 cmd) {
    if (opcode == 3) {
        return gScriptTimers[timer].count;
    }
    return 0;
}
