#include <ultra64.h>
/* functions.h declares func_1513D668 as void; this file uses its real return value (declared below) */
#define func_1513D668 func_1513D668_void_proto
#include "functions.h"
#undef func_1513D668
#include "variables.h"

void func_15143794(s16 yaw, s16 pitch, f32 speed, f32 *out);
struct260 *func_15130374(void *, u8, s32, u8, s32);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);

/* One entry of the D_800A0640 table: where on the owner's skeleton func_150CBF80 spawns, and its random ranges. */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 bone;
    /* 0x04 */ f32 offset[3]; /* bone-local spawn point */
    /* 0x10 */ u8 unk10Min;
    /* 0x11 */ u8 unk10Range;
    /* 0x12 */ s16 growDelayMin; /* subtracted from the lifetime */
    /* 0x14 */ s16 growDelayRange;
    /* 0x18 */ f32 sizeMin;
    /* 0x1C */ f32 sizeRange;
    /* 0x20 */ f32 growRateMin; /* per unit of size */
    /* 0x24 */ f32 growRateRange;
    /* 0x28 */ s16 yawOffset;
} GroundBurstDef;

/* Ground probe result filled by func_1504715C / func_15046C80. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ f32 unk4[6];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 pad1D[7];
} GroundHit;

/* Per-corner colour passed to func_1513D668. */
typedef struct {
    /* 0x0 */ s16 r;
    /* 0x2 */ s16 g;
    /* 0x4 */ s16 b;
    /* 0x6 */ s16 a;
    /* 0x8 */ s16 unk8;
} CornerColor;

/* Spawn template for func_1513D668, as filled in by func_150CBF80. */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 lifetime;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 color[4];
    /* 0x14 */ f32 size0;
    /* 0x18 */ f32 size1;
    /* 0x1C */ struct17 position;
    /* 0x28 */ f32 unk28[3];
    /* 0x34 */ f32 unk34[3];
    /* 0x40 */ s32 flags;
    /* 0x44 */ u8 unk44;
    /* 0x45 */ u8 unk45;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
} GrowSpawn;

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} Rgb8;

typedef struct {
    f32 x, y, z;
} Vec3;

/* Effect template for func_150C88D0. */
typedef struct {
    /* 0x00 */ Vec3 position;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ s8 unkD;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s8 unk2C;
    /* 0x2D */ s8 unk2D;
} Effect88D0Spawn;

/* Per-particle data copied to 0x128 of a particle by func_150CBF80. */
typedef struct {
    /* 0x0 */ s16 growDelay; /* age after which the particle starts growing */
    /* 0x4 */ f32 growRate;
} GrowParticleExtra;

typedef struct {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ s16 age;
    /* 0x01E */ u8 pad1E[0x2C - 0x1E];
    /* 0x02C */ f32 size0;
    /* 0x030 */ f32 size1;
    /* 0x034 */ u8 pad34[0x58 - 0x34];
    /* 0x058 */ s32 flags;
    /* 0x05C */ u8 alpha;
    /* 0x05D */ u8 pad5D[0x128 - 0x5D];
    /* 0x128 */ GrowParticleExtra extra;
} GrowParticle;

typedef f32 BoneMtx[4][4];

GrowParticle *func_1513D668(GrowSpawn *spawn, CornerColor *colors, u8 arg2, u8 arg3, u8 arg4, u8 arg5,
                            s16 yaw, f32 arg7, f32 arg8, s32 arg9, f32 *argA, u8 argB, s32 argC, u8 argD,
                            s32 argE);

extern GroundBurstDef D_800A0640[];
void func_1504715C(GroundHit *, struct127 *);
s32 func_15046C80(f32 *, s32, f32, GroundHit *);
void func_15143134(f32 *in, f32 *out, BoneMtx *mtx);
void func_150CCD90(f32, f32 *, f32 *, f32 *);
void func_1514C678(f32, f32, f32, f32, s32, s32, s32, s32, s32, f32, void *, u8);
void func_150C88D0(Effect88D0Spawn *, s32, s32, s32);
void *memcpy(void *, void *, s32);

/* Spawns a burst at a bone-relative point of owner, dropped onto the ground below: a growing
   ground-coloured particle, two func_1514C678 effects, a func_150C88D0 effect and a func_151D5404 effect. */
