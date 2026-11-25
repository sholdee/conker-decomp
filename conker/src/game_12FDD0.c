#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"
#include "libc/string.h"

/* Per-particle data of a beam attached to an actor's bone, stored at Particle.beam. */
typedef struct {
    /* 0x00 */ struct127 *target;
    /* 0x04 */ u8 targetId;
    /* 0x08 */ s32 boneOffset; /* byte offset of the bone's matrix in target->unk1D4 */
    /* 0x0C */ s16 node;       /* func_1503195C node, -1: use the bone matrix */
    /* 0x10 */ s32 bone;
    /* 0x14 */ u8 flags;       /* bit 0: start/end are valid this frame */
    /* 0x18 */ struct17 start; /* def.pos and def.rot transformed to world space */
    /* 0x24 */ struct17 end;
} BeamData; /* size 0x30 */

/* A particle spawned by func_1513D2F0 (as Particle in game_169510.c, followed by its data). */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ ParticleDef def;
    /* 0x070 */ u8 pad70[0x50];
    /* 0x0C0 */ Vtx vtxTemplate[4];
    /* 0x100 */ Vtx *vtxBuffers[4]; /* per viewport */
    /* 0x110 */ BeamData beam;
} Particle;

/* First argument of func_1513C650 (as Struct1513C650Arg in game_113D60.c). */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 unk06;
    /* 0x07 */ u8 unk07;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
} Struct1513C650Arg;

/* ParticleDesc.flags (see game_15D730.c) */
#define PARTICLE_LIFETIME  0x1
#define PARTICLE_FLIP_S    0x40
#define PARTICLE_FLIP_T    0x80
#define PARTICLE_FADE      0x200
#define PARTICLE_HANDLERS  0x4000
#define PARTICLE_DRAW_FUNC 0x8000
#define PARTICLE_VIEWS     0x40000

void func_15103254(s16 lifetime, u8 alpha, f32 size, vertex *pos, u8 views, u8 arg5, s32 arg6);

/* Spawns a func_1513C650 effect of the given size on a surface (none for some materials on maps 0x36 and 0x26);
 * a duration of -1 means a fixed 300 frames without the timed flag. */
struct210 *func_15102920(f32 size, u8 arg1, s32 arg2, struct17 *position, s16 duration, u8 arg5, u8 arg6,
                         s32 surfaceFlags, u8 arg8, s32 arg9) {
    struct210 *result;
    Struct1513C650Arg arg;
    s32 argB;
    s32 argC;
    s32 material;

    arg.unk22 = 10;
    arg.unk24 = 25;
    material = func_1510F8CC(surfaceFlags);
    arg.unk06 = 0x5C;
    switch (D_800BE9F0) {
    case 0x36:
        if (material == 9) {
            return NULL;
        }
        arg.unk06 = 0x85;
        size *= 0.6000000238f;
        break;
    case 0x26:
        if (material == 2) {
            return NULL;
        }
        break;
    }
    arg.unk07 = 0;
    arg.flags = ((duration == -1) ? 0 : 1) | 0x100 | 0x200 | 0x400 | 0x1000 | 0x8000 | 0x20000;
    arg.lifetime = (duration == -1) ? 300 : duration;
    arg.unk08 = 0;
    arg.unk0C = 0;
    if (D_800BE9F0 == 0x36) {
        arg.unk10 = arg1 >> 1;
    } else {
        arg.unk10 = arg1;
    }
    arg.unk11 = 0xFF;
    arg.unk14 = 0;
    arg.unk13 = 0;
    arg.unk12 = 0;
    arg.unk15 = 0xFF;
    arg.unk18 = ((arg6 != 0) ? 2 : 1) + 0x3B0000;
    arg.unk16 = 0;
    arg.unk17 = 7;
    if (arg5 != 0) {
        argB = 3;
        argC = 0xFF;
    } else {
        argB = 0;
        argC = 0;
    }
    result = func_1513C650((s32)&arg, 0, 0, arg2, position->unk0, position->unk4, position->unk8, size, size,
                           func_150ADA20() & 0xFF, (func_150ADA20() & 1) + ((func_150ADA20() & 1) * 2), argB, argC, 0,
                           arg8, arg9);
    return result;
}

/* Spawns a beam particle from `start` to `end`, attached to bone `bone` of `target` (or to its model node `node`),
 * plus a flash at `flashPos`. */
