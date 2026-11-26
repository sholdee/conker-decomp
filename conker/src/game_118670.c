#include <ultra64.h>
#include "functions.h"
#include "variables.h"

static s32 D_80088A90 = 0; /* 1 while the camera is in the transport mode this hook set */

/*
 * When both script_query queries succeed, switch flags 0x405D-0x4061 on and (unless unk2C is 0x100
 * or unk6C8 is set) put the actor into its alternate state and set D_80088A90. Otherwise leave that
 * state, switch the flags off again and mirror query 0x4040 into bit 0x80 of unk84.
 */
void func_150EB1C0(Camera *actor)
{
    s32 firstResult;
    s32 secondResult;

    firstResult = script_query(0, script_query(0, 0x2006, 0xB7) | 0x2000, 0xBC);
    secondResult = script_query(0, 0x2000, 0xBB);
    if (firstResult != 0 && secondResult != 0) {
        script_command(0, 0x405D, 1);
        script_command(0, 0x405E, 1);
        script_command(0, 0x405F, 1);
        script_command(0, 0x4060, 1);
        script_command(0, 0x4061, 1);
        if (actor->Mode != 0x100 && actor->cmdRunning == 0) {
            if (func_15123934(actor, 8, 0, 0, CAMERA_MODE_TRANSPORT)) {
                actor->AvailableModes |= 0x01100004;
                actor->AvailableModes &= ~2;
                actor->CharTrackingSpeedTarget = 0.0f;
                func_151254F4(actor, actor->CharPtr->attach_char - 1);
                actor->SpecialFlags |= 0x10;
            }
            actor->IdealTripodMagnitude = 550.0f;
            actor->Tightness = 0;
            actor->height = actor->unk34C = 320.0f;
            actor->YOffsetTget = 180.0f;
            D_80088A90 = 1;
            script_command(0, 0x405C, 0);
        }
    } else {
        if (func_151239CC(actor, CAMERA_MODE_TRANSPORT)) {
            actor->YOffsetTget = 0.0f;
            func_151254F4(actor, actor->CNumber);
            actor->SpecialFlags &= ~0x10;
            actor->CharTrackingSpeedTarget = 0.0f;
            D_80088A90 = 0;
        }
        script_command(0, 0x405C, 1);
        script_command(0, 0x405D, 0);
        script_command(0, 0x405E, 0);
        script_command(0, 0x405F, 0);
        script_command(0, 0x4060, 0);
        script_command(0, 0x4061, 0);
        if (script_query(1, 0x4040, 6, 0x9000)) {
            actor->AvailableModes |= 0x80;
        } else {
            actor->AvailableModes &= ~0x80;
        }
    }
}
