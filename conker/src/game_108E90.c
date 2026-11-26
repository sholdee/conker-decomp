#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* The timer task func_151491F4 creates; func_150DBC60 is its tick callback. */
struct PuffTimer {
    u8 pad0[0xC];
    u8 sceneTag;
};


/* Sequence event 0x35 on `owner`: type 0 starts jet preset 1 with a puff, a 30-frame timer that keeps
 * puffing (func_150DBC60) and a particle burst, all at a fixed world position; type 1 starts preset 2. */
void func_150DB9E0(Actor *owner, s32 type, u8 sceneTag) {
    SimplePuffSpray puff;
    BillboardBurst burst;

    if (owner != NULL) {
        switch (type) {
        case 0:
            flamethrower_spawnJet(owner, 1, sceneTag, 0);

            puff.pos.x = 177.9017487f;
            puff.countBase = 8;
            puff.countRange = 8;
            puff.angles.yawBase = -75;
            puff.angles.pitchBase = -38;
            puff.angles.yawRange = 22;
            puff.angles.pitchRange = 35;
            puff.itemCountBase = 3;
            puff.itemCountRange = 2;
            puff.lifeBase = 41;
            puff.lifeRange = 0;
            puff.green = 10;
            puff.red = 40;
            puff.handlers[0] = 1;
            puff.handlers[1] = 0;
            puff.handlers[2] = 0;
            puff.handlers[3] = 0;
            puff.handlers[4] = 0;
            puff.handlers[5] = 0;
            puff.blue = 255;
            puff.alpha = 255;
            puff.sizeBase = 3.0f;
            puff.sizeRange = 3.0f;
            puff.pos.y = -13.38824844f;
            puff.pos.z = -1708.023804f;
            puff.speedBase = 9.0f;
            puff.speedRange = 8.0f;
            puff.growthBase = 0.3199999928f;
            puff.growthRange = 0.0f;
            func_15152F70(&puff, sceneTag);

            func_151491F4(30, -1, 0, 1, 0, 0, sceneTag, 0);

            burst.position.x = 177.9017487f;
            burst.position.y = -13.38824844f;
            burst.position.z = -1708.023804f;
            burst.countBase = 6;
            burst.speedBase = 1285.0f;
            burst.countRange = 3;
            burst.angles.yawBase = -74;
            burst.angles.pitchBase = -38;
            burst.angles.yawRange = 21;
            burst.angles.pitchRange = 35;
            burst.speedRange = 1149.0f;
            burst.lifeBase = 14;
            burst.lifeRange = 14;
            burst.accelYBase = -61;
            burst.accelYRange = 28;
            burst.callback = -1;
            burst.flags = 18;
            burst.fadeBase = 6;
            burst.fadeRange = 6;
            burst.frameStep = 0;
            burst.widthBase = 174;
            burst.widthRange = 115;
            burst.heightBase = 115;
            burst.heightRange = 135;
            burst.anim = &D_80090514;
            burst.color = 255;
            burst.alpha = 255;
            func_15153298(&burst, sceneTag, 1);
            break;
        case 1:
            flamethrower_spawnJet(owner, 2, sceneTag, 0);
            break;
        }
    }
}

/* Tick callback of the timer func_150DB9E0 starts: emits a smaller puff at the same position. */
void func_150DBC60(PuffTimer *timer) {
    SimplePuffSpray puff;

    puff.pos.x = 177.9017487f;
    puff.countBase = 1;
    puff.countRange = 2;
    puff.angles.yawBase = -75;
    puff.angles.pitchBase = -38;
    puff.angles.yawRange = 22;
    puff.angles.pitchRange = 35;
    puff.itemCountBase = 3;
    puff.itemCountRange = 2;
    puff.lifeBase = 41;
    puff.lifeRange = 0;
    puff.sizeBase = 2.0f;
    puff.sizeRange = 2.0f;
    puff.green = 10;
    puff.red = 40;
    puff.handlers[0] = 1;
    puff.handlers[1] = 0;
    puff.handlers[2] = 0;
    puff.handlers[3] = 0;
    puff.handlers[4] = 0;
    puff.handlers[5] = 0;
    puff.blue = 255;
    puff.alpha = 255;
    puff.pos.y = -13.38824844f;
    puff.pos.z = -1708.023804f;
    puff.speedBase = 7.0f;
    puff.speedRange = 5.0f;
    puff.growthBase = 0.3199999928f;
    puff.growthRange = 0.0f;
    func_15152F70(&puff, timer->sceneTag);
}
