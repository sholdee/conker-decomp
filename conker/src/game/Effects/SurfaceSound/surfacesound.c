#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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
        if (D_800BE9E4 < gSurfaceSoundTimer){
            if(gCutsceneStates[0] != 1) {
                gSurfaceSoundTimer = (gSurfaceSoundTimer - D_800BE9E4);
            }
        } else {
            gSurfaceSoundTimer = ((func_150ADA20() & 0x3F) + 0xC0);
            sound_play(15, 18000, ((func_150ADA20() & 0x3F) + 0x20), 0, 0);
        }
    }
}
