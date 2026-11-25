#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Particle trails: a ring buffer of points emitted from a moving source and drawn as a ribbon. */

#define TRAIL_STOPPED   1 /* state->flags: no more points are emitted */
#define TRAIL_FADE_TAIL 2 /* state->flags: ramp alpha up from the oldest point */
#define TRAIL_FADE_HEAD 4 /* state->flags: ramp alpha up from the newest point */

#define TRAIL_ATTACHED  2 /* trail->flags: the ribbon starts at the emitter position */

/* One point of a trail's ring buffer. */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f vel;
    /* 0x18 */ f32 width;
    /* 0x1C */ u16 alpha;
    /* 0x1E */ s16 life;
    /* 0x20 */ u8 texS;
    /* 0x21 */ u8 pad21[3];
} TrailPoint; // size = 0x24

/* Per-trail settings and emitter state, stored behind trail->state. */
typedef struct {
    /* 0x00 */ Vec3f baseVel;
    /* 0x0C */ f32 gravity;
    /* 0x10 */ u8 phaseJitter;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 speedBase;
    /* 0x18 */ f32 speedSwing;
    /* 0x1C */ u8 flags;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ f32 emitRate;
    /* 0x24 */ u8 shade;
    /* 0x25 */ u8 pad25;
    /* 0x26 */ s16 alpha;
    /* 0x28 */ f32 widthGrowth;
    /* 0x2C */ f32 width;
    /* 0x30 */ u8 texStep;
    /* 0x31 */ u8 texStepJitter;
    /* 0x32 */ u8 fadeTailCount;
    /* 0x33 */ u8 fadeTailStep;
    /* 0x34 */ u8 fadeHeadCount;
    /* 0x35 */ u8 fadeHeadStep;
    /* 0x36 */ s16 life;
    /* 0x38 */ u8 burstPreset;
    /* 0x39 */ u8 pad39[3];
    /* 0x3C */ f32 burstRate;
    /* 0x40 */ f32 speed;
    /* 0x44 */ f32 emitAccum;
    /* 0x48 */ u8 texS;
    /* 0x49 */ u8 pad49[3];
    /* 0x4C */ f32 speedTarget;
    /* 0x50 */ u8 phase;
    /* 0x51 */ u8 pad51[3];
    /* 0x54 */ f32 burstAccum;
} TrailState; // size = 0x58

typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ Vec3f pos;     /* 0x10-0x2B are copied from the EffectTemplate header */
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u16 flags;
    /* 0x20 */ s32 kind;
    /* 0x24 */ u8 unk24;
    /* 0x25 */ u8 capacity; /* ring buffer size */
    /* 0x26 */ u8 pad26[6];
    /* 0x2C */ s8 count;
    /* 0x2D */ s8 tail; /* oldest point */
    /* 0x2E */ s8 head; /* next free slot */
    /* 0x2F */ u8 pad2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 pad31[0x23];
    /* 0x54 */ Vec3f tailPos;
    /* 0x60 */ u8 pad60[0x24];
    /* 0x84 */ u8 unk84[0x10];
    /* 0x94 */ TrailPoint *points;
    /* 0x98 */ TrailState *state;
} Trail;

extern void func_15147D64(s32, s32);

void func_150B3DD0(void) {
    u8 msg[4];

    func_15147D64(0, 5);
    msg[0] = 0;
    func_151494E0((s32)msg, 0x18);
    msg[0] = 2;
    func_151494E0((s32)msg, 0x18);
    msg[0] = 4;
    func_151494E0((s32)msg, 0x18);
    msg[0] = 1;
    func_151494E0((s32)msg, 0x18);
    msg[0] = 3;
    func_151494E0((s32)msg, 0x18);
    msg[0] = 5;
    func_151494E0((s32)msg, 0x18);
}

extern void func_1000FC18(s32, s16, s16, s16, s32);
extern void func_151478F4(void *);

void func_150B3E74(Trail *trail) {
    func_1000FC18(0x221, trail->pos.x, trail->pos.y, trail->pos.z, 4000);
    func_151478F4(trail);
}

extern void func_15147928(void *);

void func_150B3EE8(Trail *trail) {
    func_1000FC18(0x221, trail->pos.x, trail->pos.y, trail->pos.z, 4000);
    func_15147928(trail);
}

