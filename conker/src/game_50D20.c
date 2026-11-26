#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Cutscene start callback 1 (D_800840FC): when map 0xB starts cutscene 2, track 5 targets the
   actor of path `actorPath` (its unique id). */
void func_15023870(s32 map, s32 cutscene, s32 scene, s32 actorPath) {
    Actor *actor;

    if (map == 0xB && cutscene == 2) {
        actor = actor_findOnPath(actorPath);
        if (actor != NULL) {
            D_800C35F0[scene][5].targetId = actor->unique_id;
        }
    }
}
