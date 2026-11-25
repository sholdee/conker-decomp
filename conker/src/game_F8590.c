#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

extern f32 D_800A05E0[];
extern f32 D_800A05EC[];

/* Ground probe result filled by func_1504715C / func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ f32 unk04[6];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 pad1D[7];
} GroundHit; /* size 0x24 */

void func_15143134(f32 *, f32 *, Mtx *);
void func_1504715C(GroundHit *, Actor *);
s32 func_15046C80(f32 *, s32, f32, GroundHit *);
void func_15143794(s16, s16, f32, f32 *);

/*
 * Transforms the fixed offset for `side` (1 or 2) by that side's bone matrix of the actor
 * (bone 40 or 47) into `pos`.
 * If `ground` is non-NULL, also probes the ground below that point into it.
 */
s32 func_150CB0E0(f32 *pos, Actor *actor, GroundHit *ground, u8 side) {
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

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} Rgb8;

void func_150CCD90(f32, f32 *, f32 *, f32 *);
void func_1514C678(f32 x, f32 y, f32 z, f32 radius, s16 arg4, s16 arg5, s16 arg6, u8 arg7, s32 arg8,
                   f32 arg9, Rgb8 *argA, u8 argB);
s32 func_1518ABD0(s32, DustParticle *, s32);

/* Spawns a dust puff (tinted by func_150CCD90) on the ground below the actor's bone on `side`. */
void func_150CB1F4(Actor *actor, s32 side, s32 arg2) {
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
    ParticleDef desc;
    DustParticle *particle;

    if (actor->unk1D4 == NULL) {
        return;
    }
    if (func_150CB0E0(pos, actor, &ground, side) == 0) {
        return;
    }
    func_1512D748(&D_800DBFF0[D_800BE9E8], 7, 1);
    func_151D5404((Vec3f *)pos, 1502.0f, 3000.0f, 1.0f / 3000.0f, 12, 15, 0xFF, 0);
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
            desc.texture = 0x4C;
        } else {
            desc.texture = 0x4B;
        }
        desc.renderModes = 0x2603;
        desc.lifetime = 2;
        desc.frame = 0;
        desc.frameSpeed = 0;
        desc.rgba[0] = 0xFF;
        desc.rgba[1] = 0xFF;
        desc.rgba[2] = 0xFF;
        desc.rgba[3] = 0xFF;
        desc.width = desc.height = ((func_150ADA68() * 50.0f) + 325.0f) * 0.001f;
        desc.pos.x = pos[0];
        desc.pos.y = ground.height + 10.0f;
        desc.pos.z = pos[2];
        desc.rot.x = 0.0f;
        desc.rot.y = 0.0f;
        desc.rot.z = 0.0f;
        desc.scale.x = 1.0f;
        desc.scale.y = 1.0f;
        desc.scale.z = 1.0f;
        desc.flags = 0x401C0000;
        desc.unk44 = (func_150ADA20() % 51U) + 160;
        desc.unk45 = 0xFF;
        growth.growTime = (func_150ADA20() & 3) + 7;
        growth.growRate = (((func_150ADA68() * 30.0f) + 30.0f) * desc.width) * 0.001f;
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

/* matching: ParticleSpawn plus 0x4 bytes; the stack frame needs the larger size. */
typedef struct ParticleSpawn80 {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 elasticity;
    /* 0x08 */ f32 sizeXZ;
    /* 0x0C */ f32 sizeY;
    /* 0x10 */ Vec3f rot;
    /* 0x1C */ Vec3f scale;
    /* 0x28 */ Vec3f pos;
    /* 0x34 */ Vec3f vel;
    /* 0x40 */ Vec3f rotVel;
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
    /* 0x78 */ s32 sound;
    /* 0x7C */ u8 pad7C[0x4];
} ParticleSpawn80;

s32 func_15132A4C(ParticleSpawn80 *, s32, s32, s32, u8, s32);

