#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef f32 BoneMtx[4][4];

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Effect data of the wavy beam drawn between two actors' bones (func_151B5328). */
typedef struct {
    /* 0x000 */ Vtx vertices[2][44];  // double-buffered, indexed by D_800BE9C0
    /* 0x580 */ struct127 *first;
    /* 0x584 */ u8 firstId;             // first->unique_id, to notice the slot being reused
    /* 0x585 */ u8 pad585[3];
    /* 0x588 */ struct127 *second;
    /* 0x58C */ u8 secondId;
    /* 0x58D */ u8 pad58D[3];
    /* 0x590 */ f32 width;
    /* 0x594 */ f32 fadeStart;          // beam length where the wave starts
    /* 0x598 */ f32 fadeEnd;            // beam length where the wave is at fadeMax
    /* 0x59C */ f32 fadeScale;          // 1 / (fadeEnd - fadeStart)
    /* 0x5A0 */ f32 fadeMax;            // wave amplitude, as a fraction of width
    /* 0x5A4 */ u8 unk5A4[4];
} BeamData; // size 0x5A8

typedef struct {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ s16 state;               // -1 kills the effect
    /* 0x10 */ u8 pad10[0x28 - 0x10];
    /* 0x28 */ BeamData data;
} BeamEffect;

/* Message 0x2D: actors a and b swapped slots. */
typedef struct {
    struct127 *a;
    struct127 *b;
    u8 aId;
    u8 bId;
} ActorSwap;

/* Message 0: an actor went away. */
typedef struct {
    struct127 *actor;
    u8 id;
} ActorGone;

/* Spawns a beam effect between actors 0x17 and 0x16. */
BeamEffect *func_151B5240(s16 duration, u8 arg1, s32 arg2) {
    BeamEffect *effect;
    BeamData data;

    data.firstId = 0x17;
    data.secondId = 0x16;
    data.first = func_15083E90(0x17);
    data.second = func_15083E90(data.secondId);
    data.fadeEnd = 250.0f;
    data.width = 18.0f;
    data.fadeStart = 0.0f;
    data.fadeMax = 0.9f;
    data.fadeScale = 1.0f / data.fadeEnd;

    effect = (BeamEffect *)func_15149130(duration, -1, -1, 1, 1, 0x15, (struct37 *)sizeof(data), arg1, arg2);
    if (effect != NULL) {
        memcpy(&effect->data, &data, sizeof(data));
    }
    return effect;
}

