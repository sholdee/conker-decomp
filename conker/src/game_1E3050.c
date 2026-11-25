#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void *func_151B4FE0(Actor *actor, s32 arg1, s32 arg2);

/* When actor 0x53 spawns a 0x16 actor, attaches an emitter (func_151B4FE0) to it. */
void func_151B5BA0(Actor *spawner, Actor *spawned) {
    if (spawner->character_type == 0x53) {
        if (spawned->character_type == 0x16) {
            func_151B4FE0(spawned, 0xFF, 1);
        }
    }
}
