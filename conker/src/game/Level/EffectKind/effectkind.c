#include <ultra64.h>

#include "functions.h"
#include "variables.h"


u8 func_15012720(void) {
    switch(level)
    {
        case 0:
        case 44:
        case 55:
            return 2;
        case 40:
            return 6;
        default:
            return 5;
    }
}
