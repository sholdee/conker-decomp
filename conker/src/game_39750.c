// functions.h declares func_1513D2F0 with an s32 second parameter; it takes a pointer (see below).
#define func_1513D2F0 func_1513D2F0_hdr
#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#undef func_1513D2F0

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

/* Particle descriptor passed to func_15132A4C. */
typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 scaleX;
    /* 0x0C */ f32 scaleY;
    /* 0x10 */ f32 rot[3]; /* degrees */
    /* 0x1C */ f32 unk1C[3];
    /* 0x28 */ f32 posX;
    /* 0x2C */ f32 posY;
    /* 0x30 */ f32 posZ;
    /* 0x34 */ f32 phase; /* radians */
    /* 0x38 */ f32 centerY;
    /* 0x3C */ f32 phaseSpeed;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 amplitude; /* max distance from centerY that stays inside the region */
    /* 0x50 */ s32 flags;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ u16 type;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60[9];
    /* 0x69 */ u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
} Particle15132A4C; /* size 0x78 */

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
        Particle15132A4C part;
        f32 top;
        f32 bottom;

        part.unk00 = 1.0f;
        part.unk04 = 1.0f;
        part.unk1C[0] = 1.0f;
        part.unk1C[1] = 1.0f;
        part.unk1C[2] = 1.0f;
        part.flags = 0x5D40;
        part.unk54 = 300;
        part.unk58 = 0;
        part.unk60[0] = 0xFF;
        part.unk60[1] = 14;
        part.unk60[2] = 0;
        part.unk60[3] = 0;
        part.unk60[4] = 0;
        part.unk60[5] = 0;
        part.unk60[6] = 0;
        part.unk60[7] = 0;
        part.unk60[8] = 2;
        part.unk6A = 0;
        part.unk6C = 0;
        part.unk70 = 0;
        part.unk72 = 1;
        part.unk74 = 0xFF;
        inv65535 = 1.525902189e-05f;
        part.unk44 = 0.0f;
        do {
            s32 pad2; // matching: stack slot
            const u16 types[9] = { 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4E, 0x4F, 0x50, 0x51 };
            f32 range; // matching: reused for the distance to the nearer bound (frame size)
            f32 y;
            f32 below;

            func_15008340(region, &part.posX, &part.posZ, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.centerY = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.posY = y;
                below = fabsf(y - bottom);
                range = (below < above) ? below : above;
                part.amplitude = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.phase = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.phaseSpeed = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            part.rot[0] = RANDOM_U16() * inv65535 * 360.0f;
            part.rot[1] = RANDOM_U16() * inv65535 * 360.0f;
            part.rot[2] = RANDOM_U16() * inv65535 * 360.0f;
            part.unk40 = RANDOM_U16() * inv65535 * 1.600000024f;
            part.unk48 = RANDOM_U16() * inv65535 * 1.600000024f;
            part.type = types[RANDOM() % 9];
            if (part.type == 0x4B || part.type == 0x48 || part.type == 0x4E || part.type == 0x4F ||
                part.type == 0x50 || part.type == 0x51) {
                part.scaleX = part.scaleY = RANDOM_U16() * inv65535 * 0.25f + 0.5f;
            } else {
                part.scaleX = part.scaleY = RANDOM_U16() * inv65535 * 0.25f + 1.0f;
            }
            func_1510F800(0);
            part.unk5C = func_1510FD20((s32)part.posX, (s32)part.posZ);
            func_15132A4C(&part, 3, 0xFF, 0, 0xFF, 1);
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* Particle descriptor passed to func_15130280. */
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 unk08;
    /* 0x0A */ s16 unk0A;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 sizeX;
    /* 0x2C */ f32 sizeY;
    /* 0x30 */ f32 posX;
    /* 0x34 */ f32 posY;
    /* 0x38 */ f32 posZ;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 phase; /* radians */
    /* 0x4C */ f32 centerY;
    /* 0x50 */ f32 phaseSpeed;
    /* 0x54 */ f32 amplitude; /* max distance from centerY that stays inside the region */
    /* 0x58 */ s32 flags;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s8 unk60;
    /* 0x61 */ s8 unk61;
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ s8 unk64;
    /* 0x65 */ s8 unk65;
} Particle15130280; /* size 0x68 */

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
        Particle15130280 part;
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
        part.unk08 = 0x4403;
        part.unk00 = 0x200005;
        part.unk17 = 0xFF;
        fracRange = 0.25f;
        part.unk04 = 0;
        part.unk0C = 0;
        part.unk10 = 0;
        part.unk14 = 0;
        part.unk15 = 0;
        part.unk16 = 0;
        part.unk18 = 0;
        part.unk19 = 0;
        part.unk1A = 0;
        part.unk1C = 0xFF;
        part.unk3C = 0.0f;
        part.unk40 = 0.0f;
        part.unk44 = 0.0f;
        part.unk22 = 1;
        part.unk24 = 1.0f;
        part.unk0A = 300;
        part.unk1E = 1;
        part.unk20 = 0xFF;
        part.unk60 = 6;
        part.unk61 = 8;
        part.unk62 = 23;
        part.unk63 = -1;
        part.unk64 = -1;
        part.unk65 = 0;
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
                part.unk1D = 0x58;
                extra.size = part.sizeX = part.sizeY = temp;
            } else {
                part.unk1D = 0x40;
                y = temp * 0.6999999881f; // matching: y's register doubles as the smaller size
                part.sizeY = y;
                part.sizeX = y;
                extra.size = y;
            }
            part.unk1B = RANDOM() % 101 + 50;
            extra.unk06 = (RANDOM() & 3) + 2;
            extra.unk07 = (RANDOM() & 3) + 2;
            extra.unk08 = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            extra.unk0C = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            // matching: a separate statement makes IDO call func_150ADA20 for 0x40 first
            flag40 = (func_150ADA20() & 1) ? 0x40 : 0;
            part.flags = flag40 | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x4000 | 0x8000 | 0x10000 | 0x20000;
            func_15008340(region, &part.posX, &part.posZ, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.centerY = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.posY = y;
                temp = fabsf(y - bottom);
                range = (temp < above) ? temp : above;
                part.amplitude = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.phase = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.phaseSpeed = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            func_1510F800(0);
            part.unk5C = func_1510FD20((s32)part.posX, (s32)part.posZ);
            obj = func_15130280(&part, 2, 0, 28, 255, 1);
            if (obj != NULL) {
                memcpy((u8 *)obj + 0xA8, &extra, sizeof(extra));
            }
            count -= 1.0f;
        } while (count > 1.0f);
    }
}

