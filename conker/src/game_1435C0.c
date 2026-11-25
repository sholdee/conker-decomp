#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Registers a TexScroll on res's display list (game_1C2C60.c). */
extern void *func_15195FB0(void *res, s32 texture, s32 doubled, s32 occurrence, s32 isNew, s32 speedS,
                           s32 speedT);
/* Height of the prop surface below (x, y, z), or -10000 (game_13BB20.c). */
extern f32 func_1510F648(f32 x, f32 y, f32 z);
extern u16 D_800CC354;

/* Object whose display list gets a texture scroll registered on its first update. */
struct Obj15116110 {
    u8 pad0[0x3C];
    s32 scrollInfo; /* bits 0-14 texture, >> 15 doubled, bits 16-23 speedT, 24-31 speedS */
    u8 pad40[0x3C];
    void *texScroll;
};

/* Object that follows the ground height with a damped vertical offset. */
struct Obj1511617C {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
    u8 pad16[0x26];
    s32 heightOffset;
    u8 pad40[0xF];
    u8 flags;
    u8 pad50[0xC];
    s16 dy;
    u8 pad5E[0x1E];
    f32 groundY;
};

/* Object that bobs up and down on a cosine wave. */
struct Obj151162D4 {
    u8 pad0[0x12];
    s16 y;
    u8 pad14[0x4];
    f32 bobOffset;
    u8 pad1C[0x20];
    s32 wave; /* low 16 bits: amplitude (signed), high 16 bits: phase speed */
    u8 pad40[0x1C];
    s16 dy;
    u8 pad5E[0x1E];
    s32 phase; /* scaled by 360/65536 before cosf */
};

/* Registers the object's texture scroll once, from the packed scroll word, then clears the word. */
void func_15116110(struct Obj15116110 *obj) {
    s32 info;
    s32 texture, doubled;

    if (obj->texScroll == NULL) {
        info = obj->scrollInfo;
        texture = (u16)info & 0x7FFF; // matching: the (u16) and (s8) casts pick IDO's register assignment
        doubled = info >> 15;
        info >>= 0; // matching: keeps the speed shifts below the texture/doubled setup (as1 orders by -g3 line)
        obj->texScroll = func_15195FB0(obj, texture, doubled, -1, 0, (s8)(info >> 24) & 0xFF, (s8)(info >> 16) & 0xFF);
        obj->scrollInfo = 0;
    }
}

/* Snaps the object to the ground and eases its height offset towards a rest value. */
void func_1511617C(struct Obj1511617C *obj) {
    f32 delta;
    s32 target;
    s16 oldY;

    oldY = obj->y;
    obj->groundY = func_1510F648(obj->x, obj->y, obj->z);
    if (fabsf(obj->groundY - obj->y) > 200.0f) {
        obj->heightOffset = 0;
    }
    if (!(obj->flags & 4)) {
        delta = -obj->heightOffset;
        obj->heightOffset += delta * 0.2f;
    } else {
        target = -20;
        if (D_800CC354 == 75) {
            target = -40;
        }
        delta = target - obj->heightOffset;
        obj->heightOffset += delta * 0.3f;
    }
    obj->y = obj->groundY + obj->heightOffset;
    obj->dy = obj->y - oldY;
}

/* Moves the object along a cosine bob and records its vertical movement in dy. */
void func_151162D4(struct Obj151162D4 *obj) {
    f32 amplitude;
    u32 speed;
    s16 prevY;
    f32 offset;

    prevY = obj->y + obj->bobOffset;
    amplitude = (s16)(obj->wave & 0xFFFF);
    speed = (obj->wave >> 16) & 0xFFFF;
    offset = cosf(obj->phase * (360.0f / 65536.0f)) * amplitude;
    obj->bobOffset = offset;
    obj->phase += speed * D_800BE9E4;
    obj->dy = (obj->y + offset) - prevY;
}
