#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Spins the actor down: tilt and lean advance by their rates, which decay with the speed. */
void func_150D13A0(Actor *arg0) {
    arg0->vtilt += arg0->unkBC;
    arg0->unkBC *= 0.99f;
    arg0->speed *= 0.97f;
    arg0->lean_angle += arg0->unk148;
    arg0->unk148 *= 0.95f;
    actor_moveHorizontal(arg0);
}

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

/*
 * 0.2489f, in the .rodata unit after this one (0x800A08C0): IDO puts a literal of this file into the pool of
 * func_150D13A0's literals (0x800A08B0), so the separate 16-byte block is data of another unit.
 */
extern f32 D_800A08C0;

void func_150D149C(AmbientPulse *arg0) {
    f32 *p = &arg0->red;

    func_151467A4(&arg0->timer, 10.0f, &arg0->target, 50.0f, 100.0f, 123.0f, D_800A08C0, p);
    light_setAmbient(*p, gAmbientColour[1], gAmbientColour[2], 0);
}