extern void *func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Spawns a trail at pos with the settings in params. Returns the new trail, or NULL. */
Trail *func_150B3F5C(TrailStateParams *params, Vec3f *pos, s16 capacity) {
    EffectTemplate spawn;
    TrailState state;
    Trail *trail;

    memcpy(&state, params, sizeof(TrailStateParams)); /* the settings, up to state.speed */
    state.emitAccum = 0.0f;
    spawn.itemCount = capacity;
    state.texS = 0;
    spawn.flags = TRAIL_ATTACHED;
    spawn.life = 500;
    state.speedTarget = params->speedBase + params->speedSwing;
    state.phase = 0;
    state.burstAccum = 0.0f;
    state.speed = state.speedTarget;
    spawn.pos = *pos;
    spawn.kind = 9;

    trail = func_15147A80(&spawn, sizeof(TrailState), sizeof(TrailPoint), 7, 7, 7, 0, 0, 0, 0xFF, 1);
    if (trail != NULL) {
        func_1000FA64(0x221, pos->x, pos->y, pos->z, 25000, 4000, 600, func_1000EF40, 0, 0, 8,
                      func_150ADA20() & 0x40);
        memcpy(trail->state, &state, sizeof(TrailState));
    }
    return trail;
}

/* Moves and ages the trail's points, dropping expired ones from the tail. Returns 0 once a stopped trail is empty. */
s32 func_150B40E8(Trail *trail) {
    TrailState *state;
    TrailPoint *points;
    s32 i;
    s32 expired;

    state = trail->state;
    points = trail->points;

    if ((trail->count < 2) && (state->flags & TRAIL_STOPPED)) {
        return 0;
    }

    i = trail->head;
    if (i != trail->tail) {
        do {
            i--;
            expired = FALSE;
            if (i < 0) {
                i = trail->capacity - 1;
            }

            points[i].alpha = 0xFF;
            points[i].life -= D_800BE9E4;
            if (points[i].life < 0) {
                expired = TRUE;
            }

            points[i].vel.y += state->gravity * D_800BE9A4;
            points[i].pos.x += points[i].vel.x * D_800BE9A4;
            points[i].pos.y += points[i].vel.y * D_800BE9A4;
            points[i].pos.z += points[i].vel.z * D_800BE9A4;
            points[i].width += state->widthGrowth * D_800BE9A4;

            if (expired && i != trail->tail) {
                do {
                    trail->tail++;
                    if (trail->tail == trail->capacity) {
                        trail->tail = 0;
                    }
                    trail->count--;
                } while (i != trail->tail);
            }
        } while (i != trail->tail);
    }

    if (trail->count > 0) {
        trail->tailPos = points[trail->tail].pos;
    } else {
        trail->tailPos.x = 0.0f;
        trail->tailPos.y = 0.0f;
        trail->tailPos.z = 0.0f;
    }

    return 1;
}

void func_150B5088(Trail *trail);

/* Emits new trail points for this frame, interpolating speed across the frame's step. */
s32 func_150B4294(Trail *trail) {
    f32 inv;
    f32 dt;
    f32 speed;
    TrailState *state;
    TrailPoint *points;
    Vec3f baseVel;
    f32 diff;
    f32 dtStep;
    f32 speedStep;

    state = trail->state;
    points = trail->points;
    state->phase += (func_150ADA20() % (u32)(state->phaseJitter + 1)) * D_800BE9E4;
    state->speedTarget = func_151423D8(state->phase - 0x40) * state->speedSwing + state->speedBase;
    state->emitAccum += state->emitRate * D_800BE9A4;
    if (state->emitAccum > 1.0f) {
        dt = D_800BE9A4;
        inv = 1.0f / state->emitAccum;
        baseVel = state->baseVel;
        dtStep = D_800BE9A4 * inv;
        speed = state->speed;
        diff = state->speedTarget - state->speed;
        speedStep = diff * inv;
        do {
            state->burstAccum += state->burstRate;
            if (state->burstAccum > 1.0f) {
                func_150B5088(trail);
            }
            points[trail->head].pos = trail->pos;
            points[trail->head].vel.x = baseVel.x * speed;
            points[trail->head].vel.y = baseVel.y * speed;
            points[trail->head].vel.z = baseVel.z * speed;
            points[trail->head].width = state->width;
            points[trail->head].alpha = 0xFF;
            points[trail->head].life = state->life;
            points[trail->head].texS = state->texS;
            state->texS += state->texStep + func_150ADA68() * state->texStepJitter;
            points[trail->head].vel.y += state->gravity * dt;
            points[trail->head].pos.x += points[trail->head].vel.x * dt;
            points[trail->head].pos.y += points[trail->head].vel.y * dt;
            points[trail->head].pos.z += points[trail->head].vel.z * dt;
            trail->head++;
            if (trail->head == trail->capacity) {
                trail->head = 0;
            }
            trail->count++;
            if (trail->head == trail->tail) {
                /* buffer full: drop the oldest point */
                trail->tail++;
                if (trail->tail == trail->capacity) {
                    trail->tail = 0;
                }
                trail->count--;
            }
            speed += speedStep;
            dt -= dtStep;
            state->emitAccum -= 1.0f;
        } while (state->emitAccum > 1.0f);
        state->speed = speed;
    }
    return 1;
}

