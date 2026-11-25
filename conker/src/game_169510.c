#include <ultra64.h>

#include "functions.h"
#include "variables.h"
#include <stddef.h>

typedef struct {
    f32 x, y, z;
} Vec3f;

/* D_80090B60[]: one entry per texture. */
typedef struct {
    s32 unk0;
    u8 numFrames;
    u8 unk5;
    u16 width;
    u16 height;
    u16 unkA;
} TextureInfo;

/* Model instance attached to a sprite or particle, drawn through func_151462C8. */
typedef struct {
    s32 model;
    u8 unk4;
    u8 pad5[3];
    s32 handles[4];     /* one per viewport, 0..D_80082FA0 */
    s32 sharedHandle;
    s32 unk1C;
} ModelLink; /* size 0x20 */

/* Spawn template of a sprite, copied to SpriteFx.def. */
typedef struct {
    s32 flags;
    s16 lifetime;
    u8 texture;         /* D_80090B60 index, 0xFF = untextured */
    u8 unk7;
    s32 frame;          /* 16.16 animation frame */
    s32 frameSpeed;
    u8 unk10;           /* faded out with the lifetime (flag 0x8000) */
    u8 unk11;
    u8 rgba[4];
    u8 unk16;
    u8 unk17;
    u32 renderModes;    /* low byte: D_800A4AC8 index, high half: combiner index */
    s32 unk1C;
    u8 viewportMask;
    u8 pad21;
    s16 fadeStart;      /* lifetime below which unk10 / rgba[3] fade */
    s16 fadeRate;
    u8 pad26[2];
} SpriteDef; /* size 0x28 */

/* Camera-facing quad effect created by func_1513C350. */
typedef struct {
    u8 pad0[0x10];
    s32 active;         /* bit 0: alive */
    s32 unk14;
    SpriteDef def;
    Vtx vtx[4];
    u8 updateFn;        /* D_80089C70 index, 0 = none */
    u8 expireFn;        /* D_80089CA8 index, 0 = none */
    u8 pad82[2];
    ModelLink link;
    Vec3f pos;
} SpriteFx;

/* Spawn template of a particle, copied to Particle.def. */
typedef struct {
    u8 texture;         /* D_80090B60 index, 0xFF = untextured */
    u8 type;            /* selects the D_80089F64 handler */
    u16 renderModes;    /* low byte: D_800A4AC8 index, high byte: combiner index */
    s16 lifetime;
    u8 pad6[2];
    s32 frame;          /* 16.16 animation frame */
    s32 frameSpeed;
    u8 rgba[4];
    f32 width;
    f32 height;
    Vec3f pos;
    Vec3f rot;          /* beams (func_1513FAB4) use this as their end point */
    Vec3f scale;
    s32 flags;
    u8 unk44;           /* faded out with the lifetime (flag 0x8000000) */
    u8 unk45;
    u8 unk46;
    u8 unk47;
    s16 *unk48;
    u8 viewportMask;
    u8 pad4D[3];
    s32 unk50;
    s16 fadeStart;      /* lifetime below which unk44 / rgba[3] fade */
    s16 fadeRate;
} ParticleDef; /* size 0x58 */

/* Per-behaviour state that follows a particle. */
typedef union {
    struct {
        Vec3f u;
        Vec3f v;
    } axes;             /* func_1513D6FC, func_1513F4B0 */
    struct {
        f32 unk0;
        f32 unk4;
        Vec3f vel;
        Vec3f rotVel;
        Vec3f scaleVel;
        f32 gravity;
    } drift;            /* func_1513E6E8 */
    struct {
        u8 pad0[0x18];
        f32 rate;
    } grow;             /* func_1513F6E8 */
    struct {
        Vec3f vel;
        f32 gravity;
        f32 drag;
        Vec3f rotVel;
    } ballistic;        /* func_1513F728 */
} ParticleMotion;

typedef struct Particle {
    u8 pad0[4];
    struct Particle *prev;
    struct Particle *next;
    u8 padC[4];
    s32 active;         /* bit 0: alive */
    s32 unk14;
    ParticleDef def;
    u8 updateFn1;       /* D_80089CB8 index, 0 = none */
    u8 updateFn2;       /* D_80089D60 index, 0 = none */
    u8 expireFn;        /* D_80089E10 index, 0 = none */
    u8 vertexFn;        /* D_80089E1C index */
    u8 unk74;
    u8 pad75[3];
    f32 unk78;
    u8 pad7C[0x14];
    s32 unk90;
    u8 unk94;
    u8 unk95;
    u8 pad96[2];
    s32 unk98;
    ModelLink link;
    u8 padBC[4];
    Vtx vtxTemplate[4];
    Vtx *vtxBuffers[4]; /* per viewport, refreshed from vtxTemplate */
    ParticleMotion motion;
} Particle;

/* Source of the four vertex colours given to func_151400D0. */
typedef struct {
    s16 r, g, b, a;
    u16 flag;
} VtxColor;

extern TextureInfo D_80090B60[];
extern f32 D_800A5184;
extern void *func_15167A68(s32, s32, s32, s32, s32, s32);
void func_1513FFF4(Vtx *vtx, u8 texture, u8 flip);
void func_151400D0(Vtx *vtx, VtxColor *colors);

extern s32 (*D_80089C70[])(SpriteFx *);
extern s32 (*D_80089CA8[])(SpriteFx *);

/* Per-frame update of a sprite: lifetime, fading and texture animation. */
void func_1513C060(SpriteFx *sprite) {
    s32 speed;
    s8 expired;
    s32 numFrames;
    s32 lastFrame;
    s32 faded;

    if (!(sprite->active & 1)) {
        return;
    }

    expired = FALSE;
    if (sprite->def.flags & 1) {
        sprite->def.lifetime -= D_800BE9E4;
        if (sprite->def.lifetime < 0) {
            expired = TRUE;
        }
    }
    if (sprite->updateFn != 0) {
        if (D_80089C70[sprite->updateFn](sprite) == 0) {
            expired = TRUE;
        }
    }

    if ((sprite->def.flags & 0x8000) && !expired) {
        if (sprite->def.lifetime < sprite->def.fadeStart) {
            faded = sprite->def.lifetime * sprite->def.fadeRate;
            if (faded < sprite->def.unk10) {
                sprite->def.unk10 = faded;
            }
        }
    }
    if (sprite->def.flags & 0x10000) {
        if (!expired) {
            if (sprite->def.lifetime < sprite->def.fadeStart) {
                faded = sprite->def.lifetime * sprite->def.fadeRate;
                if (faded < sprite->def.unk10) {
                    sprite->def.rgba[3] = faded;
                }
            }
        }
    }

    if (!expired) {
        speed = sprite->def.frameSpeed;
        if (speed != 0) {
            numFrames = D_80090B60[sprite->def.texture].numFrames;
            lastFrame = (numFrames << 16) - 1;
            sprite->def.frame += speed * D_800BE9E4;
            if (lastFrame < sprite->def.frame) {
                if (sprite->def.flags & 2) {
                    expired = TRUE;
                } else if (sprite->def.flags & 0x40) {
                    sprite->def.frameSpeed = 0;
                    sprite->def.frame = lastFrame;
                } else if (sprite->def.flags & 4) {
                    sprite->def.frame = lastFrame - (sprite->def.frame % lastFrame);
                    sprite->def.frameSpeed = -speed;
                } else {
                    do {
                        sprite->def.frame -= lastFrame;
                    } while (lastFrame < sprite->def.frame);
                }
            } else if (sprite->def.frame < 0) {
                if (sprite->def.flags & 8) {
                    expired = TRUE;
                } else if (sprite->def.flags & 0x80) {
                    sprite->def.frameSpeed = 0;
                    sprite->def.frame = 0;
                } else if (sprite->def.flags & 4) {
                    sprite->def.frame = (-sprite->def.frame) % lastFrame;
                    sprite->def.frameSpeed = -speed;
                } else {
                    do {
                        sprite->def.frame += lastFrame;
                    } while (sprite->def.frame < 0);
                }
            }
        }
    }

    if (expired) {
        if (sprite->expireFn != 0) {
            if (D_80089CA8[sprite->expireFn](sprite) != 0) {
                func_1516972C(sprite);
            }
        } else {
            func_1516972C(sprite);
        }
    }
}

/* Allocates a sprite from a template; extraSize bytes follow it. */
struct210 *func_1513C350(struct210 *def, s32 colors, u8 updateFn, u8 expireFn, u8 flip, s32 model, s32 arg6,
                         struct167 *pos, s32 extraSize, u8 arg9, s32 argA) {
    SpriteFx *sprite;
    u8 kind;
    s32 i;

    if (colors == 0) {
        colors = (s32)&D_800A4AA0;
    }
    if (((SpriteDef *)def)->flags & 0x40000) {
        kind = 0x4A;
    } else {
        kind = 0x1B;
    }
    sprite = func_15167A68(kind, argA, extraSize + sizeof(SpriteFx), 1, arg9, 1);
    if (sprite == NULL) {
        return NULL;
    }
    memcpy(&sprite->def, def, sizeof(SpriteDef));
    func_1513FFF4(sprite->vtx, sprite->def.texture, flip);
    func_151400D0(sprite->vtx, colors);
    sprite->updateFn = updateFn;
    sprite->expireFn = expireFn;
    sprite->link.model = model;
    sprite->active = 1;
    sprite->unk14 = 0;
    sprite->link.unk1C = arg6;
    for (i = 0; i < 4; i++) {
        sprite->link.handles[i] = 0;
    }

    sprite->link.sharedHandle = 0;
    if (model) {
        for (i = 0; i <= D_80082FA0; i++) {
            sprite->link.handles[i] = func_1515D480(model);
        }
        sprite->link.sharedHandle = func_1515D440();
    }
    if (pos) {
        sprite->pos = *(Vec3f *)pos;
    }
    return (struct210 *)sprite;
}

