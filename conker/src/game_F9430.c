#include <ultra64.h>
#define func_1513D668 func_1513D668_void_proto
#include "functions.h"
#undef func_1513D668
s32 func_1513D668(s32 arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s16 arg6, f32 arg7, f32 arg8, s32 arg9, s32 argA, u8 argB, s32 argC, u8 argD, s32 argE);
#include "variables.h"

void func_15143794(s16 yaw, s16 pitch, f32 speed, f32 *out);
struct260 *func_15130374(void *, u8, s32, u8, s32);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[2];
    f32 unk4[3];
    u8 unk10;
    u8 unk11;
    s16 unk12;
    s16 unk14;
    u8 pad16[2];
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s16 unk28;
    u8 pad2A[2];
} CBF80Entry;

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
} CBF80Ground;

typedef struct {
    s16 unk0;
    f32 unk4;
} CBF80Extra;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
} CBF80Col;

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
    u8 unk4C;
    u8 pad4D[3];
    s32 unk50;
    s16 unk54;
    s16 unk56;
} CBF80Main;

typedef struct {
    u8 r;
    u8 g;
    u8 b;
} CBF80Rgb;

typedef struct {
    f32 x, y, z;
} CBF80Vec;

typedef struct {
    CBF80Vec unk0;
    u8 unkC;
    s8 unkD;
    u8 padE[2];
    f32 unk10;
    s32 unk14;
    s16 unk18;
    s16 unk1A;
    f32 unk1C;
    s16 unk20;
    u8 pad22[2];
    f32 unk24;
    f32 unk28;
    s8 unk2C;
    s8 unk2D;
    u8 pad2E[2];
} CBF80Fx;

extern CBF80Entry D_800A0640[];
extern f32 D_800A06C4;
extern f32 D_800A06C8;
void func_1504715C(CBF80Ground *, void *);
s32 func_15046C80(f32 *, s32, f32, CBF80Ground *);
void func_15143134(void *, f32 *, s32);
void func_150CCD90(f32, f32 *, f32 *, f32 *);
void func_1514C678(f32, f32, f32, f32, s32, s32, s32, s32, s32, f32, s32, u8);
void func_150C88D0(void *, s32, s32, s32);
void *memcpy(void *, void *, s32);

