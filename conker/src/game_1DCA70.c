#include <ultra64.h>
/* functions.h types func_1513D668 as void; func_151AF6D4 uses the particle it returns. */
#define func_1513D668 func_1513D668_void_proto
#include "functions.h"
#undef func_1513D668
#include "variables.h"

extern f32 D_800AA0F0[];
extern f32 D_800AA0FC[];


/* Growth applied by func_151AFC08, stored in the spray particle at 0x128. */
typedef struct {
    /* 0x00 */ s16 growEndTimer; // grows while desc.life is above this
    /* 0x04 */ f32 growRate;     // size gained per frame
} SprayGrowth;

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

/*
 * Transforms the fixed offset for `side` (1 or 2) by that side's bone matrix of the actor
 * (bone 32 or 25) into `pos`.
 * If `ground` is non-NULL, also probes the ground below that point into it.
 */
s32 func_151AF5C0(f32 *pos, struct127 *actor, GroundHit *ground, u8 side) {
    f32 probe[3];
    f32 *offset;
    Mtx *bone;

    if ((side != 1) && (side != 2)) {
        return 0;
    }

    if (side == 1) {
        offset = D_800AA0FC;
    } else {
        offset = D_800AA0F0;
    }

    if (side == 1) {
        bone = (Mtx *)actor->unk1D4 + 32;
    } else {
        bone = (Mtx *)actor->unk1D4 + 25;
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

s32 func_151AF6C0(s32 arg0, s32 arg1) {
    return 12;
}

/* One corner colour passed to func_1513D668. */
typedef struct {
    s16 r;
    s16 g;
    s16 b;
    s16 a;
    s16 unk8;
} SprayColor;

/* Particle descriptor passed to func_1513D668. */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 life;     // counts down
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
    /* 0x44 */ u8 alpha;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D[3];
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} SprayDesc; /* size 0x58 */

/* The spray particle func_151AF6D4 spawns; updated by func_151AFC08. */
typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ SprayDesc desc; // copy of the spawn descriptor
    /* 0x070 */ u8 pad70[0xB8];
    /* 0x128 */ SprayGrowth growth;
} SprayParticle;

void func_150CCD90(f32, f32 *, f32 *, f32 *);
void func_1514C678(f32 x, f32 y, f32 z, f32 radius, s16 arg4, s16 arg5, s16 arg6, u8 arg7, s32 arg8,
                   f32 arg9, u8 *rgb, u8 argB);
SprayParticle *func_1513D668(SprayDesc *desc, SprayColor *colors, u8 updateFn1, u8 updateFn2, u8 expireFn,
                             u8 flip, s16 angle, f32 width, f32 height, s32 arg9, f32 *argA, u8 argB,
                             s32 motionSize, u8 argD, s32 argE);

/* Spawns a spray particle and a puff at the emission point on the actor's `side`. */
void func_151AF6D4(struct127 *actor, s32 side, s32 arg2) {
    f32 pos[3];
    GroundHit ground;
    f32 fr;
    f32 fg;
    f32 fb;
    SprayParticle *particle;
    SprayGrowth growth;
    SprayDesc desc;
    s32 lifeRoll; // matching: IDO schedules the first roll differently when it is inlined
    SprayColor colors[4];
    s16 roll;
    u8 r;
    u8 g;
    u8 b;

    if (actor == NULL) {
        return;
    }
    if (actor->unk1D4 == NULL) {
        return;
    }
    if (func_151AF5C0(pos, actor, &ground, side) == 0) {
        return;
    }
    if (D_800BE9F0 == 20) {
        func_150CCD90(pos[2], &fr, &fg, &fb);
        r = fr;
        g = fg;
        b = fb;
    } else {
        r = g = b = 0xFF;
    }
    colors[0].r = r;
    colors[0].g = g;
    colors[0].b = b;
    colors[0].a = 180;
    colors[0].unk8 = 0;
    colors[1].r = r;
    colors[1].g = g;
    colors[1].b = b;
    colors[1].a = 180;
    colors[1].unk8 = 0;
    colors[2].r = r;
    colors[2].g = g;
    colors[2].b = b;
    colors[2].a = 180;
    colors[2].unk8 = 0;
    colors[3].r = r;
    colors[3].g = g;
    colors[3].b = b;
    colors[3].a = 180;
    colors[3].unk8 = 0;
    desc.unk0 = 0x50;
    desc.unk2 = 0x2502;
    lifeRoll = func_150ADA20();
    desc.life = (lifeRoll % 61U) + 88;
    desc.unk8 = 0;
    desc.unkC = 0;
    desc.unk10 = 0xFF;
    desc.unk11 = 0xFF;
    desc.unk12 = 0xFF;
    desc.unk13 = 0xFF;
    desc.size0 = desc.size1 = ((func_150ADA68() * 21.0f) + 91.0f) * 0.001f;
    desc.pos.unk0 = pos[0];
    desc.pos.unk4 = ground.height;
    desc.pos.unk8 = pos[2];
    desc.unk28 = 0.0f;
    desc.unk2C = 0.0f;
    desc.unk30 = 0.0f;
    desc.unk34 = 1.0f;
    desc.unk38 = 1.0f;
    desc.unk3C = 1.0f;
    desc.flags = 0x401C0001;
    desc.alpha = (func_150ADA20() % 41U) + 80;
    desc.unk45 = 0xFF;
    growth.growEndTimer = (desc.life - (func_150ADA20() % 6U)) - 8;
    growth.growRate = (((func_150ADA68() * 40.0f) + 51.0f) * desc.size0) * 0.001f;
    desc.unk46 = 0;
    desc.unk47 = 6;
    particle = func_1513D668(&desc, colors, 0, 0x16, 0, 0, (actor->unk7A >> 8) - ((side == 1) ? -10 : 10),
                             500.0f, 500.0f, 0, ground.unk04, 1, 8, 0xFF, 0);
    if (particle != NULL) {
        memcpy(&particle->growth, &growth, sizeof(growth));
    }
    roll = (func_150ADA20() % 11U) + 5;
    func_1514C678(pos[0], pos[1], pos[2], (func_150ADA68() * 17.0f) + 29.0f,
                  0, 0xFF, roll, 16, 0, 0.0f, NULL, 0xFF);
}