/* Spawns a flat (xz-plane) sprite at (x, y, z). */
struct210 *func_1513C4EC(s32 def, s32 colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width, f32 length,
                         u8 angle, u8 flip, s32 model, s32 arg6, s32 extraSize, u8 argE, s32 argF) {
    struct210 *sprite;
    struct17 pos;

    pos.unk0 = x;
    pos.unk4 = y;
    pos.unk8 = z;
    sprite = func_1513C350(def, colors, updateFn, expireFn, flip, model, arg6, &pos, extraSize, argE, argF);
    if (sprite == NULL) {
        return NULL;
    }
    func_1513E13C(sprite, x, y, z, width, length, angle);
    return sprite;
}

/* Spawns a flat (xz-plane) sprite without a model or stored position. */
struct210 *func_1513C5B0(void *def, void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width,
                         f32 length, u8 angle, u8 flip, s32 extraSize, u8 argC, s32 argD) {
    struct210 *sprite = func_1513C350(def, colors, updateFn, expireFn, flip, 0, 0, NULL, extraSize, argC, argD);

    if (sprite == NULL) {
        return NULL;
    }
    func_1513E13C(sprite, x, y, z, width, length, angle);
    return sprite;
}

/* Spawns a sprite oriented by func_1513E2AC, with the default colours. */
struct210 *func_1513C650(s32 def, u8 updateFn, u8 expireFn, s32 arg3, f32 x, f32 y, f32 z, f32 width, f32 height,
                         u8 angle, u8 flip, s32 model, s32 arg6, s32 extraSize, u8 argE, s32 argF) {
    struct210 *sprite;
    struct17 pos;

    pos.unk0 = x;
    pos.unk4 = y;
    pos.unk8 = z;

    sprite = func_1513C350(def, 0, updateFn, expireFn, flip, model, arg6, &pos, extraSize, argE, argF);
    if (sprite == NULL) {
        return NULL;
    }

    if (func_1513E2AC(sprite, 0, arg3, x, y, z, width, height, angle) == 0) {
        func_1516972C(sprite);
        return NULL;
    }
    return sprite;
}

/* As func_1513C650, without a model or stored position. */
struct210 *func_1513C73C(s32 def, u8 updateFn, u8 expireFn, s32 arg3, f32 x, f32 y, f32 z, f32 width, f32 height,
                         u8 angle, u8 flip, s32 extraSize, u8 argC, s32 argD) {
    struct210 *sprite = func_1513C350(def, 0, updateFn, expireFn, flip, 0, 0, NULL, extraSize, argC, argD);

    if (sprite == NULL) {
        return NULL;
    }

    if (func_1513E2AC(sprite, 0, arg3, x, y, z, width, height, angle) == 0) {
        func_1516972C(sprite);
        return NULL;
    }
    return sprite;
}

/* As func_1513C73C; arg1 is unused and the default colours are used. */
struct210 *func_1513C804(s32 def, s32 arg1, u8 updateFn, u8 expireFn, s32 arg4, f32 x, f32 y, f32 z, f32 width,
                         f32 height, u8 angle, u8 flip, s32 extraSize, u8 argD, s32 argE) {
    struct210 *sprite = func_1513C350(def, 0, updateFn, expireFn, flip, 0, 0, NULL, extraSize, argD, argE);

    if (sprite == NULL) {
        return NULL;
    }
    if (func_1513E2AC(sprite, 0, arg4, x, y, z, width, height, angle) == 0) {
        func_1516972C(sprite);
        return NULL;
    }
    return sprite;
}

void func_1513C8D4(struct210 *sprite) {
    func_1513C92C(sprite);
    func_15169804(sprite);
}

void func_1513C900(struct210 *sprite) {
    func_1513C92C(sprite);
    func_15169824(sprite);
}

/* Releases a sprite's model handles. */
void func_1513C92C(struct210 *arg0) {
    s32 i;

    // matching: a typed local copy of arg0 gets its own saved register
    for (i = 0; i <= D_80082FA0; i++) {
        if (((SpriteFx *)arg0)->link.handles[i]) {
            func_100043B4(((SpriteFx *)arg0)->link.handles[i], 4);
        }
    }
    if (((SpriteFx *)arg0)->link.sharedHandle != 0) {
        func_100043B4(((SpriteFx *)arg0)->link.sharedHandle, 4);
    }
}

void func_1513C9B0(struct210 *arg0) {
    SpriteFx *sprite = (SpriteFx *)arg0;
    s32 index;

    if (sprite->def.flags & 0x1000) {
        index = sprite->def.unk7;
    } else {
        index = 0;
    }
    D_80089EE8[index]();
}

void func_1513C9FC(struct210 *arg0) {
    SpriteFx *sprite = (SpriteFx *)arg0;
    s32 index;

    if (sprite->def.flags & 0x1000) {
        index = sprite->def.unk7;
    } else {
        index = 0;
    }
    D_80089EF0[index]();
}

/* Frees a particle's per-viewport vertex buffers. */
void func_1513CA48(struct210 *arg0) {
    Particle *particle = (Particle *)arg0;

    func_151D5E30(particle->vtxBuffers);
}

void func_1513CA6C(struct210 *particle) {
    func_1513CA48(particle);
    func_1513CAD4(particle);
    func_15169804(particle);
}

void func_1513CAA0(struct210 *particle) {
    func_1513CA48(particle);
    func_1513CAD4(particle);
    func_15169824(particle);
}

/* Releases a particle's model handles. */
void func_1513CAD4(struct210 *arg0) {
    s32 i;

    // matching: a typed local copy of arg0 gets its own saved register
    for (i = 0; i <= D_80082FA0; i++) {
        if (((Particle *)arg0)->link.handles[i]) {
            func_100043B4(((Particle *)arg0)->link.handles[i], 4);
        }
    }
    if (((Particle *)arg0)->link.sharedHandle != 0) {
        func_100043B4(((Particle *)arg0)->link.sharedHandle, 4);
    }
}

void func_1513CB58(struct210 *arg0) {
    Particle *particle = (Particle *)arg0;
    s32 index;

    if (particle->def.flags & 0x400000) {
        index = particle->def.type;
    } else {
        index = 0;
    }
    D_80089EF8[index]();
}

void func_1513CBA4(struct210 *arg0) {
    Particle *particle = (Particle *)arg0;
    s32 index;

    if (particle->def.flags & 0x400000) {
        index = particle->def.type;
    } else {
        index = 0;
    }
    D_80089F2C[index]();
}

