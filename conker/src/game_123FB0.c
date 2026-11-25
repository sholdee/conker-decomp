#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_1509BE40();

#define LERP(a, b, t) (((b) - (a)) * (t) + (a))

/*
 * While query 0x4003 holds and the actor is alive, measures how far the actor's position has progressed
 * along the segment from D_800A1B80 to D_800A1B8C (scaled by 0.75, clamped to [0, 1]) and passes values
 * derived from it to func_1509BFB0; also sets unk374 and turns unk37C via func_15049688. Otherwise clears
 * the flags it set.
 */
void func_150F6B00(Camera *actor) {
    f32 vertical;
    struct17 normalized;
    struct17 offset;
    struct17 direction;
    f32 blend;
    f32 inverseLength;
    f32 lateral;
    f32 projection;
    f32 clamped;

    if (func_1509BE40(1, 0x4003, 6, 0x2000) && actor->player->health) {
        actor->flags |= 0xC0001000;
        func_15048F90((struct17 *)&D_800A1B80, (struct17 *)&actor->focus.x, &offset);
        func_15048F90((struct17 *)&D_800A1B80, (struct17 *)&D_800A1B8C, &direction);
        inverseLength = 1.0f / func_150AD930(&direction.unk0);
        func_1504917C(&direction, &normalized);
        projection = func_150AD900(&normalized.unk0, &offset.unk0) * inverseLength * 0.75f;
        if (projection < 0.0f) {
            blend = 0.0f;
        } else {
            // matching: IDO needs the separate clamped temporary (and the f32 vertical below) for this frame layout
            clamped = (projection > 1.0f) ? 1.0f : projection;
            blend = clamped;
        }
        if (func_1509BE40(4, 0x2000, 0xAC, 0x4040, 0x4041, 0x4042, 0x4043)) {
            lateral = LERP(-900.0f, -900.0f, blend);
            actor->targetDistance = 220.0f;
        } else if (func_1509BE40(4, 0x2000, 0xAC, 0x4047, 0x4046, 0x4045, 0x4044)) {
            lateral = LERP(900.0f, 900.0f, blend);
            actor->targetDistance = 220.0f;
        } else {
            actor->targetDistance = 294.0f;
            lateral = LERP(0.0f, 0.0f, blend);
        }
        vertical = -20.0f;
        func_1509BFB0(3, 0x4003, 3, (s32)lateral, (s32)vertical, (s32)LERP(-5530.0f, 1198.0f, blend)); // z of the segment ends
        func_15049688(&actor->yaw, func_15048FC8((struct17 *)&D_800D9A50) - 180.0f, &actor->yawEaseVel, 8.0f, 10.0f, actor->dt);
        actor->yawRad = actor->yaw * 0.01745329238f; // degrees to radians
    } else {
        actor->flags &= 0x3FFFEFFF;
    }
}
