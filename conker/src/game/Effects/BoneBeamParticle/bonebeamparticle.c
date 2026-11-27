#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "libc/string.h"

/* This file's .data: the four textures effect_spawnFlashParticle picks one of. */
static s32 gFlashTextures[4] = { 0x60, 0x61, 0x62, 0x63 };

/* Per-particle data of a beam attached to an actor's bone, stored at BoneBeamParticle.beam. */
typedef struct {
    /* 0x00 */ Actor *target;
    /* 0x04 */ u8 targetId;
    /* 0x08 */ s32 boneOffset; /* byte offset of the bone's matrix in target->ob_matrix_ptr */
    /* 0x0C */ s16 node;       /* attach_find node, -1: use the bone matrix */
    /* 0x10 */ s32 bone;
    /* 0x14 */ u8 flags;       /* bit 0: start/end are valid this frame */
    /* 0x18 */ Vec3f start; /* def.pos and def.rot transformed to world space */
    /* 0x24 */ Vec3f end;
} BoneBeamData; /* size 0x30 */

/* A particle spawned by particle_newQuadParticle (a DefParticleHead followed by its data). */
struct BoneBeamParticle {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ BoneBeamData beam;
};

/* Spawns a particle_spawnSurfaceQuadSprite effect of the given size on a surface (none for some materials on maps 0x36 and 0x26);
 * a duration of -1 means a fixed 300 frames without the timed flag. */
QuadSprite *effect_spawnSurfaceSprite(f32 size, u8 arg1, CollisionTri *surface, Vec3f *position, s16 duration, u8 arg5, u8 arg6,
                         s32 surfaceFlags, u8 arg8, s32 arg9) {
    QuadSprite *result;
    SpriteDef arg;
    s32 argB;
    s32 argC;
    s32 material;

    arg.fadeStart = 10;
    arg.fadeRate = 25;
    material = collision_getSurfaceType(surfaceFlags);
    arg.texture = 0x5C;
    switch (level) {
    case LEVEL_W5_BANK:
        if (material == 9) {
            return NULL;
        }
        arg.texture = 0x85;
        size *= 0.6000000238f;
        break;
    case LEVEL_W2_MACHINEGUNNEST:
        if (material == 2) {
            return NULL;
        }
        break;
    }
    arg.unk7 = 0;
    arg.flags = ((duration == -1) ? 0 : 1) | 0x100 | 0x200 | 0x400 | 0x1000 | 0x8000 | 0x20000;
    arg.lifetime = (duration == -1) ? 300 : duration;
    arg.frame = 0;
    arg.frameSpeed = 0;
    if (level == LEVEL_W5_BANK) {
        arg.unk10 = arg1 >> 1;
    } else {
        arg.unk10 = arg1;
    }
    arg.unk11 = 0xFF;
    arg.rgba[2] = 0;
    arg.rgba[1] = 0;
    arg.rgba[0] = 0;
    arg.rgba[3] = 0xFF;
    arg.renderModes = ((arg6 != 0) ? 2 : 1) + 0x3B0000;
    arg.unk16 = 0;
    arg.unk17 = 7;
    if (arg5 != 0) {
        argB = 3;
        argC = 0xFF;
    } else {
        argB = 0;
        argC = 0;
    }
    result = particle_spawnSurfaceQuadSprite(&arg, 0, 0, surface, position->x, position->y, position->z, size, size,
                           math_random() & 0xFF, (math_random() & 1) + ((math_random() & 1) * 2), argB, argC, 0,
                           arg8, arg9);
    return result;
}

/* Spawns a beam particle from `start` to `end`, attached to bone `bone` of `target` (or to its model node `node`),
 * plus a flash at `flashPos`. */
BoneBeamParticle *effect_spawnBoneBeamParticle(Actor *target, u8 bone, const Vec3f *start, const Vec3f *end, ParticleSize *size, s16 lifetime,
                  u8 alpha, f32 flashSize, Vec3f *flashPos, u8 viewMask, s32 extraSize, s16 node, u8 arg12,
                  s32 arg13) {
    BoneBeamParticle *particle;
    ParticleDef def;
    BoneBeamData beam;
    s32 flip;

    if (target == NULL) {
        return NULL;
    }

    beam.flags = 0;
    beam.start = gZeroVec3f;
    beam.end = gZeroVec3f;
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

    if (math_random() & 1) {
        flip = 2;
    } else {
        flip = 0;
    }

    particle = particle_newQuadParticle(&def, &gWhiteVtxColors, 0x29, 0, 0, 0x16, flip + 1, 0, 0,
                                  extraSize + sizeof(BoneBeamData), arg12, arg13);
    if (particle != NULL) {
        memcpy(&particle->beam, &beam, sizeof(BoneBeamData));
    }

    effect_spawnFlashParticle(lifetime, alpha, flashSize, flashPos, viewMask, arg12, arg13);
    return particle;
}

/* Particle type handler: hands the beam's target pointer and id to effect_handleOwnerActorEvent. */
void effect_boneBeamMessage(BoneBeamParticle *particle, s32 arg1, u8 arg2) {
    effect_handleOwnerActorEvent(arg1, arg2, &particle->beam.target, &particle->beam.targetId, particle);
}

