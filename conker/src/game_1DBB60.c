#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Ground probe result filled in by func_15045800 (same layout as game_1D6E80.c's GroundHit). */
typedef struct GroundHit {
    /* 0x00 */ f32 height;
    /* 0x04 */ s16 points[9];
    /* 0x18 */ s32 object;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 active;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 unk20;
} GroundHit; // size 0x24

/* Callback block attached to an actor (same layout as game_1D6E80.c's ActorTask). */
typedef struct ActorTask {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0xB];
    /* 0x18 */ struct127 *actor;
    /* 0x1C */ u8 pad1C[0x18];
    /* 0x34 */ GroundHit hit;
} ActorTask;

extern s32 func_15045800(f32 *pos, s32 arg1, f32 minY, GroundHit *hit);
extern s32 func_151ABE40(f32 *pos, GroundHit *hit, s32 kind, s32 arg3, s32 arg4);
void func_10010FFC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, struct127 *arg5);

/* Ground effect 5 under the task's actor, with sound 0x5208. */
void func_151AE6B0(ActorTask *task) {
    struct127 *actor = task->actor;

    {
        f32 pos[3];
        GroundHit *hit;

        pos[0] = actor->x_position;
        if (-9500.0f < actor->unk118) {
            pos[1] = actor->unk118 + 100.0f;
        } else {
            pos[1] = actor->y_position + 150.0f;
        }
        pos[2] = actor->z_position;
        hit = &task->hit;
        if (func_15045800(pos, 0, pos[1] - 300.0f, hit) != 0) {
            pos[1] = task->hit.height;
            func_151ABE40(pos, hit, 5, task->unkC, task->unk1);
            func_10010FFC(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
}
