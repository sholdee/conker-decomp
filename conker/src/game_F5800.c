#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* D_800BE4E0: ten BounceRecords (allocated and seeded by func_1500FE30). */
#define RECORD_COUNT 10
#define POS_LIMIT 0x500

/* This file's .data. */
static TextureFrameTables D_80088810 = { { D_80090230, D_80090244 } };


/* Ease every record's speed towards its target, then advance pos, bouncing at 0 and POS_LIMIT. */
void func_150C8350(void) {
    s32 i;
    s32 target;
    s32 diff;
    s32 step;
    s32 dist;
    s32 sign;
    s32 pos;

    for (i = 0; i < RECORD_COUNT; i++) {
        if (D_800BE4E0[i].speed != D_800BE4E0[i].targetSpeed) {
            target = D_800BE4E0[i].targetSpeed;
            if (D_800BE4E0[i].speed < 0) {
                target = -target;
            }
            diff = target - D_800BE4E0[i].speed;
            step = D_800BE9E4 * 16;
            dist = diff < 0 ? -diff : diff;
            if (dist < step) {
                D_800BE4E0[i].speed = target;
            } else {
                if (diff < 0) {
                    sign = -1;
                } else {
                    sign = 1;
                }
                D_800BE4E0[i].speed = D_800BE4E0[i].speed + step * sign;
            }
        }
        D_800BE4E0[i].pos += D_800BE4E0[i].speed * D_800BE4E0[i].dir * D_800BE9A0;
        pos = D_800BE4E0[i].pos;
        if (pos >= POS_LIMIT) {
            D_800BE4E0[i].pos = pos - POS_LIMIT;
            D_800BE4E0[i].pos = POS_LIMIT - D_800BE4E0[i].pos;
            D_800BE4E0[i].dir = -D_800BE4E0[i].dir;
            if (D_800BE4E0[i].pos >= POS_LIMIT) {
                D_800BE4E0[i].pos = POS_LIMIT - 1;
            }
        } else if (pos < 0) {
            D_800BE4E0[i].pos = -pos;
            D_800BE4E0[i].dir = -D_800BE4E0[i].dir;
            if (D_800BE4E0[i].pos < 0) {
                D_800BE4E0[i].pos = 0;
            }
        }
    }
}

/* Advances the records (func_150C8350) when arg0 is 0. */
void func_150C84F4(s32 arg0) {
    if (arg0 == 0) {
        func_150C8350();
    }
}

/* Gives every record a random target speed: scale * 300 / 256 plus 0 .. scale * 50 / 256 + 49. */
void func_150C851C(s32 scale) {
    s32 i;
    s32 speed;
    s32 minSpeed;
    s32 range;

    if (D_800BE4E0 != NULL) {
        func_150CDB6C(scale);
        range = ((scale * 50) >> 8) + 50;
        minSpeed = (scale * 300) >> 8;
        for (i = 0; i < RECORD_COUNT; i++) {
            speed = minSpeed; // matching: one combined expression shifts the code
            speed += func_150ADA20() % range;
            D_800BE4E0[i].targetSpeed = speed;
        }
    }
}


/* Binds record i's current frame (pos >> 8, clamped to 0-4) from its table to segment i + 1. */
Gfx *func_150C8600(Gfx *gdl) {
    s32 i;
    s32 segment;
    s32 frame;
    s32 size;
    s32 addr;
    TextureFrameTables tables = D_80088810;

    for (i = 0; i < RECORD_COUNT; i++) {
        frame = D_800BE4E0[i].pos >> 8;
        if (frame < 0) {
            frame = 0;
        } else if (frame >= 5) {
            frame = 4;
        }
        if (D_800BE4E0[i].table >= 2) {
            D_800BE4E0[i].table = 0;
        }
        addr = texture_getAddress(tables.frames[D_800BE4E0[i].table][frame], &size, 3, 0);
        segment = i + 1;
        gSPSegment(gdl++, segment, addr);
    }
    return gdl;
}