typedef struct {
    s32 unk0;
    s32 unk4;
} RenderModePair;
extern RenderModePair D_800A4AC8[];
extern u32 D_800D2C9C;
Gfx *func_15142B7C(Gfx *gfx, s32 setModes, s32 clearModes);
Gfx *func_15142C10(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142CF0(Gfx *gfx, s32 m, s32 l, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142E24(Gfx *gfx, TextureInfo *tex, s32 frame, s32 arg3, s32 arg4, s32 arg5, s32 texture, u8 arg7, s32 arg8,
                   u8 *needSync, s32 argA);
Gfx *func_15142FBC(Gfx *gfx, s32 modeHi, s32 modeLo, u8 *needSync);
void func_151441A4(s16 *r, s16 *g, s16 *b, s16 *a, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32 mode);
void func_151442FC(s16 *r, s16 *g, s16 *b, s16 *a, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32 mode);
Gfx *func_151462C8(Gfx *gfx, ModelLink *link, s32, s32, s32, s16 viewport, Vec3f *pos, s32, s32);
Gfx *func_1513F4E4(Gfx *gfx, u8 combiner, u8 *needSync);

/* Draws a sprite's quad (and attached model) for one viewport. */
Gfx *func_1513CBF0(Gfx *gfx, SpriteFx *sprite, s16 viewport) {
    s32 geometry;
    u8 needSync;
    u8 blendMode;
    u8 combiner;
    s16 cullFlags;
    s32 pad; // matching: unused stack slot
    s16 primR;
    s16 primG;
    s16 primB;
    s16 primA;
    s16 envR;
    s16 envG;
    s16 envB;
    s16 envA;

    if (sprite->active & 1) {
        if ((sprite->def.flags & 0x2000) && !(sprite->def.viewportMask & (1 << viewport))) {
            return gfx;
        }
        needSync = TRUE;
        blendMode = sprite->def.renderModes & 0xFF;
        combiner = (sprite->def.renderModes & 0xFFFF0000) >> 16;
        cullFlags = sprite->def.flags & 0x30;
        gfx = func_151462C8(gfx, &sprite->link, 0, 0, 0, viewport, &sprite->pos, 2,
                            (sprite->def.flags & 0x4000) ? sprite->def.unk1C : 0);
        func_151441A4(&primR, &primG, &primB, &primA, sprite->def.rgba[0], sprite->def.rgba[1], sprite->def.rgba[2],
                      sprite->def.rgba[3], 0xFF, 0xFF, 0xFF, sprite->def.unk10, sprite->def.unk11,
                      (sprite->def.flags & 0x100) ? sprite->def.unk16 : 3);
        func_151442FC(&envR, &envG, &envB, &envA, sprite->def.rgba[0], sprite->def.rgba[1], sprite->def.rgba[2],
                      sprite->def.rgba[3], 0xFF, 0xFF, 0xFF, sprite->def.unk10, sprite->def.unk11,
                      (sprite->def.flags & 0x200) ? sprite->def.unk17 : 4);
        geometry = 0;
        if (cullFlags & 0x10) {
            geometry = G_CULL_FRONT;
        }
        if (cullFlags & 0x20) {
            geometry |= G_CULL_BACK;
        }
        gfx = func_15142B7C(gfx, ((sprite->def.flags & 0x400) ? G_LIGHTING : 0) | geometry | G_ZBUFFER, G_CULL_BOTH);
        if (sprite->def.texture != 0xFF) {
            gfx = func_15142E24(gfx, &D_80090B60[sprite->def.texture], sprite->def.frame, 2, 0, 0, sprite->def.texture,
                                0, 0, &needSync, (sprite->def.flags & 0x20000) ? 0x3E : 3);
        }
        gfx = func_15142C10(gfx, envR, envG, envB, envA, &needSync);
        gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &needSync);
        gfx = func_1513F4E4(gfx, combiner, &needSync);
        gfx = func_15142FBC(gfx,
                            G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE |
                                ((sprite->def.texture == 0xFF) ? 0 : D_800D2C9C) | G_TF_BILERP | G_TC_FILT |
                                G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[blendMode].unk0 | D_800A4AC8[blendMode].unk4, &needSync);

        gSPVertex(gfx++, sprite->vtx, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
    }
    return gfx;
}

void func_1513CF9C(s32 arg0, s32 arg1, u8 arg2) {
    D_80089F60[0](arg0, arg1, arg2);
}

void func_1513CFD0(s32 arg0, s32 arg1, s32 arg2) {
}

extern s32 (*D_80089CB8[])(Particle *);
extern s32 (*D_80089D60[])(Particle *);
extern s32 (*D_80089E10[])(Particle *);

/* Per-frame update of a particle: lifetime, fading and texture animation. */
void func_1513CFE4(Particle *particle) {
    s32 speed;
    s8 expired;
    s32 numFrames;
    s32 lastFrame;
    s32 faded;

    if (!(particle->active & 1)) {
        return;
    }

    expired = FALSE;
    if (particle->def.flags & 1) {
        particle->def.lifetime -= D_800BE9E4;
        if (particle->def.lifetime < 0) {
            expired = TRUE;
        }
    }
    if (particle->updateFn1 != 0) {
        if (D_80089CB8[particle->updateFn1](particle) == 0) {
            expired = TRUE;
        }
    }
    if (particle->updateFn2 != 0) {
        if (D_80089D60[particle->updateFn2](particle) == 0) {
            expired = TRUE;
        }
    }

    if ((particle->def.flags & 0x8000000) && !expired) {
        if (particle->def.lifetime < particle->def.fadeStart) {
            faded = particle->def.lifetime * particle->def.fadeRate;
            if (faded < particle->def.unk44) {
                particle->def.unk44 = faded;
            }
        }
    }
    if (particle->def.flags & 0x10000000) {
        if (!expired) {
            if (particle->def.lifetime < particle->def.fadeStart) {
                faded = particle->def.lifetime * particle->def.fadeRate;
                if (faded < particle->def.unk44) {
                    particle->def.rgba[3] = faded;
                }
            }
        }
    }

    if (!expired) {
        speed = particle->def.frameSpeed;
        if (speed != 0) {
            numFrames = D_80090B60[particle->def.texture].numFrames;
            lastFrame = (numFrames << 16) - 1;
            particle->def.frame += speed * D_800BE9E4;
            if (lastFrame < particle->def.frame) {
                if (particle->def.flags & 0x800) {
                    expired = TRUE;
                } else if (particle->def.flags & 0x400) {
                    particle->def.frame = lastFrame - (particle->def.frame % lastFrame);
                    particle->def.frameSpeed = -speed;
                } else {
                    do {
                        particle->def.frame -= lastFrame;
                    } while (lastFrame < particle->def.frame);
                }
            } else if (particle->def.frame < 0) {
                if (particle->def.flags & 0x1000) {
                    expired = TRUE;
                } else if (particle->def.flags & 0x400) {
                    particle->def.frame = (-particle->def.frame) % lastFrame;
                    particle->def.frameSpeed = -speed;
                } else {
                    do {
                        particle->def.frame += lastFrame;
                    } while (particle->def.frame < 0);
                }
            }
        }
    }

    if (expired) {
        if (particle->expireFn != 0) {
            if (D_80089E10[particle->expireFn](particle) != 0) {
                expired = FALSE;
            }
        }
    }
    if (expired) {
        func_1516972C(particle);
    }
}

/* Allocates a particle from a template; motionSize extra bytes follow it for its behaviour state. */
void *func_1513D2F0(void *def, s32 colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 model,
                    s32 arg8, s32 motionSize, u8 argA, s32 argB) {
    Particle *particle;
    s32 flags;
    s32 kind;
    s32 i;

    flags = ((ParticleDef *)def)->flags;
    if (flags & 0x800000) {
        kind = 0x56;
    } else if (flags & 0x2000000) {
        kind = 0x49;
    } else {
        kind = 0x1C;
    }
    particle = func_15167A68(kind, argB, motionSize + offsetof(Particle, motion), 1, argA,
                             (flags & 0x80000000U) ? 2 : 1);
    if (particle == NULL) {
        return NULL;
    }
    memcpy(&particle->def, def, sizeof(ParticleDef));
    particle->updateFn1 = updateFn1;
    particle->updateFn2 = updateFn2;
    particle->expireFn = expireFn;
    particle->vertexFn = vertexFn;
    particle->unk74 = 0;
    bzero(particle->vtxBuffers, sizeof(particle->vtxBuffers));
    func_1513FFF4(particle->vtxTemplate, particle->def.texture, flip);
    func_151400D0(particle->vtxTemplate, colors);
    particle->active = 1;
    particle->unk14 = 0;
    particle->unk78 = D_800A5184;
    particle->unk98 = 0;
    particle->unk95 = 0;
    particle->unk94 = 0;
    particle->unk90 = 0;
    particle->link.model = model;
    particle->link.unk1C = arg8;
    particle->link.unk4 = 0;
    for (i = 0; i < 4; i++) {
        particle->link.handles[i] = 0;
    }

    particle->link.sharedHandle = 0;
    if (model) {
        for (i = 0; i <= D_80082FA0; i++) {
            particle->link.handles[i] = func_1515D480(model);
        }
        particle->link.sharedHandle = func_1515D440();
    }
    return particle;
}

void func_1513D4B8(s32 def, s32 colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 motionSize,
                   u8 arg8, s32 arg9) {
    func_1513D2F0(def, colors, updateFn1, updateFn2, expireFn, vertexFn, flip, 0, 0, motionSize, arg8, arg9);
}

/* As func_1513D4B8 with the default colours. */
void func_1513D524(s32 def, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 motionSize, u8 arg7,
                   s32 arg8) {
    func_1513D2F0(def, &D_800A4AA0, updateFn1, updateFn2, expireFn, vertexFn, flip, 0, 0, motionSize, arg7, arg8);
}

/* Spawns a particle whose quad is spanned by two axes from func_1513D6FC (vertex function 12). */
s32 func_1513D594(s32 def, s32 colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width,
                  f32 height, s32 arg9, s32 argA, s32 model, s32 argC, u8 argD, s32 motionSize, u8 argF, s32 arg10) {
    Particle *particle;

    if (colors == 0) {
        colors = (s32)&D_800A4AA0;
    }
    particle = func_1513D2F0(def, colors, updateFn1, updateFn2, expireFn, 12, flip, model, argC,
                             motionSize + sizeof(particle->motion.axes), argF, arg10);
    if ((particle != NULL) &&
        (func_1513D6FC((struct211 *)&particle->motion.axes, angle, width, height, arg9, argA, argD) == 0)) {
        func_1516972C(particle);
        return 0;
    }
    return (s32)particle;
}

void func_1513D668(s32 def, s32 colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width,
                   f32 height, s32 arg9, s32 argA, u8 argB, s32 motionSize, u8 argD, s32 argE) {
    func_1513D594(def, colors, updateFn1, updateFn2, expireFn, flip, angle, width, height, arg9, argA, 0, 0, argB,
                  motionSize, argD, argE);
}

s32 func_1510E388(s32, s32, f32 *, f32 *);
s32 func_15144E80(s32, Vec3f *, Vec3f *, Vec3f *);
s32 func_15145128(Vec3f *, Vec3f *, f32 *, f32 *);

/*
 * Computes the two half-axes of a particle quad rotated by angle. With onSlope the quad lies on the
 * slope func_1510E388 returns (each axis keeps its length); otherwise it lies in the plane of the
 * basis func_15144E80 returns.
 */
s32 func_1513D6FC(struct211 *axes, s16 angle, f32 width, f32 height, s32 arg4, s32 arg5, u8 onSlope) {
    f32 slopeX;
    f32 slopeZ;
    f32 cosA;
    f32 sinA;
    f32 widthSin;
    f32 t;
    f32 heightSin;
    f32 heightCos;
    f32 unused1;
    f32 p2;
    f32 q2;
    f32 r2;
    Vec3f v;
    f32 k2;
    f32 cosB;
    f32 sinB;
    Vec3f basisU;
    Vec3f basisV;
    Vec3f basisN;
    f32 len;
    f32 unused2;
    f32 w;

    if (onSlope != 0) {
        cosA = func_151423D8(angle - 0x40);
        sinA = func_151423D8(angle);
        widthSin = width * sinA;
        heightSin = height * sinA;
        if (func_1510E388(arg4, arg5, &slopeX, &slopeZ) == 0) {
            return 0;
        }
        {
            f32 p;
            f32 q;
            f32 r;

            heightCos = height * cosA;
            t = heightSin * slopeZ;
            // matching: p, q (and p2, q2, r2 below) are dead, but the original computed them
            p = fabsf(heightSin);
            q = sqrtf((heightSin * heightSin) + (t * t));
            if (0 != heightCos) {
                w = heightCos * slopeX;
                v.z = w;
                v.y = fabsf(heightCos) / sqrtf((heightCos * heightCos) + (w * w));
            } else {
                v.z = 0.0f;
                v.y = 0.0f;
            }
            if (heightSin != 0) {
                v.x = t;
                k2 = fabsf(heightSin) / sqrtf((heightSin * heightSin) + (t * t));
            } else {
                v.x = 0.0f;
                k2 = 0.0f;
            }
            axes->unk0 = v.y * heightCos;
            axes->unk4 = (v.y * v.z) + (k2 * v.x) + 0.5f;
            axes->unk8 = k2 * heightSin;
            t = -(width * cosA);
            p2 = t * slopeZ;
            q2 = fabsf(t);
            r2 = sqrtf((t * t) + (p2 * p2));
            if (widthSin != 0) {
                w = widthSin * slopeX; v.z = w; v.y = fabsf(widthSin) / sqrtf((widthSin * widthSin) + (w * w));
            } else {
                v.z = 0.0f; v.y = 0.0f;
            }
            if (t != 0) {
                v.x = t * slopeZ;
                k2 = fabsf(t) / sqrtf((t * t) + ((t * slopeZ) * (t * slopeZ)));
            } else {
                v.x = 0.0f;
                k2 = 0.0f;
            }
            axes->unkC = v.y * widthSin;
            axes->unk10 = (v.y * v.z) + (k2 * v.x) + 0.5f;
            axes->unk14 = k2 * t;
        }
    } else {
        cosB = func_151423D8(angle - 0x40);
        sinB = func_151423D8(angle);
        if (func_15144E80(arg5, &basisU, &basisV, &basisN) == 0) {
            return 0;
        }
        if (func_15145128(&basisU, &basisU, &len, &unused2) == 0) {
            return 0;
        }
        if (func_15145128(&basisV, &basisV, &len, &unused2) == 0) {
            return 0;
        }
        {
            extern s32 D_1513D6FC_dummy; // matching: a declaration here opens the -g3 scope the scheduler keys on

            sinA = width * sinB;
            t = width * cosB;
            axes->unk0 = (sinA * basisU.x) + (t * basisV.x);
            axes->unk4 = (sinA * basisU.y) + (t * basisV.y);
            axes->unk8 = (sinA * basisU.z) + (t * basisV.z);
            sinA = -height * cosB;
            t = height * sinB;
            axes->unkC = (sinA * basisU.x) + (t * basisV.x);
            axes->unk10 = (sinA * basisU.y) + (t * basisV.y);
            axes->unk14 = (sinA * basisU.z) + (t * basisV.z);
        }
        return 1;
    }
end: // matching: an unused label keeps the final return 1 in its own basic block
    return 1;
}

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x4];
    u8 unk18;
    u8 pad19;
    u16 unk1A;
    u8 pad1C[0x4];
    s32 unk20;
    u8 pad24[0x4];
    u8 unk28;
    u8 unk29;
    u8 unk2A;
    u8 unk2B;
    u8 pad2C[0x8];
    u8 unk34[0x24];
    s32 unk58;
    u8 unk5C;
    u8 unk5D;
    u8 unk5E;
    u8 unk5F;
    s16 *unk60;
    u8 unk64;
    u8 pad65[0x3];
    s32 unk68;
    u8 pad6C[0x7];
    u8 unk73;
    u8 pad74[0x28];
    u8 unk9C[0x64];
    s32 unk100[4];
} Obj1513DB00;