/* Beam update: moves the beam's end points with its target's bone or model node.
 * Returns 0 when the beam should die (target gone or node missing). */
s32 effect_updateBoneBeamParticle(BoneBeamParticle *particle) {
    Actor *target;
    BoneBeamData *beam;
    const Vec3f *src[2];
    Vec3f *dst[2];

    beam = &particle->beam;
    beam->flags &= ~1;
    target = beam->target;
    if (target->unique_id != beam->targetId || target->type == 0) {
        return 0;
    }
    if (target->ob_matrix_ptr == NULL || (target->cullMask & 0xF) == 0xF) {
        return 1;
    }
    src[0] = &particle->head.def.pos;
    src[1] = &particle->head.def.rot;
    dst[0] = &beam->start;
    dst[1] = &beam->end;
    if (beam->node != -1) {
        ModelAttachment *node;

        node = attach_find(target, beam->node, 0);
        if (node == NULL) {
            return 0;
        }
        if (math_transformAttachmentOffsets(target, node, beam->bone, src, dst, 2) == 0) {
            return 0;
        }
    } else {
        math_transformOffsets(src, dst, (u8 *)target->ob_matrix_ptr + beam->boneOffset, 2);
    }
    beam->flags |= 1;
    return 1;
}

/* Vertex function: builds the beam's quad for `viewport`, from its start to def.height of the way to its end,
 * def.width wide and facing the viewport's camera. */
Vtx *effect_buildBoneBeamQuad(BoneBeamParticle *particle, s16 viewport) {
    Vec3f *camera;
    Vec3f mid;
    Vec3f dir;
    BoneBeamData *beam;
    Vec3f closest;
    f32 dx, dy, dz;
    f32 nx, ny, nz;
    Vtx *vtx;
    Vtx *result;
    f32 t;
    f32 lengthSquared;
    u8 fresh;
    f32 scale;

    camera = math_getCameraPos(viewport);
    if (!(particle->beam.flags & 1)) {
        return NULL;
    }
    particle_getViewBuffer(particle->head.vtxBuffers, viewport, 0x40, &vtx, &fresh);
    result = vtx;
    beam = &particle->beam;
    // matching: an early return for NULL reorders the blocks; the original tests vtx != NULL first
    if (vtx != NULL) {
        if (fresh) {
            memcpy(particle->head.vtxBuffers[viewport], particle->head.vtxTemplate, sizeof(particle->head.vtxTemplate));
            memcpy(particle->head.vtxBuffers[viewport] + 4, particle->head.vtxTemplate, sizeof(particle->head.vtxTemplate));
        }
    } else {
        return NULL;
    }
    mid.x = beam->start.x + (beam->end.x - beam->start.x) * particle->head.def.height;
    mid.y = beam->start.y + (beam->end.y - beam->start.y) * particle->head.def.height;
    mid.z = beam->start.z + (beam->end.z - beam->start.z) * particle->head.def.height;
    dir.x = mid.x - beam->start.x;
    dir.y = mid.y - beam->start.y;
    dir.z = mid.z - beam->start.z;
    math_closestPointOnSegment(&beam->start, &dir, camera, &closest, &t);
    dx = closest.x - camera->x;
    dy = closest.y - camera->y;
    dz = closest.z - camera->z;
    nx = dir.y * dz - dy * dir.z;
    ny = dir.z * dx - dz * dir.x;
    nz = dir.x * dy - dx * dir.y;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = particle->head.def.width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vtx->v.ob[0] = beam->start.x + nx;
    vtx->v.ob[1] = beam->start.y + ny;
    vtx->v.ob[2] = beam->start.z + nz;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = mid.x + nx;
    vtx->v.ob[1] = mid.y + ny;
    vtx->v.ob[2] = mid.z + nz;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = mid.x - nx;
    vtx->v.ob[1] = mid.y - ny;
    vtx->v.ob[2] = mid.z - nz;
    vtx->v.flag = 0;
    vtx++;
    vtx->v.ob[0] = beam->start.x - nx;
    vtx->v.ob[1] = beam->start.y - ny;
    vtx->v.ob[2] = beam->start.z - nz;
    vtx->v.flag = 0;
    return result;
}

/* Spawns a flash particle of the given size at `pos`, with one of four random textures and random flips. */
void effect_spawnFlashParticle(s16 lifetime, u8 alpha, f32 size, Vec3f *pos, u8 views, u8 arg5, s32 arg6) {
    s32 index;
    ParticleDesc desc;

    index = math_random() & 3;
    desc.texture = gFlashTextures[index];
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
    desc.flags = (math_random() & 1 ? PARTICLE_FLIP_S : 0) |
                 (PARTICLE_LIFETIME | (math_random() & 1 ? PARTICLE_FLIP_T : 0)) | PARTICLE_FADE | PARTICLE_HANDLERS |
                 PARTICLE_DRAW_FUNC | PARTICLE_VIEWS | 0x800000;
    desc.primColorMode = 6;
    desc.envColorMode = 8;
    desc.updateFunc = -1;
    desc.deathFunc = -1;
    desc.drawFunc = -1;
    desc.handlerSet = 0;
    desc.views = views;
    particle_newOwnedSpriteParticle(&desc, 1, 0, arg5, arg6);
}
