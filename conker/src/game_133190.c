#include <ultra64.h>
/* func_150ADA20 returns a full 32-bit value (asm: dsra32 v0); the shared header narrows it to u8 */
#define func_150ADA20 func_150ADA20_u8
#include "functions.h"
#undef func_150ADA20
u32 func_150ADA20(void);
#include "variables.h"

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { u8 r, g, b, a; } ColorRGBA;

/* Parameters of a lightning bolt: a cubic Bezier from start to end, split into jittered segments. */
typedef struct BoltDesc {
    /* 0x00 */ s16 life;
    /* 0x02 */ u8 flags;          /* bit 0: life counts down */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ Vec3f ctrl[4];     /* Bezier control points; ctrl[0] and ctrl[3] are the end points */
    /* 0x34 */ s8 updateKind;     /* index into D_80088C10, -1: none */
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ s32 segments;
    /* 0x3C */ f32 jitter;        /* max sideways offset of each joint */
    /* 0x40 */ f32 width;         /* half width of the drawn strip */
    /* 0x44 */ s16 rejitterBase;  /* frames between re-jitters: base + rand % (range + 1) */
    /* 0x46 */ s16 rejitterRange;
    /* 0x48 */ ColorRGBA color;
    /* 0x4C */ u8 kind;           /* index into D_80088C18 / D_80088C28 */
    /* 0x4D */ u8 pad4D[3];
} BoltDesc; /* size 0x50 */

/* One point of the Bezier sampled at parameter t, with a frame for the sideways jitter. */
typedef struct BoltSample {
    /* 0x00 */ f32 t;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f tangent;
    /* 0x1C */ Vec3f normal;
    /* 0x28 */ Vec3f binormal;
} BoltSample; /* size 0x34 */

typedef struct BoltFlare {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ Vec3f pos;
} BoltFlare;

/*
 * A bolt object. It is followed in memory by desc.segments BoltSamples (func_151061E0),
 * desc.segments + 2 jittered joint points (func_151061EC) and the per-kind user data.
 */
typedef struct BoltObj {
    /* 0x00 */ u8 pad0[0x10];      /* object header */
    /* 0x10 */ BoltDesc desc;
    /* 0x60 */ void *userData;
    /* 0x64 */ f32 step;           /* Bezier parameter step, 1 / (segments + 1) */
    /* 0x68 */ s16 rejitterTimer;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ BoltFlare *flares[2];
    /* 0x74 */ u8 vtxBuf[0x10];
    /* 0x84 */ s32 vtxBufSize;
} BoltObj; /* size 0x88 */

/* Spawn parameters for each bolt of a chain; "base + rand % (range + 1)" or "base + randf * range". */
typedef struct BoltChainConfig {
    /* 0x00 */ f32 widthBase;
    /* 0x04 */ f32 widthRange;
    /* 0x08 */ f32 jitter;
    /* 0x0C */ f32 bendBase;       /* distance of the inner control points from the ends */
    /* 0x10 */ f32 bendRange;
    /* 0x14 */ s16 bendSpread;     /* pitch range of the inner control points */
    /* 0x16 */ s16 rejitterBase;
    /* 0x18 */ s16 rejitterRange;
    /* 0x1A */ s16 lifeBase;
    /* 0x1C */ s16 lifeRange;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 segmentsBase;
    /* 0x24 */ s32 segmentsRange;
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 alphaBase;
    /* 0x2C */ u8 alphaRange;
} BoltChainConfig; /* size 0x30 */

/* A chain of bolts along a randomly spaced line of points, one bolt (link) at a time. */
typedef struct BoltChain {
    /* 0x00 */ s32 count;          /* number of points */
    /* 0x04 */ s32 index;          /* point where the next link starts */
    /* 0x08 */ BoltObj *link;      /* the live link, NULL once it is gone */
    /* 0x0C */ Vec3f *points;
    /* 0x10 */ BoltChainConfig config;
    /* 0x40 */ u8 kind;            /* index into D_80088C38 */
    /* 0x41 */ u8 pad41[3];
} BoltChain; /* size 0x44 */

typedef struct BoltChainObj {
    /* 0x00 */ struct260 base;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ BoltChain chain;
    /* 0x6C */ u8 pad6C[4];
    /* 0x70 */ Vec3f points[1];    /* chain.count points */
} BoltChainObj;

/* Spawn descriptor for func_15130280 (an end flare). */
typedef struct FlareSpawn {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 life;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 color0[4];
    /* 0x18 */ u8 color1[4];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ Vec3f pos;
    /* 0x3C */ Vec3f unk3C;
    /* 0x48 */ Vec3f vel;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ u32 flags;          /* 0x40, 0x80: random flips */
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 pad67;
    /* 0x68 */ s16 unk68;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ f32 unk6C;
} FlareSpawn;

struct Vec15107A20 { f32 x; f32 y; f32 z; };
struct Vec15106F98 { f32 x; f32 y; f32 z; };
struct S15106F98Points { struct Vec15106F98 unk0[4]; };
struct S15106F98Payload {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    struct Vec15106F98 *unkC;
    struct S15106F98Points unk10;
    u8 unk40;
    u8 pad41[3];
};

