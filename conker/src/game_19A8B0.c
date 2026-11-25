#include <ultra64.h>
#define func_15048A40 func_15048A40_void_unused
#include "functions.h"
#undef func_15048A40
#include "variables.h"

extern void func_1516F864(s32);
extern void func_1516F94C(s32, s32);

struct Table1516F024 {
    u8 unk0[0xB];
};

extern struct Table1516F024 D_8008CBC4;
extern struct Table1516F024 D_8008CBD0;
extern struct Table1516F024 D_8008CBDC;
extern struct Table1516F024 D_8008CBE8;

typedef struct ParticleLink {
    u8 pad0[4];
    s16 status;
} ParticleLink;

/* A 0x34-byte sprite particle, spawned by func_1516D4E8 and updated every frame by the
 * handler that D_8008CBA0 holds for its kind. The bytes from 0x26 on depend on the kind. */
typedef struct Particle {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 x;
    /* 0x10 */ s16 y;
    /* 0x12 */ s16 z;
    /* 0x14 */ s16 scaleX;
    /* 0x16 */ s16 scaleY;
    /* 0x18 */ s16 velocityY;
    /* 0x1A */ u8 texture; /* index into D_8008CA4C */
    /* 0x1B */ u8 frame;
    /* 0x1C */ u8 r;
    /* 0x1D */ u8 g;
    /* 0x1E */ u8 b;
    /* 0x1F */ u8 alpha;
    /* 0x20 */ u8 envR;
    /* 0x21 */ u8 envG;
    /* 0x22 */ u8 envB;
    /* 0x23 */ u8 kind; /* index into D_8008CBA0 */
    /* 0x24 */ u8 lifetime; /* fades in while nonzero, out once it runs down */
    /* 0x25 */ u8 yFraction;
    union {
        /* kinds that drift sideways with func_1516F864 */
        struct {
            /* 0x26 */ s8 velocityXHigh;
            /* 0x27 */ u8 velocityXLow;
            /* 0x28 */ s8 velocityZHigh;
            /* 0x29 */ u8 velocityZLow;
            /* 0x2A */ u8 xFraction;
            /* 0x2B */ u8 zFraction;
            /* 0x2C */ u8 unk2C;
            /* 0x2D */ u8 unk2D;
            /* 0x2E */ u8 unk2E;
            /* 0x2F */ u8 unk2F;
        } drift;
        /* func_1516FBCC: blends its colour from rgb0 to rgb1 while it grows */
        struct {
            /* 0x26 */ u8 r0;
            /* 0x27 */ u8 g0;
            /* 0x28 */ u8 b0;
            /* 0x29 */ u8 r1;
            /* 0x2A */ u8 g1;
            /* 0x2B */ u8 b1;
            /* 0x2C */ u8 blend;
            /* 0x2D */ s8 growX;
            /* 0x2E */ s8 growY;
            /* 0x2F */ u8 fadeShift;
        } tint;
    } u;
    /* 0x30 */ ParticleLink *link;
} Particle;

/* A 0x4C-byte particle emitter, spawned by func_1516D99C and run by func_1516DB90. Most of
 * its bytes are the template that func_1516D4E8 gets for each particle it spawns. */
typedef struct Emitter {
    /* 0x00 */ u8 unk0; /* 0x11, or 0x66 for an emitter whose particles are 0x67s */
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 x; /* also a pointer's high half (flags 0x30) or a D_800A6E30 row */
    /* 0x10 */ s16 y; /* also a pointer's low half (flags 0x30) */
    /* 0x12 */ s16 z;
    /* 0x14 */ s16 scaleX;
    /* 0x16 */ s16 scaleY;
    /* 0x18 */ s16 scaleXRandom;
    /* 0x1A */ s16 scaleYRandom;
    /* 0x1C */ s16 velocityY;
    /* 0x1E */ u16 velocityYRandom;
    /* 0x20 */ u16 timer; /* frames left; 0xFFFF runs forever */
    /* 0x22 */ u16 flags;
    /* 0x24 */ u8 texture;
    /* 0x25 */ u8 frame;
    /* 0x26 */ u8 r;
    /* 0x27 */ u8 g;
    /* 0x28 */ u8 b;
    /* 0x29 */ u8 alpha;
    /* 0x2A */ u8 envR;
    /* 0x2B */ u8 envG;
    /* 0x2C */ u8 envB;
    /* 0x2D */ u8 kind;
    /* 0x2E */ u8 velocityXHigh;
    /* 0x2F */ u8 velocityXLow;
    /* 0x30 */ u8 velocityZHigh;
    /* 0x31 */ u8 velocityZLow;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 velocityXHighRandom;
    /* 0x39 */ u8 velocityXLowRandom;
    /* 0x3A */ u8 velocityZHighRandom;
    /* 0x3B */ u8 velocityZLowRandom;
    /* 0x3C */ u8 initialTimer;
    /* 0x3D */ u8 radiusXZ;
    /* 0x3E */ u8 radiusY;
    /* 0x3F */ u8 object; /* D_800CC2D0 index whose bone it follows; 0xFF for none */
    /* 0x40 */ u8 particleLifetime;
    /* 0x41 */ u8 interval;
    /* 0x42 */ u8 countdown;
    /* 0x43 */ u8 chance; /* out of 256 per particle; 0 always spawns */
    /* 0x44 */ u8 burst; /* particles per spawn */
    /* 0x45 */ u8 speed;
    /* 0x46 */ u8 startOffset;
    /* 0x47 */ u8 bone;
    /* 0x48 */ u8 particleLink;
    /* 0x49 */ u8 pad49[3];
} Emitter;

extern void *func_15167A68(s32 type, s32 arg1, s32 size, s32 arg3, s32 arg4, s32 arg5);
extern ParticleLink *func_150448D0(s32, s32, s32, s32, s32, s32, s32, s16 *, s16 *);

/* OLDTYPES */
struct Obj1516D4E8 {
    char pad0[0xE];
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s32 unk30;
};

typedef struct {
    char pad_0[0x18];
    s16 field_0x18;
} Obj1516F984;

typedef struct {
    char pad_0[0x14];
    s16 field_0x14;
    s16 field_0x16;
    s16 field_0x18;
    char pad_0x1A[0x5];
    u8 field_0x1F;
    char pad_0x20[0x4];
    u8 field_0x24;
    char pad_0x25[0x1];
    u8 field_0x26;
    char pad_0x27[0x5];
    u8 field_0x2C;
    u8 field_0x2D;
} Obj1516FadeState;

typedef struct {
    char pad_0[0x14];
    s16 field_0x14;
    s16 field_0x16;
    s16 field_0x18;
    char pad_0x1A[0x5];
    u8 field_0x1F;
    char pad_0x20[0x4];
    u8 field_0x24;
    char pad_0x25[0x7];
    s8 field_0x2C;
} Obj1516FadeStepState;


#define LOW_U8(arg) (*(u8 *)((u8 *)&(arg) + 3))


