#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void func_15103690(s32 arg0) {
    func_15103828();
}

/* Debris description passed to func_15152190 (same layout as game_DC6B0.c's and game_1355D0.c's
 * DebrisBurst): a random number of pieces thrown in random directions from origin. */
typedef struct {
    /* 0x00 */ s32 count;
    /* 0x04 */ s32 countRange;
    /* 0x08 */ vertex origin;
    /* 0x14 */ s16 yaw;
    /* 0x16 */ s16 yawRange;
    /* 0x18 */ s16 pitch;
    /* 0x1A */ s16 pitchRange;
    /* 0x1C */ f32 speed;
    /* 0x20 */ f32 speedRange;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ f32 scale;
    /* 0x34 */ f32 scaleRange;
    /* 0x38 */ f32 spin;
} DebrisBurst;

void func_15152190(DebrisBurst *, s32 *variantIds, f32 *variantScales, s32 variantCount, f32 radius, s32, u8,
                   s32);

/* Throws a burst of debris (one variant, id 226) from origin. */
void func_151036B4(vertex *origin, u8 arg1, s32 arg2) {
    DebrisBurst debris;
    s32 variantIds[] = { 226 };
    f32 variantScales[] = { 1.0f };

    debris.count = 8;
    debris.countRange = 4;
    debris.origin = *origin;
    debris.yaw = 0;
    debris.yawRange = 0xFF;
    debris.pitch = -64;
    debris.pitchRange = 93;
    debris.speed = 8.0f;
    debris.speedRange = 4.0f;
    debris.unk24 = -0.7060000300f;
    debris.unk28 = 0.3220000267f;
    debris.unk2C = 20;
    debris.unk2E = 10;
    debris.scale = 0.2510000169f;
    debris.scaleRange = 0.1990000159f;
    debris.spin = 30.69600105f;
    func_15152190(&debris, variantIds, variantScales, 1, 0.0f, 1, arg1, arg2);
}

s32 func_151037DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return 0;
}
