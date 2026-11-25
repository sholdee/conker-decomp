#include <ultra64.h>
#include "functions.h"
#include "variables.h"


// Builds the camera rail from the current rail command: a segment of length 2 * halfLength
// centred on the command's point.
void func_1512DD50(Camera *camera) {
    CamRail *rail;
    f32 z;
    CamCmd *cmd;
    f32 x;
    f32 y;
    f32 scale;
    f32 dy;
    Vec3f span;
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
    rail->offset = cmd->unk20;
    rail->heightOffset = cmd->unk8;
    rail->offsetMode = cmd->unk1C;
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
    if (camera->Snap) {
        camera->eye.x = rail->eyeGoal.x;
        camera->eye.y = rail->eyeGoal.y;
        camera->eye.z = rail->eyeGoal.z;
        rail->eyeEaseA = 40.0f;
        rail->eyeEaseB = 40.0f;
        return;
    }
    func_150495B0(&rail->eyeEaseA, 10.0f, &rail->eyeEaseAVel, 0.6f, 1.4f, camera->SpringTime);
    func_150495B0(&rail->eyeEaseB, 10.0f, &rail->eyeEaseBVel, 0.6f, 1.4f, camera->SpringTime);
    func_150495B0(&camera->eye.x, rail->eyeGoal.x, &camera->CurrentVelX, rail->eyeEaseA, rail->eyeEaseB, camera->SpringTime);
    func_150495B0(&camera->eye.y, rail->eyeGoal.y, &camera->CurrentVelY, rail->eyeEaseA, rail->eyeEaseB, camera->SpringTime);
    func_150495B0(&camera->eye.z, rail->eyeGoal.z, &camera->CurrentVelZ, rail->eyeEaseA, rail->eyeEaseB, camera->SpringTime);
}