/* variables.h declares D_800DD2A0 as a single u8; it is really a u8[4]. */
#define D_800DD2A0_ARRAY ((u8 *)&D_800DD2A0)

void func_1516D400(void) {
    s32 i;
    s32 level;

    for (i = 0; i < 4; i++) {
        if (D_800DD2A0_ARRAY[i] < 0xFF) {
            level = D_800DD2A0_ARRAY[i] + D_800BE9A0;
            if (level > 0xFF) {
                level = 0xFF;
            }
            D_800DD2A0_ARRAY[i] = level;
        }
    }
}

// matching: only a K&R definition makes IDO narrow the u8/s16 args by reloading them from the stack
void func_1516D4E8(x, y, z, texture, frame, r, g, b, alpha, envR, envG, envB, kind, velocityXHigh,
    velocityXLow, velocityZHigh, velocityZLow, xFraction, zFraction, unk2C, unk2D, unk2E, unk2F, scaleX, scaleY,
    lifetime, velocityY, isAlt, linkArg, unkC, unk1)
    s16 x; s16 y; s16 z; u8 texture; u8 frame; u8 r; u8 g; u8 b; u8 alpha; u8 envR; u8 envG; u8 envB;
    u8 kind; u8 velocityXHigh; u8 velocityXLow; u8 velocityZHigh; u8 velocityZLow; u8 xFraction; u8 zFraction;
    u8 unk2C; u8 unk2D; u8 unk2E; u8 unk2F; s16 scaleX; s16 scaleY; u8 lifetime; s16 velocityY;
    s32 isAlt; s32 linkArg; u8 unkC; s32 unk1;
{
    Particle *particle;

    particle = func_15167A68(isAlt != 0 ? 0x67 : 0x12, unk1, sizeof(Particle), 0, unkC, 1);
    if (particle != NULL) {
        particle->x = x;
        particle->y = y;
        particle->z = z;
        particle->texture = texture;
        particle->frame = frame;
        particle->r = r;
        particle->g = g;
        particle->b = b;
        particle->alpha = alpha;
        particle->envR = envR;
        particle->envG = envG;
        particle->envB = envB;
        particle->kind = kind;
        particle->scaleX = scaleX;
        particle->scaleY = scaleY;
        particle->lifetime = lifetime;
        particle->velocityY = velocityY;
        particle->yFraction = 0;
        particle->u.drift.velocityXHigh = velocityXHigh;
        particle->u.drift.velocityXLow = velocityXLow;
        particle->u.drift.velocityZHigh = velocityZHigh;
        particle->u.drift.velocityZLow = velocityZLow;
        particle->u.drift.xFraction = xFraction;
        particle->u.drift.zFraction = zFraction;
        particle->u.drift.unk2C = unk2C;
        particle->u.drift.unk2D = unk2D;
        particle->u.drift.unk2E = unk2E;
        particle->u.drift.unk2F = unk2F;
        if (linkArg != 0) {
            particle->link = func_150448D0(-1, 0, linkArg, 20, 20, 20, 0, &particle->x, &particle->scaleX);
        } else {
            particle->link = NULL;
        }
    }
}

void func_1516D4E8(s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
    s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, u8, s32);
void func_1516972C(struct102 *arg0);
extern s32 (*D_8008CBA0[])(Particle *);

void func_1516D678(Particle *particle) {
    s32 value;
    ParticleLink *link;

    value = particle->lifetime;
    if (value != 0) {
        value -= D_800BE9E4;
        if (value < 0) {
            value = 0;
        }
        particle->lifetime = value;
    }
    if (D_8008CBA0[particle->kind](particle) == 1) {
        link = particle->link;
        if (link != NULL) {
            link->status = 0;
        }
        func_1516972C((struct102 *)particle);
    } else {
        /* y is 8.8 fixed point with its fraction in yFraction */
        value = particle->y;
        value = (value << 8) + particle->yFraction;
        value += particle->velocityY * D_800BE9E4;
        particle->y = value >> 8;
        particle->yFraction = value & 0xFF;
    }
}


/* The sprite description func_15142E24 fills in and func_15095760 draws. */
typedef struct ParticleSprite {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 scaleX;
    /* 0x08 */ s16 scaleY;
    /* 0x0A */ u8 alpha;
    /* 0x0B */ u8 unkB;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ s32 unk10;
} ParticleSprite;

typedef struct ParticleTexture {
    /* 0x00 */ u8 pad0[0xA];
    /* 0x0A */ u8 format;
} ParticleTexture;

extern ParticleTexture *D_8008CA4C[];
extern s32 D_800D2C9C;
extern u8 D_800D2DA8;
extern u8 D_800D2DA9;
extern u8 D_800D2DAA;
extern s16 D_800DD1BC;
extern s16 D_800DD1C8;
extern s16 D_800DD1CA;
extern s16 D_800DD1CC;
Gfx *func_15142E24(Gfx *gfx, s32 texture, s32 frame, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7,
                   s32 sprite, u8 *synced, s32 argA);
Gfx *func_15142FBC(Gfx *gfx, s32 othermodeH, s32 othermodeL, u8 *synced);
Gfx *func_15095760(Gfx *gfx, ParticleSprite *sprite);

Gfx *func_1516D738(Gfx *gfx, Particle *particle, s32 arg2) {
    ParticleSprite sprite;
    s32 pad; // matching: unused stack slot in the original frame
    ParticleTexture *texture;
    s32 synced;
    s32 r;
    s32 g;
    s32 b;
    s32 cycles;
    s32 frame;
    u8 frameIndex;
    s32 cycleType;

    synced = FALSE;
    D_800D2DAB = 0;
    texture = D_8008CA4C[particle->texture];
    frame = particle->frame;
    frameIndex = frame++; // matching: the dead increment steers IDO's register choice here
    gfx = func_15142E24(gfx, (s32)D_8008CA4C[particle->texture], frameIndex << 8, 2, 0x100, 0x100, 0, 6,
                        (s32)&sprite, (u8 *)&synced, 3);
    if (texture->format == 5) {
        cycles = 2;
    } else {
        cycles = 1;
    }
    if (cycles != D_800DD1BC) {
        if (!synced) {
            gDPPipeSync(gfx++);
            synced = TRUE;
        }
        D_800DD1BC = cycles;
        if (cycles == 1) {
            cycleType = G_CYC_1CYCLE;
            gDPSetCombineLERP(gfx++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0);
        } else {
            cycleType = G_CYC_2CYCLE;
            gDPSetCombineLERP(gfx++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL1, 0,
                              0, 0, 0, COMBINED, 0, 0, 0, COMBINED);
        }
    }
    gfx = func_15142FBC(gfx, cycleType | D_800D2C9C | G_TF_BILERP | G_TC_FILT, 0x5049DC, (u8 *)&synced);
    sprite.x = particle->x;
    sprite.y = particle->y;
    sprite.z = particle->z;
    sprite.scaleX = particle->scaleX;
    sprite.scaleY = particle->scaleY;
    sprite.alpha = particle->alpha;
    sprite.unkD = 0;
    D_800D2DA8 = particle->r;
    D_800D2DA9 = particle->g;
    D_800D2DAA = particle->b;
    r = particle->envR;
    g = particle->envG;
    b = particle->envB;
    if (r != D_800DD1C8 || g != D_800DD1CA || b != D_800DD1CC) {
        D_800DD1C8 = r;
        D_800DD1CA = g;
        D_800DD1CC = b;
        if (!synced) {
            gDPPipeSync(gfx++);
        }
        gDPSetEnvColor(gfx++, r, g, b, 0);
    }
    return func_15095760(gfx, &sprite);
}

