#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern s32 D_80090324[];
extern s32 func_1510D0EC(s32, s32, s32, s32);
extern void func_151616D0(u8, u8, s32);

typedef struct {
    u8 unk0[5];
} Struct15100340;

extern Struct15100340 D_80088BC0;

#define WGFX2(pkt, a, b)            \
{                                   \
    Gfx *_g = (Gfx *)(pkt);         \
    _g->words.w0 = (u32)(a);        \
    _g->words.w1 = (u32)(b);        \
}

/* Each call with arg0 == 0, randomly toggles the five D_800BE500 flags and notifies func_151616D0. */
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
                    func_151616D0(ids.unk0[i], 0x18, 0);
                } else {
                    func_151616D0(ids.unk0[i], 0x1C, 0);
                    func_151616D0(ids.unk0[i], 0x17, 0);
                }
            }
        }
    }
}

Gfx *func_15100464(Gfx *arg0) {
    s32 sp58[4];
    s32 *scratch;
    s32 three;
    s32 idx;
    s32 data;
    u8 *states;

    scratch = sp58;
    three = 3;
    idx = D_800DD405 >> 2;

    data = func_1510D0EC(D_80090324[idx % three], (s32)scratch, three, 0);
    WGFX2(arg0++, 0xDB060008, data);

    data = func_1510D0EC(D_80090324[(idx + 1) % three], (s32)scratch, three, 0);
    WGFX2(arg0++, 0xDB06000C, data);

    states = D_800BE500;
    idx = 0x10;
    do {
        data = func_1510D0EC(D_80090324[states[0] + 3], (s32)scratch, 3, 0);
        WGFX2(arg0++, 0xDB060000 | (idx & 0xFFFF), data);
        idx += 4;
        states++;
    } while (idx != 0x24);

    return arg0;
}
