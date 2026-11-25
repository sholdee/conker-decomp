#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"
#include "libc/string.h"

extern f32 sqrtf(f32);

typedef struct ParticleViewMask {
    s16 unk0;
    s16 views;
} ParticleViewMask;

typedef struct ParticleAttach {
    s32 unk0[7];
    u8 unk1C;
    u8 pad1D[3];
    s32 unk20;
} ParticleAttach; /* size 0x24 */

/* Per-particle data for the floor-collision update funcs (func_151319C4). */
typedef struct ParticleBounce {
    u8 flags;
    s8 hitFunc;                /* D_80089874 index, -1 for none */
    u8 pad2[2];
    f32 restitution;
} ParticleBounce;

/* Per-particle data for func_15131DEC (spawned by func_15131EE4). */
typedef struct ParticleFlash {
    f32 elapsed;
    f32 duration;
    f32 growth;
    f32 startAlpha;
    f32 alphaDecay;
} ParticleFlash;

typedef struct Particle {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ ParticleDesc desc;
    /* 0x80 */ union {
        ParticleAttach attach;
        CollisionHit floor;
    } unk80;
    /* 0xA4 */ u8 padA4[4];
    /* 0xA8 */ union {              /* caller-sized, see func_15130280 */
        f32 drag;
        ParticleBounce bounce;
        struct {
            f32 drag;
            u8 pad4[4];
            ParticleBounce bounce;
        } dragBounce;
        ParticlePulse pulse;
        ParticleFlash flash;
    } data;
} Particle;


extern IdList3 D_800A37F0;


/* Allocates a particle of the given kind with dataSize bytes of per-particle data and copies desc into it. */
Particle *func_15130280(ParticleDesc *desc, u8 kind, ParticleAttach *attach, s32 dataSize, u8 arg4, s32 arg5) {
    Particle *particle;
    s32 type;

    switch (kind) {
    case 0:
        type = 0x2B;
        break;
    case 1:
        type = 0x52;
        break;
    case 2:
        type = 0x47;
        break;
    default:
        type = 0x2B;
        break;
    }

    particle = func_15167A68(type, arg5, dataSize + 0xA8, 1, arg4, 1);
    if (particle == NULL) {
        return NULL;
    }

    memcpy(&particle->desc, desc, sizeof(ParticleDesc));

    if (attach != NULL) {
        particle->unk80.attach = *attach;
    } else {
        particle->unk80.attach.unk1C = 0;
    }

    return particle;
}

void func_15130374(ParticleDesc *desc, u8 kind, s32 dataSize, u8 arg3, s32 arg4) {
    func_15130280(desc, kind, NULL, dataSize, arg3, arg4);
}

void func_151303BC(ParticleDesc *desc, u8 kind, s32 dataSize) {
    func_15130374(desc, kind, dataSize, 0xFF, 0);
}

extern EffectTexture D_80090B60[];
extern s32 (*D_80089760[])(Particle *, Vec3f *);
extern void (*D_80089800[])(Particle *);

