#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_151D5D60();

/* Particle spawn descriptor passed to func_15149550. */
typedef struct {
    struct17 pos;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u16 unk2A;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s32 unk30;
    u8 unk34;
    u8 pad35[3];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
} Struct15149550Arg;

s32 func_15149550(Struct15149550Arg *arg0, u8 arg1, u8 arg2, s32 arg3, u8 arg4, s32 arg5);

typedef struct Vec3f1514A6A0 {
    f32 x;
    f32 y;
    f32 z;
} Vec3f1514A6A0;

typedef struct Vtx1514A6A0 {
    s16 x, y, z, flag;
    u8 other[8];
} Vtx1514A6A0;

typedef struct Beam1514A6A0 {
    u8 unknown0[0x2C];
    f32 width;
    f32 progress;
    Vec3f1514A6A0 start;
    Vec3f1514A6A0 end;
    f32 radius;
    u8 unknown50[0x70];
    u8 vertexTemplate[0x40];
    u8 *buffers[1];
} Beam1514A6A0;

/* Builds the four camera-facing vertices of a beam quad from start toward end. */
void *func_1514A6A0(Beam1514A6A0 *beam, s16 cam) {
    Vtx1514A6A0 *vertices;
    void *result;
    f32 distanceSquared;
    Vec3f1514A6A0 tip;
    f32 dx, dy, dz;
    f32 vx, vy, vz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    Vec3f1514A6A0 *eye;
    f32 radiusSquared;
    u8 fresh;

    func_151D5D60(beam->buffers, cam, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(beam->buffers[cam], beam->vertexTemplate, 0x40);
            memcpy(beam->buffers[cam] + 0x40, beam->vertexTemplate, 0x40);
        }
    } else {
        return NULL;
    }
    dx = beam->end.x - beam->start.x;
    dy = beam->end.y - beam->start.y;
    dz = beam->end.z - beam->start.z;
    tip.x = beam->start.x + dx * beam->progress;
    tip.y = beam->start.y + dy * beam->progress;
    tip.z = beam->start.z + dz * beam->progress;
    eye = (Vec3f1514A6A0 *)&D_800DBFF0[cam].unk2F8;
    dx = tip.x - beam->start.x;
    dy = tip.y - beam->start.y;
    dz = tip.z - beam->start.z;
    distanceSquared = dx * dx + dy * dy + dz * dz;
    radiusSquared = beam->radius * beam->radius;
    if (distanceSquared < radiusSquared) {
        f32 factor = 1.0f - sqrtf(distanceSquared) / beam->radius;

        dx += dx * factor;
        dy += dy * factor;
        dz += dz * factor;
    }
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
        f32 scale = beam->width / sqrtf(lengthSquared);

        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = beam->start.x + nx;
    vertices->y = beam->start.y + ny;
    vertices->z = beam->start.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = beam->start.x - nx;
    vertices->y = beam->start.y - ny;
    vertices->z = beam->start.z - nz;
    vertices->flag = 0;
    vertices++;
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
        f32 scale = beam->width / sqrtf(lengthSquared);

        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->x = tip.x + nx;
    vertices->y = tip.y + ny;
    vertices->z = tip.z + nz;
    vertices->flag = 0;
    vertices++;
    vertices->x = tip.x - nx;
    vertices->y = tip.y - ny;
    vertices->z = tip.z - nz;
    vertices->flag = 0;
    return result;
}