struct S15107C1CWord {
    s32 unk0;
};

struct S15107C1CWords3 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
};

struct S15107C1CBytes4 {
    u8 bytes[4];
};

struct S15107C1CPayload {
    struct127 *unk0;
    u8 unk4;
    u8 pad5[3];
    struct S15107C1CWords3 unk8;
    u8 unk14;
    u8 pad15[3];
};

struct S15107C1CTrace {
    u8 pad0[8];
    struct Vec15107A20 unk8;
    u8 pad14[0x59 - 0x14];
    u8 unk59;
    u8 pad5A[0x64 - 0x5A];
};

struct S15107C1CInit {
    s16 unk0;
    s8 unk2;
    u8 pad3;
    struct Vec15107A20 unk4;
    struct Vec15107A20 unk10;
    struct Vec15107A20 unk1C;
    struct Vec15107A20 unk28;
    s8 unk34;
    u8 pad35[3];
    s32 unk38;
    f32 unk3C;
    f32 unk40;
    s16 unk44;
    s16 unk46;
    struct S15107C1CWord unk48;
    s8 unk4C;
    u8 pad4D[3];
};

struct S15107C1CCreated {
    u8 pad0[0x60];
    void *unk60;
};

void func_151070F8(BoltChainObj *obj, s16 lo, s16 hi, f32 spread);
void func_15107A20(struct127 *arg0, s32 arg1, s32 arg2, struct Vec15107A20 *arg3, struct Vec15107A20 *arg4);
void func_15107AE0(struct Vec15107A20 *arg0, struct Vec15107A20 *arg1, struct Vec15107A20 *arg2, struct Vec15107A20 *arg3);
void func_15107F54(s32 *arg0, u8 arg1, void *arg2, s32 arg3);
void func_15081690(struct127 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, void *arg7, f32 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, s32 argE);
BoltObj *func_15105CE0(BoltDesc *desc, s32 extraSize, u8 arg2, s32 arg3);
void func_15106214(BoltObj *bolt);
void func_15106610(BoltObj *bolt);
BoltSample *func_151061E0(BoltObj *bolt);
Vec3f *func_151061EC(BoltObj *bolt);

extern s32 (*D_80088C10[])(BoltObj *bolt, u8 *moved);

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
extern void *func_15130280(void *, u8, s32, s32, u8, s32);

/* Allocates a bolt for desc with extraSize bytes of user data, samples and jitters it, and spawns its end flares. */
BoltObj *func_15105CE0(BoltDesc *desc, s32 extraSize, u8 arg2, s32 arg3) {
    BoltObj *bolt;
    s32 segments;
    u8 i;
    s32 points;

    segments = desc->segments;
    points = desc->segments + 2;
    bolt = func_15167A68(0x40, arg3, segments * sizeof(BoltSample) + points * sizeof(Vec3f) + extraSize + sizeof(BoltObj), 1, arg2, 1);
    if (bolt == NULL) {
        return NULL;
    }
    memcpy(&bolt->desc, desc, sizeof(BoltDesc));
    bolt->userData = (u8 *)bolt + segments * sizeof(BoltSample) + points * sizeof(Vec3f) + sizeof(BoltObj);
    bolt->step = 1.0f / (desc->segments + 1);
    bolt->rejitterTimer = func_150ADA20() % (desc->rejitterRange + 1) + desc->rejitterBase;
    func_15106214(bolt);
    func_15106610(bolt);
    bolt->flares[0] = NULL;
    bolt->flares[1] = NULL;
    bzero(bolt->vtxBuf, sizeof(bolt->vtxBuf));
    /* three vertices per joint point */
    bolt->vtxBufSize = (bolt->desc.segments + 2) * 3 * sizeof(Vtx);
    {
    FlareSpawn spawn;

    spawn.unk1D = 0x2B;
    spawn.unk8 = 0x4404;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.life = 300;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.color0[0] = 0xFF;
    spawn.color0[1] = 0xFF;
    spawn.color0[2] = 0xFF;
    spawn.color0[3] = 0xFF;
    spawn.color1[0] = desc->color.r;
    spawn.color1[1] = desc->color.g;
    spawn.color1[2] = desc->color.b;
    spawn.color1[3] = desc->color.a;
    spawn.unk1C = 0xFF;
    spawn.unk3C = *(Vec3f *)&D_800A5480;
    spawn.vel = *(Vec3f *)&D_800A5480;
    spawn.unk1E = 1;
    spawn.unk20 = 0xFF;
    spawn.unk22 = 1;
    spawn.flags = 0xC000;
    spawn.unk60 = 6;
    spawn.unk61 = 6;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk5C = 0;
    spawn.unk66 = 0xFF;
    spawn.unk68 = 0;
    spawn.unk54 = 0.0f;
    spawn.unk24 = 1.0f;
    spawn.unk6C = 1000000.0f;
    spawn.unk28 = spawn.unk2C = desc->width * 0.07692307979f * 200.0f;
    for (i = 0; i < 2; i++) {
        spawn.pos = desc->ctrl[(i == 0) ? 0 : 3];
        spawn.flags = (func_150ADA20() ? 0x40 : 0) | (func_150ADA20() ? 0x80 : 0);
        bolt->flares[i] = func_15130280(&spawn, 0, 0, 0, arg2, arg3);
    }
    }
    return bolt;
}

