#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Per-behaviour state that follows a particle (a DefParticleHead). */
typedef union {
    ParticleAxes axes;  /* func_1513D6FC, func_1513F4B0 */
    ActorRef owner;     /* func_1513E084 */
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
} DefParticleMotion;

#define PARTICLE_MOTION(particle) ((DefParticleMotion *)((particle) + 1))


/* Sprite update (QuadSprite.updateFn; 0 = none) and expire (QuadSprite.expireFn) callbacks; an update returning 0
 * expires the sprite, an expire callback returning nonzero keeps it. */
static s32 (*D_80089C70[])() = {
    NULL, func_1513416C, func_150B961C, func_150BA35C, func_150AEDD8, func_150DE2A4, func_150BB450, func_151AB090,
    func_150C5E0C, func_150C673C, func_150CB7CC, func_151AFBD4, func_150CA9D0, func_15136A1C,
};
static s32 (*D_80089CA8[])() = { NULL, func_151AB180, func_150C5EFC, func_150C682C };

/* Particle update (DefParticleHead.updateFn1, updateFn2; 0 = none) and expire callbacks, as for sprites. */
static s32 (*D_80089CB8[])() = {
    NULL, func_1513E6E8, func_1518A094, func_150B9560, func_150BA37C, func_1514A4EC, func_1514A594, func_1518EF54,
    func_1518F058, func_150DB114, func_15197AB4, func_151AC9EC, func_15190F9C, func_15191520, func_151DA368,
    func_150E6230, func_151B118C, func_151B1828, func_1514B87C, func_150CD17C, func_150C308C, func_15101378,
    func_151C2734, func_150AE35C, func_1513F6E8, func_1513F728, func_151C82D0, func_151B85AC, func_151CEA20,
    func_150EF38C, func_150F00EC, func_1519C26C, func_1514C258, func_1514C288, func_150E4174, func_151B8CFC,
    func_150F7E20, func_15141928, func_150F3214, func_1519EF04, func_150DEACC, func_15102D90,
};
static s32 (*D_80089D60[])() = {
    NULL, func_150B95FC, func_150BA424, func_1514143C, func_1514A19C, func_1514A380, func_1514A498, func_1518F108,
    func_1518F15C, func_1518F1A0, func_150DB630, func_15197BBC, func_1518F384, func_15149838, func_15149A94,
    func_15149BF4, func_15149C58, func_151ACA20, func_1519187C, func_151DA6A8, func_151DADA0, func_150E679C,
    func_151AFC08, func_150CBA30, func_150CC638, func_150C2FCC, func_151368A8, func_151B1478, func_1514AF74,
    func_1514B8B0, func_150CD17C, func_151022AC, func_15141478, func_151415D4, func_1519C4E4, func_1514BE00,
    func_1514BF7C, func_151BE210, func_150D317C, func_150D4CC4, func_151A6600, func_15141564, func_151BE6CC,
    func_150FD514,
};
static s32 (*D_80089E10[])() = { NULL, func_15149EC4, func_1514C2B8 };

/* Vertex functions of a particle (DefParticleHead.vertexFn), see func_1513E83C. The prototype keeps the viewport an
 * s16; most entries take their own view of the particle. */
static Vtx *(*D_80089E1C[])(DefParticleHead *, s16) = {
    func_1513E83C, func_151408A4, func_15149FD0, func_1513F114, func_1514A6A0, func_1518E73C, func_1518EB10,
    func_150DB2D8, func_150DB714, func_15197C10, func_15149D18, func_151AC810, func_1513F4B0, func_1519108C,
    func_151DAE28, func_150E63A0, func_151B14AC, func_150CD59C, func_151019C4, func_15101F50, func_151C2AD0,
    func_150AE36C, func_15102EB8, func_1513EE14, func_151C7E98, func_151B863C, func_1519C56C, func_1513FA2C,
    func_1513FA70, func_151BE4B8, func_15140190, func_150D2D6C, func_151A6350, func_150F7F58, func_1513EDB4,
    func_1513EDE4, func_150FDBA0, func_150F337C, func_150DEB58,
};

/* Render setup and reset display lists of the particles; their entry in the object type table points to them. */
Gfx D_80089EB8[] = {
    gsDPPipeSync(),
    gsSPMatrix(&D_80089470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPEndDisplayList(),
};
Gfx D_80089ED8[] = {
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsSPEndDisplayList(),
};

/* Destroy and free handlers of a sprite (SpriteDef.unk7) and of a particle (ParticleDef.type). */
static void (*D_80089EE8[])(QuadSprite *) = { func_1513C8D4, func_1513C8D4 };
static void (*D_80089EF0[])(QuadSprite *) = { func_1513C900, func_1513C900 };
static void (*D_80089EF8[])() = {
    func_1513CA6C, func_15101090, func_151011E8, func_151411E4, func_1513CA6C, func_1513CA6C, func_151C94D4,
    func_150EF7B0, func_151BE674, func_150F6420, func_151A6B10, func_150FDCAC, func_1513CA6C,
};
static void (*D_80089F2C[])() = {
    func_1513CAA0, func_15101148, func_15101210, func_15141250, func_1513CAA0, func_1513CAA0, func_151C95D8,
    func_150EF860, func_151BE6A0, func_150F644C, func_151A6B3C, func_150FDCD8, func_1513CAA0,
};
/* Event handler of a sprite (func_1513CF9C). */
static void (*D_80089F60[])(s32, s32, u8) = { func_1513CFD0 };
/* Event handlers of a particle (ParticleDef.type, func_1513DF9C); several take their own view of the particle and
 * of the event data. */
static void (*D_80089F64[])(DefParticleHead *, s32, u8) = {
    func_1513E070, func_1513E084, func_15102D50, func_151C86AC, func_151416E8, func_150EF910, func_151BE604,
    func_151BE604, func_151C4AB0, func_150FDB0C, func_150F31D4,
};

/* Per-frame update of a sprite: lifetime, fading and texture animation. */
void func_1513C060(QuadSprite *sprite) {
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
            numFrames = D_80090B60[sprite->def.texture].frameCount;
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
                func_1516972C(&sprite->header);
            }
        } else {
            func_1516972C(&sprite->header);
        }
    }
}

/* The default vertex colours of a sprite or particle: opaque white. */
const VtxColor D_800A4AA0[4] = {
    { 0xFF, 0xFF, 0xFF, 0xFF, 0 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0 },
};

/* Allocates a sprite from a template; extraSize bytes follow it. */
QuadSprite *func_1513C350(void *def, const void *colors, u8 updateFn, u8 expireFn, u8 flip, s32 model, s32 arg6,
                         Vec3f *pos, s32 extraSize, u8 arg9, s32 argA) {
    QuadSprite *sprite;
    u8 kind;
    s32 i;

    if (colors == NULL) {
        colors = &D_800A4AA0;
    }
    if (((SpriteDef *)def)->flags & 0x40000) {
        kind = 0x4A;
    } else {
        kind = 0x1B;
    }
    sprite = func_15167A68(kind, argA, extraSize + sizeof(QuadSprite), 1, arg9, 1);
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
        sprite->link.handles[i] = NULL;
    }

    sprite->link.sharedHandle = NULL;
    if (model) {
        for (i = 0; i <= D_80082FA0; i++) {
            sprite->link.handles[i] = func_1515D480(model);
        }
        sprite->link.sharedHandle = func_1515D440();
    }
    if (pos) {
        sprite->pos = *pos;
    }
    return sprite;
}

/* Spawns a flat (xz-plane) sprite at (x, y, z). */
QuadSprite *func_1513C4EC(void *def, const void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width, f32 length,
                         u8 angle, u8 flip, s32 model, s32 arg6, s32 extraSize, u8 argE, s32 argF) {
    QuadSprite *sprite;
    Vec3f pos;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    sprite = func_1513C350(def, colors, updateFn, expireFn, flip, model, arg6, &pos, extraSize, argE, argF);
    if (sprite == NULL) {
        return NULL;
    }
    func_1513E13C(sprite, x, y, z, width, length, angle);
    return sprite;
}

/* Spawns a flat (xz-plane) sprite without a model or stored position. */
QuadSprite *func_1513C5B0(void *def, const void *colors, u8 updateFn, u8 expireFn, f32 x, f32 y, f32 z, f32 width,
                         f32 length, u8 angle, u8 flip, s32 extraSize, u8 argC, s32 argD) {
    QuadSprite *sprite = func_1513C350(def, colors, updateFn, expireFn, flip, 0, 0, NULL, extraSize, argC, argD);

    if (sprite == NULL) {
        return NULL;
    }
    func_1513E13C(sprite, x, y, z, width, length, angle);
    return sprite;
}

