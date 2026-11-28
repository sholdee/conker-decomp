#include <ultra64.h>

#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/gfx.h"
#include "conker/level.h"
#include "conker/sound.h"
#include "conker/zone.h"

void level_setupMainWar(void) {
    sound_clearCooldowns();
}

s32 level_spawnStageZone(TriggerZone *zone) {
    StageState tmp;
    u8 pad[0xC]; // matching: stack space
    TriggerZone key;
    TriggerZone *found;
    TimedEffect *effect;
    u8 flag;

    bzero(&tmp, sizeof(tmp));
    tmp.stage = 0;
    tmp.stageTimer = (math_random() % 0x79U) + 0x12C;
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
            tmp.parts[i] = gfx_findObjectById(gStagePartObjectIds[i][tmp.set]);
        }
        tmp.eventTimer = 0;
        tmp.kindSpriteAccum = 0.0f;
        tmp.flareAccum = 0.0f;
        tmp.debrisAccum = 0.0f;
        tmp.burstAccum = 0.0f;
    }
    key.kind = 3;
    key.subTypeData = 0x15;
    key.data1 = tmp.set;
    found = math_findMatchingZone(0, gZoneCount, 0x11A0, &key);
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
