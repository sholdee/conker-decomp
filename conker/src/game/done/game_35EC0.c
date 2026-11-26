#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15008A10(void) {
    D_80088720 = collision_findObjectById(1);
    D_80088724.x = 0.0f;
    D_80088724.y = 0.0f;
    D_80088724.z = 0.0f;
    D_80088730 = collision_findObjectById(1);
}

/* Resets the screen effects of every view: the tint, the colour fades, the irises and the letterbox. */
void screen_resetEffects(void) {
    s32 i;

    gTintFrames = 0;
    gCutsceneIrisOn = 0;
    for (i = 0; i < 4; i++) {
        gFadeDirections[i] = 0;
        gFadeDurations[i] = 0;
        gFadeElapsed[i] = 0;
        gIrisStyles[i] = 0;
        gIrisScripted[i] = 0;
        gIrisClosures[i] = 0.0f;
        gIrisSpeeds[i] = 0.0f;
        gIrisTracking[i] = 0;
    }
    gLetterbox = NULL;
    D_800DDE38 = 0;
}