/* Spawns a sprite oriented by func_1513E2AC, with the default colours. */
QuadSprite *func_1513C650(void *def, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                         f32 height, u8 angle, u8 flip, s32 model, s32 arg6, s32 extraSize, u8 argE, s32 argF) {
    QuadSprite *sprite;
    Vec3f pos;

    pos.x = x;
    pos.y = y;
    pos.z = z;

    sprite = func_1513C350(def, 0, updateFn, expireFn, flip, model, arg6, &pos, extraSize, argE, argF);
    if (sprite == NULL) {
        return NULL;
    }

    if (func_1513E2AC(sprite, NULL, surface, x, y, z, width, height, angle) == 0) {
        func_1516972C(&sprite->header);
        return NULL;
    }
    return sprite;
}

/* As func_1513C650, without a model or stored position. */
QuadSprite *func_1513C73C(void *def, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                         f32 height, u8 angle, u8 flip, s32 extraSize, u8 argC, s32 argD) {
    QuadSprite *sprite = func_1513C350(def, 0, updateFn, expireFn, flip, 0, 0, NULL, extraSize, argC, argD);

    if (sprite == NULL) {
        return NULL;
    }

    if (func_1513E2AC(sprite, NULL, surface, x, y, z, width, height, angle) == 0) {
        func_1516972C(&sprite->header);
        return NULL;
    }
    return sprite;
}

/* As func_1513C73C; arg1 is unused and the default colours are used. */
QuadSprite *func_1513C804(void *def, s32 arg1, u8 updateFn, u8 expireFn, CollisionTri *surface, f32 x, f32 y, f32 z,
                         f32 width, f32 height, u8 angle, u8 flip, s32 extraSize, u8 argD, s32 argE) {
    QuadSprite *sprite = func_1513C350(def, 0, updateFn, expireFn, flip, 0, 0, NULL, extraSize, argD, argE);

    if (sprite == NULL) {
        return NULL;
    }
    if (func_1513E2AC(sprite, NULL, surface, x, y, z, width, height, angle) == 0) {
        func_1516972C(&sprite->header);
        return NULL;
    }
    return sprite;
}

void func_1513C8D4(QuadSprite *sprite) {
    func_1513C92C(sprite);
    func_15169804(&sprite->header);
}

void func_1513C900(QuadSprite *sprite) {
    func_1513C92C(sprite);
    func_15169824(&sprite->header);
}

/* Releases a sprite's light buffers. */
void func_1513C92C(QuadSprite *sprite) {
    s32 i;

    // matching: the (s32) round-trips make IDO copy the buffer pointer through an int temp
    for (i = 0; i <= D_80082FA0; i++) {
        if (sprite->link.handles[i]) {
            func_100043B4((void *)(s32)sprite->link.handles[i], 4);
        }
    }
    if (sprite->link.sharedHandle != NULL) {
        func_100043B4((void *)(s32)sprite->link.sharedHandle, 4);
    }
}

void func_1513C9B0(QuadSprite *sprite) {
    s32 index;

    if (sprite->def.flags & 0x1000) {
        index = sprite->def.unk7;
    } else {
        index = 0;
    }
    D_80089EE8[index](sprite);
}

void func_1513C9FC(QuadSprite *sprite) {
    s32 index;

    if (sprite->def.flags & 0x1000) {
        index = sprite->def.unk7;
    } else {
        index = 0;
    }
    D_80089EF0[index](sprite);
}

/* Frees a particle's per-viewport vertex buffers. */
void func_1513CA48(DefParticleHead *particle) {
    func_151D5E30(particle->vtxBuffers);
}

void func_1513CA6C(DefParticleHead *particle) {
    func_1513CA48(particle);
    func_1513CAD4(particle);
    func_15169804(&particle->header);
}

void func_1513CAA0(DefParticleHead *particle) {
    func_1513CA48(particle);
    func_1513CAD4(particle);
    func_15169824(&particle->header);
}

/* Releases a particle's light buffers. */
void func_1513CAD4(DefParticleHead *arg0) {
    s32 i;

    // matching: the (s32) round-trips make IDO copy the buffer pointer through an int temp
    for (i = 0; i <= D_80082FA0; i++) {
        if (arg0->link.handles[i]) {
            func_100043B4((void *)(s32)arg0->link.handles[i], 4);
        }
    }
    if (arg0->link.sharedHandle != NULL) {
        func_100043B4((void *)(s32)arg0->link.sharedHandle, 4);
    }
}

void func_1513CB58(DefParticleHead *particle) {
    s32 index;

    if (particle->def.flags & 0x400000) {
        index = particle->def.type;
    } else {
        index = 0;
    }
    D_80089EF8[index](particle);
}

void func_1513CBA4(DefParticleHead *particle) {
    s32 index;

    if (particle->def.flags & 0x400000) {
        index = particle->def.type;
    } else {
        index = 0;
    }
    D_80089F2C[index](particle);
}

/* The render modes of the effects (the blender words of the two cycles), indexed by the low byte of a render mode
 * field; most effect files read it with their own view. */
const RenderModePair D_800A4AC8[28] = {
{ 0x004049D8, 0x001049D8 },
{ 0x004049D8, 0x001049D8 },
{ 0x00404DD8, 0x00404DD8 },
{ 0x00404350, 0x00104350 },
{ 0x00404358, 0x00104358 },
{ 0x004049F8, 0x001049F8 },
{ 0x00404DF8, 0x00104DF8 },
{ 0x004041C8, 0x001041C8 },
{ 0x00404240, 0x00104240 },
{ 0x00443078, 0x00113078 },
{ 0xC8000000, 0x00112230 },
{ 0xC8000000, 0x00104A50 },
{ 0xC8000000, 0x001049F8 },
{ 0x0C080000, 0x001041C8 },
{ 0x0C080000, 0x001049F8 },
{ 0x0C080000, 0x001049D8 },
{ 0x00442078, 0x00112078 },
{ 0x0C080000, 0x00104A50 },
{ 0x0C083238, 0x03023238 },
{ 0x00442048, 0x00112048 },
{ 0x00442230, 0x00112230 },
{ 0x004049F8, 0x001049F8 },
{ 0x00442078, 0x00112078 },
{ 0x00404340, 0x00104340 },
{ 0x00404870, 0x00104870 },
{ 0x00404840, 0x00104840 },
{ 0x0C080000, 0x00104350 },
{ 0x00404340, 0x00104340 },
};

/* Draws a sprite's quad (and attached model) for one viewport. */
Gfx *func_1513CBF0(Gfx *gfx, QuadSprite *sprite, s16 viewport) {
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
                                ((sprite->def.texture == 0xFF) ? 0 : (u32)D_800D2C9C /* matching: unsigned ternary */) | G_TF_BILERP | G_TC_FILT |
                                G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[blendMode].cycle1 | D_800A4AC8[blendMode].cycle2, &needSync);

        gSPVertex(gfx++, sprite->vtx, 4, 0);
        gSP1Triangle(gfx++, 0, 1, 2, 0);
        gSP1Triangle(gfx++, 0, 2, 3, 0);
    }
    return gfx;
}

void func_1513CF9C(s32 arg0, s32 arg1, u8 arg2) {
    D_80089F60[0](arg0, arg1, arg2);
}

void func_1513CFD0(s32 arg0, s32 arg1, u8 arg2) {
}


/* Per-frame update of a particle: lifetime, fading and texture animation. */
void func_1513CFE4(DefParticleHead *particle) {
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
            numFrames = D_80090B60[particle->def.texture].frameCount;
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
        func_1516972C(&particle->header);
    }
}

