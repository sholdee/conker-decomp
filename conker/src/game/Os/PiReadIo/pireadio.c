#include <ultra64.h>
#include <math.h>
#include "conker/os.h"
#include "piint.h"

/* osPiReadIo.  `register` is load-bearing and only bites at -O1: without it `status` spills to
 * the stack (frame 0x20, no $s0, `addiu $sp` left out of the jr delay slot) and the function
 * is one instruction short at every flag setting.  With it, at -O1, both functions here are
 * byte-perfect -- which also puts this TU in the same -O1 family as the game's other private
 * libultra copies (game/Os/ContInit/continit, game/Os/ViSetSpecial/visetspecial, game/Math/Random/random). */
s32 osPiReadIo(u32 devAddr, u32 *data) {
    register s32 status;

    __osPiGetAccess();
    status = osPiRawReadIo(devAddr, data);
    __osPiRelAccess();
    return status;
}

f32 func_151EF080(f32 arg0) {
    return sqrtf(arg0);
}
