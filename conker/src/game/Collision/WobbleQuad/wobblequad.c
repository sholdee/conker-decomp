#include <ultra64.h>
#include <math.h>
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/particle.h"

struct Foo150AE280Inner {
    char pad0[0x8];
    f32 unk8;
    f32 unkC;
};

struct Foo150AE280 {
    char pad0[0x20];
    f32 unk20;
    char pad24[0x4];
    f32 unk28;
    char pad2C[0x144];
    struct Foo150AE280Inner unk170;
};

struct Foo150AE36CInner {
    char pad0[0x18];
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    char pad2C[0xC];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
};

struct Foo150AE36C {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    char pad40[0x80];
    char unkC0[0x40];
    s16 *unk100[0x18];
    struct Foo150AE36CInner *unk160;
};




s32 collision_updateWobble(struct Foo150AE280 *particle) {
    struct Foo150AE280Inner *phases;

    particle->unk170.unk8 += 0.065f * gFrameDelta;
    particle->unk170.unkC += 0.0484f * gFrameDelta;
    phases = &particle->unk170;
    phases->unk8 = math_wrapRadians(particle->unk170.unk8);
    phases->unkC = math_wrapRadians(phases->unkC);
    particle->unk20 = sinf(phases->unk8) * 3.0860002f;
    particle->unk28 = sinf(phases->unkC) * 5.9140005f;
    return 1;
}

s32 func_150AE35C(s32 particle) {
    return 1;
}

s16 *collision_buildTransformedUprightQuad(struct Foo150AE36C *particle, s16 viewport) {
    s16 *vertices;
    s16 *ret;
    f32 offsetX;
    f32 offsetZ;
    f32 z;
    f32 y;
    f32 x;
    f32 pad84[2];
    MtxF mtx;
    u8 fresh;

    particle_getViewBuffer(particle->unk100, viewport, 0x40, &vertices, &fresh);
    ret = vertices;
    if (vertices) {
        if (fresh) {
            memcpy(particle->unk100[viewport], particle->unkC0, 0x40);
            memcpy(particle->unk100[viewport] + 0x20, particle->unkC0, 0x40);
        }
    } else {
        return 0;
    }

    math_mtxFRotScaleXZ(mtx, particle->unk160->unk18, particle->unk160->unk1C, particle->unk160->unk20, particle->unk160->unk24, particle->unk160->unk28, particle->unk160->unk38, particle->unk160->unk3C, particle->unk160->unk40);
    math_transformPoint(mtx, particle->unk34, particle->unk38, particle->unk3C, &x, &y, &z);

    offsetX = gCameraAngleCosines[viewport] * particle->unk2C;
    offsetZ = gCameraAngleSines[viewport] * particle->unk2C;

    vertices[3] = 0;
    vertices[0xB] = 0;
    vertices[0x13] = 0;
    vertices[0x1B] = 0;
    vertices[0] = vertices[0x18] = x + offsetX;
    vertices[1] = vertices[9] = y;
    vertices[2] = vertices[0x1A] = z - offsetZ;
    vertices[8] = vertices[0x10] = x - offsetX;
    vertices[0x11] = vertices[0x19] = y + particle->unk30;
    vertices[0xA] = vertices[0x12] = z + offsetZ;

    return ret;
}
