#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15012020(s32 arg0, s32 arg1) {
    EmitterDef tmp;

    tmp.flags = 7;
    tmp.anchor = (TriggerZone *)arg0;
    tmp.rate = D_80096530;
    tmp.rateRange = D_80096534;
    tmp.pos.x = 0.0f;
    tmp.pos.y = 0.0f;
    tmp.pos.z = 0.0f;
    tmp.bottom = 0.0f;
    tmp.radius = 0.0f;
    tmp.type = 0;
    tmp.update = -1;
    tmp.spawn = 3;

    func_151A8B20(&tmp, -1, 0, 0xFF, 0);
}
