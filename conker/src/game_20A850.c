#include <ultra64.h>
#include "functions.h"
#include "variables.h"


s32 func_151DD3A0(OSMesgQueue *arg0, u8 arg1, u8 *arg2, s32 arg3) {
    s32 ret = 0;

    while (save_pollEepromWrite(1) != 0) {};

    while (arg3 > 0) {
        ret = func_151DD140(arg0, arg1, arg2);
        if (ret != 0) {
            return ret;
        }
        arg3 -= 8;
        arg1++;
        arg2 += 8;
    }

    return ret;
}
