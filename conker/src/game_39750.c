#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Pseudo-random value mixed from the CPU cycle counter and func_150ADA20. */
#define RANDOM() (osGetCount() * func_150ADA20())
/* Low 16 bits of RANDOM(); multiply by 1/65535 for a fraction in [0, 1]. */
#define RANDOM_U16() (RANDOM() & 0xFFFF)

/* Clears D_800DC468[] and the counters that go with it. */
void func_1500C2A0(void) {
    s32 i;

    for (i = 0; i < 233; i++) {
        D_800DC468[i] = 0;
    }

    D_800DC460 = 0;
    D_800DC464 = 0;
    D_800DC63C = 0;
}

/* matching: the first 0x78 bytes of ParticleSpawn; a full ParticleSpawn on the stack changes the frame. */
typedef struct ParticleSpawn78 {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ vertex rot;
    /* 0x1C */ vertex scale;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex rotVel;
    /* 0x4C */ f32 gravity;
    /* 0x50 */ u32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ u16 model;
    /* 0x58 */ u8 variant;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s16 *unk5C;
    /* 0x60 */ u8 alpha;
    /* 0x61 */ u8 updateFn;
    /* 0x62 */ u8 update2Fn;
    /* 0x63 */ u8 floorHitFn;
    /* 0x64 */ u8 floorType3Fn;
    /* 0x65 */ u8 wallHitFn;
    /* 0x66 */ u8 destroyFn;
    /* 0x67 */ u8 squashFn;
    /* 0x68 */ u8 matrixFn;
    /* 0x69 */ s8 drawFn;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct Actor *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 fadeStart;
    /* 0x74 */ s16 fadeRate;
    /* 0x76 */ u8 pad76[2];
} ParticleSpawn78;

void func_15008340(s32 region, f32 *x, f32 *z, f32 *top, f32 *bottom);
void func_1510F800(s32);
void *func_15132A4C(void *, s32, s32, s32, u8, s32);
void *func_15130280(void *, s32, s32, s32, s32, s32);
void *memcpy(void *, const void *, u32);

/*
 * Spawns func_1514462C(region) * density * 2e-6 particles at random points of the region,
 * each with a random type from a fixed list.
 */
