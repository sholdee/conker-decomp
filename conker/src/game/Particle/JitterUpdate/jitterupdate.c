#include <ultra64.h>
#include "effects.h"
#include "conker/particle.h"

/* Eases the particle toward random wobble targets kept in its state at + 0x110. */
void particle_updateJitterParticle(JitterParticle *p) {
    particle_updateJitter(p, &p->jitter);
}