/* Per-frame particle update: lifetime, motion, fade, scaling, animation and the update/death callbacks. */
void func_151303EC(Particle *p) {
    s32 pad; // matching: unused stack slot
    Vec3f prevPos;
    u8 dead;
    s32 i;
    s32 maxFrame;
    s32 rate;
    s32 frameCount;

    dead = FALSE;
    if (p->desc.flags & PARTICLE_LIFETIME) {
        p->desc.timer -= D_800BE9E4;
        if (p->desc.timer < 0) {
            dead = TRUE;
        }
    }
    if (p->desc.flags & PARTICLE_GRAVITY) {
        p->desc.vel.y += p->desc.gravity * D_800BE9A4;
    }
    prevPos = p->desc.pos;
    if (p->desc.flags & PARTICLE_MOVE) {
        p->desc.pos.x += p->desc.vel.x * D_800BE9A4;
        p->desc.pos.y += p->desc.vel.y * D_800BE9A4;
        p->desc.pos.z += p->desc.vel.z * D_800BE9A4;
    }
    if (p->desc.flags & PARTICLE_FADE) {
        if (p->desc.timer < p->desc.fadeStart) {
            i = (s16)(p->desc.timer * p->desc.fadeRate); // matching: reuses i; a separate local reallocates
            if (i < p->desc.alpha) {
                p->desc.alpha = i;
            }
        }
    }
    if ((p->desc.flags & PARTICLE_SCALE) && (p->desc.timer < p->desc.scaleStart)) {
        if (p->desc.flags & PARTICLE_SCALE_MUL) {
            i = D_800BE9E4;
            if (p->desc.flags & PARTICLE_SCALE_WIDTH_ONLY) {
                for (; i != 0; i--) {
                    p->desc.width *= p->desc.scaleRate;
                }
            } else if (p->desc.flags & PARTICLE_SCALE_HEIGHT_ONLY) {
                for (; i != 0; i--) {
                    p->desc.height *= p->desc.scaleRate;
                }
            } else {
                for (; i != 0; i--) {
                    p->desc.width *= p->desc.scaleRate;
                    p->desc.height *= p->desc.scaleRate;
                }
            }
        } else {
            if (!(p->desc.flags & PARTICLE_SCALE_WIDTH_ONLY)) {
                p->desc.height += p->desc.scaleRate * D_800BE9E4;
            }
            if (!(p->desc.flags & PARTICLE_SCALE_HEIGHT_ONLY)) {
                p->desc.width += p->desc.scaleRate * D_800BE9E4;
            }
        }
    }
    if (!dead) {
        rate = p->desc.frameRate;
        if (rate != 0) {
            frameCount = D_80090B60[p->desc.texture].frameCount;
            maxFrame = (frameCount << 16) - 1;
            p->desc.frame += rate * D_800BE9E4;
            if (maxFrame < p->desc.frame) {
                if (p->desc.flags & PARTICLE_ANIM_END_KILLS) {
                    dead = TRUE;
                } else if (p->desc.flags & PARTICLE_ANIM_PINGPONG) {
                    p->desc.frame = maxFrame - p->desc.frame % maxFrame;
                    p->desc.frameRate = -rate;
                } else {
                    do {
                        p->desc.frame -= maxFrame;
                    } while (maxFrame < p->desc.frame);
                }
            } else if (p->desc.frame < 0) {
                if (p->desc.flags & PARTICLE_ANIM_START_KILLS) {
                    dead = TRUE;
                } else if (p->desc.flags & PARTICLE_ANIM_PINGPONG) {
                    p->desc.frame = -p->desc.frame % maxFrame;
                    p->desc.frameRate = -rate;
                } else {
                    do {
                        p->desc.frame += maxFrame;
                    } while (p->desc.frame < 0);
                }
            }
        }
    }
    if (p->desc.updateFunc != -1 && !dead) {
        if (D_80089760[p->desc.updateFunc](p, &prevPos) == 0) {
            dead = TRUE;
        }
    }
    if (dead) {
        if (p->desc.deathFunc != -1) {
            D_80089800[p->desc.deathFunc](p);
        }
        func_1516972C(&p->header);
    }
}

typedef struct ParticleRenderMode {
    s32 unk0;
    s32 unk4;
} ParticleRenderMode;

extern s32 (*D_80089804[])(Particle *, s16);
extern ParticleRenderMode D_800A4AC8[];
s32 func_15144CEC(Vec3f *, f32 *, f32 *, f32 *, f32 *, s32);
void func_1513137C(s16 *r, s16 *g, s16 *b, s16 *a, Particle *p);
void func_1513164C(s16 *primR, s16 *primG, s16 *primB, s16 *primA, s16 *envR, s16 *envG, s16 *envB, s16 *envA, Particle *p);
void func_1515E888(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5, u8 *arg6, u8 *arg7);
Gfx *func_15142E24(Gfx *gfx, EffectTexture *tex, s32 frame, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *needSync, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_15142CF0(Gfx *gfx, s32 arg1, s32 arg2, s32 r, s32 g, s32 b, s32 a, u8 *needSync);
Gfx *func_1513F4E4(Gfx *gfx, u8 arg1, u8 *needSync);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *needSync);

