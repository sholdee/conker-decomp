#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void actor_setDefaultSizes(void) {
    Actor *tmp;
    s32 i;

    for (i = 0; i < gPlayerCount; i++) {
        tmp = &movement_obj[i];
        tmp->mv->height = 180;
        tmp->mv->width = 60;
        tmp->mv->depth = 60;
    }
}
