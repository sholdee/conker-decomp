#include <ultra64.h>
#include "functions.h"
#include "variables.h"


s32 func_1509DD10(s32 arg0) {
    return 1;
}

s32 func_1509DD20(s32 arg0) {
    return 1;
}

s32 func_1509DD30(s32 arg0) {
    return 1;
}

s32 func_1509DD40(s32 arg0) {
    return 1;
}

/* Script handler: spawns a swarm described by the command. */
s32 func_1509DD50(s32 arg0, SwarmSpawnCmd *cmd) {
    func_15177F90(cmd->memberCount, NULL, cmd->tag, cmd->x, cmd->y, cmd->z, cmd->unk18, 0x7FFF, 0xFF, cmd->playSfx,
                  0);
    return 1;
}

/* Script handler: destroys the swarm with the given tag. */
s32 func_1509DDC4(s32 tag, ScriptCmd *cmd) {
    u16 swarmTag = tag; // matching: the u16 local takes the register of cmd (unused)
    func_15178E14(swarmTag);
    return 0;
}

/* Script handler: makes the swarm with the given tag follow an object (op 0) or head for a fixed point (op 1). */
s32 func_1509DDFC(s32 tag, s32 op, SwarmTargetCmd *cmd) {
    switch (op) {
    case 0: {
        s32 mask = ~0x2000; // matching: the inline constant schedules differently
        func_15178BE4(tag & 0xFF, &movement_obj[func_15084000(cmd->target & mask)].xpos, cmd->argC.follow.yOffset);
        return 1;
    }
    case 1:
        func_15178C34(tag & 0xFF, cmd->target, cmd->argC.y, cmd->z, cmd->yOffset);
        return 1;
    }
    return 0;
}

/* Script query: returns the distance from the swarm with the given tag to the command's object (op 0). */
s32 func_1509DEC4(s32 tag, s32 op, SwarmTargetCmd *cmd) {
    if (op == 0) {
        s32 mask = ~0x2000; // matching: the inline constant schedules differently
        return func_15178C9C(tag & 0xFF, func_15084000(cmd->target & mask));
    }
    return 0;
}
