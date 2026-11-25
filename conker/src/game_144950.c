#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Behaviour handlers for scenery objects that swing one rotation axis toward a target angle.
 * Same object layout as MoverObj in game_143DE0.c (only the fields used here are named).
 */
typedef struct MoverObj {
    /* 0x00 */ f32 rot[3]; /* degrees */
    /* 0x0C */ u8 pad0C[0x30];
    /* 0x3C */ s32 params;
    /* 0x40 */ u8 pad40[0x3C];
    /* 0x7C */ struct {
        f32 damping;
        f32 acceleration;
        f32 velocity;
    } swing;
} MoverObj;

f32 func_151172D8(MoverObj *obj, f32 angle);

/* Swings rot[2] toward the target angle with func_151172D8's damped spring. */
void func_151174A0(MoverObj *obj) {
    obj->rot[2] = func_151172D8(obj, obj->rot[2]);
}

/* Swings rot[1] toward the target angle with func_151172D8's damped spring. */
void func_151174C8(MoverObj *obj) {
    obj->rot[1] = func_151172D8(obj, obj->rot[1]);
}
