#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Eases the particle toward random wobble targets kept in its state at + 0x110. */
void particle_updateJitterParticle(JitterParticle *p) {
    particle_updateJitter(p, &p->jitter);
}
