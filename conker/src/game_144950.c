#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Behaviour handlers for scenery objects (WorldObject) that swing one rotation axis toward a target angle.
 */
f32 func_151172D8(WorldObject *obj, f32 angle);

/* Swings rotZ toward the target angle with func_151172D8's damped spring. */
void func_151174A0(WorldObject *obj) {
    obj->rotZ = func_151172D8(obj, obj->rotZ);
}

/* Swings rotY toward the target angle with func_151172D8's damped spring. */
void func_151174C8(WorldObject *obj) {
    obj->rotY = func_151172D8(obj, obj->rotY);
}