/* Spawns `count` particles at random points within `radius` of (x, y, z). */
void func_1514AB5C(f32 x, f32 y, f32 z, f32 radius, s16 count, f32 sizeBase, f32 sizeRange,
                   f32 heightBase, f32 heightRange, s16 lifeBase, s16 lifeRange, u8 arg11) {
    Struct15149550Arg particle;
    u8 angle;
    f32 sin;
    f32 cos;
    f32 dist;
    f32 size;
    f32 height;

    if (count > 0) {
        // matching: both stores on one line (-g3 line numbers steer as1 scheduling)
        particle.unk20 = 0x6231; particle.unk22 = 0x1A4D;
        particle.unk26 = 0;
        particle.unk27 = 0;
        particle.unk28 = 0;
        particle.unk29 = 0xFF;
        particle.unk2A = 1;
        particle.unk2C = 0xFF;
        particle.unk2D = 0xFF;
        particle.unk2F = arg11;
        particle.unk38 = 0.0f;
        particle.unk40 = 0.045f;
        particle.unk44 = 0.7f;
        particle.unk30 = 0;
        particle.unk2E = 0;
        particle.unk34 = 15;
        particle.pos.unk4 = y;

        do {
            // matching: explicit mask; func_150ADA20 really returns a full word
            angle = func_150ADA20() & 0xFF;
            sin = func_151423D8(angle - 0x40);
            cos = func_151423D8(angle);
            dist = func_150ADA68() * radius;
            size = func_150ADA68() * sizeRange + sizeBase;
            height = func_150ADA68() * heightRange + heightBase;
            particle.pos.unk0 = dist * sin + x;
            particle.pos.unk8 = dist * cos + z;
            particle.unkC = 22.6f * size;
            particle.unk14 = 13.786f * size;
            particle.unk10 = 57.969f * height;
            particle.unk18 = 42.714f * height;
            particle.unk1C = 118.763f * height;
            particle.unk24 = func_150ADA20() % (u32)(lifeRange + 1) + lifeBase;
            func_15149550(&particle, 10, 0, 0, 0xFF, 1);
            count--;
        } while (count != 0);
    }
}

/* Spawns one particle at `pos` with randomized size, lifetime and fade. */
void func_1514AD9C(struct17 *pos, u8 arg1, s32 arg2) {
    Struct15149550Arg particle;
    f32 size;
    f32 height;

    // matching: both stores on one line (-g3 line numbers steer as1 scheduling)
    particle.unk20 = 0x6231; particle.unk22 = 0x1A4D; particle.pos = *pos;
    particle.unk26 = 0;
    particle.unk27 = 0;
    particle.unk28 = 0;
    particle.unk29 = 0xFF;
    particle.unk2D = 0xFF;
    particle.unk2E = 0;
    particle.unk2F = 1;
    particle.unk30 = 0;
    particle.unk2A = 1;
    particle.unk34 = 28;
    particle.unk3C = 0.0f;
    particle.unk40 = 0.080000006f;
    particle.unk44 = 0.4f;
    size = (func_150ADA68() * 501.0f + 999.0f) * 0.001f;
    height = func_150ADA68();
    particle.unkC = 22.6f * size;
    particle.unk14 = 13.786f * size;
    height = (height * 1007.0f + 1505.0f) * 0.001f;
    particle.unk10 = 57.969f * height;
    particle.unk18 = 42.714f * height;
    particle.unk1C = 118.763f * height;
    particle.unk24 = func_150ADA20() % 17U + 16;
    particle.unk38 = (func_150ADA68() * 347.0f + 50.0f) * 0.001f;
    particle.unk2C = func_150ADA20() % 156U + 100;
    func_15149550(&particle, 10, 0, 0, arg1, arg2);
}

/* Common particle instance layout used by the update callbacks below. */
typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 life;
    /* 0x01E */ u8 pad1E[0x6];
    /* 0x024 */ s32 unk24;
    /* 0x028 */ u8 pad28[0x4];
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ f32 unk30;
    /* 0x034 */ f32 unk34;
    /* 0x038 */ f32 unk38;
    /* 0x03C */ u8 pad3C[0x10];
    /* 0x04C */ f32 unk4C;
    /* 0x050 */ f32 unk50;
    /* 0x054 */ u8 pad54[0x8];
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0x3B];
    /* 0x098 */ u8 *unk98;
    /* 0x09C */ u8 pad9C[0xB4];
    /* 0x150 */ f32 unk150;
} Particle177B50;

/*
 * Shrinks unk2C/unk30 by the fraction unk150 and integrates unk38 with velocity unk50 and
 * acceleration unk4C over one frame. Returns 0 once either size drops below 10.
 */