// matching: only a K&R definition makes IDO narrow the u8/s16 args by reloading them from the stack
void func_1516D99C(x, y, z, texture, frame, r, g, b, alpha, envR, envG, envB, kind, velocityXHigh,
    velocityXLow, velocityZHigh, velocityZLow, unk32, unk33, unk34, unk35, unk36, unk37,
    velocityXHighRandom, velocityXLowRandom, velocityZHighRandom, velocityZLowRandom, scaleX, scaleY,
    scaleXRandom, scaleYRandom, timer, radiusXZ, radiusY, object, particleLifetime, velocityY,
    velocityYRandom, flags, interval, chance, burst, speed, startOffset, isAlt, bone, particleLink, unkC,
    unk1)
    s16 x; s16 y; s16 z; u8 texture; u8 frame; u8 r; u8 g; u8 b; u8 alpha; u8 envR; u8 envG; u8 envB;
    u8 kind; u8 velocityXHigh; u8 velocityXLow; u8 velocityZHigh; u8 velocityZLow; u8 unk32; u8 unk33;
    u8 unk34; u8 unk35; u8 unk36; u8 unk37; u8 velocityXHighRandom; u8 velocityXLowRandom;
    u8 velocityZHighRandom; u8 velocityZLowRandom; s16 scaleX; s16 scaleY; s16 scaleXRandom;
    s16 scaleYRandom; u16 timer; u8 radiusXZ; u8 radiusY; u8 object; u8 particleLifetime; s16 velocityY;
    u16 velocityYRandom; u16 flags; u8 interval; u8 chance; u8 burst; u8 speed; u8 startOffset;
    s32 isAlt; u8 bone; u8 particleLink; u8 unkC; s32 unk1;
{
    Emitter *emitter;

    emitter = func_15167A68(isAlt != 0 ? 0x66 : 0x11, unk1, sizeof(Emitter), 0, unkC, 1);
    if (emitter != NULL) {
        emitter->x = x;
        emitter->y = y;
        emitter->z = z;
        emitter->scaleX = scaleX;
        emitter->scaleY = scaleY;
        emitter->scaleXRandom = scaleXRandom;
        emitter->scaleYRandom = scaleYRandom;
        emitter->texture = texture;
        emitter->frame = frame;
        emitter->r = r;
        emitter->g = g;
        emitter->b = b;
        emitter->envR = envR;
        emitter->envG = envG;
        emitter->envB = envB;
        emitter->alpha = alpha;
        emitter->kind = kind;
        emitter->velocityXHigh = velocityXHigh;
        emitter->velocityXLow = velocityXLow;
        emitter->velocityZHigh = velocityZHigh;
        emitter->velocityZLow = velocityZLow;
        emitter->unk32 = unk32;
        emitter->unk33 = unk33;
        emitter->unk34 = unk34;
        emitter->unk35 = unk35;
        emitter->unk36 = unk36;
        emitter->unk37 = unk37;
        emitter->velocityXHighRandom = velocityXHighRandom;
        emitter->velocityXLowRandom = velocityXLowRandom;
        emitter->velocityZHighRandom = velocityZHighRandom;
        emitter->velocityZLowRandom = velocityZLowRandom;
        emitter->timer = timer;
        emitter->initialTimer = timer;
        emitter->radiusXZ = radiusXZ;
        emitter->radiusY = radiusY;
        emitter->object = object;
        emitter->particleLifetime = particleLifetime;
        emitter->velocityY = velocityY;
        emitter->velocityYRandom = velocityYRandom;
        emitter->flags = flags;
        emitter->interval = interval;
        emitter->countdown = 0;
        emitter->chance = chance;
        if (isAlt != 0 && burst >= 4) {
            burst = 3;
        }
        emitter->burst = burst;
        emitter->speed = speed;
        emitter->startOffset = startOffset;
        emitter->bone = bone;
        emitter->particleLink = particleLink;
    }
}
void func_1516D99C(s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
    s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
    s32, s32, s32, s32, u8, s32, s32, s32, s32, s32, u8, s32);

typedef struct {
    u8 unk0;
    u8 unk1;
    char pad2[0xA];
    u8 unkC;
    u8 padD;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    u16 unk1E;
    u16 unk20;
    u16 unk22;
    u8 unk24;
    u8 unk25;
    u8 unk26;
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    u8 unk30;
    u8 unk31;
    u8 unk32;
    u8 unk33;
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
    u8 unk3C;
    u8 unk3D;
    u8 unk3E;
    u8 unk3F;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 unk43;
    u8 unk44;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    u8 unk48;
} Emitter1516DB90;

extern s16 D_800A6E30[][7];
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern f32 func_150489B0(u8);
extern f32 func_15048A40(u8);
void func_15170034(s32 arg0, f32 *arg1, f32 *arg2, f32 *arg3);

#define HI_U8(arg) (bp[2])
#define LO_U8(arg) (bp[3])