/* Per-frame update: follow the owner, count down life, re-jitter on a timer, keep the flares on the end points. */
void func_1510608C(BoltObj *bolt) {
    s32 (*update)(BoltObj *, u8 *);
    u8 moved;
    BoltFlare *flare;

    moved = FALSE;
    if (bolt->desc.updateKind != -1) {
        update = D_80088C10[bolt->desc.updateKind];
        if (update != NULL && !update(bolt, &moved)) {
            func_1516972C((struct102 *)bolt);
        }
    }

    if (bolt->desc.flags & 1) {
        bolt->desc.life -= D_800BE9E4;
        if (bolt->desc.life < 0) {
            func_1516972C((struct102 *)bolt);
        }
    }

    if (moved) {
        func_15106214(bolt);
    }

    bolt->rejitterTimer -= D_800BE9E4;
    if (bolt->rejitterTimer < 0) {
        func_15106610(bolt);
        bolt->rejitterTimer = (func_150ADA20() % (u32)(bolt->desc.rejitterRange + 1)) + bolt->desc.rejitterBase;
    }

    flare = bolt->flares[0];
    if (flare != NULL) {
        flare->pos = bolt->desc.ctrl[0];
    }

    flare = bolt->flares[1];
    if (flare != NULL) {
        flare->pos = bolt->desc.ctrl[3];
    }
}

/* The Bezier samples that follow the bolt object. */
BoltSample *func_151061E0(BoltObj *bolt) {
    return (BoltSample *)(bolt + 1);
}

/* The jittered joint points that follow the samples. */
Vec3f *func_151061EC(BoltObj *bolt) {
    s32 segments = bolt->desc.segments;
    return (Vec3f *)((BoltSample *)(bolt + 1) + segments);
}

f32 func_151064B4(f32 t);
f32 func_151064DC(f32 t);
f32 func_15106510(f32 t);
f32 func_15106540(f32 t);
f32 func_15106558(f32 t);
f32 func_15106584(f32 t);
f32 func_151065BC(f32 t);
f32 func_151065EC(f32 t);
s32 func_15146078(Vec3f *dir, Vec3f *perp0, Vec3f *perp1);

/* Samples the Bezier at t = step, 2 * step, ...: position, tangent and a frame around the tangent. */
void func_15106214(BoltObj *bolt) {
    BoltSample *samples;
    f32 step;
    BoltSample *out;
    s32 i;
    f32 t;
    f32 b0;
    f32 b1;
    f32 b2;
    f32 b3;
    f32 d0;
    f32 d1;
    f32 d2;
    f32 d3;

    samples = func_151061E0(bolt);
    step = bolt->step;
    t = step;
    for (i = 0; i < bolt->desc.segments; i++) {
        out = &samples[i];
        out->t = t;
        b0 = func_151064B4(t);
        b1 = func_151064DC(t);
        b2 = func_15106510(t);
        b3 = func_15106540(t);
        d0 = func_15106558(t);
        d1 = func_15106584(t);
        d2 = func_151065BC(t);
        d3 = func_151065EC(t);
        out->pos.x = bolt->desc.ctrl[0].x * b0 + bolt->desc.ctrl[1].x * b1 + bolt->desc.ctrl[2].x * b2 + bolt->desc.ctrl[3].x * b3;
        out->pos.y = bolt->desc.ctrl[0].y * b0 + bolt->desc.ctrl[1].y * b1 + bolt->desc.ctrl[2].y * b2 + bolt->desc.ctrl[3].y * b3;
        out->pos.z = bolt->desc.ctrl[0].z * b0 + bolt->desc.ctrl[1].z * b1 + bolt->desc.ctrl[2].z * b2 + bolt->desc.ctrl[3].z * b3;
        out->tangent.x = bolt->desc.ctrl[0].x * d0 + bolt->desc.ctrl[1].x * d1 + bolt->desc.ctrl[2].x * d2 + bolt->desc.ctrl[3].x * d3;
        out->tangent.y = bolt->desc.ctrl[0].y * d0 + bolt->desc.ctrl[1].y * d1 + bolt->desc.ctrl[2].y * d2 + bolt->desc.ctrl[3].y * d3;
        out->tangent.z = bolt->desc.ctrl[0].z * d0 + bolt->desc.ctrl[1].z * d1 + bolt->desc.ctrl[2].z * d2 + bolt->desc.ctrl[3].z * d3;
        if (func_15146078(&out->tangent, &out->normal, &out->binormal) == 0) {
            out->normal.x = 0.0f;
            out->normal.y = 0.0f;
            out->normal.z = 0.0f;
            out->binormal.x = 0.0f;
            out->binormal.y = 0.0f;
            out->binormal.z = 0.0f;
        }
        t += step;
    }
}

/* Cubic Bernstein basis B0(t) = (1 - t)^3. */
f32 func_151064B4(f32 t) {
    f32 u = 1.0f - t;
    return u * u * u;
}

