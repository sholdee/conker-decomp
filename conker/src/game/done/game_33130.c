#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005C80(Camera *arg0) {
    arg0->mode = 8;
    arg0->flags |= 0x1300000;
    arg0->preset = 1;
    arg0->presetMask = 2;
    arg0->height = 117.0f;
    arg0->unk34C = 117.0f;
    arg0->liftTarget = 0.0f;
    arg0->lift = 0.0f;
    arg0->unk134 = 0;
    arg0->status |= 0x10;
    arg0->snap = 1;
    arg0->targetDistance = 370.0f;
}

void func_15005CF0(s32 arg0) {
}
