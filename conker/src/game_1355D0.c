#include <ultra64.h>
#include "functions.h"
// matching: callers here treat the random number as u32 (functions.h declares it u8)
#include "variables.h"

/* Debris description passed to func_15152190 (same layout as game_DC6B0.c's DebrisBurst): a random
 * number of pieces thrown in random directions from origin. */
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

extern s32 D_800A2430;
extern f32 D_800A2434;
void func_150E83AC(vertex *position, s16 duration, u8 owner, s32 kind);
void func_15136C3C(Actor *, s32, s32, s32, s32, s32, u8, s32);
void func_15152190(DebrisBurst *, s32 *variantIds, f32 *variantScales, s32 variantCount, f32 radius, s32, u8,
                   s32);

/* Plays an effect at obj lasting 500-700 frames, throws debris from 30 units above it, then calls
 * func_15136C3C on obj. */
void func_15108120(Actor *obj, u8 arg1, s32 arg2) {
    vertex pos;
    vertex debrisOrigin;

    pos.x = obj->x_position;
    pos.y = obj->y_position;
    pos.z = obj->z_position;
    debrisOrigin.x = obj->x_position;
    debrisOrigin.y = obj->y_position + 30.0f;
    debrisOrigin.z = obj->z_position;
    func_150E83AC(&pos, (func_150ADA20() % 201) + 500, arg1, arg2);
    {
        DebrisBurst debris;

        debris.count = 15;
        debris.countRange = 8;
        debris.origin = debrisOrigin;
        debris.yaw = 0;
        debris.yawRange = 0xFF;
        debris.pitch = -64;
        debris.pitchRange = 41;
        debris.unk2C = 39;
        debris.unk2E = 20;
        debris.scale = 0.3050000072f;
        debris.scaleRange = 0.3050000072f;
        debris.speed = 8.0f;
        debris.speedRange = 10.0f;
        debris.unk24 = -0.8000000119f;
        debris.unk28 = 0.3440000117f;
        debris.spin = 26.41100121f;
        func_15152190(&debris, &D_800A2430, &D_800A2434, 1, 40.0f, 0, arg1, arg2);
    }
    func_15136C3C(obj, 1, 1, 1, 0, 0, arg1, arg2);
}
