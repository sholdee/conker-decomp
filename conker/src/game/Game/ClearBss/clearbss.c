#include <ultra64.h>

#include "conker/audio.h"
#include "conker/boot.h"
#include "conker/collision.h"
#include "conker/game.h"


void game_clearBss(void) {
    // zero out bss (?)
    bzero(&gTriArenaCursor, (s32)&D_800E9D00 - (s32)&gTriArenaCursor);
    // zero out (?)
    bzero(D_80044B20, 0x3E000); // 253952 bytes
}
