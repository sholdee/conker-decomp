#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct GameCommand {
    s16 opcode;
    s16 value;
} GameCommand;

GameCommand *func_150341BC(GameCommand *, s32);
GameCommand *func_15034340(GameCommand *, s32);
GameCommand *func_150343B0(GameCommand *, s32);
GameCommand *func_15034420(GameCommand *, s32);
s16 *func_150344A0(s16 *, s32);
void *func_150BE438(void *, s32);
GameCommand *func_150B58F0(GameCommand *, s32);
GameCommand *func_150F1B48(GameCommand *, s32);
GameCommand *func_150D8590(GameCommand *, s32);
s32 func_1503DA3C(s32, s32);
s16 *func_15034860(GameCommand *, struct127 *, s32, s32);

/* Builds the command list for actor `index`, dispatching on its id, and terminates it with 0x1000. */
void func_15033FE0(GameCommand *commands, s32 index) {
    struct127 *actor;
    s32 first;
    GameCommand *cursor;

    actor = &D_800CC2D0[index];
    cursor = commands;
    if (actor->unk1D4 == NULL) {
        commands->opcode = 0x1000;
        return;
    }
    if (actor->interaction_state == 1) {
        cursor = func_150341BC(commands, index);
    } else {
        switch (actor->id) {
        case 0x2B:
            cursor = func_150D8590(commands, index);
            break;
        case 0x28:
        case 0x77:
            cursor = (GameCommand *)func_150344A0(&commands->opcode, index);
            break;
        case 0x4B:
            cursor = func_150B58F0(commands, index);
            break;
        case 0x7B:
            cursor = func_150F1B48(commands, index);
            break;
        case 0x8A:
            cursor = func_15034420(commands, index);
            break;
        case 0x21:
            cursor = func_150BE438(commands, index);
            break;
        case 0x98:
            cursor = func_15034340(commands, index);
            break;
        case 0x5A:
        case 0x74:
        case 0x7A:
        case 0x8D:
            cursor = func_150343B0(commands, index);
            break;
        }
    }
    first = func_1503DA3C(actor->id, 0);
    *func_15034860(cursor, actor, first, func_1503DA3C(actor->id, 1)) = 0x1000;
}

/* Appends the commands for an actor in interaction state 1. Angles are in degrees, scaled to 65536/360 units. */
GameCommand *func_150341BC(GameCommand *cmd, s32 index) {
    struct127 *actor;
    ActorCtl *sub;
    s16 angle;
    s32 kind;
    s32 offset;

    actor = &D_800CC2D0[index];
    sub = actor->unk31C;
    if ((sub != NULL) && !(actor->unk2F8 & 0x80)) {
        // pad54 is declared u32 in structs.h but holds an f32 here
        angle = *(f32 *)&actor->pad54 * (65536.0f / 360.0f);
        kind = actor->unk84.uh;
        if ((kind != 0x42) && (kind != 0x14) && (kind != 0x76)) {
            offset = (sub->unk78 == 10) ? 0 : 4;
            cmd->opcode = offset + 6;
            cmd->value = angle;
            cmd++;
        } else {
            cmd->opcode = 10;
            cmd->value = angle;
            cmd++;
            cmd->opcode = 4;
            cmd->value = -angle;
        // matching: an unused label stops IDO from merging the two branches' increments
        lbl:
            cmd++;
        }
    }
    if (actor->unk1D1 != 0) {
        cmd->opcode = 6;
        cmd++;
        cmd[-1].value = actor->unk1D1 * 200;
    }
    if ((D_800CC2D0[D_800C3E78].unk31C != NULL) && (D_800CC2D0[D_800C3E78].unk31C->unk12 != 0)) {
        cmd->opcode = 8;
        cmd++;
        cmd[-1].value = -D_800CC2D0[D_800C3E78].unk31C->unk12;
    }
    return cmd;
}

/* Appends an opcode-6 command scaled from unk1D1, if it is set. */
GameCommand *func_15034340(GameCommand *cmd, s32 index) {
    struct127 *actor = &D_800CC2D0[index];

    if (actor->unk1D1 != 0) {
        cmd->opcode = 6;
        cmd++;
        cmd[-1].value = actor->unk1D1 * 200;
    }
    return cmd;
}

/* Identical to func_15034340. */
GameCommand *func_150343B0(GameCommand *cmd, s32 index) {
    struct127 *actor = &D_800CC2D0[index];

    if (actor->unk1D1 != 0) {
        cmd->opcode = 6;
        cmd++;
        cmd[-1].value = actor->unk1D1 * 200;
    }
    return cmd;
}

/* Converts unk2E4 (tenths of a degree) to angle units and splits it across an opcode-0 and an opcode-0x18 command. */
GameCommand *func_15034420(GameCommand *cmd, s32 index) {
    s16 part;
    s16 value;

    value = (f32)D_800CC2D0[index].unk2E4 * (65536.0f / 3600.0f);
    part = value >> 4;
    value -= part;
    cmd[0].value = part;
    cmd[1].value = value;
    cmd[0].opcode = 0;
    cmd[1].opcode = 0x18;
    return cmd + 2;
}
