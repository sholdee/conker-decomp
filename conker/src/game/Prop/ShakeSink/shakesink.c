#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * The ROM shows one compilation unit from game/Prop/ShakeSink/shakesink.c to game/Prop/PropMotions/propmotions.c: their .rodata and .data
 * follow each other without the 16-byte unit padding. IDO put the unit's front-end data before all of
 * its float literals, so before this file's: it is defined here, and game/Prop/PropUpdates/propupdates.c reads it.
 */
/* prop_rotateAboutPivots: the (vertex, pivot) index pairs of the vertices that follow the preceding objects. */
const VertexPivots gPivotVertexPairs = { 8, { { 0, 0 }, { 1, 1 }, { 2, 0 }, { 3, 1 }, { 4, 0 }, { 5, 1 }, { 6, 1 }, { 7, 0 } } };
/* func_1511C638: the three sound ids it picks from. */
const SoundTriple D_800A2F84 = { { 0x2B6, 0x2BD, 0x2BE } };

/* Behaviour scratch state (WorldObject.touch) of the objects driven by the callbacks in this file. */
typedef struct {
    /* 0x7C */ s32 homeY;
    /* 0x80 */ s32 timer;
    /* 0x84 */ s32 returning;
} Work142560;

#define WORK(obj) ((Work142560 *)&(obj)->touch)

void func_151150B0(WorldObject *obj) {
}

/* Spins the object about Z by (params >> 16) / 256 degrees per frame, wrapping to [0, 360). */
void prop_spinZ(WorldObject *obj) {
    obj->spinZ = ((obj->moveData.word >> 16) * D_800BE9E4) * (1.0f / 256.0f);
    obj->v += obj->spinZ;
    if (obj->v < 0.0f) {
        obj->v += 360.0f;
        return;
    }
    if (obj->v >= 360.0f) {
        obj->v -= 360.0f;
    }
}

/* Spins the object about Y by (params >> 16) / 256 degrees per frame, wrapping to [0, 360). */
void prop_spinY(WorldObject *obj) {
    obj->spinY = ((obj->moveData.word >> 16) * D_800BE9E4) * (1.0f / 256.0f);
    obj->u += obj->spinY;
    if (obj->u < 0.0f) {
        obj->u += 360.0f;
        return;
    }
    if (obj->u >= 360.0f) {
        obj->u -= 360.0f;
    }
}

/* Spins the object about X by (params >> 16) / 256 degrees per frame, wrapping to [0, 360). */
void prop_spinX(WorldObject *obj) {
    obj->spinX = ((obj->moveData.word >> 16) * D_800BE9E4) * (1.0f / 256.0f);
    obj->t += obj->spinX;
    if (obj->t < 0.0f) {
        obj->t += 360.0f;
        return;
    }
    if (obj->t >= 360.0f) {
        obj->t -= 360.0f;
    }
}

void func_1511529C(s32 arg0) {
}

/*
 * Sinks the object (y) at (params >> 16) units per frame while flag 4 is set, until it is
 * (s16)params below its home height; otherwise eases it back toward home.
 */
void prop_sinkWhileTriggered(WorldObject *obj) {
    if (WORK(obj)->homeY == 0) {
        WORK(obj)->homeY = obj->y;
    }
    if (obj->object_status & 4) {
        if (WORK(obj)->homeY - obj->y < (s16)obj->moveData.word) {
            obj->y -= (s16)(obj->moveData.word >> 16) * D_800BE9E4;
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
 * when movement_obj[0] is more than 500 units away in XZ.
 */
void prop_shakeAndSink(WorldObject *obj) {
    s32 start;
    s32 hold;
    f32 t;
    f32 distSq;
    f32 s;

    hold = (s8)(obj->moveData.word >> 8);
    start = obj->moveData.word >> 24;
    if (obj->object_status & 4) {
        WORK(obj)->timer += D_800BE9E4;
    }
    if (WORK(obj)->timer <= start) {
        if (!(obj->object_status & 4) && WORK(obj)->timer != 0) {
            WORK(obj)->timer = start;
        }
    } else if (WORK(obj)->timer <= start + hold) {
        if (!(obj->object_status & 4)) {
            WORK(obj)->timer = start + hold;
        }
    } else {
        if (!(obj->object_status & 4)) {
            WORK(obj)->timer += D_800BE9E4;
        }
        t = WORK(obj)->timer - start - hold;
        if (WORK(obj)->returning == 0) {
            s = sinf(t * 0.628f * 2);
            obj->t = -3 * s; // matching: int literals here; 2.0f/3.0f schedule differently
            s = sinf(t * 0.628f * 3);
            obj->v = 2 * s;
            if (t > 10.0f) {
                if (WORK(obj)->homeY == 0) {
                    WORK(obj)->homeY = obj->y;
                }
                obj->y -= (s8)(obj->moveData.word >> 16) * D_800BE9E4;
                if ((obj->moveData.word & 0xFF) * 16 < WORK(obj)->homeY - obj->y) {
                    s32 pad; // matching: this block's -g3 scope (and slot) fixes the schedule and frame
                    obj->object_status &= ~0x61;
                    obj->object_status |= 0x20;
                    WORK(obj)->returning = 1;
                }
            }
        } else {
            {
                f32 dx;
                f32 dz;

                dx = movement_obj[0].xpos - obj->x;
                dz = movement_obj[0].zpos - obj->z;
                dx = dx * dx;
                dz = dz * dz;
                distSq = dx + dz;
            }
            if (distSq > 500.0f * 500.0f) {
                obj->object_status &= ~0x61;
                obj->object_status |= 1;
                obj->y = WORK(obj)->homeY;
                WORK(obj)->homeY = 0;
                WORK(obj)->returning = 0;
                WORK(obj)->timer = 0;
            }
        }
    }
}
