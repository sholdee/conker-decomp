#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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

/*
 * Sinks the object (y) at (params >> 16) units per frame while flag 4 is set, until it is
 * (s16)params below its home height; otherwise eases it back toward home.
 */
void func_151152A8(Game142560Object *obj) {
    if (obj->homeY == 0) {
        obj->homeY = obj->y;
    }
    if (obj->flags & 4) {
        if (obj->homeY - obj->y < (s16)obj->params) {
            obj->y -= (s16)(obj->params >> 16) * D_800BE9E4;
        }
    } else {
        obj->y += (obj->homeY - obj->y) * 0.05f * D_800BE9E4;
    }
}

/*
 * Shake-and-sink behaviour. params: start delay (bits 24-31), hold time (bits 8-15),
 * sink speed (bits 16-23) and sink depth / 16 (bits 0-7). The timer runs every frame
 * while flag 4 is set, and otherwise jumps to the end of the current phase. After the
 * delay and hold the object wobbles about X and Z, and after 10 more frames it sinks.
 * Once sunk deep enough it is marked as returning, and it resets to its home height
 * when D_800CC2D0[0] is more than 500 units away in XZ.
 */
void func_15115368(Game142560Object *obj) {
    s32 start;
    s32 hold;
    f32 t;
    f32 distSq;
    f32 s;

    hold = (s8)(obj->params >> 8);
    start = obj->params >> 24;
    if (obj->flags & 4) {
        obj->timer += D_800BE9E4;
    }
    if (obj->timer <= start) {
        if (!(obj->flags & 4) && obj->timer != 0) {
            obj->timer = start;
        }
    } else if (obj->timer <= start + hold) {
        if (!(obj->flags & 4)) {
            obj->timer = start + hold;
        }
    } else {
        if (!(obj->flags & 4)) {
            obj->timer += D_800BE9E4;
        }
        t = obj->timer - start - hold;
        if (obj->returning == 0) {
            s = sinf(t * 0.628f * 2);
            obj->angleX = -3 * s;
            s = sinf(t * 0.628f * 3);
            obj->angleZ = 2 * s;
            if (t > 10.0f) {
                if (obj->homeY == 0) {
                    obj->homeY = obj->y;
                }
                obj->y -= (s8)(obj->params >> 16) * D_800BE9E4;
                if ((obj->params & 0xFF) * 16 < obj->homeY - obj->y) {
                    s32 pad; // matching: this block's -g3 scope (and slot) fixes the schedule and frame
                    obj->flags &= ~0x61;
                    obj->flags |= 0x20;
                    obj->returning = 1;
                }
            }
        } else {
            {
                f32 dx;
                f32 dz;

                dx = D_800CC2D0[0].x_position - obj->x;
                dz = D_800CC2D0[0].z_position - obj->z;
                dx = dx * dx;
                dz = dz * dz;
                distSq = dx + dz;
            }
            if (distSq > 500.0f * 500.0f) {
                obj->flags &= ~0x61;
                obj->flags |= 1;
                obj->y = obj->homeY;
                obj->homeY = 0;
                obj->returning = 0;
                obj->timer = 0;
            }
        }
    }
}