/* B1(t) = 3t(1 - t)^2. */
f32 func_151064DC(f32 t) {
    f32 u = 1.0f - t;
    return 3.0f * t * u * u;
}

/* B2(t) = 3t^2(1 - t). */
f32 func_15106510(f32 t) {
    return 3.0f * t * t * (1.0f - t);
}

/* B3(t) = t^3. */
f32 func_15106540(f32 t) {
    return t * t * t;
}

/* B0'(t) = -3(1 - t)^2. */
f32 func_15106558(f32 t) {
    f32 u = 1.0f - t;
    return -3.0f * u * u;
}

/* B1'(t) = 9t^2 - 12t + 3. */
f32 func_15106584(f32 t) {
    return 9.0f * t * t - 12.0f * t + 3.0f;
}

/* B2'(t) = -9t^2 + 6t. */
f32 func_151065BC(f32 t) {
    return -9.0f * t * t + 6.0f * t;
}

/* B3'(t) = 3t^2. */
f32 func_151065EC(f32 t) {
    return 3.0f * t * t;
}

void func_15143874(s16 angle, f32 radius, f32 *x, f32 *y);

/* Rebuilds the joint points: the fixed end points plus each sample pushed a random distance sideways. */
void func_15106610(BoltObj *bolt) {
    BoltSample *samples;
    Vec3f *points;
    s32 i;
    s32 j;

    samples = func_151061E0(bolt);
    points = func_151061EC(bolt);
    points[0] = bolt->desc.ctrl[0];
    points[bolt->desc.segments + 1] = bolt->desc.ctrl[3];
    for (i = 1, j = 0; i < bolt->desc.segments + 1; i++, j++) {
        Vec3f *point = &points[i];
        BoltSample *sample = &samples[j];
        f32 offset1;
        f32 offset0;
        func_15143874((s16)(func_150ADA20() & 0xFF), func_150ADA68() * bolt->desc.jitter, &offset0, &offset1);
        point->x = sample->normal.x * offset0 + sample->binormal.x * offset1 + sample->pos.x;
        point->y = sample->normal.y * offset0 + sample->binormal.y * offset1 + sample->pos.y;
        point->z = sample->normal.z * offset0 + sample->binormal.z * offset1 + sample->pos.z;
    }
}

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
void func_151D5D60(void *, s16, s32, void **, u8 *);
Vec3f *func_15144B34(s32 view);
Gfx *func_15142E24(Gfx *, s32, s32, s32, s32, s32, s32, u8, s32, u8 *, s32);
Gfx *func_15142C10(Gfx *, s32, s32, s32, s32, u8 *);
Gfx *func_15142B7C(Gfx *, s32, s32);
Gfx *func_1513F4E4(Gfx *, u8, u8 *);
Gfx *func_15142FBC(Gfx *, s32, s32, u8 *);
extern u8 D_80090D70[];
extern s32 D_800A4AC8[][2];
extern s32 D_800D2C9C;

