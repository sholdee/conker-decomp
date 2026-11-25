#include <ultra64.h>
/* functions.h types func_1513D668 as void; func_150CB1F4 uses the particle it returns. */
#define func_1513D668 func_1513D668_void_proto
#include "functions.h"
#undef func_1513D668
#include "variables.h"

extern f32 D_800A05E0[];
extern f32 D_800A05EC[];
extern f32 D_800A0604;
extern f32 D_800A0608;
extern f32 D_800A060C;
extern f32 D_800A0610;
extern f32 D_800A0614;
extern f32 D_800A0618;
extern f32 D_800A061C;
extern f32 D_800A0620;

/* Ground probe result filled by func_1504715C / func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ f32 unk04[6];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 pad1D[7];
} GroundHit; /* size 0x24 */

void func_15143134(f32 *, f32 *, Mtx *);
void func_1504715C(GroundHit *, struct127 *);
s32 func_15046C80(f32 *, s32, f32, GroundHit *);
void func_15143794(s16, s16, f32, f32 *);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);

/*
 * Transforms the offset for foot `side` (1 or 2) by the actor's bone matrix into `pos`.
 * If `ground` is non-NULL, also probes the ground below that point into it.
 */
s32 func_150CB0E0(f32 *pos, struct127 *actor, GroundHit *ground, u8 side) {
    f32 probe[3];
    f32 *offset;
    Mtx *bone;

    if ((side != 1) && (side != 2)) {
        return 0;
    }

    if (side == 1) {
        offset = D_800A05EC;
    } else {
        offset = D_800A05E0;
    }

    if (side == 1) {
        bone = (Mtx *)actor->unk1D4 + 40;
    } else {
        bone = (Mtx *)actor->unk1D4 + 47;
    }

    func_15143134(offset, pos, bone);
    if (ground == NULL) {
        return 1;
    }

    probe[0] = pos[0];
    probe[1] = pos[1] + 100.0f;
    probe[2] = pos[2];
    func_1504715C(ground, actor);
    return func_15046C80(probe, 0, pos[1] - 500.0f, ground);
}

s32 func_150CB1E0(s32 arg0, s32 arg1) {
    return 0xB;
}

/* Growth applied by func_150CBA30, stored in the dust particle at 0x128. */
typedef struct {
    /* 0x00 */ s16 growTime; // frames left to grow
    /* 0x04 */ f32 growRate; // size gained per frame
} DustGrowth;

/* The dust particle func_150CB1F4 spawns; updated by func_150CBA30. */
typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 timer;
    /* 0x01E */ u8 pad1E[0xE];
    /* 0x02C */ f32 size0;
    /* 0x030 */ f32 size1;
    /* 0x034 */ u8 pad34[0x24];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0xCB];
    /* 0x128 */ DustGrowth growth;
} DustParticle;

/* One corner colour passed to func_1513D668. */
typedef struct {
    s16 r;
    s16 g;
    s16 b;
    s16 a;
    s16 unk8;
} DustColor;

/* Particle descriptor passed to func_1513D668. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 life;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ f32 size0;
    /* 0x18 */ f32 size1;
    /* 0x1C */ struct17 pos;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} DustDesc; /* size 0x58 */

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} Rgb8;

extern f32 D_800A05F8;
extern f32 D_800A05FC;
extern f32 D_800A0600;
void func_150CCD90(f32, f32 *, f32 *, f32 *);
void func_1514C678(f32 x, f32 y, f32 z, f32 radius, s16 arg4, s16 arg5, s16 arg6, u8 arg7, s32 arg8,
                   f32 arg9, Rgb8 *argA, u8 argB);
s32 func_1518ABD0(s32, DustParticle *, s32);
DustParticle *func_1513D668(DustDesc *desc, DustColor *colors, u8 updateFn1, u8 updateFn2, u8 expireFn,
                            u8 flip, s16 angle, f32 width, f32 height, s32 arg9, f32 *argA, u8 argB,
                            s32 motionSize, u8 argD, s32 argE);

