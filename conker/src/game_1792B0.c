#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Particle instance; func_1514BC08 keeps its wobble state at + 0x110. */
typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ u8 wobble[0x50];
} Particle1792B0;

s32 func_1514BC08(void *particle, void *wobble);

/* Eases the particle toward random wobble targets kept in its state at + 0x110. */
void func_1514BE00(Particle1792B0 *p) {
    func_1514BC08(p, p->wobble);
}