Gfx *func_151067B8(Gfx *gdl, BoltObj *bolt, s16 view) {
    Vec3f *camera;
    BoltObj *curve = bolt; // matching: a second copy of the bolt pointer steers IDO's register choice
    Vtx_t *vtx;
    Vec3f delta;
    Vec3f toCamera;
    Vec3f normal;
    f32 lengthSquared;
    u8 flag;
    Vec3f *points;
    s32 i;
    Vec3f previous;

    if (0) { gdl = gdl; } // matching: dead IR resets uopt alias state so the first strip reloads vtx
    func_151D5D60(bolt->vtxBuf, view, bolt->vtxBufSize, (void **)&vtx, NULL);
    if (vtx == NULL) {
        return gdl;
    }
    camera = func_15144B34(view);
    flag = 1;
    points = func_151061EC(bolt);
    gdl = func_15142E24(gdl, (s32)D_80090D70, 0, 0, 0, 0, 0x2C, 0, 0, &flag, 3);
    gdl = func_15142C10(gdl, bolt->desc.color.r, bolt->desc.color.g, bolt->desc.color.b, bolt->desc.color.a, &flag);
    gdl = func_15142B7C(gdl, 0x200005, 0x1F0600);
    gdl = func_1513F4E4(gdl, 0x44, &flag);
    gdl = func_15142FBC(gdl, D_800D2C9C | 0x80000 | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[1][0] | D_800A4AC8[1][1], &flag);
    delta.x = points[1].x - points[0].x;
    delta.y = points[1].y - points[0].y;
    delta.z = points[1].z - points[0].z;
    toCamera.x = points[0].x - camera->x;
    toCamera.y = points[0].y - camera->y;
    toCamera.z = points[0].z - camera->z;
    normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
    normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
    normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
    lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
    if (lengthSquared > 0.0001f) {
        f32 scale = bolt->desc.width / sqrtf(lengthSquared);

        normal.x *= scale;
        normal.y *= scale;
        normal.z *= scale;
    } else {
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
    }
    vtx->ob[0] = points[0].x + normal.x;
    vtx->ob[1] = points[0].y + normal.y;
    vtx->ob[2] = points[0].z + normal.z;
    vtx->tc[0] = 0;
    vtx->tc[1] = 0;
    vtx++;
    vtx->ob[0] = points[0].x;
    vtx->ob[1] = points[0].y;
    vtx->ob[2] = points[0].z;
    vtx->tc[0] = 0x200;
    vtx->tc[1] = 0;
    vtx++;
    vtx->ob[0] = points[0].x - normal.x;
    vtx->ob[1] = points[0].y - normal.y;
    vtx->ob[2] = points[0].z - normal.z;
    vtx->tc[0] = 0x400;
    vtx->tc[1] = 0;
    vtx++;
    previous = points[0];
    for (i = 1; i < curve->desc.segments + 2; i++) {
        Vec3f *point = &points[i];

        delta.x = previous.x - point->x;
        delta.y = previous.y - point->y;
        delta.z = previous.z - point->z;
        toCamera.x = point->x - camera->x;
        toCamera.y = point->y - camera->y;
        toCamera.z = point->z - camera->z;
        normal.x = delta.y * toCamera.z - toCamera.y * delta.z;
        normal.y = delta.z * toCamera.x - toCamera.z * delta.x;
        normal.z = delta.x * toCamera.y - toCamera.x * delta.y;
        lengthSquared = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
        if (lengthSquared > 0.0001f) {
            f32 scale = bolt->desc.width / sqrtf(lengthSquared);

            normal.x *= scale;
            normal.y *= scale;
            normal.z *= scale;
        } else {
            normal.x = 0.0f;
            normal.y = 0.0f;
            normal.z = 0.0f;
        }
        vtx->ob[0] = point->x - normal.x;
        vtx->ob[1] = point->y - normal.y;
        vtx->ob[2] = point->z - normal.z;
        vtx->tc[0] = 0;
        vtx->tc[1] = 0;
        vtx++;
        vtx->ob[0] = point->x;
        vtx->ob[1] = point->y;
        vtx->ob[2] = point->z;
        vtx->tc[0] = 0x200;
        vtx->tc[1] = 0;
        vtx++;
        vtx->ob[0] = point->x + normal.x;
        vtx->ob[1] = point->y + normal.y;
        vtx->ob[2] = point->z + normal.z;
        vtx->tc[0] = 0x400;
        vtx->tc[1] = 0;
        vtx++;
        gSPVertex(gdl++, vtx - 6, 6, 0);
        gSP1Triangle(gdl++, 0, 1, 3, 0);
        gSP1Triangle(gdl++, 1, 4, 3, 0);
        gSP1Triangle(gdl++, 1, 2, 5, 0);
        gSP1Triangle(gdl++, 1, 5, 4, 0);
        previous = *point;
    }
    return gdl;
}

extern void (*D_80088C18[])(BoltObj *bolt);
void func_151D5E30(void *arg0);

/* Destroys the bolt's resources: the kind's cleanup callback, both end flares and the vertex buffer. */
void func_15106E78(BoltObj *bolt) {
    void (*cleanup)(BoltObj *);
    BoltFlare *flare;

    cleanup = D_80088C18[bolt->desc.kind];
    if (cleanup != NULL) {
        cleanup(bolt);
    }
    flare = bolt->flares[0];
    if (flare != NULL) {
        func_1516972C((struct102 *)flare);
    }
    flare = bolt->flares[1];
    if (flare != NULL) {
        func_1516972C((struct102 *)flare);
    }
    func_151D5E30(bolt->vtxBuf);
}

void func_15106EF8(BoltObj *bolt) {
    func_15106E78(bolt);
    func_15169804((struct102 *)bolt);
}

void func_15169824(struct102 *arg0);

void func_15106F24(BoltObj *bolt) {
    func_15106E78(bolt);
    func_15169824((struct102 *)bolt);
}

extern void (*D_80088C28[])(BoltObj *bolt, s32 arg1, u8 arg2);
extern void (*D_80088C38[])(BoltChainObj *obj, s32 arg1, u8 arg2);

/* Dispatches to the bolt kind's handler in D_80088C28. */
void func_15106F50(BoltObj *bolt, s32 arg1, u8 arg2) {
    void (*fn)(BoltObj *, s32, u8) = D_80088C28[bolt->desc.kind];
    if (fn != NULL) {
        fn(bolt, arg1, arg2);
    }
}

/* Creates a bolt chain from start to end with (1 << depth) + 1 randomly spaced points. */
BoltChainObj *func_15106F98(Vec3f *start, Vec3f *end, s32 depth, BoltChainConfig *config, f32 spread, u8 kind, u8 arg6, s32 arg7) {
    BoltChainObj *obj;
    BoltChain chain;
    BoltChain *data;

    chain.count = (1 << depth) + 1;
    chain.link = NULL;
    chain.index = 0;
    chain.config = *config;
    chain.kind = kind;

    obj = (BoltChainObj *)func_15149130(0, -1, 0x3E, -1, 0, 0x30, (struct37 *)(chain.count * sizeof(Vec3f) + 0x48), arg6, arg7);
    if (obj != NULL) {
        data = &obj->chain;
        memcpy(data, &chain, sizeof(BoltChain));
        data->points = (Vec3f *)((u8 *)data + 0x48); // matching: = obj->points, but computed from data
        data->points[0] = *start;
        data->points[chain.count - 1] = *end;
        func_151070F8(obj, 0, (s16)(chain.count - 1), spread);
    }
    return obj;
}