void func_150CBF80(struct127 *owner, u8 index, s32 arg2, u8 arg3) {
    f32 pos[3];
    GroundBurstDef *def;
    f32 fr;
    f32 fg;
    f32 fb;
    u8 r;
    u8 g;
    u8 b;
    GroundHit ground;

    if (owner == NULL || owner->unk1D4 == NULL) {
        return;
    }
    func_1504715C(&ground, owner);
    def = &D_800A0640[index];
    /* struct127.unk1D4 (typed struct255 * in structs.h) is the owner's bone matrix array */
    func_15143134(def->offset, pos, &((BoneMtx *)owner->unk1D4)[def->bone]);
    {
        f32 ray[3];

        ray[0] = pos[0];
        ray[1] = pos[1] + 200.0f;
        ray[2] = pos[2];
        if (func_15046C80(ray, 0, pos[1] - 2000.0f, &ground) == 0) {
            return;
        }
    }
    pos[1] = ground.height;
lbl: // matching: unused label ends the basic block so the pos[1] store stays out of the jal delay slot
    func_150CCD90(pos[2], &fr, &fg, &fb);
    r = fr;
    g = fg;
    b = fb;
    {
        GrowParticleExtra extra;
        GrowSpawn spawn;
        s16 rnd;
        CornerColor colors[4];
        GrowParticle *particle;

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
        spawn.type = def->type;
        spawn.unk2 = 0x2601;
        spawn.lifetime = (func_150ADA20() % 51U) + 132;
        spawn.unk8 = 0;
        spawn.unkC = 0;
        spawn.color[0] = 0xFF;
        spawn.color[1] = 0xFF;
        spawn.color[2] = 0xFF;
        spawn.color[3] = 0xFF;
        spawn.size0 = spawn.size1 = (func_150ADA68() * def->sizeRange) + def->sizeMin;
        spawn.position.unk0 = pos[0];
        spawn.position.unk4 = pos[1] + 10.0f;
        spawn.position.unk8 = pos[2];
        spawn.unk28[0] = 0.0f;
        spawn.unk28[1] = 0.0f;
        spawn.unk28[2] = 0.0f;
        spawn.unk34[0] = 1.0f;
        spawn.unk34[1] = 1.0f;
        spawn.unk34[2] = 1.0f;
        spawn.flags = 0x1C0001;
        spawn.unk44 = (func_150ADA20() % (u32)(def->unk10Range + 1)) + def->unk10Min;
        spawn.unk45 = 0xFF;
        spawn.unk46 = 0;
        spawn.unk47 = 6;
        extra.growDelay = (spawn.lifetime - (func_150ADA20() % (u32)(def->growDelayRange + 1))) - def->growDelayMin;
        extra.growRate = ((func_150ADA68() * def->growRateRange) + def->growRateMin) * spawn.size0;
        particle = func_1513D668(&spawn, colors, 0, 0x18, 0, 0, (owner->unk7A >> 8) + def->yawOffset,
                                 500.0f, 500.0f, 0, ground.unk4, 1, 8, arg3, arg2);
        if (particle != NULL) {
            memcpy(&particle->extra, &extra, sizeof(extra));
        }
        {
            Effect88D0Spawn fx;
            f32 d;

            rnd = (func_150ADA20() % 1U) + 41; // matching: a degenerate random range; the call and divu are in the ROM
            func_1514C678(pos[0], pos[1], pos[2], (func_150ADA68() * 50.0f) + 80.0f,
                          0, 0xFF, rnd, 0x11, 0, 0.0f, NULL, arg3);
            d = 13.90000057f;
lbl2: // matching: unused label ends the basic block so &fx isn't hoisted above the 13.9f load
            {
                s16 n1; // matching: a variable, not a literal, so IDO emits the runtime int-to-float convert

                fx.position = *(Vec3 *)pos;
                fx.unk10 = 70.59999847f;
                fx.unkC = 1;
                fx.unkD = -1;
                fx.unk14 = 12;
                fx.unk18 = 40;
                fx.unk1A = 30;
                n1 = 10;
                fx.unk1C = d / (f32)n1;
                fx.unk20 = 25;
                fx.unk24 = d / fx.unk20;
                fx.unk2C = -1;
                fx.unk2D = -1;
                fx.unk28 = 0.0f;
                func_150C88D0(&fx, 0, 0xFF, 1);
            }
        }
        {
            Rgb8 rgb;

            rnd = (func_150ADA20() % 14U) + 15;
            rgb.r = r;
            rgb.g = g;
            rgb.b = b;
            func_1514C678(pos[0], pos[1], pos[2], (func_150ADA68() * 70.0f) + 70.0f,
                          0, 0xFF, rnd, 0x12, 0, 0.0f, &rgb, arg3);
        }
        func_151D5404((struct17 *)pos, 500.0f, 5000.0f, 0.0002f, 12, 15, 0xFF, 0);
    }
}

