#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Trail types (see game_175250.c). */

/* Spawn description passed to func_15147DA0 (first argument). */
typedef struct {
    Vec3f pos;
    s16 unkC; /* becomes Trail.unk1C */
    s16 unkE;
    s32 unk10;
    u8 pad14;
    u8 pointCount;
    u8 pad16[0xA];
} TrailSpawn;

/* Motion and appearance block that func_15147DA0 copies from its second argument. */
typedef struct {
    f32 halfWidth;
    Vec3f velocity;
    f32 gravity;
    f32 unk14;
    u8 flags;
    u8 texture;
    u8 unk1A;
    u8 unk1B; /* alpha */
    s16 unk1C;
    s16 unk1E;
} TrailMotion; /* size 0x20 */

/* Render settings that func_15147DA0 copies from its 12th argument. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 renderMode;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 unk1C;
    u8 unk1D;
} TrailRender; /* size 0x20 */

typedef struct {
    TrailMotion motion;
    u8 state; /* 0x20: handler index into D_8008A3E0 */
    u8 unk21; /* handler index into D_8008A3F8 */
    u8 finishFn;
    u8 hitFn;
    u8 hitKind3Fn;
    u8 unk25;
    u8 pad26[2];
    TrailRender render; /* 0x28 */
} TrailParams; /* size 0x48 */

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Vec3f position;
    /* 0x1C */ s16 unk1C; /* counts down; copied from TrailSpawn.unkC */
    /* 0x1E */ u8 pad1E[0x7A];
    /* 0x98 */ TrailParams *params;
} Trail;

s32 func_15147DA0(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                  s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10, s32 *arg11,
                  s32 arg12, u8 arg13, s32 arg14);

typedef struct {
    char pad00[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    char pad20[0x184 - 0x20];
    u32 unk184;
} Struct150AEEB0;

void func_150AEEB0(Struct150AEEB0 *arg0, u8 arg1) {
    struct { f32 x; f32 y; f32 z; s16 unk0C; s16 unk0E; s32 unk10; u8 pad14; u8 unk15; u8 pad16[2]; s32 pad18; } sp104;
    struct { f32 unk00; f32 unk04; f32 unk08; f32 unk0C; f32 unk10; u8 pad14[4]; u8 unk18; u8 unk19; u8 unk1A; u8 unk1B; s32 pad1C; } spE4;
    s32 i;
    s32 ok;
    struct { s32 unk00; s32 unk04; s32 unk08; s32 unk0C; s32 unk10; s32 unk14; s32 unk18; u8 unk1C; u8 unk1D; } spBC;
    u8 dir;
    u8 ang;
    f32 mag;
    f32 spA;
    f32 spB;
    f32 spC;
    f32 spD;

    i = 30;
    ok = 1;
    if (arg0 != NULL) {
        spBC.unk00 = 0;
        spBC.unk04 = 1;
        spBC.unk08 = 0x160600;
        spBC.unk0C = 3;
        spBC.unk10 = 0x10;
        spBC.unk14 = 0x80;
        spBC.unk18 = 0x20;
        spBC.unk1C = 0;
        spBC.unk1D = 9;
        sp104.unk10 = 1;
        sp104.unk0E = 1;
        spE4.unk19 = 8;
        spE4.unk1A = 0xFF - (((arg0->unk184 >> 5) & 3) << 6);
        spE4.unk18 = 0x28;
        spE4.unk1B = 0xFF;
        do {
            dir = func_150ADA20() & 0xFF;
            ang = -(func_150ADA20() % 21U) - 0x28;
            spA = func_151423D8(ang);
            spB = func_151423D8((u8)(ang - 0x40));
            spC = func_151423D8(dir);
            spD = func_151423D8((u8)(dir - 0x40));
            mag = (func_150ADA68() * 5.0f) + 10.0f;
            sp104.x = arg0->unk14 + (100.0f * spD);
            sp104.y = (func_150ADA68() * 15.0f) + arg0->unk18;
            sp104.z = arg0->unk1C + (100.0f * spC);
            sp104.unk15 = (func_150ADA20() % 7U) + 3;
            sp104.unk0C = (func_150ADA20() % 26U) + 0x23;
            spE4.unk00 = (func_150ADA68() * 6.0f) + 2.0f;
            spE4.unk10 = func_150ADA68() + 1.0f;
            spE4.unk04 = (mag * spA) * spD;
            spE4.unk08 = -mag * spB;
            spE4.unk0C = (mag * spA) * spC;
            if (func_15147DA0(&sp104, &spE4, 0, 1, 3, 0, 0, 0, 0, 0, 0, (s32 *)&spBC, 0, arg1, 0) == 0) {
                ok = 0;
            }
        } while (ok != 0 && --i != 0);
    }
}

/* Trail handler (D_8008A3F8[3]): fades the trail out over its last 8 frames. */
s32 func_150AF1C0(Trail *trail) {
    TrailParams *params = trail->params;
    s32 alpha = trail->unk1C * 32;

    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    params->motion.unk1B = alpha;
    if ((u8)alpha < 0) { // matching: never true, but the original emits the andi/bgez test
        return 0;
    }
    return 1;
}
