#include <ultra64.h>
#include "functions.h"
#include "variables.h"


void func_150D0650(Camera *arg0) {
    if ((gTaskDoneBits[0x0] & 0x80) == 0) {
        if (script_query(0, 0x2000, 0xBB) != -1) {
            if (func_15123934(arg0, arg0->Mode, 0, arg0->Tightness, CAMERA_MODE_PICKED_UP) != 0) {
                arg0->AvailableModes |= 0x1000000;
                func_151254F4(arg0, movement_obj[0].attach_char - 1);
                arg0->YOffsetTget = 100.0f;
            }
        } else if (func_151239CC(arg0, CAMERA_MODE_PICKED_UP) != 0) {
            func_151254F4(arg0, 0);
            arg0->YOffsetTget = 0.0f;
        }
    }
}
