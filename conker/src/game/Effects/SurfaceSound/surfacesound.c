#include <ultra64.h>
#include "conker/actor.h"
#include "conker/collision.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/sound.h"

u8 gZoneFlag = 0;
static u8 gSurfaceSoundTimer = 0;


/* Zone handler: sets gZoneFlag. */
void effect_setZoneFlag(Actor *obj, TriggerZone *zone) {
    gZoneFlag = 1;
}

s32 effect_consumeZoneFlag(void) {
    if (gZoneFlag != 0) {
        gZoneFlag = 0;
        return 1;
    } else {
        return 0;
    }
}

void effect_updateSurfaceSound(s32 arg0) {
    if (((movement_obj[0].floorMaterial & 0X1F) == 3) || ((movement_obj[0].floorMaterial & 0x1F) == 4)) {
        if (gFrameTicks < gSurfaceSoundTimer){
            if(gCutsceneStates[0] != 1) {
                gSurfaceSoundTimer = (gSurfaceSoundTimer - gFrameTicks);
            }
        } else {
            gSurfaceSoundTimer = ((math_random() & 0x3F) + 0xC0);
            sound_play(15, 18000, ((math_random() & 0x3F) + 0x20), 0, 0);
        }
    }
}