/* Draws a particle as a screen-space textured rectangle in the given view. */
Gfx *func_15130A9C(Gfx *gfx, Particle *p, s16 viewIdx) {
    s32 pad; // matching: unused stack slot
    Vec3f pos;
    u8 needSync;
    u8 mode;
    u8 shade;
    s16 primR;
    s16 primG;
    s16 primB;
    s16 primA;
    s16 envR;
    s16 envG;
    s16 envB;
    s16 envA;
    f32 screen[2];
    f32 depth;
    f32 w;
    f32 scale;
    f32 sizeX;
    f32 sizeY;
    f32 ulx;
    f32 uly;
    f32 lrx;
    f32 lry;

    if ((p->desc.flags & PARTICLE_VIEWS) && !(p->desc.views & (1 << viewIdx))) {
        return gfx;
    }
    needSync = TRUE;
    mode = p->desc.renderMode & 0xFF;
    shade = (p->desc.renderMode & 0xFF00) >> 8;
    if (p->desc.flags & PARTICLE_DRAW_FUNC) {
        if (p->desc.drawFunc != -1 && D_80089804[p->desc.drawFunc](p, viewIdx) == 0) {
            return gfx;
        }
    }
    if (p->desc.flags & PARTICLE_VIEW_MASK) {
        if (p->desc.viewMask != NULL && !(p->desc.viewMask->views & (1 << viewIdx))) {
            return gfx;
        }
    }
    if (p->desc.flags & PARTICLE_USE_OFFSET) {
        pos.x = p->desc.pos.x + p->desc.offset.x;
        pos.y = p->desc.pos.y + p->desc.offset.y;
        pos.z = p->desc.pos.z + p->desc.offset.z;
    } else {
        pos = p->desc.pos;
    }
    if (func_15144CEC(&pos, screen, &depth, &w, &scale, viewIdx)) {
        RenderView *view;
        EffectTexture *tex;
        f32 texW;
        f32 texH;
        f32 height;
        s32 s;
        s32 t;
        s32 dsdx;

        // matching: byte-offset form; &D_800BE628[viewIdx] schedules differently
        view = &D_800BE628[viewIdx];
        sizeX = p->desc.width * scale * view->unk1C * 16.94900131f;
        sizeY = p->desc.height * scale * view->unk20 * 16.94900131f;
        if (p->desc.flags & PARTICLE_MAX_SIZE) {
            if (p->desc.maxSize < sizeX) {
                sizeX = p->desc.maxSize;
            }
            if (p->desc.maxSize < sizeY) {
                sizeY = p->desc.maxSize;
            }
        }
        if (p->desc.flags & PARTICLE_MIN_SIZE) {
            if (sizeX < p->desc.minSize) {
                sizeX = p->desc.minSize;
            }
            if (sizeY < p->desc.minSize) {
                sizeY = p->desc.minSize;
            }
        }
        ulx = screen[0] - sizeX;
        lrx = screen[0] + sizeX;
        uly = screen[1] - sizeY;
        lry = screen[1] + sizeY;
        if (view->left <= lrx && ulx <= view->right && view->top <= lry && uly <= view->bottom) {
            s32 dtdy;

            ulx *= 4.0f;
            uly *= 4.0f;
            lrx *= 4.0f;
            lry *= 4.0f;
            if (needSync) {
                gDPPipeSync(gfx++);
                needSync = FALSE;
            }
            gDPSetPrimDepth(gfx++, (s32)((depth * scale * D_800BE628[viewIdx].viewports[D_800BE9C0].vp.vscale[2] + D_800BE628[viewIdx].viewports[D_800BE9C0].vp.vtrans[2]) * 32.0f), 0);
            tex = &D_80090B60[p->desc.texture];
            texW = tex->width;
            texH = tex->height;
            height = lry - uly;
            if (ulx < lrx) {
                dsdx = (4096.0f * texW) / (lrx - ulx);
            } else {
                dsdx = 0;
            }
            if (height > 0.0f) {
                dtdy = (4096.0f * texH) / height;
            } else {
                dtdy = 0;
            }
            s = 0;
            t = 0;
            if (p->desc.flags & PARTICLE_FLIP_S) {
                s = (tex->width - 1) << 5;
                dsdx = -dsdx & 0xFFFF;
            }
            if (p->desc.flags & PARTICLE_FLIP_T) {
                t = (tex->height - 1) << 5;
                dtdy = -dtdy & 0xFFFF;
            }
            if (p->desc.flags & PARTICLE_LIT) {
                u8 color[4];

                func_1513137C(&primR, &primG, &primB, &primA, p);
                func_1515E888(viewIdx, (s32)pos.x, (s32)pos.y, (s32)pos.z, 0xFF, color, NULL, NULL);
                envR = color[0];
                envG = color[1];
                envB = color[2];
                envA = p->desc.alpha;
            } else {
                func_1513164C(&primR, &primG, &primB, &primA, &envR, &envG, &envB, &envA, p);
            }
            gfx = func_1513F4E4(gfx, shade, &needSync);
            gfx = func_15142B7C(gfx, p->desc.unk0, p->desc.unk4 | 0x10000 | 0x100000);
            gfx = func_15142CF0(gfx, 0, 0, primR, primG, primB, primA, &needSync);
            gfx = func_15142C10(gfx, envR, envG, envB, envA, &needSync);
            gfx = func_15142E24(gfx, &D_80090B60[p->desc.texture], p->desc.frame, 2, 0, 0, p->desc.texture, 0, 0, &needSync, (p->desc.flags & 0x800000) ? 0x3E : 3);
            gfx = func_15142FBC(gfx, ((p->desc.flags & 0x100) ? 0x100000 : 0) | D_800D2C9C | 0x2000 | 0xC00 | 0x80 | 0x20, D_800A4AC8[mode].unk0 | D_800A4AC8[mode].unk4 | 4, &needSync);
            gSPScisTextureRectangle(gfx++, (s32)ulx, (s32)uly, (s32)lrx, (s32)lry, 0, s, t, dsdx, dtdy);
        }
    }
end: // matching: unused label; without it IDO returns gfx without the t2 copy
    return gfx;
}

