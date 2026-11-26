#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Registers a TexScroll on res's display list (game/Effects/CharacterFlameThrower/characterflamethrower.c). */
/* Height of the prop surface below (x, y, z), or -10000 (game/Collision/Floor/floor.c). */

/*
 * Behaviour scratch state (WorldObject.touch) of the objects in this file:
 * func_15116110 keeps its texture scroll, func_1511617C the ground height, func_151162D4 its bob phase.
 */
typedef union {
    void *texScroll;
    f32 groundY;
    s32 phase; /* scaled by 360/65536 before cosf */
} Work1435C0;

#define WORK(obj) ((Work1435C0 *)&(obj)->touch)

/* Registers the object's texture scroll once, from the packed scroll word in param (bits 0-14 texture,
 * >> 15 doubled, bits 16-23 speedT, 24-31 speedS), then clears the word. */
void func_15116110(WorldObject *obj) {
    s32 info;
    s32 texture, doubled;

    if (WORK(obj)->texScroll == NULL) {
        info = obj->moveData.word;
        texture = (u16)info & 0x7FFF; // matching: the (u16) and (s8) casts pick IDO's register assignment
        doubled = info >> 15;
        info >>= 0; // matching: keeps the speed shifts below the texture/doubled setup (as1 orders by -g3 line)
        WORK(obj)->texScroll = flamethrower_addObjectTexScroll(obj, texture, doubled, -1, 0, (s8)(info >> 24) & 0xFF, (s8)(info >> 16) & 0xFF);
        obj->moveData.word = 0;
    }
}

/* Snaps the object to the ground and eases its height offset (param) towards a rest value. */
void func_1511617C(WorldObject *obj) {
    f32 delta;
    s32 target;
    s16 oldY;

    oldY = obj->y;
    WORK(obj)->groundY = collision_getPropSurfaceHeight(obj->x, obj->y, obj->z);
    if (fabsf(WORK(obj)->groundY - obj->y) > 200.0f) {
        obj->moveData.word = 0;
    }
    if (!(obj->object_status & 4)) {
        delta = -obj->moveData.word;
        obj->moveData.word += delta * 0.2f;
    } else {
        target = -20;
        if (movement_obj[0].animseq.uh == 75) {
            target = -40;
        }
        delta = target - obj->moveData.word;
        obj->moveData.word += delta * 0.3f;
    }
    obj->y = WORK(obj)->groundY + obj->moveData.word;
    obj->moveY = obj->y - oldY;
}

/* Moves the object along a cosine bob (param: low 16 bits amplitude, high 16 bits phase speed) and
 * records its vertical movement in moveY. */
void func_151162D4(WorldObject *obj) {
    f32 amplitude;
    u32 speed;
    s16 prevY;
    f32 offset;

    prevY = obj->y + obj->yOffset;
    amplitude = (s16)(obj->moveData.word & 0xFFFF);
    speed = (obj->moveData.word >> 16) & 0xFFFF;
    offset = cosf(WORK(obj)->phase * (360.0f / 65536.0f)) * amplitude;
    obj->yOffset = offset;
    WORK(obj)->phase += speed * D_800BE9E4;
    obj->moveY = (obj->y + offset) - prevY;
}
