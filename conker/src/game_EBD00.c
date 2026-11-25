#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_1509BE40(s32, ...);
void func_151254F4(struct108 *arg0, s32 arg1);
void func_15123070(struct108 *arg0);

// While the values queried through func_1509BE40 allow it, sets 0x400E to 1 and resets the
// actor's unk348/unk34C/unk374; otherwise sets 0x400E back to 0 once func_151239CC(actor, 3) succeeds.
void func_150BE850(struct108 *actor) {
    s32 flagValue;
    s32 flagSet;

    flagSet = func_1509BE40(0, func_1509BE40(0, 0x2006, 0xB7) | 0x2000, 0xBC);
    flagValue = func_1509BE40(0, 0x2000, 0xBB);
    if (flagSet && flagValue != -1) {
        func_1509BFB0(0, 0x400E, 1);
        if (actor->unk2C != 8) {
            func_1512868C(actor);
        }
        if (func_15123934(actor, 8, 0, actor->unk134, 3)) {
            func_151254F4(actor, D_800CC335 - 1);
            actor->unk84 |= 0x1000200;
            actor->unk73C = 0;
            actor->unk84 &= ~4;
            actor->unk674 = 0.0f;
        }
        actor->unk374 = 778.0f;
        actor->unk348 = 575.0f;
        actor->unk34C = 575.0f;
        actor->unk190 = 0.0f;
        func_15123070(actor);
    } else if (func_151239CC(actor, 3)) {
        func_151254F4(actor, 0);
        actor->unk674 = 0.0f;
        func_1509BFB0(0, 0x400E, 0);
    }
}