/* Recursively places the points between lo and hi on the line, each midpoint randomly within +-spread of halfway. */
void func_151070F8(BoltChainObj *obj, s16 lo, s16 hi, f32 spread) {
    BoltChain *chain = &obj->chain;
    Vec3f *a;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 t;
    s16 mid;

    if (hi - lo < 2) {
        return;
    }
    mid = lo + ((hi - lo) >> 1);
    a = &chain->points[lo];
    dx = chain->points[hi].x - chain->points[lo].x;
    dy = chain->points[hi].y - chain->points[lo].y;
    dz = chain->points[hi].z - chain->points[lo].z;
    t = func_150ADA68() * (spread + spread) + (0.5f - spread);
    chain->points[mid].x = a->x + dx * t;
    chain->points[mid].y = a->y + dy * t;
    chain->points[mid].z = a->z + dz * t;
    func_151070F8(obj, lo, mid, spread);
    func_151070F8(obj, mid, hi, spread);
}

void func_15143794(s32, s32, f32, f32 *);

/* Chain update: once the current link is gone, spawns a bolt from points[index] to points[index + 1]; ends the chain after the last. */
void func_151072BC(BoltChainObj *obj) {
    BoltChain *chain;
    BoltDesc desc;
    BoltChainObj *owner;
    Vec3f offset;
    BoltChainConfig *config;

    chain = &obj->chain;
    if (chain->link == NULL) {
        if (chain->index >= chain->count - 1) {
            obj->base.unkE = -1;
            obj->base.unkD |= 1;
            return;
        }
        owner = obj;
        desc.flags = 1;
        desc.updateKind = -1;
        config = &chain->config;
        desc.width = func_150ADA68() * config->widthRange + config->widthBase;
        desc.jitter = config->jitter;
        desc.rejitterBase = config->rejitterBase;
        desc.rejitterRange = config->rejitterRange;
        desc.ctrl[0] = chain->points[chain->index];
        desc.ctrl[3] = chain->points[chain->index + 1];
        func_15143794((s16)(func_150ADA20() & 0xFF),
            (s16)(func_150ADA20() % (u32)(config->bendSpread + 1) - (config->bendSpread >> 1) - 0x40),
            func_150ADA68() * config->bendRange + config->bendBase, &offset.x);
        desc.ctrl[1].x = desc.ctrl[0].x + offset.x;
        desc.ctrl[1].y = desc.ctrl[0].y + offset.y;
        desc.ctrl[1].z = desc.ctrl[0].z + offset.z;
        func_15143794((s16)(func_150ADA20() & 0xFF),
            (s16)(func_150ADA20() % (u32)(config->bendSpread + 1) - (config->bendSpread >> 1) - 0x40),
            func_150ADA68() * config->bendRange + config->bendBase, &offset.x);
        desc.ctrl[2].x = desc.ctrl[3].x + offset.x;
        desc.ctrl[2].y = desc.ctrl[3].y + offset.y;
        desc.ctrl[2].z = desc.ctrl[3].z + offset.z;
        desc.color.r = config->r;
        desc.color.g = config->g;
        desc.color.b = config->b;
        desc.life = func_150ADA20() % (u32)(config->lifeRange + 1) + config->lifeBase;
        desc.color.a = func_150ADA20() % (u32)(config->alphaRange + 1) + config->alphaBase;
        desc.kind = 1;
        desc.segments = func_150ADA20() % (u32)(config->segmentsRange + 1) + config->segmentsBase;
        chain->link = func_15105CE0(&desc, sizeof(owner), obj->base.pad0[0xC], obj->base.pad0[1]);
        if (chain->link != NULL) {
            memcpy(chain->link->userData, &owner, sizeof(owner));
        }
        chain->index++;
    }
}

/* Kind-1 (chain link) cleanup: tells the owning chain that its link is gone. */
void func_15107604(BoltObj *bolt) {
    BoltChainObj **owner = bolt->userData;
    BoltChain *chain = &(*owner)->chain;
    chain->link = NULL;
}

/* Kills the chain's live link, if any. */
void func_1510761C(BoltChainObj *obj) {
    // matching: volatile makes IDO load the link twice, as the original does
    if (*(BoltObj *volatile *)&obj->chain.link != NULL) {
        func_1516972C(*(struct102 *volatile *)&obj->chain.link);
    }
}

void func_1510764C(BoltChainObj *obj) {
    func_1510761C(obj);
    func_1514933C(&obj->base);
}

void func_15149368(struct260 *arg0);

void func_15107678(BoltChainObj *obj) {
    func_1510761C(obj);
    func_15149368(&obj->base);
}

/* Dispatches to the chain kind's handler in D_80088C38. */
void func_151076A4(BoltChainObj *obj, s32 arg1, u8 arg2) {
    if (D_80088C38[obj->chain.kind] != NULL) {
        D_80088C38[*(volatile u8 *)&obj->chain.kind](obj, arg1, arg2); // matching: the original reloads the kind
    }
}

