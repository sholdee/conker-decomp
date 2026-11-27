#include <ultra64.h>

#include "effects.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/flamethrower.h"
#include "conker/level.h"
#include "conker/light.h"
#include "conker/texture.h"

/* Timed effect (expire callback 5, particle_emitDiscParticle) carrying a DiscEmitter at 0x28. */
typedef struct {
    /* 0x00 */ TimedEffect base;
    /* 0x28 */ DiscEmitter data;
} Effect1500EF20;

void level_setup05(void) {
    Effect1500EF20 *effect;
    DiscEmitter tmp;

    flamethrower_addTexScroll(gLevelDisplayLists[0], D_800902B8, 1, -1, 0, 0, 0, -8);
    flamethrower_addTexScroll(gLevelDisplayLists[1], D_800902B8, 1, -1, 0, 1, 0, -8);

    tmp.x = 9776.0f;
    tmp.z = -1770.0f;
    tmp.y = -1278.0f;
    tmp.radius = 400.0f;
    effect = effect_newTimedEffectNoDraw((math_random() % 0x33U) + 0x19, 5, -1, 1, 0, 0x10, 0xFF, 0);
    if (effect != NULL) {
        memcpy(&effect->data, &tmp, sizeof(tmp));
    }

    tmp.x = 7207.0f;
    tmp.z = -1412.0f;
    tmp.y = -1733.0f;
    effect = effect_newTimedEffectNoDraw((math_random() % 0x33U) + 0x19, 5, -1, 1, 0, 0x10, 0xFF, 0);
    if (effect != NULL) {
        memcpy(&effect->data, &tmp, sizeof(tmp));
    }

    tmp.z = 1412.0f;
    effect = effect_newTimedEffectNoDraw((math_random() % 0x33U) + 0x19, 5, -1, 1, 0, 0x10, 0xFF, 0);
    if (effect != NULL) {
        memcpy(&effect->data, &tmp, sizeof(tmp));
    }

    tmp.x = 9776.0f;
    tmp.z = 1770.0f;
    tmp.y = -1278.0f;
    effect = effect_newTimedEffectNoDraw((math_random() % 0x33U) + 0x19, 5, -1, 1, 0, 0x10, 0xFF, 0);
    if (effect != NULL) {
        memcpy(&effect->data, &tmp, sizeof(tmp));
    }

    gAmbientEaseRed = gAmbientColour[0];
    gAmbientEaseGreen = gAmbientColour[1];
    gAmbientEaseBlue = gAmbientColour[2];
    gAmbientEasePhase = 0.0f;
}
