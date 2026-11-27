#include <ultra64.h>

#include "conker/game.h"
#include "conker/hud.h"
#include "conker/player.h"
#include "conker/script.h"


void player_initHudBlockCount(void) {
    if ((gIsMultiplayer == 0) && ((gTaskDoneBits[0x18] & 1) == 0)) {
        gHudBlockTarget = gHudBlockCount = 3;
    } else {
        gHudBlockTarget = gHudBlockCount = 6;
    }
}

void player_resetHudBlocks(void) {
    f32 *p;
    f32 *end;
    s32 i;

    gHudClockShown = 0;
    gHudBlockDl = NULL;

    gHudBlockStackPos[0] = gHudBlockStackPath[0];
    gHudBlockStackPos[1] = gHudBlockStackPath[1];
    gHudBlockStackPos[2] = gHudBlockStackPath[2];

    gHudBlockStackWobble[0] = 0.0f;
    gHudBlockStackWobble[1] = 0.0f;
    gHudBlockStackWobble[2] = 0.0f;

    gHudBlockStackTimer = 0;
    gHudBlockDebris = NULL;

    p = gHudBlockScales, end = &gHudBlockScales[6];
    do {
        *p = 1.0f;
        p++;
    } while (p < end);

    for (i = 0; i < 6; i++) {
        gHudBubbles[i].state = 0;
    }

    gHudBubbleCooldown = 0;
    bzero(&gHudWaterStage, 0xC0);
    guPerspective(&gHudProjection, &gHudPerspNorm, 50.0f, 60.6f, 53.0f, 20000.0f, 1.0f);
}