void func_15143134(const f32 *in, Vec3f *out, BoneMtx *mtx);
Gfx *func_15142E24(Gfx *, u8 *, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_15142C10(Gfx *, s32, s32, s32, s32, u8 *);
Gfx *func_15142CF0(Gfx *, s32, s32, s32, s32, s32, s32, u8 *);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
f32 func_15143E64(Vec3f *);
extern u8 D_8008FB80;
extern u8 D_8008FB84;
extern u8 D_80090F68[];
extern s32 D_800A4AC8[][2];
static const f32 D_800AA400[3] = { -3.0f, 11.0f, -4.0f };  // beam start, in first's bone space
static const f32 D_800AA40C[3] = { 0.0f, -5.0f, -7.0f };   // beam end, in second's bone space
extern s32 D_800D2C9C;

/*
 * Beam effect draw: a red textured strip of 10 quads from a point on first's bone D_8008FB80 to a
 * point on second's bone D_8008FB84, facing the eye of view viewIdx. Once the beam is longer than
 * fadeStart, its width ripples along a sine wave. Kills the effect when either actor is gone.
 */
Gfx *func_151B5328(Gfx *gdl, BeamEffect *effect, s16 viewIdx) {
    BeamData *data;
    u8 failed;
    u8 flag;
    s32 i;
    Vtx *vtx;
    Vec3f *eye;
    f32 lenSq;
    f32 perpScale;
    Vec3f perp; // half-width offset, perpendicular to the beam and the view
    f32 scale;
    Vec3f start;
    Vec3f end;
    Vec3f dir;
    f32 viewX, viewY, viewZ; // eye to beam midpoint
    Vec3f cur;
    f32 texT;
    f32 count; // matching: never read, but IDO keeps its accumulation
    f32 length;
    f32 fade;
    f32 stepX, stepY, stepZ;
    f32 angle;
    f32 mag;
    f32 wave;

    failed = 0;
    data = &effect->data;
    if (data->first->interaction_state == 0 || data->first->unique_id != data->firstId) {
        failed = 1;
    }
    if (data->second->interaction_state == 0 || data->second->unique_id != data->secondId) {
        failed = 1;
    }
    if (!failed && data->first->unk1D4 != NULL && data->second->unk1D4 != NULL) {
        func_15143134(D_800AA400, &start, &((BoneMtx *)data->first->unk1D4)[D_8008FB80]);
        func_15143134(D_800AA40C, &end, &((BoneMtx *)data->second->unk1D4)[D_8008FB84]);
        flag = 1;
        vtx = data->vertices[D_800BE9C0];
        eye = (Vec3f *)&D_800DBFF0[viewIdx].unk2F8;
        gdl = func_15142B7C(gdl, 0x200005, 0x60600);
        gdl = func_15142E24(gdl, D_80090F68, 0, 0, 0, 0, 0x56, 0, 0, &flag, 3);
        gdl = func_15142C10(gdl, 0xFF, 0xFF, 0xFF, 0xFF, &flag);
        gdl = func_15142CF0(gdl, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF, &flag);
        gdl = func_1513F4E4(gdl, 0x2E, &flag);
        gdl = func_15142FBC(gdl, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[5][0] | D_800A4AC8[5][1], &flag);
        dir.x = end.x - start.x;
        dir.y = end.y - start.y;
        dir.z = end.z - start.z;
        length = func_15143E64(&dir);
        viewX = dir.x * 0.5f + start.x - eye->x;
        viewY = dir.y * 0.5f + start.y - eye->y;
        viewZ = dir.z * 0.5f + start.z - eye->z;
        perp.x = dir.y * viewZ - viewY * dir.z;
        perp.y = dir.z * viewX - viewZ * dir.x;
        perp.z = dir.x * viewY - viewX * dir.y;
        lenSq = perp.x * perp.x + perp.y * perp.y + perp.z * perp.z;
        if (lenSq == 0.0f) {
            perp.x = 0.0f;
            perp.y = 0.0f;
            perp.z = 0.0f;
        } else {
            mag = sqrtf(lenSq);
            perpScale = data->width / mag;
            perp.x *= perpScale;
            perp.y *= perpScale;
            perp.z *= perpScale;
        }
        stepX = dir.x * 0.1f;
        stepY = dir.y * 0.1f;
        stepZ = dir.z * 0.1f;
        if (length < data->fadeStart) {
            fade = 0.0f;
        } else if (length < data->fadeEnd) {
            fade = (length - data->fadeStart) * data->fadeScale * data->fadeMax;
        } else {
            fade = data->fadeMax;
        }
        for (i = 0, cur = start, angle = 0.0f, texT = 0.0f, count = 0.0f; i < 10; i++) {
            wave = sinf(angle);
            scale = 1.0f - wave * fade;
            vtx[0].v.ob[0] = perp.x * scale + cur.x;
            vtx[0].v.ob[1] = perp.y * scale + cur.y;
            vtx[0].v.ob[2] = perp.z * scale + cur.z;
            vtx[0].v.cn[0] = 0xFF;
            vtx[0].v.cn[1] = 0;
            vtx[0].v.cn[2] = 0;
            vtx[0].v.cn[3] = 0xFF;
            vtx[0].v.tc[0] = 0;
            vtx[0].v.tc[1] = texT;
            vtx[1].v.ob[0] = cur.x - perp.x * scale;
            vtx[1].v.ob[1] = cur.y - perp.y * scale;
            vtx[1].v.ob[2] = cur.z - perp.z * scale;
            vtx[1].v.cn[0] = 0xFF;
            vtx[1].v.cn[1] = 0;
            vtx[1].v.cn[2] = 0;
            vtx[1].v.cn[3] = 0xFF;
            vtx[1].v.tc[0] = 0x7C0;
            vtx[1].v.tc[1] = texT;
            vtx += 2;
            wave = sinf(angle + 0.31415927f);
            scale = 1.0f - wave * fade;
            vtx[0].v.ob[0] = cur.x + stepX - perp.x * scale;
            vtx[0].v.ob[1] = cur.y + stepY - perp.y * scale;
            vtx[0].v.ob[2] = cur.z + stepZ - perp.z * scale;
            vtx[0].v.cn[0] = 0xFF;
            vtx[0].v.cn[1] = 0;
            vtx[0].v.cn[2] = 0;
            vtx[0].v.cn[3] = 0xFF;
            vtx[0].v.tc[0] = 0x7C0;
            vtx[0].v.tc[1] = texT + 204.8f;
            vtx[1].v.ob[0] = cur.x + stepX + perp.x * scale;
            vtx[1].v.ob[1] = cur.y + stepY + perp.y * scale;
            vtx[1].v.ob[2] = cur.z + stepZ + perp.z * scale;
            vtx[1].v.cn[0] = 0xFF;
            vtx[1].v.cn[1] = 0;
            vtx[1].v.cn[2] = 0;
            vtx[1].v.cn[3] = 0xFF;
            vtx[1].v.tc[0] = 0;
            vtx[1].v.tc[1] = texT + 204.8f;
            vtx += 2;
            gSPVertex(gdl++, vtx - 4, 4, 0);
            gSP1Triangle(gdl++, 0, 1, 2, 0);
            gSP1Triangle(gdl++, 0, 2, 3, 0);
            cur.x += stepX;
            cur.y += stepY;
            cur.z += stepZ;
            angle += 0.31415927f;
            texT += 204.8f;
            count += 0.1f;
        }
    }
    if (failed) {
        effect->state = -1;
    }
    return gdl;
}

/* Effect message handler: follows actor slot swaps (message 0x2D) and dies with either actor (message 0). */
void func_151B5A9C(BeamEffect *effect, void *msg, u8 type) {
    BeamData *data = &effect->data;

    if (type == 0x2D) {
        ActorSwap *swap = msg;

        if (data->first == swap->a) {
            data->first = swap->b;
            data->firstId = swap->bId;
        } else if (data->first == swap->b) {
            data->first = swap->a;
            data->firstId = swap->aId;
        }

        if (data->second == swap->a) {
            data->second = swap->b;
            data->secondId = swap->bId;
        } else if (data->second == swap->b) {
            data->second = swap->a;
            data->secondId = swap->aId;
        }
    } else if (type == 0) {
        ActorGone *gone = msg;
        struct127 *actor = gone->actor;

        if (actor == data->first || actor == data->second || gone->id == data->firstId || gone->id == data->secondId) {
            func_1516972C((struct102 *)effect);
        }
    }
}
