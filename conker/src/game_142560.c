#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A2F8C;

/* Object driven by the behaviour callbacks in this file. */
typedef struct Game142560Object {
    /* 0x00 */ f32 angleX;
    /* 0x04 */ f32 angleY;
    /* 0x08 */ f32 angleZ;
    /* 0x0C */ u8 pad0C[0x4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x26];
    /* 0x3C */ s32 params; // packed per-object parameters
    /* 0x40 */ u8 pad40[0xF];
    /* 0x4F */ u8 flags;
    /* 0x50 */ u8 pad50[0x10];
    /* 0x60 */ f32 spinX;
    /* 0x64 */ f32 spinY;
    /* 0x68 */ f32 spinZ;
    /* 0x6C */ u8 pad6C[0x10];
    /* 0x7C */ s32 homeY;
    /* 0x80 */ s32 timer;
    /* 0x84 */ s32 returning;
} Game142560Object;

void func_151150B0(s32 arg0) {
}

/* Spins the object about Z by (params >> 16) / 256 degrees per frame, wrapping to [0, 360). */
void func_151150BC(Game142560Object *obj) {
    obj->spinZ = ((obj->params >> 16) * D_800BE9E4) * (1.0f / 256.0f);
    obj->angleZ += obj->spinZ;
    if (obj->angleZ < 0.0f) {
        obj->angleZ += 360.0f;
        return;
    }
    if (obj->angleZ >= 360.0f) {
        obj->angleZ -= 360.0f;
    }
}

/* Spins the object about Y by (params >> 16) / 256 degrees per frame, wrapping to [0, 360). */
void func_1511515C(Game142560Object *obj) {
    obj->spinY = ((obj->params >> 16) * D_800BE9E4) * (1.0f / 256.0f);
    obj->angleY += obj->spinY;
    if (obj->angleY < 0.0f) {
        obj->angleY += 360.0f;
        return;
    }
    if (obj->angleY >= 360.0f) {
        obj->angleY -= 360.0f;
    }
}

/* Spins the object about X by (params >> 16) / 256 degrees per frame, wrapping to [0, 360). */
void func_151151FC(Game142560Object *obj) {
    obj->spinX = ((obj->params >> 16) * D_800BE9E4) * (1.0f / 256.0f);
    obj->angleX += obj->spinX;
    if (obj->angleX < 0.0f) {
        obj->angleX += 360.0f;
        return;
    }
    if (obj->angleX >= 360.0f) {
        obj->angleX -= 360.0f;
    }
}

void func_1511529C(s32 arg0) {
}

void func_151152A8(Game142560Object *arg0) {
    s32 target;
    s32 current;
    s32 step;
    s32 *targetPtr;

    targetPtr = &arg0->homeY;
    target = *targetPtr;
    if (target == 0) {
        target = arg0->y;
        *targetPtr = target;
    }
    current = arg0->y;
    if ((arg0->flags & 0xFF) & 4) {
        step = arg0->params;
        if ((target - current) < (s16)step) {
            arg0->y = current - ((s16)(step >> 16) * D_800BE9E4);
            return;
        }
    } else {
        arg0->y = (s32)((f32)current + ((f32)(target - current) * D_800A2F8C * (f32)D_800BE9E4));
    }
}



void func_15115368(Game142560Object *arg0) {
    s32 lo;
    s32 range;
    f32 t;
    f32 d;
    f32 s;

    range = (s8)(arg0->params >> 8);
    lo = arg0->params >> 24;
    if (arg0->flags & 4) {
        arg0->timer += D_800BE9E4;
    }
    if (arg0->timer <= lo) {
        if (!(arg0->flags & 4) && arg0->timer != 0) {
            arg0->timer = lo;
        }
    } else if (arg0->timer <= lo + range) {
        if (!(arg0->flags & 4)) {
            arg0->timer = lo + range;
        }
    } else {
        if (!(arg0->flags & 4)) {
            arg0->timer += D_800BE9E4;
        }
        t = arg0->timer - lo - range;
        if (arg0->returning == 0) {
            s = sinf(t * 0.628000021f * 2);
            arg0->angleX = -3 * s;
            s = sinf(t * 0.628000021f * 3);
            arg0->angleZ = 2 * s;
            if (t > 10.0f) {
                if (arg0->homeY == 0) {
                    arg0->homeY = arg0->y;
                }
                arg0->y -= (s8)(arg0->params >> 16) * D_800BE9E4;
                if ((arg0->params & 0xFF) * 16 < arg0->homeY - arg0->y) {
                    s32 pad;
                    arg0->flags &= ~0x61;
                    arg0->flags |= 0x20;
                    arg0->returning = 1;
                }
            }
        } else {
            {
                f32 dx;
                f32 dz;
                dx = D_800CC2D0[0].x_position - arg0->x;
                dz = D_800CC2D0[0].z_position - arg0->z;
                dx = dx * dx;
                dz = dz * dz;
                d = dx + dz;
            }
            if (d > 250000.0f) {
                arg0->flags &= ~0x61;
                arg0->flags |= 1;
                arg0->y = arg0->homeY;
                arg0->homeY = 0;
                arg0->returning = 0;
                arg0->timer = 0;
            }
        }
    }
}
