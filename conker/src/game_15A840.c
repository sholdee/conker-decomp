#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Camera block at Camera + 0x69C (Camera only pads this range). */
typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 counter;
    /* 0x28 */ f32 orbitSpeed; /* degrees per frame, clamped to +-10 */
} CamOrbitState;

void func_1508EF80(struct17 *point, struct17 *pivot, f32 degrees, struct17 *out);

/* Manual camera orbit. While input bit 0 or 1 is held (and bit 21 of unk84 is clear), orbitSpeed
   accelerates by 0.5 per frame in that direction, clamped to +-10, and the eye (unk2F8) orbits around
   unk2A4 by orbitSpeed / 2.5 degrees per frame. On release orbitSpeed decays by 20% per frame. */
void func_1512D390(Camera *camera) {
    CamOrbitState *orbit;
    f32 speed;
    f32 clamped;
    u16 input;

    input = camera->contPad->button;
    orbit = (CamOrbitState *)camera->pad69C;
    if ((input & 3) && !(camera->flags & 0x200000)) {
        if (camera->unk698 != 0) {
            return;
        }
        if (input & 1) {
            camera->unk6B0 = 1; /* orbit direction */
        } else {
            camera->unk6B0 = -1;
        }
        orbit->counter++;
        orbit->orbitSpeed += (f32)camera->unk6B0 * 0.5f * D_800BE9A4;
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
        if (camera->mode & 0x400) {
            orbit->counter = 20;
            return;
        }
        func_1508EF80((struct17 *)&camera->eye.x, (struct17 *)&camera->focus.x,
                      (orbit->orbitSpeed * D_800BE9A4) / 2.5f, (struct17 *)&camera->eye.x);
    } else if (orbit->orbitSpeed != 0.0f) {
        orbit->orbitSpeed -= orbit->orbitSpeed * 0.2f * D_800BE9A4;
        func_1508EF80((struct17 *)&camera->eye.x, (struct17 *)&camera->focus.x,
                      (orbit->orbitSpeed * D_800BE9A4) / 2.5f, (struct17 *)&camera->eye.x);
    }
}