/* Picks a particle's primitive color for its primColorMode. */
void func_1513137C(s16 *r, s16 *g, s16 *b, s16 *a, Particle *p) {
    switch (p->desc.primColorMode) {
    case 1:
        *r = *g = *b = 0;
        *a = (p->desc.a * p->desc.alpha) >> 8;
        return;
    case 2:
        *r = p->desc.r;
        *g = p->desc.g;
        *b = p->desc.b;
        *a = (p->desc.a * p->desc.alpha) >> 8;
        return;
    case 3:
        *r = p->desc.r2;
        *g = p->desc.g2;
        *b = p->desc.b2;
        *a = (p->desc.a * p->desc.alpha) >> 8;
        return;
    case 4:
    case 6:
        *r = *g = *b = *a = 0;
        return;
    case 5:
        *r = *g = *b = 0;
        *a = p->desc.alpha;
        return;
    case 7:
        *r = p->desc.r;
        *g = p->desc.g;
        *b = p->desc.b;
        *a = p->desc.alpha;
        return;
    case 8:
        *r = p->desc.r;
        *g = p->desc.g;
        *b = p->desc.b;
        *a = 0;
        return;
    default:
        *r = (p->desc.r * p->desc.brightness) >> 8;
        *g = (p->desc.g * p->desc.brightness) >> 8;
        *b = (p->desc.b * p->desc.brightness) >> 8;
        *a = 0;
        return;
    }
}

