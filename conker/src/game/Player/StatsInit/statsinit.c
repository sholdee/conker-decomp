#include <ultra64.h>

#include "conker/game.h"
#include "conker/hud.h"
#include "conker/player.h"


void player_initMatch(s16 mode, s16 playerMask) {
    s32 i;
    s16 tmp;

    tmp = mode;
    bzero(&gMatchState, 524);
    gMatchState.mode = tmp;

    for (i = 0; i < 16; i++) {
        if ((1 << i) & playerMask) {
            player_setStat(i, 0, gDefaultCharacters[i]);
            gHudBlockTarget = gHudBlockCount = 6;
            player_setStat(i, 5, gDefaultHealth);
            player_setStat(i, 2, gDefaultLives);
            if (gIsMultiplayer != 0) {
                player_setStat(i, 9, gDefaultScore);
            }
        }
        func_1501748C(playerMask);
    }
    gMatchState.playerMask = playerMask;
    gCarriedUnderwaterTime = 0;
}

void func_1501748C(s16 playerMask) {
}

void player_clearMatch(void) {
    bzero(&gMatchState, 524);
}