/* Particle update: fades in over its first 32 ticks, then grows once past its delay. */
s32 func_150CC638(GrowParticle *p) {
    GrowParticleExtra *extra;

    if (p->flags & 1) {
        extra = &p->extra; // matching: the pointer is computed once and shared by both branches
        if (p->age < 32) {
            s32 fadeAlpha = p->age * 8;

            if (fadeAlpha < p->alpha) {
                p->alpha = fadeAlpha;
            }
        }
        if (extra->growDelay < p->age) {
            f32 growth = extra->growRate * D_800BE9A4;

            p->size0 += growth;
            p->size1 += growth;
        }
    }
    return 1;
}

/* Spawn template for func_15130374, as filled in by func_150CC6B8. */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 lifetime;
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
    /* 0x28 */ f32 size0;
    /* 0x2C */ f32 size1;
    /* 0x30 */ f32 position[3];
    /* 0x3C */ u8 pad3C[0xC];
    /* 0x48 */ f32 velocity[3];
    /* 0x54 */ f32 unk54;
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u8 pad5C[4];
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ s8 unk63;
    /* 0x64 */ u8 pad64[0xC];
} DecaySpawn;

/* Spawns a particle at (x, y, z) flung along yaw with a random pitch; it shrinks by 0.901 per tick (func_150CC8D4). */
s32 func_150CC6B8(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    struct260 *particle;
    DecaySpawn spawn;
    f32 size;
    f32 decay;
    f32 speed;
    u32 random;

    spawn.unk1D = 0x29;
    spawn.unk8 = 0xE03;
    spawn.flags = 0x200005;
    spawn.unk4 = 0;
    spawn.lifetime = (func_150ADA20() & 0xF) + 20;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.color1[0] = 0xB0;
    spawn.color1[1] = 0xA0;
    spawn.color1[2] = 0x2A;
    spawn.color0[0] = 0x40;
    spawn.color0[1] = 0xB;
    spawn.color0[2] = 0x6A;
    spawn.color0[3] = 0xFF;

    spawn.color1[3] = (func_150ADA20() % 156U) + 100;
    spawn.unk1C = 0xFF;
    spawn.unk60 = 3;
    spawn.unk61 = 3;

    size = (func_150ADA68() * 401.0f) + 300.0f;
    spawn.size0 = size;
    spawn.size1 = size;
    spawn.position[0] = x;
    spawn.position[1] = y;
    spawn.position[2] = z;

    random = func_150ADA20();
    speed = (func_150ADA68() * 200.0f + 350.0f) * 0.1000000015f;
    func_15143794(yaw, (s16)((random % 21U) - 25), speed, spawn.velocity);

    spawn.unk54 = 0.0f;
    spawn.unk58 = 0xE05;

    if (func_150ADA20() & 1) {
        spawn.unk58 |= 0x40;
    }

    if (func_150ADA20() & 1) {
        spawn.unk58 |= 0x80;
    }

    spawn.unk62 = 8;
    spawn.unk24 = 1.039999962f;
    decay = 0.9010000229f;
    spawn.unk63 = -1;
    spawn.unk1E = 10;
    spawn.unk20 = 25;
    spawn.unk22 = 27;

    particle = func_15130374(&spawn, 1, 4, arg14, 1);
    if (particle != NULL) {
        memcpy((u8 *)particle + 0xA8, &decay, sizeof(decay));
    }

    return 1;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ f32 unk58;
    /* 0x5C */ u8 pad5C[0x60 - 0x5C];
    /* 0x60 */ f32 unk60;
    /* 0x64 */ u8 pad64[0xA8 - 0x64];
    /* 0xA8 */ f32 decay; /* set by func_150CC6B8 */
} DecayParticle;

