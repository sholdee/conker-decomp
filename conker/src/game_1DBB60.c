#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* ActorTask callback (see game_1D6E80.c; only the task's CharacterWatcher is read here): ground effect 5
 * under the task's actor, with sound 0x5208. */
void func_151AE6B0(CharacterWatcher *task) {
    Actor *actor = task->character;

    {
        f32 pos[3];
        CollisionHit *hit;

        pos[0] = actor->xpos;
        if (-9500.0f < actor->unk118) {
            pos[1] = actor->unk118 + 100.0f;
        } else {
            pos[1] = actor->ypos + 150.0f;
        }
        pos[2] = actor->zpos;
        hit = &task->hit;
        if (func_15045800(pos, 0, pos[1] - 300.0f, hit) != 0) {
            pos[1] = task->hit.height;
            func_151ABE40(pos, hit, 5, task->header.owner, task->header.kind);
            func_10010FFC(0, 0x11, 0x5208, 0, 0, actor);
        }
    }
}