Particle *func_15102B38(struct127 *target, u8 bone, vertex *start, vertex *end, ParticleSize *size, s16 lifetime,
                  u8 alpha, f32 flashSize, struct17 *flashPos, u8 viewMask, s32 extraSize, s16 node, u8 arg12,
                  s32 arg13) {
    Particle *particle;
    ParticleDef def;
    BeamData beam;
    s32 flip;

    if (target == NULL) {
        return NULL;
    }

    beam.flags = 0;
    beam.start = *(struct17 *)&D_800A5480;
    beam.end = *(struct17 *)&D_800A5480;
    beam.bone = bone;
    beam.target = target;
    beam.node = node;
    beam.targetId = target->unique_id;
    beam.boneOffset = bone * sizeof(Mtx);
    def.texture = 0x5F;
    def.type = 5;
    def.renderModes = 0x2203;
    def.frame = 0;
    def.frameSpeed = 0;
    def.lifetime = lifetime;
    def.rgba[0] = 0xFF;
    def.rgba[1] = 0xFF;
    def.rgba[2] = 0xFF;
    def.rgba[3] = 0xFF;
    *(ParticleSize *)&def.width = *size; // matching: width and height are copied as one struct
    def.pos = *start;
    def.rot = *end;
    def.flags = 0x40CC0009;
    def.unk44 = alpha;
    def.unk45 = 0xFF;
    def.unk46 = 0;
    def.unk47 = 7;
    def.scale.x = 0.0f;
    def.scale.y = 0.0f;
    def.scale.z = 0.0f;
    def.viewportMask = viewMask;

    if (func_150ADA20() & 1) {
        flip = 2;
    } else {
        flip = 0;
    }

    particle = func_1513D2F0(&def, (s32)&D_800A4AA0, 0x29, 0, 0, 0x16, flip + 1, 0, 0,
                                  extraSize + sizeof(BeamData), arg12, arg13);
    if (particle != NULL) {
        memcpy(&particle->beam, &beam, sizeof(BeamData));
    }

    func_15103254(lifetime, alpha, flashSize, flashPos, viewMask, arg12, arg13);
    return particle;
}

/* Particle type handler: hands the beam's target pointer and id to func_15169850. */
void func_15102D50(Particle *particle, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, (s32)&particle->beam.target, (s32)&particle->beam.targetId, (s32)particle);
}

extern s32 func_1514654C(struct127 *, ActorCtl *, s32, void *, void *, s32);
extern void func_15145EA4(struct17 **src, struct17 **dst, Mtx *matrix, s32 count);

/* Beam update: moves the beam's end points with its target's bone or model node.
 * Returns 0 when the beam should die (target gone or node missing). */
s32 func_15102D90(Particle *particle) {
    struct127 *target;
    BeamData *beam;
    struct17 *src[2];
    struct17 *dst[2];

    beam = &particle->beam;
    beam->flags &= ~1;
    target = beam->target;
    if (target->unique_id != beam->targetId || target->interaction_state == 0) {
        return 0;
    }
    if (target->unk1D4 == NULL || (target->unk74 & 0xF) == 0xF) {
        return 1;
    }
    src[0] = &particle->def.pos;
    src[1] = &particle->def.rot;
    dst[0] = &beam->start;
    dst[1] = &beam->end;
    if (beam->node != -1) {
        ActorCtl *node;

        node = func_1503195C(target, beam->node, 0);
        if (node == NULL) {
            return 0;
        }
        if (func_1514654C(target, node, beam->bone, src, dst, 2) == 0) {
            return 0;
        }
    } else {
        func_15145EA4(src, dst, (Mtx *)((u8 *)target->unk1D4 + beam->boneOffset), 2);
    }
    beam->flags |= 1;
    return 1;
}

extern struct17 *func_15144B34(s32);
void func_151D5D60(Vtx **buffers, s16 viewport, s32 size, Vtx **vtx, u8 *fresh);
void func_15145548(struct17 *origin, struct17 *dir, struct17 *point, struct17 *closest, f32 *t);

/* Vertex function: builds the beam's quad for `viewport`, from its start to def.height of the way to its end,
 * def.width wide and facing the viewport's camera. */
