#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    u8 unk0[5];
} Struct15100340;

/* This file's .data: the light ids of the five D_800BE500 flags. */
static Struct15100340 D_80088BC0 = { { 9, 10, 11, 12, 13 } };

/* When arg0 is 0: advances D_800DD405, then randomly toggles each of the five D_800BE500 flags,
 * passing the matching D_80088BC0 id to light_sendEvent on each toggle. */
void func_15100340(s32 arg0) {
    s32 pad; // matching: unused stack slot, sets the frame size
    Struct15100340 ids = D_80088BC0;
    s32 i;
    u8 *flag;
    u32 chance;

    if (arg0 == 0) {
        flag = D_800BE500;
        D_800DD405++;
        for (i = 0; i < 5; i++, flag++) {
            chance = (*flag != 0) ? 70 : 35;
            if (func_150ADA20() % 1000U < chance) {
                if ((*flag ^= 1) != 0) {
                    light_sendEvent(ids.unk0[i], 0x18, 0);
                } else {
                    light_sendEvent(ids.unk0[i], 0x1C, 0);
                    light_sendEvent(ids.unk0[i], 0x17, 0);
                }
            }
        }
    }
}

/* Sets segments 2 and 3 to D_80090324 assets cycling with D_800DD405, and segments 4-8 to
 * D_80090324[3] or [4] depending on each D_800BE500 flag. */
Gfx *func_15100464(Gfx *gdl) {
    s32 size[4]; // matching: a 16-byte buffer sets the frame size; a scalar mismatches
    s32 frame;
    s32 i;
    s32 addr;

    frame = D_800DD405 >> 2;

    addr = func_1510D0EC(D_80090324[frame % 3], size, 3, 0);
    gSPSegment(gdl++, 2, addr);

    addr = func_1510D0EC(D_80090324[(frame + 1) % 3], size, 3, 0);
    gSPSegment(gdl++, 3, addr);

    for (i = 0; i < 5; i++) {
        addr = func_1510D0EC(D_80090324[D_800BE500[i] + 3], size, 3, 0);
        gSPSegment(gdl++, i + 4, addr);
    }

    return gdl;
}
