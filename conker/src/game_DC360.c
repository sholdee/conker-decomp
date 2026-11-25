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
    u8 pad16[6];
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

extern Trail *func_15147DA0(TrailSpawn *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 arg4,
                            s32 finishFn, s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 arg9, s32 arg10,
                            TrailRender *render, s32 arg12, u8 arg13, s32 arg14);

/* Sprays up to 30 trails from a ring around obj; stops early when a trail can't be created. */
void func_150AEEB0(struct127 *obj, u8 owner) {
    TrailSpawn spawn;
    TrailMotion motion;
    s32 remaining;
    s32 ok;
    TrailRender render;
    u8 yaw;
    u8 pitch;
    f32 speed;
    f32 sinPitch;
    f32 cosPitch;
    f32 sinYaw;
    f32 cosYaw;

    remaining = 30;
    ok = TRUE;
    if (obj != NULL) {
        render.unk0 = 0;
        render.unk4 = 1;
        render.unk8 = 0x160600;
        render.renderMode = 3;
        render.unk10 = 0x10;
        render.unk14 = 0x80;
        render.unk18 = 0x20;
        render.unk1C = 0;
        render.unk1D = 9;
        spawn.unk10 = 1;
        spawn.unkE = 1;
        motion.texture = 8;
        motion.unk1A = 0xFF - (((u32)obj->unk184 >> 5) & 3) * 64;
        motion.flags = 0x28;
        motion.unk1B = 0xFF;
        do {
            yaw = func_150ADA20() & 0xFF; // matching: the redundant mask changes the schedule
            pitch = -(func_150ADA20() % 21U + 40);
            sinPitch = func_151423D8(pitch);
            cosPitch = func_151423D8(pitch - 0x40);
            sinYaw = func_151423D8(yaw);
            cosYaw = func_151423D8(yaw - 0x40);
            speed = func_150ADA68() * 5.0f + 10.0f;
            spawn.pos.x = obj->x_position + 100.0f * cosYaw;
            spawn.pos.y = func_150ADA68() * 15.0f + obj->y_position;
            spawn.pos.z = obj->z_position + 100.0f * sinYaw;
            spawn.pointCount = func_150ADA20() % 7U + 3;
            spawn.unkC = func_150ADA20() % 26U + 35;
            motion.halfWidth = func_150ADA68() * 6.0f + 2.0f;
            motion.gravity = func_150ADA68() + 1.0f;
            motion.velocity.x = speed * sinPitch * cosYaw;
            motion.velocity.y = -speed * cosPitch;
            motion.velocity.z = speed * sinPitch * sinYaw;
            if (func_15147DA0(&spawn, &motion, 0, 1, 3, 0, 0, 0, 0, 0, 0, &render, 0, owner, 0) == NULL) {
                ok = FALSE;
            }
        } while (ok && --remaining != 0);
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
