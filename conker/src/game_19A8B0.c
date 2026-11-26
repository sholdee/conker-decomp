#include <ultra64.h>
#include "functions.h"
#include "variables.h"

u8 D_800DD2A0[4];

/* not referenced */
static const u8 D_800A6DF0[16] = {
    0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00,
};

/* The intensity ramps of func_1516E8CC: descending thresholds and the colour at each. */
static const u8 D_800A6E00[12] = { 255, 40, 35, 30, 25, 20, 15, 0, 255, 40, 0, 0 };
static const u8 D_800A6E0C[12][3] = {
    { 0xFF, 0xFF, 0x00 }, { 0xFF, 0xFF, 0x00 }, { 0xFF, 0x80, 0x00 }, { 0xFF, 0x80, 0x00 },
    { 0xFF, 0x00, 0x00 }, { 0xFF, 0x00, 0x00 }, { 0xC0, 0x00, 0x00 }, { 0x80, 0x00, 0x00 },
    { 0x5A, 0x5A, 0x5A }, { 0x5A, 0x5A, 0x5A }, { 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00 },
};

/* Emission segments of func_1516DB90 by emitter->x: from (x, y, z), to (x, y, z), random z mask. */
static const s16 D_800A6E30[6][7] = {
    { 0, 0, 34, 0, 0, 96, 0 },
    { 0, 0, 190, 0, 0, 0, 0 },
    { 0, 10, 20, 0, 10, 120, 0 },
    { 0, 0, -20, 0, 0, -128, 0 },
    { 0, 0, 0, 0, 0, 0, 0 },
    { 0, 50, -30, 0, -260, 390, 127 },
};

/* Render setup and teardown of the style particles (object types 18 and 103 of D_8008B4A8). */
Gfx D_8008CB70[] = {
    gsSPDisplayList(D_800873D0),
    gsDPSetCombineLERP(PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0,
                       PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, 0, TEXEL0, 0),
    gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2),
    gsSPEndDisplayList(),
};
Gfx D_8008CB90[] = {
    gsSPDisplayList(D_80087408),
    gsSPEndDisplayList(),
};

/* u8 tables indexed by a particle style */
struct StyleTable {
    u8 v[11];
};

