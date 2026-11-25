#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15016370(void) {
    Actor *tmp;
    s32 i;

    for (i = 0; i < D_8008FD8C; i++) {
        tmp = &movement_obj[i];
        tmp->unk31C->height = 180;
        tmp->unk31C->width = 60;
        tmp->unk31C->depth = 60;
    }
}
