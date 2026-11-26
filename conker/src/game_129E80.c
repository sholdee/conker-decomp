#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Runs func_150FC974 on the object (which destroys its child object), then the shared func_15149368. */
void func_150FC9D0(Effect_129DE0 *obj) {
    func_150FC974(obj);
    func_15149368(&obj->base);
}
