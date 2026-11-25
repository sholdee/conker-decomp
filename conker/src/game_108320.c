#include <ultra64.h>
#define func_1513D524 func_1513D524_void_proto
#include "functions.h"
#undef func_1513D524
#include "variables.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Spawn packet passed to func_1513D524; it lands in the new Effect at +0x18. */
typedef struct {
    u8  unk0;
    u8  pad1;
    s16 unk2;
    s16 unk4;
    u8  pad6[2];
    s32 unk8;
    s32 unkC;
    u8  color[4];
    u8  pad14[8];
    Vec3f pos;      /* -> Effect.pos */
    Vec3f dir;      /* -> Effect.dir */
    Vec3f scale;
    s32 unk40;
    u8  alpha;      /* -> Effect.alpha */
    u8  unk45;
    u8  pad46[0x12];
} EffectSpawn; /* size 0x58 */

/* Self-propelled beam quad (func_150DAE70 / func_150DB114). */
typedef struct {
    Vec3f vel;
    f32 size;
    f32 sizeRate;
    f32 scaleX;     /* pulses between 0.8 and 1.2 */
    f32 scaleY;
    f32 scaleXRate;
    f32 scaleYRate;
    f32 damping;
    u8  fadeRate;
} BeamMotion;

/* Quad that follows externally owned values (func_150DB518 / func_150DB630 / func_150DB714). */
typedef struct {
    f32 *pos;
    f32 *angle;     /* degrees */
    f32 *halfWidth;
    f32 *halfHeight;
    f32 *alpha;
} LinkedQuad;

typedef struct {
    u8  pad0[0x2C];
    f32 halfWidth;
    f32 halfHeight;
    Vec3f pos;
    Vec3f dir;
    u8  pad4C[0x10];
    u8  alpha;
    u8  pad5D[0x63];
    Vtx_t template[4];
    Vtx_t *buffers[4];  /* each holds two copies of the 4-vertex quad */
    union {
        BeamMotion beam;
        LinkedQuad linked;
    } u;
} Effect;

typedef f32 BoneMtx[4][4];

typedef struct {
    u8  pad0[0x1D4];
    BoneMtx *bones;
} BeamOwner;

extern Effect *func_1513D524(EffectSpawn *spawn, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6, u8 arg7, s32 arg8);
extern void func_15143134(Vec3f *in, Vec3f *out, BoneMtx *mtx);
extern void func_151D5D60(Vtx_t **buffers, s16 index, s32 size, Vtx_t **vertices, u8 *fresh);

extern Vec3f D_80088910[];
extern Vec3f D_8008891C[];
extern s32 D_80088928[];
extern f32 D_8008892C[];
extern s16 D_80088930[];
extern f32 D_80088934[];
extern f32 D_80088938[];
extern f32 D_8008893C[];
extern u8 D_80088940[];
extern f32 D_80088944[];
extern f32 D_80088948[];
extern f32 D_800A0BF0; /* 256.0f / 360.0f, degrees to angle units */

