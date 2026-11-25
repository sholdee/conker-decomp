#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

extern void func_151D5D60(u8 **buffers, s16 index, s32 size, void **out, u8 *isNew);

/* Particle spawn descriptor passed to func_15149550. */
typedef struct {
    Vec3f pos;
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

typedef struct Vtx1514A6A0 {
    s16 x, y, z, flag;
    u8 other[8];
} Vtx1514A6A0;

typedef struct Beam1514A6A0 {
    u8 unknown0[0x2C];
    f32 width;
    f32 progress;
    Vec3f start;
    Vec3f end;
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
    Vec3f tip;
    f32 dx, dy, dz;
    f32 vx, vy, vz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    Vec3f *eye;
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
    eye = (Vec3f *)&D_800DBFF0[cam].eye.x;
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
        particle.pos.y = y;

        do {
            // matching: explicit mask; func_150ADA20 really returns a full word
            angle = func_150ADA20() & 0xFF;
            sin = func_151423D8(angle - 0x40);
            cos = func_151423D8(angle);
            dist = func_150ADA68() * radius;
            size = func_150ADA68() * sizeRange + sizeBase;
            height = func_150ADA68() * heightRange + heightBase;
            particle.pos.x = dist * sin + x;
            particle.pos.z = dist * cos + z;
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
void func_1514AD9C(Vec3f *pos, u8 arg1, s32 arg2) {
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

/* Spawns a random number of particles scattered within 42 units of `pos` in the XZ plane. */
void func_1514B034(Vec3f *pos, u8 arg1, s32 arg2) {
    f32 count;
    Struct15149550Arg particle;
    s32 angle;
    f32 size;
    f32 height;

    count = (func_150ADA68() * 20.0f + 29.0f) * 0.0001f * 5541.7695f;
    if (count > 1.0f) {
        particle.unk20 = 0x6231;
        particle.unk22 = 0x1A4D;
        particle.pos.y = pos->y;
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
        do {
            angle = func_150ADA20();
            func_15143874(angle & 0xFF, func_150ADA68() * 42.0f, &particle.pos.x, &particle.pos.z);
            particle.pos.x += pos->x;
            particle.pos.z += pos->z;
            particle.unk40 = (func_150ADA68() * 16.0f + 29.0f) * 0.001f;
            particle.unk44 = (func_150ADA68() * 600.0f + 400.0f) * 0.001f;
            size = (func_150ADA68() * 1205.0f + 304.0f) * 0.001f;
            height = (func_150ADA68() * 1006.0f + 504.0f) * 0.001f;
            particle.unkC = 22.6f * size;
            particle.unk14 = 13.786f * size;
            particle.unk10 = 57.969f * height;
            particle.unk18 = 42.714f * height;
            particle.unk1C = 118.763f * height;
            particle.unk24 = func_150ADA20() % 31U + 20;
            particle.unk38 = func_150ADA68() * 497.0f * 0.001f;
            particle.unk2C = func_150ADA20() % 156U + 100;
            func_15149550(&particle, 10, 0, 0, arg1, arg2);
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    Vec3f unk8;
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
    Vec3f unk0;
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

extern s32 func_15046C80(Vec3f *, s32, f32, Struct1514B364Hit *);
extern void func_15152B38(Struct1514B364Spawn *, u8, s32);
extern void func_15152F70(Struct1514B364Emit *, u8);

/* If func_15046C80 finds ground within 200 units of `position`, spawns a cluster of effects there. */
void func_1514B364(Vec3f *position, Struct1514B364Hit *ground, u8 arg2, s32 arg3) {
    Vec3f pos;

    pos = *position;
    {
        Vec3f probe;

        probe.x = position->x;
        probe.y = position->y + 200.0f;
        probe.z = position->z;
        if (func_15046C80(&probe, 0, position->y - 200.0f, ground) == 0) {
            return;
        }
    }
    {
        Struct1514B364Spawn spawn;

        spawn.unk0 = 10;
        spawn.unk4 = 0;
        spawn.unk14 = 9.900001f;
        spawn.unk18 = 21.0f;
        spawn.unk1C = -3.0000002f;
        spawn.unk20 = 1.016f;
        spawn.unk24 = 30.0f;
        spawn.unk28 = 30.4f;
        spawn.unk2C = 0;
        spawn.unk2E = 0xFF;
        spawn.unk30 = -63;
        spawn.unk32 = 43;
        spawn.unk34 = 4;
        spawn.unk38 = 5;
        spawn.unk3C = 30;
        spawn.unk3E = 30;
        spawn.unk40 = 1;
        spawn.unk42 = 12;
        spawn.unk43 = 2;
        spawn.unk44 = 3;
        spawn.unk45 = 180;
        spawn.unk46 = 0;
        spawn.unk47 = 0;
        spawn.unk48 = 155;
        spawn.unk49 = 50;
        spawn.unk4A = 100;
        spawn.unk4B = 0;
        spawn.unk4C = 100;
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
        spawn.unk58 = 36;
        spawn.unk5C = 0x200005;
        spawn.unk60 = 0x60600;
        spawn.unk64 = 20;
        spawn.unk66 = 12;
        spawn.unk68 = 1;
        spawn.unk6A = 0;
        spawn.unk6C = 1.0; // matching: a double literal; 1.0f loads differently
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
        emit.unk2C = -64;
        emit.unk2E = 44;
        emit.unk30 = 5;
        emit.unk32 = 4;
        emit.unk34 = 66;
        emit.unk36 = 30;
        emit.unk38 = 10;
        emit.unk39 = 33;
        emit.unk3C = 1;
        emit.unk40 = 12;
        emit.unk44 = 0;
        emit.unk48 = 0;
        emit.unk4C = 0;
        emit.unk50 = 0;
        emit.unk54 = 0xFF;
        emit.unk55 = func_150ADA20() % 101U + 155;
        func_15152F70(&emit, 0xFF);
    }
    func_1514B034(&pos, arg2, arg3);

    {
        ParticleDef desc;
        s32 randA;
        s32 randC;
        s32 randB;
        s32 sum;

        desc.texture = 56;
        desc.renderModes = 1;
        desc.lifetime = 50;
        desc.frame = 0;
        desc.frameSpeed = 0x6666;
        desc.rgba[3] = 0xFF;
        desc.width = 1.0f;
        desc.height = 1.0f;
        desc.pos.x = pos.x;
        desc.pos.y = pos.y + 10.0f;
        desc.pos.z = pos.z;
        desc.rot.x = 0.0f;
        desc.rot.y = 0.0f;
        desc.rot.z = 0.0f;
        desc.scale.x = 0.2f;
        desc.scale.y = 1.0f;
        desc.scale.z = 1.0f;
        desc.flags = 0x400C0001;
        desc.unk44 = func_150ADA20() % 101U + 100;
        desc.unk45 = 0xFF;
        desc.unk46 = 0;
        desc.unk47 = 6;
        desc.rgba[0] = func_150ADA20() % 51U + 180;
        desc.rgba[1] = func_150ADA20() % 101U;
        desc.rgba[2] = func_150ADA20() % 1U;
        randA = func_150ADA20();
        randB = func_150ADA20();
        randC = func_150ADA20();
        sum = (randB & 1) + (randA & 1);
        func_1513D668(&desc, 0, 18, 29, 0, sum, (u8)randC,
                      100.0f, 100.0f, 0, (s32)&ground->unk04, 0, 0, arg2, arg3);
    }
}

/* Fades out over the last 16 frames of life: caps the alpha byte at unk98[0x1B] to life * 16. */
s32 func_1514B844(Particle177B50 *p) {
    u8 *unk98 = p->unk98;

    if (p->life < 16 && p->life * 16 < unk98[0x1B]) {
        unk98[0x1B] = p->life * 16;
    }
    return 1;
}

/* Grows unk2C and unk30 by unk4C, scaled by the frame delta D_800BE9A4. */
s32 func_1514B87C(Particle177B50 *p) {
    f32 growth = p->unk4C * D_800BE9A4;

    p->unk2C += growth;
    p->unk30 += growth;
    return 1;
}

/* Fades out over the last 16 frames of life: caps alpha at life * 16. */
s32 func_1514B8B0(Particle177B50 *p) {
    if (p->life < 16 && p->life * 16 < p->alpha) {
        p->alpha = p->life * 16;
    }
    return 1;
}

typedef struct {
    f32 unk0;
    f32 unk4;
} Struct1514B8E4Size;

/* Particle state at offset 0x110 written by func_1514B8E4 and animated by func_1514BC08. */
typedef struct {
    /* 0x00 */ f32 base2C;
    /* 0x04 */ f32 base30;
    /* 0x08 */ f32 range2C;
    /* 0x0C */ f32 range30A;
    /* 0x10 */ f32 target2C;
    /* 0x14 */ f32 target30;
    /* 0x18 */ f32 range30B;
    /* 0x1C */ f32 timer2C;
    /* 0x20 */ f32 timer30;
    /* 0x24 */ f32 base24;
    /* 0x28 */ f32 range24;
    /* 0x2C */ f32 target24;
    /* 0x30 */ f32 timer24;
    /* 0x34 */ f32 value24;
    /* 0x38 */ Vec3f unk38;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
} Wobble1514BC08;

/*
 * Spawns a particle at `pos` through func_1513D2F0 and stores its initial func_1514BC08 state.
 * A `life` of -1 means 300 frames without the timed-life flag.
 */
void *func_1514B8E4(Vec3f *pos, Struct1514B8E4Size *scale, s16 life, u8 arg3, Vec3f *arg4,
                    f32 arg5, f32 arg6, f32 arg7, u8 arg8, u8 arg9, u8 argA, s32 argB, u8 flags,
                    s16 argD, s16 argE, s32 argF, u8 arg10, s32 arg11) {
    void *particle;
    ParticleDef desc;
    Wobble1514BC08 state;
    s32 flag;

    if (arg4 != NULL) {
        state.unk38 = *arg4;
    } else {
        state.unk38.x = 0.0f;
        state.unk38.y = 0.0f;
        state.unk38.z = 0.0f;
    }
    state.unk44 = arg5;
    state.timer2C = 0.0f;
    state.timer30 = 0.0f;
    state.timer24 = 0.0f;
    desc.frameSpeed = 15935;
    state.value24 = 15935.0f;
    state.target24 = 15935.0f;
    state.unk48 = arg6;
    state.unk4C = arg7;
    state.base24 = 6733.0f;
    state.range24 = 18404.0f;
    state.base2C = scale->unk0 * 15.045272f;
    state.range2C = scale->unk0 * 5.8676558f;
    state.target2C = scale->unk0 * 12.111444f;
    desc.width = 0.0f;
    state.base30 = scale->unk4 * 0.396174f;
    state.range30A = scale->unk4 * 0.1042563f;
    state.range30B = scale->unk4 * 0.5197371f;
    state.target30 = scale->unk4 * 0.34404588f;
    desc.height = 0.0f;
    desc.texture = argB;
    desc.type = 0;
    desc.renderModes = 0x2203;
    if (life == -1) {
        desc.lifetime = 300;
    } else {
        desc.lifetime = life;
    }
    desc.frame = 0;
    desc.rgba[0] = 0xFF;
    desc.rgba[1] = 0xFF;
    desc.rgba[2] = 0xFF;
    desc.rgba[3] = 0xFF;
    desc.pos = *pos;
    desc.rot.x = pos->x;
    desc.rot.y = pos->y + 100.0f;
    desc.rot.z = pos->z;
    desc.scale.x = 1.0f;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    desc.flags = ((life == -1) ? 0 : 1) | 0x40000 | 0x80000 | 0x200000 | 0x400000 | ((flags & 1) ? 0x800000 : 0) | ((flags & 2) ? 0x2000000 : 0) | 0x40000000;
    desc.unk44 = arg3;
    desc.unk45 = 0xFF;
    desc.unk46 = 0;
    desc.unk47 = 7;
    desc.unk48 = 0;
    desc.fadeStart = argD;
    desc.fadeRate = argE;
    if (func_150ADA20() & 1) {
        flag = 1;
    } else {
        flag = 0;
    }
    particle = func_1513D2F0(&desc, &D_800A4AA0, arg8, arg9, argA, 28, flag | 2, 0, 0, argF + 80, arg10, arg11);
    if (particle != NULL) {
        memcpy((u8 *)particle + 0x110, &state, sizeof(state));
    }
    return particle;
}

/*
 * Eases unk2C, unk30 and unk24 toward random targets, picking a new target for each when its
 * countdown timer (decremented by the frame delta) runs out.
 */
s32 func_1514BC08(Particle177B50 *p, Wobble1514BC08 *w) {
    w->timer2C -= D_800BE9A4;
    if (w->timer2C < 0.0f) {
        w->timer2C = func_150ADA68() * 4.0f;
        w->target2C = func_150ADA68() * w->range2C + w->base2C;
    }
    p->unk2C += (w->target2C - p->unk2C) * 0.35f;
    w->timer30 -= D_800BE9A4;
    if (w->timer30 < 0.0f) {
        w->timer30 = func_150ADA68() * 9.0f;
        if (func_150ADA20() & 1) {
            w->target30 = func_150ADA68() * w->range30A + w->base30;
        } else {
            w->target30 = func_150ADA68() * w->range30B + w->base30;
        }
    }
    p->unk30 += (w->target30 - p->unk30) * 0.4f;
    w->timer24 -= D_800BE9A4;
    if (w->timer24 < 0.0f) {
        w->timer24 = func_150ADA68() * 7.0f;
        w->target24 = func_150ADA68() * w->range24 + w->base24;
    }
    w->value24 += (w->target24 - w->value24) * 0.4f;
    p->unk24 = w->value24;
    return 1;
}