/* Spawns a puff of ground dust (tinted by func_150CCD90) under the actor's foot `side`. */
void func_150CB1F4(struct127 *actor, s32 side, s32 arg2) {
    f32 pos[3];
    GroundHit ground;
    f32 fr;
    f32 fg;
    f32 fb;
    u8 r;
    u8 g;
    u8 b;
    s32 angleOffset;
    DustGrowth growth;
    DustDesc desc;
    DustParticle *particle;

    if (actor->unk1D4 == NULL) {
        return;
    }
    if (func_150CB0E0(pos, actor, &ground, side) == 0) {
        return;
    }
    func_1512D748(&D_800DBFF0[D_800BE9E8], 7, 1);
    func_151D5404((struct17 *)pos, 1502.0f, 3000.0f, 0.0003333333298f, 12, 15, 0xFF, 0);
    func_150CCD90(pos[2], &fr, &fg, &fb);
    r = fr;
    g = fg;
    b = fb;
    {
        DustColor colors[4];
        s16 roll;
        s16 roll2;
        Rgb8 rgb;

        colors[0].r = r;
        colors[0].g = g;
        colors[0].b = b;
        colors[0].a = 0xFF;
        colors[0].unk8 = 0;
        colors[1].r = r;
        colors[1].g = g;
        colors[1].b = b;
        colors[1].a = 0xFF;
        colors[1].unk8 = 0;
        colors[2].r = r;
        colors[2].g = g;
        colors[2].b = b;
        colors[2].a = 0xFF;
        colors[2].unk8 = 0;
        colors[3].r = r;
        colors[3].g = g;
        colors[3].b = b;
        colors[3].a = 0xFF;
        colors[3].unk8 = 0;
        if (side == 1) {
            desc.unk0 = 0x4C;
        } else {
            desc.unk0 = 0x4B;
        }
        desc.unk2 = 0x2603;
        desc.life = 2;
        desc.unk8 = 0;
        desc.unkC = 0;
        desc.unk10 = 0xFF;
        desc.unk11 = 0xFF;
        desc.unk12 = 0xFF;
        desc.unk13 = 0xFF;
        desc.size0 = desc.size1 = ((func_150ADA68() * 50.0f) + D_800A05F8) * D_800A05FC;
        desc.pos.unk0 = pos[0];
        desc.pos.unk4 = ground.height + 10.0f;
        desc.pos.unk8 = pos[2];
        desc.unk28 = 0.0f;
        desc.unk2C = 0.0f;
        desc.unk30 = 0.0f;
        desc.unk34 = 1.0f;
        desc.unk38 = 1.0f;
        desc.unk3C = 1.0f;
        desc.flags = 0x401C0000;
        desc.unk44 = (func_150ADA20() % 51U) + 160;
        desc.unk45 = 0xFF;
        growth.growTime = (func_150ADA20() & 3) + 7;
        growth.growRate = (((func_150ADA68() * 30.0f) + 30.0f) * desc.size0) * D_800A0600;
        desc.unk46 = 0;
        desc.unk47 = 6;
        angleOffset = (side == 1) ? -8 : 8;
        particle = func_1513D668(&desc, colors, 0, 0x17, 0, 0, (actor->unk7A >> 8) - angleOffset,
                                 500.0f, 500.0f, 0, ground.unk04, 1, 8, 0xFF, 0);
        if (particle != NULL) {
            memcpy(&particle->growth, &growth, sizeof(growth));
            if (func_1518ABD0(D_800D98E0, particle, 2) == 0) {
                func_1516972C((struct102 *)particle);
            }
        }
        roll = (func_150ADA20() & 0xF) + 10;
        func_1514C678(pos[0], pos[1], pos[2], (func_150ADA68() * 59.0f) + 80.0f,
                      0, 0xFF, roll, 14, 0, 0.0f, NULL, 0xFF);
        roll2 = (func_150ADA20() & 0xF) + 5;
        rgb.r = r;
        rgb.g = g;
        rgb.b = b;
        func_1514C678(pos[0], pos[1], pos[2], (func_150ADA68() * 59.0f) + 70.0f,
                      0, 0xFF, roll2, 15, 0, 0.0f, &rgb, 0xFF);
    }
}

/* Particle updated by func_150CB7CC. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 timer;
    /* 0x1E */ u8 pad1E[0xA];
    /* 0x28 */ u8 alpha;
} FadeParticle;