/* Picks a particle's environment color for its envColorMode. */
void func_15131514(s16 *r, s16 *g, s16 *b, s16 *a, Particle *p) {
    u8 level;

    switch (p->desc.envColorMode) {
    case 1:
    case 5:
        *r = *g = *b = 0;
        *a = 0;
        return;
    case 2:
        level = p->desc.brightness;
        *b = level;
        *g = level;
        *r = level;
        *a = 0;
        return;
    case 8:
        *r = *g = *b = 0;
        *a = p->desc.alpha;
        return;
    case 3:
        *r = p->desc.r;
        *g = p->desc.g;
        *b = p->desc.b;
        *a = 0;
        return;
    case 4:
    case 6:
        *r = p->desc.r2;
        *g = p->desc.g2;
        *b = p->desc.b2;
        *a = p->desc.alpha;
        return;
    case 7:
        level = p->desc.brightness;
        *b = level;
        *g = level;
        *r = level;
        *a = p->desc.alpha;
        return;
    case 9:
        *r = p->desc.r;
        *g = p->desc.g;
        *b = p->desc.b;
        *a = p->desc.alpha;
        return;
    default:
        level = p->desc.brightness;
        *b = level;
        *g = level;
        *r = level;
        *a = (p->desc.a * p->desc.alpha) >> 8;
        return;
    }
}

/* Picks both of a particle's colors. */
void func_1513164C(s16 *primR, s16 *primG, s16 *primB, s16 *primA, s16 *envR, s16 *envG, s16 *envB, s16 *envA, Particle *p) {
    func_15131514(envR, envG, envB, envA, p);
    func_1513137C(primR, primG, primB, primA, p);
}

void func_151318E8(Vec3f *vel, f32 drag);

/* Particle update func: applies p->data.drag to the vertical velocity. */
s32 func_151316AC(Particle *p, Vec3f *prevPos) {
    func_151318E8(&p->desc.vel, p->data.drag);
    return TRUE;
}

void func_15131918(Vec3f *vel, f32 drag);

/* Particle update func: applies p->data.drag to the horizontal velocity. */
s32 func_151316DC(Particle *p, Vec3f *prevPos) {
    func_15131918(&p->desc.vel, p->data.drag);
    return TRUE;
}

void func_15131958(Vec3f *vel, f32 drag);

/* Particle update func: applies p->data.drag to the whole velocity. */
s32 func_1513170C(Particle *p, Vec3f *prevPos) {
    func_15131958(&p->desc.vel, p->data.drag);
    return TRUE;
}

void func_1513173C(Particle *p) {
    func_15169804(&p->header);
}

void func_1513175C(Particle *p) {
    func_15169824(&p->header);
}

extern void (*D_80089814[])(Particle *);

/* Calls the particle's D_80089814 handler (set 0 without PARTICLE_HANDLERS). */
void func_1513177C(Particle *p) {
    s32 set;

    if (p->desc.flags & PARTICLE_HANDLERS) {
        set = p->desc.handlerSet;
    } else {
        set = 0;
    }
    D_80089814[set](p);
}

extern s32 (*D_80089844[])(Particle *);

/* Calls the particle's D_80089844 handler (set 0 without PARTICLE_HANDLERS). */
s32 func_151317C8(Particle *p) {
    s32 set;

    if (p->desc.flags & PARTICLE_HANDLERS) {
        set = p->desc.handlerSet;
    } else {
        set = 0;
    }
    return D_80089844[set](p);
}

s32 func_15131814(s32 arg0, s32 arg1) {
    return 0;
}

/* Moves the particle's x/z offset around an ellipse with radii amplitude[0..1] as phase advances. */
void func_15131828(Particle *p, f32 *amplitude, u8 *phase, u8 *phaseStep) {
    p->desc.offset.x = amplitude[0] * func_151423D8((u8)(phase[0] - 0x40));
    p->desc.offset.z = amplitude[1] * func_151423D8((u8)(phase[1] - 0x40));
    phase[0] += phaseStep[0] * D_800BE9E4;
    phase[1] += phaseStep[1] * D_800BE9E4;
}

/* Scales vel->y by drag once per elapsed tick. */
void func_151318E8(Vec3f *vel, f32 drag) {
    s32 i;

    for (i = D_800BE9E4; i > 0; i--) {
        vel->y *= drag;
    }
}

/* Scales vel->x and vel->z by drag once per elapsed tick. */
void func_15131918(Vec3f *vel, f32 drag) {
    s32 i;

    for (i = D_800BE9E4; i > 0; i--) {
        vel->x *= drag;
        vel->z *= drag;
    }
}

