#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    u8 unk0[5];
} Struct15100340;

/* This file's .data: the light ids of the five gFlagLightStates flags. */
static Struct15100340 gFlagLightEventIds = { { 9, 10, 11, 12, 13 } };

/* When arg0 is 0: advances gLevelTexFrame, then randomly toggles each of the five gFlagLightStates flags,
 * passing the matching gFlagLightEventIds id to light_sendEvent on each toggle. */
void effect_levelUpdateFlagLights(s32 arg0) {
    s32 pad; // matching: unused stack slot, sets the frame size
    Struct15100340 ids = gFlagLightEventIds;
    s32 i;
    u8 *flag;
    u32 chance;

    if (arg0 == 0) {
        flag = gFlagLightStates;
        gLevelTexFrame++;
        for (i = 0; i < 5; i++, flag++) {
            chance = (*flag != 0) ? 70 : 35;
            if (math_random() % 1000U < chance) {
                if ((*flag ^= 1) != 0) {
                    light_sendEvent(ids.unk0[i], 0x18, 0);
                } else {
                    light_sendEvent(ids.unk0[i], 0x1C, 0);
                    light_sendEvent(ids.unk0[i], 0x17, 0);
                }
            }
        }
    }
}

/* Sets segments 2 and 3 to D_80090324 assets cycling with gLevelTexFrame, and segments 4-8 to
 * D_80090324[3] or [4] depending on each gFlagLightStates flag. */
Gfx *effect_bindFlagLightTextures(Gfx *gdl) {
    s32 size[4]; // matching: a 16-byte buffer sets the frame size; a scalar mismatches
    s32 frame;
    s32 i;
    s32 addr;

    frame = gLevelTexFrame >> 2;

    addr = texture_getAddress(D_80090324[frame % 3], size, 3, 0);
    gSPSegment(gdl++, 2, addr);

    addr = texture_getAddress(D_80090324[(frame + 1) % 3], size, 3, 0);
    gSPSegment(gdl++, 3, addr);

    for (i = 0; i < 5; i++) {
        addr = texture_getAddress(D_80090324[gFlagLightStates[i] + 3], size, 3, 0);
        gSPSegment(gdl++, i + 4, addr);
    }

    return gdl;
}
