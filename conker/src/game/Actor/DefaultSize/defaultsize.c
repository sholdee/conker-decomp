#include <ultra64.h>

#include "conker/actor.h"
#include "conker/save.h"


void actor_setDefaultSizes(void) {
    Actor *actor;
    s32 i;

    for (i = 0; i < gPlayerCount; i++) {
        actor = &movement_obj[i];
        actor->mv->height = 180;
        actor->mv->width = 60;
        actor->mv->depth = 60;
    }
}
