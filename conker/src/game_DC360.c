#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Trail types (see game_175250.c). */

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Vec3f position;
    /* 0x1C */ s16 unk1C; /* counts down; copied from EffectTemplate.unkC */
    /* 0x1E */ u8 pad1E[0x7A];
    /* 0x98 */ TrailParams *params;
} Trail;

extern Trail *func_15147DA0(EffectTemplate *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 arg4,
                            s32 finishFn, s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 arg9, s32 arg10,
                            TrailRender *render, s32 arg12, u8 arg13, s32 arg14);

/* Sprays up to 30 trails from a ring around obj; stops early when a trail can't be created. */
void func_150AEEB0(Actor *obj, u8 owner) {
    EffectTemplate spawn;
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
        render.setGeometryMode = 1;
        render.clearGeometryMode = 0x160600;
        render.renderMode = 3;
        render.unk10 = 0x10;
        render.unk14 = 0x80;
        render.unk18 = 0x20;
        render.primMode = 0;
        render.envMode = 9;
        spawn.kind = 1;
        spawn.flags = 1;
        motion.texture = 8;
        motion.shade = 0xFF - (((u32)obj->unk184 >> 5) & 3) * 64;
        motion.flags = 0x28;
        motion.alpha = 0xFF;
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
            spawn.itemCount = func_150ADA20() % 7U + 3;
            spawn.life = func_150ADA20() % 26U + 35;
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
    params->motion.alpha = alpha;
    if ((u8)alpha < 0) { // matching: never true, but the original emits the andi/bgez test
        return 0;
    }
    return 1;
}
