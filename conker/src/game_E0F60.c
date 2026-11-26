#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern Vec3f D_8009FBC0; // a point in bone space

/* Spawns an effect at bone 25 of obj, plus a camera event (type 1, 12-27 long) and a func_151D3FF4 at the same point. */
void func_150B3AB0(Actor *obj, u8 arg1) {
    s16 yaw;
    Vec3f pos;
    RumbleParams event;

    if ((obj != NULL) && (obj->ob_matrix_ptr != NULL)) {
        func_1512D748(&camera_global_Cameras[D_800BE9E8], 6, 1);
        yaw = (obj->langle >> 8) + 0x40;
        func_15143134(&D_8009FBC0, &pos, obj->ob_matrix_ptr + 25);
        // matching: *(u8 *)&arg1 re-reads arg1 from its stack slot at each use (plain or K&R arg1 stays in a register)
        func_1514C858(pos.x, pos.y, pos.z, 10.0f, yaw, 0, 0xFF,
                      (func_150ADA20() % 31U) + 40, 8, 0, 0.0f, 0, *(u8 *)&arg1);

        event.flags = 1;
        event.duration = (func_150ADA20() & 0xF) + 12;
        event.strength = (func_150ADA20() & 3) + 5;
        event.updateKind = -1;
        event.playerMask = 1;
        func_151D8868(&event, 0, *(u8 *)&arg1, 0);
        func_151D3FF4(&pos, *(u8 *)&arg1, 0);
    }
}

/* Spawns a streak particle at (x, y, z) heading along yaw/pitch (256-step angles) at speed 10. */
s32 func_150B3C0C(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s32 pitch, s32 yaw, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 owner) {
    f32 yawCos;
    f32 yawSin;
    f32 pitchCos;
    f32 horizSpeed;
    StreakParticleDesc desc;

    yawCos = func_151423D8(yaw - 0x40);
    yawSin = func_151423D8(yaw);
    pitchCos = func_151423D8(pitch - 0x40);
    horizSpeed = func_151423D8(pitch);
    // matching: scaling in a separate statement; folding it into the call above reorders the float ops
    horizSpeed = 10.0f * horizSpeed;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;
    desc.dir.x = horizSpeed * yawCos;
    desc.dir.y = -10.0f * pitchCos;
    desc.dir.z = horizSpeed * yawSin;

    desc.headSpeed = (func_150ADA68() * 1.953000069f) + 3.736000061f;
    desc.tailSpeed = (func_150ADA68() * 0.9400000572f) + 2.22300005f;
    func_151429E0(8, &desc.headColor.red, &desc.headColor.green, &desc.headColor.blue);
    func_151429E0(8, &desc.tailColor.red, &desc.tailColor.green, &desc.tailColor.blue);
    desc.headColor.alpha = 0xFF;
    desc.tailColor.alpha = 0xFF;
    desc.flags = 9;
    desc.lifetime = (func_150ADA20() & 0xF) + 15;

    desc.width = (func_150ADA68() * 2.659999847f) + 1.849999905f;
    desc.renderMode = 0x1601;
    desc.alpha = 0xFF;
    desc.fadeStart = 10;
    desc.fadeRate = 25;
    func_15156190(&desc, 1, 0, owner, 1);
    func_15156388(&desc, 1, 0);
    return 1;
}