/* Spawns a beam effect between two bone-relative points of the owner's skeleton. */
void func_150DAE70(BeamOwner *owner, s32 type, u8 arg2, s32 arg3) {
    BoneMtx *mtx;
    Vec3f from;
    Vec3f to;
    Vec3f vel;
    EffectSpawn spawn;
    f32 distSq;
    f32 invDist;
    Effect *effect;
    BoneMtx *bones;

    if (owner != NULL) {
        bones = owner->bones;
        if (bones != NULL) {
            mtx = &bones[D_80088928[type]];
            func_15143134(&D_80088910[type], &from, mtx);
            func_15143134(&D_8008891C[type], &to, mtx);
            vel.x = D_8008892C[type] * (to.x - from.x);
            vel.y = D_8008892C[type] * (to.y - from.y);
            vel.z = D_8008892C[type] * (to.z - from.z);
            distSq = (vel.x * vel.x) + (vel.z * vel.z);
            if (!(fabsf(distSq) < 9.999999747e-05f)) {
                spawn.unk0 = 0x17;
                spawn.unkC = 0x243A;
                spawn.unk8 = 0;
                spawn.unk40 = 0x401;
                spawn.unk4 = D_80088930[type];
                spawn.alpha = 255;
                spawn.unk45 = 255;
                // matching: the three stores share one line so as1 keeps IDO's t6-t9 constant order
                spawn.color[2] = 200; spawn.color[1] = 200; spawn.color[0] = 200;
                spawn.color[3] = 200;
                spawn.unk2 = 0x401;
                spawn.pos = from;
                spawn.scale.x = 1.0f;
                spawn.scale.y = 1.0f;
                invDist = 1.0f / sqrtf(distSq);
                spawn.scale.z = 1.0f;
                spawn.dir.x = vel.z * invDist;
                spawn.dir.y = 0.0f;
                spawn.dir.z = vel.x * invDist;
                effect = func_1513D524(&spawn, 9, 0, 0, 7, 0, 0x2C, arg2, arg3);
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
}

/* Beam update: moves, pulses the quad's scale and fades out. Returns 0 once fully faded. */
s32 func_150DB114(Effect *effect) {
    BeamMotion *beam;
    f32 damping;
    s16 alpha;
    f32 max;
    f32 min;

    max = 1.2f;
    min = 0.8f;
    damping = effect->u.beam.damping;
    alpha = effect->alpha;
    beam = &effect->u.beam;
    beam->vel.x *= damping;
    beam->vel.y *= damping;
    beam->vel.z *= damping;
    effect->pos.x += beam->vel.x * D_800BE9A4;
    effect->pos.y += beam->vel.y * D_800BE9A4;
    effect->pos.z += beam->vel.z * D_800BE9A4;
    effect->u.beam.scaleX += effect->u.beam.scaleXRate * D_800BE9A4;
    if (effect->u.beam.scaleX > max) {
        beam->scaleX = max;
        beam->scaleXRate = -beam->scaleXRate;
    } else if (beam->scaleX < min) {
        beam->scaleX = min;
        beam->scaleXRate = -beam->scaleXRate;
    }
    beam->scaleY += beam->scaleYRate * D_800BE9A4;
    if (beam->scaleY > max) {
        beam->scaleY = max;
        beam->scaleYRate = -beam->scaleYRate;
    } else if (beam->scaleY < min) {
        beam->scaleY = min;
        beam->scaleYRate = -beam->scaleYRate;
    }
    beam->size += beam->sizeRate * D_800BE9A4;
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
Vtx_t *func_150DB2D8(Effect *effect, s16 bufferIndex) {
    Vtx_t *vertices;
    Vtx_t *result;
    f32 offsetX;
    f32 offsetZ;
    u8 fresh;

    func_151D5D60(effect->buffers, bufferIndex, 0x40, &vertices, &fresh);
    result = vertices;
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
void func_150DB518(f32 *pos, f32 *angle, f32 *halfWidth, f32 *alpha, f32 *halfHeight, s16 arg5, u8 arg6, s32 arg7) {
    EffectSpawn spawn;
    Effect *effect;

    if (pos != NULL && angle != NULL && halfWidth != NULL && alpha != NULL && halfHeight != NULL && arg5 != 0) {
        spawn.unk0 = 0x17;
        spawn.unkC = 0x243A;
        spawn.unk40 = 0x401;
        spawn.alpha = 255;
        spawn.unk8 = 0;
        spawn.unk4 = arg5;
        spawn.unk45 = 255;
        // matching: the three stores share one line so as1 keeps IDO's t6-t9 constant order
        spawn.color[2] = 200; spawn.color[1] = 200; spawn.color[0] = 200;
        spawn.color[3] = 200;
        spawn.unk2 = 0x401;

        effect = func_1513D524(&spawn, 0, 0xA, 0, 8, 0, 0x14, arg6, arg7);
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
s32 func_150DB630(Effect *effect) {
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
Vtx_t *func_150DB714(Effect *effect, s16 bufferIndex) {
    Vtx_t *vertices;
    Vtx_t *result;
    f32 cosine;
    s32 angle;
    f32 sine;
    f32 offsetX;
    f32 offsetZ;
    LinkedQuad *linked;
    u8 fresh;

    func_151D5D60(effect->buffers, bufferIndex, 0x40, &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(effect->buffers[bufferIndex], effect->template, sizeof(effect->template));
            memcpy(effect->buffers[bufferIndex] + 4, effect->template, sizeof(effect->template));
        }
    } else {
        return NULL;
    }
    angle = *effect->u.linked.angle * D_800A0BF0;
    linked = &effect->u.linked;
    sine = func_151423D8(angle);
    cosine = func_151423D8(angle - 0x40);
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
