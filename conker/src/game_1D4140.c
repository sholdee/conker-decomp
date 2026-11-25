#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Spawns a debris burst plus the follow-up effects at the object's position. */
void func_151A6C90(Actor *obj, u8 arg1, s32 arg2) {
    Vec3f pos;
    DebrisEmitter emitter;

    pos.x = obj->xpos;
    pos.y = obj->ypos;
    pos.z = obj->zpos;

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
        func_151D5404(&pos, 1502.0f, 3000.0f, 0.00033333333f, 12, 15, 255, 0);
    }
}