extern u8 D_80091238[];
extern s32 D_800D2C9C;
void func_151D5D60(void *, s16, s32, void **, u8 *);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);

/*
 * Draws the trail as a camera-facing ribbon, newest point first: each pair of points becomes a quad
 * whose sides are offset by the point's width along (segment x to-camera).
 */
Gfx *func_150B4710(Trail *trail, Gfx *gfx, s16 camIdx) {
    Vtx *vtx;
    TrailState *state;
    Vec3f next;
    Vec3f cur;
    s32 j;
    s32 i;
    TrailPoint *points;
    f32 *cam;
    f32 cx;
    f32 cy;
    f32 cz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 lenSq;
    u8 texS;
    u8 prevTexS;
    s16 alpha;
    s16 prevAlpha;
    u8 update;
    u8 fade;
    s16 count;
    s32 k;
    f32 scale;
    f32 nextScale;

    if (trail->count >= 2) {
        func_151D5D60(trail->unk84, camIdx, (trail->capacity * 2 + 10) * sizeof(Vtx), (void **)&vtx, NULL);
        if (vtx != NULL) {
            cam = &D_800DBFF0[camIdx].eye.x;
            state = trail->state;
            points = trail->points;
            update = 1;
// matching: the label starts a new basic block, keeping the state loads below it
lbl1:
            if (state->flags & TRAIL_FADE_TAIL) {
                u8 fade1;
                s16 count1;

                fade1 = 0;
                count1 = state->fadeTailCount;
                k = trail->tail;
                do {
                    points[k].alpha = fade1;
                    fade1 += state->fadeTailStep;
                    if (++k == trail->capacity) k = 0;
                } while (--count1 != 0 && k != trail->head);
            }
            if (state->flags & TRAIL_FADE_HEAD) {
                fade = 0;
                count = state->fadeHeadCount;
                k = trail->head - 1;
                if (k < 0) {
                    k = trail->capacity - 1;
                }
                do {
                    points[k].alpha = fade;
                    fade += state->fadeHeadStep;
                    count--;
                    k--;
                    if (k < 0) {
                        k = trail->capacity - 1;
                    }
                } while (count != 0 && k != trail->head);
            }
            gfx = func_15142E24(gfx, (s32)D_80091238, 0, 0, 0, 0, 0x92, 0, 0, &update, 3);
            gfx = func_15142B7C(gfx, 1, 0x160600);
            gfx = func_1513F4E4(gfx, 0x4D, &update);
            gfx = func_15142C10(gfx, state->shade, state->shade, state->shade, 0xFF, &update);
            gfx = func_15142FBC(gfx, D_800D2C9C | G_TP_PERSP | G_TF_BILERP | G_TC_FILT | G_CK_NONE | G_CD_NOISE | G_AD_NOISE, 0x5049D8, &update);
            if (trail->flags & TRAIL_ATTACHED) {
                j = trail->head - 1;
                if (j < 0) {
                    j = trail->capacity - 1;
                }
                cur = trail->pos;
                prevTexS = points[j].texS;
                prevAlpha = (points[j].alpha * state->alpha) >> 8;
            } else {
                i = trail->head - 1;
                if (i < 0) {
                    i = trail->capacity - 1;
                }
                j = i - 1;
                if (j < 0) {
                    j = trail->capacity - 1;
                }
                cur = points[i].pos;
                prevTexS = points[i].texS;
                prevAlpha = (points[i].alpha * state->alpha) >> 8;
            }
            next = points[j].pos;
            texS = points[j].texS;
            alpha = (points[j].alpha * state->alpha) >> 8;
            dx = cur.x - next.x;
            dy = cur.y - next.y;
            dz = cur.z - next.z;
            cx = cur.x - cam[0];
            cy = cur.y - cam[1];
            cz = cur.z - cam[2];
            nx = dy * cz - cy * dz;
            ny = dz * cx - cz * dx;
            nz = dx * cy - cx * dy;
            lenSq = nx * nx + ny * ny + nz * nz;
            if (lenSq == 0.0f) {
                nx = 0.0f;
                ny = 0.0f;
                nz = 0.0f;
            } else {
                scale = points[j].width / sqrtf(lenSq);
                nx = nx * scale;
                ny = ny * scale;
                nz = nz * scale;
            }
            vtx->v.ob[0] = (s16)(cur.x + nx);
            vtx->v.ob[1] = (s16)(cur.y + ny);
            vtx->v.ob[2] = (s16)(cur.z + nz);
            vtx->v.tc[0] = points[j].texS << 6;
            vtx->v.tc[1] = 0x43C0;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            vtx->v.ob[0] = (s16)(cur.x - nx);
            vtx->v.ob[1] = (s16)(cur.y - ny);
            vtx->v.ob[2] = (s16)(cur.z - nz);
            vtx->v.tc[0] = points[j].texS << 6;
            vtx->v.tc[1] = 0x4000;
            vtx->v.cn[0] = 0xFF;
            vtx->v.cn[1] = 0xFF;
            vtx->v.cn[2] = 0xFF;
            vtx->v.cn[3] = prevAlpha;
            vtx->v.flag = 0;
            vtx++;
            do {
                dx = cur.x - next.x;
                dy = cur.y - next.y;
                dz = cur.z - next.z;
                cx = next.x - cam[0];
                cy = next.y - cam[1];
                cz = next.z - cam[2];
                nx = dy * cz - cy * dz;
                ny = dz * cx - cz * dx;
                nz = dx * cy - cx * dy;
                lenSq = nx * nx + ny * ny + nz * nz;
                if (lenSq == 0.0f) {
                    nx = 0.0f;
                    ny = 0.0f;
                    nz = 0.0f;
                } else {
                    nextScale = points[j].width / sqrtf(lenSq);
                    nx = nx * nextScale;
                    ny = ny * nextScale;
                    nz = nz * nextScale;
                }
                vtx->v.ob[0] = (s16)(next.x + nx);
                vtx->v.ob[1] = (s16)(next.y + ny);
                vtx->v.ob[2] = (s16)(next.z + nz);
                vtx->v.tc[0] = points[j].texS << 6;
                vtx->v.tc[1] = 0x43C0;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                vtx->v.ob[0] = (s16)(next.x - nx);
                vtx->v.ob[1] = (s16)(next.y - ny);
                vtx->v.ob[2] = (s16)(next.z - nz);
                vtx->v.tc[0] = points[j].texS << 6;
                vtx->v.tc[1] = 0x4000;
                vtx->v.cn[0] = 0xFF;
                vtx->v.cn[1] = 0xFF;
                vtx->v.cn[2] = 0xFF;
                vtx->v.cn[3] = alpha;
                vtx->v.flag = 0;
                vtx++;
                gSPVertex(gfx++, vtx - 4, 4, 0);
                gSP1Triangle(gfx++, 0, 1, 2, 0);
                gSP1Triangle(gfx++, 1, 3, 2, 0);
                if (prevTexS < texS) {
                    memcpy(vtx, vtx - 2, 2 * sizeof(Vtx));
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                    vtx[-2].v.tc[0] -= 0x4000;
                    vtx++;
                }
                i = j;
                j--;
                if (j < 0) {
                    j = trail->capacity - 1;
                }
                prevTexS = texS;
                cur = points[i].pos;
                next = points[j].pos;
                alpha = (points[j].alpha * state->alpha) >> 8;
                texS = points[j].texS;
            } while (i != trail->tail);
        }
    }
    return gfx;
}

