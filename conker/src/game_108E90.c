#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Parameter block for func_15152F70 (struct Local15152F70Arg in game_17CAF0.c). Integer fields are
 * base/range pairs, (random % (range + 1)) + base; float pairs are random01 * range + base. */
typedef struct {
    Vec3f position;
    f32 sizeBase;
    f32 sizeRange;
    f32 growthBase;
    f32 growthRange;
    f32 speedBase;
    f32 speedRange;
    s16 countBase;
    s16 countRange;
    s16 yawBase;
    s16 yawRange;
    s16 pitchBase;
    s16 pitchRange;
    s16 fadeBase;
    s16 fadeRange;
    s16 lifeBase;
    s16 lifeRange;
    s8 green;
    s8 red;
    u8 pad3A[2];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 blue;
    u8 alpha;
    u8 pad56[2];
} PuffConfig;

/* Parameter block for func_15153298 (Game153298Config in game_17CAF0.c). */
typedef struct {
    Vec3f position;
    s16 countBase;
    s16 countRange;
    s16 yawBase;
    s16 yawRange;
    s16 pitchBase;
    s16 pitchRange;
    f32 speedBase;
    f32 speedRange;
    s16 lifeBase;
    s16 lifeRange;
    s16 sizeBase;
    s16 sizeRange;
    s8 mode;
    s8 kind;
    s16 fadeBase;
    s16 fadeRange;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    void *texture;
    u8 color;
    u8 alpha;
    u8 pad3E[2];
} BurstConfig;

/* The timer task func_151491F4 creates; func_150DBC60 is its tick callback. */
typedef struct {
    u8 pad0[0xC];
    u8 sceneTag;
} PuffTimer;

extern u8 D_80090514[];

void func_15152F70(PuffConfig *config, u8 sceneTag);
void func_15153298(BurstConfig *config, u8 sceneTag, s32 arg2);
void *func_15196438(Actor *owner, s32 preset, u8 sceneTag, s32 arg3);

/* Sequence event 0x35 on `owner`: type 0 starts jet preset 1 with a puff, a 30-frame timer that keeps
 * puffing (func_150DBC60) and a particle burst, all at a fixed world position; type 1 starts preset 2. */
void func_150DB9E0(Actor *owner, s32 type, u8 sceneTag) {
    PuffConfig puff;
    BurstConfig burst;

    if (owner != NULL) {
        switch (type) {
        case 0:
            func_15196438(owner, 1, sceneTag, 0);

            puff.position.x = 177.9017487f;
            puff.countBase = 8;
            puff.countRange = 8;
            puff.yawBase = -75;
            puff.pitchBase = -38;
            puff.yawRange = 22;
            puff.pitchRange = 35;
            puff.fadeBase = 3;
            puff.fadeRange = 2;
            puff.lifeBase = 41;
            puff.lifeRange = 0;
            puff.green = 10;
            puff.red = 40;
            puff.unk3C = 1;
            puff.unk40 = 0;
            puff.unk44 = 0;
            puff.unk48 = 0;
            puff.unk4C = 0;
            puff.unk50 = 0;
            puff.blue = 255;
            puff.alpha = 255;
            puff.sizeBase = 3.0f;
            puff.sizeRange = 3.0f;
            puff.position.y = -13.38824844f;
            puff.position.z = -1708.023804f;
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
            burst.yawBase = -74;
            burst.pitchBase = -38;
            burst.yawRange = 21;
            burst.pitchRange = 35;
            burst.speedRange = 1149.0f;
            burst.lifeBase = 14;
            burst.lifeRange = 14;
            burst.sizeBase = -61;
            burst.sizeRange = 28;
            burst.mode = -1;
            burst.kind = 18;
            burst.fadeBase = 6;
            burst.fadeRange = 6;
            burst.unk2E = 0;
            burst.unk30 = 174;
            burst.unk32 = 115;
            burst.unk34 = 115;
            burst.unk36 = 135;
            burst.texture = D_80090514;
            burst.color = 255;
            burst.alpha = 255;
            func_15153298(&burst, sceneTag, 1);
            break;
        case 1:
            func_15196438(owner, 2, sceneTag, 0);
            break;
        }
    }
}

/* Tick callback of the timer func_150DB9E0 starts: emits a smaller puff at the same position. */
void func_150DBC60(PuffTimer *timer) {
    PuffConfig puff;

    puff.position.x = 177.9017487f;
    puff.countBase = 1;
    puff.countRange = 2;
    puff.yawBase = -75;
    puff.pitchBase = -38;
    puff.yawRange = 22;
    puff.pitchRange = 35;
    puff.fadeBase = 3;
    puff.fadeRange = 2;
    puff.lifeBase = 41;
    puff.lifeRange = 0;
    puff.sizeBase = 2.0f;
    puff.sizeRange = 2.0f;
    puff.green = 10;
    puff.red = 40;
    puff.unk3C = 1;
    puff.unk40 = 0;
    puff.unk44 = 0;
    puff.unk48 = 0;
    puff.unk4C = 0;
    puff.unk50 = 0;
    puff.blue = 255;
    puff.alpha = 255;
    puff.position.y = -13.38824844f;
    puff.position.z = -1708.023804f;
    puff.speedBase = 7.0f;
    puff.speedRange = 5.0f;
    puff.growthBase = 0.3199999928f;
    puff.growthRange = 0.0f;
    func_15152F70(&puff, timer->sceneTag);
}
