#include <ultra64.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/gfx.h"
#include "conker/math.h"
#include "conker/zone.h"


// matching: the cosine local sits in a 0x1C-byte block, so the stack frame has the original size
struct CPad150CEF60 {
    f32 c;
    f32 pad[6];
};

void effect_levelUpdateRotatingZone(s32 unused) {
    WorldObject *object;
    f32 angle;
    struct CPad150CEF60 c;
    f32 s;
    f32 cx;
    f32 cy;
    f32 z;

    cx = -1.0f;
    cy = 44.0f;
    z = -1805.0f;
    object = gfx_findObjectById(4);
    angle = object->v * 0.017453292f;
    c.c = math_sin(angle);
    s = math_cos(angle);
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

void effect_levelHook16(s32 *arg0) {
    arg0[0x21] |= 0x4000;
}
