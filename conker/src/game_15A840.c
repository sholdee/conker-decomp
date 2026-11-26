#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Camera.orbitCounter / Camera.orbitSpeed seen from Camera + 0x69C.
   matching: the original reaches them through one base pointer; plain camera->orbitSpeed is 0x10 bytes shorter. */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 counter;
    /* 0x28 */ f32 orbitSpeed; /* degrees per frame, clamped to +-10 */
} CamOrbitState;


/* Manual camera orbit. While input bit 0 or 1 is held (and bit 21 of flags is clear), orbitSpeed
   accelerates by 0.5 per frame in that direction, clamped to +-10, and the eye orbits around
   the focus by orbitSpeed / 2.5 degrees per frame. On release orbitSpeed decays by 20% per frame. */
void func_1512D390(Camera *camera) {
    CamOrbitState *orbit;
    f32 speed;
    f32 clamped;
    u16 input;

    input = camera->PadPtr->button;
    orbit = (CamOrbitState *)camera->pad69C;
    if ((input & 3) && !(camera->AvailableModes & 0x200000)) {
        if (camera->unk698 != 0) {
            return;
        }
        if (input & 1) {
            camera->orbitDir = 1;
        } else {
            camera->orbitDir = -1;
        }
        orbit->counter++;
        orbit->orbitSpeed += camera->orbitDir * 0.5f * D_800BE9A4;
        speed = orbit->orbitSpeed;
        if (speed < -10.0f) {
            orbit->orbitSpeed = -10.0f;
        } else { // matching: a flat else-if / ternary clamp schedules differently
            if (speed > 10.0f) {
                clamped = 10.0f;
            } else {
                clamped = speed;
            }
            orbit->orbitSpeed = clamped;
        }
        if (camera->Mode & 0x400) {
            orbit->counter = 20;
            return;
        }
        func_1508EF80(&camera->eye, &camera->focus,
                      (orbit->orbitSpeed * D_800BE9A4) / 2.5f, &camera->eye);
    } else if (orbit->orbitSpeed != 0.0f) {
        orbit->orbitSpeed -= orbit->orbitSpeed * 0.2f * D_800BE9A4;
        func_1508EF80(&camera->eye, &camera->focus,
                      (orbit->orbitSpeed * D_800BE9A4) / 2.5f, &camera->eye);
    }
}