void func_1516D400(void) {
    s32 i;
    s32 intensity;

    for (i = 0; i < 4; i++) {
        if (D_800DD2A0[i] < 0xFF) {
            intensity = D_800DD2A0[i] + D_800BE9A0;
            if (intensity > 0xFF) {
                intensity = 0xFF;
            }
            D_800DD2A0[i] = intensity;
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
    StyleParticle *particle;

    particle = func_15167A68(isAlt != 0 ? 0x67 : 0x12, unk1, sizeof(StyleParticle), 0, unkC, 1);
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

/* Update handlers by StyleParticle.kind (1 frees the particle), declared without a prototype: two of them
 * take other particle types. */
static s32 (*D_8008CBA0[])() = {
    func_1516E8CC, func_1516ECAC, func_1516F024, func_1516F9C4, func_1516FBCC,
    func_1516FD50, func_151707E0, func_1516A648, func_150BF0F4,
};

void func_1516D678(StyleParticle *particle) {
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
        func_1516972C(particle);
    } else {
        /* y is 8.8 fixed point with its fraction in yFraction */
        value = particle->y;
        value = (value << 8) + particle->yFraction;
        value += particle->velocityY * D_800BE9E4;
        particle->y = value >> 8;
        particle->yFraction = value & 0xFF;
    }
}


Gfx *func_1516D738(Gfx *gfx, StyleParticle *particle, s32 arg2) {
    SpriteParams sprite; // func_15142E24 points it at the texture, func_15095760 draws it
    s32 pad; // matching: unused stack slot in the original frame
    TextureDesc *texture;
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
    // matching: the pointer goes through an integer (IDO keeps the converted value in a temporary)
    gfx = func_15142E24(gfx, (TextureDesc *)(s32)D_8008CA4C[particle->texture], frameIndex << 8, 2, 0x100, 0x100, 0, 6,
                        &sprite, (u8 *)&synced, 3);
    if (texture->fmt == 5) {
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
    sprite.flip = 0;
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
    SpriteEmitter *emitter;

    emitter = func_15167A68(isAlt != 0 ? 0x66 : 0x11, unk1, sizeof(SpriteEmitter), 0, unkC, 1);
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




void func_1516DB90(SpriteEmitter *arg0) {
    SpriteEmitter *emitter; // matching: IDO keeps this copy of the argument in its own register
    s32 i;
    s16 scaleX;
    s16 scaleY;
    s32 velocityY;
    s16 x;
    s16 y;
    union {
        s32 word;
        u8 bytes[4];
    } fixed;
    s32 velXHigh;
    s32 velXLow;
    s32 velZHigh;
    s32 velZLow;
    s16 z;
    s32 pad; // matching: unused stack slot in the original frame
    s16 pad24, pad25; // matching: unused stack slots in the original frame
    // matching: the template bytes are copied into s16 locals before the call, in this order
    s16 r, g, b, envR, envG, envB, unk32, unk33, unk34, unk35;
    f32 fromX;
    f32 fromY;
    f32 fromZ;
    f32 toX;
    f32 toY;
    f32 toZ;
    f32 velX;
    f32 velY;
    f32 velZ;
    f32 speed;
    f32 offset;
    f32 s;
    f32 c;
    u8 angle;
    u8 pad2; // matching: unused stack slot in the original frame
    u8 roll;
    s32 threshold;
    s32 scaleRandom;
    s32 isAlt;
    s32 countdown;

    emitter = arg0;
    if (emitter->unk0 == 0x11) {
        isAlt = 0;
    } else {
        isAlt = 1;
    }
    countdown = emitter->countdown;
    countdown -= D_800BE9E4;
    while (countdown < 0) {
        if (emitter->interval != 0) {
            countdown += emitter->interval;
        } else {
            countdown = 0;
        }
        for (i = 0; i < emitter->burst; i++) {
            if (emitter->chance != 0) {
                roll = func_150ADA20() & 0xFF;
                if (emitter->flags & 4) {
                    threshold = emitter->chance;
                    threshold = (emitter->timer * threshold) / emitter->initialTimer;
                } else {
                    threshold = emitter->chance;
                }
            }
            if (emitter->chance == 0 || roll < threshold) {
                if (emitter->object == 0xFF) {
                    if (emitter->flags & 0x10) {
                        /* x:y hold a pointer to an s16 position */
                        s16 *sv = (s16 *)((u16)emitter->y | (emitter->x << 16));
                        x = sv[0];
                        y = sv[1] + emitter->z;
                        z = sv[2];
                    } else if (emitter->flags & 0x20) {
                        /* x:y hold a pointer to an f32 position */
                        f32 *fv = (f32 *)((u16)emitter->y | (emitter->x << 16));
                        x = fv[0];
                        y = fv[1] + emitter->z;
                        z = fv[2];
                    } else {
                        x = emitter->x;
                        y = emitter->y;
                        z = emitter->z;
                    }
                } else {
                    if (movement_obj[emitter->object].ob_matrix_ptr == NULL) {
                        continue;
                    }
                    fromX = D_800A6E30[emitter->x][0];
                    fromY = D_800A6E30[emitter->x][1];
                    fromZ = D_800A6E30[emitter->x][2];
                    toX = D_800A6E30[emitter->x][3];
                    toY = D_800A6E30[emitter->x][4];
                    toZ = D_800A6E30[emitter->x][5];
                    if (D_800A6E30[emitter->x][6] != 0) {
                        toZ += (u32)(func_150ADA20() & D_800A6E30[emitter->x][6]);
                    }
                    func_150A7960(movement_obj[emitter->object].ob_matrix_ptr[emitter->bone], fromX, fromY, fromZ, &fromX, &fromY, &fromZ);
                    func_150A7960(movement_obj[emitter->object].ob_matrix_ptr[emitter->bone], toX, toY, toZ, &toX, &toY, &toZ);
                    velX = (toX - fromX) * 8.0f;
                    velY = (toY - fromY) * 8.0f;
                    velZ = (toZ - fromZ) * 8.0f;
                    velocityY = velY;
                    angle = 0x40 - movement_obj[emitter->object].angle;
                    velX -= movement_obj[emitter->object].speed * func_15048A40(angle) * 64.0f;
                    velZ -= movement_obj[emitter->object].speed * func_150489B0(angle) * 64.0f;
                    fixed.word = velX;
                    velXHigh = fixed.bytes[2];
                    velXLow = fixed.bytes[3];
                    fixed.word = velZ;
                    velZHigh = fixed.bytes[2];
                    velZLow = fixed.bytes[3];
                    x = fromX;
                    y = fromY;
                    z = fromZ;
                }
                if (emitter->flags & 8) {
                    func_15170034(emitter->flags, &velX, &velY, &velZ);
                    speed = emitter->speed * 16;
                    offset = (u32)emitter->startOffset;
                    y += offset * velY;
                    velY = speed * velY;
                    velocityY = velY;
                    func_150ADA20();
                    x += velX * offset;
                    z += velZ * offset;
                    velX *= speed;
                    velZ *= speed;
                    fixed.word = velX;
                    velXHigh = fixed.bytes[2];
                    velXLow = fixed.bytes[3];
                    fixed.word = velZ;
                    velZHigh = fixed.bytes[2];
                    velZLow = fixed.bytes[3];
                }
                if (emitter->radiusXZ != 0) {
                    angle = func_150ADA20();
                    s = func_15048A40(angle);
                    c = func_150489B0(angle);
                    speed = (u32)((func_150ADA20() & 0xFFFF) * emitter->radiusXZ) * (1.0f / 65536.0f);
                    x += speed * s;
                    z += speed * c;
                }
                if (emitter->radiusY != 0) {
                    s = func_15048A40(func_150ADA20() & 0xFF);
                    speed = (u32)((func_150ADA20() & 0xFFFF) * emitter->radiusY) * (1.0f / 65536.0f);
                    y += (s32)(speed * s);
                }
                scaleX = emitter->scaleX;
                scaleY = emitter->scaleY;
                if (emitter->scaleXRandom != 0) {
                    scaleRandom = func_150ADA20() % (u32)emitter->scaleXRandom;
                } else {
                    scaleRandom = 0;
                }
                if (emitter->flags & 1) {
                    scaleX += scaleRandom;
                    scaleY += scaleRandom;
                } else {
                    scaleX += scaleRandom;
                    if (emitter->scaleYRandom != 0) {
                        scaleY += func_150ADA20() % (u32)emitter->scaleYRandom;
                    }
                }
                if (!(emitter->flags & 8) && !(emitter->flags & 0x80)) {
                    velocityY = emitter->velocityY;
                    if (emitter->velocityYRandom != 0) {
                        velocityY += func_150ADA20() % emitter->velocityYRandom;
                    }
                    velXHigh = emitter->velocityXHigh;
                    if (emitter->velocityXHighRandom != 0) {
                        velXHigh += func_150ADA20() % emitter->velocityXHighRandom;
                        if (velXHigh >= 0x100) {
                            velXHigh = 0xFF;
                        }
                    }
                    velXLow = emitter->velocityXLow;
                    if (emitter->velocityXLowRandom != 0) {
                        velXLow += func_150ADA20() % emitter->velocityXLowRandom;
                        if (velXLow >= 0x100) {
                            velXLow = 0xFF;
                        }
                    }
                    velZHigh = emitter->velocityZHigh;
                    if (emitter->velocityZHighRandom != 0) {
                        velZHigh += func_150ADA20() % emitter->velocityZHighRandom;
                        if (velZHigh >= 0x100) {
                            velZHigh = 0xFF;
                        }
                    }
                    velZLow = emitter->velocityZLow;
                    if (emitter->velocityZLowRandom != 0) {
                        velZLow += func_150ADA20() % emitter->velocityZLowRandom;
                        if (velZLow >= 0x100) {
                            velZLow = 0xFF;
                        }
                    }
                }
                unk32 = emitter->unk32;
                unk33 = emitter->unk33;
                unk34 = emitter->unk34;
                unk35 = emitter->unk35;
                envR = emitter->envR;
                envG = emitter->envG;
                envB = emitter->envB;
                r = emitter->r;
                g = emitter->g;
                b = emitter->b;
                func_1516D4E8(x, y, z, emitter->texture, emitter->frame, r, g, b, emitter->alpha, envR, envG, envB,
                              emitter->kind, velXHigh, velXLow, velZHigh, velZLow, unk32, unk33, unk34, unk35,
                              emitter->unk36, emitter->unk37, scaleX, scaleY, emitter->particleLifetime, velocityY,
                              isAlt, emitter->particleLink, emitter->unkC, emitter->unk1);
            }
        }
    }
    emitter->countdown = countdown;
    i = emitter->timer;
    if (i != 0xFFFF) {
        i -= D_800BE9E4;
        if (i > 0) {
            emitter->timer = i;
        } else {
            func_1516972C(emitter);
        }
    }
}

/* Emitter that follows the f32 position vector at `position` (flags 0x20). */
void func_1516E778(f32 *position, s32 duration, u8 texture, u8 arg3, s32 arg4) {
    func_1516D99C((s16)((u32)position >> 16), (s16)((u32)position & 0xFFFF), 0, texture,
        0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0,
        8, 8, 1, 0, 0, 0, 0, 0x155, 0x155, 0x155, 0x155, duration,
        0x1E, 0, 0xFF, 0x28, 0x168, 0x18, 0x21, 4, 0, 1,
        0, 0, 0, 0, 0, arg3, arg4);
}

s32 func_1516E8CC(StyleParticle *particle) {
    s32 alpha;
    s32 intensity;
    s32 scale;
    s32 i;
    f32 t;
    f32 u;
    StyleParticle *p; // matching: IDO keeps this copy of the argument in its own register

    p = particle;
    alpha = particle->alpha;
    if (particle->lifetime != 0) {
        if (alpha != 0xFF) {
            alpha += D_800BE9E4 << 5;
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
            p->alpha = alpha;
        }
    } else {
        if (alpha != 0) {
            alpha -= D_800BE9E4 << 4;
            if (alpha < 0) {
                alpha = 0;
            }
            p->alpha = alpha;
        }
    }

    if (particle->lifetime == 0 && alpha == 0) {
        return 1;
    }

    scale = particle->scaleX;
    intensity = particle->lifetime;
    scale += D_800BE9E4 * 10;
    particle->scaleY = scale;
    particle->scaleX = scale;

    if (particle->lifetime == 0) {
        intensity += particle->alpha >> 5;
    } else {
        intensity = intensity + 8;
    }

    for (i = p->u.ramp.start; i < p->u.ramp.start + 8; i++) {
        if (intensity >= D_800A6E00[i]) {
            t = (f32)(intensity - D_800A6E00[i]) / (f32)(D_800A6E00[i - 1] - D_800A6E00[i]);
            u = 1.0f - t;
            particle->r = D_800A6E0C[i][0] * u + D_800A6E0C[i - 1][0] * t;
            particle->g = D_800A6E0C[i][1] * u + D_800A6E0C[i - 1][1] * t;
            particle->b = D_800A6E0C[i][2] * u + D_800A6E0C[i - 1][2] * t;
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

s32 func_1516ECAC(StyleParticle *particle) {
    s32 lifetime;
    s32 alpha;
    s32 size;
    s32 scale;

    lifetime = particle->lifetime;
    alpha = particle->alpha;
    if (lifetime != 0) {
        if (alpha != 0xFF) {
            alpha += D_800BE9E4 << 7;
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
            particle->alpha = alpha;
            lifetime = ((volatile StyleParticle *)particle)->lifetime; // matching: reloaded only after the alpha store
        }
    } else {
        if (alpha != 0) {
            alpha -= D_800BE9E4 * particle->u.pulse.fadeRate;
            if (alpha < 0) {
                alpha = 0;
            }
            particle->alpha = alpha;
            lifetime = ((volatile StyleParticle *)particle)->lifetime; // matching: reloaded only after the alpha store
        }
    }

    if (lifetime == 0 && alpha == 0) {
        return 1;
    }

    size = (particle->u.pulse.sizeHigh << 8) | particle->u.pulse.sizeLow;
    scale = (size * alpha) >> 7;
    particle->scaleY = scale;
    particle->scaleX = scale;
    return 0;
}


void func_1516ED68(s32 object, s32 duration, u8 arg2, s32 arg3) {
    u8 id;

    id = movement_obj[object].character_type;
    if (id != 0x3A) {
        return;
    }
    func_1516D99C(5, 0, 0, 4,
        0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0,
        2, 0, 0, 0, 0, 0, 0, 8,
        0, 0, 0, 0, 0, 0, 0, 0x124,
        0x124, 0, 0, duration, 0, 0, object, 0xC,
        0, 0, 0x81, 2, 0, 1, 0, 0,
        0, 5, 0, arg2, arg3);
}

void func_1516EED4(s32 object, s32 threshold, u8 arg2, s32 arg3) {
    if (object < 4 && D_800DD2A0[object] >= threshold) {
        D_800DD2A0[object] = 0;
        func_1516D99C(0, 0, 0, 4,
            0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0,
            2, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0x200,
            0x200, 0, 0, 0x1E, 0, 0, object, 0x14,
            0, 0, 0x81, 8, 0, 1, 0, 0,
            0, 0xC, 0, arg2, arg3);
    }
}

/* Per-style tables, indexed by u.drift.unk2C. */
s32 func_1516F024(StyleParticle *particle) {
    u8 pad[16]; // matching: the original frame has 16 unused bytes here
    s32 style;
    struct StyleTable damping = { { 250, 250, 235, 235, 250, 250, 220, 210, 210, 220, 210 } };
    struct StyleTable growth = { { 16, 7, 7, 7, 20, 8, 40, 40, 40, 20, 20 } };
    struct StyleTable maxAlpha = { { 255, 255, 255, 255, 190, 170, 120, 200, 60, 120, 200 } };
    struct StyleTable fadeRate = { { 8, 2, 10, 1, 8, 10, 255, 10, 2, 255, 10 } };
    s32 alpha;
    s32 limit;
    s32 lifetime;

    lifetime = particle->lifetime;
    style = particle->u.drift.unk2C;
    alpha = particle->alpha;
    if (lifetime != 0) {
        limit = maxAlpha.v[style];
        if (alpha != limit) {
            alpha += D_800BE9E4 << 4;
            if (limit < alpha) {
                alpha = limit;
            }
            particle->alpha = alpha;
            lifetime = ((volatile StyleParticle *)particle)->lifetime; // matching: reloaded only after the alpha store
        }
    } else {
        if (alpha != 0) {
            alpha -= D_800BE9E4 * fadeRate.v[style];
            if (alpha < 0) {
                alpha = 0;
            }
            particle->alpha = alpha;
            lifetime = ((volatile StyleParticle *)particle)->lifetime; // matching: reloaded only after the alpha store
        }
    }

    if (lifetime == 0 && alpha == 0) {
        return 1;
    }

    func_1516F864(particle);
    alpha = particle->scaleX;
    alpha += D_800BE9E4 * growth.v[style];
    particle->scaleY = alpha;
    particle->scaleX = alpha;
    if (style == 3) {
        particle->velocityY = 100;
    }
    func_1516F94C(particle, damping.v[style]);
    return 0;
}


void func_1516F1C0(s32 object, s32 duration, u8 arg2, s32 arg3) {
    func_1516D99C(2, 0, 0, 0x2D,
        0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0,
        2, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0x50,
        0x50, 0, 0, duration, 0xA, 0xA, object, 0x3C,
        0, 0, 0x81, 4, 0, 1, 0, 0,
        0, 4, 2, arg2, arg3);
}

void func_1516F2F8(object, duration, particleLifetime, speed, arg4, arg5)
    u8 object;
    s32 duration;
    s32 particleLifetime;
    s32 speed;
    u8 arg4;
    s32 arg5;
{
    func_1516D99C(3, 0, 0, 0x2B,
        0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0,
        2, 0, 0, 0, 0, 0, 0, 2,
        0, 0, 0, 0, 0, 0, 0, 0x50,
        0x50, 0, 0, duration, 4, 4, object, 0x10,
        0, 0, 0x81, 4, 0, 1, 0, 0,
        0, 1, 0, arg4, arg5);
    func_1516D99C(3, 0, 0, 0x2B,
        0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0,
        2, 0, 0, 0, 0, 0, 0, 3,
        0, 0, 0, 0, 0, 0, 0, 0x50,
        0x50, 0, 0, 7, 4, 4, object, particleLifetime,
        0, 0, 0x81, 5, 0, 1, 0, 0,
        0, 1, speed, arg4, arg5);
}

/* Spawns two emitters at (x, y, z) that differ only in their particles' style (style0,
 * style1) and the second one's spawn interval, then plays sound 7 there with arg3 as
 * its user data. */
void func_1516F548(
    s32 x, s32 y, s32 z, s32 arg3,
    s32 velocityXHigh, s32 velocityY, s32 velocityZHigh, s32 scale,
    s32 scaleRandom, s32 radius, s32 style0, s32 style1,
    s32 arg12, s32 arg13, s32 arg14, u8 arg15,
    s32 arg16) {
    func_1516D99C((s16)x, (s16)y, (s16)z, 0xD,
        0, 0x67, 0x17, 0xD, 0, 0, 0x12, 0,
        2, (u8)velocityXHigh, 0, (u8)velocityZHigh, 0, 0, 0, style0,
        0, 0, 0, 0, 0, 0, 0, (s16)((scale << 12) / 4096),
        (s16)((scale << 12) / 4096), (s16)((scaleRandom << 12) / 4096), (s16)((scaleRandom << 12) / 4096), (u16)arg3, (u8)radius, 0x14, 0xFF, 0x10,
        (s16)velocityY, 0xC8, 1, 0, 0, 1, 0, 0,
        0, 0, 0, arg15, arg16);
    func_1516D99C((s16)x, (s16)y, (s16)z, 0xD,
        0, 0x67, 0x17, 0xD, 0, 0, 0x12, 0,
        2, (u8)velocityXHigh, 0, (u8)velocityZHigh, 0, 0, 0, style1,
        0, 0, 0, 0, 0, 0, 0, (s16)((scale << 12) / 4096),
        (s16)((scale << 12) / 4096), (s16)((scaleRandom << 12) / 4096), (s16)((scaleRandom << 12) / 4096), (u16)arg3, (u8)radius, 0x14, 0xFF, 0x10,
        (s16)velocityY, 0xC8, 1, 6, 0, 1, 0, 0,
        0, 0, 0, arg15, arg16);
    func_1000FA64(7, (s16)x, (s16)y, (s16)z, 0x36B0, 0x3E8, 0x64,
        (SoundRecordCallback)func_1000EBC4, (void *)arg3, 0, 0, 0);
}

/* x and z are 8.8 fixed point with their fractions in u.drift; the velocities are 8.8 too. */
void func_1516F864(StyleParticle *particle) {
    s32 x;
    s32 velocity;
    s32 z;

    x = particle->x;
    x <<= 8;
    x += particle->u.drift.xFraction;
    velocity = particle->u.drift.velocityXHigh;
    velocity <<= 8;
    velocity += particle->u.drift.velocityXLow;
    x += velocity * D_800BE9E4;
    particle->x = x >> 8;
    x = (u8)x;
    particle->u.drift.xFraction = x;
    z = particle->z;
    z <<= 8;
    z += particle->u.drift.zFraction;
    velocity = particle->u.drift.velocityZHigh;
    velocity <<= 8;
    velocity += particle->u.drift.velocityZLow;
    z += velocity * D_800BE9E4;
    particle->z = z >> 8;
    z = (u8)z;
    particle->u.drift.zFraction = z;
}

/* Scales the X velocity by factor / 256. */
void func_1516F8EC(StyleParticle *particle, s32 factor) {
    s32 velocity;

    velocity = particle->u.drift.velocityXHigh << 8;
    velocity += particle->u.drift.velocityXLow;
    velocity = (velocity * factor) >> 8;
    particle->u.drift.velocityXHigh = velocity >> 8;
    particle->u.drift.velocityXLow = velocity & 0xFF;
}

/* Scales the Z velocity by factor / 256. */
void func_1516F91C(StyleParticle *particle, s32 factor) {
    s32 velocity;

    velocity = particle->u.drift.velocityZHigh << 8;
    velocity += particle->u.drift.velocityZLow;
    velocity = (velocity * factor) >> 8;
    particle->u.drift.velocityZHigh = velocity >> 8;
    particle->u.drift.velocityZLow = velocity & 0xFF;
}

void func_1516F94C(StyleParticle *particle, s32 factor) {
    func_1516F8EC(particle, factor);
    func_1516F91C(particle, factor);
}

void func_1516F984(StyleParticle *particle, s32 factor) {
    s32 velocity;

    func_1516F94C(particle, factor);
    velocity = particle->velocityY;
    velocity = velocity * factor;
    particle->velocityY = velocity >> 8;
}

s32 func_1516F9C4(StyleParticle *particle) {
    s32 lifetime;
    s32 alpha;
    s32 scale;

    lifetime = particle->lifetime;
    alpha = particle->alpha;
    if (lifetime != 0) {
        if (alpha != 0xFF) {
            alpha += D_800BE9E4 << 4;
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
            particle->alpha = alpha;
            lifetime = ((volatile StyleParticle *)particle)->lifetime; // matching: reloaded only after the alpha store
        }
    } else {
        if (alpha != 0) {
            alpha -= D_800BE9E4 << 3;
            if (alpha < 0) {
                alpha = 0;
            }
            particle->alpha = alpha;
        }
        scale = (alpha << 9) >> 8;
        particle->scaleY = scale;
        particle->scaleX = scale;
        lifetime = ((volatile StyleParticle *)particle)->lifetime; // matching: reloaded only after the scale stores
    }

    if (lifetime == 0 && alpha == 0) {
        return 1;
    }

    func_1516F864(particle);
    func_1516F984(particle, 0xF0);
    return 0;
}

void func_1516FA88(s16 x, s16 y, s16 z, s32 isAlt, u8 arg4, s32 arg5) {
    func_1516D99C(x, y, z, 0x1B,
        0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0xFF, 0xFF,
        3, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0x400,
        0x400, 0, 0, 2, 0, 0, 0xFF, 0xF,
        0, 0, 9, 0, 0xFF, 0xA, 0x5A, 5,
        isAlt, 0, 0, arg4, arg5);
}

s32 func_1516FBCC(StyleParticle *particle) {
    s32 v; /* alpha, then growth, then blend */
    s32 from;
    s32 to;

    v = particle->alpha;
    if (particle->lifetime != 0) {
        if (v != 0xFF) {
            v += D_800BE9E4 << 4;
            if (v >= 0x100) {
                v = 0xFF;
            }
            particle->alpha = v;
        }
    } else {
        if (v != 0) {
            v -= D_800BE9E4 << particle->u.tint.fadeShift;
            if (v < 0) {
                v = 0;
            }
            particle->alpha = v;
        }
    }
    if (particle->lifetime == 0 && v == 0) {
        return 1;
    }
    v = particle->u.tint.growX;
    v *= D_800BE9E4;
    particle->scaleX += v;
    v = particle->u.tint.growY;
    v *= D_800BE9E4;
    particle->scaleY += v;
    /* BUG (original game): scaleX is tested twice, scaleY never */
    if (particle->scaleX <= 0 || particle->scaleX <= 0) {
        particle->scaleX = particle->scaleY = 0;
        return 1;
    }
    v = particle->u.tint.blend;
    v += D_800BE9E4;
    if (v >= 0x80) {
        v = 0x7F;
    }
    particle->u.tint.blend = v;
    v *= 2;
    from = particle->u.tint.r0;
    to = particle->u.tint.r1;
    particle->r = (((to - from) * v) >> 8) + from;
    from = particle->u.tint.g0;
    to = particle->u.tint.g1;
    particle->g = (((to - from) * v) >> 8) + from;
    from = particle->u.tint.b0;
    to = particle->u.tint.b1;
    particle->b = (((to - from) * v) >> 8) + from;
    return 0;
}

s32 func_1516FD50(StyleParticle *particle) {
    s32 lifetime;
    s32 alpha;
    s32 scale;

    lifetime = particle->lifetime;
    alpha = particle->alpha;
    if (lifetime != 0) {
        if (alpha != 0xFF) {
            alpha += D_800BE9E4 << 4;
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
            particle->alpha = alpha;
            lifetime = ((volatile StyleParticle *)particle)->lifetime; // matching: reloaded only after the alpha store
        }
    } else {
        if (alpha != 0) {
            alpha -= D_800BE9E4 << 3;
            if (alpha < 0) {
                alpha = 0;
            }
            particle->alpha = alpha;
        }
        scale = (alpha << 9) >> 8;
        particle->scaleY = scale;
        particle->scaleX = scale;
        lifetime = ((volatile StyleParticle *)particle)->lifetime; // matching: reloaded only after the scale stores
    }

    if (lifetime == 0 && alpha == 0) {
        return 1;
    }

    func_1516F864(particle);
    particle->velocityY = particle->velocityY + (s8)particle->u.drift.unk2C;
    return 0;
}

void func_1516FE1C(s32 object, s32 duration, u8 arg2, s32 arg3) {
    s32 bone;
    s16 posHigh;
    s16 posLow;
    s32 flags;
    s32 i;
    s32 velocityXHigh;
    s32 position;

    velocityXHigh = 0;
    if (movement_obj[object].type == 1) {
        /* follow the object's bone, D_800A6E30 row 4 */
        posHigh = 4;
        posLow = 0;
        bone = 0x19;
        flags = 1;
    } else {
        /* follow the object's position vector; bone is left unset (unused without an object) */
        position = (s32)&movement_obj[object].xpos;
        posHigh = position >> 16;
        posLow = position & 0xFFFF;
        object = 0xFF;
        flags = 0x21;
    }
    for (i = 0; i < 2; i++) {
        if (i == 1) {
            bone = 0x1B;
            velocityXHigh = 8;
        }
        func_1516D99C(posHigh, posLow, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, velocityXHigh, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0xAA, 0xAA, 0xAA, 0xAA, (u16)duration, 0x1E, 0, (u8)object, 0x1E, 0x168,
                      0x18, (u16)flags, 4, 0, 1, 0, 0, 0, bone, 0, arg2, arg3);
    }
}


/* A random unit direction vector; flags & 0x40 halves the range of its first angle. */
void func_15170034(s32 flags, f32 *outX, f32 *outY, f32 *outZ) {
    s32 mask;
    f32 yawA;
    f32 yawB;
    f32 horizontal;
    s32 angle;

    mask = (flags & 0x40) ? 0x3F : 0x7F;
    angle = func_150ADA20() & mask;
    *outY = func_150489B0((u8)angle);
    horizontal = func_15048A40((u8)angle);
    angle = func_150ADA20() & 0xFF;
    yawB = func_150489B0((u8)angle);
    yawA = func_15048A40((u8)angle);
    *outX = horizontal * yawA;
    *outZ = horizontal * yawB;
}

struct Vec3fTable { f32 v[3]; };
struct RGBTable { u8 c[3][3]; };

void func_151700D8(f32 (*verts)[3], f32 x, f32 z, u8 style, f32 scale, s32 type, s32 countA, s32 countB, u8 arg8, s32 arg9) {
    s32 i;
    s32 j;
    s32 next;
    s32 lifetime;
    s32 size;
    u8 *color;
    f32 t;
    f32 px;
    f32 py;
    f32 pz;
    s32 velocityX;
    s32 velocityZ;
    f32 *p1;
    s32 sizeMin;
    s32 sizeRandom;
    s32 velocityYRandom;
    s32 velocityYMin;
    s32 velocityY;
    f32 *p0;
    s32 particleStyle;
    s32 pad; // matching: unused stack slot in the original frame
    s32 count;
    struct Vec3fTable unused = { { 143.0f, 30.0f, 30.0f } }; /* copied but never read, as in the original */
    struct Vec3fTable velocityYRandoms = { { 571.0f, 130.0f, 130.0f } };
    struct RGBTable colors = { { { 0x86, 0x6A, 0x3C }, { 0xC8, 0xBE, 0xB4 }, { 0xFF, 0xD2, 0x80 } } };

    velocityYRandom = velocityYRandoms.v[type] * scale;
    sizeMin = 731.0f * scale;
    sizeRandom = 836.0f * scale;
    velocityYMin = 143.0f * scale;
    p0 = verts[0];
    for (i = 0; i < 4; i++) {
        if (i != 3) {
            next = i + 1;
        } else {
            next = 0;
        }
        if (i == 0 || i == 2) {
            count = countA;
        } else {
            count = countB;
        }
        p1 = verts[next];
        velocityX = (((p0[0] + p1[0]) * 0.5f) - x) * 2.0f;
        px = (p0[1] + p1[1]) * 0.5f; // matching: dead store, overwritten in the loop below
        velocityZ = (((p0[2] + p1[2]) * 0.5f) - z) * 2.0f;
        for (j = 0; j < count; j++) {
            t = j * (1.0f / (count - 1));
            color = colors.c[type];
            particleStyle = style;
            px = p0[0] + (p1[0] - p0[0]) * t;
            py = p0[1] + (p1[1] - p0[1]) * t;
            pz = p0[2] + (p1[2] - p0[2]) * t;
            if (i) { // matching: the empty branch on i steers IDO's register allocation
            }
            lifetime = (func_150ADA20() & 0x1F) + 0xF;
            size = (func_150ADA20() % (u32)sizeRandom) + sizeMin;
            velocityY = (func_150ADA20() % (u32)velocityYRandom) + velocityYMin;
            func_1516D4E8((s16)(s32)px, (s16)(s32)py, (s16)(s32)pz, 0xD, 0,
                color[0], color[1], color[2], 0, 0, 0, 0, 2,
                (u8)((velocityX >> 8) & 0xFF), (u8)(velocityX & 0xFF), (u8)((velocityZ >> 8) & 0xFF), (u8)(velocityZ & 0xFF),
                0, 0, particleStyle, 0, 0, 0,
                (s16)size, (s16)size, lifetime, velocityY,
                0, 0, arg8, arg9);
        }
        p0 += 3;
    }
}


/* Spawns a single particle (isParticle) or an emitter heading off along `angle`, which
 * defaults to object 0's facing angle. */
void func_15170500(x, y, z, angle, isParticle, arg5, arg6)
    s16 x;
    s16 y;
    s16 z;
    s32 angle;
    s32 isParticle;
    u8 arg5;
    s32 arg6;
{
    f32 dirX;
    f32 rise;
    f32 dirZ;
    f32 spread;
    s32 velocityX;
    s32 velocityZ;
    s32 velocityY;
    f32 dirY;
    s32 pad; // matching: unused stack slot in the original frame
    s32 tilt;

    rise = 0.0f;
    if (isParticle == 0) {
        angle = movement_obj[0].langle;
    }
    tilt = func_151EF610() % 50;
    dirX = func_150489B0((u8)angle);
    dirZ = -func_15048A40((u8)angle);
    spread = func_150489B0((u8)tilt);
    dirX *= spread;
    dirZ *= spread;
    dirY = rise * spread;
    dirX += dirX; dirZ += dirZ; dirY += dirY;
    if (isParticle != 0) {
        velocityX = dirX * 256.0f;
        velocityZ = dirZ * 256.0f;
        velocityY = dirY * 256.0f;
        func_1516D4E8(x, y, z, 0x2A, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 6,
            velocityX >> 8, velocityX & 0xFF, velocityZ >> 8, velocityZ & 0xFF, 0, 0, 0, 0, 0, 1, 0x200, 0x166, 0xFF,
            velocityY - 50, 0, 0, arg5, arg6);
    } else {
        velocityX = dirX * 256.0f;
        velocityZ = dirZ * 256.0f;
        velocityY = dirY * 256.0f;
        func_1516D99C(x, y, z, 0x2A, 0, 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 6,
            velocityX >> 8, velocityX & 0xFF, velocityZ >> 8, velocityZ & 0xFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x200, 0x166,
            0, 0, 0x14, 0, 0, 0xFF, 0xFF, velocityY - 50, 0xC8, 9, 1, 0, 1, 0x32, 0x14, 0, 0, 0,
            arg5, arg6);
    }
}

s32 func_151707E0(StyleParticle *particle) {
    s32 lifetime;
    s32 alpha;
    s32 velocityX;

    lifetime = particle->lifetime;
    alpha = particle->alpha;
    if (lifetime != 0) {
        if (alpha == 0) {
            particle->lifetime = (func_150ADA20() % 25U) + 200;
        }
        if (alpha != 0xFE) {
            alpha += D_800BE9E4 << 6;
            if (alpha >= 0xFF) {
                alpha = 0xFE;
            }
            particle->alpha = alpha;
        }
    } else {
        if (alpha != 0) {
            alpha -= D_800BE9E4 << 4;
            if (alpha < 0) {
                alpha = 0;
            }
            particle->alpha = alpha;
        }
    }

    if (particle->lifetime == 0 && alpha == 0) {
        return 1;
    }

    func_1516F864(particle);
    if (particle->u.drift.unk2F != 0) {
        func_1516F8EC(particle, (func_151EF610() % 32) + 230);
        func_1516F91C(particle, (func_151EF610() % 32) + 230);
        particle->velocityY = 0;
    } else {
        /* BUG (original game): the speed test squares the X velocity twice and ignores Z */
        velocityX = (particle->u.drift.velocityXHigh << 8) + particle->u.drift.velocityXLow;
        alpha = particle->u.drift.velocityXHigh * 0x100; // matching: the original reuses alpha's local here
        alpha += particle->u.drift.velocityXLow;
        if (velocityX * velocityX + alpha * alpha > 2000) {
            func_1516F8EC(particle, (func_151EF610() % 32) + 220);
            func_1516F91C(particle, (func_151EF610() % 32) + 220);
        }
        particle->velocityY = 0;
    }
    return 0;
}

/* Spawns `count` particles flying off at `speed` in random directions from func_15170034. */
void func_151709B4(s32 x, s32 y, s32 z, s32 count, f32 speed, u8 arg5, s32 arg6) {
    s32 i;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    s32 velocityX;
    s32 velocityZ;
    s32 velocityY;
    s32 pad[2]; // matching: unused stack slots in the original frame

    for (i = 0; i < count; i++) {
        func_15170034(0x40, &dirX, &dirY, &dirZ);
        dirX *= speed;
        dirY *= speed + speed;
        dirZ *= speed;
        velocityX = dirX;
        velocityZ = dirZ;
        velocityY = dirY;
        func_1516D4E8(x, y, z, 0x2F, 0, 0xFF, 0xFF, 0xFF, 0, 0xFF, 0, 0, 7,
            velocityX >> 8, velocityX & 0xFF, velocityZ >> 8, velocityZ & 0xFF, 0, 0, 0, 0, 0, 0, 0x200, 0x200, 0x28,
            velocityY, 0, 0, arg5, arg6);
    }
}
