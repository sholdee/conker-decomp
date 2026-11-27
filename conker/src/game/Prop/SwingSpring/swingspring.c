#include <ultra64.h>
#include "conker/prop.h"

/*
 * Behaviour handlers for scenery objects (WorldObject) that swing one rotation axis toward a target angle.
 */

/* Swings rotZ toward the target angle with prop_springAngle's damped spring. */
void prop_swingZ(WorldObject *obj) {
    obj->v = prop_springAngle(obj, obj->v);
}

/* Swings rotY toward the target angle with prop_springAngle's damped spring. */
void prop_swingY(WorldObject *obj) {
    obj->u = prop_springAngle(obj, obj->u);
}
