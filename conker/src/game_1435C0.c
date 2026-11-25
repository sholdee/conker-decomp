#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 func_15195FB0(void *, s32, s32, s32, s32, s32, s32);
extern f32 func_1510F648(f32, f32, f32);
extern u16 D_800CC354;

/* Object that spawns a child from a packed spawn word on its first update. */
struct Obj15116110 {
    u8 pad0[0x3C];
    s32 spawnInfo; /* bits 0-14 id, 15+ passed whole, 16-23 and 24-31 two byte args */
    u8 pad40[0x3C];
    s32 handle;
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

/* Spawns the object's child once, from the packed spawn word, then clears the word. */
void func_15116110(struct Obj15116110 *arg0) {
    s32 info;
    s32 id, hi;

    if (arg0->handle == 0) {
        info = arg0->spawnInfo;
        id = (u16)info & 0x7FFF;
        hi = info >> 15;
        info >>= 0; // matching: keeps the byte args' shifts below the id/hi setup (as1 orders by -g3 line)
        arg0->handle = func_15195FB0(arg0, id, hi, -1, 0, (s8)(info >> 24) & 0xFF, (s8)(info >> 16) & 0xFF);
        arg0->spawnInfo = 0;
    }
}

/* Snaps the object to the ground and eases its height offset towards a rest value. */
void func_1511617C(struct Obj1511617C *arg0) {
    f32 delta;
    s32 target;
    s16 oldY;

    oldY = arg0->y;
    arg0->groundY = func_1510F648(arg0->x, arg0->y, arg0->z);
    if (fabsf(arg0->groundY - arg0->y) > 200.0f) {
        arg0->heightOffset = 0;
    }
    if (!(arg0->flags & 4)) {
        delta = -arg0->heightOffset;
        arg0->heightOffset += delta * 0.2f;
    } else {
        target = -20;
        if (D_800CC354 == 75) {
            target = -40;
        }
        delta = target - arg0->heightOffset;
        arg0->heightOffset += delta * 0.3f;
    }
    arg0->y = arg0->groundY + arg0->heightOffset;
    arg0->dy = arg0->y - oldY;

}

/* Moves the object along a cosine bob and records its vertical movement in dy. */
void func_151162D4(struct Obj151162D4 *arg0) {
    f32 amplitude;
    u32 speed;
    s16 prevY;
    f32 offset;

    prevY = arg0->y + arg0->bobOffset;
    amplitude = (s16)(arg0->wave & 0xFFFF);
    speed = (arg0->wave >> 16) & 0xFFFF;
    offset = cosf(arg0->phase * (360.0f / 65536.0f)) * amplitude;
    arg0->bobOffset = offset;
    arg0->phase += speed * D_800BE9E4;
    arg0->dy = (arg0->y + offset) - prevY;
}
