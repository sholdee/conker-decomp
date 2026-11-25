#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15047390(f32 mf[4][4], f32 eyeX, f32 eyeY, f32 eyeZ, f32 atX, f32 atY, f32 atZ, f32 upX, f32 upY, f32 upZ);
s32 func_1505D1C4(f32, f32, f32, s32, s32, u16, s32, s32);

/*
 * Casts a ray from (fromX, fromY, fromZ) to (toX, toY, toZ) against the first 25 objects and picks the
 * one func_15049440 scores lowest, skipping slot `skipIndex` and `source`'s own slot. If one is found,
 * spawns func_1505D1C4 effect `effect` at it and copies `source`'s unk76 to it.
 */
s32 func_150E1570(Actor *source, f32 fromX, f32 fromY, f32 fromZ, f32 toX, f32 toY, f32 toZ, s32 skipIndex,
                  s32 effect) {
    f32 view[4][4];
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
        sourceIndex = source->unk124 - 1;
    } else {
        sourceIndex = -1;
    }

    best = -1;
    obj = D_800CC2D0;
    for (i = 0; i < 25; i++, obj++) {
        if (obj->interaction_state != 0 && i != skipIndex && obj->health != 0 && !obj->immune && i != sourceIndex) {
            hit = func_15049440(view, obj->x_position, obj->y_position + (f32)obj->unkD6, obj->z_position,
                                (f32)obj->unkD2, (f32)obj->unkD4, 0.0f, length, 10.0f, 10.0f);
            if (hit != 0 && (best == -1 || hit < bestHit)) {
                best = i;
                bestHit = hit;
            }
        }
    }

    if (best != -1) {
        obj = &D_800CC2D0[best];
        result = func_1505D1C4(obj->x_position, (f32)obj->unkD6 + obj->y_position, obj->z_position, effect,
                               D_800C3E78, 0, 0, 0);
        obj->unk76 = source->unk76;
    }
    return result;
}
