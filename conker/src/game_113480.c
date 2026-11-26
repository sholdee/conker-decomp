#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Spawns a streak particle from start along dir; when checkHit is set, records where its path hits a surface. */
void *particle_spawnImpactStreak(Vec3f *start, Vec3f *dir, f32 arg2, f32 arg3, f32 arg4, u8 arg5, s32 arg6, u8 checkHit, u8 arg8,
                    s16 life, s8 callback) {
    void *streak;
    ParticleDef info;
    StreakImpact impact;
    s32 hitObject;
    Vec3f offset;
    s32 randomBit0;
    s32 randomBit1;

    info.texture = 0x41;
    info.renderModes = 0x2301;
    info.frame = 0;
    info.frameSpeed = 0;
    info.rgba[0] = 0;
    info.rgba[1] = 0;
    info.rgba[2] = 0;
    info.lifetime = life;
    info.rgba[3] = 0xFF;
    info.unk44 = arg8;
    info.unk45 = 0xFF;
    info.width = arg3;
    info.height = arg4;
    info.pos = *start;
    info.rot = *dir;
    info.scale.x = 0.0f;
    info.scale.y = arg2;
    info.scale.z = 0.0f;
    info.flags = 1;
    impact.flags = 0;
    impact.callback = callback;
    if (checkHit && func_150AC9C0(start->x, start->y, start->z, dir->x, dir->y, dir->z, &impact.unk10, &impact.tri,
                                  &impact.point.x, &impact.point.y, &impact.point.z, NULL, &hitObject, NULL, 0.0f) &&
        func_15145C90(hitObject)) {
        impact.flags |= 1;
        offset.x = impact.point.x - info.pos.x;
        offset.y = impact.point.y - info.pos.y;
        offset.z = impact.point.z - info.pos.z;
        if (func_15144A74(&offset, &impact.point) < 0.0f) {
            impact.side = -1.0f;
        } else {
            impact.side = 1.0f;
        }
    }
    if (func_150ADA20() & 1) {
        randomBit1 = 2;
    } else {
        randomBit1 = 0;
    }
    if (func_150ADA20() & 1) {
        randomBit0 = 1;
    } else {
        randomBit0 = 0;
    }
    streak = particle_newQuadParticleWhite(&info, 15, 21, 0, 15, randomBit0 | randomBit1, 0x30, arg5, arg6);
    if (streak != NULL) {
        memcpy(&((Streak *)streak)->impact, &impact, sizeof(impact));
    }
    return streak;
}

/* Impact callbacks, indexed by StreakImpact.callback. */
static s32 (*gStreakImpactFuncs[])(Streak *) = { particle_spawnStreakImpact };

/* Moves the streak along its direction; runs its impact callback once it crosses the surface it was aimed at. */
s8 particle_moveImpactStreak(Streak *streak) {
    s8 ret = 1;

    {
        StreakImpact *impact = &streak->impact;
        f32 side;
        Vec3f offset;

        if (impact->flags & 1) {
            offset.x = impact->point.x - streak->head.def.pos.x;
            offset.y = impact->point.y - streak->head.def.pos.y;
            offset.z = impact->point.z - streak->head.def.pos.z;
            if (func_15144A74(&offset, &impact->point) < 0.0f) {
                side = -1.0f;
            } else {
                side = 1.0f;
            }
            if (side != impact->side) {
                ret = (impact->callback != -1) ? gStreakImpactFuncs[impact->callback](streak) : 0;
            }
        }
    }
    {
        f32 vx = streak->head.def.rot.x * streak->head.def.scale.y;
        f32 vy = streak->head.def.rot.y * streak->head.def.scale.y;
        f32 vz = streak->head.def.rot.z * streak->head.def.scale.y;

        streak->head.def.pos.x += vx * gFrameDelta;
        streak->head.def.pos.y += vy * gFrameDelta;
        streak->head.def.pos.z += vz * gFrameDelta;
    }
    return ret;
}

typedef struct {
    s16 x, y, z, flag;
    u8 other[7];
    u8 alpha;
} StreakVtx;


