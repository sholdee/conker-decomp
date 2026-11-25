#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* One of the ten records in the D_800BE4E0 buffer (allocated and seeded by func_1500FE30). */
typedef struct GameF5800Record {
    u8 table;        /* which D_80088810 table to draw from (0 or 1) */
    u8 pad1;
    s8 dir;          /* +1 / -1, flips when pos bounces off either end */
    u8 pad3;
    s16 pos;         /* bounces within [0, 0x500); pos >> 8 picks the table entry */
    s16 speed;       /* eased towards targetSpeed each tick */
    s16 targetSpeed; /* re-randomised by func_150C851C */
} GameF5800Record;

#define RECORD_COUNT 10
#define POS_LIMIT 0x500

/* D_800BE4E0 is declared as s32 in variables.h; view it as the record array. */
#define sRecords ((GameF5800Record *)D_800BE4E0)

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
        if (sRecords[i].speed != sRecords[i].targetSpeed) {
            target = sRecords[i].targetSpeed;
            if (sRecords[i].speed < 0) {
                target = -target;
            }
            diff = target - sRecords[i].speed;
            step = D_800BE9E4 * 16;
            dist = diff < 0 ? -diff : diff;
            if (dist < step) {
                sRecords[i].speed = target;
            } else {
                if (diff < 0) {
                    sign = -1;
                } else {
                    sign = 1;
                }
                sRecords[i].speed = sRecords[i].speed + step * sign;
            }
        }
        sRecords[i].pos += sRecords[i].speed * sRecords[i].dir * D_800BE9A0;
        pos = sRecords[i].pos;
        if (pos >= POS_LIMIT) {
            sRecords[i].pos = pos - POS_LIMIT;
            sRecords[i].pos = POS_LIMIT - sRecords[i].pos;
            sRecords[i].dir = -sRecords[i].dir;
            if (sRecords[i].pos >= POS_LIMIT) {
                sRecords[i].pos = POS_LIMIT - 1;
            }
        } else if (pos < 0) {
            sRecords[i].pos = -pos;
            sRecords[i].dir = -sRecords[i].dir;
            if (sRecords[i].pos < 0) {
                sRecords[i].pos = 0;
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

void func_150CDB6C(s32);

/* Gives every record a random target speed: scale * 300 / 256 plus 0 .. scale * 50 / 256 + 49. */
void func_150C851C(s32 scale) {
    s32 i;
    s32 speed;
    s32 minSpeed;
    s32 range;

    if (D_800BE4E0 != 0) {
        func_150CDB6C(scale);
        range = ((scale * 50) >> 8) + 50;
        minSpeed = (scale * 300) >> 8;
        for (i = 0; i < RECORD_COUNT; i++) {
            speed = minSpeed; // matching: one combined expression shifts the code
            speed += (u32)func_150ADA20() % range;
            sRecords[i].targetSpeed = speed;
        }
    }
}

/* Two texture-id tables of five animation frames each. */
typedef struct {
    s32 *frames[2];
} TextureFrameTables;
extern TextureFrameTables D_80088810; /* = { D_80090230, D_80090244 } */
extern s32 func_1510D0EC(s32, s32 *, s32, s32);

/* Binds record i's current frame (pos >> 8, clamped to 0-4) from its table to segment i + 1. */
Gfx *func_150C8600(Gfx *gdl) {
    s32 i;
    s32 segment;
    s32 frame;
    s32 size;
    s32 addr;
    TextureFrameTables tables = D_80088810;

    for (i = 0; i < RECORD_COUNT; i++) {
        frame = sRecords[i].pos >> 8;
        if (frame < 0) {
            frame = 0;
        } else if (frame >= 5) {
            frame = 4;
        }
        if (sRecords[i].table >= 2) {
            sRecords[i].table = 0;
        }
        addr = func_1510D0EC(tables.frames[sRecords[i].table][frame], &size, 3, 0);
        segment = i + 1;
        gSPSegment(gdl++, segment, addr);
    }
    return gdl;
}
