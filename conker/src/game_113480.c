#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Hit record copied to Streak.impact (+0x110) when a streak is spawned. */
typedef struct {
    /* 0x00 */ u8 flags; /* bit 0: the streak's path hits a surface */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ Vec3f point;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14[5];
    /* 0x28 */ f32 side; /* side of the surface the streak starts on (+1 / -1) */
    /* 0x2C */ s8 callback; /* index into D_80088A10, -1 for none */
    /* 0x2D */ u8 pad2D[3];
} StreakImpact; /* size 0x30 */

/* The particle object spawned by func_150E5FD0 (kind 0x41). */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 unk1;
    /* 0x002 */ u8 pad2[0xA];
    /* 0x00C */ u8 unkC;
    /* 0x00D */ u8 padD[0xF];
    /* 0x01C */ s16 timer;
    /* 0x01E */ u8 pad1E[0xE];
    /* 0x02C */ f32 width;
    /* 0x030 */ f32 length;
    /* 0x034 */ Vec3f position;
    /* 0x040 */ Vec3f direction;
    /* 0x04C */ u8 pad4C[4];
    /* 0x050 */ f32 speed;
    /* 0x054 */ u8 pad54[8];
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0x63];
    /* 0x0C0 */ u8 vertexTemplate[0x40];
    /* 0x100 */ u8 *buffers[4];
    /* 0x110 */ StreakImpact impact;
} Streak;

extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, f32 *, f32 *, f32 *, void *, s32 *, void *, f32);
extern f32 func_15144A74(Vec3f *, Vec3f *);
extern s32 func_15145C90(s32);

/* Spawns a streak particle from start along dir; when checkHit is set, records where its path hits a surface. */
void *func_150E5FD0(Vec3f *start, Vec3f *dir, f32 arg2, f32 arg3, f32 arg4, u8 arg5, s32 arg6, u8 checkHit, u8 arg8,
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
    if (checkHit && func_150AC9C0(start->x, start->y, start->z, dir->x, dir->y, dir->z, &impact.unk10, &impact.unk14,
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
    streak = func_1513D524(&info, 15, 21, 0, 15, randomBit0 | randomBit1, 0x30, arg5, arg6);
    if (streak != NULL) {
        memcpy(&((Streak *)streak)->impact, &impact, sizeof(impact));
    }
    return streak;
}

extern s32 (*D_80088A10[])(Streak *);

/* Moves the streak along its direction; runs its impact callback once it crosses the surface it was aimed at. */
s8 func_150E6230(Streak *streak) {
    s8 ret = 1;

    {
        StreakImpact *impact = &streak->impact;
        f32 side;
        Vec3f offset;

        if (impact->flags & 1) {
            offset.x = impact->point.x - streak->position.x;
            offset.y = impact->point.y - streak->position.y;
            offset.z = impact->point.z - streak->position.z;
            if (func_15144A74(&offset, &impact->point) < 0.0f) {
                side = -1.0f;
            } else {
                side = 1.0f;
            }
            if (side != impact->side) {
                ret = (impact->callback != -1) ? D_80088A10[impact->callback](streak) : 0;
            }
        }
    }
    {
        f32 vx = streak->direction.x * streak->speed;
        f32 vy = streak->direction.y * streak->speed;
        f32 vz = streak->direction.z * streak->speed;

        streak->position.x += vx * D_800BE9A4;
        streak->position.y += vy * D_800BE9A4;
        streak->position.z += vz * D_800BE9A4;
    }
    return ret;
}

typedef struct {
    s16 x, y, z, flag;
    u8 other[7];
    u8 alpha;
} StreakVtx;

void func_151D5D60(void *, s16, s32, void **, u8 *);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

/* Builds the streak's camera-facing quad (4 vertices) for view index viewIdx; returns NULL without a buffer. */
void *func_150E63A0(Streak *streak, s16 viewIdx) {
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

    func_151D5D60(streak->buffers, viewIdx, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(streak->buffers[viewIdx], streak->vertexTemplate, sizeof(streak->vertexTemplate));
            memcpy(streak->buffers[viewIdx] + 0x40, streak->vertexTemplate, sizeof(streak->vertexTemplate));
        }
    } else {
        return NULL;
    }
    eye = &D_800DBFF0[viewIdx].eye;
    length = streak->length;
    start = streak->position;
    do { // matching: a do/while(0) (a VEC3 macro body) splits uopt's block so length's f0 is reused for dx; { } does not
        end.x = streak->position.x - streak->direction.x * length;
        end.y = streak->position.y - streak->direction.y * length;
        end.z = streak->position.z - streak->direction.z * length;
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
        scale = streak->width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = start.x - nx;
    vertices->y = start.y - ny;
    vertices->z = start.z - nz;
    vertices->alpha = streak->alpha;
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
    vertices->alpha = streak->alpha;
    vertices->flag = 0;
    return result;
}

/* Fades the streak out over its last 8 ticks. */
s32 func_150E679C(Streak *streak) {
    s32 timer = streak->timer;

    if (timer < 8) {
        s32 fade = timer * 32;

        if (fade < streak->alpha) {
            streak->alpha = fade;
        }
    }
    return 1;
}

/* Spawn description passed to func_1513C73C. */
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
s32 func_150E67D0(Streak *streak) {
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
    func_1513C73C((s32)&info, 0, 0, (s32)impact->unk14, impact->point.x, impact->point.y, impact->point.z, 50.0f,
                  50.0f, func_150ADA20() & 0xFF, 0, 0, streak->unkC, streak->unk1);
    return 0;
}
