#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_1509BE40(s32, s32, s32, ...);
void func_151254F4(Camera *actor, s32 arg1);
extern s32 D_80088A90;

/*
 * When both func_1509BE40 queries succeed, switch flags 0x405D-0x4061 on and (unless unk2C is 0x100
 * or unk6C8 is set) put the actor into its alternate state and set D_80088A90. Otherwise leave that
 * state, switch the flags off again and mirror query 0x4040 into bit 0x80 of unk84.
 */
void func_150EB1C0(Camera *actor)
{
    s32 firstResult;
    s32 secondResult;

    firstResult = func_1509BE40(0, func_1509BE40(0, 0x2006, 0xB7) | 0x2000, 0xBC);
    secondResult = func_1509BE40(0, 0x2000, 0xBB);
    if (firstResult != 0 && secondResult != 0) {
        func_1509BFB0(0, 0x405D, 1);
        func_1509BFB0(0, 0x405E, 1);
        func_1509BFB0(0, 0x405F, 1);
        func_1509BFB0(0, 0x4060, 1);
        func_1509BFB0(0, 0x4061, 1);
        if (actor->mode != 0x100 && actor->cmdRunning == 0) {
            if (func_15123934(actor, 8, 0, 0, 3)) {
                actor->flags |= 0x01100004;
                actor->flags &= ~2;
                actor->followRateTarget = 0.0f;
                func_151254F4(actor, actor->player->unk65 - 1);
                actor->status |= 0x10;
            }
            actor->targetDistance = 550.0f;
            actor->unk134 = 0;
            actor->height = actor->unk34C = 320.0f;
            actor->liftTarget = 180.0f;
            D_80088A90 = 1;
            func_1509BFB0(0, 0x405C, 0);
        }
    } else {
        if (func_151239CC(actor, 3)) {
            actor->liftTarget = 0.0f;
            func_151254F4(actor, actor->playerIndex);
            actor->status &= ~0x10;
            actor->followRateTarget = 0.0f;
            D_80088A90 = 0;
        }
        func_1509BFB0(0, 0x405C, 1);
        func_1509BFB0(0, 0x405D, 0);
        func_1509BFB0(0, 0x405E, 0);
        func_1509BFB0(0, 0x405F, 0);
        func_1509BFB0(0, 0x4060, 0);
        func_1509BFB0(0, 0x4061, 0);
        if (func_1509BE40(1, 0x4040, 6, 0x9000)) {
            actor->flags |= 0x80;
        } else {
            actor->flags &= ~0x80;
        }
    }
}