extern s32 (*D_80089E1C[])(Obj1513DB00 *, s16);

Gfx *func_1513DB00(Gfx *arg0, Obj1513DB00 *arg1, s16 arg2) {
    s32 pad8C;
    u8 flag;
    s16 r1;
    s16 g1;
    s16 b1;
    s16 a1;
    s16 r2;
    s16 g2;
    s16 b2;
    s16 a2;
    u8 mode;
    u8 hi;
    s16 bits;
    s32 cyc;
    s32 vtx;

    if (arg1->unk10 & 1) {
        flag = 1;
        if ((arg1->unk58 & 8) && !(arg1->unk64 & (1 << arg2))) {
            if (arg1->unk100[arg2]) {
                func_100043B4((s32 *)arg1->unk100[arg2], 2);
                arg1->unk100[arg2] = 0;
            }
            return arg0;
        }
        mode = arg1->unk1A & 0xFF;
        hi = (arg1->unk1A & 0xFF00) >> 8;
        bits = arg1->unk58 & 0xC000;
        if ((arg1->unk58 & 0x01000000) && arg1->unk60 != NULL && !(arg1->unk60[1] & (1 << arg2))) {
            if (arg1->unk100[arg2]) {
                func_100043B4((s32 *)arg1->unk100[arg2], 2);
                arg1->unk100[arg2] = 0;
            }
            return arg0;
        }
        vtx = D_80089E1C[arg1->unk73](arg1, arg2);
        if (vtx == 0) {
            if (arg1->unk100[arg2]) {
                func_100043B4((s32 *)arg1->unk100[arg2], 2);
                arg1->unk100[arg2] = 0;
            }
            return arg0;
        }
        arg0 = func_151462C8(arg0, arg1->unk9C, 0, 0, 0, arg2, arg1->unk34, 2, (arg1->unk58 & 0x04000000) ? arg1->unk68 : 0);
        func_151441A4(&r1, &g1, &b1, &a1, arg1->unk28, arg1->unk29, arg1->unk2A, arg1->unk2B, 0xFF, 0xFF, 0xFF, arg1->unk5C, arg1->unk5D, (arg1->unk58 & 0x40000) ? arg1->unk5E : 4);
        func_151442FC(&r2, &g2, &b2, &a2, arg1->unk28, arg1->unk29, arg1->unk2A, arg1->unk2B, 0xFF, 0xFF, 0xFF, arg1->unk5C, arg1->unk5D, (arg1->unk58 & 0x80000) ? arg1->unk5F : 5);
        cyc = 0;
        if (bits & 0x4000) {
            cyc = 0x200;
        }
        if (bits & 0x8000) {
            cyc |= 0x400;
        }
        if (arg1->unk58 & 0x100000) {
            cyc |= 0x20000;
        }
        arg0 = func_15142B7C(arg0, cyc | 1, 0x600);
        if (arg1->unk18 != 0xFF) {
            s32 wrap;
            wrap = (arg1->unk58 & 0x200000) ? 0 : 2;
            arg0 = func_15142E24(arg0, (s32)&D_80090B60[arg1->unk18], arg1->unk20, wrap, 0, 0, arg1->unk18, 0, 0, &flag, (arg1->unk58 & 0x40000000) ? 0x3E : 3);
        }
        arg0 = func_15142C10(arg0, r2, g2, b2, a2, &flag);
        arg0 = func_15142CF0(arg0, 0xF2, 0, r1, g1, b1, a1, &flag);
        arg0 = func_1513F4E4(arg0, hi, &flag);
        arg0 = func_15142FBC(arg0, G_PM_NPRIMITIVE | ((arg1->unk58 & 0x20000000) ? G_CYC_2CYCLE : G_CYC_1CYCLE) | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | ((arg1->unk18 == 0xFF) ? 0 : D_800D2C9C) | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, D_800A4AC8[mode].unk0 | D_800A4AC8[mode].unk4, &flag);
        gSPVertex(arg0++, vtx, 4, 0);
        gSP2Triangles(arg0++, 0, 1, 2, 0, 0, 2, 3, 0);
    }
    return arg0;
}

// jump table
extern void (*D_80089F64[])(void *, s32, u8);

void func_1513DF9C(void *arg0, s32 arg1, u8 arg2) {
    s32 index;

    if (!(*(s32 *)((u8 *)arg0 + 0x58) & 0x400000)) {
        index = 0;
        goto dispatch;
    }
    switch (*(u8 *)((u8 *)arg0 + 0x19)) {
    case 12:
        index = 10;
        break;
    case 11:
        index = 9;
        break;
    case 0:
    case 2:
        index = 0;
        break;
    case 1:
        index = 1;
        break;
    case 5:
        index = 2;
        break;
    case 6:
        index = 3;
        break;
    case 3:
        index = 4;
        break;
    case 7:
        index = 5;
        break;
    case 8:
        index = 6;
        break;
    case 9:
        index = 7;
        break;
    case 4:
        index = 8;
        break;
    default:
        goto unknown_type;
    }
    goto dispatch;
unknown_type:
    index = 0;
dispatch:
    D_80089F64[index](arg0, arg1, arg2);
}

void func_1513E070(s32 arg0, s32 arg1, s32 arg2) {
}

void func_1513E084(struct210 *arg0, struct212 *arg1, u8 arg2) {
    struct212 *temp_v0 = &arg0->unk110;
    if (arg2 == 0x1A) {
        if ((temp_v0->unk0 == arg1->unk0) || (temp_v0->unk4.b.unk0 == arg1->unk4.b.unk0)) {
            func_1516972C(arg0);
        }
    } else {
        temp_v0 = &arg0->unk110;
        if (arg2 == 0x2D) {
            if (temp_v0->unk0 == arg1->unk0) {
                temp_v0->unk0 = arg1->unk4.i.unk0;
                temp_v0->unk4.b.unk0 = arg1->unk9;
            } else if (temp_v0->unk0 == arg1->unk4.i.unk0) {
                temp_v0->unk0 = arg1->unk0;
                temp_v0->unk4.b.unk0 = arg1->unk8;
            }
        }
    }
}

void func_1513E134(void) {
}

