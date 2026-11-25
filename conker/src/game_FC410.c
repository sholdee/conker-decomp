#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern f32 D_800A0830;
extern f32 D_800A0834;

struct CPad150CEF60 {
    f32 c;
    f32 pad[6];
};

void func_150CEF60(s32 arg0) {
    WorldObject *temp_v0;
    f32 angle;
    struct CPad150CEF60 c;
    f32 s;
    f32 cx;
    f32 cy;

    cx = -1.0f;
    cy = 44.0f;
    temp_v0 = func_151149AC(4);
    angle = temp_v0->v * D_800A0830;
    c.c = func_150AD78C(angle);
    s = func_150AD780(angle);
    func_150A3444(2,
                  (s16)((76.0f - cx) * s - (241.0f - cy) * c.c + cx),
                  (s16)((76.0f - cx) * c.c + (241.0f - cy) * s + cy),
                  (s16)D_800A0834);
    func_151749A0(5, 3);
}

extern f32 D_800A0838;

f32 func_150CF040(s32 arg0, s32 arg1) {
    arg0 += 1;
    arg1 -= 0x2C;
    return func_150484A0((f32)arg0, (f32)arg1) * D_800A0838;
}

void func_150CF080(s32 *arg0) {
    arg0[0x21] |= 0x4000;
}
