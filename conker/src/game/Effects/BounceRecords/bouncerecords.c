#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* gBounceRecords: ten BounceRecords (allocated and seeded by level_initBounceRecords). */
#define RECORD_COUNT 10
#define POS_LIMIT 0x500

/* This file's .data. */
static TextureFrameTables gBounceRecordFrameTables = { { D_80090230, D_80090244 } };


/* Ease every record's speed towards its target, then advance pos, bouncing at 0 and POS_LIMIT. */
void effect_updateBounceRecords(void) {
    s32 i;
    s32 target;
    s32 diff;
    s32 step;
    s32 dist;
    s32 sign;
    s32 pos;

    for (i = 0; i < RECORD_COUNT; i++) {
        if (gBounceRecords[i].speed != gBounceRecords[i].targetSpeed) {
            target = gBounceRecords[i].targetSpeed;
            if (gBounceRecords[i].speed < 0) {
                target = -target;
            }
            diff = target - gBounceRecords[i].speed;
            step = gFrameTicks * 16;
            dist = diff < 0 ? -diff : diff;
            if (dist < step) {
                gBounceRecords[i].speed = target;
            } else {
                if (diff < 0) {
                    sign = -1;
                } else {
                    sign = 1;
                }
                gBounceRecords[i].speed = gBounceRecords[i].speed + step * sign;
            }
        }
        gBounceRecords[i].pos += gBounceRecords[i].speed * gBounceRecords[i].dir * gFrameSteps;
        pos = gBounceRecords[i].pos;
        if (pos >= POS_LIMIT) {
            gBounceRecords[i].pos = pos - POS_LIMIT;
            gBounceRecords[i].pos = POS_LIMIT - gBounceRecords[i].pos;
            gBounceRecords[i].dir = -gBounceRecords[i].dir;
            if (gBounceRecords[i].pos >= POS_LIMIT) {
                gBounceRecords[i].pos = POS_LIMIT - 1;
            }
        } else if (pos < 0) {
            gBounceRecords[i].pos = -pos;
            gBounceRecords[i].dir = -gBounceRecords[i].dir;
            if (gBounceRecords[i].pos < 0) {
                gBounceRecords[i].pos = 0;
            }
        }
    }
}

/* Advances the records (effect_updateBounceRecords) when arg0 is 0. */
void effect_levelUpdateBounceRecords(s32 arg0) {
    if (arg0 == 0) {
        effect_updateBounceRecords();
    }
}

/* Gives every record a random target speed: scale * 300 / 256 plus 0 .. scale * 50 / 256 + 49. */
void effect_randomizeBounceRecordSpeeds(s32 scale) {
    s32 i;
    s32 speed;
    s32 minSpeed;
    s32 range;

    if (gBounceRecords != NULL) {
        particle_setDriftBurstRate(scale);
        range = ((scale * 50) >> 8) + 50;
        minSpeed = (scale * 300) >> 8;
        for (i = 0; i < RECORD_COUNT; i++) {
            speed = minSpeed; // matching: one combined expression shifts the code
            speed += math_random() % range;
            gBounceRecords[i].targetSpeed = speed;
        }
    }
}


/* Binds record i's current frame (pos >> 8, clamped to 0-4) from its table to segment i + 1. */
Gfx *effect_bindBounceRecordFrames(Gfx *gdl) {
    s32 i;
    s32 segment;
    s32 frame;
    s32 size;
    s32 addr;
    TextureFrameTables tables = gBounceRecordFrameTables;

    for (i = 0; i < RECORD_COUNT; i++) {
        frame = gBounceRecords[i].pos >> 8;
        if (frame < 0) {
            frame = 0;
        } else if (frame >= 5) {
            frame = 4;
        }
        if (gBounceRecords[i].table >= 2) {
            gBounceRecords[i].table = 0;
        }
        addr = texture_getAddress(tables.frames[gBounceRecords[i].table][frame], &size, 3, 0);
        segment = i + 1;
        gSPSegment(gdl++, segment, addr);
    }
    return gdl;
}