void func_1513E13C(struct210 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, u8 arg6)
{
  f32 s;
  f32 c[1];
  f32 xs;
  f32 xc;
  f32 zs;
  f32 zc;
  f32 x0;
  f32 t[1];
  f32 p0;
  f32 p1;
  f32 p2;
  f32 p3;
  f32 u[1];
  c[0] = func_151423D8(arg6 - 0x40);
  s = func_151423D8(arg6);
  xs = (&arg4)[0] * s;
  x0 = (&arg1)[0] + xs;
  xc = c[0] * (&arg5)[0];
  if (((!t) && (!t)) && (!t))
  {
  }
  arg0->unk40 = x0 - xc;
  arg0->unk42 = arg2;
  zs = (&arg5)[0] * s;
  u[0] = arg3 + zs;
  zc = c[0] * (&arg4)[0];
  arg0->unk44 = u[0] + zc;
  t[0] = arg1 - xs;
  arg0->unk50 = t[0] - xc;
  arg0->unk52 = arg2;
  arg0->unk54 = u[0] - zc;
  arg0->unk60 = t[0] + xc;
  arg0->unk62 = arg2;
  t[0] = arg3 - zs;
  arg0->unk64 = t[0] - zc;
  arg0->unk70 = x0 + xc;
  arg0->unk72 = arg2;
  arg0->unk74 = t[0] + zc;
}
// PERMUTER CANDIDATE best 3348 — reconstruction below is semantically close (first 12 instrs match)
// but FP store scheduling + a +4 stack-slot shift on sp48/sp1C/sp30 differ.
// void func_1513E13C(struct210 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, u8 arg6) {
//     f32 sp48; f32 sp30; f32 sp1C;
//     f32 temp_f0, temp_f12, temp_f14, temp_f16, temp_f18, temp_f2; s32 temp_f10;
//     sp48 = func_151423D8((arg6 - 0x40));
//     temp_f0 = func_151423D8(arg6);
//     temp_f12 = arg5 * sp48;
//     temp_f14 = arg4 * temp_f0;
//     temp_f18 = arg1 + temp_f14;
//     arg0->unk40 = temp_f18 - temp_f12;
//     temp_f10 = arg2;
//     arg0->unk42 = temp_f10;
//     temp_f16 = arg5 * temp_f0;
//     temp_f2 = arg4 * sp48;
//     sp1C = arg3 + temp_f16;
//     sp30 = arg1 - temp_f14;
//     arg0->unk44 = sp1C + temp_f2;
//     arg0->unk52 = temp_f10;
//     arg0->unk50 = sp30 - temp_f12;
//     arg0->unk54 = sp1C - temp_f2;
//     arg0->unk62 = temp_f10;
//     arg0->unk60 = sp30 + temp_f12;
//     arg0->unk72 = temp_f10;
//     arg0->unk70 = temp_f18 + temp_f12;
//     sp30 = arg3 - temp_f16;
//     arg0->unk64 = sp30 - temp_f2;
//     arg0->unk74 = sp30 + temp_f2;
// }

void func_1513E2A4(void) {
}

typedef struct {
    u8 pad0[0x40];
    Vtx_t v[4];
} Quad1513E2AC;

s32 func_1513E2AC(s32 arg0, s32 arg1, s32 arg2, f32 x, f32 y, f32 z, f32 w, f32 h, u8 angle) {
    Vtx_t *vtx;
    f32 s;
    f32 c;
    f32 ux;
    f32 uy;
    f32 uz;
    f32 vx;
    f32 vy;
    f32 vz;
    Vec3f a;
    Vec3f b;
    f32 wc;
    f32 ws;
    f32 hs;
    f32 hc;
    f32 outA;
    f32 outB;
    s16 i;
    f32 pts[4][2];
    f32 bx;
    f32 by;
    f32 ky;
    f32 kx;
    f32 x0;
    Vtx_t *v;

    vtx = ((Quad1513E2AC *)arg0)->v;
    s = func_151423D8(angle - 0x40);
    c = func_151423D8(angle);
    if (arg2 != 0) {
        v = vtx;
        if (func_15144E80(arg2, &a, &b, 0) == 0) {
            return 0;
        }
        func_15145128(&a, &a, 0, 0);
        func_15145128(&b, &b, 0, 0);
        {
            f32 pad;

            wc = w * c;
            ws = w * s;
            ux = wc * a.x + ws * b.x;
            uy = wc * a.y + ws * b.y;
            uz = wc * a.z + ws * b.z;
            hs = -h * s;
            hc = h * c;
            vx = hs * a.x + hc * b.x;
            vy = hs * a.y + hc * b.y;
            vz = hs * a.z + hc * b.z;
        }
        v[0].ob[0] = x - ux + vx;
        v[0].ob[1] = y - uy + vy;
        v[0].ob[2] = z - uz + vz;
        v[1].ob[0] = x + ux + vx;
        v[1].ob[1] = y + uy + vy;
        v[1].ob[2] = z + uz + vz;
        v[2].ob[0] = x + ux - vx;
        v[2].ob[1] = y + uy - vy;
        v[2].ob[2] = z + uz - vz;
        v[3].ob[0] = x - ux - vx;
        v[3].ob[1] = y - uy - vy;
        v[3].ob[2] = z - uz - vz;
        return 1;
    }
    if (func_1510E388(arg1, arg2, &outA, &outB) == 0) {
        return 0;
    }
    hc = h * c;
    wc = w * c;
    hs = h * s;
    ws = w * s;
    pts[0][0] = wc - hs;
    pts[0][1] = hc + ws;
    pts[1][0] = -wc - hs;
    pts[1][1] = hc - ws;
    pts[2][0] = -wc + hs;
    pts[2][1] = -hc - ws;
    pts[3][0] = wc + hs;
    pts[3][1] = -hc + ws;
    for (i = 0; i < 4; i++) {
        f32 pad2;

        by = pts[i][1] * outB;
        ky = fabsf(pts[i][1]);
        ky = fabsf(pts[i][1] * pts[i][1]);
        ky = fabsf(by); ky = fabsf(by - 1.0f);
        if (pts[i][0] != 0) {
            bx = pts[i][0] * outA;
            kx = fabsf(pts[i][0]) / sqrtf(pts[i][0] * pts[i][0] + bx * bx);
        } else {
            bx = 0.0f;
            kx = 0.0f;
        }
        if (pts[i][1] != 0) {
            ky = fabsf(pts[i][1]) / sqrtf(pts[i][1] * pts[i][1] + by * by);
        } else {
            by = 0.0f;
            ky = 0.0f;
        }
        vtx->ob[0] = pts[i][0] * kx + x;
        vtx->ob[1] = kx * bx + y + ky * by + 0.5f;
        vtx->ob[2] = pts[i][1] * ky + z;
        vtx++;
    }
    return 1;
}

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
} struct_func_1513E6E8_110;

typedef struct {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C;
    u8 unk5D;
    u8 pad5E[0xB2];
    struct_func_1513E6E8_110 unk110;
} struct_func_1513E6E8;

s32 func_1513E6E8(struct_func_1513E6E8 *arg0) {
    struct_func_1513E6E8_110 *vel;

    vel = &arg0->unk110;
    if (arg0->unk58 & 0x20) {
        vel->unkC = vel->unkC + (vel->unk2C * D_800BE9A4);
    }
    if (arg0->unk58 & 0x40) {
        arg0->unk34 = arg0->unk34 + (vel->unk8 * D_800BE9A4);
        arg0->unk38 = arg0->unk38 + (vel->unkC * D_800BE9A4);
        arg0->unk3C = arg0->unk3C + (vel->unk10 * D_800BE9A4);
    }
    if (arg0->unk58 & 0x80) {
        arg0->unk40 = arg0->unk40 + (vel->unk14 * D_800BE9A4);
        arg0->unk44 = arg0->unk44 + (vel->unk18 * D_800BE9A4);
        arg0->unk48 = arg0->unk48 + (vel->unk1C * D_800BE9A4);
    }
    if (arg0->unk58 & 0x100) {
        arg0->unk4C = arg0->unk4C + (vel->unk20 * D_800BE9A4);
        arg0->unk50 = arg0->unk50 + (vel->unk24 * D_800BE9A4);
        arg0->unk54 = arg0->unk54 + (vel->unk28 * D_800BE9A4);
    }
    arg0->unk5D = 0xFF;
    return 1;
}


// ???
typedef struct Game169510ScaledOwner {
    u8 unknown0[0x2C];
    f32 width, height;
    f32 x, y, z;
    f32 rx, ry, rz;
    f32 sx, sy, sz;
    u8 unknown58[0x68];
    u8 templateData[0x40];
    u8 *buffers[1];
} Game169510ScaledOwner;

typedef struct Game169510ScaledVertex {
    s16 x, y, z, flag;
    u8 other[8];
} Game169510ScaledVertex;

void func_151D5D60(void *, s16, s32, void **, u8 *);
void func_150A7960(f32 m[4][4], f32 x, f32 y, f32 z, f32 *ox, f32 *oy, f32 *oz);

void *func_1513E83C(void *arg0, s16 arg1) {
    f32 mtx[4][4];
    Game169510ScaledOwner *owner = arg0;
    Game169510ScaledVertex *vertices;
    Game169510ScaledVertex *result;
    f32 coords[4][3];
    u8 fresh;
    s16 i;

    func_151D5D60((u8 *)arg0 + 0x100, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != 0) {
        if (fresh != 0) {
            memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100), (u8 *)arg0 + 0xC0, 0x40);
            memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100) + 0x40, (u8 *)arg0 + 0xC0, 0x40);
        }
    } else {
        return 0;
    }
    {
        s16 pad; /* -g3 scope: its .bgnb splits the &mtx init from the first load */

        coords[0][0] = owner->width;
        coords[0][1] = owner->height;
        coords[1][0] = -owner->width;
        coords[1][1] = owner->height;
        coords[2][0] = -owner->width;
        coords[2][1] = -owner->height;
        coords[3][0] = owner->width;
        coords[3][1] = -owner->height;
        func_150A8050(mtx, owner->rx, owner->ry, owner->rz);
        mtx[3][0] = owner->x;
        mtx[3][1] = owner->y;
        mtx[3][2] = owner->z;
        mtx[0][0] *= owner->sx;
        mtx[0][1] *= owner->sx;
        mtx[0][2] *= owner->sx;
        mtx[1][0] *= owner->sy;
        mtx[1][1] *= owner->sy;
        mtx[1][2] *= owner->sy;
        mtx[2][0] *= owner->sz;
        mtx[2][1] *= owner->sz;
        mtx[2][2] *= owner->sz;
        for (i = 0; i < 4; i++) {
            f32 *c = coords[i];
            func_150A7960(mtx, c[0], c[1], 0.0f, &c[0], &c[1], &c[2]);
            vertices->x = c[0];
            vertices->y = c[1];
            vertices->z = c[2];
            vertices->flag = 0;
            vertices++;
        }
    }
    return result;
}


