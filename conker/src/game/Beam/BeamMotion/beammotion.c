#include <ultra64.h>
#include "functions.h"
#include "variables.h"

void *memcpy(void *, const void *, u32);

/*
 * Fills this camera's four beam vertices: a quad from start toward end (scaled by progress),
 * widened by width perpendicular to the view direction from the camera eye.
 */
Vtx *beam_fillMotionVerts(Game1BBFC0Beam *beam, s16 cam) {
    Vtx *vtx;
    Vtx *result;
    f32 progress;
    Vec3f tip;
    f32 dx, dy, dz;
    f32 vx, vy, vz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    u8 fresh;
    Vec3f *eye;
    f32 scale;

    particle_getViewBuffer(beam->buffers, cam, 4 * sizeof(Vtx), &vtx, &fresh);
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
    eye = &camera_global_Cameras[cam].eye;
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
s32 beam_updateMotionPull(Game1BBFC0Beam *beam) {
    Game1BBFC0Motion *motion = &beam->motion;
    f32 dx;
    f32 dy;
    f32 dz;

    motion->velocity.x *= motion->drag;
    motion->velocity.y += motion->gravity * gFrameDelta;
    motion->velocity.z *= motion->drag;
    dx = motion->velocity.x * gFrameDelta;
    dy = motion->velocity.y * gFrameDelta;
    dz = motion->velocity.z * gFrameDelta;
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
s32 beam_updateMotion(Game1BBFC0Beam *beam) {
    Game1BBFC0Motion *motion = &beam->motion;
    f32 dx;
    f32 dy;
    f32 dz;

    motion->velocity.x *= motion->drag;
    motion->velocity.y += motion->gravity * gFrameDelta;
    motion->velocity.z *= motion->drag;
    dx = motion->velocity.x * gFrameDelta;
    dy = motion->velocity.y * gFrameDelta;
    dz = motion->velocity.z * gFrameDelta;
    beam->start.x += dx;
    beam->start.y += dy;
    beam->start.z += dz;
    beam->end.x += dx;
    beam->end.y += dy;
    beam->end.z += dz;
    return 1;
}

/* Shrinks the beam's width and progress; sets unk5C from unk1C while unk1C < unk48. */
s32 beam_shrinkMotion(Game1BBFC0Beam *beam) {
    beam->progress -= beam->progress * beam->motion.shrinkRate;
    beam->width -= beam->width * beam->motion.shrinkRate;
    if (beam->unk1C < beam->motion.unk48) {
        beam->unk5C = beam->unk1C * beam->motion.unk4A;
    }
    return 1;
}

/* Like beam_shrinkMotion, but shrinks only the beam's progress. */
s32 beam_shrinkMotionProgress(Game1BBFC0Beam *beam) {
    beam->progress -= beam->progress * beam->motion.shrinkRate;
    if (beam->unk1C < beam->motion.unk48) {
        beam->unk5C = beam->unk1C * beam->motion.unk4A;
    }
    return 1;
}