/* Fades the particle out over the last 32 frames of its timer. */
s32 func_150CB7CC(FadeParticle *particle) {
    s32 fade;

    if (particle->timer < 32) {
        fade = particle->timer * 8;
        if (fade < particle->alpha) {
            particle->alpha = fade;
        }
    }
    return 1;
}

/* Particle descriptor passed to func_15132A4C. */
typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10[3];
    /* 0x1C */ f32 unk1C[3];
    /* 0x28 */ f32 pos[3];
    /* 0x34 */ f32 vel[3];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8 pad76[0xA];
} ChunkDesc; /* size 0x80 */

/* Spawns one func_15132A4C particle at (x, y, z), thrown at a random speed and pitch toward `angle`. */
s32 func_150CB800(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle,
                  s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14) {
    ChunkDesc desc;
    f32 fraction;
    u32 random;

    desc.pos[0] = x;
    desc.pos[1] = y;
    desc.unk1C[0] = 1.0f;
    desc.unk1C[1] = 1.0f;
    desc.unk1C[2] = 1.0f;
    desc.unk50 = 0x29E8;
    desc.unk10[0] = 0.0f;
    desc.unk10[1] = 0.0f;
    desc.unk10[2] = 0.0f;
    desc.unk44 = 0.0f;
    desc.unk00 = 1.0f;
    desc.unk56 = 32;
    desc.pos[2] = z;
    desc.unk04 = D_800A0604;

    random = func_150ADA20();
    fraction = func_150ADA68();
    func_15143794(angle, (random % 36U) - 55, (fraction * 200.0f + 200.0f) * D_800A0608, desc.vel);
    desc.unk40 = (func_150ADA68() * 300.0f + -149.0f) * D_800A060C;
    desc.unk48 = (func_150ADA68() * 300.0f + -149.0f) * D_800A0610;
    desc.unk54 = (func_150ADA20() % 33U) + 32;
    desc.unk4C = (func_150ADA68() * D_800A0614 + D_800A0618) * D_800A061C;
    desc.unk0C = (func_150ADA68() * 300.0f + 101.0f) * D_800A0620;
    desc.unk58 = 0;
    desc.unk5C = 0;
    desc.unk08 = desc.unk0C;
    desc.unk60 = (func_150ADA20() % 101U) + 155;
    desc.unk61 = 2;
    desc.unk62 = 0;
    desc.unk63 = 0;
    desc.unk64 = 0;
    desc.unk65 = 0;
    desc.unk66 = 0;
    desc.unk67 = 0;
    desc.unk68 = 0;
    desc.unk6A = 2;
    desc.unk6C = 0;
    desc.unk70 = 0;
    desc.unk72 = 32;
    desc.unk74 = 7;
    func_15132A4C(&desc, 3, 0xFF, 0, arg14, 0);
    return 1;
}

/*
 * Update for the func_150CB1F4 dust: grows it while growTime lasts, and once flag 1 is set
 * fades it out over the last 32 frames of its timer.
 */
s32 func_150CBA30(DustParticle *dust) {
    dust->growth.growTime -= D_800BE9E4;
    if (dust->growth.growTime > 0) {
        f32 grow = dust->growth.growRate * D_800BE9A4;

        dust->size0 = dust->size0 + grow;
        dust->size1 = dust->size1 + grow;
    }
    if (dust->flags & 1) {
        if (dust->timer < 32) {
            if (dust->timer * 8 < dust->alpha) {
                dust->alpha = dust->timer * 8;
            }
        }
    }
    return 1;
}

/* Particle spawned by func_150CBABC; updated by func_150CBCE0. */
typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ f32 unk58;
    /* 0x5C */ u8 pad5C[4];
    /* 0x60 */ f32 unk60;
    /* 0x64 */ u8 pad64[0x44];
    /* 0xA8 */ f32 damping; // per-frame-step factor applied to unk58 and unk60
} SparkParticle;

/* Particle descriptor passed to func_15130374. */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 kind;
    /* 0x0A */ s16 duration;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 color0[4];
    /* 0x18 */ u8 color1[4];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 size0;
    /* 0x2C */ f32 size1;
    /* 0x30 */ f32 pos[3];
    /* 0x3C */ u8 pad3C[0xC];
    /* 0x48 */ f32 vel[3];
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 flags58;
    /* 0x5C */ u8 pad5C[4];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ u8 pad64[0xC];
} SparkDesc; /* size 0x70 */

