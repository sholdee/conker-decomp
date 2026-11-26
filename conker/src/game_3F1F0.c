#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_15011D40(void) {
    sound_clearCooldowns();
}

s32 func_15011D60(TriggerZone *zone) {
    StageState tmp;
    u8 pad[0xC]; // matching: stack space
    TriggerZone key;
    TriggerZone *found;
    TimedEffect *effect;
    u8 flag;

    bzero(&tmp, sizeof(tmp));
    tmp.stage = 0;
    tmp.stageTimer = (func_150ADA20() % 0x79U) + 0x12C;
    tmp.zone = zone;

    tmp.set = zone->data2;
    if (tmp.set >= 4) {
        flag = 1;
    } else {
        flag = 0;
    }
    tmp.mirror = flag;
    {
        u8 i;
        for (i = 0; i < 8; i++) {
            tmp.parts[i] = gfx_findObjectById(D_800A1C00[i][tmp.set]);
        }
        tmp.eventTimer = 0;
        tmp.unk2C = 0.0f;
        tmp.unk30 = 0.0f;
        tmp.unk34 = 0.0f;
        tmp.unk38 = 0.0f;
    }
    key.kind = 3;
    key.subTypeData = 0x15;
    key.data1 = tmp.set;
    found = func_151438D8(0, gZoneCount, 0x11A0, &key);
    if (found != NULL) {
        tmp.centerX = found->x;
        tmp.centerZ = found->z;
        tmp.radius = found->xSize;
    } else {
        tmp.radius = 10.0f;
    }
    effect = effect_newTimedEffect(0x12C, -1, -1, -1, 0, 0x38, sizeof(tmp), 0xFF, 1);
    if (effect != NULL) {
        memcpy(effect + 1, &tmp, sizeof(tmp));
    }
    return 1;
}
