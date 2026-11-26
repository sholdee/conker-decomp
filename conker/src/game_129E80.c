#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Runs func_150FC974 on the object (which passes its 0x38 pointer to func_1516972C), then the shared func_15149368. */
void func_150FC9D0(TimedEffect *obj) {
    func_150FC974(obj);
    func_15149368(obj);
}
