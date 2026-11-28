#include <ultra64.h>
#include <math.h>
#include "conker/beam.h"
#include "conker/camera.h"
#include "conker/game.h"
#include "conker/particle.h"
#include "libc/string.h"

/*
 * Fills this camera's four beam vertices: a quad from start toward end (scaled by progress),
 * widened by width perpendicular to the view direction from the camera eye.
 */
Vtx *beam_fillMotionVerts(MotionBeam *beam, s16 cam) {
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

    particle_getViewBuffer(beam->head.vtxBuffers, cam, 4 * sizeof(Vtx), &vtx, &fresh);
    result = vtx;
    // matching: IDO lays out the branches as in the target only with the NULL return in the else
    if (vtx != NULL) {
        if (fresh) {
            memcpy(beam->head.vtxBuffers[cam], beam->head.vtxTemplate, sizeof(beam->head.vtxTemplate));
            memcpy(beam->head.vtxBuffers[cam] + 4, beam->head.vtxTemplate, sizeof(beam->head.vtxTemplate));
        }
    } else {
        return NULL;
    }
    progress = beam->head.def.height;
    dx = beam->head.def.rot.x - beam->head.def.pos.x;
    dy = beam->head.def.rot.y - beam->head.def.pos.y;
    dz = beam->head.def.rot.z - beam->head.def.pos.z;
    tip.x = beam->head.def.pos.x + dx * progress;
    tip.y = beam->head.def.pos.y + dy * progress;
    tip.z = beam->head.def.pos.z + dz * progress;
    eye = &camera_global_Cameras[cam].eye;
    dx = tip.x - beam->head.def.pos.x;
    dy = tip.y - beam->head.def.pos.y;
    dz = tip.z - beam->head.def.pos.z;
    vx = beam->head.def.pos.x - eye->x;
    vy = beam->head.def.pos.y - eye->y;
    vz = beam->head.def.pos.z - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = beam->head.def.width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vtx->v.ob[0] = beam->head.def.pos.x + nx;
    vtx->v.ob[1] = beam->head.def.pos.y + ny;
    vtx->v.ob[2] = beam->head.def.pos.z + nz;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = beam->head.def.pos.x - nx;
    vtx->v.ob[1] = beam->head.def.pos.y - ny;
    vtx->v.ob[2] = beam->head.def.pos.z - nz;
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
        scale = beam->head.def.width / sqrtf(lengthSquared);
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
s32 beam_updateMotionPull(MotionBeam *beam) {
    MotionBeamState *motion = &beam->motion;
    f32 dx;
    f32 dy;
    f32 dz;

    motion->velocity.x *= motion->drag;
    motion->velocity.y += motion->gravity * gFrameDelta;
    motion->velocity.z *= motion->drag;
    dx = motion->velocity.x * gFrameDelta;
    dy = motion->velocity.y * gFrameDelta;
    dz = motion->velocity.z * gFrameDelta;
    beam->head.def.pos.x += dx;
    beam->head.def.pos.y += dy;
    beam->head.def.pos.z += dz;
    beam->head.def.rot.x += dx;
    beam->head.def.rot.y += dy;
    beam->head.def.rot.z += dz;
    beam->head.def.rot.x = beam->head.def.pos.x + (beam->head.def.rot.x - beam->head.def.pos.x) * motion->blend;
    beam->head.def.rot.y = beam->head.def.pos.y + (beam->head.def.rot.y - beam->head.def.pos.y) * motion->blend;
    beam->head.def.rot.z = beam->head.def.pos.z + (beam->head.def.rot.z - beam->head.def.pos.z) * motion->blend;
    return 1;
}

/* Moves the beam by its velocity, with drag and gravity. */
s32 beam_updateMotion(MotionBeam *beam) {
    MotionBeamState *motion = &beam->motion;
    f32 dx;
    f32 dy;
    f32 dz;

    motion->velocity.x *= motion->drag;
    motion->velocity.y += motion->gravity * gFrameDelta;
    motion->velocity.z *= motion->drag;
    dx = motion->velocity.x * gFrameDelta;
    dy = motion->velocity.y * gFrameDelta;
    dz = motion->velocity.z * gFrameDelta;
    beam->head.def.pos.x += dx;
    beam->head.def.pos.y += dy;
    beam->head.def.pos.z += dz;
    beam->head.def.rot.x += dx;
    beam->head.def.rot.y += dy;
    beam->head.def.rot.z += dz;
    return 1;
}

/* Shrinks the beam's width and progress; sets alpha from lifetime while lifetime < motion.fadeStart. */
s32 beam_shrinkMotion(MotionBeam *beam) {
    beam->head.def.height -= beam->head.def.height * beam->motion.shrinkRate;
    beam->head.def.width -= beam->head.def.width * beam->motion.shrinkRate;
    if (beam->head.def.lifetime < beam->motion.fadeStart) {
        beam->head.def.alpha = beam->head.def.lifetime * beam->motion.fadeRate;
    }
    return 1;
}

/* Like beam_shrinkMotion, but shrinks only the beam's progress. */
s32 beam_shrinkMotionProgress(MotionBeam *beam) {
    beam->head.def.height -= beam->head.def.height * beam->motion.shrinkRate;
    if (beam->head.def.lifetime < beam->motion.fadeStart) {
        beam->head.def.alpha = beam->head.def.lifetime * beam->motion.fadeRate;
    }
    return 1;
}
