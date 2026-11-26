#include <ultra64.h>
#include "functions.h"
#include "variables.h"


struct CPad150CEF60 {
    f32 c;
    f32 pad[6];
};

void effect_levelUpdateRotatingZone(s32 arg0) {
    WorldObject *temp_v0;
    f32 angle;
    struct CPad150CEF60 c;
    f32 s;
    f32 cx;
    f32 cy;
    f32 z;

    cx = -1.0f;
    cy = 44.0f;
    z = -1805.0f;
    temp_v0 = gfx_findObjectById(4);
    angle = temp_v0->v * 0.017453292f;
    c.c = func_150AD78C(angle);
    s = func_150AD780(angle);
    zone_setPosition(2,
                  (76.0f - cx) * s - (241.0f - cy) * c.c + cx,
                  (76.0f - cx) * c.c + (241.0f - cy) * s + cy,
                  z);
    effect_advanceLevelTexFrame(5, 3);
}

f32 func_150CF040(s32 arg0, s32 arg1) {
    arg0 += 1;
    arg1 -= 0x2C;
    return math_atan2(arg0, arg1) * 57.295776f;
}

void func_150CF080(s32 *arg0) {
    arg0[0x21] |= 0x4000;
}