struct S15107700Payload {
    struct127 *unk0;
    u8 unk4;
    u8 pad5;
    s16 unk6;
    s16 unk8;
    u8 padA[2];
};

struct S15107C1CCreated *func_15107700(struct127 *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, f32 arg5, f32 arg6, s16 arg7, s16 arg8, u8 *arg9, u8 argA, s32 argB) {
    struct S15107700Payload payload;
    struct S15107C1CCreated *created;
    struct S15107C1CInit init;
    struct S15107C1CTrace trace;
    struct Vec15107A20 vec;
    s32 flag;
    if (arg0->unk1D4 == NULL) {
        return NULL;
    }

    payload.unk0 = arg0;
    payload.unk4 = arg0->unique_id;
    payload.unk6 = arg1;
    payload.unk8 = arg2;

    if (arg3 == -1) {
        init.unk0 = 0x12C;
    } else {
        init.unk0 = arg3;
    }
    if (arg3 == -1) {
        flag = 0;
    } else {
        flag = 1;
    }
    init.unk2 = flag;
    init.unk34 = 0;
    init.unk38 = arg4;
    init.unk3C = arg5;
    init.unk40 = arg6;
    init.unk44 = arg7;
    init.unk46 = arg8;
    *(struct S15107C1CBytes4 *)&init.unk48 = *(struct S15107C1CBytes4 *)arg9;
    init.unk4C = 2;

    /* block-scoped declaration: its debug scope marker keeps IDO from hoisting &init.unk48 out of this block */
    {
    void func_15107A20(struct127 *arg0, s32 arg1, s32 arg2, struct Vec15107A20 *arg3, struct Vec15107A20 *arg4);
    func_15107A20(arg0, arg1, arg2, &init.unk4, &vec);
    func_15081690(arg0, init.unk4.x, init.unk4.y, init.unk4.z, init.unk4.x - vec.x, init.unk4.y - vec.y, init.unk4.z - vec.z, &trace, 0.0f, 0, 1, 1, -1, 0, 0);

    }
    if (trace.unk59 == 0) {
        return NULL;
    }
    init.unk28 = trace.unk8;

    func_15107AE0(&init.unk4, &init.unk28, &init.unk10, &init.unk1C);
    created = (struct S15107C1CCreated *)func_15105CE0((BoltDesc *)&init, 0xC, argA, argB);
    if (created != NULL) {
        memcpy(created->unk60, &payload, 0xC);
    }
    return created;
}


void func_151078E4(s32 *arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0[0x18], arg0[0x18] + 4, (s32)arg0);
}

struct S15107924 {
    struct127 *unk0;
    u8 unk4;
    u8 unk5;
    s16 unk6;
    s16 unk8;
};

s32 func_15107924(struct127 *arg0, u8 *arg1) {
    struct S15107924 *data;
    struct127 *obj;
    struct Vec15107A20 oldPos;

    data = (struct S15107924 *)arg0->unk60;
    obj = data->unk0;
    if ((obj->interaction_state == 0) || (obj->unique_id != data->unk4)) {
        return 0;
    }
    *arg1 = 0;
    oldPos = *(struct Vec15107A20 *)&arg0->x_position;
    func_15107A20(obj, data->unk6, data->unk8, (struct Vec15107A20 *)&arg0->x_position, NULL);
    if ((oldPos.x == arg0->x_position) && (oldPos.y == arg0->y_position) && (oldPos.z == arg0->z_position)) {
        goto ret1;
    }
    *arg1 = 1;
    func_15107AE0((struct Vec15107A20 *)&arg0->x_position, (struct Vec15107A20 *)&arg0->unk38, (struct Vec15107A20 *)&arg0->y_velocity, (struct Vec15107A20 *)&arg0->old_x_position);
ret1:
    return 1;
}

void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
void func_15143794(s32, s32, f32, f32 *);

void func_15107A20(struct127 *arg0, s32 arg1, s32 arg2, struct Vec15107A20 *arg3, struct Vec15107A20 *arg4) {
    struct Vec15107A20 sp2C;
    f32 sp28;
    f32 sp24;

    func_1515C244(arg0, &sp2C.x, &sp28, &sp24);
    func_15143794((s16)arg1, (s16)arg2, sp28, &arg3->x);
    arg3->y *= arg0->unkF0;
    arg3->x += sp2C.x;
    arg3->y += sp2C.y;
    arg3->z += sp2C.z;
    if (arg4 != NULL) {
        *arg4 = sp2C;
    }
}

void func_15107AE0(struct Vec15107A20 *arg0, struct Vec15107A20 *arg1, struct Vec15107A20 *arg2, struct Vec15107A20 *arg3) {
    f32 *start = (f32 *)arg0;
    f32 *end = (f32 *)arg1;
    f32 *first = (f32 *)arg2;
    f32 *second = (f32 *)arg3;
    f32 dx;
    f32 dy;
    f32 dz;

    dx = end[0] - start[0];
    dy = end[1] - start[1];
    dz = end[2] - start[2];
    first[0] = start[0] + dx * 0.333f;
    first[1] = start[1] + dy * 0.333f;
    first[2] = start[2] + dz * 0.333f;
    second[0] = start[0] + dx * 0.6667f;
    second[1] = start[1] + dy * 0.6667f;
    second[2] = start[2] + dz * 0.6667f;
}

