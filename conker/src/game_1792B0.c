#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Particle instance; func_1514BC08 keeps its jitter state at + 0x110. */
typedef struct {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ ParticleJitter jitter;
} Particle1792B0;

s32 func_1514BC08(void *particle, ParticleJitter *jitter);

/* Eases the particle toward random wobble targets kept in its state at + 0x110. */
void func_1514BE00(Particle1792B0 *p) {
    func_1514BC08(p, &p->jitter);
}
