#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Eases the particle toward random wobble targets kept in its state at + 0x110. */
void func_1514BE00(Particle1792B0 *p) {
    func_1514BC08(p, &p->jitter);
}
