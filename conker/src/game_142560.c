#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Behaviour scratch state (WorldObject.work) of the objects driven by the callbacks in this file. */
typedef struct {
    /* 0x7C */ s32 homeY;
    /* 0x80 */ s32 timer;
    /* 0x84 */ s32 returning;
} Work142560;

#define WORK(obj) ((Work142560 *)&(obj)->work)

void func_151150B0(s32 arg0) {
}

/* Spins the object about Z by (params >> 16) / 256 degrees per frame, wrapping to [0, 360). */
void func_151150BC(WorldObject *obj) {
    obj->spinZ = ((obj->param.word >> 16) * D_800BE9E4) * (1.0f / 256.0f);
    obj->rotZ += obj->spinZ;
    if (obj->rotZ < 0.0f) {
        obj->rotZ += 360.0f;
        return;
    }
    if (obj->rotZ >= 360.0f) {
        obj->rotZ -= 360.0f;
    }
}

/* Spins the object about Y by (params >> 16) / 256 degrees per frame, wrapping to [0, 360). */
void func_1511515C(WorldObject *obj) {
    obj->spinY = ((obj->param.word >> 16) * D_800BE9E4) * (1.0f / 256.0f);
    obj->rotY += obj->spinY;
    if (obj->rotY < 0.0f) {
        obj->rotY += 360.0f;
        return;
    }
    if (obj->rotY >= 360.0f) {
        obj->rotY -= 360.0f;
    }
}

/* Spins the object about X by (params >> 16) / 256 degrees per frame, wrapping to [0, 360). */
void func_151151FC(WorldObject *obj) {
    obj->spinX = ((obj->param.word >> 16) * D_800BE9E4) * (1.0f / 256.0f);
    obj->rotX += obj->spinX;
    if (obj->rotX < 0.0f) {
        obj->rotX += 360.0f;
        return;
    }
    if (obj->rotX >= 360.0f) {
        obj->rotX -= 360.0f;
    }
}

void func_1511529C(s32 arg0) {
}

/*
 * Sinks the object (y) at (params >> 16) units per frame while flag 4 is set, until it is
 * (s16)params below its home height; otherwise eases it back toward home.
 */
void func_151152A8(WorldObject *obj) {
    if (WORK(obj)->homeY == 0) {
        WORK(obj)->homeY = obj->y;
    }
    if (obj->flags & 4) {
        if (WORK(obj)->homeY - obj->y < (s16)obj->param.word) {
            obj->y -= (s16)(obj->param.word >> 16) * D_800BE9E4;
        }
    } else {
        obj->y += (WORK(obj)->homeY - obj->y) * 0.05f * D_800BE9E4;
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
void func_15115368(WorldObject *obj) {
    s32 start;
    s32 hold;
    f32 t;
    f32 distSq;
    f32 s;

    hold = (s8)(obj->param.word >> 8);
    start = obj->param.word >> 24;
    if (obj->flags & 4) {
        WORK(obj)->timer += D_800BE9E4;
    }
    if (WORK(obj)->timer <= start) {
        if (!(obj->flags & 4) && WORK(obj)->timer != 0) {
            WORK(obj)->timer = start;
        }
    } else if (WORK(obj)->timer <= start + hold) {
        if (!(obj->flags & 4)) {
            WORK(obj)->timer = start + hold;
        }
    } else {
        if (!(obj->flags & 4)) {
            WORK(obj)->timer += D_800BE9E4;
        }
        t = WORK(obj)->timer - start - hold;
        if (WORK(obj)->returning == 0) {
            s = sinf(t * 0.628f * 2);
            obj->rotX = -3 * s; // matching: int literals here; 2.0f/3.0f schedule differently
            s = sinf(t * 0.628f * 3);
            obj->rotZ = 2 * s;
            if (t > 10.0f) {
                if (WORK(obj)->homeY == 0) {
                    WORK(obj)->homeY = obj->y;
                }
                obj->y -= (s8)(obj->param.word >> 16) * D_800BE9E4;
                if ((obj->param.word & 0xFF) * 16 < WORK(obj)->homeY - obj->y) {
                    s32 pad; // matching: this block's -g3 scope (and slot) fixes the schedule and frame
                    obj->flags &= ~0x61;
                    obj->flags |= 0x20;
                    WORK(obj)->returning = 1;
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
                obj->y = WORK(obj)->homeY;
                WORK(obj)->homeY = 0;
                WORK(obj)->returning = 0;
                WORK(obj)->timer = 0;
            }
        }
    }
}