/* Spawns one func_15132A4C particle at (x, y, z), its velocity built by func_15143794 from `angle`. */
s32 func_150CB800(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle,
                  s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13, u8 arg14) {
    ParticleSpawn80 desc;
    f32 fraction;
    u32 random;

    desc.pos.x = x;
    desc.pos.y = y;
    desc.scale.x = 1.0f;
    desc.scale.y = 1.0f;
    desc.scale.z = 1.0f;
    desc.flags = 0x29E8;
    desc.rot.x = 0.0f;
    desc.rot.y = 0.0f;
    desc.rot.z = 0.0f;
    desc.rotVel.y = 0.0f;
    desc.radius = 1.0f;
    desc.model = 32;
    desc.pos.z = z;
    desc.elasticity = 0.2f;

    random = func_150ADA20();
    fraction = func_150ADA68();
    func_15143794(angle, (random % 36U) - 55, (fraction * 200.0f + 200.0f) * 0.1f, &desc.vel.x);
    desc.rotVel.x = (func_150ADA68() * 300.0f + -149.0f) * 0.1f;
    desc.rotVel.z = (func_150ADA68() * 300.0f + -149.0f) * 0.1f;
    desc.lifetime = (func_150ADA20() % 33U) + 32;
    desc.gravity = (func_150ADA68() * 603.0f + -2402.0f) * 0.001f;
    desc.sizeY = (func_150ADA68() * 300.0f + 101.0f) * 0.001f;
    desc.variant = 0;
    desc.unk5C = 0;
    desc.sizeXZ = desc.sizeY;
    desc.alpha = (func_150ADA20() % 101U) + 155;
    desc.updateFn = 2;
    desc.update2Fn = 0;
    desc.floorHitFn = 0;
    desc.floorType3Fn = 0;
    desc.wallHitFn = 0;
    desc.destroyFn = 0;
    desc.squashFn = 0;
    desc.matrixFn = 0;
    desc.unk6A = 2;
    desc.owner = 0;
    desc.ownerId = 0;
    desc.fadeStart = 32;
    desc.fadeRate = 7;
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

SparkParticle *func_15130374(ParticleDesc *, u8, s32, u8, s32);

/*
 * Spawns one func_15130374 particle at (x, y, z), its velocity built by func_15143794 from
 * `angle`. It stores the damping factor that func_150CBCE0 applies each frame step.
 */
s32 func_150CBABC(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7, s16 angle,
                  s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    SparkParticle *particle;
    ParticleDesc desc;
    f32 size;
    f32 damping;
    f32 fraction;
    u32 random;

    desc.texture = 0x29;
    desc.renderMode = 0xE03;
    desc.unk0 = 0x200005;
    desc.unk4 = 0;
    desc.timer = (func_150ADA20() % 41U) + 40;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r2 = 0xB0;
    desc.g2 = 0xA0;
    desc.b2 = 0x2A;
    desc.r = 0x40;
    desc.g = 0x0B;
    desc.b = 0x6A;
    desc.a = 0xFF;
    desc.alpha = (func_150ADA20() % 157U) + 100;
    desc.brightness = 0xFF;
    desc.primColorMode = 3;
    desc.envColorMode = 3;
    size = (func_150ADA68() * 401.0f) + 300.0f;
    desc.width = size;
    desc.height = size;
    desc.pos.x = x;
    desc.pos.y = y;
    desc.pos.z = z;

    random = func_150ADA20();
    fraction = func_150ADA68();
    func_15143794(angle, (random % 26U) - 25, ((fraction * 500.0f) + 1000.0f) * 0.01f, &desc.vel.x);
    desc.flags = 0xE05;
    desc.gravity = 0.0f;

    if (func_150ADA20() & 1) {
        desc.flags |= 0x40;
    }

    if (func_150ADA20() & 1) {
        desc.flags |= 0x80;
    }

    desc.updateFunc = 7;
    desc.deathFunc = -1;
    desc.fadeStart = 25;
    desc.fadeRate = 10;
    desc.scaleStart = 60;
    desc.scaleRate = 1.02f;
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
