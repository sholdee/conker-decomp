#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15005F60(Camera *arg0) {
    arg0->DistIndex = 2;
    func_15124B18(arg0);
    arg0->AvailableModes |= 0x4000;
}