void func_1516DB90(Emitter1516DB90 *arg0) {
    Emitter1516DB90 *p;
    s32 i;
    s16 a14;
    s16 a16;
    s32 life;
    s16 x;
    s16 y;
    s32 tmp;
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    s16 z;
    u8 *bp;
    s16 c24, c25, c26, c27, c28, c2A, c2B, c2C, c32, c33, c34, c35;
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 y2;
    f32 z2;
    f32 vx;
    f32 vy;
    f32 vz;
    f32 speed;
    f32 spread;
    f32 s;
    f32 c;
    u8 ang;
    u8 pad2;
    u8 rnd;
    s32 thresh;
    s32 rr;
    s32 flag;
    s32 count;

    p = arg0;
    bp = (u8 *)&tmp;
    if (p->unk0 == 0x11) {
        flag = 0;
    } else {
        flag = 1;
    }
    count = p->unk42;
    count -= D_800BE9E4;
    while (count < 0) {
        if (p->unk41 != 0) {
            count += p->unk41;
        } else {
            count = 0;
        }
        for (i = 0; i < p->unk44; i++) {
            if (p->unk43 != 0) {
                rnd = func_150ADA20() & 0xFF;
                if (p->unk22 & 4) {
                    thresh = p->unk43;
                    thresh = (p->unk20 * thresh) / p->unk3C;
                } else {
                    thresh = p->unk43;
                }
            }
            if (p->unk43 == 0 || rnd < thresh) {
                if (p->unk3F == 0xFF) {
                    if (p->unk22 & 0x10) {
                        
                        s16 *sv = (s16 *)((u16)p->unk10 | (p->unkE << 16));
                        x = sv[0];
                        y = sv[1] + p->unk12;
                        z = sv[2];
                    } else if (p->unk22 & 0x20) {
                        
                        f32 *fv = (f32 *)((u16)p->unk10 | (p->unkE << 16));
                        x = fv[0];
                        y = fv[1] + p->unk12;
                        z = fv[2];
                    } else {
                        x = p->unkE;
                        y = p->unk10;
                        z = p->unk12;
                    }
                } else {
                    if (D_800CC2D0[p->unk3F].unk1D4 == NULL) {
                        continue;
                    }
                    x1 = D_800A6E30[p->unkE][0];
                    y1 = D_800A6E30[p->unkE][1];
                    z1 = D_800A6E30[p->unkE][2];
                    x2 = D_800A6E30[p->unkE][3];
                    y2 = D_800A6E30[p->unkE][4];
                    z2 = D_800A6E30[p->unkE][5];
                    if (D_800A6E30[p->unkE][6] != 0) {
                        z2 += (u32)(func_150ADA20() & D_800A6E30[p->unkE][6]);
                    }
                    func_150A7960((f32 *)((u8 *)D_800CC2D0[p->unk3F].unk1D4 + (p->unk47 << 6)), x1, y1, z1, &x1, &y1, &z1);
                    func_150A7960((f32 *)((u8 *)D_800CC2D0[p->unk3F].unk1D4 + (p->unk47 << 6)), x2, y2, z2, &x2, &y2, &z2);
                    vx = (x2 - x1) * 8.0f;
                    vy = (y2 - y1) * 8.0f;
                    vz = (z2 - z1) * 8.0f;
                    life = vy;
                    ang = 0x40 - D_800CC2D0[p->unk3F].unk76;
                    vx -= D_800CC2D0[p->unk3F].xz_velocity * func_15048A40(ang) * 64.0f;
                    vz -= *(f32 *)((u8 *)D_800CC30C + p->unk3F * 0x32C) * func_150489B0(ang) * 64.0f;
                    tmp = vx;
                    r = HI_U8(tmp);
                    g = LO_U8(tmp);
                    tmp = vz;
                    b = HI_U8(tmp);
                    a = LO_U8(tmp);
                    x = x1;
                    y = y1;
                    z = z1;
                }
                if (p->unk22 & 8) {
                    func_15170034(p->unk22, &vx, &vy, &vz);
                    speed = p->unk45 * 16;
                    spread = (u32)p->unk46;
                    y += spread * vy;
                    vy = speed * vy;
                    life = vy;
                    func_150ADA20();
                    x += vx * spread;
                    z += vz * spread;
                    vx *= speed;
                    vz *= speed;
                    tmp = vx;
                    r = HI_U8(tmp);
                    g = LO_U8(tmp);
                    tmp = vz;
                    b = HI_U8(tmp);
                    a = LO_U8(tmp);
                }
                if (p->unk3D != 0) {
                    ang = func_150ADA20();
                    s = func_15048A40(ang);
                    c = func_150489B0(ang);
                    speed = (u32)((func_150ADA20() & 0xFFFF) * p->unk3D) * (1.0f / 65536.0f);
                    x += speed * s;
                    z += speed * c;
                }
                if (p->unk3E != 0) {
                    s = func_15048A40(func_150ADA20() & 0xFF);
                    speed = (u32)((func_150ADA20() & 0xFFFF) * p->unk3E) * (1.0f / 65536.0f);
                    y += (s32)(speed * s);
                }
                a14 = p->unk14;
                a16 = p->unk16;
                if (p->unk18 != 0) {
                    rr = (u32)func_150ADA20() % (u32)p->unk18;
                } else {
                    rr = 0;
                }
                if (p->unk22 & 1) {
                    a14 += rr;
                    a16 += rr;
                } else {
                    a14 += rr;
                    if (p->unk1A != 0) {
                        a16 += (u32)func_150ADA20() % (u32)p->unk1A;
                    }
                }
                if (!(p->unk22 & 8) && !(p->unk22 & 0x80)) {
                    life = p->unk1C;
                    if (p->unk1E != 0) {
                        life += (u32)func_150ADA20() % p->unk1E;
                    }
                    r = p->unk2E;
                    if (p->unk38 != 0) {
                        r += (u32)func_150ADA20() % p->unk38;
                        if (r >= 0x100) {
                            r = 0xFF;
                        }
                    }
                    g = p->unk2F;
                    if (p->unk39 != 0) {
                        g += (u32)func_150ADA20() % p->unk39;
                        if (g >= 0x100) {
                            g = 0xFF;
                        }
                    }
                    b = p->unk30;
                    if (p->unk3A != 0) {
                        b += (u32)func_150ADA20() % p->unk3A;
                        if (b >= 0x100) {
                            b = 0xFF;
                        }
                    }
                    a = p->unk31;
                    if (p->unk3B != 0) {
                        a += (u32)func_150ADA20() % p->unk3B;
                        if (a >= 0x100) {
                            a = 0xFF;
                        }
                    }
                }
                {
                    c32 = p->unk32; c33 = p->unk33; c34 = p->unk34; c35 = p->unk35;
                    c2A = p->unk2A; c2B = p->unk2B; c2C = p->unk2C;
                    c26 = p->unk26; c27 = p->unk27; c28 = p->unk28;
                func_1516D4E8(x, y, z, p->unk24, p->unk25, c26, c27, c28, p->unk29, c2A, c2B,
                              c2C, p->unk2D, r, g, b, a, c32, c33, c34, c35, p->unk36,
                              p->unk37, a14, a16, p->unk40, life, flag, p->unk48, p->unkC, p->unk1);
                }
            }
        }
    }
    p->unk42 = count;
    i = p->unk20;
    if (i != 0xFFFF) {
        i -= D_800BE9E4;
        if (i > 0) {
            p->unk20 = i;
        } else {
            func_1516972C((struct102 *)p);
        }
    }
}

void func_1516E778(u32 arg0, s32 arg1, u8 arg2, u8 arg3, s32 arg4) {
    func_1516D99C((s16)(arg0 >> 16), (s16)(arg0 & 0xFFFF), 0, arg2,
        0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0,
        8, 8, 1, 0, 0, 0, 0, 0x155, 0x155, 0x155, 0x155, arg1,
        0x1E, 0, 0xFF, 0x28, 0x168, 0x18, 0x21, 4, 0, 1,
        0, 0, 0, 0, 0, arg3, arg4);
}

extern u8 D_800A6E00[];
extern u8 D_800A6E0C[][3];