/* Scales all of vel by drag once per elapsed tick. */
void func_15131958(Vec3f *vel, f32 drag) {
    s32 i;

    for (i = D_800BE9E4; i > 0; i--) {
        vel->x *= drag;
        vel->y *= drag;
        vel->z *= drag;
    }
}

s32 func_151319C4(Particle *p, Vec3f *prevPos, ParticleBounce *bounce);

/* Particle update func: floor collision using p->data.bounce. */
s32 func_151319A4(Particle *p, Vec3f *prevPos) {
    return func_151319C4(p, prevPos, &p->data.bounce);
}

extern s32 func_1514672C(Vec3f *);
extern s32 func_15046C80(Vec3f *, s32, f32, CollisionHit *);
extern s32 (*D_80089874[])(Particle *, Vec3f *, f32 *, ParticleBounce *);

/*
 * When a falling particle reaches the floor below it, calls its hit func with the floor point.
 * Returns FALSE if the particle should die.
 */
s32 func_151319C4(Particle *p, Vec3f *prevPos, ParticleBounce *bounce) {
    u8 alive;
    Vec3f probe;

    alive = TRUE;
    if ((bounce->flags & 1) && bounce->hitFunc != -1 && D_80089874[bounce->hitFunc] != NULL && p->desc.pos.y < prevPos->y) {
        probe.x = p->desc.pos.x;
        probe.y = prevPos->y;
        probe.z = p->desc.pos.z;
        if (func_1514672C(&probe) == 0) {
            return FALSE;
        }
        if (func_15046C80(&probe, 0, p->desc.pos.y - p->desc.height * 0.06000000238f, &p->unk80.floor) != 0) {
            f32 hit[3];

            hit[0] = probe.x;
            hit[1] = p->unk80.floor.height;
            hit[2] = probe.z;
            alive = D_80089874[bounce->hitFunc](p, prevPos, hit, bounce);
        }
    }
    return alive;
}

/* Particle update func: drag on the whole velocity, then floor collision. */
s32 func_15131AFC(Particle *p, Vec3f *prevPos) {
    func_15131958(&p->desc.vel, p->data.dragBounce.drag);
    return func_151319C4(p, prevPos, &p->data.dragBounce.bounce);
}

/* Particle update func: drag on the horizontal velocity, then floor collision. */
s32 func_15131B3C(Particle *p, Vec3f *prevPos) {
    func_15131918(&p->desc.vel, p->data.dragBounce.drag);
    return func_151319C4(p, prevPos, &p->data.dragBounce.bounce);
}


/* Floor hit func: bounces the particle off the floor, and stops it once the bounce is small. */
// matching: hit is indexed as f32[3]; vertex access swaps IDO's add.s operands
s32 func_15131B7C(Particle *p, Vec3f *prevPos, f32 *hit, ParticleBounce *bounce) {
    p->desc.pos.y = p->desc.height * 0.06000000238f + hit[1];
    p->desc.vel.x *= bounce->restitution;
    p->desc.vel.y *= -bounce->restitution;
    p->desc.vel.z *= bounce->restitution;
    if (fabsf(p->desc.vel.y) < 1.997000098f) {
        p->desc.vel.x = 0.0f;
        p->desc.vel.y = 0.0f;
        p->desc.vel.z = 0.0f;
        p->desc.gravity = 0.0f;
        bounce->flags &= ~1;
        p->desc.flags &= ~(PARTICLE_GRAVITY | PARTICLE_MOVE);
    }
    return TRUE;
}

extern void (*D_80089878[])(Particle *, s32, u8);

/* Calls the particle's D_80089878 handler, if it has one. */
void func_15131C2C(Particle *p, s32 arg1, u8 arg2) {
    void (*handler)(Particle *, s32, u8);

    if (p->desc.flags & PARTICLE_HANDLERS) {
        handler = D_80089878[p->desc.handlerSet];
        if (handler != NULL) {
            handler(p, arg1, arg2);
        }
    }
}

