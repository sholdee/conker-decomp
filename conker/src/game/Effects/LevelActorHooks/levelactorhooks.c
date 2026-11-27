#include <ultra64.h>
#include "conker/actor.h"
#include "conker/effect.h"
#include "conker/gfx.h"
#include "conker/light.h"

/*
 * The level callbacks that actor_updateAll runs before and after the update of each actor (gLevelPreUpdateFuncs[1]
 * and gLevelPostUpdateFuncs[1] in game/Actor/Movement/movement.c). Only the level header of W1_RAPTOR_NEST selects
 * them. They make world object 0xF9 active only while an actor other than the first one updates.
 */
void func_150D1410(Actor *arg0) {
    WorldObject *temp;

    temp = gfx_findObjectById(0xF9);
    if (temp != 0) {
        if ((arg0 - movement_obj) == 0) {
            temp->inactive = 1;
        } else {
            temp->inactive = 0;
        }
    }
}

void func_150D146C(Actor *arg0) {
    WorldObject *temp;

    temp = gfx_findObjectById(0xF9);
    if (temp != 0) {
        temp->inactive = 1;
    }
}

void effect_updateAmbientPulse(AmbientPulse *arg0) {
    f32 *p = &arg0->red;

    math_randomDrift(&arg0->timer, 10.0f, &arg0->target, 50.0f, 100.0f, 123.0f, 0.2488999963f, p);
    light_setAmbient(*p, gAmbientColour[1], gAmbientColour[2], 0);
}
