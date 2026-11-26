#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_150D1B40(AmbientPulse *arg0) {
    f32 *p = &arg0->unk28;

    func_151467A4(&arg0->unk30, 10.0f, &arg0->unk2C, 86.0f, 170.0f, 255.0f, 0.2489f, p);
    func_1515D4D4(*p, D_800DCD20[1], D_800DCD20[2], 0);
}


void func_150D1BD0(s32 *arg0) {
    if (func_1509BE40(1, 0x402C, 6, 0x2000) != 0) {
        arg0[0x21] |= 0x10;
    } else {
        arg0[0x21] &= ~0x10;
    }
}