/* Allocates a particle from a template; motionSize extra bytes follow it for its behaviour state. */
void *func_1513D2F0(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 model,
                    s32 arg8, s32 motionSize, u8 argA, s32 argB) {
    DefParticleHead *particle;
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
    particle = func_15167A68(kind, argB, motionSize + sizeof(DefParticleHead), 1, argA,
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
    particle->ground.height = -10000.0f;
    particle->ground.surface = NULL;
    particle->ground.source = 0;
    particle->ground.flags = 0;
    particle->ground.material = 0;
    particle->link.model = model;
    particle->link.unk1C = arg8;
    particle->link.unk4 = 0;
    for (i = 0; i < 4; i++) {
        particle->link.handles[i] = NULL;
    }

    particle->link.sharedHandle = NULL;
    if (model) {
        for (i = 0; i <= D_80082FA0; i++) {
            particle->link.handles[i] = func_1515D480(model);
        }
        particle->link.sharedHandle = func_1515D440();
    }
    return particle;
}

void *func_1513D4B8(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip,
                    s32 motionSize, u8 arg8, s32 arg9) {
    return func_1513D2F0(def, colors, updateFn1, updateFn2, expireFn, vertexFn, flip, 0, 0, motionSize, arg8, arg9);
}

/* As func_1513D4B8 with the default colours. */
void *func_1513D524(void *def, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 motionSize,
                    u8 arg7, s32 arg8) {
    return func_1513D2F0(def, &D_800A4AA0, updateFn1, updateFn2, expireFn, vertexFn, flip, 0, 0, motionSize, arg7, arg8);
}

/* Spawns a particle whose quad is spanned by two axes from func_1513D6FC (vertex function 12). */
void *func_1513D594(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width,
                    f32 height, Vec3s **triPtrs, CollisionTri *surface, s32 model, s32 argC, u8 argD,
                    s32 motionSize, u8 argF, s32 arg10) {
    DefParticleHead *particle;

    if (colors == 0) {
        colors = &D_800A4AA0;
    }
    particle = func_1513D2F0(def, colors, updateFn1, updateFn2, expireFn, 12, flip, model, argC,
                             motionSize + sizeof(PARTICLE_MOTION(particle)->axes), argF, arg10);
    if ((particle != NULL) &&
        (func_1513D6FC(&PARTICLE_MOTION(particle)->axes, angle, width, height, triPtrs, surface, argD) == 0)) {
        func_1516972C(&particle->header);
        return NULL;
    }
    return particle;
}

void *func_1513D668(void *def, const void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle,
                    f32 width, f32 height, Vec3s **triPtrs, CollisionTri *surface, u8 argB, s32 motionSize, u8 argD,
                    s32 argE) {
    return func_1513D594(def, colors, updateFn1, updateFn2, expireFn, flip, angle, width, height, triPtrs, surface, 0,
                         0, argB, motionSize, argD, argE);
}

/*
 * Computes the two half-axes of a particle quad rotated by angle. With onSlope the quad lies on the
 * slope func_1510E388 returns (each axis keeps its length); otherwise it lies in the plane of the
 * basis func_15144E80 returns.
 */
s32 func_1513D6FC(ParticleAxes *axes, s16 angle, f32 width, f32 height, Vec3s **triPtrs, CollisionTri *surface,
                   u8 onSlope) {
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
        if (func_1510E388(triPtrs, surface->v, &slopeX, &slopeZ) == 0) {
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
            if (0 != heightCos) { // matching: operand order changes the FP compare
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
            axes->u.x = v.y * heightCos;
            axes->u.y = (v.y * v.z) + (k2 * v.x) + 0.5f;
            axes->u.z = k2 * heightSin;
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
            axes->v.x = v.y * widthSin;
            axes->v.y = (v.y * v.z) + (k2 * v.x) + 0.5f;
            axes->v.z = k2 * t;
        }
    } else {
        cosB = func_151423D8(angle - 0x40);
        sinB = func_151423D8(angle);
        if (func_15144E80(surface, &basisU, &basisV, &basisN) == 0) {
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
            axes->u.x = (sinA * basisU.x) + (t * basisV.x);
            axes->u.y = (sinA * basisU.y) + (t * basisV.y);
            axes->u.z = (sinA * basisU.z) + (t * basisV.z);
            sinA = -height * cosB;
            t = height * sinB;
            axes->v.x = (sinA * basisU.x) + (t * basisV.x);
            axes->v.y = (sinA * basisU.y) + (t * basisV.y);
            axes->v.z = (sinA * basisU.z) + (t * basisV.z);
        }
        return 1;
    }
end: // matching: an unused label keeps the final return 1 in its own basic block
    return 1;
}


/* Draws a particle's quad (and attached model) for one viewport. */
Gfx *func_1513DB00(Gfx *gfx, DefParticleHead *particle, s16 viewport) {
    s32 pad; // matching: unused stack slot
    u8 needSync;
    s16 primR;
    s16 primG;
    s16 primB;
    s16 primA;
    s16 envR;
    s16 envG;
    s16 envB;
    s16 envA;
    u8 blendMode;
    u8 combiner;
    s16 cullFlags;
    s32 geometry;
    Vtx *vtx;

    // matching: the (s32) round-trips below make IDO copy the buffer pointer through an int temp
    if (particle->active & 1) {
        needSync = TRUE;
        if ((particle->def.flags & 8) && !(particle->def.viewportMask & (1 << viewport))) {
            if (particle->vtxBuffers[viewport]) {
                func_100043B4((void *)(s32)particle->vtxBuffers[viewport], 2);
                particle->vtxBuffers[viewport] = NULL;
            }
            return gfx;
        }
        blendMode = particle->def.renderModes & 0xFF;
        combiner = (particle->def.renderModes & 0xFF00) >> 8;
        cullFlags = particle->def.flags & 0xC000;
        if ((particle->def.flags & 0x1000000) && particle->def.unk48 != NULL &&
            !(particle->def.unk48->views & (1 << viewport))) {
            if (particle->vtxBuffers[viewport]) {
                func_100043B4((void *)(s32)particle->vtxBuffers[viewport], 2);
                particle->vtxBuffers[viewport] = NULL;
            }
            return gfx;
        }
        vtx = D_80089E1C[particle->vertexFn](particle, viewport);
        if (vtx == NULL) {
            if (particle->vtxBuffers[viewport]) {
                func_100043B4((void *)(s32)particle->vtxBuffers[viewport], 2);
                particle->vtxBuffers[viewport] = NULL;
            }
            return gfx;
        }
        gfx = func_151462C8(gfx, &particle->link, 0, 0, 0, viewport, &particle->def.pos, 2,
                            (particle->def.flags & 0x4000000) ? particle->def.unk50 : 0);
        func_151441A4(&primR, &primG, &primB, &primA, particle->def.rgba[0], particle->def.rgba[1],
                      particle->def.rgba[2], particle->def.rgba[3], 0xFF, 0xFF, 0xFF, particle->def.unk44,
                      particle->def.unk45, (particle->def.flags & 0x40000) ? particle->def.unk46 : 4);
        func_151442FC(&envR, &envG, &envB, &envA, particle->def.rgba[0], particle->def.rgba[1],
                      particle->def.rgba[2], particle->def.rgba[3], 0xFF, 0xFF, 0xFF, particle->def.unk44,
                      particle->def.unk45, (particle->def.flags & 0x80000) ? particle->def.unk47 : 5);
        geometry = 0;
        if (cullFlags & 0x4000) {
            geometry = G_CULL_FRONT;
        }
        if (cullFlags & 0x8000) {
            geometry |= G_CULL_BACK;
        }
        if (particle->def.flags & 0x100000) {
            geometry |= G_LIGHTING;
        }
        gfx = func_15142B7C(gfx, geometry | G_ZBUFFER, G_CULL_BOTH);
        if (particle->def.texture != 0xFF) {
            s32 clamp;

            clamp = (particle->def.flags & 0x200000) ? 0 : 2;
            gfx = func_15142E24(gfx, &D_80090B60[particle->def.texture], particle->def.frame, clamp, 0, 0,
                                particle->def.texture, 0, 0, &needSync,
                                (particle->def.flags & 0x40000000) ? 0x3E : 3);
        }
        gfx = func_15142C10(gfx, envR, envG, envB, envA, &needSync);
        gfx = func_15142CF0(gfx, 0xF2, 0, primR, primG, primB, primA, &needSync);
        gfx = func_1513F4E4(gfx, combiner, &needSync);
        gfx = func_15142FBC(gfx,
                            G_PM_NPRIMITIVE | ((particle->def.flags & 0x20000000) ? G_CYC_2CYCLE : G_CYC_1CYCLE) |
                                G_TP_PERSP | G_TD_CLAMP | G_TL_TILE |
                                ((particle->def.texture == 0xFF) ? 0 : (u32)D_800D2C9C /* matching: unsigned ternary */) | G_TF_BILERP | G_TC_FILT |
                                G_CK_NONE | G_CD_NOISE | G_AD_NOISE,
                            D_800A4AC8[blendMode].cycle1 | D_800A4AC8[blendMode].cycle2, &needSync);
        gSPVertex(gfx++, vtx, 4, 0);
        gSP2Triangles(gfx++, 0, 1, 2, 0, 0, 2, 3, 0);
    }
    return gfx;
}


/* Dispatches to the D_80089F64 handler for the particle's type. */
void func_1513DF9C(DefParticleHead *particle, s32 arg1, u8 arg2) {
    s32 index;

    if (!(particle->def.flags & 0x400000)) {
        index = 0;
    } else {
        switch (particle->def.type) {
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
                index = 0;
                break;
        }
    }
    D_80089F64[index](particle, arg1, arg2);
}

void func_1513E070(DefParticleHead *particle, s32 arg1, u8 arg2) {
}

/* Message handler: frees the particle when its owner goes away (0x1A), or retargets it on an actor swap (0x2D). */
void func_1513E084(DefParticleHead *particle, ActorMsg *msg, u8 eventId) {
    ActorRef *link = &PARTICLE_MOTION(particle)->owner;

    if (eventId == 0x1A) {
        if ((link->actor == msg->ref.actor) || (link->uniqueId == msg->ref.uniqueId)) {
            func_1516972C(&particle->header);
        }
    } else if (eventId == 0x2D) {
        if (link->actor == msg->swap.actorA) {
            link->actor = msg->swap.actorB;
            link->uniqueId = msg->swap.idB;
        } else if (link->actor == msg->swap.actorB) {
            link->actor = msg->swap.actorA;
            link->uniqueId = msg->swap.idA;
        }
    }
}

void func_1513E134(void) {
}

/* Lays a sprite's quad flat in the xz plane around (x, y, z), rotated by angle. */
void func_1513E13C(QuadSprite *sprite, f32 x, f32 y, f32 z, f32 width, f32 length, u8 angle) {
    // matching: permuter-found shape; the one-element arrays, (&param)[0] reads, unused p0..p3 and the
    // empty if steer IDO's stack and register allocation (each plain form was tried and mismatches)
    f32 sinA;
    f32 cosA[1];
    f32 widthSin;
    f32 lengthCos;
    f32 lengthSin;
    f32 widthCos;
    f32 frontX;
    f32 t[1];
    f32 p0;
    f32 p1;
    f32 p2;
    f32 p3;
    f32 u[1];

    cosA[0] = func_151423D8(angle - 0x40);
    sinA = func_151423D8(angle);
    widthSin = (&width)[0] * sinA;
    frontX = (&x)[0] + widthSin;
    lengthCos = cosA[0] * (&length)[0];
    if (((!t) && (!t)) && (!t)) {
    }
    sprite->vtx[0].v.ob[0] = frontX - lengthCos;
    sprite->vtx[0].v.ob[1] = y;
    lengthSin = (&length)[0] * sinA;
    u[0] = z + lengthSin;
    widthCos = cosA[0] * (&width)[0];
    sprite->vtx[0].v.ob[2] = u[0] + widthCos;
    t[0] = x - widthSin;
    sprite->vtx[1].v.ob[0] = t[0] - lengthCos;
    sprite->vtx[1].v.ob[1] = y;
    sprite->vtx[1].v.ob[2] = u[0] - widthCos;
    sprite->vtx[2].v.ob[0] = t[0] + lengthCos;
    sprite->vtx[2].v.ob[1] = y;
    t[0] = z - lengthSin;
    sprite->vtx[2].v.ob[2] = t[0] - widthCos;
    sprite->vtx[3].v.ob[0] = frontX + lengthCos;
    sprite->vtx[3].v.ob[1] = y;
    sprite->vtx[3].v.ob[2] = t[0] + widthCos;
}

void func_1513E2A4(void) {
}

/*
 * Places a sprite's quad (half-size width x height, rotated by angle) around (x, y, z): in the plane of
 * the basis func_15144E80 returns for surface, or with no surface on the slope from func_1510E388.
 */
s32 func_1513E2AC(QuadSprite *sprite, Vec3s **triPtrs, CollisionTri *surface, f32 x, f32 y, f32 z, f32 width,
                  f32 height, u8 angle) {
    Vtx_t *vtx;
    f32 cosA;
    f32 sinA;
    f32 ux;
    f32 uy;
    f32 uz;
    f32 vx;
    f32 vy;
    f32 vz;
    Vec3f basisU;
    Vec3f basisV;
    f32 widthSin;
    f32 widthCos;
    f32 heightCos;
    f32 heightSin;
    f32 slopeX;
    f32 slopeZ;
    s16 i;
    f32 corners[4][2];
    f32 riseX;
    f32 riseZ;
    f32 scaleZ;
    f32 scaleX;
    f32 unused;
    Vtx_t *v;

    vtx = &sprite->vtx[0].v;
    cosA = func_151423D8(angle - 0x40);
    sinA = func_151423D8(angle);
    if (surface != NULL) {
        v = vtx;
        if (func_15144E80(surface, &basisU, &basisV, NULL) == 0) {
            return 0;
        }
        func_15145128(&basisU, &basisU, NULL, NULL);
        func_15145128(&basisV, &basisV, NULL, NULL);
        {
            f32 pad; // matching: -g3 scope

            widthSin = width * sinA;
            widthCos = width * cosA;
            ux = widthSin * basisU.x + widthCos * basisV.x;
            uy = widthSin * basisU.y + widthCos * basisV.y;
            uz = widthSin * basisU.z + widthCos * basisV.z;
            heightCos = -height * cosA;
            heightSin = height * sinA;
            vx = heightCos * basisU.x + heightSin * basisV.x;
            vy = heightCos * basisU.y + heightSin * basisV.y;
            vz = heightCos * basisU.z + heightSin * basisV.z;
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
    if (func_1510E388(triPtrs, surface->v, &slopeX, &slopeZ) == 0) {
        return 0;
    }
    heightSin = height * sinA;
    widthSin = width * sinA;
    heightCos = height * cosA;
    widthCos = width * cosA;
    corners[0][0] = widthSin - heightCos;
    corners[0][1] = heightSin + widthCos;
    corners[1][0] = -widthSin - heightCos;
    corners[1][1] = heightSin - widthCos;
    corners[2][0] = -widthSin + heightCos;
    corners[2][1] = -heightSin - widthCos;
    corners[3][0] = widthSin + heightCos;
    corners[3][1] = -heightSin + widthCos;
    for (i = 0; i < 4; i++) {
        f32 pad2; // matching: -g3 scope

        riseZ = corners[i][1] * slopeZ;
        // matching: dead stores the original made
        scaleZ = fabsf(corners[i][1]);
        scaleZ = fabsf(corners[i][1] * corners[i][1]);
        scaleZ = fabsf(riseZ); scaleZ = fabsf(riseZ - 1.0f);
        if (corners[i][0] != 0) {
            riseX = corners[i][0] * slopeX;
            scaleX = fabsf(corners[i][0]) / sqrtf(corners[i][0] * corners[i][0] + riseX * riseX);
        } else {
            riseX = 0.0f;
            scaleX = 0.0f;
        }
        if (corners[i][1] != 0) {
            scaleZ = fabsf(corners[i][1]) / sqrtf(corners[i][1] * corners[i][1] + riseZ * riseZ);
        } else {
            riseZ = 0.0f;
            scaleZ = 0.0f;
        }
        vtx->ob[0] = corners[i][0] * scaleX + x;
        vtx->ob[1] = scaleX * riseX + y + scaleZ * riseZ + 0.5f;
        vtx->ob[2] = corners[i][1] * scaleZ + z;
        vtx++;
    }
    return 1;
}

/* Moves, rotates and scales a particle by its drift velocities times the frame time, with optional gravity. */
s32 func_1513E6E8(DefParticleHead *particle) {
    DefParticleMotion *motion;

    motion = PARTICLE_MOTION(particle);
    if (particle->def.flags & 0x20) {
        motion->drift.vel.y += motion->drift.gravity * D_800BE9A4;
    }
    if (particle->def.flags & 0x40) {
        particle->def.pos.x += motion->drift.vel.x * D_800BE9A4;
        particle->def.pos.y += motion->drift.vel.y * D_800BE9A4;
        particle->def.pos.z += motion->drift.vel.z * D_800BE9A4;
    }
    if (particle->def.flags & 0x80) {
        particle->def.rot.x += motion->drift.rotVel.x * D_800BE9A4;
        particle->def.rot.y += motion->drift.rotVel.y * D_800BE9A4;
        particle->def.rot.z += motion->drift.rotVel.z * D_800BE9A4;
    }
    if (particle->def.flags & 0x100) {
        particle->def.scale.x += motion->drift.scaleVel.x * D_800BE9A4;
        particle->def.scale.y += motion->drift.scaleVel.y * D_800BE9A4;
        particle->def.scale.z += motion->drift.scaleVel.z * D_800BE9A4;
    }
    particle->def.unk45 = 0xFF;
    return 1;
}

/*
 * Vertex functions (D_80089E1C): each fetches the particle's vertex buffer for a viewport (refilled
 * from vtxTemplate when new), writes the quad's corner positions and returns it, or NULL.
 */

/* Quad of width x height, rotated by def.rot, scaled by def.scale and placed at def.pos. */
Vtx *func_1513E83C(DefParticleHead *particle, s16 viewport) {
    MtxF mtx;
    s32 pad; // matching: unused stack slot
    Vtx *vertices;
    Vtx *result;
    f32 corners[4][3];
    u8 fresh;
    s16 i;

    func_151D5D60(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    {
        s16 pad; // matching: -g3 scope; its .bgnb splits the &mtx init from the first load

        corners[0][0] = particle->def.width;
        corners[0][1] = particle->def.height;
        corners[1][0] = -particle->def.width;
        corners[1][1] = particle->def.height;
        corners[2][0] = -particle->def.width;
        corners[2][1] = -particle->def.height;
        corners[3][0] = particle->def.width;
        corners[3][1] = -particle->def.height;
        func_150A8050(mtx, particle->def.rot.x, particle->def.rot.y, particle->def.rot.z);
        mtx[3][0] = particle->def.pos.x;
        mtx[3][1] = particle->def.pos.y;
        mtx[3][2] = particle->def.pos.z;
        mtx[0][0] *= particle->def.scale.x;
        mtx[0][1] *= particle->def.scale.x;
        mtx[0][2] *= particle->def.scale.x;
        mtx[1][0] *= particle->def.scale.y;
        mtx[1][1] *= particle->def.scale.y;
        mtx[1][2] *= particle->def.scale.y;
        mtx[2][0] *= particle->def.scale.z;
        mtx[2][1] *= particle->def.scale.z;
        mtx[2][2] *= particle->def.scale.z;
        for (i = 0; i < 4; i++) {
            f32 *c = corners[i];

            func_150A7960(mtx, c[0], c[1], 0.0f, &c[0], &c[1], &c[2]);
            vertices->v.ob[0] = c[0];
            vertices->v.ob[1] = c[1];
            vertices->v.ob[2] = c[2];
            vertices->v.flag = 0;
            vertices++;
        }
    }
    return result;
}

/*
 * Upright quad of width x height facing the viewport's camera (D_800DD1D8 / D_800DD1E8 hold its
 * direction), scaled vertically by def.scale.y; reversed mirrors the corner order.
 */
Vtx *func_1513EAD8(particle, reversed, viewport)
    DefParticleHead *particle;
    u8 reversed;
    s16 viewport;
{
    Vtx *vertices;
    Vtx *result;
    s16 corners[4][3];
    f32 heightDelta, heightScale, offsetZ, offsetX;
    u8 fresh;

    func_151D5D60(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    offsetZ = D_800DD1D8[viewport] * particle->def.width;
    offsetX = D_800DD1E8[viewport] * particle->def.width;
    heightScale = particle->def.scale.y * particle->def.height;
    heightDelta = particle->def.height - heightScale;
    vertices[0].v.flag = 0;
    vertices[1].v.flag = 0;
    vertices[2].v.flag = 0;
    vertices[3].v.flag = 0;
    corners[3][0] = corners[0][0] = particle->def.pos.x + offsetX;
    corners[1][1] = corners[0][1] = particle->def.pos.y - heightScale;
    corners[3][2] = corners[0][2] = particle->def.pos.z - offsetZ;
    corners[2][0] = corners[1][0] = particle->def.pos.x - offsetX;
    corners[3][1] = corners[2][1] = particle->def.pos.y + heightDelta;
    corners[2][2] = corners[1][2] = particle->def.pos.z + offsetZ;
    if (reversed != 0) {
        vertices[0].v.ob[0] = corners[1][0];
        vertices[0].v.ob[1] = corners[1][1];
        vertices[0].v.ob[2] = corners[1][2];
        vertices[1].v.ob[0] = corners[2][0];
        vertices[1].v.ob[1] = corners[2][1];
        vertices[1].v.ob[2] = corners[2][2];
        vertices[2].v.ob[0] = corners[3][0];
        vertices[2].v.ob[1] = corners[3][1];
        vertices[2].v.ob[2] = corners[3][2];
        vertices[3].v.ob[0] = corners[0][0];
        vertices[3].v.ob[1] = corners[0][1];
        vertices[3].v.ob[2] = corners[0][2];
    } else {
        vertices[0].v.ob[0] = corners[0][0];
        vertices[0].v.ob[1] = corners[0][1];
        vertices[0].v.ob[2] = corners[0][2];
        vertices[1].v.ob[0] = corners[1][0];
        vertices[1].v.ob[1] = corners[1][1];
        vertices[1].v.ob[2] = corners[1][2];
        vertices[2].v.ob[0] = corners[2][0];
        vertices[2].v.ob[1] = corners[2][1];
        vertices[2].v.ob[2] = corners[2][2];
        vertices[3].v.ob[0] = corners[3][0];
        vertices[3].v.ob[1] = corners[3][1];
        vertices[3].v.ob[2] = corners[3][2];
    }
    return result;
}

Vtx *func_1513EDB4(void *particle, s16 viewport) {
    return func_1513EAD8(particle, FALSE, viewport);
}

Vtx *func_1513EDE4(void *particle, s16 viewport) {
    return func_1513EAD8(particle, TRUE, viewport);
}

/* Quad of width x height, rotated by def.rot and placed at def.pos (unrolled form of func_15140190). */
Vtx *func_1513EE14(DefParticleHead *particle, s16 viewport) {
    Vtx *vertices;
    Vtx *result;
    MtxF mtx;
    f32 corners[4][2];
    f32 out[4][3];
    u8 fresh;

    func_151D5D60(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    func_150A8050(mtx, particle->def.rot.x, particle->def.rot.y, particle->def.rot.z);
    mtx[3][0] = particle->def.pos.x;
    mtx[3][1] = particle->def.pos.y;
    mtx[3][2] = particle->def.pos.z;
    corners[0][0] = particle->def.width;
    corners[0][1] = particle->def.height;
    corners[1][0] = -particle->def.width;
    corners[1][1] = particle->def.height;
    corners[2][0] = -particle->def.width;
    corners[2][1] = -particle->def.height;
    corners[3][0] = particle->def.width;
    corners[3][1] = -particle->def.height;
    func_150A7960(mtx, corners[0][0], corners[0][1], 0.0f, &out[0][0], &out[0][1], &out[0][2]);
    func_150A7960(mtx, corners[1][0], corners[1][1], 0.0f, &out[1][0], &out[1][1], &out[1][2]);
    func_150A7960(mtx, corners[2][0], corners[2][1], 0.0f, &out[2][0], &out[2][1], &out[2][2]);
    func_150A7960(mtx, corners[3][0], corners[3][1], 0.0f, &out[3][0], &out[3][1], &out[3][2]);
    vertices->v.ob[0] = out[0][0];
    vertices->v.ob[1] = out[0][1];
    vertices->v.ob[2] = out[0][2];
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = out[1][0];
    vertices->v.ob[1] = out[1][1];
    vertices->v.ob[2] = out[1][2];
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = out[2][0];
    vertices->v.ob[1] = out[2][1];
    vertices->v.ob[2] = out[2][2];
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = out[3][0];
    vertices->v.ob[1] = out[3][1];
    vertices->v.ob[2] = out[3][2];
    vertices->v.flag = 0;
    return result;
}

/*
 * Camera-facing upright quad that bends towards the point def.rot: the bend angle grows with the
 * square root of that point's sideways offset (scaled by def.scale.x), capped at 90 degrees.
 */
Vtx *func_1513F114(DefParticleHead *particle, s16 viewport) {
    Vtx *vertices;
    Vtx *result;
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

    func_151D5D60(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    { // matching: this -g3 block scope steers the scheduler
        sinDir = D_800DD1D8[viewport];
        cosDir = D_800DD1E8[viewport];
        bottom = -(particle->def.scale.y * particle->def.height);
        top = particle->def.height + bottom;
        dist = (((particle->def.rot.x - particle->def.pos.x) * cosDir) -
                (sinDir * (particle->def.rot.z - particle->def.pos.z))) * D_800BE9A8;
        sign = -1.0f;
        if (dist < 0.0f) {
            sign = 1.0f;
            dist = -dist;
        }
        angle = sqrtf(dist) * particle->def.scale.x;
        if (angle > 1.57079637050628662109375f) {
            angle = 1.57079637050628662109375f;
        }
        angle *= sign;
        s = sinf(angle);
        c = cosf(angle);
        wc = particle->def.width * c;
        ws = particle->def.width * s;
        ts = top * s;
        tc = top * c;
        bs = bottom * s;
        bc = bottom * c;
        vertices[0].v.ob[0] = particle->def.pos.x + ((wc - bs) * cosDir);
        vertices[0].v.ob[1] = (particle->def.pos.y + bc) + ws;
        vertices[0].v.ob[2] = particle->def.pos.z - ((wc - bs) * sinDir);
        vertices[0].v.flag = 0;
        dist = -(wc + bs);
        vertices[1].v.ob[0] = particle->def.pos.x + (dist * cosDir);
        vertices[1].v.ob[1] = (particle->def.pos.y + bc) - ws;
        vertices[1].v.ob[2] = particle->def.pos.z - (dist * sinDir);
        vertices[1].v.flag = 0;
        vertices[2].v.ob[0] = particle->def.pos.x + ((-(wc + ts)) * cosDir);
        vertices[2].v.ob[1] = (particle->def.pos.y + tc) - ws;
        vertices[2].v.ob[2] = particle->def.pos.z - ((-(wc + ts)) * sinDir);
        vertices[2].v.flag = 0;
        dist = wc - ts;
        vertices[3].v.ob[0] = particle->def.pos.x + (dist * cosDir);
        vertices[3].v.ob[1] = (particle->def.pos.y + tc) + ws;
        vertices[3].v.ob[2] = particle->def.pos.z - (dist * sinDir);
        vertices[3].v.flag = 0;
    }
    return result;
}

/* Quad spanned by the two axes in motion.axes (vertex function 12). */
void func_1513F4B0(DefParticleHead *head, s16 viewport) {
    DefParticleHead *particle = head;

    func_15140410(particle, &PARTICLE_MOTION(particle)->axes.u, &PARTICLE_MOTION(particle)->axes.v, viewport);
}

/*
 * Combiner modes: each 16-byte entry holds the a, b, c, d inputs of cycle 0 colour, cycle 0 alpha,
 * cycle 1 colour and cycle 1 alpha, in that order.
 */
static const u8 D_800A4BA8[92][16] = {
    { 1, 31, 5, 3, 1, 7, 5, 3, 1, 31, 5, 3, 1, 7, 5, 3 },
    { 1, 3, 5, 31, 1, 3, 5, 7, 1, 3, 5, 31, 1, 3, 5, 7 },
    { 1, 31, 5, 3, 1, 7, 5, 3, 1, 31, 5, 3, 1, 7, 5, 3 },
    { 1, 31, 5, 3, 1, 7, 5, 7, 1, 31, 5, 3, 1, 7, 5, 7 },
    { 31, 31, 5, 3, 1, 7, 5, 7, 31, 31, 5, 3, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 4, 7, 1, 31, 5, 31, 1, 7, 4, 7 },
    { 1, 31, 5, 31, 1, 7, 4, 7, 1, 31, 5, 31, 1, 7, 4, 7 },
    { 1, 31, 5, 31, 1, 7, 4, 7, 1, 31, 5, 31, 1, 7, 4, 7 },
    { 1, 31, 5, 3, 1, 7, 5, 7, 1, 31, 5, 3, 1, 7, 5, 7 },
    { 1, 31, 5, 3, 1, 7, 5, 7, 31, 31, 31, 0, 7, 7, 7, 0 },
    { 1, 3, 5, 31, 1, 7, 5, 7, 31, 31, 31, 0, 7, 7, 7, 0 },
    { 1, 31, 5, 31, 1, 7, 4, 7, 1, 31, 5, 31, 1, 7, 4, 7 },
    { 31, 31, 31, 1, 1, 7, 3, 7, 31, 31, 31, 1, 1, 7, 3, 7 },
    { 3, 5, 1, 5, 3, 7, 1, 7, 3, 5, 1, 5, 3, 7, 1, 7 },
    { 3, 5, 8, 5, 3, 7, 1, 7, 3, 5, 8, 5, 3, 7, 1, 7 },
    { 3, 5, 1, 5, 3, 7, 2, 7, 31, 31, 31, 0, 7, 7, 7, 0 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 1, 31, 3, 31, 1, 7, 3, 7, 1, 31, 3, 31, 1, 7, 3, 7 },
    { 1, 31, 2, 31, 1, 7, 2, 7, 0, 31, 3, 31, 0, 7, 3, 7 },
    { 31, 31, 31, 1, 7, 7, 7, 1, 31, 31, 31, 1, 7, 7, 7, 1 },
    { 31, 31, 31, 1, 1, 7, 3, 7, 31, 31, 31, 1, 1, 7, 3, 7 },
    { 7, 31, 3, 31, 7, 7, 7, 3, 7, 31, 3, 31, 7, 7, 7, 3 },
    { 31, 31, 31, 4, 4, 7, 5, 7, 31, 31, 31, 4, 4, 7, 5, 7 },
    { 1, 31, 5, 31, 7, 7, 7, 1, 1, 31, 5, 31, 7, 7, 7, 1 },
    { 31, 31, 31, 4, 7, 7, 7, 5, 31, 31, 31, 4, 7, 7, 7, 5 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 1, 7, 4, 7, 1, 31, 4, 31, 1, 7, 4, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 3, 31, 1, 7, 5, 7, 1, 31, 3, 31, 1, 7, 5, 7 },
    { 1, 5, 15, 3, 3, 7, 1, 7, 1, 5, 15, 3, 3, 7, 1, 7 },
    { 1, 31, 5, 31, 1, 7, 3, 7, 1, 31, 5, 31, 1, 7, 3, 7 },
    { 3, 5, 8, 5, 3, 7, 1, 7, 3, 5, 8, 5, 3, 7, 1, 7 },
    { 2, 1, 13, 1, 2, 1, 0, 1, 4, 31, 0, 31, 0, 7, 4, 7 },
    { 31, 31, 31, 1, 1, 7, 5, 7, 31, 31, 31, 1, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 1, 7, 4, 7, 31, 31, 31, 1, 1, 7, 4, 7 },
    { 31, 31, 31, 3, 1, 7, 4, 7, 31, 31, 31, 3, 1, 7, 4, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 3, 7, 1, 31, 5, 31, 1, 7, 3, 7 },
    { 1, 31, 5, 31, 1, 7, 3, 7, 1, 31, 5, 31, 1, 7, 3, 7 },
    { 1, 31, 5, 31, 1, 7, 3, 7, 1, 31, 5, 31, 1, 7, 3, 7 },
    { 31, 31, 31, 1, 7, 7, 7, 1, 31, 31, 31, 1, 7, 7, 7, 1 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 7, 7, 7, 1, 31, 31, 31, 1, 7, 7, 7, 1 },
    { 1, 31, 5, 31, 7, 7, 7, 1, 6, 5, 3, 0, 7, 7, 7, 0 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 0, 31, 4, 31, 7, 7, 7, 0 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 31, 31, 31, 6, 7, 7, 7, 6, 31, 31, 31, 6, 7, 7, 7, 6 },
    { 31, 31, 31, 4, 7, 7, 7, 4, 31, 31, 31, 4, 7, 7, 7, 4 },
    { 1, 31, 4, 31, 1, 7, 4, 7, 1, 31, 4, 31, 1, 7, 4, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 4, 7, 5, 7, 1, 31, 5, 31, 4, 7, 5, 7 },
    { 31, 31, 31, 4, 7, 7, 7, 4, 31, 31, 31, 4, 7, 7, 7, 4 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 1, 7, 5, 7, 31, 31, 31, 1, 1, 7, 5, 7 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 1, 7, 5, 7, 31, 31, 31, 1, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 7, 7, 7, 1, 31, 31, 31, 1, 7, 7, 7, 1 },
    { 1, 31, 5, 31, 1, 7, 5, 7, 1, 31, 5, 31, 1, 7, 5, 7 },
    { 31, 31, 31, 1, 1, 7, 4, 7, 31, 31, 31, 1, 1, 7, 4, 7 },
    { 1, 31, 4, 31, 1, 7, 5, 7, 1, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 7, 7, 7, 6, 1, 31, 4, 31, 7, 7, 7, 6 },
    { 5, 31, 4, 31, 1, 7, 5, 7, 5, 31, 4, 31, 1, 7, 5, 7 },
    { 1, 31, 4, 31, 7, 7, 7, 1, 1, 31, 4, 31, 7, 7, 7, 1 },
    { 31, 31, 31, 4, 4, 7, 5, 7, 31, 31, 31, 4, 4, 7, 5, 7 },
    { 31, 31, 31, 1, 1, 7, 3, 7, 31, 31, 31, 1, 1, 7, 3, 7 },
    { 31, 31, 31, 1, 1, 7, 4, 7, 31, 31, 31, 1, 1, 7, 4, 7 },
    { 1, 31, 5, 31, 1, 7, 4, 7, 1, 31, 5, 31, 1, 7, 4, 7 },
    { 1, 31, 4, 31, 1, 7, 4, 7, 1, 31, 4, 31, 1, 7, 4, 7 },
    { 31, 31, 31, 6, 7, 7, 7, 4, 31, 31, 31, 6, 7, 7, 7, 4 },
    { 1, 31, 3, 31, 1, 7, 3, 7, 6, 1, 5, 0, 7, 7, 7, 0 },
    { 3, 5, 1, 5, 1, 7, 5, 7, 3, 5, 1, 5, 1, 7, 5, 7 },
    { 1, 3, 5, 3, 1, 7, 3, 7, 1, 3, 5, 3, 1, 7, 3, 7 },
    { 31, 31, 31, 1, 7, 7, 7, 5, 31, 31, 31, 1, 7, 7, 7, 5 },
    { 31, 31, 31, 5, 7, 7, 7, 5, 31, 31, 31, 5, 7, 7, 7, 5 },
    { 31, 31, 31, 6, 1, 7, 5, 7, 31, 31, 31, 6, 1, 7, 5, 7 },
    { 31, 31, 31, 6, 1, 7, 4, 7, 31, 31, 31, 6, 1, 7, 4, 7 },
    { 31, 31, 31, 5, 1, 7, 4, 7, 31, 31, 31, 5, 1, 7, 4, 7 },
    { 1, 31, 5, 31, 5, 7, 4, 7, 1, 31, 5, 31, 5, 7, 4, 7 },
    { 31, 31, 31, 5, 5, 7, 4, 7, 31, 31, 31, 5, 5, 7, 4, 7 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 5, 1, 7, 5, 7 },
    { 31, 31, 31, 5, 1, 7, 5, 7, 31, 31, 31, 0, 0, 7, 4, 7 },
};

/* Sets the colour combiner to mode combiner unless it is already current. */
Gfx *func_1513F4E4(Gfx *gfx, u8 combiner, u8 *needSync) {
    s32 key;
    const u8 *mode;

    key = func_15143044(combiner, 0);
    if (key != D_800DD1BE) {
        if (*needSync == TRUE) {
            gDPPipeSync(gfx++);
            *needSync = FALSE;
        }
        mode = D_800A4BA8[combiner];
        gDPSetCombine(gfx++,
                      GCCc0w0(mode[0], mode[2], mode[4], mode[6]) | GCCc1w0(mode[8], mode[10]),
                      GCCc0w1(mode[1], mode[3], mode[5], mode[7]) |
                          GCCc1w1(mode[9], mode[12], mode[14], mode[11], mode[13], mode[15]));
        D_800DD1BE = key;
    }
    return gfx;
}

/* Sets a particle's update, expiry and vertex function indices. */
void func_1513F680(DefParticleHead *particle, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn) {
    particle->updateFn1 = updateFn1;
    particle->updateFn2 = updateFn2;
    particle->expireFn = expireFn;
    particle->vertexFn = vertexFn;
}

/* Sets a sprite's update and expiry function indices. */
void func_1513F6C0(QuadSprite *sprite, u8 updateFn, u8 expireFn) {
    sprite->updateFn = updateFn;
    sprite->expireFn = expireFn;
}

/* Grows a particle's width and height by motion.grow.rate times the frame time. */
s32 func_1513F6E8(DefParticleHead *particle) {
    f32 rate = PARTICLE_MOTION(particle)->grow.rate;

    particle->def.width += rate * D_800BE9A4;
    particle->def.height += rate * D_800BE9A4;
    return 1;
}

/*
 * Ballistic motion: applies drag once per elapsed tick and gravity, then moves the particle by the
 * average of its old and new velocity; also spins it by motion.ballistic.rotVel.
 */
s32 func_1513F728(DefParticleHead *particle) {
    DefParticleMotion *motion;
    Vec3f oldVel;
    s32 i;

    motion = PARTICLE_MOTION(particle);
    oldVel = motion->ballistic.vel;
    for (i = D_800BE9E4; i != 0; i--) {
        motion->ballistic.vel.x *= motion->ballistic.drag;
        motion->ballistic.vel.y *= motion->ballistic.drag;
        motion->ballistic.vel.z *= motion->ballistic.drag;
    }
    {
        f32 ax;
        f32 ay;
        f32 az;

        motion->ballistic.vel.y += motion->ballistic.gravity * D_800BE9A4;
        ax = (motion->ballistic.vel.x - oldVel.x) * D_800BE9A8;
        ay = (motion->ballistic.vel.y - oldVel.y) * D_800BE9A8;
        az = (motion->ballistic.vel.z - oldVel.z) * D_800BE9A8;
        particle->def.pos.x += (oldVel.x + 0.5f * ax * D_800BE9A4) * D_800BE9A4;
        particle->def.pos.y += (oldVel.y + 0.5f * ay * D_800BE9A4) * D_800BE9A4;
        particle->def.pos.z += (oldVel.z + 0.5f * az * D_800BE9A4) * D_800BE9A4;
    }
    particle->def.rot.x += motion->ballistic.rotVel.x * D_800BE9A4;
    particle->def.rot.y += motion->ballistic.rotVel.y * D_800BE9A4;
    particle->def.rot.z += motion->ballistic.rotVel.z * D_800BE9A4;
    return 1;
}

void func_1513FA2C(DefParticleHead *particle, s16 viewport) {
    BeamScale scale;

    scale.width = 1.0f;
    scale.length = 1.0f;
    func_1513FAB4(particle, FALSE, &scale, viewport);
}

void func_1513FA70(DefParticleHead *particle, s16 viewport) {
    BeamScale scale;

    scale.width = 1.0f;
    scale.length = 1.0f;
    func_1513FAB4(particle, TRUE, &scale, viewport);
}

/*
 * Beam from def.pos towards def.rot (its length scaled by def.height * scale->length), def.width *
 * scale->width wide and turned to face the viewport's camera; reversed rotates the corner order.
 */
Vtx *func_1513FAB4(DefParticleHead *particle, u8 reversed, BeamScale *scale, s16 viewport) {
    Vtx *vertices;
    Vtx *result;
    Vec3f *camera;
    Vec3f delta;
    Vec3f end;
    Vec3f middle;
    Vec3f view;
    Vec3f side;
    f32 lengthSquared;
    f32 length;
    f32 width;
    u8 fresh;

    func_151D5D60(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    camera = func_15144B34(viewport);
    width = particle->def.width * scale->width;
    length = particle->def.height * scale->length;
    delta.x = particle->def.rot.x - particle->def.pos.x;
    delta.y = particle->def.rot.y - particle->def.pos.y;
    delta.z = particle->def.rot.z - particle->def.pos.z;
    delta.x *= length;
    delta.y *= length;
    delta.z *= length;
    end.x = particle->def.pos.x + delta.x;
    end.y = particle->def.pos.y + delta.y;
    end.z = particle->def.pos.z + delta.z;
    middle.x = particle->def.pos.x + delta.x * 0.5f;
    middle.y = particle->def.pos.y + delta.y * 0.5f;
    middle.z = particle->def.pos.z + delta.z * 0.5f;
    view.x = middle.x - camera->x;
    view.y = middle.y - camera->y;
    view.z = middle.z - camera->z;
    func_150AD8B0(&delta.x, &view.x, &side.x);
    lengthSquared = side.x * side.x + side.y * side.y + side.z * side.z;
    if (lengthSquared != 0.0f) {
        f32 sideScale = width / sqrtf(lengthSquared);

        side.x = side.x * sideScale;
        side.y = side.y * sideScale;
        side.z = side.z * sideScale;
    } else {
        side.x = 0;
        side.y = 0;
        side.z = 0;
    }
    if (reversed != 0) {
        vertices->v.ob[0] = particle->def.pos.x + side.x;
        vertices->v.ob[1] = particle->def.pos.y + side.y;
        vertices->v.ob[2] = particle->def.pos.z + side.z;
        vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = particle->def.pos.x - side.x;
        vertices->v.ob[1] = particle->def.pos.y - side.y;
        vertices->v.ob[2] = particle->def.pos.z - side.z;
        vertices->v.flag = 0;
        // matching: IDO ugen reads stale tree-node memory left by earlier functions in this file;
        // these empty statements pad the node count so that state lines up
        ;
        vertices++;
        vertices->v.ob[0] = end.x - side.x;
        vertices->v.ob[1] = end.y - side.y;
        vertices->v.ob[2] = end.z - side.z;
        vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = end.x + side.x;
        vertices->v.ob[1] = end.y + side.y;
        vertices->v.ob[2] = end.z + side.z;
        vertices->v.flag = 0;
    } else {
        vertices->v.ob[0] = particle->def.pos.x - side.x;
        vertices->v.ob[1] = particle->def.pos.y - side.y;
        vertices->v.ob[2] = particle->def.pos.z - side.z;
        vertices->v.flag = 0;
        ;;;;;;;;
        vertices++;
        vertices->v.ob[0] = end.x - side.x;
        vertices->v.ob[1] = end.y - side.y;
        vertices->v.ob[2] = end.z - side.z;
        vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = end.x + side.x;
        vertices->v.ob[1] = end.y + side.y;
        vertices->v.ob[2] = end.z + side.z;
        vertices->v.flag = 0;
        vertices++;
        vertices->v.ob[0] = particle->def.pos.x + side.x;
        vertices->v.ob[1] = particle->def.pos.y + side.y;
        vertices->v.ob[2] = particle->def.pos.z + side.z;
        vertices->v.flag = 0;
    }
    return result;
}

/* Sets the texture coordinates of a quad to cover the whole texture, mirrored per the flip bits. */
void func_1513FFF4(Vtx *vtx, u8 texture, u8 flip) {
    TextureDesc *tex;
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
void func_151400D0(Vtx *vtx, const VtxColor *colors) {
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

/* Quad of width x height, rotated by def.rot and placed at def.pos. */
Vtx *func_15140190(DefParticleHead *particle, s16 viewport) {
    Vtx *vertices;
    Vtx *result;
    MtxF mtx;
    f32 corners[4][3];
    u8 i;
    u8 fresh;

    func_151D5D60(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh != 0) {
            memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
            memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
        }
    } else {
        return NULL;
    }
    {
        s16 pad; // matching: -g3 scope

        corners[0][0] = particle->def.width;
        corners[0][1] = particle->def.height;
        corners[0][2] = 0.0f;
        corners[1][0] = -particle->def.width;
        corners[1][1] = particle->def.height;
        corners[1][2] = 0.0f;
        corners[2][0] = -particle->def.width;
        corners[2][1] = -particle->def.height;
        corners[2][2] = 0.0f;
        corners[3][0] = particle->def.width;
        corners[3][1] = -particle->def.height;
        corners[3][2] = 0.0f;
        func_150A8050(mtx, particle->def.rot.x, particle->def.rot.y, particle->def.rot.z);
        mtx[3][0] = particle->def.pos.x;
        mtx[3][1] = particle->def.pos.y;
        mtx[3][2] = particle->def.pos.z;
        for (i = 0; i < 4; i++) {
            f32 *c = corners[i];

            func_150A7960(mtx, c[0], c[1], 0.0f, &c[0], &c[1], &c[2]);
            vertices->v.ob[0] = c[0];
            vertices->v.ob[1] = c[1];
            vertices->v.ob[2] = c[2];
            vertices->v.flag = 0;
            vertices++;
        }
    }
    return result;
}

/* The object list types of the glow sprites (game_16DC80) and of the effect objects, for func_15169260. */
const s32 D_800A5168[4] = { 0x1C, 0x49, 0x56, 0x62 };
static const s32 D_800A5178[3] = { 0x1B, 0x61, 0x4A };

void func_151403A8(s32 arg0, u8 arg1) {
    func_15169260(D_800A5168, 4, arg0, arg1);
}

void func_151403DC(s32 arg0, u8 arg1) {
    func_15169260(D_800A5178, 3, arg0, arg1);
}

/* Quad centred on def.pos with half-axes axisU * width and axisV * height. */
Vtx *func_15140410(DefParticleHead *particle, Vec3f *axisU, Vec3f *axisV, s16 viewport) {
    Vtx *vertices;
    Vtx *result;
    f32 ux, uy, uz, vx, vy, vz;

    {
        u8 fresh;

        func_151D5D60(particle->vtxBuffers, viewport, sizeof(particle->vtxTemplate), &vertices, &fresh);
        result = vertices;
        if (vertices != NULL) {
            if (fresh != 0) {
                memcpy(particle->vtxBuffers[viewport], particle->vtxTemplate, sizeof(particle->vtxTemplate));
                memcpy(particle->vtxBuffers[viewport] + 4, particle->vtxTemplate, sizeof(particle->vtxTemplate));
            }
        } else {
            return NULL;
        }
    }
    ux = axisU->x * particle->def.width;
    uy = axisU->y * particle->def.width;
    uz = axisU->z * particle->def.width;
    vx = axisV->x * particle->def.height;
    vy = axisV->y * particle->def.height;
    vz = axisV->z * particle->def.height;
    vertices[0].v.ob[0] = particle->def.pos.x + ux + vx;
    vertices[0].v.ob[1] = particle->def.pos.y + uy + vy;
    vertices[0].v.ob[2] = particle->def.pos.z + uz + vz;
    vertices[0].v.flag = 0;
    vertices[1].v.ob[0] = particle->def.pos.x - ux + vx;
    vertices[1].v.ob[1] = particle->def.pos.y - uy + vy;
    vertices[1].v.ob[2] = particle->def.pos.z - uz + vz;
    vertices[1].v.flag = 0;
    vertices[2].v.ob[0] = particle->def.pos.x - ux - vx;
    vertices[2].v.ob[1] = particle->def.pos.y - uy - vy;
    vertices[2].v.ob[2] = particle->def.pos.z - uz - vz;
    vertices[2].v.flag = 0;
    vertices[3].v.ob[0] = particle->def.pos.x + ux - vx;
    vertices[3].v.ob[1] = particle->def.pos.y + uy - vy;
    vertices[3].v.ob[2] = particle->def.pos.z + uz - vz;
    vertices[3].v.flag = 0;
    return result;
}

/* Sort key: texture, then animation frame, so that particles sharing a texture load draw together. */
#define PARTICLE_SORT_KEY(p) (((p)->def.texture << 8) + ((p)->def.frame >> 16))

/* List callback: insertion-sorts draw list D_800DCE50[group][list] by PARTICLE_SORT_KEY. */
s32 func_151406AC(s32 arg0, s32 list, s32 group, s16 phase) {
    DefParticleHead head;
    DefParticleHead **slot;
    DefParticleHead *cur;
    DefParticleHead *prev;
    DefParticleHead *next;
    DefParticleHead *node;
    s32 key;
    s32 nodeKey;

    if (phase != 0) {
        return arg0;
    }
    slot = (DefParticleHead **)&D_800DCE50[group][list];
    node = *slot;
    head.def.texture = 0;
    head.def.frame = 0;
    head.header.next = &node->header;
    /* BUG (original game): dereferences the list head before the NULL check below
       (and again after the sort); an empty list writes to address 4. */
    node->header.prev = &head.header;
    if (node != NULL) {
        for (cur = (DefParticleHead *)node->header.next; cur != NULL; cur = next) {
            prev = (DefParticleHead *)cur->header.prev;
            next = (DefParticleHead *)cur->header.next;
            key = PARTICLE_SORT_KEY(cur);
            for (node = prev; node != NULL; node = (DefParticleHead *)node->header.prev) {
                nodeKey = PARTICLE_SORT_KEY(node);
                if (key < nodeKey) {
                    continue;
                }
                if (node != prev) {
                    prev->header.next = cur->header.next;
                    if (cur->header.next != NULL) {
                        cur->header.next->prev = cur->header.prev;
                    }
                    cur->header.next = node->header.next;
                    if (cur->header.next != NULL) {
                        cur->header.next->prev = &cur->header;
                    }
                    cur->header.prev = &node->header;
                    node->header.next = &cur->header;
                }
                break;
            }
        }
    }
    *slot = (DefParticleHead *)head.header.next;
    head.header.next->prev = NULL;
    return arg0;
}