void *func_1513EAD8(void *arg0, s32 arg1, s16 arg2) {
    Game169510ScaledVertex *vertices;
    Game169510ScaledVertex *result;
    s16 coords[4][3];
    f32 heightDelta, heightScale, directionZ, directionX;
    u8 fresh;

    func_151D5D60((u8 *)arg0 + 0x100, arg2, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != 0) {
        if (fresh != 0) {
            memcpy(*(u8 **)((u8 *)arg0 + arg2 * 4 + 0x100), (u8 *)arg0 + 0xC0, 0x40);
            memcpy(*(u8 **)((u8 *)arg0 + arg2 * 4 + 0x100) + 0x40, (u8 *)arg0 + 0xC0, 0x40);
        }
    } else {
        return 0;
    }
    directionZ = D_800DD1D8[arg2] * ((Game169510ScaledOwner *)arg0)->width;
    directionX = D_800DD1E8[arg2] * ((Game169510ScaledOwner *)arg0)->width;
    heightScale = ((Game169510ScaledOwner *)arg0)->sy * ((Game169510ScaledOwner *)arg0)->height;
    heightDelta = ((Game169510ScaledOwner *)arg0)->height - heightScale;
    vertices[0].flag = 0;
    vertices[1].flag = 0;
    vertices[2].flag = 0;
    vertices[3].flag = 0;
    coords[3][0] = coords[0][0] = (s16)(s32)(((Game169510ScaledOwner *)arg0)->x + directionX);
    coords[1][1] = coords[0][1] = (s16)(s32)(((Game169510ScaledOwner *)arg0)->y - heightScale);
    coords[3][2] = coords[0][2] = (s16)(s32)(((Game169510ScaledOwner *)arg0)->z - directionZ);
    coords[2][0] = coords[1][0] = (s16)(s32)(((Game169510ScaledOwner *)arg0)->x - directionX);
    coords[3][1] = coords[2][1] = (s16)(s32)(((Game169510ScaledOwner *)arg0)->y + heightDelta);
    coords[2][2] = coords[1][2] = (s16)(s32)(((Game169510ScaledOwner *)arg0)->z + directionZ);
    if (*(u8 *)((u8 *)&arg1 + 3) != 0) {
        vertices[0].x = coords[1][0];
        vertices[0].y = coords[1][1];
        vertices[0].z = coords[1][2];
        vertices[1].x = coords[2][0];
        vertices[1].y = coords[2][1];
        vertices[1].z = coords[2][2];
        vertices[2].x = coords[3][0];
        vertices[2].y = coords[3][1];
        vertices[2].z = coords[3][2];
        vertices[3].x = coords[0][0];
        vertices[3].y = coords[0][1];
        vertices[3].z = coords[0][2];
    } else {
        vertices[0].x = coords[0][0];
        vertices[0].y = coords[0][1];
        vertices[0].z = coords[0][2];
        vertices[1].x = coords[1][0];
        vertices[1].y = coords[1][1];
        vertices[1].z = coords[1][2];
        vertices[2].x = coords[2][0];
        vertices[2].y = coords[2][1];
        vertices[2].z = coords[2][2];
        vertices[3].x = coords[3][0];
        vertices[3].y = coords[3][1];
        vertices[3].z = coords[3][2];
    }
    return result;
}

void func_1513EDB4(s32 arg0, s16 arg1) {
    func_1513EAD8(arg0, 0, arg1);
}

void func_1513EDE4(s32 arg0, s16 arg1) {
    func_1513EAD8(arg0, 1, arg1);
}


void *func_1513EE14(void *arg0, s16 arg1) {
    Game169510ScaledVertex *vertices;
    Game169510ScaledVertex *result;
    f32 mtx[4][4];
    f32 coords[4][2];
    f32 out[4][3];
    u8 fresh;

    func_151D5D60((u8 *)arg0 + 0x100, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != 0) {
        if (fresh != 0) {
            memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100), (u8 *)arg0 + 0xC0, 0x40);
            memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100) + 0x40, (u8 *)arg0 + 0xC0, 0x40);
        }
    } else {
        return 0;
    }
    func_150A8050(mtx, ((Game169510ScaledOwner *)arg0)->rx, ((Game169510ScaledOwner *)arg0)->ry, ((Game169510ScaledOwner *)arg0)->rz);
    mtx[3][0] = ((Game169510ScaledOwner *)arg0)->x;
    mtx[3][1] = ((Game169510ScaledOwner *)arg0)->y;
    mtx[3][2] = ((Game169510ScaledOwner *)arg0)->z;
    coords[0][0] = ((Game169510ScaledOwner *)arg0)->width;
    coords[0][1] = ((Game169510ScaledOwner *)arg0)->height;
    coords[1][0] = -((Game169510ScaledOwner *)arg0)->width;
    coords[1][1] = ((Game169510ScaledOwner *)arg0)->height;
    coords[2][0] = -((Game169510ScaledOwner *)arg0)->width;
    coords[2][1] = -((Game169510ScaledOwner *)arg0)->height;
    coords[3][0] = ((Game169510ScaledOwner *)arg0)->width;
    coords[3][1] = -((Game169510ScaledOwner *)arg0)->height;
    func_150A7960(mtx, coords[0][0], coords[0][1], 0.0f, &out[0][0], &out[0][1], &out[0][2]);
    func_150A7960(mtx, coords[1][0], coords[1][1], 0.0f, &out[1][0], &out[1][1], &out[1][2]);
    func_150A7960(mtx, coords[2][0], coords[2][1], 0.0f, &out[2][0], &out[2][1], &out[2][2]);
    func_150A7960(mtx, coords[3][0], coords[3][1], 0.0f, &out[3][0], &out[3][1], &out[3][2]);
    vertices->x = out[0][0];
    vertices->y = out[0][1];
    vertices->z = out[0][2];
    vertices->flag = 0;
    vertices++;
    vertices->x = out[1][0];
    vertices->y = out[1][1];
    vertices->z = out[1][2];
    vertices->flag = 0;
    vertices++;
    vertices->x = out[2][0];
    vertices->y = out[2][1];
    vertices->z = out[2][2];
    vertices->flag = 0;
    vertices++;
    vertices->x = out[3][0];
    vertices->y = out[3][1];
    vertices->z = out[3][2];
    vertices->flag = 0;
    return result;
}


void *func_1513F114(void *arg0, s16 arg1)
{
  Game169510ScaledVertex *vertices;
  Game169510ScaledVertex *result;
  f32 sinDir;
  f32 cosDir;
  f32 bottom;
  f32 top;
  f32 s;
  f32 c;
  f32 wc;
  f32 ws;
  f32 dist;
  f32 sign;
  f32 bs;
  f32 bc;
  f32 ts;
  f32 tc;
  f32 angle;
  u8 fresh;
  func_151D5D60(((u8 *) arg0) + 0x100, arg1, 0x40, (void **) (&vertices), &fresh);
  result = vertices;
  if (vertices != 0)
  {
    if (fresh != 0)
    {
      memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100), (u8 *)arg0 + 0xC0, 0x40);
      memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100) + 0x40, (u8 *)arg0 + 0xC0, 0x40);
    }
  }
  else
  {
    return 0;
  }
  {
    sinDir = D_800DD1D8[arg1];
    cosDir = D_800DD1E8[arg1];
    bottom = -(((Game169510ScaledOwner *) arg0)->sy * ((Game169510ScaledOwner *) arg0)->height);
    top = ((Game169510ScaledOwner *) arg0)->height + bottom;
    dist = (((((Game169510ScaledOwner *) arg0)->rx - ((Game169510ScaledOwner *) arg0)->x) * cosDir) - (sinDir * (((Game169510ScaledOwner *) arg0)->rz - ((Game169510ScaledOwner *) arg0)->z))) * D_800BE9A8;
    sign = -1.0f;
    if (dist < 0.0f)
    {
      sign = 1.0f;
      dist = -dist;
    }
    angle = sqrtf(dist) * ((Game169510ScaledOwner *) arg0)->sx;
    if (angle > 1.57079637050628662109375f)
    {
      angle = 1.57079637050628662109375f;
    }
    angle *= sign;
    s = sinf(angle);
    c = cosf(angle);
    wc = ((Game169510ScaledOwner *) arg0)->width * c;
    ws = ((Game169510ScaledOwner *) arg0)->width * s;
    ts = top * s;
    tc = top * c;
    bs = bottom * s;
    bc = bottom * c;
    vertices[0].x = ((Game169510ScaledOwner *) arg0)->x + ((wc - bs) * cosDir);
    vertices[0].y = (((Game169510ScaledOwner *) arg0)->y + bc) + ws;
    vertices[0].z = ((Game169510ScaledOwner *) arg0)->z - ((wc - bs) * sinDir);
    vertices[0].flag = 0;
    dist = -(wc + bs);
    vertices[1].x = ((Game169510ScaledOwner *) arg0)->x + (dist * cosDir);
    vertices[1].y = (((Game169510ScaledOwner *) arg0)->y + bc) - ws;
    vertices[1].z = ((Game169510ScaledOwner *) arg0)->z - (dist * sinDir);
    vertices[1].flag = 0;
    vertices[2].x = ((Game169510ScaledOwner *) arg0)->x + ((-(wc + ts)) * cosDir);
    vertices[2].y = (((Game169510ScaledOwner *) arg0)->y + tc) - ws;
    vertices[2].z = ((Game169510ScaledOwner *) arg0)->z - ((-(wc + ts)) * sinDir);
    vertices[2].flag = 0;
    dist = wc - ts;
    vertices[3].x = ((Game169510ScaledOwner *) arg0)->x + (dist * cosDir);
    vertices[3].y = (((Game169510ScaledOwner *) arg0)->y + tc) + ws;
    vertices[3].z = ((Game169510ScaledOwner *) arg0)->z - (dist * sinDir);
    vertices[3].flag = 0;
  }
  return result;
}