/* Stops the trail: no more points are emitted and the ribbon detaches from the emitter. */
void func_150B5060(Trail *trail) {
    TrailState *state = trail->state;

    trail->unk30 = 0;
    trail->flags &= ~TRAIL_ATTACHED;
    state->flags |= TRAIL_STOPPED;
}

/* Settings for one burst of particles, selected by TrailState.burstPreset. */
typedef struct {
    /* 0x00 */ f32 radius;
    /* 0x04 */ f32 speed;
    /* 0x08 */ f32 speedRange;
    /* 0x0C */ f32 unkC;  /* particle.unk0 = unkC + rand * unk10 */
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14; /* particle.unk10 = unk14 + rand * unk18 */
    /* 0x18 */ f32 unk18;
    /* 0x1C */ s16 unk1C; /* spawn.unkC = unk1C + rand % (unk1E + 1) */
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s16 capacity;
    /* 0x22 */ s16 capacityRange;
} TrailBurstPreset; // size = 0x24

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ Vec3f vel;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 shade;
    /* 0x1B */ u8 alpha;
    /* 0x1C */ u8 pad1C[4];
} BurstParticle; // size = 0x20

extern TrailBurstPreset D_8009FBF0[];
extern s32 func_15147DA0(EffectTemplate *, BurstParticle *, s32, s32, s32, s32, s32, s32, s32, s32, s32, TrailRender *, s32, s32, s32);

