#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Descriptor passed to func_15134DAC (same layout as game_1228D0.c's struct_150F6890). */
typedef struct {
    /* 0x00 */ u8  ownerId;
    /* 0x01 */ u8  pad1[0x3];
    /* 0x04 */ Actor *owner;
    /* 0x08 */ u8  unk8;
    /* 0x09 */ u8  pad9[0x3];
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8  unk24;
    /* 0x25 */ u8  pad25;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 duration;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s8  unk2E;
    /* 0x2F */ u8  unk2F;
    /* 0x30 */ s8  unk30;
    /* 0x31 */ u8  pad31[0x3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ s8  unk39;
} struct_E9FE0_2F8;

void *func_15134DAC(struct_E9FE0_2F8 *, s32);
void func_1516962C(s32, Actor *, s32);

/*
 * Attaches two mirrored func_15134DAC effects (unk8 13 and 12) to `owner`, lasting `duration`,
 * or 300 when `duration` is 0.
 */
void func_150C7020(Actor *owner, s16 duration, s32 arg2, s32 arg3) {
    struct_E9FE0_2F8 desc;

    if (owner == NULL) {
        return;
    }

    func_1516962C(0x29, owner, 0x12);

    desc.ownerId = owner->unique_id;
    desc.owner = owner;
    desc.unk8 = 13;
    desc.unkC = 2.0f;
    desc.unk10 = 0.0f;
    desc.unk14 = 34.0f;
    desc.unk18 = -15.0f;
    desc.unk1C = 0.0f;
    desc.unk20 = 34.0f;
    desc.unk24 = 0;
    desc.unk2E = -1;
    desc.unk2F = 1;
    desc.unk30 = -1;
    desc.unk34 = 0.4720000327f;
    desc.unk38 = 2;
    desc.unk39 = -1;

    if (duration != 0) {
        desc.duration = duration;
        desc.unk24 = 2;
    } else {
        desc.duration = 300;
    }

    desc.unk26 = func_150ADA20() % 66U + 35;
    desc.unk28 = func_150ADA20() % 26U + 25;
    func_15134DAC(&desc, 0);

    desc.unk8 = 12;
    desc.unkC = -2.0f;
    desc.unk10 = 0.0f;
    desc.unk14 = 34.0f;
    desc.unk18 = 15.0f;
    desc.unk1C = 0.0f;
    desc.unk20 = 34.0f;

    desc.unk26 = func_150ADA20() % 66U + 35;
    desc.unk28 = func_150ADA20() % 26U + 25;
    func_15134DAC(&desc, 0);
}

/* Partial view of the task passed to func_150C71C0 (cf. game_1D6E80.c's Task151AA30C). */
typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
} Task150C71C0;

void func_151DB5D0(s32, f32 *pos, f32 *vel, f32, f32, f32, f32, s32, s32, s32, s32, s32, s32, s32);

/* Spawns one func_151DB5D0 particle at `from`, moving towards `to` with randomized size and lifetime. */
void func_150C71C0(f32 *from, f32 *to, s32 arg2, s32 arg3, s32 arg4, Task150C71C0 *task) {
    f32 vel[3];
    s16 fadeTime;
    s32 lifetime;
    f32 rand2;
    f32 rand1;
    f32 rand0;

    vel[0] = (to[0] - from[0]) * 0.407f;
    vel[1] = (to[1] - from[1]) * 0.407f;
    vel[2] = (to[2] - from[2]) * 0.407f;
    fadeTime = func_150ADA20() % 31U + 42;
    rand0 = func_150ADA68();
    rand1 = func_150ADA68();
    rand2 = func_150ADA68();
    lifetime = func_150ADA20() % 101U + 155;
    func_151DB5D0(0, from, vel, rand0 * 70.0f + 30.0f, rand1 * 0.020000001f + 1.0400001f, 1.0f,
                  rand2 * 0.194f + -0.35000002f, fadeTime, lifetime, fadeTime, 255 / fadeTime, 1, task->unkC,
                  task->unk1);
}