void func_1500C2F0(s32 region, f32 density) {
    f32 count;
    s32 pad; // matching: stack slot

    count = func_1514462C(region) * density * 1.999999995e-06f;
    if (count > 1.0f) {
        f32 inv65535;
        ParticleSpawn78 part;
        f32 top;
        f32 bottom;

        part.radius = 1.0f;
        part.elasticity = 1.0f;
        part.scale.x = 1.0f;
        part.scale.y = 1.0f;
        part.scale.z = 1.0f;
        part.flags = 0x5D40;
        part.lifetime = 300;
        part.variant = 0;
        part.alpha = 0xFF;
        part.updateFn = 14;
        part.update2Fn = 0;
        part.floorHitFn = 0;
        part.floorType3Fn = 0;
        part.wallHitFn = 0;
        part.destroyFn = 0;
        part.squashFn = 0;
        part.matrixFn = 2;
        part.unk6A = 0;
        part.owner = 0;
        part.ownerId = 0;
        part.fadeStart = 1;
        part.fadeRate = 0xFF;
        inv65535 = 1.525902189e-05f;
        part.rotVel.y = 0.0f;
        do {
            s32 pad2; // matching: stack slot
            const u16 types[9] = { 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4E, 0x4F, 0x50, 0x51 };
            f32 range; // matching: reused for the distance to the nearer bound (frame size)
            f32 y;
            f32 below;

            func_15008340(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.vel.y = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                below = fabsf(y - bottom);
                range = (below < above) ? below : above;
                part.gravity = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.vel.x = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.vel.z = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            part.rot.x = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.y = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.z = RANDOM_U16() * inv65535 * 360.0f;
            part.rotVel.x = RANDOM_U16() * inv65535 * 1.600000024f;
            part.rotVel.z = RANDOM_U16() * inv65535 * 1.600000024f;
            part.model = types[RANDOM() % 9];
            if (part.model == 0x4B || part.model == 0x48 || part.model == 0x4E || part.model == 0x4F ||
                part.model == 0x50 || part.model == 0x51) {
                part.sizeXZ = part.sizeY = RANDOM_U16() * inv65535 * 0.25f + 0.5f;
            } else {
                part.sizeXZ = part.sizeY = RANDOM_U16() * inv65535 * 0.25f + 1.0f;
            }
            func_1510F800(0);
            part.unk5C = func_1510FD20((s32)part.pos.x, (s32)part.pos.z);
            func_15132A4C(&part, 3, 0xFF, 0, 0xFF, 1);
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* matching: the first 0x68 bytes of ParticleDesc; a full ParticleDesc on the stack changes the frame. */
typedef struct ParticleDesc68 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 renderMode; /* low byte: D_800A4AC8 index, high byte: func_1513F4E4 arg */
    /* 0x0A */ s16 timer;
    /* 0x0C */ s32 frame;      /* 16.16 animation frame */
    /* 0x10 */ s32 frameRate;
    /* 0x14 */ u8 r;
    /* 0x15 */ u8 g;
    /* 0x16 */ u8 b;
    /* 0x17 */ u8 a;
    /* 0x18 */ u8 r2;
    /* 0x19 */ u8 g2;
    /* 0x1A */ u8 b2;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 brightness;
    /* 0x1D */ u8 texture;     /* D_80090B60 index */
    /* 0x1E */ s16 fadeStart;
    /* 0x20 */ s16 fadeRate;
    /* 0x22 */ s16 scaleStart;
    /* 0x24 */ f32 scaleRate;
    /* 0x28 */ f32 width;
    /* 0x2C */ f32 height;
    /* 0x30 */ vertex pos;
    /* 0x3C */ vertex offset;
    /* 0x48 */ vertex vel;
    /* 0x54 */ f32 gravity;
    /* 0x58 */ u32 flags;      /* PARTICLE_* */
    /* 0x5C */ struct ParticleViewMask *viewMask;
    /* 0x60 */ u8 primColorMode;
    /* 0x61 */ u8 envColorMode;
    /* 0x62 */ s8 updateFunc;  /* D_80089760 index, -1 for none */
    /* 0x63 */ s8 deathFunc;   /* D_80089800 index, -1 for none */
    /* 0x64 */ s8 drawFunc;    /* D_80089804 index, -1 for none */
    /* 0x65 */ u8 handlerSet;  /* D_80089814/D_80089844/D_80089878 index */
    /* 0x66 */ u8 views;       /* bit per view the particle is drawn in */
    /* 0x67 */ u8 pad67;
} ParticleDesc68;

/* Extra data copied to offset 0xA8 of the object func_15130280 returns. */
typedef struct {
    /* 0x00 */ f32 size;
    /* 0x04 */ s8 unk04;
    /* 0x05 */ s8 unk05;
    /* 0x06 */ s8 unk06;
    /* 0x07 */ s8 unk07;
    /* 0x08 */ f32 unk08; /* random fraction of size */
    /* 0x0C */ f32 unk0C; /* random fraction of size */
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 pad18[4];
} Particle15130280Extra; /* 0x1C bytes are copied */

/* Spawns func_1514462C(region) * density * 2e-6 func_15130280 particles of random size. */
void func_1500C8D0(s32 region, f32 density) {
    f32 count;

    count = func_1514462C(region) * density * 1.999999995e-06f;
    if (count > 1.0f) {
        f32 minFrac;
        f32 fracRange;
        f32 inv65535;
        ParticleDesc68 part;
        Particle15130280Extra extra;
        void *obj;
        f32 top;
        f32 bottom;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        f32 temp; // random size, then distance to the bottom
        s32 pad2; // matching: stack slots
        s32 pad3;

        minFrac = 0.200000003f;
        part.renderMode = 0x4403;
        part.unk0 = 0x200005;
        part.a = 0xFF;
        fracRange = 0.25f;
        part.unk4 = 0;
        part.frame = 0;
        part.frameRate = 0;
        part.r = 0;
        part.g = 0;
        part.b = 0;
        part.r2 = 0;
        part.g2 = 0;
        part.b2 = 0;
        part.brightness = 0xFF;
        part.offset.x = 0.0f;
        part.offset.y = 0.0f;
        part.offset.z = 0.0f;
        part.scaleStart = 1;
        part.scaleRate = 1.0f;
        part.timer = 300;
        part.fadeStart = 1;
        part.fadeRate = 0xFF;
        part.primColorMode = 6;
        part.envColorMode = 8;
        part.updateFunc = 23;
        part.deathFunc = -1;
        part.drawFunc = -1;
        part.handlerSet = 0;
        extra.unk04 = 0;
        extra.unk05 = 0;
        extra.unk10 = 1.0f;
        extra.unk14 = 1.0f;
        inv65535 = 1.525902189e-05f;
        do {
            s32 pad4; // matching: stack slot
            s32 flag40;

            temp = RANDOM_U16() * inv65535 * 350.0f + 50.0f;
            if (func_150ADA20() & 1) {
                part.texture = 0x58;
                extra.size = part.width = part.height = temp;
            } else {
                part.texture = 0x40;
                y = temp * 0.6999999881f; // matching: y's register doubles as the smaller size
                part.height = y;
                part.width = y;
                extra.size = y;
            }
            part.alpha = RANDOM() % 101 + 50;
            extra.unk06 = (RANDOM() & 3) + 2;
            extra.unk07 = (RANDOM() & 3) + 2;
            extra.unk08 = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            extra.unk0C = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            // matching: a separate statement makes IDO call func_150ADA20 for 0x40 first
            flag40 = (func_150ADA20() & 1) ? 0x40 : 0;
            part.flags = flag40 | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x4000 | 0x8000 | 0x10000 | 0x20000;
            func_15008340(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.vel.y = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                temp = fabsf(y - bottom);
                range = (temp < above) ? temp : above;
                part.gravity = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.vel.x = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.vel.z = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            func_1510F800(0);
            part.viewMask = func_1510FD20((s32)part.pos.x, (s32)part.pos.z);
            obj = func_15130280(&part, 2, 0, 28, 255, 1);
            if (obj != NULL) {
                memcpy((u8 *)obj + 0xA8, &extra, sizeof(extra));
            }
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* matching: the first 0x4C bytes of ParticleDef; a full ParticleDef on the stack changes the frame. */
typedef struct ParticleDef4C {
    /* 0x00 */ u8 texture;      /* D_80090B60 index, 0xFF = untextured */
    /* 0x01 */ u8 type;         /* selects the D_80089F64 handler */
    /* 0x02 */ u16 renderModes; /* low byte: D_800A4AC8 index, high byte: combiner index */
    /* 0x04 */ s16 lifetime;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 frame;       /* 16.16 animation frame */
    /* 0x0C */ s32 frameSpeed;
    /* 0x10 */ u8 rgba[4];
    /* 0x14 */ f32 width;
    /* 0x18 */ f32 height;
    /* 0x1C */ vertex pos;
    /* 0x28 */ vertex rot;      /* a point instead for func_1513F114 (bend target) and beams (end point) */
    /* 0x34 */ vertex scale;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;        /* faded out with the lifetime (flag 0x8000000) */
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s16 *unk48;
} ParticleDef4C;

/* Extra data copied to offset 0x110 of the object func_1513D2F0 returns. */
typedef struct {
    /* 0x00 */ f32 centerY;
    /* 0x04 */ f32 amplitude; /* max distance from centerY that stays inside the region */
    /* 0x08 */ f32 phase; /* radians */
    /* 0x0C */ f32 phaseSpeed;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
} Particle1513D2F0Extra; /* size 0x1C */

// The real prototype: functions.h has an s32 second parameter, and passing (s32)&D_800A4AA0 changes codegen.

/* Spawns func_1514462C(region) * density * 2e-7 func_1513D2F0 particles using D_800A4AA0. */
void func_1500CE4C(s32 region, f32 density) {
    f32 count;
    s32 pad; // matching: stack slot

    count = func_1514462C(region) * density * 2.000000023e-07f;
    if (count > 1.0f) {
        f32 inv65535;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        ParticleDef4C part;
        Particle1513D2F0Extra extra;
        f32 top;
        f32 bottom;
        f32 temp;
        void *obj;
        s32 pad2; // matching: stack slots
        s32 pad3;

        part.texture = 0x57;
        part.type = 0;
        part.renderModes = 0x3B03;
        part.lifetime = 300;
        part.frame = 0;
        part.frameSpeed = 0;
        part.rgba[3] = 0xFF;
        part.scale.x = 0.0f;
        part.scale.y = 0.0f;
        part.scale.z = 0.0f;
        part.flags = 0x037C0000;
        part.unk45 = 0xFF;
        part.unk46 = 0;
        part.unk47 = 6;
        extra.unk14 = 0.0f;
        part.rgba[0] = 0xFF;
        part.rgba[1] = 0xFF;
        part.rgba[2] = 0xFF;
        inv65535 = 1.525902189e-05f;
        do {
            temp = RANDOM_U16() * inv65535 * 12.0f + 10.0f;
            part.width = temp;
            part.height = temp;
            func_15008340(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            extra.centerY = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                temp = fabsf(y - bottom);
                range = (temp < above) ? temp : above;
                extra.amplitude = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            extra.phase = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            extra.phaseSpeed = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            extra.unk10 = RANDOM_U16() * inv65535;
            extra.unk18 = RANDOM_U16() * inv65535;
            part.rot.x = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.y = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.z = RANDOM_U16() * inv65535 * 360.0f;
            part.unk44 = RANDOM() % 156 + 100;
            func_1510F800(0);
            part.unk48 = func_1510FD20((s32)part.pos.x, (s32)part.pos.z);
            obj = func_1513D2F0(&part, &D_800A4AA0, 0x23, 0, 0, 0x1E, 0, 3, 0xFF, 0x1C, 0xFF, 1);
            if (obj != NULL) {
                memcpy((u8 *)obj + 0x110, &extra, sizeof(extra));
            }
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* Like func_1500C2F0, at a tenth of the density and always with particle type 0x53. */
void func_1500D3AC(s32 region, f32 density) {
    f32 count;
    s32 pad; // matching: stack slot

    count = func_1514462C(region) * density * 2.000000023e-07f;
    if (count > 1.0f) {
        f32 inv65535;
        ParticleSpawn78 part;
        f32 top;
        f32 bottom;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        f32 below;

        part.radius = 1.0f;
        part.elasticity = 1.0f;
        part.scale.x = 1.0f;
        part.scale.y = 1.0f;
        part.scale.z = 1.0f;
        part.flags = 0x4D40;
        part.lifetime = 300;
        part.variant = 0;
        part.alpha = 0xFF;
        part.updateFn = 14;
        part.update2Fn = 0;
        part.floorHitFn = 0;
        part.floorType3Fn = 0;
        part.wallHitFn = 0;
        part.destroyFn = 0;
        part.squashFn = 0;
        part.matrixFn = 2;
        part.unk6A = 0;
        part.owner = 0;
        part.ownerId = 0;
        part.fadeStart = 1;
        part.fadeRate = 0xFF;
        part.model = 0x53;
        inv65535 = 1.525902189e-05f;
        part.rotVel.y = 0.0f;
        do {
            func_15008340(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.vel.y = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                below = fabsf(y - bottom);
                range = (below < above) ? below : above;
                part.gravity = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.vel.x = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.vel.z = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            part.rot.x = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.y = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.z = RANDOM_U16() * inv65535 * 360.0f;
            part.rotVel.x = RANDOM_U16() * inv65535 * 1.600000024f;
            part.rotVel.z = RANDOM_U16() * inv65535 * 1.600000024f;
            part.sizeXZ = part.sizeY = RANDOM_U16() * inv65535 * 0.25f + 1.0f;
            func_1510F800(0);
            part.unk5C = func_1510FD20((s32)part.pos.x, (s32)part.pos.z);
            func_15132A4C(&part, 3, 0xFF, 0, 0xFF, 1);
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* Like func_1500C2F0 with another particle type list; type 0x39 is drawn at 35% scale. */
void func_1500D8C8(s32 region, f32 density) {
    f32 count;
    s32 pad; // matching: stack slot

    count = func_1514462C(region) * density * 1.999999995e-06f;
    if (count > 1.0f) {
        f32 inv65535;
        ParticleSpawn78 part;
        f32 top;
        f32 bottom;

        part.radius = 1.0f;
        part.elasticity = 1.0f;
        part.scale.x = 1.0f;
        part.scale.y = 1.0f;
        part.scale.z = 1.0f;
        part.flags = 0x4D40;
        part.lifetime = 300;
        part.variant = 0;
        part.alpha = 0xFF;
        part.updateFn = 14;
        part.update2Fn = 0;
        part.floorHitFn = 0;
        part.floorType3Fn = 0;
        part.wallHitFn = 0;
        part.destroyFn = 0;
        part.squashFn = 0;
        part.matrixFn = 2;
        part.unk6A = 0;
        part.owner = 0;
        part.ownerId = 0;
        part.fadeStart = 1;
        part.fadeRate = 0xFF;
        inv65535 = 1.525902189e-05f;
        part.rotVel.y = 0.0f;
        do {
            s32 pad2; // matching: stack slot
            const u16 types[14] = { 0x76, 0x77, 0x78, 0x79, 0x53, 0x3E, 0x3D, 0x39, 0x02, 0x03, 0x4E, 0x4F, 0x50, 0x51 };
            f32 range; // matching: reused for the distance to the nearer bound
            f32 y;
            f32 below;

            func_15008340(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.vel.y = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                below = fabsf(y - bottom);
                range = (below < above) ? below : above;
                part.gravity = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.vel.x = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.vel.z = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            part.rot.x = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.y = RANDOM_U16() * inv65535 * 360.0f;
            part.rot.z = RANDOM_U16() * inv65535 * 360.0f;
            part.rotVel.x = RANDOM_U16() * inv65535 * 1.600000024f;
            part.rotVel.z = RANDOM_U16() * inv65535 * 1.600000024f;
            part.model = types[RANDOM() % 14];
            part.sizeXZ = part.sizeY = RANDOM_U16() * inv65535 * 0.25f + 0.5f;
            if (part.model == 0x39) {
                part.sizeXZ *= 0.349999994f;
                part.sizeY *= 0.349999994f;
            }
            func_1510F800(0);
            part.unk5C = func_1510FD20((s32)part.pos.x, (s32)part.pos.z);
            func_15132A4C(&part, 3, 0xFF, 0, 0xFF, 1);
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* Like func_1500C8D0 with larger func_15130280 particles and random colour bytes. */
void func_1500DE88(s32 region, f32 density) {
    f32 count;

    count = func_1514462C(region) * density * 1.999999995e-06f;
    if (count > 1.0f) {
        f32 minFrac;
        f32 fracRange;
        f32 inv65535;
        ParticleDesc68 part;
        Particle15130280Extra extra;
        void *obj;
        f32 top;
        f32 bottom;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        f32 temp; // random size, then distance to the bottom

        minFrac = 0.3000000119f;
        part.renderMode = 0x5103;
        part.unk0 = 0x200005;
        part.a = 0xFF;
        part.brightness = 0xFF;
        fracRange = 0.150000006f;
        part.unk4 = 0;
        part.frame = 0;
        part.frameRate = 0;
        part.g = 0;
        part.b = 0;
        part.g2 = 0;
        part.b2 = 0;
        part.offset.x = 0.0f;
        part.offset.y = 0.0f;
        part.offset.z = 0.0f;
        part.scaleStart = 1;
        part.scaleRate = 1.0f;
        part.timer = 300;
        part.fadeStart = 1;
        part.fadeRate = 0xFF;
        part.primColorMode = 8;
        part.envColorMode = 6;
        part.updateFunc = 23;
        part.deathFunc = -1;
        part.drawFunc = -1;
        part.handlerSet = 0;
        extra.unk04 = 0;
        extra.unk05 = 0;
        extra.unk10 = 1.0f;
        extra.unk14 = 1.0f;
        part.texture = 108;
        inv65535 = 1.525902189e-05f;
        do {
            temp = RANDOM_U16() * inv65535 * 600.0f + 200.0f;
            part.height = temp;
            part.width = temp;
            extra.size = temp;
            part.alpha = RANDOM() % 71 + 50;
            extra.unk06 = RANDOM() % 3;
            extra.unk07 = RANDOM() % 3;
            extra.unk08 = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            extra.unk0C = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            part.flags = ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x4000 |
                         0x8000 | 0x20000 | 0x800000;
            part.r2 = RANDOM() % 56 + 80;
            part.r = RANDOM() % 46 + 180;
            func_15008340(region, &part.pos.x, &part.pos.z, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.vel.y = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.pos.y = y;
                temp = fabsf(y - bottom);
                range = (temp < above) ? temp : above;
                part.gravity = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.vel.x = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.vel.z = RANDOM_U16() * inv65535 * 0.01000000071f + 0.002000000095f;
            func_1510F800(0);
            part.viewMask = func_1510FD20((s32)part.pos.x, (s32)part.pos.z);
            obj = func_15130280(&part, 2, 0, 28, 255, 1);
            if (obj != NULL) {
                memcpy((u8 *)obj + 0xA8, &extra, sizeof(extra));
            }
            count -= 1.0f;
        } while (count > 1.0f);
    }
}