/* Particle update: scales unk58 and unk60 by the decay factor once per elapsed tick. */
s32 func_150CC8D4(DecayParticle *p, s32 arg1) {
    f32 *decay;
    s32 i;

    decay = &p->decay; // matching: IDO keeps the factor's address in a register only via this pointer
    for (i = D_800BE9E4; i != 0; i--) {
        p->unk58 *= *decay;
        p->unk60 *= *decay;
    }
    return 1;
}

/* Spawn template for func_15132A4C, as filled in by func_150CCA7C. */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 size0;
    /* 0x0C */ f32 size1;
    /* 0x10 */ f32 rotation[3];
    /* 0x1C */ f32 scale[3];
    /* 0x28 */ f32 position[3];
    /* 0x34 */ f32 velocity[3];
    /* 0x40 */ f32 rotationVel[3];
    /* 0x4C */ f32 gravity;
    /* 0x50 */ s32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 alpha;
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
} TumbleSpawn;

/* Spawns a tumbling particle at (x, y, z), flung along yaw with a random upward pitch and spin. */
s32 func_150CCA7C(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5,
                  s32 arg6, s32 arg7, s16 yaw, s32 arg9, s32 arg10, s32 arg11,
                  s32 arg12, s32 arg13, u8 arg14) {
    TumbleSpawn spawn;
    f32 speed;
    u32 random;

    spawn.position[0] = x;
    spawn.position[1] = y;
    spawn.scale[0] = 1.0f;
    spawn.scale[1] = 1.0f;
    spawn.scale[2] = 1.0f;
    spawn.flags = 0x29E8;
    spawn.rotation[0] = 0.0f;
    spawn.rotation[1] = 0.0f;
    spawn.rotation[2] = 0.0f;
    spawn.rotationVel[1] = 0.0f;
    spawn.unk0 = 1.0f;
    spawn.unk56 = 0x20;
    spawn.position[2] = z;
    spawn.unk4 = 0.200000003f;

    random = func_150ADA20();
    speed = (func_150ADA68() * 200.0f + 300.0f) * 0.1000000015f;
    func_15143794(yaw, (s16)((random % 44U) - 63), speed, spawn.velocity);

    spawn.rotationVel[0] = (func_150ADA68() * 260.0f + -130.0f) * 0.1000000015f;
    spawn.rotationVel[2] = (func_150ADA68() * 260.0f + -130.0f) * 0.1000000015f;
    spawn.lifetime = (func_150ADA20() % 51U) + 50;
    spawn.gravity = (func_150ADA68() * 1500.0f + -3500.0f) * 0.001000000047f;

    spawn.size0 = spawn.size1 = (func_150ADA68() * 500.0f + 100.0f) * 0.001000000047f;
    spawn.unk58 = 0;
    spawn.unk5C = 0;

    spawn.alpha = (func_150ADA20() % 101U) + 155;
    spawn.unk61 = 3;
    spawn.unk62 = 0;
    spawn.unk63 = 0;
    spawn.unk64 = 0;
    spawn.unk65 = 0;
    spawn.unk66 = 0;
    spawn.unk67 = 0;
    spawn.unk68 = 0;
    spawn.unk6A = 2;
    spawn.unk6C = 0;
    spawn.unk70 = 0;
    spawn.unk72 = 0x20;
    spawn.unk74 = 7;

    func_15132A4C(&spawn, 3, 0xFF, 0, arg14, 0);
    return 1;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 rotation[3];
    /* 0x2C */ u8 pad2C[0xC];
    /* 0x38 */ f32 position[3];
    /* 0x44 */ f32 velocity[3];
    /* 0x50 */ f32 rotationVel[3];
    /* 0x5C */ f32 gravity;
} TumbleParticle;

/* Integrates a tumbling particle's position (with gravity) and rotation over one frame step. */
s32 func_150CCCB4(TumbleParticle *p) {
    p->position[0] += p->velocity[0] * D_800BE9A4;
    p->position[1] += p->velocity[1] * D_800BE9A4 + p->gravity * D_800BE9A4 * D_800BE9A4 * 0.5f;
    p->position[2] += p->velocity[2] * D_800BE9A4;
    p->velocity[1] += p->gravity * D_800BE9A4;
    p->rotation[0] += p->rotationVel[0] * D_800BE9A4;
    p->rotation[1] += p->rotationVel[1] * D_800BE9A4;
    p->rotation[2] += p->rotationVel[2] * D_800BE9A4;
    return 1;
}
