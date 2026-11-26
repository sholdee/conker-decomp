#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-type beam parameters (one type): the two bone-relative end points, the bone, the speed scale,
 * the lifetime, the size and its rate, the damping, the fade rate and the two scale rates. */
static Vec3f D_80088910[] = { { 0.0f, -33.0f, 40.0f } };
static Vec3f D_8008891C[] = { { 0.0f, -74.0f, 98.0f } };
static s32 D_80088928[] = { 15 };
static f32 D_8008892C[] = { 0.108f };
static s16 D_80088930[] = { 100 };
static f32 D_80088934[] = { 8.11f };
static f32 D_80088938[] = { 0.7f };
static f32 D_8008893C[] = { 0.948f };
static u8 D_80088940[] = { 4 };
static f32 D_80088944[] = { 0.007f };
static f32 D_80088948[] = { 0.012f };

/* Spawns a beam effect between two bone-relative points of the owner's skeleton. */
void beam_spawnBoneQuad(Actor *owner, s32 type, u8 arg2, s32 arg3) {
    MtxF *mtx;
    Vec3f from;
    Vec3f to;
    Vec3f vel;
    ParticleDef spawn;
    f32 distSq;
    f32 invDist;
    BeamQuad *effect;

    if (owner != NULL && owner->ob_matrix_ptr != NULL) {
        mtx = &owner->ob_matrix_ptr[D_80088928[type]];
        math_transformOffset(&D_80088910[type], &from, mtx);
        math_transformOffset(&D_8008891C[type], &to, mtx);
        vel.x = D_8008892C[type] * (to.x - from.x);
        vel.y = D_8008892C[type] * (to.y - from.y);
        vel.z = D_8008892C[type] * (to.z - from.z);
        distSq = (vel.x * vel.x) + (vel.z * vel.z);
        if (!(fabsf(distSq) < 9.999999747e-05f)) {
            spawn.texture = 0x17;
            spawn.frameSpeed = 0x243A;
            spawn.frame = 0;
            spawn.flags = 0x401;
            spawn.lifetime = D_80088930[type];
            spawn.unk44 = 255;
            spawn.unk45 = 255;
            // matching: the three stores share one line so as1 keeps IDO's t6-t9 constant order
            spawn.rgba[2] = 200; spawn.rgba[1] = 200; spawn.rgba[0] = 200;
            spawn.rgba[3] = 200;
            spawn.renderModes = 0x401;
            spawn.pos = from;
            spawn.scale.x = 1.0f;
            spawn.scale.y = 1.0f;
            invDist = 1.0f / sqrtf(distSq);
            spawn.scale.z = 1.0f;
            spawn.rot.x = vel.z * invDist;
            spawn.rot.y = 0.0f;
            spawn.rot.z = vel.x * invDist;
            effect = particle_newQuadParticleWhite(&spawn, 9, 0, 0, 7, 0, 0x2C, arg2, arg3);
            if (effect != NULL) {
                effect->u.beam.vel = vel;
                effect->u.beam.size = D_80088934[type];
                effect->u.beam.sizeRate = D_80088938[type];
                effect->u.beam.scaleXRate = D_80088944[type];
                effect->u.beam.scaleYRate = D_80088948[type];
                effect->u.beam.scaleX = effect->u.beam.scaleY = 1.0f;
                effect->u.beam.damping = D_8008893C[type];
                effect->u.beam.fadeRate = D_80088940[type];
            }
        }
    }
}

/* Beam update: moves, pulses the quad's scale and fades out. Returns 0 once fully faded. */
s32 beam_updateQuad(BeamQuad *effect) {
    BeamMotion *beam;
    s16 alpha;

    beam = &effect->u.beam;
    alpha = effect->alpha;
    beam->vel.x *= beam->damping;
    beam->vel.y *= beam->damping;
    beam->vel.z *= beam->damping;
    effect->pos.x += beam->vel.x * gFrameDelta;
    effect->pos.y += beam->vel.y * gFrameDelta;
    effect->pos.z += beam->vel.z * gFrameDelta;
    beam->scaleX += beam->scaleXRate * gFrameDelta;
    if (beam->scaleX > 1.2f) {
        beam->scaleX = 1.2f;
        beam->scaleXRate = -beam->scaleXRate;
    } else if (beam->scaleX < 0.8f) {
        beam->scaleX = 0.8f;
        beam->scaleXRate = -beam->scaleXRate;
    }
    beam->scaleY += beam->scaleYRate * gFrameDelta;
    if (beam->scaleY > 1.2f) {
        beam->scaleY = 1.2f;
        beam->scaleYRate = -beam->scaleYRate;
    } else if (beam->scaleY < 0.8f) {
        beam->scaleY = 0.8f;
        beam->scaleYRate = -beam->scaleYRate;
    }
    beam->size += beam->sizeRate * gFrameDelta;
    effect->halfWidth = beam->size * beam->scaleX;
    effect->halfHeight = beam->size * beam->scaleY;
    alpha -= beam->fadeRate;
    if (alpha < 0) {
        effect->alpha = 0;
        return 0;
    }
    effect->alpha = alpha;
    return 1;
}