s32 func_1516E8CC(struct Obj1516D4E8 *arg0) {
    struct Obj1516D4E8 *vp;
    s32 temp_v1;
    s32 idx;
    s32 temp;
    s32 i;
    f32 frac;
    f32 inv;

    vp = arg0;
    temp_v1 = arg0->unk1F;
    if (arg0->unk24 != 0) {
        if (temp_v1 != 0xFF) {
            temp_v1 += D_800BE9E4 << 5;
            if (temp_v1 >= 0x100) {
                temp_v1 = 0xFF;
            }
            vp->unk1F = temp_v1;
        }
    } else {
        if (temp_v1 != 0) {
            temp_v1 -= D_800BE9E4 << 4;
            if (temp_v1 < 0) {
                temp_v1 = 0;
            }
            vp->unk1F = temp_v1;
        }
    }

    if ((arg0->unk24 == 0) && (temp_v1 == 0)) {
        return 1;
    }

    temp = arg0->unk14;
    idx = arg0->unk24;
    temp += D_800BE9E4 * 10;
    arg0->unk16 = temp;
    arg0->unk14 = temp;

    if (arg0->unk24 == 0) {
        idx += arg0->unk1F >> 5;
    } else {
        idx = idx + 8;
    }

    for (i = vp->unk26; i < vp->unk26 + 8; i++) {
        if (idx >= D_800A6E00[i]) {
            frac = (f32)(idx - D_800A6E00[i]) / (f32)(D_800A6E00[i - 1] - D_800A6E00[i]);
            inv = 1.0f - frac;
            arg0->unk1C = D_800A6E0C[i][0] * inv + D_800A6E0C[i - 1][0] * frac;
            arg0->unk1D = D_800A6E0C[i][1] * inv + D_800A6E0C[i - 1][1] * frac;
            arg0->unk1E = D_800A6E0C[i][2] * inv + D_800A6E0C[i - 1][2] * frac;
            break;
        }
    }
    return 0;
}

/* The ROM has 8 bytes of `jr ra; nop` between func_1516E8CC and func_1516ECAC.
 * splat has no symbol for them so they fold into func_1516E8CC's size; they are an
 * empty leaf function, the same shape game_981E0.c func_1507304C and
 * game_1A5440.c func_15178750_pad already reconstruct. */
static void func_1516ECA4_pad(void) {
}

s32 func_1516ECAC(Obj1516FadeState *arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a1;
    s32 temp;

    temp_v0 = arg0->field_0x24;
    temp_v1 = arg0->field_0x1F;
    if (temp_v0 != 0) {
        if (temp_v1 != 0xFF) {
            temp_v1 += D_800BE9E4 << 7;
            if (temp_v1 >= 0x100) {
                temp_v1 = 0xFF;
            }
            arg0->field_0x1F = temp_v1;
            temp_v0 = ((volatile Obj1516FadeState *)arg0)->field_0x24;
        }
    } else {
        if (temp_v1 != 0) {
            temp_v1 -= D_800BE9E4 * arg0->field_0x26;
            if (temp_v1 < 0) {
                temp_v1 = 0;
            }
            arg0->field_0x1F = temp_v1;
            temp_v0 = ((volatile Obj1516FadeState *)arg0)->field_0x24;
        }
    }

    if ((temp_v0 == 0) && (temp_v1 == 0)) {
        return 1;
    }

    temp_a1 = (arg0->field_0x2C << 8) | arg0->field_0x2D;
    temp = (temp_a1 * temp_v1) >> 7;
    arg0->field_0x16 = temp;
    arg0->field_0x14 = temp;
    return 0;
}


void func_1516ED68(s32 a0, s32 a1, u8 a2, s32 a3) {
    u8 v;
    v = D_800CC2D4[a0 * 812];
    if (v != 0x3A) {
        return;
    }
    func_1516D99C(5, 0, 0, 4,
        0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0,
        2, 0, 0, 0, 0, 0, 0, 8,
        0, 0, 0, 0, 0, 0, 0, 0x124,
        0x124, 0, 0, a1, 0, 0, a0, 0xC,
        0, 0, 0x81, 2, 0, 1, 0, 0,
        0, 5, 0, a2, a3);
}

void func_1516EED4(s32 a0, s32 a1, u8 a2, s32 a3) {
    if (a0 < 4 && (*(u8 (*)[4])&D_800DD2A0)[a0] >= a1) {
        (*(u8 (*)[4])&D_800DD2A0)[a0] = 0;
        func_1516D99C(0, 0, 0, 4,
            0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0,
            2, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0x200,
            0x200, 0, 0, 0x1E, 0, 0, a0, 0x14,
            0, 0, 0x81, 8, 0, 1, 0, 0,
            0, 0xC, 0, a2, a3);
    }
}

s32 func_1516F024(Obj1516FadeState *arg0) {
    /* Reserves the 16 bytes at the top of the -g3 stack frame (sp+0x58..0x67)
     * that the shipped build allocated but never referenced.  Declared first so
     * the remaining locals land on their original stack homes. */
    u8 c24, c25, c26, c27, c28, c2A, c2B, c2C, c32, c33, c34, c35;
    s32 padq;
    s32 temp_a2;
    struct Table1516F024 sp48;
    struct Table1516F024 sp3C;
    struct Table1516F024 sp30;
    struct Table1516F024 sp24;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a1;

    sp48 = D_8008CBC4;
    sp3C = D_8008CBD0;
    sp30 = D_8008CBDC;
    sp24 = D_8008CBE8;

    temp_a1 = arg0->field_0x24;
    temp_a2 = arg0->field_0x2C;
    temp_v0 = arg0->field_0x1F;
    if (temp_a1 != 0) {
        temp_v1 = sp30.unk0[temp_a2];
        if (temp_v0 != temp_v1) {
            temp_v0 += D_800BE9E4 << 4;
            if (temp_v1 < temp_v0) {
                temp_v0 = temp_v1;
            }
            arg0->field_0x1F = temp_v0;
            temp_a1 = ((volatile Obj1516FadeState *)arg0)->field_0x24;
        }
    } else {
        if (temp_v0 != 0) {
            temp_v0 -= D_800BE9E4 * sp24.unk0[temp_a2];
            if (temp_v0 < 0) {
                temp_v0 = 0;
            }
            arg0->field_0x1F = temp_v0;
            temp_a1 = ((volatile Obj1516FadeState *)arg0)->field_0x24;
        }
    }

    if ((temp_a1 == 0) && (temp_v0 == 0)) {
        return 1;
    }

    func_1516F864((s32)arg0);
    temp_v0 = arg0->field_0x14;
    temp_v0 += D_800BE9E4 * sp3C.unk0[temp_a2];
    arg0->field_0x16 = temp_v0;
    arg0->field_0x14 = temp_v0;
    if (temp_a2 == 3) {
        arg0->field_0x18 = 0x64;
    }
    func_1516F94C((s32)arg0, sp48.unk0[temp_a2]);
    return 0;
}


void func_1516F1C0(s32 a0, s32 a1, u8 a2, s32 a3) {
    func_1516D99C(2, 0, 0, 0x2D,
        0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0,
        2, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0x50,
        0x50, 0, 0, a1, 0xA, 0xA, a0, 0x3C,
        0, 0, 0x81, 4, 0, 1, 0, 0,
        0, 4, 2, a2, a3);
}

