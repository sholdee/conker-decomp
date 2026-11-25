#include <ultra64.h>
/* func_150ADA20 returns a full 32-bit value; the shared header narrows it to u8 */
#define func_150ADA20 func_150ADA20_u8
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"


/* Debris emitter settings read by func_151A6F00 (same layout as in game_1D43B0.c). */
typedef struct {
    /* 0x00 */ s16 count;
    /* 0x04 */ vertex pos;
    /* 0x10 */ s16 lifetime;
    /* 0x14 */ f32 radius;
    /* 0x18 */ s16 yawBase;
    /* 0x1A */ s16 yawRange;
    /* 0x1C */ s16 pitchBase;
    /* 0x1E */ s16 pitchRange;
    /* 0x20 */ f32 speedBase;
    /* 0x24 */ f32 speedRange;
    /* 0x28 */ f32 velYBase;
    /* 0x2C */ f32 velYRange;
    /* 0x30 */ vertex dir;
    /* 0x3C */ f32 sizeBase;
    /* 0x40 */ f32 sizeRange;
    /* 0x44 */ f32 gravityBase;
    /* 0x48 */ f32 gravityRange;
    /* 0x4C */ f32 spinBase;
    /* 0x50 */ f32 spinRange;
    /* 0x54 */ u8 landPuffs;
    /* 0x55 */ u8 landPuffFlag;
    /* 0x56 */ s16 landPuffLifeBase;
    /* 0x58 */ s16 landPuffLifeRange;
} DebrisEmitter;

extern void func_151A6F00(DebrisEmitter *, s32, u8, s32);
extern void func_151D5334(vertex *, f32, f32, f32, s32, u8, s32);
extern void func_151D3FF4(vertex *, u8, s32);
extern void func_151D5514(vertex *, u8, s32);
extern void func_151541B8(vertex *, f32, f32, f32, f32, u8, s32);

/* Spawns a debris burst plus the follow-up effects at the object's position. */
void func_151A6C90(struct127 *obj, u8 arg1, s32 arg2) {
    vertex pos;
    DebrisEmitter emitter;

    pos.x = obj->x_position;
    pos.y = obj->y_position;
    pos.z = obj->z_position;

    if (obj != NULL) {
        emitter.count = (func_150ADA20() & 3) + 6;
        emitter.pos = pos;
        emitter.radius = 60.0f;
        emitter.speedBase = 0; // matching: an int 0 gets its own mtc1 zero; 0.0f would share the 0.0f register
        emitter.dir.x = 0.0f;
        emitter.lifetime = 80;
        emitter.yawBase = 0;
        emitter.yawRange = 255;
        emitter.pitchBase = -63;
        emitter.speedRange = 35.0f;
        emitter.velYBase = 10.0f;
        emitter.velYRange = 40.0f;
        emitter.sizeBase = 0.2f;
        emitter.sizeRange = 0.5f;
        emitter.spinBase = -2.0f;
        emitter.spinRange = 4.0f;
        emitter.pitchRange = 80;
        emitter.landPuffs = 0;
        emitter.landPuffFlag = 0;
        emitter.landPuffLifeBase = 50;
        emitter.landPuffLifeRange = 25;
        emitter.gravityBase = -1.0f;
        emitter.gravityRange = -1.0f;
        emitter.dir.y = 0.0f;
        emitter.dir.z = 0.0f;

        func_151A6F00(&emitter, 0, arg1, arg2);
        func_151D5334(&pos, 506.0f, 1013.0f, 0.0009871669f, 5, arg1, arg2);
        func_151D3FF4(&pos, arg1, arg2);
        func_151D5514(&pos, arg1, arg2);
        func_151541B8(&pos, func_150ADA68() * 4.0f + 12.0f, 1.6409999f,
                      func_150ADA20() % 56U + 200, 0.0f, arg1, arg2);
        func_151D5404((struct17 *)&pos, 1502.0f, 3000.0f, 0.00033333333f, 12, 15, 255, 0);
    }
}
