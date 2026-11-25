#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_15143134(vertex *in, vertex *out, Mtx *mtx);
extern void func_1514C858(f32, f32, f32, f32, s32, s32, s32, s32, s32, s32, f32, s32, s32);
extern void func_151D3FF4(vertex *, u8, s32);
extern vertex D_8009FBC0; // a point in bone space

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} Color8;

/* Spawn parameters for a streak particle (copy of game_183640.c's StreakParticleDesc). */
typedef struct {
    /* 0x00 */ vertex pos;
    /* 0x0C */ vertex dir;
    /* 0x18 */ f32 headSpeed;
    /* 0x1C */ f32 tailSpeed;
    /* 0x20 */ Color8 headColor;
    /* 0x24 */ Color8 tailColor;
    /* 0x28 */ u8 flags;
    /* 0x2A */ s16 lifetime;
    /* 0x2C */ u16 renderMode;
    /* 0x30 */ f32 width;
    /* 0x34 */ u8 alpha;
    /* 0x36 */ s16 fadeStart;
    /* 0x38 */ s16 fadeRate;
} StreakParticleDesc;

extern void func_151429E0(u8, u8 *, u8 *, u8 *);
extern void func_15156190(StreakParticleDesc *desc, u8 kind, s32 extraSize, u8 owner, s32 arg4);
extern void func_15156388(StreakParticleDesc *desc, u8 kind, s32 extraSize);

/* Argument of func_151D8868 (same layout as game_12B250.c's Event_150FE288). */
typedef struct {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 duration;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 cameraMask;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 pad7;
} CameraEvent;

extern void func_151D8868(CameraEvent *event, s32 arg1, s32 arg2, s32 arg3);

/* Spawns an effect at bone 25 of obj, plus a camera event (type 1, 12-27 long) and a func_151D3FF4 at the same point. */
void func_150B3AB0(struct127 *obj, u8 arg1) {
    s16 yaw;
    vertex pos;
    CameraEvent event;

    if ((obj != NULL) && (obj->unk1D4 != NULL)) {
        func_1512D748(&D_800DBFF0[D_800BE9E8], 6, 1);
        yaw = (obj->unk7A >> 8) + 0x40;
        func_15143134(&D_8009FBC0, &pos, (Mtx *)obj->unk1D4 + 25);
        // matching: *(u8 *)&arg1 re-reads arg1 from its stack slot at each use (plain or K&R arg1 stays in a register)
        func_1514C858(pos.x, pos.y, pos.z, 10.0f, yaw, 0, 0xFF,
                      (func_150ADA20() % 31U) + 40, 8, 0, 0.0f, 0, *(u8 *)&arg1);

        event.type = 1;
        event.duration = (func_150ADA20() & 0xF) + 12;
        event.unk4 = (func_150ADA20() & 3) + 5;
        event.unk6 = -1;
        event.cameraMask = 1;
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
