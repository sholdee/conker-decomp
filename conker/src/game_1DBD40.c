#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// func_151D9014 (game_2062D0.c) is declared in each file that calls it: game_1228D0.c, game_161520.c and
// game_1DC720.c were compiled with a different view of it.
SteamParticle *func_151D9014(Vec3f *pos, const Vec3f *vel, u8 kind, f32 gravity, s16 lifetime, u8 arg5, f32 size, s32 arg7,
                             f32 burstScale, f32 altBurstScale, u8 argA, CollisionHit *ground, s32 wobbles, s32 bounces,
                             u8 argE, s32 argF);

/* Randomly spawns a particle at (x, y, z): 15% nothing, 10% a random-direction one (func_151AEAB4),
 * 75% one moving against (dirX, dirY, dirZ) (func_151AE984). arg6[0xC] is passed through. */
void func_151AE890(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, u8 *arg6) {
    Vec3f pos;
    f32 roll;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    roll = func_150ADA68();
    if (roll < 0.15000000596f) {
        return;
    }
    if (roll < 0.25f) {
        u8 flag;

        roll = func_150ADA68();
        flag = (roll > 0.0f) ? 1 : 0; // matching: a plain `roll > 0.0f` compiles differently
        func_151AEAB4(&pos, flag, arg6[0xC]);
    } else {
        s32 flag;

        roll = func_150ADA68();
        flag = (roll > 0.8050000668f) ? 1 : 0;
        func_151AE984(&pos, dirX, dirY, dirZ, flag, arg6[0xC]);
    }
}

/* Spawns a particle at pos moving against (dirX, dirY, dirZ), with randomized parameters. */
void func_151AE984(Vec3f *pos, f32 dirX, f32 dirY, f32 dirZ, s32 flag, u8 arg5) {
    f32 scale;
    Vec3f velocity;
    f32 shade;
    u32 rand2;
    u32 rand1;
    f32 rand0;

    scale = 0.1030000076f;
    velocity.x = -dirX * scale;
    velocity.y = -dirY * scale;
    velocity.z = -dirZ * scale;
    rand0 = func_150ADA68();
    rand1 = func_150ADA20();
    rand2 = func_150ADA20();
    shade = func_150ADA68() * 219.0f + 77.0f;
    // matching: the rodata holds -1.045f, so this is an add of a negative literal, not a sub
    func_151D9014(pos, &velocity, 0, rand0 * 0.6020000577f + -1.045000076f, rand1 % 31 + 60, rand2 % 101 + 155,
                  shade, (u8)flag, 1.0f, 1.0f, 0, 0, 1, 0, arg5, 1);
}

/* Spawns a particle at pos moving in a random direction at a random speed, with randomized parameters.
 * func_151423D8 is cos() over a 256-step circle, so cos(a - 0x40) is sin(a). */
void func_151AEAB4(Vec3f *pos, u8 flag, u8 arg2) {
    u32 phase; // matching: passing yaw - 0x40 through a u32 temp is needed for identical bytes
    Vec3f velocity;
    u8 yaw;
    u8 pitch;
    f32 horizontal;
    f32 yawCos;
    f32 yawSin;
    f32 pitchCos;
    f32 pitchSin;
    f32 speed;

    yaw = func_150ADA20() & 0xFF; // matching: the redundant mask is needed for identical bytes
    pitch = (func_150ADA20() & 0x7F) - 0x3F;
    yawCos = func_151423D8(yaw);
    phase = yaw - 0x40;
    yawSin = func_151423D8(phase);
    pitchCos = func_151423D8(pitch);
    pitchSin = func_151423D8(pitch - 0x40);
    speed = (func_150ADA68() * 80.0f + 20.0f) * 0.1000000015f;
    horizontal = speed * pitchCos;
    velocity.x = horizontal * yawSin;
    velocity.y = -speed * pitchSin;
    velocity.z = horizontal * yawCos;
    {
        f32 rand3;
        u32 rand2;
        u32 rand1;
        f32 rand0;

        rand0 = func_150ADA68();
        rand1 = func_150ADA20();
        rand2 = func_150ADA20();
        rand3 = func_150ADA68();
        func_151DA6F8(pos, &velocity, rand0 * 0.4060000181f + 0.2100000083f, rand1 % 41 + 60, rand2 % 101 + 155,
                      rand3 * 4.0f + 3.100000143f, func_150ADA20() % 5 + 3, flag, 1.0f, 1.0f, 0, 0, 0, 16, 15, 0,
                      arg2, 1);
    }
}