/* Beam draw: fills this frame's vertex buffer with a quad facing along dir. */
Vtx_t *beam_drawQuad(BeamQuad *effect, s16 bufferIndex) {
    Vtx_t *vertices;
    Vtx_t *result;
    f32 offsetX;
    f32 offsetZ;
    u8 fresh;

    particle_getViewBuffer(effect->buffers, bufferIndex, sizeof(effect->template), &vertices, &fresh);
    result = vertices;
    // matching: an early `return NULL` reorders the blocks; the else form keeps IDO's layout
    if (vertices != NULL) {
        if (fresh) {
            memcpy(effect->buffers[bufferIndex], effect->template, sizeof(effect->template));
            memcpy(effect->buffers[bufferIndex] + 4, effect->template, sizeof(effect->template));
        }
    } else {
        return NULL;
    }

    offsetX = effect->halfWidth * effect->dir.x;
    offsetZ = -effect->halfWidth * effect->dir.z;

    vertices[0].ob[0] = effect->pos.x + offsetX;
    vertices[0].ob[1] = effect->pos.y + effect->halfHeight;
    vertices[0].ob[2] = effect->pos.z + offsetZ;
    vertices[0].flag = 0;
    vertices[1].ob[0] = effect->pos.x + offsetX;
    vertices[1].ob[1] = effect->pos.y - effect->halfHeight;
    vertices[1].ob[2] = effect->pos.z + offsetZ;
    vertices[1].flag = 0;
    vertices[2].ob[0] = effect->pos.x - offsetX;
    vertices[2].ob[1] = effect->pos.y - effect->halfHeight;
    vertices[2].ob[2] = effect->pos.z - offsetZ;
    vertices[2].flag = 0;
    vertices[3].ob[0] = effect->pos.x - offsetX;
    vertices[3].ob[1] = effect->pos.y + effect->halfHeight;
    vertices[3].ob[2] = effect->pos.z - offsetZ;
    vertices[3].flag = 0;

    return result;
}

/* Spawns a quad whose position, angle, size and alpha follow the given values. */
void beam_spawnLinkedQuad(f32 *pos, f32 *angle, f32 *halfWidth, f32 *alpha, f32 *halfHeight, s16 arg5, u8 arg6, s32 arg7) {
    ParticleDef spawn;
    BeamQuad *effect;

    if (pos != NULL && angle != NULL && halfWidth != NULL && alpha != NULL && halfHeight != NULL && arg5 != 0) {
        spawn.texture = 0x17;
        spawn.frameSpeed = 0x243A;
        spawn.flags = 0x401;
        spawn.unk44 = 255;
        spawn.frame = 0;
        spawn.lifetime = arg5;
        spawn.unk45 = 255;
        // matching: the three stores share one line so as1 keeps IDO's t6-t9 constant order
        spawn.rgba[2] = 200; spawn.rgba[1] = 200; spawn.rgba[0] = 200;
        spawn.rgba[3] = 200;
        spawn.renderModes = 0x401;

        effect = particle_newQuadParticleWhite(&spawn, 0, 0xA, 0, 8, 0, 0x14, arg6, arg7);
        if (effect != NULL) {
            effect->u.linked.pos = pos;
            effect->u.linked.angle = angle;
            effect->u.linked.halfWidth = halfWidth;
            effect->u.linked.halfHeight = halfHeight;
            effect->u.linked.alpha = alpha;
        }
    }
}

/* Linked quad update: copies the followed alpha, clamped to 0..255. */
s32 beam_updateLinkedQuad(BeamQuad *effect) {
    f32 **source;
    f32 alpha;

    // matching: reading through a second path to the field makes IDO reload it in the else branch
    source = &effect->u.linked.alpha;
    if (*effect->u.linked.alpha > 255.0f) {
        effect->alpha = 255;
    } else {
        alpha = **source;
        if (alpha < 0.0f) {
            effect->alpha = 0;
        } else {
            effect->alpha = (u32)alpha;
        }
    }
    return 1;
}

/* Linked quad draw: fills this frame's vertex buffer with a quad rotated by the followed angle. */
Vtx_t *beam_drawLinkedQuad(BeamQuad *effect, s16 bufferIndex) {
    Vtx_t *vertices;
    Vtx_t *result;
    f32 cosine;
    s32 angle;
    f32 sine;
    f32 offsetX;
    f32 offsetZ;
    LinkedQuad *linked;
    u8 fresh;

    particle_getViewBuffer(effect->buffers, bufferIndex, sizeof(effect->template), &vertices, &fresh);
    result = vertices;
    // matching: an early `return NULL` reorders the blocks; the else form keeps IDO's layout
    if (vertices != NULL) {
        if (fresh) {
            memcpy(effect->buffers[bufferIndex], effect->template, sizeof(effect->template));
            memcpy(effect->buffers[bufferIndex] + 4, effect->template, sizeof(effect->template));
        }
    } else {
        return NULL;
    }
    linked = &effect->u.linked;
    angle = *linked->angle * (256.0f / 360.0f);
    sine = math_cosBinAngleMasked(angle);
    cosine = math_cosBinAngleMasked(angle - 0x40);
    offsetX = *linked->halfWidth * sine;
    offsetZ = -*linked->halfWidth * cosine;
    vertices[0].ob[0] = linked->pos[0] + offsetX;
    vertices[0].ob[1] = linked->pos[1] + *linked->halfHeight;
    vertices[0].ob[2] = linked->pos[2] + offsetZ;
    vertices[0].flag = 0;
    vertices[1].ob[0] = linked->pos[0] + offsetX;
    vertices[1].ob[1] = linked->pos[1] - *linked->halfHeight;
    vertices[1].ob[2] = linked->pos[2] + offsetZ;
    vertices[1].flag = 0;
    vertices[2].ob[0] = linked->pos[0] - offsetX;
    vertices[2].ob[1] = linked->pos[1] - *linked->halfHeight;
    vertices[2].ob[2] = linked->pos[2] - offsetZ;
    vertices[2].flag = 0;
    vertices[3].ob[0] = linked->pos[0] - offsetX;
    vertices[3].ob[1] = linked->pos[1] + *linked->halfHeight;
    vertices[3].ob[2] = linked->pos[2] - offsetZ;
    vertices[3].flag = 0;
    return result;
}
