#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_150E1800(Camera *arg0) {
    if (script_query(1, 0x403E, 6, 0x2000) != 0) {
        arg0->AvailableModes |= 0x20;
        return;
    }
    arg0->AvailableModes &= ~0x20;
}
