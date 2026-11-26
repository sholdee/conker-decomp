#include <ultra64.h>
#include "functions.h"
#include "variables.h"

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
    u8 pad[4]; // matching: frame size
    EffectTemplate spawn;
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
    motion.shade = 0xFF;
    motion.flags = 40;
    motion.alpha = 0xFF;
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

/* MotionTrail fade handler: unk1B follows 8 * lifetime, capped at 0xFF. */
s32 func_150C1978(MotionTrail *trail) {
    TrailParams *params = trail->params;
    s32 alpha = trail->head.tmpl.life << 3;

    if (alpha >= 0x100) {
        alpha = 0xFF;
    }
    params->motion.alpha = alpha;
    // matching: never true, but the original tests the low byte here
    if ((alpha & 0xFF) < 0) {
        return 0;
    }
    return 1;
}