s32 func_15140410(s32, s32, s32, s16);

void func_1513F4B0(struct210 *arg0, s16 arg1) {
    func_15140410(arg0, &arg0->unk110, &arg0->unk11C, arg1);
}

s16 func_15143044(u8, s32);

Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2) {
    extern s16 D_800DD1BE;
    extern u8 D_800A4BA8[];
    s32 v0;
    u8 *p;

    v0 = func_15143044(arg1, 0);
    if (v0 != D_800DD1BE) {
        if (*arg2 == 1) {
            gDPPipeSync(gdl++);
            *arg2 = 0;
        }
        p = &D_800A4BA8[arg1 * 16];
        gDPSetCombine(gdl++,
            GCCc0w0(p[0], p[2], p[4], p[6]) | GCCc1w0(p[8], p[10]),
            GCCc0w1(p[1], p[3], p[5], p[7]) | GCCc1w1(p[9], p[12], p[14], p[11], p[13], p[15]));
        D_800DD1BE = v0;
    }
    return gdl;
}
// PERMUTER NO ZERO (best 730). word0 (first ~52 instrs) BYTE-PERFECT; word1's ops are
// byte-identical to target but IDO's delay-slot scheduling gives a different temp-register
// rotation from the first pair on (all `r`/reorder marks, no wrong ops). Reconstruction:
// typedef struct { u8 unk0..unkF; } CombineEntry;  // 16-byte combiner mux table entry
// extern CombineEntry D_800A4BA8[]; extern s16 D_800DD1BE; extern s16 func_15143044(u8, s32);
// Gfx *func_1513F4E4(Gfx *gfx, u8 arg1, u8 *arg2) {
//     s16 temp_v0 = func_15143044(arg1, 0);
//     if (temp_v0 != D_800DD1BE) {
//         CombineEntry *e; Gfx *g2;
//         if (arg2->w == 1) { gDPPipeSync(gfx++); arg2->w = 0; }
//         e = &D_800A4BA8[arg1];
//         g2 = gfx++;
//         g2->words.w0 = (((e->unkA & 0x1F) | ((e->unk8 & 0xF) << 5) | ((e->unk0 & 0xF) << 20)
//              | ((e->unk2 & 0x1F) << 15) | ((e->unk4 & 7) << 12) | ((e->unk6 & 7) << 9)) & 0xFFFFFF) | 0xFC000000;
//         g2->words.w1 = (e->unkF & 7) | ((e->unk9 & 0xF) << 24) | ((e->unkC & 7) << 21) | ((e->unkE & 7) << 18)
//              | ((e->unkB & 7) << 6) | ((e->unkD & 7) << 3) | (e->unk1 << 28) | ((e->unk3 & 7) << 15)
//              | ((e->unk5 & 7) << 12) | ((e->unk7 & 7) << 9);
//         D_800DD1BE = temp_v0;
//     }
//     return gfx;
// }

void func_1513F680(struct171 *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4) {
    arg0->unk70 = arg1;
    arg0->unk71 = arg2;
    arg0->unk72 = arg3;
    arg0->unk73 = arg4;
}

void func_1513F6C0(struct171 *arg0, u8 arg1, u8 arg2) {
    *(u8 *)((u8 *)arg0 + 0x80) = arg1;
    *(u8 *)((u8 *)arg0 + 0x81) = arg2;
}

/* Grows a particle's width and height by motion.grow.rate per second. */
s32 func_1513F6E8(Particle *particle) {
    f32 rate = particle->motion.grow.rate;

    particle->def.width += rate * D_800BE9A4;
    particle->def.height += rate * D_800BE9A4;
    return 1;
}

typedef struct {
    struct17 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Struct1513F728Sub;

typedef struct {
    char pad0[0x34];
    struct17 unk34;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    char pad4C[0xC4];
    Struct1513F728Sub unk110;
} Struct1513F728Obj;

s32 func_1513F728(Struct1513F728Obj *arg0) {
    Struct1513F728Sub *sub;
    struct17 old;
    s32 i;

    sub = (Struct1513F728Sub *)((u8 *)arg0 + 0x110);
    old = sub->unk0;
    for (i = D_800BE9E4; i != 0; i--) {
        sub->unk0.unk0 *= sub->unk10;
        sub->unk0.unk4 *= sub->unk10;
        sub->unk0.unk8 *= sub->unk10;
    }
    {
        f32 ax;
        f32 ay;
        f32 az;

        sub->unk0.unk4 += sub->unkC * D_800BE9A4;
        ax = (sub->unk0.unk0 - old.unk0) * D_800BE9A8;
        ay = (sub->unk0.unk4 - old.unk4) * D_800BE9A8;
        az = (sub->unk0.unk8 - old.unk8) * D_800BE9A8;
        arg0->unk34.unk0 += (old.unk0 + 0.5f * ax * D_800BE9A4) * D_800BE9A4;
        arg0->unk34.unk4 += (old.unk4 + 0.5f * ay * D_800BE9A4) * D_800BE9A4;
        arg0->unk34.unk8 += (old.unk8 + 0.5f * az * D_800BE9A4) * D_800BE9A4;
    }
    arg0->unk40 += sub->unk14 * D_800BE9A4;
    arg0->unk44 += sub->unk18 * D_800BE9A4;
    arg0->unk48 += sub->unk1C * D_800BE9A4;
    return 1;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f1513FAB4;

typedef struct {
    u8 pad0[0x2C];
    f32 width;
    f32 length;
    Vec3f1513FAB4 start;
    Vec3f1513FAB4 end;
} Beam1513FAB4;

typedef struct {
    s16 x, y, z, flag;
    u8 other[8];
} BeamVtx1513FAB4;

typedef struct { f32 w; f32 l; } Scale1513FAB4;

extern f32 *func_15144B34(s32);
extern void func_150AD8B0(f32 *, f32 *, f32 *);

void *func_1513FAB4(Beam1513FAB4 *arg0, u8 arg1, Scale1513FAB4 *arg2, s16 arg3);

void func_1513FA2C(void *arg0, s16 arg1) {
    f32 stack_vals[2];
    stack_vals[0] = 1.0f;
    stack_vals[1] = 1.0f;
    func_1513FAB4(arg0, 0, stack_vals, arg1);
}

void func_1513FA70(s32 arg0, s16 arg1) {
    f32 sp18[2];

    sp18[0] = 1.0f;
    sp18[1] = 1.0f;
    func_1513FAB4(arg0, 1, sp18, arg1);
}



void *func_1513FAB4(Beam1513FAB4 *arg0, u8 arg1, Scale1513FAB4 *arg2, s16 arg3) {
    BeamVtx1513FAB4 *vertices;
    void *result;
    Vec3f1513FAB4 *camera;
    Vec3f1513FAB4 delta;
    Vec3f1513FAB4 end;
    Vec3f1513FAB4 middle;
    Vec3f1513FAB4 view;
    Vec3f1513FAB4 side;
    f32 lengthSquared;
    f32 length;
    f32 width;
    u8 fresh;

    func_151D5D60((u8 *)arg0 + 0x100, arg3, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != 0) {
        if (fresh != 0) {
            memcpy(*(u8 **)((u8 *)arg0 + arg3 * 4 + 0x100), (u8 *)arg0 + 0xC0, 0x40);
            memcpy(*(u8 **)((u8 *)arg0 + arg3 * 4 + 0x100) + 0x40, (u8 *)arg0 + 0xC0, 0x40);
        }
    } else {
        return 0;
    }
    camera = (Vec3f1513FAB4 *)func_15144B34(arg3);
    width = arg0->width * arg2->w;
    length = arg0->length * arg2->l;
    delta.x = arg0->end.x - arg0->start.x;
    delta.y = arg0->end.y - arg0->start.y;
    delta.z = arg0->end.z - arg0->start.z;
    delta.x *= length;
    delta.y *= length;
    delta.z *= length;
    end.x = arg0->start.x + delta.x;
    end.y = arg0->start.y + delta.y;
    end.z = arg0->start.z + delta.z;
    middle.x = arg0->start.x + delta.x * 0.5f;
    middle.y = arg0->start.y + delta.y * 0.5f;
    middle.z = arg0->start.z + delta.z * 0.5f;
    view.x = middle.x - camera->x;
    view.y = middle.y - camera->y;
    view.z = middle.z - camera->z;
    func_150AD8B0(&delta.x, &view.x, &side.x);
    lengthSquared = side.x * side.x + side.y * side.y + side.z * side.z;
    if (lengthSquared != 0.0f) {
        f32 scale = width / sqrtf(lengthSquared);

        side.x = side.x * scale;
        side.y = side.y * scale;
        side.z = side.z * scale;
    } else {
        side.x = 0;
        side.y = 0;
        side.z = 0;
    }
    if (arg1 != 0) {
        vertices->x = arg0->start.x + side.x;
        vertices->y = arg0->start.y + side.y;
        vertices->z = arg0->start.z + side.z;
        vertices->flag = 0;
        vertices++;
        vertices->x = arg0->start.x - side.x;
        vertices->y = arg0->start.y - side.y;
        vertices->z = arg0->start.z - side.z;
        vertices->flag = 0;
        /* IDO ugen reads stale tree-node memory left by earlier functions in this file;
           these empty statements pad the node count so that state lines up. */
        ;
        vertices++;
        vertices->x = end.x - side.x;
        vertices->y = end.y - side.y;
        vertices->z = end.z - side.z;
        vertices->flag = 0;
        vertices++;
        vertices->x = end.x + side.x;
        vertices->y = end.y + side.y;
        vertices->z = end.z + side.z;
        vertices->flag = 0;
    } else {
        vertices->x = arg0->start.x - side.x;
        vertices->y = arg0->start.y - side.y;
        vertices->z = arg0->start.z - side.z;
        vertices->flag = 0;
        ;;;;;;;;
        vertices++;
        vertices->x = end.x - side.x;
        vertices->y = end.y - side.y;
        vertices->z = end.z - side.z;
        vertices->flag = 0;
        vertices++;
        vertices->x = end.x + side.x;
        vertices->y = end.y + side.y;
        vertices->z = end.z + side.z;
        vertices->flag = 0;
        vertices++;
        vertices->x = arg0->start.x + side.x;
        vertices->y = arg0->start.y + side.y;
        vertices->z = arg0->start.z + side.z;
        vertices->flag = 0;
    }
    return result;
}

/* Sets the texture coordinates of a quad to cover the whole texture, mirrored per the flip bits. */
void func_1513FFF4(Vtx *vtx, u8 texture, u8 flip) {
    TextureInfo *tex;
    u16 maxS;
    u16 maxT;
    s32 coord;

    if (texture != 0xFF) {
        tex = &D_80090B60[texture];
        maxS = tex->width - 1;
        maxT = tex->height - 1;

        if (flip & 1) {
            coord = maxS << 6;
        } else {
            coord = 0;
        }
        vtx[3].v.tc[0] = coord;
        vtx[0].v.tc[0] = coord;

        if (flip & 1) {
            coord = 0;
        } else {
            coord = maxS << 6;
        }
        vtx[2].v.tc[0] = coord;
        vtx[1].v.tc[0] = coord;

        if (flip & 2) {
            coord = maxT << 6;
        } else {
            coord = 0;
        }
        vtx[1].v.tc[1] = coord;
        vtx[0].v.tc[1] = coord;

        if (flip & 2) {
            coord = 0;
        } else {
            coord = maxT << 6;
        }
        vtx[3].v.tc[1] = coord;
        vtx[2].v.tc[1] = coord;
    }
}

/* Copies four vertex colours into a quad. */
void func_151400D0(Vtx *vtx, VtxColor *colors) {
    s32 i;

    for (i = 0; i < 4; i++) {
        vtx->v.flag = colors->flag;
        vtx->v.cn[0] = colors->r;
        vtx->v.cn[1] = colors->g;
        vtx->v.cn[2] = colors->b;
        vtx->v.cn[3] = colors->a;
        vtx->v.flag = 0;
        vtx++;
        colors++;
    }
}

void *func_15140190(void *arg0, s16 arg1) {
    Game169510ScaledVertex *vertices;
    Game169510ScaledVertex *result;
    f32 mtx[4][4];
    f32 coords[4][3];
    u8 i;
    u8 fresh;

    func_151D5D60((u8 *)arg0 + 0x100, arg1, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != 0) {
        if (fresh != 0) {
            memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100), (u8 *)arg0 + 0xC0, 0x40);
            memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100) + 0x40, (u8 *)arg0 + 0xC0, 0x40);
        }
    } else {
        return 0;
    }
    {
        s16 pad;

        coords[0][0] = ((Game169510ScaledOwner *)arg0)->width;
        coords[0][1] = ((Game169510ScaledOwner *)arg0)->height;
        coords[0][2] = 0.0f;
        coords[1][0] = -((Game169510ScaledOwner *)arg0)->width;
        coords[1][1] = ((Game169510ScaledOwner *)arg0)->height;
        coords[1][2] = 0.0f;
        coords[2][0] = -((Game169510ScaledOwner *)arg0)->width;
        coords[2][1] = -((Game169510ScaledOwner *)arg0)->height;
        coords[2][2] = 0.0f;
        coords[3][0] = ((Game169510ScaledOwner *)arg0)->width;
        coords[3][1] = -((Game169510ScaledOwner *)arg0)->height;
        coords[3][2] = 0.0f;
        func_150A8050(mtx, ((Game169510ScaledOwner *)arg0)->rx, ((Game169510ScaledOwner *)arg0)->ry, ((Game169510ScaledOwner *)arg0)->rz);
        mtx[3][0] = ((Game169510ScaledOwner *)arg0)->x;
        mtx[3][1] = ((Game169510ScaledOwner *)arg0)->y;
        mtx[3][2] = ((Game169510ScaledOwner *)arg0)->z;
        for (i = 0; i < 4; i++) {
            f32 *c = coords[i];
            func_150A7960(mtx, c[0], c[1], 0.0f, &c[0], &c[1], &c[2]);
            vertices->x = c[0];
            vertices->y = c[1];
            vertices->z = c[2];
            vertices->flag = 0;
            vertices++;
        }
    }
    return result;
}

