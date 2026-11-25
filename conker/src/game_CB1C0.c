#include <ultra64.h>
/* functions.h declares this with a signature that doesn't match its real one; rename it out of the way. */
#define func_15178E14 func_15178E14_header_mismatch
#include "functions.h"
#undef func_15178E14
#include "variables.h"

s32 func_15084000(s32);
void func_15177F90(u8 memberCount, f32 *targetPos, u8 tag, s32 x, s32 y, s32 z, s32 arg6, s32 arg7, u8 arg8,
                   s32 playSfx, s32 argA);
void func_15178BE4(u8 tag, f32 *targetPos, s16 yOffset);
void func_15178C34(u8 tag, s32 x, s32 y, s32 z, s16 yOffset);
s32 func_15178C9C(u8 tag, s32 index);
void func_15178E14(u8 tag);

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
s32 func_1509DD50(s32 arg0, struct214 *cmd) {
    func_15177F90(cmd->unk7, NULL, cmd->unkB, cmd->unkC, cmd->unk10, cmd->unk14, cmd->unk18, 0x7FFF, 0xFF, cmd->unk1C,
                  0);
    return 1;
}

/* Script handler: destroys the swarm with the given tag. */
s32 func_1509DDC4(s32 tag, u16 arg1) {
    arg1 = tag; // matching: IDO copies tag through the arg1 register only via this reassignment
    func_15178E14(arg1);
    return 0;
}

/* Script handler: makes the swarm with the given tag follow an object (op 0) or head for a fixed point (op 1). */
s32 func_1509DDFC(s32 tag, s32 op, struct215 *cmd) {
    switch (op) {
    case 0: {
        s32 mask = ~0x2000; // matching: the inline constant schedules differently
        func_15178BE4(tag & 0xFF, &D_800CC2D0[func_15084000(cmd->unk8 & mask)].x_position, cmd->unkC.s.unk2);
        return 1;
    }
    case 1:
        func_15178C34(tag & 0xFF, cmd->unk8, cmd->unkC.i.unk0, cmd->unk10, cmd->unk14);
        return 1;
    }
    return 0;
}

/* Script query: returns the distance from the swarm with the given tag to the command's object (op 0). */
s32 func_1509DEC4(s32 tag, s32 op, struct215 *cmd) {
    if (op == 0) {
        s32 mask = ~0x2000; // matching: the inline constant schedules differently
        return func_15178C9C(tag & 0xFF, func_15084000(cmd->unk8 & mask));
    }
    return 0;
}
