#include <ultra64.h>

#include "conker/game.h"
#include "conker/level.h"


u8 level_getEffectKind(void) {
    switch(level)
    {
        case LEVEL_W1_BULL_PADDOCK:
        case LEVEL_W1_POO_CABIN:
        case LEVEL_W1_U_BEND:
            return 2;
        case LEVEL_W2_TANKTUNNEL:
            return 6;
        default:
            return 5;
    }
}