/* Particle updated by func_151AFBD4. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s16 timer;
    /* 0x1E */ u8 pad1E[0xA];
    /* 0x28 */ u8 alpha;
} FadeParticle;

/* Fades the particle out over the last 32 frames of its timer. */
s32 func_151AFBD4(FadeParticle *particle) {
    s32 fade;

    if (particle->timer < 32) {
        fade = particle->timer * 8;
        if (fade < particle->alpha) {
            particle->alpha = fade;
        }
    }
    return 1;
}

/*
 * Update for the func_151AF6D4 spray: once flag 1 is set fades it out over the last 32 frames
 * of its timer, and grows it until the timer reaches growth.growEndTimer.
 */
/*
 * Update for the func_151AF6D4 spray: once flag 1 is set, fades it out over the last 32 frames
 * of its life and grows it until life drops to growth.growEndTimer.
 */
s32 func_151AFC08(SprayParticle *spray) {
    SprayDesc *desc;
    SprayGrowth *growth;

    if (spray->desc.flags & 1) {
        desc = &spray->desc;
        growth = &spray->growth;
        if (spray->desc.life < 32) {
            if (spray->desc.life * 8 < spray->desc.alpha) {
                // matching: a store through a second pointer makes IDO reload life afterwards
                desc->alpha = spray->desc.life * 8;
            }
        }
        if (growth->growEndTimer < spray->desc.life) {
            f32 grow = growth->growRate * D_800BE9A4;

            spray->desc.size0 = spray->desc.size0 + grow;
            spray->desc.size1 = spray->desc.size1 + grow;
        }
    }
    return 1;
}

/* Spark descriptor passed to func_15130374 (same layout as game_100810's SparkSpawn). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 life;
    /* 0x0C */ s32 unkC;
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
    /* 0x1D */ u8 kind;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 size0;
    /* 0x2C */ f32 size1;
    /* 0x30 */ f32 pos[3];
    /* 0x3C */ f32 unk3C[3];
    /* 0x48 */ f32 vel[3];
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 flags;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ u8 pad64[0xC];
} SparkDesc; /* size 0x70 */

/* The spark func_151AFC88 spawns; updated by func_151AFEA4. */
typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ f32 unk58;
    /* 0x5C */ u8 pad5C[4];
    /* 0x60 */ f32 unk60;
    /* 0x64 */ u8 pad64[0x44];
    /* 0xA8 */ f32 damping; // per-frame factor applied to unk58 and unk60
} Spark;

Spark *func_15130374(SparkDesc *desc, u8 arg1, s32 arg2, u8 arg3, s32 arg4);
void func_15143794(s16 yaw, s16 pitch, f32 speed, f32 *vel);

/* Spawns one spark at (x, y, z), thrown in a random direction around `yaw`. */
s32 func_151AFC88(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    Spark *spark;
    SparkDesc desc;
    s32 pad1; // matching: unused stack slot
    f32 damping;
    s32 pad2; // matching: unused stack slot
    s32 roll;

    desc.kind = 0x29;
    desc.unk8 = 0xE03;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.life = (func_150ADA20() & 0xF) + 25;
    desc.unkC = 0;
    desc.unk10 = 0;
    desc.unk18 = 0xB0;
    desc.unk19 = 0xA0;
    desc.unk1A = 0x2A;
    desc.unk14 = 0x40;
    desc.unk15 = 0xB;
    desc.unk16 = 0x6A;
    desc.unk17 = 0xFF;
    desc.unk1B = (func_150ADA20() % 156U) + 100;
    desc.unk1C = 0xFF;
    desc.unk60 = 3;
    desc.unk61 = 3;
    desc.size0 = desc.size1 = (func_150ADA68() * 202.0f) + 101.0f;
    desc.pos[0] = x;
    desc.pos[1] = y;
    desc.pos[2] = z;

    roll = func_150ADA20();
    func_15143794(yaw, (roll % 21U) - 20, (func_150ADA68() * 301.0f + 500.0f) * 0.01f, desc.vel);

    desc.flags = 0xE05;
    desc.unk54 = 0.0f;

    if (func_150ADA20() & 1) {
        desc.flags |= 0x40;
    }

    if (func_150ADA20() & 1) {
        desc.flags |= 0x80;
    }

    desc.unk62 = 6;
    desc.unk24 = 1.04f;
    damping = 0.9490000606f;
    desc.unk63 = -1;
    desc.unk1E = 15;
    desc.unk20 = 17;
    desc.unk22 = 25;

    spark = func_15130374(&desc, 1, 4, arg14, 1);
    if (spark != NULL) {
        memcpy(&spark->damping, &damping, sizeof(damping));
    }
    return 1;
}

/* Damps the spark's unk58 and unk60 by its damping factor once per elapsed frame. */
s32 func_151AFEA4(Spark *spark, s32 arg1) {
    f32 *damping;
    s32 frames;

    damping = &spark->damping;
    for (frames = D_800BE9E4; frames != 0; frames--) {
        spark->unk58 *= *damping;
        spark->unk60 *= *damping;
    }
    return 1;
}
