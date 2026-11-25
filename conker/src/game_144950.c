#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Behaviour handlers for scenery objects (WorldObject) that swing one rotation axis toward a target angle.
 */

/* Swings rotZ toward the target angle with func_151172D8's damped spring. */
void func_151174A0(WorldObject *obj) {
    obj->v = func_151172D8(obj, obj->v);
}

/* Swings rotY toward the target angle with func_151172D8's damped spring. */
void func_151174C8(WorldObject *obj) {
    obj->u = func_151172D8(obj, obj->u);
}
