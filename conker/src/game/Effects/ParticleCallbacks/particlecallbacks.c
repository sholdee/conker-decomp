#include <ultra64.h>
#include "conker/camera.h"
#include "conker/effect.h"


/* Near-miss at 160/6300; see tools/nearmiss/func_150C7670.c */
typedef struct { u8 pad00[0x14]; u8 *unk14; } Mid150C7670;
struct Obj150C7670 {
    u8 pad00[0x70];
    u8 unk70;
    u8 pad71[0xFF];
    Mid150C7670 *unk170;
};

s32 func_150C7670(Obj150C7670 *particle) {
    f32 temp;

    temp = particle->unk170->unk14[0x2F];
    {
        // matching: a block-scope local for the factor lets IDO hoist the `return 1` (li v0) to the top
        f32 scale = 2.2970001f;

        particle->unk70 = temp * scale;
    }
    return 1;
}



s32 effect_setParticleIdentityMtx(void *mtx, s32 particle) {
    memcpy(mtx, &gIdentityMtx, sizeof(Mtx));
    return 1;
}