void func_1516F2F8(u8 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    func_1516D99C(3, 0, 0, 0x2B,
        0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0,
        2, 0, 0, 0, 0, 0, 0, 2,
        0, 0, 0, 0, 0, 0, 0, 0x50,
        0x50, 0, 0, a1, 4, 4, a0, 0x10,
        0, 0, 0x81, 4, 0, 1, 0, 0,
        0, 1, 0, LOW_U8(a4), a5);
    func_1516D99C(3, 0, 0, 0x2B,
        0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0,
        2, 0, 0, 0, 0, 0, 0, 3,
        0, 0, 0, 0, 0, 0, 0, 0x50,
        0x50, 0, 0, 7, 4, 4, a0, a2,
        0, 0, 0x81, 5, 0, 1, 0, 0,
        0, 1, a3, LOW_U8(a4), a5);
}

/* Spawns two type-0xD particle emitters at (arg0, arg1, arg2) -- identical apart
 * from their spawn table entry (arg10 vs arg11) and the second one's extra mode
 * byte 6 -- then plays 3-D sound 7 at the same spot with arg3 as its user data.
 * The position is latched into posX/posY/posZ while the first emitter is being
 * built so both later calls reuse it. */
void func_1516F548(
    s32 arg0, s32 arg1, s32 arg2, s32 arg3,
    s32 arg4, s32 arg5, s32 arg6, s32 arg7,
    s32 arg8, s32 arg9, s32 arg10, s32 arg11,
    s32 arg12, s32 arg13, s32 arg14, s32 arg15,
    s32 arg16) {

    func_1516D99C((s16)arg0, (s16)arg1, (s16)arg2, 0xD,
        0, 0x67, 0x17, 0xD, 0, 0, 0x12, 0,
        2, (u8)arg4, 0, (u8)arg6, 0, 0, 0, arg10,
        0, 0, 0, 0, 0, 0, 0, (s16)((arg7 << 12) / 4096),
        (s16)((arg7 << 12) / 4096), (s16)((arg8 << 12) / 4096), (s16)((arg8 << 12) / 4096), (u16)arg3, (u8)arg9, 0x14, 0xFF, 0x10,
        (s16)arg5, 0xC8, 1, 0, 0, 1, 0, 0,
        0, 0, 0, LOW_U8(arg15), arg16);
    func_1516D99C((s16)arg0, (s16)arg1, (s16)arg2, 0xD,
        0, 0x67, 0x17, 0xD, 0, 0, 0x12, 0,
        2, (u8)arg4, 0, (u8)arg6, 0, 0, 0, arg11,
        0, 0, 0, 0, 0, 0, 0, (s16)((arg7 << 12) / 4096),
        (s16)((arg7 << 12) / 4096), (s16)((arg8 << 12) / 4096), (s16)((arg8 << 12) / 4096), (u16)arg3, (u8)arg9, 0x14, 0xFF, 0x10,
        (s16)arg5, 0xC8, 1, 6, 0, 1, 0, 0,
        0, 0, 0, LOW_U8(arg15), arg16);
    func_1000FA64(7, (s16)arg0, (s16)arg1, (s16)arg2, 0x36B0, 0x3E8, 0x64,
        (s32)func_1000EBC4, (void *)arg3, 0, 0, 0);
}

typedef struct Game19A8B0Motion {
    u8 pad0[0xE];
    s16 x;
    s16 y;
    s16 z;
    s16 scaleX;
    s16 scaleY;
    s16 velocityY;
    u8 pad1A[5];
    u8 alpha;
    u8 pad20[3];
    u8 kind;
    u8 active;
    u8 yFraction;
    s8 velocityXHigh;
    u8 velocityXLow;
    s8 velocityZHigh;
    u8 velocityZLow;
    u8 xFraction;
    u8 zFraction;
    u8 sizeHigh;
    u8 sizeLow;
    u8 pad2E[2];
    void *link;
} Game19A8B0Motion;

#define MOTION ((Game19A8B0Motion *)arg0)
void func_1516F864(s32 arg0) {
    s32 x;
    s32 velocity;
    s32 z;

    x = MOTION->x;
    x <<= 8;
    x += MOTION->xFraction;
    velocity = MOTION->velocityXHigh;
    velocity <<= 8;
    velocity += MOTION->velocityXLow;
    x += velocity * D_800BE9E4;
    MOTION->x = x >> 8;
    x = (u8)x;
    MOTION->xFraction = x;
    z = MOTION->z;
    z <<= 8;
    z += MOTION->zFraction;
    velocity = MOTION->velocityZHigh;
    velocity <<= 8;
    velocity += MOTION->velocityZLow;
    z += velocity * D_800BE9E4;
    MOTION->z = z >> 8;
    z = (u8)z;
    MOTION->zFraction = z;
}
#undef MOTION

void func_1516F8EC(s32 arg0, s32 arg1)
{
  u8 *ptr;
  s32 val;
  ptr = (u8 *) arg0;
  val = (*((s8 *) (ptr + 0x26))) << 8;
  val += ptr[0x27];
  val = (val * arg1) >> 8;
  ptr[0x26] = val >> 8;
  ptr[0x27] = val & 0xFFFFu;
}

extern void func_1516F8EC(s32, s32);


void func_1516F91C(s32 arg0, s32 arg1)
{
  s32 v0;
  v0 = (*((s8 *) (arg0 + 0x28))) << 8;
  v0 += *((u8 *) (arg0 + 0x29));
  v0 = (v0 * arg1) >> 8;
  *((u8 *) (arg0 + 0x28)) = v0 >> 8;
  *((u8 *) (arg0 + 0x29)) = v0 & 0xFF;
}

extern void func_1516F91C(s32, s32);


extern void func_1516F8EC(s32, s32);
extern void func_1516F91C(s32, s32);

void func_1516F94C(s32 arg0, s32 arg1) {
    func_1516F8EC(arg0, arg1);
    func_1516F91C(arg0, arg1);
}

void func_1516F984(Obj1516F984 *arg0, s32 arg1) {
    s32 temp;
    func_1516F94C((s32)arg0, arg1);
    temp = arg0->field_0x18;
    temp = temp * arg1;
    arg0->field_0x18 = temp >> 8;
}

s32 func_1516F9C4(Obj1516FadeStepState *arg0) {
    s32 temp_a1;
    s32 temp_v0;
    s32 temp;

    temp_a1 = arg0->field_0x24;
    temp_v0 = arg0->field_0x1F;
    if (temp_a1 != 0) {
        if (temp_v0 != 0xFF) {
            temp_v0 += D_800BE9E4 << 4;
            if (temp_v0 >= 0x100) {
                temp_v0 = 0xFF;
            }
            arg0->field_0x1F = temp_v0;
            temp_a1 = ((volatile Obj1516FadeStepState *)arg0)->field_0x24;
        }
    } else {
        if (temp_v0 != 0) {
            temp_v0 -= D_800BE9E4 << 3;
            if (temp_v0 < 0) {
                temp_v0 = 0;
            }
            arg0->field_0x1F = temp_v0;
        }
        temp = (temp_v0 << 9) >> 8;
        arg0->field_0x16 = temp;
        arg0->field_0x14 = temp;
        temp_a1 = ((volatile Obj1516FadeStepState *)arg0)->field_0x24;
    }

    if ((temp_a1 == 0) && (temp_v0 == 0)) {
        return 1;
    }

    func_1516F864((s32)arg0);
    func_1516F984((Obj1516F984 *)arg0, 0xF0);
    return 0;
}

