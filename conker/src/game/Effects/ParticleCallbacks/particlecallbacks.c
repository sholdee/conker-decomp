#include <ultra64.h>
#include "conker/camera.h"
#include "conker/effect.h"


/* The model particle level_spawnLightAlphaModel spawns (update2Fn 6, gModelParticleUpdate2Funcs[6] = effect_setAlphaFromLightRadius). */
struct LightAlphaParticle {
    /* 0x000 */ ModelParticle head;
    /* 0x170 */ LightEffect *lightEffect; /* the particle's data: the LightEffect it was spawned for */
};

s32 effect_setAlphaFromLightRadius(LightAlphaParticle *particle) {
    f32 radius;

    radius = particle->lightEffect->light->radius;
    {
        // matching: a block-scope local for the factor lets IDO hoist the `return 1` (li v0) to the top
        f32 scale = 2.2970001f;

        particle->head.alpha = radius * scale;
    }
    return 1;
}



s32 effect_setParticleIdentityMtx(void *mtx, s32 particle) {
    memcpy(mtx, &gIdentityMtx, sizeof(Mtx));
    return 1;
}
