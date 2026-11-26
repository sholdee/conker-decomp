#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/*
 * Casts a ray from (fromX, fromY, fromZ) to (toX, toY, toZ) against the first 25 objects and picks the
 * one func_15049440 scores lowest, skipping slot `skipIndex` and `source`'s own slot. If one is found,
 * spawns actor_runAttackAt effect `effect` at it and copies `source`'s unk76 to it.
 */
s32 func_150E1570(Actor *source, f32 fromX, f32 fromY, f32 fromZ, f32 toX, f32 toY, f32 toZ, s32 skipIndex,
                  s32 effect) {
    MtxF view;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 length;
    s32 hit;
    s32 result;
    s32 i;
    s32 best;
    s32 bestHit;
    s32 sourceIndex;
    Actor *obj;

    bestHit = 0;
    result = 0;
    func_15047390(view, fromX, fromY, fromZ, toX, toY, toZ, 0.0f, 1.0f, 0.0f);

    dx = fromX - toX;
    dy = fromY - toY;
    dz = fromZ - toZ;
    length = sqrtf((dx * dx) + (dy * dy) + (dz * dz));

    if (source != NULL) {
        sourceIndex = source->owner - 1;
    } else {
        sourceIndex = -1;
    }

    best = -1;
    obj = movement_obj;
    for (i = 0; i < 25; i++, obj++) {
        if (obj->type != 0 && i != skipIndex && obj->energy != 0 && !obj->imune && i != sourceIndex) {
            hit = func_15049440(view, obj->xpos, obj->ypos + obj->unkD6, obj->zpos,
                                obj->unkD2, obj->unkD4, 0.0f, length, 10.0f, 10.0f);
            if (hit != 0 && (best == -1 || hit < bestHit)) {
                best = i;
                bestHit = hit;
            }
        }
    }

    if (best != -1) {
        obj = &movement_obj[best];
        result = actor_runAttackAt(obj->xpos, obj->unkD6 + obj->ypos, obj->zpos, effect,
                               dc_glbl_CharNum, 0, 0, 0);
        obj->angle = source->angle;
    }
    return result;
}