void func_150CBF80(void *arg0, u8 arg1, s32 arg2, u8 arg3) {
    f32 pos[3];
    CBF80Entry *e;
    f32 fr;
    f32 fg;
    f32 fb;
    u8 r;
    u8 g;
    u8 b;
    CBF80Ground ground;

    if (arg0 == NULL || *(s32 *)((u8 *)arg0 + 0x1D4) == 0) {
        return;
    }
    func_1504715C(&ground, arg0);
    e = &D_800A0640[arg1];
    func_15143134(e->unk4, pos, *(s32 *)((u8 *)arg0 + 0x1D4) + (e->unk1 << 6));
    {
        f32 ray[3];

        ray[0] = pos[0];
        ray[1] = pos[1] + 200.0f;
        ray[2] = pos[2];
        if (func_15046C80(ray, 0, pos[1] - 2000.0f, &ground) == 0) {
            return;
        }
    }
    pos[1] = ground.unk00;
lbl:
    func_150CCD90(pos[2], &fr, &fg, &fb);
    r = (u8)fr;
    g = (u8)fg;
    b = (u8)fb;
    {
        CBF80Extra extra;
        CBF80Main mn;
        s16 spd;
        CBF80Col cols[4];
        s32 ret;

        cols[0].unk0 = r;
        cols[0].unk2 = g;
        cols[0].unk4 = b;
        cols[0].unk6 = 0xFF;
        cols[0].unk8 = 0;
        cols[1].unk0 = r;
        cols[1].unk2 = g;
        cols[1].unk4 = b;
        cols[1].unk6 = 0xFF;
        cols[1].unk8 = 0;
        cols[2].unk0 = r;
        cols[2].unk2 = g;
        cols[2].unk4 = b;
        cols[2].unk6 = 0xFF;
        cols[2].unk8 = 0;
        cols[3].unk0 = r;
        cols[3].unk2 = g;
        cols[3].unk4 = b;
        cols[3].unk6 = 0xFF;
        cols[3].unk8 = 0;
        mn.unk0 = e->unk0;
        mn.unk2 = 0x2601;
        mn.unk4 = (func_150ADA20() % 51U) + 0x84;
        mn.unk8 = 0;
        mn.unkC = 0;
        mn.unk10 = 0xFF;
        mn.unk11 = 0xFF;
        mn.unk12 = 0xFF;
        mn.unk13 = 0xFF;
        mn.unk14 = mn.unk18 = (func_150ADA68() * e->unk1C) + e->unk18;
        mn.unk1C.unk0 = pos[0];
        mn.unk1C.unk4 = pos[1] + 10.0f;
        mn.unk1C.unk8 = pos[2];
        mn.unk28 = 0.0f;
        mn.unk2C = 0.0f;
        mn.unk30 = 0.0f;
        mn.unk34 = 1.0f;
        mn.unk38 = 1.0f;
        mn.unk3C = 1.0f;
        mn.unk40 = 0x1C0001;
        mn.unk44 = (func_150ADA20() % (u32)(e->unk11 + 1)) + e->unk10;
        mn.unk45 = 0xFF;
        mn.unk46 = 0;
        mn.unk47 = 6;
        extra.unk0 = (mn.unk4 - (func_150ADA20() % (u32)(e->unk14 + 1))) - e->unk12;
        extra.unk4 = ((func_150ADA68() * e->unk24) + e->unk20) * mn.unk14;
        ret = func_1513D668((s32)&mn, (s32)cols, 0, 0x18, 0, 0,
                            (s16)((*(u16 *)((u8 *)arg0 + 0x7A) >> 8) + e->unk28),
                            500.0f, 500.0f, 0, (s32)&ground.unk04, 1, 8, arg3, arg2);
        if (ret != 0) {
            memcpy((u8 *)ret + 0x128, &extra, 8);
        }
        {
            CBF80Fx fx;
            f32 d;

            spd = (func_150ADA20() % 1U) + 0x29;
            func_1514C678(pos[0], pos[1], pos[2], (func_150ADA68() * 50.0f) + 80.0f,
                          0, 0xFF, spd, 0x11, 0, 0.0f, 0, arg3);
            d = D_800A06C4;
lbl2:
            {
                s16 n1;
                s16 n2;

                fx.unk0 = *(CBF80Vec *)pos;
                fx.unk10 = D_800A06C8;
                fx.unkC = 1;
                fx.unkD = -1;
                fx.unk14 = 0xC;
                fx.unk18 = 0x28;
                fx.unk1A = 0x1E;
                n1 = 10;
                fx.unk1C = d / (f32)n1;
                fx.unk20 = 0x19;
                fx.unk24 = d / fx.unk20;
                fx.unk2C = -1;
                fx.unk2D = -1;
                fx.unk28 = 0.0f;
                func_150C88D0(&fx, 0, 0xFF, 1);
            }
        }
        {
            CBF80Rgb rgb;

            spd = (func_150ADA20() % 14U) + 0xF;
            rgb.r = r;
            rgb.g = g;
            rgb.b = b;
            func_1514C678(pos[0], pos[1], pos[2], (func_150ADA68() * 70.0f) + 70.0f,
                          0, 0xFF, spd, 0x12, 0, 0.0f, (s32)&rgb, arg3);
        }
        func_151D5404((struct17 *)pos, 500.0f, 5000.0f, 0.0002f, 0xC, 0xF, 0xFF, 0);
    }
}

extern f32 D_800BE9A4;

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

/* Particle update: fades in over its first 32 ticks, then grows once past its delay. */
s32 func_150CC638(GrowParticle *p) {
    GrowParticleExtra *extra;

    if (p->flags & 1) {
        extra = &p->extra;
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

    decay = &p->decay;
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