/* Throws out a burst of particles around the trail's emitter, one per whole unit of burstAccum. */
void func_150B5088(Trail *trail) {
    TrailRender style;
    TrailState *state;
    EffectTemplate spawn;
    BurstParticle particle;
    f32 x;
    f32 y;
    f32 z;
    f32 speed;
    f32 cosA;
    f32 sinA;
    f32 cosB;
    f32 sinB;
    u8 angA;
    u8 angB;
    TrailBurstPreset *preset;

    state = trail->state;
    preset = &D_8009FBF0[state->burstPreset];

    style.unk0 = 0;
    style.setGeometryMode = 1;
    style.clearGeometryMode = 0x160600;
    style.renderMode = 3;
    style.unk10 = 16;
    style.unk14 = 128;
    style.unk18 = 32;
    style.primMode = 0;
    style.envMode = 9;
    spawn.flags = 1;
    particle.unk19 = 6;
    particle.shade = state->shade;
    particle.unk18 = 8;
    particle.alpha = state->alpha;
    spawn.pos = trail->pos;

    do {
        speed = func_150ADA68() * preset->speedRange + preset->speed;
        angA = func_150ADA20() & 0xFF;
        angB = func_150ADA20() & 0xFF;
        cosA = func_151423D8(angA - 0x40);
        sinA = func_151423D8(angA);
        cosB = func_151423D8(angB - 0x40);
        sinB = func_151423D8(angB);
        x = state->baseVel.x + preset->radius * sinB * cosA;
        y = state->baseVel.y - preset->radius * cosB;
        z = state->baseVel.z + preset->radius * sinB * sinA;
        spawn.itemCount = (func_150ADA20() % (preset->capacityRange + 1)) + preset->capacity;
        spawn.life = (func_150ADA20() % (preset->unk1E + 1)) + preset->unk1C;
        particle.unk0 = func_150ADA68() * preset->unk10 + preset->unkC;
        particle.unk10 = func_150ADA68() * preset->unk18 + preset->unk14;
        particle.vel.x = speed * x;
        particle.vel.y = speed * y;
        particle.vel.z = speed * z;
        func_15147DA0(&spawn, &particle, 0, 1, 7, 0, 0, 0, 0, 0, 0, &style, 0, trail->unkC, trail->unk1);
        state->burstAccum -= 1.0f;
    } while (state->burstAccum > 1.0f);
}

/* Message handler: message 5 stops the trail. */
void func_150B538C(Trail *trail, s32 arg1, u8 msg) {
    if (msg == 5) {
        func_150B5060(trail);
    }
}
