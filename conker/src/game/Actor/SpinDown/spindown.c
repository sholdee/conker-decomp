#include <ultra64.h>
#include "conker/actor.h"

/* Spins the actor down: tilt and lean advance by their rates, which decay with the speed. */
void actor_updateSpinDown(Actor *arg0) {
    arg0->vtilt += arg0->unkBC;
    arg0->unkBC *= 0.99f;
    arg0->speed *= 0.97f;
    arg0->lean_angle += arg0->unk148;
    arg0->unk148 *= 0.95f;
    actor_moveHorizontal(arg0);
}