/* Particle descriptor passed to func_1513D2F0. */
typedef struct {
    /* 0x00 */ u8 unk00;
    /* 0x01 */ u8 unk01;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 sizeX;
    /* 0x18 */ f32 sizeY;
    /* 0x1C */ f32 posX;
    /* 0x20 */ f32 posY;
    /* 0x24 */ f32 posZ;
    /* 0x28 */ f32 rot[3]; /* degrees */
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
} Particle1513D2F0; /* size 0x4C */

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
void *func_1513D2F0(void *, void *, u8, u8, u8, u8, u8, s32, s32, s32, u8, s32);

/* Spawns func_1514462C(region) * density * 2e-7 func_1513D2F0 particles using D_800A4AA0. */
void func_1500CE4C(s32 region, f32 density) {
    f32 count;
    s32 pad; // matching: stack slot

    count = func_1514462C(region) * density * 2.000000023e-07f;
    if (count > 1.0f) {
        f32 inv65535;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        Particle1513D2F0 part;
        Particle1513D2F0Extra extra;
        f32 top;
        f32 bottom;
        f32 temp;
        void *obj;
        s32 pad2; // matching: stack slots
        s32 pad3;

        part.unk00 = 0x57;
        part.unk01 = 0;
        part.unk02 = 0x3B03;
        part.unk04 = 300;
        part.unk08 = 0;
        part.unk0C = 0;
        part.unk13 = 0xFF;
        part.unk34 = 0.0f;
        part.unk38 = 0.0f;
        part.unk3C = 0.0f;
        part.unk40 = 0x037C0000;
        part.unk45 = 0xFF;
        part.unk46 = 0;
        part.unk47 = 6;
        extra.unk14 = 0.0f;
        part.unk10 = 0xFF;
        part.unk11 = 0xFF;
        part.unk12 = 0xFF;
        inv65535 = 1.525902189e-05f;
        do {
            temp = RANDOM_U16() * inv65535 * 12.0f + 10.0f;
            part.sizeX = temp;
            part.sizeY = temp;
            func_15008340(region, &part.posX, &part.posZ, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            extra.centerY = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.posY = y;
                temp = fabsf(y - bottom);
                range = (temp < above) ? temp : above;
                extra.amplitude = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            extra.phase = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            extra.phaseSpeed = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            extra.unk10 = RANDOM_U16() * inv65535;
            extra.unk18 = RANDOM_U16() * inv65535;
            part.rot[0] = RANDOM_U16() * inv65535 * 360.0f;
            part.rot[1] = RANDOM_U16() * inv65535 * 360.0f;
            part.rot[2] = RANDOM_U16() * inv65535 * 360.0f;
            part.unk44 = RANDOM() % 156 + 100;
            func_1510F800(0);
            part.unk48 = func_1510FD20((s32)part.posX, (s32)part.posZ);
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
        Particle15132A4C part;
        f32 top;
        f32 bottom;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        f32 below;

        part.unk00 = 1.0f;
        part.unk04 = 1.0f;
        part.unk1C[0] = 1.0f;
        part.unk1C[1] = 1.0f;
        part.unk1C[2] = 1.0f;
        part.flags = 0x4D40;
        part.unk54 = 300;
        part.unk58 = 0;
        part.unk60[0] = 0xFF;
        part.unk60[1] = 14;
        part.unk60[2] = 0;
        part.unk60[3] = 0;
        part.unk60[4] = 0;
        part.unk60[5] = 0;
        part.unk60[6] = 0;
        part.unk60[7] = 0;
        part.unk60[8] = 2;
        part.unk6A = 0;
        part.unk6C = 0;
        part.unk70 = 0;
        part.unk72 = 1;
        part.unk74 = 0xFF;
        part.type = 0x53;
        inv65535 = 1.525902189e-05f;
        part.unk44 = 0.0f;
        do {
            func_15008340(region, &part.posX, &part.posZ, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.centerY = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.posY = y;
                below = fabsf(y - bottom);
                range = (below < above) ? below : above;
                part.amplitude = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.phase = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.phaseSpeed = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            part.rot[0] = RANDOM_U16() * inv65535 * 360.0f;
            part.rot[1] = RANDOM_U16() * inv65535 * 360.0f;
            part.rot[2] = RANDOM_U16() * inv65535 * 360.0f;
            part.unk40 = RANDOM_U16() * inv65535 * 1.600000024f;
            part.unk48 = RANDOM_U16() * inv65535 * 1.600000024f;
            part.scaleX = part.scaleY = RANDOM_U16() * inv65535 * 0.25f + 1.0f;
            func_1510F800(0);
            part.unk5C = func_1510FD20((s32)part.posX, (s32)part.posZ);
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
        Particle15132A4C part;
        f32 top;
        f32 bottom;

        part.unk00 = 1.0f;
        part.unk04 = 1.0f;
        part.unk1C[0] = 1.0f;
        part.unk1C[1] = 1.0f;
        part.unk1C[2] = 1.0f;
        part.flags = 0x4D40;
        part.unk54 = 300;
        part.unk58 = 0;
        part.unk60[0] = 0xFF;
        part.unk60[1] = 14;
        part.unk60[2] = 0;
        part.unk60[3] = 0;
        part.unk60[4] = 0;
        part.unk60[5] = 0;
        part.unk60[6] = 0;
        part.unk60[7] = 0;
        part.unk60[8] = 2;
        part.unk6A = 0;
        part.unk6C = 0;
        part.unk70 = 0;
        part.unk72 = 1;
        part.unk74 = 0xFF;
        inv65535 = 1.525902189e-05f;
        part.unk44 = 0.0f;
        do {
            s32 pad2; // matching: stack slot
            const u16 types[14] = { 0x76, 0x77, 0x78, 0x79, 0x53, 0x3E, 0x3D, 0x39, 0x02, 0x03, 0x4E, 0x4F, 0x50, 0x51 };
            f32 range; // matching: reused for the distance to the nearer bound
            f32 y;
            f32 below;

            func_15008340(region, &part.posX, &part.posZ, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.centerY = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.posY = y;
                below = fabsf(y - bottom);
                range = (below < above) ? below : above;
                part.amplitude = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.phase = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.phaseSpeed = RANDOM_U16() * inv65535 * 0.01300000027f + 0.005000000354f;
            part.rot[0] = RANDOM_U16() * inv65535 * 360.0f;
            part.rot[1] = RANDOM_U16() * inv65535 * 360.0f;
            part.rot[2] = RANDOM_U16() * inv65535 * 360.0f;
            part.unk40 = RANDOM_U16() * inv65535 * 1.600000024f;
            part.unk48 = RANDOM_U16() * inv65535 * 1.600000024f;
            part.type = types[RANDOM() % 14];
            part.scaleX = part.scaleY = RANDOM_U16() * inv65535 * 0.25f + 0.5f;
            if (part.type == 0x39) {
                part.scaleX *= 0.349999994f;
                part.scaleY *= 0.349999994f;
            }
            func_1510F800(0);
            part.unk5C = func_1510FD20((s32)part.posX, (s32)part.posZ);
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
        Particle15130280 part;
        Particle15130280Extra extra;
        void *obj;
        f32 top;
        f32 bottom;
        f32 range; // matching: reused for the distance to the nearer bound
        f32 y;
        f32 temp; // random size, then distance to the bottom

        minFrac = 0.3000000119f;
        part.unk08 = 0x5103;
        part.unk00 = 0x200005;
        part.unk17 = 0xFF;
        part.unk1C = 0xFF;
        fracRange = 0.150000006f;
        part.unk04 = 0;
        part.unk0C = 0;
        part.unk10 = 0;
        part.unk15 = 0;
        part.unk16 = 0;
        part.unk19 = 0;
        part.unk1A = 0;
        part.unk3C = 0.0f;
        part.unk40 = 0.0f;
        part.unk44 = 0.0f;
        part.unk22 = 1;
        part.unk24 = 1.0f;
        part.unk0A = 300;
        part.unk1E = 1;
        part.unk20 = 0xFF;
        part.unk60 = 8;
        part.unk61 = 6;
        part.unk62 = 23;
        part.unk63 = -1;
        part.unk64 = -1;
        part.unk65 = 0;
        extra.unk04 = 0;
        extra.unk05 = 0;
        extra.unk10 = 1.0f;
        extra.unk14 = 1.0f;
        part.unk1D = 108;
        inv65535 = 1.525902189e-05f;
        do {
            temp = RANDOM_U16() * inv65535 * 600.0f + 200.0f;
            part.sizeY = temp;
            part.sizeX = temp;
            extra.size = temp;
            part.unk1B = RANDOM() % 71 + 50;
            extra.unk06 = RANDOM() % 3;
            extra.unk07 = RANDOM() % 3;
            extra.unk08 = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            extra.unk0C = (RANDOM_U16() * inv65535 * fracRange + minFrac) * extra.size;
            part.flags = ((func_150ADA20() & 1) ? 0x40 : 0) | ((func_150ADA20() & 1) ? 0x80 : 0) | 0x4000 |
                         0x8000 | 0x20000 | 0x800000;
            part.unk18 = RANDOM() % 56 + 80;
            part.unk14 = RANDOM() % 46 + 180;
            func_15008340(region, &part.posX, &part.posZ, &top, &bottom);
            range = top - bottom;
            y = RANDOM_U16() * inv65535 * (range * 0.5f) + (bottom + range * 0.25f);
            part.centerY = y;
            {
                f32 above;

                above = fabsf(y - top);
                part.posY = y;
                temp = fabsf(y - bottom);
                range = (temp < above) ? temp : above;
                part.amplitude = (RANDOM_U16() * inv65535 * 0.75f + 0.25f) * range;
            }
            part.phase = RANDOM_U16() * inv65535 * 2.0f * 3.141592741f;
            part.phaseSpeed = RANDOM_U16() * inv65535 * 0.01000000071f + 0.002000000095f;
            func_1510F800(0);
            part.unk5C = func_1510FD20((s32)part.posX, (s32)part.posZ);
            obj = func_15130280(&part, 2, 0, 28, 255, 1);
            if (obj != NULL) {
                memcpy((u8 *)obj + 0xA8, &extra, sizeof(extra));
            }
            count -= 1.0f;
        } while (count > 1.0f);
    }
}