/* Builds the streak's camera-facing quad (4 vertices) for view index viewIdx; returns NULL without a buffer. */
void *particle_buildImpactStreakQuad(Streak *streak, s16 viewIdx) {
    Vec3f start;
    Vec3f end;
    Vec3f *eye;
    f32 dx, dy, dz;
    f32 vx, vy, vz;
    f32 nx, ny, nz;
    f32 scale;
    f32 length;
    f32 lengthSquared;
    f32 pad1; // matching: unused stack slots keep the original frame layout
    f32 pad2;
    StreakVtx *vertices;
    void *result;
    u8 fresh;
    s32 pad3; // matching: unused stack slot (frame layout)

    particle_getViewBuffer(streak->head.vtxBuffers, viewIdx, 0x40, &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(streak->head.vtxBuffers[viewIdx], streak->head.vtxTemplate, sizeof(streak->head.vtxTemplate));
            memcpy(streak->head.vtxBuffers[viewIdx] + 4, streak->head.vtxTemplate, sizeof(streak->head.vtxTemplate));
        }
    } else {
        return NULL;
    }
    eye = &camera_global_Cameras[viewIdx].eye;
    length = streak->head.def.height;
    start = streak->head.def.pos;
    do { // matching: a do/while(0) (a VEC3 macro body) splits uopt's block so length's f0 is reused for dx; { } does not
        end.x = streak->head.def.pos.x - streak->head.def.rot.x * length;
        end.y = streak->head.def.pos.y - streak->head.def.rot.y * length;
        end.z = streak->head.def.pos.z - streak->head.def.rot.z * length;
    } while (0);
    dx = end.x - start.x;
    dy = end.y - start.y;
    dz = end.z - start.z;
    vx = (dx * 0.5f + start.x) - eye->x;
    vy = (dy * 0.5f + start.y) - eye->y;
    vz = (dz * 0.5f + start.z) - eye->z;
    nx = dy * vz - vy * dz;
    ny = dz * vx - vz * dx;
    nz = dx * vy - vx * dy;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = streak->head.def.width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = start.x - nx;
    vertices->y = start.y - ny;
    vertices->z = start.z - nz;
    vertices->alpha = streak->head.def.unk44;
    vertices->flag = 0;
    vertices++;
    vertices->x = end.x - nx;
    vertices->y = end.y - ny;
    vertices->z = end.z - nz;
    vertices->alpha = 0;
    vertices->flag = 0;
    vertices++;
    vertices->x = end.x + nx;
    vertices->y = end.y + ny;
    vertices->z = end.z + nz;
    vertices->alpha = 0;
    vertices->flag = 0;
    vertices++;
    vertices->x = start.x + nx;
    vertices->y = start.y + ny;
    vertices->z = start.z + nz;
    vertices->alpha = streak->head.def.unk44;
    vertices->flag = 0;
    return result;
}

/* Fades the streak out over its last 8 ticks. */
s32 particle_fadeImpactStreak(Streak *streak) {
    s32 timer = streak->head.def.lifetime;

    if (timer < 8) {
        s32 fade = timer * 32;

        if (fade < streak->head.def.unk44) {
            streak->head.def.unk44 = fade;
        }
    }
    return 1;
}

/* Spawn description passed to particle_spawnSurfaceQuadSpriteNoModel. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ s8 unk16;
    /* 0x17 */ s8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C[5];
} ImpactSpawnInfo; /* size 0x30 */

/* Impact callback: spawns an effect at the streak's recorded hit point. */
s32 particle_spawnStreakImpact(Streak *streak) {
    ImpactSpawnInfo info;
    StreakImpact *impact;

    info.unk6 = 0x3A;
    info.unk0 = 1;
    info.unk4 = 100;
    info.unk8 = 0;
    info.unkC = 0;
    info.unk10 = 0xFF;
    info.unk11 = 0xFF;
    info.unk12 = 0;
    info.unk13 = 0;
    info.unk14 = 0;
    info.unk15 = 0xFF;
    info.unk18 = 0x130001;

    impact = &streak->impact;
    particle_spawnSurfaceQuadSpriteNoModel(&info, 0, 0, &impact->tri, impact->point.x, impact->point.y, impact->point.z, 50.0f,
                  50.0f, func_150ADA20() & 0xFF, 0, 0, streak->head.header.owner, streak->head.header.kind);
    return 0;
}