extern void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
extern void func_15143794(s32, s32, f32, f32 *);
extern void func_151C329C(void *, s32, s32);

void func_15107B78(struct127 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    struct { f32 x; f32 y; f32 z; } sp2C;
    struct { f32 x; f32 y; f32 z; } sp20;
    f32 sp1C;
    f32 sp18;

    if (arg0->unk1D4 != NULL) {
        func_1515C244(arg0, &sp20.x, &sp1C, &sp18);
        func_15143794((s16)arg1, (s16)arg2, sp1C, &sp2C.x);
        sp2C.y *= arg0->unkF0;
        sp2C.x += sp20.x;
        sp2C.y += sp20.y;
        sp2C.z += sp20.z;
        func_151C329C(&sp2C, (u8)arg3, arg4);
    }
}

struct S15107C1CCreated *func_15107C1C(struct127 *arg0, u8 arg1, void *arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, f32 arg7, f32 arg8, s16 arg9, s16 argA, u8 *argB, u8 argC, s32 argD) {
    struct S15107C1CPayload payload;
    struct S15107C1CCreated *created;
    struct S15107C1CInit init;
    struct S15107C1CTrace trace;
    struct Vec15107A20 vec;
    s32 flag;
    if (arg0->unk1D4 == NULL) {
        return NULL;
    }

    payload.unk0 = arg0;
    payload.unk4 = arg0->unique_id;
    payload.unk8 = *(struct S15107C1CWords3 *)arg2;
    payload.unk14 = arg1;

    if (arg5 == -1) {
        init.unk0 = 0x12C;
    } else {
        init.unk0 = arg5;
    }
    if (arg5 == -1) {
        flag = 0;
    } else {
        flag = 1;
    }
    init.unk2 = flag;
    init.unk34 = 1;
    init.unk38 = arg6;
    init.unk3C = arg7;
    init.unk40 = arg8;
    init.unk44 = arg9;
    init.unk46 = argA;
    *(struct S15107C1CBytes4 *)&init.unk48 = *(struct S15107C1CBytes4 *)argB;
    init.unk4C = 3;

    func_15107F54((s32 *)arg0, arg1, arg2, (s32)&init.unk4);
    func_15143794(arg3, arg4, 1.0f, &vec.x);
    func_15081690(arg0, init.unk4.x, init.unk4.y, init.unk4.z, vec.x, vec.y, vec.z, &trace, 300.0f, 0, 1, 1, -1, 0, 0);

    if (trace.unk59 == 0) {
        init.unk28.x = init.unk4.x;
        init.unk28.x += vec.x * 300.0f;
        init.unk28.y = init.unk4.y;
        init.unk28.y += vec.y * 300.0f;
        init.unk28.z = init.unk4.z;
        init.unk28.z += vec.z * 300.0f;
    } else {
        init.unk28 = trace.unk8;
    }

    func_15107AE0(&init.unk4, &init.unk28, &init.unk10, &init.unk1C);
    created = (struct S15107C1CCreated *)func_15105CE0((BoltDesc *)&init, 0x18, argC, argD);
    if (created != NULL) {
        memcpy(created->unk60, &payload, 0x18);
    }
    return created;
}

struct S15107E48 {
    struct127 *unk0;
    u8 unk4;
    u8 pad5[3];
    u8 unk8[0xC];
    u8 unk14;
};

s32 func_15107E48(struct127 *arg0, u8 *arg1) {
    struct S15107E48 *data;
    struct127 *obj;
    struct Vec15107A20 oldPos;

    data = (struct S15107E48 *)arg0->unk60;
    obj = data->unk0;
    if ((obj->interaction_state == 0) || (obj->unique_id != data->unk4)) {
        return 0;
    }
    if (obj->unk1D4 == NULL) {
        return 0;
    }
    *arg1 = 0;
    oldPos = *(struct Vec15107A20 *)&arg0->x_position;
    func_15107F54((s32 *)obj, data->unk14, data->unk8, (s32)(struct Vec15107A20 *)&arg0->x_position);
    if ((oldPos.x == arg0->x_position) && (oldPos.y == arg0->y_position) && (oldPos.z == arg0->z_position)) {
        goto ret1;
    }
    *arg1 = 1;
    func_15107AE0((struct Vec15107A20 *)&arg0->x_position, (struct Vec15107A20 *)&arg0->unk38, (struct Vec15107A20 *)&arg0->y_velocity, (struct Vec15107A20 *)&arg0->old_x_position);
ret1:
    return 1;
}

void func_15143134(void *, s32, s32);

void func_15107F54(s32 *arg0, u8 arg1, void *arg2, s32 arg3) {
    func_15143134(arg2, arg3, arg0[0x1D4 / 4] + (arg1 << 6));
}

void func_15169850(s32, u8, s32, s32, s32);

void func_15107F98(s32 *arg0, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, arg0[0x18], arg0[0x18] + 4, (s32)arg0);
}