void func_1516FA88(s16 arg0, s16 arg1, s16 arg2, s32 arg3, u8 arg4, s32 arg5) {
    func_1516D99C(arg0, arg1, arg2, 0x1B,
        0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0xFF,
        3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0x400,
        0x400, 0, 0, 2, 0, 0, 0xFF, 0xF,
        0, 0, 9, 0, 0xFF, 0xA, 0x5A, 5,
        arg3, 0, 0, arg4, arg5);
}

typedef struct {
    char pad_0[0x14];
    s16 sizeX;
    s16 sizeY;
    char pad_0x18[0x4];
    u8 r;
    u8 g;
    u8 b;
    u8 alpha;
    char pad_0x20[0x4];
    u8 fadingIn;
    char pad_0x25[0x1];
    u8 r0;
    u8 g0;
    u8 b0;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 blend;
    s8 growX;
    s8 growY;
    u8 fadeShift;
} Obj1516FBCC;

s32 func_1516FBCC(Obj1516FBCC *arg0) {
    s32 v; /* alpha, then growth, then blend */
    s32 lo;
    s32 hi;

    v = arg0->alpha;
    if (arg0->fadingIn != 0) {
        if (v != 0xFF) {
            v += D_800BE9E4 << 4;
            if (v >= 0x100) {
                v = 0xFF;
            }
            arg0->alpha = v;
        }
    } else {
        if (v != 0) {
            v -= D_800BE9E4 << arg0->fadeShift;
            if (v < 0) {
                v = 0;
            }
            arg0->alpha = v;
        }
    }
    if ((arg0->fadingIn == 0) && (v == 0)) {
        return 1;
    }
    v = arg0->growX;
    v *= D_800BE9E4;
    arg0->sizeX += v;
    v = arg0->growY;
    v *= D_800BE9E4;
    arg0->sizeY += v;
    /* BUG (original game): sizeX is tested twice, sizeY never */
    if ((arg0->sizeX <= 0) || (arg0->sizeX <= 0)) {
        arg0->sizeX = arg0->sizeY = 0;
        return 1;
    }
    v = arg0->blend;
    v += D_800BE9E4;
    if (v >= 0x80) {
        v = 0x7F;
    }
    arg0->blend = v;
    v *= 2;
    lo = arg0->r0;
    hi = arg0->r1;
    arg0->r = (((hi - lo) * v) >> 8) + lo;
    lo = arg0->g0;
    hi = arg0->g1;
    arg0->g = (((hi - lo) * v) >> 8) + lo;
    lo = arg0->b0;
    hi = arg0->b1;
    arg0->b = (((hi - lo) * v) >> 8) + lo;
    return 0;
}

s32 func_1516FD50(Obj1516FadeStepState *arg0) {
    s32 temp_a1;
    s32 temp_v0;
    s32 temp;

    temp_a1 = arg0->field_0x24;
    temp_v0 = arg0->field_0x1F;
    if (temp_a1 != 0) {
        if (temp_v0 != 0xFF) {
            temp_v0 += D_800BE9E4 << 4;
            if (temp_v0 >= 0x100) {
                temp_v0 = 0xFF;
            }
            arg0->field_0x1F = temp_v0;
            temp_a1 = ((volatile Obj1516FadeStepState *)arg0)->field_0x24;
        }
    } else {
        if (temp_v0 != 0) {
            temp_v0 -= D_800BE9E4 << 3;
            if (temp_v0 < 0) {
                temp_v0 = 0;
            }
            arg0->field_0x1F = temp_v0;
        }
        temp = (temp_v0 << 9) >> 8;
        arg0->field_0x16 = temp;
        arg0->field_0x14 = temp;
        temp_a1 = ((volatile Obj1516FadeStepState *)arg0)->field_0x24;
    }

    if ((temp_a1 == 0) && (temp_v0 == 0)) {
        return 1;
    }

    func_1516F864((s32)arg0);
    arg0->field_0x18 = arg0->field_0x18 + arg0->field_0x2C;
    return 0;
}

void func_1516FE1C(s32 arg0, s32 arg1, u8 arg2, s32 arg3) {
    s32 sp_s2;
    s16 posHi;
    s16 posLo;
    s32 var_a1;
    s32 i;
    s32 var_s1;
    s32 pos;

    var_s1 = 0;
    i = 0;
    if (D_800CC2D0[arg0].interaction_state == 1) {
        posHi = 4;
        posLo = 0;
        sp_s2 = 0x19;
        var_a1 = 1;
    } else {
        pos = (s32)&D_800CC2D0[arg0].x_position;
        posHi = (s16)(pos >> 16);
        posLo = (s16)(pos & 0xFFFF);
        arg0 = 0xFF;
        var_a1 = 0x21;
    }
    do {
        if (i == 1) {
            sp_s2 = 0x1B;
            var_s1 = 8;
        }
        func_1516D99C(posHi, posLo, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, var_s1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xAA, 0xAA, 0xAA, 0xAA, (u16)arg1, 0x1E, 0, (u8)arg0, 0x1E, 0x168, 0x18, (u16)var_a1, 4, 0, 1, 0, 0, 0, sp_s2, 0, arg2, arg3);
        i++;
    } while (i != 2);
}

extern u8 func_150ADA20(void);
extern f32 func_150489B0(u8);
extern f32 func_15048A40(u8);

void func_15170034(s32 arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
    s32 mask;
    f32 r;
    f32 sp24;
    f32 sp20;
    s32 v;

    mask = (arg0 & 0x40) ? 0x3F : 0x7F;
    v = func_150ADA20() & mask;
    *arg2 = func_150489B0((u8)v);
    sp20 = func_15048A40((u8)v);
    v = func_150ADA20() & 0xFF;
    sp24 = func_150489B0((u8)v);
    r = func_15048A40((u8)v);
    *arg1 = sp20 * r;
    *arg3 = sp20 * sp24;
}

struct Vec3f151700D8 { f32 v[3]; };
struct Col151700D8 { u8 c[3][3]; };
extern struct Vec3f151700D8 D_8008CBF4;
extern struct Vec3f151700D8 D_8008CC00;
extern struct Col151700D8 D_8008CC0C;