s32 func_1514AF74(Particle177B50 *p) {
    p->unk2C -= p->unk2C * p->unk150;
    p->unk30 -= p->unk30 * p->unk150;
    p->unk38 += p->unk50 * D_800BE9A4 + 0.5f * p->unk4C * D_800BE9A4 * D_800BE9A4;
    p->unk50 += p->unk4C * D_800BE9A4;
    if (p->unk2C < 10.0f || p->unk30 < 10.0f) {
        return 0;
    }
    return 1;
}

extern void func_15143874(s16, f32, f32 *, f32 *);

void func_1514B034(struct17 *arg0, u8 arg1, s32 arg2) {
    f32 count;
    Struct15149550Arg sp84;
    s32 angle;
    f32 h;
    f32 w;

    count = ((func_150ADA68() * 20.0f) + 29.0f) * 0.0001f * 5541.7695f;
    if (count > 1.0f) {
        sp84.unk20 = 0x6231;
        sp84.unk22 = 0x1A4D;
        sp84.pos.unk4 = arg0->unk4;
        sp84.unk26 = 0;
        sp84.unk27 = 0;
        sp84.unk28 = 0;
        sp84.unk29 = 0xFF;
        sp84.unk2D = 0xFF;
        sp84.unk2E = 0;
        sp84.unk2F = 1;
        sp84.unk30 = 0;
        sp84.unk2A = 1;
        sp84.unk34 = 0x1C;
        sp84.unk3C = 0.0f;
        do {
            angle = func_150ADA20();
            func_15143874(angle & 0xFF, func_150ADA68() * 42.0f, &sp84.pos.unk0, &sp84.pos.unk8);
            sp84.pos.unk0 += arg0->unk0;
            sp84.pos.unk8 += arg0->unk8;
            sp84.unk40 = ((func_150ADA68() * 16.0f) + 29.0f) * 0.001f;
            sp84.unk44 = ((func_150ADA68() * 600.0f) + 400.0f) * 0.001f;
            h = ((func_150ADA68() * 1205.0f) + 304.0f) * 0.001f;
            w = ((func_150ADA68() * 1006.0f) + 504.0f) * 0.001f;
            sp84.unkC = 22.6f * h;
            sp84.unk14 = 13.786f * h;
            sp84.unk10 = 57.969f * w;
            sp84.unk18 = 42.714f * w;
            sp84.unk1C = 118.763f * w;
            sp84.unk24 = func_150ADA20() % 31U + 20;
            sp84.unk38 = func_150ADA68() * 497.0f * 0.001f;
            sp84.unk2C = func_150ADA20() % 156U + 100;
            func_15149550(&sp84, 0xA, 0, 0, arg1, arg2);
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

typedef struct {
    u8 unk0;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    u8 pad6[2];
    s32 unk8;
    s32 unkC;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    f32 unk14;
    f32 unk18;
    struct17 unk1C;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s32 unk48;
    s32 pad4C;
    s32 pad50;
    s16 unk54;
    s16 unk56;
} Struct1514B8E4Desc;

typedef struct {
    s32 unk0;
    s32 unk4;
    struct17 unk8;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s32 unk34;
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    u16 unk40;
    u8 unk42;
    u8 unk43;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 unk48;
    u8 unk49;
    u8 unk4A;
    u8 unk4B;
    u8 unk4C;
    u8 unk4D;
    u8 unk4E;
    u8 unk4F;
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    u8 pad59[3];
    s32 unk5C;
    s32 unk60;
    s16 unk64;
    s16 unk66;
    s16 unk68;
    u8 unk6A;
    u8 pad6B[1];
    f32 unk6C;
    s8 unk70;
    s8 unk71;
    s8 unk72;
    s8 unk73;
} Struct1514B364Spawn;

typedef struct {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    u8 unk38;
    u8 unk39;
    u8 pad3A[2];
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 unk54;
    u8 unk55;
    u8 pad56[2];
} Struct1514B364Emit;

typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 unk1C;
    u8 pad1D[0x7];
} Struct1514B364Hit;

extern s32 func_15046C80(struct17 *, s32, f32, Struct1514B364Hit *);
extern void func_15152B38(Struct1514B364Spawn *, u8, s32);
extern void func_15152F70(Struct1514B364Emit *, u8);

void func_1514B364(struct17 *arg0, Struct1514B364Hit *arg1, u8 arg2, s32 arg3) {
    struct17 pos;

    pos = *arg0;
    {
        struct17 probe;

        probe.unk0 = arg0->unk0;
        probe.unk4 = arg0->unk4 + 200.0f;
        probe.unk8 = arg0->unk8;
        if (func_15046C80(&probe, 0, arg0->unk4 - 200.0f, arg1) == 0) {
            return;
        }
    }
    {
        Struct1514B364Spawn spawn;

        spawn.unk0 = 0xA;
        spawn.unk4 = 0;
        spawn.unk14 = 9.900001f;
        spawn.unk18 = 21.0f;
        spawn.unk1C = -3.0000002f;
        spawn.unk20 = 1.016f;
        spawn.unk24 = 30.0f;
        spawn.unk28 = 30.4f;
        spawn.unk2C = 0;
        spawn.unk2E = 0xFF;
        spawn.unk30 = -0x3F;
        spawn.unk32 = 0x2B;
        spawn.unk34 = 4;
        spawn.unk38 = 5;
        spawn.unk3C = 0x1E;
        spawn.unk3E = 0x1E;
        spawn.unk40 = 1;
        spawn.unk42 = 0xC;
        spawn.unk43 = 2;
        spawn.unk44 = 3;
        spawn.unk45 = 0xB4;
        spawn.unk46 = 0;
        spawn.unk47 = 0;
        spawn.unk48 = 0x9B;
        spawn.unk49 = 0x32;
        spawn.unk4A = 0x64;
        spawn.unk4B = 0;
        spawn.unk4C = 0x64;
        spawn.unk4D = 0xFF;
        spawn.unk4E = 0xFF;
        spawn.unk4F = 0xFF;
        spawn.unk50 = 0xFF;
        spawn.unk51 = 0;
        spawn.unk52 = 0;
        spawn.unk53 = 0;
        spawn.unk54 = 0;
        spawn.unk55 = 0xFF;
        spawn.unk56 = 0;
        spawn.unk57 = 1;
        spawn.unk58 = 0x24;
        spawn.unk5C = 0x200005;
        spawn.unk60 = 0x60600;
        spawn.unk64 = 0x14;
        spawn.unk66 = 0xC;
        spawn.unk68 = 1;
        spawn.unk6A = 0;
        spawn.unk6C = 1.f;
        spawn.unk70 = -1;
        spawn.unk71 = 0;
        spawn.unk72 = -1;
        spawn.unk73 = -1;
        spawn.unk8 = pos;
        func_15152B38(&spawn, arg2, arg3);
    }
    {
        Struct1514B364Emit emit;

        emit.unk0 = pos;
        emit.unkC = 3.99f;
        emit.unk10 = 8.11f;
        emit.unk14 = 0.60200006f;
        emit.unk18 = 0.407f;
        emit.unk1C = 10.139999f;
        emit.unk20 = 10.2699995f;
        emit.unk24 = 8;
        emit.unk26 = 8;
        emit.unk28 = 0;
        emit.unk2A = 0xFF;
        emit.unk2C = -0x40;
        emit.unk2E = 0x2C;
        emit.unk30 = 5;
        emit.unk32 = 4;
        emit.unk34 = 0x42;
        emit.unk36 = 0x1E;
        emit.unk38 = 0xA;
        emit.unk39 = 0x21;
        emit.unk3C = 1;
        emit.unk40 = 0xC;
        emit.unk44 = 0;
        emit.unk48 = 0;
        emit.unk4C = 0;
        emit.unk50 = 0;
        emit.unk54 = 0xFF;
        emit.unk55 = func_150ADA20() % 101U + 0x9B;
        func_15152F70(&emit, 0xFF);
    }
    func_1514B034(&pos, arg2, arg3);

    {
        Struct1514B8E4Desc desc;
        s32 a;
        s32 c;
        s32 b;
        s32 sum;

        desc.unk0 = 0x38;
        desc.unk2 = 1;
        desc.unk4 = 0x32;
        desc.unk8 = 0;
        desc.unkC = 0x6666;
        desc.unk13 = 0xFF;
        desc.unk14 = 1.0f;
        desc.unk18 = 1.0f;
        desc.unk1C.unk0 = pos.unk0;
        desc.unk1C.unk4 = pos.unk4 + 10.0f;
        desc.unk1C.unk8 = pos.unk8;
        desc.unk28 = 0.0f;
        desc.unk2C = 0.0f;
        desc.unk30 = 0.0f;
        desc.unk34 = 0.2f;
        desc.unk38 = 1.0f;
        desc.unk3C = 1.0f;
        desc.unk40 = 0x400C0001;
        desc.unk44 = func_150ADA20() % 101U + 100;
        desc.unk45 = 0xFF;
        desc.unk46 = 0;
        desc.unk47 = 6;
        desc.unk10 = func_150ADA20() % 51U + 0xB4;
        desc.unk11 = func_150ADA20() % 101U;
        desc.unk12 = func_150ADA20() % 1U;
        a = func_150ADA20();
        b = func_150ADA20();
        c = func_150ADA20();
        sum = (b & 1) + (a & 1);
        func_1513D668((s32)&desc, 0, 0x12, 0x1D, 0, sum, (u8)c,
                      100.0f, 100.0f, 0, (s32)&arg1->unk04, 0, 0, arg2, arg3);
    }
}

struct Foo1514B844 {
    char pad1C[0x1C];
    s16 unk1C;
    char pad1E[0x7A];
    u8 *unk98;
};

s32 func_1514B844(struct Foo1514B844 *arg0) {
    u8 *p = arg0->unk98;
    s16 v = arg0->unk1C;
    if (v < 0x10) {
        s32 sh = v << 4;
        if (sh < p[0x1B]) {
            p[0x1B] = sh;
        }
    }
    return 1;
}

extern f32 D_800BE9A4;

struct Foo1514B87C {
    char pad2C[0x2C];
    f32 unk2C;
    f32 unk30;
    char pad34[0x18];
    f32 unk4C;
};

s32 func_1514B87C(struct Foo1514B87C *arg0) {
    f32 temp = arg0->unk4C * D_800BE9A4;
    arg0->unk2C = arg0->unk2C + temp;
    arg0->unk30 = arg0->unk30 + temp;
    return 1;
}

struct Foo1514B8B0 {
    char pad1C[0x1C];
    s16 unk1C;
    char pad1E[0x3E];
    u8 unk5C;
};

s32 func_1514B8B0(struct Foo1514B8B0 *arg0) {
    s32 v = arg0->unk1C;
    if (v < 0x10) {
        s32 sh = v << 4;
        if (sh < arg0->unk5C) {
            arg0->unk5C = sh;
        }
    }
    return 1;
}

typedef struct {
    f32 unk0;
    f32 unk4;
} Struct1514B8E4Size;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    struct17 unk38;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
} Struct1514B8E4Extra;


void *func_1514B8E4(struct17 *arg0, Struct1514B8E4Size *arg1, s16 arg2, u8 arg3, struct17 *arg4,
                    f32 arg5, f32 arg6, f32 arg7, u8 arg8, u8 arg9, u8 argA, s32 argB, u8 argC,
                    s16 argD, s16 argE, s32 argF, u8 arg10, s32 arg11) {
    void *ret;
    Struct1514B8E4Desc desc;
    Struct1514B8E4Extra extra;
    s32 flag;

    if (arg4 != NULL) {
        extra.unk38 = *arg4;
    } else {
        extra.unk38.unk0 = 0.0f;
        extra.unk38.unk4 = 0.0f;
        extra.unk38.unk8 = 0.0f;
    }
    extra.unk44 = arg5;
    extra.unk1C = 0.0f;
    extra.unk20 = 0.0f;
    extra.unk30 = 0.0f;
    desc.unkC = 0x3E3F;
    extra.unk34 = 15935.0f;
    extra.unk2C = 15935.0f;
    extra.unk48 = arg6;
    extra.unk4C = arg7;
    extra.unk24 = 6733.0f;
    extra.unk28 = 18404.0f;
    extra.unk0 = arg1->unk0 * 15.045272f;
    extra.unk8 = arg1->unk0 * 5.8676558f;
    extra.unk10 = arg1->unk0 * 12.111444f;
    desc.unk14 = 0.0f;
    extra.unk4 = arg1->unk4 * 0.396174f;
    extra.unkC = arg1->unk4 * 0.1042563f;
    extra.unk18 = arg1->unk4 * 0.5197371f;
    extra.unk14 = arg1->unk4 * 0.34404588f;
    desc.unk18 = 0.0f;
    desc.unk0 = argB;
    desc.unk1 = 0;
    desc.unk2 = 0x2203;
    if (arg2 == -1) {
        desc.unk4 = 300;
    } else {
        desc.unk4 = arg2;
    }
    desc.unk8 = 0;
    desc.unk10 = 0xFF;
    desc.unk11 = 0xFF;
    desc.unk12 = 0xFF;
    desc.unk13 = 0xFF;
    desc.unk1C = *arg0;
    desc.unk28 = arg0->unk0;
    desc.unk2C = arg0->unk4 + 100.0f;
    desc.unk30 = arg0->unk8;
    desc.unk34 = 1.0f;
    desc.unk38 = 1.0f;
    desc.unk3C = 1.0f;
    desc.unk40 = ((arg2 == -1) ? 0 : 1) | 0x40000 | 0x80000 | 0x200000 | 0x400000 | ((argC & 1) ? 0x800000 : 0) | ((argC & 2) ? 0x2000000 : 0) | 0x40000000;
    desc.unk44 = arg3;
    desc.unk45 = 0xFF;
    desc.unk46 = 0;
    desc.unk47 = 7;
    desc.unk48 = 0;
    desc.unk54 = argD;
    desc.unk56 = argE;
    if (func_150ADA20() & 1) {
        flag = 1;
    } else {
        flag = 0;
    }
    ret = func_1513D2F0(&desc, (s32)&D_800A4AA0, arg8, arg9, argA, 0x1C, flag | 2, 0, 0, argF + 0x50, arg10, arg11);
    if (ret != NULL) {
        memcpy((u8 *)ret + 0x110, &extra, sizeof(extra));
    }
    return ret;
}

struct Foo1514BC08Obj {
    char pad0[0x24];
    s32 unk24;
    char pad28[0x4];
    f32 unk2C;
    f32 unk30;
};

struct Foo1514BC08Sub {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
};

s32 func_1514BC08(void *arg0, void *arg1) {
    struct Foo1514BC08Obj *obj;
    struct Foo1514BC08Sub *sub;

    obj = arg0;
    sub = arg1;
    sub->unk1C -= D_800BE9A4;
    if (sub->unk1C < 0.0f) {
        sub->unk1C = func_150ADA68() * 4.0f;
        sub->unk10 = func_150ADA68() * sub->unk8 + sub->unk0;
    }
    obj->unk2C = obj->unk2C + ((sub->unk10 - obj->unk2C) * 0.35f);
    sub->unk20 -= D_800BE9A4;
    if (sub->unk20 < 0.0f) {
        sub->unk20 = func_150ADA68() * 9.0f;
        if (func_150ADA20() & 1) {
            sub->unk14 = func_150ADA68() * sub->unkC + sub->unk4;
        } else {
            sub->unk14 = func_150ADA68() * sub->unk18 + sub->unk4;
        }
    }
    obj->unk30 = obj->unk30 + ((sub->unk14 - obj->unk30) * 0.4f);
    sub->unk30 -= D_800BE9A4;
    if (sub->unk30 < 0.0f) {
        sub->unk30 = func_150ADA68() * 7.0f;
        sub->unk2C = func_150ADA68() * sub->unk28 + sub->unk24;
    }
    sub->unk34 = sub->unk34 + ((sub->unk2C - sub->unk34) * 0.4f);
    obj->unk24 = sub->unk34;
    return 1;
}