Vtx *func_15102EB8(Particle *particle, s16 viewport) {
    struct17 *camera;
    struct17 mid;
    struct17 dir;
    BeamData *beam;
    struct17 closest;
    f32 dx, dy, dz;
    f32 nx, ny, nz;
    Vtx *vtx;
    Vtx *result;
    f32 t;
    f32 lengthSquared;
    u8 fresh;
    f32 scale;

    camera = func_15144B34(viewport);
    if (!(particle->beam.flags & 1)) {
        return NULL;
    }
    func_151D5D60(particle->vtxBuffers, viewport, 0x40, &vtx, &fresh);
    result = vtx;
    beam = &particle->beam;
    // matching: an early return for NULL reorders the blocks; the original tests vtx != NULL first
    if (vtx != NULL) {
        if (fresh) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    mid.unk0 = beam->start.unk0 + (beam->end.unk0 - beam->start.unk0) * particle->def.height;
    mid.unk4 = beam->start.unk4 + (beam->end.unk4 - beam->start.unk4) * particle->def.height;
    mid.unk8 = beam->start.unk8 + (beam->end.unk8 - beam->start.unk8) * particle->def.height;
    dir.unk0 = mid.unk0 - beam->start.unk0;
    dir.unk4 = mid.unk4 - beam->start.unk4;
    dir.unk8 = mid.unk8 - beam->start.unk8;
    func_15145548(&beam->start, &dir, camera, &closest, &t);
    dx = closest.unk0 - camera->unk0;
    dy = closest.unk4 - camera->unk4;
    dz = closest.unk8 - camera->unk8;
    nx = dir.unk4 * dz - dy * dir.unk8;
    ny = dir.unk8 * dx - dz * dir.unk0;
    nz = dir.unk0 * dy - dx * dir.unk4;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = particle->def.width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vtx->v.ob[0] = beam->start.unk0 + nx;
    vtx->v.ob[1] = beam->start.unk4 + ny;
    vtx->v.ob[2] = beam->start.unk8 + nz;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = mid.unk0 + nx;
    vtx->v.ob[1] = mid.unk4 + ny;
    vtx->v.ob[2] = mid.unk8 + nz;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = mid.unk0 - nx;
    vtx->v.ob[1] = mid.unk4 - ny;
    vtx->v.ob[2] = mid.unk8 - nz;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = beam->start.unk0 - nx;
    vtx->v.ob[1] = beam->start.unk4 - ny;
    vtx->v.ob[2] = beam->start.unk8 - nz;
    vtx->v.flag = 0;
    return result;
}

extern s32 D_80088BD0[];
extern void *func_15130374(ParticleDesc *, u8, s32, u8, s32);

/* Spawns a flash particle of the given size at `pos`, with one of four random textures and random flips. */
void func_15103254(s16 lifetime, u8 alpha, f32 size, vertex *pos, u8 views, u8 arg5, s32 arg6) {
    s32 index;
    ParticleDesc desc;

    index = func_150ADA20() & 3;
    desc.texture = D_80088BD0[index];
    desc.renderMode = 0x2203;
    desc.unk0 = 0x200005;
    desc.unk4 = 0x20000;
    desc.timer = lifetime + 1;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r = 0xFF;
    desc.g = 0xFF;
    desc.b = 0xFF;
    desc.a = 0xFF;
    desc.r2 = 0xFF;
    desc.g2 = 0xFF;
    desc.b2 = 0xFF;
    desc.alpha = alpha;
    desc.brightness = 0xFF;
    desc.height = size;
    desc.width = size;
    desc.pos = *pos;
    desc.fadeStart = 3;
    desc.fadeRate = 0x55;
    desc.scaleStart = 1;
    desc.offset.x = 0.0f;
    desc.offset.y = 0.0f;
    desc.offset.z = 0.0f;
    desc.vel.x = 0.0f;
    desc.vel.y = 0.0f;
    desc.vel.z = 0.0f;
    desc.gravity = 0.0f;
    desc.scaleRate = 1.0f;
    // matching: IDO ORs the terms in this grouping
    desc.flags = (func_150ADA20() & 1 ? PARTICLE_FLIP_S : 0) |
                 (PARTICLE_LIFETIME | (func_150ADA20() & 1 ? PARTICLE_FLIP_T : 0)) | PARTICLE_FADE | PARTICLE_HANDLERS |
                 PARTICLE_DRAW_FUNC | PARTICLE_VIEWS | 0x800000;
    desc.primColorMode = 6;
    desc.envColorMode = 8;
    desc.updateFunc = -1;
    desc.deathFunc = -1;
    desc.drawFunc = -1;
    desc.handlerSet = 0;
    desc.views = views;
    func_15130374(&desc, 1, 0, arg5, arg6);
}
