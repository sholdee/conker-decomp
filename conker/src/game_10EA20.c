#include <ultra64.h>
#define func_1505D1C4 func_1505D1C4_header // matching: this file keeps its own declaration of func_1505D1C4
#include "functions.h"
#include "variables.h"
#undef func_1505D1C4
s32 func_1505D1C4(f32, f32, f32, s32, s32, u16, s32, s32); // matching: the definition returns nothing; this caller uses $v0

void func_15047390(MtxF mf, f32 eyeX, f32 eyeY, f32 eyeZ, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ);

/*
 * Casts a ray from (fromX, fromY, fromZ) to (toX, toY, toZ) against the first 25 objects and picks the
 * one func_15049440 scores lowest, skipping slot `skipIndex` and `source`'s own slot. If one is found,
 * spawns func_1505D1C4 effect `effect` at it and copies `source`'s unk76 to it.
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
            hit = func_15049440(view, obj->xpos, obj->ypos + (f32)obj->unkD6, obj->zpos,
                                (f32)obj->unkD2, (f32)obj->unkD4, 0.0f, length, 10.0f, 10.0f);
            if (hit != 0 && (best == -1 || hit < bestHit)) {
                best = i;
                bestHit = hit;
            }
        }
    }

    if (best != -1) {
        obj = &movement_obj[best];
        result = func_1505D1C4(obj->xpos, (f32)obj->unkD6 + obj->ypos, obj->zpos, effect,
                               dc_glbl_CharNum, 0, 0, 0);
        obj->angle = source->angle;
    }
    return result;
}