void func_151403A8(s32 arg0, u8 arg1) {
    func_15169260(&D_800A5168, 4, arg0, arg1);
}

void func_151403DC(s32 arg0, u8 arg1) {
    func_15169260(&D_800A5178, 3, arg0, arg1);
}

s32 func_15140410(s32 arg0, s32 arg1, s32 arg2, s16 arg3) {
    Game169510ScaledVertex *vertices;
    Game169510ScaledVertex *result;
    f32 ax, ay, az, bx, by, bz;

    {
        u8 fresh;

        func_151D5D60((u8 *)arg0 + 0x100, arg3, 0x40, (void **)&vertices, &fresh);
        result = vertices;
        if (vertices != 0) {
            if (fresh != 0) {
                memcpy(*(u8 **)((u8 *)arg0 + arg3 * 4 + 0x100), (u8 *)arg0 + 0xC0, 0x40);
                memcpy(*(u8 **)((u8 *)arg0 + arg3 * 4 + 0x100) + 0x40, (u8 *)arg0 + 0xC0, 0x40);
            }
        } else {
            return 0;
        }
    }
    ax = ((f32 *)arg1)[0] * ((Game169510ScaledOwner *)arg0)->width;
    ay = ((f32 *)arg1)[1] * ((Game169510ScaledOwner *)arg0)->width;
    az = ((f32 *)arg1)[2] * ((Game169510ScaledOwner *)arg0)->width;
    bx = ((f32 *)arg2)[0] * ((Game169510ScaledOwner *)arg0)->height;
    by = ((f32 *)arg2)[1] * ((Game169510ScaledOwner *)arg0)->height;
    bz = ((f32 *)arg2)[2] * ((Game169510ScaledOwner *)arg0)->height;
    vertices[0].x = (s32)(((Game169510ScaledOwner *)arg0)->x + ax + bx);
    vertices[0].y = (s32)(((Game169510ScaledOwner *)arg0)->y + ay + by);
    vertices[0].z = (s32)(((Game169510ScaledOwner *)arg0)->z + az + bz);
    vertices[0].flag = 0;
    vertices[1].x = (s32)(((Game169510ScaledOwner *)arg0)->x - ax + bx);
    vertices[1].y = (s32)(((Game169510ScaledOwner *)arg0)->y - ay + by);
    vertices[1].z = (s32)(((Game169510ScaledOwner *)arg0)->z - az + bz);
    vertices[1].flag = 0;
    vertices[2].x = (s32)(((Game169510ScaledOwner *)arg0)->x - ax - bx);
    vertices[2].y = (s32)(((Game169510ScaledOwner *)arg0)->y - ay - by);
    vertices[2].z = (s32)(((Game169510ScaledOwner *)arg0)->z - az - bz);
    vertices[2].flag = 0;
    vertices[3].x = (s32)(((Game169510ScaledOwner *)arg0)->x + ax - bx);
    vertices[3].y = (s32)(((Game169510ScaledOwner *)arg0)->y + ay - by);
    vertices[3].z = (s32)(((Game169510ScaledOwner *)arg0)->z + az - bz);
    vertices[3].flag = 0;
    return (s32)result;
}

/* Sort-list node (0x110 bytes); func_151406AC insertion-sorts one D_800DCE50 bucket by key. */
typedef struct Node151406AC {
    u8 pad0[4];
    struct Node151406AC *prev;
    struct Node151406AC *next;
    u8 padC[0xC];
    u8 unk18;
    u8 pad19[7];
    s32 unk20;
    u8 pad24[0xEC];
} Node151406AC;

extern u8 D_800DCE50[];

s32 func_151406AC(s32 arg0, s32 arg1, s32 arg2, s16 arg3) {
    Node151406AC head;
    Node151406AC **slot;
    Node151406AC *cur;
    Node151406AC *prev;
    Node151406AC *next;
    Node151406AC *node;
    s32 key;
    s32 nodeKey;

    if (arg3 != 0) {
        return arg0;
    }
    slot = &((Node151406AC *(*)[104])D_800DCE50)[arg2][arg1];
    node = *slot;
    head.unk18 = 0;
    head.unk20 = 0;
    head.next = node;
    /* BUG (original game): dereferences the list head before the NULL check below
       (and again after the sort); an empty bucket writes to address 4. */
    node->prev = &head;
    if (node != NULL) {
        for (cur = node->next; cur != NULL; cur = next) {
            prev = cur->prev;
            next = cur->next;
            key = (cur->unk18 << 8) + (cur->unk20 >> 16);
            for (node = prev; node != NULL; node = node->prev) {
                nodeKey = (node->unk18 << 8) + (node->unk20 >> 16);
                if (key < nodeKey) {
                    continue;
                }
                if (node != prev) {
                    prev->next = cur->next;
                    if (cur->next != NULL) {
                        cur->next->prev = cur->prev;
                    }
                    cur->next = node->next;
                    if (cur->next != NULL) {
                        cur->next->prev = cur;
                    }
                    cur->prev = node;
                    node->next = cur;
                }
                break;
            }
        }
    }
    *slot = head.next;
    head.next->prev = NULL;
    return arg0;
}
