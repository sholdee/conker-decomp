#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// Current camera command (embedded in the camera at 0x6FC); the fields below are the ones a
// rail command (type 15) uses. See CamCmd in game_15BD90.c.
typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ u32 heightOffset; // copied to rail.heightOffset
    /* 0x0C */ u8 padC[0x4];
    /* 0x10 */ s16 x; // rail midpoint
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 halfLength;
    /* 0x18 */ u8 pad18[0x4];
    /* 0x1C */ u32 offsetMode; // copied to rail.offsetMode
    /* 0x20 */ u32 offset; // copied to rail.offset
    /* 0x24 */ f32 unk24; // unk24/unk28 split halfLength into horizontal/vertical parts,
    /* 0x28 */ f32 unk28; // unk2C/unk30 split the horizontal part into x/z
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
} CamRailCmd;

// Camera rail: a line segment the camera eye slides along (embedded in the camera at 0x870).
typedef struct {
    /* 0x00 */ u8 offsetMode; // which side of the rail the eye sits on (0..3)
    /* 0x04 */ vertex start;
    /* 0x10 */ vertex dir; // unit direction from start to end
    /* 0x1C */ f32 invLength;
    /* 0x20 */ f32 offset; // horizontal distance of the eye from the rail
    /* 0x24 */ f32 heightOffset;
    /* 0x28 */ f32 eyeEaseA; // easing parameters for the eye, eased toward 10.0
    /* 0x2C */ f32 eyeEaseB;
    /* 0x30 */ f32 eyeEaseAVel;
    /* 0x34 */ f32 eyeEaseBVel;
    /* 0x38 */ vertex eyeGoal;
} CamRail;

// This file's view of a camera (struct108, 0x9A0 bytes).
typedef struct {
    /* 0x000 */ u8 pad0[0x23C];
    /* 0x23C */ u8 snap; // nonzero: jump to targets instead of easing
    /* 0x23D */ u8 pad23D[0x7F];
    /* 0x2BC */ vertex target; // look-at point
    /* 0x2C8 */ u8 pad2C8[0x30];
    /* 0x2F8 */ vertex eye; // camera position
    /* 0x304 */ u8 pad304[0xBC];
    /* 0x3C0 */ vertex eyeVel; // easing velocity of eye
    /* 0x3CC */ u8 pad3CC[0x330];
    /* 0x6FC */ CamRailCmd cmd;
    /* 0x730 */ u8 pad730[0x84];
    /* 0x7B4 */ f32 dt; // frame time scale
    /* 0x7B8 */ u8 pad7B8[0xB8];
    /* 0x870 */ CamRail rail;
} Camera;

void func_150495B0(f32 *value, f32 goal, f32 *velocity, f32 arg3, f32 arg4, f32 dt);

// Builds the camera rail from the current rail command: a segment of length 2 * halfLength
// centred on the command's point.
void func_1512DD50(Camera *camera) {
    CamRail *rail;
    f32 z;
    CamRailCmd *cmd;
    f32 x;
    f32 y;
    f32 scale;
    f32 dy;
    vertex span;
    f32 dx;
    f32 dz;

    rail = &camera->rail;
    cmd = &camera->cmd;
    x = cmd->x;
    y = cmd->y;
    z = cmd->z;
    scale = cmd->halfLength;
    // matching: scale/dx reuse (horizontal part, then x part) gives IDO's register split
    dx = cmd->unk24 * scale;
    dy = cmd->unk28 * scale;
    scale = dx;
    dx = cmd->unk2C * dx;
    dz = -scale * cmd->unk30;
    rail->start.x = x - dx;
    rail->start.y = y - dy;
    rail->start.z = z - dz;
    span.x = 2.0f * dx;
    span.y = 2.0f * dy;
    span.z = 2.0f * dz;

    scale = 1.0f / func_150AD930(&span.x);
    rail->invLength = scale;
    rail->dir.x = scale * span.x;
    rail->dir.y = scale * span.y;
    rail->dir.z = scale * span.z;
    rail->offset = cmd->offset;
    rail->heightOffset = cmd->heightOffset;
    rail->offsetMode = cmd->offsetMode;
    rail->eyeEaseA = 0.0f;
    rail->eyeEaseB = 0.0f;
}

// Rail camera update: places the eye on the rail as far from its start as the look-at target
// is, offsets it to one side, and eases the eye there.
void func_1512DEA4(Camera *camera) {
    CamRail *rail;
    f32 dist;
    f32 offset[3];
    f32 x, y, z;
    f32 dx;
    f32 dy;
    f32 sideOffset;
    f32 pad0, pad1, pad2, pad3; // matching: unused, sizes the stack frame
    f32 dz;

    x = camera->rail.start.x;
    dx = x - camera->target.x;
    y = camera->rail.start.y;
    dy = y - camera->target.y;
    z = camera->rail.start.z;
    dz = z - camera->target.z;
    rail = &camera->rail;
    dist = sqrtf(dx * dx + dy * dy + dz * dz);
    // matching: the (f32) casts on locals keep IDO's local-first operand order in add.s/mul.s
    rail->eyeGoal.x = (f32)x + rail->dir.x * dist;
    rail->eyeGoal.y = (f32)y + rail->dir.y * dist;
    rail->eyeGoal.z = (f32)z + rail->dir.z * dist;
    offset[1] = camera->rail.heightOffset;
    switch (camera->rail.offsetMode) {
        case 0:
            sideOffset = rail->offset;
            offset[0] = (f32)sideOffset * rail->dir.z;
            offset[2] = (f32)sideOffset * -rail->dir.x;
            break;
        case 1:
            sideOffset = rail->offset;
            offset[0] = (f32)sideOffset * -rail->dir.z;
            offset[2] = (f32)sideOffset * rail->dir.x;
            break;
        case 2:
            sideOffset = rail->offset;
            offset[0] = (f32)sideOffset * rail->dir.x;
            offset[2] = (f32)sideOffset * rail->dir.z;
            break;
        case 3:
            sideOffset = rail->offset;
            offset[0] = (f32)sideOffset * -rail->dir.x;
            offset[2] = (f32)sideOffset * -rail->dir.z;
            break;
    }
    rail->eyeGoal.x += offset[0];
    rail->eyeGoal.y += offset[1];
    rail->eyeGoal.z += offset[2];
    if (camera->snap) {
        camera->eye.x = rail->eyeGoal.x;
        camera->eye.y = rail->eyeGoal.y;
        camera->eye.z = rail->eyeGoal.z;
        rail->eyeEaseA = 40.0f;
        rail->eyeEaseB = 40.0f;
        return;
    }
    func_150495B0(&rail->eyeEaseA, 10.0f, &rail->eyeEaseAVel, 0.6f, 1.4f, camera->dt);
    func_150495B0(&rail->eyeEaseB, 10.0f, &rail->eyeEaseBVel, 0.6f, 1.4f, camera->dt);
    func_150495B0(&camera->eye.x, rail->eyeGoal.x, &camera->eyeVel.x, rail->eyeEaseA, rail->eyeEaseB, camera->dt);
    func_150495B0(&camera->eye.y, rail->eyeGoal.y, &camera->eyeVel.y, rail->eyeEaseA, rail->eyeEaseB, camera->dt);
    func_150495B0(&camera->eye.z, rail->eyeGoal.z, &camera->eyeVel.z, rail->eyeEaseA, rail->eyeEaseB, camera->dt);
}
