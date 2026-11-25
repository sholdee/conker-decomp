#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct Game1BBFC0Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Game1BBFC0Vec3f;

typedef struct Game1BBFC0Motion {
    /* 0x00 */ u8 unknown0[0x30];
    /* 0x30 */ Game1BBFC0Vec3f velocity;
    /* 0x3C */ f32 gravity;
    /* 0x40 */ f32 drag;
    /* 0x44 */ f32 shrinkRate;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ f32 blend;
} Game1BBFC0Motion;

typedef struct Game1BBFC0Beam {
    /* 0x000 */ u8 unknown0[0x1C];
    /* 0x01C */ s16 unk1C;
    /* 0x01E */ u8 unknown1E[0xE];
    /* 0x02C */ f32 width;
    /* 0x030 */ f32 progress;
    /* 0x034 */ Game1BBFC0Vec3f start;
    /* 0x040 */ Game1BBFC0Vec3f end;
    /* 0x04C */ u8 unknown4C[0x10];
    /* 0x05C */ u8 unk5C;
    /* 0x05D */ u8 unknown5D[0x63];
    /* 0x0C0 */ Vtx vertexTemplate[4]; /* copied into a camera buffer when it is fresh */
    /* 0x100 */ Vtx *buffers[1]; /* one per camera, indexed by camera id */
    /* 0x104 */ u8 unknown104[0xC];
    /* 0x110 */ Game1BBFC0Motion motion;
} Game1BBFC0Beam;

void *memcpy(void *, const void *, u32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

/*
 * Fills this camera's four beam vertices: a quad from start toward end (scaled by progress),
 * widened by width perpendicular to the view direction from the camera eye.
 */
Vtx *func_1518EB10(Game1BBFC0Beam *beam, s16 cam) {
    Vtx *vtx;
    Vtx *result;
    f32 progress;
    Game1BBFC0Vec3f tip;
    f32 dx, dy, dz;
    f32 vx, vy, vz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    u8 fresh;
    Game1BBFC0Vec3f *eye;
    f32 scale;

    func_151D5D60(beam->buffers, cam, 4 * sizeof(Vtx), (void **)&vtx, &fresh);
    result = vtx;
    // matching: IDO lays out the branches as in the target only with the NULL return in the else
    if (vtx != NULL) {
        if (fresh) {
            memcpy(beam->buffers[cam], beam->vertexTemplate, sizeof(beam->vertexTemplate));
            memcpy(beam->buffers[cam] + 4, beam->vertexTemplate, sizeof(beam->vertexTemplate));
        }
    } else {
        return NULL;
    }
    progress = beam->progress;
    dx = beam->end.x - beam->start.x;
    dy = beam->end.y - beam->start.y;
    dz = beam->end.z - beam->start.z;
    tip.x = beam->start.x + dx * progress;
    tip.y = beam->start.y + dy * progress;
    tip.z = beam->start.z + dz * progress;
    eye = (Game1BBFC0Vec3f *)&D_800DBFF0[cam].unk2F8;
    dx = tip.x - beam->start.x;
    dy = tip.y - beam->start.y;
    dz = tip.z - beam->start.z;
    vx = beam->start.x - eye->x;
    vy = beam->start.y - eye->y;
    vz = beam->start.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = beam->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vtx->v.ob[0] = beam->start.x + nx;
    vtx->v.ob[1] = beam->start.y + ny;
    vtx->v.ob[2] = beam->start.z + nz;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = beam->start.x - nx;
    vtx->v.ob[1] = beam->start.y - ny;
    vtx->v.ob[2] = beam->start.z - nz;
    vtx->v.flag = 0;
    vtx++;
    dx = -dx;
    dy = -dy;
    dz = -dz;
    vx = tip.x - eye->x;
    vy = tip.y - eye->y;
    vz = tip.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = beam->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vtx->v.ob[0] = tip.x + nx;
    vtx->v.ob[1] = tip.y + ny;
    vtx->v.ob[2] = tip.z + nz;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = tip.x - nx;
    vtx->v.ob[1] = tip.y - ny;
    vtx->v.ob[2] = tip.z - nz;
    vtx->v.flag = 0;
    return result;
}

/* Moves the beam by its velocity (with drag and gravity), then pulls its end toward its start. */
s32 func_1518EF54(Game1BBFC0Beam *beam) {
    Game1BBFC0Motion *motion = &beam->motion;
    f32 dx;
    f32 dy;
    f32 dz;

    motion->velocity.x *= motion->drag;
    motion->velocity.y += motion->gravity * D_800BE9A4;
    motion->velocity.z *= motion->drag;
    dx = motion->velocity.x * D_800BE9A4;
    dy = motion->velocity.y * D_800BE9A4;
    dz = motion->velocity.z * D_800BE9A4;
    beam->start.x += dx;
    beam->start.y += dy;
    beam->start.z += dz;
    beam->end.x += dx;
    beam->end.y += dy;
    beam->end.z += dz;
    beam->end.x = beam->start.x + (beam->end.x - beam->start.x) * motion->blend;
    beam->end.y = beam->start.y + (beam->end.y - beam->start.y) * motion->blend;
    beam->end.z = beam->start.z + (beam->end.z - beam->start.z) * motion->blend;
    return 1;
}

/* Moves the beam by its velocity, with drag and gravity. */
s32 func_1518F058(Game1BBFC0Beam *beam) {
    Game1BBFC0Motion *motion = &beam->motion;
    f32 dx;
    f32 dy;
    f32 dz;

    motion->velocity.x *= motion->drag;
    motion->velocity.y += motion->gravity * D_800BE9A4;
    motion->velocity.z *= motion->drag;
    dx = motion->velocity.x * D_800BE9A4;
    dy = motion->velocity.y * D_800BE9A4;
    dz = motion->velocity.z * D_800BE9A4;
    beam->start.x += dx;
    beam->start.y += dy;
    beam->start.z += dz;
    beam->end.x += dx;
    beam->end.y += dy;
    beam->end.z += dz;
    return 1;
}

/* Shrinks the beam's width and progress; sets unk5C from unk1C while unk1C < unk48. */
s32 func_1518F108(Game1BBFC0Beam *beam) {
    beam->progress -= beam->progress * beam->motion.shrinkRate;
    beam->width -= beam->width * beam->motion.shrinkRate;
    if (beam->unk1C < beam->motion.unk48) {
        beam->unk5C = beam->unk1C * beam->motion.unk4A;
    }
    return 1;
}

/* Like func_1518F108, but shrinks only the beam's progress. */
s32 func_1518F15C(Game1BBFC0Beam *beam) {
    beam->progress -= beam->progress * beam->motion.shrinkRate;
    if (beam->unk1C < beam->motion.unk48) {
        beam->unk5C = beam->unk1C * beam->motion.unk4A;
    }
    return 1;
}