/* Advances phase[0..1] and sets *outX/*outY to base plus amplitude[i] times the phase's wave value. */
void func_15131C84(u8 *phase, s8 *phaseStep, f32 base, f32 *amplitude, f32 *outX, f32 *outY) {
    phase[0] += phaseStep[0] * D_800BE9E4;
    phase[1] += phaseStep[1] * D_800BE9E4;
    *outX = amplitude[0] * func_151423D8((u8)(phase[0] - 0x40)) + base;
    *outY = amplitude[1] * func_151423D8((u8)(phase[1] - 0x40)) + base;
}

/* Calls func_15169260 with the three particle object types (see func_15130280). */
void func_15131D4C(s32 arg0, u8 arg1) {
    IdList3 types;

    types = D_800A37F0;
    func_15169260(&types, 3, arg0, arg1);
}

/* Particle update func: pulses the particle's width and height (p->data.pulse). */
s32 func_15131D9C(Particle *p, Vec3f *prevPos) {
    func_15131C84(p->data.pulse.phase, p->data.pulse.phaseStep, p->data.pulse.baseSize, p->data.pulse.amplitude, &p->desc.width, &p->desc.height);
    return TRUE;
}

/* Particle update func: grows the particle with sqrt(time) and fades it quadratically; dies after duration. */
s32 func_15131DEC(Particle *p, Vec3f *prevPos) {
    f32 elapsed;
    f32 size;

    elapsed = p->data.flash.elapsed;
    size = p->data.flash.growth * sqrtf(elapsed);
    p->desc.height = size;
    p->desc.width = size;

    p->desc.alpha = (u32)(p->data.flash.startAlpha - p->data.flash.alphaDecay * elapsed * elapsed);
    p->data.flash.elapsed = elapsed + D_800BE9A4;

    if (p->data.flash.duration < p->data.flash.elapsed) {
        return FALSE;
    }
    return TRUE;
}

/* Spawns a flash particle (see func_15131DEC) at pos. */
Particle *func_15131EE4(Vec3f *pos, f32 size, u8 arg2, s32 arg3) {
    ParticleDesc desc;
    ParticleFlash flash;
    Particle *particle;
    s32 flipY;
    s32 flipX;

    flash.elapsed = 0.0f;
    flash.growth = 594.4899902f * size;
    flash.duration = func_150ADA68() * 2.0f + 5.0f;
    flash.startAlpha = func_150ADA68() * 15.0f + 240.0f;
    flash.alphaDecay = flash.startAlpha / (flash.duration * flash.duration);

    desc.alpha = (u32)flash.startAlpha;
    desc.texture = 0x69;
    desc.renderMode = 0x4417;
    desc.unk0 = 0x200004;
    desc.unk4 = 1;
    desc.frame = 0;
    desc.frameRate = 0;
    desc.r2 = 0xFF;
    desc.g2 = 0xFF;
    desc.b2 = 0xFF;
    desc.a = 0xFF;
    desc.r = 0xFF;
    desc.g = 0xFF;
    desc.b = 0xFF;
    desc.brightness = 0xFF;
    desc.pos = *pos;
    desc.offset = D_800A5480;
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.scaleStart = 1;
    desc.scaleRate = 1.0f;

    flipX = (func_150ADA20() & 1) ? PARTICLE_FLIP_S : 0;
    flipY = (func_150ADA20() & 1) ? PARTICLE_FLIP_T : 0;
    desc.flags = flipY | (PARTICLE_VIEWS | PARTICLE_DRAW_FUNC | PARTICLE_HANDLERS) | flipX;
    desc.primColorMode = 6;
    desc.envColorMode = 6;
    desc.updateFunc = 0x27;
    desc.deathFunc = -1;
    desc.drawFunc = -1;
    desc.handlerSet = 0;
    desc.viewMask = NULL;
    desc.views = 0xFF;
    desc.vel = D_800A5480;
    desc.timer = 300;
    desc.width = 0.0f;
    desc.height = 0.0f;
    desc.gravity = 0.0f;

    particle = func_15130280(&desc, 1, NULL, sizeof(flash), arg2, arg3);
    if (particle != NULL) {
        memcpy(&particle->data.flash, &flash, sizeof(flash));
    }
    return particle;
}
