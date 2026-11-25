#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct Game1BBFC0Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Game1BBFC0Vec3f;

typedef struct Game1BBFC0Vertex {
    s16 x, y, z, flag;
    u8 other[8];
} Game1BBFC0Vertex;

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
    /* 0x0C0 */ u8 vertexTemplate[0x40];
    /* 0x100 */ u8 *buffers[1];
    /* 0x104 */ u8 unknown104[0xC];
    /* 0x110 */ Game1BBFC0Motion motion;
} Game1BBFC0Beam;

void *memcpy(void *, const void *, u32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void *func_1518EB10(Game1BBFC0Beam *arg0, s16 arg1) {
    Game1BBFC0Vertex *vertices;
    void *result;
    f32 progress;
    Game1BBFC0Vec3f tip;
    f32 dx, dy, dz;
    f32 vx, vy, vz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    u8 fresh;
    Game1BBFC0Vec3f *eye;
    f32 scale;

    func_151D5D60(arg0->buffers, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(arg0->buffers[arg1], arg0->vertexTemplate, 0x40);
            memcpy(arg0->buffers[arg1] + 0x40, arg0->vertexTemplate, 0x40);
        }
    } else {
        return NULL;
    }
    progress = arg0->progress;
    dx = arg0->end.x - arg0->start.x;
    dy = arg0->end.y - arg0->start.y;
    dz = arg0->end.z - arg0->start.z;
    tip.x = arg0->start.x + dx * progress;
    tip.y = arg0->start.y + dy * progress;
    tip.z = arg0->start.z + dz * progress;
    eye = (Game1BBFC0Vec3f *)&D_800DBFF0[arg1].unk2F8;
    dx = tip.x - arg0->start.x;
    dy = tip.y - arg0->start.y;
    dz = tip.z - arg0->start.z;
    vx = arg0->start.x - eye->x;
    vy = arg0->start.y - eye->y;
    vz = arg0->start.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = arg0->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = arg0->start.x + nx;
    vertices->y = arg0->start.y + ny;
    vertices->z = arg0->start.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = arg0->start.x - nx;
    vertices->y = arg0->start.y - ny;
    vertices->z = arg0->start.z - nz;
    vertices->flag = 0;
    vertices++;
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
        scale = arg0->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = tip.x + nx;
    vertices->y = tip.y + ny;
    vertices->z = tip.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = tip.x - nx;
    vertices->y = tip.y - ny;
    vertices->z = tip.z - nz;
    vertices->flag = 0;
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
