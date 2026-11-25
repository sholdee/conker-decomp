#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

void func_1514C2F0(f32 x, f32 y, f32 z, f32 radius, u8 startAngle, s8 angleStep, s16 count,
                   u8 kind, s32 arg8, f32 arg9, s32 arg10, u8 arg11);

/* A trail object; only the fields used here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 lifetime;
    /* 0x1E */ u8 pad1E[0x98 - 0x1E];
    /* 0x98 */ TrailParams *params;
} Trail;

Trail *func_15147DA0(EffectTemplate20 *spawn, TrailMotion *motion, s32 extraSize, s32 state, s32 arg4,
                     s32 finishFn, s32 hitFn, s32 hitKind3Fn, s32 arg8, s32 arg9, s32 arg10,
                     TrailRender *render, s32 arg12, u8 arg13, s32 arg14);

/* Spawns a ring of 25 kind-2 particles (func_150C16C0) of radius 80 around (x, y, z). */
void func_150C1660(f32 x, f32 y, f32 z, u8 arg3) {
    func_1514C2F0(x, y, z, 80.0f, 0, 3, 25, 2, 0, 0.0f, 0, arg3);
}

/*
 * Particle kind 2 spawn callback (D_8008AA00[2]): launches one trail from (x, y, z)
 * outward along `angle` and upward at a random pitch, faded out by func_150C1978.
 */
s32 func_150C16C0(s32 shape, s16 index, f32 x, f32 y, f32 z, f32 originX, f32 originY,
                  f32 originZ, s32 angle, s32 baseAngle, f32 radius, s32 arg11, f32 arg12,
                  s32 arg13, u8 arg14) {
    EffectTemplate20 spawn;
    TrailMotion motion;
    TrailRender render;
    s32 count;
    s16 pitch;
    f32 speed;
    f32 pitchSin;
    f32 pitchCos;
    f32 angleSin;
    f32 angleCos;

    count = 1; // matching: a do/while spawn loop; the for form schedules differently
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
    motion.texture = 10;
    motion.unk1A = 0xFF;
    motion.flags = 40;
    motion.unk1B = 0xFF;
    spawn.pos.x = x;
    spawn.pos.y = y;
    spawn.pos.z = z;

    do {
        pitch = (func_150ADA20() % 13U) - 63;
        pitchSin = func_151423D8(pitch);
        pitchCos = func_151423D8(pitch - 64);
        angleSin = func_151423D8(angle);
        angleCos = func_151423D8(angle - 64);
        speed = func_150ADA68() * 17.4f + 25.5f;
        spawn.itemCount = func_150ADA20() % 9U + 5;
        spawn.life = (func_150ADA20() & 0x3F) + 90;
        motion.halfWidth = func_150ADA68() * 20.2f + 24.7f;
        motion.gravity = func_150ADA68() * 0.2f + 0.4f;
        motion.velocity.x = speed * pitchSin * angleCos;
        motion.velocity.y = -speed * pitchCos;
        motion.velocity.z = speed * pitchSin * angleSin;
        func_15147DA0(&spawn, &motion, 0, 1, 5, 0, 0, 0, 0, 0, 0, &render, 0, arg14, 0);
        count--;
    } while (count != 0);
    return 1;
}

/* Trail fade handler: unk1B follows 8 * lifetime, capped at 0xFF. */
s32 func_150C1978(Trail *trail) {
    TrailParams *params = trail->params;
    s32 alpha = trail->lifetime << 3;

    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    params->motion.unk1B = alpha;
    // matching: never true, but the original tests the low byte here
    if ((alpha & 0xFF) < 0) {
        return 0;
    }
    return 1;
}