SparkParticle *func_15130374(SparkDesc *, u8, s32, u8, s32);

/*
 * Spawns one func_15130374 particle at (x, y, z), thrown at a random speed and pitch toward
 * `angle`; its velocity is damped by func_150CBCE0.
 */
s32 func_150CBABC(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle,
                  s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    SparkParticle *particle;
    SparkDesc desc;
    f32 size;
    f32 damping;
    f32 fraction;
    u32 random;

    desc.unk1D = 0x29;
    desc.kind = 0xE03;
    desc.flags = 0x200005;
    desc.unk04 = 0;
    desc.duration = (func_150ADA20() % 41U) + 40;
    desc.unk0C = 0;
    desc.unk10 = 0;
    desc.color1[0] = 0xB0;
    desc.color1[1] = 0xA0;
    desc.color1[2] = 0x2A;
    desc.color0[0] = 0x40;
    desc.color0[1] = 0x0B;
    desc.color0[2] = 0x6A;
    desc.color0[3] = 0xFF;
    desc.color1[3] = (func_150ADA20() % 157U) + 100;
    desc.unk1C = 0xFF;
    desc.unk60 = 3;
    desc.unk61 = 3;
    size = (func_150ADA68() * 401.0f) + 300.0f;
    desc.size0 = size;
    desc.size1 = size;
    desc.pos[0] = x;
    desc.pos[1] = y;
    desc.pos[2] = z;

    random = func_150ADA20();
    fraction = func_150ADA68();
    func_15143794(angle, (random % 26U) - 25, ((fraction * 500.0f) + 1000.0f) * 0.01f, desc.vel);
    desc.flags58 = 0xE05;
    desc.unk54 = 0.0f;

    if (func_150ADA20() & 1) {
        desc.flags58 |= 0x40;
    }

    if (func_150ADA20() & 1) {
        desc.flags58 |= 0x80;
    }

    desc.unk62 = 7;
    desc.unk63 = -1;
    desc.unk1E = 25;
    desc.unk20 = 10;
    desc.unk22 = 60;
    desc.unk24 = 1.02f;
    damping = 0.962000072f;

    particle = func_15130374(&desc, 1, sizeof(damping), argE, 1);
    if (particle != NULL) {
        memcpy(&particle->damping, &damping, sizeof(damping));
    }
    return 1;
}

/* Multiplies unk58 and unk60 by the particle's damping once per frame step. */
s32 func_150CBCE0(SparkParticle *particle, s32 arg1) {
    s32 i;
    f32 *damping;

    damping = &particle->damping; // matching: read through a pointer, IDO reloads it after every store
    for (i = D_800BE9E4; i != 0; i--) {
        particle->unk58 *= *damping;
        particle->unk60 *= *damping;
    }
    return 1;
}

/* Particle moved by func_150CBE88. */
typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 unk20[3]; // integrated from unk50
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ f32 pos[3];
    /* 0x44 */ f32 vel[3];
    /* 0x50 */ f32 unk50[3];
    /* 0x5C */ f32 gravity;  // added to vel[1]
} BallisticParticle;

/* Advances the particle one frame step under constant vertical acceleration. */
s32 func_150CBE88(BallisticParticle *particle) {
    particle->pos[0] += particle->vel[0] * D_800BE9A4;
    particle->pos[1] += (particle->vel[1] * D_800BE9A4) + (((particle->gravity * D_800BE9A4) * D_800BE9A4) * 0.5f);
    particle->pos[2] += particle->vel[2] * D_800BE9A4;
    particle->vel[1] += particle->gravity * D_800BE9A4;
    particle->unk20[0] += particle->unk50[0] * D_800BE9A4;
    particle->unk20[1] += particle->unk50[1] * D_800BE9A4;
    particle->unk20[2] += particle->unk50[2] * D_800BE9A4;
    return 1;
}

/* Particle handled by func_150CBF5C. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 timer;
    /* 0x1E */ u8 pad1E[0x3A];
    /* 0x58 */ s32 flags;
} FadeoutParticle;

/* Sets flag 1 and a 32-frame timer (the fade-out func_150CBA30 runs on its dust). */
void func_150CBF5C(FadeoutParticle *particle) {
    particle->flags |= 1;
    particle->timer = 32;
}
