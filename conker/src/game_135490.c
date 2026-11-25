#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_1509BE40();
void func_15123070(Camera *arg0);

void func_15107FE0(Camera *arg0) {
    if (func_150859AC(arg0->player->playernumber, 1) == 9) {
        func_151239CC(arg0, 2);
        if (func_15123934(arg0, 8, 0, arg0->unk134, 3) != 0) {
            arg0->aimState = 0;
            arg0->flags |= 0x1300000;
            arg0->flags &= -5;
            func_15123070(arg0);
        }
        arg0->height = 150.0f;
        arg0->unk34C = 150.0f;
        arg0->targetDistance = 340.0f;
        arg0->status |= 0x800;
        arg0->liftTarget = -6.0f;
    } else {
        arg0->status &= ~0x800;
    }
    if (func_1509BE40(1, 0x401A, 6, 0x9000) || func_1509BE40(1, 0x401B, 6, 0x9000)) {
        arg0->flags |= 0x10000;
    } else {
        arg0->flags &= 0xFFFEFFFF;
    }
}