void func_151700D8(f32 (*verts)[3], f32 x, f32 z, u8 alpha, f32 scale, s32 type, s32 n0, s32 n1, u8 arg8, s32 arg9) {
    s32 i;
    s32 j;
    s32 next;
    s32 life;
    s32 size;
    u8 *col;
    f32 t;
    f32 px;
    f32 py;
    f32 pz;
    s32 dx;
    s32 dz;
    f32 *p1;
    s32 sizeMin;
    s32 sizeVar;
    s32 sizeRange;
    s32 lifeMin;
    s32 life2;
    f32 *p0;
    s32 a;
    s32 pad7;
    s32 count;
    struct Vec3f151700D8 unused;
    struct Vec3f151700D8 sizes;
    struct Col151700D8 colors;

    unused = D_8008CBF4;
    sizes = D_8008CC00;
    colors = D_8008CC0C;
    sizeRange = sizes.v[type] * scale;
    sizeMin = 731.0f * scale;
    sizeVar = 836.0f * scale;
    lifeMin = 143.0f * scale;
    p0 = verts[0];
    for (i = 0; i < 4; i++) {
        if (i != 3) {
            next = i + 1;
        } else {
            next = 0;
        }
        if (i == 0 || i == 2) {
            count = n0;
        } else {
            count = n1;
        }
        p1 = verts[next];
        dx = (((p0[0] + p1[0]) * 0.5f) - x) * 2.0f;
        px = (p0[1] + p1[1]) * 0.5f;
        dz = (((p0[2] + p1[2]) * 0.5f) - z) * 2.0f;
        for (j = 0; j < count; j++) {
            t = j * (1.0f / (count - 1));
            col = colors.c[type];
            a = alpha;
            px = p0[0] + (p1[0] - p0[0]) * t;
            py = p0[1] + (p1[1] - p0[1]) * t;
            pz = p0[2] + (p1[2] - p0[2]) * t;
            if (i) {
            }
            life = (func_150ADA20() & 0x1F) + 0xF;
            size = (func_150ADA20() % (u32)sizeVar) + sizeMin;
            life2 = (func_150ADA20() % (u32)sizeRange) + lifeMin;
            func_1516D4E8((s16)(s32)px, (s16)(s32)py, (s16)(s32)pz, 0xD, 0,
                col[0], col[1], col[2], 0, 0, 0, 0, 2,
                (u8)((dx >> 8) & 0xFF), (u8)(dx & 0xFF), (u8)((dz >> 8) & 0xFF), (u8)(dz & 0xFF), 0, 0, a, 0, 0, 0,
                (s16)size, (s16)size, life,
                life2,
                0, 0, arg8, arg9);
        }
        p0 += 3;
    }
}

extern s32 func_151EF610(void);

void func_15170500(s16 arg0, s16 arg1, s16 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    f32 x;
    f32 z;
    f32 y;
    f32 s;
    s32 ix;
    s32 iy;
    s32 iz;
    f32 w;
    s32 p2;
    s32 r;

    z = 0.0f;
    if (arg4 == 0) {
        arg3 = D_800CC34A[0];
    }
    r = func_151EF610() % 50;
    x = func_150489B0((u8)arg3);
    y = -func_15048A40((u8)arg3);
    s = func_150489B0((u8)r);
    x *= s;
    y *= s;
    w = z * s;
    x += x; y += y; w += w;
    if (arg4 != 0) {
        ix = x * 256.0f;
        iy = y * 256.0f;
        iz = w * 256.0f;
        func_1516D4E8(arg0, arg1, arg2, 0x2A, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 6,
            ix >> 8, ix & 0xFF, iy >> 8, iy & 0xFF, 0, 0, 0, 0, 0, 1, 0x200, 0x166, 0xFF,
            iz - 0x32, 0, 0, arg5, arg6);
    } else {
        ix = x * 256.0f;
        iy = y * 256.0f;
        iz = w * 256.0f;
        func_1516D99C(arg0, arg1, arg2, 0x2A, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 6,
            ix >> 8, ix & 0xFF, iy >> 8, iy & 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x200, 0x166,
            0, 0, 0x14, 0, 0, 0xFF, 0xFF, iz - 0x32, 0xC8, 9, 1, 0, 1, 0x32, 0x14, 0, 0, 0,
            arg5, arg6);
    }
}

typedef struct {
    char pad_0[0x18];
    s16 field_0x18;
    char pad_0x1A[0x5];
    u8 field_0x1F;
    char pad_0x20[0x4];
    u8 field_0x24;
    char pad_0x25[0x1];
    s8 field_0x26;
    u8 field_0x27;
    char pad_0x28[0x7];
    u8 field_0x2F;
} Obj151707E0;

extern s32 func_151EF610(void);

s32 func_151707E0(Obj151707E0 *arg0) {
    s32 active;
    s32 temp;
    s32 velX;

    active = arg0->field_0x24;
    temp = arg0->field_0x1F;
    if (active != 0) {
        if (temp == 0) {
            arg0->field_0x24 = (func_150ADA20() % 0x19U) + 0xC8;
        }
        if (temp != 0xFE) {
            temp += D_800BE9E4 << 6;
            if (temp >= 0xFF) {
                temp = 0xFE;
            }
            arg0->field_0x1F = temp;
        }
    } else {
        if (temp != 0) {
            temp -= D_800BE9E4 << 4;
            if (temp < 0) {
                temp = 0;
            }
            arg0->field_0x1F = temp;
        }
    }

    if ((arg0->field_0x24 == 0) && (temp == 0)) {
        return 1;
    }

    func_1516F864((s32) arg0);
    if (arg0->field_0x2F != 0) {
        func_1516F8EC((s32) arg0, (func_151EF610() % 0x20) + 0xE6);
        func_1516F91C((s32) arg0, (func_151EF610() % 0x20) + 0xE6);
        arg0->field_0x18 = 0;
    } else {
        velX = (arg0->field_0x26 << 8) + arg0->field_0x27;
        temp = arg0->field_0x26 * 0x100;
        temp += arg0->field_0x27;
        if (((velX * velX) + (temp * temp)) >= 0x7D1) {
            func_1516F8EC((s32) arg0, (func_151EF610() % 0x20) + 0xDC);
            func_1516F91C((s32) arg0, (func_151EF610() % 0x20) + 0xDC);
        }
        arg0->field_0x18 = 0;
    }
    return 0;
}

void func_151709B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, u8 arg5, s32 arg6) {
    s32 i;
    f32 x;
    f32 y;
    f32 z;
    s32 ix;
    s32 iz;
    s32 iy;
    s32 pad;
    s32 pad2;

    for (i = 0; i < arg3; i++) {
        func_15170034(0x40, &x, &y, &z);
        x *= arg4;
        y *= arg4 + arg4;
        z *= arg4;
        ix = x;
        iz = z;
        iy = y;
        func_1516D4E8(arg0, arg1, arg2, 0x2F, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0, 0, 7,
            ix >> 8, ix & 0xFF, iz >> 8, iz & 0xFF, 0, 0, 0, 0, 0, 0, 0x200, 0x200, 0x28,
            iy, 0, 0, arg5, arg6);
    }
}
